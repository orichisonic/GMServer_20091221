// BanOper.h : main header file for the BANOPER DLL
//

#if !defined(AFX_BANOPER_H__8618D407_A63B_445B_A290_E3FBCF2FCEA4__INCLUDED_)
#define AFX_BANOPER_H__8618D407_A63B_445B_A290_E3FBCF2FCEA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include <fstream> 
#include <iostream>
#include <iomanip>
using namespace std;

extern "C"  int __declspec(dllexport) CloseOper(int type,int userByID,int GameID,char* ServerIP,char* ServerName,char * UserID, char * UserName, char * NickID, char * UserNick, char * beginTime, char * endTime, char * reason,char *memo);
extern "C"  int __declspec(dllexport) OpenOper(int type,int userByID,int GameID,char* ServerIP,char* ServerName,char * UserID, char * UserName, char * NickID, char * UserNick,char * beginTime, char * endTime,char * reason,  char *memo);
extern "C"  int __declspec(dllexport) QueryAccount(int GameID,char *ServerIP,char *ServerName,char *UserName,char *UserNick);
extern "C"  int __declspec(dllexport) RemoteTest(int GameID,char* ServerIP,char * ServerName,char * UserID, char * UserName, char* NickID, char * UserNick);
void WriteLog(char* gameName,char * lpText);
/////////////////////////////////////////////////////////////////////////////
// CBanOperApp
// See BanOper.cpp for the implementation of this class
//

class CBanOperApp : public CWinApp
{
public:
	CBanOperApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBanOperApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CBanOperApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BANOPER_H__8618D407_A63B_445B_A290_E3FBCF2FCEA4__INCLUDED_)
