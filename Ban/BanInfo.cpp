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
							  char* NickID,char * UserNick,char * StartTime,char * EndTime,char * Reason, char* memo)//解封账号
{
	char MessageLog[1024];
	memset(MessageLog, 0, 1024);
	
	try
	{
		char path[256];
		GetCurrentDirectory(256,path);//获取当前目录
		char filepath[256];
		sprintf(filepath,"%s%s",path,"\\config\\scheme.ini");//BanServer.ini文件路径
		char remoteIP[40];
		GetPrivateProfileString("SERVER","RemoteIP","127.0.0.1",remoteIP,40,filepath);

		if(!strcmp(UserName,"")||!strcmp(UserNick,"")||!strcmp(UserName,"NULL")||!strcmp(UserNick,"NULL"))
		{
			QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
		}
		if(!strcmp(UserName,"")||!strcmp(UserNick,""))
		{
			sprintf(MessageLog, "<游戏号>%d, <大区>%s[%s], <帐号>%s, <角色>%s, 帐号信息不存在", GameID, ServerName, ServerIP, UserName, UserNick);
			logFile.WriteText("Ban", MessageLog);

			operBan.BuildTLV("账号不存在");
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
			sprintf(MessageLog, "<游戏号>%d, <大区>%s[%s], <帐号>%s, <角色>%s, 账号未封停", GameID, ServerName, ServerIP, UserName, UserNick);
			logFile.WriteText("Ban", MessageLog);

			operBan.BuildTLV("账号未封停");
			operBan.SendBuf(pGameSess,CEnumCore::Msg_Category::Ban_ADMIN,CEnumCore::ServiceKey::Ban_Open_Query_Resp,
				operBan.GetSendBuf(),operBan.GetSendLength());
			return TRUE;
		}
		int iResult=OpenOper(iType,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,StartTime,EndTime,Reason,memo);
		if(iResult>0)
		{
			sprintf(MessageLog, "<游戏号>%d, <大区>%s[%s], <帐号>%s, <角色>%s, 解封成功", GameID, ServerName, ServerIP, UserName, UserNick);
			logFile.WriteText("Ban", MessageLog);

			operBan.BuildTLV("解封成功");
			operBan.SendBuf(pGameSess,CEnumCore::Msg_Category::Ban_ADMIN,CEnumCore::ServiceKey::Ban_Open_Query_Resp,
				operBan.GetSendBuf(),operBan.GetSendLength());
		}
		else
		{
			sprintf(MessageLog, "<游戏号>%d, <大区>%s[%s], <帐号>%s, <角色>%s, 解封失败(操作)", GameID, ServerName, ServerIP, UserName, UserNick);
			logFile.WriteText("Ban", MessageLog);

			operBan.BuildTLV("解封失败");
			operBan.SendBuf(pGameSess,CEnumCore::Msg_Category::Ban_ADMIN,CEnumCore::ServiceKey::Ban_Open_Query_Resp,
				operBan.GetSendBuf(),operBan.GetSendLength());
		}
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("Ban","解封操作内存不足");
		return false;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("Ban","解封操作文件异常");
		return false;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("Ban","解封操作其他异常");
		return false;
	}
	catch (...)
	{
		sprintf(MessageLog, "<游戏号>%d, <大区>%s[%s], <帐号>%s, <角色>%s, 解封失败(异常\未封停)", GameID, ServerName, ServerIP, UserName, UserNick);
		logFile.WriteText("Ban", MessageLog);

		operBan.BuildTLV("账号未封停");
		operBan.SendBuf(pGameSess,CEnumCore::Msg_Category::Ban_ADMIN,CEnumCore::ServiceKey::Ban_Open_Query_Resp,
			operBan.GetSendBuf(),operBan.GetSendLength());
		return false;
	}
	return TRUE;
}

bool CBanInfo::Ban_Close_Query(int iType,int userByID,int GameID,char * ServerIP, char * ServerName, char*  UserID,char * UserName,
							  char*  NickID,char * UserNick,char * StartTime,char * EndTime,char * Reason, char* memo)//封停账号
{
	char MessageLog[1024];
	memset(MessageLog, 0, 1024);
	
	try
	{
		char path[256];
		GetCurrentDirectory(256,path);//获取当前目录
		char filepath[256];
		sprintf(filepath,"%s%s",path,"\\config\\scheme.ini");//BanServer.ini文件路径
		char remoteIP[40];
		GetPrivateProfileString("SERVER","RemoteIP","127.0.0.1",remoteIP,40,filepath);

		if(!strcmp(UserName,"")||!strcmp(UserNick,"")||!strcmp(UserName,"NULL")||!strcmp(UserNick,"NULL"))
		{
			QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
		}
		if(!strcmp(UserName,"")||!strcmp(UserNick,"")||!strcmp(UserName,"NULL")||!strcmp(UserNick,"NULL"))
		{
			sprintf(MessageLog, "<游戏号>%d, <大区>%s[%s], <帐号>%s, <角色>%s, 帐号信息不存在", GameID, ServerName, ServerIP, UserName, UserNick);
			logFile.WriteText("Ban", MessageLog);

			operBan.BuildTLV("账号不存在");
			operBan.SendBuf(pGameSess,CEnumCore::Msg_Category::Ban_ADMIN,CEnumCore::ServiceKey::Ban_Close_Query_Resp,
				operBan.GetSendBuf(),operBan.GetSendLength());
			return TRUE;
		}
		
		vector <CGlobalStruct::TFLV> DBTFLV;
		if(GameID==25&&!strcmp(memo,"0"))//魔力根据昵称封停
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
			sprintf(MessageLog, "<游戏号>%d, <大区>%s[%s], <帐号>%s, <角色>%s, 账号已封停", GameID, ServerName, ServerIP, UserName, UserNick);
			logFile.WriteText("Ban", MessageLog);

			operBan.BuildTLV("账号已封停");
			operBan.SendBuf(pGameSess,CEnumCore::Msg_Category::Ban_ADMIN,CEnumCore::ServiceKey::Ban_Close_Query_Resp,
				operBan.GetSendBuf(),operBan.GetSendLength());
			return TRUE;	
		}

		int iResult=RemoteTest(GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick);
		if(iResult<1)
		{
			sprintf(MessageLog, "<游戏号>%d, <大区>%s[%s], <帐号>%s, <角色>%s, 远程已封停", GameID, ServerName, ServerIP, UserName, UserNick);
			logFile.WriteText("Ban", MessageLog);

			operBan.BuildTLV("远程已封停");
			operBan.SendBuf(pGameSess,CEnumCore::Msg_Category::Ban_ADMIN,CEnumCore::ServiceKey::Ban_Close_Query_Resp,
				operBan.GetSendBuf(),operBan.GetSendLength());
		}
		iResult=CloseOper(iType,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,StartTime,EndTime,Reason,memo);
		if(iResult>0)
		{
			sprintf(MessageLog, "<游戏号>%d, <大区>%s[%s], <帐号>%s, <角色>%s, 封停成功", GameID, ServerName, ServerIP, UserName, UserNick);
			logFile.WriteText("Ban", MessageLog);

			operBan.BuildTLV("封停成功");
			operBan.SendBuf(pGameSess,CEnumCore::Msg_Category::Ban_ADMIN,CEnumCore::ServiceKey::Ban_Close_Query_Resp,
				operBan.GetSendBuf(),operBan.GetSendLength());
		}
		else
		{
			sprintf(MessageLog, "<游戏号>%d, <大区>%s[%s], <帐号>%s, <角色>%s, 封停失败(操作)", GameID, ServerName, ServerIP, UserName, UserNick);
			logFile.WriteText("Ban", MessageLog);

			operBan.BuildTLV("封停失败");
			operBan.SendBuf(pGameSess,CEnumCore::Msg_Category::Ban_ADMIN,CEnumCore::ServiceKey::Ban_Close_Query_Resp,
				operBan.GetSendBuf(),operBan.GetSendLength());
		}
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("Ban","封停操作内存不足");
		return false;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("Ban","封停操作文件异常");
		return false;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("Ban","封停操作其他异常");
		return false;
	}
	catch (...)
	{
		sprintf(MessageLog, "<游戏号>%d, <大区>%s[%s], <帐号>%s, <角色>%s, 封停失败(异常)", GameID, ServerName, ServerIP, UserName, UserNick);
		logFile.WriteText("Ban", MessageLog);

		operBan.BuildTLV("封停失败");
		operBan.SendBuf(pGameSess,CEnumCore::Msg_Category::Ban_ADMIN,CEnumCore::ServiceKey::Ban_Close_Query_Resp,
				operBan.GetSendBuf(),operBan.GetSendLength());
		return false;
	}
	return TRUE;
}

bool CBanInfo::Ban_AccountList_Query(int GameID,char * ServerName,char * beginTime,char * endTime,int iIndex,int iPageSize)//账号列表查询
{
	try
	{
		char path[256];
		GetCurrentDirectory(256,path);//获取当前目录
		char filepath[256];
		sprintf(filepath,"%s%s",path,"\\config\\scheme.ini");//BanServer.ini文件路径
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
			operBan.BuildTLV("查询数据库为空");
			operBan.SendBuf(pGameSess,CEnumCore::Msg_Category::Ban_ADMIN,CEnumCore::ServiceKey::Ban_AccountList_Query_Resp,
				operBan.GetSendBuf(),operBan.GetSendLength());

		}
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("Ban","查询账号列表内存不足");
		return false;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("Ban","查询账号列表文件异常");
		return false;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("Ban","查询账号列表其他异常");
		return false;
	}
	catch (...)
	{
		operBan.BuildTLV("查询数据库为空");
		operBan.SendBuf(pGameSess,CEnumCore::Msg_Category::Ban_ADMIN,CEnumCore::ServiceKey::Ban_AccountList_Query_Resp,
				operBan.GetSendBuf(),operBan.GetSendLength());
		return false;
	}
	return TRUE;
}

bool CBanInfo::Ban_Account_Query(int GameID,char * ServerName,char * UserName, char * UserNick,char* memo,char * beginTime,char * endTime)//账号查询
{
	try
	{
		char path[256];
		GetCurrentDirectory(256,path);//获取当前目录
		char filepath[256];
		sprintf(filepath,"%s%s",path,"\\config\\scheme.ini");//BanServer.ini文件路径
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
			operBan.BuildTLV("查询数据库为空");
			operBan.SendBuf(pGameSess,CEnumCore::Msg_Category::Ban_ADMIN,CEnumCore::ServiceKey::Ban_Account_Query_Resp,
				operBan.GetSendBuf(),operBan.GetSendLength());
			
		}
	

	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("Ban","单个账号查询内存不足");
		return false;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("Ban","单个账号查询文件异常");
		return false;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("Ban","单个账号查询其他异常");
		return false;
	}
	catch (...)
	{
		operBan.BuildTLV("查询数据库为空");
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