!*****************************************************************************!
!                    AUTOMAN Software Source Code.                            !
!                                                                             !
!          ***** Copyright: (c) 1993 Ab Initio Software                       !
!          ***** All rights reserved - Licensed Material.                     !
!          ***** Program property of Ab Initio Software                       !
!                                                                             !
!*****************************************************************************!
!++ AUTOMAN.FOR

! The AUTOmation MANagement routines are distributed into three modules:
!
!	automan.for		! Automation communication routines
!	automan2.for		! General purpose automation utilities
!	automan3.for		! Automation User Interface utilities
!	automan4.for		! Automation Database Utilities

! Externally Callable Routines:

!	aeqSsp_automan_dcop		! Called by tst_enterdata_gatherdata
!	aeqSsp_automan_recipe		! Called by lot_tkin
!	aeqSsp_automan_trackout		! Called by lot_trackout_main
!	aeqSsp_automan_abort		! Called by lot_abortStep
!	aeqSsp_automan_batch		! Called by aeqSsp_batch_tkin

! For Internal Use Only:

!	aeqSsp_autoMan_multiParms	! Get multiple parameters
!	aeqSsp_autoMan_isEnabled	! Test if automation is enabled
!	aeqSsp_autoMan_automan		! Connect to "automan" instance
!	aeqSsp_autoMan_AST_mbx		! Received unsolicited message
!	aeqSsp_autoMan_mbxRead		! Read and process message
!	aeqSsp_autoMan_setAST		! Set AST to receive message
!	aeqSsp_autoMan_sendConnect	! Send connect method message
!	aeqSsp_autoMan_sendMethod	! Send <method> method message
!	aeqSsp_autoMan_sendMessage	! Send MESSAGE method message
!	aeqSsp_autoMan_sendEvent	! Send <method> EVENT method message
!	aeqSsp_autoMan_send		! Send message
!	aeqSsp_autoMan_server		! Service received message
!	aeqSsp_autoMan_DATA		! Service DATA method
!	aeqSsp_autoMan_SETUP		! Service SETUP method
!	aeqSsp_autoMan_CONNECT		! Service CONNECT method
!	aeqSsp_autoMan_DISCONNECT	! Service DISCONNECT and ABORT methods
!	aeqSsp_autoMan_MESSAGE		! Service MESSAGE message
!	aeqSsp_autoMan_HOTKEY		! Service HOTKEY message
!	aeqSsp_autoMan_AST_timer	! Respond to timer
!	aeqSsp_autoMan_setTimerAST	! Set timer
!	aeqSsp_autoMan_handler		! Handle CTRL/Z, QUIT, EXIT
!	aeqSsp_autoMan_handler2		! Handle CTRL/C, CTRL/Y
!	aeqSsp_automan_setCtrlAst 	! Set up a CTRL/C handler
!	aeqSsp_automan_setCtrlAst2 	! Set up a CTRL/C handler
!	aeqSsp_autoMan_terminate	! Terminate an automation session
!	aeqSsp_autoMan_fallOut		! Fall out of an I/O wait
!	aeqSsp_autoMan_check		! Check a status
!	aeqSsp_autoMan_log 		! Output to log file
!	aeqSsp_autoMan_logDebug		! Output to log file and session screen

! Produced by:	Ab Initio Software

! Author:	Michael Bergsma 	1993

! Modifications:
!
!   $Log: automan.for,v $
!   Revision 1.5  2005/09/28 04:42:56  bergsma
!   OPCERT Functionality
!
!   Revision 1.4  2005/09/25 20:10:40  bergsma
!   Up to V3.6.3
!   Add opcert code.
!   Changes to function VT2HTML
!   Bug in EAGAIN waiting for UNIX fifo's or network sockets
!   AUTOROUTER loigical must be uppercase on VMS
!
!   Revision 1.3  2005/08/12 01:16:16  bergsma
!   no message
!
!   Revision 1.7  2005/04/13 13:49:30  bergsma
!   HS 3.5.6
!
!   Revision 1.2  2003/01/16 14:32:07  bergsma
!   V3.1.0
!   Added Modification tags.
!
!
!-

!++ AEQSSP_AUTOMAN_DCOP
!
	subroutine aeqSsp_automan_dcop( finished )

	implicit none

!	This routine is called by Tst_EnterData_Main to perform
!	automatic equipment automation providing the $AUTO_DCOP parameter
!	is defined in the data collection operation.
!
!	The $AUTO_DCOP parameter causes PROMIS to initiate a message
!	conversation with the equipment identified by the value of $AUTO_DCOP
!
!
! Include files:

	include	'FIL:TYPDEF'
	include	'FIL:FILDEF'
	include	'TST:TSTDEF'	
	include	'TST:ENTDEF'	
	include	'GUT:GUTDEF'
	include	'TST:PARMDEF'	
	include 'LOT:FUNDEF'
	include 'PAR:LOTSTDEF'
	include	'LOT:ACTLBUFF'	
	include	'FIL:TESTCDEF'	
	include	'FIL:TRESCDEF'	
	include 'LOT:RECPBUFF'
	include	'TST:TSTWCOM'	

	include	'AEQ_SSP:AUTODEF'
	include	'AEQ_SSP:AUTOTBL'

! Arguments:

	logical*4	finished				! [R/W]
	! - set to true if the data entry was or becomes FULL

! Routines used:

	logical*4	aeqSsp_autoMan_isEnabled
	integer*4	aeqSsp_autoUtil_left_justify
	integer*4	gut_trimblks
	character*(VALUE_SIZE) gut_alltrim
	character*(12)	uaf_user_getName

! Author:	Mike Bergsma		7/92
!		Ab Initio Software

! Modifications:
!
!	Mike Bergsma	3/7/94
!	- if the automation hasn't collected any data, then make sure the
!	  the tst_tstWcom_testResultsEntered variable is set back to .false.
!
!	Mike Bergsma	5/10/94
!	- add USERID to init tokens
!
!	Mike Bergsma	10/3/94
!	- add dummy DCOP header
!
!	Mike Bergsma	2/26/97
!	- add AUTOTYPE token
!
! Local variables:

	parameter	FIRSTTIME = .true.

	logical*4	more,
     &			inputOk,
     &			rowchg, 
     &			colchg

	integer*4	ios,i,numwafers

	integer*4	subs( TRES_X_DIMNUMVALUES )
	data		subs /0,0,0,0/

	character*12	user
	character*15	autoType
	character*7	dataType

	character*(TOKEN_SIZE)		initToken( MAX_TOKENS )
	integer*4			initTokenCount
	character*(VALUE_SIZE)		initValue( MAX_VALUES, MAX_TOKENS )
	integer*4			initValueLen( MAX_VALUES, MAX_TOKENS )
	integer*4			initValueCount( MAX_TOKENS )

	character*(VALUE_SIZE)		route
	integer*4			routeLen

! Code:

	! Allow only data collection functions
	if ( tst_tstWCom_callingFunction .ne. LOT__TRACKING_ENTERTEST .and.
     &	     tst_tstWCom_callingFunction .ne. LOT__TRACKING_CORTEST .and.
     &	     tst_tstWCom_callingFunction .ne. LOT__TRACKING_COROLDDATA .and.
     &	     tst_tstWCom_callingFunction .ne. LOT__TRACKING_LATEDATA .and.
     &	     tst_tstWCom_callingFunction .ne. LOT__GENDATA_ENTERDATA .and.
     &	     tst_tstWCom_callingFunction .ne. LOT__GENDATA_CORRDATA .and.
     &	     tst_tstWCom_callingFunction .ne. LOT__GENDATA_LATEDATA ) return

	! An already finished state means that automation should not be 
	! performed (providing we are not correcting old lot data).
	if ( tst_tstWCom_callingFunction .ne. LOT__TRACKING_COROLDDATA .and.
     &	     finished ) return

	! Initialize
	aeqSsp_autoTbl_canEnterData = .true.
	if ( tst_tstWCom_callingFunction .eq. LOT__GENDATA_ENTERDATA .or.
     &	     tst_tstWCom_callingFunction .eq. LOT__GENDATA_CORRDATA .or.
     &	     tst_tstWCom_callingFunction .eq. LOT__GENDATA_LATEDATA ) then

	  ! Gather general data can take parameters from TEST only
	  aeqSsp_autoTbl_hasLotid = .false.
	  aeqSsp_autoTbl_parmSource =	TST__PARMSOURCETEST

	else

	  ! Lot tracking data collection uses ACTL, RECP, and TEST.
	  ! ( Procedure level parameters are implicit )
	  aeqSsp_autoTbl_hasLotid = .true.
	  aeqSsp_autoTbl_parmSource =	TST__PARMSOURCEACTL+
     &	                       		TST__PARMSOURCERECP+
     &	                       		TST__PARMSOURCETEST

	endif

	if (tst_tstWCom_callingFunction .eq. LOT__TRACKING_ENTERTEST) then
	  autoType = 'ENTERLOTDATA'
	  dataType = 'LOTDATA'
	else if (tst_tstWCom_callingFunction .eq. LOT__TRACKING_CORTEST) then
	  autoType = 'CORRECTLOTDATA'
	  dataType = 'LOTDATA'
	else if (tst_tstWCom_callingFunction .eq. LOT__TRACKING_COROLDDATA) then
	  autoType = 'COROLDLOTDATA'
	  dataType = 'LOTDATA'
	else if (tst_tstWCom_callingFunction .eq. LOT__TRACKING_LATEDATA) then
	  autoType = 'LATELOTDATA'
	  dataType = 'LOTDATA'
	else if (tst_tstWCom_callingFunction .eq. LOT__GENDATA_ENTERDATA)then
	  autoType = 'GATHERGENDATA'
	  dataType = 'GENDATA'
	else if (tst_tstWCom_callingFunction .eq. LOT__GENDATA_CORRDATA) then
	  autoType = 'CORRECTGENDATA'
	  dataType = 'GENDATA'
	else if (tst_tstWCom_callingFunction .eq. LOT__GENDATA_LATEDATA) then
	  autoType = 'LATEGENDATA'
	  dataType = 'GENDATA'
	endif

	aeqSsp_autoTbl_autoType = autoType

	! Translate $AUTO_DCOP parameter and OPTION_AUTO
	if ( aeqSsp_autoMan_isEnabled( 	'DCOP',
     &					dataType,
     &					tres.eqpid,
     &					initToken,
     &					initTokenCount,
     &					initValue,
     &					initValueLen,
     &					initValueCount ) ) then

	  ! Automation is enabled
	  call aeqSsp_autoMan_dcOpHeader( )

	  ! Add lotid to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'LOTID'
	  initValueCount(initTokenCount) = 1
	  initValue(1,initTokenCount) = tres.lotid
	  initValueLen(1,initTokenCount) = gut_trimblks( tres.lotid )

	  ! Add eqpid to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'EQPID'
	  initValueCount(initTokenCount) = 1
	  initValue(1,initTokenCount) = tres.eqpid
	  initValueLen(1,initTokenCount) = gut_trimblks( tres.eqpid )

	  ! Add dcop to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'DCOP'
	  initValueCount(initTokenCount) = 1
	  initValue(1,initTokenCount) = tres.testopno
	  initValueLen(1,initTokenCount) = gut_trimblks( tres.testopno )

	  if ( aeqSsp_autoTbl_hasLotid ) then

	    ! Add LOTTYPE to initialization tokens
	    initTokenCount = initTokenCount + 1
	    initToken(initTokenCount) = 'LOTTYPE'
	    initValueCount(initTokenCount) = 1
	    initValue(1,initTokenCount) = actl.lottype
	    initValueLen(1,initTokenCount) = 
     &		gut_trimblks ( initValue(1,initTokenCount) )

	    ! Add NUMWAFERS to initialization tokens
	    initTokenCount = initTokenCount + 1
	    initToken(initTokenCount) = 'NUMWAFERS'
	    initValueCount(initTokenCount) = 1
	    numWafers = tres.compsToTest
	    write( initValue(1,initTokenCount), '(I2)', iostat=ios) numWafers
	    initValueLen(1,initTokenCount) = 
     &		 aeqSsp_autoUtil_left_justify( initValue(1,initTokenCount) )

	    ! Add NUMWAFS to initialization tokens
	    initTokenCount = initTokenCount + 1
	    initToken(initTokenCount) = 'NUMWAFS'
	    initValueCount(initTokenCount) = 1
	    numWafers = INT ( actl.curMainQty ) ! Convert real to integer
	    write( initValue(1,initTokenCount), '(I2)', iostat=ios) numWafers
	    initValueLen(1,initTokenCount) =
     &		aeqSsp_autoUtil_left_justify( initValue(1,initTokenCount) )

	    ! Add PRIORITY to initialization tokens
	    initTokenCount = initTokenCount + 1
	    initToken(initTokenCount) = 'PRIORITY'
	    initValueCount(initTokenCount) = 1
	    write( initValue(1,initTokenCount),'(I2)',iostat=ios) actl.priority
	    initValueLen(1,initTokenCount) = 
     &		aeqSsp_autoUtil_left_justify( initValue(1,initTokenCount) )

	    ! Add EQPTYPE to initialization tokens
	    initTokenCount = initTokenCount + 1
	    initToken(initTokenCount) = 'EQPTYPE'
	    initValueCount(initTokenCount) = 1
	    initValue(1,initTokenCount) = actl.eqpType
	    initValueLen(1,initTokenCount) = 
     &		gut_trimblks ( initValue(1,initTokenCount) )

	    ! Add PARTID to initialization tokens
	    initTokenCount = initTokenCount + 1
	    initToken(initTokenCount) = 'PARTID'
	    initValueCount(initTokenCount) = 1
	    initValue(1,initTokenCount) = 
     &		gut_alltrim ( actl.partId, GUT__PAD_BLANKS )
	    initValueLen(1,initTokenCount) = 
     &		gut_trimblks ( initValue(1,initTokenCount) )

	    ! Add PROCID to initialization tokens
	    initTokenCount = initTokenCount + 1
	    initToken(initTokenCount) = 'PROCID'
	    initValueCount(initTokenCount) = 1
	    initValue(1,initTokenCount) = 
     &		gut_alltrim ( actl.curPrcdId, GUT__PAD_BLANKS )
	    initValueLen(1,initTokenCount) = 
     &		gut_trimblks ( initValue(1,initTokenCount) )

	    ! Add STEP to initialization tokens
	    initTokenCount = initTokenCount + 1
	    initToken(initTokenCount) = 'STEP'
	    initValueCount(initTokenCount) = 1
	    initValue(1,initTokenCount) = actl.curPrcdCurInstNum
	    initValueLen(1,initTokenCount) = 
     &		gut_trimblks ( initValue(1,initTokenCount) )

	    ! Add ROUTE to initialization tokens
	    initTokenCount = initTokenCount + 1
	    initToken(initTokenCount) = 'ROUTE'
	    initValueCount(initTokenCount) = 1
            route = ' '
	    routeLen = 1 
            i = 1
	    do while ( i .le. actl.prcdStackCount-1 )
              route(routeLen+1:) =	actl.prcdStackPrcdId(i)//'('//
     &					actl.prcdStackCurInstNum(i)//')/'	
	      route = gut_alltrim ( route, GUT__PAD_BLANKS )
	      routeLen = gut_trimblks ( route )
	      i = i + 1
            enddo
	    initValue(1,initTokenCount) = route
	    initValueLen(1,initTokenCount) = routeLen

	    ! Add STAGE to initialization tokens
	    initTokenCount = initTokenCount + 1
	    initToken(initTokenCount) = 'STAGE'
	    initValueCount(initTokenCount) = 1
	    initValue(1,initTokenCount) = actl.stage	  
	    initValueLen(1,initTokenCount) = 
     &		gut_trimblks ( initValue(1,initTokenCount) )

	    ! Add LOCATION to initialization tokens
	    initTokenCount = initTokenCount + 1
	    initToken(initTokenCount) = 'LOCATION'
	    initValueCount(initTokenCount) = 1
	    initValue(1,initTokenCount) = actl.location	  
	    initValueLen(1,initTokenCount) = 
     &		gut_trimblks ( initValue(1,initTokenCount) )

	    ! Add RECPID to initialization tokens
	    initTokenCount = initTokenCount + 1
	    initToken(initTokenCount) = 'RECPID'
	    initValueCount(initTokenCount) = 1
	    initValue(1,initTokenCount) = actl.recpId
	    initValueLen(1,initTokenCount) = 
     &		gut_trimblks ( initValue(1,initTokenCount) )

	  endif

	  ! Add userid to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'USERID'
	  initValueCount(initTokenCount) = 1
	  user = uaf_user_getName()
	  initValue(1,initTokenCount) = user
	  initValueLen(1,initTokenCount) = gut_trimblks( user )

	  ! Add AUTOTYPE to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'AUTOTYPE'
	  initValueCount(initTokenCount) = 1
	  initValue(1,initTokenCount) = aeqSsp_autoTbl_autoType 
	  initValueLen(1,initTokenCount) = gut_trimblks( aeqSsp_autoTbl_autoType )

	  ! Set the indexes for the first available item to enter.
	  call aeqSsp_autoMan_next( 	FIRSTTIME,
     &					aeqSsp_autoTbl_waferIndex,
     &					aeqSsp_autoTbl_siteIndex,
     &					aeqSsp_autoTbl_itemIndex,
     &					aeqSsp_autoTbl_state )

	  ! Keep track ourselves on whether or not data was entered because
	  ! sometimes the tst_tstWcom_testResultsEntered variable will
	  ! become .true. when we really haven't entered anything.
	  aeqSsp_autoTbl_hasEnteredData = .false.

	  ! Connect to the equipment 
	  call aeqSsp_autoMan_automan (	initToken,
     &					initTokenCount,
     &					initValue,
     &					initValueLen,
     &					initValueCount) 

	  ! Exit through the handler system if automation failed
	  if ( .not. aeqSsp_autoTbl_functionStatus ) call tut_exit ()

	  ! Make sure the testResultsEntered variable is set correctly.
	  tst_tstWcom_testResultsEntered = aeqSsp_autoTbl_hasEnteredData 

	  ! Find out what happened (providing we didn't TUT_EXIT)
	  if ( tst_tstWcom_testResultsEntered ) then

	    ! Values were automatically entered

	    if ( tst_tstWcom_isEnterTest ) then

	      ! Determine if there is any data left to enter.
		
	      ! The following 3 function calls are required to
	      ! determine if there are any unentered values.

	      call tst_screenUtl_updateCurVals( rowchg, colchg )
	      call tst_etestUtil_valEntered()
	      call tst_etestUtil_findItem(	more,
     &						inputOK,
     &						tst_tstWcom_curResNum,
     &						subs,
     &						.false. )

	      ! Data entry is finished if there are no more values to enter.
	      finished = .not. more

	      if ( .not. finished ) then

	        ! Reformat the column and row areas, based on what was entered.
	        call Tst_ScreenUtl_formatCLA()
	        call Tst_ScreenUtl_formatRLA()

  	      endif

	    endif	

	  endif 

	  ! Put up the dcOp banner.
	  call tst_list_header ( TST__PARMSOURCEOPER )

	endif

	call Tst_ParmUtil_TstPrmComInit( .false. )

	return

	end

!++ AEQSSP_AUTOMAN_RECIPE
!
	subroutine aeqSsp_automan_recipe( userid, autotype )

	implicit none

!	This routine is called by Lot_Tkin_TrackIn() to perform
!	automatic equipment automation providing the $AUTO_RECIPE parameter
!	is defined in the trackin recipe.
!
!	The $AUTO_RECIPE parameter causes PROMIS to initiate a message
!	conversation with the equipment identified by the value of $AUTO_RECIPE
!
! Include files:

	include	'FIL:TYPDEF'
	include	'FIL:FILDEF'
	include	'TST:TSTDEF'	
	include	'TST:ENTDEF'	
	include	'GUT:GUTDEF'
	include	'TST:PARMDEF'	
	include 'LOT:FUNDEF'
	include 'PAR:LOTSTDEF'
	include	'LOT:ACTLBUFF'	
	include 'LOT:RECPBUFF'

	include	'AEQ_SSP:AUTODEF'
	include	'AEQ_SSP:AUTOTBL'

! Arguments:

	character*(*)	userId					! [R]
	! User name
	
	character*(*)	autoType				! [R]
	! Value for AUTOTYPE token
	! - TRACKIN from lot_tkin_main
	! - CONTINUE from aeqssp_automan_continue

! Routines used:

	logical*4	aeqSsp_autoMan_isEnabled
	integer*4	aeqSsp_autoUtil_left_justify
	integer*4	gut_trimblks
	character*(VALUE_SIZE) gut_alltrim

! Author:	Mike Bergsma		7/92
!		Ab Initio Software
!
! Modifications:
!
!	Erick Jordan	2/7/94
!	- add USERID to initialization tokens
!
!	Mike Bergsma	2/26/97
!	- add AUTOTYPE token
!	- add PARTID,PRCDID,STEP,STAGE,RECPID,LOCATION,LOTTYPE,
!	  PRIORITY,NUMWAFERS tokens
!
!	Mike Bergsma	4/18/97
!	- typo correction, token is PROCID , not PRCDID
!
!	Mike Bergsma	9/8/97
!	- added autotype argument.
!
! Local variables:

	integer*4			ios,i,
     &					numWafers
	character*(TOKEN_SIZE)		initToken( MAX_TOKENS )
	integer*4			initTokenCount
	character*(VALUE_SIZE)		initValue( MAX_VALUES, MAX_TOKENS )
	integer*4			initValueLen( MAX_VALUES, MAX_TOKENS )
	integer*4			initValueCount( MAX_TOKENS )

	character*(VALUE_SIZE)		route
	integer*4			routeLen

! Code:

	! Initialize
	aeqSsp_autoTbl_canEnterData = .false.
	aeqSsp_autoTbl_hasLotid = .true.
	aeqSsp_autoTbl_parmSource =	TST__PARMSOURCEACTL+
     &	                       		TST__PARMSOURCERECP
	aeqSsp_autoTbl_autoType = autotype
	! Translate $AUTO_RECIPE parameter and OPTION_AUTO
	if ( aeqSsp_autoMan_isEnabled( 	'RECIPE',
     &					'IN',
     &					actl.eqpid,
     &					initToken,
     &					initTokenCount,
     &					initValue,
     &					initValueLen,
     &					initValueCount ) ) then

	  ! Automation is enabled
	  call aeqSsp_autoMan_tkinHeader(	actl.lotid,
     &						actl.eqpid,
     &						actl.partId,
     &						actl.curPrcdId,
     &						actl.curPrcdCurInstNum,
     &						actl.stage,
     &						actl.recpid,
     &						'Trackin: '//recp.title,
     &						userid )

	  ! Add LOTID to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'LOTID'
	  initValueCount(initTokenCount) = 1
	  initValue(1,initTokenCount) = actl.lotid
	  initValueLen(1,initTokenCount) = gut_trimblks( actl.lotid )

	  ! Add LOTTYPE to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'LOTTYPE'
	  initValueCount(initTokenCount) = 1
	  initValue(1,initTokenCount) = actl.lottype
	  initValueLen(1,initTokenCount) = gut_trimblks( actl.lottype )

	  ! Add NUMWAFERS to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'NUMWAFERS'
	  initValueCount(initTokenCount) = 1
	  numWafers = INT ( actl.curMainQty	) ! Convert real to integer
	  write( initValue(1,initTokenCount), '(I2)', iostat=ios) numWafers
	  initValueLen(1,initTokenCount) = 
     &		aeqSsp_autoUtil_left_justify( initValue(1,initTokenCount) )

	  ! Add PRIORITY to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'PRIORITY'
	  initValueCount(initTokenCount) = 1
	  write( initValue(1,initTokenCount), '(I2)', iostat=ios) actl.priority
	  initValueLen(1,initTokenCount) = 
     &		aeqSsp_autoUtil_left_justify( initValue(1,initTokenCount) )

	  ! Add EQPID to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'EQPID'
	  initValueCount(initTokenCount) = 1
	  initValue(1,initTokenCount) = actl.eqpid
	  initValueLen(1,initTokenCount) = gut_trimblks( actl.eqpid )

	  ! Add EQPTYPE to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'EQPTYPE'
	  initValueCount(initTokenCount) = 1
	  initValue(1,initTokenCount) = actl.eqpType
	  initValueLen(1,initTokenCount) = gut_trimblks( actl.eqpType )

	  ! Add PARTID to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'PARTID'
	  initValueCount(initTokenCount) = 1
	  initValue(1,initTokenCount) = gut_alltrim (	actl.partId, 
     &							GUT__PAD_BLANKS )
	  initValueLen(1,initTokenCount) = 
     &		gut_trimblks ( initValue(1,initTokenCount) )

	  ! Add PROCID to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'PROCID'
	  initValueCount(initTokenCount) = 1
	  initValue(1,initTokenCount) = gut_alltrim (	actl.curPrcdId, 
     &							GUT__PAD_BLANKS )
	  initValueLen(1,initTokenCount) = 
     &		gut_trimblks ( initValue(1,initTokenCount) )

	  ! Add STEP to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'STEP'
	  initValueCount(initTokenCount) = 1
	  initValue(1,initTokenCount) = actl.curPrcdCurInstNum
	  initValueLen(1,initTokenCount) = gut_trimblks( actl.curPrcdCurInstNum )

	  ! Add ROUTE to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'ROUTE'
	  initValueCount(initTokenCount) = 1
          route = ' '
	  routeLen = 1 
          i = 1
	  do while ( i .le. actl.prcdStackCount-1 )
            route(routeLen+1:) =	actl.prcdStackPrcdId(i)//'('//
     &					actl.prcdStackCurInstNum(i)//')/'	
	    route = gut_alltrim ( route, GUT__PAD_BLANKS )
	    routeLen = gut_trimblks ( route )
	    i = i + 1
          enddo
	  initValue(1,initTokenCount) = route
	  initValueLen(1,initTokenCount) = routeLen

	  ! Add STAGE to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'STAGE'
	  initValueCount(initTokenCount) = 1
	  initValue(1,initTokenCount) = actl.stage	  
	  initValueLen(1,initTokenCount) = gut_trimblks( actl.stage )

	  ! Add LOCATION to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'LOCATION'
	  initValueCount(initTokenCount) = 1
	  initValue(1,initTokenCount) = actl.location	  
	  initValueLen(1,initTokenCount) = gut_trimblks( actl.location )

	  ! Add RECPID to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'RECPID'
	  initValueCount(initTokenCount) = 1
	  initValue(1,initTokenCount) = actl.recpId
	  initValueLen(1,initTokenCount) = gut_trimblks( actl.recpId )

	  ! Add USERID to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'USERID'
	  initValueCount(initTokenCount) = 1
	  initValue(1,initTokenCount) = userId
	  initValueLen(1,initTokenCount) = gut_trimblks( userId )

	  ! Add AUTOTYPE to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'AUTOTYPE'
	  initValueCount(initTokenCount) = 1
	  initValue(1,initTokenCount) = aeqSsp_autoTbl_autoType 
	  initValueLen(1,initTokenCount) = gut_trimblks( aeqSsp_autoTbl_autoType )

	  ! Connect to the equipment 
	  call aeqSsp_autoMan_automan (	initToken,
     &					initTokenCount,
     &					initValue,
     &					initValueLen,
     &					initValueCount)

	  ! Exit through the handler system if automation failed
	  if ( .not. aeqSsp_autoTbl_functionStatus ) call tut_exit ()

	endif

	return

	end

!++ AEQSSP_AUTOMAN_TRACKOUT
!
	subroutine aeqSsp_automan_trackout ( lotid, userid )

	implicit none

!	This routine is called by Lot_TrackOut_main () to perform
!	automatic equipment trackout provided by the $AUTO_RECIPE parameter
!	defined in the trackin recipe.
!
!	The $AUTO_RECIPE parameter causes PROMIS to initiate a message
!	conversation with the equipment identified by the value of $AUTO_RECIPE
!
! Include files:

	include	'FIL:TYPDEF'
	include	'FIL:FILDEF'
	include	'TST:TSTDEF'	
	include	'TST:ENTDEF'	
	include	'GUT:GUTDEF'
	include	'TST:PARMDEF'	
	include 'LOT:FUNDEF'
	include 'PAR:LOTSTDEF'
	include	'LOT:ACTLBUFF'	
	include 'LOT:RECPBUFF'

	include	'AEQ_SSP:AUTODEF'
	include	'AEQ_SSP:AUTOTBL'

! Arguments:

	character*(*)	lotid					! [R]
	! Lot id

	character*(*)	userId					! [R]
	! User Id

! Routines used:

	integer*4	fil_open
	integer*4	fil_getRec
	integer*4	lot_rdRecpRec_genRecord 
	logical*4	aeqSsp_autoMan_isEnabled
	integer*4	gut_trimblks
	integer*4	aeqSsp_autoUtil_left_justify
	character*(VALUE_SIZE) gut_alltrim

! Author:	Mike Bergsma		7/92
!		Ab Initio Software
!
! Modifications:
!
! Local variables:

	integer*4			ios,i,
     &					numWafers
	character*(TOKEN_SIZE)		initToken( MAX_TOKENS )
	integer*4			initTokenCount
	character*(VALUE_SIZE)		initValue( MAX_VALUES, MAX_TOKENS )
	integer*4			initValueLen( MAX_VALUES, MAX_TOKENS )
	integer*4			initValueCount( MAX_TOKENS )

	character*(VALUE_SIZE)		route
	integer*4			routeLen

! Code:

	! Initialize
	aeqSsp_autoTbl_canEnterData = .false.
	aeqSsp_autoTbl_hasLotid = .true.
	aeqSsp_autoTbl_parmSource =	TST__PARMSOURCEACTL+
     &	                       		TST__PARMSOURCERECP
	aeqSsp_autoTbl_autoType = 'TRACKOUT'

	! Read RECP record into the global buffer
	call gut_check ( fil_open ( FIL__RECP, FIL__READONLY))
	call fil_check( lot_rdRecpRec_genRecord ( actl.recpid, recp ) )
	
	! Translate $AUTO_RECIPE parameter and OPTION_AUTO
	if ( aeqSsp_autoMan_isEnabled( 	'RECIPE',
     &					'OUT',
     &					actl.eqpid,
     &					initToken,
     &					initTokenCount,
     &					initValue,
     &					initValueLen,
     &					initValueCount ) ) then

	  ! Automation is enabled
	  call aeqSsp_autoMan_tkinHeader(	actl.lotid,
     &						actl.eqpid,
     &						actl.partId,
     &						actl.curPrcdId,
     &						actl.curPrcdCurInstNum,
     &						actl.stage,
     &						actl.recpid,
     &						'Trackout: '//recp.title,
     &						userid )

	  ! Add LOTID to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'LOTID'
	  initValueCount(initTokenCount) = 1
	  initValue(1,initTokenCount) = actl.lotid
	  initValueLen(1,initTokenCount) = gut_trimblks( actl.lotid )

	  ! Add LOTTYPE to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'LOTTYPE'
	  initValueCount(initTokenCount) = 1
	  initValue(1,initTokenCount) = actl.lottype
	  initValueLen(1,initTokenCount) = gut_trimblks( actl.lottype )

	  ! Add NUMWAFERS to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'NUMWAFERS'
	  initValueCount(initTokenCount) = 1
	  numWafers = INT ( actl.curMainQty	) ! Convert real to integer
	  write( initValue(1,initTokenCount), '(I2)', iostat=ios) numWafers
	  initValueLen(1,initTokenCount) = 
     &		aeqSsp_autoUtil_left_justify( initValue(1,initTokenCount) )

	  ! Add PRIORITY to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'PRIORITY'
	  initValueCount(initTokenCount) = 1
	  write( initValue(1,initTokenCount), '(I2)', iostat=ios) actl.priority
	  initValueLen(1,initTokenCount) = 
     &		aeqSsp_autoUtil_left_justify( initValue(1,initTokenCount) )

	  ! Add EQPID to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'EQPID'
	  initValueCount(initTokenCount) = 1
	  initValue(1,initTokenCount) = actl.eqpid
	  initValueLen(1,initTokenCount) = gut_trimblks( actl.eqpid )

	  ! Add EQPTYPE to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'EQPTYPE'
	  initValueCount(initTokenCount) = 1
	  initValue(1,initTokenCount) = actl.eqpType
	  initValueLen(1,initTokenCount) = gut_trimblks( actl.eqpType )

	  ! Add PARTID to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'PARTID'
	  initValueCount(initTokenCount) = 1
	  initValue(1,initTokenCount) = gut_alltrim (	actl.partId, 
     &							GUT__PAD_BLANKS )
	  initValueLen(1,initTokenCount) = 
     &		gut_trimblks ( initValue(1,initTokenCount) )

	  ! Add PROCID to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'PROCID'
	  initValueCount(initTokenCount) = 1
	  initValue(1,initTokenCount) = gut_alltrim ( 	actl.curPrcdId, 
     &							GUT__PAD_BLANKS )
	  initValueLen(1,initTokenCount) = 
     &		gut_trimblks ( initValue(1,initTokenCount) )

	  ! Add STEP to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'STEP'
	  initValueCount(initTokenCount) = 1
	  initValue(1,initTokenCount) = actl.curPrcdCurInstNum
	  initValueLen(1,initTokenCount) = gut_trimblks( actl.curPrcdCurInstNum )

	  ! Add ROUTE to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'ROUTE'
	  initValueCount(initTokenCount) = 1
          route = ' '
	  routeLen = 1 
          i = 1
	  do while ( i .le. actl.prcdStackCount-1 )
            route(routeLen+1:) =	actl.prcdStackPrcdId(i)//'('//
     &					actl.prcdStackCurInstNum(i)//')/'	
	    route = gut_alltrim ( route, GUT__PAD_BLANKS )
	    routeLen = gut_trimblks ( route )
            i = i + 1
          enddo
	  initValue(1,initTokenCount) = route
	  initValueLen(1,initTokenCount) = routeLen

  	  ! Add STAGE to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'STAGE'
	  initValueCount(initTokenCount) = 1
	  initValue(1,initTokenCount) = actl.stage	  
	  initValueLen(1,initTokenCount) = gut_trimblks( actl.stage )

	  ! Add LOCATION to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'LOCATION'
	  initValueCount(initTokenCount) = 1
	  initValue(1,initTokenCount) = actl.location	  
	  initValueLen(1,initTokenCount) = gut_trimblks( actl.location )

	  ! Add RECPID to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'RECPID'
	  initValueCount(initTokenCount) = 1
	  initValue(1,initTokenCount) = actl.recpId
	  initValueLen(1,initTokenCount) = gut_trimblks( actl.recpId )

	  ! Add USERID to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'USERID'
	  initValueCount(initTokenCount) = 1
	  initValue(1,initTokenCount) = userId
	  initValueLen(1,initTokenCount) = gut_trimblks( userId )

	  ! Add AUTOTYPE to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'AUTOTYPE'
	  initValueCount(initTokenCount) = 1
	  initValue(1,initTokenCount) = aeqSsp_autoTbl_autoType 
	  initValueLen(1,initTokenCount) = gut_trimblks( aeqSsp_autoTbl_autoType )

	  ! Connect to the equipment 
	  call aeqSsp_autoMan_automan (	initToken,
     &					initTokenCount,
     &					initValue,
     &					initValueLen,
     &					initValueCount)

	  ! Exit through the handler system if automation failed
	  if ( .not. aeqSsp_autoTbl_functionStatus ) call tut_exit ()

	endif

	return

	end

!++ AEQSSP_AUTOMAN_ABORT
!
	subroutine aeqSsp_automan_abort ( userid )

	implicit none

!	This routine is called by () to perform
!	automatic equipment trackout provided by the $AUTO_RECIPE parameter
!	defined in the trackin recipe.
!
!	The $AUTO_RECIPE parameter causes PROMIS to initiate a message
!	conversation with the equipment identified by the value of $AUTO_RECIPE
!
! Include files:

	include	'FIL:TYPDEF'
	include	'FIL:FILDEF'
	include	'TST:TSTDEF'	
	include	'TST:ENTDEF'	
	include	'GUT:GUTDEF'
	include	'TST:PARMDEF'	
	include 'LOT:FUNDEF'
	include 'PAR:LOTSTDEF'
	include	'LOT:ACTLBUFF'	
	include 'LOT:RECPBUFF'

	include	'AEQ_SSP:AUTODEF'
	include	'AEQ_SSP:AUTOTBL'

! Arguments:

	character*(*)	userId					! [R]
	! User name

! Routines used:

	logical*4	aeqSsp_autoMan_isEnabled
	integer*4	gut_trimblks
	integer*4	aeqSsp_autoUtil_left_justify
	integer*4	lot_rdRecpRec_genRecord 
	character*(VALUE_SIZE) gut_alltrim

! Author:	Mike Bergsma		7/92
!		Ab Initio Software
!
! Modifications:
!
! Local variables:

	integer*4			ios,i,
     &					numWafers
	character*(TOKEN_SIZE)		initToken( MAX_TOKENS )
	integer*4			initTokenCount
	character*(VALUE_SIZE)		initValue( MAX_VALUES, MAX_TOKENS )
	integer*4			initValueLen( MAX_VALUES, MAX_TOKENS )
	integer*4			initValueCount( MAX_TOKENS )

	character*(VALUE_SIZE)		route
	integer*4			routeLen

! Code:

	! Initialize
	aeqSsp_autoTbl_canEnterData = .false.
	aeqSsp_autoTbl_hasLotid = .true.
	aeqSsp_autoTbl_parmSource =	TST__PARMSOURCEACTL+
     &	                       		TST__PARMSOURCERECP
	aeqSsp_autoTbl_autoType = 'ABORT'

	call fil_check( lot_rdRecpRec_genRecord ( actl.recpid, recp ) )

	! Translate $AUTO_RECIPE parameter and OPTION_AUTO
	if ( aeqSsp_autoMan_isEnabled( 	'RECIPE',
     &					'ABORT',
     &					actl.eqpid,
     &					initToken,
     &					initTokenCount,
     &					initValue,
     &					initValueLen,
     &					initValueCount ) ) then

	  ! Automation is enabled
	  call aeqSsp_autoMan_tkinHeader(	actl.lotid,
     &						actl.eqpid,
     &						actl.partId,
     &						actl.curPrcdId,
     &						actl.curPrcdCurInstNum,
     &						actl.stage,
     &						actl.recpid,
     &						'Abort: '//recp.title,
     &						userid )

	  ! Add LOTID to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'LOTID'
	  initValueCount(initTokenCount) = 1
	  initValue(1,initTokenCount) = actl.lotid
	  initValueLen(1,initTokenCount) = gut_trimblks( actl.lotid )

	  ! Add LOTTYPE to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'LOTTYPE'
	  initValueCount(initTokenCount) = 1
	  initValue(1,initTokenCount) = actl.lottype
	  initValueLen(1,initTokenCount) = gut_trimblks( actl.lottype )

	  ! Add NUMWAFERS to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'NUMWAFERS'
	  initValueCount(initTokenCount) = 1
	  numWafers = INT ( actl.curMainQty ) ! Convert real to integer
	  write( initValue(1,initTokenCount), '(I2)', iostat=ios) numWafers
	  initValueLen(1,initTokenCount) = 
     &		aeqSsp_autoUtil_left_justify( initValue(1,initTokenCount) )

	  ! Add PRIORITY to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'PRIORITY'
	  initValueCount(initTokenCount) = 1
	  write( initValue(1,initTokenCount), '(I2)', iostat=ios) actl.priority
	  initValueLen(1,initTokenCount) = 
     &		aeqSsp_autoUtil_left_justify( initValue(1,initTokenCount) )

	  ! Add EQPID to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'EQPID'
	  initValueCount(initTokenCount) = 1
	  initValue(1,initTokenCount) = actl.eqpid
	  initValueLen(1,initTokenCount) = gut_trimblks( actl.eqpid )

	  ! Add EQPTYPE to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'EQPTYPE'
	  initValueCount(initTokenCount) = 1
	  initValue(1,initTokenCount) = actl.eqpType
	  initValueLen(1,initTokenCount) = gut_trimblks( actl.eqpType )

	  ! Add PARTID to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'PARTID'
	  initValueCount(initTokenCount) = 1
	  initValue(1,initTokenCount) = gut_alltrim (	actl.partId, 
     &							GUT__PAD_BLANKS )
	  initValueLen(1,initTokenCount) = 
     &		gut_trimblks ( initValue(1,initTokenCount) )

	  ! Add PROCID to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'PROCID'
	  initValueCount(initTokenCount) = 1
	  initValue(1,initTokenCount) = gut_alltrim (	actl.curPrcdId, 
     &							GUT__PAD_BLANKS )
	  initValueLen(1,initTokenCount) = 
     &		gut_trimblks ( initValue(1,initTokenCount) )

	  ! Add STEP to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'STEP'
	  initValueCount(initTokenCount) = 1
	  initValue(1,initTokenCount) = actl.curPrcdCurInstNum
	  initValueLen(1,initTokenCount) = gut_trimblks( actl.curPrcdCurInstNum )

	  ! Add ROUTE to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'ROUTE'
	  initValueCount(initTokenCount) = 1
          route = ' '
	  routeLen = 1 
          i = 1
	  do while ( i .le. actl.prcdStackCount-1 )
            route(routeLen+1:) =	actl.prcdStackPrcdId(i)//'('//
     &					actl.prcdStackCurInstNum(i)//')/'	
	    route = gut_alltrim ( route, GUT__PAD_BLANKS )
	    routeLen = gut_trimblks ( route )
            i = i + 1
          enddo
	  initValue(1,initTokenCount) = route
	  initValueLen(1,initTokenCount) = routeLen

	  ! Add STAGE to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'STAGE'
	  initValueCount(initTokenCount) = 1
	  initValue(1,initTokenCount) = actl.stage	  
	  initValueLen(1,initTokenCount) = gut_trimblks( actl.stage )

	  ! Add LOCATION to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'LOCATION'
	  initValueCount(initTokenCount) = 1
	  initValue(1,initTokenCount) = actl.location	  
	  initValueLen(1,initTokenCount) = gut_trimblks( actl.location )

	  ! Add RECPID to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'RECPID'
	  initValueCount(initTokenCount) = 1
	  initValue(1,initTokenCount) = actl.recpId
	  initValueLen(1,initTokenCount) = gut_trimblks( actl.recpId )

	  ! Add USERID to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'USERID'
	  initValueCount(initTokenCount) = 1
	  initValue(1,initTokenCount) = userId
	  initValueLen(1,initTokenCount) = gut_trimblks( userId )

	  ! Add AUTOTYPE to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'AUTOTYPE'
	  initValueCount(initTokenCount) = 1
	  initValue(1,initTokenCount) = aeqSsp_autoTbl_autoType 
	  initValueLen(1,initTokenCount) = gut_trimblks( aeqSsp_autoTbl_autoType )

	  ! Connect to the equipment 
	  call aeqSsp_autoMan_automan (	initToken,
     &					initTokenCount,
     &					initValue,
     &					initValueLen,
     &					initValueCount)

	  ! Exit through the handler system if automation failed
	  if ( .not. aeqSsp_autoTbl_functionStatus ) call tut_exit ()

	endif

	return

	end

!++ AEQSSP_AUTOMAN_BATCH
!
	subroutine aeqSsp_automan_batch( batch )

	implicit none

!	This routine is called by Lot_Tkin_TrackIn() to perform
!	automatic equipment automation providing the $AUTO_RECIPE parameter
!	is defined in the trackin recipe.
!
!	The $AUTO_RECIPE parameter causes PROMIS to initiate a message
!	conversation with the equipment identified by the value of $AUTO_RECIPE
!

! Include files:

	include	'FIL:TYPDEF'
	include	'GUT:GUTDEF'
	include	'TST:PARMDEF'
	include	'AEQ_SSP:AUTODEF'
	include	'AEQ_SSP:AUTOTBL'
	include	'AEQ_SSP:BATCHDEF'

! Arguments:

	record	/batch_struct/	batch				! [R]
	! - batch structure

! Routines used:

	integer*4	gut_trimblks
	character*(VALUE_SIZE) gut_alltrim
	logical*4	aeqSsp_autoMan_isEnabled
	integer*4	aeqSsp_autoUtil_left_justify
	logical*4	aeqSsp_automan_check

! Author:	Mike Bergsma		7/92
!		Ab Initio Software
!
! Modifications:
!
!	Erick Jordan	2/7/94
!	- add USERID to initialization tokens
!
!	Mike Bergsma	2/26/97
!	- add AUTOTYPE token
!
!	Mike Bergsma	4/18/97
!	- add new tokens
!	  LOTTYPE (array)
!	  PRIORITY (array)
!	  EQPTYPE
!	  PARTID (array)
!	  PROCID (array)
!	  STEP (array)
!	  STAGE (array)
!	  LOCATION
!	  
!
! Local variables:

	integer*4			i,j,k,
     &					ios


	logical*4			found

	character*(TOKEN_SIZE)		initToken( MAX_TOKENS )
	integer*4			initTokenCount
	character*(VALUE_SIZE)		initValue( MAX_VALUES, MAX_TOKENS )
	integer*4			initValueLen( MAX_VALUES, MAX_TOKENS )
	integer*4			initValueCount( MAX_TOKENS )

! Code:

	! Initialize
	aeqSsp_autoTbl_canEnterData = .false.
	aeqSsp_autoTbl_hasLotid = .false.
	aeqSsp_autoTbl_parmSource = TST__PARMSOURCERECP
	if ( batch.isAddtoBatch ) then
	  aeqSsp_autoTbl_autoType = 'BATCHADD'
	else
	  aeqSsp_autoTbl_autoType = 'BATCH'
	endif

	! Translate $AUTO_RECIPE parameter and OPTION_AUTO
	if ( aeqSsp_autoMan_isEnabled( 	'RECIPE',
     &					'IN',
     &					batch.eqpid,
     &					initToken,
     &					initTokenCount,
     &					initValue,
     &					initValueLen,
     &					initValueCount ) ) then

	  ! Automation is enabled
	  call aeqSsp_autoMan_batchHeader(	batch.lotCount,
     &						batch.lotid,
     &						batch.eqpId,
     &						batch.eqpType,
     &						batch.mainRecpId,
     &     					batch.title,
     &						batch.userId )

	  ! Substitute singular initToken parameter values with array batch 
	  ! parameter values
	  do i=1,initTokenCount

	    ! Get next init token.	
	    found = .false.
	    j = 1

	    do while ( j .le. batch.numParms .and. .not. found )

	      ! Get next batch parameter	

	      if ( initToken(i) .eq. batch.parmName(j) ) then

		found = .true.
	        initValueCount(i) = batch.lotCount
		do k=1,batch.lotCount
	          initValue(k,i) = batch.parmVal(k,j)
	          initValueLen(k,i) = gut_trimblks ( initValue(k,i) ) 
		enddo

	      else
	        j = j + 1	
	      endif
	    enddo
	  enddo
	  
	  ! Add lots to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'LOTIDS'
	  initValueCount(initTokenCount) = batch.lotCount
	  do i=1,batch.lotCount
	    initValue(i,initTokenCount) = batch.lotid(i)
	    initValueLen(i,initTokenCount) = gut_trimblks ( batch.lotid(i) )
	  enddo

	  ! Add LOTTYPE to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'LOTTYPE'
	  initValueCount(initTokenCount) = batch.lotCount
	  do i=1,batch.lotCount
	    initValue(i,initTokenCount) = batch.lotType(i)
	    initValueLen(i,initTokenCount) = gut_trimblks ( batch.lotType(i) )
	  enddo

	  ! Add NUMWAFS to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'NUMWAFS'
	  initValueCount(initTokenCount) = batch.lotCount
	  do i=1,batch.lotCount
	    write( initValue(i,initTokenCount), '(I3)', iostat=ios) 
     &		batch.numComps(i)
	    initValueLen(i,initTokenCount) = 
     &		aeqSsp_autoUtil_left_justify( initValue(i,initTokenCount) )
	  enddo

	  ! Add PRIORITY to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'PRIORITY'
	  initValueCount(initTokenCount) = batch.lotCount
	  do i=1,batch.lotCount
	    write( initValue(i,initTokenCount), '(I2)', iostat=ios) 
     &	        batch.priority(i)
	    initValueLen(i,initTokenCount) = 
     &		aeqSsp_autoUtil_left_justify( initValue(i,initTokenCount) )
	  enddo

	  ! Add EQPID to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'EQPID'
	  initValueCount(initTokenCount) = 1
	  initValue(1,initTokenCount) = batch.eqpid
	  initValueLen(1,initTokenCount) = gut_trimblks( batch.eqpid )

	  ! Add EQPTYPE to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'EQPTYPE'
	  initValueCount(initTokenCount) = 1
	  initValue(1,initTokenCount) = batch.eqpType
	  initValueLen(1,initTokenCount) = gut_trimblks( batch.eqpType )

	  ! Add PARTID to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'PARTID'
	  initValueCount(initTokenCount) = batch.lotCount
	  do i=1,batch.lotCount
	    initValue(i,initTokenCount) = 
     &		gut_alltrim ( batch.partId(i), GUT__PAD_BLANKS )
	    initValueLen(i,initTokenCount) = 
     &		gut_trimblks ( initValue(i,initTokenCount) )
	  enddo

	  ! Add PROCID to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'PROCID'
	  initValueCount(initTokenCount) = batch.lotCount
	  do i=1,batch.lotCount
	    initValue(i,initTokenCount) = 
     &		gut_alltrim ( batch.procId(i), GUT__PAD_BLANKS )
	    initValueLen(i,initTokenCount) = 
     &		gut_trimblks ( initValue(i,initTokenCount) )
	  enddo

	  ! Add STEP to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'STEP'
	  initValueCount(initTokenCount) = batch.lotCount
	  do i=1,batch.lotCount
	    initValue(i,initTokenCount) = batch.step(i)
	    initValueLen(i,initTokenCount) = gut_trimblks ( batch.step(i) )
	  enddo

	  ! Add ROUTE to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'ROUTE'
	  initValueCount(initTokenCount) = batch.lotCount 
	  do i=1,batch.lotCount
	    initValue(i,initTokenCount) = batch.route(i)
	    initValueLen(i,initTokenCount) = gut_trimblks ( batch.route(i) )
	  enddo

	  ! Add STAGE to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'STAGE'
	  initValueCount(initTokenCount) = batch.lotCount
	  do i=1,batch.lotCount
	    initValue(i,initTokenCount) = batch.stage(i)
	    initValueLen(i,initTokenCount) = gut_trimblks ( batch.stage(i) )
	  enddo

	  ! Add LOCATION to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'LOCATION'
	  initValueCount(initTokenCount) = 1
	  initValue(1,initTokenCount) = batch.curLocation	  
	  initValueLen(1,initTokenCount) = gut_trimblks( batch.curLocation )

	  ! Add recipe to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'RECIPES'
	  initValueCount(initTokenCount) = batch.lotCount
	  do i=1,batch.lotCount
	    initValue(i,initTokenCount) = batch.recpid(i)
	    initValueLen(i,initTokenCount) = gut_trimblks ( batch.recpid(i) )
	  enddo

	  ! Add USERID to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'USERID'
	  initValueCount(initTokenCount) = 1
	  initValue(1,initTokenCount) = batch.userId
	  initValueLen(1,initTokenCount) = gut_trimblks( batch.userId )

	  ! Add AUTOTYPE to initialization tokens
	  initTokenCount = initTokenCount + 1
	  initToken(initTokenCount) = 'AUTOTYPE'
	  initValueCount(initTokenCount) = 1
	  initValue(1,initTokenCount) = aeqSsp_autoTbl_autoType 
	  initValueLen(1,initTokenCount) = gut_trimblks( aeqSsp_autoTbl_autoType )

	  if ( batch.isAddtoBatch ) then
	    ! Add TEMPLATE and TEMPLATELOTTYPE to initialization tokens
	    initTokenCount = initTokenCount + 1
	    initToken(initTokenCount) = 'TEMPLATE'
	    initValueCount(initTokenCount) = 1
	    initValue(1,initTokenCount) = batch.template
	    initValueLen(1,initTokenCount) = gut_trimblks( batch.template )
	    initTokenCount = initTokenCount + 1
	    initToken(initTokenCount) = 'TEMPLATELOTTYPE'
	    initValueCount(initTokenCount) = 1
	    initValue(1,initTokenCount) = batch.templateLotType
	    initValueLen(1,initTokenCount) = 
     &	 	 gut_trimblks( batch.templateLotType )
	  endif

	  ! Connect to the equipment 
	  call aeqSsp_autoMan_automan (	initToken,
     &					initTokenCount,
     &					initValue,
     &					initValueLen,
     &					initValueCount)

	  ! Exit through the handler system if automation failed
	  if ( .not. aeqSsp_autoTbl_functionStatus ) call tut_exit ()

	endif

	return

	end

!++ AEQSSP_AUTOMAN_CONTINUE 
!
	subroutine aeqssp_automan_continue
!
	implicit none
!
!   Include files:

	include 'FIL:TYPDEF'
	include 'FIL:FILDEF'
	include 'LOT:LOTDEF'
	include 'PAR:LOTSTDEF'
	include 'GUT:GUTDEF'
	include 'LOT:LOTCOM'
	include 'PAR:EVDEF'
	include 'LOT:ACTLBUFF'	! ACTL file
	include 'LOT:RECPBUFF'	! RECP file
	include	'FIL:LBOMPDEF'	! LBOM structure and params
	include 'LOT:PRCDBUFF'  ! PRCD structure        

!   Routines used:

	integer*4	fil_open
	integer*4	fil_free
	integer*4	gut_trimblks			
	character*(TYP__LOTIDSZ)	Gut_TrimLotId 

! Local variables:

	integer*4	status,time,count
	character*(TYP__LOCATIONIDSZ)	location
	record /LBOM_record/	lbom
	character*80	comments(1)

! Executable code:

	! Open files etc.
	call Gut_Check (Fil_Open (FIL__ACTL, FIL__SHARE))
	call Gut_Check (Fil_Open (FIL__RECP, FIL__READONLY))
	call Gut_Check (Fil_Open (FIL__LBOM, FIL__READONLY))
	call Gut_Check( Fil_Open (FIL__PRCD, FIL__READONLY))        

	! Get lot record, prompting user for lot id.
	call Lot_GetLotRec (	
     &		'Enter lot id',
     & 		FIL__LOCK,
     & 		actl.buffer,
     &	  	PAR_LOTSTDEF_CS_W_RMOV//PAR_LOTSTDEF_CS_W_RQUE//
     &	  	PAR_LOTSTDEF_CS_W_RTKI//PAR_LOTSTDEF_CS_W_RSTR//
     & 	  	PAR_LOTSTDEF_CS_W_REND//PAR_LOTSTDEF_CS_W_RTKO//
     &	  	PAR_LOTSTDEF_CS_W_RQUE_H_USE//PAR_LOTSTDEF_CS_W_RQUE_H_MER//
     &	  	PAR_LOTSTDEF_CS_W_RQUE_H_CTL//PAR_LOTSTDEF_CS_W_RTKO_H_USE//
     &	  	PAR_LOTSTDEF_CS_W_RTKO_H_CTL//
     &	  	PAR_LOTSTDEF_CS_W_RSTR_H_USE//PAR_LOTSTDEF_CS_W_RSTR_H_CTL,
     &	  	LOT__AREA_CHECK)

	! If access is allowed the following called routine
	! returns. if access is denied, the routine returns
	! to the menu level
	call Lot_Access_ExitIfNoAccessPriv ( actl.buffer )

	! Who are we dealing with here?
  	call Uaf_User_CheckName ( lot_lotCom_userId )
	                                             
	! If the recipe is blank and the state is trackout hold, we are 
	! in an odd hold condition: where the lot is put on trackout 
	! hold when returning from a rework when the step which called
	! the rework had a trackout hold specified for it.
	if (	actl.recpid .eq. ' ' .and. 
     &	    	actl.state .eq. PAR_LOTSTDEF_CS_W_RTKO_H_USE ) then

	  call tut_output (
     &		'Warning: lot is TRACKOUT HELD on return from rework. '//
     &          'You cannot continue automation from this state.' )
	  goto 999

	endif

	! Print a warning message if lot is not in RUNNING state.
	if ( actl.state .eq. PAR_LOTSTDEF_CS_W_RQUE ) then

          call tut_output (	'Warning: lot is WAITING for trackin, NOT '//
     &        			'being processed.' )
	  goto 999

      	elseif (actl.state .eq. PAR_LOTSTDEF_CS_W_RQUE_H_USE .or.
     &	  	actl.state .eq. PAR_LOTSTDEF_CS_W_RQUE_H_MER .or.
     &	  	actl.state .eq. PAR_LOTSTDEF_CS_W_RQUE_H_CTL .or.
     &	  	actl.state .eq. PAR_LOTSTDEF_CS_W_RTKO_H_USE .or.
     &	  	actl.state .eq. PAR_LOTSTDEF_CS_W_RTKO_H_CTL .or.
     &	  	actl.state .eq. PAR_LOTSTDEF_CS_W_RSTR_H_USE .or.
     &	  	actl.state .eq. PAR_LOTSTDEF_CS_W_RSTR_H_CTL ) then

	  call tut_output (	'Warning: lot is HELD. Consult "Display Lot" '//
     &         			'function for the reason.' )
	  goto 999

	endif

	call Uaf_WrkCtr_CurWctId (location)

	! check that the lot is in the proper location
	if (location .eq. ' ') then

          call tut_output ( 	'Warning: this terminal is not currently '//
     &            	        'attached to a location' ) 
	  goto 999

	elseif ( actl.location .ne. ' ' .and. 
     &	         actl.location .ne. location ) then

	  call tut_fao ( 'Lot !AS is in location !AS',
     & 			  gut_trimLotId ( actl.lotid , GUT__PAD_NULLS), 
     &			  actl.location )
	  goto 999

	endif

	! Read in the recipe record for this lot
	call Lot_RdRecpRec (actl, actl.specRecpid, recp)		

	! Start up Defio
	call Lot_Defio_Start ()

	! Get event timestamp
	call Gut_Currentim(time)

	! Init Event Reason
	lot_lotCom_EventReason = PAR_EVDEF_REAS_COMMENT	

	! Put in standard comment here
	comments(1) =	'Continue automation by '//
     &			lot_lotCom_userId(:gut_trimblks(lot_lotCom_userid))//
     &			', lot '//
     &			actl.lotid(:gut_trimblks(actl.lotid))//
     &			', recipe '//actl.specRecpId
	count = 1
	call Lot_Events_Comment ( actl, time, count, comments)       
	call tut_output ( comments(1) )

	! Init Event Reason (again, just in case)
	lot_lotCom_EventReason = PAR_EVDEF_REAS_COMMENT	

	! Get additional comments and generate events
	call Lot_AddCommnt_GetComments (	actl.buffer,
     &	                                    	'Enter additional comments',
     &					    	time )

	call aeqssp_automan_recipe ( lot_lotcom_userid, 'CONTINUE' )

	! Update the ACTL file, HIST file and the Active Lot List.
	call Lot_Defio (DEFIO_UPDATE, actl.buffer, DEFIO_REJH_NONE,,)
	call Lot_Defio_End ()

	call Gut_Check (Fil_Free (FIL__ACTL))
	call Fil_FreeAll ()

  999	continue
	return
	end

!++ AEQSSP_AUTOMAN_ISENABLED
!
	logical*4 function aeqSsp_autoMan_isEnabled(	paramSuffix,
     &							autoSuffix,
     &							eqpid,
     &							initToken,
     &							initTokenCount,
     &							initValue,
     &							initValueLen,
     &							initValueCount )

	! Test to see if automation is enabled.
	!
	! Test 1.  OPTION_AUTO must be defined to YES, DEBUG, HALT, or WARN
	!
	!	   Example:	define/group OPTION_AUTO YES
	!
	! Test 2.  The $AUTO_DCOP or $AUTO_RECIPE must be defined to one of:
	!
	!	   	a) a single PROMIS equipment id
	!	   	b) a list of PROMIS equipment ids, of which the
	!		   PROMIS user must choose one.
	!		c) a parameter name, which is translated
	!		d) the keyword EQPID, so that the TRES record
	!		   equipment id is used
	!
	!	   The resulting value of the parameter becomes the 
	!	   "targetInstance".
	!
	!	   Examples:
	!
	!		$AUTO_RECIPE = "FT650"
	!		$AUTO_RECIPE = "IVS1 IVS2"
	!		$AUTO_RECIPE = $AUTO_DIFF
	!		$AUTO_RECIPE = EQPID
	!
	! Test 3.  AUTO_<autoSuffix>_<targetInstance> must be defined to the 
	!	   target where the automation program resides:
	!
	!	   Example: define/group AUTO_DATA_FT650 "FT650.FT650@DEC81TT"
	!

	implicit none

! Include files:

	include	'FIL:TYPDEF'
	include	'GUT:GUTDEF'
	include	'AEQ_SSP:AUTODEF'
	include	'AEQ_SSP:AUTOTBL'

! Function value:
!
!	- if .true. then automation is enabled
!	- if .false. then automation is disabled

! Arguments:

	character*(*)	paramSuffix				! [R]
	! - DCOP or RECIPE

	character*(*)	autoSuffix				! [R]
	! - IN, OUT, DATA, ABORT

	character*(*)	eqpid					! [R]
	! - PROMIS equipment ID

	character*(*)		initToken( MAX_TOKENS)		! [W]
	! - tokens in CONNECT message

	integer*4		initTokenCount			! [W]
	! - # of tokens

	character*(*)		initValue( MAX_VALUES, MAX_TOKENS)	! [W]
	! - values of tokens

	integer*4		initValueLen( MAX_VALUES, MAX_TOKENS)	! [W]
	! - length of values of tokens

	integer*4		initValueCount( MAX_TOKENS )	! [W]
	! - # of values per tokens


! Routines used:

	integer*4	aeqSsp_autoUtil_left_justify
	integer*4	aeqSsp_autoUtil_strip
	integer*4	aeqSsp_autoUtil_node
	logical*4	aeqSsp_autoUtil_translate 
	logical*4	aeqSsp_autoMan_getParm
	integer*4	gut_scantoken
	integer*4	gut_trimblks
	character*8 	cfc_GetPid
	integer*4	gut_getKeyP

! Author:	Mike Bergsma		7/92
!		Ab Initio Software
!
! Modifications:
!
!	Mike Bergsma		4/18/94
!	- translate auto_<eqpid> using aeqssp_autoUtil_translate
!
!	Mike Bergsma		5/13/94
!	- automation choices must translate before they can be accepted.
!
!	Mike Bergsma		7/12/94
!	- Obscure bug - don't ask!
!
!	9/20/94	Mike Bergsma
!	- remove recursive switch to 'aeqssp_automan_getParm' because
!	  it has been rewritten to know whether to do recursion.
!
!	Mike Bergsma		1/2/95
!	- add log file support 
!
! 	Mike Brergsma
!	- support for autoSuffix
!
! Local Variables:

	integer*4	status,
     &			match,
     &			startscan,endscan,starttoken,endtoken,
     &			bufferLen,
     &			parmValLen,
     &			tokenLen,
     &			targetLen,
     &			targetIndex,
     &			targetCount

	logical*4	more

	character*60	buffer
	character*(TYP__PARMVALSZ)	parmVal,
     &					token
	character*(TARGET_SIZE)		targets(MAX_TARGETS)
	integer*4			targetsLen(MAX_TARGETS)
	character*(TARGET_SIZE)		target
	character*23			datetime
	character*8			pid

	character*(INSTANCE_SIZE)	targetInstance
	integer*4			targetInstanceLen
	character*(OBJECT_SIZE)		targetObject
	integer*4			targetObjectLen
	character*(NODE_SIZE)		targetNode
	integer*4			targetNodeLen

! Code

	! Initialize.  Assume automation is not enabled.
	! (This will be the case since most dcOp's and recipes are not
	!  enabled for automation).
	aeqSsp_automan_isEnabled = .false.
	initTokenCount = 0

	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	! TEST FOR AUTOMATION
	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	! Translate master switch OPTION_AUTO 

	call Lib$Sys_TrnLog( 'OPTION_AUTO', bufferLen, buffer,, )

	if ( buffer(:bufferLen) .eq. 'DEBUG' ) then
	  ! Same as YES but set debug flag
	  aeqSsp_autoTbl_debug = .true.
	  aeqSsp_autoTbl_halt = .false.
	  buffer = 'YES'
	  bufferLen = 3
	elseif ( buffer(:bufferLen) .eq. 'HALT' ) then
	  ! Same as YES but set halt flag and clear debug flag
	  aeqSsp_autoTbl_debug = .false.
	  aeqSsp_autoTbl_halt = .true.
	  buffer = 'YES'
	  bufferLen = 3
	elseif ( buffer(:bufferLen) .eq. 'WARN' ) then
	  ! Same as YES but set halt flag and debug flag
	  aeqSsp_autoTbl_debug = .true.
	  aeqSsp_autoTbl_halt = .true.
	  buffer = 'YES'
	  bufferLen = 3
	else
	  ! Make sure debug is off
	  aeqSsp_autoTbl_debug = .false.
	  aeqSsp_autoTbl_halt = .false.
	endif

	! Return if not enabled
	if ( buffer(:bufferLen) .ne. 'YES' ) return

	! Translate switch OPTION_AUTO_LOG
	call Lib$Sys_TrnLog( 'OPTION_AUTO_LOG', bufferLen, buffer,, )
	if ( buffer(:bufferLen) .eq. 'YES' ) then
	  aeqSsp_autoTbl_log = .true.
	else
	  aeqSsp_autoTbl_log = .false.
	endif

        call Lib$Sys_TrnLog( 'OPTION_AUTO_OPCERT', bufferLen, buffer,, )
        if ( buffer(:bufferLen) .eq. 'YES' ) then
          aeqSsp_autoTbl_opcert = .true.
        else
          aeqSsp_autoTbl_opcert = .false.
        endif

	call Lib$Sys_TrnLog( 'AUTOROUTER', bufferLen, buffer,, )
	if ( buffer(:bufferLen) .eq. 'AUTOROUTER' ) then
	  aeqSsp_autoTbl_router = 'ROUTER'
	  aeqSsp_autoTbl_routerLen = 6
	else
	  aeqSsp_autoTbl_router = buffer(:bufferLen)
	  aeqSsp_autoTbl_routerLen = bufferLen
	endif

	call Lib$Sys_TrnLog( 'AUTOFIFO', bufferLen, buffer,, )
	if ( buffer(:bufferLen) .eq. 'AUTOFIFO' ) then
	  aeqSsp_autoTbl_mbxRouter = 'MBX_'//aeqSsp_autoTbl_router(:aeqSsp_autoTbl_routerLen)
	  aeqSsp_autoTbl_mbxRouterLen = 4 + aeqSsp_autoTbl_routerLen 
	else
	  aeqSsp_autoTbl_mbxRouter = buffer(:bufferLen)//'_'//
     &                               aeqSsp_autoTbl_router(:aeqSsp_autoTbl_routerLen)
	  aeqSsp_autoTbl_mbxRouterLen = bufferLen + 1 + aeqSsp_autoTbl_routerLen 
	endif


	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	! INITIALIZE
	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	aeqSsp_autoTbl_target = ' '
	aeqSsp_autoTbl_targetLen = 0
	aeqSsp_autoTbl_targetInstance = ' '
	aeqSsp_autoTbl_targetInstLen = 0
	aeqSsp_autoTbl_targetObject = ' '
	aeqSsp_autoTbl_targetObjectLen = 0
	aeqSsp_autoTbl_targetNode = ' '
	aeqSsp_autoTbl_targetNodeLen = 0
	aeqSsp_autoTbl_sender = ' '
	aeqSsp_autoTbl_senderLen = 0
	aeqSsp_autoTbl_senderInstance = ' '
	aeqSsp_autoTbl_senderInstLen = 0
	aeqSsp_autoTbl_senderObject = ' '
	aeqSsp_autoTbl_senderObjectLen = 0
	aeqSsp_autoTbl_senderNode = ' '
	aeqSsp_autoTbl_senderNodeLen = 0


	! Initialize parameter lookup tables (full Initialization)
	!!!call Tst_ParmUtil_TstPrmComInit( .true. )

	! Check for $AUTO_* parameter. Return if not enabled
	if ( .not. aeqSsp_autoMan_getParm(	'$AUTO_'//paramSuffix,
     &			        		aeqSsp_autoTbl_parmSource,
     &						parmVal ) ) then

	  ! Before we say there's no automation, was the OPCERT parameter
	  ! set?  If so, set the target and exit.

          if ( aeqSsp_autoTbl_opcert .eq. .false. ) return

	  parmVal = 'OPCERT'

        endif

	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	! GET AUTOMATION TARGET 
	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	! Break parameter up into tokens and extract the equipment id
	parmValLen = gut_trimblks( parmVal )
	startScan = 1
	endScan = parmValLen		
	targetCount = 0
	more = .true.
	do while( more )

	  ! Extract token
	  status = gut_scantoken(	parmVal,
     &	   				starttoken,
     &					endtoken,
     &       				startscan,
     &	   				endScan ) 

	  if ((status .and. (GUT_TOKEN_NONE+GUT_TOKEN_NULL)) .eq. 0 ) then

	    ! If token is a parameter, then translate it now
     	    call aeqSsp_autoMan_getParm(	parmVal(startToken:endToken),
     &			        		aeqSsp_autoTbl_parmSource,
     &						token )
	    ! Check for keywords
	    if ( token .eq. 'EQPID' ) token = eqpid

	    ! If it's still valid, add it to the list
	    tokenLen = aeqSsp_autoUtil_strip( token )
	    if (	tokenLen .gt. 0 .and. 
     &			targetCount .lt. MAX_TARGETS ) then

	      ! Make sure it can translated
	      target = autoSuffix//'_'//token
	      targetLen = gut_trimblks ( target )
	      if ( aeqSsp_autoUtil_translate (	target, targetLen,
     &		  				targetInstance, 
     &		  				targetInstanceLen,
     &		  				targetObject, 
     &		  				targetObjectLen,
     &		  				targetNode, 
     &		  				targetNodeLen ) ) then
	
	        targetCount = targetCount + 1
	        targets(targetCount) = token
	        targetsLen(targetCount) = tokenLen

	      endif

	    endif 
	  endif 

	  if ( (status .and. GUT_LINE_END) .ne. 0 ) more = .false.

	enddo

	! If there are no automation targets, return "not enabled"
	if ( targetCount .eq. 0 ) return

	if ( targetCount .eq. 1 ) then

	  ! One choice
	  targetIndex = 1

	else

	  ! Multiple choices, targetCount > 1 

	  call tut_output( 'Automation choices are:' )
	  call gut_PrintCols( targetCount, targets )
	  match = gut_getKeyP(	'Choose automation equipment '//
     &	  			'or press RETURN for '//
     &	  			targets(1),
     &	  			targetIndex,
     &	  			targets,
     &	  			targetCount,
     &	  			GUT__STRICT+
     &	  			GUT__EMPTY+
     &	  			GUT__NONE )

	  if ( match .eq. GUT__OK_NONE ) return

	  if ( match .eq. GUT__OK_EMPTY ) then
	    targetIndex = 1
	    match = GUT__NORMAL 
	  endif

	  if ( match .ne. GUT__NORMAL ) return

	endif

	target = autoSuffix//'_'//targets(targetIndex)
	targetLen = gut_trimblks ( target )
        call aeqSsp_autoUtil_translate (target, targetLen,
     &		  			aeqSsp_autoTbl_targetInstance, 
     &		  			aeqSsp_autoTbl_targetInstLen,
     &		  			aeqSsp_autoTbl_targetObject, 
     &		  			aeqSsp_autoTbl_targetObjectLen,
     &		  			aeqSsp_autoTbl_targetNode, 
     &		  			aeqSsp_autoTbl_targetNodeLen ) 
	
	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	! GET AUTOMATION SENDER
	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	pid = Cfc_GetPid()			! get process pid
	call lib$date_time( datetime ) 	! get date and time
	aeqSsp_autoTbl_senderInstance = 
     &		pid(5:8)//datetime(19:20)//datetime(22:23)
	aeqSsp_autoTbl_senderInstLen = 
     &		aeqSsp_autoUtil_left_justify( aeqSsp_autoTbl_senderInstance )
	aeqSsp_autoTbl_senderObject = 'AM'
	aeqSsp_autoTbl_senderObjectLen = 2
	aeqSsp_autoTbl_senderNodeLen = 
     &		aeqSsp_autoUtil_node( aeqSsp_autoTbl_senderNode )


	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	! Look for $AUTO_INITDCOP* or $AUTO_INITRECP*
	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	call aeqSsp_autoMan_multiParms (paramSuffix,
     &			        	aeqSsp_autoTbl_parmSource,
     &					initToken,
     &					initTokenCount,
     &					initValue,
     &					initValueLen,
     &					initValueCount )
					
	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	! Log the session if specified.
	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	if ( aeqSsp_autotbl_log ) 
     &	  call aeqSsp_autoUtil_openLog (	
     &		'AUTOLOG:AUTO_'//autoSuffix//'_'//
     &		aeqSsp_autoTbl_targetObject(:aeqSsp_autoTbl_targetObjectLen)
     &		//'.TMP' )

	aeqSsp_autoMan_isEnabled = .true.

	return

	end

!++ AEQSSP_AUTOMAN_MULTIPARMS 

	subroutine aeqSsp_autoMan_multiParms (	auto,
     &			        		parmSource,
     &						parmName,
     &						parmNameCount,
     &						parmValue,
     &						parmValueLen,
     &						parmValueCount )
					
	implicit none

! Include files:

	include	'FIL:TYPDEF'
	include	'GUT:GUTDEF'
	include	'AEQ_SSP:AUTODEF'

! Arguments:

	character*(*)		auto					![R]
	! - RECIPE or DCOP

	integer*4		parmSource				! [R]
	! - parameter sources

	character*(*)		parmName( MAX_TOKENS)			! [W]
	! - parameters names extracted 

	integer*4		parmNameCount				! [W]
	! - # of parameters

	character*(*)		parmValue( MAX_VALUES, MAX_TOKENS)	! [W]
	! - values of parameters

	integer*4		parmValueLen( MAX_VALUES, MAX_TOKENS)	! [W]
	! - length of values of parameters

	integer*4		parmValueCount( MAX_TOKENS )		! [W]
	! - # of values per parameters
	! - from this routine, always one.


! Routines used:

	logical*4	aeqSsp_autoMan_getParm
	integer*4	gut_scantoken
	integer*4	gut_trimblks

! Author:	Mike Bergsma		7/92
!		Ab Initio Software
!
! Modifications:
!
!	Mike Bergsma		9/7/94
!	- add support for soft and hard batch.
!
!	9/20/94	Mike Bergsma
!	- remove recursive switch to 'aeqssp_automan_getParm' because
!	  it has been rewritten to know whether to do recursion.

! Local Variables:

	integer*4	status,
     &			i,k,
     &			startscan,
     &			endscan,
     &			starttoken,
     &			endtoken

	logical*4	more

	character*(TYP__PARMVALSZ)	parmVal,
     &					token

	parameter	NUM_SUFFIXES = 11
	character*(9)	suffix( NUM_SUFFIXES, 4 )
	data suffix/	'INITPARMS',
     &			'INITDCOP0',
     &			'INITDCOP1',
     &			'INITDCOP2',
     &			'INITDCOP3',
     &			'INITDCOP4',
     &			'INITDCOP5',
     &			'INITDCOP6',
     &			'INITDCOP7',
     &			'INITDCOP8',
     &			'INITDCOP9',
     &			'INITPARMS',
     &			'INITRECP0',
     &			'INITRECP1',
     &			'INITRECP2',
     &			'INITRECP3',
     &			'INITRECP4',
     &			'INITRECP5',
     &			'INITRECP6',
     &			'INITRECP7',
     &			'INITRECP8',
     &			'INITRECP9',
     &			'SOFTPARMS',
     &			'SOFTPARM0',
     &			'SOFTPARM1',
     &			'SOFTPARM2',
     &			'SOFTPARM3',
     &			'SOFTPARM4',
     &			'SOFTPARM5',
     &			'SOFTPARM6',
     &			'SOFTPARM7',
     &			'SOFTPARM8',
     &			'SOFTPARM9',
     &			'HARDPARMS',
     &			'HARDPARM0',
     &			'HARDPARM1',
     &			'HARDPARM2',
     &			'HARDPARM3',
     &			'HARDPARM4',
     &			'HARDPARM5',
     &			'HARDPARM6',
     &			'HARDPARM7',
     &			'HARDPARM8',
     &			'HARDPARM9' /

! Code:

	if ( auto .eq. 'DCOP' ) then
	  k = 1
	else if ( auto .eq. 'RECIPE' ) then
	  k = 2
	else if ( auto .eq. 'SOFT' ) then
	  k = 3
	else if ( auto .eq. 'HARD' ) then
	  k = 4
	else
	  return
	endif

	do i=1,NUM_SUFFIXES

	  if ( aeqSsp_autoMan_getParm(	'$AUTO_'//suffix(i,k),
     &			        	parmSource,
     &					parmVal ) ) then

	    startScan = 1
	    endScan = gut_trimblks( parmVal )
	    more = .true.
	    do while( more .and. parmNameCount .lt. MAX_TOKENS )

	      ! Extract token
	      status = gut_scantoken(	parmVal,
     &	   				starttoken,
     &					endtoken,
     &       				startscan,
     &	   				endScan ) 

	      if ((status .and. (GUT_TOKEN_NONE+GUT_TOKEN_NULL)) .eq. 0 ) then

	        ! Extract valid parameter/value pairs
     	        if ( aeqSsp_autoMan_getParm(	parmVal(startToken:endToken),
     &			        		parmSource,
     &						token ) ) then

		  parmNameCount = parmNameCount + 1 
		  parmName(parmNameCount) = parmVal(startToken:endToken)
		  parmValueCount(parmNameCount) = 1
		  parmValue(1,parmNameCount) = token
		  parmValueLen(1,parmNameCount) = gut_trimblks( token )

	        endif

	      endif

	      if ( (status .and. GUT_LINE_END) .ne. 0 ) more = .false.

	    enddo

	  endif

	enddo

	return

	end

!++ AEQSSP_AUTOMAN_AUTOMAN
!
	subroutine aeqSsp_autoMan_automan (	initToken,
     &						initTokenCount,
     &						initValue,
     &						initValueLen,
     &						initValueCount )

	implicit none

	! The routine displays the automation user interface and then
	! attempts to connect to the target instance.  
	!
	! Before the connect message is sent to the target instance, a connect
	! timer is set to generate a timeout condition should the connection 
	! fail to succeed within a specified time.  If the connection occurs 
	! before the timeout condition, then the timer is cancelled.
	!
	! Once connected, an AST routine is declared to service incoming
	! messages from the target instance.  Since the invocation of the AST 
	! service routine is asynchronous, this routine sets up a program loop
	! with a prompt so that the PROMIS user can enter a command at any
	! time to send a message to the target instance or to abort the
	! automation interface.
	!
	! The prompt is also subject to "fallout", in which an incoming 
	! DISCONNECT message will force the prompt to complete without
	! input from the PROMIS user.

! Include files:

	include	'($IODEF)'	
	include	'($PSLDEF)'	
	include	'TUT:TUTDEF'
	include	'AEQ_SSP:AUTODEF'
	include	'AEQ_SSP:AUTOTBL'

! Arguments:

	character*(*)		initToken( MAX_TOKENS)		! [R]
	! - tokens in CONNECT message

	integer*4		initTokenCount			! [R]
	! - # of tokens

	character*(*)		initValue( MAX_VALUES, MAX_TOKENS)	! [R]
	! - values of tokens

	integer*4		initValueLen( MAX_VALUES, MAX_TOKENS)	! [R]
	! - length of values of tokens

	integer*4		initValueCount( MAX_TOKENS )	! [R]
	! - # of values per tokens


! Function value:

	! Return .true. if CONNECTION succeeded

! Routines used:

	integer*4	sys$assign
	integer*4	sys$bintim
	integer*4	sys$cantim
	integer*4	sys$dassgn
	integer*4	sys$cancel
	logical*4	aeqSsp_autoMan_check
	integer*4	aeqSsp_autoMan_command
	integer*4 	aeqSsp_autoMan_handler
	external	aeqSsp_autoMan_handler
	external	aeqSsp_autoMan_handler2
	integer*4	gut_trimblks
	integer*4	gut_getstr

! Author:	Mike Bergsma		7/92
!		Ab Initio Software
!
! Modifications:
!
!	17-Nov-1993	Michael Bergsma
!	- do not send and EVENT messages to target unless the first message
!	  from target has been received.
!		
!	15-Nov-1993	Michael Bergsma
!	- send DISCONNECT message to ROUTER if there is no other object to
!	  send a DISCONNECT message to.
!
!	Mike Bergsma	5/10/94
!	- ask for reason if user presses 'Manual' key
!
!	Mike Bergsma		1/2/95
!	- add log file support 
!
!	Mike Bergsma		7/12/95
!	- reason was not always being asked when automation is aborted
!
!	Mike Bergsma		10/26/95
!	- disallow session abort from [Q] or [M] hot keys.
!	- do not need to prompt for reason any longer
!
!	Mike Bergsma		11/6/95
!	- allow Manual when no failure and no first message received
!
!	Mike Bergsma		12/8/96
!	- forgot to initialize msgPending.  This was the cause of I/O channel
!	  error.  Whenever the previous automation session terminates on a 
!	  received ABORT or DISCONNECT, there is a possiblility that an 
!	  additional message will be received prior to the automation session
!	  disabling the AST.  The AST sets the msgPending flag, and when
!	  the next Automation session was started, it erroneously attempted to
!	  read a non-existent message from a deleted mailbox.
!	  (For example, when an operator presses the ABORT key more than twice,
!	   then two ABORT messages are received, one from the target and one
!	   "Failed to route message" from the AutoRouter.)
!
!	Mike Bergsma		1/9/96
!	- channels can be '0'.  Therefore, they cannot be initialized to 0
!	  because the program was treating a 0 as if the channel did not exist
!
! Local Variables:

	parameter	MANUAL = ICHAR ( 'M' )

	integer*4	status,
     &			textLen,
     &			key,
     &			methodLen

	character*(VALUE_SIZE)		text
	character*(METHOD_SIZE)		method

! Code:

	! Initialize.
	call aeqSsp_autoUtil_putAddress (	aeqSsp_autoTbl_target, 
     &						aeqSsp_autoTbl_targetLen,
     &						aeqSsp_autoTbl_targetInstance,
     &						aeqSsp_autoTbl_targetInstLen,
     &						aeqSsp_autoTbl_targetObject,
     &						aeqSsp_autoTbl_targetObjectLen,
     &						aeqSsp_autoTbl_targetNode,
     &						aeqSsp_autoTbl_targetNodeLen )

	call aeqSsp_autoUtil_putAddress (	aeqSsp_autoTbl_sender, 
     &						aeqSsp_autoTbl_senderLen,
     &						aeqSsp_autoTbl_senderInstance,
     &						aeqSsp_autoTbl_senderInstLen,
     &						aeqSsp_autoTbl_senderObject,
     &						aeqSsp_autoTbl_senderObjectLen,
     &						aeqSsp_autoTbl_senderNode,
     &						aeqSsp_autoTbl_senderNodeLen )

	aeqSsp_autoTbl_inChannel = 0
	aeqSsp_autoTbl_outChannel = 0
	aeqSsp_autoTbl_connectTries = CONNECT_TRIES
	aeqSsp_autoTbl_connectStatus = .false.
	aeqSsp_autoTbl_msgPending = .false.

	! PROMIS exits after automation if _functionStatus is set to .false.
	aeqSsp_autoTbl_functionStatus = .false.

	! Prompt for single key commands until sessionStatus is set to .false.
	aeqSsp_autoTbl_sessionStatus = .true.

	! We will pagewait after automation unless _pagewait is set to .false.
	aeqSsp_autoTbl_pagewait = .true.

	! Clear the optional text3 variable
	aeqSsp_autoTbl_text3 = ' '

	! Setup the Automation User Interface
	call aeqSsp_autoMan_setupScreen()
	call aeqSsp_autoMan_initHotKeys()

	! Temporarily turn off PROMIS control/c handler
	call Tut_Init_SetCtrlAst( .false. )
	
	! Turn on our own handlers
	call Lib$Establish ( aeqSsp_autoMan_handler )
	call aeqSsp_automan_setCtrlAst ( aeqSsp_autoMan_handler2 )

	! Do not allow automation if OPTION_AUTO was HALT
	if ( aeqSsp_autoTbl_halt ) then
	  ! OPTION_AUTO is either HALT or WARN
	  ! The debug flag indicates which way.
	  aeqSsp_autoTbl_sessionStatus = aeqSsp_autoTbl_debug 
	  if ( aeqSsp_autoTbl_sessionStatus ) then
	    call aeqSsp_automan_outMessage( 
     &		'% Automation has been temporarily disabled.\n'//
     &		'The PROMIS transaction can only be performed under '//
     &		'manual operation until automation has been re-enabled', 
     &		.true. )
	  else
	    call aeqSsp_automan_outMessage( 
     &		'% Automation has been temporarily halted.\n'//
     &		'The PROMIS transaction will not be allowed under '//
     &		'any circumstances.   Please wait until automation '//
     &		'has been re-enabled', 
     &		.true. )
	  endif
	  goto 999
	endif

	! Connect to write-only mailbox owned and operated by the autoRouter
	status = sys$assign(
     &		aeqSsp_autoTbl_mbxRouter(:aeqSsp_autoTbl_mbxRouterLen),
     &		aeqSsp_autoTbl_outChannel,
     &		,, )
	if ( .not. aeqSsp_autoMan_check( status, 1 ) ) then

	  call aeqSsp_automan_outMessage( 
     &		'% The root autorouter process is not running because '//
     &		'the '//aeqSsp_autoTbl_mbxRouter(:aeqSsp_autoTbl_mbxRouterLen)//
     &		' mailbox does not exist. '//
     &		'Please contact your facilitator and request '//
     &		'that the autorouter be re-started on the VAX', 
     &		.true. )
	  goto 999

	endif

	! Send a connect message via the autoRouter to the target.
	! It will be an EVENT message so that this PROMIS instance can now be
	! a server to the client.
	call aeqSsp_autoMan_sendConnect(	initToken,
     &						initTokenCount,
     &						initValue,
     &						initValueLen,
     &						initValueCount )


	! Set a timer interval that will be used to interrupt the prompt every 
	! 5 seconds and check the status of the automation process.
	status = sys$bintim(	CONNECT_TIMER, 
     &				aeqSsp_autoTbl_AST_timer )
	if ( .not. aeqSsp_autoMan_check( status, 1 ) ) then
	  call aeqSsp_automan_outMessage( 
     &			'% Cannot set connection timer interval. '//
     &			'This is an internal program error. '//
     &			'Please try logging completely out and then '//
     &			'logging back in; or contact your facilitator.',
     &			.true. )
	  goto 998
	endif
	call aeqSsp_autoMan_AST_timer()

	! Do the Automation User Interface

	! Prompt for commands that can be sent as EVENT messages to the
	! target instance.
	do while ( aeqSsp_autoTbl_sessionStatus )

	  ! (Re-)enable AST's 
	  call sys$setast( %val(1) )

	  ! Get command token.
	  if ( .not. aeqSsp_autoTbl_msgPending ) then

	    ! Wait for "HOT" key input from user

	    ! ********************************************************
	    ! There's a potential race condition here.  
	    ! If the AST for incoming messages is triggered before 
	    ! the sys$qiow function in "aeqSsp_automan_command", then
	    ! the sys$qiow function will not be cancelled and will
	    ! remain blocked until the user presses a key.
	    ! This race condition cannot be prevented, but will likely 
	    ! not occur because messages generally do not arrive fast
	    ! enough to trigger the AST before the sys$qiow call.
	    ! ********************************************************

	    ! Re-establish the CTRL/C CTRL/Y handler 
	    call aeqSsp_automan_setCtrlAst ( aeqSsp_autoMan_handler2 )

	    key = aeqSsp_automan_command()

	    if ( key .eq. -1 ) then

     	      call aeqSsp_autoMan_logDebug ( '...waiting for event' )
	      call sys$clref  ( %val ( EFN_WAITFR ) )
	      call sys$waitfr ( %val ( EFN_WAITFR ) )
     	      call aeqSsp_autoMan_logDebug ( 'continuing after event' )
	      key = 0

	    endif

	  else

	    ! Message pending.  
	    key = 0

	  endif

	  ! Disable AST's while we process the command.
	  call sys$setast( %val(0) )

	  if ( key .eq. 0 ) then

	    ! The aeqSsp_automan_command function had its IO cancelled
	    ! probably because an AST was received when a message arrived
	    ! in the mailbox.
            if ( aeqSsp_autoTbl_msgPending ) then
              call aeqSsp_autoMan_MBXread()
            else
              call aeqSsp_autoMan_terminate ( .false. ,.false. )
	    endif

	  elseif ( aeqSsp_autoTbl_hotkey(key).isEnabled ) then

	    ! Process special commands first.
	    method = aeqSsp_autoTbl_hotkey(key).method 
	    methodLen = gut_trimblks ( method )

	    if ( aeqSsp_autoTbl_connectStatus ) then

	      if ( method .eq. 'CONNECT' ) then

	        ! Re-connect to automation.
	        ! Send CONNECT method
     	        call aeqSsp_autoMan_outMessage ( 
     &			'...retrying connection to '//
     &			aeqSsp_autoTbl_target(:aeqSsp_autoTbl_targetLen),
     &			.true. )
	        call aeqSsp_autoMan_sendConnect(	initToken,
     &							initTokenCount,
     &							initValue,
     &							initValueLen,
     &							initValueCount )
	      elseif ( method .eq. 'MESSAGE' ) then

	  	! (Re-)enable AST's, in case we get a message while
		! in the prompt.
	  	call sys$setast( %val(1) )

	        textLen = gut_getstr ( 'Enter text to send', text, 0 )

	  	! Disable AST's.
	  	call sys$setast( %val(0) )

	        call tut_output ( ' ' )

	        if ( .not. aeqSsp_autoTbl_msgPending ) then
	          if ( textLen .eq. 0 ) then
	            text = ' '
	            textlen = 1
	          endif		 
	          call aeqSsp_autoMan_outMessage( ' ', .false. )
	          call aeqSsp_autoMan_sendMessage( text(:textlen) )
		endif

	      elseif ( method .ne. ' ' ) then

	        ! Regular HOT-KEY	
	        ! Send method
                call aeqSsp_autoMan_outMessage( 
     &	    	  	'...sending '//method(:methodLen)//' message to '//
     &			aeqSsp_autoTbl_target(:aeqSsp_autoTbl_targetLen),
     &			.true. )
	        call aeqSsp_autoMan_sendEvent( method(:methodLen) )

	      else 

	        ! Key has undefined (blank) method.
     	        call tut_beep()

	      endif

	    else 

	      ! Cannot send to unconnected target.
     	      call tut_beep()

	    endif

	  else

	    ! Key is disabled.
     	    call tut_beep()

	  endif	

	enddo

	! Automation is DONE

	! Send an DISCONNECT to autorouter 
	aeqSsp_autoTbl_target = aeqSsp_autoTbl_router
	aeqSsp_autoTbl_targetLen = aeqSsp_autoTbl_routerLen
	call aeqSsp_autoMan_sendEvent( 'DISCONNECT' )
	
997	continue

	! Re-enable AST's
	call sys$setast( %val(1) )

	! Cancel all timer operations
	status = sys$cantim( %val(0), %val(PSL$C_USER) )
	if ( .not. aeqSsp_autoMan_check( status, 1 ) )
     &  call aeqSsp_automan_logDebug ( '% Failed to cancel timer.' ) 

	! Close incoming mailbox channel
	if ( aeqSsp_autoTbl_inChannel .ne. 0 ) then

	  ! Cancel all AST's
	  status = sys$cancel( %val( aeqSsp_autoTbl_inChannel ) )
	  if ( .not. aeqSsp_autoMan_check( status, 1 ) )
     &    call aeqSsp_automan_logDebug ( '% Failed to cancel AST on incoming mbx channel' ) 

	  ! Deassign the channel
     	  status = sys$dassgn( %val( aeqSsp_autoTbl_inChannel ) )
	  if ( .not. aeqSsp_autoMan_check( status, 1 ) )
     &    call aeqSsp_automan_logDebug ( '% Failed to deassign incoming mbx channel' ) 

	endif

998     continue                          


	! Close outgoing mailbox channel
	! Deassign the channel
	status = sys$dassgn( %val( aeqSsp_autoTbl_outChannel ) )
	if ( .not. aeqSsp_autoMan_check( status, 1 ) )
     &  call aeqSsp_automan_logDebug ( '% Failed to deassign outgoing mbx channel' ) 

999	continue

	! Perform the pagewait function, if requested.
	if ( aeqSsp_autoTbl_pagewait ) then

	  ! Display the pagewait window.
	  call aeqSsp_automan_pagewait ( aeqSsp_autoTbl_functionStatus,
     &					 aeqSsp_autoTbl_connectStatus,
     &					 aeqSsp_autoTbl_sessionStatus,
     &					 aeqSsp_autoTbl_text3 )

	  ! Beep at user
	  call tut_beep ()

	  ! Prompt for a return/spacebar (or Manual if enabled)
	  key = 0
	  do while ( key .eq. 0 )
	    key = aeqSsp_automan_command()
	    if ( key .eq. MANUAL .and. 
     &		.not. aeqSsp_autoTbl_connectStatus .and.
     &		aeqSsp_autoTbl_sessionStatus ) then
	      ! Manual hotkey was pressed.  Allow PROMIS function to succeed
     	      aeqSsp_autoTbl_functionStatus = .true.
	    elseif ( key .eq. 13 .or. key .eq. 32 ) then
	      ! Return or spacebar was pressed.
	    elseif ( key .eq. -1 ) then
	      !	Detached process.
	    else	
	      call tut_beep()
	      key = 0	
	    endif
	  enddo

	endif

	! Remove the automation screen and restore the PROMIS screen.
	call aeqSsp_autoMan_resetScreen ()

	! Close the log file 
	if ( aeqSsp_autotbl_log ) call aeqSsp_autoUtil_closeLog()

	! Cancel any outstanding I/O
	call tut_cancelIO()

	! Turn off our CTRL/C CTRL/Y handler
	call aeqSsp_automan_setCtrlAst ( %val(0) )

	! Turn on PROMIS control/c handler
	call Tut_Init_SetCtrlAst( .true. )
	
	return

	end

!++ AEQSSP_AUTOMAN_SETAST
!
	logical*4 function aeqSsp_autoMan_setAST( mbxChannel )

	implicit none

	! Reset the AST for another message

! Include Files:

	include	'($IODEF)'	
	include	'AEQ_SSP:AUTODEF'

! Arguments:

	integer*2	mbxChannel				! [R]
	! - set an write-AST on this mailbox channel

! Function value:
!
!	- returns success or failure

! Routines Used:

	integer*4	aeqSsp_autoMan_AST_MBX
	external	aeqSsp_autoMan_AST_MBX
	integer*4	aeqSsp_autoMan_check
	integer*4	sys$qiow

! Author:	Mike Bergsma		7/92
!		Ab Initio Software
!
! Modifications:
!
! 10/11/99 MDB - pass ast parameter (MBXchannel) by value
!
! Local Variables:

	integer*4	status
	integer*2	iosb(4)

! Code:

	! Enable write attention AST
	status = sys$qiow(	%val(EFN_ATTN),
     &				%val(MBXchannel),
     &				%val(IO$_SETMODE+IO$M_WRTATTN),
     &				iosb,
     &				,,
     &				aeqSsp_autoMan_AST_MBX,
     &				%val(MBXchannel),
     &				,,,, )

	if ( .not. aeqSsp_autoMan_check( status, iosb(1) ) ) then

          call aeqSsp_autoMan_outMessage( 
     &		'% Error setting write attention AST for incoming messages. '//
     &		'This is an internal program error. '//
     &		'Please try logging completely out and then logging '//
     &		'back in; or contact your facilitator.',
     &		.true. )

	  aeqSsp_autoMan_setAST = .false.

	else

	  aeqSsp_autoMan_setAST = .true.

	endif

	return

	end

!++ AEQSSP_AUTOMAN_SENDCONNECT

	subroutine aeqSsp_autoMan_sendConnect(	initToken,
     &						initTokenCount,
     &						initValue,
     &						initValueLen,
     &						initValueCount )

	implicit none

	! Handy routine to send a CONNECT message

! Include files:

	include	'AEQ_SSP:AUTODEF'
	include	'AEQ_SSP:AUTOTBL'

! Arguments:

	character*(*)		initToken( MAX_TOKENS)		! [R]
	! - tokens in CONNECT message

	integer*4		initTokenCount			! [R]
	! - # of tokens

	character*(*)		initValue( MAX_VALUES, MAX_TOKENS)	! [R]
	! - values of tokens

	integer*4		initValueLen( MAX_VALUES, MAX_TOKENS)	! [R]
	! - length of values of tokens

	integer*4		initValueCount( MAX_TOKENS )	! [R]
	! - # of values per tokens


! Routines:

	character*8	aeqssp_autoutil_random8
	character*(DATETIME_SIZE)	aeqssp_autoutil_timeStamp	

! Author:	Mike Bergsma		7/92
!		Ab Initio Software
!
! Modifications:
!
!	Mike Bergsma	1/10/95
!	- debug message moved to aeqSsp_autoMan_send

! Local Variables:

	! MESSAGE buffer
	character*(MESSAGE_SIZE) 	message
	integer*4			messageLen
	character*8			tid
	character*(DATETIME_SIZE)	ts

! Code:

	tid = aeqSsp_autoutil_random8 ()
	ts = aeqSsp_autoUtil_timeStamp( )

	call aeqSsp_autoMan_outMessage ( 
     &		'...sending connection request to '//
     &		aeqSsp_autoTbl_target(:aeqSsp_autoTbl_targetLen),
     &		.true. )

	call aeqSsp_autoUtil_unparse(	DEFAULT_DELIMITER,
     &			aeqSsp_autoTbl_target(:aeqSsp_autoTbl_targetLen),
     &					'EVENT',
     &					'CONNECT',
     &			aeqSsp_autoTbl_sender(:aeqSsp_autoTbl_senderLen),
     &					tid, ts,
     &					initToken,
     &	   				initTokenCount, 
     &					initValue,
     &	   				initValueLen,
     &	   				initValueCount,
     &					message, messageLen )

	call aeqSsp_autoMan_send( message(:messageLen) )

	return

	end

!++ AEQSSP_AUTOMAN_SENDMESSAGE

	subroutine aeqSsp_autoMan_sendMessage ( buffer )

	implicit none

	! Handy routine to send an MESSAGE

! Include files:

	include	'AEQ_SSP:AUTODEF'
	include	'AEQ_SSP:AUTOTBL'

! Arguments:

	character*(*)		buffer			! [R]
	! - message text to send

! Routines Used:

! Author:	Mike Bergsma		7/92
!		Ab Initio Software
!
! Modifications:

! Local Variables:

	! COMPONENTS of a MESSAGE
	character*(TOKEN_SIZE)		token( MAX_TOKENS )
	integer*4			tokenCount
	character*(VALUE_SIZE)		value( MAX_VALUES, MAX_TOKENS )
	integer*4			valueLen( MAX_VALUES, MAX_TOKENS )
	integer*4			valueCount( MAX_TOKENS )

! Code:

	tokenCount = 1
	token(1) = 'TEXT'
	valueCount(1) = 1
	value(1,1) = buffer
	valueLen(1,1) = LEN ( buffer )

	call aeqSsp_autoMan_sendMethod(	'EVENT',
     &					'MESSAGE',
     &					token,
     &					tokenCount,
     &					value,
     &					valueLen,
     &					valueCount )

	return

	end

!++ AEQSSP_AUTOMAN_SENDEVENT

	subroutine aeqSsp_autoMan_sendEvent( method )

	implicit none

	! Handy routine to send an EVENT message for specified method 

! Include files:

	include	'AEQ_SSP:AUTODEF'
	include	'AEQ_SSP:AUTOTBL'

! Arguments:

	character*(*)		method				! [R]
	! - method to send

! Routines Used:

! Author:	Mike Bergsma		7/92
!		Ab Initio Software
!
! Modifications:

! Local Variables:

	! COMPONENTS of a MESSAGE
	character*(TOKEN_SIZE)		token( MAX_TOKENS )
	integer*4			tokenCount
	character*(VALUE_SIZE)		value( MAX_VALUES, MAX_TOKENS )
	integer*4			valueLen( MAX_VALUES, MAX_TOKENS )
	integer*4			valueCount( MAX_TOKENS )

! Code:

	tokenCount = 0
	call aeqSsp_autoMan_sendMethod(	'EVENT',
     &					method,
     &					token,
     &					tokenCount,
     &					value,
     &					valueLen,
     &					valueCount )

	return

	end

!++ AEQSSP_AUTOMAN_SENDMETHOD

	subroutine aeqSsp_autoMan_sendMethod(	mode,
     &						method,
     &						token,
     &						tokenCount,
     &						value,
     &						valueLen,
     &						valueCount )

	implicit none

	! Handy routine to send a message 

! Include files:

	include	'AEQ_SSP:AUTODEF'
	include	'AEQ_SSP:AUTOTBL'

! Arguments:

	character*(*)		mode				! [R]
	! - method to send

	character*(*)		method				! [R]
	! - method to send

	! COMPONENTS of a MESSAGE				! [R]
	character*(*)		token( MAX_TOKENS )
	! - tokens

	integer*4		tokenCount			! [R]
	! - number of tokens

	character*(*)		value( MAX_VALUES, MAX_TOKENS )	! [R]
	! - token values

	integer*4		valueLen( MAX_VALUES, MAX_TOKENS ) ! [R]
	! - token value lengths

	integer*4		valueCount( MAX_TOKENS )	! [R]
	! - token value counts

! Routines Used:

	character*8	aeqssp_autoutil_random8
	character*(DATETIME_SIZE)	aeqssp_autoutil_timeStamp	

! Author:	Mike Bergsma		7/92
!		Ab Initio Software
!
! Modifications:
!
!	Mike Bergsma	1/10/95
!	- debug message moved to aeqSsp_autoMan_send

! Local Variables:

	! MESSAGE buffer
	character*(MESSAGE_SIZE) 	message
	integer*4			messageLen
	character*8			tid
	character*(DATETIME_SIZE)	ts

! Code:

	tid = aeqSsp_autoutil_random8 ()
	ts = aeqSsp_autoUtil_timeStamp( )

	call aeqSsp_autoUtil_unparse(	DEFAULT_DELIMITER,
     &			aeqSsp_autoTbl_target(:aeqSsp_autoTbl_targetLen),
     &					mode,
     &					method,
     &			aeqSsp_autoTbl_sender(:aeqSsp_autoTbl_senderLen),
     &					tid, ts,
     &					token,
     &	   				tokenCount, 
     &					value,
     &					valueLen,
     &	   				valueCount,
     &					message, messageLen )

	call aeqSsp_autoMan_send( message(:messageLen) )

	return

	end

!++ AEQSSP_AUTOMAN_SEND

	subroutine aeqSsp_autoMan_send( message )

	implicit none

	! Low level routine that sends message

! Include files:

	include	'($IODEF)'
	include	'AEQ_SSP:AUTODEF'
	include	'AEQ_SSP:AUTOTBL'

! Arguments:

	character*(*)		message				! [R]
	! - message to send

! Routines Used:

	integer*4	sys$qiow
	integer*4	aeqSsp_autoMan_check

! Author:	Mike Bergsma		7/92
!		Ab Initio Software
!
! Modifications:
!	Mike Bergsma	1/10/95
!	- if debug, log message to scrolling region.

! Local Variables:

	integer*2	iosb(4)
	integer*4	status

! Code:

	! Send a message
	call aeqSsp_automan_logDebug (  '...sending: '//message )

	! Write message to mailbox.
	status = sys$qiow(	%val(EFN_MBX_OUT),
     &				%val(aeqSsp_autoTbl_outChannel),
     &				%val(IO$_WRITEVBLK+IO$M_NOW+IO$M_NORSWAIT),
     &				iosb,
     &				,,
     &				%ref(message),
     &				%val(LEN(message)),
     &				,,, )

	if ( .not. aeqSsp_autoMan_check( status, iosb(1)) ) then

          call aeqSsp_autoMan_outMessage( 
     &		'% Failed to send message to '//
     &		aeqSsp_autoTbl_target(:aeqSsp_autoTbl_targetLen)//
     &		' because writing the message to '//
     &		aeqSsp_autoTbl_mbxRouter(:aeqSsp_autoTbl_mbxRouterLen)//' has failed\n'//
     &		'The autorouter may have crashed. '//
     &		'Please retry the automation session or '//
     &		'contact your facilitator if the problem persists.',
     &		.true. )

	  ! Do not continue. Abort automation and abort PROMIS function.
	  call aeqSsp_autoMan_terminate ( .false., .false. )

	endif

	return

	end

!++ AEQSSP_AUTOMAN_FALLOUT
!
	subroutine aeqSsp_autoMan_fallout()

	implicit none

	! Cancels pending I/O from 'aeqSsp_autoMan_command' prompt which 
	! then returns and allow the processing of the message received.

! Include files:

	include	'($IODEF)'	
	include	'($PSLDEF)'	
	include 'AEQ_SSP:AUTODEF'
	include	'TUT:TUTDEF'
	include	'TUT:TUTCOM'

! Arguments:

! Routines Used:

	integer*4	sys$cancel
	logical*4	aeqSsp_automan_check

! Author:	Mike Bergsma		7/92
!		Ab Initio Software
!
! Modifications:
!
! Local Variables:

	integer*4	status

! Code:

	if ( tut_chan .ne. -1 .and. tut_chan .ne. 0 ) then

	  ! Cancel terminal i/o 
	  status = sys$cancel( %val(tut_chan) ) 
	  if ( .not. aeqSsp_autoMan_check( status, 1 ) )
     &    call aeqSsp_automan_logDebug ( '% Failed to cancel (tut_chan) terminal I/O' ) 

	  ! By setting the status of the tut iosb to success, we don't get
	  ! an error message later...
	  tut_iosb(1) = 1
	else

	  ! This is a detached process, wake it up
	  call aeqSsp_automan_logDebug( 'wake up' )
	  call sys$setef ( %val ( EFN_WAITFR ) )

	endif

	return
	end

!++ AEQSSP_AUTOMAN_AST_MBX
!
	integer*4 function aeqSsp_autoMan_AST_MBX( astParm )

	implicit none

	! Invoked when a message and incoming message is received

! Include files:

	include	'($IODEF)'	
	include	'($PSLDEF)'	
	include	'TUT:TUTDEF'
	include	'TUT:TUTCOM'
	include	'AEQ_SSP:AUTODEF'
	include	'AEQ_SSP:AUTOTBL'

! Arguments:

	integer*4	astParm					! [R]
	! AST argument

! Routines Used:

	integer*4	sys$cancel

! Arguments:

! Function value:
!
!	- returns 1 

! Routines Used:


! Author:	Mike Bergsma		7/92
!		Ab Initio Software
!
! Modifications:
!
! 	Mike Bergsma	02/20/00
!	- don't do anything about left-over AST's
!
! Local Variables:

	integer*4	status
	integer*2	channel

! Code:

	! Check to see if this AST was leftover from the last automation
	! session, probably from a double "ABORT" or "DISCONNECT".
	if ( aeqSsp_autoTbl_inChannel .eq. 0 ) then
	  ! The 'astParm' was the channel that had the AST on it.
	  !!!!if ( astParm .lt. 16000 ) then
	    !!!channel = astParm	
	    !!!status = sys$cancel( %val( channel ) )
          !!!endif
	  goto 999
	endif
	  
	! Set a flag indicating a message is in the mailbox.
	aeqSsp_autoTbl_msgPending = .true.

	! Cancel terminal i/o 
	call aeqSsp_automan_fallout()

	! Disable next AST from occurring until we've processed this one.
	call sys$setast( %val(0) )

	aeqSsp_autoMan_AST_MBX = 1

999	continue

	return
	end

!++ AEQSSP_AUTOMAN_MBXREAD
!
	integer*4 function aeqSsp_autoMan_MBXread()

	implicit none

	! Read and process a message 

! Include files:

	include	'($SSDEF)'
	include	'($IODEF)'	
	include	'($PSLDEF)'	
	include	'AEQ_SSP:AUTODEF'
	include	'AEQ_SSP:AUTOTBL'

! Arguments:

! Function value:
!
!	- returns success 

! Routines Used:

	integer*4	aeqSsp_autoMan_check
	logical*4	aeqSsp_autoUtil_parse
	logical*4	aeqSsp_autoUtil_getAddress
	logical*4	aeqSsp_autoMan_setAST
	integer*4	sys$qiow
	integer*4	sys$cantim
	integer*4	gut_trimblks

! Author:	Mike Bergsma		7/92
!		Ab Initio Software
!
! Modifications:
!
!	17-Nov-1993	Michael Bergsma
!	- set the 'connectStatus' flag when the first message from the target
!	  object has arrived
!
!	4-Jan-1993	Michael Bergsma
!	- output 'connected' message when the first message arrives.
!
!	10-Jan-1995	Michael Bergsma
!	- aeqssp_automan_outMessage changed to outStatus
!
!	10-June-1995	MB
!	- senderLen was not initialized
!
!	30-Oct-1995	MB
!	- cancel timer after first message arrives
!
!	6-Dec-1995	MB
!	- set first message only if not from ROUTER
!
! Local Variables:

	integer*2	iosb(4)
	integer*4	status

	! MESSAGE buffer
	character*(MESSAGE_SIZE) 	message
	integer*4			messageLen

	! COMPONENTS of a message
	character*1			delimiter
	character*(TARGET_SIZE)		target
	character*(INSTANCE_SIZE)	targetInstance
	character*(OBJECT_SIZE)		targetObject
	character*(NODE_SIZE)		targetNode
	integer*4			targetLen,
     &					targetInstanceLen,
     &					targetObjectLen,
     &					targetNodeLen
	character*(MODE_SIZE)		mode
	integer*4			modeLen
	character*(METHOD_SIZE)		method
	integer*4			methodLen
	character*(SENDER_SIZE)		sender
	character*(INSTANCE_SIZE)	senderInstance
	character*(OBJECT_SIZE)		senderObject
	character*(NODE_SIZE)		senderNode
	integer*4			senderLen,
     &					senderInstanceLen,
     &					senderObjectLen,
     &					senderNodeLen
	character*(FIELD_SIZE)		tid,ts
	integer*4			tidLen,tsLen
	character*(TOKEN_SIZE)		token( MAX_TOKENS )
	integer*4			tokenCount
	character*(VALUE_SIZE)		value( MAX_VALUES, MAX_TOKENS )
	integer*4			valueLen( MAX_VALUES, MAX_TOKENS )
	integer*4			valueCount( MAX_TOKENS )

! Code:

	! Read next message from mailbox
	status = sys$qiow(	%val(EFN_MBX_IN),
     &				%val(aeqSsp_autoTbl_inChannel),
     &				%val(IO$_READVBLK+IO$M_NOW),
     &				iosb,
     &				,,
     &				%ref(message),
     &				%val(MESSAGE_SIZE),
     &				,,, )

	! Check for errors.
	if ( .not. aeqSsp_autoMan_check( status, iosb(1) ) ) then

	  ! Bad errors

	  sender = 
     &		aeqSsp_autoTbl_senderInstance(:aeqSsp_autoTbl_senderInstLen)//
     &		'.'//
     &		aeqSsp_autoTbl_senderObject(:aeqSsp_autoTbl_senderObjectLen)
	  senderLen = gut_trimblks ( sender )

	  call aeqSsp_automan_outMessage( 
     &		'% Failed to read from '//
     &		aeqSsp_autoTbl_mbxRouter(:aeqSsp_autoTbl_mbxRouterLen)//'_'//sender(:senderLen)//
     &		'.\nPlease retry the automation session or '//
     &		'contact your facilitator if the problem persists.',
     &		.true. )

	  ! Do not continue, abort automation and abort PROMIS function
	  call aeqSsp_autoMan_terminate ( .false., .false. )
	  goto 999

	else

	  ! Re-establish write-attention AST
	  if ( aeqSsp_autoMan_setAST( aeqSsp_autoTbl_inChannel ) ) then

	    ! Write attention AST has been re-enabled.

	    ! Clear message pending flag. 
	    aeqSsp_autoTbl_msgPending = .false.

	  else

	    call aeqSsp_automan_terminate( .false., .false. )
	    goto 999

	  endif
	  
	endif

	! Get length of message
	messageLen = iosb(2) 

	! Check message length
	if ( messageLen .le. 0 ) then

	  ! Zero length message.  Ignore?
	  call aeqSsp_autoMan_outMessage( 
     &		'% Received zero length message, ignoring.\n'//
     &		'Please retry the automation session or '//
     &		'contact your facilitator if the problem persists.',
     &		.true. )

	! Parse message into component parts
	elseif ( .not. aeqSsp_autoUtil_parse (	message, messageLen,
     &						delimiter,
     &						target, targetLen,
     &						mode, modeLen,
     &						method, methodLen,
     &						sender, senderLen,
     &						tid, tidLen, ts, tsLen,
     &	   					token, tokenCount,
     &	   					value, valueLen, 
     &						valueCount ) ) then

	  ! Invalid message.  
	  call aeqSsp_autoMan_outMessage( 
     &		'% Invalid incoming message.\n'//
     &		'Please retry the automation session or '//
     &		'contact your facilitator if the problem persists.',
     &		.true. )

	elseif ( .not. aeqSsp_autoUtil_getAddress (	target,
     &							targetLen,
     &							targetInstance,
     &							targetInstanceLen,
     &							targetObject,
     &							targetObjectLen,
     &							targetNode,
     &							targetNodeLen ) ) then

	  ! Target of message is invalid
	  call aeqSsp_autoMan_outMessage( 
     &		'% Invalid syntax of target: '//target(:targetLen)//
     &		'.\nPlease retry the automation session or '//
     &		'contact your facilitator if the problem persists.',
     &		.true. )

	elseif ( .not. aeqSsp_autoUtil_getAddress (	sender,
     &							senderLen,
     &						      	senderInstance,
     &							senderInstanceLen,
     &							senderObject,
     &							senderObjectLen,
     &							senderNode,
     &							senderNodeLen ) ) then
	  ! Sender of message is invalid
	  call aeqSsp_autoMan_outMessage( 
     &	      	'% Invalid syntax of sender: '//sender(:senderLen)//
     &		'.\nPlease retry the automation session or '//
     &		'contact your facilitator if the problem persists.',
     &		.true. )

	! Check that message is a QUERY or and EVENT
	elseif ( mode(:modeLen) .ne. 'QUERY' .and.
     &	         mode(:modeLen) .ne. 'EVENT' ) then

	  call aeqSsp_autoMan_outMessage( 
     &		'% Incoming message was not QUERY or EVENT.\n'//
     &		'Please retry the automation session or '//
     &		'contact your facilitator if the problem persists.',
     &		.true. )

	else

	  ! Display message for the system analyst and for curious people
     	  call aeqSsp_automan_logDebug ( 'Received: '//message(:messageLen) )

	  if ( senderInstanceLen .ne. 0 .or.
     &	       senderObject(:senderObjectLen) .ne. 
     &         aeqSsp_autotbl_router(:aeqSsp_autoTbl_routerLen) ) then

	    ! Message is not from ROUTER, that's good.

	    ! In case sender of the message changed, update the global target.
	    aeqSsp_autoTbl_targetInstance = senderInstance
 	    aeqSsp_autoTbl_targetInstLen = senderInstanceLen
	    aeqSsp_autoTbl_targetObject = senderObject
	    aeqSsp_autoTbl_targetObjectLen = senderObjectLen
	    aeqSsp_autoTbl_targetNode = senderNode
	    aeqSsp_autoTbl_targetNodeLen = senderNodeLen
	    call aeqSsp_autoUtil_putAddress (	aeqSsp_autoTbl_target, 
     &						aeqSsp_autoTbl_targetLen,
     &						aeqSsp_autoTbl_targetInstance,
     &						aeqSsp_autoTbl_targetInstLen,
     &						aeqSsp_autoTbl_targetObject,
     &						aeqSsp_autoTbl_targetObjectLen,
     &						aeqSsp_autoTbl_targetNode,
     &						aeqSsp_autoTbl_targetNodeLen )

            if ( .not. aeqSsp_autoTbl_connectStatus ) then

	      ! This was our first message 
	      aeqSsp_autoTbl_connectStatus = .true.

	      ! Cancel connection timer.
	      status = sys$cantim( %val(0), %val(PSL$C_USER) )
		if ( .not. aeqSsp_autoMan_check( status, 1 ) )
     &  	  call aeqSsp_automan_logDebug ( '% Failed to cancel connection timer' )

	      call aeqSsp_automan_outMessage ( 
     &			'Connected to '//
     &			aeqSsp_autoTbl_target(:aeqSsp_autoTbl_targetLen),
     &			.true. )

	      ! We are officially connected to the target object.
	      ! We can remove the pagewait and set the PROMIS function to 
	      ! succeed
	      aeqSsp_autoTbl_pagewait = .false.
	      aeqSsp_autoTbl_functionStatus = .true.

	    endif

	  endif

	  ! Process incoming request
	  call aeqSsp_autoMan_server(	sender(:senderLen),
     &	  				mode(:modeLen),
     &	  				method(:methodLen),
     &	  				target(:targetLen),
     &					tid(:tidLen), ts(:tsLen),
     &	  				token, tokenCount,
     &	  				value, valueLen, valueCount )


	  ! Check to see if the session should be terminating

	  if ( senderInstanceLen .eq. 0 .and.
     &	       senderObject(:senderObjectLen) .eq. 
     &         aeqSsp_autotbl_router(:aeqSsp_autoTbl_routerLen) ) then

	    ! CASE 1:
	    !
	    !	ROUTER has sent us a message, meaning the connection
	    !	to the target is unreachable or has been lost.
	    !	Escape to Manual operation will be allowed so that
	    !	the PROMIS function may be allowed to succeed,
	    !
            call aeqSsp_automan_terminate( .false., .false. )
	    goto 999

	  elseif ( method(:methodLen) .eq. 'DISCONNECT' ) then

	    ! CASE 2:
	    !
	    !	The target has sent us a DISCONNECT message which
	    !	means automation was successful
	    !	The PROMIS function will succeed.
	    !
	    call aeqSsp_automan_terminate( .true., .true. )
	    goto 999

	  elseif ( method(:methodLen) .eq. 'ABORT' ) then

	    ! CASE 3:
	    !
	    !	The target has sent us a ABORT message which
	    !	means automation was denied
	    !	The PROMIS function will fail.
	    !
	    call aeqSsp_automan_terminate( .false., .true. )
	    goto 999

	  endif

	endif

999	continue

	aeqSsp_autoMan_MBXread = 1

	return

	end

!++ AEQSSP_AUTOMAN_SERVER
!
	subroutine aeqSsp_autoMan_server(	target,
     &						mode,
     &						method,
     &						sender, 
     &						tid, ts,
     &						token, tokenCount, 
     &						value, valueLen, valueCount )

	implicit none

! 	This routine processes QUERY and EVENT requests from the action 
!
!	schedule for the following methods and tokens.
!
!	Method	Token		Access		Value
!	------------------------------------------------------------------------
!	CONNECT	LOTID		Read-only	Lot id (for enter lot data)
!		EQPID		Read-only	Equipment id (for gather data)
!		USERID		Read-only	Userid
!		AUTOTYPE	Read-only	DATA, BATCH, TRACKIN
!
!	DISCONNECT/ABORT
!		PAGEWAIT	Read-write	Optional. Sets pageWait ON.
!		TEXT		Write-only	Text message
!		TEXT2		Write-only	Text message2
!		TEXT3		Write-only	Text message3
!		$<param>	Read-write	Parameter value
!		$<dbField>	Read-only	Database info
!
!	MESSAGE	TEXT		Write-only	Text message
!		TEXT2		Write-only	Text message2
!		TEXT3		Write-only	Text message3
!		PROMPT		Read-only	Prompt reponse
!		ECHO		Write-only	YES (default) or NO
!
!	SETUP	$<param>	Read-write	Parameter value
!		$<dbField>	Read-only	Database info
!		USERID		Read-only	Track-in User Id
!		NUMWAFERS	Read-only	Number of wafers in lot
!		WAFERS		Read-only	Array of lot wafers
!		AUTOTYPE	Read-only	DATA, TRACKIN, BATCH
!
!	DATA	STATE		Read-only	State of data. (NONE,SOME,DONE)
!		TESTTYPE	Read-only	ITEM, 2DCOMP, SITE, etc
!		WAFERID		Read/write	Wafer index into test results
!		SITEID		Read/write	Site index into test results
!		ITEMID		Read/write	Item index into test results
!		&<data item>	Write-only	Test results value
!		$<param>	Read-write	Parameter value
!		$<dbField>	Read-only	Database info
!		NUMWAFERS	Read-only	Number of wafers to test
!		WAFERS		Read-only	Array of test wafers
!		NUMSITES	Read-only	Number of sites to test
!		SITES		Read-only	Array of site names
!		NUMITEMS	Read-only	Number of items to test
!		ITEMS		Read-only	Array of data item names
!
!	HOTKEY	KEY		Write-only	Key name to define (or undefine)
!		POSITION	Write-only	Position (1-7) or (0)
!		METHOD		Write-only	Method to send as EVENT message
!		ENABLED		Write-only	YES or NO
	        	
! Include files:

	include	'FIL:TYPDEF'
	include	'FIL:FILDEF'
	include	'GUT:GUTDEF'
	include	'TUT:TUTDEF'
	include	'TST:TSTDEF'	
	include	'TST:ENTDEF'	
	include	'TST:PARMDEF'	
	include 'LOT:FUNDEF'
	include 'PAR:LOTSTDEF'
	include 'PAR:PRODSTDEF'
	include	'LOT:ACTLBUFF'	
	include	'FIL:TESTCDEF'	
	include	'FIL:TRESCDEF'	
	include	'TST:TSTWCOM'	

	include	'AEQ_SSP:AUTODEF'
	include	'AEQ_SSP:AUTOTBL'

! Arguments:

	character*(*)			target				! [R]
	! - target instance

	character*(*)			method				! [R]
	! - instance method

	character*(*)			mode				! [R]
	! - mode (QUERY, REPLY, or EVENT)

	character*(*)			sender				! [R]
	! - sender instance

	character*(*)			tid				! [R]
	! - transaction id

	character*(*)			ts				! [R]
	! - timestamp

	character*(*)			token( MAX_TOKENS)		! [R]
	! - tokens in message

	integer*4			tokenCount			! [R]
	! - # of tokens

	character*(*)			value( MAX_VALUES, MAX_TOKENS)	! [R]
	! - values of tokens

	integer*4			valueLen( MAX_VALUES, MAX_TOKENS)! [R]
	! - length of values of tokens

	integer*4			valueCount( MAX_TOKENS )	! [R]
	! - # of values per tokens

! Routines Used:

	integer*4	gut_trimblks

	logical*4	aeqSsp_autoUtil_unparse

	logical*4	aeqSsp_automan_DATA 
	logical*4	aeqSsp_automan_SETUP
	logical*4	aeqSsp_automan_CONNECT
	logical*4	aeqSsp_automan_DISCONNECT
	logical*4	aeqSsp_automan_MESSAGE
	logical*4	aeqSsp_automan_HOTKEY

! Author:	Mike Bergsma		7/92
!		Ab Initio Software
!
! Modifications:
!
!	21-Jan-1994	Michael Bergsma
!	- add USERID token for SETUP method
!
!	Mike Bergsma	3/7/94
!	- keep track of our own variable "aeqSsp_autoTbl_hasEnteredData"
!
!	Mike Bergsma	3/9/94
!	- when received ABORT, send back ABORT
!
!	Mike Bergsma	5/10/94
!	- modularize, each "method" is its own subroutine.
!
!	Mike Bergsma	1/10/95
!	- debug message moved to aeqSsp_autoMan_send
!
!	Mike Bergsma	7/15/96
!	- add ECHO token
!
!	Mike Bergsma	2/26/97
!	- add AUTOTYPE token
!
! Local variables:

	logical*4	status

	character*(VALUE_SIZE) 		statusText

	! Reply MESSAGE 
	character*(MESSAGE_SIZE) 	replyMessage
	integer*4			replyMessageLen
	character*(TOKEN_SIZE)		replyToken( MAX_TOKENS )
	integer*4			replyTokenCount
	character*(VALUE_SIZE)		replyValue( MAX_VALUES, MAX_TOKENS )
	integer*4			replyValueLen( MAX_VALUES, MAX_TOKENS )
	integer*4			replyValueCount( MAX_TOKENS )

! Code:
	! Do CASE handling of each method.

     	call aeqSsp_automan_logDebug ( '...processing '//method//' method' )

	! Intialize reply
	replyTokenCount = 0

	if ( method .eq. 'DATA' ) then

	  status = aeqSsp_automan_DATA (	mode,
     &						token, tokenCount, 
     &						value, valueLen, valueCount,
     &						statusText,
     &						replyToken, replyTokenCount,
     &						replyValue,
     &						replyValueLen,
     &						replyValueCount )

	elseif ( method .eq. 'SETUP' ) then

	  status = aeqSsp_automan_SETUP (	mode,
     &						token, tokenCount, 
     &						value, valueLen, valueCount,
     &						statusText,
     &						replyToken, replyTokenCount,
     &						replyValue,
     &						replyValueLen,
     &						replyValueCount )

	elseif ( method .eq. 'CONNECT' ) then

	  status = aeqSsp_automan_CONNECT (	mode,
     &						token, tokenCount, 
     &						value, valueLen, valueCount,
     &						statusText,
     &						replyToken, replyTokenCount,
     &						replyValue,
     &						replyValueLen,
     &						replyValueCount )


	elseif ( method .eq. 'DISCONNECT' .or.
     &		 method .eq. 'ABORT' ) then

	  status = aeqSsp_automan_DISCONNECT (	mode,
     &						token, tokenCount, 
     &						value, valueLen, valueCount,
     &						statusText,
     &						replyToken, replyTokenCount,
     &						replyValue,
     &						replyValueLen,
     &						replyValueCount )

	elseif ( method .eq. 'MESSAGE' ) then

	  status = aeqSsp_automan_MESSAGE (	mode,
     &						token, tokenCount, 
     &						value, valueLen, valueCount,
     &						statusText,
     &						replyToken, replyTokenCount,
     &						replyValue,
     &						replyValueLen,
     &						replyValueCount )

	elseif ( method .eq. 'HOTKEY' ) then

	  status = aeqSsp_automan_HOTKEY (	mode,
     &						token, tokenCount, 
     &						value, valueLen, valueCount,
     &						statusText,
     &						replyToken, replyTokenCount,
     &						replyValue,
     &						replyValueLen,
     &						replyValueCount )

	else

	  ! Unrecognized method
	  status = .false.
	  statusText = '% Undefined method '//method

	endif

	if ( .not. status )
     &	  call aeqSsp_automan_logDebug( statusText )

	if ( mode .eq. 'QUERY' ) then

	  ! Reply is expected.
	  ! Add status to end of reply.
	  replyTokenCount = replyTokenCount + 1
	  replyToken(replyTokenCount) = 'STATUS'
	  replyValueCount(replyTokenCount) = 1
	  replyValue(1,replyTokenCount) = statusText
	  replyValueLen(1,replyTokenCount) = 
     &			gut_trimblks( replyValue(1,replyTokenCount) )

	  if ( aeqSsp_autoUtil_unparse(	DEFAULT_DELIMITER,
     &					target,
     &					'REPLY',
     &					method,
     &					sender,
     &					tid, ts,
     &					replyToken, 
     &	   				replyTokenCount, 
     &					replyValue,
     &					replyValueLen,
     &	   				replyValueCount,
     &					replyMessage, 
     &					replyMessageLen ) ) then

	    call aeqSsp_autoMan_send( replyMessage(:replyMessageLen) )

	  else

	    ! Cannot construct message to REPLY to target
	    replyTokenCount = 1
	    replyToken(1) = 'STATUS'
	    replyValueCount(1) = 1
	    replyValue(1,1) = '% Message buffer overflow'
	    replyValueLen(1,replyTokenCount) = 
     &			gut_trimblks( replyValue(1,replyTokenCount) )

	    call aeqSsp_autoUtil_unparse(	DEFAULT_DELIMITER,
     &						target,
     &						'REPLY',
     &						method,
     &						sender,
     &						tid, ts,
     &						replyToken, 
     &	   					replyTokenCount, 
     &						replyValue,
     &						replyValueLen,
     &	   					replyValueCount,
     &						replyMessage, 
     &						replyMessageLen ) 

	    call aeqSsp_autoMan_send( replyMessage(:replyMessageLen) )

	  endif

	endif

	return

	end

!++ AEQSSP_AUTOMAN_DATA
!	
	logical*4 function aeqSsp_automan_DATA (	mode,
     &							token, 
     &							tokenCount, 
     &							value, 
     &							valueLen, 
     &							valueCount,
     &							statusText,
     &							replyToken, 
     &							replyTokenCount,
     &							replyValue,
     &							replyValueLen,
     &							replyValueCount )

	implicit none

! 	This routine processes QUERY and EVENT requests for the DATA method

! Include files:

	include	'FIL:TYPDEF'
	include	'FIL:FILDEF'
	include	'GUT:GUTDEF'
	include	'TUT:TUTDEF'
	include	'TST:TSTDEF'	
	include	'TST:ENTDEF'	
	include	'TST:PARMDEF'	
	include 'LOT:FUNDEF'
	include 'PAR:LOTSTDEF'
	include 'PAR:PRODSTDEF'
	include	'LOT:ACTLBUFF'	
	include	'FIL:TESTCDEF'	
	include	'FIL:TRESCDEF'	
	include	'TST:TSTWCOM'	

	include	'AEQ_SSP:AUTODEF'
	include	'AEQ_SSP:AUTOTBL'

! Arguments:

	character*(*)			mode				! [R]
	! - mode (QUERY, REPLY, or EVENT)

	character*(*)			token( MAX_TOKENS)		! [R]
	! - tokens in message

	integer*4			tokenCount			! [R]
	! - # of tokens

	character*(*)			value( MAX_VALUES, MAX_TOKENS)	! [R]
	! - values of tokens

	integer*4			valueLen( MAX_VALUES, MAX_TOKENS)! [R]
	! - length of values of tokens

	integer*4			valueCount( MAX_TOKENS )	! [R]
	! - # of values per tokens

	character*(*)			statusText			! [W]
	! - reply status text

	character*(*)			replyToken( MAX_TOKENS)		! [W]
	! - tokens in reply message

	integer*4			replyTokenCount			! [W]
	! - # of reply tokens

	character*(*)			replyValue( MAX_VALUES, MAX_TOKENS) ![W]
	! - values of reply tokens

	integer*4			replyValueLen(MAX_VALUES,MAX_TOKENS)![W]
	! - length of values of reply tokens

	integer*4			replyValueCount( MAX_TOKENS )	! [W]
	! - # of values per reply tokens

! Routines Used:

	integer*4	gut_trimblks
	integer*4	aeqSsp_autoUtil_left_justify
	logical*4	aeqSsp_autoMan_validData
	integer*4	aeqSsp_autoMan_siteIndex 
	integer*4	aeqSsp_autoMan_waferIndex
	integer*4	aeqSsp_autoMan_itemIndex
	logical*4	aeqSsp_autoMan_getParm
	logical*4	aeqSsp_autoMan_getTRfield

! Author:	Mike Bergsma		7/92
!		Ab Initio Software
!
! Modifications:
!
!	9/20/94	Mike Bergsma
!	- remove recursive switch to 'aeqssp_automan_getParm' because
!	  it has been rewritten to know whether to do recursion.
!
!	Mike Bergsma		12/22/94
!	- add support for $file.field database variables
!
! Local variables:

	parameter	FIRSTTIME = .true.

	logical*4	status,
     &			okWafer,
     &			okSite,
     &			okItem

	integer*4	i,j,
     &			ios,
     &			compOffset,
     &			waferIdLen,
     &			siteIdLen,
     &			itemIdLen,
     &			dataLen,
     &			waferIndex,
     &			siteIndex,
     &			itemIndex,
     &    		statusTextLen

	character*(TYP__COMPONENTIDSZ)	waferId
	character*(TYP__COMPONENTIDSZ)	siteId
	character*(TYP__DIPROMPTSZ) 	itemId

	character*(TYP__PARMNMSZ)	parmName
	character*(TYP__PARMVALSZ)	parmVal
	character*(1)			parmType

	integer*4			subs( TRES_X_DIMNUMVALUES )
	data				subs /0,0,0,0/

	character*1			dataType

	! Data value storage
	character*(VALUE_SIZE)		charVal
	integer*4			intVal
	integer*4			dateVal
	real*4				realVal

	status = .true.
	statusText = '$ Acknowledged'

	if ( .not. aeqSsp_autoTbl_canEnterData ) then
	  ! Data entry not allowed.
	  status = .false.
	  statusText = '% Data entry not allowed'
	endif

	i = 1
	do while ( i .le. tokenCount .and. status )

	  if ( valueCount(i) .eq. 0 .and. mode .eq. 'QUERY' .and.
     &	       replyTokenCount .eq. MAX_TOKENS - 1 ) then
	    status = .false.
	    statusText = '% MAX_TOKENS exceeded in AUTODEF.FOR.'

	  ! Do CASE handling of each token in the reply.
	  else if ( token(i) .eq. 'STATE' ) then

	    if ( valueCount(i) .eq. 0 .and. mode .eq. 'QUERY' ) then

	      ! Target wants value of STATE
	      replyTokenCount = replyTokenCount + 1
	      replyToken(replyTokenCount) = token(i)
	      replyValueCount(replyTokenCount) = 1
	      replyValue(1,replyTokenCount) = aeqSsp_autoTbl_state
	      replyValueLen(1,replyTokenCount) = 
     &			gut_trimblks( replyValue(1,replyTokenCount) )

	    else

	      ! Target wants to set STATE
	      status = .false.
	      statusText = '% Access violation. Cannot set STATE'

	    endif

	  else if ( token(i) .eq. 'TESTTYPE' ) then

	    if ( valueCount(i) .eq. 0 .and. mode .eq. 'QUERY' ) then

	      ! Target wants value of TESTTYPE
	      replyTokenCount = replyTokenCount + 1
	      replyToken(replyTokenCount) = token(i)
	      replyValueCount(replyTokenCount) = 1
	      replyValue(1,replyTokenCount) = test.testType
	      replyValueLen(1,replyTokenCount) = 
     &			gut_trimblks( replyValue(1,replyTokenCount) )

	    else

	      ! Target wants to set TESTTYPE
	      status = .false.
	      statusText = '% Access violation. Cannot set TESTTYPE'

	    endif

	  elseif ( token(i) .eq. 'NUMWAFERS' ) then

	    if ( valueCount(i) .eq. 0 .and. mode .eq. 'QUERY' ) then

	      ! Target wants value of NUMWAFERS
	      replyTokenCount = replyTokenCount + 1
	      replyToken(replyTokenCount) = token(i)
	      replyValueCount(replyTokenCount) = 1
	      write( replyValue(1,replyTokenCount), '(I2)', iostat=ios )
     &			tres.compsToTest
	      replyValueLen(1,replyTokenCount) = 
     &		aeqSsp_autoUtil_left_justify( replyValue(1,replyTokenCount) )
		
	    else

	      ! Target wants to set NUMWAFERS
	      status = .false.
	      statusText = '% Access violation. Cannot set NUMWAFERS'

	    endif

	  elseif ( token(i) .eq. 'WAFERS' ) then

	    if ( valueCount(i) .eq. 0 .and. mode .eq. 'QUERY' ) then

	      ! Target wants list of all WAFERS
	      replyTokenCount = replyTokenCount + 1
	      replyToken(replyTokenCount) = token(i)
	      replyValueCount(replyTokenCount) = tres.compsToTest

	      ! For repeatable dcop's the component(s) to return begins
	      ! at the offset of the last test.
    	      if ( tst_tstWcom_isEnterTest .and. 
     &		   tres.compsToTest .le. tres.compCount .and. 
     &             tres.compsToTest+tres.compCount .le. actl.compCount .and.
     &             actl.cacheMaxRepeats(tst_tstwcom_testIndex) .lt.
     &             (actl.testsdone(tst_tstwcom_testIndex) +
     &              actl.testslatered(tst_tstwcom_testIndex)) ) then
	        compOffset = tres.compCount
	      else
		compOffset = 0	      
	      endif	
	      
	      do j=1,tres.compsToTest
	        replyValue(j,replyTokenCount) = tres.compIds(j+compOffset)
		replyValueLen(j,replyTokenCount) = 
     &			gut_trimblks( replyValue(j,replyTokenCount) )
	      enddo
		
	    else

	      ! Target wants to set WAFERS
	      status = .false.
	      statusText = '% Access violation. Cannot set WAFERS'

	    endif

	  elseif ( token(i) .eq. 'NUMSITES' ) then

	    if ( valueCount(i) .eq. 0 .and. mode .eq. 'QUERY' ) then

	      ! Target wants value of NUMSITES
	      replyTokenCount = replyTokenCount + 1
	      replyToken(replyTokenCount) = token(i)
	      replyValueCount(replyTokenCount) = 1
	      write( replyValue(1,replyTokenCount), '(I2)', iostat=ios )
     &			test.nosites
	      replyValueLen(1,replyTokenCount) = 
     &		aeqSsp_autoUtil_left_justify( replyValue(1,replyTokenCount) )
		
	    else

	      ! Target wants to set NUMSITES
	      status = .false.
	      statusText = '% Access violation. Cannot set NUMSITES'

	    endif

	  elseif ( token(i) .eq. 'SITES' ) then

	    if ( valueCount(i) .eq. 0 .and. mode .eq. 'QUERY' ) then

	      ! Target wants values for all SITES
	      replyTokenCount = replyTokenCount + 1
	      replyToken(replyTokenCount) = token(i)
	      replyValueCount(replyTokenCount) = test.noSites
	      do j=1,test.noSites
	        replyValue(j,replyTokenCount) = tst_tstwcom_SiteNames(j)
		replyValueLen(j,replyTokenCount) = 
     &			gut_trimblks( replyValue(j,replyTokenCount) )
	      enddo
		
	    else

	      ! Target wants to set SITES
	      status = .false.
	      statusText = '% Access violation. Cannot set SITES'
              
	    endif

	  elseif ( token(i) .eq. 'NUMITEMS' ) then

	    if ( valueCount(i) .eq. 0 .and. mode .eq. 'QUERY' ) then

	      ! Target wants value of NUMITEMS
	      replyTokenCount = replyTokenCount + 1
	      replyToken(replyTokenCount) = token(i)
	      replyValueCount(replyTokenCount) = 1
	      write( replyValue(1,replyTokenCount), '(I3)', iostat=ios )
     &			test.noTestDi
	      replyValueLen(1,replyTokenCount) = 
     &		aeqSsp_autoUtil_left_justify( replyValue(1,replyTokenCount) )
		
	    else

	      ! Target wants to set NUMITEMS
	      status = .false.
	      statusText = '% Access violation. Cannot set NUMITEMS'

	    endif

	  elseif ( token(i) .eq. 'ITEMS' ) then

	    if ( valueCount(i) .eq. 0 .and. mode .eq. 'QUERY' ) then

	      ! Target wants values of all ITEMS
	      replyTokenCount = replyTokenCount + 1
	      replyToken(replyTokenCount) = token(i)
	      replyValueCount(replyTokenCount) = test.noTestDi
	      do j=1,test.noTestDi
		replyValue(j,replyTokenCount) = test.di(j).prompt
		replyValueLen(j,replyTokenCount) = 
     &			gut_trimblks( replyValue(j,replyTokenCount) )
	      enddo
		
	    else

	      ! Target wants to set ITEMS
	      status = .false.
	      statusText = '% Access violation. Cannot set ITEMS'

	    endif

	  elseif ( token(i) .eq. 'WAFERID' ) then

	    if ( tst_tstWcom_typeIndex .eq. TST__2DCOMP .or.
     &	         tst_tstWcom_typeIndex .eq. TST__PROBE .or.
     &	         tst_tstWcom_typeIndex .eq. TST__SITE ) then

	      if ( valueCount(i) .eq. 0 .and. mode .eq. 'QUERY' ) then

		! Target wants current value of WAFERID
		replyTokenCount = replyTokenCount + 1
		replyToken(replyTokenCount) = token(i)
		replyValueCount(replyTokenCount) = 1

	        waferIndex = aeqSsp_autoTbl_waferIndex
	        call aeqSsp_autoMan_waferId(	waferIndex, 
     &						waferId, 
     &						waferIdLen )

		replyValue(1,replyTokenCount) = waferId
		replyValueLen(1,replyTokenCount) = 
     &			gut_trimblks( replyValue(1,replyTokenCount) )
		
	      elseif ( valueCount(i) .eq. 0 ) then

		status = .false.
		statusText = '% WAFERID value not specified'

	      elseif ( valueCount(i) .eq. 1 ) then

		! Target wants to set WAFERID
		waferId = value(1,i)(:valueLen(1,i))
	        waferIdLen = gut_trimblks( waferId )
	        waferIndex = aeqSsp_autoMan_waferIndex( waferId, waferIdLen )

	        if ( waferIndex .eq. INDEX_UNDEFINED ) then
		  statusText = '% WAFERID is not available'
		  status = .false.
	        elseif ( waferIndex .le. INDEX_UNSPECIFIED ) then
		  statusText = '% WAFERID not specified'
		  status = .false.
	        else
		  ! Success
		  aeqSsp_autoTbl_waferIndex = waferIndex
	        endif

	      else

		statusText = '% Too many WAFERID values'
		status = .false.

	      endif

	    else
	      statusText = '% DcOp is not a 2DCOMP/PROBE/SITE test'
	      status = .false.
	    endif

	  elseif ( token(i) .eq. 'SITEID' ) then

	    if ( tst_tstWcom_typeIndex .eq. TST__SITE ) then

	      if ( valueCount(i) .eq. 0 .and. mode .eq. 'QUERY' ) then

		! Target wants current value of SITEID
		replyTokenCount = replyTokenCount + 1
		replyToken(replyTokenCount) = token(i)
		replyValueCount(replyTokenCount) = 1

	        siteIndex = aeqSsp_autoTbl_siteIndex
	        call aeqSsp_autoMan_siteId(	siteIndex, 
     &						siteId, 
     &						siteIdLen )

		replyValue(1,replyTokenCount) = siteId
		replyValueLen(1,replyTokenCount) = 
     &		gut_trimblks( replyValue(1,replyTokenCount) )
		
	      elseif ( valueCount(i) .eq. 0 ) then

		statusText = '% SITEID not specified'
		status = .false.

	      elseif ( valueCount(i) .eq. 1 ) then

	        ! Target wants to set SITEID
	        siteId = value(1,i)(:valueLen(1,i))
	        siteIdLen = gut_trimblks( siteId )
	        siteIndex = aeqSsp_autoMan_siteIndex(	siteId,
     &							siteIdLen,
     &							test.noSites )

	        if ( siteIndex .eq. INDEX_UNDEFINED ) then
		  statusText = '% SITEID not in range'
		  status = .false.
	        elseif ( siteIndex .le. INDEX_UNSPECIFIED ) then
		  statusText = '% SITEID not specified'
		  status = .false.
	        else
		  ! Success
		  aeqSsp_autoTbl_siteIndex = siteIndex
	        endif

	      else

		statusText = '% Too many SITEID values'
		status = .false.

	      endif

	    else
	      ! Not a site test, therefore site is implicitly = 1
	      replyTokenCount = replyTokenCount + 1
	      replyToken(replyTokenCount) = token(i)
	      replyValueCount(replyTokenCount) = 1
	      replyValue(1,replyTokenCount) = '1'
	      replyValueLen(1,replyTokenCount) = 1
	    endif

	  elseif ( token(i) .eq. 'ITEMID' ) then

	    if ( valueCount(i) .eq. 0 .and. mode .eq. 'QUERY' ) then

	      ! Target wants current value of ITEMID
	      replyTokenCount = replyTokenCount + 1
	      replyToken(replyTokenCount) = token(i)
	      replyValueCount(replyTokenCount) = 1

	      itemIndex = aeqSsp_autoTbl_itemIndex
	      call aeqSsp_autoMan_itemId(	itemIndex, 
     &						itemId, 
     &						itemIdLen )

	      ! Success
	      replyValue(1,replyTokenCount) = itemId
	      replyValueLen(1,replyTokenCount) = 
     &			gut_trimblks( replyValue(1,replyTokenCount) )
		
	    elseif ( valueCount(i) .eq. 0 ) then

	      statusText = '% ITEMID not specified'
	      status = .false.

	    elseif ( valueCount(i) .eq. 1 ) then

	      ! Target wants to set ITEMID
	      itemId = value(1,i)(:valueLen(1,i))
	      itemIdLen = gut_trimblks( itemId )
	      itemIndex = aeqSsp_autoMan_itemIndex( itemId, itemIdLen )

	      if ( itemIndex .eq. INDEX_UNDEFINED ) then
		statusText = '% ITEMID not recognized'
		status = .false.
	      elseif ( itemIndex .le. INDEX_UNSPECIFIED ) then
		statusText = '% ITEMID not specified'
		status = .false.
	      else
		! Success
		aeqSsp_autoTbl_itemIndex = itemIndex
	      endif

	    else

	      statusText = '% Too many ITEMID values'
	      status = .false.

	    endif

	  elseif ( token(i)(1:1) .eq. '$' ) then

	    if ( INDEX ( token(i), '.' ) .eq. 6 ) then
	    
	      if ( valueCount(i) .eq. 0 .and. mode .eq. 'QUERY' ) then

	        ! Target wants current value of database field
	        status = aeqSsp_autoMan_getTRfield(
     &					token(i),
     &			        	charVal,
     &					dataLen,
     &					statusText,
     &					statusTextLen )

	        if ( status ) then

		  replyTokenCount = replyTokenCount + 1
		  replyToken(replyTokenCount) = token(i)
		  replyValueCount(replyTokenCount) = 1
		  replyValue(1,replyTokenCount) = charVal
		  replyValueLen(1,replyTokenCount) = dataLen
		
	        endif

	      else

	        ! Target wants to set database field
	        status = .false.
	        statusText = '% Access violation. Cannot set database field'

	      endif

	    else

	      ! Must be parameter field.
	      if ( valueCount(i) .eq. 0 .and. mode .eq. 'QUERY' ) then

	        ! Target wants current value of parameter
	        parmName = token(i)
	        if ( aeqSsp_autoMan_getParm(	parmName,
     &			        		aeqSsp_autoTbl_parmSource,
     &						parmVal ) ) then

		  replyTokenCount = replyTokenCount + 1
		  replyToken(replyTokenCount) = parmName
		  replyValueCount(replyTokenCount) = 1
		  replyValue(1,replyTokenCount) = parmVal
		  replyValueLen(1,replyTokenCount) = 
     &			gut_trimblks( replyValue(1,replyTokenCount) )

	        else

	          statusText = '% Undefined parameter '//parmName
		  status = .false.
		
	        endif

	      elseif ( valueCount(i) .eq. 1 .or. valueCount(i) .eq. 2 ) then

	        if ( aeqSsp_autoTbl_hasLotid ) then

		  parmName = token(i)
		  parmVal = value(1,i)(:valueLen(1,i))

	          if ( valueCount(i) .eq. 2 ) then

		    parmType = value(2,i)
		    if ( parmType .ne. PAR_PRODSTDEF_PTYPE_STRING .and.
     &		         parmType .ne. PAR_PRODSTDEF_PTYPE_REAL4 .and.
     &		         parmType .ne. PAR_PRODSTDEF_PTYPE_INTEGER .and.
     &		         parmType .ne. PAR_PRODSTDEF_PTYPE_PART .and.
     &		         parmType .ne. PAR_PRODSTDEF_PTYPE_DCOPER ) 
     &		      parmType = PAR_PRODSTDEF_PTYPE_STRING

		  else

		    parmType = PAR_PRODSTDEF_PTYPE_STRING

		  endif

                  call Tst_ActlUtil_PutSetParm(	actl, 
     &						parmName, 
     &						parmVal, 
     &						parmType, 
     &						.true. )

	        else

	          statusText = '% Cannot set parameter for non lot tests' 
		  status = .false.

	        endif
	      endif
	    endif

	  elseif ( token(i)(1:1) .eq. '&' ) then

	    if ( valueCount(i) .eq. 0 .and. mode .eq. 'QUERY' ) then

	      ! Target wants current value of data item
	      status = .false.
	      statusText = '% Access violation. Cannot retrieve data'
		
	    elseif ( valueCount(i) .eq. 0 ) then

	      statusText = '% Data values not specified'
	      status = .false.

	    elseif ( valueCount(i) .gt. 0 ) then

	      ! Target wants to set values
               
	      ! First see if item id was specified.
	      itemId = token(i)(2:)
	      itemIdLen = gut_trimblks( itemId )
	      itemIndex = aeqSsp_autoMan_itemIndex( itemId, itemIdLen )
	        
	      if ( itemIndex .eq. INDEX_UNDEFINED ) then
		statusText = '% Undefined ITEMID in dcOp'
		status = .false.
	      elseif ( itemIndex .gt. 0 ) then
		aeqSsp_autoTbl_itemIndex = itemIndex
	      !else 
		! Item not specified.  That's ok. Just use current index
	      endif

	      j = 1
	      do while( status .and. j  .le. valueCount(i) )

	        okItem = (	aeqSsp_autoTbl_itemIndex .ge. 1 .and. 
     &			   	aeqSsp_autoTbl_itemIndex .le. test.noTestDi )

	        if (	tst_tstWcom_typeIndex .eq. TST__2DCOMP .or.
     &	             	tst_tstWcom_typeIndex .eq. TST__PROBE .or.
     &	             	tst_tstWcom_typeIndex .eq. TST__SITE ) then

	          okWafer = (	aeqSsp_autoTbl_waferIndex .ge. 1 .and. 
     &		  	     	aeqSsp_autoTbl_waferIndex .le. 
     &				tst_tstWCom_numAvailComps )

	        else

	          okWafer = .true.

	        endif

	        if ( tst_tstWcom_typeIndex .eq. TST__SITE ) then

	          okSite = ( 	aeqSsp_autoTbl_siteIndex .ge. 1 .and. 
     &				aeqSsp_autoTbl_siteIndex .le. test.noSites )

	        else

	          okSite = .true.

	        endif

	        if ( okWafer .and. okSite .and. okItem ) then

	          dataType = test.di(aeqSsp_autoTbl_itemIndex).type
	          if ( aeqSsp_autoMan_validData(value(j,i)(:valueLen(j,i)),
     &		  				dataType,
     &						realVal,
     &						intVal,
     &						charVal,
     &						dataLen ) ) then

	      	    call aeqSsp_autoMan_itemId(	aeqSsp_autoTbl_itemIndex,
     &						itemId, 
     &						itemIdLen )

	      	    ! Calculate array indices for WAFER (and SITE)
	      	    if ( tst_tstWcom_typeIndex .eq. TST__SITE ) then

	              ! SITE test
	              subs(1) = aeqSsp_autoTbl_waferIndex 	! Wafer index
	              subs(2) = aeqSsp_autoTbl_siteIndex	! Site index
	              subs(3) = aeqSsp_autoTbl_itemIndex	! Item index

	              call aeqSsp_autoMan_waferId(	subs(1),
     &							waferId, 
     &							waferIdLen )

	              call aeqSsp_autoMan_siteId(	subs(2),
     &							siteId, 
     &							siteIdLen )

	              call aeqSsp_automan_logDebug ( 
     &			'...setting '//itemId(:itemIdLen)//'('//
     &			waferId(:waferIdLen)//','//siteId(:siteIdLen)//
     &			') = '//value(j,i)(:valueLen(j,i)) )

	            elseif ( tst_tstWcom_typeIndex .eq. TST__2DCOMP .or.
     &                       tst_tstWcom_typeIndex .eq. TST__PROBE ) then

	              ! WAFER test
	              subs(1) = aeqSsp_autoTbl_waferIndex	! Wafer index
	              subs(2) = aeqSsp_autoTbl_itemIndex	!  Item index

	              call aeqSsp_autoMan_waferId(	subs(1),
     &							waferId, 
     &							waferIdLen )

	              call aeqSsp_automan_logDebug ( 
     &			'...setting '//itemId(:itemIdLen)//'('//
     &			waferId(:waferIdLen)//') = '//
     &			value(j,i)(:valueLen(j,i)) )

	            else

	              ! ITEM test
	              subs(1) = aeqSsp_autoTbl_itemIndex	!  Item index

	              call aeqSsp_automan_logDebug ( 
     &			'...setting '//itemId(:itemIdLen)//
     &			' = '//value(j,i)(:valueLen(j,i)) )

 		    endif

	            call Tst_DatAccess_SetVal(	subs(1),
     &		    				subs(2),
     &		    				subs(3),
     &		    				subs(4),
     &		    				dataType,
     &		    				dataLen, 
     &		    				intVal, 
     &		    				realVal,
     &		    				charVal, 
     &		    				dateVal )

	            tst_tstWcom_VALentered = .true.
		    aeqSsp_autoTbl_hasEnteredData = .true.

		    ! Update tst_tstWcom_curResNum
	            call gut_indices_ArrayToInt(	test.numDims,
     &	    						tres.dimNumValues,
     &	    						tst_tstWcom_curResNum,
     &	    						subs )

		    ! Get indexes for next item to enter.
		    ! Finish if no more data to enter
		    call aeqSsp_autoMan_next(	.not. FIRSTTIME, 
     &						aeqSsp_autoTbl_waferIndex,
     &						aeqSsp_autoTbl_siteIndex,
     &						aeqSsp_autoTbl_itemIndex,
     &						aeqSsp_autoTbl_state )

	          else 

		    statusText =	'% Rejected data value "'//
     &					value(j,i)(:valueLen(j,i))//'"'
		    status = .false.

	          endif

	        else

		  status = .false.     
  		  if ( .not. okWafer ) then
		    statusText = '% WAFERID not set'
		  elseif ( .not. okSite ) then
		    statusText = '% SITEID not set'
		  elseif ( .not. okItem ) then
		    statusText = '% ITEMID not set'
		  endif	

		endif

		j = j + 1

	      enddo ! Values

	    endif	

	  elseif ( token(i) .ne. 'STATUS' ) then

	    ! Unrecognized token
	    status = .false.
	    statusText = '% Undefined token '//token(i)

	  endif
	
	  i = i + 1

	enddo ! All tokens

	aeqSsp_autoMan_Data = status

	return

	end

!++ AEQSSP_AUTOMAN_SETUP
!	
	logical*4 function aeqSsp_automan_SETUP (	mode,
     &							token, 
     &							tokenCount, 
     &							value, 
     &							valueLen, 
     &							valueCount,
     &							statusText,
     &							replyToken, 
     &							replyTokenCount,
     &							replyValue,
     &							replyValueLen,
     &							replyValueCount )

	implicit none

! 	This routine processes QUERY and EVENT requests for the SETUP method

! Include files:

	include	'FIL:TYPDEF'
	include	'FIL:FILDEF'
	include	'GUT:GUTDEF'
	include	'TUT:TUTDEF'
	include	'TST:TSTDEF'	
	include	'TST:ENTDEF'	
	include	'TST:PARMDEF'	
	include 'LOT:FUNDEF'
	include 'PAR:LOTSTDEF'
	include 'PAR:PRODSTDEF'
	include	'LOT:ACTLBUFF'	
	include	'FIL:TESTCDEF'	
	include	'FIL:TRESCDEF'	
	include	'TST:TSTWCOM'	

	include	'AEQ_SSP:AUTODEF'
	include	'AEQ_SSP:AUTOTBL'

! Arguments:

	character*(*)			mode				! [R]
	! - mode (QUERY, REPLY, or EVENT)

	character*(*)			token( MAX_TOKENS)		! [R]
	! - tokens in message

	integer*4			tokenCount			! [R]
	! - # of tokens

	character*(*)			value( MAX_VALUES, MAX_TOKENS)	! [R]
	! - values of tokens

	integer*4			valueLen( MAX_VALUES, MAX_TOKENS)! [R]
	! - length of values of tokens

	integer*4			valueCount( MAX_TOKENS )	! [R]
	! - # of values per tokens

	character*(*)			statusText			! [W]
	! - reply status text

	character*(*)			replyToken( MAX_TOKENS)		! [W]
	! - tokens in reply message

	integer*4			replyTokenCount			! [W]
	! - # of reply tokens

	character*(*)			replyValue( MAX_VALUES, MAX_TOKENS) ![W]
	! - values of reply tokens

	integer*4			replyValueLen(MAX_VALUES,MAX_TOKENS)![W]
	! - length of values of reply tokens

	integer*4			replyValueCount( MAX_TOKENS )	! [W]
	! - # of values per reply tokens

! Routines Used:

	integer*4	gut_trimblks
	integer*4	aeqSsp_autoUtil_left_justify
	logical*4	aeqSsp_autoMan_getParm
	logical*4	aeqSsp_autoMan_getTRfield

! Author:	Mike Bergsma		7/92
!		Ab Initio Software
!
! Modifications:
!
!	9/20/94	Mike Bergsma
!	- remove recursive switch to 'aeqssp_automan_getParm' because
!	  it has been rewritten to know whether to do recursion.
!
!	Mike Bergsma		12/22/94
!	- add support for $file.field database variables
!
! Local variables:

	logical*4	status

	integer*4	i,j,k,
     &			dataLen,
     &			ios,
     &			statusTextLen

	character*(TYP__PARMNMSZ)	parmName
	character*(TYP__PARMVALSZ)	parmVal
	character*(1)			parmType
	character*(VALUE_SIZE)		charVal

	status = .true.
	statusText = '$ Acknowledged'

	i = 1
	do while ( i .le. tokenCount .and. status )

	  if ( valueCount(i) .eq. 0 .and. mode .eq. 'QUERY' .and.
     &	       replyTokenCount .eq. MAX_TOKENS - 1 ) then
	    status = .false.
	    statusText = '% MAX_TOKENS exceeded in AUTODEF.FOR.'

	  else if ( token(i)(1:1) .eq. '$' ) then

	    if ( INDEX ( token(i), '.' ) .eq. 6 ) then
	    
	      if ( valueCount(i) .eq. 0 .and. mode .eq. 'QUERY' ) then

	        ! Target wants current value of database field
	        status = aeqSsp_autoMan_getTRfield(
     &					token(i),
     &			        	charVal,
     &					dataLen,
     &					statusText,
     &					statusTextLen )

	        if ( status ) then

		  replyTokenCount = replyTokenCount + 1
		  replyToken(replyTokenCount) = token(i)
		  replyValueCount(replyTokenCount) = 1
		  replyValue(1,replyTokenCount) = charVal
		  replyValueLen(1,replyTokenCount) = dataLen
		
	        endif

	      else

	        ! Target wants to set database field
	        status = .false.
	        statusText = '% Access violation. Cannot set database field'

	      endif

	    else

	      ! Must be parameter field.
	      if ( valueCount(i) .eq. 0 .and. mode .eq. 'QUERY' ) then

	        ! Target wants current value of parameter
	        parmName = token(i)
	        if ( aeqSsp_autoMan_getParm(	parmName,
     &			        		aeqSsp_autoTbl_parmSource,
     &						parmVal ) ) then

		  replyTokenCount = replyTokenCount + 1
		  replyToken(replyTokenCount) = parmName
		  replyValueCount(replyTokenCount) = 1
		  replyValue(1,replyTokenCount) = parmVal
		  replyValueLen(1,replyTokenCount) = 
     &			gut_trimblks( replyValue(1,replyTokenCount) )

	        else

	          statusText = '% Undefined parameter '//parmName
		  status = .false.
		
	        endif

	      elseif ( valueCount(i) .eq. 1 .or. valueCount(i) .eq. 2 ) then

		if ( aeqSsp_autoTbl_hasLotid ) then

		  parmName = token(i)
		  parmVal = value(1,i)(:valueLen(1,i))

	          if ( valueCount(i) .eq. 2 ) then

		    parmType = value(2,i)
		    if ( parmType .ne. PAR_PRODSTDEF_PTYPE_STRING .and.
     &		         parmType .ne. PAR_PRODSTDEF_PTYPE_REAL4 .and.
     &		         parmType .ne. PAR_PRODSTDEF_PTYPE_INTEGER .and.
     &		         parmType .ne. PAR_PRODSTDEF_PTYPE_PART .and.
     &		         parmType .ne. PAR_PRODSTDEF_PTYPE_DCOPER ) 
     &		      parmType = PAR_PRODSTDEF_PTYPE_STRING

		  else

		    parmType = PAR_PRODSTDEF_PTYPE_STRING

		  endif

                  call Tst_ActlUtil_PutSetParm(	actl, 
     &		  				parmName, 
     &		  				parmVal, 
     &		  				parmType, 
     &		  				.true. )
		else

	          statusText = '% Cannot set parameter for non lot tests' 
		  status = .false.

		endif
	      endif
	    endif

	  elseif ( token(i) .eq. 'NUMWAFERS' ) then

	    if ( valueCount(i) .eq. 0 .and. mode .eq. 'QUERY' ) then

	      ! Target wants value of NUMWAFERS
	      if ( aeqSsp_autoTbl_hasLotid ) then

		replyTokenCount = replyTokenCount + 1
		replyToken(replyTokenCount) = token(i)
		replyValueCount(replyTokenCount) = 1
	        k = INT ( actl.curMainQty ) ! Convert real to integer
		write( replyValue(1,replyTokenCount), '(I2)', iostat=ios ) k
     		replyValueLen(1,replyTokenCount) = 
     &		  aeqSsp_autoUtil_left_justify( replyValue(1,replyTokenCount) )

	      else

		status = .false.
		statusText = '% WAFERS do not exist'

	      endif
		
	    else

	      ! Target wants to set NUMWAFERS
	      status = .false.
	      statusText = '% Access violation. Cannot set NUMWAFERS'

	    endif

	  elseif ( token(i) .eq. 'WAFERS' ) then

	    if ( valueCount(i) .eq. 0 .and. mode .eq. 'QUERY' ) then

	      if ( aeqSsp_autoTbl_hasLotid ) then

		! Target wants list of all WAFERS
		replyTokenCount = replyTokenCount + 1
		replyToken(replyTokenCount) = token(i)

		k = 0
		do j=1,actl.compCount
		  if ( actl.compState(j) .eq. 1 ) then
		    k = k + 1
	            replyValue(k,replyTokenCount) = actl.compIds(j)
		    replyValueLen(k,replyTokenCount) = 
     &			gut_trimblks( replyValue(k,replyTokenCount) )
		  endif
		enddo
		replyValueCount(replyTokenCount) = k

	      else

		status = .false.
		statusText = '% WAFERS do not exist'

	      endif
		
	    else

	      ! Target wants to set WAFERS
	      status = .false.
	      statusText = '% Access violation. Cannot set WAFERS'

	    endif

	  elseif ( token(i) .eq. 'USERID' ) then

	    if ( valueCount(i) .eq. 0 .and. mode .eq. 'QUERY' ) then

	      ! Target wants value of USERID
	      if ( aeqSsp_autoTbl_hasLotid ) then
               
		replyTokenCount = replyTokenCount + 1
		replyToken(replyTokenCount) = token(i)
		replyValueCount(replyTokenCount) = 1
		replyValue(1,replyTokenCount) = actl.empIdTrackIn
		replyValueLen(1,replyTokenCount) = 
     &			gut_trimblks( replyValue(1,replyTokenCount) )

	      else

		status = .false.
		statusText = '% USERID does not exist'

	      endif
		
	    else

	      ! Target wants to set USERID
	      status = .false.
	      statusText = '% Access violation. Cannot set USERID'

	    endif

	  elseif ( token(i) .eq. 'AUTOTYPE' ) then

	    if ( valueCount(i) .eq. 0 .and. mode .eq. 'QUERY' ) then

	      ! Target wants value of AUTOTYPE
	      replyTokenCount = replyTokenCount + 1
	      replyToken(replyTokenCount) = token(i)
	      replyValueCount(replyTokenCount) = 1
	      replyValue(1,replyTokenCount) = aeqSsp_autoTbl_autoType 
	      replyValueLen(1,replyTokenCount) = 
     &			gut_trimblks( replyValue(1,replyTokenCount) )
		
	    else

	      ! Target wants to set AUTOTYPE
	      status = .false.
	      statusText = '% Access violation. Cannot set AUTOTYPE'

	    endif

	  elseif ( token(i) .ne. 'STATUS' ) then

	    ! Unrecognized token
	    status = .false.
	    statusText = '% Undefined token '//token(i)

	  endif
	
	  i = i + 1

	enddo 

	aeqSsp_autoMan_SETUP = status

	return

	end

!++ AEQSSP_AUTOMAN_CONNECT
!	
	logical*4 function aeqSsp_automan_CONNECT (	mode,
     &							token, 
     &							tokenCount, 
     &							value, 
     &							valueLen, 
     &							valueCount,
     &							statusText,
     &							replyToken, 
     &							replyTokenCount,
     &							replyValue,
     &							replyValueLen,
     &							replyValueCount )

	implicit none

! 	This routine processes QUERY and EVENT requests for the CONNECT method

! Include files:

	include	'FIL:TYPDEF'
	include	'FIL:FILDEF'
	include	'GUT:GUTDEF'
	include	'TUT:TUTDEF'
	include	'TST:TSTDEF'	
	include	'TST:ENTDEF'	
	include	'TST:PARMDEF'	
	include 'LOT:FUNDEF'
	include 'PAR:LOTSTDEF'
	include 'PAR:PRODSTDEF'
	include	'LOT:ACTLBUFF'	
	include	'FIL:TESTCDEF'	
	include	'FIL:TRESCDEF'	
	include	'TST:TSTWCOM'	

	include	'AEQ_SSP:AUTODEF'
	include	'AEQ_SSP:AUTOTBL'

! Arguments:

	character*(*)			mode				! [R]
	! - mode (QUERY, REPLY, or EVENT)

	character*(*)			token( MAX_TOKENS)		! [R]
	! - tokens in message

	integer*4			tokenCount			! [R]
	! - # of tokens

	character*(*)			value( MAX_VALUES, MAX_TOKENS)	! [R]
	! - values of tokens

	integer*4			valueLen( MAX_VALUES, MAX_TOKENS)! [R]
	! - length of values of tokens

	integer*4			valueCount( MAX_TOKENS )	! [R]
	! - # of values per tokens

	character*(*)			statusText			! [W]
	! - reply status text

	character*(*)			replyToken( MAX_TOKENS)		! [W]
	! - tokens in reply message

	integer*4			replyTokenCount			! [W]
	! - # of reply tokens

	character*(*)			replyValue( MAX_VALUES, MAX_TOKENS) ![W]
	! - values of reply tokens

	integer*4			replyValueLen(MAX_VALUES,MAX_TOKENS)![W]
	! - length of values of reply tokens

	integer*4			replyValueCount( MAX_TOKENS )	! [W]
	! - # of values per reply tokens

! Routines Used:

	integer*4	gut_trimblks
	character*12	uaf_user_getName

! Author:	Mike Bergsma		7/92
!		Ab Initio Software
!
! Modifications:
!
!	Mike Bergsma		5/10/94
!	- support for USERID
!
!	Mike Bergsma	2/26/97
!	- add AUTOTYPE token
!
! Local variables:

	logical*4	status
	integer*4	i

	status = .true.
	statusText = '$ Acknowledged'

	i = 1
	do while ( i .le. tokenCount .and. status )

	  if ( valueCount(i) .eq. 0 .and. mode .eq. 'QUERY' .and.
     &	       replyTokenCount .eq. MAX_TOKENS - 1 ) then
	    status = .false.
	    statusText = '% MAX_TOKENS exceeded in AUTODEF.FOR.'

	  else if ( token(i) .eq. 'LOTID' ) then

	    if ( valueCount(i) .eq. 0 .and. mode .eq. 'QUERY' ) then

	      ! Target wants value of LOTID
	      replyTokenCount = replyTokenCount + 1
	      replyToken(replyTokenCount) = token(i)
	      replyValueCount(replyTokenCount) = 1
	      replyValue(1,replyTokenCount) = tres.lotid
	      replyValueLen(1,replyTokenCount) = 
     &			gut_trimblks( replyValue(1,replyTokenCount) )
		
	    else

	      ! Target wants to set LOTID.   
	      status = .false.
	      statusText = '% Access violation. Cannot set LOTID'

	    endif

	  elseif ( token(i) .eq. 'EQPID' ) then

	    if ( valueCount(i) .eq. 0 .and. mode .eq. 'QUERY' ) then

	      ! Target wants value of EQPID
	      replyTokenCount = replyTokenCount + 1
	      replyToken(replyTokenCount) = token(i)
	      replyValueCount(replyTokenCount) = 1
	      replyValue(1,replyTokenCount) = tres.eqpid
	      replyValueLen(1,replyTokenCount) = 
     &			gut_trimblks( replyValue(1,replyTokenCount) )
		
	    else

	      ! Target wants to set EQPID
	      status = .false.
	      statusText = '% Access violation. Cannot set EQPID'

	    endif

	  elseif ( token(i) .eq. 'USERID' ) then

	    if ( valueCount(i) .eq. 0 .and. mode .eq. 'QUERY' ) then

	      ! Target wants value of USERID
	      replyTokenCount = replyTokenCount + 1
	      replyToken(replyTokenCount) = token(i)
	      replyValueCount(replyTokenCount) = 1
	      replyValue(1,replyTokenCount) = uaf_user_getName()
	      replyValueLen(1,replyTokenCount) = 
     &			gut_trimblks( replyValue(1,replyTokenCount) )
		
	    else

	      ! Target wants to set USERID
	      status = .false.
	      statusText = '% Access violation. Cannot set USERID'

	    endif

	  elseif ( token(i) .eq. 'AUTOTYPE' ) then

	    if ( valueCount(i) .eq. 0 .and. mode .eq. 'QUERY' ) then

	      ! Target wants value of AUTOTYPE
	      replyTokenCount = replyTokenCount + 1
	      replyToken(replyTokenCount) = token(i)
	      replyValueCount(replyTokenCount) = 1
	      replyValue(1,replyTokenCount) = aeqSsp_autoTbl_autoType 
	      replyValueLen(1,replyTokenCount) = 
     &			gut_trimblks( replyValue(1,replyTokenCount) )
		
	    else

	      ! Target wants to set AUTOTYPE
	      status = .false.
	      statusText = '% Access violation. Cannot set AUTOTYPE'

	    endif

	  elseif ( token(i) .ne. 'STATUS' ) then

	    ! Unrecognized token
	    status = .false.
	    statusText = '% Undefined token '//token(i)

	  endif
	
	  i = i + 1

	enddo 

	aeqSsp_autoMan_CONNECT = status

	return

	end

!++ AEQSSP_AUTOMAN_DISCONNECT
!	
	logical*4 function aeqSsp_automan_DISCONNECT (	mode,
     &							token, 
     &							tokenCount, 
     &							value, 
     &							valueLen, 
     &							valueCount,
     &							statusText,
     &							replyToken, 
     &							replyTokenCount,
     &							replyValue,
     &							replyValueLen,
     &							replyValueCount )

	implicit none

! 	This routine processes QUERY and EVENT requests for the 
!	DISCONNECT and ABORT methods

! Include files:

	include	'FIL:TYPDEF'
	include	'FIL:FILDEF'
	include	'GUT:GUTDEF'
	include	'TUT:TUTDEF'
	include	'TST:TSTDEF'	
	include	'TST:ENTDEF'	
	include	'TST:PARMDEF'	
	include 'LOT:FUNDEF'
	include 'PAR:LOTSTDEF'
	include 'PAR:PRODSTDEF'
	include	'LOT:ACTLBUFF'	
	include	'FIL:TESTCDEF'	
	include	'FIL:TRESCDEF'	

	include	'AEQ_SSP:AUTODEF'
	include	'AEQ_SSP:AUTOTBL'

! Arguments:

	character*(*)			mode				! [R]
	! - mode (QUERY, REPLY, or EVENT)

	character*(*)			token( MAX_TOKENS)		! [R]
	! - tokens in message

	integer*4			tokenCount			! [R]
	! - # of tokens

	character*(*)			value( MAX_VALUES, MAX_TOKENS)	! [R]
	! - values of tokens

	integer*4			valueLen( MAX_VALUES, MAX_TOKENS)! [R]
	! - length of values of tokens

	integer*4			valueCount( MAX_TOKENS )	! [R]
	! - # of values per tokens

	character*(*)			statusText			! [W]
	! - reply status text

	character*(*)			replyToken( MAX_TOKENS)		! [W]
	! - tokens in reply message

	integer*4			replyTokenCount			! [W]
	! - # of reply tokens

	character*(*)			replyValue( MAX_VALUES, MAX_TOKENS) ![W]
	! - values of reply tokens

	integer*4			replyValueLen(MAX_VALUES,MAX_TOKENS)![W]
	! - length of values of reply tokens

	integer*4			replyValueCount( MAX_TOKENS )	! [W]
	! - # of values per reply tokens

! Routines Used:

	logical*4	aeqSsp_autoMan_getTRfield

! Author:	Mike Bergsma		7/92
!		Ab Initio Software
!
! Modifications:
!
!	Mike Bergsma		12/22/94
!	- add support for $file.field database variables
!
!	Mike Bergsma		7/12/95
!	- after sending DISCONNECT or ABORT, close the output channel
!
!	Mike Bergsma		10/30/95
!	- no longer send DISCONNECT or ABORT.
!
! Local variables:

	logical*4	status
	integer*4	i,j,k,
     &			dataLen,
     &			statusTextLen

	character*(MAX_MESSAGE_LEN)	text

	character*(TYP__PARMNMSZ)	parmName
	character*(TYP__PARMVALSZ)	parmVal
	character*(1)			parmType
	character*(VALUE_SIZE)		charVal

	status = .true.
	statusText = '$ Acknowledged'

	i = 1
	do while ( i .le. tokenCount .and. status )

	  if ( valueCount(i) .eq. 0 .and. mode .eq. 'QUERY' .and.
     &	       replyTokenCount .eq. MAX_TOKENS - 1 ) then
	    status = .false.
	    statusText = '% MAX_TOKENS exceeded in AUTODEF.FOR.'

	  else if ( token(i) .eq. 'PAGEWAIT' ) then

	    if ( valueCount(i) .eq. 0 .and. mode .eq. 'QUERY' ) then

	      ! Target wants current value of PAGEWAIT
	      replyTokenCount = replyTokenCount + 1
	      replyToken(replyTokenCount) = token(i)

	      replyValueCount(replyTokenCount) = 1

	      if ( aeqSsp_autoTbl_pageWait ) then
		replyValue(1,replyTokenCount) = 'YES'
		replyValueLen(1,replyTokenCount) = 3
	      else
		replyValue(1,replyTokenCount) = 'NO'
		replyValueLen(1,replyTokenCount) = 2
	      endif
		
	    elseif ( valueCount(i) .eq. 0 ) then

	      statusText = '% PAGEWAIT value not specified'
	      status = .false.

	    elseif ( valueCount(i) .eq. 1 ) then

	      ! Target wants to set PAGEWAIT
	      if ( value(1,i)(:valueLen(1,i)) .eq. 'NO' ) then
	        aeqSsp_autoTbl_pageWait = .false.
	      elseif ( value(1,i)(:valueLen(1,i)) .eq. 'YES' ) then
		aeqSsp_autoTbl_pageWait = .true.
	      else
		statusText = '% PAGEWAIT value must be YES or NO'
		status = .false.
	      endif

	    else

	      statusText = '% Too many PAGEWAIT values'
	      status = .false.

	    endif

	  elseif ( token(i) .eq. 'TEXT' .or. 
     &		   token(i) .eq. 'TEXT2' .or. 
     &		   token(i) .eq. 'TEXT3' ) then

	    if ( valueCount(i) .gt. 0 ) then

	      ! Target wants to display text

	      k = 0
	      j = 1
	      text = ' '
	      do while ( j .le. valueCount(i) .and. k .lt. MAX_MESSAGE_LEN )

		text(k+1:)= value(j,i)(:valueLen(j,i))
		k = k + valueLen(j,i)
		j = j + 1

	      enddo

	      if ( token(i) .eq. 'TEXT' ) then
	        call aeqSsp_autoMan_outMessage( text, .false. )
	      elseif ( token(i) .eq. 'TEXT2' ) then
	        call aeqSsp_autoMan_outMessage( text, .true. )
	      else
		aeqSsp_autoTbl_text3 = text
	      endif	

	    endif

	  elseif ( token(i)(1:1) .eq. '$' ) then

	    if ( INDEX ( token(i), '.' ) .eq. 6 ) then
	    
	      if ( valueCount(i) .eq. 0 .and. mode .eq. 'QUERY' ) then

	        ! Target wants current value of database field
	        status = aeqSsp_autoMan_getTRfield(
     &					token(i),
     &			        	charVal,
     &					dataLen,
     &					statusText,
     &					statusTextLen )

	        if ( status ) then

		  replyTokenCount = replyTokenCount + 1
		  replyToken(replyTokenCount) = token(i)
		  replyValueCount(replyTokenCount) = 1
		  replyValue(1,replyTokenCount) = charVal
		  replyValueLen(1,replyTokenCount) = dataLen
		
	        endif

	      else

	        ! Target wants to set database field
	        status = .false.
	        statusText = '% Access violation. Cannot set database field'

	      endif

	    else

	      ! Must be parameter field.
	      if ( valueCount(i) .eq. 0 .and. mode .eq. 'QUERY' ) then

	        ! Target wants current value of parameter
	        status = .false.
	        statusText = '% Access violation. Parameter can be set only'
		
	      elseif ( valueCount(i) .eq. 0 ) then

	        statusText = '% Parameter value not specified'
	        status = .false.

	      elseif ( valueCount(i) .eq. 1 .or. valueCount(i) .eq. 2 ) then

	        if ( aeqSsp_autoTbl_hasLotid ) then

		  parmName = token(i)
		  parmVal = value(1,i)(:valueLen(1,i))

	          if ( valueCount(i) .eq. 2 ) then

		    parmType = value(2,i)
		    if ( parmType .ne. PAR_PRODSTDEF_PTYPE_STRING .and.
     &		         parmType .ne. PAR_PRODSTDEF_PTYPE_REAL4 .and.
     &		         parmType .ne. PAR_PRODSTDEF_PTYPE_INTEGER .and.
     &		         parmType .ne. PAR_PRODSTDEF_PTYPE_PART .and.
     &		         parmType .ne. PAR_PRODSTDEF_PTYPE_DCOPER ) 
     &		      parmType = PAR_PRODSTDEF_PTYPE_STRING

		  else

		    parmType = PAR_PRODSTDEF_PTYPE_STRING

		  endif

                  call Tst_ActlUtil_PutSetParm(	actl, 
     &						parmName, 
     &						parmVal, 
     &						parmType, 
     &						.true. )

	        else

	          statusText = '% Cannot set parameter for non lot tests' 
		  status = .false.

	        endif

	      endif
	    endif

	  elseif ( token(i) .ne. 'STATUS' ) then

	    ! Unrecognized token
	    status = .false.
	    statusText = '% Undefined token '//token(i)

	  endif
	
	  i = i + 1

	enddo 

	aeqSsp_autoMan_DISCONNECT = status

	return

	end

!++ AEQSSP_AUTOMAN_MESSAGE
!	
	logical*4 function aeqSsp_automan_MESSAGE (
     &						mode,
     &						token, 
     &						tokenCount, 
     &						value, 
     &						valueLen, 
     &						valueCount,
     &						statusText,
     &						replyToken, 
     &						replyTokenCount,
     &						replyValue,
     &						replyValueLen,
     &						replyValueCount )

	implicit none

! 	This routine processes QUERY and EVENT requests for the MESSAGE method

! Include files:

	include	'AEQ_SSP:AUTODEF'
	include	'AEQ_SSP:AUTOTBL'

! Arguments:

	character*(*)			mode				! [R]
	! - mode (QUERY, REPLY, or EVENT)

	character*(*)			token( MAX_TOKENS)		! [R]
	! - tokens in message

	integer*4			tokenCount			! [R]
	! - # of tokens

	character*(*)			value( MAX_VALUES, MAX_TOKENS)	! [R]
	! - values of tokens

	integer*4			valueLen( MAX_VALUES, MAX_TOKENS)! [R]
	! - length of values of tokens

	integer*4			valueCount( MAX_TOKENS )	! [R]
	! - # of values per tokens

	character*(*)			statusText			! [W]
	! - reply status text

	character*(*)			replyToken( MAX_TOKENS)		! [W]
	! - tokens in reply message

	integer*4			replyTokenCount			! [W]
	! - # of reply tokens

	character*(*)			replyValue( MAX_VALUES, MAX_TOKENS) ![W]
	! - values of reply tokens

	integer*4			replyValueLen(MAX_VALUES,MAX_TOKENS)![W]
	! - length of values of reply tokens

	integer*4			replyValueCount( MAX_TOKENS )	! [W]
	! - # of values per reply tokens

! Routines Used:

	integer*4	gut_getstr 
	integer*4	tut_getnoecho

! Author:	Mike Bergsma		7/92
!		Ab Initio Software
!
! Modifications:
!
!	Mike Bergsma	7/15/96
!	- add ECHO token
!
!	Mike Bergsma	11/19/96
!	- added support for PAGEWAIT, as so many MESSAGE messages have it.
!
! Local variables:

	logical*4	status,
     &			echo
	integer*4	i,j,k,
     &			promptReplyLen

	character*(MAX_MESSAGE_LEN)	text
	character*(VALUE_SIZE)		promptReply


	status = .true.
	statusText = '$ Acknowledged'
	echo = .true.

	i = 1
	do while ( i .le. tokenCount .and. status )

	  if ( valueCount(i) .eq. 0 .and. mode .eq. 'QUERY' .and.
     &	       replyTokenCount .eq. MAX_TOKENS - 1 ) then
	    status = .false.
	    statusText = '% MAX_TOKENS exceeded in AUTODEF.FOR.'

	  elseif ( token(i) .eq. 'TEXT' .or. 
     &		   token(i) .eq. 'TEXT2' .or. 
     &		   token(i) .eq. 'TEXT3' .or.
     &		   token(i) .eq. 'STATUS' ) then

	    if ( valueCount(i) .gt. 0 ) then

	      ! Target wants to display text

	      k = 0
	      j = 1
	      text = ' '
	      do while ( j .le. valueCount(i) .and. k .lt. MAX_MESSAGE_LEN )

		text(k+1:)= value(j,i)(:valueLen(j,i))
		k = k + valueLen(j,i)
		j = j + 1

	      enddo

	      if ( token(i) .eq. 'TEXT' ) then
	        call aeqSsp_autoMan_outMessage( text, .false. )
	      elseif ( token(i) .eq. 'TEXT2' .or. token(i) .eq. 'STATUS' ) then
	        call aeqSsp_autoMan_outMessage( text, .true. )
	      else
		aeqSsp_autoTbl_text3 = text
	      endif	

	    endif

	  elseif ( token(i) .eq. 'ECHO' ) then

	    if ( valueCount(i) .eq. 1 ) then
		
	      ! Target wants to set ECHO
	      if ( value(1,i)(:valueLen(1,i)) .eq. 'NO' ) then
	        echo = .false.
	      elseif ( value(1,i)(:valueLen(1,i)) .eq. 'YES' ) then
		echo = .true.
	      else
		statusText = '% ECHO value must be YES or NO'
		status = .false.
	      endif

	    else

	      statusText = '% ECHO value must be YES or NO'
	      status = .false.

	    endif

	  elseif ( token(i) .eq. 'PROMPT' ) then

	    if ( valueCount(i) .eq. 0 .and. mode .eq. 'QUERY' ) then

	      ! Target wants value of PROMPT.

	      ! (Re-)enable AST's, in case we get a message while in the prompt.
	      call sys$setast( %val(1) )

	      ! Get the prompt
	      if ( echo ) then	
	        promptReplyLen = gut_getstr ( '> ', promptReply, 0 )
	      else	
	        call tut_getnoecho ( '> ', promptReply, promptReplyLen )
	      endif
	
	      ! Disable AST's
	      call sys$setast( %val(0) )

	      call tut_output ( ' ' )

	      if ( aeqSsp_autoTbl_msgPending ) then

		status = .false.
	        statusText = '% Message interrupted query'

	      else

	        if ( promptReplyLen .eq. 0 ) then
		  promptReply = ' '
		  promptReplyLen = 1
	        endif		 
		
	        replyTokenCount = replyTokenCount + 1
	        replyToken(replyTokenCount) = token(i)
	        replyValueCount(replyTokenCount) = 1
	        replyValue(1,replyTokenCount) = promptReply
	        replyValueLen(1,replyTokenCount) = promptReplyLen
		
	      endif	
	    else

	      ! Target wants to set PROMPT
	      status = .false.
	      statusText = '% Access violation. Cannot set PROMPT'

	    endif

	  elseif ( token(i) .eq. 'PAGEWAIT' ) then

	    if ( valueCount(i) .eq. 0 .and. mode .eq. 'QUERY' ) then

	      ! Target wants current value of PAGEWAIT
	      replyTokenCount = replyTokenCount + 1
	      replyToken(replyTokenCount) = token(i)

	      replyValueCount(replyTokenCount) = 1

	      if ( aeqSsp_autoTbl_pageWait ) then
		replyValue(1,replyTokenCount) = 'YES'
		replyValueLen(1,replyTokenCount) = 3
	      else
		replyValue(1,replyTokenCount) = 'NO'
		replyValueLen(1,replyTokenCount) = 2
	      endif
		
	    elseif ( valueCount(i) .eq. 0 ) then

	      statusText = '% PAGEWAIT value not specified'
	      status = .false.

	    elseif ( valueCount(i) .eq. 1 ) then

	      ! Target wants to set PAGEWAIT
	      if ( value(1,i)(:valueLen(1,i)) .eq. 'NO' ) then
	        aeqSsp_autoTbl_pageWait = .false.
	      elseif ( value(1,i)(:valueLen(1,i)) .eq. 'YES' ) then
		aeqSsp_autoTbl_pageWait = .true.
	      else
		statusText = '% PAGEWAIT value must be YES or NO'
		status = .false.
	      endif

	    else

	      statusText = '% Too many PAGEWAIT values'
	      status = .false.

	    endif

	  elseif ( token(i) .ne. 'STATUS' ) then

	    ! Unrecognized token
	    status = .false.
	    statusText = '% Undefined token '//token(i)

	  endif
	
	  i = i + 1

	enddo 

	aeqSsp_autoMan_MESSAGE = status

	return

	end

!++ AEQSSP_AUTOMAN_HOTKEY
!	
	logical*4 function aeqSsp_automan_HOTKEY (
     &						mode,
     &						token, 
     &						tokenCount, 
     &						value, 
     &						valueLen, 
     &						valueCount,
     &						statusText,
     &						replyToken, 
     &						replyTokenCount,
     &						replyValue,
     &						replyValueLen,
     &						replyValueCount )

	implicit none

! 	This routine processes QUERY and EVENT requests for the HOTKEY method

! Include files:

	include	'FIL:TYPDEF'
	include	'FIL:FILDEF'
	include	'GUT:GUTDEF'
	include	'TUT:TUTDEF'

	include	'AEQ_SSP:AUTODEF'
	include	'AEQ_SSP:AUTOTBL'

! Arguments:

	character*(*)			mode				! [R]
	! - mode (QUERY, REPLY, or EVENT)

	character*(*)			token( MAX_TOKENS)		! [R]
	! - tokens in message

	integer*4			tokenCount			! [R]
	! - # of tokens

	character*(*)			value( MAX_VALUES, MAX_TOKENS)	! [R]
	! - values of tokens

	integer*4			valueLen( MAX_VALUES, MAX_TOKENS)! [R]
	! - length of values of tokens

	integer*4			valueCount( MAX_TOKENS )	! [R]
	! - # of values per tokens

	character*(*)			statusText			! [W]
	! - reply status text

	character*(*)			replyToken( MAX_TOKENS)		! [W]
	! - tokens in reply message

	integer*4			replyTokenCount			! [W]
	! - # of reply tokens

	character*(*)			replyValue( MAX_VALUES, MAX_TOKENS) ![W]
	! - values of reply tokens

	integer*4			replyValueLen(MAX_VALUES,MAX_TOKENS)![W]
	! - length of values of reply tokens

	integer*4			replyValueCount( MAX_TOKENS )	! [W]
	! - # of values per reply tokens

! Routines Used:

	logical*4	gut_editint 

! Author:	Mike Bergsma		7/92
!		Ab Initio Software
!
! Modifications:
!
! Local variables:

	logical*4	status,
     &			isEnabled
	integer*4	i,j,
     &			key,
     &			position,
     &			valueIndex
	logical*4	moreKeys
	character*1	keyChar
	character*8	keyName
	character*3	answer

	character*(METHOD_SIZE)		hotMethod

	status = .true.
	statusText = '$ Acknowledged'

	! Key defintions can be done in parallel as well as sequentially.
	! First scan the entire set of tokens, element 1.
	! Then scan again, looking for KEY element 2, 3, and so on.
	! Not all token arrays are necessarily of equal size.  The
	! KEY token array determines the actual number of keys 
	! processed.  
	
	valueIndex = 1
	moreKeys = .true.
	do while ( moreKeys .and. status )

	  moreKeys = .false.
	  key = 0
	  i = 1

	  do while ( i .le. tokenCount .and. status )

	    if ( valueCount(i) .eq. 0 .and. mode .eq. 'QUERY' .and.
     &	       replyTokenCount .eq. MAX_TOKENS - 1 ) then
	      status = .false.
	      statusText = '% MAX_TOKENS exceeded in AUTODEF.FOR.'

	    else if ( token(i) .eq. 'KEY' ) then

	      if ( valueIndex .le. valueCount(i) ) then

		moreKeys = .true.

	        ! Get key value.
	        keyName = value(valueIndex,i)(:valueLen(valueIndex,i))
	        keyChar = keyName(1:1)
	        call gut_uppercase ( keyChar )	
	        if ( keyChar .eq. '^' ) then

		  ! Control character
		  key = ICHAR ( keyName(2:2) ) - 64
		  if ( key .ge. 1 .and. key .le. 31 ) then

		    aeqSsp_autoTbl_hotKey(key).keyName = keyName

		    ! Update
	    	    call aeqSsp_autoMan_outHotKey ( 
     &			aeqSsp_autoTbl_hotKey(key).position,
     &			aeqSsp_autoTbl_hotKey(key).keyName,
     &			aeqSsp_autoTbl_hotKey(key).isEnabled )

		  else
		    key = 0
		    statusText = '% Invalid control character specified'
		    status = .false.
		  endif

	        elseif (( keyChar .ge. 'A' .and. keyChar .le. 'Z' ) .or. 
     &			( keyChar .ge. '0' .and. keyChar .le. '9' ) .or. 
     &			( keyChar .eq. ' ' ) ) then

		  ! Regular hot key
		  key = ICHAR ( keyChar ) 
		  aeqSsp_autoTbl_hotKey(key).keyName = keyName

		  ! Check for spacebar/esc/return/linefeed combo
		  if ( key .eq. 32 ) then
		    aeqSsp_autoTbl_hotKey(27).keyName = keyName
		    aeqSsp_autoTbl_hotKey(13).keyName = keyName
		    aeqSsp_autoTbl_hotKey(10).keyName = keyName
		  endif

		  ! Update
	    	  call aeqSsp_autoMan_outHotKey ( 
     &			aeqSsp_autoTbl_hotKey(key).position,
     &			aeqSsp_autoTbl_hotKey(key).keyName,
     &			aeqSsp_autoTbl_hotKey(key).isEnabled )

	        else

		  key = 0
		  statusText = '% Invalid hot key specified'
		  status = .false.

	        endif	

	      endif

	    elseif ( token(i) .eq. 'METHOD' ) then

	      if ( key .ne. 0 ) then

	        if ( valueIndex .le. valueCount(i) ) then

	          hotMethod = value(valueIndex,i)(:valueLen(valueIndex,i))
	
	          ! Make uppercase
	          call gut_uppercase ( hotMethod )	

	          ! Strip out illegal characters.
	          call aeqSsp_autoUtil_strip( hotMethod )

	          aeqSsp_autoTbl_hotKey(key).method = hotMethod

		  ! Check for spacebar/esc/return/linefeed combo
	          if ( key .eq. 32 ) then
		    aeqSsp_autoTbl_hotKey(27).method = hotMethod
		    aeqSsp_autoTbl_hotKey(13).method = hotMethod
		    aeqSsp_autoTbl_hotKey(10).method = hotMethod
	          endif

	        endif	

	      endif	

	    elseif ( token(i) .eq. 'POSITION' ) then

	      if ( valueIndex .le. valueCount(i) ) then

	        if ( .not. gut_editint (
     &			value(valueIndex,i)(:valueLen(valueIndex,i)),
     &			position,
     &			0, 
     &			7 ) ) position = 0

	        if ( position .ge. 0 .and. position .le. 7 ) then
	
		  if ( key .eq. 0 ) then   
		    j = 128
		    do while ( j .ge. 1 .and. key .eq. 0 )
		      if ( aeqSsp_autoTbl_hotkey(j).position.eq.position) then
                	key = j
		      else
			j = j - 1
		      endif
		    enddo
		  endif

		  if ( key .ne. 0 ) then 

		    if ( aeqSsp_autoTbl_hotKey(key).position .ne. position .and.
     &		         aeqSsp_autoTbl_hotKey(key).position .ne. 0 .and.
     &			 aeqSsp_autoTbl_hotKey(key).isEnabled ) then
		    
		      ! Position has changed. Erase previous position
	    	      call aeqSsp_autoMan_outHotKey ( 
     &			aeqSsp_autoTbl_hotKey(key).position,
     &			aeqSsp_autoTbl_hotKey(key).keyName,
     &			.false. )

		    endif

		    aeqSsp_autoTbl_hotKey(key).position = position

		    ! Check for spacebar/esc/return/linefeed combo
		    if ( key .eq. 32 ) then
		      aeqSsp_autoTbl_hotKey(27).position = position
		      aeqSsp_autoTbl_hotKey(13).position = position
		      aeqSsp_autoTbl_hotKey(10).position = position
		    endif

		    ! Update
	    	    call aeqSsp_autoMan_outHotKey ( 
     &			aeqSsp_autoTbl_hotKey(key).position,
     &			aeqSsp_autoTbl_hotKey(key).keyName,
     &			aeqSsp_autoTbl_hotKey(key).isEnabled )

	   	    ! Make sure no other keys share this position
		    do j=1,128
		      if ( j .ne. key .and. 
     &			   aeqSsp_autoTbl_hotKey(j).position .eq. position) then
		        aeqSsp_autoTbl_hotKey(j).position = 0
		      endif
		    enddo		
	
	          else

		    statusText = '% POSITION value not between 0 and 7'
		    status = .false.


	          endif	
	        endif	
	      endif	

	    elseif ( token(i) .eq. 'ENABLED' ) then

	      if ( key .ne. 0 ) then

	        if ( valueIndex .le. valueCount(i) ) then

	          answer = value(valueIndex,i)(:valueLen(valueIndex,i))

	          ! Make uppercase
	          call gut_uppercase ( answer )	

	          if ( answer .eq. 'YES' .or. answer .eq. 'NO' ) then 

	            if ( answer .eq. 'YES' ) then	
		      isEnabled = .true.
	            else
		      isEnabled = .false.
		    endif

		    aeqSsp_autoTbl_hotKey(key).isEnabled = isEnabled

		    ! Check for spacebar/esc/return/linefeed combo
		    if ( key .eq. 32 ) then
		      aeqSsp_autoTbl_hotKey(27).isEnabled = isEnabled
		      aeqSsp_autoTbl_hotKey(13).isEnabled = isEnabled
		      aeqSsp_autoTbl_hotKey(10).isEnabled = isEnabled
		    endif

		    ! Update
	    	    call aeqSsp_autoMan_outHotKey ( 
     &			aeqSsp_autoTbl_hotKey(key).position,
     &			aeqSsp_autoTbl_hotKey(key).keyName,
     &			aeqSsp_autoTbl_hotKey(key).isEnabled )

	          endif	

	        endif	

	      endif

	    elseif ( token(i) .ne. 'STATUS' ) then

	      ! Unrecognized token
	      status = .false.
	      statusText = '% Undefined token '//token(i)

	    endif
	
	    i = i + 1

	  enddo 

	  ! Advance to next array position
	  valueIndex = valueIndex + 1

	enddo 

	aeqSsp_autoMan_HOTKEY = status

	return

	end

!++ AEQSSP_AUTOMAN_AST_TIMER
!
	subroutine aeqSsp_automan_AST_timer()

	implicit none

	! An AST to service the connect timer
	!
	! THIS ROUTINE IS AN CALLED ONCE, THEN IS CALLED THEREAFTER AS AN AST

! Include files:

	include	'($SSDEF)'
	include	'($IODEF)'	
	include	'AEQ_SSP:AUTODEF'
	include	'AEQ_SSP:AUTOTBL'
	include	'TUT:TUTDEF'
	include	'TUT:TUTCOM'

! Arguments:

! Routines Used:

	integer*4	sys$assign
	integer*4	gut_trimblks
	integer*4	aeqSsp_autoMan_check
	logical*4	aeqSsp_autoMan_setTimerAST
	logical*4	aeqSsp_autoMan_setAST
	integer*4	sys$fao 

! Author:	Mike Bergsma		7/93
!		Ab Initio Software
!
! Modifications:
!
!	17-Nov-1993	Michael Bergsma
!	- we are really only connecting to the AUTOROUTER.  A real connection
!	  is not established until the first message arrives.
!
! Local Variables:

	integer*4	status

	integer*2	iosb(4)

	character*(SENDER_SIZE)		sender
	integer*4			senderLen

	character*10	channelNum
	integer*4	channelNumLen

	sender = 
     &		aeqSsp_autoTbl_senderInstance(:aeqSsp_autoTbl_senderInstLen)//
     &		'.'//
     &		aeqSsp_autoTbl_senderObject(:aeqSsp_autoTbl_senderObjectLen)
	senderLen = gut_trimblks ( sender )

	if ( aeqSsp_autoTbl_connectTries .gt. 0 ) then

	  ! Keep trying to connect 

	  if ( aeqSsp_autoTbl_inChannel .eq. 0 ) then
	
	    ! Not yet connected to the 
	    ! aeqSsp_autoTbl_mbxRouter(:aeqSsp_autoTbl_mbxRouterLen)_<target> mailbox
	    ! Look for the mailbox created by the AUTOROUTER
	    status = sys$assign (
     &         aeqSsp_autoTbl_mbxRouter(:aeqSsp_autoTbl_mbxRouterLen)//'_'//sender(:senderLen), 
     &	       aeqSsp_autoTbl_inChannel,
     &	       ,, )

	    ! Check to see if the mailbox got created.
	    if ( status ) then

	      ! Connected to AUTOROUTER.
	      status = sys$fao (	'!UL',
     &					channelNumLen,
     &					channelNum, 
     &					%VAL(aeqSsp_autoTbl_inChannel) )

	      call aeqSsp_automan_logDebug( 
     &		'Connection ('//channelNum(:channelNumLen)//
     &		') established with '//
     &          aeqSsp_autoTbl_mbxRouter(:aeqSsp_autoTbl_mbxRouterLen)//'_'//sender(:senderLen) )

!!!	      ! If this is a detached process, wake it up
!!!	      if ( tut_chan .eq. -1 .or. tut_chan .eq. 0 ) then
!!!	        call aeqSsp_automan_logDebug( 'wake up' )
!!!		call sys$setef ( %val ( EFN_WAITFR ) )
!!!	      endif

	      ! Continue waiting until the first message comes in.
	      if (.not. 
     &		  aeqSsp_autoMan_setTimerAST(aeqSsp_autoTbl_AST_timer)) then
     		call aeqSsp_autoMan_terminate( .false., .false. )
	        call aeqSsp_automan_fallout()
	      endif

	      ! Set a read attention AST on the read-only mailbox.  
	      ! Thus, whenever the autoRouter writes a message to the mailbox, 
	      ! the AST will be invoked to read it.
	      if (.not. aeqSsp_autoMan_setAST( aeqSsp_autoTbl_inChannel )) then
          	call aeqSsp_autoMan_terminate( .false., .false. )
	        call aeqSsp_automan_fallout()
	      endif	
		
	    elseif ( status .eq. SS$_IVDEVNAM ) then

	      ! Device not created yet
	      aeqSsp_autoTbl_connectTries = aeqSsp_autoTbl_connectTries - 1 

	      ! Try again later	
	      call aeqSsp_autoMan_logDebug( 
     &		'...waiting for connection to '//
     &          aeqSsp_autoTbl_mbxRouter(:aeqSsp_autoTbl_mbxRouterLen)//'_'//sender(:senderLen) )

	      if (.not. 
     &		  aeqSsp_autoMan_setTimerAST(aeqSsp_autoTbl_AST_timer)) then
     		call aeqSsp_autoMan_terminate( .false., .false. )
	        call aeqSsp_automan_fallout()
	      endif	

	    else

		if ( .not. aeqSsp_autoMan_check( status, iosb(1) ) )
     &  	  call aeqSsp_automan_logDebug ( '% Failed to assign incoming mbx channel')
 
	      call aeqSsp_automan_outMessage( 
     &		'% Could not connect to '//
     &          aeqSsp_autoTbl_mbxRouter(:aeqSsp_autoTbl_mbxRouterLen)//'_'//sender(:senderLen)//
     &		'.\nPlease retry the automation session or '//
     &		'contact your facilitator if the problem persists.',
     &		.false. )

	      ! Do not continue. Abort entire operation.
	      call aeqSsp_autoMan_terminate( .false., .false. )
	      call aeqSsp_automan_fallout()
	   
	    endif

	  else

	    ! We are connected to the target Node, but we must now wait for
	    ! the first message from the target object.
	    if ( .not. aeqSsp_autoTbl_connectStatus ) then

	      ! Try again later	
     	      aeqSsp_autoTbl_connectTries = aeqSsp_autoTbl_connectTries - 1 

	      call aeqSsp_autoMan_logDebug( 
     &		'.....waiting for first message from '//
     &		aeqSsp_autoTbl_target(:aeqSsp_autoTbl_targetLen) )

	      if (.not. 
     &		   aeqSsp_autoMan_setTimerAST(aeqSsp_autoTbl_AST_timer)) then
     		call aeqSsp_autoMan_terminate( .false., .false. )
	        call aeqSsp_automan_fallout()
	      endif	

	    endif	

	  endif

	else

	  ! Out of tries...
	  if ( aeqSsp_autoTbl_inChannel .eq. 0 ) then

	    call aeqSsp_automan_outMessage( 
     &	    	'% Timed out waiting for connection to '//
     &		aeqSsp_autoTbl_mbxRouter(:aeqSsp_autoTbl_mbxRouterLen)//'_'//sender(:senderLen)//
     &		'.\nAutorouter failed to route message to target. '//
     &		'A network connection may be unreachable or the '//
     &		'autorouter has malfunctioned. '//
     &		'Please retry the automation session or '//
     &		'contact your facilitator if the problem persists.',
     &		.false. )

	    ! Do not continue. Abort entire operation.
	    call aeqSsp_autoMan_terminate( .false., .false. )
	    call aeqSsp_automan_fallout()

	  elseif ( .not. aeqSsp_autoTbl_connectStatus ) then

	    call aeqSsp_automan_outMessage( 
     &		'% Timed out waiting for first message from '//
     &		aeqSsp_autoTbl_target(:aeqSsp_autoTbl_targetLen)//
     &		'.\nThe AUTOMATION target failed to respond. '//
     &		'Please retry the automation session or '//
     &		'contact your facilitator if the problem persists.',
     &		.false. )

	    ! Do not continue. Abort entire operation.
	    call aeqSsp_autoMan_terminate( .false., .false. )
	    call aeqSsp_automan_fallout()

	  endif

	endif


	return

	end

!++ AEQSSP_AUTOMAN_SETTIMERAST
!
	logical*4 function aeqSsp_automan_setTimerAST( deltaTime )

	implicit none

	! Re-issue the connect timer AST

! Include files:

	include	'AEQ_SSP:AUTODEF'

! Arguments:

	integer*4	deltaTime(2)				! [R]
	! - VMS delta time

! Function value:
!
!	- returns success or failure

! Routines Used:

	external	aeqSsp_autoMan_AST_timer
	integer*4	aeqSsp_autoMan_check
	integer*4	sys$setimr

! Author:	Mike Bergsma		7/92
!		Ab Initio Software
!
! Modifications:

! Local Variables:

	integer*4	status

! Code:

	status = sys$setimr(	%val(EFN_TIMER),
     &				deltaTime,
     &				aeqSsp_automan_AST_timer,
     &				, )
	if ( .not. aeqSsp_autoMan_check( status, 1 ) ) then

	  call aeqSsp_automan_outMessage( 
     &		'% Cannot set connection timer. '//
     &		'This is an internal program error.\n'//
     &		'Please try logging completely out and then logging '//
     &		'back in; or contact your facilitator.',
     &		.false. )

	  aeqSsp_autoMan_setTimerAST = .false.

	else

	  aeqSsp_autoMan_setTimerAST = .true.

	endif

	return

	end

!++ AEQSSP_AUTOMAN_SETCTRLAST2

	subroutine aeqSsp_automan_setctrlAst2 ( )

	implicit none

	! Set up handler for CONTROL_C

! Include files:

! Arguments:

! Routines Used:

	external	aeqSsp_autoMan_handler2

! Author:	Mike Bergsma		7/92
!		Ab Initio Software
!
! Modifications:

! Local Variables:

	call aeqSsp_automan_setCtrlAst ( aeqSsp_autoMan_handler2 )

	end

!++ AEQSSP_AUTOMAN_SETCTRLAST
!
	subroutine aeqSsp_automan_setCtrlAst ( handler )

	implicit none

	! Trap control/C because otherwise it cancels the AST
	! that detects incoming messages

! Include files:

	include '($IODEF)'	!I/O code definitions
	include	'($PSLDEF)'	
	include	'TUT:TUTDEF'
	include	'TUT:TUTCOM'
	include 'CFC:EFNDEF'
	include	'AEQ_SSP:AUTODEF'

! Arguments:

	external	handler
	! - handler to set

! Routines Used:

	logical*4	aeqSsp_autoUtil_check
	integer*4	sys$qiow
	

! Author:	Mike Bergsma		7/92
!		Ab Initio Software
!
! Modifications:
!
!	6/2/99		Mike Bergsma
!	- change to use OUTBAND method
!
! Local Variables:

	integer*4	status
	integer*2	iosb(4)

	integer*4	ctrlmask(2)		! Control mask ^Y and ^P and ^C
	  data		ctrlmask / 0 , '02010008'x /

! Code:

	if ( tut_termtype .ne. TUT_K_MAILBOX .and.
     &	     tut_chan .ne. -1 .and. tut_chan .ne. 0 ) then

	  status = sys$qiow(	%val(0),		! Event flag
     &	    			%val(tut_chan),		! I/O channel
     &	    			%val(IO$_SETMODE+	! Set mode
     &				IO$M_OUTBAND),		!  - ctrl/c 
     &	    			iosb,			! I/O status block
     &	    			,,			! AST (none)
     &				handler,		! P1: AST address
     &				ctrlmask,		! P2: AST parameter
     &	    			,,,,)
	  if ( .not. aeqSsp_autoUtil_check( status, iosb(1) ) )
     &    call aeqSsp_autoutil_log ( '% Failed to set CTRL/C AST on tut_chan') 

	else

	  ! Some day, maybe important...

	endif

	return

	end

!++ AEQSSP_AUTOMAN_HANDLER
!
	integer*4 function aeqSsp_autoMan_handler(sigargs, mechargs)

	! Handler for the AUTOMAN routines.
	! Check for signal condition of EXIT, QUIT, ^Z, ^C, ^Z
	! In any case, function returns SS$_CONTINUE

	implicit none

! Function value:

	! 1 if the signal was handled successfully (i.e. CONTINUE)
	! 0 if the signal was not QUIT or EXIT (i.e. RESIGNAL)

! Include files:

	include	'($SSDEF)'	
	include	'($PSLDEF)'	
	include	'TUT:TUTDEF'

! Arguments:

	integer*4	sigargs(*)		! READ
	! Signal arguments

	integer*4	mechargs(*)		! READ
	! Mechanism arguments

! Routines used: 

	integer 	Sig_Chf_GetCond
	logical 	Sig_Chf_MatchCond

! Author:	Mike Bergsma		7/92
!		Ab Initio Software
!
! Modifications:
!
!	17-Nov-1993	Michael Bergsma
!	- don't send DISCONNECT message unless we have received at least one
!	  message from the target object
!
!	15-Nov-1993	Michael Bergsma
!	- send DISCONNECT message to ROUTER if there is no other object
!
!	Mike Bergsma	3/7/94
!	- if the automation hasn't collected any data, then make sure the
!	  the tst_tstWcom_testResultsEntered variable is set back to .false.
!
!	Mike Bergsma	3/10/94
!	- send ABORT method rather than DISCONNECT method
!
!	Mike Bergsma	1/12/95
!	- this handler now just prevents CTRL/Z, etc from having any effect.
!
!	Mike Bergsma	1/12/97
!	- modify to support Alpha implementation
!
!	Mike Bergsma	6/2/99
!	- update for PROMIS 5.5
!
! 	Mike Bergsma	2/20/00
!	- when timer signal occurs, try continuing...
!~	  

! Local variables:

	integer*4 	condition
!
! Code:
!
	condition = Sig_Chf_GetCond (sigArgs, mechArgs)	 ! get the condtion

	if ( Sig_Chf_MatchCond (sigArgs, mechArgs, TUT__CONTROLC) .or.
     &	     Sig_Chf_MatchCond (sigArgs, mechArgs, TUT__EXIT) .or.
     &	     Sig_Chf_MatchCond (sigArgs, mechArgs, TUT__QUIT) ) then

	  call aeqSsp_automan_outMessage( 
     &		'% CTRL/C, CTRL/Z, EXIT, and QUIT have been disabled.\n'//
     &		'Please respond as required by the Automation session',
     &		.true. )

   	  aeqSsp_autoMan_handler = SS$_CONTINUE		!Continue

	else 

	  ! It's not one we know about.
	  call aeqSsp_automan_outMessage( 
     &		'Timer or other signal occurred.\n',
     &		.true. )

   	  !aeqSsp_autoMan_handler = SS$_RESIGNAL
	  aeqSsp_autoMan_handler = SS$_CONTINUE

	endif

 	return

	end

!++ AEQSSP_AUTOMAN_HANDLER2
!
	subroutine aeqSsp_autoMan_handler2 ( chr )

	! CTRL/C, CTRL/Y Handler for the AUTOMAN routines.

	implicit none

! Include files:

	include	'($SSDEF)'	
	include	'($PSLDEF)'	
	include	'TUT:TUTDEF'

! Arguments:

	integer*4	chr
	! Control character typed ( by value )

! Routines used: 

! Author:	Mike Bergsma		6/99
!		Ab Initio Software
!
! Modifications:
!~	  

! Local variables:

	character*1	tmpchr

	parameter	CTRLC = char(3)
	parameter	CTRLP = char(16)
	parameter	CTRLY = char(25)

! Code

	tmpchr = char(%loc(chr))

	if ( tmpchr .eq. CTRLY .or. tmpchr .eq. CTRLC ) then	

	  call aeqSsp_automan_outMessage( 
     &		'% CTRL/C and CTRL/Y have been disabled.\n'//
     &		'Please respond as required by the Automation session',
     &		.true. )
	  
	  ! Re-establish the handler
	  call aeqSsp_automan_setctrlAst2()

	endif

 	return

	end

!++ AEQSSP_AUTOMAN_TERMINATE
!
	subroutine aeqSsp_autoMan_terminate ( 	functionStatus, 
     &						connectStatus )

! 	Completes the automation session.

	implicit none

! Include files:

	include	'AEQ_SSP:AUTODEF'
	include	'AEQ_SSP:AUTOTBL'

! Arguments:

	logical*4	functionStatus					! [R]
	! - status of PROMIS function. If false, it will abort.

	logical*4	connectStatus 					! [R]
	! - status of connection. If false, it will allow Manual


! Routines Used:


! Author:	Mike Bergsma		4/96
!		Ab Initio Software
!
! Modifications:
!
! Local variables:
!
! Code:

	! Exit command loop 
	aeqSsp_autoTbl_sessionStatus = .false.

	! Abort PROMIS function if .false.
	aeqSsp_autoTbl_functionStatus = functionStatus

	! Allow manual if .false.
	aeqSsp_autoTbl_connectStatus = connectStatus

	! Turn on pagewait if false connect, function, and auto status
	if ( 	.not. functionStatus .and.
     &		.not. connectStatus ) aeqSsp_autoTbl_pagewait = .true.

	return

	end

!++ AEQSSP_AUTOMAN_CHECK
!
	integer*4 function aeqSsp_autoMan_check( status, iostatus )

!	Checks and displays system serivce status and ioStatus varibles

	implicit none

! Include files:

	include	'AEQ_SSP:AUTODEF'
	include	'AEQ_SSP:AUTOTBL'

! Arguments:

	integer*4	status					! [R]
	! - status from system service (eg: SYS$* or LIB$*)

	integer*2	ioStatus				! [R]
	! - additional ioStatus (eg: from SYS$QIO)
	! - specify 1 if no there is no ioStatus 

! Function Value:
!
!	- returns 1 is both status and ioStatus are success
!	- returns -1 if either status or ioStatus are failure

! Routines Used:


! Author:	Mike Bergsma		4/93
!		Ab Initio Software
!
! Modifications:

! Local Varibles:

	integer*4	msgLen
	byte		outadr(4)
	character*80	message

! Code:

	if ( .not. status ) then

	  call sys$getmsg(	%val(status),
     &				msgLen,
     &				message,
     &				,
     &				outadr )
     	  call aeqSsp_autoMan_outMessage ( message(:msgLen), .true. )

	endif

	if ( .not. ioStatus ) then

	  call sys$getmsg(	%val(ioStatus),
     &				msgLen,
     &				message,
     &				,
     &				outadr )
     	  call aeqSsp_autoMan_outMessage ( message(:msgLen), .true. )

	endif

	aeqSsp_autoMan_check = ( status .and. ioStatus )

	return 

	end

!++ AEQSSP_AUTOMAN_INITHOTKEYS
!
	subroutine aeqSsp_autoMan_initHotKeys ( )

!	Initializes certain special hot keys

	implicit none

! Include files:

	include	'AEQ_SSP:AUTODEF'
	include	'AEQ_SSP:AUTOTBL'

! Arguments:

! Function Value:

! Routines Used:
!

! Author:	Mike Bergsma		4/93
!		Ab Initio Software

! Modifications:

!	Mike Bergsma		11/6/95
!	- disable all hot keys initially
!
! Local Varibles:

	integer*4	i,k

! Code:
!
	! First set all keys to disabled.
	do i=1,128
	  aeqSsp_autoTbl_hotKey(i).isEnabled = .false.
	  aeqSsp_autoTbl_hotKey(i).position = 0
	  aeqSsp_autoTbl_hotKey(i).keyname = ' '
	  aeqSsp_autoTbl_hotKey(i).method = ' '
	enddo


	! Position 1
	k = ICHAR('M')
	aeqSsp_autoTbl_hotKey(k).position = 1
	aeqSsp_autoTbl_hotKey(k).keyName = 'Manual'
	aeqSsp_autoTbl_hotKey(k).method = 'DISCONNECT'
	call aeqSsp_automan_outHotKey ( 
     &		aeqSsp_autoTbl_hotKey(k).position,
     &		aeqSsp_autoTbl_hotKey(k).keyName,
     &		aeqSsp_autoTbl_hotKey(k).isEnabled )

	! Position 2
	k = ICHAR('R')
	aeqSsp_autoTbl_hotKey(k).position = 2
	aeqSsp_autoTbl_hotKey(k).keyName = 'Retry'
	aeqSsp_autoTbl_hotKey(k).method = 'CONNECT'
	call aeqSsp_automan_outHotKey ( 
     &		aeqSsp_autoTbl_hotKey(k).position,
     &		aeqSsp_autoTbl_hotKey(k).keyName,
     &		aeqSsp_autoTbl_hotKey(k).isEnabled )

	! Position 3
	k = ICHAR('Q')
	aeqSsp_autoTbl_hotKey(k).position = 3
	aeqSsp_autoTbl_hotKey(k).keyName = 'Quit'
	aeqSsp_autoTbl_hotKey(k).method = 'ABORT'
	call aeqSsp_automan_outHotKey ( 
     &		aeqSsp_autoTbl_hotKey(k).position,
     &		aeqSsp_autoTbl_hotKey(k).keyName,
     &		aeqSsp_autoTbl_hotKey(k).isEnabled )

	! Position 4
	k = 26	! CTRL/Z
	aeqSsp_autoTbl_hotKey(k).position = 4
	aeqSsp_autoTbl_hotKey(k).keyName = '^Z'
	aeqSsp_autoTbl_hotKey(k).method = 'ABORT'
	call aeqSsp_automan_outHotKey ( 
     &		aeqSsp_autoTbl_hotKey(k).position,
     &		aeqSsp_autoTbl_hotKey(k).keyName,
     &		aeqSsp_autoTbl_hotKey(k).isEnabled )

	! Sharing position 4
	k = 3 	! CTRL/C
	aeqSsp_autoTbl_hotKey(k).position = 4
	aeqSsp_autoTbl_hotKey(k).keyName = '^C'
	aeqSsp_autoTbl_hotKey(k).method = 'ABORT'

	! Position 5
	k = ICHAR('S')
	aeqSsp_autoTbl_hotKey(k).position = 5
	aeqSsp_autoTbl_hotKey(k).keyName = 'Setup'
	aeqSsp_autoTbl_hotKey(k).method = 'SETUP'
	call aeqSsp_automan_outHotKey ( 
     &		aeqSsp_autoTbl_hotKey(k).position,
     &		aeqSsp_autoTbl_hotKey(k).keyName,
     &		aeqSsp_autoTbl_hotKey(k).isEnabled )

	! Position 6
	k = ICHAR('D')
	aeqSsp_autoTbl_hotKey(k).position = 6
	aeqSsp_autoTbl_hotKey(k).keyName = 'Data'
	aeqSsp_autoTbl_hotKey(k).method = 'DATA'
	call aeqSsp_automan_outHotKey ( 
     &		aeqSsp_autoTbl_hotKey(k).position,
     &		aeqSsp_autoTbl_hotKey(k).keyName,
     &		aeqSsp_autoTbl_hotKey(k).isEnabled )

	! Position 7
	k = ICHAR('T')
	aeqSsp_autoTbl_hotKey(k).position = 7
	aeqSsp_autoTbl_hotKey(k).keyName = 'Text Msg'
	aeqSsp_autoTbl_hotKey(k).method = 'MESSAGE'
	call aeqSsp_automan_outHotKey ( 
     &		aeqSsp_autoTbl_hotKey(k).position,
     &		aeqSsp_autoTbl_hotKey(k).keyName,
     &		aeqSsp_autoTbl_hotKey(k).isEnabled )


	! Spacebar/esc/return/linefeed combo
	aeqSsp_autoTbl_hotKey(32).position = 0
	aeqSsp_autoTbl_hotKey(32).keyName = ' Return'
	aeqSsp_autoTbl_hotKey(32).method = 'DISCONNECT'
	aeqSsp_autoTbl_hotKey(13).position = 0
	aeqSsp_autoTbl_hotKey(13).keyName = ' Return'
	aeqSsp_autoTbl_hotKey(13).method = 'DISCONNECT'
	aeqSsp_autoTbl_hotKey(10).position = 0
	aeqSsp_autoTbl_hotKey(10).keyName = ' Return'
	aeqSsp_autoTbl_hotKey(10).method = 'DISCONNECT'
	aeqSsp_autoTbl_hotKey(27).position = 0
	aeqSsp_autoTbl_hotKey(27).keyName = ' Return'
	aeqSsp_autoTbl_hotKey(27).method = 'DISCONNECT'

	return
	end

!++ AEQSSP_AUTOMAN_LOG
!
	subroutine aeqSsp_autoMan_log ( message )

!	Log a message if the log flag is set.

	implicit none

! Include files:

	include	'AEQ_SSP:AUTODEF'
	include	'AEQ_SSP:AUTOTBL'

! Arguments:

	character*(*)	message				! [R]
	! - text message

! Routines Used:

! Author:	Mike Bergsma		4/95
!		Ab Initio Software
!
! Modifications:

! Local variables:

! Code:

	if ( aeqSsp_autotbl_log ) 
     &	  call aeqSsp_autoUtil_logMessage ( message, LOG_UNIT )

	end

!++ AEQSSP_AUTOMAN_LOGDEBUG
!
	subroutine aeqSsp_autoMan_logDebug ( message )

!	Maybe log a status text message and maybe display as well.

	implicit none

! Include files:

	include	'AEQ_SSP:AUTODEF'
	include	'AEQ_SSP:AUTOTBL'

! Arguments:

	character*(*)	message				! [R]
	! - text message

! Routines Used:

! Author:	Mike Bergsma		4/95
!		Ab Initio Software
!
! Modifications:
!

! Local variables:

! Code:

	if ( aeqSsp_autotbl_debug ) then

     	  call aeqSsp_automan_outMessage ( message, .true. )

	elseif ( aeqSsp_autotbl_log ) then

     	  call aeqSsp_autoUtil_logMessage ( message, LOG_UNIT )

	endif

	end
