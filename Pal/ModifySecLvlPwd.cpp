// ModifySecLvlPwd.cpp: implementation of the CModifySecLvlPwd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ModifySecLvlPwd.h"
#include "md5.h"
#pragma comment(lib,"md5.lib")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CModifySecLvlPwd::CModifySecLvlPwd()
{

}

CModifySecLvlPwd::~CModifySecLvlPwd()
{

}
bool CModifySecLvlPwd::PutSecLvlPwd(char *szAccount, char * szPasswd,char *lpGMServerName)
{
	bool d_result=false;
	try
	{
		char logdbip[20];
		char szSql[2048];
		char szRemoteSql[2048];
		ZeroMemory(logdbip, 10);
		ZeroMemory(szSql, 2048);
		ZeroMemory(szRemoteSql, 2048);
		operPal.FindAccountDBIP(logdbip,lpGMServerName);
		operPal.getRemoteSql("PAL","PAL_PUTSECLVLPWD",szRemoteSql,0);
		
		sprintf(szSql,szRemoteSql,szPasswd,szAccount);
		
		d_result=m_SqlHelper.SqlHelperMain("PAL",logdbip,  szSql, 0);//获取对应的LogDBIP
	}
	catch(...)
	{
		
	}
	return d_result;
}

bool CModifySecLvlPwd::SaveSecLvlPwd(char *szRoleAccount, char *szNewPasswd, char *lpGMServerName,char* szMD5NewPasswd)////保存原来帐号密码
{
	

		char szOldPasswd[32];
		char szSql[256];
		ZeroMemory(szOldPasswd,32);
		bool b_result=false;
		GetSecLvlPwd(szRoleAccount,szOldPasswd,lpGMServerName);
		
		
		////////////MD5加密///////////////////////
		char bindpasswd[100];
		ZeroMemory(bindpasswd,100);
		sprintf(bindpasswd,"%s",szNewPasswd);
		int newlength=0;
		MD5_EnCode(szMD5NewPasswd,&newlength,bindpasswd,strlen(bindpasswd));
		
		bool flag=true;
		sprintf(szSql,"exec Pal_IsSecLvPw '%s'",szRoleAccount);
	//	vector<CGlobalStruct::DataRecord>* datarecord = new vector<CGlobalStruct::DataRecord>;
		vector<CGlobalStruct::DataRecord> datarecord;
		flag=m_SqlHelper.SqlHelperMain("PAL",&datarecord, "127.0.0.1", szSql, 4, 1, 1);//获取SqlExpress中的相应sql语句
		
		if(datarecord.empty())//如果记录为空
		{
			sprintf(szSql,"exec Pal_InsertSecLvPw '%s','%s','%s'",
				szRoleAccount,szOldPasswd,szNewPasswd);
			flag=m_SqlHelper.SqlHelperMain("PAL","127.0.0.1",  szSql, 4);//获取对应的LogDBIP
			return true;
		} 
		else
		{
			if(!datarecord.empty())
			{
				datarecord.clear();
			}
	//		delete datarecord;
	//		datarecord=NULL;
			return false;
		}
		
		return true;	
}

void CModifySecLvlPwd::GetSecLvlPwd(char *szAccount, char *szPasswd, char *lpGMServerName)
{

	try
	{
		char logdbip[20];
		char szSql[2048];
		char szRemoteSql[2048];
		operPal.FindAccountDBIP(logdbip,lpGMServerName);
		operPal.getRemoteSql("PAL","PAL_GETSECLVLPWD",szRemoteSql,0);
		sprintf(szSql,szRemoteSql,szAccount);
		vector<CGlobalStruct::DataRecord> datarecord;
		m_SqlHelper.SqlHelperMain("PAL",&datarecord, logdbip, szSql, 0, 1, 1);//获取SqlExpress中的相应sql语句
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
			if(!strcmp("expw_1", m_DataRecord.recordfieldname))//获取SqlExpress中对应sql_type的sql_statement
			{
				sprintf(szPasswd,m_DataRecord.recorddata);
			}
		}
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
//		delete datarecord;
//		datarecord=NULL;
	}
	catch(...)
	{
		sprintf(szPasswd,"");
	}

}


bool CModifySecLvlPwd::RecallSecLvlPwd(char *szRoleAccount,char* szPasswd)
{
	bool flag=true;
	try
	{

		char szSql[256];
		vector<CGlobalStruct::DataRecord> datarecord;
		sprintf(szSql,"exec Pal_FindOldSecLvPw '%s'",szRoleAccount);
		m_SqlHelper.SqlHelperMain("PAL",&datarecord, "127.0.0.1", szSql, 4, 1, 1);//获取SqlExpress中的相应sql语句

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
				sprintf(szPasswd,m_DataRecord.recorddata);
			}
		}
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
//		delete datarecord;
//		datarecord=NULL;
		sprintf(szSql,"exec Pal_UpdateSecLvPwFlag '%s'",szRoleAccount);

		flag=m_SqlHelper.SqlHelperMain("127.0.0.1", szSql, 4);//获取SqlExpress中的相应sql语句
	
	}
	catch(...)
	{
		
	}
	return flag;

}

