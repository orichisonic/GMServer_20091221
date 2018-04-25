#include "stdafx.h"
#include ".\logfile.h"
#include <fstream> 
#include <iostream>
using namespace std;


CLogFile::CLogFile(void)
: fp(NULL)
{
}

CLogFile::~CLogFile(void)
{
}

void CLogFile::WriteTime()
{
	SYSTEMTIME nowtime;
	GetLocalTime(&nowtime);
	char filename[40];
	sprintf(filename,".\\log\\demo%i%i%i.log",nowtime.wYear,nowtime.wMonth,nowtime.wDay);
	fp=fopen(filename,"a");
	if(fp==NULL)
	{
		fp=fopen(filename,"w+");
	}
//	printf("now time:%i-%i-%i %i:%i:%i\n",nowtime.wYear,nowtime.wMonth,nowtime.wDay,nowtime.wHour,nowtime.wMinute,nowtime.wSecond);
	fprintf(fp,"now time:%i-%i-%i %i:%i:%i\n",nowtime.wYear,nowtime.wMonth,nowtime.wDay,nowtime.wHour,nowtime.wMinute,nowtime.wSecond);
	fclose(fp);
}
void CLogFile::WriteLog(char * m_username,char * lpText)
{
	//HANDLE m_Mutex;
	//m_Mutex=CreateMutex(NULL, FALSE,"WriteLog"); 
	//WaitForSingleObject(m_Mutex,INFINITE);
	SYSTEMTIME nowtime;
	GetLocalTime(&nowtime);
	char filename[40];
	sprintf(filename,"\\log\\GMServer%i%i%i.log",nowtime.wYear,nowtime.wMonth,nowtime.wDay);
	fp=fopen(filename,"a");
	if(fp==NULL)
	{
		fp=fopen(filename,"w+");
	}
//	printf("username:%s,%s",m_username,lpText);
	fprintf(fp,"username:%s,%s",m_username,lpText);
	fclose(fp);
	//CloseHandle(m_Mutex);

}
//д��־��Ϣ
void CLogFile::WriteLog(char* gameName, char* m_UserName,char * lpText)
{
	SYSTEMTIME nowtime;
	GetLocalTime(&nowtime);//��ȡϵͳ��ǰʱ��
	
	char path[256];
	GetCurrentDirectory(256,path);//��ȡ��ǰ����Ŀ¼
	char filepath[256];
	sprintf(filepath,"%s%s",path,"\\log\\GMServer%i%i%i.log");
	char filename[256];
	sprintf(filename,filepath,nowtime.wYear,nowtime.wMonth,nowtime.wDay);//��ȡ�ļ�·��
	
	ofstream out;
	out.open(filename,ios::app);//����־�ļ�
	if (!out) 
	{ 
		cout<<"Open File Error"<<endl; 
	} 
	
	//����־�ļ���д���û����;�������
// 	cout<<nowtime.wYear<<"-"<<nowtime.wMonth<<"-"<<nowtime.wDay<<" "<<nowtime.wHour<<":"<<nowtime.wMinute<<":"<<nowtime.wMinute;
// 	cout<<"---"<<gameName<<"---";
// 	cout<<"Username:"<<m_UserName<<","<<lpText<<endl;
	
	out<<nowtime.wYear<<"-"<<nowtime.wMonth<<"-"<<nowtime.wDay<<" "<<nowtime.wHour<<":"<<nowtime.wMinute<<":"<<nowtime.wMinute;
	out<<"---"<<gameName<<"---";
	out<<"UserName:"<<m_UserName<<", "<<lpText<<"\n";
	out.close();
	
}
void CLogFile::WriteText(char * lpText)
{
	//HANDLE m_Mutex;
	//m_Mutex=CreateMutex(NULL, FALSE,"WriteLog"); 
	//WaitForSingleObject(m_Mutex,INFINITE);
	
	SYSTEMTIME nowtime;
	GetLocalTime(&nowtime);
	char filename[40];
	sprintf(filename,".\\log\\demo%i%i%i.log",nowtime.wYear,nowtime.wMonth,nowtime.wDay);
	fp=fopen(filename,"a");
	if(fp==NULL)
	{
		fp=fopen(filename,"w+");
	}
//	printf("%i-%i-%i %i:%i:%i:----%s----�ɽ�OL----%s\n",nowtime.wYear,nowtime.wMonth,nowtime.wDay,nowtime.wHour,nowtime.wMinute,nowtime.wSecond,szOperator,lpText);
	fprintf(fp,"%i-%i-%i %i:%i:%i:----%s----�ɽ�OL----%s\n",nowtime.wYear,nowtime.wMonth,nowtime.wDay,nowtime.wHour,nowtime.wMinute,nowtime.wSecond,szOperator,lpText);
	fclose(fp);
	//CloseHandle(m_Mutex);

}
void CLogFile::SetOperator(char * lpOperator)
{
	sprintf(szOperator,"%s",lpOperator);
}