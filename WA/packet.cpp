#include "StdAfx.h"
#include "packet.h"

CPacket::CPacket()
{
	reset();
	m_pBuf->header.length = NET_HEADER_LEN;
}

CPacket::CPacket(WORD method)
{
	reset();
	m_pBuf->header.length = NET_HEADER_LEN;
	m_pBuf->header.method = method;
}


CPacket::~CPacket()
{
	m_pBuf = NULL;
	m_pRead = NULL;
}

CPacket::CPacket(const CPacket& res)
{
	reset();
	memcpy(m_packet, res.m_packet, res.m_pBuf->header.length);
	m_overflow = res.m_overflow;
}

CPacket& CPacket::Method(WORD method) throw()
{
	m_pBuf->header.method = method;
	return *this;
}

void CPacket::ReInit(WORD method) throw()
{
	m_overflow = false;
	m_pBuf = (nio_packet*)m_packet;
	m_pBuf->header.length = NET_HEADER_LEN;
	m_pBuf->header.method = method;
	m_pRead = m_pBuf->content; //+0
}

const CPacket& CPacket::operator=(const CPacket& res)
{
	reset();
	memcpy(m_packet, res.m_packet, res.m_pBuf->header.length);
	m_overflow = res.m_overflow;
	return *this;
}

CPacket& CPacket::operator()(WORD method)
{
	m_pBuf->header.method = method;
	return *this;
}


CPacket& CPacket::operator<<(int val)
{
	add(val);
	return *this;
}

void CPacket::ResetReadPtr()
{
	m_overflow = false;
	m_pRead = m_pBuf->content;	
}

/** 随意移动读指针
*/
bool CPacket::MoveReadPtr(WORD len)
{
	len = len - 4;	//减去包头长度
	m_pRead = m_pBuf->content;
	m_pRead += len;//移动读指针

	if((m_pRead >= ((m_pBuf->content)+(m_pBuf->header.length - NET_HEADER_LEN))) || (m_pRead < m_pBuf->content) ) //如果超出content范围就不能移动该指针
	{
		return false;
	}

	return true;
}

CPacket& CPacket::operator<<(long val)
{
	add(val);
	return *this;
}

CPacket& CPacket::operator<<(double val)
{
	add(val);
	return *this;
}

CPacket& CPacket::operator<<(float val)
{
	add(val);
	return *this;
}

CPacket& CPacket::operator<<(unsigned long val)
{
	add(val);
	return *this;
}

CPacket& CPacket::operator<<(LONGLONG val)
{
	add(val);
	return *this;
}

CPacket& CPacket::operator<<(ULONGLONG val)
{
	add(val);
	return *this;
}

CPacket& CPacket::operator<<(unsigned int val)
{
	add(val);
	return *this;
}

CPacket& CPacket::operator<<(bool val) 
{
	add(val);
	return *this;
}

CPacket& CPacket::operator<<(short val)
{
	add(val);
	return *this;
}

CPacket& CPacket::operator<<(const tstring& val)
{
	addBuffer(LPVOID(val.c_str()), WORD(val.length()+1));
	return *this;
}

CPacket& CPacket::operator <<(const wstring& val)
{
	int len = static_cast<int>(val.length());
	this->add(len);
	this->addBuffer((char *)(val.c_str()),static_cast<WORD>(len*sizeof(WCHAR)/sizeof(CHAR)));
	return *this;
}

CPacket& CPacket::operator<<(const CPacket &val)
{
	if (m_pBuf->header.length + val.m_pBuf->header.length > NET_PACK_LEN)
	{
		m_overflow = true;
		return *this;
	}

	memcpy(m_pBuf->content+m_pBuf->header.length - NET_HEADER_LEN, val.m_pRead, val.m_pBuf->content+val.GetContentLen()-val.m_pRead);
	m_pBuf->header.length += static_cast<WORD>(val.m_pBuf->content+val.GetContentLen()-val.m_pRead);

	return *this;
}

CPacket& CPacket::operator<<(char val)
{
	add(val);
	return *this;
}

CPacket& CPacket::operator<<(unsigned char val)
{
	add(val);
	return *this;
}

CPacket& CPacket::operator<<(WORD val)
{
	add(val);
	return *this;
}

CPacket& CPacket::operator<<(const char* val)
{
	WORD len = (WORD)strlen(val) + 1;
	addBuffer(LPVOID(val), len);
	return *this;
}

void CPacket::WriteBuff(const unsigned char* val, WORD len)
{
	addBuffer(LPVOID(val), len);
}

void CPacket::addBuffer(void* val, WORD len)
{
	if ((len + m_pBuf->header.length) > NET_PACK_LEN)
	{
		m_overflow = true;
		return;
	}

	memcpy(m_pBuf->content+m_pBuf->header.length - NET_HEADER_LEN, val, len);

	m_pBuf->header.length += len;
}


template<class T> void CPacket::Get(T& val)
{
	if (m_pRead + sizeof(T) <= m_pBuf->content + m_pBuf->header.length - NET_HEADER_LEN) {
		memcpy(&val, m_pRead, sizeof(T));    
		m_pRead += sizeof(val);
	}
	else {
		m_overflow = true;
	}
}

CPacket& CPacket::operator>>(int& val)
{
	if (m_pRead + sizeof(int) <= m_pBuf->content + m_pBuf->header.length - NET_HEADER_LEN) {
		memcpy(&val, m_pRead, sizeof(int));    
		m_pRead += sizeof(val);
	}
	else {
		m_overflow = true;
	}
	return *this;
}

CPacket& CPacket::operator>>(long& val)
{
	if (m_pRead + sizeof(long) <= m_pBuf->content + m_pBuf->header.length - NET_HEADER_LEN) {
		memcpy(&val, m_pRead, sizeof(long));    
		m_pRead += sizeof(val);
	}
	else {
		m_overflow = true;
	}
	return *this;
}

CPacket& CPacket::operator>>(short& val)
{
	if (m_pRead + sizeof(short) <= m_pBuf->content + m_pBuf->header.length - NET_HEADER_LEN) {
		memcpy(&val, m_pRead, sizeof(short));    
		m_pRead += sizeof(val);
	}
	else {
		m_overflow = true;
	}
	return *this;
}

CPacket& CPacket::operator>>(bool& val)
{
	if (m_pRead + sizeof(bool) <= m_pBuf->content + m_pBuf->header.length - NET_HEADER_LEN) {
		memcpy(&val, m_pRead, sizeof(bool));    
		m_pRead += sizeof(val);
	}
	else {
		m_overflow = true;
	}
	return *this;
}

CPacket& CPacket::operator>>(float& val)
{
	if (m_pRead + sizeof(float) <= m_pBuf->content + m_pBuf->header.length - NET_HEADER_LEN) {
		memcpy(&val, m_pRead, sizeof(float));    
		m_pRead += sizeof(val);
	}
	else {
		m_overflow = true;
	}
	return *this;
}

CPacket& CPacket::operator>>(double& val)
{
	if (m_pRead + sizeof(double) <= m_pBuf->content + m_pBuf->header.length - NET_HEADER_LEN) {
		memcpy(&val, m_pRead, sizeof(double));    
		m_pRead += sizeof(val);
	}
	else {
		m_overflow = true;
	}
	return *this;
}

CPacket& CPacket::operator>>(tstring& val)
{
	val = m_pRead;
	m_pRead += (val.length() + 1);
	if(m_pRead > m_pBuf->content + m_pBuf->header.length - NET_HEADER_LEN)
		m_overflow = true;

	return *this;
}

CPacket& CPacket::operator>>(wstring& val)
{


	int len=0;
	if (m_pRead + sizeof(int) <= m_pBuf->content + m_pBuf->header.length - NET_HEADER_LEN) {
		memcpy(&len, m_pRead, sizeof(int));    
		m_pRead += sizeof(len);
	}
	else {
		m_overflow = true;
	}
	if(m_pRead + len*sizeof(WCHAR) > m_pBuf->content + m_pBuf->header.length - NET_HEADER_LEN) {
		m_overflow = true;

		return *this;
	}
	//memset(( m_pRead + (len)*sizeof(WCHAR) ), 0, 2);
	val = (WCHAR*)m_pRead;
	val= val.substr(0, len);
	m_pRead += len*sizeof(WCHAR);

	return *this;
}

CPacket& CPacket::operator>>(char& val)
{
	if (m_pRead + sizeof(char) <= m_pBuf->content + m_pBuf->header.length - NET_HEADER_LEN) {
		memcpy(&val, m_pRead, sizeof(char));    
		m_pRead += sizeof(val);
	}
	else {
		m_overflow = true;
	}
	return *this;
}

CPacket& CPacket::operator>>(unsigned char& val)
{
	if (m_pRead + sizeof(unsigned char) <= m_pBuf->content + m_pBuf->header.length - NET_HEADER_LEN) {
		memcpy(&val, m_pRead, sizeof(unsigned char));    
		m_pRead += sizeof(val);
	}
	else {
		m_overflow = true;
	}
	return *this;
}

CPacket& CPacket::operator>>(WORD& val)
{
	if (m_pRead + sizeof(WORD) <= m_pBuf->content + m_pBuf->header.length - NET_HEADER_LEN) {
		memcpy(&val, m_pRead, sizeof(WORD));    
		m_pRead += sizeof(val);
	}
	else {
		m_overflow = true;
	}
	return *this;
}

CPacket& CPacket::operator>>(unsigned int& val)
{
	if (m_pRead + sizeof(unsigned int) <= m_pBuf->content + m_pBuf->header.length - NET_HEADER_LEN) {
		memcpy(&val, m_pRead, sizeof(unsigned int));    
		m_pRead += sizeof(val);
	}
	else {
		m_overflow = true;
	}
	return *this;
}

CPacket& CPacket::operator>>(DWORD& val)
{
	if (m_pRead + sizeof(DWORD) <= m_pBuf->content + m_pBuf->header.length - NET_HEADER_LEN) {
		memcpy(&val, m_pRead, sizeof(DWORD));    
		m_pRead += sizeof(val);
	}
	else {
		m_overflow = true;
	}
	return *this;
}

CPacket& CPacket::operator>>(LONGLONG& val)
{
	if (m_pRead + sizeof(LONGLONG) <= m_pBuf->content + m_pBuf->header.length - NET_HEADER_LEN) {
		memcpy(&val, m_pRead, sizeof(LONGLONG));    
		m_pRead += sizeof(val);
	}
	else {
		m_overflow = true;
	}
	return *this;
}

CPacket& CPacket::operator>>(ULONGLONG& val)
{
	if (m_pRead + sizeof(ULONGLONG) <= m_pBuf->content + m_pBuf->header.length - NET_HEADER_LEN) {
		memcpy(&val, m_pRead, sizeof(ULONGLONG));    
		m_pRead += sizeof(val);
	}
	else {
		m_overflow = true;
	}
	return *this;
}

CPacket& CPacket::operator>>(char* val)
{
	WORD nLen = (WORD)(strlen(m_pRead)) + 1;
	if(m_pRead + nLen > m_pBuf->content + m_pBuf->header.length - NET_HEADER_LEN)
	{
		m_overflow = true;
	}
	else
	{
		memcpy((void*)val, m_pRead, nLen);    
		m_pRead += nLen;
	}	

	return *this;
}

/*CPacket& CPacket::operator>>(int& val)
{
	Get(val);
	return *this;
}

CPacket& CPacket::operator>>(long& val)
{
	Get(val);
	return *this;
}

CPacket& CPacket::operator>>(short& val)
{
	Get(val);
	return *this;
}

CPacket& CPacket::operator>>(bool& val)
{
	Get(val);
	return *this;
}

CPacket& CPacket::operator>>(float& val)
{
	Get(val);
	return *this;
}

CPacket& CPacket::operator>>(double& val)
{
	Get(val);
	return *this;
}

CPacket& CPacket::operator>>(tstring& val)
{
	val = m_pRead;
	m_pRead += (val.length() + 1);
	if(m_pRead > m_pBuf->content + m_pBuf->header.length - NET_HEADER_LEN)
		m_overflow = true;

	return *this;
}

CPacket& CPacket::operator>>(wstring& val)
{
	int len;
	this->Get(len);
	if(m_pRead + len*sizeof(WCHAR) > m_pBuf->content + m_pBuf->header.length - NET_HEADER_LEN) {
		m_overflow = true;

		return *this;
	}
	//memset(( m_pRead + (len)*sizeof(WCHAR) ), 0, 2);
	val = (WCHAR*)m_pRead;
	val= val.substr(0, len);
	m_pRead += len*sizeof(WCHAR);

	return *this;
}

CPacket& CPacket::operator>>(char& val)
{
	Get(val);
	return *this;
}

CPacket& CPacket::operator>>(unsigned char& val)
{
	Get(val);
	return *this;
}

CPacket& CPacket::operator>>(WORD& val)
{
	Get(val);
	return *this;
}

CPacket& CPacket::operator>>(unsigned int& val)
{
	Get(val);
	return *this;
}

CPacket& CPacket::operator>>(DWORD& val)
{
	Get(val);
	return *this;
}

CPacket& CPacket::operator>>(LONGLONG& val)
{
	Get(val);
	return *this;
}

CPacket& CPacket::operator>>(ULONGLONG& val)
{
	Get(val);
	return *this;
}

CPacket& CPacket::operator>>(char* val)
{
	WORD nLen = (WORD)(strlen(m_pRead)) + 1;
	if(m_pRead + nLen > m_pBuf->content + m_pBuf->header.length - NET_HEADER_LEN)
	{
		m_overflow = true;
	}
	else
	{
		memcpy((void*)val, m_pRead, nLen);    
		m_pRead += nLen;
	}	

	return *this;
}*/

void CPacket::ReadBuff(unsigned char *val, WORD len)
{
	if(m_pRead + len > m_pBuf->content + m_pBuf->header.length - NET_HEADER_LEN)
	{
		m_overflow = true;
	}
	else
	{
		memcpy((void*)val, m_pRead, len);    
		m_pRead += len;
	}	
}

void CPacket::reset()
{
	memset(m_packet, 0x00, sizeof(m_packet));
	m_overflow = false;
	m_pBuf = (nio_packet*)m_packet;
	m_pRead = m_pBuf->content;
	m_pBuf->header.length = NET_HEADER_LEN;
}

