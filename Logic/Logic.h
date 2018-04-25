// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
//��ı�׼�������� DLL �е������ļ��������������϶���� LOGIC_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
//�κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ 
// LOGIC_API ������Ϊ�ǴӴ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�

#include "LogFile.h"
#include "MSocket.h"

#ifdef LOGIC_EXPORTS
#define LOGIC_API __declspec(dllexport)
#else
#define LOGIC_API __declspec(dllimport)
#endif

struct LOGSTRUCT
{
	SOCKET comsocket;
    int userID;
	char username[256];
    char clientIP[256];
    char realname[256];
    LOGSTRUCT()
	{
		comsocket=INVALID_SOCKET;
		userID=0;
		ZeroMemory(username,256);
		ZeroMemory(clientIP,256);
		ZeroMemory(realname,256);
	}
};
static LOGSTRUCT logStruct[256];

// �����Ǵ� Logic.dll ������
class LOGIC_API CLogic {
	
public:
	CLogic(void);
	~CLogic(void);
	
public:
	//����logStruct�ṹ��
	static void AddLog(SOCKET m_socket,int m_userID,char * m_UserName,char * m_realname,char * m_clientIP);
	
	////����SOCKET��ɾ����Ӧ���û���Ϣ
	static void DelLog(SOCKET  m_socket);
	
	//�����û���ID����û���
	static void FindUserName(int m_userID,char * m_UserName);
	
	//����SOCKET���õ���Ӧ���û���
	static void SocketToUserName(SOCKET m_socket,char * m_UserName);
	
	//����SOCKET���õ���Ӧ���û���ʵ����
	static void SocketToRealName(SOCKET m_socket,char * m_realname);
	
	//����SOCKET���õ���Ӧ���û�ID
	static int SocketToUserID(SOCKET m_socket);
	
	//������Ϣ
	static void ReturnMessage(SOCKET m_socket,unsigned int nid,CEnumCore::Msg_Category m_category,CEnumCore::ServiceKey mServiceKey,char * lpMessage);
	
	//�������ݼ���Ϣ
	static void ReturnData(SOCKET m_socket,unsigned int nid,CEnumCore::Msg_Category m_category,CEnumCore::ServiceKey mServiceKey,unsigned char * lpdata,int length);
	
	//��������COperVector
	static void ReturnData(SOCKET m_socket,unsigned int nid,CEnumCore::Msg_Category m_category,CEnumCore::ServiceKey mServiceKey,COperVector opervecotr);


	static void ReturnData(CEnumCore::Msg_Category m_category,CEnumCore::ServiceKey mServiceKey,COperVector opervecotr,unsigned char * m_lpRetbuf, int * m_strNum);

	static void ReturnMessage(CEnumCore::Msg_Category m_category,CEnumCore::ServiceKey mServiceKey,char * lpMessage,unsigned char * m_lpRetbuf, int * m_strNum);

	static void ReturnData(CEnumCore::Msg_Category m_category,CEnumCore::ServiceKey mServiceKey,unsigned char * lpdata,int length,unsigned char * m_lpRetbuf, int * m_strNum);

	//�ʼ�IP,�˿�,�ռ���,������,ͷ,����,����
	static bool SendMail(char *lpip,int Port,char *lpRecver,char *lpSender,char *lpHeader,char *lpSubject,char *lpBody);

};

extern LOGIC_API int nLogic;

LOGIC_API int fnLogic(void);
