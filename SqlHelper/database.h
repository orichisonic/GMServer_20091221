// Database.h: interface for the CDatabase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATABASE_H__51BE411E_8AAE_4058_AB38_969071AF6B59__INCLUDED_)
#define AFX_DATABASE_H__51BE411E_8AAE_4058_AB38_969071AF6B59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef SQLOK
#define  SQLOK( nResult )	( nResult == SQL_SUCCESS || nResult == SQL_SUCCESS_WITH_INFO )
#endif

#include <sql.h>
#include <sqltypes.h>
#include <sqlext.h>

#pragma comment (lib, "odbc32.lib")

#include "..\\Global\\Global.h"
#pragma comment(lib,"..\\Global\\debug\\Global.lib")

#include <vector>
using namespace std;



class CDatabase 
{
public:
	SQLHDBC m_hdbc;
	SQLHENV m_henv;
	
	char	m_szDBConnect[MAX_PATH];
	
	bool IsOpen() const ;
	bool Open(const char *  szConnect);
	void Close();
	CDatabase();
	virtual ~CDatabase();
};


#endif // !defined(AFX_DATABASE_H__51BE411E_8AAE_4058_AB38_969071AF6B59__INCLUDED_)
