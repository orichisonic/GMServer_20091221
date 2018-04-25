#ifndef  CRC_H_
#define  CRC_H_

#include "windows.h"
  
class __declspec(dllexport) CCrc  
{  
private:
	int m_nWidth;  
	DWORD m_Ponynomial;  
	DWORD m_dwPreCalc;  
	DWORD m_dwPostCalc;  
	BOOL m_bMsbFirst;  
	BOOL m_bReverse;  
	DWORD Reverse(ULONG dw, int nWidth);  
public:  
	CCrc(int nWidth, DWORD Ponynomial, DWORD dwPreCalc, DWORD dwPostCalc, BOOL bMsbFirst = FALSE, BOOL bReverse = FALSE);  
	DWORD Calc(BYTE *pData, int nLen);  
};  
 
#endif //__CRC_H__  