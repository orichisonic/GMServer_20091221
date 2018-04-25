// BanishPlayer.cpp: implementation of the CBanishPlayer class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "BanishPlayer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBanishPlayer::CBanishPlayer()
{

}

CBanishPlayer::~CBanishPlayer()
{

}
vector <CGlobalStruct::TFLV> CBanishPlayer::BanishPlayerMain(char * szGMServerName,char * szGMServerIP,char * szRoleName)
{	
			int iIndex=0;
			int iPageSize=0;

			char strlog[80];
			sprintf(strlog,"����<%s>��ɫ<%s>����",szGMServerName,szRoleName);
			logFile.WriteText("�ɽ�OL",strlog);

			if(!DBVect.empty())
			{
				DBVect.clear();
			}
			CGlobalStruct::TFLV m_tflv;


			// ע������CallBack��ʽ
			g_ccNetObj.RegEvent_OnConnected(COperPal::OnConnect, 0);
			g_ccNetObj.RegEvent_OnDisconnected(COperPal::OnDisconnect, 0);
			g_ccNetObj.RegEvent_OnConnectFailed(COperPal::OnConnectfailed, 0);

			// ע���Ӧ�¼�CallBack��ʽ
			g_ccNetObj.RegEvent_OnPacket(ENUM_PGGMCConnection_StoC_LoginResult,	LoginResult, 0);
			g_ccNetObj.RegEvent_OnPacket(ENUM_PGPlayerControl_StoC_BanishPlayer, BanishPlayer, 0);

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

						operPal.GetUserNamePwd("User1",szAccount,szPassword,&iGMServerPort);
						

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
							m_tflv.m_tvlength=strlen("Connect Failed");
							sprintf((LPSTR)&m_tflv.lpdata,"%s","Connect Failed");
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
						m_tflv.m_tvlength=strlen("Login Failed");
						sprintf((LPSTR)&m_tflv.lpdata,"%s","Login Failed");
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
			
						PGPlayerControl_CtoS_BanishPlayer sPkt;
						sPkt.iWorldID = iWorldID;
						sprintf(sPkt.szRoleName, "%s", szRoleName);

						// ������GMSҪ����ĳ�������
						g_ccNetObj.Send(sizeof(sPkt), (S_ObjNetPktBase*)&sPkt);	
						g_state = 3;			
					}
					break;
					// �ȴ���ĳ�������
				case 3:
					n_send++;
					Sleep(100);
					if(n_send>send_num)
					{
						g_state=-1;
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen("Active Failed");
						sprintf((LPSTR)&m_tflv.lpdata,"%s","Active Failed");
						DBVect.push_back(m_tflv);
					}
					printf("�ȴ���ĳ���������,��%i��\n",n_send);
					break;
				}
			}			
			g_ccNetObj.Disconnect();
			printf("\n");
			printf("\n==================== Shutdown ====================\n");
			return DBVect;
	


}

void CBanishPlayer::LoginResult(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
{
		char bufstr[256];
		ZeroMemory(bufstr,256);
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
			printf("��½ʧ��\n");	
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

// ��ĳ������߽��-----------------------------------------------------------------------------------
void CBanishPlayer::BanishPlayer(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
{

		PGPlayerControl_StoC_BanishPlayer *pPkt = (PGPlayerControl_StoC_BanishPlayer *)pData;
		char bufstr[256];
		switch(pPkt->emResult)
		{
		case ENUM_BanishPlayerResult_Success:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"�߽�ɫ�� %s �ɹ�, �����: %d\n", pPkt->szRoleName, pPkt->iWorldID);
			printf("�߽�ɫ�� %s �ɹ�, �����: %d\n", pPkt->szRoleName, pPkt->iWorldID);
			break;
			
		case ENUM_BanishPlayerResult_RoleNotFound:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"���˽��: ��ɫ %s������\n", pPkt->szRoleName);
			printf("���˽��: ��ɫ %s������\n", pPkt->szRoleName);
			break;
			
		case ENUM_BanishPlayerResult_RoleNotOnline:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"���˽��:  ��ɫ %s ������\n", pPkt->szRoleName);
			printf("���˽��:  ��ɫ %s ������\n", pPkt->szRoleName);
			break;	
			
		case ENUM_BanishPlayerResult_WorldNotFound:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"���˽��: �����Ҳ���\n");
			printf("���˽��: �����Ҳ���\n");
			break;	
			
		case ENUM_BanishPlayerResult_LeaderNotFound:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"���˽��: Ƶ���Ҳ���\n");
			printf("���˽��: Ƶ���Ҳ���\n");
			break;
			
		case ENUM_BanishPlayerResult_LeaderDisconnect:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"���˽��: Ƶ���Ͽ�\n");
			printf("���˽��: Ƶ���Ͽ�\n");
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
