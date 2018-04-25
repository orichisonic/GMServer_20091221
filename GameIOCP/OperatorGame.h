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
	
	//���ڱ�����Զ�����ݿ�ִ�в�����sql���
	char operRemoteSql[2048];
	
	
	//���ڹ��췢��������
	COperVector SendVectBuf;
	
	
	//���ڲ�ѯ�������ݿ�
	CSqlHelper m_SqlHelper;
	
	//�����ѯ�õ��ĳ�������
	char petname[128];
	
	//���ݷ��������ƻ�ȡ��Ҫ��IP
	char connLocalDB[20];
	
	//������sql���
	char szSql[256];
	
	//��SqlExrepss�л�ȡ��sql���
	char m_sql[256];
	
	//�������ļ�����ֵ
	char strMsg[2048];
	
	
	CLogFile logFile;
	char errMessage[256];
	
	//����Ҫ���л�Ӧ��CEnumCore::ServiceKey
	CEnumCore::ServiceKey m_ServiceKey;

	int iEnbodyLength;
};

#endif // !defined(AFX_OPERATORGAME_H__8E9D3118_2198_49B8_89D9_B39DBF112276__INCLUDED_)
