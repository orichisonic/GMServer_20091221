#ifndef _THREADMANAGER_H_
#define _THREADMANAGER_H_
#pragma warning( disable : 4786 )
#pragma warning( disable : 4127 )

#include "Singleton.h"
//#include "Synchronized.h"
#include "Thread.h"

#include "Tasker.h"
#include "IOCompletionPort.h"

#pragma warning( push, 3 )
#include "Deque.h"
#pragma warning( pop )


class ThreadManager : public SingleTon< ThreadManager >
{
public:
	Deque< Thread * >				m_threadList;

	//Synchronized::SectionObject		m_threadSec;

public:
	void	initialize();
	void	release();
	void	addThread( Thread *thread);
	bool	eraseThread( Thread *thread );
	void	eraseAllThread();

	void	suspend( Thread *thread );
	void	resume( Thread *thread );
	CTasker* FindThread(SOCKET m_socket);
	Thread* addSocket(SOCKET m_socket);

public:
	ThreadManager();
	virtual ~ThreadManager();

};

#define THREADMANAGER ThreadManager::getInstance()

#endif
