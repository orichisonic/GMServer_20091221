// SltqBan.cpp: implementation of the CSltqBan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SltqBan.h"
#include "gmsglobaldefine.h"
#include "gmsprotocols2t.h"
#include "gmsprotocolt2s.h"
#include "MySocket.h"

using namespace GMSBase;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define		MAXLEN     1024

CSltqBan::CSltqBan()
{

}

CSltqBan::~CSltqBan()
{

}
//踢人
int CSltqBan::KickPlayer(int GameID,char * STLQ_ServerIP,char* STLQ_ServerName,char *UserNick)
{
	int res=1;

	try
	{
		char InterfaceSer[30];
		char message[512];
		memset(InterfaceSer, 0, 30);
		memset(message, 0, 512);

		operBan.FindStlDBIP(STLQ_ServerIP,STLQ_ServerName, InterfaceSer,1);
		
		if (!strcmp(InterfaceSer,"") || UserNick==NULL)
		{
			operBan.OperFileLog("篮球", "ServerIp or UserNick empty!");
			return 0;
		}

		CMySocket m_socket;
		char sendbuff[MAXLEN];
		char recvBuff[MAXLEN];

		REQ_LOGIN_GMS req_login_gas;
		RES_LOGIN_GMS res_login_gas;

		short int REQ_LOGIN_GMS_Len = sizeof(REQ_LOGIN_GMS);
		
		memset(sendbuff, 0, MAXLEN);
		
		sprintf(req_login_gas.szAccountName,"%s","admin");//"gmtools2");    "admin"
		sprintf(req_login_gas.szPassword,"%s","d!46KaP4");//"gmtools2");    "d!46KaP4"
		req_login_gas.nDstGMSID=8888;
		
		memcpy(sendbuff, &REQ_LOGIN_GMS_Len, 2);
		memcpy(sendbuff+2,(char *)&req_login_gas,sizeof(REQ_LOGIN_GMS));
		
		if(!m_socket.MyConnect(InterfaceSer,13334))	//114.80.167.227, 192.168.53.12
		{
			sprintf(message, "<大区>%s[%s], <角色名>%s, connect error", STLQ_ServerName, InterfaceSer, UserNick);
			operBan.OperFileLog("篮球", message);
			m_socket.Close();
			return 0;
		}
		
		m_socket.MySend(sendbuff, REQ_LOGIN_GMS_Len+2);
		
		memset(recvBuff, 0, MAXLEN);
		int recvByte = m_socket.MyRecv(recvBuff, MAXLEN);
		if (recvByte>0)
		{
			int packeLen = 	*(short int *)recvBuff;
			
			memcpy(&res_login_gas, recvBuff+2, packeLen);
			
			if (!res_login_gas.bOk)
			{
				sprintf(message, "<大区>%s[%s], <角色名>%s, Login Failed", STLQ_ServerName, InterfaceSer, UserNick);
				operBan.OperFileLog("篮球", message);
				m_socket.Close();
				return 0;
			}
			
			REQ_KICK_PLAYER req_kick_player;
			RES_KICK_PLAYER res_kick_player;
			
			short int REQ_KICK_PLAYER_Len = sizeof(REQ_KICK_PLAYER);
			
			memset(sendbuff, 0, MAXLEN);
			
			sprintf(req_kick_player.szCharName, "%s", UserNick);

			
			memcpy(sendbuff, &REQ_KICK_PLAYER_Len, 2);
			memcpy(sendbuff+2,(char *)&req_kick_player,sizeof(REQ_KICK_PLAYER));
			
			m_socket.MySend(sendbuff, REQ_KICK_PLAYER_Len+2);
			
			memset(recvBuff, 0, MAXLEN);
			recvByte = m_socket.MyRecv(recvBuff, MAXLEN);
			if (recvByte >0)
			{
				packeLen = 	*(short int *)recvBuff;
				
				memcpy(&res_kick_player, recvBuff+2, packeLen);

				sprintf(message, "<大区>%s[%s], <角色名>%s, Kick Success!", STLQ_ServerName, InterfaceSer, UserNick);
				operBan.OperFileLog("篮球", message);
			}
			else
			{
				sprintf(message, "<大区>%s[%s], <角色名>%s, Kick Failed!", STLQ_ServerName, InterfaceSer, UserNick);
				operBan.OperFileLog("篮球", message);
				res = 0; 
			}
		}
		else
		{
			sprintf(message, "<大区>%s[%s], <角色名>%s, Login Failed1", STLQ_ServerName, InterfaceSer, UserNick);
			operBan.OperFileLog("篮球", message);
			res = 0; 
		}
		
		m_socket.Close();
	}
	catch(...)
	{
		res=0;
	}

	return res;

	//	operBan.FindDBIP(GameID,STLQ_ServerIP,STLQ_ServerName,STLQ_ServerIP,3);//1);


	/*if(LoginServer(STLQ_ServerIP))
		return 0;*/

/*
	CMSocket m_socket;
	m_socket.ConnectURL(STLQ_ServerIP,13334);
	char* pSend=new char[sizeof(REQ_LOGIN_GMS)+2];
////	unsigned char* pRecv=new unsigned char[sizeof(RES_LOGIN_GMS)+2];
	ZeroMemory(pSend,sizeof(REQ_LOGIN_GMS)+2);
////////	ZeroMemory(pRecv,sizeof(RES_LOGIN_GMS)+2);
	REQ_LOGIN_GMS req_login_gas;
	RES_LOGIN_GMS res_login_gas;
	sprintf(req_login_gas.szAccountName,"%s","admin");//"gmtools2");
	sprintf(req_login_gas.szPassword,"%s","d!46KaP4");//"gmtools2");
	req_login_gas.nDstGMSID=8888;
	*pSend=sizeof(REQ_LOGIN_GMS);
	memcpy(pSend+2,(char *)&req_login_gas,sizeof(REQ_LOGIN_GMS));
	
	m_socket.SendNewBuf(pSend,sizeof(REQ_LOGIN_GMS)+2);
	Sleep(5000);
/////	m_socket.MyReceive(pRecv);
	/ *if(pRecv[6]==0x01)
	{
		return true;
	}
	else
	{
		return false;
	}
	delete[] pSend;
	delete[] pRecv;
	pSend=NULL;
	pRecv=NULL;


	pSend=new char[sizeof(REQ_KICK_PLAYER)+2];
	pRecv=new char[sizeof(RES_KICK_PLAYER)+2];* /
	ZeroMemory(pSend,sizeof(REQ_KICK_PLAYER)+2);
///////	ZeroMemory(pRecv,sizeof(RES_KICK_PLAYER)+2);
	REQ_KICK_PLAYER req_kick_player;
//	RES_KICK_PLAYER res_kick_player;

	*pSend=sizeof(REQ_KICK_PLAYER);
	memcpy(pSend+2,(char *)&req_kick_player,sizeof(REQ_KICK_PLAYER));

	m_socket.SendNewBuf(pSend,sizeof(REQ_KICK_PLAYER)+2);
////	m_socket.MyReceive(pRecv);
	/ *if(pRecv[6]==0x01)
	{
		res=1;
	}
	else
	{
		res=0;
	}* /
	delete[] pSend;
////	delete[] pRecv;
	pSend=NULL;
////	pRecv=NULL;
	m_socket.closeSocket();
//	DisConnect();
	return res;*/

}

//解封账号
int CSltqBan::OpenPlayer(int GameID,char * STLQ_ServerIP,char* STLQ_ServerName,char *UserName,char *UserNick)
{
	char ServerIP[20];
	ZeroMemory(ServerIP,20);
	//operBan.FindDBIP(GameID,STLQ_ServerIP,STLQ_ServerName,SeverIP,2);//STLQ_ServerIP
	operBan.FindStlDBIP(STLQ_ServerIP,STLQ_ServerName,ServerIP,2);
	if(!operBan.getRemoteSql("STLQ_UnBanChar_Query",operRemoteSql,1))
	{
		return false;
	}
	sprintf(operszSql,operRemoteSql,UserName);//构造完整查询的sql语句
	operBan.OperFileLog("篮球", operszSql, false);
	//operBan.QuerySingleValue("SLTQ",SeverIP,0,operszSql,UserName);
	int res=DBVect.OperVectorMain("SLTQ",ServerIP,operszSql,0);
	return res;

}
//封停账号
int CSltqBan::ClosePlayer(int GameID,char * STLQ_ServerIP,char* ServerName,char *UserName,char * UserNick,char * szBeginTime,char * szBanTime)
{
	char ServerIP[20];
	ZeroMemory(ServerIP,20);
	char BanDay[10];
	ZeroMemory(BanDay,10);
	sprintf(BanDay,"0");

	operBan.FindStlDBIP(STLQ_ServerIP,ServerName,ServerIP,2);
	if(!operBan.getRemoteSql("STLQ_BanChar_Query",operRemoteSql,1))
	{
		return false;
	}

	sprintf(operszSql,operRemoteSql,szBeginTime, szBeginTime, szBanTime, UserName);//构造完整查询的sql语句
	operBan.OperFileLog("篮球", operszSql, false);
	//operBan.QuerySingleValue("SLTQ",SeverIP,0,operszSql,UserName);
	int res=DBVect.OperVectorMain("SLTQ",ServerIP,operszSql,0);
	return res;

/*	int res=0;
	//	LoginServer(STLQ_ServerIP);
	char szAccount[32];
	char szPassword[32];
	int iGMServerPort=0;
	unsigned int nDstGMSID=0;
	GetUserNamePwd(STLQ_ServerIP,szAccount,szPassword,&iGMServerPort,&nDstGMSID);
	
	
	CMSocket m_socket;
	m_socket.ConnectURL(STLQ_ServerIP,13334);
	//	m_socket.ConnectURL("192.168.53.139",13334);
	char* pSend=new char[sizeof(REQ_LOGIN_GMS)+2];
	//unsigned char * pRecv=new unsigned char[20];//RES_LOGIN_GMS
	unsigned char * pRecv=new unsigned char[sizeof(RES_LOGIN_GMS)+2];
	unsigned char * pRecvBan=new unsigned char[sizeof(RES_BAN_PLAYER)+2];
	
	ZeroMemory(pSend,sizeof(REQ_LOGIN_GMS)+2);//
	ZeroMemory(pRecv,sizeof(RES_LOGIN_GMS)+2);//20);
	ZeroMemory(pRecvBan,sizeof(RES_BAN_PLAYER)+2);
	REQ_LOGIN_GMS req_login_gas;
	RES_LOGIN_GMS res_login_gms;
	sprintf(req_login_gas.szAccountName,"%s","admin");
	sprintf(req_login_gas.szPassword,"%s","d!46KaP4");
	req_login_gas.nDstGMSID=8888;
	*pSend=sizeof(REQ_LOGIN_GMS);
	memcpy(pSend+2,(char *)&req_login_gas,sizeof(REQ_LOGIN_GMS));//
	
	m_socket.SendNewBuf(pSend,sizeof(REQ_LOGIN_GMS)+2);//
	
	REQ_BAN_PLAYER req_ban_player;
	RES_BAN_PLAYER res_ban_player;
	Sleep(10000);
	m_socket.MyReceive(pRecv);
	memcpy((char *)&res_login_gms,pRecv+2,sizeof(RES_LOGIN_GMS)+2);
	if(res_login_gms.bOk==true)
	{
		
		//pSend=new char[sizeof(REQ_BAN_PLAYER)+2];
		//	char pRecv[20];
		ZeroMemory(pSend,sizeof(REQ_BAN_PLAYER)+2);
		//ZeroMemory(pRecv,sizeof(RES_LOGIN_GMS)+2);
		
		sprintf(req_ban_player.szAccountName,"%s",UserName);
		//	sprintf(req_ban_player.szAccountName,"%s","st10000");
		//maple add
		sprintf(req_ban_player.szCharName,"%s",UserNick);//"一二三四五");//
		req_ban_player.nDay=0;
		
		
		*pSend=sizeof(REQ_BAN_PLAYER);
		memcpy(pSend+2,(char *)&req_ban_player,sizeof(REQ_BAN_PLAYER));
		res=m_socket.SendNewBuf(pSend,sizeof(REQ_BAN_PLAYER)+2);
		m_socket.MyReceive(pRecvBan);
		memcpy((char *)&res_ban_player,pRecvBan+2,sizeof(RES_BAN_PLAYER)+2);
		if(res_ban_player.bBan==true)
		{
			res=1;
		}
		else
			res=0;
	}
	
	delete[] pSend;
	delete[] pRecv;
	delete[] pRecvBan;
	pRecv=NULL;
	pSend=NULL;
	pRecvBan=NULL;
	m_socket.closeSocket();
	return res;
*/
}
//查询信息
int CSltqBan::QueryAccount(int GameID,char * STLQ_ServerIP,char * ServerName,char *UserName,char * UserNick)
{
	char SeverIP[20];
	ZeroMemory(SeverIP,20);
	//operBan.FindDBIP(GameID,STLQ_ServerIP,ServerName,SeverIP,3);//STLQ_ServerIP
	operBan.FindStlDBIP(STLQ_ServerIP,ServerName,SeverIP,3);

	if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
	{
		if(!operBan.getRemoteSql("SLTQ_Nick_Account_Ban",operRemoteSql,1))
		{
			return false;
		}
		sprintf(operszSql,operRemoteSql,UserNick);
		operBan.OperFileLog("篮球", operszSql, false);
	//	sprintf(operszSql,"select account_name from SDGameDB140.dbo.CharacterInfo where char_name='%s'",UserNick);
		operBan.QuerySingleValue("SLTQ",STLQ_ServerIP,0,operszSql,UserName);
	}
	else
	{
		if(!operBan.getRemoteSql("SLTQ_Account_Nick_Ban",operRemoteSql,1))
		{
			return false;
		}
		sprintf(operszSql,operRemoteSql,UserName);
		operBan.OperFileLog("篮球", operszSql, false);
	//	sprintf(operszSql,"select char_name from SDGameDB140.dbo.CharacterInfo where account_name='%s'",UserName);
		operBan.QuerySingleValue("SLTQ",SeverIP,0,operszSql,UserNick);//STLQ_ServerIP
	}
	
	return 1;
}

bool CSltqBan::LoginServer(char *STLQ_ServerIP)
{
//	m_socket.ConnectURL(STLQ_ServerIP,13334);
/*	m_socket.ConnectURL("192.168.53.12",13334);
	char pRecv[20];
	char* pSend=new char[sizeof(REQ_LOGIN_GMS)+2];
	ZeroMemory(pSend,sizeof(REQ_LOGIN_GMS)+2);
	REQ_LOGIN_GMS req_login_gas;
	sprintf(req_login_gas.szAccountName,"%s","gmtools2");
	sprintf(req_login_gas.szPassword,"%s","gmtools2");
	req_login_gas.nDstGMSID=8888;
	*pSend=sizeof(REQ_LOGIN_GMS);
	memcpy(pSend+2,(char *)&req_login_gas,sizeof(REQ_LOGIN_GMS));

	m_socket.SendNewBuf(pSend,sizeof(REQ_LOGIN_GMS)+2);
	m_socket.MyReceive((unsigned char *)pRecv);
	if(pRecv[6]==0x01)
	{
		return true;
	}
	else
	{
		return false;
	}
	delete[] pSend;
	pSend=NULL;*/
	return true;
}
bool CSltqBan::DisConnect()
{
//	m_socket.closeSocket();
	return true;
}

void CSltqBan::GetUserNamePwd(char * title,char * m_UserName,char * m_pwd,int * m_port,unsigned int * m_DstGMSID)
{
	//////////////////读配置文件////////////////////////
	char path[256];
	GetCurrentDirectory(256,path);
	char filepath[256];
	wsprintf(filepath,"%s%s",path,"\\config\\SltServer.ini");
	
	GetPrivateProfileString(title,"szAccount",NULL,m_UserName,32,filepath);
	GetPrivateProfileString(title,"szPassword",NULL,m_pwd,32,filepath);
	* m_port=GetPrivateProfileInt(title,"iGMServerPort",13334,filepath);
	* m_DstGMSID=GetPrivateProfileInt(title,"nDstGMSID",8888,filepath);
}