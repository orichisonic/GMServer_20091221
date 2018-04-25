// DelCharacterItem.h: interface for the CDelCharacterItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DELCHARACTERITEM_H__8D259DBA_1973_4A8A_80CC_A2C08DA57E2F__INCLUDED_)
#define AFX_DELCHARACTERITEM_H__8D259DBA_1973_4A8A_80CC_A2C08DA57E2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseStruct.h"
#include "OperPal.h"
class CDelCharacterItem  
{
public:
	CDelCharacterItem();
	virtual ~CDelCharacterItem();
	vector <CGlobalStruct::TFLV> DelCharacterItemMain(char *szGMServerName, char * szGMServerIP,char* szRoleName,int iType, int iPosition);
private:
	C_ObjNetClient g_ccNetObj;		// ��·����Ԫ��
	
public:
	static void LoginResult	(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam);	// ������ 
	static void DelCharacterItem(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam);	// �޸Ĺ���ȼ����
public:
	CLogFile logFile;
	COperPal operPal;
};

#endif // !defined(AFX_DELCHARACTERITEM_H__8D259DBA_1973_4A8A_80CC_A2C08DA57E2F__INCLUDED_)
