#pragma once
#ifdef _USING_RD_IOBJNET_
	#define _DEF_RD_IOBJNET_BEGIN_ namespace RDIOBJNET {
	#define _DEF_RD_IOBJNET_END_ };
	#define _NS_RDIOBJNET RDIOBJNET
#else
	#define _DEF_RD_IOBJNET_BEGIN_	
	#define _DEF_RD_IOBJNET_END_		
	#define _NS_RDIOBJNET		
#endif
