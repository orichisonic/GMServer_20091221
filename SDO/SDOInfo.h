#ifndef SDOINFO_H_
#define SDOINFO_H_

#include "OperatorSDO.h"
#include "..\\GameIOCP\\GameInfo.h"

class  CSDOInfo:public CGameInfo 
{
private:

	//���ڱ����������ݿ�ִ�в�����sql���
	char operszSql[2048];

	//���ڱ�����Զ�����ݿ�ִ�в�����sql���
	char operRemoteSql[2048];

	//���ݷ��������ƻ�ȡ��Ҫ��IP
	char localIP[20];

	//���ݿ�����
	char SDO_DBName[64];

	//��־���ݿ�����
	char Log_DBName[64];

	//װ�����ݿ�����
	char Item_DBName[64];

	//��¼��־��Ϣ
	char strLog[2048];
/*
	//��ɫID
	int UserID;

	//����ԱID
	int UserByID;
	
	//�ʺ�
	char SDO_Account[40];

	//��ɫ�ǳ�
	char SDO_NickName[40];

	//��ʼʱ��
	char SDO_BeginTime[40];

	//����ʱ��
	char SDO_EndTime[40];

	//����ID
	int FamilyID;

	//��������
	char FamilyName[20];

	//����ҳ����
	int index;

	//����ҳ���С
	int iPageSize;

private:

	//���ڱ����ѯ�ӿڷ��صĽ��
	COperVector DBVect;

	//���ڱ�������ͻ������ݵõ��Ľ��
	COperVector recvVect;

	//����д��־��Ϣ
	CLogFile logFile;

	//�����м�ĸ�������
	COperatorSDO operSDO;
*/
public:

	CSDOInfo(void);
	~CSDOInfo(void);

	//��ʼ�����еı���
	virtual COperVector * initialize(CSession * pSess,CEnumCore::Message_Tag_ID MessageTagID, unsigned char * pbuf, int length);
	
public:

	//��ѯ��һ���������Ϣ
	vector<CGlobalStruct::TFLV> SDO_UserInfo(int userByID,char * ServerName, char * ServerIP,char * Account,char * NickName,int iIndex,int iPageSize);

	//��ѯ����ʺ���Ϣ
	vector<CGlobalStruct::TFLV> SDO_AccountInfo(int userByID,char * ServerName, char * ServerIP,char * Account,char * NickName,int iIndex,int iPageSize);

	//��ѯ�����Ʒ
	vector<CGlobalStruct::TFLV> SDO_ItemInfo(int userByID,char * ServerName, char * ServerIP,char * Account, int UserID,int iIndex,int iPageSize);

	//��ѯ��ҳ�����Ϣ
	vector<CGlobalStruct::TFLV> SDO_PetInfo(int userByID,char * ServerName, char * ServerIP,char * Account, int UserID,int iIndex,int iPageSize);

	//�鿴��ҵĳ���ɾ����Ϣ
	vector<CGlobalStruct::TFLV> SDO_PetDrop(int userByID, char * ServerName, char * ServerIP,char * Account, int UserID,int iIndex,int iPageSize);

	//��ѯ����˺ŵ�¼��¼
	vector<CGlobalStruct::TFLV> SDO_LoginLog(int userByID, char * ServerName, char * ServerIP,char * Account,int iIndex,int iPageSize);

	//��ѯ�����Ʒ���ͼ�¼
	vector<CGlobalStruct::TFLV> SDO_UserTrade_Log(int userByID, char * ServerName, char * ServerIP,char * Account,char * NickName,int itype, int iIndex,int iPageSize);

	//��ѯ������Ѽ�¼
	vector<CGlobalStruct::TFLV> SDO_UserConsume(int userByID, char * ServerName, char * ServerIP,char * Account,char * NickName,int iMoneytype, char *ItemName, char *BeginTime, char *EndTime, int iIndex,int iPageSize);

	//�鿴��ҵ�ǰ״̬(������/����/����״̬)
	vector<CGlobalStruct::TFLV> SDO_UserStatus(int userByID, char * ServerName, char * ServerIP,char * Account,int UserID,int iIndex,int iPageSize);

	//�鿴��һ���״̬
	vector<CGlobalStruct::TFLV> SDO_UserMarriage(int userByID, char * ServerName, char * ServerIP,char * Account,char * NickName,int UserID,int iIndex,int iPageSize);

	//�鿴���ҡҡ�ֻ����Ʒ
	vector<CGlobalStruct::TFLV> SDO_YYItem_QUERY(int userByID, char * ServerName, char * ServerIP,char * Account,int iIndex,int iPageSize);

	//ǿ���������
	vector<CGlobalStruct::TFLV> SDO_KickPlayer_Off(int userByID, char * ServerName, char * ServerIP,char * Account, int UserID);

	//��ѯ��ҷ�ͣ״̬
	vector<CGlobalStruct::TFLV> SDO_UserBanStatus(int userByID, char * ServerName, char * ServerIP,char * Account,int iIndex,int iPageSize);

	//��ͣ����ʺ�
	vector<CGlobalStruct::TFLV> SDO_Account_Close(int userByID, char * ServerName, char * ServerIP,char * Account,char *BanReason);

	//��ѯ���б���ͣ�ʺ��б�
	vector<CGlobalStruct::TFLV> SDO_QueryBan_All(int userByID, char * ServerName, char * ServerIP,int iIndex,int iPageSize);

	//�������ʺ�
	vector<CGlobalStruct::TFLV> SDO_Account_Open(int userByID, char * ServerName, char * ServerIP,char * Account,char *UnbindReason);

	//��ѯ������Ϣ�б�
	vector<CGlobalStruct::TFLV> SDO_NoticeInfo(int userByID, char * ServerName, char * ServerIP,int iIndex,int iPageSize);

	//��ӹ�����Ϣ
	vector<CGlobalStruct::TFLV> SDO_AddNotice(int userByID, char * ServerName, char * ServerIP, char *BoardMessage, char *BeginTime, char *EndTime, int interval, int iNoticeType);

	//��ѯ���͹���Ĵ����б���Ϣ
	vector<CGlobalStruct::TFLV> SDO_BoardCity_Query(int userByID, int taskID,int iIndex,int iPageSize);

	//�޸Ĺ�����Ϣ
	vector<CGlobalStruct::TFLV> SDO_EditNotice(int userByID, char * ServerName, char * ServerIP, int taskID, char *BoardMessage, char *BeginTime, char *EndTime, int interval,int status,int iNoticeType);

	//��ѯ���������Ϣ
	vector<CGlobalStruct::TFLV> SDO_FamilyInfo(int userByID, char * ServerName, char * ServerIP, char *FamilyName, int index, int iPageSize);

	//��ѯ�����Ա��Ϣ
	vector<CGlobalStruct::TFLV> SDO_FamilyMember(int userByID, char * ServerName, char * ServerIP, int FamilyID, char *FamilyName, int iIndex, int iPageSize);

	//��ѯ�����м����Ա��Ϣ
	vector<CGlobalStruct::TFLV> SDO_FamilyFormal(int userByID, char * ServerName, char * ServerIP, int FamilyID, char *FamilyName, int iIndex, int iPageSize);

	//��ѯ����ѫ����Ϣ
	vector<CGlobalStruct::TFLV> SDO_FamilyBadge(int userByID, char * ServerName, char * ServerIP, int FamilyID, char *FamilyName, int iIndex, int iPageSize);

	//��ѯ�����������ϸ
	vector<CGlobalStruct::TFLV> SDO_FamilyDonate(int userByID, char * ServerName, char * ServerIP, int FamilyID, char *FamilyName, char *BeginTime, char *EndTime,int iIndex, int iPageSize);

	//��ѯ���幺���¼
	vector<CGlobalStruct::TFLV> SDO_FamilyConsume(int userByID, char * ServerName, char * ServerIP, int FamilyID, char *FamilyName, char *BeginTime, char *EndTime,int iIndex, int iPageSize);

	//�޸ļ����Աְ��
	vector<CGlobalStruct::TFLV> SDO_ModiMemDuty(int userByID, char * ServerName, char * ServerIP, char *NickName, int UserID, int FamilyID, char *FamilyName, int type);

	//��ɢ����
	vector<CGlobalStruct::TFLV> SDO_DeleteFamily(int userByID, char * ServerName, char * ServerIP, int FamilyID, char *FamilyName);

	//��Ӽ������
	vector<CGlobalStruct::TFLV> SDO_Insert_FamilyBadge(int userByID, char * ServerName, char * ServerIP, char *NickName, int UserID, int FamilyID, char *FamilyName, int EnblemType, int EnblemNum);

	//ɾ���������
	vector<CGlobalStruct::TFLV> SDO_Delete_FamilyBadge(int userByID, char * ServerName, char * ServerIP, char *NickName, int UserID, char *FamilyName, int EmblemType);

	//�鿴���G��
	vector<CGlobalStruct::TFLV> SDO_UserGCash_Query(int userByID, char * ServerName, char * ServerIP, char *Account);

	//�������G��
	vector<CGlobalStruct::TFLV> SDO_UserGCash_Update(int userByID, char * ServerName, char * ServerIP, char *Account, int GCash);

	//������ս��Ϣ��ѯ
	vector<CGlobalStruct::TFLV> SDO_CHALLENGE_QUERY(int userByID, char * ServerName, char * ServerIP, int iIndex, int iPageSize);

	//���ô�����ս��Ϣ
	vector<CGlobalStruct::TFLV> SDO_CHALLENGE_INSERT(int userByID, char * ServerName, char * ServerIP, int weekDay, int matPtMin, int stPtMin, int GCash, int scence, int isBattle);
			
	//�޸Ĵ�����ս��Ϣ
	vector<CGlobalStruct::TFLV> SDO_CHALLENGE_UPDATE(int userByID, char * ServerName, char * ServerIP, int ChallengeID, int weekDay, int matPtMin, int stPtMin, int GCash, int scence, int isBattle);
			
	//ɾ��������ս��Ϣ
	vector<CGlobalStruct::TFLV> SDO_CHALLENGE_DELETE(int userByID, char * ServerName, char * ServerIP, int ChallengeID);

	//�鿴��Ϸ�����б�
	vector<CGlobalStruct::TFLV> SDO_SCENE_QUERY(int userByID, char * ServerName, char * ServerIP);

	//��Ӵ�����ս�ĳ���
	vector<CGlobalStruct::TFLV> SDO_SCENE_CREATE(int userByID, char * ServerName, char * ServerIP, char *SceneName);
	
	//�޸Ĵ�����ս�ĳ���
	vector<CGlobalStruct::TFLV> SDO_SCENE_UPDATE(int userByID, char * ServerName, char * ServerIP, int SceneID, char *SceneName);
			
	//ɾ��������ս�ĳ���
	vector<CGlobalStruct::TFLV> SDO_SCENE_DELETE(int userByID, char * ServerName, char * ServerIP, int SceneID);

	//�鿴����Ϸ��ҡҡ�ֻ�ø���
	vector<CGlobalStruct::TFLV> SDO_YYHappyItem_Query(int userByID, char * ServerName, char * ServerIP, int iIndex, int iPageSize);
	
	//���ҡҡ�ֻ�ø���
	vector<CGlobalStruct::TFLV> SDO_YYHappyItem_Insert(int userByID, char * ServerName, char * ServerIP, int itemCode1, int itemCode2, int level, int levPercent, int precent, int timeslimit, int dayslimit, char *itemName1, char *itemName2);
		
	//����ҡҡ�ֻ�ø���
	vector<CGlobalStruct::TFLV> SDO_YYHappyItem_Update(int userByID, char * ServerName, char * ServerIP, int itemCode, int itemCode1, int itemCode2, int level, int levPercent, int precent, int timeslimit, int dayslimit, char *itemName1, char *itemName2);
	
	//ɾ��ҡҡ�ֻ�ø���
	vector<CGlobalStruct::TFLV> SDO_YYHappyItem_Delete(int userByID, char * ServerName, char * ServerIP, int itemCode);
	
	//�鿴��Ϸ����ĵ�����Ϣ
	vector<CGlobalStruct::TFLV> SDO_Item_QueryAll(int userByID, char * ServerName, char * ServerIP, int BigType, int SmallType, int sex, char * itemName);

	//���߻�ȡ���ʲ�ѯ
	vector<CGlobalStruct::TFLV> SDO_MedalItem_Query(int userByID, char * ServerName, char * ServerIP, int iIndex, int iPageSize);
			
	//�������߻�ȡ���ʲ�ѯ
	vector<CGlobalStruct::TFLV> SDO_MedalItem_Owner_Query(int userByID, char * ServerName, char * ServerIP, char *itemName, int iIndex, int iPageSize);
			
	//��ӵ��߻�ȡ����
	vector<CGlobalStruct::TFLV> SDO_MedalItem_Insert(int userByID, char * ServerName, char * ServerIP, int itemCode, int precent, char *itemName);
			
	//���µ��߻�ȡ����
	vector<CGlobalStruct::TFLV> SDO_MedalItem_Update(int userByID, char * ServerName, char * ServerIP, int itemCode, int precent, char *itemName);
			
	//ɾ�����߻�ȡ����
	vector<CGlobalStruct::TFLV> SDO_MedalItem_Delete(int userByID, char * ServerName, char * ServerIP, int itemCode, char *itemName);

	//������ҽ�ɫ��Ϣ
	vector<CGlobalStruct::TFLV> SDO_CharacterInfo_Update(int userByID, char * ServerName, char * ServerIP, int UserID, char *Account,int level, int experience, int battle, int win, int draw, int lose, int MCash, int GCash);
/*			
	//�鿴������ϵ���
	bool SDO_UserEquip_Query();
*/			
	//ɾ��������ϵ���
	vector<CGlobalStruct::TFLV> SDO_ItemShop_Delete(int userByID, char * ServerName, char * ServerIP, int UserID, char *Account, int itemCode, char *itemName);
			
	//�鿴�������е���
	vector<CGlobalStruct::TFLV> SDO_GiftBox_Query(int userByID, char * ServerName, char * ServerIP, int UserID, char *Account, int iIndex, int iPageSize);
			
	//ɾ���������е���
	vector<CGlobalStruct::TFLV> SDO_GiftBox_Delete(int userByID, char * ServerName, char * ServerIP, int UserID, char *Account, int itemCode, char *itemName);
			
	//����ҷ��͵���
	vector<CGlobalStruct::TFLV> SDO_GiftBox_Insert(int userByID, char * ServerName, char * ServerIP, int UserID, char *Account, int itemCode, int timeslimit, int DateLimit, char *itemName, char *title, char *content);

	//������ӱ�����Ϣ
	vector<CGlobalStruct::TFLV> SDO_Challenge_InsertAll(int userByID, char * ServerName, char * ServerIP, int weekDay, int matPtMin, int stPtMin, int GCash, int scence, int isBattle);
	
	//��ȡ�������� <iSouNT: 1 ��ʱ���棬0 ��ʱ����
	void GetNoticeType(char *strDesNT,int &iDesNT, int iSouNT);

};
#endif




















