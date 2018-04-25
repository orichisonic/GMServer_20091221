// GT.h : main header file for the GT DLL
//

#if !defined(AFX_GT_H__CF217A56_1AE6_40D2_8E27_F417DA63B053__INCLUDED_)
#define AFX_GT_H__CF217A56_1AE6_40D2_8E27_F417DA63B053__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "GTInfo.h"
/////////////////////////////////////////////////////////////////////////////
// CGTApp
// See GT.cpp for the implementation of this class
//

class CGTApp : public CWinApp
{
public:
	vector <CGlobalStruct::TFLV> UserInfo(COperVector * precvVect);
	vector <CGlobalStruct::TFLV> UserProperty(COperVector * precvVect,CEnumCore::Message_Tag_ID m_message_tag);
	vector <CGlobalStruct::TFLV> LogInfo(COperVector * precvVect,CEnumCore::Message_Tag_ID m_message_tag);
	CGTApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGTApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CGTApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GT_H__CF217A56_1AE6_40D2_8E27_F417DA63B053__INCLUDED_)
