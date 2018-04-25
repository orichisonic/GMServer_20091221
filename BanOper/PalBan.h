// PalBan.h: interface for the CPalBan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PALBAN_H__CEF9021C_3D3F_406C_8B19_1B5AEAA6368C__INCLUDED_)
#define AFX_PALBAN_H__CEF9021C_3D3F_406C_8B19_1B5AEAA6368C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OperBan.h"


class CPalBan  
{
public:
	CPalBan();
	virtual ~CPalBan();
	int ClosePlayer(int GameID,char * Pal_ServerIP,char * Pal_ServerName,char * Pal_Account);
	int OpenPlayer(int GameID,char * Pal_ServerIP,char * Pal_ServerName,char * Pal_Account);
	int KickPlayer(int GameID,char * Pal_ServerIP,char * Pal_ServerName,char *Pal_userNick);
	//≤È—Ø–≈œ¢
	int FindAccount(int GameID,char * Pal_ServerIP,char * Pal_ServerName,char *Pal_Account, char * Pal_RoleName);

private:
	COperBan operBan;
	char operszSql[2048];
	char operRemoteSql[2048];
	COperVector DBVect;

};

#endif // !defined(AFX_PALBAN_H__CEF9021C_3D3F_406C_8B19_1B5AEAA6368C__INCLUDED_)
