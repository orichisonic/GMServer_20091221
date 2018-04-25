// OperatorGame.cpp: implementation of the COperatorGame class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameIOCP.h"
#include "OperatorGame.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COperatorGame::COperatorGame()
{

}

COperatorGame::~COperatorGame()
{
	SendVectBuf.Destroy();
}

bool COperatorGame::initialize(CSession * pSess)
{
	return true;
}

//��ѯSQL��� iFlag��ʱû���õ�
bool COperatorGame::getRemoteSql(char * GameName,char * lpFlag,char* lpSql,int iFlag)
{
	try
	{
		vector<CGlobalStruct::DataRecord> datarecord;
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
		sprintf(m_sql,"exec GetSql '%s' ", lpFlag);
		//��ӡSQL
		m_SqlHelper.SqlHelperMain(GameName,&datarecord,"127.0.0.1", m_sql, 4, 0, 0);//��ȡSqlExpress�е���Ӧsql���
		
		if(datarecord.empty())//�����¼Ϊ��
		{
			if(iFlag==1)
			{
			}
			return false;
		}
		
		//��ȡ��ѯ�ļ�¼����ÿһ��
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;	
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("sql_statement", m_DataRecord.recordfieldname))//��ȡ��Ӧ��LogDBIP
			{
				sprintf(lpSql, "%s", m_DataRecord.recorddata);
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
		sprintf(errMessage,"��ȡSQL����ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog(GameName,errMessage);
		sprintf(lpSql,"");
		return false;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ȡSQL����ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog(GameName,errMessage);
		sprintf(lpSql,"");
		return false;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ȡSQL��������쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog(GameName,errMessage);
		sprintf(lpSql,"");
		return false;
	}
	return true;
}


//���ָ������һ���ֶ�ֵ
bool COperatorGame::QuerySingleValue(char * GameName,char * lpIP,int flag,char* procName,char* result)
{
	try
	{
		vector<CGlobalStruct::DataRecord> datarecord;
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
		m_SqlHelper.SqlHelperMain(GameName,&datarecord, lpIP, procName, flag, 0, 0);//��ȡ��Ӧ��LogDBIP
		
		logFile.WriteErrorLog(GameName,procName);

		if(datarecord.empty())//�����¼Ϊ��
		{
			sprintf(result,"");
			return false;
		}
		
		//��ȡ��ѯ�ļ�¼����ÿһ��
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;	
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			sprintf(result,"%s",m_DataRecord.recorddata);
			break;
		}
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��õ�һ���ֶ�ֵ�ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog(GameName,errMessage);
		sprintf(result,"");
		return false;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��õ�һ���ֶ�ֵ�ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog(GameName,errMessage);
		sprintf(result,"");
		return false;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��õ�һ���ֶ�ֵ�����쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog(GameName,errMessage);
		sprintf(result,"");
		return false;
	}
	return true;

}


//ͨ��DBID��IP
bool COperatorGame::FindDBIP(char * GameName,char * lpIP,char * lpNewIP,int dbid)
{
	try
	{
		char szSql[256];
		ZeroMemory(szSql,256);
		if(!strcmp(GameName,"JW2"))
		{
			sprintf(szSql,"exec JW2_FindDBIP '%s',%i",lpIP,dbid);
		}
		QuerySingleValue(GameName,"127.0.0.1",4,szSql,lpIP);
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"���IP��ַ�ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog(GameName,errMessage);
		sprintf(lpNewIP,"%s",lpIP);
		return false;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"���IP��ַ�ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog(GameName,errMessage);
		sprintf(lpNewIP,"%s",lpIP);
		return false;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"���IP��ַ�����쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog(GameName,errMessage);
		sprintf(lpNewIP,"%s",lpIP);
		return false;
	}
	return true;
}
bool COperatorGame::SendBuf(CSession * pGameSess,CEnumCore::Msg_Category m_category,CEnumCore::ServiceKey m_servicekey,unsigned char * buf,int length)
{
	try
	{
		int sBodyLength = 0;
		CSocketData SendDataBuf;
		SendDataBuf.Create(length);//���췢�����ݰ�
		SendDataBuf.AddHeader(1, m_category, m_servicekey, length);//������ݰ���ͷ��
		SendDataBuf.EnSendBuf(&sBodyLength, buf, length);//���ܲ����췢�����ݰ�
		pGameSess->Send(SendDataBuf.GetSendBuf(),sBodyLength);
		SendDataBuf.Destory();
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"���������ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("BASE",errMessage);
		return false;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"���������ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("BASE",errMessage);
		return false;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�������������쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("BASE",errMessage);
		return false;
	}
	return true;
}
bool COperatorGame::BuildTLV(COperVector m_OpVect)
{
	try
	{
		vector <CGlobalStruct::TFLV> DBTFLV=m_OpVect.GetInnerList();
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
		sprintf(errMessage,"�������ݰ��ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("BASE",errMessage);
		return false;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�������ݰ��ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("BASE",errMessage);
		return false;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�������ݰ������쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("BASE",errMessage);
		return false;
	}
	return TRUE;
}

bool COperatorGame::BuildTLV(char * message)
{
	try
	{
		SendVectBuf.InitCreate(1, 1);//��ʼ�����͵�����
		SendVectBuf.AddTFLV(CEnumCore::TagName::MESSAGE,CEnumCore::TagFormat::TLV_STRING,strlen(message),(unsigned char*)message);//Ĭ��Ϊֱ�����TFLV
		iEnbodyLength = SendVectBuf.EnBody();
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"������Ϣ�ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("BASE",errMessage);
		return false;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"������Ϣ�ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("BASE",errMessage);
		return false;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"������Ϣ�����쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("BASE",errMessage);
		return false;
	}
	return TRUE;
}
unsigned char * COperatorGame::GetSendBuf()
{
	return SendVectBuf.getDes();
}

int COperatorGame::GetSendLength()
{
	return iEnbodyLength;
}