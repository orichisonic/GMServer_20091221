// CharacterQuest.h: interface for the CCharacterQuest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARACTERQUEST_H__782DF867_08A7_483F_8D5A_829F34AC0230__INCLUDED_)
#define AFX_CHARACTERQUEST_H__782DF867_08A7_483F_8D5A_829F34AC0230__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseStruct.h"
#include "OperPal.h"
class CCharacterQuest  
{
public:
	CCharacterQuest();
	virtual ~CCharacterQuest();
	vector <CGlobalStruct::TFLV> CharacterQuestMain(char * szGMServerName, char * szGMServerIP,char * szRoleName,int iQuestType);
	
private:
	C_ObjNetClient g_ccNetObj;		// ��·����Ԫ��
	
public:
	static 	void LoginResult	(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam);	// ������
	static void CharacterQuest	(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam);	// ȡ�ý�ɫ������Ѷ���
public:
	COperPal operPal;
	CLogFile logFile;
};

#endif // !defined(AFX_CHARACTERQUEST_H__782DF867_08A7_483F_8D5A_829F34AC0230__INCLUDED_)
