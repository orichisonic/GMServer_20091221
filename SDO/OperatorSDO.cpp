#include "stdafx.h"
#include ".\OperatorSDO.h"

//构造函数
COperatorSDO::COperatorSDO(void)
{
}

//析构函数
COperatorSDO::~COperatorSDO(void)
{
	SendVectBuf.Destroy();
}
//构建数据包
bool COperatorSDO::BuildTLV(vector <CGlobalStruct::TFLV> DBTFLV,CEnumCore::ServiceKey m_ServiceKey,char * ServerIP)
{
	try
	{
		if(DBTFLV.empty())
		{
			return false;
		}
		SendVectBuf.InitCreate(DBTFLV.size()+1, 1);//初始化发送的数据
		UINT tmpServiceKey = (UINT)m_ServiceKey;//保存临时的ServiceKey
		char strResult[256];
		vector<CGlobalStruct::TFLV>::iterator iter;
		for( iter = DBTFLV.begin(); iter != DBTFLV.end(); iter++ )
		{	
			CGlobalStruct::TFLV m_tflv= *iter;
			if(m_tflv.m_tagName == CEnumCore::TagName::SDO_Level)
			{
				char UserLvl[10];
				ZeroMemory(UserLvl,10);
				CGlobalStruct::TFLV tflv_Exp = *(iter+1);
				int exp = StrToInt((LPCSTR)tflv_Exp.lpdata);//获取玩家的经验值
				if(!getRemoteSql("SDO", "SDO_LevelExp_Query",m_sql,0))//获取查询玩家等级的sql语句
				{
					sprintf(UserLvl,"%s","0");
				}
				else
				{
					QuerySingleValue(UserLvl, "", "127.0.0.1", 0, m_sql, exp);//获取等级
				}				
				SendVectBuf.AddTFLV(CEnumCore::TagName::SDO_Level,CEnumCore::TagFormat::TLV_STRING,strlen(UserLvl),(unsigned char*)&UserLvl);
			}
			else if(m_tflv.m_tagName == CEnumCore::TagName::SDO_ItemName)
			{				
				CGlobalStruct::TFLV tflv_ItemCode = *(iter-1);
				int ItemCode=StrToInt((LPCSTR)tflv_ItemCode.lpdata);//获取玩家的的道具code
				char ItemName[128];				
				ZeroMemory(ItemName,128);
				sprintf(ItemName,(LPCSTR)m_tflv.lpdata);//获取玩家的的道具名称
				if(!strcmp("",ItemName) || !strcmp("NULL",ItemName))
				{					
					switch (tmpServiceKey)
					{
					case CEnumCore::ServiceKey::SDO_MEDALITEM_QUERY_RESP:
					case CEnumCore::ServiceKey::SDO_MEDALITEM_OWNER_QUERY_RESP:
						{
							if(getRemoteSql("SDO", "SDO_Get_MedalName",m_sql,0))//获取查询勋章真实名称的sql语句
							{
								QuerySingleValue(ItemName, "", "127.0.0.1", 0, m_sql,ItemCode);//获取勋章名称
							}	
						}
						break;
					default:
						GetItemName(ItemName,ItemCode);//获取玩家道具名称
						break;
					}					
			    	SendVectBuf.AddTFLV(CEnumCore::TagName::SDO_ItemName,CEnumCore::TagFormat::TLV_STRING,strlen(ItemName),(unsigned char*)&ItemName);
				}
				else
				{
					SendVectBuf.AddTFLV(m_tflv);//直接添加TFLV
				}
			}
			else if(m_tflv.m_tagName == CEnumCore::TagName::SDO_ItemName1)
			{
				
				CGlobalStruct::TFLV tflv_ItemCode = *(iter-1);
				int ItemCode=StrToInt((LPCSTR)tflv_ItemCode.lpdata);//获取玩家的的道具code
				char ItemName[128];				
				ZeroMemory(ItemName,128);
				sprintf(ItemName,(LPCSTR)m_tflv.lpdata);//获取玩家的的道具名称
				if(!strcmp("",ItemName) || !strcmp("NULL",ItemName))
				{
					GetItemName(ItemName,ItemCode);//获取玩家道具名称
					SendVectBuf.AddTFLV(CEnumCore::TagName::SDO_ItemName1,CEnumCore::TagFormat::TLV_STRING,strlen(ItemName),(unsigned char*)&ItemName);
				}
				else
				{
					SendVectBuf.AddTFLV(m_tflv);//直接添加TFLV
				}
			}
			else if(m_tflv.m_tagName == CEnumCore::TagName::SDO_ItemName2)
			{
				CGlobalStruct::TFLV tflv_ItemCode = *(iter-1);
				int ItemCode=StrToInt((LPCSTR)tflv_ItemCode.lpdata);//获取玩家的的道具code
				char ItemName[128];				
				ZeroMemory(ItemName,128);
				sprintf(ItemName,(LPCSTR)m_tflv.lpdata);//获取玩家的的道具名称
				if(!strcmp("",ItemName) || !strcmp("NULL",ItemName))
				{
					GetItemName(ItemName,ItemCode);//获取玩家道具名称
					SendVectBuf.AddTFLV(CEnumCore::TagName::SDO_ItemName2,CEnumCore::TagFormat::TLV_STRING,strlen(ItemName),(unsigned char*)&ItemName);
				}
				else
				{
					SendVectBuf.AddTFLV(m_tflv);//直接添加TFLV
				}
			}
			else if(m_tflv.m_tagName == CEnumCore::TagName::SDO_Precent)
			{
				CGlobalStruct::TFLV tflv_ItemCode = *(iter-2);
				int ItemCode=StrToInt((LPCSTR)tflv_ItemCode.lpdata);//获取玩家的的道具code
				if(tmpServiceKey == CEnumCore::ServiceKey::SDO_MEDALITEM_OWNER_QUERY_RESP)
				{
					int perc = 0;
					char tempIP[20];
					if(!GetServerIP(tempIP,ServerIP,2))
					{
						return FALSE;//无法获取真正的服务器IP
					}

					if(getRemoteSql("SDO","SDO_MedalItemPerc_Query", m_sql, 0))			//获取查询道具获取比率的sql语句
					{
						printf(m_sql);
						QuerySingleValue(&perc, "SDO", tempIP, 0, m_sql, ItemCode);		//获取道具获取比率	
					}	

					char StrInt[10];
					ZeroMemory(StrInt,10);
					_itoa(perc, StrInt, 10);//获取道具比率
					SendVectBuf.AddTFLV(CEnumCore::TagName::SDO_Precent,CEnumCore::TagFormat::TLV_INTEGER,sizeof(int),(unsigned char*)&StrInt);
				}
				else
				{
					SendVectBuf.AddTFLV(m_tflv);//直接添加TFLV
				}
			}
			else
			{
				try
				{
					SendVectBuf.AddTFLV(m_tflv);//默认为直接添加TFLV
				}
				catch(...)
				{
				}		
			}
		}				
		iEnbodyLength = SendVectBuf.EnBody();
		}
		catch (CMemoryException* e)
		{
			logFile.WriteErrorLog("SDO","构建数据包内存不足");
			return false;
		}
		catch (CFileException* e)
		{
			logFile.WriteErrorLog("SDO","构建数据包文件异常");
			return false;
		}
		catch (CException* e)
		{
			logFile.WriteErrorLog("SDO","构建数据包其他异常");
			return false;
		}
		catch (runtime_error err)
		{
			logFile.WriteErrorLog("SDO",(char *)err.what());
			return false;
		}
		catch (...)
		{
			return false;
		}
		return TRUE;
}

bool COperatorSDO::BuildTLV(char * message)
{
	return COperatorGame::BuildTLV(message);
}

//查询替换单个值
void COperatorSDO::QuerySingleValue(char* SingleValue, char* gamename, char* ServerIP, int nFlag, const char * szFormat, ...)
{
	try
	{
		va_list vaList;
		va_start(vaList, szFormat);
		vsprintf(szSql, szFormat, vaList);//获取整个sql语句
		va_end( vaList );

		vector<CGlobalStruct::DataRecord> datarecord;
		if(strcmp("",gamename))
		{
			m_SqlHelper.SqlHelperMain(gamename, &datarecord, ServerIP, szSql, nFlag, 1, 1);//获取要替换的值
		}
		else
		{
			m_SqlHelper.SqlHelperMain(&datarecord, ServerIP, szSql, nFlag, 1, 1);//获取要替换的值
		}		
		
		if(datarecord.empty())//如果记录为空
		{
			sprintf(SingleValue,"");
			return;
		}
		
		//获取查询的记录集的每一项
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;	
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("name", m_DataRecord.recordfieldname))//获取相应的LogDBIP
			{
		    	sprintf(SingleValue,"%s", m_DataRecord.recorddata);//保存查询得到的第一条记录
			}
		}
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
	}
	catch (...)
	{
	}
}

//获取int型的单个值
void COperatorSDO::QuerySingleValue(int* SingleValue, char* gamename, char* ServerIP, int nFlag, const char * szFormat, ...)
{
	try
	{
		va_list vaList;
		va_start(vaList, szFormat);
		vsprintf(szSql, szFormat, vaList);//获取整个sql语句
		va_end( vaList );
		
		vector<CGlobalStruct::DataRecord> datarecord;
		if(strcmp("",gamename))
		{
			m_SqlHelper.SqlHelperMain(gamename, &datarecord, ServerIP, szSql, nFlag, 1, 1);//获取要替换的值
		}
		else
		{
			m_SqlHelper.SqlHelperMain(&datarecord, ServerIP, szSql, nFlag, 1, 1);//获取要替换的值
		}		
		
		if(datarecord.empty())//如果记录为空
		{
			*SingleValue = 0;
			return;
		}
		
		//获取查询的记录集的每一项
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;	
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("name", m_DataRecord.recordfieldname))//获取相应的LogDBIP
			{
				*SingleValue = StrToInt(m_DataRecord.recorddata);//保存查询得到的第一条记录
			}
		}
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
	}
	catch (...)
	{
	}
}

//查找字符串中特定的字符第一次出现的位置
int COperatorSDO::StringFind(const char* string,const char* find,int number)
{
	char* pos = (char *)string;
	char *p=pos;
	int count = 0;
	while (number > 0)
	{
		//定义查找到的字符位置的指针，以便临时指针进行遍历
		pos = strstr(p,find);
		//当位置指针为0时，说明没有找到这个字符
		if (pos == 0)
			return -1;
		//当位置指针和临时指针相等说明下一个字符就是要找的字符，如果临时指针小于位置指针，则进行遍历字符串操作，并将count增1
		while(p <= pos)
		{
			p++;
			count++;
		}
		//对要查找的次数减一
		number--;
	}
	return count;
}

//获取选定字符前面的字符串
bool COperatorSDO::strGetChar(const char* string, const char* find, char *strRet, int retLength, int number)
{
	try
	{
		ZeroMemory(strRet, retLength);
		int charPos = 0;
		int charPosPrev = 0;
		if(number <= 0)
		{
			return false;
		}
		else if(number == 1)
		{
			charPos = StringFind(string, find, number);
			if(charPos == -1)
			{
				return false;
			}
			memcpy(strRet, string, charPos-1);//获取足够长度的字符串
		}
		else
		{
			charPos = StringFind(string, find, number);
			if(charPos == -1)
			{
				return false;
			}
			charPosPrev = StringFind(string, find, number-1);
			memcpy(strRet, (char*)&string[charPosPrev], charPos-charPosPrev-1);//获取足够长度的字符串
		}
	}
	catch (...)
	{
		return false;
	}
	return true;
}

//获取记入日志中的信息
bool COperatorSDO::GetLogText(char* retMsg, char* baseMsg, char* diffMsg, char* Result, ...)
{
	try
	{
		ZeroMemory(strMsg, 2048);

		char tmpBaseMsg[2048];
		char tmpDiffMsg[128];
		char tmpResult[128];
		ZeroMemory(tmpBaseMsg, 2048);
		ZeroMemory(tmpDiffMsg, 128);
		ZeroMemory(tmpResult, 128);		

		logFile.ReadValue("SDO", "Message", baseMsg, tmpBaseMsg, 2048);//获取相应的基础信息
		sprintf(retMsg, "%s", tmpBaseMsg);//获取基础操作信息

		va_list vaList;
		va_start(vaList, Result);
		vsprintf(retMsg, tmpBaseMsg, vaList);//获取返回语句
		va_end( vaList );

		if(strcmp("",diffMsg))//特殊信息如果不为空
		{
			logFile.ReadValue("SDO", "Message", diffMsg, tmpDiffMsg, 128);//获取相应的特殊信息
			sprintf(strMsg, "%s", retMsg);//获取基础操作信息
			sprintf(retMsg, "%s%s", strMsg,tmpDiffMsg);//基础操作信息、特殊操作信息合并
		}
		if(strcmp("",Result))//如果最后的操作结果不为空
		{
			logFile.ReadValue("SDO", "Message", Result, tmpResult, 128);//获取相应的结果信息
			sprintf(strMsg, "%s", retMsg);//获取基础操作信息
			sprintf(retMsg, "%s,%s", strMsg, tmpResult);//基础操作信息、特殊操作信息、操作结果合并
		}		
	}
	catch (...)
	{
	}
	return false;
}

//写日志信息
bool COperatorSDO::WriteText(char* gameName, char* strLog)
{
	try
	{
		logFile.ReadValue("SDO", "GameName", gameName, strMsg, 2048);//获取真正的游戏名称		
		logFile.WriteText(strMsg, strLog);
	}
	catch(...)
	{
	}
	return false;
}

//返回成功、失败或系统错误的信息
vector<CGlobalStruct::TFLV> COperatorSDO::ReturnOpMsg(char* strLog, char* result)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	CGlobalStruct::TFLV tagtflv;
	tagtflv.m_tagName=CEnumCore::TagName::MESSAGE;
	tagtflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;

	try
	{
		char tmpResult[128];
		ZeroMemory(tmpResult, 128);//初始化临时结果信息
		logFile.ReadValue("SDO", "Message", result, tmpResult, 128);//获取成功、失败或发生系统错误的信息
		sprintf(strMsg, "%s", strLog);
		sprintf(strLog, "%s,%s", strMsg, tmpResult);//保存最终的日志信息
		WriteText("SDO",strLog);//将信息写入日志中
		
		sprintf((char *)&tagtflv.lpdata, "%s", tmpResult);
	}
	catch(...)
	{
	}
	
	tagtflv.m_tvlength = strlen((char *)&tagtflv.lpdata);
	DBTFLV.push_back(tagtflv);
	return DBTFLV;
}

//将日志信息写入数据库
bool COperatorSDO::WriteDBLog(int userByID, char* gameName, char* sp_Name, char* serverIP, char* strLog)
{
	try
	{
		logFile.ReadValue("SDO", "GameName", gameName, strMsg, 2048);//获取真正的游戏名称		
		logFile.WriteDBLog(userByID, strMsg, sp_Name, serverIP, strLog);//将日志信息写入数据库
	}
	catch(...)
	{
	}
	return false;
}

//获得真实的服务器IP
bool COperatorSDO::GetServerIP(char* DestServerIP, char* SrcServerIP, int gameDBID)
{
	try
	{
		if(!getRemoteSql("SDO","SDO_GetServerIP",m_sql,1))
		{
			return false;
		}
		QuerySingleValue(DestServerIP, "", "127.0.0.1", 0, m_sql, SrcServerIP ,gameDBID);//获取真正的服务器IP

	}
	catch (...)
	{
		return false;
	}
	return true;
}

//根据道具code查询道具名称
bool COperatorSDO::GetItemName(char* ItemName, int ItemCode)
{
	try
	{
		if(getRemoteSql("SDO","SDO_GetRealItemName",m_sql,0))//获取查询玩家道具真实名称的sql语句
		{
			QuerySingleValue(ItemName, "", "127.0.0.1", 0, m_sql,ItemCode);//获取玩家道具名称
		}		
	}
	catch (...)
	{
		return false;
	}
	return true;
}

//将第二个查询得到的相关内容备份到第一个数据库中
bool COperatorSDO::BackupDataBase(char* DestGame,char* DestServerIP,int Flag,char* DestDBName,char* SrcGame,char* SrcServerIP,int nFlag,char* SqlCondition)
{
	try
	{
		vector<CGlobalStruct::DataRecord> datarecord;
		if(strcmp("",SrcGame))
		{
			m_SqlHelper.SqlHelperMain(SrcGame, &datarecord, SrcServerIP, SqlCondition, nFlag, 1, 1);//获取所有要查询备份的值
		}
		else
		{
			m_SqlHelper.SqlHelperMain(&datarecord, SrcServerIP, SqlCondition, nFlag, 1, 1);//获取所有要查询备份的值
		}
		
		
		if(datarecord.empty())//如果记录为空
		{
			return true;
		}

		ZeroMemory(strMsg,2048);
		char tmpSql[1024];
		char tempSql[1024];
		ZeroMemory(tmpSql,1024);
		ZeroMemory(tempSql,1024);
		if(!getRemoteSql("SDO","SDO_InsertIntoTemp",tmpSql,1))
		{
			return false;//无法得到将信息插入临时表的sql语句
		}
		sprintf(strMsg,tmpSql,DestDBName);//构造完整查询的sql语句

		int valueNum=0;//用于判断在sql语句中要不要加","号
		//获取查询的记录集的每一项
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;	
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(strcmp("PageCount",m_DataRecord.recordfieldname))
			{
				if(valueNum == 0)
				{
					sprintf(tmpSql,"%s",strMsg);
					switch(m_DataRecord.recordfieldtype)
					{
					case 200:
						sprintf(tempSql,"%s'%s'",tmpSql,m_DataRecord.recorddata);//构造完整查询的sql语句
						break;
					case 4:
						sprintf(tempSql,"%s%s",tmpSql,m_DataRecord.recorddata);//构造完整查询的sql语句
						break;
					case 135:
						sprintf(tempSql,"%s'%s'",tmpSql,m_DataRecord.recorddata);//构造完整查询的sql语句
						break;
					case 17:
						sprintf(tempSql,"%s%s",tmpSql,m_DataRecord.recorddata);//构造完整查询的sql语句
						break;
					case 11:
					case 12:
						if(strcmp(m_DataRecord.recorddata, "NULL"))
						{
							sprintf(tempSql,"%s,'%s'",tmpSql,m_DataRecord.recorddata);//构造完整查询的sql语句
						}
						else
						{
							sprintf(tempSql,"%s,%s",tmpSql,m_DataRecord.recorddata);//构造完整查询的sql语句
						}
						break;
					default:
						sprintf(tempSql,"%s'%s'",tmpSql,m_DataRecord.recorddata);//构造完整查询的sql语句
						break;
					}					
					valueNum++;
				}
				else
				{
					sprintf(tmpSql,"%s",tempSql);
					switch(m_DataRecord.recordfieldtype)
					{
					case 200:
						sprintf(tempSql,"%s,'%s'",tmpSql,m_DataRecord.recorddata);//构造完整查询的sql语句
						break;
					case 4:
						sprintf(tempSql,"%s,%s",tmpSql,m_DataRecord.recorddata);//构造完整查询的sql语句
						break;
					case 135:
						sprintf(tempSql,"%s,'%s'",tmpSql,m_DataRecord.recorddata);//构造完整查询的sql语句
						break;
					case 17:
						sprintf(tempSql,"%s,%s",tmpSql,m_DataRecord.recorddata);//构造完整查询的sql语句
						break;
					case 11:
					case 12:
						if(strcmp(m_DataRecord.recorddata, "NULL"))
						{
							sprintf(tempSql,"%s,'%s'",tmpSql,m_DataRecord.recorddata);//构造完整查询的sql语句
						}
						else
						{
							sprintf(tempSql,"%s,%s",tmpSql,m_DataRecord.recorddata);//构造完整查询的sql语句
						}
						break;
					default:
						sprintf(tempSql,"%s,'%s'",tmpSql,m_DataRecord.recorddata);//构造完整查询的sql语句
						break;
					}
					valueNum++;					
				}
			}
			else
			{
				sprintf(tmpSql,"%s",tempSql);
				sprintf(tempSql,"%s)",tmpSql);//构造完整查询的sql语句
				valueNum=0;	
				if(strcmp("",DestGame))
				{
					if(!(m_SqlHelper.SqlHelperMain(DestGame, DestServerIP, tempSql, Flag)))//执行插入操作
					{
						BuildTLV("ExecuteSqlWrong");
						return false;
					}
				}
				else
				{
					if(!(m_SqlHelper.SqlHelperMain(DestServerIP, tempSql, Flag)))//执行插入操作
					{
						BuildTLV("ExecuteSqlWrong");
						return false;
					}
				}
			}
		}
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
	}
	catch (...)
	{
	}
	return true;
}

//备份修改之前的信息
bool COperatorSDO::BakUpdateInfo(int userByID, char* spName, char* game, char* serverIP,int Flag, char* DBName, char* QueryFlag, char* UpdateFlag, int ID1, int ID2)
{
	try
	{
		if(!getRemoteSql("SDO",QueryFlag,m_sql,0))//无法得到备份的sql语句
		{
			return false;
		}

		if(strcmp("",DBName) && (ID1>=0) && (ID2>=0))
		{
			sprintf(szSql, m_sql, DBName, ID1, ID2);
			sprintf(m_sql,"%s",szSql);
		}
		else if(strcmp("",DBName) && (ID1>=0) && (ID2<0))
		{
			sprintf(szSql, m_sql, DBName, ID1);
			sprintf(m_sql,"%s",szSql);
		}
		else if(strcmp("",DBName) && (ID1<0) && (ID2<0))
		{
			sprintf(szSql, m_sql, DBName);
			sprintf(m_sql,"%s",szSql);
		}
		else if(!strcmp("",DBName) && (ID1>=0) && (ID2>=0))
		{
			sprintf(szSql, m_sql, ID1, ID2);
			sprintf(m_sql,"%s",szSql);
		}
		else if(!strcmp("",DBName) && (ID1>=0) && (ID2<0))
		{
			sprintf(szSql, m_sql, ID1);
			sprintf(m_sql,"%s",szSql);
		}
		
		vector<CGlobalStruct::DataRecord> datarecord;
		if(strcmp("",game))
		{
			m_SqlHelper.SqlHelperMain(game, &datarecord, serverIP, szSql, Flag, 1, 1);//获取要替换的值
		}
		else
		{
			m_SqlHelper.SqlHelperMain(&datarecord, serverIP, szSql, Flag, 1, 1);//获取要替换的值
		}		
		
		if(datarecord.empty())//如果记录为空
		{
			return false;
		}

		if(!getRemoteSql("SDO",UpdateFlag,m_sql,0))//无法得到备份的sql语句
		{
			return false;
		}

		if(strcmp("",DBName))
		{
			sprintf(szSql, "%s,%s", m_sql, DBName);
			sprintf(m_sql,"%s",szSql);
		}
		
		int valueNum = 0;//用于判断是第几个值
		int SqlNum = 0;//用于判断是第几个sql语句
		char tmpSql[1024];//临时保存查询得到的更新语句
		char BakSql[1024];//保存最后的备份语句
		ZeroMemory(tmpSql,1024);
		ZeroMemory(BakSql,1024);
		
		//获取查询的记录集的每一项
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;	
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(strcmp("PageCount",m_DataRecord.recordfieldname))
			{
				if(valueNum == 0)
				{
					sprintf(tmpSql,"%s,%s", m_sql,m_DataRecord.recorddata);
					valueNum++;
				}
				else
				{
					sprintf(szSql,"%s,%s", tmpSql,m_DataRecord.recorddata);
					sprintf(tmpSql,"%s",szSql);//构造完整查询的sql语句
					valueNum++;				
				}
			}
			else
			{
				if(SqlNum == 0)
				{
					sprintf(BakSql,"%s",tmpSql);//构造完整查询的sql语句
				}
				else
				{
					sprintf(BakSql,"%s;%s",BakSql,tmpSql);//构造完整查询的sql语句
				}
				SqlNum++;//sql语句增加
				valueNum=0;	//值的数量置0
			}
		}
		if(!datarecord.empty())
		{
			datarecord.clear();
		}		
		logFile.WriteBakLog(userByID, spName, serverIP, BakSql);//将日志信息写入数据库
	}
	catch (...)
	{
	}
	return false;
}

//备份删除之前的信息
bool COperatorSDO::BakDeleteInfo(int userByID, char* spName, char* game, char* serverIP,int Flag, char* DBName, char* TableName, char* QueryFlag, char* InsertFlag, int ID1, int ID2)
{
	try
	{
		if(!getRemoteSql("SDO",QueryFlag,m_sql,0))//无法得到备份的sql语句
		{
			return false;
		}

		if(strcmp("",DBName) && strcmp("",TableName) && (ID1>=0) && (ID2>=0))
		{
			sprintf(szSql, m_sql, DBName, TableName, ID1, ID2);
			sprintf(m_sql,"%s",szSql);
		}
		else if(strcmp("",DBName) && strcmp("",TableName) && (ID1>=0) && (ID2<0))
		{
			sprintf(szSql, m_sql, DBName, TableName, ID1);
			sprintf(m_sql,"%s",szSql);
		}
		else if(strcmp("",DBName) && strcmp("",TableName) && (ID1<0) && (ID2<0))
		{
			sprintf(szSql, m_sql, DBName, TableName);
			sprintf(m_sql,"%s",szSql);
		}
		else if(strcmp("",DBName) && !strcmp("",TableName) && (ID1>=0) && (ID2>=0))
		{
			sprintf(szSql, m_sql, DBName, ID1, ID2);
			sprintf(m_sql,"%s",szSql);
		}
		else if(strcmp("",DBName) && !strcmp("",TableName) && (ID1>=0) && (ID2<0))
		{
			sprintf(szSql, m_sql, DBName, ID1);
			sprintf(m_sql,"%s",szSql);
		}
		else if(strcmp("",DBName) && !strcmp("",TableName) && (ID1<0) && (ID2<0))
		{
			sprintf(szSql, m_sql, DBName);
			sprintf(m_sql,"%s",szSql);
		}
		else if(!strcmp("",DBName) && strcmp("",TableName) && (ID1>=0) && (ID2>=0))
		{
			sprintf(szSql, m_sql, TableName, ID1, ID2);
			sprintf(m_sql,"%s",szSql);
		}
		else if(!strcmp("",DBName) && strcmp("",TableName) && (ID1>=0) && (ID2<0))
		{
			sprintf(szSql, m_sql, TableName, ID1);
			sprintf(m_sql,"%s",szSql);
		}
		else if(!strcmp("",DBName) && strcmp("",TableName) && (ID1<0) && (ID2<0))
		{
			sprintf(szSql, m_sql, TableName);
			sprintf(m_sql,"%s",szSql);
		}
		else if(!strcmp("",DBName) && !strcmp("",TableName) && (ID1>=0) && (ID2>=0))
		{
			sprintf(szSql, m_sql, ID1, ID2);
			sprintf(m_sql,"%s",szSql);
		}
		else if(!strcmp("",DBName) && !strcmp("",TableName) && (ID1>=0) && (ID2<0))
		{
			sprintf(szSql, m_sql, ID1);
			sprintf(m_sql,"%s",szSql);
		}
		
		vector<CGlobalStruct::DataRecord> datarecord;
		if(strcmp("",game))
		{
			m_SqlHelper.SqlHelperMain(game, &datarecord, serverIP, szSql, Flag, 1, 1);//获取要替换的值
		}
		else
		{
			m_SqlHelper.SqlHelperMain(&datarecord, serverIP, szSql, Flag, 1, 1);//获取要替换的值
		}		
		
		if(datarecord.empty())//如果记录为空
		{
			return false;
		}
		
		if(!getRemoteSql("SDO",InsertFlag,m_sql,0))//无法得到备份的sql语句
		{
			return false;
		}
		
		if(strcmp("",DBName) && strcmp("",TableName))
		{
			sprintf(szSql, m_sql, DBName, TableName);
			sprintf(m_sql,"%s",szSql);
		}
		else if(strcmp("",DBName) && !strcmp("",TableName))
		{
			sprintf(szSql, m_sql, DBName);
			sprintf(m_sql,"%s",szSql);
		}
		else if(!strcmp("",DBName) && strcmp("",TableName))
		{
			sprintf(szSql, m_sql, TableName);
			sprintf(m_sql,"%s",szSql);
		}
		
		int valueNum = 0;//用于判断是第几个值
		int SqlNum = 0;//用于判断是第几个sql语句
		char tmpSql[1024];//临时保存查询得到的更新语句
		char BakSql[1024];//保存最后的备份语句
		ZeroMemory(tmpSql,1024);
		ZeroMemory(BakSql,1024);
		
		//获取查询的记录集的每一项
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;	
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(strcmp("PageCount",m_DataRecord.recordfieldname))
			{
				if(valueNum == 0)
				{
					sprintf(tmpSql,"%s",m_sql);
					sprintf(szSql,"%s%s",tmpSql,m_DataRecord.recorddata);//构造完整查询的sql语句
					valueNum++;
				}
				else
				{
					sprintf(tmpSql,"%s",szSql);
					sprintf(szSql,"%s,%s",tmpSql,m_DataRecord.recorddata);//构造完整查询的sql语句
					valueNum++;					
				}
			}
			else
			{
				sprintf(tmpSql,"%s",szSql);
				sprintf(szSql,"%s)",tmpSql);//构造完整查询的sql语句
				if(SqlNum == 0)
				{
					sprintf(BakSql,"%s",szSql);//构造完整查询的sql语句
				}
				else
				{
					sprintf(BakSql,"%s;%s",BakSql,szSql);//构造完整查询的sql语句
				}
				SqlNum++;//sql语句增加
				valueNum=0;	//值的数量置0
			}
		}
		if(!datarecord.empty())
		{
			datarecord.clear();
		}		
		logFile.WriteBakLog(userByID, spName, serverIP, BakSql);//将日志信息写入数据库
	}
	catch (...)
	{
	}
	return false;
}

