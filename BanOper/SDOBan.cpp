// SDOBan.cpp: implementation of the CSDOBan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SDOBan.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSDOBan::CSDOBan()
{

}

CSDOBan::~CSDOBan()
{

}
//踢人
int CSDOBan::KickPlayer(int GameID,char * SDO_ServerIP,char* SDO_ServerName,char *UserName)
{
	operBan.FindDBIP(GameID,SDO_ServerIP,SDO_ServerName,SDO_ServerIP,1);
	if(!operBan.getRemoteSql("SDO_KickPlayer_Off",operRemoteSql,1))
	{
		return false;
	}
	int UserID=QueryAccountID(GameID,SDO_ServerIP,SDO_ServerName,UserName);
	sprintf(operszSql,operRemoteSql,"sdogame",UserID);//构造完整查询的sql语句
	int res=DBVect.OperVectorMain("SDO", SDO_ServerIP, operszSql, 0);//构造从数据库查询得到的数据
	return res;
}

//解封账号
int CSDOBan::OpenPlayer(int GameID,char * SDO_ServerIP,char* SDO_ServerName,char* UserName)
{
	operBan.FindDBIP(GameID,SDO_ServerIP,SDO_ServerName,SDO_ServerIP,2);
	if(!operBan.getRemoteSql("SDO_Account_Open",operRemoteSql,1))
	{
		return false;
	}
	sprintf(operszSql,operRemoteSql,"sdogame",UserName);//构造完整查询的sql语句
	int res=DBVect.OperVectorMain("SDO", SDO_ServerIP, operszSql, 0);//构造从数据库查询得到的数据
	return res;
}
//封停账号
int CSDOBan::ClosePlayer(int GameID,char * SDO_ServerIP,char* SDO_ServerName,char* UserName)
{
	operBan.FindDBIP(GameID,SDO_ServerIP,SDO_ServerName,SDO_ServerIP,2);
	if(!operBan.getRemoteSql("SDO_Account_Close",operRemoteSql,1))
	{
		return false;
	}
	sprintf(operszSql,operRemoteSql,"sdogame","sdogame",UserName);//构造完整查询的sql语句
	int res=DBVect.OperVectorMain("SDO", SDO_ServerIP, operszSql, 0);//构造从数据库查询得到的数据
	return res;
}

//查询帐号有没有封停
int CSDOBan::RemoteTest(int GameID,char * SDO_ServerIP,char * SDO_ServerName,char *UserName)
{
	char UserID[20];
	int res=0;
	operBan.FindDBIP(GameID,SDO_ServerIP,SDO_ServerName,SDO_ServerIP,2);
	sprintf(operszSql,"select UserID from [sdogame].dbo.T_sdo_banishment where userid = '%s'",UserName);//构造完整查询的sql语句
	operBan.QuerySingleValue("SDO",SDO_ServerIP,0,operszSql,UserID);
	if(!strcmp(UserID,"")||!strcmp(UserID,"NULL"))
	{
		res=1;
	}
	return res;
}
//查询信息(通过昵称查账号)
int CSDOBan::QueryAccount(int GameID,char * SDO_ServerIP,char * SDO_ServerName,char *UserName,char * UserNick)
{
	operBan.FindDBIP(GameID,SDO_ServerIP,SDO_ServerName,SDO_ServerIP,3);
	if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
	{
		if(!operBan.getRemoteSql("SDO_Nick_Account_Ban",operRemoteSql,1))
		{
			return false;
		}
		sprintf(operszSql,operRemoteSql,UserNick);
	//	sprintf(operszSql,"select userid from member where usernick='%s'",UserNick);
		operBan.QuerySingleValue("SDO",SDO_ServerIP,0,operszSql,UserName);
	}
	else
	{
		if(!operBan.getRemoteSql("SDO_Account_Nick_Ban",operRemoteSql,1))
		{
			return false;
		}
		sprintf(operszSql,operRemoteSql,UserName);
	//	sprintf(operszSql,"select usernick from member where userid='%s'",UserName);
		operBan.QuerySingleValue("SDO",SDO_ServerIP,0,operszSql,UserNick);
	}
	return 1;
}

int CSDOBan::QueryAccountID(int GameID,char* SDO_ServerIP,char* SDO_ServerName,char *UserName)
{
	char strResult[256];
	operBan.FindDBIP(GameID,SDO_ServerIP,SDO_ServerName,SDO_ServerIP,3);
	sprintf(operszSql,"select id from member where userid='%s'",UserName);
	operBan.QuerySingleValue("SDO",SDO_ServerIP,0,operszSql,strResult);
	return StrToInt(strResult);
}