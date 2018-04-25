#if !defined _SYNCHRONIZE_
#define _SYNCHRONIZE_

class  Synchronize
{
public:

	Synchronize(CCriticalSection & csSection):m_pcsSection(&csSection)
	{	
		m_pcsSection->Lock();
	}

	virtual ~Synchronize()
	{
		m_pcsSection->Unlock();
	}
	
private:
	CCriticalSection *	m_pcsSection;

};

#endif _SYNCHRONIZE_

