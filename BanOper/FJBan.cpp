// FJBan.cpp: implementation of the CFJBan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FJBan.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFJBan::CFJBan()
{

}

CFJBan::~CFJBan()
{

}

//踢人
int CFJBan::KickPlayer(int GameID,char * FJ_ServerIP,char* FJ_ServerName,char *UserName)
{

	return 0;
}

//解封账号
int CFJBan::OpenPlayer(int GameID,char * FJ_ServerIP,char* FJ_ServerName,char *UserName)
{
	operBan.FindDBIP(GameID,FJ_ServerIP,FJ_ServerName,FJ_ServerIP,2);
	if(!operBan.getRemoteSql("FJ_ACCOUNT_OPEN",operRemoteSql,1))
	{
		return false;
	}
	sprintf(operszSql,operRemoteSql,UserName);//构造完整查询的sql语句
	int res=DBVect.OperVectorMain("FJ", FJ_ServerIP, operszSql,0);//构造从数据库查询得到的数据
	return res;
}
//封停账号
int CFJBan::ClosePlayer(int GameID,char * FJ_ServerIP,char* FJ_ServerName,char *UserName)
{
	operBan.FindDBIP(GameID,FJ_ServerIP,FJ_ServerName,FJ_ServerIP,2);
	if(!operBan.getRemoteSql("FJ_ACCOUNT_CLOSE",operRemoteSql,1))
	{
		return false;
	}
	sprintf(operszSql,operRemoteSql,UserName);//构造完整查询的sql语句
	int res=DBVect.OperVectorMain("FJ", FJ_ServerIP, operszSql, 0);//构造从数据库查询得到的数据
	return res;
}
//查询信息
int CFJBan::QueryAccount(int GameID,char * FJ_ServerIP,char * FJ_ServerName,char *UserName,char * UserNick)
{
	operBan.FindDBIP(GameID,FJ_ServerIP,FJ_ServerName,FJ_ServerIP,3);

	if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
	{
		if(!operBan.getRemoteSql("FJ_Nick_Account_Ban",operRemoteSql,1))
		{
			return false;
		}
		sprintf(operszSql,operRemoteSql,UserNick);
	//	sprintf(operszSql,"select account from lingo.dbo.characters where char_name='%s'",UserNick);
		operBan.QuerySingleValue("FJ",FJ_ServerIP,0,operszSql,UserName);
	}
	else
	{
		if(!operBan.getRemoteSql("FJ_Account_Nick_Ban",operRemoteSql,1))
		{
			return false;
		}
		sprintf(operszSql,operRemoteSql,UserName);
	//	sprintf(operszSql,"select char_name from lingo.dbo.characters where account='%s'",UserName);
		operBan.QuerySingleValue("FJ",FJ_ServerIP,0,operszSql,UserNick);
	}
	return 1;
}

