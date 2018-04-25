#include "stdafx.h"
#include "Thread.h"
#include <process.h>

#include "IOCompletionPort.h" //for zb testing

Thread::Thread()
{
	m_thread		  = INVALID_HANDLE_VALUE;
	m_eventThreadStop = INVALID_HANDLE_VALUE;
	m_threadMode	  = THREAD_WAITING;
}

Thread::~Thread()
{
	// for testing
	CloseHandle( m_eventThreadStop );
	CloseHandle( m_thread );
	
	m_eventThreadStop = NULL;
	m_thread = NULL;
	m_threadMode = THREAD_WAITING;
}

DWORD Thread::start()
{
	unsigned int uiID = 0;
	m_eventThreadStop = CreateEvent( NULL, TRUE, FALSE, NULL );
	m_threadMode	  = THREAD_RUNNING;
	m_thread		  = ( HANDLE )_beginthreadex( 0 , 0 , threadFunc,  ( void * )this , 0 , &uiID );
	
//	printf("_beginthreadex: handle: %d\n", m_thread); // add for testing
	
	return ( uiID );
}


void Thread::terminate()
{
	if( m_thread == INVALID_HANDLE_VALUE ) return;

	SetEvent( m_eventThreadStop );

	WaitForSingleObject( m_thread, 1000 );

	{
		DWORD dwExitCode;

		GetExitCodeThread(m_thread, &dwExitCode);

		if( dwExitCode == STILL_ACTIVE ) 
		{
			TerminateThread( m_thread, 0 );
			//ExitThread(0);
			CloseHandle( m_thread );
		}
		else 
		{
			CloseHandle( m_thread );
		}
	}

	ResetEvent( m_eventThreadStop );
	CloseHandle( m_eventThreadStop );

	m_eventThreadStop = INVALID_HANDLE_VALUE;
	m_threadMode = THREAD_WAITING;
}

void Thread::suspend()
{
	if ( m_thread == INVALID_HANDLE_VALUE ) return;
	SuspendThread( m_thread );
	m_threadMode = THREAD_SUSPEND;
}

void Thread::resume()
{
	if ( m_thread == INVALID_HANDLE_VALUE ) return;
	ResumeThread( m_thread );
	m_threadMode = THREAD_RUNNING;
}


unsigned __stdcall Thread::threadFunc( void *pParam )
{
	Thread *pThread = ( Thread * ) ( pParam );	
	DWORD dwRet = pThread->run();
//	printf("threadFunc\n");
//	THREADMANAGER->m_threadList.remove(pThread);
	/////////testing//////////
/*	int threadnum=0;
	Thread *pThread1=NULL;
	for(  pThread1= THREADMANAGER->m_threadList.getHead(); !THREADMANAGER->m_threadList.isEnd(); pThread1 = THREADMANAGER->m_threadList.getNext() )
	{
		threadnum++;
		
	}
	printf("现在堆栈的线程数3 %i\n",threadnum);*/
	return ( dwRet );
}

// for testing
void Thread::EndThread()
{
	SetEvent( m_eventThreadStop );
}