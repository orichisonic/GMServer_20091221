#pragma once

#include "OperatorXDJG.h"
#include "..\\GameIOCP\\GameInfo.h"

#include "..\\OperVector\\MyEvent.h"
#pragma comment(lib,"..\\OperVector\\debug\\OperVector.lib")

#include "XDJG_StructInfo.h"

#define HTTPPORT   8082

class CMyEvent;

//Added by tengjie 2010-07-08
//һ������
#define NONE				0x0000
#define POWER				0x0001		//����
#define AGILITY				0x0002		//����
#define STAMINA				0x0003		//����
#define SPIRIT				0x0004		//����
#define MAGIC				0x0005		//����
//��������
#define AP					0x0006		//ŭ��
#define HP					0x0007		//����ֵ
#define MP					0x0008		//ħ��ֵ
#define APMAX				0x0009		//ŭ��
#define HPMAX				0x000a		//�������ֵ
#define MPMAX				0x000b		//ħ�����ֵ
#define ARMORING			0x000c		//����ֵ
#define MOVINGSPEED			0x000d		//�ƶ��ٶ�
#define HANDSPEED			0x000e		//����
#define MAINHANDSPEED		0x000f		//�������������ٶ�
#define SUBHANDSPEED		0x0010		//�������������ٶ�
#define DOUBLEHANDSPEED		0x0011		//˫�����������ٶ�
#define LONGDISHANDSPEED	0x0012		//Զ�����������ٶ�
#define NEARPA				0x0013		//������������
#define SLAVENEARPA			0x0014		//���ֽ�������
#define FARPA				0x0015		//Զ����������
#define SUPERATTACK			0x0016		//���ܹ�����

//���ּ���
#define PHYSICALHITRATE		0x0017		//����������
#define MAGICALHITRATE		0x0018		//����������
#define DODGERATE			0x0019		//������
#define BLOCKRATE			0x001a		//����
#define PARRYINGRATE		0x001b		//�м���
#define PERFECTRATE			0x001c		//����һ����
#define STRENGTHCRITRATE	0x001d		//��������һ����
#define SPIRITCRITRATE		0x001e		//��������һ����
#define CRITRESISTANCERATE	0x001f		//�����ֿ���
#define DAMAGESHORTDISTANCE	0x0020		//���������˺�ֵ
#define DAMAGELONGDISTANCE	0x0021		//Զ�������˺�ֵ
#define DAMAGESPIRITE		0x0022		//�����˺�ֵ
#define DOCURE				0x0023		//����Ч��

//��ϵ����
#define FASTNESSFIRE		0x0024		//���濹��
#define FASTNESSFROST		0x0025		//��˪����
#define FASTNESSTHUNDER		0x0026		//�׵翹��
#define FASTNESSERODE		0x0027		//��ʴ����
#define FASTNESSENERGY		0x0028		//��������
#define FASTNESSSPIRIT		0x0029		//������

//��ϵ�˺�
#define DODAMAGEFIRE		0x002a		//���������˺�(����)
#define DODAMAGEFROST		0x002b		//������˪�˺�(����)
#define DODAMAGETHUNDER		0x002c		//�����׵��˺�(����)
#define DODAMAGEERODE		0x002d		//������ʴ�˺�(΢������)
#define DODAMAGEENERGY		0x002e		//���������˺�(����)
#define DODAMAGESPIRIT		0x002f		//���������˺�(�����)

//��������
#define ONNEARAP			0x0030		//�ܵ����������˺�ֵ
#define ONFARAP				0x0031		//�ܵ�Զ�������˺�ֵ
#define ONCURE				0x0032		//�ܵ�������ֵ

//�ܵ��ĸ�ϵ�˺�
#define ONDAMAGEFIRE		0x0033		//�ܵ������˺�
#define ONDAMAGEFROST		0x0034		//�ܵ���˪�˺�
#define ONDAMAGETHUNDER		0x0035		//�ܵ��׵��˺�
#define ONDAMAGEERODE		0x0036		//�ܵ���ʴ�˺�
#define ONDAMAGEENERGY		0x0037		//�ܵ������˺�
#define ONDAMAGESPIRIT		0x0038		//�ܵ������˺�
#define PRIMARYPA			0x0039		//�����˺�
#define STEALTH				0x003a		//Ǳ������
#define DETECT				0x003b		//��Ǳ����
#define DOCUREPERCENT		0x003c		//������ưٷֱ�
#define DODAMAGEPERCENT		0x003d		//��������˺��ٷֱ�
#define DOMAGICPERCENT		0x003e		//��������˺��ٷֱ�
#define ONCUREPERCENT		0x003f		//��������ưٷֱ�
#define ONDAMAGEPERCENT		0x0040		//����������˺��ٷֱ�
#define ONMAGICPERCENT		0x0041		//����������˺��ٷֱ�

#define SAFEFLAG	0x80
#define ITEMFLAG	0x40
#define STARLEVEL	0x7F

typedef struct _ST_ITEMINFO{
	int iItemId;
	string strItemName;
	string strItemType;
}st_ItemInfo,*pSt_ItemInfo;

typedef vector<st_ItemInfo> Vec_ItemInfo;

typedef struct _ST_SENDITEMINFO{
	int iItemID;
	int iItemNum;
// 	int iPropertyId;
// 	int iPropertyValue;
// 	int iIncreaseNum;
// 	int iIncreaseType;
// 	int iIncreaseValue;
// 	int iIncreaseEndValue;
// 	int iStarsLevel;
// 	int iAssemblyID;
// 	int iSlotNum;
// 	int iEquipSlotID1;
// 	int iEquipSlotID2;
// 	int iEquipSlotID3;
// 	int iQualityEquip;
// 	int iIsBind;
//	string strItemname;
	string strAttrValue;
}St_SendItemInfo, *pSt_SendItemInfo;

typedef vector<St_SendItemInfo> Vec_SendItemInfo;

typedef struct _ST_GUILDITEMINFO{
	int iItemId;
	int iItemNum;
}st_GuildItemInfo,*pSt_GuildItemInfo;

typedef vector<st_GuildItemInfo> Vec_GuildItemInfo;
//End

class  CXDJGInfo : public CGameInfo
{
public:
	CXDJGInfo(void);
	~CXDJGInfo(void);
 public:
	COperVector * initialize(CSession * pSess,CEnumCore::Message_Tag_ID MessageTagID, unsigned char * pbuf, int length);
	
	//
	vector <CGlobalStruct::TFLV> XDJG_Net_UserInfo(char *Account, char * Character, char *ServerIP, int type, int iIndex, int iPageSize);
	bool getUserInfoDataset(vector <CGlobalStruct::TFLV> &DataSetTFLV, vector<string> vectorCol, char *Character, char *ServerIP, int iIndex, int iPageSize, int iCol);
	vector <CGlobalStruct::TFLV> XDJG_Net_KickPlayer(int userByID, char *ServerIP, char * Character, char * Account);
	vector <CGlobalStruct::TFLV> XDJG_Net_ClosePlayer_List(char *ServerIP, int iIndex, int iPageSize);
	//Modified by tengjie 2010-05-17
//	vector <CGlobalStruct::TFLV> XDJG_Net_ClosePlayer(int userByID, char *ServerIP, char *Account, char *AccountID, int iTime,char *Reason);
	vector <CGlobalStruct::TFLV> XDJG_Net_ClosePlayer(int userByID, char *ServerIP, char *Account, char *AccountID, int iTime,char *Reason,int iReasonType);
	CString XDJG_CalEndFreezeTime(int iTime);	
//	vector <CGlobalStruct::TFLV> XDJG_Net_OpenPlayer(int userByID, char *ServerIP, char *Account, char *Reason);
	vector <CGlobalStruct::TFLV> XDJG_Net_OpenPlayer(int userByID, char *ServerIP, char *Account, char *Reason,int iReasonType);

	vector <CGlobalStruct::TFLV> XDJG_Net_BoardTasker_Query(char * GameName,char * m_sql,int iIndex,int iPageSize);
	vector <CGlobalStruct::TFLV> XDJG_Net_BoardTasker_Add(int userByID, char * ServerIP, char *BoardMessage, char *BeginTime, char *EndTime, int interval);
	vector <CGlobalStruct::TFLV> XDJG_Net_BoardTasker_Update(int userByID, char * ServerIP, int Taskid, char *BoardMessage, char *BeginTime, char *EndTime, int interval);
	vector <CGlobalStruct::TFLV> XDJG_Net_BoardTasker_Delete(int userByID, int Taskid);

	//Added by tengjie 2010-07-06
	//��ɫ�������
	vector <CGlobalStruct::TFLV> XDJG_Net_RoleSkillAdd(int userByID, char *ServerIP, char *Character, int SkillID);
	 //��ɫ����ɾ��
	vector <CGlobalStruct::TFLV> XDJG_Net_RoleSkillDel(int userByID, char *ServerIP, char *Character, int SkillID);
	//�������
//	vector <CGlobalStruct::TFLV> XDJG_Net_ItemAdd(int userByID, char *ServerIP, char *Character, char *ItemID, char *ItemNum);
	vector <CGlobalStruct::TFLV> XDJG_Net_ItemAdd(int userByID, char *ServerIP, char *Character, char *SendCharacter, char *mailTitle, char *mailContent, int Money, char *ItemID, char *ItemNum, char *PropertyId, char *PropertyValue, char *IncreaseNum, 
												  char *IncreaseType, char *IncreaseValue, char *IncreaseEndValue, char *StarsLevel, char *AssemblyID, char *SlotNum, char *EquipSlotID1, char *EquipSlotID2, char *EquipSlotID3, char *QualityEquip, char *IsBind/* ,char *ItemName*/);
	//����ɾ��
	vector <CGlobalStruct::TFLV> XDJG_Net_ItemDel(int userByID, char *ServerIP, char *Character, int ItemID, int ItemNum);
	//��Ϸ�Ҳ�ѯ
	vector <CGlobalStruct::TFLV> XDJG_Net_MoneyQuery(char *ServerIP, char *Account);
	//�����Ϸ��
	vector <CGlobalStruct::TFLV> XDJG_Net_MoneyAdd(int userByID, char *ServerIP, char *Account, int iMoney);
	//��־��:��ҵ�¼��־
	vector <CGlobalStruct::TFLV> XDJG_DB_Login_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, char *QueryIP, int iIndex, int iPageSize);
	//�̳���Ʒ������־
	vector <CGlobalStruct::TFLV> XDJG_DB_Shoping_Log(int userByID, char *ServerIP, char *Account, char *Character, int QueryType, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	//�����Ʒ��־:���׻��
	vector <CGlobalStruct::TFLV> XDJG_DB_TransGain_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	//�����Ʒ��־:����ʧȥ
	vector <CGlobalStruct::TFLV> XDJG_DB_TransLose_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	//�����Ʒ��־:��NPC������
	vector <CGlobalStruct::TFLV> XDJG_DB_BuyFromNpc_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	//�����Ʒ��־:����NPC
	vector <CGlobalStruct::TFLV> XDJG_DB_SaleToNpc_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	//�����Ʒ��־:ʰȡ��Ʒ
	vector <CGlobalStruct::TFLV> XDJG_DB_PickUpItem_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	//�����Ʒ��־:������Ʒ
	vector <CGlobalStruct::TFLV> XDJG_DB_DiscardItem_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	//�����Ʒ��־:��Ʒ����
	vector <CGlobalStruct::TFLV> XDJG_DB_ConsumeItem_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	//�����Ʒ��־:����������
	vector <CGlobalStruct::TFLV> XDJG_DB_AuctioneerSaled_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	//�����Ʒ��־:�����й���
	vector <CGlobalStruct::TFLV> XDJG_DB_AuctioneerBought_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	//����ʼ���־:�����ʼ�
	vector <CGlobalStruct::TFLV> XDJG_DB_SendMail_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	//����ʼ���־:��ȡ�ʼ�
	vector <CGlobalStruct::TFLV> XDJG_DB_ReceiveMail_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	//M��ʹ����־:�����������Ѽ�¼
	vector <CGlobalStruct::TFLV> XDJG_DB_ExpansionPack_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	//M��ʹ����־:������־
	vector <CGlobalStruct::TFLV> XDJG_DB_Deliver_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	//M��ʹ����־:��ɫ�ֿ��������Ѽ�¼
	vector <CGlobalStruct::TFLV> XDJG_DB_ExpansionWarehouse_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	//��ҽ�ɫ��־:��ɫ����
	vector <CGlobalStruct::TFLV> XDJG_DB_RoleUpgrade_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	//��ҽ�ɫ��־:��ɫ����
	vector <CGlobalStruct::TFLV> XDJG_DB_RoleCreate_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	//��ҽ�ɫ��־:��ɫ����ѧϰ
	vector <CGlobalStruct::TFLV> XDJG_DB_RoleSkill_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	//��ҽ�ɫ��־:��ȡ����
	vector <CGlobalStruct::TFLV> XDJG_DB_ReceiveTask_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	//��ҽ�ɫ��־:�������
	vector <CGlobalStruct::TFLV> XDJG_DB_CompleteTask_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	//��ҽ�ɫ��־:������ѯ
	vector <CGlobalStruct::TFLV> XDJG_DB_Copy_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	
	void GetItemInfo(Vec_ItemInfo &VecItemInfo);

	//��ѯ��
	//�����Ʒ��Ϣ:������Ʒ��Ϣ
	vector <CGlobalStruct::TFLV> XDJG_DB_ItemBody_Query(int userByID, char *ServerIP, char *Account, char *Character, int iIndex, int iPageSize);
	//�����Ʒ��Ϣ:������Ʒ��Ϣ
	vector <CGlobalStruct::TFLV> XDJG_DB_Itembackpack_Query(int userByID, char *ServerIP, char *Account, char *Character, int iIndex, int iPageSize);
	 
	//�����Ʒ��Ϣ:�ֿ���Ʒ��Ϣ
	vector <CGlobalStruct::TFLV> XDJG_DB_ItemStorage_Query(int userByID, char *ServerIP, char *Account, char *Character, int iIndex, int iPageSize);
	 
	//�����Ʒ��Ϣ:����ֿ���Ϣ
	vector <CGlobalStruct::TFLV> XDJG_DB_GuildStorage_Query(int userByID, char *ServerIP, char *Account, char *Character, int iIndex, int iPageSize);
	 
	//�����Ʒ��Ϣ:��ղֿ���Ϣ
	vector <CGlobalStruct::TFLV> XDJG_DB_KingkongStorage_Query(int userByID, char *ServerIP, char *Account, char *Character, int iIndex, int iPageSize);
	
	//�ʼ���Ϣ
	vector <CGlobalStruct::TFLV> XDJG_DB_MailInfo_Query(int userByID, char *ServerIP, char *Account, char *Character, int iIndex, int iPageSize);
	 
	//��������Ϣ
	vector <CGlobalStruct::TFLV> XDJG_DB_AuctioneerInfo_Query(int userByID, char *ServerIP, char *Account, char *Character, int iIndex, int iPageSize);
	
	//�û���ɫ��Ϣ���޸�:�޸ĵȼ�
	vector <CGlobalStruct::TFLV> XDJG_Net_ModifyLevel(int userByID, char *ServerIP, char *Character, int UpdateType, int Level, int Exp);
	
	//�û��������:�޸�����
	vector <CGlobalStruct::TFLV> XDJG_Net_ModifyPwd(int userByID, char *ServerIP, char *Account, char* NewPwd);
	
	//�û��������:��ʱ����ָ�
	vector <CGlobalStruct::TFLV> XDJG_Net_ResumePwd(int userByID, char *ServerIP, char *Account);
	 
	//�û���ɫ����:��ɫ�ָ�
	vector <CGlobalStruct::TFLV> XDJG_Net_ResumeRole(int userByID, char *ServerIP, char *Character);
	 
	//�û���ɫ����:��ɫɾ��
	vector <CGlobalStruct::TFLV> XDJG_Net_DeleteRole(int userByID, char *ServerIP, char *Character);
	 
	//�޸Ľ�ɫ���Ͻ�Ǯ����(�󶨱�)
	vector <CGlobalStruct::TFLV> XDJG_Net_UpRoleBind(int userByID, char *ServerIP, char *Character, int Money);
	
	//�޸Ľ�ɫ���Ͻ�Ǯ����(�ǰ󶨱�)
	vector <CGlobalStruct::TFLV> XDJG_Net_UpRoleNonBind(int userByID, char *ServerIP, char *Character, int Money);

	//��ӽ�ɫ���Ͻ�Ǯ����(�󶨱�)
	vector <CGlobalStruct::TFLV> XDJG_Net_AddRoleBind(int userByID, char *ServerIP, char *Character, int Money);
	
	//��ӽ�ɫ���Ͻ�Ǯ����(�ǰ󶨱�)
	vector <CGlobalStruct::TFLV> XDJG_Net_AddRoleNonBind(int userByID, char *ServerIP, char *Character, int Money);

	vector <CGlobalStruct::TFLV> CombinationTflv(St_ResCont &sResCont, vector <string> &VecItem);

	//��ȡsql
	string GetSqlCommend(char *sqlType);
	
	XDJG_StructInfo m_StructInfo;

	//��ɫ������Ϣ��ѯ
	vector <CGlobalStruct::TFLV> XDJG_RoleSkills_Query(char *ServerIP, char *Account, char *Character, int iIndex, int iPageSize);

	//�����б��ѯ
	vector <CGlobalStruct::TFLV> XDJG_SkillList_Query(char *GameName, char *Sql, char *Profession);

	//�������Ͳ�ѯ
	vector <CGlobalStruct::TFLV> XDJG_ItemType_Query(char *GameName, char *Sql);
	
	//��Ӧ���͵ĵ����б��ѯ
	vector <CGlobalStruct::TFLV> XDJG_ItemNameLst_Query(char *GameName, char *Sql, char *ItemType, char *ItemName, int index, int pagesize);

	//��Ʒ���Բ�ѯ
	vector <CGlobalStruct::TFLV> XDJG_ItemAttribute_Query(char *ServerIP, char *Character, char *ItemId);
	
	//��ȡLogDB������
	vector <string> GetLogDbName(char *BeginTime, char *EndTime);

	vector <CGlobalStruct::TFLV> QueryPlayerInfo(char *ServerIP, string strCharacter, int type, int iIndex, int iPageSize);

	//��ȡ��������
	string CXDJGInfo::GetSkillName(int SkillId, int iGetType);

	//��ȡ���������
	string CXDJGInfo::GetLifeSkillName(char *Prototype);

	//��ȡ��ɫ�ƺ��б�
	vector <CGlobalStruct::TFLV> GetRoleTitles(char *ServerIP, char *Character);

	//ȡ������б�
	vector <CGlobalStruct::TFLV> GetLifeSkills(char *ServerIP, char *Character);

	//��������������Ϣ
	void AnalysisItemInfo(char *ItemID, char *ItemNum, char *PropertyId, char *PropertyValue, char *IncreaseNum, char *IncreaseType, char *IncreaseValue, char *IncreaseEndValue, char *StarsLevel, char *AssemblyID, char *SlotNum, char *EquipSlotID1, char *EquipSlotID2, char *EquipSlotID3, char *QualityEquip, char *IsBind/*, char *ItemName*/);
	
	//��ӵ���
//	BOOL SendSingleItem(char *ServerIP, char *Character, vector<St_SendItemInfo>::iterator pSendItemInfo);
	BOOL SendSingleItem(char *ServerIP, char *Character, char *SendCharacter, char *mailTitle, char *mailContent, int Money, vector<St_SendItemInfo>::iterator pSendItemInfo);

	//���õ�������
	BOOL SetItemAttribute(char *ServerIP, char *Character, vector<St_SendItemInfo>::iterator pSendItemInfo);

	//������Ϣ��ѯ
	//���������Ϣ
	vector <CGlobalStruct::TFLV> XDJG_GuildBaseInfo_Query(char *ServerIP, char *GuildName);
		
	//�����Ա�б�
	vector <CGlobalStruct::TFLV> XDJG_GuildMemberList_Query(char *ServerIP, char *GuildName);
	
	//��������ѯ
	vector <CGlobalStruct::TFLV> XDJG_PlayerTaskInfo_Query(char *ServerIP, char *Account, char *Character, int iIndex, int iPageSize); 

	//��ճ�����Ϣ
	//��ճ���������Ϣ
	vector <CGlobalStruct::TFLV> XDJG_CarBaseInfo_Query(char *ServerIP, char *Account, char *Character, int iIndex, int iPageSize); 
	
	//��ճ�����ϸ��Ϣ
	vector <CGlobalStruct::TFLV> XDJG_CarDetailInfo_Query(char *ServerIP, char *Account, char *Character, int iIndex, int iPageSize); 
	
	//��ҹ�����Ϣ
	//��ҹ��������Ϣ
	vector <CGlobalStruct::TFLV> XDJG_PlayerGuildBaseInfo_Query(char *ServerIP, char *Account, char *Character, int iIndex, int iPageSize); 

	//��ҹ�����Ʒ��Ϣ
	vector <CGlobalStruct::TFLV> XDJG_PlayerGuildItemInfo_Query(char *ServerIP, char *Account, char *Character, int iIndex, int iPageSize); 

	//�����Ʒ��־
	//װ��������־
	vector <CGlobalStruct::TFLV> XDJG_EquipIncrease_Log_Query(char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);

	//��Ƭ�ϳ���־
	vector <CGlobalStruct::TFLV> XDJG_Synthesis_Log_Query(char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);

	//װ��������־
	vector <CGlobalStruct::TFLV> XDJG_EquipTransfer_Log_Query(char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
				
	//��Ʒ����
	vector <CGlobalStruct::TFLV> XDJG_ItemIdentify_Log_Query(char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	
	//���˲ֿ�����־
	vector <CGlobalStruct::TFLV> XDJG_RoleStore_Log_Query(char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	
	//����ֿ�����־
	vector <CGlobalStruct::TFLV> XDJG_GuildStore_Log_Query(char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);

	//������־
	//�������
	vector <CGlobalStruct::TFLV> XDJG_GuildCreate_Log_Query(char *ServerIP, char *Guildname, char *BeginTime, char *EndTime, int iIndex, int iPageSize);

	//�����ɢ
	vector <CGlobalStruct::TFLV> XDJG_GuildDismiss_Log_Query(char *ServerIP, char *Guildname, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	
	//������ӳ�Ա		
	vector <CGlobalStruct::TFLV> XDJG_GuildAddMember_Log_Query(char *ServerIP, char *Guildname, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	
	//�����Ա�뿪
	vector <CGlobalStruct::TFLV> XDJG_GuildMemberLeaved_Log_Query(char *ServerIP, char *Guildname, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	
	//����ְ����
	vector <CGlobalStruct::TFLV> XDJG_GuildPostChanged_Log_Query(char *ServerIP, char *Guildname, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	
	//���ݽ�ɫȡ�ʺ�
	CGlobalStruct::TFLV GetAccount(char *ServerIP, char *RoleName);

	//��ȡ������ʱ��
	string GetRealTime(int time);

	//�ʺ��Ƿ��ͣ
	vector <CGlobalStruct::TFLV> XDJG_AccountIsClose_Query(char *ServerIP, char *Account);

	//ȡ�������
	string GetCarName(int carId);
	
	//ȡ��������
	string GetTaskName(int TaskId);

	//ȡ��ɾ���Ľ�ɫ��Ϣ
	vector <CGlobalStruct::TFLV> GetDeletedRolesInfo(char *ServerIP, char *Account, int iIndex, int iPageSize);

	//NPC������ȡ��ѯ
	vector <CGlobalStruct::TFLV> XDJG_NPCGetItem_Query(char *ServerIP, char *Account, int AccountID, char *Character,int iIndex, int iPageSize);
	
	//��ȡ�콱����Ϣ
	vector <CGlobalStruct::TFLV> GetAwardPackageInfo(int iPackageId);

	//��������
	BOOL SetItemAttr(char *ServerIP,char *Character,int iType,int iParam1,int iParam2);

	//ת����16����
	string ConvertTo16(int iValue);

	//��ӳƺ�
	vector <CGlobalStruct::TFLV> XDJG_AddTitle_Query(int userByID, char *ServerIP,char *Account,char *Character,int iTitleID, char *TitleName);
			
	//�Ƴ��ƺ�
	vector <CGlobalStruct::TFLV> XDJG_RemoveTitle_Query(int userByID, char *ServerIP,char *Account,char *Character,int iTitleID, char *TitleName);
			
	//���BUFF
	vector <CGlobalStruct::TFLV> XDJG_AddBuff_Query(int userByID, char *ServerIP,char *Account,char *Character,int iBuffID, char *BuffName);
			
	//�Ƴ�BUFF
	vector <CGlobalStruct::TFLV> XDJG_RemoveBuff_Query(int userByID, char *ServerIP,char *Account,char *Character,int iBuffID, char *BuffName);

	//�ƶ���ָ��λ��
	vector <CGlobalStruct::TFLV> XDJG_MoveToPosition_Query(int userByID, char *ServerIP,char *Account,char *Character, int iPX, int iPY, int iPZ);
			
	//�ƶ���ָ��������
	vector <CGlobalStruct::TFLV> XDJG_MoveToRole_Query(int userByID, char *ServerIP,char *Account,char *Character,char *TargetCharacter);

	//�ƶ���Ŀ��������
	vector <CGlobalStruct::TFLV> XDJG_MoveToTargetRole_Query(int userByID, char *ServerIP,char *Account,char *Character,char *TargetCharacter);
	
	//�ƺ��б��ѯ
	vector <CGlobalStruct::TFLV> XDJG_TitleList_Query(char *Gamename, char *Sql);
			
	//BUFF�б��ѯ
	vector <CGlobalStruct::TFLV> XDJG_BuffList_Query(char *Gamename, char *Sql);
	//End
	
	//	vector <CGlobalStruct::TFLV> XDJG_Net_BoardTasker_Oper(int userByID, int op_type,char *content, char *begintime, int interval, int repeat, char * begindate, char *enddate, int notice_index/*=0*/); 
	//	vector <CGlobalStruct::TFLV> XDJG_Net_BoardTasker_Query(char * GameName,char * m_sql,int iIndex,int iPageSize);
	//	bool getBoardTaskerDetail(vector<string> &vectorDetail, int notice_index);
private:
	//��¼��־��Ϣ
	char strLog[2048];

	//�������ݿ�IP��ַ
	char LocalIP[30];

	//�ʺ����ݿ�
	char AccountDB[30];

	//��־���ݿ�
	char LogDB[30];

	//��Ϸ���ݿ�
	char GameDB[30];

	//�ʼ����ݿ�
	char MailDB[30];

	//Added by tengjie 2010-05-17
	struct  _ST_XDJG
	{
		enum __ENUM
		{
			NONE_BANACOOUNT = 0,
			MAX_LOGINTIMES	= 1,
			BANED_BYGM		= 2,
			BANED_BYPLUG	= 5,
			WEB_FREEZE		= 10,
			WEB_LOCK		= 11,
			NONE_REASON		= 15
		}_value;
	};

	Vec_SendItemInfo m_VecSendItemInfo;

	CSqlHelper m_SqlHelper;
	//�������˿ں�
//	int port;
	
	COperatorXDJG m_operXDJG;

	Vec_GuildItemInfo m_VecGuildItemInfo;
};