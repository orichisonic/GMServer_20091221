#include "stdafx.h"
#include ".\logfile.h"

CLogFile::CLogFile(void)
{
}

CLogFile::~CLogFile(void)
{
}
//写文件
void CLogFile::WriteLogFile(char * pText)
{
	SYSTEMTIME nowtime;
	GetLocalTime(&nowtime);//获取系统当前时间
	
	char path[256];
	char filepath[256];
	char filename[256];
	ZeroMemory(path,256);
	ZeroMemory(filepath,256);
	ZeroMemory(filename,256);
	GetCurrentDirectory(256,path);//获取当前运行目录
	sprintf(filepath,"%s%s",path,"\\log\\GMServer%04i%02i%02i.log");
	sprintf(filename,filepath,nowtime.wYear,nowtime.wMonth,nowtime.wDay);//获取文件路径
	
	ofstream out;
	out.open(filename,ios::app);//打开日志文件
	if (!out) 
	{ 
		cout<<"Open File Error"<<endl; 
	}
	
	//在日志文件中写入当前的时间
	out<<nowtime.wYear<<"-"<<setfill('0')<<setw(2)<<nowtime.wMonth<<"-"
		<<setw(2)<<nowtime.wDay<<" "
		<<setw(2)<<nowtime.wHour<<":"
		<<setw(2)<<nowtime.wMinute<<":"
		<<setw(2)<<nowtime.wSecond;
	out<<" "<<pText<<"\n";
	out.close();
}
//写当前时间信息
void CLogFile::WriteTime()
{
	SYSTEMTIME nowtime;
	GetLocalTime(&nowtime);//获取系统当前时间

	char path[256];
	GetCurrentDirectory(256,path);//获取当前运行目录
	char filepath[256];
	sprintf(filepath,"%s%s",path,"\\log\\GMServer%04i%02i%02i.log");
	char filename[256];
	sprintf(filename,filepath,nowtime.wYear,nowtime.wMonth,nowtime.wDay);//获取文件路径

	ofstream out;
	out.open(filename,ios::app);//打开日志文件
	if (!out) 
	{ 
		cout<<"Open File Error"<<endl; 
	}

	//在日志文件中写入当前的时间
	cout<<"now time:"<<nowtime.wYear<<"-"<<setfill('0')<<setw(2)<<nowtime.wMonth<<"-"
		<<setw(2)<<nowtime.wDay<<" "
		<<setw(2)<<nowtime.wHour<<":"
		<<setw(2)<<nowtime.wMinute<<":"
		<<setw(2)<<nowtime.wSecond<<endl;
	out<<"now time:"<<nowtime.wYear<<"-"<<setfill('0')<<setw(2)<<nowtime.wMonth<<"-"
		<<setw(2)<<nowtime.wDay<<" "
		<<setw(2)<<nowtime.wHour<<":"
		<<setw(2)<<nowtime.wMinute<<":"
		<<setw(2)<<nowtime.wSecond<<"\n";
	out.close();
}

//写日志信息
void CLogFile::WriteLog(char* gameName, char* m_UserName,char * lpText)
{
	SYSTEMTIME nowtime;
	GetLocalTime(&nowtime);//获取系统当前时间

	char path[256];
	GetCurrentDirectory(256,path);//获取当前运行目录
	char filepath[256];
	sprintf(filepath,"%s%s",path,"\\log\\GMServer%04i%02i%02i.log");
	char filename[256];
	sprintf(filename,filepath,nowtime.wYear,nowtime.wMonth,nowtime.wDay);//获取文件路径

	ofstream out;
	out.open(filename,ios::app);//打开日志文件
	if (!out) 
	{ 
		cout<<"Open File Error"<<endl; 
	} 

	//在日志文件中写入用户名和具体内容
	cout<<nowtime.wYear<<"-"<<setfill('0')<<setw(2)<<nowtime.wMonth<<"-"
		<<setw(2)<<nowtime.wDay<<" "
		<<setw(2)<<nowtime.wHour<<":"
		<<setw(2)<<nowtime.wMinute<<":"
		<<setw(2)<<nowtime.wSecond;
	cout<<"---"<<gameName<<"---";
	cout<<"Username:"<<m_UserName<<","<<lpText<<endl;

	out<<nowtime.wYear<<"-"<<setfill('0')<<setw(2)<<nowtime.wMonth<<"-"
		<<setw(2)<<nowtime.wDay<<" "
		<<setw(2)<<nowtime.wHour<<":"
		<<setw(2)<<nowtime.wMinute<<":"
		<<setw(2)<<nowtime.wSecond;
	out<<"---"<<gameName<<"---";
	out<<"UserName:"<<m_UserName<<", "<<lpText<<"\n";
	out.close();

}

//写日志信息
void CLogFile::WriteText(char* gameName, char * lpText)
{
	SYSTEMTIME nowtime;
	GetLocalTime(&nowtime);//获取系统当前时间

    char path[256];
	GetCurrentDirectory(256,path);//获取当前运行目录
	char filepath[256];
	sprintf(filepath,"%s%s",path,"\\log\\GMServer%04i%02i%02i.log");
	char filename[256];
	sprintf(filename,filepath,nowtime.wYear,nowtime.wMonth,nowtime.wDay);//获取文件路径

	ofstream out;
	out.open(filename,ios::app);//打开日志文件
	if (!out) 
	{ 
		cout<<"Open File Error"<<endl; 
	} 

	//在日志文件中写入具体内容
	cout<<nowtime.wYear<<"-"<<setfill('0')<<setw(2)<<nowtime.wMonth<<"-"
		<<setw(2)<<nowtime.wDay<<" "
		<<setw(2)<<nowtime.wHour<<":"
		<<setw(2)<<nowtime.wMinute<<":"
		<<setw(2)<<nowtime.wSecond;
	cout<<"---"<<gameName<<"---";
	cout<<lpText<<endl;

	out<<nowtime.wYear<<"-"<<setfill('0')<<setw(2)<<nowtime.wMonth<<"-"
		<<setw(2)<<nowtime.wDay<<" "
		<<setw(2)<<nowtime.wHour<<":"
		<<setw(2)<<nowtime.wMinute<<":"
		<<setw(2)<<nowtime.wSecond;
	out<<"---"<<gameName<<"---";
	out<<lpText<<"\n";
	out.close();
}
//写日志信息
void CLogFile::WriteErrorLog(char * gameName,char * lpText)
{
	SYSTEMTIME nowtime;
	GetLocalTime(&nowtime);//获取系统当前时间
	
    char path[256];
	GetCurrentDirectory(256,path);//获取当前运行目录
	char filepath[256];
	sprintf(filepath,"%s%s",path,"\\log\\GMServer%04i%02i%02i.log");
	char filename[256];
	sprintf(filename,filepath,nowtime.wYear,nowtime.wMonth,nowtime.wDay);//获取文件路径
	
	ofstream out;
	out.open(filename,ios::app);//打开日志文件
	if (!out) 
	{ 
		cout<<"Open File Error"<<endl; 
	} 
	
	out<<nowtime.wYear<<"-"<<setfill('0')<<setw(2)<<nowtime.wMonth<<"-"
		<<setw(2)<<nowtime.wDay<<" "
		<<setw(2)<<nowtime.wHour<<":"
		<<setw(2)<<nowtime.wMinute<<":"
		<<setw(2)<<nowtime.wSecond;
	out<<"---"<<gameName<<"---";
	out<<lpText<<"\n";
	out.close();
}
//在数据库中写入备份日志信息
bool CLogFile::WriteBakLog(int UserByID, char* SP_Name, char* ServerIP, char* Text)
{
	try
	{
		char connLocalIP[20];
		ZeroMemory(connLocalIP,20);//初始化本地数据库IP
		ReadValue("scheme","DATABASE","Address",connLocalIP,20);//获取本地的数据库地址
		
		char m_Sql[1024];//用于执行操作的sql语句
		ZeroMemory(m_Sql,1024);//初始化sql语句
		sprintf(m_Sql,"exec SP_InsertBaklog %i,'%s','%s','%s'",UserByID,ServerIP,SP_Name,Text);
		m_SqlHelper.SqlHelperMain(connLocalIP,m_Sql,4);//在本地数据库中插入操作信息
	}
	catch (...)
	{
		return false;
	}
	return true;
}

//在数据库中写入日志信息
bool CLogFile::WriteDBLog(int UserByID, char* GameName, char* SP_Name, char* ServerIP, char* Text)
{
	try
	{
		char connLocalIP[20];
		ZeroMemory(connLocalIP,20);//初始化本地数据库IP
		ReadValue("scheme","DATABASE","Address",connLocalIP,20);//获取本地的数据库地址
		char m_Sql[1024];//用于执行操作的sql语句
		ZeroMemory(m_Sql,1024);//初始化sql语句
		sprintf(m_Sql,"exec sp_InsertGMtoolslog %i,'%s','%s','%s','%s'",UserByID,GameName,ServerIP,SP_Name,Text);
		m_SqlHelper.SqlHelperMain("JW2",connLocalIP,m_Sql,4);//在本地数据库中插入操作信息
	}
	catch (...)
	{
		return false;
	}
	return true;
}

//读取配置文件
bool CLogFile::ReadValue(char * strFile,char * strSection,char * strKey,char * strValue, int strLength)
{
	try
	{
		char path[256];
		ZeroMemory(path, 256);
		GetCurrentDirectory(256,path);//获取当前运行目录
		
		char filepath[256];
		ZeroMemory(filepath, 256);
		wsprintf(filepath,"%s%s%s%s",path,"\\Config\\",strFile,".ini");//构造对用的ini文件的路径
		
		ZeroMemory(strValue, strLength);
		if(GetPrivateProfileString(strSection,strKey,"",strValue,strLength,filepath)!=-1)//读取ini文件中的相应信息
		{
			return true;
		}

	}
	catch(...)
	{
		return false;
	}
	
	return true;

}

//读取int配置文件
int CLogFile::ReadIntValue(char * strFile,char * strSection,char * strKey)
{
	int retValue = 0;
	try
	{
		char path[256];
		ZeroMemory(path, 256);
		GetCurrentDirectory(256,path);//获取当前运行目录
		
		char filepath[256];
		ZeroMemory(filepath, 256);
		wsprintf(filepath,"%s%s%s%s",path,"\\Config\\",strFile,".ini");//构造对用的ini文件的路径
		

		retValue = GetPrivateProfileInt(strSection,strKey,0,filepath);//读取ini文件中的相应信息
	}
	catch(...)
	{
		return -1;
	}
	
	return retValue;	
}
