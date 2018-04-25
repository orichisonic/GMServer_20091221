#pragma once

#include "..\\GameIOCP\\OperatorGame.h"

class COperatorJW2:public COperatorGame
{
	
public:
	//���캯��
	COperatorJW2(void);
	
	//��������
	~COperatorJW2(void);

	bool BuildTLV(vector <CGlobalStruct::TFLV> DBTFLV,CEnumCore::ServiceKey m_ServiceKey,char * JW2_ServerIP);
	
	bool BuildTLV(char * message);
	
	bool ReadValue(char * strKey,char * strValue,int strLength);

	//��ȡ������־�е���Ϣ
	bool GetLogText(char* retMsg, char* baseMsg, char* diffMsg, char* Result, ...);
	//д��־��Ϣ
	bool WriteText(char* strLog);
	//ͨ��DBID��IP
	void FindDBIP(char * lpIP,char * lpNewIP,int dbid);

	//��ȡ��Ҫ���ַ���
	bool getSpecChar(const char* string, const char* find, int nNum, const char* findNext, int mNum, char* nChar);
	
	//�����ַ������ض����ַ���һ�γ��ֵ�λ��
	int StringFind(const char* string,const char* find,int number);

	//tagID��sql_type
	bool TagIDToSqlType(CEnumCore::Message_Tag_ID m_tagID,char * sql_type);

	int TagIDToDBID(CEnumCore::Message_Tag_ID m_tagID);

};
