#ifndef PAL_DATAREADER_H
#define PAL_DATAREADER_H

// Load .dbf file
void Pal_LoadData(char* szDataDir);

// Get Pet Name
const char* Pal_GetPetName(int iPetID);

// Get Item Name
const char* Pal_GetItemName(int iItemID);

// Get Quest Name
const char* Pal_GetQuestName(int iQuestID);

#endif

