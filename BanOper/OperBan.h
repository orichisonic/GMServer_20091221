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
	bool getRemoteSql(char * lpFlag,char* lpSql,int iFlag);//iFlag=0�Ҳ���ֱ�ӷ���,����������
	//���ָ������һ���ֶ�ֵ
	void QuerySingleValue(char * gamename,char * lpIP,int flag,char* procName,char* result);
	//�������ݿ�
	int RecordData(int iflag,int iType,int userByID,int GameID,char* ServerIP,char* ServerName,char* UserID, char * UserName, char* NickID, char * UserNick,char * beginTime, char * endTime, char * reason, char *memo);

	void FindStlDBIP(char * ServerIP,char * ServerName,char * Re_ServerIP, int iflag);

	int StringFind(const char* string,const char* find,int number);

	void OperFileLog(const char* gameName,char * lpText, bool bCout=true);

	COperBan();
	virtual ~COperBan();
private:
	char m_sql[2048];
	CSqlHelper m_SqlHelper;

	//�����ַ������ض����ַ���һ�γ��ֵ�λ��
	

};

#endif // !defined(AFX_OPERBAN_H__C7645760_77D6_4F2E_8685_FD1A99F23BBF__INCLUDED_)
