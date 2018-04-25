// ModifyCharacterMoney.h: interface for the CModifyCharacterMoney class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODIFYCHARACTERMONEY_H__570A7E86_0F82_4A44_A855_9E59178EFF80__INCLUDED_)
#define AFX_MODIFYCHARACTERMONEY_H__570A7E86_0F82_4A44_A855_9E59178EFF80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseStruct.h"
#include "OperPal.h"
class CModifyCharacterMoney  
{
public:
	CModifyCharacterMoney();
	virtual ~CModifyCharacterMoney();
	vector <CGlobalStruct::TFLV> ModifyCharacterMoneyMain(char *szGMServerName, char * szGMServerIP,char* szRoleName,int iType,int iCount);
	CLogFile logFile;
	COperPal operPal;
private:
	C_ObjNetClient g_ccNetObj;		// ��·����Ԫ��
	
public:
	static void LoginResult	(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam);	// ������ 
	static void ModifyCharacterMoney(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam);	// �޸Ĺ���ȼ����
	
};

#endif // !defined(AFX_MODIFYCHARACTERMONEY_H__570A7E86_0F82_4A44_A855_9E59178EFF80__INCLUDED_)
