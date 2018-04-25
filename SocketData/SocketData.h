#pragma once

#include "CRC.h"

#include "..\\OperVector\\OperVector.h"
#pragma comment(lib,"..\\OperVector\\debug\\OperVector.lib")

class __declspec(dllexport) CSocketData
{
private:

	//�������õ�����ʱ����
	unsigned char *lptmpsendbuf;

	//���淢�͵�����
	unsigned char *lpsendbuf;

	//������յ�����
	unsigned char *lpDatabuf;

	//���ݰ�ͷ
	CGlobalStruct::PACKETHEAD m_packethead;

	//���ݰ���Message_Tag_ID
	CEnumCore::Message_Tag_ID m_message_tag_id;

public:
	CSocketData(void);
	~CSocketData(void);

	//��ʼ�����ݰ�
	bool Create(int length);
	
	//�õ���ǰʱ�������ݰ�ͷ��Ϣ
	bool GetNowTime(void);

	//�������ݰ���ͷ��
	bool AddHeader(unsigned int nid,CEnumCore::Msg_Category m_msg_category,CEnumCore::ServiceKey m_servicekey,unsigned int bodyLen);

	//������ݰ���ͷ��
	int AddPacketHead(unsigned char * lpDes, CGlobalStruct::PACKETHEAD m_packethead, unsigned char* lpSou);

	//�滻���ݰ��е�0xfe��0xef��0xfd
	int EncodeMessage(unsigned char* lpDes, int deslength,unsigned char* lpSou);

	//��������Ҫ���͵�����
	unsigned char * EnSendBuf(int* alllength, unsigned char * pbodybuf,int bodylen);



	//��ԭ���ݰ��е�0xfe��0xef��0xfd
	int DecodeMessage(unsigned char* lpDes, int soulength,unsigned char * lpSou);

	//�����õ����ݰ�ͷ
	int DePacketHead(unsigned char* lpSou);
	
	//�����õ����ݰ���Message_Tag_ID
	bool GetMessageID(void);

	//�������ݰ�
	bool DeHead(unsigned char * lpbuf,int length);



	//��ȡ���յ�����
	unsigned char * GetDataBuf();

	//��ȡ���͵�����
	unsigned char * GetSendBuf();

	//��ȡ���ݰ�ͷ
	CGlobalStruct::PACKETHEAD GetPacketHead();

	//�������ݰ���Message_Tag_ID
	CEnumCore::Message_Tag_ID GetMessageTagID();

	//�ͷ����ݰ�
	bool Destory(void);
	unsigned char * EnSendBuf(int* alllength, unsigned char * pbodybuf,int bodylen,bool isfileup);
    int EncodeMessage(unsigned char* lpDes, int deslength,unsigned char* lpSou,bool isfileup);
	static bool isfileupdate;
	void setfileupdatestate(bool trueorfalse);
	bool getfileupdatestate();

};
