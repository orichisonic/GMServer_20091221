#pragma once

#include "CRC.h"

#include "..\\OperVector\\OperVector.h"
#pragma comment(lib,"..\\OperVector\\debug\\OperVector.lib")

class __declspec(dllexport) CSocketData
{
private:

	//解析中用到的临时数据
	unsigned char *lptmpsendbuf;

	//保存发送的数据
	unsigned char *lpsendbuf;

	//保存接收的数据
	unsigned char *lpDatabuf;

	//数据包头
	CGlobalStruct::PACKETHEAD m_packethead;

	//数据包的Message_Tag_ID
	CEnumCore::Message_Tag_ID m_message_tag_id;

public:
	CSocketData(void);
	~CSocketData(void);

	//初始化数据包
	bool Create(int length);
	
	//得到当前时间作数据包头信息
	bool GetNowTime(void);

	//构造数据包的头部
	bool AddHeader(unsigned int nid,CEnumCore::Msg_Category m_msg_category,CEnumCore::ServiceKey m_servicekey,unsigned int bodyLen);

	//添加数据包的头部
	int AddPacketHead(unsigned char * lpDes, CGlobalStruct::PACKETHEAD m_packethead, unsigned char* lpSou);

	//替换数据包中的0xfe、0xef和0xfd
	int EncodeMessage(unsigned char* lpDes, int deslength,unsigned char* lpSou);

	//构造最终要发送的数据
	unsigned char * EnSendBuf(int* alllength, unsigned char * pbodybuf,int bodylen);



	//还原数据包中的0xfe、0xef和0xfd
	int DecodeMessage(unsigned char* lpDes, int soulength,unsigned char * lpSou);

	//解析得到数据包头
	int DePacketHead(unsigned char* lpSou);
	
	//解析得到数据包的Message_Tag_ID
	bool GetMessageID(void);

	//解析数据包
	bool DeHead(unsigned char * lpbuf,int length);



	//获取接收的数据
	unsigned char * GetDataBuf();

	//获取发送的数据
	unsigned char * GetSendBuf();

	//获取数据包头
	CGlobalStruct::PACKETHEAD GetPacketHead();

	//返回数据包的Message_Tag_ID
	CEnumCore::Message_Tag_ID GetMessageTagID();

	//释放数据包
	bool Destory(void);
	unsigned char * EnSendBuf(int* alllength, unsigned char * pbodybuf,int bodylen,bool isfileup);
    int EncodeMessage(unsigned char* lpDes, int deslength,unsigned char* lpSou,bool isfileup);
	static bool isfileupdate;
	void setfileupdatestate(bool trueorfalse);
	bool getfileupdatestate();

};
