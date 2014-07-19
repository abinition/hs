$! MMM.COM
$!
$! Relink MUCHMAIN.SHR
$!
$!
$! Modifications:
$!
$!   $Log: mmm.com,v $
$!   Revision 1.3  2009-08-19 14:30:15  bergsma
$!   New PROMIS has less libraries to link
$!
$!   Revision 1.2  2003-01-16 14:40:47  bergsma
$!   V3.1.0
$!   Added Modification tag.
$!
$!
$ set noon
$ @aeq_ssp:arch
$ linkopts = "/nomap"
$ if ( arch .eqs. "AS_ALPHA" ) then $ linkopts = linkopts + "/nonative"
$ link 'linkopts'/nomap/exec=muchmain.shr -
mep_exe:muchmain+-
mep:mepmain/opt+-
wct:wctwrtshr/opt+-
tim:timwrtshr/opt+-
!sex:sexgblrw/opt+-
!aeq_msg:psetcom/opt+-
prom:promis/lib+-
bld:image/opt
$ exit
