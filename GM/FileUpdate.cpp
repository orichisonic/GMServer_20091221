#include "stdafx.h"
#include "FileUpdate.h"

#include "winver.h"
#pragma comment(lib,"Version.lib")

#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>


//���캯��
CFileUpdate::CFileUpdate()
{
}

//��������
CFileUpdate::~CFileUpdate()
{
}

//��ȡ�ļ��İ汾��
void CFileUpdate::GetVersionFromPath(char * IDEPath,char * lpVersion)
{
	int len=GetFileVersionInfoSize(IDEPath,NULL);//�õ��ļ��汾��Ϣ��С
	char *lpData =new char [len+1];//�����ڴ�ռ�
	ZeroMemory(lpData,len+1);

	BOOL bSuccess = GetFileVersionInfo(IDEPath, 0, len, lpData);//�õ��ļ��汾��Ϣ
	LPVOID lpbuf;
	UINT uLen = 0;
	bSuccess = VerQueryValue(lpData,
		TEXT("\\StringFileInfo\\080403a8\\FileVersion"), //0804����//04b0��1252,ANSI
		&lpbuf,
		&uLen); //�����ļ��汾��Ϣ
	sprintf(lpVersion,"%s",lpbuf);
	lpbuf=NULL;
	delete[] lpData;
	lpData=NULL;
}
void CFileUpdate::GetUpdateFile(char *lppath)
{
	char lpfile[1024];
	char strVersion[20];
	char strFilePath[100];
	ZeroMemory(lpfile,1024);
	ZeroMemory(strVersion,20);
	ZeroMemory(strFilePath,100);
	sprintf(lpfile,"%s%s",lppath,"DLL\\*.dll");//��������dll·��
	HANDLE hFind;  
	WIN32_FIND_DATA fd;  
	FileInfo info;
	if ((hFind = ::FindFirstFile(lpfile,&fd)) !=  INVALID_HANDLE_VALUE)   //��ѯ��һ�������������ļ� 
	{
		sprintf(info.FileName,"%s",fd.cFileName);//��д�ṹ���ļ���
		sprintf(strFilePath,"%s%s%s",lppath,"DLL\\",fd.cFileName);//��д�ṹ���ļ�ȫ·��
		GetVersionFromPath(strFilePath,strVersion);//��ȡ�ļ��汾
		sprintf(info.FileVersion,strVersion);//��д�ṹ���ļ��汾
		sprintf(info.FilePath,"%s%s",lppath,"DLL\\");//��д�ṹ���ļ�·��
		info.FileSize=GetFileSize(strFilePath);//��д�ṹ���ļ���С
		vector_updatefile.push_back(info);//�ѽṹ��������
		updatefilenum++;//�����ļ����ݼ�һ
		
		//����
		while(::FindNextFile(hFind, &fd))//��ѯ�ڶ��������������ļ� 
		{
			if(!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))//����ļ����Ͳ����ļ���
			{
				sprintf(info.FileName,"%s",fd.cFileName);//��д�ṹ���ļ���
				sprintf(strFilePath,"%s%s%s",lppath,"DLL\\",fd.cFileName);//��д�ṹ���ļ�ȫ·��
				GetVersionFromPath(strFilePath,strVersion);//��ȡ�ļ��汾
				sprintf(info.FileVersion,strVersion);//��д�ṹ���ļ��汾
				sprintf(info.FilePath,"%s%s",lppath,"DLL\\");//��д�ṹ���ļ�·��
				info.FileSize=GetFileSize(strFilePath);//��д�ṹ���ļ���С
				vector_updatefile.push_back(info);//�ѽṹ��������
				updatefilenum++;//�����ļ����ݼ�һ
			}
		}
		::FindClose(hFind);  //�ر��ļ�
	}   
	


	sprintf(lpfile,"%s%s",lppath,"EXE\\*.exe");//��������exe·��
	if ((hFind = ::FindFirstFile(lpfile,&fd)) !=  INVALID_HANDLE_VALUE)     //��ѯ��һ�������������ļ� 
	{
		sprintf(info.FileName,"%s",fd.cFileName);//��д�ṹ���ļ���
		sprintf(strFilePath,"%s%s%s",lppath,"EXE\\",fd.cFileName);//��д�ṹ���ļ�ȫ·��
		GetVersionFromPath(strFilePath,strVersion);//��ȡ�ļ��汾
		sprintf(info.FileVersion,strVersion);//��д�ṹ���ļ��汾
		sprintf(info.FilePath,"%s%s",lppath,"EXE\\");//��д�ṹ���ļ�·��
		info.FileSize=GetFileSize(strFilePath);//��д�ṹ���ļ���С
		vector_updatefile.push_back(info);//�ѽṹ��������
		updatefilenum++;//�����ļ����ݼ�һ

		//����
		while(::FindNextFile(hFind, &fd))//��ѯ�ڶ��������������ļ� 
		{
			if(!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))//����ļ����Ͳ����ļ���
			{
				sprintf(info.FileName,"%s",fd.cFileName);//��д�ṹ���ļ���
				sprintf(strFilePath,"%s%s%s",lppath,"EXE\\",fd.cFileName);//��д�ṹ���ļ�ȫ·��
				GetVersionFromPath(strFilePath,strVersion);//��ȡ�ļ��汾
				sprintf(info.FileVersion,strVersion);//��д�ṹ���ļ��汾
				sprintf(info.FilePath,"%s%s",lppath,"EXE\\");//��д�ṹ���ļ�·��
				info.FileSize=GetFileSize(strFilePath);//��д�ṹ���ļ���С
				vector_updatefile.push_back(info);//�ѽṹ��������
				updatefilenum++;//�����ļ����ݼ�һ
			}
		}
		::FindClose(hFind);   //�ر��ļ�
	}   

}
void CFileUpdate::ActiveCompare()
{
	int i=0,j=0;
	FileInfo clientfileinfo,updatefileinfo;
	for(i=0;i<clientfilenum;i++)
	{
		clientfileinfo=vector_clientfile.at(i);//ȡ�ͻ����ļ���Ϣ
		for(j=0;j<updatefilenum;j++)
		{
			updatefileinfo=vector_updatefile.at(j);//ȡ�����ļ���Ϣ
			if(!strcmp(clientfileinfo.FileName,updatefileinfo.FileName))//�Ƚ��ļ����Ƿ�һ��
			{
				int aaa=strcmp(clientfileinfo.FileVersion,updatefileinfo.FileVersion);//�Ƚ��ļ��汾��
				if(strcmp(clientfileinfo.FileVersion,updatefileinfo.FileVersion)>=0)//����ͻ��˱ȷ�����ļ��汾�Ŵ�
				{
					vector_updatefile[j].flag=0;//���ñ�־���ø���
				}
			}
		}
	}
}
void CFileUpdate::GetIniFile(char *lppath)
{
	char lpfile[1024];
	char strVersion[20];
	char strFilePath[100];
	ZeroMemory(lpfile,1024);
	ZeroMemory(strVersion,20);
	ZeroMemory(strFilePath,100);
	HANDLE hFind;  
	WIN32_FIND_DATA fd;  
	FileInfo info;
	sprintf(lpfile,"%s%s",lppath,"INI\\*.ini");//��������INI·��
	if ((hFind = ::FindFirstFile(lpfile,&fd)) !=  INVALID_HANDLE_VALUE)     //��ѯ��һ�������������ļ� 
	{
		sprintf(info.FileName,"%s",fd.cFileName);//��д�ṹ���ļ���
		sprintf(strFilePath,"%s%s%s",lppath,"INI\\",fd.cFileName);//��д�ṹ���ļ�ȫ·��
		sprintf(info.FileVersion,"0.0.0.0");//��д�ṹ���ļ��汾
		sprintf(info.FilePath,"%s%s",lppath,"INI\\");//��д�ṹ���ļ�·��
		info.FileSize=GetFileSize(strFilePath);//��д�ṹ���ļ���С
		vector_inifile.push_back(info);//�ѽṹ��������
		inifilenum++;

		//����
		while(::FindNextFile(hFind, &fd))//��ѯ�ڶ��������������ļ� 
		{
			if(!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))//����ļ����Ͳ����ļ���
			{
				sprintf(info.FileName,"%s",fd.cFileName);//��д�ṹ���ļ���
				sprintf(strFilePath,"%s%s%s",lppath,"INI\\",fd.cFileName);//��д�ṹ���ļ�ȫ·��
				sprintf(info.FileVersion,"0.0.0.0");//��д�ṹ���ļ��汾
				sprintf(info.FilePath,"%s%s",lppath,"INI\\");//��д�ṹ���ļ�·��
				info.FileSize=GetFileSize(strFilePath);//��д�ṹ���ļ���С
				vector_inifile.push_back(info);//�ѽṹ��������
				inifilenum++;
			}
		}
		::FindClose(hFind);  //�ر��ļ�
	}  
}

/////////////////////////////////////
//������CompareFile�Ƚ��ļ�
////////////////////
void CFileUpdate::CompareFile(SOCKET m_socket,char * lpDllFile,char * lpExeFile)
{
	try
	{
		///������
		vector_clientfile.clear();//��տͻ����ļ�����
		clientfilenum=0;//���ÿͻ����ļ�����Ϊ0
		AnalogFile(lpDllFile);//����dll�ļ��ַ���
		AnalogFile(lpExeFile);//����exe�ļ��ַ���
		///�õ���Ҫ���µ��ļ�Ŀ¼
		char path[256];
		ZeroMemory(path,256);
		GetCurrentDirectory(256,path);//�õ���ǰĿ¼
		char updatefilepath[256];
		ZeroMemory(updatefilepath,256);
		wsprintf(updatefilepath,"%s%s",path,"\\patch\\");//��ø���Ŀ¼
		vector_updatefile.clear();//��ո����ļ�����
		updatefilenum=0;//���ø����ļ�����Ϊ0
		GetUpdateFile(updatefilepath);//�õ������ļ�

		ActiveCompare();//�Ƚ��ļ�
		COperVector opervector;

		if(updatefilenum==0)
		{
			opervector.InitCreate(1,6);//����ռ�
		}
		else
		{
			inifilenum=0;//����ini�ļ�����Ϊ0
			vector_inifile.clear();//���ini����
			GetIniFile(updatefilepath);//�õ�ini�ļ���Ϣ
			opervector.InitCreate(updatefilenum+inifilenum+5,6);//����ռ�
		}
		/////////�������ļ��õ��ļ���Ϣ


		char strInt[10];
		ZeroMemory(strInt,10);
		bool updateflag=false;
		for(int i=0;i<updatefilenum;i++)
		{
			if(vector_updatefile[i].flag==1)//��־ΪҪ����
			{
				updateflag=true;//�ļ����±��Ϊtrue
				//��Ӹ����ļ���TFLV
				opervector.AddTFLV(CEnumCore::TagName::UpdateFileName,CEnumCore::TagFormat::TLV_STRING,strlen(vector_updatefile[i].FileName),(unsigned char *)&vector_updatefile[i].FileName);
				//��Ӹ����ļ��汾TFLV
				opervector.AddTFLV(CEnumCore::TagName::UpdateFileVersion,CEnumCore::TagFormat::TLV_STRING,strlen(vector_updatefile[i].FileVersion),(unsigned char *)&vector_updatefile[i].FileVersion);
				//��Ӹ����ļ�·��TFLV
				opervector.AddTFLV(CEnumCore::TagName::UpdateFilePath,CEnumCore::TagFormat::TLV_STRING,strlen(vector_updatefile[i].FilePath),(unsigned char *)&vector_updatefile[i].FilePath);
				_itoa(vector_updatefile[i].FileSize,strInt,10);
				//��Ӹ����ļ���СTFLV
				opervector.AddTFLV(CEnumCore::TagName::UpdateFileSize,CEnumCore::TagFormat::TLV_INTEGER,4,(unsigned char *)&strInt);

			}
		}
		if(updateflag==true)//���updateflagΪtrue
		{
			inifilenum=0;
			vector_inifile.clear();
			GetIniFile(updatefilepath);//�õ�ini�ļ���Ϣ
			for(int i=0;i<inifilenum;i++)
			{
				if(vector_inifile[i].flag==1)
				{
					//��Ӹ����ļ���TFLV
					opervector.AddTFLV(CEnumCore::TagName::UpdateFileName,CEnumCore::TagFormat::TLV_STRING,strlen(vector_inifile[i].FileName),(unsigned char *)&vector_inifile[i].FileName);
					//��Ӹ����ļ��汾TFLV
					opervector.AddTFLV(CEnumCore::TagName::UpdateFileVersion,CEnumCore::TagFormat::TLV_STRING,strlen(vector_inifile[i].FileVersion),(unsigned char *)&vector_inifile[i].FileVersion);
					//��Ӹ����ļ�·��TFLV
					opervector.AddTFLV(CEnumCore::TagName::UpdateFilePath,CEnumCore::TagFormat::TLV_STRING,strlen(vector_inifile[i].FilePath),(unsigned char *)&vector_inifile[i].FilePath);
					_itoa(vector_inifile[i].FileSize,strInt,10);
					//��Ӹ����ļ���СTFLV
					opervector.AddTFLV(CEnumCore::TagName::UpdateFileSize,CEnumCore::TagFormat::TLV_INTEGER,4,(unsigned char *)&strInt);

				}
			}


		}
		if(updateflag==false)//���updateflagΪfalse,û���ļ�����
		{
			//���û���ļ�������ϢTFLV
			opervector.AddTFLV(CEnumCore::TagName::MESSAGE,CEnumCore::TagFormat::TLV_STRING,strlen("FAILURE"),(unsigned char *)&"FAILURE");

		}	
		CSocketData socketdata;
		CMSocket msocket;
		msocket.SetSocket(m_socket);//����SOCKET

		int alllength=0;
		alllength=opervector.EnBody();//�����ڴ��
		socketdata.Create(alllength);//����ռ�

		socketdata.AddHeader(1,CEnumCore::Msg_Category::COMMON,CEnumCore::ServiceKey::CLIENT_PATCH_COMPARE_RESP,alllength);//��ͷ


		socketdata.EnSendBuf(&alllength,opervector.getDes(),alllength);//���������ڴ�

		msocket.SendBuf((char *)socketdata.GetSendBuf(),alllength);//�����ڴ�
		socketdata.Destory();//�����ڴ��

	}
	catch(...)
	{

	}
}

/////////////////////////////////////
//������CompareFile�Ƚ��ļ�
////////////////////
void CFileUpdate::CompareFile(char * lpDllFile,char * lpExeFile,CSession * pSess)
{
	try
	{
		///������
		vector_clientfile.clear();//��տͻ����ļ�����
		clientfilenum=0;//���ÿͻ����ļ�����Ϊ0
		AnalogFile(lpDllFile);//����dll�ļ��ַ���
		AnalogFile(lpExeFile);//����exe�ļ��ַ���
		///�õ���Ҫ���µ��ļ�Ŀ¼
		char path[256];
		ZeroMemory(path,256);
		GetCurrentDirectory(256,path);//�õ���ǰĿ¼
		char updatefilepath[256];
		ZeroMemory(updatefilepath,256);
		wsprintf(updatefilepath,"%s%s",path,"\\patch\\");//��ø���Ŀ¼
		vector_updatefile.clear();//��ո����ļ�����
		updatefilenum=0;//���ø����ļ�����Ϊ0
		GetUpdateFile(updatefilepath);//�õ������ļ�

		ActiveCompare();//�Ƚ��ļ�
		COperVector opervector;

		if(updatefilenum==0)
		{
			opervector.InitCreate(1,6);//����ռ�
		}
		else
		{
			inifilenum=0;//����ini�ļ�����Ϊ0
			vector_inifile.clear();//���ini����
			GetIniFile(updatefilepath);//�õ�ini�ļ���Ϣ
			opervector.InitCreate(updatefilenum+inifilenum+5,6);//����ռ�
		}
		/////////�������ļ��õ��ļ���Ϣ


		char strInt[10];
		ZeroMemory(strInt,10);
		bool updateflag=false;
		for(int i=0;i<updatefilenum;i++)
		{
			if(vector_updatefile[i].flag==1)//��־ΪҪ����
			{
				updateflag=true;//�ļ����±��Ϊtrue
				//��Ӹ����ļ���TFLV
				opervector.AddTFLV(CEnumCore::TagName::UpdateFileName,CEnumCore::TagFormat::TLV_STRING,strlen(vector_updatefile[i].FileName),(unsigned char *)&vector_updatefile[i].FileName);
				//��Ӹ����ļ��汾TFLV
				opervector.AddTFLV(CEnumCore::TagName::UpdateFileVersion,CEnumCore::TagFormat::TLV_STRING,strlen(vector_updatefile[i].FileVersion),(unsigned char *)&vector_updatefile[i].FileVersion);
				//��Ӹ����ļ�·��TFLV
				opervector.AddTFLV(CEnumCore::TagName::UpdateFilePath,CEnumCore::TagFormat::TLV_STRING,strlen(vector_updatefile[i].FilePath),(unsigned char *)&vector_updatefile[i].FilePath);
				_itoa(vector_updatefile[i].FileSize,strInt,10);
				//��Ӹ����ļ���СTFLV
				opervector.AddTFLV(CEnumCore::TagName::UpdateFileSize,CEnumCore::TagFormat::TLV_INTEGER,4,(unsigned char *)&strInt);

			}
		}
		if(updateflag==true)//���updateflagΪtrue
		{
			inifilenum=0;
			vector_inifile.clear();
			GetIniFile(updatefilepath);//�õ�ini�ļ���Ϣ
			for(int i=0;i<inifilenum;i++)
			{
				if(vector_inifile[i].flag==1)
				{
					//��Ӹ����ļ���TFLV
					opervector.AddTFLV(CEnumCore::TagName::UpdateFileName,CEnumCore::TagFormat::TLV_STRING,strlen(vector_inifile[i].FileName),(unsigned char *)&vector_inifile[i].FileName);
					//��Ӹ����ļ��汾TFLV
					opervector.AddTFLV(CEnumCore::TagName::UpdateFileVersion,CEnumCore::TagFormat::TLV_STRING,strlen(vector_inifile[i].FileVersion),(unsigned char *)&vector_inifile[i].FileVersion);
					//��Ӹ����ļ�·��TFLV
					opervector.AddTFLV(CEnumCore::TagName::UpdateFilePath,CEnumCore::TagFormat::TLV_STRING,strlen(vector_inifile[i].FilePath),(unsigned char *)&vector_inifile[i].FilePath);
					_itoa(vector_inifile[i].FileSize,strInt,10);
					//��Ӹ����ļ���СTFLV
					opervector.AddTFLV(CEnumCore::TagName::UpdateFileSize,CEnumCore::TagFormat::TLV_INTEGER,4,(unsigned char *)&strInt);

				}
			}


		}
		if(updateflag==false)//���updateflagΪfalse,û���ļ�����
		{
			//���û���ļ�������ϢTFLV
			opervector.AddTFLV(CEnumCore::TagName::MESSAGE,CEnumCore::TagFormat::TLV_STRING,strlen("FAILURE"),(unsigned char *)&"FAILURE");
		}	
		CSocketData socketdata;
		int alllength=0;
		alllength=opervector.EnBody();//�����ڴ��
		socketdata.Create(alllength);//����ռ�
		socketdata.AddHeader(1,CEnumCore::Msg_Category::COMMON,CEnumCore::ServiceKey::CLIENT_PATCH_COMPARE_RESP,alllength);//��ͷ
		socketdata.EnSendBuf(&alllength,opervector.getDes(),alllength);//���������ڴ�
		pSess->Send(socketdata.GetSendBuf(),alllength);
		socketdata.Destory();//�����ڴ��

	}
	catch(...)
	{

	}
}

//�����ļ�
void CFileUpdate::UpdateFile(SOCKET m_socket,char * filename,char * filepath,int fileSize)
{
	try
	{
		updatefileinfo.flag=true;//�����ļ���־
		updatefileinfo.m_socket=m_socket;//�����ļ���Socket
		sprintf(updatefileinfo.m_fileinfo.FileName,filename);//�����ļ����ļ���
		sprintf(updatefileinfo.m_fileinfo.FilePath,filepath);//�����ļ���·��
		updatefileinfo.m_fileinfo.FileSize=fileSize;//�����ļ��Ĵ�С
		UpFileThread(&updatefileinfo);//��ʼ�����ļ��߳�
	}
	catch(...)
	{

	}
	

	
}

//�����ļ�
void CFileUpdate::UpdateFile(char * filename,char * filepath,int fileSize,CSession *pSess)
{
	try
	{
		updatefileinfo.flag=true;//�����ļ���־
		sprintf(updatefileinfo.m_fileinfo.FileName,filename);//�����ļ����ļ���
		sprintf(updatefileinfo.m_fileinfo.FilePath,filepath);//�����ļ���·��
		updatefileinfo.m_fileinfo.FileSize=fileSize;//�����ļ��Ĵ�С
		UpFileThread(&updatefileinfo,pSess);//��ʼ�����ļ��߳�
	}
	catch(...)
	{
		
	}
	
	
	
}

//�����ļ��߳�
void CFileUpdate::UpFileThread(UpdateFileInfo * pUpdateFileInfo,CSession * pSess)
{
	try
	{
		
		char strFile[256];
		ZeroMemory(strFile,256);
		char * lpFileData = new char[pUpdateFileInfo->m_fileinfo.FileSize];//�����ļ���С�Ŀռ�
		ZeroMemory(lpFileData,pUpdateFileInfo->m_fileinfo.FileSize);//��տռ�
		sprintf(strFile,"%s%s",pUpdateFileInfo->m_fileinfo.FilePath,pUpdateFileInfo->m_fileinfo.FileName);//�����ļ�����Ϣ
		
		
		OFSTRUCT ofstruct;
		DWORD nBytesRead=0;
		HANDLE hFile=NULL;
		//	OVERLAPPED m_ovSend;
		//	memset(&m_ovSend, 0, sizeof(OVERLAPPED));	
		hFile=(HANDLE)OpenFile(strFile,&ofstruct,OF_SHARE_DENY_WRITE);//���ļ�
		//Sleep(1000);
		//printf("have read file\n");
		bool istrue=ReadFile(hFile, lpFileData, (DWORD)(pUpdateFileInfo->m_fileinfo.FileSize),&nBytesRead, NULL) ;//��ȡ�ļ�
		CloseHandle(hFile);//�ر��ļ�
		
		Sleep(1000);	
		////////������TFLV
		char strInt[10];
		int alllength=0;
		ZeroMemory(strInt,10);
		COperVector tmptflv;
		tmptflv.InitCreate(1,2);
		
		//����ļ���TFLV
		tmptflv.AddTFLV(CEnumCore::TagName::UpdateFileName,CEnumCore::TagFormat::TLV_STRING,strlen(pUpdateFileInfo->m_fileinfo.FileName),(unsigned char *)&pUpdateFileInfo->m_fileinfo.FileName);
		//����ļ���СTFLV
		_itoa(pUpdateFileInfo->m_fileinfo.FileSize,strInt,10);
		tmptflv.AddTFLV(CEnumCore::TagName::UpdateFileSize,CEnumCore::TagFormat::TLV_INTEGER,4,(unsigned char *)&strInt);
		
		
		int tmplength=tmptflv.EnBody();	//������Ϣ�ڴ�
		
		alllength=tmplength+pUpdateFileInfo->m_fileinfo.FileSize;	//�������峤��
		
		char * lpresult=new char[alllength];
		memcpy(lpresult,tmptflv.getDes(),tmplength);//������Ϣ����
		memcpy(lpresult+tmplength,lpFileData,pUpdateFileInfo->m_fileinfo.FileSize);//�����ļ���С
		
		// printf("111111111\n");
		
		//Sleep(1000);
		
		
		
		
		CSocketData packethead;
		
		packethead.Create(alllength);//����ռ�
		//Sleep(1000);
		packethead.AddHeader(1,CEnumCore::Msg_Category::COMMON,CEnumCore::ServiceKey::CLIENT_PATCH_UPDATE_RESP,alllength);//��ͷ
		//	printf("22222222\n");
		//return;
		Sleep(5000);
		//	packethead.EnSendBuf(&alllength,(unsigned char *)lpresult,alllength);//���������ڴ�
		packethead.EnSendBuf(&alllength,(unsigned char *)lpresult,alllength,true);
		
		//	printf("333333333\n");
		Sleep(3000);
		pSess->Send(packethead.GetSendBuf(),alllength);
		Sleep(1000);
		packethead.Destory();//����ڴ�
		
		
		
		
		delete[] lpresult;
		delete[] lpFileData;
		lpFileData=NULL;
		lpresult=NULL;
		tmptflv.Destroy();
		//	Sleep(3000);
		checkfilelist::beforhavebegin=true;
	}
	catch(...)
	{
		checkfilelist::beforhavebegin=true;
		printf("have error\n");
	}
}
//�����ļ��߳�
void CFileUpdate::UpFileThread(UpdateFileInfo * pUpdateFileInfo)
{
	try
	{
		char strFile[256];
		ZeroMemory(strFile,256);
		char * lpFileData = new char[pUpdateFileInfo->m_fileinfo.FileSize];//�����ļ���С�Ŀռ�
		ZeroMemory(lpFileData,pUpdateFileInfo->m_fileinfo.FileSize);//��տռ�
		sprintf(strFile,"%s%s",pUpdateFileInfo->m_fileinfo.FilePath,pUpdateFileInfo->m_fileinfo.FileName);//�����ļ�����Ϣ

		OFSTRUCT ofstruct;
		DWORD nBytesRead=0;
		HANDLE hFile=NULL;

		hFile=(HANDLE)OpenFile(strFile,&ofstruct,OF_SHARE_DENY_WRITE);//���ļ�
		ReadFile(hFile, lpFileData, (DWORD)(pUpdateFileInfo->m_fileinfo.FileSize),&nBytesRead, NULL) ;//��ȡ�ļ�
		CloseHandle(hFile);//�ر��ļ�

		////////������TFLV
		char strInt[10];
		int alllength=0;
		ZeroMemory(strInt,10);
		COperVector tmptflv;
		tmptflv.InitCreate(1,2);

		//����ļ���TFLV
		tmptflv.AddTFLV(CEnumCore::TagName::UpdateFileName,CEnumCore::TagFormat::TLV_STRING,strlen(pUpdateFileInfo->m_fileinfo.FileName),(unsigned char *)&pUpdateFileInfo->m_fileinfo.FileName);
		//����ļ���СTFLV
		_itoa(pUpdateFileInfo->m_fileinfo.FileSize,strInt,10);
		tmptflv.AddTFLV(CEnumCore::TagName::UpdateFileSize,CEnumCore::TagFormat::TLV_INTEGER,4,(unsigned char *)&strInt);
		
	
		int tmplength=tmptflv.EnBody();	//������Ϣ�ڴ�
		
		alllength=tmplength+pUpdateFileInfo->m_fileinfo.FileSize;	//�������峤��
		char * lpresult=new char[alllength];
		memcpy(lpresult,tmptflv.getDes(),tmplength);//������Ϣ����
		memcpy(lpresult+tmplength,lpFileData,pUpdateFileInfo->m_fileinfo.FileSize);//�����ļ���С


		CSocketData packethead;
		CMSocket selfSocket;
		selfSocket.SetSocket(pUpdateFileInfo->m_socket);//����SOCKET

		packethead.Create(alllength);//����ռ�

		packethead.AddHeader(1,CEnumCore::Msg_Category::COMMON,CEnumCore::ServiceKey::CLIENT_PATCH_UPDATE_RESP,alllength);//��ͷ


		packethead.EnSendBuf(&alllength,(unsigned char *)lpresult,alllength);//���������ڴ�

		selfSocket.SendBuf((char *)packethead.GetSendBuf(),alllength);//��������
		packethead.Destory();//����ڴ�




		delete[] lpresult;
		delete[] lpFileData;
		lpFileData=NULL;
		lpresult=NULL;
		tmptflv.Destroy();
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("GM","�����ļ��߳��ڴ治��");
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("GM","�����ļ��߳��ļ��쳣");
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("GM","�����ļ��߳������쳣");
	}
}
//�����ļ��ַ���
int  CFileUpdate::AnalogFile(char * lpstr)
{
	int FileNum=0;
	char *lpfile=strtok(lpstr,";");//��ѯ��־";"
	while(lpfile!=NULL)
	{
		sprintf(strFile,lpfile);//�õ�С�ļ��ַ���
		AnalogSingle(strFile);//����С�ļ��ַ���
		lpfile=strtok(NULL,";");//��ѯ��־";"
	}
	return FileNum;
}
//����С�ļ��ַ���
void CFileUpdate::AnalogSingle(char * lpstr)
{
	char filename[40];
	char fileversion[40];
	ZeroMemory(filename,40);
	ZeroMemory(fileversion,40);
	for(int i=0;i<strlen(lpstr);i++)
	{
		if(lpstr[i]==',')//��ѯ��־","
		{
			memcpy(filename,lpstr,i);//�õ��ļ���
			filename[i]='\0';
			break;
		}
	}

	sprintf(clientinfo.FileName,"%s",filename);//���ýṹ���ļ���
	int aa=strlen(clientinfo.FileName);//�õ��ļ�������
	for(i=0;i<strlen(lpstr+aa);i++)
	{
		if(lpstr[i+aa+1]==',')
		{
			memcpy(fileversion,lpstr+aa+1,i);//��ȡ�ļ��汾
			fileversion[i]='\0';
			break;
		}
	}
	sprintf(clientinfo.FileVersion,"%s",fileversion);//���ýṹ��汾�� 



	vector_clientfile.push_back(clientinfo);//����ͻ����ļ�����
	clientfilenum++;//�ͻ����ļ���һ

}
//�õ��ļ���С
long CFileUpdate::GetFileSize(char *filename)
{
	long siz = 0;
	FILE *fp = fopen(filename, "rb");//���ļ�
	if (fp) {
		fseek(fp, 0, SEEK_END);//��ѯָ���Ƿ����
		siz = ftell(fp);//�õ���С
		fclose(fp);//�ر��ļ�
	}
	return siz;
} 
bool checkfilelist::beforhavebegin=true;
bool checkfilelist::operupdateyesno=true;