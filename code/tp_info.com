$ ! AEQ_SSP:TP_INFO.COM
$ !
$ ! Usage:	Services AbInitio Protocol messages for PROMIS functions
$ !
$ ! Author:	Michael Bergsma	9/29/94
$ !
$ ! Modifications:
$ !
$ !  $Log: tp_info.com,v $
$ !  Revision 1.2  2003-01-16 14:40:47  bergsma
$ !  V3.1.0
$ !  Added Modification tag.
$ !
$ !	
$ set ver
$ set noon
$
$ set proc/name=TP_INFO
$ if .not. $status then exit
$
$ 
$ if f$logical ( "PROMIS_STATUS" ) .eqs. "STARTING" then $ wait 00:05:00
$ if f$logical ( "PROMIS_STATUS" ) .eqs. "RUNNING"  then $ @aeq_ssp:mm
$
$ exit 
