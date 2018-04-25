// Ban.h : main header file for the BAN DLL
//

#if !defined(AFX_BAN_H__A917FE2A_7E6B_4813_BD46_96D28553C046__INCLUDED_)
#define AFX_BAN_H__A917FE2A_7E6B_4813_BD46_96D28553C046__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "BanInfo.h"
/////////////////////////////////////////////////////////////////////////////
// CBanApp
// See Ban.cpp for the implementation of this class
//

class CBanApp : public CWinApp
{
public:
	CBanApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBanApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CBanApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BAN_H__A917FE2A_7E6B_4813_BD46_96D28553C046__INCLUDED_)
