$ ! AEQ_SSP:ROUTER.COM
$ !
$ ! Usage:      Runs the ROUTER which routes AbInitio Protocol messages
$ !
$ ! Author:     Michael Bergsma 7/14/93
$ !
$ ! Modifications:
$ !
$ !     1/11/96 - increase process prio from 4 to 6
$ !
$ !     2/5/97  - turn OPTION_AUTO on only if OPS or DEV on PRIMARY
$ set ver
$ set noon
$
$ if ( f$search ( "aeq_ssp:config.com" ) .nes. "" )
$ then
$   @aeq_ssp:config
$ else
$   @aeq_ssp:config_template
$ endif
$ //@autorun:define_ports
$
$ if ( .not. allowAutoRouter ) then $ exit
$
$ set proc/priv=(SYSNAM,DETACH,TMPMBX,NETMBX,ALTPRI)
$
$RESTART:
$ set proc/prio=6
$ hse :==$aeq_ssp:hss
$ sh time
$ port = f$trnlnm("autoport") 
$ hse -r -t router -n 'port' -f autorun:router.hyp -l autolog:router_start.log
$ show time
$ !!!goto RESTART
$
$ !!!define/group/nolog OPTION_AUTO NO
$
$ exit
