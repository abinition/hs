$ ! CONFIG_TEMPLATE.COM
$ !
$ ! *** YOU SHOULD CREATE YOUR OWN CONFIG.COM ***
$ !
$ ! Modifications:
$ !
$ !
$ !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
$ !
$ !  M O D I F Y   T H I S  S E C T I O N  O N L Y
$ !
$ ! There can only be one AUTOROUTER per port, see option 13
$ ! This command procedure assumes two nodes, production and development
$ ! Add more nodes as needed.
$ !
$ ! 1. Allowed run account
$ list_user    = "/PROMISOPS/PROMISDEV/PROMISTST/"
$ 
$ ! 2. Allowed node name. Put "*" if node name does not matter.
$ list_node    = "/*/*/*/"
$
$ ! 3. Primary or Secondary. Put "*" if doesn't matter.
$ list_where   = "/PRIMARY/PRIMARY/PRIMARY/"
$
$ ! 4. Allowed promis system name.  Put "*" if promis name does not matter.
$ !    Put in "OFF" or "DISABLED" to prevent automation router and detached
$ !    processes from starting.
$ list_system  = "/OPS/DEV/TST/"
$
$ ! 5. AutoRun directory, where services are found, including autorouter
$ defaultRun   = "''f$trnlnm(""AEQ_SSP"")'" - "]" + ".RUN]"
$ list_run     = "/''defaultRun'/''defaultRun'/''defaultRun'/"
$
$ ! 6. AutoLog directory.
$ defaultLog   = "''f$trnlnm(""AEQ_SSP"")'" - "]" + ".LOG]"  
$ list_log     = "/''defaultLog'/''defaultLog'/''defaultLog'/"
$
$ ! 7. List of detached services to start up. Separate with ":"
$ defaultDet   = ":tp_info:"
$ list_det     = "/''defaultDet'/''defaultDet'/''defaultDet'/"
$
$ ! 8. Whether OPTION_AUTO should be set to YES or NO
$ !    This is for AUTOMAN only, it does not affect whether AUTOMATION starts
$ defaultAuto = "NO"
$ list_auto    = "/''defaultAuto'/''defaultAuto'/''defaultAuto'/"
$
$ ! 9. Fifo directory.  In VMS, this is the prefix for mailbox names".
$ defaultFifo = "MBX"
$ list_fifo    = "/''defaultFifo'/''defaultFifo'/''defaultFifo'/"
$
$ ! 10. Route node name.  In VMS, this is the next part of the mailbox name, the root node
$ defaultRouter = "ROUTER"
$ list_router   = "/''defaultRouter'/''defaultRouter'/''defaultRouter'/"
$
$ ! 11. AutoSpool directory.
$ defaultSpool   = "''f$trnlnm(""AEQ_SSP"")'" - "]" + ".SPOOL]"  
$ list_spool     = "/''defaultSpool'/''defaultSpool'/''defaultSpool'/"
$
$ ! 12. AutoBin directory.
$ defaultBin   = "''f$trnlnm(""AEQ_SSP"")'" 
$ list_Bin     = "/''defaultBin'/''defaultBin'/''defaultBin'/"
$
$ ! 13. AutoPort number.
$ defaultPort   = "7001"  
$ list_Port     = "/''defaultPort'/''defaultPort'/''defaultPort'/"
$
$ ! 14. AutoSqlServer .
$ defaultSQLserver   = "sql_server"  
$ list_SQLserver     = "/''defaultSQLserver'/''defaultSQLserver'/''defaultSQLserver'/"
$
$ ! 15. Auto DCOP directory for DCOP/prompt mapping
$ defaultDCOP = "''f$trnlnm(""AEQ_SSP"")'" - "]" + ".DCOPS]"
$ list_dcop     = "/''defaultDCOP'/''defaultDCOP'/''defaultDCOP'/"
$
$ ! 16. Automation Statistics.  Alternate log directory for ROUTER logs
$ defaultStat     = "''f$trnlnm(""AEQ_SSP"")'" - "]" + ".STATS]"
$ list_stat       = "/''defaultStat'/''defaultStat'/''defaultStat'/
$
$ ! 17. Automation owner of detached processes started by START.COM
$ ! Set to same name as user command procedure.... 
$ list_mgr    = list_user
$ 
$ !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
$ 
$ ! Get specifics of PROMIS system running on the current node
$ nodename  == f$getsyi("NODENAME")
$ username  == f$edit(f$getjpi("","USERNAME"),"TRIM")
$ promiswhere == f$logical ( "PROMIS_WHERE" )
$ if promiswhere .eqs. "" then promiswhere == "PRIMARY"
$ promissystemname == f$logical("PROMIS_SYSTEMNAME")
$ if promissystemname .eqs. "" then promissystemname == "PROMIS"
$
$ ! Determine whether AutoRouter is ok to run or not
$
$ ! Search for the correct account, node, promis where, promis system
$
$ allowAutoRouter == 0
$ i = 1
$LOOP:
$
$ user   = f$element(i,"/",list_user)
$ if ( user .eqs. "" .or. user .eqs. "/" ) then $ goto END_LOOP
$
$ node    = f$element(i,"/",list_node)
$ where   = f$element(i,"/",list_where)
$ system  = f$element(i,"/",list_system)
$ automan = f$element(i,"/",list_auto)
$ 
$ !write sys$output "''user' == ''username'?"
$ !write sys$output "''where' == ''promiswhere'?"
$ !write sys$output "''node' == ''nodename'?"
$ !write sys$output "''system' == ''promissystemname'?"
$
$ if (	user .eqs. username .and. -
	( where .eqs. "*" .or. where .eqs. promiswhere ) .and. -
	( node .eqs. "*" .or. node .eqs. nodename ) .and. -
        ( system .eqs. "*" .or. system .eqs. promissystemname ) ) 
$ then 
$   allowAutoRouter == 1
$   goto END_LOOP
$ else
$   i = i + 1
$ endif
$ goto LOOP
$
$END_LOOP:
$
$ if allowAutoRouter 
$ then
$   write sys$output "AutoRouter is allowed to run"
$ else
$   write sys$output "AutoRouter is not allowed to run"
$   exit
$ endif
$
$ if ( automan .eqs. "YES" ) 
$ then 
$    write sys$output "AUTOMAN is turned ON"
$ else
$    write sys$output "AUTOMAN is turned OFF"
$ endif
$
$ write sys$output "Defining group logicals"
$
$ define/group/nolog AUTOLOG 		"''f$element(i,"/",list_log)'"
$ define/group/nolog AUTOSPOOL		"''f$element(i,"/",list_spool)'"
$ define/group/nolog AUTORUN 		"''f$element(i,"/",list_run)'"
$ define/group/nolog AUTOFIFO 		"''f$element(i,"/",list_fifo)'"
$ define/group/nolog AUTOBIN 		"''f$element(i,"/",list_bin)'"
$ define/group/nolog AUTOROUTER 	"''f$element(i,"/",list_router)'"
$ define/group/nolog AUTODET 		"''f$element(i,"/",list_det)'"
$ define/group/nolog AUTOPORT 		"''f$element(i,"/",list_port)'"
$ define/group/nolog AUTOSQLSERVER 	"''f$element(i,"/",list_SQLserver)'"
$ define/group/nolog AUTOSQLSERVER2 	"''f$element(i,"/",list_SQLserver2)'"
$ define/group/nolog AUTOSQLSERVER3 	"''f$element(i,"/",list_SQLserver3)'"
$ define/group/nolog AUTOSTAT         	"''f$element(i,"/",list_stat)'"
$ define/group/nolog AUTODCOP         	"''f$element(i,"/",list_dcop)'"
$ define/group/nolog AUTOMGR          	"''f$element(i,"/",list_mgr)'"
$
$ define/group/nolog OPTION_AUTO 	"''f$element(i,"/",list_auto)'"
$
$ define/group/nolog OPTION_AUTO_LOG YES
$
$ define/group/nolog MM AEQ_SSP:MM.COM
$ define/group/nolog HS AEQ_SSP:HS.COM
$
$ write sys$output "Username          = ''username'"
$ write sys$output "Nodename          = ''nodename'"
$ write sys$output "System            = ''promissystemname'"
$ write sys$output "Where             = ''promiswhere'"
$ write sys$output "Autolog           = ''f$trnlnm(""AUTOLOG"")'"
$ write sys$output "AutoSpool         = ''f$trnlnm(""AUTOSPOOL"")'"
$ write sys$output "Autorun           = ''f$trnlnm(""AUTORUN"")'"
$ write sys$output "Services          = ''f$trnlnm(""AUTODET"")'"
$ write sys$output "Mailbox prefix    = ''f$trnlnm(""AUTOFIFO"")'"
$ write sys$output "AutoBin           = ''f$trnlnm(""AUTOBIN"")'"
$ write sys$output "Root router       = ''f$trnlnm(""AUTOROUTER"")'"
$ write sys$output "Router port       = ''f$trnlnm(""AUTOPORT"")'"
$ write sys$output "SQL server        = ''f$trnlnm(""AUTOSQLSERVER"")'"
$ write sys$output "AutoStat          = ''f$trnlnm(""AUTOSTAT"")'"
$ write sys$output "AutoDcop          = ''f$trnlnm(""AUTODCOP"")'"
$ write sys$output "AutoMgr           = ''f$trnlnm(""AUTOMGR"")'"
$ write sys$output "OPTION_AUTO       = ''f$trnlnm(""OPTION_AUTO"")'"
$ write sys$output "OPTION_AUTO_LOG   = ''f$trnlnm(""OPTION_AUTO_LOG"")'"
$
$ ! Define some useful symbols
$ xclose :== @AEQ_SSP:stop
$ xopen  :== @AEQ_SSP:start
$ xstop  :== @AEQ_SSP:stop
$ xstart :== @AEQ_SSP:start
$ hss    :== $AEQ_SSP:HSS
$ hsp    :== $AEQ_SSP:HSP
$ hse    :== $AEQ_SSP:HS
$ dird   :== dir/size/date=modified
$ grp    :== @IPSA_UTIL:GRP
$
$ exit
