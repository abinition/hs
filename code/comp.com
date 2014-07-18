$!+
$! COMP.COM - Command procedure to set comp symbol
$!
$! Author:	Michael Bergsma		12/30/97
$!
$!
$! Modifications:
$!
$!   $Log: comp.com,v $
$!   Revision 1.2  2003-01-16 14:40:47  bergsma
$!   V3.1.0
$!   Added Modification tag.
$!
$!
$!-
$!
$! Define FORTRAN compiler options 
$!
$ !!comp_optimize == "/NOOPTIMIZE"
$ !!comp_list == "/LIST"
$ !!comp_debug == "/DEBUG"
$ !!comp_preproc == "NO"
$ !!def sys$welcome "PROMIS Systems Corporation"
$ comp:==@bld:compile
