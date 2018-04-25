#pragma once

#include "..\\GameIOCP\\OperatorGame.h"

class COperatorJW2:public COperatorGame
{
	
public:
	//构造函数
	COperatorJW2(void);
	
	//析构函数
	~COperatorJW2(void);

	bool BuildTLV(vector <CGlobalStruct::TFLV> DBTFLV,CEnumCore::ServiceKey m_ServiceKey,char * JW2_ServerIP);
	
	bool BuildTLV(char * message);
	
	bool ReadValue(char * strKey,char * strValue,int strLength);

	//获取记入日志中的信息
	bool GetLogText(char* retMsg, char* baseMsg, char* diffMsg, char* Result, ...);
	//写日志信息
	bool WriteText(char* strLog);
	//通过DBID查IP
	void FindDBIP(char * lpIP,char * lpNewIP,int dbid);

	//获取需要的字符串
	bool getSpecChar(const char* string, const char* find, int nNum, const char* findNext, int mNum, char* nChar);
	
	//查找字符串中特定的字符第一次出现的位置
	int StringFind(const char* string,const char* find,int number);

	//tagID找sql_type
	bool TagIDToSqlType(CEnumCore::Message_Tag_ID m_tagID,char * sql_type);

	int TagIDToDBID(CEnumCore::Message_Tag_ID m_tagID);

};
