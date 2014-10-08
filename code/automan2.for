!*****************************************************************************!
!                    AUTOMAN Software Source Code.                            !
!                                                                             !
!          ***** Copyright: (c) 1993 Ab Initio Software                       !
!          ***** All rights reserved - Licensed Material.                     !
!          ***** Program property of Ab Initio Software                       !
!                                                                             !
!*****************************************************************************!
!++ AUTOMAN2.FOR

! The AUTOmation MANagement routines are distributed into three modules:
!
!	automan.for		! Automation communication routines
!	automan2.for		! General purpose automation utilities
!	automan3.for		! Automation User Interface utilities
!	automan4.for		! Automation Database Utilities

! Externally Callable Routines:

! For Internal Use Only:

!	aeqSsp_autoMan_itemIndex	Get item index from item ID
!	aeqSsp_autoMan_siteIndex	Get site index from site ID
!	aeqSsp_autoMan_waferIndex	Get wafer index from wafer ID
!	aeqSsp_autoMan_itemId		Get item ID from item index
!	aeqSsp_autoMan_siteId		Get site ID from site index
!	aeqSsp_autoMan_waferId		Get wafer ID from wafer index
!	aeqSsp_autoMan_fmtWaferId	Format wafer ID from lotid and wafer #
!	aeqSsp_autoMan_next		Get next wafer, site, and item indexes
!	aeqSsp_autoMan_validData	Check data against data type
!	aeqSsp_autoMan_getParm		Substitute a parameter

! Produced by:	Ab Initio Software

! Author:	Michael Bergsma 	1993

! Modifications:
!
!    $Log: automan2.for,v $
!    Revision 1.3  2011-06-09 22:31:59  bergsma
!    Remove "err=999" on format statement
!
!    Revision 1.2  2003-01-16 14:32:07  bergsma
!    V3.1.0
!    Added Modification tags.
!
!
!-

!++ AEQSSP_AUTOMAN_ITEMINDEX
!                                      
	integer*4 function aeqSsp_autoMan_itemIndex( itemId, itemIdLen )

!	Given an item name, return the item index

	implicit none

! Include files:

	include	'AEQ_SSP:AUTODEF'
	include	'FIL:FILDEF'
	include	'FIL:TYPDEF'
	include	'GUT:GUTDEF'
	include	'FIL:TESTCDEF'	

! Arguments:

	character*(*)	itemId 				! [R/W]
	! - if numeric, then fetch that numbered item 
	! - if alphanumeric, then search for item id 

	integer*4	itemIdLen			! [R/W]
	! - length of item id

! Function value:
	! -1 means itemId was UNSPECIFIED
	!  0 means itemId was UNDEFINED
	! >0 is itemId

! Routines used:

	integer*4	gut_trimblks
	logical*4	gut_editint

! Author:	Mike Bergsma		4/93
!		Ab Initio Software
!
! Modifications:

! Local Variables:

	integer*4	itemIndex			
	integer*4	i
	logical*4	found
	character*(TYP__DIPROMPTSZ)	prompt,item

! Code:

	if ( itemIdLen .gt. 0 ) then

	  ! Check if item was specified as a number.
	  if ( gut_editint(	itemId(:itemIdLen),
     &				itemIndex,
     &       			1, test.noTestDi) ) then

	    itemId = test.di(itemIndex).prompt
	    itemIdLen = gut_trimblks( itemId )

	  else

	    i = 1
	    found = .false.
	    do while( .not. found .and. i  .le. test.noTestDi )

	      ! Compare is case sensitive, so copy and uppercase both strings.

	      prompt = test.di(i).prompt
	      call gut_upperCase( prompt )

	      item = itemId
	      call gut_upperCase( item )

	      if ( item(:itemIdLen) .eq. prompt ) then
	        found = .true.
	        itemIndex = i
	      else
	        i = i + 1
	      endif

	    enddo

	    if ( .not. found ) itemIndex = INDEX_UNDEFINED

	  endif

	else

	  itemIndex = INDEX_UNSPECIFIED
	  itemId = '(UNSPECIFIED)'
	  itemIdLen = 13

	endif

	aeqSsp_autoMan_itemIndex = itemIndex

	return

	end

!++ AEQSSP_AUTOMAN_SITEINDEX

	integer*4 function aeqSsp_autoMan_siteIndex(	siteId,
     &							siteIdLen,
     &							noSites )

!	Given a site name, return the site index

	implicit none

! Include files:

	include	'AEQ_SSP:AUTODEF'

! Arguments:
	
	character*(*)	siteId				! [R/W]
	! - numeric site id
	! - (someday could support alphanumeric id, like CENTER, LEFT, ...)

	integer*4	siteIdLen			! [R/W]
	! - length of site id

	integer*4	noSites				! [R]
	! - maximum number of sites possible

! Function value:
	! -1 means siteId was UNSPECIFIED
	!  0 means siteId was UNDEFINED
	! >0 is siteId

! Routines used:

	logical*4	gut_editint

! Author:	Mike Bergsma		4/93
!		Ab Initio Software
!
! Modifications:

! Local variables:

	integer*4	siteIndex			

! Code:

	if ( siteIdLen .gt. 0 ) then

	  if ( .not. gut_editint( 	siteId(:siteIdLen), 
     &					siteIndex, 
     &					1, 
     &					noSites ) ) then
	    siteIndex = INDEX_UNDEFINED

	  endif

	else
	  siteIndex = INDEX_UNSPECIFIED
	  siteId = '?'
	  siteIdLen = 1
	endif
	  
	aeqSsp_autoMan_siteIndex = siteIndex

	return

	end

!++ AEQSSP_AUTOMAN_WAFERINDEX

	integer*4 function aeqSsp_autoMan_waferIndex( waferId, waferIdLen )

!	Given an wafer id, return the wafer index
!
!	WE DON'T LIKE THIS ROUTINE, BUT WE CAN FIND NO PROMIS LIBRARY
!	FUNCTION THAT DOES THE REQUIRED FIDDLING WITH THE WAFERID ARRAYS

	implicit none

! Include files:

	include	'AEQ_SSP:AUTODEF'

	include	'FIL:TYPDEF'
	include	'FIL:FILDEF'
	include	'GUT:GUTDEF'	
	include	'TST:TSTDEF'	
	include	'TST:ENTDEF'	
	include	'TST:PARMDEF'	
	include 'LOT:FUNDEF'
	include 'PAR:LOTSTDEF'
	include	'FIL:TRESCDEF'	
	include	'FIL:TESTPDEF'	
	include	'TST:TSTWCOM'	

! Arguments:

	character*(*)	waferId				! [R/W]
	! - if numeric, then fetch that numbered wafer
	! - if alphanumeric, then search for wafer id 

	integer*4	waferIdLen			! [R/W]
	! - length of wafer ID

! Function value:
!
! 	-1 means waferId was UNSPECIFIED
!  	0 means waferId was UNDEFINED
!	>0 is waferId index

! Routines used:

	integer*4		gut_strXMatch
	logical*4		tst_tresUtil_ValidWafer

! Author:	Mike Bergsma		4/93
!		Ab Initio Software
!
! Modifications:

! Local variables:

	integer*4	status,
     &			waferIndex,
     &     		newIndex,
     &			actlIndex,
     &			availIndex,
     &			newAvailIndex

	character*1			saveIndexC
	character*1			saveFreeItem
	character*(TYP__COMPONENTIDSZ)	saveCompId

! Code:

	! Make sure the wafer is formatted correctly
	if ( waferIdLen .gt. 0 ) 
     &	  call aeqSsp_autoMan_fmtWaferId( tres.lotid, waferId, waferIdLen )

	if ( waferIdLen .gt. 0 ) then

	  ! Look for the wafer in the TRES array.
	  ! Search both tested and untested wafers.
	  status = gut_strxmatch(	waferId,
     &		  			waferIndex, 
     !!!&				TRES_X_COMPIDS,
     &					TYP__MAXTESTCOMPS,
     &					tres.compIds )

	  ! If not there, then the wafer is UNDEFINED
	  if ( status .ne. GUT__NORMAL ) waferIndex = INDEX_UNDEFINED

	  if ( waferIndex .gt. 0 ) then

	    ! WAFER is known by TRES

	    ! For "enter data", make sure the wafer is present in ACTL.

	    ! Fetch index that points to position in the ACTL record
	    actlIndex = ICHAR( tres.compIndexC( waferIndex:waferIndex ) )

	    ! For "enter lot data" or "probe" data entry, the wafer
	    ! must exist within the actl record.
	    ! For "correct lot data", with the exception of "probe",
	    ! the wafer need not be present in the actl record, it only
	    ! needs to have already been tested.
	    if ( ( tst_tstWcom_isEnterTest .or.
     &		   tst_tstWcom_typeIndex .eq. TST__PROBE ) .and.
     &	         ( actlIndex .eq. 0 .or.
     &		   .not. tst_tresUtil_ValidWafer ( waferIndex ) ) ) then

	      ! Not valid
	      waferIndex = INDEX_UNDEFINED

	    else

	      ! Valid wafer.

	      ! Has the wafer been tested already?
	      ! If it has, then the wafer index will be within the 
	      ! "components already tested" array.

	      if ( waferIndex .gt. tres.compCount ) then

	        ! Wafer has not been tested.

		! Check to see if the new wafer can be added to the test.
		! - NO if the we're doing enterData with auto wafer prompting
		!   and the wafer index is greater than the allowed total 
		!   number of wafers.
		! - NO if the we're doing correctData and the wafer index is 
		!   not in the list of wafers that we must pick from.

	        if ( ( tst_tstWcom_isEnterTest .and. 
     &		       tst_tstWcom_AutoCprompt .and.
     &		       waferIndex .gt. tres.compsToTest ) .or. 
     &		     ( .not. tst_tstWcom_isEnterTest .and. 
     &		       waferIndex .gt. tres.compCount ) ) then
!!!     &		       waferIndex .gt. tres.compsToTest ) ) then

		  ! Wafer cannot be tested because its not part of the required
		  ! list of wafers.
	          waferIndex = INDEX_UNDEFINED

	        else

		  ! New wafer can be tested.

	          ! Add new wafer to TRES array and update available wafer list

	          tres.compCount = tres.compCount + 1
	          newIndex = tres.compCount

  	          ! If the order of the wafers is different, then switch
	          ! places in the tres array.
		  ! THIS IS THE PART OF THE CODE WE HATE THE MOST

	          if ( waferIndex .gt. newIndex ) then

		    ! New wafer tested out of expected order.

		    ! Switch positions of the wafers in the TRES and
		    ! TSTWCOM arrays.

		    ! Save wafer that is being overwritten by new wafer
		    saveCompId = tres.compIds( newIndex ) 
		    saveFreeItem = tres.comp1stFreeItem( newIndex:newIndex )
		    saveIndexC = tres.compIndexC( newIndex:newIndex )

		    ! Copy in new wafer
		    tres.compIds( newIndex ) = tres.compIds( waferIndex )
		    tres.comp1stFreeItem( newIndex:newIndex ) =
     &		  	tres.comp1stFreeItem( waferIndex:waferIndex )
		    tres.compIndexC( newIndex:newIndex ) =
     &			tres.compIndexC( waferIndex:waferIndex )

		    ! Restore saved wafer to new position 
		    tres.compIds( waferIndex ) = saveCompId
		    tres.comp1stFreeItem( waferIndex:waferIndex ) = saveFreeItem
		    tres.compIndexC( waferIndex:waferIndex ) = saveIndexC

		    ! Switch the wafers in the avail list.

		    ! I do not check the status of gut_strXmatrch or the
		    ! bounds of availIndex or newAvailIndex because I trust
		    ! that the availCompIds array is identical to the 
		    ! tres.compids array.  
		    ! See "Tst_TestUtil_WaferNamesInit()"

		    call gut_strxmatch(	waferId, 
     &		       			availIndex, 
     &		       	       		tst_tstWCom_numAvailComps,
     &		       			tst_tstWCom_availCompIds )
		    tst_tstwcom_availCompIds( availIndex ) = 
     &			tres.compIds( waferIndex )

		    call gut_strxmatch(	saveCompId,
     &		  			newAvailIndex, 
     &				       	tst_tstWCom_numAvailComps,
     &					tst_tstWCom_availCompIds )
		    tst_tstwcom_availCompIds( newAvailIndex ) = 
     &			tres.compIds( newIndex )

		    ! Set new index.
	            waferIndex = newIndex 

		  elseif ( waferIndex .lt. newIndex ) then

		    ! Should not be possible since waferindex > tres.compCount
		    call gut_assert( .false. )
		    
	          endif

		  !!! Should be 0 as per fil:tresfil.def coc.  It had been
		  !!! 1 for so long, and not caused a problem...
		  tres.comp1stFreeItem( waferIndex:waferIndex ) = CHAR( 0 )
		  !!!tres.comp1stFreeItem( waferIndex:waferIndex ) = CHAR( 1 )

	        endif

	      endif ! waferIndex > tres.compCount

	    endif ! tst_tresUtil_ValidWafer ( waferIndex )

	  endif ! waferIndex > 0

	else ! waferIdLen < 1

	  waferIndex = INDEX_UNSPECIFIED
	  waferId = '?'
	  waferIdLen = 1

	endif

	aeqSsp_autoMan_waferIndex = waferIndex

	return

	end

!++ AEQSSP_AUTOMAN_ITEMID
!
	subroutine aeqSsp_autoMan_itemId( itemIndex, itemId, itemIdLen )

	implicit none

	! Given an item index, return an item ID

! Include files:

	include	'FIL:TYPDEF'
	include	'FIL:TESTCDEF'	

! Arguments:

	integer*4	itemIndex				! [R]
	! - item index

	character*(*)	itemId					! [W]
	! - returned item ID

	integer*4	itemIdLen				! [W]
	! - item ID length

! Routines Used:

	integer*4	gut_trimblks

! Author:	Mike Bergsma		4/93
!		Ab Initio Software
!
! Modifications:

! Local Variables:

! Code:

	if ( itemIndex .lt. 1 .or. itemIndex .gt. test.noTestDi ) 
     &	itemIndex = 1

	itemId = test.di(itemIndex).prompt
	itemIdLen = gut_trimblks( itemId )

	return

	end

!++ AEQSSP_AUTOMAN_SITEID
!
	subroutine aeqSsp_autoMan_siteId(	siteIndex, 
     &						siteId, 
     &						siteIdLen )

	! Given a site index, return a valid site ID

	implicit none

! Include files:

	include	'FIL:TYPDEF'
	include	'FIL:TESTCDEF'	

! Arguments:

	integer*4	siteIndex				! [R]
	! - site index

	character*(*)	siteId					! [W]
	! - site id returned

	integer*4	siteIdLen				! [W]
	! - length of siteId

! Routines Used:

	integer*4	aeqSsp_autoUtil_left_justify

! Author:	Mike Bergsma		4/93
!		Ab Initio Software
!
! Modifications:

! Local Variables:

! Code:

	if ( siteIndex .lt. 1 .or. siteIndex .gt. test.noSites ) siteIndex = 1

	write( siteId, '(I5)' ) siteIndex
	siteIdLen = aeqSsp_autoUtil_left_justify( siteId )

	return
	end

!++ AEQSSP_AUTOMAN_WAFERID
!
	subroutine aeqSsp_autoMan_waferId(	waferIndex,
     &						waferId,
     &						waferIdLen )

	! Given a wafer Index, return a valid wafer ID

	implicit none

! Include files:

	include	'FIL:TYPDEF'
	include	'FIL:FILDEF'
	include	'FIL:TRESCDEF'	

! Arguments:

	integer*4	waferIndex				! [R]
	! - wafer index

	character*(*) 	waferId					! [W]
	! - wafer ID returned

	integer*4	waferIdLen
	! - length of wafer ID

! Routines Used:

	integer*4	gut_trimblks

! Author:	Mike Bergsma		4/93
!		Ab Initio Software
!
! Modifications:
!
!	MDB	8/17/99
!	- support repeatable dcop's
!	
! Local Variables:

! Code:

!!!	if ( 	waferIndex .lt. 1 .or. 
!!!     &		waferIndex .gt. tres.compsToTest ) waferIndex = 1
	if ( 	waferIndex .lt. 1 .or. 
     &		waferIndex .gt. tres.compCount ) waferIndex = 1

	waferId = tres.compIds( waferIndex ) 
	waferIdLen = gut_trimblks( waferId )

	return
	end

!++ AEQSSP_AUTOMAN_FMTWAFERID

	subroutine aeqSsp_autoMan_fmtWaferId( lotid, waferId, waferIdLen )

	implicit none

	! Format a wafer id from a lotid and a numeric wafer id 

! Include files:

	include	'FIL:TYPDEF'
	include	'PAR:PARDEF'
	include 'PAR:LOTSTDEF'

! Arguments:

	character*(*)	lotid					! [R]
	! - the lot ID, needed if the value is just a number

	character*(*)	waferId					! [R/W]
	! - a wafer ID or a wafer number.

	integer*4	waferIdLen				! [R/W]
	! - length of wafer ID

! Routines used:

	character*12	Ssp_FmtCompId
	logical*4	gut_editInt
	integer*4	gut_trimblks

! Author:	Michael Bergsma

! Modifications:

! Local variables:

	integer*4	waferIndex,
     &			lotNumLen

! Code:

	call gut_assert( waferIdLen .gt. 0 )

	! Is the wafer id a number or a full wafer id?
	! Check if a valid number.
	if ( gut_editint(	waferId(:waferIdLen),
     &				waferIndex,
     &       			1, TYP__MAXCOMPONENTS ) ) then

	  ! If the following condition is false, then correct the calling
	  ! routine so that lotid can never be blank for a WAFER or SITE
	  ! test.
	  call gut_assert( lotid .ne. ' ' )

	  lotNumLen = INDEX( lotid, PAR_PARDEF_LOTIDSEPARATOR ) - 1
	  if ( lotNumLen .gt. 0 ) then
	    waferId = Ssp_FmtCompId( lotNumLen, lotid, waferIndex ) 
	    waferIdLen = gut_trimblks( waferId )
	  endif

	endif	

	return

	end

!++ AEQSSP_AUTOMAN_NEXT
!
	subroutine aeqSsp_autoMan_next(	firsttime,
     &					waferIndex,
     &					siteIndex,
     &					itemIndex,
     &					state )

	! A routine that finds the next data item to enter and returns
	! the indexes to that data item.

	implicit none

! Include files:

	include	'FIL:TYPDEF'
	include	'FIL:FILDEF'
	include 'PAR:LOTSTDEF'
	include	'TST:TSTDEF'	
	include	'TST:PARMDEF'	
	include	'TST:ENTDEF'	
	include 'FIL:TESTPDEF'
	include 'FIL:TRESPDEF'
	include	'TST:TSTWCOM'	

! Arguments:

	logical*4	firsttime					! [R]
	! - if firstTime is .true., then reset indexes

	integer*4	waferIndex					! [R/W]
	! - previous and next wafer index

	integer*4	siteIndex					! [R/W]
	! - previous and next site index

	integer*4	itemIndex					! [R/W]
	! - previous and next item index

	character*(*)	state						! [W]
	! - state of test
	! - NONE - nothing entered yet
	! - SOME - some data entered
	! - DOME - all data entered

! Routines Used:

! Author:	Mike Bergsma		4/93
!		Ab Initio Software
!
! Modifications:
!
! Local Variables:

	logical*4	more,
     &			inputOk,
     &			rowchg, 
     &			colchg

	integer*4	subs( TRES_X_DIMNUMVALUES )
	data		subs /0,0,0,0/

! Code:

	if ( firstTime ) then

	  tst_tstWcom_curResNum = 1
	  tst_tstWcom_NONEentered = .false.
	  tst_tstWcom_ENDentered = .false.
	  tst_tstWcom_NEXTentered = .false.
	  tst_tstWcom_VALentered = .false.
	  waferIndex = 0
	  siteIndex = 0
	  itemIndex = 0

	endif

	!===============================================================!
	! These two routines must be called before tst_etestUtil_findItem
	call tst_screenUtl_updateCurVals( rowchg, colchg )
	call tst_etestUtil_valEntered()
	!===============================================================!

	call tst_etestUtil_findItem(	more,
     &					inputOK,
     &	    				tst_tstWcom_curResNum,
     &					subs,
     &					firsttime )

	if ( tst_tstWcom_typeIndex .eq. TST__SITE ) then

	  waferIndex = subs(1) 
	  itemIndex = subs(3) 
	  siteIndex = subs(2)		

	elseif ( tst_tstWcom_typeIndex .eq. TST__2DCOMP .or.
     &   	 tst_tstWcom_typeIndex .eq. TST__PROBE ) then

	  waferIndex = subs(1) 
	  itemIndex = subs(2) 
	  siteIndex = 1

	else

	  waferIndex = 0
	  itemIndex = subs(1)
	  siteIndex = 0

	endif

	if ( tst_tstWcom_isEnterTest ) then

	  ! Enter lot data.

	  if ( more .and. inputOk ) then

	    ! Data can still be entered

	    if ( .not. tst_tstWcom_anyData ) then

	      ! No data yet	
	      state = 'NONE'

	    else

	      ! Some data has already been entered
	      state = 'SOME'

	    endif

	  else

	    ! Data can no longer be entered
	    state = 'DONE'

	  endif

	else

	  ! Correct test
	  if (	more .and. inputOk ) then

	    if ( firstTime ) then

	      state = 'NONE'

	    elseif ( 	waferIndex .le. 1 .and. 
     &		   	siteIndex .le. 1 .and. 
     &		   	itemIndex .le. 1 ) then

	      ! Return to start of data entry.
	      state = 'DONE'

	    else

	      ! Data entry in progress.
	      state = 'SOME'

	    endif

	  else

	    ! Data can no longer be entered
	    state = 'DONE'

	  endif

	endif

	return
	end

!++ AEQSSP_AUTOMAN_VALIDDATA
!
	logical*4 function aeqSsp_autoMan_validData(	value,
     &							datatype,
     &							realVal,
     &							intVal,
     &							charVal, 
     &							dataLen )

	! Converts ASCII value into 'internal' format.

	implicit none

! Include files:

	include	'TST:TSTDEF'

! Arguments:

	character*(*)		value			! [R]
	! - value to convert

	character*1		dataType		! [R]
	! - datatype to convert to: 
	! - INTEGER, REAL, DATE, CHARACTER

	real*4		realVal				! [W]
	! - value if dataType is REAL

	integer*4	intVal				! [W]
	! - value if dataType is INTEGER or DATE

	character*(*)	charVal				! [W]
	! - value if dataType is CHARACTER

	integer*4	dataLen				! [W]
	! - length of data, if CHARACTER

! Routines

	integer*4	aeqSsp_autoUtil_left_justify
	integer*4	gut_trimblks
	logical*4	gut_DateCvt_DayTimToInternal

! Function value:
!
!	- returns success or failure

! Author: 	Michael Bergsma		4/27/92

! Modifications:
!
!	17-Nov-1993	Michael Bergsma
!	- use dayTimToInternal routine to convert dateTime instead of
!	  just date.

! Local variables:

	logical*4	rejected

	integer*4	convertStrLen,
     &			dateStrLen,
     &			exponent

	character*15	convertStr
	character*20	dateStr

! Code:

	! Assume no good

	rejected = .false.

	if ( datatype .eq. TST__REALVALUE .or.
     &	     datatype .eq. TST__INTEGERVALUE ) then

	  ! Real or integer
	  dataLen = 4

	  ! Make a copy
	  convertStr = value

	  ! Numeric values get left justified.
	  convertStrLen = aeqSsp_autoUtil_left_justify( convertStr )

	  if ( convertStrLen .gt. 0 ) then

	    if ( convertStr(:convertStrLen) .eq. 'NONE' ) then

	      if ( datatype .eq. TST__REALVALUE ) then
                realVal = TST__NONEVALUE 
	      else	
                intVal = TST__NONEINTVAL
	      endif

	    elseif ( convertStr(:convertStrLen) .eq. 'MISSING' ) then

	      if ( datatype .eq. TST__REALVALUE ) then
                realVal = TST__UNKNOWNVAL
	      else	
     	        intVal = TST__UNKNOWNINTVAL
	      endif

	    else

	      ! Use FORTRAN to convert.  It does a good job.

	      ! In case there's lowercase 'e'	
	      call gut_uppercase( convertStr )

	      ! Does the string need a decimal point in order to convert?
     	      if ( INDEX( convertStr, '.' ) .eq. 0 ) then

	        exponent = INDEX( convertStr, 'E' ) 
	        if ( exponent .eq. 0 ) then

		  ! Add decimal point at end of number
	          if ( convertStrLen .lt. 15 ) then
		    convertStrLen = convertStrLen + 1
	            convertStr(convertStrLen:convertStrLen) = '.' 
		  else
		    rejected = .true.
		  endif

		else

		  ! Add decimal point just before the 'E'
	          if ( convertStrLen .lt. 15 ) then
		    convertStrLen = convertStrLen + 1
	            convertStr(exponent:) = '.'//convertStr(exponent:)
		  else
		    rejected = .true.
		  endif

		endif

	      endif

	      ! Convert
	      !!!read( convertStr(:convertStrLen), '(F14.7)', err=999 ) realVal
	      read( convertStr(:convertStrLen), '(F14.7)' ) realVal

	      if ( datatype .eq. TST__REALVALUE ) then

		! Make sure converted value is in range.
	        if ( realVal .lt. TST__MINTESTVAL .or. 
     &		     realVal .gt. TST__MAXTESTVAL ) realVal = TST__UNKNOWNVAL

	      else	

	        if ( realVal .lt. TST__MININTVAL .or. 
     &		     realVal .gt. TST__MAXINTVAL ) then

		  ! Cannot convert to integer
		  intVal = TST__UNKNOWNINTVAL
		  rejected = .true.

		else

		  ! Convert to integer, truncating in the process.
		  intVal = realVal 

		endif

	      endif	

	      if ( .false. ) then

	        ! Conversion failed if we get here.
999	        continue
	        rejected = .true.

	        if ( datatype .eq. TST__REALVALUE ) then
                  realVal = TST__UNKNOWNVAL
	        else	
     	          intVal = TST__UNKNOWNINTVAL
	        endif

 	      endif	

	    endif

	  else

	    rejected = .true.

	    if ( datatype .eq. TST__REALVALUE ) then
              realVal = TST__UNKNOWNVAL
	    else	
     	      intVal = TST__UNKNOWNINTVAL
	    endif

	  endif

	elseif ( dataType .eq. TST__DATEVALUE ) then

	  dataLen = 4

	  ! Make a copy
	  dateStr = value

	  ! Date values get left justified.
	  dateStrLen = aeqSsp_autoUtil_left_justify( dateStr )

     	  if ( dateStrLen .gt. 0 ) then

	    call gut_uppercase( dateStr )

	    if ( dateStr(:dateStrLen) .eq. 'NONE' ) then
     	      intVal = TST__NONEDATE
	    elseif ( dateStr(:dateStrLen) .eq. 'MISSING' ) then
     	      intVal = TST__NULLDATE
	    elseif ( .not. gut_DateCvt_DayTimToInternal(	
     &				dateStr(:dateStrLen), 
     &				intVal ) ) then
     	      intVal = TST__NULLDATE
	      rejected = .true.
	    endif

	  else
    	    intVal = TST__NULLDATE
	  endif

	elseif ( dataType .eq. TST__ALPHANUMVALUE ) then

	  charVal = value
	  dataLen = gut_trimblks( charVal )

	endif
	
	aeqSsp_autoMan_validData = .not. rejected

	return

	end

!++ AEQSSP_AUTOMAN_GETPARM
!
	logical*4 function aeqSsp_autoMan_getParm(	parmName,
     &							sources,
     &							parmVal )

	! Get the value 'parmVal' of a parameter given by 'parmName', 
	! substituting from 'sources'.
	! If recursive is .true., then recursive parameter substitution 
	! is allowed up to 20 iterations

	implicit none

	character*(*)	parmName				! [R]
	! - parameter to lookup

	integer*4	sources					! [R]
	! - parameter sources

	character*(*)	parmVal					! [W]
	! - parameter value returned

! Include Files:

	include	'FIL:TYPDEF'
	include	'PAR:PARDEF'
	include	'PAR:PRODSTDEF'

	include	'LOT:ACTLBUFF'	
	include	'FIL:TESTCDEF'	
	include	'FIL:TRESCDEF'	
	include 'LOT:RECPBUFF'	
	include 'FIL:HISTDEF'
	include 'FIL:LBOMDEF'
	include 'FIL:PPARDEF'

	include 'TST:TSTDEF'
	include 'TST:PARMDEF'
	include 'TST:ONEPRMREC'

! Routines used:

	integer*4	Tst_ParmUtil_GetParm
	integer*4	Tst_ParmUtil_GetOneParm

! Function value:
!
!	- returns success or failure

! Author:	Mike Bergsma		4/93
!		Ab Initio Software
!
! Modifications:
!
!	Mike Bergsma	5/25/94
!	- PROMIS _getParm suddenly does recursion!!  See comment below.
!
!	Mike Bergsma	9/20/94
!	- always do recursion, unless parameter starts with $AUTO_INIT
!	- use PROMIS routine now ( retire logic for this one )
!
!	Mike Bergsma	3/19/96
!	- was getting subscript out of range error when checking
!	  the first 14 characters of the passed parameter which does not have
!	  14 characters - that was very bad style - sorry.

! Local Variables:

	logical*4	okParm

	integer*4	status,
     &			origin

	record /tst_onePrmRec/ parm

! Code

	! Assume no substution or bad parameter
	okParm = .false.

	! Initialize parameter lookup tables (partial Initialization)
	call Tst_ParmUtil_TstPrmComInit( .false. )

	! Initialize return values
	status = TST__NOPARMVAL
	origin = TST__PARMSOURCENULL

	! Fetch the parameter.

	if ( LEN( parmName ) .ge. 14 .and.
     &	     ( parmName(1:14) .eq. '$AUTO_INITPARM' .or.
     &	       parmName(1:14) .eq. '$AUTO_INITRECP' .or.
     &	       parmName(1:14) .eq. '$AUTO_INITDCOP' .or.
     &	       parmName(1:14) .eq. '$AUTO_SOFTPARM' .or.
     &	       parmName(1:14) .eq. '$AUTO_HARDPARM' ) ) then

	  status = Tst_ParmUtil_GetOneParm(
     &					parmName, 		! $<parm>
     &			        	sources,
     &					TST__PARMSOURCENULL,	! Destination
     &				       	actl, hist, tres, lbom, 
     &					TST__NULLCOMPONENT, 
     &					ppar, recp, test,
     &					origin,			! Origin
     &					parm )			! Value
	else
	  status = Tst_ParmUtil_GetParm(
     &					parmName, 		! $<parm>
     &			        	sources,
     &					TST__PARMSOURCENULL,	! Destination
     &				       	actl, hist, tres, lbom, 
     &					TST__NULLCOMPONENT, 
     &					ppar, recp, test,
     &					origin,			! Origin
     &					parm )			! Value

	endif

	if ( status ) then

	  ! Parameter was substituted
	  okParm = .true.

	  ! Copy value	
	  parmVal = parm.parmVal

	else

	  parmVal = parmName

	endif

	aeqSsp_autoMan_getParm = okParm
	
	return
	end
