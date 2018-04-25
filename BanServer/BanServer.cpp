// BanServer.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "BanServer.h"
#include <process.h>


void ReadData();
static int time=0;
HANDLE m_Mutex;//�����ݿ�
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
	//�����߳�
	char path[256];
	GetCurrentDirectory(256,path);//��ȡ��ǰĿ¼
	char filepath[256];
	sprintf(filepath,"%s%s",path,"\\config\\BanServer.ini");//BanServer.ini�ļ�·��
	
	time=GetPrivateProfileInt("SERVER","Time",0,filepath);//�õ�ʱ��

	// ( HANDLE )_beginthreadex( 0 , 0 , ReadDataThread,  (LPVOID)time , 0 , NULL );
	CreateThread(NULL,0,ReadDataThread,NULL,0,0);
	dllmodule=LoadLibrary("BanOper.dll");
	if (dllmodule)
	{
		CloseOper=(CLOSEOPER)GetProcAddress(dllmodule,"CloseOper");//��ȡ�ӿ�
		OpenOper=(OPENOPER)GetProcAddress(dllmodule,"OpenOper");//��ȡ�ӿ�
		
	}
	else
	{
	}
//	m_Mutex=CreateMutex(NULL, FALSE, "BanServer"); 
}

void ReadData()
{
	char m_sql[256];
	//��ϷID
	int GameID;
	
	//������IP
	char ServerIP[40];
	char ServerName[256];
	char reason[256];
	
	//�˺�
	char UserName[256];
	
	//�˺�ID
	char UserID[80];
	
	//�ǳ�
	char UserNick[256];
	
	//�ǳ�ID
	char NickID[80];
	
	//������ID
	int userByID;

	char StartTime[80];
	char EndTime[80];

	char memo[80];

//	WaitForSingleObject(m_Mutex,INFINITE);

	CSqlHelper m_SqlHelper;
	

	
	vector<CGlobalStruct::DataRecord> datarecord;

	//��ȡ��ͣʱ���˺�
	printf("read Game_Ban\n");
	
	sprintf(m_sql,"exec Game_TimeClose");
	m_SqlHelper.SqlHelperMain("Ban",&datarecord,"127.0.0.1", m_sql, 4, 0, 0);//��ȡSqlExpress�е���Ӧsql���
	
	if(!datarecord.empty())//�����¼Ϊ��
	{
		//��ȡ��ѯ�ļ�¼����ÿһ��
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;	
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("userByID", m_DataRecord.recordfieldname))//��ȡ��Ӧ��LogDBIP
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
	//��ȡ���ʱ���˺�
	sprintf(m_sql,"exec Game_TimeOpen");
	m_SqlHelper.SqlHelperMain("Ban",&datarecord,"127.0.0.1", m_sql, 4, 0, 0);//��ȡSqlExpress�е���Ӧsql���
	
	if(!datarecord.empty())//�����¼Ϊ��
	{
		//��ȡ��ѯ�ļ�¼����ÿһ��
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;	
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("userByID", m_DataRecord.recordfieldname))//��ȡ��Ӧ��LogDBIP
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

