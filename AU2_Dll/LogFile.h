#pragma once
#include <windows.h>
#include <stdio.h>
class CLogFile
{
public:
	CLogFile(void);
	~CLogFile(void);
	void WriteLog(char * lpText);
	FILE* fp;
};
