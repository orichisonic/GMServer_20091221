#ifndef PAL_DATAREADER_H
#define PAL_DATAREADER_H

#define PUZZLEMAP_CUTCOUNT		9		// �C�ӹ�ų�����ϼƶq
#define ROLENAME_MAX			32		// �̤j����r��

// �����ų��T---------------------------------------------------------------------------------------
struct S_PuzzleMapInfo
{
	char szName[ROLENAME_MAX];							// ��ų�W��
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

// ���J�һݪ�DBF��
void Pal_LoadData(char* szDataDir);

// ���o�d���W��
const char* Pal_GetPetName(int iPetID);

// ���o�D��W��
const char* Pal_GetItemName(int iItemID);

// ���o���ȦW��
const char* Pal_GetQuestName(int iQuestID);

// ���o�ϰ�W��
const char* Pal_GetZoneName(int iZoneID);

// ���o�n��W��
const char* Pal_GetFameName(int iFameID);

// ���o¾�~�ޯ൥��
int Pal_GetSkillLv(int iVocType, int iIndex, int iMagicID);

// ���o�Ͳ��ޯ�W��
const char* Pal_GetLiveHoodName(int iLiveHoodID);

// ���o���]�����T
void Pal_GetMobFlagResult(int *dwResult, int *dwMobFlag);
// ���o���]���Ǫ��s��(-1�N��ӥ��]�s�����s�b)
int  Pal_GetMobID(int iIndex);
// ���o���]���X�нs��(-1�N��ӥ��]�s�����s�b)
int  Pal_GetMobFlag(int iMobID);

// ���o�t����ų���a���`��
int  Pal_GetLocalCount();
// ���o��ų�����T
void Pal_GetPuzzleMapFlagResult(S_PuzzleMapInfo *pPuzzleMapInfo, int *dwPuzzleMapFlag);
// ���o��ų�X�нs��(false�N��ϰ�s�����s�b)
bool Pal_GetMobPuzzleMapFlag(S_GMPuzzleMapProc *sGMPuzzleMapProc);

// ���o�ٸ������T
void Pal_GetTitleFlagResult(int *dwResult, int *dwTitleFlag);
// ���o�ٸ��W��
const char* Pal_GetTitleName(int iTitleID);

// �H�ϰ�s���Ӭd�ߦw���I�y��(false�N��ϰ�s�����s�b)
bool Pal_GetSavePoint(int iZoneID, int *pX, int *pY, int *pZ);

#endif

