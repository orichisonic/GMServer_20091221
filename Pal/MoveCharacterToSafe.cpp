// MoveCharacterToSafe.cpp: implementation of the CMoveCharacterToSafe class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MoveCharacterToSafe.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMoveCharacterToSafe::CMoveCharacterToSafe()
{

}

CMoveCharacterToSafe::~CMoveCharacterToSafe()
{

}
vector<CGlobalStruct::TFLV> CMoveCharacterToSafe::MoveCharacterToSafeMain(char * szGMServerName,char * szGMServerIP,char * szRoleName,int iZoneID)
{	

			char strlog[80];
			sprintf(strlog,"大区<%s>角色<%s>移动到安全点",szGMServerName,szRoleName);
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
			g_ccNetObj.RegEvent_OnPacket(ENUM_PGPlayerControl_StoC_MoveCharacterToSafe, MoveCharacterToSafe, 0);

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

						int  X, Y, Z;					// 位置座标
						// 以区域编号来查询安全点座标
						bool bSuccess = Pal_GetSavePoint(iZoneID, &X, &Y, &Z);
						// 若查询失败则结束
						if (!bSuccess) 
						{
							printf("查询安全点座标失败,指令终止");
							m_tflv.nIndex=DBVect.size()+1;
							m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
							m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
							m_tflv.m_tvlength=strlen("查询安全点座标失败,指令终止");
							sprintf((LPSTR)&m_tflv.lpdata,"%s","查询安全点座标失败,指令终止");
							DBVect.push_back(m_tflv);		
							g_state = -1;
							break;
						}
						
						PGPlayerControl_CtoS_MoveCharacterToSafe sPkt;
						sPkt.iWorldID = iWorldID;
						sprintf(sPkt.szRoleName, "%s", szRoleName);
						sPkt.iZoneID	= iZoneID;
						sPkt.X			= X;
						sPkt.Y			= Y;
						sPkt.Z			= Z;
						
						// 传送至GMS要求移动角色至安全点
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
void CMoveCharacterToSafe::LoginResult(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
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
void CMoveCharacterToSafe::MoveCharacterToSafe(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
{
	PGPlayerControl_StoC_MoveCharacterToSafe *pPkt = (PGPlayerControl_StoC_MoveCharacterToSafe *)pData;
	char bufstr[256];		
	switch(pPkt->emResult)
	{
		case ENUM_MoveCharacterToSafeResult_Success:			
			printf("MoveCharacterToSafe succsee, world id: %d, role name: %s, zone id: %d\n", 
				pPkt->iWorldID, pPkt->szRoleName, pPkt->iZoneID);
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"移动角色到安全点成功:世界号%d,角色名：%s,地区ID %d\n", pPkt->iWorldID, pPkt->szRoleName, pPkt->iZoneID);
			break;
			
		case ENUM_MoveCharacterToSafeResult_RoleNotFound:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"移动角色到安全点:  角色 %s 不存在\n", pPkt->szRoleName);
			printf("MoveCharacterToSafe result: Character %s not exist\n", pPkt->szRoleName);
			break;	
			
		case ENUM_MoveCharacterToSafeResult_WorldNotFound:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"移动角色到安全点: 世界没有找到");
			printf("MoveCharacterToSafe result: world not found\n");
			break;	
			
		case ENUM_MoveCharacterToSafeResult_LeaderNotFound:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"移动角色到安全点: 观测者没有找到");
			printf("MoveCharacterToSafe result: leader not found\n");
			break;
			
		case ENUM_MoveCharacterToSafeResult_LeaderDisconnect:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"移动角色到安全点: 观测者断开连接");
			printf("MoveCharacterToSafe result: leader disconnect\n");
			break;
			
		case ENUM_MoveCharacterToSafeResult_GMOOperationFailed:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"移动角色到安全点: 远程操作失败");
			printf("MoveCharacterToSafe result: Operation failed from GMObserver\n");
			break;	
			
		case ENUM_MoveCharacterToSafeResult_LocalOperationFailed:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"移动角色到安全点: 本地操作失败");
			printf("MoveCharacterToSafe result: Operation failed from localbserver\n");
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
