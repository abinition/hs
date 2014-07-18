$! AEQ_SSP:START.COM
$!
$! Modifications:
$!
$! $Log: start.com,v $
$! Revision 1.15  2006-01-23 17:42:42  bergsma
$! Uppercase incoming argument P1
$!
$! Revision 1.14  2005/12/08 03:06:48  bergsma
$! UIC of started process is AUTOMGR, not f$user()
$!
$! Revision 1.13  2005/08/03 14:00:20  bergsma
$! no message
$!
$! Revision 1.12  2005/03/07 19:55:23  myockey
$! Increased io_direct and io_buffer
$!
$! Revision 1.1  2005/02/06 17:57:50  bergsma
$! TLOGFEED 3.4.3
$!
$! Revision 1.11  2004/12/10 22:39:41  bergsma
$! Need to add and restore privs when starting automation services.
$!
$! Revision 1.10  2003/04/23 22:53:09  bergsma
$! Remove hardcoding of "MBX", replace with AUTOFIFO translation.
$!
$! Revision 1.9  2003/02/17 09:56:59  bergsma
$! Only send a CONNECT message to the target if it was NOT started using
$! LOGINOUT.  It is expected that all service hyperscripts such as TP_*.scr or the
$! TLOGFEED.HYP have *.COM procedures in AUTORUN and that they call CONNECT();
$! within the hyperscript program.
$!
$! Revision 1.8  2002/11/22 16:07:21  bergsma
$! Restore to version 3.0.6
$!
$! Revision 1.4  2002/10/29 02:45:10  bergsma
$! Extra 'endif' removed.
$!
$! Revision 1.3  2002/10/12 02:20:06  bergsma
$! Send a CONNECT message, BUT NOT TO TP_* processes, they CONNECT() themselves
$!
$! Revision 1.2  2002/09/16 18:06:34  bergsma
$! After starting a service, always send it a CONNECT message.
$!
$!
$!
$
$ if p1 .eqs. "" then -
  inquire p1 "Enter automation target, (ie: ROUTER, PRIF, PROT) "
$
$ if p1 .eqs. "" then exit
$
$ ! Determine the target (image) name, converting to uppercase
$ target := 'p1
$ len = f$length ( target )
$
$ router := 'f$trnlnm("AUTOROUTER")
$ mbx    := 'f$trnlnm("AUTOFIFO")
$ mbxRouter = "''mbx'_''router'"
$
$ ! Take off mbxRouter prefix if it exists
$ prefix = "''mbxRouter'_"
$ if f$locate ( prefix, target ) .eq. 0 
$ then 
$   target = target - prefix
$ endif
$
$ ! Establish base I/O
$ buf_lim = 500000      ! Base
$ max_msg_size = 5120
$
$ ! Determine image name from target
$ if target .eqs. router 
$ then 
$   image = "''router'"
$   ! MBX_ROUTER:   5120 max bytes/message * 16 message capacity = 80K
$   ! MBX_<client>: 5120 max b/m * 4 msg capacity * average 32 clients = 640K
$   ! Total of 720K buffer size required, or 144 message.
$   !!!buf_lim = buf_lim + ( 144 * max_msg_size )
$   buf_lim = buf_lim + ( 300 * max_msg_size )
$ else
$   ! MBX_<client>: 5120 max b/m * 4 msg capacity = 20K
$   ! Total of 20K buffer size required, or 4 messages
$   buf_lim = buf_lim + ( 4 * max_msg_size )
$   image = "''target'"
$ endif
$
$ ! Determine MBX name
$ mbxName = "''mbx'_''target'"
$
$ @aeq_ssp:findproc 'image'
$ if aeqssp_procId .nes. "" 
$ then
$
$   procGroup = f$fao("!OB",f$getjpi("''aeqssp_procId'","GRP"))
$   ourGroup = f$fao("!OB",f$getjpi("","GRP"))
$
$   if ( procGroup .eq. ourGroup )
$   then
$     write sys$output -
        "   Process ''image' is already running"
$   else
$     write sys$output -
        "   Process ''image' is already running in group ''procGroup'" 
$   endif
$
$ else 
$
$   if f$trnlnm(mbxName) .nes. "" 
$   then  
$
$     refcnt = 0
$     refcnt = f$getdvi(mbxname,"REFCNT")
$     write sys$output -
      "Warning - ''mbxname' is currently referenced by ''refcnt' processes"
$
$   endif
$
$   automgr = f$trnlnm("AUTOMGR") 
$   if automgr .eqs. "" then automgr = f$user()
$   
$   if f$search( "AUTOLOG:''filespec'.LOG" ) .nes. "" then -
      rename AUTOLOG:'filespec'.LOG .OLD
$
$   if ( f$search ( "AUTORUN:''target'.COM" ) .nes. "" )
$   then
$     write sys$output "   Starting ''image'"
$     @cfc:setpriv "NETMBX,TMPMBX,SYSNAM,DETACH,ALTPRI"
$     define/group/nolog _AR AUTORUN:
$     define/user sys$output nl:
$     run sys$system:loginout -
                /Process_name   = 'image' -
                /Input          = _AR:'target'.COM -
                /Output         = AUTOLOG:'target'.LOG -
                /Uic            = 'automgr' -
                /privs          = (NETMBX,TMPMBX,SYSNAM,DETACH) -
                /noAuthorize -
                /priority       = 4 -
                /page_file      = 2000000 -
                /working        = 2048 -
                /maximum_Working= 4096 -                
                /extent         = 8192 -                
                /enqueue_limit  = 500 -
                /io_direct      = 500 -
                /io_buffer      = 500 -
                /queue_limt     = 20 -
                /ast_limit      = 100 -
                /buffer_limit   = 'buf_lim'
$     set process/privilege=('cfc_oldprivs')
$   else
$     write sys$output "   Sending CONNECT message to ''image'"
$     @aeq_ssp:open 'target'
$   endif
$ endif
$
$ exit
