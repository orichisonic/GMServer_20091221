// GuildMemberList.h: interface for the CGuildMemberList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUILDMEMBERLIST_H__1A91CE5B_C4A1_4E68_840D_A318906E1CAD__INCLUDED_)
#define AFX_GUILDMEMBERLIST_H__1A91CE5B_C4A1_4E68_840D_A318906E1CAD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseStruct.h"
#include "OperPal.h"
class CGuildMemberList  
{
public:
	CGuildMemberList();
	virtual ~CGuildMemberList();
	vector <CGlobalStruct::TFLV>  GuildMemberListMain(char *szGMServerName, char * szGMServerIP,char* szGuildName);
	CLogFile logFile;
	COperPal operPal;
private:
	C_ObjNetClient g_ccNetObj;		// 网路连线元件
	
public:
	static 	void LoginResult	(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam);	// 登入结果
	static void GuildMemberList(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam);	// 取得公会成员列表结果
	
};

#endif // !defined(AFX_GUILDMEMBERLIST_H__1A91CE5B_C4A1_4E68_840D_A318906E1CAD__INCLUDED_)
