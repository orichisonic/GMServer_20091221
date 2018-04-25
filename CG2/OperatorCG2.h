#pragma once

#import "msxml2.dll"
#include <atlbase.h>
#include <vector>
#include <iostream>
using namespace MSXML2;
#include "stdio.h"
using namespace std;
#include <windows.h>
#include <wininet.h>
#include "CG2Session.h"
class COperatorCG2
{
private:
	//���ڷ�������
	CMSocket mSocket;
	
	//���ڹ��췢��������
	COperVector SendVectBuf;
	
	//���ڹ��췢�����ݰ�
//	CSocketData SendDataBuf;
	
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
	char retMessage[10][256];


	//�����ļ�
	CLogFile logFile;

	CCG2Session * pCG2Session;
	
public:
	//���캯��
	COperatorCG2(void);
	
	//��������
	~COperatorCG2(void);
	
	//��ʼ���������ݵ���
	bool initialize(CSession *pSess,SOCKET s_Socket);

	bool initialize();

	//���ص�����Ϣ
	void ReturnMessage(unsigned int nid, CEnumCore::Msg_Category m_category, CEnumCore::ServiceKey mServiceKey, char* strMessage);

	//��ȡSqlExpress�е���Ӧsql���
	void getRemoteSql(char * lpFlag,char* lpSql);

	//��ȡ���ݿ�ID
	int GetDBID(char * remoteip,char* username);
	
	void ReturnData(CEnumCore::Msg_Category m_category,CEnumCore::ServiceKey mServiceKey,unsigned char * lpdata,int length);

	//����������Ҫ���͵�����
	void ReturnData(unsigned int nid,CEnumCore::Msg_Category m_category,CEnumCore::ServiceKey mServiceKey, COperVector* m_OpVect, int SendLength);
	
	//�������ݣ������ݣ�
	void ReturnData(unsigned int nid,CEnumCore::Msg_Category m_category,CEnumCore::ServiceKey mServiceKey, COperVector* m_OpVect, COperVector* m_OpVect1,int iIndex,int iPageSize);

	//���ָ������һ���ֶ�ֵ
	void CG2_Proc_Query(char * lpIP,int flag,char* procName,char* result);

	void ReturnData(SOCKET m_socket,COperVector* m_socketdata,CEnumCore::Msg_Category m_category,CEnumCore::ServiceKey mServiceKey,int iIndex,int iPageSize);

	//��ȡ���ݿ�ID
	void FindDBIP(char * lpIP,char * lpNewIP,int dbid);

	//�����������ݣ���ɫ�ƺţ�
	void ReturnSpecData(unsigned int nid,CEnumCore::Msg_Category m_category,CEnumCore::ServiceKey mServiceKey, COperVector* m_OpVect, int SendLength);

	bool ReadValue(char * strKey,char * strValue,int strLength);

	bool ReadXml(char * strFlag,char *strUrl,char * strpostData,vector<CGlobalStruct::DataRecord> * pVect,int* iCow,int *iCol);
	bool ReadXmlGet(char * strFlag,char *strUrl,char * strpostData,vector<CGlobalStruct::DataRecord> * pVect,int* iCow,int *iCol);

	bool SendEmail(char * lpRecv,char *lpContent);
};
