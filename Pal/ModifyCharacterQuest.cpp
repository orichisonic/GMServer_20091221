// ModifyCharacterQuest.cpp: implementation of the CModifyCharacterQuest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ModifyCharacterQuest.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CModifyCharacterQuest::CModifyCharacterQuest()
{

}

CModifyCharacterQuest::~CModifyCharacterQuest()
{

}
vector <CGlobalStruct::TFLV> CModifyCharacterQuest::ModifyCharacterQuestMain(int userByID,char *szGMServerName, char * szGMServerIP,char* szRoleName,int iQuestID,int iQuestStatus)
{


		char strlog[80];
		ZeroMemory(strlog,80);
		sprintf(strlog,"����<%s>�޸Ľ�ɫ<%s>����",szGMServerName,szRoleName);
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
		g_ccNetObj.RegEvent_OnPacket(ENUM_PGPlayerControl_StoC_ModifyCharacterQuest, ModifyCharacterQuest, 0);
		
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
					operPal.GetUserNamePwd("User3",szAccount,szPassword,&iGMServerPort);

					
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
					//	char szAccount[32] = "silent2";			// �����ʺ�
					//	char szPassword[32] = "silent2";		// ��������		
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
					//int  iWorldID		= 0;		// ������
					//char szRoleName[32] = "Kitty";	// ��ɫ����
					//	int  iQuestID		= 1702;		// ������(0~6143)
					//	int  iQuestStatus	= 2;		// ����״̬(0:δ�� 1:����� 2:�ѽ�)

					iWorldID=operPal.FindWorldID(szGMServerName,szGMServerIP);

					
					PGPlayerControl_CtoS_ModifyCharacterQuest sPkt;
					sPkt.iWorldID		= iWorldID;
					sprintf(sPkt.szRoleName, "%s", szRoleName);
					sPkt.iQuestID		= iQuestID;
					sPkt.iQuestStatus	= iQuestStatus;
					
					// ������GMSҪ���޸Ľ�ɫ����
					g_ccNetObj.Send(sizeof(sPkt), (S_ObjNetPktBase*)&sPkt);	
					g_state = 3;			
				}
				break;
				// �ȴ��޸Ľ�ɫ����
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
				printf("�ȴ��޸Ľ�ɫ������,��%i��\n",n_send);
				break;
			}
		}			
		g_ccNetObj.Disconnect();
		printf("\n");
		printf("\n==================== Shutdown ====================\n");
		

		char text[256];
		char queststatus[256];
		ZeroMemory(text,256);
		ZeroMemory(queststatus,256);
		switch(iQuestStatus)
		{
		case 0:
			sprintf(queststatus,"δ��");
			break;
		case 1:
			sprintf(queststatus,"�����");
			break;
		case 2:
			sprintf(queststatus,"�ѽ�");
			break;
		default:
			break;
		}
		ZeroMemory(text,256);
		sprintf(text,"������:%s,����״̬:%s",Pal_GetQuestName(iQuestID),queststatus);	
		operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYGUILDNAME,szGMServerName,&(COperVector(DBVect)),text);
		return DBVect;
}

void CModifyCharacterQuest::LoginResult(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
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
// �޸Ľ�ɫ������-----------------------------------------------------------------------------------
void CModifyCharacterQuest::ModifyCharacterQuest(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
{

		PGPlayerControl_StoC_ModifyCharacterQuest *pPkt = (PGPlayerControl_StoC_ModifyCharacterQuest *)pData;
		char strInt[10];
		char bufstr[256];
		ZeroMemory(strInt,10);
		ZeroMemory(bufstr,256);
		
		switch(pPkt->emResult)
		{
		case ENUM_ModifyCharacterQuestResult_Success:		
			printf("ModifyCharacterQuest succsee\n");
			printf("WorldID: %d\n"		, pPkt->iWorldID);
			printf("RoleName: %s\n"		, pPkt->szRoleName);
			printf("QuestID: %d\n"		, pPkt->iQuestID);
			printf("QuestStatus: %d\n"	, pPkt->iQuestStatus);
			
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"�޸Ľ�ɫ����ɹ�");

			/*_itoa(pPkt->iWorldID,strInt,10);
			pOperVector->AddTFLV(CEnumCore::TagName::PAL_WORLDID,CEnumCore::TagFormat::TLV_INTEGER,sizeof(int),(unsigned char *)&strInt);
			pOperVector->AddTFLV(CEnumCore::TagName::PAL_ROLENAME,CEnumCore::TagFormat::TLV_STRING,strlen(pPkt->szRoleName),(unsigned char *)pPkt->szRoleName);
			_itoa(pPkt->iQuestID,strInt,10);
			pOperVector->AddTFLV(CEnumCore::TagName::PAL_QUESTID,CEnumCore::TagFormat::TLV_INTEGER,sizeof(int),(unsigned char *)&strInt);
			_itoa(pPkt->iQuestStatus,strInt,10);
			pOperVector->AddTFLV(CEnumCore::TagName::PAL_QUESTTYPE,CEnumCore::TagFormat::TLV_INTEGER,sizeof(int),(unsigned char *)&strInt);
*/
			break;
		case ENUM_ModifyCharacterQuestResult_WorldNotFound:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"�޸Ľ�ɫ������: ����û���ҵ�\n");
			printf("ModifyCharacterQuest result: World not found\n");
			break;	
		case ENUM_ModifyCharacterQuestResult_LeaderNotFound:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"�޸Ľ�ɫ������: Ƶ��û���ҵ�\n");
			printf("ModifyCharacterQuest result: Leader not found\n");
			break;
		case ENUM_ModifyCharacterQuestResult_LeaderDisconnect:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"�޸Ľ�ɫ������: Ƶ��û������\n");
			printf("ModifyCharacterQuest result: Leader disconnect\n");
			break;
		case ENUM_ModifyCharacterQuestResult_ArgumentError:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"�޸Ľ�ɫ������: ��������\n");
			printf("ModifyCharacterQuest result: Argument error\n");
			break;	
		case ENUM_ModifyCharacterQuestResult_RoleNotFound:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"�޸Ľ�ɫ������:��ɫ %s û���ҵ�\n", pPkt->szRoleName);
			printf("ModifyCharacterQuest result: Character %s not found\n", pPkt->szRoleName);
			break;	
		case ENUM_ModifyCharacterQuestResult_RoleIsOnline:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"�޸Ľ�ɫ������:��ɫ %s ����\n", pPkt->szRoleName);
			printf("ModifyCharacterQuest result: Character %s is online now\n", pPkt->szRoleName);
			break;	
		case ENUM_ModifyCharacterQuestResult_GMOOperationFailed:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"�޸Ľ�ɫ������: CMObserver����ʧ��\n");
			printf("ModifyCharacterQuest result: Operation failed from GMObserver\n");
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
