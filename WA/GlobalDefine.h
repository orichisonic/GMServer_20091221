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
		//��ӹ���
			GM_BROADCAST_ADD = 56000,
			GM_BROADCAST_ADD_ACK,
			//��ѯ���й���
			GM_BROADCAST_SELECT,
			GM_BROADCAST_SELECT_ACK,
			//ɾ������
			GM_BROADCAST_REMOVE,
			GM_BROADCAST_REMOVE_ACK,
			
			//ǿ���������
			GM_KICK_USER,
			GM_KICK_USER_ACK,
			//��ɫ����С����
			GM_MOVE_USER,
			GM_MOVE_USER_ACK,
			//�������
			GM_FAMILY_DISBAND,
			GM_FAMILY_DISBAND_ACK,
			//�˺ŷ�ͣ�����
			GM_USER_STOP,
			GM_USER_STOP_ACK,
			//��ӵ��ߡ�����
			GM_ADD_ITEM,//ServerManager --> Server
			GM_ITEM_ADD,
			GM_ITEM_ADD_ACK,
			//��ѯ���ߡ�����
			GM_ITEM_SELECT,
			GM_ITEM_SELECT_ACK,
			//������ҵ�����״̬
			GM_USER_RESET,
			GM_USER_RESET_ACK,
			//�������ͳ��
			GM_ONLINECOUNT_REQ,
			GM_ONLINECOUNT_ACK,
			//��ֹ������ϴ�ͼƬ
			GM_PICTURE_STOP,
			GM_PICTURE_STOP_ACK,
			//����
			GM_TRANSMIT_REQ,
			GM_TRANSMIT_ACK,
			//Υ��ͼƬ����
			GM_CONTENT_CLEAR,
			GM_CONTENT_CLEAR_ACK,

			//����Զ�̷����
		//	CONNECT_SERVERMANAGER_LOGIN = 10000,
		//	CONNECT_SERVERMANAGER_LOGIN_ACK = 1000
			CONNECT_GATEWAY_LOGIN = 10004,
			CONNECT_GATEWAY_LOGIN_ACK = 10005,
	};
};

#endif 

