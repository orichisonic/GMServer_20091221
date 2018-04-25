#ifndef GTOPER_H_
#define GTOPER_H_

#include "packet.h"
#include "OperatorGT.h"
#include "..\\GameIOCP\\GameInfo.h"
struct TIMEEVENT
{
	//��ʼʱ��
	int starthour;		//[0,23]
	int startminute;	//[0,59]
};
struct ITEMPACKET
{
	char szAccount[256];
	int	 mID;
	int  iNum;
};
class  CGTOper:public CGameInfo
{

private:

	//�����м�ĸ�������
	COperatorGT operGT;

public:

	CGTOper(void);
	~CGTOper(void);
private:
	char strLog[2048];

	//���յ����ݳ���
	int recvLen;

	//����Զ�̽ӿ�
	CMSocket msocket;
public:
	
	//����Զ�̽ӿ�
	bool CONNECT_SERVERMANAGER_LOGIN(char * ServerIP,char * retMessage);

	//��ѯ����
	vector<CGlobalStruct::TFLV> GT_BROADCAST_SELECT(char * ServerIP,int iIndex,int iPageSize);

	//��ӹ���
	BOOL GT_BROADCAST_ADD(int IsImmediate,wstring context,DWORD interval,size_t StartTime,size_t EndTime);

	//ɾ������
	BOOL GT_BROADCAST_REMOVE(int NoticeID);

	//ǿ��������ߣ���ѯͨ�����ݱ�����ֻ���˺����ˣ�
	BOOL GT_KICK_USER(char * ServerIP,char * szAccount,char * retMessage);
	
	//
	static int GT_KICK_USER(char * ServerIP,char * strAccount);

	bool GT_BanUser(char * ServerIP,char * strAccount,int status);//1.��ͣ,0.���


	//��ɫ��������
	void GT_MOVE_USER(int userByID, char * ServerName, char * ServerIP,char * UserName);

	//������ҵ�����״̬
	void GT_USER_RESET(int userByID, char * ServerName, char * ServerIP,char * UserName);

	//��ѯ���ߡ�����
	void GM_ITEM_Check();

	//��ӵ���
	void GT_ITEM_ADD(int userByID,char * ServerName, char * ServerIP,char * UserName,char * szMailTitle, char * szMailContent,int count,char * itemName);

	
	//��ͣ�ͽ��
	BOOL GT_BanUser(int userByID, char * ServerName, char * ServerIP,char * UserName,int UserID,char* userState,char * Reason);

	//˲���ƶ�
	void GT_Transmit_Query(int userByID, char * ServerName, char * ServerIP,char * UserName,int x,int y,int z);
	
	//��ֹ���������ϴ�ͼƬ
	bool GT_Picture_Stop(int userByID, char * ServerName, char * ServerIP,char * UserName,int UserID,char* userState,char * Reason);
	
	//����Υ��ͼƬ
	void GT_Picture_Clear(int userByID, char * ServerName, char * ServerIP,char * UserName);
	
	//��ո���ǩ��
	void GT_ClearName_Query(int userByID, char * ServerName, char * ServerIP,char * UserName);

	//���ݵ���
	bool GT_Import_Item(int userByID,char * ServerName, char * ServerIP, char * MailTitle,char * MailContent, char * ListUserName, char * ListItemID, char * ListItemNum);


	//�û���Ϣ��ѯ
	bool UserInfo(char * ServerName, char * ServerIP,char * UserName, char * UserNick,int iIndex,int iPageSize);
	//�����ʺ�����
	vector <CGlobalStruct::TFLV> KickPlayer(int userByID,char * ServerName, char * ServerIP,char * UserName);

	//��ѯ����
	bool BroadTask_Query(char * ServerIP,int iIndex,int iPageSize);

	//���¹���
	bool BroadTask_Update(int userByID,int taskid,char * boardMessage,char * begintime,char * endtime,int interval,int status);

	//���빫��
	bool BroadTask_Insert(int userByID,char * ServerIP,char * GSServerIP,char * boardMessage,char * begintime,char * endtime,int interval);
	
	
	


};
#endif




















