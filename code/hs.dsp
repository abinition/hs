# Microsoft Developer Studio Project File - Name="hs" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=hs - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "hs.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "hs.mak" CFG="hs - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "hs - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "hs - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "hs - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /I "c:\mysql\include\\" /I "C:\openssl\openssl-0.9.7c\inc32" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "AS_SSL" /D "AS_SQL" /D "AS_MYSQL" /FR /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib libmysql.lib ntwdblib.lib ssleay32.lib libeay32.lib libmysql.lib /nologo /subsystem:console /map /machine:I386 /libpath:"c:\mysql\lib\opt" /libpath:"C:\openssl\openssl-0.9.7c\out32"

!ELSEIF  "$(CFG)" == "hs - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MD /W3 /Gm /GX /ZI /Od /I "c:\mysql\include\\" /I "C:\openssl\openssl-0.9.7c\include" /I "c:\j2sdk1.4.2_01\include" /I "c:\j2sdk1.4.2_01\include\win32" /D "WIN32" /D "_DEBUG" /D "_USRDLL" /D "_MBCS" /D "AS_SQL" /D "AS_MYSQL" /D "AS_MEMTRACK" /D "AS_SSL" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib libmysql.lib ntwdblib.lib EXCHINST.LIB ssleay32.lib libeay32.lib /nologo /subsystem:console /debug /machine:I386 /libpath:"C:\openssl\openssl-0.9.7c\lib" /libpath:"c:\mysql\lib\opt"
# SUBTRACT LINK32 /profile /map

!ENDIF 

# Begin Target

# Name "hs - Win32 Release"
# Name "hs - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\aimsg.c
# End Source File
# Begin Source File

SOURCE=.\branch.c
# End Source File
# Begin Source File

SOURCE=.\cgi.c
# End Source File
# Begin Source File

SOURCE=.\channel.c
# End Source File
# Begin Source File

SOURCE=.\concept.c
# End Source File
# Begin Source File

SOURCE=.\data.c
# End Source File
# Begin Source File

SOURCE=.\dateparse.c
# End Source File
# Begin Source File

SOURCE=.\env.c
# End Source File
# Begin Source File

SOURCE=.\fileio.c
# End Source File
# Begin Source File

SOURCE=.\frame.c
# End Source File
# Begin Source File

SOURCE=.\function.c
# End Source File
# Begin Source File

SOURCE=.\hash.c
# End Source File
# Begin Source File

SOURCE=.\hs.c
# End Source File
# Begin Source File

SOURCE=.\hsms.c
# End Source File
# Begin Source File

SOURCE=.\http.c
# End Source File
# Begin Source File

SOURCE=.\hyp.c
# End Source File
# Begin Source File

SOURCE=.\instance.c
# End Source File
# Begin Source File

SOURCE=.\label.c
# End Source File
# Begin Source File

SOURCE=.\load.c
# End Source File
# Begin Source File

SOURCE=.\memtrack.c
# End Source File
# Begin Source File

SOURCE=.\method.c
# End Source File
# Begin Source File

SOURCE=.\operand.c
# End Source File
# Begin Source File

SOURCE=.\operator.c
# End Source File
# Begin Source File

SOURCE=.\parse.c
# End Source File
# Begin Source File

SOURCE=.\port.c
# End Source File
# Begin Source File

SOURCE=.\promis.c
# End Source File
# Begin Source File

SOURCE=.\route.c
# End Source File
# Begin Source File

SOURCE=.\router.c
# End Source File
# Begin Source File

SOURCE=.\secs.c
# End Source File
# Begin Source File

SOURCE=.\secs1.c
# End Source File
# Begin Source File

SOURCE=.\secs2.c
# End Source File
# Begin Source File

SOURCE=.\signal.c
# End Source File
# Begin Source File

SOURCE=.\sock.c
# End Source File
# Begin Source File

SOURCE=.\sort.c
# End Source File
# Begin Source File

SOURCE=.\sql.c
# End Source File
# Begin Source File

SOURCE=.\ssl.c
# End Source File
# Begin Source File

SOURCE=.\stack.c
# End Source File
# Begin Source File

SOURCE=.\stmt.c
# End Source File
# Begin Source File

SOURCE=.\system.c
# End Source File
# Begin Source File

SOURCE=.\tcp.c
# End Source File
# Begin Source File

SOURCE=.\type.c
# End Source File
# Begin Source File

SOURCE=.\util.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Abinition.h
# End Source File
# Begin Source File

SOURCE=.\aimsg.h
# End Source File
# Begin Source File

SOURCE=.\auto.h
# End Source File
# Begin Source File

SOURCE=.\branch.h
# End Source File
# Begin Source File

SOURCE=.\cgi.h
# End Source File
# Begin Source File

SOURCE=.\channel.h
# End Source File
# Begin Source File

SOURCE=.\concept.h
# End Source File
# Begin Source File

SOURCE=.\data.h
# End Source File
# Begin Source File

SOURCE=.\dateparse.h
# End Source File
# Begin Source File

SOURCE=.\env.h
# End Source File
# Begin Source File

SOURCE=.\fileio.h
# End Source File
# Begin Source File

SOURCE=.\frame.h
# End Source File
# Begin Source File

SOURCE=.\function.h
# End Source File
# Begin Source File

SOURCE=.\hash.h
# End Source File
# Begin Source File

SOURCE=.\hsms.h
# End Source File
# Begin Source File

SOURCE=.\http.h
# End Source File
# Begin Source File

SOURCE=.\hyp.h
# End Source File
# Begin Source File

SOURCE=.\hypdef.h
# End Source File
# Begin Source File

SOURCE=.\instance.h
# End Source File
# Begin Source File

SOURCE=.\label.h
# End Source File
# Begin Source File

SOURCE=.\load.h
# End Source File
# Begin Source File

SOURCE=.\memtrack.h
# End Source File
# Begin Source File

SOURCE=.\method.h
# End Source File
# Begin Source File

SOURCE=.\operand.h
# End Source File
# Begin Source File

SOURCE=.\operator.h
# End Source File
# Begin Source File

SOURCE=.\parse.h
# End Source File
# Begin Source File

SOURCE=.\port.h
# End Source File
# Begin Source File

SOURCE=.\promis.h
# End Source File
# Begin Source File

SOURCE=.\route.h
# End Source File
# Begin Source File

SOURCE=.\router.h
# End Source File
# Begin Source File

SOURCE=.\secs.h
# End Source File
# Begin Source File

SOURCE=.\secs1.h
# End Source File
# Begin Source File

SOURCE=.\secs2.h
# End Source File
# Begin Source File

SOURCE=.\secshead.h
# End Source File
# Begin Source File

SOURCE=.\signl.h
# End Source File
# Begin Source File

SOURCE=.\sock.h
# End Source File
# Begin Source File

SOURCE=.\sort.h
# End Source File
# Begin Source File

SOURCE=.\sql.h
# End Source File
# Begin Source File

SOURCE=.\ssl.h
# End Source File
# Begin Source File

SOURCE=.\stack.h
# End Source File
# Begin Source File

SOURCE=.\stmt.h
# End Source File
# Begin Source File

SOURCE=.\system.h
# End Source File
# Begin Source File

SOURCE=.\tcp.h
# End Source File
# Begin Source File

SOURCE=.\typdef.h
# End Source File
# Begin Source File

SOURCE=.\type.h
# End Source File
# Begin Source File

SOURCE=.\util.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\sqlold.ccc
# End Source File
# End Target
# End Project
