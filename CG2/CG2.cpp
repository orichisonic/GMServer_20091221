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
	bool b_result=socketdata.DeHead(pbuf,length);//�������յ������ݰ�
	if(!b_result)
		return false;
	m_category=socketdata.GetPacketHead().m_msg_category;
	if(m_category!=CEnumCore::Msg_Category::CG2_ADMIN)
	{
		goto FALSE_FUNC;
	}
	m_message_tag=socketdata.GetMessageTagID();*/
	
	if(!CG2Info.initialize(pSess,m_socket, pbuf,length,m_message_tag))//��ʼ�������
	{
		return false;
	}
	switch((unsigned int)m_message_tag)
	{
	case CEnumCore::Message_Tag_ID::CG2_Account_Query://��ɫ��������
		CG2Info.CG2_Account_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Account_Detail_Query://��ɫ��ϸ����
		CG2Info.CG2_Account_Detail_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Account_Attribute_Query://��ɫ��������
		CG2Info.CG2_Account_Attribute_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Account_Guild_Query://��ɫ��������
		CG2Info.CG2_Account_Guild_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Account_Title_Query://��ɫ�ƺ�����
		CG2Info.CG2_Account_Title_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Account_Address_Query://��ɫ����P�S
		CG2Info.CG2_Account_Address_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Account_Skill_Query://��ɫ�����YӍ
		CG2Info.CG2_Account_Skill_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Account_Teach_Query://ʦ����ϵ��ѯ
		CG2Info.CG2_Account_Teach_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Log_Query:///////////////��־��ѯ
		CG2Info.CG2_Log_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Job_Query://��ѯְҵ����CG2_Job_Query
		CG2Info.CG2_Job_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_GetSkill_Query:	//��ѯ�������//maple add
		CG2Info.CG2_GetSkill_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_SkillList_Query:	//��ѯ��������//maple add
		CG2Info.CG2_SkillList_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_SearchItem_Query://ģ����ѯ����
		CG2Info.CG2_SearchItem_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Get_Item_Query:	//��ѯ�������////maple add
		CG2Info.CG2_Get_Item_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Get_Item_Detail_Query://�������Ʋ�ѯ////maple add
		CG2Info.CG2_Get_Item_Detail_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Insert_Item_Query://��ӵ���///maple add
		CG2Info.CG2_Insert_Item_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_InsertBankItem_Query://������е���
		CG2Info.CG2_InsertBankItem_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_InsertBankItemALL_Query://�������е�����ӣ����룩
		CG2Info.CG2_InsertBankItemALL_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_UpdateJob_Query:///�޸���ҽ�ɫְҵ
		CG2Info.CG2_UpdateJob_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Update_Account_Skill_Query://�޸ļ��ܵȼ�////maple add
		CG2Info.CG2_Update_Account_Skill_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Insert_Account_Skill_Query://��ӽ�ɫ����///maple add
		CG2Info.CG2_Insert_Account_Skill_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Use_Query://�ʺŵ�½�ǳ���¼
		CG2Info.CG2_Use_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_KickAccount_Query://ǿ���������
		CG2Info.CG2_KickAccount_Query ();
		break;
	case CEnumCore::Message_Tag_ID::CG2_BanAccount_Query://��ѯ���з�ͣ�ʺ���Ϣ
		CG2Info.CG2_BanAccount_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Account_BANISHMENT_QUERY://��ѯ�ض��ʺ���û�б���ͣ
		CG2Info.CG2_Account_BANISHMENT_QUERY();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Account_Close://��ͣ����ʺ�
		CG2Info.CG2_Account_Close();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Account_Open://�������ʺ�
		CG2Info.CG2_Account_Open();
		break;
	case CEnumCore::Message_Tag_ID::CG2_SearchNote_Query://�鿴����
		CG2Info.CG2_SearchNote_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_SendNote_Query://���͹���
		CG2Info.CG2_SendNote_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_DeleteNote_Query://ֹͣ���͹���
		CG2Info.CG2_DeleteNote_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_BakAccount_Query://��ѯ������ҽ�ɫ��Ϣ
		CG2Info.CG2_BakAccount_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Recovery_Account_Qurey://�ָ����
		CG2Info.CG2_Recovery_Account_Qurey();
		break;
	case CEnumCore::Message_Tag_ID::CG2_LoveTree_Query://��ѯ��Ұ�����
		CG2Info.CG2_LoveTree_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Marriage_Info_Query://��ѯ��Ұ�����
		CG2Info.CG2_Marriage_Info_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_SearchGuild_Query://������Ϣ��ѯ
		CG2Info.CG2_SearchGuild_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_SearchGuild_Detail_Query://�����Ա��Ϣ
		CG2Info.CG2_SearchGuild_Detail_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_RenameGuild_Query://�޸Ĺ�������
		CG2Info.CG2_RenameGuild_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_UpdateGuildLv_Query://�޸Ĺ���ȼ�
		CG2Info.CG2_UpdateGuildLv_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_UpdateGuildHonor_Query://�޸Ĺ�������
		CG2Info.CG2_UpdateGuildHonor_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_PackItem_Query://��ѯ����������Ϣ
		CG2Info.CG2_PackItem_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_BagItem_Del_Query://ɾ����ɫ����������
		CG2Info.CG2_BagItem_Del_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_TempItem_Query://��ɫ��ʱ������Ϣ
		CG2Info.CG2_TempItem_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_TmpItem_Del_Query://ɾ����ʱ����������
		CG2Info.CG2_TmpItem_Del_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_CharItem_Query://����װ����Ϣ
		CG2Info.CG2_CharItem_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_EquipItem_Del_Query://ɾ������װ������
		CG2Info.CG2_EquipItem_Del_Query();
		break;

	case CEnumCore::Message_Tag_ID::CG2_ShopItem_Query://�շѵ��߲�ѯ
		CG2Info.CG2_ShopItem_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_PetInfo_Query://�����������
		CG2Info.CG2_PetInfo_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_PetSkill_Query://���＼����Ϣ
		CG2Info.CG2_PetSkill_Query();
		break;	
	case CEnumCore::Message_Tag_ID::CG2_BankInfo_Query://���д����Ϣ
		CG2Info.CG2_BankInfo_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_SetBankMoney_Query://�������н�Ǯ
		CG2Info.CG2_SetBankMoney_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_BankItem_Query://������Ʒ��Ϣ
		CG2Info.CG2_BankItem_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_BankItem_Del_Query://ɾ�����е���
		CG2Info.CG2_BankItem_Del_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_BankPet_Query://���г�����Ϣ
		CG2Info.CG2_BankPet_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Update_Pet_Query://�޸ĳ�������
		CG2Info.CG2_Update_Pet_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_AddPet_Query://��ӳ���
		CG2Info.CG2_AddPet_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_UpdatePetLevel_Query://�޸ĳ���ȼ�
		CG2Info.CG2_UpdatePetLevel_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_PetList_Query://�����б�
		CG2Info.CG2_PetList_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Insert_Pet_Skill_Query://��ӳ��＼��
		CG2Info.CG2_Insert_Pet_Skill_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Update_Pet_Skill_Query://�޸ĳ��＼��
		CG2Info.CG2_Update_Pet_Skill_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_PetPic_Del_Query://����/ɾ������ͼ��
		CG2Info.CG2_PetPic_Del_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_PetPic_Query://����ͼ����Ϣ
		CG2Info.CG2_PetPic_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Insert_TmpPwd_Query://�����ʱ����
		CG2Info.CG2_Insert_TmpPwd_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_SearchPassWord_Query://�鿴��ʱ����
		CG2Info.CG2_SearchPassWord_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Recover_TmpPwd_Query://�ָ���ʱ����
		CG2Info.CG2_Recover_TmpPwd_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_ChangeCharNo_Query://��ɫ��λ
		CG2Info.CG2_ChangeCharNo_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Change_Map_Query://�ƶ���ɫ����
		CG2Info.CG2_ChangeMap_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_MapList_Query://��ѯ��ͼ�б�
		CG2Info.CG2_MapList_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_SearchMap_Query://ģ����ѯ��ͼ
		CG2Info.CG2_SearchMap_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_SendAsk_Query://�����ʴ���Ϣ
		CG2Info.CG2_SendAsk_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_UserShoppingInfo_Query://�̳���Ϣ��ѯ
		CG2Info.CG2_UserShoppingInfo_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Send_ItemPassword://���͵���������
		CG2Info.CG2_Send_ItemPassword();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Get_EmailAddress://�ȵ�Email��ַ
		CG2Info.CG2_Get_EmailAddress();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Account_Active_Query://�����˺�
		CG2Info.CG2_Account_Active_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Sn_Active_Query://ͨ��������
		CG2Info.CG2_Sn_Active_Query();
		break;
	case CEnumCore::Message_Tag_ID::CG2_Account_Ban_Query://
		break;
	case CEnumCore::Message_Tag_ID::CG2_RecoverLogItem_Query://�ָ���־����
		break;
	case CEnumCore::Message_Tag_ID::CG2_RecoverLogPet_Query://�ָ���־����
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
