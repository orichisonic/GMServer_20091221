#include "stdafx.h"
#include "SendCustomItemMail.h"

CSendCustomItemMail::CSendCustomItemMail()
{
	
}

CSendCustomItemMail::~CSendCustomItemMail()
{
	
}

vector <CGlobalStruct::TFLV> CSendCustomItemMail::SendCustomItemMailMain(char * szGMServerName,char * szGMServerIP,char * szRoleName,char * szSubject,
						char * szContent,int iMoney, int itemType, int iCustomItemID, char *szCustomItemIDName,bool bBindSetup, char *szInherit,char *szReason)
{
	
	char strlog[80];
	ZeroMemory(strlog,80);
	sprintf(strlog,"����<%s>��ɫ<%s>���Ϳ�����Ʒ�ʼ�",szGMServerName,szRoleName);
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
	g_ccNetObj.RegEvent_OnPacket(ENUM_PGServerControl_StoC_SendCustomItemMail, SendCustomItemMail, 0);
	
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
	int iInherit[10] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};

	ZeroMemory(szAccount,32);
	ZeroMemory(szPassword,32);

	char seps[] = ",";
	char *token;
	int iInheritIndex=0;
	token = strtok(szInherit, seps);
	
	while( token != NULL )
	{
		iInherit[iInheritIndex]= atoi(token);
		token = strtok( NULL, seps );
		iInheritIndex++;
	}


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
				operPal.GetUserNamePwd("User5",szAccount,szPassword,&iGMServerPort);
			
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
				
				iWorldID=operPal.FindWorldID(szGMServerName,szGMServerIP);

				PGServerControl_CtoS_SendCustomItemMail sPkt;
				sPkt.iWorldID	= iWorldID;
				strcpy(sPkt.szRoleName, szRoleName);
				strcpy(sPkt.szSubject, szSubject);
				strcpy(sPkt.szContent, szContent);
				sPkt.iMoney	= iMoney;
				sPkt.iCustomItemID	= iCustomItemID;
				sPkt.bBindSetup	= bBindSetup;
				memcpy(sPkt.iInherit, iInherit, sizeof(int)*10);/* �ȴ�ȷ��*/
				strcpy(sPkt.szReason, szReason);
				
				// ������GMSҪ����Ϳ�����Ʒ�ʼ�
				g_ccNetObj.Send(sizeof(sPkt), (S_ObjNetPktBase*)&sPkt);	
				g_state = 3;		
			}
			break;
			// �ȴ����Ϳ�����Ʒ�ʼ�
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
			printf("�ȴ����Ϳ�����Ʒ�ʼ���,��%i��\n",n_send);
			break;
		}
	}			
	g_ccNetObj.Disconnect();
	printf("\n");
	printf("\n==================== Shutdown ====================\n");

	
	return DBVect;
}

void CSendCustomItemMail::LoginResult(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
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
// ���Ϳ�����Ʒ�ʼ����-----------------------------------------------------------------------------------
void CSendCustomItemMail::SendCustomItemMail(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
{
	char bufstr[256];

	PGServerControl_StoC_SendCustomItemMail *pPkt = (PGServerControl_StoC_SendCustomItemMail *)pData;

	switch(pPkt->emResult)
	{
	case ENUM_SendCustomItemMailResult_Success:	
		// ������
		// ������
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"���Ϳ�����Ʒ�ʼ��ɹ�\n");

		printf("SendCustomItemMail success\n");
		printf("WorldID: %d\n"		, pPkt->iWorldID);
		printf("RoleName: %s\n"		, pPkt->szRoleName);
		printf("Subject: %s\n"		, pPkt->szSubject);
		printf("Money: %d\n"		, pPkt->iMoney);
		printf("CustomItemID: %d\n"	, pPkt->iCustomItemID);
		printf("BindSetup: %d\n"	, pPkt->bBindSetup);
		printf("����ֵ(����): %d\n"	, pPkt->iInherit[0]);
		printf("��ħ: %d\n"			, pPkt->iInherit[1]);
		printf("������: %d\n"		, pPkt->iInherit[2]);
		printf("�巨��: %d\n"		, pPkt->iInherit[3]);
		printf("��ʯ1: %d\n"		, pPkt->iInherit[4]);
		printf("��ʯ2: %d\n"		, pPkt->iInherit[5]);
		printf("��ʯ3: %d\n"		, pPkt->iInherit[6]);
		printf("��ʯ4: %d\n"		, pPkt->iInherit[7]);
		printf("��ʯ5: %d\n"		, pPkt->iInherit[8]);
		printf("��ʯ6: %d\n"		, pPkt->iInherit[9]);
		break;
		
	case ENUM_SendCustomItemMailResult_ArgumentError:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"���Ϳ�����Ʒ�ʼ�����������ȷ");
		printf("SendCustomItemMail result: Argument error\n");
		break;	

	case ENUM_SendCustomItemMailResult_ItemIDError:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"���Ϳ�����Ʒ�ʼ���Ʒ����Ʒ��Ų���ȷ");
		printf("SendCustomItemMail result: Incorrect CustomItemID %d\n", pPkt->iCustomItemID);
		break;	
		
	case ENUM_SendCustomItemMailResult_WorldNotFound:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"���Ϳ�����Ʒ�ʼ���Ʒ���Ҳ�������");
		printf("SendCustomItemMail result: World not found\n");
		break;	
		
	case ENUM_SendCustomItemMailResult_LeaderNotFound:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"���Ϳ�����Ʒ�ʼ���Ʒ���Ҳ�����ͷ�۲���");
		printf("SendCustomItemMail result: Leader not found\n");
		break;
		
	case ENUM_SendCustomItemMailResult_LeaderDisconnect:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"���Ϳ�����Ʒ�ʼ���Ʒ����ͷ�۲���δ����");
		printf("SendCustomItemMail result: Leader disconnect\n");
		break;
		
	case ENUM_SendCustomItemMailResult_RoleNotFound:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"���Ϳ�����Ʒ�ʼ���Ʒ����ɫ������");
		printf("SendCustomItemMail result: Character %s not found\n", pPkt->szRoleName);
		break;
		
	case ENUM_SendCustomItemMailResult_LocalDisconnect:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"���Ϳ�����Ʒ�ʼ���Ʒ�������ŷ���δ����");
		printf("SendCustomItemMail result: Local server disconnect");
		break;
		
	case ENUM_SendCustomItemMailResult_LocalOperationFailed:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"���Ϳ�����Ʒ�ʼ���Ʒ������ʧ��\n");
		printf("SendCustomItemMail result: Operation failed from localserver\n");
		break;
		
	default:
		break;
	}//switch
	g_state = -1;

	CGlobalStruct::TFLV m_tflv;
	m_tflv.nIndex=DBVect.size()+1;
	m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
	m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
	m_tflv.m_tvlength=strlen(bufstr);
	sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
	DBVect.push_back(m_tflv);
}
