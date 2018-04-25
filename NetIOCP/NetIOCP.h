// NetIOCP.h : main header file for the NETIOCP DLL
//

#if !defined(AFX_NETIOCP_H__EB2DF9AB_9297_4E12_9548_812E8894FA26__INCLUDED_)
#define AFX_NETIOCP_H__EB2DF9AB_9297_4E12_9548_812E8894FA26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CNetIOCPApp
// See NetIOCP.cpp for the implementation of this class
//

class CNetIOCPApp : public CWinApp
{
public:
	CNetIOCPApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetIOCPApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CNetIOCPApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETIOCP_H__EB2DF9AB_9297_4E12_9548_812E8894FA26__INCLUDED_)
