#include "stdafx.h"
#include ".\OperatorGT.h"

//构造函数
COperatorGT::COperatorGT(void)
{	
}

//析构函数
COperatorGT::~COperatorGT(void)
{

}
bool COperatorGT::BuildTLV(char * message)
{
	return COperatorGame::BuildTLV(message);
}
bool COperatorGT::BuildTLV(COperVector m_OpVect,CEnumCore::ServiceKey m_ServiceKey,char * ServerIP)
{
	try
	{
		char strInt[10];
		char m_result[256];
		int allTFLV = m_OpVect.getAllTFLV();//获取所有的TFLV数量
		if(allTFLV<=0)
		{
			return false;
		}
		SendVectBuf.InitCreate(allTFLV, 1);//初始化发送的数据


		vector<CGlobalStruct::TFLV>::iterator iter;
		vector<CGlobalStruct::TFLV> v_DateSet=m_OpVect.GetInnerList();
		for( iter = v_DateSet.begin(); iter != v_DateSet.end(); iter++ )
		{

			CGlobalStruct::TFLV m_tflv = *iter;//获取每一个TFLV

			if(m_ServiceKey==CEnumCore::ServiceKey::GT_Attr_Queue_Resp)
			{
				if(m_tflv.m_tagName == CEnumCore::TagName::GT_Title)
				{
					CGlobalStruct::TFLV lastm_tflv=m_OpVect.getTLVBynIndex(m_tflv.nIndex-1);
					CGlobalStruct::TFLV next_tflv;
					switch(StrToInt((char *)&m_tflv.lpdata))
					{
					case 0:
						sprintf(m_sql,"exec GT_ExpToPhysical %i",StrToInt((char *)&lastm_tflv.lpdata));
						break;
					case 1:
						sprintf(m_sql,"exec GT_ExpToSmart %i",StrToInt((char *)&lastm_tflv.lpdata));
						break;
					case 2:
						sprintf(m_sql,"exec GT_ExpToCharm %i",StrToInt((char *)&lastm_tflv.lpdata));
						break;
					case 3:
						next_tflv=m_OpVect.getTLVBynIndex(m_tflv.nIndex+1);
						sprintf(m_sql,"exec GT_ExpToLust %i,%i",StrToInt((char *)&lastm_tflv.lpdata),StrToInt((char *)&next_tflv.lpdata));
						break;
					case 4:
						next_tflv=m_OpVect.getTLVBynIndex(m_tflv.nIndex+1);
						sprintf(m_sql,"exec GT_ExpToFanta %i,%i",StrToInt((char *)&lastm_tflv.lpdata),StrToInt((char *)&next_tflv.lpdata));
						break;
					case 5:
						sprintf(m_sql,"exec GT_ExpToAllexp %i",StrToInt((char *)&lastm_tflv.lpdata));
						break;
					default:
						break;
					}
					QuerySingleValue("GT","127.0.0.1",4,m_sql,m_result);
					if(!strcmp(m_result,""))
					{
						sprintf(m_result,"无");
					}
					SendVectBuf.AddTFLV(CEnumCore::TagName::GT_Title,CEnumCore::TagFormat::TLV_STRING,strlen(m_result),(unsigned char*)&m_result);

				}
				else if(m_tflv.m_tagName == CEnumCore::TagName::GT_Sex)
				{

				}
				else
				{
					SendVectBuf.AddTFLV(m_tflv);//默认为直接添加TFLV
				}
			}
			else if(m_ServiceKey==CEnumCore::ServiceKey::GT_FriendTrade_Info_Resp)
			{
				if(m_tflv.m_tagName == CEnumCore::TagName::GT_IsSell)
				{
					if(!strcmp((char *)&m_tflv.lpdata,"")||!strcmp((char *)&m_tflv.lpdata,"NULL"))
					{
						sprintf(m_result,"No");
					}
					else
					{
						sprintf(m_result,"Yes");
					}	
					SendVectBuf.AddTFLV(CEnumCore::TagName::GT_IsSell,CEnumCore::TagFormat::TLV_STRING,strlen(m_result),(unsigned char*)&m_result);
				}
				else if(m_tflv.m_tagName == CEnumCore::TagName::GT_Price)
				{
					if(!strcmp((char *)&m_tflv.lpdata,"0"))
					{
						sprintf(m_result,"5000");
					}
					else
					{
						sprintf(m_result,"%s",m_tflv.lpdata);
					}

					SendVectBuf.AddTFLV(CEnumCore::TagName::GT_Price,CEnumCore::TagFormat::TLV_STRING,strlen(m_result),(unsigned char*)&m_result);

				}
				else 
				{
					SendVectBuf.AddTFLV(m_tflv);//默认为直接添加TFLV
				}
			}
			else if(m_ServiceKey==CEnumCore::ServiceKey::GT_ItemConsume_Query_Resp)
			{
				if(m_tflv.m_tagName == CEnumCore::TagName::GT_JW2Name)
				{
					sprintf(m_sql,"exec GT_ZoneidToName %i",StrToInt((char *)&m_tflv.lpdata));
					QuerySingleValue("GT","127.0.0.1",4,m_sql,m_result);
					if(!strcmp(m_result,"")||!strcmp(m_result,"NULL"))
					{
						sprintf(m_result,"%s",m_tflv.lpdata);
					}
					SendVectBuf.AddTFLV(CEnumCore::TagName::GT_JW2Name,CEnumCore::TagFormat::TLV_STRING,strlen(m_result),(unsigned char*)&m_result);


				}
				else
				{
					SendVectBuf.AddTFLV(m_tflv);//默认为直接添加TFLV
				}
			}

			else
			{
				if(m_tflv.m_tagName == CEnumCore::TagName::GT_GetReward)////获得奖励
				{
					char RewardName[512];
					ZeroMemory(RewardName,512);
					GetTreasReward(RewardName,(char*)m_tflv.lpdata,ServerIP);
					SendVectBuf.AddTFLV(CEnumCore::TagName::GT_GetReward,CEnumCore::TagFormat::TLV_STRING,strlen(RewardName),(unsigned char*)&RewardName);
				}
				else if(m_tflv.m_tagName==CEnumCore::TagName::GT_Financial)//财力值
				{
					
					sprintf(m_sql,"exec FindFinancial %i",StrToInt((char *)&m_tflv.lpdata));
					QuerySingleValue("GT","127.0.0.1",4,m_sql,m_result);
					int iNowFinancial=StrToInt((char *)m_tflv.lpdata)-StrToInt((char *)&m_result);
					_itoa(iNowFinancial,strInt,10);
					SendVectBuf.AddTFLV(CEnumCore::TagName::GT_Financial,CEnumCore::TagFormat::TLV_INTEGER,sizeof(int),(unsigned char*)&strInt);
					
				}
				else if(m_tflv.m_tagName == CEnumCore::TagName::GT_TaskReward2)//任务完成奖励2
				{
					if(strcmp("NULL",(char*)m_tflv.lpdata))
					{
						int nPos=StringFind((char*)m_tflv.lpdata,":",1);//第一个:的位置
						int mPos=StringFind((char*)m_tflv.lpdata,")",1);//第一个)的位置
						if(nPos>0)
						{
							char titleLvl[10];//保存称号的等级
							char titleName[20];//保存称号名称
							char RewardName[25];//保存最后返回的奖励名称
							ZeroMemory(titleLvl,10);
							ZeroMemory(titleName,20);
							ZeroMemory(RewardName,25);
							memcpy(titleLvl,(LPCSTR)&m_tflv.lpdata[nPos],mPos-nPos-1);
							getRemoteSql("GT","GT_GetTaskTitle",m_sql,4);//获取查询称号名称的sql语句
							ReplaceSingleValue(titleName,"",connLocalDB,0,m_sql,titleLvl,titleLvl);//获取称号
							sprintf(RewardName,"(称号：%s)",titleName);
							SendVectBuf.AddTFLV(CEnumCore::TagName::GT_TaskReward2,CEnumCore::TagFormat::TLV_STRING,strlen(RewardName),(unsigned char*)&RewardName);
						}
						else
						{
							SendVectBuf.AddTFLV(m_tflv);//默认为直接添加TFLV
						}	
					}					
					else
					{
						SendVectBuf.AddTFLV(m_tflv);//默认为直接添加TFLV
					}					
				}
				else
				{
					SendVectBuf.AddTFLV(m_tflv);//默认为直接添加TFLV
				}	
			}
		}
		iEnbodyLength = SendVectBuf.EnBody();
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"构建数据包内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return false;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"构建数据包文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return false;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"构建数据包其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return false;
	}
	return TRUE;
}
//查询替换单个值
void COperatorGT::ReplaceSingleValue(char* SingleValue, char* gamename, char* ServerIP, int nFlag, const char * szFormat, ...)
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
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"替换值内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"替换值文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"替换值其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
	}
}

//获取开启宝箱的道具奖励
void COperatorGT::GetTreasReward(char* RewardName,char* ItemList,char * ServerIP)
{
	try
	{
		char tmpRewardName[512];
		ZeroMemory(tmpRewardName,512);//保存临时的道具信息
		char SingleRewardName[64];
		ZeroMemory(SingleRewardName,64);//保存查询得到的单个道具名称

		char strType[10];
		char strCode[10];
		ZeroMemory(strType,10);//用于保存char*类型的道具类型
		ZeroMemory(strCode,10);//用于保存char*类型的道具ID
		int itemtype=0,itemcode=0;//道具类型和道具ID

		int count=1;//判断是第几次取出道具用"|"分隔出来的道具信息
		int beginPos = 0;//保存道具类型字符串的初始位置
		int midPos = StringFind(ItemList,"*",count);//获取第一个"*"字符的初始位置
		int endPos = StringFind(ItemList,"|",count);//获取第一个"|"字符的初始位置
		
		while(midPos>0)
		{			
			if(endPos>0)
			{
				ZeroMemory(strType,10);//用于保存char*类型的道具类型
	        	ZeroMemory(strCode,10);//用于保存char*类型的道具ID
				memcpy(strType,(char*)&ItemList[beginPos],midPos-beginPos-1);
		    	memcpy(strCode,(char*)&ItemList[midPos],endPos-midPos-1);
			}
			else
			{
				ZeroMemory(strType,10);//用于保存char*类型的道具类型
	        	ZeroMemory(strCode,10);//用于保存char*类型的道具ID
				memcpy(strType,(char*)&ItemList[beginPos],midPos-beginPos-1);
				sprintf(strCode,"%s",(char*)&ItemList[midPos]);
			}
			itemtype = StrToInt(strType);
			itemcode = StrToInt(strCode);

			getRemoteSql("GT","GT_GetItemName", m_sql,4);//获取道具对应的可装备/使用职业

			ReplaceSingleValue(SingleRewardName,"GT",ServerIP,0,m_sql,"gtown_db_gamedb",itemtype,itemcode);//获取单个称号
			if(!strcmp("",SingleRewardName))
			{
				sprintf(tmpRewardName,"%s|%i*%i",RewardName, itemtype,itemcode);
				sprintf(RewardName,tmpRewardName);
			}
			else
			{
				sprintf(tmpRewardName,"%s|%s", RewardName, SingleRewardName);
				sprintf(RewardName,tmpRewardName);//将奖励的道具信息合并起来
			}

			count++;//继续向后分割
			beginPos = endPos;//获取第count个道具类型的初始位置
			midPos = StringFind(ItemList,"*",count);//获取第count个"*"的位置信息
	    	endPos = StringFind(ItemList,"|",count);//获取第count个"|"的位置信息		
		}
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"道具奖励转换内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"道具奖励转换文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"道具奖励转换其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
	}
}

//查找字符串中特定的字符第一次出现的位置
int COperatorGT::StringFind(const char* string,const char* find,int number)
{
	int count = 0;
	try
	{
		char* pos = (char *)string;
		char *p=pos;
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
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查找字符串中特定的字符内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return -1;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查找字符串中特定的字符文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return -1;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查找字符串中特定的字符其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return -1;
	}
	
	return count;
}


bool COperatorGT::TagIDToSqlType(CEnumCore::Message_Tag_ID m_tagID,char * sql_type)
{
	try
	{
		switch(m_tagID)
		{
		case CEnumCore::Message_Tag_ID::GT_User_DetailInfo_Query:
			sprintf(sql_type,"%s","GT_UserInfo_Detail_New");
			break;
		case CEnumCore::Message_Tag_ID::GT_User_FamilyInfo_Query:
			sprintf(sql_type,"%s","GT_User_FamilyInfo_New");
			break;
		case CEnumCore::Message_Tag_ID::GT_User_SentiInfo_Query:
			sprintf(sql_type,"%s","GT_User_SentiInfo_New");
			break;
		case CEnumCore::Message_Tag_ID::GT_User_MarrigeInfo_Query:
			sprintf(sql_type,"%s","GT_User_MarriageInfo_New");
			break;
		case CEnumCore::Message_Tag_ID::GT_UserPack_Query:
			sprintf(sql_type,"%s","GT_User_PackInfo_New");
			break;
		case CEnumCore::Message_Tag_ID::GT_UserEquip_Query:
			sprintf(sql_type,"%s","GT_User_EquipInfo_New");
			break;
		case CEnumCore::Message_Tag_ID::GT_IMFriend_Query:
			sprintf(sql_type,"%s","GT_FriendInfo_New");
			break;
		case CEnumCore::Message_Tag_ID::GT_IMBlackList_Query:
			sprintf(sql_type,"%s","GT_BlackList_New");
			break;
		case CEnumCore::Message_Tag_ID::GT_IMSecretLove_Query:
			sprintf(sql_type,"%s","GT_SecretLove_Info_New");
			break;
		case CEnumCore::Message_Tag_ID::GT_PetInfo_Query:
			sprintf(sql_type,"%s","GT_PetInfo_New");
			break;
		case CEnumCore::Message_Tag_ID::GT_RoomInfo_Query:
			sprintf(sql_type,"%s","GT_User_RoomInfo_New");
			break;
		case CEnumCore::Message_Tag_ID::GT_FriendTrade_Info:////朋友买卖中角色信息
			sprintf(sql_type,"%s","GT_FriendTrade_Info_New");
			break;
		case CEnumCore::Message_Tag_ID::GT_HelotInfo_Query:
			sprintf(sql_type,"%s","GT_HelotInfo_Query_New");
			break;
		case CEnumCore::Message_Tag_ID::GT_FriendTrade_Log://///朋友买卖中买卖日志
			sprintf(sql_type,"%s","GT_FriendTrade_Log_New");
			break;
		default:
			return FALSE;
		}
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"TagName转字符串内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return FALSE;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"TagName转字符串文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return FALSE;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"TagName转字符串其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return FALSE;
	}

	return TRUE;
}
bool COperatorGT::TagIDToSqlType(CEnumCore::Message_Tag_ID m_tagID,int type,char * sql_type)
{
	try
	{
		if(type==1)
		{
			switch(m_tagID)
			{
			case CEnumCore::Message_Tag_ID::GT_TradeLog_Query:
				sprintf(sql_type,"%s","GT_TradeLog_Sender_New");
				break;
			case CEnumCore::Message_Tag_ID::GT_MailLog_Query:
				sprintf(sql_type,"%s","GT_MailLog_Sender_New");
				break;
			default:
				return FALSE;
			}
			
		}
		else if(type==2)
		{
			switch(m_tagID)
			{
			case CEnumCore::Message_Tag_ID::GT_TradeLog_Query:
				sprintf(sql_type,"%s","GT_TradeLog_Receiver_New");
				break;
			case CEnumCore::Message_Tag_ID::GT_MailLog_Query:
				sprintf(sql_type,"%s","GT_MailLog_Receiver_New");
				break;
			default:
				return FALSE;
			}
		}
		else
		{
			switch(m_tagID)
			{
			case CEnumCore::Message_Tag_ID::GT_TaskLog_Query:
				sprintf(sql_type,"%s","GT_TaskFinish_Log_New");
				break;
			case CEnumCore::Message_Tag_ID::GT_UseItemLog_Query:
				sprintf(sql_type,"%s","GT_UseItem_Log_New");
				break;
			case CEnumCore::Message_Tag_ID::GT_GuajiLog_Query:
				sprintf(sql_type,"%s","GT_OnlineReward_Log_New");
				break;
			case CEnumCore::Message_Tag_ID::GT_BugleLog_Query:
				sprintf(sql_type,"%s","GT_Bugle_Log_New");
				break;
			case CEnumCore::Message_Tag_ID::GT_LoginLog_Query:
				sprintf(sql_type,"%s","GT_Login_Log_New");
				break;
			default:
				return FALSE;
			}
		}
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"TagName转字符串内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return FALSE;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"TagName转字符串文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return FALSE;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"TagName转字符串其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return FALSE;
	}
	
	return TRUE;
}