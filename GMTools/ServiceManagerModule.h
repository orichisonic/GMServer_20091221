// ServiceManagerModule.h: interface for the CServiceManagerModule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVICEMANAGERMODULE_H__17B695C9_9B98_42C0_BB31_F4B1FB469192__INCLUDED_)
#define AFX_SERVICEMANAGERMODULE_H__17B695C9_9B98_42C0_BB31_F4B1FB469192__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\\NetIOCP\\ServiceModule.h"
#include "..\\NetIOCP\\Singleton.h"

class CServiceManagerModule: public CServiceModule , public Singleton<CServiceManagerModule>
{
public:
	CServiceManagerModule();
	virtual ~CServiceManagerModule();
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

#endif // !defined(AFX_SERVICEMANAGERMODULE_H__17B695C9_9B98_42C0_BB31_F4B1FB469192__INCLUDED_)
