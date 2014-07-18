$ ! STATS.COM
$ !
$ ! Modifications:
$ ! 
$ !  $Log: stats.com,v $
$ !  Revision 1.1  2003/04/04 16:01:15  bergsma
$ !  The stats.c and stats.com will get statistics from router.log that can
$ !  be loaded into excell.
$ !
$ !
$
$ on error then exit ;

$ if p1 .eqs "" 
$ then
$   inquire p1 "Enter starting day of period in format DD-MMM-YYYY"   
$ endif
$
$ if p2 .eqs. ""
$ then
$   inquire p2 "Enter ending day of period in formay DD-MMM-YYYY"
$ endif
$
$ if p1 .eqs. "" .or. p2 .eqs. "" then $ exit
$ 
$ startfile = "ROUTER_''f$cvtime(p1,"COMPARISON","DATE")'" - "-" - "-"
$ endfile   = "ROUTER_''f$cvtime(p2,"COMPARISON","DATE")'" - "-" - "-" 
$ 
$ delete connect.tmp;*/nolog
$ open/write c connect.tmp
$ close c
$ 
$SCAN:
$ filespec = f$search ( "AUTOSTAT:ROUTER_*.log", 1 ) 
$ filename = f$parse  ( filespec,,,"NAME" )
$ if ( filespec .eqs. "" ) then $ goto END_SCAN
$ if ( filename .gts. endfile ) then $ goto END_SCAN
$ if ( filename .lts. startfile ) then $ goto SCAN
$ 
$ ! Extract all the CONNECT messages
$ sea 'filespec' "event|connect" /out=a.tmp
$ if .not. $status then $ goto SCAN
$
$ ! Remove references to router
$ sea a.tmp "router|"/match=(NOR)/out=b.tmp
$ if .not. $status then $ goto SCAN
$
$ append b.tmp connect.tmp
$ delete a.tmp;*,b.tmp;*/nolog
$ goto SCAN
$END_SCAN:
$
$ convert/fdl=connect.fdl connect.tmp connect.dat
$ stats :== $AEQ_SSP:STATS
$ stats -f "connect.dat" -l "transact.out"
$ delete/nolog connect.dat;*,connect.tmp;*
$ exit
