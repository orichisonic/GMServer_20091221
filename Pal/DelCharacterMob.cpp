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
			sprintf(strlog,"大区<%s>角色<%s>删除伏魔",szGMServerName,szRoleName);
			logFile.WriteText("仙剑OL",strlog);

			CGlobalStruct::TFLV m_tflv;
			if(!DBVect.empty())
			{
				DBVect.clear();
			}


			// 载入DBF档以查询样版名称(指定"游戏目录\Kernel\\data")
			Pal_LoadData(".\\data");

			// 注册连线CallBack函式
			g_ccNetObj.RegEvent_OnConnected(COperPal::OnConnect, 0);
			g_ccNetObj.RegEvent_OnDisconnected(COperPal::OnDisconnect, 0);
			g_ccNetObj.RegEvent_OnConnectFailed(COperPal::OnConnectfailed, 0);

			// 注册对应事件CallBack函式
			g_ccNetObj.RegEvent_OnPacket(ENUM_PGGMCConnection_StoC_LoginResult,	LoginResult, 0);
			g_ccNetObj.RegEvent_OnPacket(ENUM_PGPlayerControl_StoC_DelCharacterMob, DelCharacterMob, 0);

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
						int  iFlagID = Pal_GetMobFlag(iMobID);	// 使用怪物编号来查询旗标编号

						PGPlayerControl_CtoS_DelCharacterMob sPkt;
						sPkt.iWorldID	= iWorldID;
						sprintf(sPkt.szRoleName, "%s", szRoleName);
						sPkt.iMobID		= iMobID;
						sPkt.iFlagID	= iFlagID;

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
void CDelCharacterMob::LoginResult(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
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
		sprintf(bufstr,"删除角色伏魔成功:世界号%i,角色名:%s,旗标:%i\n", pPkt->iWorldID,pPkt->szRoleName,pPkt->iMobID);
		
		break;
	case ENUM_DelCharacterMobResult_WorldNotFound:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"删除角色伏魔结果:世界号没有找到\n");
		printf("DelCharacterMob result: World not found\n");
		break;	
	case ENUM_DelCharacterMobResult_LeaderNotFound:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"删除角色伏魔结果: 观测者没有找到\n");
		printf("DelCharacterMob result: Leader not found\n");
		break;
	case ENUM_DelCharacterMobResult_LeaderDisconnect:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"删除角色伏魔结果: 观测者断开\n");
		printf("DelCharacterMob result: Leader disconnect\n");
		break;	
	case ENUM_DelCharacterMobResult_RoleNotFound:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"删除角色伏魔结果: 角色 %s 没有找到\n", pPkt->szRoleName);
		printf("DelCharacterMob result: Character %s not found\n", pPkt->szRoleName);
		break;	
	case ENUM_DelCharacterMobResult_RoleIsOnline:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"删除角色伏魔结果: 角色 %s 现在在线\n", pPkt->szRoleName);
		printf("DelCharacterMob result: Character %s is online now\n", pPkt->szRoleName);
		break;	
	case ENUM_DelCharacterMobResult_MobNotFound:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"删除角色伏魔结果: 角色 %s 还没有有这个伏魔\n", pPkt->szRoleName);
		printf("DelCharacterMob result: Character %s have not obtained this mob yet\n", pPkt->szRoleName);
		break;	
	case ENUM_DelCharacterMobResult_FlagError:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"删除角色伏魔结果: 旗标没有找到\n");
		printf("DelCharacterMob result: Flag error\n");
		break;	
	case ENUM_DelCharacterMobResult_GMOOperationFailed:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"删除角色伏魔结果: 远程服务操作失败\n");
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
