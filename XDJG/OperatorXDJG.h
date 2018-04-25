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
	//���캯��
	COperatorXDJG(void);
	
	//��������
	~COperatorXDJG(void);

	//ͨ������ӿڻ�ȡ��Ϣ
//	bool getXmlInfo(char *resq, char *strUrl,char * strpostData,vector<CGlobalStruct::DataRecord> * pVect,int iIndex, int iPageSize);

	//ͨ��web service���ؽ������Ϣ
	bool getWebInfo(char *strUrl, vector<string> &result);

	//ͨ��web service���ز������
	bool getWebInfo(char *strUrl, string &result);

	//���ӷ�ҳ��������ָ����¼�ֶ�
	bool getPageVector(vector<string> &newDataVect, int &iPageCount, vector<string> DataVect, int iIndex, int iPageSize, int iCol);

	//test
	//���ӷ�ҳ��������ָ����¼�ֶ�
	bool getPageVector(vector<colInfo> &newDataVect, int &iPageCount, vector<string> DataVect, int iIndex, int iPageSize, int iCol);

	//�����ַ����еı�ʶ������
	void splitString(const string strSrc, string &strLabel, string &strContent);

	//���سɹ���ʧ�ܻ�ϵͳ�������Ϣ
	vector<CGlobalStruct::TFLV> ReturnOpMsg(char* operName, char* result, char *funcName);

	bool GetServerIP(char* DestServerIP, char* SrcServerIP, int gameDBID);
 	
	bool BuildTLV(vector <CGlobalStruct::TFLV> DBTFLV,CEnumCore::ServiceKey m_ServiceKey,char * ServerIP);
	bool BuildTLV(char * message);

	void GmOperLog(int UserByID, char * lpServerIp, char *OperName, char * Content, CGlobalStruct::TFLV result);
};
