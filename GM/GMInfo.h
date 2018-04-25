// GMAPI.h: interface for the CGMAPI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GMAPI_H__E47D161E_8853_42BB_A347_AB885BABEC4B__INCLUDED_)
#define AFX_GMAPI_H__E47D161E_8853_42BB_A347_AB885BABEC4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#pragma warning(disable: 4786)   //类名超过255警告
#include "shlwapi.h"
#pragma comment(lib,"shlwapi.lib")

#include "md5.h"
#pragma comment(lib,"MD5.lib")

#include "FileUpdate.h"

#include "OperatorGM.h"
#include <Winsock2.h>
#include <iostream>
#include <string>


#include "..\\GameIOCP\\GameInfo.h"

class CGMInfo:public CGameInfo
{
public:
	//判断是否登陆成功
	void IsLogin();

	//模块和用户关系
	void User_Module_Query();

	//查询用户列表
	void User_Query_All();

	//查看所有游戏服务器
	void  Server_IP_Query();

	//查看所有游戏服务器
	void ServerInfo_IP_All_Query();

	//更新GM帐号的过期日期
	void Gmtools_UserMac_Update();

	//创建用户
	void User_Create();

	
	//更新用户
	void User_Update();

	//删除用户
	void User_Delete();


	//修改密码
	void User_Passwd_Modif();


	//查询部门列表
	void Depart_Query();


	//查看工具操作记录
	void Gmtools_OperateLog_Query();

	//游戏查询
	void Game_Query();

	//查询游戏的模块列表
	void Game_Module_Query();

	//分配模块
	void User_Module_Update();

	//文件比较
	void GM_FileCompare();

	//文件更新
    void GM_FileUpdate();


	//获取UserName,PASSWORD,User_State,MAC,
	bool GetUserInfo(char * username,char * passwd,char * mac,int *user_state);

	//Added by tengjie 2010-05-05:外挂进程信息查询
	void Gm_HarmFulProc_Query();

	//查询帐号是否为vip用户
	void GM_Net_vipAccountInfo(char * UserName);

	//踢人下线
	BOOL GM_Net_KickPlayer(int userByID,char * ServerIP, char * UserName, char * Character);

	//批量封停帐号
	void GM_Net_ClosePlayerGroup(int userByID,char * ServerIP, char * UserName,int CharacterID,char * Character,int iTime, int Reason, char *description);

	void GM_SqlForCloseUserInfo(char *UserName,char *ServerIP,int iTime,char *description,int iStatus);

	CGMInfo();
	virtual ~CGMInfo();
private:
	//加密后的MD5
	char szMD5Password[256];

	//SQL语句
	char sql[4096];
	
	//用户名
	char m_username[256];

	//文件相关类
	CFileUpdate FUpdate;

	//返回消息
	char retMessage[256];

	//用于中间的辅助操作
	COperatorGM operGM;
};

#endif // !defined(AFX_GMAPI_H__E47D161E_8853_42BB_A347_AB885BABEC4B__INCLUDED_)
