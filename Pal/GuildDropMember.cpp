// GuildDropMember.cpp: implementation of the CCharacterInfo class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "GuildDropMember.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CGuildDropMember::CGuildDropMember()
{
	
}

CGuildDropMember::~CGuildDropMember()
{
	
}

vector <CGlobalStruct::TFLV>  CGuildDropMember::GuildDropMemberMain(char *szGMServerName, char * szGMServerIP,char* szGuildName,char* szRoleName)
{
	char strlog[80];
	sprintf(strlog,"大区<%s>帮会<%s>开除会员<%s>",szGMServerName,szGuildName,szRoleName);
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
	g_ccNetObj.RegEvent_OnPacket(ENUM_PGPlayerControl_StoC_GuildDropMember, GuildDropMember, 0);
	
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
	
	ZeroMemory(szAccount,32);
	ZeroMemory(szPassword,32);
	ZeroMemory(szGMServerIP,16);
	
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
				// 设定GMS连线参数
				operPal.GetUserNamePwd("User4",szAccount,szPassword,&iGMServerPort);
				
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
			{
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
			}
			break;
			// 登入成功
		case 2:
			{
				printf("登入成功\n\n");
				
				// 设定命令参数
				iWorldID=operPal.FindWorldID(szGMServerName,szGMServerIP);
				
				PGPlayerControl_CtoS_GuildDropMember sPkt;
				sPkt.iWorldID = iWorldID;
				sprintf(sPkt.szGuildName, "%s", szGuildName);
				sprintf(sPkt.szRoleName, "%s", szRoleName);
				
				// 传送至GMS要求开除公会会员
				g_ccNetObj.Send(sizeof(sPkt), (S_ObjNetPktBase*)&sPkt);	
				g_state = 3;			
			}
			break;
			// 等待开除公会会员
		case 3:
			{
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
				printf("等待开除公会会员中,第%i次\n",n_send);
			}
			break;
		}
	}			
	printf("\n");
	printf("\n==================== Shutdown ====================\n");
	return DBVect;
}

void CGuildDropMember::LoginResult(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
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
		{
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"登陆失败,帐号错误\n");
			printf("登陆失败,帐号错误\n");
		}
		break;		
	case ENUM_GMCLoginResult_PasswordFailed:
		{
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"登陆失败,密码错误\n");
			printf("登陆失败,密码错误\n");
		}
		break;		
	case ENUM_GMCLoginResult_IPFailed:
		{
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"登陆失败,IP地址错误\n");
			printf("登陆失败,IP地址错误\n");
		}
		break;		
	default:
		{
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"登陆失败\n");
			printf("登陆失败\n");
		}
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

void CGuildDropMember::GuildDropMember(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
{
	PGPlayerControl_StoC_GuildDropMember *pPkt = (PGPlayerControl_StoC_GuildDropMember *)pData;
	
	char bufstr[256];
	ZeroMemory(bufstr,256);
	switch(pPkt->emResult)
	{
	case ENUM_GuildDropMemberResult_Success:		
		{
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"解除工会成员成功, 大区号: %d, 工会名: %s, 角色名:\n", pPkt->iWorldID, pPkt->szGuildName, pPkt->szRoleName);
			printf("GuildDropMember succsee, world id: %d, guild name: %s, role name: %s\n", 
			pPkt->iWorldID, pPkt->szGuildName, pPkt->szRoleName);
		}
		break;
		
	case ENUM_GuildDropMemberResult_WorldNotFound:
		{
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"解除工会成员结果: 大区没有找到\n");
			printf("GuildDropMember result: World not found\n");
		}
		break;	
		
	case ENUM_GuildDropMemberResult_LeaderNotFound:
		{
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"解除工会成员结果: 频道没有找到\n");
			printf("GuildDropMember result: Leader not found\n");
		}
		break;
		
	case ENUM_GuildDropMemberResult_LeaderDisconnect:
		{
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"解除工会成员结果: 频道没有连接\n");
			printf("GuildDropMember result: Leader disconnect\n");
		}
		break;
		
	case ENUM_GuildDropMemberResult_GuildNotFound:
		{
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"解除工会成员结果: 帮会%s 没有找到\n", pPkt->szGuildName);
			printf("GuildDropMember result: Guild %s not found\n", pPkt->szGuildName);
		}
		break;	
		
	case ENUM_GuildDropMemberResult_RoleNotFound:
		{
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"解除工会成员结果: 角色%s 没有找到\n", pPkt->szRoleName);
			printf("GuildDropMember result: Character %s not found\n", pPkt->szRoleName);
		}
		break;	
		
	case ENUM_GuildDropMemberResult_LastMember:
		{
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"解除工会成员结果: 不能解除最后一个角色\n");
			printf("GuildDropMember result: Operation failed, last member can't be dismissed\n");
		}
		break;	
		
	case ENUM_GuildDropMemberResult_GMOOperationFailed:
		{
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"解除工会成员结果:游戏服务器操作失败\n");
			printf("GuildDropMember result: Operation failed from GMObserver\n");
		}
		break;	
		
	case ENUM_GuildDropMemberResult_ChatOperationFailed:
		{
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"解除工会成员结果:服务器操作失败\n");
			printf("GuildDropMember result: Operation failed from chatserver\n");
		}
		break;	
		
	default:
		break;
	}//switch
	
	g_state = -1;
}