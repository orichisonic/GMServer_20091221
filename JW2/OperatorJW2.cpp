#include "stdafx.h"
#include ".\operatorJW2.h"
#include "time.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
//���캯��
COperatorJW2::COperatorJW2(void)
{
}

//��������
COperatorJW2::~COperatorJW2(void)
{
	SendVectBuf.Destroy();
}

bool COperatorJW2::ReadValue(char * strKey,char * strValue,int strLength)
{
	return logFile.ReadValue("M_JW2","JW2",strKey,strValue,strLength);
}
//�������ݰ�
bool COperatorJW2::BuildTLV(vector <CGlobalStruct::TFLV> DBTFLV,CEnumCore::ServiceKey m_ServiceKey,char * JW2_ServerIP)
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
					switch(m_ServiceKey)
				{
					case CEnumCore::ServiceKey::JW2_ACCOUNT_QUERY_RESP:
					{
						if(m_tflv.m_tagName==CEnumCore::TagName::JW2_LASTLOGINDATE)//login_db.logininfo.time
						{
							ZeroMemory(strResult,256);
							if(getRemoteSql("JW2","JW2_LastLogDate_Query",operRemoteSql,4))//��ȡ��ѯ��ҵȼ���sql���
							{
								sprintf(operszSql,operRemoteSql,StrToInt((char *)&m_tflv.lpdata));
								FindDBIP(JW2_ServerIP,JW2_ServerIP,3);
								QuerySingleValue("JW2",JW2_ServerIP,1,operszSql,strResult);
							}
							else
							{
								sprintf(strResult,"");
							}				
							SendVectBuf.AddTFLV(CEnumCore::TagName::JW2_LASTLOGINDATE,CEnumCore::TagFormat::TLV_STRING,strlen(strResult),(unsigned char*)&strResult);
						}
						else if(m_tflv.m_tagName==CEnumCore::TagName::JW2_REGISTDATE)//login_db.member.registdate
						{
							ZeroMemory(strResult,256);
							if(getRemoteSql("JW2","JW2_RegistDate_Query",operRemoteSql,4))//��ȡ��ѯ��ҵȼ���sql���
							{
								sprintf(operszSql,operRemoteSql,StrToInt((char *)&m_tflv.lpdata));
								FindDBIP(JW2_ServerIP,JW2_ServerIP,3);
								QuerySingleValue("JW2",JW2_ServerIP,1,operszSql,strResult);
							}
							else
							{
								sprintf(strResult,"");
							}				
							SendVectBuf.AddTFLV(CEnumCore::TagName::JW2_REGISTDATE,CEnumCore::TagFormat::TLV_STRING,strlen(strResult),(unsigned char*)&strResult);
					
						}
						else if(m_tflv.m_tagName==CEnumCore::TagName::JW2_FCMSTATUS)//item_db.fcm��û�м�¼��->��
						{
							ZeroMemory(strResult,256);
							if(getRemoteSql("JW2","JW2_Fcm_Query",operRemoteSql,4))//��ȡ��ѯ��ҵȼ���sql���
							{
								sprintf(operszSql,operRemoteSql,StrToInt((char *)&m_tflv.lpdata));
								FindDBIP(JW2_ServerIP,JW2_ServerIP,2);
								QuerySingleValue("JW2",JW2_ServerIP,1,operszSql,strResult);
								if(strlen(strResult)>0)
								{
									sprintf(strResult,"%s","Yes");
								}
								else
								{
									sprintf(strResult,"%s","No");
								}
							}
							else
							{
								sprintf(strResult,"%s","No");
							}				
							SendVectBuf.AddTFLV(CEnumCore::TagName::JW2_FCMSTATUS,CEnumCore::TagFormat::TLV_STRING,strlen(strResult),(unsigned char*)&strResult);
					
						}
						else if(m_tflv.m_tagName==CEnumCore::TagName::JW2_NowTitle)//item_db.fcm��û�м�¼��->��
						{
								ZeroMemory(strResult,256);
								int iTitle=StrToInt((char*)m_tflv.lpdata);
								int iTitleType = (iTitle & 0xff000000)>>24;
								int iTitleNo= (iTitle & 0x00ffffff) ;
								sprintf(operszSql,"exec JW2_FindTitleName %i,%i",iTitleType,iTitleNo);
								QuerySingleValue("JW2","127.0.0.1",4,operszSql,strResult);
								if(strlen(strResult)>0)
								{
								}
								else
								{
									sprintf(strResult,"%s",m_tflv.lpdata);
								}
		
							SendVectBuf.AddTFLV(CEnumCore::TagName::JW2_NowTitle,CEnumCore::TagFormat::TLV_STRING,strlen(strResult),(unsigned char*)&strResult);
				
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
					break;
					//�������,������߹�����־
					case CEnumCore::ServiceKey::JW2_ItemInfo_Query_Resp:
					case CEnumCore::ServiceKey::JW2_FamilyBuyLog_Query_Resp:
					case CEnumCore::ServiceKey::JW2_FamilyItemInfo_Query_Resp:
						{
							if(m_tflv.m_tagName==CEnumCore::TagName::JW2_ItemName)
							{
								ZeroMemory(strResult,256);
								sprintf(operszSql,"exec JW2_ItemCodeToName '%s'",m_tflv.lpdata);
								QuerySingleValue("JW2","127.0.0.1",4,operszSql,strResult);
								if(strlen(strResult)>0)
								{
								}
								else
								{
									sprintf(strResult,"%s",m_tflv.lpdata);
								}
								SendVectBuf.AddTFLV(CEnumCore::TagName::JW2_ItemName,CEnumCore::TagFormat::TLV_STRING,strlen(strResult),(unsigned char*)&strResult);
							}
							else if(m_tflv.m_tagName==CEnumCore::TagName::JW2_FamilyPoint)
							{
								ZeroMemory(strResult,256);
								sprintf(operszSql,"exec JW2_ItemCodeToFamilyPoint '%s'",m_tflv.lpdata);
								QuerySingleValue("JW2","127.0.0.1",4,operszSql,strResult);
								if(strlen(strResult)>0)
								{
								}
								else
								{
									sprintf(strResult,"0");
								}
								SendVectBuf.AddTFLV(CEnumCore::TagName::JW2_FamilyPoint,CEnumCore::TagFormat::TLV_STRING,strlen(strResult),(unsigned char*)&strResult);
							}
							else if(m_tflv.m_tagName==CEnumCore::TagName::JW2_Money)
							{
								ZeroMemory(strResult,256);
								sprintf(operszSql,"exec JW2_ItemCodeToMoney '%s'",m_tflv.lpdata);
								QuerySingleValue("JW2","127.0.0.1",4,operszSql,strResult);
								if(strlen(strResult)>0)
								{
								}
								else
								{
									sprintf(strResult,"0");
								}
								SendVectBuf.AddTFLV(CEnumCore::TagName::JW2_Money,CEnumCore::TagFormat::TLV_STRING,strlen(strResult),(unsigned char*)&strResult);
							}
							else if(m_tflv.m_tagName==CEnumCore::TagName::JW2_BUYDATE)
							{
								ZeroMemory(strResult,256);
								sprintf(operszSql,"exec JW2_ItemCodeToProductID '%s'",m_tflv.lpdata);
								QuerySingleValue("JW2","127.0.0.1",4,operszSql,strResult);
								if(strlen(strResult)>0)
								{
									FindDBIP(JW2_ServerIP,JW2_ServerIP,6);
									if(getRemoteSql("JW2","JW2_ProductIDToTime",operRemoteSql,4))
									{
										sprintf(operszSql,operRemoteSql,strResult);
										QuerySingleValue("JW2",JW2_ServerIP,1,operszSql,strResult);
										if(strlen(strResult)>0)
										{
										}
										else
										{
											sprintf(strResult,"%s",m_tflv.lpdata);
										}	
									}
									else
									{
										sprintf(strResult,"%s",m_tflv.lpdata);
									}
								}
								else
								{
									sprintf(strResult,"%s",m_tflv.lpdata);
								}
								SendVectBuf.AddTFLV(CEnumCore::TagName::JW2_BUYDATE,CEnumCore::TagFormat::TLV_STRING,strlen(strResult),(unsigned char*)&strResult);
							}
							else
							{
								SendVectBuf.AddTFLV(m_tflv);//Ĭ��Ϊֱ�����TFLV
							}
						}
					break;
					//������Ϣ,������־
					case CEnumCore::ServiceKey::JW2_CashMoney_Log_Resp:
					case CEnumCore::ServiceKey::JW2_PetInfo_Query_Resp:
						{
							if(m_tflv.m_tagName==CEnumCore::TagName::JW2_PetLevel)
							{
								ZeroMemory(strResult,256);
								sprintf(operszSql,"exec JW2_PetExpToLevel %i",StrToInt((char *)&m_tflv.lpdata));
								QuerySingleValue("JW2","127.0.0.1",4,operszSql,strResult);
								if(strlen(strResult)>0)
								{
								}
								else
								{
									sprintf(strResult,"0");
								}
								SendVectBuf.AddTFLV(CEnumCore::TagName::JW2_PetLevel,CEnumCore::TagFormat::TLV_STRING,strlen(strResult),(unsigned char*)&strResult);
							}
							else if(m_tflv.m_tagName==CEnumCore::TagName::JW2_UserNick)
							{
								ZeroMemory(strResult,256);
								FindDBIP(JW2_ServerIP,JW2_ServerIP,1);
								if(getRemoteSql("JW2","JW2_USERSNTOUSERNICK",operRemoteSql,4))
								{
									sprintf(operszSql,operRemoteSql,StrToInt((char *)&m_tflv.lpdata));
									QuerySingleValue("JW2",JW2_ServerIP,1,operszSql,strResult);
									if(strlen(strResult)>0)
									{
									}
									else
									{
										sprintf(strResult,"%s",m_tflv.lpdata);
									}	
								}
								else
								{
									sprintf(strResult,"%s",m_tflv.lpdata);
								}
								
								SendVectBuf.AddTFLV(CEnumCore::TagName::JW2_UserNick,CEnumCore::TagFormat::TLV_STRING,strlen(strResult),(unsigned char*)&strResult);
							}
							else
							{
								SendVectBuf.AddTFLV(m_tflv);//Ĭ��Ϊֱ�����TFLV
							}
						}
					break;
				case CEnumCore::ServiceKey::JW2_User_Family_Query_Resp:
				case CEnumCore::ServiceKey::JW2_FAMILYMEMBER_QUERY_RESP:
				case CEnumCore::ServiceKey::JW2_FamilyMember_Applying_Resp:
				{
					if(m_tflv.m_tagName==CEnumCore::TagName::JW2_DUTYNAME)
					{
						switch(StrToInt((char *)&m_tflv.lpdata))//0�����У�1ƽ��2�������Ա��3��Ǯ�ܼң�4�ܼң�255�峤
						{
						case 0:
							ReadValue("MS_Application",strResult,256);
							break;
						case 1:
							ReadValue("MS_Civilian",strResult,256);
							break;
						case 2:
							ReadValue("MS_Administator",strResult,256);
							break;
						case 3:
							ReadValue("MS_MoneyChamberlain",strResult,256);
							break;
						case 4:
							ReadValue("MS_Chamberlain",strResult,256);
							break;
						case 255:
							ReadValue("MS_Shaikh",strResult,256);
							break;
						default:
							sprintf(strResult,"%s",m_tflv.lpdata);
							break;
						}
						SendVectBuf.AddTFLV(CEnumCore::TagName::JW2_DUTYNAME,CEnumCore::TagFormat::TLV_STRING,strlen(strResult),(unsigned char*)&strResult);
					}
					else
					{
						SendVectBuf.AddTFLV(m_tflv);//Ĭ��Ϊֱ�����TFLV
					}

				}
				break;
				case CEnumCore::ServiceKey::JW2_MoneyLog_Query_Resp:
				case CEnumCore::ServiceKey::JW2_SMALL_PRESENT_QUERY_RESP:
					{
						if(m_tflv.m_tagName==CEnumCore::TagName::JW2_ACCOUNT)
						{
							ZeroMemory(strResult,256);
							if(getRemoteSql("JW2","JW2_UserSn_Account",operRemoteSql,4))//��ȡ��ѯ��ҵȼ���sql���
							{
								sprintf(operszSql,operRemoteSql,StrToInt((char *)&m_tflv.lpdata));
								FindDBIP(JW2_ServerIP,JW2_ServerIP,1);
								QuerySingleValue("JW2",JW2_ServerIP,1,operszSql,strResult);
							}
							else
							{
								sprintf(strResult,"%s",m_tflv.lpdata);
							}				
							SendVectBuf.AddTFLV(CEnumCore::TagName::JW2_ACCOUNT,CEnumCore::TagFormat::TLV_STRING,strlen(strResult),(unsigned char*)&strResult);
						
						}
					else if(m_tflv.m_tagName==CEnumCore::TagName::JW2_RECEID)
					{
						ZeroMemory(strResult,256);
						if(getRemoteSql("JW2","JW2_UserSn_Account",operRemoteSql,4))//��ȡ��ѯ��ҵȼ���sql���
						{
							sprintf(operszSql,operRemoteSql,StrToInt((char *)&m_tflv.lpdata));
							FindDBIP(JW2_ServerIP,JW2_ServerIP,1);
							QuerySingleValue("JW2",JW2_ServerIP,1,operszSql,strResult);
						}
						else
						{
							sprintf(strResult,"%s",m_tflv.lpdata);
						}				
						SendVectBuf.AddTFLV(CEnumCore::TagName::JW2_RECEID,CEnumCore::TagFormat::TLV_STRING,strlen(strResult),(unsigned char*)&strResult);
						
					}
					else if(m_tflv.m_tagName==CEnumCore::TagName::JW2_AvatarItem)
					{
						ZeroMemory(strResult,256);
						sprintf(operszSql,"exec JW2_ItemCodeToName '%s'",m_tflv.lpdata);
						QuerySingleValue("JW2","127.0.0.1",4,operszSql,strResult);
						if(strlen(strResult)>0)
						{
						}
						else
						{
							sprintf(strResult,"%s",m_tflv.lpdata);
						}
						SendVectBuf.AddTFLV(CEnumCore::TagName::JW2_AvatarItem,CEnumCore::TagFormat::TLV_STRING,strlen(strResult),(unsigned char*)&strResult);
					}
					else
					{
						SendVectBuf.AddTFLV(m_tflv);//Ĭ��Ϊֱ�����TFLV
					}
				}
				break;
				//ֽ������Ϣ
				case CEnumCore::ServiceKey::JW2_MailInfo_Query_Resp:
				{
					if(m_tflv.m_tagName==CEnumCore::TagName::JW2_ItemName)
					{
						ZeroMemory(strResult,256);
						sprintf(operszSql,"exec JW2_ItemCodeToName '%s'",m_tflv.lpdata);
						QuerySingleValue("JW2","127.0.0.1",4,operszSql,strResult);
						if(strlen(strResult)>0)
						{
						}
						else
						{
							sprintf(strResult,"%s",m_tflv.lpdata);
						}
						SendVectBuf.AddTFLV(CEnumCore::TagName::JW2_ItemName,CEnumCore::TagFormat::TLV_STRING,strlen(strResult),(unsigned char*)&strResult);
					}
					else
					{
						SendVectBuf.AddTFLV(m_tflv);//Ĭ��Ϊֱ�����TFLV
					}
				}
				break;
				//������־
			case CEnumCore::ServiceKey::JW2_FamilyFund_Log_Resp:
				{
					if(m_tflv.m_tagName==CEnumCore::TagName::JW2_Money)
					{
						int itmpMoeny=StrToInt((char *)&m_tflv.lpdata);
						if(itmpMoeny/1000000==0)
						{
							sprintf(strResult,"%i gb",itmpMoeny);
						}
						else
						{
							sprintf(strResult,"%i mb",itmpMoeny%1000000);
						}
						SendVectBuf.AddTFLV(CEnumCore::TagName::JW2_Money,CEnumCore::TagFormat::TLV_STRING,strlen(strResult),(unsigned char*)&strResult);
					}
					else if(m_tflv.m_tagName==CEnumCore::TagName::JW2_ItemName)
					{
						ZeroMemory(strResult,256);
						sprintf(operszSql,"exec JW2_ItemCodeToName '%s'",m_tflv.lpdata);
						QuerySingleValue("JW2","127.0.0.1",4,operszSql,strResult);
						if(strlen(strResult)>0)
						{
						}
						else
						{
							sprintf(strResult,"%s",m_tflv.lpdata);
						}
						SendVectBuf.AddTFLV(CEnumCore::TagName::JW2_ItemName,CEnumCore::TagFormat::TLV_STRING,strlen(strResult),(unsigned char*)&strResult);
					}
					else
					{
						SendVectBuf.AddTFLV(m_tflv);//Ĭ��Ϊֱ�����TFLV
					}
				}
				break;
			case CEnumCore::ServiceKey::JW2_ITEM_SELECT_RESP:
				{
					SendVectBuf.AddTFLV(m_tflv);//Ĭ��Ϊֱ�����TFLV
				}
				break;
			default:
					{
						if(m_tflv.m_tagName==CEnumCore::TagName::JW2_AvatarItemName)
						{
							ZeroMemory(strResult,256);
							sprintf(operszSql,"exec FindAvatarItem '%s'",m_tflv.lpdata);
							QuerySingleValue("JW2","127.0.0.1",4,operszSql,strResult);
							if(strlen(strResult)==0)
							{
								sprintf(strResult,"%s",m_tflv.lpdata);
							}
							SendVectBuf.AddTFLV(CEnumCore::TagName::JW2_AvatarItemName,CEnumCore::TagFormat::TLV_STRING,strlen(strResult),(unsigned char*)&strResult);
					
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
					break;
					}
				
				}
				iEnbodyLength = SendVectBuf.EnBody();
		}
		catch (CMemoryException* e)
		{
			logFile.WriteErrorLog("JW2","�������ݰ��ڴ治��");
			return false;
		}
		catch (CFileException* e)
		{
			logFile.WriteErrorLog("JW2","�������ݰ��ļ��쳣");
			return false;
		}
		catch (CException* e)
		{
			logFile.WriteErrorLog("JW2","�������ݰ������쳣");
			return false;
		}
		catch (runtime_error err)
		{
			logFile.WriteErrorLog("JW2",(char *)err.what());
			return false;
		}
		catch (...)
		{
			return false;
		}
		return TRUE;
}
bool COperatorJW2::BuildTLV(char * message)
{
	return COperatorGame::BuildTLV(message);
}

//��ȡ������־�е���Ϣ
bool COperatorJW2::GetLogText(char* retMsg, char* baseMsg, char* diffMsg, char* Result, ...)
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
		
		ReadValue(baseMsg, tmpBaseMsg, 2048);//��ȡ��Ӧ�Ļ�����Ϣ
		
		va_list vaList;
		va_start(vaList, Result);
		vsprintf(strMsg, tmpBaseMsg, vaList);//��ȡ�������
		va_end( vaList );
		
		if(strcmp("",diffMsg))//������Ϣ�����Ϊ��
		{
			ReadValue(diffMsg, tmpDiffMsg, 128);//��ȡ��Ӧ��������Ϣ
		//	sprintf(strMsg, "%s", retMsg);//��ȡ����������Ϣ
			sprintf(retMsg, "%s%s", strMsg,tmpDiffMsg);//����������Ϣ�����������Ϣ�ϲ�
		}
		if(strcmp("",Result))//������Ĳ��������Ϊ��
		{
			ReadValue(Result, tmpResult, 128);//��ȡ��Ӧ�Ľ����Ϣ
		//	sprintf(strMsg, "%s", retMsg);//��ȡ����������Ϣ
			sprintf(retMsg, "%s,%s", strMsg, tmpResult);//����������Ϣ�����������Ϣ����������ϲ�
		}		
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","��ȡ������־�е���Ϣ�ڴ治��");
		return false;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","��ȡ������־�е���Ϣ�ļ��쳣");
		return false;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","��ȡ������־�е���Ϣ�����쳣");
		return false;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return false;
	}
	catch (...)
	{
		logFile.WriteErrorLog("JW2","Unknown Exception");
		return false;
	}
	return false;
}

//д��־��Ϣ
bool COperatorJW2::WriteText(char* strLog)
{
	try
	{
		ReadValue("GameName", strMsg, 2048);//��ȡ��������Ϸ����		
		logFile.WriteText(strMsg, strLog);
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
	}
	return false;
}

//ͨ��DBID��IP
void COperatorJW2::FindDBIP(char * lpIP,char * lpNewIP,int dbid)
{
	try
	{
		char szSql[256];
		sprintf(szSql,"exec JW2_FindDBIP '%s',%i",lpIP,dbid);
		QuerySingleValue("JW2","127.0.0.1",4,szSql,lpIP);
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
	}
}

//��ȡ��Ҫ���ַ���
bool COperatorJW2::getSpecChar(const char* string, const char* find, int nNum, const char* findNext, int mNum, char* nChar)
{
	int nCount = 0;
	int mCount = 0;
	if(strcmp("",find))
	{
		nCount = StringFind(string, find, nNum);//��ȡ��Ӧ�ĵ�һ��ָ���ַ���λ��
	}
	if(strcmp("",findNext))
	{
		mCount = StringFind(string, findNext, mNum);//��ȡ��Ӧ�ĵڶ���ָ���ַ���λ��
	}
	if(mCount > 0)
	{
		memcpy(nChar, (char*)&string[nCount], mCount-nCount-1);
	}
	else
	{
		sprintf(nChar, "%s", (char*)&string[nCount]);
	}
	
	return true;
}

//�����ַ������ض����ַ���һ�γ��ֵ�λ��
int COperatorJW2::StringFind(const char* string,const char* find,int number)
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

bool COperatorJW2::TagIDToSqlType(CEnumCore::Message_Tag_ID m_tagID,char * sql_type)
{
	switch(m_tagID)
	{
	case CEnumCore::Message_Tag_ID::JW2_RPG_QUERY:
		sprintf(sql_type,"%s","JW2_RPG_QUERY");
		break;
	case CEnumCore::Message_Tag_ID::JW2_ITEMSHOP_BYOWNER_QUERY:
		sprintf(sql_type,"%s","JW2_ITEMSHOP_BYOWNER_QUERY");
		break;
	case CEnumCore::Message_Tag_ID::JW2_HOME_ITEM_QUERY:
		sprintf(sql_type,"%s","JW2_HOME_ITEM_QUERY");
		break;
	case CEnumCore::Message_Tag_ID::JW2_WASTE_ITEM_QUERY:
		sprintf(sql_type,"%s","JW2_WASTE_ITEM_QUERY");
		break;
	case CEnumCore::Message_Tag_ID::JW2_SMALL_BUGLE_QUERY:
		sprintf(sql_type,"%s","JW2_SMALL_BUGLE_QUERY");
		break;
	case CEnumCore::Message_Tag_ID::JW2_WEDDINGINFO_QUERY:
		sprintf(sql_type,"%s","JW2_WEDDINGINFO_QUERY");
		break;
	case CEnumCore::Message_Tag_ID::JW2_WEDDINGLOG_QUERY:
		sprintf(sql_type,"%s","JW2_WEDDINGINFO_QUERY");
		break;
	case CEnumCore::Message_Tag_ID::JW2_COUPLEINFO_QUERY:
		sprintf(sql_type,"%s","JW2_COUPLEINFO_QUERY");
		break;
	case CEnumCore::Message_Tag_ID::JW2_COUPLELOG_QUERY:
		sprintf(sql_type,"%s","JW2_COUPLELOG_QUERY");
		break;
	case CEnumCore::Message_Tag_ID::JW2_WEDDINGGROUND_QUERY:
		sprintf(sql_type,"%s","JW2_WEDDINGGROUND_QUERY");
		break;
	case CEnumCore::Message_Tag_ID::JW2_User_Family_Query:
		sprintf(sql_type,"%s","JW2_User_Family_Query");
		break;
	case CEnumCore::Message_Tag_ID::JW2_CoupleParty_Card:
		sprintf(sql_type,"%s","JW2_CoupleParty_Card");
		break;
	case CEnumCore::Message_Tag_ID::JW2_MailInfo_Query:
		sprintf(sql_type,"%s","JW2_MailInfo_Query");
		break;
	case CEnumCore::Message_Tag_ID::JW2_FAMILYMEMBER_QUERY:
		sprintf(sql_type,"%s","JW2_FAMILYMEMBER_QUERY");
		break;
	case CEnumCore::Message_Tag_ID::JW2_FamilyItemInfo_Query:
		sprintf(sql_type,"%s","JW2_FamilyItemInfo_Query");
		break;
	case CEnumCore::Message_Tag_ID::JW2_FriendList_Query:
		sprintf(sql_type,"%s","JW2_FriendList_Query");
		break;
	case CEnumCore::Message_Tag_ID::JW2_FamilyMember_Applying:
		sprintf(sql_type,"%s","JW2_FamilyMember_Applying");
		break;
	case CEnumCore::Message_Tag_ID::JW2_PetInfo_Query:
		sprintf(sql_type,"%s","JW2_PetInfo_Query");
		break;
	case CEnumCore::Message_Tag_ID::JW2_BasicRank_Query:
		sprintf(sql_type,"%s","JW2_BasicRank_Query");
		break;
	case CEnumCore::Message_Tag_ID::JW2_Messenger_Query:
		sprintf(sql_type,"%s","JW2_Messenger_Query");
		break;
	case CEnumCore::Message_Tag_ID::JW2_Wedding_Paper:
		sprintf(sql_type,"%s","JW2_Wedding_Paper");
		break;
	case CEnumCore::Message_Tag_ID::JW2_Item_Log:
		sprintf(sql_type,"%s","JW2_Item_Log");
		break;
	case CEnumCore::Message_Tag_ID::JW2_Task_Log:
		sprintf(sql_type,"%s","JW2_Task_Log");
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

int COperatorJW2::TagIDToDBID(CEnumCore::Message_Tag_ID m_tagID)
{
	int DBID=0;
	switch(m_tagID)
	{
	case CEnumCore::Message_Tag_ID::JW2_DUMMONEY_QUERY:
	case CEnumCore::Message_Tag_ID::JW2_WEDDINGINFO_QUERY:
	case CEnumCore::Message_Tag_ID::JW2_WEDDINGLOG_QUERY:
	case CEnumCore::Message_Tag_ID::JW2_COUPLEINFO_QUERY:
	case CEnumCore::Message_Tag_ID::JW2_COUPLELOG_QUERY:
	case CEnumCore::Message_Tag_ID::JW2_WEDDINGGROUND_QUERY:
	case CEnumCore::Message_Tag_ID::JW2_CoupleParty_Card:
	case CEnumCore::Message_Tag_ID::JW2_FriendList_Query:
	case CEnumCore::Message_Tag_ID::JW2_Wedding_Paper:
		DBID=1;
		break;
	case CEnumCore::Message_Tag_ID::JW2_RPG_QUERY:
	case CEnumCore::Message_Tag_ID::JW2_ITEMSHOP_BYOWNER_QUERY:
	case CEnumCore::Message_Tag_ID::JW2_HOME_ITEM_QUERY:
	case CEnumCore::Message_Tag_ID::JW2_WASTE_ITEM_QUERY:
	case CEnumCore::Message_Tag_ID::JW2_SMALL_BUGLE_QUERY:
	case CEnumCore::Message_Tag_ID::JW2_PetInfo_Query:
		DBID=2;
		break;
	case CEnumCore::Message_Tag_ID::JW2_User_Family_Query:
	case CEnumCore::Message_Tag_ID::JW2_MailInfo_Query:
	case CEnumCore::Message_Tag_ID::JW2_FAMILYMEMBER_QUERY:
	case CEnumCore::Message_Tag_ID::JW2_FamilyItemInfo_Query:
	case CEnumCore::Message_Tag_ID::JW2_FamilyMember_Applying:
	case CEnumCore::Message_Tag_ID::JW2_BasicRank_Query:
	case CEnumCore::Message_Tag_ID::JW2_Messenger_Query:
	case CEnumCore::Message_Tag_ID::JW2_Item_Log:
	case CEnumCore::Message_Tag_ID::JW2_Task_Log:
		DBID=6;
		break;
	default:
		break;
	}
	return DBID;
}