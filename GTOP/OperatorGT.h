#pragma once
#include "..\\GameIOCP\\OperatorGame.h"
class COperatorGT : public COperatorGame
{
private:
	//完整的sql语句
	char szSql[256];
	
	//从SqlExrepss中获取的sql语句
	char m_sql[256];


public:
	//构造函数
	COperatorGT(void);
	
	//析构函数
	~COperatorGT(void);
	
	

	//获取需要的字符串
	bool getSpecChar(const char* string, const char* find, int nNum, const char* findNext, int mNum, char* nChar);

	//查找字符串中特定的字符第一次出现的位置
	int StringFind(const char* string,const char* find,int number);
	
	// 宽字节转为字节
	char* wcharToChar(wstring str);
	
	// 字节转为宽字节
	wchar_t* charToWChar(char* str);


	//获得真实的服务器IP
	bool GetServerIP(char* gameName, char* SrcServerIP, int gameDBID, char* DestServerIP, int* DestPort);

	bool BuildTLV(COperVector m_OpVect);
	bool BuildTLV(char * message);

private:
	char* psText;
	wchar_t* pwText;
	
};
