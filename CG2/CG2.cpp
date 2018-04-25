// CG2.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "CG2.h"

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
// CCG2App

BEGIN_MESSAGE_MAP(CCG2App, CWinApp)
	//{{AFX_MSG_MAP(CCG2App)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCG2App construction

CCG2App::CCG2App()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCG2App object

extern "C"  bool __declspec(dllexport) CG2_Main(CSession * pSess,SOCKET m_socket,CEnumCore::Msg_Category m_category,CEnumCore::Message_Tag_ID m_message_tag,unsigned char * pbuf,int length)
{
	CCG2Info CG2Info;
/*	CSocketData socketdata;
	CEnumCore::Msg_Category m_category;
	CEnumCore::Message_Tag_ID m_message_tag;
	
	
	socketdata.Create(length);
	bool b_result=socketdata.DeHead(pbuf,length);//解析接收到的数据包
	if(!b_result)
		return false;
	m_category=socketdata.GetPacketHead().m_msg_category;
	if(m_category!=CEnumCore::Msg_Category::CG2_ADMIN)
	{
		goto FALSE_FUNC;
	}
	m_message_tag=socketdata.GetMessageTagID();*/
	
	if(!CG2Info.initialize(pSess,m_socket, pbuf,length,m_message_tag))//初始化类对象
	{
		return false;
	}
	switch((unsigned int)m_message_tag)
	{
	case CEnumCore::Message_Tag_ID::CG2_Account_Query://角色基本资料
		CG2Info.CG2_Account_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Account_Detail_Query://角色详细资料
		CG2Info.CG2_Account_Detail_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Account_Attribute_Query://角色属性资料
		CG2Info.CG2_Account_Attribute_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Account_Guild_Query://角色公会资料
		CG2Info.CG2_Account_Guild_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Account_Title_Query://角色称号资料
		CG2Info.CG2_Account_Title_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Account_Address_Query://角色社會關係
		CG2Info.CG2_Account_Address_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Account_Skill_Query://角色技能資訊
		CG2Info.CG2_Account_Skill_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Account_Teach_Query://师生关系查询
		CG2Info.CG2_Account_Teach_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Log_Query:///////////////日志查询
		CG2Info.CG2_Log_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Job_Query://查询职业种类CG2_Job_Query
		CG2Info.CG2_Job_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_GetSkill_Query:	//查询技能类别//maple add
		CG2Info.CG2_GetSkill_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_SkillList_Query:	//查询技能名称//maple add
		CG2Info.CG2_SkillList_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_SearchItem_Query://模糊查询道具
		CG2Info.CG2_SearchItem_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Get_Item_Query:	//查询道具类别////maple add
		CG2Info.CG2_Get_Item_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Get_Item_Detail_Query://道具名称查询////maple add
		CG2Info.CG2_Get_Item_Detail_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Insert_Item_Query://添加道具///maple add
		CG2Info.CG2_Insert_Item_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_InsertBankItem_Query://添加银行道具
		CG2Info.CG2_InsertBankItem_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_InsertBankItemALL_Query://批量银行道具添加（导入）
		CG2Info.CG2_InsertBankItemALL_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_UpdateJob_Query:///修改玩家角色职业
		CG2Info.CG2_UpdateJob_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Update_Account_Skill_Query://修改技能等级////maple add
		CG2Info.CG2_Update_Account_Skill_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Insert_Account_Skill_Query://添加角色技能///maple add
		CG2Info.CG2_Insert_Account_Skill_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Use_Query://帐号登陆登出记录
		CG2Info.CG2_Use_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_KickAccount_Query://强制玩家下线
		CG2Info.CG2_KickAccount_Query ();
		break;
	case CEnumCore::Message_Tag_ID::CG2_BanAccount_Query://查询所有封停帐号信息
		CG2Info.CG2_BanAccount_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Account_BANISHMENT_QUERY://查询特定帐号有没有被封停
		CG2Info.CG2_Account_BANISHMENT_QUERY();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Account_Close://封停玩家帐号
		CG2Info.CG2_Account_Close();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Account_Open://解封玩家帐号
		CG2Info.CG2_Account_Open();
		break;
	case CEnumCore::Message_Tag_ID::CG2_SearchNote_Query://查看公告
		CG2Info.CG2_SearchNote_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_SendNote_Query://发送公告
		CG2Info.CG2_SendNote_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_DeleteNote_Query://停止发送公告
		CG2Info.CG2_DeleteNote_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_BakAccount_Query://查询备份玩家角色信息
		CG2Info.CG2_BakAccount_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Recovery_Account_Qurey://恢复玩家
		CG2Info.CG2_Recovery_Account_Qurey();
		break;
	case CEnumCore::Message_Tag_ID::CG2_LoveTree_Query://查询玩家爱情树
		CG2Info.CG2_LoveTree_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Marriage_Info_Query://查询玩家爱情树
		CG2Info.CG2_Marriage_Info_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_SearchGuild_Query://公会信息查询
		CG2Info.CG2_SearchGuild_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_SearchGuild_Detail_Query://公会成员信息
		CG2Info.CG2_SearchGuild_Detail_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_RenameGuild_Query://修改公会名字
		CG2Info.CG2_RenameGuild_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_UpdateGuildLv_Query://修改公会等级
		CG2Info.CG2_UpdateGuildLv_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_UpdateGuildHonor_Query://修改公会荣誉
		CG2Info.CG2_UpdateGuildHonor_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_PackItem_Query://查询背包道具信息
		CG2Info.CG2_PackItem_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_BagItem_Del_Query://删除角色道具栏道具
		CG2Info.CG2_BagItem_Del_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_TempItem_Query://角色临时背包信息
		CG2Info.CG2_TempItem_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_TmpItem_Del_Query://删除临时道具栏道具
		CG2Info.CG2_TmpItem_Del_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_CharItem_Query://身上装备信息
		CG2Info.CG2_CharItem_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_EquipItem_Del_Query://删除随身装备道具
		CG2Info.CG2_EquipItem_Del_Query();
		break;

	case CEnumCore::Message_Tag_ID::CG2_ShopItem_Query://收费道具查询
		CG2Info.CG2_ShopItem_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_PetInfo_Query://宠物基本资料
		CG2Info.CG2_PetInfo_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_PetSkill_Query://宠物技能信息
		CG2Info.CG2_PetSkill_Query();
		break;	
	case CEnumCore::Message_Tag_ID::CG2_BankInfo_Query://银行存款信息
		CG2Info.CG2_BankInfo_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_SetBankMoney_Query://设置银行金钱
		CG2Info.CG2_SetBankMoney_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_BankItem_Query://银行物品信息
		CG2Info.CG2_BankItem_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_BankItem_Del_Query://删除银行道具
		CG2Info.CG2_BankItem_Del_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_BankPet_Query://银行宠物信息
		CG2Info.CG2_BankPet_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Update_Pet_Query://修改宠物属性
		CG2Info.CG2_Update_Pet_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_AddPet_Query://添加宠物
		CG2Info.CG2_AddPet_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_UpdatePetLevel_Query://修改宠物等级
		CG2Info.CG2_UpdatePetLevel_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_PetList_Query://宠物列表
		CG2Info.CG2_PetList_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Insert_Pet_Skill_Query://添加宠物技能
		CG2Info.CG2_Insert_Pet_Skill_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Update_Pet_Skill_Query://修改宠物技能
		CG2Info.CG2_Update_Pet_Skill_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_PetPic_Del_Query://设置/删除宠物图鉴
		CG2Info.CG2_PetPic_Del_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_PetPic_Query://宠物图鉴信息
		CG2Info.CG2_PetPic_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Insert_TmpPwd_Query://添加临时密码
		CG2Info.CG2_Insert_TmpPwd_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_SearchPassWord_Query://查看临时密码
		CG2Info.CG2_SearchPassWord_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Recover_TmpPwd_Query://恢复临时密码
		CG2Info.CG2_Recover_TmpPwd_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_ChangeCharNo_Query://角色换位
		CG2Info.CG2_ChangeCharNo_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Change_Map_Query://移动角色坐标
		CG2Info.CG2_ChangeMap_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_MapList_Query://查询地图列表
		CG2Info.CG2_MapList_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_SearchMap_Query://模糊查询地图
		CG2Info.CG2_SearchMap_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_SendAsk_Query://发送问答信息
		CG2Info.CG2_SendAsk_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_UserShoppingInfo_Query://商城信息查询
		CG2Info.CG2_UserShoppingInfo_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Send_ItemPassword://发送道具密码锁
		CG2Info.CG2_Send_ItemPassword();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Get_EmailAddress://等到Email地址
		CG2Info.CG2_Get_EmailAddress();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Account_Active_Query://激活账号
		CG2Info.CG2_Account_Active_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Sn_Active_Query://通过激活码
		CG2Info.CG2_Sn_Active_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Account_Ban_Query://
		break;
	case CEnumCore::Message_Tag_ID::CG2_RecoverLogItem_Query://恢复日志道具
		break;
	case CEnumCore::Message_Tag_ID::CG2_RecoverLogPet_Query://恢复日志宠物
		break;
	default:
		goto FALSE_FUNC;
		break;
	}
	if(!CG2Info.Destroy())
	{
		return FALSE;
	}
	return TRUE;
FALSE_FUNC:
	CG2Info.Destroy();
	return FALSE;
}



int CG2_GMInsert_Query(char *ServerIP, char *UserName,char * UserNick,char * strEndTime,char * Reason,char * memo,int iFunc)
{
	CCG2Info cg2Info;
	return cg2Info.CG2_GMInsert_Query(ServerIP,UserName,UserNick,strEndTime,Reason,memo,iFunc);	
}
CCG2App theApp;
