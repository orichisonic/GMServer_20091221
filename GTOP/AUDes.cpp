#include "StdAfx.h"
#include "AUDes.h"
#include "Windows.h"

BYTE	static	gGetPackageEdFactor()
{
	static	BYTE __x = 0 ;
	
	__x = (__x+1) % 10;

	return __x;
}


DWORD	static	gGetPackageSeed(VOID)
{

	return rand();

}

DWORD	GetNewKeyVectorSize(DWORD StepVar = 2)
{
	return rand()%9 + StepVar;
}



typedef struct	tagPackageType
{
	union	
		{
			VOID*	lpVoidPtr;
			BYTE*	lpBytePtr;
			struct
				{
					unsigned short int	PackageSize;
					BYTE	PackageData[PACKAGE_SIZE];
				
				}PackageData;
		};
}PackageType;



typedef	union	tagAu0x51PackageM
{
	VOID*	lpVoidPtr;
	BYTE*	lpBytePtr;
	struct
		{
			unsigned short int	PackageSize;
			BYTE				PackageData[PACKAGE_SIZE - sizeof(unsigned short int)];
		}*PackageData;


}Au0x51Package;


// [12/12/2008 yuan_yao]
//	���������� 


BYTE	TestPackageBuffer[512] = 
{
	//	0x10, 0x00, 0x11, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x03, 0x61, 0x62, 0x63, 0x00, 0x00, 0x00, 0x00,
	//0x08, 00, 0x0c, 00, 00, 00, 0x14, 0x0
	0x19, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x0C, 0x73, 0x63, 0x6F, 0x72, 0x70, 0x69, 0x6F, 0x6E, 0x20, 0x31, 0x31, 0x31, 0x00, 0x00, 0x00, 0x00
};

BYTE	TestOutOut[512];


// [12/12/2008 yuan_yao]
//	������ 5.1 �����㷨
/************************************************************************\
	[12/12/2008 yuan_yao]                                    
	
	������5.1������㷨����:
		�ܵ�����:

		Step1.ʹ��һ����̬���ɵ���Կ���飬���������İ�����ѭ�����ܡ�

		Step2.ʹ��һ����̬��ȡ��ѡ���ӣ���һ���̶����ݵ�������ѡȡһ�����ӣ�
			  ����������ӽ���Կ����ĳ��Ƚ��м��ܣ��㷨��ͬ�����İ������㷨����
	
		Step3.���¹�����ṹ��

		Step4.��ɡ�

		���¹����İ��ṹΪ: 
				������(����)			+	��Կ				+		����
				|-----16bit------|		+ |---0 < x <= 9--- |			|---|
		
		��Step1�У���Կ����ĳ���ʹ���������̬��ȡ����ΧΪ(0,9]��
		��Step2�У�ѡ���ӵĻ�ȡ������һ��α����������α������� [0,9] ֮��
				   ѭ�����Ե���������Ϊ1��


\************************************************************************/
VOID	AUDes::Au0x51E(BYTE* lpBuffer, BYTE* lpOutData)
{
	//BYTE	RandVector[] = {6,2,5,4,6,2,7,8,3,2};
	BYTE	RandVector[] = {3,3,3,3,3,3,3,3,3,3};
	
	BYTE	NewKeyVector[MAX_KEY_VECTOR_SIZE];
	BYTE	DataEd[PACKAGE_SIZE];

	INT		Keyfactor = 0x7f;	
	INT		NewKeyVectorSize, __NewKeyVectorSize;
	
	BYTE	E1;
	INT		Loop_End;
	BYTE	DataEdRol;

	
	Au0x51Package	lpPackageM;
	
	ZeroMemory(&lpPackageM, sizeof(lpPackageM));

	lpPackageM.lpVoidPtr = (Au0x51Package*)lpBuffer;

	ZeroMemory(NewKeyVector, sizeof(NewKeyVector));
	
	// [12/12/2008 yuan_yao]
	//	��ȡ��Key����ĳ��� 
	NewKeyVectorSize = GetNewKeyVectorSize();
	
	// [12/12/2008 yuan_yao]
	//	 ����Key���� ����������
	for(INT Loop_i=1; Loop_i < NewKeyVectorSize+1; Loop_i++ )
		{
			NewKeyVector[Loop_i] = (BYTE)(gGetPackageSeed() % Keyfactor);
		}
	
	// [12/12/2008 yuan_yao]
	//	���� 
	Loop_End = lpPackageM.PackageData->PackageSize;
	for(INT Loop_j = 0; Loop_j < Loop_End; Loop_j++)
		{
			E1 = Loop_j%(NewKeyVectorSize-1);
			E1 = NewKeyVector[E1+1]^lpPackageM.lpBytePtr[Loop_j];
			E1 ^= (Loop_End - Loop_j);
			DataEd[Loop_j] = E1;
		}
		
	// [12/12/2008 yuan_yao]
	//	����Կ���ȼ��� 
	DataEdRol = RandVector[gGetPackageEdFactor()];
	__NewKeyVectorSize = NewKeyVectorSize;
	
	_asm
		{
			push	ecx;
			mov	eax, [__NewKeyVectorSize];
			dec eax;
			
			mov	cl, byte ptr[DataEdRol];
			rol al, cl;
			not	al;
			and eax, 0x000000FF;
			pop		ecx;
			mov	[NewKeyVectorSize], eax;
		}
	
	// [12/12/2008 yuan_yao]
	//	�����°� 
	NewKeyVector[0] = NewKeyVectorSize;
	memcpy(lpOutData+2, NewKeyVector, __NewKeyVectorSize);
	memcpy(lpOutData+2 + __NewKeyVectorSize, DataEd, lpPackageM.PackageData->PackageSize);
	*((unsigned short int*)lpOutData) = 
			(unsigned short int)(lpPackageM.PackageData->PackageSize + __NewKeyVectorSize + sizeof(unsigned short int));

}
PackageType	gPackage;