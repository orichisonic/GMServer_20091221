// ModifyAccountBankPw.cpp: implementation of the CModifyAccountBankPw class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ModifyAccountBankPw.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CModifyAccountBankPw::CModifyAccountBankPw()
{

}

CModifyAccountBankPw::~CModifyAccountBankPw()
{

}
vector <CGlobalStruct::TFLV> CModifyAccountBankPw::ModifyAccountBankPwMain(char *szGMServerName, char * szGMServerIP,char* Account,char * szBankPw)
{

		/*	char strlog[80];
			sprintf(strlog,"����<%s>�޸��ʻ�<%s>��������",szGMServerName,Account);
			logFile.WriteText("�ɽ�OL",strlog);*/
			CGlobalStruct::TFLV m_tflv;
			if(!DBVect.empty())
			{
				DBVect.clear();
			}
			
			// ע������CallBack��ʽ
			g_ccNetObj.RegEvent_OnConnected(COperPal::OnConnect, 0);
			g_ccNetObj.RegEvent_OnDisconnected(COperPal::OnDisconnect, 0);
			g_ccNetObj.RegEvent_OnConnectFailed(COperPal::OnConnectfailed, 0);
			
			// ע���Ӧ�¼�CallBack��ʽ
			g_ccNetObj.RegEvent_OnPacket(ENUM_PGGMCConnection_StoC_LoginResult,	LoginResult, 0);
			g_ccNetObj.RegEvent_OnPacket(ENUM_PGPlayerControl_StoC_ModifyAccountBankPw, ModifyAccountBankPw, 0);
			
			// ��ʼ��·�趨
			g_ccNetObj.SetConnectCount(10);		// �趨�������ߴ���Ϊ10
			g_ccNetObj.SetReConnect(false);		// �趨���ߵ�ʱ�򲻻���������
			g_ccNetObj.SetShowIP(true);			// �趨����ʱ����ʾIP
			
			Sleep(100);
			g_state = 0;
			
			int n_login=0,n_send=0,login_num=0,send_num=0;
			operPal.GetLogSendNum(&login_num,&send_num);
			
			char szAccount[32];
			char szPassword[32];
			int iGMServerPort=0;
			int iWorldID=0;

			ZeroMemory(szAccount,32);
			ZeroMemory(szPassword,32);

			while(g_state != -1)
			{
				Sleep(10);
				g_ccNetObj.Flush();
				
				// ��״ִ̬��
				switch(g_state)
				{
					// ����GMServer
				case 0:	
					{
						// �趨GMS���߲���
						operPal.GetUserNamePwd("User4",szAccount,szPassword,&iGMServerPort);
						
						// ����GMS���߳ɹ�
						if(g_ccNetObj.WaitConnect(szGMServerIP, iGMServerPort, 65535, 65535))
						{		
							// ȡ�ñ���IP
							struct in_addr localaddr;   
							struct hostent *hInfo = NULL;   
							char szHostname[50];     
							gethostname(szHostname,49);							// ��������   
							hInfo = gethostbyname(szHostname);					// ������Ѷ   
							memcpy(&localaddr, hInfo->h_addr, hInfo->h_length);	// ����IP   
							
							// �趨GMS�������
							//char szAccount[32] = "silent2";			// �����ʺ�
							//char szPassword[32] = "silent2";		// ��������		
							char *szIP = inet_ntoa(localaddr);		// ����IP	
							
							PGGMCConnection_CtoS_RequestLogin sPkt;	
							sprintf(sPkt.szAccount, "%s", szAccount);
							sprintf(sPkt.szPassword, "%s", szPassword);		
							memcpy(sPkt.szIP, szIP, sizeof(sPkt.szIP));  
							
							// �������ܵ���GMS
							g_ccNetObj.Send(sizeof(sPkt), (S_ObjNetPktBase*) &sPkt);
							g_state = 1;
						}
						else
						{
							g_state=-1;
							m_tflv.nIndex=DBVect.size()+1;
							m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
							m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
							m_tflv.m_tvlength=strlen("���Ӵ���");
							sprintf((LPSTR)&m_tflv.lpdata,"%s","���Ӵ���");
							DBVect.push_back(m_tflv);		
						}
					}
					
					break;
					// �ȴ�������
				case 1:
					n_login++;
					Sleep(100);
					if(n_login>login_num)
					{
						g_state=-1;
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen("���볬ʱ");
						sprintf((LPSTR)&m_tflv.lpdata,"%s","���볬ʱ");
						DBVect.push_back(m_tflv);
					}
					printf("������������,��%i��\n",n_login);
					break;
					// ����ɹ�
				case 2:
					{

						printf("����ɹ�\n\n");
						
						// �趨�������
					//	int  iWorldID		= 0;		// ������
					//	char szRoleName[32] = "Kitty";	// ��ɫ����
					//	char szBankPw[7]	= "654321";// ����������(6λӢ����)


						iWorldID=operPal.FindWorldID(szGMServerName,szGMServerIP);	

						
						PGPlayerControl_CtoS_ModifyAccountBankPw sPkt;
						sPkt.iWorldID	= iWorldID;
						sprintf(sPkt.szAccount, "%s", Account);
						sprintf(sPkt.szBankPw, "%s", szBankPw);
						
						// ������GMSҪ���޸Ľ�ɫ��������
						g_ccNetObj.Send(sizeof(sPkt), (S_ObjNetPktBase*)&sPkt);	
						g_state = 3;	

					}
					break;
				// �ȴ��޸Ĺ���ȼ�
				case 3:
					n_send++;
					Sleep(100);
					if(n_send>send_num)
					{
						g_state=-1;
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen("�ȴ���ʱ");
						sprintf((LPSTR)&m_tflv.lpdata,"%s","�ȴ���ʱ");
						DBVect.push_back(m_tflv);
					}
					printf("�ȴ��޸Ĺ���ȼ���,��%i��\n",n_send);
					break;
				}
			}			
			g_ccNetObj.Disconnect();
			printf("\n");
			printf("\n==================== Shutdown ====================\n");
			return DBVect;
}

void CModifyAccountBankPw::LoginResult(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
{
	char bufstr[256];
	PGGMCConnection_StoC_LoginResult *pPkt = (PGGMCConnection_StoC_LoginResult *)pData;
	switch(pPkt->emResult)
	{
	case ENUM_GMCLoginResult_Success:
		printf("��½�ɹ�\n");
		g_state = 2;
		return;		
	case ENUM_GMCLoginResult_AccountFailed:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"��½ʧ��,�ʺŴ���\n");
		printf(bufstr,"��½ʧ��,�ʺŴ���\n");
		
		break;		
	case ENUM_GMCLoginResult_PasswordFailed:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"��½ʧ��,�������\n");
		printf("��½ʧ��,�������\n");
		break;		
	case ENUM_GMCLoginResult_IPFailed:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"��½ʧ��,IP��ַ����\n");
		printf("��½ʧ��,IP��ַ����\n");
		break;		
	default:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"��½ʧ��\n");
		printf(bufstr,"��½ʧ��\n");	
		break;
	}
	CGlobalStruct::TFLV m_tflv;
	m_tflv.nIndex=DBVect.size()+1;
	m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
	m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
	m_tflv.m_tvlength=strlen(bufstr);
	sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
	DBVect.push_back(m_tflv);
	g_state = -1;

}
// �޸��ʺ�����������-------------------------------------------------------------------------------
void CModifyAccountBankPw::ModifyAccountBankPw(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
{

		PGPlayerControl_StoC_ModifyAccountBankPw *pPkt = (PGPlayerControl_StoC_ModifyAccountBankPw *)pData;
		char bufstr[256];
		ZeroMemory(bufstr,256);
		switch(pPkt->emResult)
		{
		case ENUM_ModifyAccountBankPwResult_Success:	
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"�޸��ʺ���������ɹ�,������:%d,�ʺ�:%s\n",pPkt->iWorldID, pPkt->szAccount);
			printf("ModifyAccountBankPw succsee\n");
			printf("WorldID: %d\n"	, pPkt->iWorldID);
			printf("Account: %s\n"	, pPkt->szAccount);

			break;
		case ENUM_ModifyAccountBankPwResult_WorldNotFound:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"�޸��ʺ�����������: ����û���ҵ�\n");
			printf("ModifyAccountBankPw result: World not found\n");
			break;	
		case ENUM_ModifyAccountBankPwResult_LeaderNotFound:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"�޸��ʺ�����������: Ƶ��û���ҵ�\n");
			printf("ModifyAccountBankPw result: Leader not found\n");
			break;
		case ENUM_ModifyAccountBankPwResult_LeaderDisconnect:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"�޸��ʺ�����������: Ƶ��û������\n");
			printf("ModifyAccountBankPw result: Leader disconnect\n");
			break;
		case ENUM_ModifyAccountBankPwResult_PasswordError:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"�޸��ʺ�����������: �����ʽ����\n");
			printf("ModifyCharacterDropPw result: Password format error\n");
			break;
		case ENUM_ModifyAccountBankPwResult_RoleNotFound:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"�޸��ʺ�����������: �ʺ� %s û���ҵ�\n", pPkt->szAccount);
			printf("ModifyAccountBankPw result: Account %s not found\n", pPkt->szAccount);
			break;	
		case ENUM_ModifyAccountBankPwResult_RoleIsOnline:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"�޸��ʺ�����������: �ʺ� %s ������\n", pPkt->szAccount);
			printf("ModifyAccountBankPw result: Account %s is online now\n", pPkt->szAccount);
			break;	
		case ENUM_ModifyAccountBankPwResult_GMOOperationFailed:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"�޸��ʺ�����������: ��Ϸ����������ʧ��\n");
			printf("ModifyAccountBankPw result: Operation failed from GMObserver\n");
			break;	
		default:
			break;
		}
		CGlobalStruct::TFLV m_tflv;
		m_tflv.nIndex=DBVect.size()+1;
		m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
		m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
		m_tflv.m_tvlength=strlen(bufstr);
		sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
		DBVect.push_back(m_tflv);
		g_state = -1;


}

bool CModifyAccountBankPw::SaveAccountBankPw(char * szRoleAccount,char* szOldPasswd,char * szNewPasswd,char * lpGMServerName)//����ԭ���ʺ���������
{


	char szSql[256];

	bool flag=true;
	
	sprintf(szSql,"exec Pal_IsAccountBankPw '%s'",szRoleAccount);
	
	vector<CGlobalStruct::DataRecord> datarecord;
	if(!datarecord.empty())
	{
		datarecord.clear();
	}

	m_SqlHelper.SqlHelperMain(&datarecord, "127.0.0.1", szSql, 4, 1, 1);//��ȡSqlExpress�е���Ӧsql���
	
	if(datarecord.empty())//�����¼Ϊ��
	{
		sprintf(szSql,"exec Pal_InsertBankPw '%s','%s','%s'",szRoleAccount,szOldPasswd,szNewPasswd);
		m_SqlHelper.SqlHelperMain("127.0.0.1",  szSql, 4);//��ȡ��Ӧ��LogDBIP
		return true;
	} 
	else
	{	
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
	//	delete datarecord;
	//	datarecord=NULL;
		return false;
	}
	
	
	return true;
}
bool CModifyAccountBankPw::FindLocalAccountBankPw(char * szAccount)
{
	
	char szSql[256];
	
	bool flag=true;
	
	sprintf(szSql,"exec Pal_IsAccountBankPw '%s'",szAccount);
	vector<CGlobalStruct::DataRecord> datarecord;
	if(!datarecord.empty())
	{
		datarecord.clear();
	}
	flag=m_SqlHelper.SqlHelperMain(&datarecord, "127.0.0.1", szSql, 4, 1, 1);//��ȡSqlExpress�е���Ӧsql���
	
	if(datarecord.empty())//�����¼Ϊ��
	{
		return false;
	} 
	else
	{
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
		return true;
	}

		
	return true;
}

void CModifyAccountBankPw::GetBankPw(char * szAccount,char * szPasswd,char* lpGMServerName)
{
	try
	{
		char logdbip[20];
		char szSql[2048];
		char szRemoteSql[2048];
		ZeroMemory(logdbip,20);
		ZeroMemory(szSql,2048);
		ZeroMemory(szRemoteSql,2048);
		operPal.FindGameDBIP(logdbip,lpGMServerName);
		operPal.getRemoteSql("PAL","PAL_GETBANKPW",szRemoteSql,0);
		sprintf(szSql,szRemoteSql,szAccount);
	//	vector<CGlobalStruct::DataRecord>* datarecord = new vector<CGlobalStruct::DataRecord>;
		vector<CGlobalStruct::DataRecord> datarecord;
		m_SqlHelper.SqlHelperMain("PAL",&datarecord, logdbip, szSql, 1, 1, 1);//��ȡSqlExpress�е���Ӧsql���
		
		if(datarecord.empty())//�����¼Ϊ��
		{
			sprintf(szPasswd,"");
			return;
		}
		
		//��ȡ��ѯ�ļ�¼����ÿһ��
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;	
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("bankpassword", m_DataRecord.recordfieldname))//��ȡSqlExpress�ж�Ӧsql_type��sql_statement
			{
				sprintf(szPasswd,m_DataRecord.recorddata);
			}
		}
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
	//	delete datarecord;
	//	datarecord=NULL;	
	}
	catch(...)
	{
		sprintf(szPasswd,"");
	}
	

}


bool CModifyAccountBankPw::RecallAccountBankPw(char* szRoleAccount,char * szBankPw)
{

	bool flag=true;
	try
	{
		
		char szSql[256];
		sprintf(szSql,"exec Pal_FindOldBankPw '%s'",szRoleAccount);
	//	vector<CGlobalStruct::DataRecord>* datarecord = new vector<CGlobalStruct::DataRecord>;
		vector<CGlobalStruct::DataRecord> datarecord;
		m_SqlHelper.SqlHelperMain(&datarecord, "127.0.0.1", szSql, 4, 1, 1);//��ȡSqlExpress�е���Ӧsql���
		
		if(datarecord.empty())//�����¼Ϊ��
		{
			sprintf(szBankPw,"");
			return false;
		}
		
		//��ȡ��ѯ�ļ�¼����ÿһ��
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;	
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("oldbankpasswd", m_DataRecord.recordfieldname))//��ȡSqlExpress�ж�Ӧsql_type��sql_statement
			{
				sprintf(szBankPw,m_DataRecord.recorddata);
			}
		}
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
		//delete datarecord;
	//	datarecord=NULL;
		sprintf(szSql,"exec Pal_UpdateBankPwFlag '%s'",szRoleAccount);
		
		flag=m_SqlHelper.SqlHelperMain("127.0.0.1", szSql, 4);//��ȡSqlExpress�е���Ӧsql���
		
	}
	catch(...)
	{
		
	}
	return flag;
}
