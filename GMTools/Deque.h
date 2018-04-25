#ifndef _DEQUE_H_
#define _DEQUE_H_
#pragma warning( disable : 4786 )
#pragma warning( disable : 4284 )
#pragma warning( disable : 4146 )
#pragma warning( disable : 4018 )

#pragma warning( push, 3 )
#include <deque>
#include <algorithm>
#pragma warning( pop )
using namespace std;

/********************************************************************************

*********************************************************************************/


template< class __T__ >
class Deque
{
protected:
	typedef std::deque< __T__ >			DequeObject;

private:
	DequeObject							m_dequeObject;
	typename DequeObject::iterator		m_dequePosition;

public:
	inline void removeAll()
	{
		DequeObject::const_iterator iter_end( m_dequeObject.end() );
		for ( DequeObject::const_iterator iter = m_dequeObject.begin(); iter != iter_end; ++iter )
		{
			__T__ value = ( *iter );
			if( value )
				delete value;
		}
		
		clear();
	}

	inline void clear()
	{
		m_dequeObject.clear();
	}

	inline bool isEmpty()
	{
		return ( m_dequeObject.empty() );
	}

	inline int size()
	{
		return (int)( m_dequeObject.size() );
	}

	inline bool isExist( __T__ key )
	{
		DequeObject::iterator iter = std::find( m_dequeObject.begin(), m_dequeObject.end(), key );
		return ( iter != m_dequeObject.end() );
	}

	inline __T__ pop()
	{
		DequeObject::iterator iter = m_dequeObject.begin();
		if ( iter == m_dequeObject.end() ) return ( NULL );
		__T__ element = ( *iter );
		m_dequeObject.erase( iter );
		return ( element );
	}

	inline void put( __T__ Value )
	{
		m_dequeObject.push_back( Value );
	}

	inline void putToFront( __T__ Value )
	{
		m_dequeObject.push_front( Value );
	}

	inline void putToBack( __T__ Value )
	{
		m_dequeObject.push_back( Value );
	}

	inline __T__ getHead()
	{
		m_dequePosition = m_dequeObject.begin();
		if( m_dequePosition != m_dequeObject.end() )
			return ( *m_dequePosition );
		else
			return ( NULL );
	}

	inline __T__ getNext()
	{
		if( m_dequePosition != m_dequeObject.end() )
		{
			m_dequePosition++;
			if( m_dequePosition != m_dequeObject.end() )
				return ( *m_dequePosition );
		}

		return ( NULL );
	}

	inline __T__ getBefore( __T__ value )
	{
		DequeObject::iterator iter = find( m_dequeObject.begin(), m_dequeObject.end(), value );
		if( iter == m_dequeObject.end() )		return ( NULL );
		if( iter == m_dequeObject.begin() )		return ( NULL );

		iter--;
		return ( *iter );
	}

	inline __T__ getAfter( __T__ value )
	{
		DequeObject::iterator iter = find( m_dequeObject.begin(), m_dequeObject.end(), value );
		if( iter == m_dequeObject.end() )		return ( NULL );

		iter++;		// beforeÀÏ°æ¿ì iter--;
		if( iter == m_dequeObject.end() )		return ( NULL );
		return ( *iter );
	}

	inline bool remove( __T__ value )
	{
		DequeObject::iterator iter = std::find( m_dequeObject.begin(), m_dequeObject.end(), value );
		if( iter != m_dequeObject.end() )
		{
			__T__ value = ( *iter );
			m_dequeObject.erase( iter );
			m_dequePosition = iter;
			if( value )		delete ( value );
			return true;
		}
		else
			return false;
	}

	inline bool erase( __T__ value )
	{
		DequeObject::iterator iter = find( m_dequeObject.begin(), m_dequeObject.end(), value );
		if( iter != m_dequeObject.end() )
		{
			m_dequeObject.erase( iter );
			m_dequePosition = iter;
			return true;
		}
		else
			return false;
	}

	inline __T__ eraseThisPosition()
	{
		if( m_dequePosition != m_dequeObject.end() )
		{
			m_dequePosition = m_dequeObject.erase( m_dequePosition );
			if( m_dequePosition != m_dequeObject.end() )
			{
				return ( *m_dequePosition );
			}
			else 
				return ( NULL );
		}
		else
			return ( NULL );						
	}

	inline bool isEnd()
	{
		return ( m_dequePosition == m_dequeObject.end() );
	}

	inline void assign( Deque *deque )
	{
		if( !deque )		return;

		m_dequeObject.assign( deque->m_dequeObject.begin(), deque->m_dequeObject.end() );
	}
	
	inline void randomShuffle()
	{
		random_shuffle( m_dequeObject.begin(), m_dequeObject.end() );
	}

	inline __T__	getValue( int index )
	{
		if( ( m_dequeObject.size() <= index ) || ( index < 0 ) )
			return ( 0 );
		
		DequeObject::iterator iter = ( m_dequeObject.begin() + index );
		if( iter != m_dequeObject.end() )
			return ( *iter );
		else
			return ( 0 );
	}

	inline __T__ operator [] ( int index )
	{
		return ( getValue(index) );
	}

	inline void setValue( int index, __T__ value )
	{
		if( ( m_dequeObject.size() <= index ) || ( index < 0 ) )
			return;

		m_dequeObject[index] = value;
	}

public:
	Deque()
	{
		m_dequeObject.clear();
	}

	virtual ~Deque()
	{
		clear();
	}
};

#endif