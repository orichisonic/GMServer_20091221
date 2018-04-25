// GuildDropMember.cpp: implementation of the CCharacterInfo class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "GuildDropMember.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CGuildDropMember::CGuildDropMember()
{
	
}

CGuildDropMember::~CGuildDropMember()
{
	
}

vector <CGlobalStruct::TFLV>  CGuildDropMember::GuildDropMemberMain(char *szGMServerName, char * szGMServerIP,char* szGuildName,char* szRoleName)
{
	char strlog[80];
	sprintf(strlog,"����<%s>���<%s>������Ա<%s>",szGMServerName,szGuildName,szRoleName);
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
	g_ccNetObj.RegEvent_OnPacket(ENUM_PGPlayerControl_StoC_GuildDropMember, GuildDropMember, 0);
	
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
			{
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
			}
			break;
			// ����ɹ�
		case 2:
			{
				printf("����ɹ�\n\n");
				
				// �趨�������
				iWorldID=operPal.FindWorldID(szGMServerName,szGMServerIP);
				
				PGPlayerControl_CtoS_GuildDropMember sPkt;
				sPkt.iWorldID = iWorldID;
				sprintf(sPkt.szGuildName, "%s", szGuildName);
				sprintf(sPkt.szRoleName, "%s", szRoleName);
				
				// ������GMSҪ�󿪳������Ա
				g_ccNetObj.Send(sizeof(sPkt), (S_ObjNetPktBase*)&sPkt);	
				g_state = 3;			
			}
			break;
			// �ȴ����������Ա
		case 3:
			{
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
				printf("�ȴ����������Ա��,��%i��\n",n_send);
			}
			break;
		}
	}			
	printf("\n");
	printf("\n==================== Shutdown ====================\n");
	return DBVect;
}

void CGuildDropMember::LoginResult(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
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
		{
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"��½ʧ��,�ʺŴ���\n");
			printf("��½ʧ��,�ʺŴ���\n");
		}
		break;		
	case ENUM_GMCLoginResult_PasswordFailed:
		{
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"��½ʧ��,�������\n");
			printf("��½ʧ��,�������\n");
		}
		break;		
	case ENUM_GMCLoginResult_IPFailed:
		{
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"��½ʧ��,IP��ַ����\n");
			printf("��½ʧ��,IP��ַ����\n");
		}
		break;		
	default:
		{
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"��½ʧ��\n");
			printf("��½ʧ��\n");
		}
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

void CGuildDropMember::GuildDropMember(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
{
	PGPlayerControl_StoC_GuildDropMember *pPkt = (PGPlayerControl_StoC_GuildDropMember *)pData;
	
	char bufstr[256];
	ZeroMemory(bufstr,256);
	switch(pPkt->emResult)
	{
	case ENUM_GuildDropMemberResult_Success:		
		{
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"��������Ա�ɹ�, ������: %d, ������: %s, ��ɫ��:\n", pPkt->iWorldID, pPkt->szGuildName, pPkt->szRoleName);
			printf("GuildDropMember succsee, world id: %d, guild name: %s, role name: %s\n", 
			pPkt->iWorldID, pPkt->szGuildName, pPkt->szRoleName);
		}
		break;
		
	case ENUM_GuildDropMemberResult_WorldNotFound:
		{
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"��������Ա���: ����û���ҵ�\n");
			printf("GuildDropMember result: World not found\n");
		}
		break;	
		
	case ENUM_GuildDropMemberResult_LeaderNotFound:
		{
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"��������Ա���: Ƶ��û���ҵ�\n");
			printf("GuildDropMember result: Leader not found\n");
		}
		break;
		
	case ENUM_GuildDropMemberResult_LeaderDisconnect:
		{
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"��������Ա���: Ƶ��û������\n");
			printf("GuildDropMember result: Leader disconnect\n");
		}
		break;
		
	case ENUM_GuildDropMemberResult_GuildNotFound:
		{
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"��������Ա���: ���%s û���ҵ�\n", pPkt->szGuildName);
			printf("GuildDropMember result: Guild %s not found\n", pPkt->szGuildName);
		}
		break;	
		
	case ENUM_GuildDropMemberResult_RoleNotFound:
		{
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"��������Ա���: ��ɫ%s û���ҵ�\n", pPkt->szRoleName);
			printf("GuildDropMember result: Character %s not found\n", pPkt->szRoleName);
		}
		break;	
		
	case ENUM_GuildDropMemberResult_LastMember:
		{
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"��������Ա���: ���ܽ�����һ����ɫ\n");
			printf("GuildDropMember result: Operation failed, last member can't be dismissed\n");
		}
		break;	
		
	case ENUM_GuildDropMemberResult_GMOOperationFailed:
		{
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"��������Ա���:��Ϸ����������ʧ��\n");
			printf("GuildDropMember result: Operation failed from GMObserver\n");
		}
		break;	
		
	case ENUM_GuildDropMemberResult_ChatOperationFailed:
		{
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"��������Ա���:����������ʧ��\n");
			printf("GuildDropMember result: Operation failed from chatserver\n");
		}
		break;	
		
	default:
		break;
	}//switch
	
	g_state = -1;
}