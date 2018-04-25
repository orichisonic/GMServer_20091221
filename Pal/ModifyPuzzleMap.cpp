// ModifyPuzzleMap.cpp: implementation of the CModifyPuzzleMap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ModifyPuzzleMap.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CModifyPuzzleMap::CModifyPuzzleMap()
{

}

CModifyPuzzleMap::~CModifyPuzzleMap()
{

}
vector<CGlobalStruct::TFLV> CModifyPuzzleMap::ModifyPuzzleMapMain(char * szGMServerName,char * szGMServerIP,char * szRoleName,char *strPuzzleMap,
																  int iMapID1,int iMapID2,int iMapID3,int iMapID4,int iMapID5,int iMapID6,int iMapID7,int iMapID8,int iMapID9)
{	


			char strlog[256];
			sprintf(strlog,"����<%s>��ɫ<%s>�޸Ľ�ɫͼ��",szGMServerName,szRoleName);
			logFile.WriteText("�ɽ�OL",strlog);

			CGlobalStruct::TFLV m_tflv;
			if(!DBVect.empty())
			{
				DBVect.clear();
			}

			Pal_LoadData(".\\data");

			// ע������CallBack��ʽ
			g_ccNetObj.RegEvent_OnConnected(COperPal::OnConnect, 0);
			g_ccNetObj.RegEvent_OnDisconnected(COperPal::OnDisconnect, 0);
			g_ccNetObj.RegEvent_OnConnectFailed(COperPal::OnConnectfailed, 0);

			// ע���Ӧ�¼�CallBack��ʽ
			g_ccNetObj.RegEvent_OnPacket(ENUM_PGGMCConnection_StoC_LoginResult,	LoginResult, 0);
			g_ccNetObj.RegEvent_OnPacket(ENUM_PGPlayerControl_StoC_ModifyPuzzleMap, ModifyPuzzleMap, 0);

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
						S_GMPuzzleMapProc sGMPuzzleMapProc;// ͼ��������Ѷ
						sGMPuzzleMapProc.iZoneID =operPal.GetPuzzleMap(strPuzzleMap);			// ��ͼ���
						if(iMapID1==0)
						{
							sGMPuzzleMapProc.bIsOpen[0] = false;	// �趨��1��ƴͼ����
						}
						else
						{	
							sGMPuzzleMapProc.bIsOpen[0] = true;	// �趨��1��ƴͼ����
						}
						if(iMapID2==0)
						{
							sGMPuzzleMapProc.bIsOpen[1] = false;	// �趨��1��ƴͼ����
						}
						else
						{	
							sGMPuzzleMapProc.bIsOpen[1] = true;	// �趨��1��ƴͼ����
						}
						if(iMapID3==0)
						{
							sGMPuzzleMapProc.bIsOpen[2] = false;	// �趨��1��ƴͼ����
						}
						else
						{	
							sGMPuzzleMapProc.bIsOpen[2] = true;	// �趨��1��ƴͼ����
						}
						if(iMapID4==0)
						{
							sGMPuzzleMapProc.bIsOpen[3] = false;	// �趨��1��ƴͼ����
						}
						else
						{	
							sGMPuzzleMapProc.bIsOpen[3] = true;	// �趨��1��ƴͼ����
						}
						if(iMapID5==0)
						{
							sGMPuzzleMapProc.bIsOpen[4] = false;	// �趨��1��ƴͼ����
						}
						else
						{	
							sGMPuzzleMapProc.bIsOpen[4] = true;	// �趨��1��ƴͼ����
						}
						if(iMapID6==0)
						{
							sGMPuzzleMapProc.bIsOpen[5] = false;	// �趨��1��ƴͼ����
						}
						else
						{	
							sGMPuzzleMapProc.bIsOpen[5] = true;	// �趨��1��ƴͼ����
						}
						if(iMapID7==0)
						{
							sGMPuzzleMapProc.bIsOpen[6] = false;	// �趨��1��ƴͼ����
						}
						else
						{	
							sGMPuzzleMapProc.bIsOpen[6] = true;	// �趨��1��ƴͼ����
						}
						if(iMapID8==0)
						{
							sGMPuzzleMapProc.bIsOpen[7] = false;	// �趨��1��ƴͼ����
						}
						else
						{	
							sGMPuzzleMapProc.bIsOpen[7] = true;	// �趨��1��ƴͼ����
						}
						if(iMapID9==0)
						{
							sGMPuzzleMapProc.bIsOpen[8] = false;	// �趨��1��ƴͼ����
						}
						else
						{	
							sGMPuzzleMapProc.bIsOpen[8] = true;	// �趨��1��ƴͼ����
						}
						
		
						Pal_GetMobPuzzleMapFlag(&sGMPuzzleMapProc);	// ȡ��ͼ�������
				
						PGPlayerControl_CtoS_ModifyPuzzleMap sPkt;
						sPkt.iWorldID			= iWorldID;
						sprintf(sPkt.szRoleName, "%s", szRoleName);
						sPkt.sGMPuzzleMapProc	= sGMPuzzleMapProc;
						
						// ������GMSҪ���޸Ľ�ɫͼ��
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
					printf("�ȴ�ȡ���޸Ľ�ɫͼ����,��%i��\n",n_send);
					break;
				}
			}			
			g_ccNetObj.Disconnect();
			printf("\n");
			printf("\n==================== Shutdown ====================\n");
			return DBVect;
}
void CModifyPuzzleMap::LoginResult(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
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
void CModifyPuzzleMap::ModifyPuzzleMap(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
{
	PGPlayerControl_StoC_ModifyPuzzleMap *pPkt = (PGPlayerControl_StoC_ModifyPuzzleMap *)pData;
	char bufstr[256];	
	switch(pPkt->emResult)
	{
	case ENUM_ModifyPuzzleMapResult_Success:
		{
			printf("ModifyPuzzleMap success\n");
			printf("WorldID: %d\n"	, pPkt->iWorldID);
			printf("RoleName: %s\n"	, pPkt->szRoleName);
			
			printf("ZoneID: %d\n"	, pPkt->sGMPuzzleMapProc.iZoneID);
			printf("ZoneName: %s\n" , Pal_GetZoneName(pPkt->sGMPuzzleMapProc.iZoneID));
			printf("0=�ر�,1=����:\n");
			for (int i=0; i<PUZZLEMAP_CUTCOUNT; i++)
			{
				printf("Pic %d: %d\n", i+1, pPkt->sGMPuzzleMapProc.bIsOpen[i]);
			}
			
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"�޸Ľ�ɫͼ��: �����ɹ�\n");
		}
		break;
	case ENUM_ModifyPuzzleMapResult_WorldNotFound:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"�޸Ľ�ɫͼ��: ����û���ҵ�\n");
		printf("ModifyPuzzleMap result: World not found\n");
		break;	
	case ENUM_ModifyPuzzleMapResult_LeaderNotFound:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"�޸Ľ�ɫͼ��: �۲���û���ҵ�\n");
		printf("ModifyPuzzleMap result: Leader not found\n");
		break;
	case ENUM_ModifyPuzzleMapResult_LeaderDisconnect:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"�޸Ľ�ɫͼ��: �۲���û������\n");
		printf("ModifyPuzzleMap result: Leader disconnect\n");
		break;	
	case ENUM_ModifyPuzzleMapResult_RoleNotFound:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"�޸Ľ�ɫͼ��: ��ɫ %s û���ҵ�\n", pPkt->szRoleName);
		printf("ModifyPuzzleMap result: Character %s not found\n", pPkt->szRoleName);
		break;	
	case ENUM_ModifyPuzzleMapResult_RoleIsOnline:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"�޸Ľ�ɫͼ��: ��ɫ %s ��������\n", pPkt->szRoleName);
		printf("ModifyPuzzleMap result: Character %s is online now\n", pPkt->szRoleName);
		break;	
	case ENUM_ModifyPuzzleMapResult_FlagError:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"�޸Ľ�ɫͼ��: ������\n");
		printf("ModifyPuzzleMap result: Flag error\n");
		break;	
	case ENUM_ModifyPuzzleMapResult_GMOOperationFailed:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"�޸Ľ�ɫͼ��: ����������ʧ��\n");
		printf("ModifyPuzzleMap result: Operation failed from GMObserver\n");
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
