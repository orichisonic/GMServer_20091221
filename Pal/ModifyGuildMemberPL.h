// ModifyGuildMemberPL.h: interface for the CModifyGuildMemberPL class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODIFYGUILDMEMBERPL_H__907D9930_F581_4E23_B667_A7AC309814E9__INCLUDED_)
#define AFX_MODIFYGUILDMEMBERPL_H__907D9930_F581_4E23_B667_A7AC309814E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseStruct.h"
#include "OperPal.h"

class CModifyGuildMemberPL  
{
public:
	CModifyGuildMemberPL();
	virtual ~CModifyGuildMemberPL();
	vector <CGlobalStruct::TFLV> ModifyGuildMemberPLMain(char *szGMServerName, char * szGMServerIP,char* szGuildName,char * szRoleName, int iLevel);
	COperPal operPal;
	CLogFile logFile;
private:
	C_ObjNetClient g_ccNetObj;		// ��·����Ԫ��
	
public:
	static 	void LoginResult	(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam);	// ������
	static void ModifyGuildMemberPL	(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam);	// �޸Ĺ����Աְ�׽��
	
};

#endif // !defined(AFX_MODIFYGUILDMEMBERPL_H__907D9930_F581_4E23_B667_A7AC309814E9__INCLUDED_)
