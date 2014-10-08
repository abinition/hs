# Microsoft Developer Studio Project File - Name="hsdll" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=hsdll - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "hsdll.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "hsdll.mak" CFG="hsdll - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "hsdll - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "hsdll - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "hsdll - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "HSDLL_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /I "$(MYSQL_HOME)\include\\" /I "$(OPENSSL_HOME)\include\\" /I "$(JAVA_HOME)\include" /I "$(JAVA_HOME)\include\win32" /I "..\hsdll" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "HSDLL_EXPORTS" /D "AS_DLL" /D "AS_JNI" /D "AS_SSL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib ntwdblib.lib EXCHINST.LIB ssleay32.lib libeay32.lib /nologo /dll /machine:I386 /libpath:"$(OPENSSL_HOME)\lib" /libpath:"$(MYSQL_HOME)\lib\opt"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "hsdll - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "HSDLL_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "c:\abinition\hs\hsdll" /I "$(MYSQL_HOME)\include\\" /I "$(OPENSSL_HOME)\include\\" /I "$(JAVA_HOME)\include" /I "$(JAVA_HOME)\include\win32" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "HSDLL_EXPORTS" /D "AS_DLL" /D "AS_JNI" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib libmysql.lib ntwdblib.lib EXCHINST.LIB ssleay32.lib libeay32.lib /nologo /dll /map /debug /machine:I386 /pdbtype:sept /libpath:"$(OPENSSL_HOME)\out32" /libpath:"$(MYSQL_HOME)\lib\opt"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "hsdll - Win32 Release"
# Name "hsdll - Win32 Debug"
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

SOURCE=..\code\hsms.c
# End Source File
# Begin Source File

SOURCE=..\code\http.c
# End Source File
# Begin Source File

SOURCE=..\code\hyp.c
# End Source File
# Begin Source File

SOURCE=..\code\instance.c
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

SOURCE=..\code\memtrkmn.c
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
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\code\aimsg.h
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

SOURCE=..\code\resource.h
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

SOURCE=..\code\stmt.h
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
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
