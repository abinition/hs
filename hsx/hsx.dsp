# Microsoft Developer Studio Project File - Name="hsx" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=hsx - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "hsx.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "hsx.mak" CFG="hsx - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "hsx - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "hsx - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "hsx - Win32 Release MinSize" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "hsx - Win32 Release MinDependency" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "hsx - Win32 Unicode Release MinSize" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "hsx - Win32 Unicode Release MinDependency" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "hsx - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "hsx - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "c:\abinition\hs\hsx" /I "c:\abinition\hs\code" /I "$(MYSQL_HOME)\include\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "AS_ATL" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib libmysql.lib ssleay32.lib libeay32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept /libpath:"$(OPENSSL_HOME)\out32" /libpath:"$(MYSQL_HOME)\lib\opt"
# Begin Custom Build - Performing registration
OutDir=.\Debug
TargetPath=.\Debug\hsx.dll
InputPath=.\Debug\hsx.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "hsx - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugU"
# PROP BASE Intermediate_Dir "DebugU"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugU"
# PROP Intermediate_Dir "DebugU"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_UNICODE" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_UNICODE" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# Begin Custom Build - Performing registration
OutDir=.\DebugU
TargetPath=.\DebugU\hsx.dll
InputPath=.\DebugU\hsx.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	if "%OS%"=="" goto NOTNT 
	if not "%OS%"=="Windows_NT" goto NOTNT 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	goto end 
	:NOTNT 
	echo Warning : Cannot register Unicode DLL on Windows 95 
	:end 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "hsx - Win32 Release MinSize"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinSize"
# PROP BASE Intermediate_Dir "ReleaseMinSize"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseMinSize"
# PROP Intermediate_Dir "ReleaseMinSize"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_ATL_DLL" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_ATL_DLL" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# Begin Custom Build - Performing registration
OutDir=.\ReleaseMinSize
TargetPath=.\ReleaseMinSize\hsx.dll
InputPath=.\ReleaseMinSize\hsx.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "hsx - Win32 Release MinDependency"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinDependency"
# PROP BASE Intermediate_Dir "ReleaseMinDependency"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseMinDependency"
# PROP Intermediate_Dir "ReleaseMinDependency"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_ATL_STATIC_REGISTRY" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_ATL_STATIC_REGISTRY" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# Begin Custom Build - Performing registration
OutDir=.\ReleaseMinDependency
TargetPath=.\ReleaseMinDependency\hsx.dll
InputPath=.\ReleaseMinDependency\hsx.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "hsx - Win32 Unicode Release MinSize"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseUMinSize"
# PROP BASE Intermediate_Dir "ReleaseUMinSize"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseUMinSize"
# PROP Intermediate_Dir "ReleaseUMinSize"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_UNICODE" /D "_ATL_DLL" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_UNICODE" /D "_ATL_DLL" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# Begin Custom Build - Performing registration
OutDir=.\ReleaseUMinSize
TargetPath=.\ReleaseUMinSize\hsx.dll
InputPath=.\ReleaseUMinSize\hsx.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	if "%OS%"=="" goto NOTNT 
	if not "%OS%"=="Windows_NT" goto NOTNT 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	goto end 
	:NOTNT 
	echo Warning : Cannot register Unicode DLL on Windows 95 
	:end 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "hsx - Win32 Unicode Release MinDependency"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseUMinDependency"
# PROP BASE Intermediate_Dir "ReleaseUMinDependency"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseUMinDependency"
# PROP Intermediate_Dir "ReleaseUMinDependency"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_UNICODE" /D "_ATL_STATIC_REGISTRY" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_UNICODE" /D "_ATL_STATIC_REGISTRY" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# Begin Custom Build - Performing registration
OutDir=.\ReleaseUMinDependency
TargetPath=.\ReleaseUMinDependency\hsx.dll
InputPath=.\ReleaseUMinDependency\hsx.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	if "%OS%"=="" goto NOTNT 
	if not "%OS%"=="Windows_NT" goto NOTNT 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	goto end 
	:NOTNT 
	echo Warning : Cannot register Unicode DLL on Windows 95 
	:end 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "hsx - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "hsx___Win32_Release"
# PROP BASE Intermediate_Dir "hsx___Win32_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "c:\abinition\hs\hsx" /I "c:\abinition\hs\code" /I "$(MYSQL_HOME)\include\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "AS_ATL" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MD /W3 /Gm /GX /ZI /Od /I "c:\abinition\hs\hsx" /I "c:\abinition\hs\code" /I "$(MYSQL_HOME)\include\\" /I "\\" /I "$(OPENSSL_HOME)\inc32\\" /I "$(JAVA_HOME)\include" /I "$(JAVA_HOME)\include\win32" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "AS_ATL" /D "AS_SSL" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib libmysql.lib ssleay32.lib libeay32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept /libpath:"$(OPENSSL_HOME)\lib" /libpath:"$(MYSQL_HOME)\lib\opt"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib libmysql.lib ssleay32.lib libeay32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept /libpath:"$(OPENSSL_HOME)\out32" /libpath:"$(MYSQL_HOME)\lib\opt"
# Begin Custom Build - Performing registration
OutDir=.\Release
TargetPath=.\Release\hsx.dll
InputPath=.\Release\hsx.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "hsx - Win32 Debug"
# Name "hsx - Win32 Unicode Debug"
# Name "hsx - Win32 Release MinSize"
# Name "hsx - Win32 Release MinDependency"
# Name "hsx - Win32 Unicode Release MinSize"
# Name "hsx - Win32 Unicode Release MinDependency"
# Name "hsx - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\code\aimsg.c
# End Source File
# Begin Source File

SOURCE=..\code\branch.c
# End Source File
# Begin Source File

SOURCE=..\code\cgi.c
# End Source File
# Begin Source File

SOURCE=..\code\channel.c
# End Source File
# Begin Source File

SOURCE=..\code\concept.c
# End Source File
# Begin Source File

SOURCE=..\code\data.c
# End Source File
# Begin Source File

SOURCE=..\code\dateparse.c
# End Source File
# Begin Source File

SOURCE=..\code\env.c
# End Source File
# Begin Source File

SOURCE=..\code\fileio.c
# End Source File
# Begin Source File

SOURCE=..\code\frame.c
# End Source File
# Begin Source File

SOURCE=..\code\function.c
# End Source File
# Begin Source File

SOURCE=..\code\hash.c
# End Source File
# Begin Source File

SOURCE=..\code\hs.c
# End Source File
# Begin Source File

SOURCE=.\HSListBox.cpp
# End Source File
# Begin Source File

SOURCE=..\code\hsms.c
# End Source File
# Begin Source File

SOURCE=.\hsx.cpp
# End Source File
# Begin Source File

SOURCE=.\hsx.def
# End Source File
# Begin Source File

SOURCE=.\hsx.idl
# ADD MTL /tlb ".\hsx.tlb" /h "hsx.h" /iid "hsx_i.c" /Oicf
# End Source File
# Begin Source File

SOURCE=.\hsx.rc
# End Source File
# Begin Source File

SOURCE=..\code\http.c
# End Source File
# Begin Source File

SOURCE=..\code\hyp.c
# End Source File
# Begin Source File

SOURCE=.\HyperScript2.cpp
# End Source File
# Begin Source File

SOURCE=..\code\instance.c
# End Source File
# Begin Source File

SOURCE=.\interface.cpp
# End Source File
# Begin Source File

SOURCE=.\JSCall.cpp
# End Source File
# Begin Source File

SOURCE=..\code\label.c
# End Source File
# Begin Source File

SOURCE=..\code\load.c
# End Source File
# Begin Source File

SOURCE=..\code\memtrack.c
# End Source File
# Begin Source File

SOURCE=..\code\method.c
# End Source File
# Begin Source File

SOURCE=..\code\operand.c
# End Source File
# Begin Source File

SOURCE=..\code\operator.c
# End Source File
# Begin Source File

SOURCE=..\code\parse.c
# End Source File
# Begin Source File

SOURCE=..\code\port.c
# End Source File
# Begin Source File

SOURCE=..\code\promis.c
# End Source File
# Begin Source File

SOURCE=..\code\route.c
# End Source File
# Begin Source File

SOURCE=..\code\router.c
# End Source File
# Begin Source File

SOURCE=..\code\secs.c
# End Source File
# Begin Source File

SOURCE=..\code\secs1.c
# End Source File
# Begin Source File

SOURCE=..\code\secs2.c
# End Source File
# Begin Source File

SOURCE=..\code\signal.c
# End Source File
# Begin Source File

SOURCE=..\code\sock.c
# End Source File
# Begin Source File

SOURCE=..\code\sort.c
# End Source File
# Begin Source File

SOURCE=..\code\sql.c
# End Source File
# Begin Source File

SOURCE=..\code\ssl.c
# End Source File
# Begin Source File

SOURCE=..\code\stack.c
# End Source File
# Begin Source File

SOURCE=..\code\stats.c
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\code\stmt.c
# End Source File
# Begin Source File

SOURCE=..\code\system.c
# End Source File
# Begin Source File

SOURCE=..\code\tcp.c
# End Source File
# Begin Source File

SOURCE=..\code\type.c
# End Source File
# Begin Source File

SOURCE=..\code\util.c
# End Source File
# Begin Source File

SOURCE=.\utilx.cpp
# End Source File
# Begin Source File

SOURCE=.\WebPickle.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\code\aimsg.h
# End Source File
# Begin Source File

SOURCE=.\atlcontrols.h
# End Source File
# Begin Source File

SOURCE=..\code\auto.h
# End Source File
# Begin Source File

SOURCE=..\code\branch.h
# End Source File
# Begin Source File

SOURCE=..\code\cgi.h
# End Source File
# Begin Source File

SOURCE=..\code\channel.h
# End Source File
# Begin Source File

SOURCE=..\code\concept.h
# End Source File
# Begin Source File

SOURCE=..\code\data.h
# End Source File
# Begin Source File

SOURCE=..\code\dateparse.h
# End Source File
# Begin Source File

SOURCE=..\code\env.h
# End Source File
# Begin Source File

SOURCE=..\code\fileio.h
# End Source File
# Begin Source File

SOURCE=..\code\frame.h
# End Source File
# Begin Source File

SOURCE=..\code\function.h
# End Source File
# Begin Source File

SOURCE=..\code\hash.h
# End Source File
# Begin Source File

SOURCE=.\HSListBox.h
# End Source File
# Begin Source File

SOURCE=..\code\hsms.h
# End Source File
# Begin Source File

SOURCE=..\code\http.h
# End Source File
# Begin Source File

SOURCE=..\code\hyp.h
# End Source File
# Begin Source File

SOURCE=..\code\hypdef.h
# End Source File
# Begin Source File

SOURCE=.\HyperScript2.h
# End Source File
# Begin Source File

SOURCE=..\code\instance.h
# End Source File
# Begin Source File

SOURCE=.\interface.h
# End Source File
# Begin Source File

SOURCE=.\JSCall.h
# End Source File
# Begin Source File

SOURCE=..\code\label.h
# End Source File
# Begin Source File

SOURCE=..\code\load.h
# End Source File
# Begin Source File

SOURCE=..\code\mem.h
# End Source File
# Begin Source File

SOURCE=..\code\memtrack.h
# End Source File
# Begin Source File

SOURCE=..\code\method.h
# End Source File
# Begin Source File

SOURCE=..\code\operand.h
# End Source File
# Begin Source File

SOURCE=..\code\operator.h
# End Source File
# Begin Source File

SOURCE=..\code\parse.h
# End Source File
# Begin Source File

SOURCE=..\code\port.h
# End Source File
# Begin Source File

SOURCE=..\code\promis.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=..\code\route.h
# End Source File
# Begin Source File

SOURCE=..\code\router.h
# End Source File
# Begin Source File

SOURCE=..\code\secs.h
# End Source File
# Begin Source File

SOURCE=..\code\secs1.h
# End Source File
# Begin Source File

SOURCE=..\code\secs2.h
# End Source File
# Begin Source File

SOURCE=..\code\secshead.h
# End Source File
# Begin Source File

SOURCE=..\code\signl.h
# End Source File
# Begin Source File

SOURCE=..\code\sock.h
# End Source File
# Begin Source File

SOURCE=..\code\sort.h
# End Source File
# Begin Source File

SOURCE=..\code\sql.h
# End Source File
# Begin Source File

SOURCE=..\code\ssl.h
# End Source File
# Begin Source File

SOURCE=..\code\stack.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\code\stmt.h
# End Source File
# Begin Source File

SOURCE=.\stringArray.h
# End Source File
# Begin Source File

SOURCE=..\code\system.h
# End Source File
# Begin Source File

SOURCE=..\code\tcp.h
# End Source File
# Begin Source File

SOURCE=..\code\typdef.h
# End Source File
# Begin Source File

SOURCE=..\code\type.h
# End Source File
# Begin Source File

SOURCE=..\code\util.h
# End Source File
# Begin Source File

SOURCE=.\utilx.h
# End Source File
# Begin Source File

SOURCE=.\WebPickle.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\HyperScript2.rgs
# End Source File
# Begin Source File

SOURCE=.\WebPickle.rgs
# End Source File
# End Group
# Begin Source File

SOURCE=.\TestIFrame.htm
# End Source File
# Begin Source File

SOURCE=.\TestWebPickle.htm
# End Source File
# Begin Source File

SOURCE=.\WebPickleUI.htm
# End Source File
# End Target
# End Project
# Section hsx : {00000000-0000-0000-0000-800000800000}
# 	1:13:IDD_HSLISTBOX:105
# End Section
