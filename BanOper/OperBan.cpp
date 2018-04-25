// OperBan.cpp: implementation of the COperBan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OperBan.h"
#include <fstream> 
#include <iostream>
#include <iomanip>

using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COperBan::COperBan()
{

}

COperBan::~COperBan()
{

}

void COperBan::FindDBIP(int GameID, char* ServerIP,char * ServerName,char* Re_ServerIP,int iflag)
{
	sprintf(m_sql,"exec FindDBIP %i,'%s',%i",GameID,ServerName,iflag);
	QuerySingleValue("JW2","127.0.0.1",4,m_sql,Re_ServerIP);
}

//maple add
void COperBan::FindStlDBIP(char * ServerIP,char * ServerName,char * Re_ServerIP, int iflag)
{
	sprintf(m_sql,"exec JW2_FindDBIP '%s',%i",ServerIP,iflag);
	QuerySingleValue("JW2","127.0.0.1",4,m_sql,Re_ServerIP);
}

//��ȡSqlExpress�е���Ӧsql���
bool COperBan::getRemoteSql(char * lpFlag,char* lpSql,int iFlag)//iFlag=0�Ҳ���ֱ�ӷ���,����������
{
	try
	{
		vector<CGlobalStruct::DataRecord> datarecord;
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
		sprintf(m_sql,"exec GetSql '%s' ", lpFlag);
		m_SqlHelper.SqlHelperMain("JW2",&datarecord,"127.0.0.1", m_sql, 4, 0, 0);//��ȡSqlExpress�е���Ӧsql���
		
		if(datarecord.empty())//�����¼Ϊ��
		{
			return false;
		}
		
		//��ȡ��ѯ�ļ�¼����ÿһ��
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;	
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("sql_statement", m_DataRecord.recordfieldname))//��ȡ��Ӧ��LogDBIP
			{
				sprintf(lpSql, "%s", m_DataRecord.recorddata);
			}
		}
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
		
	}
	catch(...)
	{
		sprintf(lpSql,"");
		return false;
	}
	return true;
}
//���ָ������һ���ֶ�ֵ
void COperBan::QuerySingleValue(char * gamename,char * lpIP,int flag,char* procName,char* result)
{
	
	vector<CGlobalStruct::DataRecord> datarecord;
	if(!datarecord.empty())
	{
		datarecord.clear();
	}
	if(!strcmp(gamename,"Pal"))
	{
		m_SqlHelper.SqlHelperMain(&datarecord, lpIP, procName, flag, 0, 0);//��ȡ��Ӧ��LogDBIP

	}
	else
	{
		m_SqlHelper.SqlHelperMain(gamename,&datarecord, lpIP, procName, flag, 0, 0);//��ȡ��Ӧ��LogDBIP

	}
	
	if(datarecord.empty())//�����¼Ϊ��
	{
		result="";
	}
	
	//��ȡ��ѯ�ļ�¼����ÿһ��
	vector<CGlobalStruct::DataRecord>::iterator iter;
	CGlobalStruct::DataRecord  m_DataRecord;	
	for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
	{
		m_DataRecord = *iter;
		sprintf(result,"%s",m_DataRecord.recorddata);
		break;
	}
	if(!datarecord.empty())
	{
		datarecord.clear();
	}
}

//�������ݿ�
int COperBan::RecordData(int iflag,int iType,int userByID,int GameID,char* ServerIP,char * ServerName,char *UserID, char * UserName, char * NickID, char * UserNick, char * beginTime, char * endTime, char * reason,char *memo)
{
	char message[30];
	memset(message, 0, 30);

	char path[256];
	GetCurrentDirectory(256,path);//��ȡ��ǰĿ¼
	char filepath[256];
	sprintf(filepath,"%s%s",path,"\\config\\scheme.ini");//BanServer.ini�ļ�·��
	char remoteIP[40];
	GetPrivateProfileString("SERVER","RemoteIP","127.0.0.1",remoteIP,40,filepath);
	int iResult=0;
	switch(iflag)
	{
	case 1://��ͣ
		sprintf(m_sql,"exec Game_BanUser %i,%i,%i,'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s'",iType,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,beginTime,endTime,reason,memo,remoteIP);		
		sprintf(message, "<��Ϸ��:%d>%s", GameID, ServerName);
		OperFileLog(message, m_sql, false);	
		if(m_SqlHelper.SqlHelperMain("JW2","127.0.0.1",m_sql,4))
			iResult=1;
		break;
	case 2://���
		sprintf(m_sql,"exec Game_UnBanUser %i,%i,%i,'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s'",iType,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,beginTime,endTime,reason,memo,remoteIP);

		sprintf(message, "<��Ϸ��:%d>%s", GameID, ServerName);
		OperFileLog(message, m_sql, false);

		if(m_SqlHelper.SqlHelperMain("JW2","127.0.0.1",m_sql,4))
			iResult=1;
		break;
	}
	return iResult;	
}

//�����ַ������ض����ַ���һ�γ��ֵ�λ��
int COperBan::StringFind(const char* string,const char* find,int number)
{
	char* pos = (char *)string;
	char *p=pos;
	int count = 0;
	while (number > 0)
	{
		//������ҵ����ַ�λ�õ�ָ�룬�Ա���ʱָ����б���
		pos = strstr(p,find);
		//��λ��ָ��Ϊ0ʱ��˵��û���ҵ�����ַ�
		if (pos == 0)
			return -1;
		//��λ��ָ�����ʱָ�����˵����һ���ַ�����Ҫ�ҵ��ַ��������ʱָ��С��λ��ָ�룬����б����ַ�������������count��1
		while(p <= pos)
		{
			p++;
			count++;
		}
		//��Ҫ���ҵĴ�����һ
		number--;
	}
	return count;
}

//��¼��ͣ��������־,��bCout �Ƿ��ڿ���̨��ӡ, Ĭ�ϴ�ӡ,��true ��ӡ false ����ӡ
void COperBan::OperFileLog(const char* gameName,char * lpText, bool bCout/*=true*/)
{
	SYSTEMTIME nowtime;
	GetLocalTime(&nowtime);//��ȡϵͳ��ǰʱ��
	
	char path[256];
	GetCurrentDirectory(256,path);//��ȡ��ǰ����Ŀ¼
	char filepath[256];
	sprintf(filepath,"%s%s",path,"\\log\\BanOper%04i%02i%02i.log");
	char filename[256];
	sprintf(filename,filepath,nowtime.wYear,nowtime.wMonth,nowtime.wDay);//��ȡ�ļ�·��
	
	ofstream out;
	out.open(filename,ios::app);//����־�ļ�
	if (!out) 
	{ 
		cout<<"Open File Error"<<endl; 
	}
	
	//����־�ļ���д���û����;�������
	if (bCout)
	{
		cout<<nowtime.wYear<<"-"<<setfill('0')<<setw(2)<<nowtime.wMonth<<"-"
			<<setw(2)<<nowtime.wDay<<" "
			<<setw(2)<<nowtime.wHour<<":"
			<<setw(2)<<nowtime.wMinute<<":"
			<<setw(2)<<nowtime.wMinute;
		cout<<"---"<<gameName<<"---";
		cout<<lpText<<endl;
	}

	out<<nowtime.wYear<<"-"<<setfill('0')<<setw(2)<<nowtime.wMonth<<"-"
		<<setw(2)<<nowtime.wDay<<" "
		<<setw(2)<<nowtime.wHour<<":"
		<<setw(2)<<nowtime.wMinute<<":"
		<<setw(2)<<nowtime.wMinute;
	out<<"---"<<gameName<<"---";
	out<<lpText<<"\n";
	out.close();
}