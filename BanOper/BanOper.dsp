# Microsoft Developer Studio Project File - Name="BanOper" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=BanOper - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BanOper.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BanOper.mak" CFG="BanOper - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BanOper - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "BanOper - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "BanOper - Win32 Release"

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
# ADD LINK32 ..\\NetIOCP\\Release\\NetIOCP.lib ..\\GameIOCP\\Release\\GameIOCP.lib /nologo /subsystem:windows /dll /machine:I386
# Begin Special Build Tool
TargetPath=.\Release\BanOper.dll
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy "$(TargetPath)" "..\release\" /y /s
# End Special Build Tool

!ELSEIF  "$(CFG)" == "BanOper - Win32 Debug"

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
# ADD LINK32 ..\\NetIOCP\\Debug\\NetIOCP.lib ..\\GameIOCP\\Debug\\GameIOCP.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# Begin Special Build Tool
TargetPath=.\Debug\BanOper.dll
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy "$(TargetPath)" "..\Debug\" /y /s
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "BanOper - Win32 Release"
# Name "BanOper - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AUBan.cpp
# End Source File
# Begin Source File

SOURCE=.\BanOper.cpp
# End Source File
# Begin Source File

SOURCE=.\BanOper.def
# End Source File
# Begin Source File

SOURCE=.\BanOper.rc
# End Source File
# Begin Source File

SOURCE=.\CG2Ban.cpp
# End Source File
# Begin Source File

SOURCE=.\FJBan.cpp
# End Source File
# Begin Source File

SOURCE=.\GlobalDefine.cpp
# End Source File
# Begin Source File

SOURCE=.\GTBan.cpp
# End Source File
# Begin Source File

SOURCE=.\JW2Ban.cpp
# End Source File
# Begin Source File

SOURCE=.\MFBan.cpp
# End Source File
# Begin Source File

SOURCE=.\MySocket.cpp
# End Source File
# Begin Source File

SOURCE=.\OperBan.cpp
# End Source File
# Begin Source File

SOURCE=.\packet.cpp
# End Source File
# Begin Source File

SOURCE=.\PalBan.cpp
# End Source File
# Begin Source File

SOURCE=.\RayBan.cpp
# End Source File
# Begin Source File

SOURCE=.\SDBan.cpp
# End Source File
# Begin Source File

SOURCE=.\SDOBan.cpp
# End Source File
# Begin Source File

SOURCE=.\SltqBan.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AU2_Dll.h
# End Source File
# Begin Source File

SOURCE=.\AUBan.h
# End Source File
# Begin Source File

SOURCE=.\BanOper.h
# End Source File
# Begin Source File

SOURCE=.\CG2Ban.h
# End Source File
# Begin Source File

SOURCE=.\FJBan.h
# End Source File
# Begin Source File

SOURCE=.\GlobalDefine.h
# End Source File
# Begin Source File

SOURCE=.\gmsglobaldefine.h
# End Source File
# Begin Source File

SOURCE=.\gmsprotocols2t.h
# End Source File
# Begin Source File

SOURCE=.\gmsprotocolt2s.h
# End Source File
# Begin Source File

SOURCE=.\GTBan.h
# End Source File
# Begin Source File

SOURCE=.\JW2Ban.h
# End Source File
# Begin Source File

SOURCE=.\MFBan.h
# End Source File
# Begin Source File

SOURCE=.\MySocket.h
# End Source File
# Begin Source File

SOURCE=.\OperBan.h
# End Source File
# Begin Source File

SOURCE=.\packet.h
# End Source File
# Begin Source File

SOURCE=.\PalBan.h
# End Source File
# Begin Source File

SOURCE=.\RayBan.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SDBan.h
# End Source File
# Begin Source File

SOURCE=.\SDOBan.h
# End Source File
# Begin Source File

SOURCE=.\SltqBan.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\BanOper.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
