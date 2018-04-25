// JW2Ban.h: interface for the CJW2Ban class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JW2BAN_H__F84314E6_F455_48B8_9421_8D0AC91B749B__INCLUDED_)
#define AFX_JW2BAN_H__F84314E6_F455_48B8_9421_8D0AC91B749B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OperBan.h"
#include "AU2_Dll.h"
#pragma comment(lib,"AU2_Dll.lib")


class CJW2Ban  
{
public:
	CJW2Ban();
	virtual ~CJW2Ban();
	int ClosePlayer(int GameID,char * JW2_ServerIP,char * JW2_ServerName,char *JW2_UserSN);
	int OpenPlayer(int GameID,char * JW2_ServerIP,char * JW2_ServerName,char *JW2_UserSN);
	int KickPlayer(int GameID,char * JW2_ServerIP,char * JW2_ServerName,char *JW2_UserName, int userByID);
	//≤È—Ø–≈œ¢
	int QueryAccount(int GameID,char * JW2_ServerIP,char * JW2_ServerName,char *UserName,char * UserNick);
	int FindUserSN(int GameID,char * JW2_ServerIP,char * JW2_ServerName,char *UserName);
private:
	COperBan operBan;
	char operszSql[2048];
	char operRemoteSql[2048];
	COperVector DBVect;

};

#endif // !defined(AFX_JW2BAN_H__F84314E6_F455_48B8_9421_8D0AC91B749B__INCLUDED_)
