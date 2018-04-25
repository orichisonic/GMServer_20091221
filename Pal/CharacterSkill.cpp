// CharacterSkill.cpp: implementation of the CCharacterSkill class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CharacterSkill.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCharacterSkill::CCharacterSkill()
{

}

CCharacterSkill::~CCharacterSkill()
{

}
vector <CGlobalStruct::TFLV> CCharacterSkill::CharacterSkillMain(char *szGMServerName, char * szGMServerIP,char* szRoleName)
{	

			char strlog[80];
			sprintf(strlog,"大区<%s>角色<%s>查询角色技能",szGMServerName,szRoleName);
			logFile.WriteText("仙剑OL",strlog);
			CGlobalStruct::TFLV m_tflv;
			if(!DBVect.empty())
			{
				DBVect.clear();
			}
			// 注册连线CallBack函式
			g_ccNetObj.RegEvent_OnConnected(COperPal::OnConnect, 0);
			g_ccNetObj.RegEvent_OnDisconnected(COperPal::OnDisconnect, 0);
			g_ccNetObj.RegEvent_OnConnectFailed(COperPal::OnConnectfailed, 0);

			// 注册对应事件CallBack函式
			g_ccNetObj.RegEvent_OnPacket(ENUM_PGGMCConnection_StoC_LoginResult,	LoginResult, 0);
			g_ccNetObj.RegEvent_OnPacket(ENUM_PGPlayerStatus_StoC_CharacterSkill, CharacterSkill, 0);

			// 初始网路设定
			g_ccNetObj.SetConnectCount(10);		// 设定重新连线次数为10
			g_ccNetObj.SetReConnect(false);		// 设定断线的时候不会重新连线
			g_ccNetObj.SetShowIP(true);			// 设定连线时会显示IP

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

				// 依状态执行
				switch(g_state)
				{
					// 登入GMServer
				case 0:	
					{
						operPal.GetUserNamePwd("User2",szAccount,szPassword,&iGMServerPort);

						// 若与GMS连线成功
						if(g_ccNetObj.WaitConnect(szGMServerIP, iGMServerPort, 65535, 65535))
						{		
							// 取得本机IP
							struct in_addr localaddr;   
							struct hostent *hInfo = NULL;   
							char szHostname[50];     
							gethostname(szHostname,49);							// 主机名称   
							hInfo = gethostbyname(szHostname);					// 主机资讯   
							memcpy(&localaddr, hInfo->h_addr, hInfo->h_length);	// 主机IP   

							// 设定GMS登入参数
					
							char *szIP = inet_ntoa(localaddr);		// 登入IP	

							PGGMCConnection_CtoS_RequestLogin sPkt;	
							sprintf(sPkt.szAccount, "%s", szAccount);
							sprintf(sPkt.szPassword, "%s", szPassword);		
							memcpy(sPkt.szIP, szIP, sizeof(sPkt.szIP));  

							// 传送帐密登入GMS
							g_ccNetObj.Send(sizeof(sPkt), (S_ObjNetPktBase*) &sPkt);
							g_state = 1;
						}
						else
						{
							g_state=-1;
							m_tflv.nIndex=DBVect.size()+1;
							m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
							m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
							m_tflv.m_tvlength=strlen("连接错误");
							sprintf((LPSTR)&m_tflv.lpdata,"%s","连接错误");
							DBVect.push_back(m_tflv);		
						}

					}
					break;
					// 等待登入中
				case 1:
					n_login++;
					Sleep(100);
					if(n_login>login_num)
					{
						g_state=-1;
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen("登入超时");
						sprintf((LPSTR)&m_tflv.lpdata,"%s","登入超时");
						DBVect.push_back(m_tflv);
					}
					printf("服务器登入中,第%i次\n",n_login);
					break;
					// 登入成功
				case 2:
					{
						printf("登入成功\n\n");

						// 设定命令参数
						iWorldID=operPal.FindWorldID(szGMServerName,szGMServerIP);

						PGPlayerStatus_CtoS_CharacterSkill sPkt;
						sPkt.iWorldID = iWorldID;
						sprintf(sPkt.szRoleName, "%s", szRoleName);

						// 传送至GMS要求取得角色好友列表
						g_ccNetObj.Send(sizeof(sPkt), (S_ObjNetPktBase*)&sPkt);	
						g_state = 3;			
					}
					break;
					// 等待取得角色好友列表
				case 3:
					n_send++;
					Sleep(100);
					if(n_send>send_num)
					{
						g_state=-1;
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen("等待超时");
						sprintf((LPSTR)&m_tflv.lpdata,"%s","等待超时");
						DBVect.push_back(m_tflv);
					}

					printf("等待取得角色背包资讯中,第%i次\n",n_send);
					break;
				}
			}			
			g_ccNetObj.Disconnect();
			printf("\n");
			printf("\n==================== Shutdown ====================\n");
			return DBVect;


}
void CCharacterSkill::LoginResult(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
{

	char bufstr[256];
	PGGMCConnection_StoC_LoginResult *pPkt = (PGGMCConnection_StoC_LoginResult *)pData;
	switch(pPkt->emResult)
	{
	case ENUM_GMCLoginResult_Success:
		printf("登陆成功\n");
		g_state = 2;
		return;		
	case ENUM_GMCLoginResult_AccountFailed:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"登陆失败,帐号错误\n");
		printf(bufstr,"登陆失败,帐号错误\n");
		
		break;		
	case ENUM_GMCLoginResult_PasswordFailed:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"登陆失败,密码错误\n");
		printf("登陆失败,密码错误\n");
		break;		
	case ENUM_GMCLoginResult_IPFailed:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"登陆失败,IP地址错误\n");
		printf("登陆失败,IP地址错误\n");
		break;		
	default:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"登陆失败\n");
		printf(bufstr,"登陆失败\n");	
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


// 取得角色技能结果-------------------------------------------------------------------------------
void CCharacterSkill::CharacterSkill(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
{

		PGPlayerStatus_StoC_CharacterSkill *pPkt = (PGPlayerStatus_StoC_CharacterSkill *)pData;
		char strInt[10];
		char bufstr[256];
		switch(pPkt->emResult)
		{
		case ENUM_CharacterSkillResult_Success:
			{
				int iLevel;
				char szName[32];
				
				// 设定职业名称
				switch (pPkt->iVocType)
				{
				case 0:
					sprintf(szName, "剑侠");
					break;
				case 1:
					sprintf(szName, "灵蛇刀客");
					break;
				case 2:
					sprintf(szName, "蝶衣舞者");
					break;
				case 3:
					sprintf(szName, "五灵师");
					break;
				case 4:
					sprintf(szName, "灵狐夜隐");
					break;
				case 5:
					sprintf(szName, "练气士");
					break;
				default:
					sprintf(szName, "");
					break;
				}
				
				// 输出结果
				printf("\n====================================================\n");
				printf("World: %d\n",		pPkt->iWorldID);
				printf("Account: %s\n",		pPkt->szAccount);
				printf("RoleName: %s\n",	pPkt->szRoleName);
				printf("Voc: %s\n",			szName);
				printf("iSkillPoint: %d\n",	pPkt->iSkillPoint);
				
				printf("\n[ 职业技能资讯 ]\n");
				printf("\n名称(等级)\n\n");
				for (int i=0; i<100; i++)
				{
					iLevel = Pal_GetSkillLv(pPkt->iVocType, i, pPkt->iVocSkillID[i]);	// 取得技能等级
					
					if (iLevel == 0) continue;	//  0:尚未习得此技能
					if (iLevel > 0)				// >0:已习得此技能,输出资讯
					{
						sprintf(szName, Pal_GetItemName(pPkt->iVocSkillID[i]));			// 取得技能名称
						// 若查不到资料
						if (strlen(szName) == 0) sprintf(szName, "Unknow Name");
						printf("%s(%d)\n", szName, iLevel);
					}
				}
				
				printf("\n[ 生产技能资讯 ]\n");
				printf("\n名称\t等级\t经验值\n\n");
				for (i=0; i<6; i++)
				{
					sprintf(szName, Pal_GetLiveHoodName(i));
					if (szName != NULL)
						printf("%s\t%d\t%d\n",	szName, pPkt->sLifeHoodSkill.iLevel[i], pPkt->sLifeHoodSkill.iEXP[i]);
				}

				_itoa(pPkt->iWorldID,strInt,10);


			}
			break;

		case ENUM_CharacterSkillResult_RoleNotFound:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"角色技能结果: Character %s not found\n", pPkt->szRoleName);
			printf("Charfriendlist result: Character %s not found\n", pPkt->szRoleName);
			g_state = -1;
			break;


		case ENUM_CharacterSkillResult_WorldNotFound:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"角色技能结果: 大区没有找到\n");
			printf("Charfriendlist result: World not found\n");
			g_state = -1;
			break;

		case ENUM_CharacterSkillResult_LeaderNotFound:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"角色技能结果: 频道没有找到\n");
			printf("Charfriendlist result: Leader not found\n");
			g_state = -1;
			break;

		case ENUM_CharacterSkillResult_LeaderDisconnect:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"角色技能结果: 频道没有连接\n");
			printf("Charfriendlist result: Leader disconnect\n");
				g_state = -1;
			break;

		default:
				g_state = -1;
			break;
		}	
}
