// AccountInfo.cpp: implementation of the CAccountInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AccountInfo.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAccountInfo::CAccountInfo()
{

}

CAccountInfo::~CAccountInfo()
{

}
vector <CGlobalStruct::TFLV> CAccountInfo::AccountInfoMain(char *szGMServerName, char * szGMServerIP,char* szAccountName)
{
	
			char strlog[80];
			sprintf(strlog,"����<%s>�˺�<%s>ȡ���ʺ���Ѷ",szGMServerName,szAccountName);
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
			g_ccNetObj.RegEvent_OnPacket(ENUM_PGPlayerStatus_StoC_AccountInfo, AccountInfo, 0);

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
						// �趨GMS���߲���
						//char szGMServerIP[16] = "60.206.15.78";	   // GMServer IPλַ
						//	int  iGMServerPort = 9991;					// GMServer Port

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
							m_tflv.m_tvlength=strlen("����ʧ��");
							sprintf((LPSTR)&m_tflv.lpdata,"%s","����ʧ��");
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

						PGPlayerStatus_CtoS_AccountInfo sPkt;
						sPkt.iWorldID = iWorldID;
						sprintf(sPkt.szAccountName, "%s", szAccountName);

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
						m_tflv.m_tvlength=strlen("�ȴ���ʱ");
						sprintf((LPSTR)&m_tflv.lpdata,"%s","�ȴ���ʱ");
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
void CAccountInfo::LoginResult(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
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
void CAccountInfo::AccountInfo(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
{

		PGPlayerStatus_StoC_AccountInfo *pPkt = (PGPlayerStatus_StoC_AccountInfo *)pData;
		CGlobalStruct::TFLV m_tflv;
		char strInt[10];
		char bufstr[256];
		switch(pPkt->emResult)
		{
			case ENUM_AccountInfoResult_Success:
				{
					// ������
					printf("\n====================================================\n");
					printf("World: %d\n",		pPkt->iWorldID);
					printf("AccountID: %d\n",	pPkt->iAccountID);
					printf("AccountName: %s\n", pPkt->szAccountName);
					printf("MaxRoleCount: %d\n",pPkt->iMaxRoleCount);
				
					printf("\n��ɫ�б�:\n");
					printf("[ ��ɫID ] [   ��ɫ����   ] [   ����ʱ��   ]\n");
					int i=0;
					for (i=0; i<30; i++)
					{
						// �ж��Ƿ��ѵ����һ������
						if (pPkt->sGMAccountRoleInfo[i].iDBID == -1) break;
							printf("%10d %16s %s\n", pPkt->sGMAccountRoleInfo[i].iDBID, pPkt->sGMAccountRoleInfo[i].szRoleName, 
								pPkt->sGMAccountRoleInfo[i].szCreateTime);
					}
					for (i=0; i<30; i++)
					{
						// �ж��Ƿ��ѵ����һ������
						if (pPkt->sGMAccountRoleInfo[i].iDBID == -1) break;
						{
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
							m_tflv.m_tvlength=strlen(pPkt->szAccountName);
							sprintf((LPSTR)&m_tflv.lpdata,"%s",pPkt->szAccountName);
							DBVect.push_back(m_tflv);
							
							_itoa(pPkt->sGMAccountRoleInfo[i].iDBID,strInt,10);
							m_tflv.nIndex=DBVect.size()+1;
							m_tflv.m_tagName=CEnumCore::TagName::PAL_ROLEID;
							m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
							m_tflv.m_tvlength=sizeof(int);
							sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
							DBVect.push_back(m_tflv);
							
							m_tflv.nIndex=DBVect.size()+1;
							m_tflv.m_tagName=CEnumCore::TagName::PAL_ROLENAME;
							m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
							m_tflv.m_tvlength=strlen(pPkt->sGMAccountRoleInfo[i].szRoleName);
							sprintf((LPSTR)&m_tflv.lpdata,"%s",pPkt->sGMAccountRoleInfo[i].szRoleName);
							DBVect.push_back(m_tflv);


							m_tflv.nIndex=DBVect.size()+1;
							m_tflv.m_tagName=CEnumCore::TagName::PAL_TIME;
							m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
							m_tflv.m_tvlength=strlen(pPkt->sGMAccountRoleInfo[i].szCreateTime);
							sprintf((LPSTR)&m_tflv.lpdata,"%s",pPkt->sGMAccountRoleInfo[i].szCreateTime);
							DBVect.push_back(m_tflv);

						}

					}
				}
				break;
			case ENUM_AccountInfoResult_AccountNotFound:
				printf("AccountInfo result: Account %s not found\n", pPkt->szAccountName);
				ZeroMemory(bufstr,256);
				sprintf(bufstr,"�˺���Ϣ: �˺�%sû���ҵ�\n", pPkt->szAccountName);
				m_tflv.nIndex=DBVect.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(bufstr);
				sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
				DBVect.push_back(m_tflv);
				break;
			
			case ENUM_AccountInfoResult_RoleNotFound:
				printf("AccountInfo result: Account %s has no role\n", pPkt->szAccountName);
				ZeroMemory(bufstr,256);
				sprintf(bufstr,"�˺���Ϣ:�˺� %s û�н�ɫ\n", pPkt->szAccountName);
				m_tflv.nIndex=DBVect.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(bufstr);
				sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
				DBVect.push_back(m_tflv);
				break;	
			
			case ENUM_AccountInfoResult_WorldNotFound:
				printf("AccountInfo result: World not found\n");
				ZeroMemory(bufstr,256);
				sprintf(bufstr,"�˺���Ϣ: ����û���ҵ�\n");
				m_tflv.nIndex=DBVect.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(bufstr);
				sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
				DBVect.push_back(m_tflv);
				break;
			
			case ENUM_AccountInfoResult_LeaderNotFound:
				printf("AccountInfo result: Leader not found\n");
				ZeroMemory(bufstr,256);
				sprintf(bufstr,"�˺���Ϣ: �۲���û���ҵ�\n");

				break;
			
			case ENUM_AccountInfoResult_LeaderDisconnect:
				printf("AccountInfo result: Leader disconnect\n");
				ZeroMemory(bufstr,256);
				sprintf(bufstr,"�˺���Ϣ: �۲���û������\n");
				m_tflv.nIndex=DBVect.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(bufstr);
				sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
				DBVect.push_back(m_tflv);
				break;
			default:
				break;
		}
		g_state = -1;

}
