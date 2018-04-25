// CharacterInfo.h: interface for the CCharacterInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARACTERINFO_H__CE92E01C_A7E3_4A02_8D82_BAAAE699BB5E__INCLUDED_)
#define AFX_CHARACTERINFO_H__CE92E01C_A7E3_4A02_8D82_BAAAE699BB5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseStruct.h"
#include "OperPal.h"

class CCharacterInfo  
{
public:
	CCharacterInfo();
	virtual ~CCharacterInfo();
	vector <CGlobalStruct::TFLV> CharacterInfoMain(char *szGMServerName, char * szGMServerIP,char* szRoleName);
private:
	C_ObjNetClient g_ccNetObj;		// ��·����Ԫ��
	
public:
	static 	void LoginResult	(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam);	// ������
	static void CharacterInfo	(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam);	// ȡ�ý�ɫ������Ѷ���
	
public:
	COperPal operPal;
	CLogFile logFile;
};

#endif // !defined(AFX_CHARACTERINFO_H__CE92E01C_A7E3_4A02_8D82_BAAAE699BB5E__INCLUDED_)





















