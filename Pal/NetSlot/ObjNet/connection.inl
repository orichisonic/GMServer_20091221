//====================================================================================================

inline SOCKET Connection::GetSocket(void)
{ return m_sock; }

//====================================================================================================
inline void Connection::ShutdownSendPipe(void) 
{
	m_WrPacketList.Write( INVALID_BUFFERHANDLE );
}
inline void Connection::SendPkt( int nSize, void* vpData )
{
	if (PackBuffer.PackSize() >0)
		PackBuffer.Add(nSize,vpData);
	else
		SendPkt_Old(nSize,vpData);
}

inline void
Connection::SendPkt_Old( int nSize, void* vpData ) 
{
	HBUFFER hBuffer = g_BufferMan.AddBuffer( nSize, vpData );
	SendPkt_Old( hBuffer );
}

inline void
Connection::SendPkt_Old( HBUFFER hBuffer ) 
{
	m_WrPacketList.Write( hBuffer );
}

#define GET_ADDRESS( func )\
{\
	int size = sizeof(*addr);\
	if( IsSocketError(func(m_sock, addr, &size) ) )\
		throw LSockException(0);\
}\

inline void Connection::GetPeerAddress(sockaddr* addr) //throw SockException
{
	GET_ADDRESS(getpeername);
}
inline void Connection::GetSockAddress(sockaddr* addr) //throw SockException
{
	GET_ADDRESS(getsockname);
}
//====================================================================================================


inline bool Connection::HaveSomethingToWrite(void)
{
	return (!m_WrPacketList.IsEmpty() || m_hWrBuffer != INVALID_BUFFERHANDLE || !PackBuffer.IsEmpty());
}

