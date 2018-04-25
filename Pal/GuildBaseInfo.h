// GuildBaseInfo.h: interface for the CGuildBaseInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUILDBASEINFO_H__719450E2_C022_4936_81C9_5C328A383F70__INCLUDED_)
#define AFX_GUILDBASEINFO_H__719450E2_C022_4936_81C9_5C328A383F70__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseStruct.h"
#include "OperPal.h"

class CGuildBaseInfo  
{
public:
	CGuildBaseInfo();
	virtual ~CGuildBaseInfo();
	vector <CGlobalStruct::TFLV>  GuildBaseInfoMain(char *szGMServerName, char * szGMServerIP,char* szGuildName);

public:
	COperPal operPal;
	CLogFile logFile;
private:
	C_ObjNetClient g_ccNetObj;		// 网路连线元件
	
public:
	static 	void LoginResult	(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam);	// 登入结果
	static void GuildBaseInfo	(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam);	// 取得公会基本资讯结果
	
	
};

#endif // !defined(AFX_GUILDBASEINFO_H__719450E2_C022_4936_81C9_5C328A383F70__INCLUDED_)
