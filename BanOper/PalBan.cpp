// PalBan.cpp: implementation of the CPalBan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PalBan.h"
#include "..\\Pal\\Pal.h"
#pragma comment(lib,"..\\Pal\\debug\\Pal.lib")
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPalBan::CPalBan()
{

}

CPalBan::~CPalBan()
{

}

//����
int CPalBan::KickPlayer(int GameID,char * Pal_ServerIP,char * Pal_ServerName,char *Pal_userNick)
{
	char message[512];
	memset(message, 0, 512);
	
	if (Pal_ServerIP==NULL || Pal_userNick==NULL)
	{
		operBan.OperFileLog("Ban[�ɽ�]", "ServerIp or UserNick empty!");
		return 0;
	}
	sprintf(message, "<����>%s[%s], <��ɫ��>%s, begin Kick!", Pal_ServerName, Pal_ServerIP, Pal_userNick);
	operBan.OperFileLog("Ban[�ɽ�]", message);

	Pal_BanishPlayer(Pal_ServerIP,Pal_ServerName,Pal_userNick);

	sprintf(message, "<����>%s[%s], <��ɫ��>%s, Kick Success!", Pal_ServerName, Pal_ServerIP, Pal_userNick);
	operBan.OperFileLog("Ban[�ɽ�]", message);

	return 1;
}

//����˺�
int CPalBan::OpenPlayer(int GameID,char * Pal_ServerIP,char * Pal_ServerName,char * Pal_Account)
{
	operBan.FindDBIP(GameID,Pal_ServerIP,Pal_ServerName,Pal_ServerIP,2);
	if(!operBan.getRemoteSql("PAL_ACCOUNT_OPEN",operRemoteSql,1))
	{
		return false;
	}
	sprintf(operszSql,operRemoteSql,Pal_Account);//����������ѯ��sql���
	int res=DBVect.OperVectorMain(Pal_ServerIP, operszSql, 0);//��������ݿ��ѯ�õ�������
	return res;
}
//��ͣ�˺�
int CPalBan::ClosePlayer(int GameID,char * Pal_ServerIP,char * Pal_ServerName,char * Pal_Account)
{
	operBan.FindDBIP(GameID,Pal_ServerIP,Pal_ServerName,Pal_ServerIP,2);
	if(!operBan.getRemoteSql("PAL_ACCOUNT_CLOSE",operRemoteSql,1))
	{
		return false;
	}
	sprintf(operszSql,operRemoteSql,Pal_Account);//����������ѯ��sql���
	int res=DBVect.OperVectorMain(Pal_ServerIP, operszSql, 0);//��������ݿ��ѯ�õ�������
	return res;
}
//��ѯ��Ϣ
int CPalBan::FindAccount(int GameID,char * Pal_ServerIP,char * Pal_ServerName,char *Pal_Account, char * Pal_RoleName)
{
	char temp_Pal_ServerIP[30];
	memset(temp_Pal_ServerIP, 0, 30);

	operBan.FindDBIP(GameID,Pal_ServerIP,Pal_ServerName,temp_Pal_ServerIP,3);
	if(!strcmp(Pal_Account,"")||!strcmp(Pal_Account,"NULL"))
	{
		if(!operBan.getRemoteSql("PAL_Nick_Account_Ban",operRemoteSql,1))
		{
			return false;
		}
		sprintf(operszSql,operRemoteSql,Pal_RoleName);
	//	sprintf(operszSql,"select a.accountname from gamedb.account a,gamedb.roledata b where a.id=b.accountid and b.rolename='%s'",Pal_RoleName);
		operBan.QuerySingleValue("Pal",temp_Pal_ServerIP,1,operszSql,Pal_Account);
	}
	else
	{
		if(!operBan.getRemoteSql("PAL_Account_Nick_Ban",operRemoteSql,1))
		{
			return false;
		}
		sprintf(operszSql,operRemoteSql,Pal_Account);
	//	sprintf(operszSql,"select b.rolename from gamedb.account a,gamedb.roledata b where a.id=b.accountid and a.accountname='%s'",Pal_Account);
		operBan.QuerySingleValue("Pal",temp_Pal_ServerIP,1,operszSql,Pal_RoleName);

	}

	return 1;
}