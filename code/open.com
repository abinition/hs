$! OPEN.COM
$!
$! Modifications:
$!
$!   $Log: open.com,v $
$!   Revision 1.2  2003-01-16 14:40:47  bergsma
$!   V3.1.0
$!   Added Modification tag.
$!
$!
$ if p1 .eqs. "" then goto DONE
$ router = "''f$trnlnm("AUTOROUTER")'"
$ mbxRouter = "''f$trnlnm("AUTOFIFO")'_''router'"
$ open/write/err=DONE mbx 'mbxRouter'           
$ write mbx "|''p1'|event|CONNECT|god|||"
$ close mbx
$DONE:
$ exit
