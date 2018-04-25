// RayBan.h: interface for the CRayBan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RAYBAN_H__ED3AE17B_4933_4E7D_92B7_B96DC9D166F3__INCLUDED_)
#define AFX_RAYBAN_H__ED3AE17B_4933_4E7D_92B7_B96DC9D166F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "OperBan.h"
class CRayBan  
{
public:
	CRayBan();
	virtual ~CRayBan();
	int ClosePlayer(int GameID,char * RAY_ServerIP,char * RAY_ServerName,char * UserName);
	int OpenPlayer(int GameID,char * RAY_ServerIP,char * RAY_ServerName,char * UserName);
	int KickPlayer(int GameID,char * RAY_ServerIP,char * RAY_ServerName,char * UserName);
	//≤È—Ø–≈œ¢
	int QueryAccount(int GameID,char * RAY_ServerIP,char * RAY_ServerName,char *UserName,char * UserNick);
private:
	COperBan operBan;
	char operszSql[2048];
	char operRemoteSql[2048];
	COperVector DBVect;
};

#endif // !defined(AFX_RAYBAN_H__ED3AE17B_4933_4E7D_92B7_B96DC9D166F3__INCLUDED_)
