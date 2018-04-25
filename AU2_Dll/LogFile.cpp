#include "stdAfx.h"
#include "logfile.h"

CLogFile::CLogFile(void)
: fp(NULL)
{
}

CLogFile::~CLogFile(void)
{
}

void CLogFile::WriteLog(char * lpText)
{
	//HANDLE m_Mutex;
	//m_Mutex=CreateMutex(NULL, FALSE, "WriteLog"); 
	//WaitForSingleObject(m_Mutex,INFINITE);
	SYSTEMTIME nowtime;
	GetLocalTime(&nowtime);
	char filename[40];
	sprintf(filename,".\\log\\AU2_%04i%02i%02i.log",nowtime.wYear,nowtime.wMonth,nowtime.wDay);
	fp=fopen(filename,"a");
	if(fp==NULL)
	{
		fp=fopen(filename,"w+");
	}
	printf("%04i-%02i-%02i %02i:%02i:%02i ",nowtime.wYear,nowtime.wMonth,nowtime.wDay,nowtime.wHour,nowtime.wMinute,nowtime.wSecond);
	fprintf(fp,"%04i-%02i-%02i %02i:%02i:%02i ",nowtime.wYear,nowtime.wMonth,nowtime.wDay,nowtime.wHour,nowtime.wMinute,nowtime.wSecond);
	printf("%s\n",lpText);
	fprintf(fp,"%s\n",lpText);	
	fclose(fp);
	//CloseHandle(m_Mutex);

}
