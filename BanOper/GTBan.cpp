// GTBan.cpp: implementation of the CGTBan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GTBan.h"
#include "..\\GTOP\\GTOP.h"
#pragma comment(lib,"..\\GTOP\\debug\\GTOP.lib")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGTBan::CGTBan()
{

}

CGTBan::~CGTBan()
{

}
int CGTBan::KickPlayer(int GameID,char * GT_ServerIP,char * GT_ServerName,char *GT_Account)
{
	int res=GT_KICK_USER(GT_ServerIP,GT_Account);	
	return 1;
}

//Ω‚∑‚’À∫≈
int CGTBan::OpenPlayer(int GameID,char * GT_ServerIP,char *GT_ServerName,char * GT_Account)
{
	int res=GT_BAN_USER(GT_ServerIP,GT_Account,0);
	return res;
}
//∑‚Õ£’À∫≈
int CGTBan::ClosePlayer(int GameID,char * GT_ServerIP,char * GT_ServerName,char * GT_Account)
{
	int res=GT_BAN_USER(GT_ServerIP,GT_Account,1);
	return res;
}
//≤È—Ø–≈œ¢
int CGTBan::QueryAccount(int GameID,char * GT_ServerIP,char * GT_ServerName,char *GT_Account, char * GT_UserNick)
{
	operBan.FindDBIP(GameID,GT_ServerIP,GT_ServerName,GT_ServerIP,3);
	if(!strcmp(GT_Account,"")||!strcmp(GT_Account,"NULL"))
	{
		if(!operBan.getRemoteSql("GT_Nick_Account_Ban",operRemoteSql,1))
		{
			return false;
		}
		sprintf(operszSql,operRemoteSql,GT_UserNick);
	//	sprintf(operszSql,"select userName from gtown_db_gamedb.dbo.gUser where nickName='%s'",GT_UserNick);
		operBan.QuerySingleValue("GT",GT_ServerIP,0,operszSql,GT_Account);
	}
	else
	{
		if(!operBan.getRemoteSql("GT_Account_Nick_Ban",operRemoteSql,1))
		{
			return false;
		}
		sprintf(operszSql,operRemoteSql,GT_Account);
	//	sprintf(operszSql,"select nickName from gtown_db_gamedb.dbo.gUser where userName='%s'",GT_Account);
		operBan.QuerySingleValue("GT",GT_ServerIP,0,operszSql,GT_UserNick);

	}
	return 1;
}

