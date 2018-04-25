// IOCPThread.h: interface for the CIOCPThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IOCPTHREAD_H__DE1B40BB_D23F_48DA_83D4_91F4949F743B__INCLUDED_)
#define AFX_IOCPTHREAD_H__DE1B40BB_D23F_48DA_83D4_91F4949F743B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Session.h"
class CServiceModule;


class __declspec(dllexport) CIOCPThread : public CWinThread  
{
	DECLARE_DYNCREATE(CIOCPThread)
protected:
	CIOCPThread();
	virtual ~CIOCPThread();

	DECLARE_MESSAGE_MAP()

public:
	bool SetModule(CServiceModule* pModule, int nTh);
	
	CServiceModule* m_pModule;

	int			m_nTh;
	int			m_nPreDay;
	int			m_nTemp1;

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
	
	void ErrorInSession( CSession* pSession, LPOVERLAPPED2 lpov );

};

#endif // !defined(AFX_IOCPTHREAD_H__948090CD_DEC7_4197_84E3_F872865448F5__INCLUDED_)
