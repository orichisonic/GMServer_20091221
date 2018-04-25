// MySocket.cpp: implementation of the CMySocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MySocket.h"
#include <assert.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMySocket::CMySocket()
{
	m_socket = INVALID_SOCKET;
}

CMySocket::~CMySocket()
{
	if (m_socket != INVALID_SOCKET)
	{
		::closesocket(m_socket);
		m_socket = INVALID_SOCKET;
	}
}

BOOL CMySocket::Initialize()
{
	WSADATA wsaData;
	int nRet = ::WSAStartup(0x0202, &wsaData);
	if (0 != nRet)
	{
		return FALSE;
	}
	
	return TRUE;
}

BOOL CMySocket::Uninitialize()
{
	int nRet = ::WSACleanup();
	if (0 != nRet)
	{
		return FALSE;
	}
	
	return TRUE;
}

BOOL CMySocket::MyConnect(LPCSTR lpIP, int dwPort, int timeout)
{
	ASSERT(NULL != lpIP);
	ULONG NonBlock=1;	
	fd_set fdConnect;
	FD_ZERO(&fdConnect);
	timeval lmtTime;
	lmtTime.tv_sec = timeout;
	lmtTime.tv_usec = 0;
	
	if((m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == SOCKET_ERROR)
	{
		return FALSE;
	}

	FD_SET(m_socket, &fdConnect);

	SOCKADDR_IN ServerAddr;
	memset(&ServerAddr, 0, sizeof(SOCKADDR_IN));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_addr.s_addr = inet_addr(lpIP);
	ServerAddr.sin_port = htons(LOWORD(dwPort));

	ioctlsocket(m_socket, FIONBIO, &NonBlock);
 	
	int nRet = ::connect(m_socket, (LPSOCKADDR)&ServerAddr, sizeof(ServerAddr));
	nRet = select(0, NULL, &fdConnect, NULL, &lmtTime);
	if (!nRet)
	{
		NonBlock = 0;
		ioctlsocket(m_socket, FIONBIO, &NonBlock);
		return FALSE;
	}

	FD_ZERO(&fdConnect);
	NonBlock = 0;
	ioctlsocket(m_socket, FIONBIO, &NonBlock);
	return TRUE;
}

BOOL CMySocket::MySend(LPCSTR lpData, const DWORD dwBytes)
{
	int sendbyte;	//实际发送字节数	
	if ((sendbyte=::send(m_socket, lpData, dwBytes, 0)) == dwBytes)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

int CMySocket::MyRecv(LPSTR lpcbBuff /* =NULL */, DWORD dwBuffSize, long lTime /*=10*/)
{
	timeval waittime;
	waittime.tv_usec = 0;
	waittime.tv_sec = lTime;

	int ret;
	int recvSize =0; 
	fd_set fdread;
	FD_ZERO(&fdread);
	FD_SET(m_socket, &fdread);

	ret = select(0, &fdread, NULL, NULL, &waittime);
	int error;
	if (ret > 0)
	{
		if (FD_ISSET(m_socket, &fdread))
		{
			recvSize =::recv(m_socket, lpcbBuff, dwBuffSize, 0);
			error = WSAGetLastError();
		}
	}

	FD_ZERO(&fdread);

	return recvSize;
}

BOOL CMySocket::Close()
{
	if (INVALID_SOCKET == m_socket)
		return FALSE;
	
	if (SOCKET_ERROR == ::closesocket(m_socket))
	{
		return FALSE;
	}
	
	m_socket = INVALID_SOCKET;
	
	return TRUE;
}
