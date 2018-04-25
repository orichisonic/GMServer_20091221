// BanishPlayer.cpp: implementation of the CBanishPlayer class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "BanishPlayer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBanishPlayer::CBanishPlayer()
{

}

CBanishPlayer::~CBanishPlayer()
{

}
vector <CGlobalStruct::TFLV> CBanishPlayer::BanishPlayerMain(char * szGMServerName,char * szGMServerIP,char * szRoleName)
{	
			int iIndex=0;
			int iPageSize=0;

			char strlog[80];
			sprintf(strlog,"大区<%s>角色<%s>踢人",szGMServerName,szRoleName);
			logFile.WriteText("仙剑OL",strlog);

			if(!DBVect.empty())
			{
				DBVect.clear();
			}
			CGlobalStruct::TFLV m_tflv;


			// 注册连线CallBack函式
			g_ccNetObj.RegEvent_OnConnected(COperPal::OnConnect, 0);
			g_ccNetObj.RegEvent_OnDisconnected(COperPal::OnDisconnect, 0);
			g_ccNetObj.RegEvent_OnConnectFailed(COperPal::OnConnectfailed, 0);

			// 注册对应事件CallBack函式
			g_ccNetObj.RegEvent_OnPacket(ENUM_PGGMCConnection_StoC_LoginResult,	LoginResult, 0);
			g_ccNetObj.RegEvent_OnPacket(ENUM_PGPlayerControl_StoC_BanishPlayer, BanishPlayer, 0);

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

						operPal.GetUserNamePwd("User1",szAccount,szPassword,&iGMServerPort);
						

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
			
						PGPlayerControl_CtoS_BanishPlayer sPkt;
						sPkt.iWorldID = iWorldID;
						sprintf(sPkt.szRoleName, "%s", szRoleName);

						// 传送至GMS要求踢某玩家下线
						g_ccNetObj.Send(sizeof(sPkt), (S_ObjNetPktBase*)&sPkt);	
						g_state = 3;			
					}
					break;
					// 等待踢某玩家下线
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
					printf("等待踢某玩家下线中,第%i次\n",n_send);
					break;
				}
			}			
			g_ccNetObj.Disconnect();
			printf("\n");
			printf("\n==================== Shutdown ====================\n");
			return DBVect;
	


}

void CBanishPlayer::LoginResult(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
{
		char bufstr[256];
		ZeroMemory(bufstr,256);
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
			printf("登陆失败\n");	
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

// 踢某玩家下线结果-----------------------------------------------------------------------------------
void CBanishPlayer::BanishPlayer(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
{

		PGPlayerControl_StoC_BanishPlayer *pPkt = (PGPlayerControl_StoC_BanishPlayer *)pData;
		char bufstr[256];
		switch(pPkt->emResult)
		{
		case ENUM_BanishPlayerResult_Success:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"踢角色名 %s 成功, 世界号: %d\n", pPkt->szRoleName, pPkt->iWorldID);
			printf("踢角色名 %s 成功, 世界号: %d\n", pPkt->szRoleName, pPkt->iWorldID);
			break;
			
		case ENUM_BanishPlayerResult_RoleNotFound:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"踢人结果: 角色 %s不存在\n", pPkt->szRoleName);
			printf("踢人结果: 角色 %s不存在\n", pPkt->szRoleName);
			break;
			
		case ENUM_BanishPlayerResult_RoleNotOnline:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"踢人结果:  角色 %s 不在线\n", pPkt->szRoleName);
			printf("踢人结果:  角色 %s 不在线\n", pPkt->szRoleName);
			break;	
			
		case ENUM_BanishPlayerResult_WorldNotFound:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"踢人结果: 大区找不到\n");
			printf("踢人结果: 大区找不到\n");
			break;	
			
		case ENUM_BanishPlayerResult_LeaderNotFound:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"踢人结果: 频道找不到\n");
			printf("踢人结果: 频道找不到\n");
			break;
			
		case ENUM_BanishPlayerResult_LeaderDisconnect:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"踢人结果: 频道断开\n");
			printf("踢人结果: 频道断开\n");
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
