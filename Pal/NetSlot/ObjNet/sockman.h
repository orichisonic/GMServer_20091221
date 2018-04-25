#ifndef _SOCKET_MANAGER_H
#define _SOCKET_MANAGER_H
#define _DEF_PACKSIZE_ 0

class Connection;

class cSockMan: public IObjNet
{
public:
	cSockMan(){_PackSize = _DEF_PACKSIZE_;} 
	~cSockMan() throw();

	void	LSAPI Init(bool bIsCompress);
	void	LSAPI UnInit(void);
	void	LSAPI GetPeerAddress( LNETID smid, sockaddr_in* pAddr );

	void	LSAPI EnumLNETID( SM_EnumLNETIDCallBack, void* pContext );
	void	LSAPI EnumConnections(LNET_EnumPlayersCallBack callback, void* vpContext) throw();
	int		LSAPI GetNumOfPlayer(void);

	//void	Connect( char*  ipaddr, UINT uPort, INetEventListener* listener );
	//void	Connect( u_long  ipaddr, u_short uPort, INetEventListener* listener );
	SOCKET	LSAPI Host(sockaddr_in* TCPAddr, INewConnectionListener* pListener);
	void	LSAPI RemoveHost( SOCKET sock );
	void	LSAPI AttachListener( SOCKET server_socket, INewConnectionListener* listener);

	bool	LSAPI WaitingConnect( sockaddr_in* pAddr, INetEventListener* pListener );
	int		LSAPI Connect( sockaddr_in* pAddr, INetEventListener* pListener );
	void	LSAPI CancelConnect( int connect_number );
	void	LSAPI AttachClientListener( LNETID id, INetEventListener* pListener );
	LNETID	LSAPI AddSocket( SOCKET sock, INetEventListener* pListener );

	void	LSAPI Flush(timeval* wait_time);
	void	LSAPI RevFlush(timeval* wait_time);
	void	LSAPI SendFlush(timeval* wait_time);

	
	void	LSAPI Send(LNETID smid, int nSize, void* vpData, int nPriority );
	void	LSAPI SendToAll( int nSize, void* vpData );
	void	LSAPI SendToMutiple( int nNum, const LNET_PKT_DESTINATION* dest_array, int nSize, void* vpData);

	void	LSAPI Close( LNETID smid );
	void	LSAPI ShutdownSendPipe( LNETID smid );

	UINT32	LSAPI GetContext(LNETID smid);
	void	LSAPI SetContext(LNETID smid, UINT32 c);

	LNETID	LSAPI AddConnection( Connection* pConn );

	UINT32	LSAPI GetNumOfBytesSend(void) throw() {return m_nNumOfBytesSend;};
	UINT32	LSAPI GetNumOfBytesRecv(void) throw() {return m_nNumOfBytesRecv;};
	bool	LSAPI IsValidLNETID( LNETID u32ID ) throw() { return m_Connections.IsValidAt( u32ID ); }

	UINT32	LSAPI GetPacketLeftInSendQueue( LNETID u32id );

	void	LSAPI GetCompressSuccessVSFailure( int& nSuccess, int& nFailed );
	int		LSAPI GetNumOf_NotCompressedBytes( void );
	int		LSAPI GetNumOf_CompressedBytes( void );

	UINT32	LSAPI GetNumOfConnections( void );
	void	LSAPI SetMaxPacketSize( UINT32 u32Size );
	bool	LSAPI CheckPacketEmpty();


	
	void	LSAPI NeedKey(){_NeedKey = true;};

	
	void	LSAPI NeedCheckConnect(){_NeedCheckConnect = true;};
	
	bool	LSAPI SetNetBuffer(int SockID , int SendSize, int RecvSize);
	
	SOCKET	LSAPI GetSocket(int SockID);
	
	void	LSAPI TCP_NODelay(int SockID ,bool Enable);
	
	void	LSAPI PackSize(int size){ _PackSize = size;	};
	
	int		LSAPI PackSize(){ return _PackSize;	};
	
	unsigned int LSAPI MemUseSize() { return g_BufferMan.GetMemUseSize();};
private: 	
	void	LSAPI ProcessNewConnect(SOCKET sock,INewConnectionListener* pListener);
private:
	UINT32Array		m_ServerSocketList_sock;		
	UINT32Array		m_ServerSocketList_pListener;	
	UINT32Array		m_Connections;					
	UINT32Array		m_WCList_sock;					
	UINT32Array		m_WCList_pListener;				
	UINT32	m_nNumOfBytesRecv;
	UINT32	m_nNumOfBytesSend;
	INT32	m_u32MaxPacketSize;
	bool	m_bHaveShutDownConnection;	
	bool	_NeedKey;
	bool	_NeedCheckConnect;	
	int		_PackSize;		//«Ê¥]¤j¤p
};
#endif //_SOCKET_MANAGER_H
