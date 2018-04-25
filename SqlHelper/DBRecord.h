// DBRecord.h: interface for the CDBRecord class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBRECORD_H__7A1CFBF5_55CB_4B13_862D_2C726A8CE450__INCLUDED_)
#define AFX_DBRECORD_H__7A1CFBF5_55CB_4B13_862D_2C726A8CE450__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class  CDatabase;

#define MAX_DATALEN 8192
#define MAX_COLUMN 256
#define SQLERR_FORMAT       "SQL Error State:%s, Native Error Code: %lX, ODBC Error: %s"
#define SQLERRMSGTITLE      "SQL_ERROR results"
#define SQLERRCNTDTITLE     "SQL_ERROR results continued"
#define SQLWRNMSGTITLE      "SQL_SUCCESS_WITH_INFO results"
#define SQLWRNCNTDTITLE     "SQL_SUCCESS_WITH_INFO results continued"

#include <vector>
#include "..\\Global\\Global.h"
#pragma comment(lib,"..\\Global\\debug\\Global.lib")

class CDBRecord  
{
public:
	CDBRecord(CDatabase* pDB);
	virtual ~CDBRecord();
	
	//�����ݼ�
	bool Open();
	
	//�ر����ݼ�
	void Close();
	
	//ִ��SQL���
	bool Query(const char * szFormat);

	//maple add
	SQLCHAR* ExecuteSql(const char * szFormat);

	//ִ��SQL���
	//bool Query(const char * szFormat, ...);

	//ȡһ������
	bool Fetch();

	//�Ƿ����һ��
	void EndFetch();
	
	//�õ��ֶ�����
	int GetFieldNum();
	
	//�Ƶ���һ��
	void MoveFirst();
	
	//����
	virtual BOOL BindColumns(){ return FALSE;}
	
	
	//ȡ���ֶ�
	CDBRecord&	operator >> ( char &nVal );
	CDBRecord&	operator >> ( short &nVal );
	CDBRecord&	operator >> ( int &nVal );
	CDBRecord&	operator >> ( BYTE &nVal );
	CDBRecord&	operator >> ( WORD &wVal );
	CDBRecord&	operator >> ( DWORD &dwVal );
	CDBRecord&	operator >> ( std::string &strVal );
	
	CDBRecord&	operator >> ( char *pVal );
	CDBRecord&	operator >> ( BYTE *pVal );
	CDBRecord&	operator >> ( TIMESTAMP_STRUCT &nVal );
	
	CDBRecord&	operator >> ( CGlobalStruct::DataRecord* datarecord);

	// ���ֽ�תΪ�ֽ�
	void wcharToChar(wchar_t* str, char* psText);
	
	void DisplayError(SQLRETURN nResult, SWORD fHandleType, SQLHANDLE handle, LPCTSTR lpszHeader);
	std::string m_errMsg;

protected:
	CDatabase*	m_pDB;
	SQLHSTMT	m_hSTMT;
	int			m_nColumnNo;
	
};

#endif // !defined(AFX_DBRECORD_H__7A1CFBF5_55CB_4B13_862D_2C726A8CE450__INCLUDED_)
