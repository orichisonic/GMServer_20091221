// SDO.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "SDO.h"

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
// CSDOApp

BEGIN_MESSAGE_MAP(CSDOApp, CWinApp)
	//{{AFX_MSG_MAP(CSDOApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSDOApp construction

CSDOApp::CSDOApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSDOApp object
extern "C"  bool __declspec(dllexport) SDO_Main(CSession * pSess,SOCKET m_socket,CEnumCore::Msg_Category m_category,CEnumCore::Message_Tag_ID m_message_tag,unsigned char * pbuf,int length)
{
	try
	{
//		CSDOApp theApp;
		CSDOInfo SDOInfo;
		COperatorSDO operSDO;
		vector <CGlobalStruct::TFLV> DBTFLV;
		CEnumCore::ServiceKey m_ServiceKey= (m_message_tag & 0x00FFFF) | 0x8000;//获取要回应的CEnumCore::ServiceKey

		COperVector * precvVect = SDOInfo.initialize(pSess,m_message_tag, pbuf,length);
		if(!precvVect)//初始化类对象
		{
			return false;
		}

		switch((unsigned int)m_message_tag)
		{
		//查询玩家资料信息	
		case CEnumCore::Message_Tag_ID::SDO_CHARACTERINFO_QUERY:
			DBTFLV = SDOInfo.SDO_UserInfo(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_NickName,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
				);
			break;
		//查询玩家帐号信息
		case CEnumCore::Message_Tag_ID::SDO_ACCOUNT_QUERY:
			DBTFLV = SDOInfo.SDO_AccountInfo(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_NickName,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
				);
			break;
		//查看玩家身上的道具
		case CEnumCore::Message_Tag_ID::SDO_ITEMSHOP_BYOWNER_QUERY:
			DBTFLV = SDOInfo.SDO_ItemInfo(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_Account,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_UserIndexID,0)).lpdata),			
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
				);
			break;
		//查看玩家的宠物信息
		case CEnumCore::Message_Tag_ID::SDO_PetInfo_Query:
			DBTFLV = SDOInfo.SDO_PetInfo(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_Account,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_UserIndexID,0)).lpdata),			
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
				);
			break;
		//查看玩家的宠物删除信息
		case CEnumCore::Message_Tag_ID::SDO_PetDrop_Query:
			DBTFLV = SDOInfo.SDO_PetDrop(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_Account,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_UserIndexID,0)).lpdata),			
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
				);
			break;
		//查询玩家账号登录记录
		case CEnumCore::Message_Tag_ID::SDO_USERONLINE_QUERY:
			DBTFLV = SDOInfo.SDO_LoginLog(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_Account,0)).lpdata,			
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
				);
			break;
		//查询玩家物品赠送记录
		case CEnumCore::Message_Tag_ID::SDO_ITEMSHOP_TRADE_QUERY:
			DBTFLV = SDOInfo.SDO_UserTrade_Log(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_NickName,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_Type,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
				);
			break;
		//查询玩家消费记录
		case CEnumCore::Message_Tag_ID::SDO_CONSUME_QUERY:
			DBTFLV = SDOInfo.SDO_UserConsume(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_NickName,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_MoneyType,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ItemName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_BeginTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
				);
			break;
		//查看玩家当前状态(服务器/房间/在线状态)
		case CEnumCore::Message_Tag_ID::SDO_USERLOGIN_STATUS_QUERY:
			DBTFLV = SDOInfo.SDO_UserStatus(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_Account,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_UserIndexID,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;
		//查看玩家婚姻状态
		case CEnumCore::Message_Tag_ID::SDO_USERMARRIAGE_QUERY:
			DBTFLV = SDOInfo.SDO_UserMarriage(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_NickName,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_UserIndexID,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
				);
			break;
		//查看玩家摇摇乐获得物品
		case CEnumCore::Message_Tag_ID::SDO_RewardItem_QUERY:
			DBTFLV = SDOInfo.SDO_YYItem_QUERY(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_Account,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
				);
			break;
		//强制玩家下线
		case CEnumCore::Message_Tag_ID::SDO_USERLOGIN_DEL:
			DBTFLV = SDOInfo.SDO_KickPlayer_Off(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_Account,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_UserIndexID,0)).lpdata)
				);
			break;
		//查询玩家封停状态
		case CEnumCore::Message_Tag_ID::SDO_MEMBERSTOPSTATUS_QUERY:
			DBTFLV = SDOInfo.SDO_UserBanStatus(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_Account,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
				);
			break;
		//封停玩家帐号
		case CEnumCore::Message_Tag_ID::SDO_ACCOUNT_CLOSE:
			DBTFLV = SDOInfo.SDO_Account_Close(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_REASON,0)).lpdata);
			break;
		//查询所有被封停帐号列表
		case CEnumCore::Message_Tag_ID::SDO_MEMBERBANISHMENT_QUERY:
			DBTFLV = SDOInfo.SDO_QueryBan_All(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
				);
			break;
		//解封玩家帐号
		case CEnumCore::Message_Tag_ID::SDO_ACCOUNT_OPEN:
			DBTFLV = SDOInfo.SDO_Account_Open(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_REASON,0)).lpdata
				);
			break;
		//查询公告信息列表
		case CEnumCore::Message_Tag_ID::SDO_BOARDTASK_QUERY:
			DBTFLV = SDOInfo.SDO_NoticeInfo(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,				
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
				);
			break;
		//添加公告信息
		case CEnumCore::Message_Tag_ID::SDO_BOARDTASK_INSERT:
			DBTFLV = SDOInfo.SDO_AddNotice(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_BoardMessage,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_BeginTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_Interval,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_Type,0)).lpdata)
				);
			break;
		//查询发送公告的大区列表信息
		case CEnumCore::Message_Tag_ID::SDO_BOARDMESSAGE_REQ:
			DBTFLV = SDOInfo.SDO_BoardCity_Query(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_TaskID,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
				);
			break;
		//修改公告信息
		case CEnumCore::Message_Tag_ID::SDO_BOARDTASK_UPDATE:
			DBTFLV = SDOInfo.SDO_EditNotice(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_TaskID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_BoardMessage,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_BeginTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_Interval,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_Status,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_Type,0)).lpdata)
				);
			break;
		//查询家族基本信息
		case CEnumCore::Message_Tag_ID::SDO_Family_Info_QUERY:
			DBTFLV = SDOInfo.SDO_FamilyInfo(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_FamilyName,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
				);
			break;
			//查询家族成员信息
		case CEnumCore::Message_Tag_ID::SDO_FamilyMember_Query:
			DBTFLV = SDOInfo.SDO_FamilyMember(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_FamilyID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_FamilyName,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
				);
			break;
		//查询申请中家族成员信息
		case CEnumCore::Message_Tag_ID::SDO_FamilyFormal_Query:
			DBTFLV = SDOInfo.SDO_FamilyFormal(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_FamilyID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_FamilyName,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
				);
			break;
		//查询家族勋章信息
		case CEnumCore::Message_Tag_ID::SDO_Personal_Emblem_Query:
			DBTFLV = SDOInfo.SDO_FamilyBadge(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_FamilyID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_FamilyName,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
				);
			break;
		//查询家族金库捐赠明细
		case CEnumCore::Message_Tag_ID::SDO_Family_Donate_Query:
			DBTFLV = SDOInfo.SDO_FamilyDonate(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_FamilyID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_FamilyName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_BeginTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
				);
			break;
		//查询家族购买记录
		case CEnumCore::Message_Tag_ID::SDO_Family_CONSUME_QUERY:
			DBTFLV = SDOInfo.SDO_FamilyConsume(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_FamilyID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_FamilyName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_BeginTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
				);
			break;
		//修改家族成员职务
		case CEnumCore::Message_Tag_ID::SDO_Family_Duty_Query:
			DBTFLV = SDOInfo.SDO_ModiMemDuty(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_NickName,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_UserIndexID,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_FamilyID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_FamilyName,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_Type,0)).lpdata)
				); 
			break;
		//解散家族
		case CEnumCore::Message_Tag_ID::SDO_Family_Delete_Query:
			DBTFLV = SDOInfo.SDO_DeleteFamily(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_FamilyID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_FamilyName,0)).lpdata
				);
			break;
		//添加家族徽章
		case CEnumCore::Message_Tag_ID::SDO_Family_Emblem_Insert_Query:
			DBTFLV = SDOInfo.SDO_Insert_FamilyBadge(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_NickName,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_UserIndexID,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_FamilyID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_FamilyName,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_Emblemtype,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_Emblemnum,0)).lpdata)
				);
			break;
		//删除家族徽章
		case CEnumCore::Message_Tag_ID::SDO_Family_Emblem_Delete_Query:
			DBTFLV = SDOInfo.SDO_Delete_FamilyBadge(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_NickName,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_UserIndexID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_FamilyName,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_Emblemtype,0)).lpdata)
				);
			break;
		//查看玩家G币
		case CEnumCore::Message_Tag_ID::SDO_USERGCASH_QUERY:
			DBTFLV = SDOInfo.SDO_UserGCash_Query(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_Account,0)).lpdata);
			break;
		//补发玩家G币
		case CEnumCore::Message_Tag_ID::SDO_USERGCASH_UPDATE:
			DBTFLV = SDOInfo.SDO_UserGCash_Update(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_Account,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_GCash,0)).lpdata)
				);
			break;
		//大赛挑战信息查询
		case CEnumCore::Message_Tag_ID::SDO_CHALLENGE_QUERY:
			DBTFLV = SDOInfo.SDO_CHALLENGE_QUERY(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
				);
			break;
		//设置大赛挑战信息
		case CEnumCore::Message_Tag_ID::SDO_CHALLENGE_INSERT:
			DBTFLV = SDOInfo.SDO_CHALLENGE_INSERT(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_WeekDay,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_MatPtMin,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_StPtMin,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_GCash,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_Sence,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_IsBattle,0)).lpdata)
				);
			break;
		//修改大赛挑战信息
		case CEnumCore::Message_Tag_ID::SDO_CHALLENGE_UPDATE:
			DBTFLV = SDOInfo.SDO_CHALLENGE_UPDATE(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_SenceID,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_WeekDay,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_MatPtMin,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_StPtMin,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_GCash,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_Sence,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_IsBattle,0)).lpdata)
				);
			break;
		//删除大赛挑战信息
		case CEnumCore::Message_Tag_ID::SDO_CHALLENGE_DELETE:
			DBTFLV = SDOInfo.SDO_CHALLENGE_DELETE(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_SenceID,0)).lpdata)
				);
			break;
		//查看游戏场景列表
		case CEnumCore::Message_Tag_ID::SDO_CHALLENGE_SCENE_QUERY:
			DBTFLV = SDOInfo.SDO_SCENE_QUERY(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata
				);
			break;
		//添加大赛挑战的场景
		case CEnumCore::Message_Tag_ID::SDO_CHALLENGE_SCENE_CREATE:
			DBTFLV = SDOInfo.SDO_SCENE_CREATE(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_Sence,0)).lpdata
				);
			break;
		//修改大赛挑战的场景
		case CEnumCore::Message_Tag_ID::SDO_CHALLENGE_SCENE_UPDATE:
			DBTFLV = SDOInfo.SDO_SCENE_UPDATE(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_SenceID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_Sence,0)).lpdata
				);
			break;
		//删除大赛挑战的场景
		case CEnumCore::Message_Tag_ID::SDO_CHALLENGE_SCENE_DELETE:
			DBTFLV = SDOInfo.SDO_SCENE_DELETE(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_SenceID,0)).lpdata)
				);
			break;
		//查看在游戏里摇摇乐获得概率
		case CEnumCore::Message_Tag_ID::SDO_YYHAPPYITEM_QUERY:
			DBTFLV = SDOInfo.SDO_YYHappyItem_Query(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
				);
			break;
		//添加摇摇乐获得概率
		case CEnumCore::Message_Tag_ID::SDO_YYHAPPYITEM_CREATE:
			DBTFLV = SDOInfo.SDO_YYHappyItem_Insert(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ItemCode1,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ItemCode2,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_Level,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_LevPercent,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_Precent,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_TimesLimit,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_DaysLimit,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ItemName1,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ItemName2,0)).lpdata
				);
			break;
		//更新摇摇乐获得概率
		case CEnumCore::Message_Tag_ID::SDO_YYHAPPYITEM_UPDATE:
			DBTFLV = SDOInfo.SDO_YYHappyItem_Update(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ItemCode,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ItemCode1,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ItemCode2,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_Level,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_LevPercent,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_Precent,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_TimesLimit,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_DaysLimit,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ItemName1,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ItemName2,0)).lpdata
				);
			break;
		//删除摇摇乐获得概率
		case CEnumCore::Message_Tag_ID::SDO_YYHAPPYITEM_DELETE:
			DBTFLV = SDOInfo.SDO_YYHappyItem_Delete(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ItemCode,0)).lpdata)
				);
			break;
		//查看游戏里面的道具信息
		case CEnumCore::Message_Tag_ID::SDO_ITEMSHOP_QUERY:
			DBTFLV = SDOInfo.SDO_Item_QueryAll(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_BigType,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_SmallType,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_SEX,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ItemName,0)).lpdata
				);
			break;
		//道具获取比率查询
		case CEnumCore::Message_Tag_ID::SDO_MEDALITEM_QUERY:
			DBTFLV = SDOInfo.SDO_MedalItem_Query(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
				);
			break;
		//单个道具获取比率查询
		case CEnumCore::Message_Tag_ID::SDO_MEDALITEM_OWNER_QUERY:
			DBTFLV = SDOInfo.SDO_MedalItem_Owner_Query(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ItemName,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
				);
			break;
		//添加道具获取比率
		case CEnumCore::Message_Tag_ID::SDO_MEDALITEM_CREATE:
			DBTFLV = SDOInfo.SDO_MedalItem_Insert(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ItemCode,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_Precent,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ItemName,0)).lpdata
				);
			break;
		//更新道具获取比率
		case CEnumCore::Message_Tag_ID::SDO_MEDALITEM_UPDATE:
			DBTFLV = SDOInfo.SDO_MedalItem_Update(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ItemCode,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_Precent,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ItemName,0)).lpdata
				);
			break;
		//删除道具获取比率
		case CEnumCore::Message_Tag_ID::SDO_MEDALITEM_DELETE:
			DBTFLV = SDOInfo.SDO_MedalItem_Delete(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ItemCode,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ItemName,0)).lpdata
				);
			break;
		//更新玩家角色信息
		case CEnumCore::Message_Tag_ID::SDO_CHARACTERINFO_UPDATE:
			DBTFLV = SDOInfo.SDO_CharacterInfo_Update(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_UserIndexID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_Account,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_Level,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_Exp,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_GameTotal,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_GameWin,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_DogFall,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_GameFall,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_MCash,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_GCash,0)).lpdata)
				);
			break;
/*   重复？？？
			//查看玩家身上道具
		case CEnumCore::Message_Tag_ID::SDO_ITEMSHOP_BYOWNER_QUERY:
			SDOInfo.SDO_UserEquip_Query()
			break;
*/
		//删除玩家身上道具
		case CEnumCore::Message_Tag_ID::SDO_ITEMSHOP_DELETE:
			DBTFLV = SDOInfo.SDO_ItemShop_Delete(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_UserIndexID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_Account,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ItemCode,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ItemName,0)).lpdata
				);
			break;
		//查看玩家礼物盒道具
		case CEnumCore::Message_Tag_ID::SDO_GIFTBOX_QUERY:
			DBTFLV = SDOInfo.SDO_GiftBox_Query(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_UserIndexID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_Account,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
				);
			break;
		//删除玩家礼物盒道具
		case CEnumCore::Message_Tag_ID::SDO_GIFTBOX_DELETE:
			DBTFLV = SDOInfo.SDO_GiftBox_Delete(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_UserIndexID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_Account,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ItemCode,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ItemName,0)).lpdata);
			break;
		//给玩家发送道具
		case CEnumCore::Message_Tag_ID::SDO_GIFTBOX_CREATE:
			DBTFLV = SDOInfo.SDO_GiftBox_Insert(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_UserIndexID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_Account,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ItemCode,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_TimesLimit,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_DateLimit,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ItemName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_Title,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_Context,0)).lpdata
				);
			break;
		//批量添加比赛信息
		case CEnumCore::Message_Tag_ID::SDO_CHALLENGE_INSERTALL:
			DBTFLV = SDOInfo.SDO_Challenge_InsertAll(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_WeekDay,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_MatPtMin,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_StPtMin,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_GCash,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_Sence,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_IsBattle,0)).lpdata)
				);
			break;
		default:
			SDOInfo.Destroy();
			return FALSE;
		}

		if(!DBTFLV.empty())
		{
			operSDO.BuildTLV(DBTFLV,m_ServiceKey,(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::SDO_ServerIP,0)).lpdata);
		}
		else
		{
			operSDO.BuildTLV("数据库为空");
		}
		operSDO.SendBuf(SDOInfo.pGameSess,CEnumCore::Msg_Category::SDO_ADMIN,m_ServiceKey,operSDO.GetSendBuf(),operSDO.GetSendLength());

		if(!SDOInfo.Destroy())
		{
			return FALSE;
		}
		return TRUE;
	}
	catch (...)
	{
	}

	return FALSE;
}
