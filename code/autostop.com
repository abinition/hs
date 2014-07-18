$ ! AEQ_SSP:AUTOSTOP.COM
$ !
$!
$! Modifications:
$!
$!   $Log: autostop.com,v $
$!   Revision 1.2  2003/01/16 14:40:47  bergsma
$!   V3.1.0
$!   Added Modification tag.
$!
$!
$ if ( f$search ( "aeq_ssp:config.com" ) .nes. "" )
$ then
$   @aeq_ssp:config
$ else
$   @aeq_ssp:config_template
$ endif
$
$ if ( .not. allowAutoRouter ) then $ exit
$
$ router = "''f$trnlnm("AUTOROUTER")'"
$ mbxRouter = "''f$trnlnm("AUTOFIFO")'_''router'"
$
$ on warning then goto STOP
$ if p1 .nes. "" 
$ then 
$   @aeq_ssp:stop 'p1'
$   goto DONE
$ endif
$
$ ! Stop services first
$ list_det = "''f$trnlnm(""AUTODET"")'"
$ i = 1
$LOOP:
$
$ service = f$element( i, ":", list_det )
$ if ( service .eqs. "" .or. service .eqs. "/" ) then $ goto END_LOOP
$
$STOP:
$ @aeq_ssp:stop 'service'
$ i = i + 1
$ goto LOOP
$
$END_LOOP:
$
$ ! Stop AutoRouter last
$ @aeq_ssp:stop 'router'
$ if p1 .nes. "" then goto DONE
$
$DONE:
$ exit
