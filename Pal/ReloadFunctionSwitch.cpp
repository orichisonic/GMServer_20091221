// ReloadFunctionSwitch.cpp: implementation of the CReloadFunctionSwitch class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ReloadFunctionSwitch.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CReloadFunctionSwitch::CReloadFunctionSwitch()
{

}

CReloadFunctionSwitch::~CReloadFunctionSwitch()
{

}
void CReloadFunctionSwitch::ReloadFunctionSwitchMain(SOCKET m_socket,COperVector * recvopervector,CSession * pPalSession)
{	



			// ע������CallBack��ʽ
			g_ccNetObj.RegEvent_OnConnected(COperPal::OnConnect, 0);
			g_ccNetObj.RegEvent_OnDisconnected(COperPal::OnDisconnect, 0);
			g_ccNetObj.RegEvent_OnConnectFailed(COperPal::OnConnectfailed, 0);

			// ע���Ӧ�¼�CallBack��ʽ
			g_ccNetObj.RegEvent_OnPacket(ENUM_PGGMCConnection_StoC_LoginResult,	LoginResult, 0);
			g_ccNetObj.RegEvent_OnPacket(ENUM_PGServerControl_StoC_ReloadFunctionSwitch, ReloadFunctionSwitch, 0);

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
			char szGMServerIP[16];
			char szRoleName[32];
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
						sprintf(szGMServerIP,"%s",(char *)&(recvopervector->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata);
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
					}
					printf("������������,��%i��\n",n_login);
					break;
					// ����ɹ�
				case 2:
					{
						printf("����ɹ�\n\n");

						// �趨�������
					//	int  iWorldID = 2;							// ������
					//	char szRoleName[32] = "�ɵ���";		// ��ɫ����

						PGServerControl_CtoS_ReloadFunctionSwitch sPkt;
						sPkt.iWorldID = iWorldID;

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
					}
					printf("�ȴ�ȡ�ý�ɫ������Ѷ��,��%i��\n",n_send);
					break;
				}
			}			
			g_ccNetObj.Disconnect();
			printf("\n");
			printf("\n==================== Shutdown ====================\n");
			


}
void CReloadFunctionSwitch::LoginResult(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
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
void CReloadFunctionSwitch::ReloadFunctionSwitch(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
{

		PGServerControl_StoC_ReloadFunctionSwitch  *pPkt = (PGServerControl_StoC_ReloadFunctionSwitch  *)pData;
		char strInt[10];
		char bufstr[256];
		switch(pPkt->emResult)
		{
		case ENUM_ReloadFunctionSwitchResult_Success:
			{
				printf("ReloadFunctionSwitch success, world id: %d\n", pPkt->iWorldID);	
				ZeroMemory(bufstr,256);
				sprintf(bufstr,"���ع��ܿ��سɹ�:����ţ�%i\n", pPkt->iWorldID);

			}
			break;

		case ENUM_ReloadFunctionSwitchResult_WorldNotFound:
				printf("ReloadFunctionSwitch result: World not found\n");
				ZeroMemory(bufstr,256);
				sprintf(bufstr,"���ع��ܿ��ؽ��:����û���ҵ�\n");
			break;
		case ENUM_ReloadFunctionSwitchResult_LeaderNotFound:
				printf("ReloadFunctionSwitch result: Leader not found\n");
				ZeroMemory(bufstr,256);
				sprintf(bufstr,"���ع��ܿ��ؽ��:�۲���û���ҵ�\n");
			break;

		case ENUM_ReloadFunctionSwitchResult_LeaderDisconnect:
			printf("ReloadFunctionSwitch result: Leader disconnect\n");
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"���ع��ܿ��ؽ��:�۲��߶���\n");
			break;
		case ENUM_ReloadFunctionSwitchResult_LocalDisconnect:
			printf("ReloadFunctionSwitch result: Local Disconnect\n");
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"���ع��ܿ��ؽ��:�������ӶϿ�\n");
			break;
		default:
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
