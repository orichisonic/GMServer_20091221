#include "stdafx.h"
#include ".\OperatorSDO.h"

//���캯��
COperatorSDO::COperatorSDO(void)
{
}

//��������
COperatorSDO::~COperatorSDO(void)
{
	SendVectBuf.Destroy();
}
//�������ݰ�
bool COperatorSDO::BuildTLV(vector <CGlobalStruct::TFLV> DBTFLV,CEnumCore::ServiceKey m_ServiceKey,char * ServerIP)
{
	try
	{
		if(DBTFLV.empty())
		{
			return false;
		}
		SendVectBuf.InitCreate(DBTFLV.size()+1, 1);//��ʼ�����͵�����
		UINT tmpServiceKey = (UINT)m_ServiceKey;//������ʱ��ServiceKey
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
				int exp = StrToInt((LPCSTR)tflv_Exp.lpdata);//��ȡ��ҵľ���ֵ
				if(!getRemoteSql("SDO", "SDO_LevelExp_Query",m_sql,0))//��ȡ��ѯ��ҵȼ���sql���
				{
					sprintf(UserLvl,"%s","0");
				}
				else
				{
					QuerySingleValue(UserLvl, "", "127.0.0.1", 0, m_sql, exp);//��ȡ�ȼ�
				}				
				SendVectBuf.AddTFLV(CEnumCore::TagName::SDO_Level,CEnumCore::TagFormat::TLV_STRING,strlen(UserLvl),(unsigned char*)&UserLvl);
			}
			else if(m_tflv.m_tagName == CEnumCore::TagName::SDO_ItemName)
			{				
				CGlobalStruct::TFLV tflv_ItemCode = *(iter-1);
				int ItemCode=StrToInt((LPCSTR)tflv_ItemCode.lpdata);//��ȡ��ҵĵĵ���code
				char ItemName[128];				
				ZeroMemory(ItemName,128);
				sprintf(ItemName,(LPCSTR)m_tflv.lpdata);//��ȡ��ҵĵĵ�������
				if(!strcmp("",ItemName) || !strcmp("NULL",ItemName))
				{					
					switch (tmpServiceKey)
					{
					case CEnumCore::ServiceKey::SDO_MEDALITEM_QUERY_RESP:
					case CEnumCore::ServiceKey::SDO_MEDALITEM_OWNER_QUERY_RESP:
						{
							if(getRemoteSql("SDO", "SDO_Get_MedalName",m_sql,0))//��ȡ��ѯѫ����ʵ���Ƶ�sql���
							{
								QuerySingleValue(ItemName, "", "127.0.0.1", 0, m_sql,ItemCode);//��ȡѫ������
							}	
						}
						break;
					default:
						GetItemName(ItemName,ItemCode);//��ȡ��ҵ�������
						break;
					}					
			    	SendVectBuf.AddTFLV(CEnumCore::TagName::SDO_ItemName,CEnumCore::TagFormat::TLV_STRING,strlen(ItemName),(unsigned char*)&ItemName);
				}
				else
				{
					SendVectBuf.AddTFLV(m_tflv);//ֱ�����TFLV
				}
			}
			else if(m_tflv.m_tagName == CEnumCore::TagName::SDO_ItemName1)
			{
				
				CGlobalStruct::TFLV tflv_ItemCode = *(iter-1);
				int ItemCode=StrToInt((LPCSTR)tflv_ItemCode.lpdata);//��ȡ��ҵĵĵ���code
				char ItemName[128];				
				ZeroMemory(ItemName,128);
				sprintf(ItemName,(LPCSTR)m_tflv.lpdata);//��ȡ��ҵĵĵ�������
				if(!strcmp("",ItemName) || !strcmp("NULL",ItemName))
				{
					GetItemName(ItemName,ItemCode);//��ȡ��ҵ�������
					SendVectBuf.AddTFLV(CEnumCore::TagName::SDO_ItemName1,CEnumCore::TagFormat::TLV_STRING,strlen(ItemName),(unsigned char*)&ItemName);
				}
				else
				{
					SendVectBuf.AddTFLV(m_tflv);//ֱ�����TFLV
				}
			}
			else if(m_tflv.m_tagName == CEnumCore::TagName::SDO_ItemName2)
			{
				CGlobalStruct::TFLV tflv_ItemCode = *(iter-1);
				int ItemCode=StrToInt((LPCSTR)tflv_ItemCode.lpdata);//��ȡ��ҵĵĵ���code
				char ItemName[128];				
				ZeroMemory(ItemName,128);
				sprintf(ItemName,(LPCSTR)m_tflv.lpdata);//��ȡ��ҵĵĵ�������
				if(!strcmp("",ItemName) || !strcmp("NULL",ItemName))
				{
					GetItemName(ItemName,ItemCode);//��ȡ��ҵ�������
					SendVectBuf.AddTFLV(CEnumCore::TagName::SDO_ItemName2,CEnumCore::TagFormat::TLV_STRING,strlen(ItemName),(unsigned char*)&ItemName);
				}
				else
				{
					SendVectBuf.AddTFLV(m_tflv);//ֱ�����TFLV
				}
			}
			else if(m_tflv.m_tagName == CEnumCore::TagName::SDO_Precent)
			{
				CGlobalStruct::TFLV tflv_ItemCode = *(iter-2);
				int ItemCode=StrToInt((LPCSTR)tflv_ItemCode.lpdata);//��ȡ��ҵĵĵ���code
				if(tmpServiceKey == CEnumCore::ServiceKey::SDO_MEDALITEM_OWNER_QUERY_RESP)
				{
					int perc = 0;
					char tempIP[20];
					if(!GetServerIP(tempIP,ServerIP,2))
					{
						return FALSE;//�޷���ȡ�����ķ�����IP
					}

					if(getRemoteSql("SDO","SDO_MedalItemPerc_Query", m_sql, 0))			//��ȡ��ѯ���߻�ȡ���ʵ�sql���
					{
						printf(m_sql);
						QuerySingleValue(&perc, "SDO", tempIP, 0, m_sql, ItemCode);		//��ȡ���߻�ȡ����	
					}	

					char StrInt[10];
					ZeroMemory(StrInt,10);
					_itoa(perc, StrInt, 10);//��ȡ���߱���
					SendVectBuf.AddTFLV(CEnumCore::TagName::SDO_Precent,CEnumCore::TagFormat::TLV_INTEGER,sizeof(int),(unsigned char*)&StrInt);
				}
				else
				{
					SendVectBuf.AddTFLV(m_tflv);//ֱ�����TFLV
				}
			}
			else
			{
				try
				{
					SendVectBuf.AddTFLV(m_tflv);//Ĭ��Ϊֱ�����TFLV
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
			logFile.WriteErrorLog("SDO","�������ݰ��ڴ治��");
			return false;
		}
		catch (CFileException* e)
		{
			logFile.WriteErrorLog("SDO","�������ݰ��ļ��쳣");
			return false;
		}
		catch (CException* e)
		{
			logFile.WriteErrorLog("SDO","�������ݰ������쳣");
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

//��ѯ�滻����ֵ
void COperatorSDO::QuerySingleValue(char* SingleValue, char* gamename, char* ServerIP, int nFlag, const char * szFormat, ...)
{
	try
	{
		va_list vaList;
		va_start(vaList, szFormat);
		vsprintf(szSql, szFormat, vaList);//��ȡ����sql���
		va_end( vaList );

		vector<CGlobalStruct::DataRecord> datarecord;
		if(strcmp("",gamename))
		{
			m_SqlHelper.SqlHelperMain(gamename, &datarecord, ServerIP, szSql, nFlag, 1, 1);//��ȡҪ�滻��ֵ
		}
		else
		{
			m_SqlHelper.SqlHelperMain(&datarecord, ServerIP, szSql, nFlag, 1, 1);//��ȡҪ�滻��ֵ
		}		
		
		if(datarecord.empty())//�����¼Ϊ��
		{
			sprintf(SingleValue,"");
			return;
		}
		
		//��ȡ��ѯ�ļ�¼����ÿһ��
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;	
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("name", m_DataRecord.recordfieldname))//��ȡ��Ӧ��LogDBIP
			{
		    	sprintf(SingleValue,"%s", m_DataRecord.recorddata);//�����ѯ�õ��ĵ�һ����¼
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

//��ȡint�͵ĵ���ֵ
void COperatorSDO::QuerySingleValue(int* SingleValue, char* gamename, char* ServerIP, int nFlag, const char * szFormat, ...)
{
	try
	{
		va_list vaList;
		va_start(vaList, szFormat);
		vsprintf(szSql, szFormat, vaList);//��ȡ����sql���
		va_end( vaList );
		
		vector<CGlobalStruct::DataRecord> datarecord;
		if(strcmp("",gamename))
		{
			m_SqlHelper.SqlHelperMain(gamename, &datarecord, ServerIP, szSql, nFlag, 1, 1);//��ȡҪ�滻��ֵ
		}
		else
		{
			m_SqlHelper.SqlHelperMain(&datarecord, ServerIP, szSql, nFlag, 1, 1);//��ȡҪ�滻��ֵ
		}		
		
		if(datarecord.empty())//�����¼Ϊ��
		{
			*SingleValue = 0;
			return;
		}
		
		//��ȡ��ѯ�ļ�¼����ÿһ��
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;	
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("name", m_DataRecord.recordfieldname))//��ȡ��Ӧ��LogDBIP
			{
				*SingleValue = StrToInt(m_DataRecord.recorddata);//�����ѯ�õ��ĵ�һ����¼
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

//�����ַ������ض����ַ���һ�γ��ֵ�λ��
int COperatorSDO::StringFind(const char* string,const char* find,int number)
{
	char* pos = (char *)string;
	char *p=pos;
	int count = 0;
	while (number > 0)
	{
		//������ҵ����ַ�λ�õ�ָ�룬�Ա���ʱָ����б���
		pos = strstr(p,find);
		//��λ��ָ��Ϊ0ʱ��˵��û���ҵ�����ַ�
		if (pos == 0)
			return -1;
		//��λ��ָ�����ʱָ�����˵����һ���ַ�����Ҫ�ҵ��ַ��������ʱָ��С��λ��ָ�룬����б����ַ�������������count��1
		while(p <= pos)
		{
			p++;
			count++;
		}
		//��Ҫ���ҵĴ�����һ
		number--;
	}
	return count;
}

//��ȡѡ���ַ�ǰ����ַ���
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
			memcpy(strRet, string, charPos-1);//��ȡ�㹻���ȵ��ַ���
		}
		else
		{
			charPos = StringFind(string, find, number);
			if(charPos == -1)
			{
				return false;
			}
			charPosPrev = StringFind(string, find, number-1);
			memcpy(strRet, (char*)&string[charPosPrev], charPos-charPosPrev-1);//��ȡ�㹻���ȵ��ַ���
		}
	}
	catch (...)
	{
		return false;
	}
	return true;
}

//��ȡ������־�е���Ϣ
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

		logFile.ReadValue("SDO", "Message", baseMsg, tmpBaseMsg, 2048);//��ȡ��Ӧ�Ļ�����Ϣ
		sprintf(retMsg, "%s", tmpBaseMsg);//��ȡ����������Ϣ

		va_list vaList;
		va_start(vaList, Result);
		vsprintf(retMsg, tmpBaseMsg, vaList);//��ȡ�������
		va_end( vaList );

		if(strcmp("",diffMsg))//������Ϣ�����Ϊ��
		{
			logFile.ReadValue("SDO", "Message", diffMsg, tmpDiffMsg, 128);//��ȡ��Ӧ��������Ϣ
			sprintf(strMsg, "%s", retMsg);//��ȡ����������Ϣ
			sprintf(retMsg, "%s%s", strMsg,tmpDiffMsg);//����������Ϣ�����������Ϣ�ϲ�
		}
		if(strcmp("",Result))//������Ĳ��������Ϊ��
		{
			logFile.ReadValue("SDO", "Message", Result, tmpResult, 128);//��ȡ��Ӧ�Ľ����Ϣ
			sprintf(strMsg, "%s", retMsg);//��ȡ����������Ϣ
			sprintf(retMsg, "%s,%s", strMsg, tmpResult);//����������Ϣ�����������Ϣ����������ϲ�
		}		
	}
	catch (...)
	{
	}
	return false;
}

//д��־��Ϣ
bool COperatorSDO::WriteText(char* gameName, char* strLog)
{
	try
	{
		logFile.ReadValue("SDO", "GameName", gameName, strMsg, 2048);//��ȡ��������Ϸ����		
		logFile.WriteText(strMsg, strLog);
	}
	catch(...)
	{
	}
	return false;
}

//���سɹ���ʧ�ܻ�ϵͳ�������Ϣ
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
		ZeroMemory(tmpResult, 128);//��ʼ����ʱ�����Ϣ
		logFile.ReadValue("SDO", "Message", result, tmpResult, 128);//��ȡ�ɹ���ʧ�ܻ���ϵͳ�������Ϣ
		sprintf(strMsg, "%s", strLog);
		sprintf(strLog, "%s,%s", strMsg, tmpResult);//�������յ���־��Ϣ
		WriteText("SDO",strLog);//����Ϣд����־��
		
		sprintf((char *)&tagtflv.lpdata, "%s", tmpResult);
	}
	catch(...)
	{
	}
	
	tagtflv.m_tvlength = strlen((char *)&tagtflv.lpdata);
	DBTFLV.push_back(tagtflv);
	return DBTFLV;
}

//����־��Ϣд�����ݿ�
bool COperatorSDO::WriteDBLog(int userByID, char* gameName, char* sp_Name, char* serverIP, char* strLog)
{
	try
	{
		logFile.ReadValue("SDO", "GameName", gameName, strMsg, 2048);//��ȡ��������Ϸ����		
		logFile.WriteDBLog(userByID, strMsg, sp_Name, serverIP, strLog);//����־��Ϣд�����ݿ�
	}
	catch(...)
	{
	}
	return false;
}

//�����ʵ�ķ�����IP
bool COperatorSDO::GetServerIP(char* DestServerIP, char* SrcServerIP, int gameDBID)
{
	try
	{
		if(!getRemoteSql("SDO","SDO_GetServerIP",m_sql,1))
		{
			return false;
		}
		QuerySingleValue(DestServerIP, "", "127.0.0.1", 0, m_sql, SrcServerIP ,gameDBID);//��ȡ�����ķ�����IP

	}
	catch (...)
	{
		return false;
	}
	return true;
}

//���ݵ���code��ѯ��������
bool COperatorSDO::GetItemName(char* ItemName, int ItemCode)
{
	try
	{
		if(getRemoteSql("SDO","SDO_GetRealItemName",m_sql,0))//��ȡ��ѯ��ҵ�����ʵ���Ƶ�sql���
		{
			QuerySingleValue(ItemName, "", "127.0.0.1", 0, m_sql,ItemCode);//��ȡ��ҵ�������
		}		
	}
	catch (...)
	{
		return false;
	}
	return true;
}

//���ڶ�����ѯ�õ���������ݱ��ݵ���һ�����ݿ���
bool COperatorSDO::BackupDataBase(char* DestGame,char* DestServerIP,int Flag,char* DestDBName,char* SrcGame,char* SrcServerIP,int nFlag,char* SqlCondition)
{
	try
	{
		vector<CGlobalStruct::DataRecord> datarecord;
		if(strcmp("",SrcGame))
		{
			m_SqlHelper.SqlHelperMain(SrcGame, &datarecord, SrcServerIP, SqlCondition, nFlag, 1, 1);//��ȡ����Ҫ��ѯ���ݵ�ֵ
		}
		else
		{
			m_SqlHelper.SqlHelperMain(&datarecord, SrcServerIP, SqlCondition, nFlag, 1, 1);//��ȡ����Ҫ��ѯ���ݵ�ֵ
		}
		
		
		if(datarecord.empty())//�����¼Ϊ��
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
			return false;//�޷��õ�����Ϣ������ʱ���sql���
		}
		sprintf(strMsg,tmpSql,DestDBName);//����������ѯ��sql���

		int valueNum=0;//�����ж���sql�����Ҫ��Ҫ��","��
		//��ȡ��ѯ�ļ�¼����ÿһ��
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
						sprintf(tempSql,"%s'%s'",tmpSql,m_DataRecord.recorddata);//����������ѯ��sql���
						break;
					case 4:
						sprintf(tempSql,"%s%s",tmpSql,m_DataRecord.recorddata);//����������ѯ��sql���
						break;
					case 135:
						sprintf(tempSql,"%s'%s'",tmpSql,m_DataRecord.recorddata);//����������ѯ��sql���
						break;
					case 17:
						sprintf(tempSql,"%s%s",tmpSql,m_DataRecord.recorddata);//����������ѯ��sql���
						break;
					case 11:
					case 12:
						if(strcmp(m_DataRecord.recorddata, "NULL"))
						{
							sprintf(tempSql,"%s,'%s'",tmpSql,m_DataRecord.recorddata);//����������ѯ��sql���
						}
						else
						{
							sprintf(tempSql,"%s,%s",tmpSql,m_DataRecord.recorddata);//����������ѯ��sql���
						}
						break;
					default:
						sprintf(tempSql,"%s'%s'",tmpSql,m_DataRecord.recorddata);//����������ѯ��sql���
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
						sprintf(tempSql,"%s,'%s'",tmpSql,m_DataRecord.recorddata);//����������ѯ��sql���
						break;
					case 4:
						sprintf(tempSql,"%s,%s",tmpSql,m_DataRecord.recorddata);//����������ѯ��sql���
						break;
					case 135:
						sprintf(tempSql,"%s,'%s'",tmpSql,m_DataRecord.recorddata);//����������ѯ��sql���
						break;
					case 17:
						sprintf(tempSql,"%s,%s",tmpSql,m_DataRecord.recorddata);//����������ѯ��sql���
						break;
					case 11:
					case 12:
						if(strcmp(m_DataRecord.recorddata, "NULL"))
						{
							sprintf(tempSql,"%s,'%s'",tmpSql,m_DataRecord.recorddata);//����������ѯ��sql���
						}
						else
						{
							sprintf(tempSql,"%s,%s",tmpSql,m_DataRecord.recorddata);//����������ѯ��sql���
						}
						break;
					default:
						sprintf(tempSql,"%s,'%s'",tmpSql,m_DataRecord.recorddata);//����������ѯ��sql���
						break;
					}
					valueNum++;					
				}
			}
			else
			{
				sprintf(tmpSql,"%s",tempSql);
				sprintf(tempSql,"%s)",tmpSql);//����������ѯ��sql���
				valueNum=0;	
				if(strcmp("",DestGame))
				{
					if(!(m_SqlHelper.SqlHelperMain(DestGame, DestServerIP, tempSql, Flag)))//ִ�в������
					{
						BuildTLV("ExecuteSqlWrong");
						return false;
					}
				}
				else
				{
					if(!(m_SqlHelper.SqlHelperMain(DestServerIP, tempSql, Flag)))//ִ�в������
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

//�����޸�֮ǰ����Ϣ
bool COperatorSDO::BakUpdateInfo(int userByID, char* spName, char* game, char* serverIP,int Flag, char* DBName, char* QueryFlag, char* UpdateFlag, int ID1, int ID2)
{
	try
	{
		if(!getRemoteSql("SDO",QueryFlag,m_sql,0))//�޷��õ����ݵ�sql���
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
			m_SqlHelper.SqlHelperMain(game, &datarecord, serverIP, szSql, Flag, 1, 1);//��ȡҪ�滻��ֵ
		}
		else
		{
			m_SqlHelper.SqlHelperMain(&datarecord, serverIP, szSql, Flag, 1, 1);//��ȡҪ�滻��ֵ
		}		
		
		if(datarecord.empty())//�����¼Ϊ��
		{
			return false;
		}

		if(!getRemoteSql("SDO",UpdateFlag,m_sql,0))//�޷��õ����ݵ�sql���
		{
			return false;
		}

		if(strcmp("",DBName))
		{
			sprintf(szSql, "%s,%s", m_sql, DBName);
			sprintf(m_sql,"%s",szSql);
		}
		
		int valueNum = 0;//�����ж��ǵڼ���ֵ
		int SqlNum = 0;//�����ж��ǵڼ���sql���
		char tmpSql[1024];//��ʱ�����ѯ�õ��ĸ������
		char BakSql[1024];//�������ı������
		ZeroMemory(tmpSql,1024);
		ZeroMemory(BakSql,1024);
		
		//��ȡ��ѯ�ļ�¼����ÿһ��
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
					sprintf(tmpSql,"%s",szSql);//����������ѯ��sql���
					valueNum++;				
				}
			}
			else
			{
				if(SqlNum == 0)
				{
					sprintf(BakSql,"%s",tmpSql);//����������ѯ��sql���
				}
				else
				{
					sprintf(BakSql,"%s;%s",BakSql,tmpSql);//����������ѯ��sql���
				}
				SqlNum++;//sql�������
				valueNum=0;	//ֵ��������0
			}
		}
		if(!datarecord.empty())
		{
			datarecord.clear();
		}		
		logFile.WriteBakLog(userByID, spName, serverIP, BakSql);//����־��Ϣд�����ݿ�
	}
	catch (...)
	{
	}
	return false;
}

//����ɾ��֮ǰ����Ϣ
bool COperatorSDO::BakDeleteInfo(int userByID, char* spName, char* game, char* serverIP,int Flag, char* DBName, char* TableName, char* QueryFlag, char* InsertFlag, int ID1, int ID2)
{
	try
	{
		if(!getRemoteSql("SDO",QueryFlag,m_sql,0))//�޷��õ����ݵ�sql���
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
			m_SqlHelper.SqlHelperMain(game, &datarecord, serverIP, szSql, Flag, 1, 1);//��ȡҪ�滻��ֵ
		}
		else
		{
			m_SqlHelper.SqlHelperMain(&datarecord, serverIP, szSql, Flag, 1, 1);//��ȡҪ�滻��ֵ
		}		
		
		if(datarecord.empty())//�����¼Ϊ��
		{
			return false;
		}
		
		if(!getRemoteSql("SDO",InsertFlag,m_sql,0))//�޷��õ����ݵ�sql���
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
		
		int valueNum = 0;//�����ж��ǵڼ���ֵ
		int SqlNum = 0;//�����ж��ǵڼ���sql���
		char tmpSql[1024];//��ʱ�����ѯ�õ��ĸ������
		char BakSql[1024];//�������ı������
		ZeroMemory(tmpSql,1024);
		ZeroMemory(BakSql,1024);
		
		//��ȡ��ѯ�ļ�¼����ÿһ��
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
					sprintf(szSql,"%s%s",tmpSql,m_DataRecord.recorddata);//����������ѯ��sql���
					valueNum++;
				}
				else
				{
					sprintf(tmpSql,"%s",szSql);
					sprintf(szSql,"%s,%s",tmpSql,m_DataRecord.recorddata);//����������ѯ��sql���
					valueNum++;					
				}
			}
			else
			{
				sprintf(tmpSql,"%s",szSql);
				sprintf(szSql,"%s)",tmpSql);//����������ѯ��sql���
				if(SqlNum == 0)
				{
					sprintf(BakSql,"%s",szSql);//����������ѯ��sql���
				}
				else
				{
					sprintf(BakSql,"%s;%s",BakSql,szSql);//����������ѯ��sql���
				}
				SqlNum++;//sql�������
				valueNum=0;	//ֵ��������0
			}
		}
		if(!datarecord.empty())
		{
			datarecord.clear();
		}		
		logFile.WriteBakLog(userByID, spName, serverIP, BakSql);//����־��Ϣд�����ݿ�
	}
	catch (...)
	{
	}
	return false;
}

