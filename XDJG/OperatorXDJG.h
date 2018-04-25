#pragma once

#include "..\\GameIOCP\\OperatorGame.h"

// #import "msxml2.dll"
// using namespace MSXML2;

typedef struct
{
	string label;
	string content;
}colInfo;

class COperatorXDJG : public COperatorGame
{
public:
	//构造函数
	COperatorXDJG(void);
	
	//析构函数
	~COperatorXDJG(void);

	//通过网络接口获取信息
//	bool getXmlInfo(char *resq, char *strUrl,char * strpostData,vector<CGlobalStruct::DataRecord> * pVect,int iIndex, int iPageSize);

	//通过web service返回结果集信息
	bool getWebInfo(char *strUrl, vector<string> &result);

	//通过web service返回操作结果
	bool getWebInfo(char *strUrl, string &result);

	//更加翻页返回请求指定记录字段
	bool getPageVector(vector<string> &newDataVect, int &iPageCount, vector<string> DataVect, int iIndex, int iPageSize, int iCol);

	//test
	//更加翻页返回请求指定记录字段
	bool getPageVector(vector<colInfo> &newDataVect, int &iPageCount, vector<string> DataVect, int iIndex, int iPageSize, int iCol);

	//解析字符串中的标识和内容
	void splitString(const string strSrc, string &strLabel, string &strContent);

	//返回成功、失败或系统错误的信息
	vector<CGlobalStruct::TFLV> ReturnOpMsg(char* operName, char* result, char *funcName);

	bool GetServerIP(char* DestServerIP, char* SrcServerIP, int gameDBID);
 	
	bool BuildTLV(vector <CGlobalStruct::TFLV> DBTFLV,CEnumCore::ServiceKey m_ServiceKey,char * ServerIP);
	bool BuildTLV(char * message);

	void GmOperLog(int UserByID, char * lpServerIp, char *OperName, char * Content, CGlobalStruct::TFLV result);
};
