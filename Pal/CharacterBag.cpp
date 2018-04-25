// CharacterBag.cpp: implementation of the CCharacterBag class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "CharacterBag.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCharacterBag::CCharacterBag()
{

}

CCharacterBag::~CCharacterBag()
{

}
vector <CGlobalStruct::TFLV> CCharacterBag::CharacterBagMain(char *szGMServerName, char * szGMServerIP,char* szRoleName, int iBagType)
{

			// ����DBF���Բ�ѯ��������(ָ��"��ϷĿ¼\Kernel\\data")
			Pal_LoadData(".\\data");
		
			// ע������CallBack��ʽ
			g_ccNetObj.RegEvent_OnConnected(COperPal::OnConnect, 0);
			g_ccNetObj.RegEvent_OnDisconnected(COperPal::OnDisconnect, 0);
			g_ccNetObj.RegEvent_OnConnectFailed(COperPal::OnConnectfailed, 0);
		
			// ע���Ӧ�¼�CallBack��ʽ
			g_ccNetObj.RegEvent_OnPacket(ENUM_PGGMCConnection_StoC_LoginResult,	LoginResult, 0);
			g_ccNetObj.RegEvent_OnPacket(ENUM_PGPlayerStatus_StoC_CharacterBag, CharacterBag, 0);
	
			// ��ʼ��·�趨
			g_ccNetObj.SetConnectCount(10);		// �趨�������ߴ���Ϊ10
			g_ccNetObj.SetReConnect(false);		// �趨���ߵ�ʱ�򲻻���������
			g_ccNetObj.SetShowIP(true);			// �趨����ʱ����ʾIP


			CGlobalStruct::TFLV m_tflv;
			if(!DBVect.empty())
			{
				DBVect.clear();
			}

			
			Sleep(100);
			g_state = 0;


			int n_login=0,n_send=0,login_num=0,send_num=0;
			operPal.GetLogSendNum(&login_num,&send_num);//�������ļ��л�õ�¼�����ͷ��ʹ���

			char szAccount[32];
			char szPassword[32];
			ZeroMemory(szAccount,32);
			ZeroMemory(szPassword,32);
		
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
						operPal.GetUserNamePwd("User1",szAccount,szPassword,&iGMServerPort);///�������ļ��л�õ�¼�ӿڵ��û���������

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


						PGPlayerStatus_CtoS_CharacterBag sPkt;
						sPkt.iWorldID = iWorldID;
						sprintf(sPkt.szRoleName, "%s", szRoleName);
						sPkt.iBagType = iBagType;

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
void CCharacterBag::LoginResult(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
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
void CCharacterBag::CharacterBag(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
{
			PGPlayerStatus_StoC_CharacterBag *pPkt = (PGPlayerStatus_StoC_CharacterBag *)pData;
			CGlobalStruct::TFLV m_tflv;

			char strInt[10];
			char bufstr[256];
			switch(pPkt->emResult)
			{
			case ENUM_CharacterBagResult_Success:
				{
					// ��Ϊ�������
					if (pPkt->iWorldID == -1)
					{
						g_state = -1;
						return;
					}

					// �趨������������
					char szBagType[10];
					switch (pPkt->iBagType)
					{
					case 0:
						sprintf(szBagType, "����");
						break;
					case 1:
						sprintf(szBagType, "�����");
						break;
					case 2:
						sprintf(szBagType, "PK��������");
						break;
					case 3:
						sprintf(szBagType, "����");
						break;
					default:
						sprintf(szBagType, "����");
					}

					time_t t = pPkt->sGMItemData.iCreateTime;
					struct tm *local = localtime(&t);

					char nowtime[40];
					sprintf(nowtime,"%d-%d-%d %d:%d:%d",local->tm_year+1900,local->tm_mon+1,local->tm_mday,local->tm_hour,local->tm_min,local->tm_sec);


					// ������
					printf("\n====================================================\n");
					printf("World: %d\n",				pPkt->iWorldID);
					printf("Account: %s\n",				pPkt->szAccount);
					printf("RoleName: %s\n",			pPkt->szRoleName);
					printf("��������: %s\n",			szBagType);
					printf("\n[ ���һ������ ]\n");
					printf("����: %s\n",				pPkt->sGMItemData.szName);
					printf("λ��: %d\n",				pPkt->sGMItemData.iPos);
					printf("�������: %d\n",			pPkt->sGMItemData.iItemType);
					printf("����ID: %d\n",				pPkt->sGMItemData.iOrgObjID);
					printf("����ʱ��: %s",				asctime(local));
					printf("���: %d\n",				pPkt->sGMItemData.iSerial);
					printf("����ʱ��: %d\n",			pPkt->sGMItemData.iLiveTime);
					printf("�´ο���ʹ�õ�ʱ��: %d\n",	pPkt->sGMItemData.iCoolDown/1000);

					if (pPkt->sGMItemData.bNPCTrade) printf("�Ƿ���Ը�NPC����: ��\n");
					else printf("�Ƿ���Ը�NPC����: ����\n");
					if (pPkt->sGMItemData.bUserTrade) printf("�Ƿ���Ը���ҽ���: ��\n");
					else printf("�Ƿ���Ը���ҽ���: ����\n");
					if (pPkt->sGMItemData.bItemMallGoods) printf("�Ƿ�Ϊ�̳���Ʒ: ��\n");
					else printf("�Ƿ�Ϊ�̳���Ʒ: ����\n");	

					printf("����: %d\n",				pPkt->sGMItemData.iCount);
					printf("����: %d\n",				pPkt->sGMItemData.iSellCost);
					printf("Ŀǰ�;ö�: %d\n",			pPkt->sGMItemData.wDurable);

					// ������Ϊһ����Ʒ
					if (pPkt->sGMItemData.iItemType == 0)
					{
						printf("\n[ ��ʯ��ϸ���� ]\n");
						printf("����ֵ(����): %d %s\n",	pPkt->sGMItemData.iInheritID[0], Pal_GetItemName(pPkt->sGMItemData.iInheritID[0]));
						printf("��ħ: %d %s\n",			pPkt->sGMItemData.iInheritID[1], Pal_GetItemName(pPkt->sGMItemData.iInheritID[1]));
						printf("������: %d %s\n",		pPkt->sGMItemData.iInheritID[2], Pal_GetItemName(pPkt->sGMItemData.iInheritID[2]));
						printf("�巨��: %d %s\n",		pPkt->sGMItemData.iInheritID[3], Pal_GetItemName(pPkt->sGMItemData.iInheritID[3]));
						printf("��ʯ1: %d %s\n",		pPkt->sGMItemData.iInheritID[4], Pal_GetItemName(pPkt->sGMItemData.iInheritID[4]));
						printf("��ʯ2: %d %s\n",		pPkt->sGMItemData.iInheritID[5], Pal_GetItemName(pPkt->sGMItemData.iInheritID[5]));
						printf("��ʯ3: %d %s\n",		pPkt->sGMItemData.iInheritID[6], Pal_GetItemName(pPkt->sGMItemData.iInheritID[6]));
						printf("��ʯ4: %d %s\n",		pPkt->sGMItemData.iInheritID[7], Pal_GetItemName(pPkt->sGMItemData.iInheritID[7]));
						printf("��ʯ5: %d %s\n",		pPkt->sGMItemData.iInheritID[8], Pal_GetItemName(pPkt->sGMItemData.iInheritID[8]));
						printf("��ʯ6: %d %s\n",		pPkt->sGMItemData.iInheritID[9], Pal_GetItemName(pPkt->sGMItemData.iInheritID[9]));
					}
					// ������Ϊ����
					else
						if (pPkt->sGMItemData.iItemType == 1)
						{
							printf("\n[ ������ϸ���� ]\n");
							printf("����ֵ: %d\n",			pPkt->sGMItemData.sPet.iExp);
							printf("ĿǰHPֵ: %d\n",		pPkt->sGMItemData.sPet.iNowHP);
							printf("�����������: %d (%s)\n", pPkt->sGMItemData.sPet.iPetIndex, Pal_GetPetName(pPkt->sGMItemData.sPet.iPetIndex));
							printf("��ͯ����: %d\n",		pPkt->sGMItemData.sPet.iRenewCount);
							printf("�ȼ�: %d\n",			pPkt->sGMItemData.sPet.iLV);
							printf("����: %d\n",			pPkt->sGMItemData.sPet.iGeneration);
							printf("�ϳɴ���: %d\n",		pPkt->sGMItemData.sPet.iMixCount);
							printf("�ɳ���: %d\n",			pPkt->sGMItemData.sPet.iGrowthRate);
							printf("Ʒ��: %d\n\n",			pPkt->sGMItemData.sPet.iAttrQuality);

							printf("HP�ӳ�ֵ: %d\n",		pPkt->sGMItemData.sPet.iHP);
							printf("����: %d\n",			pPkt->sGMItemData.sPet.iWushu);
							printf("����: %d\n",			pPkt->sGMItemData.sPet.iSpirit);
							printf("����: %d\n",			pPkt->sGMItemData.sPet.iConstitution);
							printf("����: %d\n",			pPkt->sGMItemData.sPet.iQuality);
							printf("��: %d\n",			pPkt->sGMItemData.sPet.iDexterity);
							printf("����: %d\n",			pPkt->sGMItemData.sPet.iFate);
							printf("HPֵ�۸�: %d\n",		pPkt->sGMItemData.sPet.iHP_Plus);
							printf("�����۸�: %d\n",		pPkt->sGMItemData.sPet.iWushu_Plus);
							printf("���Ի۸�: %d\n",		pPkt->sGMItemData.sPet.iSpirit_Plus);
							printf("�����۸�: %d\n",		pPkt->sGMItemData.sPet.iConstitution_Plus);
							printf("�����۸�: %d\n",		pPkt->sGMItemData.sPet.iQuality_Plus);
							printf("���۸�: %d\n",		pPkt->sGMItemData.sPet.iDexterity_Plus);
							printf("���˻۸�: %d\n\n",		pPkt->sGMItemData.sPet.iFate_Plus);

							//Pal_GetPetSkillName(pPkt->sGMItemData.sPet.sSkill[i])

							printf("����1: %d\n",			Pal_GetPetSkillName(pPkt->sGMItemData.sPet.sSkill[0]));
							printf("����2: %d\n",			Pal_GetPetSkillName(pPkt->sGMItemData.sPet.sSkill[1]));
							printf("����3: %d\n",			Pal_GetPetSkillName(pPkt->sGMItemData.sPet.sSkill[2]));
							printf("����4: %d\n",			Pal_GetPetSkillName(pPkt->sGMItemData.sPet.sSkill[3]));
							printf("����5: %d\n",			Pal_GetPetSkillName(pPkt->sGMItemData.sPet.sSkill[4]));
							printf("����6: %d\n",			Pal_GetPetSkillName(pPkt->sGMItemData.sPet.sSkill[5]));
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

					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_BAGTYPE;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=strlen(szBagType);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",szBagType);
					DBVect.push_back(m_tflv);

					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_ITEMNAME;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=strlen(pPkt->sGMItemData.szName);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",pPkt->sGMItemData.szName);
					DBVect.push_back(m_tflv);
					
					_itoa(pPkt->sGMItemData.iPos,strInt,10);
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_ITEMPOS;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);
					
					if (pPkt->sGMItemData.iItemType == 0)
					{
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_ITEMTYPE;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen("һ����Ʒ");
						sprintf((LPSTR)&m_tflv.lpdata,"%s","һ����Ʒ");
						DBVect.push_back(m_tflv);
					}
					else
					{
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_ITEMTYPE;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen("����");
						sprintf((LPSTR)&m_tflv.lpdata,"%s","����");
						DBVect.push_back(m_tflv);
					}

					_itoa(pPkt->sGMItemData.iOrgObjID,strInt,10);
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_ORGOBJID;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);
					

					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_LOCALTIME;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=strlen(nowtime);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",nowtime);
					DBVect.push_back(m_tflv);

					_itoa(pPkt->sGMItemData.iSerial,strInt,10);
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_ITEMSERIAL;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);

					_itoa(pPkt->sGMItemData.iLiveTime,strInt,10);
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_LIVETIME;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);
				
					if (pPkt->sGMItemData.bNPCTrade) 
					{
						_itoa(1,strInt,10);
					}
					else
					{
						_itoa(0,strInt,10);
					}
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_NPCTRADE;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);

					if (pPkt->sGMItemData.bUserTrade) 
					{
						_itoa(1,strInt,10);
					}
					else
					{
						_itoa(0,strInt,10);
					}
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_USERTRADE;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);

					if (pPkt->sGMItemData.bItemMallGoods) 
					{
						_itoa(1,strInt,10);
					}
					else
					{
						_itoa(0,strInt,10);
					}
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_ITEMMALLGOODS;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);


					_itoa(pPkt->sGMItemData.iCount,strInt,10);
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_ITEMCOUNT;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);

					_itoa(pPkt->sGMItemData.wDurable,strInt,10);
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_ITEMDURABLE;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);

					if (pPkt->sGMItemData.iItemType == 0)
					{
						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%i%s",pPkt->sGMItemData.iInheritID[0], Pal_GetItemName(pPkt->sGMItemData.iInheritID[0]));
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_INHERITID0;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%i%s",pPkt->sGMItemData.iInheritID[1], Pal_GetItemName(pPkt->sGMItemData.iInheritID[1]));
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_INHERITID1;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);
						
						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%i%s",pPkt->sGMItemData.iInheritID[2], Pal_GetItemName(pPkt->sGMItemData.iInheritID[2]));
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_INHERITID2;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%i%s",pPkt->sGMItemData.iInheritID[3], Pal_GetItemName(pPkt->sGMItemData.iInheritID[3]));
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_INHERITID3;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%i%s",pPkt->sGMItemData.iInheritID[4], Pal_GetItemName(pPkt->sGMItemData.iInheritID[4]));
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_INHERITID4;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%i%s",pPkt->sGMItemData.iInheritID[5], Pal_GetItemName(pPkt->sGMItemData.iInheritID[5]));
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_INHERITID5;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);
				
						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%i%s",pPkt->sGMItemData.iInheritID[6], Pal_GetItemName(pPkt->sGMItemData.iInheritID[6]));
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_INHERITID6;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%i%s",pPkt->sGMItemData.iInheritID[7], Pal_GetItemName(pPkt->sGMItemData.iInheritID[7]));
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_INHERITID7;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%i%s",pPkt->sGMItemData.iInheritID[8], Pal_GetItemName(pPkt->sGMItemData.iInheritID[8]));
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_INHERITID8;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%i%s",pPkt->sGMItemData.iInheritID[9], Pal_GetItemName(pPkt->sGMItemData.iInheritID[9]));
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_INHERITID9;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						/////////û�еĻ��ַ���-1///////////////////
						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETEXP;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETNOWHP;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETINDEX;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETRENEWCOUNT;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETLV;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETGENERATION;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETMIXCOUNT;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETGROWTHRATE;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETATTRQUALITY;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETHP;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETWUSHU;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETSPIRIT;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETCONSTITUTION;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETQUALITY;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETDEXTERITY;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETFATE;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETHPPLUS;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETWUSHUPLUS;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETSPIRITPLUS;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETCONSTITUTIONPLUS;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETQUALITYPLUS;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETDEXTERITYPLUS;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETFATEPLUS;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETSKILL0;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETSKILL1;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETSKILL2;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETSKILL3;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETSKILL4;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETSKILL5;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);
					}
					else if (pPkt->sGMItemData.iItemType == 1)
					{
						/////////û�еĻ��ַ���-1///////////////////
						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_INHERITID0;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_INHERITID1;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_INHERITID2;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_INHERITID3;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_INHERITID4;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_INHERITID5;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_INHERITID6;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_INHERITID7;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_INHERITID8;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_INHERITID9;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						//////////////////////////////////////////////
						_itoa(pPkt->sGMItemData.sPet.iExp,strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETEXP;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						DBVect.push_back(m_tflv);

						_itoa(pPkt->sGMItemData.sPet.iNowHP,strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETNOWHP;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%i(%s)",pPkt->sGMItemData.sPet.iPetIndex, Pal_GetItemName(pPkt->sGMItemData.sPet.iPetIndex));
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETINDEX;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						_itoa(pPkt->sGMItemData.sPet.iRenewCount,strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETRENEWCOUNT;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						DBVect.push_back(m_tflv);

						_itoa(pPkt->sGMItemData.sPet.iLV,strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETLV;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						DBVect.push_back(m_tflv);

						_itoa(pPkt->sGMItemData.sPet.iGeneration,strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETGENERATION;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						DBVect.push_back(m_tflv);

						_itoa(pPkt->sGMItemData.sPet.iMixCount,strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETMIXCOUNT;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						DBVect.push_back(m_tflv);

						_itoa(pPkt->sGMItemData.sPet.iGrowthRate,strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETGROWTHRATE;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						DBVect.push_back(m_tflv);

						_itoa(pPkt->sGMItemData.sPet.iAttrQuality,strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETATTRQUALITY;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						DBVect.push_back(m_tflv);

						_itoa(pPkt->sGMItemData.sPet.iHP,strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETHP;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						DBVect.push_back(m_tflv);

						_itoa(pPkt->sGMItemData.sPet.iWushu,strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETWUSHU;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						DBVect.push_back(m_tflv);

						_itoa(pPkt->sGMItemData.sPet.iSpirit,strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETSPIRIT;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						DBVect.push_back(m_tflv);

						_itoa(pPkt->sGMItemData.sPet.iConstitution,strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETCONSTITUTION;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						DBVect.push_back(m_tflv);

						_itoa(pPkt->sGMItemData.sPet.iQuality,strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETQUALITY;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						DBVect.push_back(m_tflv);

						_itoa(pPkt->sGMItemData.sPet.iDexterity,strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETDEXTERITY;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						DBVect.push_back(m_tflv);

						_itoa(pPkt->sGMItemData.sPet.iFate,strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETFATE;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						DBVect.push_back(m_tflv);

						_itoa(pPkt->sGMItemData.sPet.iHP_Plus,strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETHPPLUS;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						DBVect.push_back(m_tflv);

						_itoa(pPkt->sGMItemData.sPet.iWushu_Plus,strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETWUSHUPLUS;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						DBVect.push_back(m_tflv);

						_itoa(pPkt->sGMItemData.sPet.iSpirit_Plus,strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETSPIRITPLUS;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						DBVect.push_back(m_tflv);

						_itoa(pPkt->sGMItemData.sPet.iConstitution_Plus,strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETCONSTITUTIONPLUS;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						DBVect.push_back(m_tflv);

						_itoa(pPkt->sGMItemData.sPet.iQuality_Plus,strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETQUALITYPLUS;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						DBVect.push_back(m_tflv);

						_itoa(pPkt->sGMItemData.sPet.iDexterity_Plus,strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETDEXTERITYPLUS;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						DBVect.push_back(m_tflv);

						_itoa(pPkt->sGMItemData.sPet.iFate_Plus,strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETFATEPLUS;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						DBVect.push_back(m_tflv);

						//_itoa(pPkt->sGMItemData.sPet.sSkill[0],strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETSKILL0;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",Pal_GetPetSkillName(pPkt->sGMItemData.sPet.sSkill[0]));//strInt);
						DBVect.push_back(m_tflv);

						//_itoa(pPkt->sGMItemData.sPet.sSkill[1],strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETSKILL1;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",Pal_GetPetSkillName(pPkt->sGMItemData.sPet.sSkill[1]));//strInt);
						DBVect.push_back(m_tflv);

						//_itoa(pPkt->sGMItemData.sPet.sSkill[2],strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETSKILL2;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",Pal_GetPetSkillName(pPkt->sGMItemData.sPet.sSkill[2]));//strInt);
						DBVect.push_back(m_tflv);

						//_itoa(pPkt->sGMItemData.sPet.sSkill[3],strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETSKILL3;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",Pal_GetPetSkillName(pPkt->sGMItemData.sPet.sSkill[3]));
						DBVect.push_back(m_tflv);

						//_itoa(pPkt->sGMItemData.sPet.sSkill[4],strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETSKILL4;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",Pal_GetPetSkillName(pPkt->sGMItemData.sPet.sSkill[4]));
						DBVect.push_back(m_tflv);

						//_itoa(pPkt->sGMItemData.sPet.sSkill[5],strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETSKILL5;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",Pal_GetPetSkillName(pPkt->sGMItemData.sPet.sSkill[5]));
						DBVect.push_back(m_tflv);
					}
				}
				break;

			case ENUM_CharacterBagResult_TypeError:
				ZeroMemory(bufstr,256);
				sprintf(bufstr,"%s","��ɫ�������:���󱳰�����\n");
				printf("��ɫ�������:���󱳰�����\n");
				m_tflv.nIndex=DBVect.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(bufstr);
				sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
				DBVect.push_back(m_tflv);
				g_state = -1;
				break;

			case ENUM_CharacterBagResult_RoleNotFound:
				
				ZeroMemory(bufstr,256);
				sprintf(bufstr,"��ɫ�������: ��ɫ %s û���ҵ�\n", pPkt->szRoleName);
				printf("��ɫ�������: ��ɫ %s û���ҵ�\n", pPkt->szRoleName);	
				m_tflv.nIndex=DBVect.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(bufstr);
				sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
				DBVect.push_back(m_tflv);
				g_state = -1;
				break;

			case ENUM_CharacterBagResult_CharacterBagEmpty:
				ZeroMemory(bufstr,256);
				sprintf(bufstr,"��ɫ�������: %s�ı����ǿյ�\n", pPkt->szRoleName);
				printf("��ɫ�������: %s�ı����ǿյ�\n", pPkt->szRoleName);
				m_tflv.nIndex=DBVect.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(bufstr);
				sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
				DBVect.push_back(m_tflv);
				g_state = -1;
				break;

			case ENUM_CharacterBagResult_WorldNotFound:
				ZeroMemory(bufstr,256);
				sprintf(bufstr,"��ɫ�������:����û���ҵ�\n");
				printf("��ɫ�������:����û���ҵ�\n");
				m_tflv.nIndex=DBVect.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(bufstr);
				sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
				DBVect.push_back(m_tflv);
				g_state = -1;
				break;

			case ENUM_CharacterBagResult_LeaderNotFound:
				ZeroMemory(bufstr,256);
				sprintf(bufstr,"��ɫ�������: Ƶ��û���ҵ�\n");
				printf("��ɫ�������: Ƶ��û���ҵ�\n");
				m_tflv.nIndex=DBVect.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(bufstr);
				sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
				DBVect.push_back(m_tflv);
				g_state = -1;
				break;

			case ENUM_CharacterBagResult_LeaderDisconnect:
				ZeroMemory(bufstr,256);
				sprintf(bufstr,"��ɫ�������: Ƶ��û������\n");
				printf("��ɫ�������: Ƶ��û������\n");	
				m_tflv.nIndex=DBVect.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(bufstr);
				sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
				DBVect.push_back(m_tflv);
				g_state = -1;
				break;

			default:
				g_state = -1;
				break;
			}

}
