#pragma once

#include "..\\GameIOCP\\OperatorGame.h"
class COperatorGT : public COperatorGame
{
private:
	//������sql���
	char szSql[1024];
	
	//��SqlExrepss�л�ȡ��sql���
	char m_sql[1024];
		
public:
	//���캯��
	COperatorGT(void);
	
	//��������
	~COperatorGT(void);
	
	
	//��ѯ�滻����ֵ
	void ReplaceSingleValue(char* SingleValue, char* gamename, char* ServerIP, int nFlag, const char * szFormat, ...);

	//��ȡ��������ĵ��߽���
	void GetTreasReward(char* RewardName,char* ItemList,char * ServerIP);

	//�����ַ������ض����ַ���һ�γ��ֵ�λ��
	int StringFind(const char* string,const char* find,int number);

	bool TagIDToSqlType(CEnumCore::Message_Tag_ID m_tagID,char * sql_type);
	bool TagIDToSqlType(CEnumCore::Message_Tag_ID m_tagID,int type,char * sql_type);

	bool BuildTLV(COperVector m_OpVect,CEnumCore::ServiceKey m_ServiceKey,char * ServerIP);
	bool BuildTLV(char * message);
	
};
