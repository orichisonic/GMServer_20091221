// OperBan.h: interface for the COperBan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPERBAN_H__C7645760_77D6_4F2E_8685_FD1A99F23BBF__INCLUDED_)
#define AFX_OPERBAN_H__C7645760_77D6_4F2E_8685_FD1A99F23BBF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\\Logic\\Logic.h"
#pragma comment(lib,"..\\Logic\\debug\\Logic.lib")

class COperBan  
{
public:
	void FindDBIP(int GameID,char * ServerIP,char * ServerName,char * Re_ServerIP,int iflag);
	bool getRemoteSql(char * lpFlag,char* lpSql,int iFlag);//iFlag=0找不到直接返回,其他不返回
	//获得指定语句第一个字段值
	void QuerySingleValue(char * gamename,char * lpIP,int flag,char* procName,char* result);
	//操作数据库
	int RecordData(int iflag,int iType,int userByID,int GameID,char* ServerIP,char* ServerName,char* UserID, char * UserName, char* NickID, char * UserNick,char * beginTime, char * endTime, char * reason, char *memo);

	void FindStlDBIP(char * ServerIP,char * ServerName,char * Re_ServerIP, int iflag);

	int StringFind(const char* string,const char* find,int number);

	void OperFileLog(const char* gameName,char * lpText, bool bCout=true);

	COperBan();
	virtual ~COperBan();
private:
	char m_sql[2048];
	CSqlHelper m_SqlHelper;

	//查找字符串中特定的字符第一次出现的位置
	

};

#endif // !defined(AFX_OPERBAN_H__C7645760_77D6_4F2E_8685_FD1A99F23BBF__INCLUDED_)
