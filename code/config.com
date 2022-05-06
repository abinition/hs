$ ! CONFIG_TEMPLATE.COM
$ !
$ ! *** YOU SHOULD CREATE YOUR OWN CONFIG.COM ***
$ !
$ ! Modifications:
$ !
$ !
$ !     bergsma         8/11/20 Switch to port 7002
$ !
$ !     ryan eckholm	 14-dec-2020 pd-005031 pure migration
$ !	ryan eckholm	5/21/15 itdr-xxxx set autolog/run directories
$ ! 	jane libby	3/30/15 itdr-xxxx activate hitachi migration changes
$ !	ryan eckholm	3/27/15	itdr-xxxx hitachi migration
$ !	ryan eckholm   11/13/13 now pointing to devapp01
$ !	ryan eckholm	9/08/11 pcr-8519 set to devapp03
$ !	ryan eckholm	8/25/11	pcr-8519 turn off tlogfeed
$ !	ryan eckholm	2/12/10	pcr-7964 modified tlog directory locations
$ ! 	$Log: config_template.com,v $
$ ! 	Revision 1.24  2010-01-18 22:21:12  bergsma
$ ! 	no message
$ ! 	
$ ! 	Revision 1.15  2009-11-17 16:07:01  bergsma
$ ! 	Better integration with AUTOSQLSERVER2
$ ! 	
$ ! 	Revision 1.14  2009-10-09 13:26:03  bergsma
$ ! 	Germany - October 2009 - Updates
$ ! 	
$ ! 	Revision 1.13  2008-05-06 02:16:21  bergsma
$ ! 	no message
$ ! 	
$ ! 	Revision 1.12  2005-12-08 03:04:41  bergsma
$ ! 	Added AUTOMGR, AUTOSTAT, AUTODCOP
$ ! 	
$ ! 	Revision 1.11  2005/09/25 20:10:40  bergsma
$ ! 	Up to V3.6.3
$ ! 	Add opcert code.
$ ! 	Changes to function VT2HTML
$ ! 	Bug in EAGAIN waiting for UNIX fifo's or network sockets
$ ! 	AUTOROUTER loigical must be uppercase on VMS
$ ! 	
$ ! 	Revision 1.10  2005/03/07 21:45:35  bergsma
$ ! 	Added OPTION_AUTO, it used to be defined in ROUTER.COM and turned off
$ ! 	there as well.
$ ! 	
$ ! 	Revision 1.9  2005/02/25 18:17:12  bergsma
$ ! 	HS 3.4.5
$ ! 	
$ ! 	Revision 1.8  2005/02/15 07:10:17  bergsma
$ ! 	Added AUTOSQLSERVER
$ ! 	
$ ! 	Revision 1.7  2004/10/16 04:32:45  bergsma
$ ! 	Added AUTOPORT logical.  Created default values for other logicals.
$ ! 	
$ ! 	Revision 1.6  2004/06/13 14:11:40  bergsma
$ ! 	For PROMIS, in the CONFIG_TEMPLATE.COM, an invalid username sets the
$ ! 	automation logicals to default values - JOB only, not GROUP.
$ ! 	
$ ! 	Revision 1.5  2003/02/17 09:53:56  bergsma
$ ! 	Added AUTOBIN
$ ! 	
$ ! 	Revision 1.4  2002/11/22 16:07:56  bergsma
$ ! 	Restore to V3.0.6 copy.
$ ! 	
$ ! 	Revision 1.2  2002/09/03 21:20:08  bergsma
$ ! 	Added AUTOSPOOL logical.
$ ! 	
$ !
$ !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
$ !
$ !  M O D I F Y   T H I S  S E C T I O N  O N L Y
$ !
$ ! There can only be one AUTOROUTER per node.
$ ! This command procedure assumes two nodes, production and development
$ ! Add more nodes as needed.
$ !
$ ! 1. Allowed run account
$! list_user    = "/PROM55DEV/PROMISDEV/PROMISTST/"
$!b pcr-8519
$ list_user = "/PROM58QA1/"
$!!! list_user = "/DONOTRUN/"
$!e pcr-8519 
$ ! 2. Allowed node name. Put "*" if node name does not matter.
$! list_node    = "/*/*/*/"
$ list_node    = "/MES19/"
$
$ ! 3. Primary or Secondary. Put "*" if doesn't matter.
$! list_where   = "/PRIMARY/PRIMARY/PRIMARY/"
$ list_where = "/PRIMARY/"
$
$ ! 4. Allowed promis system name.  Put "*" if promis name does not matter.
$ !    Put in "OFF" or "DISABLED" to prevent automation router and detached
$ !    processes from starting.
$! list_system  = "/OPS/DEV/TST/"
$ list_system = "/PROM58QA1/"
$list_system = "/SC_QA1/"
$
$ ! 5. AutoRun directory, where services are found, including autorouter
$! defaultRun   = "''f$trnlnm(""AEQ_SSP"")'" - "]" + ".RUN]"
$! defaultdev = "''f$trnlnm(""AEQ_SSP"")'" - "]" + ".RUN]"
$! list_run     = "/''defaultRun'/''defaultRun'/''defaultRun'/"
$!b itdr-xxxx
$! list_run = "/$1$DGA128:[TLOGFEED.run]/"
$ list_run = "/$1$DGA323:[TLOGFEED.run]/" 
$!e itdr-xxxx      
$ ! 6. AutoLog directory.
$! defaultLog   = "''f$trnlnm(""AEQ_SSP"")'" - "]" + ".LOG]"  
$! list_log     = "/''defaultLog'/''defaultLog'/''defaultLog'/"
$! list_log = "/$1$DGA128:[tlogfeed.log]/"
$! b itdr-xxxx
$ list_log = "/$1$DGA323:[tlogfeed.log]/"
$!e itdr-xxxx
$
$ ! 7. List of detached services to start up. Separate with ":"
$! defaultDet   = ":tp_info:"
$! list_det     = "/''defaultDet'/''defaultDet'/''defaultDet'/"
$ list_det = "/:tlog_lot2:tlog_tables2:tlog_prod2:tlog_rwo2:tlog_data2:/"
$
$ ! 8. Whether OPTION_AUTO should be set to YES or NO
$ !    This is for AUTOMAN only, it does not affect whether AUTOMATION starts
$ defaultAuto = "NO"
$ list_auto    = "/''defaultAuto'/"
$
$ ! 9. Fifo directory.  In VMS, this is the prefix for mailbox names".
$ defaultFifo = "MBQ"
$ list_fifo    = "/''defaultFifo'/"
$
$ ! 10. Route node name.  In VMS, this is the next part of the mailbox name, the root node
$ defaultRouter = "ROUTED"
$ list_router   = "/''defaultRouter'/"
$
$ ! 11. AutoSpool directory.
$! defaultSpool   = "''f$trnlnm(""AEQ_SSP"")'" - "]" + ".SPOOL]"  
$! list_Spool     = "/''defaultSpool'/''defaultSpool'/''defaultSpool'/"
$!b itdr-xxxx
$! list_spool = "/$1$DGA128:[tlogfeed.spool]/"
$ list_spool = "/$1$DGA328:[tlogfeed.spool]/"
$!e itdr-xxxx
$ ! 12. AutoBin directory.
$ defaultBin   = "''f$trnlnm(""AEQ_SSP"")'" 
$ list_Bin     = "/''defaultBin'/"
$
$ ! 13. AutoPort number.
$ defaultPort   = "7002"  
$ list_Port     = "/''defaultPort'/"
$
$ ! 14. AutoSqlServer .
$ defaultSQLserver = "ussc-qaapp01.sjm.com"
$!  
$ list_SQLserver     = "/''defaultSQLserver'/"
$ list_SQLserver2    = "/''defaultSQLserver'"
$! list_SQLserver3     = "/''defaultSQLserver3'/''defaultSQLserver3'/''defaultSQLserver3'/"
$
$ ! 15. Auto DCOP directory for DCOP/prompt mapping
$ defaultDCOP = "''f$trnlnm(""AEQ_SSP"")'" - "]" + ".DCOPS]"
$ list_dcop     = "/''defaultDCOP'/"
$
$ ! 16. Automation Statistics.  Alternate log directory for ROUTER logs
$ defaultStat     = "''f$trnlnm(""AEQ_SSP"")'" - "]" + ".STATS]"
$ list_stat       = "/''defaultStat'/"
$
$ ! 17. Automation owner of detached processes started by START.COM
$ ! Set to same name as user command procedure.... 
$ list_mgr    = "/[700,2]/"
$ 
$ ! 18. Automation host name
$ list_host =  "/USSC-MES19/"

$ !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
$ 
$ ! Get specifics of PROMIS system running on the current node
$ nodename  == f$getsyi("NODENAME")
$! nodename :== USSC-MES01
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
$ host    =  f$element(i,"/",list_host)
$ 
$ write sys$output "''user' == ''username'?"
$ write sys$output "''where' == ''promiswhere'?"
$ write sys$output "''node' == ''nodename'?"
$ write sys$output "''system' == ''promissystemnam
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
$!b pcr-
$ set proc/priv=GRPNAM
$!e pcr-
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
$! define/group/nolog AUTOSQLSERVER3 	"''f$element(i,"/",list_SQLserver3)'"
$ define/group/nolog AUTOSTAT         	"''f$element(i,"/",list_stat)'"
$ define/group/nolog AUTODCOP         	"''f$element(i,"/",list_dcop)'"
$ define/group/nolog AUTOMGR          	"''f$element(i,"/",list_mgr)'"
$ define/group/nolog AUTOHOST            "''f$element(i,"/",list_host)'"
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
$ write sys$output "Host              = ''f$trnlnm(""AUTHOSTG"")'"
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
$ xmenu  :== @AUTORUN:menu
$ hss    :== $AEQ_SSP:HSS
$ hsp    :== $AEQ_SSP:HSP
$ hse    :== $AEQ_SSP:HS
$ dird	 :== dir/size/date=mod
$ exit
