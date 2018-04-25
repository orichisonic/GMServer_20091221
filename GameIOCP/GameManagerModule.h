// GameManagerModule.h: interface for the CGameManagerModule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GameManagerModule_H__17B695C9_9B98_42C0_BB31_F4B1FB469192__INCLUDED_)
#define AFX_GameManagerModule_H__17B695C9_9B98_42C0_BB31_F4B1FB469192__INCLUDED_

#if _MSC_VER > 1000
#pragma  once
#endif // _MSC_VER > 1000

#include "..\\NetIOCP\\ServiceModule.h"
#include "..\\NetIOCP\\Singleton.h"

class CGameManagerModule: public CServiceModule , public Singleton<CGameManagerModule>
{
public:
	CGameManagerModule();
	virtual ~CGameManagerModule();
public:
	bool Init();
	bool InitNetwork();	
	bool InitWorkerThread(int nWorkerCount);
	void CleanUpNetwork();
	void AcceptNewSession(SOCKET hSocket);
	
	
	bool OnStartService();
	bool OnStopService();
	bool OnInterrogateService();

	bool ActiveNow();
	void Destory();
public:
	CString m_strIPAddr;

};

#endif // !defined(AFX_GameManagerModule_H__17B695C9_9B98_42C0_BB31_F4B1FB469192__INCLUDED_)
