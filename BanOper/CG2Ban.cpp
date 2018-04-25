// CG2Ban.cpp: implementation of the CCG2Ban class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CG2Ban.h"
#include "..\\CG2\\CG2.h"
#pragma comment(lib,"..\\CG2\\debug\\CG2.lib")
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCG2Ban::CCG2Ban()
{

}

CCG2Ban::~CCG2Ban()
{

}
//Ãﬂ»À
int CCG2Ban::KickPlayer(int GameID,char * ServerIP,char* ServerName,char *UserName,char *UserNick,char * memo)
{
	operBan.FindDBIP(GameID,ServerIP,ServerName,ServerIP,1);
	int res=CG2_GMInsert_Query(ServerIP,UserName,UserNick,"","",memo,0);
	return res;
}

//Ω‚∑‚’À∫≈
int CCG2Ban::OpenPlayer(int GameID,char * ServerIP,char* ServerName,char *UserName,char *UserNick,char * reason,char * memo)
{
	operBan.FindDBIP(GameID,ServerIP,ServerName,ServerIP,2);
	int res=CG2_GMInsert_Query(ServerIP,UserName,UserNick,"",reason,memo,1);
	return res;
}
//∑‚Õ£’À∫≈
int CCG2Ban::ClosePlayer(int GameID,char * ServerIP,char* ServerName,char *UserName,char *UserNick,char *EndTime, char * reason,char * memo)
{
	operBan.FindDBIP(GameID,ServerIP,ServerName,ServerIP,2);
	int res=CG2_GMInsert_Query(ServerIP,UserName,UserNick,EndTime,reason,memo,2);
	return res;
}
//≤È—Ø–≈œ¢
int CCG2Ban::QueryAccount(int GameID,char * ServerIP,char * ServerName,char *UserName,char * UserNick)
{
	operBan.FindDBIP(GameID,ServerIP,ServerName,ServerIP,3);
	
	if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
	{
		if(!operBan.getRemoteSql("CG2_Nick_Account_Ban",operRemoteSql,1))
		{
			return false;
		}
		sprintf(operszSql,operRemoteSql,UserNick);

	//	sprintf(operszSql,"select cdkey from tbl_charBase where name='%s'",UserNick);
		operBan.QuerySingleValue("CG2",ServerIP,3,operszSql,UserName);
		if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
		{
			operBan.QuerySingleValue("CG2",ServerIP,5,operszSql,UserName);
		}
	}
	else
	{
		if(!operBan.getRemoteSql("CG2_Account_Nick_Ban",operRemoteSql,1))
		{
			return false;
		}
		sprintf(operszSql,operRemoteSql,UserName);
	//	sprintf(operszSql,"select name from tbl_charBase where cdkey='%s'",UserName);
		operBan.QuerySingleValue("CG2",ServerIP,3,operszSql,UserNick);
		if(!strcmp(UserNick,"")||!strcmp(UserNick,"NULL"))
		{
			operBan.QuerySingleValue("CG2",ServerIP,5,operszSql,UserNick);
		}
	}

	return 1;
}

