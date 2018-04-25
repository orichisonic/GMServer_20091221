// DelCharacterMob.cpp: implementation of the CDelCharacterMob class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DelCharacterMob.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDelCharacterMob::CDelCharacterMob()
{

}

CDelCharacterMob::~CDelCharacterMob()
{

}
vector <CGlobalStruct::TFLV>  CDelCharacterMob::DelCharacterMobMain(char *szGMServerName, char * szGMServerIP,char* szRoleName,int iMobID)
{	

			char strlog[256];
			sprintf(strlog,"����<%s>��ɫ<%s>ɾ����ħ",szGMServerName,szRoleName);
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
			g_ccNetObj.RegEvent_OnPacket(ENUM_PGPlayerControl_StoC_DelCharacterMob, DelCharacterMob, 0);

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
						int  iFlagID = Pal_GetMobFlag(iMobID);	// ʹ�ù���������ѯ�����

						PGPlayerControl_CtoS_DelCharacterMob sPkt;
						sPkt.iWorldID	= iWorldID;
						sprintf(sPkt.szRoleName, "%s", szRoleName);
						sPkt.iMobID		= iMobID;
						sPkt.iFlagID	= iFlagID;

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
void CDelCharacterMob::LoginResult(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
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
void CDelCharacterMob::DelCharacterMob(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
{

	PGPlayerControl_StoC_DelCharacterMob  *pPkt = (PGPlayerControl_StoC_DelCharacterMob *)pData;
	char strInt[10];
	char bufstr[256];
	switch(pPkt->emResult)
	{
	case ENUM_DelCharacterMobResult_Success:		
		printf("DelCharacterMob success\n");
		printf("WorldID: %d\n"	, pPkt->iWorldID);
		printf("RoleName: %s\n"	, pPkt->szRoleName);
		printf("MobID: %d\n"	, pPkt->iMobID);
		
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"ɾ����ɫ��ħ�ɹ�:�����%i,��ɫ��:%s,���:%i\n", pPkt->iWorldID,pPkt->szRoleName,pPkt->iMobID);
		
		break;
	case ENUM_DelCharacterMobResult_WorldNotFound:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"ɾ����ɫ��ħ���:�����û���ҵ�\n");
		printf("DelCharacterMob result: World not found\n");
		break;	
	case ENUM_DelCharacterMobResult_LeaderNotFound:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"ɾ����ɫ��ħ���: �۲���û���ҵ�\n");
		printf("DelCharacterMob result: Leader not found\n");
		break;
	case ENUM_DelCharacterMobResult_LeaderDisconnect:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"ɾ����ɫ��ħ���: �۲��߶Ͽ�\n");
		printf("DelCharacterMob result: Leader disconnect\n");
		break;	
	case ENUM_DelCharacterMobResult_RoleNotFound:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"ɾ����ɫ��ħ���: ��ɫ %s û���ҵ�\n", pPkt->szRoleName);
		printf("DelCharacterMob result: Character %s not found\n", pPkt->szRoleName);
		break;	
	case ENUM_DelCharacterMobResult_RoleIsOnline:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"ɾ����ɫ��ħ���: ��ɫ %s ��������\n", pPkt->szRoleName);
		printf("DelCharacterMob result: Character %s is online now\n", pPkt->szRoleName);
		break;	
	case ENUM_DelCharacterMobResult_MobNotFound:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"ɾ����ɫ��ħ���: ��ɫ %s ��û���������ħ\n", pPkt->szRoleName);
		printf("DelCharacterMob result: Character %s have not obtained this mob yet\n", pPkt->szRoleName);
		break;	
	case ENUM_DelCharacterMobResult_FlagError:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"ɾ����ɫ��ħ���: ���û���ҵ�\n");
		printf("DelCharacterMob result: Flag error\n");
		break;	
	case ENUM_DelCharacterMobResult_GMOOperationFailed:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"ɾ����ɫ��ħ���: Զ�̷������ʧ��\n");
		printf("DelCharacterMob result: Operation failed from GMObserver\n");
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
