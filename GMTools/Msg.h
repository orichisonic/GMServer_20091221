// Msg.h: interface for the CMsg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSG_H__15A29606_3C37_4B10_872B_AB3D13E2E395__INCLUDED_)
#define AFX_MSG_H__15A29606_3C37_4B10_872B_AB3D13E2E395__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


const int NETWORK_BUF_SIZE		=	327680;
const int NETWORK_MSG_SIZE		=	4096;
const int NETWORK_MSG_HEADER	=   4;

__declspec(dllexport) int nayaStrlen( LPCTSTR p, int nMaxLen );
__declspec(dllexport) int nayaStrcmp( LPCTSTR p1, LPCTSTR p2, int nMaxLen );

class __declspec(dllexport) CMsg  
{
public:
	CMsg();
	virtual ~CMsg();

protected:
	LPWORD m_pSize;
	LPWORD m_pID;

	LPBYTE m_pRead;
	LPBYTE m_pWrite;

	BYTE m_pBuf[NETWORK_BUF_SIZE];

public:
	void Clear()
	{
		m_pSize = (LPWORD)&m_pBuf[0];
		m_pID	= (LPWORD)&m_pBuf[2];
		m_pRead = m_pWrite = &m_pBuf[NETWORK_MSG_HEADER]; // m_pBuf로 m_pRead 와 m_pWrite를 초기화
	}

	CMsg & ID (WORD wID)
	{
		* m_pID		= wID;
		* m_pSize	= NETWORK_MSG_HEADER;                  
		m_pRead		= m_pWrite = &m_pBuf[NETWORK_MSG_HEADER];
		return *this;
	}

	int GetRemainingRData()
	{
		if ( m_pRead < m_pBuf + GetSize() )
			return ( m_pBuf + GetSize() - m_pRead );

		return 0;
	}

	WORD	ID() {return *m_pID;};
	WORD	GetSize() const  {return *m_pSize;};
	LPBYTE	GetBuf()  {return m_pBuf;};

	void	ReadData( void * pData, int n );// throw (OutOfBoundException *);
	void	WriteData( void * pData, int n );//throw (OutOfBoundException *);
	void *	WriteInData( void *pData, int n );//throw (OutOfBoundException *);
	
	CMsg& operator << (bool arg)	{WriteData(&arg, sizeof(bool));		return *this;};
	CMsg& operator << (char arg)	{WriteData(&arg, sizeof(char));		return *this;};
	CMsg& operator << (BYTE arg)	{WriteData(&arg, sizeof(BYTE));		return *this;};
	CMsg& operator << (short arg)	{WriteData(&arg, sizeof(short));	return *this;};
	CMsg& operator << (WORD arg)	{WriteData(&arg, sizeof(WORD));		return *this;};
	CMsg& operator << (int arg)		{WriteData(&arg, sizeof(int));		return *this;};
	CMsg& operator << (DWORD arg)	{WriteData(&arg, sizeof(DWORD));	return *this;};
	CMsg& operator << (float arg)	{WriteData(&arg, sizeof(float));	return *this;};
	CMsg& operator << (double arg)	{WriteData(&arg, sizeof(double));	return *this;};
	

	CMsg& operator >> (bool& arg)	{ReadData(&arg, sizeof(bool));		return *this;};
	CMsg& operator >> (char& arg)	{ReadData(&arg, sizeof(char));		return *this;};
	CMsg& operator >> (BYTE& arg)	{ReadData(&arg, sizeof(BYTE));		return *this;};
	CMsg& operator >> (short& arg)	{ReadData(&arg, sizeof(short));		return *this;};
	CMsg& operator >> (WORD& arg)	{ReadData(&arg, sizeof(WORD));		return *this;};
	CMsg& operator >> (int& arg)	{ReadData(&arg, sizeof(int));		return *this;};
	CMsg& operator >> (DWORD& arg)	{ReadData(&arg, sizeof(DWORD));		return *this;};
	CMsg& operator >> (float& arg)	{ReadData(&arg, sizeof(float));		return *this;};
	CMsg& operator >> (double& arg)	{ReadData(&arg, sizeof(double));	return *this;};
	

	template <typename T> CMsg& operator << ( T  arg) { WriteData(&arg, sizeof(T)); return *this;}
	template <typename T> CMsg& operator >> ( T &arg) { ReadData(&arg, sizeof(T)); return *this;}
	void WriteStructData(void* pData, int nByte)
	{
		/*if( nByte >= NETWORK_BUF_SIZE - NETWORK_MSG_HEADER )
			throw new OutOfBoundException("CMsg::WriteData > NETWORK_BUF_SIZE");*/
		
		memcpy(&m_pBuf[NETWORK_MSG_HEADER], pData, nByte);
		*m_pSize = NETWORK_MSG_HEADER + nByte;
	}
	int GetStructData(void* pData, int nByte)
	{
		int copylen = nByte > *m_pSize - NETWORK_MSG_HEADER ?
			*m_pSize - NETWORK_MSG_HEADER : nByte;
		memcpy(pData, &m_pBuf[NETWORK_MSG_HEADER], copylen);
		return copylen;
	}

	template <> CMsg & operator << ( LPCTSTR lpsz ) { WriteData((LPVOID)lpsz, strlen(lpsz) + 1); return * this;};
	template <> CMsg & operator << ( CString  str ) { WriteData((LPVOID)(LPCTSTR)str, str.GetLength() + 1); return * this;};

	template <> CMsg & operator >> ( CString &str );

};

#endif // !defined(AFX_MSG_H__15A29606_3C37_4B10_872B_AB3D13E2E395__INCLUDED_)
