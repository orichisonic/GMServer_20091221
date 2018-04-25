#pragma once

#include "..\\SocketData\\SocketData.h"
#pragma comment(lib,"..\\SocketData\\debug\\SocketData.lib")

class __declspec(dllexport) CLogFile
{
public:
	//构造函数
	CLogFile(void);

	//析构函数
	~CLogFile(void);

	//写文件
    void WriteLogFile(char * pText);

	//写日志信息
	void WriteLog(char* gameName, char* m_username,char* lpText);

	//写日志信息
	void WriteText(char* gameName, char* lpText);

	//写当前时间信息
	void WriteTime();

	//在数据库中写入备份日志信息
	bool WriteBakLog(int UserByID, char* SP_Name, char* ServerIP, char* Text);

	void WriteErrorLog(char * gamename,char * lpText);
	
	//在数据库中写入日志信息
	bool WriteDBLog(int UserByID, char* GameName, char* SP_Name, char* ServerIP, char* Text);

	//读取配置文件
	bool ReadValue(char * strFile,char * strSection,char * strKey,char * strValue, int strLength);

	//读取int配置文件
	int ReadIntValue(char * strFile,char * strSection,char * strKey);
private:
	//用于在数据库中写入日志信息
	CSqlHelper m_SqlHelper;
};
