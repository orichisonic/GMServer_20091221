// XDJG.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "XDJG.h"

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
// CXDJGApp

BEGIN_MESSAGE_MAP(CXDJGApp, CWinApp)
	//{{AFX_MSG_MAP(CXDJGApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXDJGApp construction

CXDJGApp::CXDJGApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CXDJGApp object
extern "C"  bool __declspec(dllexport) XDJG_Main(CSession * pSess,SOCKET m_socket,CEnumCore::Msg_Category m_category,CEnumCore::Message_Tag_ID m_message_tag,unsigned char * pbuf,int length)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	try
	{
//		CXDJGApp theApp;
		CXDJGInfo XDJGInfo;
		COperatorXDJG operXDJG;
		vector <CGlobalStruct::TFLV> DBTFLV;

		//获取要回应的CEnumCore::ServiceKey
		CEnumCore::ServiceKey m_ServiceKey= (m_message_tag & 0x00FFFF) | 0x8000;

		//初始化类对象
		COperVector * precvVect = XDJGInfo.initialize(pSess,m_message_tag, pbuf,length);

		if(!precvVect)
		{
			return false;
		}

		switch((unsigned int)m_message_tag)
		{
		//查询玩家资料信息
		case CEnumCore::Message_Tag_ID::XDJG_Account_Query:
			DBTFLV = XDJGInfo.XDJG_Net_UserInfo((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_SEARCHTYPE,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//踢玩家下线
		case CEnumCore::Message_Tag_ID::XDJG_KickPlayer:
			DBTFLV = XDJGInfo.XDJG_Net_KickPlayer(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata);
			break;

		//玩家帐号封停
		case CEnumCore::Message_Tag_ID::XDJG_ClosePlayer:
			DBTFLV = XDJGInfo.XDJG_Net_ClosePlayer(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_AccountID,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_BANTIME,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_BANREASON,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_BANTYPE,0)).lpdata));
			break;

		//玩家帐号解封
		case CEnumCore::Message_Tag_ID::XDJG_OpenPlayer:
			DBTFLV = XDJGInfo.XDJG_Net_OpenPlayer(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_BANREASON,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_BANTYPE,0)).lpdata));
			break;

		//查询封停列表
		case CEnumCore::Message_Tag_ID::XDJG_CloseList:
			DBTFLV = XDJGInfo.XDJG_Net_ClosePlayer_List((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//添加公告
		case CEnumCore::Message_Tag_ID::XDJG_BoardTasker_Add:
			DBTFLV = XDJGInfo.XDJG_Net_BoardTasker_Add(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_BoardMessage,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_BeginTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Interval,0)).lpdata));
			break;

		//修改公告
		case CEnumCore::Message_Tag_ID::XDJG_BoardTasker_Update:
			DBTFLV = XDJGInfo.XDJG_Net_BoardTasker_Update(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_TaskID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_BoardMessage,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_BeginTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Interval,0)).lpdata));
			break;

		//删除公告
		case CEnumCore::Message_Tag_ID::XDJG_BoardTasker_Del:
			DBTFLV = XDJGInfo.XDJG_Net_BoardTasker_Delete(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_TaskID,0)).lpdata));
			break;

		//查询公告
		case CEnumCore::Message_Tag_ID::XDJG_BoardTasker_Query:
			DBTFLV=XDJGInfo.XDJG_Net_BoardTasker_Query("XDJG","exec XDJG_BOARDTASK_QUERY",
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//Added by tengjie 2010-07-06
		//日志类:玩家登录日志
		case CEnumCore::Message_Tag_ID::XDJG_Login_Log:
			DBTFLV = XDJGInfo.XDJG_DB_Login_Log(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_LogInOutIP,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//商城物品购买日志
		case CEnumCore::Message_Tag_ID::XDJG_Shoping_Log:
			DBTFLV = XDJGInfo.XDJG_DB_Shoping_Log(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_QueryType,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//玩家物品日志:交易获得
		case CEnumCore::Message_Tag_ID::XDJG_TransGain_Log:
			DBTFLV = XDJGInfo.XDJG_DB_TransGain_Log(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//玩家物品日志:交易失去
		case CEnumCore::Message_Tag_ID::XDJG_TransLose_Log:
			DBTFLV = XDJGInfo.XDJG_DB_TransLose_Log(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//玩家物品日志:从NPC出购买
		case CEnumCore::Message_Tag_ID::XDJG_BuyFromNpc_Log:
			DBTFLV = XDJGInfo.XDJG_DB_BuyFromNpc_Log(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//玩家物品日志:卖给NPC
		case CEnumCore::Message_Tag_ID::XDJG_SaleToNpc_Log:
			DBTFLV = XDJGInfo.XDJG_DB_SaleToNpc_Log(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//玩家物品日志:拾取物品
		case CEnumCore::Message_Tag_ID::XDJG_PickUpItem_Log:
			DBTFLV = XDJGInfo.XDJG_DB_PickUpItem_Log(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//玩家物品日志:丢弃物品
		case CEnumCore::Message_Tag_ID::XDJG_DiscardItem_Log:
			DBTFLV = XDJGInfo.XDJG_DB_DiscardItem_Log(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//玩家物品日志:物品消耗
		case CEnumCore::Message_Tag_ID::XDJG_ConsumeItem_Log:
			DBTFLV = XDJGInfo.XDJG_DB_ConsumeItem_Log(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//玩家物品日志:拍卖行卖出
		case CEnumCore::Message_Tag_ID::XDJG_AuctioneerSaled_Log:
			DBTFLV = XDJGInfo.XDJG_DB_AuctioneerSaled_Log(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//玩家物品日志:拍卖行购入
		case CEnumCore::Message_Tag_ID::XDJG_AuctioneerBought_Log:
			DBTFLV = XDJGInfo.XDJG_DB_AuctioneerBought_Log(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//玩家邮件日志:发送邮件
		case CEnumCore::Message_Tag_ID::XDJG_SendMail_Log:
			DBTFLV = XDJGInfo.XDJG_DB_SendMail_Log(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//玩家邮件日志:收取邮件
		case CEnumCore::Message_Tag_ID::XDJG_ReceiveMail_Log:
			DBTFLV = XDJGInfo.XDJG_DB_ReceiveMail_Log(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//M币使用日志:背包扩充续费记录
		case CEnumCore::Message_Tag_ID::XDJG_ExpansionPack_Log:
			DBTFLV = XDJGInfo.XDJG_DB_ExpansionPack_Log(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//M币使用日志:传送日志
		case CEnumCore::Message_Tag_ID::XDJG_Deliver_Log:
			DBTFLV = XDJGInfo.XDJG_DB_Deliver_Log(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//M币使用日志:角色仓库扩充续费记录
		case CEnumCore::Message_Tag_ID::XDJG_ExpansionWarehouse_Log:
			DBTFLV = XDJGInfo.XDJG_DB_ExpansionWarehouse_Log(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//玩家角色日志:角色升级
		case CEnumCore::Message_Tag_ID::XDJG_RoleUpgrade_Log:
			DBTFLV = XDJGInfo.XDJG_DB_RoleUpgrade_Log(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//玩家角色日志:角色创建
		case CEnumCore::Message_Tag_ID::XDJG_RoleCreate_Log:
			DBTFLV = XDJGInfo.XDJG_DB_RoleCreate_Log(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//玩家角色日志:角色技能学习
		case CEnumCore::Message_Tag_ID::XDJG_RoleSkill_Log:
			DBTFLV = XDJGInfo.XDJG_DB_RoleSkill_Log(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//玩家角色日志:领取任务
		case CEnumCore::Message_Tag_ID::XDJG_ReceiveTask_Log:
			DBTFLV = XDJGInfo.XDJG_DB_ReceiveTask_Log(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//玩家角色日志:完成任务
		case CEnumCore::Message_Tag_ID::XDJG_CompleteTask_Log:
			DBTFLV = XDJGInfo.XDJG_DB_CompleteTask_Log(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//玩家角色日志:副本查询
		case CEnumCore::Message_Tag_ID::XDJG_Copy_Log:
			DBTFLV = XDJGInfo.XDJG_DB_Copy_Log(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//查询类
		//玩家物品信息:身上物品信息
		case CEnumCore::Message_Tag_ID::XDJG_ItemBody_Query:
			DBTFLV = XDJGInfo.XDJG_DB_ItemBody_Query(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;
			
		//玩家物品信息:背包物品信息
		case CEnumCore::Message_Tag_ID::XDJG_Itembackpack_Query:
			DBTFLV = XDJGInfo.XDJG_DB_Itembackpack_Query(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;
			
		//玩家物品信息:仓库物品信息
		case CEnumCore::Message_Tag_ID::XDJG_ItemStorage_Query:
			DBTFLV = XDJGInfo.XDJG_DB_ItemStorage_Query(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;
			
		//玩家物品信息:公会仓库信息
		case CEnumCore::Message_Tag_ID::XDJG_GuildStorage_Query:
			DBTFLV = XDJGInfo.XDJG_DB_GuildStorage_Query(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;
			
		//玩家物品信息:金刚仓库信息
		case CEnumCore::Message_Tag_ID::XDJG_KingkongStorage_Query:
			DBTFLV = XDJGInfo.XDJG_DB_KingkongStorage_Query(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;
			
		//邮件信息
		case CEnumCore::Message_Tag_ID::XDJG_MailInfo_Query:
			DBTFLV = XDJGInfo.XDJG_DB_MailInfo_Query(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;
			
		//拍卖行信息
		case CEnumCore::Message_Tag_ID::XDJG_AuctioneerInfo_Query:
			DBTFLV = XDJGInfo.XDJG_DB_AuctioneerInfo_Query(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//操作类
		//角色技能添加
		case CEnumCore::Message_Tag_ID::XDJG_RoleSkillAdd:
			DBTFLV = XDJGInfo.XDJG_Net_RoleSkillAdd(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_SkillID,0)).lpdata));
			break;
		//角色技能删除
		case CEnumCore::Message_Tag_ID::XDJG_RoleSkillDel:
			DBTFLV = XDJGInfo.XDJG_Net_RoleSkillDel(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_SkillID,0)).lpdata));
			break;
		//道具添加
		case CEnumCore::Message_Tag_ID::XDJG_ItemAdd:
			DBTFLV = XDJGInfo.XDJG_Net_ItemAdd(
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_SendCharacter,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_MailTitle,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_MailContent,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Money,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ItemID,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ItemNum,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_PropertyId,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_PropertyValue,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_IncreaseNum,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_IncreaseType,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_IncreaseValue,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_IncreaseEndValue,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StarsLevel,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_AssemblyID,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_SlotNum,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EquipSlotID1,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EquipSlotID2,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EquipSlotID3,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_QualityEquip,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_IsBind,0)).lpdata/*,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ItemName,0)).lpdata*/);
			break;
// 		case CEnumCore::Message_Tag_ID::XDJG_ItemAdd:
// 			DBTFLV = XDJGInfo.XDJG_Net_ItemAdd(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
// 				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
// 				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
// 				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ItemID,0)).lpdata,
// 				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ItemNum,0)).lpdata);
// 			break;
		//道具删除
		case CEnumCore::Message_Tag_ID::XDJG_ItemDel:
			DBTFLV = XDJGInfo.XDJG_Net_ItemDel(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ItemID,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ItemNum,0)).lpdata));
			break;

		//游戏币查询
		case CEnumCore::Message_Tag_ID::XDJG_MoneyQuery:
			DBTFLV = XDJGInfo.XDJG_Net_MoneyQuery((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata);
			break;

		//添加游戏币
		case CEnumCore::Message_Tag_ID::XDJG_MoneyAdd:
			DBTFLV = XDJGInfo.XDJG_Net_MoneyAdd(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Money,0)).lpdata));
			break;

		//用户角色信息的修改:修改等级/经验
		case CEnumCore::Message_Tag_ID::XDJG_ModifyLevel_operate:
			DBTFLV = XDJGInfo.XDJG_Net_ModifyLevel(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_UpdateType,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Level,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Exp,0)).lpdata));
			break;
				
		//用户密码管理:修改密码
		case CEnumCore::Message_Tag_ID::XDJG_ModifyPwd_operate:
			DBTFLV = XDJGInfo.XDJG_Net_ModifyPwd(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Pwd,0)).lpdata);
			break;
				
		//用户密码管理:临时密码恢复
		case CEnumCore::Message_Tag_ID::XDJG_ResumePwd_operate:
			DBTFLV = XDJGInfo.XDJG_Net_ResumePwd(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata);
			break;
			
		//用户角色管理:角色恢复
		case CEnumCore::Message_Tag_ID::XDJG_ResumeRole_operate:
			DBTFLV = XDJGInfo.XDJG_Net_ResumeRole(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata);
			break;

		//用户角色管理:角色删除
		case CEnumCore::Message_Tag_ID::XDJG_DeleteRole_operate:
			DBTFLV = XDJGInfo.XDJG_Net_DeleteRole(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata);
			break;
				
		//修改角色身上金钱数量(绑定币)
		case CEnumCore::Message_Tag_ID::XDJG_UpRoleBind_operate:
			DBTFLV = XDJGInfo.XDJG_Net_UpRoleBind(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Money,0)).lpdata));
			break;
				
		//修改角色身上金钱数量(非绑定币)
		case CEnumCore::Message_Tag_ID::XDJG_UpRoleNonBind_operate:
			DBTFLV = XDJGInfo.XDJG_Net_UpRoleNonBind(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Money,0)).lpdata));
			break;

		//添加角色身上金钱数量(绑定币)
		case CEnumCore::Message_Tag_ID::XDJG_AddRoleBind_operate:
			DBTFLV = XDJGInfo.XDJG_Net_AddRoleBind(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Money,0)).lpdata));
			break;

		//添加角色身上金钱数量(非绑定币)
		case CEnumCore::Message_Tag_ID::XDJG_AddRoleNonBind_operate:
			DBTFLV = XDJGInfo.XDJG_Net_AddRoleNonBind(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Money,0)).lpdata));
			break;

		//角色技能信息查询
		case CEnumCore::Message_Tag_ID::XDJG_RoleSkills_Query:
			DBTFLV = XDJGInfo.XDJG_RoleSkills_Query((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//技能列表查询
		case CEnumCore::Message_Tag_ID::XDJG_SkillList_Query:
			DBTFLV = XDJGInfo.XDJG_SkillList_Query("XDJG", "exec XDJG_Skilllist_Query %s",
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Profession,0)).lpdata);
			break;

		//道具类型查询
		case CEnumCore::Message_Tag_ID::XDJG_ItemType_Query:
			DBTFLV = XDJGInfo.XDJG_ItemType_Query("XDJG", "exec XDJG_Itemtype_Query");
			break;

		//相应类型的道具列表查询
		case CEnumCore::Message_Tag_ID::XDJG_ItemNameLst_Query:
			DBTFLV = XDJGInfo.XDJG_ItemNameLst_Query("XDJG","exec XDJG_Item_Query '%s','%s'",
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ItemType,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ItemName,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//物品属性查询
		case CEnumCore::Message_Tag_ID::XDJG_ItemAttribute_Query:
			DBTFLV = XDJGInfo.XDJG_ItemAttribute_Query(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ItemID,0)).lpdata);
			break;

		//公会信息查询
		//公会基本信息
		case CEnumCore::Message_Tag_ID::XDJG_GuildBaseInfo_Query:
			DBTFLV = XDJGInfo.XDJG_GuildBaseInfo_Query(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_GuildName,0)).lpdata);
			break;
			
		//公会成员列表
		case CEnumCore::Message_Tag_ID::XDJG_GuildMemberList_Query:
			DBTFLV = XDJGInfo.XDJG_GuildMemberList_Query(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_GuildName,0)).lpdata);
			break;

		//玩家任务查询
		case CEnumCore::Message_Tag_ID::XDJG_PlayerTaskInfo_Query:
			DBTFLV = XDJGInfo.XDJG_PlayerTaskInfo_Query(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//金刚车辆信息
		//金刚车辆基本信息
		case CEnumCore::Message_Tag_ID::XDJG_CarBaseInfo_Query:
			DBTFLV = XDJGInfo.XDJG_CarBaseInfo_Query(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//金刚车辆详细信息
		case CEnumCore::Message_Tag_ID::XDJG_CarDetailInfo_Query:
			DBTFLV = XDJGInfo.XDJG_CarDetailInfo_Query(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//玩家公会信息
		//玩家公会基本信息
		case CEnumCore::Message_Tag_ID::XDJG_PlayerGuildBaseInfo_Query:
			DBTFLV = XDJGInfo.XDJG_PlayerGuildBaseInfo_Query(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//玩家公会物品信息
		case CEnumCore::Message_Tag_ID::XDJG_PlayerGuildItemInfo_Query:
			DBTFLV = XDJGInfo.XDJG_PlayerGuildItemInfo_Query(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//玩家物品日志
		//装备增幅日志
		case CEnumCore::Message_Tag_ID::XDJG_EquipIncrease_Log:
			DBTFLV = XDJGInfo.XDJG_EquipIncrease_Log_Query(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//插片合成日志
		case CEnumCore::Message_Tag_ID::XDJG_Synthesis_Log:
			DBTFLV = XDJGInfo.XDJG_Synthesis_Log_Query(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//装备传导日志
		case CEnumCore::Message_Tag_ID::XDJG_EquipTransfer_Log:
			DBTFLV = XDJGInfo.XDJG_EquipTransfer_Log_Query(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;
				
		//物品鉴定
		case CEnumCore::Message_Tag_ID::XDJG_ItemIdentify_Log:
			DBTFLV = XDJGInfo.XDJG_ItemIdentify_Log_Query(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//个人仓库存放日志
		case CEnumCore::Message_Tag_ID::XDJG_RoleStore_Log:
			DBTFLV = XDJGInfo.XDJG_RoleStore_Log_Query(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//公会仓库存放日志
		case CEnumCore::Message_Tag_ID::XDJG_GuildStore_Log:
			DBTFLV = XDJGInfo.XDJG_GuildStore_Log_Query(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//公会日志
		//公会成立
		case CEnumCore::Message_Tag_ID::XDJG_GuildCreate_Log:
			DBTFLV = XDJGInfo.XDJG_GuildCreate_Log_Query(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_GuildName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//公会解散
		case CEnumCore::Message_Tag_ID::XDJG_GuildDismiss_Log:
			DBTFLV = XDJGInfo.XDJG_GuildDismiss_Log_Query(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_GuildName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//公会添加成员		
		case CEnumCore::Message_Tag_ID::XDJG_GuildAddMember_Log:
			DBTFLV = XDJGInfo.XDJG_GuildAddMember_Log_Query(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_GuildName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//公会成员离开
		case CEnumCore::Message_Tag_ID::XDJG_GuildMemberLeaved_Log:
			DBTFLV = XDJGInfo.XDJG_GuildMemberLeaved_Log_Query(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_GuildName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//公会职务变更
		case CEnumCore::Message_Tag_ID::XDJG_GuildPostChanged_Log:
			DBTFLV = XDJGInfo.XDJG_GuildPostChanged_Log_Query(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_GuildName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//帐号是否封停
		case CEnumCore::Message_Tag_ID::XDJG_AccountIsClose_Query:
			DBTFLV = XDJGInfo.XDJG_AccountIsClose_Query(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata);
			break;

		//NPC道具领取查询
		case CEnumCore::Message_Tag_ID::XDJG_NPCGetItem_Query:
			DBTFLV = XDJGInfo.XDJG_NPCGetItem_Query(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_AccountID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//添加称号
		case CEnumCore::Message_Tag_ID::XDJG_AddTitle_Query:
			DBTFLV = XDJGInfo.XDJG_AddTitle_Query(
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_TitleID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_TitleName,0)).lpdata);
			break;
				
		//移除称号
		case CEnumCore::Message_Tag_ID::XDJG_RemoveTitle_Query:
			DBTFLV = XDJGInfo.XDJG_RemoveTitle_Query(
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_TitleID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_TitleName,0)).lpdata);
			break;
			
		//添加BUFF
		case CEnumCore::Message_Tag_ID::XDJG_AddBuff_Query:
			DBTFLV = XDJGInfo.XDJG_AddBuff_Query(
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_BuffID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_BuffName,0)).lpdata);
			break;

		//移除BUFF
		case CEnumCore::Message_Tag_ID::XDJG_RemoveBuff_Query:
			DBTFLV = XDJGInfo.XDJG_RemoveBuff_Query(
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_BuffID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_BuffName,0)).lpdata);
			break;
				
		//移动到指定位置
		case CEnumCore::Message_Tag_ID::XDJG_MoveToPosition_Query:
			DBTFLV = XDJGInfo.XDJG_MoveToPosition_Query(
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_PositionX,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_PositionY,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_PositionZ,0)).lpdata));
			break;
				
		//移动到指定玩家身边
		case CEnumCore::Message_Tag_ID::XDJG_MoveToRole_Query:
			DBTFLV = XDJGInfo.XDJG_MoveToRole_Query(
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_TargetCharacter,0)).lpdata);
			break;
				
		//移动到目标玩家身边
		case CEnumCore::Message_Tag_ID::XDJG_MoveToTargetRole_Query:
			DBTFLV = XDJGInfo.XDJG_MoveToTargetRole_Query(
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_TargetCharacter,0)).lpdata);
			break;

		//称号列表查询
		case CEnumCore::Message_Tag_ID::XDJG_TitleList_Query:
			DBTFLV = XDJGInfo.XDJG_TitleList_Query("XDJG","exec XDJG_TitleInfo_Query");
			break;
				
		//BUFF列表查询
		case CEnumCore::Message_Tag_ID::XDJG_BuffList_Query:
			DBTFLV = XDJGInfo.XDJG_BuffList_Query("XDJG","exec XDJG_BuffInfo_Query");
			break;
		//End

		default:
//			XDJGInfo.Destroy();
//			return FALSE;
			break;
		}

 		
		if(!DBTFLV.empty())
		{

#ifdef _DEBUG
			vector <CGlobalStruct::TFLV>::iterator iter;
			int tflvSize = DBTFLV.size();
			for(iter=DBTFLV.begin(); iter!=DBTFLV.end(); iter++)
			{
				
				printf("%x %x %s\n", (*iter).m_tagName, (*iter).m_tagFormat, (*iter).lpdata);
			}
#endif
			operXDJG.BuildTLV(DBTFLV,m_ServiceKey,(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata);
		}
		else
		{
			operXDJG.BuildTLV("数据库为空");
		}

		//数据发送
		operXDJG.SendBuf(XDJGInfo.pGameSess,CEnumCore::Msg_Category::XDJG_ADMIN,m_ServiceKey,operXDJG.GetSendBuf(),operXDJG.GetSendLength());
		
		if(!XDJGInfo.Destroy())
		{
			return FALSE;
		}

	}
	catch (...)
	{
		return FALSE;
	}
	
	return TRUE;
}
