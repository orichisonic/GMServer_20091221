#pragma once
#include "..\\GameIOCP\\OperatorGame.h"

class COperatorPal:public COperatorGame
{
public:
	//构造函数
	COperatorPal(void);
	
	//析构函数
	~COperatorPal(void);
	
	
	//将元宝消耗记录中的英文说明改为中文
	void TranslateSYSMEMO(char * m_result,char *m_resource);
	
	//查找字符串中特定的字符第一次出现的位置
	int StringFind(const char* string,const char* find,int number);
	
	//根据宠物名称获取宠物技能
	void PetSkillToName(char * m_result,char *m_resource,char *petname);
	
	//获取发送的道具名称
	void FindRewardName(char * m_result,char *m_resource);
	
	//获取寄卖行物品类型
	void TranslateItemType(char * m_result,char *m_resource);
	
	//获取寄卖行道具可装备/使用职业
	void TranslateItemVoc(char * m_result,char *m_resource);
	

	//获取对应的LogDBIP
	void FindLogdbIP(char * logdbip,char * servername);
	
	//获取对应的GameDBIP
	void FindGameDBIP(char * gamedbip,char * servername);
	
	//获取对应的AccountDBIP
	void FindAccountDBIP(char * accountdbip,char * servername);
	
	//获取查询日志的数据库名称
	void FindTableAndSql(int bigtype,int smalltype,char * strTable,char *strsql);
	
	//获取查询历史日志的数据库名称
	void FindTableAndSql1(int bigtype,int smalltype,char * strTable,char *strsql);

	//查询记录数
	int FindRecordNum(char * ip,char * sql,int flag);

	//"1231012"时间格式转换成"年-月-日 时-分-秒"
	void NumTimeToFormatTime(char * strTime,char * numTime);


	bool BuildTLV(COperVector m_OpVect,CEnumCore::ServiceKey m_ServiceKey);
	bool BuildTLV(COperVector m_OpVect,CEnumCore::ServiceKey m_ServiceKey,int PageNum);
	bool BuildTLV(char * message);
	bool BuildTLV(COperVector DBVect);
	
};
