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

//初始化类中的变量
COperVector * CXDJGInfo::initialize(CSession * pSess,CEnumCore::Message_Tag_ID MessageTagID, unsigned char * pbuf, int length)
{
	COperVector * operVector = NULL;
	try
	{
		ZeroMemory(LocalIP, 30);
		ZeroMemory(AccountDB, 30);
		ZeroMemory(LogDB, 30);
		logFile.ReadValue("Scheme", "DATABASE", "Address",LocalIP, 30);//获取本地的数据库地址

		logFile.ReadValue("Database", "XDJG", "AccountDB", AccountDB, 30);

		logFile.ReadValue("Database", "XDJG", "LogDB", LogDB, 30);

		logFile.ReadValue("Database", "XDJG", "CharacterDB", GameDB, 30);

		//Added by tengjie 2010-07-08:For ItemName
//		m_VecItemInfo.clear();
//		GetItemInfo();
		//End

//		port = logFile.ReadIntValue("XDJG", "SERVER", "PORT");

		//调用基类初始化方法
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
			_ItemInfo.strItemName = (char*)iter->recorddata;//记录道具名称
			VecItemInfo.push_back(_ItemInfo);
		}
		else
			continue;
	}

	printf("道具个数:%d\n",VecItemInfo.size());
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
		logFile.WriteLog("<侠盗金刚>",operName, strParams);//记LOG打印输入参数

		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=query_player(%s,%d)",
			ServerIP, HTTPPORT,
			strCharacter.c_str(), type);
		
		if(m_operXDJG.getWebInfo(strConn, strResult))
		{
			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "用户查询");				
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
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "等待超时", "用户查询");
							}
							Sleep(1000);
							break;
						default:
							bFinishFlag = true;
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "查询失败", "用户查询");	
						}
					}
				}
				else
				{
					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "操作失败", "用户查询");
					
				}
			}
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "用户查询");
		}
	}
	catch (...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "用户查询");
	}

	delete []strConn;
	delete []strParams;

	return DBTFLV;
}

//用户信息查询
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
		sprintf(strParams, "[角色信息查询]Account:%s Character:%s ServerIP:%s type:%d\n", \
			Account, Character, ServerIP, type);
			logFile.WriteLog("<侠盗金刚>",operName, strParams);//记LOG打印输入参数

		if(strcmp(Account,""))
		{
			if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,3))
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "用户信息查询");
				return DBTFLV;//无法获取真正的服务器IP
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

			//取已删除的角色信息
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
					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "用户查询");				
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
									DBTFLV = m_operXDJG.ReturnOpMsg(operName, "等待超时", "用户查询");
									
								}
								Sleep(1000);
								break;
							default:
								bFinishFlag = true;
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "查询失败", "用户查询");
								
							}
						}
					}
					else
					{
						DBTFLV = m_operXDJG.ReturnOpMsg(operName, "操作失败", "用户查询");
						
					}		
					
				}
				
			}
			else
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "用户查询");
				
			}
		}

	}catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "用户查询");
	}

	delete []strConn;
	delete []strParams;

	return DBTFLV;
}

//取已删除的角色信息
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
		sprintf(strParams, "[已删除的角色信息查询]Account:%s ServerIP:%s\n", Account, ServerIP);
		logFile.WriteLog("<侠盗金刚>",operName, strParams);//记LOG打印输入参数

		strSql.Format("SELECT NAME AS XDJG_Character, guid AS XDJG_CharacterID, account_name AS XDJG_Account, \
							  account_id AS XDJG_AccountID, grade AS XDJG_Level, property AS XDJG_RoleProperty, \
							  CASE sex WHEN 0 THEN '女' ELSE '男' END AS XDJG_Sex, \
							  CASE profession WHEN 1 THEN '战士' \
											  WHEN 2 THEN '隐匿者' \
											  WHEN 3 THEN '机械师' \
											  WHEN 4 THEN '控能师' \
											  WHEN 5 THEN '守护者' \
											  WHEN 6 THEN '侵蚀者' \
											  ELSE '未知' END AS XDJG_Profession, scene_type_id AS XDJG_SceneTypeID, \
							  scene_obj_id AS XDJG_SceneInstanceID,position_x AS XDJG_PositionX,position_y AS XDJG_PositionY, \
							  position_z AS XDJG_PositionZ, money AS XDJG_Money,'已删除' AS XDJG_IsDeleted \
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "已删除的角色信息查询");//将错误信息写入日志中并返回错误信息
	}
	
	delete []strParams;

	return DBTFLV;
}

//内部调用用户信息查询
bool CXDJGInfo::getUserInfoDataset(vector <CGlobalStruct::TFLV> &DataSetTFLV, vector<string> vectorCol, char *Character, char *ServerIP, int iIndex, int iPageSize, int type)
{

	try
	{
		char strParams[512];
		ZeroMemory(strParams,512);
		sprintf(strParams, "Character:%s ServerIP:%s type:%d\n", Character, ServerIP, type);
		logFile.WriteLog("<侠盗金刚>",operName, strParams);//记LOG打印输入参数

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

		//根据页数和每页记录数，返回需要的查询数据
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
					m_tflv.m_tvlength=strlen("未删除")+1;
					sprintf((char *)&m_tflv.lpdata,"%s","未删除");
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
			//处理详细信息显示
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

				//防御信息:
				case 23:
					//护甲值
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_Armor;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=(*iter_newDataVector).length();
					sprintf((char *)&m_tflv.lpdata,"%s",(*iter_newDataVector).c_str());
					DataSetTFLV.push_back(m_tflv);
					break;
				case 24:
					//闪躲率
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_DodgeRate;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=(*iter_newDataVector).length();
					sprintf((char *)&m_tflv.lpdata,"%s",(*iter_newDataVector).c_str());
					DataSetTFLV.push_back(m_tflv);
					break;
				case 25:
					//格挡率
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_WardRate;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=(*iter_newDataVector).length();
					sprintf((char *)&m_tflv.lpdata,"%s",(*iter_newDataVector).c_str());
					DataSetTFLV.push_back(m_tflv);
					break;
				case 26:
					//招架率
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_ParryRate;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=(*iter_newDataVector).length();
					sprintf((char *)&m_tflv.lpdata,"%s",(*iter_newDataVector).c_str());
					DataSetTFLV.push_back(m_tflv);
					break;
				case 27:
					//致命抵抗率
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_FatalResistanceRate;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=(*iter_newDataVector).length();
					sprintf((char *)&m_tflv.lpdata,"%s",(*iter_newDataVector).c_str());
					DataSetTFLV.push_back(m_tflv);
					break;
				case 28:
					//异能抗性
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
					//善恶
					m_tflv.nIndex=DataSetTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_GoodEvil;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=(*iter_newDataVector).length();
					sprintf((char *)&m_tflv.lpdata,"%s",(*iter_newDataVector).c_str());
					DataSetTFLV.push_back(m_tflv);
					break;
				case 31:
					//红名时间
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

			//获取角色称号列表
			DBTFLV = GetRoleTitles(ServerIP, Character);
			for(pDBTFLV = DBTFLV.begin(); pDBTFLV != DBTFLV.end(); pDBTFLV++)
			{
				DataSetTFLV.push_back(*pDBTFLV);
			}

			//取生活技能列表
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

//获取角色名称列表
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
		sprintf(strParams, "[角色名称列表]ServerIP:%s Character:%s\n", ServerIP, Character);
		logFile.WriteLog("<侠盗金刚>",operName, strParams);//记LOG打印输入参数

		//角色称号,生活技能
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,3))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "角色名称列表");
			return DBTFLV;//无法获取真正的服务器IP
		}
		
		//角色称号列表
		strSql.Format("Select titles AS XDJG_Titles \
						 from %s.character \
						where name = '%s'",GameDB, Character);
		
		m_SqlHelper.SqlHelperMain("XDJG", &datarecord,realServerIP,strSql.GetBuffer(0),12,0,0);
		
		vector<CGlobalStruct::DataRecord>::iterator iter;
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			if(!strcmp("XDJG_Titles", iter->recordfieldname))
			{
				memcpy(&m_StructInfo.m_sCharacterTitle, iter->recorddata, sizeof(m_StructInfo.m_sCharacterTitle));//物品属性
				break;
			}
			else
				continue;
		}

		//角色称号列表
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

//获取角色生活技能列表
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
		sprintf(strParams, "[角色生活技能列表]ServerIP:%s Character:%s\n", ServerIP, Character);
		logFile.WriteLog("<侠盗金刚>",operName, strParams);//记LOG打印输入参数

		//角色称号,生活技能
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,3))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "公会基本信息查询");
			return DBTFLV;//无法获取真正的服务器IP
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

//获取生活技能名称
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
			strSkillName = "冶金";
			break;
		case 2:
			strSkillName = "布皮";
			break;
		case 3:
			strSkillName = "生物";
			break;
		case 4:
			strSkillName = "能源";
			break;
		case 5:
			strSkillName = "化学";
			break;
		case 6:
			strSkillName = "锻造";
			break;
		case 7:
			strSkillName = "巧工";
			break;
		case 8:
			strSkillName = "机械";
			break;
		case 9:
			strSkillName = "循环";
			break;
		case 10:
			strSkillName = "芯片";
			break;
		case 11:
			strSkillName = "烹饪";
			break;
		case 12:
			strSkillName = "驾驶";
			break;
		case 13:
			strSkillName = "商业";
			break;
		case 14:
			strSkillName = "普通拾取";
			break;
		default:
			strSkillName = "未知";
			break;
		}
	}
	catch (...)
	{
		return strSkillName;
	}

	return strSkillName;
}

//通过接口踢人：帐号踢人和角色踢人
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
		sprintf(retMessage, "踢人：<大区>%s, <帐号>%s, <角色名>%s", ServerIP, Account, Character);
		logFile.WriteLog("<侠盗金刚>",operName,"XDJG踢人");//记LOG
		
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
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "用户不在线", "XDJG踢人");
				break;
			case '1':
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "踢人成功", "XDJG踢人");
				break;
			default:
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "踢人失败", "XDJG踢人");
				break;
			}
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "XDJG踢人");
		}

	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "XDJG踢人");

	}

	m_operXDJG.GmOperLog(userByID, ServerIP, "XDJG踢人", retMessage, DBTFLV[0]);
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
		sprintf(message,"[玩家封停列表查询]:<大区>%s",ServerIP);
		logFile.WriteLog("<侠盗金刚>",operName,message);//记LOG

		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,4))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "公会基本信息查询");
			return DBTFLV;//无法获取真正的服务器IP
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "封停列表查询");;//将错误信息写入日志中并返回错误信息
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
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "XDJG踢人");

		switch(iReasonType)
		{
		case _ST_XDJG::WEB_FREEZE:
			szData.Format("该账号已被冻结\n，请于%s后登陆",XDJG_CalEndFreezeTime(iBanTime).GetBuffer(0));
			sprintf(retMessage, "玩家封停：<大区>%s, <帐号>%s, <封停时间>%d天, <封停原因>%s", ServerIP, Account, iTime,szData.GetBuffer(0));
			sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=disable_account(%s,%d,%s,%d)", 
					ServerIP, HTTPPORT,
					Account, iBanTime, "被冻结",iReasonType);
			break;
		case _ST_XDJG::WEB_LOCK:
			szData.Format("%s","该账号已申请页面锁定，请登陆官方网页，进行解锁后方可登陆");
			sprintf(retMessage, "玩家封停：<大区>%s, <帐号>%s, <封停时间>%d天, <封停原因>%s", ServerIP, Account, iTime,Reason);
			sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=disable_account(%s,%d,%s,%d)", 
					ServerIP, HTTPPORT,
					Account, iBanTime, "被锁定",iReasonType);
			break;
		case _ST_XDJG::MAX_LOGINTIMES:
			sprintf(retMessage, "玩家封停：<大区>%s, <帐号>%s, <封停时间>%d天, <封停原因>%s", ServerIP, Account, iTime,Reason);
			sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=disable_account(%s,%d,%s,%d)", 
				ServerIP, HTTPPORT,
				Account, iBanTime, "达到最大错误登录计数",iReasonType);
			break;
		case _ST_XDJG::BANED_BYGM:
			sprintf(retMessage, "玩家封停：<大区>%s, <帐号>%s, <封停时间>%d天, <封停原因>%s", ServerIP, Account, iTime,Reason);
			sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=disable_account(%s,%d,%s,%d)", 
				ServerIP, HTTPPORT,
				Account, iBanTime, "被GM封停",iReasonType);
			break;
		case _ST_XDJG::BANED_BYPLUG:
			sprintf(retMessage, "玩家封停：<大区>%s, <帐号>%s, <封停时间>%d天, <封停原因>%s", ServerIP, Account, iTime,Reason);
			sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=disable_account(%s,%d,%s,%d)", 
				ServerIP, HTTPPORT,
				Account, iBanTime, "外挂封停",iReasonType);
			break;
		case _ST_XDJG::NONE_REASON:
			sprintf(retMessage, "玩家封停：<大区>%s, <帐号>%s, <封停时间>%d天, <封停原因>%s", ServerIP, Account, iTime,Reason);
			sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=disable_account(%s,%d,%s,%d)", 
				ServerIP, HTTPPORT,
				Account, iBanTime, "无条件封停",iReasonType);
			break;
		default:
			sprintf(retMessage, "玩家封停：<大区>%s, <帐号>%s, <封停时间>%d天, <封停原因>%s", ServerIP, Account, iTime,Reason);
			sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=disable_account(%s,%d,%s,%d)", 
					ServerIP, HTTPPORT,
					Account, iBanTime, Reason,iReasonType);
			break;
		}
		
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG

		if(m_operXDJG.getWebInfo(strConn, strResult))
		{
			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "XDJG封停");				
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
							
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "封停成功", "XDJG封停");
							/*
							
							  if(	GameDBOper("XDJG","127.0.0.1",4,"exec XDJG_BanUser %d, %s, %s, %s,%d,'%s'",
							  userByID,
							  Account,
							  AccountID,
							  ServerIP,
							  iTime,
							  Reason))
							  {
							  DBTFLV = m_operXDJG.ReturnOpMsg(operName, "封停成功", "XDJG封停");
							  }
							  else
							  {
							  DBTFLV = m_operXDJG.ReturnOpMsg(operName, "插入本地数据库错误", "XDJG封停");
						}*/
							
							
							break;
						case -2:
							wait_num++;
							if (wait_num > Maxnum_wait)
							{
								bFinishFlag = true; 
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "稍后再试", "XDJG封停");
								
							}
							Sleep(1500);
							break;
						default:
							bFinishFlag = true;
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "封停失败", "XDJG封停");
						}
					}
				}
				else
				{
					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "操作失败", "XDJG封停");
				}

			}

		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "XDJG封停");
		}
		
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "XDJG封停");

	}

	m_operXDJG.GmOperLog(userByID, ServerIP, "XDJG封停", retMessage, DBTFLV[0]);
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
		sprintf(retMessage, "玩家解封:<大区>%s, <帐号>%s, <解封原因>%s", ServerIP, Account, Reason);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=enable_account(%s,%d)", 
			ServerIP, HTTPPORT,
			Account,iReasonType);
		
		if(m_operXDJG.getWebInfo(strConn, strResult))
		{
			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "玩家解封");				
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
							
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "解封成功", "玩家解封");

							break;
						case -2:
							wait_num++;
							if (wait_num > Maxnum_wait)
							{
								bFinishFlag = true; 
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "等待超时", "玩家解封");
								
							}
							Sleep(1000);
							break;
						default:
							bFinishFlag = true;
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "解封失败", "玩家解封");
						}
					}
				}
				else
				{
					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "操作失败", "玩家解封");
				}

			}

		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "玩家解封");		
		}
	}
	catch (...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "玩家解封");	
	}

	m_operXDJG.GmOperLog(userByID, ServerIP, "玩家解封", retMessage, DBTFLV[0]);
	return DBTFLV;
}
//查询公告
vector<CGlobalStruct::TFLV> CXDJGInfo::XDJG_Net_BoardTasker_Query(char * GameName,char * m_sql,int iIndex,int iPageSize)
{
	logFile.WriteLog("<侠盗金刚>",operName,"公告查询");//记LOG
	return GameDBQuery(GameName, LocalIP, 4, iIndex, iPageSize,m_sql);
}

//添加公告
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_Net_BoardTasker_Add(int userByID, char * ServerIP, char *BoardMessage, char *BeginTime, char *EndTime, int interval)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	char retMessage[255];
	memset(retMessage, 0, 255);
	
	try
	{
		sprintf(retMessage, "添加公告:<大区>%s, <公告>%s, <开始时间>%s, <结束时间>%s", ServerIP, BoardMessage, BeginTime, EndTime);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG

		printf("exec XDJG_BOARDTASK_INSERT %i,'%s','%s','%s','%s',%i", 
								userByID,ServerIP,BoardMessage,BeginTime, EndTime, interval);

		int tmpLength = 0;
		tmpLength = GameDBOper("XDJG",LocalIP,4,"exec XDJG_BOARDTASK_INSERT %i,'%s','%s','%s','%s',%i",
								userByID,ServerIP,BoardMessage,BeginTime, EndTime, interval);
		
		if(tmpLength == 0)
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "添加公告失败", "公告管理");
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "添加公告成功", "公告管理");
		}
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "添加公告错误", "公告管理");
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
		sprintf(retMessage, "公告修改:<大区>%s, <公告ID>%d, <公告>%s, <开始时间>%s, <结束时间>%s", ServerIP, Taskid, BoardMessage, BeginTime, EndTime);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG

		printf("exec XDJG_BOARDTASK_UPDATE %i, %i, '%s','%s','%s','%s',%i", 
			userByID,Taskid, ServerIP,BoardMessage,BeginTime, EndTime, interval);		
		int tmpLength = 0;
		tmpLength = GameDBOper("XDJG",LocalIP,4,"exec XDJG_BOARDTASK_UPDATE %i, %i, '%s','%s','%s','%s',%i", 
			userByID,Taskid, ServerIP,BoardMessage,BeginTime, EndTime, interval);
		
		if(tmpLength == 0)
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "更新公告失败", "公告管理");
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "更新公告成功", "公告管理");
		}
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "更新公告错误", "公告管理");
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
		sprintf(retMessage, "公告删除:<公告ID>%d", Taskid);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG

		int tmpLength = 0;
		tmpLength = GameDBOper("XDJG",LocalIP,4,"exec XDJG_BOARDTASK_DELETE %i, %i", 
			userByID,Taskid);
		
		if(tmpLength == 0)
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "删除公告失败", "公告管理");
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "删除公告成功", "公告管理");
		}
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "删除公告错误", "公告管理");
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

		szTime.Format("%04d年%02d月%02d日 %02d:%02d",timeDest.wYear, timeDest.wMonth, timeDest.wDay, timeDest.wHour, timeDest.wMinute);
	}
	catch (...)
	{
		
	}

	return szTime;
}

//Added by tengjie 2010-07-06
//角色技能添加
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
		sprintf(retMessage, "角色技能添加:<大区>%s, <角色名>%s, <技能ID>%d", ServerIP, Character, SkillID);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,3))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "角色技能添加");
			return DBTFLV;//无法获取真正的服务器IP
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
				memcpy(&m_StructInfo.m_sSkillCont, m_DataRecord.recorddata, sizeof(m_StructInfo.m_sSkillCont));//物品属性
				break;
			}
			else
				continue;
		}

		for(iCount = 0; iCount < m_StructInfo.m_sSkillCont.skill_count; iCount++)
		{
			if(SkillID == m_StructInfo.m_sSkillCont.skill_list[iCount])
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "该角色已拥有此技能", "XDJG角色技能添加");
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
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "XDJG角色技能添加");				
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
							
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "添加技能成功", "XDJG角色技能添加");
							
							break;
						case -2:
							wait_num++;
							if (wait_num > Maxnum_wait)
							{
								bFinishFlag = true; 
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "添加技能超时", "XDJG角色技能添加");
								
							}
							Sleep(1000);
							break;
						default:
							bFinishFlag = true;
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "添加技能失败", "XDJG角色技能添加");
						}
					}
				}
				else
				{
					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "操作失败", "XDJG角色技能添加");
				}
				
			}
			
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "XDJG角色技能添加");		
		}
	}
	catch (...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "XDJG角色技能添加");
	}
	
	m_operXDJG.GmOperLog(userByID, ServerIP, "XDJG角色技能添加", retMessage, DBTFLV[0]);
	return DBTFLV;
}

//角色技能删除
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
		sprintf(retMessage, "角色技能删除:<大区>%s, <角色名>%s, <技能ID>%d", ServerIP, Character, SkillID);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=remove_skill(%s,%d)", 
			ServerIP, HTTPPORT,
			Character,SkillID);
		
		if(m_operXDJG.getWebInfo(strConn, strResult))
		{
			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "XDJG角色技能删除");				
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
							
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "删除技能成功", "XDJG角色技能删除");
							
							break;
						case -2:
							wait_num++;
							if (wait_num > Maxnum_wait)
							{
								bFinishFlag = true; 
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "删除技能超时", "XDJG角色技能删除");
								
							}
							Sleep(1000);
							break;
						default:
							bFinishFlag = true;
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "删除技能失败", "XDJG角色技能删除");
						}
					}
				}
				else
				{
					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "操作失败", "XDJG角色技能删除");
				}
			}
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "XDJG角色技能删除");		
		}
	}
	catch (...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "XDJG角色技能删除");	
	}
	
	m_operXDJG.GmOperLog(userByID, ServerIP, "XDJG角色技能删除", retMessage, DBTFLV[0]);
	return DBTFLV;
}

//道具添加
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
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "不能发送空道具", "XDJG邮件寄送道具");
			return DBTFLV;
		}

		if (!strcmp(mailTitle,""))
		{
			strcpy(mailTitle,"系统邮件");
		}

		if(!strcmp(mailContent,""))
		{
			strcpy(mailContent,"系统邮件");
		}

		//分析道具属性信息并填入数组
		AnalysisItemInfo(ItemID,ItemNum,PropertyId,PropertyValue,IncreaseNum,IncreaseType,IncreaseValue,IncreaseEndValue,StarsLevel,AssemblyID,SlotNum,EquipSlotID1,EquipSlotID2,EquipSlotID3,QualityEquip,IsBind/*,ItemName*/);

		if(m_VecSendItemInfo.size() == 0)
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "没有道具信息", "XDJG邮件寄送道具");
			return DBTFLV;
		}

		for(pSendItemInfo = m_VecSendItemInfo.begin(); pSendItemInfo != m_VecSendItemInfo.end(); pSendItemInfo++)
		{
			//添加道具
			if(SendSingleItem(ServerIP, Character, SendCharacter, mailTitle, mailContent, Money, pSendItemInfo))
			{
				szBack.Format("邮件发送成功(道具id:%d, 道具数量:%d)\n", pSendItemInfo->iItemID, pSendItemInfo->iItemNum);
				strResult += szBack.GetBuffer(0);

				//设置道具属性(该接口一次只能修改一种属性)
//				SetItemAttribute(ServerIP, Character, pSendItemInfo);
			}
			else
			{
				szBack.Format("邮件发送失败(道具id:%d, 道具数量:%d)\n", pSendItemInfo->iItemID, pSendItemInfo->iItemNum);
				strResult += szBack.GetBuffer(0);
			}
		}

		DBTFLV = m_operXDJG.ReturnOpMsg(operName, (char *)strResult.c_str(), "XDJG邮件寄送道具");

	}
	catch (...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "XDJG邮件寄送道具");	
	}
	
	m_operXDJG.GmOperLog(userByID, ServerIP, "XDJG邮件寄送道具", (char *)strResult.c_str(), DBTFLV[0]);
	return DBTFLV;
}

//添加道具
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
		
		sprintf(retMessage, "添加道具:<大区>%s, <角色名>%s, <道具ID>%d, <道具数目>%d, <邮件标题>%s, <邮件内容>%s, <金钱数>%d", ServerIP, Character, pSendItemInfo->iItemID, pSendItemInfo->iItemNum, mailTitle, mailContent, Money);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
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
// 		sprintf(retMessage, "道具添加:<大区>%s, <角色名>%s, <道具ID>%d, <道具数目>%d", ServerIP, Character, pSendItemInfo->iItemID, pSendItemInfo->iItemNum);
// 		logFile.WriteLog("<侠盗金刚>",operName,"道具添加");//记LOG
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

//设置道具属性
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
// 		sprintf(retMessage, "设置道具属性:<大区>%s, <角色名>%s, <道具ID>%d, <道具数目>%d", ServerIP, Character, pSendItemInfo->iItemID, pSendItemInfo->iItemNum);
// 		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
// 		
// 		//是否绑定
// // 		if(pSendItemInfo->iIsBind == 0)
// // 			SetItemAttr(ServerIP,Character,9,0,0);
// // 		else if(pSendItemInfo->iIsBind == 1)
// // 			SetItemAttr(ServerIP,Character,9,1,0);
// // 
// // 		//打孔数
// // 		if(pSendItemInfo->iSlotNum != 0)
// // 			SetItemAttr(ServerIP,Character,8,pSendItemInfo->iSlotNum,0);
// // 
// // 		//星数
// // 		if(pSendItemInfo->iStarsLevel != 0)
// // 			SetItemAttr(ServerIP,Character,3,pSendItemInfo->iStarsLevel,0);
// // 
// // 		//增幅次数
// // 		if(pSendItemInfo->iIncreaseNum != 0)
// // 			SetItemAttr(ServerIP,Character,2,pSendItemInfo->iIncreaseNum,0);
// // 
// // 		//装备资质
// // 		if(pSendItemInfo->iQualityEquip == 1)
// // 			SetItemAttr(ServerIP,Character,4,1,0);
// // 		else if(pSendItemInfo->iQualityEquip == 2)
// // 			SetItemAttr(ServerIP,Character,4,2,0);
// // 
// // 		//增幅属性
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

//设置属性
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
// 		logFile.WriteLog("<侠盗金刚>",operName,"设置道具属性");//记LOG
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

//分析道具属性信息并填入数组
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

		//道具id
		char *pitemID = strtok(ItemID,";");
		while(pitemID != NULL)
		{
			VecItemID.push_back(atoi(pitemID));
			
			pitemID = strtok(NULL,";");
		}
		
		//道具数量
		char *pitemNum = strtok(ItemNum,";");
		while(pitemNum != NULL)
		{
			VecItemNum.push_back(atoi(pitemNum));
			
			pitemNum = strtok(NULL,";");
		}

		//附加属性id
		char *pPropertyId = strtok(PropertyId,";");
		while(pPropertyId != NULL)
		{
			//转换成16进制字符串
			VecPropertyId.push_back(ConvertTo16(atoi(pPropertyId)));
			
//			VecPropertyId.push_back(atoi(pPropertyId));
			pPropertyId = strtok(NULL,";");
		}

		//附加属性值
		char *pPropertyValue = strtok(PropertyValue,";");
		while(pPropertyValue != NULL)
		{
			//转换成16进制字符串
			VecPropertyValue.push_back(ConvertTo16(atoi(pPropertyValue)));

//			VecPropertyValue.push_back(atoi(pPropertyValue));
			pPropertyValue = strtok(NULL,";");
		}

		//增幅等级
		char *pIncreaseNum = strtok(IncreaseNum,";");
		while(pIncreaseNum != NULL)
		{
			//转换成16进制字符串
			VecIncreaseNum.push_back(ConvertTo16(atoi(pIncreaseNum)));
			
//			VecIncreaseNum.push_back(atoi(pIncreaseNum));
			pIncreaseNum = strtok(NULL,";");
		}

		//增幅属性
		char *pIncreaseType = strtok(IncreaseType,";");
		while(pIncreaseType != NULL)
		{
			//转换成16进制字符串
			VecIncreaseType.push_back(ConvertTo16(atoi(pIncreaseType)));

//			VecIncreaseType.push_back(atoi(pIncreaseType));
			
			pIncreaseType = strtok(NULL,";");
		}

		//增幅值
		char *pIncreaseValue = strtok(IncreaseValue,";");
		while(pIncreaseValue != NULL)
		{
			//转换成16进制字符串
			VecIncreaseValue.push_back(ConvertTo16(atoi(pIncreaseValue)));

//			VecIncreaseValue.push_back(atoi(pIncreaseValue));
			pIncreaseValue = strtok(NULL,";");
		}

		//上回增幅值
		char *pIncreaseEndValue = strtok(IncreaseEndValue,";");
		while(pIncreaseEndValue != NULL)
		{
			//转换成16进制字符串
			VecIncreaseEndValue.push_back(ConvertTo16(atoi(pIncreaseEndValue)));
			
//			VecIncreaseEndValue.push_back(atoi(pIncreaseEndValue));
			pIncreaseEndValue = strtok(NULL,";");
		}

		//星数
		char *pStarsLevel = strtok(StarsLevel,";");
		while(pStarsLevel != NULL)
		{
			//转换成16进制字符串
			VecStarsLevel.push_back(ConvertTo16(atoi(pStarsLevel)));
			
//			VecStarsLevel.push_back(atoi(pStarsLevel));
			pStarsLevel = strtok(NULL,";");
		}

		//组装id
		char *pAssemblyID = strtok(AssemblyID,";");
		while(pAssemblyID != NULL)
		{
			//转换成16进制字符串
			VecAssemblyID.push_back(ConvertTo16(atoi(pAssemblyID)));
			
//			VecAssemblyID.push_back(atoi(pAssemblyID));
			pAssemblyID = strtok(NULL,";");
		}

		//插孔数量
		char *pSlotNum = strtok(SlotNum,";");
		while(pSlotNum != NULL)
		{
			//转换成16进制字符串
			VecSlotNum.push_back(ConvertTo16(atoi(pSlotNum)));
			
//			VecSlotNum.push_back(atoi(pSlotNum));
			pSlotNum = strtok(NULL,";");
		}

		//插孔1
		char *pEquipSlotID1 = strtok(EquipSlotID1,";");
		while(pEquipSlotID1 != NULL)
		{
			//转换成16进制字符串
			VecEquipSlotID1.push_back(ConvertTo16(atoi(pEquipSlotID1)));
			
//			VecEquipSlotID1.push_back(atoi(pEquipSlotID1));
			pEquipSlotID1 = strtok(NULL,";");
		}

		//插孔2
		char *pEquipSlotID2 = strtok(EquipSlotID2,";");
		while(pEquipSlotID2 != NULL)
		{
			//转换成16进制字符串
			VecEquipSlotID2.push_back(ConvertTo16(atoi(pEquipSlotID2)));
			
//			VecEquipSlotID2.push_back(atoi(pEquipSlotID2));
			pEquipSlotID2 = strtok(NULL,";");
		}

		//插孔3
		char *pEquipSlotID3 = strtok(EquipSlotID3,";");
		while(pEquipSlotID3 != NULL)
		{
			//转换成16进制字符串
			VecEquipSlotID3.push_back(ConvertTo16(atoi(pEquipSlotID3)));
			
//			VecEquipSlotID3.push_back(atoi(pEquipSlotID3));
			pEquipSlotID3 = strtok(NULL,";");
		}

		//资质
		char *pQualityEquip = strtok(QualityEquip,";");
		while(pQualityEquip != NULL)
		{
			//转换成16进制字符串
			VecQualityEquip.push_back(ConvertTo16(atoi(pQualityEquip)));
			
//			VecQualityEquip.push_back(atoi(pQualityEquip));
			pQualityEquip = strtok(NULL,";");
		}

		//绑定
		char *pIsBind = strtok(IsBind,";");
		while(pIsBind != NULL)
		{
			//转换成16进制字符串
			VecIsBind.push_back(ConvertTo16(atoi(pIsBind)));

//			VecIsBind.push_back(atoi(pIsBind));
			
			pIsBind = strtok(NULL,";");
		}

		//道具名称
// 		char *pItemName = strtok(ItemName,";");
// 		while(pItemName != NULL)
// 		{
// 			VecItemName.push_back(atoi(pItemName));
// 			
// 			pItemName = strtok(NULL,";");
// 		}

		//构造结构体
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

//转换成16进制字符串
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
// 		sprintf(retMessage, "道具添加:<大区>%s, <角色名>%s, <道具ID>%d, <道具数目>%d", ServerIP, Character, ItemID, ItemNum);
// 		logFile.WriteLog("<侠盗金刚>",operName,"道具添加");//记LOG
// 		
// 		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=add_item(%s,%d,%d)", 
// 			ServerIP, HTTPPORT,
// 			Character,ItemID,ItemNum);
// 		
// 		if(m_operXDJG.getWebInfo(strConn, strResult))
// 		{
// 			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
// 			{
// 				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "XDJG道具添加");				
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
// 							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "添加道具成功", "XDJG道具添加");
// 							
// 							break;
// 						case -2:
// 							wait_num++;
// 							if (wait_num > Maxnum_wait)
// 							{
// 								bFinishFlag = true; 
// 								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "添加道具超时", "XDJG道具添加");
// 								
// 							}
// 							Sleep(1000);
// 							break;
// 						default:
// 							bFinishFlag = true;
// 							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "添加道具失败", "XDJG道具添加");
// 						}
// 					}
// 				}
// 				else
// 				{
// 					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "操作失败", "XDJG道具添加");
// 				}
// 				
// 			}
// 			
// 		}
// 		else
// 		{
// 			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "XDJG道具添加");		
// 		}
// 	}
// 	catch (...)
// 	{
// 		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "XDJG道具添加");	
// 	}
// 	
// 	m_operXDJG.GmOperLog(userByID, ServerIP, "XDJG道具添加", retMessage, DBTFLV[0]);
// 	return DBTFLV;
// }

//道具删除
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
		sprintf(retMessage, "道具添加:<大区>%s, <角色名>%s, <道具ID>%d, <道具数目>%d", ServerIP, Character, ItemID, ItemNum);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=delete_item(%s,%d,%d)", 
			ServerIP, HTTPPORT,
			Character,ItemID,ItemNum);
		
		if(m_operXDJG.getWebInfo(strConn, strResult))
		{
			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "XDJG道具删除");				
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
							
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "删除道具成功", "XDJG道具删除");
							
							break;
						case -2:
							wait_num++;
							if (wait_num > Maxnum_wait)
							{
								bFinishFlag = true; 
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "删除道具超时", "XDJG道具删除");
								
							}
							Sleep(1000);
							break;
						default:
							bFinishFlag = true;
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "删除道具失败", "XDJG道具删除");
						}
					}
				}
				else
				{
					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "操作失败", "XDJG道具删除");
				}
				
			}
			
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "XDJG道具删除");		
		}
	}
	catch (...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "XDJG道具删除");	
	}
	
	m_operXDJG.GmOperLog(userByID, ServerIP, "XDJG道具删除", retMessage, DBTFLV[0]);
	return DBTFLV;
}

//游戏币查询
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
		sprintf(retMessage, "游戏币查询:<大区>%s, <帐号>%s",ServerIP, Account);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=query_gold(%s)", 
			ServerIP, HTTPPORT,
			Account);
		
		if(m_operXDJG.getWebInfo(strConn, strResult))
		{
			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "XDJG游戏币查询");				
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
									DBTFLV = m_operXDJG.ReturnOpMsg(operName, "游戏币查询超时", "XDJG游戏币查询");
								}
								Sleep(1000);
								break;
							default:
								bFinishFlag = true;
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "游戏币查询失败", "XDJG游戏币查询");
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
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "游戏币查询失败", "XDJG游戏币查询");
							}
						}
					}
				}
				else
				{
					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "操作失败", "XDJG游戏币查询");
				}
			}
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "XDJG游戏币查询");		
		}
	}
	catch (...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "XDJG游戏币查询");	
	}
	
	return DBTFLV;
}
//添加游戏币
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
		sprintf(retMessage, "添加游戏币:<大区>%s, <帐号>%s, <游戏币数>%d", ServerIP, Account, iMoney);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=add_gold(%s,%d)", 
			ServerIP, HTTPPORT,
			Account,iMoney);
		
		if(m_operXDJG.getWebInfo(strConn, strResult))
		{
			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "XDJG游戏币添加");				
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
							
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "添加游戏币成功", "XDJG游戏币添加");
							
							break;
						case -2:
							wait_num++;
							if (wait_num > Maxnum_wait)
							{
								bFinishFlag = true; 
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "添加游戏币超时", "XDJG游戏币添加");
								
							}
							Sleep(1000);
							break;
						default:
							bFinishFlag = true;
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "添加游戏币失败", "XDJG游戏币添加");
						}
					}
				}
				else
				{
					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "操作失败", "XDJG游戏币添加");
				}
				
			}
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "XDJG游戏币添加");		
		}
	}
	catch (...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "XDJG游戏币添加");	
	}
	
	m_operXDJG.GmOperLog(userByID, ServerIP, "XDJG游戏币添加", retMessage, DBTFLV[0]);
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

		//取开始时间的年月日
		pBegin = strtok(BeginTime, cTag);
		while(pBegin != NULL)
		{
			if(strlen(pBegin) > 4)
				break;
			strBeData += pBegin;
			pBegin = strtok(NULL, cTag);
		}

		//取结束时间的年月日
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

//日志类:玩家登录日志
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
		sprintf(retMessage, "玩家登录日志:<大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character,BeginTime, EndTime);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "玩家登录日志");
			return DBTFLV;//无法获取真正的服务器IP
		}
		
		//获取LogDB的名称
// 		LogDbName = GetLogDbName(BeginTime, EndTime);
// 		if(LogDbName.size() == 0)
// 			return DBTFLV;

		if(!strcmp(QueryIP,""))
		{
			strSql.Format("SELECT * FROM (SELECT account_name AS XDJG_Account,'%s' AS XDJG_Character,'登录' AS XDJG_LogType,create_time AS XDJG_LogTime,ipaddr AS XDJG_LogIP \
											FROM %s.ACCOUNT_LOGIN \
										   WHERE account_name = '%s' and create_time between '%s' and '%s' \
										   UNION \
										  SELECT b.account_name AS XDJG_Account,'%s' AS XDJG_Character,'登出' AS XDJG_LogType,b.create_time AS XDJG_LogTime,c.ipaddr AS XDJG_LogIP \
											FROM %s.ACCOUNT_LOGOUT b, %s.ACCOUNT_LOGIN c \
										   WHERE b.account_name = '%s' and b.account_name = c.account_name \
											 and b.create_time between '%s' and '%s') a \
							ORDER BY a.XDJG_LogTime",Character,LogDB,Account,BeginTime,EndTime,Character,LogDB,LogDB,Account,BeginTime,EndTime);
		}
		else
		{
			strSql.Format("SELECT * FROM (SELECT account_name AS XDJG_Account,'登录' AS XDJG_LogType,create_time AS XDJG_LogTime,'%s' AS XDJG_LogIP \
										    FROM %s.ACCOUNT_LOGIN \
										   WHERE ipaddr = '%s' and create_time between '%s' and '%s' \
										   UNION \
										  SELECT b.account_name AS XDJG_Account,'登出' AS XDJG_LogType,b.create_time AS XDJG_LogTime,'%s' AS XDJG_LogIP \
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "玩家登录日志查询");//将错误信息写入日志中并返回错误信息
	}
	
	return DBTFLV;
}

//商城物品购买日志--有问题
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
		sprintf(retMessage, "商城物品购买日志查询:<大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character,BeginTime, EndTime);
		logFile.WriteLog("<侠盗金刚>",operName,"retMessage");//记LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "商城物品购买日志");
			return DBTFLV;//无法获取真正的服务器IP
		}

		//获取LogDB的名称
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

		//取物品名称
		GetItemInfo(VecItemInfo);

		for(pDBTFLV = DBTFLV.begin(); pDBTFLV != DBTFLV.end(); pDBTFLV++)
		{
			//将道具id变成道具名称
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
			//将道具类型变成名称
			else if(pDBTFLV->m_tagName == CEnumCore::TagName::XDJG_ItemType)
			{
				switch (atoi((char*)pDBTFLV->lpdata))
				{
				case 0:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("无特定归属")+1;
					memcpy(&pDBTFLV->lpdata, "无特定归属", strlen("无特定归属")+1);
					break;
				case 1:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("装备")+1;
					memcpy(&pDBTFLV->lpdata, "装备", strlen("装备")+1);
					break;
				case 2:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("消耗品")+1;
					memcpy(&pDBTFLV->lpdata, "消耗品", strlen("消耗品")+1);
					break;
				case 3:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("强化道具")+1;
					memcpy(&pDBTFLV->lpdata, "强化道具", strlen("强化道具")+1);
					break;
				case 4:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("组装道具")+1;
					memcpy(&pDBTFLV->lpdata, "组装道具", strlen("组装道具")+1);
					break;
				case 5:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("任务道具")+1;
					memcpy(&pDBTFLV->lpdata, "任务道具", strlen("任务道具")+1);
					break;
				case 6:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("生活技能配方")+1;
					memcpy(&pDBTFLV->lpdata, "生活技能配方", strlen("生活技能配方")+1);
					break;
				case 7:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("战斗技能书")+1;
					memcpy(&pDBTFLV->lpdata, "战斗技能书", strlen("战斗技能书")+1);
					break;
				case 8:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("容器")+1;
					memcpy(&pDBTFLV->lpdata, "容器", strlen("容器")+1);
					break;
				case 9:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("宝箱")+1;
					memcpy(&pDBTFLV->lpdata, "宝箱", strlen("宝箱")+1);
					break;
				case 10:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("书籍")+1;
					memcpy(&pDBTFLV->lpdata, "书籍", strlen("书籍")+1);
					break;
				case 11:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("车辆")+1;
					memcpy(&pDBTFLV->lpdata, "车辆", strlen("车辆")+1);
					break;
				case 12:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("车辆装备道具")+1;
					memcpy(&pDBTFLV->lpdata, "车辆装备道具", strlen("车辆装备道具")+1);
					break;
				case 13:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("激活任务道具")+1;
					memcpy(&pDBTFLV->lpdata, "激活任务道具", strlen("激活任务道具")+1);
					break;
				case 14:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("宠物")+1;
					memcpy(&pDBTFLV->lpdata, "宠物", strlen("宠物")+1);
					break;
				case 15:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("充能道具")+1;
					memcpy(&pDBTFLV->lpdata, "充能道具", strlen("充能道具")+1);
					break;
				case 16:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("提炼道具")+1;
					memcpy(&pDBTFLV->lpdata, "提炼道具", strlen("提炼道具")+1);
					break;
				case 17:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("开槽道具")+1;
					memcpy(&pDBTFLV->lpdata, "开槽道具", strlen("开槽道具")+1);
					break;
				case 18:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("镶嵌宝石/插槽道具")+1;
					memcpy(&pDBTFLV->lpdata, "镶嵌宝石/插槽道具", strlen("镶嵌宝石/插槽道具")+1);
					break;
				case 19:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("催化剂")+1;
					memcpy(&pDBTFLV->lpdata, "催化剂", strlen("催化剂")+1);
					break;
				case 20:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("合并道具")+1;
					memcpy(&pDBTFLV->lpdata, "合并道具", strlen("合并道具")+1);
					break;
				case 21:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("合成道具")+1;
					memcpy(&pDBTFLV->lpdata, "合成道具", strlen("合成道具")+1);
					break;
				case 22:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("置换道具")+1;
					memcpy(&pDBTFLV->lpdata, "置换道具", strlen("置换道具")+1);
					break;
				case 23:
					*pDBTFLV->lpdata = NULL;
					memcpy(&pDBTFLV->lpdata, "生活技能工具", strlen("生活技能工具")+1);
					break;
				case 24:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("停车券")+1;
					memcpy(&pDBTFLV->lpdata, "停车券", strlen("停车券")+1);
					break;
				case 25:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("车辆装备置换道具")+1;
					memcpy(&pDBTFLV->lpdata, "车辆装备置换道具", strlen("车辆装备置换道具")+1);
					break;
				case 26:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("聊天频道道具")+1;
					memcpy(&pDBTFLV->lpdata, "聊天频道道具", strlen("聊天频道道具")+1);
					break;
				case 27:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("药水")+1;
					memcpy(&pDBTFLV->lpdata, "药水", strlen("药水")+1);
					break;
				case 28:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("药剂")+1;
					memcpy(&pDBTFLV->lpdata, "药剂", strlen("药剂")+1);
					break;
				case 29:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("食物")+1;
					memcpy(&pDBTFLV->lpdata, "食物", strlen("食物")+1);
					break;
				case 30:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("饮料")+1;
					memcpy(&pDBTFLV->lpdata, "饮料", strlen("饮料")+1);
					break;
				case 31:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("炸弹")+1;
					memcpy(&pDBTFLV->lpdata, "炸弹", strlen("炸弹")+1);
					break;
				case 32:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("CP补给品")+1;
					memcpy(&pDBTFLV->lpdata, "CP补给品", strlen("CP补给品")+1);
					break;
				case 33:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("车辆升级道具")+1;
					memcpy(&pDBTFLV->lpdata, "车辆升级道具", strlen("车辆升级道具")+1);
					break;
				case 34:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("DNA记忆体")+1;
					memcpy(&pDBTFLV->lpdata, "DNA记忆体", strlen("DNA记忆体")+1);
					break;
				case 35:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("精力补充剂")+1;
					memcpy(&pDBTFLV->lpdata, "精力补充剂", strlen("精力补充剂")+1);
					break;
				case 36:
					*pDBTFLV->lpdata = NULL;
					pDBTFLV->m_tvlength = strlen("装备增幅回退道具")+1;
					memcpy(&pDBTFLV->lpdata, "装备增幅回退道具", strlen("装备增幅回退道具")+1);
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "商城物品购买日志查询");//将错误信息写入日志中并返回错误信息
	}
	
	return DBTFLV;
}

//玩家物品日志:交易获得
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
		sprintf(retMessage, "玩家物品日志:交易获得:<大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character,BeginTime, EndTime);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG

		//获取LogDB的ip
		if(!m_operXDJG.GetServerIP(logDBServerIP,ServerIP,2))
		{
			return DBTFLV;//无法获取LogDB的IP
		}

		//获取LogDB的名称
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

		//取物品名称
		GetItemInfo(VecItemInfo);

		//将道具id变成道具名称
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
// 				pDBTFLV->m_tvlength = strlen("未知")+1;
// 				memcpy(&pDBTFLV->lpdata, "未知", strlen("未知")+1);

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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "玩家物品日志:交易获得");//将错误信息写入日志中并返回错误信息
	}

	return DBTFLV;
}

//玩家物品日志:交易失去
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
		
		sprintf(retMessage, "玩家物品日志:交易失去:<大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character,BeginTime, EndTime);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		//获取GameDB的ip
// 		if(!m_operXDJG.GetServerIP(gameDBServerIP,ServerIP,3))
// 		{
// 			return DBTFLV;//无法获取GameDB的IP
// 		}
// 		
// 		//取到二进制物品属性字段
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
// 				memcpy(&m_StructInfo.m_sResCont , m_DataRecord.recorddata, sizeof(m_StructInfo.m_sResCont));//物品属性
// 				break;
// 			}
// 			else
// 				continue;
// 		}
// 		
// 		//清空buff
// 		strSql = "";

		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "玩家物品日志:交易失去");
			return DBTFLV;//无法获取真正的服务器IP
		}
		
		//获取LogDB的名称
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
		
		//取物品名称
		GetItemInfo(VecItemInfo);

		//将道具id变成道具名称
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
// 				pDBTFLV->m_tvlength = strlen("未知")+1;
// 				memcpy(&pDBTFLV->lpdata, "未知", strlen("未知")+1);
				
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "玩家物品日志:交易失去");//将错误信息写入日志中并返回错误信息
	}
	
	return DBTFLV;
}

//玩家物品日志:从NPC出购买
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
		
		sprintf(retMessage, "玩家物品日志:从NPC出购买:<大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character,BeginTime, EndTime);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		//获取GameDB的ip
// 		if(!m_operXDJG.GetServerIP(gameDBServerIP,ServerIP,3))
// 		{
// 			return DBTFLV;//无法获取GameDB的IP
// 		}
// 		
// 		//取到二进制物品属性字段
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
// 				memcpy(&m_StructInfo.m_sResCont , m_DataRecord.recorddata, sizeof(m_StructInfo.m_sResCont));//物品属性
// 				break;
// 			}
// 			else
// 				continue;
// 		}
// 		
// 		//清空buff
// 		strSql = "";

		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "玩家物品日志:从NPC出购买");
			return DBTFLV;//无法获取真正的服务器IP
		}
		
		//获取LogDB的名称
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
// 								     CASE bind_money_lose WHEN 0 THEN '非绑定币' \
// 													             ELSE '绑定币' END AS XDJG_MoneyType \
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
							  CASE bind_money_lose WHEN 0 THEN '非绑定币' \
							  ELSE '绑定币' END AS XDJG_MoneyType \
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
		
		//取物品名称
		GetItemInfo(VecItemInfo);

		//将道具id变成道具名称
		for(pDBTFLV = DBTFLV.begin(); pDBTFLV != DBTFLV.end(); pDBTFLV++)
		{
			if(pDBTFLV->m_tagName == CEnumCore::TagName::XDJG_TempItemID)
			{
				//For test
// 				pDBTFLV->m_tagName = CEnumCore::TagName::XDJG_ItemName;
// 				pDBTFLV->m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
// 				*pDBTFLV->lpdata = NULL;
// 				pDBTFLV->m_tvlength = strlen("未知")+1;
// 				memcpy(&pDBTFLV->lpdata, "未知", strlen("未知")+1);
				
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "玩家物品日志:从NPC出购买");//将错误信息写入日志中并返回错误信息
	}
	
	return DBTFLV;
}

//玩家物品日志:卖给NPC
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
		
		sprintf(retMessage, "玩家物品日志:卖给NPC:<大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character,BeginTime, EndTime);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		//获取GameDB的ip
// 		if(!m_operXDJG.GetServerIP(gameDBServerIP,ServerIP,3))
// 		{
// 			return DBTFLV;//无法获取GameDB的IP
// 		}
// 		
// 		//取到二进制物品属性字段
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
// 				memcpy(&m_StructInfo.m_sResCont , m_DataRecord.recorddata, sizeof(m_StructInfo.m_sResCont));//物品属性
// 				break;
// 			}
// 			else
// 				continue;
// 		}
// 		
// 		//清空buff
// 		strSql = "";

		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "玩家物品日志:卖给NPC");
			return DBTFLV;//无法获取真正的服务器IP
		}

		//获取LogDB的名称
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
		
		//取物品名称
		GetItemInfo(VecItemInfo);

		//将道具id变成道具名称
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "玩家物品日志:卖给NPC");//将错误信息写入日志中并返回错误信息
	}
	
	return DBTFLV;
}

//玩家物品日志:拾取物品
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
		
		sprintf(retMessage, "玩家物品日志:拾取物品:<大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character,BeginTime, EndTime);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		//获取GameDB的ip
// 		if(!m_operXDJG.GetServerIP(gameDBServerIP,ServerIP,3))
// 		{
// 			return DBTFLV;//无法获取GameDB的IP
// 		}
// 		
// 		//取到二进制物品属性字段
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
// 				memcpy(&m_StructInfo.m_sResCont , m_DataRecord.recorddata, sizeof(m_StructInfo.m_sResCont));//物品属性
// 				break;
// 			}
// 			else
// 				continue;
// 		}
// 		
// 		//清空buff
// 		strSql = "";

		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "玩家物品日志:拾取物品");
			return DBTFLV;//无法获取真正的服务器IP
		}
		
		//获取LogDB的名称
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
// 									 CASE bind_money_lose WHEN 0 THEN '非绑定币' \
// 									 ELSE '绑定币' END AS XDJG_MoneyType \
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "玩家物品日志:拾取物品");//将错误信息写入日志中并返回错误信息
	}
	
	return DBTFLV;
}

//玩家物品日志:丢弃物品
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
		
		sprintf(retMessage, "玩家物品日志:丢弃物品:<大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character,BeginTime, EndTime);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		//获取GameDB的ip
		if(!m_operXDJG.GetServerIP(gameDBServerIP,ServerIP,3))
		{
			return DBTFLV;//无法获取GameDB的IP
		}
		
		//取到二进制物品属性字段
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
// 				memcpy(&m_StructInfo.m_sResCont , m_DataRecord.recorddata, sizeof(m_StructInfo.m_sResCont));//物品属性
// 				break;
// 			}
// 			else
// 				continue;
// 		}
// 		
// 		//清空buff
// 		strSql = "";

		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "玩家物品日志:丢弃物品");
			return DBTFLV;//无法获取真正的服务器IP
		}
		
		//获取LogDB的名称
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "玩家物品日志:丢弃物品");//将错误信息写入日志中并返回错误信息
	}
	
	return DBTFLV;
}

//玩家物品日志:物品消耗--有问题
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
		sprintf(retMessage, "玩家物品日志:物品消耗:<大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character,BeginTime, EndTime);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			return DBTFLV;//无法获取真正的IP
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
		
		//取物品名称
		GetItemInfo(VecItemInfo);

		//将道具id变成道具名称
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "玩家物品日志:物品消耗");//将错误信息写入日志中并返回错误信息
	}
	
	return DBTFLV;
}

//玩家物品日志:拍卖行卖出
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
		
		sprintf(retMessage, "玩家物品日志:拍卖行卖出:<大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character,BeginTime, EndTime);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		//获取GameDB的ip
// 		if(!m_operXDJG.GetServerIP(gameDBServerIP,ServerIP,3))
// 		{
// 			return DBTFLV;//无法获取GameDB的IP
// 		}
// 		
// 		//取到二进制物品属性字段
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
// 				memcpy(&m_StructInfo.m_sResCont , m_DataRecord.recorddata, sizeof(m_StructInfo.m_sResCont));//物品属性
// 				break;
// 			}
// 			else
// 				continue;
// 		}
// 		
// 		//清空buff
// 		strSql = "";

		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "玩家物品日志:拍卖行卖出");
			return DBTFLV;//无法获取真正的服务器IP
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
		
		//取物品名称
		GetItemInfo(VecItemInfo);

		//将道具id变成道具名称
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "玩家物品日志:拍卖行卖出");//将错误信息写入日志中并返回错误信息
	}
	
	return DBTFLV;
}

//玩家物品日志:拍卖行购入
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
		
		sprintf(retMessage, "玩家物品日志:拍卖行购入:<大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character,BeginTime, EndTime);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		//获取GameDB的ip
// 		if(!m_operXDJG.GetServerIP(gameDBServerIP,ServerIP,3))
// 		{
// 			return DBTFLV;//无法获取GameDB的IP
// 		}
// 		
// 		//取到二进制物品属性字段
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
// 				memcpy(&m_StructInfo.m_sResCont , m_DataRecord.recorddata, sizeof(m_StructInfo.m_sResCont));//物品属性
// 				break;
// 			}
// 			else
// 				continue;
// 		}
// 		
// 		//清空buff
// 		strSql = "";

		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "玩家物品日志:拍卖行购入");
			return DBTFLV;//无法获取真正的服务器IP
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
		
		//取物品名称
		GetItemInfo(VecItemInfo);

		//将道具id变成道具名称
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "玩家物品日志:拍卖行购入");//将错误信息写入日志中并返回错误信息
	}
	
	return DBTFLV;
}

//玩家邮件日志:发送邮件
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
		
		sprintf(retMessage, "玩家邮件日志:发送邮件:<大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character,BeginTime, EndTime);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		//获取GameDB的ip
// 		if(!m_operXDJG.GetServerIP(gameDBServerIP,ServerIP,3))
// 		{
// 			return DBTFLV;//无法获取GameDB的IP
// 		}
// 		
// 		//取到二进制物品属性字段
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
// 				memcpy(&m_StructInfo.m_sResCont , m_DataRecord.recorddata, sizeof(m_StructInfo.m_sResCont));//物品属性
// 				break;
// 			}
// 			else
// 				continue;
// 		}
// 		
// 		//清空buff
// 		strSql = "";

		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "玩家邮件日志:发送邮件");
			return DBTFLV;//无法获取真正的服务器IP
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "玩家邮件日志:发送邮件");//将错误信息写入日志中并返回错误信息
	}
	
	return DBTFLV;
}

//玩家邮件日志:收取邮件
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
		
		sprintf(retMessage, "玩家邮件日志:收取邮件:<大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character,BeginTime, EndTime);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		//获取GameDB的ip
// 		if(!m_operXDJG.GetServerIP(gameDBServerIP,ServerIP,3))
// 		{
// 			return DBTFLV;//无法获取GameDB的IP
// 		}
// 		
// 		//取到二进制物品属性字段
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
// 				memcpy(&m_StructInfo.m_sResCont , m_DataRecord.recorddata, sizeof(m_StructInfo.m_sResCont));//物品属性
// 				break;
// 			}
// 			else
// 				continue;
// 		}
// 		
// 		//清空buff
// 		strSql = "";
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "玩家邮件日志:收取邮件");
			return DBTFLV;//无法获取真正的服务器IP
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "玩家邮件日志:收取邮件");//将错误信息写入日志中并返回错误信息
	}
	
	return DBTFLV;
}

//M币使用日志:背包扩充续费记录
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_DB_ExpansionPack_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	char realServerIP[40];
	
	try
	{
		logFile.WriteLog("<侠盗金刚>",operName,"M币使用日志:背包扩充续费记录");//记LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "公会基本信息查询");
			return DBTFLV;//无法获取真正的服务器IP
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "M币使用日志:背包扩充续费记录");//将错误信息写入日志中并返回错误信息
	}
	
	return DBTFLV;
}

//M币使用日志:传送日志
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_DB_Deliver_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	char realServerIP[40];
	
	try
	{
		logFile.WriteLog("<侠盗金刚>",operName,"M币使用日志:传送日志");//记LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "公会基本信息查询");
			return DBTFLV;//无法获取真正的服务器IP
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "M币使用日志:传送日志");//将错误信息写入日志中并返回错误信息
	}
	
	return DBTFLV;
}

//M币使用日志:角色仓库扩充续费记录
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_DB_ExpansionWarehouse_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	char realServerIP[40];
	
	try
	{
		logFile.WriteLog("<侠盗金刚>",operName,"M币使用日志:角色仓库扩充续费记录");//记LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "公会基本信息查询");
			return DBTFLV;//无法获取真正的服务器IP
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "M币使用日志:角色仓库扩充续费记录");//将错误信息写入日志中并返回错误信息
	}
	
	return DBTFLV;
}

//玩家角色日志:角色升级
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_DB_RoleUpgrade_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	char realServerIP[40];
	char retMessage[255];
	ZeroMemory(retMessage,255);
	
	try
	{
		sprintf(retMessage, "玩家角色日志:角色升级:<大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character,BeginTime, EndTime);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "玩家角色日志:角色升级");
			return DBTFLV;//无法获取真正的服务器IP
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "玩家角色日志:角色升级");//将错误信息写入日志中并返回错误信息
	}
	
	return DBTFLV;
}

//玩家角色日志:角色创建
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_DB_RoleCreate_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	char realServerIP[40];
	char retMessage[255];
	ZeroMemory(retMessage,255);
	
	try
	{
		sprintf(retMessage, "玩家角色日志:角色创建:<大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character,BeginTime, EndTime);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "玩家角色日志:角色创建");
			return DBTFLV;//无法获取真正的服务器IP
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "玩家角色日志:角色创建");//将错误信息写入日志中并返回错误信息
	}
	
	return DBTFLV;	
}

//玩家角色日志:角色技能学习
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
		sprintf(retMessage, "玩家角色日志:角色技能学习:<大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character,BeginTime, EndTime);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "玩家角色日志:角色技能学习");
			return DBTFLV;//无法获取真正的服务器IP
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "玩家角色日志:角色技能学习");//将错误信息写入日志中并返回错误信息
	}
	
	return DBTFLV;
}

//玩家角色日志:领取任务
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
		sprintf(retMessage, "玩家角色日志:领取任务:<大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character,BeginTime, EndTime);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "玩家角色日志:领取任务");
			return DBTFLV;//无法获取真正的服务器IP
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "玩家角色日志:领取任务");//将错误信息写入日志中并返回错误信息
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
		logFile.WriteLog("<侠盗金刚>",operName,"取任务名称");//记LOG
		
		//取任务名称
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

//玩家角色日志:完成任务
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
		sprintf(retMessage, "玩家角色日志:完成任务:<大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character,BeginTime, EndTime);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "玩家角色日志:完成任务");
			return DBTFLV;//无法获取真正的服务器IP
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
		
		//取物品名称
		GetItemInfo(VecItemInfo);

		//将道具id变成道具名称
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "玩家角色日志:完成任务");//将错误信息写入日志中并返回错误信息
	}
	
	return DBTFLV;
}

//玩家角色日志:副本查询
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_DB_Copy_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	char realServerIP[40];
	char retMessage[255];
	ZeroMemory(retMessage,255);
	
	try
	{
		sprintf(retMessage, "玩家角色日志:副本查询:<大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character,BeginTime, EndTime);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "玩家角色日志:副本查询");
			return DBTFLV;//无法获取真正的服务器IP
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "玩家角色日志:角色技能学习");//将错误信息写入日志中并返回错误信息
	}
	
	return DBTFLV;
}

//查询类
//玩家物品信息:身上物品信息
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
		sprintf(retMessage, "玩家角色日志:身上物品信息:<大区>%s, <帐号>%s, <角色名>%s", ServerIP, Account, Character);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,3))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "玩家物品信息:身上物品信息");
			return DBTFLV;//无法获取真正的服务器IP
		}

		//取到二进制物品属性字段
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
				memcpy(&m_StructInfo.m_sResCont , m_DataRecord.recorddata, sizeof(m_StructInfo.m_sResCont));//物品属性
				break;
			}
			else
				continue;
		}

		//取当前身上的道具
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
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "玩家身上没有物品", "身上物品信息查询");
			return DBTFLV;
		}

		//取物品名称
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
			//玩家帐号
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Account;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(Account) + 1;
			sprintf((char *)&m_tflv.lpdata,"%s",Account);
			DBTFLV.push_back(m_tflv);

			//角色名
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Character;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(Character) + 1;
			sprintf((char *)&m_tflv.lpdata,"%s",Character);
			DBTFLV.push_back(m_tflv);

			//物品ID
			szData.Format("%i", Vec_Res[iCount].itemId);
			m_tflv.nIndex = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_ItemID;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//物品名称
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

			//是否绑定
			m_tflv.nIndex = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_IsBind;
			switch(Vec_Res[iCount].property.safeFlags & 0xFF)
			{
			case SAFEFLAG:
				m_tflv.m_tvlength = strlen("是") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "是");
				break;
			default:
				m_tflv.m_tvlength = strlen("否") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "否");
				break;
			}
			DBTFLV.push_back(m_tflv);

			//是否为优质装备
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_QualityEquip;
			if((Vec_Res[iCount].property.itemFlags & ITEMFLAG) == ITEMFLAG)
			{
				if((Vec_Res[iCount].property.itemFlags & SAFEFLAG) == SAFEFLAG)
				{
					m_tflv.m_tvlength = strlen("是") + 1;
					sprintf((char*)&m_tflv.lpdata, "%s", "是");
				}
				else
				{
					m_tflv.m_tvlength = strlen("否") + 1;
					sprintf((char*)&m_tflv.lpdata, "%s", "否");
				}

			}
			else
			{
				m_tflv.m_tvlength = strlen("未鉴定") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "未鉴定");
			}
			DBTFLV.push_back(m_tflv);

			//叠加数量
			szData.Format("%i", Vec_Res[iCount].property.usingTime);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_SuperpositionNum;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//创建时间
			strTime = GetRealTime(Vec_Res[iCount].property.createTime);
//			szData.Format("%i", Vec_Res[iCount].property.createTime);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_CreateTime;
			m_tflv.m_tvlength = strTime.length();
			sprintf((char*)&m_tflv.lpdata, "%s", strTime.c_str());
			DBTFLV.push_back(m_tflv);

			//耐久值
			szData.Format("%i", Vec_Res[iCount].property.abrasable);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_DurableValue;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//增幅类型
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_IncreaseType;
			switch(Vec_Res[iCount].property.strengthenType)
			{
			case 0:
				m_tflv.m_tvlength = strlen("没有") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "没有");
				break;
			case 1:
				m_tflv.m_tvlength = strlen("力量") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "力量");
				break;
			case 2:
				m_tflv.m_tvlength = strlen("敏捷") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "敏捷");
				break;
			case 3:
				m_tflv.m_tvlength = strlen("体力") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "体力");
				break;
			case 4:
				m_tflv.m_tvlength = strlen("灵力") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "灵力");
				break;
			case 5:
				m_tflv.m_tvlength = strlen("异能") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "异能");
				break;
			default:
				m_tflv.m_tvlength = strlen("未知") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "未知");
				break;
			}
			DBTFLV.push_back(m_tflv);

			//最后一次增幅附加值
			szData.Format("%i", Vec_Res[iCount].property.strengthenLastTimeQuantity);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_IncreaseEndValue;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//增幅次数
			szData.Format("%i", Vec_Res[iCount].property.strengthenEffect);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_IncreaseNum;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//装备组装ID
			szData.Format("%i", Vec_Res[iCount].property.compositingEffect);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_AssemblyID;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);
			
			//增幅附加值
			szData.Format("%i", Vec_Res[iCount].property.strengthenQuantity);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_IncreaseValue;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//装备镶嵌宝石的ID1
			szData.Format("%i", Vec_Res[iCount].property.equipSlotEffect1);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_EquipSlotID1;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);
			
			//装备镶嵌宝石的ID2
			szData.Format("%i", Vec_Res[iCount].property.equipSlotEffect2);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_EquipSlotID2;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//装备镶嵌宝石的ID3
			szData.Format("%i", Vec_Res[iCount].property.equipSlotEffect3);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_EquipSlotID3;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);
			
			//增幅为多少星(星级)
			szData.Format("%i", (Vec_Res[iCount].property.equipSlotEffect3 & STARLEVEL));
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_StarsLevel;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//随机属性值
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "玩家物品信息:身上物品信息");//将错误信息写入日志中并返回错误信息
	}
	
	return DBTFLV;
}

//获取真正的时间
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

//玩家物品信息:背包物品信息
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
		sprintf(retMessage, "玩家角色日志:背包物品信息:<大区>%s, <帐号>%s, <角色名>%s", ServerIP, Account, Character);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,3))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "玩家物品信息:背包物品信息");
			return DBTFLV;//无法获取真正的服务器IP
		}

		//取到二进制物品属性字段
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
				memcpy(&m_StructInfo.m_sResCont , m_DataRecord.recorddata, sizeof(m_StructInfo.m_sResCont));//物品属性
				break;
			}
			else
				continue;
		}

		//取当前身上的道具
		for(int i = 0; i < m_StructInfo.m_sResCont.limit_character_res_num; i++)
		{
			if(m_StructInfo.m_sResCont.res_list[i].itemId != 0)
				Vec_Res.push_back(m_StructInfo.m_sResCont.res_list[i]);
			else
				continue;
		}
		
		if(Vec_Res.size() == 0)
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "玩家背包没有物品", "背包物品信息查询");
			return DBTFLV;
		}
		
		//取物品名称
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
			//玩家帐号
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Account;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(Account) + 1;
			sprintf((char *)&m_tflv.lpdata,"%s",Account);
			DBTFLV.push_back(m_tflv);

			//角色名
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Character;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(Character) + 1;
			sprintf((char *)&m_tflv.lpdata,"%s",Character);
			DBTFLV.push_back(m_tflv);

			//物品ID
			szData.Format("%i", Vec_Res[iCount].itemId);
			m_tflv.nIndex = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_ItemID;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//物品名称
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

			//是否绑定
			m_tflv.nIndex = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_IsBind;
			switch(Vec_Res[iCount].property.safeFlags & 0xFF)
			{
			case SAFEFLAG:
				m_tflv.m_tvlength = strlen("是") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "是");
				break;
			default:
				m_tflv.m_tvlength = strlen("否") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "否");
				break;
			}
			DBTFLV.push_back(m_tflv);

			//是否为优质装备
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_QualityEquip;
			if((Vec_Res[iCount].property.itemFlags & ITEMFLAG) == ITEMFLAG)
			{
				if((Vec_Res[iCount].property.itemFlags & SAFEFLAG) == SAFEFLAG)
				{
					m_tflv.m_tvlength = strlen("是") + 1;
					sprintf((char*)&m_tflv.lpdata, "%s", "是");
				}
				else
				{
					m_tflv.m_tvlength = strlen("否") + 1;
					sprintf((char*)&m_tflv.lpdata, "%s", "否");
				}

			}
			else
			{
				m_tflv.m_tvlength = strlen("未鉴定") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "未鉴定");
			}
			DBTFLV.push_back(m_tflv);

			//叠加数量
			szData.Format("%i", Vec_Res[iCount].property.usingTime);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_SuperpositionNum;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//创建时间
			strTime = GetRealTime(Vec_Res[iCount].property.createTime);
//			szData.Format("%i", Vec_Res[iCount].property.createTime);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_CreateTime;
			m_tflv.m_tvlength = strTime.length();
			sprintf((char*)&m_tflv.lpdata, "%s", strTime.c_str());
			DBTFLV.push_back(m_tflv);

			//耐久值
			szData.Format("%i", Vec_Res[iCount].property.abrasable);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_DurableValue;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//增幅类型
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_IncreaseType;
			switch(Vec_Res[iCount].property.strengthenType)
			{
			case 0:
				m_tflv.m_tvlength = strlen("没有") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "没有");
				break;
			case 1:
				m_tflv.m_tvlength = strlen("力量") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "力量");
				break;
			case 2:
				m_tflv.m_tvlength = strlen("敏捷") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "敏捷");
				break;
			case 3:
				m_tflv.m_tvlength = strlen("体力") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "体力");
				break;
			case 4:
				m_tflv.m_tvlength = strlen("灵力") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "灵力");
				break;
			case 5:
				m_tflv.m_tvlength = strlen("异能") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "异能");
				break;
			default:
				m_tflv.m_tvlength = strlen("未知") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "未知");
				break;
			}
			DBTFLV.push_back(m_tflv);

			//最后一次增幅附加值
			szData.Format("%i", Vec_Res[iCount].property.strengthenLastTimeQuantity);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_IncreaseEndValue;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//增幅次数
			szData.Format("%i", Vec_Res[iCount].property.strengthenEffect);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_IncreaseNum;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//装备组装ID
			szData.Format("%i", Vec_Res[iCount].property.compositingEffect);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_AssemblyID;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);
			
			//增幅附加值
			szData.Format("%i", Vec_Res[iCount].property.strengthenQuantity);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_IncreaseValue;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//装备镶嵌宝石的ID1
			szData.Format("%i", Vec_Res[iCount].property.equipSlotEffect1);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_EquipSlotID1;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);
			
			//装备镶嵌宝石的ID2
			szData.Format("%i", Vec_Res[iCount].property.equipSlotEffect2);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_EquipSlotID2;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//装备镶嵌宝石的ID3
			szData.Format("%i", Vec_Res[iCount].property.equipSlotEffect3);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_EquipSlotID3;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);
			
			//增幅为多少星(星级)
			szData.Format("%i", (Vec_Res[iCount].property.equipSlotEffect3 & STARLEVEL));
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_StarsLevel;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//随机属性值
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "玩家物品信息:背包物品信息查询");//将错误信息写入日志中并返回错误信息
	}
	
	return DBTFLV;
}

//玩家物品信息:仓库物品信息
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
		sprintf(retMessage, "玩家角色日志:仓库物品信息查询:<大区>%s, <帐号>%s, <角色名>%s", ServerIP, Account, Character);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,3))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "玩家物品信息:仓库物品信息");
			return DBTFLV;//无法获取真正的服务器IP
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
				memcpy(&m_StructInfo.m_sWareHouse , m_DataRecord.recorddata, sizeof(m_StructInfo.m_sWareHouse));//物品属性
				break;
			}
			else
				continue;
		}

		//取当前身上的道具
		for(int i = 0; i < limit_per_warehouse_res_num; i++)
		{
			if(m_StructInfo.m_sWareHouse.res_list[i].itemId != 0)
				Vec_Res.push_back(m_StructInfo.m_sWareHouse.res_list[i]);
			else
				continue;
		}
		
		if(Vec_Res.size() == 0)
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "玩家仓库没有物品", "仓库物品信息查询");
			return DBTFLV;
		}
		
		//取物品名称
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
			//玩家帐号
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Account;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(Account) + 1;
			sprintf((char *)&m_tflv.lpdata,"%s",Account);
			DBTFLV.push_back(m_tflv);

			//角色名
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Character;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(Character) + 1;
			sprintf((char *)&m_tflv.lpdata,"%s",Character);
			DBTFLV.push_back(m_tflv);

			//物品ID
			szData.Format("%i", Vec_Res[iCount].itemId);
			m_tflv.nIndex = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_ItemID;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//物品名称
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

			//是否绑定
			m_tflv.nIndex = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_IsBind;
			switch(Vec_Res[iCount].property.safeFlags & 0xFF)
			{
			case SAFEFLAG:
				m_tflv.m_tvlength = strlen("是") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "是");
				break;
			default:
				m_tflv.m_tvlength = strlen("否") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "否");
				break;
			}
			DBTFLV.push_back(m_tflv);

			//是否为优质装备
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_QualityEquip;
			if((Vec_Res[iCount].property.itemFlags & ITEMFLAG) == ITEMFLAG)
			{
				if((Vec_Res[iCount].property.itemFlags & SAFEFLAG) == SAFEFLAG)
				{
					m_tflv.m_tvlength = strlen("是") + 1;
					sprintf((char*)&m_tflv.lpdata, "%s", "是");
				}
				else
				{
					m_tflv.m_tvlength = strlen("否") + 1;
					sprintf((char*)&m_tflv.lpdata, "%s", "否");
				}

			}
			else
			{
				m_tflv.m_tvlength = strlen("未鉴定") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "未鉴定");
			}
			DBTFLV.push_back(m_tflv);

			//叠加数量
			szData.Format("%i", Vec_Res[iCount].property.usingTime);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_SuperpositionNum;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//创建时间
			strTime = GetRealTime(Vec_Res[iCount].property.createTime);
//			szData.Format("%i", Vec_Res[iCount].property.createTime);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_CreateTime;
			m_tflv.m_tvlength = strTime.length();
			sprintf((char*)&m_tflv.lpdata, "%s", strTime.c_str());
			DBTFLV.push_back(m_tflv);

			//耐久值
			szData.Format("%i", Vec_Res[iCount].property.abrasable);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_DurableValue;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//增幅类型
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_IncreaseType;
			switch(Vec_Res[iCount].property.strengthenType)
			{
			case 0:
				m_tflv.m_tvlength = strlen("没有") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "没有");
				break;
			case 1:
				m_tflv.m_tvlength = strlen("力量") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "力量");
				break;
			case 2:
				m_tflv.m_tvlength = strlen("敏捷") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "敏捷");
				break;
			case 3:
				m_tflv.m_tvlength = strlen("体力") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "体力");
				break;
			case 4:
				m_tflv.m_tvlength = strlen("灵力") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "灵力");
				break;
			case 5:
				m_tflv.m_tvlength = strlen("异能") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "异能");
				break;
			default:
				m_tflv.m_tvlength = strlen("未知") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "未知");
				break;
			}
			DBTFLV.push_back(m_tflv);

			//最后一次增幅附加值
			szData.Format("%i", Vec_Res[iCount].property.strengthenLastTimeQuantity);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_IncreaseEndValue;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//增幅次数
			szData.Format("%i", Vec_Res[iCount].property.strengthenEffect);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_IncreaseNum;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//装备组装ID
			szData.Format("%i", Vec_Res[iCount].property.compositingEffect);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_AssemblyID;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);
			
			//增幅附加值
			szData.Format("%i", Vec_Res[iCount].property.strengthenQuantity);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_IncreaseValue;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//装备镶嵌宝石的ID1
			szData.Format("%i", Vec_Res[iCount].property.equipSlotEffect1);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_EquipSlotID1;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);
			
			//装备镶嵌宝石的ID2
			szData.Format("%i", Vec_Res[iCount].property.equipSlotEffect2);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_EquipSlotID2;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//装备镶嵌宝石的ID3
			szData.Format("%i", Vec_Res[iCount].property.equipSlotEffect3);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_EquipSlotID3;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);
			
			//增幅为多少星(星级)
			szData.Format("%i", (Vec_Res[iCount].property.equipSlotEffect3 & STARLEVEL));
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_StarsLevel;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//随机属性值
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "玩家物品信息:仓库物品信息查询");//将错误信息写入日志中并返回错误信息
	}
	
	return DBTFLV;
}

//玩家物品信息:公会仓库信息
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
		sprintf(retMessage, "玩家角色日志:公会仓库信息:<大区>%s, <帐号>%s, <角色名>%s", ServerIP, Account, Character);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,3))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "玩家物品信息:公会仓库信息");
			return DBTFLV;//无法获取真正的服务器IP
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
				memcpy(&m_StructInfo.m_sGuildWareHouse, m_DataRecord.recorddata, sizeof(m_StructInfo.m_sGuildWareHouse));//物品属性
				break;
			}
			else
				continue;
		}

		//取当前身上的道具
		for(int i = 0; i < m_StructInfo.m_sGuildWareHouse.limit_guild_ware_house_size_per_level; i++)
		{
			if(m_StructInfo.m_sGuildWareHouse.items.res_list[i].itemId != 0)
				Vec_Res.push_back(m_StructInfo.m_sGuildWareHouse.items.res_list[i]);
			else
				continue;
		}
		
		if(Vec_Res.size() == 0)
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "公会仓库没有物品", "公会仓库信息查询");
			return DBTFLV;
		}
		
		//取物品名称
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
			//玩家帐号
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Account;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(Account) + 1;
			sprintf((char *)&m_tflv.lpdata,"%s",Account);
			DBTFLV.push_back(m_tflv);

			//角色名
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Character;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(Character) + 1;
			sprintf((char *)&m_tflv.lpdata,"%s",Character);
			DBTFLV.push_back(m_tflv);

			//物品ID
			szData.Format("%i", Vec_Res[iCount].itemId);
			m_tflv.nIndex = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_ItemID;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//物品名称
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

			//是否绑定
			m_tflv.nIndex = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_IsBind;
			switch(Vec_Res[iCount].property.safeFlags & 0xFF)
			{
			case SAFEFLAG:
				m_tflv.m_tvlength = strlen("是") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "是");
				break;
			default:
				m_tflv.m_tvlength = strlen("否") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "否");
				break;
			}
			DBTFLV.push_back(m_tflv);

			//是否为优质装备
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_QualityEquip;
			if((Vec_Res[iCount].property.itemFlags & ITEMFLAG) == ITEMFLAG)
			{
				if((Vec_Res[iCount].property.itemFlags & SAFEFLAG) == SAFEFLAG)
				{
					m_tflv.m_tvlength = strlen("是") + 1;
					sprintf((char*)&m_tflv.lpdata, "%s", "是");
				}
				else
				{
					m_tflv.m_tvlength = strlen("否") + 1;
					sprintf((char*)&m_tflv.lpdata, "%s", "否");
				}

			}
			else
			{
				m_tflv.m_tvlength = strlen("未鉴定") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "未鉴定");
			}
			DBTFLV.push_back(m_tflv);

			//叠加数量
			szData.Format("%i", Vec_Res[iCount].property.usingTime);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_SuperpositionNum;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//创建时间
			strTime = GetRealTime(Vec_Res[iCount].property.createTime);
//			szData.Format("%i", Vec_Res[iCount].property.createTime);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_CreateTime;
			m_tflv.m_tvlength = strTime.length();
			sprintf((char*)&m_tflv.lpdata, "%s", strTime.c_str());
			DBTFLV.push_back(m_tflv);

			//耐久值
			szData.Format("%i", Vec_Res[iCount].property.abrasable);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_DurableValue;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//增幅类型
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_IncreaseType;
			switch(Vec_Res[iCount].property.strengthenType)
			{
			case 0:
				m_tflv.m_tvlength = strlen("没有") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "没有");
				break;
			case 1:
				m_tflv.m_tvlength = strlen("力量") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "力量");
				break;
			case 2:
				m_tflv.m_tvlength = strlen("敏捷") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "敏捷");
				break;
			case 3:
				m_tflv.m_tvlength = strlen("体力") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "体力");
				break;
			case 4:
				m_tflv.m_tvlength = strlen("灵力") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "灵力");
				break;
			case 5:
				m_tflv.m_tvlength = strlen("异能") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "异能");
				break;
			default:
				m_tflv.m_tvlength = strlen("未知") + 1;
				sprintf((char*)&m_tflv.lpdata, "%s", "未知");
				break;
			}
			DBTFLV.push_back(m_tflv);

			//最后一次增幅附加值
			szData.Format("%i", Vec_Res[iCount].property.strengthenLastTimeQuantity);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_IncreaseEndValue;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//增幅次数
			szData.Format("%i", Vec_Res[iCount].property.strengthenEffect);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_IncreaseNum;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//装备组装ID
			szData.Format("%i", Vec_Res[iCount].property.compositingEffect);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_AssemblyID;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);
			
			//增幅附加值
			szData.Format("%i", Vec_Res[iCount].property.strengthenQuantity);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_IncreaseValue;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//装备镶嵌宝石的ID1
			szData.Format("%i", Vec_Res[iCount].property.equipSlotEffect1);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_EquipSlotID1;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);
			
			//装备镶嵌宝石的ID2
			szData.Format("%i", Vec_Res[iCount].property.equipSlotEffect2);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_EquipSlotID2;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//装备镶嵌宝石的ID3
			szData.Format("%i", Vec_Res[iCount].property.equipSlotEffect3);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_EquipSlotID3;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);
			
			//增幅为多少星(星级)
			szData.Format("%i", (Vec_Res[iCount].property.equipSlotEffect3 & STARLEVEL));
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_StarsLevel;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//随机属性值
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "玩家物品信息:公会仓库信息");//将错误信息写入日志中并返回错误信息
	}
	
	return DBTFLV;
}

//玩家物品信息:金刚仓库信息
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
		sprintf(retMessage, "玩家角色日志:金刚仓库信息:<大区>%s, <帐号>%s, <角色名>%s", ServerIP, Account, Character);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,3))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "玩家物品信息:金刚仓库信息");
			return DBTFLV;//无法获取真正的服务器IP
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
				memcpy(&m_StructInfo.m_sGarage, m_DataRecord.recorddata, sizeof(m_StructInfo.m_sGarage));//物品属性
				break;
			}
			else
				continue;
		}

		//取当前身上的金刚
		for(int i = 0; i < m_StructInfo.m_sGarage.limit_garage_car_num; i++)
		{
			if(m_StructInfo.m_sGarage.car_list[i].carId != 0)
				Vec_Car.push_back(m_StructInfo.m_sGarage.car_list[i]);
			else
				continue;
		}
		
		if(Vec_Car.size() == 0)
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "金刚仓库没有物品", "金刚仓库信息查询");
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
			//玩家帐号
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Account;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(Account) + 1;
			sprintf((char *)&m_tflv.lpdata,"%s",Account);
			DBTFLV.push_back(m_tflv);

			//角色名
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Character;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(Character) + 1;
			sprintf((char *)&m_tflv.lpdata,"%s",Character);
			DBTFLV.push_back(m_tflv);

			//金刚ID
			strData.Format("%d", Vec_Car[iCount].carId);
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_CarID;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strData.GetLength()+1;
			sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//取金刚名称
			string strCarName = GetCarName(Vec_Car[iCount].carId);
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_CarName;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strCarName.length();
			sprintf((char *)&m_tflv.lpdata,"%s",strCarName.c_str());
			DBTFLV.push_back(m_tflv);

			//创建时间
			strTime = GetRealTime(Vec_Car[iCount].createTime);
//			strData.Format("%u", Vec_Car[iCount].createTime);
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_CreateTime;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strTime.length();
			sprintf((char *)&m_tflv.lpdata,"%s",strTime.c_str());
			DBTFLV.push_back(m_tflv);

			//当前CP值
			strData.Format("%d", Vec_Car[iCount].cpValue);
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_cpValue;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strData.GetLength()+1;
			sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//是否锁定
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_IsLock;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			switch(Vec_Car[iCount].isLocked)
			{ 
			case 0:
				m_tflv.m_tvlength=strlen("否")+1;
				sprintf((char *)&m_tflv.lpdata,"%s","否");
				break;
			case 1:
				m_tflv.m_tvlength=strlen("是")+1;
				sprintf((char *)&m_tflv.lpdata,"%s","是");
				break;
			default:
				break;
			}
 			DBTFLV.push_back(m_tflv);

			//道具是否绑定在第7位上
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_IsBind;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			switch(Vec_Car[iCount].safeFlags & 0xFF)
			{
			case SAFEFLAG:
				m_tflv.m_tvlength=strlen("是")+1;
				sprintf((char *)&m_tflv.lpdata,"%s","是");
				break;
			default:
				m_tflv.m_tvlength=strlen("否")+1;
				sprintf((char *)&m_tflv.lpdata,"%s","否");
				break;
			}
			DBTFLV.push_back(m_tflv);

			//用户锁定解锁次数
			strData.Format("%d", Vec_Car[iCount].userlockUnlockTime);
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_UnlockTime;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strData.GetLength()+1;
			sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//属性ID
			strData.Format("%d", Vec_Car[iCount]._TranFightableID);
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_PropertyId;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strData.GetLength()+1;
			sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//当前经验
			strData.Format("%d", Vec_Car[iCount]._Exp);
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Exp;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strData.GetLength()+1;
			sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//当前金刚技能ID
			strData.Format("%d", Vec_Car[iCount]._TranSkillID);
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_SkillID;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strData.GetLength()+1;
			sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//当前外观ID
			strData.Format("%d", Vec_Car[iCount]._TranLookID);
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_LookID;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strData.GetLength()+1;
			sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//天赋附加值
			strData.Format("%d", Vec_Car[iCount]._TranSkillStrID);
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_SkillStrValue;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strData.GetLength()+1;
			sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//时间芯片最大数量
			strData.Format("%d", Vec_Car[iCount]._TimePlugNum);
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_TimePlugNum;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strData.GetLength()+1;
			sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//等级
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
					//时间芯片IDs1
					strData.Format("%d", Vec_Car[iCount]._TranPluginSlots[iNum]);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_TimePlugSlot1;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=strData.GetLength()+1;
					sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
					DBTFLV.push_back(m_tflv);
					break;
				case 1:
					//时间芯片IDs2
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

			//商标创建时间
			strData.Format("%d", Vec_Car[iCount]._BrandCreateTime);
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_BrandCreateTime;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strData.GetLength()+1;
			sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//商标ID
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "玩家物品信息:金刚仓库信息");//将错误信息写入日志中并返回错误信息
	}
	
	return DBTFLV;
}

//取金刚名称
string CXDJGInfo::GetCarName(int carId)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CString strSql = "";
	char strConn[512];
	string szCarName = "";
	vector<CGlobalStruct::DataRecord> datarecord;
	
	try
	{
		logFile.WriteLog("<侠盗金刚>",operName,"取金刚名称");//记LOG
		
		//取金刚名称
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

//邮件信息
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
		sprintf(retMessage, "邮件信息查询:<大区>%s, <帐号>%s, <角色名>%s", ServerIP, Account, Character);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,3))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "邮件信息");
			return DBTFLV;//无法获取真正的服务器IP
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
		
		//取物品名称
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
				memcpy(&m_StructInfo.m_sMail, iter->recorddata, sizeof(m_StructInfo.m_sMail));//物品属性
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

		//将临时道具id转变为道具名称
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

						//物品名称
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
							strItemname += "未知";
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "邮件信息查询");//将错误信息写入日志中并返回错误信息
	}
	
	return DBTFLV;

}

//拍卖行信息
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
		sprintf(retMessage, "拍卖行信息查询:<大区>%s, <帐号>%s, <角色名>%s", ServerIP, Account, Character);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,3))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "拍卖行信息");
			return DBTFLV;//无法获取真正的服务器IP
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
		
		//取物品名称
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
				memcpy(&sResItems, iter->recorddata, sizeof(sResItems));//物品属性
				if(sResItems.itemId != 0)
				{
					szData.Format("%d", sResItems.itemId);
					m_tflv.nIndex  = DBTFLV.size() + 1;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tagName = CEnumCore::TagName::XDJG_ItemID;
					m_tflv.m_tvlength = szData.GetLength()+1;
					sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
					DBTFLV.push_back(m_tflv);

					//物品名称
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "拍卖行信息查询");//将错误信息写入日志中并返回错误信息
	}

	return DBTFLV;
}

//用户角色信息的修改:修改等级/经验
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
			sprintf(retMessage, "修改等级:<大区>%s, <角色>%s, <修改后等级>%d", ServerIP, Character, Level);
			sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=modify_character(%d,%d,%s)", 
				ServerIP, HTTPPORT,
			UpdateType,Level,Character);
		}
		else if(2 == UpdateType)
		{
			sprintf(retMessage, "修改经验:<大区>%s, <角色>%s, <修改后经验>%d", ServerIP, Character, Exp);
			sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=modify_character(%d,%d,%s)", 
				ServerIP, HTTPPORT,
			UpdateType,Exp,Character);
		}
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		if(m_operXDJG.getWebInfo(strConn, strResult))
		{
			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "修改等级/经验");				
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
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "修改等级/经验成功", "修改等级/经验");
							break;
						case -2:
							wait_num++;
							if (wait_num > Maxnum_wait)
							{
								bFinishFlag = true; 
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "修改等级/经验超时", "修改等级/经验");
							}
							Sleep(1000);
							break;
						default:
							bFinishFlag = true;
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "修改等级/经验失败", "修改等级/经验");
						}
					}
				}
				else
				{
					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "操作失败", "修改等级/经验");
				}
				
			}
			
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "修改等级/经验");		
		}
	}
	catch (...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "修改等级/经验");	
	}
	
	//记日志
	m_operXDJG.GmOperLog(userByID, ServerIP, "修改等级/经验", retMessage, DBTFLV[0]);
	return DBTFLV;

// 	vector<CGlobalStruct::TFLV> DBTFLV;
// 	CString strSql = "";
// 	char realServerIP[40];
// 	
// 	try
// 	{
// 		logFile.WriteLog("<侠盗金刚>",operName,"用户角色信息的修改:修改等级");//记LOG
// 		
// 		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,3))
// 		{
// 			return DBTFLV;//无法获取真正的服务器IP
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
// 			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "修改等级成功", "用户角色信息的修改:修改等级");
// 		else
// 			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "修改等级失败", "用户角色信息的修改:修改等级");
// 	}
// 	catch(...)
// 	{
// 		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "用户角色信息的修改:修改等级");//将错误信息写入日志中并返回错误信息
// 	}
// 	
// 	return DBTFLV;
}

//用户密码管理:修改密码
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
		sprintf(retMessage, "修改密码:<大区>%s, <帐号>%s, <新密码>%s", ServerIP, Account, NewPwd);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG

		string strTempPass = strrev((char*)md5_PassWord(NewPwd).c_str());
		strTempPass += "CT_hei8evkjA*10";

		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=modify_password(%s,%s)", 
			ServerIP, HTTPPORT,
			Account, md5_PassWord((char *)strTempPass.c_str()).c_str());
		
		if(m_operXDJG.getWebInfo(strConn, strResult))
		{
			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "XDJG修改密码");				
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
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "修改密码失败", "XDJG修改密码");
						case -2:
							wait_num++;
							if (wait_num > Maxnum_wait)
							{
								bFinishFlag = true; 
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "修改密码超时", "XDJG修改密码");
							}
							Sleep(1000);
							break;
						default:
							strR = strtok((char*)strResult.c_str(),",");
							if(atoi(strR) == 1)
							{
								bFinishFlag = true;
								strR = strtok(NULL,",");
								//取系统时间:修改密码的时间
								GetLocalTime(&timeDest);	
								szTime.Format("%04d-%02d-%02d %02d:%02d:%02d",timeDest.wYear,timeDest.wMonth,timeDest.wDay,timeDest.wHour,timeDest.wMinute,timeDest.wSecond);
								
								strSql.Format("exec XDJG_SAVE_OLDPASSWORD %d, '%s', '%s', '%s', '%s', '%s', %d",userByID, ServerIP, Account, strR, NewPwd, szTime.GetBuffer(0), 0);

								if(!m_SqlHelper.SqlHelperMain("127.0.0.1",strSql.GetBuffer(0),4))
								{
									DBTFLV = m_operXDJG.ReturnOpMsg(operName, "保存原密码失败", "用户密码管理:修改密码");
									return DBTFLV;
								}
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "修改密码成功", "XDJG修改密码");
								break;
							}
							else
							{
								bFinishFlag = true;
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "修改密码失败", "XDJG修改密码");
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
// 									DBTFLV = m_operXDJG.ReturnOpMsg(operName, "修改密码超时", "XDJG修改密码");
// 								}
// 								Sleep(1000);
// 								break;
// 							default:
// 								bFinishFlag = true;
// 								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "修改密码失败", "XDJG修改密码");
// 							}
// 						}
// 						else
// 						{
// 							if(atoi(strR) == 1)
// 							{
// 								strR = strtok(NULL,",");
// 								//取系统时间:修改密码的时间
// 								GetLocalTime(&timeDest);	
// 								szTime.Format("%04d-%02d-%02d %02d:%02d:%02d",timeDest.wYear,timeDest.wMonth,timeDest.wDay,timeDest.wHour,timeDest.wMinute,timeDest.wSecond);
// 								
// 								strSql.Format("exec XDJG_SAVE_OLDPASSWORD %d, '%s', '%s', '%s', '%s', '%s', %d",userByID, ServerIP, Account, strR, NewPwd, szTime.GetBuffer(0), 0);
// 
// 								if(!m_SqlHelper.SqlHelperMain("127.0.0.1",strSql.GetBuffer(0),4))
// 								{
// 									DBTFLV = m_operXDJG.ReturnOpMsg(operName, "保存原密码失败", "用户密码管理:修改密码");
// 									return DBTFLV;
// 								}
// 								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "修改密码成功", "XDJG修改密码");
// 								break;
// 							}
// 							else
// 							{
// 								bFinishFlag = true;
// 								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "修改密码失败", "XDJG修改密码");
// 							}
// 						}
					}
				}
				else
				{
					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "操作失败", "XDJG修改密码");
				}
				
			}
			
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "XDJG修改密码");		
		}
	}
	catch (...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "XDJG修改密码");	
	}
	
	//记日志
	m_operXDJG.GmOperLog(userByID, ServerIP, "XDJG修改密码", retMessage, DBTFLV[0]);
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
// 		logFile.WriteLog("<侠盗金刚>",operName,"用户密码管理:修改密码");//记LOG
// 		
// 		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
// 		{
// 			return DBTFLV;//无法获取真正的服务器IP
// 		}
// 		
// 		//取到原始密码
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
// 				szOldPwd = m_DataRecord.recorddata;//保存原始密码
// 				break;
// 			}
// 			else
// 				continue;
// 		}
// 
// 		//取系统时间:修改密码的时间
// 		GetLocalTime(&timeDest);	
// 		szTime.Format("%04d-%02d-%02d %02d:%02d:%02d",timeDest.wYear,timeDest.wMonth,timeDest.wDay,timeDest.wHour,timeDest.wMinute,timeDest.wSecond);
// 		
// 		strSql.Format("exec XDJG_SAVE_OLDPASSWORD %d, '%s', '%s', '%s', '%s', '%s', %d",userByID, ServerIP, Account, szOldPwd.c_str(), NewPwd, szTime.GetBuffer(0), 0);
// 
// 		if(!m_SqlHelper.SqlHelperMain("127.0.0.1",strSql.GetBuffer(0),4))
// 		{
// 			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "保存原密码失败", "用户密码管理:修改密码");
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
// 			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "修改密码成功", "用户密码管理:修改密码");
// 		else
// 			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "修改密码失败", "用户密码管理:修改密码");
// 	}
// 	catch(...)
// 	{
// 		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "用户密码管理:修改密码");//将错误信息写入日志中并返回错误信息
// 	}
// 	
// 	return DBTFLV;
}

//用户密码管理:临时密码恢复
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
		//取修改前的密码
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
				szOldPwd = m_DataRecord.recorddata;//获取原密码
				break;
			}
			else
				continue;
		}

		sprintf(retMessage, "临时密码恢复:<大区>%s, <帐号>%s, <原密码>%s", ServerIP, Account, szOldPwd.c_str());
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG

		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=modify_password(%s,%s)", 
			ServerIP, HTTPPORT,
			Account,szOldPwd.c_str());
		
		if(m_operXDJG.getWebInfo(strConn, strResult))
		{
			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "临时密码恢复");				
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
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "修改密码失败", "XDJG修改密码");
						case -2:
							wait_num++;
							if (wait_num > Maxnum_wait)
							{
								bFinishFlag = true; 
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "修改密码超时", "XDJG修改密码");
							}
							Sleep(1000);
							break;
						default:
							bFinishFlag = true;
							strR = strtok((char*)strResult.c_str(),",");
							if(atoi(strR) == 1)
							{
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "恢复密码成功", "临时密码恢复");
							}
							else
							{
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "恢复密码失败", "临时密码恢复");
							}
							break;
						}
					}
				}
				else
				{
					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "操作失败", "临时密码恢复");
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
// 									DBTFLV = m_operXDJG.ReturnOpMsg(operName, "恢复密码超时", "临时密码恢复");
// 								}
// 								Sleep(1000);
// 								break;
// 							default:
// 								bFinishFlag = true;
// 								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "恢复密码失败", "临时密码恢复");
// 							}
// 						}
// 						else
// 						{
// 							if(atoi(strR) == 1)
// 							{
// 								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "恢复密码成功", "临时密码恢复");
// 								break;
// 							}
// 							else
// 							{
// 								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "恢复密码失败", "临时密码恢复");
// 								break;
// 							}
// 						}
//					}
// 				}
// 				else
// 				{
// 					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "操作失败", "临时密码恢复");
// 				}
//				
//			}
			
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "临时密码恢复");		
		}
		
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "用户密码管理:临时密码恢复");//将错误信息写入日志中并返回错误信息
	}
	
	//记日志
	m_operXDJG.GmOperLog(userByID, ServerIP, "XDJG临时密码恢复", retMessage, DBTFLV[0]);
	return DBTFLV;
}

//用户角色管理:角色恢复
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
		sprintf(retMessage, "角色恢复:<大区>%s, <恢复角色>%s", ServerIP, Character);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=manage_character(%d,%s)", 
			ServerIP, HTTPPORT,
			2,Character);
		
		if(m_operXDJG.getWebInfo(strConn, strResult))
		{
			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "角色恢复");				
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
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "角色恢复成功", "角色恢复");
							break;
						case -2:
							wait_num++;
							if (wait_num > Maxnum_wait)
							{
								bFinishFlag = true; 
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "角色恢复超时", "角色恢复");
							}
							Sleep(1000);
							break;
						default:
							bFinishFlag = true;
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "角色恢复失败", "角色恢复");
						}
					}
				}
				else
				{
					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "操作失败", "角色恢复");
				}
				
			}
			
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "角色恢复");		
		}
		
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "角色恢复");//将错误信息写入日志中并返回错误信息
	}
	
	//记日志
	m_operXDJG.GmOperLog(userByID, ServerIP, "XDJG角色恢复", retMessage, DBTFLV[0]);
	return DBTFLV;

// 	vector<CGlobalStruct::TFLV> DBTFLV;
// 	CString strSql = "";
// 	char realServerIP[40];
// 	
// 	try
// 	{
// 		logFile.WriteLog("<侠盗金刚>",operName,"用户角色管理:角色恢复");//记LOG
// 		
// 		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,3))
// 		{
// 			return DBTFLV;//无法获取真正的服务器IP
// 		}
// 		
// 		strSql.Format("update %s.character set Deleted = 0 where name = '%s'", GameDB, Character);
// 		
// #ifdef _DEBUG
// 		printf(strSql, GameDB, GameDB);
// #endif
// 		
// 		if(GameDBOper("XDJG", realServerIP, 1 , strSql.GetBuffer(0), GameDB, GameDB))
// 			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "角色恢复成功", "用户角色管理:角色恢复");
// 		else
// 			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "角色恢复失败", "用户角色管理:角色恢复");
// 	}
// 	catch(...)
// 	{
// 		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "用户角色管理:角色恢复");//将错误信息写入日志中并返回错误信息
// 	}
// 	
// 	return DBTFLV;
}

//用户角色管理:角色删除
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
		sprintf(retMessage, "角色删除:<大区>%s, <删除角色>%s", ServerIP, Character);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=manage_character(%d,%s)", 
			ServerIP, HTTPPORT,
			1,Character);
		
		if(m_operXDJG.getWebInfo(strConn, strResult))
		{
			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "角色删除");				
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
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "角色删除成功", "角色删除");
							break;
						case -2:
							wait_num++;
							if (wait_num > Maxnum_wait)
							{
								bFinishFlag = true; 
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "角色删除超时", "角色删除");
							}
							Sleep(1000);
							break;
						default:
							bFinishFlag = true;
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "角色删除失败", "角色删除");
						}
					}
				}
				else
				{
					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "操作失败", "角色删除");
				}
				
			}
			
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "角色删除");		
		}
		
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "角色删除");//将错误信息写入日志中并返回错误信息
	}
	
	//记日志
	m_operXDJG.GmOperLog(userByID, ServerIP, "XDJG角色删除", retMessage, DBTFLV[0]);
	return DBTFLV;

// 	vector<CGlobalStruct::TFLV> DBTFLV;
// 	CString strSql = "";
// 	char realServerIP[40];
// 	
// 	try
// 	{
// 		logFile.WriteLog("<侠盗金刚>",operName,"用户角色管理:角色删除");//记LOG
// 		
// 		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,3))
// 		{
// 			return DBTFLV;//无法获取真正的服务器IP
// 		}
// 		
// 		strSql.Format("update %s.character set Deleted = 1 where name = '%s'", GameDB, Character);
// 		
// #ifdef _DEBUG
// 		printf(strSql, GameDB, GameDB);
// #endif
// 		
// 		if(GameDBOper("XDJG", realServerIP, 1 , strSql.GetBuffer(0), GameDB, GameDB))
// 			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "角色删除成功", "用户角色管理:角色删除");
// 		else
// 			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "角色删除失败", "用户角色管理:角色删除");
// 	}
// 	catch(...)
// 	{
// 		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "用户角色管理:角色删除");//将错误信息写入日志中并返回错误信息
// 	}
// 	
// 	return DBTFLV;
}

//修改角色身上金钱数量(绑定币)
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
		sprintf(retMessage, "修改角色身上金钱数量(绑定币):<大区>%s, <角色>%s, <修改钱数>%d", ServerIP, Character, Money);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=add_bindmoney(%s,%d)", 
			ServerIP, HTTPPORT,
			Character,Money);
		
		if(m_operXDJG.getWebInfo(strConn, strResult))
		{
			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "修改角色身上金钱数量(绑定币)");				
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
							
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "修改金钱成功(绑定币)", "修改角色身上金钱数量(绑定币)");
							
							break;
						case -2:
							wait_num++;
							if (wait_num > Maxnum_wait)
							{
								bFinishFlag = true; 
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "修改金钱超时(绑定币)", "修改角色身上金钱数量(绑定币)");
								
							}
							Sleep(1000);
							break;
						default:
							bFinishFlag = true;
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "修改金钱失败(绑定币)", "修改角色身上金钱数量(绑定币)");
						}
					}
				}
				else
				{
					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "操作失败", "修改角色身上金钱数量(绑定币)");
				}
				
			}
			
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "修改角色身上金钱数量(绑定币)");		
		}
	}
	catch (...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "修改角色身上金钱数量(绑定币)");	
	}
	
	m_operXDJG.GmOperLog(userByID, ServerIP, "XDJG修改角色身上金钱数量(绑定币)", retMessage, DBTFLV[0]);
	return DBTFLV;
}

//修改角色身上金钱数量(非绑定币)
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
		sprintf(retMessage, "修改角色身上金钱数量(非绑定币):<大区>%s, <角色>%s, <修改钱数>%d", ServerIP, Character, Money);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=add_money(%s,%d)", 
			ServerIP, HTTPPORT,
			Character,Money);
		
		if(m_operXDJG.getWebInfo(strConn, strResult))
		{
			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "修改角色身上金钱数量(非绑定币)");				
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
							
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "修改金钱成功(非绑定币)", "修改角色身上金钱数量(非绑定币)");
							
							break;
						case -2:
							wait_num++;
							if (wait_num > Maxnum_wait)
							{
								bFinishFlag = true; 
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "修改金钱超时(非绑定币)", "修改角色身上金钱数量(非绑定币)");
								
							}
							Sleep(1000);
							break;
						default:
							bFinishFlag = true;
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "修改金钱失败(非绑定币)", "修改角色身上金钱数量(非绑定币)");
						}
					}
				}
				else
				{
					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "操作失败", "修改角色身上金钱数量(非绑定币)");
				}
				
			}
			
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "修改角色身上金钱数量(非绑定币)");		
		}
	}
	catch (...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "修改角色身上金钱数量(非绑定币)");	
	}
	
	m_operXDJG.GmOperLog(userByID, ServerIP, "XDJG修改角色身上金钱数量(非绑定币)", retMessage, DBTFLV[0]);
	return DBTFLV;
}

//添加角色身上金钱数量(绑定币)
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
		sprintf(retMessage, "添加角色身上金钱数量(绑定币):<大区>%s, <角色>%s, <添加钱数>%d", ServerIP, Character, Money);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=add_bindmoney(%s,%d)", 
			ServerIP, HTTPPORT,
			Character,Money);
		
		if(m_operXDJG.getWebInfo(strConn, strResult))
		{
			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "添加角色身上金钱数量(绑定币)");				
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
							
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "添加金钱成功(绑定币)", "添加角色身上金钱数量(绑定币)");
							
							break;
						case -2:
							wait_num++;
							if (wait_num > Maxnum_wait)
							{
								bFinishFlag = true; 
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "添加金钱超时(绑定币)", "添加角色身上金钱数量(绑定币)");
								
							}
							Sleep(1000);
							break;
						default:
							bFinishFlag = true;
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "添加金钱失败(绑定币)", "添加角色身上金钱数量(绑定币)");
						}
					}
				}
				else
				{
					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "操作失败", "添加角色身上金钱数量(绑定币)");
				}
				
			}
			
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "添加角色身上金钱数量(绑定币)");		
		}
	}
	catch (...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "添加角色身上金钱数量(绑定币)");	
	}
	
	m_operXDJG.GmOperLog(userByID, ServerIP, "XDJG添加角色身上金钱数量(绑定币)", retMessage, DBTFLV[0]);
	return DBTFLV;
}

//添加角色身上金钱数量(非绑定币)
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
		sprintf(retMessage, "添加角色身上金钱数量(非绑定币):<大区>%s, <角色>%s, <添加钱数>%d", ServerIP, Character, Money);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=add_money(%s,%d)", 
			ServerIP, HTTPPORT,
			Character,Money);
		
		if(m_operXDJG.getWebInfo(strConn, strResult))
		{
			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "添加角色身上金钱数量(非绑定币)");				
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
							
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "添加金钱成功(非绑定币)", "添加角色身上金钱数量(非绑定币)");
							
							break;
						case -2:
							wait_num++;
							if (wait_num > Maxnum_wait)
							{
								bFinishFlag = true; 
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "添加金钱超时(非绑定币)", "添加角色身上金钱数量(非绑定币)");
								
							}
							Sleep(1000);
							break;
						default:
							bFinishFlag = true;
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "添加金钱失败(非绑定币)", "添加角色身上金钱数量(非绑定币)");
						}
					}
				}
				else
				{
					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "操作失败", "添加角色身上金钱数量(非绑定币)");
				}
			}
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "添加角色身上金钱数量(非绑定币)");		
		}
	}
	catch (...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "添加角色身上金钱数量(非绑定币)");	
	}
	
	m_operXDJG.GmOperLog(userByID, ServerIP, "XDJG添加角色身上金钱数量(非绑定币)", retMessage, DBTFLV[0]);
	return DBTFLV;
}

//暂时没用
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
		//打印SQL
		m_SqlHelper.SqlHelperMain("XDJG",&datarecord,"127.0.0.1", strSql.GetBuffer(0), 4, 0, 0);//获取SqlExpress中的相应sql语句
		
		if(datarecord.empty())//如果记录为空
		{
			return szSql;
		}
		
		//获取查询的记录集的每一项
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;	
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("sql_statement", m_DataRecord.recordfieldname))//获取相应的LogDBIP
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
		sprintf(errMessage,"获取SQL语句内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("XDJG",errMessage);
		return szSql;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"获取SQL语句文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("XDJG",errMessage);
		return szSql;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"获取SQL语句其他异常,错误信息:%s",errMessage);
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
// 					//是否绑定
// 					TempDbTflv.nIndex = New_DbTflv.size() + 1;
// 					TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
// 					TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_IsBind;
// 					switch(sResCont.res_list[i].property.safeFlags & 0xFF)
// 					{
// 					case SAFEFLAG:
// 						TempDbTflv.m_tvlength = strlen("是") + 1;
// 						sprintf((char*)&TempDbTflv.lpdata, "%s", "是");
// 						break;
// 					default:
// 						TempDbTflv.m_tvlength = strlen("否") + 1;
// 						sprintf((char*)&TempDbTflv.lpdata, "%s", "否");
// 						break;
// 					}
// 					New_DbTflv.push_back(TempDbTflv);
// 
// 					//是否为优质装备
// 					TempDbTflv.nIndex  = New_DbTflv.size() + 1;
// 					TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
// 					TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_QualityEquip;
// 					if((sResCont.res_list[i].property.itemFlags & ITEMFLAG) == ITEMFLAG)
// 					{
// 						if((sResCont.res_list[i].property.itemFlags & SAFEFLAG) == SAFEFLAG)
// 						{
// 							TempDbTflv.m_tvlength = strlen("是") + 1;
// 							sprintf((char*)&TempDbTflv.lpdata, "%s", "是");
// 						}
// 						else
// 						{
// 							TempDbTflv.m_tvlength = strlen("否") + 1;
// 							sprintf((char*)&TempDbTflv.lpdata, "%s", "否");
// 						}
// 
// 					}
// 					else
// 					{
// 						TempDbTflv.m_tvlength = strlen("未鉴定") + 1;
// 						sprintf((char*)&TempDbTflv.lpdata, "%s", "未鉴定");
// 					}
// 					New_DbTflv.push_back(TempDbTflv);
// 
// 					//叠加数量
// 					szData.Format("%i", sResCont.res_list[i].property.usingTime);
// 					TempDbTflv.nIndex  = New_DbTflv.size() + 1;
// 					TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
// 					TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_SuperpositionNum;
// 					TempDbTflv.m_tvlength = szData.GetLength()+1;
// 					sprintf((char*)&TempDbTflv.lpdata, "%s", szData.GetBuffer(0));
// 					New_DbTflv.push_back(TempDbTflv);
// 
// 					//创建时间
// 					szData.Format("%i", sResCont.res_list[i].property.createTime);
// 					TempDbTflv.nIndex  = New_DbTflv.size() + 1;
// 					TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
// 					TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_CreateTime;
// 					TempDbTflv.m_tvlength = szData.GetLength()+1;
// 					sprintf((char*)&TempDbTflv.lpdata, "%s", szData.GetBuffer(0));
// 					New_DbTflv.push_back(TempDbTflv);
// 
// 					//耐久值
// 					szData.Format("%i", sResCont.res_list[i].property.abrasable);
// 					TempDbTflv.nIndex  = New_DbTflv.size() + 1;
// 					TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
// 					TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_DurableValue;
// 					TempDbTflv.m_tvlength = szData.GetLength()+1;
// 					sprintf((char*)&TempDbTflv.lpdata, "%s", szData.GetBuffer(0));
// 					New_DbTflv.push_back(TempDbTflv);
// 
// 					//增幅类型
// 					TempDbTflv.nIndex  = New_DbTflv.size() + 1;
// 					TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
// 					TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_IncreaseType;
// 					switch(sResCont.res_list[i].property.strengthenType)
// 					{
// 					case 0:
// 						TempDbTflv.m_tvlength = strlen("没有") + 1;
// 						sprintf((char*)&TempDbTflv.lpdata, "%s", "没有");
// 						break;
// 					case 1:
// 						TempDbTflv.m_tvlength = strlen("力量") + 1;
// 						sprintf((char*)&TempDbTflv.lpdata, "%s", "力量");
// 						break;
// 					case 2:
// 						TempDbTflv.m_tvlength = strlen("敏捷") + 1;
// 						sprintf((char*)&TempDbTflv.lpdata, "%s", "敏捷");
// 						break;
// 					case 3:
// 						TempDbTflv.m_tvlength = strlen("体力") + 1;
// 						sprintf((char*)&TempDbTflv.lpdata, "%s", "体力");
// 						break;
// 					case 4:
// 						TempDbTflv.m_tvlength = strlen("灵力") + 1;
// 						sprintf((char*)&TempDbTflv.lpdata, "%s", "灵力");
// 						break;
// 					case 5:
// 						TempDbTflv.m_tvlength = strlen("异能") + 1;
// 						sprintf((char*)&TempDbTflv.lpdata, "%s", "异能");
// 						break;
// 					default:
// 						TempDbTflv.m_tvlength = strlen("未知") + 1;
// 						sprintf((char*)&TempDbTflv.lpdata, "%s", "未知");
// 						break;
// 					}
// 					New_DbTflv.push_back(TempDbTflv);
// 
// 					//最后一次增幅附加值
// 					szData.Format("%i", sResCont.res_list[i].property.strengthenLastTimeQuantity);
// 					TempDbTflv.nIndex  = New_DbTflv.size() + 1;
// 					TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
// 					TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_IncreaseEndValue;
// 					TempDbTflv.m_tvlength = szData.GetLength()+1;
// 					sprintf((char*)&TempDbTflv.lpdata, "%s", szData.GetBuffer(0));
// 					New_DbTflv.push_back(TempDbTflv);
// 
// 					//增幅次数
// 					szData.Format("%i", sResCont.res_list[i].property.strengthenEffect);
// 					TempDbTflv.nIndex  = New_DbTflv.size() + 1;
// 					TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
// 					TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_IncreaseNum;
// 					TempDbTflv.m_tvlength = szData.GetLength()+1;
// 					sprintf((char*)&TempDbTflv.lpdata, "%s", szData.GetBuffer(0));
// 					New_DbTflv.push_back(TempDbTflv);
// 
// 					//装备组装ID
// 					szData.Format("%i", sResCont.res_list[i].property.compositingEffect);
// 					TempDbTflv.nIndex  = New_DbTflv.size() + 1;
// 					TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
// 					TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_AssemblyID;
// 					TempDbTflv.m_tvlength = szData.GetLength()+1;
// 					sprintf((char*)&TempDbTflv.lpdata, "%s", szData.GetBuffer(0));
// 					New_DbTflv.push_back(TempDbTflv);
// 					
// 					//增幅附加值
// 					szData.Format("%i", sResCont.res_list[i].property.strengthenQuantity);
// 					TempDbTflv.nIndex  = New_DbTflv.size() + 1;
// 					TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
// 					TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_IncreaseValue;
// 					TempDbTflv.m_tvlength = szData.GetLength()+1;
// 					sprintf((char*)&TempDbTflv.lpdata, "%s", szData.GetBuffer(0));
// 					New_DbTflv.push_back(TempDbTflv);
// 
// 					//装备镶嵌宝石的ID1
// 					szData.Format("%i", sResCont.res_list[i].property.equipSlotEffect1);
// 					TempDbTflv.nIndex  = New_DbTflv.size() + 1;
// 					TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
// 					TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_EquipSlotID1;
// 					TempDbTflv.m_tvlength = szData.GetLength()+1;
// 					sprintf((char*)&TempDbTflv.lpdata, "%s", szData.GetBuffer(0));
// 					New_DbTflv.push_back(TempDbTflv);
// 					
// 					//装备镶嵌宝石的ID2
// 					szData.Format("%i", sResCont.res_list[i].property.equipSlotEffect2);
// 					TempDbTflv.nIndex  = New_DbTflv.size() + 1;
// 					TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
// 					TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_EquipSlotID2;
// 					TempDbTflv.m_tvlength = szData.GetLength()+1;
// 					sprintf((char*)&TempDbTflv.lpdata, "%s", szData.GetBuffer(0));
// 					New_DbTflv.push_back(TempDbTflv);
// 
// 					//装备镶嵌宝石的ID3
// 					szData.Format("%i", sResCont.res_list[i].property.equipSlotEffect3);
// 					TempDbTflv.nIndex  = New_DbTflv.size() + 1;
// 					TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
// 					TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_EquipSlotID3;
// 					TempDbTflv.m_tvlength = szData.GetLength()+1;
// 					sprintf((char*)&TempDbTflv.lpdata, "%s", szData.GetBuffer(0));
// 					New_DbTflv.push_back(TempDbTflv);
// 					
// 					//增幅为多少星(星级)
// 					szData.Format("%i", (sResCont.res_list[i].property.equipSlotEffect3 & STARLEVEL));
// 					TempDbTflv.nIndex  = New_DbTflv.size() + 1;
// 					TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
// 					TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_StarsLevel;
// 					TempDbTflv.m_tvlength = szData.GetLength()+1;
// 					sprintf((char*)&TempDbTflv.lpdata, "%s", szData.GetBuffer(0));
// 					New_DbTflv.push_back(TempDbTflv);
// 
// 					//随机属性值
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

//角色技能信息查询
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
		sprintf(retMessage, "角色技能信息查询:<大区>%s, <帐号>%s,<角色>%s", ServerIP, Account, Character);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,3))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "角色技能信息查询");
			return DBTFLV;//无法获取真正的服务器IP
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
		printf("玩家拥有技能个数:%d\n",m_StructInfo.m_sSkillCont.skill_count);
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
			//玩家帐号
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Account;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(Account) + 1;
			sprintf((char *)&m_tflv.lpdata,"%s",Account);
			DBTFLV.push_back(m_tflv);
			
			//角色名
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Character;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(Character) + 1;
			sprintf((char *)&m_tflv.lpdata,"%s",Character);
			DBTFLV.push_back(m_tflv);

			//技能ID
			strData.Format("%d", m_StructInfo.m_sSkillCont.skill_list[iCount]);
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_SkillID;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strData.GetLength()+1;
			sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//技能名称
			if(strData.GetLength() == 5)
			{
				string strTemp = strData.GetBuffer(0);
				string strSkillName = GetSkillName(atoi(strTemp.substr(0,3).c_str()), 1);
				strData.Format("(等级%d)",atoi(strTemp.substr(3,6).c_str()));
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "角色技能信息查询");//将错误信息写入日志中并返回错误信息
	}
	
	return DBTFLV;
}

//获取技能名称
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

//技能列表查询
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_SkillList_Query(char *GameName, char *Sql, char *Profession)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	try
	{
		DBTFLV=GameDBQuery(GameName,"127.0.0.1",4,0,0,Sql,Profession);
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog(GameName,"技能列表查询内存不足");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog(GameName,"技能列表查询文件异常");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog(GameName,"技能列表查询其他异常");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog(GameName,(char *)err.what());
		return DBTFLV;
	}
	return DBTFLV;
}

//道具类型查询
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_ItemType_Query(char *GameName, char *Sql)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	try
	{
		DBTFLV=GameDBQuery(GameName,"127.0.0.1",4,0,0,Sql);
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog(GameName,"道具类型查询内存不足");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog(GameName,"道具类型查询文件异常");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog(GameName,"道具类型查询其他异常");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog(GameName,(char *)err.what());
		return DBTFLV;
	}
	return DBTFLV;
}

//相应类型的道具列表查询
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
		logFile.WriteErrorLog(GameName,"道具列表查询内存不足");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog(GameName,"道具列表查询文件异常");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog(GameName,"道具列表查询其他异常");
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

	//取当前身上的道具id
	for(i = 0; i < sResCont.limit_character_res_num; i++)
	{
		if(sResCont.res_list[i].itemId != 0)
			vec_Itemid.push_back(sResCont.res_list[i].itemId);
		else
			continue;
	}

	//取物品名称
	GetItemInfo(VecItemInfo);

	for(pItemID = VecItem.begin(); pItemID != VecItem.end(); pItemID++)
	{
		int iItemId = atoi(pItemID->c_str());

		pID = vec_Itemid.begin();
		while(pID != vec_Itemid.end())
		{
			if(iItemId == *pID && iItemId != 0 && *pID != 0)
			{
				//物品ID
				szData.Format("%i", iItemId);
				TempDbTflv.nIndex = DBTFLV.size() + 1;
				TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
				TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_ItemID;
				TempDbTflv.m_tvlength = szData.GetLength()+1;
				sprintf((char*)&TempDbTflv.lpdata, "%s", szData.GetBuffer(0));
				DBTFLV.push_back(TempDbTflv);

				//物品名称
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

				//是否绑定
				TempDbTflv.nIndex = DBTFLV.size() + 1;
				TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
				TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_IsBind;
				switch(sResCont.res_list[i].property.safeFlags & 0xFF)
				{
				case SAFEFLAG:
					TempDbTflv.m_tvlength = strlen("是") + 1;
					sprintf((char*)&TempDbTflv.lpdata, "%s", "是");
					break;
				default:
					TempDbTflv.m_tvlength = strlen("否") + 1;
					sprintf((char*)&TempDbTflv.lpdata, "%s", "否");
					break;
				}
				DBTFLV.push_back(TempDbTflv);

				//是否为优质装备
				TempDbTflv.nIndex  = DBTFLV.size() + 1;
				TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
				TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_QualityEquip;
				if((sResCont.res_list[i].property.itemFlags & ITEMFLAG) == ITEMFLAG)
				{
					if((sResCont.res_list[i].property.itemFlags & SAFEFLAG) == SAFEFLAG)
					{
						TempDbTflv.m_tvlength = strlen("是") + 1;
						sprintf((char*)&TempDbTflv.lpdata, "%s", "是");
					}
					else
					{
						TempDbTflv.m_tvlength = strlen("否") + 1;
						sprintf((char*)&TempDbTflv.lpdata, "%s", "否");
					}

				}
				else
				{
					TempDbTflv.m_tvlength = strlen("未鉴定") + 1;
					sprintf((char*)&TempDbTflv.lpdata, "%s", "未鉴定");
				}
				DBTFLV.push_back(TempDbTflv);

				//叠加数量
				szData.Format("%i", sResCont.res_list[i].property.usingTime);
				TempDbTflv.nIndex  = DBTFLV.size() + 1;
				TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
				TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_SuperpositionNum;
				TempDbTflv.m_tvlength = szData.GetLength()+1;
				sprintf((char*)&TempDbTflv.lpdata, "%s", szData.GetBuffer(0));
				DBTFLV.push_back(TempDbTflv);

				//创建时间
				szData.Format("%i", sResCont.res_list[i].property.createTime);
				TempDbTflv.nIndex  = DBTFLV.size() + 1;
				TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
				TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_CreateTime;
				TempDbTflv.m_tvlength = szData.GetLength()+1;
				sprintf((char*)&TempDbTflv.lpdata, "%s", szData.GetBuffer(0));
				DBTFLV.push_back(TempDbTflv);

				//耐久值
				szData.Format("%i", sResCont.res_list[i].property.abrasable);
				TempDbTflv.nIndex  = DBTFLV.size() + 1;
				TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
				TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_DurableValue;
				TempDbTflv.m_tvlength = szData.GetLength()+1;
				sprintf((char*)&TempDbTflv.lpdata, "%s", szData.GetBuffer(0));
				DBTFLV.push_back(TempDbTflv);

				//增幅类型
				TempDbTflv.nIndex  = DBTFLV.size() + 1;
				TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
				TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_IncreaseType;
				switch(sResCont.res_list[i].property.strengthenType)
				{
				case 0:
					TempDbTflv.m_tvlength = strlen("没有") + 1;
					sprintf((char*)&TempDbTflv.lpdata, "%s", "没有");
					break;
				case 1:
					TempDbTflv.m_tvlength = strlen("力量") + 1;
					sprintf((char*)&TempDbTflv.lpdata, "%s", "力量");
					break;
				case 2:
					TempDbTflv.m_tvlength = strlen("敏捷") + 1;
					sprintf((char*)&TempDbTflv.lpdata, "%s", "敏捷");
					break;
				case 3:
					TempDbTflv.m_tvlength = strlen("体力") + 1;
					sprintf((char*)&TempDbTflv.lpdata, "%s", "体力");
					break;
				case 4:
					TempDbTflv.m_tvlength = strlen("灵力") + 1;
					sprintf((char*)&TempDbTflv.lpdata, "%s", "灵力");
					break;
				case 5:
					TempDbTflv.m_tvlength = strlen("异能") + 1;
					sprintf((char*)&TempDbTflv.lpdata, "%s", "异能");
					break;
				default:
					TempDbTflv.m_tvlength = strlen("未知") + 1;
					sprintf((char*)&TempDbTflv.lpdata, "%s", "未知");
					break;
				}
				DBTFLV.push_back(TempDbTflv);

				//最后一次增幅附加值
				szData.Format("%i", sResCont.res_list[i].property.strengthenLastTimeQuantity);
				TempDbTflv.nIndex  = DBTFLV.size() + 1;
				TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
				TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_IncreaseEndValue;
				TempDbTflv.m_tvlength = szData.GetLength()+1;
				sprintf((char*)&TempDbTflv.lpdata, "%s", szData.GetBuffer(0));
				DBTFLV.push_back(TempDbTflv);

				//增幅次数
				szData.Format("%i", sResCont.res_list[i].property.strengthenEffect);
				TempDbTflv.nIndex  = DBTFLV.size() + 1;
				TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
				TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_IncreaseNum;
				TempDbTflv.m_tvlength = szData.GetLength()+1;
				sprintf((char*)&TempDbTflv.lpdata, "%s", szData.GetBuffer(0));
				DBTFLV.push_back(TempDbTflv);

				//装备组装ID
				szData.Format("%i", sResCont.res_list[i].property.compositingEffect);
				TempDbTflv.nIndex  = DBTFLV.size() + 1;
				TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
				TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_AssemblyID;
				TempDbTflv.m_tvlength = szData.GetLength()+1;
				sprintf((char*)&TempDbTflv.lpdata, "%s", szData.GetBuffer(0));
				DBTFLV.push_back(TempDbTflv);
				
				//增幅附加值
				szData.Format("%i", sResCont.res_list[i].property.strengthenQuantity);
				TempDbTflv.nIndex  = DBTFLV.size() + 1;
				TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
				TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_IncreaseValue;
				TempDbTflv.m_tvlength = szData.GetLength()+1;
				sprintf((char*)&TempDbTflv.lpdata, "%s", szData.GetBuffer(0));
				DBTFLV.push_back(TempDbTflv);

				//装备镶嵌宝石的ID1
				szData.Format("%i", sResCont.res_list[i].property.equipSlotEffect1);
				TempDbTflv.nIndex  = DBTFLV.size() + 1;
				TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
				TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_EquipSlotID1;
				TempDbTflv.m_tvlength = szData.GetLength()+1;
				sprintf((char*)&TempDbTflv.lpdata, "%s", szData.GetBuffer(0));
				DBTFLV.push_back(TempDbTflv);
				
				//装备镶嵌宝石的ID2
				szData.Format("%i", sResCont.res_list[i].property.equipSlotEffect2);
				TempDbTflv.nIndex  = DBTFLV.size() + 1;
				TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
				TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_EquipSlotID2;
				TempDbTflv.m_tvlength = szData.GetLength()+1;
				sprintf((char*)&TempDbTflv.lpdata, "%s", szData.GetBuffer(0));
				DBTFLV.push_back(TempDbTflv);

				//装备镶嵌宝石的ID3
				szData.Format("%i", sResCont.res_list[i].property.equipSlotEffect3);
				TempDbTflv.nIndex  = DBTFLV.size() + 1;
				TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
				TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_EquipSlotID3;
				TempDbTflv.m_tvlength = szData.GetLength()+1;
				sprintf((char*)&TempDbTflv.lpdata, "%s", szData.GetBuffer(0));
				DBTFLV.push_back(TempDbTflv);
				
				//增幅为多少星(星级)
				szData.Format("%i", (sResCont.res_list[i].property.equipSlotEffect3 & STARLEVEL));
				TempDbTflv.nIndex  = DBTFLV.size() + 1;
				TempDbTflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
				TempDbTflv.m_tagName = CEnumCore::TagName::XDJG_StarsLevel;
				TempDbTflv.m_tvlength = szData.GetLength()+1;
				sprintf((char*)&TempDbTflv.lpdata, "%s", szData.GetBuffer(0));
				DBTFLV.push_back(TempDbTflv);

				//随机属性值
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "玩家当前没有该道具", "物品属性");
	}

	return DBTFLV;
}

//物品属性查询
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
		sprintf(retMessage, "物品属性查询:<大区>%s, <角色>%s, <道具id>%s", ServerIP, Character, ItemId);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		//获取GameDB的ip
		if(!m_operXDJG.GetServerIP(gameDBServerIP,ServerIP,3))
		{
			return DBTFLV;//无法获取GameDB的IP
		}
		
		//取到二进制物品属性字段
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
				memcpy(&m_StructInfo.m_sResCont , m_DataRecord.recorddata, sizeof(m_StructInfo.m_sResCont));//物品属性
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
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取道具ID出错", "物品属性");
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

//公会信息查询
//公会基本信息
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
		sprintf(retMessage, "公会基本信息查询:<大区>%s, <工会名称>%s", ServerIP, GuildName);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,3))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "公会基本信息查询");
			return DBTFLV;//无法获取真正的服务器IP
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "公会基本信息查询");//将错误信息写入日志中并返回错误信息
	}
	
	return DBTFLV;
}

//公会成员列表
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
		sprintf(retMessage, "公会成员列表查询:<大区>%s, <工会名称>%s", ServerIP, GuildName);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,3))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "公会成员列表");
			return DBTFLV;//无法获取真正的服务器IP
		}
		
		strSql.Format("SELECT a.guildid AS XDJG_GuildID, '%s' AS XDJG_GuildName, \
							  b.name AS XDJG_Guildmember, \
							  CASE b.charindex WHEN a.leaderguid THEN '会长' \
											   ELSE '普通会员' END AS XDJG_GuildPosition,\
							  b.create_time AS XDJG_JoinTime \
						 FROM %s.family a, %s.family_member b \
						WHERE a.guid = b. familyid \
						  AND a.name = '%s'", \
							  GuildName,GameDB,GameDB,GuildName);
		
		DBTFLV = GameDBQuery("XDJG",realServerIP,12, 0, 0, strSql.GetBuffer(0), GameDB, GameDB);
	}
	catch(...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "公会成员列表");//将错误信息写入日志中并返回错误信息
	}
	
	return DBTFLV;
}

//玩家任务查询
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
		sprintf(retMessage, "玩家任务查询:<大区>%s, <帐号>%s, <角色>%s", ServerIP, Account, Character);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,3))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "玩家任务查询");
			return DBTFLV;//无法获取真正的服务器IP
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
			//玩家帐号
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Account;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(Account) + 1;
			sprintf((char *)&m_tflv.lpdata,"%s",Account);
			DBTFLV.push_back(m_tflv);

			//角色名
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Character;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(Character) + 1;
			sprintf((char *)&m_tflv.lpdata,"%s",Character);
			DBTFLV.push_back(m_tflv);

			//任务名称
			szData.Format("%i", m_StructInfo.m_sTaskCont.task_count);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_Task_Name;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);
			
			//任务ID
			szData.Format("%i", m_StructInfo.m_sTaskCont.task_count);
			m_tflv.nIndex  = DBTFLV.size() + 1;
			m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tagName = CEnumCore::TagName::XDJG_Task_ID;
			m_tflv.m_tvlength = szData.GetLength()+1;
			sprintf((char*)&m_tflv.lpdata, "%s", szData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//完成情况
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "公会基本信息查询");//将错误信息写入日志中并返回错误信息
	}
	
	return DBTFLV;
}

//金刚车辆信息
//金刚车辆基本信息
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
		sprintf(retMessage, "金刚车辆基本信息查询:<大区>%s, <帐号>%s, <角色>%s", ServerIP, Account, Character);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,3))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "金刚车辆基本信息");
			return DBTFLV;//无法获取真正的服务器IP
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
		
		//取当前身上的道具
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
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "玩家身上没有金刚", "金刚车辆基本信息");
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
			//玩家帐号
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Account;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(Account) + 1;
			sprintf((char *)&m_tflv.lpdata,"%s",Account);
			DBTFLV.push_back(m_tflv);

			//角色名
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Character;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(Character) + 1;
			sprintf((char *)&m_tflv.lpdata,"%s",Character);
			DBTFLV.push_back(m_tflv);

			//金刚ID
			strData.Format("%d", Vec_Car[iCount].carId);
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_CarID;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strData.GetLength()+1;
			sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//等级
			strData.Format("%d", Vec_Car[iCount]._Level);
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Level;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strData.GetLength()+1;
			sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//天赋附加值
			strData.Format("%d", Vec_Car[iCount]._TranSkillStrID);
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_SkillStrValue;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strData.GetLength()+1;
			sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//当前金刚技能ID
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
					//时间芯片IDs1
					strData.Format("%d", Vec_Car[iCount]._TranPluginSlots[iNum]);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_TimePlugSlot1;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=strData.GetLength()+1;
					sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
					DBTFLV.push_back(m_tflv);
					break;
				case 1:
					//时间芯片IDs2
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "金刚车辆基本信息");//将错误信息写入日志中并返回错误信息
	}
	
	return DBTFLV;
}

//金刚车辆详细信息
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
		sprintf(retMessage, "金刚车辆详细信息查询:<大区>%s, <帐号>%s, <角色>%s", ServerIP, Account, Character);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,3))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "金刚车辆详细信息");
			return DBTFLV;//无法获取真正的服务器IP
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
		
		//取当前身上的道具
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
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "玩家身上没有金刚", "金刚车辆详细信息");
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
			//玩家帐号
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Account;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(Account) + 1;
			sprintf((char *)&m_tflv.lpdata,"%s",Account);
			DBTFLV.push_back(m_tflv);

			//角色名
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Character;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(Character) + 1;
			sprintf((char *)&m_tflv.lpdata,"%s",Character);
			DBTFLV.push_back(m_tflv);

			//金刚ID
			strData.Format("%d", Vec_Car[iCount].carId);
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_CarID;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strData.GetLength()+1;
			sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//等级
			strData.Format("%d", Vec_Car[iCount]._Level);
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Level;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strData.GetLength()+1;
			sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//天赋附加值
			strData.Format("%d", Vec_Car[iCount]._TranSkillStrID);
			m_tflv.nIndex=DBTFLV.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_SkillStrValue;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strData.GetLength()+1;
			sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
			DBTFLV.push_back(m_tflv);

			//当前金刚技能ID
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
					//时间芯片IDs1
					strData.Format("%d", Vec_Car[iCount]._TranPluginSlots[iNum]);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_TimePlugSlot1;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=strData.GetLength()+1;
					sprintf((char *)&m_tflv.lpdata,"%s",strData.GetBuffer(0));
					DBTFLV.push_back(m_tflv);
					break;
				case 1:
					//时间芯片IDs2
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "金刚车辆详细信息");//将错误信息写入日志中并返回错误信息
	}
	
	return DBTFLV;
}

//玩家公会信息
//玩家公会基本信息
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
		sprintf(retMessage, "玩家公会基本信息:<大区>%s, <帐号>%s, <角色>%s", ServerIP, Account, Character);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,3))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "玩家公会基本信息");
			return DBTFLV;//无法获取真正的服务器IP
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "玩家公会基本信息");//将错误信息写入日志中并返回错误信息
	}
	
	return DBTFLV;
}

//玩家公会物品信息
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

		sprintf(retMessage, "玩家公会物品信息:<大区>%s, <帐号>%s, <角色>%s", ServerIP, Account, Character);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,3))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "玩家公会物品信息");
			return DBTFLV;//无法获取真正的服务器IP
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
		
		//取物品名称
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "玩家公会物品信息");//将错误信息写入日志中并返回错误信息
	}
	
	return DBTFLV;
}

//玩家物品日志
//装备增幅日志
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
		sprintf(retMessage, "装备增幅日志:<大区>%s, <帐号>%s, <角色>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, BeginTime, EndTime);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "装备增幅日志");
			return DBTFLV;//无法获取真正的服务器IP
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "装备增幅日志");//将错误信息写入日志中并返回错误信息
	}
	
	return DBTFLV;
}

//插片合成日志
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
		sprintf(retMessage, "插片合成日志:<大区>%s, <帐号>%s, <角色>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, BeginTime, EndTime);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "插片合成日志");
			return DBTFLV;//无法获取真正的服务器IP
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "插片合成日志");//将错误信息写入日志中并返回错误信息
	}
	
	return DBTFLV;
}

//装备传导日志
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
		sprintf(retMessage, "装备传导日志:<大区>%s, <帐号>%s, <角色>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, BeginTime, EndTime);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "装备传导日志");
			return DBTFLV;//无法获取真正的服务器IP
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "装备传导日志");//将错误信息写入日志中并返回错误信息
	}
	
	return DBTFLV;
}

//物品鉴定
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
		sprintf(retMessage, "物品鉴定:<大区>%s, <帐号>%s, <角色>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, BeginTime, EndTime);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "物品鉴定");
			return DBTFLV;//无法获取真正的服务器IP
		}
		
		strSql.Format("SELECT '%s' AS XDJG_Account, '%s' AS XDJG_Character, \
							  item_id AS XDJG_ItemID, create_time AS XDJG_OperateTime \
						 from %s.IDENTIFY \
						where create_time between '%s' and '%s' \
						  and character_name = '%s'",Account,Character,LogDB,BeginTime,EndTime,Character);
		
		DBTFLV = GameDBQuery("XDJG",realServerIP,13, iIndex, iPageSize, strSql.GetBuffer(0), LogDB, LogDB);

		//取物品名称
		GetItemInfo(VecItemInfo);

		for(pDBTFLV = DBTFLV.begin();pDBTFLV != DBTFLV.end();pDBTFLV++)
		{
			if(pDBTFLV->m_tagName == CEnumCore::TagName::XDJG_ItemID)
			{
				//物品名称
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "物品鉴定");//将错误信息写入日志中并返回错误信息
	}
	
	return DBTFLV;
}

//个人仓库存放日志
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
		sprintf(retMessage, "个人仓库存放日志:<大区>%s, <帐号>%s, <角色>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, BeginTime, EndTime);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "装备传导日志");
			return DBTFLV;//无法获取真正的服务器IP
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "装备传导日志");//将错误信息写入日志中并返回错误信息
	}
	
	return DBTFLV;
}

//公会仓库存放日志
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
		sprintf(retMessage, "公会仓库存放日志:<大区>%s, <帐号>%s, <角色>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, BeginTime, EndTime);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "公会仓库存放日志");
			return DBTFLV;//无法获取真正的服务器IP
		}
		
		strSql.Format("SELECT '%s' AS XDJG_Account, '%s' AS XDJG_Character, \
							  item_id AS XDJG_TempItemID, item_id AS XDJG_ItemID, \
							  item_num AS XDJG_ItemNum, '存入' AS XDJG_Operatetype, \
							  create_time AS XDJG_OperateTime \
						 FROM %s.GUILD_STOREITEM \
						where create_time between '%s' and '%s' \
						  and character_name = '%s' \
						UNION \
					   SELECT '%s' AS XDJG_Account, '%s' AS XDJG_Character, \
							  item_id AS XDJG_TempItemID, item_id AS XDJG_ItemID, \
							  item_num AS XDJG_ItemNum, '取出' AS XDJG_Operatetype, \
							  create_time AS XDJG_OperateTime \
						 FROM %s.GUILD_GETITEM \
						where create_time between '%s' and '%s' \
						  and character_name = '%s'",Account,Character,LogDB,BeginTime,EndTime,Character, Account,Character,LogDB,BeginTime,EndTime,Character);
		
 		DBTFLV = GameDBQuery("XDJG",realServerIP,13, 0, 0, strSql.GetBuffer(0), LogDB, LogDB);

		//取物品名称
		GetItemInfo(VecItemInfo);

		for(pDBTFLV = DBTFLV.begin();pDBTFLV != DBTFLV.end();pDBTFLV++)
		{
			if(pDBTFLV->m_tagName == CEnumCore::TagName::XDJG_TempItemID)
			{
				//物品名称
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "公会仓库存放日志");//将错误信息写入日志中并返回错误信息
	}
	
	return DBTFLV;
}

//公会日志
//公会成立
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
		sprintf(retMessage, "公会成立日志:<大区>%s, <工会名称>%s, <开始时间>%s, <结束时间>%s", ServerIP, Guildname, BeginTime, EndTime);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG

		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "公会成立日志");
			return DBTFLV;//无法获取真正的服务器IP
		}
		
		strSql.Format("SELECT create_time AS XDJG_CreateTime, character_name AS XDJG_Character, \
							  character_name AS XDJG_TempCharacter, \
							  guild_name AS XDJG_GuildName, money_lose AS XDJG_MoneyLose \
							  FROM %s.CREATE_GUILD \
							  where create_time between '%s' and '%s' \
							    and guild_name = '%s'",LogDB,BeginTime,EndTime,Guildname);
		
		DBTFLV = GameDBQuery("XDJG",realServerIP,13, 0, 0, strSql.GetBuffer(0), LogDB, LogDB);

		//到GameDB取玩家帐号
		for(pDBTFLV = DBTFLV.begin(); pDBTFLV != DBTFLV.end(); pDBTFLV++)
		{
			if(pDBTFLV->m_tagName == CEnumCore::TagName::XDJG_Character)
			{
				if(!m_operXDJG.GetServerIP(GameDBServerIP,ServerIP,3))
				{
					NEW_DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取GameDB的IP错误", "公会成立日志");
					return NEW_DBTFLV;//无法获取真正的服务器IP
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

		//把临时角色转换成玩家帐号
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "公会成立日志");//将错误信息写入日志中并返回错误信息
	}
	
	return DBTFLV;
}

//公会解散
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
		sprintf(retMessage, "公会解散日志:<大区>%s, <工会名称>%s, <开始时间>%s, <结束时间>%s", ServerIP, Guildname, BeginTime, EndTime);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG

		//到GameDb取公会id
		if(!m_operXDJG.GetServerIP(GameDBServerIP,ServerIP,3))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取GameDB的IP错误", "公会解散日志");
			return DBTFLV;//无法获取真正的服务器IP
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
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "公会解散日志");
			return DBTFLV;//无法获取真正的服务器IP
		}
		
		strSql.Format("SELECT create_time AS XDJG_CreateTime, character_name AS XDJG_Character, \
							  character_name AS XDJG_TempCharacter, '%s' AS XDJG_GuildName \
						 FROM %s.DISMISS_GUILD \
						where create_time between '%s' and '%s' \
						  and guild_id = '%s'",Guildname,LogDB,BeginTime,EndTime,strGuid.c_str());
		
		DBTFLV = GameDBQuery("XDJG",realServerIP,13, 0, 0, strSql.GetBuffer(0), LogDB, LogDB);
		
		//到GameDB取玩家帐号
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
		
		//把临时角色转换成玩家帐号
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "公会解散日志");//将错误信息写入日志中并返回错误信息
	}
	
	return DBTFLV;
}

//根据角色取帐号
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
		sprintf(retMessage, "根据角色取帐号:<大区>%s, <角色>%s", ServerIP, RoleName);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
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

//公会添加成员		
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
		sprintf(retMessage, "公会添加成员:<大区>%s, <工会名称>%s, <开始时间>%s, <结束时间>%s", ServerIP, Guildname, BeginTime, EndTime);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,3))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "公会添加成员");
			return DBTFLV;//无法获取真正的服务器IP
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

				//根据角色取帐号
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "公会添加成员");//将错误信息写入日志中并返回错误信息
	}

	return DBTFLV;
}

//公会成员离开
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
		sprintf(retMessage, "公会成员离开:<大区>%s, <工会名称>%s, <开始时间>%s, <结束时间>%s", ServerIP, Guildname, BeginTime, EndTime);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,3))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "公会成员离开");
			return DBTFLV;//无法获取真正的服务器IP
		}
		
		strSql.Format("SELECT FROM_UNIXTIME(TIMESTAMP) AS XDJG_LeaveTime, \
							  '%s' AS XDJG_GuildName, \
							  CASE event_type WHEN 4 THEN '强制离开' \
 													 ELSE '主动离开' END AS XDJG_ExitGuildType, \
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
				
				if(!strcmp((char*)m_tflv.lpdata, "强制离开"))
				{
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_Character;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=strlen(m_StructInfo.m_misc.name2) + 1;
					sprintf((char *)&m_tflv.lpdata,"%s",m_StructInfo.m_misc.name2);
					DBTFLV.push_back(m_tflv);
					
					//根据角色取帐号
					m_tflv = GetAccount(realServerIP, m_StructInfo.m_misc.name2);
					m_tflv.nIndex = DBTFLV.size()+1;
					DBTFLV.push_back(m_tflv);
				}
				else if(!strcmp((char*)m_tflv.lpdata, "主动离开"))
				{
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::XDJG_Character;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=strlen(m_StructInfo.m_misc.name1) + 1;
					sprintf((char *)&m_tflv.lpdata,"%s",m_StructInfo.m_misc.name1);
					DBTFLV.push_back(m_tflv);
					
					//根据角色取帐号
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "公会成员离开");//将错误信息写入日志中并返回错误信息
	}
	
	return DBTFLV;
}

//公会职务变更
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
		sprintf(retMessage, "公会职务变更:<大区>%s, <工会名称>%s, <开始时间>%s, <结束时间>%s", ServerIP, Guildname, BeginTime, EndTime);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,3))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "公会职务变更");
			return DBTFLV;//无法获取真正的服务器IP
		}
		
		strSql.Format("SELECT FROM_UNIXTIME(TIMESTAMP) AS XDJG_LeaderChangedTime, \
						      '%s' AS XDJG_GuildName, misc AS XDJG_GuildLog, \
							  '会长' AS XDJG_GuildTransferedPosition \
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
				
				//根据角色取帐号
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "公会职务变更");//将错误信息写入日志中并返回错误信息
	}
	
	return DBTFLV;
}

//帐号是否封停
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_AccountIsClose_Query(char *ServerIP, char *Account)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	char realServerIP[40];
	CString strSql = "";
	char retMessage[255];
	ZeroMemory(retMessage,255);
	
	try
	{
		sprintf(retMessage, "帐号是否封停:<大区>%s, <帐号>%s", ServerIP, Account);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,4))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "帐号是否封停");
			return DBTFLV;//无法获取真正的服务器IP
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "帐号是否封停");//将错误信息写入日志中并返回错误信息
	}
	
	if(DBTFLV.empty())
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未封停", "帐号是否封停");

	return DBTFLV;
}

//NPC道具领取查询
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
		sprintf(retMessage, "NPC道具领取查询:<大区>%s, <帐号>%s, <帐号ID>%d, <角色>%s", ServerIP, Account,AccountID, Character);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		if(!m_operXDJG.GetServerIP(realServerIP,ServerIP,2))
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "获取IP地址错误", "帐号是否封停");
			return DBTFLV;//无法获取真正的服务器IP
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
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "NPC道具领取查询");//将错误信息写入日志中并返回错误信息
	}
	
	return DBTFLV;
}

//获取领奖包信息
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
		logFile.WriteLog("<侠盗金刚>",operName,"获取领奖包信息");//记LOG
		
		//取任务名称
		strSql.Format("select PackageName AS XDJG_ActivityName,Item_1_id AS XDJG_ItemID1, \
							  Item_1_Num AS XDJG_ItemNum1,Item_2_id AS XDJG_ItemID2, \
							  Item_2_Num AS XDJG_ItemNum2,Item_3_id AS XDJG_ItemID3, \
							  Item_3_Num AS XDJG_ItemNum3,Item_4_id AS XDJG_ItemID4, \
							  Item_4_Num AS XDJG_ItemNum4,Item_5_id AS XDJG_ItemID5, \
							  Item_5_Num AS XDJG_ItemNum5,Item_6_id AS XDJG_ItemID6, \
							  Item_6_Num AS XDJG_ItemNum6, \
							  CASE gold_type WHEN 8 THEN '金刚徽章' \
											 ELSE '未知' END AS XDJG_MoneyType,gold AS XDJG_Money, \
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
		
		//取物品名称
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
					//物品名称
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
					//物品名称
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
					//物品名称
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
					//物品名称
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
					//物品名称
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
					//物品名称
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
					//物品名称
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
					//物品名称
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
					//物品名称
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
					//物品名称
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
					//物品名称
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
					//物品名称
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

//添加称号
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
		sprintf(retMessage, "添加称号:<大区>%s, <帐号>%s, <角色>%s, <称号ID>%d, <称号名称>%s", ServerIP, Account, Character, iTitleID, TitleName);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=add_title(%s,%d)", 
			ServerIP, HTTPPORT,
			Character,iTitleID);
		
		if(m_operXDJG.getWebInfo(strConn, strResult))
		{
			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "添加称号");				
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
							
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "添加成功", "添加称号");
							
							break;
						case -2:
							wait_num++;
							if (wait_num > Maxnum_wait)
							{
								bFinishFlag = true; 
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "等待超时", "添加称号");
							}
							Sleep(1000);
							break;
						default:
							bFinishFlag = true;
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "添加失败", "添加称号");
						}
					}
				}
				else
				{
					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "添加失败", "添加称号");
				}
			}
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "添加称号");		
		}
	}
	catch (...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "添加称号");	
	}
	
	m_operXDJG.GmOperLog(userByID, ServerIP, "添加称号", retMessage, DBTFLV[0]);
	return DBTFLV;
}

//移除称号
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
		sprintf(retMessage, "移除称号:<大区>%s, <帐号>%s, <角色>%s, <称号ID>%d, <称号名称>%s", ServerIP, Account, Character, iTitleID, TitleName);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=remove_title(%s,%d)", 
			ServerIP, HTTPPORT,
			Character,iTitleID);
		
		if(m_operXDJG.getWebInfo(strConn, strResult))
		{
			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "移除称号");				
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
							
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "移除成功", "移除称号");
							
							break;
						case -2:
							wait_num++;
							if (wait_num > Maxnum_wait)
							{
								bFinishFlag = true; 
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "等待超时", "移除称号");
							}
							Sleep(1000);
							break;
						default:
							bFinishFlag = true;
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "移除失败", "移除称号");
						}
					}
				}
				else
				{
					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "移除失败", "移除称号");
				}
			}
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "移除称号");		
		}
	}
	catch (...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "移除称号");	
	}
	
	m_operXDJG.GmOperLog(userByID, ServerIP, "移除称号", retMessage, DBTFLV[0]);
	return DBTFLV;
}

//添加BUFF
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
		sprintf(retMessage, "添加BUFF:<大区>%s, <帐号>%s, <角色>%s, <BUFF ID>%d, <BUFF名称>%s", ServerIP, Account, Character, iBuffID, BuffName);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=add_buff(%s,%d)", 
			ServerIP, HTTPPORT,
			Character,iBuffID);
		
		if(m_operXDJG.getWebInfo(strConn, strResult))
		{
			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "添加BUFF");				
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
							
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "添加成功", "添加BUFF");
							
							break;
						case -2:
							wait_num++;
							if (wait_num > Maxnum_wait)
							{
								bFinishFlag = true; 
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "等待超时", "添加BUFF");
							}
							Sleep(1000);
							break;
						default:
							bFinishFlag = true;
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "添加失败", "添加BUFF");
						}
					}
				}
				else
				{
					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "添加失败", "添加BUFF");
				}
			}
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "添加BUFF");		
		}
	}
	catch (...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "添加BUFF");	
	}
	
	m_operXDJG.GmOperLog(userByID, ServerIP, "添加BUFF", retMessage, DBTFLV[0]);
	return DBTFLV;
}

//移除BUFF
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
		sprintf(retMessage, "添加BUFF:<大区>%s, <帐号>%s, <角色>%s, <BUFF ID>%d, <BUFF名称>%s", ServerIP, Account, Character, iBuffID, BuffName);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=remove_buff(%s,%d)", 
			ServerIP, HTTPPORT,
			Character,iBuffID);
		
		if(m_operXDJG.getWebInfo(strConn, strResult))
		{
			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "移除BUFF");				
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
							
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "移除成功", "移除BUFF");
							
							break;
						case -2:
							wait_num++;
							if (wait_num > Maxnum_wait)
							{
								bFinishFlag = true; 
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "等待超时", "移除BUFF");
							}
							Sleep(1000);
							break;
						default:
							bFinishFlag = true;
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "移除失败", "移除BUFF");
						}
					}
				}
				else
				{
					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "移除失败", "移除BUFF");
				}
			}
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "移除BUFF");		
		}
	}
	catch (...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "移除BUFF");	
	}
	
	m_operXDJG.GmOperLog(userByID, ServerIP, "移除BUFF", retMessage, DBTFLV[0]);
	return DBTFLV;
}

//移动到指定位置
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
		sprintf(retMessage, "移动到指定位置:<大区>%s, <帐号>%s, <角色>%s, <X坐标>%d, <Y坐标>%d, <Z坐标>%d", ServerIP, Account, Character, iPX, iPY, iPZ);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=move(%s,%d,%d,%d)", 
			ServerIP, HTTPPORT,
			Character,iPX,iPY,iPZ);
		
		if(m_operXDJG.getWebInfo(strConn, strResult))
		{
			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "移动到指定位置");				
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
							
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "移动成功", "移动到指定位置");
							
							break;
						case -2:
							wait_num++;
							if (wait_num > Maxnum_wait)
							{
								bFinishFlag = true; 
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "等待超时", "移动到指定位置");
							}
							Sleep(1000);
							break;
						default:
							bFinishFlag = true;
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "移动失败", "移动到指定位置");
						}
					}
				}
				else
				{
					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "移动失败", "移动到指定位置");
				}
			}
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "移动到指定位置");		
		}
	}
	catch (...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "移动到指定位置");	
	}
	
	m_operXDJG.GmOperLog(userByID, ServerIP, "移动到指定位置", retMessage, DBTFLV[0]);
	return DBTFLV;
}

//移动到目标玩家身边
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
		sprintf(retMessage, "移动到目标玩家身边:<大区>%s, <帐号>%s, <角色>%s, <目标角色>%s", ServerIP, Account, Character, TargetCharacter);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=goto(%s,%s)", 
			ServerIP, HTTPPORT,
			Character,TargetCharacter);
		
		if(m_operXDJG.getWebInfo(strConn, strResult))
		{
			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "移动到指定玩家身边");				
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
							
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "移动成功", "移动到目标玩家身边");
							
							break;
						case -2:
							wait_num++;
							if (wait_num > Maxnum_wait)
							{
								bFinishFlag = true; 
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "等待超时", "移动到目标玩家身边");
							}
							Sleep(1000);
							break;
						default:
							bFinishFlag = true;
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "移动失败", "移动到目标玩家身边");
						}
					}
				}
				else
				{
					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "移动失败", "移动到目标玩家身边");
				}
			}
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "移动到目标玩家身边");		
		}
	}
	catch (...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "移动到目标玩家身边");	
	}
	
	m_operXDJG.GmOperLog(userByID, ServerIP, "移动到目标玩家身边", retMessage, DBTFLV[0]);
	return DBTFLV;
}

//移动到指定玩家身边
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
		sprintf(retMessage, "移动到指定玩家身边:<大区>%s, <帐号>%s, <角色>%s, <目标角色>%s", ServerIP, Account, Character, TargetCharacter);
		logFile.WriteLog("<侠盗金刚>",operName,retMessage);//记LOG
		
		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=summon(%s,%s)", 
			ServerIP, HTTPPORT,
			Character,TargetCharacter);
		
		if(m_operXDJG.getWebInfo(strConn, strResult))
		{
			if((strResult[0]<'0' || strResult[0]>'9') && strResult[0] != '-')
			{
				DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "移动到指定玩家身边");				
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
							
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "移动成功", "移动到指定玩家身边");
							
							break;
						case -2:
							wait_num++;
							if (wait_num > Maxnum_wait)
							{
								bFinishFlag = true; 
								DBTFLV = m_operXDJG.ReturnOpMsg(operName, "等待超时", "移动到指定玩家身边");
							}
							Sleep(1000);
							break;
						default:
							bFinishFlag = true;
							DBTFLV = m_operXDJG.ReturnOpMsg(operName, "移动失败", "移动到指定玩家身边");
						}
					}
				}
				else
				{
					DBTFLV = m_operXDJG.ReturnOpMsg(operName, "移动失败", "移动到指定玩家身边");
				}
			}
		}
		else
		{
			DBTFLV = m_operXDJG.ReturnOpMsg(operName, "连接失败", "移动到指定玩家身边");		
		}
	}
	catch (...)
	{
		DBTFLV = m_operXDJG.ReturnOpMsg(operName, "未知错误", "移动到指定玩家身边");	
	}
	
	m_operXDJG.GmOperLog(userByID, ServerIP, "移动到指定玩家身边", retMessage, DBTFLV[0]);
	return DBTFLV;
}

//称号列表查询
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_TitleList_Query(char *Gamename, char *Sql)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	try
	{
		DBTFLV=GameDBQuery(Gamename,"127.0.0.1",4,0,0,Sql);
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog(Gamename,"道具列表查询内存不足");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog(Gamename,"道具列表查询文件异常");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog(Gamename,"道具列表查询其他异常");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog(Gamename,(char *)err.what());
		return DBTFLV;
	}
	return DBTFLV;
}

//BUFF列表查询
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_BuffList_Query(char *Gamename, char *Sql)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	try
	{
		DBTFLV=GameDBQuery(Gamename,"127.0.0.1",4,0,0,Sql);
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog(Gamename,"道具列表查询内存不足");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog(Gamename,"道具列表查询文件异常");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog(Gamename,"道具列表查询其他异常");
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
		sprintf(retMessage,"%s","连接失败");
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
					sprintf(retMessage,"%s","添加公告成功");
					m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata,"%s",(char *)&retMessage);
					m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
					logFile.WriteLog("XDJG","XDJG",retMessage);
					DBTFLV.push_back(m_tflv);
					break;
				case 2:
					DBTFLV = m_operXDJG.ReturnOpMsg(strLog,"Update_BoardTasker_Success");
					sprintf(retMessage,"%s","修改公告成功");
					m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata,"%s",(char *)&retMessage);
					m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
					logFile.WriteLog("XDJG","XDJG",retMessage);
					DBTFLV.push_back(m_tflv);
					break;
				case 3:
					DBTFLV = m_operXDJG.ReturnOpMsg(strLog,"Delete_BoardTasker_Success");
					sprintf(retMessage,"%s","删除公告成功");
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
					sprintf(retMessage,"%s","添加公告失败");
					m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata,"%s",(char *)&retMessage);
					m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
					logFile.WriteLog("XDJG","XDJG",retMessage);
					DBTFLV.push_back(m_tflv);
					break;
				case 2:
					//DBTFLV = m_operXDJG.ReturnOpMsg(strLog,"Update_BoardTasker_Fail");
					sprintf(retMessage,"%s","修改公告失败");
					m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata,"%s",(char *)&retMessage);
					m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
					logFile.WriteLog("XDJG","XDJG",retMessage);
					DBTFLV.push_back(m_tflv);
					break;
				case 3:
					//DBTFLV = m_operXDJG.ReturnOpMsg(strLog,"Delete_BoardTasker_Fail");
					sprintf(retMessage,"%s","删除公告失败");
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
		sprintf(retMessage,"%s","连接失败");
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
