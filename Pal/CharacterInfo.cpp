// CharacterInfo.cpp: implementation of the CCharacterInfo class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "CharacterInfo.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCharacterInfo::CCharacterInfo()
{

}

CCharacterInfo::~CCharacterInfo()
{

}
vector <CGlobalStruct::TFLV> CCharacterInfo::CharacterInfoMain(char *szGMServerName, char * szGMServerIP,char* szRoleName)
{
	CGlobalStruct::TFLV m_tflv;
	char strlog[80];
	sprintf(strlog,"大区<%s>角色<%s>查询角色基本信息",szGMServerName,szRoleName);
	logFile.WriteText("仙剑OL",strlog);

	///new adding //////////////////
	// 载入DBF档以查询样版名称(指定"游戏目录\Kernel\\data")
	Pal_LoadData(".\\data");

	// 注册连线CallBack函式
	g_ccNetObj.RegEvent_OnConnected(COperPal::OnConnect, 0);
	g_ccNetObj.RegEvent_OnDisconnected(COperPal::OnDisconnect, 0);
	g_ccNetObj.RegEvent_OnConnectFailed(COperPal::OnConnectfailed, 0);

	// 注册对应事件CallBack函式
	g_ccNetObj.RegEvent_OnPacket(ENUM_PGGMCConnection_StoC_LoginResult,	LoginResult, 0);
	g_ccNetObj.RegEvent_OnPacket(ENUM_PGPlayerStatus_StoC_CharacterInfo, CharacterInfo, 0);

	// 初始网路设定
	g_ccNetObj.SetConnectCount(10);		// 设定重新连线次数为10
	g_ccNetObj.SetReConnect(false);		// 设定断线的时候不会重新连线
	g_ccNetObj.SetShowIP(true);			// 设定连线时会显示IP

	Sleep(100);
	g_state = 0;

	if(!DBVect.empty())
	{
		DBVect.clear();
	}

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
				//int  iWorldID = 0;							// 世界编号
				//char szRoleName[32] = "专业程式设计师";		// 角色名称
				iWorldID=operPal.FindWorldID(szGMServerName,szGMServerIP);
				PGPlayerStatus_CtoS_CharacterInfo sPkt;
				sPkt.iWorldID = iWorldID;
				sprintf(sPkt.szRoleName, "%s", szRoleName);

				// 传送至GMS要求取得角色基本资讯
				g_ccNetObj.Send(sizeof(sPkt), (S_ObjNetPktBase*)&sPkt);	
				g_state = 3;			
			}
			break;
			// 等待取得角色基本资讯
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
			printf("等待取得角色基本资讯中,第%i次\n",n_send);
			break;
		}
	}			
	g_ccNetObj.Disconnect();
	printf("\n");
	printf("\n==================== Shutdown ====================\n");
	return DBVect;
}


void CCharacterInfo::LoginResult(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
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

// 取得角色基本资讯结果-------------------------------------------------------------------------------
void CCharacterInfo::CharacterInfo(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
{
	PGPlayerStatus_StoC_CharacterInfo *pPkt = (PGPlayerStatus_StoC_CharacterInfo *)pData;
	char strInt[10];
	char bufstr[256];
	CGlobalStruct::TFLV m_tflv;
	switch(pPkt->emResult)
	{
	case ENUM_CharacterInfoResult_Success:
		{
			char szTemp[32];
			int  dwReslut[1024];		// 储存称号具体资讯
			
			// 取得称号具体资讯
			Pal_GetMobFlagResult(dwReslut, pPkt->sGMRoleInfo.dwTitleFlag);

			// 设定职业与性别名称
			char szVoc[32];
			char szSex[32];
			switch (pPkt->sGMRoleInfo.emVoc)
			{
			case ENUM_Vocation_1:
				sprintf(szVoc, "剑侠");
				break;
			case ENUM_Vocation_2:
				sprintf(szVoc, "灵蛇刀客");
				break;
			case ENUM_Vocation_3:
				sprintf(szVoc, "蝶衣舞者");
				break;
			case ENUM_Vocation_4:
				sprintf(szVoc, "五灵师");
				break;
			case ENUM_Vocation_5:
				sprintf(szVoc, "灵狐夜隐");
				break;
			case ENUM_Vocation_6:
				sprintf(szVoc, "练气士");
				break;
			default:
				sprintf(szVoc, "");
				break;
			}	
			if (pPkt->sGMRoleInfo.iSex) sprintf(szSex, "男");
			else sprintf(szSex, "女");
			
			time_t t = pPkt->iLoginTime;
			struct tm *local = localtime(&t);
			char nowtime[40];
			sprintf(nowtime,"%d-%d-%d %d:%d:%d",local->tm_year+1900,local->tm_mon+1,local->tm_mday,local->tm_hour,local->tm_min,local->tm_sec);

			// 输出结果
			printf("\n====================================================\n");
			printf("World: %d\n",					pPkt->iWorldID);
			printf("Account: %s\n",					pPkt->szAccount);
			printf("RoleName: %s\n",				pPkt->szRoleName);
			printf("DBID: %d\n",					pPkt->iDBID);
			printf("ZoneID: %d\n",					pPkt->iZoneID);

			// 若为空字串(角色不在线上)则查询区域名称
			if (strlen(pPkt->szZoneName) == 0)
				printf("ZoneName: %s\n",			Pal_GetZoneName(pPkt->iZoneID));
			else
				printf("ZoneName: %s\n",			pPkt->szZoneName);
			
			printf("LoginTime: %s",					asctime(local));
			printf("IP: %s\n",						pPkt->szIP);
			
			printf("\n[ 角色基本资讯 ]\n");
			printf("座标X: %d\n",					pPkt->sGMRoleInfo.X);
			printf("座标Y: %d\n",					pPkt->sGMRoleInfo.Y);
			printf("座标Z: %d\n",					pPkt->sGMRoleInfo.Z);
			printf("昵称: %s\n",					pPkt->sGMRoleInfo.szNickName);
			printf("公会名称: %s\n",				pPkt->sGMRoleInfo.szGuildName);
			printf("公会职级: %d\n",				pPkt->sGMRoleInfo.iGuildPowerLevel);
			printf("职业: %s\n",					szVoc);
			printf("等级: %d\n",					pPkt->sGMRoleInfo.iLevel);
			printf("目前经验: %d\n",				pPkt->sGMRoleInfo.iExp);
			printf("身上金钱: %d\n",				pPkt->sGMRoleInfo.iBodyMoney);
			printf("仓库金钱: %d\n\n",				pPkt->sGMRoleInfo.iBankMoney);
			
			printf("性别: %s\n",					szSex);
			printf("名声: %d\n",					pPkt->sGMRoleInfo.iFame);
			printf("目前血量: %d\n",				pPkt->sGMRoleInfo.iHP);
			printf("目前法力: %d\n",				pPkt->sGMRoleInfo.iMP);
			printf("目前真气: %d\n",				pPkt->sGMRoleInfo.iAP);
			printf("目前精力: %d\n",				pPkt->sGMRoleInfo.iEP);
			printf("可配置技能点数: %d\n",			pPkt->sGMRoleInfo.iSkillPoint);
			printf("目前技能树走到的位置: %d\n",	pPkt->sGMRoleInfo.iSkillTreeNode);
			printf("转生次数: %d\n",				pPkt->sGMRoleInfo.iReincarnation);
			
			if (pPkt->sGMRoleInfo.iTitleID != -1)
			{
				sprintf(szTemp, Pal_GetTitleName(pPkt->sGMRoleInfo.iTitleID));
				// 若查不到资料
				if (strlen(szTemp) == 0) sprintf(szTemp, "Unknow Name");
			}
			else
			{
				sprintf(szTemp, "不使用称号");
			}
			printf("目前使用称号: %s\n", szTemp);
			printf("已取得称号列表: \n");
			printf("===== Start ===== \n");
			for (int i=0; i<1024; i++)
			{
				// 若称号旗标开启
				if (dwReslut[i] == 1)
				{
					sprintf(szTemp, Pal_GetTitleName(i));	// 取得称号名称
					// 若查不到资料
					if (strlen(szTemp) == 0) sprintf(szTemp, "Unknow Name");
					printf("  %s\n", szTemp);
				}// if
			}// for
			printf("====== End ====== \n\n");
			printf("邪气值: %d\n",	pPkt->sGMRoleInfo.iMurdererCount);
			printf("恶业值: %d\n",	pPkt->sGMRoleInfo.iEvilCount);
			printf("武术: %d\n",	pPkt->sGMRoleInfo.iWushu);
			printf("体质: %d\n",	pPkt->sGMRoleInfo.iConstitution);
			printf("灵性: %d\n",	pPkt->sGMRoleInfo.iSpirit);
			printf("灵耐: %d\n",	pPkt->sGMRoleInfo.iQuality);
			printf("身法: %d\n",	pPkt->sGMRoleInfo.iDexterity);
			printf("吉运: %d\n",	pPkt->sGMRoleInfo.iFate);
		


			_itoa(pPkt->iWorldID,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_WORLDID;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);


			
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_ACCOUNT;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(pPkt->szAccount);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",pPkt->szAccount);
			DBVect.push_back(m_tflv);

			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_ROLENAME;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(pPkt->szRoleName);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",pPkt->szRoleName);
			DBVect.push_back(m_tflv);

			
			_itoa(pPkt->iDBID,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_ROLEID;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);



			_itoa(pPkt->iZoneID,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_ZONEID;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);



			if (strlen(pPkt->szZoneName) == 0)
			{
				m_tflv.nIndex=DBVect.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::PAL_ZONENAME;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(Pal_GetZoneName(pPkt->iZoneID));
				sprintf((LPSTR)&m_tflv.lpdata,"%s",Pal_GetZoneName(pPkt->iZoneID));
				DBVect.push_back(m_tflv);
			}
			else
			{
				m_tflv.nIndex=DBVect.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::PAL_ZONENAME;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(pPkt->szZoneName);
				sprintf((LPSTR)&m_tflv.lpdata,"%s",pPkt->szZoneName);
				DBVect.push_back(m_tflv);
			}




			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_LOGINTIME;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(nowtime);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",nowtime);
			DBVect.push_back(m_tflv);

			
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_LOGINIP;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(pPkt->szIP);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",pPkt->szIP);
			DBVect.push_back(m_tflv);

			
			_itoa(pPkt->sGMRoleInfo.X,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_X;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);

			_itoa(pPkt->sGMRoleInfo.Y,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_Y;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);

			_itoa(pPkt->sGMRoleInfo.Z,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_Z;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);


			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_NICKNAME;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(pPkt->sGMRoleInfo.szNickName);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",pPkt->sGMRoleInfo.szNickName);
			DBVect.push_back(m_tflv);

			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_GUILDNAME;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(pPkt->sGMRoleInfo.szGuildName);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",pPkt->sGMRoleInfo.szGuildName);
			DBVect.push_back(m_tflv);

			
			_itoa(pPkt->sGMRoleInfo.iGuildPowerLevel,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_GUILDPOWERLEVEL;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);


			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_VOC;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(szVoc);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",szVoc);
			DBVect.push_back(m_tflv);

			_itoa(pPkt->sGMRoleInfo.iLevel,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_LEVEL;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);

			_itoa(pPkt->sGMRoleInfo.iExp,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_EXP;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);

			_itoa(pPkt->sGMRoleInfo.iBodyMoney,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_BODYMONEY;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);

			_itoa(pPkt->sGMRoleInfo.iBankMoney,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_BANKMONEY;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);

			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_SEX;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen(szSex);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",szSex);
			DBVect.push_back(m_tflv);

			_itoa(pPkt->sGMRoleInfo.iFame,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_FAME;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);
			
			_itoa(pPkt->sGMRoleInfo.iHP,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_HP;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);

			_itoa(pPkt->sGMRoleInfo.iMP,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_MP;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);

			_itoa(pPkt->sGMRoleInfo.iAP,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_AP;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);

			_itoa(pPkt->sGMRoleInfo.iEP,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_EP;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);

			_itoa(pPkt->sGMRoleInfo.iSkillPoint,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_SKILLPOINT;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);

			_itoa(pPkt->sGMRoleInfo.iSkillTreeNode,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_SKILLTREENODE;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);

			_itoa(pPkt->sGMRoleInfo.iReincarnation,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_REINCARNATION;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);
			
			_itoa(pPkt->sGMRoleInfo.iMurdererCount,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_MurdererCount;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);
			
			_itoa(pPkt->sGMRoleInfo.iEvilCount,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_EvilCount;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);
			
			_itoa(pPkt->sGMRoleInfo.iWushu,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_Wushu;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);
			
			_itoa(pPkt->sGMRoleInfo.iConstitution,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_Constitution;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);
			
			_itoa(pPkt->sGMRoleInfo.iSpirit,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_Spirit;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);
			
			_itoa(pPkt->sGMRoleInfo.iQuality,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_Quality;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);
			
			_itoa(pPkt->sGMRoleInfo.iDexterity,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_Dexterity;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);
			
			_itoa(pPkt->sGMRoleInfo.iFate,strInt,10);
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::PAL_Fate;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=sizeof(int);
			sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
			DBVect.push_back(m_tflv);
		}
		break;

	case ENUM_CharacterInfoResult_RoleNotFound:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"角色信息结果: 角色 %s 没有找到\n", pPkt->szRoleName);
		printf("Charinfo result: Character %s not found\n", pPkt->szRoleName);
		m_tflv.nIndex=DBVect.size()+1;
		m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
		m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
		m_tflv.m_tvlength=strlen(bufstr);
		sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
		DBVect.push_back(m_tflv);
		break;

	case ENUM_CharacterInfoResult_WorldNotFound:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"角色信息结果: 大区没有找到\n");
		printf("Charinfo result: World not found\n");
		m_tflv.nIndex=DBVect.size()+1;
		m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
		m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
		m_tflv.m_tvlength=strlen(bufstr);
		sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
		DBVect.push_back(m_tflv);
		break;

	case ENUM_CharacterInfoResult_LeaderNotFound:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"角色信息结果: 频道没有找到\n");
		printf("Charinfo result: Leader not found\n");
		m_tflv.nIndex=DBVect.size()+1;
		m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
		m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
		m_tflv.m_tvlength=strlen(bufstr);
		sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
		DBVect.push_back(m_tflv);
		break;

	case ENUM_CharacterInfoResult_LeaderDisconnect:
		ZeroMemory(bufstr,256);
		sprintf(bufstr,"角色信息结果: 频道没有连接\n");
		printf("Charinfo result: Leader disconnect\n");
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
	g_state = -1;
}	
