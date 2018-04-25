// ModifySafeLock.h: interface for the CModifySafeLock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODIFYSAFELOCK_H__7F15E47E_C2B1_494F_A701_34708104501E__INCLUDED_)
#define AFX_MODIFYSAFELOCK_H__7F15E47E_C2B1_494F_A701_34708104501E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseStruct.h"
#include "OperPal.h"

class CModifySafeLock  
{
public:	
	bool LockSafeLock(char *szAccount, char *lpGMServerName);
	bool UnLockSafeLock(char* szRoleAccount, char *lpGMServerName);
	bool GetSafeLock(char * szAccount, char* lpGMServerName);
	CModifySafeLock();
	virtual ~CModifySafeLock();
	
public:
	COperPal operPal;	
	CLogFile logFile;
	CSqlHelper m_SqlHelper;
	//保存本地查询的结果
//	vector<CGlobalStruct::DataRecord> datarecord;
};

#endif // !defined(AFX_MODIFYSAFELOCK_H__7F15E47E_C2B1_494F_A701_34708104501E__INCLUDED_)
