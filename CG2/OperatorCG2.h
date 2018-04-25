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
	//用于发送数据
	CMSocket mSocket;
	
	//用于构造发送数据体
	COperVector SendVectBuf;
	
	//用于构造发送数据包
//	CSocketData SendDataBuf;
	
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
	char retMessage[10][256];


	//配置文件
	CLogFile logFile;

	CCG2Session * pCG2Session;
	
public:
	//构造函数
	COperatorCG2(void);
	
	//析构函数
	~COperatorCG2(void);
	
	//初始化发送数据的类
	bool initialize(CSession *pSess,SOCKET s_Socket);

	bool initialize();

	//返回单个信息
	void ReturnMessage(unsigned int nid, CEnumCore::Msg_Category m_category, CEnumCore::ServiceKey mServiceKey, char* strMessage);

	//获取SqlExpress中的相应sql语句
	void getRemoteSql(char * lpFlag,char* lpSql);

	//获取数据库ID
	int GetDBID(char * remoteip,char* username);
	
	void ReturnData(CEnumCore::Msg_Category m_category,CEnumCore::ServiceKey mServiceKey,unsigned char * lpdata,int length);

	//解析并返回要发送的数据
	void ReturnData(unsigned int nid,CEnumCore::Msg_Category m_category,CEnumCore::ServiceKey mServiceKey, COperVector* m_OpVect, int SendLength);
	
	//返回数据（并数据）
	void ReturnData(unsigned int nid,CEnumCore::Msg_Category m_category,CEnumCore::ServiceKey mServiceKey, COperVector* m_OpVect, COperVector* m_OpVect1,int iIndex,int iPageSize);

	//获得指定语句第一个字段值
	void CG2_Proc_Query(char * lpIP,int flag,char* procName,char* result);

	void ReturnData(SOCKET m_socket,COperVector* m_socketdata,CEnumCore::Msg_Category m_category,CEnumCore::ServiceKey mServiceKey,int iIndex,int iPageSize);

	//获取数据库ID
	void FindDBIP(char * lpIP,char * lpNewIP,int dbid);

	//返回特殊数据（角色称号）
	void ReturnSpecData(unsigned int nid,CEnumCore::Msg_Category m_category,CEnumCore::ServiceKey mServiceKey, COperVector* m_OpVect, int SendLength);

	bool ReadValue(char * strKey,char * strValue,int strLength);

	bool ReadXml(char * strFlag,char *strUrl,char * strpostData,vector<CGlobalStruct::DataRecord> * pVect,int* iCow,int *iCol);
	bool ReadXmlGet(char * strFlag,char *strUrl,char * strpostData,vector<CGlobalStruct::DataRecord> * pVect,int* iCow,int *iCol);

	bool SendEmail(char * lpRecv,char *lpContent);
};
