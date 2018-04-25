// ModifyGuildLv.h: interface for the CModifyGuildLv class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODIFYGUILDLV_H__496AD19B_6121_4F09_98D0_060B2825CFCA__INCLUDED_)
#define AFX_MODIFYGUILDLV_H__496AD19B_6121_4F09_98D0_060B2825CFCA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseStruct.h"
#include "OperPal.h"

class CModifyGuildLv  
{
public:
	CModifyGuildLv();
	virtual ~CModifyGuildLv();
	vector <CGlobalStruct::TFLV> ModifyGuildLvMain(char *szGMServerName, char * szGMServerIP,char* szGuildName,int iLevel);
public:
	CLogFile logFile;
	COperPal operPal;
private:
	C_ObjNetClient g_ccNetObj;		// ��·����Ԫ��
	
public:
	static void LoginResult	(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam);	// ������ 
	static void ModifyGuildLv(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam);	// �޸Ĺ���ȼ����
	
	
};

#endif // !defined(AFX_MODIFYGUILDLV_H__496AD19B_6121_4F09_98D0_060B2825CFCA__INCLUDED_)
