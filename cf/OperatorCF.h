#pragma once

#include "..\\GameIOCP\\OperatorGame.h"
class COperatorCF : public COperatorGame
{
private:
	
public:
	//���캯��
	COperatorCF(void);
	
	//��������
	~COperatorCF(void);
	
	
	//��ȡSqlExpress�е���Ӧsql���
	void getRemoteSql(char * lpFlag,char* lpSql);

	
	bool GetServerIP(char* gameName, char* SrcServerIP, int gameDBID, char* DestServerIP, int* DestPort);


	bool BuildTLV(vector <CGlobalStruct::TFLV> DBTFLV,CEnumCore::ServiceKey m_ServiceKey,char * ServerIP);
	bool BuildTLV(char * message);
	
};
