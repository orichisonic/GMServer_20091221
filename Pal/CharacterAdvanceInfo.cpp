// CharacterAdvanceInfo.cpp: implementation of the CCharacterAdvanceInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CharacterAdvanceInfo.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCharacterAdvanceInfo::CCharacterAdvanceInfo()
{

}

CCharacterAdvanceInfo::~CCharacterAdvanceInfo()
{

}

vector <CGlobalStruct::TFLV> CCharacterAdvanceInfo::CharacterAdvanceInfoMain(char *szGMServerName, char * szGMServerIP,char* szRoleName)
{	
	
			// ע������CallBack��ʽ
			g_ccNetObj.RegEvent_OnConnected(COperPal::OnConnect, 0);
			g_ccNetObj.RegEvent_OnDisconnected(COperPal::OnDisconnect, 0);
			g_ccNetObj.RegEvent_OnConnectFailed(COperPal::OnConnectfailed, 0);
	
			// ע���Ӧ�¼�CallBack��ʽ
			g_ccNetObj.RegEvent_OnPacket(ENUM_PGGMCConnection_StoC_LoginResult,	LoginResult, 0);
			g_ccNetObj.RegEvent_OnPacket(ENUM_PGPlayerStatus_StoC_CharacterAdvanceInfo, CharacterAdvanceInfo, 0);


			char strlog[80];
			sprintf(strlog,"����<%s>��ɫ<%s>ȡ�ý�ɫ������Ѷ",szGMServerName,szRoleName);
			logFile.WriteText("�ɽ�OL",strlog);

			Pal_LoadData(".\\data");
			

			// ��ʼ��·�趨
			g_ccNetObj.SetConnectCount(10);		// �趨�������ߴ���Ϊ10
			g_ccNetObj.SetReConnect(false);		// �趨���ߵ�ʱ�򲻻���������
			g_ccNetObj.SetShowIP(true);			// �趨����ʱ����ʾIP

			Sleep(100);
			g_state = 0;

			CGlobalStruct::TFLV m_tflv;
			if(!DBVect.empty())
			{
				DBVect.clear();
			}

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
			
						PGPlayerStatus_CtoS_CharacterAdvanceInfo sPkt;
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
void CCharacterAdvanceInfo::LoginResult(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
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
void CCharacterAdvanceInfo::CharacterAdvanceInfo(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
{

		PGPlayerStatus_StoC_CharacterAdvanceInfo *pPkt = (PGPlayerStatus_StoC_CharacterAdvanceInfo *)pData;
		CGlobalStruct::TFLV m_tflv;
		char strInt[10];
		char bufstr[256];
		char szFame[32];
		switch(pPkt->emResult)
		{
		case ENUM_CharacterAdvanceInfoResult_Success:
			{
				// ������
				printf("\n====================================================\n");
				printf("World: %d\n",		pPkt->iWorldID);
				printf("Account: %s\n",		pPkt->szAccount);
				printf("RoleName: %s\n",	pPkt->szRoleName);
				for (int i=0; i<20; i++)
				{
					char szFame[32];
					sprintf(szFame, Pal_GetFameName(i));
					if (szFame != NULL)
						printf("%s: %d\n", szFame, pPkt->iPlotFame[i]);
				}	

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
			
				sprintf(szFame, Pal_GetFameName(0));
				if (szFame != NULL)
				{
					_itoa(pPkt->iPlotFame[0],strInt,10);
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_PlotName0;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);
				}
				sprintf(szFame, Pal_GetFameName(1));
				if (szFame != NULL)
				{
					_itoa(pPkt->iPlotFame[1],strInt,10);
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_PlotName1;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);
				}
				sprintf(szFame, Pal_GetFameName(2));
				if (szFame != NULL)
				{
					_itoa(pPkt->iPlotFame[2],strInt,10);
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_PlotName2;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);		
				}
				sprintf(szFame, Pal_GetFameName(3));
				if (szFame != NULL)
				{
					_itoa(pPkt->iPlotFame[3],strInt,10);
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_PlotName3;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);			
				}
				sprintf(szFame, Pal_GetFameName(4));
				if (szFame != NULL)
				{
					_itoa(pPkt->iPlotFame[4],strInt,10);
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_PlotName4;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);			
				}
				sprintf(szFame, Pal_GetFameName(5));
				if (szFame != NULL)
				{
					_itoa(pPkt->iPlotFame[5],strInt,10);
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_PlotName5;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);			
				}
				sprintf(szFame, Pal_GetFameName(6));
				if (szFame != NULL)
				{
					_itoa(pPkt->iPlotFame[6],strInt,10);
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_PlotName6;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);			
				}
				sprintf(szFame, Pal_GetFameName(7));
				if (szFame != NULL)
				{
					_itoa(pPkt->iPlotFame[7],strInt,10);
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_PlotName7;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);			
				}
				sprintf(szFame, Pal_GetFameName(8));
				if (szFame != NULL)
				{
					_itoa(pPkt->iPlotFame[8],strInt,10);
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_PlotName8;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);			
				}
				sprintf(szFame, Pal_GetFameName(9));
				if (szFame != NULL)
				{
					_itoa(pPkt->iPlotFame[9],strInt,10);
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_PlotName9;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);			
				}
				sprintf(szFame, Pal_GetFameName(10));
				if (szFame != NULL)
				{
					_itoa(pPkt->iPlotFame[10],strInt,10);
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_PlotName10;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);			
				}
				sprintf(szFame, Pal_GetFameName(11));
				if (szFame != NULL)
				{
					_itoa(pPkt->iPlotFame[11],strInt,10);
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_PlotName11;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);			
				}
				sprintf(szFame, Pal_GetFameName(12));
				if (szFame != NULL)
				{
					_itoa(pPkt->iPlotFame[12],strInt,10);
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_PlotName12;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);			
				}
				sprintf(szFame, Pal_GetFameName(13));
				if (szFame != NULL)
				{
					_itoa(pPkt->iPlotFame[13],strInt,10);
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_PlotName13;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);			
				}
				sprintf(szFame, Pal_GetFameName(14));
				if (szFame != NULL)
				{
					_itoa(pPkt->iPlotFame[14],strInt,10);
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_PlotName14;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);			
				}
				sprintf(szFame, Pal_GetFameName(15));
				if (szFame != NULL)
				{
					_itoa(pPkt->iPlotFame[15],strInt,10);
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_PlotName15;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);			
				}
				sprintf(szFame, Pal_GetFameName(16));
				if (szFame != NULL)
				{
					_itoa(pPkt->iPlotFame[16],strInt,10);
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_PlotName16;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);			
				}
				sprintf(szFame, Pal_GetFameName(17));
				if (szFame != NULL)
				{
					_itoa(pPkt->iPlotFame[17],strInt,10);
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_PlotName17;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);			
				}
				sprintf(szFame, Pal_GetFameName(18));
				if (szFame != NULL)
				{
					_itoa(pPkt->iPlotFame[18],strInt,10);
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_PlotName18;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);				
				}
				sprintf(szFame, Pal_GetFameName(19));
				if (szFame != NULL)
				{
					_itoa(pPkt->iPlotFame[19],strInt,10);
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_PlotName19;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);
				}


			}
		break;
		case ENUM_CharacterAdvanceInfoResult_RoleNotFound:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"��ɫ�����б���: Character %s not found\n", pPkt->szRoleName);
			printf("Charfriendlist result: Character %s not found\n", pPkt->szRoleName);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(bufstr);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
			DBVect.push_back(m_tflv);
			break;

		case ENUM_CharacterAdvanceInfoResult_WorldNotFound:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"��ɫ�����б���: ��ɫ %s û������\n", pPkt->szRoleName);
			printf("Charfriendlist result: Character %s has no friend\n", pPkt->szRoleName);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(bufstr);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
			DBVect.push_back(m_tflv);
			break;


		case ENUM_CharacterAdvanceInfoResult_LeaderNotFound:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"��ɫ�����б���: Ƶ��û���ҵ�\n");
			printf("Charfriendlist result: Leader not found\n");
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(bufstr);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
			DBVect.push_back(m_tflv);
			break;

		case ENUM_CharacterAdvanceInfoResult_LeaderDisconnect:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"��ɫ�����б���: Ƶ��û������\n");
			printf("Charfriendlist result: Leader disconnect\n");
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
