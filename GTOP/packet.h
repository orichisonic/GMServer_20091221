#ifndef PACKET_H__
#define PACKET_H__

#include <malloc.h>
#include <vector>
#include "globaldefine.h"


//���п������ݴ�С
#define NIO_CONTENT_SIZE 8188


//��ͷ
struct nio_packet_header 
{
	WORD length;
	WORD method;
};

//��
struct nio_packet
{
	nio_packet_header header;
	char content[NIO_CONTENT_SIZE];
};



//��ͷ����
#define NET_HEADER_LEN		sizeof(nio_packet_header)

//������
#define NET_PACK_LEN		sizeof(nio_packet)




class CPacket
{
public:
	CPacket();
	CPacket(WORD method);
	virtual ~CPacket();

	//request function
	CPacket(const CPacket& res);
	const CPacket& operator=(const CPacket& res);
	CPacket& operator()(WORD method);

	CPacket& operator<<(int val);
	CPacket& operator<<(long val);
	CPacket& operator<<(double val);
	CPacket& operator<<(float val);
	CPacket& operator<<(unsigned long val);
	CPacket& operator<<(LONGLONG val);
	CPacket& operator<<(ULONGLONG val);
	CPacket& operator<<(unsigned int val);
	CPacket& operator<<(bool val) ;	
	CPacket& operator<<(short val);
	CPacket& operator<<(const tstring& val);
	CPacket& operator<<(const wstring& val);
	CPacket& operator<<(const CPacket& val);

	CPacket& operator<<(char val);
	CPacket& operator<<(unsigned char val);
	CPacket& operator<<(WORD val);
	CPacket& operator<<(const char* val);

	//�ڰ���д��ָ�����ȵ�unsigned charֵ(Ŀǰ���м��ר��)
	void WriteBuff(const unsigned char* val, WORD len);


	//response function
	CPacket& operator>>(int& val);
	CPacket& operator>>(long& val);
	CPacket& operator>>(short& val);
	CPacket& operator>>(bool& val);
	CPacket& operator>>(float& val);
	CPacket& operator>>(double& val);
	CPacket& operator>>(tstring& val);
	CPacket& operator>>(wstring& val);

	CPacket& operator>>(char& val);
	CPacket& operator>>(unsigned char& val);
	CPacket& operator>>(WORD& val);
	CPacket& operator>>(unsigned int& val);
	CPacket& operator>>(DWORD& val);
	CPacket& operator>>(LONGLONG& val);
	CPacket& operator>>(ULONGLONG& val);
	CPacket& operator>>(char* val);

	//�ڰ��ж�ȡָ�����ȵ�unsigned charֵ(Ŀǰ���м��ר��)
	void ReadBuff(unsigned char *val, WORD len);


	//���������
	inline char* GetPack() throw()
	{
		return m_packet;
	}

	//��ð��е��ַ���
	inline char* GetBuffer()
	{
		return m_pBuf->content;
	}

	//����в��뷽��
	CPacket& Method(WORD method) throw();

	// ������Э��ĵ�, ���õ�ǰλ��, �Ա�Ų������
	void ReInit(WORD method) throw();

	//��ð����������ݵ��ֽ���
	inline WORD GetContentLen() const throw() 
	{
		return m_pBuf->header.length - NET_HEADER_LEN;
	}

	//������������ֽ���
	inline WORD GetPackLen() const throw()
	{
		return m_pBuf->header.length;
	}

	//��ð��еķ���
	inline WORD GetMethod() const  throw()
	{
		return m_pBuf->header.method;
	}

	//���ö�ȡָ���λ��
	void ResetReadPtr();

	//�ƶ���ָ�뵽ָ��λ��,len���ƶ���
	bool MoveReadPtr(WORD len);

	//��֤�԰��Ĳ����Ƿ�Ϸ���������ÿ�ζ԰���д�����󶼵��øú����жϺϷ���
	//����true -- �Ϸ���false -- �Ƿ�
	inline bool IsValid() const  throw()
	{
		return !m_overflow;
	}	

	//��������³�ʼ�����г�Ա������
	inline void Clear()  throw()
	{
		reset();
	}

	inline void ClearContent() throw()
	{
		ReInit(m_pBuf->header.method);
	}

	inline bool IsEmpty() const  throw()
	{
		return (strlen(m_packet) > 0) ? true : false;
	}

private:
	void reset();

	//request function
	void addBuffer(void* val, WORD len);

	template <class T>void add(T val)
	{
		addBuffer(&val, sizeof(T));
	}


	//response function
	template<typename T> static void Get(T &val);

private:
	char m_packet[NET_PACK_LEN];						//������
	nio_packet* m_pBuf;									//ָ�����ָ��
	char* m_pRead;										//�������ݱ�������λ��
	bool m_overflow;									//��ʶ�԰������Ƿ���ȷ
};


template<class T>
CPacket& operator << (CPacket& msg,const std::vector<T> & arg)
{
	unsigned char size=static_cast<unsigned char>(arg.size());
	msg << size;
	for(unsigned char i=0;i<size;++i)
	{
		msg << arg[i];
	}		
	return msg;
}

template<class T>
CPacket& operator >> (CPacket& msg,std::vector<T>& arg)
{
	unsigned char size;
	arg.clear();
	T tObj;
	msg >> size;
	for(unsigned char i=0;i<size;++i)
	{
		msg >> tObj;
		arg.push_back(tObj);
	}

	return msg;
}



#endif