#pragma once
#ifndef CQTIMER_H
#define CQTIMER_H

#include "assert.h"
#include <windows.h>

class CQTimer_
{
public:
	CQTimer_(void);
	~CQTimer_(void);
	
	//Resets the timer
	//---------------------------------
    void Reset(); 
	//Stats the timer
	//---------------------------------
    void Start(); 
	//Sop or pause the timer
	//---------------------------------
    void Stop();  
	//Adavnce the timer deslta seconds
	//---------------------------------
    void Advance(); 
	//Get the absolute system time
	//---------------------------------
    double GetAbsoluteTime(); 
	//Get the current time
	//---------------------------------
    double GetTime(); 
	//Get the time that elapsed between Get*ElapsedTime() calls
	//---------------------------------
    double GetElapsedTime(); 
	//Get the time values
	//---------------------------------
    void GetTimeValues( double* pfTime, double* pfAbsoluteTime, double* pfElapsedTime );
	//Returns true if timer stopped
	//---------------------------------
    bool IsStopped(); 

    //Limit the current thread to one processor (the current one). This ensures that timing code runs
    //on only one processor, and will not suffer any ill effects from power management.
	//---------------------------------
    void LimitThreadAffinityToCurrentProc();

protected:
	//If stopped, returns time when stopped otherwise returns current time
	//---------------------------------
    LARGE_INTEGER GetAdjustedCurrentTime();

    bool m_bTimerStopped;
    LONGLONG m_llQPFTicksPerSec;
    
    LONGLONG m_llStopTime;
    LONGLONG m_llLastElapsedTime;
    LONGLONG m_llBaseTime;
};

#endif


