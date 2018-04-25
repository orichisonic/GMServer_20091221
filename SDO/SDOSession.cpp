// SDOSession.cpp: implementation of the CSDOSession class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SDO.h"
#include "SDOSession.h"
#include "SDOManagerModule.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSDOSession::CSDOSession(CSDOManagerModule * pModule)
:CSession(pModule)
{
}

CSDOSession::~CSDOSession()
{
}

bool CSDOSession::Create(SOCKET hSocket)
{
	return CSession::Create(hSocket);
}
bool CSDOSession::Connect(LPCTSTR strAddr, int nPort)
{
	return CSession::Connect(strAddr,nPort);
}
int CSDOSession::Receive(DWORD dwFlag)
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
int CSDOSession::Send(LPBYTE pBuf, int nBytes)
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
void CSDOSession::CheckErrorReason(int nError)
{
	CSession::CheckErrorReason(nError);
}
void CSDOSession::Close()
{
	CSession::Close();

}
int	CSDOSession::ParseNetworkMessage()
{
	return -1;
}