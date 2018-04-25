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
		d_result=m_SqlHelper.SqlHelperMain("PAL",lpGMServerip, szSql, 0);//获取SqlExpress中的相应sql语句
		
	}
	catch(...)
	{
		logFile.WriteErrorLog("PAL","设置账号密码未知异常");
		return false;
	}
	
	return d_result;
}

int CModifyAccountPw::SaveAccountPw(char *szRoleAccount, char *szNewPasswd, char *lpGMServerip,char* szMD5NewPasswd)////保存原来帐号密码
{
	char szOldPasswd[32];
	char szSql[256];
	ZeroMemory(szOldPasswd,32);
	ZeroMemory(szSql,256);
	
	char bindpasswd[100];
	ZeroMemory(bindpasswd,100);
	sprintf(bindpasswd,"%s%s",szRoleAccount, szNewPasswd);
	int newlength=0;
	MD5_EnCode(szMD5NewPasswd, &newlength, bindpasswd, strlen(bindpasswd));//把新密码md5加密

	GetAccountPw(szRoleAccount, szOldPasswd, lpGMServerip);//获得原来密码
	if (!strcmp(szOldPasswd, ""))
	{
		return -1;
	}

	sprintf(szSql,"exec Pal_IsAccountPw '%s', '%s'",szRoleAccount, lpGMServerip);
	
	vector<CGlobalStruct::DataRecord> datarecord;
	m_SqlHelper.SqlHelperMain(&datarecord, "127.0.0.1", szSql, 4, 1, 1);//获取SqlExpress中的相应sql语句
	
	if(datarecord.empty())//如果记录为空
	{
		sprintf(szSql,"exec Pal_InsertAccountPw '%s','%s','%s', '%s'",
						szRoleAccount, szOldPasswd, szNewPasswd, lpGMServerip);
		m_SqlHelper.SqlHelperMain("127.0.0.1",  szSql, 4);//获取对应的LogDBIP
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
		m_SqlHelper.SqlHelperMain("PAL",&datarecord, lpGMServerip, szSql, 0, 1, 1);//获取SqlExpress中的相应sql语句
		
		if(datarecord.empty())//如果记录为空
		{
			sprintf(szPasswd,"");
			return;
		}
		
		//获取查询的记录集的每一项
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;	
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("password", m_DataRecord.recordfieldname))//获取SqlExpress中对应sql_type的sql_statement
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
		sprintf(szSql,"exec Pal_FindOldAccountPw '%s', '%s'",szRoleAccount, lpGMServerip);//在本地数据库中查找是否有需要恢复的记录
		vector<CGlobalStruct::DataRecord> datarecord;

		m_SqlHelper.SqlHelperMain("PAL",&datarecord, "127.0.0.1", szSql, 4, 0, 0);//获取SqlExpress中的相应sql语句
		
		if(datarecord.empty())//如果记录为空
		{
			sprintf(szPasswd,"");
			return false;
		}
		//获取查询的记录集的每一项
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;	
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("oldpasswd", m_DataRecord.recordfieldname))//获取SqlExpress中对应sql_type的sql_statement
			{
				sprintf(szPasswd,m_DataRecord.recorddata);//如果记录集中有记录，获取记录的原来密码
			}
		}
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
		sprintf(szSql,"exec Pal_UpdateAccountPwFlag '%s', '%s'",szRoleAccount, lpGMServerip);//修改记录的flag值为0（还原）
		flag=m_SqlHelper.SqlHelperMain("127.0.0.1", szSql, 4);//获取SqlExpress中的相应sql语句
		
	}
	catch(...)
	{
		return false;
	}
	return flag;
	
}