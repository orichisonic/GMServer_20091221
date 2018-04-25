//////////////////////////////////////////////////////////////////////////////////////
// Singleton.h: interface for the Singleton class.
//////////////////////////////////////////////////////////////////////////////////////
//    ex) sample
//
//    class Test : public Singleton < Test >
//    {
//    public:
//	    Test* Sample( int num );
//	    //...
//    };
//
//
//
//    #define g_Test	Test::GetSingleton()
//
//    void SomeFuncton( void )
//    {
//	    Test * pTemp = Test::GetSingleton().Sample( 7 );
//	    // or   Test * pTemp = g_Test.Sample( 7 );
//    }
//
//////////////////////////////////////////////////////////////////////////////////////
#ifndef __SINGLETON_H__
#define __SINGLETON_H__

///////////////////////////////////////////////////////////////////////////////
// include define statement
///////////////////////////////////////////////////////////////////////////////

#include "assert.h"

///////////////////////////////////////////////////////////////////////////////
// class define statement
///////////////////////////////////////////////////////////////////////////////
template <typename T>  
class Singleton
{
    static T* ms_Singleton;

public:
    Singleton(void)
    {
        assert(!ms_Singleton);
        int offset = (int)(T*)1 - (int)(Singleton <T>*)(T*)1;
        ms_Singleton = (T*)((int)this + offset);
    }

    ~Singleton(void)
    {
        assert(ms_Singleton);
        ms_Singleton = 0;
    }

    static T& GetSingleton(void)
    {
        assert(ms_Singleton); 
		return (*ms_Singleton);
    }

    static T* GetSingletonPtr(void)
    {
       assert(ms_Singleton);
        return ms_Singleton;
    }
};

template <typename T> T* Singleton <T>::ms_Singleton = 0;

#endif
