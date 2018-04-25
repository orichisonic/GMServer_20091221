// PalSend.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "SqlHelper.h"
#include "PalSend.h"
#include "LogFile.h"
#include "ASTA.h"
#include "ATAS.h"

using namespace MSG_ASTA;
using namespace MSG_ATAS;

DWORD WINAPI SendThread(LPVOID lptaskid)
{
	try
	{
		int m_taskid=(int)lptaskid;
		WaitForSingleObject(m_SendMutex,INFINITE);
		CSqlHelper mysqlhelper,m_sqlhelper;
		char szSql[256];
		int interval=0,m_iWorldID=0;
		char m_szGMServerIP[40];
		bool m_result=mysqlhelper.ConnectData(szData);	
		if(m_result)
		{
			sprintf(szSql,"exec pal_sendbytaskid %i",m_taskid);
			m_result=mysqlhelper.DoSql(szSql);
			if(m_result)
			{
				while(!mysqlhelper.m_pRecordset->adoEOF)	
				{
					if(mysqlhelper.m_pRecordset->GetFields()->GetItem("serverIP")->Value.vt!=VT_NULL&&
						mysqlhelper.m_pRecordset->GetFields()->GetItem("WorldId")->Value.vt!=VT_NULL)
					{
						wsprintf(m_szGMServerIP,"%s",(LPCSTR)_bstr_t(mysqlhelper.m_pRecordset->GetFields()->GetItem("ServerIP")->GetValue()));
						m_iWorldID=StrToInt((LPCSTR)_bstr_t(mysqlhelper.m_pRecordset->GetFields()->GetItem("WorldId")->GetValue()));
						interval=StrToInt((LPCSTR)_bstr_t(mysqlhelper.m_pRecordset->GetFields()->GetItem("Interval")->GetValue()));
						wsprintf(szContent,"%s",(LPCSTR)_bstr_t(mysqlhelper.m_pRecordset->GetFields()->GetItem("boardMessage")->GetValue()));
						logFile.SetOperator((char *)_bstr_t(mysqlhelper.m_pRecordset->GetFields()->GetItem("username")->GetValue()));
						logFile.WriteText("开始发送公告");
						taskid=m_taskid;
						SendNonscheduleBulletinMain(taskid,m_szGMServerIP,m_iWorldID,szContent,interval*60);
						if(interval==0)
						{
							m_result=m_sqlhelper.ConnectData(szData);
							if(m_result==true)
							{
								wsprintf(szSql,"exec pal_endatonce %i",m_taskid);
								m_sqlhelper.DoSql(szSql);
							}
							m_sqlhelper.CloseData();
						}
					
			

					}
					mysqlhelper.m_pRecordset->MoveNext();
				}
			}
		
		}
		mysqlhelper.CloseData();

		ReleaseMutex(m_SendMutex);
	}
	catch (...)
	{

	}

	return TRUE;
}

//发送JW2的公告
DWORD WINAPI SendJW2Thread(LPVOID lptaskid)
{
	try
	{

		int m_taskid=(int)lptaskid;
		WaitForSingleObject(m_SendJW2Mutex,INFINITE);
		CSqlHelper mysqlhelper,m_sqlhelper;
		char szSql[256];
		int interval=0,m_iWorldID=0;
		char m_szGMServerIP[40];
		bool m_result=mysqlhelper.ConnectData(szData);
		if(m_result)
		{
			sprintf(szSql,"select b.ServerIP,a.boardMessage from jw2_BoardTasker a,jw2_boardMessage b where a.taskid=b.taskid and a.taskid=%i ",m_taskid);
			m_result=mysqlhelper.DoSql(szSql);
			if(m_result)
			{
				while(!mysqlhelper.m_pRecordset->adoEOF)	
				{
					if(mysqlhelper.m_pRecordset->GetFields()->GetItem("serverIP")->Value.vt!=VT_NULL)
					{
						wsprintf(m_szGMServerIP,"%s",(LPCSTR)_bstr_t(mysqlhelper.m_pRecordset->GetFields()->GetItem("ServerIP")->GetValue()));
						wsprintf(szContent,"%s",(LPCSTR)_bstr_t(mysqlhelper.m_pRecordset->GetFields()->GetItem("boardMessage")->GetValue()));
						logFile.WriteText("开始发送公告");
						taskid=m_taskid;
						AnalyseTaskId(taskid,szContent);	
					}
					if(m_sqlhelper.ConnectData(szData))
					{
						sprintf(szSql,"update jw2_BoardTasker set status=1 where atonce=-1 and taskid=%i",m_taskid);
						m_sqlhelper.DoSql(szSql);	
						m_sqlhelper.CloseData();
					}
					mysqlhelper.m_pRecordset->MoveNext();
				}
			}
			
		}
		mysqlhelper.CloseData();
		
		ReleaseMutex(m_SendJW2Mutex);

	}
	catch (...)
	{
		return false;
	}
	return TRUE;
}
//发送勇士无双的公告
DWORD WINAPI SendWAThread(LPVOID lptaskid)
{
	try
	{
		int m_taskid=(int)lptaskid;
		WaitForSingleObject(m_SendWAMutex,INFINITE);
		CSqlHelper mysqlhelper,m_sqlhelper;
		char szSql[256];
		int interval=0,m_iWorldID=0;
		int Port=0;	//发送端口
		char m_szGMServerIP[40];
		
		char szData[256];
		char databaseip[20];    //本地数据库ip
		char databaseUser[20];	//本地数据库用户名
		char database[20];		//本地默认数据库
		char databasepwd[20];	//本地数据库密码
		char szContent[512];
		int Realmid;
		int ClusterID;
		//////////////////读配置文件////////////////////////
		char path[256];
		GetCurrentDirectory(256,path);
		char filepath[256];
		sprintf(filepath,"%s%s",path,".\\config\\PalSend.ini");
		
		
		GetPrivateProfileString("DATABASE","Address","127.0.0.1",databaseip,20,filepath);
		GetPrivateProfileString("DATABASE","User",NULL,databaseUser,20,filepath);
		GetPrivateProfileString("DATABASE","Data",NULL,database,20,filepath);
		GetPrivateProfileString("DATABASE","Pwd",NULL,databasepwd,20,filepath);

		sprintf(szData,"Provider=SQLOLEDB.1;Persist Security Info=True;User ID=%s;Password=%s;Data Source=%s;Initial Catalog=%s",databaseUser,databasepwd,databaseip,database);
		
		
		bool m_result=mysqlhelper.ConnectData(szData);
		
		if(m_result)
		{
			sprintf(szSql,"exec WA_sendbytaskid %i",m_taskid);
			m_result=mysqlhelper.DoSql(szSql);
			if(m_result)
			{
				while(!mysqlhelper.m_pRecordset->adoEOF)	
				{
					if(mysqlhelper.m_pRecordset->GetFields()->GetItem("serverIP")->Value.vt!=VT_NULL)
					{
						sprintf(m_szGMServerIP,"%s",(LPCSTR)_bstr_t(mysqlhelper.m_pRecordset->GetFields()->GetItem("ServerIP")->GetValue()));
						Port=StrToInt((LPCSTR)_bstr_t(mysqlhelper.m_pRecordset->GetFields()->GetItem("serverPort")->GetValue()));
						Realmid=StrToInt((LPCSTR)_bstr_t(mysqlhelper.m_pRecordset->GetFields()->GetItem("Realmid")->GetValue()));
						ClusterID=StrToInt((LPCSTR)_bstr_t(mysqlhelper.m_pRecordset->GetFields()->GetItem("ClusterID")->GetValue()));
						interval=StrToInt((LPCSTR)_bstr_t(mysqlhelper.m_pRecordset->GetFields()->GetItem("Interval")->GetValue()));
						sprintf(szContent,"%s",(LPCSTR)_bstr_t(mysqlhelper.m_pRecordset->GetFields()->GetItem("boardMessage")->GetValue()));
//						printf("公告ID:%d****************大区IP:%s, 端口:%d, 大区id:%d, 组ID:%d\n",m_taskid, m_szGMServerIP, Port, Realmid, ClusterID);
						m_result=WA_SendButtle(m_szGMServerIP, Port, Realmid,ClusterID,szContent);
						if(m_result)
						{
							if(interval==0)
							{
								m_result=m_sqlhelper.ConnectData(szData);
								if(m_result==true)
								{
									wsprintf(szSql,"exec WA_Endatonce %i",m_taskid);
									m_sqlhelper.DoSql(szSql);
								}
								m_sqlhelper.CloseData();
							}
						}
					}
					mysqlhelper.m_pRecordset->MoveNext();
				}
			}
			
		}
		mysqlhelper.CloseData();
		ReleaseMutex(m_SendWAMutex);	
	}
	catch (...)
	{
		return false;
	}

	return TRUE;
}

//add by ltx
//发送侠盗金刚的公告
DWORD WINAPI SendXDJGThread(LPVOID lptaskid)
{
	try
	{
		int m_taskid=(int)lptaskid;
		WaitForSingleObject(m_SendXDJGMutex,INFINITE);
		CSqlHelper mysqlhelper,m_sqlhelper;
		char szSql[256];
		int interval=0,m_iWorldID=0;
		char m_szGMServerIP[40];
		
		char szData[256];
		char databaseip[20];    //本地数据库ip
		char databaseUser[20];	//本地数据库用户名
		char database[20];		//本地默认数据库
		char databasepwd[20];	//本地数据库密码
		char szContent[512];
		//////////////////读配置文件////////////////////////
		char path[256];
		GetCurrentDirectory(256,path);
		char filepath[256];
		sprintf(filepath,"%s%s",path,".\\config\\PalSend.ini");
		
		
		GetPrivateProfileString("DATABASE","Address","127.0.0.1",databaseip,20,filepath);
		GetPrivateProfileString("DATABASE","User",NULL,databaseUser,20,filepath);
		GetPrivateProfileString("DATABASE","Data",NULL,database,20,filepath);
		GetPrivateProfileString("DATABASE","Pwd",NULL,databasepwd,20,filepath);
		
		sprintf(szData,"Provider=SQLOLEDB.1;Persist Security Info=True;User ID=%s;Password=%s;Data Source=%s;Initial Catalog=%s",databaseUser,databasepwd,databaseip,database);
		
		
		bool m_result=mysqlhelper.ConnectData(szData);

		if(m_result)
		{
			sprintf(szSql,"exec XDJG_sendbytaskid %i",m_taskid);
			m_result=mysqlhelper.DoSql(szSql);
			if(m_result)
			{
				while(!mysqlhelper.m_pRecordset->adoEOF)	
				{
					if(mysqlhelper.m_pRecordset->GetFields()->GetItem("serverIP")->Value.vt!=VT_NULL)
					{
						sprintf(m_szGMServerIP,"%s",(LPCSTR)_bstr_t(mysqlhelper.m_pRecordset->GetFields()->GetItem("ServerIP")->GetValue()));
						interval=StrToInt((LPCSTR)_bstr_t(mysqlhelper.m_pRecordset->GetFields()->GetItem("Interval")->GetValue()));
						sprintf(szContent,"%s",(LPCSTR)_bstr_t(mysqlhelper.m_pRecordset->GetFields()->GetItem("boardMessage")->GetValue()));
//						printf("ip:%s  content:%s\n",m_szGMServerIP, szContent);
						m_result=XDJG_SendBulletin(m_szGMServerIP, szContent, 8082);
						if(m_result)
						{
							if(interval==0)
							{
								m_result=m_sqlhelper.ConnectData(szData);
								if(m_result==true)
								{
									wsprintf(szSql,"exec XDJG_Endatonce %i",m_taskid);
									m_sqlhelper.DoSql(szSql);
								}
								m_sqlhelper.CloseData();
							}
						}
					}
					mysqlhelper.m_pRecordset->MoveNext();
				}
			}
			
		}
		mysqlhelper.CloseData();
		ReleaseMutex(m_SendXDJGMutex);
	}
	catch (...)
	{
		return false;
	}			
	return true;
}
//Added by tengjie 2010-08-12
// string Encode(const char* lpszData)
// {
// 	string strResult = "";
// 	unsigned char* pInTmp = (unsigned char*)lpszData;
// 	// do encoding
// 	while (*pInTmp)
// 	{
// 		if(isalnum(*pInTmp))
// 			strResult += *pInTmp;
// 		else
// 			if(isspace(*pInTmp))
// 				strResult += '+';
// 			else
// 			{
// 				strResult += '%';
// 				strResult += toHex(*pInTmp>>4);
// 				strResult += toHex(*pInTmp%16);
// 			}
// 			pInTmp++;
//  }
// 	return strResult;
// }
// 
// //发送神兵的定时公告
// bool SC_SendButtle(char * ServerIP, char * BoardMessage)
// {
// 	CString szTemUrl = "";
// 	CInternetSession MySession("SC_Broadcast");
// 	CHttpFile* myHttpFile = NULL;
// 	CString myData;
// 	CString strBackInfo;
// 	int HttpPort = 60003;
// 	string strEncode = "";
// 
// 	try
// 	{
// 		strEncode = Encode(BoardMessage);
// 
// 		printf("----------%s\n",strEncode.c_str());
// 
// 		szTemUrl.Format("http://%s:%d/rpc_proxy?app_name=ws_chanpin&fun=Broadcast(0,%s)",ServerIP,HttpPort,strEncode.c_str());	
// 
// 		printf("###########%s\n",szTemUrl.GetBuffer(0));
// 		myHttpFile = (CHttpFile *)MySession.OpenURL(szTemUrl.GetBuffer(0), 1, INTERNET_FLAG_TRANSFER_ASCII | INTERNET_FLAG_RELOAD, NULL, 0);
// 		
// 		printf("@@@@@@@@@@@%s\n",myData.GetBuffer(0));
// 		if (myHttpFile==NULL)
// 		{
// 			printf("~~~~~~~~~~~~~~~~~~~~\n");
// 			return false;
// 		}
// 
// 		Sleep(200);
// 
// 		printf("+++++++++++%s\n",myData.GetBuffer(0));
// 		while (myHttpFile->ReadString(myData))
// 		{
// 			strBackInfo += myData;
// 		}
// 
// 		if(!strBackInfo.CompareNoCase("TRUE"))
// 			printf("<神兵传奇>发送公告:[%s]成功\n", BoardMessage);
// 		else
// 		{
// 			printf("<神兵传奇>发送公告:[%s]失败\n", BoardMessage);
// 			return false;
// 		}
// 
// 		MySession.Close();
// 	}
// 	catch(...)
// 	{
// 		return false;
// 	}
// 
// 	return true;
// }
// 
// //Added by tengjie 20100812
// DWORD WINAPI SendSCIPGroupThread(LPVOID lpIpContent)
// {
// 	try
// 	{
// 		Sc_TotalInfo * pTaskInfo=(Sc_TotalInfo *)(lpIpContent);
// 
// 		CSqlHelper mysqlhelper,m_sqlhelper;
// 		int iList=0;
// 		
// 		char szSql[256];
// 
// 		for(iList = 0; iList < pTaskInfo->VecTaskInfo.size(); iList++)
// 		{
// 			bool m_result=SC_SendButtle((char*)pTaskInfo->VecTaskInfo[iList].szIP.c_str(), (char*)pTaskInfo->VecTaskInfo[iList].szContent.c_str());
// 			
// 			if(m_result)
// 			{
// 				if(pTaskInfo->VecTaskInfo[iList].iInterval==0)
// 				{
// 					m_result=m_sqlhelper.ConnectData((char*)pTaskInfo->strData.c_str());
// 					if(m_result==true)
// 					{
// 						wsprintf(szSql,"exec SC_Endatonce %i",pTaskInfo->VecTaskInfo[iList].iTaskID);
// 						m_sqlhelper.DoSql(szSql);
// 					}
// 					m_sqlhelper.CloseData();
// 				}
// 			}
// 		}
// 	}
// 	catch (...)
// 	{
// 		return false;
// 	}
// 
// 	return true;
// }
// 
// //发送神兵的公告
// DWORD WINAPI SendSCThread(LPVOID lptaskid)
// {
// 	try
// 	{
// 		int m_taskid=(int)lptaskid;
// 		WaitForSingleObject(m_SendSCMutex,INFINITE);
// 		CSqlHelper mysqlhelper,m_sqlhelper;
// 		char szSql[256];
// 		int interval=0,m_iWorldID=0;
// 		char m_szGMServerIP[40];
// 
// 		Sc_TotalInfo *pTotalInfo = new Sc_TotalInfo;
// 		if(pTotalInfo == NULL)
// 		{
// 			logFile.WriteText("[SendSCThread:pTotalInfo]动态分配内存失败\n");
// 			return false;
// 		}
// 
// 		Sc_TaskInfo sTaskInfo;
// 		int iTotal = 0;
// 		int iTemp = 0;
// 
// 		char szData[256];
// 		char databaseip[20];    //本地数据库ip
// 		char databaseUser[20];	//本地数据库用户名
// 		char database[20];		//本地默认数据库
// 		char databasepwd[20];	//本地数据库密码
// 		char szContent[512];
// 		//////////////////读配置文件////////////////////////
// 		char path[256];
// 		GetCurrentDirectory(256,path);
// 		char filepath[256];
// 		sprintf(filepath,"%s%s",path,".\\config\\PalSend.ini");
// 		
// 		
// 		GetPrivateProfileString("DATABASE","Address","127.0.0.1",databaseip,20,filepath);
// 		GetPrivateProfileString("DATABASE","User",NULL,databaseUser,20,filepath);
// 		GetPrivateProfileString("DATABASE","Data",NULL,database,20,filepath);
// 		GetPrivateProfileString("DATABASE","Pwd",NULL,databasepwd,20,filepath);
// 		
// 		sprintf(szData,"Provider=SQLOLEDB.1;Persist Security Info=True;User ID=%s;Password=%s;Data Source=%s;Initial Catalog=%s",databaseUser,databasepwd,databaseip,database);
// 		
// 		
// 		bool m_result=mysqlhelper.ConnectData(szData);
// 		
// 		if(m_result)
// 		{
// 			sprintf(szSql,"exec SC_sendbytaskid %i",m_taskid);
// 			m_result=mysqlhelper.DoSql(szSql);
// 			if(m_result)
// 			{
// 				while(!mysqlhelper.m_pRecordset->adoEOF)	
// 				{
// 					if(mysqlhelper.m_pRecordset->GetFields()->GetItem("serverIP")->Value.vt!=VT_NULL)
// 					{
// 						sprintf(m_szGMServerIP,"%s",(LPCSTR)_bstr_t(mysqlhelper.m_pRecordset->GetFields()->GetItem("ServerIP")->GetValue()));
// 						interval=StrToInt((LPCSTR)_bstr_t(mysqlhelper.m_pRecordset->GetFields()->GetItem("Interval")->GetValue()));
// 						sprintf(szContent,"%s",(LPCSTR)_bstr_t(mysqlhelper.m_pRecordset->GetFields()->GetItem("boardMessage")->GetValue()));
// // 						m_result=SC_SendButtle(m_szGMServerIP, szContent);
// // 
// // 						if(m_result)
// // 						{
// // 							if(interval==0)
// // 							{
// // 								m_result=m_sqlhelper.ConnectData(szData);
// // 								if(m_result==true)
// // 								{
// // 									wsprintf(szSql,"exec SC_Endatonce %i",m_taskid);
// // 									m_sqlhelper.DoSql(szSql);
// // 								}
// // 								m_sqlhelper.CloseData();
// // 							}
// // 						}
// 
// 						sTaskInfo.iInterval = interval;
// 						sTaskInfo.iTaskID = m_taskid;
// 						sTaskInfo.szIP = m_szGMServerIP;
// 						sTaskInfo.szContent = szContent;
// 
// 						pTotalInfo->VecTaskInfo.push_back(sTaskInfo);
// 						iTemp++;
// 
// 						if (iTemp == 10)
// 						{
// 							iTemp = 0;
// 							pTotalInfo->iTotalNum = ++iTotal;
// 							pTotalInfo->strData = szData;
// 							CreateThread(NULL,0,SendSCIPGroupThread,(LPVOID)pTotalInfo,0,0);
// 						}
// 
// 					}
// 					mysqlhelper.m_pRecordset->MoveNext();
// 				}
// 			}
// 		}
// 
// 		if(iTotal < 10)
// 		{
// 			pTotalInfo->iTotalNum = ++iTotal;
// 			CreateThread(NULL,0,SendSCIPGroupThread,(LPVOID)pTotalInfo,0,0);
// 		}
// 		
// 		delete(pTotalInfo);
// 		mysqlhelper.CloseData();
// 		ReleaseMutex(m_SendSCMutex);
// 	}
// 	catch (...)
// 	{
// 		return false;
// 	}			
// 	return true;
// }

// DWORD WINAPI SendSCThread(LPVOID lptaskid)
// {
// 	try
// 	{
// 		int m_taskid=(int)lptaskid;
// 		WaitForSingleObject(m_SendSCMutex,INFINITE);
// 		CSqlHelper mysqlhelper,m_sqlhelper;
// 		char szSql[256];
// 		int interval=0,m_iWorldID=0;
// 		char m_szGMServerIP[40];
// 		
// 		char szData[256];
// 		char databaseip[20];    //本地数据库ip
// 		char databaseUser[20];	//本地数据库用户名
// 		char database[20];		//本地默认数据库
// 		char databasepwd[20];	//本地数据库密码
// 		char szContent[512];
// 		//////////////////读配置文件////////////////////////
// 		char path[256];
// 		GetCurrentDirectory(256,path);
// 		char filepath[256];
// 		sprintf(filepath,"%s%s",path,".\\config\\PalSend.ini");
// 		
// 		
// 		GetPrivateProfileString("DATABASE","Address","127.0.0.1",databaseip,20,filepath);
// 		GetPrivateProfileString("DATABASE","User",NULL,databaseUser,20,filepath);
// 		GetPrivateProfileString("DATABASE","Data",NULL,database,20,filepath);
// 		GetPrivateProfileString("DATABASE","Pwd",NULL,databasepwd,20,filepath);
// 		
// 		sprintf(szData,"Provider=SQLOLEDB.1;Persist Security Info=True;User ID=%s;Password=%s;Data Source=%s;Initial Catalog=%s",databaseUser,databasepwd,databaseip,database);
// 		
// 		
// 		bool m_result=mysqlhelper.ConnectData(szData);
// 		
// 		if(m_result)
// 		{
// 			sprintf(szSql,"exec SC_sendbytaskid %i",m_taskid);
// 			m_result=mysqlhelper.DoSql(szSql);
// 			if(m_result)
// 			{
// 				while(!mysqlhelper.m_pRecordset->adoEOF)	
// 				{
// 					if(mysqlhelper.m_pRecordset->GetFields()->GetItem("serverIP")->Value.vt!=VT_NULL)
// 					{
// 						sprintf(m_szGMServerIP,"%s",(LPCSTR)_bstr_t(mysqlhelper.m_pRecordset->GetFields()->GetItem("ServerIP")->GetValue()));
// 						interval=StrToInt((LPCSTR)_bstr_t(mysqlhelper.m_pRecordset->GetFields()->GetItem("Interval")->GetValue()));
// 						sprintf(szContent,"%s",(LPCSTR)_bstr_t(mysqlhelper.m_pRecordset->GetFields()->GetItem("boardMessage")->GetValue()));
// 						printf("m_szGMServerIP:%s, szContent:%s\n",m_szGMServerIP, szContent);
// 						m_result=SC_SendButtle(m_szGMServerIP, szContent);
// 						if(m_result)
// 						{
// 							if(interval==0)
// 							{
// 								m_result=m_sqlhelper.ConnectData(szData);
// 								if(m_result==true)
// 								{
// 									wsprintf(szSql,"exec SC_Endatonce %i",m_taskid);
// 									m_sqlhelper.DoSql(szSql);
// 								}
// 								m_sqlhelper.CloseData();
// 							}
// 						}
// 					}
// 					mysqlhelper.m_pRecordset->MoveNext();
// 				}
// 			}
// 			
// 		}
// 		mysqlhelper.CloseData();
// 		ReleaseMutex(m_SendSCMutex);
// 	}
// 	catch (...)
// 	{
// 		return false;
// 	}			
// 	return true;
// }

void ReadDataBase()
{
	try
	{
		WaitForSingleObject(m_Mutex,INFINITE);
		char szSql[1024];
		int m_taskid;
		bool flag=false;
		int interval = 0;
		
		SYSTEMTIME nowtime;
		GetLocalTime(&nowtime);
		printf("read database,现在时间:%i-%i-%i %i:%i:%i\n",nowtime.wYear,nowtime.wMonth,nowtime.wDay,nowtime.wHour,nowtime.wMinute,nowtime.wSecond);
		
		///////////读数据库///////////////////////////////
		CSqlHelper mysqlhelper, m_sqlhelper;
		bool m_result=mysqlhelper.ConnectData(szData);
		if(m_result==true)
		{
			////////////////查询立即发送-1状态为0的消息////////////////////////////////
			sprintf(szSql,"exec pal_sendatonce");
			m_result=mysqlhelper.DoSql(szSql);
			if(m_result)
			{
				while(!mysqlhelper.m_pRecordset->adoEOF)
				{
					
					m_taskid=StrToInt((LPCSTR)_bstr_t(mysqlhelper.m_pRecordset->GetFields()->GetItem("taskid")->GetValue()));
					CreateThread(NULL,0,SendThread,(LPVOID)m_taskid,0,0);
					mysqlhelper.m_pRecordset->MoveNext();
				}
			}
	
			sprintf(szSql,"select taskid  from dbo.jw2_BoardTasker where atonce=-1 and status=0");
			m_result=mysqlhelper.DoSql(szSql);
			if(m_result)
			{
				while(!mysqlhelper.m_pRecordset->adoEOF)
				{
					
					m_taskid=StrToInt((LPCSTR)_bstr_t(mysqlhelper.m_pRecordset->GetFields()->GetItem("taskid")->GetValue()));
					CreateThread(NULL,0,SendJW2Thread,(LPVOID)m_taskid,0,0);
					mysqlhelper.m_pRecordset->MoveNext();
				}
			}
			
			sprintf(szSql,"exec WA_sendatonce");
			m_result=mysqlhelper.DoSql(szSql);
			if(m_result)
			{
				while(!mysqlhelper.m_pRecordset->adoEOF)
				{
					
					m_taskid=StrToInt((LPCSTR)_bstr_t(mysqlhelper.m_pRecordset->GetFields()->GetItem("taskid")->GetValue()));
					CreateThread(NULL,0,SendWAThread,(LPVOID)m_taskid,0,0);
			//		waSend.AddTaskid(m_taskid);
					mysqlhelper.m_pRecordset->MoveNext();
				}
			}

			//add by ltx
			//发送侠盗金刚及时消息
			sprintf(szSql,"exec XDJG_sendatonce");
			m_result=mysqlhelper.DoSql(szSql);

			if(m_result)
			{
				while(!mysqlhelper.m_pRecordset->adoEOF)
				{
					m_taskid=StrToInt((LPCSTR)_bstr_t(mysqlhelper.m_pRecordset->GetFields()->GetItem("taskid")->GetValue()));
					CreateThread(NULL,0,SendXDJGThread,(LPVOID)m_taskid,0,0);

					mysqlhelper.m_pRecordset->MoveNext();
				}
			}

			///////////////////查询固定公告发送的状态为0的消息启动线程///////////////////////
			
			wsprintf(szSql,"exec pal_notatoncetostart");
			mysqlhelper.DoSql(szSql);


			sprintf(szSql,"update dbo.jw2_BoardTasker set status=2 where  status=0 and atonce=0 and  sendbegintime<getdate() and sendendtime>getdate()");
			mysqlhelper.DoSql(szSql);


			sprintf(szSql,"exec WA_NotatonceToStart");
			mysqlhelper.DoSql(szSql);
			
			//add by ltx
			sprintf(szSql,"exec XDJG_NotatonceToStart");
			mysqlhelper.DoSql(szSql);

			//Added by tengjie 20100812
			sprintf(szSql,"exec SC_NotatonceToStart");
			mysqlhelper.DoSql(szSql);

			//////以上表示如果已经到了公告开始发送时间，把status置为2(正在发送)

			sprintf(szSql,"exec Pal_SendNotatonce");
			m_result=mysqlhelper.DoSql(szSql);
			
			if(m_result)
			{
				while(!mysqlhelper.m_pRecordset->adoEOF)
				{
					m_taskid=StrToInt((LPCSTR)_bstr_t(mysqlhelper.m_pRecordset->GetFields()->GetItem("taskid")->GetValue()));
					CreateThread(NULL,0,SendThread,(LPVOID)m_taskid,0,0);
					mysqlhelper.m_pRecordset->MoveNext();
				}

			}

			
			sprintf(szSql,"select taskid  from dbo.jw2_BoardTasker where atonce=0 and status=2 and datediff(mi,sendbegintime,getdate())%sinterval=0 order by taskid","%");
			m_result=mysqlhelper.DoSql(szSql);
			
			
			if(m_result)
			{
				while(!mysqlhelper.m_pRecordset->adoEOF)
				{
					m_taskid=StrToInt((LPCSTR)_bstr_t(mysqlhelper.m_pRecordset->GetFields()->GetItem("taskid")->GetValue()));
					CreateThread(NULL,0,SendJW2Thread,(LPVOID)m_taskid,0,0);
					mysqlhelper.m_pRecordset->MoveNext();
				}
				
			}
		
			sprintf(szSql,"exec WA_SendNotatonce");
			m_result=mysqlhelper.DoSql(szSql);
			
			if(m_result)
			{
				while(!mysqlhelper.m_pRecordset->adoEOF)
				{
					m_taskid=StrToInt((LPCSTR)_bstr_t(mysqlhelper.m_pRecordset->GetFields()->GetItem("taskid")->GetValue()));
			//		waSend.AddTaskid(m_taskid);
					CreateThread(NULL,0,SendWAThread,(LPVOID)m_taskid,0,0);
					mysqlhelper.m_pRecordset->MoveNext();
				}
				
			}

			//add by ltx
			sprintf(szSql,"exec XDJG_SendNotatonce");
			m_result=mysqlhelper.DoSql(szSql);
			
			if(m_result)
			{
				while(!mysqlhelper.m_pRecordset->adoEOF)
				{
					m_taskid=StrToInt((LPCSTR)_bstr_t(mysqlhelper.m_pRecordset->GetFields()->GetItem("taskid")->GetValue()));
					CreateThread(NULL,0,SendXDJGThread,(LPVOID)m_taskid,0,0);
					mysqlhelper.m_pRecordset->MoveNext();
				}
				
			}

			//Added by tengjie 20100812
// 			sprintf(szSql,"exec SC_SendNotatonce");
// 			m_result=mysqlhelper.DoSql(szSql);
// 			
// 			if(m_result)
// 			{
// 				while(!mysqlhelper.m_pRecordset->adoEOF)
// 				{
// 					m_taskid=StrToInt((LPCSTR)_bstr_t(mysqlhelper.m_pRecordset->GetFields()->GetItem("taskid")->GetValue()));
// 					CreateThread(NULL,0,SendSCThread,(LPVOID)m_taskid,0,0);
// 					mysqlhelper.m_pRecordset->MoveNext();
// 				}
// 			}
			////////////////////////////////////删除固定公告/////////////////////////////////////////////////
			
			wsprintf(szSql,"exec Pal_EndNotatonce");
			mysqlhelper.DoSql(szSql);
			
			wsprintf(szSql," update jw2_BoardTasker set status=1 where status=2 and atonce =0  and sendendtime<getdate()");
			mysqlhelper.DoSql(szSql);


			wsprintf(szSql,"exec WA_EndNotatonce");
			mysqlhelper.DoSql(szSql);

			//add by ltx
			wsprintf(szSql,"exec XDJG_EndNotatonce");
			mysqlhelper.DoSql(szSql);
			
			//Added by tengjie 2010-08-12
// 			wsprintf(szSql,"exec SC_EndNotatonce");
// 			mysqlhelper.DoSql(szSql);
		}
		mysqlhelper.CloseData();
		ReleaseMutex(m_Mutex);
	}
	catch (...)
	{
		return;
	}

}
//////////////////////////
DWORD WINAPI DateBaseThread(LPVOID lpTime)
{
	try
	{
		ReadDataBase();
		while(true)
		{
			Sleep((int)lpTime);
			ReadDataBase();
		}
	
	}
	catch (...)
	{
		return false;
		
	}
	return TRUE;
}
void GetAccountPasswd(char * m_Account,char *m_passwd,int* iGMServerPort,int* iTime,int m_taskid)
{
	try
	{
		int iUser=m_taskid%6;
		char path[256];
		char filepath[256];
		ZeroMemory(path,256);
		ZeroMemory(filepath,256);
		GetCurrentDirectory(256,path);
		wsprintf(filepath,"%s%s",path,".\\config\\PalSend.ini");
		switch(iUser)
		{
		case 0:
			{
				GetPrivateProfileString("User","szAccount",NULL,m_Account,32,filepath);
				GetPrivateProfileString("User","szPassword",NULL,m_passwd,32,filepath);
				*iGMServerPort=GetPrivateProfileInt("User","iGMServerPort",0,filepath);
			//	*iTime=GetPrivateProfileInt("User","freTime",0,filepath);
			}
			break;
		case 1:
			{
				GetPrivateProfileString("User1","szAccount",NULL,m_Account,32,filepath);
				GetPrivateProfileString("User1","szPassword",NULL,m_passwd,32,filepath);
				*iGMServerPort=GetPrivateProfileInt("User1","iGMServerPort",0,filepath);
			//	*iTime=GetPrivateProfileInt("User1","freTime",0,filepath);
			}
			break;
		case 2:
			{
				GetPrivateProfileString("User2","szAccount",NULL,m_Account,32,filepath);
				GetPrivateProfileString("User2","szPassword",NULL,m_passwd,32,filepath);
				*iGMServerPort=GetPrivateProfileInt("User2","iGMServerPort",0,filepath);
			//	*iTime=GetPrivateProfileInt("User2","freTime",0,filepath);
			}
			break;
		case 3:
			{
				GetPrivateProfileString("User3","szAccount",NULL,m_Account,32,filepath);
				GetPrivateProfileString("User3","szPassword",NULL,m_passwd,32,filepath);
				*iGMServerPort=GetPrivateProfileInt("User3","iGMServerPort",0,filepath);
			//	*iTime=GetPrivateProfileInt("User3","freTime",0,filepath);
			}
			break;
		case 4:
			{
				GetPrivateProfileString("User4","szAccount",NULL,m_Account,32,filepath);
				GetPrivateProfileString("User4","szPassword",NULL,m_passwd,32,filepath);
				*iGMServerPort=GetPrivateProfileInt("User4","iGMServerPort",0,filepath);
			//	*iTime=GetPrivateProfileInt("User4","freTime",0,filepath);
			}
			break;
		case 5:
			{
				GetPrivateProfileString("User5","szAccount",NULL,m_Account,32,filepath);
				GetPrivateProfileString("User5","szPassword",NULL,m_passwd,32,filepath);
				*iGMServerPort=GetPrivateProfileInt("User5","iGMServerPort",0,filepath);
			//	*iTime=GetPrivateProfileInt("User5","freTime",0,filepath);
			}
			break;
		default:
			{
				GetPrivateProfileString("User","szAccount",NULL,m_Account,32,filepath);
				GetPrivateProfileString("User","szPassword",NULL,m_passwd,32,filepath);
				*iGMServerPort=GetPrivateProfileInt("User","iGMServerPort",0,filepath);
			//	*iTime=GetPrivateProfileInt("User","freTime",0,filepath);
			}
			break;
		}

	}
	catch (...)
	{
		
	}

}
void PalSend()
{
	try
	{
		char databaseip[20];    //本地数据库ip
		char databaseUser[20];	//本地数据库用户名
		char database[20];		//本地默认数据库
		char databasepwd[20];	//本地数据库密码
		//////////////////读配置文件////////////////////////
		char path[256];
		GetCurrentDirectory(256,path);
		char filepath[256];
		wsprintf(filepath,"%s%s",path,".\\config\\PalSend.ini");
		
		
		freTime=GetPrivateProfileInt("REMOTE","freTime",0,filepath);
		
		SendFreTime=GetPrivateProfileInt("REMOTE","SendFreTime",0,filepath);
		
		GetPrivateProfileString("DATABASE","Address","127.0.0.1",databaseip,20,filepath);
		GetPrivateProfileString("DATABASE","User",NULL,databaseUser,20,filepath);
		GetPrivateProfileString("DATABASE","Data",NULL,database,20,filepath);
		GetPrivateProfileString("DATABASE","Pwd",NULL,databasepwd,20,filepath);
		
		
		
		login_num=GetPrivateProfileInt("REMOTE","Login_num",0,filepath);
		send_num=GetPrivateProfileInt("REMOTE","Send_num",0,filepath);
		
		wsprintf(szData,"Provider=SQLOLEDB.1;Persist Security Info=True;User ID=%s;Password=%s;Data Source=%s;Initial Catalog=%s",databaseUser,databasepwd,databaseip,database);
		
		CreateThread(NULL,0,DateBaseThread,(LPVOID)freTime,0,0);
		m_Mutex=CreateMutex(NULL, FALSE, "DateBaseThread"); 
		m_SendMutex=CreateMutex(NULL,FALSE,"SendThread");
		m_SendJW2Mutex=CreateMutex(NULL,FALSE,"SendJW2Thread");
		m_SendWAMutex=CreateMutex(NULL,FALSE,"SendWAThread");
		//add by ltx
		m_SendXDJGMutex=CreateMutex(NULL,FALSE,"SendXDJGThread");
		//Added by tengjie 2010-08-12
//		m_SendSCMutex=CreateMutex(NULL,FALSE,"SendSCThread");

		printf("PalSend Start\n");
	}
	catch (...)
	{
		
	}
	

}

void SendNonscheduleBulletinMain(int m_taskid,char *pszGMServerIP,int m_WorldID,char *pszContent,int interval)
{
	try
	{
		int n_login =0;
		int n_send=0;
		// 注册连线CallBack函式
		g_ccNetObj.RegEvent_OnConnected(OnConnect, 0);
		g_ccNetObj.RegEvent_OnDisconnected(OnDisconnect, 0);
		g_ccNetObj.RegEvent_OnConnectFailed(OnConnectfailed, 0);
		
		// 注册对应事件CallBack函式
		g_ccNetObj.RegEvent_OnPacket(ENUM_PGGMCConnection_StoC_LoginResult,	LoginResult, 0);
		g_ccNetObj.RegEvent_OnPacket(ENUM_PGServerControl_StoC_SendNonscheduleBulletin, SendNonscheduleBulletin, 0);

		// 初始网路设定
		g_ccNetObj.SetConnectCount(10);		// 设定重新连线次数为10
		g_ccNetObj.SetReConnect(false);		// 设定断线的时候不会重新连线
		g_ccNetObj.SetShowIP(true);			// 设定连线时会显示IP

		Sleep(100);
		g_state = 0;

		char szAccount[32];
		char szPassword[32];
		ZeroMemory(szAccount,32);
		ZeroMemory(szPassword,32);
		int iTime=0;
		int iGMServerPort=0;
		while(g_state != -1)
		{
			Sleep(10);
			g_ccNetObj.Flush();
			
			// 依状态执行
			switch(g_state)
			{
			// 登入GMServer
			case 0:	
				{
					// 设定GMS连线参数
				//	char szGMServerIP[16] = "60.206.15.78";	// GMServer IP位址
				//	int  iGMServerPort = 9991;					// GMServer Port
					GetAccountPasswd(szAccount,szPassword,&iGMServerPort,&iTime,m_taskid);
					// 若与GMS连线成功
					if(g_ccNetObj.WaitConnect(pszGMServerIP, iGMServerPort, 65535, 65535))
					{		
						// 取得本机IP
						struct in_addr localaddr;   
						struct hostent *hInfo = NULL;   
						char szHostname[50];     
						gethostname(szHostname,49);							// 主机名称   
						hInfo = gethostbyname(szHostname);					// 主机资讯   
						memcpy(&localaddr, hInfo->h_addr, hInfo->h_length);	// 主机IP   

						// 设定GMS登入参数
					//	char szAccount[32] = "zhanghuanjun004";			// 登入帐号
					//	char szPassword[32] = "slyxf";		// 登入密码		
						char *szIP = inet_ntoa(localaddr);		// 登入IP	

						PGGMCConnection_CtoS_RequestLogin sPkt;	
						sprintf(sPkt.szAccount, "%s", szAccount);
						sprintf(sPkt.szPassword, "%s", szPassword);		
						memcpy(sPkt.szIP, szIP, sizeof(sPkt.szIP));

						// 传送帐密登入GMS
						g_ccNetObj.Send(sizeof(sPkt), (S_ObjNetPktBase*) &sPkt);
						g_state = 1;
					}
					else
					{
						g_state=-1;
					}
					break;
				}
			// 等待登入中
			case 1:
				n_login++;
				if(n_login>login_num)
				{
					g_state=-1;
				}
				printf("服务器登入中,第%i次\n",n_login);
				break;
			// 登入成功
			case 2:
				{
					printf("登入成功\n\n");
					
					// 设定命令参数
					int  iWorldID = m_WorldID;				// 世界编号	
					int  iDelayTime = 0;			// 延迟发送时间(秒),0代表立即发送
					char szContent[1024] = "test";	// 公告内容
					
					PGServerControl_CtoS_SendNonscheduleBulletin sPkt;
					sPkt.iWorldID = iWorldID;
					sPkt.iDelayTime = iDelayTime;
					sprintf(sPkt.szContent, "%s", pszContent);

					// 传送至GMS要求发送非固定公告
					g_ccNetObj.Send(sizeof(sPkt), (S_ObjNetPktBase*)&sPkt);	
					g_state = 3;			
				}
				break;
			// 等待发送非固定公告
		// 等待发送非固定公告
			case 3:
				n_send++;
				if(n_send>send_num)
				{
					g_state=-1;
				}
				printf("等待发送公告中,第%i次\n",n_send);
				break;
			}
		}			
		printf("\n");
		printf("\n==================== Shutdown ====================\n");
		g_ccNetObj.Disconnect();
		logFile.WriteText("结束发送公告");
	}
	catch (...)
	{
	}
	
}

//====================================================================================================
// 介面
//====================================================================================================

// 连线成功-------------------------------------------------------------------------------------------
void OnConnect(int iSockID, long lParam)
{
//	printf("Connect Success\n");
}

// 断线-----------------------------------------------------------------------------------------------
void OnDisconnect(int iSockID, long lParam)
{
//	printf("Disconnect from GMServer\n");
}

// 连线失败-------------------------------------------------------------------------------------------
void OnConnectfailed(int iConnectID, long lParam)
{
//	printf("Connect Failed\n");
}

// 登入结果-------------------------------------------------------------------------------------------
void LoginResult(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
{
	PGGMCConnection_StoC_LoginResult *pPkt = (PGGMCConnection_StoC_LoginResult *)pData;
	switch(pPkt->emResult)
	{
	case ENUM_GMCLoginResult_Success:
		printf("Login success\n");
		break;		
	case ENUM_GMCLoginResult_AccountFailed:
		printf("Login failed, account error\n");
		break;		
	case ENUM_GMCLoginResult_PasswordFailed:
		printf("Login failed, password error\n");
		break;		
	case ENUM_GMCLoginResult_IPFailed:
		printf("Login failed, ip error\n");
		break;		
	case ENUM_GMCLoginResult_AccountUsing:
		printf("Login failed, account is using\n");
		break;
	default:
		printf("Login failed\n");
		break;
	}
	g_state = 2;
}

// 发送非固定公告结果-----------------------------------------------------------------------------------
void SendNonscheduleBulletin(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam)
{
	SYSTEMTIME nowtime;
	GetLocalTime(&nowtime);

	PGServerControl_StoC_SendNonscheduleBulletin *pPkt = (PGServerControl_StoC_SendNonscheduleBulletin *)pData;
	CSqlHelper m_sqlhelper;
	switch(pPkt->emResult)
	{
	case ENUM_SendNonscheduleBulletinResult_Success:
		printf("\n<<< text=%s>>>\n\n",szContent);
		printf("\n<<< Send the schedule taskid=%i bulletin finish>>>\n\n",taskid);
		g_state = -1;
		break;
		
	case ENUM_SendNonscheduleBulletinResult_WorldNotFound:
		printf("SendNonscheduleBulletin result: world not found\n");
		g_state = -1;
		break;
		
	case ENUM_SendNonscheduleBulletinResult_LeaderNotFound:
		printf("SendNonscheduleBulletin result: leader not found\n");
		g_state = -1;
		break;
		
	case ENUM_SendNonscheduleBulletinResult_LeaderDisconnect:
		printf("SendNonscheduleBulletin result: leader disconnect\n");
		g_state = -1;
		break;
		
	default:
		break;
	}//switch
}

void GetServerIPGateWay(char* lpServerIP, char* lpGateWay)
{
	CSqlHelper m_sqlhelper;
	char m_sql[1024];
	sprintf(m_sql,"exec JW2_FindDBIP '%s',5",lpServerIP);
	m_sqlhelper.ConnectData(szData);
	m_sqlhelper.DoSql(m_sql);
	if(!m_sqlhelper.m_pRecordset->adoEOF)
	{
		sprintf(lpGateWay,"%s",(LPCSTR)_bstr_t(m_sqlhelper.m_pRecordset->GetFields()->GetItem("serverIP")->GetValue()));
	}
	m_sqlhelper.CloseData();
}


void AnalyseTaskId(int taskid,char * message)
{
	char operatorName[40];
	CSqlHelper *pmysqlhelper=new CSqlHelper();
	CSqlHelper *ptmpsqlhelper=new CSqlHelper();
	int i=0;
	char m_sql[256];
	char GateWay[40];
	char ServerIP[40];
	ZeroMemory(GateWay,40);
	ZeroMemory(ServerIP,40);
	pmysqlhelper->ConnectData(szData);
	sprintf(m_sql,"select a.serverIP,b.RealName from jw2_boardmessage a, gmtools_users b where taskid=%i and  a.userId=b.userID",taskid);
//	logFile.WriteLog("AnalyseTaskId\n");
	pmysqlhelper->DoSql(m_sql);
	while(!pmysqlhelper->m_pRecordset->adoEOF)
	{
		sprintf(ServerIP,"%s",(LPCSTR)_bstr_t(pmysqlhelper->m_pRecordset->GetFields()->GetItem("serverIP")->GetValue()));
		sprintf(operatorName,"%s",(LPCSTR)_bstr_t(pmysqlhelper->m_pRecordset->GetFields()->GetItem("RealName")->GetValue()));
		if(!strcmp(ServerIP,"255.255.255.255"))
		{
			ptmpsqlhelper->ConnectData(szData);
			sprintf(m_sql,"select serverIP from GMTools_Serverinfo where gameid=35 and gamedbid=5'");
			ptmpsqlhelper->DoSql(m_sql);
			while(!ptmpsqlhelper->m_pRecordset->adoEOF)
			{
				sprintf(ServerIP,"%s",(LPCSTR)_bstr_t(ptmpsqlhelper->m_pRecordset->GetFields()->GetItem("serverIP")->GetValue()));
				GetServerIPGateWay(ServerIP,GateWay);
				printf("发送IP:%s,内容%s,添加者%s",GateWay,message,operatorName);
				SendBulletin(GateWay,message,operatorName);
				ptmpsqlhelper->m_pRecordset->MoveNext();
				
			}
			ptmpsqlhelper->CloseData();
			
			
		}
		else
		{
			GetServerIPGateWay(ServerIP,GateWay);
			printf("发送IP:%s,内容%s,添加者%s",GateWay,message,operatorName);
		//	logFile.WriteLog(ServerIP);
			SendBulletin(GateWay,message,operatorName);
			
		}
		
		pmysqlhelper->m_pRecordset->MoveNext();
	}
	
	pmysqlhelper->CloseData();
	delete pmysqlhelper;
	delete ptmpsqlhelper;
	pmysqlhelper=NULL;
	ptmpsqlhelper=NULL;
	
}

// add by ltx

//发送公告
bool WA_SendButtle(char * ServerIP, int Port,int Realmid,int ClusterID, char * BoardMessage)
{
	bool bullflag = false;
	char retMessage[1024];

	try
	{
		REQ_SYSTEM_BROADCAST req_system_broadcast;				//发送公告请求
		REP_SYSTEM_BROADCAST rep_system_broadcast;				//发送公告回应
		short int req_packetLength;
		short int flag = 24;
		char sendbuf[0x400];
		char recvbuf[0x2800];

		CMySocket tempSocket;
		int sendcount=0;
		bool ishavelogin=false;
		while(sendcount<2&&ishavelogin==false)
		{
			sendcount++;
			if (WA_Connect_Remote_Login(&tempSocket, ServerIP, Port,Realmid))
			{
				ishavelogin=true;
				printf("第%d次登录成功\n",sendcount);
			}
			else
			{
				printf("第%d次登录失败[ServerIP: %s++++++大区id: %d]\n",sendcount, ServerIP, Realmid);
			}

		}
		if (!ishavelogin)
		{
			tempSocket.Close();
           return false;
		}
	//	if (!WA_Connect_Remote_Login(&tempSocket, ServerIP, Port))
	//	{
	//		logFile.WriteLog("WA","GM","勇士无双----远程登录失败\n");
	//		return false;		
	//	}
		else
		{
			req_packetLength = sizeof(REQ_SYSTEM_BROADCAST);
			
			memset(sendbuf,0, 0x400);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			req_system_broadcast.bType = REQ_SYSTEM_BROADCAST::T_NOW;
			req_system_broadcast.srv_cluster_id=ClusterID;
			req_system_broadcast.srv_realm_id=Realmid;
			req_system_broadcast.AddBroadcaseTxt(BoardMessage);
			
			memcpy(sendbuf+4, &req_system_broadcast, req_packetLength);			

			if (!tempSocket.MySend(sendbuf, req_packetLength+4))
			{
				bullflag = false;
			}
			else
			{
				memset(recvbuf, 0, 0x2800);
				
				int recvbyte = tempSocket.MyRecv(recvbuf,0x2800);
				if ( recvbyte>0 )
				{
					short int packetSize = *(short int *)recvbuf;
					memcpy(&rep_system_broadcast, recvbuf+4, packetSize);
					switch (rep_system_broadcast.eResult)
					{
					case REP_DELETE_ITEM::E_SUCCESS:	//成功
						{
							ZeroMemory(retMessage,1024);
							sprintf(retMessage,"勇士无双公告[%s]------%s\n",ServerIP, BoardMessage);
							logFile.WriteLog("WA","GM",retMessage);
							bullflag = true;
							break;
						}
					default:							//失败
						bullflag = false;
						break;
					}
				}
			}
			
			tempSocket.Close();
			return bullflag;
		}
	}
	catch(...)
	{
		return false;
	}
	
}

//连接远程服务端
bool WA_Connect_Remote_Login(CMySocket *bullSocket, char * ServerIP, int Port,int Realmid)
{
	bool bLoginFlag = false;	//登录成功标识
	
	try
	{
		char path[256];
		char filepath[256];
		char szAccount[40];
		char szPassword[40];
		int iPort=0;
		GetCurrentDirectory(256,path);
		sprintf(filepath,"%s%s",path,"\\config\\MWA.ini");
		
		GetPrivateProfileString("USER","szAccount","gm01",szAccount,40,filepath);
		GetPrivateProfileString("USER","szPassword","111111",szPassword,40,filepath);
		
		REQ_LOGIN req_login;		//登录请求
		REP_LOGIN rep_login;		//登录回应
		short int req_packetLength;
		short int flag = 24;
		char sendbuf[0x400];
		char recvbuf[0x2800];
		
//		printf("ServerIP:%s,Port:%d,szAccount:%s,szPassword:%s\n",ServerIP, Port, szAccount, szPassword);

		if(bullSocket->MyConnect(ServerIP, Port))
		{
			req_packetLength = sizeof(REQ_LOGIN);
			
			memset(sendbuf,0, 0x400);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段填充24
			strcpy(req_login.loginUserName, szAccount);
			strcpy(req_login.loginPassWord, szPassword);
			req_login.srv_realm_id=Realmid;
			memcpy(sendbuf+4, &req_login, req_packetLength);
			if (!bullSocket->MySend(sendbuf, req_packetLength+4))
			{
				logFile.WriteLog("WA","GM","发送请求失败\n");
			}
			else
			{
				memset(recvbuf, 0, 0x2800);
				int recvbyte = bullSocket->MyRecv(recvbuf, 0x2800);	//默认超时时间20秒
				if (recvbyte>0)
				{
					short int packetSize = *(short int *)recvbuf;
					memcpy(&rep_login, recvbuf+4, packetSize);
					switch (rep_login.eResult)
					{
					case REP_LOGIN::E_SUCCESS:	//登录成功
						{
							printf("登录成功");
							logFile.WriteLog("WA","GM","<公告>登陆成功\n");
							bLoginFlag = true;
							return bLoginFlag;
						}
					default:					//登录失败
						logFile.WriteLog("WA","GM","<公告>登陆失败1\n");
						break;
					}
				}
				else
				{
					logFile.WriteLog("WA","GM","<公告>登陆失败2\n");
				}
			}
		}
		
	}	
	catch (...)
	{
		logFile.WriteLog("WA","GM","<公告>发生未知错误");
	}
	
	bullSocket->Close();
	return bLoginFlag;
}


/*
bool SendButtle(char * ServerIP,char * BoardMessage)
{
	if(Connect_Remote_Login(ServerIP))
	{
		char retMessage[256];
		REQ_SYSTEM_BROADCAST req_system_broadcast;
		req_system_broadcast.bType=REQ_SYSTEM_BROADCAST::T_NOW;
		req_system_broadcast.AddBroadcaseTxt(BoardMessage);
		char * pSend=new char[sizeof(REQ_SYSTEM_BROADCAST)+2];
		ZeroMemory(pSend,sizeof(REQ_SYSTEM_BROADCAST)+2);
		*pSend=req_system_broadcast.GetSize();
		memcpy(pSend+2,(char *)&req_system_broadcast,sizeof(REQ_SYSTEM_BROADCAST));
		int iSend=send(sendSocket,pSend,req_system_broadcast.GetSize()+2,0);
		delete[] pSend;
		pSend=NULL;
		REP_SYSTEM_BROADCAST rep_system_broadcast;
		char * pRecv=new char[sizeof(REP_SYSTEM_BROADCAST)+2];
		ZeroMemory(pRecv,sizeof(REP_SYSTEM_BROADCAST)+2);
		if(iSend>0)
		{
			int recvLen = recv(sendSocket,pRecv,sizeof(REP_SYSTEM_BROADCAST)+2,0);//获取封停或解封玩家帐号的请求
			if(recvLen>0)
			{
				memcpy(&rep_system_broadcast,pRecv+2, recvLen);
				if(rep_system_broadcast.eResult==1)
				{
					sprintf(retMessage,"[游戏:]勇士无双------公告%s发送成功\n",BoardMessage);
					logFile.WriteLog("WA","GM",retMessage);
					delete[] pRecv;
					pRecv=NULL;
					return TRUE;
				}
			}
			else
			{
				delete[] pRecv;
				pRecv=NULL;
				return false;
			}
		}
		delete[] pRecv;
		pRecv=NULL;
		
	}
	else//连接失败 （写log)
	{
		logFile.WriteLog("WA","GM","勇士无双----远程端口连接失败\n");
	}
	return FALSE;
}
void initSocket(void)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	
	wVersionRequested = MAKEWORD( 2, 2 );
	
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		/ * Tell the user that we could not find a usable * /
		/ * WinSock DLL.                                  * /
		return;
	}
	
	
	
	if ( LOBYTE( wsaData.wVersion ) != 2 ||
		HIBYTE( wsaData.wVersion ) != 2 ) {
		/ * Tell the user that we could not find a usable * /
		/ * WinSock DLL.                                  * /
		WSACleanup( );
		return; 
	}
	
}

//连接远程服务端
bool Connect_Remote_Login(char * ServerIP)
{
	try
	{
		char path[256];
		char filepath[256];
		char szAccount[40];
		char szPassword[40];
		char retMessage[256];
		int iPort=0;
		GetCurrentDirectory(256,path);
		sprintf(filepath,"%s%s",path,".\\config\\PalSend.ini");
		
		GetPrivateProfileString("WA","szAccount","gm0001",szAccount,40,filepath);
		GetPrivateProfileString("WA","szPassword","1",szPassword,40,filepath);
		iPort=GetPrivateProfileInt("WA","iPort",18889,filepath);


		if(sendSocket!=INVALID_SOCKET)
			closesocket(sendSocket);
		sendSocket=socket(AF_INET,SOCK_STREAM,0);
		
		struct sockaddr_in addr;


		addr.sin_family=AF_INET;
		addr.sin_port=htons(iPort);
		addr.sin_addr.s_addr=inet_addr(ServerIP);
		
	
		int bNodelay = 1; 
		int err; 
		err=setsockopt(sendSocket,IPPROTO_TCP,TCP_NODELAY,(char *)&bNodelay,sizeof(bNodelay));
		
		if(err!=NO_ERROR)
			printf("setsockopt failed\n");

		if(connect(sendSocket, (struct sockaddr*)&addr, sizeof(addr)) != 0 )
		{
			logFile.WriteLog("WA","GM","连接远程服务器失败!\n");
			closesocket(sendSocket);
			return false;
		}

		REQ_LOGIN req_login;
		ZeroMemory(req_login.loginUserName,sizeof(req_login.loginUserName));
		ZeroMemory(req_login.loginPassWord,sizeof(req_login.loginPassWord));
		sprintf(req_login.loginUserName,"%s",szAccount);
		sprintf(req_login.loginPassWord,"%s",szPassword);
		char * pSend=new char[sizeof(REQ_LOGIN)+2];
		ZeroMemory(pSend,sizeof(REQ_LOGIN)+2);
		*pSend=sizeof(REQ_LOGIN);
		memcpy(pSend+2,(char *)&req_login,sizeof(REQ_LOGIN));
		int iSend=send(sendSocket,pSend,sizeof(REQ_LOGIN)+2,0);
		delete[] pSend;
		pSend=NULL;
		REP_LOGIN rep_login;
		char * pRecv=new char[sizeof(REP_LOGIN)+2];
		ZeroMemory(pRecv,sizeof(REP_LOGIN)+2);
		if(iSend>0)
		{
			int recvLen = recv(sendSocket,pRecv,sizeof(REP_LOGIN)+2,0);//获取封停或解封玩家帐号的请求
			if(recvLen>0)
			{
				memcpy(&rep_login,pRecv+2, recvLen);
				if(rep_login.eResult==1)
				{
					sprintf(retMessage,"[游戏:]勇士无双------[IP]:%s登陆成功\n",ServerIP);
					logFile.WriteLog("WA","GM",retMessage);
					delete[] pRecv;
					pRecv=NULL;
					return TRUE;
				}
			}
			else
			{
				logFile.WriteLog("WA","GM","登陆包回收错误");
				delete[] pRecv;
				pRecv=NULL;
				return FALSE;
			}
		}
		delete[] pRecv;
		pRecv=NULL;
		
	}	
	catch(...)
	{

	}

	return FALSE;
}*/

