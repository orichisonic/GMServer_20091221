// ModifyCharacterDropPw.cpp: implementation of the CModifyCharacterDropPw class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ModifyCharacterDropPw.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CModifyCharacterDropPw::CModifyCharacterDropPw()
{

}

CModifyCharacterDropPw::~CModifyCharacterDropPw()
{

}
vector <CGlobalStruct::TFLV> CModifyCharacterDropPw::ModifyCharacterDropPwMain(char *szGMServerName, char * szGMServerIP,char* szRoleName,char * szDropRolePw)
{
			/*char strlog[80];
			sprintf(strlog,"大区<%s>修改角色<%s>删除密码",szGMServerName,szRoleName);
			logFile.WriteText("仙剑OL",strlog);*/

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
			g_ccNetObj.RegEvent_OnPacket(ENUM_PGPlayerControl_StoC_ModifyCharacterDropPw, ModifyCharacterDropPw, 0);
			
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
						//int  iWorldID			= 0;				// 世界编号
						//char szRoleName[32]		= "Kitty";			// 角色名称
						//char szDropRolePw[13]	= "123456";	// 新删角密码(6~12英数字)
						
						iWorldID=operPal.FindWorldID(szGMServerName,szGMServerIP);

						PGPlayerControl_CtoS_ModifyCharacterDropPw sPkt;
						sPkt.iWorldID	= iWorldID;
						sprintf(sPkt.szRoleName, "%s", szRoleName);
						sprintf(sPkt.szDropRolePw, "%s", szDropRolePw);
						
						// 传送至GMS要求修改角色删除密码
						g_ccNetObj.Send(sizeof(sPkt), (S_ObjNetPktBase*)&sPkt);	
						g_state = 3;	

					}
					break;
				// 等待修改公会等级
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
					printf("等待修改公会等级中,第%i次\n",n_send);
					break;
				}
			}			
			g_ccNetObj.Disconnect();
			printf("\n");
			printf("\n==================== Shutdown ====================\n");
			return DBVect;

}

void CModifyCharacterDropPw::LoginResult(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
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
// 修改角色删除密码结果-------------------------------------------------------------------------------
void CModifyCharacterDropPw::ModifyCharacterDropPw(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
{
	PGPlayerControl_StoC_ModifyCharacterDropPw *pPkt = (PGPlayerControl_StoC_ModifyCharacterDropPw *)pData;
	char bufstr[256];
	switch(pPkt->emResult)
	{
	case ENUM_ModifyCharacterDropPwResult_Success:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"修改删除角色密码成功,大区号:%d,角色名:%s\n",pPkt->iWorldID,pPkt->szRoleName);
		printf("ModifyCharacterDropPw succsee\n");
		printf("WorldID: %d\n"	, pPkt->iWorldID);
		printf("RoleName: %s\n"	, pPkt->szRoleName);
		break;
	case ENUM_ModifyCharacterDropPwResult_WorldNotFound:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"修改删除角色密码结果: 大区没有找到\n");	
		printf("ModifyCharacterDropPw result: World not found\n");
		break;	
	case ENUM_ModifyCharacterDropPwResult_LeaderNotFound:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"修改删除角色密码结果: 频道没有找到\n");	
		printf("ModifyCharacterDropPw result: Leader not found\n");
		break;
	case ENUM_ModifyCharacterDropPwResult_LeaderDisconnect:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"修改删除角色密码结果: 频道没有连接\n");	
		printf("ModifyCharacterDropPw result: Leader disconnect\n");
		break;
	case ENUM_ModifyCharacterDropPwResult_PasswordError:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"修改删除角色密码结果: 密码格式错误\n");	
		printf("ModifyCharacterDropPw result: Password format error\n");
		break;
	case ENUM_ModifyCharacterDropPwResult_RoleNotFound:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"修改删除角色密码结果:  角色 %s 没有找到\n", pPkt->szRoleName);
		printf("ModifyCharacterDropPw result: Character %s not found\n", pPkt->szRoleName);
		break;	
	case ENUM_ModifyCharacterDropPwResult_RoleIsOnline:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"修改删除角色密码结果:  角色 %s 正在线\n", pPkt->szRoleName);
		printf("ModifyCharacterDropPw result: Character %s is online now\n", pPkt->szRoleName);
		break;	
	case ENUM_ModifyCharacterDropPwResult_GMOOperationFailed:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"修改删除角色密码结果:游戏服务器操作失败\n");
		printf("ModifyCharacterDropPw result: Operation failed from GMObserver\n");
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

bool CModifyCharacterDropPw::SaveCharacterDropPw(char * szRoleName,char * szOldPasswd,char * szNewPasswd,char * lpGMServerName)//保存原来角色删除密码
{
	char szSql[256];
	
	bool flag=true;
	
	sprintf(szSql,"exec Pal_IsDropPw '%s'",szRoleName);
	vector<CGlobalStruct::DataRecord> datarecord;
	flag=m_SqlHelper.SqlHelperMain(&datarecord, "127.0.0.1", szSql, 4, 1, 1);//获取SqlExpress中的相应sql语句
	
	if(datarecord.empty())//如果记录为空
	{
		sprintf(szSql,"exec Pal_InsertDropPw '%s','%s','%s'",
				szRoleName,szOldPasswd,szNewPasswd);
		flag=m_SqlHelper.SqlHelperMain("127.0.0.1",  szSql, 4);//获取对应的LogDBIP
		return true;
	} 
	else
	{
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
		//delete datarecord;
		//datarecord=NULL;
		return false;
	}
	
	return true;

	
}
bool CModifyCharacterDropPw::FindLocalCharacterDropPw(char * szRoleName)
{
	char szSql[256];
	
	bool flag=true;
	
	sprintf(szSql,"exec Pal_IsDropPw '%s'",szRoleName);
	vector<CGlobalStruct::DataRecord> datarecord;
	flag=m_SqlHelper.SqlHelperMain(&datarecord, "127.0.0.1", szSql, 4, 1, 1);//获取SqlExpress中的相应sql语句
	
	if(datarecord.empty())//如果记录为空
	{
		return false;
	} 
	else
	{
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
	//	delete datarecord;
	//	datarecord=NULL;
		return true;
	}

	return true;
}

void CModifyCharacterDropPw::GetDropPw(char * szRoleName,char * szPasswd,char* lpGMServerName)
{
	try
	{
		char logdbip[20];
		char szSql[2048];
		char szRemoteSql[2048];
		ZeroMemory(logdbip,20);
		ZeroMemory(szSql,2048);
		ZeroMemory(szRemoteSql,2048);
		operPal.FindGameDBIP(logdbip,lpGMServerName);
		operPal.getRemoteSql("PAL","PAL_GETDROPPW",szRemoteSql,0);
		sprintf(szSql,szRemoteSql,szRoleName);
	//	vector<CGlobalStruct::DataRecord>* datarecord = new vector<CGlobalStruct::DataRecord>;
		vector<CGlobalStruct::DataRecord> datarecord;
		m_SqlHelper.SqlHelperMain("PAL",&datarecord, logdbip, szSql, 1, 1, 1);//获取SqlExpress中的相应sql语句
		
		if(datarecord.empty())//如果记录为空
		{
			sprintf(szPasswd,"");
			return;
		}
		
		//获取查询的记录集的每一项
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;	
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("droprolepassword", m_DataRecord.recordfieldname))//获取SqlExpress中对应sql_type的sql_statement
			{
				sprintf(szPasswd,m_DataRecord.recorddata);
			}
		}
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
//		delete datarecord;
//		datarecord=NULL;
		
	}
	catch(...)
	{
		sprintf(szPasswd,"");
	}



}


bool CModifyCharacterDropPw::RecallCharacterDropPw(char* szRoleName,char * szDropRolePw)
{
	bool flag=true;
	try
	{
		
		char szSql[256];
		sprintf(szSql,"exec Pal_FindOldDropPw '%s' ",szRoleName);
	//	vector<CGlobalStruct::DataRecord>* datarecord = new vector<CGlobalStruct::DataRecord>;
		vector<CGlobalStruct::DataRecord> datarecord;
		m_SqlHelper.SqlHelperMain(&datarecord, "127.0.0.1", szSql, 4, 1, 1);//获取SqlExpress中的相应sql语句
		if(datarecord.empty())//如果记录为空
		{
			sprintf(szDropRolePw,"");
			return false;
		}
		
		//获取查询的记录集的每一项
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;	
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("oldroledroppwd", m_DataRecord.recordfieldname))//获取SqlExpress中对应sql_type的sql_statement
			{
				sprintf(szDropRolePw,m_DataRecord.recorddata);
			}
		}
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
	//	delete datarecord;
	//	datarecord=NULL;
		sprintf(szSql,"exec Pal_UpdateDropPwFlag '%s'",szRoleName);
		
		flag=m_SqlHelper.SqlHelperMain("127.0.0.1", szSql, 4);//获取SqlExpress中的相应sql语句
		
	}
	catch(...)
	{
		return false;	
	}
	return flag;



}
