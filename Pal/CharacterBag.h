// CharacterBag.h: interface for the CCharacterBag class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARACTERBAG_H__14201D2E_1122_44C4_8E8B_902C8D0B3650__INCLUDED_)
#define AFX_CHARACTERBAG_H__14201D2E_1122_44C4_8E8B_902C8D0B3650__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseStruct.h"
#include "OperPal.h"

class CCharacterBag  
{
public:
	CCharacterBag();
	virtual ~CCharacterBag();
	vector <CGlobalStruct::TFLV> CharacterBagMain(char *szGMServerName, char * szGMServerIP,char* szRoleName, int iBagType);
private:
	C_ObjNetClient g_ccNetObj;		// ��·����Ԫ��
	
public:
	static 	void LoginResult	(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam);	// ������
	static void CharacterBag (int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam);	// ȡ�ý�ɫ������Ѷ���
public:
	COperPal operPal;
	CLogFile logFile;
};

#endif // !defined(AFX_CHARACTERBAG_H__14201D2E_1122_44C4_8E8B_902C8D0B3650__INCLUDED_)
