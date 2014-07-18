$!
$! Check for ORACLE PRO-C Precompiler
$ sql = 0
$ build = "link/exec="
$ if f$logical( "ORA_RDBMS" ) .eqs. "" .and. -
     f$logical( "ora_tor_mfgp_user_v7" ) .nes. "" then $ @ora_tor_mfgp_user_v7
$ if "''proc'" .nes. "" 
$ then
$   sql = 1
$   build = "lnproc " 
$ endif
$
$ set ver
$ 'build'muchmain.shr mep_exe:muchmain+-
mep:mepmain/opt+-
wct:wctwrtshr/opt+-
tim:timwrtshr/opt+-
sex:sexgblrw/opt+-
aeq_msg:psetcom/opt+-
prom:promis/lib+-
bld:image/opt
$
$ 'build'mepmain.shr mep_exe:mepmain+-
prom:promis/lib+-
bld:image/opt
$
$ !!!copy m*.shr vax2z"PROMISDEV CARAMEL"::AEQ_CRUZ:
