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
$!   $Log: build_hs.com,v $
$!   Revision 1.11  2009-12-08 21:01:50  bergsma
$!   Differentiate between hsp and hss
$!
$!   Revision 1.10  2009-03-01 22:24:51  bergsma
$!   no message
$!
$!   Revision 1.9  2005-05-10 17:29:48  bergsma
$!   Typo, hs.olb should be hs_non_promis.olb
$!
$!   Revision 1.8  2005/04/03 17:36:19  bergsma
$!   HS 3.54  (FIX OF FLOATING POINT OVERFLOW IN TLOGFEED).
$!   1. Don't delete LISting files.
$!   2. PackStart in aeqssp_autofil not being cleared - was causing an
$!   unpack operation when not required.
$!
$!   Revision 1.7  2004/12/06 20:46:53  bergsma
$!   For newer versions of VMS, the TCP/IP stack is available in the
$!   default system library.
$!
$!   Revision 1.6  2004/07/01 01:55:15  bergsma
$!   Do not put stats.c in the library!
$!
$!   Revision 1.4  2004/04/29 08:33:44  bergsma
$!   Added HTTP support
$!
$!   Revision 1.3  2003/06/05 21:35:17  bergsma
$!   Integrate stats.exe from stats.c
$!
$!   Revision 1.2  2003/01/16 14:40:47  bergsma
$!   V3.1.0
$!   Added Modification tag.
$!
$!
$!-
$ on error then $ goto ERROR
$ build_hs == 0
$ @aeq_ssp:comp
$ @aeq_ssp:ccc
$
$ if ( ccc .eqs. "" )
$ then
$   write sys$output "CXX or VAXC or DECC compilers not Found."
$   if ( f$search ( "hs_non_promis.olb" ) .nes. "" )
$   then
$     ! HS library supplied.
$     comp automan.for
$     comp automan4.for
$     comp autoutil.for
$     libr hs_non_promis.olb automan,automan4,autoutil
$     open/write opt hs.opt
$       write opt "sys$library:decc$shr.exe/share"
$       if ( runlib .nes. "" ) then $ write opt "''runlib'"
$     close opt
$     ! Pull out 'main()' for building hs.exe
$     libr hs_non_promis/extract=(hs)/output=hss.obj
$     ! Pull out 'main()' for building stats.exe
$     libr hs_non_promis/extract=(stats)/output=stats.obj
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
$! 1. contains main() for hs.exe (and stats.exe)
$!
$ ccc HS.c /obj=hss.obj
$ ccc stats.c
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
$! 3. Needed for GD access
$!
$ ccc gd.c
$
$!
$! Create new library
$!
$ if f$search ("hs_non_promis.olb") .nes. "" then $ delete hs_non_promis.olb;*
$ libr/create hs_non_promis
$!
$!
$! Insert object files into the hs library.
$!
$ libr hs_non_promis.olb -
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
gd.obj+-
hash.obj+-
hsms.obj+-
hss.obj+-
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
$ 'build'/exec=hss.exe/map hss.obj+hs_non_promis.olb/lib+hs.opt/opt
$ 'build'/exec=stats.exe stats.obj+hs_non_promis.olb/lib+hs.opt/opt
$!
$! Cleanup the directory
$!
$ purge *.*
$ !!delete *.lis;*
$!
$DONE:
$ exit
$
$ERROR:
$ build_hs == 0
$ exit
