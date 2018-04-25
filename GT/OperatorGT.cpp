#include "stdafx.h"
#include ".\OperatorGT.h"

//���캯��
COperatorGT::COperatorGT(void)
{	
}

//��������
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
		int allTFLV = m_OpVect.getAllTFLV();//��ȡ���е�TFLV����
		if(allTFLV<=0)
		{
			return false;
		}
		SendVectBuf.InitCreate(allTFLV, 1);//��ʼ�����͵�����


		vector<CGlobalStruct::TFLV>::iterator iter;
		vector<CGlobalStruct::TFLV> v_DateSet=m_OpVect.GetInnerList();
		for( iter = v_DateSet.begin(); iter != v_DateSet.end(); iter++ )
		{

			CGlobalStruct::TFLV m_tflv = *iter;//��ȡÿһ��TFLV

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
						sprintf(m_result,"��");
					}
					SendVectBuf.AddTFLV(CEnumCore::TagName::GT_Title,CEnumCore::TagFormat::TLV_STRING,strlen(m_result),(unsigned char*)&m_result);

				}
				else if(m_tflv.m_tagName == CEnumCore::TagName::GT_Sex)
				{

				}
				else
				{
					SendVectBuf.AddTFLV(m_tflv);//Ĭ��Ϊֱ�����TFLV
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
					SendVectBuf.AddTFLV(m_tflv);//Ĭ��Ϊֱ�����TFLV
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
					SendVectBuf.AddTFLV(m_tflv);//Ĭ��Ϊֱ�����TFLV
				}
			}

			else
			{
				if(m_tflv.m_tagName == CEnumCore::TagName::GT_GetReward)////��ý���
				{
					char RewardName[512];
					ZeroMemory(RewardName,512);
					GetTreasReward(RewardName,(char*)m_tflv.lpdata,ServerIP);
					SendVectBuf.AddTFLV(CEnumCore::TagName::GT_GetReward,CEnumCore::TagFormat::TLV_STRING,strlen(RewardName),(unsigned char*)&RewardName);
				}
				else if(m_tflv.m_tagName==CEnumCore::TagName::GT_Financial)//����ֵ
				{
					
					sprintf(m_sql,"exec FindFinancial %i",StrToInt((char *)&m_tflv.lpdata));
					QuerySingleValue("GT","127.0.0.1",4,m_sql,m_result);
					int iNowFinancial=StrToInt((char *)m_tflv.lpdata)-StrToInt((char *)&m_result);
					_itoa(iNowFinancial,strInt,10);
					SendVectBuf.AddTFLV(CEnumCore::TagName::GT_Financial,CEnumCore::TagFormat::TLV_INTEGER,sizeof(int),(unsigned char*)&strInt);
					
				}
				else if(m_tflv.m_tagName == CEnumCore::TagName::GT_TaskReward2)//������ɽ���2
				{
					if(strcmp("NULL",(char*)m_tflv.lpdata))
					{
						int nPos=StringFind((char*)m_tflv.lpdata,":",1);//��һ��:��λ��
						int mPos=StringFind((char*)m_tflv.lpdata,")",1);//��һ��)��λ��
						if(nPos>0)
						{
							char titleLvl[10];//����ƺŵĵȼ�
							char titleName[20];//����ƺ�����
							char RewardName[25];//������󷵻صĽ�������
							ZeroMemory(titleLvl,10);
							ZeroMemory(titleName,20);
							ZeroMemory(RewardName,25);
							memcpy(titleLvl,(LPCSTR)&m_tflv.lpdata[nPos],mPos-nPos-1);
							getRemoteSql("GT","GT_GetTaskTitle",m_sql,4);//��ȡ��ѯ�ƺ����Ƶ�sql���
							ReplaceSingleValue(titleName,"",connLocalDB,0,m_sql,titleLvl,titleLvl);//��ȡ�ƺ�
							sprintf(RewardName,"(�ƺţ�%s)",titleName);
							SendVectBuf.AddTFLV(CEnumCore::TagName::GT_TaskReward2,CEnumCore::TagFormat::TLV_STRING,strlen(RewardName),(unsigned char*)&RewardName);
						}
						else
						{
							SendVectBuf.AddTFLV(m_tflv);//Ĭ��Ϊֱ�����TFLV
						}	
					}					
					else
					{
						SendVectBuf.AddTFLV(m_tflv);//Ĭ��Ϊֱ�����TFLV
					}					
				}
				else
				{
					SendVectBuf.AddTFLV(m_tflv);//Ĭ��Ϊֱ�����TFLV
				}	
			}
		}
		iEnbodyLength = SendVectBuf.EnBody();
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�������ݰ��ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return false;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�������ݰ��ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return false;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�������ݰ������쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return false;
	}
	return TRUE;
}
//��ѯ�滻����ֵ
void COperatorGT::ReplaceSingleValue(char* SingleValue, char* gamename, char* ServerIP, int nFlag, const char * szFormat, ...)
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
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�滻ֵ�ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�滻ֵ�ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�滻ֵ�����쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
	}
}

//��ȡ��������ĵ��߽���
void COperatorGT::GetTreasReward(char* RewardName,char* ItemList,char * ServerIP)
{
	try
	{
		char tmpRewardName[512];
		ZeroMemory(tmpRewardName,512);//������ʱ�ĵ�����Ϣ
		char SingleRewardName[64];
		ZeroMemory(SingleRewardName,64);//�����ѯ�õ��ĵ�����������

		char strType[10];
		char strCode[10];
		ZeroMemory(strType,10);//���ڱ���char*���͵ĵ�������
		ZeroMemory(strCode,10);//���ڱ���char*���͵ĵ���ID
		int itemtype=0,itemcode=0;//�������ͺ͵���ID

		int count=1;//�ж��ǵڼ���ȡ��������"|"�ָ������ĵ�����Ϣ
		int beginPos = 0;//������������ַ����ĳ�ʼλ��
		int midPos = StringFind(ItemList,"*",count);//��ȡ��һ��"*"�ַ��ĳ�ʼλ��
		int endPos = StringFind(ItemList,"|",count);//��ȡ��һ��"|"�ַ��ĳ�ʼλ��
		
		while(midPos>0)
		{			
			if(endPos>0)
			{
				ZeroMemory(strType,10);//���ڱ���char*���͵ĵ�������
	        	ZeroMemory(strCode,10);//���ڱ���char*���͵ĵ���ID
				memcpy(strType,(char*)&ItemList[beginPos],midPos-beginPos-1);
		    	memcpy(strCode,(char*)&ItemList[midPos],endPos-midPos-1);
			}
			else
			{
				ZeroMemory(strType,10);//���ڱ���char*���͵ĵ�������
	        	ZeroMemory(strCode,10);//���ڱ���char*���͵ĵ���ID
				memcpy(strType,(char*)&ItemList[beginPos],midPos-beginPos-1);
				sprintf(strCode,"%s",(char*)&ItemList[midPos]);
			}
			itemtype = StrToInt(strType);
			itemcode = StrToInt(strCode);

			getRemoteSql("GT","GT_GetItemName", m_sql,4);//��ȡ���߶�Ӧ�Ŀ�װ��/ʹ��ְҵ

			ReplaceSingleValue(SingleRewardName,"GT",ServerIP,0,m_sql,"gtown_db_gamedb",itemtype,itemcode);//��ȡ�����ƺ�
			if(!strcmp("",SingleRewardName))
			{
				sprintf(tmpRewardName,"%s|%i*%i",RewardName, itemtype,itemcode);
				sprintf(RewardName,tmpRewardName);
			}
			else
			{
				sprintf(tmpRewardName,"%s|%s", RewardName, SingleRewardName);
				sprintf(RewardName,tmpRewardName);//�������ĵ�����Ϣ�ϲ�����
			}

			count++;//�������ָ�
			beginPos = endPos;//��ȡ��count���������͵ĳ�ʼλ��
			midPos = StringFind(ItemList,"*",count);//��ȡ��count��"*"��λ����Ϣ
	    	endPos = StringFind(ItemList,"|",count);//��ȡ��count��"|"��λ����Ϣ		
		}
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"���߽���ת���ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"���߽���ת���ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"���߽���ת�������쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
	}
}

//�����ַ������ض����ַ���һ�γ��ֵ�λ��
int COperatorGT::StringFind(const char* string,const char* find,int number)
{
	int count = 0;
	try
	{
		char* pos = (char *)string;
		char *p=pos;
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
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�����ַ������ض����ַ��ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return -1;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�����ַ������ض����ַ��ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return -1;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�����ַ������ض����ַ������쳣,������Ϣ:%s",errMessage);
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
		case CEnumCore::Message_Tag_ID::GT_FriendTrade_Info:////���������н�ɫ��Ϣ
			sprintf(sql_type,"%s","GT_FriendTrade_Info_New");
			break;
		case CEnumCore::Message_Tag_ID::GT_HelotInfo_Query:
			sprintf(sql_type,"%s","GT_HelotInfo_Query_New");
			break;
		case CEnumCore::Message_Tag_ID::GT_FriendTrade_Log://///����������������־
			sprintf(sql_type,"%s","GT_FriendTrade_Log_New");
			break;
		default:
			return FALSE;
		}
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"TagNameת�ַ����ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return FALSE;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"TagNameת�ַ����ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return FALSE;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"TagNameת�ַ��������쳣,������Ϣ:%s",errMessage);
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
		sprintf(errMessage,"TagNameת�ַ����ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return FALSE;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"TagNameת�ַ����ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return FALSE;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"TagNameת�ַ��������쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return FALSE;
	}
	
	return TRUE;
}