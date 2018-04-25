// ReloadFunctionSwitch.cpp: implementation of the CReloadFunctionSwitch class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ReloadFunctionSwitch.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CReloadFunctionSwitch::CReloadFunctionSwitch()
{

}

CReloadFunctionSwitch::~CReloadFunctionSwitch()
{

}
void CReloadFunctionSwitch::ReloadFunctionSwitchMain(SOCKET m_socket,COperVector * recvopervector,CSession * pPalSession)
{	



			// 注册连线CallBack函式
			g_ccNetObj.RegEvent_OnConnected(COperPal::OnConnect, 0);
			g_ccNetObj.RegEvent_OnDisconnected(COperPal::OnDisconnect, 0);
			g_ccNetObj.RegEvent_OnConnectFailed(COperPal::OnConnectfailed, 0);

			// 注册对应事件CallBack函式
			g_ccNetObj.RegEvent_OnPacket(ENUM_PGGMCConnection_StoC_LoginResult,	LoginResult, 0);
			g_ccNetObj.RegEvent_OnPacket(ENUM_PGServerControl_StoC_ReloadFunctionSwitch, ReloadFunctionSwitch, 0);

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
			char szGMServerIP[16];
			char szRoleName[32];
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
						sprintf(szGMServerIP,"%s",(char *)&(recvopervector->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata);
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
					}
					printf("服务器登入中,第%i次\n",n_login);
					break;
					// 登入成功
				case 2:
					{
						printf("登入成功\n\n");

						// 设定命令参数
					//	int  iWorldID = 2;							// 世界编号
					//	char szRoleName[32] = "松岛枫";		// 角色名称

						PGServerControl_CtoS_ReloadFunctionSwitch sPkt;
						sPkt.iWorldID = iWorldID;

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
					}
					printf("等待取得角色背包资讯中,第%i次\n",n_send);
					break;
				}
			}			
			g_ccNetObj.Disconnect();
			printf("\n");
			printf("\n==================== Shutdown ====================\n");
			


}
void CReloadFunctionSwitch::LoginResult(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
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
void CReloadFunctionSwitch::ReloadFunctionSwitch(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
{

		PGServerControl_StoC_ReloadFunctionSwitch  *pPkt = (PGServerControl_StoC_ReloadFunctionSwitch  *)pData;
		char strInt[10];
		char bufstr[256];
		switch(pPkt->emResult)
		{
		case ENUM_ReloadFunctionSwitchResult_Success:
			{
				printf("ReloadFunctionSwitch success, world id: %d\n", pPkt->iWorldID);	
				ZeroMemory(bufstr,256);
				sprintf(bufstr,"重载功能开关成功:世界号：%i\n", pPkt->iWorldID);

			}
			break;

		case ENUM_ReloadFunctionSwitchResult_WorldNotFound:
				printf("ReloadFunctionSwitch result: World not found\n");
				ZeroMemory(bufstr,256);
				sprintf(bufstr,"重载功能开关结果:世界没有找到\n");
			break;
		case ENUM_ReloadFunctionSwitchResult_LeaderNotFound:
				printf("ReloadFunctionSwitch result: Leader not found\n");
				ZeroMemory(bufstr,256);
				sprintf(bufstr,"重载功能开关结果:观察者没有找到\n");
			break;

		case ENUM_ReloadFunctionSwitchResult_LeaderDisconnect:
			printf("ReloadFunctionSwitch result: Leader disconnect\n");
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"重载功能开关结果:观测者断线\n");
			break;
		case ENUM_ReloadFunctionSwitchResult_LocalDisconnect:
			printf("ReloadFunctionSwitch result: Local Disconnect\n");
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"重载功能开关结果:本地连接断开\n");
			break;
		default:
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
