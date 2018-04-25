// JW2.h : main header file for the JW2 DLL
//

#if !defined(AFX_JW2_H__E3F5A97E_3595_4C76_A469_8945604307B6__INCLUDED_)
#define AFX_JW2_H__E3F5A97E_3595_4C76_A469_8945604307B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CJW2App
// See JW2.cpp for the implementation of this class
//
#include "JW2Info.h"
#include "OperatorJW2.h"

class CJW2App : public CWinApp
{
public:
	CJW2App();
	vector <CGlobalStruct::TFLV> UserDefault(COperVector * precvVect,CEnumCore::Message_Tag_ID m_message_tag);
	vector <CGlobalStruct::TFLV> UserInfo(COperVector * precvVect);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJW2App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CJW2App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JW2_H__E3F5A97E_3595_4C76_A469_8945604307B6__INCLUDED_)
