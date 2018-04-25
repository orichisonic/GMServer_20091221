// SDBan.cpp: implementation of the CSDBan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SDBan.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSDBan::CSDBan()
{

}

CSDBan::~CSDBan()
{

}
//����
int CSDBan::KickPlayer(int GameID,char * SD_ServerIP,char* SD_ServerName,char *UserName)
{

	operBan.FindDBIP(GameID,SD_ServerIP,SD_ServerName,SD_ServerIP,1);
//	int res=BanishPlayer(JW2_ServerIP,JW2_UserName,operName);
	int res=0;
	return res;
}

//����˺�
int CSDBan::OpenPlayer(int GameID,char * SD_ServerIP,char* SD_ServerName,char * UserName)
{
	operBan.FindDBIP(GameID,SD_ServerIP,SD_ServerName,SD_ServerIP,2);
	if(!operBan.getRemoteSql("SD_ACCOUNT_OPEN",operRemoteSql,1))
	{
		return false;
	}
	sprintf(operszSql,operRemoteSql,UserName);//����������ѯ��sql���
	int res=DBVect.OperVectorMain("SD", SD_ServerIP, operszSql, 1);//��������ݿ��ѯ�õ�������
	return res;
}
//��ͣ�˺�
int CSDBan::ClosePlayer(int GameID,char * SD_ServerIP,char* SD_ServerName,char * UserName)
{
	operBan.FindDBIP(GameID,SD_ServerIP,SD_ServerName,SD_ServerIP,2);
	if(!operBan.getRemoteSql("SD_ACCOUNT_CLOSE",operRemoteSql,1))
	{
		return false;
	}
	sprintf(operszSql,operRemoteSql,UserName);//����������ѯ��sql���
	int res=DBVect.OperVectorMain("SD", SD_ServerIP, operszSql, 1);//��������ݿ��ѯ�õ�������
	return res;
}
//��ѯ��Ϣ
int CSDBan::QueryAccount(int GameID,char * SD_ServerIP,char* SD_ServerName,char *UserName,char * UserNick)
{
	operBan.FindDBIP(GameID,SD_ServerIP,SD_ServerName,SD_ServerIP,3);
	operBan.QuerySingleValue("SD",SD_ServerIP,1,operszSql,UserName);
	return 1;
}

