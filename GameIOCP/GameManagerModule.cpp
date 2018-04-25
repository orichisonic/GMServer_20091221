// GameManagerModule.cpp: implementation of the CGameManagerModule class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameIOCP.h"
#include "GameManagerModule.h"
#include "GameSession.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CGameManagerModule::CGameManagerModule()
{

}

CGameManagerModule::~CGameManagerModule()
{
	Destory();
}
bool CGameManagerModule::Init()
{
	return TRUE;
}

bool CGameManagerModule::InitNetwork()
{

	return CServiceModule::InitNetwork();
}
bool CGameManagerModule::InitWorkerThread(int nWorkerCount)
{
	return CServiceModule::InitWorkerThread(nWorkerCount);
}
void CGameManagerModule::CleanUpNetwork()
{

}
void CGameManagerModule::AcceptNewSession(SOCKET hSocket)
{
	CServiceModule::AcceptNewSession(hSocket);
}


bool CGameManagerModule::OnStartService()
{
	return TRUE;
}
bool CGameManagerModule::OnStopService()
{
	return TRUE;
}
bool CGameManagerModule::OnInterrogateService()
{
	return TRUE;
}

bool CGameManagerModule::ActiveNow()
{

	return true;
}

void CGameManagerModule::Destory()
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