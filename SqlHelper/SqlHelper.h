// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
//��ı�׼�������� DLL �е������ļ��������������϶���� SQLHELPER_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
//�κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ 
// SQLHELPER_API ������Ϊ�ǴӴ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef SQLHELPER_EXPORTS
#define SQLHELPER_API __declspec(dllexport)
#else
#define SQLHELPER_API __declspec(dllimport)
#endif
#include "database.h"
#include "DBRecord.h"
using namespace std;
#include <vector>
#include "..\\Global\\Global.h"
#pragma comment(lib,"..\\Global\\Debug\\Global.lib")

// �����Ǵ� SqlHelper.dll ������
class SQLHELPER_API CSqlHelper {
public:
	//���캯��
	CSqlHelper(void);

	//��������
	~CSqlHelper(void);

	//���vector,����ip��ַ,Sql���,flag��־,���ز�ѯ���Ƿ�鵽
	bool SqlHelperMain(vector<CGlobalStruct::DataRecord>* ret_vector,char * ip,char *sql,int flag,int nIndex, int nPageSize);

	//������Ϸ��ѯ�෵��vecter��
	bool SqlHelperMain(char * gamename,vector<CGlobalStruct::DataRecord>* ret_vector,char * ip,char *sql,int flag,int nIndex, int nPageSize);
	
	vector<CGlobalStruct::DataRecord> SqlHelperMain(char * gamename,char * ip,char *sql,int flag,int nIndex, int nPageSize);

	//���ز������Ƿ�ɹ�
	bool SqlHelperMain(char * ip,char *sql,int flag);
	
	//������Ϸ�����෵�سɹ�ʧ��
	bool SqlHelperMain(char * gamename,char * ip,char *sql,int flag);

	//maple add
	void SqlHelperMain_char(char * gamename,char * ip,char *sql,int flag,char * result);
	
	//��ȡ����
	int GetFieldNum();

private:
	//�������ݿ�����Driver
	void MakeDataSource(char * szData,char *ip,int flag);

	//������Ϸ�������ݿ�����Driver
	void MakeDataSource(char * gamename,char * szData,char *ip,int flag);

	//��ȡSQL���ݿ�
	void GetSQLData(char* gamename,char * szData,char * lpIP,int flag);

	//��ȡMySql���ݿ�
	void GetMySqlData(char* gamename,char * szData,char * lpIP,int flag);


	//��ȡSQL���ݿ�
	void GetSQLData(char * szData,char * lpIP,int flag);
	
	//��ȡMySql���ݿ�
	void GetMySqlData(char * szData,char * lpIP,int flag);
	//��ȡSQL���ݿ�
	void GetSQLData(char * user,char * passwd,int iSQLPort,char * szData,char * lpIP);
	//��ȡMySql���ݿ�
	void GetMySqlData(char * user,char * passwd,int iMySQLPort,char * CharSet,char * szData,char * lpIP);

	// TODO: �ڴ�������ķ�����
	void GetDefault(char * gamename,char *user,char * passwd,char * CharSet,int* iSQLPort,int *iMySqlPort);

	//����SQL��������limit
	void MakeSQL(char * lpobjSQL,char * lpResSQL,int flag,int iIndex,int iPageSize);




private:
	int fieldnum;
	//�������ݿ�
//	CDatabase 	database;

	//datarecord
//	CGlobalStruct::DataRecord tmpDataRecord;


};

extern SQLHELPER_API int nSqlHelper;

SQLHELPER_API int fnSqlHelper(void);
