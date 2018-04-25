// CharacterBlackList.cpp: implementation of the CCharacterBlackList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CharacterBlackList.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCharacterBlackList::CCharacterBlackList()
{

}

CCharacterBlackList::~CCharacterBlackList()
{

}

vector <CGlobalStruct::TFLV> CCharacterBlackList::CharacterBlackListMain(char *szGMServerName, char * szGMServerIP,char* szRoleName)
{	


			char strlog[80];
			sprintf(strlog,"大区<%s>角色<%s>查询角色黑名单列表",szGMServerName,szRoleName);
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
			g_ccNetObj.RegEvent_OnPacket(ENUM_PGPlayerStatus_StoC_CharacterBlackList, CharacterBlackList, 0);

			

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
						// 设定GMS连线参数
						//char szGMServerIP[16] = "60.206.15.78";	   // GMServer IP位址
						//	int  iGMServerPort = 9991;					// GMServer Port

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
						//	char szAccount[32] = "silent2";			// 登入帐号
						//	char szPassword[32] = "silent2";		// 登入密码	
							
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
							m_tflv.m_tvlength=strlen("Connect Failed");
							sprintf((LPSTR)&m_tflv.lpdata,"%s","Connect Failed");
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
						m_tflv.m_tvlength=strlen("Login Failed");
						sprintf((LPSTR)&m_tflv.lpdata,"%s","Login Failed");
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
						
						PGPlayerStatus_CtoS_CharacterBlackList sPkt;
						sPkt.iWorldID = iWorldID;
						sprintf(sPkt.szRoleName, "%s", szRoleName);
				
						// 传送至GMS要求取得角色黑名单列表
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
						m_tflv.m_tvlength=strlen("Active Failed");
						sprintf((LPSTR)&m_tflv.lpdata,"%s","Active Failed");
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
void CCharacterBlackList::LoginResult(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
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


// 取得角色好友列表结果-------------------------------------------------------------------------------
void CCharacterBlackList::CharacterBlackList(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
{

		PGPlayerStatus_StoC_CharacterBlackList *pPkt = (PGPlayerStatus_StoC_CharacterBlackList *)pData;
		CGlobalStruct::TFLV m_tflv;
		char strInt[10];
		char bufstr[256];
		switch(pPkt->emResult)
		{
		case ENUM_CharacterBlackListResult_Success:
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
				
				printf("\n[ 黑名单列表 ]\n");
				for(int i=0; i<15; i++)
				{
					// 无资料则离开
					if(pPkt->szBlackList[i][0] == '\0') continue;
					
					printf("角色名称: %s\n",pPkt->szBlackList[i]);
					switch(i)
					{
					case 0:
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_BlackList0;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(pPkt->szBlackList[i]);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",pPkt->szBlackList[i]);
						break;
					case 1:
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_BlackList1;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(pPkt->szBlackList[i]);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",pPkt->szBlackList[i]);
						break;
					case 2:
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_BlackList2;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(pPkt->szBlackList[i]);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",pPkt->szBlackList[i]);
						break;
					case 3:
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_BlackList3;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(pPkt->szBlackList[i]);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",pPkt->szBlackList[i]);
						break;
					case 4:
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_BlackList4;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(pPkt->szBlackList[i]);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",pPkt->szBlackList[i]);
						break;
					case 5:
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_BlackList5;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(pPkt->szBlackList[i]);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",pPkt->szBlackList[i]);
						break;
					case 6:
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_BlackList6;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(pPkt->szBlackList[i]);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",pPkt->szBlackList[i]);
						break;
					case 7:
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_BlackList7;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(pPkt->szBlackList[i]);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",pPkt->szBlackList[i]);
						break;
					case 8:
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_BlackList8;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(pPkt->szBlackList[i]);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",pPkt->szBlackList[i]);
						break;
					case 9:
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_BlackList9;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(pPkt->szBlackList[i]);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",pPkt->szBlackList[i]);
						break;
					case 10:
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_BlackList10;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(pPkt->szBlackList[i]);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",pPkt->szBlackList[i]);
						break;
					case 11:
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_BlackList11;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(pPkt->szBlackList[i]);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",pPkt->szBlackList[i]);
						break;
					case 12:
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_BlackList12;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(pPkt->szBlackList[i]);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",pPkt->szBlackList[i]);
						break;
					case 13:
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_BlackList13;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(pPkt->szBlackList[i]);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",pPkt->szBlackList[i]);
						break;
					case 14:
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_BlackList14;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(pPkt->szBlackList[i]);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",pPkt->szBlackList[i]);
						break;
					}
					DBVect.push_back(m_tflv);


				}



			


			}
			break;

		case ENUM_CharacterBlackListResult_RoleNotFound:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"角色朋友列表结果: Character %s not found\n", pPkt->szRoleName);
			printf("Charfriendlist result: Character %s not found\n", pPkt->szRoleName);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(bufstr);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
			DBVect.push_back(m_tflv);
			g_state = -1;
			break;

		case ENUM_CharacterBlackListResult_BlackListEmpty:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"角色朋友列表结果: 角色 %s 没有朋友\n", pPkt->szRoleName);
			printf("Charfriendlist result: Character %s has no friend\n", pPkt->szRoleName);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(bufstr);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
			DBVect.push_back(m_tflv);
			g_state = -1;
			break;

		case ENUM_CharacterBlackListResult_WorldNotFound:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"角色朋友列表结果: 大区没有找到\n");
			printf("Charfriendlist result: World not found\n");
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(bufstr);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
			DBVect.push_back(m_tflv);
			g_state = -1;
			break;

		case ENUM_CharacterBlackListResult_LeaderNotFound:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"角色朋友列表结果: 频道没有找到\n");
			printf("Charfriendlist result: Leader not found\n");
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(bufstr);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
			DBVect.push_back(m_tflv);
			g_state = -1;
			break;

		case ENUM_CharacterBlackListResult_LeaderDisconnect:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"角色朋友列表结果: 频道没有连接\n");
			printf("Charfriendlist result: Leader disconnect\n");
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
