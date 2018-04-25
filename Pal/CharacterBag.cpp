// CharacterBag.cpp: implementation of the CCharacterBag class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "CharacterBag.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCharacterBag::CCharacterBag()
{

}

CCharacterBag::~CCharacterBag()
{

}
vector <CGlobalStruct::TFLV> CCharacterBag::CharacterBagMain(char *szGMServerName, char * szGMServerIP,char* szRoleName, int iBagType)
{

			// 载入DBF档以查询样版名称(指定"游戏目录\Kernel\\data")
			Pal_LoadData(".\\data");
		
			// 注册连线CallBack函式
			g_ccNetObj.RegEvent_OnConnected(COperPal::OnConnect, 0);
			g_ccNetObj.RegEvent_OnDisconnected(COperPal::OnDisconnect, 0);
			g_ccNetObj.RegEvent_OnConnectFailed(COperPal::OnConnectfailed, 0);
		
			// 注册对应事件CallBack函式
			g_ccNetObj.RegEvent_OnPacket(ENUM_PGGMCConnection_StoC_LoginResult,	LoginResult, 0);
			g_ccNetObj.RegEvent_OnPacket(ENUM_PGPlayerStatus_StoC_CharacterBag, CharacterBag, 0);
	
			// 初始网路设定
			g_ccNetObj.SetConnectCount(10);		// 设定重新连线次数为10
			g_ccNetObj.SetReConnect(false);		// 设定断线的时候不会重新连线
			g_ccNetObj.SetShowIP(true);			// 设定连线时会显示IP


			CGlobalStruct::TFLV m_tflv;
			if(!DBVect.empty())
			{
				DBVect.clear();
			}

			
			Sleep(100);
			g_state = 0;


			int n_login=0,n_send=0,login_num=0,send_num=0;
			operPal.GetLogSendNum(&login_num,&send_num);//从配置文件中获得登录次数和发送次数

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
						operPal.GetUserNamePwd("User1",szAccount,szPassword,&iGMServerPort);///从配置文件中获得登录接口的用户名和密码

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
						iWorldID=operPal.FindWorldID(szGMServerName,szGMServerIP);


						PGPlayerStatus_CtoS_CharacterBag sPkt;
						sPkt.iWorldID = iWorldID;
						sprintf(sPkt.szRoleName, "%s", szRoleName);
						sPkt.iBagType = iBagType;

						// 传送至GMS要求取得角色背包资讯
						g_ccNetObj.Send(sizeof(sPkt), (S_ObjNetPktBase*)&sPkt);	
						g_state = 3;			
					}
					break;
					// 等待取得角色背包资讯
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
					printf("等待取得角色背包资讯中,第%i次\n",n_send);
					break;
				}	
			}
			g_ccNetObj.Disconnect();
			printf("\n");
			printf("\n==================== Shutdown ====================\n");
			return DBVect;
		


}
void CCharacterBag::LoginResult(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
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

// 取得角色背包资讯结果-------------------------------------------------------------------------------
void CCharacterBag::CharacterBag(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
{
			PGPlayerStatus_StoC_CharacterBag *pPkt = (PGPlayerStatus_StoC_CharacterBag *)pData;
			CGlobalStruct::TFLV m_tflv;

			char strInt[10];
			char bufstr[256];
			switch(pPkt->emResult)
			{
			case ENUM_CharacterBagResult_Success:
				{
					// 若为结束封包
					if (pPkt->iWorldID == -1)
					{
						g_state = -1;
						return;
					}

					// 设定背包类型名称
					char szBagType[10];
					switch (pPkt->iBagType)
					{
					case 0:
						sprintf(szBagType, "身上");
						break;
					case 1:
						sprintf(szBagType, "宠物包");
						break;
					case 2:
						sprintf(szBagType, "PK不掉宝包");
						break;
					case 3:
						sprintf(szBagType, "银行");
						break;
					default:
						sprintf(szBagType, "背包");
					}

					time_t t = pPkt->sGMItemData.iCreateTime;
					struct tm *local = localtime(&t);

					char nowtime[40];
					sprintf(nowtime,"%d-%d-%d %d:%d:%d",local->tm_year+1900,local->tm_mon+1,local->tm_mday,local->tm_hour,local->tm_min,local->tm_sec);


					// 输出结果
					printf("\n====================================================\n");
					printf("World: %d\n",				pPkt->iWorldID);
					printf("Account: %s\n",				pPkt->szAccount);
					printf("RoleName: %s\n",			pPkt->szRoleName);
					printf("背包类型: %s\n",			szBagType);
					printf("\n[ 物件一般资料 ]\n");
					printf("名称: %s\n",				pPkt->sGMItemData.szName);
					printf("位置: %d\n",				pPkt->sGMItemData.iPos);
					printf("物件类型: %d\n",			pPkt->sGMItemData.iItemType);
					printf("样版ID: %d\n",				pPkt->sGMItemData.iOrgObjID);
					printf("建立时间: %s",				asctime(local));
					printf("序号: %d\n",				pPkt->sGMItemData.iSerial);
					printf("到期时间: %d\n",			pPkt->sGMItemData.iLiveTime);
					printf("下次可以使用的时间: %d\n",	pPkt->sGMItemData.iCoolDown/1000);

					if (pPkt->sGMItemData.bNPCTrade) printf("是否可以跟NPC交易: 可\n");
					else printf("是否可以跟NPC交易: 不可\n");
					if (pPkt->sGMItemData.bUserTrade) printf("是否可以跟玩家交易: 可\n");
					else printf("是否可以跟玩家交易: 不可\n");
					if (pPkt->sGMItemData.bItemMallGoods) printf("是否为商城物品: 是\n");
					else printf("是否为商城物品: 不是\n");	

					printf("数量: %d\n",				pPkt->sGMItemData.iCount);
					printf("卖价: %d\n",				pPkt->sGMItemData.iSellCost);
					printf("目前耐久度: %d\n",			pPkt->sGMItemData.wDurable);

					// 若类型为一般物品
					if (pPkt->sGMItemData.iItemType == 0)
					{
						printf("\n[ 玄石详细资料 ]\n");
						printf("基本值(字首): %d %s\n",	pPkt->sGMItemData.iInheritID[0], Pal_GetItemName(pPkt->sGMItemData.iInheritID[0]));
						printf("附魔: %d %s\n",			pPkt->sGMItemData.iInheritID[1], Pal_GetItemName(pPkt->sGMItemData.iInheritID[1]));
						printf("冲物理: %d %s\n",		pPkt->sGMItemData.iInheritID[2], Pal_GetItemName(pPkt->sGMItemData.iInheritID[2]));
						printf("冲法术: %d %s\n",		pPkt->sGMItemData.iInheritID[3], Pal_GetItemName(pPkt->sGMItemData.iInheritID[3]));
						printf("玄石1: %d %s\n",		pPkt->sGMItemData.iInheritID[4], Pal_GetItemName(pPkt->sGMItemData.iInheritID[4]));
						printf("玄石2: %d %s\n",		pPkt->sGMItemData.iInheritID[5], Pal_GetItemName(pPkt->sGMItemData.iInheritID[5]));
						printf("玄石3: %d %s\n",		pPkt->sGMItemData.iInheritID[6], Pal_GetItemName(pPkt->sGMItemData.iInheritID[6]));
						printf("玄石4: %d %s\n",		pPkt->sGMItemData.iInheritID[7], Pal_GetItemName(pPkt->sGMItemData.iInheritID[7]));
						printf("玄石5: %d %s\n",		pPkt->sGMItemData.iInheritID[8], Pal_GetItemName(pPkt->sGMItemData.iInheritID[8]));
						printf("玄石6: %d %s\n",		pPkt->sGMItemData.iInheritID[9], Pal_GetItemName(pPkt->sGMItemData.iInheritID[9]));
					}
					// 若类型为宠物
					else
						if (pPkt->sGMItemData.iItemType == 1)
						{
							printf("\n[ 宠物详细资料 ]\n");
							printf("经验值: %d\n",			pPkt->sGMItemData.sPet.iExp);
							printf("目前HP值: %d\n",		pPkt->sGMItemData.sPet.iNowHP);
							printf("宠物索引编号: %d (%s)\n", pPkt->sGMItemData.sPet.iPetIndex, Pal_GetPetName(pPkt->sGMItemData.sPet.iPetIndex));
							printf("还童次数: %d\n",		pPkt->sGMItemData.sPet.iRenewCount);
							printf("等级: %d\n",			pPkt->sGMItemData.sPet.iLV);
							printf("世代: %d\n",			pPkt->sGMItemData.sPet.iGeneration);
							printf("合成次数: %d\n",		pPkt->sGMItemData.sPet.iMixCount);
							printf("成长率: %d\n",			pPkt->sGMItemData.sPet.iGrowthRate);
							printf("品质: %d\n\n",			pPkt->sGMItemData.sPet.iAttrQuality);

							printf("HP加乘值: %d\n",		pPkt->sGMItemData.sPet.iHP);
							printf("武术: %d\n",			pPkt->sGMItemData.sPet.iWushu);
							printf("灵性: %d\n",			pPkt->sGMItemData.sPet.iSpirit);
							printf("防御: %d\n",			pPkt->sGMItemData.sPet.iConstitution);
							printf("灵御: %d\n",			pPkt->sGMItemData.sPet.iQuality);
							printf("身法: %d\n",			pPkt->sGMItemData.sPet.iDexterity);
							printf("吉运: %d\n",			pPkt->sGMItemData.sPet.iFate);
							printf("HP值慧根: %d\n",		pPkt->sGMItemData.sPet.iHP_Plus);
							printf("武术慧根: %d\n",		pPkt->sGMItemData.sPet.iWushu_Plus);
							printf("灵性慧根: %d\n",		pPkt->sGMItemData.sPet.iSpirit_Plus);
							printf("防御慧根: %d\n",		pPkt->sGMItemData.sPet.iConstitution_Plus);
							printf("灵御慧根: %d\n",		pPkt->sGMItemData.sPet.iQuality_Plus);
							printf("身法慧根: %d\n",		pPkt->sGMItemData.sPet.iDexterity_Plus);
							printf("吉运慧根: %d\n\n",		pPkt->sGMItemData.sPet.iFate_Plus);

							//Pal_GetPetSkillName(pPkt->sGMItemData.sPet.sSkill[i])

							printf("技能1: %d\n",			Pal_GetPetSkillName(pPkt->sGMItemData.sPet.sSkill[0]));
							printf("技能2: %d\n",			Pal_GetPetSkillName(pPkt->sGMItemData.sPet.sSkill[1]));
							printf("技能3: %d\n",			Pal_GetPetSkillName(pPkt->sGMItemData.sPet.sSkill[2]));
							printf("技能4: %d\n",			Pal_GetPetSkillName(pPkt->sGMItemData.sPet.sSkill[3]));
							printf("技能5: %d\n",			Pal_GetPetSkillName(pPkt->sGMItemData.sPet.sSkill[4]));
							printf("技能6: %d\n",			Pal_GetPetSkillName(pPkt->sGMItemData.sPet.sSkill[5]));
						}

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

					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_BAGTYPE;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=strlen(szBagType);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",szBagType);
					DBVect.push_back(m_tflv);

					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_ITEMNAME;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=strlen(pPkt->sGMItemData.szName);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",pPkt->sGMItemData.szName);
					DBVect.push_back(m_tflv);
					
					_itoa(pPkt->sGMItemData.iPos,strInt,10);
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_ITEMPOS;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);
					
					if (pPkt->sGMItemData.iItemType == 0)
					{
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_ITEMTYPE;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen("一般物品");
						sprintf((LPSTR)&m_tflv.lpdata,"%s","一般物品");
						DBVect.push_back(m_tflv);
					}
					else
					{
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_ITEMTYPE;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen("宠物");
						sprintf((LPSTR)&m_tflv.lpdata,"%s","宠物");
						DBVect.push_back(m_tflv);
					}

					_itoa(pPkt->sGMItemData.iOrgObjID,strInt,10);
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_ORGOBJID;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);
					

					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_LOCALTIME;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength=strlen(nowtime);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",nowtime);
					DBVect.push_back(m_tflv);

					_itoa(pPkt->sGMItemData.iSerial,strInt,10);
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_ITEMSERIAL;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);

					_itoa(pPkt->sGMItemData.iLiveTime,strInt,10);
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_LIVETIME;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);
				
					if (pPkt->sGMItemData.bNPCTrade) 
					{
						_itoa(1,strInt,10);
					}
					else
					{
						_itoa(0,strInt,10);
					}
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_NPCTRADE;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);

					if (pPkt->sGMItemData.bUserTrade) 
					{
						_itoa(1,strInt,10);
					}
					else
					{
						_itoa(0,strInt,10);
					}
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_USERTRADE;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);

					if (pPkt->sGMItemData.bItemMallGoods) 
					{
						_itoa(1,strInt,10);
					}
					else
					{
						_itoa(0,strInt,10);
					}
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_ITEMMALLGOODS;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);


					_itoa(pPkt->sGMItemData.iCount,strInt,10);
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_ITEMCOUNT;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);

					_itoa(pPkt->sGMItemData.wDurable,strInt,10);
					m_tflv.nIndex=DBVect.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_ITEMDURABLE;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBVect.push_back(m_tflv);

					if (pPkt->sGMItemData.iItemType == 0)
					{
						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%i%s",pPkt->sGMItemData.iInheritID[0], Pal_GetItemName(pPkt->sGMItemData.iInheritID[0]));
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_INHERITID0;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%i%s",pPkt->sGMItemData.iInheritID[1], Pal_GetItemName(pPkt->sGMItemData.iInheritID[1]));
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_INHERITID1;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);
						
						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%i%s",pPkt->sGMItemData.iInheritID[2], Pal_GetItemName(pPkt->sGMItemData.iInheritID[2]));
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_INHERITID2;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%i%s",pPkt->sGMItemData.iInheritID[3], Pal_GetItemName(pPkt->sGMItemData.iInheritID[3]));
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_INHERITID3;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%i%s",pPkt->sGMItemData.iInheritID[4], Pal_GetItemName(pPkt->sGMItemData.iInheritID[4]));
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_INHERITID4;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%i%s",pPkt->sGMItemData.iInheritID[5], Pal_GetItemName(pPkt->sGMItemData.iInheritID[5]));
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_INHERITID5;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);
				
						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%i%s",pPkt->sGMItemData.iInheritID[6], Pal_GetItemName(pPkt->sGMItemData.iInheritID[6]));
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_INHERITID6;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%i%s",pPkt->sGMItemData.iInheritID[7], Pal_GetItemName(pPkt->sGMItemData.iInheritID[7]));
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_INHERITID7;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%i%s",pPkt->sGMItemData.iInheritID[8], Pal_GetItemName(pPkt->sGMItemData.iInheritID[8]));
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_INHERITID8;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%i%s",pPkt->sGMItemData.iInheritID[9], Pal_GetItemName(pPkt->sGMItemData.iInheritID[9]));
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_INHERITID9;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						/////////没有的话字符串-1///////////////////
						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETEXP;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETNOWHP;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETINDEX;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETRENEWCOUNT;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETLV;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETGENERATION;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETMIXCOUNT;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETGROWTHRATE;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETATTRQUALITY;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETHP;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETWUSHU;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETSPIRIT;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETCONSTITUTION;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETQUALITY;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETDEXTERITY;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETFATE;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETHPPLUS;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETWUSHUPLUS;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETSPIRITPLUS;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETCONSTITUTIONPLUS;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETQUALITYPLUS;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETDEXTERITYPLUS;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETFATEPLUS;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETSKILL0;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETSKILL1;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETSKILL2;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETSKILL3;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETSKILL4;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETSKILL5;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);
					}
					else if (pPkt->sGMItemData.iItemType == 1)
					{
						/////////没有的话字符串-1///////////////////
						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_INHERITID0;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_INHERITID1;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_INHERITID2;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_INHERITID3;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_INHERITID4;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_INHERITID5;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_INHERITID6;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_INHERITID7;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_INHERITID8;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%s","-1");
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_INHERITID9;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						//////////////////////////////////////////////
						_itoa(pPkt->sGMItemData.sPet.iExp,strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETEXP;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						DBVect.push_back(m_tflv);

						_itoa(pPkt->sGMItemData.sPet.iNowHP,strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETNOWHP;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						DBVect.push_back(m_tflv);

						ZeroMemory(bufstr,256);
						sprintf(bufstr,"%i(%s)",pPkt->sGMItemData.sPet.iPetIndex, Pal_GetItemName(pPkt->sGMItemData.sPet.iPetIndex));
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETINDEX;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(bufstr);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
						DBVect.push_back(m_tflv);

						_itoa(pPkt->sGMItemData.sPet.iRenewCount,strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETRENEWCOUNT;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						DBVect.push_back(m_tflv);

						_itoa(pPkt->sGMItemData.sPet.iLV,strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETLV;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						DBVect.push_back(m_tflv);

						_itoa(pPkt->sGMItemData.sPet.iGeneration,strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETGENERATION;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						DBVect.push_back(m_tflv);

						_itoa(pPkt->sGMItemData.sPet.iMixCount,strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETMIXCOUNT;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						DBVect.push_back(m_tflv);

						_itoa(pPkt->sGMItemData.sPet.iGrowthRate,strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETGROWTHRATE;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						DBVect.push_back(m_tflv);

						_itoa(pPkt->sGMItemData.sPet.iAttrQuality,strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETATTRQUALITY;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						DBVect.push_back(m_tflv);

						_itoa(pPkt->sGMItemData.sPet.iHP,strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETHP;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						DBVect.push_back(m_tflv);

						_itoa(pPkt->sGMItemData.sPet.iWushu,strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETWUSHU;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						DBVect.push_back(m_tflv);

						_itoa(pPkt->sGMItemData.sPet.iSpirit,strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETSPIRIT;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						DBVect.push_back(m_tflv);

						_itoa(pPkt->sGMItemData.sPet.iConstitution,strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETCONSTITUTION;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						DBVect.push_back(m_tflv);

						_itoa(pPkt->sGMItemData.sPet.iQuality,strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETQUALITY;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						DBVect.push_back(m_tflv);

						_itoa(pPkt->sGMItemData.sPet.iDexterity,strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETDEXTERITY;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						DBVect.push_back(m_tflv);

						_itoa(pPkt->sGMItemData.sPet.iFate,strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETFATE;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						DBVect.push_back(m_tflv);

						_itoa(pPkt->sGMItemData.sPet.iHP_Plus,strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETHPPLUS;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						DBVect.push_back(m_tflv);

						_itoa(pPkt->sGMItemData.sPet.iWushu_Plus,strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETWUSHUPLUS;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						DBVect.push_back(m_tflv);

						_itoa(pPkt->sGMItemData.sPet.iSpirit_Plus,strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETSPIRITPLUS;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						DBVect.push_back(m_tflv);

						_itoa(pPkt->sGMItemData.sPet.iConstitution_Plus,strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETCONSTITUTIONPLUS;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						DBVect.push_back(m_tflv);

						_itoa(pPkt->sGMItemData.sPet.iQuality_Plus,strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETQUALITYPLUS;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						DBVect.push_back(m_tflv);

						_itoa(pPkt->sGMItemData.sPet.iDexterity_Plus,strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETDEXTERITYPLUS;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						DBVect.push_back(m_tflv);

						_itoa(pPkt->sGMItemData.sPet.iFate_Plus,strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETFATEPLUS;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						DBVect.push_back(m_tflv);

						//_itoa(pPkt->sGMItemData.sPet.sSkill[0],strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETSKILL0;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",Pal_GetPetSkillName(pPkt->sGMItemData.sPet.sSkill[0]));//strInt);
						DBVect.push_back(m_tflv);

						//_itoa(pPkt->sGMItemData.sPet.sSkill[1],strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETSKILL1;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",Pal_GetPetSkillName(pPkt->sGMItemData.sPet.sSkill[1]));//strInt);
						DBVect.push_back(m_tflv);

						//_itoa(pPkt->sGMItemData.sPet.sSkill[2],strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETSKILL2;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",Pal_GetPetSkillName(pPkt->sGMItemData.sPet.sSkill[2]));//strInt);
						DBVect.push_back(m_tflv);

						//_itoa(pPkt->sGMItemData.sPet.sSkill[3],strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETSKILL3;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",Pal_GetPetSkillName(pPkt->sGMItemData.sPet.sSkill[3]));
						DBVect.push_back(m_tflv);

						//_itoa(pPkt->sGMItemData.sPet.sSkill[4],strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETSKILL4;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",Pal_GetPetSkillName(pPkt->sGMItemData.sPet.sSkill[4]));
						DBVect.push_back(m_tflv);

						//_itoa(pPkt->sGMItemData.sPet.sSkill[5],strInt,10);
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PAL_PETSKILL5;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",Pal_GetPetSkillName(pPkt->sGMItemData.sPet.sSkill[5]));
						DBVect.push_back(m_tflv);
					}
				}
				break;

			case ENUM_CharacterBagResult_TypeError:
				ZeroMemory(bufstr,256);
				sprintf(bufstr,"%s","角色背包结果:错误背包类型\n");
				printf("角色背包结果:错误背包类型\n");
				m_tflv.nIndex=DBVect.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(bufstr);
				sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
				DBVect.push_back(m_tflv);
				g_state = -1;
				break;

			case ENUM_CharacterBagResult_RoleNotFound:
				
				ZeroMemory(bufstr,256);
				sprintf(bufstr,"角色背包结果: 角色 %s 没有找到\n", pPkt->szRoleName);
				printf("角色背包结果: 角色 %s 没有找到\n", pPkt->szRoleName);	
				m_tflv.nIndex=DBVect.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(bufstr);
				sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
				DBVect.push_back(m_tflv);
				g_state = -1;
				break;

			case ENUM_CharacterBagResult_CharacterBagEmpty:
				ZeroMemory(bufstr,256);
				sprintf(bufstr,"角色背包结果: %s的背包是空的\n", pPkt->szRoleName);
				printf("角色背包结果: %s的背包是空的\n", pPkt->szRoleName);
				m_tflv.nIndex=DBVect.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(bufstr);
				sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
				DBVect.push_back(m_tflv);
				g_state = -1;
				break;

			case ENUM_CharacterBagResult_WorldNotFound:
				ZeroMemory(bufstr,256);
				sprintf(bufstr,"角色背包结果:大区没有找到\n");
				printf("角色背包结果:大区没有找到\n");
				m_tflv.nIndex=DBVect.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(bufstr);
				sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
				DBVect.push_back(m_tflv);
				g_state = -1;
				break;

			case ENUM_CharacterBagResult_LeaderNotFound:
				ZeroMemory(bufstr,256);
				sprintf(bufstr,"角色背包结果: 频道没有找到\n");
				printf("角色背包结果: 频道没有找到\n");
				m_tflv.nIndex=DBVect.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(bufstr);
				sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
				DBVect.push_back(m_tflv);
				g_state = -1;
				break;

			case ENUM_CharacterBagResult_LeaderDisconnect:
				ZeroMemory(bufstr,256);
				sprintf(bufstr,"角色背包结果: 频道没有连接\n");
				printf("角色背包结果: 频道没有连接\n");	
				m_tflv.nIndex=DBVect.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen(bufstr);
				sprintf((LPSTR)&m_tflv.lpdata,"%s",bufstr);
				DBVect.push_back(m_tflv);
				g_state = -1;
				break;

			default:
				g_state = -1;
				break;
			}

}
