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

//获取SqlExpress中的相应sql语句
bool COperBan::getRemoteSql(char * lpFlag,char* lpSql,int iFlag)//iFlag=0找不到直接返回,其他不返回
{
	try
	{
		vector<CGlobalStruct::DataRecord> datarecord;
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
		sprintf(m_sql,"exec GetSql '%s' ", lpFlag);
		m_SqlHelper.SqlHelperMain("JW2",&datarecord,"127.0.0.1", m_sql, 4, 0, 0);//获取SqlExpress中的相应sql语句
		
		if(datarecord.empty())//如果记录为空
		{
			return false;
		}
		
		//获取查询的记录集的每一项
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;	
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("sql_statement", m_DataRecord.recordfieldname))//获取相应的LogDBIP
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
//获得指定语句第一个字段值
void COperBan::QuerySingleValue(char * gamename,char * lpIP,int flag,char* procName,char* result)
{
	
	vector<CGlobalStruct::DataRecord> datarecord;
	if(!datarecord.empty())
	{
		datarecord.clear();
	}
	if(!strcmp(gamename,"Pal"))
	{
		m_SqlHelper.SqlHelperMain(&datarecord, lpIP, procName, flag, 0, 0);//获取对应的LogDBIP

	}
	else
	{
		m_SqlHelper.SqlHelperMain(gamename,&datarecord, lpIP, procName, flag, 0, 0);//获取对应的LogDBIP

	}
	
	if(datarecord.empty())//如果记录为空
	{
		result="";
	}
	
	//获取查询的记录集的每一项
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

//操作数据库
int COperBan::RecordData(int iflag,int iType,int userByID,int GameID,char* ServerIP,char * ServerName,char *UserID, char * UserName, char * NickID, char * UserNick, char * beginTime, char * endTime, char * reason,char *memo)
{
	char message[30];
	memset(message, 0, 30);

	char path[256];
	GetCurrentDirectory(256,path);//获取当前目录
	char filepath[256];
	sprintf(filepath,"%s%s",path,"\\config\\scheme.ini");//BanServer.ini文件路径
	char remoteIP[40];
	GetPrivateProfileString("SERVER","RemoteIP","127.0.0.1",remoteIP,40,filepath);
	int iResult=0;
	switch(iflag)
	{
	case 1://封停
		sprintf(m_sql,"exec Game_BanUser %i,%i,%i,'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s'",iType,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,beginTime,endTime,reason,memo,remoteIP);		
		sprintf(message, "<游戏号:%d>%s", GameID, ServerName);
		OperFileLog(message, m_sql, false);	
		if(m_SqlHelper.SqlHelperMain("JW2","127.0.0.1",m_sql,4))
			iResult=1;
		break;
	case 2://解封
		sprintf(m_sql,"exec Game_UnBanUser %i,%i,%i,'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s'",iType,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,beginTime,endTime,reason,memo,remoteIP);

		sprintf(message, "<游戏号:%d>%s", GameID, ServerName);
		OperFileLog(message, m_sql, false);

		if(m_SqlHelper.SqlHelperMain("JW2","127.0.0.1",m_sql,4))
			iResult=1;
		break;
	}
	return iResult;	
}

//查找字符串中特定的字符第一次出现的位置
int COperBan::StringFind(const char* string,const char* find,int number)
{
	char* pos = (char *)string;
	char *p=pos;
	int count = 0;
	while (number > 0)
	{
		//定义查找到的字符位置的指针，以便临时指针进行遍历
		pos = strstr(p,find);
		//当位置指针为0时，说明没有找到这个字符
		if (pos == 0)
			return -1;
		//当位置指针和临时指针相等说明下一个字符就是要找的字符，如果临时指针小于位置指针，则进行遍历字符串操作，并将count增1
		while(p <= pos)
		{
			p++;
			count++;
		}
		//对要查找的次数减一
		number--;
	}
	return count;
}

//记录封停解封操作日志,　bCout 是否在控制台打印, 默认打印,　true 打印 false 不打印
void COperBan::OperFileLog(const char* gameName,char * lpText, bool bCout/*=true*/)
{
	SYSTEMTIME nowtime;
	GetLocalTime(&nowtime);//获取系统当前时间
	
	char path[256];
	GetCurrentDirectory(256,path);//获取当前运行目录
	char filepath[256];
	sprintf(filepath,"%s%s",path,"\\log\\BanOper%04i%02i%02i.log");
	char filename[256];
	sprintf(filename,filepath,nowtime.wYear,nowtime.wMonth,nowtime.wDay);//获取文件路径
	
	ofstream out;
	out.open(filename,ios::app);//打开日志文件
	if (!out) 
	{ 
		cout<<"Open File Error"<<endl; 
	}
	
	//在日志文件中写入用户名和具体内容
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