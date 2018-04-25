#include "stdafx.h"
#include ".\OperatorCF.h"

//构造函数
COperatorCF::COperatorCF(void)
{	
}

//析构函数
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
		sprintf(errMessage,"构建包内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("CF",errMessage);
		return false;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"构建包文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("CF",errMessage);
		return false;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"构建包其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("CF",errMessage);
		return false;
	}
	return TRUE;
}
//获得真实的服务器IP和端口
bool COperatorCF::GetServerIP(char* gameName, char* SrcServerIP, int gameDBID, char* DestServerIP, int* DestPort)
{
	try
	{
		sprintf(szSql,"exec CF_FindGateCFyPort '%s',%i",SrcServerIP,gameDBID);
	
		vector<CGlobalStruct::DataRecord> datarecord;

		m_SqlHelper.SqlHelperMain("CF", &datarecord, connLocalDB, szSql, 4, 1, 1);//获取要替换的值
		
		if(datarecord.empty())//如果记录为空
		{
			return false;
		}
		
		//获取查询的记录集的每一项
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;	
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("ServerIP", m_DataRecord.recordfieldname))//获取相应的ServerIP
			{
				sprintf(DestServerIP, m_DataRecord.recorddata);//保存查询得到的第一条记录
			}
			else if(!strcmp("Port", m_DataRecord.recordfieldname))//获取相应的Port
			{
				*DestPort = StrToInt(m_DataRecord.recorddata);//保存查询得到的第一条记录
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
		sprintf(errMessage,"获得真实的服务器IP和端口内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("CF",errMessage);
		return false;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"获得真实的服务器IP和端口文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("CF",errMessage);
		return false;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"获得真实的服务器IP和端口其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("CF",errMessage);
		return false;
	}
	return true;
}


