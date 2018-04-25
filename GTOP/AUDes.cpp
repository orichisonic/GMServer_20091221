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
//	测试用数据 


BYTE	TestPackageBuffer[512] = 
{
	//	0x10, 0x00, 0x11, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x03, 0x61, 0x62, 0x63, 0x00, 0x00, 0x00, 0x00,
	//0x08, 00, 0x0c, 00, 00, 00, 0x14, 0x0
	0x19, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x0C, 0x73, 0x63, 0x6F, 0x72, 0x70, 0x69, 0x6F, 0x6E, 0x20, 0x31, 0x31, 0x31, 0x00, 0x00, 0x00, 0x00
};

BYTE	TestOutOut[512];


// [12/12/2008 yuan_yao]
//	劲舞团 5.1 加密算法
/************************************************************************\
	[12/12/2008 yuan_yao]                                    
	
	劲舞团5.1版加密算法如下:
		总的流程:

		Step1.使用一个动态生成的密钥数组，对整个明文包进行循环加密。

		Step2.使用一个动态获取的选择子，在一个固定内容的数组中选取一个因子，
			  并用这个因子将密钥数组的长度进行加密（算法不同于明文包加密算法）。
	
		Step3.重新构造包结构。

		Step4.完成。

		重新构造后的包结构为: 
				包长度(自身)			+	密钥				+		密文
				|-----16bit------|		+ |---0 < x <= 9--- |			|---|
		
		在Step1中，密钥数组的长度使用随机数动态获取，范围为(0,9]。
		在Step2中，选择子的获取依赖于一个伪随机数，这个伪随机数在 [0,9] 之间
				   循环线性递增，幅度为1。


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
	//	获取新Key数组的长度 
	NewKeyVectorSize = GetNewKeyVectorSize();
	
	// [12/12/2008 yuan_yao]
	//	 生成Key数组 这里有问题
	for(INT Loop_i=1; Loop_i < NewKeyVectorSize+1; Loop_i++ )
		{
			NewKeyVector[Loop_i] = (BYTE)(gGetPackageSeed() % Keyfactor);
		}
	
	// [12/12/2008 yuan_yao]
	//	加密 
	Loop_End = lpPackageM.PackageData->PackageSize;
	for(INT Loop_j = 0; Loop_j < Loop_End; Loop_j++)
		{
			E1 = Loop_j%(NewKeyVectorSize-1);
			E1 = NewKeyVector[E1+1]^lpPackageM.lpBytePtr[Loop_j];
			E1 ^= (Loop_End - Loop_j);
			DataEd[Loop_j] = E1;
		}
		
	// [12/12/2008 yuan_yao]
	//	将密钥长度加密 
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
	//	构造新包 
	NewKeyVector[0] = NewKeyVectorSize;
	memcpy(lpOutData+2, NewKeyVector, __NewKeyVectorSize);
	memcpy(lpOutData+2 + __NewKeyVectorSize, DataEd, lpPackageM.PackageData->PackageSize);
	*((unsigned short int*)lpOutData) = 
			(unsigned short int)(lpPackageM.PackageData->PackageSize + __NewKeyVectorSize + sizeof(unsigned short int));

}
PackageType	gPackage;