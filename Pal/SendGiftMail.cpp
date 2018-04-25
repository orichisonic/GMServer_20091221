// SendGiftMail.cpp: implementation of the CSendGiftMail class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SendGiftMail.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSendGiftMail::CSendGiftMail()
{

}

CSendGiftMail::~CSendGiftMail()
{

}
vector <CGlobalStruct::TFLV> CSendGiftMail::SendGiftMailMain(char * szGMServerName,
															 char * szGMServerIP,
															 char * szRoleName,
															 char * szSubject,
															 char * szContent,
															 int iMoney,
															 int Gift0,
															 int Gift1,
															 int Gift2,
															 int Gift3,
															 int Gift4,
															 char * szReason)
{
	

	char strlog[80];
	ZeroMemory(strlog,80);
	sprintf(strlog,"����<%s>��ɫ<%s>���͵���",szGMServerName,szRoleName);
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
	g_ccNetObj.RegEvent_OnPacket(ENUM_PGServerControl_StoC_SendGiftMail, SendGiftMail, 0);
	
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
	int  aiGiftDBID[5];

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
					DBVect.push_back(m_tflv);				}
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
				aiGiftDBID[0]=Gift0;
				aiGiftDBID[1]=Gift1;
				aiGiftDBID[2]=Gift2;
				aiGiftDBID[3]=Gift3;
				aiGiftDBID[4]=Gift4;
				
				
				
				PGServerControl_CtoS_SendGiftMail sPkt;
				sPkt.iWorldID	= iWorldID;
				sprintf(sPkt.szRoleName, "%s", szRoleName);
				sprintf(sPkt.szSubject, "%s", szSubject);
				sprintf(sPkt.szContent, "%s", szContent);
				sPkt.iMoney		= iMoney;
				memcpy(sPkt.aiGiftDBID, aiGiftDBID, sizeof(int)*5);  
				sprintf(sPkt.szReason, "%s", szReason);
				
				// ������GMSҪ����ͽ�Ʒ�ʼ�
				g_ccNetObj.Send(sizeof(sPkt), (S_ObjNetPktBase*)&sPkt);	
				g_state = 3;			
			}
			break;
			// �ȴ����ͽ�Ʒ�ʼ�
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
			printf("�ȴ�ȡ���ų̹����б���,��%i��\n",n_send);
			break;
		}
	}			
	g_ccNetObj.Disconnect();
	printf("\n");
	printf("\n==================== Shutdown ====================\n");
	
	return DBVect;
}
void CSendGiftMail::LoginResult(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
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
// ���ͽ�Ʒ�ʼ����-----------------------------------------------------------------------------------
void CSendGiftMail::SendGiftMail(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
{
	PGServerControl_StoC_SendGiftMail *pPkt = (PGServerControl_StoC_SendGiftMail *)pData;
	char bufstr[256];
	ZeroMemory(bufstr,256);
	switch(pPkt->emResult)
	{
	case ENUM_SendGiftMailResult_Success:	
		// ������
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"���������ʼ��ɹ�\n");
		printf("SendGiftMail succsee\n");
		printf("WorldID: %d\n"	, pPkt->iWorldID);
		printf("RoleName: %s\n"	, pPkt->szRoleName);
		printf("Subject: %s\n"	, pPkt->szSubject);
		printf("Money: %d\n"	, pPkt->iMoney);
		printf("Item1 ID: %d\n"	, pPkt->aiGiftDBID[0]);
		printf("Item2 ID: %d\n"	, pPkt->aiGiftDBID[1]);
		printf("Item3 ID: %d\n"	, pPkt->aiGiftDBID[2]);
		printf("Item4 ID: %d\n"	, pPkt->aiGiftDBID[3]);
		printf("Item5 ID: %d\n"	, pPkt->aiGiftDBID[4]);
		break;
		
	case ENUM_SendGiftMailResult_ArgumentError:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"���������ʼ����: ��������\n");
		printf("SendGiftMail result: Argument error\n");
		break;	
		
	case ENUM_SendGiftMailResult_WorldNotFound:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"���������ʼ����: �����Ҳ���\n");
		printf("SendGiftMail result: World not found\n");
		break;	
		
	case ENUM_SendGiftMailResult_LeaderNotFound:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"���������ʼ����: Ƶ���Ҳ���\n");
		printf("SendGiftMail result: Leader not found\n");
		break;
		
	case ENUM_SendGiftMailResult_LeaderDisconnect:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"���������ʼ����: Ƶ��û������\n");
		printf("SendGiftMail result: Leader disconnect\n");
		break;
		
	case ENUM_SendGiftMailResult_RoleNotFound:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"���������ʼ����: ��ɫ %sû���ҵ�\n", pPkt->szRoleName);
		printf("SendGiftMail result: Character %s not found\n", pPkt->szRoleName);
		break;
		
	case ENUM_SendGiftMailResult_LocalDisconnect:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"���������ʼ����: ���ط���Ͽ�\n");
		printf("SendGiftMail result: Local server disconnect\n");
		break;
		
	case ENUM_SendGiftMailResult_LocalOperationFailed:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"���������ʼ����: ���ط������ʧ��\n");
		printf("SendGiftMail result: Operation failed from localserver\n");
		printf("Item1 status: %d\n", pPkt->aiItemStatus[0]);
		printf("Item2 status: %d\n", pPkt->aiItemStatus[1]);
		printf("Item3 status: %d\n", pPkt->aiItemStatus[2]);
		printf("Item4 status: %d\n", pPkt->aiItemStatus[3]);
		printf("Item5 status: %d\n", pPkt->aiItemStatus[4]);
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

