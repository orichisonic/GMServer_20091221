#include "StdAfx.h"
#include "XDJGInfo.h"

CXDJGInfo::CXDJGInfo(void)
{
}

CXDJGInfo::~CXDJGInfo(void)
{
}

//��ʼ�����еı���
COperVector * CXDJGInfo::initialize(CSession * pSess,CEnumCore::Message_Tag_ID MessageTagID, unsigned char * pbuf, int length)
{
	COperVector * operVector = NULL;
	try
	{
		ZeroMemory(serverip, 30);
		logFile.ReadValue("XDJG", "SERVER","IP",serverip, 30);//��ȡ���ص����ݿ��ַ
		port = logFile.ReadIntValue("XDJG", "SERVER", "PORT");

		//���û����ʼ������
		operVector = CGameInfo::initialize(pSess, MessageTagID, pbuf, length);
		
	}
	catch(...)
	{
	}
	return operVector;
}

//�û���Ϣ��ѯ
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
				sprintf(strRetMessage, "%s", "���ʧ��");
				break;
			}
		}		
	}
	else
	{
		sprintf(retMessage,"%s","��ѯʧ��");
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
			//��ɫ��
		case 1:
			m_tflv.nIndex=vectorColInfo->size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_UserNick;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			break;
			//��ɫid
		case 2:
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_NickID;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			break;
			//�ʺ���
		case 3:
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Account;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			break;
			//�ʺ�id
		case 4:
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_AccountID;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			break;
			//�ȼ�
		case 5:
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Level;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			break;
			//����
		case 6:
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Exp;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			break;
			//�Ա�
		case 7:
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Sex;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			break;
			//ְҵ
		case 8:
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Profession;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			break;
			//��������id
		case 9:
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_SceneTypeID;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			break;
			// ����ʵ��id
		case 10:
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_SceneInstanceID;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			break;
			//λ��
		case 11:
			m_tflv.m_tagName=CEnumCore::TagName::XDJG_Position;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			break;
			//��Ǯ��
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

//����
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

//�û��ʺŷ�ͣ
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
				sprintf(strRetMessage, "%s", "��ͣ�ɹ�");
				break;
			case -2:
				Sleep(1000);
				break;
			default:
				iFinishFlag = 1;
				sprintf(strRetMessage, "%s", "��ͣʧ��");
				break;
			}
		}		
	}
	else
	{
		sprintf(strRetMessage, "%s", "��ͣʧ��");
	}

	m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
	m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
	sprintf((char *)&m_tflv.lpdata,"%s",(char *)&retMessage);
	m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
	DBTFLV.push_back(m_tflv);
	logFile.WriteLog("XDJG","GM",retMessage);
	return DBTFLV;
}

//�û��ʺŽ��
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
				sprintf(strRetMessage, "%s", "���ɹ�");
				break;
			case -2:
				Sleep(1000);
				break;
			default:
				iFinishFlag = 1;
				sprintf(strRetMessage, "%s", "���ʧ��");
				break;
			}
		}		
	}
	else
	{
		sprintf(strRetMessage, "%s", "��ͣʧ��");
	}
	
	m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
	m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
	sprintf((char *)&m_tflv.lpdata,"%s",(char *)&retMessage);
	m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
	DBTFLV.push_back(m_tflv);
	logFile.WriteLog("XDJG","GM",retMessage);
	return DBTFLV;	
}

//�����б��ѯ
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
		
		//��ѯ�������ϸ��Ϣ
		sprintf(strConn, "http://%s:%d/rpc_proxy?app_name=ws&fun=query_notice(%s)", );
		m_operXDJG.getWebInfo(strConn, &vectorColInfo);

		vector <string>::iterator iter_col;
		int icolNum = 1;
		for (iter_col=vectorColInfo.begin(); iter_col!=vectorColInfo.end();iter_col++)
		{
						
			switch (icolNum)
			{
				//����id
			case 1:
				m_tflv.nIndex=vectorColInfo->size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_TaskID;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
				break;
				//��������
			case 2:
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_BoardMessage;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				break;
				//���濪ʼʱ��
			case 3:
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_StartTime;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				break;
				//���ʱ��
			case 4:
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_Interval;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
				m_tflv.m_tvlength=sizeof(int);
				break;
				//�ظ�����
			case 5:
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_Repeat;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
				m_tflv.m_tvlength=sizeof(int);
				break;
				//��ʼ����
			case 6:
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_StartDate;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=sizeof(int);
				break;
				//��������
			case 7:
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_EndDate;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=sizeof(int);
				break;
				//�´ι���ʱ��
			case 8:
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_NextTime;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				break;
				//��ִ�д���
			case 9:
				m_tflv.m_tagName=CEnumCore::TagName::XDJG_ExecTimes;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
				m_tflv.m_tvlength=sizeof(int);
				break;
				// ����
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

//�����б����
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

