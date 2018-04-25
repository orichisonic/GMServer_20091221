#ifndef _THREAD_H_
#define _THREAD_H_



#define THREAD_WAITING	0
#define THREAD_RUNNING	1
#define THREAD_SUSPEND	2


class Thread  
{
public: 
	HANDLE			m_thread; //add for testing
protected:
	//	HANDLE			m_thread; //cancel for testing
	HANDLE			m_eventThreadStop;
	BYTE			m_threadMode;
	
protected:
	DWORD			start();
	void			terminate();
	void			suspend();
	void			resume();
	virtual DWORD	run() = 0;
	static unsigned __stdcall threadFunc( void *pParam );
	
public:
	virtual void EndThread();
	friend class ThreadManager;
	Thread();
	virtual ~Thread();
};

#endif