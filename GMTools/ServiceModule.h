// ServiceModule.h: interface for the ServiceModule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVICEMODULE_H__2C8B7613_9E32_4E57_BA24_CD1939B19A9B__INCLUDED_)
#define AFX_SERVICEMODULE_H__2C8B7613_9E32_4E57_BA24_CD1939B19A9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define	MAX_WORKER_THREADS 64 // 4 * CPU + 1

extern __declspec(dllexport) HANDLE g_hInterrogate;
extern __declspec(dllexport) HANDLE g_hEndService;

class CIOCPThread;
class CSession;

class __declspec(dllexport) CServiceModule  
{
public:
	CServiceModule(void);
	virtual ~CServiceModule();
	virtual bool Init() = 0;
	virtual bool InitNetwork();	
	virtual bool InitWorkerThread(int nWorkerCount);
	virtual void CleanUpNetwork();
	virtual bool RemoveSession(CSession* pSession, int nError);
	virtual void AcceptNewSession(SOCKET hSocket);
	
	HANDLE	GetIOCP() {return m_hIOCP;};
	
	
	SOCKET  GetsockListen(){return m_sockListen;}
	
	friend UINT AcceptThread(LPVOID lpParameter);
	
	virtual bool OnStartService() = 0;
	virtual bool OnStopService() = 0;
	virtual bool OnInterrogateService() = 0;
public:
	int		m_nPort;
	CString m_strIPAddr;	
	
protected:
	bool	m_bOnlyLocalServer;
	
	HANDLE m_hListeningThreadKilled;
	HANDLE m_hKillListeningThread;
	HANDLE m_hListenEvent;

	HANDLE m_hCloseEvent;
	
	CString m_strHostName;
	
	SOCKET	m_sockListen;
	
	CCriticalSection m_csSessionList;
	DWORD	m_nIOCPThreadIDs[MAX_WORKER_THREADS];
	
	HANDLE	m_hIOCP;
	
	CList <CSession*, CSession*> m_listActive;
	CList <CSession*, CSession*> m_listInActive;
	
};

#endif // !defined(AFX_SERVICEMODULE_H__2C8B7613_9E32_4E57_BA24_CD1939B19A9B__INCLUDED_)
