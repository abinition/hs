!*****************************************************************************!
!                    AUTOMAN Software Source Code.                            !
!                                                                             !
!          ***** Copyright: (c) 1993 Ab Initio Software                       !
!          ***** All rights reserved - Licensed Material.                     !
!          ***** Program property of Ab Initio Software                       !
!                                                                             !
!*****************************************************************************!
!++ AUTOMAN3.FOR

! The AUTOmation MANagement routines are distributed into three modules:
!
!	automan.for		! Automation communication routines
!	automan2.for		! General purpose automation utilities
!	automan3.for		! Automation User Interface utilities
!	automan4.for		! Automation Database Utilities

! Externally Callable Routines:

! For Internal Use Only:

!	aeqSsp_automan_setupScreen	! Draw the interface on the screen
!	aeqSsp_automan_pagewait 	! Perform the screen pagewait
!	aeqSsp_automan_resetScreen	! Erase and reset the screen 
!	aeqSsp_autoMan_dcOpHeader	! Data collection output header
!	aeqSsp_autoMan_tkinHeader	! Trackin output header
!	aeqSsp_autoMan_batchHeader	! Batch Trackin output header
!	aeqSsp_autoMan_outMessage	! Output message text
!	aeqSsp_automan_outHotKey 	! Output hot key
!	aeqSsp_autoMan_command		! Prompt for a command

! Produced by:	Ab Initio Software

! Author:	Michael Bergsma 	1993

! Modifications:
!
!     $Log: automan3.for,v $
!     Revision 1.4  2009-08-19 14:30:47  bergsma
!     Automan Upper Window message truncation
!
!     Revision 1.3  2005-03-30 04:05:02  bergsma
!     HS needs to externalize backslash, not ignore it.  Thus, AUTOMAN must be
!     modified to interpret \\s, \\u, and \\g.
!
!     Revision 1.2  2003/01/16 14:31:47  bergsma
!     Header display for batch lot was failing (subscript out of range) when
!     the number of lots exceeded 10 or more, because MAX_BATCH_LOTS was increased
!     from 10 to 20.
!
!
!-

!++ AEQSSP_AUTOMAN_SETUPSCREEN
!
	subroutine aeqSsp_automan_setupScreen()

!	Prepare the screen for the automation user interface.

	implicit none

! Include files:

	include	'AEQ_SSP:AUTODEF'
	include	'TUT:TUTDEF'

! Arguments:

! Routines Used:

! Author:	Mike Bergsma		4/93
!		Ab Initio Software
!
! Modifications:
! 
!	Mike Bergsma	4/10/94
!	- new screen format
!
!	Mike Bergsma	1/10/95
!	- Version 5.0
!	- new screen format, no debug section.
!
	integer*4	i

! Code:

	! Draw the box.
	call tut_posOut(TOP_LINE_ROW, 1,
     &			TUT_T_LINEFONT//
     &			'lqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq'//
     &			'qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq'//
     &			TUT_T_STDFONT//AUTOMAN_VERSION//
     &			TUT_T_LINEFONT//'qk'//TUT_T_STDFONT )

	call tut_posOut(MESSAGE_START_ROW, 1,
     &			TUT_T_LINEFONT//
     &			'x'//TUT_T_STDFONT//
     &			'                                      '//
     &			'                                       '//
     &			TUT_T_LINEFONT//'x' )

	do i=1,MAX_MESSAGE_LINES-1

	  call tut_posOut(MESSAGE_START_ROW+i, 1,
     &			TUT_T_LINEFONT//
     &			'x'//TUT_T_STDFONT//
     &			'                                      '//
     &			'                                       '//
     &			TUT_T_LINEFONT//'x' )

	enddo

	call tut_posOut(SECOND_LINE_ROW, 1,
     &			TUT_T_LINEFONT//
     &			'tqqqqqqqqqqwqqqqqqqqqqwqqqqqqqqqqwqqqqq'//
     &			'qqqqqwqqqqqqqqqqwqqqqqqqqqqwqqqqqqqqqqqu'//
     &			TUT_T_STDFONT )

	call tut_posOut(COMMAND_ROW, 1,
     &			TUT_T_LINEFONT//
     &			'x'//TUT_T_STDFONT//'          '//TUT_T_LINEFONT//
     &			'x'//TUT_T_STDFONT//'          '//TUT_T_LINEFONT//
     &			'x'//TUT_T_STDFONT//'          '//TUT_T_LINEFONT//
     &			'x'//TUT_T_STDFONT//'          '//TUT_T_LINEFONT//
     &			'x'//TUT_T_STDFONT//'          '//TUT_T_LINEFONT//
     &			'x'//TUT_T_STDFONT//'          '//TUT_T_LINEFONT//
     &			'x'//TUT_T_STDFONT//'          '//TUT_T_LINEFONT//
     &			' x' )

	call tut_posOut(THIRD_LINE_ROW, 1,
     &			TUT_T_LINEFONT//
     &			'tqqqqqqqqqqvqqqqqqqqqqvqqqqqqqqqqvqqqqq'//
     &			'qqqqqvqqqqqqqqqqvqqqqqqqqqqvqqqqqqqqqqqu'//
     &			TUT_T_STDFONT )

	call tut_posOut(MESSAGE2_START_ROW, 1,
     &			TUT_T_LINEFONT//
     &			'x'//TUT_T_STDFONT//
     &			'                                      '//
     &			'                                       '//
     &			TUT_T_LINEFONT//'x' )

	do i=1,MAX_MESSAGE2_LINES-1

	  call tut_posOut(MESSAGE2_START_ROW+i, 1,
     &			TUT_T_LINEFONT//
     &			'x'//TUT_T_STDFONT//
     &			'                                      '//
     &			'                                       '//
     &			TUT_T_LINEFONT//'x' )

	enddo

	  call tut_posOut(BOTTOM_LINE_ROW, 1,
     &			TUT_T_LINEFONT//
     &			'mqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq'//
     &			'qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqj'//
     &			TUT_T_STDFONT )

	end

!++ AEQSSP_AUTOMAN_PAGEWAIT
!
	subroutine aeqSsp_automan_pagewait (	functionStatus, 
     &						connectStatus,
     &						sessionStatus,
     &						optionalText3 )

!	Set up the screen for the pagewait function

	implicit none

! Include files:

	include	'AEQ_SSP:AUTODEF'
	include	'TUT:TUTDEF'

! Arguments:

	logical*4	functionStatus			! [R]
	! - influence which message is seen

	logical*4	connectStatus			! [R]
	! - influence which message is seen

	logical*4	sessionStatus			! [R]
	! - influence which message is seen

	character*(*)	optionalText3			! [R]
	! - optional text3

! Routines Used:

! Author:	Mike Bergsma		4/93
!		Ab Initio Software
!
! Modifications:
!
!	12/20/95	Mike Bergsma
!	- add additional message
!
!	10/5/98		Mike Bergsma
!	- make the text an optional variable
!
! Local Variables:
!
	character*73	text3

! Code:

	call tut_posOut(SECOND_LINE_ROW, 1,
     &			TUT_T_LINEFONT//
     &			'tqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq'//
     &			'qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqu'//
     &			TUT_T_STDFONT )

	if ( optionalText3 .ne. ' ' ) then

	  text3 = optionalText3

	elseif ( functionStatus ) then

	  ! Automation connected, function succeeded

	  text3 = ' Transaction success! Press SPACEBAR/RETURN'//
     &		  ' to complete PROMIS function. '

	elseif ( connectStatus ) then

	  ! Automation connected, but function failed

	  text3 = '  Transaction denied! Press SPACEBAR/RETURN'//
     &		  ' to cancel PROMIS function.   '

	elseif ( sessionStatus ) then

	  ! Automation not connected, OPTION_AUTO = WARN

	  text3 = ' Transaction warning! Press SPACEBAR/RETURN'//
     &		  ' and retry or press [M]anual. '

	else

	  ! Automation not connected, OPTION_AUTO = HALT

	  text3 = '   Automation halted! Press SPACEBAR/RETURN'//
     &		  ' to cancel PROMIS function.   '

	endif

	call tut_posOut(
     &		COMMAND_ROW, 
     &		1,
     &		TUT_T_LINEFONT//'x  '//TUT_T_STDFONT//
     &		TUT_T_REVERSE//TUT_T_BLINK//TUT_T_BOLD//
     &		text3//
     &		TUT_T_STANDARD//TUT_T_LINEFONT//'  x' )

	call tut_posOut(THIRD_LINE_ROW, 1,
     &			TUT_T_LINEFONT//
     &			'tqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq'//
     &			'qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqu'//
     &			TUT_T_STDFONT )

	return
	end

!++ AEQSSP_AUTOMAN_RESETSCREEN
!
	subroutine aeqSsp_automan_resetScreen()

!	Reset the screen prior to returning to PROMIS

	implicit none

! Include files:

	include	'AEQ_SSP:AUTODEF'
	include	'TUT:TUTDEF'
	include	'TUT:TUTCOM'

! Arguments:

! Routines Used:

! Author:	Mike Bergsma		4/93
!		Ab Initio Software
!
! Modifications:
!
!	Mike Bergsma	1/10/95
!	- pagewait not longer performed
!
! Code:

	call Tut_SetPgWait( .false. )	! Temporarily no pagewait
	call Tut_Line_Begin		! Output a new line
	call Tut_Util_SetAttr(char(0))	! Standard attributes
	call Tut_SetScroll(-1)		! No scrolling region
	call tut_clear()		! Clear the screen.
	call tut_refresh( .false. )	! Update bottom row banner
	call Tut_ReStrWait ()		! Enable paging if previously set

	end

!++ AEQSSP_AUTOMAN_DCOPHEADER
!
	subroutine aeqSsp_autoMan_dcopHeader( )

! 	Since data collection already has a header, this routine serves only
!	to set the scrolling region.

	implicit none

! Include files:

	include	'AEQ_SSP:AUTODEF'

! Arguments:

! Routines Used:

! Author:	Mike Bergsma		10/94
!		Ab Initio Software
!
! Modifications:

! Local Variables:

! Code:
	call Tut_SetNoWait()
	! Set a new scrolling area.
	call tut_setscroll( BOTTOM_LINE_ROW+1 )	
	call tut_refresh( .false. )

	end

!++ AEQSSP_AUTOMAN_TKINHEADER
!
	subroutine aeqSsp_autoMan_tkinHeader(	lotid,
     &						eqpid,
     &						part,
     &						prcd,
     &						inst,
     &						stage,
     &						recipe,
     &						descr,
     &						user )

! 	Output some header information.
!
!	Currently used only by TRACKIN automation to give the same look and
!	feel as the PROMIS header displayed used when doing DCOP automation.
!
!------------------------------------------------------------------------------
!Descr : 123456789.123456789.123456789.123456789.123456789.123456789.12
!Lot Id: 12345.789012                     Part: foobarxxxxxxx                
!Inst #: 123                              Prcd: fooballs                     
!Recipe: KENT1.01                         Eqpt: 1CD1         User: KENT     
!Stage : 1234567890                       Date: 26-MAY-1988  Time: 17:00:00
!------------------------------------------------------------------------------

	implicit none

! Include files:

	include	'AEQ_SSP:AUTODEF'
	include	'TUT:TUTDEF'

! Arguments:

	character*(*)	lotid					! [R]
	! PROMIS Lot ID 

	character*(*)	eqpid					! [R]
	! PROMIS Equipment ID 

	character*(*)	part					! [R]
	! Part id

	character*(*)	prcd					! [R]
	! Procedure id

	character*(*)	inst					! [R]
	! Instruction number

	character*(*)	stage					! [R]
	! Current Stage

	character*(*)	recipe					! [R]
	! Recipe id

	character*(*)	descr					! [R]
	! Recipe title

	character*(*)	user					! [R]
	! User name

! Routines Used:

	character*20	gut_cnvDayTim
	integer*4	aeqSsp_autoUtil_collapse

! Author:	Mike Bergsma		4/93
!		Ab Initio Software
!
!	Mike Bergsma		1/2/95
!	- add log file support (aeqSsp_automan_log)

!
! Modifications:

! Local Variables:

	integer*4	dateTime
	character*80	line
	character*40	leftLine,rightLine
	character*20	dateTimeStr 
	character*32	partId,prcdID

! Code:

	call Tut_SetNoWait()
	call tut_clear()
	! Set a new scrolling area.
	call tut_setscroll( BOTTOM_LINE_ROW+1 )	
	call tut_refresh( .false. )

	line = 'Descr : '//descr
	call tut_posout( HEADER_ROW_1, 1, 
     &			 TUT_T_REVERSE//line//TUT_T_STANDARD )
	call aeqSsp_automan_log ( line )

	leftLine = 'Lot Id: '//lotid
	partId = part
	call aeqSsp_autoUtil_collapse( partId )
	rightLine = 'Part: '//partId
	call tut_posout( HEADER_ROW_2, 1, 
     &			 TUT_T_REVERSE//leftLine//rightLine//TUT_T_STANDARD )
	call aeqSsp_automan_log ( leftLine//rightLine )

	leftLine = 'Inst #: '//inst
	prcdId = prcd
	call aeqSsp_autoUtil_collapse( prcdId )
	rightLine = 'Prcd: '//prcdId
	call tut_posout( HEADER_ROW_3, 1, 
     &			 TUT_T_REVERSE//leftLine//rightLine//TUT_T_STANDARD )
	call aeqSsp_automan_log ( leftLine//rightLine )

	leftLine = 'Recipe: '//recipe
	rightLine = 'Eqpt: '//eqpid//'      User: '//user
	call tut_posout( HEADER_ROW_4, 1, 
     &			 TUT_T_REVERSE//leftLine//rightLine//TUT_T_STANDARD )
	call aeqSsp_automan_log ( leftLine//rightLine )

	call gut_currentim( dateTime )
	dateTimeStr = gut_cnvDayTim( dateTime )
	leftLine = 'Stage : '//stage
	rightLine = 'Date: '//dateTimeStr(1:12)//'  Time: '//dateTimeStr(13:20)
	call tut_posout( HEADER_ROW_5, 1, 
     &			 TUT_T_REVERSE//leftLine//rightLine//TUT_T_STANDARD )
	call aeqSsp_automan_log ( leftLine//rightLine )


	end

!++ AEQSSP_AUTOMAN_BATCHHEADER
!
	subroutine aeqSsp_autoMan_batchHeader(	numLots,
     &						lots, 
     &						eqpid,
     &						eqpType,
     &						recipe,
     &     					descr,
     &						user )

! 	Output some header information.
!
!	Currently used only by TRACKIN automation to give the same look and
!	feel as the PROMIS header displayed used when doing DCOP automation.
!
!------------------------------------------------------------------------------
!Descr : 123456789.123456789.123456789.123456789.123456789.123456789.12
!Recipe: KENT1.01                        EqpId: 1CD1         EqpType: KENT     
!User  : 1234567890                      Date: 26-MAY-1988   Time: 17:00:00
!Batch : 12345.789012 12345.789012 12345.789012 12345.789012 12345.789012 
!        12345.789012 12345.789012 12345.789012 12345.789012 12345.789012 
!------------------------------------------------------------------------------

	implicit none

! Include files:

	include	'FIL:TYPDEF'
	include	'TUT:TUTDEF'
	include	'AEQ_SSP:AUTODEF'

! Arguments:

	integer*4	numLots					! [R]
	! - number of batch lots

	character*(*)	lots( MAX_BATCH_LOTS )			! [R]
	! - list of lots in batch

	character*(*)	eqpid					! [R]
	! - PROMIS Equipment ID 

	character*(*)	eqpType					! [R]
	! - PROMIS Equipment Type 

	character*(*)	recipe					! [R]
	! - recipe id

	character*(*)	descr					! [R]
	! - recipe description

	character*(*)	user					! [R]
	! - user name

! Routines Used:

	character*20	gut_cnvDayTim

! Author:	Mike Bergsma		4/93
!		Ab Initio Software
!
! Modifications:
!
!	Mike Bergsma		1/2/95
!	- add log file support (aeqSsp_automan_log)

! Local Variables:

	integer*4	i,j,dateTime
	character*80	line
	character*40	leftLine,rightLine
	character*20	dateTimeStr 

! Code:

	call Tut_SetNoWait()
	call tut_clear()
	! Set a new scrolling area.
	call tut_setscroll( BOTTOM_LINE_ROW+1 )	
	call tut_refresh( .false. )

	!Descr : 123456789.123456789.123456789.123456789.123456789.123456789.12
	line = 'Descr : '//descr//' '
	call tut_posout( HEADER_ROW_1, 1, 
     &			 TUT_T_REVERSE//line//TUT_T_STANDARD )
	call aeqSsp_automan_log ( line )

	!Recipe: KENT1.01               EqpId: 1CD1         EqpType: KENT     
	leftLine = 'Recipe: '//recipe//' '
	rightLine = 'EqpId: '//eqpid//'     EqpType: '//eqpType//' '
	call tut_posout( HEADER_ROW_2, 1, 
     &			 TUT_T_REVERSE//leftLine//rightLine//TUT_T_STANDARD )
	call aeqSsp_automan_log ( leftLine//rightLine )

	!User  : 1234567890             Date: 26-MAY-1988   Time: 17:00:00
	call gut_currentim( dateTime )
	dateTimeStr = gut_cnvDayTim( dateTime )
	leftLine = 'User  : '//user//' '
	rightLine = 'Date: '//dateTimeStr(1:12)//'  Time: '//dateTimeStr(13:20)
	call tut_posout( HEADER_ROW_3, 1, 
     &			 TUT_T_REVERSE//leftLine//rightLine//TUT_T_STANDARD )
	call aeqSsp_automan_log ( leftLine//rightLine )


	!Batch : 12345.789012 12345.789012 12345.789012 12345.789012 
	!        12345.789012 12345.789012 12345.789012 12345.789012 
	line = 'Batch : '
	do i=1,MIN( numLots, 5 )
	  j = ((i-1)*(TYP__LOTIDSZ+1)) + 9
	  line(j:) = lots(i)
	enddo
	call tut_posout( HEADER_ROW_4, 1, 
     &			 TUT_T_REVERSE//line//TUT_T_STANDARD )
	call aeqSsp_automan_log ( line )

	line = ' '
	do i=6,MIN( numLots, 10 )
	  j = ((i-6)*(TYP__LOTIDSZ+1)) + 9
	  line(j:) = lots(i)
	enddo
	call tut_posout( HEADER_ROW_5, 1, 
     &			 TUT_T_REVERSE//line//TUT_T_STANDARD )
	call aeqSsp_automan_log ( line )

	line = ' '
	do i=11,MIN( numLots, 15 )
	  j = ((i-11)*(TYP__LOTIDSZ+1)) + 9
	  line(j:) = lots(i)
	enddo
	call tut_posout( HEADER_ROW_5, 1, 
     &			 TUT_T_REVERSE//line//TUT_T_STANDARD )
	call aeqSsp_automan_log ( line )

	line = ' '
	do i=16,MIN( numLots, MAX_BATCH_LOTS )
	  j = ((i-16)*(TYP__LOTIDSZ+1)) + 9
	  line(j:) = lots(i)
	enddo
	call tut_posout( HEADER_ROW_5, 1, 
     &			 TUT_T_REVERSE//line//TUT_T_STANDARD )
	call aeqSsp_automan_log ( line )

	return

	end

!++ AEQSSP_AUTOMAN_OUTHOTKEY
!
	subroutine aeqSsp_automan_outHotKey (	position, 
     &						hotKeyName, 
     &						isEnabled )

! 	Displays hot key at position specified.

	implicit none

! Include files:

	include	'AEQ_SSP:AUTODEF'
	include	'TUT:TUTDEF'

! Arguments:

	integer*4	position				! [R]
	! - allowed values are 1 to 7

	character*(*)	hotkeyname				! [R]
	! - maximum length is 8 characters

	logical*4	isEnabled				! [R]
	! - display or erase

! Routines Used:

	integer*4	gut_trimblks

! Author:	Mike Bergsma		4/93
!		Ab Initio Software
!
! Modifications:
!
! Local Variables:

	integer*4	nameLen,
     &			column,
     &			offset,
     &			keyNameLen

	character*8	keyName
	character*20	name

! Code:

	! Ignore invalid positions
	if ( position .lt. 1 .or. position .gt. 7 ) return

	! Positions 3 14 25 36 47 58 69
	column = ((position-1)*11) + 3

	name = ' '
	keyName = hotKeyName

	if ( isEnabled ) then

	  ! Center name.  Position i is column start
	  keyNameLen = gut_trimblks ( keyName ) 
	  offset = ( 8 - keyNameLen ) / 2

	  if ( keyName(1:1) .eq. '^' ) then

	    ! Bold the ^ and the next character
	    name(offset+1:) = TUT_T_BOLD//keyName(1:2)//TUT_T_STANDARD//keyName(3:)

	  elseif ( keyName(1:1) .eq. ' ' ) then

	    ! Bold and flash a diamond character
	    name(offset+1:) = 	TUT_T_BOLD//TUT_T_BLINK//
     &			 	TUT_T_LINEFONT//'`'//TUT_T_STDFONT//
     &				TUT_T_STANDARD//keyName(2:)

	  else

	    ! Bold the first character	
	    name(offset+1:) = TUT_T_BOLD//keyName(1:1)//TUT_T_STANDARD//keyName(2:)

	  endif

	else

	  ! Erase 
	  name = TUT_T_STANDARD//'        '//TUT_T_STANDARD
	  keyNameLen = 8
	  offset = 0

	endif

	nameLen = gut_trimblks ( name ) + ( 8 - keyNameLen - offset )
	call tut_posout(	COMMAND_ROW, 
     &				column, 
     &				name(:nameLen) )

	return
	end

!++ AEQSSP_AUTOMAN_OUTMESSAGE
!
	subroutine aeqSsp_autoMan_outMessage( message, isMessage2 )

! 	Outputs a text message in the message area.  Automatically 
!	wraps words to the next line if they would be truncated.  Recognizes
!	some backslash control sequences.

	implicit none

! Include files:

	include	'AEQ_SSP:AUTODEF'
	include	'TUT:TUTDEF'

! Arguments:

	character*(*)	message					! [R]
	! - text message to display

	logical*4	isMessage2				! [R]
	! - if .true., message is displayed in the second
	!   message area.

! Routines Used:

	integer*4	gut_trimblks

! Author:	Mike Bergsma		4/93
!		Ab Initio Software
!
! Modifications:
!
! 	Mike Bergsma		8/30/94
!	- no longer do we flash if '%' is the first character of the line
!
! 	Mike Bergsma		10/3/94
!	- erase entire screen prior to outputting tokens
!
!	Mike Bergsma		1/2/95
!	- add log file support (aeqSsp_automan_log)
!
!	Mike Bergsma		7/12/95
!	- \n at start of message didn't work
!	- long tokens were not being wrapped properly
!	- too many tokens was going off the end of the screen
!
!	Mike Bergsma		8/22/95
!	- rewrite it - too many problems...
!
!	Mike Bergsma		3/26/96
!	- subscript out of range error when logging message of zero length
!
!	Mike Bergsma		4/25/96
!	- add support for second message (signal) area

! Local Variables:

	parameter	RENDITION_STANDARD	= 0
	parameter	RENDITION_BOLD 		= 1
	parameter	RENDITION_REVERSE	= 2
	parameter	RENDITION_FLASH		= 4
	parameter	RENDITION_UNDERSCORE	= 8

	integer*4	i,j,k,m,
     &			row,
     &			msgStartRow,
     &			msgEndRow,
     &			lineLen,
     &			bells,
     &			save_i,
     &			save_j,
     &			save_m,
     &			messageLen

	logical*4	newLine

	character*1	modifier

	character*(MESSAGE_LINE_LEN*2) 	line

! Code:

	! Setup for message area 1 or 2
	if ( isMessage2 ) then
	  msgStartRow = MESSAGE2_START_ROW
	  msgEndRow = MESSAGE2_END_ROW
	else
	  msgStartRow = MESSAGE_START_ROW
	  msgEndRow = MESSAGE_END_ROW
	endif

	! Replace all backslash sequences with the corresponding
	! control sequences
	!
	!	\n 	- newline
	!	\g	- bell
	!	\s	- standard rendition
	! 	\b	- bold rendition
	! 	\f	- flash rendition
	!	\r	- reverse rendition
	!	\u	- underline rendition
	!

	! Initialize
	messageLen = gut_trimblks ( message )

	! Erase the screen
	line = ' '
	do i=msgStartRow,msgEndRow
	  call tut_posout( i, MESSAGE_START_COL, line(:MESSAGE_LINE_LEN) )
	enddo

	! Scan through message and build output lines one at a time.
	! Keep track of newlines and rendition sequences.

	i = 1		! Pointer to input message
	j = 1		! Pointer to current output line
	k = 0		! # of visible chars in current output line
	bells = 0	! # of bells to ring
	m = RENDITION_STANDARD		! Current rendition mask.
	row = msgStartRow ! Current row
	newLine = .false.
	save_i = 0
	save_j = 0
	save_m = 0

	do while ( i .le. messageLen .and. row .le. msgEndRow ) 

      if ( message(i:i) .ne. '\' .or. i .eq. messageLen ) then

	    ! No translation. Copy to current line
	    line(j:j) = message(i:i)
	    if ( line(j:j) .eq. ' ' ) then
	      save_i = i
	      save_j = j
	      save_m = m
	    endif
	    j = j + 1
	    k = k + 1
	
      else

	    ! Modifier found - get translation
        i = i + 1	
	    modifier = message(i:i)

	    if ( modifier .eq. '\' ) then

		  ! Check for \\s and \\u and \\g
		  if ( i + 1 .le. messageLen ) then
			modifier = message(i+1:i+1)
		  endif

		  if ( modifier .eq. 's' .or. modifier .eq. 'u' .or. modifier .eq. 'g' ) then

			! Ignore this \\ sequence.  They are not supported
			! Hyperscript escape sequences and so turn up as 
			! \\s \\u and \\g
			! Subtracting from will advance to the next \
			i = i - 1

		  else

			! '\\' converts to '\'
			line(j:j) = '\'
			j = j + 1
			k = k + 1
		  endif

	    elseif ( modifier .eq. 'n' ) then
               
	      ! '\n' converts to newline
	      newLine = .true.

	    elseif ( modifier .eq. 'g' ) then
               
	      ! '\g' converts to BELL
	      bells = bells + 1
	       	
	    elseif ( modifier .eq. 's' ) then

	      ! '\s' converts to standard rendition
	      line(j:j+1) = TUT_T_STANDARD   
	      j = j + 2
	      m = 0
		
	    elseif ( modifier .eq. 'b' ) then

	      ! '\b' converts to bold
	      line(j:j+1) = TUT_T_BOLD	   
	      j = j + 2
	      m = ( m .or. RENDITION_BOLD )
		
	    elseif ( modifier .eq. 'f' ) then

	      ! '\f' converts to flash
	      line(j:j+1) = TUT_T_BLINK	   
	      j = j + 2
	      m = ( m .or. RENDITION_FLASH )
		
	    elseif ( modifier .eq. 'r' ) then

	      ! '\r' converts to reverse
	      line(j:j+1) = TUT_T_REVERSE	   
	      j = j + 2
	      m = ( m .or. RENDITION_REVERSE )
		
	    elseif ( modifier .eq. 'u' ) then

	      ! '\u' converts to underline
	      line(j:j+1) = TUT_T_UNDERSCORE 
	      j = j + 2
	      m = ( m .or. RENDITION_UNDERSCORE )

	    endif	

	  endif	

	  if ( k .gt. MESSAGE_LINE_LEN ) then

	    ! Past the end of the line. 
	    if ( save_i .gt. 0 ) then

	      ! Restore back to the last whitespace
	      i = save_i
	      j = save_j
	      m = save_m
	      line(j+1:) = ' ' 

	    else

	      ! Back off one character
	      i = i - 1
	      line(j-1:) = ' ' 
	
	    endif

	    newLine = .true.

	  endif

	  if ( newLine ) then

	    ! Output line and start another.

	    ! Cancel rendition
	    if ( m .ne. RENDITION_STANDARD ) then

	      line(j:j+1) = TUT_T_STANDARD   
	      j = j + 2

	    endif

	    ! Write out current line 
	    lineLen = gut_trimblks ( line )
	    if ( lineLen .gt. 0 ) 
     &	      call tut_posout( row, MESSAGE_START_COL, line(:lineLen) )
	    row = row + 1
	    line = ' '	
	    j = 1
	    k = 0
	    save_i = 0
	    save_j = 0
	    save_m = 0
	    newLine = .false.

	    if ( m .ne. RENDITION_STANDARD ) then

	      ! Continue rendition on next line
	      if ( ( m .and. RENDITION_BOLD ) .ne. 0 ) then
	        line(j:j+1) = TUT_T_BOLD
	        j = j + 2
	      endif	
	      if ( ( m .and. RENDITION_REVERSE ) .ne. 0 ) then
	        line(j:j+1) = TUT_T_REVERSE
	        j = j + 2
	      endif	
	      if ( ( m .and. RENDITION_FLASH ) .ne. 0 ) then
	        line(j:j+1) = TUT_T_BLINK
	        j = j + 2
	      endif	
	      if ( ( m .and. RENDITION_UNDERSCORE ) .ne. 0 ) then
	        line(j:j+1) = TUT_T_UNDERSCORE
	        j = j + 2
	      endif	

	    endif

	  endif

	  ! Advance to next character in message
	  i = i + 1

	enddo

	! Write out any remaining tokens.
	lineLen = gut_trimblks ( line )
	if ( lineLen .gt. 0 ) 
     &	  call tut_posout ( row, MESSAGE_START_COL, line(:lineLen) )

	! Ring the bells
	do i=1,bells
	  call tut_beep()
	enddo

	! Log the message
	if ( messageLen .gt. 0 ) 
     &		call aeqSsp_automan_log ( message(:messageLen) )


	end

!++ AEQSSP_AUTOMAN_COMMAND

	integer*4 function aeqSsp_autoMan_command()

	implicit none

	! Prompt for a single keystoke "command" from the user.
	!

! Include files:

	include '($IODEF)/NOLIST'	!I/O code definitions
	include	'TUT:TUTDEF'
	include	'TUT:TUTCOM'
	include 'CFC:EFNDEF'
	include	'AEQ_SSP:AUTODEF'

! Arguments:

! Function value:
!
!	- returns integer representation of character pressed

! Routines Used:

	integer*4 	sys$qiow

! Author:	Mike Bergsma		4/93
!		Ab Initio Software
!
! Modifications:
!
!	17-Nov-1993	Michael Bergsma
!	- don't print SYSTEM-F-ABORT message via aeqSsp_automan_check
!	  when I/O is cancelled.
!
!	Mike Bergsma	1/10/95
!	- simply return character key.
!
!	Mike Bergsma	6/2/99
!	- return -1 if this is not a terminal

! Local Variables:

	integer*4	status
	integer*2	iosb(4)
	character*1	key

	byte terminators(32)		!Terminator mask
     &	  /32*'FF'x/			!  everything allowed
	character*32 termc		! ..equivalent char string
	equivalence (terminators,termc)

	integer*4	termd(2)	! descriptor for above	
	integer*2	terml(4)	! length field for termd
     &			/32,0,0,0/	! it's always the same
	equivalence (termd,terml)				

! Code:

	key = CHAR ( 0 ) 
	if ( tut_termtype .ne. TUT_K_MAILBOX .and. 
     & 	     tut_chan .ne. -1 .and. tut_chan .ne. 0 ) then

	  ! init descriptor with address of termination mask
	  termd(2) = %loc(termc)

	  status = sys$qiow(	%val(EFN__TUTREAD),     ! Event flag
     &	    			%val(tut_chan),		! I/O channel
     &	    			%val(IO$_READVBLK+	! Read
     &				IO$M_NOECHO +		!  - noecho
     &				IO$M_NOFILTR),		!  - pass ^R, ^U, DEL
     &	    			iosb,			! I/O status block
     &	    			,,			! AST (none)
     &	    			%ref(key),		! P1: put it here
     &	    			%val(1),		! P2: buffer size
     &	    			,			! P3: No timeout
     &	    			termd,			! P4: Terminator block
     &	    			,)			! P5 & P6

	  if ( .not. status .or. .not. iosb(1) ) then

	    ! Fallout
	    aeqSsp_autoMan_command = 0

	  else

	    ! Always uppercase the key.
	    call gut_uppercase ( key )
	    aeqSsp_autoMan_command = ICHAR ( key )
	  
	  endif

	else

	  ! Automan running as detached process
	  ! Fallout
	  aeqSsp_autoMan_command = -1

	endif

	return

	end
