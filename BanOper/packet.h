#ifndef PACKET_H__
#define PACKET_H__

#include <malloc.h>
#include <vector>
#include "globaldefine.h"


//包中可容内容大小
#define NIO_CONTENT_SIZE 8188


//包头
struct nio_packet_header 
{
	WORD length;
	WORD method;
};

//包
struct nio_packet
{
	nio_packet_header header;
	char content[NIO_CONTENT_SIZE];
};



//包头长度
#define NET_HEADER_LEN		sizeof(nio_packet_header)

//包长度
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

	//在包中写入指定长度的unsigned char值(目前是中间件专用)
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

	//在包中读取指定长度的unsigned char值(目前是中间件专用)
	void ReadBuff(unsigned char *val, WORD len);


	//获得整个包
	inline char* GetPack() throw()
	{
		return m_packet;
	}

	//获得包中的字符串
	inline char* GetBuffer()
	{
		return m_pBuf->content;
	}

	//向包中插入方法
	CPacket& Method(WORD method) throw();

	// 将包的协议改掉, 重置当前位置, 以便挪作他用
	void ReInit(WORD method) throw();

	//获得包中内容数据的字节数
	inline WORD GetContentLen() const throw() 
	{
		return m_pBuf->header.length - NET_HEADER_LEN;
	}

	//获得整个包的字节数
	inline WORD GetPackLen() const throw()
	{
		return m_pBuf->header.length;
	}

	//获得包中的方法
	inline WORD GetMethod() const  throw()
	{
		return m_pBuf->header.method;
	}

	//重置读取指针的位置
	void ResetReadPtr();

	//移动读指针到指定位置,len是移动量
	bool MoveReadPtr(WORD len);

	//验证对包的操作是否合法，建议在每次对包读写操作后都调用该函数判断合法性
	//返回true -- 合法；false -- 非法
	inline bool IsValid() const  throw()
	{
		return !m_overflow;
	}	

	//清除并重新初始化所有成员变量；
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
	char m_packet[NET_PACK_LEN];						//整个包
	nio_packet* m_pBuf;									//指向包的指针
	char* m_pRead;										//包中数据被读到的位置
	bool m_overflow;									//标识对包操作是否正确
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