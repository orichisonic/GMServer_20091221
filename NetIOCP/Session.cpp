// Session.cpp: implementation of the CSession class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Session.h"
#include "ServiceModule.h"
#include "Synchronize.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CSession::CSession(CServiceModule* pModule)
:m_pModule(pModule)
{
	m_hSocket		= INVALID_SOCKET;
	m_nReadCount	= 0;
//	m_nSendCount	= 0;
	m_bSendInProgress	= false;
	m_bIsActive			= false;
	i_status=0;

	m_pSendBuf = new BYTE[NETWORK_BUF_SIZE * 2];
	ZeroMemory(m_pSendBuf, NETWORK_BUF_SIZE*2);
}

CSession::~CSession()
{
	Close();
}

bool CSession::AssociateIOCP()
{
	if (CreateIoCompletionPort((HANDLE)m_hSocket, m_pModule->GetIOCP(), (DWORD)this, 0) != NULL)
	{
		m_bIsActive = true;
		i_status=1;

		m_msg.Clear();
		m_nReadCount = 0;

		m_nBytesInSendBuf = 0;
		m_bSendInProgress = false;

		Receive(0);
		return true;
	}
	else
		return false;
}

bool CSession::Create(SOCKET hSocket)
{
	Close();
	m_hSocket	=	hSocket;
	if(m_hSocket == INVALID_SOCKET)
		return false;

	
	u_long argp = 1;
	ioctlsocket(m_hSocket, FIONBIO, &argp);

	sockaddr_in peeraddr;
	int nNamelen = sizeof(sockaddr_in);

	if( getpeername(m_hSocket, (SOCKADDR*)&peeraddr, &nNamelen))
	{
		return false;
	}

	m_strIPAddr = inet_ntoa(peeraddr.sin_addr);

	return AssociateIOCP();
}

bool CSession::Connect(LPCTSTR strAddr, int nPort)
{
	Close();
	
	m_hSocket = socket(AF_INET, SOCK_STREAM, 0);
	if( m_hSocket == INVALID_SOCKET)
		return FALSE;

	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family		= AF_INET;
	serv_addr.sin_addr.s_addr	= inet_addr(strAddr);
	serv_addr.sin_port			= htons(nPort);

	connect(m_hSocket, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(m_hSocket, &fds);

	struct timeval t;
	t.tv_sec = 5;
	t.tv_usec = 0;

	select (0, NULL, &fds, NULL, &t);

	if (!FD_ISSET(m_hSocket, &fds))
	{
		closesocket (m_hSocket);
		m_hSocket = NULL;
		return false;
	}
	else
		return AssociateIOCP();
}

void CSession::Close()
{
	m_bIsActive = false;
	i_status=0;
	m_strIPAddr	=	"";
	m_strUserID =   "";

	if( m_hSocket != INVALID_SOCKET )
	{
		shutdown(m_hSocket, SD_SEND );
		closesocket(m_hSocket);
		CLogic::DelLog(m_hSocket);
		m_hSocket = INVALID_SOCKET;
	}
	m_nReadCount = 0;
	m_msg.Clear();
}

int CSession::Dispatch(int nBytes, LPOVERLAPPED2 lpov)
{
	if( lpov == NULL )
		return ERROR_INVALIDMSG;

	if((lpov->dwFlag & 0xff) == PACKET_RECV)
		return DispatchRecv(nBytes, lpov);

	else if((lpov->dwFlag & 0xff) == PACKET_SEND)
		return DispatchSend(nBytes, lpov);
	
	else
	{
		return ERROR_INVALIDMSG;
	}
		
}

int	CSession::DispatchRecv(int nBytes, LPOVERLAPPED2 lpov)
{
//	m_nReadCount += nBytes;
	m_nReadCount=nBytes;
	int nSize = m_msg.GetSize();
	while((m_nReadCount >= NETWORK_MSG_HEADER) && (m_nReadCount >= nSize))
	{
		int nError = ParseNetworkMessage();
		if( nError != ERROR_NONE)
			return nError;
		RemoveMsg();
		nSize = m_msg.GetSize();
	}
	return Receive(PACKET_NORMAL);
}

int	CSession::DispatchSend(int nBytes, LPOVERLAPPED2 lpov)
{
	Synchronize s(m_csSendBuf);
	

	m_nBytesInSendBuf -= nBytes;

	ASSERT(m_nBytesInSendBuf >= 0);

	if( m_nBytesInSendBuf < 0 )
	{
		m_nBytesInSendBuf  = 0 ;
	}
	else
		memcpy(m_pSendBuf, m_pSendBuf + nBytes, m_nBytesInSendBuf);

	int nError;
	if (m_nBytesInSendBuf)
		nError = SendDataInBuf();
	else
	{
		nError = ERROR_NONE;
		m_bSendInProgress = false;
	}
	
	return nError;

}

// !!! Caller must Lock m_csSendBuf before call SendDataInBuf function !!!
int CSession::SendDataInBuf()
{
	DWORD dwBytesWritten;

	m_bSendInProgress = true;
	memset(&m_ovSend, 0, sizeof(OVERLAPPED2));
	m_ovSend.dwFlag = PACKET_SEND;

	if (!WriteFile((HANDLE)m_hSocket, m_pSendBuf, m_nBytesInSendBuf, &dwBytesWritten, (LPOVERLAPPED)&m_ovSend))
	{
		int nLastError = GetLastError();
		if ((nLastError != ERROR_IO_PENDING) && (nLastError != ERROR_SUCCESS))
		{
//			LOG_FILE(LOG_FILEPATH, "ERROR WriteFile = %d \n", nLastError);
			return ERROR_SEND2;
		}
			
	}
	return ERROR_NONE;
}

int	CSession::Send(LPBYTE pBuf, int nBytes)
{
	Synchronize s(m_csSendBuf);

	ASSERT(m_nBytesInSendBuf + nBytes <= NETWORK_BUF_SIZE + 2);
	if (m_nBytesInSendBuf + nBytes > NETWORK_BUF_SIZE + 2)
	{
		return ERROR_SENDBUFUNFL;
	}

	memcpy(m_pSendBuf + m_nBytesInSendBuf, pBuf, nBytes);
	m_nBytesInSendBuf += nBytes;
//	memcpy(m_pSendBuf, pBuf, nBytes);
//	m_nBytesInSendBuf=0;

	int nError = ERROR_NONE;
	if (!m_bSendInProgress)
		nError = SendDataInBuf();
	return nError;
}

int CSession::Receive(DWORD dwFlag)
{
	
	if( NETWORK_BUF_SIZE > m_nReadCount )
	{
		memset(&m_ovRecv, 0, sizeof(OVERLAPPED2));
		m_ovRecv.dwFlag = dwFlag |PACKET_RECV ;

		if( !ReadFile((HANDLE)m_hSocket, m_msg.GetBuf() + m_nReadCount, NETWORK_BUF_SIZE - m_nReadCount , NULL,(LPOVERLAPPED)&m_ovRecv))
		{
				int nLastError = GetLastError();
				if((nLastError != ERROR_IO_PENDING) && (nLastError != ERROR_SUCCESS))
				{
					return ERROR_RECV2;
				}
		}


//		LPBYTE lpBYTE=m_msg.GetBuf() + m_nReadCount;
	//	printf("%d %i  \r\n",m_msg.GetBuf() + m_nReadCount,NETWORK_BUF_SIZE - m_nReadCount);

	
		return ERROR_NONE;
	}
	else
	{
		return ERROR_RECVBUFFULL;
	}
}

void CSession::RemoveMsg()
{
	if(m_nReadCount < NETWORK_MSG_HEADER)
		return;
	int nSize = m_msg.GetSize();
	m_nReadCount -= nSize;
	memcpy(m_msg.GetBuf(), m_msg.GetBuf() + nSize, m_nReadCount);
	m_msg.Clear();
}

void CSession::CheckErrorReason(int nError)
{
	m_pModule->RemoveSession(this, nError);	
}
