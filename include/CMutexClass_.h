//// CMutexClass_.h
//
//#pragma once
//
//typedef enum THREAD_STATE
//{
//	THREAD_STATE_NONE		= 0,		//Thread state none
//	THREAD_STATE_STOPED		= 1,		//Thread state stoped
//	THREAD_STATE_RUNNING	= 2,		//Thread state running
//	THREAD_STATE_SUSPENDED	= 3,		//Thread state suspended
//	THREAD_STATE_SLEEPING	= 4,		//Thread state sleeping
//
//} THREAD_STATE, *LPTHREAD_STATE;
//
//class CMutex_
//{
//public:
//	CMutex_();
//	~CMutex_();
//	
//	bool Created();
//	void Lock();
//	void Unlock(DWORD waitTime = 3);
//	bool IsLocked();
//
//private:
//	bool m_bCreated;
//	bool m_bLocked;
//	HANDLE m_mutex;
//	DWORD m_ownerID;
//};
//
//class CThread_
//{
//public:
//	CThread_();
//	CThread_(LPTHREAD_START_ROUTINE threadFunction, LPVOID pData, bool bCreateSuspended, int nPriority);
//
//	bool Create(LPTHREAD_START_ROUTINE threadFunction, LPVOID pData, bool bCreateSuspended, int nPriority = THREAD_PRIORITY_NORMAL);
//
//	virtual ~CThread_();
//
//	void Resume();
//	void Pause();
//	void UnlockMutex(DWORD waitTime = 1);
//	bool Restart(LPTHREAD_START_ROUTINE threadFunction, LPVOID pData, bool bCreateSuspended, int nPriority = THREAD_PRIORITY_NORMAL);
//
//
//	static bool ThreadIdsEqual(DWORD *p1, DWORD *p2)
//	{
//		return ((*p1 == *p2) ? true : false);
//	}
//	bool IsCreated()
//	{
//		return !(m_hThread == NULL);
//	}
//	static DWORD ThreadId()
//	{
//		return GetCurrentThreadId();		
//	}
//
//	CMutex_	  m_mutex;         //Mutex objetc to protect the internal data
//	
//private:
//
//	 HANDLE			m_hThread;
//	 DWORD			m_dwThreadId;
//	 THREAD_STATE	m_threadState;
//
//};
//
