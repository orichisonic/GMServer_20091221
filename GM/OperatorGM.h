#pragma once

#include "..\\GameIOCP\\OperatorGame.h"
#include <afxinet.h>

class COperatorGM : public COperatorGame
{
private:

public:
	//���캯��
	COperatorGM(void);
	
	//��������
	~COperatorGM(void);

public:
	//��ѯ�Ƿ�vip�ʺ�
	bool getWebInfo(char* strUrl, char* send, vector<string>& result);
	
	//��ȡ�ڵ�����ֵ
	string getXMLNodeAttribute(vector<string>& strList, string nodeName);
	
	//bool BuildTLV(vector <CGlobalStruct::TFLV> DBTFLV,CEnumCore::ServiceKey m_ServiceKey);

	vector<CGlobalStruct::TFLV> ReturnOpMsg(char* operName, char* result,char *funcName);

};
