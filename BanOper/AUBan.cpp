// AUBan.cpp: implementation of the CAUBan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AUBan.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAUBan::CAUBan()
{

}

CAUBan::~CAUBan()
{

}
//����
int CAUBan::KickPlayer(int GameID,char * AU_ServerIP,char* AU_ServerName,char *UserName)
{
	LoadDll();
	char message[512];
	memset(message, 0, 512);

	char strUserNick[256];
	AccountToNick(GameID,AU_ServerIP,AU_ServerName,UserName,strUserNick);
	AU_BanPacket au_BanPack;
	au_BanPack.count=1;
	au_BanPack.major=0x64;
	au_BanPack.minor=0x04;

	sprintf(au_BanPack.nick,"%s",strUserNick);
	sprintf((char *)&au_BanPack.nicklength,"%c",strlen(strUserNick));
	au_BanPack.nick[strlen(strUserNick)]='\0';
	au_BanPack.length=2+4+2+1+strlen(strUserNick);
	char strSend[0x2800];
	int iSend=0;
	void * pcreate=au_create();

	char packet[1024];
	
	ZeroMemory(packet,1024);
	memcpy(packet,(unsigned short int*)&au_BanPack.length,2);
	memcpy(packet+2,(unsigned int*)&au_BanPack.count,4);
	packet[6]=au_BanPack.major;
	packet[7]=au_BanPack.minor;
	packet[8]=au_BanPack.nicklength;
	sprintf(&packet[9],"%s",strUserNick);

	iSend=au_encrypt(pcreate,packet,au_BanPack.length,(char *)&strSend,0x2800);

	

	CMSocket m_socket;
	char strIP[40];

	vector<CGlobalStruct::DataRecord> datarecord;
	if(!datarecord.empty())
	{
		datarecord.clear();
	}
	CSqlHelper m_SqlHelper;
	int res=0;
	operBan.FindDBIP(GameID,AU_ServerIP,AU_ServerName,AU_ServerIP,1);
	sprintf(operszSql,"select gsserverip from au_gsserverip where serverip='%s'",AU_ServerIP);
	m_SqlHelper.SqlHelperMain(&datarecord, "127.0.0.1",operszSql, 4, 0, 0);//��ȡ��Ӧ��LogDBIP
		
	//��ȡ��ѯ�ļ�¼����ÿһ��
	vector<CGlobalStruct::DataRecord>::iterator iter;
	CGlobalStruct::DataRecord  m_DataRecord;	
	for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
	{
		m_DataRecord = *iter;
		if(!strcmp("gsserverip", m_DataRecord.recordfieldname))//��ȡ��Ӧ��LogDBIP
		{
			sprintf(strIP,"%s",m_DataRecord.recorddata);
			m_socket.ConnectURL(strIP,25550);//����Զ��
			res=m_socket.SendNewBuf((char*)&strSend,iSend);
			m_socket.closeSocket();

			sprintf(message, "<����>%s[%s:25550], <�û���>%s, Kick Success!", AU_ServerName, strIP, UserName);
			operBan.OperFileLog("������", message);
		}
		

	}
	if(!datarecord.empty())
	{
		datarecord.clear();
	}

	FreeDll();
	return res;
}

//����˺�
int CAUBan::OpenPlayer(int GameID,char * AU_ServerIP,char* AU_ServerName,char* UserName)
{
	operBan.FindDBIP(GameID,AU_ServerIP,AU_ServerName,AU_ServerIP,2);
	if(!operBan.getRemoteSql("AU_ACCOUNT_OPEN",operRemoteSql,1))
	{
		return false;
	}
	sprintf(operszSql,operRemoteSql,UserName);//����������ѯ��sql���

	operBan.OperFileLog("������", operszSql, false);
	int res=DBVect.OperVectorMain("AU", AU_ServerIP, operszSql, 1);//��������ݿ��ѯ�õ�������
	return res;
}
//��ͣ�˺�
int CAUBan::ClosePlayer(int GameID,char * AU_ServerIP,char* AU_ServerName,char* UserName)
{
	operBan.FindDBIP(GameID,AU_ServerIP,AU_ServerName,AU_ServerIP,2);
	if(!operBan.getRemoteSql("AU_ACCOUNT_CLOSE",operRemoteSql,1))
	{
		return false;
	}
	sprintf(operszSql,operRemoteSql,UserName);//����������ѯ��sql���

	operBan.OperFileLog("������", operszSql, false);
	int res=DBVect.OperVectorMain("AU", AU_ServerIP, operszSql, 1);//��������ݿ��ѯ�õ�������
	return res;
}
//��ѯ��Ϣ
int CAUBan::QueryAccount(int GameID,char * AU_ServerIP,char * AU_ServerName,char *UserName,char * UserNick)
{
	operBan.FindDBIP(GameID,AU_ServerIP,AU_ServerName,AU_ServerIP,3);
	if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
	{
		if(!operBan.getRemoteSql("AU_Nick_Account_Ban",operRemoteSql,1))
		{
			return false;
		}
		sprintf(operszSql,operRemoteSql,UserNick);
		operBan.OperFileLog("������", operszSql, false);
	//	sprintf(operszSql,"select userid from Audition.UserInfo where usernick='%s'",UserNick);
		operBan.QuerySingleValue("AU",AU_ServerIP,1,operszSql,UserName);
	}
	else
	{
		if(!operBan.getRemoteSql("AU_Account_Nick_Ban",operRemoteSql,1))
		{
			return false;
		}
		sprintf(operszSql,operRemoteSql,UserName);
		operBan.OperFileLog("������", operszSql, false);
	//	sprintf(operszSql,"select usernick from Audition.UserInfo where userid='%s'",UserName);
		operBan.QuerySingleValue("AU",AU_ServerIP,1,operszSql,UserNick);
	}
	return 1;
}

int CAUBan::AccountToNick(int GameID,char * AU_ServerIP,char * AU_ServerName,char *UserName,char * UserNick)
{
	operBan.FindDBIP(GameID,AU_ServerIP,AU_ServerName,AU_ServerIP,3);
	if(!operBan.getRemoteSql("AU_Account_Nick",operRemoteSql,1))
	{
		return false;
	}
	sprintf(operszSql,operRemoteSql,UserName);
//	sprintf(operszSql,"select usernick from Audition.UserInfo where userid='%s'",UserName);
	operBan.QuerySingleValue("AU",AU_ServerIP,1,operszSql,UserNick);
	return 1;
}

void CAUBan::LoadDll()
{
	hInst=LoadLibrary("cppPacketEncryptionDLL.dll");
//	Create au_create;
	if(hInst)
	{
		au_create=(Create)GetProcAddress(hInst,"Encryption_Create");//��ȡ�ӿ�
		au_encrypt=(Encrypt)GetProcAddress(hInst,"Encryption_Encrypt");//��ȡ�ӿ�
		
	}
}
void CAUBan::FreeDll()
{
	if(hInst)
		FreeLibrary(hInst);
}