#pragma once

#include "..\\GameIOCP\\OperatorGame.h"

class COperatorSDO:public COperatorGame
{
private:
/*
	//���ڷ�������
	CMSocket mSocket;
	
	//���ڹ��췢��������
	COperVector SendVectBuf;
	
	//���ڹ��췢�����ݰ�
	CSocketData SendDataBuf;
	
	//���ڲ�ѯ�������ݿ�
	CSqlHelper m_SqlHelper;

	//���ڻ�ȡ�����ļ��е���Ӧֵ
	CLogFile logFile;
	
	//����Զ�����ݿ������IP
	char connRemoteDB[20];
	
	//���汾�����ݿ������IP
	char connLocalDB[20];
	
	//��־���ݿ�����
	char Log_DBName[64];
	
	//���ݿ�����
	char SDO_DBName[64];
	
	//װ�����ݿ�����
	char Item_DBName[64];
	
	//������sql���
	char szSql[1024];
	
	//��SqlExrepss�л�ȡ��sql���
	char m_sql[1024];

	//���췵����Ϣ
	char strMsg[2048];

	//����Ҫ���л�Ӧ��CEnumCore::ServiceKey
	CEnumCore::ServiceKey m_ServiceKey;

	CSDOSession *pSDOSession;
*/	
public:
	//���캯��
	COperatorSDO(void);
	
	//��������
	~COperatorSDO(void);

	bool BuildTLV(vector <CGlobalStruct::TFLV> DBTFLV,CEnumCore::ServiceKey m_ServiceKey,char * ServerIP);

	bool BuildTLV(char * message);

	//�����ַ������ض����ַ���һ�γ��ֵ�λ��
	int StringFind(const char* string,const char* find,int number);

	//��ѯ�滻����ֵ
	void QuerySingleValue(char* SingleValue, char* gamename, char* ServerIP, int nFlag, const char * szFormat, ...);
	void QuerySingleValue(int* SingleValue, char* gamename, char* ServerIP, int nFlag, const char * szFormat, ...);

	//��ȡѡ���ַ�ǰ����ַ���
	bool strGetChar(const char* string, const char* find, char *strRet, int retLength, int number);
	
	//��ȡ������־�е���Ϣ
	bool GetLogText(char* retMsg, char* baseMsg, char* diffMsg, char* Result, ...);
	
	//д��־��Ϣ
	bool WriteText(char* gameName, char* strLog);
	
	//���سɹ���ʧ�ܻ�ϵͳ�������Ϣ
	vector<CGlobalStruct::TFLV> ReturnOpMsg(char* strLog, char* result);
	
	//����־��Ϣд�����ݿ�
	bool WriteDBLog(int userByID, char* gameName, char* sp_Name, char* serverIP, char* strLog);
	
	//�����ʵ�ķ�����IP
	bool GetServerIP(char* DestServerIP, char* SrcServerIP, int gameDBID);
	
	//���ݵ���code��ѯ��������
	bool GetItemName(char* ItemName, int ItemCode);
	
	//���ڶ�����ѯ�õ���������ݱ��ݵ���һ�����ݿ���
	bool BackupDataBase(char* DestGame,char* DestServerIP,int Flag,char* DestDBName,char* SrcGame,char* SrcServerIP,int nFlag,char* SqlCondition);
	
	//�����޸�֮ǰ����Ϣ
	bool BakUpdateInfo(int userByID, char* spName, char* game, char* serverIP,int Flag, char* DBName, char* QueryFlag, char* UpdateFlag, int ID1, int ID2);
	
	//����ɾ��֮ǰ����Ϣ
	bool BakDeleteInfo(int userByID, char* spName, char* game, char* serverIP,int Flag, char* DBName, char* TableName, char* QueryFlag, char* InsertFlag, int ID1, int ID2);
	
//	//�ͷű���
//	bool Destroy();	
};
