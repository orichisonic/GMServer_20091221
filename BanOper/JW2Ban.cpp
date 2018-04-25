// JW2Ban.cpp: implementation of the CJW2Ban class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JW2Ban.h"
#include "OperBan.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJW2Ban::CJW2Ban()
{

}

CJW2Ban::~CJW2Ban()
{

}
//踢人
int CJW2Ban::KickPlayer(int GameID,char * JW2_ServerIP,char* JW2_ServerName,char *JW2_UserName, int userByID)
{
	char operName[10];
	sprintf(operName,"%i",userByID);
	operBan.FindDBIP(GameID,JW2_ServerIP,JW2_ServerName,JW2_ServerIP,1);
	int res=BanishPlayer(JW2_ServerIP,JW2_UserName,operName);
	return res;
}

//解封账号
int CJW2Ban::OpenPlayer(int GameID,char * JW2_ServerIP,char* JW2_ServerName,char *UserName)
{
	int JW2_UserSN=0;
	JW2_UserSN=FindUserSN(GameID,JW2_ServerIP,JW2_ServerName,UserName);
	operBan.FindDBIP(GameID,JW2_ServerIP,JW2_ServerName,JW2_ServerIP,2);
	if(!operBan.getRemoteSql("JW2_ACCOUNT_OPEN",operRemoteSql,1))
	{
		return false;
	}
	sprintf(operszSql,operRemoteSql,JW2_UserSN);//构造完整查询的sql语句
	int res=DBVect.OperVectorMain("JW2", JW2_ServerIP, operszSql, 1);//构造从数据库查询得到的数据
	return res;
}
//封停账号
int CJW2Ban::ClosePlayer(int GameID,char * JW2_ServerIP,char* JW2_ServerName,char *UserName)
{
	int JW2_UserSN=0;
	JW2_UserSN=FindUserSN(GameID,JW2_ServerIP,JW2_ServerName,UserName);
	operBan.FindDBIP(GameID,JW2_ServerIP,JW2_ServerName,JW2_ServerIP,2);
	if(!operBan.getRemoteSql("JW2_ACCOUNT_CLOSE",operRemoteSql,1))
	{
		return false;
	}
	sprintf(operszSql,operRemoteSql,JW2_UserSN);//构造完整查询的sql语句
	int res=DBVect.OperVectorMain("JW2", JW2_ServerIP, operszSql, 1);//构造从数据库查询得到的数据
	return res;
}
//查询信息
int CJW2Ban::QueryAccount(int GameID,char * JW2_ServerIP,char * JW2_ServerName,char *UserName,char * UserNick)
{
	operBan.FindDBIP(GameID,JW2_ServerIP,JW2_ServerName,JW2_ServerIP,3);
	if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
	{
		if(!operBan.getRemoteSql("JW2_Nick_Account_Ban",operRemoteSql,1))
		{
			return false;
		}
		sprintf(operszSql,operRemoteSql,UserNick);
	//	sprintf(operszSql,"select UserID from game_db.userinfo where UserNick='%s'",UserNick);
		operBan.QuerySingleValue("JW2",JW2_ServerIP,1,operszSql,UserName);
	}
	else
	{
		if(!operBan.getRemoteSql("JW2_Account_Nick_Ban",operRemoteSql,1))
		{
			return false;
		}
		sprintf(operszSql,operRemoteSql,UserName);
	//	sprintf(operszSql,"select UserNick from game_db.userinfo where UserID='%s'",UserName);
		operBan.QuerySingleValue("JW2",JW2_ServerIP,1,operszSql,UserNick);

	}

	return 1;
}

int CJW2Ban::FindUserSN(int GameID,char * JW2_ServerIP,char * JW2_ServerName,char *UserName)
{
	char m_result[10];
	operBan.FindDBIP(GameID,JW2_ServerIP,JW2_ServerName,JW2_ServerIP,3);
	sprintf(operszSql,"select UserSN from game_db.userinfo where UserID='%s'",UserName);
	operBan.QuerySingleValue("JW2",JW2_ServerIP,1,operszSql,m_result);
	return StrToInt(m_result);
}