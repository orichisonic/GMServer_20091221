#ifndef OPERINFO_H
#define OPERINFO_H

//#include <tchar.h>

class COperInfo
{
private:
	char m_Ip[20]; //发送服务器ip地址

	int m_iPort; //发送服务器端口号

	char m_message[512]; //要处理的信息：公告或用户ID

	bool m_bConnected; //是否连接成功
public:

	//构造函数
	COperInfo();

	//析构函数
	~COperInfo();

	//初始化类中的变量和一些基本工作
	bool initialize(char *ip, int port, char *message);
	
	//关闭DSNShell资源
	void Destroy();

	//连接服务器
	bool OnConnect();

	//是否连接成功
	bool isConnect() {return m_bConnected;}

	//发送公告
	bool SendContent();

	//踢人
	bool KickUser();

};
#endif

