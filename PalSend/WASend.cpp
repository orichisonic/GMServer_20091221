// WASend.cpp: implementation of the CWASend class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WASend.h"
#include "SqlHelper.h"
#include <process.h>
#include "Winsock2.h"
#pragma comment(lib,"Ws2_32.lib")
#include "shlwapi.h"
#pragma comment(lib,"shlwapi.lib")
#include <stdio.h>
using namespace MSG_BSTRS;
using namespace MSG_RSTBS;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWASend::CWASend()
{
	initSocket();
}

CWASend::~CWASend()
{
	
	WSACleanup();
}

unsigned __stdcall CWASend::SendThread(void *pParam)
{
	try
	{
		int m_taskid=(int)pParam;
	//	WaitForSingleObject(m_SendMutex,INFINITE);
		CSqlHelper mysqlhelper,m_sqlhelper;
		char szSql[256];
		int interval=0,m_iWorldID=0;
		char m_szGMServerIP[40];
			
		char szData[256];
		char databaseip[20];    //本地数据库ip
		char databaseUser[20];	//本地数据库用户名
		char database[20];		//本地默认数据库
		char databasepwd[20];	//本地数据库密码
		char szContent[512];
		//////////////////读配置文件////////////////////////
			char path[256];
			GetCurrentDirectory(256,path);
			char filepath[256];
			sprintf(filepath,"%s%s",path,".\\config\\PalSend.ini");
			
			
			GetPrivateProfileString("DATABASE","Address","127.0.0.1",databaseip,20,filepath);
			GetPrivateProfileString("DATABASE","User",NULL,databaseUser,20,filepath);
			GetPrivateProfileString("DATABASE","Data",NULL,database,20,filepath);
			GetPrivateProfileString("DATABASE","Pwd",NULL,databasepwd,20,filepath);
			
			
			sprintf(szData,"Provider=SQLOLEDB.1;Persist Security Info=True;User ID=%s;Password=%s;Data Source=%s;Initial Catalog=%s",databaseUser,databasepwd,databaseip,database);
			
			
			bool m_result=mysqlhelper.ConnectData(szData);	
			
			
			if(m_result)
			{
				sprintf(szSql,"exec wa_sendbytaskid %i",m_taskid);
				m_result=mysqlhelper.DoSql(szSql);
				if(m_result)
				{
					while(!mysqlhelper.m_pRecordset->adoEOF)	
					{
						if(mysqlhelper.m_pRecordset->GetFields()->GetItem("serverIP")->Value.vt!=VT_NULL)
						{
							sprintf(m_szGMServerIP,"%s",(LPCSTR)_bstr_t(mysqlhelper.m_pRecordset->GetFields()->GetItem("ServerIP")->GetValue()));
							interval=StrToInt((LPCSTR)_bstr_t(mysqlhelper.m_pRecordset->GetFields()->GetItem("Interval")->GetValue()));
							sprintf(szContent,"%s",(LPCSTR)_bstr_t(mysqlhelper.m_pRecordset->GetFields()->GetItem("boardMessage")->GetValue()));
							SendButtle(m_szGMServerIP,szContent);
							if(interval==0)
							{
								m_result=m_sqlhelper.ConnectData(szData);
								if(m_result==true)
								{
									wsprintf(szSql,"exec wa_endatonce %i",m_taskid);
									m_sqlhelper.DoSql(szSql);
								}
								m_sqlhelper.CloseData();
							}
							
							
							
						}
						mysqlhelper.m_pRecordset->MoveNext();
					}
				}
				
			}
			mysqlhelper.CloseData();
		//	ReleaseMutex(m_SendMutex);

	}
	catch (...)
	{
		
	}
	return TRUE;
}
bool CWASend::SendButtle(char * ServerIP,char * BoardMessage)
{
	if(Connect_Remote_Login(ServerIP))
	{
	//	printf("连接成功\n");//连接成功 
		REQ_SYSTEM_BROADCAST req_system_broadcast;
		req_system_broadcast.bType=REQ_SYSTEM_BROADCAST::T_NOW;
		req_system_broadcast.AddBroadcaseTxt(BoardMessage);
		char * pSend=new char[sizeof(REQ_SYSTEM_BROADCAST)+2];
		ZeroMemory(pSend,sizeof(REQ_SYSTEM_BROADCAST)+2);
		*pSend=req_system_broadcast.GetSize();
		memcpy(pSend+2,(char *)&req_system_broadcast,sizeof(REQ_SYSTEM_BROADCAST));
	//	int iSend=send(sendSocket,pSend,sizeof(REQ_SYSTEM_BROADCAST)+2,0);
		int iSend=send(sendSocket,pSend,req_system_broadcast.GetSize()+2,0);
		delete[] pSend;
		pSend=NULL;
		REP_SYSTEM_BROADCAST rep_system_broadcast;
		char * pRecv=new char[sizeof(REP_SYSTEM_BROADCAST)+2];
		ZeroMemory(pRecv,sizeof(REP_SYSTEM_BROADCAST)+2);
		if(iSend>0)
		{
			int recvLen = recv(sendSocket,pRecv,sizeof(REP_SYSTEM_BROADCAST)+2,0);//获取封停或解封玩家帐号的请求
			if(recvLen>0)
			{
				memcpy(&rep_system_broadcast,pRecv+2, recvLen);
				if(rep_system_broadcast.eResult==1)
				{
					printf("公告%s发送成功\n",BoardMessage);
					return TRUE;
				}
			}
			else
			{
				return false;
			}
		}
		delete[] pRecv;
		pRecv=NULL;
		
	}
	else//连接失败 （写log)
	{
		//printf("远程端口连接失败\n");
	}
	return TRUE;
}
void CWASend::AddTaskid(int taskid)
{
//	vector_taskid.push_back(taskid);
//	m_taskid=taskid;
	unsigned int uiID = 0;
	_beginthreadex( 0 , 0 , SendThread,  ( void * )taskid , 0 , &uiID );
//	m_SendMutex=CreateMutex(NULL,FALSE,"WASendThread");
}
void CWASend::initSocket(void)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	
	wVersionRequested = MAKEWORD( 2, 2 );
	
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		return;
	}
	
	
	
	if ( LOBYTE( wsaData.wVersion ) != 2 ||
		HIBYTE( wsaData.wVersion ) != 2 ) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		WSACleanup( );
		return; 
	}
	
	
}

//连接远程服务端
bool CWASend::Connect_Remote_Login(char * ServerIP)
{
	try
	{

		char path[256];
		char filepath[256];
		char szAccount[40];
		char szPassword[40];
		int iPort=0;
		GetCurrentDirectory(256,path);
		sprintf(filepath,"%s%s",path,".\\config\\PalSend.ini");
		
		GetPrivateProfileString("WA","szAccount","gm0001",szAccount,40,filepath);
		GetPrivateProfileString("WA","szPassword","1",szPassword,40,filepath);
		iPort=GetPrivateProfileInt("WA","iPort",18889,filepath);


		if(sendSocket!=INVALID_SOCKET)
			closesocket(sendSocket);
		sendSocket=socket(AF_INET,SOCK_STREAM,0);
		
		struct sockaddr_in addr;


		addr.sin_family=AF_INET;
		addr.sin_port=htons(iPort);
		addr.sin_addr.s_addr=inet_addr(ServerIP);
		
	
		int bNodelay = 1; 
		int err; 
		err=setsockopt(sendSocket,IPPROTO_TCP,TCP_NODELAY,(char *)&bNodelay,sizeof(bNodelay));
		
		if(err!=NO_ERROR)
			printf("setsockopt failed\n");

		if(connect(sendSocket, (struct sockaddr*)&addr, sizeof(addr)) != 0 )
		{
			return false;
		}
		REQ_LOGIN req_login;
		ZeroMemory(req_login.loginUserName,sizeof(req_login.loginUserName));
		ZeroMemory(req_login.loginPassWord,sizeof(req_login.loginPassWord));
		sprintf(req_login.loginUserName,"%s",szAccount);
		sprintf(req_login.loginPassWord,"%s",szPassword);
		char * pSend=new char[sizeof(REQ_LOGIN)+2];
		ZeroMemory(pSend,sizeof(REQ_LOGIN)+2);
		*pSend=sizeof(REQ_LOGIN);
		memcpy(pSend+2,(char *)&req_login,sizeof(REQ_LOGIN));
		int iSend=send(sendSocket,pSend,sizeof(REQ_LOGIN)+2,0);
		delete[] pSend;
		pSend=NULL;
		REP_LOGIN rep_login;
		char * pRecv=new char[sizeof(REP_LOGIN)+2];
		ZeroMemory(pRecv,sizeof(REP_LOGIN)+2);
		if(iSend>0)
		{
			int recvLen = recv(sendSocket,pRecv,sizeof(REP_LOGIN)+2,0);//获取封停或解封玩家帐号的请求
			if(recvLen>0)
			{
				memcpy(&rep_login,pRecv+2, recvLen);
				if(rep_login.eResult==1)
				{
					printf("[IP]:%s登陆成功\n",ServerIP);
					return TRUE;
				}
			}
			else
			{
				return false;
			}
		}
		delete[] pRecv;
		pRecv=NULL;
		
	}	
	catch(...)
	{
	}
	return false;
}
