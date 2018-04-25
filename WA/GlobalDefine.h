#ifndef GLOBALDEFINE_H
#define GLOBALDEFINE_H

#include <Windows.h>
#include <stdio.h>
#include <string>

using namespace std;

typedef basic_string< TCHAR,
char_traits<TCHAR>, 
allocator<TCHAR> >
tstring;
#define _L(x)      L ## x

struct ServiceKey
{
	enum GMProtocol{
		//添加公告
			GM_BROADCAST_ADD = 56000,
			GM_BROADCAST_ADD_ACK,
			//查询所有公告
			GM_BROADCAST_SELECT,
			GM_BROADCAST_SELECT_ACK,
			//删除公告
			GM_BROADCAST_REMOVE,
			GM_BROADCAST_REMOVE_ACK,
			
			//强制玩家下线
			GM_KICK_USER,
			GM_KICK_USER_ACK,
			//角色移至小黑屋
			GM_MOVE_USER,
			GM_MOVE_USER_ACK,
			//家族管理
			GM_FAMILY_DISBAND,
			GM_FAMILY_DISBAND_ACK,
			//账号封停、解封
			GM_USER_STOP,
			GM_USER_STOP_ACK,
			//添加道具、宠物
			GM_ADD_ITEM,//ServerManager --> Server
			GM_ITEM_ADD,
			GM_ITEM_ADD_ACK,
			//查询道具、宠物
			GM_ITEM_SELECT,
			GM_ITEM_SELECT_ACK,
			//重设玩家到出生状态
			GM_USER_RESET,
			GM_USER_RESET_ACK,
			//在线玩家统计
			GM_ONLINECOUNT_REQ,
			GM_ONLINECOUNT_ACK,
			//禁止、解禁上传图片
			GM_PICTURE_STOP,
			GM_PICTURE_STOP_ACK,
			//传送
			GM_TRANSMIT_REQ,
			GM_TRANSMIT_ACK,
			//违禁图片清理
			GM_CONTENT_CLEAR,
			GM_CONTENT_CLEAR_ACK,

			//连接远程服务端
		//	CONNECT_SERVERMANAGER_LOGIN = 10000,
		//	CONNECT_SERVERMANAGER_LOGIN_ACK = 1000
			CONNECT_GATEWAY_LOGIN = 10004,
			CONNECT_GATEWAY_LOGIN_ACK = 10005,
	};
};

#endif 

