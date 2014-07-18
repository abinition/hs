$! AEQ_SSP:STOP.COM
$!
$! Modifications:
$!
$! $Log: stop.com,v $
$! Revision 1.2  2002/09/16 18:04:54  bergsma
$! When shutting down a detached process service, give it more time (3 seconds)
$! to shutdown normally before killing it with stop/id
$!
$! 
$
$ if p1 .eqs. "" then -
  inquire p1 "Enter automation target (ie: ROUTER, TP_INFO, HEARTBEAT) "
$
$ if p1 .eqs. "" then exit
$
$ set noon
$
$ router = "''f$trnlnm("AUTOROUTER")'"
$ mbxRouter = "''f$trnlnm("AUTOFIFO")'_''router'"
$
$ ! Determine the image name
$ target = p1
$ len = f$length ( target )
$
$ ! Take off mbxRouter prefix if it exists
$ prefix = "''mbxRouter'_"
$ if f$locate ( prefix, target ) .eq. 0 
$ then 
$   target = target - prefix
$ endif
$
$ if ( target .nes. router )
$ then
$   isRouter = 0
$   mbxName = "''mbxRouter'_''target'"
$ else
$   isRouter = 1
$   mbxName = "''mbxRouter'" 
$ endif
$
$ @aeq_ssp:findproc 'router'
$ if ( aeqssp_procId .nes. "" .and. f$trnlnm ( mbxRouter ) .nes. "" )
$ then
$   isRouterUp = 1
$ else
$   isRouterUp = 0
$ endif
$
$ if ( isRouterUp )
$ then
$
$     ! How many times is the mailbox being referenced
$     refcnt = 0
$     if ( f$logical(mbxname) .nes. "" ) then $ refcnt = f$getdvi(mbxname,"REFCNT")
$     if ( refcnt .gt. 0 ) then $ write sys$output "   ''mbxname' is referenced by ''refcnt' processes"
$     
$     open/write mbx 'mbxRouter'
$     if ( refcnt .gt. 1 .and. .not. isRouter )
$     then
$       write sys$output "   Instructing ''target' to ABORT"
$       write mbx "|''target'|EVENT|ABORT|''router'|||"
$     endif
$     if ( refcnt .gt. 0 .and. .not. isRouter ) 
$     then
$       write sys$output "   Instructing ''router' to un-register from ''target'"
$       write mbx "|''router'|EVENT|ABORT|''target'|||"
$     endif
$     if ( isRouter ) 
$     then
$       write sys$output "   Instructing ''router' to terminate"
$       write mbx "|''router'|EVENT|TERMINATE|''router'|||"
$     endif
$
$     close mbx
$
$     if ( f$locate ( ".AM", target ) .ne. f$length ( target ) )
$     then
$       write sys$output "   Writing an EOF to ''mbxname'"
$       open/write mbx 'mbxname'
$       close mbx
$     endif
$ endif
$
$  ! If the process is still running, kill it
$  write sys$output "   ...waiting 3 seconds for clean shutdown"
$  wait 00:00:03
$  write sys$output "   ...checking that ''target' is not running"
$  @aeq_ssp:findproc 'target'
$  if ( aeqssp_procId .nes. "" )
$  then
$    write sys$output "   ...still running! Trying STOP/ID"
$    stop/id='aeqssp_procId'
$  endif
$
$ exit
