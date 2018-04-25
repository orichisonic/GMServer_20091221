// GameIOCP.h : main header file for the GAMEIOCP DLL
//

#if !defined(AFX_GAMEIOCP_H__CECF220A_3BD0_403E_A200_787034CA1497__INCLUDED_)
#define AFX_GAMEIOCP_H__CECF220A_3BD0_403E_A200_787034CA1497__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CGameIOCPApp
// See GameIOCP.cpp for the implementation of this class
//

class CGameIOCPApp : public CWinApp
{
public:
	CGameIOCPApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGameIOCPApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CGameIOCPApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GAMEIOCP_H__CECF220A_3BD0_403E_A200_787034CA1497__INCLUDED_)
