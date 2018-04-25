// CG2Session.cpp: implementation of the CCG2Session class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CG2.h"
#include "CG2Session.h"
#include "CG2ManagerModule.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCG2Session::CCG2Session(CCG2ManagerModule * pModule)
:CSession(pModule)
{
}

CCG2Session::~CCG2Session()
{
}

bool CCG2Session::Create(SOCKET hSocket)
{
	return CSession::Create(hSocket);
}
bool CCG2Session::Connect(LPCTSTR strAddr, int nPort)
{
	return CSession::Connect(strAddr,nPort);
}
int CCG2Session::Receive(DWORD dwFlag)
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
int CCG2Session::Send(LPBYTE pBuf, int nBytes)
{
	int i=0;
	int iSend=nBytes;
	if(nBytes>0)
	{
		if(nBytes>=NETWORK_BUF_SIZE)
		{
			i=0;
			while(iSend>0)
			{
				while(i<nBytes/NETWORK_BUF_SIZE)
				{
					if(m_nBytesInSendBuf==0)
					{
						CSession::Send(pBuf+i*NETWORK_BUF_SIZE,NETWORK_BUF_SIZE);
						iSend=iSend-NETWORK_BUF_SIZE;
						i++;
						printf("Send %i\n",i);
					}
					else
					{
						Sleep(500);
					}
				}
				if(m_nBytesInSendBuf==0)
				{
					printf("Send last\n");
					CSession::Send(pBuf+nBytes/NETWORK_BUF_SIZE*NETWORK_BUF_SIZE,nBytes%NETWORK_BUF_SIZE);
					iSend=iSend-NETWORK_BUF_SIZE;
				}
				else
				{
					Sleep(500);
				}

			}
			/*for(int i=0;i<nBytes/NETWORK_BUF_SIZE;i++)
			{
				if(CSession::Send(pBuf+i*NETWORK_BUF_SIZE,NETWORK_BUF_SIZE)==N)
				if(CSession::Send(pBuf+i*NETWORK_BUF_SIZE,NETWORK_BUF_SIZE))
				{
					printf("send %i\n",i);
					Sleep(1000);
			 	}
				
		}*/
		
		/*	if(m_nBytesInSendBuf==0)
			{
				printf("Send last\n");
				CSession::Send(pBuf+nBytes/NETWORK_BUF_SIZE*NETWORK_BUF_SIZE,nBytes%NETWORK_BUF_SIZE);
			}
			else
			{
				Sleep(500);
				CSession::Send(pBuf+nBytes/NETWORK_BUF_SIZE*NETWORK_BUF_SIZE,nBytes%NETWORK_BUF_SIZE);
			}*/
		}
		else
		{
			CSession::Send(pBuf,nBytes);
		}
	}

	return nBytes;
}
void CCG2Session::CheckErrorReason(int nError)
{
	CSession::CheckErrorReason(nError);
}
void CCG2Session::Close()
{
	CSession::Close();

}
int	CCG2Session::ParseNetworkMessage()
{
	return -1;
}