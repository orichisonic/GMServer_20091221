#include "stdafx.h"
#include "IOCompletionPort.h"

//�����̣߳��������Կͻ��˵�����
DWORD AcceptThread::run()
{
	int nIndex=0;
	int i=1;

	WSANETWORKEVENTS wsaevent;
	WSAEVENT e=WSACreateEvent();
	WSAEventSelect(m_hServerSocket,e,FD_ACCEPT);//����ѡ��������Ϣ
	while( WaitForSingleObject ( m_eventThreadStop, 0 ) != WAIT_OBJECT_0 )//ѭ���������Կͻ��˵�����
	{
		nIndex=WSAWaitForMultipleEvents(1,&e,false,INFINITE,false);//�ȴ���Ϣ��Ϣ
		WSAEnumNetworkEvents(m_hServerSocket,e,&wsaevent);
		if(wsaevent.lNetworkEvents&FD_ACCEPT)//����ͨ���߳�
		{
			IOCPThread* pThread = new IOCPThread;
		
			pThread->m_iThreadID=i;
			pThread->m_socket=accept(m_hServerSocket,NULL,NULL);
			i++;
			THREADMANAGER->addThread(pThread);//��ʼ����ͨ���߳�
		//	printf("�������Կͻ��˵����� �̺߳ţ�%i  SOCKET��: %i\n", i-1, pThread->m_socket);
		}
	}	

	WSACloseEvent(e);
	return ( 0 );
}

// ͨ���߳�
DWORD IOCPThread::run()
{
	int nIndex=0;
	int i=0;
	bool m_result;
	
	CTasker * pThread = new CTasker;
	pThread->m_socket=(SOCKET)this->m_socket;
	THREADMANAGER->addThread(pThread);//��ʼ���з����̣߳�ÿ���ͻ����������߳�Ϊ�����
	
	WSANETWORKEVENTS wsaevent;
	WSAEVENT e=WSACreateEvent();
	WSAEventSelect((SOCKET)this->m_socket,e,FD_READ|FD_CLOSE);//���ն�ȡ��ر��¼�
	
	while( TRUE )
	{
		WSAWaitForMultipleEvents(1,&e,false,INFINITE,false);
		WSAEnumNetworkEvents((SOCKET)this->m_socket,e,&wsaevent);
		if(wsaevent.lNetworkEvents & FD_READ)//�ͻ��������ȡ�¼�
		{
			CTasker * pDoThread=NULL;
			pDoThread=THREADMANAGER->FindThread((SOCKET)this->m_socket);
			pDoThread->Receive();
			
			printf("IOCPThread::run, handle: %d\r\n", pDoThread->m_thread); // for testing
		}
		else if(wsaevent.lNetworkEvents & FD_CLOSE)//�ر��¼�
		{	
			printf("�ر�ͨ���߳�  SOCKET��: %i\n",(int)(SOCKET)this->m_socket);
			WSACloseEvent(e);
			CLogic::DelLog((SOCKET)this->m_socket);//ɾ���û�����
			pThread->EndThread();//�����߳�
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
	//	setsockopt( m_hServerSocket, SOL_SOCKET, (int)~SO_REUSEADDR, (char*)&reuse, sizeof(reuse) );//��ռռ�ж˿ں͵�

	}

#ifdef USE_NONE_BLOCK
	{
		u_long argp = 1;
		ioctlsocket( m_hServerSocket, FIONBIO, &argp );//������SOCKET��Ϊ������ʽ��
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
	serv_addr.sin_port = (WORD)htons(port);//���ö˿�
	serv_addr.sin_addr.S_un.S_addr = htons( INADDR_ANY );

	ret = ::bind( m_hServerSocket, (LPSOCKADDR)&serv_addr, sizeof(serv_addr) );//������SOCKET��ָ���˿ڰ�����
	

	if( ret == SOCKET_ERROR )
		return false;

	return true;
}

bool IOCompletionPort::listen( int backlog )
{
	return ( ::listen( m_hServerSocket, backlog ) != SOCKET_ERROR );//����ָ����Ŀ������
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

// ���� 2005-03-18
bool IOCompletionPort::initializeOnlyIOCP( WORD workerThreadCount )
{
	if( !startupSocket() )
		return false;
	if( !createCompletionPort( workerThreadCount ) )
		return false;

	return true;
}

// ���� 2005-03-18
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
	printf("��ʼ�����߳�\n");
	// ���� 2005-10-25 +2�� *2�� ����
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
