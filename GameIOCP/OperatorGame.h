// OperatorGame.h: interface for the COperatorGame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPERATORGAME_H__8E9D3118_2198_49B8_89D9_B39DBF112276__INCLUDED_)
#define AFX_OPERATORGAME_H__8E9D3118_2198_49B8_89D9_B39DBF112276__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameSession.h"

class __declspec(dllexport) COperatorGame  
{
public:
	COperatorGame();
	virtual ~COperatorGame();
	virtual bool getRemoteSql(char * GameName,char * lpFlag,char* lpSql,int iFlag);
	virtual	bool QuerySingleValue(char * GameName,char * lpIP,int flag,char* procName,char* result);
	virtual bool FindDBIP(char * GameName,char * lpIP,char * lpNewIP,int dbid);
	virtual bool BuildTLV(COperVector m_OpVect);
	virtual bool BuildTLV(char * message);
	virtual bool SendBuf(CSession * pGameSess,CEnumCore::Msg_Category m_category,CEnumCore::ServiceKey m_servicekey,unsigned char * buf,int length);
	virtual bool initialize(CSession * pSess);
	unsigned char  * GetSendBuf();
	int GetSendLength();

protected:
	char operszSql[2048];
	
	//用于保存在远程数据库执行操作的sql语句
	char operRemoteSql[2048];
	
	
	//用于构造发送数据体
	COperVector SendVectBuf;
	
	
	//用于查询本地数据库
	CSqlHelper m_SqlHelper;
	
	//保存查询得到的宠物名称
	char petname[128];
	
	//根据服务器名称获取需要的IP
	char connLocalDB[20];
	
	//完整的sql语句
	char szSql[256];
	
	//从SqlExrepss中获取的sql语句
	char m_sql[256];
	
	//读配置文件返回值
	char strMsg[2048];
	
	
	CLogFile logFile;
	char errMessage[256];
	
	//保存要进行回应的CEnumCore::ServiceKey
	CEnumCore::ServiceKey m_ServiceKey;

	int iEnbodyLength;
};

#endif // !defined(AFX_OPERATORGAME_H__8E9D3118_2198_49B8_89D9_B39DBF112276__INCLUDED_)
