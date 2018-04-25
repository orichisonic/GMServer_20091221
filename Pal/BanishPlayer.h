// BanishPlayer.h: interface for the CBanishPlayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BANISHPLAYER_H__5BD634CB_B880_487C_8CC5_6E8541EFF5EC__INCLUDED_)
#define AFX_BANISHPLAYER_H__5BD634CB_B880_487C_8CC5_6E8541EFF5EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseStruct.h"
#include "OperPal.h"
class CBanishPlayer  
{
public:
	CBanishPlayer();
	virtual ~CBanishPlayer();
	vector <CGlobalStruct::TFLV> BanishPlayerMain(char * szGMServerName,char * szGMServerIP,char * szRoleName);
private:
	C_ObjNetClient g_ccNetObj;		// ��·����Ԫ��
	
public:
	static 	void LoginResult	(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam);	// ������
	static void BanishPlayer	(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam);	// ��ĳ������߽��
	
public:
	COperPal operPal;
	CLogFile logFile;
	
};

#endif // !defined(AFX_BANISHPLAYER_H__5BD634CB_B880_487C_8CC5_6E8541EFF5EC__INCLUDED_)
