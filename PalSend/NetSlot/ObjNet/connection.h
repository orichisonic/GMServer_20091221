#pragma once

#include "ThreadFreeFIFO.h"

#include "LSocket.h"	//<1>¤@²Õ
#include "LMutex.h"		//<2>

#include "LBuffer.h"
#include "LNet.h"
#include "LPackBuffer.h"


#define CONNECTION_MAX_READ_PACKET_SIZE 65535
#define CONNECTION_MAX_WRITE_PACKET_SIZE 65535


#define _Conn_CheckType_CheckConnect_		0

#define _Conn_CheckType_NeedCheckConnect_	1

struct CheckPacket
{
	unsigned short Command;
	unsigned short Type;
	CheckPacket()
	{
		Command = 0xffff;
	}
};


class Connection
{
public:
	enum CN_STATUS{
		NOT_CREATED,
		CONNECTING,
		ESTABLISHED,
		PEER_SHUTDOWN_SEND,
		SHUTDOWN_SEND_PIPE,
		SHUTDOWNED
	};
protected:
	SOCKET			m_sock;
	LMutex			m_WrMutex;
	ThreadFreeFIFO<HBUFFER>	m_WrPacketList;
	WORD			m_u16WrPacketIndex;		
	HBUFFER			m_hWrBuffer;
	HBUFFER			m_hRdBuffer;		
	WORD			m_u16ReadSize;		
	WORD			m_u16ReadIndex;		
	UINT32			m_u32TotalRecvSize;	
	UINT32			m_u32Context;
	CN_STATUS		m_status;
	INetEventListener* m_pListener;
	LNETID			m_id;
	unsigned short	_Key;	
	bool			_ClientFlag;	
	bool			_NeedCheckConnect;	
	int				_NextCheckConnect;	
	int				_CheckConnectTimes;	
	static	unsigned  char	_KeyTable[16];	
	bool			_DataEncodeFlag;	
	cPackBufferMan	PackBuffer;
public:
	//----------------------------------------------------------------
	void Key(unsigned short V){_Key = V;}
	void NeedCheckConnect(){_NeedCheckConnect = true;_NextCheckConnect= clock()+5000;}
	void SetClient(){_ClientFlag = true;}
	

	bool IS_Client(){return _ClientFlag;};

	void SendCheckPack();
	//----------------------------------------------------------------
	void Create(SOCKET sock, LNETID id);
	void CleanUp(void);
	Connection();
	~Connection();
	void AttachListener( INetEventListener* pListener ) {m_pListener = pListener;}
	void SendPkt( int nSize, void* vpData );
	void SendPkt_Old( int nSize, void* vpData );
	void SendPkt_Old( HBUFFER );
	inline SOCKET GetSocket(void);
	inline void GetPeerAddress(sockaddr* addr);
	inline void GetSockAddress(sockaddr* addr);
	UINT32	GetContext(void) throw() { return m_u32Context; }
	void	SetContext(UINT32 c) throw() { 
		
		m_u32Context = c; 
	}

	bool ProcessDataOut_Old(void);	
	bool NeedRead(void) { return !(m_status == SHUTDOWNED || m_status == PEER_SHUTDOWN_SEND || m_status == NOT_CREATED ); }
	bool HaveSomethingToWrite(void);
	void ShutdownSendPipe(void);
	bool ProcessDataIn_Old(void);		
	bool Encode(unsigned short Size,char*);	
	bool Decode(unsigned short Size,char*);	
	friend class cSockMan;	
	int  ProcessPackData();		
	bool ProcessDataOut_Pack(void);
	void OnPacket_New ( LNETID id, HBUFFER hIncomingData);	
	int  ProcessUnPackData(int NetID, int nSize, Pack_Head_Struct* Head );		
	bool ProcessDataIn_Pack(void);
	void SetPackSize(int size) { PackBuffer.SetPackSize(size);}
	
	int	 PackBufferCount() {return 	(PackBuffer.GetPackCount()+PackBuffer.GetPrePackCount());}
};
#include "Connection.inl"
