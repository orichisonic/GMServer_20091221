// RayBan.cpp: implementation of the CRayBan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RayBan.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRayBan::CRayBan()
{

}

CRayBan::~CRayBan()
{

}
//����
int CRayBan::KickPlayer(int GameID,char * RAY_ServerIP,char* RAY_ServerName,char *UserName)
{

	operBan.FindDBIP(GameID,RAY_ServerIP,RAY_ServerName,RAY_ServerIP,1);
//	int res=BanishPlayer(JW2_ServerIP,JW2_UserName,operName);
	int res=0;
	return res;
}

//����˺�
int CRayBan::OpenPlayer(int GameID,char * RAY_ServerIP,char* RAY_ServerName,char * UserName)
{
	operBan.FindDBIP(GameID,RAY_ServerIP,RAY_ServerName,RAY_ServerIP,2);
	if(!operBan.getRemoteSql("RAY_ACCOUNT_OPEN",operRemoteSql,1))
	{
		return false;
	}
	sprintf(operszSql,operRemoteSql,UserName);//����������ѯ��sql���
	int res=DBVect.OperVectorMain("RAY", RAY_ServerIP, operszSql, 0);//��������ݿ��ѯ�õ�������
	return res;
}
//��ͣ�˺�
int CRayBan::ClosePlayer(int GameID,char * RAY_ServerIP,char* RAY_ServerName,char * UserName)
{
	operBan.FindDBIP(GameID,RAY_ServerIP,RAY_ServerName,RAY_ServerIP,2);
	if(!operBan.getRemoteSql("RAY_ACCOUNT_CLOSE",operRemoteSql,1))
	{
		return false;
	}
	sprintf(operszSql,operRemoteSql,UserName);//����������ѯ��sql���
	int res=DBVect.OperVectorMain("RAY", RAY_ServerIP, operszSql, 0);//��������ݿ��ѯ�õ�������
	return res;
}
//��ѯ��Ϣ
int CRayBan::QueryAccount(int GameID,char * RAY_ServerIP,char * RAY_ServerName,char *UserName,char * UserNick)
{
	operBan.FindDBIP(GameID,RAY_ServerIP,RAY_ServerName,RAY_ServerIP,3);
	if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
	{
		if(!operBan.getRemoteSql("RAY_Nick_Account_Ban",operRemoteSql,1))
		{
			return false;
		}
		sprintf(operszSql,operRemoteSql,UserNick);
	//	sprintf(operszSql,"select a.nyuserid from GlobalAccount.dbo.account a,GlobalAccount.dbo.character b where a.accountid=b.accountid and b.charactername='%s'",UserNick);
		operBan.QuerySingleValue("RAY",RAY_ServerIP,0,operszSql,UserName);
	}
	else
	{
		if(!operBan.getRemoteSql("RAY_Account_Nick_Ban",operRemoteSql,1))
		{
			return false;
		}
		sprintf(operszSql,operRemoteSql,UserName);
	//	sprintf(operszSql,"select b.charactername from GlobalAccount.dbo.account a,GlobalAccount.dbo.character b where a.accountid=b.accountid and a.nyuserid='%s'",UserName);
		operBan.QuerySingleValue("RAY",RAY_ServerIP,0,operszSql,UserNick);
	}
	return 1;
}

