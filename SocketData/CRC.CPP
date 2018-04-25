#include "stdafx.h"
#include "CRC.h"

CCrc::CCrc(int nWidth, DWORD Ponynomial, DWORD dwPreCalc, DWORD dwPostCalc, BOOL bMsbFirst, BOOL bReverse)  
:m_nWidth(nWidth),  
m_bMsbFirst(bMsbFirst),  
m_dwPreCalc(dwPreCalc),  
m_dwPostCalc(dwPostCalc),  
m_bReverse(bReverse)  
{  
    m_Ponynomial   =   Reverse(Ponynomial,   nWidth);  
}  

DWORD CCrc::Reverse(DWORD dw, int nWidth)  
{  
	DWORD dw1 = 0;  
	DWORD dw2 = dw;  
	for (int i = 0; i < nWidth; i++)  
	{        
		dw1 <<= 1;  
		dw1 |= (dw2 & 1);  
		dw2 >>= 1;  
	}  
	return dw1;  
}  

DWORD CCrc::Calc(BYTE *pData, int nLen)  
{  
	DWORD dwReg = m_dwPreCalc;  

	for(int i = 0; i < nLen; i++)  
	{  
		BYTE b = pData[i];  
		if(m_bMsbFirst)  
		{
		    b = (BYTE)Reverse(b, 8);  
		}
		dwReg ^= b;  
		for(int j = 0; j <  8; j++)  
		{  
			int c = dwReg & 1;  
			dwReg >>= 1;  
			if(c)
			{
			    dwReg ^= m_Ponynomial;  
			}
		}  
	} 
	if(m_bReverse)  
	{
		dwReg = Reverse(dwReg, m_nWidth); 
	}
	return dwReg ^ m_dwPostCalc;  
}  