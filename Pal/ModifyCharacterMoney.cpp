// ModifyCharacterMoney.cpp: implementation of the CModifyCharacterMoney class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ModifyCharacterMoney.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CModifyCharacterMoney::CModifyCharacterMoney()
{

}

CModifyCharacterMoney::~CModifyCharacterMoney()
{

}
vector <CGlobalStruct::TFLV> CModifyCharacterMoney::ModifyCharacterMoneyMain(char *szGMServerName, char * szGMServerIP,char* szRoleName,int iType,int iCount)
{

			char strlog[80];
			ZeroMemory(strlog,80);
			sprintf(strlog,"����<%s>�޸Ľ�ɫ<%s>��Ǯ",szGMServerName,szRoleName);
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
			g_ccNetObj.RegEvent_OnPacket(ENUM_PGPlayerControl_StoC_ModifyCharacterMoney, ModifyCharacterMoney, 0);
			
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
							//char szAccount[32] = "silent2";			// �����ʺ�
							//char szPassword[32] = "silent2";		// ��������		
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
						//int  iType			= 1;		// ����(0:���� 1:����)
						//int  iCount			= -1;		// ���ӽ��(��Ϊ������������)
						
						iWorldID=operPal.FindWorldID(szGMServerName,szGMServerIP);
						
						PGPlayerControl_CtoS_ModifyCharacterMoney sPkt;
						sPkt.iWorldID	= iWorldID;
						sprintf(sPkt.szRoleName, "%s", szRoleName);
						sPkt.iType		= iType;
						sPkt.iCount		= iCount;
						
						// ������GMSҪ���޸Ľ�ɫ��Ǯ
						g_ccNetObj.Send(sizeof(sPkt), (S_ObjNetPktBase*)&sPkt);	
						g_state = 3;	

					}
					break;
				// �ȴ��޸Ĺ���ȼ�
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
					printf("�ȴ��޸Ĺ���ȼ���,��%i��\n",n_send);
					break;
				}
			}			
			g_ccNetObj.Disconnect();
			printf("\n");
			printf("\n==================== Shutdown ====================\n");
			return DBVect;

	
}

void CModifyCharacterMoney::LoginResult(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
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
// �޸Ľ�ɫ��Ǯ���-----------------------------------------------------------------------------------

void CModifyCharacterMoney::ModifyCharacterMoney(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
{
	try
	{
		PGPlayerControl_StoC_ModifyCharacterMoney *pPkt = (PGPlayerControl_StoC_ModifyCharacterMoney *)pData;
		char bufstr[256];
		ZeroMemory(bufstr,256);
		
		switch(pPkt->emResult)
		{
		case ENUM_ModifyCharacterMoneyResult_Success:		
			ZeroMemory(bufstr,256);
			
			sprintf(bufstr,"�޸Ľ�ɫ��Ǯ�ɹ�,������:%d,��ɫ��:%s,����:%d,��������:%d\n",
				pPkt->iWorldID,
				pPkt->szRoleName,
				pPkt->iType,
				pPkt->iCount);
			printf("ModifyCharacterMoney succsee\n");
			printf("WorldID: %d\n"	, pPkt->iWorldID);
			printf("RoleName: %s\n"	, pPkt->szRoleName);
			printf("Type: %d\n"		, pPkt->iType);
			printf("Count: %d\n"	, pPkt->iCount);
			break;
		case ENUM_ModifyCharacterMoneyResult_WorldNotFound:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"�޸Ľ�ɫ��Ǯ���: ����û���ҵ�\n");
			printf("ModifyCharacterMoney result: World not found\n");
			break;	
		case ENUM_ModifyCharacterMoneyResult_LeaderNotFound:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"�޸Ľ�ɫ��Ǯ���: Ƶ��û���ҵ�\n");
			printf("ModifyCharacterMoney result: Leader not found\n");
			break;
		case ENUM_ModifyCharacterMoneyResult_LeaderDisconnect:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"�޸Ľ�ɫ��Ǯ���: Ƶ��û������\n");
			printf("ModifyCharacterMoney result: Leader disconnect\n");
			break;
		case ENUM_ModifyCharacterMoneyResult_TypeError:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"�޸Ľ�ɫ��Ǯ���: ���ʹ���\n");
			printf("ModifyCharacterMoney result: Type error\n");
			break;	
		case ENUM_ModifyCharacterMoneyResult_RoleNotFound:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"�޸Ľ�ɫ��Ǯ���: ��ɫ %sû���ҵ�\n", pPkt->szRoleName);
			printf("ModifyCharacterMoney result: Character %s not found\n", pPkt->szRoleName);
			break;	
		case ENUM_ModifyCharacterMoneyResult_RoleIsOnline:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"�޸Ľ�ɫ��Ǯ���: ��ɫ %s����\n", pPkt->szRoleName);
			printf("ModifyCharacterMoney result: Character %s is online now\n", pPkt->szRoleName);
			break;	
		case ENUM_ModifyCharacterMoneyResult_CountError:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"�޸Ľ�ɫ��Ǯ���: ����������, ��ɫ��Ǯ������Χ\n");
			printf("ModifyCharacterMoney result: Count error, character money out of range\n");
			break;	
		case ENUM_ModifyCharacterMoneyResult_GMOOperationFailed:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"�޸Ľ�ɫ��Ǯ���:��Ϸ����������ʧ��\n");
			printf("ModifyCharacterMoney result: Operation failed from GMObserver\n");
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
	catch (...)
	{
	}
}
