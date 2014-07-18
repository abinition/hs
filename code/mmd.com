$! MMD.COM
$!
$! Modifications:
$!
$!   $Log: mmd.com,v $
$!   Revision 1.2  2003-01-16 14:40:47  bergsma
$!   V3.1.0
$!   Added Modification tag.
$!
$!
$ link /nomap -
mep_exe:muchmain+-
aeq_ssp:automan+-
aeq_ssp:automan2+-
aeq_ssp:automan3+-
aeq_ssp:automan4+-
aeq_ssp:autoutil+-
aeq_ssp:batch+-
mep:mepmain/opt+-
wct:wctwrtshr/opt+-
tim:timwrtshr/opt+-
sex:sexgblrw/opt+-
aeq_msg:psetcom/opt+-
prom:promis/lib+-
bld:image/opt
