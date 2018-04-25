#pragma once

//
// 服务器后台管理相关定义
//

#pragma pack(push, 1)

namespace BACKSTAGEMGR
{
	const int MSG_BASE_BSTRS				= 25000; 
	const int MSG_BASE_RSTBS				= 26000;

	const int MAX_ID_LENGTH					= 20;
	const int MAX_PASSWORD_LENGTH			= 21;
	const int MAX_CHARACTER_NAME_LENGTH		= 13;

	struct TCP_MSG_BASE
	{
		unsigned short header;

		TCP_MSG_BASE(unsigned short id)
			:header(id)
		{
		}
	};

}; // 

#pragma pack(pop)