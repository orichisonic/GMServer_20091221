#if !defined(_L_TEST_VALID_H_)
#define _L_TEST_VALID_H_

// inherit 時方便使用
#define TEST_VALID public TestValid

#ifdef _DEBUG
	#include "LDebug.h"
#endif

class TestValid  
{
#ifdef _DEBUG
	DWORD dwPointerCheck1;
	DWORD dwPointerCheck2;
public:
	virtual bool IsValid(void)
	{
		return ( (dwPointerCheck1 == 0x786403e4) && (dwPointerCheck2 == 0xa3cd4ffc));
	}
	TestValid() : dwPointerCheck1(0x786403e4),dwPointerCheck2(0xa3cd4ffc){}
	~TestValid() { dwPointerCheck1 = 0xcdcdcdcd; dwPointerCheck2 = 0xcdcdcdcd;}
#endif

};

#ifndef _DEBUG

	#define ASSERT_VALID( p ) NULL

#else

	#define ASSERT_VALID( p ) \
	{\
	picaASSERT( (p) != NULL );\
	picaASSERT( (p)->IsValid() );\
	}

#endif

#endif // !_L_TEST_VALID_H_
