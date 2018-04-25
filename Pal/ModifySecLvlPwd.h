// ModifySecLvlPwd.h: interface for the CModifySecLvlPwd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODIFYSECLVLPWD_H__9D8098D7_EA8C_4987_BD00_35B22AA40713__INCLUDED_)
#define AFX_MODIFYSECLVLPWD_H__9D8098D7_EA8C_4987_BD00_35B22AA40713__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseStruct.h"
#include "OperPal.h"

class CModifySecLvlPwd  
{
public:
	bool RecallSecLvlPwd(char* szRoleAccount,char * szPasswd);
	bool PutSecLvlPwd(char *szAccount,char * szPasswd, char *lpGMServerName);
	void GetSecLvlPwd(char * szAccount,char * szPasswd,char* lpGMServerName);
	bool SaveSecLvlPwd(char * szRoleAccount,char * szNewPasswd,char * lpGMServerName,char * szMD5Passwd);
	void ReturnMessage(SOCKET m_socket,CEnumCore::ServiceKey m_ServiceKey,char *lpMessage);
	COperPal operPal;
	CLogFile logFile;
	CSqlHelper m_SqlHelper;
	//保存本地查询的结果
//	vector<CGlobalStruct::DataRecord> datarecord;
public:
	CModifySecLvlPwd();
	virtual ~CModifySecLvlPwd();
	
};

#endif // !defined(AFX_MODIFYSECLVLPWD_H__9D8098D7_EA8C_4987_BD00_35B22AA40713__INCLUDED_)
