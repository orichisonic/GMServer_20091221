// GMServer.cpp : �������̨Ӧ�ó������ڵ㡣
//


#include "stdafx.h"
#include "GMTools.h"

bool FindProcess(LPCTSTR ProcessName);
void LocalDll();
HINSTANCE dllmodule;

vector <Dll_Main> dll_vector;
Dll_Main GM_Main;
Dll_Main JW2_Main;
Dll_Main GT_Main;
Dll_Main GTOP_Main;
Dll_Main Pal_Main;
Dll_Main PalLog_Main;
Dll_Main CG2_Main;
Dll_Main SDO_Main;
Dll_Main WA_Main;
Dll_Main Ban_Main;
Dll_Main CF_Main;
Dll_Main XDJG_Main;

int main(int argc, char* argv[])
{
	try
	{
		char path[256];
		GetCurrentDirectory(256,path);//��ȡ��ǰĿ¼
		char filepath[256];
		sprintf(filepath,"%s%s",path,"\\config\\scheme.ini");//��ȡscheme.ini�ļ�·��
		char lpIP[40];
		int ListenPort=GetPrivateProfileInt("SERVER","Port",0,filepath);//�õ������˿ں�
		GetPrivateProfileString("SERVER","Address","127.0.0.1",lpIP,40,filepath);

		////////����////////////
		HMODULE dllmodule;
		dllmodule=LoadLibrary("PalSend.dll");//����DLL
		PalSend palsend;
		if (dllmodule)
		{
			palsend=(PalSend)GetProcAddress(dllmodule,"PalSend");//��ȡ�ӿ�
			palsend();//��������Server
		}
		else
		{
		}
		
		dllmodule=LoadLibrary("BanServer.dll");
		BanServer banServer;
		if (dllmodule)
		{
			banServer=(BanServer)GetProcAddress(dllmodule,"StartServer");//��ȡ�ӿ�
			banServer();//��������Server
		}
		else
		{
		}
		
		//�������
// 		HMODULE dll=LoadLibrary("SendSCBroad.dll");
// 		SendSCBroad ScSend;
// 		if (dll)
// 		{
// 			ScSend=(SendSCBroad)GetProcAddress(dll,"SendSCBroad");//��ȡ�ӿ�
// 			ScSend();//��������Server
// 		}
// 		else
// 		{
// 		}

		LocalDll();//���ص�ǰĿ¼��DLL
		
		CServiceManagerModule serviceManageModule;
		serviceManageModule.m_nPort=ListenPort;
		serviceManageModule.OnStartService();
		
		while(true)
		{
			if(!FindProcess(PROCESS_NAME))
			{
				PROCESS_INFORMATION piProcInfoGPS;
				STARTUPINFO siStartupInfo;
				SECURITY_ATTRIBUTES saProcess, saThread;
				ZeroMemory( &siStartupInfo, sizeof(siStartupInfo) );
				siStartupInfo.cb = sizeof(siStartupInfo);
				saProcess.nLength = sizeof(saProcess);
				saProcess.lpSecurityDescriptor = NULL;
				saProcess.bInheritHandle = true;
				saThread.nLength = sizeof(saThread);
				saThread.lpSecurityDescriptor = NULL;
				saThread.bInheritHandle = true;
				::CreateProcess( NULL, (LPTSTR)PROCESS_NAME, &saProcess, &saThread, false, CREATE_NEW_CONSOLE, NULL, NULL, &siStartupInfo, &piProcInfoGPS );
			}
			Sleep(100000);
		}

		FreeLibrary(dllmodule);//�ͷ�DLL
		if(!dll_vector.empty())//�ͷŽӿڶ���
		{
			dll_vector.clear();
		}
	}
	catch(...)
	{

	}

	return 0;
}

bool FindProcess(LPCTSTR ProcessName)
{
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);
	Process32First(hSnapShot, &pe);
	if(lstrcmp(ProcessName, pe.szExeFile) == 0)
		return true;
	while(Process32Next(hSnapShot, &pe))
	{
		if(lstrcmp(ProcessName, pe.szExeFile) == 0)
			return true;
	}
	
    return false;
}

void LocalDll()
{
	try
	{
		vector<FileInfo> file_dll_vector;
		GetLocalDll(&file_dll_vector);//�ҵ���ǰĿ¼������DLL���ɶ���
		char inMain[40];//��ں����ļ�����ȥ��.dll)+_Main
		FileInfo m_file_info;
		vector<FileInfo>::iterator iter;
		
		
		for( iter = file_dll_vector.begin(); iter != file_dll_vector.end(); iter++ )//��Ѱ����
		{
			m_file_info = *iter;
			dllmodule=LoadLibrary(m_file_info.FileName);//����DLL
			if(dllmodule)
			{
				char *lpfile=strtok(m_file_info.FileName,".");//��ȡ�ļ���
				ZeroMemory(inMain,40);
				sprintf(inMain,"%s%s",lpfile,"_Main");//������ں�����
				if(!strcmp(inMain,"GM_Main"))
				{
					GM_Main=(Dll_Main)GetProcAddress(dllmodule,inMain);//������ں���
					if(GM_Main!=NULL)
						printf("GM_Main\n");
				}
				else if(!strcmp(inMain,"JW2_Main"))
				{
					JW2_Main=(Dll_Main)GetProcAddress(dllmodule,inMain);//������ں���
					if(JW2_Main!=NULL)
						printf("JW2_Main\n");
				}
				else if(!strcmp(inMain,"GT_Main"))
				{
					GT_Main=(Dll_Main)GetProcAddress(dllmodule,inMain);//������ں���
					if(GT_Main!=NULL)
						printf("GT_Main\n");
				}
				else if(!strcmp(inMain,"GTOP_Main"))
				{
					GTOP_Main=(Dll_Main)GetProcAddress(dllmodule,inMain);//������ں���
					if(GTOP_Main!=NULL)
						printf("GTOP_Main\n");
				}
				else if(!strcmp(inMain,"Pal_Main"))
				{
					Pal_Main=(Dll_Main)GetProcAddress(dllmodule,inMain);//������ں���
					if(Pal_Main!=NULL)
						printf("Pal_Main\n");
				}
				else if(!strcmp(inMain,"PalLog_Main"))
				{
					PalLog_Main=(Dll_Main)GetProcAddress(dllmodule,inMain);//������ں���
					if(PalLog_Main!=NULL)
						printf("PalLog_Main\n");
				}
				else if(!strcmp(inMain,"Ban_Main"))
				{
					Ban_Main=(Dll_Main)GetProcAddress(dllmodule,inMain);//������ں���
					if(Ban_Main!=NULL)
						printf("Ban_Main\n");
				}
				else if(!strcmp(inMain,"SDO_Main"))
				{
					SDO_Main=(Dll_Main)GetProcAddress(dllmodule,inMain);//������ں���
					if(SDO_Main!=NULL)
						printf("SDO_Main\n");
				}
				else if(!strcmp(inMain,"CG2_Main"))
				{
					CG2_Main=(Dll_Main)GetProcAddress(dllmodule,inMain);//������ں���
					if(CG2_Main!=NULL)
						printf("CG2_Main\n");
				}
				else if(!strcmp(inMain,"WA_Main"))
				{
					WA_Main=(Dll_Main)GetProcAddress(dllmodule,inMain);//������ں���
					if(WA_Main!=NULL)
						printf("WA_Main\n");
				}
				else if(!strcmp(inMain,"CF_Main"))
				{
					CF_Main=(Dll_Main)GetProcAddress(dllmodule,inMain);//������ں���
					if(CF_Main!=NULL)
						printf("CF_Main\n");
				}
				else if(!strcmp(inMain,"XDJG_Main"))
				{
					XDJG_Main=(Dll_Main)GetProcAddress(dllmodule,inMain);//������ں���
					if(XDJG_Main!=NULL)
						printf("XDJG_Main\n");
				}				
				
			}
		}
	}
	catch(...)
	{

	}	
}

///�õ���ǰĿ¼��all dll
void GetLocalDll(vector <FileInfo> * m_file_vector)
{
	try
	{
		char lpfile[1024];
		char strFilePath[100];
		ZeroMemory(lpfile,1024);
		char path[256];
		GetCurrentDirectory(256,path);//��ȡ��ǰĿ¼
		sprintf(lpfile,"%s%s",path,"\\*.dll");//��ȡ��ǰĿ¼dll��׺��
		HANDLE hFind;  
		WIN32_FIND_DATA fd;  
		FileInfo info;
		
		if ((hFind = ::FindFirstFile(lpfile,&fd)) !=  INVALID_HANDLE_VALUE)    //�ҵ���һ�������������ļ�
		{
			sprintf(info.FileName,"%s",fd.cFileName);//��ȡ�ļ���
			m_file_vector->push_back(info);//��ӵ��ļ�����
			
			//����
			while(::FindNextFile(hFind, &fd))
			{
				if(!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{
					sprintf(info.FileName,"%s",fd.cFileName);//��ȡ�ļ���
					m_file_vector->push_back(info);//��ӵ��ļ�����
				}
			}
			::FindClose(hFind);  
		}  
	}
	catch(...)
	{

	}
}

