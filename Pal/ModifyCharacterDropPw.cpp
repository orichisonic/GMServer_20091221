// ModifyCharacterDropPw.cpp: implementation of the CModifyCharacterDropPw class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ModifyCharacterDropPw.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CModifyCharacterDropPw::CModifyCharacterDropPw()
{

}

CModifyCharacterDropPw::~CModifyCharacterDropPw()
{

}
vector <CGlobalStruct::TFLV> CModifyCharacterDropPw::ModifyCharacterDropPwMain(char *szGMServerName, char * szGMServerIP,char* szRoleName,char * szDropRolePw)
{
			/*char strlog[80];
			sprintf(strlog,"����<%s>�޸Ľ�ɫ<%s>ɾ������",szGMServerName,szRoleName);
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
			g_ccNetObj.RegEvent_OnPacket(ENUM_PGPlayerControl_StoC_ModifyCharacterDropPw, ModifyCharacterDropPw, 0);
			
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
						//int  iWorldID			= 0;				// ������
						//char szRoleName[32]		= "Kitty";			// ��ɫ����
						//char szDropRolePw[13]	= "123456";	// ��ɾ������(6~12Ӣ����)
						
						iWorldID=operPal.FindWorldID(szGMServerName,szGMServerIP);

						PGPlayerControl_CtoS_ModifyCharacterDropPw sPkt;
						sPkt.iWorldID	= iWorldID;
						sprintf(sPkt.szRoleName, "%s", szRoleName);
						sprintf(sPkt.szDropRolePw, "%s", szDropRolePw);
						
						// ������GMSҪ���޸Ľ�ɫɾ������
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

void CModifyCharacterDropPw::LoginResult(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
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
// �޸Ľ�ɫɾ��������-------------------------------------------------------------------------------
void CModifyCharacterDropPw::ModifyCharacterDropPw(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
{
	PGPlayerControl_StoC_ModifyCharacterDropPw *pPkt = (PGPlayerControl_StoC_ModifyCharacterDropPw *)pData;
	char bufstr[256];
	switch(pPkt->emResult)
	{
	case ENUM_ModifyCharacterDropPwResult_Success:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"�޸�ɾ����ɫ����ɹ�,������:%d,��ɫ��:%s\n",pPkt->iWorldID,pPkt->szRoleName);
		printf("ModifyCharacterDropPw succsee\n");
		printf("WorldID: %d\n"	, pPkt->iWorldID);
		printf("RoleName: %s\n"	, pPkt->szRoleName);
		break;
	case ENUM_ModifyCharacterDropPwResult_WorldNotFound:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"�޸�ɾ����ɫ������: ����û���ҵ�\n");	
		printf("ModifyCharacterDropPw result: World not found\n");
		break;	
	case ENUM_ModifyCharacterDropPwResult_LeaderNotFound:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"�޸�ɾ����ɫ������: Ƶ��û���ҵ�\n");	
		printf("ModifyCharacterDropPw result: Leader not found\n");
		break;
	case ENUM_ModifyCharacterDropPwResult_LeaderDisconnect:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"�޸�ɾ����ɫ������: Ƶ��û������\n");	
		printf("ModifyCharacterDropPw result: Leader disconnect\n");
		break;
	case ENUM_ModifyCharacterDropPwResult_PasswordError:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"�޸�ɾ����ɫ������: �����ʽ����\n");	
		printf("ModifyCharacterDropPw result: Password format error\n");
		break;
	case ENUM_ModifyCharacterDropPwResult_RoleNotFound:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"�޸�ɾ����ɫ������:  ��ɫ %s û���ҵ�\n", pPkt->szRoleName);
		printf("ModifyCharacterDropPw result: Character %s not found\n", pPkt->szRoleName);
		break;	
	case ENUM_ModifyCharacterDropPwResult_RoleIsOnline:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"�޸�ɾ����ɫ������:  ��ɫ %s ������\n", pPkt->szRoleName);
		printf("ModifyCharacterDropPw result: Character %s is online now\n", pPkt->szRoleName);
		break;	
	case ENUM_ModifyCharacterDropPwResult_GMOOperationFailed:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"�޸�ɾ����ɫ������:��Ϸ����������ʧ��\n");
		printf("ModifyCharacterDropPw result: Operation failed from GMObserver\n");
		break;	
	default:
		break;
	}//switch
	
	CGlobalStruct::TFLV m_tflv;
	m_tflv.nIndex=DBVect.size()+1;
	m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
	m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
	m_tflv.m_tvlength=strlen(bufstr);
	sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
	DBVect.push_back(m_tflv);
	g_state = -1;

}

bool CModifyCharacterDropPw::SaveCharacterDropPw(char * szRoleName,char * szOldPasswd,char * szNewPasswd,char * lpGMServerName)//����ԭ����ɫɾ������
{
	char szSql[256];
	
	bool flag=true;
	
	sprintf(szSql,"exec Pal_IsDropPw '%s'",szRoleName);
	vector<CGlobalStruct::DataRecord> datarecord;
	flag=m_SqlHelper.SqlHelperMain(&datarecord, "127.0.0.1", szSql, 4, 1, 1);//��ȡSqlExpress�е���Ӧsql���
	
	if(datarecord.empty())//�����¼Ϊ��
	{
		sprintf(szSql,"exec Pal_InsertDropPw '%s','%s','%s'",
				szRoleName,szOldPasswd,szNewPasswd);
		flag=m_SqlHelper.SqlHelperMain("127.0.0.1",  szSql, 4);//��ȡ��Ӧ��LogDBIP
		return true;
	} 
	else
	{
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
		//delete datarecord;
		//datarecord=NULL;
		return false;
	}
	
	return true;

	
}
bool CModifyCharacterDropPw::FindLocalCharacterDropPw(char * szRoleName)
{
	char szSql[256];
	
	bool flag=true;
	
	sprintf(szSql,"exec Pal_IsDropPw '%s'",szRoleName);
	vector<CGlobalStruct::DataRecord> datarecord;
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
	//	delete datarecord;
	//	datarecord=NULL;
		return true;
	}

	return true;
}

void CModifyCharacterDropPw::GetDropPw(char * szRoleName,char * szPasswd,char* lpGMServerName)
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
		operPal.getRemoteSql("PAL","PAL_GETDROPPW",szRemoteSql,0);
		sprintf(szSql,szRemoteSql,szRoleName);
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
			if(!strcmp("droprolepassword", m_DataRecord.recordfieldname))//��ȡSqlExpress�ж�Ӧsql_type��sql_statement
			{
				sprintf(szPasswd,m_DataRecord.recorddata);
			}
		}
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
//		delete datarecord;
//		datarecord=NULL;
		
	}
	catch(...)
	{
		sprintf(szPasswd,"");
	}



}


bool CModifyCharacterDropPw::RecallCharacterDropPw(char* szRoleName,char * szDropRolePw)
{
	bool flag=true;
	try
	{
		
		char szSql[256];
		sprintf(szSql,"exec Pal_FindOldDropPw '%s' ",szRoleName);
	//	vector<CGlobalStruct::DataRecord>* datarecord = new vector<CGlobalStruct::DataRecord>;
		vector<CGlobalStruct::DataRecord> datarecord;
		m_SqlHelper.SqlHelperMain(&datarecord, "127.0.0.1", szSql, 4, 1, 1);//��ȡSqlExpress�е���Ӧsql���
		if(datarecord.empty())//�����¼Ϊ��
		{
			sprintf(szDropRolePw,"");
			return false;
		}
		
		//��ȡ��ѯ�ļ�¼����ÿһ��
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;	
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("oldroledroppwd", m_DataRecord.recordfieldname))//��ȡSqlExpress�ж�Ӧsql_type��sql_statement
			{
				sprintf(szDropRolePw,m_DataRecord.recorddata);
			}
		}
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
	//	delete datarecord;
	//	datarecord=NULL;
		sprintf(szSql,"exec Pal_UpdateDropPwFlag '%s'",szRoleName);
		
		flag=m_SqlHelper.SqlHelperMain("127.0.0.1", szSql, 4);//��ȡSqlExpress�е���Ӧsql���
		
	}
	catch(...)
	{
		return false;	
	}
	return flag;



}
