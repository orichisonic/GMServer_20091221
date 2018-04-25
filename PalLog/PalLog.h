// PalLog.h : main header file for the PALLOG DLL
//

#if !defined(AFX_PALLOG_H__56BCFDA0_93F6_4D3C_B2DC_A3B3713EF76F__INCLUDED_)
#define AFX_PALLOG_H__56BCFDA0_93F6_4D3C_B2DC_A3B3713EF76F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "PalLogInfo.h"
/////////////////////////////////////////////////////////////////////////////
// CPalLogApp
// See PalLog.cpp for the implementation of this class
//

class CPalLogApp : public CWinApp
{
public:
	CPalLogApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPalLogApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CPalLogApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PALLOG_H__56BCFDA0_93F6_4D3C_B2DC_A3B3713EF76F__INCLUDED_)
