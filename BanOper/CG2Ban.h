// CG2Ban.h: interface for the CCG2Ban class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CG2BAN_H__F9967CC0_0D10_4954_BF9D_6EB3443A5560__INCLUDED_)
#define AFX_CG2BAN_H__F9967CC0_0D10_4954_BF9D_6EB3443A5560__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "OperBan.h"
class CCG2Ban  
{
public:
	CCG2Ban();
	virtual ~CCG2Ban();
	int ClosePlayer(int GameID,char * ServerIP,char* ServerName,char *UserName,char *UserNick,char *EndTime, char * reason,char * memo);
	int OpenPlayer(int GameID,char * ServerIP,char* ServerName,char *UserName,char *UserNick,char * reason,char * memo);
	int KickPlayer(int GameID,char * ServerIP,char* ServerName,char *UserName,char *UserNick,char * memo);
	//≤È—Ø–≈œ¢
	int QueryAccount(int GameID,char * ServerIP,char * ServerName,char *UserName,char * UserNick);
private:
	COperBan operBan;
	char operszSql[2048];
	char operRemoteSql[2048];
	COperVector DBVect;
};

#endif // !defined(AFX_CG2BAN_H__F9967CC0_0D10_4954_BF9D_6EB3443A5560__INCLUDED_)
