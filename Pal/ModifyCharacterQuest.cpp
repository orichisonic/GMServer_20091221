// ModifyCharacterQuest.cpp: implementation of the CModifyCharacterQuest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ModifyCharacterQuest.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CModifyCharacterQuest::CModifyCharacterQuest()
{

}

CModifyCharacterQuest::~CModifyCharacterQuest()
{

}
vector <CGlobalStruct::TFLV> CModifyCharacterQuest::ModifyCharacterQuestMain(int userByID,char *szGMServerName, char * szGMServerIP,char* szRoleName,int iQuestID,int iQuestStatus)
{


		char strlog[80];
		ZeroMemory(strlog,80);
		sprintf(strlog,"大区<%s>修改角色<%s>任务",szGMServerName,szRoleName);
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
		g_ccNetObj.RegEvent_OnPacket(ENUM_PGPlayerControl_StoC_ModifyCharacterQuest, ModifyCharacterQuest, 0);
		
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
					operPal.GetUserNamePwd("User3",szAccount,szPassword,&iGMServerPort);

					
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
					//int  iWorldID		= 0;		// 世界编号
					//char szRoleName[32] = "Kitty";	// 角色名称
					//	int  iQuestID		= 1702;		// 任务编号(0~6143)
					//	int  iQuestStatus	= 2;		// 任务状态(0:未解 1:已完成 2:已接)

					iWorldID=operPal.FindWorldID(szGMServerName,szGMServerIP);

					
					PGPlayerControl_CtoS_ModifyCharacterQuest sPkt;
					sPkt.iWorldID		= iWorldID;
					sprintf(sPkt.szRoleName, "%s", szRoleName);
					sPkt.iQuestID		= iQuestID;
					sPkt.iQuestStatus	= iQuestStatus;
					
					// 传送至GMS要求修改角色任务
					g_ccNetObj.Send(sizeof(sPkt), (S_ObjNetPktBase*)&sPkt);	
					g_state = 3;			
				}
				break;
				// 等待修改角色任务
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
				printf("等待修改角色任务中,第%i次\n",n_send);
				break;
			}
		}			
		g_ccNetObj.Disconnect();
		printf("\n");
		printf("\n==================== Shutdown ====================\n");
		

		char text[256];
		char queststatus[256];
		ZeroMemory(text,256);
		ZeroMemory(queststatus,256);
		switch(iQuestStatus)
		{
		case 0:
			sprintf(queststatus,"未接");
			break;
		case 1:
			sprintf(queststatus,"已完成");
			break;
		case 2:
			sprintf(queststatus,"已接");
			break;
		default:
			break;
		}
		ZeroMemory(text,256);
		sprintf(text,"任务名:%s,任务状态:%s",Pal_GetQuestName(iQuestID),queststatus);	
		operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYGUILDNAME,szGMServerName,&(COperVector(DBVect)),text);
		return DBVect;
}

void CModifyCharacterQuest::LoginResult(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
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
// 修改角色任务结果-----------------------------------------------------------------------------------
void CModifyCharacterQuest::ModifyCharacterQuest(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
{

		PGPlayerControl_StoC_ModifyCharacterQuest *pPkt = (PGPlayerControl_StoC_ModifyCharacterQuest *)pData;
		char strInt[10];
		char bufstr[256];
		ZeroMemory(strInt,10);
		ZeroMemory(bufstr,256);
		
		switch(pPkt->emResult)
		{
		case ENUM_ModifyCharacterQuestResult_Success:		
			printf("ModifyCharacterQuest succsee\n");
			printf("WorldID: %d\n"		, pPkt->iWorldID);
			printf("RoleName: %s\n"		, pPkt->szRoleName);
			printf("QuestID: %d\n"		, pPkt->iQuestID);
			printf("QuestStatus: %d\n"	, pPkt->iQuestStatus);
			
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"修改角色任务成功");

			/*_itoa(pPkt->iWorldID,strInt,10);
			pOperVector->AddTFLV(CEnumCore::TagName::PAL_WORLDID,CEnumCore::TagFormat::TLV_INTEGER,sizeof(int),(unsigned char *)&strInt);
			pOperVector->AddTFLV(CEnumCore::TagName::PAL_ROLENAME,CEnumCore::TagFormat::TLV_STRING,strlen(pPkt->szRoleName),(unsigned char *)pPkt->szRoleName);
			_itoa(pPkt->iQuestID,strInt,10);
			pOperVector->AddTFLV(CEnumCore::TagName::PAL_QUESTID,CEnumCore::TagFormat::TLV_INTEGER,sizeof(int),(unsigned char *)&strInt);
			_itoa(pPkt->iQuestStatus,strInt,10);
			pOperVector->AddTFLV(CEnumCore::TagName::PAL_QUESTTYPE,CEnumCore::TagFormat::TLV_INTEGER,sizeof(int),(unsigned char *)&strInt);
*/
			break;
		case ENUM_ModifyCharacterQuestResult_WorldNotFound:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"修改角色任务结果: 大区没有找到\n");
			printf("ModifyCharacterQuest result: World not found\n");
			break;	
		case ENUM_ModifyCharacterQuestResult_LeaderNotFound:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"修改角色任务结果: 频道没有找到\n");
			printf("ModifyCharacterQuest result: Leader not found\n");
			break;
		case ENUM_ModifyCharacterQuestResult_LeaderDisconnect:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"修改角色任务结果: 频道没有连接\n");
			printf("ModifyCharacterQuest result: Leader disconnect\n");
			break;
		case ENUM_ModifyCharacterQuestResult_ArgumentError:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"修改角色任务结果: 参数错误\n");
			printf("ModifyCharacterQuest result: Argument error\n");
			break;	
		case ENUM_ModifyCharacterQuestResult_RoleNotFound:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"修改角色任务结果:角色 %s 没有找到\n", pPkt->szRoleName);
			printf("ModifyCharacterQuest result: Character %s not found\n", pPkt->szRoleName);
			break;	
		case ENUM_ModifyCharacterQuestResult_RoleIsOnline:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"修改角色任务结果:角色 %s 在线\n", pPkt->szRoleName);
			printf("ModifyCharacterQuest result: Character %s is online now\n", pPkt->szRoleName);
			break;	
		case ENUM_ModifyCharacterQuestResult_GMOOperationFailed:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"修改角色任务结果: CMObserver操作失败\n");
			printf("ModifyCharacterQuest result: Operation failed from GMObserver\n");
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
