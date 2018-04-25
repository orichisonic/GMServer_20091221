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
	
	//打开数据集
	bool Open();
	
	//关闭数据集
	void Close();
	
	//执行SQL语句
	bool Query(const char * szFormat);

	//maple add
	SQLCHAR* ExecuteSql(const char * szFormat);

	//执行SQL语句
	//bool Query(const char * szFormat, ...);

	//取一行数据
	bool Fetch();

	//是否最后一行
	void EndFetch();
	
	//得到字段数量
	int GetFieldNum();
	
	//移到第一行
	void MoveFirst();
	
	//绑定列
	virtual BOOL BindColumns(){ return FALSE;}
	
	
	//取列字段
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

	// 宽字节转为字节
	void wcharToChar(wchar_t* str, char* psText);
	
	void DisplayError(SQLRETURN nResult, SWORD fHandleType, SQLHANDLE handle, LPCTSTR lpszHeader);
	std::string m_errMsg;

protected:
	CDatabase*	m_pDB;
	SQLHSTMT	m_hSTMT;
	int			m_nColumnNo;
	
};

#endif // !defined(AFX_DBRECORD_H__7A1CFBF5_55CB_4B13_862D_2C726A8CE450__INCLUDED_)
