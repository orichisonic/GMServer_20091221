// CharacterPuzzleMap.cpp: implementation of the CCharacterPuzzleMap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CharacterPuzzleMap.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCharacterPuzzleMap::CCharacterPuzzleMap()
{

}

CCharacterPuzzleMap::~CCharacterPuzzleMap()
{

}

vector <CGlobalStruct::TFLV> CCharacterPuzzleMap::CharacterPuzzleMapMain(char *szGMServerName, char * szGMServerIP,char* szRoleName)
{	
			char strlog[256];
			sprintf(strlog,"����<%s>��ɫ<%s>��ѯ��ɫͼ����Ϣ",szGMServerName,szRoleName);
			logFile.WriteText("�ɽ�OL",strlog);

			Pal_LoadData(".\\data");
			int iCount = Pal_GetLocalCount();
			// ע������CallBack��ʽ
			g_ccNetObj.RegEvent_OnConnected(COperPal::OnConnect, 0);
			g_ccNetObj.RegEvent_OnDisconnected(COperPal::OnDisconnect, 0);
			g_ccNetObj.RegEvent_OnConnectFailed(COperPal::OnConnectfailed, 0);
			
			// ע���Ӧ�¼�CallBack��ʽ
			g_ccNetObj.RegEvent_OnPacket(ENUM_PGGMCConnection_StoC_LoginResult,	LoginResult, 0);
			g_ccNetObj.RegEvent_OnPacket(ENUM_PGPlayerStatus_StoC_CharacterPuzzleMap, CharacterPuzzleMap, 0);

			CGlobalStruct::TFLV m_tflv;
			if(!DBVect.empty())
			{
				DBVect.clear();
			}
			

		

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

						PGPlayerStatus_CtoS_CharacterPuzzleMap sPkt;
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
void CCharacterPuzzleMap::LoginResult(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
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
void CCharacterPuzzleMap::CharacterPuzzleMap(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
{

		
		PGPlayerStatus_StoC_CharacterPuzzleMap *pPkt = (PGPlayerStatus_StoC_CharacterPuzzleMap *)pData;
		CGlobalStruct::TFLV m_tflv;
		char strInt[10];
		char bufstr[256];
		switch(pPkt->emResult)
		{
		case ENUM_CharacterPuzzleMapResult_Success:
			{
				BOOL iFlag=false;
				char szName[32];
				int iCount = Pal_GetLocalCount();									// ȡ�ú���ͼ���ĵ�ͼ����
				S_PuzzleMapInfo *sPuzzleMapInfo = new S_PuzzleMapInfo[iCount];		// ����ͼ��������Ѷ
				Pal_GetPuzzleMapFlagResult(sPuzzleMapInfo, pPkt->dwPuzzleMapFlag);	// ȡ��ͼ��������Ѷ
				
				// ������
				printf("\n====================================================\n");
				printf("World: %d\n",					pPkt->iWorldID);
				printf("Account: %s\n",					pPkt->szAccount);
				printf("RoleName: %s\n",				pPkt->szRoleName);
				
				printf("\n[ ͼ����Ѷ ]\n");
				printf("\n��������:ͼ�����1~9(0=δȡ��,1=��ȡ��)\n\n");

				for (int i=0; i<iCount; i++)
				{
					printf("%32s: ", sPuzzleMapInfo[i].szName);
					for (int j=0; j<9; j++)
					{
						printf("%d, ", sPuzzleMapInfo[i].iMapCollection[j]);
					}
					printf("\n");
				}

					
				

				for (i=0; i<iCount; i++)
				{
					iFlag=true;
					sprintf(szName,"%s",sPuzzleMapInfo[i].szName);
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
					m_tflv.m_tagName=CEnumCore::TagName::PAL_PuzzleMap;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=strlen(szName);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",szName);
					DBVect.push_back(m_tflv);


					_itoa(sPuzzleMapInfo[i].iMapCollection[0],strInt,10);
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_PuzzleMapID1;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);
		

					_itoa(sPuzzleMapInfo[i].iMapCollection[1],strInt,10);
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_PuzzleMapID2;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);

					_itoa(sPuzzleMapInfo[i].iMapCollection[2],strInt,10);
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_PuzzleMapID3;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);

					_itoa(sPuzzleMapInfo[i].iMapCollection[3],strInt,10);
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_PuzzleMapID4;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);

					_itoa(sPuzzleMapInfo[i].iMapCollection[4],strInt,10);
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_PuzzleMapID5;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);

					_itoa(sPuzzleMapInfo[i].iMapCollection[5],strInt,10);
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_PuzzleMapID6;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);

					_itoa(sPuzzleMapInfo[i].iMapCollection[6],strInt,10);
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_PuzzleMapID7;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);

					_itoa(sPuzzleMapInfo[i].iMapCollection[7],strInt,10);
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_PuzzleMapID8;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);

					_itoa(sPuzzleMapInfo[i].iMapCollection[8],strInt,10);
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_PuzzleMapID9;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);
				}
				
				delete [] sPuzzleMapInfo;
				if(!iFlag)
				{
					ZeroMemory(bufstr,256);
					sprintf(bufstr,"ͼ�����: ��ɫ%sû��ͼ��\n", pPkt->szRoleName);
					
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=strlen(bufstr);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
					DBVect.push_back(m_tflv);
				}
			
				
				
			}
			break;
		case ENUM_CharacterPuzzleMapResult_RoleNotFound:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"��ɫͼ�����: ��ɫ %s û���ҵ�\n", pPkt->szRoleName);
			printf("CharacterPuzzleMap result: Character %s not found\n", pPkt->szRoleName);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(bufstr);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
			DBVect.push_back(m_tflv);
				g_state = -1;
			break;
			
		case ENUM_CharacterPuzzleMapResult_WorldNotFound:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"��ɫͼ�����: ����û���ҵ�\n");
			printf("CharacterPuzzleMap result: World not found\n");
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(bufstr);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
			DBVect.push_back(m_tflv);
				g_state = -1;
			break;
			
		case ENUM_CharacterPuzzleMapResult_LeaderNotFound:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"��ɫͼ�����: �۲���û���ҵ�\n");
			printf("CharacterPuzzleMap result: Leader not found\n");
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(bufstr);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
			DBVect.push_back(m_tflv);
				g_state = -1;
			break;
			
		case ENUM_CharacterPuzzleMapResult_LeaderDisconnect:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"��ɫͼ�����: �۲���û������\n");
			printf("CharacterPuzzleMap result: Leader disconnect\n");
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(bufstr);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
			DBVect.push_back(m_tflv);
				g_state = -1;
			break;
			
		default:
			break;
		}
	
}
