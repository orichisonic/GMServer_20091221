#ifndef FILEUPDATE_H_
#define FILEUPDATE_H_

#include "Winsock2.h"
#pragma comment(lib,"Ws2_32.lib")

#include <windows.h>
#include <stdio.h>
#include <vector>
#include "OperatorGM.h"

using namespace std;
struct FileInfo
{
	char FilePath[256];
	char FileName[256];
	char FileVersion[40];
	int FileSize;
	bool flag;
	FileInfo()
	{
		FileSize=0;
		flag=1;
	}

};

struct UpdateFileInfo
{
	SOCKET m_socket;
	FileInfo m_fileinfo;
	BOOL flag;
	UpdateFileInfo()
	{
		flag=false;
	}
};

class CFileUpdate
{
public:
	//构造函数
	CFileUpdate();
	
	//析构函数
	~CFileUpdate();
private:
	CLogFile logFile;
public:
	//获取文件的版本号
	void GetVersionFromPath(char * IDEPath,char * lpVersion);
	void CompareFile(SOCKET m_socket,char * lpDllFile,char * lpExeFile);
	void CompareFile(char * lpDllFile,char * lpExeFile,CSession * pSess);
	void UpdateFile(char * filename,char * filepath,int fileSize,CSession * pSess);
	void UpdateFile(SOCKET m_socket,char * filename,char * filepath,int fileSize);
	int  AnalogFile(char * lpstr);
	void AnalogSingle(char * lpstr);
	void GetUpdateFile(char *lppath);
	void ActiveCompare();
	long GetFileSize(char *filename);
	void GetIniFile(char *lppath);
	void UpFileThread(UpdateFileInfo * pUpdateFileInfo);
	void UpFileThread(UpdateFileInfo * pUpdateFileInfo,CSession * pSess);
	
	
	UpdateFileInfo updatefileinfo;
	
	
	FileInfo info;
	FileInfo clientinfo;	
	
	vector<FileInfo> vector_clientfile;
	int clientfilenum;
	vector<FileInfo> vector_updatefile;
	int updatefilenum;
	
	vector<FileInfo> vector_inifile;
	int inifilenum;
	
	char strFile[20];
	HANDLE m_ReadMutex;
};
#endif
class checkfilelist
{
public:
	static bool beforhavebegin;
	static bool operupdateyesno;
	
};