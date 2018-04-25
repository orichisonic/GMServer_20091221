#pragma once

#include "..\\GameIOCP\\OperatorGame.h"
class COperatorGT : public COperatorGame
{
private:
	//完整的sql语句
	char szSql[1024];
	
	//从SqlExrepss中获取的sql语句
	char m_sql[1024];
		
public:
	//构造函数
	COperatorGT(void);
	
	//析构函数
	~COperatorGT(void);
	
	
	//查询替换单个值
	void ReplaceSingleValue(char* SingleValue, char* gamename, char* ServerIP, int nFlag, const char * szFormat, ...);

	//获取开启宝箱的道具奖励
	void GetTreasReward(char* RewardName,char* ItemList,char * ServerIP);

	//查找字符串中特定的字符第一次出现的位置
	int StringFind(const char* string,const char* find,int number);

	bool TagIDToSqlType(CEnumCore::Message_Tag_ID m_tagID,char * sql_type);
	bool TagIDToSqlType(CEnumCore::Message_Tag_ID m_tagID,int type,char * sql_type);

	bool BuildTLV(COperVector m_OpVect,CEnumCore::ServiceKey m_ServiceKey,char * ServerIP);
	bool BuildTLV(char * message);
	
};
