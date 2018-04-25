// Msg.cpp: implementation of the CMsg class.
//
//////////////////////////////////////////////////////////////////////
#include "stdAfx.h"
#include "Msg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsg::CMsg()
{
	m_pBuf = new BYTE[NETWORK_BUF_SIZE];
	ZeroMemory(m_pBuf, NETWORK_BUF_SIZE);
	Clear();
}

CMsg::~CMsg()
{
	delete []m_pBuf;
	m_pBuf = NULL;
}

void CMsg::ReadData(void * pData, int n)// throw (OutOfBoundException *)
{
	/*if( m_pRead + n > m_pBuf + NETWORK_BUF_SIZE)
		throw new OutOfBoundException("CMsg::ReadData > NETWORK_BUF_SIZE");
	if( m_pRead + n > m_pBuf + GetSize())
		throw new OutOfBoundException("CMsg::ReadData > GetSize()");*/

	memcpy(pData, m_pRead, n);
	m_pRead += n;
}


void CMsg::WriteData(void * pData, int n)// throw (OutOfBoundException *)
{
/*	if( m_pWrite + n >= m_pBuf + NETWORK_BUF_SIZE )
		throw new OutOfBoundException("CMsg::WriteData > NETWORK_BUF_SIZE");*/
	
	memcpy(m_pWrite, pData, n);
	m_pWrite += n;
	*m_pSize += n;
}

void * CMsg::WriteInData(void * pData, int n)// throw (OutOfBoundException *)
{
	/*if( m_pWrite + n >= m_pBuf + NETWORK_BUF_SIZE )
		throw new OutOfBoundException("CMsg::WriteInData > NETWORK_BUF_SIZE");*/
	
	memcpy(m_pWrite, pData, n);
	void * pTemp = m_pWrite;
	m_pWrite += n;
	*m_pSize += n;
	return pTemp;
}

template <> CMsg & CMsg::operator >> ( CString &str )
{
		int nLen;// = strlen((LPCTSTR)m_pRead) + 1;
		nLen = nayaStrlen( (LPCTSTR)m_pRead, ( m_pBuf + GetSize() - m_pRead ) );
		if ( nLen != -1 && nLen != -2 )
		{
			nLen++;
			ReadData( str.GetBuffer(nLen), nLen );
			str.ReleaseBuffer(); // GetBuffer로 할당된 메모리중 쓰지 않는 나머지.~ 없엔다.
		}
	/*	else if ( nLen == -1 )
		{
			str = "NULL";
			throw new OutOfBoundException("CMsg>>(CString) -1");
		}
		else // if ( nLen == -2 )
		{
			str = "NULL";
			throw new OutOfBoundException("CMsg>>(CString) -2");
		}*/
		return * this;
}
__declspec(dllexport) int nayaStrlen( LPCTSTR p, int nMaxLen )
{
	if ( IsBadReadPtr( p, nMaxLen ) )
		return -1;

	for ( int i = 0 ; i < nMaxLen ; i++ )
		if ( p[i] == '\0' )
			break;

	if ( i == nMaxLen )
		return -2;

	return i;
}

__declspec(dllexport) int nayaStrcmp( LPCTSTR p1, LPCTSTR p2, int nMaxLen )
{
	int nLen1, nLen2;
	nLen1 = nayaStrlen( p1, nMaxLen );
	nLen2 = nayaStrlen( p2, nMaxLen );

	if ( nLen1 > 0 && nLen2 > 0 && nLen1 == nLen2 )
	{
		for ( int i = 0 ; i < nLen1 ; i++ )
		{
			if ( p1[i] != p2[i] )
				return 1;
		}
		return 0;
	}

	return -1;
}


