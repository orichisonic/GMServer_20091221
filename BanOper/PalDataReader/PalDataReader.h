#ifndef PAL_DATAREADER_H
#define PAL_DATAREADER_H

#define PUZZLEMAP_CUTCOUNT		9		// 每個圖鑒的拼圖數量
#define ROLENAME_MAX			32		// 最大角色字數

// 角色圖鑒資訊---------------------------------------------------------------------------------------
struct S_PuzzleMapInfo
{
	char szName[ROLENAME_MAX];							// 圖鑒名稱
	int	 iMapCollection[PUZZLEMAP_CUTCOUNT];	// 地圖收集用的Flag旗標
};
// 圖鑒操作結構---------------------------------------------------------------------------------------
struct S_GMPuzzleMapProc
{
	int  iZoneID;						// 區域編號
	int  iFlagID[PUZZLEMAP_CUTCOUNT];	// 旗標編號
	bool bIsOpen[PUZZLEMAP_CUTCOUNT];	// 開關
	
	S_GMPuzzleMapProc()
	{
		iZoneID = -1;
		
		for (int i=0; i<PUZZLEMAP_CUTCOUNT; i++)
		{
			iFlagID[i] = -1;
		}
	}
};

// 載入所需的DBF檔
void Pal_LoadData(char* szDataDir);

// 取得寵物名稱
const char* Pal_GetPetName(int iPetID);

// 取得道具名稱
const char* Pal_GetItemName(int iItemID);

// 取得任務名稱
const char* Pal_GetQuestName(int iQuestID);

// 取得區域名稱
const char* Pal_GetZoneName(int iZoneID);

// 取得聲望名稱
const char* Pal_GetFameName(int iFameID);

// 取得職業技能等級
int Pal_GetSkillLv(int iVocType, int iIndex, int iMagicID);

// 取得生產技能名稱
const char* Pal_GetLiveHoodName(int iLiveHoodID);

// 取得伏魔具體資訊
void Pal_GetMobFlagResult(int *dwResult, int *dwMobFlag);
// 取得伏魔錄怪物編號(-1代表該伏魔編號不存在)
int  Pal_GetMobID(int iIndex);
// 取得伏魔錄旗標編號(-1代表該伏魔編號不存在)
int  Pal_GetMobFlag(int iMobID);

// 取得含有圖鑒的地圖總數
int  Pal_GetLocalCount();
// 取得圖鑒具體資訊
void Pal_GetPuzzleMapFlagResult(S_PuzzleMapInfo *pPuzzleMapInfo, int *dwPuzzleMapFlag);
// 取得圖鑒旗標編號(false代表區域編號不存在)
bool Pal_GetMobPuzzleMapFlag(S_GMPuzzleMapProc *sGMPuzzleMapProc);

// 取得稱號具體資訊
void Pal_GetTitleFlagResult(int *dwResult, int *dwTitleFlag);
// 取得稱號名稱
const char* Pal_GetTitleName(int iTitleID);

// 以區域編號來查詢安全點座標(false代表區域編號不存在)
bool Pal_GetSavePoint(int iZoneID, int *pX, int *pY, int *pZ);

#endif

