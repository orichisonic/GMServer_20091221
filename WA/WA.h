// WA.h : main header file for the WA DLL
//

#if !defined(AFX_WA_H__84974246_580E_4E74_85FC_8EC51019EE35__INCLUDED_)
#define AFX_WA_H__84974246_580E_4E74_85FC_8EC51019EE35__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "WAInfo.h"
/////////////////////////////////////////////////////////////////////////////
// CWAApp
// See WA.cpp for the implementation of this class
//

class CWAApp : public CWinApp
{
public:
	CWAApp();
// 	vector <CGlobalStruct::TFLV> UserInfo(vector <CGlobalStruct::TFLV> RECTFLV);
// 	vector <CGlobalStruct::TFLV> KickPlayer(vector <CGlobalStruct::TFLV> RECTFLV);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWAApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CWAApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WA_H__84974246_580E_4E74_85FC_8EC51019EE35__INCLUDED_)