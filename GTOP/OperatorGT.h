#pragma once
#include "..\\GameIOCP\\OperatorGame.h"
class COperatorGT : public COperatorGame
{
private:
	//������sql���
	char szSql[256];
	
	//��SqlExrepss�л�ȡ��sql���
	char m_sql[256];


public:
	//���캯��
	COperatorGT(void);
	
	//��������
	~COperatorGT(void);
	
	

	//��ȡ��Ҫ���ַ���
	bool getSpecChar(const char* string, const char* find, int nNum, const char* findNext, int mNum, char* nChar);

	//�����ַ������ض����ַ���һ�γ��ֵ�λ��
	int StringFind(const char* string,const char* find,int number);
	
	// ���ֽ�תΪ�ֽ�
	char* wcharToChar(wstring str);
	
	// �ֽ�תΪ���ֽ�
	wchar_t* charToWChar(char* str);


	//�����ʵ�ķ�����IP
	bool GetServerIP(char* gameName, char* SrcServerIP, int gameDBID, char* DestServerIP, int* DestPort);

	bool BuildTLV(COperVector m_OpVect);
	bool BuildTLV(char * message);

private:
	char* psText;
	wchar_t* pwText;
	
};
