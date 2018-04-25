#ifndef PAL_DATAREADER_H
#define PAL_DATAREADER_H

#define PUZZLEMAP_CUTCOUNT		9		// �C�ӹ�ų�����ϼƶq
#define ROLENAME_MAX			32		// �̤j����r��

//====================================================================================================
// ���c�w�q
//====================================================================================================

// �����ų��T---------------------------------------------------------------------------------------
struct S_PuzzleMapInfo
{
	int  iZoneID;								// �a�Ͻs��
	char szName[ROLENAME_MAX];					// ��ų�W��
	int	 iMapCollection[PUZZLEMAP_CUTCOUNT];	// �a�Ϧ����Ϊ�Flag�X��
};

// ��ų�ާ@���c---------------------------------------------------------------------------------------
struct S_GMPuzzleMapProc
{
	int  iZoneID;						// �ϰ�s��
	int  iFlagID[PUZZLEMAP_CUTCOUNT];	// �X�нs��
	bool bIsOpen[PUZZLEMAP_CUTCOUNT];	// �}��
	
	S_GMPuzzleMapProc()
	{
		iZoneID = -1;
		
		for (int i=0; i<PUZZLEMAP_CUTCOUNT; i++)
		{
			iFlagID[i] = -1;
		}
	}
};

// �Ȼs���~���c---------------------------------------------------------------------------------------
struct S_GMCustomItem
{
	int  iCustomItemID;	// �Ȼs���~�s��
	int  iType;			// ����(1�Z�� 2���� 3�s�F�Z�P���]����)
	int  iItemGUID;		// ���~�s��
	char szName[32];	// ���~�W��

	S_GMCustomItem()
	{
		memset(this, 0, sizeof(S_GMCustomItem));
	}
};

//====================================================================================================
void Pal_LoadData(char* szDataDir);					// ���J�һݪ�DBF��

// ���o�W��-------------------------------------------------------------------------------------------
const char* Pal_GetPetName		(int iPetID);		// �d��
const char* Pal_GetPetSkillName	(int iIndex);		// �d���ޯ�
const char* Pal_GetItemName		(int iItemID);		// �D��
const char* Pal_GetQuestName	(int iQuestID);		// ����
const char* Pal_GetZoneName		(int iZoneID);		// �ϰ�
const char* Pal_GetFameName		(int iFameID);		// �n��
const char* Pal_GetLiveHoodName	(int iLiveHoodID);	// �Ͳ��ޯ�
const char* Pal_GetTitleName	(int iTitleID);		// �ٸ�

// ���]����-------------------------------------------------------------------------------------------
void Pal_GetMobFlagResult(int *dwResult, int *dwMobFlag);	// ���]�����T
int  Pal_GetMobID(int iIndex);		// ���]���Ǫ��s��(�^��-1:�ӥ��]�s�����s�b)
int  Pal_GetMobFlag(int iMobID);	// ���]���X�нs��(�^��-1:�ӥ��]�s�����s�b)

// ��ų����-------------------------------------------------------------------------------------------
int  Pal_GetLocalCount();	// �t����ų���a���`��
void Pal_GetPuzzleMapFlagResult(S_PuzzleMapInfo *pPuzzleMapInfo, int *dwPuzzleMapFlag);	// ��ų�����T
bool Pal_GetMobPuzzleMapFlag(S_GMPuzzleMapProc *sGMPuzzleMapProc);	// ��ų�X�нs��(�^��false:�ϰ�s�����s�b)

// ���o�䥦��T---------------------------------------------------------------------------------------
int  Pal_GetSkillLv(int iVocType, int iIndex, int iMagicID);	// ¾�~�ޯ൥��
void Pal_GetTitleFlagResult(int *dwResult, int *dwTitleFlag);	// �ٸ������T
bool Pal_GetSavePoint(int iZoneID, int *pX, int *pY, int *pZ);	// �H�ϰ�s���d�ߦw���I�y��(�^��false:�ϰ�s�����s�b)
bool Pal_GetGMCustomItem(S_GMCustomItem *pGMCustomItem);		// �Ȼs���~��T(�H�Ȼs���~�s���d�ߧ����T,�i�q1�}�l�̴M�d�ߦ�false)



#endif

