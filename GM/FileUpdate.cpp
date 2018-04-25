#include "stdafx.h"
#include "FileUpdate.h"

#include "winver.h"
#pragma comment(lib,"Version.lib")

#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>


//构造函数
CFileUpdate::CFileUpdate()
{
}

//析构函数
CFileUpdate::~CFileUpdate()
{
}

//获取文件的版本号
void CFileUpdate::GetVersionFromPath(char * IDEPath,char * lpVersion)
{
	int len=GetFileVersionInfoSize(IDEPath,NULL);//得到文件版本信息大小
	char *lpData =new char [len+1];//分配内存空间
	ZeroMemory(lpData,len+1);

	BOOL bSuccess = GetFileVersionInfo(IDEPath, 0, len, lpData);//得到文件版本信息
	LPVOID lpbuf;
	UINT uLen = 0;
	bSuccess = VerQueryValue(lpData,
		TEXT("\\StringFileInfo\\080403a8\\FileVersion"), //0804中文//04b0即1252,ANSI
		&lpbuf,
		&uLen); //分析文件版本信息
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
	sprintf(lpfile,"%s%s",lppath,"DLL\\*.dll");//设置所有dll路径
	HANDLE hFind;  
	WIN32_FIND_DATA fd;  
	FileInfo info;
	if ((hFind = ::FindFirstFile(lpfile,&fd)) !=  INVALID_HANDLE_VALUE)   //查询第一个满足条件的文件 
	{
		sprintf(info.FileName,"%s",fd.cFileName);//填写结构体文件名
		sprintf(strFilePath,"%s%s%s",lppath,"DLL\\",fd.cFileName);//填写结构体文件全路径
		GetVersionFromPath(strFilePath,strVersion);//获取文件版本
		sprintf(info.FileVersion,strVersion);//填写结构体文件版本
		sprintf(info.FilePath,"%s%s",lppath,"DLL\\");//填写结构体文件路径
		info.FileSize=GetFileSize(strFilePath);//填写结构体文件大小
		vector_updatefile.push_back(info);//把结构体放入队列
		updatefilenum++;//升级文件数据加一
		
		//遍历
		while(::FindNextFile(hFind, &fd))//查询第二个满足条件的文件 
		{
			if(!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))//如果文件类型不是文件夹
			{
				sprintf(info.FileName,"%s",fd.cFileName);//填写结构体文件名
				sprintf(strFilePath,"%s%s%s",lppath,"DLL\\",fd.cFileName);//填写结构体文件全路径
				GetVersionFromPath(strFilePath,strVersion);//获取文件版本
				sprintf(info.FileVersion,strVersion);//填写结构体文件版本
				sprintf(info.FilePath,"%s%s",lppath,"DLL\\");//填写结构体文件路径
				info.FileSize=GetFileSize(strFilePath);//填写结构体文件大小
				vector_updatefile.push_back(info);//把结构体放入队列
				updatefilenum++;//升级文件数据加一
			}
		}
		::FindClose(hFind);  //关闭文件
	}   
	


	sprintf(lpfile,"%s%s",lppath,"EXE\\*.exe");//设置所有exe路径
	if ((hFind = ::FindFirstFile(lpfile,&fd)) !=  INVALID_HANDLE_VALUE)     //查询第一个满足条件的文件 
	{
		sprintf(info.FileName,"%s",fd.cFileName);//填写结构体文件名
		sprintf(strFilePath,"%s%s%s",lppath,"EXE\\",fd.cFileName);//填写结构体文件全路径
		GetVersionFromPath(strFilePath,strVersion);//获取文件版本
		sprintf(info.FileVersion,strVersion);//填写结构体文件版本
		sprintf(info.FilePath,"%s%s",lppath,"EXE\\");//填写结构体文件路径
		info.FileSize=GetFileSize(strFilePath);//填写结构体文件大小
		vector_updatefile.push_back(info);//把结构体放入队列
		updatefilenum++;//升级文件数据加一

		//遍历
		while(::FindNextFile(hFind, &fd))//查询第二个满足条件的文件 
		{
			if(!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))//如果文件类型不是文件夹
			{
				sprintf(info.FileName,"%s",fd.cFileName);//填写结构体文件名
				sprintf(strFilePath,"%s%s%s",lppath,"EXE\\",fd.cFileName);//填写结构体文件全路径
				GetVersionFromPath(strFilePath,strVersion);//获取文件版本
				sprintf(info.FileVersion,strVersion);//填写结构体文件版本
				sprintf(info.FilePath,"%s%s",lppath,"EXE\\");//填写结构体文件路径
				info.FileSize=GetFileSize(strFilePath);//填写结构体文件大小
				vector_updatefile.push_back(info);//把结构体放入队列
				updatefilenum++;//升级文件数据加一
			}
		}
		::FindClose(hFind);   //关闭文件
	}   

}
void CFileUpdate::ActiveCompare()
{
	int i=0,j=0;
	FileInfo clientfileinfo,updatefileinfo;
	for(i=0;i<clientfilenum;i++)
	{
		clientfileinfo=vector_clientfile.at(i);//取客户端文件信息
		for(j=0;j<updatefilenum;j++)
		{
			updatefileinfo=vector_updatefile.at(j);//取更新文件信息
			if(!strcmp(clientfileinfo.FileName,updatefileinfo.FileName))//比较文件名是否一样
			{
				int aaa=strcmp(clientfileinfo.FileVersion,updatefileinfo.FileVersion);//比较文件版本号
				if(strcmp(clientfileinfo.FileVersion,updatefileinfo.FileVersion)>=0)//如果客户端比服务端文件版本号大
				{
					vector_updatefile[j].flag=0;//设置标志不用更新
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
	sprintf(lpfile,"%s%s",lppath,"INI\\*.ini");//设置所有INI路径
	if ((hFind = ::FindFirstFile(lpfile,&fd)) !=  INVALID_HANDLE_VALUE)     //查询第一个满足条件的文件 
	{
		sprintf(info.FileName,"%s",fd.cFileName);//填写结构体文件名
		sprintf(strFilePath,"%s%s%s",lppath,"INI\\",fd.cFileName);//填写结构体文件全路径
		sprintf(info.FileVersion,"0.0.0.0");//填写结构体文件版本
		sprintf(info.FilePath,"%s%s",lppath,"INI\\");//填写结构体文件路径
		info.FileSize=GetFileSize(strFilePath);//填写结构体文件大小
		vector_inifile.push_back(info);//把结构体放入队列
		inifilenum++;

		//遍历
		while(::FindNextFile(hFind, &fd))//查询第二个满足条件的文件 
		{
			if(!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))//如果文件类型不是文件夹
			{
				sprintf(info.FileName,"%s",fd.cFileName);//填写结构体文件名
				sprintf(strFilePath,"%s%s%s",lppath,"INI\\",fd.cFileName);//填写结构体文件全路径
				sprintf(info.FileVersion,"0.0.0.0");//填写结构体文件版本
				sprintf(info.FilePath,"%s%s",lppath,"INI\\");//填写结构体文件路径
				info.FileSize=GetFileSize(strFilePath);//填写结构体文件大小
				vector_inifile.push_back(info);//把结构体放入队列
				inifilenum++;
			}
		}
		::FindClose(hFind);  //关闭文件
	}  
}

/////////////////////////////////////
//函数名CompareFile比较文件
////////////////////
void CFileUpdate::CompareFile(SOCKET m_socket,char * lpDllFile,char * lpExeFile)
{
	try
	{
		///分析包
		vector_clientfile.clear();//清空客户端文件队列
		clientfilenum=0;//设置客户端文件数量为0
		AnalogFile(lpDllFile);//分析dll文件字符串
		AnalogFile(lpExeFile);//分析exe文件字符串
		///得到需要更新的文件目录
		char path[256];
		ZeroMemory(path,256);
		GetCurrentDirectory(256,path);//得到当前目录
		char updatefilepath[256];
		ZeroMemory(updatefilepath,256);
		wsprintf(updatefilepath,"%s%s",path,"\\patch\\");//获得更新目录
		vector_updatefile.clear();//清空更新文件队列
		updatefilenum=0;//设置更新文件数量为0
		GetUpdateFile(updatefilepath);//得到更新文件

		ActiveCompare();//比较文件
		COperVector opervector;

		if(updatefilenum==0)
		{
			opervector.InitCreate(1,6);//分配空间
		}
		else
		{
			inifilenum=0;//设置ini文件数量为0
			vector_inifile.clear();//清空ini队列
			GetIniFile(updatefilepath);//得到ini文件信息
			opervector.InitCreate(updatefilenum+inifilenum+5,6);//分配空间
		}
		/////////找所有文件得到文件信息


		char strInt[10];
		ZeroMemory(strInt,10);
		bool updateflag=false;
		for(int i=0;i<updatefilenum;i++)
		{
			if(vector_updatefile[i].flag==1)//标志为要更新
			{
				updateflag=true;//文件更新标记为true
				//添加更新文件名TFLV
				opervector.AddTFLV(CEnumCore::TagName::UpdateFileName,CEnumCore::TagFormat::TLV_STRING,strlen(vector_updatefile[i].FileName),(unsigned char *)&vector_updatefile[i].FileName);
				//添加更新文件版本TFLV
				opervector.AddTFLV(CEnumCore::TagName::UpdateFileVersion,CEnumCore::TagFormat::TLV_STRING,strlen(vector_updatefile[i].FileVersion),(unsigned char *)&vector_updatefile[i].FileVersion);
				//添加更新文件路径TFLV
				opervector.AddTFLV(CEnumCore::TagName::UpdateFilePath,CEnumCore::TagFormat::TLV_STRING,strlen(vector_updatefile[i].FilePath),(unsigned char *)&vector_updatefile[i].FilePath);
				_itoa(vector_updatefile[i].FileSize,strInt,10);
				//添加更新文件大小TFLV
				opervector.AddTFLV(CEnumCore::TagName::UpdateFileSize,CEnumCore::TagFormat::TLV_INTEGER,4,(unsigned char *)&strInt);

			}
		}
		if(updateflag==true)//如果updateflag为true
		{
			inifilenum=0;
			vector_inifile.clear();
			GetIniFile(updatefilepath);//得到ini文件信息
			for(int i=0;i<inifilenum;i++)
			{
				if(vector_inifile[i].flag==1)
				{
					//添加更新文件名TFLV
					opervector.AddTFLV(CEnumCore::TagName::UpdateFileName,CEnumCore::TagFormat::TLV_STRING,strlen(vector_inifile[i].FileName),(unsigned char *)&vector_inifile[i].FileName);
					//添加更新文件版本TFLV
					opervector.AddTFLV(CEnumCore::TagName::UpdateFileVersion,CEnumCore::TagFormat::TLV_STRING,strlen(vector_inifile[i].FileVersion),(unsigned char *)&vector_inifile[i].FileVersion);
					//添加更新文件路径TFLV
					opervector.AddTFLV(CEnumCore::TagName::UpdateFilePath,CEnumCore::TagFormat::TLV_STRING,strlen(vector_inifile[i].FilePath),(unsigned char *)&vector_inifile[i].FilePath);
					_itoa(vector_inifile[i].FileSize,strInt,10);
					//添加更新文件大小TFLV
					opervector.AddTFLV(CEnumCore::TagName::UpdateFileSize,CEnumCore::TagFormat::TLV_INTEGER,4,(unsigned char *)&strInt);

				}
			}


		}
		if(updateflag==false)//如果updateflag为false,没有文件更新
		{
			//添加没有文件更新消息TFLV
			opervector.AddTFLV(CEnumCore::TagName::MESSAGE,CEnumCore::TagFormat::TLV_STRING,strlen("FAILURE"),(unsigned char *)&"FAILURE");

		}	
		CSocketData socketdata;
		CMSocket msocket;
		msocket.SetSocket(m_socket);//设置SOCKET

		int alllength=0;
		alllength=opervector.EnBody();//构建内存段
		socketdata.Create(alllength);//分配空间

		socketdata.AddHeader(1,CEnumCore::Msg_Category::COMMON,CEnumCore::ServiceKey::CLIENT_PATCH_COMPARE_RESP,alllength);//加头


		socketdata.EnSendBuf(&alllength,opervector.getDes(),alllength);//构建发送内存

		msocket.SendBuf((char *)socketdata.GetSendBuf(),alllength);//发送内存
		socketdata.Destory();//回收内存段

	}
	catch(...)
	{

	}
}

/////////////////////////////////////
//函数名CompareFile比较文件
////////////////////
void CFileUpdate::CompareFile(char * lpDllFile,char * lpExeFile,CSession * pSess)
{
	try
	{
		///分析包
		vector_clientfile.clear();//清空客户端文件队列
		clientfilenum=0;//设置客户端文件数量为0
		AnalogFile(lpDllFile);//分析dll文件字符串
		AnalogFile(lpExeFile);//分析exe文件字符串
		///得到需要更新的文件目录
		char path[256];
		ZeroMemory(path,256);
		GetCurrentDirectory(256,path);//得到当前目录
		char updatefilepath[256];
		ZeroMemory(updatefilepath,256);
		wsprintf(updatefilepath,"%s%s",path,"\\patch\\");//获得更新目录
		vector_updatefile.clear();//清空更新文件队列
		updatefilenum=0;//设置更新文件数量为0
		GetUpdateFile(updatefilepath);//得到更新文件

		ActiveCompare();//比较文件
		COperVector opervector;

		if(updatefilenum==0)
		{
			opervector.InitCreate(1,6);//分配空间
		}
		else
		{
			inifilenum=0;//设置ini文件数量为0
			vector_inifile.clear();//清空ini队列
			GetIniFile(updatefilepath);//得到ini文件信息
			opervector.InitCreate(updatefilenum+inifilenum+5,6);//分配空间
		}
		/////////找所有文件得到文件信息


		char strInt[10];
		ZeroMemory(strInt,10);
		bool updateflag=false;
		for(int i=0;i<updatefilenum;i++)
		{
			if(vector_updatefile[i].flag==1)//标志为要更新
			{
				updateflag=true;//文件更新标记为true
				//添加更新文件名TFLV
				opervector.AddTFLV(CEnumCore::TagName::UpdateFileName,CEnumCore::TagFormat::TLV_STRING,strlen(vector_updatefile[i].FileName),(unsigned char *)&vector_updatefile[i].FileName);
				//添加更新文件版本TFLV
				opervector.AddTFLV(CEnumCore::TagName::UpdateFileVersion,CEnumCore::TagFormat::TLV_STRING,strlen(vector_updatefile[i].FileVersion),(unsigned char *)&vector_updatefile[i].FileVersion);
				//添加更新文件路径TFLV
				opervector.AddTFLV(CEnumCore::TagName::UpdateFilePath,CEnumCore::TagFormat::TLV_STRING,strlen(vector_updatefile[i].FilePath),(unsigned char *)&vector_updatefile[i].FilePath);
				_itoa(vector_updatefile[i].FileSize,strInt,10);
				//添加更新文件大小TFLV
				opervector.AddTFLV(CEnumCore::TagName::UpdateFileSize,CEnumCore::TagFormat::TLV_INTEGER,4,(unsigned char *)&strInt);

			}
		}
		if(updateflag==true)//如果updateflag为true
		{
			inifilenum=0;
			vector_inifile.clear();
			GetIniFile(updatefilepath);//得到ini文件信息
			for(int i=0;i<inifilenum;i++)
			{
				if(vector_inifile[i].flag==1)
				{
					//添加更新文件名TFLV
					opervector.AddTFLV(CEnumCore::TagName::UpdateFileName,CEnumCore::TagFormat::TLV_STRING,strlen(vector_inifile[i].FileName),(unsigned char *)&vector_inifile[i].FileName);
					//添加更新文件版本TFLV
					opervector.AddTFLV(CEnumCore::TagName::UpdateFileVersion,CEnumCore::TagFormat::TLV_STRING,strlen(vector_inifile[i].FileVersion),(unsigned char *)&vector_inifile[i].FileVersion);
					//添加更新文件路径TFLV
					opervector.AddTFLV(CEnumCore::TagName::UpdateFilePath,CEnumCore::TagFormat::TLV_STRING,strlen(vector_inifile[i].FilePath),(unsigned char *)&vector_inifile[i].FilePath);
					_itoa(vector_inifile[i].FileSize,strInt,10);
					//添加更新文件大小TFLV
					opervector.AddTFLV(CEnumCore::TagName::UpdateFileSize,CEnumCore::TagFormat::TLV_INTEGER,4,(unsigned char *)&strInt);

				}
			}


		}
		if(updateflag==false)//如果updateflag为false,没有文件更新
		{
			//添加没有文件更新消息TFLV
			opervector.AddTFLV(CEnumCore::TagName::MESSAGE,CEnumCore::TagFormat::TLV_STRING,strlen("FAILURE"),(unsigned char *)&"FAILURE");
		}	
		CSocketData socketdata;
		int alllength=0;
		alllength=opervector.EnBody();//构建内存段
		socketdata.Create(alllength);//分配空间
		socketdata.AddHeader(1,CEnumCore::Msg_Category::COMMON,CEnumCore::ServiceKey::CLIENT_PATCH_COMPARE_RESP,alllength);//加头
		socketdata.EnSendBuf(&alllength,opervector.getDes(),alllength);//构建发送内存
		pSess->Send(socketdata.GetSendBuf(),alllength);
		socketdata.Destory();//回收内存段

	}
	catch(...)
	{

	}
}

//更新文件
void CFileUpdate::UpdateFile(SOCKET m_socket,char * filename,char * filepath,int fileSize)
{
	try
	{
		updatefileinfo.flag=true;//更新文件标志
		updatefileinfo.m_socket=m_socket;//更新文件的Socket
		sprintf(updatefileinfo.m_fileinfo.FileName,filename);//更新文件的文件名
		sprintf(updatefileinfo.m_fileinfo.FilePath,filepath);//更新文件的路径
		updatefileinfo.m_fileinfo.FileSize=fileSize;//更新文件的大小
		UpFileThread(&updatefileinfo);//开始更新文件线程
	}
	catch(...)
	{

	}
	

	
}

//更新文件
void CFileUpdate::UpdateFile(char * filename,char * filepath,int fileSize,CSession *pSess)
{
	try
	{
		updatefileinfo.flag=true;//更新文件标志
		sprintf(updatefileinfo.m_fileinfo.FileName,filename);//更新文件的文件名
		sprintf(updatefileinfo.m_fileinfo.FilePath,filepath);//更新文件的路径
		updatefileinfo.m_fileinfo.FileSize=fileSize;//更新文件的大小
		UpFileThread(&updatefileinfo,pSess);//开始更新文件线程
	}
	catch(...)
	{
		
	}
	
	
	
}

//更新文件线程
void CFileUpdate::UpFileThread(UpdateFileInfo * pUpdateFileInfo,CSession * pSess)
{
	try
	{
		
		char strFile[256];
		ZeroMemory(strFile,256);
		char * lpFileData = new char[pUpdateFileInfo->m_fileinfo.FileSize];//分配文件大小的空间
		ZeroMemory(lpFileData,pUpdateFileInfo->m_fileinfo.FileSize);//清空空间
		sprintf(strFile,"%s%s",pUpdateFileInfo->m_fileinfo.FilePath,pUpdateFileInfo->m_fileinfo.FileName);//构成文件名信息
		
		
		OFSTRUCT ofstruct;
		DWORD nBytesRead=0;
		HANDLE hFile=NULL;
		//	OVERLAPPED m_ovSend;
		//	memset(&m_ovSend, 0, sizeof(OVERLAPPED));	
		hFile=(HANDLE)OpenFile(strFile,&ofstruct,OF_SHARE_DENY_WRITE);//打开文件
		//Sleep(1000);
		//printf("have read file\n");
		bool istrue=ReadFile(hFile, lpFileData, (DWORD)(pUpdateFileInfo->m_fileinfo.FileSize),&nBytesRead, NULL) ;//读取文件
		CloseHandle(hFile);//关闭文件
		
		Sleep(1000);	
		////////加两个TFLV
		char strInt[10];
		int alllength=0;
		ZeroMemory(strInt,10);
		COperVector tmptflv;
		tmptflv.InitCreate(1,2);
		
		//添加文件名TFLV
		tmptflv.AddTFLV(CEnumCore::TagName::UpdateFileName,CEnumCore::TagFormat::TLV_STRING,strlen(pUpdateFileInfo->m_fileinfo.FileName),(unsigned char *)&pUpdateFileInfo->m_fileinfo.FileName);
		//添加文件大小TFLV
		_itoa(pUpdateFileInfo->m_fileinfo.FileSize,strInt,10);
		tmptflv.AddTFLV(CEnumCore::TagName::UpdateFileSize,CEnumCore::TagFormat::TLV_INTEGER,4,(unsigned char *)&strInt);
		
		
		int tmplength=tmptflv.EnBody();	//构成信息内存
		
		alllength=tmplength+pUpdateFileInfo->m_fileinfo.FileSize;	//计算总体长度
		
		char * lpresult=new char[alllength];
		memcpy(lpresult,tmptflv.getDes(),tmplength);//复制信息内容
		memcpy(lpresult+tmplength,lpFileData,pUpdateFileInfo->m_fileinfo.FileSize);//复制文件大小
		
		// printf("111111111\n");
		
		//Sleep(1000);
		
		
		
		
		CSocketData packethead;
		
		packethead.Create(alllength);//分配空间
		//Sleep(1000);
		packethead.AddHeader(1,CEnumCore::Msg_Category::COMMON,CEnumCore::ServiceKey::CLIENT_PATCH_UPDATE_RESP,alllength);//加头
		//	printf("22222222\n");
		//return;
		Sleep(5000);
		//	packethead.EnSendBuf(&alllength,(unsigned char *)lpresult,alllength);//构建发送内存
		packethead.EnSendBuf(&alllength,(unsigned char *)lpresult,alllength,true);
		
		//	printf("333333333\n");
		Sleep(3000);
		pSess->Send(packethead.GetSendBuf(),alllength);
		Sleep(1000);
		packethead.Destory();//清空内存
		
		
		
		
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
//更新文件线程
void CFileUpdate::UpFileThread(UpdateFileInfo * pUpdateFileInfo)
{
	try
	{
		char strFile[256];
		ZeroMemory(strFile,256);
		char * lpFileData = new char[pUpdateFileInfo->m_fileinfo.FileSize];//分配文件大小的空间
		ZeroMemory(lpFileData,pUpdateFileInfo->m_fileinfo.FileSize);//清空空间
		sprintf(strFile,"%s%s",pUpdateFileInfo->m_fileinfo.FilePath,pUpdateFileInfo->m_fileinfo.FileName);//构成文件名信息

		OFSTRUCT ofstruct;
		DWORD nBytesRead=0;
		HANDLE hFile=NULL;

		hFile=(HANDLE)OpenFile(strFile,&ofstruct,OF_SHARE_DENY_WRITE);//打开文件
		ReadFile(hFile, lpFileData, (DWORD)(pUpdateFileInfo->m_fileinfo.FileSize),&nBytesRead, NULL) ;//读取文件
		CloseHandle(hFile);//关闭文件

		////////加两个TFLV
		char strInt[10];
		int alllength=0;
		ZeroMemory(strInt,10);
		COperVector tmptflv;
		tmptflv.InitCreate(1,2);

		//添加文件名TFLV
		tmptflv.AddTFLV(CEnumCore::TagName::UpdateFileName,CEnumCore::TagFormat::TLV_STRING,strlen(pUpdateFileInfo->m_fileinfo.FileName),(unsigned char *)&pUpdateFileInfo->m_fileinfo.FileName);
		//添加文件大小TFLV
		_itoa(pUpdateFileInfo->m_fileinfo.FileSize,strInt,10);
		tmptflv.AddTFLV(CEnumCore::TagName::UpdateFileSize,CEnumCore::TagFormat::TLV_INTEGER,4,(unsigned char *)&strInt);
		
	
		int tmplength=tmptflv.EnBody();	//构成信息内存
		
		alllength=tmplength+pUpdateFileInfo->m_fileinfo.FileSize;	//计算总体长度
		char * lpresult=new char[alllength];
		memcpy(lpresult,tmptflv.getDes(),tmplength);//复制信息内容
		memcpy(lpresult+tmplength,lpFileData,pUpdateFileInfo->m_fileinfo.FileSize);//复制文件大小


		CSocketData packethead;
		CMSocket selfSocket;
		selfSocket.SetSocket(pUpdateFileInfo->m_socket);//设置SOCKET

		packethead.Create(alllength);//分配空间

		packethead.AddHeader(1,CEnumCore::Msg_Category::COMMON,CEnumCore::ServiceKey::CLIENT_PATCH_UPDATE_RESP,alllength);//加头


		packethead.EnSendBuf(&alllength,(unsigned char *)lpresult,alllength);//构建发送内存

		selfSocket.SendBuf((char *)packethead.GetSendBuf(),alllength);//发送内容
		packethead.Destory();//清空内存




		delete[] lpresult;
		delete[] lpFileData;
		lpFileData=NULL;
		lpresult=NULL;
		tmptflv.Destroy();
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("GM","更新文件线程内存不足");
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("GM","更新文件线程文件异常");
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("GM","更新文件线程其他异常");
	}
}
//分析文件字符串
int  CFileUpdate::AnalogFile(char * lpstr)
{
	int FileNum=0;
	char *lpfile=strtok(lpstr,";");//查询标志";"
	while(lpfile!=NULL)
	{
		sprintf(strFile,lpfile);//得到小文件字符串
		AnalogSingle(strFile);//分析小文件字符串
		lpfile=strtok(NULL,";");//查询标志";"
	}
	return FileNum;
}
//分析小文件字符串
void CFileUpdate::AnalogSingle(char * lpstr)
{
	char filename[40];
	char fileversion[40];
	ZeroMemory(filename,40);
	ZeroMemory(fileversion,40);
	for(int i=0;i<strlen(lpstr);i++)
	{
		if(lpstr[i]==',')//查询标志","
		{
			memcpy(filename,lpstr,i);//得到文件名
			filename[i]='\0';
			break;
		}
	}

	sprintf(clientinfo.FileName,"%s",filename);//设置结构体文件名
	int aa=strlen(clientinfo.FileName);//得到文件名长度
	for(i=0;i<strlen(lpstr+aa);i++)
	{
		if(lpstr[i+aa+1]==',')
		{
			memcpy(fileversion,lpstr+aa+1,i);//获取文件版本
			fileversion[i]='\0';
			break;
		}
	}
	sprintf(clientinfo.FileVersion,"%s",fileversion);//设置结构体版本号 



	vector_clientfile.push_back(clientinfo);//放入客户端文件队列
	clientfilenum++;//客户端文件加一

}
//得到文件大小
long CFileUpdate::GetFileSize(char *filename)
{
	long siz = 0;
	FILE *fp = fopen(filename, "rb");//打开文件
	if (fp) {
		fseek(fp, 0, SEEK_END);//查询指针是否截至
		siz = ftell(fp);//得到大小
		fclose(fp);//关闭文件
	}
	return siz;
} 
bool checkfilelist::beforhavebegin=true;
bool checkfilelist::operupdateyesno=true;