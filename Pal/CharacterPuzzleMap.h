// CharacterPuzzleMap.h: interface for the CCharacterPuzzleMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CharacterPuzzleMap_H__24A2EC29_AAAB_4FF2_AB9C_15ABFD68C297__INCLUDED_)
#define AFX_CharacterPuzzleMap_H__24A2EC29_AAAB_4FF2_AB9C_15ABFD68C297__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseStruct.h"
#include "OperPal.h"

class CCharacterPuzzleMap  
{
public:
	CCharacterPuzzleMap();
	virtual ~CCharacterPuzzleMap();
	vector <CGlobalStruct::TFLV> CharacterPuzzleMapMain(char *szGMServerName, char * szGMServerIP,char* szRoleName);
private:
	C_ObjNetClient g_ccNetObj;		// 网路连线元件
	
public:
	static 	void LoginResult	(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam);	// 登入结果
	static void CharacterPuzzleMap(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam);	// 取得角色好友列表结果
public:
	COperPal operPal;
	CLogFile logFile;
};

#endif // !defined(AFX_CharacterPuzzleMap_H__24A2EC29_AAAB_4FF2_AB9C_15ABFD68C297__INCLUDED_)
