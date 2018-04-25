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

	//构造SQL语句查询数据库，得到数据结构集，以及TFLV队列
	virtual int	  GameDBOper(char * GameName,char * ServerIP, int flag,const char * szFormat,...);
	virtual	vector<CGlobalStruct::TFLV> GameDBQuery(char * GameName,char * ServerIP, int flag,int index,int iPageSize,const char * szFormat,...);

	COperVector * GetDBVect();

	int GetDBLength();

	bool Destroy();

	///////////功能接口////////////////
	//用户信息查询
	virtual vector <CGlobalStruct::TFLV> UserInfo(char * GameName,char * sql_type1, char * sql_type2,char * ServerName, 
											char * ServerIP,char * UserName, char * UserNick,int flag,int iIndex,int iPageSize);

	vector<CGlobalStruct::TFLV> UserDefault(char * GameName,char * sql_type,char * ServerName, char * ServerIP,char * UserName, int UserID,int flag,int iIndex,int iPageSize);
	//根据帐号踢人
	virtual vector<CGlobalStruct::TFLV>  KickPlayer(int userByID,char * ServerName, char * ServerIP,char * UserName);
	//根据帐号封停
	virtual vector<CGlobalStruct::TFLV>  ClosePlayer(char * GameName,char * sql1,char * ServerIP,int flag,char * sql2, char* sql3);
	//根据帐号解封
	virtual vector<CGlobalStruct::TFLV>  OpenPlayer(char * GameName,char * sql1,char * ServerIP,int flag,char * sql2, char* sql3);
	//查询封停列表
	virtual vector<CGlobalStruct::TFLV>  CloseList(char * GameName,char * ServerName,char * ServerIP,char * sql,int iIndex, int iPageSize);
	//查询单个账号
	virtual vector<CGlobalStruct::TFLV>  CloseSingle(char * GameName,char * sql,char * ServerName, char * ServerIP,char * UserName,int iIndex, int iPageSize);
	//查询公告
	virtual vector<CGlobalStruct::TFLV>  BroadTask_Query(char * GameName,char * m_sql,int iIndex,int iPageSize);
	//更新公告
	virtual vector<CGlobalStruct::TFLV>  BroadTask_Update(char * GameName,char * sql,int userByID,int taskid,char * boardMessage,char * begintime,char * endtime,int interval,int status);
	//插入公告
	virtual vector<CGlobalStruct::TFLV>  BroadTask_Insert(char * GameName,char * sql,int userByID,char * ServerIP,char * GSServerIP,char * boardMessage,char * begintime,char * endtime,int interval);

	//登入登出记录
	virtual vector<CGlobalStruct::TFLV>  LoginInfo(char * ServerName, char * ServerIP,char * UserName, char * UserNick,char * IP,char * begintime, char * endtime,int iIndex, int iPageSize);
	virtual vector<CGlobalStruct::TFLV> BaseLog(char * GameName,char * sql_type,char * ServerName, char * ServerIP,char* UserNick,int UserID,int type,char * szStartTime,char * szEndTime,int flag,int iIndex,int iPageSize);

//	string UserLogInterface(char *ServerIP,string NewUserName,string NewTime, int gameID, string BanType = "0", string QueryType = "0");
	string UserLogInterface(char *ServerIP,string NewUserName,string NewTime, int gameID, string BanType = "0");

	//md5加密
	string md5_PassWord(char *PassWord);
public:
	
	COperVector sendopervector;

	char m_operszSql[2048];

	CLogFile logFile;
	char errMessage[256];

	//收到的数据结构体
	COperVector recvVect;
	
	char  operName[256];

	//本地IP
	char LocalDB[40];

	int sendDBLength;

	CSession *pGameSess;

	CMD5 md5_OOp;
};

#endif // !defined(AFX_GAMEINFO_H__8BCCAF94_B083_4949_89D5_63C4D992DFEE__INCLUDED_)
