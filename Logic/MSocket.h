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
	//������յ�������
	unsigned char* lpRecvbuf;
public:
	// ��ȡm_socket
	SOCKET m_socket;

public:	

	//��ʼ��SOCKET
	void initSocket();

	//����Զ�̷�����
	int ConnectURL(char* m_ip,int m_port);

	// ���շ��͵�����
	int MyReceive();

	int MyReceive(unsigned char * lpRecv);

	//���յ�������
    int ReceiveBuf(char* buf,int length);

	// ����ȫ������
	int recvData();

	// ��������
	int recvWAData();

	// ����ȫ�����ݲ��ж�
	bool SendData(char *msg,int wSize);

	// ���Ͳ�������
	int SendBuf(char* buff,int length);

	bool SendNewData(char *msg,int wSize);
	
	// ���Ͳ�������
	int SendNewBuf(char* buff,int length);
	
	// ����m_socket
	void SetSocket(SOCKET tmpsocket);

	//��ȡm_socket
	SOCKET GetSocket();

	//�ر�SOCKET
	void closeSocket();

	// ����m_socket
	bool MyDestory(void);

	// ���ؽ��յ�������
	unsigned char* recvBuffer();
};

#endif