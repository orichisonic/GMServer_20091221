// CF.h : main header file for the CF DLL
//

#if !defined(AFX_CF_H__B7265863_96EC_4499_A2CD_22A8948220C8__INCLUDED_)
#define AFX_CF_H__B7265863_96EC_4499_A2CD_22A8948220C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "CFInfo.h"
/////////////////////////////////////////////////////////////////////////////
// CCFApp
// See CF.cpp for the implementation of this class
//

class CCFApp : public CWinApp
{
public:
	CCFApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCFApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCFApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CF_H__B7265863_96EC_4499_A2CD_22A8948220C8__INCLUDED_)
