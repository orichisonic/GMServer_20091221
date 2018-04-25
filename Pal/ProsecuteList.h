#if !defined(AFX_PROSECUTELIST_H__CE92E01C_A7E3_4A02_8D82_BAAAE699BB5E__INCLUDED_)
#define AFX_PROSECUTELIST_H__CE92E01C_A7E3_4A02_8D82_BAAAE699BB5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseStruct.h"
#include "OperPal.h"

class CProsecuteList  
{
public:
	CProsecuteList();
	virtual ~CProsecuteList();
	vector <CGlobalStruct::TFLV> ProsecuteListMain(char *szGMServerName, char * szGMServerIP);//,char* szRoleName);
private:
	C_ObjNetClient g_ccNetObj;		// 网路连线元件
	
public:
	static 	void LoginResult	(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam);	// 登入结果
	static void ProsecuteList	(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam);	// 取得工会日志资讯结果
	
public:
	COperPal operPal;
	CLogFile logFile;
};

#endif // !defined(AFX_PROSECUTELIST_H__CE92E01C_A7E3_4A02_8D82_BAAAE699BB5E__INCLUDED_)