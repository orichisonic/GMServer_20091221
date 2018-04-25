// SQL Server 에 연결
/*/*!----------------------------------------------------------------------------

# 클래스명 : CDatabase

# 참조객체 : 

# 설    명 :  // ODBC  SQL 연결 
    1. SQL_HANDLE_ENV를 사용
		SQLAllocHandle을 호출함으로써 ODBC를 초기화하고 환경 핸들을 할당합니다.
	2. SQLSetEnvAttr을(를) 호출하여 응용 프로그램이 ODBC 3.x 형식 함수 호출을 사용
	3. SQL_HANDLE_DBC를 사용하여 SQLAllocHandle을 호출함으로써 연결 핸들을 할당합니다
	4. SQLDriverConnect를 호출하여 SQL Server에 연결하는 데 연결 문자열 사용

# 내    력 : 2002-09-03 (Kim Jong Nyool) : 초기 골격 생성

# 문 제 점 : 

-----------------------------------------------------------------------------*/


// Database.cpp: implementation of the CDatabase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Database.h"
//#include "Jdebug.hpp"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDatabase::CDatabase()
{
	m_hdbc = SQL_NULL_HANDLE;
	m_henv = SQL_NULL_HANDLE;
	memset(m_szDBConnect, 0, MAX_PATH);
}

CDatabase::~CDatabase()
{
	if (IsOpen())
		Close();
}

bool CDatabase::IsOpen() const
{
	return (m_hdbc != SQL_NULL_HANDLE);
}

void CDatabase::Close()
{
	if (m_hdbc != SQL_NULL_HANDLE)
	{
		SQLDisconnect(m_hdbc);
		SQLFreeHandle (SQL_HANDLE_DBC, m_hdbc);
		m_hdbc = SQL_NULL_HANDLE;

		if (m_henv != SQL_NULL_HANDLE)
			SQLFreeHandle (SQL_HANDLE_ENV, m_henv);
	}
	memset(m_szDBConnect, 0, MAX_PATH);
}

bool CDatabase::Open(const char * szConnect)
{
	try
	{
		
		if( strlen(szConnect) <= 2 )
		{
		//	printf("lenght\n");
			return false;
		}
		else
		{
		//	printf("cc\n");
		}
		strncpy(m_szDBConnect, szConnect , MAX_PATH);
		
		SQLRETURN nResult;                //result code
		
		if (IsOpen ())
		{
		//	printf("lenght\n");
			return false;
		}
		else
		{
		//	printf("cc1");
		}
		
		if (m_henv == SQL_NULL_HANDLE)
		{
			nResult = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, (SQLHANDLE*)&m_henv); // 1
			if (!SQLOK(nResult))
			{
				m_henv = SQL_NULL_HANDLE;
			//	printf("lenght\n");
				return false;
			}
			else
			{
			//	printf("cc2");
			}	
			nResult = SQLSetEnvAttr(m_henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC2, SQL_IS_INTEGER); // 2
			if (!SQLOK(nResult))
			{
				SQLFreeHandle (SQL_HANDLE_ENV, m_henv);
				m_henv = SQL_NULL_HANDLE;
				return false;
			}
			else
			{
			//	printf("cc3");
			}
		}
		
		else
		{
		//	printf("cc4");
		}
		
		// Allocate a new Connection Handle.
		nResult = SQLAllocHandle(SQL_HANDLE_DBC, m_henv, (SQLHANDLE*)&m_hdbc); // 3
		if (!SQLOK(nResult))
		{
			m_hdbc = SQL_NULL_HANDLE;
			if (m_henv != SQL_NULL_HANDLE)
				SQLFreeHandle (SQL_HANDLE_ENV, m_henv);
			else
			{
			//	printf("cc8");
			}
			return false;
		}
		else
		{
		//	printf("cc5");
		}
		// 4
		
		TCHAR szConnOut[1024];
		SQLSMALLINT nNumOut = 0;
		nResult = SQLDriverConnect (m_hdbc, NULL,    
			(SQLCHAR*)m_szDBConnect, SQL_NTS,  
			(SQLCHAR*)szConnOut, 1024, &nNumOut, SQL_DRIVER_NOPROMPT);
		
		if (!SQLOK(nResult)) 
		{
// 			SQLCHAR Sqlstate[4];
// 			SQLCHAR ErrorText[1024];
// 			SQLINTEGER ErrorNo;
// 			SQLSMALLINT ErrorLen;
// 			SQLGetDiagRec(SQL_HANDLE_DBC, m_hdbc, 1, Sqlstate, &ErrorNo, ErrorText, 1024, &ErrorLen);
			SQLFreeHandle(SQL_HANDLE_DBC, m_hdbc);
			m_hdbc = SQL_NULL_HANDLE;
			if (m_henv != SQL_NULL_HANDLE)
				SQLFreeHandle (SQL_HANDLE_ENV, m_henv);
			else
			{
			//	printf("cc6");
			}
			return false;
		}
		else
		{
		//	printf("cc7");
		}
	}
	catch(...)
	{
	//	printf("open dd");
		return false;
	}
	return true;
}