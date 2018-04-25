// GuildLogInfo.cpp: implementation of the CCharacterInfo class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "GuildLogInfo.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CGuildLogInfo::CGuildLogInfo()
{
	
}

CGuildLogInfo::~CGuildLogInfo()
{
	
}

vector <CGlobalStruct::TFLV> CGuildLogInfo::GuildLogInfoMain(char *szGMServerName, char * szGMServerIP,char* szGuildName)
{
	int iIndex=0;
	int iPageSize=0;
	
	
	char strlog[80];
	sprintf(strlog,"大区<%s>帮会<%s>日志信息",szGMServerName,szGuildName);
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
	g_ccNetObj.RegEvent_OnPacket(ENUM_PGPlayerStatus_StoC_GuildLogInfo, GuildLogInfo, 0);
	
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
				
				PGPlayerStatus_CtoS_GuildLogInfo sPkt;
				sPkt.iWorldID = iWorldID;
				sprintf(sPkt.szGuildName, "%s", szGuildName);
				
				// 传送至GMS要求取得公会日志资讯
				g_ccNetObj.Send(sizeof(sPkt), (S_ObjNetPktBase*)&sPkt);	
				g_state = 3;			
			}
			break;
			// 等待取得公会日志资讯
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
					printf("等待取得公会日志资讯中,第%i次\n",n_send);
			}
			break;
		}
	}			
	printf("\n");
	printf("\n==================== Shutdown ====================\n");
	return DBVect;
}

void CGuildLogInfo::LoginResult(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
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
		printf("登陆失败,帐号错误\n");
		
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

void CGuildLogInfo::GuildLogInfo(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
{
	char strInt[10];
	char bufstr[256];
	ZeroMemory(strInt,10);
	ZeroMemory(bufstr,256);
	PGPlayerStatus_StoC_GuildLogInfo *pPkt = (PGPlayerStatus_StoC_GuildLogInfo *)pData;
	CGlobalStruct::TFLV m_tflv;
	
	switch(pPkt->emResult)
	{
	case ENUM_GuildLogInfoResult_Success:
		{
			// 若为结束封包
			if (pPkt->iWorldID == -1)
			{
				g_state = -1;
				return;
			}
			
			char szTemp[128];
			char szLogType[32];
			
			time_t t = pPkt->sLogRecord.iTime;
			struct tm *local = localtime(&t);
			char nowtime[40];
			sprintf(nowtime,"%d-%d-%d %d:%d:%d",local->tm_year+1900,local->tm_mon+1,local->tm_mday,local->tm_hour,local->tm_min,local->tm_sec);

			
			// 设定日志类型名称与内容
			switch (pPkt->emType)
			{
			case ENUM_GuildLog_Fame:
				{
					sprintf(szLogType, "声望");
					sprintf(szTemp, "[%s]贡献了%d声望", pPkt->sLogRecord.szRoleName, 
						pPkt->sLogRecord.iValue); 
					break;
				}	
			case ENUM_GuildLog_Item:
				{
					sprintf(szLogType, "物品");
					// 旗标1(0:提领, 1:存入)
					if(pPkt->sLogRecord.iFlag1 == 0)
					{
						if(pPkt->sLogRecord.iValue == 1)
							sprintf(szTemp, "[%s]提领了%s",		pPkt->sLogRecord.szRoleName, 
							pPkt->sLogRecord.szTargetName);
						else if(pPkt->sLogRecord.iValue > 1)
							sprintf(szTemp, "[%s]提领了%sx%d",	pPkt->sLogRecord.szRoleName, 
							pPkt->sLogRecord.szTargetName, 
							pPkt->sLogRecord.iValue);
					}
					else
					{
						if(pPkt->sLogRecord.iValue == 1)
							sprintf(szTemp, "[%s]存入了%s",		pPkt->sLogRecord.szRoleName, 
							pPkt->sLogRecord.szTargetName);
						else if(pPkt->sLogRecord.iValue > 1)
							sprintf(szTemp, "[%s]存入了%sx%d",	pPkt->sLogRecord.szRoleName, 
							pPkt->sLogRecord.szTargetName, 
							pPkt->sLogRecord.iValue);
					}
					break;
				}
			case ENUM_GuildLog_Money:
				{
					sprintf(szLogType, "文钱");
					if(pPkt->sLogRecord.iValue < 0)
						sprintf(szTemp, "[%s]提领了%d文钱", pPkt->sLogRecord.szRoleName, 
						abs(pPkt->sLogRecord.iValue));
					if(pPkt->sLogRecord.iValue > 0)
						sprintf(szTemp, "[%s]存入了%d文钱", pPkt->sLogRecord.szRoleName, 
						pPkt->sLogRecord.iValue);
					break;
				}
			default:
				{
					sprintf(szLogType, "Unknow Type");
					sprintf(szTemp, "");
					break;
				}
			}// switch

						
			// 输出结果
			printf("\n====================================================\n");
			printf("World: %d\n",		pPkt->iWorldID);
			printf("GuildName: %s\n",	pPkt->szGuildName);
			printf("日志类型: %s\n",	szLogType);
			printf("时间: %s\n",		asctime(local));
			printf("日志内容: %s\n",	szTemp);

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
			m_tflv.m_tagName=CEnumCore::TagName::PAL_LOGTYPE;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(szLogType);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",szLogType);
			DBVect.push_back(m_tflv);

			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_TIME;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(nowtime);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",nowtime);
			DBVect.push_back(m_tflv);

			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_SYSMEMO;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(szTemp);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",szTemp);
			DBVect.push_back(m_tflv);
		}
		break;
		
	case ENUM_GuildLogInfoResult_GuildNotFound:
		g_state = -1;
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"帮会日志信息结果: 帮会 %s 没有找到\n", pPkt->szGuildName);
		printf("GuildLogInfo result: Guild %s not found\n", pPkt->szGuildName);
		m_tflv.nIndex=DBVect.size()+1;
		m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
		m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
		m_tflv.m_tvlength=strlen(bufstr);
		sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
		DBVect.push_back(m_tflv);
		break;
		
	case ENUM_GuildLogInfoResult_GuildLogEmpty:
		g_state = -1;
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"帮会日志信息结果: 帮会 %s 日志为空\n", pPkt->szGuildName);
		printf("GuildLogInfo result: Guild %s has no log data\n", pPkt->szGuildName);
		m_tflv.nIndex=DBVect.size()+1;
		m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
		m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
		m_tflv.m_tvlength=strlen(bufstr);
		sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
		DBVect.push_back(m_tflv);
		break;
		
	case ENUM_GuildLogInfoResult_WorldNotFound:
		g_state = -1;
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"帮会日志信息结果: 大区没有找到\n");
		printf("GuildLogInfo result: World not found\n");
		m_tflv.nIndex=DBVect.size()+1;
		m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
		m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
		m_tflv.m_tvlength=strlen(bufstr);
		sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
		DBVect.push_back(m_tflv);
		break;
		
	case ENUM_GuildLogInfoResult_LeaderNotFound:
		g_state = -1;
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"帮会日志信息结果: 频道没有找到\n");
		printf("GuildLogInfo result: Leader not found\n");
		m_tflv.nIndex=DBVect.size()+1;
		m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
		m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
		m_tflv.m_tvlength=strlen(bufstr);
		sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
		DBVect.push_back(m_tflv);
		break;
		
	case ENUM_GuildLogInfoResult_LeaderDisconnect:
		g_state = -1;
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"帮会日志信息结果: 频道没有连接\n");
		printf("GuildLogInfo result: Leader disconnect\n");
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
}