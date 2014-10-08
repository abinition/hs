# Microsoft Developer Studio Generated NMAKE File, Based on hsdll.dsp
!IF "$(CFG)" == ""
CFG=hsdll - Win32 Debug
!MESSAGE No configuration specified. Defaulting to hsdll - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "hsdll - Win32 Release" && "$(CFG)" != "hsdll - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "hsdll - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\hsdll.dll"


CLEAN :
	-@erase "$(INTDIR)\aimsg.obj"
	-@erase "$(INTDIR)\branch.obj"
	-@erase "$(INTDIR)\cgi.obj"
	-@erase "$(INTDIR)\channel.obj"
	-@erase "$(INTDIR)\concept.obj"
	-@erase "$(INTDIR)\data.obj"
	-@erase "$(INTDIR)\dateparse.obj"
	-@erase "$(INTDIR)\env.obj"
	-@erase "$(INTDIR)\fileio.obj"
	-@erase "$(INTDIR)\frame.obj"
	-@erase "$(INTDIR)\function.obj"
	-@erase "$(INTDIR)\hash.obj"
	-@erase "$(INTDIR)\hs.obj"
	-@erase "$(INTDIR)\hsms.obj"
	-@erase "$(INTDIR)\http.obj"
	-@erase "$(INTDIR)\hyp.obj"
	-@erase "$(INTDIR)\instance.obj"
	-@erase "$(INTDIR)\label.obj"
	-@erase "$(INTDIR)\load.obj"
	-@erase "$(INTDIR)\memtrack.obj"
	-@erase "$(INTDIR)\memtrkmn.obj"
	-@erase "$(INTDIR)\method.obj"
	-@erase "$(INTDIR)\operand.obj"
	-@erase "$(INTDIR)\operator.obj"
	-@erase "$(INTDIR)\parse.obj"
	-@erase "$(INTDIR)\port.obj"
	-@erase "$(INTDIR)\promis.obj"
	-@erase "$(INTDIR)\route.obj"
	-@erase "$(INTDIR)\router.obj"
	-@erase "$(INTDIR)\secs.obj"
	-@erase "$(INTDIR)\secs1.obj"
	-@erase "$(INTDIR)\secs2.obj"
	-@erase "$(INTDIR)\signal.obj"
	-@erase "$(INTDIR)\sock.obj"
	-@erase "$(INTDIR)\sort.obj"
	-@erase "$(INTDIR)\sql.obj"
	-@erase "$(INTDIR)\ssl.obj"
	-@erase "$(INTDIR)\stack.obj"
	-@erase "$(INTDIR)\stmt.obj"
	-@erase "$(INTDIR)\system.obj"
	-@erase "$(INTDIR)\tcp.obj"
	-@erase "$(INTDIR)\type.obj"
	-@erase "$(INTDIR)\util.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\hsdll.dll"
	-@erase "$(OUTDIR)\hsdll.exp"
	-@erase "$(OUTDIR)\hsdll.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /I "c:\mysql\include\\" /I "c:\openssl\include\\" /I "E:\j2sdk1.4.2_01\include" /I "E:\j2sdk1.4.2_01\include\win32" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "HSDLL_EXPORTS" /D "AS_DLL" /D "AS_JNI" /D "AS_SSL" /Fp"$(INTDIR)\hsdll.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\hsdll.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib ntwdblib.lib EXCHINST.LIB ssleay32.lib libeay32.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\hsdll.pdb" /machine:I386 /out:"$(OUTDIR)\hsdll.dll" /implib:"$(OUTDIR)\hsdll.lib" /libpath:"c:\openssl\lib" 
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
	"$(INTDIR)\memtrack.obj" \
	"$(INTDIR)\memtrkmn.obj" \
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

"$(OUTDIR)\hsdll.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "hsdll - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\hsdll.dll" "$(OUTDIR)\hsdll.bsc"


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
	-@erase "$(INTDIR)\memtrack.obj"
	-@erase "$(INTDIR)\memtrack.sbr"
	-@erase "$(INTDIR)\memtrkmn.obj"
	-@erase "$(INTDIR)\memtrkmn.sbr"
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
	-@erase "$(OUTDIR)\hsdll.bsc"
	-@erase "$(OUTDIR)\hsdll.dll"
	-@erase "$(OUTDIR)\hsdll.exp"
	-@erase "$(OUTDIR)\hsdll.ilk"
	-@erase "$(OUTDIR)\hsdll.lib"
	-@erase "$(OUTDIR)\hsdll.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "c:\abinition\hs\hsdll" /I "$(MYSQL_HOME)\include\\" /I "$(OPENSSL_HOME)\include\\" /I "$(JAVA_HOME)\include" /I "$(JAVA_HOME)\include\win32" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "HSDLL_EXPORTS" /D "AS_DLL" /D "AS_JNI" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\hsdll.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\hsdll.bsc" 
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
	"$(INTDIR)\memtrack.sbr" \
	"$(INTDIR)\memtrkmn.sbr" \
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

"$(OUTDIR)\hsdll.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib libmysql.lib ntwdblib.lib EXCHINST.LIB ssleay32.lib libeay32.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\hsdll.pdb" /debug /machine:I386 /out:"$(OUTDIR)\hsdll.dll" /implib:"$(OUTDIR)\hsdll.lib" /pdbtype:sept /libpath:"$(OPENSSL_HOME)\lib" /libpath:"$(MYSQL_HOME)\lib\opt" 
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
	"$(INTDIR)\memtrack.obj" \
	"$(INTDIR)\memtrkmn.obj" \
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

"$(OUTDIR)\hsdll.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("hsdll.dep")
!INCLUDE "hsdll.dep"
!ELSE 
!MESSAGE Warning: cannot find "hsdll.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "hsdll - Win32 Release" || "$(CFG)" == "hsdll - Win32 Debug"
SOURCE=..\code\aimsg.c

!IF  "$(CFG)" == "hsdll - Win32 Release"


"$(INTDIR)\aimsg.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "hsdll - Win32 Debug"


"$(INTDIR)\aimsg.obj"	"$(INTDIR)\aimsg.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\code\branch.c

!IF  "$(CFG)" == "hsdll - Win32 Release"


"$(INTDIR)\branch.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "hsdll - Win32 Debug"


"$(INTDIR)\branch.obj"	"$(INTDIR)\branch.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\code\cgi.c

!IF  "$(CFG)" == "hsdll - Win32 Release"


"$(INTDIR)\cgi.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "hsdll - Win32 Debug"


"$(INTDIR)\cgi.obj"	"$(INTDIR)\cgi.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\code\channel.c

!IF  "$(CFG)" == "hsdll - Win32 Release"


"$(INTDIR)\channel.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "hsdll - Win32 Debug"


"$(INTDIR)\channel.obj"	"$(INTDIR)\channel.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\code\concept.c

!IF  "$(CFG)" == "hsdll - Win32 Release"


"$(INTDIR)\concept.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "hsdll - Win32 Debug"


"$(INTDIR)\concept.obj"	"$(INTDIR)\concept.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\code\data.c

!IF  "$(CFG)" == "hsdll - Win32 Release"


"$(INTDIR)\data.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "hsdll - Win32 Debug"


"$(INTDIR)\data.obj"	"$(INTDIR)\data.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\code\dateparse.c

!IF  "$(CFG)" == "hsdll - Win32 Release"


"$(INTDIR)\dateparse.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "hsdll - Win32 Debug"


"$(INTDIR)\dateparse.obj"	"$(INTDIR)\dateparse.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\code\env.c

!IF  "$(CFG)" == "hsdll - Win32 Release"


"$(INTDIR)\env.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "hsdll - Win32 Debug"


"$(INTDIR)\env.obj"	"$(INTDIR)\env.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\code\fileio.c

!IF  "$(CFG)" == "hsdll - Win32 Release"


"$(INTDIR)\fileio.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "hsdll - Win32 Debug"


"$(INTDIR)\fileio.obj"	"$(INTDIR)\fileio.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\code\frame.c

!IF  "$(CFG)" == "hsdll - Win32 Release"


"$(INTDIR)\frame.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "hsdll - Win32 Debug"


"$(INTDIR)\frame.obj"	"$(INTDIR)\frame.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\code\function.c

!IF  "$(CFG)" == "hsdll - Win32 Release"


"$(INTDIR)\function.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "hsdll - Win32 Debug"


"$(INTDIR)\function.obj"	"$(INTDIR)\function.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\code\hash.c

!IF  "$(CFG)" == "hsdll - Win32 Release"


"$(INTDIR)\hash.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "hsdll - Win32 Debug"


"$(INTDIR)\hash.obj"	"$(INTDIR)\hash.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\code\hs.c

!IF  "$(CFG)" == "hsdll - Win32 Release"


"$(INTDIR)\hs.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "hsdll - Win32 Debug"


"$(INTDIR)\hs.obj"	"$(INTDIR)\hs.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\code\hsms.c

!IF  "$(CFG)" == "hsdll - Win32 Release"


"$(INTDIR)\hsms.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "hsdll - Win32 Debug"


"$(INTDIR)\hsms.obj"	"$(INTDIR)\hsms.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\code\http.c

!IF  "$(CFG)" == "hsdll - Win32 Release"


"$(INTDIR)\http.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "hsdll - Win32 Debug"


"$(INTDIR)\http.obj"	"$(INTDIR)\http.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\code\hyp.c

!IF  "$(CFG)" == "hsdll - Win32 Release"


"$(INTDIR)\hyp.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "hsdll - Win32 Debug"


"$(INTDIR)\hyp.obj"	"$(INTDIR)\hyp.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\code\instance.c

!IF  "$(CFG)" == "hsdll - Win32 Release"


"$(INTDIR)\instance.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "hsdll - Win32 Debug"


"$(INTDIR)\instance.obj"	"$(INTDIR)\instance.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\code\label.c

!IF  "$(CFG)" == "hsdll - Win32 Release"


"$(INTDIR)\label.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "hsdll - Win32 Debug"


"$(INTDIR)\label.obj"	"$(INTDIR)\label.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\code\load.c

!IF  "$(CFG)" == "hsdll - Win32 Release"


"$(INTDIR)\load.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "hsdll - Win32 Debug"


"$(INTDIR)\load.obj"	"$(INTDIR)\load.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\code\memtrack.c

!IF  "$(CFG)" == "hsdll - Win32 Release"


"$(INTDIR)\memtrack.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "hsdll - Win32 Debug"


"$(INTDIR)\memtrack.obj"	"$(INTDIR)\memtrack.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\code\memtrkmn.c

!IF  "$(CFG)" == "hsdll - Win32 Release"


"$(INTDIR)\memtrkmn.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "hsdll - Win32 Debug"


"$(INTDIR)\memtrkmn.obj"	"$(INTDIR)\memtrkmn.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\code\method.c

!IF  "$(CFG)" == "hsdll - Win32 Release"


"$(INTDIR)\method.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "hsdll - Win32 Debug"


"$(INTDIR)\method.obj"	"$(INTDIR)\method.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\code\operand.c

!IF  "$(CFG)" == "hsdll - Win32 Release"


"$(INTDIR)\operand.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "hsdll - Win32 Debug"


"$(INTDIR)\operand.obj"	"$(INTDIR)\operand.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\code\operator.c

!IF  "$(CFG)" == "hsdll - Win32 Release"


"$(INTDIR)\operator.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "hsdll - Win32 Debug"


"$(INTDIR)\operator.obj"	"$(INTDIR)\operator.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\code\parse.c

!IF  "$(CFG)" == "hsdll - Win32 Release"


"$(INTDIR)\parse.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "hsdll - Win32 Debug"


"$(INTDIR)\parse.obj"	"$(INTDIR)\parse.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\code\port.c

!IF  "$(CFG)" == "hsdll - Win32 Release"


"$(INTDIR)\port.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "hsdll - Win32 Debug"


"$(INTDIR)\port.obj"	"$(INTDIR)\port.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\code\promis.c

!IF  "$(CFG)" == "hsdll - Win32 Release"


"$(INTDIR)\promis.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "hsdll - Win32 Debug"


"$(INTDIR)\promis.obj"	"$(INTDIR)\promis.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\code\route.c

!IF  "$(CFG)" == "hsdll - Win32 Release"


"$(INTDIR)\route.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "hsdll - Win32 Debug"


"$(INTDIR)\route.obj"	"$(INTDIR)\route.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\code\router.c

!IF  "$(CFG)" == "hsdll - Win32 Release"


"$(INTDIR)\router.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "hsdll - Win32 Debug"


"$(INTDIR)\router.obj"	"$(INTDIR)\router.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\code\secs.c

!IF  "$(CFG)" == "hsdll - Win32 Release"


"$(INTDIR)\secs.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "hsdll - Win32 Debug"


"$(INTDIR)\secs.obj"	"$(INTDIR)\secs.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\code\secs1.c

!IF  "$(CFG)" == "hsdll - Win32 Release"


"$(INTDIR)\secs1.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "hsdll - Win32 Debug"


"$(INTDIR)\secs1.obj"	"$(INTDIR)\secs1.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\code\secs2.c

!IF  "$(CFG)" == "hsdll - Win32 Release"


"$(INTDIR)\secs2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "hsdll - Win32 Debug"


"$(INTDIR)\secs2.obj"	"$(INTDIR)\secs2.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\code\signal.c

!IF  "$(CFG)" == "hsdll - Win32 Release"


"$(INTDIR)\signal.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "hsdll - Win32 Debug"


"$(INTDIR)\signal.obj"	"$(INTDIR)\signal.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\code\sock.c

!IF  "$(CFG)" == "hsdll - Win32 Release"


"$(INTDIR)\sock.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "hsdll - Win32 Debug"


"$(INTDIR)\sock.obj"	"$(INTDIR)\sock.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\code\sort.c

!IF  "$(CFG)" == "hsdll - Win32 Release"


"$(INTDIR)\sort.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "hsdll - Win32 Debug"


"$(INTDIR)\sort.obj"	"$(INTDIR)\sort.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\code\sql.c

!IF  "$(CFG)" == "hsdll - Win32 Release"


"$(INTDIR)\sql.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "hsdll - Win32 Debug"


"$(INTDIR)\sql.obj"	"$(INTDIR)\sql.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\code\ssl.c

!IF  "$(CFG)" == "hsdll - Win32 Release"


"$(INTDIR)\ssl.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "hsdll - Win32 Debug"


"$(INTDIR)\ssl.obj"	"$(INTDIR)\ssl.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\code\stack.c

!IF  "$(CFG)" == "hsdll - Win32 Release"


"$(INTDIR)\stack.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "hsdll - Win32 Debug"


"$(INTDIR)\stack.obj"	"$(INTDIR)\stack.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\code\stmt.c

!IF  "$(CFG)" == "hsdll - Win32 Release"


"$(INTDIR)\stmt.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "hsdll - Win32 Debug"


"$(INTDIR)\stmt.obj"	"$(INTDIR)\stmt.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\code\system.c

!IF  "$(CFG)" == "hsdll - Win32 Release"


"$(INTDIR)\system.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "hsdll - Win32 Debug"


"$(INTDIR)\system.obj"	"$(INTDIR)\system.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\code\tcp.c

!IF  "$(CFG)" == "hsdll - Win32 Release"


"$(INTDIR)\tcp.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "hsdll - Win32 Debug"


"$(INTDIR)\tcp.obj"	"$(INTDIR)\tcp.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\code\type.c

!IF  "$(CFG)" == "hsdll - Win32 Release"


"$(INTDIR)\type.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "hsdll - Win32 Debug"


"$(INTDIR)\type.obj"	"$(INTDIR)\type.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\code\util.c

!IF  "$(CFG)" == "hsdll - Win32 Release"


"$(INTDIR)\util.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "hsdll - Win32 Debug"


"$(INTDIR)\util.obj"	"$(INTDIR)\util.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 


!ENDIF 

