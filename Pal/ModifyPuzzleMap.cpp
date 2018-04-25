// ModifyPuzzleMap.cpp: implementation of the CModifyPuzzleMap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ModifyPuzzleMap.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CModifyPuzzleMap::CModifyPuzzleMap()
{

}

CModifyPuzzleMap::~CModifyPuzzleMap()
{

}
vector<CGlobalStruct::TFLV> CModifyPuzzleMap::ModifyPuzzleMapMain(char * szGMServerName,char * szGMServerIP,char * szRoleName,char *strPuzzleMap,
																  int iMapID1,int iMapID2,int iMapID3,int iMapID4,int iMapID5,int iMapID6,int iMapID7,int iMapID8,int iMapID9)
{	


			char strlog[256];
			sprintf(strlog,"大区<%s>角色<%s>修改角色图鉴",szGMServerName,szRoleName);
			logFile.WriteText("仙剑OL",strlog);

			CGlobalStruct::TFLV m_tflv;
			if(!DBVect.empty())
			{
				DBVect.clear();
			}

			Pal_LoadData(".\\data");

			// 注册连线CallBack函式
			g_ccNetObj.RegEvent_OnConnected(COperPal::OnConnect, 0);
			g_ccNetObj.RegEvent_OnDisconnected(COperPal::OnDisconnect, 0);
			g_ccNetObj.RegEvent_OnConnectFailed(COperPal::OnConnectfailed, 0);

			// 注册对应事件CallBack函式
			g_ccNetObj.RegEvent_OnPacket(ENUM_PGGMCConnection_StoC_LoginResult,	LoginResult, 0);
			g_ccNetObj.RegEvent_OnPacket(ENUM_PGPlayerControl_StoC_ModifyPuzzleMap, ModifyPuzzleMap, 0);

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
						S_GMPuzzleMapProc sGMPuzzleMapProc;// 图鉴操作资讯
						sGMPuzzleMapProc.iZoneID =operPal.GetPuzzleMap(strPuzzleMap);			// 地图编号
						if(iMapID1==0)
						{
							sGMPuzzleMapProc.bIsOpen[0] = false;	// 设定第1张拼图开关
						}
						else
						{	
							sGMPuzzleMapProc.bIsOpen[0] = true;	// 设定第1张拼图开关
						}
						if(iMapID2==0)
						{
							sGMPuzzleMapProc.bIsOpen[1] = false;	// 设定第1张拼图开关
						}
						else
						{	
							sGMPuzzleMapProc.bIsOpen[1] = true;	// 设定第1张拼图开关
						}
						if(iMapID3==0)
						{
							sGMPuzzleMapProc.bIsOpen[2] = false;	// 设定第1张拼图开关
						}
						else
						{	
							sGMPuzzleMapProc.bIsOpen[2] = true;	// 设定第1张拼图开关
						}
						if(iMapID4==0)
						{
							sGMPuzzleMapProc.bIsOpen[3] = false;	// 设定第1张拼图开关
						}
						else
						{	
							sGMPuzzleMapProc.bIsOpen[3] = true;	// 设定第1张拼图开关
						}
						if(iMapID5==0)
						{
							sGMPuzzleMapProc.bIsOpen[4] = false;	// 设定第1张拼图开关
						}
						else
						{	
							sGMPuzzleMapProc.bIsOpen[4] = true;	// 设定第1张拼图开关
						}
						if(iMapID6==0)
						{
							sGMPuzzleMapProc.bIsOpen[5] = false;	// 设定第1张拼图开关
						}
						else
						{	
							sGMPuzzleMapProc.bIsOpen[5] = true;	// 设定第1张拼图开关
						}
						if(iMapID7==0)
						{
							sGMPuzzleMapProc.bIsOpen[6] = false;	// 设定第1张拼图开关
						}
						else
						{	
							sGMPuzzleMapProc.bIsOpen[6] = true;	// 设定第1张拼图开关
						}
						if(iMapID8==0)
						{
							sGMPuzzleMapProc.bIsOpen[7] = false;	// 设定第1张拼图开关
						}
						else
						{	
							sGMPuzzleMapProc.bIsOpen[7] = true;	// 设定第1张拼图开关
						}
						if(iMapID9==0)
						{
							sGMPuzzleMapProc.bIsOpen[8] = false;	// 设定第1张拼图开关
						}
						else
						{	
							sGMPuzzleMapProc.bIsOpen[8] = true;	// 设定第1张拼图开关
						}
						
		
						Pal_GetMobPuzzleMapFlag(&sGMPuzzleMapProc);	// 取得图鉴旗标编号
				
						PGPlayerControl_CtoS_ModifyPuzzleMap sPkt;
						sPkt.iWorldID			= iWorldID;
						sprintf(sPkt.szRoleName, "%s", szRoleName);
						sPkt.sGMPuzzleMapProc	= sGMPuzzleMapProc;
						
						// 传送至GMS要求修改角色图鉴
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
					printf("等待取得修改角色图鉴中,第%i次\n",n_send);
					break;
				}
			}			
			g_ccNetObj.Disconnect();
			printf("\n");
			printf("\n==================== Shutdown ====================\n");
			return DBVect;
}
void CModifyPuzzleMap::LoginResult(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
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
void CModifyPuzzleMap::ModifyPuzzleMap(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
{
	PGPlayerControl_StoC_ModifyPuzzleMap *pPkt = (PGPlayerControl_StoC_ModifyPuzzleMap *)pData;
	char bufstr[256];	
	switch(pPkt->emResult)
	{
	case ENUM_ModifyPuzzleMapResult_Success:
		{
			printf("ModifyPuzzleMap success\n");
			printf("WorldID: %d\n"	, pPkt->iWorldID);
			printf("RoleName: %s\n"	, pPkt->szRoleName);
			
			printf("ZoneID: %d\n"	, pPkt->sGMPuzzleMapProc.iZoneID);
			printf("ZoneName: %s\n" , Pal_GetZoneName(pPkt->sGMPuzzleMapProc.iZoneID));
			printf("0=关闭,1=开启:\n");
			for (int i=0; i<PUZZLEMAP_CUTCOUNT; i++)
			{
				printf("Pic %d: %d\n", i+1, pPkt->sGMPuzzleMapProc.bIsOpen[i]);
			}
			
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"修改角色图鉴: 操作成功\n");
		}
		break;
	case ENUM_ModifyPuzzleMapResult_WorldNotFound:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"修改角色图鉴: 世界没有找到\n");
		printf("ModifyPuzzleMap result: World not found\n");
		break;	
	case ENUM_ModifyPuzzleMapResult_LeaderNotFound:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"修改角色图鉴: 观测者没有找到\n");
		printf("ModifyPuzzleMap result: Leader not found\n");
		break;
	case ENUM_ModifyPuzzleMapResult_LeaderDisconnect:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"修改角色图鉴: 观测者没有连接\n");
		printf("ModifyPuzzleMap result: Leader disconnect\n");
		break;	
	case ENUM_ModifyPuzzleMapResult_RoleNotFound:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"修改角色图鉴: 角色 %s 没有找到\n", pPkt->szRoleName);
		printf("ModifyPuzzleMap result: Character %s not found\n", pPkt->szRoleName);
		break;	
	case ENUM_ModifyPuzzleMapResult_RoleIsOnline:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"修改角色图鉴: 角色 %s 现在在线\n", pPkt->szRoleName);
		printf("ModifyPuzzleMap result: Character %s is online now\n", pPkt->szRoleName);
		break;	
	case ENUM_ModifyPuzzleMapResult_FlagError:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"修改角色图鉴: 旗标错误\n");
		printf("ModifyPuzzleMap result: Flag error\n");
		break;	
	case ENUM_ModifyPuzzleMapResult_GMOOperationFailed:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"修改角色图鉴: 服务器操作失败\n");
		printf("ModifyPuzzleMap result: Operation failed from GMObserver\n");
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
