// GTOPManagerModule.cpp: implementation of the CGTOPManagerModule class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GTOP.h"
#include "GTOPManagerModule.h"
#include "GTOPSession.h"

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
	CGTOPManagerModule * pModule = (CGTOPManagerModule*)lpParameter;
	while(true)
	{
		pModule->ActiveNow();
		Sleep(200);
	}
	return 0;
}

CGTOPManagerModule::CGTOPManagerModule()
{

}

CGTOPManagerModule::~CGTOPManagerModule()
{
	Destory();
}
bool CGTOPManagerModule::Init()
{
	return TRUE;
}

bool CGTOPManagerModule::InitNetwork()
{

	return CServiceModule::InitNetwork();
}
bool CGTOPManagerModule::InitWorkerThread(int nWorkerCount)
{
	return CServiceModule::InitWorkerThread(nWorkerCount);
}
void CGTOPManagerModule::CleanUpNetwork()
{

}
void CGTOPManagerModule::AcceptNewSession(SOCKET hSocket)
{
	CServiceModule::AcceptNewSession(hSocket);
}


bool CGTOPManagerModule::OnStartService()
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
bool CGTOPManagerModule::OnStopService()
{
	return TRUE;
}
bool CGTOPManagerModule::OnInterrogateService()
{
	return TRUE;
}

bool CGTOPManagerModule::ActiveNow()
{

	return true;
}

void CGTOPManagerModule::Destory()
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