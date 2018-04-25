#include "stdafx.h"
#include ".\OperatorWA.h"

//构造函数
COperatorWA::COperatorWA(void)
{	
}

//析构函数
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
		logFile.WriteErrorLog("WA",errMessage);
		return false;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"构建包文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("WA",errMessage);
		return false;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"构建包其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("WA",errMessage);
		return false;
	}
	return TRUE;
}
//获得真实的服务器组和区域组ID
// bool COperatorWA::GetClusterID(char* gameName, char* ServerIP, int gameDBID, char* city, int &ClusterID,int &realm_id)
// {
// 	try
// 	{
// 		sprintf(szSql,"exec WA_ClusterID_Query '%s','%s',%i",ServerIP,city,gameDBID);
// 		
// 		vector<CGlobalStruct::DataRecord> datarecord;
// 		
// 		m_SqlHelper.SqlHelperMain("WA", &datarecord, connLocalDB, szSql, 4, 1, 1);//获取要替换的值
// 		
// 		if(datarecord.empty())//如果记录为空
// 		{
// 			return false;
// 		}
// 		
// 		//获取查询的记录集的每一项
// 		vector<CGlobalStruct::DataRecord>::iterator iter;
// 		CGlobalStruct::DataRecord  m_DataRecord;	
// 		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
// 		{
// 			m_DataRecord = *iter;
// 			if(!strcmp("ClusterID", m_DataRecord.recordfieldname))//获取相应的ServerIP
// 			{
// 				realm_id = StrToInt(m_DataRecord.recorddata);//保存查询得到的第一条记录
// 			}
// 			else if(!strcmp("Realmid", m_DataRecord.recordfieldname))//获取相应的Port
// 			{
// 				ClusterID = StrToInt(m_DataRecord.recorddata);//保存查询得到的第一条记录
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
// 		sprintf(errMessage,"获得真实的服务器组和区域组ID内存不足,错误信息:%s",errMessage);
// 		logFile.WriteErrorLog("WA",errMessage);
// 		return false;
// 	}
// 	catch (CFileException* e)
// 	{
// 		e->GetErrorMessage(errMessage,256);
// 		sprintf(errMessage,"获得真实的服务器组和区域组ID文件异常,错误信息:%s",errMessage);
// 		logFile.WriteErrorLog("WA",errMessage);
// 		return false;
// 	}
// 	catch (CException* e)
// 	{
// 		e->GetErrorMessage(errMessage,256);
// 		sprintf(errMessage,"获得真实的服务器IP和端口其他异常,错误信息:%s",errMessage);
// 		logFile.WriteErrorLog("WA",errMessage);
// 		return false;
// 	}
// 	return true;
// }

//获得真实的服务器IP和端口
bool COperatorWA::GetServerIP(char* gameName, char* SrcServerIP, int gameDBID, char* DestServerIP, int* DestPort, int &ClusterID,int &realm_id)
{
	try
	{
		sprintf(szSql,"exec WA_FindGateWayPort '%s',%i",SrcServerIP,gameDBID);
	
		vector<CGlobalStruct::DataRecord> datarecord;

		m_SqlHelper.SqlHelperMain("WA", &datarecord, connLocalDB, szSql, 4, 1, 1);//获取要替换的值
		
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
			else if(!strcmp("ClusterID", m_DataRecord.recordfieldname))//获取相应的服务器组ID
			{
				ClusterID = StrToInt(m_DataRecord.recorddata);//保存查询得到的第一条记录
			}
			else if(!strcmp("Realmid", m_DataRecord.recordfieldname))//获取相应的大区ID
			{
				realm_id = StrToInt(m_DataRecord.recorddata);//保存查询得到的第一条记录
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
		sprintf(errMessage,"获得真实的服务器IP和端口内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("WA",errMessage);
		return false;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"获得真实的服务器IP和端口文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("WA",errMessage);
		return false;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"获得真实的服务器IP和端口其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("WA",errMessage);
		return false;
	}
	return true;
}

//返回成功、失败或系统错误的信息
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
		logFile.WriteLog("<勇士无双>", operName, const_cast<char *>(strLog.c_str()));
	}
	catch(...)
	{
	}
	
	return DBTFLV;
}

//操作日志记录
void COperatorWA::GmOperLog(int UserByID, char * lpServerIp, char *OperName, char * Content, CGlobalStruct::TFLV result)
{
	try
	{
		char message[2048];
		char szSql[2048];
		sprintf(message, "操作结果:<%s> 详细信息[%s]", result.lpdata, Content);
		sprintf(szSql, "insert into GMTools_log(UserID, SP_Name, Game_Name, ServerIP, Real_Act) values(%d, '%s', '%s', '%s', '%s')",
			UserByID, OperName, "勇士OL", lpServerIp, message);
		
		m_SqlHelper.SqlHelperMain("127.0.0.1", szSql, 4);
	}
	catch(...)
	{

	}
}
//插入本地数据库
void COperatorWA::InsertLocalDB(int UserByID, char * lpServerIp, char* city,char* account,char *SouStr, char * DestStr)
{
	try
	{
		char message[2048];
		char szSql[2048];
		sprintf(message, "操作结果:<修改新密码> 服务器IP<%s>,大区名<%s>,帐号名<%s>,旧密码<%s>,新密码<%s>详细信息[%s]", lpServerIp, city,account,SouStr,DestStr);
		sprintf(szSql, "insert into WA_AccountPwd(ServerIP, City, Account,OldPwd,NewPwd,OperateMan) values('%s', '%s', '%s', '%s', '%s',%d)",
			lpServerIp, city, account,SouStr, DestStr, UserByID);
		
		m_SqlHelper.SqlHelperMain("127.0.0.1", szSql, 4);
	}
	catch(...)
	{
		
	}

}
//解析时间字符串
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


