#pragma once
#include "..\\GameIOCP\\OperatorGame.h"

class COperatorPal:public COperatorGame
{
public:
	//���캯��
	COperatorPal(void);
	
	//��������
	~COperatorPal(void);
	
	
	//��Ԫ�����ļ�¼�е�Ӣ��˵����Ϊ����
	void TranslateSYSMEMO(char * m_result,char *m_resource);
	
	//�����ַ������ض����ַ���һ�γ��ֵ�λ��
	int StringFind(const char* string,const char* find,int number);
	
	//���ݳ������ƻ�ȡ���＼��
	void PetSkillToName(char * m_result,char *m_resource,char *petname);
	
	//��ȡ���͵ĵ�������
	void FindRewardName(char * m_result,char *m_resource);
	
	//��ȡ��������Ʒ����
	void TranslateItemType(char * m_result,char *m_resource);
	
	//��ȡ�����е��߿�װ��/ʹ��ְҵ
	void TranslateItemVoc(char * m_result,char *m_resource);
	

	//��ȡ��Ӧ��LogDBIP
	void FindLogdbIP(char * logdbip,char * servername);
	
	//��ȡ��Ӧ��GameDBIP
	void FindGameDBIP(char * gamedbip,char * servername);
	
	//��ȡ��Ӧ��AccountDBIP
	void FindAccountDBIP(char * accountdbip,char * servername);
	
	//��ȡ��ѯ��־�����ݿ�����
	void FindTableAndSql(int bigtype,int smalltype,char * strTable,char *strsql);
	
	//��ȡ��ѯ��ʷ��־�����ݿ�����
	void FindTableAndSql1(int bigtype,int smalltype,char * strTable,char *strsql);

	//��ѯ��¼��
	int FindRecordNum(char * ip,char * sql,int flag);

	//"1231012"ʱ���ʽת����"��-��-�� ʱ-��-��"
	void NumTimeToFormatTime(char * strTime,char * numTime);


	bool BuildTLV(COperVector m_OpVect,CEnumCore::ServiceKey m_ServiceKey);
	bool BuildTLV(COperVector m_OpVect,CEnumCore::ServiceKey m_ServiceKey,int PageNum);
	bool BuildTLV(char * message);
	bool BuildTLV(COperVector DBVect);
	
};
