$ ! CONFIG.COM
$
$ !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
$ !
$ !  M O D I F Y   T H I S  S E C T I O N  O N L Y
$ !
$ ! There can only be one AUTOROUTER per node.
$ ! This command procedure assumes two nodes, production and development
$ ! Add more nodes as needed.
$ !
$ ! 1. Allowed run account
$ list_user    = "/PROD/TEST3/DEV/TRAIN/REL/"
$
$ ! 2. Allowed node name. Put "*" if node name does not matter.
$ node_prod   = f$logical("VAX_WITH_PROMPROD")
$ node_test3  = f$logical("VAX_WITH_PROMTEST3")
$ node_dev    = f$logical("VAX_WITH_PROMDEV")
$ node_train  = f$logical("VAX_WITH_PROMTRAIN")
$ node_rel    = f$logical("VAX_WITH_PROMREL")
$ list_node       = "/''node_prod'/''node_test3'/''node_dev'/''node_train'/''nod
e_rel'/"
$
$ ! 3. Primary or Secondary. Put "*" if doesn't matter.
$ list_where   = "/PRIMARY/PRIMARY/PRIMARY/PRIMARY/PRIMARY/"
$
$ ! 4. Allowed promis system name.  Put "*" if promis name does not matter.
$ list_system  = "/*/*/*/*/*/"
$
$ ! 5. AutoRun directory, where services are found, including autorouter
$ defaultRun      = "''f$trnlnm(""AEQ_SSP"")'" - "]" + ".RUN]"
$! area_prod   = "AREA$AUTOROUTER_HYPERSCRIPTS:[AUTOROUTER_HYPERSCRIPTS]"
$ area_prod   = "AREA$HYPERSCRIPTS_PROD:[HYPERSCRIPTS_PROD]"
$ area_test3  = "AREA$HYPERSCRIPTS_TEST3:[HYPERSCRIPTS_TEST3]"
$ area_dev    = "AREA$HYPERSCRIPTS_DEV:[HYPERSCRIPTS_DEV]"
$ area_train  = "AREA$HYPERSCRIPTS_DEV:[HYPERSCRIPTS_DEV]"
$ area_rel    = "AREA$HYPERSCRIPTS_DEV:[HYPERSCRIPTS_DEV]"
$ list_run        = "/''area_prod'/''area_test3'/''area_dev'/''area_train'/''are
a_rel'/"
$
$ ! 6. AutoLog directory.
$ defaultLog   = "''f$trnlnm(""AEQ_SSP"")'" - "]" + ".LOG]"
$! logprod      = "AREA$PROMPROD_AUTOROUTER_LOG:[PROMPROD_AUTOROUTER_LOG]"
$ logprod      = "AREA$AUTOLOG_PROD:[AUTOLOG_PROD]"
$ logtest3     = "AREA$AUTOLOG_TEST3:[AUTOLOG_TEST3]"
$ logdev       = "AREA$AUTOLOG_DEV:[AUTOLOG_DEV]"
$ logtrain     = "AREA$AUTOLOG_TRAIN:[AUTOLOG_TRAIN]"
$ logrel       = "AREA$AUTOLOG_REL:[AUTOLOG_REL]"
$ list_log     = "/''logprod'/''logtest3'/''logdev'/''logtrain'/''logrel'/"
$
$ ! 7. List of detached services to start up. Separate with ":"
$ defaultDet   = ":tp_semy:tp_info:tp_canon:tp_xc_s900_act:"
$ detprod      = defaultDet
$ dettest3     = defaultDet
$ detdev       = defaultDet
$ dettrain     = defaultDet
$ detrel       = ":tlogfeed_lot:tlogfeed_prod:tlogfeed_tables:tp_semy:tp_info:tp
_canon:tp_xc_s900_act:"
$ list_det     = "/''detprod'/''dettest3'/''detdev'/''dettrain'/''detrel'/"
$
$ ! 8. Whether OPTION_AUTO should be set to YES or NO
$ default_auto = "NO"
$ list_auto    = "/YES/YES/YES/YES/YES/"
$
$ ! 9. Fifo directory.  Used for intermediate router files.
$ defFifo = "MBX"
$ list_fifo    = "/''defFifo'/''defFifo'/''defFifo'/''defFifo'/''defFifo'/"
$
$ ! 10. Auto DCOP directory (for WCook) for DCOP/prompt mapping
$ defaultDCOP     = "''f$trnlnm(""AEQ_SSP"")'" - "]" + ".DCOPS]"
$! dcop_prod   = "AREA$AUTOROUTER_HYPERSCRIPTS:[AUTOROUTER_HYPERSCRIPTS.DCOPS]"
$ dcop_prod   = "AREA$HYPERSCRIPTS_PROD:[HYPERSCRIPTS_PROD.DCOPS]"
$ dcop_test3  = "AREA$HYPERSCRIPTS_TEST3:[HYPERSCRIPTS_TEST3.DCOPS]"
$ dcop_dev    = "AREA$HYPERSCRIPTS_DEV:[HYPERSCRIPTS_DEV.DCOPS]"
$ dcop_train  = "AREA$HYPERSCRIPTS_TRAIN:[HYPERSCRIPTS_TRAIN.DCOPS]"
$ dcop_rel    = "AREA$HYPERSCRIPTS_REL:[HYPERSCRIPTS_REL.DCOPS]"
$ list_dcop     = "/''dcop_prod'/''dcop_test3'/''dcop_dev'/''dcop_train'/''dcop_
rel'/"
$
$ ! 11. Automation Statistics
$ defaultStat     = "''f$trnlnm(""AEQ_SSP"")'" - "]" + ".STATS]"
$! stat_prod   = "AREA$PROMPROD_AUTOMATION_STATS:[PROMPROD_AUTOMATION_STATS]"
$ stat_prod   = "AREA$AUTOSTAT_PROD:[AUTOSTAT_PROD]"
$ stat_test3  = "AREA$AUTOSTAT_TEST3:[AUTOSTAT_TEST3]"
$ stat_dev    = "AREA$AUTOSTAT_DEV:[AUTOSTAT_DEV]"
$ stat_train  = "AREA$AUTOSTAT_TRAIN:[AUTOSTAT_TRAIN]"
$ stat_rel    = "AREA$AUTOSTAT_REL:[AUTOSTAT_REL]"
$ list_stat       = "/''stat_prod'/''stat_test3'/''stat_dev'/''stat_train'/''sta
t_rel'/"
$
$ ! 12. Route node name.  In VMS, this is the next part of the mailbox name, the
 root node
$ defaultRouter = "ROUTER"
$ list_router   = "/''defaultRouter'/''defaultRouter'/''defaultRouter'/''default
Router'/''defaultRouter'/"
$
$ ! 13. AutoSpool directory.
$ defaultSpool   = "''f$trnlnm(""AEQ_SSP"")'" - "]" + ".SPOOL]"
$ list_Spool     = "/AEQ_SSP:/AEQ_SSP:/AEQ_SSP:/AEQ_SSP:/AEQ_SSP:/"
$
$ ! 14. AutoBin directory.
$ defaultBin   = "''f$trnlnm(""AEQ_SSP"")'" - "]"
$ list_Bin     = "/AEQ_SSP:/AEQ_SSP:/AEQ_SSP:/AEQ_SSP:/AEQ_SSP:/"
$
$ ! 15. AutoPort number.
$ defaultPort   = "7001"
$ list_Port     = "/''defaultPort'/''defaultPort'/''defaultPort'/''defaultPort'/
''defaultPort'/"
$
$ ! 16. AutoSqlServer .
$ defSQL   = "sql_server"
$ list_SQLserver     = "/''defSQL'/''defSQL'/''defSQL'/''defSQL'/wssc38.na.visha
yint.com/"
$
$ ! 17. MGR account
$ list_mgr    = "/MGRPROMPROD/MGRPROMTEST3/MGRPROMDEV/MGRPROMTRAIN/MGRPROMREL/"
$
$ !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
$
$ ! Get specifics of PROMIS system running on the current node
$ nodename  == f$getsyi("NODENAME")
$ username  == f$edit(f$getjpi("","USERNAME"),"TRIM")
$ promiswhere == f$logical ( "PROMIS_WHERE" )
$ if promiswhere .eqs. "" then promiswhere == "PRIMARY"
$ promissystemname == f$extract( 0, 4, f$logical("PROMIS_SYSTEMNAME") )
$ if promissystemname .eqs. "" then promissystemname == "PROMIS"
$ isMGRPROM = ( f$extract ( 0, 7, username ) .eqs. "MGRPROM" )
$
$ ! Determine whether AutoRouter is ok to run or not
$
$ ! Search for the correct account, node, promis where,promis system
$
$ usersuffix = username - "AUTO" - "MGRPROM"
$
$ allowAutoRouter == 0
$ i = 1
$LOOP:
$
$ user   = f$element(i,"/",list_user)
$ if ( user .eqs. "" .or. user .eqs. "/" ) then $ goto END_LOOP
$
$ node   = f$element(i,"/",list_node)
$ where  = f$element(i,"/",list_where)
$ system = f$element(i,"/",list_system)
$
$ if (  user .eqs. usersuffix .and. -
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
$
$   write sys$output "AutoRouter is allowed to run"
$   if ( isMGRPROM )
$   then
$     write sys$output "Defining group logicals"
$
$     define/group/nolog AUTOLOG          "''f$element(i,"/",list_log)'"
$     define/group/nolog AUTOSTAT         "''f$element(i,"/",list_stat)'"
$     define/group/nolog AUTOSPOOL        "''f$element(i,"/",list_spool)'"
$     define/group/nolog AUTORUN          "''f$element(i,"/",list_run)'"
$     define/group/nolog AUTOFIFO         "''f$element(i,"/",list_fifo)'"
$     define/group/nolog AUTOBIN          "''f$element(i,"/",list_bin)'"
$     define/group/nolog AUTOROUTER       "''f$element(i,"/",list_router)'"
$     define/group/nolog AUTODET          "''f$element(i,"/",list_det)'"
$     define/group/nolog AUTOPORT         "''f$element(i,"/",list_port)'"
$     define/group/nolog AUTOSQLSERVER    "''f$element(i,"/",list_SQLserver)'"
$     define/group/nolog AUTODCOP         "''f$element(i,"/",list_dcop)'"
$     define/group/nolog AUTOMGR          "''f$element(i,"/",list_mgr)'"
$     define/group/nolog OPTION_AUTO      "''f$element(i,"/",list_auto)'"
$     define/group/nolog OPTION_AUTO_LOG YES
$     define/group/nolog MM AEQ_SSP:MM.COM
$     define/group/nolog HS AEQ_SSP:HS.COM
$
$   endif
$
$ else
$
$   write sys$output "AutoRouter is not allowed to run"
$
$ endif
$
$ write sys$output "Username            = ''username'"
$ write sys$output "Nodename            = ''nodename'"
$ write sys$output "System              = ''promissystemname'"
$ write sys$output "Where               = ''promiswhere'"
$ write sys$output "Autolog             = ''f$trnlnm(""AUTOLOG"")'"
$ write sys$output "AutoSpool           = ''f$trnlnm(""AUTOSPOOL"")'"
$ write sys$output "Autorun             = ''f$trnlnm(""AUTORUN"")'"
$ write sys$output "Services            = ''f$trnlnm(""AUTODET"")'"
$ write sys$output "Mailbox prefix      = ''f$trnlnm(""AUTOFIFO"")'"
$ write sys$output "AutoBin             = ''f$trnlnm(""AUTOBIN"")'"
$ write sys$output "Root router         = ''f$trnlnm(""AUTOROUTER"")'"
$ write sys$output "Router port         = ''f$trnlnm(""AUTOPORT"")'"
$ write sys$output "SQL server          = ''f$trnlnm(""AUTOSQLSERVER"")'"
$ write sys$output "AutoStat            = ''f$trnlnm(""AUTOSTAT"")'"
$ write sys$output "AutoDcop            = ''f$trnlnm(""AUTODCOP"")'"
$ write sys$output "AutoMgr             = ''f$trnlnm(""AUTOMGR"")'"
$ write sys$output "OPTION_AUTO         = ''f$trnlnm(""OPTION_AUTO"")'"
$ write sys$output "OPTION_AUTO_LOG     = ''f$trnlnm(""OPTION_AUTO_LOG"")'"
$
$ ! Define some useful symbols
$ xclose :== @AEQ_SSP:stop
$ xopen  :== @AEQ_SSP:start
$ xstop  :== @AEQ_SSP:stop
$ xstart :== @AEQ_SSP:start
$ xmenu  :== @AUTORUN:menu
$ hss    :== $AEQ_SSP:HSS
$ hsp    :== $AEQ_SSP:HSP
$ hse    :== $AEQ_SSP:HS
$
$ exit