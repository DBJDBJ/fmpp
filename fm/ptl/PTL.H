//*****************************************************************************/
//
// PTL -- a persistent template container library
// Based on the C++ Standard Template Library (STL)
// December, 1997 - Al Stevens - Dr. Dobb's Journal
//
// Ported to MS VC 7.1 (and improved)
//
// Dusan Jovanovic : March 1998 - Dec 2005
//
//                 Copyright (c)  2000-2006 by DBJSolutions Ltd. 
//                          All Rights Reserved
//
//        THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF DBJSolutions LTD
//
//        The copyright notice above does not evidence any
//        actual or intended publication of such source code.
//
//  $Author: DBJ $
//  $Date: $
//  $Revision: 15 $
//*****************************************************************************/
#pragma once
// ========================================================
#include "../fm.h"
#include "ptlstore.h"

namespace PersistentTemplateLibrary	{

// ========================================================
//         PTLcntr class
//   abstract base persistent container class
//   derived from STL templatized container
// ========================================================
//
template <class C>
class PTLcntr : public C
{
	// ---- cannot be copied
	//
    PTLcntr(const PTLcntr&) { }
	//
	PTLcntr& operator=(const PTLcntr& pc)
		{ return *this; }
protected:
	//
    PTLStore theStore_ ; // DBJ changed from 'cs';
	//
	bool changed;
	//
	explicit PTLcntr(const _bstr_t& name)
	{
		// --- assumes save container upon destruction
		changed = true;
		// --- open for input; derived constructors read and close
	    theStore_.open(name);
	}
	//
    virtual ~PTLcntr()
    	{ }
	//
	virtual void save() = 0;
public:
	//
	void setchanged()
		{ changed = true; }
	//
	void suppress_save()
		{ changed = false; }
	//
	void save_now()
		{ save(); }
	//
	bool ischanged() const
		{ return changed; }
};
// ========================================================
//                  PTLseq
//   base class for PTLvector, PTLdeque, and PTLlist
// ========================================================
//
template <class T, class C>
class PTLseq : public PTLcntr<C>
{
	//
	void save()
	{
	    theStore_.open(size());
	    for (iterator iter = begin(); iter != end(); iter++)
	   	    theStore_ << *iter;
		changed = false;
	    theStore_.close();
	}
protected:
	//
    explicit PTLseq(const _bstr_t& name) :
					PTLcntr<C>(name)
	{
		// --- cs is opened and objcount is loaded by PTLcntr ctor
    	for (int i = 0; i < theStore_.count(); i++)    {
	        T t;
	        theStore_ >> t;
	        push_back(t);
	    }
	    theStore_.close();
	}
	//
    ~PTLseq()
		{ if (changed) save(); }
};
// ========================================================
//                    PTLsets
//     base class for PTLset and PTLmultiset
// ========================================================
//
template <class T, class C>
class PTLsets : public PTLcntr<C>
{
	//
	void save()
	{
	    theStore_.open(size());
	    for (iterator iter = begin(); iter != end(); iter++)
	   	    theStore_ << *iter;
		changed = false;
	    theStore_.close();
	}
protected:
	//
    explicit PTLsets(const _bstr_t& name) : PTLcntr<C>(name)
	{
	    for (int i = 0; i < theStore_.count(); i++)    {
	        T t;
	        theStore_ >> t;
	        insert(t);
	    }
	    theStore_.close();
	}
	//
    virtual ~PTLsets()
		{ if (changed) save(); }
};
// ========================================================
//                    PTLmaps
//     base class for PTLmap and PTLmultimap
// ========================================================
//
template <class K, class T, class C>
class PTLmaps : public PTLcntr<C>
{
	//
	void save()
	{
	    theStore_.open(size());
	    for (iterator iter = begin(); iter != end(); iter++)    
		{
	   	    theStore_ << iter->first;
	       	theStore_ << iter->second;
	    }
		changed = false;
	    theStore_.close();
	}
protected:
	//
    explicit PTLmaps(const _bstr_t& name) : PTLcntr<C>(name)
	{
    	for (int i = 0; i < theStore_.count(); i++)    {
        	K k;
	        theStore_ >> k;
    	    T t;
        	theStore_ >> t;
	        insert(std::pair<K,T>(k, t));
    	}
	    theStore_.close();
	}
	//
    virtual ~PTLmaps()
		{ if (changed) save(); }
};
// =============================================================
//
//           PTL public interface
//
// template parameter notation:
//   T = type being contained
//   K = key value for sets and maps
//   C = STL container being used by container adaptor
//   P = predicate function
//
// =============================================================
//                 PTLvector
// =============================================================
//
template <class T>
class PTLvector : public PTLseq<T, std::vector<T> >	{
public:
	//
	explicit PTLvector(const _bstr_t& name) : 
			PTLseq<T, std::vector<T> >(name)
		{ }
};
// =============================================================
//                 PTLdeque
// =============================================================
//
template <class T>
class PTLdeque : public PTLseq<T, std::deque<T> >	{
public:
	//
	explicit PTLdeque(const _bstr_t& name) : 
			PTLseq<T, std::deque<T> >(name)
		{ }
};
// =============================================================
//                 PTLlist
// =============================================================
//
template <class T>
class PTLlist : public PTLseq<T, std::list<T> >	{
public:
	//
	explicit PTLlist(const _bstr_t& name) : 
			PTLseq<T, std::list<T> >(name)
		{ }
};
// =============================================================
//                 PTLstack
// =============================================================
#if ( _MSC_VER >= 1100 ) // MSVC5 and up
template <class T, class C = std::deque<T> >
class PTLstack : public PTLcntr<std::stack<T, C> >
#else
template <class T>
class PTLstack : public PTLcntr<std::stack<T, std::deque<T> > >
#endif
{
	void save()
	{
    	theStore_.open(size());
#if ( _MSC_VER >= 1100 ) // MSVC5 and up
	    std::stack<T, C> stack;
#else
		std::stack<T, std::deque<T> > stack;
#endif
   		while (!empty())    {
       		stack.push(top());
	        pop();
    	}
	   	while (!stack.empty())    {
    	   	theStore_ << stack.top();
        	stack.pop();
		}
		changed = false;
	    theStore_.close();
	}
public:
 #if ( _MSC_VER >= 1100 ) // MSVC5 and up
   explicit PTLstack(const _bstr_t& name) :
						PTLcntr<std::stack<T, C> >(name)
#else
   explicit PTLstack(const _bstr_t& name) :
						PTLcntr<std::stack<T, std::deque<T> > >(name)
#endif
	{
    	for (int i = 0; i < theStore_.count(); i++)    {
        	T t;
	        theStore_ >> t;
    	    push(t);
	    }
	    theStore_.close();
	}
    ~PTLstack()
		{ if (changed) save(); }
};
// =============================================================
//                 PTLqueue
// =============================================================
#if ( _MSC_VER >= 1100 ) // MSVC5 and up
template <class T, class C = PTLdeque<T> >
class PTLqueue : public PTLcntr<std::queue<T, C> >
#else
template <class T>
class PTLqueue : public PTLcntr<std::queue<T, std::deque<T> > >
#endif
{
	void save()
	{
    	theStore_.open(size());
	    while (!empty())    {
   		    theStore_ << front();
       		pop();
		}
		changed = false;
	    theStore_.close();
	}
public:
#if ( _MSC_VER >= 1100 ) // MSVC5 and up
  explicit PTLqueue(const _bstr_t& name) :
						PTLcntr<std::queue<T, C> >(name)
#else
  explicit PTLqueue(const _bstr_t& name) :
						PTLcntr<std::queue<T, std::deque<T> > >(name)
#endif
	{
    	for (int i = 0; i < theStore_.count(); i++)    {
        	T t;
	        theStore_ >> t;
    	    push(t);
	    }
	    theStore_.close();
	}
    ~PTLqueue()
		{ if (changed) save(); }
};
// =============================================================
//                 PTLpriority_queue
// =============================================================
#if ( _MSC_VER >= 1100 ) // MSVC5 and up
template <class T, 
          class C=std::vector<T>, 
		  class P=std::less<C::value_type> >
class PTLpriority_queue : 
		  public PTLcntr<std::priority_queue<T,C,P> >
#else
template <class T > 
//          class C=std::vector<T>, 
//  		  class P=std::less<C::value_type> >
class PTLpriority_queue : 
		  public 
			  PTLcntr<
			  std::priority_queue<
				T,std::vector<T>,std::less<std::vector<T> > > 
				> 
//			  >
#endif
{
	void save()
	{
	    theStore_.open(size());
	    while (!empty())    {
	   	    theStore_ << top();
	       	pop();
		}
		changed = false;
	    theStore_.close();
	}
public:
#if ( _MSC_VER >= 1100 ) // MSVC5 and up
    explicit PTLpriority_queue(const _bstr_t& name) :
					PTLcntr<std::priority_queue<T,C,P> >(name)
#else
    explicit PTLpriority_queue(const _bstr_t& name) :
	PTLcntr<std::priority_queue<T,std::vector<T>,std::less<C::value_type> > >(name)
#endif
	{
		for (int i = 0; i < theStore_.count(); i++)    {
	    	T t;
		    theStore_ >> t;
			push(t);
	    }
    	theStore_.close();
	}
    virtual ~PTLpriority_queue()
		{ if (changed) save(); }
};
// =============================================================
//                    PTLset
// =============================================================
#if ( _MSC_VER >= 1100 ) // MSVC5 and up
template <class K, class P = std::less<K> >
class PTLset : public PTLsets<K, std::set<K,P> >	
#else
template <class K>
class PTLset : public PTLsets<K, std::set<K,std::less<K> > >	
#endif
{
public:
#if ( _MSC_VER >= 1100 ) // MSVC5 and up
	explicit PTLset(const _bstr_t& name) : 
				PTLsets<K, std::set<K,P> >(name)
#else
	explicit PTLset(const _bstr_t& name) : 
				PTLsets<K, std::set<K,std::less<K>> >(name)
#endif
		{ }
};
// =============================================================
//                    PTLmultiset
// =============================================================
#if ( _MSC_VER >= 1100 ) // MSVC5 and up
template <class K, class P = std::less<K> >
class PTLmultiset : public PTLsets<K, std::multiset<K,P> >
#else
template <class K>
class PTLmultiset : public PTLsets<K, std::multiset<K,std::less<K> > >
#endif
{
public:
#if ( _MSC_VER >= 1100 ) // MSVC5 and up
	explicit PTLmultiset(const _bstr_t& name) : 
				PTLsets<K, std::multiset<K,P> >(name)
#else
	explicit PTLmultiset(const _bstr_t& name) : 
				PTLsets<K, std::multiset<K,std::less<K> > >(name)
#endif
		{ }
};
// =============================================================
//                    PTLmap
// =============================================================
#if ( _MSC_VER >= 1100 ) // MSVC5 and up
template <class K, class T, class P = std::less<K> >
class PTLmap : public PTLmaps<K, T, std::map<K,T,P> >	
#else
template <class K, class T> // , class P = std::less<K> >
class PTLmap : public PTLmaps<K, T, std::map<K,T,std::less<K> > >	
#endif
{
public:
#if ( _MSC_VER >= 1100 ) // MSVC5 and up
	explicit PTLmap(const _bstr_t& name) : 
				PTLmaps<K, T, std::map<K,T,P> >(name)
#else
	explicit PTLmap(const _bstr_t& name) : 
				PTLmaps<K, T, std::map<K,T,std::less<K> > >(name)
#endif
		{ }
};
// =============================================================
//                    PTLmultimap
// =============================================================
#if ( _MSC_VER >= 1100 ) // MSVC5 and up
template <class K, class T, class P = std::less<K> >
class PTLmultimap : public PTLmaps<K,T,std::multimap<K,T,P> > 
#else
template <class K, class T> // , class P = std::less<K> >
class PTLmultimap : public PTLmaps<K,T,std::multimap<K,T,std::less<K> > > 
#endif
 {
public:
#if ( _MSC_VER >= 1100 ) // MSVC5 and up
	explicit PTLmultimap(const _bstr_t& name) : 
				PTLmaps<K, T, std::multimap<K,T,P> >(name)
#else
	explicit PTLmultimap(const _bstr_t& name) : 
				PTLmaps<K, T, std::multimap<K,T,std::less<K> > >(name)
#endif
		{ }
};

} // namespace PersistentTemplateLibrary


