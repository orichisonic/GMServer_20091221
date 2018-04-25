#ifndef GTINFO_H_
#define GTINFO_H_

#include "OperatorGT.h"

#include "..\\GameIOCP\\GameInfo.h"
class  CGTInfo:public CGameInfo
{
private:
	//���ڱ����������ݿ�ִ�в�����sql���
	char operszSql[2048];

	//���ڱ�����Զ�����ݿ�ִ�в�����sql���
	char operRemoteSql[2048];

	//���ڱ���ִ�в������û���
	char m_UserName[128];

	//��¼��־��Ϣ
	char strLog[2048];

public:


	//����д��־��Ϣ


public:

	CGTInfo(void);
	~CGTInfo(void);

	
private:
	//���յ����ݳ���
	int recvLen;

public:

	//��ѯ��ɫ��ϸ��Ϣ
	vector <CGlobalStruct::TFLV> GT_FriendTrade_Log(char * ServerName, char * ServerIP,char * UserName, int UserID,int iIndex,int iPageSize);

	//��ѯ������Ϣ
	vector <CGlobalStruct::TFLV> GT_FamilyInfo(char * ServerName, char * ServerIP,char * FamilyName,int iIndex,int iPageSize);

	//��ѯ�����Ա��Ϣ
	vector <CGlobalStruct::TFLV> GT_MemberInfo(char * ServerName, char * ServerIP,char * FamilyName,int FamilyID,int iIndex,int iPageSize);

	//�ʼ���Ϣ��ѯ
	vector <CGlobalStruct::TFLV> GT_MailInfo(char * ServerName, char * ServerIP,char * UserName, int UserID,char* szStartTime,char* szEndTime,int iIndex,int iPageSize);


	//��ѯ��ұ��俪����Ϣ
	vector <CGlobalStruct::TFLV> GT_TreasureBox_Open(char * ServerName, char * ServerIP,char * UserName, char* szStartTime,char* szEndTime,int iIndex,int iPageSize);


	//��ѯ���߹������Ѽ�¼
	vector <CGlobalStruct::TFLV> GT_ItemBuy(char * ServerName, char * ServerIP,char* UserName,
						 int UserID,int type,char * ItemName,char * szStartTime,char * szEndTime,int iIndex,int iPageSize);

	//Ѫƴս��
	vector <CGlobalStruct::TFLV> GT_Mcoin_Rank(char * ServerName,char * ServerIP,int iType,int iIndex,int iPageSize);
	
	//�ռ�����
	vector <CGlobalStruct::TFLV> GT_Sns_Query(char * ServerName,char * ServerIP,int iType,int iIndex,int iPageSize);
	
	//��������
	vector <CGlobalStruct::TFLV> GT_Attr_Queue(char * ServerName,char * ServerIP,int iType,int iIndex,int iPageSize);

	//��Ƭ����
	vector <CGlobalStruct::TFLV> GT_Card_Record(char * ServerName,char * ServerIP,int iType,int iIndex,int iPageSize);

	//����
	vector <CGlobalStruct::TFLV> GT_FriendTrade_Queue(char * ServerName,char * ServerIP,int iType,int iIndex,int iPageSize);
	
	//���߲�ѯ
	vector <CGlobalStruct::TFLV> GT_Select_Item(char * ServerName,char * ServerIP,char * ItemName);

	vector <CGlobalStruct::TFLV> GT_Back_Money(int userByID,char * ServerName, char * ServerIP, char * UserName, int iID,int iType,int iMoney,char * strItemName);
};
#endif




















