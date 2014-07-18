!*****************************************************************************!
!                    AUTOMAN Software Source Code.                            !
!                                                                             !
!          ***** Copyright: (c) 1993 Ab Initio Software                       !
!          ***** All rights reserved - Licensed Material.                     !
!          ***** Program property of Ab Initio Software                       !
!                                                                             !
!*****************************************************************************!
!++ BATCH.FOR

! Externally Callable Routines:

!       aeqSsp_batch_trackin            ! Menu function entry point

! For Internal Use Only:

!       aeqSsp_batch_andGo              ! Construct WIP, then do main 
!       aeqSsp_Batch_main               ! Main batch function
!       aeqSsp_batch_getWip             ! Get WIP at location
!       aeqSsp_batch_getEqpid           ! Get batch equpipment
!       aeqSsp_batch_getBatch           ! Construct batch lots
!       aeqSsp_batch_addBatch           ! Add a lot to the batch
!       aeqSsp_batch_initRecipe         ! Initialize the batch recipe
!       aeqSsp_batch_getParms           ! Get batch parameters
!       aeqSsp_batch_checkParms         ! Check batch parameters
!       aeqSsp_batch_getMaxProcCap      ! Get maximum processing capacity
!       aeqSsp_batch_testLot            ! Test if a lot can be added
!       aeqSsp_batch_dispatch           ! Display a dispatch list
!       aeqSsp_batch_selectRecp         ! Selects alternate recipes
!       aeqSsp_batch_listCount          ! List lots and their sizes
!       aeqSsp_batch_listRecipe         ! List lots and their recipes
!       aeqSsp_batch_log                ! Log and display a message
!       aeqSsp_batch_handler            ! Handler for EXIT and QUIT
!
! Modifications:
!
!    $Log: batch.for,v $
!    Revision 1.6  2003/06/05 21:43:32  bergsma
!    In the batch function, fix the problem whereby the OPTION_AUTO process
!    logical gets set to NO and then it is never deleted, so that the next
!    time the batch function is run, automation does no come up
!
!    Revision 1.5  2003/01/16 14:32:07  bergsma
!    V3.1.0
!    Added Modification tags.
!
!

!+      AEQSSP_BATCH_TRACKIN
!
        subroutine aeqSsp_Batch_trackin

        implicit none

! Description:

        ! Perform batch trackin.  This function is called from the PROMIS
        ! "batch into" function.  It allows the user to pick one or more
        ! lots that are allowed to be batched together, then calls the
        ! Lot_Tkin_TrackIn function to track-in each lot. Since Lot_Tkin_TrackIn
        ! has no argument list, they must be supplied via the tut input buffer
        ! prior to each call.

! Include Files:

        include 'FIL:TYPDEF'

! Routines used:

        external        uaf_wrkCtr_CurWctId 
        external        aeqSsp_batch_andGo
        external        tut_output

! Author:       Mike Bergsma            9/93
!               Ab Initio Software

! Modifications:

! Local variables:

        character*(TYP__LOCATIONIDSZ) curLocation

        ! check that this terminal is attached to a location
        call Uaf_WrkCtr_CurWctId ( curLocation )

        if ( curLocation .eq. ' ' ) then

          call Tut_Output(      'Error: this terminal is not currently '//
     &                          'attached to a location' )

        else

          ! Gather the WIP information from the WCT GLOBAL area and 
          ! perform the batch function
          call aeqSsp_batch_andGo( curLocation )

        endif


        return

        end

!++ AEQSSP_BATCH_ANDGO
!
        subroutine aeqSsp_batch_andGo( curLocation )

        implicit none                   ! force declaration of variables

! Description:

        ! Gathers information for all lots waiting at <curLocation>, then
        ! calls aeqSsp_batch_main to construct and execute a batch lot.

!   Include files:

        include 'FIL:TYPDEF'
        include 'FIL:FILDEF'
        include 'PAR:LOTSTDEF'
        include 'WCT:WIPBUFCOM'

! Arguments:

        character*(*)   curLocation                             ! [R]
        ! - workcenter location 

! Routines used:

        external        Wct_WipAccess_NumLotsInList 
        external        Mdl_Dynmem_Real4Vector
        external        Mdl_Dynmem_CharVector
        external        Mdl_Dynmem_ByteVector
        external        aeqSsp_batch_main
        

! Author:       Mike Bergsma            9/93
!               Ab Initio Software

! Modifications:
!       Paul R. Neveu   2/13/2001
!       Add Advanced Equipment functionality for
!       Capabilities and Constraints Checking


! Local Variables:

        integer*4       wipCount

        call Wct_WipAccess_numLotsInList ( wipCount )

        ! Dynamically allocate memory for WIP information, then call
        ! aeqSsp_batch_main to construct and execute batch.
        call Mdl_DynMem_CharVector ( TYP__LOTIDSZ, wipCount,    ! wipLotid
     &       Mdl_DynMem_CharVector, TYP__RECPIDSZ, wipCount,    ! wipRecpid
     &       Mdl_DynMem_CharVector, TYP__EQPTYPSZ, wipCount,    ! wipEqpType
     &       Mdl_DynMem_CharVector, TYP__EQPIDSZ, wipCount,     ! wipEqpId
     &       Mdl_DynMem_CharVector, TYP__CAPABILITYSZ, wipCount,     ! wipCapability !Add for Advanced Equipment
     &       Mdl_DynMem_Real4Vector, wipCount,                  ! wipMainQty
     &       Mdl_DynMem_ByteVector, wipCount,                   ! wipState
     &       Mdl_DynMem_ByteVector, wipCount,			! wipFlag
     &       aeqSsp_batch_main, curLocation )                   ! target

        return

        end
 
!++ AEQSSP_BATCH_MAIN

        subroutine aeqSsp_Batch_main(   curLocation,
     &                                  wipLotid,
     &                                  wipRecpid,
     &                                  wipEqpType,
     &                                  wipEqpId,
     &                                  wipCapability,		! Added for Advanced Equipment
     &                                  wipCurMainQty,
     &                                  wipState,
     &                                  wipFlag )

        implicit none                   ! force declaration of variables

! Description:

        ! 1. Build a list of lots waiting and running from the WCT global for
        !    the current location.
        ! 2. Get a list of all equipment ids that are available at the location.
        ! 3. Prompt for an equipment id, then eliminate all lots from the list
        !    that cannot track-in to or not already running at that equipment id.  
        ! 4. If automatic selection is required, fetch the first list of batch
        !    lots waiting for the equipment type from the dispatch list and 
        !    display this list to the user. THIS IS AN INFORMATIONAL STEP ONLY!!
        ! 5. Prompt for first batch lot. Get the recipe id and parameter values.
        ! 6. Perform batch lot loop:
        !    Do while ( ( lot count < equipment capacity ) and ( not END ) )
        !       Prompt for ith lot from list or END
        !       If recipe or recipe parameters match then 
        !         Add lot to batch.  Remove lot from list.
        !       Else 
        !         Tell user 
        !    enddo
        !    Tell what capacity was reached.
        !
        ! 7. Call the lot_tkin_trackin function once for each lot in the batch.
        !

! Include files:

        include 'FIL:FILDEF'
        include 'FIL:TYPDEF'
        include 'LOT:ACTLBUFF'
        include 'LOT:RECPBUFF'
        include 'GUT:GUTDEF'
        include 'TBL:TBLDEF'
        include 'TUT:TUTDEF'
        include 'PAR:LOTSTDEF'
        include 'PAR:PRODSTDEF'
        include 'AEQ_SSP:AUTODEF'
        include 'AEQ_SSP:BATCHDEF'
        include 'FIL:PRCDPDEF'
        include 'FIL:PINSPDEF'
        include 'FIL:PIARPDEF'
        include 'FIL:EQPSDEF'
        include 'FIL:EQPCDEF'
        include 'FIL:SCHDDEF'
        include 'FIL:FUTADEF'
        include 'FIL:TESTDEF'
	include 'FIL:OPERPDEF'
        include 'TBL:EQSTCOM'

! Common areas:

        record /batch_struct/           batch
        common /batch_common/           batch


! Arguments:

        character*(*)           curLocation                             ! [R]
        ! - current workcenter location

        character*(*)           wipLotid(*)                             ! [R]
        ! - dynamic array of lot ids from WCT global section

        character*(*)           wipRecpid(*)                            ! [R]
        ! - dynamic array of lot recipes from WCT global section

        character*(*)           wipEqpType(*)                           ! [R]
        ! - dynamic array of lot equipment types from WCT global section

        character*(*)           wipEqpId(*)                             ! [R]
        ! - dynamic array of lot equipment ids from WCT global section

        character*(*)           wipCapability(*)                        ! [R]  ! Added for Advanced Equipment
        ! - dynamic array of lot Capability Requirements from WCT global section

        real*4                  wipCurMainQty(*)                        ! [R]
        ! - dynamic array of lot sizes

        byte                    wipState(*)                             ! [R]
        ! - dynamic array of lot state (RUNNING, WAITING)

        byte                    wipFlag(*) 				! [R]
        ! - dynamic array of flags used later for sorting

! Routines used:

        integer*4       aeqSsp_batch_getWip
        logical*4       gut_getyesno
        integer*4       gut_trimblks
        external        tut_output
        integer*4       fil_open
        integer*4       fil_getRec
        external        fil_closeall
        logical*4       wct_wipAccess_getLotData 
        logical*4       Ssp_Option_EqpBsyEnabled 
        integer*4       lot_rdRecpRec_genRecord 
        external        aeqSsp_batch_handler
        integer*4       aeqSsp_autoutil_strip 
        integer*4       aeqSsp_autoUtil_left_justify

! Author:       Mike Bergsma            9/93
!               Ab Initio Software

! Modifications:
!	Mike Bergma     2/23/2001
!       Restrict BATCH function if logical name OPTION_BATCH_RESTRICTED defined
!       to automated equipment only...
!
!       Paul R. Neveu   2/13/2001
!       Add Advanced Equipment functionality for
!       Capabilities and Constraints Checking
!
!       Mike Bergsma    28-Feb-94
!       - Erick Jordan created a lot override for parameter $AUTO_INITPARMS 
!         which included in it a parameter that was not in the recipe.  
!         Thus, it is possible that there can exist batch parameters that are 
!         not defined in the first recipe. In this case, the parameter should
!         be inserted into the recipe before the automation routines are 
!         called.
!
!       Mike Bergsma    02-Jan-95
!       - log (temporarily) the batch track-in
!
!       Mike Bergsma    04-Aug-95
!       - stop using PROMIS log facility, now do logging just
!         like in the aeqSsp_automan_* routines.
!
!       Mike Bergsma    29-Mar-96
!       - was still logging to BATCH_*, not AUTO_*
!
!       Mike Bergsma    18-Nov-96
!       - move lock for EQPS record after first lot is entered
!
!       Mike Bergsma    06-Nov-02
!       - open OPER record, needed by tst_recputil_fixopdesc
!
!
! Local variables:

        integer*4       status,
     &                  i,j,
     &                  wipCount,
     &                  wipCountWaiting,
     &                  wipCountRunning,
     &                  logFileLen,
     &                  bufferLen,
     &                  eqpWipCount

        logical*4       found

        character*1     answer
        character*2     intstr
        character*60    buffer

        character*(TYP__EQPTYPSZ)       eqpType
        character*(TYP__EQPIDSZ)        eqpId
        character*(TYP__CAPABILITYSZ)   eqpCapability  ! Added For Advanced Equipment Capability Checking
        character*(TYP__EQPIDSZ)        logFile


        character*(TYP__STAGEIDSZ)      lotStageId
        character*(TYP__LOCATIONIDSZ)   lotLocId
        character*(1)                   lotState
        integer*4                       lotPcode
        integer*4                       lotKtime
        integer*4                       lotQtime

	integer*4	bufferlen, eqpidlen
	character*60	buffer
        character*(TYP__EQPIDSZ)        strip_eqpId

! Code:

        ! Open required files
        call fil_check ( fil_open ( FIL__ACTL, FIL__SHARE ) )
        call fil_check ( fil_open ( FIL__EQPS, FIL__READONLY ) )
        call fil_check ( fil_open ( FIL__EQPC, FIL__READONLY ) )  !added for Advanced Equipment
        call fil_check ( fil_open ( FIL__RECP, FIL__READONLY ) )
        call fil_check ( fil_open ( FIL__PRCD, FIL__READONLY ) )
        call fil_check ( fil_open ( FIL__PINS, FIL__READONLY ) )
        call fil_check ( fil_open ( FIL__PIAR, FIL__READONLY ) )
        call fil_check ( fil_open ( FIL__SCHD, FIL__READONLY ) )
        call fil_check ( fil_open ( FIL__FUTA, FIL__READONLY ) )
	    call fil_check ( fil_open ( FIL__OPER, FIL__READONLY ) )
        call fil_check ( fil_open ( FIL__TEST, FIL__READONLY ) )

        ! Get list of all lots waiting and running at current location
        ! The waiting and running lots are not sorted in the list.
        wipCount = aeqSsp_batch_getWip  (       curLocation,
     &                                          wipLotid,
     &                                          wipRecpid,
     &                                          wipEqpType,
     &                                          wipEqpId,
     &                                          wipCapability,		!Added for Advanced Equipment Capability Checking
     &                                          wipCurMainQty,
     &                                          wipState,
     &                                          wipFlag,
     &                                          wipCountWaiting,
     &                                          wipCountRunning )

        if ( wipCountWaiting .eq. 0 ) then

          call tut_output ( ' ' )
          call tut_output(      'No lots are waiting at location '//
     &                          curLocation )
          return

        endif

        ! Prompt for an available equipment id at this location
        ! The wip* array is sorted so that lots waiting for the equipment
        ! type selected are first, then lots running on the equipment id 
        ! selected are second (the first running lot is at location
        ! wipLotid(wipCountWaiting+1)
        call aeqSsp_batch_getEqpid (    curLocation,
     &                                  wipCount,
     &                                  wipLotid,
     &                                  wipRecpid,
     &                                  wipEqpType,
     &                                  wipEqpId,
     &                                  wipCapability,		!Added for Advanced Equipment Capability Checking
     &                                  wipCurMainQty,
     &                                  wipState,
     &                                  wipFlag,
     &                                  eqpType,
     &                                  eqpid,
     &                                  wipCountWaiting,
     &                                  wipCountRunning )

	! Determine if batch function is allowed
	call Lib$Sys_TrnLog( 'OPTION_BATCH_RESTRICTED', bufferLen, buffer,, )
	if ( buffer(:bufferLen) .eq. 'YES' ) then
	  strip_eqpid = eqpid 
          call aeqSsp_autoUtil_strip( strip_eqpid )
          eqpidLen = aeqSsp_autoUtil_left_justify( strip_eqpid )
	  call Lib$Sys_TrnLog( 'AUTO_IN_'//strip_eqpid(:eqpidLen), bufferLen, buffer,, )
	  if ( buffer(:bufferLen) .eq. 'AUTO_IN_'//strip_eqpid(:eqpidLen) ) then
	    ! Must not be defined.  
	    call tut_output ('Batch Into Step function is not allowed with non-automated equipment '//eqpid )
            return 
	  endif
	endif

        ! Lock the equipment status record.
!       eqps.eqpId = eqpid
!       eqps.recType = 'C'
!       eqps.serialNo = 0
!
!       Change LOCK to NOLOCK.  As the equipment status is now checked
!       for each lotid, there is no longer any benefit to locking it here.
        call Eqp_EqpsUtil_MakeCurKey ( eqps, eqpid )
        status = fil_getrec (   FIL__EQPS, EQPS__EQPSKEY,
     &                          eqps.eqpsKey, FIL__CMP_EQ, 
     &                          FIL__NOLOCK, eqps.buffer )
!     &                         FIL__LOCK, eqps.buffer )
!
!
!       if ( status .eq. FIL__RLK ) then
!         call tut_output ( ' ' )
!         call tut_output ( 'Cannot lock equipment for batch function.' )
!         call tut_output ( 'The equipment is already locked by another user.' )
!         return          
!       else
          call fil_check ( status )
!       endif

! This section moved to addBatch.  
!       call Tbl_Eqstcd_Find(   eqps.status, 
!     &                         answer,
!     &                         EQST_T_AVAILNORMLOT, 
!     &                         EQST_S_AVAILNORMLOT )
!
!       if ( answer .eq. 'F' ) then
!         call tut_output ( ' ' )
!         call tut_output ( 
!     &         'Cannot perform batch because equipment is not available.' )
!         call tut_output ( 
!     &         'The equipment is in state '//
!     &         eqps.status(:gut_trimblks(eqps.status)) )
!         return
!       endif

        call tut_output ( ' ' )
!!!     if ( gut_getyesno ( 
!!!  &  'Do you want to preview the next batch from the dispatch list?' ) ) 
!!!  &    call aeqSsp_batch_dispatch ( curLocation, eqpType )

        ! Initialize the batch
        batch.lotCount = 0
        batch.firstLot = 0
        batch.numParms = 0
        batch.isAddtoBatch = .false.
        batch.template = ' '
        batch.templateLotType = ' '
        batch.title = ' '
        batch.eqpid = eqpid
        batch.eqptype = eqptype
        batch.eqpCapability = ' '  ! Don't know yet
        batch.eqpStatus = eqps.status
        batch.curLocation = curLocation
        batch.mainRecpid = ' '
        batch.eqpBsyEnabled = Ssp_Option_EqpBsyEnabled ()
        batch.capAttr = ' '
        batch.basis = 'wafers'
        batch.maxProcCap = 0.0
        batch.loadSize = 0.0
        batch.remCap = 0.0
        batch.numParms = 0
        call Uaf_User_CheckName ( batch.userId )

        ! Translate switch OPTION_AUTO_LOG
        call Lib$Sys_TrnLog( 'OPTION_AUTO_LOG', bufferLen, buffer,, )
        if ( buffer(:bufferLen) .eq. 'YES' ) then
          batch.log = .true.
        else
          batch.log = .false.
        endif
        batch.log = batch.log

        logFile = batch.eqpid
        logFileLen = aeqSsp_autoutil_strip ( logFile )
        batch.saveOptionLen = 0
        batch.sendEvent = .false.

        if ( batch.log ) then
          call aeqSsp_autoUtil_openLog (        
     &          'AUTOLOG:AUTO_IN_'//logFile(:logFileLen)//'.TMP' )
          call aeqSsp_batch_log (       batch.log, .false.,
     &          '+++ Begin Batch ++++++++++++++++++++++++++++++++++++++' )
        endif

        ! Declare handler for ^Z, ^C, EXIT, QUIT, or forced tut_exit().
        !
        !       1. Alert user that one or more lots were not tracked-in
        !       2. Deassign OPTION_AUTO logical 
        !       3. Close log file.
        !
        call Lib$Establish( aeqSsp_batch_handler )

        ! Construct the batch
        call aeqSsp_batch_getBatch (    wipCount,
     &                                  wipLotid,
     &                                  wipRecpid,
     &                                  wipEqpType,
     &                                  wipEqpId,
     &                                  wipCapability,		!Added for Advanced Equipment Capability Checking
     &                                  wipCurMainQty,
     &                                  wipState,
     &                                  wipFlag,
     &                                  wipCountWaiting,
     &                                  wipCountRunning )

        ! Check to see if a batch of lots was created
        if ( batch.lotCount .eq. 0 ) then

          ! No lots were selected
          call aeqSsp_batch_log (       batch.log, .true.,
     &          'No batched lots were selected for equipment '//eqpid )

        elseif ( batch.firstLot .eq. 0 ) then

          ! Only DUMMY lots were selected.
          call aeqSsp_batch_log (       batch.log, .true.,
     &          'Only DUMMY lots were selected for the batch.' )
          call aeqSsp_batch_log (       batch.log, .true.,
     &          'A batch cannot be created for equipment '//eqpid )

        else

          ! Display the batch
          call tut_output (
     &          'You have selected the following lots for batch track-in:'//
     &          TUT_T_BLINK//TUT_T_BOLD )
          call aeqSsp_batch_listCount(  batch.basis,
     &                                  batch.lotCount,
     &                                  batch.lotid,
     &                                  batch.numComps )
          call tut_output ( TUT_T_STANDARD )
          
          if ( .not. gut_getyesno ( 'Proceed with batch track-in?' ) ) then

            call aeqSsp_batch_log (     batch.log, .true.,
     &          'Batch track-in will not be performed' )

          else

            ! Re-read the main recipe record if necessary
            if ( batch.mainRecpid .ne. recp.recpid )
     &        call fil_check( lot_rdRecpRec_genRecord (batch.mainRecpid,recp))

            ! Update the recpid parameter values to reflect the ACTL overrides
            ! from the first lot.
            do i=1,batch.numParms

              found = .false.   

              j = 1
              do while ( j .le. recp.numParms .and. .not. found )
                if ( recp.parmName(j) .eq. batch.parmName(i) ) then
                  recp.parmVal(j) = batch.parmVal(batch.firstLot,i)
                  found = .true.
                else
                  j = j + 1
                endif   
              enddo

              ! If we didn't find it, then insert it.
              if ( .not. found ) then
                j = recp.numParms + 1
                recp.parmName(j) = batch.parmName(i) 
                recp.parmVal(j) = batch.parmVal(batch.firstLot,i)
                recp.numParms = j
              endif

            enddo

            ! CLose batch log session temporarily so as not to
            ! intefere with subsequent automation logging.
            if ( batch.log ) call aeqSsp_autoUtil_closeLog()

            ! Call the automation routines.
            call aeqSsp_automan_batch ( batch )

            ! Re-open log session
            if ( batch.log ) 
     &        call aeqSsp_autoUtil_openLog (    
     &                  'AUTOLOG:AUTO_IN_'//logFile(:logFileLen)//'.TMP' )

            ! Disable automation 
            ! Save any local (process) value of logical OPTION_AUTO
            call Lib$Sys_TrnLog(        'OPTION_AUTO',
     &                                  batch.saveOptionLen,
     &                                  batch.saveOption,
     &                                  'LNM$PROCESS', )

		    if ( batch.saveOption(:batch.saveOptionLen) .eq. 'NO_BATCH' ) then

			  ! Should have been deassigned from last batch function.  Don't save it.
			  batch.saveOptionLen = 0

			endif

            ! Set new local (process) value of OPTION_AUTO to NO_BATCH
            call Lib$set_logical(       'OPTION_AUTO',
     &                                  'NO_BATCH',
     &                                  'LNM$PROCESS' )

            ! Temporarily turn off pagewait     
            call Tut_SetNoWait ()

            ! At this point, if something goes wrong, we want to
            ! send an event
            batch.sendEvent = .true.

            ! Read lock the PROMIS internal tables
            ! - prevents problems when table install occurs
            call Tbl_Util_Rlock()
	    call Tbl_Util_Unlock() 

            ! Perform the track-ins
            do i=1,batch.lotCount

              ! Do not track in dummy lots
              if ( batch.lotid(i) .ne. 'DUMMY' ) then

                buffer = batch.lotid(i)
                bufferlen = gut_trimblks ( buffer )

                ! Fill tut buffer with equipment id and lotid
                if ( batch.hasAltRecipe(i) ) then       
                  if ( batch.numAltRecipe(i) .eq. 0 ) then
                    buffer(bufferLen+1:) = ',,'//eqpid
                    bufferlen = gut_trimblks ( buffer )
                  else
                    write ( intstr, '(I2)' ) batch.numAltRecipe(i)
                    buffer(bufferLen+1:) = ','//intstr//','//eqpid
                    bufferlen = gut_trimblks ( buffer )
                  endif
                else
                  buffer(bufferLen+1:) = ','//eqpid
                  bufferlen = gut_trimblks ( buffer )
                endif

                call tut_unget( buffer(:bufferLen) )

                call tut_clear()

                call aeqSsp_batch_log ( batch.log, .true.,
     &          TUT_T_BLINK//TUT_T_BOLD//
     &          '...attempting to automatically track in '//batch.lotid(i) )

                ! Call the function
                call Lot_Tkin_TrackIn()

                ! Check if the lot was tracked in successfully.  If not
                ! then panic appropriately.
                call Wct_WipAccess_GetLotData ( batch.lotid(i),
     &                                          lotLocId, 
     &                                          lotstate, 
     &                                          lotpcode,
     &                                          lotktime, 
     &                                          lotqtime, 
     &                                          lotstageid )

                if ( lotState .eq. PAR_LOTSTDEF_CS_W_RSTR .or.
     &               lotState .eq. PAR_LOTSTDEF_CS_W_RSTR_H_CTL .or.
     &               lotState .eq. PAR_LOTSTDEF_CS_W_RSTR_H_USE ) then

                  call aeqSsp_batch_log (       batch.log, .true.,
     &                  'Tracked in lot '//batch.lotid(i) )

                  batch.isTrackedIn(i) = .true.

                else

                  call aeqSsp_batch_log (       batch.log, .true.,
     &                  'Failed to track in lot '//batch.lotid(i) )
                  
                  ! Lot was not tracked in.  Abort function. 
                  call aeqSsp_batch_sendEvent ()
                  call tut_exit()

                endif

              endif
            enddo

            ! Display lotids tracked in
            if ( batch.lotCount .gt. 0 ) then
              call aeqSsp_batch_log ( batch.log,  .true.,
     &          'The following lots have been batched and tracked-in:'//
     &          TUT_T_BLINK//TUT_T_BOLD )
              call aeqSsp_batch_listCount(      batch.basis,
     &                                          batch.lotCount,
     &                                          batch.lotid,
     &                                          batch.numComps )
              call aeqSsp_batch_log ( batch.log,  .true., TUT_T_STANDARD )
            endif

            ! Restore value of process logical OPTION_AUTO if prev defined
            if ( batch.saveOptionlen .gt. 0 ) then
              if ( batch.saveOption(:batch.saveOptionLen) .ne. 'OPTION_AUTO' .and.
     &			   batch.saveOption(:batch.saveOptionLen) .ne. 'NO_BATCH' ) then
                call lib$set_logical(   'OPTION_AUTO',
     &                                  batch.saveOption(:batch.saveOptionLen),
     &                                  'LNM$PROCESS' )
              else        
                call lib$delete_logical('OPTION_AUTO', 'LNM$PROCESS' )
              endif
            endif
 
            ! Restore pagewait
            call Tut_ReStrWait ()               

          endif

        endif

        ! Close all files. 
        call fil_closeall()

        ! Close the log file
        if ( batch.log ) then
          call aeqSsp_batch_log (       batch.log, .false.,
     &          '+++ End Batch ++++++++++++++++++++++++++++++++++++++++' )
          call aeqSsp_autoutil_closeLog ( )
        endif

        return

        end


!++ AEQSSP_BATCH_GETWIP
!
        integer*4 function aeqSsp_batch_getWip( curLocation,
     &                                          wipLotid,
     &                                          wipRecpid,
     &                                          wipEqpType,
     &                                          wipEqpId,
     &                                          wipCapability,		!Added for Advanced Equipment Capability Checking
     &                                          wipCurMainQty,
     &                                          wipState,
     &                                          wipFlag,
     &                                          wipCountWaiting,
     &                                          wipCountRunning )

        implicit none

! Description:

        ! Get WCT GLOBAL information for all WIP that is in a waiting or
        ! running state at <curLocation>.

! Include files:

        include 'FIL:TYPDEF'
        include 'WCT:WCTDEF'
        include 'WCT:WIPLOTCOM'
        include 'PAR:LOTSTDEF'

! Function value:

        ! number of lots found

! Arguments:

        character*(*)           curLocation                             ! [R]
        ! - current workcenter location

        character*(*)           wipLotid(*)                             ! [W]
        ! - dynamic array of lot ids from WCT global section

        character*(*)           wipRecpid(*)                            ! [W]
        ! - dynamic array of lot recipes from WCT global section

        character*(*)           wipEqpType(*)                           ! [W]
        ! - dynamic array of lot equipment types from WCT global section

        character*(*)           wipEqpId(*)                             ! [W]
        ! - dynamic array of lot equipment ids from WCT global section

        character*(*)           wipCapability(*)                        ! [W]   ! Added for Advanced Equipment Capability
        ! - dynamic array of lot equipment ids from WCT global section

        real*4                  wipCurMainQty(*)                        ! [W]
        ! - dynamic array of lot sizes from WCT global section

        byte                    wipState(*)                             ! [W]
        ! - dynamic array of lot state (RUNNING, WAITING)

        byte                    wipFlag(*)                              ! [W]
        ! - dynamic array of lot condition - initialized to 0

        integer*4               wipCountWaiting                         ! [W]
        ! - number of waiting lots

        integer*4               wipCountRunning                         ! [W]
        ! - number of running lots

! Routines used:

        logical*4       Wct_LockTable_ReadLock          ! Read lock table
        external        Wct_LockTable_ReleaseLocks      ! Unlock table

! Author:       Mike Bergsma            9/93
!               Ab Initio Software

! Modifications:
!       Paul R. Neveu   2/13/2001
!       Add Advanced Equipment functionality for
!       Capabilities and Constraints Checking

! Local variables:

        integer*4       i,qty

! Code:

100     continue

        ! Read lock the WIP table

        if ( .not. Wct_LockTable_ReadLock () ) then

          ! Deadlock detected - try again

          call Wct_LockTable_ReleaseLocks ()
          go to 100

        endif

        ! Search the used portion of the table for all waiting lots at the 
        ! specified location

        qty = 0
        wipCountWaiting = 0
        wipCountRunning = 0

        do i = 1, wct_wipLotCom_maxIndexUsed

          if (  wct_wipLotCom_location(i) .eq. curLocation .and.
     &          ( wct_wipLotCom_state(i) .eq. PAR_LOTSTDEF_CS_W_RQUE .or.
     &            wct_wipLotCom_state(i) .eq. PAR_LOTSTDEF_CS_W_LQUE .or.
     &            wct_wipLotCom_state(i) .eq. PAR_LOTSTDEF_CS_W_RSTR ) ) then

            ! Found a lot at <curLocation> in wait or running state.

            ! Copy information
            if ( wct_wipLotCom_state(i) .eq. PAR_LOTSTDEF_CS_W_RSTR ) then
              wipCountRunning = wipCountRunning + 1     
            else
              wipCountWaiting = wipCountWaiting + 1     
            endif

            qty = qty + 1
            wipLotid(qty) = wct_wipLotCom_lotid(i)
            wipRecpid(qty) = wct_wipLotCom_recpid(i)
            wipEqpType(qty) = wct_wipLotCom_eqptype(i)
            wipEqpId(qty) = wct_wipLotCom_eqpid(i)
            wipCapability(qty) = wct_wipLotCom_capability(i)    !Added for Advanced Equipment Capaibility Checks
            wipCurMainQty(qty) = wct_wipLotCom_curMainQty(i)
            wipState(qty) = ICHAR ( wct_wipLotCom_state(i) )
            wipFlag(qty) = 0

          endif

        enddo

        ! Return number of lots retrieved
        aeqSsp_batch_getWip = qty

        ! Unlock the table

        call Wct_LockTable_ReleaseLocks ()

        return
        end

!++ AEQSSP_BATCH_GETEQPID 
!
        subroutine aeqSsp_batch_getEqpid (      curLocation,
     &                                          wipCount,
     &                                          wipLotid,
     &                                          wipRecpid,
     &                                          wipEqpType,
     &                                          wipEqpId,
     &                                          wipCapability,	! added for Advanced Equipment Capability Checking
     &                                          wipCurMainQty,
     &                                          wipState,
     &                                          wipFlag,
     &                                          eqpType, 
     &                                          eqpid,
     &                                          wipCountWaiting,
     &                                          wipCountRunning )

        implicit none

! Description:

        ! Prompt for an equipment id that has one or more lots waiting
        ! to use it.

! Include files:

        include 'FIL:TYPDEF'
        include 'TBL:TBLDEF'
        include 'EQP:eqpdef.for'                ! Equipment Constraint Definition Requires This
        include 'FIL:eqpcpdef.for'              ! equipment capability
        include 'PAR:LOTSTDEF'

! Arguments:

        character*(*)           curLocation                     ! [R]
        ! - location id

        integer*4               wipCount                        ! [R]
        ! - number of lots at curLocation

        character*(*)           wipLotid( wipCount )            ! [R/W]
        ! - [R] = list of lots waiting at curLocation
        ! - [W] = list of lots waiting for eqpType

        character*(*)           wipRecpid( wipCount )           ! [R/W]
        ! - list of recipes for wipLotid

        character*(*)           wipEqpType( wipCount )          ! [R/W]
        ! - list of equipment types for wipLotid

        character*(*)           wipEqpId( wipCount )            ! [R/W]
        ! - list of equipment ids for wipLotid

        character*(*)           wipCapability( wipCount )       ! [R/W]  ! Added for Advanced Equipmetn Capability Check
        ! - list of Equipment Capability for wipLotid

        real*4                  wipCurMainQty( wipCount )       ! [R/W]
        ! - list of lot sizes for wipLotid

        byte                    wipState( wipCount )            ! [R/W]
        ! - list of lot state for wipLotid

        byte                    wipFlag( wipCount )            	! [R/W]
        ! - list of lot flags : used internally

        character*(*)           eqpType                         ! [W]
        ! - equipment type 

        character*(*)           eqpid                           ! [W]
        ! - equipment id 

        integer*4               wipCountWaiting                         ! [W]
        ! - number of waiting lots

        integer*4               wipCountRunning                         ! [W]
        ! - number of running lots


! Routines used:

        integer*4       tbl_equnit_promptLocEqp 
        external        tbl_equnit_getType 
        logical*4       wct_wipAccess_anyLotsInEqpWct 
        integer*4       Wct_EqpAccess_SelectEqpc                        !for advanced equipment capability testing
	integer*4	Gut_CurrenTim					!for Constraint Check
        integer*4	EqpConst_ExpUtil_CheckEqpLot                    !Advance Equipment Constraint Checking


! Author:       Mike Bergsma            9/93
!               Ab Initio Software

! Modifications:
!       Paul R. Neveu   2/13/2001
!       Add Advanced Equipment functionality for
!       Capabilities and Constraints Checking

! Local Variables:

        integer*4                       status,
     &                                  i,
     &                                  qty,
     &                                  eqpidLen, 
     &					count,		
     &                                  txtime,                
     &                                  ConstChk	

        byte                            runningState,
     &                                  state,
     &					flag

        logical*4                       found

        real*4                          curMainQty

        character*(TYP__EQPIDSZ)        eqpId2
        character*(TYP__EQPTYPSZ)       eqpType2
        character*(TYP__RECPIDSZ)       recpId
        character*(TYP__LOTIDSZ)        lotid

        ! Capability placeholder
        character*(TYP__CAPABILITYSZ)   capability

        ! Capability required for Advanced Equipment
        character*(TYP__CAPABILITYSZ)   blank_Capability

        ! List of EQPC records associated with one EQPID
        record /EQPC_record/    eqpcList(EQP__MAXEQPC_PER_EQP)

        ! List of Equipment Constraint Field
        logical*4                       accept

        ! Required for Advanced Equipment Checking
        character*(TYP__COMMENTSZ)      rejectReason

        ! To make sure we can track-into this lot.
        character*(TYP__CONSTRAINTIDSZ) constraintId

! Code:

        status = TBL__MATCH_NO
        blank_Capability = ' '
        runningState = ICHAR ( PAR_LOTSTDEF_CS_W_RSTR ) 
        status = Gut_CurrenTim (txTime)

        do while( status .ne. TBL__NORMAL )

          ! The specialist selects an equipment to use
          call tut_output ( ' ' )
          status = Tbl_Equnit_PromptLocEqp (
     &          'Enter an equipment id for batch track-in',
     &          curLocation,
!!!  &          TBL__STRICT+TBL__DISPOPT,
     &          TBL__STRICT,
     &          eqpId,
     &          eqpidLen )

          if ( status .eq. TBL__NORMAL ) then

            ! Check if equipment id's type is in the lot list.
            call Tbl_Equnit_GetType ( eqpId, eqpType )

            found = .false.
            do i=1, wipCount 

	      if ( wipState(i) .ne. runningState ) then

                ! Check if the Equipment could be tracked into
                ! given its current recipe and other characteristics
                ! this is a test of all constraints that would
                ! invalidate this piece of equipment for this
                ! particular lot and recipe combination!
                constChk = EqpConst_ExpUtil_CheckEqpLot (
     &                               eqpid,                    
     &                               wipLotid(i),              
     &                               wipRecpid(i),
     &				     ' ',  ! no group capability        
     &                               txTime,                   
     &                               accept,
     &                               rejectReason,
     &                               constraintID)

	        if ( constChk .and. accept) then
		  wipFlag(i) = 1
                  found = .true.
                endif
              endif
            enddo       

            if ( .not. found ) then

              call tut_output(  'No lots are waiting for equipment type '//
     &                          eqpType )

!!!           But lots with alternate recipes may be available...
!!!           call tut_output(  'Please select another equipment unit.' )
!!!           status = TBL__MATCH_NO
                
            endif

            ! Check to see that no lots are running in that equipment id
            if ( wct_wipAccess_anyLotsInEqpWct ( eqpId, curLocation ) ) then

              call tut_output( 
     &                  'Lots are already running at equipment '//eqpId )
                
            endif
        
          endif

        enddo

        ! Sort lot list so that those lots which are waiting to for
        ! the equipment type are at the front of the list and those
        ! lots which are running on the equipment type are second
        ! in the list. Everything else is put at the end.
        qty = 0
        wipCountWaiting = 0

        ! Sort waiting wip first
        do i=1,wipCount

          if (  wipFlag(i) .and.
     &          wipState(i) .ne. runningState ) then

            qty = qty + 1
            if ( qty .lt. i ) then

              ! Remove lot which does not use eqpType
              lotid = wipLotid(qty) 
              recpid = wipRecpid(qty) 
              eqptype2 = wipEqpType(qty) 
              eqpid2 = wipEqpId(qty) 
              curMainQty = wipCurMainQty(qty) 
              capability = wipCapability(qty)     
              state = wipState(qty)     
              flag = wipFlag(qty)     

              ! Insert lot which does use eqpType
              wipLotid(qty) = wipLotid(i) 
              wipRecpid(qty) = wipRecpid(i) 
              wipEqpType(qty) = wipEqpType(i) 
              wipEqpId(qty) = wipEqpId(i) 
              wipCurMainQty(qty) = wipCurMainQty(i) 
              wipCapability(qty) = wipCapability(i) 
              wipState(qty) = wipState(i) 
              wipFlag(qty) = wipFlag(i) 

              ! Restore lot which does not use eqpType
              wipLotid(i) = lotid
              wipRecpid(i) = recpid
              wipEqpType(i) = eqptype2
              wipEqpId(i) = eqpId2
              wipCurMainQty(i) = curMainQty
              wipCapability(i) = capability
              wipState(i) = state
              wipFlag(i) = flag

            endif

          endif
        
        enddo

        wipCountWaiting = qty

        ! Sort waiting wip first
        do i=qty+1,wipCount

          if (  wipEqpid(i) .eq. eqpId .and.
     &          wipState(i) .eq. runningState ) then

            qty = qty + 1
            if ( qty .lt. i ) then

              ! Remove lot which does not use eqpType
              lotid = wipLotid(qty) 
              recpid = wipRecpid(qty) 
              eqptype2 = wipEqpType(qty) 
              eqpid2 = wipEqpId(qty) 
              curMainQty = wipCurMainQty(qty) 
              capability = wipCapability(qty)     
              state = wipState(qty)     
              flag = wipFlag(qty)     

              ! Insert lot which does use eqpType
              wipLotid(qty) = wipLotid(i) 
              wipRecpid(qty) = wipRecpid(i) 
              wipEqpType(qty) = wipEqpType(i) 
              wipEqpId(qty) = wipEqpId(i) 
              wipCurMainQty(qty) = wipCurMainQty(i) 
              wipCapability(qty) = wipCapability(i) 
              wipState(qty) = wipState(i) 
              wipFlag(qty) = wipFlag(i) 

              ! Restore lot which does not use eqpType
              wipLotid(i) = lotid
              wipRecpid(i) = recpid
              wipEqpType(i) = eqptype2
              wipEqpId(i) = eqpId2
              wipCurMainQty(i) = curMainQty
              wipCapability(i) = capability
              wipState(i) = state
              wipFlag(i) = flag

            endif

          endif

        enddo

        wipCountRunning = qty - wipCountWaiting
!???	wipcount = qty

        return

        end

!++ AEQSSP_BATCH_GETBATCH 
!
        subroutine aeqSsp_batch_getBatch (      wipCount,
     &                                          wipLotid,
     &                                          wipRecpid,
     &                                          wipEqpType,
     &                                          wipEqpId,
     &                                          wipCapability,
     &                                          wipCurMainQty,
     &                                          wipState,
     &                                          wipFlag,
     &                                          wipCountWaiting,
     &                                          wipCountRunning )

        implicit none

! Description:

        ! Constructs a batch of lots from a list of wip lots.
        ! For each lot, the ACTL and RECP records are read into the global 
        ! buffer area.  The first lot defines the parameters and capacity
        ! constraints for the batch.  Subsequent lots in the batch must have
        ! identical recipe parameters.

! Include files:

        include 'FIL:FILDEF'
        include 'FIL:TYPDEF'
        include 'LOT:ACTLBUFF'
        include 'LOT:RECPBUFF'
        include 'PAR:PRODSTDEF'
        include 'PAR:PARDEF'
        include 'PAR:LOTSTDEF'
        include 'TST:PARMDEF'
        include 'GUT:GUTDEF'
        include 'TUT:TUTDEF'
        include 'UAF:SALESDEF'  
        include 'AEQ_SSP:AUTODEF'
        include 'AEQ_SSP:BATCHDEF'
        include 'FIL:SCHDDEF'
        include 'TMS:TMSDEF'

! Common areas:

        record /batch_struct/           batch
        common /batch_common/           batch

! Arguments:

        integer*4               wipCount                        ! [R/W]
        ! - number of lots waiting and running at curLocation

        character*(*)           wipLotid( wipCount )            ! [R/W]
        ! - [R] = list of lots waiting at curLocation
        ! - [W] = list of lots waiting for eqpType

        character*(*)           wipRecpid( wipCount )           ! [R/W]
        ! - list of recipes for wipLotid

        character*(*)           wipEqpType( wipCount )          ! [R/W]
        ! - list of equipment types for wipLotid

        character*(*)           wipEqpId( wipCount )            ! [R/W]
        ! - list of equipment ids for wipLotid

        character*(*)           wipCapability( wipCount )       ! [R/W]
        ! - list of equipment capability

        real*4                  wipCurMainQty( wipCount )       ! [R/W]
        ! - list of lot sizes for wipLotid

        byte                    wipState( wipCount )            ! [R/W]
        ! - list of lot state for wipLotid

        byte                    wipFlag( wipCount )            ! [R/W]
        ! - list of lot flags (not used)

        integer*4               wipCountWaiting                 ! [R/W]
        ! - number of lots waiting for equipment

        integer*4               wipCountRunning                 ! [R/W]
        ! - number of lots running at equipment


! Routines used:

        external        tut_output

        external        gut_uppercase
        integer*4       gut_trimblks
        integer*4       gut_scantable
        character*(VALUE_SIZE) gut_alltrim
        integer*4       lot_rdRecpRec_genRecord 

        external        fil_check
        integer*4       fil_open
        integer*4       fil_getRec
        integer*4       fil_getNext 
        integer*4       ssp_getLotId  
        logical*4       uaf_checksmod
        logical*4       Tbl_EqStCd_Available
        logical*4       Lot_MiscFns_IsMergeScheduled

        logical*4       aeqSsp_batch_checkParms 
        logical*4       aeqSsp_batch_testLot
        logical*4       aeqSsp_batch_selectRecp
        real*4          aeqSsp_batch_getMaxProcCap 

! Author:       Mike Bergsma            9/93
!               Ab Initio Software

! Modifications:
!
!       1/28/94 Mike Bergsma
!       - forgot to initialize parmNameCount
!
!       8/4/94  Mike Bergsma
!       - aeqSsp_batch_getMaxProcCap was declared as returning integer
!         when in fact it returns a real.
!
!       9/20/94 Mike Bergsma
!       - remove recursive switch to 'aeqssp_automan_getParm' because
!         it has been rewritten to know whether to do recursion.
!
!       9/30/94 Mike Bergsma
!       - DUMMY lots were not being allowed in first position
!
!       7/12/95 Mike Bergsma
!       - sigh! Need more logging for the obscure 1 in a million bug
!
!       Mike Bergsma    04-Aug-95
!       - stop using PROMIS log facility, now do logging just
!         like in the aeqSsp_automan_* routines.
!
!       11-Aug-95       Mike Bergsma
!       - call Tst_ParmUtil_TstPrmComInit full initialization in
!         case obscure batch bug is related to the lack of
!         full initialization (Note: partial initialization is
!         done in aeqSsp_autoMan_getParm 
!
!       26-Mar-96       Mike Bergsma
!       - fix subscript out of range error.
!
!       18-Nov-96       Mike Bergsma
!       - when using lot aliases, ssp_getlotid returns length of alias
!         rather than length of lotid.  Fix is to do a gut_trimblks on
!         the lotid variable  after ssp_getlotid is called, and also to
!         handle ambiguous entries (wipIndex .eq. -1)
!       - moved lock for EQPS record to just after first lot is entered
!
!       8-Mar-97        Mike Bergsma
!       - checking for TMS overdue tasks
!
!       Mike Bergsma    4/18/97
!       - add new array variables for each lot in the batch
!         lotType,priority,partid,procid,step,stage
!
!       Mike Bergsma    6/11/97
!       - for 'add-to-batch', allow running lot only from batch eqpid
!
!       Mike Bergsma    10/6/97
!       - use actl.specRecpid instead of wipRecpid (from global sections),
!         because wipRecpid does not reflect the case of a newly activated
!         recipe.
!
!       Mike Bergsma    11/18/97
!       - use Tbl_EqStCd_Available(status,lottype) instead of 
!         just checking that (status .eq. 'AVAIL') for tms.
!
!       Mike Bergsma    10/4/98
!       - check if automatic merge would occur
!       - check if TMS task already active
!
!       Mike Bergsma    4/25/99
!       - restrict track-in based on lot type.
!
! Local Variables:

        integer*4       status,
     &                  i,
     &                  wipIndex,
     &                  wipIndex2,
     &                  lotidLen,
     &                  recpidLen,
     &                  currentTime,
     &                  numAltRecipe,
     &                  txtime,                
     &                  constChk

        logical*4       validLot,
     &                  hasAltRecipe,
     &                  tmsdue,
     &                  accept

        character*80    line
        integer*2       lineLen

        character*(TYP__LOTIDSZ)        lotid
        character*(TYP__RECPIDSZ)       recpid

        character*(VALUE_SIZE)          route
        integer*4                       routeLen

	real*4 		earlyWarnStart,lateWarnEnd 

        logical*4                       accept
             ! List of Equipment Constraint Field
        character*(TYP__COMMENTSZ)      rejectReason
            ! Required for Advanced Equipment Checking
        character*(TYP__CONSTRAINTIDSZ) constraintId
            ! To make sure we can track-into this lot.


! Code:

        ! Initialize parameter lookup tables (full Initialization)
        call Tst_ParmUtil_TstPrmComInit( .true. )

        ! Prompt for each lot in the batch
        validLot = .true.
        tmsdue = .false.
        do while (      batch.lotCount .lt. MAX__BATCH_LOTS .and. 
     &                  wipCount .gt. 0 .and.
     &                  validLot )

          call tut_output ( ' ' )

          ! Prompt for a batch lot
          if ( batch.lotCount .eq. 0 .and. .not. batch.isAddtoBatch ) then
            lotidLen = ssp_getLotId ( 
     &                  'Enter first lot id to batch, or NONE, RETURN, END',
     &                  lotId, 
     &                  GUT__EMPTY+GUT__NONE+GUT__END )
          else
            lotidLen = ssp_getLotId ( 
     &                  'Enter next lot id in batch, or NONE, RETURN, END',
     &                  lotId, 
     &                  GUT__EMPTY+GUT__NONE+GUT__END )
          endif

          wipIndex = 0

          if ( lotidLen .eq. 0 ) then

            ! User pressed RETURN

            if ( wipCountWaiting .gt. 0 ) then

              ! Display lots
              call tut_output ( ' ' )
              call tut_output ( 
     &          'Lots waiting for equipment '//batch.eqpId//': ' )
!!!     &          'Lots waiting for equipment type '//batch.eqpType//': ' )

              call aeqSsp_batch_listRecipe(     wipCountWaiting,
     &                                          wipLotid,
     &                                          wipRecpid )
            else
        
              call tut_output ( ' ' )
              call tut_output ( 
     &          'No lots are waiting for equipment '//batch.eqpId )
!!!     &          'No lots are waiting for equipment type '//batch.eqpType )

            endif

            if ( batch.lotCount .eq. 0 .and. .not. batch.isAddtoBatch ) then

              if ( wipCountRunning .gt. 0 .and. .not. batch.isAddtoBatch ) then

                call tut_output ( ' ' )
                call tut_output ( 
     &            'Lots running at equipment '//batch.eqpId//': ' )

                call aeqSsp_batch_listRecipe(   wipCountRunning,
     &                                          wipLotid(wipCountWaiting+1),
     &                                          wipRecpid(wipCountWaiting+1) )

              else
        
                call tut_output ( ' ' )
                call tut_output ( 
     &          'No lots are running in equipment type '//batch.eqpId )

              endif

            endif

            goto 100

          endif

          ! Something was entered

          !!! PROMIS BUG 
          !!! When a lot alias is used, ssp_getLotid returns length of alias
          !!! rather than length of lotid, therefore we need a gut_trimblks
          !!! here.
          lotidLen = gut_trimblks ( lotid )
          !!!

          call gut_uppercase ( lotid(:lotidLen) )
          if ( lotid(:lotidLen) .eq. 'END' ) return

          if ( lotid(:lotidLen) .eq. 'NONE' ) then

            call aeqSsp_batch_addBatch (        'DUMMY',        ! Lotid
     &                                          'DUMMY',        ! LotType
     &                                          0.0,            ! Qty
     &                                          0,              ! Priority
     &                                          'DUMMY',        ! Partid
     &                                          'DUMMY',        ! Procid
     &                                          '000.000',      ! Step
     &                                          'DUMMY',        ! Route
     &                                          'DUMMY',        ! Stage
     &                                          'DUMMY',        ! Recipe
     &                                          .false.,        ! hasAlt
     &                                          0 )             ! numAlt

            ! Make sure all parameters are set to undefined
            do i=1,TYP__MAXRECPPARMS
              batch.parmVal(batch.lotCount,i) = 'UNDEFINED'
            enddo

            call sys$fao ( 
     &          'A DUMMY (empty) lot has been entered at position !UL',
     &          lineLen, line,
     &          %val ( batch.lotCount ) )
            call aeqSsp_batch_log ( batch.log, .true., line(:lineLen) )

            goto 100

          endif

          ! A lot has been selected for batch. See if it is in the available
          ! wip list for the current location
          wipIndex = gut_scantable(     lotid(:lotidLen), 
     &                                  wipLotid, 
     &                                  wipCount )

          if ( wipIndex .eq. 0 ) then
            call aeqSsp_batch_log ( batch.log, .true.,
     &          'Could not find lot '//lotid(:lotidLen)//
     &          ' at location '//batch.curLocation )
            validLot = .false.
            goto 100
          elseif ( wipIndex .eq. -1 ) then
            ! Ambiguous entry. 
            validLot = .false.
            goto 100
          endif 

          lotId = wipLotid( wipIndex )
          lotidLen = gut_trimblks( lotid )

          ! Fetch the actl record for the waiting lot. 
          call fil_check( fil_getrec (  FIL__ACTL, ACTL__LOTID, 
     &                                  lotid, FIL__CMP_EQ, 
     &                                  FIL__NOLOCK, actl.buffer ) )

          ! Check to see if this is 'add to batch'
          if ( wipIndex .gt. wipCountWaiting .and.
     &         wipIndex .le. wipCountWaiting+wipCountRunning .and.
     &         wipState(wipIndex) .eq. ICHAR (PAR_LOTSTDEF_CS_W_RSTR) ) then

            if ( batch.isAddtoBatch .or. 
     &           wipEqpid(wipIndex) .ne. batch.eqpid ) then

              call aeqSsp_batch_log ( batch.log, .true.,
     &          'Lot is already running in equipment '//batch.eqpid )
              validLot = .false.
              goto 100

            endif

            ! Get the running recipe
            call fil_check( lot_rdRecpRec_genRecord ( actl.specRecpid,
     &                                                recp ) )

            ! Initialize the recipe from the actl record
            call aeqSsp_batch_log (     batch.log, .true.,
     &          'The currently RUNNING batch lot '//actl.lotid//
     &          ' uses recipe '//recp.recpid )

            call aeqSsp_batch_log (     batch.log, .true.,
     &                  'Each lot selected will '//
     &                  TUT_T_BLINK//TUT_T_REVERSE//TUT_T_BOLD//
     &                  'ADD TO BATCH'//
     &                  TUT_T_STANDARD//
     &                  ' previously tracked in with this lot' ) 

            call aeqSsp_batch_initRecipe () 
            batch.isAddtoBatch = .true.
            batch.template = actl.lotid    
            batch.templateLotType = actl.lotType

	    ! The running lot determines the capability for the rest of the lots
	    batch.eqpCapability = actl.capability

            ! Re-prompt for a waiting lot to add.
            goto 100

          endif

          if ( wipState(wipIndex) .ne. PAR_LOTSTDEF_CS_W_RQUE .and.
     &         wipState(wipIndex) .ne. PAR_LOTSTDEF_CS_W_LQUE ) then
            call aeqSsp_batch_log ( batch.log, .true.,
     &          'Lot is not waiting at equipment '//batch.eqpid )
            validLot = .false.
            goto 100
          endif

          ! Check that equipment is in an available state
          if ( .not. Tbl_EqStCd_Available(batch.eqpStatus,actl.lotType )) then

            call tut_output ( ' ' )
            call tut_output ( 
     &          'Lot is not allowed to track into the equipment.' )
            call tut_output ( 
     &          'The equipment is in state '//
     &          batch.eqpStatus(:gut_trimblks(batch.eqpStatus))//
     &          ' and the incompatible lot type is '//actl.lottype )
            validLot = .false.
            goto 100

          endif

          ! Initialize recipe if this is the first (non-DUMMY) lot 
          if ( batch.firstLot .eq. 0 .and. .not. batch.isAddtoBatch .and.
     &	       wipCapability ( wipIndex ) .ne. ' ' ) then
	    batch.eqpCapability = wipCapability ( wipIndex ) 
	  endif

          call aeqSsp_batch_log ( batch.log, .false.,
     &          ' ' )
          call aeqSsp_batch_log ( batch.log, .false.,
     &          'Testing lot '//lotid(:lotidLen)//' for inclusion in batch' )

          if ( .not. aeqSsp_batch_selectRecp (  wipRecpId ( wipIndex ),
     &                                          wipEqpType ( wipIndex ),
     &                                          wipCapability ( wipIndex ),
     &                                          batch.eqpid,
     &                                          batch.eqpType,
     &                                          batch.eqpCapability,
     &                                          batch.curLocation,
     &                                          batch.log,
     &                                          actl,
     &                                          recp,
     &                                          hasAltRecipe,
     &                                          numAltRecipe ) ) then

            validLot = .false.
            goto 100

          endif

          ! Set default RECP record for the lot. 
          recpId = recp.recpId
          recpIdLen = gut_trimblks( recpid )

          ! Make sure lot record knows about the correct equipment type
          actl.eqpType = batch.eqpType
          actl.capability = batch.eqpCapability

          ! Get capacity info in ACTL for PROMIS capacity functions 
          ! (tbl_equnit_*) to work.
          actl.specRecpId = recpId
          actl.cacheRecpCapAttr = recp.capAttr
          actl.cacheRecpMaxProcCap = 
     &          aeqSsp_batch_getMaxProcCap (    batch.eqpBsyEnabled,
     &                                          recp.maxProcCap )

          ! Initialize recipe if this is the first (non-DUMMY) lot 
          if ( batch.firstLot .eq. 0 .and. .not. batch.isAddtoBatch ) then
            call aeqSsp_batch_log (     batch.log, .true.,
     &          'The first batch lot '//actl.lotid//
     &          ' uses recipe '//recp.recpid )
            call aeqSsp_batch_initRecipe () 
          endif

          ! Check if the equipment has the capacity to hold this lot
          ! plus whatever else is batched so far.
          validLot = aeqSsp_batch_testLot ( actl ) 
          if ( .not. validLot ) goto 100
        
          if ( batch.firstLot .ne. 0 .or. batch.isAddtoBatch ) then

            ! Check critical parameters for 2nd, 3rd, ... lots

            if ( batch.mainRecpid .ne. recpid ) 
     &        call aeqSsp_batch_log ( batch.log, .true.,
     &          'Lot uses recipe '//recpid(:recpidLen)//
     &          ' instead of the batch recipe '//batch.mainRecpid )

            if ( .not. aeqSsp_batch_checkParms ( ) ) then
              validLot = .false.
              goto 100
            endif

          endif

          ! Lock the actl record. 
          status = fil_getrec ( FIL__ACTL, ACTL__LOTID, 
     &                          lotid, FIL__CMP_EQ, 
     &                          FIL__LOCK, actl.buffer )

          if ( status .eq. FIL__RLK ) then
            call aeqSsp_batch_log (     batch.log, .true.,
     &          'Cannot lock lot for batch function, '//
     &          'The lot is already locked by another user.' )
            validlot = .false.
            goto 100
          endif


          ! Check for automatic merge
          if ( Lot_MiscFns_IsMergeScheduled ( actl ) ) then
            call aeqSsp_batch_log (     batch.log, .true.,
     &          'Lot is scheduled for an automatic merge. '//
     &          'Please perform merge manually.')
            validlot = .false.
            goto 100

          endif

          ! Check for overdue or running tasks
          status = fil_getrec ( FIL__SCHD, SCHD__ITEMID, batch.eqpId,
     &                          FIL__CMP_GE, FIL__NOLOCK, schd )

          do while ( status .and. schd.itemId .eq. batch.eqpid ) 

            if ( schd.inProgress ) then

              if ( schd.changeStatus .ne. ' ' .and. .not. 
     &             Tbl_EqStCd_Available (schd.changeStatus,actl.lotType) ) then

                call aeqSsp_batch_log ( batch.log, .true.,
     &          'TMS task '//schd.gTaskName(:gut_trimblks(schd.gTaskName))//
     &          ' is currently in progress. Please complete task.' )
                validlot = .false.
                goto 100

              endif
                
            else if ( schd.scheduleMethod .eq. TMS_TMSDEF_TYP_TIME ) then

              call gut_currentim ( currentTime )
              if ( currentTime .ge. schd.scheduledTime .and.
     &             .not. schd.inProgress .and.
     &             schd.changeStatus .ne. ' ' .and.
     &             .not. Tbl_EqStCd_Available (schd.changeStatus,actl.lotType))
     &          tmsdue = .true.

            elseif ( schd.scheduleMethod .eq. TMS_TMSDEF_TYP_NUMLOTS ) then

              schd.tallyNumLots = schd.tallyNumLots + batch.lotcount
              if ( schd.tallyNumLots .ge. schd.scheduledNumLots .and.
     &             .not. schd.inProgress .and.
     &             schd.changeStatus .ne. ' ' .and.
     &             .not. Tbl_EqStCd_Available (schd.changeStatus,actl.lotType))
     &          tmsdue = .true.

            elseif ( schd.scheduleMethod .eq. TMS_TMSDEF_TYP_QTYMAT ) then

              schd.tallyMatQty = schd.tallyMatQty + batch.loadSize
              if ( schd.tallyMatQty .ge. schd.scheduledMatQty .and.
     &             .not. schd.inProgress .and.
     &             schd.changeStatus .ne. ' ' .and.
     &             .not.Tbl_EqStCd_Available (schd.changeStatus,actl.lotType))
     &          tmsdue = .true.

            elseif ( schd.scheduleMethod .eq. TMS_TMSDEF_TYP_RUNS ) then

              schd.tallyNumRuns = schd.tallyNumRuns + 1
              if ( schd.tallyNumRuns .ge. schd.scheduledNumRuns .and.
     &             .not. schd.inProgress .and.
     &             schd.changeStatus .ne. ' ' .and.
     &             .not.Tbl_EqStCd_Available (schd.changeStatus,actl.lotType))
     &          tmsdue = .true.

            elseif ( schd.scheduleMethod .eq. TMS_TMSDEF_TYP_VOLACCUM ) then

              earlyWarnStart = schd.scheduledVolAcc - schd.earlyVolAccAllow
	      lateWarnEnd = schd.scheduledVolAcc + Schd.LateVolAccAllow

              if ( schd.tallyVolAcc .ge. earlyWarnStart .and.
     &             schd.tallyVolAcc .le. LateWarnEnd .and.
     &             .not. schd.inProgress .and.
     &             schd.changeStatus .ne. ' ' .and.
     &             .not.Tbl_EqStCd_Available (schd.changeStatus,actl.lotType))
     &          tmsdue = .true.

            endif

            if ( tmsDue ) then
              call aeqSsp_batch_log (   batch.log, .true.,
     &          'TMS task '//schd.gTaskName(:gut_trimblks(schd.gTaskName))//
     &          ' would change the equipment status to '//schd.changeStatus )
              validlot = .false.
              goto 100
            endif

            status = fil_getnext ( FIL__SCHD, FIL__NOLOCK, schd.buffer )

          enddo

          ! Determine route
          route = ' '
          routeLen = 1
          i = 1
          do while ( i .le. actl.prcdStackCount-1 )
            route(routeLen+1:) =        actl.prcdStackPrcdId(i)//'('//
     &                                  actl.prcdStackCurInstNum(i)//')/'
            route = gut_alltrim ( route, GUT__PAD_BLANKS )
            routeLen = gut_trimblks ( route )
            i = i + 1
          enddo

          ! Add lot to batch
          call aeqSsp_batch_addBatch (  lotid,
     &                                  actl.lottype,
     &                                  actl.curMainQty,
     &                                  actl.priority,
     &                                  actl.partid,
     &                                  actl.curPrcdId,
     &                                  actl.curPrcdCurInstNum,
     &                                  route(:routeLen),
     &                                  actl.stage,
     &                                  recpid,
     &                                  hasAltRecipe,
     &                                  numAltRecipe )

          if ( batch.firstLot .eq. 0 ) then
            batch.firstLot = batch.lotcount
            if ( .not. batch.isAddToBatch ) then
              batch.template = actl.lotid
              batch.templateLotType = actl.lotType
            endif  
          endif

          ! Remove lot from list
          do i=wipIndex,wipCount-1

            wipLotid(i) = wipLotid(i+1) 
            wipRecpid(i) = wipRecpid(i+1) 
            wipEqpType(i) = wipEqpType(i+1) 
            wipEqpId(i) = wipEqpId(i+1) 
            wipCurMainQty(i) = wipCurMainQty(i+1) 
            wipState(i) = wipState(i+1) 
               
          enddo 
          wipCountWaiting = wipCountWaiting - 1
          wipCount = wipCount - 1

100       continue

        enddo

        if ( .not. validlot ) then
          call aeqSsp_batch_log ( batch.log, .false., 'Batch aborted' )
          call tut_exit ()
        endif

        return

        end

!++ AEQSSP_BATCH_ADDBATCH 
!
        subroutine aeqSsp_batch_addBatch (      lotid,
     &                                          lottype,
     &                                          curMainQty,
     &                                          priority,
     &                                          partid,
     &                                          procid,
     &                                          step,
     &                                          route,
     &                                          stage,
     &                                          recpid,
     &                                          hasAltRecipe,
     &                                          numAltRecipe )

        implicit none

! Description:

        ! Adds a lot to the batch structure

! Include files:

        include 'FIL:FILDEF'
        include 'FIL:TYPDEF'
        include 'PAR:PARDEF'
        include 'PAR:PRODSTDEF'
        include 'AEQ_SSP:AUTODEF'
        include 'AEQ_SSP:BATCHDEF'

! Common areas:

        record /batch_struct/           batch
        common /batch_common/           batch

! Arguments:

        character*(*)           lotid                           ! [R]
        ! - name of lot to be added to batch

        character*(*)           lotType                         ! [R]
        ! - type of lot to be added to batch

        real*4                  curMainQty                      ! [R]
        ! - component quantity of lot to be added to batch

        integer*4               priority                        ! [R]
        ! - priority of lot to be added to batch

        character*(*)           partid                          ! [R]
        ! - partid of lot to be added to batch

        character*(*)           procid                          ! [R]
        ! - procid of lot to be added to batch

        character*(*)           step                            ! [R]
        ! - current step of lot to be added to batch

        character*(*)           route                           ! [R]
        ! - current route of lot to be added to batch

        character*(*)           stage                           ! [R]
        ! - stage of lot to be added to batch

        character*(*)           recpid                          ! [R]
        ! - recpid of lot to be added to batch

        logical*4               hasAltRecipe                    ! [R]
        ! - if an alternate recipe exists

        integer*4               numAltRecipe                    ! [R]
        ! - how many alternate recipes

! Routines used:


! Author:       Mike Bergsma            9/93
!               Ab Initio Software

! Modifications:
!
!       Mike Bergsma    04-Aug-95
!       - stop using PROMIS log facility, now do logging just
!         like in the aeqSsp_automan_* routines.
!
!       Mike Bergsma    4/18/97
!       - add new array variables for each lot in the batch
!         lotType,priority,partid,procid,step,stage
!
! Local Variables:

        character*80    line
        integer*2       lineLen

        ! Add lot to batch
        batch.lotCount = batch.lotCount + 1
        batch.lotid( batch.lotCount ) = lotid
        batch.lotType( batch.lotCount ) = lotType
        batch.numComps( batch.lotCount ) = INT ( curMainQty )
        batch.priority( batch.lotCount ) = priority
        batch.partid( batch.lotCount ) = partid
        batch.procid( batch.lotCount ) = procid
        batch.step( batch.lotCount ) = step
        batch.route( batch.lotCount ) = route
        batch.stage( batch.lotCount ) = stage
        batch.recpid( batch.lotCount ) = recpid
        batch.hasAltRecipe( batch.lotCount ) = hasAltRecipe
        batch.numAltRecipe( batch.lotCount ) = numAltRecipe
        batch.isTrackedIn( batch.lotCount ) = .false.

        if ( batch.eqpBsyEnabled .and. batch.maxProcCap .gt. 0.0 ) then

          ! Update the load size of the batch
          if ( batch.capAttr .eq. PAR_PRODSTDEF_QA_LOTBASIS ) then
            batch.loadSize = batch.loadSize + 1.0
          else
            batch.loadSize = batch.loadSize + curMainQty
          endif

          ! Increment the prior load size to include this lot.

          call sys$fao ( 
     &          'Lot !AS (!UL !AS). Batch load size is now !UL !AS',
     &          lineLen, line,
     &          batch.lotid( batch.lotCount ),
     &          %val ( batch.numComps( batch.lotCount ) ),
     &          batch.basis,
     &          %val ( INT ( batch.loadSize ) ),
     &          batch.basis )
          call aeqSsp_batch_log ( batch.log, .true., line(:lineLen) )

          call sys$fao ( 
     &          'Remaining equipment capacity is now !SL !AS',
     &          lineLen, line,
     &          %val ( INT ( batch.remCap ) ),
     &          batch.basis )
          call aeqSsp_batch_log ( batch.log, .true., line(:lineLen) )

        endif

        return

        end

!++ AEQSSP_BATCH_INITRECIPE 
!
        subroutine aeqSsp_batch_initRecipe ( )

        implicit none

! Description:

        ! Initialize batch from recipe 
        ! Common buffers for recp and actl records must be filled.

! Include files:

        include 'FIL:FILDEF'
        include 'FIL:TYPDEF'
        include 'LOT:ACTLBUFF'
        include 'LOT:RECPBUFF'
        include 'PAR:PRODSTDEF'
        include 'PAR:PARDEF'
        include 'TST:PARMDEF'
        include 'GUT:GUTDEF'
        include 'TUT:TUTDEF'
        include 'AEQ_SSP:AUTODEF'
        include 'AEQ_SSP:BATCHDEF'

! Common areas:

        record /batch_struct/           batch
        common /batch_common/           batch

! Arguments:

! Routines used:

! Author:       Mike Bergsma            9/93
!               Ab Initio Software

! Modifications:
!
!       Mike Bergsma    5/12/94
!       - provide for "$AUTO_SOFTBATCH"
!
!       9/20/94 Mike Bergsma
!       - remove recursive switch to 'aeqssp_automan_getParm' because
!         it has been rewritten to know whether to do recursion.
!
!       Mike Bergsma    04-Aug-95
!       - stop using PROMIS log facility, now do logging just
!         like in the aeqSsp_automan_* routines.
!
!       Mike Bergsma    16-Aug-95
!       - move section of code to aeqSsp_batch_getParms
!         like in the aeqSsp_automan_* routines.
!
! Local Variables:

        parameter                       SOFT = .true.
        parameter                       HARD = .false.
        integer*4                       i

        integer*4                       parmCount
        character*(TYP__PARMNMSZ)       parmName( TYP__MAXRECPPARMS )
        character*(TYP__PARMVALSZ)      parmVal( TYP__MAXRECPPARMS )
        logical*4                       parmIsHard( TYP__MAXRECPPARMS )
        logical*4                       parmIsInit( TYP__MAXRECPPARMS )

        character*80                    line
        integer*2                       lineLen

! Code:

        ! Save recipe name and title
        batch.mainRecpid = recp.recpid
        batch.title = recp.title

        ! Initialize the batch parameters from the first lot's recipe and
        ! resolve any values that may be overridden by ACTL parameters. 
        ! If $AUTO_INITPARMS or $AUTO_INTRECPn parameter lists are specified,
        ! then only those initialization parameters are required to match.
        ! All remaining lots must have this identical set of parameters,
        ! unless this is a 'soft' batch.

        ! Get all parameters from the recipe
        call aeqSsp_batch_getParms (    parmCount,
     &                                  parmName,
     &                                  parmVal,
     &                                  parmIsHard,
     &                                  parmIsInit,
     &                                  batch.log )

        ! Initialize the batch with these parameters
        do i=1,parmCount

          batch.parmName(i) = parmName(i)
          batch.parmIsHard(i) = parmIsHard(i)
          batch.parmIsInit(i) = parmIsInit(i)
          batch.parmVal(batch.lotCount+1,i) = parmVal(i)

        enddo
        batch.numParms = parmCount

        ! Set capacity attribute and maximium processing capacity
        batch.capAttr = recp.capAttr
               
        ! Use planning standards if recipe capacity is still undefined ( 0.0 )
        ! or disabled ( .not. eqpBsyEnabled )
        if ( actl.cacheRecpMaxProcCap .eq. 0.0 ) then
          batch.maxProcCap = recp.maxLoadSize
        else
          batch.maxProcCap = actl.cacheRecpMaxProcCap 
        endif

        if ( batch.capAttr .eq. PAR_PRODSTDEF_QA_LOTBASIS ) then
          batch.basis = 'lots'
        else
          batch.basis = 'wafers'
        endif
               
        if ( batch.maxProcCap .gt. 0.0 ) then

          call sys$fao ( 
     &          'Recipe maximum equipment capacity is !UL !AS', 
     &          lineLen, line,
     &          %val ( INT ( batch.maxProcCap ) ),
     &          batch.basis )

          call aeqSsp_batch_log ( batch.log, .true., line(:lineLen) )

        else

          if ( batch.eqpBsyEnabled ) then

            call aeqSsp_batch_log (     batch.log, .true.,
     &                  TUT_T_BLINK//TUT_T_BOLD//
     &                  'Maximum processing capacity is not defined '//
     &                  'in recipe '//batch.mainRecpid//
     &                  TUT_T_STANDARD )

          else

            call aeqSsp_batch_log (     batch.log, .true.,
     &                  TUT_T_BLINK//TUT_T_BOLD//
     &                  'Planning maximum load size is not defined '//
     &                  'in recipe '//batch.mainRecpid//
     &                  TUT_T_STANDARD )

          endif

        endif

        return

        end

!++ AEQSSP_BATCH_GETPARMS
!
        subroutine aeqSsp_batch_getParms (      parmCount,
     &                                          parmName,
     &                                          parmVal,
     &                                          parmIsHard,
     &                                          parmIsInit,
     &                                          logFlag )

        implicit none

! Description:

        ! Resolve parameters for lot
        ! Common buffers for recp and actl records must be filled.

! Include files:

        include 'FIL:FILDEF'
        include 'FIL:TYPDEF'
        include 'LOT:ACTLBUFF'
        include 'LOT:RECPBUFF'
        include 'PAR:PRODSTDEF'
        include 'PAR:PARDEF'
        include 'TST:PARMDEF'
        include 'GUT:GUTDEF'
        include 'TUT:TUTDEF'
        include 'AEQ_SSP:AUTODEF'

! Arguments:

        integer*4               parmCount                               ! [W]
        ! - # of parameters

        character*(*)           parmName( TYP__MAXRECPPARMS )           ! [W]
        ! - parameters names extracted 

        character*(*)           parmVal( TYP__MAXRECPPARMS )            ! [W]
        ! - values of parameters

        logical*4               parmIsHard ( TYP__MAXRECPPARMS )        ! [W]
        ! - true if parameter is hard, false if parameter is soft

        logical*4               parmIsInit ( TYP__MAXRECPPARMS )        ! [W]
        ! - true if parameter is an initialization parameter

        logical*4               logFlag                                 ! [R]
        ! - if true, log the results

! Routines used:

        logical*4       aeqSsp_autoMan_getParm 

! Author:       Mike Bergsma            8/95
!               Ab Initio Software

! Modifications:
!


! Local Variables:

        parameter                       SOFT = .false.
        parameter                       HARD = .true.
        integer*4                       i,j

        character*(TYP__PARMNMSZ)       initParmName( MAX_TOKENS )
        integer*4                       initParmNameCount
        character*(TYP__PARMVALSZ)      initparmVal( MAX_VALUES, MAX_TOKENS )
        integer*4                       initparmValLen( MAX_VALUES, MAX_TOKENS )
        integer*4                       initparmValCount( MAX_TOKENS )

        logical*4                       isSoftBatch,
     &                                  found

        character*(TYP__PARMVALSZ)      parmValue

! Code:

        ! Extract all parameters from the recipe
        do i=1,recp.numParms

          parmName(i) = recp.parmName(i)

          ! Get the value of parmName(i), with overrides from ACTL.
          call aeqSsp_autoMan_getParm ( parmName(i),
     &                                  TST__PARMSOURCEACTL+
     &                                  TST__PARMSOURCERECP,
     &                                  parmVal(i) )

          ! ALL PARAMETERS ARE HARD BY DEFAULT 
          parmIsHard(i) = HARD

          ! PARAMETERS ARE ASSUMED NOT TO BE INITIALIZATION PARAMETERS
          parmIsInit(i) = .false.

        enddo
        parmCount = recp.numParms

        ! Get list of initialization parameters
        initParmNameCount = 0
        call aeqSsp_autoMan_multiParms ('RECIPE',
     &                                  TST__PARMSOURCEACTL+
     &                                  TST__PARMSOURCERECP,
     &                                  initParmName,
     &                                  initParmNameCount,
     &                                  initparmVal,
     &                                  initparmValLen,
     &                                  initparmValCount )

        if ( initParmNameCount .gt. 0 ) then

          ! (SINCE) INIT PARAMETERS WERE SPECIFIED, ONLY THEY WILL BE
          ! HARD, ALL OTHER PARAMETERS WILL BE SOFT.

          ! First set all parameters back to SOFT
          do i=1,parmCount
            parmIsHard(i) = SOFT
          enddo

          ! For each init parameter, find it in parameter list and set
          ! to HARD.  If it is not found, add it and set its value
          ! to UNDEFINED.

          do i=1,initParmNameCount

            j = 1
            found = .false.
            do while ( j .le. parmCount .and. .not. found )
              if ( initParmName(i) .eq. parmName(j) ) then
                found = .true.
                parmIsHard(j) = HARD
                parmIsInit(j) = .true.
              else      
                j = j + 1
              endif     
            enddo

            if ( .not. found ) then
              parmCount = parmCount + 1
              parmName(parmCount) = initParmName(i)
              parmVal(parmCount) = 'UNDEFINED'
              parmIsHard(parmCount) = HARD
              parmIsInit(parmCount) = .true.
            endif

          enddo

        endif

        ! Check $AUTO_SOFTBATCH
        if ( aeqSsp_autoMan_getParm (   '$AUTO_SOFTBATCH',
     &                                  TST__PARMSOURCEACTL+
     &                                  TST__PARMSOURCERECP,
     &                                  parmValue ) ) then
          isSoftBatch = ( parmValue .eq. 'YES' )
        else
          isSoftBatch = .false.
        endif

        ! If $AUTO_SOFTBATCH is YES, then there are no HARD parameters
        if ( isSoftBatch ) then
          do i=1,parmCount
            parmIsHard(i) = SOFT
          enddo
        endif

        ! Check for SOFTPARM and HARDPARM values.
        initParmNameCount = 0
        call aeqSsp_autoMan_multiParms ('HARD',
     &                                  TST__PARMSOURCEACTL+
     &                                  TST__PARMSOURCERECP,
     &                                  initParmName,
     &                                  initParmNameCount,
     &                                  initparmVal,
     &                                  initparmValLen,
     &                                  initparmValCount )

        if ( initParmNameCount .gt. 0 ) then

          ! THERE IS A LIST OF SPECIFIC HARD PARAMETERS

          do i=1,initParmNameCount
            j = 1
            found = .false.
            do while ( j .le. parmCount .and. .not. found )
              if ( initParmName(i) .eq. parmName(j) ) then
                found = .true.
                parmIsHard(j) = HARD
              else      
                j = j + 1
              endif     
            enddo
          enddo

        endif

        initParmNameCount = 0
        call aeqSsp_autoMan_multiParms ('SOFT',
     &                                  TST__PARMSOURCEACTL+
     &                                  TST__PARMSOURCERECP,
     &                                  initParmName,
     &                                  initParmNameCount,
     &                                  initparmVal,
     &                                  initparmValLen,
     &                                  initparmValCount )

        if ( initParmNameCount .gt. 0 ) then

          ! THERE IS A LIST OF SPECIFIC SOFT PARAMETERS

          do i=1,initParmNameCount
            j = 1
            found = .false.
            do while ( j .le. parmCount .and. .not. found )
              if ( initParmName(i) .eq. parmName(j) ) then
                found = .true.
                parmIsHard(j) = SOFT
              else
                j = j + 1
              endif     
            enddo
          enddo

        endif

        ! Print out a log, if requested
        call aeqSsp_batch_log ( logFlag, .false., 'Hard parameters are:' )
        do i=1,parmCount

           if ( parmIsHard(i) )
     &       call aeqSsp_batch_log (    logFlag, .false.,
     &          '   '//parmName(i)//' = '//parmVal(i) )
          enddo

          call aeqSsp_batch_log ( logFlag, .false., 'Soft parameters are:' )
          do i=1,parmCount
             
            if ( .not. parmIsHard(i) )
     &        call aeqSsp_batch_log (   logFlag, .false.,
     &            '   '//parmName(i)//' = '//parmVal(i) )
          enddo
        
        return

        end

!++ AEQSSP_BATCH_CHECKPARMS
!
        logical*4 function aeqSsp_batch_checkParms ( )

        implicit none

! Description:

        ! Check parameters for next lot against established batch parameters
        ! Common buffers for recp and actl records must be filled.

! Include files:

        include 'FIL:FILDEF'
        include 'FIL:TYPDEF'
        include 'LOT:ACTLBUFF'
        include 'LOT:RECPBUFF'
        include 'PAR:PRODSTDEF'
        include 'PAR:PARDEF'
        include 'TST:PARMDEF'
        include 'GUT:GUTDEF'
        include 'TUT:TUTDEF'
        include 'AEQ_SSP:AUTODEF'
        include 'AEQ_SSP:BATCHDEF'

! Common areas:

        record /batch_struct/           batch
        common /batch_common/           batch

! Routines used:

        logical*4       aeqSsp_batch_getParms
        integer*4       gut_trimblks

! Author:       Mike Bergsma            8/95
!               Ab Initio Software

! Modifications:
!

! Local Variables:

        integer*4       i,j,
     &                  firstLot,
     &                  nextLot

        logical*4       found

        integer*4                       parmCount
        character*(TYP__PARMNMSZ)       parmName( TYP__MAXRECPPARMS )
        character*(TYP__PARMVALSZ)      parmVal( TYP__MAXRECPPARMS )
        logical*4                       parmIsHard( TYP__MAXRECPPARMS )
        logical*4                       parmIsInit( TYP__MAXRECPPARMS )

! Code:

        ! Assume failure
        aeqSsp_batch_checkParms = .false.

        ! Get all parameters for the lot 
        call aeqSsp_batch_getParms (    parmCount,
     &                                  parmName,
     &                                  parmVal,
     &                                  parmIsHard,
     &                                  parmIsInit,
     &                                  batch.log )

        ! Parameter values are added into the batch array prior to
        ! the lot's complete acceptance for the batch, thus the
        ! parameter values must be inserted into the array
        ! position 'batch.lotCount+1'
        nextLot = batch.lotCount + 1
        firstLot = batch.firstLot

        ! In the case of "add to batch", this may be the first lot.
        if ( firstLot .eq. 0 ) firstLot = nextLot

        !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        ! Check #1: CHECK BATCH PARAMETERS AGAINST NEXT LOT

        do i=1,batch.numParms

          ! Look for a lot parameter of the same name 
          j = 1
          found = .false.
          do while ( j .le. parmCount .and. .not. found )
            if ( batch.parmName(i) .eq. parmName(j) ) then
              found = .true.
            else        
              j = j + 1
            endif       
          enddo

          if ( found ) then

            if ( batch.parmIsHard(i) ) then           

              ! The recipe parameter values for this lot must equal the
              ! recipe parameter values from the first lot in the batch

              if ( batch.parmVal(firstLot,i) .ne. parmVal(j) ) then

                ! No match.
                call aeqSsp_batch_log ( batch.log, .true.,
     &                  'Batch parameter '//
     &                  batch.parmName(i)(:gut_trimblks(batch.parmName(i)))//
     &                  ' value must be "'//
     &                  batch.parmVal(firstLot,i)
     &                  (:gut_trimblks(batch.parmVal(firstLot,i)))//'"' )

                call aeqSsp_batch_log ( batch.log, .true.,
     &                  'Lot parameter   '//
     &                  batch.parmName(i)(:gut_trimblks(batch.parmName(i)))//
     &                  ' value is      "'//
     &                  parmVal(j)(:gut_trimblks(parmVal(j)))//'"' )

                goto 100

              endif

            endif

            if ( batch.parmIsInit(i) .and. .not. parmIsInit(j) ) then
              call aeqSsp_batch_log (   batch.log, .true.,
     &          'Lot does not specify the initialization parameter '//
     &          batch.parmName(i)(:gut_trimblks(batch.parmName(i))) )
              goto 100
            endif

            ! Set batch parameter 
            batch.parmVal(nextLot,i) = parmVal(j)

          else 

            ! Parameter was not found.

            if ( batch.parmIsHard ( i ) ) then
              call aeqSsp_batch_log (   batch.log, .true.,
     &                  'Batch parameter '//
     &                  batch.parmName(i)(:gut_trimblks(batch.parmName(i)))//
     &                  ' is critical and is not defined for the lot' )

              goto 100
            endif

            if ( batch.parmIsInit(i) ) then
              call aeqSsp_batch_log (   batch.log, .true.,
     &          'Lot does not specify the initialization parameter '//
     &          batch.parmName(i)(:gut_trimblks(batch.parmName(i))) )

              goto 100
            endif

            ! Set batch parameter.
            batch.parmVal(nextLot,i) = 'UNDEFINED'

          endif ! found

        enddo

        !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        ! Check #2 : CHECK LOT PARAMETERS AGAINST BATCH

        do i=1,parmCount

          ! Look for a lot parameter of the same name 
          j = 1
          found = .false.           
          do while ( j .le. batch.numParms .and. .not. found )
            if ( parmName(i) .eq. batch.parmName(j) ) then
              found = .true.
            else        
              j = j + 1
            endif       
          enddo

          if ( .not. found ) then

            ! Parameter was not found

            if ( parmIsHard(i) ) then         

              call aeqSsp_batch_log (   batch.log, .true.,
     &                  'Lot parameter '//
     &                  parmName(i)(:gut_trimblks(parmName(i)))//
     &                  ' is critical and is not specified in the batch' )

              goto 100

            endif

            if ( parmIsInit(i) ) then
              call aeqSsp_batch_log (   batch.log, .true.,
     &          'Lot specifies an invalid initialization parameter '//
     &          batch.parmName(i)(:gut_trimblks(batch.parmName(i))) )

              goto 100
            endif

          endif ! not found

        enddo

        aeqSsp_batch_checkParms = .true.

100     continue
        return

        end

!++ AEQSSP_BATCH_GETMAXPROCCAP  
!
        real*4 function aeqSsp_batch_getMaxProcCap (    eqpBsyEnabled,
     &                                                  recpMaxProcCap )

        implicit none

! Description:

        ! Determine the maximim processing capacity.

! Include files:

        include 'FIL:TYPDEF'
        include 'PAR:PARDEF'
        include 'TST:PARMDEF'

! Arguments:

        logical*4               eqpBsyEnabled                   ! [R]
        ! - if Ssp_Option_EqpBsyEnabled () is true.

        character*(*)           recpMaxProcCap                  ! [R]
        ! - dispatch maximum processing capacity from RECP record

! Routines used:

        logical*4       aeqSsp_autoMan_getParm 
        logical*4       gut_editReal 

! Author:       Mike Bergsma            9/93
!               Ab Initio Software

! Modifications:
!
!       9/20/94 Mike Bergsma
!       - remove recursive switch to 'aeqssp_automan_getParm' because
!         it has been rewritten to know whether to do recursion.

! Local Variables:

        real*4                          maxProcCap
        character*(TYP__PARMVALSZ)      maxProcCapS
! Code:

        ! If the max process capacity is a parameter, substitute it first.
        maxProcCapS = recpMaxProcCap
        maxProcCap = 0.0

        if ( eqpBsyEnabled ) then

          ! Translate capacity value if a parameter
          if ( maxProcCapS(1:1) .eq. '$' ) 
     &          call aeqSsp_autoMan_getParm (   recpMaxProcCap,
     &                                          TST__PARMSOURCEACTL+
     &                                          TST__PARMSOURCERECP,
     &                                          maxProcCapS )

          ! Convert the capacity value to a floating value
          if ( .not. gut_editReal (     maxProcCapS,
     &                                  maxProcCap,
     &                                  0.0,
     &                                  TYP__MAXREAL4 ) ) maxProcCap = 0.0

        endif

        aeqSsp_batch_getMaxProcCap = maxProcCap

        return

        end

!++ AEQSSP_BATCH_TESTLOT
!
        logical*4 function aeqSsp_batch_testLot( actlRec )

        implicit none

! Description:

        ! Tests whether lotid can be tracked into eqpid along with other
        ! lots that contribute <priorLoad> to the equipment.

! Include Files:

        include 'FIL:FILDEF'
        include 'FIL:TYPDEF'
        include 'FIL:ACTLPDEF'
        include 'TBL:TBLDEF'
        include 'PAR:PRODSTDEF'
        include 'AEQ_SSP:AUTODEF'
        include 'AEQ_SSP:BATCHDEF'

! Common areas:

        record /batch_struct/           batch
        common /batch_common/           batch

! Arguments:

        record /actl_record/ actlRec                            ! [R]
        ! - lot record to test for inclusion in the batch

! Routines Used:

        external        fil_check

! Author:       Mike Bergsma            9/93
!               Ab Initio Software

! Modifications:
!
!       Mike Bergsma    04-Aug-95
!       - stop using PROMIS log facility, now do logging just
!         like in the aeqSsp_automan_* routines.

! Local Variables:

        integer*4       i,
     &                  listSize

        logical*4       found,
     &                  validLot

        character*80    line
        integer*2       lineLen

        character*(TYP__EQPIDSZ)        eqpIdList( TBL_EQUNCOM_MAXCOUNT )
        real*4                          remCapList( TBL_EQUNCOM_MAXCOUNT )

! Code:

        ! Assume lot will not be valid
        validLot = .false.

        ! Get the list of valid equipment units for this lot/recipe
        ! considering actl.cacheRecpMaxProcCap and actl.cacheRecpCapAttr
        !
        ! If OPTION_EQPAVAILENABLED is NO, then all available equipment units 
        ! that can process actlrec.lottype will be returned. Otherwise, only 
        ! those units which:
        !
        !       1. are compatible:      maxProcCap, capAttr, and, matType of 
        !                               lots already running is the same as 
        !                               found in ACTL record for lot.
        !       2. have capacity :      (curloadSize + ACTL size) < maxProcCap
        !
        ! will be returned in the eqpIdList array.

        call Tbl_EqUnit_GetValidEqp ( actlRec, eqpIdList, remCapList, listSize )
        
        ! Search the list for our equipment id.  If we find it, then there
        ! is remaining capacity on those equipment units
        found = .false.
        i = 1
        do while ( .not. found .and. i .le. listSize )
          if ( eqpidList(i) .eq. batch.eqpid ) then
            found = .true.
          else
            i = i + 1
          endif
        enddo

        if ( found ) then

          if (  batch.eqpBsyEnabled .and. 
     &          actlRec.cacheRecpMaxProcCap .gt. 0.0 ) then

            ! Using PROMIS tracking equipment capacity calculations.
            !
            ! - OPTION_EQPAVAILENABLED = YES
            ! - recp.maxProcCap > 0.0
            !
            if ( batch.capAttr .eq. PAR_PRODSTDEF_QA_LOTBASIS ) then
              batch.remCap =    remCapList(i) - 
     &                          ( batch.loadSize + 1.0 )
            else
              batch.remCap =    remCapList(i) - 
     &                          ( batch.loadSize + actlRec.curMainQty )
            endif

          elseif ( batch.maxProcCap .gt. 0.0 ) then

            ! Using PROMIS planning equipment capacity calculations.
            !
            ! - recp.maxLoadSize > 0.0
            !
            if ( batch.capAttr .eq. PAR_PRODSTDEF_QA_LOTBASIS ) then
              batch.remCap = batch.maxProcCap - 
     &                          ( batch.loadSize + 1.0 )
            else
              batch.remCap = batch.maxProcCap - 
     &                          ( batch.loadSize + actlRec.curMainQty )
            endif

          endif

          if ( batch.remCap .ge. 0.0 ) then

            validLot = .true.   

          else

            call aeqSsp_batch_log (     batch.log, .true.,
     &          'Lot could not be included in batch for equipment '//
     &          batch.eqpid )

            if ( batch.maxProcCap .gt. 0.0 ) then

              call sys$fao ( 
     &          'Capacity of equipment would exceed !UL !AS',
     &          lineLen, line,
     &          %val ( INT ( batch.maxProcCap ) ),
     &          batch.basis )

              call aeqSsp_batch_log ( batch.log, .true., line(:lineLen) )

            else

              call aeqSsp_batch_log (   batch.log,  .true.,
     &          'Lot is not compatible with equipment '//batch.eqpid )

            endif       

          endif

        else

          call aeqSsp_batch_log (       batch.log,  .true.,
     &          'Lot could not be included in batch for equipment '//
     &          batch.eqpid )
          call aeqSsp_batch_log (       batch.log,  .true.,
     &          'because of insufficient equipment capacity '//
     &          'or lot type incompatiblity' )

        endif

        ! Return status
        aeqSsp_batch_testLot = validLot

        return
        end

!++     AEQSSP_BATCH_DISPATCH
!
        subroutine aeqSsp_batch_dispatch( curLocation, eqpType )

        implicit none

! Description:

        ! Read the dispatch list for the specified equipment type and display
        ! the first list of lots that can be batched.

! Include Files:

        include 'FIL:TYPDEF'
        include 'TBL:TBLDEF'
        include 'OPR:OPRDEF'
        include 'CFC:LUNDEF'
        include 'TUT:TUTDEF'

! Arguments:

        character*(*)   curLocation                             ! [R]
        ! - workcenter location

        character*(*)   eqpType                                 ! [R]
        ! - equipment type

! Routines used:

        integer*4       Tbl_Locate_GetDispatchType 
        external        Opr_Dispatch_SetFileName 
        external        Tbl_Locate_GetProdArea 
        logical*4       gut_maskstring
        integer*4       tut_setwidth

! Author:       Mike Bergsma            9/93
!               Ab Initio Software

! Modifications:
!
!       Mike Bergsma    28-Feb-94
!       - find all WAITing lots in file using the first recipe encountered 

! Local Variables:

        integer*4                       status,
     &                                  width,
     &                                  ios

        character*6                     state
        character*(TYP__RECPIDSZ)       recpid,firstRecpid
        character*(TYP__DISPATCHTYPESZ) dispatchType
        character*(TYP__PRODAREASZ)     prodarea
        character*(120)                 fileName

        parameter FILL = 132 - TYP__LOTIDSZ - TYP__STAGEIDSZ - TYP__RECPIDSZ - 4

        structure /dispatch_struct/

          union
            map
              character*12                      lotid   
              character*2                       fill1   
              character*10                      stage   
              character*2                       fill2   
              character*8                       recpid  
              character*2                       fill3
              character*11                      qty
              character*2                       fill4   
              character*15                      partid
              character*2                       fill5   
              character*15                      procid
              character*2                       fill6   
              character*1                       prio
              character*2                       fill7   
              character*11                      critRatio
              character*2                       fill8   
              character*11                      queueTime
              character*2                       fill9   
              character*11                      state
              character*2                       fill10  
              character*3                       ti
            end map
            map
              character*132                     cBuffer
            end map
          end union

        end structure

        record /dispatch_struct/ dispatch

! Code:

        status =  Tbl_Locate_GetDispatchType ( curLocation, dispatchType )

        if ( status .ne. TBL__NORMAL .or. dispatchType .eq. ' ' ) then

          call tut_output ( ' ' )
          call tut_output ( 'No dispatch for Location '//curLocation )

        else

          call Tbl_Locate_GetProdArea ( curLocation, prodArea )

          call Opr_Dispatch_SetFileName (       prodarea, 
     &                                          eqpType, 
     &                                          fileName, 
     &                                          OPR__EQPTYPE)
          open (        file            = fileName, 
     &                  unit            = LUN__FILE1,
     &                  type            = 'OLD',
     &                  form            = 'FORMATTED',
     &                  readonly,
     &                  iostat          = ios )

          call tut_output ( ' ' )

          if ( ios .ne. 0 ) then

            call tut_output ( 'Dispatch list has not been generated.' )

          else

            firstRecpId = ' '
            recpId = ' '
            call tut_clear ( )
            width = tut_setwidth( TUT_K_WIDE )
            do while ( .true. )

              read ( LUN__FILE1, 9000, end=100 ) dispatch.cBuffer
9000          format ( a )

              if ( gut_maskstring( dispatch.recpid, 'ccccc.nn' ) ) then

                ! Line has a valid recipe
                recpId = dispatch.recpId
                state = dispatch.state

                if ( firstRecpId .eq. ' ' ) firstRecpId = recpid

                if ( recpId .eq. firstRecpid .and. state .eq. 'WAIT' ) 
     &                  call tut_output( dispatch.cBuffer )     

              else

                ! Line does not have a valid recipe
                ! Display lines only if we haven't encountered the first recipe
                if ( firstRecpId .eq. ' ' ) call tut_output( dispatch.cBuffer ) 

              endif

            enddo

100         continue

            close ( unit = LUN__FILE1 )

            call tut_pagewait ( )
            if ( width .ne. TUT_K_WIDE ) call tut_setwidth( TUT_K_NARROW ) 

          endif

        endif

        return

        end

!++     AEQSSP_BATCH_SELECTRECP

        logical*4 function aeqSsp_batch_selectRecp (    primaryRecpId,
     &                                                  primaryRecpEqpType,
     &                                                  primaryRecpCapability,
     &                                                  batchEqpId, 
     &                                                  batchEqpType, 
     &                                                  batchEqpCapability, 
     &                                                  batchLocation,
     &                                                  batchLog,
     &                                                  actl, 
     &                                                  recp,
     &                                                  hasAltRecipe,
     &                                                  numAltRecipe )

        implicit none   

! Description:

        ! Selects the recipe to use for the lot

!   Include files:

        include 'FIL:TYPDEF'
        include 'FIL:FILDEF'
        include 'TBL:TBLDEF'
        include 'PAR:PARDEF'
        include 'GUT:GUTDEF'
        include 'TST:PARMDEF'
        include 'FIL:ACTLPDEF'
        include 'FIL:RECPPDEF'
        include 'FIL:PRCDDEF'
        include 'FIL:PINSDEF'
        include 'FIL:PIARDEF'

!   Arguments:

        character*(*)           primaryRecpId                           ! [R]
        ! - primary recipe for lot

        character*(*)           primaryRecpEqpType                      ! [R]
        ! - primary recipe equipment type for lot

        character*(*)           primaryRecpCapability  			! [R]
        ! - primary recipe equipment capability for lot

        character*(*)           batchEqpId                            	! [R]
        ! - required equipment id

        character*(*)           batchEqpType                            ! [R]
        ! - required equipment type 

        character*(*)           batchEqpCapability			! [R]
        ! - required equipment capability

        character*(*)           batchLocation                           ! [R]
        ! - required location

        logical*4               batchLog                                ! [R]
        ! - controls logging

        record /ACTL_record/    actl                                    ! [R]
        ! - lot record

        record /RECP_record/    recp                                    ! [W]
        ! - selected recp record

        logical*4               hasAltRecipe                            ! [W]
        ! - set to true if the lot has alternate recipes

        integer*4               numAltRecipe                            ! [W]
        ! - if alternate recipe was selected, this is its number

! Routines Used:

        external        pro_keyUtil_setPinsKey 
        external        gut_check 
        integer*4       gut_getIntP 
        integer*4       lot_rdPinsRec_genRecord 
        external        fil_check 
        integer*4       fil_getRec 
        external        pro_keyUtil_setPiarKey 
        integer*4       lot_rdRecpRec_genRecord
        integer*4       tbl_prArea_validProdArea 
	integer*4	EqpConst_ExpUtil_CheckTkin

! Author:       Mike Bergsma            7/92
!               Ab Initio Software
!
! Modifications:
!		Paul R. Neveu	2-Apr-2001
!		Modify logic to support Enhanced equipment Routing Selection
!		Modify logic to Check equipment Constraints & Validate Type or
!		capability depending upon which is used.
!

! Local Variables:

        integer*4       i,choice,status,ConstChk,txtime
	logical*4	accept

        character*(PINS_S_PINSKEY)      pinsKey
        character*(PIAR_S_PIARKEY)      piarKey
        character*(TYP__LOCATIONIDSZ)   routlocation
        character*(TYP__PRODAREASZ)     prodArea

        integer*4                       numValidRecipes
        character*(TYP__RECPIDSZ)       recpid(PAR_PARDEF_MAXALTRECIPES+1) 
        integer*4                       selection(PAR_PARDEF_MAXALTRECIPES+1) 
        character*(TYP__COMMENTSZ)      title(PAR_PARDEF_MAXALTRECIPES+1) 
        character*(TYP__COMMENTSZ)      rejectReason
            ! Required for Advanced Equipment Checking
        character*(TYP__CONSTRAINTIDSZ) constraintId
            ! To make sure we can track-into this lot.

        record /RECP_record/            altRecp 


        ! Set flag to indicate whether alternate recipies exist for the
        ! lot.  If they do, the track-in prompts will be different,
        ! even if the primary recipe is choosen
        hasAltRecipe = ( actl.cachePinsPiarCount .ne. 0 ) 

        ! Assume no alternates exist or will not be selected.
        numAltRecipe = 0

        ! Assume we'll find a recipe
        aeqSsp_batch_selectRecp = .true.

        ! Find all valid recipes that have the right equipment type
        numValidRecipes = 0
          
	! If this is the first call to this routine,
	! ie: the first lot is selected, then the batch capability
	! field is not determined, it will become the capability
	! 
        ! Check if the Equipment could be tracked into
        ! given its current recipe and other characrteristics
        ! this is a test of all constraints that would
        ! invalidate this piece of equipment for this
        ! particulat lot and recipe combination!
        !
        ! If the recipe has been updated while the lot is waiting
        ! then the WIP table value for recpid (ie: primaryRecpId)
        ! may be incorrect.  In that case, if the specific recipe
        ! of the lot is specified, use it instead.
        if ( actl.specRecpId .ne. ' ' ) primaryRecpId = actl.specRecpid

        call fil_check( lot_rdRecpRec_genRecord ( primaryRecpId, recp ) )

        constChk = EqpConst_ExpUtil_CheckTkin (  
     &                               batchEqpid,
     &                               actl,
     &                               recp,
     &                               ' ', ! No group capability
     &                               txTime,
     &                               accept,
     &                               rejectReason,
     &                               constraintID)

	if ( constChk .and. accept) then
           if ( ( Recp.eqptype .eq. batchEqpType .and.
     &            Recp.capability .eq. ' ' ) .or.
     &          ( Recp.capability .ne. ' '   .and.
     &            Recp.capability .eq. batchEqpCapability ) )  then

          ! The primary recipe is ok
             numValidRecipes = numValidRecipes + 1 
             recpid(numValidRecipes) = primaryRecpId
             selection(numValidRecipes) = 0 ! Because its the primary recipe
             title(numValidRecipes) = recp.title

           endif
        endif

        if ( hasAltRecipe ) then

          ! Get procedure record
          call fil_check ( fil_GetRec ( FIL__PRCD, PRCD__PRCDID,
     &                                  actl.curPrcdId, FIL__CMP_EQ,
     &                                  FIL__NOLOCK, prcd ) )

          ! Get PINS record.
          call pro_keyUtil_setPinsKey ( actl.curPrcdName, 
     &                                  actl.curPrcdVersion, 
     &                                  actl.curPrcdcurInstNum, 
     &                                  pinsKey )
          call fil_check ( lot_rdPinsRec_genRecord ( pinsKey, pins.buffer ))
          
          ! Loop over all alternate recipes find all recipes that
          ! use the required equipment type

          do i=1,pins.altRecpCount 

            ! Get the PIAR record
            call pro_keyUtil_setPiarKey (       pins.prcdName, 
     &                                          pins.prcdVersion,
     &                                          pins.pinsInstNum, 
     &                                          i, 
     &                                          piarKey )
            call fil_check ( fil_getRec (       FIL__PIAR, PIAR__PIARKEY,
     &                                          piarKey, FIL__CMP_EQ, 
     &                                          FIL__NOLOCK, piar ) )

            ! Fetch recipe record for the alternate
            call fil_check( lot_rdRecpRec_genRecord (piar.altRecpId,altrecp))

            ! Check if the Equipment could be tracked into
            ! given its current recipe and other characrteristics
            ! this is a test of all constraints that would
            ! invalidate this piece of equipment for this
            ! particular lot and recipe combination!
            constChk = EqpConst_ExpUtil_CheckTkin (  
     &                               batchEqpid,
     &                               actl,
     &                               altRecp,
     &				     ' ', ! No group capability
     &                               txTime,
     &                               accept,
     &                               rejectReason,
     &                               constraintID)

	    if ( constChk .and. accept) then

               if ( altRecp.recpid .ne. primaryRecpid .and.
     &              ( ( altRecp.eqptype .eq. batchEqpType .and.
     &                  altRecp.capability .eq. ' ' ) .or.
     &                ( altRecp.capability .ne. ' '   .and.
     &                  altRecp.capability .eq. batchEqpCapability) ) ) then

                 ! Alternate recipe uses the specified equipment type
    	         ! or capabilty

                 ! Check if the production area is valid.
                 ! Override if specified.
                 prodArea = prcd.mainProdArea
                 if ( pins.stepProdArea .ne. ' ' ) then

                    ! Execute Recipe Instruction overrides production area.
                    if ( tbl_prArea_validProdArea ( pins.stepProdArea ) .eq.
     &                   TBL__NORMAL ) prodArea = pins.stepProdArea

                 endif

                    ! Get the routing location for the equipment type in the
                    ! production area specified

!              call tbl_routng_calcLocation (    altRecp.eqpType, 
!     &                                          prodArea, 
!     &                                          routLocation ) 
!
!
!          Logic added for routing with Advanced Equipment
!
                 call tbl_routng_getLocationActl ( altRecp.eqpType,       
     &                                                altRecp.capability, 
     &                                                prodArea, 
     &                                                routLocation,  
     &                                                actl)


!                 if ( actl.desigLocation .eq. ' ' .or.
!     &                   batchLocation .eq. routLocation ) then
!
!     Because we already choose the equipment, the only valid location is where it is
!     all other alternates would not be successful...
!
		  if ( batchLocation .eq. routLocation ) then
                      ! Ok alternate
                      numValidRecipes = numValidRecipes + 1 
                      recpid(numValidRecipes) = altRecp.recpid
                      selection(numValidRecipes) = i
                      title(numValidRecipes) = altRecp.title

                 endif

               endif
            endif

          enddo

        endif
        
        if ( numValidRecipes .gt. 0 ) then

          if ( numValidRecipes .eq. 1 ) then

            choice = 1
            if ( selection(choice) .gt. 0 ) then
              
              if ( batchEqpCapability .ne. ' ' ) then
                call aeqSsp_batch_log (   batchLog, .true.,
     &          'Warning! Lot will use an alternate recipe '//
     &          'for equipment capability '//batchEqpCapability )
	      else
                call aeqSsp_batch_log (   batchLog, .true.,
     &          'Warning! Lot will use an alternate recipe '//
     &          'for equipment type '//batchEqpType )
	      endif
              call tut_beep ( ' ' )

              call fil_check( lot_rdRecpRec_genRecord ( recpid(choice), recp ))
              numAltRecipe = selection (choice) 

            endif

          elseif ( numValidRecipes .gt. 1 ) then

            ! User must choose

            call tut_output ( ' ' )
            call tut_output ( 'Please choose from the following recipes:')
            call tut_output ( ' ' )
            
            do i=1,numValidRecipes

              if ( i .eq. 1 .and. selection(i) .eq. 0 ) then    

                call tut_fao (  '!UL: (Primary)   !AS - !AS',
     &                          %val(i), recpid(i), title(i) )
              else
        
                call tut_fao (  '!UL: (Alternate) !AS - !AS',
     &                          %val(i), recpid(i), title(i) )
              endif
        
            enddo

            call tut_output ( ' ' )
            choice = 1
            status = gut_getIntP (
     &          'Enter the recipe number or RETURN for',
     &          choice,1,numValidRecipes,GUT__STRICT+GUT__EMPTY+GUT__DISPOLD)

            if ( status .eq. GUT__OK_EMPTY ) choice = 1             
            call fil_check( lot_rdRecpRec_genRecord ( recpid(choice), recp ) )
            numAltRecipe = selection (choice) 

          endif

        else

          if ( batchEqpCapability .ne. ' ' ) then
            call aeqSsp_batch_log ( batchLog, .true.,
     &          'Lot has no primary or alternate recipes for capability '//
     &          batchEqpCapability )
 	  else
            call aeqSsp_batch_log ( batchLog, .true.,
     &          'Lot has no primary or alternate recipes for equipment '//
     &          batchEqpType )
	  endif

          if ( .not. (constChk .and. accept)) then
            call aeqSsp_batch_log (     batchLog, .true.,
     &          'Lot constraint detected for equipment '//batchEqpid//
     &          '. Reason: '//rejectReason)
          endif

          call aeqSsp_batch_log ( batchLog, .true.,
     &      'Lot may be at the wrong step. Please check the PROMIS recipe' )
          aeqSsp_batch_selectRecp = .false.

        endif

        return

        end

!++     AEQSSP_BATCH_LISTCOUNT

        subroutine aeqSsp_batch_listCount (     lotBasis,
     &                                          lotCount,
     &                                          lotid,
     &                                          lotSize )

        implicit none   

! Description:

        ! Lists out lots with sizes

! Include files:

! Arguments:

        character*(*)   lotBasis                                ! [R]
        ! - material type (wafers or lots)

        integer*4       lotCount                                ! [R]
        ! - number of lots

        character*(*)   lotId( lotCount )                       ! [R]
        ! - list of lotids

        integer*4       lotSize( lotCount )                     ! [R]
        ! - list of lot sizes

! Routines Used:

! Author:       Mike Bergsma            7/92
!               Ab Initio Software
!
! Modifications:

! Local Variables:

        integer*4       i

        do i=1,lotCount,3

          if ( i .eq. lotCount ) then
            call tut_fao (      
     &                  '!12AS(!3UL !6AS)',
     &                  lotid(i),
     &                  %VAL ( lotSize(i) ),
     &                  lotbasis )
          elseif ( i+1 .eq. lotCount ) then
            call tut_fao (
     &                  '!12AS(!3UL !6AS)   !12AS(!3UL !6AS)',
     &                  lotid(i),
     &                  %VAL ( lotSize(i) ),
     &                  lotBasis,
     &                  lotid(i+1),
     &                  %VAL ( lotSize(i+1) ),
     &                  lotBasis )
          else
            call tut_fao (
     &                 '!12AS(!3UL !6AS)   !12AS(!3UL !6AS)   !12AS(!3UL !6AS)',
     &                  lotid(i),
     &                  %VAL ( lotSize(i) ),
     &                  lotBasis,
     &                  lotid(i+1),
     &                  %VAL ( lotSize(i+1) ),
     &                  lotBasis,
     &                  lotid(i+2),
     &                  %VAL ( lotSize(i+2) ),
     &                  lotBasis )
          endif

        enddo

        return

        end

!++     AEQSSP_BATCH_LISTRECIPE

        subroutine aeqSsp_batch_listRecipe (    lotCount,
     &                                          lotid,
     &                                          lotRecipe )

        implicit none   

! Description:

        ! Lists out lots with recipes

! Include files:

! Arguments:

        integer*4       lotCount                                ! [R]
        ! - number of lots

        character*(*)   lotId( lotCount )                       ! [R]
        ! - list of lotids

        character*(*)   lotRecipe ( lotCount )                  ! [R]
        ! - list of lot recipes

! Routines Used:

! Author:       Mike Bergsma            7/92
!               Ab Initio Software
!
! Modifications:

! Local Variables:

        integer*4       i

        do i=1,lotCount,3

          if ( i .eq. lotCount ) then
            call tut_fao (      
     &                  '!12AS(!8AS)',
     &                  lotid(i),
     &                  lotRecipe(i) )
          elseif ( i+1 .eq. lotCount ) then
            call tut_fao (
     &                  '!12AS(!8AS)     !12AS(!8AS)',
     &                  lotid(i),
     &                  lotRecipe(i),
     &                  lotid(i+1),
     &                  lotRecipe(i+1) )
          else
            call tut_fao (
     &                 '!12AS(!8AS)     !12AS(!8AS)     !12AS(!8AS)',
     &                  lotid(i),
     &                  lotRecipe(i),
     &                  lotid(i+1),
     &                  lotRecipe(i+1),
     &                  lotid(i+2),
     &                  lotRecipe(i+2) )
          endif

        enddo

        return

        end

!++ AEQSSP_BATCH_LOG
!
        subroutine aeqSsp_batch_log ( log, display, message )

!       Log a message if the log flag is set.

        implicit none

! Include files:

        include 'AEQ_SSP:AUTODEF'

! Arguments:

        logical*4       log                             ! [R]
        ! - if true then log it

        logical*4       display                         ! [R]
        ! - if true then display it

        character*(*)   message                         ! [R]
        ! - status text message

! Routines Used:

! Author:       Mike Bergsma            4/95
!               Ab Initio Software
!
! Modifications:

! Local variables:

! Code:

        if ( display ) then
          call tut_output ( ' ' )
          call tut_output ( message )
        endif

        if ( log ) then
          call aeqSsp_autoUtil_logMessage ( message, LOG_UNIT )
        endif

        end

!++ AEQSSP_BATCH_HANDLER
!
        integer*4 function aeqSsp_batch_handler (sigargs, mechargs)

        ! Control_Z and Control_C handler for the BATCH routines.
        ! Check for signal condition of EXIT, QUIT, ^Z or ^C

        implicit none

! Function value:

        ! Always returns 0 to resignal

! Include files:

        include '($SSDEF)'      
        include '($PSLDEF)'     

        include 'FIL:FILDEF'
        include 'FIL:TYPDEF'
        include 'GUT:GUTDEF'
        include 'TUT:TUTDEF'
        include 'AEQ_SSP:AUTODEF'
        include 'AEQ_SSP:BATCHDEF'

! Common areas:

        record /batch_struct/           batch
        common /batch_common/           batch

! Arguments:

        integer*4       sigargs(*)              ! READ
        ! Signal arguments

        integer*4       mechargs(*)             ! READ
        ! Mechanism arguments

! Routines used: 

        external        gut_check       !Signal conditional error
        integer         Sig_Chf_GetCond
        logical         Sig_Chf_MatchCond

! Author:       Mike Bergsma            7/92
!               Ab Initio Software
!
! Modifications:
!
!       Mike Bergsma    1/12/97
!       - modify to support Alpha implementation
!
!       Mike Bergsma    6/2/99
!       - always resignal, no matter what the condition
!
!       Mike Bergsma    6/2/99
!       - update for PROMIS 5.5
!~        

! Local variables:

        integer*4       i,condition
!
! Code:
!
        aeqSsp_batch_handler = 0     ! resignal 

        condition = Sig_Chf_GetCond (sigArgs, mechArgs)  ! get the condtion

        if ( Sig_Chf_MatchCond (sigArgs, mechArgs, TUT__CONTROLC) .or.
     &       Sig_Chf_MatchCond (sigArgs, mechArgs, TUT__EXIT) .or.
     &       Sig_Chf_MatchCond (sigArgs, mechArgs, TUT__QUIT) ) then

          call aeqSsp_batch_log ( batch.log,  .true., 'Interrupt (^C,^Z)' )
          call aeqSsp_batch_abort ()

        else

          call aeqSsp_batch_log ( batch.log,  .true., 'Interrupt (?)' )
          call aeqSsp_batch_abort ()

        endif

        return

        end

!++ AEQSSP_BATCH_ABORT
!
        subroutine aeqSsp_batch_abort ()

        ! Aborts the batch and cleans up the mess

        implicit none

! Include files:

        include '($SSDEF)'      
        include '($PSLDEF)'     

        include 'FIL:FILDEF'
        include 'FIL:TYPDEF'
        include 'GUT:GUTDEF'
        include 'TUT:TUTDEF'
        include 'AEQ_SSP:AUTODEF'
        include 'AEQ_SSP:BATCHDEF'

! Common areas:

        record /batch_struct/           batch
        common /batch_common/           batch

! Arguments:

! Routines used: 

! Author:       Mike Bergsma            6/99
!               Ab Initio Software
!
! Modifications:
!
        integer*4       i

! Code:
!
        do i=1,3
          call tut_beep()
        enddo

        call aeqSsp_batch_log ( batch.log,  .true., 
     &                            'Batch function was aborted!' )

        call aeqSsp_batch_log ( batch.log, .true. , ' ' )

        ! Restore value of process logical OPTION_AUTO if prev defined
        if ( batch.saveOptionLen .gt. 0 ) then
          if ( batch.saveOption(:batch.saveOptionLen) .ne. 'OPTION_AUTO'  .and.
     &		   batch.saveOption(:batch.saveOptionLen) .ne. 'NO_BATCH' ) then
            call lib$set_logical(       'OPTION_AUTO',
     &                                  batch.saveOption(:batch.saveOptionLen),
     &                                  'LNM$PROCESS' )
          else  
            call lib$delete_logical( 'OPTION_AUTO', 'LNM$PROCESS' )
          endif
        endif

        ! Close all files. 
        call fil_closeall()

        ! Restore lock (if set)
        call Tbl_Util_Unlock()

        ! Close the log file if open.
        if ( batch.log ) then
          call aeqSsp_batch_log ( batch.log, .false.,
     &          '+++ End Batch ++++++++++++++++++++++++++++++++++++++++' )
          call aeqSsp_autoutil_closeLog ( )
        endif

        ! Restore pagewait
        call Tut_ReStrWait ()           

        ! Check to see if we should send an event message
        if ( batch.sendEvent ) then

          call aeqSsp_batch_log ( batch.log,  .true.,
     &          TUT_T_REVERSE//TUT_T_BLINK//TUT_T_BOLD//
     &          'Some or all of the batch lots were NOT tracked in.'//
     &          TUT_T_STANDARD )

          call aeqSsp_batch_sendEvent ()

        endif

        return

        end

!++ AEQSSP_BATCH_SENDEVENT
!
        subroutine aeqSsp_batch_sendEvent ()
!
        implicit none

        ! Handy routine to send an event message 

! Include files:

        include '($IODEF)'      
        include '($SSDEF)'      
        include '($PSLDEF)'     

        include 'FIL:FILDEF'
        include 'FIL:TYPDEF'
        include 'GUT:GUTDEF'
        include 'TUT:TUTDEF'
        include 'AEQ_SSP:AUTODEF'
        include 'AEQ_SSP:BATCHDEF'

! Common areas:

        record /batch_struct/           batch
        common /batch_common/           batch

! Arguments:

! Routines used:

        integer*4       sys$assign
        integer*4       sys$qiow
        integer*4       gut_trimblks
        logical*4       aeqSsp_autoUtil_check
        integer*4       aeqSsp_autoUtil_collapse
        logical*4       aeqSsp_autoUtil_getAddress 
        character*8     aeqssp_autoutil_random8
        character*(DATETIME_SIZE)       aeqssp_autoutil_timeStamp       

! Modifications:

! Local Variables:


        integer*4       i,
     &                  status,
     &                  targetLen,
     &                  senderLen,
     &                  messageLen

        integer*2       iosb(4),
     &                  outputChannel

        character*(MESSAGE_SIZE)        message         
        character*(SENDER_SIZE)         sender
        character*(TARGET_SIZE)         target
        character*(INSTANCE_SIZE)       targetInstance
        character*(OBJECT_SIZE)         targetObject
        character*(NODE_SIZE)           targetNode
        integer*4                       targetInstanceLen,
     &                                  targetObjectLen,
     &                                  targetNodeLen

        character*(TOKEN_SIZE)          token( MAX_TOKENS )
        integer*4                       tokenCount
        character*(VALUE_SIZE)          value( MAX_VALUES, MAX_TOKENS )
        integer*4                       valueLen( MAX_VALUES, MAX_TOKENS )
        integer*4                       valueCount( MAX_TOKENS )

        character*8                     tid
        character*(DATETIME_SIZE)       ts

        character*60                    buffer
        integer*4                       bufferLen
        character*(TARGET_SIZE)         mbxRouter
        integer*4                       mbxRouterLen

! Code:

        tid = aeqSsp_autoutil_random8 ()
        ts = aeqSsp_autoUtil_timeStamp( )

        ! Connect to the router mailbox/fifo
	call Lib$Sys_TrnLog( 'AUTOFIFO', bufferLen, buffer,, )
	if ( buffer(:bufferLen) .eq. 'AUTOFIFO' ) then
	  mbxRouter = 'MBX'
          mbxRouterLen = 3
	else
	  mbxRouter = buffer(:bufferLen)
          mbxRouterLen = bufferLen
	endif

	call Lib$Sys_TrnLog( 'AUTOROUTER', bufferLen, buffer,, )
	if ( buffer(:bufferLen) .eq. 'AUTOROUTER' ) then
	  mbxRouter(mbxRouterLen+1:) = '_ROUTER'
          mbxRouterLen = mbxRouterLen + 7
	else
	  mbxRouter(mbxRouterLen+1:) = '_'//buffer(:bufferLen)
          mbxRouterLen = mbxRouterLen + 1 + bufferLen
	endif

        status = sys$assign(
     &          mbxRouter(:mbxRouterLen),
     &          outputChannel,
     &          ,, )
        if ( .not. aeqSsp_autoutil_check( status, 1 ) ) goto 999

        ! Get the complete address specification.
        ! The AUTO_ALARM logical is not required 
        target = 'BATCH_FAIL'
        targetLen = 10
        if ( .not. aeqSsp_autoUtil_getAddress ( target,
     &                                          targetLen,
     &                                          targetInstance,
     &                                          targetInstanceLen,
     &                                          targetObject,
     &                                          targetObjectLen,
     &                                          targetNode,
     &                                          targetNodeLen ) ) goto 999


        ! Construct the sender address
        sender = batch.userid//'.batch'
        senderLen = aeqSsp_autoUtil_collapse ( sender )

        ! Add the TOKENS
        tokenCount = 2
        token(1) = 'LOTID'
        token(2) = 'STATE'
        valueCount(1) = batch.lotCount 
        valueCount(2) = batch.lotCount
        do i=1,batch.lotCount

          value(i,1) = batch.lotid(i)
          valueLen(i,1) = gut_trimblks( value(i,1) )

          if ( batch.isTrackedIn(i) ) then
            value(i,2) = 'RUNNING'
          else
            value(i,2) = 'WAITING'
          endif
          valueLen(i,2) = gut_trimblks( value(i,2) )

        enddo

        tokenCount = 3
        token(3) = 'TEXT'
        valueCount(3) = 24
        do i=1,24
          call aeqSsp_autoMan_snap ( i, value(i,3), valueLen(i,3) )
        enddo

        ! Add the TOKENS
        tokenCount = 4
        token(4) = 'EQPID'
        valueCount(4) = batch.lotCount 
        value(1,4) = batch.eqpid
        valueLen(1,4) = gut_trimblks( value(1,4) )

        call aeqSsp_autoUtil_unparse(   DEFAULT_DELIMITER,
     &                                  target(:targetLen),
     &                                  'EVENT',
     &                                  'MESSAGE',
     &                                  sender(:senderLen),
     &                                  tid, ts,
     &                                  token, 
     &                                  tokenCount, 
     &                                  value,
     &                                  valueLen,
     &                                  valueCount,
     &                                  message, 
     &                                  messageLen ) 

        ! Send message to the AUTOROUTER to process.
        status = sys$qiow(      %val(EFN_MBX_OUT),
     &                          %val(outputChannel),
     &                          %val(IO$_WRITEVBLK+IO$M_NOW+IO$M_NORSWAIT),
     &                          iosb,
     &                          ,,
     &                          %ref(message),
     &                          %val(messageLen),
     &                          ,,, )
        if ( .not. aeqSsp_autoUtil_check( status, iosb(1) ) ) goto 999

999     continue

        end
