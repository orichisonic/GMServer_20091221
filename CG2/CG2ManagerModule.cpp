// CG2ManagerModule.cpp: implementation of the CCG2ManagerModule class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CG2.h"
#include "CG2ManagerModule.h"
#include "CG2Session.h"

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
	CCG2ManagerModule * pModule = (CCG2ManagerModule*)lpParameter;
	while(true)
	{
		pModule->ActiveNow();
		Sleep(200);
	}
	return 0;
}

CCG2ManagerModule::CCG2ManagerModule()
{

}

CCG2ManagerModule::~CCG2ManagerModule()
{
	Destory();
}
bool CCG2ManagerModule::Init()
{
	return TRUE;
}

bool CCG2ManagerModule::InitNetwork()
{

	return CServiceModule::InitNetwork();
}
bool CCG2ManagerModule::InitWorkerThread(int nWorkerCount)
{
	return CServiceModule::InitWorkerThread(nWorkerCount);
}
void CCG2ManagerModule::CleanUpNetwork()
{

}
void CCG2ManagerModule::AcceptNewSession(SOCKET hSocket)
{
	CServiceModule::AcceptNewSession(hSocket);
}


bool CCG2ManagerModule::OnStartService()
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
bool CCG2ManagerModule::OnStopService()
{
	return TRUE;
}
bool CCG2ManagerModule::OnInterrogateService()
{
	return TRUE;
}

bool CCG2ManagerModule::ActiveNow()
{

	return true;
}

void CCG2ManagerModule::Destory()
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