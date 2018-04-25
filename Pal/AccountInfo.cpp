// AccountInfo.cpp: implementation of the CAccountInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AccountInfo.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAccountInfo::CAccountInfo()
{

}

CAccountInfo::~CAccountInfo()
{

}
vector <CGlobalStruct::TFLV> CAccountInfo::AccountInfoMain(char *szGMServerName, char * szGMServerIP,char* szAccountName)
{
	
			char strlog[80];
			sprintf(strlog,"大区<%s>账号<%s>取得帐号资讯",szGMServerName,szAccountName);
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
			g_ccNetObj.RegEvent_OnPacket(ENUM_PGPlayerStatus_StoC_AccountInfo, AccountInfo, 0);

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
							m_tflv.m_tvlength=strlen("连接失败");
							sprintf((LPSTR)&m_tflv.lpdata,"%s","连接失败");
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

						PGPlayerStatus_CtoS_AccountInfo sPkt;
						sPkt.iWorldID = iWorldID;
						sprintf(sPkt.szAccountName, "%s", szAccountName);

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
void CAccountInfo::LoginResult(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
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
void CAccountInfo::AccountInfo(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
{

		PGPlayerStatus_StoC_AccountInfo *pPkt = (PGPlayerStatus_StoC_AccountInfo *)pData;
		CGlobalStruct::TFLV m_tflv;
		char strInt[10];
		char bufstr[256];
		switch(pPkt->emResult)
		{
			case ENUM_AccountInfoResult_Success:
				{
					// 输出结果
					printf("\n====================================================\n");
					printf("World: %d\n",		pPkt->iWorldID);
					printf("AccountID: %d\n",	pPkt->iAccountID);
					printf("AccountName: %s\n", pPkt->szAccountName);
					printf("MaxRoleCount: %d\n",pPkt->iMaxRoleCount);
				
					printf("\n角色列表:\n");
					printf("[ 角色ID ] [   角色名称   ] [   建立时间   ]\n");
					int i=0;
					for (i=0; i<30; i++)
					{
						// 判断是否已到最后一笔资料
						if (pPkt->sGMAccountRoleInfo[i].iDBID == -1) break;
							printf("%10d %16s %s\n", pPkt->sGMAccountRoleInfo[i].iDBID, pPkt->sGMAccountRoleInfo[i].szRoleName, 
								pPkt->sGMAccountRoleInfo[i].szCreateTime);
					}
					for (i=0; i<30; i++)
					{
						// 判断是否已到最后一笔资料
						if (pPkt->sGMAccountRoleInfo[i].iDBID == -1) break;
						{
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
							m_tflv.m_tvlength=strlen(pPkt->szAccountName);
							sprintf((LPSTR)&m_tflv.lpdata,"%s",pPkt->szAccountName);
							DBVect.push_back(m_tflv);
							
							_itoa(pPkt->sGMAccountRoleInfo[i].iDBID,strInt,10);
							m_tflv.nIndex=DBVect.size()+1;
							m_tflv.m_tagName=CEnumCore::TagName::PAL_ROLEID;
							m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
							m_tflv.m_tvlength=sizeof(int);
							sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
							DBVect.push_back(m_tflv);
							
							m_tflv.nIndex=DBVect.size()+1;
							m_tflv.m_tagName=CEnumCore::TagName::PAL_ROLENAME;
							m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
							m_tflv.m_tvlength=strlen(pPkt->sGMAccountRoleInfo[i].szRoleName);
							sprintf((LPSTR)&m_tflv.lpdata,"%s",pPkt->sGMAccountRoleInfo[i].szRoleName);
							DBVect.push_back(m_tflv);


							m_tflv.nIndex=DBVect.size()+1;
							m_tflv.m_tagName=CEnumCore::TagName::PAL_TIME;
							m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
							m_tflv.m_tvlength=strlen(pPkt->sGMAccountRoleInfo[i].szCreateTime);
							sprintf((LPSTR)&m_tflv.lpdata,"%s",pPkt->sGMAccountRoleInfo[i].szCreateTime);
							DBVect.push_back(m_tflv);

						}

					}
				}
				break;
			case ENUM_AccountInfoResult_AccountNotFound:
				printf("AccountInfo result: Account %s not found\n", pPkt->szAccountName);
				ZeroMemory(bufstr,256);
				sprintf(bufstr,"账号信息: 账号%s没有找到\n", pPkt->szAccountName);
				m_tflv.nIndex=DBVect.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(bufstr);
				sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
				DBVect.push_back(m_tflv);
				break;
			
			case ENUM_AccountInfoResult_RoleNotFound:
				printf("AccountInfo result: Account %s has no role\n", pPkt->szAccountName);
				ZeroMemory(bufstr,256);
				sprintf(bufstr,"账号信息:账号 %s 没有角色\n", pPkt->szAccountName);
				m_tflv.nIndex=DBVect.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(bufstr);
				sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
				DBVect.push_back(m_tflv);
				break;	
			
			case ENUM_AccountInfoResult_WorldNotFound:
				printf("AccountInfo result: World not found\n");
				ZeroMemory(bufstr,256);
				sprintf(bufstr,"账号信息: 世界没有找到\n");
				m_tflv.nIndex=DBVect.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(bufstr);
				sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
				DBVect.push_back(m_tflv);
				break;
			
			case ENUM_AccountInfoResult_LeaderNotFound:
				printf("AccountInfo result: Leader not found\n");
				ZeroMemory(bufstr,256);
				sprintf(bufstr,"账号信息: 观测者没有找到\n");

				break;
			
			case ENUM_AccountInfoResult_LeaderDisconnect:
				printf("AccountInfo result: Leader disconnect\n");
				ZeroMemory(bufstr,256);
				sprintf(bufstr,"账号信息: 观测者没有连接\n");
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
