// Pal.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Pal.h"
//#include "PalManagerModule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CPalApp

BEGIN_MESSAGE_MAP(CPalApp, CWinApp)
	//{{AFX_MSG_MAP(CPalApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPalApp construction

CPalApp::CPalApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CPalApp object
extern "C"  bool __declspec(dllexport) Pal_Main(CSession * pSess,SOCKET m_socket,CEnumCore::Msg_Category m_category,CEnumCore::Message_Tag_ID m_message_tag,unsigned char * pbuf,int length)
{
		HANDLE m_Mutex;
		m_Mutex=CreateMutex(NULL, FALSE, "PAL_MAIN");//启用互斥量让PAL_MAIN独用接口通道
		WaitForSingleObject(m_Mutex,INFINITE);//锁线程

		CPalAPI PalAPI;
		COperVector * precvVect=PalAPI.initialize(pSess,m_message_tag,pbuf, length);//初始化类对象
		if(!precvVect)
		{
			return FALSE;
		}

		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_CHARACTERINFO)//查询角色基本信息
			{
				try
				{
					PalAPI.UserInfo(
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ACCOUNT,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_BANISHPLAYER)//踢人
			{
				try
				{
					PalAPI.Pal_KickPlayer(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata//用rolename代替账号
						);

				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_CHARACTERBAG)//查询玩家背包
			{
				try
				{
					PalAPI.Pal_CharacterBag(
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_BAGTYPE,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_CHARACTERFRIENDLIST)//取得角色好友列表
			{
				try
				{
					PalAPI.Pal_CharacterFriendList(
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_CHARACTERQUEST)//取得角色任务资讯
			{
				try
				{
					PalAPI.Pal_CharacterQuest(
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_QUESTTYPE,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_GUILDBASEINFO)//取得公会基本资讯
			{
				try
				{
					PalAPI.Pal_GuileBaseInfo(
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GUILDNAME,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_GUILDMEMBERLIST)//取得公会成员列表
			{
				try
				{
					PalAPI.Pal_GuildMemberList(
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GUILDNAME,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_SENDGIFTMAIL)//寄送奖品邮件
			{
				try
				{
					PalAPI.Pal_SendGiftMail(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_MAILSUBJECT,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_MAILCONTENT,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_MAILMONEY,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_MAILGIFTDBID0,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_MAILGIFTDBID1,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_MAILGIFTDBID2,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_MAILGIFTDBID3,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_MAILGIFTDBID4,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_MAILCONTENT,0)).lpdata);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_DISMISSGUILD)//解散公会
			{
				try
				{
					PalAPI.Pal_DismissGuild(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GUILDNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_REASON,0)).lpdata);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_MODIFYGUILDNAME)//修改公会名称
			{
				try
				{
					PalAPI.Pal_ModifyGuildName(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GUILDNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_NEWGUILDNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_REASON,0)).lpdata
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_MODIFYGUILDMEMBERPL)//修改公会成员职阶
			{
				try
				{
					PalAPI.Pal_ModifyGuildMemberPL(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GUILDNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GUILDPOWERLEVEL,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_REASON,0)).lpdata
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_MODIFYCHARACTERQUEST)//修改角色任务
			{
				try
				{
					PalAPI.Pal_ModifyCharacterQuest(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_QUESTID,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_QUESTTYPE,0)).lpdata)
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_MODIFYCHARACTERMONEY)//修改角色金钱
			{
				try
				{
					PalAPI.Pal_ModifyCharacterMoney(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_MONEYTYPE,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_MONEYCOUNT,0)).lpdata)
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_MODIFYCHARACTERDROPPW)//修改角色删除密码
			{
				try
				{
					PalAPI.Pal_ModifyCharacterDropPw(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_DROPROLEPW,0)).lpdata
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_RECALLCHARACTERDROPPW)//还原角色删除密码
			{
				try
				{
					PalAPI.Pal_RecallCharacterDropPw(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_MODIFYACCOUNTBANKPW)//修改帐号银行密码
			{
				try
				{
					PalAPI.Pal_ModifyAccountBankPw(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ACCOUNT,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_BANKPW,0)).lpdata
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_RECALLACCOUNTBANKPW)//还原帐号银行密码
			{
				try
				{
					PalAPI.Pal_RecallAccountBankPw(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ACCOUNT,0)).lpdata
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_MODIFYACCOUNTPW)//修改帐号密码
			{
				try
				{
					PalAPI.Pal_ModifyAccountPw(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ACCOUNT,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ACCOUNTPW,0)).lpdata
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_RECALLACCOUNTPW)//还原帐号密码
			{
				try
				{
					PalAPI.Pal_RecallAccountPw(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ACCOUNT,0)).lpdata
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_MODIFYSECONDPW)//修改二级密码
			{
				try
				{
					PalAPI.Pal_ModifySecondPw(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ACCOUNT,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_SECONDPW,0)).lpdata
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_RECALLSECONDPW)//还原二级密码
			{
				try
				{
					PalAPI.Pal_RecallSecondPw(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ACCOUNT,0)).lpdata);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_USESAFELOCK)//锁定安全锁
			{
				try
				{
					PalAPI.Pal_UseSafeLock(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ACCOUNT,0)).lpdata);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_FREESAFELOCK)//解锁密码锁
			{
				try
				{
					PalAPI.Pal_FreeSafeLock(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ACCOUNT,0)).lpdata);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_DELCHARACTERITEM)//删除角色物品响应
			{
				try
				{
					PalAPI.Pal_DelCharacterItem(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_BAGTYPE,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ITEMPOS,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ITEMNAME,0)).lpdata
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_MODIFYGUILDLV)//修改公会等级
			{
				try
				{
					PalAPI.Pal_ModifyGuildLv(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GUILDNAME,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GUILDLEVEL,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_REASON,0)).lpdata
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_MODIFYGUILDNOTICE)//修改公会公告
			{
				try
				{
					PalAPI.Pal_ModifyGuildNotice(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GUILDNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GUILDLEVEL,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_REASON,0)).lpdata);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_ACCOUNTLOCAL_QUERY)//本地封停的玩家帐号查询
			{
				try
				{
					PalAPI.CloseList(
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_ACCOUNT_CLOSE)//封停的玩家帐号(帐号找角色)
			{
				try
				{
					PalAPI.Pal_ClosePlayer(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ACCOUNT,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_REASON,0)).lpdata
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_ACCOUNT_OPEN)//解封的玩家帐号
			{
				try
				{
					PalAPI.Pal_OpenPlayer(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ACCOUNT,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_REASON,0)).lpdata
						);
				}
				catch (...)
				{
				return false;
				}
			}
		//maple update
		//批量封停玩家帐号
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_ACCOUNT_BANISHMENT_QUERY)//玩家封停帐号查询
			{
				try
				{
					PalAPI.CloseSingle(
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ACCOUNT,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
						);
					
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_BOARDTASK_INSERT)//公告添加
			{
				try
				{
					PalAPI.BroadTask_Insert(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,//服务器名字
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_WORLDID,0)).lpdata,//服务器worldID
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_BOARDMESSAGE,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_BEGINTIME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ENDTIME,0)).lpdata,
						StrToInt((LPCSTR)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_INTERVAL,0)).lpdata)
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_BOARDTASK_QUERY)//公告查询
			{
				try
				{
					//公告查询
					PalAPI.BroadTask_Query(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_BOARDTASK_UPDATE)//公告更新
			{
				try
				{
					PalAPI.BroadTask_Update(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						StrToInt((LPCSTR)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_TASKID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_BOARDMESSAGE,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_BEGINTIME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ENDTIME,0)).lpdata,
						StrToInt((LPCSTR)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_INTERVAL,0)).lpdata),
						StrToInt((LPCSTR)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_STATUS,0)).lpdata));
				}
				catch (...)
				{
					return false;
				}
			}
			if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_ITEMTYPE_QUERY)//道具类别查询
			{
				try
				{
					PalAPI.Pal_ItemType_Query();
				}
				catch (...)
				{
					return false;
				}
			}
			if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_ITEMNAME_QUERY)//道具名称查询
			{
				try
				{
					PalAPI.Pal_ItemName_Query(
						StrToInt((LPCSTR)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ITEMBIGTYPEID,0)).lpdata));
				}
				catch (...)
				{
					return false;
				}
			}
			if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_SendBankPwd_Query)//发送仓库密码
			{
				try
				{
					PalAPI.PAL_SendBankPwd_Query(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ACCOUNT,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_UserMailBox,0)).lpdata
						);
				}
				catch (...)
				{
					return false;
				}
			}
			if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_SendDeletePwd_Query)//发送角色删除密码
			{
				try
				{
					PalAPI.PAL_SendDeletePwd_Query(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_UserMailBox,0)).lpdata
						);
				}
				catch (...)
				{
					return false;
				}
			}
			if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_MAILBOX_QUERY)//邮箱查询
			{
				try
				{
					PalAPI.Pal_Get_EmailAddress((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ACCOUNT,0)).lpdata);
				}
				catch (...)
				{
					return false;
				}
			}
			if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_CHARACTERADVANCEINFO)//取得角色进阶资讯
			{
				try
				{
					PalAPI.Pal_CharacterAdvanceInfo(
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
						);
				}
				catch (...)
				{
					return false;
				}
			}
			if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_CHARACTERSKILL)//取得角色技能资讯
			{
				try
				{
					PalAPI.Pal_CharacterSkill(
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_CHARACTERMOB)//取得角色伏魔资讯
			{
				try
				{
					PalAPI.Pal_CharacterMob(
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_CHARACTERPUZZLEMAP)//取得角色图鉴资讯
			{
				try
				{
					PalAPI.Pal_CharacterPuzzleMap(
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_CHARACTERBLACKLIST)//取得角色黑名单列表
			{
				try
				{
					PalAPI.Pal_CharacterBlackList(
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_DELCHARACTERMOB)//删除角色伏魔
			{
				try
				{
					PalAPI.Pal_DelCharacterMob(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_MobID,0)).lpdata)
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_MODIFYPUZZLEMAP)//修改角色图鉴
			{
				try
				{
					PalAPI.Pal_ModifyPuzzleMap(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_PuzzleMap,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_PuzzleMapID1,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_PuzzleMapID2,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_PuzzleMapID3,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_PuzzleMapID4,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_PuzzleMapID5,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_PuzzleMapID6,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_PuzzleMapID7,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_PuzzleMapID8,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_PuzzleMapID9,0)).lpdata)
						);
				}
				catch (...)
				{
					return false;
				}
			}
	/*	if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_ACCOUNTINFO)//取得帐号资讯
			{
				try
				{
					PalAPI.Pal_AccountInfo(
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ACCOUNT,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
						);
				}
				catch (...)
				{
					return false;
				}
			}*/
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_MOVECHARACTERTOSAFE)//移动角色至安全点
			{
				try
				{
					PalAPI.Pal_MoveCharacterToSafe(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_MapID,0)).lpdata)
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_RELOADFUNCTIONSWITCH)//批量封停玩家帐号 maple update//重新载入功能开关列表
			{
				try
				{
					//PalAPI.Pal_ReloadFunctionSwitch();//maple update
					PalAPI.Pal_BanMultiPlayer(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ACCOUNT,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_REASON,0)).lpdata);
				}
				catch (...)
				{
					return false;
				}
			}
		// add by ltx
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_SENDCUSTOMITEMMAIL)//寄送客制物品邮件
		{
			try
			{
				PalAPI.Pal_SendCustomItemMail(
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_MAILSUBJECT,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_MAILCONTENT,0)).lpdata,
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_MAILMONEY,0)).lpdata),
					0,//ItemType
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_CustomIterID,0)).lpdata),
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_CustomIter,0)).lpdata,
					(bool)StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_BindSetup,0)).lpdata),
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_INHERITID0_NAME,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_MAILREASON,0)).lpdata);
			}
			catch (...)
			{
				return false;
			}
		}

		// add by ltx
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_MODIFYACCOUNTPWALL)
		{
			//Modified by tengjie 2010-07-08
// 			PalAPI.Pal_ModifyAccountPwAll(
// 				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
// 				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ACCOUNT,0)).lpdata,
// 				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ACCOUNTPW,0)).lpdata);
			PalAPI.Pal_ModifyAccountPwAll(
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ACCOUNT,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ACCOUNTPW,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata);
		}

		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_RECALLACCOUNTPWALL)
		{
			//Modified by tengjie 2010-07-08
// 			PalAPI.Pal_RecallAccountPwAll(
// 				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
// 				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ACCOUNT,0)).lpdata);
			PalAPI.Pal_RecallAccountPwAll(
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ACCOUNT,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata);
		}

		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_SearchMap_Query)//查询地图
			{
				try
				{
					PalAPI.PAL_SearchMap_Query();
				}
				catch (...)
				{
					return false;
				}
			}
		else
		{
			goto FALSE_FUNC;
		}
		if(!PalAPI.Destroy())
		{
			return FALSE;
		}
		ReleaseMutex(m_Mutex);//解锁
		CloseHandle(m_Mutex);
		return TRUE;
FALSE_FUNC:
		PalAPI.Destroy();
		ReleaseMutex(m_Mutex);//解锁
		CloseHandle(m_Mutex);
		return FALSE;
}

bool Pal_BanishPlayer(char * ServerIP,char * ServerName,char * pal_userNick)
{
	CPalAPI PalAPI;
	PalAPI.Pal_BanishPlayer(ServerIP,ServerName,pal_userNick);
	return TRUE;
}

CPalApp theApp;
