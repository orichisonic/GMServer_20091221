// JW2.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "JW2.h"
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
// CJW2App

BEGIN_MESSAGE_MAP(CJW2App, CWinApp)
	//{{AFX_MSG_MAP(CJW2App)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJW2App construction

CJW2App::CJW2App()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CJW2App object


/////////////////////////////////////////////////////////////////////////////
// CJW2App initialization

BOOL CJW2App::InitInstance()
{
	return TRUE;
}
extern "C"  bool __declspec(dllexport) JW2_Main(CSession * pSess,SOCKET m_socket,CEnumCore::Msg_Category m_category,CEnumCore::Message_Tag_ID m_message_tag,unsigned char * pbuf,int length)
{
	try
	{
		CJW2App theApp;
		CJW2Info JW2Info;
		COperatorJW2 operJW2;
		vector <CGlobalStruct::TFLV> DBTFLV;
		CEnumCore::ServiceKey m_ServiceKey= (m_message_tag & 0x00FFFF) | 0x8000;//获取要回应的CEnumCore::ServiceKey
		COperVector * precvVect=JW2Info.initialize(pSess,m_message_tag, pbuf, length);
		if(!precvVect)//初始化类对象
		{
			return false;
		}
		switch((unsigned int)m_message_tag)
		{
		//玩家帐号信息查询（1.2.3.4.5.8）	
		case CEnumCore::Message_Tag_ID::JW2_ACCOUNT_QUERY:
			{
				DBTFLV=theApp.UserInfo(precvVect);
			}
			break;
		//只用到一个关键字
		case CEnumCore::Message_Tag_ID::JW2_RPG_QUERY:
		case CEnumCore::Message_Tag_ID::JW2_ITEMSHOP_BYOWNER_QUERY:
		case CEnumCore::Message_Tag_ID::JW2_HOME_ITEM_QUERY:
		case CEnumCore::Message_Tag_ID::JW2_WASTE_ITEM_QUERY:
		case CEnumCore::Message_Tag_ID::JW2_SMALL_BUGLE_QUERY:
		case CEnumCore::Message_Tag_ID::JW2_DUMMONEY_QUERY:
		case CEnumCore::Message_Tag_ID::JW2_WEDDINGINFO_QUERY:
		case CEnumCore::Message_Tag_ID::JW2_WEDDINGLOG_QUERY:
		case CEnumCore::Message_Tag_ID::JW2_COUPLEINFO_QUERY:
		case CEnumCore::Message_Tag_ID::JW2_COUPLELOG_QUERY:
		case CEnumCore::Message_Tag_ID::JW2_WEDDINGGROUND_QUERY:
		case CEnumCore::Message_Tag_ID::JW2_User_Family_Query:
		case CEnumCore::Message_Tag_ID::JW2_CoupleParty_Card:
		case CEnumCore::Message_Tag_ID::JW2_MailInfo_Query:
		case CEnumCore::Message_Tag_ID::JW2_FriendList_Query:
		case CEnumCore::Message_Tag_ID::JW2_Messenger_Query:
		case CEnumCore::Message_Tag_ID::JW2_Wedding_Paper:
			{
				DBTFLV=theApp.UserDefault(precvVect,m_message_tag);
			}
			break;
		//家族信息
		case CEnumCore::Message_Tag_ID::JW2_FAMILYINFO_QUERY:
			DBTFLV=JW2Info.JW2_FAMILYINFO_QUERY(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_FAMILYNAME,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
				);
			break;
		case CEnumCore::Message_Tag_ID::JW2_BasicInfo_Query:
			DBTFLV=JW2Info.JW2_BasicInfo_Query(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_FAMILYNAME,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
				);
			break;
		//家族成员信息
		case CEnumCore::Message_Tag_ID::JW2_FAMILYMEMBER_QUERY:
		case CEnumCore::Message_Tag_ID::JW2_FamilyItemInfo_Query:
		case CEnumCore::Message_Tag_ID::JW2_FamilyMember_Applying:
		case CEnumCore::Message_Tag_ID::JW2_PetInfo_Query:
		case CEnumCore::Message_Tag_ID::JW2_BasicRank_Query:
			DBTFLV=JW2Info.JW2_Family_Detail(
				m_message_tag,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_FAMILYNAME,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_FAMILYID,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
				);
			break;
		//查看购物，送礼（7－10）
		case CEnumCore::Message_Tag_ID::JW2_SMALL_PRESENT_QUERY:
			DBTFLV=JW2Info.JW2_SMALL_PRESENT_QUERY(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ACCOUNT,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_UserSN,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_GOODSTYPE,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;
		//强制玩家下线
		case CEnumCore::Message_Tag_ID::JW2_BANISHPLAYER:
			DBTFLV=JW2Info.KickPlayer(
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ACCOUNT,0)).lpdata
				);
			break;
		//解封帐号logindb
		case CEnumCore::Message_Tag_ID::JW2_ACCOUNT_OPEN:
			DBTFLV=JW2Info.MakeOpen(
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ACCOUNT,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_UserNick,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_UserSN,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_Reason,0)).lpdata);
			break;
		//封停帐号logindb
		case CEnumCore::Message_Tag_ID::JW2_ACCOUNT_CLOSE:
			DBTFLV=JW2Info.MakeClose(
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ACCOUNT,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_UserNick,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_UserSN,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_Reason,0)).lpdata);
			break;
		//玩家封停帐号信息查询
		case CEnumCore::Message_Tag_ID::JW2_ACCOUNT_BANISHMENT_QUERY:
			DBTFLV=JW2Info.CloseSingle("JW2",
				"select distinct BanPlayerAccount as jw2_usersn,BanPlayerUserNick as jw2_usernick,BanPlayerUserId as jw2_userid,city as jw2_serverip,BanReason as jw2_reason from JW2_AutoUnBanUser a, gmtools_serverinfo b where a.serverip=b.serverip and (b.city='%s' and BanPlayerUserId='%s')",
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ACCOUNT,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;
		//游戏服务器封停的玩家帐号查询
		case CEnumCore::Message_Tag_ID::JW2_ACCOUNTREMOTE_QUERY:
			DBTFLV=JW2Info.CloseList("JW2","exec JW2_ACCOUNTREMOTE_QUERY '%s'",
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerIP,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;
		//公告更新
		case CEnumCore::Message_Tag_ID::JW2_BOARDTASK_UPDATE:
			DBTFLV=JW2Info.BroadTask_Update("JW2","exec JW2_BOARDTASK_UPDATE %i,%i,'%s','%s','%s',%i,%i",
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				StrToInt((LPCSTR)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_TaskID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_BoardMessage,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_BeginTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_EndTime,0)).lpdata,
				StrToInt((LPCSTR)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_Interval,0)).lpdata),
				StrToInt((LPCSTR)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_Status,0)).lpdata));
			break;
		//公告查询
		case CEnumCore::Message_Tag_ID::JW2_BOARDTASK_QUERY:
			DBTFLV=JW2Info.BroadTask_Query("JW2","exec JW2_BOARDTASK_QUERY",
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;
		//公告添加
		case CEnumCore::Message_Tag_ID::JW2_BOARDTASK_INSERT:
			DBTFLV=JW2Info.BroadTask_Insert("JW2","exec JW2_BOARDTASK_INSERT %i,'%s','%s','%s','%s','%s',%i",
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_GSServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_BoardMessage,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_BeginTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_EndTime,0)).lpdata,
				StrToInt((LPCSTR)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_Interval,0)).lpdata));
			break;
		//玩家人物登入/登出信息	
		case CEnumCore::Message_Tag_ID::JW2_LOGINOUT_QUERY:
			DBTFLV=JW2Info.LoginInfo(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ACCOUNT,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_UserNick,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_IP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_BeginTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;
		//道具查询
		case CEnumCore::Message_Tag_ID::JW2_ItemInfo_Query:
			DBTFLV=JW2Info.JW2_ItemInfo_Query(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ACCOUNT,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_UserSN,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ItemPos,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
				);
			break;
		//道具本地模糊查询
		case CEnumCore::Message_Tag_ID::JW2_ITEM_SELECT:
			DBTFLV=JW2Info.JW2_ITEM_SELECT(
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_AvatarItemName,0)).lpdata);
			break;
		//道具删除（身上0，物品栏1，礼盒2）
		case CEnumCore::Message_Tag_ID::JW2_ITEM_DEL:
			DBTFLV=JW2Info.JW2_ITEM_DEL(
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ACCOUNT,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_UserSN,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_AvatarItem,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_AvatarItemName,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ItemPos,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ItemNo,0)).lpdata)
				);
			break;
		//修改等级
		case CEnumCore::Message_Tag_ID::JW2_MODIFYLEVEL_QUERY:
			DBTFLV=JW2Info.JW2_MODIFYLEVEL_QUERY(
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ACCOUNT,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_UserSN,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_Level,0)).lpdata)
				);
			break;
		//修改经验
		case CEnumCore::Message_Tag_ID::JW2_MODIFYEXP_QUERY:
			DBTFLV=JW2Info.JW2_MODIFYEXP_QUERY(
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ACCOUNT,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_UserSN,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_Exp,0)).lpdata)
				);
			break;
		//修改金钱
		case CEnumCore::Message_Tag_ID::JW2_MODIFY_MONEY:
			DBTFLV=JW2Info.JW2_MODIFY_MONEY(
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ACCOUNT,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_UserSN,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_Money,0)).lpdata)
				);
			break;
		//增加道具
		case CEnumCore::Message_Tag_ID::JW2_ADD_ITEM:
			DBTFLV=JW2Info.JW2_ADD_ITEM(
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ACCOUNT,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_UserSN,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_MailTitle,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_MailContent,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_AvatarItem,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageCount,0)).lpdata));
			break;
		//修改密码
		case CEnumCore::Message_Tag_ID::JW2_MODIFY_PWD:
			DBTFLV=JW2Info.JW2_MODIFY_PWD(
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ACCOUNT,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_UserSN,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_PWD,0)).lpdata
				);
			break;	
		//恢复密码
		case CEnumCore::Message_Tag_ID::JW2_RECALL_PWD:
			DBTFLV=JW2Info.JW2_RECALL_PWD(
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ACCOUNT,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_UserSN,0)).lpdata)
				);
			break;
		//金钱日志查询
		case CEnumCore::Message_Tag_ID::JW2_MoneyLog_Query:
			DBTFLV=JW2Info.JW2_MoneyLog_Query(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ACCOUNT,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_UserSN,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_GOODSTYPE,0)).lpdata),				
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_BeginTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
				);
			break;
			//基金日志
		case CEnumCore::Message_Tag_ID::JW2_FamilyFund_Log:
			DBTFLV=JW2Info.JW2_FamilyFund_Log(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_FAMILYNAME,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_FAMILYID,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_Status,0)).lpdata),				
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_BeginTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
				);
			break;
			//家族道具购买日志
		case CEnumCore::Message_Tag_ID::JW2_FamilyBuyLog_Query:
			DBTFLV=JW2Info.JW2_FamilyBuyLog_Query(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_FAMILYNAME,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_FAMILYID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_BeginTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
				);
			break;
		//道具日志
		//任务日志
		case CEnumCore::Message_Tag_ID::JW2_Item_Log:
		case CEnumCore::Message_Tag_ID::JW2_Task_Log:
			{
				char sql_type[256];
				char ServerIP[50];
				sprintf(ServerIP,"%s",(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerIP,0)).lpdata);
				operJW2.TagIDToSqlType(m_message_tag,sql_type);
				operJW2.FindDBIP(ServerIP,ServerIP,operJW2.TagIDToDBID(m_message_tag));
				DBTFLV=JW2Info.BaseLog("JW",sql_type,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerName,0)).lpdata,
				ServerIP,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ACCOUNT,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_UserSN,0)).lpdata),
				2,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_BeginTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_EndTime,0)).lpdata,
				1,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
				);
			}
			break;
		//消费日志
		case CEnumCore::Message_Tag_ID::JW2_CashMoney_Log:
			DBTFLV=JW2Info.JW2_CashMoney_Log(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ACCOUNT,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_UserSN,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_MoneyType,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_BeginTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
				);
			break;
			//批量踢人
		case CEnumCore::Message_Tag_ID::JW2_Import_KickPlayer:
			DBTFLV=JW2Info.JW2_Import_KickPlayer();
			break;
			//批量封停
		case CEnumCore::Message_Tag_ID::JW2_Import_AccountClose:
			DBTFLV=JW2Info.JW2_Import_AccountClose();
			break;
			//批量解封
		case CEnumCore::Message_Tag_ID::JW2_Import_AccountOpen:
			DBTFLV=JW2Info.JW2_Import_AccountOpen();
			break;
			//更改GM状态
		case CEnumCore::Message_Tag_ID::JW2_GM_Update:
			DBTFLV=JW2Info.JW2_GM_Update(
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ACCOUNT,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_UserSN,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_Status,0)).lpdata)
				);
			break;
		//插入权限
		case CEnumCore::Message_Tag_ID::JW2_ProductManage_Insert:
			DBTFLV=JW2Info.JW2_ProductManage_Insert(
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_AvatarItem,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_BeginTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_EndTime,0)).lpdata,
				StrToInt((LPCSTR)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_TotalPrice,0)).lpdata)
				);
			break;
		//查看权限
		case CEnumCore::Message_Tag_ID::JW2_ProductManage_View:
			DBTFLV=JW2Info.JW2_ProductManage_View(
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;
		//删除权限
		case CEnumCore::Message_Tag_ID::JW2_ProductManage_Del:
			DBTFLV=JW2Info.JW2_ProductManage_Del(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerName,0)).lpdata,////ServerIP
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerIP,0)).lpdata,////serverList
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_TaskID,0)).lpdata)
				);
			break;
		//maple add/////验证权限
		case CEnumCore::Message_Tag_ID::JW2_ProductManage_Validate:
			DBTFLV=JW2Info.JW2_ProductManage_Validate(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerName,0)).lpdata,////ServerIP
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerIP,0)).lpdata,////serverList
				StrToInt((LPCSTR)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_UserSN,0)).lpdata),//GMID
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_AvatarItem,0)).lpdata
				);
			break;
		///maple add//验证通过在权限表中减去
		case CEnumCore::Message_Tag_ID::JW2_ProductManage_Deduct:
			DBTFLV=JW2Info.JW2_ProductManage_Deduct(
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				StrToInt((LPCSTR)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_UserSN,0)).lpdata),//GMID
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerIP,0)).lpdata,////serverList
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_AvatarItem,0)).lpdata);///ItemLit
			break;
		default:
			goto FALSE_FUNC;
			break;
		}
		if(!DBTFLV.empty())
		{
			operJW2.BuildTLV(DBTFLV,m_ServiceKey,(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerIP,0)).lpdata);
		}
		else
		{
			operJW2.BuildTLV("数据库为空");
		}
		operJW2.SendBuf(JW2Info.pGameSess,CEnumCore::Msg_Category::JW2_ADMIN,m_ServiceKey,operJW2.GetSendBuf(),operJW2.GetSendLength());
		if(!JW2Info.Destroy())
		{
			return FALSE;
		}
		return TRUE;
	FALSE_FUNC:
		JW2Info.Destroy();
		return FALSE;
	}
	catch (CMemoryException* e)
	{
		
	}
	catch (CFileException* e)
	{
	}
	catch (CException* e)
	{
	}
	return FALSE;
	

}
vector <CGlobalStruct::TFLV> CJW2App::UserInfo(COperVector * precvVect)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	try
	{
		CJW2Info JW2Info;
		DBTFLV=JW2Info.UserInfo(
			"JW2","JW2_ACCOUNT_QUERYBYACCOUNT","JW2_ACCOUNT_QUERYBYNICKNAME",
			(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerName,0)).lpdata,
			(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerIP,0)).lpdata,
			(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ACCOUNT,0)).lpdata,
			(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_UserNick,0)).lpdata,
			1,
			StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
			StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
	}
	catch (CMemoryException* e)
	{
		return DBTFLV;	
	}
	catch (CFileException* e)
	{
		return DBTFLV;
	}
	catch (CException* e)
	{
		return DBTFLV;
	}
	return DBTFLV;
}
vector <CGlobalStruct::TFLV> CJW2App::UserDefault(COperVector * precvVect,CEnumCore::Message_Tag_ID m_message_tag)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	try
	{
		CJW2Info JW2Info;
		COperatorJW2 operJW2;
		char sql_type[256];
		char ServerIP[50];
		sprintf(ServerIP,"%s",(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerIP,0)).lpdata);
		operJW2.TagIDToSqlType(m_message_tag,sql_type);
		operJW2.FindDBIP(ServerIP,ServerIP,operJW2.TagIDToDBID(m_message_tag));
		DBTFLV=JW2Info.UserDefault("JW2",sql_type,
			(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ServerName,0)).lpdata,
			ServerIP,
			(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_ACCOUNT,0)).lpdata,
			StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::JW2_UserSN,0)).lpdata),
			1,
			StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
			StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
	}
	catch (CMemoryException* e)
	{
		return DBTFLV;	
	}
	catch (CFileException* e)
	{
		return DBTFLV;
	}
	catch (CException* e)
	{
		return DBTFLV;
	}
	return DBTFLV;
}

extern CJW2App theApp;
