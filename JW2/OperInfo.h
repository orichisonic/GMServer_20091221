#ifndef OPERINFO_H
#define OPERINFO_H

//#include <tchar.h>

class COperInfo
{
private:
	char m_Ip[20]; //���ͷ�����ip��ַ

	int m_iPort; //���ͷ������˿ں�

	char m_message[512]; //Ҫ�������Ϣ��������û�ID

	bool m_bConnected; //�Ƿ����ӳɹ�
public:

	//���캯��
	COperInfo();

	//��������
	~COperInfo();

	//��ʼ�����еı�����һЩ��������
	bool initialize(char *ip, int port, char *message);
	
	//�ر�DSNShell��Դ
	void Destroy();

	//���ӷ�����
	bool OnConnect();

	//�Ƿ����ӳɹ�
	bool isConnect() {return m_bConnected;}

	//���͹���
	bool SendContent();

	//����
	bool KickUser();

};
#endif

