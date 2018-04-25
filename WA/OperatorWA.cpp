#include "stdafx.h"
#include ".\OperatorWA.h"

//���캯��
COperatorWA::COperatorWA(void)
{	
}

//��������
COperatorWA::~COperatorWA(void)
{

}
bool COperatorWA::BuildTLV(char * message)
{
	return COperatorGame::BuildTLV(message);
}
bool COperatorWA::BuildTLV(vector <CGlobalStruct::TFLV> DBTFLV,CEnumCore::ServiceKey m_ServiceKey,char * ServerIP)
{
	try
	{
		if(DBTFLV.empty())
		{
			return false;
		}
		SendVectBuf.InitCreate(DBTFLV.size()+1, 1);//��ʼ�����͵�����
		
		vector<CGlobalStruct::TFLV>::iterator iter;
		for( iter = DBTFLV.begin(); iter != DBTFLV.end(); iter++ )
		{
			CGlobalStruct::TFLV m_tflv= *iter;
			SendVectBuf.AddTFLV(m_tflv);//Ĭ��Ϊֱ�����TFLV
			
		}
		iEnbodyLength = SendVectBuf.EnBody();
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�������ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("WA",errMessage);
		return false;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�������ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("WA",errMessage);
		return false;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�����������쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("WA",errMessage);
		return false;
	}
	return TRUE;
}
//�����ʵ�ķ��������������ID
// bool COperatorWA::GetClusterID(char* gameName, char* ServerIP, int gameDBID, char* city, int &ClusterID,int &realm_id)
// {
// 	try
// 	{
// 		sprintf(szSql,"exec WA_ClusterID_Query '%s','%s',%i",ServerIP,city,gameDBID);
// 		
// 		vector<CGlobalStruct::DataRecord> datarecord;
// 		
// 		m_SqlHelper.SqlHelperMain("WA", &datarecord, connLocalDB, szSql, 4, 1, 1);//��ȡҪ�滻��ֵ
// 		
// 		if(datarecord.empty())//�����¼Ϊ��
// 		{
// 			return false;
// 		}
// 		
// 		//��ȡ��ѯ�ļ�¼����ÿһ��
// 		vector<CGlobalStruct::DataRecord>::iterator iter;
// 		CGlobalStruct::DataRecord  m_DataRecord;	
// 		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
// 		{
// 			m_DataRecord = *iter;
// 			if(!strcmp("ClusterID", m_DataRecord.recordfieldname))//��ȡ��Ӧ��ServerIP
// 			{
// 				realm_id = StrToInt(m_DataRecord.recorddata);//�����ѯ�õ��ĵ�һ����¼
// 			}
// 			else if(!strcmp("Realmid", m_DataRecord.recordfieldname))//��ȡ��Ӧ��Port
// 			{
// 				ClusterID = StrToInt(m_DataRecord.recorddata);//�����ѯ�õ��ĵ�һ����¼
// 			}
// 		}
// 		if(!datarecord.empty())
// 		{
// 			datarecord.clear();
// 		}
// 		
// 	}
// 	catch (CMemoryException* e)
// 	{
// 		e->GetErrorMessage(errMessage,256);
// 		sprintf(errMessage,"�����ʵ�ķ��������������ID�ڴ治��,������Ϣ:%s",errMessage);
// 		logFile.WriteErrorLog("WA",errMessage);
// 		return false;
// 	}
// 	catch (CFileException* e)
// 	{
// 		e->GetErrorMessage(errMessage,256);
// 		sprintf(errMessage,"�����ʵ�ķ��������������ID�ļ��쳣,������Ϣ:%s",errMessage);
// 		logFile.WriteErrorLog("WA",errMessage);
// 		return false;
// 	}
// 	catch (CException* e)
// 	{
// 		e->GetErrorMessage(errMessage,256);
// 		sprintf(errMessage,"�����ʵ�ķ�����IP�Ͷ˿������쳣,������Ϣ:%s",errMessage);
// 		logFile.WriteErrorLog("WA",errMessage);
// 		return false;
// 	}
// 	return true;
// }

//�����ʵ�ķ�����IP�Ͷ˿�
bool COperatorWA::GetServerIP(char* gameName, char* SrcServerIP, int gameDBID, char* DestServerIP, int* DestPort, int &ClusterID,int &realm_id)
{
	try
	{
		sprintf(szSql,"exec WA_FindGateWayPort '%s',%i",SrcServerIP,gameDBID);
	
		vector<CGlobalStruct::DataRecord> datarecord;

		m_SqlHelper.SqlHelperMain("WA", &datarecord, connLocalDB, szSql, 4, 1, 1);//��ȡҪ�滻��ֵ
		
		if(datarecord.empty())//�����¼Ϊ��
		{
			return false;
		}
		
		//��ȡ��ѯ�ļ�¼����ÿһ��
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;	
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("ServerIP", m_DataRecord.recordfieldname))//��ȡ��Ӧ��ServerIP
			{
				sprintf(DestServerIP, m_DataRecord.recorddata);//�����ѯ�õ��ĵ�һ����¼
			}
			else if(!strcmp("Port", m_DataRecord.recordfieldname))//��ȡ��Ӧ��Port
			{
				*DestPort = StrToInt(m_DataRecord.recorddata);//�����ѯ�õ��ĵ�һ����¼
			}
			else if(!strcmp("ClusterID", m_DataRecord.recordfieldname))//��ȡ��Ӧ�ķ�������ID
			{
				ClusterID = StrToInt(m_DataRecord.recorddata);//�����ѯ�õ��ĵ�һ����¼
			}
			else if(!strcmp("Realmid", m_DataRecord.recordfieldname))//��ȡ��Ӧ�Ĵ���ID
			{
				realm_id = StrToInt(m_DataRecord.recorddata);//�����ѯ�õ��ĵ�һ����¼
			}
		}
		//printf("COperatorWA::GetServerIP()  %d %d--\n", ClusterID, realm_id);
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
		
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�����ʵ�ķ�����IP�Ͷ˿��ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("WA",errMessage);
		return false;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�����ʵ�ķ�����IP�Ͷ˿��ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("WA",errMessage);
		return false;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�����ʵ�ķ�����IP�Ͷ˿������쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("WA",errMessage);
		return false;
	}
	return true;
}

//���سɹ���ʧ�ܻ�ϵͳ�������Ϣ
vector<CGlobalStruct::TFLV> COperatorWA::ReturnOpMsg(char* operName, char* result,char *funcName)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV tagtflv;
	string strLog;
	try
	{
		tagtflv.m_tagName=CEnumCore::TagName::MESSAGE;
		tagtflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
		sprintf((char *)&tagtflv.lpdata, "%s", result);
		tagtflv.m_tvlength = strlen((char *)&tagtflv.lpdata);
		DBTFLV.push_back(tagtflv);
		strLog = result;
		strLog += "<";
		strLog += funcName; 
		strLog += ">";  
		logFile.WriteLog("<��ʿ��˫>", operName, const_cast<char *>(strLog.c_str()));
	}
	catch(...)
	{
	}
	
	return DBTFLV;
}

//������־��¼
void COperatorWA::GmOperLog(int UserByID, char * lpServerIp, char *OperName, char * Content, CGlobalStruct::TFLV result)
{
	try
	{
		char message[2048];
		char szSql[2048];
		sprintf(message, "�������:<%s> ��ϸ��Ϣ[%s]", result.lpdata, Content);
		sprintf(szSql, "insert into GMTools_log(UserID, SP_Name, Game_Name, ServerIP, Real_Act) values(%d, '%s', '%s', '%s', '%s')",
			UserByID, OperName, "��ʿOL", lpServerIp, message);
		
		m_SqlHelper.SqlHelperMain("127.0.0.1", szSql, 4);
	}
	catch(...)
	{

	}
}
//���뱾�����ݿ�
void COperatorWA::InsertLocalDB(int UserByID, char * lpServerIp, char* city,char* account,char *SouStr, char * DestStr)
{
	try
	{
		char message[2048];
		char szSql[2048];
		sprintf(message, "�������:<�޸�������> ������IP<%s>,������<%s>,�ʺ���<%s>,������<%s>,������<%s>��ϸ��Ϣ[%s]", lpServerIp, city,account,SouStr,DestStr);
		sprintf(szSql, "insert into WA_AccountPwd(ServerIP, City, Account,OldPwd,NewPwd,OperateMan) values('%s', '%s', '%s', '%s', '%s',%d)",
			lpServerIp, city, account,SouStr, DestStr, UserByID);
		
		m_SqlHelper.SqlHelperMain("127.0.0.1", szSql, 4);
	}
	catch(...)
	{
		
	}

}
//����ʱ���ַ���
void COperatorWA::ParseTime(char *time, ADMIN::DATE_TIME &tagTime)
{
	vector<string> list_time;
	char seps[] = "- :";
	char *token;
	token = strtok(time, seps);
	
	while( token != NULL )
	{
		string singletime = token;
		list_time.push_back(singletime);
		token = strtok( NULL, seps );
	}

	if (list_time.size()<6)
	{
		return;
	}

	for (int i=0; i<6; i++)
	{
		unsigned int itempTime = atoi(list_time[i].c_str());
		switch (i)
		{
		case 0:
			tagTime.year = itempTime;
			break;
		case 1:
			tagTime.month = itempTime;
			break;
		case 2:
			tagTime.day = itempTime;
			break;
		case 3:
			tagTime.hour = itempTime;
			break;
		case 4:
			tagTime.minute = itempTime;
			break;
		case 5:
			tagTime.second = itempTime;
			break;
		}
	}
	
	return;
}


