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

//查询SQL语句 iFlag暂时没有用到
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
		//打印SQL
		m_SqlHelper.SqlHelperMain(GameName,&datarecord,"127.0.0.1", m_sql, 4, 0, 0);//获取SqlExpress中的相应sql语句
		
		if(datarecord.empty())//如果记录为空
		{
			if(iFlag==1)
			{
			}
			return false;
		}
		
		//获取查询的记录集的每一项
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;	
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("sql_statement", m_DataRecord.recordfieldname))//获取相应的LogDBIP
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
		sprintf(errMessage,"获取SQL语句内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog(GameName,errMessage);
		sprintf(lpSql,"");
		return false;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"获取SQL语句文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog(GameName,errMessage);
		sprintf(lpSql,"");
		return false;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"获取SQL语句其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog(GameName,errMessage);
		sprintf(lpSql,"");
		return false;
	}
	return true;
}


//获得指定语句第一个字段值
bool COperatorGame::QuerySingleValue(char * GameName,char * lpIP,int flag,char* procName,char* result)
{
	try
	{
		vector<CGlobalStruct::DataRecord> datarecord;
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
		m_SqlHelper.SqlHelperMain(GameName,&datarecord, lpIP, procName, flag, 0, 0);//获取对应的LogDBIP
		
		logFile.WriteErrorLog(GameName,procName);

		if(datarecord.empty())//如果记录为空
		{
			sprintf(result,"");
			return false;
		}
		
		//获取查询的记录集的每一项
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
		sprintf(errMessage,"获得第一个字段值内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog(GameName,errMessage);
		sprintf(result,"");
		return false;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"获得第一个字段值文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog(GameName,errMessage);
		sprintf(result,"");
		return false;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"获得第一个字段值其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog(GameName,errMessage);
		sprintf(result,"");
		return false;
	}
	return true;

}


//通过DBID查IP
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
		sprintf(errMessage,"获得IP地址内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog(GameName,errMessage);
		sprintf(lpNewIP,"%s",lpIP);
		return false;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"获得IP地址文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog(GameName,errMessage);
		sprintf(lpNewIP,"%s",lpIP);
		return false;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"获得IP地址其他异常,错误信息:%s",errMessage);
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
		SendDataBuf.Create(length);//构造发送数据包
		SendDataBuf.AddHeader(1, m_category, m_servicekey, length);//添加数据包的头部
		SendDataBuf.EnSendBuf(&sBodyLength, buf, length);//加密并构造发送数据包
		pGameSess->Send(SendDataBuf.GetSendBuf(),sBodyLength);
		SendDataBuf.Destory();
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"发送数据内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("BASE",errMessage);
		return false;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"发送数据文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("BASE",errMessage);
		return false;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"发送数据其他异常,错误信息:%s",errMessage);
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
		SendVectBuf.InitCreate(DBTFLV.size()+1, 1);//初始化发送的数据
		
		vector<CGlobalStruct::TFLV>::iterator iter;
		for( iter = DBTFLV.begin(); iter != DBTFLV.end(); iter++ )
		{
			CGlobalStruct::TFLV m_tflv= *iter;
			SendVectBuf.AddTFLV(m_tflv);//默认为直接添加TFLV
			
		}
		iEnbodyLength = SendVectBuf.EnBody();
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"构建数据包内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("BASE",errMessage);
		return false;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"构建数据包文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("BASE",errMessage);
		return false;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"构建数据包其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("BASE",errMessage);
		return false;
	}
	return TRUE;
}

bool COperatorGame::BuildTLV(char * message)
{
	try
	{
		SendVectBuf.InitCreate(1, 1);//初始化发送的数据
		SendVectBuf.AddTFLV(CEnumCore::TagName::MESSAGE,CEnumCore::TagFormat::TLV_STRING,strlen(message),(unsigned char*)message);//默认为直接添加TFLV
		iEnbodyLength = SendVectBuf.EnBody();
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"构建消息内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("BASE",errMessage);
		return false;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"构建消息文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("BASE",errMessage);
		return false;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"构建消息其他异常,错误信息:%s",errMessage);
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