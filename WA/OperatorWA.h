#pragma once
#include "AdminDef.h"

#include "..\\GameIOCP\\OperatorGame.h"
class COperatorWA : public COperatorGame
{
private:
	
public:
	//���캯��
	COperatorWA(void);
	
	//��������
	~COperatorWA(void);
	
	bool GetServerIP(char* gameName, char* SrcServerIP, int gameDBID, char* DestServerIP, int* DestPort, int &ClusterID,int &realm_id);
//	bool GetClusterID(char* gameName, char* ServerIP, int gameDBID, char* city, int& ClusterID,int& realm_id);

	
	bool BuildTLV(vector <CGlobalStruct::TFLV> DBTFLV,CEnumCore::ServiceKey m_ServiceKey,char * ServerIP);
	bool BuildTLV(char * message);
	
	void GmOperLog(int UserByID, char * lpServerIp, char *OperName, char * Content, CGlobalStruct::TFLV result);
	void InsertLocalDB(int UserByID, char * lpServerIp, char* city,char* account,char *SouStr, char * DestStr);
	//����ʱ���ַ���תΪADMIN::DATE_TIME��ʽ
	void ParseTime(char *time, ADMIN::DATE_TIME &tagTime);
	
	//���سɹ���ʧ�ܻ�ϵͳ�������Ϣ
	vector<CGlobalStruct::TFLV> ReturnOpMsg(char* operName, char* result, char *funcName);
	
};
