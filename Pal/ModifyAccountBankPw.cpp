// ModifyAccountBankPw.cpp: implementation of the CModifyAccountBankPw class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ModifyAccountBankPw.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CModifyAccountBankPw::CModifyAccountBankPw()
{

}

CModifyAccountBankPw::~CModifyAccountBankPw()
{

}
vector <CGlobalStruct::TFLV> CModifyAccountBankPw::ModifyAccountBankPwMain(char *szGMServerName, char * szGMServerIP,char* Account,char * szBankPw)
{

		/*	char strlog[80];
			sprintf(strlog,"大区<%s>修改帐户<%s>银行密码",szGMServerName,Account);
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
			g_ccNetObj.RegEvent_OnPacket(ENUM_PGPlayerControl_StoC_ModifyAccountBankPw, ModifyAccountBankPw, 0);
			
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
					//	int  iWorldID		= 0;		// 世界编号
					//	char szRoleName[32] = "Kitty";	// 角色名称
					//	char szBankPw[7]	= "654321";// 新银行密码(6位英数字)


						iWorldID=operPal.FindWorldID(szGMServerName,szGMServerIP);	

						
						PGPlayerControl_CtoS_ModifyAccountBankPw sPkt;
						sPkt.iWorldID	= iWorldID;
						sprintf(sPkt.szAccount, "%s", Account);
						sprintf(sPkt.szBankPw, "%s", szBankPw);
						
						// 传送至GMS要求修改角色银行密码
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

void CModifyAccountBankPw::LoginResult(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
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
// 修改帐号银行密码结果-------------------------------------------------------------------------------
void CModifyAccountBankPw::ModifyAccountBankPw(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
{

		PGPlayerControl_StoC_ModifyAccountBankPw *pPkt = (PGPlayerControl_StoC_ModifyAccountBankPw *)pData;
		char bufstr[256];
		ZeroMemory(bufstr,256);
		switch(pPkt->emResult)
		{
		case ENUM_ModifyAccountBankPwResult_Success:	
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"修改帐号银行密码成功,大区号:%d,帐号:%s\n",pPkt->iWorldID, pPkt->szAccount);
			printf("ModifyAccountBankPw succsee\n");
			printf("WorldID: %d\n"	, pPkt->iWorldID);
			printf("Account: %s\n"	, pPkt->szAccount);

			break;
		case ENUM_ModifyAccountBankPwResult_WorldNotFound:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"修改帐号银行密码结果: 大区没有找到\n");
			printf("ModifyAccountBankPw result: World not found\n");
			break;	
		case ENUM_ModifyAccountBankPwResult_LeaderNotFound:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"修改帐号银行密码结果: 频道没有找到\n");
			printf("ModifyAccountBankPw result: Leader not found\n");
			break;
		case ENUM_ModifyAccountBankPwResult_LeaderDisconnect:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"修改帐号银行密码结果: 频道没有连接\n");
			printf("ModifyAccountBankPw result: Leader disconnect\n");
			break;
		case ENUM_ModifyAccountBankPwResult_PasswordError:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"修改帐号银行密码结果: 密码格式错误\n");
			printf("ModifyCharacterDropPw result: Password format error\n");
			break;
		case ENUM_ModifyAccountBankPwResult_RoleNotFound:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"修改帐号银行密码结果: 帐号 %s 没有找到\n", pPkt->szAccount);
			printf("ModifyAccountBankPw result: Account %s not found\n", pPkt->szAccount);
			break;	
		case ENUM_ModifyAccountBankPwResult_RoleIsOnline:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"修改帐号银行密码结果: 帐号 %s 正在线\n", pPkt->szAccount);
			printf("ModifyAccountBankPw result: Account %s is online now\n", pPkt->szAccount);
			break;	
		case ENUM_ModifyAccountBankPwResult_GMOOperationFailed:
			ZeroMemory(bufstr,256);
			sprintf(bufstr,"修改帐号银行密码结果: 游戏服务器操作失败\n");
			printf("ModifyAccountBankPw result: Operation failed from GMObserver\n");
			break;	
		default:
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

bool CModifyAccountBankPw::SaveAccountBankPw(char * szRoleAccount,char* szOldPasswd,char * szNewPasswd,char * lpGMServerName)//保存原来帐号银行密码
{


	char szSql[256];

	bool flag=true;
	
	sprintf(szSql,"exec Pal_IsAccountBankPw '%s'",szRoleAccount);
	
	vector<CGlobalStruct::DataRecord> datarecord;
	if(!datarecord.empty())
	{
		datarecord.clear();
	}

	m_SqlHelper.SqlHelperMain(&datarecord, "127.0.0.1", szSql, 4, 1, 1);//获取SqlExpress中的相应sql语句
	
	if(datarecord.empty())//如果记录为空
	{
		sprintf(szSql,"exec Pal_InsertBankPw '%s','%s','%s'",szRoleAccount,szOldPasswd,szNewPasswd);
		m_SqlHelper.SqlHelperMain("127.0.0.1",  szSql, 4);//获取对应的LogDBIP
		return true;
	} 
	else
	{	
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
	//	delete datarecord;
	//	datarecord=NULL;
		return false;
	}
	
	
	return true;
}
bool CModifyAccountBankPw::FindLocalAccountBankPw(char * szAccount)
{
	
	char szSql[256];
	
	bool flag=true;
	
	sprintf(szSql,"exec Pal_IsAccountBankPw '%s'",szAccount);
	vector<CGlobalStruct::DataRecord> datarecord;
	if(!datarecord.empty())
	{
		datarecord.clear();
	}
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
		return true;
	}

		
	return true;
}

void CModifyAccountBankPw::GetBankPw(char * szAccount,char * szPasswd,char* lpGMServerName)
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
		operPal.getRemoteSql("PAL","PAL_GETBANKPW",szRemoteSql,0);
		sprintf(szSql,szRemoteSql,szAccount);
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
			if(!strcmp("bankpassword", m_DataRecord.recordfieldname))//获取SqlExpress中对应sql_type的sql_statement
			{
				sprintf(szPasswd,m_DataRecord.recorddata);
			}
		}
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
	//	delete datarecord;
	//	datarecord=NULL;	
	}
	catch(...)
	{
		sprintf(szPasswd,"");
	}
	

}


bool CModifyAccountBankPw::RecallAccountBankPw(char* szRoleAccount,char * szBankPw)
{

	bool flag=true;
	try
	{
		
		char szSql[256];
		sprintf(szSql,"exec Pal_FindOldBankPw '%s'",szRoleAccount);
	//	vector<CGlobalStruct::DataRecord>* datarecord = new vector<CGlobalStruct::DataRecord>;
		vector<CGlobalStruct::DataRecord> datarecord;
		m_SqlHelper.SqlHelperMain(&datarecord, "127.0.0.1", szSql, 4, 1, 1);//获取SqlExpress中的相应sql语句
		
		if(datarecord.empty())//如果记录为空
		{
			sprintf(szBankPw,"");
			return false;
		}
		
		//获取查询的记录集的每一项
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;	
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("oldbankpasswd", m_DataRecord.recordfieldname))//获取SqlExpress中对应sql_type的sql_statement
			{
				sprintf(szBankPw,m_DataRecord.recorddata);
			}
		}
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
		//delete datarecord;
	//	datarecord=NULL;
		sprintf(szSql,"exec Pal_UpdateBankPwFlag '%s'",szRoleAccount);
		
		flag=m_SqlHelper.SqlHelperMain("127.0.0.1", szSql, 4);//获取SqlExpress中的相应sql语句
		
	}
	catch(...)
	{
		
	}
	return flag;
}
