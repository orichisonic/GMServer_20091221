#pragma once
#include <stdio.h>
#include <windows.h>
class CLogFile
{
public:
	CLogFile(void);
	~CLogFile(void);
	void WriteLog(char * m_username,char * lpText);
	void WriteLog(char* gameName, char* m_UserName,char * lpText);
	void WriteText(char * lpText);
	void WriteTime();
	void SetOperator(char * lpOperator);
private:
	char szOperator[32];
	FILE* fp;
};
