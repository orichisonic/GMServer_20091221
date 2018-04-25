#pragma once

#include "..\\GameIOCP\\OperatorGame.h"
#include <afxinet.h>

class COperatorGM : public COperatorGame
{
private:

public:
	//构造函数
	COperatorGM(void);
	
	//析构函数
	~COperatorGM(void);

public:
	//查询是否vip帐号
	bool getWebInfo(char* strUrl, char* send, vector<string>& result);
	
	//获取节点属性值
	string getXMLNodeAttribute(vector<string>& strList, string nodeName);
	
	//bool BuildTLV(vector <CGlobalStruct::TFLV> DBTFLV,CEnumCore::ServiceKey m_ServiceKey);

	vector<CGlobalStruct::TFLV> ReturnOpMsg(char* operName, char* result,char *funcName);

};
