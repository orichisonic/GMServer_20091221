#include "md5.h"
#include <afxwin.h>         // MFC core and standard components
#include "MD5Checksum.h"
#pragma comment(lib,"MD5.lib")
void  wcharToChar(char * psText,int* parseLength,CString str);

bool MD5_EnCode(char * pszOutText,int* iOut,char *pszInText,int iIn)
{
	wcharToChar(pszOutText,iOut,CMD5Checksum::GetMD5((BYTE *)pszInText,iIn));
/*	int i=0;
	int j=0;
	char szPassword[100];
	ZeroMemory(szPassword,100);
	for(i=0;i<*iOut;i++)
	{

		if(pszOutText[i]>='a' && pszOutText[i]<='z')
		{
			szPassword[j]=pszOutText[i]-0x20;
		}
		else
		{
			szPassword[j]=pszOutText[i];
		}
		j++;
		if(i!=0&&i!=(*iOut-1)&&(i-1)%2==0)
		{
			szPassword[j]='-';
				j++;
		}
	}*/
	return true;
}
void  wcharToChar(char * psText,int* parseLength,CString str)
{
	int length=str.GetLength();
	strncpy(psText,str,length);  
	psText[length+1]=0;
	*parseLength = length;
}

bool MD5_Password(char * szPassword,int* iOut,char *pszInText,int iIn)
{
	char pszOutText[100];
	
	wcharToChar(pszOutText,iOut,CMD5Checksum::GetMD5((BYTE *)pszInText,iIn));
	int i=0;
	int j=0;
	
	ZeroMemory(szPassword,100);
	for(i=0;i<*iOut;i++)
	{

		if(pszOutText[i]>='a' && pszOutText[i]<='z')
		{
			szPassword[j]=pszOutText[i]-0x20;
		}
		else
		{
			szPassword[j]=pszOutText[i];
		}
		j++;
		if(i!=0&&i!=(*iOut-1)&&(i-1)%2==0)
		{
			szPassword[j]='-';
				j++;
		}
	}
	return true;
}