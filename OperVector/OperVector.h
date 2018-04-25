#pragma once

#include "MyEvent.h"

#include "shlwapi.h"
#pragma comment(lib,"shlwapi.lib")

#include<vector>
using namespace std;

class __declspec(dllexport) COperVector
{
private:

	//���ݰ��е����ݵ�Ԫ����Query������
	int alltflv,allquery,colTFLV;

	//����TFLV��vector
	vector<CGlobalStruct::TFLV> InnerList;

	//�������ݿ��ѯ���ݼ�
//	vector<CGlobalStruct::DataRecord> m_datarecord;
	
	//���͵�����
	unsigned char *lpDes;

	//ִ�����ݿ���ز���
	CSqlHelper m_SqlHelper;

	//����Ƿ�������
	bool queryflag;

public:
	
	//���캯��
	COperVector(void);

	//�������ݰ��ĳ�ʼ������
	COperVector(int querynum,int tflvnum);
	COperVector(vector<CGlobalStruct::TFLV> m_innerList);

	//��������
	~COperVector(void);



	// ����Ҫ���͵�����
	unsigned char * getDes();

	//�������ݰ��е����ݵ�Ԫ��
	bool Create(int m_alltflv);

	//���ض���������������ʼ�����ݰ�
	bool InitCreate(int tflvnum,int querynum);




	// ����Ҫ���͵����ݵ�Ԫ
	int MakeTLV(unsigned char * lpDes,CGlobalStruct::TFLV m_tflv);

	// ����ض����ݵ����ݵ�Ԫ
	bool AddTFLV(CEnumCore::TagName tag, CEnumCore::TagFormat tagFormat, int length, unsigned char * lpdata);

	// ����ض����ݵ����ݵ�Ԫ
	bool AddTFLV(CEnumCore::TagName tag, CEnumCore::TagFormat tagFormat, size_t length, unsigned char * lpdata);

	// ��ӵ������ݵ�Ԫ
	bool AddTFLV(CGlobalStruct::TFLV m_tflv);

	// �������ݰ�
	int EnBody(void);
	int EnBody(vector<CGlobalStruct::TFLV> *TFLVList, unsigned char * pDes);



	// �������յ������ݲ���װ
	CGlobalStruct::TFLV DeMakeTLV(unsigned char * lpSou,int* m_length, int index, int* m_Judge);

	// �������յ�������
	BOOL DeBody(unsigned char* lpSou, int bodylength);



	// ��ȡ���е�TFLV��Ŀ
	int getAllTFLV();

	// ��ȡ���е�Query��Ŀ
	int getQueryNum();

	// ��ȡ���е�����
	int getColTFLVNum();




	// �������ݵ�Ԫ��������ѯ���ݵ�Ԫ
	CGlobalStruct::TFLV getTLVBynIndex(int nIndex);

	// �������ݵ�Ԫ��TagName�Ϳ�ʼ������ѯ���ݵ�Ԫ
	CGlobalStruct::TFLV getTLVByTag(CEnumCore::TagName tag,int beginIndex);

	// �������ݵ�Ԫ���С��кŲ�ѯ���ݵ�Ԫ
	CGlobalStruct::TFLV getTLV(int nRow,int nCol);

	// �������ݵ�Ԫ���кź�������ѯ���ݵ�Ԫ
	CGlobalStruct::TFLV getTLVByName(int nRow,CEnumCore::TagName tag);

	//������������
	bool getActiveCol(CEnumCore::TagName tag);

	//�Ƿ����ù���������
	bool IsActiveCol();

	//���innerlist
	void ClearInnerList();

	//�ͷ����ݰ��е���Դ
	bool Destroy(void);

public:

	//ִ�в�ѯ����������Ҫ���͵�����
	int OperVectorMain(char* ip, char* sql, int flag,int nIndex,int nPageSize);

	//ִ�в��������ز������
	int OperVectorMain(char* ip, char* sql, int flag);

	//ִ�в�ѯ����������Ҫ���͵�����
	int OperVectorMain(char * gamename,char* ip, char* sql, int flag,int nIndex,int nPageSize);
	
	//ִ�в��������ز������
	int OperVectorMain(char * gamename,char* ip, char* sql, int flag);
	
	//maple add
	int OperVectorMain_char(char * gamename,char* ip, char* sql, int flag,char *str);

public:

	//ִ�в�ѯ�����ز�ѯ�����
//	vector<CGlobalStruct::DataRecord> * GetDBSet(char * gamename,char* ip, char* sql, int flag,int nIndex,int nPageSize);

	vector<CGlobalStruct::DataRecord> GetDBSet(char * gamename,char* ip, char* sql, int flag,int nIndex,int nPageSize);
	//��������������TFLV����
	vector<CGlobalStruct::TFLV> GetTFLV(vector<CGlobalStruct::DataRecord> m_datarecord);
	
	vector<CGlobalStruct::TFLV> GetInnerList();
};
