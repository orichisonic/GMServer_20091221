
#pragma once

#include "LU32Array.h"

typedef unsigned int HBUFFER;
#define INVALID_BUFFERHANDLE INVALID_UINT32

class cBufferMan {
	UINT32Array	m_arrayReference;
	UINT32Array m_arrayBufferPointer;
	unsigned int m_MemUseSize;	
public:
	cBufferMan(void) throw();
	~cBufferMan() throw();

	// for read
	HBUFFER CreateBuffer( int nSize ) throw();
	void GetBuffer( HBUFFER hBuffer, int& nSize, void*& vpData ) throw();
	
	// for write
	HBUFFER AddBuffer( int nSize, void* vpData ) throw();
	void GetPureBuffer( HBUFFER hBuffer, int& nSize, void*& vpData ) throw();

	void AddRef ( HBUFFER ) throw();
	void Release( HBUFFER ) throw();
	void AddRef ( HBUFFER, int nAddRef ) throw();
	void Release( HBUFFER, int nRelRef ) throw();
	
	unsigned int GetMemUseSize() {return m_MemUseSize;};	
};

extern cBufferMan g_BufferMan;
