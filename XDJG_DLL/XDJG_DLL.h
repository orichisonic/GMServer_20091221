// XDJG_DLL.h : main header file for the XDJG_DLL DLL
//

#if !defined(AFX_XDJG_DLL_H__0BB8B6E0_D16B_4478_A4DC_F561CAB8D676__INCLUDED_)
#define AFX_XDJG_DLL_H__0BB8B6E0_D16B_4478_A4DC_F561CAB8D676__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CXDJG_DLLApp
// See XDJG_DLL.cpp for the implementation of this class
//

class CXDJG_DLLApp : public CWinApp
{
public:
	CXDJG_DLLApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXDJG_DLLApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXDJG_DLLApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XDJG_DLL_H__0BB8B6E0_D16B_4478_A4DC_F561CAB8D676__INCLUDED_)
