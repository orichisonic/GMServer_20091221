// ModifyGuildNotice.cpp: implementation of the CModifyGuildNotice class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ModifyGuildNotice.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CModifyGuildNotice::CModifyGuildNotice()
{

}

CModifyGuildNotice::~CModifyGuildNotice()
{

}

vector <CGlobalStruct::TFLV> CModifyGuildNotice::ModifyGuildNoticeMain(char *szGMServerName, char * szGMServerIP,char* szGuildName,char * szNotice)
{
	char strlog[80];
	sprintf(strlog,"大区<%s>修改帮会<%s>公告<%s>",szGMServerName,szGuildName,szNotice);
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
	g_ccNetObj.RegEvent_OnPacket(ENUM_PGPlayerControl_StoC_ModifyGuildNotice, ModifyGuildNotice, 0);
	
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
				// 设定GMS连线参数
				operPal.GetUserNamePwd("User5",szAccount,szPassword,&iGMServerPort);
			//	char szGMServerIP[16] = "172.18.103.250";	// GMServer IP位址
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
					//char szAccount[32] = "silent2";			// 登入帐号
					//char szPassword[32] = "silent2";		// 登入密码		
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

				PGPlayerControl_CtoS_ModifyGuildNotice sPkt;
				sPkt.iWorldID = iWorldID;
				sprintf(sPkt.szGuildName, "%s", szGuildName);
				sprintf(sPkt.szNotice, "%s", szNotice);

				// 传送至GMS要求修改公会公告
				g_ccNetObj.Send(sizeof(sPkt), (S_ObjNetPktBase*)&sPkt);	
				g_state = 3;
		
			}
			break;
			// 等待修改公会成员职阶
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
			printf("等待修改公会公告中,第%i次\n",n_send);
			break;
		}
	}			
	g_ccNetObj.Disconnect();
	printf("\n");
	printf("\n==================== Shutdown ====================\n");
	return DBVect;
}

void CModifyGuildNotice::LoginResult(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
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
// 修改公会公告结果-----------------------------------------------------------------------------------
void CModifyGuildNotice::ModifyGuildNotice(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
{
	PGPlayerControl_StoC_ModifyGuildNotice *pPkt = (PGPlayerControl_StoC_ModifyGuildNotice *)pData;
	char bufstr[256];
	ZeroMemory(bufstr,256);

	switch(pPkt->emResult)
	{
	case ENUM_ModifyGuildNoticeResult_Success:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"修改帮会公告成功,大区号:%d,帮会名:%s,公告内容:%s\n",
			pPkt->iWorldID,
			pPkt->szGuildName,
			pPkt->szNotice);
		printf("ModifyGuildLv succsee\n");
		printf("WorldID: %d\n"		, pPkt->iWorldID);
		printf("GuiildName: %s\n"	, pPkt->szGuildName);
		printf("Notice: %s\n"		, pPkt->szNotice);
		break;
	case ENUM_ModifyGuildNoticeResult_WorldNotFound:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"ModifyGuildNotice result: World not found\n");
		printf("ModifyGuildNotice result: World not found\n");
		break;	
	case ENUM_ModifyGuildNoticeResult_LeaderNotFound:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"ModifyGuildNotice result: Leader not found\n");
		printf("ModifyGuildNotice result: Leader not found\n");
		break;
	case ENUM_ModifyGuildNoticeResult_LeaderDisconnect:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"ModifyGuildNotice result: Leader disconnect\n");
		printf("ModifyGuildNotice result: Leader disconnect\n");
		break;
	case ENUM_ModifyGuildNoticeResult_GuildNotFound:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"ModifyGuildNotice result: Guild %s not found\n", pPkt->szGuildName);
		printf("ModifyGuildNotice result: Guild %s not found\n", pPkt->szGuildName);
		break;	
	case ENUM_ModifyGuildNoticeResult_GMOOperationFailed:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"ModifyGuildNotice result: Operation failed from GMObserver\n");
		printf("ModifyGuildNotice result: Operation failed from GMObserver\n");
		break;	
	case ENUM_ModifyGuildNoticeResult_ChatOperationFailed:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"ModifyGuildNotice result: Operation failed from chatserver\n");
		printf("ModifyGuildNotice result: Operation failed from chatserver\n");
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