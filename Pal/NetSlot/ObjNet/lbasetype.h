#ifndef _BASE_TYPE_H_
#define _BASE_TYPE_H_

#include <basetsd.h>

typedef unsigned long       DWORD;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;

typedef unsigned int		UINT;
//typedef unsigned int        UINT32
typedef unsigned short	    UINT16;
typedef unsigned char	    UINT8;


#define INVALID_UINT32 (unsigned int)(0xffffffff)

#define LSAPI __stdcall

#ifdef _WIN32
// for access()
#define F_OK	0
#define W_OK	02
#define R_OK	04
#endif


#endif // _BASE_TYPE_H_
