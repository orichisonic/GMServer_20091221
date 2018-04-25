// MoveCharacterToSafe.h: interface for the CMoveCharacterToSafe class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MoveCharacterToSafe_H__24A2EC29_AAAB_4FF2_AB9C_15ABFD68C297__INCLUDED_)
#define AFX_MoveCharacterToSafe_H__24A2EC29_AAAB_4FF2_AB9C_15ABFD68C297__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseStruct.h"
#include "OperPal.h"

class CMoveCharacterToSafe  
{
public:
	CMoveCharacterToSafe();
	virtual ~CMoveCharacterToSafe();
	vector<CGlobalStruct::TFLV> MoveCharacterToSafeMain(char * szGMServerName,char * szGMServerIP,char * szRoleName,int iZoneID);
private:
	C_ObjNetClient g_ccNetObj;		// ��·����Ԫ��
	
public:
	static 	void LoginResult	(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam);	// ������
	static void MoveCharacterToSafe(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam);	// ȡ�ý�ɫ�����б���
public:
	COperPal operPal;
	CLogFile logFile;
};

#endif // !defined(AFX_MoveCharacterToSafe_H__24A2EC29_AAAB_4FF2_AB9C_15ABFD68C297__INCLUDED_)
