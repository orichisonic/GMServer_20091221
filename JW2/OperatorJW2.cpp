#include "stdafx.h"
#include ".\operatorJW2.h"
#include "time.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
//构造函数
COperatorJW2::COperatorJW2(void)
{
}

//析构函数
COperatorJW2::~COperatorJW2(void)
{
	SendVectBuf.Destroy();
}

bool COperatorJW2::ReadValue(char * strKey,char * strValue,int strLength)
{
	return logFile.ReadValue("M_JW2","JW2",strKey,strValue,strLength);
}
//构建数据包
bool COperatorJW2::BuildTLV(vector <CGlobalStruct::TFLV> DBTFLV,CEnumCore::ServiceKey m_ServiceKey,char * JW2_ServerIP)
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
					switch(m_ServiceKey)
				{
					case CEnumCore::ServiceKey::JW2_ACCOUNT_QUERY_RESP:
					{
						if(m_tflv.m_tagName==CEnumCore::TagName::JW2_LASTLOGINDATE)//login_db.logininfo.time
						{
							ZeroMemory(strResult,256);
							if(getRemoteSql("JW2","JW2_LastLogDate_Query",operRemoteSql,4))//获取查询玩家等级的sql语句
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
							if(getRemoteSql("JW2","JW2_RegistDate_Query",operRemoteSql,4))//获取查询玩家等级的sql语句
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
						else if(m_tflv.m_tagName==CEnumCore::TagName::JW2_FCMSTATUS)//item_db.fcm有没有记录有->是
						{
							ZeroMemory(strResult,256);
							if(getRemoteSql("JW2","JW2_Fcm_Query",operRemoteSql,4))//获取查询玩家等级的sql语句
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
						else if(m_tflv.m_tagName==CEnumCore::TagName::JW2_NowTitle)//item_db.fcm有没有记录有->是
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
								SendVectBuf.AddTFLV(m_tflv);//默认为直接添加TFLV
							}
							catch(...)
							{
							}	
						}

					}
					break;
					//家族道具,家族道具购买日志
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
								SendVectBuf.AddTFLV(m_tflv);//默认为直接添加TFLV
							}
						}
					break;
					//宠物信息,消费日志
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
								SendVectBuf.AddTFLV(m_tflv);//默认为直接添加TFLV
							}
						}
					break;
				case CEnumCore::ServiceKey::JW2_User_Family_Query_Resp:
				case CEnumCore::ServiceKey::JW2_FAMILYMEMBER_QUERY_RESP:
				case CEnumCore::ServiceKey::JW2_FamilyMember_Applying_Resp:
				{
					if(m_tflv.m_tagName==CEnumCore::TagName::JW2_DUTYNAME)
					{
						switch(StrToInt((char *)&m_tflv.lpdata))//0申请中，1平民，2家族管理员，3金钱管家，4管家，255族长
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
						SendVectBuf.AddTFLV(m_tflv);//默认为直接添加TFLV
					}

				}
				break;
				case CEnumCore::ServiceKey::JW2_MoneyLog_Query_Resp:
				case CEnumCore::ServiceKey::JW2_SMALL_PRESENT_QUERY_RESP:
					{
						if(m_tflv.m_tagName==CEnumCore::TagName::JW2_ACCOUNT)
						{
							ZeroMemory(strResult,256);
							if(getRemoteSql("JW2","JW2_UserSn_Account",operRemoteSql,4))//获取查询玩家等级的sql语句
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
						if(getRemoteSql("JW2","JW2_UserSn_Account",operRemoteSql,4))//获取查询玩家等级的sql语句
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
						SendVectBuf.AddTFLV(m_tflv);//默认为直接添加TFLV
					}
				}
				break;
				//纸条箱信息
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
						SendVectBuf.AddTFLV(m_tflv);//默认为直接添加TFLV
					}
				}
				break;
				//捐献日志
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
						SendVectBuf.AddTFLV(m_tflv);//默认为直接添加TFLV
					}
				}
				break;
			case CEnumCore::ServiceKey::JW2_ITEM_SELECT_RESP:
				{
					SendVectBuf.AddTFLV(m_tflv);//默认为直接添加TFLV
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
								SendVectBuf.AddTFLV(m_tflv);//默认为直接添加TFLV
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
			logFile.WriteErrorLog("JW2","构建数据包内存不足");
			return false;
		}
		catch (CFileException* e)
		{
			logFile.WriteErrorLog("JW2","构建数据包文件异常");
			return false;
		}
		catch (CException* e)
		{
			logFile.WriteErrorLog("JW2","构建数据包其他异常");
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

//获取记入日志中的信息
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
		
		ReadValue(baseMsg, tmpBaseMsg, 2048);//获取相应的基础信息
		
		va_list vaList;
		va_start(vaList, Result);
		vsprintf(strMsg, tmpBaseMsg, vaList);//获取返回语句
		va_end( vaList );
		
		if(strcmp("",diffMsg))//特殊信息如果不为空
		{
			ReadValue(diffMsg, tmpDiffMsg, 128);//获取相应的特殊信息
		//	sprintf(strMsg, "%s", retMsg);//获取基础操作信息
			sprintf(retMsg, "%s%s", strMsg,tmpDiffMsg);//基础操作信息、特殊操作信息合并
		}
		if(strcmp("",Result))//如果最后的操作结果不为空
		{
			ReadValue(Result, tmpResult, 128);//获取相应的结果信息
		//	sprintf(strMsg, "%s", retMsg);//获取基础操作信息
			sprintf(retMsg, "%s,%s", strMsg, tmpResult);//基础操作信息、特殊操作信息、操作结果合并
		}		
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","获取记入日志中的信息内存不足");
		return false;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","获取记入日志中的信息文件异常");
		return false;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","获取记入日志中的信息其他异常");
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

//写日志信息
bool COperatorJW2::WriteText(char* strLog)
{
	try
	{
		ReadValue("GameName", strMsg, 2048);//获取真正的游戏名称		
		logFile.WriteText(strMsg, strLog);
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
	}
	return false;
}

//通过DBID查IP
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

//获取需要的字符串
bool COperatorJW2::getSpecChar(const char* string, const char* find, int nNum, const char* findNext, int mNum, char* nChar)
{
	int nCount = 0;
	int mCount = 0;
	if(strcmp("",find))
	{
		nCount = StringFind(string, find, nNum);//获取相应的第一个指定字符的位置
	}
	if(strcmp("",findNext))
	{
		mCount = StringFind(string, findNext, mNum);//获取相应的第二个指定字符的位置
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

//查找字符串中特定的字符第一次出现的位置
int COperatorJW2::StringFind(const char* string,const char* find,int number)
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