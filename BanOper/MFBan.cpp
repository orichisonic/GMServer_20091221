// MFBan.cpp: implementation of the CMFBan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MFBan.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMFBan::CMFBan()
{

}

CMFBan::~CMFBan()
{

}
//踢人
int CMFBan::KickPlayer(int GameID,char * MF_ServerIP,char* MF_ServerName,char *UserName)
{
	char message[512];
	memset(message, 0, 512);

	char UserID[20];
	QueryUserID(GameID,MF_ServerIP,MF_ServerName,UserName,UserID);
	int iPlayerID=StrToInt(UserID);
	operBan.FindDBIP(GameID,MF_ServerIP,MF_ServerName,MF_ServerIP,1);
	
	CMSocket m_socket;
	char strIP[40];
	int iPort=0;
	unsigned char strSend[8];	//固定包长是8个字节
	int iSend=8;

	strSend[0]=0x08;
	strSend[1]=0x00;
	strSend[2]=0x11;
	strSend[3]=0x00;
	memcpy(&strSend[4],(int *)&iPlayerID,sizeof(unsigned int));
	
	vector<CGlobalStruct::DataRecord> datarecord;
	if(!datarecord.empty())
	{
		datarecord.clear();
	}

	CSqlHelper m_SqlHelper;
	int res=1;
	sprintf(operszSql,"select manegerip,port from Mf_ManegerIP where serverip='%s'",MF_ServerIP);
	m_SqlHelper.SqlHelperMain(&datarecord, "127.0.0.1",operszSql, 4, 0, 0);//获取对应的LogDBIP
	printf(operszSql);
	//获取查询的记录集的每一项
	vector<CGlobalStruct::DataRecord>::iterator iter;
	CGlobalStruct::DataRecord  m_DataRecord;	
	for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
	{
		m_DataRecord = *iter;
		if(!strcmp("manegerip", m_DataRecord.recordfieldname))//获取相应的LogDBIP
		{
			sprintf(strIP,"%s",m_DataRecord.recorddata);
		}
		else if(!strcmp("port", m_DataRecord.recordfieldname))//获取相应的LogDBIP
		{
			iPort=StrToInt(m_DataRecord.recorddata);
			m_socket.ConnectURL(strIP,iPort);//连接远程
			res=m_socket.SendNewBuf((char*)strSend,iSend);
			m_socket.closeSocket();

			sprintf(message, "<大区>%s[%s:%d], <用户名>%s(%s), Kick Success!", MF_ServerName, strIP, iPort, UserName, UserID);
			operBan.OperFileLog("宠物森林", message);
		}
	}
	if(!datarecord.empty())
	{
		datarecord.clear();
	}
	
	return res;
}

//解封账号
int CMFBan::OpenPlayer(int GameID,char * MF_ServerIP,char* MF_ServerName,char *UserName)
{
	char UserID[20];
	QueryUserID(GameID,MF_ServerIP,MF_ServerName,UserName,UserID);
	operBan.FindDBIP(GameID,MF_ServerIP,MF_ServerName,MF_ServerIP,2);
	if(!operBan.getRemoteSql("MF_ACCOUNT_OPEN",operRemoteSql,1))
	{
		return false;
	}
	sprintf(operszSql,operRemoteSql,UserID);//构造完整查询的sql语句
	operBan.OperFileLog("宠物森林", operszSql, false);
	int res=DBVect.OperVectorMain("MF", MF_ServerIP, operszSql, 1);//构造从数据库查询得到的数据
	return res;
}
//封停账号
int CMFBan::ClosePlayer(int GameID,char * MF_ServerIP,char* MF_ServerName,char *UserName,char * reason)
{
	char UserID[20];
	QueryUserID(GameID,MF_ServerIP,MF_ServerName,UserName,UserID);
	operBan.FindDBIP(GameID,MF_ServerIP,MF_ServerName,MF_ServerIP,2);
	if(!operBan.getRemoteSql("MF_ACCOUNT_CLOSE",operRemoteSql,1))
	{
		return false;
	}
	sprintf(operszSql,operRemoteSql,UserID,"2030-12-30 00:00:00",reason);//构造完整查询的sql语句
	operBan.OperFileLog("宠物森林", operszSql, false);
	int res=DBVect.OperVectorMain("MF", MF_ServerIP, operszSql, 1);//构造从数据库查询得到的数据
	return res;
}
//查询帐号有没有封停
int CMFBan::RemoteTest(int GameID,char * MF_ServerIP,char* MF_ServerName,char *UserName)
{
	char UserID[20];
	int res=0;
	QueryUserID(GameID,MF_ServerIP,MF_ServerName,UserName,UserID);
	operBan.FindDBIP(GameID,MF_ServerIP,MF_ServerName,MF_ServerIP,2);
	sprintf(operszSql,"select id ago.lock_user where id=%i",StrToInt(UserID));//构造完整查询的sql语句
	operBan.QuerySingleValue("MF",MF_ServerIP,1,operszSql,UserID);
	if(!strcmp(UserID,"")||!strcmp(UserID,"NULL"))
	{
		res=1;
	}
	return res;
}
//查询信息
int CMFBan::QueryAccount(int GameID,char * MF_ServerIP,char * MF_ServerName,char *UserName,char * UserNick)
{
	operBan.FindDBIP(GameID,MF_ServerIP,MF_ServerName,MF_ServerIP,3);
	if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
	{
		if(!operBan.getRemoteSql("MF_Nick_Account_Ban",operRemoteSql,1))
		{
			return false;
		}
		sprintf(operszSql,operRemoteSql,UserNick);
		operBan.OperFileLog("宠物森林", operszSql, false);
	//	sprintf(operszSql,"select username from ago.member a,ago.character b where a.id=b.id and b.nickname='%s'",UserNick);
		operBan.QuerySingleValue("MF",MF_ServerIP,1,operszSql,UserName);
	}
	else
	{
		if(!operBan.getRemoteSql("MF_Account_Nick_Ban",operRemoteSql,1))
		{
			return false;
		}
		sprintf(operszSql,operRemoteSql,UserName);
		operBan.OperFileLog("宠物森林", operszSql, false);
	//	sprintf(operszSql,"select nickname from ago.member a,ago.character b where a.id=b.id and a.username='%s'",UserName);
		operBan.QuerySingleValue("MF",MF_ServerIP,1,operszSql,UserNick);
	}
	return 1;
}

int CMFBan::QueryUserID(int GameID,char * MF_ServerIP,char * MF_ServerName,char *UserName,char * UserID)
{
	operBan.FindDBIP(GameID,MF_ServerIP,MF_ServerName,MF_ServerIP,3);
	sprintf(operszSql,"select id from ago.member where username='%s'",UserName);
	operBan.QuerySingleValue("MF",MF_ServerIP,1,operszSql,UserID);
	return 1;
}
