$!+
$! ARCH.COM - Command procedure to set arch symbol
$!
$! Author:	Michael Bergsma		12/30/97
$!
$!
$! Modifications:
$!
$!   $Log: arch.com,v $
$!   Revision 1.2  2003/01/16 14:40:47  bergsma
$!   V3.1.0
$!   Added Modification tag.
$!
$!
$!-
$ ! Determine machine architecture
$ s = f$getsyi("HW_NAME")		! the name of the machine
$ ls = f$length(s)			! the length of the name
$ arch == "AS_VAX"			! assume this is a VAX
$ if f$locate("VAX",s) .eq. ls then -	! search for "VAX" in HW_NAME
  	arch == "AS_ALPHA"  		! sometimes this is an ALPHA
$ exit
