#include "stdafx.h"
#include ".\OperatorCF.h"

//���캯��
COperatorCF::COperatorCF(void)
{	
}

//��������
COperatorCF::~COperatorCF(void)
{

}
bool COperatorCF::BuildTLV(char * message)
{
	return COperatorGame::BuildTLV(message);
}
bool COperatorCF::BuildTLV(vector <CGlobalStruct::TFLV> DBTFLV,CEnumCore::ServiceKey m_ServiceKey,char * ServerIP)
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
		logFile.WriteErrorLog("CF",errMessage);
		return false;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�������ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("CF",errMessage);
		return false;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�����������쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("CF",errMessage);
		return false;
	}
	return TRUE;
}
//�����ʵ�ķ�����IP�Ͷ˿�
bool COperatorCF::GetServerIP(char* gameName, char* SrcServerIP, int gameDBID, char* DestServerIP, int* DestPort)
{
	try
	{
		sprintf(szSql,"exec CF_FindGateCFyPort '%s',%i",SrcServerIP,gameDBID);
	
		vector<CGlobalStruct::DataRecord> datarecord;

		m_SqlHelper.SqlHelperMain("CF", &datarecord, connLocalDB, szSql, 4, 1, 1);//��ȡҪ�滻��ֵ
		
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
		}
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
		
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�����ʵ�ķ�����IP�Ͷ˿��ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("CF",errMessage);
		return false;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�����ʵ�ķ�����IP�Ͷ˿��ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("CF",errMessage);
		return false;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�����ʵ�ķ�����IP�Ͷ˿������쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("CF",errMessage);
		return false;
	}
	return true;
}


