// XDJG.h : main header file for the XDJG DLL
//

#if !defined(AFX_XDJG_H__3B679F8B_3205_48D2_801E_9107D4D6D2EB__INCLUDED_)
#define AFX_XDJG_H__3B679F8B_3205_48D2_801E_9107D4D6D2EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "XDJGInfo.h"
/////////////////////////////////////////////////////////////////////////////
// CXDJGApp
// See XDJG.cpp for the implementation of this class
//

class CXDJGApp : public CWinApp
{
public:
	CXDJGApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXDJGApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXDJGApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XDJG_H__3B679F8B_3205_48D2_801E_9107D4D6D2EB__INCLUDED_)
