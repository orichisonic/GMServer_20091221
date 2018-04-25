// GTBan.h: interface for the CGTBan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GTBAN_H__58124BB0_0B6E_4200_AC2F_E0302D90CB88__INCLUDED_)
#define AFX_GTBAN_H__58124BB0_0B6E_4200_AC2F_E0302D90CB88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "OperBan.h"
#include "..\\GTOP\\GTOP.h"
#pragma comment(lib,"..\\GTOP\\debug\\GTOP.lib")
class CGTBan  
{
public:
	CGTBan();
	virtual ~CGTBan();
	int ClosePlayer(int GameID,char * GT_ServerIP,char * GT_ServerName,char * GT_Account);
	int OpenPlayer(int GameID,char * GT_ServerIP,char *GT_ServerName,char * GT_Account);
	int KickPlayer(int GameID,char * GT_ServerIP,char * GT_ServerName,char *GT_Account);
	//≤È—Ø–≈œ¢
	int QueryAccount(int GameID,char * GT_ServerIP,char * GT_ServerName,char *GT_Account, char * GT_UserNick);
	
private:
	COperBan operBan;
	char operszSql[2048];
	char operRemoteSql[2048];
	COperVector DBVect;
};

#endif // !defined(AFX_GTBAN_H__58124BB0_0B6E_4200_AC2F_E0302D90CB88__INCLUDED_)
