/*!----------------------------------------------------------------------------

# 클래스명 : CDBRecord

# 참조객체 : 

# 설    명 : 
	
	* SQL 의 데이타 가져 오고 에러 체크 *
   SQL Server 명령 수행 
	1. SQL_HANDLE_STMT을 사용하여 SQLAllocHandle을 호출함으로써 명령문 핸들을 할당합니다
	2. SQLSetStmtAttr을(를) 호출하여 명령문 옵션을 설정하거나 SQLGetStmtAttr을(를) 호출하여 명령문 특성을 가져올 수도 있습니다. 
	3. 명령문을 여러 번 실행하려면 SQLPrepare로 실행할 명령문을 준비할 수도 있습니다.
	4. 명령에 바운드된 매개 변수 표식이 있을 경우 SQLBindParameter를사용하여 이 매개 변수 표식을 프로그램 변수에 바인딩할 수도 있습니다. 
	   명령문이 준비되었으면 SQLNumParams 및 SQLDescribeParam을 호출하여 매개 변수의 개수와 특성을 알 수 있습니다.
	5. SQLExecDirect를 사용하여 직접 명령문을 실행합니다. 
	   명령문이 준비되었으면 SQLExecute를 사용하여 이 명령문을 여러 번 실행합니다. 
	6. SQLGetData를 사용하여 결과 집합 열의 데이터를 프로그램 변수로 이동
	   SQLRowCount를 호출하여 INSERT, UPDATE 또는 DELETE 문으로 영향 받는 행의 수를 알아 냅니다.
	7. SQL_HANDLE_STMT의 HandleType을 사용하여 SQLFreeHandle을 호출함으로써 명령문 핸들을 해제
	
	1 -> 5 -> 6 -> SQLFetch (다음 rowset을 해줌 )-> 7

# 내    력 : 2002-09-03 (Kim Jong Nyool) : 초기 골격 생성

# 문 제 점 : 

-----------------------------------------------------------------------------*/
// DBRecord.cpp: implementation of the CDBRecord class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Database.h"
#include "DBRecord.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDBRecord::CDBRecord(CDatabase* pDB)
{
	m_pDB = pDB;
	m_hSTMT = SQL_NULL_HANDLE;
}

CDBRecord::~CDBRecord()
{
	Close();
}

bool CDBRecord::Open()
{
	if((m_pDB == NULL)||(m_pDB->m_hdbc == SQL_NULL_HANDLE))
		return false;
	// 1.
	int nRet = SQLOK(SQLAllocHandle(SQL_HANDLE_STMT, m_pDB->m_hdbc, (SQLHANDLE*)&m_hSTMT));
	if(!SQLOK( nRet))
	{
		m_pDB->Close();
		if( m_pDB->Open(m_pDB->m_szDBConnect))
		{
			nRet = SQLOK(SQLAllocHandle(SQL_HANDLE_STMT, m_pDB->m_hdbc, (SQLHANDLE*)&m_hSTMT));
			if( !SQLOK(nRet))
			{
				DisplayError(nRet,SQL_HANDLE_STMT, m_hSTMT, "CDBRecord::Open");
				return false;
			}
			else
				return true;
		}
		else
		{
			DisplayError(nRet, SQL_HANDLE_STMT, m_hSTMT, "CDBRecord::Open2");
			return false;
		}
	}
	else
		return true;
}

void CDBRecord::Close()
{
	if( m_hSTMT != SQL_NULL_HANDLE)
	{
		SQLFreeHandle(SQL_HANDLE_STMT, m_hSTMT);
		m_hSTMT = SQL_NULL_HANDLE;
	}
}

//maple add
SQLCHAR* CDBRecord::ExecuteSql(const char * szFormat)
{
	SQLRETURN retcode,ret;
	SQLCHAR  szSalesPerson[300];
	SQLINTEGER cbSalesPerson = SQL_NTS,cbStatus=SQL_NTS,cbItem=SQL_NTS;

	
	SQLCHAR szStatus[100];
	SQLCHAR szItem[100];
	SQLSMALLINT      sCustID;

	SQLINTEGER      cbCustID = 0;

//	retcode = SQLPrepare(m_hSTMT,(unsigned char*)szFormat, SQL_NTS);


	//char exSql[128];
	//ZeroMemory(exSql,128);
//	strcpy(exSql,"exec JW2_PRODUCTPOWER_Valid ?,?,?,?");
	retcode =SQLPrepare(m_hSTMT,(unsigned char*)szFormat,SQL_NTS);

	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
	{

/*		strcpy((char *)szStatus,"222.211.80.58");
		ret=SQLBindParameter(m_hSTMT, 1, SQL_PARAM_INPUT, SQL_C_CHAR,SQL_CHAR, 100, 0, szStatus, 0, &cbStatus);
		
		cbCustID=2;
		ret=SQLBindParameter(m_hSTMT, 2, SQL_PARAM_INPUT, SQL_C_SSHORT,SQL_INTEGER, 0, 0, &sCustID, 0, &cbCustID);
		
		strcpy((char *)szItem,"101050*1|");
		ret=SQLBindParameter(m_hSTMT, 3, SQL_PARAM_INPUT, SQL_C_CHAR,SQL_CHAR, 100, 0, szItem, 0, &cbItem);
*/


		ret=SQLBindParameter(m_hSTMT, 4, SQL_PARAM_OUTPUT, SQL_C_CHAR,SQL_VARCHAR, 300,0, &szSalesPerson, 500, &cbSalesPerson); ////SQL_CHAR
		if ( (ret != SQL_SUCCESS) && (ret != SQL_SUCCESS_WITH_INFO) )
		{
			return szSalesPerson;
		}

		retcode = SQLExecute(m_hSTMT);
	}
	return szSalesPerson;
}

//獵契SQL刀얌
bool CDBRecord::Query(const char * szFormat)
{
	Sleep(200);
	if( !m_hSTMT )
		return false;
	
	int nRet;
	//unsigned char szQuery[8192];
	
	//va_list vaList;
	//va_start(vaList, szFormat);
	//vsprintf((char*)szQuery, szFormat, vaList);
	//va_end( vaList );
	
	//nRet = SQLExecDirect(m_hSTMT, szQuery, SQL_NTS);
	nRet = SQLExecDirect(m_hSTMT, (unsigned char*)szFormat, SQL_NTS);
	
	if( nRet == SQL_ERROR)
	{
		Close();
		Open();
		//nRet = SQLExecDirect(m_hSTMT, szQuery, SQL_NTS);
		nRet = SQLExecDirect(m_hSTMT, (unsigned char*)szFormat, SQL_NTS);
		
		if(!SQLOK(nRet))
		{
			//DisplayError(nRet, SQL_HANDLE_STMT, m_hSTMT, CString("CDBRecord::Query - ") + szQuery );
			return false;
		}
		else
			return true;
	}
	else if(!SQLOK(nRet))
	{
		//(nRet, SQL_HANDLE_STMT, m_hSTMT, CString("CDBRecord::Query - ")+ szQuery);
		return false;
	}
	else
		return true;
}

// bool CDBRecord::Query(const char * szFormat, ...)
// {
// 	if( !m_hSTMT )
// 		return false;
// 	
// 	int nRet;
// 	unsigned char szQuery[8192];
// 	
// 	va_list vaList;
// 	va_start(vaList, szFormat);
// 	vsprintf((char*)szQuery, szFormat, vaList);
// 	va_end( vaList );
// 
// 	nRet = SQLExecDirect(m_hSTMT, szQuery, SQL_NTS);
// 
// 	if( nRet == SQL_ERROR)
// 	{
// 		Close();
// 		Open();
// 		nRet = SQLExecDirect(m_hSTMT, szQuery, SQL_NTS);
// 		if(!SQLOK(nRet))
// 		{
// 			//DisplayError(nRet, SQL_HANDLE_STMT, m_hSTMT, CString("CDBRecord::Query - ") + szQuery );
// 			return false;
// 		}
// 		else
// 			return true;
// 	}
// 	else if(!SQLOK(nRet))
// 	{
// 		//(nRet, SQL_HANDLE_STMT, m_hSTMT, CString("CDBRecord::Query - ")+ szQuery);
// 		return false;
// 	}
// 	else
// 		return true;
// }

bool CDBRecord::Fetch()
{
	int nRet;
	m_nColumnNo = 0;
	nRet = SQLFetch(m_hSTMT);
	return SQLOK(nRet);
}

void CDBRecord::EndFetch()
{
	int nRet;
	nRet = SQLCloseCursor(m_hSTMT);
}

void CDBRecord::DisplayError(SQLRETURN nResult, SWORD fHandleType, SQLHANDLE handle , LPCTSTR lpszHeader)
{
	//UCHAR	szErrState[SQL_SQLSTATE_SIZE + 1]; // SQL Error State string
	//UCHAR	szErrText[SQL_MAX_MESSAGE_LENGTH + 1];	// SQL Error Text string
	//char	szBuffer[SQL_SQLSTATE_SIZE + SQL_MAX_MESSAGE_LENGTH + MAX_DATALEN + 1];
	//char	szDispBuffer[SQL_SQLSTATE_SIZE + SQL_MAX_MESSAGE_LENGTH + MAX_DATALEN + 1];

	//SWORD	wErrMsgLen;				// Error message length
	//UDWORD	dwErrCode;              // Native Error code
	//int		iSize;					// Display Error Text size
	//SQLRETURN	nErrResult;			// Return Code from SQLGetDiagRec
	//SWORD	sMsgNum = 1;

	//szBuffer[0] = '\0';
	//CTime ct = CTime::GetCurrentTime();
	//CString fileName = ct.Format("log\\DB_%Y_%m_%d.log");
	//CString tempstr = ct.Format("[%H:%M:%S]");
	//CString line1;
	//line1.Format("%s sql_error = %d", tempstr, nResult);
	//m_errMsg = line1;

	//CStdioFile file;

	//if(!file.Open(fileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText | CFile::shareDenyNone))
	//{
	//	m_errMsg += "\n";
	//	m_errMsg += "Open DB log file error";
	//	return;
	//}
	//file.SeekToEnd();
	//file.WriteString(line1 + "\n");
	//do
	//{
	//	strcpy(szDispBuffer, szBuffer);
	//	while((nErrResult = SQLGetDiagRec(fHandleType, handle, sMsgNum++, szErrState, 
	//		  (SQLINTEGER*)&dwErrCode, szErrText, SQL_MAX_MESSAGE_LENGTH - 1, &wErrMsgLen)) != SQL_NO_DATA ) 
	//	{
	//		if( nErrResult == SQL_ERROR || nErrResult == SQL_INVALID_HANDLE)
	//			break;
	//		wsprintf(szBuffer, SQLERR_FORMAT, (LPSTR)szErrState, dwErrCode, (LPSTR)szErrText);
	//		iSize = strlen(szDispBuffer);
	//		if (iSize && (iSize+strlen(szBuffer) + 1) >= 80)
	//		{
	//			strcat(szDispBuffer, "\n");
	//			break;
	//		}
	//		strcat(szDispBuffer, szBuffer);
	//	}
	//	// display proper ERROR or WARNING message with proper title

	//	if ((nResult != SQL_SUCCESS_WITH_INFO) && (nResult != SQL_NO_DATA))
	//	{
	//		file.WriteString( CString(lpszHeader) + "\n" );
	//		m_errMsg += ( "\n" + CString(lpszHeader) );
	//		file.WriteString( CString(szDispBuffer) + "\n" );
	//		m_errMsg += ( "\n" + CString(szDispBuffer) );
	//	}
	//}
	//while (!(nErrResult == SQL_NO_DATA || nErrResult == SQL_ERROR || nErrResult == SQL_INVALID_HANDLE));
	//file.Close();
}
int CDBRecord::GetFieldNum()
{
	int fieldnum=0;
	SQLNumResultCols(m_hSTMT,(SQLSMALLINT *)&fieldnum);
	return fieldnum;
}
void CDBRecord::MoveFirst()
{
	SQLSetPos(m_hSTMT,0,SQL_POSITION,SQL_LOCK_NO_CHANGE);
}
CDBRecord & CDBRecord::operator >> ( char & nVal)
{
	m_nColumnNo++;
	SQLGetData(m_hSTMT, m_nColumnNo, SQL_C_STINYINT, &nVal, sizeof( char), 0);
	return *this;
}

CDBRecord & CDBRecord::operator >> ( short & nVal)
{
	m_nColumnNo++;
	SQLGetData(m_hSTMT, m_nColumnNo, SQL_C_SHORT, & nVal, sizeof(short), 0);
	return *this;
}

CDBRecord & CDBRecord::operator >> ( int &nVal )
{
	m_nColumnNo++;
	SQLGetData( m_hSTMT, m_nColumnNo, SQL_C_SLONG, &nVal, sizeof( int ), 0 );
	return *this;
}

CDBRecord & CDBRecord::operator >> ( BYTE &nVal )
{
	m_nColumnNo++;
	SQLGetData( m_hSTMT, m_nColumnNo, SQL_C_UTINYINT, &nVal, sizeof( BYTE ), 0 );
	return *this;
}

CDBRecord & CDBRecord::operator >> ( WORD &wVal )
{
	m_nColumnNo++;
	SQLGetData( m_hSTMT, m_nColumnNo, SQL_C_USHORT, &wVal, sizeof( WORD ), 0 );
	return *this;
}

CDBRecord & CDBRecord::operator >> ( DWORD &dwVal )
{
	m_nColumnNo++;
	SQLGetData( m_hSTMT, m_nColumnNo, SQL_C_ULONG, &dwVal, sizeof( DWORD ), 0 );
	return *this;
}

CDBRecord &CDBRecord::operator >> ( std::string &strVal )
{
	BYTE cBuffer[1024]= {0};

	*this >> cBuffer;
//	strVal = cBuffer;

	return *this;
}


CDBRecord & CDBRecord::operator >> (char * pVal)
{
	int nWidth;
	m_nColumnNo++;
	// SQLColAttribute 결과 열에 대한 특정 설명자 정보
	SQLColAttribute(m_hSTMT, m_nColumnNo, SQL_DESC_OCTET_LENGTH, 0, 0, 0, &nWidth );
	SQLGetData(m_hSTMT, m_nColumnNo, SQL_C_CHAR, pVal, nWidth, 0);
	return *this;
}

CDBRecord & CDBRecord::operator >> (BYTE * pVal)
{
	int nWidth;
	m_nColumnNo++;
	SQLColAttribute(m_hSTMT, m_nColumnNo, SQL_DESC_LENGTH, 0, 0, 0, &nWidth);
	memset(pVal, 0, nWidth);
	SQLGetData(m_hSTMT, m_nColumnNo, SQL_C_BINARY, pVal, nWidth, 0);
	return *this;
}

CDBRecord & CDBRecord::operator >> (TIMESTAMP_STRUCT & nVal)
{
	m_nColumnNo ++;
	SQLGetData(m_hSTMT, m_nColumnNo, SQL_C_TIMESTAMP, & nVal, sizeof(TIMESTAMP_STRUCT), 0 );
	return *this;
}
CDBRecord & CDBRecord::operator >>( CGlobalStruct::DataRecord* datarecord)
{
	int nWidth=0;
	int fieldlength=0;
	TIMESTAMP_STRUCT time_struct;
	ZeroMemory(&time_struct,sizeof(TIMESTAMP_STRUCT));
	int iNum=0;
	short sNum=0;
	unsigned char uchar=0;
	ZeroMemory(datarecord->recorddata,12000);
	m_nColumnNo ++;

	///삿혤코휭//////////////
	SQLColAttribute(m_hSTMT, m_nColumnNo, SQL_DESC_OCTET_LENGTH, 0, 0, 0, &nWidth);
	SQLDescribeCol(m_hSTMT,m_nColumnNo,(SQLCHAR *)&(datarecord->recordfieldname),50,(SQLSMALLINT *)&fieldlength,(SQLSMALLINT *)&(datarecord->recordfieldtype),NULL,NULL,NULL);
	switch(datarecord->recordfieldtype)
	{
	case 4://鑒俚
		SQLGetData(m_hSTMT, m_nColumnNo, SQL_C_DEFAULT, &iNum, nWidth, 0);
		sprintf(datarecord->recorddata,"%i",iNum);
		break;
	case 5://smallint鑒俚
		SQLGetData(m_hSTMT, m_nColumnNo, SQL_C_SSHORT, &sNum, nWidth, 0);
		sprintf(datarecord->recorddata,"%i",sNum);
		break;
	case 6://륫듐謹
		SQLGetData(m_hSTMT, m_nColumnNo, SQL_C_CHAR, &(datarecord->recorddata), nWidth, 0);
		break;
	case 12://俚륜눔
		SQLGetData(m_hSTMT, m_nColumnNo, SQL_C_DEFAULT, &(datarecord->recorddata), nWidth+1, 0);
		if(!strcmp(datarecord->recorddata,""))
		{
			sprintf(datarecord->recorddata,"NULL");
		}
		break;
	case 65529://bit
		SQLGetData(m_hSTMT, m_nColumnNo, SQL_C_STINYINT, &uchar, nWidth, 0);
		sprintf(datarecord->recorddata,"%i",(int)uchar);
		break;
	case 65530://tinyint
		SQLGetData(m_hSTMT, m_nColumnNo, SQL_C_UTINYINT, &uchar, nWidth, 0);
		sprintf(datarecord->recorddata,"%i",(int)uchar);
		break;
	case 11://휑퍅
		{
			SQLGetData(m_hSTMT, m_nColumnNo, SQL_C_DEFAULT, &time_struct, nWidth, 0);
			if(time_struct.year<=0)
			{
				sprintf(datarecord->recorddata,"%s","NULL");
			}
			else
			{
				sprintf(datarecord->recorddata,"%i-%i-%i %i:%i:%i",
					time_struct.year,time_struct.month,time_struct.day,
					time_struct.hour,time_struct.minute,time_struct.second);
			}
		}
		break;
	case 65526://ntext
		{
			wchar_t rData[12000];
			ZeroMemory(rData, 12000);
			SQLGetData(m_hSTMT, m_nColumnNo, SQL_C_WCHAR, &rData, nWidth, 0);
			wcharToChar((wchar_t*)&rData, (char*)&(datarecord->recorddata));
		}
		break;
	case 65534://binary
		{
			SQLGetData(m_hSTMT, m_nColumnNo, SQL_C_BINARY, &uchar, nWidth, 0);
			sprintf(datarecord->recorddata,"%i",(int)uchar);
		}
		break;
	case 65531://bigint
		{
			SQLGetData(m_hSTMT, m_nColumnNo, SQL_C_SLONG,&iNum, nWidth, 0);
			sprintf(datarecord->recorddata,"%i",iNum);
		}
		break;
	case 65533://varbinary
		{
			SQLGetData(m_hSTMT, m_nColumnNo, SQL_C_BINARY, &datarecord->recorddata, nWidth, 0);
		}
		break;
	default:
		SQLGetData(m_hSTMT, m_nColumnNo, SQL_C_CHAR, &(datarecord->recorddata), nWidth, 0);
		break;
	}

	return *this;
}

// 욱俚쌘瘻槨俚쌘
void CDBRecord::wcharToChar(wchar_t* str, char* psText)
{
	DWORD size = WideCharToMultiByte(CP_ACP, NULL, str, -1, NULL, 0, NULL, NULL);    
	WideCharToMultiByte(CP_ACP, NULL, str, -1, psText, size, NULL, NULL);
}
