// ServiceManagerModule.cpp: implementation of the CServiceManagerModule class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GMTOOLS.h"
#include "ServiceManagerModule.h"
#include "ServerSession.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
UINT UpdateThread(LPVOID lpParameter)
{
	try
	{
		char path[256];
		GetCurrentDirectory(256,path);//获取当前目录
		char filepath[256];
		sprintf(filepath,"%s%s",path,"\\config\\scheme.ini");//获取scheme.ini文件路径
		
		int SynTime=GetPrivateProfileInt("SERVER","SynTime",20000,filepath);//得到监听端口号
		
		CServiceManagerModule * pModule = (CServiceManagerModule*)lpParameter;
		while(true)
		{
			pModule->ActiveNow();
			Sleep(SynTime);
		}
	}
	catch(...)
	{
	}
/*	catch (CMemoryException* e)
	{
		
	}
	catch (CFileException* e)
	{
	}
	catch (CException* e)
	{
	}*/
	
	return 0;
}

CServiceManagerModule::CServiceManagerModule()
{

}

CServiceManagerModule::~CServiceManagerModule()
{
	Destory();
}
bool CServiceManagerModule::Init()
{
	try
	{
		char path[256];
		GetCurrentDirectory(256,path);//获取当前目录
		char filepath[256];
		sprintf(filepath,"%s%s",path,"\\config\\scheme.ini");//获取scheme.ini文件路径
		
		int MaxClient=GetPrivateProfileInt("SERVER","MaxClient",10,filepath);//得到监听端口号
		
		for( int i = 0 ; i < MaxClient+1; i++)
			m_listInActive.AddTail( new CServerSession(this) );
	}
	catch(...)
	{
	}
/*	catch (CMemoryException* e)
	{
		
	}
	catch (CFileException* e)
	{
	}
	catch (CException* e)
	{
	}*/


	return TRUE;
}

bool CServiceManagerModule::InitNetwork()
{

	return CServiceModule::InitNetwork();
}
bool CServiceManagerModule::InitWorkerThread(int nWorkerCount)
{
	return CServiceModule::InitWorkerThread(nWorkerCount);
}
void CServiceManagerModule::CleanUpNetwork()
{

}
void CServiceManagerModule::AcceptNewSession(SOCKET hSocket)
{
	CServiceModule::AcceptNewSession(hSocket);
}


bool CServiceManagerModule::OnStartService()
{
	try
	{
		char path[256];
		GetCurrentDirectory(256,path);//获取当前目录
		char filepath[256];
		sprintf(filepath,"%s%s",path,"\\config\\scheme.ini");//获取scheme.ini文件路径
		
		int WorkThread=GetPrivateProfileInt("SERVER","WorkThread",10,filepath);//得到监听端口号
		Init();
		InitNetwork();
		InitWorkerThread(WorkThread);
		if( AfxBeginThread( UpdateThread, this) == NULL )
			return false;
	}
	catch(...)
	{
	}
/*	catch (CMemoryException* e)
	{
		
	}
	catch (CFileException* e)
	{
	}
	catch (CException* e)
	{
	}*/
	return TRUE;
}
bool CServiceManagerModule::OnStopService()
{
	return TRUE;
}
bool CServiceManagerModule::OnInterrogateService()
{
	return TRUE;
}

bool CServiceManagerModule::ActiveNow()
{
	try
	{
		if(!m_listActive.IsEmpty())
		{
			int iNum=m_listActive.GetCount();
			CServerSession * pSess=(CServerSession*)m_listActive.GetHead();
			POSITION pos = m_listActive.GetHeadPosition();
			for (int i=0;i < iNum;i++)
			{
				pSess=(CServerSession*)m_listActive.GetNext(pos);
				if(pSess!=NULL)
				{
					if(((CServerSession *)pSess)->GetStatus()!=1)
					{
						((CServerSession *)pSess)->SetStatus(0);
						Sleep(500);
						RemoveSession(pSess, 0); 
					}
					else
					{
						((CServerSession *)pSess)->SendActive();
						Sleep(1000);
					}
				}
				
			}
		}
	}
	catch(...)
	{
	}
/*	catch (CMemoryException* e)
	{
		
	}
	catch (CFileException* e)
	{
	}
	catch (CException* e)
	{
	}*/
	return true;
}

void CServiceManagerModule::Destory()
{
	try
	{
		CSession * pSession;
		
		while( !m_listActive.IsEmpty())
		{
			pSession = m_listActive.RemoveHead();
			pSession->Close();
			delete pSession;
		}
		
		while( !m_listInActive.IsEmpty())
		{
			pSession = m_listInActive.RemoveHead();
			pSession->Close();
			delete pSession;
		}
		
		CleanUpNetwork();
	}
	catch(...)
	{
	}
/*	catch (CMemoryException* e)
	{
		
	}
	catch (CFileException* e)
	{
	}
	catch (CException* e)
	{
	}*/
	
}

