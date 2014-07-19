$!+
$! LINK_HS.COM -  Command procedure to link HS.EXE
$!
$! Output:
$!
$!	AEQ_SSP:HS.EXE
$!
$! Author:	Michael Bergsma		11/7/95
$!
$!
$! Modifications:
$!
$!   $Log: link_hs.com,v $
$!   Revision 1.5  2009-12-08 21:01:50  bergsma
$!   Differentiate between hsp and hss
$!
$!   Revision 1.4  2009-08-24 20:41:39  bergsma
$!   Make sure FILTABLES is defined
$!
$!   Revision 1.3  2005-04-03 17:36:19  bergsma
$!   HS 3.54  (FIX OF FLOATING POINT OVERFLOW IN TLOGFEED).
$!   1. Don't delete LISting files.
$!   2. PackStart in aeqssp_autofil not being cleared - was causing an
$!   unpack operation when not required.
$!
$!   Revision 1.2  2003/01/16 14:40:47  bergsma
$!   V3.1.0
$!   Added Modification tag.
$!
$!-
$ on error then $ goto ERROR
$ link_hs == 0
$ @aeq_ssp:arch
$ @aeq_ssp:ccc AS_PROMIS
$
$! Check that global sections are installed.
$ if ( f$trnlnm ( "FILTABLES" ) .eqs. "" ) then @PDB:GLOBAL INSTALL 
$!
$! Link the standalone image
$!
$ library hs_promis.olb/extract=(HS)/out=hsp.obj
$ 'build'/exec=hsp.exe/map hsp.obj+hs_promis/lib+hs.opt/opt+prom:promis/lib+bld:image/opt
$
$ ! HS.EXE will be the one with the PROMIS hooks.
$ copy hsp.exe hs.exe
$
$ link_hs == 1
$ exit
$!
$ERROR:
$ link_hs == 0
$exit:
