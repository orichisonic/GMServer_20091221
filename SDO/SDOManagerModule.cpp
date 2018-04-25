// SDOManagerModule.cpp: implementation of the CSDOManagerModule class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SDO.h"
#include "SDOManagerModule.h"
#include "SDOSession.h"

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
	CSDOManagerModule * pModule = (CSDOManagerModule*)lpParameter;
	while(true)
	{
		pModule->ActiveNow();
		Sleep(200);
	}
	return 0;
}

CSDOManagerModule::CSDOManagerModule()
{

}

CSDOManagerModule::~CSDOManagerModule()
{
	Destory();
}
bool CSDOManagerModule::Init()
{
	return TRUE;
}

bool CSDOManagerModule::InitNetwork()
{

	return CServiceModule::InitNetwork();
}
bool CSDOManagerModule::InitWorkerThread(int nWorkerCount)
{
	return CServiceModule::InitWorkerThread(nWorkerCount);
}
void CSDOManagerModule::CleanUpNetwork()
{

}
void CSDOManagerModule::AcceptNewSession(SOCKET hSocket)
{
	CServiceModule::AcceptNewSession(hSocket);
}


bool CSDOManagerModule::OnStartService()
{
	char path[256];
	GetCurrentDirectory(256,path);//获取当前目录
	char filepath[256];
	sprintf(filepath,"%s%s",path,"\\config\\scheme.ini");//获取scheme.ini文件路径
	
	int WorkThread=GetPrivateProfileInt("SERVER","WorkThread",10,filepath);//得到监听端口号
	Init();
	InitNetwork();
	InitWorkerThread(WorkThread);
//	if( AfxBeginThread( UpdateThread, this) == NULL )
	//	return false;

	return TRUE;
}
bool CSDOManagerModule::OnStopService()
{
	return TRUE;
}
bool CSDOManagerModule::OnInterrogateService()
{
	return TRUE;
}

bool CSDOManagerModule::ActiveNow()
{

	return true;
}

void CSDOManagerModule::Destory()
{
/*	CSession * pSession;
	
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
	
	CleanUpNetwork();*/
}