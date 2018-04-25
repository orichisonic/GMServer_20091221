// ProsecuteList.cpp: implementation of the CCharacterInfo class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "ProsecuteList.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CProsecuteList::CProsecuteList()
{
	
}

CProsecuteList::~CProsecuteList()
{
	
}

vector <CGlobalStruct::TFLV> CProsecuteList::ProsecuteListMain(char *szGMServerName, char * szGMServerIP)//,char* szRoleName)
{
	int iIndex=0;
	int iPageSize=0;
	
	
	char strlog[80];
	sprintf(strlog,"大区<%s>角色<%s>检举列表信息",szGMServerName);
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
	g_ccNetObj.RegEvent_OnPacket(ENUM_PGServerStatus_StoC_ProsecuteList, ProsecuteList, 0);
	
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
	ZeroMemory(szAccount,32);
	ZeroMemory(szPassword,32);
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
				// 设定GMS连线参数
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
				int  iWorldID = 0;		// 世界编号
				iWorldID=operPal.FindWorldID(szGMServerName,szGMServerIP);
				
				PGServerStatus_CtoS_ProsecuteList sPkt;
				sPkt.iWorldID = iWorldID;
				///sprintf(sPkt.szRoleName, "%s", szRoleName);
				
				// 传送至GMS要求取得玩家检举列表
				g_ccNetObj.Send(sizeof(sPkt), (S_ObjNetPktBase*)&sPkt);	
				g_state = 3;			
			}
			break;
			// 等待取得玩家检举列表
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
				printf("等待取得检举列表信息中,第%i次\n",n_send);
			}
			break;
		}
	}			
	printf("\n");
	printf("\n==================== Shutdown ====================\n");
	return DBVect;
}

void CProsecuteList::LoginResult	(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
{
	char bufstr[256];

	PGGMCConnection_StoC_LoginResult *pPkt = (PGGMCConnection_StoC_LoginResult *)pData;
	switch(pPkt->emResult)
	{
	case ENUM_GMCLoginResult_Success:
		printf("Login success\n");
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

void CProsecuteList::ProsecuteList(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
{
	char strInt[10];
	char bufstr[256];
	CGlobalStruct::TFLV m_tflv;

	PGServerStatus_StoC_ProsecuteList *pPkt = (PGServerStatus_StoC_ProsecuteList *)pData;
	
	switch(pPkt->emResult)
	{
	case ENUM_ProsecuteListResult_Success:
		{
			// 若为结束封包
			if (pPkt->iWorldID == -1)
			{
				g_state = -1;
				printf("\n<<< Ending to get prosecute list >>>\n\n");
				return;
			}
			
			// 输出结果
			printf("\n====================================================\n");
			printf("World: %d\n",		pPkt->iWorldID);
			printf("Type: %d\n",		pPkt->iType);
			printf("RoleID: %d\n",		pPkt->iRoleID);
			printf("RoleName: %s\n",	pPkt->szRoleName);
			printf("ZoneID: %d\n",		pPkt->iZoneID);
			printf("Time: %s\n",		pPkt->szTime);
			printf("FromID: %d\n",		pPkt->iFromID);
			printf("FromName: %s\n",	pPkt->szFromName);

			_itoa(pPkt->iWorldID,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_WORLDID;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);
			
			_itoa(pPkt->iType,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_EVENTTYPE;//检举类型
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);

			_itoa(pPkt->iRoleID,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_ROLEID;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);

			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_ROLENAME;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(pPkt->szRoleName);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",pPkt->szRoleName);
			DBVect.push_back(m_tflv);

			_itoa(pPkt->iZoneID,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_ZONEID;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);

			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_TIME;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(pPkt->szTime);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",pPkt->szTime);
			DBVect.push_back(m_tflv);

			_itoa(pPkt->iFromID,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_FROMID;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);

			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_FROMNAME;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(pPkt->szFromName);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",pPkt->szFromName);
			DBVect.push_back(m_tflv);
		}
		break;
		
	case ENUM_ProsecuteListResult_WorldNotFound:
		{
			g_state = -1;
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"检举列表: 大区没有找到\n");
			printf("ProsecuteList result: World not found\n");
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(bufstr);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
			DBVect.push_back(m_tflv);
		}
		break;
		
	case ENUM_ProsecuteListResult_LeaderNotFound:
		{
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"检举列表: 频道没有找到\n");
			printf("ProsecuteList result: Leader not found\n");
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(bufstr);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
			DBVect.push_back(m_tflv);
			g_state = -1;
		}
		break;
		
	case ENUM_ProsecuteListResult_LeaderDisconnect:
		{
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"检举列表: 频道没有连接\n");
			printf("ProsecuteList result: Leader disconnect\n");
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(bufstr);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
			DBVect.push_back(m_tflv);
			g_state = -1;
		}
		break;
		
	case ENUM_ProsecuteListResult_ProsecuteNotFound:
		{
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"检举列表: 检举列表为空\n");
			printf("ProsecuteList result: Prosecute list is empty\n");
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(bufstr);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
			DBVect.push_back(m_tflv);
			g_state = -1;
		}
		break;//return;
		
	default:
		break;
	}
}