// IOCPThread.cpp: implementation of the CIOCPThread class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "IOCPThread.h"
#include "ServiceModule.h"
#include "Synchronize.h"
#include "Session.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//  DECLARE_DYNCREATE와 IMPLEMENT_DYNCREATE 매크로들을 사용한다면, 
//  RUNTIME_CLASS 매크로와 CObject:을 사용할 수 있습니다 : 
//  실행 시간에 당신의 객체들의 클래스를 결정지는 IsKindOf 방법.

IMPLEMENT_DYNCREATE(CIOCPThread, CWinThread)

CIOCPThread::CIOCPThread()
{

	m_nTh = 0;
	m_nPreDay = -1;
	m_nTemp1 = 0;
}

CIOCPThread::~CIOCPThread()
{
}

BOOL CIOCPThread::InitInstance()
{
	return TRUE;
}

int CIOCPThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CIOCPThread, CWinThread)
//{{AFX_MSG_MAP(CIOCPThread)
// NOTE - the ClassWizard will add and remove mapping macros here.
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

int CIOCPThread::Run()
{

		DWORD dwBytesTrans=0;
		LPOVERLAPPED lpov;
		DWORD dwKey=0;
		BOOL fStatus=false;
	
		while(1)
		{
			try
			{				

				fStatus = GetQueuedCompletionStatus(m_pModule->GetIOCP(), &dwBytesTrans, &dwKey, &lpov, INFINITE);
				
				if( fStatus && dwKey && lpov && dwBytesTrans )
				{
					CSession* pSess = (CSession*)dwKey ;
					if( pSess->GetStatus()!=0)
					{
						int nError = pSess->Dispatch(dwBytesTrans, (LPOVERLAPPED2)lpov);
						if( nError != ERROR_NONE ) 
						{
							pSess->SetStatus(0);
							Sleep(100);
							m_pModule->RemoveSession(pSess, nError); 
							printf("close 3\n");
						}

					}
					else
					{
						pSess->SetStatus(0);
						Sleep(100);
						m_pModule->RemoveSession(pSess, 0); 
						printf("Close1\n");

					}
				}
				else
				{
					if(dwKey && GetLastError() != ERROR_OPERATION_ABORTED)
					{
						((CSession *)dwKey)->SetStatus(0);
						Sleep(100);
						ErrorInSession((CSession*)dwKey, (LPOVERLAPPED2)lpov);
						printf("Close2\n");
					}
					else if ( (dwBytesTrans == 0) && (dwKey == 0) && ((DWORD)lpov == 1) )
					{
						AfxEndThread( 0, TRUE );
						printf("Close2-1\n");
					}
					
				}

			}
			catch(...)
			{
				printf("Catch Error");
			}
			Sleep(100);
		}

	return 0;
}
bool CIOCPThread::SetModule( CServiceModule * pModule, int nTh )
{
	//	static nTh = 0;
	m_nTh = nTh;
	m_pModule = pModule;
	
	return true;
}

void CIOCPThread::ErrorInSession( CSession* pSession, LPOVERLAPPED2 lpov )
{
	int nError;
	
	if ( lpov )
	{
		if ( (lpov->dwFlag & 0xff) == PACKET_RECV )			nError = ERROR_RECV;
		else if ( (lpov->dwFlag & 0xff) == PACKET_SEND )	nError = ERROR_SEND;
		else												nError = ERROR_INVALIDMSG;
	}
	else	nError = ERROR_INVALIDMSG;
	
	pSession->CheckErrorReason(nError);
}

