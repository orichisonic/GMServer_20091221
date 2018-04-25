// CharacterFriendList.cpp: implementation of the CCharacterFriendList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CharacterFriendList.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCharacterFriendList::CCharacterFriendList()
{

}

CCharacterFriendList::~CCharacterFriendList()
{

}

vector <CGlobalStruct::TFLV> CCharacterFriendList::CharacterFriendListMain(char *szGMServerName, char * szGMServerIP,char* szRoleName)
{	


			char strlog[80];
			sprintf(strlog,"����<%s>��ɫ<%s>��ѯ��ɫ�����б�",szGMServerName,szRoleName);
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
			g_ccNetObj.RegEvent_OnPacket(ENUM_PGPlayerStatus_StoC_CharacterFriendList, CharacterFriendList, 0);

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
						operPal.GetUserNamePwd("User2",szAccount,szPassword,&iGMServerPort);

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
		

						PGPlayerStatus_CtoS_CharacterFriendList sPkt;
						sPkt.iWorldID = iWorldID;
						sprintf(sPkt.szRoleName, "%s", szRoleName);

						// ������GMSҪ��ȡ�ý�ɫ�����б�
						g_ccNetObj.Send(sizeof(sPkt), (S_ObjNetPktBase*)&sPkt);	
						g_state = 3;			
					}
					break;
					// �ȴ�ȡ�ý�ɫ�����б�
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
					printf("�ȴ�ȡ�ý�ɫ������Ѷ��,��%i��\n",n_send);
					break;
				}
			}			
			g_ccNetObj.Disconnect();
			printf("\n");
			printf("\n==================== Shutdown ====================\n");
			return DBVect;


}
void CCharacterFriendList::LoginResult(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
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


// ȡ�ý�ɫ�����б���-------------------------------------------------------------------------------
void CCharacterFriendList::CharacterFriendList(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
{

		PGPlayerStatus_StoC_CharacterFriendList *pPkt = (PGPlayerStatus_StoC_CharacterFriendList *)pData;
		CGlobalStruct::TFLV m_tflv;
		char strInt[10];
		char bufstr[256];
		switch(pPkt->emResult)
		{
		case ENUM_CharacterFriendListResult_Success:
			{
				// ��Ϊ�������
				if (pPkt->iWorldID == -1)
				{
					g_state = -1;
					return;
				}

				// �趨��ɫ��ϵ����
				char szRelation[32];
				switch (pPkt->emRelation)
				{
				case ENUM_RelationshipPairType_Recognition:
					sprintf(szRelation, "��ʶ");
					break;
				case ENUM_RelationshipPairType_FamilyMembers:
					sprintf(szRelation, "����");
					break;
				case ENUM_RelationshipPairType_Confidant:
					sprintf(szRelation, "֪��");
					break;
				case ENUM_RelationshipPairType_SwornBrothers:
					sprintf(szRelation, "����");
					break;
				case ENUM_RelationshipPairType_FightingBrother:
					sprintf(szRelation, "ս��");
					break;
				case ENUM_RelationshipPairType_MasterApprentice:
					sprintf(szRelation, "ʦͽ");
					break;
				case ENUM_RelationshipPairType_Lover:
					sprintf(szRelation, "����");
					break;
				case ENUM_RelationshipPairType_Couple:
					sprintf(szRelation, "����");
					break;
				default:
					sprintf(szRelation, "");
					break;
				}

				// ������
				printf("\n====================================================\n");
				printf("World: %d\n",			pPkt->iWorldID);
				printf("Account: %s\n",			pPkt->szAccount);
				printf("RoleName: %s\n",		pPkt->szRoleName);

				printf("\n[ ��ɫ������Ѷ ]\n");
				printf("��ɫ����: %s\n",		pPkt->szFriendName);
				printf("��ϵ: %s\n",			szRelation);


				_itoa(pPkt->iWorldID,strInt,10);
				m_tflv.nIndex=DBVect.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::PAL_WORLDID;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
				m_tflv.m_tvlength=sizeof(int);
				sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
				DBVect.push_back(m_tflv);


				m_tflv.nIndex=DBVect.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::PAL_ACCOUNT;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(pPkt->szAccount);
				sprintf((LPSTR)&m_tflv.lpdata,"%s",pPkt->szAccount);
				DBVect.push_back(m_tflv);

				m_tflv.nIndex=DBVect.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::PAL_ROLENAME;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(pPkt->szRoleName);
				sprintf((LPSTR)&m_tflv.lpdata,"%s",pPkt->szRoleName);
				DBVect.push_back(m_tflv);

				m_tflv.nIndex=DBVect.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::PAL_FRIENDNAME;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(pPkt->szFriendName);
				sprintf((LPSTR)&m_tflv.lpdata,"%s",pPkt->szFriendName);
				DBVect.push_back(m_tflv);

				m_tflv.nIndex=DBVect.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::PAL_RELATION;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(szRelation);
				sprintf((LPSTR)&m_tflv.lpdata,"%s",szRelation);
				DBVect.push_back(m_tflv);
			}
			break;

		case ENUM_CharacterFriendListResult_RoleNotFound:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"��ɫ�����б���: Character %s not found\n", pPkt->szRoleName);
			printf("Charfriendlist result: Character %s not found\n", pPkt->szRoleName);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(bufstr);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
			DBVect.push_back(m_tflv);
				g_state = -1;
			break;

		case ENUM_CharacterFriendListResult_CharacterFriendListEmpty:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"��ɫ�����б���: ��ɫ %s û������\n", pPkt->szRoleName);
			printf("Charfriendlist result: Character %s has no friend\n", pPkt->szRoleName);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(bufstr);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
			DBVect.push_back(m_tflv);
				g_state = -1;
			break;

		case ENUM_CharacterFriendListResult_WorldNotFound:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"��ɫ�����б���: ����û���ҵ�\n");
			printf("Charfriendlist result: World not found\n");
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(bufstr);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
			DBVect.push_back(m_tflv);
				g_state = -1;
			break;

		case ENUM_CharacterFriendListResult_LeaderNotFound:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"��ɫ�����б���: Ƶ��û���ҵ�\n");
			printf("Charfriendlist result: Leader not found\n");
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(bufstr);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
			DBVect.push_back(m_tflv);
				g_state = -1;
			break;

		case ENUM_CharacterFriendListResult_LeaderDisconnect:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"��ɫ�����б���: Ƶ��û������\n");
			printf("Charfriendlist result: Leader disconnect\n");
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(bufstr);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
			DBVect.push_back(m_tflv);
				g_state = -1;
			break;

		default:
				g_state = -1;
			break;
	}	

}
