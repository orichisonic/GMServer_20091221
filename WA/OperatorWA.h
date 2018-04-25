#pragma once
#include "AdminDef.h"

#include "..\\GameIOCP\\OperatorGame.h"
class COperatorWA : public COperatorGame
{
private:
	
public:
	//构造函数
	COperatorWA(void);
	
	//析构函数
	~COperatorWA(void);
	
	bool GetServerIP(char* gameName, char* SrcServerIP, int gameDBID, char* DestServerIP, int* DestPort, int &ClusterID,int &realm_id);
//	bool GetClusterID(char* gameName, char* ServerIP, int gameDBID, char* city, int& ClusterID,int& realm_id);

	
	bool BuildTLV(vector <CGlobalStruct::TFLV> DBTFLV,CEnumCore::ServiceKey m_ServiceKey,char * ServerIP);
	bool BuildTLV(char * message);
	
	void GmOperLog(int UserByID, char * lpServerIp, char *OperName, char * Content, CGlobalStruct::TFLV result);
	void InsertLocalDB(int UserByID, char * lpServerIp, char* city,char* account,char *SouStr, char * DestStr);
	//解析时间字符串转为ADMIN::DATE_TIME格式
	void ParseTime(char *time, ADMIN::DATE_TIME &tagTime);
	
	//返回成功、失败或系统错误的信息
	vector<CGlobalStruct::TFLV> ReturnOpMsg(char* operName, char* result, char *funcName);
	
};
