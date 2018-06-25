// Archivo DLL principal.

#include "stdafx.h"

#include "CMutexClass_.h"

CMutex_::CMutex_()  : m_bCreated(true), m_bLocked(false)
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

CMutex_::~CMutex_()
{
	WaitForSingleObject(m_mutex,INFINITE);
	CloseHandle(m_mutex);
}

bool CMutex_::Created()
{
	return m_bCreated;
}
void CMutex_::Lock()
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
		MessageBoxA(NULL,&msg[2],"Error CMutex_::Lock",MB_ICONHAND);
		exit(-1);
	}
}

void CMutex_::Unlock(DWORD waitTime)
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
		MessageBoxA(NULL,&psz[2],"Error CMutex_::Unlock",MB_ICONHAND);
		exit(-1);
	}
}

bool CMutex_::IsLocked()
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
	while(m_mutex.IsLocked()){}
	SuspendThread(m_hThread);
	m_threadState = THREAD_STATE_SUSPENDED;
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