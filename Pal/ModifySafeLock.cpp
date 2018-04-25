// ModifySafeLock.cpp: implementation of the CModifySafeLock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ModifySafeLock.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CModifySafeLock::CModifySafeLock()
{

}

CModifySafeLock::~CModifySafeLock()
{

}
bool CModifySafeLock::LockSafeLock(char *szAccount, char *lpGMServerName)
{
	
	bool d_result=false;
	try
	{
		char logdbip[20];
		char szRemoteData[256];
		char szSql[2048];
		char szRemoteSql[2048];
		operPal.FindAccountDBIP(logdbip,lpGMServerName);
		operPal.getRemoteSql("PAL","PAL_LockSafeLock",szRemoteSql,0);
		
		sprintf(szSql,szRemoteSql,szAccount);

		d_result=m_SqlHelper.SqlHelperMain("PAL",logdbip,  szSql, 0);//��ȡ��Ӧ��LogDBIP

		
	}
	catch(...)
	{
		
	}

	return d_result;
	
}

bool CModifySafeLock::UnLockSafeLock(char *szRoleAccount, char *lpGMServerName)
{
	bool d_result=false;
	try
	{
		char logdbip[20];
		char szRemoteData[256];
		char szSql[2048];
		char szRemoteSql[2048];
		operPal.FindAccountDBIP(logdbip,lpGMServerName);
		operPal.getRemoteSql("PAL","PAL_UnLockSafeLock",szRemoteSql,0);
		
		sprintf(szSql,szRemoteSql,szRoleAccount);
		
		d_result=m_SqlHelper.SqlHelperMain("PAL",logdbip,  szSql, 0);//��ȡ��Ӧ��LogDBIP
	}
	catch(...)
	{
		
	}
	return d_result;
}

bool CModifySafeLock::GetSafeLock(char *szAccount, char *lpGMServerName)
{

	bool d_result=false;
	try
	{
		
		char logdbip[20];
		char szRemoteData[256];
		char szSql[2048];
		char szRemoteSql[2048];
		char lockStatus[4];
		operPal.FindAccountDBIP(logdbip,lpGMServerName);
		
		operPal.getRemoteSql("PAL","PAL_GetSafeLock",szRemoteSql,0);//��ð�ȫ����״̬
			
		sprintf(szSql,szRemoteSql,szAccount);
	//	vector<CGlobalStruct::DataRecord>* datarecord = new vector<CGlobalStruct::DataRecord>;
		vector<CGlobalStruct::DataRecord> datarecord;
		m_SqlHelper.SqlHelperMain("PAL",&datarecord, logdbip,  szSql, 0, 1, 1);//��ȡ��Ӧ��LogDBIP


		if(datarecord.empty())//�����¼Ϊ��
		{
			return true;
		}
		
		//��ȡ��ѯ�ļ�¼����ÿһ��
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;	
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("prg_lock", m_DataRecord.recordfieldname))//��ȡ��Ӧ��LogDBIP
			{
				sprintf(lockStatus,"%s", m_DataRecord.recorddata);
				if(!strcmp(lockStatus,"0"))
				{
					return true;
				}
				else
				{
					return false;
				}	
			}
		}
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
	//	delete datarecord;
	//	datarecord=NULL;
	
	}
	catch(...)
	{
		
	}
	return true;

}

