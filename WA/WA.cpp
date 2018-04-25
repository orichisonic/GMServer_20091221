// WA.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "WA.h"
#include <iostream>
using namespace std;
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
// CWAApp

BEGIN_MESSAGE_MAP(CWAApp, CWinApp)
	//{{AFX_MSG_MAP(CWAApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWAApp construction

CWAApp::CWAApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CWAApp object
extern "C"  bool __declspec(dllexport) WA_Main(CSession * pSess,SOCKET m_socket,CEnumCore::Msg_Category m_category,CEnumCore::Message_Tag_ID m_message_tag,unsigned char * pbuf,int length)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CLogFile logFile;
	
	try
	{
		CWAApp theApp;
		CWAInfo WAInfo;
		COperatorWA operWA;
		vector <CGlobalStruct::TFLV> DBTFLV;
		CEnumCore::ServiceKey m_ServiceKey= (m_message_tag & 0x00FFFF) | 0x8000;//获取要回应的CEnumCore::ServiceKey
		vector <CGlobalStruct::TFLV> RECTFLV=WAInfo.initialize(pSess, pbuf, length);

		if(RECTFLV.empty())//初始化类对象
		{
			operWA.BuildTLV("远程命令未空");
			operWA.SendBuf(WAInfo.pGameSess,CEnumCore::Msg_Category::WA_ADMIN,m_ServiceKey,operWA.GetSendBuf(),operWA.GetSendLength());
			return TRUE;
		}

		switch((unsigned int)m_message_tag)
		{
		//角色信息查询(帐号、角色)
		case CEnumCore::Message_Tag_ID::WA_Account_Query:

			DBTFLV=WAInfo.WA_Net_UserInfo(
					(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
					(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
					(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
					StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
					StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));

			break;

		//踢人(帐号、角色)
		case CEnumCore::Message_Tag_ID::WA_KickPlayer:

			DBTFLV=WAInfo.WA_Net_KickPlayer(StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
					(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
					(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
					(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata);
				
			break;

		//帐号封停(帐号、角色)
		case CEnumCore::Message_Tag_ID::WA_ClosePlayer:

				DBTFLV=WAInfo.WA_Net_ClosePlayer(
					StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
					(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
					(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
					StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_NickID,0)).lpdata),
					(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
					StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Time,0)).lpdata),
					StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Reason,0)).lpdata),
					(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Description,0)).lpdata);
						
				break;
		//批量帐号封停
		case CEnumCore::Message_Tag_ID::WA_ClosePlayerGroup:
			cout<<"WA_ClosePlayerGroup-------"<<endl;
			DBTFLV=WAInfo.WA_Net_ClosePlayerGroup(
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_NickID,0)).lpdata),
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Time,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Reason,0)).lpdata),
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Description,0)).lpdata);
			break;
		//帐号解封(帐号、角色)
		case CEnumCore::Message_Tag_ID::WA_OpenPlayer:
			cout<<"<<<<<<<<<<<<<<WA_OpenPlayer"<<endl;
			DBTFLV=WAInfo.WA_Net_OpenPlayer(
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_NickID,0)).lpdata),
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata);
						
			break;

		//封停列表查询
		case CEnumCore::Message_Tag_ID::WA_CloseList:

			DBTFLV=WAInfo.WA_Net_CloseList(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerName,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
					
			break;

		//Added by tengjie 2010-05-14:单个帐号封停查询(帐号)
		case CEnumCore::Message_Tag_ID::WA_CloseSingle:
			
			DBTFLV=WAInfo.WA_Net_CloseSingle(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			
				break;

		//添加公告
		case CEnumCore::Message_Tag_ID::WA_BoardTasker_Add:

			DBTFLV=WAInfo.WA_BoardTasker_Add(StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BoardMessage,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((LPCSTR)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Interval,0)).lpdata));				

			break;

			//修改公告
		case CEnumCore::Message_Tag_ID::WA_BoardTasker_Update:

			DBTFLV=WAInfo.WA_BoardTasker_Update(StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				StrToInt((LPCSTR)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_TaskID,0)).lpdata),
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BoardMessage,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((LPCSTR)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Interval,0)).lpdata));

			break;

		//删除公告
		case CEnumCore::Message_Tag_ID::WA_BoardTasker_Del:

			DBTFLV=WAInfo.WA_BoardTasker_Delete(StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				StrToInt((LPCSTR)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_TaskID,0)).lpdata));				
			
			break;

		//查询公告		
		case CEnumCore::Message_Tag_ID::WA_BoardTasker_Query:

			DBTFLV=WAInfo.BroadTask_Query("WA","exec WA_BOARDTASK_QUERY",
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;
		//道具类型查询		
		case CEnumCore::Message_Tag_ID::WA_ItemType_Query:
			DBTFLV=WAInfo.WA_ITEMType_QUERY("WA","exec WA_ItemType_Query",0,0);
			break;
		//道具查询		
		case CEnumCore::Message_Tag_ID::WA_ItemList_Query:
			
			DBTFLV=WAInfo.WA_ITEM_QUERY("WA","exec WA_Item_Query '%s'",
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ItemType,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_itemName,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;
		//技能查询		
		case CEnumCore::Message_Tag_ID::WA_SkillList_Query:
			DBTFLV=WAInfo.WA_SkillList_QUERY("WA","exec WA_SkillList_Query '%s'",
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_SkillName,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;
		//角色身上技能查询		
// 		case CEnumCore::Message_Tag_ID::WA_RoleSkill_Query:
// 			DBTFLV=WAInfo.WA_RoleSkill_Query(
// 				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
// 				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerName,0)).lpdata,
// 				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
// 				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
// 				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
// 				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
// 			break;
		//修改玩家技能等级		
		case CEnumCore::Message_Tag_ID::WA_ModifyCharacterSkill:
				DBTFLV=WAInfo.WA_ModfiyPlayerSkill(StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,1,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,				
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_SkillID,0)).lpdata),"",
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_SkillLvl,0)).lpdata));
			break;
		//添加玩家道具
// 		case CEnumCore::Message_Tag_ID::WA_MailItem:
// 			DBTFLV=WAInfo.WA_SendMailItem(StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
// 				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
// 				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,1,
// 				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
// 				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ItemID,0)).lpdata),
// 				1,
// 				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_itemName,0)).lpdata);
// 			break;

		case CEnumCore::Message_Tag_ID::WA_MailItem:
			DBTFLV=WAInfo.WA_SendMailItem(StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,1,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ItemID,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ItemNum,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_itemName,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_MailTag,0)).lpdata),
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ItemGen,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_MailSubject,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_MailContent,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_SendMoney,0)).lpdata),
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ItemExpire,0)).lpdata);
			break;

		//恢复角色		
		case CEnumCore::Message_Tag_ID::WA_RecoverCharacter:
			DBTFLV=WAInfo.WA_RestoreRoleInfo(StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,1,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata
				);
			break;
		//玩家密码查询	
		case CEnumCore::Message_Tag_ID::WA_UserPwd_Recover:
			
			DBTFLV=WAInfo.WA_UserPwd_QUERY("WA",(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerName,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				0,
				0);
			break;
		//修改玩家登录密码	
		case CEnumCore::Message_Tag_ID::WA_ModifyAccountPassw:
			
			DBTFLV=WAInfo.WA_ChangeLoginPasswd(StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				1,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_NewPasswd,0)).lpdata);
			break;

		//Added by tengjie 2010-06-30
		//恢复玩家密码	
		case CEnumCore::Message_Tag_ID::WA_Query_ResumePassw:
			DBTFLV=WAInfo.WA_ResumePassw(
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata);
			break;

		//好友信息查询
		case CEnumCore::Message_Tag_ID::WA_FriendInfo_Query:

			DBTFLV=WAInfo.WA_Net_FriendInfo(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerName,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;
			
			//修改角色名
		case CEnumCore::Message_Tag_ID::WA_PlayerRole_Modfiy:
			DBTFLV=WAInfo.WA_RoleName_Modfiy(
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_NewRole,0)).lpdata);
			break;

		//角色物品信息查询(身上、背包、银行)
		case CEnumCore::Message_Tag_ID::WA_BodyItemInfo_Query:
		case CEnumCore::Message_Tag_ID::WA_BagItemInfo_Query:
		case CEnumCore::Message_Tag_ID::WA_BankItemInfo_Query:

			DBTFLV=WAInfo.WA_Net_ItemInfo(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerName,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ItemPos,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			
			break;

		//徽章信息查询
		case CEnumCore::Message_Tag_ID::WA_EmblemInfo_Query:

			DBTFLV=WAInfo.WA_Net_EmblemInfo(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerName,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));

			break;

		//宠物信息查询
		case CEnumCore::Message_Tag_ID::WA_PetInfo_Query:

			DBTFLV=WAInfo.WA_Net_PetInfo(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerName,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_medalPetIndex,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));

			break;

		//道具删除
		case CEnumCore::Message_Tag_ID::WA_Item_Del:

			DBTFLV=WAInfo.WA_Net_DeleteItem(
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_NickID,0)).lpdata),
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ItemID,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ItemPos,0)).lpdata));

			break;
		//修改角色属性值
		case CEnumCore::Message_Tag_ID::WA_RoleInfo_Modfiy:
			
			DBTFLV=WAInfo.WA_Net_ModifyLevel(
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_NickID,0)).lpdata),
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Level,0)).lpdata));
			
			break;
		//金钱修改
		case CEnumCore::Message_Tag_ID::WA_Money_Update:

			DBTFLV=WAInfo.WA_Net_ModifyMoney(
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_NickID,0)).lpdata),
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_CurMoney,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Money,0)).lpdata));

			break;

		//邮箱信息查询
		case CEnumCore::Message_Tag_ID::WA_MailInfo_Query:
				break;
			
		//============================================================LOG=============================================================
		// 物品:怪物掉落
		case CEnumCore::Message_Tag_ID::WA_Query_MonsterDropItem:
			DBTFLV=WAInfo.WA_Log_MonsterDropItem(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerName,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_NickID,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		// 物品:从NPC处购买
		case CEnumCore::Message_Tag_ID::WA_Query_BuyItemNPC:
			DBTFLV=WAInfo.WA_Log_BuyItemNPC(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerName,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_NickID,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));

			break;

		// 物品:卖给NPC
		case CEnumCore::Message_Tag_ID::WA_Query_SellItemTONPC:
			DBTFLV=WAInfo.WA_Log_SellItemTONPC(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerName,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_NickID,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));

			break;

		// 物品:玩家自行删除
		case CEnumCore::Message_Tag_ID::WA_Query_DropItem:
			DBTFLV=WAInfo.WA_Log_DropItem(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerName,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_NickID,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));

			break;

		// 物品:交易获得
		case CEnumCore::Message_Tag_ID::WA_Query_GetItemTrade:
			DBTFLV=WAInfo.WA_Log_GetItemTrade(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerName,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_NickID,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));

			break;

		// 物品:交易失去
		case CEnumCore::Message_Tag_ID::WA_Query_LoseItemTrade:
			DBTFLV=WAInfo.WA_Log_LoseItemTrade(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerName,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_NickID,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));

			break;

		// 物品:装备绑定记录
		case CEnumCore::Message_Tag_ID::WA_Query_BindItem:
			DBTFLV=WAInfo.WA_Log_BindItem(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerName,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_NickID,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));

			break;

		// 物品:装备解绑定记录
		case CEnumCore::Message_Tag_ID::WA_Query_UnBindItem:
			DBTFLV=WAInfo.WA_Log_UnBindItem(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerName,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_NickID,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));

			break;

		// 物品:角色“摆摊”日志查询
		case CEnumCore::Message_Tag_ID::WA_Query_BoothItem:
			DBTFLV=WAInfo.WA_Log_BoothItem(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerName,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_NickID,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));

			break;
		// 物品:道具删除记录(指使用时间到了)
		case CEnumCore::Message_Tag_ID::WA_Query_DeleteItem:
			DBTFLV=WAInfo.WA_Log_DeleteItem(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerName,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_NickID,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));

			break;

		// 物品:道具使用记录
		case CEnumCore::Message_Tag_ID::WA_Query_UseItem:
			DBTFLV=WAInfo.WA_Log_UseItem(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerName,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_NickID,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			
			break;

		// 物品:装备制作记录
		case CEnumCore::Message_Tag_ID::WA_Query_CraftItem:
			DBTFLV=WAInfo.WA_Log_CraftItem(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerName,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_NickID,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			
			break;

		// 物品:装备打孔
		case CEnumCore::Message_Tag_ID::WA_Query_PunchItem:
			DBTFLV=WAInfo.WA_Log_PunchItem(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerName,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_NickID,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			
			break;

		// 物品:装备改孔
		case CEnumCore::Message_Tag_ID::WA_Query_ChangeSlotItem:
			DBTFLV=WAInfo.WA_Log_ChangeSlotItem(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerName,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_NickID,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			
			break;

		// 物品:装备升星
		case CEnumCore::Message_Tag_ID::WA_Query_UpgradeStar:
			DBTFLV=WAInfo.WA_Log_UpgradeStar(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerName,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_NickID,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			
			break;

		// 物品:宝石
		case CEnumCore::Message_Tag_ID::WA_Query_Gem:
			DBTFLV=WAInfo.WA_Log_Gem(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerName,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_NickID,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			
			break;

		// 物品:制药记录
		case CEnumCore::Message_Tag_ID::WA_Query_Pharma:
			DBTFLV=WAInfo.WA_Log_Pharma(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerName,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_NickID,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			
			break;

		// 物品:宝石合成
		case CEnumCore::Message_Tag_ID::WA_Query_CraftGem:
			DBTFLV=WAInfo.WA_Log_CraftGem(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerName,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_NickID,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			
			break;

		// 物品:材料合成
		case CEnumCore::Message_Tag_ID::WA_Query_CraftMatirail:
			DBTFLV=WAInfo.WA_Log_CraftMatirail(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerName,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_NickID,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			
			break;

		// 物品:图纸合成
		case CEnumCore::Message_Tag_ID::WA_Query_CraftRecipe:
			DBTFLV=WAInfo.WA_Log_CraftRecipe(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerName,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_NickID,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			
			break;

		// 迷宫积分兑换奖励
		case CEnumCore::Message_Tag_ID::WA_Query_MazeIntegral:
			DBTFLV=WAInfo.WA_Log_MazeIntegral(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerName,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_NickID,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			
			break;

		// 游戏币:怪物掉落
		case CEnumCore::Message_Tag_ID::WA_Query_MonsterDropMoney:
			DBTFLV=WAInfo.WA_Log_MonsterDropMoney(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerName,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_NickID,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			
			break;

		// 游戏币:任务获得
		case CEnumCore::Message_Tag_ID::WA_Query_GetMoneyQuest:
			DBTFLV=WAInfo.WA_Log_GetMoneyQuest(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerName,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_NickID,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			
			break;

		// 游戏币:修装
		case CEnumCore::Message_Tag_ID::WA_Query_LoseMoneyRepair:
			DBTFLV=WAInfo.WA_Log_LoseMoneyRepair(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerName,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_NickID,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			
			break;

		// 游戏币:交易获得
		case CEnumCore::Message_Tag_ID::WA_Query_GetMoneyTrade:
			DBTFLV=WAInfo.WA_Log_GetMoneyTrade(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerName,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_NickID,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			
			break;

		// 游戏币:交易失去
		case CEnumCore::Message_Tag_ID::WA_Query_LoseMoneyTrade:
			DBTFLV=WAInfo.WA_Log_LoseMoneyTrade(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerName,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_NickID,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			
			break;

		// 任务:完成获得记录
		case CEnumCore::Message_Tag_ID::WA_Query_QuestReward:
			DBTFLV=WAInfo.WA_Log_QuestReward(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerName,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_NickID,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			
			break;

		// 任务:任务物品删除记录
		case CEnumCore::Message_Tag_ID::WA_Query_QuestGive:
			DBTFLV=WAInfo.WA_Log_QuestGive(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerName,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_NickID,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			
			break;

		// 邮件:发送记录
		case CEnumCore::Message_Tag_ID::WA_Query_MailSend:
			DBTFLV=WAInfo.WA_Log_MailSend(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerName,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_NickID,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			
			break;

		// 邮件:接收记录
		case CEnumCore::Message_Tag_ID::WA_Query_MailRecv:
			DBTFLV=WAInfo.WA_Log_MailRecv(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerName,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_NickID,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			
			break;

		// 邮件:删除记录
		case CEnumCore::Message_Tag_ID::WA_Query_MailDelete:
			DBTFLV=WAInfo.WA_Log_MailDelete(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerName,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_NickID,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			
			break;

		// 商城:购买记录
		case CEnumCore::Message_Tag_ID::WA_Query_ItemShop:
			DBTFLV=WAInfo.WA_Log_ItemShop(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerName,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_NickID,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			
			break;

		// 查询用户的副本日志
		case CEnumCore::Message_Tag_ID::WA_Query_Instance:
			DBTFLV=WAInfo.WA_Log_Instance(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerName,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_NickID,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			
			break;

		// 玩家登陆/登出日志
		case CEnumCore::Message_Tag_ID::WA_Query_LoginLogout:
			DBTFLV=WAInfo.WA_Log_LoginLogout(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerName,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_NickID,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//Added by tengjie 2010-06-11
		//角色日志:角色升级记录
		case CEnumCore::Message_Tag_ID::WA_Character_Upgrade:
			DBTFLV=WAInfo.WA_Character_Upgrade_Query(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//角色日志:角色创建记录查询
		case CEnumCore::Message_Tag_ID::WA_Character_Create:
			DBTFLV=WAInfo.WA_Character_Create_Query(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//角色日志:角色删除记录查询
		case CEnumCore::Message_Tag_ID::WA_Character_Delete:
			DBTFLV=WAInfo.WA_Character_Delete_Query(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//角色日志:加入帮会
		case CEnumCore::Message_Tag_ID::WA_Join_Guild:
			DBTFLV=WAInfo.WA_Join_Guild_Query(	
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//角色日志:离开帮会
		case CEnumCore::Message_Tag_ID::WA_Quit_Guild:

			DBTFLV=WAInfo.WA_Quit_Guild_Query(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//角色日志:创建帮会
		case CEnumCore::Message_Tag_ID::WA_Create_Guild:
			DBTFLV=WAInfo.WA_Create_Guild_Query(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//角色日志:解散帮会
		case CEnumCore::Message_Tag_ID::WA_Dissolve_Guild:
			DBTFLV=WAInfo.WA_Dissolve_Guild_Query(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//角色日志:转职信息查询
		case CEnumCore::Message_Tag_ID::WA_Transfer_Info:
			DBTFLV=WAInfo.WA_Transfer_Info_Query(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//角色日志:技能日志记录
		case CEnumCore::Message_Tag_ID::WA_Skills_Log:
			DBTFLV=WAInfo.WA_Skills_Log_Query(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//角色日志:生活技能日志记录
		case CEnumCore::Message_Tag_ID::WA_LifeSkills_Log:
			DBTFLV=WAInfo.WA_LifeSkills_Log_Query(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//佣兵团日志:创建记录
		case CEnumCore::Message_Tag_ID::WA_Create_MercenaryGroup:
			DBTFLV=WAInfo.WA_CreateMercenary_Log_Query(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_GuildName,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//佣兵团日志:解散记录
		case CEnumCore::Message_Tag_ID::WA_Dissolve_MercenaryGroup:
			DBTFLV=WAInfo.WA_DissolveMercenary_Log_Query(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_GuildName,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//宠物徽章日志:宠物精魄捕捉记录
		case CEnumCore::Message_Tag_ID::WA_CatchPetSpirit_Record:
			DBTFLV=WAInfo.WA_CatchPetSpirit_Log_Query(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//宠物徽章日志
		//精魄交易记录:交易获得
		case CEnumCore::Message_Tag_ID::WA_GainSpirit_Record:
			DBTFLV=WAInfo.WA_GainSpirit_Log_Query(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//宠物徽章日志
		//精魄交易记录:交易失去
		case CEnumCore::Message_Tag_ID::WA_LoseSpirit_Record:
			DBTFLV=WAInfo.WA_LoseSpirit_Log_Query(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//宠物徽章日志:精魄出售记录
		case CEnumCore::Message_Tag_ID::WA_SellSpirit_Record:
			DBTFLV=WAInfo.WA_SellSpirit_Log_Query(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//用户信息查询:技能查询
		case CEnumCore::Message_Tag_ID::WA_SkillsInfo_Query:
			DBTFLV=WAInfo.WA_SkillsInfo_Log_Query(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;
			
		//用户信息查询:任务查询
		case CEnumCore::Message_Tag_ID::WA_MissionsCompleted_Query:
			DBTFLV=WAInfo.WA_MissionsCompleted_Log_Query(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;
		
		//佣兵团信息查询
		//佣兵团信息查询
		case CEnumCore::Message_Tag_ID::WA_MercenaryGroup_Info_Query:
			DBTFLV=WAInfo.WA_MercenaryGroup_Query(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_GuildName,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;
				
		//佣兵团成员列表查询
		case CEnumCore::Message_Tag_ID::WA_MemberList_Query:
			DBTFLV=WAInfo.WA_MemberList_Info_Query(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_GuildName,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//操作类:
		//宠物管理:宠物列表查询
		case CEnumCore::Message_Tag_ID::WA_PetList_Query:
			DBTFLV=WAInfo.WA_PET_QUERY("WA","exec WA_PetList_Query",0,0);
			break;
			
		//宠物管理:添加宠物(等级\属性)
		case CEnumCore::Message_Tag_ID::WA_PetAdded_Operate:
			DBTFLV=WAInfo.WA_PetAdded_Operater(
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_petTypeId,0)).lpdata));
			break;
			
		//宠物管理:删除宠物(等级\属性)
		case CEnumCore::Message_Tag_ID::WA_PetDeleted_Operate:
			DBTFLV=WAInfo.WA_PetDeleted_Operater(
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_petTypeId,0)).lpdata));
			break;

		//家族管理:转让会长权限
		case CEnumCore::Message_Tag_ID::WA_AttornPopedom_Operate:
			DBTFLV=WAInfo.WA_AttornPopedom_Operater(
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_GuildName,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata);
			break;

		//公会管理:修改公会名称
		case CEnumCore::Message_Tag_ID::WA_ModifyGuildName_Operate:
			DBTFLV=WAInfo.WA_ModifyGuildName_Operater(
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_GuildName,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_NewGuildName,0)).lpdata);
			break;

		//公会管理:踢出公会成员
		case CEnumCore::Message_Tag_ID::WA_KickGuildMember_Operate:
			DBTFLV=WAInfo.WA_KickGuildMember_Operater(
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_GuildName,0)).lpdata);
			break;

		//公会管理:解散公会
		case CEnumCore::Message_Tag_ID::WA_DissolveGuild_Operate:
			DBTFLV=WAInfo.WA_DissolveGuild_Operater(
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_GuildName,0)).lpdata);
			break;

		// 玩家登陆/登出日志(按ip方式查询)
		case CEnumCore::Message_Tag_ID::WA_Query_LoginLogoutByIP:
			DBTFLV=WAInfo.WA_Log_LoginLogoutByIP(
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_BeginTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_EndTime,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_LoginIP,0)).lpdata,
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//玩家角色属性恢复		
		case CEnumCore::Message_Tag_ID::WA_ResumeAttribute_Op:
			DBTFLV=WAInfo.WA_ResumeAttribute_Operate(StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata);
			break;

		//批量发送邮件(无道具)
		case CEnumCore::Message_Tag_ID::WA_SendMails_Op:
			DBTFLV=WAInfo.WA_SendMails_Operate(StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_MailSubject,0)).lpdata,
				(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_MailContent,0)).lpdata);
			break;
		//End

		default:
			break;
			
		}

		if(!DBTFLV.empty())
		{
			operWA.BuildTLV(DBTFLV,m_ServiceKey,(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata);
#ifdef _DEBUG
			vector <CGlobalStruct::TFLV>::iterator iter;
			int tflvSize = DBTFLV.size();
			for(iter=DBTFLV.begin(); iter!=DBTFLV.end(); iter++)
			{
				
				printf("%x %x %s\n", (*iter).m_tagName, (*iter).m_tagFormat, (*iter).lpdata);
			}
#endif
		}
		else
		{
			operWA.BuildTLV("数据库为空");
		}
		if (m_ServiceKey == CEnumCore::ServiceKey::WA_ClosePlayerGroup_Resp)
		{
			printf("发送批量封停服务器回应消息\n");
			operWA.SendBuf(WAInfo.pGameSess,CEnumCore::Msg_Category::COMMON,m_ServiceKey,operWA.GetSendBuf(),operWA.GetSendLength());
		}
		else
			operWA.SendBuf(WAInfo.pGameSess,CEnumCore::Msg_Category::WA_ADMIN,m_ServiceKey,operWA.GetSendBuf(),operWA.GetSendLength());

		//销毁内存
		if(!WAInfo.Destroy())
		{
			return false;
		}
	}
	catch(...)
	{
		return false;
	}

	return true;
}

extern CWAApp theApp;
