// ServiceModule.cpp: implementation of the CServiceModule class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NetIOCP.h"
#include "ServiceModule.h"
#include "Synchronize.h"
#include "IOCPThread.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

HANDLE g_hInterrogate;
HANDLE g_hEndService;

CServiceModule::CServiceModule()
{
	
}

CServiceModule::~CServiceModule()
{
	if( m_sockListen != INVALID_SOCKET )
		closesocket(m_sockListen);
	
	while(!m_listActive.IsEmpty())
		delete m_listActive.RemoveHead();
	
	while(!m_listInActive.IsEmpty())
		delete m_listInActive.RemoveHead();
	
	CleanUpNetwork();
}

//接受新的客户端并且创建新的线程
UINT AcceptThread(LPVOID lpParameter)
{
	SOCKET sock;
	CServiceModule* pModule = (CServiceModule*)lpParameter;
	bool m_bOnlyLocalServer = pModule->m_bOnlyLocalServer;
	
	SOCKADDR_IN sockaddrIn;
	int nLengthAddr;
	ZeroMemory( &sockaddrIn, sizeof(sockaddrIn));
	
	HANDLE ah[2] = {pModule->m_hListenEvent, pModule->m_hKillListeningThread};
	while(1)
	{	
		DWORD dwRet = WSAWaitForMultipleEvents(2, ah, FALSE, WSA_INFINITE, FALSE);
		switch(dwRet)
		{
		case WSA_WAIT_EVENT_0:
			nLengthAddr = sizeof(SOCKADDR);
			sock = accept(pModule->GetsockListen(), (LPSOCKADDR)&sockaddrIn, &nLengthAddr);
			if(sock != INVALID_SOCKET)
			{
				pModule->AcceptNewSession(sock);
				printf("Accept\n");
			}
			break;
		case WSA_WAIT_EVENT_0+1:
			{
				printf("Listen Error\n");
				SetEvent(pModule->m_hListeningThreadKilled);
				AfxEndThread(0, TRUE);
			}
			break;
		}
	}
	return 0;
}

//初始化网络信息
bool CServiceModule::InitNetwork()
{
	// initialize Winsock library
	WSADATA wsaData;
	
	WORD wVersionRequested = MAKEWORD(2,2);
	int nResult = WSAStartup(wVersionRequested, &wsaData);
	if(nResult != 0)
		return FALSE;
	
	if( LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2 )
	{
		WSACleanup();
		return FALSE;
	}
	
	struct sockaddr_in serv_addr;
	int reuse = 1;
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	
	serv_addr.sin_family	=	AF_INET ;
	// Host Name
	PHOSTENT	pHostEntry;
	IN_ADDR		inAddr;
	char		szHostName[255];
	if(!gethostname(szHostName,255))
		pHostEntry = gethostbyname(szHostName);
	memcpy(&inAddr,pHostEntry->h_addr, 4); 
	m_strHostName = pHostEntry->h_name;
	
	char path[256];
	GetCurrentDirectory(256,path);//获取当前目录
	char filepath[256];
	sprintf(filepath,"%s%s",path,"\\config\\scheme.ini");//获取scheme.ini文件路径
	char lpIP[40];
	GetPrivateProfileString("SERVER","Address","127.0.0.1",lpIP,40,filepath);
	m_strIPAddr=(LPCTSTR)&lpIP;
//	serv_addr.sin_addr.s_addr = inet_addr(lpIP);
	if( m_strIPAddr == "")
	{
		serv_addr.sin_addr.s_addr = htons(INADDR_ANY);
		m_strIPAddr = inet_ntoa(inAddr);
		printf("%s\n",m_strIPAddr);
	}
	else
	{
		serv_addr.sin_addr.s_addr = inet_addr(m_strIPAddr);
	}
	serv_addr.sin_port	=	htons(m_nPort);
	
	m_sockListen = socket(AF_INET , SOCK_STREAM, 0);
	setsockopt(m_sockListen, SOL_SOCKET, ~SO_REUSEADDR, (char*)&reuse, sizeof(reuse)); 
	
	if(m_sockListen == INVALID_SOCKET || 
		bind(m_sockListen, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR ||
		listen(m_sockListen, 5/*SOMAXCONN*/) == SOCKET_ERROR )
	{
		printf("服务器监听失败!");
		return FALSE;
	}
		
	
	m_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if(m_hIOCP == INVALID_HANDLE_VALUE)
		return FALSE;
	g_hInterrogate = CreateEvent(NULL, FALSE, FALSE, NULL);
	g_hEndService  = CreateEvent(NULL, FALSE, FALSE, NULL);
	
	return TRUE;
}
//初始化工作线程信息
bool CServiceModule::InitWorkerThread(int nWorkerCount)
{
	m_hListenEvent				= CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hKillListeningThread		= CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hListeningThreadKilled	= CreateEvent(NULL, FALSE, FALSE, NULL);
	
	if( AfxBeginThread( AcceptThread, this) == NULL )
		return false;
	if( WSAEventSelect( m_sockListen, m_hListenEvent, FD_ACCEPT) == SOCKET_ERROR )
		return false;
	
	CIOCPThread* pThread;
	for(int i = 0 ; i < nWorkerCount ; i++)
	{
		pThread = (CIOCPThread*)AfxBeginThread(RUNTIME_CLASS(CIOCPThread), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
		
		
		if( !pThread->SetModule( this, i ))
		{
			ASSERT(0);
			return false;
		}

		m_nIOCPThreadIDs[i] = pThread->m_nThreadID;
		pThread->ResumeThread();
	}
	return true;
}
//进入1个新的客户端，创建1个新会话
void CServiceModule::AcceptNewSession(SOCKET hSocket)
{
	Synchronize s(m_csSessionList);

	printf("Accept\n");
	if(m_listInActive.GetCount()>1)
	{
		CSession* pSess = m_listInActive.RemoveHead();
		if(!pSess->Create(hSocket))// 家南 可记 汲沥秦林绊 IOCP俊 殿废
		{
			pSess->Close();
			m_listInActive.AddTail(pSess); // 后技记篮 咯弊俊 嘿烙	
		}
		else
		{
			m_listActive.AddTail(pSess); // 货肺款 技记篮 咯弊俊 嘿烙
			if(!m_listActive.IsEmpty())
			{
				for(int i=0;i<m_listActive.GetCount();i++)
				{
					printf("连接%i",i);
				}
			}
		}
		return ;
	}
	else 
	{
		//超过最大连接数
		if(!m_listActive.IsEmpty())
		{
			int iNum=m_listActive.GetCount();
			CSession * pSess=(CSession*)m_listActive.GetHead();
			POSITION pos = m_listActive.GetHeadPosition();
			for (int i=0;i < iNum;i++)
			{
				pSess=(CSession*)m_listActive.GetNext(pos);
				if(pSess!=NULL)
				{
					Sleep(500);
					RemoveSession(pSess, 0); 
				}
				
			}
		}
		send(hSocket,"超过最大连接数",strlen("超过最大连接数"),0);
		closesocket(hSocket);
	}

}
//客户端退出，从用户列表移除1个客户端的会话
bool CServiceModule::RemoveSession(CSession* pSession, int nError)
{
	POSITION	pos;
	
	CSession* pSess = pSession;
	
	Synchronize s(m_csSessionList);
	
	pos = m_listActive.Find(pSession);
	if( pos )
	{
		pSess->SetStatus(0);
		pSess->Close();
		m_listActive.RemoveAt(pos);
		m_listInActive.AddTail(pSess);
		return true;
	}
	return false;
}
//服务器端口，移除所有网络的客户端
void CServiceModule::CleanUpNetwork()
{
	if( m_hListenEvent )			CloseHandle(m_hListenEvent); m_hListenEvent = NULL;
	if( m_hKillListeningThread )	CloseHandle(m_hKillListeningThread); m_hKillListeningThread = NULL;
	if( m_hListeningThreadKilled )	CloseHandle(m_hListeningThreadKilled); m_hListeningThreadKilled = NULL;
	if( g_hInterrogate )			CloseHandle(g_hInterrogate); g_hInterrogate = NULL;
	if( g_hEndService )				CloseHandle(g_hEndService); g_hEndService = NULL;
	if( m_hIOCP )					CloseHandle(m_hIOCP); m_hIOCP = NULL;
	
	WSACleanup();
}
