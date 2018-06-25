#ifndef CTHREAD_H
#define CTHREAD_H

//#define _WIN32_WINNT

#include <iostream>
using namespace std;
//CRITICAL_SECTION    __moebius_g_cs;
//
//#define SET_ACCESSOR_( x, y )       inline void Set##y( x t )   { CThreadPauseStatus_ l; this->##y = t; };
//#define GET_ACCESSOR_( x, y )       inline x Get##y()           { CThreadPauseStatus_ l; return this->##y; };
//#define GET_SET_ACCESSOR_( x, y )   SET_ACCESSOR_( x, y ) GET_ACCESSOR_( x, y )
//
//
//#define SETP_ACCESSOR_( x, y )      inline void SetPausing( x* t )  { CThreadPauseStatus_ l; Pausing = *t; };
//#define GETP_ACCESSOR_( x, y )      inline x* GetPausing()          { CThreadPauseStatus_ l; return &Pausing; };
//#define GETP_SETP_ACCESSOR_( x, y ) SETP_ACCESSOR_( x, y ) GETP_ACCESSOR_( x, y )

typedef enum THREAD_STATE
{
	THREAD_STATE_NONE		= 0,		//Thread state none
	THREAD_STATE_STOPED		= 1,		//Thread state stoped
	THREAD_STATE_RUNNING	= 2,		//Thread state running
	THREAD_STATE_SUSPENDED	= 3,		//Thread state suspended
	THREAD_STATE_SLEEPING	= 4,		//Thread state sleeping

	
    //D3DDEVTYPE_FORCE_DWORD = 0xffffffff,
} THREAD_STATE, *LPTHREAD_STATE;

class CMutexClass_
{
public:

	CMutexClass_(void);
	~CMutexClass_(void);
	
	bool Created();
	void Lock();
	void Unlock(DWORD waitTime = 3);
	bool IsLocked();

private:
	bool m_bCreated;
	bool m_bLocked;
	HANDLE m_mutex;
	DWORD m_ownerID;
};

class CThread_
{
public:
	/*class CThreadPauseStatus_
	{
	public:
		inline CThreadPauseStatus_();
		inline ~CThreadPauseStatus_();
	};*/

	CThread_();
	CThread_(LPTHREAD_START_ROUTINE threadFunction, LPVOID pData, bool bCreateSuspended, int nPriority);

	bool Create(LPTHREAD_START_ROUTINE threadFunction, LPVOID pData, bool bCreateSuspended, int nPriority = THREAD_PRIORITY_NORMAL);

	virtual ~CThread_();

	void Resume();
	void Pause();
	void UnlockMutex(DWORD waitTime = 1);
	bool Restart(LPTHREAD_START_ROUTINE threadFunction, LPVOID pData, bool bCreateSuspended, int nPriority = THREAD_PRIORITY_NORMAL);


	static bool ThreadIdsEqual(DWORD *p1, DWORD *p2)
	{
		return ((*p1 == *p2) ? true : false);
	}
	bool IsCreated()
	{
		return !(m_hThread == NULL);
	}
	static DWORD ThreadId()
	{
		return GetCurrentThreadId();		
	}

	CMutexClass_	  m_mutex;         //Mutex objetc to protect the internal data
	//bool m_free = false;
	
private:

	 HANDLE			m_hThread;
	 DWORD			m_dwThreadId;
	 THREAD_STATE	m_threadState;
	 //CMutexClass_   m_mutex;

	 //bool			m_pausing;
	 //bool			m_mutex;

//public:
//	void Setm_pausing(bool t ) 
//	{ 
//		CThreadPauseStatus_ l; 
//		m_pausing = t; 
//	}
//	void Setm_mutex( bool t )  
//	{ 
//		CThreadPauseStatus_ l;
//		m_mutex = t; 
//	}
//	void Setm_threadState( THREAD_STATE t ) 
//	{ 
//		CThreadPauseStatus_ l;
//		m_threadState = t;
//	}
//	bool Getm_pausing()
//	{ 
//		CThreadPauseStatus_ l; 
//		return m_pausing; 
//	}
//	bool Getm_mutex()
//	{ 
//		CThreadPauseStatus_ l;
//		return m_mutex; 
//	}
//	THREAD_STATE Getm_threadState()
//	{ 
//		CThreadPauseStatus_ l; 
//		return m_threadState; 
//	}
	 //GET_SET_ACCESSOR_(bool, m_pausing);
	 //GET_SET_ACCESSOR_(bool,m_mutex);
	 //GET_SET_ACCESSOR_(THREAD_STATE,m_threadState);
};

//inline CThread_::CThreadPauseStatus_::CThreadPauseStatus_()  {EnterCriticalSection( &__moebius_g_cs ); }
//inline CThread_::CThreadPauseStatus_::~CThreadPauseStatus_() {LeaveCriticalSection( &__moebius_g_cs ); }

CMutexClass_::CMutexClass_(void) : m_bCreated(true), m_bLocked(false)
{
	//Creating the mutex handle,...
	m_mutex = CreateMutex(NULL,FALSE,NULL);
	if( !m_mutex ) 
	{
		m_bCreated = FALSE;
	}
	//Make the memory for the owner id
	memset(&m_ownerID,0,sizeof(DWORD));
}

CMutexClass_::~CMutexClass_(void)
{
	
	WaitForSingleObject(m_mutex,INFINITE);
	CloseHandle(m_mutex);
}

bool CMutexClass_::Created()
{
	return m_bCreated;
}
void CMutexClass_::Lock()
{
	DWORD id = CThread_::ThreadId();
	try 
	{
		if(CThread_::ThreadIdsEqual(&m_ownerID,&id))
		{
			throw "\n\tThe same thread can not acquire a mutex twice!\n"; 
		}
		WaitForSingleObject(m_mutex,INFINITE);
		m_ownerID = CThread_::ThreadId();
		m_bLocked =  true;
	}
	catch(char *msg)
	{
		MessageBoxA(NULL,&msg[2],"Error CMutexClass_::Lock",MB_ICONHAND);
		exit(-1);
	}
}

void CMutexClass_::Unlock(DWORD waitTime)
{
	DWORD id = CThread_::ThreadId();
	try 
	{
		if(!CThread_::ThreadIdsEqual(&id,&m_ownerID) )
		{
			return;
			//throw "\n\tonly the thread that acquires a mutex can release it!"; 
		}
		m_bLocked =  false;
		memset(&m_ownerID,0,sizeof(DWORD));
		ReleaseMutex(m_mutex);
		Sleep(waitTime);
	}
	catch ( char *psz)
	{
		MessageBoxA(NULL,&psz[2],"Error CMutexClass_::Unlock",MB_ICONHAND);
		exit(-1);
	}
}

bool CMutexClass_::IsLocked()
{
	return m_bLocked;
}
CThread_::CThread_()
{
}

CThread_::CThread_(LPTHREAD_START_ROUTINE threadFunction, LPVOID pData, bool bCreateSuspended, int nPriority)
{
	Create(threadFunction, pData, bCreateSuspended, nPriority);
}

bool CThread_::Create(LPTHREAD_START_ROUTINE threadFunction, LPVOID pData, bool bCreateSuspended, int nPriority)
{
	CloseHandle(m_hThread);

	m_hThread = CreateThread(NULL,
            0,
            threadFunction,								//Thread funcion
            pData,										//Data passed to the thread function
			bCreateSuspended ? CREATE_SUSPENDED : 0,	//Default creation flags
            &m_dwThreadId);								//Save the thread ID

	if(!m_hThread)
	{
		return false;
	}
	if(!SetThreadPriority(m_hThread, nPriority))
	{
		return false;
	}
	m_threadState = THREAD_STATE_SUSPENDED;
	if(!bCreateSuspended)
	{
		ResumeThread(m_hThread);
		m_threadState = THREAD_STATE_RUNNING;
	}

	if(!m_mutex.Created())
	{
		return false;
	}
	return true;
}

CThread_::~CThread_()
{
	CloseHandle(m_hThread);
}

void CThread_::Pause()
{
	//if(Getm_threadState() == THREAD_STATE_RUNNING)
	//{
		while(m_mutex.IsLocked()){}
		SuspendThread(m_hThread);
		m_threadState = THREAD_STATE_SUSPENDED;
	//	if(Getm_mutex())//m_mutex.IsLocked())
	//	{
	//		Setm_pausing(true);
	//	}
	//	else
	//	{
	//		Setm_threadState(THREAD_STATE_SUSPENDED);
	//		SuspendThread(m_hThread);
	//	}
	//	
	//}
}

void CThread_::Resume()
{
	if(m_threadState == THREAD_STATE_SUSPENDED)
	{
		ResumeThread(m_hThread);
		m_threadState = THREAD_STATE_RUNNING;
	}
}

bool CThread_::Restart(LPTHREAD_START_ROUTINE threadFunction, LPVOID pData, bool bCreateSuspended, int nPriority)
{
	CloseHandle(m_hThread);
	m_hThread = CreateThread(NULL,
            0,
            threadFunction,								//Thread funcion
            pData,										//Data passed to the thread function
			bCreateSuspended ? CREATE_SUSPENDED : 0,	//Default creation flags
            &m_dwThreadId);								//Save the thread ID

	if(!m_hThread)
	{
		return false;
	}
	if(!SetThreadPriority(m_hThread, nPriority))
	{
		return false;
	}

	m_threadState = THREAD_STATE_SUSPENDED;
	if(!bCreateSuspended)
	{
		ResumeThread(m_hThread);
		m_threadState = THREAD_STATE_RUNNING;
	}

	return true;
}
void CThread_::UnlockMutex(DWORD waitTime)
{
	m_mutex.Unlock(waitTime + 2);
	//Setm_mutex(false);
	//if(Getm_pausing()&& Getm_threadState() == THREAD_STATE_RUNNING)
	//{
	//	Setm_threadState(THREAD_STATE_SUSPENDED);
	//	Setm_pausing(false);
	//	SuspendThread(m_hThread);
	//	Sleep(waitTime);
	//	/*if(!Getm_pausing())
	//	{*/
	//
	//	//}
	//}
}
//void CThread_::GetID(DWORD *id)
//{
//	m_dwThreadId = GetThreadId(m_hThread);
//	*id =  m_dwThreadId;
//}


#endif

