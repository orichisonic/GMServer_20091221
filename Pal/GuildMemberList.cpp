// GuildMemberList.cpp: implementation of the CGuildMemberList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GuildMemberList.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGuildMemberList::CGuildMemberList()
{

}

CGuildMemberList::~CGuildMemberList()
{

}
vector <CGlobalStruct::TFLV> CGuildMemberList::GuildMemberListMain(char *szGMServerName, char * szGMServerIP,char* szGuildName)
{

		int iIndex=0;
		int iPageSize=0;

		char strlog[80];
		sprintf(strlog,"大区<%s>帮会<%s>成员列表",szGMServerName,szGuildName);
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
		g_ccNetObj.RegEvent_OnPacket(ENUM_PGPlayerStatus_StoC_GuildMemberList, GuildMemberList, 0);
		
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
					//int  iWorldID = 0;							// 世界编号
					//char szGuildName[32] = "ＫＩＬＬ　ＩＤ";	// 公会名称
					iWorldID=operPal.FindWorldID(szGMServerName,szGMServerIP);
							
					PGPlayerStatus_CtoS_GuildMemberList sPkt;
					sPkt.iWorldID = iWorldID;
					sprintf(sPkt.szGuildName, "%s", szGuildName);
					
					// 传送至GMS要求取得公会成员列表
					g_ccNetObj.Send(sizeof(sPkt), (S_ObjNetPktBase*)&sPkt);	
					g_state = 3;			
				}
				break;
				// 等待取得公会成员列表
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
				printf("等待取得公会成员列表中,第%i次\n",n_send);
				break;
			}
		}			
		g_ccNetObj.Disconnect();
		printf("\n");
		printf("\n==================== Shutdown ====================\n");
		return DBVect;

	
}
void CGuildMemberList::LoginResult(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
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
void CGuildMemberList::GuildMemberList(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
{

			CGlobalStruct::TFLV m_tflv;
			PGPlayerStatus_StoC_GuildMemberList *pPkt = (PGPlayerStatus_StoC_GuildMemberList *)pData;
			char strInt[10];
			char bufstr[256];
			ZeroMemory(strInt,10);
			ZeroMemory(bufstr,256);
			switch(pPkt->emResult)
			{
			case ENUM_GuildMemberListResult_Success:
				{
					// 若为结束封包
					if (pPkt->iWorldID == -1)
					{
						g_state = -1;
						return;
					}
					
					time_t t = pPkt->iJoinDate;
					struct tm *local = localtime(&t);
					char nowtime[40];
					sprintf(nowtime,"%d-%d-%d %d:%d:%d",local->tm_year+1900,local->tm_mon+1,local->tm_mday,local->tm_hour,local->tm_min,local->tm_sec);
					
					// 输出结果
					printf("\n====================================================\n");
					printf("World: %d\n",			pPkt->iWorldID);
					printf("GuildName: %s\n",		pPkt->szGuildName);
					printf("成员角色名称: %s\n",	pPkt->szRoleName);
					printf("职等: %d\n",			pPkt->iLevel);	
					printf("加入时间: %s\n",		asctime(local));
					
					
					_itoa(pPkt->iWorldID,strInt,10);
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_WORLDID;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);
					
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_GUILDNAME;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=strlen(pPkt->szGuildName);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",pPkt->szGuildName);
					DBVect.push_back(m_tflv);

					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_ROLENAME;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=strlen(pPkt->szRoleName);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",pPkt->szRoleName);
					DBVect.push_back(m_tflv);
					
					_itoa(pPkt->iLevel,strInt,10);
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_GUILDPOWERLEVEL;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);

					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_GUILDJOINTIME;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=strlen(nowtime);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",nowtime);
					DBVect.push_back(m_tflv);
					
				}
				break;
				
			case ENUM_GuildMemberListResult_GuildNotFound:
				ZeroMemory(bufstr,256);
				sprintf(bufstr,"帮会成员列表结果: 帮会 %s 没有找到\n", pPkt->szGuildName);
				printf("Guildmemberlist result: Guild %s not found\n", pPkt->szGuildName);
				m_tflv.nIndex=DBVect.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(bufstr);
				sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
				DBVect.push_back(m_tflv);
				g_state = -1;
				break;
				
			case ENUM_GuildMemberListResult_WorldNotFound:
				ZeroMemory(bufstr,256);
				sprintf(bufstr,"帮会成员列表结果:大区没有找到\n");
				printf("Guildmemberlist result: World not found\n");
				m_tflv.nIndex=DBVect.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(bufstr);
				sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
				DBVect.push_back(m_tflv);
				g_state = -1;
				break;
				
			case ENUM_GuildMemberListResult_LeaderNotFound:
				ZeroMemory(bufstr,256);
				sprintf(bufstr,"帮会成员列表结果:频道没有找到\n");
				printf("Guildmemberlist result: Leader not found\n");
				m_tflv.nIndex=DBVect.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(bufstr);
				sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
				DBVect.push_back(m_tflv);
				g_state = -1;
				break;
				
			case ENUM_GuildMemberListResult_LeaderDisconnect:
				ZeroMemory(bufstr,256);
				sprintf(bufstr,"帮会成员列表结果:频道没有连接\n");
				printf("Guildmemberlist result: Leader disconnect\n");
				m_tflv.nIndex=DBVect.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(bufstr);
				sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
				DBVect.push_back(m_tflv);
				g_state = -1;
				break;
			default:
				break;
		}//switch	

}
