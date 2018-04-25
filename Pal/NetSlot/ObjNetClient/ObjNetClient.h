#ifndef __OBJNETCLIENT_H__
#define __OBJNETCLIENT_H__

#pragma warning(disable:4786)

class C_ObjNetClient : public INetEventListener
{
private:
    //-------------------------------------
    IObjNet *m_pObjNet;
    //-------------------------------------
    int         m_iSvrSockID;
    int         m_iPktSize;
    int         m_iPackSize;
    sockaddr_in m_SockAddr;
	int			m_iConnectCount;
	bool		m_bReConnect;
	bool		m_bShowIP;
    //-------------------------------------
    std::vector<S_EventFunctionInfo *> m_EventList_OnPacket;
    std::vector<S_EventFunctionInfo *> m_EventList_OnConnectFailed;
    std::vector<S_EventFunctionInfo *> m_EventList_OnConnected;
    std::vector<S_EventFunctionInfo *> m_EventList_OnDisconnected;
    //-------------------------------------

private:
    //-------------------------------------
    void Cls_EventList_OnPacket();          
    void Cls_EventList_OnConnectFailed();   
    void Cls_EventList_OnConnected();       
    void Cls_EventList_OnDisconnected();    
    //-------------------------------------

public:
    //-------------------------------------
     C_ObjNetClient();
    ~C_ObjNetClient();
    //-------------------------------------
    bool WaitConnect(char *szIP, int iPort, int iPktSize, int iPackSize); 
    bool WaitConnect(); 
    void Release(); 
	void Disconnect(); 
    //-------------------------------------
    void SetBufferSize(int iSockID, int iSendBufSize, int iRecvBufSize); 
    void SetTCPNoDelay(int iSockID, bool bNoDelay);                      
    //-------------------------------------
    void Send(int iSize, S_ObjNetPktBase *pPkt);    
    //-------------------------------------
    void Flush(long lDelayTime = 0);    
    //-------------------------------------
    void GetServerAddress(sockaddr_in *pAddr);  
    int  GetNumOfBytesSend();                   
    int  GetNumOfBytesRecv();                   
    int  GetPktLeftInSendQueue();               
    bool CheckIsPktEmpty();                     
    int  GetSvrSockID() { return m_iSvrSockID;};
	bool IsConnect() { return (m_iSvrSockID >= 0);};
	void SetConnectCount(int iCount);			
	void SetReConnect(bool bEnable);			
	void SetShowIP(bool bShow);					
    //-------------------------------------
    bool RegEvent_OnPacket       (UINT uiCmdID, Event_OnPacket pFunc, long lParam); 
    void RegEvent_OnConnectFailed(Event_OnConnectFailed pFunc, long lParam);        
    void RegEvent_OnConnected    (Event_OnConnected pFunc, long lParam);            
    void RegEvent_OnDisconnected (Event_OnDisconnected pFunc, long lParam);         
    //-------------------------------------

public:
    //-------------------------------------
    // Ä~©Ó¦Û INetEventListener
    //-------------------------------------
    bool LSAPI OnPacket       (LNETID SockID, HBUFFER hIncomingData);
    void LSAPI OnConnectFailed(int iConnectID);
    void LSAPI OnConnected    (LNETID SockID);
    void LSAPI OnPeerShutdown (LNETID SockID);
    void LSAPI OnDisconnected (LNETID SockID);
    //-------------------------------------
};
//-----------------------------------------------------------------------------
#endif                                                          // __OBJNETCLIENT_H__
//-----------------------------------------------------------------------------