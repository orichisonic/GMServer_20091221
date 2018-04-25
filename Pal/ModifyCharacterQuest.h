// ModifyCharacterQuest.h: interface for the CModifyCharacterQuest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODIFYCHARACTERQUEST_H__5C74A6C8_35FD_4C9E_A23D_044A94AD1F90__INCLUDED_)
#define AFX_MODIFYCHARACTERQUEST_H__5C74A6C8_35FD_4C9E_A23D_044A94AD1F90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseStruct.h"
#include "OperPal.h"
class CModifyCharacterQuest  
{
public:
	CModifyCharacterQuest();
	virtual ~CModifyCharacterQuest();
	vector <CGlobalStruct::TFLV> ModifyCharacterQuestMain(int userByID,char *szGMServerName, char * szGMServerIP,char* szRoleName,int iQuestID,int iQuestStatus);
	CLogFile logFile;
	COperPal operPal;
private:
	C_ObjNetClient g_ccNetObj;		// ��·����Ԫ��
	
public:
	static 	void LoginResult	(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam);	// ������
	static void ModifyCharacterQuest(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam);	// �޸Ľ�ɫ������
};

#endif // !defined(AFX_MODIFYCHARACTERQUEST_H__5C74A6C8_35FD_4C9E_A23D_044A94AD1F90__INCLUDED_)
