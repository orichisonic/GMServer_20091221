// CharacterInfo.cpp: implementation of the CCharacterInfo class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "CharacterInfo.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCharacterInfo::CCharacterInfo()
{

}

CCharacterInfo::~CCharacterInfo()
{

}
vector <CGlobalStruct::TFLV> CCharacterInfo::CharacterInfoMain(char *szGMServerName, char * szGMServerIP,char* szRoleName)
{
	CGlobalStruct::TFLV m_tflv;
	char strlog[80];
	sprintf(strlog,"����<%s>��ɫ<%s>��ѯ��ɫ������Ϣ",szGMServerName,szRoleName);
	logFile.WriteText("�ɽ�OL",strlog);

	///new adding //////////////////
	// ����DBF���Բ�ѯ��������(ָ��"��ϷĿ¼\Kernel\\data")
	Pal_LoadData(".\\data");

	// ע������CallBack��ʽ
	g_ccNetObj.RegEvent_OnConnected(COperPal::OnConnect, 0);
	g_ccNetObj.RegEvent_OnDisconnected(COperPal::OnDisconnect, 0);
	g_ccNetObj.RegEvent_OnConnectFailed(COperPal::OnConnectfailed, 0);

	// ע���Ӧ�¼�CallBack��ʽ
	g_ccNetObj.RegEvent_OnPacket(ENUM_PGGMCConnection_StoC_LoginResult,	LoginResult, 0);
	g_ccNetObj.RegEvent_OnPacket(ENUM_PGPlayerStatus_StoC_CharacterInfo, CharacterInfo, 0);

	// ��ʼ��·�趨
	g_ccNetObj.SetConnectCount(10);		// �趨�������ߴ���Ϊ10
	g_ccNetObj.SetReConnect(false);		// �趨���ߵ�ʱ�򲻻���������
	g_ccNetObj.SetShowIP(true);			// �趨����ʱ����ʾIP

	Sleep(100);
	g_state = 0;

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
				//int  iWorldID = 0;							// ������
				//char szRoleName[32] = "רҵ��ʽ���ʦ";		// ��ɫ����
				iWorldID=operPal.FindWorldID(szGMServerName,szGMServerIP);
				PGPlayerStatus_CtoS_CharacterInfo sPkt;
				sPkt.iWorldID = iWorldID;
				sprintf(sPkt.szRoleName, "%s", szRoleName);

				// ������GMSҪ��ȡ�ý�ɫ������Ѷ
				g_ccNetObj.Send(sizeof(sPkt), (S_ObjNetPktBase*)&sPkt);	
				g_state = 3;			
			}
			break;
			// �ȴ�ȡ�ý�ɫ������Ѷ
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


void CCharacterInfo::LoginResult(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
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

// ȡ�ý�ɫ������Ѷ���-------------------------------------------------------------------------------
void CCharacterInfo::CharacterInfo(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
{
	PGPlayerStatus_StoC_CharacterInfo *pPkt = (PGPlayerStatus_StoC_CharacterInfo *)pData;
	char strInt[10];
	char bufstr[256];
	CGlobalStruct::TFLV m_tflv;
	switch(pPkt->emResult)
	{
	case ENUM_CharacterInfoResult_Success:
		{
			char szTemp[32];
			int  dwReslut[1024];		// ����ƺž�����Ѷ
			
			// ȡ�óƺž�����Ѷ
			Pal_GetMobFlagResult(dwReslut, pPkt->sGMRoleInfo.dwTitleFlag);

			// �趨ְҵ���Ա�����
			char szVoc[32];
			char szSex[32];
			switch (pPkt->sGMRoleInfo.emVoc)
			{
			case ENUM_Vocation_1:
				sprintf(szVoc, "����");
				break;
			case ENUM_Vocation_2:
				sprintf(szVoc, "���ߵ���");
				break;
			case ENUM_Vocation_3:
				sprintf(szVoc, "��������");
				break;
			case ENUM_Vocation_4:
				sprintf(szVoc, "����ʦ");
				break;
			case ENUM_Vocation_5:
				sprintf(szVoc, "���ҹ��");
				break;
			case ENUM_Vocation_6:
				sprintf(szVoc, "����ʿ");
				break;
			default:
				sprintf(szVoc, "");
				break;
			}	
			if (pPkt->sGMRoleInfo.iSex) sprintf(szSex, "��");
			else sprintf(szSex, "Ů");
			
			time_t t = pPkt->iLoginTime;
			struct tm *local = localtime(&t);
			char nowtime[40];
			sprintf(nowtime,"%d-%d-%d %d:%d:%d",local->tm_year+1900,local->tm_mon+1,local->tm_mday,local->tm_hour,local->tm_min,local->tm_sec);

			// ������
			printf("\n====================================================\n");
			printf("World: %d\n",					pPkt->iWorldID);
			printf("Account: %s\n",					pPkt->szAccount);
			printf("RoleName: %s\n",				pPkt->szRoleName);
			printf("DBID: %d\n",					pPkt->iDBID);
			printf("ZoneID: %d\n",					pPkt->iZoneID);

			// ��Ϊ���ִ�(��ɫ��������)���ѯ��������
			if (strlen(pPkt->szZoneName) == 0)
				printf("ZoneName: %s\n",			Pal_GetZoneName(pPkt->iZoneID));
			else
				printf("ZoneName: %s\n",			pPkt->szZoneName);
			
			printf("LoginTime: %s",					asctime(local));
			printf("IP: %s\n",						pPkt->szIP);
			
			printf("\n[ ��ɫ������Ѷ ]\n");
			printf("����X: %d\n",					pPkt->sGMRoleInfo.X);
			printf("����Y: %d\n",					pPkt->sGMRoleInfo.Y);
			printf("����Z: %d\n",					pPkt->sGMRoleInfo.Z);
			printf("�ǳ�: %s\n",					pPkt->sGMRoleInfo.szNickName);
			printf("��������: %s\n",				pPkt->sGMRoleInfo.szGuildName);
			printf("����ְ��: %d\n",				pPkt->sGMRoleInfo.iGuildPowerLevel);
			printf("ְҵ: %s\n",					szVoc);
			printf("�ȼ�: %d\n",					pPkt->sGMRoleInfo.iLevel);
			printf("Ŀǰ����: %d\n",				pPkt->sGMRoleInfo.iExp);
			printf("���Ͻ�Ǯ: %d\n",				pPkt->sGMRoleInfo.iBodyMoney);
			printf("�ֿ��Ǯ: %d\n\n",				pPkt->sGMRoleInfo.iBankMoney);
			
			printf("�Ա�: %s\n",					szSex);
			printf("����: %d\n",					pPkt->sGMRoleInfo.iFame);
			printf("ĿǰѪ��: %d\n",				pPkt->sGMRoleInfo.iHP);
			printf("Ŀǰ����: %d\n",				pPkt->sGMRoleInfo.iMP);
			printf("Ŀǰ����: %d\n",				pPkt->sGMRoleInfo.iAP);
			printf("Ŀǰ����: %d\n",				pPkt->sGMRoleInfo.iEP);
			printf("�����ü��ܵ���: %d\n",			pPkt->sGMRoleInfo.iSkillPoint);
			printf("Ŀǰ�������ߵ���λ��: %d\n",	pPkt->sGMRoleInfo.iSkillTreeNode);
			printf("ת������: %d\n",				pPkt->sGMRoleInfo.iReincarnation);
			
			if (pPkt->sGMRoleInfo.iTitleID != -1)
			{
				sprintf(szTemp, Pal_GetTitleName(pPkt->sGMRoleInfo.iTitleID));
				// ���鲻������
				if (strlen(szTemp) == 0) sprintf(szTemp, "Unknow Name");
			}
			else
			{
				sprintf(szTemp, "��ʹ�óƺ�");
			}
			printf("Ŀǰʹ�óƺ�: %s\n", szTemp);
			printf("��ȡ�óƺ��б�: \n");
			printf("===== Start ===== \n");
			for (int i=0; i<1024; i++)
			{
				// ���ƺ���꿪��
				if (dwReslut[i] == 1)
				{
					sprintf(szTemp, Pal_GetTitleName(i));	// ȡ�óƺ�����
					// ���鲻������
					if (strlen(szTemp) == 0) sprintf(szTemp, "Unknow Name");
					printf("  %s\n", szTemp);
				}// if
			}// for
			printf("====== End ====== \n\n");
			printf("а��ֵ: %d\n",	pPkt->sGMRoleInfo.iMurdererCount);
			printf("��ҵֵ: %d\n",	pPkt->sGMRoleInfo.iEvilCount);
			printf("����: %d\n",	pPkt->sGMRoleInfo.iWushu);
			printf("����: %d\n",	pPkt->sGMRoleInfo.iConstitution);
			printf("����: %d\n",	pPkt->sGMRoleInfo.iSpirit);
			printf("����: %d\n",	pPkt->sGMRoleInfo.iQuality);
			printf("��: %d\n",	pPkt->sGMRoleInfo.iDexterity);
			printf("����: %d\n",	pPkt->sGMRoleInfo.iFate);
		


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

			
			_itoa(pPkt->iDBID,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_ROLEID;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);



			_itoa(pPkt->iZoneID,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_ZONEID;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);



			if (strlen(pPkt->szZoneName) == 0)
			{
				m_tflv.nIndex=DBVect.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::PAL_ZONENAME;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(Pal_GetZoneName(pPkt->iZoneID));
				sprintf((LPSTR)&m_tflv.lpdata,"%s",Pal_GetZoneName(pPkt->iZoneID));
				DBVect.push_back(m_tflv);
			}
			else
			{
				m_tflv.nIndex=DBVect.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::PAL_ZONENAME;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(pPkt->szZoneName);
				sprintf((LPSTR)&m_tflv.lpdata,"%s",pPkt->szZoneName);
				DBVect.push_back(m_tflv);
			}




			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_LOGINTIME;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(nowtime);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",nowtime);
			DBVect.push_back(m_tflv);

			
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_LOGINIP;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(pPkt->szIP);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",pPkt->szIP);
			DBVect.push_back(m_tflv);

			
			_itoa(pPkt->sGMRoleInfo.X,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_X;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);

			_itoa(pPkt->sGMRoleInfo.Y,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_Y;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);

			_itoa(pPkt->sGMRoleInfo.Z,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_Z;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);


			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_NICKNAME;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(pPkt->sGMRoleInfo.szNickName);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",pPkt->sGMRoleInfo.szNickName);
			DBVect.push_back(m_tflv);

			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_GUILDNAME;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(pPkt->sGMRoleInfo.szGuildName);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",pPkt->sGMRoleInfo.szGuildName);
			DBVect.push_back(m_tflv);

			
			_itoa(pPkt->sGMRoleInfo.iGuildPowerLevel,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_GUILDPOWERLEVEL;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);


			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_VOC;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(szVoc);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",szVoc);
			DBVect.push_back(m_tflv);

			_itoa(pPkt->sGMRoleInfo.iLevel,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_LEVEL;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);

			_itoa(pPkt->sGMRoleInfo.iExp,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_EXP;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);

			_itoa(pPkt->sGMRoleInfo.iBodyMoney,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_BODYMONEY;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);

			_itoa(pPkt->sGMRoleInfo.iBankMoney,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_BANKMONEY;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);

			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_SEX;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(szSex);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",szSex);
			DBVect.push_back(m_tflv);

			_itoa(pPkt->sGMRoleInfo.iFame,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_FAME;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);
			
			_itoa(pPkt->sGMRoleInfo.iHP,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_HP;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);

			_itoa(pPkt->sGMRoleInfo.iMP,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_MP;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);

			_itoa(pPkt->sGMRoleInfo.iAP,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_AP;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);

			_itoa(pPkt->sGMRoleInfo.iEP,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_EP;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);

			_itoa(pPkt->sGMRoleInfo.iSkillPoint,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_SKILLPOINT;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);

			_itoa(pPkt->sGMRoleInfo.iSkillTreeNode,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_SKILLTREENODE;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);

			_itoa(pPkt->sGMRoleInfo.iReincarnation,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_REINCARNATION;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);
			
			_itoa(pPkt->sGMRoleInfo.iMurdererCount,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_MurdererCount;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);
			
			_itoa(pPkt->sGMRoleInfo.iEvilCount,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_EvilCount;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);
			
			_itoa(pPkt->sGMRoleInfo.iWushu,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_Wushu;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);
			
			_itoa(pPkt->sGMRoleInfo.iConstitution,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_Constitution;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);
			
			_itoa(pPkt->sGMRoleInfo.iSpirit,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_Spirit;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);
			
			_itoa(pPkt->sGMRoleInfo.iQuality,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_Quality;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);
			
			_itoa(pPkt->sGMRoleInfo.iDexterity,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_Dexterity;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);
			
			_itoa(pPkt->sGMRoleInfo.iFate,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_Fate;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);
		}
		break;

	case ENUM_CharacterInfoResult_RoleNotFound:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"��ɫ��Ϣ���: ��ɫ %s û���ҵ�\n", pPkt->szRoleName);
		printf("Charinfo result: Character %s not found\n", pPkt->szRoleName);
		m_tflv.nIndex=DBVect.size()+1;
		m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
		m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
		m_tflv.m_tvlength=strlen(bufstr);
		sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
		DBVect.push_back(m_tflv);
		break;

	case ENUM_CharacterInfoResult_WorldNotFound:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"��ɫ��Ϣ���: ����û���ҵ�\n");
		printf("Charinfo result: World not found\n");
		m_tflv.nIndex=DBVect.size()+1;
		m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
		m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
		m_tflv.m_tvlength=strlen(bufstr);
		sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
		DBVect.push_back(m_tflv);
		break;

	case ENUM_CharacterInfoResult_LeaderNotFound:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"��ɫ��Ϣ���: Ƶ��û���ҵ�\n");
		printf("Charinfo result: Leader not found\n");
		m_tflv.nIndex=DBVect.size()+1;
		m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
		m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
		m_tflv.m_tvlength=strlen(bufstr);
		sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
		DBVect.push_back(m_tflv);
		break;

	case ENUM_CharacterInfoResult_LeaderDisconnect:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"��ɫ��Ϣ���: Ƶ��û������\n");
		printf("Charinfo result: Leader disconnect\n");
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
