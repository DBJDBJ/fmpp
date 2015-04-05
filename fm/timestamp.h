//*****************************************************************************/
//
//                  
//
//                 Copyright (c)  1997 - 2015 by Dusan B. Jovanovic (dbj@dbj.org) 
//                          All Rights Reserved
//
//        THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Dusan B. Jovanovic (dbj@dbj.org)
//
//        The copyright notice above does not evidence any
//        actual or intended publication of such source code.
//
//  $Author: DBJ  $
//  $Date: $
//  $Revision: $
//*****************************************************************************/
//
#pragma once
//
namespace dbjsys {
	namespace fm {
//*****************************************************************************/
// holds the time of its creation in a string in a format "yymmddhhmmss"
// 
class Timestamp {
	enum { vallen_ = 15 } ;
	//
	// 
	const wchar_t * getval_() const { return val_ ; }
	//
protected :
	//
	// 
	static const void setval_ ( wchar_t val_[], const time_t & time_argument = time_t(0)  ) ;
	//
	// 
	wchar_t val_[vallen_] ;
	//
public :
	dbjMAKE_ERR(Timestamp);
	//
	// 
	Timestamp () { setval_(val_); }
	//
	// 
	~Timestamp() { val_[0] = L'\x00' ; }
	//
	// 
	Timestamp(const Timestamp & other_) { wcscpy_s(val_, vallen_, other_.val_); }
	//
	// 
	Timestamp & operator = ( const Timestamp & other_ ) 
	{
		if ( this != &other_ ){  wcscpy_s( val_, vallen_, other_.val_ ) ; }
		return *this ;
	}
	//
	// 
	const wchar_t * operator () () const { return getval_(); }
	// the only way out
} ;

//----------------------------------------------------------------------
//
// put the current time, as string, into the string given
// whichhas tohave 15 chars allocated; from 0 to 14.
//
inline const void Timestamp::setval_ (  wchar_t val_[], const time_t & time_argument /* = time_t(0)  */ )  
{
static const wchar_t * number_[] = {
L"00", L"01", L"02", L"03", L"04", L"05", L"06", L"07", L"08", L"09", L"10", 
L"11", L"12", L"13", L"14", L"15", L"16", L"17", L"18", L"19", L"20", L"21", 
L"22", L"23", L"24", L"25", L"26", L"27", L"28", L"29", L"30", L"31", L"32", 
L"33", L"34", L"35", L"36", L"37", L"38", L"39", L"40", L"41", L"42", L"43", 
L"44", L"45", L"46", L"47", L"48", L"49", L"50", L"51", L"52", L"53", L"54", 
L"55", L"56", L"57", L"58", L"59", L"60", L"61", L"62", L"63", L"64", L"65", 
L"66", L"67", L"68", L"69", L"70", L"71", L"72", L"73", L"74", L"75", L"76", 
L"77", L"78", L"79", L"80", L"81", L"82", L"83", L"84", L"85", L"86", L"87", 
L"88", L"89", L"90", L"91", L"92", L"93", L"94", L"95", L"96", L"97", L"98", 
L"99", (const wchar_t *)0
} ;
	struct tm * newtime		= 0 ;
	time_t		aclock		= (time_t)0 ;

	if ( time_argument == time_t(0) ){
				time( &aclock ); // Get current time in seconds 
	}
	else {
				aclock = time_argument ; // use UD time
	}

    dbjVERIFY(localtime_s(newtime, &aclock));  // Convert time to struct tm form 

	// This will cause a crash if century is larger than 99!!!
    // But don't forget time(time_t * t) will fail lot sooner than that!!!
    unsigned long cent_ = 19 + static_cast<unsigned long>(newtime->tm_year / 100) ;
    unsigned long year_ = newtime->tm_year % 100 ;
    
    val_[0]  = number_[cent_][0] ;
	val_[1]  = number_[cent_][1] ;
	val_[2]  = number_[year_][0] ;
	val_[3]  = number_[year_][1] ;
	val_[4]  = number_[newtime->tm_mon + 1][0] ;
	val_[5]  = number_[newtime->tm_mon + 1][1] ;
	val_[6]  = number_[newtime->tm_mday][0] ;
	val_[7]  = number_[newtime->tm_mday][1] ;
	val_[8]  = number_[newtime->tm_hour][0] ;
	val_[9]  = number_[newtime->tm_hour][1] ;
	val_[10] = number_[newtime->tm_min][0] ;
	val_[11] = number_[newtime->tm_min][1] ;
	val_[12] = number_[newtime->tm_sec][0] ;
	val_[13] = number_[newtime->tm_sec][1] ;
	val_[14] = L'' ; // a.k.a. 14
}
//*****************************************************************************/
// User Definable TimeStamp
// 
class UDTimeStamp : public Timestamp 
{
public :
	typedef time_t time_type ;

	// 
	UDTimeStamp( const time_type & user_given_time  )
	{
		setval_(val_, user_given_time );
	}
} ;
//*****************************************************************************/
	} //	namespace fm 
} // namespace dbjsys 
//*****************************************************************************/
