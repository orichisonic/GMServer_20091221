// BanInfo.cpp: implementation of the CBanInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BanInfo.h"
#include <string>
#include <vector>
using namespace std;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBanInfo::CBanInfo()
{

}

CBanInfo::~CBanInfo()
{

}
bool CBanInfo::Ban_Open_Query(int iType,int userByID,int GameID,char * ServerIP, char * ServerName, char* UserID,char * UserName,
							  char* NickID,char * UserNick,char * StartTime,char * EndTime,char * Reason, char* memo)//����˺�
{
	char MessageLog[1024];
	memset(MessageLog, 0, 1024);
	
	try
	{
		char path[256];
		GetCurrentDirectory(256,path);//��ȡ��ǰĿ¼
		char filepath[256];
		sprintf(filepath,"%s%s",path,"\\config\\scheme.ini");//BanServer.ini�ļ�·��
		char remoteIP[40];
		GetPrivateProfileString("SERVER","RemoteIP","127.0.0.1",remoteIP,40,filepath);

		if(!strcmp(UserName,"")||!strcmp(UserNick,"")||!strcmp(UserName,"NULL")||!strcmp(UserNick,"NULL"))
		{
			QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
		}
		if(!strcmp(UserName,"")||!strcmp(UserNick,""))
		{
			sprintf(MessageLog, "<��Ϸ��>%d, <����>%s[%s], <�ʺ�>%s, <��ɫ>%s, �ʺ���Ϣ������", GameID, ServerName, ServerIP, UserName, UserNick);
			logFile.WriteText("Ban", MessageLog);

			operBan.BuildTLV("�˺Ų�����");
			operBan.SendBuf(pGameSess,CEnumCore::Msg_Category::Ban_ADMIN,CEnumCore::ServiceKey::Ban_Open_Query_Resp,
				operBan.GetSendBuf(),operBan.GetSendLength());
			return TRUE;
		}

		vector <CGlobalStruct::TFLV> DBTFLV;
		if(!strcmp(UserName,""))
		{
			DBTFLV=GameDBQuery("Ban","127.0.0.1",4,0,0,"exec Ban_Account %i,'%s','%s',0,'%s','%s','%s','%s'",GameID,ServerName,UserNick,memo,StartTime,EndTime,remoteIP);
		}
		else
		{
			DBTFLV=GameDBQuery("Ban","127.0.0.1",4,0,0,"exec Ban_Account %i,'%s','%s',1,'%s','%s','%s','%s'",GameID,ServerName,UserName,memo,StartTime,EndTime,remoteIP);
		}
		if(DBTFLV.empty())
		{
			sprintf(MessageLog, "<��Ϸ��>%d, <����>%s[%s], <�ʺ�>%s, <��ɫ>%s, �˺�δ��ͣ", GameID, ServerName, ServerIP, UserName, UserNick);
			logFile.WriteText("Ban", MessageLog);

			operBan.BuildTLV("�˺�δ��ͣ");
			operBan.SendBuf(pGameSess,CEnumCore::Msg_Category::Ban_ADMIN,CEnumCore::ServiceKey::Ban_Open_Query_Resp,
				operBan.GetSendBuf(),operBan.GetSendLength());
			return TRUE;
		}
		int iResult=OpenOper(iType,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,StartTime,EndTime,Reason,memo);
		if(iResult>0)
		{
			sprintf(MessageLog, "<��Ϸ��>%d, <����>%s[%s], <�ʺ�>%s, <��ɫ>%s, ���ɹ�", GameID, ServerName, ServerIP, UserName, UserNick);
			logFile.WriteText("Ban", MessageLog);

			operBan.BuildTLV("���ɹ�");
			operBan.SendBuf(pGameSess,CEnumCore::Msg_Category::Ban_ADMIN,CEnumCore::ServiceKey::Ban_Open_Query_Resp,
				operBan.GetSendBuf(),operBan.GetSendLength());
		}
		else
		{
			sprintf(MessageLog, "<��Ϸ��>%d, <����>%s[%s], <�ʺ�>%s, <��ɫ>%s, ���ʧ��(����)", GameID, ServerName, ServerIP, UserName, UserNick);
			logFile.WriteText("Ban", MessageLog);

			operBan.BuildTLV("���ʧ��");
			operBan.SendBuf(pGameSess,CEnumCore::Msg_Category::Ban_ADMIN,CEnumCore::ServiceKey::Ban_Open_Query_Resp,
				operBan.GetSendBuf(),operBan.GetSendLength());
		}
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("Ban","�������ڴ治��");
		return false;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("Ban","�������ļ��쳣");
		return false;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("Ban","�����������쳣");
		return false;
	}
	catch (...)
	{
		sprintf(MessageLog, "<��Ϸ��>%d, <����>%s[%s], <�ʺ�>%s, <��ɫ>%s, ���ʧ��(�쳣\δ��ͣ)", GameID, ServerName, ServerIP, UserName, UserNick);
		logFile.WriteText("Ban", MessageLog);

		operBan.BuildTLV("�˺�δ��ͣ");
		operBan.SendBuf(pGameSess,CEnumCore::Msg_Category::Ban_ADMIN,CEnumCore::ServiceKey::Ban_Open_Query_Resp,
			operBan.GetSendBuf(),operBan.GetSendLength());
		return false;
	}
	return TRUE;
}

bool CBanInfo::Ban_Close_Query(int iType,int userByID,int GameID,char * ServerIP, char * ServerName, char*  UserID,char * UserName,
							  char*  NickID,char * UserNick,char * StartTime,char * EndTime,char * Reason, char* memo)//��ͣ�˺�
{
	char MessageLog[1024];
	memset(MessageLog, 0, 1024);
	
	try
	{
		char path[256];
		GetCurrentDirectory(256,path);//��ȡ��ǰĿ¼
		char filepath[256];
		sprintf(filepath,"%s%s",path,"\\config\\scheme.ini");//BanServer.ini�ļ�·��
		char remoteIP[40];
		GetPrivateProfileString("SERVER","RemoteIP","127.0.0.1",remoteIP,40,filepath);

		if(!strcmp(UserName,"")||!strcmp(UserNick,"")||!strcmp(UserName,"NULL")||!strcmp(UserNick,"NULL"))
		{
			QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
		}
		if(!strcmp(UserName,"")||!strcmp(UserNick,"")||!strcmp(UserName,"NULL")||!strcmp(UserNick,"NULL"))
		{
			sprintf(MessageLog, "<��Ϸ��>%d, <����>%s[%s], <�ʺ�>%s, <��ɫ>%s, �ʺ���Ϣ������", GameID, ServerName, ServerIP, UserName, UserNick);
			logFile.WriteText("Ban", MessageLog);

			operBan.BuildTLV("�˺Ų�����");
			operBan.SendBuf(pGameSess,CEnumCore::Msg_Category::Ban_ADMIN,CEnumCore::ServiceKey::Ban_Close_Query_Resp,
				operBan.GetSendBuf(),operBan.GetSendLength());
			return TRUE;
		}
		
		vector <CGlobalStruct::TFLV> DBTFLV;
		if(GameID==25&&!strcmp(memo,"0"))//ħ�������ǳƷ�ͣ
		{
			DBTFLV=GameDBQuery("Ban","127.0.0.1",4,0,0,"exec Ban_Account %i,'%s','%s',0,'%s','%s','%s','%s'",GameID,ServerName,UserNick,memo,StartTime,EndTime,remoteIP);
		}
		else
		{
			if(!strcmp(UserName,""))
			{
				DBTFLV=GameDBQuery("Ban","127.0.0.1",4,0,0,"exec Ban_Account %i,'%s','%s',0,'%s','%s','%s','%s'",GameID,ServerName,UserNick,memo,StartTime,EndTime,remoteIP);
			}
			else
			{
				DBTFLV=GameDBQuery("Ban","127.0.0.1",4,0,0,"exec Ban_Account %i,'%s','%s',1,'%s','%s','%s','%s'",GameID,ServerName,UserName,memo,StartTime,EndTime,remoteIP);
			}
		}
		
		if(!DBTFLV.empty())
		{
			sprintf(MessageLog, "<��Ϸ��>%d, <����>%s[%s], <�ʺ�>%s, <��ɫ>%s, �˺��ѷ�ͣ", GameID, ServerName, ServerIP, UserName, UserNick);
			logFile.WriteText("Ban", MessageLog);

			operBan.BuildTLV("�˺��ѷ�ͣ");
			operBan.SendBuf(pGameSess,CEnumCore::Msg_Category::Ban_ADMIN,CEnumCore::ServiceKey::Ban_Close_Query_Resp,
				operBan.GetSendBuf(),operBan.GetSendLength());
			return TRUE;	
		}

		int iResult=RemoteTest(GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick);
		if(iResult<1)
		{
			sprintf(MessageLog, "<��Ϸ��>%d, <����>%s[%s], <�ʺ�>%s, <��ɫ>%s, Զ���ѷ�ͣ", GameID, ServerName, ServerIP, UserName, UserNick);
			logFile.WriteText("Ban", MessageLog);

			operBan.BuildTLV("Զ���ѷ�ͣ");
			operBan.SendBuf(pGameSess,CEnumCore::Msg_Category::Ban_ADMIN,CEnumCore::ServiceKey::Ban_Close_Query_Resp,
				operBan.GetSendBuf(),operBan.GetSendLength());
		}
		iResult=CloseOper(iType,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,StartTime,EndTime,Reason,memo);
		if(iResult>0)
		{
			sprintf(MessageLog, "<��Ϸ��>%d, <����>%s[%s], <�ʺ�>%s, <��ɫ>%s, ��ͣ�ɹ�", GameID, ServerName, ServerIP, UserName, UserNick);
			logFile.WriteText("Ban", MessageLog);

			operBan.BuildTLV("��ͣ�ɹ�");
			operBan.SendBuf(pGameSess,CEnumCore::Msg_Category::Ban_ADMIN,CEnumCore::ServiceKey::Ban_Close_Query_Resp,
				operBan.GetSendBuf(),operBan.GetSendLength());
		}
		else
		{
			sprintf(MessageLog, "<��Ϸ��>%d, <����>%s[%s], <�ʺ�>%s, <��ɫ>%s, ��ͣʧ��(����)", GameID, ServerName, ServerIP, UserName, UserNick);
			logFile.WriteText("Ban", MessageLog);

			operBan.BuildTLV("��ͣʧ��");
			operBan.SendBuf(pGameSess,CEnumCore::Msg_Category::Ban_ADMIN,CEnumCore::ServiceKey::Ban_Close_Query_Resp,
				operBan.GetSendBuf(),operBan.GetSendLength());
		}
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("Ban","��ͣ�����ڴ治��");
		return false;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("Ban","��ͣ�����ļ��쳣");
		return false;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("Ban","��ͣ���������쳣");
		return false;
	}
	catch (...)
	{
		sprintf(MessageLog, "<��Ϸ��>%d, <����>%s[%s], <�ʺ�>%s, <��ɫ>%s, ��ͣʧ��(�쳣)", GameID, ServerName, ServerIP, UserName, UserNick);
		logFile.WriteText("Ban", MessageLog);

		operBan.BuildTLV("��ͣʧ��");
		operBan.SendBuf(pGameSess,CEnumCore::Msg_Category::Ban_ADMIN,CEnumCore::ServiceKey::Ban_Close_Query_Resp,
				operBan.GetSendBuf(),operBan.GetSendLength());
		return false;
	}
	return TRUE;
}

bool CBanInfo::Ban_AccountList_Query(int GameID,char * ServerName,char * beginTime,char * endTime,int iIndex,int iPageSize)//�˺��б��ѯ
{
	try
	{
		char path[256];
		GetCurrentDirectory(256,path);//��ȡ��ǰĿ¼
		char filepath[256];
		sprintf(filepath,"%s%s",path,"\\config\\scheme.ini");//BanServer.ini�ļ�·��
		char remoteIP[40];
		GetPrivateProfileString("SERVER","RemoteIP","127.0.0.1",remoteIP,40,filepath);
		vector <CGlobalStruct::TFLV> DBTFLV=GameDBQuery("Ban","127.0.0.1",4,iIndex,iPageSize,"exec Ban_AccountList %i,'%s','%s','%s','%s'",GameID,ServerName,beginTime,endTime,remoteIP);
		if(!DBTFLV.empty())
		{
			operBan.BuildTLV(COperVector(DBTFLV));
			operBan.SendBuf(pGameSess,CEnumCore::Msg_Category::Ban_ADMIN,CEnumCore::ServiceKey::Ban_AccountList_Query_Resp,
				operBan.GetSendBuf(),operBan.GetSendLength());
		}
		else
		{
			operBan.BuildTLV("��ѯ���ݿ�Ϊ��");
			operBan.SendBuf(pGameSess,CEnumCore::Msg_Category::Ban_ADMIN,CEnumCore::ServiceKey::Ban_AccountList_Query_Resp,
				operBan.GetSendBuf(),operBan.GetSendLength());

		}
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("Ban","��ѯ�˺��б��ڴ治��");
		return false;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("Ban","��ѯ�˺��б��ļ��쳣");
		return false;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("Ban","��ѯ�˺��б������쳣");
		return false;
	}
	catch (...)
	{
		operBan.BuildTLV("��ѯ���ݿ�Ϊ��");
		operBan.SendBuf(pGameSess,CEnumCore::Msg_Category::Ban_ADMIN,CEnumCore::ServiceKey::Ban_AccountList_Query_Resp,
				operBan.GetSendBuf(),operBan.GetSendLength());
		return false;
	}
	return TRUE;
}

bool CBanInfo::Ban_Account_Query(int GameID,char * ServerName,char * UserName, char * UserNick,char* memo,char * beginTime,char * endTime)//�˺Ų�ѯ
{
	try
	{
		char path[256];
		GetCurrentDirectory(256,path);//��ȡ��ǰĿ¼
		char filepath[256];
		sprintf(filepath,"%s%s",path,"\\config\\scheme.ini");//BanServer.ini�ļ�·��
		char remoteIP[40];
		GetPrivateProfileString("SERVER","RemoteIP","127.0.0.1",remoteIP,40,filepath);
	
		//Added by tengjie 2010-05-18
		string strTemp = AnalysisEndTime(endTime);
		

		vector <CGlobalStruct::TFLV> DBTFLV;
		if(!strcmp(UserName,""))
		{
			DBTFLV=GameDBQuery("Ban","127.0.0.1",4,0,0,"exec Ban_Account %i,'%s','%s',0,'%s','%s','%s','%s'",GameID,ServerName,UserNick,memo,beginTime,strTemp.c_str(),remoteIP);
		}
		else
		{
			DBTFLV=GameDBQuery("Ban","127.0.0.1",4,0,0,"exec Ban_Account %i,'%s','%s',1,'%s','%s','%s','%s'",GameID,ServerName,UserName,memo,beginTime,strTemp.c_str(),remoteIP);
		}
		if(!DBTFLV.empty())
		{
			operBan.BuildTLV(COperVector(DBTFLV));
			operBan.SendBuf(pGameSess,CEnumCore::Msg_Category::Ban_ADMIN,CEnumCore::ServiceKey::Ban_Account_Query_Resp,
				operBan.GetSendBuf(),operBan.GetSendLength());
		}
		else
		{
			operBan.BuildTLV("��ѯ���ݿ�Ϊ��");
			operBan.SendBuf(pGameSess,CEnumCore::Msg_Category::Ban_ADMIN,CEnumCore::ServiceKey::Ban_Account_Query_Resp,
				operBan.GetSendBuf(),operBan.GetSendLength());
			
		}
	

	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("Ban","�����˺Ų�ѯ�ڴ治��");
		return false;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("Ban","�����˺Ų�ѯ�ļ��쳣");
		return false;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("Ban","�����˺Ų�ѯ�����쳣");
		return false;
	}
	catch (...)
	{
		operBan.BuildTLV("��ѯ���ݿ�Ϊ��");
		operBan.SendBuf(pGameSess,CEnumCore::Msg_Category::Ban_ADMIN,CEnumCore::ServiceKey::Ban_Account_Query_Resp,
				operBan.GetSendBuf(),operBan.GetSendLength());
		return false;
	}
	return TRUE;
}

string CBanInfo::AnalysisEndTime(char *endTime)
{
	try
	{
		string strTemp = "";
		CString szTime = "";
		char *pTempTime = endTime;
		char *pEndTime  = endTime + strlen(endTime);
		char pStrYear[5];
		char pStrMon[3];
		char pStrDay[3];
		ZeroMemory(pStrYear, 5);
		ZeroMemory(pStrMon,  5);
		ZeroMemory(pStrDay,  5);
		int  iNum = 0;
		while(endTime < pEndTime)
		{
			if(*endTime == '-' && iNum == 0)
			{
				iNum = endTime - pTempTime;
				memcpy(pStrYear, pTempTime, iNum);
				endTime++;
				iNum = 1;
				pTempTime = endTime;
				continue;
			}
			if(*endTime == '-' && iNum == 1)
			{
				iNum = endTime - pTempTime;
				memcpy(pStrMon, pTempTime, iNum);
				endTime++;
				pTempTime = endTime;
				continue;
			}
			else if(*endTime == ' ')
			{
				iNum = endTime - pTempTime;
				memcpy(pStrDay, pTempTime, iNum);
				switch(atoi(pStrMon))
				{
				case 1:
				case 3:
				case 5:
				case 7:
				case 8:
				case 10:
				case 12:
					if(atoi(pStrDay) + 1 > 31)
					{
						 strcpy(pStrDay, "01");
						 if(atoi(pStrMon) + 1 > 12)
						 {
							strcpy(pStrMon, "01");
							szTime.Format("%d", atoi(pStrYear) + 1);
							sprintf(pTempTime, "%s-0%s-%s 00:00:00", szTime.GetBuffer(0), pStrMon, pStrDay);
							strTemp = pTempTime;
						 }
						 else
						 {
							 szTime.Format("%d", atoi(pStrMon) + 1);
							 sprintf(pTempTime, "%s-0%s-%s 00:00:00", pStrYear, szTime.GetBuffer(0), pStrDay);
							 strTemp = pTempTime;
						 }
					}
					else
					{
						szTime.Format("%d", atoi(pStrDay) + 1);
						sprintf(pTempTime, "%s-%s-%s 00:00:00", pStrYear, pStrMon, szTime.GetBuffer(0));
						strTemp = pTempTime;
					}
					break;
				case 2:
					if(atoi(pStrDay) + 1 > 28)
					{
						strcpy(pStrDay, "01");
						if(atoi(pStrMon) + 1 > 12)
						{
							strcpy(pStrMon, "01");
							szTime.Format("%d", atoi(pStrYear) + 1);
							sprintf(pTempTime, "%s-0%s-%s 00:00:00", szTime.GetBuffer(0), pStrMon, pStrDay);
							strTemp = pTempTime;
						}
						else
						{
							szTime.Format("%d", atoi(pStrMon) + 1);
							sprintf(pTempTime, "%s-0%s-%s 00:00:00", pStrYear, szTime.GetBuffer(0), pStrDay);
							strTemp = pTempTime;
						}
					}
					else
					{
						szTime.Format("%d", atoi(pStrDay) + 1);
						sprintf(pTempTime, "%s-%s-%s 00:00:00", pStrYear, pStrMon, szTime.GetBuffer(0));
						strTemp = pTempTime;
					}
					break;
				case 4:
				case 6:
				case 9:
				case 11:
					if(atoi(pStrDay) + 1 > 30)
					{
						strcpy(pStrDay, "01");
						if(atoi(pStrMon) + 1 > 12)
						{
							strcpy(pStrMon, "01");
							szTime.Format("%d", atoi(pStrYear) + 1);
							sprintf(pTempTime, "%s-0%s-%s 00:00:00", szTime.GetBuffer(0), pStrMon, pStrDay);
							strTemp = pTempTime;
						}
						else
						{
							szTime.Format("%d", atoi(pStrMon) + 1);
							sprintf(pTempTime, "%s-0%s-%s 00:00:00", pStrYear, szTime.GetBuffer(0), pStrDay);
							strTemp = pTempTime;
						}
					}
					else
					{
						szTime.Format("%d", atoi(pStrDay) + 1);
						sprintf(pTempTime, "%s-%s-%s 00:00:00", pStrYear, pStrMon, szTime.GetBuffer(0));
						strTemp = pTempTime;
					}
					break;
				default:
					break;
				}
				break;
			}
			else
			{
				endTime++;
				continue;
			}
		}

		return strTemp;
	}
	catch (...)
	{
		return("");
	}
}