#include "stdafx.h"
#include "SendCustomItemMail.h"

CSendCustomItemMail::CSendCustomItemMail()
{
	
}

CSendCustomItemMail::~CSendCustomItemMail()
{
	
}

vector <CGlobalStruct::TFLV> CSendCustomItemMail::SendCustomItemMailMain(char * szGMServerName,char * szGMServerIP,char * szRoleName,char * szSubject,
						char * szContent,int iMoney, int itemType, int iCustomItemID, char *szCustomItemIDName,bool bBindSetup, char *szInherit,char *szReason)
{
	
	char strlog[80];
	ZeroMemory(strlog,80);
	sprintf(strlog,"大区<%s>角色<%s>寄送客制物品邮件",szGMServerName,szRoleName);
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
	g_ccNetObj.RegEvent_OnPacket(ENUM_PGServerControl_StoC_SendCustomItemMail, SendCustomItemMail, 0);
	
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
	int iInherit[10] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};

	ZeroMemory(szAccount,32);
	ZeroMemory(szPassword,32);

	char seps[] = ",";
	char *token;
	int iInheritIndex=0;
	token = strtok(szInherit, seps);
	
	while( token != NULL )
	{
		iInherit[iInheritIndex]= atoi(token);
		token = strtok( NULL, seps );
		iInheritIndex++;
	}


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
				
				iWorldID=operPal.FindWorldID(szGMServerName,szGMServerIP);

				PGServerControl_CtoS_SendCustomItemMail sPkt;
				sPkt.iWorldID	= iWorldID;
				strcpy(sPkt.szRoleName, szRoleName);
				strcpy(sPkt.szSubject, szSubject);
				strcpy(sPkt.szContent, szContent);
				sPkt.iMoney	= iMoney;
				sPkt.iCustomItemID	= iCustomItemID;
				sPkt.bBindSetup	= bBindSetup;
				memcpy(sPkt.iInherit, iInherit, sizeof(int)*10);/* 等待确认*/
				strcpy(sPkt.szReason, szReason);
				
				// 传送至GMS要求寄送客制物品邮件
				g_ccNetObj.Send(sizeof(sPkt), (S_ObjNetPktBase*)&sPkt);	
				g_state = 3;		
			}
			break;
			// 等待寄送客制物品邮件
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
			printf("等待寄送客制物品邮件中,第%i次\n",n_send);
			break;
		}
	}			
	g_ccNetObj.Disconnect();
	printf("\n");
	printf("\n==================== Shutdown ====================\n");

	
	return DBVect;
}

void CSendCustomItemMail::LoginResult(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
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
// 寄送客制物品邮件结果-----------------------------------------------------------------------------------
void CSendCustomItemMail::SendCustomItemMail(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
{
	char bufstr[256];

	PGServerControl_StoC_SendCustomItemMail *pPkt = (PGServerControl_StoC_SendCustomItemMail *)pData;

	switch(pPkt->emResult)
	{
	case ENUM_SendCustomItemMailResult_Success:	
		// 输出结果
		// 输出结果
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"寄送客制物品邮件成功\n");

		printf("SendCustomItemMail success\n");
		printf("WorldID: %d\n"		, pPkt->iWorldID);
		printf("RoleName: %s\n"		, pPkt->szRoleName);
		printf("Subject: %s\n"		, pPkt->szSubject);
		printf("Money: %d\n"		, pPkt->iMoney);
		printf("CustomItemID: %d\n"	, pPkt->iCustomItemID);
		printf("BindSetup: %d\n"	, pPkt->bBindSetup);
		printf("基本值(字首): %d\n"	, pPkt->iInherit[0]);
		printf("附魔: %d\n"			, pPkt->iInherit[1]);
		printf("冲物理: %d\n"		, pPkt->iInherit[2]);
		printf("冲法术: %d\n"		, pPkt->iInherit[3]);
		printf("玄石1: %d\n"		, pPkt->iInherit[4]);
		printf("玄石2: %d\n"		, pPkt->iInherit[5]);
		printf("玄石3: %d\n"		, pPkt->iInherit[6]);
		printf("玄石4: %d\n"		, pPkt->iInherit[7]);
		printf("玄石5: %d\n"		, pPkt->iInherit[8]);
		printf("玄石6: %d\n"		, pPkt->iInherit[9]);
		break;
		
	case ENUM_SendCustomItemMailResult_ArgumentError:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"寄送客制物品邮件：参数不正确");
		printf("SendCustomItemMail result: Argument error\n");
		break;	

	case ENUM_SendCustomItemMailResult_ItemIDError:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"寄送客制物品邮件物品：物品编号不正确");
		printf("SendCustomItemMail result: Incorrect CustomItemID %d\n", pPkt->iCustomItemID);
		break;	
		
	case ENUM_SendCustomItemMailResult_WorldNotFound:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"寄送客制物品邮件物品：找不到世界");
		printf("SendCustomItemMail result: World not found\n");
		break;	
		
	case ENUM_SendCustomItemMailResult_LeaderNotFound:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"寄送客制物品邮件物品：找不到领头观测者");
		printf("SendCustomItemMail result: Leader not found\n");
		break;
		
	case ENUM_SendCustomItemMailResult_LeaderDisconnect:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"寄送客制物品邮件物品：领头观测者未连线");
		printf("SendCustomItemMail result: Leader disconnect\n");
		break;
		
	case ENUM_SendCustomItemMailResult_RoleNotFound:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"寄送客制物品邮件物品：角色不存在");
		printf("SendCustomItemMail result: Character %s not found\n", pPkt->szRoleName);
		break;
		
	case ENUM_SendCustomItemMailResult_LocalDisconnect:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"寄送客制物品邮件物品：区域伺服器未连线");
		printf("SendCustomItemMail result: Local server disconnect");
		break;
		
	case ENUM_SendCustomItemMailResult_LocalOperationFailed:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"寄送客制物品邮件物品：操作失败\n");
		printf("SendCustomItemMail result: Operation failed from localserver\n");
		break;
		
	default:
		break;
	}//switch
	g_state = -1;

	CGlobalStruct::TFLV m_tflv;
	m_tflv.nIndex=DBVect.size()+1;
	m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
	m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
	m_tflv.m_tvlength=strlen(bufstr);
	sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
	DBVect.push_back(m_tflv);
}
