#include "stdafx.h"
#include ".\msocket.h"


#define MAXBUFFER 2048*1024

CMSocket::CMSocket(void)
{
	lpRecvbuf = NULL;
	
	
}

CMSocket::~CMSocket(void)
{
	MyDestory();
}

//初始化SOCKET
void CMSocket::initSocket()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 2, 2 );

	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		return;
	}

	if ( LOBYTE( wsaData.wVersion ) != 2 ||
		HIBYTE( wsaData.wVersion ) != 2 ) {
			/* Tell the user that we could not find a usable */
			/* WinSock DLL.                                  */
			WSACleanup( );
			return; 
		}
}

//连接远程服务器
int CMSocket::ConnectURL(char* m_ip,int m_port)
{
	initSocket();
    m_socket = socket(AF_INET,SOCK_STREAM, 0 );

	char MesHint[64];
	ZeroMemory(MesHint, 64);
	if(m_socket == INVALID_SOCKET)
	{
		sprintf(MesHint, "Create Socket Error %d", WSAGetLastError());
		cout<<MesHint<<endl;
	}
	{
		struct sockaddr_in	serv_addr;
		ZeroMemory(&serv_addr, sizeof(serv_addr));
		serv_addr.sin_family      = AF_INET;
		serv_addr.sin_addr.s_addr = inet_addr(m_ip);
		serv_addr.sin_port   = htons(m_port);
		if(connect(m_socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) != 0 )
		{
			sprintf(MesHint, "Can't Connect to Server:%s:[%d]", m_ip, m_port);
		    cout<<MesHint<<endl;
		}
		else
		{
			sprintf(MesHint, "Connect to Server:%s:[%d] Successfully",m_ip,m_port);
		    cout<<MesHint<<endl;
			return 1;
		}
	}
	return 0;
}

int CMSocket::MyReceive(unsigned char * lpRecv)
{
	int tmpLength = 0;
	if((tmpLength = recv(m_socket,(char *)lpRecv,MAXBUFFER,0)) == SOCKET_ERROR)
	{
		return -1;
	}
	return tmpLength;
}
// 接收数据
int CMSocket::MyReceive()
{
	MyDestory();

	lpRecvbuf = new unsigned char[MAXBUFFER];
	ZeroMemory(lpRecvbuf, MAXBUFFER);
	int actlength=0;
	int i=1;
	int tmpLength = 0;
	while(true)
	{
		tmpLength = recv(m_socket,(char *)lpRecvbuf,MAXBUFFER,0);
		i++;
		if(tmpLength==SOCKET_ERROR)
		{
			return -1;
		}
		else if(tmpLength>0)
		{
			break;
		}
		if(i>3)
		{
			printf("接受错误\n");
			break;
		}
	}
	actlength = actlength + tmpLength;
	return actlength;
}

//接收数据
int CMSocket::ReceiveBuf(char* buf,int length)
{
	int recved=0;
	while(recved<length)
	{
		int now=recv(m_socket, buf+recved, length-recved,0);
		if(now==-1||now==0) break;
		recved+=now;
	}
	return recved;
}

// 接收数据
int CMSocket::recvData()
{
	int recved=0;
	try
	{
		MyDestory(); // for testing
		
		
		lpRecvbuf = new unsigned char[MAXBUFFER];
		ZeroMemory( lpRecvbuf, MAXBUFFER);
		recved = ReceiveBuf((char*)lpRecvbuf, 2);
		if( recved != 2)
		{
			return recved;
		}
		else
		{
			WORD size = (WORD)(lpRecvbuf[1]*256+lpRecvbuf[0]);
			if(size > 0)
			{
				recved = ReceiveBuf((char*)lpRecvbuf + 2,size-2);
				if(recved >= size-2 )
				{
					return recved+2;
				}
			}
		}
		
		
	}
	catch(...)
	{
		return -1;
	}
	
	return 0;
}
// 接收数据
int CMSocket::recvWAData()
{
	int recved=0;
	try
	{
		MyDestory(); // for testing
		
		
		lpRecvbuf = new unsigned char[MAXBUFFER];
		ZeroMemory( lpRecvbuf, MAXBUFFER);
		recved = ReceiveBuf((char*)lpRecvbuf, 2);
		if( recved != 2)
		{
			return recved;
		}
		else
		{
			WORD size = (WORD)(lpRecvbuf[1]*256+lpRecvbuf[0]);
			if(size > 0)
			{
				recved = ReceiveBuf((char*)lpRecvbuf + 2,size);
				if(recved >= size )
				{
					return recved+2;
				}
			}
		}
		
		
	}
	catch(...)
	{
		return -1;
	}
	
	return 0;
}
// 发送部分数据
int CMSocket::SendBuf(char* buff,int length)
{
	int sended = 0;
	int now = 0;
	int sendtime=0;
	while(sended < length)
	{
	//	(CSession * )m_socket->Send(buff+sended,length-sended);
		now = send(m_socket, buff+sended, length-sended ,0);
		if(now==SOCKET_ERROR)
		{
			break;
		}
		sended += now;
		sendtime++;
		if(sendtime>3)
		{
			return -1;
		}
	}
	return sended;
}

// 发送全部数据并判断
bool CMSocket::SendData(char *msg,int wSize)
{
	return SendBuf(msg, wSize) >= wSize;
}
bool CMSocket::SendNewData(char *msg,int wSize)
{
	return SendNewBuf(msg, wSize) >= wSize;
}

// 发送部分数据
int CMSocket::SendNewBuf(char* buff,int length)
{
	int sended = 0;
	int now = 0;
	int sendtime=0;
	while(sended < length)
	{
		now = send(m_socket, buff+sended, length-sended ,0);
		if(now==SOCKET_ERROR)
		{
			break;
		}
		sended += now;
		sendtime++;
		if(sendtime>3)
		{
			return -1;
		}
	}
	return sended;
}
// 设置m_socket
void CMSocket::SetSocket(SOCKET tmpsocket)
{
	m_socket = tmpsocket;  
}

//获取m_socket
SOCKET CMSocket::GetSocket()
{
	return m_socket;
}

//关闭SOCKET
void CMSocket::closeSocket()
{
	closesocket(m_socket);
}

// 销毁m_socket
bool CMSocket::MyDestory(void)
{
	if(lpRecvbuf != NULL)
	{
		delete[] lpRecvbuf;
		lpRecvbuf = NULL;
	}
	return false;
}

//返回接收到的数据
unsigned char* CMSocket::recvBuffer()
{
	return lpRecvbuf;
}
