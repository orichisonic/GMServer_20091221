#ifndef _IOCOMPLETIONPORT_H
#define _IOCOMPLETIONPORT_H

#include "SingleTon.h"
#include "Thread.h"
#include "ThreadManager.h"
#include "Tasker.h"

#define DF_DEFAULT_IOCP_THREAD_COUNT		3
#define DF_SOCKET_VERSION					MAKEWORD( 2, 2 )
#define DF_SEND_BUFFER_SIZE					8192 * 2
#define DF_RECV_BUFFER_SIZE					8192 * 2


//class Session;

class AcceptThread : public Thread		// Accept 역할 담당..
{
private:
	virtual DWORD run();
};

class IOCPThread  : public Thread		// IOCP Thread
{
private:
	virtual DWORD run();
	//add for testing
public:
	IOCPThread()
	{
		m_iThreadID = 0;
		m_socket = NULL;
	}
	
	~IOCPThread()
	{
		closesocket(m_socket);
		m_socket = NULL;
	}   
public:
	int m_iThreadID;
	SOCKET m_socket;
};

class   IOCompletionPort : public SingleTon < IOCompletionPort >
{
	// add for testing
public:
	IOCompletionPort();
	~IOCompletionPort();
public:
//	SOCKET		m_hServerSocket;
	HANDLE		m_hCompletionPort;
	int			m_iMaxWorkerThread;

public:
	bool	startupSocket();
	bool	bind( WORD port );
	bool	listen( int backlog = SOMAXCONN );
	
public:
	//SOCKET	accept( struct sockaddr_in *AcceptAddr );
	bool	initialize( WORD port, WORD workerThreadCount );
	bool	createCompletionPort( WORD workerThreadCount );
	bool	associateIocpHandle( HANDLE	handle, DWORD key );
	BOOL	getCompletionStatus( LPDWORD pdwOutBytesTransferred, DWORD *pOutCompKey, LPOVERLAPPED* pOutOverlapped, int* pnOutErrCode );
	bool	threadCreate();
	bool	serviceRun( WORD port, WORD maxWorkerThread = DF_DEFAULT_IOCP_THREAD_COUNT );
//	void	postCompletionStatus( Session *pSession );

	bool	initializeOnlyIOCP( WORD workerThreadCount );
	bool	startService( WORD port );
};
static SOCKET		m_hServerSocket;
#define IOCP_MANAGER_POINT		IOCompletionPort::getInstance()	
#define IOCPThread_POINT		IOCPThread::getInstance()	

#endif