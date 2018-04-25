#include "stdafx.h"
#include ".\logfile.h"

CLogFile::CLogFile(void)
{
}

CLogFile::~CLogFile(void)
{
}
//д�ļ�
void CLogFile::WriteLogFile(char * pText)
{
	SYSTEMTIME nowtime;
	GetLocalTime(&nowtime);//��ȡϵͳ��ǰʱ��
	
	char path[256];
	char filepath[256];
	char filename[256];
	ZeroMemory(path,256);
	ZeroMemory(filepath,256);
	ZeroMemory(filename,256);
	GetCurrentDirectory(256,path);//��ȡ��ǰ����Ŀ¼
	sprintf(filepath,"%s%s",path,"\\log\\GMServer%04i%02i%02i.log");
	sprintf(filename,filepath,nowtime.wYear,nowtime.wMonth,nowtime.wDay);//��ȡ�ļ�·��
	
	ofstream out;
	out.open(filename,ios::app);//����־�ļ�
	if (!out) 
	{ 
		cout<<"Open File Error"<<endl; 
	}
	
	//����־�ļ���д�뵱ǰ��ʱ��
	out<<nowtime.wYear<<"-"<<setfill('0')<<setw(2)<<nowtime.wMonth<<"-"
		<<setw(2)<<nowtime.wDay<<" "
		<<setw(2)<<nowtime.wHour<<":"
		<<setw(2)<<nowtime.wMinute<<":"
		<<setw(2)<<nowtime.wSecond;
	out<<" "<<pText<<"\n";
	out.close();
}
//д��ǰʱ����Ϣ
void CLogFile::WriteTime()
{
	SYSTEMTIME nowtime;
	GetLocalTime(&nowtime);//��ȡϵͳ��ǰʱ��

	char path[256];
	GetCurrentDirectory(256,path);//��ȡ��ǰ����Ŀ¼
	char filepath[256];
	sprintf(filepath,"%s%s",path,"\\log\\GMServer%04i%02i%02i.log");
	char filename[256];
	sprintf(filename,filepath,nowtime.wYear,nowtime.wMonth,nowtime.wDay);//��ȡ�ļ�·��

	ofstream out;
	out.open(filename,ios::app);//����־�ļ�
	if (!out) 
	{ 
		cout<<"Open File Error"<<endl; 
	}

	//����־�ļ���д�뵱ǰ��ʱ��
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

//д��־��Ϣ
void CLogFile::WriteLog(char* gameName, char* m_UserName,char * lpText)
{
	SYSTEMTIME nowtime;
	GetLocalTime(&nowtime);//��ȡϵͳ��ǰʱ��

	char path[256];
	GetCurrentDirectory(256,path);//��ȡ��ǰ����Ŀ¼
	char filepath[256];
	sprintf(filepath,"%s%s",path,"\\log\\GMServer%04i%02i%02i.log");
	char filename[256];
	sprintf(filename,filepath,nowtime.wYear,nowtime.wMonth,nowtime.wDay);//��ȡ�ļ�·��

	ofstream out;
	out.open(filename,ios::app);//����־�ļ�
	if (!out) 
	{ 
		cout<<"Open File Error"<<endl; 
	} 

	//����־�ļ���д���û����;�������
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

//д��־��Ϣ
void CLogFile::WriteText(char* gameName, char * lpText)
{
	SYSTEMTIME nowtime;
	GetLocalTime(&nowtime);//��ȡϵͳ��ǰʱ��

    char path[256];
	GetCurrentDirectory(256,path);//��ȡ��ǰ����Ŀ¼
	char filepath[256];
	sprintf(filepath,"%s%s",path,"\\log\\GMServer%04i%02i%02i.log");
	char filename[256];
	sprintf(filename,filepath,nowtime.wYear,nowtime.wMonth,nowtime.wDay);//��ȡ�ļ�·��

	ofstream out;
	out.open(filename,ios::app);//����־�ļ�
	if (!out) 
	{ 
		cout<<"Open File Error"<<endl; 
	} 

	//����־�ļ���д���������
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
//д��־��Ϣ
void CLogFile::WriteErrorLog(char * gameName,char * lpText)
{
	SYSTEMTIME nowtime;
	GetLocalTime(&nowtime);//��ȡϵͳ��ǰʱ��
	
    char path[256];
	GetCurrentDirectory(256,path);//��ȡ��ǰ����Ŀ¼
	char filepath[256];
	sprintf(filepath,"%s%s",path,"\\log\\GMServer%04i%02i%02i.log");
	char filename[256];
	sprintf(filename,filepath,nowtime.wYear,nowtime.wMonth,nowtime.wDay);//��ȡ�ļ�·��
	
	ofstream out;
	out.open(filename,ios::app);//����־�ļ�
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
//�����ݿ���д�뱸����־��Ϣ
bool CLogFile::WriteBakLog(int UserByID, char* SP_Name, char* ServerIP, char* Text)
{
	try
	{
		char connLocalIP[20];
		ZeroMemory(connLocalIP,20);//��ʼ���������ݿ�IP
		ReadValue("scheme","DATABASE","Address",connLocalIP,20);//��ȡ���ص����ݿ��ַ
		
		char m_Sql[1024];//����ִ�в�����sql���
		ZeroMemory(m_Sql,1024);//��ʼ��sql���
		sprintf(m_Sql,"exec SP_InsertBaklog %i,'%s','%s','%s'",UserByID,ServerIP,SP_Name,Text);
		m_SqlHelper.SqlHelperMain(connLocalIP,m_Sql,4);//�ڱ������ݿ��в��������Ϣ
	}
	catch (...)
	{
		return false;
	}
	return true;
}

//�����ݿ���д����־��Ϣ
bool CLogFile::WriteDBLog(int UserByID, char* GameName, char* SP_Name, char* ServerIP, char* Text)
{
	try
	{
		char connLocalIP[20];
		ZeroMemory(connLocalIP,20);//��ʼ���������ݿ�IP
		ReadValue("scheme","DATABASE","Address",connLocalIP,20);//��ȡ���ص����ݿ��ַ
		char m_Sql[1024];//����ִ�в�����sql���
		ZeroMemory(m_Sql,1024);//��ʼ��sql���
		sprintf(m_Sql,"exec sp_InsertGMtoolslog %i,'%s','%s','%s','%s'",UserByID,GameName,ServerIP,SP_Name,Text);
		m_SqlHelper.SqlHelperMain("JW2",connLocalIP,m_Sql,4);//�ڱ������ݿ��в��������Ϣ
	}
	catch (...)
	{
		return false;
	}
	return true;
}

//��ȡ�����ļ�
bool CLogFile::ReadValue(char * strFile,char * strSection,char * strKey,char * strValue, int strLength)
{
	try
	{
		char path[256];
		ZeroMemory(path, 256);
		GetCurrentDirectory(256,path);//��ȡ��ǰ����Ŀ¼
		
		char filepath[256];
		ZeroMemory(filepath, 256);
		wsprintf(filepath,"%s%s%s%s",path,"\\Config\\",strFile,".ini");//������õ�ini�ļ���·��
		
		ZeroMemory(strValue, strLength);
		if(GetPrivateProfileString(strSection,strKey,"",strValue,strLength,filepath)!=-1)//��ȡini�ļ��е���Ӧ��Ϣ
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

//��ȡint�����ļ�
int CLogFile::ReadIntValue(char * strFile,char * strSection,char * strKey)
{
	int retValue = 0;
	try
	{
		char path[256];
		ZeroMemory(path, 256);
		GetCurrentDirectory(256,path);//��ȡ��ǰ����Ŀ¼
		
		char filepath[256];
		ZeroMemory(filepath, 256);
		wsprintf(filepath,"%s%s%s%s",path,"\\Config\\",strFile,".ini");//������õ�ini�ļ���·��
		

		retValue = GetPrivateProfileInt(strSection,strKey,0,filepath);//��ȡini�ļ��е���Ӧ��Ϣ
	}
	catch(...)
	{
		return -1;
	}
	
	return retValue;	
}
