// ModifyGuildNotice.h: interface for the CModifyGuildNotice class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODIFYGUILDNOTICE_H__00690271_51CD_476E_B6F1_A525AE821B1C__INCLUDED_)
#define AFX_MODIFYGUILDNOTICE_H__00690271_51CD_476E_B6F1_A525AE821B1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseStruct.h"
#include "OperPal.h"

class CModifyGuildNotice  
{
public:
	CModifyGuildNotice();
	virtual ~CModifyGuildNotice();
	vector <CGlobalStruct::TFLV> ModifyGuildNoticeMain(char *szGMServerName, char * szGMServerIP,char* szGuildName,char * szNotice);
	CLogFile logFile;
	COperPal operPal;
private:
	C_ObjNetClient g_ccNetObj;		// 网路连线元件
	
public:
	static 	void LoginResult	(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam);	// 登入结果 
	static void ModifyGuildNotice(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam);// 修改公会公告结果
	
};

#endif // !defined(AFX_MODIFYGUILDNOTICE_H__00690271_51CD_476E_B6F1_A525AE821B1C__INCLUDED_)
