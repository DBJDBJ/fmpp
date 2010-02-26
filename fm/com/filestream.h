//*****************************************************************************/
//
//                  
//
//                 Copyright (c)  2000-2006 by DBJSolutions LTD 
//                          All Rights Reserved
//
//        THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF DBJSolutions LTD
//
//        The copyright notice above does not evidence any
//        actual or intended publication of such source code.
//
//  $Author: DBJ  $
//  $Date: $
//  $Revision: $
//*****************************************************************************/


// FileStream.h
// Copyright (c) 1998 Valery Pryamikov.

#pragma once 

#ifndef __FILE_STREAM_H__
#define __FILE_STREAM_H__

namespace dbjsys {
	namespace fm {
//--------------------------------------------------------------------------------------------


// ThreadingModel = Both
// 
class __declspec(uuid("{3AE7B7AF-1C79-48ca-87E2-793B6CEDC2E4}")) FileStream: public IStream
{
private: 
	//NOTE:
	//all members of the class FileStream are declared as private, 
	//for prohibiting the direct using of this class beside its IStream interface implementation
	// in any other place than two friend functions GetFileHandleFromStream and CreateStreamOnFileHandle, 
	// 
	STDMETHODIMP QueryInterface(REFIID riid, void **ppvObject) {
		if (!ppvObject) 
            return E_POINTER;

		*ppvObject = NULL;
		if (riid == IID_IUnknown || riid == IID_IStream || riid == __uuidof(FileStream))
			return reinterpret_cast<IUnknown*>(*ppvObject = static_cast<IStream*>(this))->AddRef(), S_OK;

		if (riid != IID_IMarshal || !bool(pftm)) 
            return E_NOINTERFACE;

		return pftm->QueryInterface(riid,  ppvObject);
	}

	// 
	STDMETHODIMP_(ULONG) AddRef( ) {return InterlockedIncrement(&dwRef);}
	// 
	STDMETHODIMP_(ULONG) Release() {
		DWORD dw = InterlockedDecrement(&dwRef);
		if (!dw) 
			delete this;
		return dw;
	}

	// 
	STDMETHODIMP Read(void *pv, ULONG cb, ULONG *pcbRead) {
		if (!pv) return E_INVALIDARG;
		ULONG cbSafeRead = 0;
		if (!pcbRead) pcbRead = &cbSafeRead;
		HRESULT hr;
		if (!ReadFile(hFile, pv, cb, pcbRead, NULL))
			return (((hr = GetLastError())==ERROR_HANDLE_EOF)?S_FALSE:(FAILED(hr)?hr:HRESULT_FROM_WIN32(hr)));

		return (cb == *pcbRead)?S_OK:S_FALSE;
	}

	// 
	STDMETHODIMP Write(const void *pv, ULONG cb, ULONG *pcbWritten) {
		if (!pv) return E_INVALIDARG;
		ULONG cbSafeWritten = 0;
		if (!pcbWritten) pcbWritten = &cbSafeWritten;
		HRESULT hr;
		if (!WriteFile(hFile, pv, cb, pcbWritten, NULL))
			return (FAILED(hr = GetLastError())?hr:HRESULT_FROM_WIN32(hr));

		return (cb == *pcbWritten)?S_OK:S_FALSE;
	}
	// 
	STDMETHODIMP Seek(LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER *plibNewPosition) {
		ULARGE_INTEGER safeNewPos;
		if (!plibNewPosition) plibNewPosition = &safeNewPos;
		DWORD seektype = 0;

		if (((dlibMove.QuadPart<0)?-1*dlibMove.QuadPart:dlibMove.QuadPart) > 0x7FFFFFFF)
			return E_INVALIDARG; //don't support that large files.
		switch (dwOrigin) {
		case STREAM_SEEK_SET:
			seektype = FILE_BEGIN;
			break;
		case STREAM_SEEK_CUR:
			seektype = FILE_CURRENT;
			break;
		case STREAM_SEEK_END:
			seektype = FILE_END;
			break;
		default:
			return E_INVALIDARG;
		}
		if ((plibNewPosition->QuadPart = 
			(unsigned __int64)(SetFilePointer(hFile, long(dlibMove.QuadPart), NULL, seektype))
			,plibNewPosition)->LowPart == 0xFFFFFFFF)
			return (FAILED(seektype=GetLastError())?seektype:HRESULT_FROM_WIN32(seektype));
		return S_OK;
	}
	// 
    STDMETHODIMP SetSize(ULARGE_INTEGER libNewSize) {
		if (libNewSize.QuadPart > 0x7FFFFFFF)
			return E_INVALIDARG; //don't support that large files.
		HRESULT hr;
		DWORD curpos = SetFilePointer(hFile, 0, NULL, FILE_CURRENT);
		if (curpos == 0xFFFFFFFF)
			return (FAILED(hr=GetLastError())?hr:HRESULT_FROM_WIN32(hr));
		DWORD filelen = SetFilePointer(hFile, 0, NULL, FILE_END);
		if (filelen == 0xFFFFFFFF)
			return (FAILED(hr=GetLastError())?hr:HRESULT_FROM_WIN32(hr));
		if (filelen-curpos>= libNewSize.LowPart) 
			return (SetFilePointer(hFile, curpos, NULL, FILE_BEGIN), S_OK);
		if (SetFilePointer(hFile, libNewSize.LowPart, NULL, FILE_BEGIN) == 0xFFFFFFFF)
			return (FAILED(hr=GetLastError())?hr:HRESULT_FROM_WIN32(hr));
		if (!SetEndOfFile(hFile)) {
			(FAILED(hr=GetLastError())?hr:HRESULT_FROM_WIN32(hr));
			SetFilePointer(hFile, curpos, NULL, FILE_BEGIN); //try to restore position
			return hr;
		}
		if (SetFilePointer(hFile, curpos, NULL, FILE_BEGIN) == 0xFFFFFFFF)
			return (FAILED(curpos=GetLastError())?curpos:HRESULT_FROM_WIN32(curpos));
		return S_OK;
	}

	// 
    STDMETHODIMP CopyTo(IStream *pstm, ULARGE_INTEGER cb, ULARGE_INTEGER *pcbRead, ULARGE_INTEGER *pcbWritten) {
		if (!pstm) return E_INVALIDARG;
		ULARGE_INTEGER cbSafeRead, cbSafeWritten;
		if (!pcbWritten) pcbWritten = &cbSafeWritten;
		if (!pcbRead) pcbRead = &cbSafeRead;

		if (cb.QuadPart > 0x7FFFFFFF)
			return E_INVALIDARG; //don't support that large files.
		pcbWritten->QuadPart = pcbRead->QuadPart = 0;
		DWORD nBufferSize = min(cb.LowPart, DWORD(MAX_READ_BUFFER_SIZE));
		LPBYTE buffer = new BYTE[nBufferSize];
		if (!buffer) return E_OUTOFMEMORY;
		__try {
			HRESULT hr;
			while (pcbWritten->QuadPart == pcbRead->QuadPart && pcbRead->QuadPart < cb.QuadPart) {
				nBufferSize = min(nBufferSize, cb.LowPart-pcbRead->LowPart);
				DWORD dwRead = 0;
				if (!ReadFile(hFile, buffer, nBufferSize, &dwRead, NULL)) do {
					if (((hr = GetLastError())==ERROR_HANDLE_EOF) && dwRead) break;
					else if (hr == ERROR_HANDLE_EOF) return S_FALSE;
					else return FAILED(hr)?hr:HRESULT_FROM_WIN32(hr);
				} while(false);
				pcbRead->QuadPart+=dwRead;
				DWORD dwWritten = 0;
				hr = S_OK;
				if (FAILED(hr = pstm->Write(buffer, dwRead, &dwWritten))) return hr;
				pcbWritten->QuadPart+=dwWritten;
				if (dwRead != nBufferSize) return (FAILED(hr)?hr:S_FALSE);
			}
			return S_OK;
		} __finally {
			if (buffer) delete[] buffer;
		}
		return E_UNEXPECTED;
	}
	// 
    STDMETHODIMP Commit(DWORD) {
        return (FlushFileBuffers(hFile),S_OK);
    }
	// 
    STDMETHODIMP Revert() {
        return S_FALSE;
    } // has not effect

	// 
    STDMETHODIMP LockRegion(ULARGE_INTEGER , ULARGE_INTEGER , DWORD ) {
        /*
        BOOL LockFileEx(
          HANDLE hFile,                   // handle to file
          DWORD dwFlags,                  // lock options
          DWORD dwReserved,               // reserved
          DWORD nNumberOfBytesToLockLow,  // low-order word of length 
          DWORD nNumberOfBytesToLockHigh, // high-order word of length
          LPOVERLAPPED lpOverlapped       // contains starting offset
        );*/
        return STG_E_INVALIDFUNCTION ;
    }
	// 
    STDMETHODIMP UnlockRegion(ULARGE_INTEGER , ULARGE_INTEGER , DWORD ) {
        /*
        BOOL UnlockFileEx(
          HANDLE hFile,                     // handle to file
          DWORD dwReserved,                 // reserved
          DWORD nNumberOfBytesToUnlockLow,  // low-order part of length
          DWORD nNumberOfBytesToUnlockHigh, // high-order part of length
          LPOVERLAPPED lpOverlapped         // unlock region start
        );*/
        return STG_E_INVALIDFUNCTION ;
    }
	// 
    STDMETHODIMP Stat(STATSTG *, DWORD ) {
        /*
        BOOL GetFileInformationByHandle(
          HANDLE hFile,                                  // handle to file 
          LPBY_HANDLE_FILE_INFORMATION lpFileInformation // buffer
        );
        */
        return E_NOTIMPL;
    }
	// 
    STDMETHOD(Clone)(IStream ** ppStream ) {
        if( ppStream == 0 )
            return E_POINTER ;

        if( ppStream != 0 )
            return E_INVALIDARG ;
        
        // return a copy of this (FileStream)
        return E_NOTIMPL ;
    }

	enum CONSTS {
		MAX_READ_BUFFER_SIZE = 0xFFFF //
	};

private:
	// 
	long dwRef;
	// 
	HANDLE hFile;
	// 
	IUnknownPtr pftm;
	// 
	FileStream(HANDLE hFile, HRESULT &hr/*error code from CoCreateFreeThreadedMarshaler or DuplicateHandle if any of them failed*/) {
		dwRef = 1;
		FileStream::hFile = INVALID_HANDLE_VALUE;
		if (FAILED(hr = CoCreateFreeThreadedMarshaler(this, &pftm ))) return;
		else if (!DuplicateHandle(
			GetCurrentProcess(), 
			hFile, //CreateStreamOnFileHandle checks that hFile is not INVALID_HANDLE_VALUE.
			GetCurrentProcess(), 
			&(FileStream::hFile), 
			0, TRUE, DUPLICATE_SAME_ACCESS))
			hr = (FAILED(hr = GetLastError())?hr:HRESULT_FROM_WIN32(hr));
	}

	// 
	~FileStream() {
		if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);
	}

	// 
    FileStream(const FileStream &) ;
	// 
    FileStream & operator=( const FileStream & ) ;

public:

    typedef _com_error Err ;

	// 
	static HRESULT CreateStreamOnFileHandle(HANDLE hFile, BOOL fTakeOwnership, IStream **ppStream)
	{
		if (!ppStream) return E_POINTER;
		if (hFile == INVALID_HANDLE_VALUE) return E_INVALIDARG;
		HRESULT hr;
		FileStream *pstream = new FileStream(hFile, hr);
		if (!pstream) return E_OUTOFMEMORY;
		if (FAILED(hr)) return (pstream->dwRef = 1, pstream->Release(), hr);
		if (fTakeOwnership) CloseHandle(hFile);
		*ppStream = pstream;
		return S_OK;
	}

	//if GetFileHandleFromStream succeeded, than received handle MUST be closed by call to CloseHandle function.
	// 
	static HRESULT GetFileHandleFromStream(IStream *pStream, DWORD dwDesiredAccess/*=0 for using source handle's access*/, HANDLE* phFile)
	{
		if (!phFile) return E_POINTER;
		*phFile = INVALID_HANDLE_VALUE;
		if (!pStream) return E_INVALIDARG;
		FileStream *pstm = NULL;
		if (FAILED(pStream->QueryInterface(__uuidof(FileStream), (void**)&pstm)))
			return E_INVALIDARG;
		if (pstm->hFile == INVALID_HANDLE_VALUE) return E_UNEXPECTED;
		HRESULT hr;
		if (!DuplicateHandle(GetCurrentProcess(), pstm->hFile, 
			GetCurrentProcess(), phFile, dwDesiredAccess, TRUE, (!dwDesiredAccess)?DUPLICATE_SAME_ACCESS:0))
			return (FAILED(hr = GetLastError())?hr:HRESULT_FROM_WIN32(hr));
		return S_OK;
	}


	// 
	static HRESULT WriteToStream(const _bstr_t & bstr, IStream* pStream)
	{
		_ASSERT(pStream != NULL);
		ULONG cb;
		ULONG cbStrLen = bstr.length() * sizeof(wchar_t);
		HRESULT hr = pStream->Write((void*) &cbStrLen, sizeof(cbStrLen), &cb);
		if (FAILED(hr))
			return hr;
		return cbStrLen ? pStream->Write((void*) (const wchar_t*)bstr, cbStrLen, &cb) : S_OK;
	}
	// 
	static HRESULT ReadFromStream(IStream* pStream, _bstr_t & bstr)
	{
		_ASSERT(pStream != NULL);
		_ASSERT(bstr.length() == 0); // should be empty
		ULONG cbStrLen = 0;
		HRESULT hr = pStream->Read((void*) &cbStrLen, sizeof(cbStrLen), NULL);
		if ((hr == S_OK) && (cbStrLen != 0))
		{
			BSTR str = SysAllocStringByteLen(NULL, cbStrLen);
			if (str == NULL)
				hr = E_OUTOFMEMORY;
			else
				hr = pStream->Read((void*) str, cbStrLen, NULL);

			bstr.Assign( str ) ;
		}

		if (hr == S_FALSE)
			hr = E_FAIL;
		return hr;
	}
	// 
	static HRESULT WriteToStream(const _variant_t & var, IStream* pStream)
	{
		HRESULT hr = pStream->Write(&var.vt, sizeof(VARTYPE), NULL);
		if (FAILED(hr))
			return hr;

		int cbWrite = 0;
		switch (var.vt)
		{
		case VT_UNKNOWN:
		case VT_DISPATCH:
			{
				IPersistStreamPtr spStream;
				if (var.punkVal != NULL)
				{
					hr = var.punkVal->QueryInterface(IID_IPersistStream, (void**)&spStream);
					if (FAILED(hr))
						return hr;
				}
				if (spStream != NULL)
					return OleSaveToStream(spStream, pStream);
				else
					return WriteClassStm(pStream, CLSID_NULL);
			}
		case VT_UI1:
		case VT_I1:
			cbWrite = sizeof(BYTE);
			break;
		case VT_I2:
		case VT_UI2:
		case VT_BOOL:
			cbWrite = sizeof(short);
			break;
		case VT_I4:
		case VT_UI4:
		case VT_R4:
		case VT_INT:
		case VT_UINT:
		case VT_ERROR:
			cbWrite = sizeof(long);
			break;
		case VT_R8:
		case VT_CY:
		case VT_DATE:
			cbWrite = sizeof(double);
			break;
		case VT_ARRAY:
			_ASSERT(0) ;	// TODO
		default:
			break;
		}
		if (cbWrite != 0)
			return pStream->Write((void*) &var.bVal, cbWrite, NULL);

		_bstr_t bstrWrite;
		bstrWrite = var;

		return WriteToStream( bstrWrite, pStream);
	}

	// 
	static HRESULT ReadFromStream(IStream* pStream, _variant_t & var )
	{
		_ASSERT(pStream != NULL);
		HRESULT hr;
		VARTYPE vtRead;
		hr = pStream->Read(&vtRead, sizeof(VARTYPE), NULL);
		if (hr == S_FALSE)
			hr = E_FAIL;
		if (FAILED(hr))
			return hr;

		int cbRead = 0;
		switch (vtRead)
		{
		case VT_UNKNOWN:
		case VT_DISPATCH:
			{
				IUnknown* pUnk = NULL;
				hr = OleLoadFromStream(pStream,
					(vtRead == VT_UNKNOWN) ? IID_IUnknown : IID_IDispatch,
					(void**)&pUnk);
				if( SUCCEEDED(hr) )
					var = _variant_t(pUnk, false) ;
				if (hr == REGDB_E_CLASSNOTREG)
					hr = S_OK;
				return hr;
			}
		case VT_UI1:
		case VT_I1:
			cbRead = sizeof(BYTE);
			break;
		case VT_I2:
		case VT_UI2:
		case VT_BOOL:
			cbRead = sizeof(short);
			break;
		case VT_I4:
		case VT_UI4:
		case VT_R4:
		case VT_INT:
		case VT_UINT:
		case VT_ERROR:
			cbRead = sizeof(long);
			break;
		case VT_R8:
		case VT_CY:
		case VT_DATE:
			cbRead = sizeof(double);
			break;
		case VT_ARRAY:
			_ASSERT(0) ;	// TODO
		default:
			break;
		}
		if (cbRead != 0)
		{
			VARIANT v ;
			::VariantInit( &v ) ;
			hr = pStream->Read((void*) &v.bVal, cbRead, NULL);
			if (hr == S_FALSE)
				hr = E_FAIL;

			var.Attach( v ) ;
			return hr;
		}

		_bstr_t bstrRead;

		hr = ReadFromStream(pStream, bstrRead);
		if (FAILED(hr))
			return hr;
		
		var = bstrRead ;

		return hr;
	}

private:
	class __impl
	{
	public:
		__impl() : hFile_(INVALID_HANDLE_VALUE)
		{
		}

		__impl( const __impl & o ) :
			stream_(o.stream_),
			hFile_(duplicateHandle(o.hFile_))
		{
		}

		__impl & operator=( const __impl & o )
		{
			if( this != &o )
			{
				stream_ = o.stream_ ;
				hFile_ = duplicateHandle( o.hFile_ ) ;
			}
			return *this ;
		}

        ~__impl()
        {
            close() ;
        }

		void close()
		{
			if( bool(stream_) )
			{
				HRESULT hr = stream_->Commit(0) ;
				dbjCOMVERIFY(hr) ;
			}
			stream_ = 0 ;

			if( hFile_ != INVALID_HANDLE_VALUE)
            {
                if( ! ::CloseHandle( hFile_ ) )     
                    throw Err( HRESULT_FROM_WIN32( ::GetLastError() ) ) ;
                hFile_  = INVALID_HANDLE_VALUE ;
            }
		}

		bool is_open() const
		{
			return stream_.GetInterfacePtr() ? true : false ;
		}

		void open( const _bstr_t & filename, DWORD access, DWORD open, DWORD share ) 
		{	
			// open the file
			hFile_ = CreateFile(filename, access, share, NULL, open, FILE_ATTRIBUTE_NORMAL, NULL) ;
			if( hFile_ ==  INVALID_HANDLE_VALUE ) 
                throw Err( HRESULT_FROM_WIN32( ::GetLastError() ) ) ;

			// create a stream on the file
			dbjCOMVERIFY( FileStream::CreateStreamOnFileHandle(hFile_, FALSE, &stream_ ) ) ;
		}

		_variant_t read()
		{
			_variant_t var ;
			dbjCOMVERIFY( FileStream::ReadFromStream( stream_, var ) ) ;
			return var ;
		} 

		void read( void * dest, size_t size )
		{
			_ASSERT( dest ) ;
			ULONG actual ;
			dbjCOMVERIFY( stream_->Read( dest, (unsigned long)size, &actual ) ) ;
			_ASSERT( actual == size ) ;
		}

		void write( const _variant_t & var )
		{
			dbjCOMVERIFY( FileStream::WriteToStream( var, stream_ ) ) ;
		}
		
		void write( const void * src, size_t size )
		{
			_ASSERT( src ) ;
			ULONG actual ;
			dbjCOMVERIFY( stream_->Write( src, (unsigned long)size, &actual ) ) ;
			_ASSERT( actual == size ) ;
		}

		void seek( DWORD distance, DWORD method ) 
		{
			DWORD seektype ;
			switch( method )
			{
			case FILE_BEGIN:
				seektype = STREAM_SEEK_SET;
				break;
			case FILE_CURRENT:
				seektype = STREAM_SEEK_CUR;
				break;
			case FILE_END:
				seektype = STREAM_SEEK_END;
				break;
			default:
				_ASSERT(0) ;
			}

			LARGE_INTEGER dis ;
			dis.QuadPart = distance ;
			dbjCOMVERIFY( stream_->Seek( dis, seektype, 0 ) ) ;
		}

		unsigned long size() const
		{
		    _ASSERT( hFile_ !=  INVALID_HANDLE_VALUE ) ;
            DWORD ret = ::GetFileSize( hFile_, NULL ) ;
            if( ret == -1 )
                throw Err( HRESULT_FROM_WIN32( ::GetLastError() ) ) ;

            return ret ;
		}

        unsigned long pos() const
        {
            _ASSERT( hFile_ !=  INVALID_HANDLE_VALUE ) ;
            // You can also use the SetFilePointer function to query the current 
            // file pointer position. To do this, specify a move method of 
            // FILE_CURRENT and a distance of zero. 
            DWORD ret = SetFilePointer(hFile_, 0, NULL, FILE_CURRENT);
            if( INVALID_SET_FILE_POINTER == ret )
                throw Err( HRESULT_FROM_WIN32( ::GetLastError() ) ) ;

            return ret ;
        }

	protected:
		IStreamPtr stream_ ;
		HANDLE hFile_ ;


		HANDLE duplicateHandle( HANDLE handle_to_duplicate)
		{
			HANDLE newhFile = INVALID_HANDLE_VALUE ;
			BOOL ret = DuplicateHandle( GetCurrentProcess(), handle_to_duplicate, 
				GetCurrentProcess(), &newhFile, 
				0, TRUE, DUPLICATE_SAME_ACCESS ) ;

			if( !ret || newhFile == INVALID_HANDLE_VALUE ) 
                throw Err( HRESULT_FROM_WIN32( ::GetLastError() ) ) ;

			return newhFile ;
		}
	} ;

public:

	class Reader
	{
	public:
		Reader( const _bstr_t & filename, DWORD openmode = OPEN_EXISTING, 
			DWORD sharemode = FILE_SHARE_READ )
		{
			impl_.open( filename, GENERIC_READ, openmode, sharemode ) ;
		}

		_variant_t read()
		{
			_ASSERT( is_valid() ) ;
			return impl_.read() ;
		}

		void close()
		{
			impl_.close() ;
		}

		void seek( DWORD distance, DWORD method ) 
		{
			impl_.seek( distance, method ) ;
		}

		void read( void * dest, size_t size )
		{
			impl_.read( dest, size ) ;
		}

        unsigned long size() const
        {
            return impl_.size() ;
        }

        unsigned long pos() const
        {
            return impl_.pos() ;
        }
		
	private:
		bool is_valid()
		{
			return impl_.is_open() ;
		}

		__impl impl_ ;
	} ;

	class Writer
	{
	public:
		Writer( const _bstr_t & filename, DWORD openmode = CREATE_ALWAYS, 
			DWORD sharemode = 0 )
		{
			impl_.open( filename, GENERIC_WRITE, openmode, sharemode ) ;
		}
		
		void close()
		{
			impl_.close() ;
		}

		void write( const _variant_t & var )
		{
			_ASSERT( is_valid() ) ;
			impl_.write( var ) ;
		}
		
		void seek( DWORD distance, DWORD method ) 
		{
			impl_.seek( distance, method ) ;
		}

		void write( const void * dest, size_t size )
		{
			impl_.write( dest, size ) ;
		}

        unsigned long pos() const
        {
            return impl_.pos() ;
        }

	private:
		bool is_valid()
		{
			return impl_.is_open() ;
		}

		__impl impl_ ;
	} ;
};



//--------------------------------------------------------------------------------------------
	} // namespace fm 
} // namespace dbjsys 
//--------------------------------------------------------------------------------------------



#endif
