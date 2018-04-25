// GTOP.h : main header file for the GTOP DLL
//

#if !defined(AFX_GTOP_H__F35B2C64_20B3_4470_8863_0407C28F1A54__INCLUDED_)
#define AFX_GTOP_H__F35B2C64_20B3_4470_8863_0407C28F1A54__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CGTOPApp
// See GTOP.cpp for the implementation of this class
//
extern "C"  bool __declspec(dllexport) GT_KICK_USER(char * ServerIP,char * Account);
extern "C"  bool __declspec(dllexport) GT_BAN_USER(char * ServerIP,char * Account,int status);
class CGTOPApp : public CWinApp
{
public:
	CGTOPApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGTOPApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CGTOPApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GTOP_H__F35B2C64_20B3_4470_8863_0407C28F1A54__INCLUDED_)
