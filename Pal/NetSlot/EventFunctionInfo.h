#ifndef __EventFunctionInfo_H__
#define __EventFunctionInfo_H__
//-----------------------------------------------------------------------------
typedef void (*Event_OnNewConnection)(int iSockID, long lParam);
typedef void (*Event_OnDisconnected) (int iSockID, long lParam);
typedef void (*Event_OnConnectFailed)(int iConnectID, long lParam);
typedef void (*Event_OnConnected)    (int iSockID, long lParam);
typedef void (*Event_OnPacket)       (int iSockID, int iSize, S_ObjNetPktBase *pPkt, long lParam);
//-----------------------------------------------------------------------------
struct S_EventFunctionInfo
{
    // �ƥ�禡
    union
    {
        Event_OnNewConnection EventFunc_OnNewConnection;		
        Event_OnDisconnected  EventFunc_OnDisconnected;
		Event_OnConnected     EventFunc_OnConnected;
		Event_OnConnectFailed EventFunc_OnConnectFailed;
        Event_OnPacket        EventFunc_OnPacket;        
    };

    // �ϥΪ̦ۭq�Ѽ�
    long lParam;
};
//-----------------------------------------------------------------------------
#endif                                                          // __EventFunctionInfo_H__
//-----------------------------------------------------------------------------