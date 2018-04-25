// 下列 ifdef 块是创建使从 DLL 导出更简单的
//宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 LOGIC_EXPORTS
// 符号编译的。在使用此 DLL 的
//任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将 
// LOGIC_API 函数视为是从此 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。

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

// 此类是从 Logic.dll 导出的
class LOGIC_API CLogic {
	
public:
	CLogic(void);
	~CLogic(void);
	
public:
	//构造logStruct结构体
	static void AddLog(SOCKET m_socket,int m_userID,char * m_UserName,char * m_realname,char * m_clientIP);
	
	////根据SOCKET，删除相应的用户信息
	static void DelLog(SOCKET  m_socket);
	
	//根据用户的ID获得用户名
	static void FindUserName(int m_userID,char * m_UserName);
	
	//根据SOCKET，得到相应的用户名
	static void SocketToUserName(SOCKET m_socket,char * m_UserName);
	
	//根据SOCKET，得到相应的用户真实姓名
	static void SocketToRealName(SOCKET m_socket,char * m_realname);
	
	//根据SOCKET，得到相应的用户ID
	static int SocketToUserID(SOCKET m_socket);
	
	//返回消息
	static void ReturnMessage(SOCKET m_socket,unsigned int nid,CEnumCore::Msg_Category m_category,CEnumCore::ServiceKey mServiceKey,char * lpMessage);
	
	//返回数据集消息
	static void ReturnData(SOCKET m_socket,unsigned int nid,CEnumCore::Msg_Category m_category,CEnumCore::ServiceKey mServiceKey,unsigned char * lpdata,int length);
	
	//返回数据COperVector
	static void ReturnData(SOCKET m_socket,unsigned int nid,CEnumCore::Msg_Category m_category,CEnumCore::ServiceKey mServiceKey,COperVector opervecotr);


	static void ReturnData(CEnumCore::Msg_Category m_category,CEnumCore::ServiceKey mServiceKey,COperVector opervecotr,unsigned char * m_lpRetbuf, int * m_strNum);

	static void ReturnMessage(CEnumCore::Msg_Category m_category,CEnumCore::ServiceKey mServiceKey,char * lpMessage,unsigned char * m_lpRetbuf, int * m_strNum);

	static void ReturnData(CEnumCore::Msg_Category m_category,CEnumCore::ServiceKey mServiceKey,unsigned char * lpdata,int length,unsigned char * m_lpRetbuf, int * m_strNum);

	//邮件IP,端口,收件人,发件人,头,主题,内容
	static bool SendMail(char *lpip,int Port,char *lpRecver,char *lpSender,char *lpHeader,char *lpSubject,char *lpBody);

};

extern LOGIC_API int nLogic;

LOGIC_API int fnLogic(void);
