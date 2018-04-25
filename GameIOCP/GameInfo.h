// GameInfo.h: interface for the CGameInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEINFO_H__8BCCAF94_B083_4949_89D5_63C4D992DFEE__INCLUDED_)
#define AFX_GAMEINFO_H__8BCCAF94_B083_4949_89D5_63C4D992DFEE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OperatorGame.h"
#include "MD5.h"

class __declspec(dllexport) CGameInfo  
{
public:

	CGameInfo();
	virtual ~CGameInfo();

	virtual COperVector * initialize(CSession * pSess,CEnumCore::Message_Tag_ID MessageTagID, unsigned char * pbuf, int length);

	//����SQL����ѯ���ݿ⣬�õ����ݽṹ�����Լ�TFLV����
	virtual int	  GameDBOper(char * GameName,char * ServerIP, int flag,const char * szFormat,...);
	virtual	vector<CGlobalStruct::TFLV> GameDBQuery(char * GameName,char * ServerIP, int flag,int index,int iPageSize,const char * szFormat,...);

	COperVector * GetDBVect();

	int GetDBLength();

	bool Destroy();

	///////////���ܽӿ�////////////////
	//�û���Ϣ��ѯ
	virtual vector <CGlobalStruct::TFLV> UserInfo(char * GameName,char * sql_type1, char * sql_type2,char * ServerName, 
											char * ServerIP,char * UserName, char * UserNick,int flag,int iIndex,int iPageSize);

	vector<CGlobalStruct::TFLV> UserDefault(char * GameName,char * sql_type,char * ServerName, char * ServerIP,char * UserName, int UserID,int flag,int iIndex,int iPageSize);
	//�����ʺ�����
	virtual vector<CGlobalStruct::TFLV>  KickPlayer(int userByID,char * ServerName, char * ServerIP,char * UserName);
	//�����ʺŷ�ͣ
	virtual vector<CGlobalStruct::TFLV>  ClosePlayer(char * GameName,char * sql1,char * ServerIP,int flag,char * sql2, char* sql3);
	//�����ʺŽ��
	virtual vector<CGlobalStruct::TFLV>  OpenPlayer(char * GameName,char * sql1,char * ServerIP,int flag,char * sql2, char* sql3);
	//��ѯ��ͣ�б�
	virtual vector<CGlobalStruct::TFLV>  CloseList(char * GameName,char * ServerName,char * ServerIP,char * sql,int iIndex, int iPageSize);
	//��ѯ�����˺�
	virtual vector<CGlobalStruct::TFLV>  CloseSingle(char * GameName,char * sql,char * ServerName, char * ServerIP,char * UserName,int iIndex, int iPageSize);
	//��ѯ����
	virtual vector<CGlobalStruct::TFLV>  BroadTask_Query(char * GameName,char * m_sql,int iIndex,int iPageSize);
	//���¹���
	virtual vector<CGlobalStruct::TFLV>  BroadTask_Update(char * GameName,char * sql,int userByID,int taskid,char * boardMessage,char * begintime,char * endtime,int interval,int status);
	//���빫��
	virtual vector<CGlobalStruct::TFLV>  BroadTask_Insert(char * GameName,char * sql,int userByID,char * ServerIP,char * GSServerIP,char * boardMessage,char * begintime,char * endtime,int interval);

	//����ǳ���¼
	virtual vector<CGlobalStruct::TFLV>  LoginInfo(char * ServerName, char * ServerIP,char * UserName, char * UserNick,char * IP,char * begintime, char * endtime,int iIndex, int iPageSize);
	virtual vector<CGlobalStruct::TFLV> BaseLog(char * GameName,char * sql_type,char * ServerName, char * ServerIP,char* UserNick,int UserID,int type,char * szStartTime,char * szEndTime,int flag,int iIndex,int iPageSize);

//	string UserLogInterface(char *ServerIP,string NewUserName,string NewTime, int gameID, string BanType = "0", string QueryType = "0");
	string UserLogInterface(char *ServerIP,string NewUserName,string NewTime, int gameID, string BanType = "0");

	//md5����
	string md5_PassWord(char *PassWord);
public:
	
	COperVector sendopervector;

	char m_operszSql[2048];

	CLogFile logFile;
	char errMessage[256];

	//�յ������ݽṹ��
	COperVector recvVect;
	
	char  operName[256];

	//����IP
	char LocalDB[40];

	int sendDBLength;

	CSession *pGameSess;

	CMD5 md5_OOp;
};

#endif // !defined(AFX_GAMEINFO_H__8BCCAF94_B083_4949_89D5_63C4D992DFEE__INCLUDED_)
