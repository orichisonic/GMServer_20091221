#ifndef PALLOGINFO_H_
#define PALLOGINFO_H_

#include "OperatorPal.h"
#include "..\\GameIOCP\\GameInfo.h"

class CPalLogInfo:public CGameInfo
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

private:
	//����д��־��Ϣ
	CLogFile logFile;

	//�����м�ĸ�������
	COperatorPal operAssist;

public:

	//���캯��
	CPalLogInfo();

	//��������
	~CPalLogInfo();


	//��ѯ�̳ǹ����¼
	bool Shop_Buy_Query(char * ServerName,char * ServerIP,char * RoleName,int iMallType,char * szBegintime, char * szEndtime,int iIndex, int iPageSize);

	//��ѯ������Ϣ
	bool MailBox_Info_Query(char * ServerName,char * ServerIP,char * RoleName,int RoleID, char * szBegintime, char * szEndtime,int iIndex, int iPageSize);

	//��ѯ�����ʼ��ĸ���
	bool MailBox_Info_Detail(char * ServerName,char * ServerIP,char * RoleName,int MailID, char * szBegintime, char * szEndtime,int iIndex, int iPageSize);

	//��ѯ������Ϣ
	bool Auction_Info_Query(char * ServerName,char * ServerIP,char * RoleName,int RoleID, char * szBegintime, char * szEndtime,int iIndex, int iPageSize);

	//��ѯ��־��Ϣ
	bool LogInfo_Query(char * ServerName,char * ServerIP,char * RoleName,int iBigType,char * szBigType,int iSmallType,char * szSmallType,char * szBegintime, char * szEndtime,int iIndex, int iPageSize);

	//��ѯ��־��Ϣ
	bool HisLogInfo_Query(char * ServerName,char * ServerIP,char * RoleName,int iBigType,char * szBigType,int iSmallType,char * szSmallType,char * szBegintime, char * szEndtime,int iIndex, int iPageSize);

	//��ѯ���CDKEY������ȡ��Ϣ
	bool CDKey_Get_Query(char * ServerName,char * ServerIP,char * szAccount, char * szBegintime, char * szEndtime,int iIndex, int iPageSize);

	//��ѯ���Ԫ�����ļ�¼
	bool Money_Waste_Query(char * ServerName,char * ServerIP,char * szAccount, char * szBegintime, char * szEndtime,int iIndex, int iPageSize);

	//��ȡ��־����
	bool GetLogBigType();

	//��ȡ��־С��
	bool GetLogSmallType(int iBigType);

	//��ѯ��ɫɾ����־
	bool Pal_RoleDel_Query(char * ServerName,char * ServerIP,char * szAccount,char * RoleName, int iType,char * szBegintime, char * szEndtime,int iIndex, int iPageSize);

	// add by ltx
	//װ�����߲�ѯ
	void Pal_Equipment_Query(char * CustomItemName);



	//�û���Ϣ��ѯ
	bool UserInfo(char * ServerName, char * ServerIP,char * UserName, char * UserNick,int iIndex,int iPageSize);
	//�����ʺŷ�ͣ
	bool ClosePlayer(int userByID,char * ServerName, char * ServerIP ,char * UserName,char * UserNick, char* Reason);
	//�����ʺŽ��
	bool OpenPlayer(int userByID,char * ServerName, char * ServerIP ,char * UserName,char * UserNick, char* Reason);
	//��ѯ��ͣ�б�
	bool CloseList(char * ServerName,char * ServerIP,int iIndex, int iPageSize);
	//��ѯ�����˺�
	bool CloseSingle(char * ServerName, char * ServerIP,char * UserName,int iIndex, int iPageSize);
	//��ѯ����
	bool BroadTask_Query(int iIndex,int iPageSize);
	//���¹���
	bool BroadTask_Update(int userByID,int taskid,char * boardMessage,char * begintime,char * endtime,int interval,int status);
	//���빫��
	bool BroadTask_Insert(int userByID,char * ServerIP,char * GSServerIP,char * boardMessage,char * begintime,char * endtime,int interval);
	
	//��½�ǳ���Ϣ
	vector <CGlobalStruct::TFLV> LoginInfo(char * ServerName, char * ServerIP,char * UserName, char * UserNick,char * IP,char * begintime, char * endtime,int iIndex, int iPageSize);
	


};
#endif