$! HS.COM
$!
$! Execute HyperScript via HS.COM
$!
$!
$! Modifications:
$!
$!   $Log: hs.com,v $
$!   Revision 1.2  2003/01/16 14:40:47  bergsma
$!   V3.1.0
$!   Added Modification tag.
$!
$!
$!+
$ ! Initialize
$ show time
$ !!!set proc/priority=5
$ ! 
$ define vaxc$path autorun:
$
$ ! Determine name of script file to execute
$ prcnam = f$getjpi("","PRCNAM")
$ target = prcnam 
$ dot = f$locate ( ".", prcnam )
$ if dot .lt. f$length ( prcnam ) then $ prcnam[dot*8,8] == "_"
$ ! Look for com procedure first
$ comfile = "autorun:''prcnam'.com"
$ if f$search( comfile, 0 ) .nes. "" 
$ then
$   @'comfile'
$ else
$   scrfile = "autorun:''prcnam'.hyp"
$   if f$search( scrfile, 0 ) .nes. "" 
$   then 
$     hse := $aeq_ssp:hs.exe
$     write sys$output "$ hs -f ''scrfile' -t ''target' -e CONNECT -e DISCONNECT -e ABORT"
$     hse -f 'scrfile' -t 'target' -e "CONNECT" -e "DISCONNECT" -e "ABORT"
$   else
$     write sys$output "Cannot locate ''scrfile'"
$   endif
$ endif
$ exit
