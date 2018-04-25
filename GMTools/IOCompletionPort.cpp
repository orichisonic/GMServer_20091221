#include "stdafx.h"
#include "IOCompletionPort.h"

//监听线程，监听来自客户端的连接
DWORD AcceptThread::run()
{
	int nIndex=0;
	int i=1;

	WSANETWORKEVENTS wsaevent;
	WSAEVENT e=WSACreateEvent();
	WSAEventSelect(m_hServerSocket,e,FD_ACCEPT);//设置选择连接消息
	while( WaitForSingleObject ( m_eventThreadStop, 0 ) != WAIT_OBJECT_0 )//循环监听来自客户端的连接
	{
		nIndex=WSAWaitForMultipleEvents(1,&e,false,INFINITE,false);//等待消息消息
		WSAEnumNetworkEvents(m_hServerSocket,e,&wsaevent);
		if(wsaevent.lNetworkEvents&FD_ACCEPT)//创建通信线程
		{
			IOCPThread* pThread = new IOCPThread;
		
			pThread->m_iThreadID=i;
			pThread->m_socket=accept(m_hServerSocket,NULL,NULL);
			i++;
			THREADMANAGER->addThread(pThread);//开始运行通信线程
		//	printf("接收来自客户端的连接 线程号：%i  SOCKET号: %i\n", i-1, pThread->m_socket);
		}
	}	

	WSACloseEvent(e);
	return ( 0 );
}

// 通信线程
DWORD IOCPThread::run()
{
	int nIndex=0;
	int i=0;
	bool m_result;
	
	CTasker * pThread = new CTasker;
	pThread->m_socket=(SOCKET)this->m_socket;
	THREADMANAGER->addThread(pThread);//开始运行服务线程，每个客户端有两个线程为其服务
	
	WSANETWORKEVENTS wsaevent;
	WSAEVENT e=WSACreateEvent();
	WSAEventSelect((SOCKET)this->m_socket,e,FD_READ|FD_CLOSE);//接收读取或关闭事件
	
	while( TRUE )
	{
		WSAWaitForMultipleEvents(1,&e,false,INFINITE,false);
		WSAEnumNetworkEvents((SOCKET)this->m_socket,e,&wsaevent);
		if(wsaevent.lNetworkEvents & FD_READ)//客户端请求读取事件
		{
			CTasker * pDoThread=NULL;
			pDoThread=THREADMANAGER->FindThread((SOCKET)this->m_socket);
			pDoThread->Receive();
			
			printf("IOCPThread::run, handle: %d\r\n", pDoThread->m_thread); // for testing
		}
		else if(wsaevent.lNetworkEvents & FD_CLOSE)//关闭事件
		{	
			printf("关闭通信线程  SOCKET号: %i\n",(int)(SOCKET)this->m_socket);
			WSACloseEvent(e);
			CLogic::DelLog((SOCKET)this->m_socket);//删除用户队列
			pThread->EndThread();//结束线程
			break;
		}
		Sleep(1000);
	}
	return 0;
}


//////////////////////////////////////////////////////////////////////
// Member functions
//////////////////////////////////////////////////////////////////////

bool IOCompletionPort::startupSocket()
{
	WSADATA 	wsaData;

	if( WSAStartup( DF_SOCKET_VERSION, &wsaData ) != 0 ) 
		return false;

	m_hServerSocket = ::socket( AF_INET, SOCK_STREAM, 0 );
	if( m_hServerSocket == INVALID_SOCKET )
		return false;

	{
	//	int reuse = 1;
	//	setsockopt( m_hServerSocket, SOL_SOCKET, (int)~SO_REUSEADDR, (char*)&reuse, sizeof(reuse) );//独占占有端口和地

	}

#ifdef USE_NONE_BLOCK
	{
		u_long argp = 1;
		ioctlsocket( m_hServerSocket, FIONBIO, &argp );//将监听SOCKET改为非阻塞式的
	}
#endif

	return true;
}

bool IOCompletionPort::bind( WORD port )
{
	int ret=-1;
	SOCKADDR_IN 	serv_addr;

	ZeroMemory( &serv_addr, sizeof (serv_addr) );
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = (WORD)htons(port);//设置端口
	serv_addr.sin_addr.S_un.S_addr = htons( INADDR_ANY );

	ret = ::bind( m_hServerSocket, (LPSOCKADDR)&serv_addr, sizeof(serv_addr) );//将监听SOCKET和指定端口绑定起来
	

	if( ret == SOCKET_ERROR )
		return false;

	return true;
}

bool IOCompletionPort::listen( int backlog )
{
	return ( ::listen( m_hServerSocket, backlog ) != SOCKET_ERROR );//监听指定数目的连接
}

bool IOCompletionPort::initialize( WORD port, WORD workerThreadCount )
{
	if( !startupSocket() )
		return false;
	if( !bind( port ) )
		return false;
	if( !listen( 200 ) )
		return false;
	if( !createCompletionPort( workerThreadCount ) )
		return false;

	return true;
}

// 弥柳 2005-03-18
bool IOCompletionPort::initializeOnlyIOCP( WORD workerThreadCount )
{
	if( !startupSocket() )
		return false;
	if( !createCompletionPort( workerThreadCount ) )
		return false;

	return true;
}

// 弥柳 2005-03-18
bool IOCompletionPort::startService( WORD port )
{
	if( !bind( port ) )
		return false;
	if( !listen() )
		return false;

	return true;
}

bool IOCompletionPort::createCompletionPort( WORD workerThreadCount )
{
	m_iMaxWorkerThread = workerThreadCount;
	m_hCompletionPort = ::CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, 0, m_iMaxWorkerThread );

	return ( m_hCompletionPort != INVALID_HANDLE_VALUE );
}

bool IOCompletionPort::associateIocpHandle( HANDLE	handle, DWORD key )
{
	HANDLE	iocpHandle;

	if( handle == INVALID_HANDLE_VALUE )
		return false;
	
	iocpHandle = ::CreateIoCompletionPort( handle, m_hCompletionPort, key, 0 );
	
	return ( iocpHandle == m_hCompletionPort );
}

BOOL IOCompletionPort::getCompletionStatus( LPDWORD pdwOutBytesTransferred, DWORD *pOutCompKey, LPOVERLAPPED* pOutOverlapped, int* pnOutErrCode )
{
	BOOL ret = ::GetQueuedCompletionStatus( m_hCompletionPort, pdwOutBytesTransferred, pOutCompKey,	pOutOverlapped, INFINITE );
	
	if( !ret && pOutOverlapped != NULL )
	{
		*pnOutErrCode = GetLastError();
		return false;
	}
	
	return ret;
}

bool IOCompletionPort::threadCreate()
{
	AcceptThread* pAcptThread = new AcceptThread;
	THREADMANAGER->addThread( pAcptThread );
	printf("开始监听线程\n");
	// 弥柳 2005-10-25 +2甫 *2肺 利侩
	int iThreadCount = m_iMaxWorkerThread * 2;
	IOCPThread *pThread = NULL;
	
	for( int i = 0; i < iThreadCount; ++i )
	{
		pThread = new IOCPThread;
		pThread->m_iThreadID = i % m_iMaxWorkerThread;
		pThread->m_socket=INVALID_SOCKET;
		THREADMANAGER->addThread( pThread );
	}
	
	return true;
}

bool IOCompletionPort::serviceRun( WORD Port, WORD maxWorkerThread )
{
	if( !initialize( Port, maxWorkerThread ) )
		return false;
	
	if( !threadCreate() )
		return false;
	
	return true;
}

//add for tesging
IOCompletionPort::IOCompletionPort()
{
	m_hCompletionPort = NULL;
	m_iMaxWorkerThread = 0;
}

IOCompletionPort::~IOCompletionPort()
{
	CloseHandle(m_hCompletionPort);
	m_iMaxWorkerThread = 0;
	
	WSACleanup();
}
