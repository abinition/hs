$! CC.COM
$!
$! 'C' compiler options
$!
$! Modifications:
$!
$!	$Log: ccc.com,v $
$!	Revision 1.3  2004/12/06 20:46:33  bergsma
$!	For newer versions of VMS, the TCP/IP stack is available in the
$!	default system library.
$!	
$!	Revision 1.2  2002/09/03 21:14:12  bergsma
$!	Check for C++ first, then DEC C compiler.
$!	
$!
$ ! Clear any predefined targets for 'cc'
$ if "''cc'" .nes. "" then delete/sym/global cc
$
$ ! Get architecture options
$ @aeq_ssp:arch
$
$ ! Get TCP options
$ @aeq_ssp:tcp
$
$ ! Initialize compile define options
$ if p1 .nes. "" 
$ then
$   options == "''arch'''tcpdef',''p1'"
$ else
$   options == "''arch'''tcpdef'"
$ endif
$
$! Check for ORACLE
$ sql = 0
$
$ if f$search ("sys$system:cxx$compiler.exe") .nes. "" 
$ then 
$ 
$   options == options + ",AS_CXX"
$   ccc :== cxx	/nowarn/debug/object/list/noopt/define=('options')
$   
$   open/write opt hs.opt
$     write opt "''runlib'"
$   close opt
$
$   build == "cxxlink"
$
$ else if f$search ("sys$system:decc$compiler.exe") .nes. "" 
$ then 
$ 
$   options == options + ",AS_DECC"
$   ccc :== cc	/decc-
		/warn=noinfo/debug/object/list/noopt-
		/define=('options')   
$   
$   ! Clear any predefined system include paths for 'cc'
$   if f$trnlnm("DECC$SYSTEM_INCLUDE","LNM$PROCESS_TABLE") .nes. "" then -
    deass DECC$SYSTEM_INCLUDE
$
$   def sys sys$common:[decc$lib.reference.decc$rtldef]
$
$   open/write opt hs.opt
$     write opt "sys$library:decc$shr.exe/share"
$     write opt "''runlib'"
$   close opt
$
$   build == "link"
$
$ else if f$search ("sys$system:vaxc.exe") .nes. "" 
$ then 
$
$   options == options + ",AS_VAXC"
$   ccc :== cc	/vaxc/stand=noport-
		/warn=noinfo/debug/object/list/noopt-
		/define=('options')
$
$   define/nolog sys sys$library:
$
$   open/write opt hs.opt
$     write opt "sys$library:vaxcrtl.exe/share"
$     write opt "''runlib'"
$   close opt
$   
$   build == "link"
$
$ else
$
$   ccc == ""
$   build == ""
$
$ endif
$ endif
$ endif
$
$ exit
