// GameSession.cpp: implementation of the CGameSession class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameIOCP.h"
#include "GameSession.h"
#include "GameManagerModule.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGameSession::CGameSession(CGameManagerModule * pModule)
:CSession(pModule)
{
}

CGameSession::~CGameSession()
{
}

bool CGameSession::Create(SOCKET hSocket)
{
	return CSession::Create(hSocket);
}
bool CGameSession::Connect(LPCTSTR strAddr, int nPort)
{
	return CSession::Connect(strAddr,nPort);
}
int CGameSession::Receive(DWORD dwFlag)
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
		return ERROR_NONE;
	}
	else
	{
		return ERROR_RECVBUFFULL;
	}
}
int CGameSession::Send(LPBYTE pBuf, int nBytes)
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
void CGameSession::CheckErrorReason(int nError)
{
	CSession::CheckErrorReason(nError);
}
void CGameSession::Close()
{
	CSession::Close();

}
int	CGameSession::ParseNetworkMessage()
{
	return -1;
}