// GMAPI.h: interface for the CGMAPI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GMAPI_H__E47D161E_8853_42BB_A347_AB885BABEC4B__INCLUDED_)
#define AFX_GMAPI_H__E47D161E_8853_42BB_A347_AB885BABEC4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#pragma warning(disable: 4786)   //��������255����
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
	//�ж��Ƿ��½�ɹ�
	void IsLogin();

	//ģ����û���ϵ
	void User_Module_Query();

	//��ѯ�û��б�
	void User_Query_All();

	//�鿴������Ϸ������
	void  Server_IP_Query();

	//�鿴������Ϸ������
	void ServerInfo_IP_All_Query();

	//����GM�ʺŵĹ�������
	void Gmtools_UserMac_Update();

	//�����û�
	void User_Create();

	
	//�����û�
	void User_Update();

	//ɾ���û�
	void User_Delete();


	//�޸�����
	void User_Passwd_Modif();


	//��ѯ�����б�
	void Depart_Query();


	//�鿴���߲�����¼
	void Gmtools_OperateLog_Query();

	//��Ϸ��ѯ
	void Game_Query();

	//��ѯ��Ϸ��ģ���б�
	void Game_Module_Query();

	//����ģ��
	void User_Module_Update();

	//�ļ��Ƚ�
	void GM_FileCompare();

	//�ļ�����
    void GM_FileUpdate();


	//��ȡUserName,PASSWORD,User_State,MAC,
	bool GetUserInfo(char * username,char * passwd,char * mac,int *user_state);

	//Added by tengjie 2010-05-05:��ҽ�����Ϣ��ѯ
	void Gm_HarmFulProc_Query();

	//��ѯ�ʺ��Ƿ�Ϊvip�û�
	void GM_Net_vipAccountInfo(char * UserName);

	//��������
	BOOL GM_Net_KickPlayer(int userByID,char * ServerIP, char * UserName, char * Character);

	//������ͣ�ʺ�
	void GM_Net_ClosePlayerGroup(int userByID,char * ServerIP, char * UserName,int CharacterID,char * Character,int iTime, int Reason, char *description);

	void GM_SqlForCloseUserInfo(char *UserName,char *ServerIP,int iTime,char *description,int iStatus);

	CGMInfo();
	virtual ~CGMInfo();
private:
	//���ܺ��MD5
	char szMD5Password[256];

	//SQL���
	char sql[4096];
	
	//�û���
	char m_username[256];

	//�ļ������
	CFileUpdate FUpdate;

	//������Ϣ
	char retMessage[256];

	//�����м�ĸ�������
	COperatorGM operGM;
};

#endif // !defined(AFX_GMAPI_H__E47D161E_8853_42BB_A347_AB885BABEC4B__INCLUDED_)
