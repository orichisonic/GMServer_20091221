// ModifyAccountPw.h: interface for the CModifyAccountPw class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODIFYACCOUNTPW_H__08D75B16_0981_474F_94A7_5EB2CBD79ED2__INCLUDED_)
#define AFX_MODIFYACCOUNTPW_H__08D75B16_0981_474F_94A7_5EB2CBD79ED2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseStruct.h"
#include "OperPal.h"
class CModifyAccountPw  
{
public:
	CModifyAccountPw();
	virtual ~CModifyAccountPw();
public:
	bool RecallAccountPw(char* szRoleAccount,char * szPasswd, char *lpGMServerip);
	bool PutAccountPw(char *szAccount,char * szPasswd, char *lpGMServerip);
	void GetAccountPw(char * szAccount,char * szPasswd,char* lpGMServerip);
	int SaveAccountPw(char * szRoleAccount,char * szNewPasswd,char * lpGMServerip,char * szMD5Passwd);
	CLogFile logFile;
	COperPal operPal;
	CSqlHelper m_SqlHelper;
	//保存本地查询的结果
	//vector<CGlobalStruct::DataRecord> datarecord;
};

#endif // !defined(AFX_MODIFYACCOUNTPW_H__08D75B16_0981_474F_94A7_5EB2CBD79ED2__INCLUDED_)
