// MoveCharacterToSafe.cpp: implementation of the CMoveCharacterToSafe class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MoveCharacterToSafe.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMoveCharacterToSafe::CMoveCharacterToSafe()
{

}

CMoveCharacterToSafe::~CMoveCharacterToSafe()
{

}
vector<CGlobalStruct::TFLV> CMoveCharacterToSafe::MoveCharacterToSafeMain(char * szGMServerName,char * szGMServerIP,char * szRoleName,int iZoneID)
{	

			char strlog[80];
			sprintf(strlog,"����<%s>��ɫ<%s>�ƶ�����ȫ��",szGMServerName,szRoleName);
			logFile.WriteText("�ɽ�OL",strlog);

			CGlobalStruct::TFLV m_tflv;
			if(!DBVect.empty())
			{
				DBVect.clear();
			}

			// ����DBF���Բ�ѯ��������(ָ��"��ϷĿ¼\Kernel\\data")
			Pal_LoadData(".\\data");

			// ע������CallBack��ʽ
			g_ccNetObj.RegEvent_OnConnected(COperPal::OnConnect, 0);
			g_ccNetObj.RegEvent_OnDisconnected(COperPal::OnDisconnect, 0);
			g_ccNetObj.RegEvent_OnConnectFailed(COperPal::OnConnectfailed, 0);

			// ע���Ӧ�¼�CallBack��ʽ
			g_ccNetObj.RegEvent_OnPacket(ENUM_PGGMCConnection_StoC_LoginResult,	LoginResult, 0);
			g_ccNetObj.RegEvent_OnPacket(ENUM_PGPlayerControl_StoC_MoveCharacterToSafe, MoveCharacterToSafe, 0);

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

						int  X, Y, Z;					// λ������
						// ������������ѯ��ȫ������
						bool bSuccess = Pal_GetSavePoint(iZoneID, &X, &Y, &Z);
						// ����ѯʧ�������
						if (!bSuccess) 
						{
							printf("��ѯ��ȫ������ʧ��,ָ����ֹ");
							m_tflv.nIndex=DBVect.size()+1;
							m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
							m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
							m_tflv.m_tvlength=strlen("��ѯ��ȫ������ʧ��,ָ����ֹ");
							sprintf((LPSTR)&m_tflv.lpdata,"%s","��ѯ��ȫ������ʧ��,ָ����ֹ");
							DBVect.push_back(m_tflv);		
							g_state = -1;
							break;
						}
						
						PGPlayerControl_CtoS_MoveCharacterToSafe sPkt;
						sPkt.iWorldID = iWorldID;
						sprintf(sPkt.szRoleName, "%s", szRoleName);
						sPkt.iZoneID	= iZoneID;
						sPkt.X			= X;
						sPkt.Y			= Y;
						sPkt.Z			= Z;
						
						// ������GMSҪ���ƶ���ɫ����ȫ��
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
void CMoveCharacterToSafe::LoginResult(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
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
void CMoveCharacterToSafe::MoveCharacterToSafe(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
{
	PGPlayerControl_StoC_MoveCharacterToSafe *pPkt = (PGPlayerControl_StoC_MoveCharacterToSafe *)pData;
	char bufstr[256];		
	switch(pPkt->emResult)
	{
		case ENUM_MoveCharacterToSafeResult_Success:			
			printf("MoveCharacterToSafe succsee, world id: %d, role name: %s, zone id: %d\n", 
				pPkt->iWorldID, pPkt->szRoleName, pPkt->iZoneID);
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"�ƶ���ɫ����ȫ��ɹ�:�����%d,��ɫ����%s,����ID %d\n", pPkt->iWorldID, pPkt->szRoleName, pPkt->iZoneID);
			break;
			
		case ENUM_MoveCharacterToSafeResult_RoleNotFound:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"�ƶ���ɫ����ȫ��:  ��ɫ %s ������\n", pPkt->szRoleName);
			printf("MoveCharacterToSafe result: Character %s not exist\n", pPkt->szRoleName);
			break;	
			
		case ENUM_MoveCharacterToSafeResult_WorldNotFound:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"�ƶ���ɫ����ȫ��: ����û���ҵ�");
			printf("MoveCharacterToSafe result: world not found\n");
			break;	
			
		case ENUM_MoveCharacterToSafeResult_LeaderNotFound:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"�ƶ���ɫ����ȫ��: �۲���û���ҵ�");
			printf("MoveCharacterToSafe result: leader not found\n");
			break;
			
		case ENUM_MoveCharacterToSafeResult_LeaderDisconnect:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"�ƶ���ɫ����ȫ��: �۲��߶Ͽ�����");
			printf("MoveCharacterToSafe result: leader disconnect\n");
			break;
			
		case ENUM_MoveCharacterToSafeResult_GMOOperationFailed:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"�ƶ���ɫ����ȫ��: Զ�̲���ʧ��");
			printf("MoveCharacterToSafe result: Operation failed from GMObserver\n");
			break;	
			
		case ENUM_MoveCharacterToSafeResult_LocalOperationFailed:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"�ƶ���ɫ����ȫ��: ���ز���ʧ��");
			printf("MoveCharacterToSafe result: Operation failed from localbserver\n");
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
