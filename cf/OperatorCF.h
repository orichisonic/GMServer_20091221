#pragma once

#include "..\\GameIOCP\\OperatorGame.h"
class COperatorCF : public COperatorGame
{
private:
	
public:
	//构造函数
	COperatorCF(void);
	
	//析构函数
	~COperatorCF(void);
	
	
	//获取SqlExpress中的相应sql语句
	void getRemoteSql(char * lpFlag,char* lpSql);

	
	bool GetServerIP(char* gameName, char* SrcServerIP, int gameDBID, char* DestServerIP, int* DestPort);


	bool BuildTLV(vector <CGlobalStruct::TFLV> DBTFLV,CEnumCore::ServiceKey m_ServiceKey,char * ServerIP);
	bool BuildTLV(char * message);
	
};
