#ifndef _SENDCUSTOMITEMMAIL_H_
#define _SENDCUSTOMITEMMAIL_H_

#include "BaseStruct.h"
#include "OperPal.h"

class CSendCustomItemMail  
{
public:
	CSendCustomItemMail();
	virtual ~CSendCustomItemMail();
	vector <CGlobalStruct::TFLV> SendCustomItemMailMain(char * szGMServerName,char * szGMServerIP,char * szRoleName,char * szSubject,
						char * szContent,int iMoney, int itemType, int iCustomItemID, char *szCustomItemIDName,bool bBindSetup, char *szInherit,char *szReason);
public:
	COperPal operPal;
	CLogFile logFile;
private:
	C_ObjNetClient g_ccNetObj;		// 网路连线元件
	
public:
	static 	void LoginResult	(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam);	// 登入结果
	
	static void SendCustomItemMail(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam);	// 寄送客制物品邮件结果
};

#endif