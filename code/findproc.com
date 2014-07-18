$ !AEQ_SSP:FINDPROC.COM
$!
$! Modifications:
$!
$!   $Log: findproc.com,v $
$!   Revision 1.2  2003/01/16 14:40:47  bergsma
$!   V3.1.0
$!   Added Modification tag.
$!
$!
$
$ set noon
$ aeqssp_procid == ""
$ if p1 .eqs. "" then exit
$
$ show system /output=sys1.tmp 
$ set mess /noid/nosev/nofac/notext
$ search sys1.tmp " ''p1' " /output=sys2.tmp
$ set mess /id/sev/fac/text
$ delete/nolog sys1.tmp;
$ procCount = 0
$ line = ""
$ open/read 		tmpfile sys2.tmp
$READ_AGAIN:
$ read/end_of_file=EOF 	tmpfile line
$ procCount = procCount + 1
$ if procCount .gt. 1 then goto DONE
$ goto READ_AGAIN
$EOF:
$ if procCount .eq. 1 then aeqssp_procid == "''f$extract(0,8,line)'"
$DONE:
$ close tmpfile
$ delete/nolog sys2.tmp;
$ exit
