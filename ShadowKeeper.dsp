# Microsoft Developer Studio Project File - Name="ShadowKeeper" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ShadowKeeper - Win32 Profiling
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ShadowKeeper.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ShadowKeeper.mak" CFG="ShadowKeeper - Win32 Profiling"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ShadowKeeper - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ShadowKeeper - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "ShadowKeeper - Win32 Profiling" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ShadowKeeper - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 version.lib StaticZlib.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"StaticZlib.lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "ShadowKeeper - Win32 Debug"

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
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 version.lib StaticZlib.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "ShadowKeeper - Win32 Profiling"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ShadowKeeper___Win32_Profiling"
# PROP BASE Intermediate_Dir "ShadowKeeper___Win32_Profiling"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Profiling"
# PROP Intermediate_Dir "Profiling"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 version.lib StaticZlib.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# SUBTRACT BASE LINK32 /nodefaultlib
# ADD LINK32 version.lib StaticZlib.lib /nologo /subsystem:windows /profile /debug /machine:I386
# SUBTRACT LINK32 /nodefaultlib

!ENDIF 

# Begin Target

# Name "ShadowKeeper - Win32 Release"
# Name "ShadowKeeper - Win32 Debug"
# Name "ShadowKeeper - Win32 Profiling"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AddAbilitiesDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BitmapWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\BrowseDirsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ChangesMadeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CharTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorPickerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CopyFilesDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CreatureBrowserDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\CreatureBrowserFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\CreatureBrowserView.cpp
# End Source File
# Begin Source File

SOURCE=.\CreatureFilterDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CreatureListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\DaleKeeper.cpp
# End Source File
# Begin Source File

SOURCE=.\DaleKeeper.rc
# End Source File
# Begin Source File

SOURCE=.\DaleKeeperDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\DaleKeeperView.cpp
# End Source File
# Begin Source File

SOURCE=.\DirTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\EditAffectDataDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EditAffectDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EditListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EditUserListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EditVarDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ExportOptionsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ExportResourceDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Global.cpp
# End Source File
# Begin Source File

SOURCE=.\IndexResourcesDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Inf2DA.cpp
# End Source File
# Begin Source File

SOURCE=.\InfBam.cpp
# End Source File
# Begin Source File

SOURCE=.\InfBifFile.cpp
# End Source File
# Begin Source File

SOURCE=.\InfChr.cpp
# End Source File
# Begin Source File

SOURCE=.\InfCreature.cpp
# End Source File
# Begin Source File

SOURCE=.\InfGame.cpp
# End Source File
# Begin Source File

SOURCE=.\InfKey.cpp
# End Source File
# Begin Source File

SOURCE=.\InfTlk.cpp
# End Source File
# Begin Source File

SOURCE=.\InstallPathDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemBrowserCom.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemBrowserDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemBrowserFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemBrowserView.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemFilterDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\LimitSpellsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MDIClientWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\MiscSettingsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NameRefDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\OpenChrDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\OpenCreDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\OpenSavedDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PalImageList.cpp
# End Source File
# Begin Source File

SOURCE=.\PortraitsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SaveChrDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SaveCreDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SaveGameDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetQtyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetValueDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\SortStringArray.cpp
# End Source File
# Begin Source File

SOURCE=.\SpellBitmaps.cpp
# End Source File
# Begin Source File

SOURCE=.\SpellBrowserCom.cpp
# End Source File
# Begin Source File

SOURCE=.\SpellBrowserDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\SpellBrowserFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\SpellBrowserView.cpp
# End Source File
# Begin Source File

SOURCE=.\SpellFilterDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SpellListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\StaticUrl.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StringFinderDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TabAbilitiesDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TabAffectsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TabAppearanceDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TabCharacteristicsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TabGlobalVarsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TabInvDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TabMemSpellsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TabProfsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TabResistancesDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TabSavesDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TabSpellsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TabStatesDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TabThiefSkillsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ValueList.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AddAbilitiesDlg.h
# End Source File
# Begin Source File

SOURCE=.\BitmapWnd.h
# End Source File
# Begin Source File

SOURCE=.\BrowseDirsDlg.h
# End Source File
# Begin Source File

SOURCE=.\ChangesMadeDlg.h
# End Source File
# Begin Source File

SOURCE=.\CharTabCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\ColorPickerDlg.h
# End Source File
# Begin Source File

SOURCE=.\CopyFilesDlg.h
# End Source File
# Begin Source File

SOURCE=.\CreatureBrowserDoc.h
# End Source File
# Begin Source File

SOURCE=.\CreatureBrowserFrame.h
# End Source File
# Begin Source File

SOURCE=.\CreatureBrowserView.h
# End Source File
# Begin Source File

SOURCE=.\CreatureFilterDlg.h
# End Source File
# Begin Source File

SOURCE=.\CreatureListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\DaleKeeper.h
# End Source File
# Begin Source File

SOURCE=.\DaleKeeperDoc.h
# End Source File
# Begin Source File

SOURCE=.\DaleKeeperView.h
# End Source File
# Begin Source File

SOURCE=.\DirTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\EditAffectDataDlg.h
# End Source File
# Begin Source File

SOURCE=.\EditAffectDlg.h
# End Source File
# Begin Source File

SOURCE=.\EditListDlg.h
# End Source File
# Begin Source File

SOURCE=.\EditUserListDlg.h
# End Source File
# Begin Source File

SOURCE=.\EditVarDlg.h
# End Source File
# Begin Source File

SOURCE=.\ExportOptionsDlg.h
# End Source File
# Begin Source File

SOURCE=.\ExportResourceDlg.h
# End Source File
# Begin Source File

SOURCE=.\Global.h
# End Source File
# Begin Source File

SOURCE=.\IndexResourcesDlg.h
# End Source File
# Begin Source File

SOURCE=.\Inf2DA.h
# End Source File
# Begin Source File

SOURCE=.\InfBam.h
# End Source File
# Begin Source File

SOURCE=.\InfBifFile.h
# End Source File
# Begin Source File

SOURCE=.\InfChr.h
# End Source File
# Begin Source File

SOURCE=.\InfCreature.h
# End Source File
# Begin Source File

SOURCE=.\InfGame.h
# End Source File
# Begin Source File

SOURCE=.\InfKey.h
# End Source File
# Begin Source File

SOURCE=.\InfTlk.h
# End Source File
# Begin Source File

SOURCE=.\InstallPathDlg.h
# End Source File
# Begin Source File

SOURCE=.\ItemBrowserCom.h
# End Source File
# Begin Source File

SOURCE=.\ItemBrowserDoc.h
# End Source File
# Begin Source File

SOURCE=.\ItemBrowserFrame.h
# End Source File
# Begin Source File

SOURCE=.\ItemBrowserView.h
# End Source File
# Begin Source File

SOURCE=.\ItemFilterDlg.h
# End Source File
# Begin Source File

SOURCE=.\ItemListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\LimitSpellsDlg.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MDIClientWnd.h
# End Source File
# Begin Source File

SOURCE=.\MiscSettingsDlg.h
# End Source File
# Begin Source File

SOURCE=.\NameRefDlg.h
# End Source File
# Begin Source File

SOURCE=.\OpenChrDlg.h
# End Source File
# Begin Source File

SOURCE=.\OpenCreDlg.h
# End Source File
# Begin Source File

SOURCE=.\OpenSavedDlg.h
# End Source File
# Begin Source File

SOURCE=.\PalImageList.h
# End Source File
# Begin Source File

SOURCE=.\PortraitsDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SaveChrDlg.h
# End Source File
# Begin Source File

SOURCE=.\SaveCreDlg.h
# End Source File
# Begin Source File

SOURCE=.\SaveGameDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetQtyDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetValueDlg.h
# End Source File
# Begin Source File

SOURCE=..\Common\SortStringArray.h
# End Source File
# Begin Source File

SOURCE=.\SpellBitmaps.h
# End Source File
# Begin Source File

SOURCE=.\SpellBrowserCom.h
# End Source File
# Begin Source File

SOURCE=.\SpellBrowserDoc.h
# End Source File
# Begin Source File

SOURCE=.\SpellBrowserFrame.h
# End Source File
# Begin Source File

SOURCE=.\SpellBrowserView.h
# End Source File
# Begin Source File

SOURCE=.\SpellFilterDlg.h
# End Source File
# Begin Source File

SOURCE=.\SpellListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\StaticUrl.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StringFinderDlg.h
# End Source File
# Begin Source File

SOURCE=.\TabAbilitiesDlg.h
# End Source File
# Begin Source File

SOURCE=.\TabAffectsDlg.h
# End Source File
# Begin Source File

SOURCE=.\TabAppearanceDlg.h
# End Source File
# Begin Source File

SOURCE=.\TabCharacteristicsDlg.h
# End Source File
# Begin Source File

SOURCE=.\TabGlobalVarsDlg.h
# End Source File
# Begin Source File

SOURCE=.\TabInvDlg.h
# End Source File
# Begin Source File

SOURCE=.\TabMemSpellsDlg.h
# End Source File
# Begin Source File

SOURCE=.\TabProfsDlg.h
# End Source File
# Begin Source File

SOURCE=.\TabResistancesDlg.h
# End Source File
# Begin Source File

SOURCE=.\TabSavesDlg.h
# End Source File
# Begin Source File

SOURCE=.\TabSpellsDlg.h
# End Source File
# Begin Source File

SOURCE=.\TabStatesDlg.h
# End Source File
# Begin Source File

SOURCE=.\TabThiefSkillsDlg.h
# End Source File
# Begin Source File

SOURCE=.\ValueList.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\DaleKeeper.ico
# End Source File
# Begin Source File

SOURCE=.\res\DaleKeeper.rc2
# End Source File
# Begin Source File

SOURCE=.\res\DaleKeeperDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Group "Rules"

# PROP Default_Filter "txt"
# Begin Source File

SOURCE=.\PROFS.txt
# End Source File
# Begin Source File

SOURCE=.\PROFSMAX.txt
# End Source File
# End Group
# Begin Source File

SOURCE=.\Misc.txt
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
