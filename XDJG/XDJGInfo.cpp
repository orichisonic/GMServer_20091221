#include "StdAfx.h"
#include "XDJGInfo.h"

CXDJGInfo::CXDJGInfo(void)
{
	m_VecSendItemInfo.clear();
	m_VecGuildItemInfo.clear();
}

CXDJGInfo::~CXDJGInfo(void)
{
	m_VecSendItemInfo.clear();
	m_VecGuildItemInfo.clear();
}

//��ʼ�����еı���
COperVector * CXDJGInfo::initialize(CSession * pSess,CEnumCore::Message_Tag_ID MessageTagID, unsigned char * pbuf, int length)
{
	COperVector * operVector = NULL;
	try
	{
		ZeroMemory(LocalIP, 30);
		ZeroMemory(AccountDB, 30);
		ZeroMemory(LogDB, 30);
		logFile.ReadValue("Scheme", "DATABASE", "Address",LocalIP, 30);//��ȡ���ص����ݿ��ַ

		logFile.ReadValue("Database", "XDJG", "AccountDB", AccountDB, 30);

		logFile.ReadValue("Database", "XDJG", "LogDB", LogDB, 30);

		logFile.ReadValue("Database", "XDJG", "CharacterDB", GameDB, 30);

		//Added by tengjie 2010-07-08:For ItemName
//		m_VecItemInfo.clear();
//		GetItemInfo();
		//End

//		port = logFile.ReadIntValue("XDJG", "SERVER", "PORT");

		//���û����ʼ������
		operVector = CGameInfo::initialize(pSess, MessageTagID, pbuf, length);
	}
	catch(...)
	{
		operVector = NULL;
	}

	return operVector;
}

//Added by tengjie 2010-07-08
void CXDJGInfo::GetItemInfo(Vec_ItemInfo &VecItemInfo)
{
	string szSql = "";
	string szData = "";
	vector<CGlobalStruct::DataRecord> datarecord;
	st_ItemInfo _ItemInfo;
	
//	m_VecItemInfo.clear();
	if(!VecItemInfo.empty())
		VecItemInfo.clear();
	
	szSql = "exec XDJG_GetItemInfo";
	
	m_SqlHelper.SqlHelperMain("XDJG", &datarecord,"127.0.0.1",(char*)szSql.c_str(),4,0,0);
	
	printf("++++++++++++datarecord's size: %d\n", datarecord.size());
	if(datarecord.empty())
		return;

	vector<CGlobalStruct::DataRecord>::iterator iter;
	for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
	{
		if(!strcmp("ItemID", iter->recordfieldname))
		{
			_ItemInfo.iItemId = atoi((char*)iter->recorddata);
		}
		else if(!strcmp("ItemName", iter->recordfieldname))
		{
			_ItemInfo.strItemName = (char*)iter->recorddata;//��¼��������
			VecItemInfo.push_back(_ItemInfo);
		}
		else
			continue;
	}

	printf("���߸���:%d\n",VecItemInfo.size());
}

vector <CGlobalStruct::TFLV> CXDJGInfo::QueryPlayerInfo(char *ServerIP, string strCharacter, int type, int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	string strResult;
	vector <string> vectorColInfo;
	char *strConn = new char[512];
	char *strParams = new char[512];
	ZeroMemory(strConn, 512);
	ZeroMemory(strParams, 512);

	try
	{
		sprintf(strParams, "ServerIP:%s strCharacter:%s type:%d\n", \
						ServerIP, strCharacter.c_str(), type);
		logFile.WriteLog("<�������>",operName, strParams);//��LOG��ӡ�������

		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=query_player(%s,%d)",
			ServerIP, HTTPPORT,
			strCharacter.c_str(), type);
		
		if(m_operXDJG.getWebInfo(strConn, strResult))
		{
			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "�û���ѯ");				
			}
			else
			{
				int iResult = atoi(strResult.c_str());
				
				if (iResult>=0)
				{
					
					bool bFinishFlag = false;
					int Maxnum_wait = 10, wait_num=0;
					
					while (!bFinishFlag)
					{
						sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=get_operation_result(%d)", 
							ServerIP, HTTPPORT,
							iResult);
						
						if (!vectorColInfo.empty())
						{
							vectorColInfo.clear();
						}
						
						m_operXDJG.getWebInfo(strConn, vectorColInfo);
						
						int iResultFlag = atoi((vectorColInfo.front()).c_str());
						
						switch (iResultFlag)
						{
						case 0:
							bFinishFlag = true;
							getUserInfoDataset(DBTFLV, vectorColInfo, (char *)strCharacter.c_str(), ServerIP, iIndex, iPageSize, type);
							break;
						case -2:
							wait_num++;
							if (wait_num > Maxnum_wait)
							{
								bFinishFlag = true; 
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�ȴ���ʱ", "�û���ѯ");
							}
							Sleep(1000);
							break;
						default:
							bFinishFlag = true;
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ѯʧ��", "�û���ѯ");	
						}
					}
				}
				else
				{
					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "�û���ѯ");
					
				}
			}
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "�û���ѯ");
		}
	}
	catch (...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "�û���ѯ");
	}

	delete []strConn;
	delete []strParams;

	return DBTFLV;
}

//�û���Ϣ��ѯ
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_Net_UserInfo(char *Account, char * Character, char *ServerIP, int type, int iIndex, int iPageSize)
{

	vector <CGlobalStruct::TFLV> DBTFLV;
	vector <CGlobalStruct::TFLV> New_DBTFLV;
	vector <CGlobalStruct::TFLV>::iterator pNewDBTFLV = NULL;
	vector <CGlobalStruct::TFLV> Del_DBTFLV;
	vector <CGlobalStruct::TFLV>::iterator pDelDBTFLV = NULL;
	string strResult;
	vector <string> vectorColInfo;
	vector<CGlobalStruct::DataRecord> datarecord;
	CString strSql = "";
	char realServerIP[40] = "";
	string strCharacter = "";

	char *strConn = new char[512];
	char *strParams = new char[512];
	ZeroMemory(strConn, 512);
	ZeroMemory(strParams, 512);

	try
	{
		sprintf(strParams, "[��ɫ��Ϣ��ѯ]Account:%s Character:%s ServerIP:%s type:%d\n", \
			Account, Character, ServerIP, type);
			logFile.WriteLog("<�������>",operName, strParams);//��LOG��ӡ�������

		if(strcmp(Account,""))
		{
			if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,3))
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "�û���Ϣ��ѯ");
				return DBTFLV;//�޷���ȡ�����ķ�����IP
			}
			
			strSql.Format("Select name from %s.character where account_name = '%s' and Deleted = 0",GameDB, Account);
	
			m_SqlHelper.SqlHelperMain("XDJG", &datarecord,realServerIP,strSql.GetBuffer(0),12,0,0);
			
			int iCount = 0;
			vector<CGlobalStruct::DataRecord>::iterator iter;
			for(iter = datarecord.begin(); iter != datarecord.end(); iter++)
			{
				if(!strcmp(iter->recordfieldname,"name"))
				{
					New_DBTFLV.clear();
					iCount++;
					strCharacter = iter->recorddata;
					New_DBTFLV = QueryPlayerInfo(ServerIP, strCharacter, type, iIndex, iPageSize);

					if(New_DBTFLV.size() == 1)
						continue;

					for(pNewDBTFLV = New_DBTFLV.begin(); pNewDBTFLV != New_DBTFLV.end(); pNewDBTFLV++)
					{
						DBTFLV.push_back(*pNewDBTFLV);
					}
				}
				else
				{
					iCount++;
					continue;
				}
			}

			//ȡ��ɾ���Ľ�ɫ��Ϣ
			Del_DBTFLV = GetDeletedRolesInfo(realServerIP, Account, iIndex, iPageSize);
			for(pDelDBTFLV = Del_DBTFLV.begin(); pDelDBTFLV != Del_DBTFLV.end(); pDelDBTFLV++)
			{
				DBTFLV.push_back(*pDelDBTFLV);
			}

			if(DBTFLV.size() == 0)
				DBTFLV = New_DBTFLV;
		}
		else
		{
			sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=query_player(%s,%d)",
				ServerIP, HTTPPORT,
				Character, type);
			
			if(m_operXDJG.getWebInfo(strConn, strResult))
			{
				if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
				{
					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "�û���ѯ");				
				}
				else
				{
					int iResult = atoi(strResult.c_str());
					
					if (iResult>=0)
					{
						
						bool bFinishFlag = false;
						int Maxnum_wait = 10, wait_num=0;
						
						while (!bFinishFlag)
						{
							sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=get_operation_result(%d)", 
								ServerIP, HTTPPORT,
								iResult);
							
							if (!vectorColInfo.empty())
							{
								vectorColInfo.clear();
							}
							
							m_operXDJG.getWebInfo(strConn, vectorColInfo);
							
							int iResultFlag = atoi((vectorColInfo.front()).c_str());
							
							switch (iResultFlag)
							{
							case 0:
								bFinishFlag = true;
								getUserInfoDataset(DBTFLV, vectorColInfo, Character, ServerIP, iIndex, iPageSize, type);
								
								break;
							case -2:
								wait_num++;
								if (wait_num > Maxnum_wait)
								{
									bFinishFlag = true; 
									DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�ȴ���ʱ", "�û���ѯ");
									
								}
								Sleep(1000);
								break;
							default:
								bFinishFlag = true;
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ѯʧ��", "�û���ѯ");
								
							}
						}
					}
					else
					{
						DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "�û���ѯ");
						
					}		
					
				}
				
			}
			else
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "�û���ѯ");
				
			}
		}

	}catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "�û���ѯ");
	}

	delete []strConn;
	delete []strParams;

	return DBTFLV;
}

//ȡ��ɾ���Ľ�ɫ��Ϣ
vector <CGlobalStruct::TFLV> CXDJGInfo::GetDeletedRolesInfo(char *ServerIP, char *Account, int iIndex, int iPageSize)
{
	CGlobalStruct::TFLV m_tflv;
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	CString szData = "";
	vector<CGlobalStruct::DataRecord> datarecord;
	vector<CGlobalStruct::DataRecord>::iterator pData = NULL;
	string strPosition = "";
	char *strParams = new char[512];
	ZeroMemory(strParams, 512);
	
	try
	{
		sprintf(strParams, "[��ɾ���Ľ�ɫ��Ϣ��ѯ]Account:%s ServerIP:%s\n", Account, ServerIP);
		logFile.WriteLog("<�������>",operName, strParams);//��LOG��ӡ�������

		strSql.Format("SELECT NAME AS XDJG_Character, guid AS XDJG_CharacterID, account_name AS XDJG_Account, \
							  account_id AS XDJG_AccountID, grade AS XDJG_Level, property AS XDJG_RoleProperty, \
							  CASE sex WHEN 0 THEN 'Ů' ELSE '��' END AS XDJG_Sex, \
							  CASE profession WHEN 1 THEN 'սʿ' \
											  WHEN 2 THEN '������' \
											  WHEN 3 THEN '��еʦ' \
											  WHEN 4 THEN '����ʦ' \
											  WHEN 5 THEN '�ػ���' \
											  WHEN 6 THEN '��ʴ��' \
											  ELSE 'δ֪' END AS XDJG_Profession, scene_type_id AS XDJG_SceneTypeID, \
							  scene_obj_id AS XDJG_SceneInstanceID,position_x AS XDJG_PositionX,position_y AS XDJG_PositionY, \
							  position_z AS XDJG_PositionZ, money AS XDJG_Money,'��ɾ��' AS XDJG_IsDeleted \
						 FROM %s.character \
						WHERE account_name = '%s' AND deleted = 1", \
							  GameDB,Account);
		
		m_SqlHelper.SqlHelperMain("XDJG", &datarecord,ServerIP,strSql.GetBuffer(0),12,iIndex,iPageSize);

		if(datarecord.size() == 0)
			return DBTFLV;

		for(pData = datarecord.begin(); pData != datarecord.end(); pData++)
		{
			if(!strcmp(pData->recordfieldname,"XDJG_Character"))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_Character;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(pData->recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",pData->recorddata);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp(pData->recordfieldname,"XDJG_CharacterID"))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_CharacterID;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(pData->recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",pData->recorddata);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp(pData->recordfieldname,"XDJG_Account"))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_Account;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(pData->recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",pData->recorddata);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp(pData->recordfieldname,"XDJG_AccountID"))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_AccountID;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(pData->recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",pData->recorddata);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp(pData->recordfieldname,"XDJG_Level"))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_Level;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(pData->recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",pData->recorddata);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp(pData->recordfieldname,"XDJG_RoleProperty"))
			{
				memcpy(&m_StructInfo.m_sCharacterProperty, pData->recorddata, sizeof(m_StructInfo.m_sCharacterProperty));
				szData.Format("%d", m_StructInfo.m_sCharacterProperty.experience);
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_RoleProperty;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=szData.GetLength();
				sprintf((char *)&m_tflv.lpdata,"%s",szData.GetBuffer(0));
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp(pData->recordfieldname,"XDJG_Sex"))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_Sex;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(pData->recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",pData->recorddata);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp(pData->recordfieldname,"XDJG_Profession"))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_Profession;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(pData->recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",pData->recorddata);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp(pData->recordfieldname,"XDJG_SceneTypeID"))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_Account;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(pData->recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",pData->recorddata);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp(pData->recordfieldname,"XDJG_SceneInstanceID"))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_AccountID;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(pData->recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",pData->recorddata);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp(pData->recordfieldname,"XDJG_PositionX"))
			{
				strPosition += pData->recorddata;
				strPosition += ",";
			}
			else if(!strcmp(pData->recordfieldname,"XDJG_PositionY"))
			{
				strPosition += pData->recorddata;
				strPosition += ",";
			}
			else if(!strcmp(pData->recordfieldname,"XDJG_PositionZ"))
			{
				strPosition += pData->recorddata;
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_Position;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strPosition.length();
				sprintf((char *)&m_tflv.lpdata,"%s",strPosition.c_str());
				DBTFLV.push_back(m_tflv);

				strPosition = "";
			}
			else if(!strcmp(pData->recordfieldname,"XDJG_Money"))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_Money;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(pData->recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",pData->recorddata);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp(pData->recordfieldname,"XDJG_IsDeleted"))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_IsDeleted;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(pData->recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",pData->recorddata);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp(pData->recordfieldname,"PageCount"))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::PageCount;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(pData->recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",pData->recorddata);
				DBTFLV.push_back(m_tflv);
			}
		}
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "��ɾ���Ľ�ɫ��Ϣ��ѯ");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	delete []strParams;

	return DBTFLV;
}

//�ڲ������û���Ϣ��ѯ
bool CXDJGInfo::getUserInfoDataset(vector <CGlobalStruct::TFLV> &DataSetTFLV, vector<string> vectorCol, char *Character, char *ServerIP, int iIndex, int iPageSize, int type)
{

	try
	{
		char strParams[512];
		ZeroMemory(strParams,512);
		sprintf(strParams, "Character:%s ServerIP:%s type:%d\n", Character, ServerIP, type);
		logFile.WriteLog("<�������>",operName, strParams);//��LOG��ӡ�������

		CGlobalStruct::TFLV m_tflv;
		int iPageCount = 0;
		int iCol = 0;
		vector<string> newDataVector;

		vector <CGlobalStruct::TFLV> DBTFLV;
		vector <CGlobalStruct::TFLV>::iterator pDBTFLV = NULL;

		vector <CGlobalStruct::TFLV> VecDBTFLV;
		vector <CGlobalStruct::TFLV>::iterator pVecDBTFLV = NULL;

		//Modified by tengjie 2010-07-06
// 		switch (type)
// 		{
// 		case 1:
// 		case 3:
// 			iCol = 12;
// 			break;
// 		case 2:
// 		case 4:
// 			break;
// 		}
		
		iCol = vectorCol.size();

		//����ҳ����ÿҳ��¼����������Ҫ�Ĳ�ѯ����
		m_operXDJG.getPageVector(newDataVector, iPageCount, vectorCol, iIndex, iPageSize, iCol);
		//	m_operXDJG.getPageVector(newDataVector1, iPageCount, vectorCol, iIndex, iPageSize, iCol);//test
		
		vector<string>::iterator iter_newDataVector;
		
		int iColNum = 0;
		if (type == 1 || type == 3)
		{
			for (iter_newDataVector=newDataVector.begin(); iter_newDataVector!=newDataVector.end(); iter_newDataVector++)
			{
				iColNum = iColNum%iCol;
				switch (iColNum)
				{
				case 0:
					//Character
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_Character;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=(*iter_newDataVector).length();
					sprintf((char *)&m_tflv.lpdata,"%s",(*iter_newDataVector).c_str());
					DataSetTFLV.push_back(m_tflv);
					break;
				case 1:
					//CharacterID
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_CharacterID;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=(*iter_newDataVector).length();
					sprintf((char *)&m_tflv.lpdata,"%s",(*iter_newDataVector).c_str());
					DataSetTFLV.push_back(m_tflv);
					break;
				case 2:
					//Account
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_Account;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=(*iter_newDataVector).length();
					sprintf((char *)&m_tflv.lpdata,"%s",(*iter_newDataVector).c_str());
					DataSetTFLV.push_back(m_tflv);
					break;
				case 3:
					//AccountID
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_AccountID;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=(*iter_newDataVector).length();
					sprintf((char *)&m_tflv.lpdata,"%s",(*iter_newDataVector).c_str());
					DataSetTFLV.push_back(m_tflv);
					break;
				case 4:
					//Level
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_Level;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=(*iter_newDataVector).length();
					sprintf((char *)&m_tflv.lpdata,"%s",(*iter_newDataVector).c_str());
					DataSetTFLV.push_back(m_tflv);
					break;
				case 5:
					//Exp
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_Exp;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=(*iter_newDataVector).length();
					sprintf((char *)&m_tflv.lpdata,"%s",(*iter_newDataVector).c_str());
					DataSetTFLV.push_back(m_tflv);
					break;
				case 6:
					//Sex
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_Sex;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=(*iter_newDataVector).length();
					sprintf((char *)&m_tflv.lpdata,"%s",(*iter_newDataVector).c_str());
					DataSetTFLV.push_back(m_tflv);
					break;
				case 7:
					//Profession
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_Profession;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=(*iter_newDataVector).length();
					sprintf((char *)&m_tflv.lpdata,"%s",(*iter_newDataVector).c_str());
					DataSetTFLV.push_back(m_tflv);
					break;
				case 8:
					//SceneTypeID
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_SceneTypeID;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=(*iter_newDataVector).length();
					sprintf((char *)&m_tflv.lpdata,"%s",(*iter_newDataVector).c_str());
					DataSetTFLV.push_back(m_tflv);
					break;
				case 9:
					//SceneInstanceID
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_SceneInstanceID;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=(*iter_newDataVector).length();
					sprintf((char *)&m_tflv.lpdata,"%s",(*iter_newDataVector).c_str());
					DataSetTFLV.push_back(m_tflv);
					break;
				case 10:
					//Position
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_Position;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=(*iter_newDataVector).length();
					sprintf((char *)&m_tflv.lpdata,"%s",(*iter_newDataVector).c_str());
					DataSetTFLV.push_back(m_tflv);
					break;
				case 11:
					//Money
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_Money;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=(*iter_newDataVector).length();
					sprintf((char *)&m_tflv.lpdata,"%s",(*iter_newDataVector).c_str());
					DataSetTFLV.push_back(m_tflv);

					//Isdeleted
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_IsDeleted;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=strlen("δɾ��")+1;
					sprintf((char *)&m_tflv.lpdata,"%s","δɾ��");
					DataSetTFLV.push_back(m_tflv);
					
					//PageCount
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((char *)&m_tflv.lpdata,"%d", iPageCount);
					DataSetTFLV.push_back(m_tflv);
					break;
				default:
					break;
				}
				
				iColNum++;
			}


		}
		else
		{
			//������ϸ��Ϣ��ʾ
			for (iter_newDataVector=newDataVector.begin(); iter_newDataVector!=newDataVector.end(); iter_newDataVector++)
			{
				iColNum = iColNum%newDataVector.size();
				switch (iColNum)
				{
				case 0:
					//Character
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_Character;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=(*iter_newDataVector).length();
					sprintf((char *)&m_tflv.lpdata,"%s",(*iter_newDataVector).c_str());
					DataSetTFLV.push_back(m_tflv);
					break;
				case 1:
					//CharacterID
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_CharacterID;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=(*iter_newDataVector).length();
					sprintf((char *)&m_tflv.lpdata,"%s",(*iter_newDataVector).c_str());
					DataSetTFLV.push_back(m_tflv);
					break;
				case 2:
					//Account
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_Account;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=(*iter_newDataVector).length();
					sprintf((char *)&m_tflv.lpdata,"%s",(*iter_newDataVector).c_str());
					DataSetTFLV.push_back(m_tflv);
					break;
				case 3:
					//AccountID
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_AccountID;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=(*iter_newDataVector).length();
					sprintf((char *)&m_tflv.lpdata,"%s",(*iter_newDataVector).c_str());
					DataSetTFLV.push_back(m_tflv);
					break;
				case 4:
					//Level
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_Level;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=(*iter_newDataVector).length();
					sprintf((char *)&m_tflv.lpdata,"%s",(*iter_newDataVector).c_str());
					DataSetTFLV.push_back(m_tflv);
					break;
				case 5:
					//Exp
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_Exp;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=(*iter_newDataVector).length();
					sprintf((char *)&m_tflv.lpdata,"%s",(*iter_newDataVector).c_str());
					DataSetTFLV.push_back(m_tflv);
					break;
				case 6:
					//Sex
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_Sex;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=(*iter_newDataVector).length();
					sprintf((char *)&m_tflv.lpdata,"%s",(*iter_newDataVector).c_str());
					DataSetTFLV.push_back(m_tflv);
					break;
				case 7:
					//Profession
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_Profession;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=(*iter_newDataVector).length();
					sprintf((char *)&m_tflv.lpdata,"%s",(*iter_newDataVector).c_str());
					DataSetTFLV.push_back(m_tflv);
					break;
				case 8:
					//SceneTypeID
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_SceneTypeID;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=(*iter_newDataVector).length();
					sprintf((char *)&m_tflv.lpdata,"%s",(*iter_newDataVector).c_str());
					DataSetTFLV.push_back(m_tflv);
					break;
				case 9:
					//SceneInstanceID
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_SceneInstanceID;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=(*iter_newDataVector).length();
					sprintf((char *)&m_tflv.lpdata,"%s",(*iter_newDataVector).c_str());
					DataSetTFLV.push_back(m_tflv);
					break;
				case 10:
					//Position
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_Position;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=(*iter_newDataVector).length();
					sprintf((char *)&m_tflv.lpdata,"%s",(*iter_newDataVector).c_str());
					DataSetTFLV.push_back(m_tflv);
					break;
				case 11:
					//Money
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_Money;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=(*iter_newDataVector).length();
					sprintf((char *)&m_tflv.lpdata,"%s",(*iter_newDataVector).c_str());
					DataSetTFLV.push_back(m_tflv);
					break;
				case 12:
					//Strength
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_Strength;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=(*iter_newDataVector).length();
					sprintf((char *)&m_tflv.lpdata,"%s",(*iter_newDataVector).c_str());
					DataSetTFLV.push_back(m_tflv);
					break;
				case 13:
					//Agility
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_Agility;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=(*iter_newDataVector).length();
					sprintf((char *)&m_tflv.lpdata,"%s",(*iter_newDataVector).c_str());
					DataSetTFLV.push_back(m_tflv);
					break;
				case 14:
					//Stamina
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_Stamina;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=(*iter_newDataVector).length();
					sprintf((char *)&m_tflv.lpdata,"%s",(*iter_newDataVector).c_str());
					DataSetTFLV.push_back(m_tflv);
					break;
				case 15:
					//Spirit
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_Spirit;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=(*iter_newDataVector).length();
					sprintf((char *)&m_tflv.lpdata,"%s",(*iter_newDataVector).c_str());
					DataSetTFLV.push_back(m_tflv);
					break;
				case 16:
					//Magic
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_Magic;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=(*iter_newDataVector).length();
					sprintf((char *)&m_tflv.lpdata,"%s",(*iter_newDataVector).c_str());
					DataSetTFLV.push_back(m_tflv);
					break;
				case 17:
					//NearPa
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_NearPa;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=(*iter_newDataVector).length();
					sprintf((char *)&m_tflv.lpdata,"%s",(*iter_newDataVector).c_str());
					DataSetTFLV.push_back(m_tflv);
					break;
				case 18:
					//FarPa
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_FarPa;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=(*iter_newDataVector).length();
					sprintf((char *)&m_tflv.lpdata,"%s",(*iter_newDataVector).c_str());
					DataSetTFLV.push_back(m_tflv);
					break;
				case 19:
					//SuperAttack
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_SuperAttack;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=(*iter_newDataVector).length();
					sprintf((char *)&m_tflv.lpdata,"%s",(*iter_newDataVector).c_str());
					DataSetTFLV.push_back(m_tflv);
					break;
				case 20:
					//PerfectRate
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_PerfectRate;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=(*iter_newDataVector).length();
					sprintf((char *)&m_tflv.lpdata,"%s",(*iter_newDataVector).c_str());
					DataSetTFLV.push_back(m_tflv);
					break;
				case 21:
					//StrengthCritRate
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_StrengthCritRate;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=(*iter_newDataVector).length();
					sprintf((char *)&m_tflv.lpdata,"%s",(*iter_newDataVector).c_str());
					DataSetTFLV.push_back(m_tflv);
					break;
				case 22:
					//SpiritCritRate
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_SpiritCritRate;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=(*iter_newDataVector).length();
					sprintf((char *)&m_tflv.lpdata,"%s",(*iter_newDataVector).c_str());
					DataSetTFLV.push_back(m_tflv);
					break;

				//������Ϣ:
				case 23:
					//����ֵ
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_Armor;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=(*iter_newDataVector).length();
					sprintf((char *)&m_tflv.lpdata,"%s",(*iter_newDataVector).c_str());
					DataSetTFLV.push_back(m_tflv);
					break;
				case 24:
					//������
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_DodgeRate;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=(*iter_newDataVector).length();
					sprintf((char *)&m_tflv.lpdata,"%s",(*iter_newDataVector).c_str());
					DataSetTFLV.push_back(m_tflv);
					break;
				case 25:
					//����
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_WardRate;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=(*iter_newDataVector).length();
					sprintf((char *)&m_tflv.lpdata,"%s",(*iter_newDataVector).c_str());
					DataSetTFLV.push_back(m_tflv);
					break;
				case 26:
					//�м���
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_ParryRate;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=(*iter_newDataVector).length();
					sprintf((char *)&m_tflv.lpdata,"%s",(*iter_newDataVector).c_str());
					DataSetTFLV.push_back(m_tflv);
					break;
				case 27:
					//�����ֿ���
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_FatalResistanceRate;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=(*iter_newDataVector).length();
					sprintf((char *)&m_tflv.lpdata,"%s",(*iter_newDataVector).c_str());
					DataSetTFLV.push_back(m_tflv);
					break;
				case 28:
					//���ܿ���
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_FlameResistance;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=(*iter_newDataVector).length();
					sprintf((char *)&m_tflv.lpdata,"%s",(*iter_newDataVector).c_str());
					DataSetTFLV.push_back(m_tflv);
					break;
				case 29:
					//PK
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_PK;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=(*iter_newDataVector).length();
					sprintf((char *)&m_tflv.lpdata,"%s",(*iter_newDataVector).c_str());
					DataSetTFLV.push_back(m_tflv);
					break;
				case 30:
					//�ƶ�
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_GoodEvil;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=(*iter_newDataVector).length();
					sprintf((char *)&m_tflv.lpdata,"%s",(*iter_newDataVector).c_str());
					DataSetTFLV.push_back(m_tflv);
					break;
				case 31:
					//����ʱ��
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_RedNameTime;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=(*iter_newDataVector).length();
					sprintf((char *)&m_tflv.lpdata,"%s",(*iter_newDataVector).c_str());
					DataSetTFLV.push_back(m_tflv);
					break;
				default:
					break;
				}
				iColNum++;
			}

			//��ȡ��ɫ�ƺ��б�
			DBTFLV = GetRoleTitles(ServerIP, Character);
			for(pDBTFLV = DBTFLV.begin(); pDBTFLV != DBTFLV.end(); pDBTFLV++)
			{
				DataSetTFLV.push_back(*pDBTFLV);
			}

			//ȡ������б�
			VecDBTFLV = GetLifeSkills(ServerIP, Character);
			for(pVecDBTFLV = VecDBTFLV.begin(); pVecDBTFLV != VecDBTFLV.end(); pVecDBTFLV++)
			{
				DataSetTFLV.push_back(*pVecDBTFLV);
			}

			//PageCount
			m_tflv.nIndex=DataSetTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PageCount;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((char *)&m_tflv.lpdata,"%d", iPageCount);
			DataSetTFLV.push_back(m_tflv);

		}
	}
	catch(...)
	{
		return false;
	}

	return true;
}

//��ȡ��ɫ�����б�
vector <CGlobalStruct::TFLV> CXDJGInfo::GetRoleTitles(char *ServerIP, char *Character)
{
	CGlobalStruct::TFLV m_tflv;
	vector <CGlobalStruct::TFLV> DBTFLV;
	vector<CGlobalStruct::DataRecord> datarecord;
	CString strData = "";
	CString strSql = "";
	char realServerIP[30] = "";
	int iCount = 0;
	char *strParams = new char[512];
	ZeroMemory(strParams, 512);

	try
	{
		sprintf(strParams, "[��ɫ�����б�]ServerIP:%s Character:%s\n", ServerIP, Character);
		logFile.WriteLog("<�������>",operName, strParams);//��LOG��ӡ�������

		//��ɫ�ƺ�,�����
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,3))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "��ɫ�����б�");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		//��ɫ�ƺ��б�
		strSql.Format("Select titles AS XDJG_Titles \
						 from %s.character \
						where name = '%s'",GameDB, Character);
		
		m_SqlHelper.SqlHelperMain("XDJG", &datarecord,realServerIP,strSql.GetBuffer(0),12,0,0);
		
		vector<CGlobalStruct::DataRecord>::iterator iter;
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			if(!strcmp("XDJG_Titles", iter->recordfieldname))
			{
				memcpy(&m_StructInfo.m_sCharacterTitle, iter->recorddata, sizeof(m_StructInfo.m_sCharacterTitle));//��Ʒ����
				break;
			}
			else
				continue;
		}

		//��ɫ�ƺ��б�
		for(iCount = 0;iCount < m_StructInfo.m_sCharacterTitle.titles_count; iCount++)
		{
			switch(iCount)
			{
			case 0:
				strData.Format("%d", m_StructInfo.m_sCharacterTitle.titles_list[iCount]);
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_RoleID1;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strData.GetLength()+1;
				sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
				DBTFLV.push_back(m_tflv);
				break;
			case 1:
				strData.Format("%d", m_StructInfo.m_sCharacterTitle.titles_list[iCount]);
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_RoleID2;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strData.GetLength()+1;
				sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
				DBTFLV.push_back(m_tflv);
				break;
			case 2:
				strData.Format("%d", m_StructInfo.m_sCharacterTitle.titles_list[iCount]);
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_RoleID3;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strData.GetLength()+1;
				sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
				DBTFLV.push_back(m_tflv);
				break;
			case 3:
				strData.Format("%d", m_StructInfo.m_sCharacterTitle.titles_list[iCount]);
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_RoleID4;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strData.GetLength()+1;
				sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
				DBTFLV.push_back(m_tflv);
				break;
			case 4:
				strData.Format("%d", m_StructInfo.m_sCharacterTitle.titles_list[iCount]);
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_RoleID5;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strData.GetLength()+1;
				sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
				DBTFLV.push_back(m_tflv);
				break;
			case 5:
				strData.Format("%d", m_StructInfo.m_sCharacterTitle.titles_list[iCount]);
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_RoleID6;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strData.GetLength()+1;
				sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
				DBTFLV.push_back(m_tflv);
				break;
			case 6:
				strData.Format("%d", m_StructInfo.m_sCharacterTitle.titles_list[iCount]);
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_RoleID7;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strData.GetLength()+1;
				sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
				DBTFLV.push_back(m_tflv);
				break;
			case 7:
				strData.Format("%d", m_StructInfo.m_sCharacterTitle.titles_list[iCount]);
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_RoleID8;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strData.GetLength()+1;
				sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
				DBTFLV.push_back(m_tflv);
				break;
			default:
				break;
			}
		}
	}
	catch (...)
	{
		return DBTFLV;
	}

	delete []strParams;

	return DBTFLV;
}

//��ȡ��ɫ������б�
vector <CGlobalStruct::TFLV> CXDJGInfo::GetLifeSkills(char *ServerIP, char *Character)
{
	CGlobalStruct::TFLV m_tflv;
	vector <CGlobalStruct::TFLV> DBTFLV;
	vector<CGlobalStruct::DataRecord> datarecord;
	CString strSql = "";
	char realServerIP[30] = "";
	int iNum = 1;

	try
	{
		char strParams[512];
		ZeroMemory(strParams,512);
		sprintf(strParams, "[��ɫ������б�]ServerIP:%s Character:%s\n", ServerIP, Character);
		logFile.WriteLog("<�������>",operName, strParams);//��LOG��ӡ�������

		//��ɫ�ƺ�,�����
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,3))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "���������Ϣ��ѯ");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		strSql.Format("Select prototype AS XDJG_Prototype \
						 from %s.civil \
						where charindex = (select guid from %s.character where name = '%s')",GameDB, GameDB, Character);
		
		m_SqlHelper.SqlHelperMain("XDJG", &datarecord,realServerIP,strSql.GetBuffer(0),12,0,0);
		
		vector<CGlobalStruct::DataRecord>::iterator iter_next;
		for( iter_next = datarecord.begin(); iter_next != datarecord.end(); iter_next++ )
		{
			if(!strcmp("XDJG_Prototype", iter_next->recordfieldname))
			{
				switch(iNum)
				{
				case 1:
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_LifeSkill1;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=GetLifeSkillName(iter_next->recorddata).length()+1;
					sprintf((char *)&m_tflv.lpdata,"%s", GetLifeSkillName(iter_next->recorddata).c_str());
					DBTFLV.push_back(m_tflv);
					break;
				case 2:
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_LifeSkill2;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=GetLifeSkillName(iter_next->recorddata).length()+1;
					sprintf((char *)&m_tflv.lpdata,"%s", GetLifeSkillName(iter_next->recorddata).c_str());
					DBTFLV.push_back(m_tflv);
					break;
				case 3:
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_LifeSkill3;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=GetLifeSkillName(iter_next->recorddata).length()+1;
					sprintf((char *)&m_tflv.lpdata,"%s", GetLifeSkillName(iter_next->recorddata).c_str());
					DBTFLV.push_back(m_tflv);
					break;
				case 4:
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_LifeSkill4;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=GetLifeSkillName(iter_next->recorddata).length()+1;
					sprintf((char *)&m_tflv.lpdata,"%s", GetLifeSkillName(iter_next->recorddata).c_str());
					DBTFLV.push_back(m_tflv);
					break;
				case 5:
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_LifeSkill5;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=GetLifeSkillName(iter_next->recorddata).length()+1;
					sprintf((char *)&m_tflv.lpdata,"%s", GetLifeSkillName(iter_next->recorddata).c_str());
					DBTFLV.push_back(m_tflv);
					break;
				case 6:
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_LifeSkill6;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=GetLifeSkillName(iter_next->recorddata).length()+1;
					sprintf((char *)&m_tflv.lpdata,"%s", GetLifeSkillName(iter_next->recorddata).c_str());
					DBTFLV.push_back(m_tflv);
					break;
				case 7:
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_LifeSkill7;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=GetLifeSkillName(iter_next->recorddata).length()+1;
					sprintf((char *)&m_tflv.lpdata,"%s", GetLifeSkillName(iter_next->recorddata).c_str());
					DBTFLV.push_back(m_tflv);
					break;
				case 8:
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_LifeSkill8;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=GetLifeSkillName(iter_next->recorddata).length()+1;
					sprintf((char *)&m_tflv.lpdata,"%s", GetLifeSkillName(iter_next->recorddata).c_str());
					DBTFLV.push_back(m_tflv);
					break;
				default:
					break;
				}

				iNum++;
			}
			else
				continue;
		}
	}
	catch (...)
	{
		return DBTFLV;
	}

	return DBTFLV;
}

//��ȡ���������
string CXDJGInfo::GetLifeSkillName(char *Prototype)
{
	string strSkillName = "";
	int iType = atoi(Prototype);

	try
	{
		if(Prototype == NULL)
			return strSkillName;

		switch(iType)
		{
		case 1:
			strSkillName = "ұ��";
			break;
		case 2:
			strSkillName = "��Ƥ";
			break;
		case 3:
			strSkillName = "����";
			break;
		case 4:
			strSkillName = "��Դ";
			break;
		case 5:
			strSkillName = "��ѧ";
			break;
		case 6:
			strSkillName = "����";
			break;
		case 7:
			strSkillName = "�ɹ�";
			break;
		case 8:
			strSkillName = "��е";
			break;
		case 9:
			strSkillName = "ѭ��";
			break;
		case 10:
			strSkillName = "оƬ";
			break;
		case 11:
			strSkillName = "���";
			break;
		case 12:
			strSkillName = "��ʻ";
			break;
		case 13:
			strSkillName = "��ҵ";
			break;
		case 14:
			strSkillName = "��ͨʰȡ";
			break;
		default:
			strSkillName = "δ֪";
			break;
		}
	}
	catch (...)
	{
		return strSkillName;
	}

	return strSkillName;
}

//ͨ���ӿ����ˣ��ʺ����˺ͽ�ɫ����
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_Net_KickPlayer(int userByID, char *ServerIP, char * Character, char * Account)
{
	vector <CGlobalStruct::TFLV> DBTFLV;

	string strResult;
	vector <string> vectorColInfo;
	char retMessage[255];
	memset(retMessage, 0, 255);
	char strConn[512];
	ZeroMemory(strConn, 512);

	try
	{
		sprintf(retMessage, "���ˣ�<����>%s, <�ʺ�>%s, <��ɫ��>%s", ServerIP, Account, Character);
		logFile.WriteLog("<�������>",operName,"XDJG����");//��LOG
		
		if (strcmp(Account,""))
		{
			sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=kick_account(%s)", 
				ServerIP, HTTPPORT,
				Account);
		}
		else
		{
			sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=kick_player(%s)", 
				ServerIP, HTTPPORT,			
				Character);
		}
		
		if(m_operXDJG.getWebInfo(strConn, strResult))
		{
			switch (strResult[0])
			{
			case '0':
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�û�������", "XDJG����");
				break;
			case '1':
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "���˳ɹ�", "XDJG����");
				break;
			default:
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "XDJG����");
				break;
			}
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "XDJG����");
		}

	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "XDJG����");

	}

	m_operXDJG.GmOperLog(userByID, ServerIP, "XDJG����", retMessage, DBTFLV[0]);
	return DBTFLV;
}

vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_Net_ClosePlayer_List(char *ServerIP, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	char *operRemoteSql = new char[2048];
	ZeroMemory(operRemoteSql,2048);
	char realServerIP[40];
	char message[100];
	ZeroMemory(message,100);

	try
	{
		sprintf(message,"[��ҷ�ͣ�б��ѯ]:<����>%s",ServerIP);
		logFile.WriteLog("<�������>",operName,message);//��LOG

		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,4))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "���������Ϣ��ѯ");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}

		if(!m_operXDJG.getRemoteSql("XDJG","XDJG_ACCOUNT_CLOSE_LIST",operRemoteSql,1))
		{
			return DBTFLV;
		}

#ifdef _DEBUG
		printf(operRemoteSql, AccountDB, AccountDB);
#endif

		DBTFLV = GameDBQuery("XDJG",realServerIP,12, iIndex, iPageSize, operRemoteSql, AccountDB, AccountDB);
		
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "��ͣ�б��ѯ");;//��������Ϣд����־�в����ش�����Ϣ
	}

	delete []operRemoteSql;

	return DBTFLV;	
}

vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_Net_ClosePlayer(int userByID, char *ServerIP, char *Account, char *AccountID, int iTime,char *Reason,int iReasonType)
{
	vector <CGlobalStruct::TFLV> DBTFLV;

	string strResult;
	vector <string> vectorColInfo;
	char strConn[512];
	CString szData = "";
	ZeroMemory(strConn, 512);
	char retMessage[255];
	memset(retMessage, 0, 255);
	int iBanTime = iTime*24*60;
	
	try
	{
		if (strcmp(Account,""))
		{
			sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=kick_account(%s)", 
				ServerIP, HTTPPORT,
				Account);
		}

		if(!m_operXDJG.getWebInfo(strConn, strResult))
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "XDJG����");

		switch(iReasonType)
		{
		case _ST_XDJG::WEB_FREEZE:
			szData.Format("���˺��ѱ�����\n������%s���½",XDJG_CalEndFreezeTime(iBanTime).GetBuffer(0));
			sprintf(retMessage, "��ҷ�ͣ��<����>%s, <�ʺ�>%s, <��ͣʱ��>%d��, <��ͣԭ��>%s", ServerIP, Account, iTime,szData.GetBuffer(0));
			sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=disable_account(%s,%d,%s,%d)", 
					ServerIP, HTTPPORT,
					Account, iBanTime, "������",iReasonType);
			break;
		case _ST_XDJG::WEB_LOCK:
			szData.Format("%s","���˺�������ҳ�����������½�ٷ���ҳ�����н����󷽿ɵ�½");
			sprintf(retMessage, "��ҷ�ͣ��<����>%s, <�ʺ�>%s, <��ͣʱ��>%d��, <��ͣԭ��>%s", ServerIP, Account, iTime,Reason);
			sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=disable_account(%s,%d,%s,%d)", 
					ServerIP, HTTPPORT,
					Account, iBanTime, "������",iReasonType);
			break;
		case _ST_XDJG::MAX_LOGINTIMES:
			sprintf(retMessage, "��ҷ�ͣ��<����>%s, <�ʺ�>%s, <��ͣʱ��>%d��, <��ͣԭ��>%s", ServerIP, Account, iTime,Reason);
			sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=disable_account(%s,%d,%s,%d)", 
				ServerIP, HTTPPORT,
				Account, iBanTime, "�ﵽ�������¼����",iReasonType);
			break;
		case _ST_XDJG::BANED_BYGM:
			sprintf(retMessage, "��ҷ�ͣ��<����>%s, <�ʺ�>%s, <��ͣʱ��>%d��, <��ͣԭ��>%s", ServerIP, Account, iTime,Reason);
			sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=disable_account(%s,%d,%s,%d)", 
				ServerIP, HTTPPORT,
				Account, iBanTime, "��GM��ͣ",iReasonType);
			break;
		case _ST_XDJG::BANED_BYPLUG:
			sprintf(retMessage, "��ҷ�ͣ��<����>%s, <�ʺ�>%s, <��ͣʱ��>%d��, <��ͣԭ��>%s", ServerIP, Account, iTime,Reason);
			sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=disable_account(%s,%d,%s,%d)", 
				ServerIP, HTTPPORT,
				Account, iBanTime, "��ҷ�ͣ",iReasonType);
			break;
		case _ST_XDJG::NONE_REASON:
			sprintf(retMessage, "��ҷ�ͣ��<����>%s, <�ʺ�>%s, <��ͣʱ��>%d��, <��ͣԭ��>%s", ServerIP, Account, iTime,Reason);
			sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=disable_account(%s,%d,%s,%d)", 
				ServerIP, HTTPPORT,
				Account, iBanTime, "��������ͣ",iReasonType);
			break;
		default:
			sprintf(retMessage, "��ҷ�ͣ��<����>%s, <�ʺ�>%s, <��ͣʱ��>%d��, <��ͣԭ��>%s", ServerIP, Account, iTime,Reason);
			sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=disable_account(%s,%d,%s,%d)", 
					ServerIP, HTTPPORT,
					Account, iBanTime, Reason,iReasonType);
			break;
		}
		
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG

		if(m_operXDJG.getWebInfo(strConn, strResult))
		{
			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "XDJG��ͣ");				
			}
			else
			{
				int iResult = atoi(strResult.c_str());
				if (iResult>=0)
				{
					
					bool bFinishFlag = false;
					int Maxnum_wait = 10, wait_num=0;
					
					while (!bFinishFlag)
					{
						sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=get_operation_result(%d)", 
							ServerIP, HTTPPORT,
							iResult);

						//Added by tengjie 2010-07-07
						Sleep(200);

						m_operXDJG.getWebInfo(strConn, vectorColInfo);
						int iResultFlag = atoi((vectorColInfo.front()).c_str());
						switch (iResultFlag)
						{
						case 1:
							bFinishFlag = true;
							
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ͣ�ɹ�", "XDJG��ͣ");
							/*
							
							  if(	GameDBOper("XDJG","127.0.0.1",4,"exec XDJG_BanUser %d, %s, %s, %s,%d,'%s'",
							  userByID,
							  Account,
							  AccountID,
							  ServerIP,
							  iTime,
							  Reason))
							  {
							  DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ͣ�ɹ�", "XDJG��ͣ");
							  }
							  else
							  {
							  DBTFLV = m_operXDJG.ReturnOpMsg(operName, "���뱾�����ݿ����", "XDJG��ͣ");
						}*/
							
							
							break;
						case -2:
							wait_num++;
							if (wait_num > Maxnum_wait)
							{
								bFinishFlag = true; 
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�Ժ�����", "XDJG��ͣ");
								
							}
							Sleep(1500);
							break;
						default:
							bFinishFlag = true;
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ͣʧ��", "XDJG��ͣ");
						}
					}
				}
				else
				{
					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "XDJG��ͣ");
				}

			}

		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "XDJG��ͣ");
		}
		
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "XDJG��ͣ");

	}

	m_operXDJG.GmOperLog(userByID, ServerIP, "XDJG��ͣ", retMessage, DBTFLV[0]);
	return DBTFLV;
}

vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_Net_OpenPlayer(int userByID, char *ServerIP, char *Account, char *Reason,int iReasonType)
{
	vector <CGlobalStruct::TFLV> DBTFLV;

	string strResult;
	vector <string> vectorColInfo;
	char strConn[512];
	ZeroMemory(strConn, 512);
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{
		sprintf(retMessage, "��ҽ��:<����>%s, <�ʺ�>%s, <���ԭ��>%s", ServerIP, Account, Reason);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=enable_account(%s,%d)", 
			ServerIP, HTTPPORT,
			Account,iReasonType);
		
		if(m_operXDJG.getWebInfo(strConn, strResult))
		{
			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "��ҽ��");				
			}
			else
			{
				int iResult = atoi(strResult.c_str());
				if (iResult>=0)
				{
					
					bool bFinishFlag = false;
					int Maxnum_wait = 10, wait_num=0;
					
					while (!bFinishFlag)
					{
						sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=get_operation_result(%d)", 
							ServerIP, HTTPPORT,
							iResult);
						m_operXDJG.getWebInfo(strConn, strResult);
						int iResultFlag = atoi(strResult.c_str());
						switch (iResultFlag)
						{
						case 1:
							bFinishFlag = true;
							
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "���ɹ�", "��ҽ��");

							break;
						case -2:
							wait_num++;
							if (wait_num > Maxnum_wait)
							{
								bFinishFlag = true; 
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�ȴ���ʱ", "��ҽ��");
								
							}
							Sleep(1000);
							break;
						default:
							bFinishFlag = true;
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "���ʧ��", "��ҽ��");
						}
					}
				}
				else
				{
					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "��ҽ��");
				}

			}

		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "��ҽ��");		
		}
	}
	catch (...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "��ҽ��");	
	}

	m_operXDJG.GmOperLog(userByID, ServerIP, "��ҽ��", retMessage, DBTFLV[0]);
	return DBTFLV;
}
//��ѯ����
vector<CGlobalStruct::TFLV> CXDJGInfo::XDJG_Net_BoardTasker_Query(char * GameName,char * m_sql,int iIndex,int iPageSize)
{
	logFile.WriteLog("<�������>",operName,"�����ѯ");//��LOG
	return GameDBQuery(GameName, LocalIP, 4, iIndex, iPageSize,m_sql);
}

//��ӹ���
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_Net_BoardTasker_Add(int userByID, char * ServerIP, char *BoardMessage, char *BeginTime, char *EndTime, int interval)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	char retMessage[255];
	memset(retMessage, 0, 255);
	
	try
	{
		sprintf(retMessage, "��ӹ���:<����>%s, <����>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, BoardMessage, BeginTime, EndTime);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG

		printf("exec XDJG_BOARDTASK_INSERT %i,'%s','%s','%s','%s',%i", 
								userByID,ServerIP,BoardMessage,BeginTime, EndTime, interval);

		int tmpLength = 0;
		tmpLength = GameDBOper("XDJG",LocalIP,4,"exec XDJG_BOARDTASK_INSERT %i,'%s','%s','%s','%s',%i",
								userByID,ServerIP,BoardMessage,BeginTime, EndTime, interval);
		
		if(tmpLength == 0)
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ӹ���ʧ��", "�������");
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ӹ���ɹ�", "�������");
		}
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ӹ������", "�������");
	}

	return DBTFLV;
}

vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_Net_BoardTasker_Update(int userByID, char * ServerIP, int Taskid, char *BoardMessage, char *BeginTime, char *EndTime, int interval)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{
		sprintf(retMessage, "�����޸�:<����>%s, <����ID>%d, <����>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Taskid, BoardMessage, BeginTime, EndTime);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG

		printf("exec XDJG_BOARDTASK_UPDATE %i, %i, '%s','%s','%s','%s',%i", 
			userByID,Taskid, ServerIP,BoardMessage,BeginTime, EndTime, interval);		
		int tmpLength = 0;
		tmpLength = GameDBOper("XDJG",LocalIP,4,"exec XDJG_BOARDTASK_UPDATE %i, %i, '%s','%s','%s','%s',%i", 
			userByID,Taskid, ServerIP,BoardMessage,BeginTime, EndTime, interval);
		
		if(tmpLength == 0)
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "���¹���ʧ��", "�������");
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "���¹���ɹ�", "�������");
		}
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "���¹������", "�������");
	}
	
	return DBTFLV;
}

vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_Net_BoardTasker_Delete(int userByID, int Taskid)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{
		sprintf(retMessage, "����ɾ��:<����ID>%d", Taskid);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG

		int tmpLength = 0;
		tmpLength = GameDBOper("XDJG",LocalIP,4,"exec XDJG_BOARDTASK_DELETE %i, %i", 
			userByID,Taskid);
		
		if(tmpLength == 0)
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "ɾ������ʧ��", "�������");
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "ɾ������ɹ�", "�������");
		}
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "ɾ���������", "�������");
	}
	
	return DBTFLV;
}

//Added by tengjie 2010-05-17
CString CXDJGInfo::XDJG_CalEndFreezeTime(int iTime)
{
	CString szTime = "";

	try
	{
		CTime time = CTime::GetCurrentTime();
		SYSTEMTIME timeDest;
		time.GetAsSystemTime(timeDest);

		if((timeDest.wMinute+iTime)/60 > 0)
		{
			timeDest.wHour += (timeDest.wMinute+iTime)/60;
			timeDest.wMinute = (timeDest.wMinute+iTime)%60;
			if(timeDest.wHour/24 > 0)
			{
				timeDest.wDay += timeDest.wHour/24;
				timeDest.wHour = timeDest.wHour%24;
				switch(timeDest.wMonth)
				{
				case 1:
				case 3:
				case 5:
				case 7:
				case 8:
				case 10:
				case 12:
					if(timeDest.wDay/31 > 0)
					{
						timeDest.wMonth += timeDest.wDay/31;
						timeDest.wDay = timeDest.wDay%31;
						if(timeDest.wMonth/12 > 0)
						{
							timeDest.wYear += timeDest.wMonth/12;
							timeDest.wMonth = timeDest.wMonth%12;
						}
					}
					break;
				case 2:
					if(timeDest.wDay/28 > 0)
					{
						timeDest.wMonth += timeDest.wDay/28;
						timeDest.wDay = timeDest.wDay%28;
						if(timeDest.wMonth/12 > 0)
						{
							timeDest.wYear += timeDest.wMonth/12;
							timeDest.wMonth = timeDest.wMonth%12;
						}
					}
					break;
				case 4:
				case 6:
				case 9:
				case 11:
					if(timeDest.wDay/30 > 0)
					{
						timeDest.wMonth += timeDest.wDay/30;
						timeDest.wDay = timeDest.wDay%30;
						if(timeDest.wMonth/12 > 0)
						{
							timeDest.wYear += timeDest.wMonth/12;
							timeDest.wMonth = timeDest.wMonth%12;
						}
					}
					break;
				default:
					break;
				}
			}
		}
		else
			timeDest.wMinute += iTime;

		szTime.Format("%04d��%02d��%02d�� %02d:%02d",timeDest.wYear, timeDest.wMonth, timeDest.wDay, timeDest.wHour, timeDest.wMinute);
	}
	catch (...)
	{
		
	}

	return szTime;
}

//Added by tengjie 2010-07-06
//��ɫ�������
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_Net_RoleSkillAdd(int userByID, char *ServerIP, char *Character, int SkillID)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	vector<CGlobalStruct::DataRecord> datarecord;
	string strResult;
	CString strSql = "";
	vector <string> vectorColInfo;
	char strConn[512];
	ZeroMemory(strConn, 512);
	char retMessage[255];
	ZeroMemory(retMessage, 255);
	char realServerIP[40];
	ZeroMemory(realServerIP, 40);
	int iCount = 0;
	
	try
	{
		sprintf(retMessage, "��ɫ�������:<����>%s, <��ɫ��>%s, <����ID>%d", ServerIP, Character, SkillID);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,3))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "��ɫ�������");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		strSql.Format("Select skills AS XDJG_Skills from %s.character \
						where name = '%s'" , GameDB, Character);

		m_SqlHelper.SqlHelperMain("XDJG", &datarecord,realServerIP,strSql.GetBuffer(0),12,0,0);
		
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("XDJG_Skills", m_DataRecord.recordfieldname))
			{
				memcpy(&m_StructInfo.m_sSkillCont, m_DataRecord.recorddata, sizeof(m_StructInfo.m_sSkillCont));//��Ʒ����
				break;
			}
			else
				continue;
		}

		for(iCount = 0; iCount < m_StructInfo.m_sSkillCont.skill_count; iCount++)
		{
			if(SkillID == m_StructInfo.m_sSkillCont.skill_list[iCount])
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�ý�ɫ��ӵ�д˼���", "XDJG��ɫ�������");
				return DBTFLV;
			}
		}

		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=add_skill(%s,%d)", 
			ServerIP, HTTPPORT,
			Character,SkillID);
		
		if(m_operXDJG.getWebInfo(strConn, strResult))
		{
			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "XDJG��ɫ�������");				
			}
			else
			{
				int iResult = atoi(strResult.c_str());
				if (iResult>=0)
				{
					
					bool bFinishFlag = false;
					int Maxnum_wait = 10, wait_num=0;
					
					while (!bFinishFlag)
					{
						sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=get_operation_result(%d)", 
							ServerIP, HTTPPORT,
							iResult);
						m_operXDJG.getWebInfo(strConn, strResult);
						int iResultFlag = atoi(strResult.c_str());
						switch (iResultFlag)
						{
						case 1:
							bFinishFlag = true;
							
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��Ӽ��ܳɹ�", "XDJG��ɫ�������");
							
							break;
						case -2:
							wait_num++;
							if (wait_num > Maxnum_wait)
							{
								bFinishFlag = true; 
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��Ӽ��ܳ�ʱ", "XDJG��ɫ�������");
								
							}
							Sleep(1000);
							break;
						default:
							bFinishFlag = true;
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��Ӽ���ʧ��", "XDJG��ɫ�������");
						}
					}
				}
				else
				{
					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "XDJG��ɫ�������");
				}
				
			}
			
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "XDJG��ɫ�������");		
		}
	}
	catch (...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "XDJG��ɫ�������");
	}
	
	m_operXDJG.GmOperLog(userByID, ServerIP, "XDJG��ɫ�������", retMessage, DBTFLV[0]);
	return DBTFLV;
}

//��ɫ����ɾ��
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_Net_RoleSkillDel(int userByID, char *ServerIP, char *Character, int SkillID)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	
	string strResult;
	vector <string> vectorColInfo;
	char strConn[512];
	ZeroMemory(strConn, 512);
	char retMessage[255];
	memset(retMessage, 0, 255);
	
	try
	{
		sprintf(retMessage, "��ɫ����ɾ��:<����>%s, <��ɫ��>%s, <����ID>%d", ServerIP, Character, SkillID);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=remove_skill(%s,%d)", 
			ServerIP, HTTPPORT,
			Character,SkillID);
		
		if(m_operXDJG.getWebInfo(strConn, strResult))
		{
			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "XDJG��ɫ����ɾ��");				
			}
			else
			{
				int iResult = atoi(strResult.c_str());
				if (iResult>=0)
				{
					bool bFinishFlag = false;
					int Maxnum_wait = 10, wait_num=0;
					
					while (!bFinishFlag)
					{
						sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=get_operation_result(%d)", 
							ServerIP, HTTPPORT,
							iResult);
						m_operXDJG.getWebInfo(strConn, strResult);
						int iResultFlag = atoi(strResult.c_str());
						switch (iResultFlag)
						{
						case 1:
							bFinishFlag = true;
							
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "ɾ�����ܳɹ�", "XDJG��ɫ����ɾ��");
							
							break;
						case -2:
							wait_num++;
							if (wait_num > Maxnum_wait)
							{
								bFinishFlag = true; 
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "ɾ�����ܳ�ʱ", "XDJG��ɫ����ɾ��");
								
							}
							Sleep(1000);
							break;
						default:
							bFinishFlag = true;
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "ɾ������ʧ��", "XDJG��ɫ����ɾ��");
						}
					}
				}
				else
				{
					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "XDJG��ɫ����ɾ��");
				}
			}
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "XDJG��ɫ����ɾ��");		
		}
	}
	catch (...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "XDJG��ɫ����ɾ��");	
	}
	
	m_operXDJG.GmOperLog(userByID, ServerIP, "XDJG��ɫ����ɾ��", retMessage, DBTFLV[0]);
	return DBTFLV;
}

//�������
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_Net_ItemAdd(int userByID, char *ServerIP, char *Character, char *SendCharacter, char *mailTitle, char *mailContent, int Money, char *ItemID, char *ItemNum, char *PropertyId, char *PropertyValue, char *IncreaseNum, 
														 char *IncreaseType, char *IncreaseValue, char *IncreaseEndValue, char *StarsLevel, char *AssemblyID, char *SlotNum, char *EquipSlotID1, char *EquipSlotID2, char *EquipSlotID3, char *QualityEquip, char *IsBind/*, char *ItemName*/)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	vector <St_SendItemInfo>::iterator pSendItemInfo = NULL;
	string strResult = "";
	CString szBack = "";
	
	try
	{
		if(ItemID == NULL || ItemNum == NULL)
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "���ܷ��Ϳյ���", "XDJG�ʼ����͵���");
			return DBTFLV;
		}

		if (!strcmp(mailTitle,""))
		{
			strcpy(mailTitle,"ϵͳ�ʼ�");
		}

		if(!strcmp(mailContent,""))
		{
			strcpy(mailContent,"ϵͳ�ʼ�");
		}

		//��������������Ϣ����������
		AnalysisItemInfo(ItemID,ItemNum,PropertyId,PropertyValue,IncreaseNum,IncreaseType,IncreaseValue,IncreaseEndValue,StarsLevel,AssemblyID,SlotNum,EquipSlotID1,EquipSlotID2,EquipSlotID3,QualityEquip,IsBind/*,ItemName*/);

		if(m_VecSendItemInfo.size() == 0)
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "û�е�����Ϣ", "XDJG�ʼ����͵���");
			return DBTFLV;
		}

		for(pSendItemInfo = m_VecSendItemInfo.begin(); pSendItemInfo != m_VecSendItemInfo.end(); pSendItemInfo++)
		{
			//��ӵ���
			if(SendSingleItem(ServerIP, Character, SendCharacter, mailTitle, mailContent, Money, pSendItemInfo))
			{
				szBack.Format("�ʼ����ͳɹ�(����id:%d, ��������:%d)\n", pSendItemInfo->iItemID, pSendItemInfo->iItemNum);
				strResult += szBack.GetBuffer(0);

				//���õ�������(�ýӿ�һ��ֻ���޸�һ������)
//				SetItemAttribute(ServerIP, Character, pSendItemInfo);
			}
			else
			{
				szBack.Format("�ʼ�����ʧ��(����id:%d, ��������:%d)\n", pSendItemInfo->iItemID, pSendItemInfo->iItemNum);
				strResult += szBack.GetBuffer(0);
			}
		}

		DBTFLV = m_operXDJG.ReturnOpMsg(operName, (char *)strResult.c_str(), "XDJG�ʼ����͵���");

	}
	catch (...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "XDJG�ʼ����͵���");	
	}
	
	m_operXDJG.GmOperLog(userByID, ServerIP, "XDJG�ʼ����͵���", (char *)strResult.c_str(), DBTFLV[0]);
	return DBTFLV;
}

//��ӵ���
BOOL CXDJGInfo::SendSingleItem(char *ServerIP, char *Character, char *SendCharacter, char *mailTitle, char *mailContent, int Money, vector<St_SendItemInfo>::iterator pSendItemInfo)
{
	string strResult;
	char strConn[512];
	ZeroMemory(strConn, 512);
	char retMessage[255];
	memset(retMessage, 0, 255);
	
	try
	{
		if(pSendItemInfo == NULL)
			return FALSE;
		
		sprintf(retMessage, "��ӵ���:<����>%s, <��ɫ��>%s, <����ID>%d, <������Ŀ>%d, <�ʼ�����>%s, <�ʼ�����>%s, <��Ǯ��>%d", ServerIP, Character, pSendItemInfo->iItemID, pSendItemInfo->iItemNum, mailTitle, mailContent, Money);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=send_mail(%s,%s,%s,%s,%d,%d,%d,%s)", 
			ServerIP, HTTPPORT,
			Character,SendCharacter,mailTitle,mailContent,Money,pSendItemInfo->iItemID, pSendItemInfo->iItemNum, pSendItemInfo->strAttrValue.c_str());
		
		if(m_operXDJG.getWebInfo(strConn, strResult))
		{
			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
			{
				return FALSE;				
			}
			else
			{
				int iResult = atoi(strResult.c_str());
				if (iResult>=0)
				{
					
					bool bFinishFlag = false;
					int Maxnum_wait = 10, wait_num=0;
					
					while (!bFinishFlag)
					{
						sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=get_operation_result(%d)", 
							ServerIP, HTTPPORT,
							iResult);
						m_operXDJG.getWebInfo(strConn, strResult);
						Sleep(100);
						int iResultFlag = atoi(strResult.c_str());
						switch (iResultFlag)
						{
						case 1:
							return TRUE;
						case -2:
							wait_num++;
							if (wait_num > Maxnum_wait)
							{
								return FALSE;
							}
							Sleep(1000);
						default:
							return FALSE;
						}
					}
				}
				else
				{
					return FALSE;
				}
			}
		}
		else
		{
			return FALSE;
		}
	}
	catch (...)
	{
		return FALSE;
	}
	
	return TRUE;

// 	string strResult;
// 	char strConn[512];
// 	ZeroMemory(strConn, 512);
// 	char retMessage[255];
// 	memset(retMessage, 0, 255);
// 
// 	try
// 	{
// 		if(pSendItemInfo == NULL)
// 			return FALSE;
// 
// 		sprintf(retMessage, "�������:<����>%s, <��ɫ��>%s, <����ID>%d, <������Ŀ>%d", ServerIP, Character, pSendItemInfo->iItemID, pSendItemInfo->iItemNum);
// 		logFile.WriteLog("<�������>",operName,"�������");//��LOG
// 		
// 		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=add_item(%s,%d,%d)", 
// 			ServerIP, HTTPPORT,
// 			Character,pSendItemInfo->iItemID, pSendItemInfo->iItemNum);
// 		
// 		if(m_operXDJG.getWebInfo(strConn, strResult))
// 		{
// 			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
// 			{
// 				return FALSE;				
// 			}
// 			else
// 			{
// 				int iResult = atoi(strResult.c_str());
// 				if (iResult>=0)
// 				{
// 					
// 					bool bFinishFlag = false;
// 					int Maxnum_wait = 10, wait_num=0;
// 					
// 					while (!bFinishFlag)
// 					{
// 						sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=get_operation_result(%d)", 
// 							ServerIP, HTTPPORT,
// 							iResult);
// 						m_operXDJG.getWebInfo(strConn, strResult);
// 						int iResultFlag = atoi(strResult.c_str());
// 						switch (iResultFlag)
// 						{
// 						case 1:
// 							return TRUE;
// 						case -2:
// 							wait_num++;
// 							if (wait_num > Maxnum_wait)
// 							{
// 								return FALSE;
// 							}
// 							Sleep(1000);
// 						default:
// 							return FALSE;
// 						}
// 					}
// 				}
// 				else
// 				{
// 					return FALSE;
// 				}
// 				
// 			}
// 			
// 		}
// 		else
// 		{
// 			return FALSE;
// 		}
// 	}
// 	catch (...)
// 	{
// 		return FALSE;
// 	}
// 	
// 	return TRUE;
}

//���õ�������
// BOOL CXDJGInfo::SetItemAttribute(char *ServerIP, char *Character, vector<St_SendItemInfo>::iterator pSendItemInfo)
// {
// 	string strResult;
// 	char strConn[512];
// 	ZeroMemory(strConn, 512);
// 	char retMessage[255];
// 	memset(retMessage, 0, 255);
// 	int iType = 0;
// 	int iParam1 = 0;
// 	int iParam2 = 0;
// 
// 	try
// 	{
// 		if(pSendItemInfo == NULL)
// 			return FALSE;
// 		
// 		sprintf(retMessage, "���õ�������:<����>%s, <��ɫ��>%s, <����ID>%d, <������Ŀ>%d", ServerIP, Character, pSendItemInfo->iItemID, pSendItemInfo->iItemNum);
// 		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
// 		
// 		//�Ƿ��
// // 		if(pSendItemInfo->iIsBind == 0)
// // 			SetItemAttr(ServerIP,Character,9,0,0);
// // 		else if(pSendItemInfo->iIsBind == 1)
// // 			SetItemAttr(ServerIP,Character,9,1,0);
// // 
// // 		//�����
// // 		if(pSendItemInfo->iSlotNum != 0)
// // 			SetItemAttr(ServerIP,Character,8,pSendItemInfo->iSlotNum,0);
// // 
// // 		//����
// // 		if(pSendItemInfo->iStarsLevel != 0)
// // 			SetItemAttr(ServerIP,Character,3,pSendItemInfo->iStarsLevel,0);
// // 
// // 		//��������
// // 		if(pSendItemInfo->iIncreaseNum != 0)
// // 			SetItemAttr(ServerIP,Character,2,pSendItemInfo->iIncreaseNum,0);
// // 
// // 		//װ������
// // 		if(pSendItemInfo->iQualityEquip == 1)
// // 			SetItemAttr(ServerIP,Character,4,1,0);
// // 		else if(pSendItemInfo->iQualityEquip == 2)
// // 			SetItemAttr(ServerIP,Character,4,2,0);
// // 
// // 		//��������
// // 		if(pSendItemInfo->iIncreaseType != 0)
// // 			SetItemAttr(ServerIP,Character,1,pSendItemInfo->iIncreaseType,pSendItemInfo->iIncreaseValue);
// 
// 
// 	}
// 	catch (...)
// 	{
// 		return FALSE;
// 	}
// 	
// 	return TRUE;
// }

//��������
// BOOL CXDJGInfo::SetItemAttr(char *ServerIP,char *Character,int iType,int iParam1,int iParam2)
// {
// 	string strResult;
// 	char strConn[512];
// 	ZeroMemory(strConn, 512);
// 	char retMessage[255];
// 	memset(retMessage, 0, 255);
// 	
// 	try
// 	{
// 		logFile.WriteLog("<�������>",operName,"���õ�������");//��LOG
// 		
// 		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=set_item_attr(%s,%d,%d,%d,%d)", 
// 			ServerIP, HTTPPORT,
// 			Character,iType, iParam1, iParam2, 0);
// 		
// 		if(m_operXDJG.getWebInfo(strConn, strResult))
// 		{
// 			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
// 			{
// 				return FALSE;				
// 			}
// 			else
// 			{
// 				int iResult = atoi(strResult.c_str());
// 				if (iResult>=0)
// 				{
// 					
// 					bool bFinishFlag = false;
// 					int Maxnum_wait = 10, wait_num=0;
// 					
// 					while (!bFinishFlag)
// 					{
// 						sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=get_operation_result(%d)", 
// 							ServerIP, HTTPPORT,
// 							iResult);
// 						m_operXDJG.getWebInfo(strConn, strResult);
// 						int iResultFlag = atoi(strResult.c_str());
// 						switch (iResultFlag)
// 						{
// 						case 1:
// 							return TRUE;
// 						case -2:
// 							wait_num++;
// 							if (wait_num > Maxnum_wait)
// 							{
// 								return FALSE;
// 							}
// 							Sleep(1000);
// 						default:
// 							return FALSE;
// 						}
// 					}
// 				}
// 				else
// 				{
// 					return FALSE;
// 				}
// 			}
// 		}
// 		else
// 		{
// 			return FALSE;
// 		}
// 	}
// 	catch (...)
// 	{
// 		return FALSE;
// 	}
// 
// 	return TRUE;
// }

//��������������Ϣ����������
void CXDJGInfo::AnalysisItemInfo(char *ItemID, char *ItemNum, char *PropertyId, char *PropertyValue, char *IncreaseNum, char *IncreaseType, char *IncreaseValue, char *IncreaseEndValue, char *StarsLevel, char *AssemblyID, char *SlotNum, char *EquipSlotID1, char *EquipSlotID2, char *EquipSlotID3, char *QualityEquip, char *IsBind/*, char *ItemName*/)
{
	St_SendItemInfo sSendItemInfo;
	vector<int> VecItemID;
	vector<int> VecItemNum;
	vector<string> VecPropertyId;
	vector<string> VecPropertyValue;
	vector<string> VecIncreaseNum;
	vector<string> VecIncreaseType;
	vector<string> VecIncreaseValue;
	vector<string> VecIncreaseEndValue;
	vector<string> VecStarsLevel;
	vector<string> VecAssemblyID;
	vector<string> VecSlotNum;
	vector<string> VecEquipSlotID1;
	vector<string> VecEquipSlotID2;
	vector<string> VecEquipSlotID3;
	vector<string> VecQualityEquip;
	vector<string> VecIsBind;
	vector<string> VecItemName;
	CString szValue = "";
	int iCount = 0;

	try
	{
		if(!m_VecSendItemInfo.empty())
			m_VecSendItemInfo.clear();

		//����id
		char *pitemID = strtok(ItemID,";");
		while(pitemID != NULL)
		{
			VecItemID.push_back(atoi(pitemID));
			
			pitemID = strtok(NULL,";");
		}
		
		//��������
		char *pitemNum = strtok(ItemNum,";");
		while(pitemNum != NULL)
		{
			VecItemNum.push_back(atoi(pitemNum));
			
			pitemNum = strtok(NULL,";");
		}

		//��������id
		char *pPropertyId = strtok(PropertyId,";");
		while(pPropertyId != NULL)
		{
			//ת����16�����ַ���
			VecPropertyId.push_back(ConvertTo16(atoi(pPropertyId)));
			
//			VecPropertyId.push_back(atoi(pPropertyId));
			pPropertyId = strtok(NULL,";");
		}

		//��������ֵ
		char *pPropertyValue = strtok(PropertyValue,";");
		while(pPropertyValue != NULL)
		{
			//ת����16�����ַ���
			VecPropertyValue.push_back(ConvertTo16(atoi(pPropertyValue)));

//			VecPropertyValue.push_back(atoi(pPropertyValue));
			pPropertyValue = strtok(NULL,";");
		}

		//�����ȼ�
		char *pIncreaseNum = strtok(IncreaseNum,";");
		while(pIncreaseNum != NULL)
		{
			//ת����16�����ַ���
			VecIncreaseNum.push_back(ConvertTo16(atoi(pIncreaseNum)));
			
//			VecIncreaseNum.push_back(atoi(pIncreaseNum));
			pIncreaseNum = strtok(NULL,";");
		}

		//��������
		char *pIncreaseType = strtok(IncreaseType,";");
		while(pIncreaseType != NULL)
		{
			//ת����16�����ַ���
			VecIncreaseType.push_back(ConvertTo16(atoi(pIncreaseType)));

//			VecIncreaseType.push_back(atoi(pIncreaseType));
			
			pIncreaseType = strtok(NULL,";");
		}

		//����ֵ
		char *pIncreaseValue = strtok(IncreaseValue,";");
		while(pIncreaseValue != NULL)
		{
			//ת����16�����ַ���
			VecIncreaseValue.push_back(ConvertTo16(atoi(pIncreaseValue)));

//			VecIncreaseValue.push_back(atoi(pIncreaseValue));
			pIncreaseValue = strtok(NULL,";");
		}

		//�ϻ�����ֵ
		char *pIncreaseEndValue = strtok(IncreaseEndValue,";");
		while(pIncreaseEndValue != NULL)
		{
			//ת����16�����ַ���
			VecIncreaseEndValue.push_back(ConvertTo16(atoi(pIncreaseEndValue)));
			
//			VecIncreaseEndValue.push_back(atoi(pIncreaseEndValue));
			pIncreaseEndValue = strtok(NULL,";");
		}

		//����
		char *pStarsLevel = strtok(StarsLevel,";");
		while(pStarsLevel != NULL)
		{
			//ת����16�����ַ���
			VecStarsLevel.push_back(ConvertTo16(atoi(pStarsLevel)));
			
//			VecStarsLevel.push_back(atoi(pStarsLevel));
			pStarsLevel = strtok(NULL,";");
		}

		//��װid
		char *pAssemblyID = strtok(AssemblyID,";");
		while(pAssemblyID != NULL)
		{
			//ת����16�����ַ���
			VecAssemblyID.push_back(ConvertTo16(atoi(pAssemblyID)));
			
//			VecAssemblyID.push_back(atoi(pAssemblyID));
			pAssemblyID = strtok(NULL,";");
		}

		//�������
		char *pSlotNum = strtok(SlotNum,";");
		while(pSlotNum != NULL)
		{
			//ת����16�����ַ���
			VecSlotNum.push_back(ConvertTo16(atoi(pSlotNum)));
			
//			VecSlotNum.push_back(atoi(pSlotNum));
			pSlotNum = strtok(NULL,";");
		}

		//���1
		char *pEquipSlotID1 = strtok(EquipSlotID1,";");
		while(pEquipSlotID1 != NULL)
		{
			//ת����16�����ַ���
			VecEquipSlotID1.push_back(ConvertTo16(atoi(pEquipSlotID1)));
			
//			VecEquipSlotID1.push_back(atoi(pEquipSlotID1));
			pEquipSlotID1 = strtok(NULL,";");
		}

		//���2
		char *pEquipSlotID2 = strtok(EquipSlotID2,";");
		while(pEquipSlotID2 != NULL)
		{
			//ת����16�����ַ���
			VecEquipSlotID2.push_back(ConvertTo16(atoi(pEquipSlotID2)));
			
//			VecEquipSlotID2.push_back(atoi(pEquipSlotID2));
			pEquipSlotID2 = strtok(NULL,";");
		}

		//���3
		char *pEquipSlotID3 = strtok(EquipSlotID3,";");
		while(pEquipSlotID3 != NULL)
		{
			//ת����16�����ַ���
			VecEquipSlotID3.push_back(ConvertTo16(atoi(pEquipSlotID3)));
			
//			VecEquipSlotID3.push_back(atoi(pEquipSlotID3));
			pEquipSlotID3 = strtok(NULL,";");
		}

		//����
		char *pQualityEquip = strtok(QualityEquip,";");
		while(pQualityEquip != NULL)
		{
			//ת����16�����ַ���
			VecQualityEquip.push_back(ConvertTo16(atoi(pQualityEquip)));
			
//			VecQualityEquip.push_back(atoi(pQualityEquip));
			pQualityEquip = strtok(NULL,";");
		}

		//��
		char *pIsBind = strtok(IsBind,";");
		while(pIsBind != NULL)
		{
			//ת����16�����ַ���
			VecIsBind.push_back(ConvertTo16(atoi(pIsBind)));

//			VecIsBind.push_back(atoi(pIsBind));
			
			pIsBind = strtok(NULL,";");
		}

		//��������
// 		char *pItemName = strtok(ItemName,";");
// 		while(pItemName != NULL)
// 		{
// 			VecItemName.push_back(atoi(pItemName));
// 			
// 			pItemName = strtok(NULL,";");
// 		}

		//����ṹ��
		for(iCount = 0; iCount < VecItemID.size(); iCount++)
		{
			sSendItemInfo.iItemID = VecItemID[iCount];
			sSendItemInfo.iItemNum = VecItemNum[iCount];
//			sSendItemInfo.strItemName = VecItemName[iCount];

			szValue.Format("%s%s%s%s%s%s%s%s%s%s%s%s%s%s",VecPropertyId[iCount].c_str(),VecPropertyValue[iCount].c_str(),VecIncreaseNum[iCount].c_str(), 
							VecIncreaseType[iCount].c_str(),VecIncreaseValue[iCount].c_str(),VecIncreaseEndValue[iCount].c_str(),
							VecStarsLevel[iCount].c_str(),VecAssemblyID[iCount].c_str(),VecSlotNum[iCount].c_str(),VecEquipSlotID1[iCount].c_str(),
							VecEquipSlotID2[iCount].c_str(),VecEquipSlotID3[iCount].c_str(),VecQualityEquip[iCount].c_str(),VecIsBind[iCount].c_str());

			sSendItemInfo.strAttrValue = szValue.GetBuffer(0);

			szValue = "";

// 			sSendItemInfo.iPropertyId = VecPropertyId[iCount];
// 			sSendItemInfo.iPropertyValue = VecPropertyValue[iCount];
// 			sSendItemInfo.iIncreaseNum = VecIncreaseNum[iCount];
// 
// 			sSendItemInfo.iIncreaseType = VecIncreaseType[iCount];
// 			sSendItemInfo.iIncreaseValue = VecIncreaseValue[iCount];
// 			sSendItemInfo.iIncreaseEndValue = VecIncreaseEndValue[iCount];
// 			sSendItemInfo.iStarsLevel = VecStarsLevel[iCount];
// 			sSendItemInfo.iAssemblyID = VecAssemblyID[iCount];
// 			sSendItemInfo.iSlotNum = VecSlotNum[iCount];
// 			sSendItemInfo.iEquipSlotID1 = VecEquipSlotID1[iCount];
// 			sSendItemInfo.iEquipSlotID2 = VecEquipSlotID2[iCount];
// 			sSendItemInfo.iEquipSlotID3 = VecEquipSlotID3[iCount];
// 			sSendItemInfo.iQualityEquip = VecQualityEquip[iCount];
// 			sSendItemInfo.iIsBind = VecIsBind[iCount];

			m_VecSendItemInfo.push_back(sSendItemInfo);
		}
	}
	catch (CMemoryException* e)
	{
		return;
	}
}

//ת����16�����ַ���
string CXDJGInfo::ConvertTo16(int iValue)
{
	CString strValue = "";
		
	try
	{
		if(iValue < 0 || iValue == -1 || iValue >= 10000)
			return "ffff";

		switch(iValue/16)
		{
		case 0:
			switch(iValue%16)
			{
			case 0:
				strValue.Format("0010");
				break;
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
				strValue.Format("000%d",iValue%16);
				break;
			case 10:
				strValue.Format("000a");
				break;
			case 11:
				strValue.Format("000b");
				break;
			case 12:
				strValue.Format("000c");
				break;
			case 13:
				strValue.Format("000d");
				break;
			case 14:
				strValue.Format("000e");
				break;
			case 15:
				strValue.Format("000f");
				break;
			default:
				strValue.Format("0000");
				break;
			}
			break;
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
			switch(iValue%16)
			{
			case 0:
				strValue.Format("0020");
				break;
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
				strValue.Format("00%d%d",iValue/16,iValue%16);
				break;
			case 10:
				strValue.Format("00%da",iValue/16);
				break;
			case 11:
				strValue.Format("00%db",iValue/16);
				break;
			case 12:
				strValue.Format("00%dc",iValue/16);
				break;
			case 13:
				strValue.Format("00%dd",iValue/16);
				break;
			case 14:
				strValue.Format("00%de",iValue/16);
				break;
			case 15:
				strValue.Format("00%df",iValue/16);
				break;
			}
			break;
		case 10:
			strValue.Format("00a%d",iValue%16);
			break;
		case 11:
			strValue.Format("00b%d",iValue%16);
			break;
		case 12:
			strValue.Format("00c%d",iValue%16);
			break;
		case 13:
			strValue.Format("00d%d",iValue%16);
			break;
		case 14:
			strValue.Format("00e%d",iValue%16);
			break;
		case 15:
			strValue.Format("00f%d",iValue%16);
			break;
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		case 27:
		case 28:
		case 29:
		case 30:
		case 31:
			switch(iValue%16)
			{
			case 0:
				strValue.Format("01%d0",iValue/16-16);
				break;
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
				strValue.Format("01%d%d",iValue/16-16,iValue%16);
				break;
			case 10:
				strValue.Format("01%da",iValue/16-16);
				break;
			case 11:
				strValue.Format("01%db",iValue/16-16);
				break;
			case 12:
				strValue.Format("01%dc",iValue/16-16);
				break;
			case 13:
				strValue.Format("01%dd",iValue/16-16);
				break;
			case 14:
				strValue.Format("01%de",iValue/16-16);
				break;
			case 15:
				strValue.Format("01%df",iValue/16-16);
				break;
			}
			break;
		case 32:
		case 33:
		case 34:
		case 35:
		case 36:
		case 37:
		case 38:
		case 39:
		case 40:
		case 41:
		case 42:
		case 43:
		case 44:
		case 45:
		case 46:
		case 47:
		case 48:
		case 49:
		case 50:
		case 51:
		case 52:
		case 53:
		case 54:
		case 55:
		case 56:
		case 57:
		case 58:
		case 59:
		case 60:
		case 61:
		case 62:
		case 63:
			switch(iValue%16)
			{
			case 0:
				strValue.Format("02%d0",iValue/16-16);
				break;
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
				strValue.Format("02%d%d",iValue/16-16,iValue%16);
				break;
			case 10:
				strValue.Format("02%da",iValue/16-16);
				break;
			case 11:
				strValue.Format("02%db",iValue/16-16);
				break;
			case 12:
				strValue.Format("02%dc",iValue/16-16);
				break;
			case 13:
				strValue.Format("02%dd",iValue/16-16);
				break;
			case 14:
				strValue.Format("02%de",iValue/16-16);
				break;
			case 15:
				strValue.Format("02%df",iValue/16-16);
				break;
			}
		break;
		}

	}
	catch (...)
	{
		return "";
	}

	return strValue;
}

// vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_Net_ItemAdd(int userByID, char *ServerIP, char *Character, char *ItemID, char *ItemNum)
// {
// 	vector <CGlobalStruct::TFLV> DBTFLV;
// 	
// 	string strResult;
// 	vector <string> vectorColInfo;
// 	char strConn[512];
// 	ZeroMemory(strConn, 512);
// 	char retMessage[255];
// 	memset(retMessage, 0, 255);
// 	vector<string> strItemId;
// 	vector<string> strItemNum;
// 
// 	try
// 	{
// 
// 		
// 		sprintf(retMessage, "�������:<����>%s, <��ɫ��>%s, <����ID>%d, <������Ŀ>%d", ServerIP, Character, ItemID, ItemNum);
// 		logFile.WriteLog("<�������>",operName,"�������");//��LOG
// 		
// 		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=add_item(%s,%d,%d)", 
// 			ServerIP, HTTPPORT,
// 			Character,ItemID,ItemNum);
// 		
// 		if(m_operXDJG.getWebInfo(strConn, strResult))
// 		{
// 			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
// 			{
// 				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "XDJG�������");				
// 			}
// 			else
// 			{
// 				int iResult = atoi(strResult.c_str());
// 				if (iResult>=0)
// 				{
// 					
// 					bool bFinishFlag = false;
// 					int Maxnum_wait = 10, wait_num=0;
// 					
// 					while (!bFinishFlag)
// 					{
// 						sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=get_operation_result(%d)", 
// 							ServerIP, HTTPPORT,
// 							iResult);
// 						m_operXDJG.getWebInfo(strConn, strResult);
// 						int iResultFlag = atoi(strResult.c_str());
// 						switch (iResultFlag)
// 						{
// 						case 1:
// 							bFinishFlag = true;
// 							
// 							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ӵ��߳ɹ�", "XDJG�������");
// 							
// 							break;
// 						case -2:
// 							wait_num++;
// 							if (wait_num > Maxnum_wait)
// 							{
// 								bFinishFlag = true; 
// 								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ӵ��߳�ʱ", "XDJG�������");
// 								
// 							}
// 							Sleep(1000);
// 							break;
// 						default:
// 							bFinishFlag = true;
// 							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ӵ���ʧ��", "XDJG�������");
// 						}
// 					}
// 				}
// 				else
// 				{
// 					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "XDJG�������");
// 				}
// 				
// 			}
// 			
// 		}
// 		else
// 		{
// 			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "XDJG�������");		
// 		}
// 	}
// 	catch (...)
// 	{
// 		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "XDJG�������");	
// 	}
// 	
// 	m_operXDJG.GmOperLog(userByID, ServerIP, "XDJG�������", retMessage, DBTFLV[0]);
// 	return DBTFLV;
// }

//����ɾ��
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_Net_ItemDel(int userByID, char *ServerIP, char *Character, int ItemID, int ItemNum)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	
	string strResult;
	vector <string> vectorColInfo;
	char strConn[512];
	ZeroMemory(strConn, 512);
	char retMessage[255];
	memset(retMessage, 0, 255);
	
	try
	{
		sprintf(retMessage, "�������:<����>%s, <��ɫ��>%s, <����ID>%d, <������Ŀ>%d", ServerIP, Character, ItemID, ItemNum);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=delete_item(%s,%d,%d)", 
			ServerIP, HTTPPORT,
			Character,ItemID,ItemNum);
		
		if(m_operXDJG.getWebInfo(strConn, strResult))
		{
			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "XDJG����ɾ��");				
			}
			else
			{
				int iResult = atoi(strResult.c_str());
				if (iResult>=0)
				{
					
					bool bFinishFlag = false;
					int Maxnum_wait = 10, wait_num=0;
					
					while (!bFinishFlag)
					{
						sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=get_operation_result(%d)", 
							ServerIP, HTTPPORT,
							iResult);
						m_operXDJG.getWebInfo(strConn, strResult);
						int iResultFlag = atoi(strResult.c_str());
						switch (iResultFlag)
						{
						case 1:
							bFinishFlag = true;
							
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "ɾ�����߳ɹ�", "XDJG����ɾ��");
							
							break;
						case -2:
							wait_num++;
							if (wait_num > Maxnum_wait)
							{
								bFinishFlag = true; 
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "ɾ�����߳�ʱ", "XDJG����ɾ��");
								
							}
							Sleep(1000);
							break;
						default:
							bFinishFlag = true;
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "ɾ������ʧ��", "XDJG����ɾ��");
						}
					}
				}
				else
				{
					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "XDJG����ɾ��");
				}
				
			}
			
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "XDJG����ɾ��");		
		}
	}
	catch (...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "XDJG����ɾ��");	
	}
	
	m_operXDJG.GmOperLog(userByID, ServerIP, "XDJG����ɾ��", retMessage, DBTFLV[0]);
	return DBTFLV;
}

//��Ϸ�Ҳ�ѯ
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_Net_MoneyQuery(char *ServerIP, char *Account)
{
	CGlobalStruct::TFLV m_tflv;
	vector <CGlobalStruct::TFLV> DBTFLV;
	
	string strResult;
	vector <string> vectorColInfo;
	char strConn[512];
	ZeroMemory(strConn, 512);
	char retMessage[512];
	ZeroMemory(retMessage, 512);
	
	try
	{
		sprintf(retMessage, "��Ϸ�Ҳ�ѯ:<����>%s, <�ʺ�>%s",ServerIP, Account);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=query_gold(%s)", 
			ServerIP, HTTPPORT,
			Account);
		
		if(m_operXDJG.getWebInfo(strConn, strResult))
		{
			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "XDJG��Ϸ�Ҳ�ѯ");				
			}
			else
			{
				int iResult = atoi(strResult.c_str());
				if (iResult>=0)
				{
					
					bool bFinishFlag = false;
					int Maxnum_wait = 10, wait_num=0;
					char *strR = NULL;
					CString szResult = "";
					
					while (!bFinishFlag)
					{
						sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=get_operation_result(%d)", 
							ServerIP, HTTPPORT,
							iResult);
						m_operXDJG.getWebInfo(strConn, strResult);

						Sleep(200);

						szResult = strResult.c_str(); 
						strR = strtok(szResult.GetBuffer(0),",");
						if(strR == NULL)
						{
							int iResultFlag = atoi(strResult.c_str());
							switch (iResultFlag)
							{
							case -2:
								wait_num++;
								if (wait_num > Maxnum_wait)
								{
									bFinishFlag = true; 
									DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��Ϸ�Ҳ�ѯ��ʱ", "XDJG��Ϸ�Ҳ�ѯ");
								}
								Sleep(1000);
								break;
							default:
								bFinishFlag = true;
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��Ϸ�Ҳ�ѯʧ��", "XDJG��Ϸ�Ҳ�ѯ");
							}
						}
						else
						{
							if(atoi(strR) == 0)
							{
								strR = strtok(NULL,",");
								m_tflv.nIndex = DBTFLV.size()+1;
								m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
								m_tflv.m_tagName = CEnumCore::TagName::XDJG_Money;
								m_tflv.m_tvlength = strlen(strR) + 1;
								sprintf((char *)&m_tflv.lpdata,"%s",strR);
								DBTFLV.push_back(m_tflv);
								break;
							}
							else
							{
								bFinishFlag = true;
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��Ϸ�Ҳ�ѯʧ��", "XDJG��Ϸ�Ҳ�ѯ");
							}
						}
					}
				}
				else
				{
					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "XDJG��Ϸ�Ҳ�ѯ");
				}
			}
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "XDJG��Ϸ�Ҳ�ѯ");		
		}
	}
	catch (...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "XDJG��Ϸ�Ҳ�ѯ");	
	}
	
	return DBTFLV;
}
//�����Ϸ��
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_Net_MoneyAdd(int userByID, char *ServerIP, char *Account, int iMoney)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	
	string strResult;
	vector <string> vectorColInfo;
	char strConn[512];
	ZeroMemory(strConn, 512);
	char retMessage[255];
	memset(retMessage, 0, 255);
	
	try
	{
		sprintf(retMessage, "�����Ϸ��:<����>%s, <�ʺ�>%s, <��Ϸ����>%d", ServerIP, Account, iMoney);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=add_gold(%s,%d)", 
			ServerIP, HTTPPORT,
			Account,iMoney);
		
		if(m_operXDJG.getWebInfo(strConn, strResult))
		{
			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "XDJG��Ϸ�����");				
			}
			else
			{
				int iResult = atoi(strResult.c_str());
				if (iResult>=0)
				{
					
					bool bFinishFlag = false;
					int Maxnum_wait = 10, wait_num=0;
					
					while (!bFinishFlag)
					{
						sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=get_operation_result(%d)", 
							ServerIP, HTTPPORT,
							iResult);
						m_operXDJG.getWebInfo(strConn, strResult);
						int iResultFlag = atoi(strResult.c_str());
						switch (iResultFlag)
						{
						case 0:
							bFinishFlag = true;
							
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�����Ϸ�ҳɹ�", "XDJG��Ϸ�����");
							
							break;
						case -2:
							wait_num++;
							if (wait_num > Maxnum_wait)
							{
								bFinishFlag = true; 
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�����Ϸ�ҳ�ʱ", "XDJG��Ϸ�����");
								
							}
							Sleep(1000);
							break;
						default:
							bFinishFlag = true;
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�����Ϸ��ʧ��", "XDJG��Ϸ�����");
						}
					}
				}
				else
				{
					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "XDJG��Ϸ�����");
				}
				
			}
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "XDJG��Ϸ�����");		
		}
	}
	catch (...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "XDJG��Ϸ�����");	
	}
	
	m_operXDJG.GmOperLog(userByID, ServerIP, "XDJG��Ϸ�����", retMessage, DBTFLV[0]);
	return DBTFLV;
}

vector <string> CXDJGInfo::GetLogDbName(char *BeginTime, char *EndTime)
{
	vector <string> strLogDb;
	char *pBegin = NULL;
	char *pEnd = NULL;
	long iData = 0;
	char cTag[] = "- ";
	string strDbName = "";
	string strBeData = "";
	string strEnData = "";
	CString szData = "";

	try
	{
		if(BeginTime == NULL || EndTime == NULL)
			return strLogDb;

		//ȡ��ʼʱ���������
		pBegin = strtok(BeginTime, cTag);
		while(pBegin != NULL)
		{
			if(strlen(pBegin) > 4)
				break;
			strBeData += pBegin;
			pBegin = strtok(NULL, cTag);
		}

		//ȡ����ʱ���������
		pEnd = strtok(EndTime, cTag);
		while(pEnd != NULL)
		{
			if(strlen(pEnd) > 4)
				break;
			strEnData += pEnd;
			pEnd = strtok(NULL, cTag);
		}

		if(atol(strBeData.c_str()) >= atol(strEnData.c_str()))
			return strLogDb;

		for(iData = atol(strBeData.c_str()); iData <= atol(strEnData.c_str()); iData++)
		{
			strDbName = "OMPLog_";
			szData.Format("%ld", iData);
			strDbName += szData.GetBuffer(0);
			strLogDb.push_back(strDbName);
		}
	}
	catch (...)
	{
		return strLogDb;
	}

	return strLogDb;
}

//��־��:��ҵ�¼��־
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_DB_Login_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, char *QueryIP, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	CString strTemSql = "";
	char realServerIP[40];
	vector<string> LogDbName;
	vector<string>::iterator pLogDbName = NULL;
	char retMessage[255];
	ZeroMemory(retMessage,255);
	
	try
	{
		sprintf(retMessage, "��ҵ�¼��־:<����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character,BeginTime, EndTime);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "��ҵ�¼��־");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		//��ȡLogDB������
// 		LogDbName = GetLogDbName(BeginTime, EndTime);
// 		if(LogDbName.size() == 0)
// 			return DBTFLV;

		if(!strcmp(QueryIP,""))
		{
			strSql.Format("SELECT * FROM (SELECT account_name AS XDJG_Account,'%s' AS XDJG_Character,'��¼' AS XDJG_LogType,create_time AS XDJG_LogTime,ipaddr AS XDJG_LogIP \
											FROM %s.ACCOUNT_LOGIN \
										   WHERE account_name = '%s' and create_time between '%s' and '%s' \
										   UNION \
										  SELECT b.account_name AS XDJG_Account,'%s' AS XDJG_Character,'�ǳ�' AS XDJG_LogType,b.create_time AS XDJG_LogTime,c.ipaddr AS XDJG_LogIP \
											FROM %s.ACCOUNT_LOGOUT b, %s.ACCOUNT_LOGIN c \
										   WHERE b.account_name = '%s' and b.account_name = c.account_name \
											 and b.create_time between '%s' and '%s') a \
							ORDER BY a.XDJG_LogTime",Character,LogDB,Account,BeginTime,EndTime,Character,LogDB,LogDB,Account,BeginTime,EndTime);
		}
		else
		{
			strSql.Format("SELECT * FROM (SELECT account_name AS XDJG_Account,'��¼' AS XDJG_LogType,create_time AS XDJG_LogTime,'%s' AS XDJG_LogIP \
										    FROM %s.ACCOUNT_LOGIN \
										   WHERE ipaddr = '%s' and create_time between '%s' and '%s' \
										   UNION \
										  SELECT b.account_name AS XDJG_Account,'�ǳ�' AS XDJG_LogType,b.create_time AS XDJG_LogTime,'%s' AS XDJG_LogIP \
										    FROM %s.ACCOUNT_LOGOUT b, %s.ACCOUNT_LOGIN c \
										   WHERE c.ipaddr = '%s' and b.account_name = c.account_name \
										     and b.create_time between '%s' and '%s') a \
										   ORDER BY a.XDJG_Account,a.XDJG_LogTime",QueryIP,LogDB,QueryIP,BeginTime,EndTime,QueryIP,LogDB,LogDB,QueryIP,BeginTime,EndTime);
		}

#ifdef _DEBUG
		printf(strSql);
#endif

		DBTFLV = GameDBQuery("XDJG",realServerIP,13, iIndex, iPageSize, strSql.GetBuffer(0));
		
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "��ҵ�¼��־��ѯ");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	return DBTFLV;
}

//�̳���Ʒ������־--������
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_DB_Shoping_Log(int userByID, char *ServerIP, char *Account, char *Character, int QueryType, char *BeginTime, char *EndTime, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	vector<CGlobalStruct::TFLV>::iterator pDBTFLV = NULL;
	CString strSql = "";
	CString strItemID = "";
	CString strTemSql = "";
	char realServerIP[40];
	vector<st_ItemInfo>::iterator pItemInfo = NULL;
	vector<string> LogDbName;
	vector<string>::iterator pLogDbName = NULL;
	char retMessage[255];
	ZeroMemory(retMessage,255);
	Vec_ItemInfo VecItemInfo;
	
	try
	{
		sprintf(retMessage, "�̳���Ʒ������־��ѯ:<����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character,BeginTime, EndTime);
		logFile.WriteLog("<�������>",operName,"retMessage");//��LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "�̳���Ʒ������־");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}

		//��ȡLogDB������
// 		LogDbName = GetLogDbName(BeginTime, EndTime);
// 		if(LogDbName.size() == 0)
// 			return DBTFLV;

		if(QueryType == 1)
		{
			strSql.Format("SELECT '%s' AS XDJG_AccountBuyer,'%s' AS XDJG_RoleBuyer,friend_name AS XDJG_RoleReceiver, \
								  item_id AS XDJG_ItemID,item_num AS XDJG_ItemNum,type AS XDJG_ItemType, \
								  gold AS XDJG_Money, create_time AS XDJG_LogTime \
							 FROM %s.SHOPPING \
							WHERE create_time BETWEEN '%s' AND '%s' and character_name = '%s'", \
								  Account,Character,LogDB,BeginTime,EndTime, Character);
		}
		else if(QueryType == 2)
		{
			strSql.Format("SELECT '%s' AS XDJG_AccountBuyer,'%s' AS XDJG_RoleBuyer,character_name AS XDJG_RoleReceiver, \
								  item_id AS XDJG_ItemID,item_num AS XDJG_ItemNum,type AS XDJG_ItemType, \
								  gold AS XDJG_Money, create_time AS XDJG_LogTime \
							 FROM %s.SHOPPING \
							WHERE create_time BETWEEN '%s' AND '%s' and friend_name = '%s'", \
								  Account,Character,LogDB,BeginTime,EndTime, Character);
		}

	
#ifdef _DEBUG
		printf(strSql);
#endif
		
		DBTFLV = GameDBQuery("XDJG",realServerIP,13, iIndex, iPageSize, strSql.GetBuffer(0));
		
		if(DBTFLV.empty())
		{
			return DBTFLV;
		}

		//ȡ��Ʒ����
		GetItemInfo(VecItemInfo);

		for(pDBTFLV = DBTFLV.begin(); pDBTFLV != DBTFLV.end(); pDBTFLV++)
		{
			//������id��ɵ�������
			if(pDBTFLV->m_tagName == CEnumCore::TagName::XDJG_ItemID)
			{
				for(pItemInfo = VecItemInfo.begin(); pItemInfo != VecItemInfo.end(); pItemInfo++)
				{
					if(pItemInfo->iItemId == atoi((char*)pDBTFLV->lpdata))
					{
						pDBTFLV->m_tagName = CEnumCore::TagName::XDJG_ItemName;
						pDBTFLV->m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
						*pDBTFLV->lpdata = NULL;
						pDBTFLV->m_tvlength = pItemInfo->strItemName.size();
						memcpy(&pDBTFLV->lpdata, pItemInfo->strItemName.c_str(), pItemInfo->strItemName.size());
						break;
					}
					else
						continue;
				}
				
				if(pDBTFLV->m_tagName != CEnumCore::TagName::XDJG_ItemName)
				{
					pDBTFLV->m_tagName = CEnumCore::TagName::XDJG_ItemName;
					pDBTFLV->m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("")+1;
					memcpy(&pDBTFLV->lpdata, "", strlen("")+1);
				}
			}
			//���������ͱ������
			else if(pDBTFLV->m_tagName == CEnumCore::TagName::XDJG_ItemType)
			{
				switch (atoi((char*)pDBTFLV->lpdata))
				{
				case 0:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("���ض�����")+1;
					memcpy(&pDBTFLV->lpdata, "���ض�����", strlen("���ض�����")+1);
					break;
				case 1:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("װ��")+1;
					memcpy(&pDBTFLV->lpdata, "װ��", strlen("װ��")+1);
					break;
				case 2:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("����Ʒ")+1;
					memcpy(&pDBTFLV->lpdata, "����Ʒ", strlen("����Ʒ")+1);
					break;
				case 3:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("ǿ������")+1;
					memcpy(&pDBTFLV->lpdata, "ǿ������", strlen("ǿ������")+1);
					break;
				case 4:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("��װ����")+1;
					memcpy(&pDBTFLV->lpdata, "��װ����", strlen("��װ����")+1);
					break;
				case 5:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("�������")+1;
					memcpy(&pDBTFLV->lpdata, "�������", strlen("�������")+1);
					break;
				case 6:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("������䷽")+1;
					memcpy(&pDBTFLV->lpdata, "������䷽", strlen("������䷽")+1);
					break;
				case 7:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("ս��������")+1;
					memcpy(&pDBTFLV->lpdata, "ս��������", strlen("ս��������")+1);
					break;
				case 8:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("����")+1;
					memcpy(&pDBTFLV->lpdata, "����", strlen("����")+1);
					break;
				case 9:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("����")+1;
					memcpy(&pDBTFLV->lpdata, "����", strlen("����")+1);
					break;
				case 10:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("�鼮")+1;
					memcpy(&pDBTFLV->lpdata, "�鼮", strlen("�鼮")+1);
					break;
				case 11:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("����")+1;
					memcpy(&pDBTFLV->lpdata, "����", strlen("����")+1);
					break;
				case 12:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("����װ������")+1;
					memcpy(&pDBTFLV->lpdata, "����װ������", strlen("����װ������")+1);
					break;
				case 13:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("�����������")+1;
					memcpy(&pDBTFLV->lpdata, "�����������", strlen("�����������")+1);
					break;
				case 14:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("����")+1;
					memcpy(&pDBTFLV->lpdata, "����", strlen("����")+1);
					break;
				case 15:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("���ܵ���")+1;
					memcpy(&pDBTFLV->lpdata, "���ܵ���", strlen("���ܵ���")+1);
					break;
				case 16:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("��������")+1;
					memcpy(&pDBTFLV->lpdata, "��������", strlen("��������")+1);
					break;
				case 17:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("���۵���")+1;
					memcpy(&pDBTFLV->lpdata, "���۵���", strlen("���۵���")+1);
					break;
				case 18:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("��Ƕ��ʯ/��۵���")+1;
					memcpy(&pDBTFLV->lpdata, "��Ƕ��ʯ/��۵���", strlen("��Ƕ��ʯ/��۵���")+1);
					break;
				case 19:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("�߻���")+1;
					memcpy(&pDBTFLV->lpdata, "�߻���", strlen("�߻���")+1);
					break;
				case 20:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("�ϲ�����")+1;
					memcpy(&pDBTFLV->lpdata, "�ϲ�����", strlen("�ϲ�����")+1);
					break;
				case 21:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("�ϳɵ���")+1;
					memcpy(&pDBTFLV->lpdata, "�ϳɵ���", strlen("�ϳɵ���")+1);
					break;
				case 22:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("�û�����")+1;
					memcpy(&pDBTFLV->lpdata, "�û�����", strlen("�û�����")+1);
					break;
				case 23:
					*pDBTFLV->lpdata = NULL;
					memcpy(&pDBTFLV->lpdata, "����ܹ���", strlen("����ܹ���")+1);
					break;
				case 24:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("ͣ��ȯ")+1;
					memcpy(&pDBTFLV->lpdata, "ͣ��ȯ", strlen("ͣ��ȯ")+1);
					break;
				case 25:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("����װ���û�����")+1;
					memcpy(&pDBTFLV->lpdata, "����װ���û�����", strlen("����װ���û�����")+1);
					break;
				case 26:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("����Ƶ������")+1;
					memcpy(&pDBTFLV->lpdata, "����Ƶ������", strlen("����Ƶ������")+1);
					break;
				case 27:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("ҩˮ")+1;
					memcpy(&pDBTFLV->lpdata, "ҩˮ", strlen("ҩˮ")+1);
					break;
				case 28:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("ҩ��")+1;
					memcpy(&pDBTFLV->lpdata, "ҩ��", strlen("ҩ��")+1);
					break;
				case 29:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("ʳ��")+1;
					memcpy(&pDBTFLV->lpdata, "ʳ��", strlen("ʳ��")+1);
					break;
				case 30:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("����")+1;
					memcpy(&pDBTFLV->lpdata, "����", strlen("����")+1);
					break;
				case 31:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("ը��")+1;
					memcpy(&pDBTFLV->lpdata, "ը��", strlen("ը��")+1);
					break;
				case 32:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("CP����Ʒ")+1;
					memcpy(&pDBTFLV->lpdata, "CP����Ʒ", strlen("CP����Ʒ")+1);
					break;
				case 33:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("������������")+1;
					memcpy(&pDBTFLV->lpdata, "������������", strlen("������������")+1);
					break;
				case 34:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("DNA������")+1;
					memcpy(&pDBTFLV->lpdata, "DNA������", strlen("DNA������")+1);
					break;
				case 35:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("���������")+1;
					memcpy(&pDBTFLV->lpdata, "���������", strlen("���������")+1);
					break;
				case 36:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("װ���������˵���")+1;
					memcpy(&pDBTFLV->lpdata, "װ���������˵���", strlen("װ���������˵���")+1);
					break;
				default:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("")+1;
					memcpy(&pDBTFLV->lpdata, "", strlen("")+1);
					break;
				}
			}
			else
				continue;
		}
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "�̳���Ʒ������־��ѯ");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	return DBTFLV;
}

//�����Ʒ��־:���׻��
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_DB_TransGain_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	char logDBServerIP[40];
	char gameDBServerIP[40];
	vector<CGlobalStruct::TFLV>::iterator pDBTFLV = NULL;
	CString strItemID = "";
	vector<st_ItemInfo>::iterator pItemInfo = NULL;
	vector<CGlobalStruct::DataRecord> datarecord;
	vector<string> LogDbName;
	vector<string>::iterator pLogDbName = NULL;
	CString strTemSql = "";
	char retMessage[255];
	ZeroMemory(retMessage,255);
	Vec_ItemInfo VecItemInfo;

	try
	{
		sprintf(retMessage, "�����Ʒ��־:���׻��:<����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character,BeginTime, EndTime);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG

		//��ȡLogDB��ip
		if(!m_operXDJG.GetServerIP(logDBServerIP,ServerIP,2))
		{
			return DBTFLV;//�޷���ȡLogDB��IP
		}

		//��ȡLogDB������
// 		LogDbName = GetLogDbName(BeginTime, EndTime);
// 		if(LogDbName.size() == 0)
// 			return DBTFLV;
// 
// 		pLogDbName = LogDbName.begin();
// 		while(pLogDbName != LogDbName.end())
// 		{
// 			strTemSql.Format("Select '%s' AS XDJG_Account, '%s' AS XDJG_Character, \
// 									 target_character_name AS XDJG_CharacterFrom,item_id_gain_1 AS XDJG_TempItemId1, \
// 									 item_num_gain_1 AS XDJG_ItemNum1,item_id_gain_1 AS XDJG_ItemID1,item_id_gain_2 AS XDJG_TempItemId2, \
// 									 item_num_gain_2 AS XDJG_ItemNum2,item_id_gain_2 AS XDJG_ItemID2,item_id_gain_3 AS XDJG_TempItemId3, \
// 									 item_num_gain_3 AS XDJG_ItemNum3,item_id_gain_3 AS XDJG_ItemID3,item_id_gain_4 AS XDJG_TempItemId4, \
// 									 item_num_gain_4 AS XDJG_ItemNum4,item_id_gain_4 AS XDJG_ItemID4,item_id_gain_5 AS XDJG_TempItemId5, \
// 									 item_num_gain_5 AS XDJG_ItemNum5,item_id_gain_5 AS XDJG_ItemID5,item_id_gain_6 AS XDJG_TempItemId6, \
// 									 item_num_gain_6 AS XDJG_ItemNum6,item_id_gain_6 AS XDJG_ItemID6,item_id_gain_7 AS XDJG_TempItemId7, \
// 									 item_num_gain_7 AS XDJG_ItemNum7,item_id_gain_7 AS XDJG_ItemID7,item_id_gain_8 AS XDJG_TempItemId8, \
// 									 item_num_gain_8 AS XDJG_ItemNum8,item_id_gain_8 AS XDJG_ItemID8,create_time AS XDJG_LogTime \
// 								from %s.TRADE \
// 							   where create_time between '%s' and '%s' and character_name = '%s'", \
// 									 Account,"1231312",pLogDbName->c_str(),BeginTime,EndTime,"1231312");
// 
// 			strSql += strTemSql;
// 			pLogDbName++;
// 			if(pLogDbName != LogDbName.end())
// 				strSql += " UNION ";
// 		}

		strSql.Format("Select '%s' AS XDJG_Account, '%s' AS XDJG_Character, \
							  target_character_name AS XDJG_CharacterFrom,item_id_gain_1 AS XDJG_TempItemId1, \
							  item_num_gain_1 AS XDJG_ItemNum1,item_id_gain_1 AS XDJG_ItemID1,item_id_gain_2 AS XDJG_TempItemId2, \
							  item_num_gain_2 AS XDJG_ItemNum2,item_id_gain_2 AS XDJG_ItemID2,item_id_gain_3 AS XDJG_TempItemId3, \
							  item_num_gain_3 AS XDJG_ItemNum3,item_id_gain_3 AS XDJG_ItemID3,item_id_gain_4 AS XDJG_TempItemId4, \
							  item_num_gain_4 AS XDJG_ItemNum4,item_id_gain_4 AS XDJG_ItemID4,item_id_gain_5 AS XDJG_TempItemId5, \
							  item_num_gain_5 AS XDJG_ItemNum5,item_id_gain_5 AS XDJG_ItemID5,item_id_gain_6 AS XDJG_TempItemId6, \
							  item_num_gain_6 AS XDJG_ItemNum6,item_id_gain_6 AS XDJG_ItemID6,item_id_gain_7 AS XDJG_TempItemId7, \
							  item_num_gain_7 AS XDJG_ItemNum7,item_id_gain_7 AS XDJG_ItemID7,item_id_gain_8 AS XDJG_TempItemId8, \
							  item_num_gain_8 AS XDJG_ItemNum8,item_id_gain_8 AS XDJG_ItemID8,create_time AS XDJG_LogTime \
						 from %s.TRADE \
					    where create_time between '%s' and '%s' and character_name = '%s'", \
							  Account,Character,LogDB,BeginTime,EndTime,Character);

#ifdef _DEBUG
		printf(strSql);
#endif

		DBTFLV = GameDBQuery("XDJG",logDBServerIP,1, iIndex, iPageSize, strSql.GetBuffer(0));

		if(DBTFLV.empty())
		{
			return DBTFLV;
		}

		//ȡ��Ʒ����
		GetItemInfo(VecItemInfo);

		//������id��ɵ�������
		for(pDBTFLV = DBTFLV.begin(); pDBTFLV != DBTFLV.end(); pDBTFLV++)
		{
			if(pDBTFLV->m_tagName == CEnumCore::TagName::XDJG_TempItemId1 ||
			   pDBTFLV->m_tagName == CEnumCore::TagName::XDJG_TempItemId2 ||
			   pDBTFLV->m_tagName == CEnumCore::TagName::XDJG_TempItemId3 ||
			   pDBTFLV->m_tagName == CEnumCore::TagName::XDJG_TempItemId4 ||
			   pDBTFLV->m_tagName == CEnumCore::TagName::XDJG_TempItemId5 ||
			   pDBTFLV->m_tagName == CEnumCore::TagName::XDJG_TempItemId6 ||
			   pDBTFLV->m_tagName == CEnumCore::TagName::XDJG_TempItemId7 ||
			   pDBTFLV->m_tagName == CEnumCore::TagName::XDJG_TempItemId8)
			{
				//For test
// 				pDBTFLV->m_tagName = CEnumCore::TagName::XDJG_ItemName;
// 				pDBTFLV->m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
// 				*pDBTFLV->lpdata = NULL;
// 				pDBTFLV->m_tvlength = strlen("δ֪")+1;
// 				memcpy(&pDBTFLV->lpdata, "δ֪", strlen("δ֪")+1);

				for(pItemInfo = VecItemInfo.begin(); pItemInfo != VecItemInfo.end(); pItemInfo++)
				{
					if(pItemInfo->iItemId == atoi((char*)pDBTFLV->lpdata))
					{
						pDBTFLV->m_tagName = CEnumCore::TagName::XDJG_ItemName;
						pDBTFLV->m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
						*pDBTFLV->lpdata = NULL;
						pDBTFLV->m_tvlength = pItemInfo->strItemName.size();
						memcpy(&pDBTFLV->lpdata, pItemInfo->strItemName.c_str(), pItemInfo->strItemName.size());
						break;
					}
					else
						continue;
				}

				if(pDBTFLV->m_tagName != CEnumCore::TagName::XDJG_ItemName)
				{
					pDBTFLV->m_tagName = CEnumCore::TagName::XDJG_ItemName;
					pDBTFLV->m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("")+1;
					memcpy(&pDBTFLV->lpdata, "", strlen("")+1);
				}
			}
			else
				continue;
		}
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "�����Ʒ��־:���׻��");//��������Ϣд����־�в����ش�����Ϣ
	}

	return DBTFLV;
}

//�����Ʒ��־:����ʧȥ
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_DB_TransLose_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	char realServerIP[40];
	char gameDBServerIP[40];
	vector<CGlobalStruct::TFLV>::iterator pDBTFLV = NULL;
	CString strItemID = "";
	vector<st_ItemInfo>::iterator pItemInfo = NULL;
	vector<CGlobalStruct::DataRecord> datarecord;
	vector<string> LogDbName;
	vector<string>::iterator pLogDbName = NULL;
	CString strTemSql = "";
	char retMessage[255];
	ZeroMemory(retMessage,255);
	Vec_ItemInfo VecItemInfo;
	
	try
	{
//		m_StructInfo.clear();
		
		sprintf(retMessage, "�����Ʒ��־:����ʧȥ:<����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character,BeginTime, EndTime);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		//��ȡGameDB��ip
// 		if(!m_operXDJG.GetServerIP(gameDBServerIP,ServerIP,3))
// 		{
// 			return DBTFLV;//�޷���ȡGameDB��IP
// 		}
// 		
// 		//ȡ����������Ʒ�����ֶ�
// 		strSql.Format("select items AS XDJG_Items \
// 						 from %s.character \
// 						where name = '%s' \
// 						  and account_name = '%s'",GameDB, Character, Account);
// 		
// 		m_SqlHelper.SqlHelperMain("XDJG", &datarecord,gameDBServerIP,strSql.GetBuffer(0),1,0,0);
// 		
// 		vector<CGlobalStruct::DataRecord>::iterator iter;
// 		CGlobalStruct::DataRecord  m_DataRecord;
// 		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
// 		{
// 			m_DataRecord = *iter;
// 			if(!strcmp("XDJG_Items", m_DataRecord.recordfieldname))
// 			{
// 				memcpy(&m_StructInfo.m_sResCont , m_DataRecord.recorddata, sizeof(m_StructInfo.m_sResCont));//��Ʒ����
// 				break;
// 			}
// 			else
// 				continue;
// 		}
// 		
// 		//���buff
// 		strSql = "";

		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "�����Ʒ��־:����ʧȥ");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		//��ȡLogDB������
// 		LogDbName = GetLogDbName(BeginTime, EndTime);
// 		if(LogDbName.size() == 0)
// 			return DBTFLV;
// 
// 		pLogDbName = LogDbName.begin();
// 		while(pLogDbName != LogDbName.end())
// 		{
// 			strTemSql.Format("Select '%s' AS XDJG_Account, '%s' AS XDJG_Character, \
// 								     target_character_name AS XDJG_CharacterFrom,item_id_throw_1 AS XDJG_TempItemId1, \
// 								     item_num_throw_1 AS XDJG_ItemNum1,item_id_throw_1 AS XDJG_ItemID1,item_id_throw_2 AS XDJG_TempItemId2, \
// 								     item_num_throw_2 AS XDJG_ItemNum2,item_id_throw_2 AS XDJG_ItemID2,item_id_throw_3 AS XDJG_TempItemId3, \
// 								     item_num_throw_3 AS XDJG_ItemNum3,item_id_throw_3 AS XDJG_ItemID3,item_id_throw_4 AS XDJG_TempItemId4, \
// 								     item_num_throw_4 AS XDJG_ItemNum4,item_id_throw_4 AS XDJG_ItemID4,item_id_throw_5 AS XDJG_TempItemId5, \
// 								     item_num_throw_5 AS XDJG_ItemNum5,item_id_throw_5 AS XDJG_ItemID5,item_id_throw_6 AS XDJG_TempItemId6, \
// 								     item_num_throw_6 AS XDJG_ItemNum6,item_id_throw_6 AS XDJG_ItemID6,item_id_throw_7 AS XDJG_TempItemId7, \
// 								     item_num_throw_7 AS XDJG_ItemNum7,item_id_throw_7 AS XDJG_ItemID7,item_id_throw_8 AS XDJG_TempItemId8, \
// 								     item_num_throw_8 AS XDJG_ItemNum8,item_id_throw_8 AS XDJG_ItemID8,create_time AS XDJG_LogTime \
// 							    from %s.TRADE \
// 							   where create_time between '%s' and '%s' and character_name = '%s'", \
// 								     Account,Character,LogDB,BeginTime,EndTime,Character);
// 		
// 		strSql += strTemSql;
// 		pLogDbName++;
// 		if(pLogDbName != LogDbName.end())
// 			strSql += " UNION ";
// 		}

		strSql.Format("Select '%s' AS XDJG_Account, '%s' AS XDJG_Character, \
							  target_character_name AS XDJG_CharacterFrom,item_id_throw_1 AS XDJG_TempItemId1, \
							  item_num_throw_1 AS XDJG_ItemNum1,item_id_throw_1 AS XDJG_ItemID1,item_id_throw_2 AS XDJG_TempItemId2, \
							  item_num_throw_2 AS XDJG_ItemNum2,item_id_throw_2 AS XDJG_ItemID2,item_id_throw_3 AS XDJG_TempItemId3, \
							  item_num_throw_3 AS XDJG_ItemNum3,item_id_throw_3 AS XDJG_ItemID3,item_id_throw_4 AS XDJG_TempItemId4, \
							  item_num_throw_4 AS XDJG_ItemNum4,item_id_throw_4 AS XDJG_ItemID4,item_id_throw_5 AS XDJG_TempItemId5, \
							  item_num_throw_5 AS XDJG_ItemNum5,item_id_throw_5 AS XDJG_ItemID5,item_id_throw_6 AS XDJG_TempItemId6, \
							  item_num_throw_6 AS XDJG_ItemNum6,item_id_throw_6 AS XDJG_ItemID6,item_id_throw_7 AS XDJG_TempItemId7, \
							  item_num_throw_7 AS XDJG_ItemNum7,item_id_throw_7 AS XDJG_ItemID7,item_id_throw_8 AS XDJG_TempItemId8, \
							  item_num_throw_8 AS XDJG_ItemNum8,item_id_throw_8 AS XDJG_ItemID8,create_time AS XDJG_LogTime \
						 from %s.TRADE \
						where create_time between '%s' and '%s' and character_name = '%s'", \
							  Account,Character,LogDB,BeginTime,EndTime,Character);

#ifdef _DEBUG
		printf(strSql);
#endif
		
		DBTFLV = GameDBQuery("XDJG",realServerIP,13, iIndex, iPageSize, strSql.GetBuffer(0));
		
		if(DBTFLV.empty())
		{
			return DBTFLV;
		}
		
		//ȡ��Ʒ����
		GetItemInfo(VecItemInfo);

		//������id��ɵ�������
		for(pDBTFLV = DBTFLV.begin(); pDBTFLV != DBTFLV.end(); pDBTFLV++)
		{
			if(pDBTFLV->m_tagName == CEnumCore::TagName::XDJG_TempItemId1 ||
				pDBTFLV->m_tagName == CEnumCore::TagName::XDJG_TempItemId2 ||
				pDBTFLV->m_tagName == CEnumCore::TagName::XDJG_TempItemId3 ||
				pDBTFLV->m_tagName == CEnumCore::TagName::XDJG_TempItemId4 ||
				pDBTFLV->m_tagName == CEnumCore::TagName::XDJG_TempItemId5 ||
				pDBTFLV->m_tagName == CEnumCore::TagName::XDJG_TempItemId6 ||
				pDBTFLV->m_tagName == CEnumCore::TagName::XDJG_TempItemId7 ||
				pDBTFLV->m_tagName == CEnumCore::TagName::XDJG_TempItemId8)
			{
				//For test
// 				pDBTFLV->m_tagName = CEnumCore::TagName::XDJG_ItemName;
// 				pDBTFLV->m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
// 				*pDBTFLV->lpdata = NULL;
// 				pDBTFLV->m_tvlength = strlen("δ֪")+1;
// 				memcpy(&pDBTFLV->lpdata, "δ֪", strlen("δ֪")+1);
				
				for(pItemInfo = VecItemInfo.begin(); pItemInfo != VecItemInfo.end(); pItemInfo++)
				{
					if(pItemInfo->iItemId == atoi((char*)pDBTFLV->lpdata))
					{
						pDBTFLV->m_tagName = CEnumCore::TagName::XDJG_ItemName;
						pDBTFLV->m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
						*pDBTFLV->lpdata = NULL;
						pDBTFLV->m_tvlength = pItemInfo->strItemName.size();
						memcpy(&pDBTFLV->lpdata, pItemInfo->strItemName.c_str(), pItemInfo->strItemName.size());
						break;
					}
					else
						continue;
				}
				
				if(pDBTFLV->m_tagName != CEnumCore::TagName::XDJG_ItemName)
				{
					pDBTFLV->m_tagName = CEnumCore::TagName::XDJG_ItemName;
					pDBTFLV->m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("")+1;
					memcpy(&pDBTFLV->lpdata, "", strlen("")+1);
				}
			}
			else
				continue;
		}
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "�����Ʒ��־:����ʧȥ");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	return DBTFLV;
}

//�����Ʒ��־:��NPC������
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_DB_BuyFromNpc_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	char realServerIP[40];
	char gameDBServerIP[40];
	vector<CGlobalStruct::TFLV>::iterator pDBTFLV = NULL;
	CString strItemID = "";
	vector<st_ItemInfo>::iterator pItemInfo = NULL;
	vector<CGlobalStruct::DataRecord> datarecord;
	vector<string> LogDbName;
	vector<string>::iterator pLogDbName = NULL;
	CString strTemSql = "";
	char retMessage[255];
	ZeroMemory(retMessage,255);
	Vec_ItemInfo VecItemInfo;
	
	try
	{
//		m_StructInfo.clear();
		
		sprintf(retMessage, "�����Ʒ��־:��NPC������:<����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character,BeginTime, EndTime);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		//��ȡGameDB��ip
// 		if(!m_operXDJG.GetServerIP(gameDBServerIP,ServerIP,3))
// 		{
// 			return DBTFLV;//�޷���ȡGameDB��IP
// 		}
// 		
// 		//ȡ����������Ʒ�����ֶ�
// 		strSql.Format("select items AS XDJG_Items \
// 						 from %s.character \
// 						where name = '%s' \
// 						  and account_name = '%s'",GameDB, Character, Account);
// 		
// 		m_SqlHelper.SqlHelperMain("XDJG", &datarecord,gameDBServerIP,strSql.GetBuffer(0),1,0,0);
// 		
// 		vector<CGlobalStruct::DataRecord>::iterator iter;
// 		CGlobalStruct::DataRecord  m_DataRecord;
// 		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
// 		{
// 			m_DataRecord = *iter;
// 			if(!strcmp("XDJG_Items", m_DataRecord.recordfieldname))
// 			{
// 				memcpy(&m_StructInfo.m_sResCont , m_DataRecord.recorddata, sizeof(m_StructInfo.m_sResCont));//��Ʒ����
// 				break;
// 			}
// 			else
// 				continue;
// 		}
// 		
// 		//���buff
// 		strSql = "";

		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "�����Ʒ��־:��NPC������");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		//��ȡLogDB������
// 		LogDbName = GetLogDbName(BeginTime, EndTime);
// 		if(LogDbName.size() == 0)
// 			return DBTFLV;
// 		
// 		pLogDbName = LogDbName.begin();
// 		while(pLogDbName != LogDbName.end())
// 		{
// 			strTemSql.Format("Select '%s' AS XDJG_Account, '%s' AS XDJG_Character, \
// 								     target_character_name AS XDJG_CharacterFrom,item_id_gain_1 AS XDJG_TempItemID, \
// 								     item_num_gain_1 AS XDJG_ItemNum,item_id_gain_1 AS XDJG_ItemID,create_time AS XDJG_LogTime, \
// 								     (bind_money_lose+money_lose) AS XDJG_Money, \
// 								     CASE bind_money_lose WHEN 0 THEN '�ǰ󶨱�' \
// 													             ELSE '�󶨱�' END AS XDJG_MoneyType \
// 							    from %s.BUY \
// 							   where create_time between '%s' and '%s' and character_name = '%s'", \
// 							         Account,Character,pLogDbName->c_str(),BeginTime,EndTime,Character);
// 			
// 			strSql += strTemSql;
// 			pLogDbName++;
// 			if(pLogDbName != LogDbName.end())
// 				strSql += " UNION ";
// 		}

		strSql.Format("Select '%s' AS XDJG_Account, '%s' AS XDJG_Character, \
							  target_character_name AS XDJG_CharacterFrom,item_id_gain_1 AS XDJG_TempItemID, \
							  item_num_gain_1 AS XDJG_ItemNum,item_id_gain_1 AS XDJG_ItemID,create_time AS XDJG_LogTime, \
							  (bind_money_lose+money_lose) AS XDJG_Money, \
							  CASE bind_money_lose WHEN 0 THEN '�ǰ󶨱�' \
							  ELSE '�󶨱�' END AS XDJG_MoneyType \
						 from %s.BUY \
						where create_time between '%s' and '%s' and character_name = '%s'", \
							  Account,Character,LogDB,BeginTime,EndTime,Character);

#ifdef _DEBUG
		printf(strSql);
#endif
		
		DBTFLV = GameDBQuery("XDJG",realServerIP,13, iIndex, iPageSize, strSql.GetBuffer(0));
		
		if(DBTFLV.empty())
		{
			return DBTFLV;
		}
		
		//ȡ��Ʒ����
		GetItemInfo(VecItemInfo);

		//������id��ɵ�������
		for(pDBTFLV = DBTFLV.begin(); pDBTFLV != DBTFLV.end(); pDBTFLV++)
		{
			if(pDBTFLV->m_tagName == CEnumCore::TagName::XDJG_TempItemID)
			{
				//For test
// 				pDBTFLV->m_tagName = CEnumCore::TagName::XDJG_ItemName;
// 				pDBTFLV->m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
// 				*pDBTFLV->lpdata = NULL;
// 				pDBTFLV->m_tvlength = strlen("δ֪")+1;
// 				memcpy(&pDBTFLV->lpdata, "δ֪", strlen("δ֪")+1);
				
				for(pItemInfo = VecItemInfo.begin(); pItemInfo != VecItemInfo.end(); pItemInfo++)
				{
					if(pItemInfo->iItemId == atoi((char*)pDBTFLV->lpdata))
					{
						pDBTFLV->m_tagName = CEnumCore::TagName::XDJG_ItemName;
						pDBTFLV->m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
						*pDBTFLV->lpdata = NULL;
						pDBTFLV->m_tvlength = pItemInfo->strItemName.size();
						memcpy(&pDBTFLV->lpdata, pItemInfo->strItemName.c_str(), pItemInfo->strItemName.size());
						break;
					}
					else
						continue;
				}
				
				if(pDBTFLV->m_tagName != CEnumCore::TagName::XDJG_ItemName)
				{
					pDBTFLV->m_tagName = CEnumCore::TagName::XDJG_ItemName;
					pDBTFLV->m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("")+1;
					memcpy(&pDBTFLV->lpdata, "", strlen("")+1);
				}
			}
			else
				continue;
		}
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "�����Ʒ��־:��NPC������");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	return DBTFLV;
}

//�����Ʒ��־:����NPC
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_DB_SaleToNpc_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	char realServerIP[40];
	char gameDBServerIP[40];
	vector<CGlobalStruct::TFLV>::iterator pDBTFLV = NULL;
	CString strItemID = "";
	vector<st_ItemInfo>::iterator pItemInfo = NULL;
	vector<CGlobalStruct::DataRecord> datarecord;
	vector<string> LogDbName;
	vector<string>::iterator pLogDbName = NULL;
	CString strTemSql = "";
	char retMessage[255];
	ZeroMemory(retMessage,255);
	Vec_ItemInfo VecItemInfo;
	
	try
	{
//		m_StructInfo.clear();
		
		sprintf(retMessage, "�����Ʒ��־:����NPC:<����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character,BeginTime, EndTime);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		//��ȡGameDB��ip
// 		if(!m_operXDJG.GetServerIP(gameDBServerIP,ServerIP,3))
// 		{
// 			return DBTFLV;//�޷���ȡGameDB��IP
// 		}
// 		
// 		//ȡ����������Ʒ�����ֶ�
// 		strSql.Format("select items AS XDJG_Items \
// 						 from %s.character \
// 						where name = '%s' \
// 						  and account_name = '%s'",GameDB, Character, Account);
// 		
// 		m_SqlHelper.SqlHelperMain("XDJG", &datarecord,gameDBServerIP,strSql.GetBuffer(0),1,0,0);
// 		
// 		vector<CGlobalStruct::DataRecord>::iterator iter;
// 		CGlobalStruct::DataRecord  m_DataRecord;
// 		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
// 		{
// 			m_DataRecord = *iter;
// 			if(!strcmp("XDJG_Items", m_DataRecord.recordfieldname))
// 			{
// 				memcpy(&m_StructInfo.m_sResCont , m_DataRecord.recorddata, sizeof(m_StructInfo.m_sResCont));//��Ʒ����
// 				break;
// 			}
// 			else
// 				continue;
// 		}
// 		
// 		//���buff
// 		strSql = "";

		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "�����Ʒ��־:����NPC");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}

		//��ȡLogDB������
// 		LogDbName = GetLogDbName(BeginTime, EndTime);
// 		if(LogDbName.size() == 0)
// 			return DBTFLV;
// 		
// 		pLogDbName = LogDbName.begin();
// 		while(pLogDbName != LogDbName.end())
// 		{
// 			strTemSql.Format("Select '%s' AS XDJG_Account, '%s' AS XDJG_Character, \
// 									 target_character_name AS XDJG_CharacterFrom,item_id AS XDJG_TempItemID, \
// 									 item_num AS XDJG_ItemNum,item_id AS XDJG_ItemID,create_time AS XDJG_LogTime, \
// 									 bind_money_gain AS XDJG_Money \
// 								from %s.SELL \
// 							   where create_time between '%s' and '%s' and character_name = '%s'", \
// 								     Account,Character,pLogDbName->c_str(),BeginTime,EndTime,Character);
// 			
// 			strSql += strTemSql;
// 			pLogDbName++;
// 			if(pLogDbName != LogDbName.end())
// 				strSql += " UNION ";
// 		}

		strSql.Format("Select '%s' AS XDJG_Account, '%s' AS XDJG_Character, \
							  target_character_name AS XDJG_CharacterFrom,item_id AS XDJG_TempItemID, \
							  item_num AS XDJG_ItemNum,item_id AS XDJG_ItemID,create_time AS XDJG_LogTime, \
							  bind_money_gain AS XDJG_Money \
						 from %s.SELL \
						where create_time between '%s' and '%s' and character_name = '%s'", \
							  Account,Character,LogDB,BeginTime,EndTime,Character);

#ifdef _DEBUG
		printf(strSql);
#endif
		
		DBTFLV = GameDBQuery("XDJG",realServerIP,13, iIndex, iPageSize, strSql.GetBuffer(0));
		
		if(DBTFLV.empty())
		{
			return DBTFLV;
		}
		
		//ȡ��Ʒ����
		GetItemInfo(VecItemInfo);

		//������id��ɵ�������
		for(pDBTFLV = DBTFLV.begin(); pDBTFLV != DBTFLV.end(); pDBTFLV++)
		{
			if(pDBTFLV->m_tagName == CEnumCore::TagName::XDJG_TempItemID)
			{
				for(pItemInfo = VecItemInfo.begin(); pItemInfo != VecItemInfo.end(); pItemInfo++)
				{
					if(pItemInfo->iItemId == atoi((char*)pDBTFLV->lpdata))
					{
						pDBTFLV->m_tagName = CEnumCore::TagName::XDJG_ItemName;
						pDBTFLV->m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
						*pDBTFLV->lpdata = NULL;
						pDBTFLV->m_tvlength = pItemInfo->strItemName.size();
						memcpy(&pDBTFLV->lpdata, pItemInfo->strItemName.c_str(), pItemInfo->strItemName.size());
						break;
					}
					else
						continue;
				}
				
				if(pDBTFLV->m_tagName != CEnumCore::TagName::XDJG_ItemName)
				{
					pDBTFLV->m_tagName = CEnumCore::TagName::XDJG_ItemName;
					pDBTFLV->m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("")+1;
					memcpy(&pDBTFLV->lpdata, "", strlen("")+1);
				}
			}
			else
				continue;
		}
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "�����Ʒ��־:����NPC");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	return DBTFLV;
}

//�����Ʒ��־:ʰȡ��Ʒ
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_DB_PickUpItem_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	char realServerIP[40];
	char gameDBServerIP[40];
	vector<CGlobalStruct::DataRecord> datarecord;
	vector<string> LogDbName;
	vector<string>::iterator pLogDbName = NULL;
	CString strTemSql = "";
	char retMessage[255];
	ZeroMemory(retMessage,255);

	try
	{
//		m_StructInfo.clear();
		
		sprintf(retMessage, "�����Ʒ��־:ʰȡ��Ʒ:<����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character,BeginTime, EndTime);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		//��ȡGameDB��ip
// 		if(!m_operXDJG.GetServerIP(gameDBServerIP,ServerIP,3))
// 		{
// 			return DBTFLV;//�޷���ȡGameDB��IP
// 		}
// 		
// 		//ȡ����������Ʒ�����ֶ�
// 		strSql.Format("select items AS XDJG_Items \
// 						 from %s.character \
// 						where name = '%s' \
// 						  and account_name = '%s'",GameDB, Character, Account);
// 		
// 		m_SqlHelper.SqlHelperMain("XDJG", &datarecord,gameDBServerIP,strSql.GetBuffer(0),1,0,0);
// 		
// 		vector<CGlobalStruct::DataRecord>::iterator iter;
// 		CGlobalStruct::DataRecord  m_DataRecord;
// 		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
// 		{
// 			m_DataRecord = *iter;
// 			if(!strcmp("XDJG_Items", m_DataRecord.recordfieldname))
// 			{
// 				memcpy(&m_StructInfo.m_sResCont , m_DataRecord.recorddata, sizeof(m_StructInfo.m_sResCont));//��Ʒ����
// 				break;
// 			}
// 			else
// 				continue;
// 		}
// 		
// 		//���buff
// 		strSql = "";

		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "�����Ʒ��־:ʰȡ��Ʒ");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		//��ȡLogDB������
// 		LogDbName = GetLogDbName(BeginTime, EndTime);
// 		if(LogDbName.size() == 0)
// 			return DBTFLV;
// 		
// 		pLogDbName = LogDbName.begin();
// 		while(pLogDbName != LogDbName.end())
// 		{
// 			strTemSql.Format("Select '%s' AS XDJG_Account, '%s' AS XDJG_Character, \
// 									 target_character_name AS XDJG_CharacterFrom,item_id_gain_1 AS XDJG_TempItemID, \
// 									 item_num_gain_1 AS XDJG_ItemNum,item_id_gain_1 AS XDJG_ItemID,create_time AS XDJG_LogTime, \
// 									 (bind_money_lose+money_lose) AS XDJG_Money, \
// 									 CASE bind_money_lose WHEN 0 THEN '�ǰ󶨱�' \
// 									 ELSE '�󶨱�' END AS XDJG_MoneyType \
// 								from %s.BUY \
// 							   where create_time between '%s' and '%s' and character_name = '%s'", \
// 									 Account,Character,LogDB,BeginTime,EndTime,Character);
// 			
// 			strSql += strTemSql;
// 			pLogDbName++;
// 			if(pLogDbName != LogDbName.end())
// 				strSql += " UNION ";
// 		}

		strSql.Format("Select '%s' AS XDJG_Account, character_name AS XDJG_Character, \
							  item_name AS XDJG_ItemName, item_num AS XDJG_ItemNum, \
							  item_id AS XDJG_ItemID \
						 from %s.GAIN_ITEM \
						where create_time between '%s' and '%s' and character_name = '%s' \
						  and item_change_type = 1", \
							  Account,LogDB,BeginTime,EndTime,Character);

#ifdef _DEBUG
		printf(strSql);
#endif
		
		DBTFLV = GameDBQuery("XDJG",realServerIP,13, iIndex, iPageSize, strSql.GetBuffer(0));
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "�����Ʒ��־:ʰȡ��Ʒ");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	return DBTFLV;
}

//�����Ʒ��־:������Ʒ
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_DB_DiscardItem_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	char realServerIP[40];
	char gameDBServerIP[40];
	vector<CGlobalStruct::DataRecord> datarecord;
	vector<string> LogDbName;
	vector<string>::iterator pLogDbName = NULL;
	CString strTemSql = "";
	char retMessage[255];
	ZeroMemory(retMessage,255);
	
	try
	{
//		m_StructInfo.clear();
		
		sprintf(retMessage, "�����Ʒ��־:������Ʒ:<����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character,BeginTime, EndTime);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		//��ȡGameDB��ip
		if(!m_operXDJG.GetServerIP(gameDBServerIP,ServerIP,3))
		{
			return DBTFLV;//�޷���ȡGameDB��IP
		}
		
		//ȡ����������Ʒ�����ֶ�
// 		strSql.Format("select items AS XDJG_Items \
// 													from %s.character \
// 													where name = '%s' \
// 													and account_name = '%s'",GameDB, Character, Account);
// 		
// 		m_SqlHelper.SqlHelperMain("XDJG", &datarecord,gameDBServerIP,strSql.GetBuffer(0),1,0,0);
// 		
// 		vector<CGlobalStruct::DataRecord>::iterator iter;
// 		CGlobalStruct::DataRecord  m_DataRecord;
// 		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
// 		{
// 			m_DataRecord = *iter;
// 			if(!strcmp("XDJG_Items", m_DataRecord.recordfieldname))
// 			{
// 				memcpy(&m_StructInfo.m_sResCont , m_DataRecord.recorddata, sizeof(m_StructInfo.m_sResCont));//��Ʒ����
// 				break;
// 			}
// 			else
// 				continue;
// 		}
// 		
// 		//���buff
// 		strSql = "";

		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "�����Ʒ��־:������Ʒ");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		//��ȡLogDB������
// 		LogDbName = GetLogDbName(BeginTime, EndTime);
// 		if(LogDbName.size() == 0)
// 			return DBTFLV;
// 		
// 		pLogDbName = LogDbName.begin();
// 		while(pLogDbName != LogDbName.end())
// 		{
// 			strTemSql.Format("Select '%s' AS XDJG_Account, '%s' AS XDJG_Character, \
// 							         item_num AS XDJG_ItemNum, item_id AS XDJG_TempItemID, \
// 							         item_name AS XDJG_ItemName, create_time AS XDJG_LogTime \
// 					            from %s.THROW_ITEM \
// 						       where create_time between '%s' and '%s' and character_name = '%s'", \
// 						             Account,Character,pLogDbName->c_str(),BeginTime,EndTime,Character);
// 			
// 			strSql += strTemSql;
// 			pLogDbName++;
// 			if(pLogDbName != LogDbName.end())
// 				strSql += " UNION ";
// 		}

		strSql.Format("Select '%s' AS XDJG_Account, '%s' AS XDJG_Character, \
							  item_name AS XDJG_ItemName, item_num AS XDJG_ItemNum, \
							  item_id AS XDJG_ItemID, create_time AS XDJG_LogTime \
						 from %s.THROW_ITEM \
						where create_time between '%s' and '%s' and character_name = '%s'", \
						      Account,Character,LogDB,BeginTime,EndTime,Character);

#ifdef _DEBUG
		printf(strSql);
#endif
		
		DBTFLV = GameDBQuery("XDJG",realServerIP,13, iIndex, iPageSize, strSql.GetBuffer(0));
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "�����Ʒ��־:������Ʒ");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	return DBTFLV;
}

//�����Ʒ��־:��Ʒ����--������
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_DB_ConsumeItem_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	vector<CGlobalStruct::TFLV>::iterator pDBTFLV = NULL;
	vector<st_ItemInfo>::iterator pItemInfo = NULL;
	CString strSql = "";
	char realServerIP[40];
	char retMessage[255];
	ZeroMemory(retMessage,255);
	Vec_ItemInfo VecItemInfo;
	
	try
	{
		sprintf(retMessage, "�����Ʒ��־:��Ʒ����:<����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character,BeginTime, EndTime);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			return DBTFLV;//�޷���ȡ������IP
		}
		
		strSql.Format("Select '%s' AS XDJG_Account, '%s' AS XDJG_Character, \
							  item_id AS XDJG_ItemID,  item_id AS XDJG_TempItemID, \
							  create_time AS XDJG_LogTime \
						 from %s.USE_ITEM \
						 where create_time between '%s' and '%s' and character_name = '%s'", \
							  Account,Character,LogDB,BeginTime,EndTime,Character);

#ifdef _DEBUG
		printf(strSql, LogDB, LogDB);
#endif
		
		DBTFLV = GameDBQuery("XDJG",realServerIP,13, iIndex, iPageSize, strSql.GetBuffer(0), LogDB, LogDB);

		if(DBTFLV.empty())
		{
			return DBTFLV;
		}
		
		//ȡ��Ʒ����
		GetItemInfo(VecItemInfo);

		//������id��ɵ�������
		for(pDBTFLV = DBTFLV.begin(); pDBTFLV != DBTFLV.end(); pDBTFLV++)
		{
			if(pDBTFLV->m_tagName == CEnumCore::TagName::XDJG_TempItemID)
			{
				for(pItemInfo = VecItemInfo.begin(); pItemInfo != VecItemInfo.end(); pItemInfo++)
				{
					if(pItemInfo->iItemId == atoi((char*)pDBTFLV->lpdata))
					{
						pDBTFLV->m_tagName = CEnumCore::TagName::XDJG_ItemName;
						pDBTFLV->m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
						*pDBTFLV->lpdata = NULL;
						pDBTFLV->m_tvlength = pItemInfo->strItemName.size();
						memcpy(&pDBTFLV->lpdata, pItemInfo->strItemName.c_str(), pItemInfo->strItemName.size());
						break;
					}
					else
						continue;
				}
				
				if(pDBTFLV->m_tagName != CEnumCore::TagName::XDJG_ItemName)
				{
					pDBTFLV->m_tagName = CEnumCore::TagName::XDJG_ItemName;
					pDBTFLV->m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("")+1;
					memcpy(&pDBTFLV->lpdata, "", strlen("")+1);
				}
			}
			else
				continue;
		}
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "�����Ʒ��־:��Ʒ����");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	return DBTFLV;
}

//�����Ʒ��־:����������
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_DB_AuctioneerSaled_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	char realServerIP[40];
	vector<CGlobalStruct::TFLV>::iterator pDBTFLV = NULL;
	CString strItemID = "";
	vector<st_ItemInfo>::iterator pItemInfo = NULL;
	char gameDBServerIP[40];
	vector<CGlobalStruct::DataRecord> datarecord;
	char retMessage[255];
	ZeroMemory(retMessage,255);
	Vec_ItemInfo VecItemInfo;
	
	try
	{
//		m_StructInfo.clear();
		
		sprintf(retMessage, "�����Ʒ��־:����������:<����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character,BeginTime, EndTime);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		//��ȡGameDB��ip
// 		if(!m_operXDJG.GetServerIP(gameDBServerIP,ServerIP,3))
// 		{
// 			return DBTFLV;//�޷���ȡGameDB��IP
// 		}
// 		
// 		//ȡ����������Ʒ�����ֶ�
// 		strSql.Format("select items AS XDJG_Items \
// 						 from %s.character \
// 						where name = '%s' \
// 						  and account_name = '%s'",GameDB, Character, Account);
// 		
// 		m_SqlHelper.SqlHelperMain("XDJG", &datarecord,gameDBServerIP,strSql.GetBuffer(0),1,0,0);
// 		
// 		vector<CGlobalStruct::DataRecord>::iterator iter;
// 		CGlobalStruct::DataRecord  m_DataRecord;
// 		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
// 		{
// 			m_DataRecord = *iter;
// 			if(!strcmp("XDJG_Items", m_DataRecord.recordfieldname))
// 			{
// 				memcpy(&m_StructInfo.m_sResCont , m_DataRecord.recorddata, sizeof(m_StructInfo.m_sResCont));//��Ʒ����
// 				break;
// 			}
// 			else
// 				continue;
// 		}
// 		
// 		//���buff
// 		strSql = "";

		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "�����Ʒ��־:����������");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		strSql.Format("Select '%s' AS XDJG_Account, '%s' AS XDJG_Character, \
							  item_id AS XDJG_TempItemID, item_num AS XDJG_ItemNum, \
							  item_id AS XDJG_ItemID,create_time AS XDJG_LogTime \
						 from %s.AH_SOLD \
						where create_time between '%s' and '%s' and character_name = '%s'", \
							  Account,Character,LogDB,BeginTime,EndTime,Character);

#ifdef _DEBUG
		printf(strSql, LogDB, LogDB);
#endif
		
		DBTFLV = GameDBQuery("XDJG",realServerIP,13, iIndex, iPageSize, strSql.GetBuffer(0), LogDB, LogDB);
		
		if(DBTFLV.empty())
		{
			return DBTFLV;
		}
		
		//ȡ��Ʒ����
		GetItemInfo(VecItemInfo);

		//������id��ɵ�������
		for(pDBTFLV = DBTFLV.begin(); pDBTFLV != DBTFLV.end(); pDBTFLV++)
		{
			if(pDBTFLV->m_tagName == CEnumCore::TagName::XDJG_TempItemID)
			{
				for(pItemInfo = VecItemInfo.begin(); pItemInfo != VecItemInfo.end(); pItemInfo++)
				{
					if(pItemInfo->iItemId == atoi((char*)pDBTFLV->lpdata))
					{
						pDBTFLV->m_tagName = CEnumCore::TagName::XDJG_ItemName;
						pDBTFLV->m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
						*pDBTFLV->lpdata = NULL;
						pDBTFLV->m_tvlength = pItemInfo->strItemName.size();
						memcpy(&pDBTFLV->lpdata, pItemInfo->strItemName.c_str(), pItemInfo->strItemName.size());
						break;
					}
					else
						continue;
				}
				
				if(pDBTFLV->m_tagName != CEnumCore::TagName::XDJG_ItemName)
				{
					pDBTFLV->m_tagName = CEnumCore::TagName::XDJG_ItemName;
					pDBTFLV->m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("")+1;
					memcpy(&pDBTFLV->lpdata, "", strlen("")+1);
				}
			}
			else
				continue;
		}
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "�����Ʒ��־:����������");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	return DBTFLV;
}

//�����Ʒ��־:�����й���
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_DB_AuctioneerBought_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	char realServerIP[40];
	vector<CGlobalStruct::TFLV>::iterator pDBTFLV = NULL;
	CString strItemID = "";
	vector<st_ItemInfo>::iterator pItemInfo = NULL;
	char gameDBServerIP[40];
	vector<CGlobalStruct::DataRecord> datarecord;
	char retMessage[255];
	ZeroMemory(retMessage,255);
	Vec_ItemInfo VecItemInfo;
	
	try
	{
//		m_StructInfo.clear();
		
		sprintf(retMessage, "�����Ʒ��־:�����й���:<����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character,BeginTime, EndTime);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		//��ȡGameDB��ip
// 		if(!m_operXDJG.GetServerIP(gameDBServerIP,ServerIP,3))
// 		{
// 			return DBTFLV;//�޷���ȡGameDB��IP
// 		}
// 		
// 		//ȡ����������Ʒ�����ֶ�
// 		strSql.Format("select items AS XDJG_Items \
// 						 from %s.character \
// 						where name = '%s' \
// 						  and account_name = '%s'",GameDB, Character, Account);
// 		
// 		m_SqlHelper.SqlHelperMain("XDJG", &datarecord,gameDBServerIP,strSql.GetBuffer(0),1,0,0);
// 		
// 		vector<CGlobalStruct::DataRecord>::iterator iter;
// 		CGlobalStruct::DataRecord  m_DataRecord;
// 		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
// 		{
// 			m_DataRecord = *iter;
// 			if(!strcmp("XDJG_Items", m_DataRecord.recordfieldname))
// 			{
// 				memcpy(&m_StructInfo.m_sResCont , m_DataRecord.recorddata, sizeof(m_StructInfo.m_sResCont));//��Ʒ����
// 				break;
// 			}
// 			else
// 				continue;
// 		}
// 		
// 		//���buff
// 		strSql = "";

		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "�����Ʒ��־:�����й���");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		strSql.Format("Select '%s' AS XDJG_Account, '%s' AS XDJG_Character, \
							  item_id AS XDJG_TempItemID, item_num AS XDJG_ItemNum, \
							  item_id AS XDJG_ItemID,create_time AS XDJG_LogTime \
						 from %s.AH_SOLD \
						where create_time between '%s' and '%s' and target_character_name = '%s'", \
							  Account,Character,LogDB,BeginTime,EndTime,Character);

#ifdef _DEBUG
		printf(strSql, LogDB, LogDB);
#endif
		
		DBTFLV = GameDBQuery("XDJG",realServerIP,13, iIndex, iPageSize, strSql.GetBuffer(0), LogDB, LogDB);
		
		if(DBTFLV.empty())
		{
			return DBTFLV;
		}
		
		//ȡ��Ʒ����
		GetItemInfo(VecItemInfo);

		//������id��ɵ�������
		for(pDBTFLV = DBTFLV.begin(); pDBTFLV != DBTFLV.end(); pDBTFLV++)
		{
			if(pDBTFLV->m_tagName == CEnumCore::TagName::XDJG_TempItemID)
			{
				for(pItemInfo = VecItemInfo.begin(); pItemInfo != VecItemInfo.end(); pItemInfo++)
				{
					if(pItemInfo->iItemId == atoi((char*)pDBTFLV->lpdata))
					{
						pDBTFLV->m_tagName = CEnumCore::TagName::XDJG_ItemName;
						pDBTFLV->m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
						*pDBTFLV->lpdata = NULL;
						pDBTFLV->m_tvlength = pItemInfo->strItemName.size();
						memcpy(&pDBTFLV->lpdata, pItemInfo->strItemName.c_str(), pItemInfo->strItemName.size());
						break;
					}
					else
						continue;
				}
				
				if(pDBTFLV->m_tagName != CEnumCore::TagName::XDJG_ItemName)
				{
					pDBTFLV->m_tagName = CEnumCore::TagName::XDJG_ItemName;
					pDBTFLV->m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("")+1;
					memcpy(&pDBTFLV->lpdata, "", strlen("")+1);
				}
			}
			else
				continue;
		}
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "�����Ʒ��־:�����й���");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	return DBTFLV;
}

//����ʼ���־:�����ʼ�
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_DB_SendMail_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	char realServerIP[40];
	char gameDBServerIP[40];
	vector<CGlobalStruct::DataRecord> datarecord;
	char retMessage[255];
	ZeroMemory(retMessage,255);
	
	try
	{
//		m_StructInfo.clear();
		
		sprintf(retMessage, "����ʼ���־:�����ʼ�:<����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character,BeginTime, EndTime);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		//��ȡGameDB��ip
// 		if(!m_operXDJG.GetServerIP(gameDBServerIP,ServerIP,3))
// 		{
// 			return DBTFLV;//�޷���ȡGameDB��IP
// 		}
// 		
// 		//ȡ����������Ʒ�����ֶ�
// 		strSql.Format("select items AS XDJG_Items \
// 						 from %s.character \
// 						where name = '%s' \
// 						  and account_name = '%s'",GameDB, Character, Account);
// 		
// 		m_SqlHelper.SqlHelperMain("XDJG", &datarecord,gameDBServerIP,strSql.GetBuffer(0),1,0,0);
// 		
// 		vector<CGlobalStruct::DataRecord>::iterator iter;
// 		CGlobalStruct::DataRecord  m_DataRecord;
// 		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
// 		{
// 			m_DataRecord = *iter;
// 			if(!strcmp("XDJG_Items", m_DataRecord.recordfieldname))
// 			{
// 				memcpy(&m_StructInfo.m_sResCont , m_DataRecord.recorddata, sizeof(m_StructInfo.m_sResCont));//��Ʒ����
// 				break;
// 			}
// 			else
// 				continue;
// 		}
// 		
// 		//���buff
// 		strSql = "";

		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "����ʼ���־:�����ʼ�");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		strSql.Format("Select '%s' AS XDJG_Account, '%s' AS XDJG_Character, \
							  target_character_name AS XDJG_CharacterFrom, item_name AS XDJG_ItemName, \
							  item_num AS XDJG_ItemNum,item_id AS XDJG_ItemID, create_time AS XDJG_LogTime \
						 from %s.SEND_MAIL \
						where create_time between '%s' and '%s' and character_name = '%s'", \
							  Account,Character,LogDB,BeginTime,EndTime,Character);
		
#ifdef _DEBUG
		printf(strSql, LogDB, LogDB);
#endif
		
		DBTFLV = GameDBQuery("XDJG",realServerIP,13, iIndex, iPageSize, strSql.GetBuffer(0), LogDB, LogDB);
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "����ʼ���־:�����ʼ�");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	return DBTFLV;
}

//����ʼ���־:��ȡ�ʼ�
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_DB_ReceiveMail_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	char realServerIP[40];
	char gameDBServerIP[40];
	vector<CGlobalStruct::DataRecord> datarecord;
	char retMessage[255];
	ZeroMemory(retMessage,255);
	
	try
	{
//		m_StructInfo.clear();
		
		sprintf(retMessage, "����ʼ���־:��ȡ�ʼ�:<����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character,BeginTime, EndTime);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		//��ȡGameDB��ip
// 		if(!m_operXDJG.GetServerIP(gameDBServerIP,ServerIP,3))
// 		{
// 			return DBTFLV;//�޷���ȡGameDB��IP
// 		}
// 		
// 		//ȡ����������Ʒ�����ֶ�
// 		strSql.Format("select items AS XDJG_Items \
// 						 from %s.character \
// 						where name = '%s' \
// 						  and account_name = '%s'",GameDB, Character, Account);
// 		
// 		m_SqlHelper.SqlHelperMain("XDJG", &datarecord,gameDBServerIP,strSql.GetBuffer(0),1,0,0);
// 		
// 		vector<CGlobalStruct::DataRecord>::iterator iter;
// 		CGlobalStruct::DataRecord  m_DataRecord;
// 		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
// 		{
// 			m_DataRecord = *iter;
// 			if(!strcmp("XDJG_Items", m_DataRecord.recordfieldname))
// 			{
// 				memcpy(&m_StructInfo.m_sResCont , m_DataRecord.recorddata, sizeof(m_StructInfo.m_sResCont));//��Ʒ����
// 				break;
// 			}
// 			else
// 				continue;
// 		}
// 		
// 		//���buff
// 		strSql = "";
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "����ʼ���־:��ȡ�ʼ�");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		strSql.Format("Select '%s' AS XDJG_Account, '%s' AS XDJG_Character, \
							  character_name AS XDJG_CharacterFrom, item_name AS XDJG_ItemName, \
							  item_num AS XDJG_ItemNum,item_id AS XDJG_ItemID, create_time AS XDJG_LogTime \
						 from %s.RECEIVE_MAIL \
						where create_time between '%s' and '%s' and target_character_name = '%s'", \
							  Account,Character,LogDB,BeginTime,EndTime,Character);
		
#ifdef _DEBUG
		printf(strSql, LogDB, LogDB);
#endif
		
		DBTFLV = GameDBQuery("XDJG",realServerIP,13, iIndex, iPageSize, strSql.GetBuffer(0), LogDB, LogDB);
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "����ʼ���־:��ȡ�ʼ�");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	return DBTFLV;
}

//M��ʹ����־:�����������Ѽ�¼
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_DB_ExpansionPack_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	char realServerIP[40];
	
	try
	{
		logFile.WriteLog("<�������>",operName,"M��ʹ����־:�����������Ѽ�¼");//��LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "���������Ϣ��ѯ");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		strSql.Format("Select '%s' AS XDJG_Account, '%s' AS XDJG_Character, \
							  target_character_name AS XDJG_CharacterFrom, item_name AS XDJG_ItemName, \
							  item_num AS XDJG_ItemNum,item_id AS XDJG_ItemID, create_time AS XDJG_LogTime \
						 from %s.RECEIVE_MAIL \
						where create_time between '%s' and '%s' and character_name = '%s'", \
							  Account,Character,LogDB,BeginTime,EndTime,Character);
		
#ifdef _DEBUG
		printf(strSql, LogDB, LogDB);
#endif
		
		DBTFLV = GameDBQuery("XDJG",realServerIP,13, iIndex, iPageSize, strSql.GetBuffer(0), LogDB, LogDB);
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "M��ʹ����־:�����������Ѽ�¼");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	return DBTFLV;
}

//M��ʹ����־:������־
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_DB_Deliver_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	char realServerIP[40];
	
	try
	{
		logFile.WriteLog("<�������>",operName,"M��ʹ����־:������־");//��LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "���������Ϣ��ѯ");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		strSql.Format("Select '%s' AS XDJG_Account, '%s' AS XDJG_Character, \
							  target_character_name AS XDJG_CharacterFrom, item_name AS XDJG_ItemName, \
							  item_num AS XDJG_ItemNum,item_id AS XDJG_ItemID, create_time AS XDJG_LogTime \
						 from %s.RECEIVE_MAIL \
						where create_time between '%s' and '%s' and character_name = '%s'", \
							  Account,Character,LogDB,BeginTime,EndTime,Character);
		
#ifdef _DEBUG
		printf(strSql, LogDB, LogDB);
#endif
		
		DBTFLV = GameDBQuery("XDJG",realServerIP,13, iIndex, iPageSize, strSql.GetBuffer(0), LogDB, LogDB);
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "M��ʹ����־:������־");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	return DBTFLV;
}

//M��ʹ����־:��ɫ�ֿ��������Ѽ�¼
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_DB_ExpansionWarehouse_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	char realServerIP[40];
	
	try
	{
		logFile.WriteLog("<�������>",operName,"M��ʹ����־:��ɫ�ֿ��������Ѽ�¼");//��LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "���������Ϣ��ѯ");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		strSql.Format("Select '%s' AS XDJG_Account, '%s' AS XDJG_Character, \
							  target_character_name AS XDJG_CharacterFrom, item_name AS XDJG_ItemName, \
							  item_num AS XDJG_ItemNum,item_id AS XDJG_ItemID, create_time AS XDJG_LogTime \
						 from %s.RECEIVE_MAIL \
						where create_time between '%s' and '%s' and character_name = '%s'", \
							  Account,Character,LogDB,BeginTime,EndTime,Character);
		
#ifdef _DEBUG
		printf(strSql, LogDB, LogDB);
#endif
		
		DBTFLV = GameDBQuery("XDJG",realServerIP,13, iIndex, iPageSize, strSql.GetBuffer(0), LogDB, LogDB);
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "M��ʹ����־:��ɫ�ֿ��������Ѽ�¼");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	return DBTFLV;
}

//��ҽ�ɫ��־:��ɫ����
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_DB_RoleUpgrade_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	char realServerIP[40];
	char retMessage[255];
	ZeroMemory(retMessage,255);
	
	try
	{
		sprintf(retMessage, "��ҽ�ɫ��־:��ɫ����:<����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character,BeginTime, EndTime);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "��ҽ�ɫ��־:��ɫ����");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		strSql.Format("Select '%s' AS XDJG_Account, '%s' AS XDJG_Character, \
							  create_time AS XDJG_LogTime, level AS XDJG_Level \
						 from %s.PLAYER_LEVELUP \
						where create_time between '%s' and '%s' and character_name = '%s'", \
							  Account,Character,LogDB,BeginTime,EndTime,Character);
		
#ifdef _DEBUG
		printf(strSql, LogDB, LogDB);
#endif
		
		DBTFLV = GameDBQuery("XDJG",realServerIP,13, iIndex, iPageSize, strSql.GetBuffer(0), LogDB, LogDB);
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "��ҽ�ɫ��־:��ɫ����");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	return DBTFLV;
}

//��ҽ�ɫ��־:��ɫ����
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_DB_RoleCreate_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	char realServerIP[40];
	char retMessage[255];
	ZeroMemory(retMessage,255);
	
	try
	{
		sprintf(retMessage, "��ҽ�ɫ��־:��ɫ����:<����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character,BeginTime, EndTime);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "��ҽ�ɫ��־:��ɫ����");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		strSql.Format("Select account_name AS XDJG_Account, '%s' AS XDJG_Character, \
							  create_time AS XDJG_LogTime \
						 from %s.CREATE_CHARACTER \
						where create_time between '%s' and '%s' and character_name = '%s'", \
							  Character,LogDB,BeginTime,EndTime,Character);
		
#ifdef _DEBUG
		printf(strSql, LogDB, LogDB);
#endif
		
		DBTFLV = GameDBQuery("XDJG",realServerIP,13, iIndex, iPageSize, strSql.GetBuffer(0), LogDB, LogDB);
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "��ҽ�ɫ��־:��ɫ����");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	return DBTFLV;	
}

//��ҽ�ɫ��־:��ɫ����ѧϰ
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_DB_RoleSkill_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	vector<CGlobalStruct::TFLV>::iterator pDBTFLV = NULL;
	CString strSql = "";
	char realServerIP[40];
	vector<CGlobalStruct::DataRecord> datarecord;
	char retMessage[255];
	ZeroMemory(retMessage,255);
	
	try
	{
		sprintf(retMessage, "��ҽ�ɫ��־:��ɫ����ѧϰ:<����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character,BeginTime, EndTime);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "��ҽ�ɫ��־:��ɫ����ѧϰ");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		strSql.Format("Select '%s' AS XDJG_Character, '%s' AS XDJG_Account, \
							  create_time AS XDJG_LogTime, skill_id AS XDJG_SkillId, \
							  skill_id AS XDJG_TempSkillId, skill_level AS XDJG_Level\
						 from %s.PLAYER_LEARN_BATTLESKILL \
						where create_time between '%s' and '%s' and character_name = '%s'", \
							  Character,Account,LogDB,BeginTime,EndTime,Character);
		
#ifdef _DEBUG
		printf(strSql, LogDB, LogDB);
#endif
		
		DBTFLV = GameDBQuery("XDJG",realServerIP,13, iIndex, iPageSize, strSql.GetBuffer(0), LogDB, LogDB);

		if(DBTFLV.empty())
			return DBTFLV;

		for (pDBTFLV = DBTFLV.begin(); pDBTFLV != DBTFLV.end(); pDBTFLV++)
		{
			if(pDBTFLV->m_tagName == CEnumCore::TagName::XDJG_TempSkillId)
			{
				if(strlen((char*)pDBTFLV->lpdata) == 5)
				{
					*(pDBTFLV->lpdata + 3) = '0';
					*(pDBTFLV->lpdata + 4) = '1';
				}
				strSql.Format("select SkillName AS XDJG_SkillName from XDJG_SkillList where SkillId = %d", atoi((char*)pDBTFLV->lpdata));
				
				m_SqlHelper.SqlHelperMain("XDJG", &datarecord,"127.0.0.1",strSql.GetBuffer(0),4,0,0);
				
				if(datarecord.empty())
				{
					pDBTFLV->m_tagName = CEnumCore::TagName::XDJG_SkillName;
					pDBTFLV->m_tvlength = strlen("")+1;
					*pDBTFLV->lpdata = NULL;
					memcpy((char*)&pDBTFLV->lpdata, "", strlen("")+1);
				}

				vector<CGlobalStruct::DataRecord>::iterator iter;
				CGlobalStruct::DataRecord  m_DataRecord;
				for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
				{
					m_DataRecord = *iter;
					if(!strcmp("XDJG_SkillName", m_DataRecord.recordfieldname))
					{
						pDBTFLV->m_tagName = CEnumCore::TagName::XDJG_SkillName;
						pDBTFLV->m_tvlength = strlen(m_DataRecord.recorddata)+1;
						*pDBTFLV->lpdata = NULL;
						memcpy((char*)&pDBTFLV->lpdata, m_DataRecord.recorddata, strlen(m_DataRecord.recorddata)+1);
						break;
					}
					else
						continue;
				}
			}
		}
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "��ҽ�ɫ��־:��ɫ����ѧϰ");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	return DBTFLV;
}

//��ҽ�ɫ��־:��ȡ����
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_DB_ReceiveTask_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	vector<CGlobalStruct::TFLV>::iterator pDBTFLV = NULL;
	CString strSql = "";
	char realServerIP[40];
	char retMessage[255];
	ZeroMemory(retMessage,255);
	
	try
	{
		sprintf(retMessage, "��ҽ�ɫ��־:��ȡ����:<����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character,BeginTime, EndTime);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "��ҽ�ɫ��־:��ȡ����");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		strSql.Format("Select '%s' AS XDJG_Character, '%s' AS XDJG_Account, \
							  create_time AS XDJG_LogTime, task_id AS XDJG_Task_ID, \
							  task_id AS XDJG_TempTaskId, task_type AS XDJG_TaskType\
						 from %s.ASSIGN_TASK \
						where create_time between '%s' and '%s' and character_name = '%s'", \
							  Character,Account,LogDB,BeginTime,EndTime,Character);
		
#ifdef _DEBUG
		printf(strSql, LogDB, LogDB);
#endif
		
		DBTFLV = GameDBQuery("XDJG",realServerIP,13, iIndex, iPageSize, strSql.GetBuffer(0), LogDB, LogDB);

		for (pDBTFLV = DBTFLV.begin(); pDBTFLV != DBTFLV.end(); pDBTFLV++)
		{
			if(pDBTFLV->m_tagName == CEnumCore::TagName::XDJG_TempTaskId)
			{
				string strTaskName = GetTaskName(atoi((char*)pDBTFLV->lpdata));
				*pDBTFLV->lpdata = NULL;
				pDBTFLV->m_tagName = CEnumCore::TagName::XDJG_Task_Name;
				pDBTFLV->m_tvlength = strTaskName.length();
				memcpy((char*)&pDBTFLV->lpdata, strTaskName.c_str(), strTaskName.length());
			}
		}
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "��ҽ�ɫ��־:��ȡ����");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	return DBTFLV;	
}

string CXDJGInfo::GetTaskName(int TaskId)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	char strConn[512];
	string szTaskName = "";
	vector<CGlobalStruct::DataRecord> datarecord;
	
	try
	{
		logFile.WriteLog("<�������>",operName,"ȡ��������");//��LOG
		
		//ȡ��������
		strSql.Format("select TaskName from XDJG_TaskInfo where TaskId = %d",TaskId);
		
		m_SqlHelper.SqlHelperMain("XDJG", &datarecord,"127.0.0.1",strSql.GetBuffer(0),4,0,0);
		
		if(datarecord.empty())
			return szTaskName;
		
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("TaskName", m_DataRecord.recordfieldname))
			{
				szTaskName = m_DataRecord.recorddata;
				break;
			}
			else
				continue;
		}
	}
	catch(...)
	{
		return szTaskName;
	}
	
	return szTaskName;
}

//��ҽ�ɫ��־:�������
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_DB_CompleteTask_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	char realServerIP[40];
	vector<CGlobalStruct::TFLV>::iterator pDBTFLV = NULL;
	CString strItemID = "";
	vector<st_ItemInfo>::iterator pItemInfo = NULL;
	char retMessage[255];
	ZeroMemory(retMessage,255);
	Vec_ItemInfo VecItemInfo;
	
	try
	{
		sprintf(retMessage, "��ҽ�ɫ��־:�������:<����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character,BeginTime, EndTime);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "��ҽ�ɫ��־:�������");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}

		strSql.Format("Select '%s' AS XDJG_Character, '%s' AS XDJG_Account, \
							  create_time AS XDJG_LogTime, task_id AS XDJG_Task_ID, \
							  task_id AS XDJG_TempTaskId, task_type AS XDJG_TaskType, item_id_gain_1 AS XDJG_TempItemId1, \
							  item_num_gain_1 AS XDJG_ItemNum1,item_id_gain_1 AS XDJG_ItemID1,item_id_gain_2 AS XDJG_TempItemId2, \
							  item_num_gain_2 AS XDJG_ItemNum2,item_id_gain_2 AS XDJG_ItemID2,item_id_gain_3 AS XDJG_TempItemId3, \
							  item_num_gain_3 AS XDJG_ItemNum3,item_id_gain_3 AS XDJG_ItemID3,item_id_throw_1 AS XDJG_TempItemThrowId1, \
							  item_num_throw_1 AS XDJG_ItemThrowNum1,item_id_throw_1 AS XDJG_ItemThrowID1,item_id_throw_2 AS XDJG_TempItemThrowId2, \
							  item_num_throw_2 AS XDJG_ItemThrowNum2,item_id_throw_2 AS XDJG_ItemThrowID2,item_id_throw_3 AS XDJG_TempItemThrowId3, \
							  item_num_throw_3 AS XDJG_ItemThrowNum3,item_id_throw_3 AS XDJG_ItemThrowID3,money AS XDJG_Money \
						 from %s.COMMIT_TASK \
						where create_time between '%s' and '%s' and character_name = '%s'", \
							  Character,Account,LogDB,BeginTime,EndTime,Character);
		
#ifdef _DEBUG
		printf(strSql, LogDB, LogDB);
#endif
		
		DBTFLV = GameDBQuery("XDJG",realServerIP,13, iIndex, iPageSize, strSql.GetBuffer(0), LogDB, LogDB);

		if(DBTFLV.empty())
		{
			return DBTFLV;
		}
		
		//ȡ��Ʒ����
		GetItemInfo(VecItemInfo);

		//������id��ɵ�������
		for(pDBTFLV = DBTFLV.begin(); pDBTFLV != DBTFLV.end(); pDBTFLV++)
		{
			if(pDBTFLV->m_tagName == CEnumCore::TagName::XDJG_TempItemId1 ||
			   pDBTFLV->m_tagName == CEnumCore::TagName::XDJG_TempItemId2 ||
			   pDBTFLV->m_tagName == CEnumCore::TagName::XDJG_TempItemId3 ||
			   pDBTFLV->m_tagName == CEnumCore::TagName::XDJG_TempItemThrowId1 ||
			   pDBTFLV->m_tagName == CEnumCore::TagName::XDJG_TempItemThrowId2 ||
			   pDBTFLV->m_tagName == CEnumCore::TagName::XDJG_TempItemThrowId3)
			{
				for(pItemInfo = VecItemInfo.begin(); pItemInfo != VecItemInfo.end(); pItemInfo++)
				{
					if(pItemInfo->iItemId == atoi((char*)pDBTFLV->lpdata))
					{
						pDBTFLV->m_tagName = CEnumCore::TagName::XDJG_ItemName;
						pDBTFLV->m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
						*pDBTFLV->lpdata = NULL;
						pDBTFLV->m_tvlength = pItemInfo->strItemName.size();
						memcpy(&pDBTFLV->lpdata, pItemInfo->strItemName.c_str(), pItemInfo->strItemName.size());
						break;
					}
					else
						continue;
				}
				
				if(pDBTFLV->m_tagName != CEnumCore::TagName::XDJG_ItemName)
				{
					pDBTFLV->m_tagName = CEnumCore::TagName::XDJG_ItemName;
					pDBTFLV->m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("")+1;
					memcpy(&pDBTFLV->lpdata, "", strlen("")+1);
				}
			}
			else if(pDBTFLV->m_tagName == CEnumCore::TagName::XDJG_TempTaskId)
			{
				string strTaskName = GetTaskName(atoi((char*)pDBTFLV->lpdata));
				*pDBTFLV->lpdata = NULL;
				pDBTFLV->m_tagName = CEnumCore::TagName::XDJG_Task_Name;
				pDBTFLV->m_tvlength = strTaskName.length();
				memcpy((char*)&pDBTFLV->lpdata, strTaskName.c_str(), strTaskName.length());
			}
			else
				continue;
		}

	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "��ҽ�ɫ��־:�������");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	return DBTFLV;
}

//��ҽ�ɫ��־:������ѯ
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_DB_Copy_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	char realServerIP[40];
	char retMessage[255];
	ZeroMemory(retMessage,255);
	
	try
	{
		sprintf(retMessage, "��ҽ�ɫ��־:������ѯ:<����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character,BeginTime, EndTime);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "��ҽ�ɫ��־:������ѯ");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		strSql.Format("Select '%s' AS XDJG_Character, '%s' AS XDJG_Account, \
							  create_time AS XDJG_LogTime, skill_id AS XDJG_SkillId, \
							  skill_level AS XDJG_Level\
						 from %s.TASK_COMMIT \
						where create_time between '%s' and '%s' and character_name = '%s'", \
							  Character,Account,LogDB,BeginTime,EndTime,Character);
		
#ifdef _DEBUG
		printf(strSql, LogDB, LogDB);
#endif
		
		DBTFLV = GameDBQuery("XDJG",realServerIP,13, iIndex, iPageSize, strSql.GetBuffer(0), LogDB, LogDB);
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "��ҽ�ɫ��־:��ɫ����ѧϰ");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	return DBTFLV;
}

//��ѯ��
//�����Ʒ��Ϣ:������Ʒ��Ϣ
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_DB_ItemBody_Query(int userByID, char *ServerIP, char *Account, char *Character, int iIndex, int iPageSize)
{
	CGlobalStruct::TFLV m_tflv;
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	CString szData = "";
	string strTime = "";
	char realServerIP[40];
	int iCount = 0;
	vector<CGlobalStruct::DataRecord> datarecord;
	vector<st_ItemInfo>::iterator pItemInfo = NULL;
	vector<_Res> Vec_Res;
	char retMessage[255];
	ZeroMemory(retMessage,255);
	Vec_ItemInfo VecItemInfo;
	
	try
	{
		sprintf(retMessage, "��ҽ�ɫ��־:������Ʒ��Ϣ:<����>%s, <�ʺ�>%s, <��ɫ��>%s", ServerIP, Account, Character);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,3))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "�����Ʒ��Ϣ:������Ʒ��Ϣ");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}

		//ȡ����������Ʒ�����ֶ�
		strSql.Format("select items AS XDJG_Items \
						 from %s.character \
						where name = '%s' \
						  and account_name = '%s'",GameDB, Character, Account);

#ifdef _DEBUG
		printf(strSql, GameDB, GameDB);
#endif

		m_SqlHelper.SqlHelperMain("XDJG", &datarecord,realServerIP,strSql.GetBuffer(0),12,0,0);
		
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("XDJG_Items", m_DataRecord.recordfieldname))
			{
				memcpy(&m_StructInfo.m_sResCont , m_DataRecord.recorddata, sizeof(m_StructInfo.m_sResCont));//��Ʒ����
				break;
			}
			else
				continue;
		}

		//ȡ��ǰ���ϵĵ���
		for(int i = 0; i < m_StructInfo.m_sResCont.limit_character_equip_num; i++)
		{
			if(m_StructInfo.m_sResCont.equip_list[i].itemId != 0)
			{
				Vec_Res.push_back(m_StructInfo.m_sResCont.equip_list[i]);
			}
			else
				continue;
		}

		if(Vec_Res.size() == 0)
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�������û����Ʒ", "������Ʒ��Ϣ��ѯ");
			return DBTFLV;
		}

		//ȡ��Ʒ����
		GetItemInfo(VecItemInfo);

		int iPageCount = 0;
		int iLogInfoLen = Vec_Res.size();
		
		if ( iLogInfoLen%iPageSize == 0 )
		{
			iPageCount = iLogInfoLen/iPageSize;
		}
		else
		{
			iPageCount = iLogInfoLen/iPageSize + 1;
		}
		
		for ( iCount = iPageSize*(iIndex-1); iCount < iLogInfoLen && iCount < iPageSize*iIndex; iCount++ )
		{
			//����ʺ�
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Account;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(Account) + 1;
			sprintf((char *)&m_tflv.lpdata,"%s",Account);
			DBTFLV.push_back(m_tflv);

			//��ɫ��
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Character;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(Character) + 1;
			sprintf((char *)&m_tflv.lpdata,"%s",Character);
			DBTFLV.push_back(m_tflv);

			//��ƷID
			szData.Format("%i", Vec_Res[iCount].itemId);
			m_tflv.nIndex = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_ItemID;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//��Ʒ����
			for(pItemInfo = VecItemInfo.begin(); pItemInfo != VecItemInfo.end(); pItemInfo++)
			{
				if(pItemInfo->iItemId == (int)Vec_Res[iCount].itemId)
				{
					m_tflv.nIndex = DBTFLV.size() + 1;
					m_tflv.m_tagName = CEnumCore::TagName::XDJG_ItemName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = pItemInfo->strItemName.size();
					memcpy(&m_tflv.lpdata, pItemInfo->strItemName.c_str(), pItemInfo->strItemName.size());
					DBTFLV.push_back(m_tflv);
					break;
				}
				else
					continue;
			}

			if(m_tflv.m_tagName != CEnumCore::TagName::XDJG_ItemName)
			{
				m_tflv.nIndex = DBTFLV.size() + 1;
				m_tflv.m_tagName = CEnumCore::TagName::XDJG_ItemName;
				m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength = strlen("")+1;
				sprintf((char*)&m_tflv.lpdata, "%s", "");
				DBTFLV.push_back(m_tflv);
			}

			//�Ƿ��
			m_tflv.nIndex = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_IsBind;
			switch(Vec_Res[iCount].property.safeFlags & 0xFF)
			{
			case SAFEFLAG:
				m_tflv.m_tvlength = strlen("��") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "��");
				break;
			default:
				m_tflv.m_tvlength = strlen("��") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "��");
				break;
			}
			DBTFLV.push_back(m_tflv);

			//�Ƿ�Ϊ����װ��
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_QualityEquip;
			if((Vec_Res[iCount].property.itemFlags & ITEMFLAG) == ITEMFLAG)
			{
				if((Vec_Res[iCount].property.itemFlags & SAFEFLAG) == SAFEFLAG)
				{
					m_tflv.m_tvlength = strlen("��") + 1;
					sprintf((char*)&m_tflv.lpdata, "%s", "��");
				}
				else
				{
					m_tflv.m_tvlength = strlen("��") + 1;
					sprintf((char*)&m_tflv.lpdata, "%s", "��");
				}

			}
			else
			{
				m_tflv.m_tvlength = strlen("δ����") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "δ����");
			}
			DBTFLV.push_back(m_tflv);

			//��������
			szData.Format("%i", Vec_Res[iCount].property.usingTime);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_SuperpositionNum;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//����ʱ��
			strTime = GetRealTime(Vec_Res[iCount].property.createTime);
//			szData.Format("%i", Vec_Res[iCount].property.createTime);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_CreateTime;
			m_tflv.m_tvlength = strTime.length();
			sprintf((char*)&m_tflv.lpdata, "%s", strTime.c_str());
			DBTFLV.push_back(m_tflv);

			//�;�ֵ
			szData.Format("%i", Vec_Res[iCount].property.abrasable);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_DurableValue;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//��������
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_IncreaseType;
			switch(Vec_Res[iCount].property.strengthenType)
			{
			case 0:
				m_tflv.m_tvlength = strlen("û��") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "û��");
				break;
			case 1:
				m_tflv.m_tvlength = strlen("����") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "����");
				break;
			case 2:
				m_tflv.m_tvlength = strlen("����") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "����");
				break;
			case 3:
				m_tflv.m_tvlength = strlen("����") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "����");
				break;
			case 4:
				m_tflv.m_tvlength = strlen("����") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "����");
				break;
			case 5:
				m_tflv.m_tvlength = strlen("����") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "����");
				break;
			default:
				m_tflv.m_tvlength = strlen("δ֪") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "δ֪");
				break;
			}
			DBTFLV.push_back(m_tflv);

			//���һ����������ֵ
			szData.Format("%i", Vec_Res[iCount].property.strengthenLastTimeQuantity);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_IncreaseEndValue;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//��������
			szData.Format("%i", Vec_Res[iCount].property.strengthenEffect);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_IncreaseNum;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//װ����װID
			szData.Format("%i", Vec_Res[iCount].property.compositingEffect);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_AssemblyID;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);
			
			//��������ֵ
			szData.Format("%i", Vec_Res[iCount].property.strengthenQuantity);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_IncreaseValue;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//װ����Ƕ��ʯ��ID1
			szData.Format("%i", Vec_Res[iCount].property.equipSlotEffect1);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_EquipSlotID1;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);
			
			//װ����Ƕ��ʯ��ID2
			szData.Format("%i", Vec_Res[iCount].property.equipSlotEffect2);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_EquipSlotID2;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//װ����Ƕ��ʯ��ID3
			szData.Format("%i", Vec_Res[iCount].property.equipSlotEffect3);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_EquipSlotID3;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);
			
			//����Ϊ������(�Ǽ�)
			szData.Format("%i", (Vec_Res[iCount].property.equipSlotEffect3 & STARLEVEL));
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_StarsLevel;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//�������ֵ
			szData.Format("%i", (Vec_Res[iCount].property.safeFlags & 0x0000FFFF));
			m_tflv.nIndex = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_RandomProperty;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//PageCount
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PageCount;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((char *)&m_tflv.lpdata,"%d", iPageCount);
			DBTFLV.push_back(m_tflv);
			
		}
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "�����Ʒ��Ϣ:������Ʒ��Ϣ");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	return DBTFLV;
}

//��ȡ������ʱ��
string CXDJGInfo::GetRealTime(int time)
{
	CString szSql = "";
	string szData = "";
	vector<CGlobalStruct::DataRecord> datarecord;
	
	try
	{
		szSql.Format("select DATEADD(s,%d,'19700101') AS XDJG_CreateTime", time);
		
		m_SqlHelper.SqlHelperMain("XDJG", &datarecord,"127.0.0.1",szSql.GetBuffer(0),4,0,0);
		
		if(datarecord.empty())
			return "";
		
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("XDJG_CreateTime", m_DataRecord.recordfieldname))
			{
				szData = m_DataRecord.recorddata;
				break;
			}
			else
				continue;
		}	
	}
	catch (...)
	{
		return "error";
	}
	
	return szData;
}

//�����Ʒ��Ϣ:������Ʒ��Ϣ
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_DB_Itembackpack_Query(int userByID, char *ServerIP, char *Account, char *Character, int iIndex, int iPageSize)
{
	CGlobalStruct::TFLV m_tflv;
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	CString szData = "";
	char realServerIP[40];
	int iCount = 0;
	vector<CGlobalStruct::DataRecord> datarecord;
	vector<st_ItemInfo>::iterator pItemInfo = NULL;
	vector<_Res> Vec_Res;
	string strTime = "";
	char retMessage[255];
	ZeroMemory(retMessage,255);
	Vec_ItemInfo VecItemInfo;
	
	try
	{
		sprintf(retMessage, "��ҽ�ɫ��־:������Ʒ��Ϣ:<����>%s, <�ʺ�>%s, <��ɫ��>%s", ServerIP, Account, Character);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,3))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "�����Ʒ��Ϣ:������Ʒ��Ϣ");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}

		//ȡ����������Ʒ�����ֶ�
		strSql.Format("select items AS XDJG_Items \
						 from %s.character \
						where name = '%s' \
						  and account_name = '%s'",GameDB, Character, Account);

#ifdef _DEBUG
		printf(strSql, GameDB, GameDB);
#endif

		m_SqlHelper.SqlHelperMain("XDJG", &datarecord,realServerIP,strSql.GetBuffer(0),12,0,0);
		
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("XDJG_Items", m_DataRecord.recordfieldname))
			{
				memcpy(&m_StructInfo.m_sResCont , m_DataRecord.recorddata, sizeof(m_StructInfo.m_sResCont));//��Ʒ����
				break;
			}
			else
				continue;
		}

		//ȡ��ǰ���ϵĵ���
		for(int i = 0; i < m_StructInfo.m_sResCont.limit_character_res_num; i++)
		{
			if(m_StructInfo.m_sResCont.res_list[i].itemId != 0)
				Vec_Res.push_back(m_StructInfo.m_sResCont.res_list[i]);
			else
				continue;
		}
		
		if(Vec_Res.size() == 0)
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ұ���û����Ʒ", "������Ʒ��Ϣ��ѯ");
			return DBTFLV;
		}
		
		//ȡ��Ʒ����
		GetItemInfo(VecItemInfo);

		int iPageCount = 0;
		int iLogInfoLen = Vec_Res.size();
		
		if ( iLogInfoLen%iPageSize == 0 )
		{
			iPageCount = iLogInfoLen/iPageSize;
		}
		else
		{
			iPageCount = iLogInfoLen/iPageSize + 1;
		}
		
		for ( iCount = iPageSize*(iIndex-1); iCount < iLogInfoLen && iCount < iPageSize*iIndex; iCount++ )
		{
			//����ʺ�
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Account;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(Account) + 1;
			sprintf((char *)&m_tflv.lpdata,"%s",Account);
			DBTFLV.push_back(m_tflv);

			//��ɫ��
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Character;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(Character) + 1;
			sprintf((char *)&m_tflv.lpdata,"%s",Character);
			DBTFLV.push_back(m_tflv);

			//��ƷID
			szData.Format("%i", Vec_Res[iCount].itemId);
			m_tflv.nIndex = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_ItemID;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//��Ʒ����
			for(pItemInfo = VecItemInfo.begin(); pItemInfo != VecItemInfo.end(); pItemInfo++)
			{
				if(pItemInfo->iItemId == (int)Vec_Res[iCount].itemId)
				{
					m_tflv.nIndex = DBTFLV.size() + 1;
					m_tflv.m_tagName = CEnumCore::TagName::XDJG_ItemName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = pItemInfo->strItemName.size();
					memcpy(&m_tflv.lpdata, pItemInfo->strItemName.c_str(), pItemInfo->strItemName.size());
					DBTFLV.push_back(m_tflv);
					break;
				}
				else
					continue;
			}

			if(m_tflv.m_tagName != CEnumCore::TagName::XDJG_ItemName)
			{
				m_tflv.nIndex = DBTFLV.size() + 1;
				m_tflv.m_tagName = CEnumCore::TagName::XDJG_ItemName;
				m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength = strlen("")+1;
				sprintf((char*)&m_tflv.lpdata, "%s", "");
				DBTFLV.push_back(m_tflv);
			}

			//�Ƿ��
			m_tflv.nIndex = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_IsBind;
			switch(Vec_Res[iCount].property.safeFlags & 0xFF)
			{
			case SAFEFLAG:
				m_tflv.m_tvlength = strlen("��") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "��");
				break;
			default:
				m_tflv.m_tvlength = strlen("��") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "��");
				break;
			}
			DBTFLV.push_back(m_tflv);

			//�Ƿ�Ϊ����װ��
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_QualityEquip;
			if((Vec_Res[iCount].property.itemFlags & ITEMFLAG) == ITEMFLAG)
			{
				if((Vec_Res[iCount].property.itemFlags & SAFEFLAG) == SAFEFLAG)
				{
					m_tflv.m_tvlength = strlen("��") + 1;
					sprintf((char*)&m_tflv.lpdata, "%s", "��");
				}
				else
				{
					m_tflv.m_tvlength = strlen("��") + 1;
					sprintf((char*)&m_tflv.lpdata, "%s", "��");
				}

			}
			else
			{
				m_tflv.m_tvlength = strlen("δ����") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "δ����");
			}
			DBTFLV.push_back(m_tflv);

			//��������
			szData.Format("%i", Vec_Res[iCount].property.usingTime);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_SuperpositionNum;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//����ʱ��
			strTime = GetRealTime(Vec_Res[iCount].property.createTime);
//			szData.Format("%i", Vec_Res[iCount].property.createTime);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_CreateTime;
			m_tflv.m_tvlength = strTime.length();
			sprintf((char*)&m_tflv.lpdata, "%s", strTime.c_str());
			DBTFLV.push_back(m_tflv);

			//�;�ֵ
			szData.Format("%i", Vec_Res[iCount].property.abrasable);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_DurableValue;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//��������
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_IncreaseType;
			switch(Vec_Res[iCount].property.strengthenType)
			{
			case 0:
				m_tflv.m_tvlength = strlen("û��") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "û��");
				break;
			case 1:
				m_tflv.m_tvlength = strlen("����") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "����");
				break;
			case 2:
				m_tflv.m_tvlength = strlen("����") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "����");
				break;
			case 3:
				m_tflv.m_tvlength = strlen("����") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "����");
				break;
			case 4:
				m_tflv.m_tvlength = strlen("����") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "����");
				break;
			case 5:
				m_tflv.m_tvlength = strlen("����") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "����");
				break;
			default:
				m_tflv.m_tvlength = strlen("δ֪") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "δ֪");
				break;
			}
			DBTFLV.push_back(m_tflv);

			//���һ����������ֵ
			szData.Format("%i", Vec_Res[iCount].property.strengthenLastTimeQuantity);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_IncreaseEndValue;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//��������
			szData.Format("%i", Vec_Res[iCount].property.strengthenEffect);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_IncreaseNum;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//װ����װID
			szData.Format("%i", Vec_Res[iCount].property.compositingEffect);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_AssemblyID;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);
			
			//��������ֵ
			szData.Format("%i", Vec_Res[iCount].property.strengthenQuantity);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_IncreaseValue;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//װ����Ƕ��ʯ��ID1
			szData.Format("%i", Vec_Res[iCount].property.equipSlotEffect1);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_EquipSlotID1;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);
			
			//װ����Ƕ��ʯ��ID2
			szData.Format("%i", Vec_Res[iCount].property.equipSlotEffect2);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_EquipSlotID2;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//װ����Ƕ��ʯ��ID3
			szData.Format("%i", Vec_Res[iCount].property.equipSlotEffect3);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_EquipSlotID3;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);
			
			//����Ϊ������(�Ǽ�)
			szData.Format("%i", (Vec_Res[iCount].property.equipSlotEffect3 & STARLEVEL));
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_StarsLevel;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//�������ֵ
			szData.Format("%i", (Vec_Res[iCount].property.safeFlags & 0x0000FFFF));
			m_tflv.nIndex = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_RandomProperty;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//PageCount
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PageCount;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((char *)&m_tflv.lpdata,"%d", iPageCount);
			DBTFLV.push_back(m_tflv);
		}
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "�����Ʒ��Ϣ:������Ʒ��Ϣ��ѯ");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	return DBTFLV;
}

//�����Ʒ��Ϣ:�ֿ���Ʒ��Ϣ
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_DB_ItemStorage_Query(int userByID, char *ServerIP, char *Account, char *Character, int iIndex, int iPageSize)
{
	CGlobalStruct::TFLV m_tflv;
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	CString szData = "";
	char realServerIP[40];
	int iCount = 0;
	vector<CGlobalStruct::DataRecord> datarecord;
	vector<st_ItemInfo>::iterator pItemInfo = NULL;
	vector<_Res> Vec_Res;
	string strTime = "";
	char retMessage[255];
	ZeroMemory(retMessage,255);
	Vec_ItemInfo VecItemInfo;
	
	try
	{
		sprintf(retMessage, "��ҽ�ɫ��־:�ֿ���Ʒ��Ϣ��ѯ:<����>%s, <�ʺ�>%s, <��ɫ��>%s", ServerIP, Account, Character);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,3))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "�����Ʒ��Ϣ:�ֿ���Ʒ��Ϣ");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		strSql.Format("Select items AS XDJG_Items from %s.warehouse \
							  where charindex = (select guid from %s.character where name = '%s')", \
							  GameDB,GameDB,Character);
		
#ifdef _DEBUG
		printf(strSql, GameDB, GameDB);
#endif
		
		m_SqlHelper.SqlHelperMain("XDJG", &datarecord,realServerIP,strSql.GetBuffer(0),12,0,0);
		
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("XDJG_Items", m_DataRecord.recordfieldname))
			{
				memcpy(&m_StructInfo.m_sWareHouse , m_DataRecord.recorddata, sizeof(m_StructInfo.m_sWareHouse));//��Ʒ����
				break;
			}
			else
				continue;
		}

		//ȡ��ǰ���ϵĵ���
		for(int i = 0; i < limit_per_warehouse_res_num; i++)
		{
			if(m_StructInfo.m_sWareHouse.res_list[i].itemId != 0)
				Vec_Res.push_back(m_StructInfo.m_sWareHouse.res_list[i]);
			else
				continue;
		}
		
		if(Vec_Res.size() == 0)
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��Ҳֿ�û����Ʒ", "�ֿ���Ʒ��Ϣ��ѯ");
			return DBTFLV;
		}
		
		//ȡ��Ʒ����
		GetItemInfo(VecItemInfo);

		int iPageCount = 0;
		int iLogInfoLen = Vec_Res.size();
		
		if ( iLogInfoLen%iPageSize == 0 )
		{
			iPageCount = iLogInfoLen/iPageSize;
		}
		else
		{
			iPageCount = iLogInfoLen/iPageSize + 1;
		}
		
		for ( iCount = iPageSize*(iIndex-1); iCount < iLogInfoLen && iCount < iPageSize*iIndex; iCount++ )
		{
			//����ʺ�
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Account;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(Account) + 1;
			sprintf((char *)&m_tflv.lpdata,"%s",Account);
			DBTFLV.push_back(m_tflv);

			//��ɫ��
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Character;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(Character) + 1;
			sprintf((char *)&m_tflv.lpdata,"%s",Character);
			DBTFLV.push_back(m_tflv);

			//��ƷID
			szData.Format("%i", Vec_Res[iCount].itemId);
			m_tflv.nIndex = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_ItemID;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//��Ʒ����
			for(pItemInfo = VecItemInfo.begin(); pItemInfo != VecItemInfo.end(); pItemInfo++)
			{
				if(pItemInfo->iItemId == (int)Vec_Res[iCount].itemId)
				{
					m_tflv.nIndex = DBTFLV.size() + 1;
					m_tflv.m_tagName = CEnumCore::TagName::XDJG_ItemName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = pItemInfo->strItemName.size();
					memcpy(&m_tflv.lpdata, pItemInfo->strItemName.c_str(), pItemInfo->strItemName.size());
					DBTFLV.push_back(m_tflv);
					break;
				}
				else
					continue;
			}

			if(m_tflv.m_tagName != CEnumCore::TagName::XDJG_ItemName)
			{
				m_tflv.nIndex = DBTFLV.size() + 1;
				m_tflv.m_tagName = CEnumCore::TagName::XDJG_ItemName;
				m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength = strlen("")+1;
				sprintf((char*)&m_tflv.lpdata, "%s", "");
				DBTFLV.push_back(m_tflv);
			}

			//�Ƿ��
			m_tflv.nIndex = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_IsBind;
			switch(Vec_Res[iCount].property.safeFlags & 0xFF)
			{
			case SAFEFLAG:
				m_tflv.m_tvlength = strlen("��") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "��");
				break;
			default:
				m_tflv.m_tvlength = strlen("��") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "��");
				break;
			}
			DBTFLV.push_back(m_tflv);

			//�Ƿ�Ϊ����װ��
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_QualityEquip;
			if((Vec_Res[iCount].property.itemFlags & ITEMFLAG) == ITEMFLAG)
			{
				if((Vec_Res[iCount].property.itemFlags & SAFEFLAG) == SAFEFLAG)
				{
					m_tflv.m_tvlength = strlen("��") + 1;
					sprintf((char*)&m_tflv.lpdata, "%s", "��");
				}
				else
				{
					m_tflv.m_tvlength = strlen("��") + 1;
					sprintf((char*)&m_tflv.lpdata, "%s", "��");
				}

			}
			else
			{
				m_tflv.m_tvlength = strlen("δ����") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "δ����");
			}
			DBTFLV.push_back(m_tflv);

			//��������
			szData.Format("%i", Vec_Res[iCount].property.usingTime);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_SuperpositionNum;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//����ʱ��
			strTime = GetRealTime(Vec_Res[iCount].property.createTime);
//			szData.Format("%i", Vec_Res[iCount].property.createTime);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_CreateTime;
			m_tflv.m_tvlength = strTime.length();
			sprintf((char*)&m_tflv.lpdata, "%s", strTime.c_str());
			DBTFLV.push_back(m_tflv);

			//�;�ֵ
			szData.Format("%i", Vec_Res[iCount].property.abrasable);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_DurableValue;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//��������
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_IncreaseType;
			switch(Vec_Res[iCount].property.strengthenType)
			{
			case 0:
				m_tflv.m_tvlength = strlen("û��") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "û��");
				break;
			case 1:
				m_tflv.m_tvlength = strlen("����") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "����");
				break;
			case 2:
				m_tflv.m_tvlength = strlen("����") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "����");
				break;
			case 3:
				m_tflv.m_tvlength = strlen("����") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "����");
				break;
			case 4:
				m_tflv.m_tvlength = strlen("����") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "����");
				break;
			case 5:
				m_tflv.m_tvlength = strlen("����") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "����");
				break;
			default:
				m_tflv.m_tvlength = strlen("δ֪") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "δ֪");
				break;
			}
			DBTFLV.push_back(m_tflv);

			//���һ����������ֵ
			szData.Format("%i", Vec_Res[iCount].property.strengthenLastTimeQuantity);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_IncreaseEndValue;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//��������
			szData.Format("%i", Vec_Res[iCount].property.strengthenEffect);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_IncreaseNum;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//װ����װID
			szData.Format("%i", Vec_Res[iCount].property.compositingEffect);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_AssemblyID;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);
			
			//��������ֵ
			szData.Format("%i", Vec_Res[iCount].property.strengthenQuantity);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_IncreaseValue;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//װ����Ƕ��ʯ��ID1
			szData.Format("%i", Vec_Res[iCount].property.equipSlotEffect1);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_EquipSlotID1;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);
			
			//װ����Ƕ��ʯ��ID2
			szData.Format("%i", Vec_Res[iCount].property.equipSlotEffect2);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_EquipSlotID2;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//װ����Ƕ��ʯ��ID3
			szData.Format("%i", Vec_Res[iCount].property.equipSlotEffect3);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_EquipSlotID3;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);
			
			//����Ϊ������(�Ǽ�)
			szData.Format("%i", (Vec_Res[iCount].property.equipSlotEffect3 & STARLEVEL));
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_StarsLevel;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//�������ֵ
			szData.Format("%i", (Vec_Res[iCount].property.safeFlags & 0x0000FFFF));
			m_tflv.nIndex = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_RandomProperty;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//PageCount
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PageCount;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((char *)&m_tflv.lpdata,"%d", iPageCount);
			DBTFLV.push_back(m_tflv);
		}
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "�����Ʒ��Ϣ:�ֿ���Ʒ��Ϣ��ѯ");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	return DBTFLV;
}

//�����Ʒ��Ϣ:����ֿ���Ϣ
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_DB_GuildStorage_Query(int userByID, char *ServerIP, char *Account, char *Character, int iIndex, int iPageSize)
{
	CGlobalStruct::TFLV m_tflv;
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	CString szData = "";
	char realServerIP[40];
	int iCount = 0;
	vector<CGlobalStruct::DataRecord> datarecord;
	vector<st_ItemInfo>::iterator pItemInfo = NULL;
	vector<_Res> Vec_Res;
	string strTime = "";
	char retMessage[255];
	ZeroMemory(retMessage,255);
	Vec_ItemInfo VecItemInfo;
	
	try
	{
		sprintf(retMessage, "��ҽ�ɫ��־:����ֿ���Ϣ:<����>%s, <�ʺ�>%s, <��ɫ��>%s", ServerIP, Account, Character);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,3))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "�����Ʒ��Ϣ:����ֿ���Ϣ");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		strSql.Format("Select items AS XDJG_Items from %s.family_storage \
							  where familyindex = (select familyid from %s.family_member where name = '%s')" ,\
							  GameDB,GameDB,Character);
		
#ifdef _DEBUG
		printf(strSql, GameDB, GameDB);
#endif
		
		m_SqlHelper.SqlHelperMain("XDJG", &datarecord,realServerIP,strSql.GetBuffer(0),12,0,0);
		
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("XDJG_Items", m_DataRecord.recordfieldname))
			{
				memcpy(&m_StructInfo.m_sGuildWareHouse, m_DataRecord.recorddata, sizeof(m_StructInfo.m_sGuildWareHouse));//��Ʒ����
				break;
			}
			else
				continue;
		}

		//ȡ��ǰ���ϵĵ���
		for(int i = 0; i < m_StructInfo.m_sGuildWareHouse.limit_guild_ware_house_size_per_level; i++)
		{
			if(m_StructInfo.m_sGuildWareHouse.items.res_list[i].itemId != 0)
				Vec_Res.push_back(m_StructInfo.m_sGuildWareHouse.items.res_list[i]);
			else
				continue;
		}
		
		if(Vec_Res.size() == 0)
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ֿ�û����Ʒ", "����ֿ���Ϣ��ѯ");
			return DBTFLV;
		}
		
		//ȡ��Ʒ����
		GetItemInfo(VecItemInfo);

		int iPageCount = 0;
		int iLogInfoLen = Vec_Res.size();
		
		if ( iLogInfoLen%iPageSize == 0 )
		{
			iPageCount = iLogInfoLen/iPageSize;
		}
		else
		{
			iPageCount = iLogInfoLen/iPageSize + 1;
		}
		
		for ( iCount = iPageSize*(iIndex-1); iCount < iLogInfoLen && iCount < iPageSize*iIndex; iCount++ )
		{
			//����ʺ�
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Account;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(Account) + 1;
			sprintf((char *)&m_tflv.lpdata,"%s",Account);
			DBTFLV.push_back(m_tflv);

			//��ɫ��
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Character;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(Character) + 1;
			sprintf((char *)&m_tflv.lpdata,"%s",Character);
			DBTFLV.push_back(m_tflv);

			//��ƷID
			szData.Format("%i", Vec_Res[iCount].itemId);
			m_tflv.nIndex = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_ItemID;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//��Ʒ����
			for(pItemInfo = VecItemInfo.begin(); pItemInfo != VecItemInfo.end(); pItemInfo++)
			{
				if(pItemInfo->iItemId == (int)Vec_Res[iCount].itemId)
				{
					m_tflv.nIndex = DBTFLV.size() + 1;
					m_tflv.m_tagName = CEnumCore::TagName::XDJG_ItemName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = pItemInfo->strItemName.size();
					memcpy(&m_tflv.lpdata, pItemInfo->strItemName.c_str(), pItemInfo->strItemName.size());
					DBTFLV.push_back(m_tflv);
					break;
				}
				else
					continue;
			}

			if(m_tflv.m_tagName != CEnumCore::TagName::XDJG_ItemName)
			{
				m_tflv.nIndex = DBTFLV.size() + 1;
				m_tflv.m_tagName = CEnumCore::TagName::XDJG_ItemName;
				m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength = strlen("")+1;
				sprintf((char*)&m_tflv.lpdata, "%s", "");
				DBTFLV.push_back(m_tflv);
			}

			//�Ƿ��
			m_tflv.nIndex = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_IsBind;
			switch(Vec_Res[iCount].property.safeFlags & 0xFF)
			{
			case SAFEFLAG:
				m_tflv.m_tvlength = strlen("��") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "��");
				break;
			default:
				m_tflv.m_tvlength = strlen("��") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "��");
				break;
			}
			DBTFLV.push_back(m_tflv);

			//�Ƿ�Ϊ����װ��
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_QualityEquip;
			if((Vec_Res[iCount].property.itemFlags & ITEMFLAG) == ITEMFLAG)
			{
				if((Vec_Res[iCount].property.itemFlags & SAFEFLAG) == SAFEFLAG)
				{
					m_tflv.m_tvlength = strlen("��") + 1;
					sprintf((char*)&m_tflv.lpdata, "%s", "��");
				}
				else
				{
					m_tflv.m_tvlength = strlen("��") + 1;
					sprintf((char*)&m_tflv.lpdata, "%s", "��");
				}

			}
			else
			{
				m_tflv.m_tvlength = strlen("δ����") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "δ����");
			}
			DBTFLV.push_back(m_tflv);

			//��������
			szData.Format("%i", Vec_Res[iCount].property.usingTime);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_SuperpositionNum;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//����ʱ��
			strTime = GetRealTime(Vec_Res[iCount].property.createTime);
//			szData.Format("%i", Vec_Res[iCount].property.createTime);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_CreateTime;
			m_tflv.m_tvlength = strTime.length();
			sprintf((char*)&m_tflv.lpdata, "%s", strTime.c_str());
			DBTFLV.push_back(m_tflv);

			//�;�ֵ
			szData.Format("%i", Vec_Res[iCount].property.abrasable);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_DurableValue;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//��������
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_IncreaseType;
			switch(Vec_Res[iCount].property.strengthenType)
			{
			case 0:
				m_tflv.m_tvlength = strlen("û��") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "û��");
				break;
			case 1:
				m_tflv.m_tvlength = strlen("����") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "����");
				break;
			case 2:
				m_tflv.m_tvlength = strlen("����") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "����");
				break;
			case 3:
				m_tflv.m_tvlength = strlen("����") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "����");
				break;
			case 4:
				m_tflv.m_tvlength = strlen("����") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "����");
				break;
			case 5:
				m_tflv.m_tvlength = strlen("����") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "����");
				break;
			default:
				m_tflv.m_tvlength = strlen("δ֪") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "δ֪");
				break;
			}
			DBTFLV.push_back(m_tflv);

			//���һ����������ֵ
			szData.Format("%i", Vec_Res[iCount].property.strengthenLastTimeQuantity);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_IncreaseEndValue;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//��������
			szData.Format("%i", Vec_Res[iCount].property.strengthenEffect);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_IncreaseNum;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//װ����װID
			szData.Format("%i", Vec_Res[iCount].property.compositingEffect);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_AssemblyID;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);
			
			//��������ֵ
			szData.Format("%i", Vec_Res[iCount].property.strengthenQuantity);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_IncreaseValue;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//װ����Ƕ��ʯ��ID1
			szData.Format("%i", Vec_Res[iCount].property.equipSlotEffect1);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_EquipSlotID1;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);
			
			//װ����Ƕ��ʯ��ID2
			szData.Format("%i", Vec_Res[iCount].property.equipSlotEffect2);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_EquipSlotID2;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//װ����Ƕ��ʯ��ID3
			szData.Format("%i", Vec_Res[iCount].property.equipSlotEffect3);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_EquipSlotID3;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);
			
			//����Ϊ������(�Ǽ�)
			szData.Format("%i", (Vec_Res[iCount].property.equipSlotEffect3 & STARLEVEL));
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_StarsLevel;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//�������ֵ
			szData.Format("%i", (Vec_Res[iCount].property.safeFlags & 0x0000FFFF));
			m_tflv.nIndex = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_RandomProperty;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//PageCount
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PageCount;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((char *)&m_tflv.lpdata,"%d", iPageCount);
			DBTFLV.push_back(m_tflv);
		}
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "�����Ʒ��Ϣ:����ֿ���Ϣ");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	return DBTFLV;
}

//�����Ʒ��Ϣ:��ղֿ���Ϣ
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_DB_KingkongStorage_Query(int userByID, char *ServerIP, char *Account, char *Character, int iIndex, int iPageSize)
{
	CGlobalStruct::TFLV m_tflv;
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	CString strData = "";
	char realServerIP[40];
	int iCount = 0;
	int iNum = 0;
	vector<CGlobalStruct::DataRecord> datarecord;
	vector<_Car> Vec_Car;
	string strTime = "";
	char retMessage[255];
	ZeroMemory(retMessage,255);
	
	try
	{
		sprintf(retMessage, "��ҽ�ɫ��־:��ղֿ���Ϣ:<����>%s, <�ʺ�>%s, <��ɫ��>%s", ServerIP, Account, Character);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,3))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "�����Ʒ��Ϣ:��ղֿ���Ϣ");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		strSql.Format("Select items AS XDJG_Items from %s.garage \
							  where charindex = (select guid from %s.character where name = '%s')" ,\
							  GameDB,GameDB,Character);
		
#ifdef _DEBUG
		printf(strSql, GameDB, GameDB);
#endif
		
		m_SqlHelper.SqlHelperMain("XDJG", &datarecord,realServerIP,strSql.GetBuffer(0),12,0,0);
		
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("XDJG_Items", m_DataRecord.recordfieldname))
			{
				memcpy(&m_StructInfo.m_sGarage, m_DataRecord.recorddata, sizeof(m_StructInfo.m_sGarage));//��Ʒ����
				break;
			}
			else
				continue;
		}

		//ȡ��ǰ���ϵĽ��
		for(int i = 0; i < m_StructInfo.m_sGarage.limit_garage_car_num; i++)
		{
			if(m_StructInfo.m_sGarage.car_list[i].carId != 0)
				Vec_Car.push_back(m_StructInfo.m_sGarage.car_list[i]);
			else
				continue;
		}
		
		if(Vec_Car.size() == 0)
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ղֿ�û����Ʒ", "��ղֿ���Ϣ��ѯ");
			return DBTFLV;
		}
		
		int iPageCount = 0;
		printf("***************%d\n",Vec_Car.size());
		int iLogInfoLen = Vec_Car.size();
		
		if ( iLogInfoLen%iPageSize == 0 )
		{
			iPageCount = iLogInfoLen/iPageSize;
		}
		else
		{
			iPageCount = iLogInfoLen/iPageSize + 1;
		}
		
		for ( iCount = iPageSize*(iIndex-1); iCount < iLogInfoLen && iCount < iPageSize*iIndex; iCount++ )
		{
			//����ʺ�
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Account;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(Account) + 1;
			sprintf((char *)&m_tflv.lpdata,"%s",Account);
			DBTFLV.push_back(m_tflv);

			//��ɫ��
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Character;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(Character) + 1;
			sprintf((char *)&m_tflv.lpdata,"%s",Character);
			DBTFLV.push_back(m_tflv);

			//���ID
			strData.Format("%d", Vec_Car[iCount].carId);
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_CarID;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strData.GetLength()+1;
			sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//ȡ�������
			string strCarName = GetCarName(Vec_Car[iCount].carId);
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_CarName;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strCarName.length();
			sprintf((char *)&m_tflv.lpdata,"%s",strCarName.c_str());
			DBTFLV.push_back(m_tflv);

			//����ʱ��
			strTime = GetRealTime(Vec_Car[iCount].createTime);
//			strData.Format("%u", Vec_Car[iCount].createTime);
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_CreateTime;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strTime.length();
			sprintf((char *)&m_tflv.lpdata,"%s",strTime.c_str());
			DBTFLV.push_back(m_tflv);

			//��ǰCPֵ
			strData.Format("%d", Vec_Car[iCount].cpValue);
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_cpValue;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strData.GetLength()+1;
			sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//�Ƿ�����
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_IsLock;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			switch(Vec_Car[iCount].isLocked)
			{ 
			case 0:
				m_tflv.m_tvlength=strlen("��")+1;
				sprintf((char *)&m_tflv.lpdata,"%s","��");
				break;
			case 1:
				m_tflv.m_tvlength=strlen("��")+1;
				sprintf((char *)&m_tflv.lpdata,"%s","��");
				break;
			default:
				break;
			}
 			DBTFLV.push_back(m_tflv);

			//�����Ƿ���ڵ�7λ��
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_IsBind;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			switch(Vec_Car[iCount].safeFlags & 0xFF)
			{
			case SAFEFLAG:
				m_tflv.m_tvlength=strlen("��")+1;
				sprintf((char *)&m_tflv.lpdata,"%s","��");
				break;
			default:
				m_tflv.m_tvlength=strlen("��")+1;
				sprintf((char *)&m_tflv.lpdata,"%s","��");
				break;
			}
			DBTFLV.push_back(m_tflv);

			//�û�������������
			strData.Format("%d", Vec_Car[iCount].userlockUnlockTime);
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_UnlockTime;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strData.GetLength()+1;
			sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//����ID
			strData.Format("%d", Vec_Car[iCount]._TranFightableID);
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_PropertyId;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strData.GetLength()+1;
			sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//��ǰ����
			strData.Format("%d", Vec_Car[iCount]._Exp);
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Exp;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strData.GetLength()+1;
			sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//��ǰ��ռ���ID
			strData.Format("%d", Vec_Car[iCount]._TranSkillID);
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_SkillID;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strData.GetLength()+1;
			sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//��ǰ���ID
			strData.Format("%d", Vec_Car[iCount]._TranLookID);
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_LookID;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strData.GetLength()+1;
			sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//�츳����ֵ
			strData.Format("%d", Vec_Car[iCount]._TranSkillStrID);
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_SkillStrValue;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strData.GetLength()+1;
			sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//ʱ��оƬ�������
			strData.Format("%d", Vec_Car[iCount]._TimePlugNum);
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_TimePlugNum;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strData.GetLength()+1;
			sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//�ȼ�
			strData.Format("%d", Vec_Car[iCount]._Level);
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Level;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strData.GetLength()+1;
			sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			for(iNum = 0; iNum < 2; iNum++)
			{
				switch (iNum)
				{
				case 0:
					//ʱ��оƬIDs1
					strData.Format("%d", Vec_Car[iCount]._TranPluginSlots[iNum]);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_TimePlugSlot1;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=strData.GetLength()+1;
					sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
					DBTFLV.push_back(m_tflv);
					break;
				case 1:
					//ʱ��оƬIDs2
					strData.Format("%d", Vec_Car[iCount]._TranPluginSlots[iNum]);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_TimePlugSlot2;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=strData.GetLength()+1;
					sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
					DBTFLV.push_back(m_tflv);
					break;
				default:
					break;
				}
			}

			//�̱괴��ʱ��
			strData.Format("%d", Vec_Car[iCount]._BrandCreateTime);
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_BrandCreateTime;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strData.GetLength()+1;
			sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//�̱�ID
			strData.Format("%d", Vec_Car[iCount]._BrandID);
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_BrandID;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strData.GetLength()+1;
			sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//PageCount
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PageCount;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((char *)&m_tflv.lpdata,"%d", iPageCount);
			DBTFLV.push_back(m_tflv);
			
		}
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "�����Ʒ��Ϣ:��ղֿ���Ϣ");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	return DBTFLV;
}

//ȡ�������
string CXDJGInfo::GetCarName(int carId)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	char strConn[512];
	string szCarName = "";
	vector<CGlobalStruct::DataRecord> datarecord;
	
	try
	{
		logFile.WriteLog("<�������>",operName,"ȡ�������");//��LOG
		
		//ȡ�������
		strSql.Format("select CarName from XDJG_CarList where CarID = %d",carId);
		
		m_SqlHelper.SqlHelperMain("XDJG", &datarecord,"127.0.0.1",strSql.GetBuffer(0),4,0,0);
		
		if(datarecord.empty())
			return szCarName;
		
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("CarName", m_DataRecord.recordfieldname))
			{
				szCarName = m_DataRecord.recorddata;
				break;
			}
			else
				continue;
		}
	}
	catch(...)
	{
		return szCarName;
	}

	return szCarName;
}

//�ʼ���Ϣ
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_DB_MailInfo_Query(int userByID, char *ServerIP, char *Account, char *Character, int iIndex, int iPageSize)
{
	CGlobalStruct::TFLV m_tflv;
	vector<CGlobalStruct::TFLV> DBTFLV;
	vector<CGlobalStruct::TFLV>::iterator pDBTFLV = NULL;
	CString strSql = "";
	CString szData = "";
	char realServerIP[40];
	vector<CGlobalStruct::DataRecord> datarecord;
	int iItemNum = 0;
	int iTemp = 0;
	bool iTorF = false;
	string strData = "";
	vector<st_ItemInfo>::iterator pItemInfo = NULL;
	CString strtempItemid = "";
	string strItemname = "";
	char *ptempId = NULL;
	char retMessage[255];
	ZeroMemory(retMessage,255);
	Vec_ItemInfo VecItemInfo;

	try
	{
		sprintf(retMessage, "�ʼ���Ϣ��ѯ:<����>%s, <�ʺ�>%s, <��ɫ��>%s", ServerIP, Account, Character);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,3))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "�ʼ���Ϣ");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		strSql.Format("Select '%s' AS XDJG_Account, '%s' AS XDJG_Character, \
							  (SELECT b.account_name FROM %s.character b WHERE b.guid = a.sender_id) AS XDJG_SendAccount, \
							  sender_name AS XDJG_SendCharacter,items AS XDJG_Items,golden AS XDJG_Money,title AS XDJG_Title, \
							  FROM_UNIXTIME(sendoffTime) AS XDJG_ReceiveTime \
						 FROM %s.mail a \
						where receiver_name = '%s'", \
							  Account,Character,GameDB,GameDB,Character);
		
#ifdef _DEBUG
		printf(strSql, GameDB, GameDB);
#endif
		
		m_SqlHelper.SqlHelperMain("XDJG", &datarecord,realServerIP,strSql.GetBuffer(0),12,0,0);
		
		//ȡ��Ʒ����
		GetItemInfo(VecItemInfo);

		vector<CGlobalStruct::DataRecord>::iterator iter;
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			iTorF = false;

			if(!strcmp("XDJG_Account", iter->recordfieldname))
			{
				m_tflv.nIndex  = DBTFLV.size() + 1;
				m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tagName = CEnumCore::TagName::XDJG_Account;
				m_tflv.m_tvlength = strlen(Account)+1;
				sprintf((char*)&m_tflv.lpdata, "%s", Account);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp("XDJG_Character", iter->recordfieldname))
			{
				m_tflv.nIndex  = DBTFLV.size() + 1;
				m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tagName = CEnumCore::TagName::XDJG_Character;
				m_tflv.m_tvlength = strlen(Character)+1;
				sprintf((char*)&m_tflv.lpdata, "%s", Character);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp("XDJG_SendAccount", iter->recordfieldname))
			{
				m_tflv.nIndex  = DBTFLV.size() + 1;
				m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tagName = CEnumCore::TagName::XDJG_SendAccount;
				m_tflv.m_tvlength = strlen(iter->recorddata)+1;
				sprintf((char*)&m_tflv.lpdata, "%s", iter->recorddata);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp("XDJG_SendCharacter", iter->recordfieldname))
			{
				m_tflv.nIndex  = DBTFLV.size() + 1;
				m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tagName = CEnumCore::TagName::XDJG_SendCharacter;
				m_tflv.m_tvlength = strlen(iter->recorddata)+1;
				sprintf((char*)&m_tflv.lpdata, "%s", iter->recorddata);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp("XDJG_Items", iter->recordfieldname))
			{
				memcpy(&m_StructInfo.m_sMail, iter->recorddata, sizeof(m_StructInfo.m_sMail));//��Ʒ����
				for(iItemNum = 0; iItemNum < m_StructInfo.m_sMail.limit_mail_item_num; iItemNum++)
				{
					if(m_StructInfo.m_sMail.item_list[iItemNum].itemId != 0)
					{
						iTorF = true;
						szData.Format("%d", m_StructInfo.m_sMail.item_list[iItemNum].itemId);
						m_tflv = DBTFLV.back();
						if(m_tflv.m_tagName == CEnumCore::TagName::XDJG_ItemID && 
						   m_StructInfo.m_sMail.item_list[iItemNum].itemId == atoi((char*)m_tflv.lpdata))
						{
							continue;
						}
						else if(m_tflv.m_tagName == CEnumCore::TagName::XDJG_ItemID && 
								m_StructInfo.m_sMail.item_list[iItemNum].itemId != atoi((char*)m_tflv.lpdata))
						{
							szData.Format("%s;%d", DBTFLV[DBTFLV.size() - 1].lpdata,m_StructInfo.m_sMail.item_list[iItemNum].itemId);
							DBTFLV.pop_back();
							DBTFLV.pop_back();

							m_tflv.nIndex  = DBTFLV.size() + 1;
							m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
							m_tflv.m_tagName = CEnumCore::TagName::XDJG_TempItemID;
							m_tflv.m_tvlength = szData.GetLength()+1;
							sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
							DBTFLV.push_back(m_tflv);

							m_tflv.nIndex  = DBTFLV.size() + 1;
							m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
							m_tflv.m_tagName = CEnumCore::TagName::XDJG_ItemID;
							m_tflv.m_tvlength = szData.GetLength()+1;
							sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
							DBTFLV.push_back(m_tflv);
							continue;
						}
						else
						{
							szData.Format("%d", m_StructInfo.m_sMail.item_list[iItemNum].itemId);
							m_tflv.nIndex  = DBTFLV.size() + 1;
							m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
							m_tflv.m_tagName = CEnumCore::TagName::XDJG_TempItemID;
							m_tflv.m_tvlength = szData.GetLength()+1;
							sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
							DBTFLV.push_back(m_tflv);

							szData.Format("%d", m_StructInfo.m_sMail.item_list[iItemNum].itemId);
							m_tflv.nIndex  = DBTFLV.size() + 1;
							m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
							m_tflv.m_tagName = CEnumCore::TagName::XDJG_ItemID;
							m_tflv.m_tvlength = szData.GetLength()+1;
							sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
							DBTFLV.push_back(m_tflv);
						}

// 						iTemp+=1;
// 						szData.Format("%d", m_StructInfo.m_sMail.item_list[iItemNum].itemId);
// 						m_tflv.nIndex  = DBTFLV.size() + 1;
// 						m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
// 						switch(iTemp)
// 						{
// 						case 1:
// 							m_tflv.m_tagName = CEnumCore::TagName::XDJG_ItemID1;
// 							break;
// 						case 2:
// 							m_tflv.m_tagName = CEnumCore::TagName::XDJG_ItemID2;
// 							break;
// 						case 3:
// 							m_tflv.m_tagName = CEnumCore::TagName::XDJG_ItemID3;
// 							break;
// 						case 4:
// 							m_tflv.m_tagName = CEnumCore::TagName::XDJG_ItemID4;
// 							break;
// 						case 5:
// 							m_tflv.m_tagName = CEnumCore::TagName::XDJG_ItemID5;
// 							break;
// 						case 6:
// 							m_tflv.m_tagName = CEnumCore::TagName::XDJG_ItemID6;
// 							break;
// 						case 7:
// 							m_tflv.m_tagName = CEnumCore::TagName::XDJG_ItemID7;
// 							break;
// 						case 8:
// 							m_tflv.m_tagName = CEnumCore::TagName::XDJG_ItemID8;
// 							break;
// 						default:
// 							break;
// 
// 						}
// 						m_tflv.m_tvlength = szData.GetLength()+1;
// 						sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
// 						DBTFLV.push_back(m_tflv);
					}
				}

				if(iTorF == false)
				{
					m_tflv.nIndex  = DBTFLV.size() + 1;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tagName = CEnumCore::TagName::XDJG_TempItemID;
					m_tflv.m_tvlength = strlen("0")+1;
					sprintf((char*)&m_tflv.lpdata, "%s", "0");
					DBTFLV.push_back(m_tflv);

					m_tflv.nIndex  = DBTFLV.size() + 1;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tagName = CEnumCore::TagName::XDJG_ItemID;
					m_tflv.m_tvlength = strlen("0")+1;
					sprintf((char*)&m_tflv.lpdata, "%s", "0");
					DBTFLV.push_back(m_tflv);
				}

			}
			else if(!strcmp("XDJG_Money", iter->recordfieldname))
			{
				m_tflv.nIndex  = DBTFLV.size() + 1;
				m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tagName = CEnumCore::TagName::XDJG_Money;
				m_tflv.m_tvlength = strlen(iter->recorddata)+1;
				sprintf((char*)&m_tflv.lpdata, "%s", iter->recorddata);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp("XDJG_Title", iter->recordfieldname))
			{
				m_tflv.nIndex  = DBTFLV.size() + 1;
				m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tagName = CEnumCore::TagName::XDJG_Title;
				m_tflv.m_tvlength = strlen(iter->recorddata)+1;
				sprintf((char*)&m_tflv.lpdata, "%s", iter->recorddata);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp("XDJG_ReceiveTime", iter->recordfieldname))
			{
				m_tflv.nIndex  = DBTFLV.size() + 1;
				m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tagName = CEnumCore::TagName::XDJG_ReceiveTime;
				m_tflv.m_tvlength = strlen(iter->recorddata)+1;
				sprintf((char*)&m_tflv.lpdata, "%s", iter->recorddata);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp("PageCount", iter->recordfieldname))
			{
				m_tflv.nIndex  = DBTFLV.size() + 1;
				m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tagName = CEnumCore::TagName::PageCount;
				m_tflv.m_tvlength = strlen(iter->recorddata)+1;
				sprintf((char*)&m_tflv.lpdata, "%s", iter->recorddata);
				DBTFLV.push_back(m_tflv);
			}
			else
				continue;
		}

		//����ʱ����idת��Ϊ��������
		for (pDBTFLV = DBTFLV.begin(); pDBTFLV != DBTFLV.end(); pDBTFLV++)
		{
			if(pDBTFLV->m_tagName == CEnumCore::TagName::XDJG_TempItemID)
			{
				strtempItemid = pDBTFLV->lpdata;
//				strtempItemid = "1234;13500;5000";
				CString strLast = strtempItemid.Right(strtempItemid.Find(';'));
				if(strtempItemid.Find(';') > 0)
				{
					ptempId = strtok(strtempItemid.GetBuffer(0),";");

					while(ptempId != NULL)
					{
						iTorF = FALSE;

						//��Ʒ����
						for(pItemInfo = VecItemInfo.begin(); pItemInfo != VecItemInfo.end(); pItemInfo++)
						{
							if(pItemInfo->iItemId == atoi(ptempId))
							{
								iTorF = TRUE;
								strItemname += pItemInfo->strItemName.c_str();
								if(strLast.Compare(ptempId))
									strItemname += ";";
								break;
							}
							else
								continue;
						}

						if(iTorF == FALSE)
						{
							strItemname += "δ֪";
							if(strLast.Compare(ptempId))
								strItemname += ";";
						}
							
						ptempId = strtok(NULL,";");
					}

					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tagName = CEnumCore::TagName::XDJG_ItemName;
					pDBTFLV->m_tvlength =strItemname.length();
					sprintf((char*)&pDBTFLV->lpdata, "%s", strItemname.c_str());
				}
				else
				{
					if(!strtempItemid.Compare("0"))
					{
						*pDBTFLV->lpdata = NULL;
						pDBTFLV->m_tagName = CEnumCore::TagName::XDJG_ItemName;
						pDBTFLV->m_tvlength = strlen("")+1;
						sprintf((char*)&pDBTFLV->lpdata, "%s", "");
					}
					else
					{
						for(pItemInfo = VecItemInfo.begin(); pItemInfo != VecItemInfo.end(); pItemInfo++)
						{
							if(pItemInfo->iItemId == atoi(strtempItemid.GetBuffer(0)))
							{
								*pDBTFLV->lpdata = NULL;
								pDBTFLV->m_tagName = CEnumCore::TagName::XDJG_ItemName;
								pDBTFLV->m_tvlength = pItemInfo->strItemName.size();
								memcpy(&pDBTFLV->lpdata, pItemInfo->strItemName.c_str(), pItemInfo->strItemName.size());
								break;
							}
							else
								continue;
						}
					}
				}
			}
		}

	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "�ʼ���Ϣ��ѯ");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	return DBTFLV;

}

//��������Ϣ
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_DB_AuctioneerInfo_Query(int userByID, char *ServerIP, char *Account, char *Character, int iIndex, int iPageSize)
{
	CGlobalStruct::TFLV m_tflv;
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	CString szData = "";
	char realServerIP[40];
	vector<CGlobalStruct::DataRecord> datarecord;
	_Res sResItems;
	vector<st_ItemInfo>::iterator pItemInfo = NULL;
	char retMessage[255];
	ZeroMemory(retMessage,255);
	Vec_ItemInfo VecItemInfo;
	
	try
	{
		sprintf(retMessage, "��������Ϣ��ѯ:<����>%s, <�ʺ�>%s, <��ɫ��>%s", ServerIP, Account, Character);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,3))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "��������Ϣ");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}

		strSql.Format("Select '%s' AS XDJG_Account, '%s' AS XDJG_Character, \
							  item AS XDJG_Items ,LTRIM(TIMEDIFF(FROM_UNIXTIME(expiretime),create_time)) AS XDJG_SurplusTime, \
							  startbid AS XDJG_AuctionPrice, deposit AS XDJG_Deposit \
							  from %s.auction_site \
							  where ownername = '%s'", \
							  Account,Character,GameDB,Character);

#ifdef _DEBUG
		printf(strSql, GameDB, GameDB);
#endif

		m_SqlHelper.SqlHelperMain("XDJG", &datarecord,realServerIP,strSql.GetBuffer(0),12,0,0);
		
		//ȡ��Ʒ����
		GetItemInfo(VecItemInfo);

		vector<CGlobalStruct::DataRecord>::iterator iter;
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			if(!strcmp("XDJG_Account", iter->recordfieldname))
			{
				m_tflv.nIndex  = DBTFLV.size() + 1;
				m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tagName = CEnumCore::TagName::XDJG_Account;
				m_tflv.m_tvlength = strlen(Account)+1;
				sprintf((char*)&m_tflv.lpdata, "%s", Account);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp("XDJG_Character", iter->recordfieldname))
			{
				m_tflv.nIndex  = DBTFLV.size() + 1;
				m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tagName = CEnumCore::TagName::XDJG_Character;
				m_tflv.m_tvlength = strlen(Character)+1;
				sprintf((char*)&m_tflv.lpdata, "%s", Character);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp("XDJG_Items", iter->recordfieldname))
			{
				memcpy(&sResItems, iter->recorddata, sizeof(sResItems));//��Ʒ����
				if(sResItems.itemId != 0)
				{
					szData.Format("%d", sResItems.itemId);
					m_tflv.nIndex  = DBTFLV.size() + 1;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tagName = CEnumCore::TagName::XDJG_ItemID;
					m_tflv.m_tvlength = szData.GetLength()+1;
					sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
					DBTFLV.push_back(m_tflv);

					//��Ʒ����
					for(pItemInfo = VecItemInfo.begin(); pItemInfo != VecItemInfo.end(); pItemInfo++)
					{
						if(pItemInfo->iItemId == (int)sResItems.itemId)
						{
							m_tflv.nIndex = DBTFLV.size() + 1;
							m_tflv.m_tagName = CEnumCore::TagName::XDJG_ItemName;
							m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
							m_tflv.m_tvlength = pItemInfo->strItemName.size();
							memcpy(&m_tflv.lpdata, pItemInfo->strItemName.c_str(), pItemInfo->strItemName.size());
							DBTFLV.push_back(m_tflv);
							break;
						}
						else
							continue;
					}

					if(m_tflv.m_tagName != CEnumCore::TagName::XDJG_ItemName)
					{
						m_tflv.nIndex = DBTFLV.size() + 1;
						m_tflv.m_tagName = CEnumCore::TagName::XDJG_ItemName;
						m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength = strlen("")+1;
						sprintf((char*)&m_tflv.lpdata, "%s", "");
						DBTFLV.push_back(m_tflv);
					}
				}
				else
				{
					m_tflv.nIndex  = DBTFLV.size() + 1;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tagName = CEnumCore::TagName::XDJG_ItemID;
					m_tflv.m_tvlength = strlen("0")+1;
					sprintf((char*)&m_tflv.lpdata, "%s", "0");
					DBTFLV.push_back(m_tflv);

					m_tflv.nIndex = DBTFLV.size() + 1;
					m_tflv.m_tagName = CEnumCore::TagName::XDJG_ItemName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen("")+1;
					sprintf((char*)&m_tflv.lpdata, "%s", "");
					DBTFLV.push_back(m_tflv);
				}
			}
			else if(!strcmp("XDJG_SurplusTime", iter->recordfieldname))
			{
				m_tflv.nIndex  = DBTFLV.size() + 1;
				m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tagName = CEnumCore::TagName::XDJG_SurplusTime;
				m_tflv.m_tvlength = strlen(iter->recorddata)+1;
				sprintf((char*)&m_tflv.lpdata, "%s", iter->recorddata);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp("XDJG_AuctionPrice", iter->recordfieldname))
			{
				m_tflv.nIndex  = DBTFLV.size() + 1;
				m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tagName = CEnumCore::TagName::XDJG_AuctionPrice;
				m_tflv.m_tvlength = strlen(iter->recorddata)+1;
				sprintf((char*)&m_tflv.lpdata, "%s", iter->recorddata);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp("XDJG_Deposit", iter->recordfieldname))
			{
				m_tflv.nIndex  = DBTFLV.size() + 1;
				m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tagName = CEnumCore::TagName::XDJG_Deposit;
				m_tflv.m_tvlength = strlen(iter->recorddata)+1;
				sprintf((char*)&m_tflv.lpdata, "%s", iter->recorddata);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp("PageCount", iter->recordfieldname))
			{
				m_tflv.nIndex  = DBTFLV.size() + 1;
				m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tagName = CEnumCore::TagName::PageCount;
				m_tflv.m_tvlength = strlen(iter->recorddata)+1;
				sprintf((char*)&m_tflv.lpdata, "%s", iter->recorddata);
				DBTFLV.push_back(m_tflv);
			}
			else
				continue;
		}
//		DBTFLV = GameDBQuery("XDJG",realServerIP,1, iIndex, iPageSize, strSql.GetBuffer(0), GameDB, GameDB);
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "��������Ϣ��ѯ");//��������Ϣд����־�в����ش�����Ϣ
	}

	return DBTFLV;
}

//�û���ɫ��Ϣ���޸�:�޸ĵȼ�/����
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_Net_ModifyLevel(int userByID, char *ServerIP, char *Character, int UpdateType, int Level, int Exp)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	
	string strResult;
	vector <string> vectorColInfo;
	char strConn[512];
	CString szTime = "";
	CString strSql = "";
	SYSTEMTIME timeDest;
	ZeroMemory(strConn, 512);
	char retMessage[255];
	ZeroMemory(retMessage, 255);
	
	try
	{
		if(1 == UpdateType)
		{
			sprintf(retMessage, "�޸ĵȼ�:<����>%s, <��ɫ>%s, <�޸ĺ�ȼ�>%d", ServerIP, Character, Level);
			sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=modify_character(%d,%d,%s)", 
				ServerIP, HTTPPORT,
			UpdateType,Level,Character);
		}
		else if(2 == UpdateType)
		{
			sprintf(retMessage, "�޸ľ���:<����>%s, <��ɫ>%s, <�޸ĺ���>%d", ServerIP, Character, Exp);
			sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=modify_character(%d,%d,%s)", 
				ServerIP, HTTPPORT,
			UpdateType,Exp,Character);
		}
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		if(m_operXDJG.getWebInfo(strConn, strResult))
		{
			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "�޸ĵȼ�/����");				
			}
			else
			{
				int iResult = atoi(strResult.c_str());
				if (iResult>=0)
				{
					
					bool bFinishFlag = false;
					int Maxnum_wait = 10, wait_num=0;
					char *strR = NULL;
					CString szResult = "";
					
					while (!bFinishFlag)
					{
						sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=get_operation_result(%d)", 
							ServerIP, HTTPPORT,
							iResult);
						m_operXDJG.getWebInfo(strConn, strResult);

						int iResultFlag = atoi(strResult.c_str());
						switch (iResultFlag)
						{
						case 1:
							bFinishFlag = true;
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�޸ĵȼ�/����ɹ�", "�޸ĵȼ�/����");
							break;
						case -2:
							wait_num++;
							if (wait_num > Maxnum_wait)
							{
								bFinishFlag = true; 
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�޸ĵȼ�/���鳬ʱ", "�޸ĵȼ�/����");
							}
							Sleep(1000);
							break;
						default:
							bFinishFlag = true;
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�޸ĵȼ�/����ʧ��", "�޸ĵȼ�/����");
						}
					}
				}
				else
				{
					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "�޸ĵȼ�/����");
				}
				
			}
			
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "�޸ĵȼ�/����");		
		}
	}
	catch (...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "�޸ĵȼ�/����");	
	}
	
	//����־
	m_operXDJG.GmOperLog(userByID, ServerIP, "�޸ĵȼ�/����", retMessage, DBTFLV[0]);
	return DBTFLV;

// 	vector<CGlobalStruct::TFLV> DBTFLV;
// 	CString strSql = "";
// 	char realServerIP[40];
// 	
// 	try
// 	{
// 		logFile.WriteLog("<�������>",operName,"�û���ɫ��Ϣ���޸�:�޸ĵȼ�");//��LOG
// 		
// 		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,3))
// 		{
// 			return DBTFLV;//�޷���ȡ�����ķ�����IP
// 		}
// 		
// 		strSql.Format("update %s.character set grade = %d \
// 						where name = '%s'", \
// 							  GameDB,Level,Character);
// 		
// #ifdef _DEBUG
// 		printf(strSql, GameDB, GameDB);
// #endif
// 		
// 		if(GameDBOper("XDJG", realServerIP, 1 , strSql.GetBuffer(0), GameDB, GameDB))
// 			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�޸ĵȼ��ɹ�", "�û���ɫ��Ϣ���޸�:�޸ĵȼ�");
// 		else
// 			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�޸ĵȼ�ʧ��", "�û���ɫ��Ϣ���޸�:�޸ĵȼ�");
// 	}
// 	catch(...)
// 	{
// 		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "�û���ɫ��Ϣ���޸�:�޸ĵȼ�");//��������Ϣд����־�в����ش�����Ϣ
// 	}
// 	
// 	return DBTFLV;
}

//�û��������:�޸�����
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_Net_ModifyPwd(int userByID, char *ServerIP, char *Account, char* NewPwd)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	
	string strResult;
	vector <string> vectorColInfo;
	char strConn[512];
	CString szTime = "";
	CString strSql = "";
	SYSTEMTIME timeDest;
	ZeroMemory(strConn, 512);
	char retMessage[255];
	ZeroMemory(retMessage, 255);

	try
	{
		sprintf(retMessage, "�޸�����:<����>%s, <�ʺ�>%s, <������>%s", ServerIP, Account, NewPwd);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG

		string strTempPass = strrev((char*)md5_PassWord(NewPwd).c_str());
		strTempPass += "CT_hei8evkjA*10";

		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=modify_password(%s,%s)", 
			ServerIP, HTTPPORT,
			Account, md5_PassWord((char *)strTempPass.c_str()).c_str());
		
		if(m_operXDJG.getWebInfo(strConn, strResult))
		{
			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "XDJG�޸�����");				
			}
			else
			{
				int iResult = atoi(strResult.c_str());
				if (iResult>=0)
				{
					
					bool bFinishFlag = false;
					int Maxnum_wait = 10, wait_num=0;
					char *strR = NULL;
					CString szResult = "";
					
					while (!bFinishFlag)
					{
						sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=get_operation_result(%d)", 
							ServerIP, HTTPPORT,
							iResult);
						m_operXDJG.getWebInfo(strConn, strResult);

						int iResultFlag = atoi(strResult.c_str());
						switch (iResultFlag)
						{
						case -1:
						case -3:
							bFinishFlag = true;
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�޸�����ʧ��", "XDJG�޸�����");
						case -2:
							wait_num++;
							if (wait_num > Maxnum_wait)
							{
								bFinishFlag = true; 
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�޸����볬ʱ", "XDJG�޸�����");
							}
							Sleep(1000);
							break;
						default:
							strR = strtok((char*)strResult.c_str(),",");
							if(atoi(strR) == 1)
							{
								bFinishFlag = true;
								strR = strtok(NULL,",");
								//ȡϵͳʱ��:�޸������ʱ��
								GetLocalTime(&timeDest);	
								szTime.Format("%04d-%02d-%02d %02d:%02d:%02d",timeDest.wYear,timeDest.wMonth,timeDest.wDay,timeDest.wHour,timeDest.wMinute,timeDest.wSecond);
								
								strSql.Format("exec XDJG_SAVE_OLDPASSWORD %d, '%s', '%s', '%s', '%s', '%s', %d",userByID, ServerIP, Account, strR, NewPwd, szTime.GetBuffer(0), 0);

								if(!m_SqlHelper.SqlHelperMain("127.0.0.1",strSql.GetBuffer(0),4))
								{
									DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ԭ����ʧ��", "�û��������:�޸�����");
									return DBTFLV;
								}
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�޸�����ɹ�", "XDJG�޸�����");
								break;
							}
							else
							{
								bFinishFlag = true;
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�޸�����ʧ��", "XDJG�޸�����");
							}
							break;
						}

// 						szResult = strResult.c_str(); 
// 						strR = strtok(szResult.GetBuffer(0),",");
// 						if(strR == NULL)
// 						{
// 							int iResultFlag = atoi(strResult.c_str());
// 							switch (iResultFlag)
// 							{
// 							case -2:
// 								wait_num++;
// 								if (wait_num > Maxnum_wait)
// 								{
// 									bFinishFlag = true; 
// 									DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�޸����볬ʱ", "XDJG�޸�����");
// 								}
// 								Sleep(1000);
// 								break;
// 							default:
// 								bFinishFlag = true;
// 								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�޸�����ʧ��", "XDJG�޸�����");
// 							}
// 						}
// 						else
// 						{
// 							if(atoi(strR) == 1)
// 							{
// 								strR = strtok(NULL,",");
// 								//ȡϵͳʱ��:�޸������ʱ��
// 								GetLocalTime(&timeDest);	
// 								szTime.Format("%04d-%02d-%02d %02d:%02d:%02d",timeDest.wYear,timeDest.wMonth,timeDest.wDay,timeDest.wHour,timeDest.wMinute,timeDest.wSecond);
// 								
// 								strSql.Format("exec XDJG_SAVE_OLDPASSWORD %d, '%s', '%s', '%s', '%s', '%s', %d",userByID, ServerIP, Account, strR, NewPwd, szTime.GetBuffer(0), 0);
// 
// 								if(!m_SqlHelper.SqlHelperMain("127.0.0.1",strSql.GetBuffer(0),4))
// 								{
// 									DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ԭ����ʧ��", "�û��������:�޸�����");
// 									return DBTFLV;
// 								}
// 								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�޸�����ɹ�", "XDJG�޸�����");
// 								break;
// 							}
// 							else
// 							{
// 								bFinishFlag = true;
// 								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�޸�����ʧ��", "XDJG�޸�����");
// 							}
// 						}
					}
				}
				else
				{
					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "XDJG�޸�����");
				}
				
			}
			
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "XDJG�޸�����");		
		}
	}
	catch (...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "XDJG�޸�����");	
	}
	
	//����־
	m_operXDJG.GmOperLog(userByID, ServerIP, "XDJG�޸�����", retMessage, DBTFLV[0]);
	return DBTFLV;

// 	vector<CGlobalStruct::TFLV> DBTFLV;
// 	CString strSql = "";
// 	char realServerIP[40];
// 	vector<CGlobalStruct::DataRecord> datarecord;
// 	CString szTime = "";
// 	SYSTEMTIME timeDest;
// 	string szOldPwd = "";
// 	
// 	try
// 	{
// 		logFile.WriteLog("<�������>",operName,"�û��������:�޸�����");//��LOG
// 		
// 		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
// 		{
// 			return DBTFLV;//�޷���ȡ�����ķ�����IP
// 		}
// 		
// 		//ȡ��ԭʼ����
// 		strSql.Format("Select password from %s.account where name = '%s'", AccountDB, Account);
// 		
// 		m_SqlHelper.SqlHelperMain("XDJG", &datarecord,realServerIP,strSql.GetBuffer(0),1,0,0);
// 
// 		vector<CGlobalStruct::DataRecord>::iterator iter;
// 		CGlobalStruct::DataRecord  m_DataRecord;
// 		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
// 		{
// 			m_DataRecord = *iter;
// 			if(!strcmp("password", m_DataRecord.recordfieldname))
// 			{
// 				szOldPwd = m_DataRecord.recorddata;//����ԭʼ����
// 				break;
// 			}
// 			else
// 				continue;
// 		}
// 
// 		//ȡϵͳʱ��:�޸������ʱ��
// 		GetLocalTime(&timeDest);	
// 		szTime.Format("%04d-%02d-%02d %02d:%02d:%02d",timeDest.wYear,timeDest.wMonth,timeDest.wDay,timeDest.wHour,timeDest.wMinute,timeDest.wSecond);
// 		
// 		strSql.Format("exec XDJG_SAVE_OLDPASSWORD %d, '%s', '%s', '%s', '%s', '%s', %d",userByID, ServerIP, Account, szOldPwd.c_str(), NewPwd, szTime.GetBuffer(0), 0);
// 
// 		if(!m_SqlHelper.SqlHelperMain("127.0.0.1",strSql.GetBuffer(0),4))
// 		{
// 			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ԭ����ʧ��", "�û��������:�޸�����");
// 			return DBTFLV;
// 		}
// 
// #ifdef _DEBUG
// 		printf(strSql, AccountDB, AccountDB);
// #endif
// 
// 		strSql.Format("update %s.account set password = '%s' where name = '%s'", AccountDB, NewPwd, Account);
// 
// 		if(GameDBOper("XDJG", realServerIP, 1 , strSql.GetBuffer(0), AccountDB, AccountDB))
// 			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�޸�����ɹ�", "�û��������:�޸�����");
// 		else
// 			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�޸�����ʧ��", "�û��������:�޸�����");
// 	}
// 	catch(...)
// 	{
// 		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "�û��������:�޸�����");//��������Ϣд����־�в����ش�����Ϣ
// 	}
// 	
// 	return DBTFLV;
}

//�û��������:��ʱ����ָ�
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_Net_ResumePwd(int userByID, char *ServerIP, char *Account)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	char strConn[512];
	string szOldPwd = "";
	string strResult = "";
	vector<CGlobalStruct::DataRecord> datarecord;
	ZeroMemory(strConn, 512);
	char retMessage[255];
	ZeroMemory(retMessage, 255);
	
	try
	{
		//ȡ�޸�ǰ������
		strSql.Format("exec XDJG_SAVE_OLDPASSWORD %d, '%s', '%s', '%s', '%s', '%s', %d",userByID, ServerIP, Account, "", "", "", 1);
		
		m_SqlHelper.SqlHelperMain("XDJG", &datarecord,"127.0.0.1",strSql.GetBuffer(0),4,0,0);
		
		if(datarecord.empty())
			return DBTFLV;

		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("OldPwd", m_DataRecord.recordfieldname))
			{
				szOldPwd = m_DataRecord.recorddata;//��ȡԭ����
				break;
			}
			else
				continue;
		}

		sprintf(retMessage, "��ʱ����ָ�:<����>%s, <�ʺ�>%s, <ԭ����>%s", ServerIP, Account, szOldPwd.c_str());
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG

		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=modify_password(%s,%s)", 
			ServerIP, HTTPPORT,
			Account,szOldPwd.c_str());
		
		if(m_operXDJG.getWebInfo(strConn, strResult))
		{
			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "��ʱ����ָ�");				
			}
			else
			{
				int iResult = atoi(strResult.c_str());
				if (iResult>=0)
				{
					
					bool bFinishFlag = false;
					int Maxnum_wait = 10, wait_num=0;
					char *strR = NULL;
					CString szResult = "";
					
					while (!bFinishFlag)
					{
						sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=get_operation_result(%d)", 
							ServerIP, HTTPPORT,
							iResult);
						m_operXDJG.getWebInfo(strConn, strResult);

						int iResultFlag = atoi(strResult.c_str());
						switch (iResultFlag)
						{
						case -1:
						case -3:
							bFinishFlag = true;
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�޸�����ʧ��", "XDJG�޸�����");
						case -2:
							wait_num++;
							if (wait_num > Maxnum_wait)
							{
								bFinishFlag = true; 
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�޸����볬ʱ", "XDJG�޸�����");
							}
							Sleep(1000);
							break;
						default:
							bFinishFlag = true;
							strR = strtok((char*)strResult.c_str(),",");
							if(atoi(strR) == 1)
							{
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�ָ�����ɹ�", "��ʱ����ָ�");
							}
							else
							{
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�ָ�����ʧ��", "��ʱ����ָ�");
							}
							break;
						}
					}
				}
				else
				{
					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "��ʱ����ָ�");
				}
			}


// 						szResult = strResult.c_str(); 
// 						strR = strtok(szResult.GetBuffer(0),",");
// 						if(strR == NULL)
// 						{
// 							int iResultFlag = atoi(strResult.c_str());
// 							switch (iResultFlag)
// 							{
// 							case -2:
// 								wait_num++;
// 								if (wait_num > Maxnum_wait)
// 								{
// 									bFinishFlag = true; 
// 									DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�ָ����볬ʱ", "��ʱ����ָ�");
// 								}
// 								Sleep(1000);
// 								break;
// 							default:
// 								bFinishFlag = true;
// 								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�ָ�����ʧ��", "��ʱ����ָ�");
// 							}
// 						}
// 						else
// 						{
// 							if(atoi(strR) == 1)
// 							{
// 								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�ָ�����ɹ�", "��ʱ����ָ�");
// 								break;
// 							}
// 							else
// 							{
// 								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�ָ�����ʧ��", "��ʱ����ָ�");
// 								break;
// 							}
// 						}
//					}
// 				}
// 				else
// 				{
// 					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "��ʱ����ָ�");
// 				}
//				
//			}
			
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "��ʱ����ָ�");		
		}
		
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "�û��������:��ʱ����ָ�");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	//����־
	m_operXDJG.GmOperLog(userByID, ServerIP, "XDJG��ʱ����ָ�", retMessage, DBTFLV[0]);
	return DBTFLV;
}

//�û���ɫ����:��ɫ�ָ�
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_Net_ResumeRole(int userByID, char *ServerIP, char *Character)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	char strConn[512];
	string strResult = "";
	ZeroMemory(strConn, 512);
	char retMessage[255];
	ZeroMemory(retMessage, 255);

	try
	{
		sprintf(retMessage, "��ɫ�ָ�:<����>%s, <�ָ���ɫ>%s", ServerIP, Character);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=manage_character(%d,%s)", 
			ServerIP, HTTPPORT,
			2,Character);
		
		if(m_operXDJG.getWebInfo(strConn, strResult))
		{
			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "��ɫ�ָ�");				
			}
			else
			{
				int iResult = atoi(strResult.c_str());
				if (iResult>=0)
				{
					
					bool bFinishFlag = false;
					int Maxnum_wait = 10, wait_num=0;
					while (!bFinishFlag)
					{
						sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=get_operation_result(%d)", 
							ServerIP, HTTPPORT,
							iResult);

						m_operXDJG.getWebInfo(strConn, strResult);
						int iResultFlag = atoi(strResult.c_str());
						switch (iResultFlag)
						{
						case 1:
							bFinishFlag = true;
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ɫ�ָ��ɹ�", "��ɫ�ָ�");
							break;
						case -2:
							wait_num++;
							if (wait_num > Maxnum_wait)
							{
								bFinishFlag = true; 
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ɫ�ָ���ʱ", "��ɫ�ָ�");
							}
							Sleep(1000);
							break;
						default:
							bFinishFlag = true;
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ɫ�ָ�ʧ��", "��ɫ�ָ�");
						}
					}
				}
				else
				{
					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "��ɫ�ָ�");
				}
				
			}
			
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "��ɫ�ָ�");		
		}
		
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "��ɫ�ָ�");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	//����־
	m_operXDJG.GmOperLog(userByID, ServerIP, "XDJG��ɫ�ָ�", retMessage, DBTFLV[0]);
	return DBTFLV;

// 	vector<CGlobalStruct::TFLV> DBTFLV;
// 	CString strSql = "";
// 	char realServerIP[40];
// 	
// 	try
// 	{
// 		logFile.WriteLog("<�������>",operName,"�û���ɫ����:��ɫ�ָ�");//��LOG
// 		
// 		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,3))
// 		{
// 			return DBTFLV;//�޷���ȡ�����ķ�����IP
// 		}
// 		
// 		strSql.Format("update %s.character set Deleted = 0 where name = '%s'", GameDB, Character);
// 		
// #ifdef _DEBUG
// 		printf(strSql, GameDB, GameDB);
// #endif
// 		
// 		if(GameDBOper("XDJG", realServerIP, 1 , strSql.GetBuffer(0), GameDB, GameDB))
// 			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ɫ�ָ��ɹ�", "�û���ɫ����:��ɫ�ָ�");
// 		else
// 			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ɫ�ָ�ʧ��", "�û���ɫ����:��ɫ�ָ�");
// 	}
// 	catch(...)
// 	{
// 		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "�û���ɫ����:��ɫ�ָ�");//��������Ϣд����־�в����ش�����Ϣ
// 	}
// 	
// 	return DBTFLV;
}

//�û���ɫ����:��ɫɾ��
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_Net_DeleteRole(int userByID, char *ServerIP, char *Character)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	char strConn[512];
	string strResult = "";
	ZeroMemory(strConn, 512);
	char retMessage[255];
	ZeroMemory(retMessage, 255);
	
	try
	{
		sprintf(retMessage, "��ɫɾ��:<����>%s, <ɾ����ɫ>%s", ServerIP, Character);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=manage_character(%d,%s)", 
			ServerIP, HTTPPORT,
			1,Character);
		
		if(m_operXDJG.getWebInfo(strConn, strResult))
		{
			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "��ɫɾ��");				
			}
			else
			{
				int iResult = atoi(strResult.c_str());
				if (iResult>=0)
				{
					
					bool bFinishFlag = false;
					int Maxnum_wait = 10, wait_num=0;
					while (!bFinishFlag)
					{
						sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=get_operation_result(%d)", 
							ServerIP, HTTPPORT,
							iResult);
						
						m_operXDJG.getWebInfo(strConn, strResult);
						int iResultFlag = atoi(strResult.c_str());
						switch (iResultFlag)
						{
						case 1:
							bFinishFlag = true;
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ɫɾ���ɹ�", "��ɫɾ��");
							break;
						case -2:
							wait_num++;
							if (wait_num > Maxnum_wait)
							{
								bFinishFlag = true; 
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ɫɾ����ʱ", "��ɫɾ��");
							}
							Sleep(1000);
							break;
						default:
							bFinishFlag = true;
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ɫɾ��ʧ��", "��ɫɾ��");
						}
					}
				}
				else
				{
					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "��ɫɾ��");
				}
				
			}
			
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "��ɫɾ��");		
		}
		
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "��ɫɾ��");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	//����־
	m_operXDJG.GmOperLog(userByID, ServerIP, "XDJG��ɫɾ��", retMessage, DBTFLV[0]);
	return DBTFLV;

// 	vector<CGlobalStruct::TFLV> DBTFLV;
// 	CString strSql = "";
// 	char realServerIP[40];
// 	
// 	try
// 	{
// 		logFile.WriteLog("<�������>",operName,"�û���ɫ����:��ɫɾ��");//��LOG
// 		
// 		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,3))
// 		{
// 			return DBTFLV;//�޷���ȡ�����ķ�����IP
// 		}
// 		
// 		strSql.Format("update %s.character set Deleted = 1 where name = '%s'", GameDB, Character);
// 		
// #ifdef _DEBUG
// 		printf(strSql, GameDB, GameDB);
// #endif
// 		
// 		if(GameDBOper("XDJG", realServerIP, 1 , strSql.GetBuffer(0), GameDB, GameDB))
// 			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ɫɾ���ɹ�", "�û���ɫ����:��ɫɾ��");
// 		else
// 			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ɫɾ��ʧ��", "�û���ɫ����:��ɫɾ��");
// 	}
// 	catch(...)
// 	{
// 		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "�û���ɫ����:��ɫɾ��");//��������Ϣд����־�в����ش�����Ϣ
// 	}
// 	
// 	return DBTFLV;
}

//�޸Ľ�ɫ���Ͻ�Ǯ����(�󶨱�)
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_Net_UpRoleBind(int userByID, char *ServerIP, char *Character, int Money)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	
	string strResult;
	vector <string> vectorColInfo;
	char strConn[512];
	ZeroMemory(strConn, 512);
	char retMessage[255];
	ZeroMemory(retMessage, 255);
	
	try
	{
		sprintf(retMessage, "�޸Ľ�ɫ���Ͻ�Ǯ����(�󶨱�):<����>%s, <��ɫ>%s, <�޸�Ǯ��>%d", ServerIP, Character, Money);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=add_bindmoney(%s,%d)", 
			ServerIP, HTTPPORT,
			Character,Money);
		
		if(m_operXDJG.getWebInfo(strConn, strResult))
		{
			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "�޸Ľ�ɫ���Ͻ�Ǯ����(�󶨱�)");				
			}
			else
			{
				int iResult = atoi(strResult.c_str());
				if (iResult>=0)
				{
					
					bool bFinishFlag = false;
					int Maxnum_wait = 10, wait_num=0;
					
					while (!bFinishFlag)
					{
						sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=get_operation_result(%d)", 
							ServerIP, HTTPPORT,
							iResult);
						m_operXDJG.getWebInfo(strConn, strResult);
						int iResultFlag = atoi(strResult.c_str());
						switch (iResultFlag)
						{
						case 1:
							bFinishFlag = true;
							
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�޸Ľ�Ǯ�ɹ�(�󶨱�)", "�޸Ľ�ɫ���Ͻ�Ǯ����(�󶨱�)");
							
							break;
						case -2:
							wait_num++;
							if (wait_num > Maxnum_wait)
							{
								bFinishFlag = true; 
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�޸Ľ�Ǯ��ʱ(�󶨱�)", "�޸Ľ�ɫ���Ͻ�Ǯ����(�󶨱�)");
								
							}
							Sleep(1000);
							break;
						default:
							bFinishFlag = true;
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�޸Ľ�Ǯʧ��(�󶨱�)", "�޸Ľ�ɫ���Ͻ�Ǯ����(�󶨱�)");
						}
					}
				}
				else
				{
					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "�޸Ľ�ɫ���Ͻ�Ǯ����(�󶨱�)");
				}
				
			}
			
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "�޸Ľ�ɫ���Ͻ�Ǯ����(�󶨱�)");		
		}
	}
	catch (...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "�޸Ľ�ɫ���Ͻ�Ǯ����(�󶨱�)");	
	}
	
	m_operXDJG.GmOperLog(userByID, ServerIP, "XDJG�޸Ľ�ɫ���Ͻ�Ǯ����(�󶨱�)", retMessage, DBTFLV[0]);
	return DBTFLV;
}

//�޸Ľ�ɫ���Ͻ�Ǯ����(�ǰ󶨱�)
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_Net_UpRoleNonBind(int userByID, char *ServerIP, char *Character, int Money)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	
	string strResult;
	vector <string> vectorColInfo;
	char strConn[512];
	ZeroMemory(strConn, 512);
	char retMessage[255];
	ZeroMemory(retMessage, 255);
	
	try
	{
		sprintf(retMessage, "�޸Ľ�ɫ���Ͻ�Ǯ����(�ǰ󶨱�):<����>%s, <��ɫ>%s, <�޸�Ǯ��>%d", ServerIP, Character, Money);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=add_money(%s,%d)", 
			ServerIP, HTTPPORT,
			Character,Money);
		
		if(m_operXDJG.getWebInfo(strConn, strResult))
		{
			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "�޸Ľ�ɫ���Ͻ�Ǯ����(�ǰ󶨱�)");				
			}
			else
			{
				int iResult = atoi(strResult.c_str());
				if (iResult>=0)
				{
					
					bool bFinishFlag = false;
					int Maxnum_wait = 10, wait_num=0;
					
					while (!bFinishFlag)
					{
						sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=get_operation_result(%d)", 
							ServerIP, HTTPPORT,
							iResult);
						m_operXDJG.getWebInfo(strConn, strResult);
						int iResultFlag = atoi(strResult.c_str());
						switch (iResultFlag)
						{
						case 1:
							bFinishFlag = true;
							
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�޸Ľ�Ǯ�ɹ�(�ǰ󶨱�)", "�޸Ľ�ɫ���Ͻ�Ǯ����(�ǰ󶨱�)");
							
							break;
						case -2:
							wait_num++;
							if (wait_num > Maxnum_wait)
							{
								bFinishFlag = true; 
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�޸Ľ�Ǯ��ʱ(�ǰ󶨱�)", "�޸Ľ�ɫ���Ͻ�Ǯ����(�ǰ󶨱�)");
								
							}
							Sleep(1000);
							break;
						default:
							bFinishFlag = true;
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�޸Ľ�Ǯʧ��(�ǰ󶨱�)", "�޸Ľ�ɫ���Ͻ�Ǯ����(�ǰ󶨱�)");
						}
					}
				}
				else
				{
					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "�޸Ľ�ɫ���Ͻ�Ǯ����(�ǰ󶨱�)");
				}
				
			}
			
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "�޸Ľ�ɫ���Ͻ�Ǯ����(�ǰ󶨱�)");		
		}
	}
	catch (...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "�޸Ľ�ɫ���Ͻ�Ǯ����(�ǰ󶨱�)");	
	}
	
	m_operXDJG.GmOperLog(userByID, ServerIP, "XDJG�޸Ľ�ɫ���Ͻ�Ǯ����(�ǰ󶨱�)", retMessage, DBTFLV[0]);
	return DBTFLV;
}

//��ӽ�ɫ���Ͻ�Ǯ����(�󶨱�)
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_Net_AddRoleBind(int userByID, char *ServerIP, char *Character, int Money)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	
	string strResult;
	vector <string> vectorColInfo;
	char strConn[512];
	ZeroMemory(strConn, 512);
	char retMessage[255];
	ZeroMemory(retMessage, 255);
	
	try
	{
		sprintf(retMessage, "��ӽ�ɫ���Ͻ�Ǯ����(�󶨱�):<����>%s, <��ɫ>%s, <���Ǯ��>%d", ServerIP, Character, Money);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=add_bindmoney(%s,%d)", 
			ServerIP, HTTPPORT,
			Character,Money);
		
		if(m_operXDJG.getWebInfo(strConn, strResult))
		{
			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "��ӽ�ɫ���Ͻ�Ǯ����(�󶨱�)");				
			}
			else
			{
				int iResult = atoi(strResult.c_str());
				if (iResult>=0)
				{
					
					bool bFinishFlag = false;
					int Maxnum_wait = 10, wait_num=0;
					
					while (!bFinishFlag)
					{
						sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=get_operation_result(%d)", 
							ServerIP, HTTPPORT,
							iResult);
						m_operXDJG.getWebInfo(strConn, strResult);
						int iResultFlag = atoi(strResult.c_str());
						switch (iResultFlag)
						{
						case 1:
							bFinishFlag = true;
							
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ӽ�Ǯ�ɹ�(�󶨱�)", "��ӽ�ɫ���Ͻ�Ǯ����(�󶨱�)");
							
							break;
						case -2:
							wait_num++;
							if (wait_num > Maxnum_wait)
							{
								bFinishFlag = true; 
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ӽ�Ǯ��ʱ(�󶨱�)", "��ӽ�ɫ���Ͻ�Ǯ����(�󶨱�)");
								
							}
							Sleep(1000);
							break;
						default:
							bFinishFlag = true;
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ӽ�Ǯʧ��(�󶨱�)", "��ӽ�ɫ���Ͻ�Ǯ����(�󶨱�)");
						}
					}
				}
				else
				{
					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "��ӽ�ɫ���Ͻ�Ǯ����(�󶨱�)");
				}
				
			}
			
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "��ӽ�ɫ���Ͻ�Ǯ����(�󶨱�)");		
		}
	}
	catch (...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "��ӽ�ɫ���Ͻ�Ǯ����(�󶨱�)");	
	}
	
	m_operXDJG.GmOperLog(userByID, ServerIP, "XDJG��ӽ�ɫ���Ͻ�Ǯ����(�󶨱�)", retMessage, DBTFLV[0]);
	return DBTFLV;
}

//��ӽ�ɫ���Ͻ�Ǯ����(�ǰ󶨱�)
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_Net_AddRoleNonBind(int userByID, char *ServerIP, char *Character, int Money)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	
	string strResult;
	vector <string> vectorColInfo;
	char strConn[512];
	ZeroMemory(strConn, 512);
	char retMessage[255];
	ZeroMemory(retMessage, 255);
	
	try
	{
		sprintf(retMessage, "��ӽ�ɫ���Ͻ�Ǯ����(�ǰ󶨱�):<����>%s, <��ɫ>%s, <���Ǯ��>%d", ServerIP, Character, Money);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=add_money(%s,%d)", 
			ServerIP, HTTPPORT,
			Character,Money);
		
		if(m_operXDJG.getWebInfo(strConn, strResult))
		{
			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "��ӽ�ɫ���Ͻ�Ǯ����(�ǰ󶨱�)");				
			}
			else
			{
				int iResult = atoi(strResult.c_str());
				if (iResult>=0)
				{
					
					bool bFinishFlag = false;
					int Maxnum_wait = 10, wait_num=0;
					
					while (!bFinishFlag)
					{
						sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=get_operation_result(%d)", 
							ServerIP, HTTPPORT,
							iResult);
						m_operXDJG.getWebInfo(strConn, strResult);
						int iResultFlag = atoi(strResult.c_str());
						switch (iResultFlag)
						{
						case 1:
							bFinishFlag = true;
							
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ӽ�Ǯ�ɹ�(�ǰ󶨱�)", "��ӽ�ɫ���Ͻ�Ǯ����(�ǰ󶨱�)");
							
							break;
						case -2:
							wait_num++;
							if (wait_num > Maxnum_wait)
							{
								bFinishFlag = true; 
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ӽ�Ǯ��ʱ(�ǰ󶨱�)", "��ӽ�ɫ���Ͻ�Ǯ����(�ǰ󶨱�)");
								
							}
							Sleep(1000);
							break;
						default:
							bFinishFlag = true;
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ӽ�Ǯʧ��(�ǰ󶨱�)", "��ӽ�ɫ���Ͻ�Ǯ����(�ǰ󶨱�)");
						}
					}
				}
				else
				{
					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "��ӽ�ɫ���Ͻ�Ǯ����(�ǰ󶨱�)");
				}
			}
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "��ӽ�ɫ���Ͻ�Ǯ����(�ǰ󶨱�)");		
		}
	}
	catch (...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "��ӽ�ɫ���Ͻ�Ǯ����(�ǰ󶨱�)");	
	}
	
	m_operXDJG.GmOperLog(userByID, ServerIP, "XDJG��ӽ�ɫ���Ͻ�Ǯ����(�ǰ󶨱�)", retMessage, DBTFLV[0]);
	return DBTFLV;
}

//��ʱû��
string CXDJGInfo::GetSqlCommend(char *sqlType)
{
	CString strSql = "";
	string szSql = "";
	vector<CGlobalStruct::DataRecord> datarecord;

	try
	{
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
		sprintf(strSql.GetBuffer(0),"exec GetSql '%s' ", sqlType);
		//��ӡSQL
		m_SqlHelper.SqlHelperMain("XDJG",&datarecord,"127.0.0.1", strSql.GetBuffer(0), 4, 0, 0);//��ȡSqlExpress�е���Ӧsql���
		
		if(datarecord.empty())//�����¼Ϊ��
		{
			return szSql;
		}
		
		//��ȡ��ѯ�ļ�¼����ÿһ��
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;	
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("sql_statement", m_DataRecord.recordfieldname))//��ȡ��Ӧ��LogDBIP
			{
				szSql = m_DataRecord.recorddata;
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
		logFile.WriteErrorLog("XDJG",errMessage);
		return szSql;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ȡSQL����ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("XDJG",errMessage);
		return szSql;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ȡSQL��������쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("XDJG",errMessage);
		return szSql;
	}
	return szSql;
}

// vector <CGlobalStruct::TFLV> CXDJGInfo::CombinationTflv(St_ResCont &sResCont, vector <CGlobalStruct::TFLV> &DBTFLV)
// {
// 	vector <CGlobalStruct::TFLV> New_DbTflv;
// 	vector <CGlobalStruct::TFLV>::iterator pDbTflv;
// 	CGlobalStruct::TFLV TempDbTflv;
// 	CString szData = "";
// 	TempDbTflv.nIndex = 0;
// 	int i = 0;
// 
// 	if(DBTFLV.size() == 0 || DBTFLV.size() == 1)
// 		return DBTFLV;
// 
// 	for(pDbTflv = DBTFLV.begin(); pDbTflv != DBTFLV.end(); pDbTflv++)
// 	{
// 		if( pDbTflv->m_tagName == CEnumCore::TagName::XDJG_ItemID  ||
// 			pDbTflv->m_tagName == CEnumCore::TagName::XDJG_ItemID1 ||
// 			pDbTflv->m_tagName == CEnumCore::TagName::XDJG_ItemID2 ||
// 			pDbTflv->m_tagName == CEnumCore::TagName::XDJG_ItemID3 ||
// 			pDbTflv->m_tagName == CEnumCore::TagName::XDJG_ItemID4 ||
// 			pDbTflv->m_tagName == CEnumCore::TagName::XDJG_ItemID5 ||
// 			pDbTflv->m_tagName == CEnumCore::TagName::XDJG_ItemID6 ||
// 			pDbTflv->m_tagName == CEnumCore::TagName::XDJG_ItemID7 ||
// 			pDbTflv->m_tagName == CEnumCore::TagName::XDJG_ItemID8 )
// 		{
// 			i = 0;
// 			int iItemId = atoi((char*)pDbTflv->lpdata);
// 			New_DbTflv.push_back(*pDbTflv);
// 
// 			while(TRUE)
// 			{
// 				if(iItemId == sResCont.res_list[i].itemId && iItemId != 0 && sResCont.res_list[i].itemId != 0)
// 				{
// 					//�Ƿ��
// 					TempDbTflv.nIndex = New_DbTflv.size() + 1;
// 					TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
// 					TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_IsBind;
// 					switch(sResCont.res_list[i].property.safeFlags & 0xFF)
// 					{
// 					case SAFEFLAG:
// 						TempDbTflv.m_tvlength = strlen("��") + 1;
// 						sprintf((char*)&TempDbTflv.lpdata, "%s", "��");
// 						break;
// 					default:
// 						TempDbTflv.m_tvlength = strlen("��") + 1;
// 						sprintf((char*)&TempDbTflv.lpdata, "%s", "��");
// 						break;
// 					}
// 					New_DbTflv.push_back(TempDbTflv);
// 
// 					//�Ƿ�Ϊ����װ��
// 					TempDbTflv.nIndex  = New_DbTflv.size() + 1;
// 					TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
// 					TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_QualityEquip;
// 					if((sResCont.res_list[i].property.itemFlags & ITEMFLAG) == ITEMFLAG)
// 					{
// 						if((sResCont.res_list[i].property.itemFlags & SAFEFLAG) == SAFEFLAG)
// 						{
// 							TempDbTflv.m_tvlength = strlen("��") + 1;
// 							sprintf((char*)&TempDbTflv.lpdata, "%s", "��");
// 						}
// 						else
// 						{
// 							TempDbTflv.m_tvlength = strlen("��") + 1;
// 							sprintf((char*)&TempDbTflv.lpdata, "%s", "��");
// 						}
// 
// 					}
// 					else
// 					{
// 						TempDbTflv.m_tvlength = strlen("δ����") + 1;
// 						sprintf((char*)&TempDbTflv.lpdata, "%s", "δ����");
// 					}
// 					New_DbTflv.push_back(TempDbTflv);
// 
// 					//��������
// 					szData.Format("%i", sResCont.res_list[i].property.usingTime);
// 					TempDbTflv.nIndex  = New_DbTflv.size() + 1;
// 					TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
// 					TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_SuperpositionNum;
// 					TempDbTflv.m_tvlength = szData.GetLength()+1;
// 					sprintf((char*)&TempDbTflv.lpdata, "%s", szData.GetBuffer(0));
// 					New_DbTflv.push_back(TempDbTflv);
// 
// 					//����ʱ��
// 					szData.Format("%i", sResCont.res_list[i].property.createTime);
// 					TempDbTflv.nIndex  = New_DbTflv.size() + 1;
// 					TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
// 					TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_CreateTime;
// 					TempDbTflv.m_tvlength = szData.GetLength()+1;
// 					sprintf((char*)&TempDbTflv.lpdata, "%s", szData.GetBuffer(0));
// 					New_DbTflv.push_back(TempDbTflv);
// 
// 					//�;�ֵ
// 					szData.Format("%i", sResCont.res_list[i].property.abrasable);
// 					TempDbTflv.nIndex  = New_DbTflv.size() + 1;
// 					TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
// 					TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_DurableValue;
// 					TempDbTflv.m_tvlength = szData.GetLength()+1;
// 					sprintf((char*)&TempDbTflv.lpdata, "%s", szData.GetBuffer(0));
// 					New_DbTflv.push_back(TempDbTflv);
// 
// 					//��������
// 					TempDbTflv.nIndex  = New_DbTflv.size() + 1;
// 					TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
// 					TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_IncreaseType;
// 					switch(sResCont.res_list[i].property.strengthenType)
// 					{
// 					case 0:
// 						TempDbTflv.m_tvlength = strlen("û��") + 1;
// 						sprintf((char*)&TempDbTflv.lpdata, "%s", "û��");
// 						break;
// 					case 1:
// 						TempDbTflv.m_tvlength = strlen("����") + 1;
// 						sprintf((char*)&TempDbTflv.lpdata, "%s", "����");
// 						break;
// 					case 2:
// 						TempDbTflv.m_tvlength = strlen("����") + 1;
// 						sprintf((char*)&TempDbTflv.lpdata, "%s", "����");
// 						break;
// 					case 3:
// 						TempDbTflv.m_tvlength = strlen("����") + 1;
// 						sprintf((char*)&TempDbTflv.lpdata, "%s", "����");
// 						break;
// 					case 4:
// 						TempDbTflv.m_tvlength = strlen("����") + 1;
// 						sprintf((char*)&TempDbTflv.lpdata, "%s", "����");
// 						break;
// 					case 5:
// 						TempDbTflv.m_tvlength = strlen("����") + 1;
// 						sprintf((char*)&TempDbTflv.lpdata, "%s", "����");
// 						break;
// 					default:
// 						TempDbTflv.m_tvlength = strlen("δ֪") + 1;
// 						sprintf((char*)&TempDbTflv.lpdata, "%s", "δ֪");
// 						break;
// 					}
// 					New_DbTflv.push_back(TempDbTflv);
// 
// 					//���һ����������ֵ
// 					szData.Format("%i", sResCont.res_list[i].property.strengthenLastTimeQuantity);
// 					TempDbTflv.nIndex  = New_DbTflv.size() + 1;
// 					TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
// 					TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_IncreaseEndValue;
// 					TempDbTflv.m_tvlength = szData.GetLength()+1;
// 					sprintf((char*)&TempDbTflv.lpdata, "%s", szData.GetBuffer(0));
// 					New_DbTflv.push_back(TempDbTflv);
// 
// 					//��������
// 					szData.Format("%i", sResCont.res_list[i].property.strengthenEffect);
// 					TempDbTflv.nIndex  = New_DbTflv.size() + 1;
// 					TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
// 					TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_IncreaseNum;
// 					TempDbTflv.m_tvlength = szData.GetLength()+1;
// 					sprintf((char*)&TempDbTflv.lpdata, "%s", szData.GetBuffer(0));
// 					New_DbTflv.push_back(TempDbTflv);
// 
// 					//װ����װID
// 					szData.Format("%i", sResCont.res_list[i].property.compositingEffect);
// 					TempDbTflv.nIndex  = New_DbTflv.size() + 1;
// 					TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
// 					TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_AssemblyID;
// 					TempDbTflv.m_tvlength = szData.GetLength()+1;
// 					sprintf((char*)&TempDbTflv.lpdata, "%s", szData.GetBuffer(0));
// 					New_DbTflv.push_back(TempDbTflv);
// 					
// 					//��������ֵ
// 					szData.Format("%i", sResCont.res_list[i].property.strengthenQuantity);
// 					TempDbTflv.nIndex  = New_DbTflv.size() + 1;
// 					TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
// 					TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_IncreaseValue;
// 					TempDbTflv.m_tvlength = szData.GetLength()+1;
// 					sprintf((char*)&TempDbTflv.lpdata, "%s", szData.GetBuffer(0));
// 					New_DbTflv.push_back(TempDbTflv);
// 
// 					//װ����Ƕ��ʯ��ID1
// 					szData.Format("%i", sResCont.res_list[i].property.equipSlotEffect1);
// 					TempDbTflv.nIndex  = New_DbTflv.size() + 1;
// 					TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
// 					TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_EquipSlotID1;
// 					TempDbTflv.m_tvlength = szData.GetLength()+1;
// 					sprintf((char*)&TempDbTflv.lpdata, "%s", szData.GetBuffer(0));
// 					New_DbTflv.push_back(TempDbTflv);
// 					
// 					//װ����Ƕ��ʯ��ID2
// 					szData.Format("%i", sResCont.res_list[i].property.equipSlotEffect2);
// 					TempDbTflv.nIndex  = New_DbTflv.size() + 1;
// 					TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
// 					TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_EquipSlotID2;
// 					TempDbTflv.m_tvlength = szData.GetLength()+1;
// 					sprintf((char*)&TempDbTflv.lpdata, "%s", szData.GetBuffer(0));
// 					New_DbTflv.push_back(TempDbTflv);
// 
// 					//װ����Ƕ��ʯ��ID3
// 					szData.Format("%i", sResCont.res_list[i].property.equipSlotEffect3);
// 					TempDbTflv.nIndex  = New_DbTflv.size() + 1;
// 					TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
// 					TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_EquipSlotID3;
// 					TempDbTflv.m_tvlength = szData.GetLength()+1;
// 					sprintf((char*)&TempDbTflv.lpdata, "%s", szData.GetBuffer(0));
// 					New_DbTflv.push_back(TempDbTflv);
// 					
// 					//����Ϊ������(�Ǽ�)
// 					szData.Format("%i", (sResCont.res_list[i].property.equipSlotEffect3 & STARLEVEL));
// 					TempDbTflv.nIndex  = New_DbTflv.size() + 1;
// 					TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
// 					TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_StarsLevel;
// 					TempDbTflv.m_tvlength = szData.GetLength()+1;
// 					sprintf((char*)&TempDbTflv.lpdata, "%s", szData.GetBuffer(0));
// 					New_DbTflv.push_back(TempDbTflv);
// 
// 					//�������ֵ
// 					szData.Format("%i", (sResCont.res_list[i].property.safeFlags & 0x0000FFFF));
// 					TempDbTflv.nIndex = New_DbTflv.size() + 1;
// 					TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
// 					TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_RandomProperty;
// 					TempDbTflv.m_tvlength = szData.GetLength()+1;
// 					sprintf((char*)&TempDbTflv.lpdata, "%s", szData.GetBuffer(0));
// 					New_DbTflv.push_back(TempDbTflv);
// 
// 					break;
// 				}
// 				else if(iItemId == 0 || sResCont.res_list[i].itemId == 0)
// 					break;
// 				else
// 				{
// 					i++;
// 					continue;
// 				}
// 			}
// 		}
// 		else
// 		{
// 			if(TempDbTflv.nIndex != 0)
// 				pDbTflv->nIndex = New_DbTflv.size() + 1;
// 			New_DbTflv.push_back(*pDbTflv);
// 			continue;
// 		}
// 	}
// 
// 	return New_DbTflv;
// }

//��ɫ������Ϣ��ѯ
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_RoleSkills_Query(char *ServerIP, char *Account, char *Character, int iIndex, int iPageSize)
{
	CGlobalStruct::TFLV m_tflv;
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	CString strData = "";
	char realServerIP[40];
	int iCount = 0;
	int iNum = 0;
	vector<CGlobalStruct::DataRecord> datarecord;
	char retMessage[255];
	ZeroMemory(retMessage,255);
	
	try
	{
		sprintf(retMessage, "��ɫ������Ϣ��ѯ:<����>%s, <�ʺ�>%s,<��ɫ>%s", ServerIP, Account, Character);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,3))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "��ɫ������Ϣ��ѯ");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		strSql.Format("Select skills AS XDJG_Skills from %s.character \
							  where name = '%s'" ,\
							  GameDB,Character);
		
		m_SqlHelper.SqlHelperMain("XDJG", &datarecord,realServerIP,strSql.GetBuffer(0),12,0,0);
		
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("XDJG_Skills", m_DataRecord.recordfieldname))
			{
				memcpy(&m_StructInfo.m_sSkillCont, m_DataRecord.recorddata, sizeof(m_StructInfo.m_sSkillCont));
				break;
			}
			else
				continue;
		}

		int iPageCount = 0;
		printf("���ӵ�м��ܸ���:%d\n",m_StructInfo.m_sSkillCont.skill_count);
		int iLogInfoLen = m_StructInfo.m_sSkillCont.skill_count;
		
		if ( iLogInfoLen%iPageSize == 0 )
		{
			iPageCount = iLogInfoLen/iPageSize;
		}
		else
		{
			iPageCount = iLogInfoLen/iPageSize + 1;
		}
		
		for( iCount = iPageSize*(iIndex-1); iCount < iLogInfoLen && iCount < iPageSize*iIndex; iCount++ )
		{
			//����ʺ�
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Account;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(Account) + 1;
			sprintf((char *)&m_tflv.lpdata,"%s",Account);
			DBTFLV.push_back(m_tflv);
			
			//��ɫ��
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Character;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(Character) + 1;
			sprintf((char *)&m_tflv.lpdata,"%s",Character);
			DBTFLV.push_back(m_tflv);

			//����ID
			strData.Format("%d", m_StructInfo.m_sSkillCont.skill_list[iCount]);
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_SkillID;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strData.GetLength()+1;
			sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//��������
			if(strData.GetLength() == 5)
			{
				string strTemp = strData.GetBuffer(0);
				string strSkillName = GetSkillName(atoi(strTemp.substr(0,3).c_str()), 1);
				strData.Format("(�ȼ�%d)",atoi(strTemp.substr(3,6).c_str()));
				strSkillName += strData.GetBuffer(0);
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_SkillName;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strSkillName.length()+1;
				sprintf((char *)&m_tflv.lpdata,"%s",strSkillName.c_str());
				DBTFLV.push_back(m_tflv);
			}
			else
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_SkillName;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=GetSkillName(m_StructInfo.m_sSkillCont.skill_list[iCount], 2).length()+1;
				sprintf((char *)&m_tflv.lpdata,"%s",GetSkillName(m_StructInfo.m_sSkillCont.skill_list[iCount], 2).c_str());
				DBTFLV.push_back(m_tflv);
			}

			//PageCount
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PageCount;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((char *)&m_tflv.lpdata,"%d", iPageCount);
			DBTFLV.push_back(m_tflv);
		}
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "��ɫ������Ϣ��ѯ");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	return DBTFLV;
}

//��ȡ��������
string CXDJGInfo::GetSkillName(int SkillId, int iGetType)
{
	string strData = "";
	CString strSql = "";
	CString szTemp = "";
	vector<CGlobalStruct::DataRecord> datarecord;

	try
	{
		if(0 == SkillId)
			return "";

		if(1 == iGetType)
		{
			szTemp.Format("%d%s", SkillId, "01");
			strSql.Format("select SkillName from XDJG_SkillList where SkillId = %d", atoi(szTemp.GetBuffer(0)));
		}
		else
			strSql.Format("select SkillName from XDJG_SkillList where SkillId = %d", SkillId);
		
		m_SqlHelper.SqlHelperMain("XDJG", &datarecord,"127.0.0.1",strSql.GetBuffer(0),4,0,0);

		if(datarecord.empty())
			return "";

		vector<CGlobalStruct::DataRecord>::iterator pdatarecord = NULL;
		for(pdatarecord = datarecord.begin(); pdatarecord != datarecord.end(); pdatarecord++)
		{
			if(!strcmp("SkillName", pdatarecord->recordfieldname))
			{
				strData = pdatarecord->recorddata;
				break;
			}
			else
				continue;
		}
	}
	catch (...)
	{
		return "";
	}

	return strData;
	
}

//�����б��ѯ
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_SkillList_Query(char *GameName, char *Sql, char *Profession)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	try
	{
		DBTFLV=GameDBQuery(GameName,"127.0.0.1",4,0,0,Sql,Profession);
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog(GameName,"�����б��ѯ�ڴ治��");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog(GameName,"�����б��ѯ�ļ��쳣");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog(GameName,"�����б��ѯ�����쳣");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog(GameName,(char *)err.what());
		return DBTFLV;
	}
	return DBTFLV;
}

//�������Ͳ�ѯ
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_ItemType_Query(char *GameName, char *Sql)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	try
	{
		DBTFLV=GameDBQuery(GameName,"127.0.0.1",4,0,0,Sql);
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog(GameName,"�������Ͳ�ѯ�ڴ治��");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog(GameName,"�������Ͳ�ѯ�ļ��쳣");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog(GameName,"�������Ͳ�ѯ�����쳣");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog(GameName,(char *)err.what());
		return DBTFLV;
	}
	return DBTFLV;
}

//��Ӧ���͵ĵ����б��ѯ
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_ItemNameLst_Query(char *GameName, char *Sql, char *ItemType, char *ItemName, int index, int pagesize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	try
	{
		printf("type:%s, name:%s\n",ItemType, ItemName);
		DBTFLV=GameDBQuery(GameName,"127.0.0.1",4,index,pagesize,Sql,ItemType, ItemName);
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog(GameName,"�����б��ѯ�ڴ治��");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog(GameName,"�����б��ѯ�ļ��쳣");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog(GameName,"�����б��ѯ�����쳣");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog(GameName,(char *)err.what());
		return DBTFLV;
	}
	return DBTFLV;
}

vector <CGlobalStruct::TFLV> CXDJGInfo::CombinationTflv(St_ResCont &sResCont, vector <string> &VecItem)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	vector <string>::iterator pItemID = NULL;
	vector <st_ItemInfo>::iterator pItemInfo = NULL;
	CGlobalStruct::TFLV TempDbTflv;
	vector<int> vec_Itemid;
	vector<int>::iterator pID = NULL;
	CString szData = "";
	int i = 0;
	Vec_ItemInfo VecItemInfo;

	if(VecItem.size() == 0)
		return DBTFLV;

	//ȡ��ǰ���ϵĵ���id
	for(i = 0; i < sResCont.limit_character_res_num; i++)
	{
		if(sResCont.res_list[i].itemId != 0)
			vec_Itemid.push_back(sResCont.res_list[i].itemId);
		else
			continue;
	}

	//ȡ��Ʒ����
	GetItemInfo(VecItemInfo);

	for(pItemID = VecItem.begin(); pItemID != VecItem.end(); pItemID++)
	{
		int iItemId = atoi(pItemID->c_str());

		pID = vec_Itemid.begin();
		while(pID != vec_Itemid.end())
		{
			if(iItemId == *pID && iItemId != 0 && *pID != 0)
			{
				//��ƷID
				szData.Format("%i", iItemId);
				TempDbTflv.nIndex = DBTFLV.size() + 1;
				TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
				TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_ItemID;
				TempDbTflv.m_tvlength = szData.GetLength()+1;
				sprintf((char*)&TempDbTflv.lpdata, "%s", szData.GetBuffer(0));
				DBTFLV.push_back(TempDbTflv);

				//��Ʒ����
				for(pItemInfo = VecItemInfo.begin(); pItemInfo != VecItemInfo.end(); pItemInfo++)
				{
					if(pItemInfo->iItemId == iItemId)
					{
						TempDbTflv.nIndex = DBTFLV.size() + 1;
						TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_ItemName;
						TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
						TempDbTflv.m_tvlength = pItemInfo->strItemName.size();
						memcpy(&TempDbTflv.lpdata, pItemInfo->strItemName.c_str(), pItemInfo->strItemName.size());
						DBTFLV.push_back(TempDbTflv);
						break;
					}
					else
						continue;
				}

				if(TempDbTflv.m_tagName != CEnumCore::TagName::XDJG_ItemName)
				{
					TempDbTflv.nIndex = DBTFLV.size() + 1;
					TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_ItemName;
					TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					TempDbTflv.m_tvlength = strlen("")+1;
					sprintf((char*)&TempDbTflv.lpdata, "%s", "");
					DBTFLV.push_back(TempDbTflv);
				}

				//�Ƿ��
				TempDbTflv.nIndex = DBTFLV.size() + 1;
				TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
				TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_IsBind;
				switch(sResCont.res_list[i].property.safeFlags & 0xFF)
				{
				case SAFEFLAG:
					TempDbTflv.m_tvlength = strlen("��") + 1;
					sprintf((char*)&TempDbTflv.lpdata, "%s", "��");
					break;
				default:
					TempDbTflv.m_tvlength = strlen("��") + 1;
					sprintf((char*)&TempDbTflv.lpdata, "%s", "��");
					break;
				}
				DBTFLV.push_back(TempDbTflv);

				//�Ƿ�Ϊ����װ��
				TempDbTflv.nIndex  = DBTFLV.size() + 1;
				TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
				TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_QualityEquip;
				if((sResCont.res_list[i].property.itemFlags & ITEMFLAG) == ITEMFLAG)
				{
					if((sResCont.res_list[i].property.itemFlags & SAFEFLAG) == SAFEFLAG)
					{
						TempDbTflv.m_tvlength = strlen("��") + 1;
						sprintf((char*)&TempDbTflv.lpdata, "%s", "��");
					}
					else
					{
						TempDbTflv.m_tvlength = strlen("��") + 1;
						sprintf((char*)&TempDbTflv.lpdata, "%s", "��");
					}

				}
				else
				{
					TempDbTflv.m_tvlength = strlen("δ����") + 1;
					sprintf((char*)&TempDbTflv.lpdata, "%s", "δ����");
				}
				DBTFLV.push_back(TempDbTflv);

				//��������
				szData.Format("%i", sResCont.res_list[i].property.usingTime);
				TempDbTflv.nIndex  = DBTFLV.size() + 1;
				TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
				TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_SuperpositionNum;
				TempDbTflv.m_tvlength = szData.GetLength()+1;
				sprintf((char*)&TempDbTflv.lpdata, "%s", szData.GetBuffer(0));
				DBTFLV.push_back(TempDbTflv);

				//����ʱ��
				szData.Format("%i", sResCont.res_list[i].property.createTime);
				TempDbTflv.nIndex  = DBTFLV.size() + 1;
				TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
				TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_CreateTime;
				TempDbTflv.m_tvlength = szData.GetLength()+1;
				sprintf((char*)&TempDbTflv.lpdata, "%s", szData.GetBuffer(0));
				DBTFLV.push_back(TempDbTflv);

				//�;�ֵ
				szData.Format("%i", sResCont.res_list[i].property.abrasable);
				TempDbTflv.nIndex  = DBTFLV.size() + 1;
				TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
				TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_DurableValue;
				TempDbTflv.m_tvlength = szData.GetLength()+1;
				sprintf((char*)&TempDbTflv.lpdata, "%s", szData.GetBuffer(0));
				DBTFLV.push_back(TempDbTflv);

				//��������
				TempDbTflv.nIndex  = DBTFLV.size() + 1;
				TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
				TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_IncreaseType;
				switch(sResCont.res_list[i].property.strengthenType)
				{
				case 0:
					TempDbTflv.m_tvlength = strlen("û��") + 1;
					sprintf((char*)&TempDbTflv.lpdata, "%s", "û��");
					break;
				case 1:
					TempDbTflv.m_tvlength = strlen("����") + 1;
					sprintf((char*)&TempDbTflv.lpdata, "%s", "����");
					break;
				case 2:
					TempDbTflv.m_tvlength = strlen("����") + 1;
					sprintf((char*)&TempDbTflv.lpdata, "%s", "����");
					break;
				case 3:
					TempDbTflv.m_tvlength = strlen("����") + 1;
					sprintf((char*)&TempDbTflv.lpdata, "%s", "����");
					break;
				case 4:
					TempDbTflv.m_tvlength = strlen("����") + 1;
					sprintf((char*)&TempDbTflv.lpdata, "%s", "����");
					break;
				case 5:
					TempDbTflv.m_tvlength = strlen("����") + 1;
					sprintf((char*)&TempDbTflv.lpdata, "%s", "����");
					break;
				default:
					TempDbTflv.m_tvlength = strlen("δ֪") + 1;
					sprintf((char*)&TempDbTflv.lpdata, "%s", "δ֪");
					break;
				}
				DBTFLV.push_back(TempDbTflv);

				//���һ����������ֵ
				szData.Format("%i", sResCont.res_list[i].property.strengthenLastTimeQuantity);
				TempDbTflv.nIndex  = DBTFLV.size() + 1;
				TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
				TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_IncreaseEndValue;
				TempDbTflv.m_tvlength = szData.GetLength()+1;
				sprintf((char*)&TempDbTflv.lpdata, "%s", szData.GetBuffer(0));
				DBTFLV.push_back(TempDbTflv);

				//��������
				szData.Format("%i", sResCont.res_list[i].property.strengthenEffect);
				TempDbTflv.nIndex  = DBTFLV.size() + 1;
				TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
				TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_IncreaseNum;
				TempDbTflv.m_tvlength = szData.GetLength()+1;
				sprintf((char*)&TempDbTflv.lpdata, "%s", szData.GetBuffer(0));
				DBTFLV.push_back(TempDbTflv);

				//װ����װID
				szData.Format("%i", sResCont.res_list[i].property.compositingEffect);
				TempDbTflv.nIndex  = DBTFLV.size() + 1;
				TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
				TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_AssemblyID;
				TempDbTflv.m_tvlength = szData.GetLength()+1;
				sprintf((char*)&TempDbTflv.lpdata, "%s", szData.GetBuffer(0));
				DBTFLV.push_back(TempDbTflv);
				
				//��������ֵ
				szData.Format("%i", sResCont.res_list[i].property.strengthenQuantity);
				TempDbTflv.nIndex  = DBTFLV.size() + 1;
				TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
				TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_IncreaseValue;
				TempDbTflv.m_tvlength = szData.GetLength()+1;
				sprintf((char*)&TempDbTflv.lpdata, "%s", szData.GetBuffer(0));
				DBTFLV.push_back(TempDbTflv);

				//װ����Ƕ��ʯ��ID1
				szData.Format("%i", sResCont.res_list[i].property.equipSlotEffect1);
				TempDbTflv.nIndex  = DBTFLV.size() + 1;
				TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
				TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_EquipSlotID1;
				TempDbTflv.m_tvlength = szData.GetLength()+1;
				sprintf((char*)&TempDbTflv.lpdata, "%s", szData.GetBuffer(0));
				DBTFLV.push_back(TempDbTflv);
				
				//װ����Ƕ��ʯ��ID2
				szData.Format("%i", sResCont.res_list[i].property.equipSlotEffect2);
				TempDbTflv.nIndex  = DBTFLV.size() + 1;
				TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
				TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_EquipSlotID2;
				TempDbTflv.m_tvlength = szData.GetLength()+1;
				sprintf((char*)&TempDbTflv.lpdata, "%s", szData.GetBuffer(0));
				DBTFLV.push_back(TempDbTflv);

				//װ����Ƕ��ʯ��ID3
				szData.Format("%i", sResCont.res_list[i].property.equipSlotEffect3);
				TempDbTflv.nIndex  = DBTFLV.size() + 1;
				TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
				TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_EquipSlotID3;
				TempDbTflv.m_tvlength = szData.GetLength()+1;
				sprintf((char*)&TempDbTflv.lpdata, "%s", szData.GetBuffer(0));
				DBTFLV.push_back(TempDbTflv);
				
				//����Ϊ������(�Ǽ�)
				szData.Format("%i", (sResCont.res_list[i].property.equipSlotEffect3 & STARLEVEL));
				TempDbTflv.nIndex  = DBTFLV.size() + 1;
				TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
				TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_StarsLevel;
				TempDbTflv.m_tvlength = szData.GetLength()+1;
				sprintf((char*)&TempDbTflv.lpdata, "%s", szData.GetBuffer(0));
				DBTFLV.push_back(TempDbTflv);

				//�������ֵ
				szData.Format("%i", (sResCont.res_list[i].property.safeFlags & 0x0000FFFF));
				TempDbTflv.nIndex = DBTFLV.size() + 1;
				TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
				TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_RandomProperty;
				TempDbTflv.m_tvlength = szData.GetLength()+1;
				sprintf((char*)&TempDbTflv.lpdata, "%s", szData.GetBuffer(0));
				DBTFLV.push_back(TempDbTflv);

				//PageCount
				TempDbTflv.nIndex=DBTFLV.size()+1;
				TempDbTflv.m_tagName=CEnumCore::TagName::PageCount;
				TempDbTflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				TempDbTflv.m_tvlength=sizeof(int);
				sprintf((char *)&TempDbTflv.lpdata,"%d", 1);
				DBTFLV.push_back(TempDbTflv);

				break;
			}
			else if(iItemId == 0 || *pID == 0)
				break;
			else
			{
				pID++;
				continue;
			}
		}
	}

	if(DBTFLV.size() == 0)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ҵ�ǰû�иõ���", "��Ʒ����");
	}

	return DBTFLV;
}

//��Ʒ���Բ�ѯ
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_ItemAttribute_Query(char *ServerIP, char *Character, char *ItemId)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	char gameDBServerIP[40];
	CString strItemID = "";
	vector<CGlobalStruct::DataRecord> datarecord;
	vector<string> Vec_ItemId;
	char *pTemp = NULL;
	char retMessage[255];
	ZeroMemory(retMessage,255);
	
	try
	{
		sprintf(retMessage, "��Ʒ���Բ�ѯ:<����>%s, <��ɫ>%s, <����id>%s", ServerIP, Character, ItemId);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		//��ȡGameDB��ip
		if(!m_operXDJG.GetServerIP(gameDBServerIP,ServerIP,3))
		{
			return DBTFLV;//�޷���ȡGameDB��IP
		}
		
		//ȡ����������Ʒ�����ֶ�
		strSql.Format("select items AS XDJG_Items \
						 from %s.character \
						where name = '%s' ",GameDB, Character);
		
		m_SqlHelper.SqlHelperMain("XDJG", &datarecord,gameDBServerIP,strSql.GetBuffer(0),12,0,0);
		
		if(datarecord.empty())
			return DBTFLV;

		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("XDJG_Items", m_DataRecord.recordfieldname))
			{
				memcpy(&m_StructInfo.m_sResCont , m_DataRecord.recorddata, sizeof(m_StructInfo.m_sResCont));//��Ʒ����
				break;
			}
			else
				continue;
		}

		pTemp = strtok(ItemId, ";");
		while(pTemp != NULL)
		{
			Vec_ItemId.push_back(pTemp);
			pTemp = strtok(NULL, ";");
		}

		if(Vec_ItemId.empty())
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡ����ID����", "��Ʒ����");
			return DBTFLV;
		}

		DBTFLV = CombinationTflv(m_StructInfo.m_sResCont, Vec_ItemId);
	}
	catch (...)
	{
		return DBTFLV;
	}

	return DBTFLV;
}

//������Ϣ��ѯ
//���������Ϣ
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_GuildBaseInfo_Query(char *ServerIP, char *GuildName)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	char realServerIP[40];
	vector<CGlobalStruct::DataRecord> datarecord;
	char retMessage[255];
	ZeroMemory(retMessage,255);
	
	try
	{
		sprintf(retMessage, "���������Ϣ��ѯ:<����>%s, <��������>%s", ServerIP, GuildName);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,3))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "���������Ϣ��ѯ");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		strSql.Format("Select guildid AS XDJG_GuildID, '%s' AS XDJG_GuildName, \
							  level AS XDJG_GuildLevel, FROM_UNIXTIME(createdate) AS XDJG_CreateTime, \
							  (select name from %s.family_member where charindex = leaderguid) AS XDJG_GuildLeader \
							  from %s.family \
							  where name = '%s'", \
							  GuildName,GameDB,GameDB,GuildName);

		DBTFLV = GameDBQuery("XDJG",realServerIP,12, 0, 0, strSql.GetBuffer(0), GameDB, GameDB);
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "���������Ϣ��ѯ");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	return DBTFLV;
}

//�����Ա�б�
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_GuildMemberList_Query(char *ServerIP, char *GuildName)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	char realServerIP[40];
	vector<CGlobalStruct::DataRecord> datarecord;
	char retMessage[255];
	ZeroMemory(retMessage,255);
	
	try
	{
		sprintf(retMessage, "�����Ա�б��ѯ:<����>%s, <��������>%s", ServerIP, GuildName);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,3))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "�����Ա�б�");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		strSql.Format("SELECT a.guildid AS XDJG_GuildID, '%s' AS XDJG_GuildName, \
							  b.name AS XDJG_Guildmember, \
							  CASE b.charindex WHEN a.leaderguid THEN '�᳤' \
											   ELSE '��ͨ��Ա' END AS XDJG_GuildPosition,\
							  b.create_time AS XDJG_JoinTime \
						 FROM %s.family a, %s.family_member b \
						WHERE a.guid = b. familyid \
						  AND a.name = '%s'", \
							  GuildName,GameDB,GameDB,GuildName);
		
		DBTFLV = GameDBQuery("XDJG",realServerIP,12, 0, 0, strSql.GetBuffer(0), GameDB, GameDB);
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "�����Ա�б�");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	return DBTFLV;
}

//��������ѯ
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_PlayerTaskInfo_Query(char *ServerIP, char *Account, char *Character, int iIndex, int iPageSize)
{
	CGlobalStruct::TFLV m_tflv;
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	CString szData = "";
	char realServerIP[40];
	vector<CGlobalStruct::DataRecord> datarecord;
	vector<CGlobalStruct::DataRecord>::iterator pData = NULL;
	char retMessage[255];
	ZeroMemory(retMessage,255);
	
	try
	{
		sprintf(retMessage, "��������ѯ:<����>%s, <�ʺ�>%s, <��ɫ>%s", ServerIP, Account, Character);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,3))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "��������ѯ");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		strSql.Format("Select tasks from %s.character where name = '%s'",GameDB, Character);
		
		m_SqlHelper.SqlHelperMain("XDJG", &datarecord,realServerIP,strSql.GetBuffer(0),12,0,0);

		if(datarecord.empty())
			return DBTFLV;

		for(pData = datarecord.begin(); pData != datarecord.end(); pData++)
		{
			if(!strcmp(pData->recordfieldname, "tasks"))
			{
				memcpy(&m_StructInfo.m_sTaskCont, pData->recorddata, sizeof(m_StructInfo.m_sTaskCont));
				break;
			}
		}

		if(m_StructInfo.m_sTaskCont.task_count != 0)
		{
			//����ʺ�
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Account;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(Account) + 1;
			sprintf((char *)&m_tflv.lpdata,"%s",Account);
			DBTFLV.push_back(m_tflv);

			//��ɫ��
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Character;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(Character) + 1;
			sprintf((char *)&m_tflv.lpdata,"%s",Character);
			DBTFLV.push_back(m_tflv);

			//��������
			szData.Format("%i", m_StructInfo.m_sTaskCont.task_count);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_Task_Name;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);
			
			//����ID
			szData.Format("%i", m_StructInfo.m_sTaskCont.task_count);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_Task_ID;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//������
			szData.Format("%i", m_StructInfo.m_sTaskCont.task_count);
			m_tflv.nIndex = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_TaskStep;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//PageCount
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PageCount;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((char *)&m_tflv.lpdata,"%d", 1);
			DBTFLV.push_back(m_tflv);
		}
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "���������Ϣ��ѯ");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	return DBTFLV;
}

//��ճ�����Ϣ
//��ճ���������Ϣ
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_CarBaseInfo_Query(char *ServerIP, char *Account, char *Character, int iIndex, int iPageSize)
{
	CGlobalStruct::TFLV m_tflv;
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	CString strData = "";
	char realServerIP[40];
	vector<CGlobalStruct::DataRecord> datarecord;
	vector<CGlobalStruct::DataRecord>::iterator pData = NULL;
	vector<_Car> Vec_Car;
	int iCount = 0;
	int iNum = 0;
	char retMessage[255];
	ZeroMemory(retMessage,255);
	
	try
	{
		sprintf(retMessage, "��ճ���������Ϣ��ѯ:<����>%s, <�ʺ�>%s, <��ɫ>%s", ServerIP, Account, Character);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,3))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "��ճ���������Ϣ");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		strSql.Format("Select items AS XDJG_Items from %s.character where name = '%s'",GameDB, Character);
		
		m_SqlHelper.SqlHelperMain("XDJG", &datarecord,realServerIP,strSql.GetBuffer(0),12,0,0);
		
		if(datarecord.empty())
			return DBTFLV;
		
		for(pData = datarecord.begin(); pData != datarecord.end(); pData++)
		{
			if(!strcmp(pData->recordfieldname, "XDJG_Items"))
			{
				memcpy(&m_StructInfo.m_sResCont, pData->recorddata, sizeof(m_StructInfo.m_sResCont));
			}
		}
		
		//ȡ��ǰ���ϵĵ���
		for(int i = 0; i < m_StructInfo.m_sResCont.limit_character_car_num; i++)
		{
			if(m_StructInfo.m_sResCont.car_list[i].carId != 0)
			{
				Vec_Car.push_back(m_StructInfo.m_sResCont.car_list[i]);
			}
			else
				continue;
		}

		if(Vec_Car.size() == 0)
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�������û�н��", "��ճ���������Ϣ");
			return DBTFLV;
		}

		int iPageCount = 0;
		printf("***************%d\n",Vec_Car.size());
		int iLogInfoLen = Vec_Car.size();
		
		if ( iLogInfoLen%iPageSize == 0 )
		{
			iPageCount = iLogInfoLen/iPageSize;
		}
		else
		{
			iPageCount = iLogInfoLen/iPageSize + 1;
		}
		
		for ( iCount = iPageSize*(iIndex-1); iCount < iLogInfoLen && iCount < iPageSize*iIndex; iCount++ )
		{
			//����ʺ�
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Account;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(Account) + 1;
			sprintf((char *)&m_tflv.lpdata,"%s",Account);
			DBTFLV.push_back(m_tflv);

			//��ɫ��
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Character;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(Character) + 1;
			sprintf((char *)&m_tflv.lpdata,"%s",Character);
			DBTFLV.push_back(m_tflv);

			//���ID
			strData.Format("%d", Vec_Car[iCount].carId);
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_CarID;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strData.GetLength()+1;
			sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//�ȼ�
			strData.Format("%d", Vec_Car[iCount]._Level);
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Level;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strData.GetLength()+1;
			sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//�츳����ֵ
			strData.Format("%d", Vec_Car[iCount]._TranSkillStrID);
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_SkillStrValue;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strData.GetLength()+1;
			sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//��ǰ��ռ���ID
			strData.Format("%d", Vec_Car[iCount]._TranSkillID);
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_SkillID;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strData.GetLength()+1;
			sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			for(iNum = 0; iNum < 2; iNum++)
			{
				switch (iNum)
				{
				case 0:
					//ʱ��оƬIDs1
					strData.Format("%d", Vec_Car[iCount]._TranPluginSlots[iNum]);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_TimePlugSlot1;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=strData.GetLength()+1;
					sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
					DBTFLV.push_back(m_tflv);
					break;
				case 1:
					//ʱ��оƬIDs2
					strData.Format("%d", Vec_Car[iCount]._TranPluginSlots[iNum]);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_TimePlugSlot2;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=strData.GetLength()+1;
					sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
					DBTFLV.push_back(m_tflv);
					break;
				default:
					break;
				}
			}

			//PageCount
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PageCount;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((char *)&m_tflv.lpdata,"%d", iPageCount);
			DBTFLV.push_back(m_tflv);
			
		}

	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "��ճ���������Ϣ");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	return DBTFLV;
}

//��ճ�����ϸ��Ϣ
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_CarDetailInfo_Query(char *ServerIP, char *Account, char *Character, int iIndex, int iPageSize)
{
	CGlobalStruct::TFLV m_tflv;
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	CString strData = "";
	char realServerIP[40];
	vector<CGlobalStruct::DataRecord> datarecord;
	vector<CGlobalStruct::DataRecord>::iterator pData = NULL;
	vector<_Car> Vec_Car;
	int iCount = 0;
	int iNum = 0;
	char retMessage[255];
	ZeroMemory(retMessage,255);
	
	try
	{
		sprintf(retMessage, "��ճ�����ϸ��Ϣ��ѯ:<����>%s, <�ʺ�>%s, <��ɫ>%s", ServerIP, Account, Character);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,3))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "��ճ�����ϸ��Ϣ");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		strSql.Format("Select items AS XDJG_Items from %s.character where name = '%s'",GameDB, Character);
		
		m_SqlHelper.SqlHelperMain("XDJG", &datarecord,realServerIP,strSql.GetBuffer(0),12,0,0);
		
		if(datarecord.empty())
			return DBTFLV;
		
		for(pData = datarecord.begin(); pData != datarecord.end(); pData++)
		{
			if(!strcmp(pData->recordfieldname, "XDJG_Items"))
			{
				memcpy(&m_StructInfo.m_sResCont, pData->recorddata, sizeof(m_StructInfo.m_sResCont));
			}
		}
		
		//ȡ��ǰ���ϵĵ���
		for(int i = 0; i < m_StructInfo.m_sResCont.limit_character_car_num; i++)
		{
			if(m_StructInfo.m_sResCont.car_list[i].carId != 0)
			{
				Vec_Car.push_back(m_StructInfo.m_sResCont.car_list[i]);
			}
			else
				continue;
		}

		if(Vec_Car.size() == 0)
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�������û�н��", "��ճ�����ϸ��Ϣ");
			return DBTFLV;
		}

		int iPageCount = 0;
		printf("***************%d\n",Vec_Car.size());
		int iLogInfoLen = Vec_Car.size();
		
		if ( iLogInfoLen%iPageSize == 0 )
		{
			iPageCount = iLogInfoLen/iPageSize;
		}
		else
		{
			iPageCount = iLogInfoLen/iPageSize + 1;
		}
		
		for ( iCount = iPageSize*(iIndex-1); iCount < iLogInfoLen && iCount < iPageSize*iIndex; iCount++ )
		{
			//����ʺ�
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Account;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(Account) + 1;
			sprintf((char *)&m_tflv.lpdata,"%s",Account);
			DBTFLV.push_back(m_tflv);

			//��ɫ��
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Character;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(Character) + 1;
			sprintf((char *)&m_tflv.lpdata,"%s",Character);
			DBTFLV.push_back(m_tflv);

			//���ID
			strData.Format("%d", Vec_Car[iCount].carId);
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_CarID;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strData.GetLength()+1;
			sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//�ȼ�
			strData.Format("%d", Vec_Car[iCount]._Level);
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Level;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strData.GetLength()+1;
			sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//�츳����ֵ
			strData.Format("%d", Vec_Car[iCount]._TranSkillStrID);
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_SkillStrValue;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strData.GetLength()+1;
			sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//��ǰ��ռ���ID
			strData.Format("%d", Vec_Car[iCount]._TranSkillID);
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_SkillID;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strData.GetLength()+1;
			sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			for(iNum = 0; iNum < 2; iNum++)
			{
				switch (iNum)
				{
				case 0:
					//ʱ��оƬIDs1
					strData.Format("%d", Vec_Car[iCount]._TranPluginSlots[iNum]);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_TimePlugSlot1;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=strData.GetLength()+1;
					sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
					DBTFLV.push_back(m_tflv);
					break;
				case 1:
					//ʱ��оƬIDs2
					strData.Format("%d", Vec_Car[iCount]._TranPluginSlots[iNum]);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_TimePlugSlot2;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=strData.GetLength()+1;
					sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
					DBTFLV.push_back(m_tflv);
					break;
				default:
					break;
				}
			}

			//PageCount
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PageCount;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((char *)&m_tflv.lpdata,"%d", iPageCount);
			DBTFLV.push_back(m_tflv);
			
		}

	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "��ճ�����ϸ��Ϣ");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	return DBTFLV;
}

//��ҹ�����Ϣ
//��ҹ��������Ϣ
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_PlayerGuildBaseInfo_Query(char *ServerIP, char *Account, char *Character, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	char realServerIP[40];
	vector<CGlobalStruct::DataRecord> datarecord;
	char retMessage[255];
	ZeroMemory(retMessage,255);
	
	try
	{
		sprintf(retMessage, "��ҹ��������Ϣ:<����>%s, <�ʺ�>%s, <��ɫ>%s", ServerIP, Account, Character);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,3))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "��ҹ��������Ϣ");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		strSql.Format("SELECT '%s' AS XDJG_Account, '%s' AS XDJG_Character, \
							  a.name AS XDJG_GuildName, a.level AS XDJG_GuildLevel, \
							  (SELECT COUNT(*) FROM %s.family_member WHERE familyid = a.guid) AS XDJG_Guildmembers, \
							  FROM_UNIXTIME(a.createdate) AS XDJG_CreateTime, \
							  (SELECT name FROM %s.character WHERE guid = a.leaderguid) AS XDJG_GuildLeader \
						 FROM %s.family a, %s.family_member b \
						WHERE a.guid = b.familyid \
						  AND b.name = '%s'", 
							  Account,Character,GameDB,GameDB,GameDB,GameDB,Character);
		
		DBTFLV = GameDBQuery("XDJG",realServerIP,12, 0, 0, strSql.GetBuffer(0), GameDB, GameDB);
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "��ҹ��������Ϣ");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	return DBTFLV;
}

//��ҹ�����Ʒ��Ϣ
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_PlayerGuildItemInfo_Query(char *ServerIP, char *Account, char *Character, int iIndex, int iPageSize)
{
	CGlobalStruct::TFLV m_tflv;
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	char realServerIP[40];
	vector<CGlobalStruct::DataRecord> datarecord;
	vector<CGlobalStruct::DataRecord>::iterator pData = NULL;
	int iNum = 0;
	st_GuildItemInfo sGuildItemInfo;
	vector<st_GuildItemInfo>::iterator pGuildItemInfo = NULL;
	BOOL iFlag = FALSE;
	vector<st_ItemInfo>::iterator pItemInfo = NULL;
	char retMessage[255];
	ZeroMemory(retMessage,255);
	Vec_ItemInfo VecItemInfo;
	
	try
	{
		m_VecGuildItemInfo.clear();

		sprintf(retMessage, "��ҹ�����Ʒ��Ϣ:<����>%s, <�ʺ�>%s, <��ɫ>%s", ServerIP, Account, Character);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,3))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "��ҹ�����Ʒ��Ϣ");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}

// 		strSql.Format("SELECT items AS XDJG_Items \
// 							  FROM  %s.family_storage \
// 							  WHERE familyindex  = '536'", 
// 							  GameDB);
// 		
// 		m_SqlHelper.SqlHelperMain("XDJG", &datarecord,realServerIP,strSql.GetBuffer(0),1,0,0);
// 
// 		for(pData = datarecord.begin(); pData != datarecord.end(); pData++)
// 		{
// 			if(!strcmp(pData->recordfieldname, "XDJG_Items"))
// 			{
// 				memcpy(&m_StructInfo.m_sGuildWareHouse, pData->recorddata, sizeof(m_StructInfo.m_sGuildWareHouse));
// 			}
// 		}

		strSql.Format("SELECT '%s' AS XDJG_Account, '%s' AS XDJG_Character, \
							  a.name AS XDJG_GuildName, c.items AS XDJG_Items \
						 FROM %s.family a, %s.family_member b, %s.family_storage c \
						WHERE a.guid = b.familyid \
						  AND b.familyid = c.familyindex \
						  AND b.name = '%s'", 
							  Account,Character,GameDB,GameDB,GameDB,Character);
		
		m_SqlHelper.SqlHelperMain("XDJG", &datarecord,realServerIP,strSql.GetBuffer(0),12,0,0);
		
		if(datarecord.empty())
			return DBTFLV;
		
		//ȡ��Ʒ����
		GetItemInfo(VecItemInfo);

		for(pData = datarecord.begin(); pData != datarecord.end(); pData++)
		{
			if(!strcmp(pData->recordfieldname, "XDJG_Account"))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_Account;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(Account) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",Account);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp(pData->recordfieldname, "XDJG_Character"))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_Character;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(Character) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",Character);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp(pData->recordfieldname, "XDJG_GuildName"))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_GuildName;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(pData->recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",pData->recorddata);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp(pData->recordfieldname, "XDJG_Items"))
			{
				memcpy(&m_StructInfo.m_sGuildWareHouse, pData->recorddata, sizeof(m_StructInfo.m_sGuildWareHouse));
				for(iNum = 0; iNum < m_StructInfo.m_sGuildWareHouse.limit_guild_ware_house_size_per_level; iNum++)
				{
					iFlag = FALSE;
					if(m_StructInfo.m_sGuildWareHouse.items.res_list[iNum].itemId != 0)
					{
						if(m_VecGuildItemInfo.empty())
						{
							sGuildItemInfo.iItemId = m_StructInfo.m_sGuildWareHouse.items.res_list[iNum].itemId;
							sGuildItemInfo.iItemNum = 1;
							m_VecGuildItemInfo.push_back(sGuildItemInfo);
						}
						else
						{
							for(pGuildItemInfo = m_VecGuildItemInfo.begin(); pGuildItemInfo != m_VecGuildItemInfo.end(); pGuildItemInfo++)
							{
								if(pGuildItemInfo->iItemId == m_StructInfo.m_sGuildWareHouse.items.res_list[iNum].itemId)
								{
									iFlag = TRUE;
									pGuildItemInfo->iItemNum += 1;
								}
							}

							if(iFlag == FALSE)
							{
								sGuildItemInfo.iItemId = m_StructInfo.m_sGuildWareHouse.items.res_list[iNum].itemId;
								sGuildItemInfo.iItemNum = 1;
								m_VecGuildItemInfo.push_back(sGuildItemInfo);
							}
						}
					}
				}

				if(m_VecGuildItemInfo.size() == 0)
				{
					DBTFLV.clear();
					return DBTFLV;

// 					m_tflv.nIndex = DBTFLV.size() + 1;
// 					m_tflv.m_tagName = CEnumCore::TagName::XDJG_ItemName;
// 					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
// 					m_tflv.m_tvlength = strlen("")+1;
// 					sprintf((char *)&m_tflv.lpdata,"%s","");
// 					DBTFLV.push_back(m_tflv);
// 					
// 					m_tflv.nIndex = DBTFLV.size() + 1;
// 					m_tflv.m_tagName = CEnumCore::TagName::XDJG_ItemNum;
// 					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
// 					m_tflv.m_tvlength = strlen("0")+1;
// 					sprintf((char *)&m_tflv.lpdata,"%s","0");
// 					DBTFLV.push_back(m_tflv);
				}
				else
				{
					for(pGuildItemInfo = m_VecGuildItemInfo.begin(); pGuildItemInfo != m_VecGuildItemInfo.end(); pGuildItemInfo++)
					{
						for(pItemInfo = VecItemInfo.begin(); pItemInfo != VecItemInfo.end(); pItemInfo++)
						{
							if(pItemInfo->iItemId == pGuildItemInfo->iItemId)
							{
								m_tflv.nIndex = DBTFLV.size() + 1;
								m_tflv.m_tagName = CEnumCore::TagName::XDJG_ItemName;
								m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
								m_tflv.m_tvlength = pItemInfo->strItemName.length();
								sprintf((char *)&m_tflv.lpdata,"%s",pItemInfo->strItemName.c_str());
								DBTFLV.push_back(m_tflv);

								m_tflv.nIndex = DBTFLV.size() + 1;
								m_tflv.m_tagName = CEnumCore::TagName::XDJG_ItemNum;
								m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
								m_tflv.m_tvlength = sizeof(int);
								sprintf((char *)&m_tflv.lpdata,"%d",pGuildItemInfo->iItemNum);
								DBTFLV.push_back(m_tflv);
								break;
							}
							else
								continue;
						}
					}
				}
			}
			else if(!strcmp(pData->recordfieldname, "PageCount"))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::PageCount;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(pData->recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",pData->recorddata);
				DBTFLV.push_back(m_tflv);
			}
		}
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "��ҹ�����Ʒ��Ϣ");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	return DBTFLV;
}

//�����Ʒ��־
//װ��������־
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_EquipIncrease_Log_Query(char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	char realServerIP[40];
	vector<CGlobalStruct::DataRecord> datarecord;
	char retMessage[255];
	ZeroMemory(retMessage,255);
	
	try
	{
		sprintf(retMessage, "װ��������־:<����>%s, <�ʺ�>%s, <��ɫ>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, BeginTime, EndTime);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "װ��������־");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		strSql.Format("SELECT '%s' AS XDJG_Account, '%s' AS XDJG_Character, \
							  item_name AS XDJG_ItemName, item_id AS XDJG_ItemID, \
							  create_time AS XDJG_OperateTime \
						 from %s.STR \
						 where create_time between '%s' and '%s' \
						   and character_name = '%s'",Account,Character,LogDB,BeginTime,EndTime,Character);

		DBTFLV = GameDBQuery("XDJG",realServerIP,13, iIndex, iPageSize, strSql.GetBuffer(0), LogDB, LogDB);
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "װ��������־");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	return DBTFLV;
}

//��Ƭ�ϳ���־
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_Synthesis_Log_Query(char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	char realServerIP[40];
	vector<CGlobalStruct::DataRecord> datarecord;
	char retMessage[255];
	ZeroMemory(retMessage,255);
	
	try
	{
		sprintf(retMessage, "��Ƭ�ϳ���־:<����>%s, <�ʺ�>%s, <��ɫ>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, BeginTime, EndTime);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "��Ƭ�ϳ���־");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		strSql.Format("SELECT '%s' AS XDJG_Account, '%s' AS XDJG_Character, \
							  item_name AS XDJG_ItemName, item_id AS XDJG_ItemID, \
							  create_time AS XDJG_OperateTime, item_strengthen_level AS XDJG_Level \
							  from %s.PLUGIN \
							  where create_time between '%s' and '%s' \
						        and character_name = '%s'",Account,Character,LogDB,BeginTime,EndTime,Character);
		
		DBTFLV = GameDBQuery("XDJG",realServerIP,13, iIndex, iPageSize, strSql.GetBuffer(0), LogDB, LogDB);
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "��Ƭ�ϳ���־");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	return DBTFLV;
}

//װ��������־
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_EquipTransfer_Log_Query(char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	char realServerIP[40];
	vector<CGlobalStruct::DataRecord> datarecord;
	char retMessage[255];
	ZeroMemory(retMessage,255);
	
	try
	{
		sprintf(retMessage, "װ��������־:<����>%s, <�ʺ�>%s, <��ɫ>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, BeginTime, EndTime);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "װ��������־");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		strSql.Format("SELECT '%s' AS XDJG_Account, '%s' AS XDJG_Character, \
							  item_name_to AS XDJG_ItemName, item_id_to AS XDJG_ItemID, \
							  item_name_from AS XDJG_ConsumeItem, \
							  create_time AS XDJG_OperateTime, item_strengthen_level_to AS XSJG_Level \
						 from %s.REPLACE \
						 where create_time between '%s' and '%s' \
						   and character_name = '%s'",Account,Character,LogDB,BeginTime,EndTime,Character);
		
		DBTFLV = GameDBQuery("XDJG",realServerIP,13, iIndex, iPageSize, strSql.GetBuffer(0), LogDB, LogDB);
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "װ��������־");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	return DBTFLV;
}

//��Ʒ����
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_ItemIdentify_Log_Query(char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	vector<CGlobalStruct::TFLV>::iterator pDBTFLV = NULL;
	CString strSql = "";
	char realServerIP[40];
	vector<CGlobalStruct::DataRecord> datarecord;
	vector<st_ItemInfo>::iterator pItemInfo = NULL;
	char retMessage[255];
	ZeroMemory(retMessage,255);
	Vec_ItemInfo VecItemInfo;
	
	try
	{
		sprintf(retMessage, "��Ʒ����:<����>%s, <�ʺ�>%s, <��ɫ>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, BeginTime, EndTime);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "��Ʒ����");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		strSql.Format("SELECT '%s' AS XDJG_Account, '%s' AS XDJG_Character, \
							  item_id AS XDJG_ItemID, create_time AS XDJG_OperateTime \
						 from %s.IDENTIFY \
						where create_time between '%s' and '%s' \
						  and character_name = '%s'",Account,Character,LogDB,BeginTime,EndTime,Character);
		
		DBTFLV = GameDBQuery("XDJG",realServerIP,13, iIndex, iPageSize, strSql.GetBuffer(0), LogDB, LogDB);

		//ȡ��Ʒ����
		GetItemInfo(VecItemInfo);

		for(pDBTFLV = DBTFLV.begin();pDBTFLV != DBTFLV.end();pDBTFLV++)
		{
			if(pDBTFLV->m_tagName == CEnumCore::TagName::XDJG_ItemID)
			{
				//��Ʒ����
				for(pItemInfo = VecItemInfo.begin(); pItemInfo != VecItemInfo.end(); pItemInfo++)
				{
					if(pItemInfo->iItemId == atoi((char*)pDBTFLV->lpdata))
					{
						pDBTFLV->m_tagName = CEnumCore::TagName::XDJG_ItemName;
						pDBTFLV->m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
						pDBTFLV->m_tvlength = pItemInfo->strItemName.size();
						memcpy(&pDBTFLV->lpdata, pItemInfo->strItemName.c_str(), pItemInfo->strItemName.size());
						break;
					}
					else
						continue;
				}

				if(pDBTFLV->m_tagName != CEnumCore::TagName::XDJG_ItemName)
				{
					pDBTFLV->m_tagName = CEnumCore::TagName::XDJG_ItemName;
					pDBTFLV->m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					pDBTFLV->m_tvlength = strlen("")+1;
					memcpy(&pDBTFLV->lpdata, "%s", strlen("")+1);
				}
			}
		}
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "��Ʒ����");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	return DBTFLV;
}

//���˲ֿ�����־
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_RoleStore_Log_Query(char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	char realServerIP[40];
	vector<CGlobalStruct::DataRecord> datarecord;
	char retMessage[255];
	ZeroMemory(retMessage,255);
	
	try
	{
		sprintf(retMessage, "���˲ֿ�����־:<����>%s, <�ʺ�>%s, <��ɫ>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, BeginTime, EndTime);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "װ��������־");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		strSql.Format("SELECT '%s' AS XDJG_Account, '%s' AS XDJG_Character, \
							  item_name AS XDJG_ItemName, item_id AS XDJG_ItemID, \
							  create_time AS XDJG_OperateTime, item_strengthen_level AS XSJG_Level \
						 from %s.REPLACE \
						where create_time between '%s' and '%s' \
						  and character_name = '%s'",Account,Character,LogDB,BeginTime,EndTime,Character);
		
		DBTFLV = GameDBQuery("XDJG",realServerIP,13, iIndex, iPageSize, strSql.GetBuffer(0), LogDB, LogDB);
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "װ��������־");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	return DBTFLV;
}

//����ֿ�����־
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_GuildStore_Log_Query(char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	vector<CGlobalStruct::TFLV>::iterator pDBTFLV = NULL;
	CString strSql = "";
	char realServerIP[40];
	vector<CGlobalStruct::DataRecord> datarecord;
	vector<st_ItemInfo>::iterator pItemInfo = NULL;
	char retMessage[255];
	ZeroMemory(retMessage,255);
	Vec_ItemInfo VecItemInfo;
	
	try
	{
		sprintf(retMessage, "����ֿ�����־:<����>%s, <�ʺ�>%s, <��ɫ>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, BeginTime, EndTime);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "����ֿ�����־");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		strSql.Format("SELECT '%s' AS XDJG_Account, '%s' AS XDJG_Character, \
							  item_id AS XDJG_TempItemID, item_id AS XDJG_ItemID, \
							  item_num AS XDJG_ItemNum, '����' AS XDJG_Operatetype, \
							  create_time AS XDJG_OperateTime \
						 FROM %s.GUILD_STOREITEM \
						where create_time between '%s' and '%s' \
						  and character_name = '%s' \
						UNION \
					   SELECT '%s' AS XDJG_Account, '%s' AS XDJG_Character, \
							  item_id AS XDJG_TempItemID, item_id AS XDJG_ItemID, \
							  item_num AS XDJG_ItemNum, 'ȡ��' AS XDJG_Operatetype, \
							  create_time AS XDJG_OperateTime \
						 FROM %s.GUILD_GETITEM \
						where create_time between '%s' and '%s' \
						  and character_name = '%s'",Account,Character,LogDB,BeginTime,EndTime,Character, Account,Character,LogDB,BeginTime,EndTime,Character);
		
 		DBTFLV = GameDBQuery("XDJG",realServerIP,13, 0, 0, strSql.GetBuffer(0), LogDB, LogDB);

		//ȡ��Ʒ����
		GetItemInfo(VecItemInfo);

		for(pDBTFLV = DBTFLV.begin();pDBTFLV != DBTFLV.end();pDBTFLV++)
		{
			if(pDBTFLV->m_tagName == CEnumCore::TagName::XDJG_TempItemID)
			{
				//��Ʒ����
				for(pItemInfo = VecItemInfo.begin(); pItemInfo != VecItemInfo.end(); pItemInfo++)
				{
					if(pItemInfo->iItemId == atoi((char*)pDBTFLV->lpdata))
					{
						pDBTFLV->m_tagName = CEnumCore::TagName::XDJG_ItemName;
						pDBTFLV->m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
						pDBTFLV->m_tvlength = pItemInfo->strItemName.size();
						memcpy(&pDBTFLV->lpdata, pItemInfo->strItemName.c_str(), pItemInfo->strItemName.size());
						break;
					}
					else
						continue;
				}
				
				if(pDBTFLV->m_tagName != CEnumCore::TagName::XDJG_ItemName)
				{
					pDBTFLV->m_tagName = CEnumCore::TagName::XDJG_ItemName;
					pDBTFLV->m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					pDBTFLV->m_tvlength = strlen("")+1;
					memcpy(&pDBTFLV->lpdata, "%s", strlen("")+1);
				}
			}
		}
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "����ֿ�����־");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	return DBTFLV;
}

//������־
//�������
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_GuildCreate_Log_Query(char *ServerIP, char *Guildname, char *BeginTime, char *EndTime, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> NEW_DBTFLV;
	vector<CGlobalStruct::TFLV> DBTFLV;
	vector<CGlobalStruct::TFLV>::iterator pDBTFLV = NULL;
	CString strSql = "";
	string strAccount = "";
	char GameDBServerIP[40];
	char realServerIP[40];
	vector<CGlobalStruct::DataRecord> datarecord;
	vector<CGlobalStruct::DataRecord>::iterator pData = NULL;
	char retMessage[255];
	ZeroMemory(retMessage,255);
	
	try
	{
		sprintf(retMessage, "���������־:<����>%s, <��������>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Guildname, BeginTime, EndTime);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG

		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "���������־");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		strSql.Format("SELECT create_time AS XDJG_CreateTime, character_name AS XDJG_Character, \
							  character_name AS XDJG_TempCharacter, \
							  guild_name AS XDJG_GuildName, money_lose AS XDJG_MoneyLose \
							  FROM %s.CREATE_GUILD \
							  where create_time between '%s' and '%s' \
							    and guild_name = '%s'",LogDB,BeginTime,EndTime,Guildname);
		
		DBTFLV = GameDBQuery("XDJG",realServerIP,13, 0, 0, strSql.GetBuffer(0), LogDB, LogDB);

		//��GameDBȡ����ʺ�
		for(pDBTFLV = DBTFLV.begin(); pDBTFLV != DBTFLV.end(); pDBTFLV++)
		{
			if(pDBTFLV->m_tagName == CEnumCore::TagName::XDJG_Character)
			{
				if(!m_operXDJG.GetServerIP(GameDBServerIP,ServerIP,3))
				{
					NEW_DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡGameDB��IP����", "���������־");
					return NEW_DBTFLV;//�޷���ȡ�����ķ�����IP
				}
				
				strSql.Format("Select account_name AS XDJG_Account \
								 from %s.character \
								where name = '%s'",
									  GameDB, pDBTFLV->lpdata);
				
				m_SqlHelper.SqlHelperMain("XDJG", &datarecord,GameDBServerIP,strSql.GetBuffer(0),12,0,0);
				
				if(datarecord.empty())
					return NEW_DBTFLV;
				
				for(pData = datarecord.begin(); pData != datarecord.end(); pData++)
				{
					if(!strcmp(pData->recordfieldname, "XDJG_Account"))
					{
						strAccount = pData->recorddata;
						break;
					}
				}
				
				break;
			}
		}

		//����ʱ��ɫת��������ʺ�
		for(pDBTFLV = DBTFLV.begin(); pDBTFLV != DBTFLV.end(); pDBTFLV++)
		{
			if(pDBTFLV->m_tagName == CEnumCore::TagName::XDJG_TempCharacter)
			{
				pDBTFLV->m_tagName = CEnumCore::TagName::XDJG_Account;
				pDBTFLV->m_tvlength = strAccount.length();
				*pDBTFLV->lpdata = NULL;
				sprintf((char*)&pDBTFLV->lpdata, "%s", strAccount.c_str());
			}
			else
				continue;
		}
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "���������־");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	return DBTFLV;
}

//�����ɢ
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_GuildDismiss_Log_Query(char *ServerIP, char *Guildname, char *BeginTime, char *EndTime, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	vector<CGlobalStruct::TFLV>::iterator pDBTFLV = NULL;
	CString strSql = "";
	string strGuid = "";
	string strAccount = "";
	char GameDBServerIP[40];
	char realServerIP[40];
	vector<CGlobalStruct::DataRecord> datarecord;
	vector<CGlobalStruct::DataRecord>::iterator pData = NULL;
	char retMessage[255];
	ZeroMemory(retMessage,255);
	
	try
	{
		sprintf(retMessage, "�����ɢ��־:<����>%s, <��������>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Guildname, BeginTime, EndTime);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG

		//��GameDbȡ����id
		if(!m_operXDJG.GetServerIP(GameDBServerIP,ServerIP,3))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡGameDB��IP����", "�����ɢ��־");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		strSql.Format("Select guid AS XDJG_GuildID \
						 from %s.family \
						where name = '%s'",
							  GameDB, Guildname);
		
		m_SqlHelper.SqlHelperMain("XDJG", &datarecord,GameDBServerIP,strSql.GetBuffer(0),12,0,0);
		
		if(datarecord.empty())
			return DBTFLV;
		
		for(pData = datarecord.begin(); pData != datarecord.end(); pData++)
		{
			if(!strcmp(pData->recordfieldname, "XDJG_GuildID"))
			{
				strGuid = pData->recorddata;
				break;
			}
		}
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "�����ɢ��־");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		strSql.Format("SELECT create_time AS XDJG_CreateTime, character_name AS XDJG_Character, \
							  character_name AS XDJG_TempCharacter, '%s' AS XDJG_GuildName \
						 FROM %s.DISMISS_GUILD \
						where create_time between '%s' and '%s' \
						  and guild_id = '%s'",Guildname,LogDB,BeginTime,EndTime,strGuid.c_str());
		
		DBTFLV = GameDBQuery("XDJG",realServerIP,13, 0, 0, strSql.GetBuffer(0), LogDB, LogDB);
		
		//��GameDBȡ����ʺ�
		for(pDBTFLV = DBTFLV.begin(); pDBTFLV != DBTFLV.end(); pDBTFLV++)
		{
			if(pDBTFLV->m_tagName == CEnumCore::TagName::XDJG_Character)
			{
				strSql.Format("Select account_name AS XDJG_Account \
								 from %s.character \
								 where name = '%s'",
									   GameDB, pDBTFLV->lpdata);
				
				m_SqlHelper.SqlHelperMain("XDJG", &datarecord,GameDBServerIP,strSql.GetBuffer(0),12,0,0);
				
				if(datarecord.empty())
					return DBTFLV;
				
				for(pData = datarecord.begin(); pData != datarecord.end(); pData++)
				{
					if(!strcmp(pData->recordfieldname, "XDJG_Account"))
					{
						strAccount = pData->recorddata;
						break;
					}
				}
				
				break;
			}
		}
		
		//����ʱ��ɫת��������ʺ�
		for(pDBTFLV = DBTFLV.begin(); pDBTFLV != DBTFLV.end(); pDBTFLV++)
		{
			if(pDBTFLV->m_tagName == CEnumCore::TagName::XDJG_TempCharacter)
			{
				pDBTFLV->m_tagName = CEnumCore::TagName::XDJG_Account;
				pDBTFLV->m_tvlength = strAccount.length();
				*pDBTFLV->lpdata = NULL;
				sprintf((char*)&pDBTFLV->lpdata, "%s", strAccount.c_str());
			}
			else
				continue;
		}
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "�����ɢ��־");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	return DBTFLV;
}

//���ݽ�ɫȡ�ʺ�
CGlobalStruct::TFLV CXDJGInfo::GetAccount(char *ServerIP, char *RoleName)
{
	CGlobalStruct::TFLV m_tflv;
	vector<CGlobalStruct::TFLV> DBTFLV;
	vector<CGlobalStruct::TFLV>::iterator pDBTFLV = NULL;
	CString strSql = "";
	char realServerIP[40];
	char retMessage[255];
	ZeroMemory(retMessage,255);
	
	try
	{
		sprintf(retMessage, "���ݽ�ɫȡ�ʺ�:<����>%s, <��ɫ>%s", ServerIP, RoleName);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		{
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Account;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen("") + 1;
			sprintf((char *)&m_tflv.lpdata,"%s","");
		}

		strSql.Format("SELECT account_name AS XDJG_Account from %s.character \
							  where name = '%s'",GameDB,RoleName);
		
		DBTFLV = GameDBQuery("XDJG",ServerIP,12, 0, 0, strSql.GetBuffer(0), GameDB, GameDB);

		if(DBTFLV.empty())
			return m_tflv;

		for(pDBTFLV = DBTFLV.begin(); pDBTFLV != DBTFLV.end(); pDBTFLV++)
		{
			if(pDBTFLV->m_tagName == CEnumCore::TagName::XDJG_Account)
				return *pDBTFLV;
		}
	}
	catch(...)
	{
		return m_tflv;
	}
	
	return m_tflv;
}

//������ӳ�Ա		
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_GuildAddMember_Log_Query(char *ServerIP, char *Guildname, char *BeginTime, char *EndTime, int iIndex, int iPageSize)
{
	CGlobalStruct::TFLV m_tflv;
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	CString strData = "";
	char realServerIP[40];
	vector<CGlobalStruct::DataRecord> datarecord;
	vector<CGlobalStruct::DataRecord>::iterator pData = NULL;
	vector<_Car> Vec_Car;
	int iCount = 0;
	int iNum = 0;
	char retMessage[255];
	ZeroMemory(retMessage,255);
	
	try
	{
		sprintf(retMessage, "������ӳ�Ա:<����>%s, <��������>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Guildname, BeginTime, EndTime);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,3))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "������ӳ�Ա");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		strSql.Format("SELECT FROM_UNIXTIME(TIMESTAMP) AS XDJG_JoinTime, \
							  '%s' AS XDJG_GuildName, misc AS XDJG_GuildLog \
						 FROM %s.Guild_Logs \
						WHERE guildid = (SELECT guid FROM %s.family WHERE NAME = '%s') \
						  AND event_type = 3",Guildname, GameDB, GameDB, Guildname);
		
		m_SqlHelper.SqlHelperMain("XDJG", &datarecord,realServerIP,strSql.GetBuffer(0),12,0,0);
		
		if(datarecord.empty())
			return DBTFLV;
		
		for(pData = datarecord.begin(); pData != datarecord.end(); pData++)
		{
			if(!strcmp(pData->recordfieldname, "XDJG_JoinTime"))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_JoinTime;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(pData->recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",pData->recorddata);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp(pData->recordfieldname, "XDJG_GuildName"))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_GuildName;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(pData->recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",pData->recorddata);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp(pData->recordfieldname, "XDJG_GuildLog"))
			{
				memcpy(&m_StructInfo.m_misc, pData->recorddata, sizeof(m_StructInfo.m_misc));

				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_Character;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(m_StructInfo.m_misc.name1) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",m_StructInfo.m_misc.name1);
				DBTFLV.push_back(m_tflv);

				//���ݽ�ɫȡ�ʺ�
				m_tflv = GetAccount(realServerIP, m_StructInfo.m_misc.name1);
				m_tflv.nIndex = DBTFLV.size()+1;
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp(pData->recordfieldname, "PageCount"))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::PageCount;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(pData->recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",pData->recorddata);
				DBTFLV.push_back(m_tflv);
			}
		}

	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "������ӳ�Ա");//��������Ϣд����־�в����ش�����Ϣ
	}

	return DBTFLV;
}

//�����Ա�뿪
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_GuildMemberLeaved_Log_Query(char *ServerIP, char *Guildname, char *BeginTime, char *EndTime, int iIndex, int iPageSize)
{
	CGlobalStruct::TFLV m_tflv;
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	CString strData = "";
	char realServerIP[40];
	vector<CGlobalStruct::DataRecord> datarecord;
	vector<CGlobalStruct::DataRecord>::iterator pData = NULL;
	vector<_Car> Vec_Car;
	int iCount = 0;
	int iNum = 0;
	char retMessage[255];
	ZeroMemory(retMessage,255);
	
	try
	{
		sprintf(retMessage, "�����Ա�뿪:<����>%s, <��������>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Guildname, BeginTime, EndTime);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,3))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "�����Ա�뿪");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		strSql.Format("SELECT FROM_UNIXTIME(TIMESTAMP) AS XDJG_LeaveTime, \
							  '%s' AS XDJG_GuildName, \
							  CASE event_type WHEN 4 THEN 'ǿ���뿪' \
 													 ELSE '�����뿪' END AS XDJG_ExitGuildType, \
							  misc AS XDJG_GuildLog \
						 FROM %s.Guild_Logs \
						WHERE guildid = (SELECT guid FROM %s.family WHERE NAME = '%s') \
						  AND event_type in (4,5)",Guildname, GameDB, GameDB, Guildname);
		
		m_SqlHelper.SqlHelperMain("XDJG", &datarecord,realServerIP,strSql.GetBuffer(0),12,0,0);
		
		if(datarecord.empty())
			return DBTFLV;
		
		for(pData = datarecord.begin(); pData != datarecord.end(); pData++)
		{
			if(!strcmp(pData->recordfieldname, "XDJG_LeaveTime"))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_LeaveTime;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(pData->recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",pData->recorddata);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp(pData->recordfieldname, "XDJG_GuildName"))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_GuildName;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(pData->recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",pData->recorddata);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp(pData->recordfieldname, "XDJG_ExitGuildType"))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_ExitGuildType;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(pData->recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",pData->recorddata);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp(pData->recordfieldname, "XDJG_GuildLog"))
			{
				memcpy(&m_StructInfo.m_misc, pData->recorddata, sizeof(m_StructInfo.m_misc));
				
				if(!strcmp((char*)m_tflv.lpdata, "ǿ���뿪"))
				{
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_Character;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=strlen(m_StructInfo.m_misc.name2) + 1;
					sprintf((char *)&m_tflv.lpdata,"%s",m_StructInfo.m_misc.name2);
					DBTFLV.push_back(m_tflv);
					
					//���ݽ�ɫȡ�ʺ�
					m_tflv = GetAccount(realServerIP, m_StructInfo.m_misc.name2);
					m_tflv.nIndex = DBTFLV.size()+1;
					DBTFLV.push_back(m_tflv);
				}
				else if(!strcmp((char*)m_tflv.lpdata, "�����뿪"))
				{
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_Character;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=strlen(m_StructInfo.m_misc.name1) + 1;
					sprintf((char *)&m_tflv.lpdata,"%s",m_StructInfo.m_misc.name1);
					DBTFLV.push_back(m_tflv);
					
					//���ݽ�ɫȡ�ʺ�
					m_tflv = GetAccount(realServerIP, m_StructInfo.m_misc.name1);
					m_tflv.nIndex = DBTFLV.size()+1;
					DBTFLV.push_back(m_tflv);
				}
				else
				{
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_Character;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=strlen("") + 1;
					sprintf((char *)&m_tflv.lpdata,"%s","");
					DBTFLV.push_back(m_tflv);

					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_Account;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=strlen("") + 1;
					sprintf((char *)&m_tflv.lpdata,"%s","");
					DBTFLV.push_back(m_tflv);
				}
			}
			else if(!strcmp(pData->recordfieldname, "PageCount"))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::PageCount;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(pData->recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",pData->recorddata);
				DBTFLV.push_back(m_tflv);
			}
		}
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "�����Ա�뿪");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	return DBTFLV;
}

//����ְ����
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_GuildPostChanged_Log_Query(char *ServerIP, char *Guildname, char *BeginTime, char *EndTime, int iIndex, int iPageSize)
{
	CGlobalStruct::TFLV m_tflv;
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	CString strData = "";
	char realServerIP[40];
	vector<CGlobalStruct::DataRecord> datarecord;
	vector<CGlobalStruct::DataRecord>::iterator pData = NULL;
	vector<_Car> Vec_Car;
	int iCount = 0;
	int iNum = 0;
	char retMessage[255];
	ZeroMemory(retMessage,255);
	
	try
	{
		sprintf(retMessage, "����ְ����:<����>%s, <��������>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Guildname, BeginTime, EndTime);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,3))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "����ְ����");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		strSql.Format("SELECT FROM_UNIXTIME(TIMESTAMP) AS XDJG_LeaderChangedTime, \
						      '%s' AS XDJG_GuildName, misc AS XDJG_GuildLog, \
							  '�᳤' AS XDJG_GuildTransferedPosition \
						 FROM %s.Guild_Logs \
						WHERE guildid = (SELECT guid FROM %s.family WHERE NAME = '%s') \
						  AND event_type = 6",Guildname, GameDB, GameDB, Guildname);
		
		m_SqlHelper.SqlHelperMain("XDJG", &datarecord,realServerIP,strSql.GetBuffer(0),12,0,0);
		
		if(datarecord.empty())
			return DBTFLV;
		
		for(pData = datarecord.begin(); pData != datarecord.end(); pData++)
		{
			if(!strcmp(pData->recordfieldname, "XDJG_LeaderChangedTime"))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_LeaderChangedTime;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(pData->recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",pData->recorddata);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp(pData->recordfieldname, "XDJG_GuildName"))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_GuildName;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(pData->recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",pData->recorddata);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp(pData->recordfieldname, "XDJG_GuildLog"))
			{
				memcpy(&m_StructInfo.m_misc, pData->recorddata, sizeof(m_StructInfo.m_misc));
				
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_Character;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(m_StructInfo.m_misc.name2) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",m_StructInfo.m_misc.name2);
				DBTFLV.push_back(m_tflv);
				
				//���ݽ�ɫȡ�ʺ�
				m_tflv = GetAccount(realServerIP, m_StructInfo.m_misc.name2);
				m_tflv.nIndex = DBTFLV.size()+1;
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp(pData->recordfieldname, "XDJG_GuildTransferedPosition"))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_GuildTransferedPosition;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(pData->recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",pData->recorddata);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp(pData->recordfieldname, "PageCount"))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::PageCount;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(pData->recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",pData->recorddata);
				DBTFLV.push_back(m_tflv);
			}
		}
		
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "����ְ����");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	return DBTFLV;
}

//�ʺ��Ƿ��ͣ
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_AccountIsClose_Query(char *ServerIP, char *Account)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	char realServerIP[40];
	CString strSql = "";
	char retMessage[255];
	ZeroMemory(retMessage,255);
	
	try
	{
		sprintf(retMessage, "�ʺ��Ƿ��ͣ:<����>%s, <�ʺ�>%s", ServerIP, Account);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,4))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "�ʺ��Ƿ��ͣ");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		strSql.Format("select b.id as XDJG_AccountId, b.name as XDJG_Account, \
							  a.start_time as XDJG_Opentime, a.enable_time as XDJG_Closetime, \
							  a.desc as XDJG_Reason \
						 from %s.login_disabled a, %s.account b \
						where a.account_id = b.id and b.name = '%s'", 
							  AccountDB,AccountDB,Account);
		
#ifdef _DEBUG
		printf(strSql, AccountDB, AccountDB);
#endif
		
		DBTFLV = GameDBQuery("XDJG",realServerIP,12, 0, 0, strSql.GetBuffer(0), AccountDB, AccountDB);
	
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "�ʺ��Ƿ��ͣ");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	if(DBTFLV.empty())
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ��ͣ", "�ʺ��Ƿ��ͣ");

	return DBTFLV;
}

//NPC������ȡ��ѯ
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_NPCGetItem_Query(char *ServerIP, char *Account, int AccountID, char *Character,int iIndex, int iPageSize)
{
	CGlobalStruct::TFLV m_tflv;
	vector<CGlobalStruct::TFLV> DBTFLV;
	vector<CGlobalStruct::TFLV> New_DBTFLV;
	vector<CGlobalStruct::TFLV>::iterator pNewDBTFLV = NULL;
	char realServerIP[40];
	CString strSql = "";
	vector<CGlobalStruct::DataRecord> datarecord;
	vector<CGlobalStruct::DataRecord>::iterator pData = NULL;
	char retMessage[255];
	ZeroMemory(retMessage,255);
	
	try
	{
		sprintf(retMessage, "NPC������ȡ��ѯ:<����>%s, <�ʺ�>%s, <�ʺ�ID>%d, <��ɫ>%s", ServerIP, Account,AccountID, Character);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ȡIP��ַ����", "�ʺ��Ƿ��ͣ");
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}

		strSql.Format("SELECT '%s' AS XDJG_Account, '%s' AS XDJG_Character, \
							  encouragement_ID AS XDJG_packageId, create_time AS XDJG_GetTime \
						 FROM %s.REQ_GAIN_ENCOURAGEMENT \
						WHERE account_id = %d",Account, Character, LogDB, AccountID);
		
		m_SqlHelper.SqlHelperMain("XDJG", &datarecord,realServerIP,strSql.GetBuffer(0),13,0,0);
		
		if(datarecord.empty())
			return DBTFLV;
		
		for(pData = datarecord.begin(); pData != datarecord.end(); pData++)
		{
			if(!strcmp(pData->recordfieldname, "XDJG_Account"))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_Account;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(pData->recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",pData->recorddata);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp(pData->recordfieldname, "XDJG_Character"))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_Character;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(pData->recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",pData->recorddata);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp(pData->recordfieldname, "XDJG_packageId"))
			{
				New_DBTFLV = GetAwardPackageInfo(atoi((char*)pData->recorddata));

				for(pNewDBTFLV = New_DBTFLV.begin(); pNewDBTFLV != New_DBTFLV.end(); pNewDBTFLV++)
				{
					pNewDBTFLV->nIndex = DBTFLV.size()+1;
					DBTFLV.push_back(*pNewDBTFLV);
				}
			}
			else if(!strcmp(pData->recordfieldname, "XDJG_GetTime"))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_GetTime;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(pData->recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",pData->recorddata);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp(pData->recordfieldname, "PageCount"))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::PageCount;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(pData->recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",pData->recorddata);
				DBTFLV.push_back(m_tflv);
			}
		}
		
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "NPC������ȡ��ѯ");//��������Ϣд����־�в����ش�����Ϣ
	}
	
	return DBTFLV;
}

//��ȡ�콱����Ϣ
vector <CGlobalStruct::TFLV> CXDJGInfo::GetAwardPackageInfo(int iPackageId)
{
	CGlobalStruct::TFLV m_tflv;
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	char strConn[512];
	vector<CGlobalStruct::DataRecord> datarecord;
	vector<st_ItemInfo>::iterator pItemInfo = NULL;
	Vec_ItemInfo VecItemInfo;
	
	try
	{
		logFile.WriteLog("<�������>",operName,"��ȡ�콱����Ϣ");//��LOG
		
		//ȡ��������
		strSql.Format("select PackageName AS XDJG_ActivityName,Item_1_id AS XDJG_ItemID1, \
							  Item_1_Num AS XDJG_ItemNum1,Item_2_id AS XDJG_ItemID2, \
							  Item_2_Num AS XDJG_ItemNum2,Item_3_id AS XDJG_ItemID3, \
							  Item_3_Num AS XDJG_ItemNum3,Item_4_id AS XDJG_ItemID4, \
							  Item_4_Num AS XDJG_ItemNum4,Item_5_id AS XDJG_ItemID5, \
							  Item_5_Num AS XDJG_ItemNum5,Item_6_id AS XDJG_ItemID6, \
							  Item_6_Num AS XDJG_ItemNum6, \
							  CASE gold_type WHEN 8 THEN '��ջ���' \
											 ELSE 'δ֪' END AS XDJG_MoneyType,gold AS XDJG_Money, \
							  chioceAwardNum AS XDJG_ChoiceNum,choiceItem_1_Id AS XDJG_ChoiceItemID1, \
							  choiceItem_1_Num AS XDJG_ChoiceItemNum1,choiceItem_2_Id AS XDJG_ChoiceItemID2, \
							  choiceItem_2_Num AS XDJG_ChoiceItemNum2,choiceItem_3_Id AS XDJG_ChoiceItemID3, \
							  choiceItem_3_Num AS XDJG_ChoiceItemNum3,choiceItem_4_Id AS XDJG_ChoiceItemID4, \
							  choiceItem_4_Num AS XDJG_ChoiceItemNum4,choiceItem_5_Id AS XDJG_ChoiceItemID5, \
							  choiceItem_5_Num AS XDJG_ChoiceItemNum5,choiceItem_6_Id AS XDJG_ChoiceItemID6, \
							  choiceItem_6_Num AS XDJG_ChoiceItemNum6 \
						 from XDJG_NPCAwardPackage \
						where PackageId = %d",iPackageId);
		
		m_SqlHelper.SqlHelperMain("XDJG", &datarecord,"127.0.0.1",strSql.GetBuffer(0),4,0,0);
		
		if(datarecord.empty())
			return DBTFLV;
		
		//ȡ��Ʒ����
		GetItemInfo(VecItemInfo);

		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("XDJG_ActivityName", m_DataRecord.recordfieldname))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_ActivityName;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(m_DataRecord.recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",m_DataRecord.recorddata);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp("XDJG_ItemID1", m_DataRecord.recordfieldname))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_ItemID1;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(m_DataRecord.recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",m_DataRecord.recorddata);
				DBTFLV.push_back(m_tflv);

				if(atoi((char*)m_DataRecord.recorddata) == 0)
				{
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_ItemName;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=strlen("") + 1;
					sprintf((char *)&m_tflv.lpdata,"%s","");
					DBTFLV.push_back(m_tflv);
				}
				else
				{
					//��Ʒ����
					for(pItemInfo = VecItemInfo.begin(); pItemInfo != VecItemInfo.end(); pItemInfo++)
					{
						if(pItemInfo->iItemId == atoi((char*)m_DataRecord.recorddata))
						{
							m_tflv.nIndex = DBTFLV.size() + 1;
							m_tflv.m_tagName = CEnumCore::TagName::XDJG_ItemName;
							m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
							m_tflv.m_tvlength = pItemInfo->strItemName.size();
							memcpy(&m_tflv.lpdata, pItemInfo->strItemName.c_str(), pItemInfo->strItemName.size());
							DBTFLV.push_back(m_tflv);
							break;
						}
						else
							continue;
					}

					if(m_tflv.m_tagName != CEnumCore::TagName::XDJG_ItemName)
					{
						m_tflv.nIndex=DBTFLV.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::XDJG_ItemName;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen("") + 1;
						sprintf((char *)&m_tflv.lpdata,"%s","");
						DBTFLV.push_back(m_tflv);
					}
				}
			}
			else if(!strcmp("XDJG_ItemNum1", m_DataRecord.recordfieldname))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_ItemNum1;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(m_DataRecord.recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",m_DataRecord.recorddata);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp("XDJG_ItemID2", m_DataRecord.recordfieldname))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_ItemID2;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(m_DataRecord.recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",m_DataRecord.recorddata);
				DBTFLV.push_back(m_tflv);

				if(atoi((char*)m_DataRecord.recorddata) == 0)
				{
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_ItemName;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=strlen("") + 1;
					sprintf((char *)&m_tflv.lpdata,"%s","");
					DBTFLV.push_back(m_tflv);
				}
				else
				{
					//��Ʒ����
					for(pItemInfo = VecItemInfo.begin(); pItemInfo != VecItemInfo.end(); pItemInfo++)
					{
						if(pItemInfo->iItemId == atoi((char*)m_DataRecord.recorddata))
						{
							m_tflv.nIndex = DBTFLV.size() + 1;
							m_tflv.m_tagName = CEnumCore::TagName::XDJG_ItemName;
							m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
							m_tflv.m_tvlength = pItemInfo->strItemName.size();
							memcpy(&m_tflv.lpdata, pItemInfo->strItemName.c_str(), pItemInfo->strItemName.size());
							DBTFLV.push_back(m_tflv);
							break;
						}
						else
							continue;
					}

					if(m_tflv.m_tagName != CEnumCore::TagName::XDJG_ItemName)
					{
						m_tflv.nIndex=DBTFLV.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::XDJG_ItemName;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen("") + 1;
						sprintf((char *)&m_tflv.lpdata,"%s","");
						DBTFLV.push_back(m_tflv);
					}
				}
			}
			else if(!strcmp("XDJG_ItemNum2", m_DataRecord.recordfieldname))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_ItemNum2;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(m_DataRecord.recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",m_DataRecord.recorddata);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp("XDJG_ItemID3", m_DataRecord.recordfieldname))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_ItemID3;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(m_DataRecord.recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",m_DataRecord.recorddata);
				DBTFLV.push_back(m_tflv);
				
				if(atoi((char*)m_DataRecord.recorddata) == 0)
				{
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_ItemName;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=strlen("") + 1;
					sprintf((char *)&m_tflv.lpdata,"%s","");
					DBTFLV.push_back(m_tflv);
				}
				else
				{
					//��Ʒ����
					for(pItemInfo = VecItemInfo.begin(); pItemInfo != VecItemInfo.end(); pItemInfo++)
					{
						if(pItemInfo->iItemId == atoi((char*)m_DataRecord.recorddata))
						{
							m_tflv.nIndex = DBTFLV.size() + 1;
							m_tflv.m_tagName = CEnumCore::TagName::XDJG_ItemName;
							m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
							m_tflv.m_tvlength = pItemInfo->strItemName.size();
							memcpy(&m_tflv.lpdata, pItemInfo->strItemName.c_str(), pItemInfo->strItemName.size());
							DBTFLV.push_back(m_tflv);
							break;
						}
						else
							continue;
					}

					if(m_tflv.m_tagName != CEnumCore::TagName::XDJG_ItemName)
					{
						m_tflv.nIndex=DBTFLV.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::XDJG_ItemName;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen("") + 1;
						sprintf((char *)&m_tflv.lpdata,"%s","");
						DBTFLV.push_back(m_tflv);
					}
				}
			}
			else if(!strcmp("XDJG_ItemNum3", m_DataRecord.recordfieldname))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_ItemNum3;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(m_DataRecord.recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",m_DataRecord.recorddata);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp("XDJG_ItemID4", m_DataRecord.recordfieldname))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_ItemID4;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(m_DataRecord.recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",m_DataRecord.recorddata);
				DBTFLV.push_back(m_tflv);
				
				if(atoi((char*)m_DataRecord.recorddata) == 0)
				{
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_ItemName;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=strlen("") + 1;
					sprintf((char *)&m_tflv.lpdata,"%s","");
					DBTFLV.push_back(m_tflv);
				}
				else
				{
					//��Ʒ����
					for(pItemInfo = VecItemInfo.begin(); pItemInfo != VecItemInfo.end(); pItemInfo++)
					{
						if(pItemInfo->iItemId == atoi((char*)m_DataRecord.recorddata))
						{
							m_tflv.nIndex = DBTFLV.size() + 1;
							m_tflv.m_tagName = CEnumCore::TagName::XDJG_ItemName;
							m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
							m_tflv.m_tvlength = pItemInfo->strItemName.size();
							memcpy(&m_tflv.lpdata, pItemInfo->strItemName.c_str(), pItemInfo->strItemName.size());
							DBTFLV.push_back(m_tflv);
							break;
						}
						else
							continue;
					}

					if(m_tflv.m_tagName != CEnumCore::TagName::XDJG_ItemName)
					{
						m_tflv.nIndex=DBTFLV.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::XDJG_ItemName;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen("") + 1;
						sprintf((char *)&m_tflv.lpdata,"%s","");
						DBTFLV.push_back(m_tflv);
					}
				}
			}
			else if(!strcmp("XDJG_ItemNum4", m_DataRecord.recordfieldname))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_ItemNum4;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(m_DataRecord.recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",m_DataRecord.recorddata);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp("XDJG_ItemID5", m_DataRecord.recordfieldname))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_ItemID5;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(m_DataRecord.recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",m_DataRecord.recorddata);
				DBTFLV.push_back(m_tflv);
				
				if(atoi((char*)m_DataRecord.recorddata) == 0)
				{
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_ItemName;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=strlen("") + 1;
					sprintf((char *)&m_tflv.lpdata,"%s","");
					DBTFLV.push_back(m_tflv);
				}
				else
				{
					//��Ʒ����
					for(pItemInfo = VecItemInfo.begin(); pItemInfo != VecItemInfo.end(); pItemInfo++)
					{
						if(pItemInfo->iItemId == atoi((char*)m_DataRecord.recorddata))
						{
							m_tflv.nIndex = DBTFLV.size() + 1;
							m_tflv.m_tagName = CEnumCore::TagName::XDJG_ItemName;
							m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
							m_tflv.m_tvlength = pItemInfo->strItemName.size();
							memcpy(&m_tflv.lpdata, pItemInfo->strItemName.c_str(), pItemInfo->strItemName.size());
							DBTFLV.push_back(m_tflv);
							break;
						}
						else
							continue;
					}

					if(m_tflv.m_tagName != CEnumCore::TagName::XDJG_ItemName)
					{
						m_tflv.nIndex=DBTFLV.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::XDJG_ItemName;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen("") + 1;
						sprintf((char *)&m_tflv.lpdata,"%s","");
						DBTFLV.push_back(m_tflv);
					}
				}
			}
			else if(!strcmp("XDJG_ItemNum5", m_DataRecord.recordfieldname))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_ItemNum5;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(m_DataRecord.recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",m_DataRecord.recorddata);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp("XDJG_ItemID6", m_DataRecord.recordfieldname))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_ItemID6;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(m_DataRecord.recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",m_DataRecord.recorddata);
				DBTFLV.push_back(m_tflv);
				
				if(atoi((char*)m_DataRecord.recorddata) == 0)
				{
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_ItemName;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=strlen("") + 1;
					sprintf((char *)&m_tflv.lpdata,"%s","");
					DBTFLV.push_back(m_tflv);
				}
				else
				{
					//��Ʒ����
					for(pItemInfo = VecItemInfo.begin(); pItemInfo != VecItemInfo.end(); pItemInfo++)
					{
						if(pItemInfo->iItemId == atoi((char*)m_DataRecord.recorddata))
						{
							m_tflv.nIndex = DBTFLV.size() + 1;
							m_tflv.m_tagName = CEnumCore::TagName::XDJG_ItemName;
							m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
							m_tflv.m_tvlength = pItemInfo->strItemName.size();
							memcpy(&m_tflv.lpdata, pItemInfo->strItemName.c_str(), pItemInfo->strItemName.size());
							DBTFLV.push_back(m_tflv);
							break;
						}
						else
							continue;
					}

					if(m_tflv.m_tagName != CEnumCore::TagName::XDJG_ItemName)
					{
						m_tflv.nIndex=DBTFLV.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::XDJG_ItemName;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen("") + 1;
						sprintf((char *)&m_tflv.lpdata,"%s","");
						DBTFLV.push_back(m_tflv);
					}
				}
			}
			else if(!strcmp("XDJG_ItemNum6", m_DataRecord.recordfieldname))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_ItemNum6;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(m_DataRecord.recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",m_DataRecord.recorddata);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp("XDJG_MoneyType", m_DataRecord.recordfieldname))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_MoneyType;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(m_DataRecord.recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",m_DataRecord.recorddata);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp("XDJG_Money", m_DataRecord.recordfieldname))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_Money;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(m_DataRecord.recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",m_DataRecord.recorddata);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp("XDJG_ChoiceItemID1", m_DataRecord.recordfieldname))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_ChoiceItemID1;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(m_DataRecord.recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",m_DataRecord.recorddata);
				DBTFLV.push_back(m_tflv);

				if(atoi((char*)m_DataRecord.recorddata) == 0)
				{
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_ItemName;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=strlen("") + 1;
					sprintf((char *)&m_tflv.lpdata,"%s","");
					DBTFLV.push_back(m_tflv);
				}
				else
				{
					//��Ʒ����
					for(pItemInfo = VecItemInfo.begin(); pItemInfo != VecItemInfo.end(); pItemInfo++)
					{
						if(pItemInfo->iItemId == atoi((char*)m_DataRecord.recorddata))
						{
							m_tflv.nIndex = DBTFLV.size() + 1;
							m_tflv.m_tagName = CEnumCore::TagName::XDJG_ItemName;
							m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
							m_tflv.m_tvlength = pItemInfo->strItemName.size();
							memcpy(&m_tflv.lpdata, pItemInfo->strItemName.c_str(), pItemInfo->strItemName.size());
							DBTFLV.push_back(m_tflv);
							break;
						}
						else
							continue;
					}

					if(m_tflv.m_tagName != CEnumCore::TagName::XDJG_ItemName)
					{
						m_tflv.nIndex=DBTFLV.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::XDJG_ItemName;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen("") + 1;
						sprintf((char *)&m_tflv.lpdata,"%s","");
						DBTFLV.push_back(m_tflv);
					}
				}
			}
			else if(!strcmp("XDJG_ChoiceItemNum1", m_DataRecord.recordfieldname))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_ChoiceItemNum1;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(m_DataRecord.recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",m_DataRecord.recorddata);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp("XDJG_ChoiceItemID2", m_DataRecord.recordfieldname))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_ChoiceItemID2;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(m_DataRecord.recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",m_DataRecord.recorddata);
				DBTFLV.push_back(m_tflv);

				if(atoi((char*)m_DataRecord.recorddata) == 0)
				{
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_ItemName;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=strlen("") + 1;
					sprintf((char *)&m_tflv.lpdata,"%s","");
					DBTFLV.push_back(m_tflv);
				}
				else
				{
					//��Ʒ����
					for(pItemInfo = VecItemInfo.begin(); pItemInfo != VecItemInfo.end(); pItemInfo++)
					{
						if(pItemInfo->iItemId == atoi((char*)m_DataRecord.recorddata))
						{
							m_tflv.nIndex = DBTFLV.size() + 1;
							m_tflv.m_tagName = CEnumCore::TagName::XDJG_ItemName;
							m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
							m_tflv.m_tvlength = pItemInfo->strItemName.size();
							memcpy(&m_tflv.lpdata, pItemInfo->strItemName.c_str(), pItemInfo->strItemName.size());
							DBTFLV.push_back(m_tflv);
							break;
						}
						else
							continue;
					}

					if(m_tflv.m_tagName != CEnumCore::TagName::XDJG_ItemName)
					{
						m_tflv.nIndex=DBTFLV.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::XDJG_ItemName;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen("") + 1;
						sprintf((char *)&m_tflv.lpdata,"%s","");
						DBTFLV.push_back(m_tflv);
					}
				}
			}
			else if(!strcmp("XDJG_ChoiceItemNum2", m_DataRecord.recordfieldname))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_ChoiceItemNum2;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(m_DataRecord.recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",m_DataRecord.recorddata);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp("XDJG_ChoiceItemID3", m_DataRecord.recordfieldname))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_ChoiceItemID3;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(m_DataRecord.recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",m_DataRecord.recorddata);
				DBTFLV.push_back(m_tflv);
				
				if(atoi((char*)m_DataRecord.recorddata) == 0)
				{
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_ItemName;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=strlen("") + 1;
					sprintf((char *)&m_tflv.lpdata,"%s","");
					DBTFLV.push_back(m_tflv);
				}
				else
				{
					//��Ʒ����
					for(pItemInfo = VecItemInfo.begin(); pItemInfo != VecItemInfo.end(); pItemInfo++)
					{
						if(pItemInfo->iItemId == atoi((char*)m_DataRecord.recorddata))
						{
							m_tflv.nIndex = DBTFLV.size() + 1;
							m_tflv.m_tagName = CEnumCore::TagName::XDJG_ItemName;
							m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
							m_tflv.m_tvlength = pItemInfo->strItemName.size();
							memcpy(&m_tflv.lpdata, pItemInfo->strItemName.c_str(), pItemInfo->strItemName.size());
							DBTFLV.push_back(m_tflv);
							break;
						}
						else
							continue;
					}

					if(m_tflv.m_tagName != CEnumCore::TagName::XDJG_ItemName)
					{
						m_tflv.nIndex=DBTFLV.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::XDJG_ItemName;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen("") + 1;
						sprintf((char *)&m_tflv.lpdata,"%s","");
						DBTFLV.push_back(m_tflv);
					}
				}
			}
			else if(!strcmp("XDJG_ChoiceItemNum3", m_DataRecord.recordfieldname))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_ChoiceItemNum3;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(m_DataRecord.recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",m_DataRecord.recorddata);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp("XDJG_ChoiceItemID4", m_DataRecord.recordfieldname))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_ChoiceItemID4;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(m_DataRecord.recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",m_DataRecord.recorddata);
				DBTFLV.push_back(m_tflv);
				
				if(atoi((char*)m_DataRecord.recorddata) == 0)
				{
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_ItemName;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=strlen("") + 1;
					sprintf((char *)&m_tflv.lpdata,"%s","");
					DBTFLV.push_back(m_tflv);
				}
				else
				{
					//��Ʒ����
					for(pItemInfo = VecItemInfo.begin(); pItemInfo != VecItemInfo.end(); pItemInfo++)
					{
						if(pItemInfo->iItemId == atoi((char*)m_DataRecord.recorddata))
						{
							m_tflv.nIndex = DBTFLV.size() + 1;
							m_tflv.m_tagName = CEnumCore::TagName::XDJG_ItemName;
							m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
							m_tflv.m_tvlength = pItemInfo->strItemName.size();
							memcpy(&m_tflv.lpdata, pItemInfo->strItemName.c_str(), pItemInfo->strItemName.size());
							DBTFLV.push_back(m_tflv);
							break;
						}
						else
							continue;
					}

					if(m_tflv.m_tagName != CEnumCore::TagName::XDJG_ItemName)
					{
						m_tflv.nIndex=DBTFLV.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::XDJG_ItemName;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen("") + 1;
						sprintf((char *)&m_tflv.lpdata,"%s","");
						DBTFLV.push_back(m_tflv);
					}
				}
			}
			else if(!strcmp("XDJG_ChoiceItemNum4", m_DataRecord.recordfieldname))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_ChoiceItemNum4;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(m_DataRecord.recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",m_DataRecord.recorddata);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp("XDJG_ChoiceItemID5", m_DataRecord.recordfieldname))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_ChoiceItemID5;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(m_DataRecord.recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",m_DataRecord.recorddata);
				DBTFLV.push_back(m_tflv);
				
				if(atoi((char*)m_DataRecord.recorddata) == 0)
				{
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_ItemName;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=strlen("") + 1;
					sprintf((char *)&m_tflv.lpdata,"%s","");
					DBTFLV.push_back(m_tflv);
				}
				else
				{
					//��Ʒ����
					for(pItemInfo = VecItemInfo.begin(); pItemInfo != VecItemInfo.end(); pItemInfo++)
					{
						if(pItemInfo->iItemId == atoi((char*)m_DataRecord.recorddata))
						{
							m_tflv.nIndex = DBTFLV.size() + 1;
							m_tflv.m_tagName = CEnumCore::TagName::XDJG_ItemName;
							m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
							m_tflv.m_tvlength = pItemInfo->strItemName.size();
							memcpy(&m_tflv.lpdata, pItemInfo->strItemName.c_str(), pItemInfo->strItemName.size());
							DBTFLV.push_back(m_tflv);
							break;
						}
						else
							continue;
					}

					if(m_tflv.m_tagName != CEnumCore::TagName::XDJG_ItemName)
					{
						m_tflv.nIndex=DBTFLV.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::XDJG_ItemName;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen("") + 1;
						sprintf((char *)&m_tflv.lpdata,"%s","");
						DBTFLV.push_back(m_tflv);
					}
				}
			}
			else if(!strcmp("XDJG_ChoiceItemNum5", m_DataRecord.recordfieldname))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_ChoiceItemNum5;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(m_DataRecord.recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",m_DataRecord.recorddata);
				DBTFLV.push_back(m_tflv);
			}
			else if(!strcmp("XDJG_ChoiceItemID6", m_DataRecord.recordfieldname))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_ChoiceItemID6;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(m_DataRecord.recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",m_DataRecord.recorddata);
				DBTFLV.push_back(m_tflv);
				
				if(atoi((char*)m_DataRecord.recorddata) == 0)
				{
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_ItemName;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=strlen("") + 1;
					sprintf((char *)&m_tflv.lpdata,"%s","");
					DBTFLV.push_back(m_tflv);
				}
				else
				{
					//��Ʒ����
					for(pItemInfo = VecItemInfo.begin(); pItemInfo != VecItemInfo.end(); pItemInfo++)
					{
						if(pItemInfo->iItemId == atoi((char*)m_DataRecord.recorddata))
						{
							m_tflv.nIndex = DBTFLV.size() + 1;
							m_tflv.m_tagName = CEnumCore::TagName::XDJG_ItemName;
							m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
							m_tflv.m_tvlength = pItemInfo->strItemName.size();
							memcpy(&m_tflv.lpdata, pItemInfo->strItemName.c_str(), pItemInfo->strItemName.size());
							DBTFLV.push_back(m_tflv);
							break;
						}
						else
							continue;
					}

					if(m_tflv.m_tagName != CEnumCore::TagName::XDJG_ItemName)
					{
						m_tflv.nIndex=DBTFLV.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::XDJG_ItemName;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen("") + 1;
						sprintf((char *)&m_tflv.lpdata,"%s","");
						DBTFLV.push_back(m_tflv);
					}
				}
			}
			else if(!strcmp("XDJG_ChoiceItemNum6", m_DataRecord.recordfieldname))
			{
				m_tflv.nIndex=DBTFLV.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_ChoiceItemNum6;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(m_DataRecord.recorddata) + 1;
				sprintf((char *)&m_tflv.lpdata,"%s",m_DataRecord.recorddata);
				DBTFLV.push_back(m_tflv);
			}
			else
				continue;

		}
	}
	catch(...)
	{
		return DBTFLV;
	}
	
	return DBTFLV;
}

//��ӳƺ�
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_AddTitle_Query(int userByID, char *ServerIP,char *Account,char *Character,int iTitleID, char *TitleName)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	
	string strResult;
	vector <string> vectorColInfo;
	char strConn[512];
	ZeroMemory(strConn, 512);
	char retMessage[255];
	memset(retMessage, 0, 255);
	
	try
	{
		sprintf(retMessage, "��ӳƺ�:<����>%s, <�ʺ�>%s, <��ɫ>%s, <�ƺ�ID>%d, <�ƺ�����>%s", ServerIP, Account, Character, iTitleID, TitleName);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=add_title(%s,%d)", 
			ServerIP, HTTPPORT,
			Character,iTitleID);
		
		if(m_operXDJG.getWebInfo(strConn, strResult))
		{
			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "��ӳƺ�");				
			}
			else
			{
				int iResult = atoi(strResult.c_str());
				if (iResult>=0)
				{
					
					bool bFinishFlag = false;
					int Maxnum_wait = 10, wait_num=0;
					
					while (!bFinishFlag)
					{
						sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=get_operation_result(%d)", 
							ServerIP, HTTPPORT,
							iResult);
						m_operXDJG.getWebInfo(strConn, strResult);
						int iResultFlag = atoi(strResult.c_str());
						switch (iResultFlag)
						{
						case 1:
							bFinishFlag = true;
							
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ӳɹ�", "��ӳƺ�");
							
							break;
						case -2:
							wait_num++;
							if (wait_num > Maxnum_wait)
							{
								bFinishFlag = true; 
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�ȴ���ʱ", "��ӳƺ�");
							}
							Sleep(1000);
							break;
						default:
							bFinishFlag = true;
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "���ʧ��", "��ӳƺ�");
						}
					}
				}
				else
				{
					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "���ʧ��", "��ӳƺ�");
				}
			}
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "��ӳƺ�");		
		}
	}
	catch (...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "��ӳƺ�");	
	}
	
	m_operXDJG.GmOperLog(userByID, ServerIP, "��ӳƺ�", retMessage, DBTFLV[0]);
	return DBTFLV;
}

//�Ƴ��ƺ�
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_RemoveTitle_Query(int userByID, char *ServerIP,char *Account,char *Character,int iTitleID, char *TitleName)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	
	string strResult;
	vector <string> vectorColInfo;
	char strConn[512];
	ZeroMemory(strConn, 512);
	char retMessage[255];
	memset(retMessage, 0, 255);
	
	try
	{
		sprintf(retMessage, "�Ƴ��ƺ�:<����>%s, <�ʺ�>%s, <��ɫ>%s, <�ƺ�ID>%d, <�ƺ�����>%s", ServerIP, Account, Character, iTitleID, TitleName);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=remove_title(%s,%d)", 
			ServerIP, HTTPPORT,
			Character,iTitleID);
		
		if(m_operXDJG.getWebInfo(strConn, strResult))
		{
			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "�Ƴ��ƺ�");				
			}
			else
			{
				int iResult = atoi(strResult.c_str());
				if (iResult>=0)
				{
					
					bool bFinishFlag = false;
					int Maxnum_wait = 10, wait_num=0;
					
					while (!bFinishFlag)
					{
						sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=get_operation_result(%d)", 
							ServerIP, HTTPPORT,
							iResult);
						m_operXDJG.getWebInfo(strConn, strResult);
						int iResultFlag = atoi(strResult.c_str());
						switch (iResultFlag)
						{
						case 1:
							bFinishFlag = true;
							
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�Ƴ��ɹ�", "�Ƴ��ƺ�");
							
							break;
						case -2:
							wait_num++;
							if (wait_num > Maxnum_wait)
							{
								bFinishFlag = true; 
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�ȴ���ʱ", "�Ƴ��ƺ�");
							}
							Sleep(1000);
							break;
						default:
							bFinishFlag = true;
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�Ƴ�ʧ��", "�Ƴ��ƺ�");
						}
					}
				}
				else
				{
					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�Ƴ�ʧ��", "�Ƴ��ƺ�");
				}
			}
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "�Ƴ��ƺ�");		
		}
	}
	catch (...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "�Ƴ��ƺ�");	
	}
	
	m_operXDJG.GmOperLog(userByID, ServerIP, "�Ƴ��ƺ�", retMessage, DBTFLV[0]);
	return DBTFLV;
}

//���BUFF
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_AddBuff_Query(int userByID, char *ServerIP,char *Account,char *Character,int iBuffID, char *BuffName)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	
	string strResult;
	vector <string> vectorColInfo;
	char strConn[512];
	ZeroMemory(strConn, 512);
	char retMessage[255];
	memset(retMessage, 0, 255);
	
	try
	{
		sprintf(retMessage, "���BUFF:<����>%s, <�ʺ�>%s, <��ɫ>%s, <BUFF ID>%d, <BUFF����>%s", ServerIP, Account, Character, iBuffID, BuffName);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=add_buff(%s,%d)", 
			ServerIP, HTTPPORT,
			Character,iBuffID);
		
		if(m_operXDJG.getWebInfo(strConn, strResult))
		{
			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "���BUFF");				
			}
			else
			{
				int iResult = atoi(strResult.c_str());
				if (iResult>=0)
				{
					
					bool bFinishFlag = false;
					int Maxnum_wait = 10, wait_num=0;
					
					while (!bFinishFlag)
					{
						sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=get_operation_result(%d)", 
							ServerIP, HTTPPORT,
							iResult);
						m_operXDJG.getWebInfo(strConn, strResult);
						int iResultFlag = atoi(strResult.c_str());
						switch (iResultFlag)
						{
						case 1:
							bFinishFlag = true;
							
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "��ӳɹ�", "���BUFF");
							
							break;
						case -2:
							wait_num++;
							if (wait_num > Maxnum_wait)
							{
								bFinishFlag = true; 
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�ȴ���ʱ", "���BUFF");
							}
							Sleep(1000);
							break;
						default:
							bFinishFlag = true;
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "���ʧ��", "���BUFF");
						}
					}
				}
				else
				{
					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "���ʧ��", "���BUFF");
				}
			}
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "���BUFF");		
		}
	}
	catch (...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "���BUFF");	
	}
	
	m_operXDJG.GmOperLog(userByID, ServerIP, "���BUFF", retMessage, DBTFLV[0]);
	return DBTFLV;
}

//�Ƴ�BUFF
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_RemoveBuff_Query(int userByID, char *ServerIP,char *Account,char *Character,int iBuffID, char *BuffName)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	
	string strResult;
	vector <string> vectorColInfo;
	char strConn[512];
	ZeroMemory(strConn, 512);
	char retMessage[255];
	memset(retMessage, 0, 255);
	
	try
	{
		sprintf(retMessage, "���BUFF:<����>%s, <�ʺ�>%s, <��ɫ>%s, <BUFF ID>%d, <BUFF����>%s", ServerIP, Account, Character, iBuffID, BuffName);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=remove_buff(%s,%d)", 
			ServerIP, HTTPPORT,
			Character,iBuffID);
		
		if(m_operXDJG.getWebInfo(strConn, strResult))
		{
			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "�Ƴ�BUFF");				
			}
			else
			{
				int iResult = atoi(strResult.c_str());
				if (iResult>=0)
				{
					
					bool bFinishFlag = false;
					int Maxnum_wait = 10, wait_num=0;
					
					while (!bFinishFlag)
					{
						sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=get_operation_result(%d)", 
							ServerIP, HTTPPORT,
							iResult);
						m_operXDJG.getWebInfo(strConn, strResult);
						int iResultFlag = atoi(strResult.c_str());
						switch (iResultFlag)
						{
						case 1:
							bFinishFlag = true;
							
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�Ƴ��ɹ�", "�Ƴ�BUFF");
							
							break;
						case -2:
							wait_num++;
							if (wait_num > Maxnum_wait)
							{
								bFinishFlag = true; 
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�ȴ���ʱ", "�Ƴ�BUFF");
							}
							Sleep(1000);
							break;
						default:
							bFinishFlag = true;
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�Ƴ�ʧ��", "�Ƴ�BUFF");
						}
					}
				}
				else
				{
					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�Ƴ�ʧ��", "�Ƴ�BUFF");
				}
			}
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "�Ƴ�BUFF");		
		}
	}
	catch (...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "�Ƴ�BUFF");	
	}
	
	m_operXDJG.GmOperLog(userByID, ServerIP, "�Ƴ�BUFF", retMessage, DBTFLV[0]);
	return DBTFLV;
}

//�ƶ���ָ��λ��
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_MoveToPosition_Query(int userByID, char *ServerIP,char *Account,char *Character, int iPX, int iPY, int iPZ)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	
	string strResult;
	vector <string> vectorColInfo;
	char strConn[512];
	ZeroMemory(strConn, 512);
	char retMessage[255];
	memset(retMessage, 0, 255);
	
	try
	{
		sprintf(retMessage, "�ƶ���ָ��λ��:<����>%s, <�ʺ�>%s, <��ɫ>%s, <X����>%d, <Y����>%d, <Z����>%d", ServerIP, Account, Character, iPX, iPY, iPZ);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=move(%s,%d,%d,%d)", 
			ServerIP, HTTPPORT,
			Character,iPX,iPY,iPZ);
		
		if(m_operXDJG.getWebInfo(strConn, strResult))
		{
			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "�ƶ���ָ��λ��");				
			}
			else
			{
				int iResult = atoi(strResult.c_str());
				if (iResult>=0)
				{
					
					bool bFinishFlag = false;
					int Maxnum_wait = 10, wait_num=0;
					
					while (!bFinishFlag)
					{
						sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=get_operation_result(%d)", 
							ServerIP, HTTPPORT,
							iResult);
						m_operXDJG.getWebInfo(strConn, strResult);
						int iResultFlag = atoi(strResult.c_str());
						switch (iResultFlag)
						{
						case 1:
							bFinishFlag = true;
							
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�ƶ��ɹ�", "�ƶ���ָ��λ��");
							
							break;
						case -2:
							wait_num++;
							if (wait_num > Maxnum_wait)
							{
								bFinishFlag = true; 
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�ȴ���ʱ", "�ƶ���ָ��λ��");
							}
							Sleep(1000);
							break;
						default:
							bFinishFlag = true;
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�ƶ�ʧ��", "�ƶ���ָ��λ��");
						}
					}
				}
				else
				{
					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�ƶ�ʧ��", "�ƶ���ָ��λ��");
				}
			}
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "�ƶ���ָ��λ��");		
		}
	}
	catch (...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "�ƶ���ָ��λ��");	
	}
	
	m_operXDJG.GmOperLog(userByID, ServerIP, "�ƶ���ָ��λ��", retMessage, DBTFLV[0]);
	return DBTFLV;
}

//�ƶ���Ŀ��������
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_MoveToRole_Query(int userByID, char *ServerIP,char *Account,char *Character,char *TargetCharacter)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	
	string strResult;
	vector <string> vectorColInfo;
	char strConn[512];
	ZeroMemory(strConn, 512);
	char retMessage[255];
	memset(retMessage, 0, 255);
	
	try
	{
		sprintf(retMessage, "�ƶ���Ŀ��������:<����>%s, <�ʺ�>%s, <��ɫ>%s, <Ŀ���ɫ>%s", ServerIP, Account, Character, TargetCharacter);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=goto(%s,%s)", 
			ServerIP, HTTPPORT,
			Character,TargetCharacter);
		
		if(m_operXDJG.getWebInfo(strConn, strResult))
		{
			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "�ƶ���ָ��������");				
			}
			else
			{
				int iResult = atoi(strResult.c_str());
				if (iResult>=0)
				{
					
					bool bFinishFlag = false;
					int Maxnum_wait = 10, wait_num=0;
					
					while (!bFinishFlag)
					{
						sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=get_operation_result(%d)", 
							ServerIP, HTTPPORT,
							iResult);
						m_operXDJG.getWebInfo(strConn, strResult);
						int iResultFlag = atoi(strResult.c_str());
						switch (iResultFlag)
						{
						case 1:
							bFinishFlag = true;
							
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�ƶ��ɹ�", "�ƶ���Ŀ��������");
							
							break;
						case -2:
							wait_num++;
							if (wait_num > Maxnum_wait)
							{
								bFinishFlag = true; 
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�ȴ���ʱ", "�ƶ���Ŀ��������");
							}
							Sleep(1000);
							break;
						default:
							bFinishFlag = true;
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�ƶ�ʧ��", "�ƶ���Ŀ��������");
						}
					}
				}
				else
				{
					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�ƶ�ʧ��", "�ƶ���Ŀ��������");
				}
			}
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "�ƶ���Ŀ��������");		
		}
	}
	catch (...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "�ƶ���Ŀ��������");	
	}
	
	m_operXDJG.GmOperLog(userByID, ServerIP, "�ƶ���Ŀ��������", retMessage, DBTFLV[0]);
	return DBTFLV;
}

//�ƶ���ָ��������
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_MoveToTargetRole_Query(int userByID, char *ServerIP,char *Account,char *Character,char *TargetCharacter)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	
	string strResult;
	vector <string> vectorColInfo;
	char strConn[512];
	ZeroMemory(strConn, 512);
	char retMessage[255];
	memset(retMessage, 0, 255);
	
	try
	{
		sprintf(retMessage, "�ƶ���ָ��������:<����>%s, <�ʺ�>%s, <��ɫ>%s, <Ŀ���ɫ>%s", ServerIP, Account, Character, TargetCharacter);
		logFile.WriteLog("<�������>",operName,retMessage);//��LOG
		
		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=summon(%s,%s)", 
			ServerIP, HTTPPORT,
			Character,TargetCharacter);
		
		if(m_operXDJG.getWebInfo(strConn, strResult))
		{
			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "�ƶ���ָ��������");				
			}
			else
			{
				int iResult = atoi(strResult.c_str());
				if (iResult>=0)
				{
					
					bool bFinishFlag = false;
					int Maxnum_wait = 10, wait_num=0;
					
					while (!bFinishFlag)
					{
						sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=get_operation_result(%d)", 
							ServerIP, HTTPPORT,
							iResult);
						m_operXDJG.getWebInfo(strConn, strResult);
						int iResultFlag = atoi(strResult.c_str());
						switch (iResultFlag)
						{
						case 1:
							bFinishFlag = true;
							
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�ƶ��ɹ�", "�ƶ���ָ��������");
							
							break;
						case -2:
							wait_num++;
							if (wait_num > Maxnum_wait)
							{
								bFinishFlag = true; 
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�ȴ���ʱ", "�ƶ���ָ��������");
							}
							Sleep(1000);
							break;
						default:
							bFinishFlag = true;
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�ƶ�ʧ��", "�ƶ���ָ��������");
						}
					}
				}
				else
				{
					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "�ƶ�ʧ��", "�ƶ���ָ��������");
				}
			}
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "����ʧ��", "�ƶ���ָ��������");		
		}
	}
	catch (...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "δ֪����", "�ƶ���ָ��������");	
	}
	
	m_operXDJG.GmOperLog(userByID, ServerIP, "�ƶ���ָ��������", retMessage, DBTFLV[0]);
	return DBTFLV;
}

//�ƺ��б��ѯ
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_TitleList_Query(char *Gamename, char *Sql)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	try
	{
		DBTFLV=GameDBQuery(Gamename,"127.0.0.1",4,0,0,Sql);
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog(Gamename,"�����б��ѯ�ڴ治��");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog(Gamename,"�����б��ѯ�ļ��쳣");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog(Gamename,"�����б��ѯ�����쳣");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog(Gamename,(char *)err.what());
		return DBTFLV;
	}
	return DBTFLV;
}

//BUFF�б��ѯ
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_BuffList_Query(char *Gamename, char *Sql)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	try
	{
		DBTFLV=GameDBQuery(Gamename,"127.0.0.1",4,0,0,Sql);
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog(Gamename,"�����б��ѯ�ڴ治��");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog(Gamename,"�����б��ѯ�ļ��쳣");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog(Gamename,"�����б��ѯ�����쳣");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog(Gamename,(char *)err.what());
		return DBTFLV;
	}
	return DBTFLV;
}
//End

/*


  vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_Net_BoardTasker_Query(int iIndex, int iPageSize)
  {
  vector <CGlobalStruct::TFLV> DBTFLV;
  CGlobalStruct::TFLV m_tflv;
  vector <string> vectorInfo;
  vector <string> vectorBoardId;
  vector <string> vectorColInfo;
  char strConn[512];
  char retMessage[512];
  
	ZeroMemory(strConn, 512);
	sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=query_notice(0)",
	ServerIP, port			
	);
	
	  if (m_operXDJG.getWebInfo(strConn, vectorInfo))
	  {
	  
		}
		else
		{
		sprintf(retMessage,"%s","����ʧ��");
		m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
		m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
		sprintf((char *)&m_tflv.lpdata,"%s",(char *)&retMessage);
		m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
		logFile.WriteLog("XDJG","XDJG",retMessage);
		DBTFLV.push_back(m_tflv);
		}
		
		  return DBTFLV;
		  }
		  
			bool CXDJGInfo::getBoardTaskerDetail(vector<string> &vectorDetail, int notice_index)
			{
			char strConn[512];
			ZeroMemory(strConn, 512);
			sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=query_notice(%d)",
			ServerIP, port,			
			notice_index);
			
			  if (m_operXDJG.getWebInfo(strConn, vectorDetail))
			  {
			  int iResult = atoi((vectorDetail.front()).c_str());
			  if (iResult<0)
			  {
			  return false;
			  }
			  
				}
				else
				{
				return false;		
				}
				
				  return true;
}

vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_Net_BoardTasker_Oper(int userByID, int op_type,char *content, char *begintime, int interval, int repeat, char * begindate, char *enddate, int notice_index)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	string strResult;
	char strConn[512];
	char retMessage[512];

	ZeroMemory(strConn, 512);
	sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=notice_operation(%d,%d,%s,%s,%d,%d,%s,%s)",
			ServerIP, port,
			op_type, notice_index, content, begintime, interval, repeat, begindate, enddate);
	
	if (m_operXDJG.getWebInfo(strConn, strResult))
	{
		int iResult = atoi(strResult.c_str());
		switch (iResult)
		{
		case 0:
			{
				switch (op_type)
				{
				case 1:
					sprintf(retMessage,"%s","��ӹ���ɹ�");
					m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata,"%s",(char *)&retMessage);
					m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
					logFile.WriteLog("XDJG","XDJG",retMessage);
					DBTFLV.push_back(m_tflv);
					break;
				case 2:
					DBTFLV = m_operXDJG.ReturnOpMsg(strLog,"Update_BoardTasker_Success");
					sprintf(retMessage,"%s","�޸Ĺ���ɹ�");
					m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata,"%s",(char *)&retMessage);
					m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
					logFile.WriteLog("XDJG","XDJG",retMessage);
					DBTFLV.push_back(m_tflv);
					break;
				case 3:
					DBTFLV = m_operXDJG.ReturnOpMsg(strLog,"Delete_BoardTasker_Success");
					sprintf(retMessage,"%s","ɾ������ɹ�");
					m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata,"%s",(char *)&retMessage);
					m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
					logFile.WriteLog("XDJG","XDJG",retMessage);
					DBTFLV.push_back(m_tflv);
					break;
				}
			}
			break;
		default:
			{
				switch (op_type)
				{
				case 1:
					//DBTFLV = m_operXDJG.ReturnOpMsg(strLog,"Add_BoardTasker_Fail");
					sprintf(retMessage,"%s","��ӹ���ʧ��");
					m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata,"%s",(char *)&retMessage);
					m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
					logFile.WriteLog("XDJG","XDJG",retMessage);
					DBTFLV.push_back(m_tflv);
					break;
				case 2:
					//DBTFLV = m_operXDJG.ReturnOpMsg(strLog,"Update_BoardTasker_Fail");
					sprintf(retMessage,"%s","�޸Ĺ���ʧ��");
					m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata,"%s",(char *)&retMessage);
					m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
					logFile.WriteLog("XDJG","XDJG",retMessage);
					DBTFLV.push_back(m_tflv);
					break;
				case 3:
					//DBTFLV = m_operXDJG.ReturnOpMsg(strLog,"Delete_BoardTasker_Fail");
					sprintf(retMessage,"%s","ɾ������ʧ��");
					m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata,"%s",(char *)&retMessage);
					m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
					logFile.WriteLog("XDJG","XDJG",retMessage);
					DBTFLV.push_back(m_tflv);
					break;
				}
			}
			break;
		}
	}
	else
	{
		sprintf(retMessage,"%s","����ʧ��");
		m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
		m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
		sprintf((char *)&m_tflv.lpdata,"%s",(char *)&retMessage);
		m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
		logFile.WriteLog("XDJG","XDJG",retMessage);
		DBTFLV.push_back(m_tflv);
	}

	return DBTFLV;
}
*/
