// ModifyPuzzleMap.h: interface for the CModifyPuzzleMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ModifyPuzzleMap_H__24A2EC29_AAAB_4FF2_AB9C_15ABFD68C297__INCLUDED_)
#define AFX_ModifyPuzzleMap_H__24A2EC29_AAAB_4FF2_AB9C_15ABFD68C297__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseStruct.h"
#include "OperPal.h"

class CModifyPuzzleMap  
{
public:
	CModifyPuzzleMap();
	virtual ~CModifyPuzzleMap();
	vector<CGlobalStruct::TFLV> ModifyPuzzleMapMain(char * szGMServerName,char * szGMServerIP,char * szRoleName,char *strPuzzleMap,
		int iMapID1,int iMapID2,int iMapID3,int iMapID4,int iMapID5,int iMapID6,int iMapID7,int iMapID8,int iMapID9);
private:
	C_ObjNetClient g_ccNetObj;		// ��·����Ԫ��
	
public:
	static 	void LoginResult	(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam);	// ������
	static void ModifyPuzzleMap(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam);	// ȡ�ý�ɫ�����б���
public:
	COperPal operPal;
	CLogFile logFile;
};

#endif // !defined(AFX_ModifyPuzzleMap_H__24A2EC29_AAAB_4FF2_AB9C_15ABFD68C297__INCLUDED_)
