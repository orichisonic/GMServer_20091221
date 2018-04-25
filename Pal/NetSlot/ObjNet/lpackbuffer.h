
#ifndef _LPACKBUFFER_H_
#define _LPACKBUFFER_H_


#define _DEF_PACK_HEAD_ID1_ 0xAC231234
#define _DEF_PACK_HEAD_ID2_ 0x34221A3C

struct BufferStruct
{
	unsigned short Size;
	char Data[64*1204];		
};

struct Pack_Head_Struct
{
	unsigned  int     Pack_Head_Identity1;		
	unsigned  int     Pack_Head_Identity2;		
	int		          TotalSize;				
	int				  Count;					
	BufferStruct	  Data;				        
	void Init() {
		Pack_Head_Identity1 = _DEF_PACK_HEAD_ID1_;
		Pack_Head_Identity2 = _DEF_PACK_HEAD_ID2_;
		TotalSize=0;
		Count=0;
	};
	bool IsPackData () {
		if( (Pack_Head_Identity1 == _DEF_PACK_HEAD_ID1_)&&
			(Pack_Head_Identity2 == _DEF_PACK_HEAD_ID2_) ) {
			return true;
		} 
		else {
			return false;
		}
	}
};

class cPackBufferMan
{
	int		m_PackSize;									
	std::vector <BufferStruct*>		m_InputBuffer;		
	std::queue <Pack_Head_Struct*>	m_OutputBuffer;		
	std::map<int,BufferStruct*>		m_RecvPack;			
	
public:
	cPackBufferMan(int IniPackSize=1024);
	~cPackBufferMan();
	bool	Add(int nSize , void* Buff );					
	bool	AddBuffer(int nSize , void* Buff );				
	void	SetPackSize(int Size);							
	bool	GetPackBuffer(int *nSize,void** OutBuffer);		
	int		GetPrePackCount();								
	int		GetPackCount();									
	int		ProcessPack();									
	void	PopPack();										
	bool	IsEmpty()	{	return 	m_InputBuffer.size()==0?true:false;}
	
	int	ProcessUnPack(Pack_Head_Struct* Head , int Size );
	//
	BufferStruct*  GetUnPackData(int Index);
	void Clear();
	inline int PackSize() { return m_PackSize; }
};
#endif