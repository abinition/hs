$! MM.COM
$
$! Execute PROMIS using PCL.COM interface
$!
$! Modifications:
$!
$!   $Log: mm.com,v $
$!   Revision 1.2  2003/01/16 14:40:47  bergsma
$!   V3.1.0
$!   Added Modification tag.
$!
$
$ !
$ ! Determine name of script file to execute
$ prcnam = f$getjpi("","PRCNAM")
$ dot = f$locate ( ".", prcnam )
$ if dot .lt. f$length ( prcnam ) then $ prcnam[dot*8,8] == "_"
$ scrfile = "autorun:''prcnam'.scr"
$ if f$search( scrfile, 0 ) .nes. "" 
$ then 
$   @mep:pcl @'scrfile'
$ else
$   write sys$output "Cannot locate ''scrfile'"
$ endif
$ exit
