$!+
$! TCP.COM - Command procedure to compile AutoRouter code
$!
$! Modifications:
$!
$!   $Log: tcp.com,v $
$!   Revision 1.3  2004/12/06 20:46:05  bergsma
$!   For newer versions of VMS, the TCP/IP stack is available in the
$!   default system library.
$!
$!   Revision 1.2  2003/01/16 14:40:47  bergsma
$!   V3.1.0
$!   Added Modification tag.
$!
$!
$!-
$ set noon
$!
$! Determine what TCP/IP vendor software is going to be used
$!
$ if f$search ( "sys$library:ucx$ipc_shr.exe" ) .nes. "" .and. -
     f$search ( "sys$library:tcpip$ipc_shr.exe") .eqs. "" ) 
$ then
$
$   ! UCX - old style
$   tcpdef == ",AS_UCX"
$   runlib == "sys$library:ucx$ipc_shr/share"
$
$ else
$ if f$search ( "sys$library:ucx$ipc_shr.exe" ) .eqs. "" .and. -
     f$search ( "sys$library:tcpip$ipc_shr.exe") .nes. "" ) 
$ then
$
$   ! UCX - New style
$   tcpdef == ""
$   runlib == ""
$
$
$ else
$ if f$logical ( "TCPWARE_INCLUDE" ) .nes. "" .and. -
     f$search  ( "SYS$LIBRARY:TCPWARE_SOCKLIB_SHR.EXE" ) .nes. ""
$ then
$
$   ! TCPware
$   tcpdef == ",AS_TCPWARE"
$   runlib == "SYS$LIBRARY:TCPWARE_SOCKLIB_SHR/share"
$
$   ! Set logicals in case we are using VAXC
$   define sys TCPWARE_INCLUDE
$   define arpa TCPWARE_INCLUDE
$   define netinet TCPWARE_INCLUDE
$
$ else
$ if f$logical ( "TWG$TCP" ) .nes. "" .and. -
     f$logical ( "TWG$COMMON" ) .nes. "" .and. -
     f$search  ( "TWG$TCP:[netdist.misc]def.com" ) .nes. "" .and. -
     f$search  ( "TWG$COMMON:[netdist.etc]twglib.exe" ) .nes. ""
$ then
$
$   ! Wollongong
$   tcpdef == ",AS_GONG"
$   runlib == "twg$common:[netdist.etc]twglib/share"
$
$   ! Set logicals for the include files in case we are using VAXC
$   @twg$tcp:[netdist.misc]def.com
$
$ else
$ if f$logical ( "MULTINET_SOCKET_LIBRARY" ) .nes. ""
$ then
$
$   ! Multinet
$   tcpdef == ",AS_MULTI"
$   runlib == "MULTINET:MULTINET_SOCKET_LIBRARY.exe/share"
$
$   ! Set logicals in case we are using VAXC
$   define DECC$USER_INCLUDE MULTINET:[multinet.include]
$   define sys MULTINET:[multinet.include.sys]
$   define arpa MULTINET:[multinet.include.arpa]
$   define netinet MULTINET:[multinet.include.netinet]
$
$ else
$
$   write sys$output "Specific TCP/IP vendor software not found."
$   write sys$output "Using default system TCP/IP stack"
$   tcpdef == ""
$   runlib == ""
$
$ endif
$ endif
$ endif
$ endif
$ endif
$
$ exit
