#pragma once
#ifndef CQTIMER_H
#define CQTIMER_H

#include "assert.h"

class CQTimer_
{
public:
	CQTimer_(void);
	~CQTimer_(void);
	
	//Resets the timer
	//---------------------------------
    void Reset(); 
	//Starts the timer
	//---------------------------------
    void Start(); 
	//Stop or pause the timer
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
	//Return true if timer stopped
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


CQTimer_::CQTimer_()
{
	m_bTimerStopped     = true;
    m_llQPFTicksPerSec  = 0;

    m_llStopTime        = 0;
    m_llLastElapsedTime = 0;
    m_llBaseTime        = 0;
    
    LARGE_INTEGER qwTicksPerSec;
    QueryPerformanceFrequency(&qwTicksPerSec);
    m_llQPFTicksPerSec = qwTicksPerSec.QuadPart;
}
CQTimer_::~CQTimer_()
{
}
void CQTimer_::Reset()
{
    LARGE_INTEGER qwTime = GetAdjustedCurrentTime();
    m_llBaseTime        = qwTime.QuadPart;
    m_llLastElapsedTime = qwTime.QuadPart;
    m_llStopTime        = 0;
    m_bTimerStopped     = false;
}
void CQTimer_::Start()
{
    // Get the current time
    LARGE_INTEGER qwTime;
    QueryPerformanceCounter(&qwTime);
    if(m_bTimerStopped)
	{
		m_llBaseTime += qwTime.QuadPart - m_llStopTime;
	}
    m_llStopTime = 0;
    m_llLastElapsedTime = qwTime.QuadPart;
    m_bTimerStopped = false;
}
void CQTimer_::Stop()
{
    if(!m_bTimerStopped)
    {
        LARGE_INTEGER qwTime;
        QueryPerformanceCounter( &qwTime );
        m_llStopTime = qwTime.QuadPart;
        m_llLastElapsedTime = qwTime.QuadPart;
        m_bTimerStopped = true;
    }
}
void CQTimer_::Advance()
{
    m_llStopTime += m_llQPFTicksPerSec/10;
}
double CQTimer_::GetAbsoluteTime()
{
    LARGE_INTEGER qwTime;
    QueryPerformanceCounter( &qwTime );
    double fTime = (qwTime.QuadPart / (double) m_llQPFTicksPerSec);
    return fTime;
}
double CQTimer_::GetTime()
{
    LARGE_INTEGER qwTime = GetAdjustedCurrentTime();
    double fAppTime = (double) ( qwTime.QuadPart - m_llBaseTime ) / (double) m_llQPFTicksPerSec;
    return fAppTime;
}
void CQTimer_::GetTimeValues( double* pfTime, double* pfAbsoluteTime, double* pfElapsedTime )
{
    assert( pfTime && pfAbsoluteTime && pfElapsedTime );    
    LARGE_INTEGER qwTime = GetAdjustedCurrentTime();
    double fElapsedTime = ((double) ( qwTime.QuadPart - m_llLastElapsedTime ) / (double) m_llQPFTicksPerSec);
    m_llLastElapsedTime = qwTime.QuadPart;
    if( fElapsedTime < 0.0f )
	{
		    fElapsedTime = 0.0f;
	}
    *pfAbsoluteTime = qwTime.QuadPart / (double) m_llQPFTicksPerSec;
    *pfTime = ( qwTime.QuadPart - m_llBaseTime ) / (double) m_llQPFTicksPerSec;   
    *pfElapsedTime = fElapsedTime;
}
double CQTimer_::GetElapsedTime()
{
    LARGE_INTEGER qwTime = GetAdjustedCurrentTime();
    double fElapsedTime = (double) ( qwTime.QuadPart - m_llLastElapsedTime ) / (double) m_llQPFTicksPerSec;
    m_llLastElapsedTime = qwTime.QuadPart;
    if( fElapsedTime < 0.0f )
	{
		fElapsedTime = 0.0f;
	}
    return fElapsedTime;
}
LARGE_INTEGER CQTimer_::GetAdjustedCurrentTime()
{
    LARGE_INTEGER qwTime;
    if( m_llStopTime != 0 )
	{
		qwTime.QuadPart = m_llStopTime;
	}
    else
	{
		QueryPerformanceCounter( &qwTime );
	}
    return qwTime;
}
bool CQTimer_::IsStopped()
{
    return m_bTimerStopped;
}
void CQTimer_::LimitThreadAffinityToCurrentProc()
{
    HANDLE hCurrentProcess = GetCurrentProcess();
    
    // Get the processor affinity mask for this process
    DWORD_PTR dwProcessAffinityMask = 0;
    DWORD_PTR dwSystemAffinityMask = 0;
    
    if(GetProcessAffinityMask(hCurrentProcess, &dwProcessAffinityMask, &dwSystemAffinityMask ) != 0 && dwProcessAffinityMask )
    {
        // Find the lowest processor that our process is allows to run against
        DWORD_PTR dwAffinityMask = (dwProcessAffinityMask & ((~dwProcessAffinityMask)+1));

        // Set this as the processor that our thread must always run against
        // This must be a subset of the process affinity mask
        HANDLE hCurrentThread = GetCurrentThread();
        if( INVALID_HANDLE_VALUE != hCurrentThread )
        {
            SetThreadAffinityMask( hCurrentThread, dwAffinityMask );
            CloseHandle( hCurrentThread );
        }
    }
    CloseHandle( hCurrentProcess );
}

#endif