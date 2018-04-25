#pragma once

#include "..\\GameIOCP\\OperatorGame.h"

class COperatorSDO:public COperatorGame
{
private:
/*
	//用于发送数据
	CMSocket mSocket;
	
	//用于构造发送数据体
	COperVector SendVectBuf;
	
	//用于构造发送数据包
	CSocketData SendDataBuf;
	
	//用于查询本地数据库
	CSqlHelper m_SqlHelper;

	//用于获取配置文件中的相应值
	CLogFile logFile;
	
	//保存远程数据库服务器IP
	char connRemoteDB[20];
	
	//保存本地数据库服务器IP
	char connLocalDB[20];
	
	//日志数据库名称
	char Log_DBName[64];
	
	//数据库名称
	char SDO_DBName[64];
	
	//装备数据库名称
	char Item_DBName[64];
	
	//完整的sql语句
	char szSql[1024];
	
	//从SqlExrepss中获取的sql语句
	char m_sql[1024];

	//构造返回信息
	char strMsg[2048];

	//保存要进行回应的CEnumCore::ServiceKey
	CEnumCore::ServiceKey m_ServiceKey;

	CSDOSession *pSDOSession;
*/	
public:
	//构造函数
	COperatorSDO(void);
	
	//析构函数
	~COperatorSDO(void);

	bool BuildTLV(vector <CGlobalStruct::TFLV> DBTFLV,CEnumCore::ServiceKey m_ServiceKey,char * ServerIP);

	bool BuildTLV(char * message);

	//查找字符串中特定的字符第一次出现的位置
	int StringFind(const char* string,const char* find,int number);

	//查询替换单个值
	void QuerySingleValue(char* SingleValue, char* gamename, char* ServerIP, int nFlag, const char * szFormat, ...);
	void QuerySingleValue(int* SingleValue, char* gamename, char* ServerIP, int nFlag, const char * szFormat, ...);

	//获取选定字符前面的字符串
	bool strGetChar(const char* string, const char* find, char *strRet, int retLength, int number);
	
	//获取记入日志中的信息
	bool GetLogText(char* retMsg, char* baseMsg, char* diffMsg, char* Result, ...);
	
	//写日志信息
	bool WriteText(char* gameName, char* strLog);
	
	//返回成功、失败或系统错误的信息
	vector<CGlobalStruct::TFLV> ReturnOpMsg(char* strLog, char* result);
	
	//将日志信息写入数据库
	bool WriteDBLog(int userByID, char* gameName, char* sp_Name, char* serverIP, char* strLog);
	
	//获得真实的服务器IP
	bool GetServerIP(char* DestServerIP, char* SrcServerIP, int gameDBID);
	
	//根据道具code查询道具名称
	bool GetItemName(char* ItemName, int ItemCode);
	
	//将第二个查询得到的相关内容备份到第一个数据库中
	bool BackupDataBase(char* DestGame,char* DestServerIP,int Flag,char* DestDBName,char* SrcGame,char* SrcServerIP,int nFlag,char* SqlCondition);
	
	//备份修改之前的信息
	bool BakUpdateInfo(int userByID, char* spName, char* game, char* serverIP,int Flag, char* DBName, char* QueryFlag, char* UpdateFlag, int ID1, int ID2);
	
	//备份删除之前的信息
	bool BakDeleteInfo(int userByID, char* spName, char* game, char* serverIP,int Flag, char* DBName, char* TableName, char* QueryFlag, char* InsertFlag, int ID1, int ID2);
	
//	//释放变量
//	bool Destroy();	
};
