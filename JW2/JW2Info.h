#ifndef PALLOGINFO_H_
#define PALLOGINFO_H_
#define GameDB 1
#define ItemDB 2
#define LoginDB 3
#define LogDB 4
#define GateWay 5
#define Messenger 6

#include "OperatorJW2.h"

#include "..\\GameIOCP\\GameInfo.h"

class CJW2Info:public CGameInfo
{
private:
	//���ڱ����������ݿ�ִ�в�����sql���
	char operszSql[2048];

	//���ڱ�����Զ�����ݿ�ִ�в�����sql���
	char operRemoteSql[2048];


	//��¼��־��Ϣ
	char strLog[2048];

	//���ݷ��������ƻ�ȡ��Ҫ��IP
	char logdbip[20];
		

public:

	//���캯��
	CJW2Info();

	//��������
	~CJW2Info();


	//�����ʺ�����
	vector<CGlobalStruct::TFLV> KickPlayer(int userByID,char * ServerName, char * ServerIP,char * UserName);
	//�����ʺŷ�ͣ
	vector<CGlobalStruct::TFLV> MakeClose(int userByID,char * ServerName, char * ServerIP ,char * UserName,char * UserNick,int JW2_UserSN, char* Reason);
	//�����ʺŽ��
	vector<CGlobalStruct::TFLV> MakeOpen(int userByID,char * ServerName, char * ServerIP ,char * UserName,char * UserNick,int JW2_UserSN,char* Reason);

	//�鿴�������7��10��
	vector<CGlobalStruct::TFLV> JW2_SMALL_PRESENT_QUERY(char* ServerName,char * ServerIP,char * UserName,int JW2_UserSN,int iType,int iIndex,int iPageSize);

	//����������/�ǳ���Ϣ
	vector<CGlobalStruct::TFLV> LoginInfo(char * ServerName, char * ServerIP,char * UserName, char * UserNick,char * IP,char * begintime, char * endtime,int iIndex, int iPageSize);
	
	//��ѯ������Ϣ
	vector<CGlobalStruct::TFLV> JW2_FAMILYINFO_QUERY(char * ServerName, char * ServerIP,char * FamilyName, int iIndex,int iPageSize);

	vector<CGlobalStruct::TFLV> JW2_Family_Detail(CEnumCore::Message_Tag_ID m_tagID,char * ServerName, char * ServerIP,char * FamilyName, int FamilyID,int iIndex,int iPageSize);

	//�޸�����
	vector<CGlobalStruct::TFLV>  JW2_MODIFY_PWD(int userByID, char * ServerName,char * ServerIP,char * UserName,int JW2_UserSN,char * szNewPw);

	//�ָ�����
	vector<CGlobalStruct::TFLV> JW2_RECALL_PWD(int userByID, char * ServerName,char * ServerIP,char * UserName,int JW2_UserSN);

	//�޸ĵȼ�
	vector<CGlobalStruct::TFLV> JW2_MODIFYLEVEL_QUERY(int userByID, char * ServerName,char * ServerIP,char * UserName,int JW2_UserSN,int iLevel);
	
	//�޸ľ���
	vector<CGlobalStruct::TFLV> JW2_MODIFYEXP_QUERY(int userByID, char * ServerName,char * ServerIP,char * UserName,int JW2_UserSN,int iExp);

	//��ѯ����
	vector<CGlobalStruct::TFLV> JW2_ItemInfo_Query(char* ServerName,char * ServerIP,char * UserName,int JW2_UserSN,int iItemPos,int iIndex,int iPageSize);

	//ɾ������
	vector<CGlobalStruct::TFLV> JW2_ITEM_DEL(int userByID,char * ServerName,char * ServerIP,char * UserName, int JW2_UserID,char * avatarItem,char * avatarItemName,int iItemPos, int iItemNo);

	//��ӵ���
	vector<CGlobalStruct::TFLV> JW2_ADD_ITEM(int userByID,char * ServerName, char * ServerIP ,char* UserName,int JW2_UserSN,
							char * strMailTitle,char * strMailContent,char* JW2_AvatarItem,int count);

	//�޸Ľ�Ǯ
	vector<CGlobalStruct::TFLV> JW2_MODIFY_MONEY(int userByID, char * ServerName,char * ServerIP,char * UserName,int JW2_UserSN,int iMoney);

	//ģ�����߲�ѯ
	vector<CGlobalStruct::TFLV> JW2_ITEM_SELECT(int userByID,char * JW2_AvatarItemName);
	
	//������Ϣ��ѯ
	vector<CGlobalStruct::TFLV> JW2_BasicInfo_Query(char * ServerName, char * ServerIP,char * FamilyName, int iIndex,int iPageSize);


	
	//��Ǯ��־��ѯ
	vector<CGlobalStruct::TFLV> JW2_MoneyLog_Query(char * ServerName, char * ServerIP,char* UserName,
						 int JW2_UserSN,int iType,char * szStartTime,char * szEndTime,int iIndex,int iPageSize);
	
	//������־
	vector<CGlobalStruct::TFLV> JW2_FamilyFund_Log(char * ServerName, char * ServerIP,char* FamilyName,
						 int FamilyID,int iStatus,char * szStartTime,char * szEndTime,int iIndex,int iPageSize);
	
	//������߹�����־
	vector<CGlobalStruct::TFLV> JW2_FamilyBuyLog_Query(char * ServerName, char * ServerIP,char* FamilyName,
						 int FamilyID,char * szStartTime,char * szEndTime,int iIndex,int iPageSize);
	
	//������־
	vector<CGlobalStruct::TFLV> JW2_CashMoney_Log(char * ServerName, char * ServerIP,char* UserName,int JW2_UserSN,int iType,char * szStartTime,char * szEndTime,int iIndex,int iPageSize);
	
	//��������
	vector<CGlobalStruct::TFLV> JW2_Import_KickPlayer();
	
	//������ͣ
	vector<CGlobalStruct::TFLV> JW2_Import_AccountClose();
	
	//�������
	vector<CGlobalStruct::TFLV> JW2_Import_AccountOpen();
	
	//����GM״̬
	vector<CGlobalStruct::TFLV> JW2_GM_Update(int userByID, char * ServerName,char * ServerIP,char * UserName,int JW2_UserSN,int iStatus);

	//����Ȩ��
	vector<CGlobalStruct::TFLV> JW2_ProductManage_Insert(int userByID,char * ServerName,char * ServerIP,char * JW2_AvatarItem,	char * GMAccount,char * begintime,char * endtime,int totalprice);
		
	//�鿴Ȩ��
	vector<CGlobalStruct::TFLV> JW2_ProductManage_View(int iIndex, int iPageSize);
	
	//ɾ��Ȩ��
	vector<CGlobalStruct::TFLV> JW2_ProductManage_Del(char * ServerName, char * ServerIP,int userByID,int iTaskid);

	//maple add
	//��֤Ȩ��
	vector<CGlobalStruct::TFLV> JW2_ProductManage_Validate(char * ServerName,char * ServerIP, int GMID,char * JW2_AvatarItem);

	//��֤ͨ����Ȩ�ޱ��м�ȥ
	vector<CGlobalStruct::TFLV> JW2_ProductManage_Deduct(int userByID,int GMID,char * ServerIP, char * JW2_AvatarItem);
};
#endif