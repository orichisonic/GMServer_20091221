#ifndef MSOCKET_H_
#define MSOCKET_H_

#include "Winsock2.h"
#pragma comment(lib,"Ws2_32.lib")

class  __declspec(dllexport) CMSocket
{
public:
	CMSocket(void);
	~CMSocket(void);

private:
	//保存接收到的数据
	unsigned char* lpRecvbuf;
public:
	// 获取m_socket
	SOCKET m_socket;

public:	

	//初始化SOCKET
	void initSocket();

	//连接远程服务器
	int ConnectURL(char* m_ip,int m_port);

	// 接收发送的数据
	int MyReceive();

	int MyReceive(unsigned char * lpRecv);

	//接收单个数据
    int ReceiveBuf(char* buf,int length);

	// 接收全部数据
	int recvData();

	// 接收数据
	int recvWAData();

	// 发送全部数据并判断
	bool SendData(char *msg,int wSize);

	// 发送部分数据
	int SendBuf(char* buff,int length);

	bool SendNewData(char *msg,int wSize);
	
	// 发送部分数据
	int SendNewBuf(char* buff,int length);
	
	// 设置m_socket
	void SetSocket(SOCKET tmpsocket);

	//获取m_socket
	SOCKET GetSocket();

	//关闭SOCKET
	void closeSocket();

	// 销毁m_socket
	bool MyDestory(void);

	// 返回接收到的数据
	unsigned char* recvBuffer();
};

#endif