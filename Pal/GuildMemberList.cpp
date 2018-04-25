// GuildMemberList.cpp: implementation of the CGuildMemberList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GuildMemberList.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGuildMemberList::CGuildMemberList()
{

}

CGuildMemberList::~CGuildMemberList()
{

}
vector <CGlobalStruct::TFLV> CGuildMemberList::GuildMemberListMain(char *szGMServerName, char * szGMServerIP,char* szGuildName)
{

		int iIndex=0;
		int iPageSize=0;

		char strlog[80];
		sprintf(strlog,"����<%s>���<%s>��Ա�б�",szGMServerName,szGuildName);
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
		g_ccNetObj.RegEvent_OnPacket(ENUM_PGPlayerStatus_StoC_GuildMemberList, GuildMemberList, 0);
		
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
					//int  iWorldID = 0;							// ������
					//char szGuildName[32] = "�ˣɣ̡̣��ɣ�";	// ��������
					iWorldID=operPal.FindWorldID(szGMServerName,szGMServerIP);
							
					PGPlayerStatus_CtoS_GuildMemberList sPkt;
					sPkt.iWorldID = iWorldID;
					sprintf(sPkt.szGuildName, "%s", szGuildName);
					
					// ������GMSҪ��ȡ�ù����Ա�б�
					g_ccNetObj.Send(sizeof(sPkt), (S_ObjNetPktBase*)&sPkt);	
					g_state = 3;			
				}
				break;
				// �ȴ�ȡ�ù����Ա�б�
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
				printf("�ȴ�ȡ�ù����Ա�б���,��%i��\n",n_send);
				break;
			}
		}			
		g_ccNetObj.Disconnect();
		printf("\n");
		printf("\n==================== Shutdown ====================\n");
		return DBVect;

	
}
void CGuildMemberList::LoginResult(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
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
// ȡ�ý�ɫ������Ѷ���-------------------------------------------------------------------------------
void CGuildMemberList::GuildMemberList(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
{

			CGlobalStruct::TFLV m_tflv;
			PGPlayerStatus_StoC_GuildMemberList *pPkt = (PGPlayerStatus_StoC_GuildMemberList *)pData;
			char strInt[10];
			char bufstr[256];
			ZeroMemory(strInt,10);
			ZeroMemory(bufstr,256);
			switch(pPkt->emResult)
			{
			case ENUM_GuildMemberListResult_Success:
				{
					// ��Ϊ�������
					if (pPkt->iWorldID == -1)
					{
						g_state = -1;
						return;
					}
					
					time_t t = pPkt->iJoinDate;
					struct tm *local = localtime(&t);
					char nowtime[40];
					sprintf(nowtime,"%d-%d-%d %d:%d:%d",local->tm_year+1900,local->tm_mon+1,local->tm_mday,local->tm_hour,local->tm_min,local->tm_sec);
					
					// ������
					printf("\n====================================================\n");
					printf("World: %d\n",			pPkt->iWorldID);
					printf("GuildName: %s\n",		pPkt->szGuildName);
					printf("��Ա��ɫ����: %s\n",	pPkt->szRoleName);
					printf("ְ��: %d\n",			pPkt->iLevel);	
					printf("����ʱ��: %s\n",		asctime(local));
					
					
					_itoa(pPkt->iWorldID,strInt,10);
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_WORLDID;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);
					
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_GUILDNAME;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=strlen(pPkt->szGuildName);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",pPkt->szGuildName);
					DBVect.push_back(m_tflv);

					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_ROLENAME;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=strlen(pPkt->szRoleName);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",pPkt->szRoleName);
					DBVect.push_back(m_tflv);
					
					_itoa(pPkt->iLevel,strInt,10);
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_GUILDPOWERLEVEL;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);

					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_GUILDJOINTIME;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=strlen(nowtime);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",nowtime);
					DBVect.push_back(m_tflv);
					
				}
				break;
				
			case ENUM_GuildMemberListResult_GuildNotFound:
				ZeroMemory(bufstr,256);
				sprintf(bufstr,"����Ա�б���: ��� %s û���ҵ�\n", pPkt->szGuildName);
				printf("Guildmemberlist result: Guild %s not found\n", pPkt->szGuildName);
				m_tflv.nIndex=DBVect.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(bufstr);
				sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
				DBVect.push_back(m_tflv);
				g_state = -1;
				break;
				
			case ENUM_GuildMemberListResult_WorldNotFound:
				ZeroMemory(bufstr,256);
				sprintf(bufstr,"����Ա�б���:����û���ҵ�\n");
				printf("Guildmemberlist result: World not found\n");
				m_tflv.nIndex=DBVect.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(bufstr);
				sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
				DBVect.push_back(m_tflv);
				g_state = -1;
				break;
				
			case ENUM_GuildMemberListResult_LeaderNotFound:
				ZeroMemory(bufstr,256);
				sprintf(bufstr,"����Ա�б���:Ƶ��û���ҵ�\n");
				printf("Guildmemberlist result: Leader not found\n");
				m_tflv.nIndex=DBVect.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(bufstr);
				sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
				DBVect.push_back(m_tflv);
				g_state = -1;
				break;
				
			case ENUM_GuildMemberListResult_LeaderDisconnect:
				ZeroMemory(bufstr,256);
				sprintf(bufstr,"����Ա�б���:Ƶ��û������\n");
				printf("Guildmemberlist result: Leader disconnect\n");
				m_tflv.nIndex=DBVect.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(bufstr);
				sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
				DBVect.push_back(m_tflv);
				g_state = -1;
				break;
			default:
				break;
		}//switch	

}
