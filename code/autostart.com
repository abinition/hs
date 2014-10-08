$ ! AEQ_SSP:AUTOSTART.COM
$ !
$ ! Modifications:
$ ! 	$Log: autostart.com,v $
$ ! 	Revision 1.5  2002-11-22 16:06:27  bergsma
$ ! 	Restore back to V3.0.6.
$ ! 	
$ !
$ if ( f$search ( "aeq_ssp:config.com" ) .nes. "" )
$ then
$   @aeq_ssp:config
$ else
$   @aeq_ssp:config_template
$ endif
$
$ if ( .not. allowAutoRouter ) then $ exit
$
$ on warning then goto START
$ if p1 .nes. "" 
$ then 
$   @aeq_ssp:start 'p1'
$   goto DONE
$ endif
$
$ router = "''f$trnlnm("AUTOROUTER")'"
$ mbxRouter = "''f$trnlnm("AUTOFIFO")'_''router'"
$
$ ! Start AutoRouter first
$ @aeq_ssp:start 'router'
$ if p1 .nes. "" then goto DONE
$ !
$ !
$ set noon
$ count = 10
$ WAIT_ROUTER:
$  open/read mbx/error=OPEN_ERROR 'mbxRouter'
$  if $status then $ goto END_WAIT
$ OPEN_ERROR:
$  if ( count .eq. 0 ) 
$  then
$    write sys$output "Router failed to initialize"
$    goto DONE
$  endif
$  write sys$output "...waiting for router to initialize"
$  count = count - 1 
$  wait 00:00:03
$  goto WAIT_ROUTER
$ END_WAIT:
$
$ close mbx
$ write sys$output "Router has started, mailbox ''mbxRouter' is created" 
$ 
$ ! Start services
$ list_det = "''f$trnlnm(""AUTODET"")'"
$ i = 1
$LOOP:
$
$ service = f$element( i, ":", list_det )
$ if ( service .eqs. "" .or. service .eqs. "/" ) then $ goto END_LOOP
$
$START:
$ @aeq_ssp:start 'service'
$ i = i + 1
$ goto LOOP
$END_LOOP:
$
$DONE:
$ exit
