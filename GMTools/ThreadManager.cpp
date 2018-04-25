#include "stdafx.h"
#include "ThreadManager.h"



ThreadManager::ThreadManager()
{
	initialize();
}

ThreadManager::~ThreadManager()
{
	release();
}

void ThreadManager::initialize()
{
	m_threadList.clear();
}

void ThreadManager::release()
{
	eraseAllThread();
}

void ThreadManager::addThread( Thread *thread )
{
	{
	//	SYNC( m_threadSec );
		m_threadList.put( thread  );
	}

	thread->start();
}

bool ThreadManager::eraseThread( Thread *thread )
{
	{
	//	SYNC( m_threadSec );

		if( m_threadList.isExist( thread ) )
		{
			thread->terminate();
			m_threadList.remove( thread );
		
			return ( true );
		}
	}
	return ( false );
}

void ThreadManager::eraseAllThread()
{
//	LOG_POINT->m_bUseThread = false;

//	SYNC( m_threadSec );

	for( Thread *pThread = m_threadList.getHead(); !m_threadList.isEnd(); pThread = m_threadList.getNext() )
	{
		if( pThread )
			pThread->terminate();
	}

	m_threadList.removeAll();
}

void ThreadManager::suspend( Thread *thread )
{
	{
//		SYNC( m_threadSec );

		if( m_threadList.isExist( thread ) )
		{
			thread->suspend();
		}
	}
}

void ThreadManager::resume( Thread *thread )
{
	{
//		SYNC( m_threadSec );

		if( m_threadList.isExist( thread ) )
		{
			thread->resume();
		}
	}
}
CTasker * ThreadManager::FindThread(SOCKET m_socket)
{
	/////////for testing/////////
	int threadnum=0;
	Thread *pThread=NULL;
	for(  pThread= m_threadList.getHead(); !m_threadList.isEnd(); pThread = m_threadList.getNext() )
	{
		threadnum++;

	}
	printf("现在堆栈的线程数 %i\n",threadnum);
	for(pThread = m_threadList.getHead(); !m_threadList.isEnd(); pThread = m_threadList.getNext() )
	{
		if(((CTasker *) pThread)->m_socket==m_socket)
			return (CTasker *)pThread;
	}
	return NULL;
}
Thread * ThreadManager::addSocket(SOCKET m_socket)
{
	for( Thread *pThread = m_threadList.getHead(); !m_threadList.isEnd(); pThread = m_threadList.getNext() )
	{
		if(((IOCPThread *) pThread)->m_socket==INVALID_SOCKET)
		{
			((IOCPThread *) pThread)->m_socket=m_socket;
			return pThread;
		}
	}
	return NULL;
}