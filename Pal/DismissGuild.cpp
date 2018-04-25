// DismissGuild.cpp: implementation of the CDismissGuild class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DismissGuild.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDismissGuild::CDismissGuild()
{

}

CDismissGuild::~CDismissGuild()
{

}
vector <CGlobalStruct::TFLV> CDismissGuild::DismissGuildMain(char *szGMServerName, char * szGMServerIP,char* szGuildName)
{


		char strlog[80];
		sprintf(strlog,"����<%s>��ɢ���<%s>",szGMServerName,szGuildName);
		logFile.WriteText("�ɽ�OL",strlog);

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
		g_ccNetObj.RegEvent_OnPacket(ENUM_PGPlayerControl_StoC_DismissGuild, DismissGuild, 0);
		
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
		ZeroMemory(szGMServerIP,16);
		ZeroMemory(szGuildName,32);

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
					iWorldID=operPal.FindWorldID(szGMServerName,szGMServerIP);					

					PGPlayerControl_CtoS_DismissGuild sPkt;
					sPkt.iWorldID = iWorldID;
					sprintf(sPkt.szGuildName, "%s", szGuildName);
					
					// ������GMSҪ���ɢ����
					g_ccNetObj.Send(sizeof(sPkt), (S_ObjNetPktBase*)&sPkt);	
					g_state = 3;			
				}
				break;
				// �ȴ���ɢ����
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
				printf("�ȴ���ɢ������,��%i��\n",n_send);
				break;
			}
		}			
		g_ccNetObj.Disconnect();
		printf("\n");
		printf("\n==================== Shutdown ====================\n");
		return DBVect;
}
void CDismissGuild::LoginResult(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
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

// ��ɢ������---------------------------------------------------------------------------------------
void CDismissGuild::DismissGuild(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
{

		PGPlayerControl_StoC_DismissGuild *pPkt = (PGPlayerControl_StoC_DismissGuild *)pData;
		char bufstr[256];
		ZeroMemory(bufstr,256);
		switch(pPkt->emResult)
		{
		case ENUM_DismissGuildResult_Success:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"��ɢ���ɹ�, ������: %d, �����: %s\n", pPkt->iWorldID, pPkt->szGuildName);
			printf("DismissGuild succsee, world id: %d, guild name: %s\n", pPkt->iWorldID, pPkt->szGuildName);
			break;
			
		case ENUM_DismissGuildResult_WorldNotFound:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"��ɢ�����: ����û���ҵ�\n");
			printf("DismissGuild result: World not found\n");
			break;	
			
		case ENUM_DismissGuildResult_LeaderNotFound:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"��ɢ�����: Ƶ��û���ҵ�\n");
			printf("DismissGuild result: Leader not found\n");
			break;
			
		case ENUM_DismissGuildResult_LeaderDisconnect:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"��ɢ�����: Ƶ��û������\n");
			printf("DismissGuild result: Leader disconnect\n");
			break;
			
		case ENUM_DismissGuildResult_GuildNotFound:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"��ɢ�����: ���%s û���ҵ�\n", pPkt->szGuildName);
			printf("DismissGuild result: Guild %s not found\n", pPkt->szGuildName);
			break;	
			
		case ENUM_DismissGuildResult_GMOOperationFailed:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"��ɢ�����:��Ϸ����������ʧ��\n");
			printf("DismissGuild result: Operation failed from GMObserver\n");
			break;	
			
		case ENUM_DismissGuildResult_SystemNotReady:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"��ɢ�����:������û��׼����\n");
			printf("DismissGuild result: System is not ready from chatserver\n");
			break;	
			
		case ENUM_DismissGuildResult_ChatOperationFailed:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"��ɢ�����:����������ʧ��\n");
			printf("DismissGuild result: Operation failed from chatserver\n");
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