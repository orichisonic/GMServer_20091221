// FJBan.h: interface for the CFJBan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FJBAN_H__368CBDE5_869B_495A_8113_9C26597734A1__INCLUDED_)
#define AFX_FJBAN_H__368CBDE5_869B_495A_8113_9C26597734A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "OperBan.h"
class CFJBan  
{
public:
	CFJBan();
	virtual ~CFJBan();
	int ClosePlayer(int GameID,char * FJ_ServerIP,char * FJ_ServerName,char *UserName);
	int OpenPlayer(int GameID,char * FJ_ServerIP,char * FJ_ServerName,char *UserName);
	int KickPlayer(int GameID,char * FJ_ServerIP,char * FJ_ServerName,char *UserName);
	//≤È—Ø–≈œ¢
	int QueryAccount(int GameID,char * FJ_ServerIP,char * FJ_ServerName,char *UserName,char * UserNick);
	
	
private:
	COperBan operBan;
	char operszSql[2048];
	char operRemoteSql[2048];
	COperVector DBVect;
};

#endif // !defined(AFX_FJBAN_H__368CBDE5_869B_495A_8113_9C26597734A1__INCLUDED_)
