$! MMM.COM
$!
$! Relink MUCHMAIN.SHR
$!
$!
$! Modifications:
$!
$!   $Log: mmm.com,v $
$!   Revision 1.2  2003/01/16 14:40:47  bergsma
$!   V3.1.0
$!   Added Modification tag.
$!
$!
$ set noon
$ @aeq_ssp:arch
$ linkopts = "/nomap"
$ if ( arch .eqs. "AS_ALPHA" ) then $ linkopts = linkopts + "/nonative"
$ link 'linkopts'/debug -
aeq_ssp:promis_t+-
mep:mepmain/opt+-
wct:wctwrtshr/opt+-
tim:timwrtshr/opt+-
sex:sexgblrw/opt+-
aeq_msg:psetcom/opt+-
prom:promis/lib+-
bld:image/opt
$ exit