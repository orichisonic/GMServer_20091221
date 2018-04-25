#ifndef _OBJ_NET_H_
#define _OBJ_NET_H_
#ifndef _LIB
  #ifdef OBJNET_EXPORTS
    #define OBJNET_API __declspec(dllexport)
  #else
    #define OBJNET_API __declspec(dllimport)
  #endif
#else
  #define OBJNET_API
#endif
//-----------------------------------------------------------------------------
//#include <winsock.h>	//##
#include "LNet.h"
//-----------------------------------------------------------------------------
#pragma pack( push, _obj_net_h_)
#pragma pack(8)
//-----------------------------------------------------------------------------
class IObjNet
{
public:
    virtual ~IObjNet ()throw(){};    
    virtual SOCKET  LSAPI Host(sockaddr_in* TCPAddr, INewConnectionListener* pListener) = 0;    
    virtual void    LSAPI RemoveHost( SOCKET host_sock ) = 0;    
    virtual bool    LSAPI WaitingConnect( sockaddr_in* pAddr, INetEventListener* pListener ) = 0;    
    virtual int     LSAPI Connect( sockaddr_in* pAddr, INetEventListener* pListener ) = 0;    
    virtual void    LSAPI CancelConnect( int connect_number ) = 0;    
    virtual void    LSAPI AttachClientListener( LNETID id, INetEventListener* pListener ) = 0;    
    virtual void    LSAPI Flush(timeval* wait_time) = 0;    
    virtual void    LSAPI SendFlush(timeval* wait_time) = 0;    
    virtual void    LSAPI RevFlush(timeval* wait_time) = 0;    
    virtual void    LSAPI Send(LNETID smid, int nSize, void* vpData, int nPriority ) = 0;    
    virtual void    LSAPI SendToAll( int nSize, void* vpData ) = 0;    
    virtual void    LSAPI SendToMutiple( int nNum, const LNET_PKT_DESTINATION* dest_array, int nSize, void* vpData) = 0;
    virtual void    LSAPI Close( LNETID smid ) = 0;
    virtual void    LSAPI ShutdownSendPipe( LNETID smid ) = 0;
    virtual UINT32  LSAPI GetContext(LNETID smid) = 0;
    virtual void    LSAPI SetContext(LNETID smid, UINT32 c) = 0;
    virtual UINT32  LSAPI GetNumOfBytesSend(void) = 0;
    virtual UINT32  LSAPI GetNumOfBytesRecv(void) = 0;
    virtual bool    LSAPI IsValidLNETID( LNETID u32ID ) = 0;
    virtual int     LSAPI GetNumOfPlayer(void) = 0;
    virtual void    LSAPI GetPeerAddress( LNETID u32id, sockaddr_in* pAddr ) = 0;   
    virtual void    LSAPI EnumLNETID( SM_EnumLNETIDCallBack, void* pContext ) = 0;
    virtual void    LSAPI EnumConnections(LNET_EnumPlayersCallBack callback, void* vpContext) = 0;    
    virtual UINT32  LSAPI GetPacketLeftInSendQueue( LNETID u32id ) = 0;    
    virtual void    LSAPI GetCompressSuccessVSFailure( int& nSuccess, int& nFailed ) = 0;    
    virtual UINT32  LSAPI GetNumOfConnections( void ) = 0;    
    virtual void    LSAPI SetMaxPacketSize( UINT32 u32Size ) = 0;    
    virtual bool    LSAPI CheckPacketEmpty() = 0;    
    virtual void    LSAPI NeedKey() = 0;    
    virtual void    LSAPI NeedCheckConnect() = 0;        
    virtual bool    LSAPI SetNetBuffer(int SockID , int SendSize, int RecvSize) = 0;    
    virtual SOCKET  LSAPI GetSocket(int SockID) = 0;    
    virtual void    LSAPI TCP_NODelay(int SockID ,bool Enable)  = 0;    
    virtual void    LSAPI PackSize(int size)  = 0;    
    virtual int     LSAPI PackSize()  = 0;    
    virtual unsigned int LSAPI MemUseSize()  = 0;
};
//-----------------------------------------------------------------------------
extern "C" 
{
    //OBJNET_API IObjNet *LSAPI CreateObjNet( bool bIsCompress ) throw();
    OBJNET_API IObjNet *LSAPI CreateObjNet( void ) throw();
    OBJNET_API void     LSAPI DestroyObjNet( IObjNet* ) throw();
    OBJNET_API void     LSAPI ObjNetBufferAddRef( HBUFFER hBuffer );
    OBJNET_API void     LSAPI ObjNetBufferRelease( HBUFFER hBuffer );
    OBJNET_API void     LSAPI ObjNetGetBuffer( HBUFFER hBuffer, int& nSize, void*& vpData );

    // Check Socket
    bool IsValidSocket(SOCKET s);
}
//-----------------------------------------------------------------------------
#pragma pack( pop, _obj_net_h_ )
//-----------------------------------------------------------------------------
#endif //_OBJ_NET_H_
//-----------------------------------------------------------------------------