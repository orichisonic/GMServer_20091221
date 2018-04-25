// DismissGuild.h: interface for the CDismissGuild class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISMISSGUILD_H__B51BF8C2_06D5_4B42_9D78_390151F3CF9A__INCLUDED_)
#define AFX_DISMISSGUILD_H__B51BF8C2_06D5_4B42_9D78_390151F3CF9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseStruct.h"
#include "OperPal.h"

class CDismissGuild  
{
public:
	CDismissGuild();
	virtual ~CDismissGuild();
	vector <CGlobalStruct::TFLV> DismissGuildMain(char *szGMServerName, char * szGMServerIP,char* szGuildName);
public:
	CLogFile logFile;
	COperPal operPal;
private:
	C_ObjNetClient g_ccNetObj;		// 网路连线元件
	
public:
	static 	void LoginResult	(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam);	// 登入结果
	static void DismissGuild	(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam);	// 解散公会结果
	
};

#endif // !defined(AFX_DISMISSGUILD_H__B51BF8C2_06D5_4B42_9D78_390151F3CF9A__INCLUDED_)
