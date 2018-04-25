// BanServer.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "BanServer.h"
#include <process.h>


void ReadData();
static int time=0;
HANDLE m_Mutex;//读数据库
HINSTANCE dllmodule;

typedef int (_cdecl *CLOSEOPER)(int,int,int,char*,char*,char*, char*, char*, char*, char*, char*,char*,char*);
typedef int (_cdecl *OPENOPER)(int,int,int,char*,char*,char*, char*, char*, char*,char*, char*,char*,char*);
CLOSEOPER CloseOper;
OPENOPER OpenOper;

DWORD WINAPI ReadDataThread(LPVOID lpMessage)
{
//	int sleeptime=(int)lpsleeptime;
	ReadData();
	while(true)
	{
		Sleep(time);
		ReadData();
	}
	return 0;
}

void StartServer()
{
	//启动线程
	char path[256];
	GetCurrentDirectory(256,path);//获取当前目录
	char filepath[256];
	sprintf(filepath,"%s%s",path,"\\config\\BanServer.ini");//BanServer.ini文件路径
	
	time=GetPrivateProfileInt("SERVER","Time",0,filepath);//得到时间

	// ( HANDLE )_beginthreadex( 0 , 0 , ReadDataThread,  (LPVOID)time , 0 , NULL );
	CreateThread(NULL,0,ReadDataThread,NULL,0,0);
	dllmodule=LoadLibrary("BanOper.dll");
	if (dllmodule)
	{
		CloseOper=(CLOSEOPER)GetProcAddress(dllmodule,"CloseOper");//获取接口
		OpenOper=(OPENOPER)GetProcAddress(dllmodule,"OpenOper");//获取接口
		
	}
	else
	{
	}
//	m_Mutex=CreateMutex(NULL, FALSE, "BanServer"); 
}

void ReadData()
{
	char m_sql[256];
	//游戏ID
	int GameID;
	
	//服务器IP
	char ServerIP[40];
	char ServerName[256];
	char reason[256];
	
	//账号
	char UserName[256];
	
	//账号ID
	char UserID[80];
	
	//昵称
	char UserNick[256];
	
	//昵称ID
	char NickID[80];
	
	//操作者ID
	int userByID;

	char StartTime[80];
	char EndTime[80];

	char memo[80];

//	WaitForSingleObject(m_Mutex,INFINITE);

	CSqlHelper m_SqlHelper;
	

	
	vector<CGlobalStruct::DataRecord> datarecord;

	//读取封停时间账号
	printf("read Game_Ban\n");
	
	sprintf(m_sql,"exec Game_TimeClose");
	m_SqlHelper.SqlHelperMain("Ban",&datarecord,"127.0.0.1", m_sql, 4, 0, 0);//获取SqlExpress中的相应sql语句
	
	if(!datarecord.empty())//如果记录为空
	{
		//获取查询的记录集的每一项
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;	
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("userByID", m_DataRecord.recordfieldname))//获取相应的LogDBIP
			{
				userByID=StrToInt(m_DataRecord.recorddata);
			}
			else if(!strcmp("GameID", m_DataRecord.recordfieldname))
			{
				GameID=StrToInt(m_DataRecord.recorddata);
			}
			else if(!strcmp("ServerIP", m_DataRecord.recordfieldname))
			{
				sprintf(ServerIP,"%s",m_DataRecord.recorddata);
			}
			else if(!strcmp("ServerName", m_DataRecord.recordfieldname))
			{
				sprintf(ServerName,"%s",m_DataRecord.recorddata);
			}
			else if(!strcmp("UserID", m_DataRecord.recordfieldname))
			{
				sprintf(UserID,"%s",m_DataRecord.recorddata);
			}
			else if(!strcmp("UserName", m_DataRecord.recordfieldname))
			{
				sprintf(UserName,"%s",m_DataRecord.recorddata);
			}
			else if(!strcmp("NickID", m_DataRecord.recordfieldname))
			{
				sprintf(NickID,"%s",m_DataRecord.recorddata);
			}
			else if(!strcmp("UserNick", m_DataRecord.recordfieldname))
			{
				sprintf(UserNick,"%s",m_DataRecord.recorddata);
			}
			else if(!strcmp("memo", m_DataRecord.recordfieldname))
			{
				sprintf(memo,"%s",m_DataRecord.recorddata);
			}
			else if(!strcmp("reason", m_DataRecord.recordfieldname))
			{
				sprintf(reason,"%s",m_DataRecord.recorddata);
			}
			else if(!strcmp("starttime",m_DataRecord.recordfieldname))
			{
				sprintf(StartTime,"%s",m_DataRecord.recorddata);
				CloseOper(0,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,"","",reason,memo);
			}///maple add
			/*else if(!strcmp("endtime",m_DataRecord.recordfieldname))
			{
				sprintf(EndTime,"%s",m_DataRecord.recorddata);
				CloseOper(0,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,StartTime,EndTime,reason,memo);
			}*/
		}
		
		datarecord.clear();
		

	}
	//读取解封时间账号
	sprintf(m_sql,"exec Game_TimeOpen");
	m_SqlHelper.SqlHelperMain("Ban",&datarecord,"127.0.0.1", m_sql, 4, 0, 0);//获取SqlExpress中的相应sql语句
	
	if(!datarecord.empty())//如果记录为空
	{
		//获取查询的记录集的每一项
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;	
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("userByID", m_DataRecord.recordfieldname))//获取相应的LogDBIP
			{
				userByID=StrToInt(m_DataRecord.recorddata);
			}
			else if(!strcmp("GameID", m_DataRecord.recordfieldname))
			{
				GameID=StrToInt(m_DataRecord.recorddata);
			}
			else if(!strcmp("ServerIP", m_DataRecord.recordfieldname))
			{
				sprintf(ServerIP,"%s",m_DataRecord.recorddata);
			}
			else if(!strcmp("ServerName", m_DataRecord.recordfieldname))
			{
				sprintf(ServerName,"%s",m_DataRecord.recorddata);
			}
			else if(!strcmp("UserID", m_DataRecord.recordfieldname))
			{
				sprintf(UserID,"%s",m_DataRecord.recorddata);
			}
			else if(!strcmp("UserName", m_DataRecord.recordfieldname))
			{
				sprintf(UserName,"%s",m_DataRecord.recorddata);
			}
			else if(!strcmp("NickID", m_DataRecord.recordfieldname))
			{
				sprintf(NickID,"%s",m_DataRecord.recorddata);
			}
			else if(!strcmp("UserNick", m_DataRecord.recordfieldname))
			{
				sprintf(UserNick,"%s",m_DataRecord.recorddata);
			}
			else if(!strcmp("memo", m_DataRecord.recordfieldname))
			{
				sprintf(memo,"%s",m_DataRecord.recorddata);
			}
			else if(!strcmp("reason", m_DataRecord.recordfieldname))
			{
				sprintf(reason,"%s",m_DataRecord.recorddata);
			}
			else if(!strcmp("endtime",m_DataRecord.recordfieldname))
			{
				sprintf(EndTime,"%s",m_DataRecord.recorddata);
				OpenOper(0,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,"","",reason,memo);
			}//maple add
			/*else if(!strcmp("starttime",m_DataRecord.recordfieldname))
			{
				sprintf(StartTime,"%s",m_DataRecord.recorddata);
				OpenOper(0,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,StartTime,EndTime,reason,memo);
			}*/
		}
		datarecord.clear();
	}

	if(!datarecord.empty())
	{
		datarecord.clear();
	}
//	ReleaseMutex(m_Mutex);

}

