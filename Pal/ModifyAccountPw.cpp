// ModifyAccountPw.cpp: implementation of the CModifyAccountPw class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ModifyAccountPw.h"
#include "md5.h"
#pragma comment(lib,"md5.lib")
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CModifyAccountPw::CModifyAccountPw()
{

}

CModifyAccountPw::~CModifyAccountPw()
{

}

bool CModifyAccountPw::PutAccountPw(char *szAccount, char * szPasswd,char *lpGMServerip)
{
	CLogFile logFile;
	bool d_result=false;

	try
	{
		char szSql[2048];
		char szRemoteSql[2048];
		ZeroMemory(szSql,2048);
		ZeroMemory(szRemoteSql,2048);

		operPal.getRemoteSql("PAL","PAL_PUTACCOUNTPW",szRemoteSql,0);
		
		sprintf(szSql,szRemoteSql,szPasswd,szAccount);
		d_result=m_SqlHelper.SqlHelperMain("PAL",lpGMServerip, szSql, 0);//��ȡSqlExpress�е���Ӧsql���
		
	}
	catch(...)
	{
		logFile.WriteErrorLog("PAL","�����˺�����δ֪�쳣");
		return false;
	}
	
	return d_result;
}

int CModifyAccountPw::SaveAccountPw(char *szRoleAccount, char *szNewPasswd, char *lpGMServerip,char* szMD5NewPasswd)////����ԭ���ʺ�����
{
	char szOldPasswd[32];
	char szSql[256];
	ZeroMemory(szOldPasswd,32);
	ZeroMemory(szSql,256);
	
	char bindpasswd[100];
	ZeroMemory(bindpasswd,100);
	sprintf(bindpasswd,"%s%s",szRoleAccount, szNewPasswd);
	int newlength=0;
	MD5_EnCode(szMD5NewPasswd, &newlength, bindpasswd, strlen(bindpasswd));//��������md5����

	GetAccountPw(szRoleAccount, szOldPasswd, lpGMServerip);//���ԭ������
	if (!strcmp(szOldPasswd, ""))
	{
		return -1;
	}

	sprintf(szSql,"exec Pal_IsAccountPw '%s', '%s'",szRoleAccount, lpGMServerip);
	
	vector<CGlobalStruct::DataRecord> datarecord;
	m_SqlHelper.SqlHelperMain(&datarecord, "127.0.0.1", szSql, 4, 1, 1);//��ȡSqlExpress�е���Ӧsql���
	
	if(datarecord.empty())//�����¼Ϊ��
	{
		sprintf(szSql,"exec Pal_InsertAccountPw '%s','%s','%s', '%s'",
						szRoleAccount, szOldPasswd, szNewPasswd, lpGMServerip);
		m_SqlHelper.SqlHelperMain("127.0.0.1",  szSql, 4);//��ȡ��Ӧ��LogDBIP
		return 1;
	} 
	else
	{
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
		return 0;
	}	
}

void CModifyAccountPw::GetAccountPw(char *szAccount, char *szPasswd, char *lpGMServerip)
{
	try
	{
		char szSql[2048];
		char szRemoteSql[2048];
	
		ZeroMemory(szSql,2048);
		ZeroMemory(szRemoteSql,2048);
		operPal.getRemoteSql("PAL","PAL_GETACCOUNTPW", szRemoteSql, 0);
		sprintf(szSql,szRemoteSql,szAccount);
		
		vector<CGlobalStruct::DataRecord> datarecord;
		m_SqlHelper.SqlHelperMain("PAL",&datarecord, lpGMServerip, szSql, 0, 1, 1);//��ȡSqlExpress�е���Ӧsql���
		
		if(datarecord.empty())//�����¼Ϊ��
		{
			sprintf(szPasswd,"");
			return;
		}
		
		//��ȡ��ѯ�ļ�¼����ÿһ��
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;	
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("password", m_DataRecord.recordfieldname))//��ȡSqlExpress�ж�Ӧsql_type��sql_statement
			{
				sprintf(szPasswd,m_DataRecord.recorddata);
			}
		}
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
	}
	catch(...)
	{
		sprintf(szPasswd,"");
	}
	
}

bool CModifyAccountPw::RecallAccountPw(char *szRoleAccount,char* szPasswd, char *lpGMServerip)
{
	bool b_result=false;
	bool flag=true;
	try
	{
		char szSql[256];
		sprintf(szSql,"exec Pal_FindOldAccountPw '%s', '%s'",szRoleAccount, lpGMServerip);//�ڱ������ݿ��в����Ƿ�����Ҫ�ָ��ļ�¼
		vector<CGlobalStruct::DataRecord> datarecord;

		m_SqlHelper.SqlHelperMain("PAL",&datarecord, "127.0.0.1", szSql, 4, 0, 0);//��ȡSqlExpress�е���Ӧsql���
		
		if(datarecord.empty())//�����¼Ϊ��
		{
			sprintf(szPasswd,"");
			return false;
		}
		//��ȡ��ѯ�ļ�¼����ÿһ��
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;	
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("oldpasswd", m_DataRecord.recordfieldname))//��ȡSqlExpress�ж�Ӧsql_type��sql_statement
			{
				sprintf(szPasswd,m_DataRecord.recorddata);//�����¼�����м�¼����ȡ��¼��ԭ������
			}
		}
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
		sprintf(szSql,"exec Pal_UpdateAccountPwFlag '%s', '%s'",szRoleAccount, lpGMServerip);//�޸ļ�¼��flagֵΪ0����ԭ��
		flag=m_SqlHelper.SqlHelperMain("127.0.0.1", szSql, 4);//��ȡSqlExpress�е���Ӧsql���
		
	}
	catch(...)
	{
		return false;
	}
	return flag;
	
}