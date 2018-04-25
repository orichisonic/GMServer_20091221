// CG2ManagerModule.h: interface for the CCG2ManagerModule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CG2ManagerModule_H__17B695C9_9B98_42C0_BB31_F4B1FB469192__INCLUDED_)
#define AFX_CG2ManagerModule_H__17B695C9_9B98_42C0_BB31_F4B1FB469192__INCLUDED_

#if _MSC_VER > 1000
#pragma  once
#endif // _MSC_VER > 1000

#include "..\\NetIOCP\\ServiceModule.h"
#include "..\\NetIOCP\\Singleton.h"

class CCG2ManagerModule: public CServiceModule , public Singleton<CCG2ManagerModule>
{
public:
	CCG2ManagerModule();
	virtual ~CCG2ManagerModule();
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

#endif // !defined(AFX_CG2ManagerModule_H__17B695C9_9B98_42C0_BB31_F4B1FB469192__INCLUDED_)
