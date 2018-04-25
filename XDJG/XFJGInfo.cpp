#include "StdAfx.h"
#include "XDJGInfo.h"

CXDJGInfo::CXDJGInfo(void)
{
}

CXDJGInfo::~CXDJGInfo(void)
{
}

//初始化类中的变量
COperVector * CXDJGInfo::initialize(CSession * pSess,CEnumCore::Message_Tag_ID MessageTagID, unsigned char * pbuf, int length)
{
	COperVector * operVector = NULL;
	try
	{
		ZeroMemory(serverip, 30);
		logFile.ReadValue("XDJG", "SERVER","IP",serverip, 30);//获取本地的数据库地址
		port = logFile.ReadIntValue("XDJG", "SERVER", "PORT");

		//调用基类初始化方法
		operVector = CGameInfo::initialize(pSess, MessageTagID, pbuf, length);
		
	}
	catch(...)
	{
	}
	return operVector;
}

//用户信息查询
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_Net_UserInfo(char * ServerIP, char * UserName, int type, int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <string> vectorInfo;
	vector <string> vectorColInfo;
	char strConn[512];
	ZeroMemory(strConn, 512);
	
	sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=query_player(%s,%d)", UserName, type);
	m_operXDJG.getWebInfo(strConn, &vectorInfo);
	int iResult = atoi((vectorInfo.front()).c_str());
	if (iResult>0)
	{
		int iFinishFlag = 0;
		while (!iFinishFlag)
		{
			sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=get_operation_result(%d)", iResult);
			m_operXDJG.getWebInfo(strConn, &vectorInfo);
			int iResultFlag = atoi((vectorInfo.front()).c_str());
			switch (iResultFlag)
			{
			case 0:
				iFinishFlag = 1;
				break;
			case -2:
				Sleep(1000);
				break;
			default:
				iFinishFlag = 1;
				sprintf(strRetMessage, "%s", "解封失败");
				break;
			}
		}		
	}
	else
	{
		sprintf(retMessage,"%s","查询失败");
		m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
		m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
		sprintf((char *)&m_tflv.lpdata,"%s",(char *)&retMessage);
		m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
//		logFile.WriteLog("WA","GM",retMessage);
		DBTFLV.push_back(m_tflv);
	}
	
	for (iter_col=vectorColInfo.begin(); iter_col!=vectorColInfo.end();iter_col++)
	{
		switch (icolNum)
		{
			//角色名
		case 1:
			m_tflv.nIndex=vectorColInfo->size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_UserNick;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			break;
			//角色id
		case 2:
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_NickID;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			break;
			//帐号名
		case 3:
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Account;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			break;
			//帐号id
		case 4:
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_AccountID;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			break;
			//等级
		case 5:
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Level;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			break;
			//经验
		case 6:
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Exp;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			break;
			//性别
		case 7:
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Sex;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			break;
			//职业
		case 8:
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Profession;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			break;
			//场景类型id
		case 9:
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_SceneTypeID;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			break;
			// 场景实例id
		case 10:
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_SceneInstanceID;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			break;
			//位置
		case 11:
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Position;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			break;
			//金钱数
		case 12:
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Money;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			break;
		default:
			break;
		}
	}
}

//踢人
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_Net_KickPlayer(int userByID,char * ServerIP,char * UserName, char * Account)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <string> vectorInfo;
	vector <string> vectorColInfo;
	char strConn[512];
	ZeroMemory(strConn, 512);
	
	if (!strcmp(UserName,""))
	{
		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=kick_player(%s)", UserName);
	}
	else
	{
		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=kick_player(%s)", Account);
	}

	m_operXDJG.getWebInfo(strConn, &vectorInfo);

	int iResult = atoi((vectorInfo.front()).c_str());
	switch (iResult)
	{
	case 0:
		DBTFLV = m_operXDJG.ReturnOpMsg(strLog,"kick_Success");
		break;

	default:
		DBTFLV = m_operXDJG.ReturnOpMsg(strLog,"kick_Fail");
		break;

	}
	return DBTFLV;
}

//用户帐号封停
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_Net_ClosePlayer(int userByID,char * ServerIP,char * UserName,int iTime,char * Reason)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <string> vectorInfo;
	vector <string> vectorColInfo;
	char strConn[512];
	char strRetMessage[255];
	ZeroMemory(strConn, 512);
	
	sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=disable_account(%s, %d, %s, %s )", UserName, iTime, " ", Reason);
	m_operXDJG.getWebInfo(strConn, &vectorInfo);
	int iResult = atoi((vectorInfo.front()).c_str());
	if (iResult>=0)
	{
		int iFinishFlag = 0;
		while (!iFinishFlag)
		{
			sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=get_operation_result(%d)", iResult);
			m_operXDJG.getWebInfo(strConn, &vectorInfo);
			int iResultFlag = atoi((vectorInfo.front()).c_str());
			switch (iResultFlag)
			{
			case 1:
				iFinishFlag = 1;
				sprintf(strRetMessage, "%s", "封停成功");
				break;
			case -2:
				Sleep(1000);
				break;
			default:
				iFinishFlag = 1;
				sprintf(strRetMessage, "%s", "封停失败");
				break;
			}
		}		
	}
	else
	{
		sprintf(strRetMessage, "%s", "封停失败");
	}

	m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
	m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
	sprintf((char *)&m_tflv.lpdata,"%s",(char *)&retMessage);
	m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
	DBTFLV.push_back(m_tflv);
	logFile.WriteLog("XDJG","GM",retMessage);
	return DBTFLV;
}

//用户帐号解封
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_Net_OpenPlayer(int userByID,char * ServerIP,char * UserName,char * Reason)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <string> vectorInfo;
	vector <string> vectorColInfo;
	char strConn[512];
	char strRetMessage[255];
	ZeroMemory(strConn, 512);
	
	sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=enable_account(%s, %s)", UserName, Reason);
	m_operXDJG.getWebInfo(strConn, &vectorInfo);
	int iResult = atoi((vectorInfo.front()).c_str());
	if (iResult>=0)
	{
		int iFinishFlag = 0;
		while (!iFinishFlag)
		{
			sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=get_operation_result(%d)", iResult);
			m_operXDJG.getWebInfo(strConn, &vectorInfo);
			int iResultFlag = atoi((vectorInfo.front()).c_str());
			switch (iResultFlag)
			{
			case 1:
				iFinishFlag = 1;
				sprintf(strRetMessage, "%s", "解封成功");
				break;
			case -2:
				Sleep(1000);
				break;
			default:
				iFinishFlag = 1;
				sprintf(strRetMessage, "%s", "解封失败");
				break;
			}
		}		
	}
	else
	{
		sprintf(strRetMessage, "%s", "封停失败");
	}
	
	m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
	m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
	sprintf((char *)&m_tflv.lpdata,"%s",(char *)&retMessage);
	m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
	DBTFLV.push_back(m_tflv);
	logFile.WriteLog("XDJG","GM",retMessage);
	return DBTFLV;	
}

//公告列表查询
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_Net_BoardTasker_Query(int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <string> vectorInfo;
	vector <string> vectorColInfo;
	char strConn[512];
	
	ZeroMemory(strConn, 512);
	sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=query_notice(0)");
	
	m_operXDJG.getWebInfo(strConn, &vectorInfo);
	vector <string>::iterator iter;
	for (iter=vectorInfo.begin(); iter!=vectorInfo.end();iter++)
	{
		string strColMess = *iter;
		
		//查询公告的详细信息
		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=query_notice(%s)", );
		m_operXDJG.getWebInfo(strConn, &vectorColInfo);

		vector <string>::iterator iter_col;
		int icolNum = 1;
		for (iter_col=vectorColInfo.begin(); iter_col!=vectorColInfo.end();iter_col++)
		{
						
			switch (icolNum)
			{
				//公告id
			case 1:
				m_tflv.nIndex=vectorColInfo->size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_TaskID;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
				break;
				//公告内容
			case 2:
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_BoardMessage;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				break;
				//公告开始时间
			case 3:
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_StartTime;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				break;
				//间隔时间
			case 4:
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_Interval;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
				m_tflv.m_tvlength=sizeof(int);
				break;
				//重复次数
			case 5:
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_Repeat;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
				m_tflv.m_tvlength=sizeof(int);
				break;
				//起始日期
			case 6:
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_StartDate;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=sizeof(int);
				break;
				//结束日期
			case 7:
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_EndDate;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=sizeof(int);
				break;
				//下次公告时间
			case 8:
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_NextTime;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				break;
				//已执行次数
			case 9:
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_ExecTimes;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
				m_tflv.m_tvlength=sizeof(int);
				break;
				// 开关
			case 10:
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_Onoff;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
				m_tflv.m_tvlength=sizeof(int);
				break;
			default:
				break;
			}

			sprintf((LPSTR)&m_tflv.lpdata,"%s",);
			DBTFLV->push_back(m_tflv);
			case 
		}
	}
	return DBTFLV;
}

//公告列表操作
vector <CGlobalStruct::TFLV> CXDJGInfo::XDJG_Net_BoardTasker_Oper(int op_type,char *content, char *begintime, int interval, int repeat, char * begindate, char *enddate, int notice_index/*=0*/)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	vector <string> vectorInfo;
	char strConn[512];

	ZeroMemory(strConn, 512);
	sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=notice_operation(%d,%d,%s,%s,%s,%d,%s,%s)",
			serverip, port,
			op_type, notice_index, content, begintime, interval, begindate, enddate);
	
	m_operXDJG.getWebInfo(strConn, &vectorInfo);
	int iResult = atoi((vectorInfo.front()).c_str());
	switch (iResult)
	{
	case 0:
		{
			switch (op_type)
			{
			case 1:
				DBTFLV = m_operXDJG.ReturnOpMsg(strLog,"Add_BoardTasker_Success");
				break;
			case 2:
				DBTFLV = m_operXDJG.ReturnOpMsg(strLog,"Update_BoardTasker_Success");
				break;
			case 3:
				DBTFLV = m_operXDJG.ReturnOpMsg(strLog,"Delete_BoardTasker_Success");
				break;
			}
		}
		break;
	default:
		{
			switch (op_type)
			{
			case 1:
				DBTFLV = m_operXDJG.ReturnOpMsg(strLog,"Add_BoardTasker_Fail");
				break;
			case 2:
				DBTFLV = m_operXDJG.ReturnOpMsg(strLog,"Update_BoardTasker_Fail");
				break;
			case 3:
				DBTFLV = m_operXDJG.ReturnOpMsg(strLog,"Delete_BoardTasker_Fail");
				break;
			}
		}
		break;
	}

	return DBTFLV;
}

