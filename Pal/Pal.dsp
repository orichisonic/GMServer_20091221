# Microsoft Developer Studio Project File - Name="Pal" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Pal - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Pal.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Pal.mak" CFG="Pal - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Pal - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Pal - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Pal - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 ..\\NetIOCP\\Release\\NetIOCP.lib ..\\GameIOCP\\Release\\GameIOCP.lib /nologo /subsystem:windows /dll /machine:I386 /nodefaultlib:"LIBC.lib" /def:".\Pal.def" /force
# SUBTRACT LINK32 /pdb:none /map
# Begin Special Build Tool
TargetPath=.\Release\Pal.dll
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy "$(TargetPath)" "..\release\" /y /s
# End Special Build Tool

!ELSEIF  "$(CFG)" == "Pal - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ..\\NetIOCP\\Debug\\NetIOCP.lib ..\\GameIOCP\\Debug\\GameIOCP.lib /nologo /subsystem:windows /dll /map /debug /machine:I386 /nodefaultlib:"LIBC.lib" /force /pdbtype:sept
# Begin Special Build Tool
TargetPath=.\Debug\Pal.dll
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy "$(TargetPath)" "..\Debug\" /y /s
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "Pal - Win32 Release"
# Name "Pal - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AccountInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\BanishPlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\CharacterAdvanceInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\CharacterBag.cpp
# End Source File
# Begin Source File

SOURCE=.\CharacterBlackList.cpp
# End Source File
# Begin Source File

SOURCE=.\CharacterFriendList.cpp
# End Source File
# Begin Source File

SOURCE=.\CharacterInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\CharacterMob.cpp
# End Source File
# Begin Source File

SOURCE=.\CharacterPuzzleMap.cpp
# End Source File
# Begin Source File

SOURCE=.\CharacterQuest.cpp
# End Source File
# Begin Source File

SOURCE=.\CharacterSkill.cpp

!IF  "$(CFG)" == "Pal - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Pal - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DelCharacterItem.cpp
# End Source File
# Begin Source File

SOURCE=.\DelCharacterMob.cpp
# End Source File
# Begin Source File

SOURCE=.\DismissGuild.cpp
# End Source File
# Begin Source File

SOURCE=.\GuildBaseInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\GuildMemberList.cpp
# End Source File
# Begin Source File

SOURCE=.\ModifyAccountBankPw.cpp
# End Source File
# Begin Source File

SOURCE=.\ModifyAccountPw.cpp
# End Source File
# Begin Source File

SOURCE=.\ModifyCharacterDropPw.cpp
# End Source File
# Begin Source File

SOURCE=.\ModifyCharacterMoney.cpp
# End Source File
# Begin Source File

SOURCE=.\ModifyCharacterQuest.cpp
# End Source File
# Begin Source File

SOURCE=.\ModifyGuildLv.cpp
# End Source File
# Begin Source File

SOURCE=.\ModifyGuildMemberPL.cpp
# End Source File
# Begin Source File

SOURCE=.\ModifyGuildName.cpp
# End Source File
# Begin Source File

SOURCE=.\ModifyGuildNotice.cpp
# End Source File
# Begin Source File

SOURCE=.\ModifyPuzzleMap.cpp
# End Source File
# Begin Source File

SOURCE=.\ModifySafeLock.cpp
# End Source File
# Begin Source File

SOURCE=.\ModifySecLvlPwd.cpp
# End Source File
# Begin Source File

SOURCE=.\MoveCharacterToSafe.cpp
# End Source File
# Begin Source File

SOURCE=.\OperPal.cpp
# End Source File
# Begin Source File

SOURCE=.\Pal.cpp
# End Source File
# Begin Source File

SOURCE=.\Pal.def

!IF  "$(CFG)" == "Pal - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Pal - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Pal.rc
# End Source File
# Begin Source File

SOURCE=.\PalAPI.cpp
# End Source File
# Begin Source File

SOURCE=.\ReloadFunctionSwitch.cpp
# End Source File
# Begin Source File

SOURCE=.\SendCustomItemMail.cpp
# End Source File
# Begin Source File

SOURCE=.\SendGiftMail.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AccountInfo.h
# End Source File
# Begin Source File

SOURCE=.\BanishPlayer.h
# End Source File
# Begin Source File

SOURCE=.\BaseStruct.h
# End Source File
# Begin Source File

SOURCE=.\CharacterAdvanceInfo.h
# End Source File
# Begin Source File

SOURCE=.\CharacterBag.h
# End Source File
# Begin Source File

SOURCE=.\CharacterBlackList.h
# End Source File
# Begin Source File

SOURCE=.\CharacterFriendList.h
# End Source File
# Begin Source File

SOURCE=.\CharacterInfo.h
# End Source File
# Begin Source File

SOURCE=.\CharacterMob.h
# End Source File
# Begin Source File

SOURCE=.\CharacterPuzzleMap.h
# End Source File
# Begin Source File

SOURCE=.\CharacterQuest.h
# End Source File
# Begin Source File

SOURCE=.\CharacterSkill.h
# End Source File
# Begin Source File

SOURCE=.\DelCharacterItem.h
# End Source File
# Begin Source File

SOURCE=.\DelCharacterMob.h
# End Source File
# Begin Source File

SOURCE=.\DismissGuild.h
# End Source File
# Begin Source File

SOURCE=.\GuildBaseInfo.h
# End Source File
# Begin Source File

SOURCE=.\GuildMemberList.h
# End Source File
# Begin Source File

SOURCE=.\md5.h
# End Source File
# Begin Source File

SOURCE=.\ModifyAccountBankPw.h
# End Source File
# Begin Source File

SOURCE=.\ModifyAccountPw.h
# End Source File
# Begin Source File

SOURCE=.\ModifyCharacterDropPw.h
# End Source File
# Begin Source File

SOURCE=.\ModifyCharacterMoney.h
# End Source File
# Begin Source File

SOURCE=.\ModifyCharacterQuest.h
# End Source File
# Begin Source File

SOURCE=.\ModifyGuildLv.h
# End Source File
# Begin Source File

SOURCE=.\ModifyGuildMemberPL.h
# End Source File
# Begin Source File

SOURCE=.\ModifyGuildName.h
# End Source File
# Begin Source File

SOURCE=.\ModifyGuildNotice.h
# End Source File
# Begin Source File

SOURCE=.\ModifyPuzzleMap.h
# End Source File
# Begin Source File

SOURCE=.\ModifySafeLock.h
# End Source File
# Begin Source File

SOURCE=.\ModifySecLvlPwd.h
# End Source File
# Begin Source File

SOURCE=.\MoveCharacterToSafe.h
# End Source File
# Begin Source File

SOURCE=.\OperPal.h
# End Source File
# Begin Source File

SOURCE=.\Pal.h
# End Source File
# Begin Source File

SOURCE=.\PalAPI.h
# End Source File
# Begin Source File

SOURCE=.\ReloadFunctionSwitch.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SendCustomItemMail.h
# End Source File
# Begin Source File

SOURCE=.\SendGiftMail.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Pal.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
