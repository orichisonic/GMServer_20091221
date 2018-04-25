// ModifyGuildName.h: interface for the CModifyGuildName class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODIFYGUILDNAME_H__D0C05CEE_F87B_4AFA_AA76_BC559E54F086__INCLUDED_)
#define AFX_MODIFYGUILDNAME_H__D0C05CEE_F87B_4AFA_AA76_BC559E54F086__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseStruct.h"
#include "OperPal.h"

class CModifyGuildName  
{
public:
	CModifyGuildName();
	virtual ~CModifyGuildName();
	vector <CGlobalStruct::TFLV> ModifyGuildNameMain(char *szGMServerName, char * szGMServerIP,char* szGuildName,char * szNewName);
	CLogFile logFile;
	COperPal operPal;
private:
	C_ObjNetClient g_ccNetObj;		// ��·����Ԫ��
	
public:
	static 	void LoginResult	(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam);	// ������
	static void ModifyGuildName(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam);	// �޸Ĺ������ƽ��
	
};

#endif // !defined(AFX_MODIFYGUILDNAME_H__D0C05CEE_F87B_4AFA_AA76_BC559E54F086__INCLUDED_)
