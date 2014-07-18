$!+
$! BUILD_HS.COM - Command procedure to build standalone HyperScript
$!		  image without any PROMIS integration
$!
$! Pre-requisites:
$!
$!	The VAXC or DECC C or CXX compiler is expected to be installed.
$!
$! Output:
$!
$!	Image AEQ_SSP:HS.EXE
$!
$! Author:	Michael Bergsma		3/18/97
$!
$!
$! Modifications:
$!
$!   $Log: build_hs_mysql.com,v $
$!   Revision 1.1  2005-05-10 17:30:14  bergsma
$!   Build file for MYSQL on VMS
$!
$!
$!
$!-
$ on error then $ goto ERROR
$ build_hs == 0
$ @aeq_ssp:comp
$ @aeq_ssp:ccc "AS_SQL,AS_MYSQL"
$ ccc=="''ccc'/INCLUDE=([],MYSQL_ROOT:[INCLUDE])"
$ @area$mysql_v41:[mysql_v41.vms]logicals
$ @mysql_root:[vms]symbols
$ open/write opt mysql.opt
$ write opt "mysql_root:[vms.lib]mysqlclient_mixed/lib,-"
$ write opt "mysql_root:[vms.lib]mysqlclient_upper/lib,-"
$ write opt "AREA$MYSQL_V41:[MYSQL_V41.OPENSSL.OPENSSL097E.ALPHA_LIB]OPENSSL097E_LIBSSL_SHR32/share,-"
$ write opt "AREA$MYSQL_V41:[MYSQL_V41.OPENSSL.OPENSSL097E.ALPHA_LIB]OPENSSL097E_LIBCRYPTO_SHR32/share,-"
$ write opt "AREA$MYSQL_V41:[MYSQL_V41.LIBZ]LIBZSHR32-1_2_2_EV56/share"
$ close opt
$
$ if ( ccc .eqs. "" )
$ then
$   write sys$output "CXX or VAXC or DECC compilers not Found."
$   if ( f$search ( "hs_sql_promis.olb" ) .nes. "" )
$   then
$     ! HS library supplied.
$     comp automan.for
$     comp automan4.for
$     comp autoutil.for
$     libr hs.olb automan,automan4,autoutil
$     open/write opt hs.opt
$       write opt "sys$library:decc$shr.exe/share"
$       if ( runlib .nes. "" ) then $ write opt "''runlib'"
$     close opt
$     ! Pull out 'main()' for building hs.exe
$     libr hs_sql_promis/extract=(hs)/output=hs.obj
$     goto LINK_HS
$   else
$     write sys$output "HyperScript could not be compiled"
$   endif
$   goto DONE
$
$ endif
$
$!
$! Compile all routines
$!
$!
$! 1. contains main() for hs.exe
$!
$ ccc HS.c
$!
$! 2. HyperScript modules
$!
$ ccc aimsg.c
$ ccc branch.c
$ ccc cgi.c
$ ccc channel.c
$ ccc concept.c
$ ccc data.c
$ ccc dateparse.c
$ ccc env.c
$ ccc hash.c
$ ccc fileio.c
$ ccc frame.c
$ ccc function.c
$ ccc hash.c
$ ccc hsms.c
$ ccc hyp.c
$ ccc http.c
$ ccc instance.c
$ ccc label.c
$ ccc load.c
$ ccc method.c
$ ccc memtrack.c
$ ccc operand.c
$ ccc operator.c
$ ccc parse.c
$ ccc port.c
$ ccc route.c
$ ccc router.c
$ ccc secs.c
$ ccc secs1.c
$ ccc secs2.c
$ ccc signal.c
$ ccc sock.c
$ ccc sort.c
$ ccc stack.c
$ ccc stmt.c
$ ccc system.c
$ ccc type.c
$ ccc tcp.c
$ ccc util.c
$!
$! 3. Needed for SQL access
$!
$ ccc sql.c
$
$!
$! Create new library
$!
$ if f$search ("hs_sql_promis.olb") .nes. "" then $ delete hs_sql_promis.olb;*
$ libr/create hs_sql_promis
$!
$!
$! Insert object files into the hs library.
$!
$ libr hs_sql_promis.olb -
aimsg.obj+-
branch.obj+-
cgi.obj+-
channel.obj+-
concept.obj+-
data.obj+-
dateparse.obj+-
env.obj+-
fileio.obj+-
frame.obj+-
function.obj+-
hash.obj+-
hsms.obj+-
hs.obj+-
hyp.obj+-
http.obj+-
instance.obj+-
label.obj+-
load.obj+-
method.obj+-
memtrack.obj+-
operand.obj+-
operator.obj+-
parse.obj+-
port.obj+-
route.obj+-
router.obj+-
secs1.obj+-
secs2.obj+-
secs.obj+-
signal.obj+-
sock.obj+-
sort.obj+-
sql.obj+-
stack.obj+-
stmt.obj+-
system.obj+-
tcp.obj+-
type.obj+-
util.obj
$
$LINK_HS:
$!
$!
$! Link an image
$!
$ 'build'/exec=hs_mysql.exe/map hs.obj+hs_sql_promis.olb/lib+hs.opt/opt+mysql.opt/opt
$!
$! Cleanup the directory
$!
$ purge *.*
$!
$DONE:
$ exit
$
$ERROR:
$ build_hs == 0
$ exit
