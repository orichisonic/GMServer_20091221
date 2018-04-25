// AU2_Dll.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"
#include "AU2_Dll.h"
#include "LogFile.h"

#pragma comment(lib,"DSNShell.lib")

//bool TestNet(char * IP, int iPort);
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

CLogFile logFile;

int  SendBulletin(char *lpGateWay,char *lpBoardMessaeg,char *lpRealName)
{

	int state=-1;
	int DSNHandle = -1;

	try
	{
// 		if(!TestNet(lpGateWay,58118))
// 		{
// 			printf("%s网络不通\n",lpGateWay);
// 			return -1;
// 		}
	
		
	/*	WCHAR   wstr[MAX_PATH]; 
		int a=MultiByteToWideChar(CP_ACP,   0,  lpGateWay,   strlen(lpGateWay),   wstr,   sizeof(wstr)); 
		wstr[a]='\0';*/

		//计算这个GB2312实际有几个字组成
		DWORD num= MultiByteToWideChar(CP_ACP,0,lpGateWay,-1,NULL,0); 

		//定义一个UNICODE的指针
		WCHAR *wstr; 

		//动态的申请空间存字
		wstr= (WCHAR*)calloc(num,sizeof(WCHAR));
		
		//判断申请到空间没有
		if(wstr == NULL)              
		{
			free(wstr);
		}

		memset(wstr, 0, num*sizeof(WCHAR));

		//把GB2312变成UNICODE
		MultiByteToWideChar(CP_ACP,0, lpGateWay, -1, wstr, num); 

		DSNHandle = CreateDSNShell();
		if (DSNHandle == -1)
		{
			logFile.WriteLog("CreateDSNShell false");
			return -1;
		}

		if (DSNSHELL_NOTCONNECT == DSNShellIsGWConnected(DSNHandle))
		{
			if(DSNSHELL_OK==DSNShellConnectGW(DSNHandle, (LPCTSTR)wstr, 58118))
			{
				logFile.WriteLog("Begin Connect");
			}
		}

		free(wstr);

		int x=0;
		while(true)
		{
			UPDATE_RET temp_update;
			DSNShellUpdate(DSNHandle, temp_update);

			Sleep(1000);
			state = DSNShellIsGWConnected(DSNHandle);
			printf("Now Connection Status：%i\n",state);
			if(DSNSHELL_OK == state)
			{
				
				logFile.WriteLog("Connection Success");
				
				//计算这个GB2312实际有几个字组成
				num= MultiByteToWideChar(CP_ACP,0,lpGateWay,-1,NULL,0); 

				//动态的申请空间存字
				wstr= (WCHAR*)calloc(num,sizeof(WCHAR));

				//判断申请到空间没有
				if(wstr == NULL)           
				{
					free(wstr);
				}

				memset(wstr, 0, num*sizeof(WCHAR));

				MultiByteToWideChar(CP_ACP,0, lpBoardMessaeg, -1, wstr, num); 


				//a=MultiByteToWideChar(CP_ACP,   0,  lpBoardMessaeg,   strlen(lpBoardMessaeg),   wstr,   sizeof(wstr)); 
				//wstr[a]='\0';
				
				char szText[100];
				ZeroMemory(szText,100);

				if(DSNSHELL_OK == DSNShellSendBulletin(DSNHandle, (LPCTSTR)wstr))
				{
						sprintf(szText,"Send Messaage: %s(<GMUser>%s ADD)",lpBoardMessaeg,lpRealName);
						logFile.WriteLog(szText);
						
				}

				free(wstr);
				break;
				
			}
			else if(DSNSHELL_CONNECTINGNOW == state)
			{
				x++;
				if(x>20)
				{
					break;
				}


			}
			else
			{
				x++;
				if(x>5)
				{
					logFile.WriteLog("Connection OverTime");
					break;
				}
			}
		}

		DestroyDSNShell(DSNHandle);
	}
	catch(...)
	{

		if (DSNHandle != -1)
		{
			DestroyDSNShell(DSNHandle);
		}
		return -1;
	}
	
	return state;
}
int BanishPlayer(char* lpGateWay,char* lpUserId,char *lpRealName)
{	
	int state=-1;
	int DSNHandle = -1;

	try
	{

// 		if(!TestNet(lpGateWay,58118))
// 		{
// 			printf("%s网络不通\n",lpGateWay);
// 			return -1;
// 		}

		//计算这个GB2312实际有几个字组成
		DWORD num= MultiByteToWideChar(CP_ACP,0,lpGateWay,-1,NULL,0); 

		//定义一个UNICODE的指针
		WCHAR *wstr;                             

		//动态的申请空间存字
		wstr= (WCHAR*)calloc(num,sizeof(WCHAR));
		
		//判断申请到空间没有
		if(wstr == NULL)              
		{
			free(wstr);
		}

		memset(wstr, 0, num*sizeof(WCHAR));
		
		//把GB2312变成UNICODE
		MultiByteToWideChar(CP_ACP,0, lpGateWay, -1, wstr, num); 

		DSNHandle = CreateDSNShell();
		if (DSNHandle == -1)
		{
			logFile.WriteLog("CreateDSNShell false");
			return -1;
		}

		if (DSNSHELL_NOTCONNECT == DSNShellIsGWConnected(DSNHandle))
		{
			if(DSNSHELL_OK==DSNShellConnectGW(DSNHandle, (LPCTSTR)wstr, 58118))
			{
				logFile.WriteLog("Begin Connect");
			}
		}

		free(wstr);

		int x=0;
		while(true)
		{
			UPDATE_RET temp_update;
			DSNShellUpdate(DSNHandle, temp_update);						

			Sleep(1000);
			state = DSNShellIsGWConnected(DSNHandle);
			printf("Now Connection Status:%i\n",state);
			if(DSNSHELL_OK == state)
			{
				logFile.WriteLog("Connection Success");
				char szText[100];

				//计算这个GB2312实际有几个字组成
				num= MultiByteToWideChar(CP_ACP,0,lpGateWay,-1,NULL,0); 


				//动态的申请空间存字
				wstr= (WCHAR*)calloc(num,sizeof(WCHAR));
				
				//判断申请到空间没有
				if(wstr == NULL)              
				{
					free(wstr);
				}
				memset(wstr, 0, num*sizeof(WCHAR));

				MultiByteToWideChar(CP_ACP,0, lpUserId, -1, wstr, num); 

				//a=MultiByteToWideChar(CP_ACP,   0,  lpUserId,   strlen(lpUserId),   wstr,   sizeof(wstr)); 
				//wstr[a]='\0';

				if(DSNSHELL_OK == DSNShellKickUser(DSNHandle, (LPCTSTR)wstr))
				{
						sprintf(szText,"<GMUser>%s Banish %s Success\n",lpRealName,lpUserId);
						logFile.WriteLog(szText);
				}
				free(wstr);
				break;
			}
			else if(DSNSHELL_CONNECTINGNOW == state)
			{
				x++;
				if(x>20)
				{
					break;
				}

			}
			else
			{
				x++;
				if(x>5)
				{
					logFile.WriteLog("Connection OverTime");
					break;
				}
			}

		}
		
		//DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, lpUserId, -1, NULL, 0);
		//wchar_t *pwText = new wchar_t[dwNum];
		//ZeroMemory( pwText, dwNum );
		//if(!pwText)
		//{
		//	delete []pwText;
		//	pwText=NULL;
		//}
		//int parseLength=MultiByteToWideChar(CP_ACP, 0, lpUserId, -1, pwText, dwNum);
		//pwText[dwNum]='\0';
		
		DestroyDSNShell(DSNHandle);
	}
	catch(...)
	{
		if (DSNHandle != -1)
		{
			DestroyDSNShell(DSNHandle);
		}

		return -1;
	}

	return state;
}
/*
bool TestNet(char * IP, int iPort)
{
	sockaddr_in   addr;   
	SOCKET m_socket=socket(AF_INET,SOCK_STREAM,IPPROTO_IP);   

	if(m_socket==INVALID_SOCKET)   
		return   false;   

	addr.sin_family=AF_INET;   
	addr.sin_port=htons(iPort);   
	addr.sin_addr.s_addr=inet_addr(IP);   
	int   iret=connect(m_socket,(sockaddr *)&addr,sizeof(sockaddr));

	if(iret==0)   
	{   
		closesocket(m_socket);
		return   true;   
	}   
	closesocket(m_socket);
	return   false;   

}*/
