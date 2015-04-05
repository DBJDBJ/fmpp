#ifndef SCMQUERY_H_
#define SCMQUERY_H_ 1

namespace dbjsys
{
    namespace fm
    {
// 
        class SCMQuery
        {
        public:
            typedef Error< SCMQuery > Err;
            typedef enum STATUS
            {
                NotInstalled, Stopped, Running
            };

	// 
            SCMQuery( const std::wstring & machineName = L"" ) ;
	// 
            ~SCMQuery() ;

	// 
            BOOL openService( const std::wstring service ) ;
	// 
            SCMQuery::STATUS getserviceStatus() ;

        private:
        
	// 
            void createSCM() ;

	// 
            std::wstring hostName_ ;
	// 
            SC_HANDLE  scHandle_ ;
	// 
            SC_HANDLE serviceHandle_ ;
        }; // class SCMQuery


   //----------------------------------------------------------------------------------------
        inline SCMQuery::SCMQuery( const std::wstring & machineName )
        {
            hostName_       = machineName ;
            scHandle_       = NULL ;
            serviceHandle_  = NULL ;
        }

    //----------------------------------------------------------------------------------------
        inline SCMQuery::~SCMQuery()
        {
            try
            {
                if( serviceHandle_ )
                {
                    CloseServiceHandle( serviceHandle_ ) ;
                    serviceHandle_ = NULL ;
                }

                if( scHandle_ )
                {
                    CloseServiceHandle( scHandle_ ) ;
                    scHandle_ = NULL ;
                }
            }
            catch( ... )
            {
                // LOG IT
            }
        }

    //----------------------------------------------------------------------------------------
        inline void SCMQuery::createSCM()
        {
            try
            {
                if ( NULL != scHandle_ )
                    CloseServiceHandle( scHandle_ ) ;

                const wchar_t * machineName_ = NULL ;

                if ( hostName_.size() > 0 )
                    machineName_ = hostName_.c_str() ;

                scHandle_ = OpenSCManagerW( machineName_, NULL, SC_MANAGER_ENUMERATE_SERVICE ) ;
            }
            catch( ... )
            {
                dbjTHROWERR( L"An unknown exception occured while trying to create the SCM Handle" ) ;
            }

            if ( NULL == scHandle_  )
            {
                    dbjTHROWERR( doctor::errstring() ) ;
            }
        }

    //----------------------------------------------------------------------------------------
        inline BOOL SCMQuery::openService( const std::wstring service )
        {
            if ( NULL == scHandle_ )
                createSCM() ;

            if ( serviceHandle_ )
                CloseServiceHandle( serviceHandle_ ) ;

            serviceHandle_ = OpenServiceW( scHandle_, service.c_str(), SERVICE_QUERY_STATUS ) ;

            if( NULL == serviceHandle_ )
            {
                CloseServiceHandle( serviceHandle_ ) ;

                if ( GetLastError() == ERROR_SERVICE_DOES_NOT_EXIST )
                    return false ;
                else
                { 
                    dbjTHROWERR( doctor::errstring() ) ;
                }
            }

            return true ;
        }

    //----------------------------------------------------------------------------------------
        inline SCMQuery::STATUS SCMQuery::getserviceStatus()
        {
            dbjVERIFY ( NULL != serviceHandle_ ) ; // Must open the service first

            try
            {
                SERVICE_STATUS status_[1] ;
                BOOL qStatus_ = QueryServiceStatus( serviceHandle_, (LPSERVICE_STATUS)status_) ;

                if ( ! qStatus_ ) 
                { 
                    dbjTHROWERR( doctor::errstring() ) ;
                }

                switch( status_[0].dwCurrentState  )
                {
                    case SERVICE_STOPPED: return SCMQuery::Stopped ;
                    case SERVICE_RUNNING: return SCMQuery::Running ;
                    default: return SCMQuery::Stopped;
                }
            }
            catch( ... )
            {
                dbjTHROWERR( L"An unknown exception occured while trying to enumerate through the services" ) ;
            }

            return SCMQuery::NotInstalled ;
        }

    }; // namespace fm
}; // namespace dbjsys


#endif // SCMQUERY_H_
