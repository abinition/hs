# Microsoft Developer Studio Generated NMAKE File, Based on hs.dsp
!IF "$(CFG)" == ""
CFG=hs - Win32 Debug
!MESSAGE No configuration specified. Defaulting to hs - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "hs - Win32 Release" && "$(CFG)" != "hs - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "hs - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\hs.exe" "$(OUTDIR)\hs.bsc"


CLEAN :
	-@erase "$(INTDIR)\aimsg.obj"
	-@erase "$(INTDIR)\aimsg.sbr"
	-@erase "$(INTDIR)\branch.obj"
	-@erase "$(INTDIR)\branch.sbr"
	-@erase "$(INTDIR)\cgi.obj"
	-@erase "$(INTDIR)\cgi.sbr"
	-@erase "$(INTDIR)\channel.obj"
	-@erase "$(INTDIR)\channel.sbr"
	-@erase "$(INTDIR)\concept.obj"
	-@erase "$(INTDIR)\concept.sbr"
	-@erase "$(INTDIR)\data.obj"
	-@erase "$(INTDIR)\data.sbr"
	-@erase "$(INTDIR)\dateparse.obj"
	-@erase "$(INTDIR)\dateparse.sbr"
	-@erase "$(INTDIR)\env.obj"
	-@erase "$(INTDIR)\env.sbr"
	-@erase "$(INTDIR)\fileio.obj"
	-@erase "$(INTDIR)\fileio.sbr"
	-@erase "$(INTDIR)\frame.obj"
	-@erase "$(INTDIR)\frame.sbr"
	-@erase "$(INTDIR)\function.obj"
	-@erase "$(INTDIR)\function.sbr"
	-@erase "$(INTDIR)\hash.obj"
	-@erase "$(INTDIR)\hash.sbr"
	-@erase "$(INTDIR)\hs.obj"
	-@erase "$(INTDIR)\hs.sbr"
	-@erase "$(INTDIR)\hsms.obj"
	-@erase "$(INTDIR)\hsms.sbr"
	-@erase "$(INTDIR)\http.obj"
	-@erase "$(INTDIR)\http.sbr"
	-@erase "$(INTDIR)\hyp.obj"
	-@erase "$(INTDIR)\hyp.sbr"
	-@erase "$(INTDIR)\instance.obj"
	-@erase "$(INTDIR)\instance.sbr"
	-@erase "$(INTDIR)\label.obj"
	-@erase "$(INTDIR)\label.sbr"
	-@erase "$(INTDIR)\load.obj"
	-@erase "$(INTDIR)\load.sbr"
	-@erase "$(INTDIR)\mapi.obj"
	-@erase "$(INTDIR)\mapi.sbr"
	-@erase "$(INTDIR)\memtrack.obj"
	-@erase "$(INTDIR)\memtrack.sbr"
	-@erase "$(INTDIR)\method.obj"
	-@erase "$(INTDIR)\method.sbr"
	-@erase "$(INTDIR)\operand.obj"
	-@erase "$(INTDIR)\operand.sbr"
	-@erase "$(INTDIR)\operator.obj"
	-@erase "$(INTDIR)\operator.sbr"
	-@erase "$(INTDIR)\parse.obj"
	-@erase "$(INTDIR)\parse.sbr"
	-@erase "$(INTDIR)\port.obj"
	-@erase "$(INTDIR)\port.sbr"
	-@erase "$(INTDIR)\promis.obj"
	-@erase "$(INTDIR)\promis.sbr"
	-@erase "$(INTDIR)\route.obj"
	-@erase "$(INTDIR)\route.sbr"
	-@erase "$(INTDIR)\router.obj"
	-@erase "$(INTDIR)\router.sbr"
	-@erase "$(INTDIR)\secs.obj"
	-@erase "$(INTDIR)\secs.sbr"
	-@erase "$(INTDIR)\secs1.obj"
	-@erase "$(INTDIR)\secs1.sbr"
	-@erase "$(INTDIR)\secs2.obj"
	-@erase "$(INTDIR)\secs2.sbr"
	-@erase "$(INTDIR)\signal.obj"
	-@erase "$(INTDIR)\signal.sbr"
	-@erase "$(INTDIR)\sock.obj"
	-@erase "$(INTDIR)\sock.sbr"
	-@erase "$(INTDIR)\sort.obj"
	-@erase "$(INTDIR)\sort.sbr"
	-@erase "$(INTDIR)\sql.obj"
	-@erase "$(INTDIR)\sql.sbr"
	-@erase "$(INTDIR)\ssl.obj"
	-@erase "$(INTDIR)\ssl.sbr"
	-@erase "$(INTDIR)\stack.obj"
	-@erase "$(INTDIR)\stack.sbr"
	-@erase "$(INTDIR)\stmt.obj"
	-@erase "$(INTDIR)\stmt.sbr"
	-@erase "$(INTDIR)\system.obj"
	-@erase "$(INTDIR)\system.sbr"
	-@erase "$(INTDIR)\tcp.obj"
	-@erase "$(INTDIR)\tcp.sbr"
	-@erase "$(INTDIR)\type.obj"
	-@erase "$(INTDIR)\type.sbr"
	-@erase "$(INTDIR)\util.obj"
	-@erase "$(INTDIR)\util.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\hs.bsc"
	-@erase "$(OUTDIR)\hs.exe"
	-@erase "$(OUTDIR)\hs.map"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /I "$(MYSQL_HOME)\include\\" /I "$(OPENSSL_HOME)\include\\" /I "$(JAVA_HOME)\include" /I "$(JAVA_HOME)\include\win32" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "AS_SQL" /D "AS_MYSQL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\hs.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\hs.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\aimsg.sbr" \
	"$(INTDIR)\branch.sbr" \
	"$(INTDIR)\cgi.sbr" \
	"$(INTDIR)\channel.sbr" \
	"$(INTDIR)\concept.sbr" \
	"$(INTDIR)\data.sbr" \
	"$(INTDIR)\dateparse.sbr" \
	"$(INTDIR)\env.sbr" \
	"$(INTDIR)\fileio.sbr" \
	"$(INTDIR)\frame.sbr" \
	"$(INTDIR)\function.sbr" \
	"$(INTDIR)\hash.sbr" \
	"$(INTDIR)\hs.sbr" \
	"$(INTDIR)\hsms.sbr" \
	"$(INTDIR)\http.sbr" \
	"$(INTDIR)\hyp.sbr" \
	"$(INTDIR)\instance.sbr" \
	"$(INTDIR)\label.sbr" \
	"$(INTDIR)\load.sbr" \
	"$(INTDIR)\mapi.sbr" \
	"$(INTDIR)\memtrack.sbr" \
	"$(INTDIR)\method.sbr" \
	"$(INTDIR)\operand.sbr" \
	"$(INTDIR)\operator.sbr" \
	"$(INTDIR)\parse.sbr" \
	"$(INTDIR)\port.sbr" \
	"$(INTDIR)\promis.sbr" \
	"$(INTDIR)\route.sbr" \
	"$(INTDIR)\router.sbr" \
	"$(INTDIR)\secs.sbr" \
	"$(INTDIR)\secs1.sbr" \
	"$(INTDIR)\secs2.sbr" \
	"$(INTDIR)\signal.sbr" \
	"$(INTDIR)\sock.sbr" \
	"$(INTDIR)\sort.sbr" \
	"$(INTDIR)\sql.sbr" \
	"$(INTDIR)\ssl.sbr" \
	"$(INTDIR)\stack.sbr" \
	"$(INTDIR)\stmt.sbr" \
	"$(INTDIR)\system.sbr" \
	"$(INTDIR)\tcp.sbr" \
	"$(INTDIR)\type.sbr" \
	"$(INTDIR)\util.sbr"

"$(OUTDIR)\hs.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib libmysql.lib ntwdblib.lib ssleay32.lib libeay32.lib libmysql.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\hs.pdb" /map:"$(INTDIR)\hs.map" /machine:I386 /out:"$(OUTDIR)\hs.exe" /libpath:"$(OPENSSL_HOME)\lib" /libpath:"$(MYSQL_HOME)\lib\opt" 
LINK32_OBJS= \
	"$(INTDIR)\aimsg.obj" \
	"$(INTDIR)\branch.obj" \
	"$(INTDIR)\cgi.obj" \
	"$(INTDIR)\channel.obj" \
	"$(INTDIR)\concept.obj" \
	"$(INTDIR)\data.obj" \
	"$(INTDIR)\dateparse.obj" \
	"$(INTDIR)\env.obj" \
	"$(INTDIR)\fileio.obj" \
	"$(INTDIR)\frame.obj" \
	"$(INTDIR)\function.obj" \
	"$(INTDIR)\hash.obj" \
	"$(INTDIR)\hs.obj" \
	"$(INTDIR)\hsms.obj" \
	"$(INTDIR)\http.obj" \
	"$(INTDIR)\hyp.obj" \
	"$(INTDIR)\instance.obj" \
	"$(INTDIR)\label.obj" \
	"$(INTDIR)\load.obj" \
	"$(INTDIR)\mapi.obj" \
	"$(INTDIR)\memtrack.obj" \
	"$(INTDIR)\method.obj" \
	"$(INTDIR)\operand.obj" \
	"$(INTDIR)\operator.obj" \
	"$(INTDIR)\parse.obj" \
	"$(INTDIR)\port.obj" \
	"$(INTDIR)\promis.obj" \
	"$(INTDIR)\route.obj" \
	"$(INTDIR)\router.obj" \
	"$(INTDIR)\secs.obj" \
	"$(INTDIR)\secs1.obj" \
	"$(INTDIR)\secs2.obj" \
	"$(INTDIR)\signal.obj" \
	"$(INTDIR)\sock.obj" \
	"$(INTDIR)\sort.obj" \
	"$(INTDIR)\sql.obj" \
	"$(INTDIR)\ssl.obj" \
	"$(INTDIR)\stack.obj" \
	"$(INTDIR)\stmt.obj" \
	"$(INTDIR)\system.obj" \
	"$(INTDIR)\tcp.obj" \
	"$(INTDIR)\type.obj" \
	"$(INTDIR)\util.obj"

"$(OUTDIR)\hs.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "hs - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\hs.exe" "$(OUTDIR)\hs.bsc"


CLEAN :
	-@erase "$(INTDIR)\aimsg.obj"
	-@erase "$(INTDIR)\aimsg.sbr"
	-@erase "$(INTDIR)\branch.obj"
	-@erase "$(INTDIR)\branch.sbr"
	-@erase "$(INTDIR)\cgi.obj"
	-@erase "$(INTDIR)\cgi.sbr"
	-@erase "$(INTDIR)\channel.obj"
	-@erase "$(INTDIR)\channel.sbr"
	-@erase "$(INTDIR)\concept.obj"
	-@erase "$(INTDIR)\concept.sbr"
	-@erase "$(INTDIR)\data.obj"
	-@erase "$(INTDIR)\data.sbr"
	-@erase "$(INTDIR)\dateparse.obj"
	-@erase "$(INTDIR)\dateparse.sbr"
	-@erase "$(INTDIR)\env.obj"
	-@erase "$(INTDIR)\env.sbr"
	-@erase "$(INTDIR)\fileio.obj"
	-@erase "$(INTDIR)\fileio.sbr"
	-@erase "$(INTDIR)\frame.obj"
	-@erase "$(INTDIR)\frame.sbr"
	-@erase "$(INTDIR)\function.obj"
	-@erase "$(INTDIR)\function.sbr"
	-@erase "$(INTDIR)\hash.obj"
	-@erase "$(INTDIR)\hash.sbr"
	-@erase "$(INTDIR)\hs.obj"
	-@erase "$(INTDIR)\hs.sbr"
	-@erase "$(INTDIR)\hsms.obj"
	-@erase "$(INTDIR)\hsms.sbr"
	-@erase "$(INTDIR)\http.obj"
	-@erase "$(INTDIR)\http.sbr"
	-@erase "$(INTDIR)\hyp.obj"
	-@erase "$(INTDIR)\hyp.sbr"
	-@erase "$(INTDIR)\instance.obj"
	-@erase "$(INTDIR)\instance.sbr"
	-@erase "$(INTDIR)\label.obj"
	-@erase "$(INTDIR)\label.sbr"
	-@erase "$(INTDIR)\load.obj"
	-@erase "$(INTDIR)\load.sbr"
	-@erase "$(INTDIR)\mapi.obj"
	-@erase "$(INTDIR)\mapi.sbr"
	-@erase "$(INTDIR)\memtrack.obj"
	-@erase "$(INTDIR)\memtrack.sbr"
	-@erase "$(INTDIR)\method.obj"
	-@erase "$(INTDIR)\method.sbr"
	-@erase "$(INTDIR)\operand.obj"
	-@erase "$(INTDIR)\operand.sbr"
	-@erase "$(INTDIR)\operator.obj"
	-@erase "$(INTDIR)\operator.sbr"
	-@erase "$(INTDIR)\parse.obj"
	-@erase "$(INTDIR)\parse.sbr"
	-@erase "$(INTDIR)\port.obj"
	-@erase "$(INTDIR)\port.sbr"
	-@erase "$(INTDIR)\promis.obj"
	-@erase "$(INTDIR)\promis.sbr"
	-@erase "$(INTDIR)\route.obj"
	-@erase "$(INTDIR)\route.sbr"
	-@erase "$(INTDIR)\router.obj"
	-@erase "$(INTDIR)\router.sbr"
	-@erase "$(INTDIR)\secs.obj"
	-@erase "$(INTDIR)\secs.sbr"
	-@erase "$(INTDIR)\secs1.obj"
	-@erase "$(INTDIR)\secs1.sbr"
	-@erase "$(INTDIR)\secs2.obj"
	-@erase "$(INTDIR)\secs2.sbr"
	-@erase "$(INTDIR)\signal.obj"
	-@erase "$(INTDIR)\signal.sbr"
	-@erase "$(INTDIR)\sock.obj"
	-@erase "$(INTDIR)\sock.sbr"
	-@erase "$(INTDIR)\sort.obj"
	-@erase "$(INTDIR)\sort.sbr"
	-@erase "$(INTDIR)\sql.obj"
	-@erase "$(INTDIR)\sql.sbr"
	-@erase "$(INTDIR)\ssl.obj"
	-@erase "$(INTDIR)\ssl.sbr"
	-@erase "$(INTDIR)\stack.obj"
	-@erase "$(INTDIR)\stack.sbr"
	-@erase "$(INTDIR)\stmt.obj"
	-@erase "$(INTDIR)\stmt.sbr"
	-@erase "$(INTDIR)\system.obj"
	-@erase "$(INTDIR)\system.sbr"
	-@erase "$(INTDIR)\tcp.obj"
	-@erase "$(INTDIR)\tcp.sbr"
	-@erase "$(INTDIR)\type.obj"
	-@erase "$(INTDIR)\type.sbr"
	-@erase "$(INTDIR)\util.obj"
	-@erase "$(INTDIR)\util.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\hs.bsc"
	-@erase "$(OUTDIR)\hs.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /Gm /GX /ZI /Od /I "$(MYSQL_HOME)\include\\" /I "$(OPENSSL_HOME)\include\\" /I "$(JAVA_HOME)\include" /I "$(JAVA_HOME)\include\win32" /I "C:\Program Files\Microsoft SDK\include" /D "WIN32" /D "_DEBUG" /D "_USRDLL" /D "_MBCS" /D "AS_SQL" /D "AS_MYSQL" /D "AS_SSL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\hs.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\hs.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\aimsg.sbr" \
	"$(INTDIR)\branch.sbr" \
	"$(INTDIR)\cgi.sbr" \
	"$(INTDIR)\channel.sbr" \
	"$(INTDIR)\concept.sbr" \
	"$(INTDIR)\data.sbr" \
	"$(INTDIR)\dateparse.sbr" \
	"$(INTDIR)\env.sbr" \
	"$(INTDIR)\fileio.sbr" \
	"$(INTDIR)\frame.sbr" \
	"$(INTDIR)\function.sbr" \
	"$(INTDIR)\hash.sbr" \
	"$(INTDIR)\hs.sbr" \
	"$(INTDIR)\hsms.sbr" \
	"$(INTDIR)\http.sbr" \
	"$(INTDIR)\hyp.sbr" \
	"$(INTDIR)\instance.sbr" \
	"$(INTDIR)\label.sbr" \
	"$(INTDIR)\load.sbr" \
	"$(INTDIR)\mapi.sbr" \
	"$(INTDIR)\memtrack.sbr" \
	"$(INTDIR)\method.sbr" \
	"$(INTDIR)\operand.sbr" \
	"$(INTDIR)\operator.sbr" \
	"$(INTDIR)\parse.sbr" \
	"$(INTDIR)\port.sbr" \
	"$(INTDIR)\promis.sbr" \
	"$(INTDIR)\route.sbr" \
	"$(INTDIR)\router.sbr" \
	"$(INTDIR)\secs.sbr" \
	"$(INTDIR)\secs1.sbr" \
	"$(INTDIR)\secs2.sbr" \
	"$(INTDIR)\signal.sbr" \
	"$(INTDIR)\sock.sbr" \
	"$(INTDIR)\sort.sbr" \
	"$(INTDIR)\sql.sbr" \
	"$(INTDIR)\ssl.sbr" \
	"$(INTDIR)\stack.sbr" \
	"$(INTDIR)\stmt.sbr" \
	"$(INTDIR)\system.sbr" \
	"$(INTDIR)\tcp.sbr" \
	"$(INTDIR)\type.sbr" \
	"$(INTDIR)\util.sbr"

"$(OUTDIR)\hs.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib libmysql.lib ntwdblib.lib EXCHINST.LIB ssleay32.lib libeay32.lib Rpcns4.lib rpcrt4.lib Mpr.lib /nologo /subsystem:console /profile /debug /machine:I386 /out:"$(OUTDIR)\hs.exe" /libpath:"$(OPENSSL_HOME)\lib" /libpath:"$(MYSQL_HOME)\lib\opt" 
LINK32_OBJS= \
	"$(INTDIR)\aimsg.obj" \
	"$(INTDIR)\branch.obj" \
	"$(INTDIR)\cgi.obj" \
	"$(INTDIR)\channel.obj" \
	"$(INTDIR)\concept.obj" \
	"$(INTDIR)\data.obj" \
	"$(INTDIR)\dateparse.obj" \
	"$(INTDIR)\env.obj" \
	"$(INTDIR)\fileio.obj" \
	"$(INTDIR)\frame.obj" \
	"$(INTDIR)\function.obj" \
	"$(INTDIR)\hash.obj" \
	"$(INTDIR)\hs.obj" \
	"$(INTDIR)\hsms.obj" \
	"$(INTDIR)\http.obj" \
	"$(INTDIR)\hyp.obj" \
	"$(INTDIR)\instance.obj" \
	"$(INTDIR)\label.obj" \
	"$(INTDIR)\load.obj" \
	"$(INTDIR)\mapi.obj" \
	"$(INTDIR)\memtrack.obj" \
	"$(INTDIR)\method.obj" \
	"$(INTDIR)\operand.obj" \
	"$(INTDIR)\operator.obj" \
	"$(INTDIR)\parse.obj" \
	"$(INTDIR)\port.obj" \
	"$(INTDIR)\promis.obj" \
	"$(INTDIR)\route.obj" \
	"$(INTDIR)\router.obj" \
	"$(INTDIR)\secs.obj" \
	"$(INTDIR)\secs1.obj" \
	"$(INTDIR)\secs2.obj" \
	"$(INTDIR)\signal.obj" \
	"$(INTDIR)\sock.obj" \
	"$(INTDIR)\sort.obj" \
	"$(INTDIR)\sql.obj" \
	"$(INTDIR)\ssl.obj" \
	"$(INTDIR)\stack.obj" \
	"$(INTDIR)\stmt.obj" \
	"$(INTDIR)\system.obj" \
	"$(INTDIR)\tcp.obj" \
	"$(INTDIR)\type.obj" \
	"$(INTDIR)\util.obj"

"$(OUTDIR)\hs.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("hs.dep")
!INCLUDE "hs.dep"
!ELSE 
!MESSAGE Warning: cannot find "hs.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "hs - Win32 Release" || "$(CFG)" == "hs - Win32 Debug"
SOURCE=.\aimsg.c

"$(INTDIR)\aimsg.obj"	"$(INTDIR)\aimsg.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\branch.c

"$(INTDIR)\branch.obj"	"$(INTDIR)\branch.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\cgi.c

"$(INTDIR)\cgi.obj"	"$(INTDIR)\cgi.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\channel.c

"$(INTDIR)\channel.obj"	"$(INTDIR)\channel.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\concept.c

"$(INTDIR)\concept.obj"	"$(INTDIR)\concept.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\data.c

"$(INTDIR)\data.obj"	"$(INTDIR)\data.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\dateparse.c

"$(INTDIR)\dateparse.obj"	"$(INTDIR)\dateparse.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\env.c

"$(INTDIR)\env.obj"	"$(INTDIR)\env.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\fileio.c

"$(INTDIR)\fileio.obj"	"$(INTDIR)\fileio.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\frame.c

"$(INTDIR)\frame.obj"	"$(INTDIR)\frame.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\function.c

"$(INTDIR)\function.obj"	"$(INTDIR)\function.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\hash.c

"$(INTDIR)\hash.obj"	"$(INTDIR)\hash.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\hs.c

"$(INTDIR)\hs.obj"	"$(INTDIR)\hs.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\hsms.c

"$(INTDIR)\hsms.obj"	"$(INTDIR)\hsms.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\http.c

"$(INTDIR)\http.obj"	"$(INTDIR)\http.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\hyp.c

"$(INTDIR)\hyp.obj"	"$(INTDIR)\hyp.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\instance.c

"$(INTDIR)\instance.obj"	"$(INTDIR)\instance.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\label.c

"$(INTDIR)\label.obj"	"$(INTDIR)\label.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\load.c

"$(INTDIR)\load.obj"	"$(INTDIR)\load.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\mapi.c

"$(INTDIR)\mapi.obj"	"$(INTDIR)\mapi.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\memtrack.c

"$(INTDIR)\memtrack.obj"	"$(INTDIR)\memtrack.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\method.c

"$(INTDIR)\method.obj"	"$(INTDIR)\method.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\operand.c

"$(INTDIR)\operand.obj"	"$(INTDIR)\operand.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\operator.c

"$(INTDIR)\operator.obj"	"$(INTDIR)\operator.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\parse.c

"$(INTDIR)\parse.obj"	"$(INTDIR)\parse.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\port.c

"$(INTDIR)\port.obj"	"$(INTDIR)\port.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\promis.c

"$(INTDIR)\promis.obj"	"$(INTDIR)\promis.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\route.c

"$(INTDIR)\route.obj"	"$(INTDIR)\route.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\router.c

"$(INTDIR)\router.obj"	"$(INTDIR)\router.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\secs.c

"$(INTDIR)\secs.obj"	"$(INTDIR)\secs.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\secs1.c

"$(INTDIR)\secs1.obj"	"$(INTDIR)\secs1.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\secs2.c

"$(INTDIR)\secs2.obj"	"$(INTDIR)\secs2.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\signal.c

"$(INTDIR)\signal.obj"	"$(INTDIR)\signal.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\sock.c

"$(INTDIR)\sock.obj"	"$(INTDIR)\sock.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\sort.c

"$(INTDIR)\sort.obj"	"$(INTDIR)\sort.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\sql.c

"$(INTDIR)\sql.obj"	"$(INTDIR)\sql.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ssl.c

"$(INTDIR)\ssl.obj"	"$(INTDIR)\ssl.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\stack.c

"$(INTDIR)\stack.obj"	"$(INTDIR)\stack.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\stmt.c

"$(INTDIR)\stmt.obj"	"$(INTDIR)\stmt.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\system.c

"$(INTDIR)\system.obj"	"$(INTDIR)\system.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\tcp.c

"$(INTDIR)\tcp.obj"	"$(INTDIR)\tcp.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\type.c

"$(INTDIR)\type.obj"	"$(INTDIR)\type.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\util.c

"$(INTDIR)\util.obj"	"$(INTDIR)\util.sbr" : $(SOURCE) "$(INTDIR)"



!ENDIF 

