#pragma once

#include "..\\SocketData\\SocketData.h"
#pragma comment(lib,"..\\SocketData\\debug\\SocketData.lib")

class __declspec(dllexport) CLogFile
{
public:
	//���캯��
	CLogFile(void);

	//��������
	~CLogFile(void);

	//д�ļ�
    void WriteLogFile(char * pText);

	//д��־��Ϣ
	void WriteLog(char* gameName, char* m_username,char* lpText);

	//д��־��Ϣ
	void WriteText(char* gameName, char* lpText);

	//д��ǰʱ����Ϣ
	void WriteTime();

	//�����ݿ���д�뱸����־��Ϣ
	bool WriteBakLog(int UserByID, char* SP_Name, char* ServerIP, char* Text);

	void WriteErrorLog(char * gamename,char * lpText);
	
	//�����ݿ���д����־��Ϣ
	bool WriteDBLog(int UserByID, char* GameName, char* SP_Name, char* ServerIP, char* Text);

	//��ȡ�����ļ�
	bool ReadValue(char * strFile,char * strSection,char * strKey,char * strValue, int strLength);

	//��ȡint�����ļ�
	int ReadIntValue(char * strFile,char * strSection,char * strKey);
private:
	//���������ݿ���д����־��Ϣ
	CSqlHelper m_SqlHelper;
};
