#ifndef PAL_DATAREADER_H
#define PAL_DATAREADER_H

#define PUZZLEMAP_CUTCOUNT		9		// 每個圖鑒的拼圖數量
#define ROLENAME_MAX			32		// 最大角色字數

//====================================================================================================
// 結構定義
//====================================================================================================

// 角色圖鑒資訊---------------------------------------------------------------------------------------
struct S_PuzzleMapInfo
{
	int  iZoneID;								// 地圖編號
	char szName[ROLENAME_MAX];					// 圖鑒名稱
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

// 客製物品結構---------------------------------------------------------------------------------------
struct S_GMCustomItem
{
	int  iCustomItemID;	// 客製物品編號
	int  iType;			// 類型(1武器 2防具 3御靈卵與伏魔元神)
	int  iItemGUID;		// 物品編號
	char szName[32];	// 物品名稱

	S_GMCustomItem()
	{
		memset(this, 0, sizeof(S_GMCustomItem));
	}
};

//====================================================================================================
void Pal_LoadData(char* szDataDir);					// 載入所需的DBF檔

// 取得名稱-------------------------------------------------------------------------------------------
const char* Pal_GetPetName		(int iPetID);		// 寵物
const char* Pal_GetPetSkillName	(int iIndex);		// 寵物技能
const char* Pal_GetItemName		(int iItemID);		// 道具
const char* Pal_GetQuestName	(int iQuestID);		// 任務
const char* Pal_GetZoneName		(int iZoneID);		// 區域
const char* Pal_GetFameName		(int iFameID);		// 聲望
const char* Pal_GetLiveHoodName	(int iLiveHoodID);	// 生產技能
const char* Pal_GetTitleName	(int iTitleID);		// 稱號

// 伏魔相關-------------------------------------------------------------------------------------------
void Pal_GetMobFlagResult(int *dwResult, int *dwMobFlag);	// 伏魔具體資訊
int  Pal_GetMobID(int iIndex);		// 伏魔錄怪物編號(回傳-1:該伏魔編號不存在)
int  Pal_GetMobFlag(int iMobID);	// 伏魔錄旗標編號(回傳-1:該伏魔編號不存在)

// 圖鑒相關-------------------------------------------------------------------------------------------
int  Pal_GetLocalCount();	// 含有圖鑒的地圖總數
void Pal_GetPuzzleMapFlagResult(S_PuzzleMapInfo *pPuzzleMapInfo, int *dwPuzzleMapFlag);	// 圖鑒具體資訊
bool Pal_GetMobPuzzleMapFlag(S_GMPuzzleMapProc *sGMPuzzleMapProc);	// 圖鑒旗標編號(回傳false:區域編號不存在)

// 取得其它資訊---------------------------------------------------------------------------------------
int  Pal_GetSkillLv(int iVocType, int iIndex, int iMagicID);	// 職業技能等級
void Pal_GetTitleFlagResult(int *dwResult, int *dwTitleFlag);	// 稱號具體資訊
bool Pal_GetSavePoint(int iZoneID, int *pX, int *pY, int *pZ);	// 以區域編號查詢安全點座標(回傳false:區域編號不存在)
bool Pal_GetGMCustomItem(S_GMCustomItem *pGMCustomItem);		// 客製物品資訊(以客製物品編號查詢完整資訊,可從1開始依尋查詢至false)



#endif

