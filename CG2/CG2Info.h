#ifndef PALLOGINFO_H_
#define PALLOGINFO_H_

#include "OperatorCG2.h"

class CCG2Info
{
private:
	SOCKET m_hServerSocket;

	//���ڱ����������ݿ�ִ�в�����sql���
	char operszSql[2048];

	//���ڱ�����Զ�����ݿ�ִ�в�����sql���
	char operRemoteSql[2048];

	//���ڱ���ִ�в������û���
	char m_UserName[128];

	//��¼��־��Ϣ
	char strLog[2048];

	//���ݷ��������ƻ�ȡ��Ҫ��IP
	char logdbip[20];

	//����������
	char cg2_serveripname[40];

	//������IP
	char cg2_serverip[40];
	
	//�ǳ�
	char cg2_nickname[40];
	
	//�ʺ�
	char cg2_username[40];

	//���ID
	char cg2_usersn[40];

	//����
	char cg2_guildname[40];

	//����Id
	int cg2_guildid;

	//�û���ɫ��
	int cg2_charno;

	//����ҳ����
	int index;

	//����ҳ���С
	int iPageSize;

	//������
	int userByID;

	//�ļ�λ��
	char filepath[256];

	//��ǰʱ��
	char curTime[128];
	
	//������Ϣ
	char RetMessage[256];

	//��ȡ�����ļ������ص�����
	char retMessage[10][256];

private:

	//���ڱ����ѯ���ݿⷵ�صĽ��
	COperVector DBVect;

	//���ڱ����ѯ���ݿⷵ�صĽ��2
	COperVector DBVect1;

	//���ڱ�������ͻ������ݵõ��Ľ��
	COperVector recvVect;

	//����д��־��Ϣ
	CLogFile logFile;

	//�����м�ĸ�������
	COperatorCG2 operAssist;

public:

	//���캯��
	CCG2Info();

	//��������
	~CCG2Info();

	//��ʼ�����еı���
	bool initialize(CSession * pSess,SOCKET m_Socket, unsigned char * pbuf, int length,CEnumCore::Message_Tag_ID m_message_tag_id);

	//ɾ�����еı���
	bool Destroy();

	//��ɫ��������
	bool CG2_Account_Query();


	//��ɫ��ϸ����
	bool CG2_Account_Detail_Query();

	//��ɫ��������
	bool CG2_Account_Attribute_Query();


	//��ɫ��������
	bool CG2_Account_Guild_Query();

	//��ɫ�ƺ�����
	bool CG2_Account_Title_Query();

	//��ɫ����P�S
	bool CG2_Account_Address_Query();

	//��ɫ�����YӍ
	bool CG2_Account_Skill_Query();

	//ʦ����ϵ��ѯ
	bool CG2_Account_Teach_Query();

	//��־��ѯ
	bool CG2_Log_Query();

	//��ѯְҵ����
	bool CG2_Job_Query();

	//��ѯ�������
	bool CG2_GetSkill_Query();

	//��ѯ��������
	bool CG2_SkillList_Query();

	//ģ����ѯ����
	bool CG2_SearchItem_Query();

	//��ѯ�������
	bool CG2_Get_Item_Query();

	//�������Ʋ�ѯ
	bool CG2_Get_Item_Detail_Query();

	//������е���
	bool CG2_InsertBankItem_Query();

	//�������е�����ӣ����룩
	bool CG2_InsertBankItemALL_Query();
		
	//��ӵ���
	bool CG2_Insert_Item_Query();

	//�޸���ҽ�ɫְҵ
	bool CG2_UpdateJob_Query();

	//�޸ļ��ܵȼ�
	bool CG2_Update_Account_Skill_Query();

	//��ӽ�ɫ����
	bool CG2_Insert_Account_Skill_Query();
	
	//�ʺŵ�½�ǳ���¼
	bool CG2_Use_Query();

	//ǿ���������
	bool CG2_KickAccount_Query();

	//ǿ���������(û�лش�)
	bool CG2_KickAccount_Query_NoAnswer();

	//��ѯ���з�ͣ�ʺ���Ϣ
	bool CG2_BanAccount_Query();
	
	//��ѯ�ض��ʺ���û�б���ͣ
	bool CG2_Account_BANISHMENT_QUERY();

	//��ͣ����ʺ�
	bool CG2_Account_Close();

	//�������ʺ�
	bool CG2_Account_Open();


	//�鿴����
	bool CG2_SearchNote_Query();


	//���͹���
	bool CG2_SendNote_Query();


	//ֹͣ���͹���
	bool CG2_DeleteNote_Query();

	//��ѯ������ҽ�ɫ��Ϣ
	bool CG2_BakAccount_Query();


	//�ָ���ɫ����
	bool CG2_Recovery_Account_Qurey();

	//��Ұ�����
	bool CG2_LoveTree_Query();

	//��һ������
	bool CG2_Marriage_Info_Query();

	//������Ϣ
	bool CG2_SearchGuild_Query();

	//�����Ա��Ϣ(����)
	bool CG2_SearchGuild_Detail_Query();

	//��ɢ����
	bool CG2_CloseGuild_Query();
		
	//�޸Ĺ�������
	bool CG2_RenameGuild_Query();

	//����ȼ��޸�
	bool CG2_UpdateGuildLv_Query();

	//���������޸�
	bool CG2_UpdateGuildHonor_Query();

	//��ѯ����������Ϣ
	bool CG2_PackItem_Query();

	//ɾ����ɫ����������
	bool CG2_BagItem_Del_Query();	

	//��ɫ��ʱ������Ϣ
	bool CG2_TempItem_Query();

	//ɾ����ʱ����������
	bool CG2_TmpItem_Del_Query();

	//����װ����Ϣ
	bool CG2_CharItem_Query();

	//ɾ������װ������
	bool CG2_EquipItem_Del_Query();
	
	//�շѵ��߲�ѯ
	bool CG2_ShopItem_Query();

	//�����������
	bool CG2_PetInfo_Query();

	//���ﱳ����Ϣ
	bool CG2_PetItem_Query();

	//���＼����Ϣ
	bool CG2_PetSkill_Query();

	//���д����Ϣ
	bool CG2_BankInfo_Query();

	//�������н�Ǯ
	bool CG2_SetBankMoney_Query();
	
	//������Ʒ��Ϣ
	bool CG2_BankItem_Query();

	//ɾ�����е���
	bool CG2_BankItem_Del_Query();

	//���г�����Ϣ
	bool CG2_BankPet_Query();

	//�޸ĳ�������
	bool CG2_Update_Pet_Query();

	//��ӳ���
	bool CG2_AddPet_Query();

	//�޸ĳ���ȼ�
	bool CG2_UpdatePetLevel_Query();

	//����ͼ����Ϣ
	bool CG2_PetPic_Query();

	//�����б�
	bool CG2_PetList_Query();

	//��ӳ��＼��
	bool CG2_Insert_Pet_Skill_Query();

	//��ӳ��＼��
	bool CG2_Update_Pet_Skill_Query();

	//����/ɾ������ͼ��
	bool CG2_PetPic_Del_Query();

	//�����ʱ����
	bool CG2_Insert_TmpPwd_Query();

	//�鿴��ʱ����
	bool CG2_SearchPassWord_Query();

	//�ָ���ʱ����
	bool CG2_Recover_TmpPwd_Query();

	//��ɫ��λ
	bool CG2_ChangeCharNo_Query();

	//�ƶ���ɫ����
	bool CG2_ChangeMap_Query();

	//��ѯ��ͼ�б�
	bool CG2_MapList_Query();

	//ģ����ѯ��ͼ
	bool CG2_SearchMap_Query();

	//�����ʴ���Ϣ
	bool CG2_SendAsk_Query();

	//�̳���Ϣ��ѯ
	bool CG2_UserShoppingInfo_Query();

	//���͵���������
	bool CG2_Send_ItemPassword();

	//�ȵ�Email��ַ
	bool CG2_Get_EmailAddress();

	////�˺ż����ѯ
	bool CG2_Account_Active_Query();

	//ͨ���������ѯ
	bool CG2_Sn_Active_Query();


	int CG2_GMInsert_Query(char *ServerIP, char *UserName,char * UserNick,char * strEndTime,char * Reason,char * memo,int iFunc);


};
#endif