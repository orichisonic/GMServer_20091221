// CharacterQuest.cpp: implementation of the CCharacterQuest class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "CharacterQuest.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCharacterQuest::CCharacterQuest()
{

}

CCharacterQuest::~CCharacterQuest()
{

}
vector <CGlobalStruct::TFLV> CCharacterQuest::CharacterQuestMain(char * szGMServerName, char * szGMServerIP,char * szRoleName,int iQuestType)
{
			char strlog[80];
			ZeroMemory(strlog,80);
			sprintf(strlog,"大区<%s>角色<%s>查询角色任务信息",szGMServerName,szRoleName);
			logFile.WriteText("仙剑OL",strlog);
			
			// 载入DBF档以查询样版名称(指定"游戏目录\Kernel\\data")
			//Pal_LoadData("D:\\test\\仙剑奇侠传Online(for test)\\Kernel\\data");
			Pal_LoadData(".\\data");

			// 注册连线CallBack函式
			g_ccNetObj.RegEvent_OnConnected(COperPal::OnConnect, 0);
			g_ccNetObj.RegEvent_OnDisconnected(COperPal::OnDisconnect, 0);
			g_ccNetObj.RegEvent_OnConnectFailed(COperPal::OnConnectfailed, 0);

			// 注册对应事件CallBack函式
			g_ccNetObj.RegEvent_OnPacket(ENUM_PGGMCConnection_StoC_LoginResult,	LoginResult, 0);
			g_ccNetObj.RegEvent_OnPacket(ENUM_PGPlayerStatus_StoC_CharacterQuest, CharacterQuest, 0);

			// 初始网路设定
			g_ccNetObj.SetConnectCount(10);		// 设定重新连线次数为10
			g_ccNetObj.SetReConnect(false);		// 设定断线的时候不会重新连线
			g_ccNetObj.SetShowIP(true);			// 设定连线时会显示IP

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

			ZeroMemory(szAccount,32);
			ZeroMemory(szPassword,32);

			


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
						operPal.GetUserNamePwd("User3",szAccount,szPassword,&iGMServerPort);

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
						
						// 任务类型
						iWorldID=operPal.FindWorldID(szGMServerName,szGMServerIP);

						
						PGPlayerStatus_CtoS_CharacterQuest sPkt;
						sPkt.iWorldID = iWorldID;
						sprintf(sPkt.szRoleName, "%s", szRoleName);
						sPkt.iQuestType = iQuestType;
						
						// 传送至GMS要求取得角色任务资讯
						g_ccNetObj.Send(sizeof(sPkt), (S_ObjNetPktBase*)&sPkt);	
						g_state = 3;			
					}
					break;
					// 等待取得角色任务资讯
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
					printf("等待取得角色任务资讯中,第%i次\n",n_send);
					break;
				}
			}			
			g_ccNetObj.Disconnect();
			printf("\n");
			printf("\n==================== Shutdown ====================\n");
			return DBVect;

}
void CCharacterQuest::LoginResult(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
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

// 取得角色任务资讯结果-------------------------------------------------------------------------------
void CCharacterQuest::CharacterQuest(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
{

		PGPlayerStatus_StoC_CharacterQuest *pPkt = (PGPlayerStatus_StoC_CharacterQuest *)pData;
		char strInt[10];
		char bufstr[256];
		ZeroMemory(strInt,10);
		ZeroMemory(bufstr,256);
		CGlobalStruct::TFLV m_tflv;
		switch(pPkt->emResult)
		{
		case ENUM_CharacterQuestResult_Success:
			{
				// 若为结束封包
				if (pPkt->iWorldID == -1)
				{
					g_state = -1;
					return;
				}
				
				// 输出结果
				printf("\n====================================================\n");
				printf("World: %d\n",		pPkt->iWorldID);
				printf("Account: %s\n",		pPkt->szAccount);
				printf("RoleName: %s\n",	pPkt->szRoleName);
				printf("任务类型: %d\n",	pPkt->iQuestType);
				printf("任务名称: %d(%s)\n",pPkt->iQuestID, Pal_GetQuestName(pPkt->iQuestID));
				
				
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
			
				
				_itoa(pPkt->iQuestType,strInt,10);
				m_tflv.nIndex=DBVect.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::PAL_QUESTTYPE;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
				m_tflv.m_tvlength=sizeof(int);
				sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
				DBVect.push_back(m_tflv);
				
				_itoa(pPkt->iQuestID,strInt,10);
				m_tflv.nIndex=DBVect.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::PAL_QUESTID;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
				m_tflv.m_tvlength=sizeof(int);
				sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
				DBVect.push_back(m_tflv);
				

				m_tflv.nIndex=DBVect.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::PAL_QUESTNAME;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(Pal_GetQuestName(pPkt->iQuestID));
				sprintf((LPSTR)&m_tflv.lpdata,"%s",Pal_GetQuestName(pPkt->iQuestID));
				DBVect.push_back(m_tflv);
				
			}
			break;
			
		case ENUM_CharacterQuestResult_TypeError:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"角色基本信息结果: 错误任务类型\n");
			printf("Charquest result: Incorrect quest type\n");
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(bufstr);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
			DBVect.push_back(m_tflv);
				g_state = -1;
			break;
			
		case ENUM_CharacterQuestResult_RoleNotFound:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"角色基本信息结果: 角色 %s 没有找到 \n", pPkt->szRoleName);
			printf("Charquest result: Character %s not found\n", pPkt->szRoleName);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(bufstr);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
			DBVect.push_back(m_tflv);
				g_state = -1;
			break;
			
		case ENUM_CharacterQuestResult_QuestNotFound:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"角色基本信息结果: 任务没有找到\n");
			printf("Charquest result: Quest not found\n");
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(bufstr);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
			DBVect.push_back(m_tflv);
				g_state = -1;
			break;
			
		case ENUM_CharacterQuestResult_WorldNotFound:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"角色基本信息结果: 大区没有找到\n");
			printf("Charinfo result: World not found\n");
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(bufstr);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
			DBVect.push_back(m_tflv);
				g_state = -1;
			break;
			
		case ENUM_CharacterQuestResult_LeaderNotFound:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"角色基本信息结果: 频道没有找到\n");
			printf("Charinfo result: Leader not found\n");
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(bufstr);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
			DBVect.push_back(m_tflv);
				g_state = -1;
			break;
			
		case ENUM_CharacterQuestResult_LeaderDisconnect:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"角色基本信息结果: 频道没有连接\n");
			printf("Charinfo result: Leader disconnect\n");
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