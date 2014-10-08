$! INSTALL.COM
$!
$! Installs HyperScript and AutoMan
$!
$! Modifications:
$! 
$!    $Log: install.com,v $
$!    Revision 1.3  2005-03-14 17:02:53  bergsma
$!    EXPUTIL No longer a valid patch
$!
$!    Revision 1.2  2002/11/12 20:13:42  bergsma
$!    Added AEQ: patch
$!
$!
$ on error then goto ERROR
$
$ ! Make sure aspatch.for is compiled and linked
$ set def aeq_ssp:
$ if ( f$search ( "aspatch.exe" ) .eqs. "" ) 
$ then
$   fortran/extend/list aspatch
$   link aspatch
$ endif
$
$ install == 1 
$ patchErrors = 0
$
$ arg:='p1
$ if arg .eqs. "" then arg = "ALL"
$ goto 'arg'
$
$ALL:
$HYPERSCRIPT:
$AUTOMAN:
$!
$AEQ:
$ fileName = "AEQ"
$ fileDir = "AEQ"
$ fileType = "BLD"
$ gosub PATCH
$ if ( patch .eq. 0 ) then patchErrors = patchErrors + 1
$ if arg .eqs. "AEQ" then goto WRAPUP
$
$AEQSTART:
$ fileName = "AEQSTART"
$ fileDir = "AEQ"
$ fileType = "COM"
$ gosub PATCH
$ if ( patch .eq. 0 ) then patchErrors = patchErrors + 1
$ if arg .eqs. "AEQSTART" then goto WRAPUP
$
$AEQSTOP:
$ fileName = "AEQSTOP"
$ fileDir = "AEQ"
$ fileType = "COM"
$ gosub PATCH
$ if ( patch .eq. 0 ) then patchErrors = patchErrors + 1
$ if arg .eqs. "AEQSTOP" then goto WRAPUP
$
$GETTOKEN:
$ fileName = "GETTOKEN"
$ fileDir= "TUT"
$ filetype = "FOR"
$ gosub PATCH
$ if ( patch .eq. 0 ) then patchErrors = patchErrors + 1
$ if arg .eqs. "GETTOKEN" then goto WRAPUP
$
$INQUIRE:
$ fileName = "INQUIRE"
$ fileDir= "TUT"
$ filetype = "FOR"
$ gosub PATCH
$ if ( patch .eq. 0 ) then patchErrors = patchErrors + 1
$ if arg .eqs. "INQUIRE" then goto WRAPUP
$
$UAFDEF:
$ fileName = "UAFDEF"
$ fileDir= "UAF"
$ filetype = "FOR"
$ gosub PATCH
$ if ( patch .eq. 0 ) then patchErrors = patchErrors + 1
$ if arg .eqs. "UAFDEF" then goto WRAPUP
$
$SEARCH:
$ fileName = "SEARCH"
$ fileDir= "UAF"
$ filetype = "FOR"
$ gosub PATCH
$ if ( patch .eq. 0 ) then patchErrors = patchErrors + 1
$ if arg .eqs. "SEARCH" then goto WRAPUP
$ 
$INFO:
$ fileName = "INFO"
$ fileDir= "UAF"
$ filetype = "FOR"
$ gosub PATCH
$ if ( patch .eq. 0 ) then patchErrors = patchErrors + 1
$ if arg .eqs. "INFO" then goto WRAPUP
$
$AUTH:
$ fileName = "AUTH"
$ fileDir= "UAF"
$ filetype = "FOR"
$ gosub PATCH
$ if ( patch .eq. 0 ) then patchErrors = patchErrors + 1
$ if arg .eqs. "AUTH" then goto WRAPUP
$
$TKIN:
$ fileName = "TKIN"
$ fileDir= "LOT"
$ filetype = "FOR"
$ gosub PATCH
$ if ( patch .eq. 0 ) then patchErrors = patchErrors + 1
$ if arg .eqs. "TKIN" then goto WRAPUP
$
$ENTERDATA:
$ fileName = "ENTERDATA"
$ fileDir= "TST"
$ filetype = "FOR"
$ gosub PATCH
$ if ( patch .eq. 0 ) then patchErrors = patchErrors + 1
$ if arg .eqs. "ENTERDATA" then goto WRAPUP
$
$ABORTSTEP:
$ fileName = "ABORTSTEP"
$ fileDir= "LOT"
$ filetype = "FOR"
$ gosub PATCH
$ if ( patch .eq. 0 ) then patchErrors = patchErrors + 1
$ if arg .eqs. "ABORTSTEP" then goto WRAPUP
$
$TRACKOUT:
$ fileName = "TRACKOUT"
$ fileDir= "LOT"
$ filetype = "FOR"
$ gosub PATCH
$ if ( patch .eq. 0 ) then patchErrors = patchErrors + 1
$ if arg .eqs. "TRACKOUT" then goto WRAPUP
$
$OPERMENU:
$ fileName = "OPERMENU"
$ fileDir= "SSP"
$ filetype = "MNU"
$ gosub PATCH
$ if ( patch .eq. 0 ) then patchErrors = patchErrors + 1
$ if arg .eqs. "OPERMENU" then goto WRAPUP
$
$LOTTRMENU:
$ fileName = "LOTTRMENU"
$ fileDir= "LOT"
$ filetype = "MNU"
$ gosub PATCH
$ if ( patch .eq. 0 ) then patchErrors = patchErrors + 1
$ if arg .eqs. "LOTTRMENU" then goto WRAPUP
$
$AEQSSP:
$ fileName = "AEQSSP"
$ fileDir= "AEQ_SSP"
$ filetype = "BLD"
$ gosub PATCH
$ if ( patch .eq. 0 ) then patchErrors = patchErrors + 1
$ if arg .eqs. "AEQSSP" then goto WRAPUP
$
$EXPUTIL:
$! fileName = "EXPUTIL"
$! fileDir= "EQP_CONST"
$! filetype = "FOR"
$! gosub PATCH
$! if ( patch .eq. 0 ) then patchErrors = patchErrors + 1
$! if arg .eqs. "EXPUTIL" then goto WRAPUP
$
$ goto WRAPUP
$
$PATCH_ERROR:
$ write sys$output "Aborting patch operation"
$ exit
$
$ERROR:
$ write sys$output "Install script error occurred"
$ exit
$
$WRAPUP:
$ write sys$output "There were a total of ''patchErrors' patch errors"
$ if patchErrors .gt. 0 then install == 0
$ exit
$
$!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
$PATCH:
$
$ patch = 0
$ source = "''fileDir':''filename'.''filetype'"
$
$ write sys$output "--------------------------------------------------"
$ write sys$output "...patching ''source'"
$
$ ! Save original
$ orig = "AEQ_SSP:''filename'.''filetype'_ORIG"
$ if ( f$search(orig) .eqs. "" ) then copy 'source' 'orig'
$
$ ! Define output files
$ new = "AEQ_SSP:''filename'.''filetype'_NEW"
$ err = "AEQ_SSP:''filename'.''filetype'_ERR"
$
$ ! Multiple patch files may exist
$ patchfilespec = "AEQ_SSP:''filename'.PAT*" 
$LOOP:
$ 
$ ! Delete prio incarnations
$ if ( f$search(new) .nes. "" ) then delete/nolog 'new';*
$ if ( f$search(err) .nes. "" ) then delete/nolog 'err';*
$
$ ! Get next patchfile variation.
$ patchfile = f$search ( patchfilespec, 1 )
$ if ( patchfile .eqs. "" ) then $ goto ENDLOOP
$
$ ! Perform the patch
$ !
$ patch = 1
$ define/nolog as_patch  "''patchfile'"
$ define/nolog as_source "''source'"
$ define/nolog as_output "''new'"
$ define/nolog as_error  "''err'"
$ write sys$output "Patching ''source'"
$ run aeq_ssp:aspatch
$
$ ! Check for errors
$ if ( f$search(err) .nes. "" ) 
$ then
$   write sys$output " "
$   write sys$output "Failed to patch ''source' using patchfile ''patchfile'"
$   write sys$output "1. The source file may already be patched."
$   write sys$output "2. The source file may require a different patch file."
$   write sys$output "The patch failed with the following errors:"
$   type 'err'
$   write sys$output " "
$   write sys$output "The original file ''source' has NOT BEEN CHANGED"
$   write sys$output " "
$   patch = 0
$ else if ( f$search(new) .eqs. "" ) 
$ then
$   write sys$output " "
$   write sys$output "Patched file ''new' cannot be located!!!"
$   write sys$output " "
$   write sys$output "PLEASE CONTACT ABINITION FOR SUPPORT!!!"
$   write sys$output " "
$   write sys$output "The original file ''source' has NOT BEEN CHANGED"
$   write sys$output " "
$   patch = 0
$ else 
$   write sys$output "Updating ''source'"
$   copy/log 'new' 'source'
$   diffile = "AEQ_SSP:''filename'.''filetype'_DIF"
$   diff/output='diffile' 'source'
$   write sys$output "Differences are in ''diffile'"
$   write sys$output " "
$   write sys$output "The file ''source' has BEEN CHANGED"
$   write sys$output " "
$   write sys$output "The original file is ''orig' "
$   write sys$output " "
$   goto ENDLOOP
$ endif
$ endif
$ goto LOOP
$ENDLOOP:
$ 
$ return
