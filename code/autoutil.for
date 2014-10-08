!*****************************************************************************!
!                    AUTOSERVER Software Source Code.                         !
!                                                                             !
!          ***** Copyright: (c) 1993 Ab Initio Software                       !
!          ***** All rights reserved - Licensed Material.                     !
!          ***** Program property of Ab Initio Software                       !
!                                                                             !
!*****************************************************************************!
!++ AUTOUTIL.FOR

! 	Utilities to support AUTOSERVER.FOR and AUTOCLIENT.FOR

! Externally Callable Routines:

!	aeqSsp_autoUtil_parse		- parses message into components
!	aeqSsp_autoUtil_parseCase	- parses case sensitive
!	aeqSsp_autoUtil_unparse		- unparses message from components
!	aeqSsp_autoUtil_putAddress	- creates address
!	aeqSsp_autoUtil_getAddress	- determines target value from address
!	aeqSsp_autoUtil_translate	- translates AUTO_<target> logical
!	aeqSsp_autoUtil_strip		- strips token of undesirable chars
!	aeqSsp_autoUtil_collapse	- strips token of spaces 
!	aeqSsp_autoUtil_left_justify	- left justifies a token
!	aeqSsp_autoUtil_check		- status/iosb check
!	aeqSsp_autoUtil_error		- FORTRAN ios check
!	aeqSsp_autoUtil_logMessage	- log a message to unit specified
!	aeqSsp_autoUtil_log		- log specifically to SYS$OUTPUT
!	aeqSsp_autoUtil_appendLog	- append a message to a log file
!	aeqSsp_autoUtil_openLog		- open a log file 
!	aeqSsp_autoUtil_closeLog	- close a log file
!	aeqSsp_autoUtil_ghostBuster	- removes ghost characters
!	aeqSsp_autoUtil_initDTcontext	- init context for dateTime
!	aeqSsp_autoUtil_timeStamp 	- get current time in ascii 
!	aeqSsp_autoUtil_dateString	- get specified time in ascii 
!     aeqSsp_autoUtil_dateString2   - as above, but with PROMIS internal date
!	aeqSsp_autoUtil_node		- get node name
!	aeqssp_autoutil_match 		- wildcard matching with '*'

! For Internal Use Only:

!	None

! Produced by:	Ab Initio Software

! Author:	Michael Bergsma 	1993

! Modifications:
!
!	Mike Bergsma		4/18/94
!
!	- added aeqSsp_autoUtil_appendFile
!	- added aeqSsp_autoUtil_getAddress
!	- added aeqSsp_autoUtil_translate
!
!  $Log: autoutil.for,v $
!  Revision 1.2  2003-01-16 14:32:07  bergsma
!  V3.1.0
!  Added Modification tags.
!
!-

!++ AEQSSP_AUTOUTIL_PARSE
!
	logical*4 function aeqSsp_autoUtil_parse(	message, messageLen,
     &							delimiter,
     &		  					target, targetLen,
     &		  					mode, modeLen,
     &		  					method, methodLen,
     &		  					sender, senderLen,
     &							tid, tidLen,
     &							ts, tsLen,
     &	   						token, tokenCount, 
     &	   						value, valueLen,
     &							valueCount )

! 	After calling 'aeqSsp_autoutil_parseCase', this routine
!	makes uppercase the mode, method, and tokens of the message.
!

	implicit none

! Include files:

	include	'AEQ_SSP:AUTODEF'

! Arguments:

	character*(*) 	message						! [R]
	! - the message line

	integer*4	messageLen					! [R]
	! - the length of the message

	character*1	delimiter					! [W]
	! - the delimiter from the message line

	character*(*)	target						! [W]
	! - the target from the message line

	integer*4	targetLen					! [W]
	! - the target length, or zero if there was no target

	character*(*)	mode						! [W]
	! - the mode from the message line

	integer*4	modeLen						! [W]
	! - the mode length, or zero if there was no mode

	character*(*)	method						! [W]
	! - the method from the message line

	integer*4	methodLen					! [W]
	! - the method length, or zero if there was no method

	character*(*)	sender						! [W]
	! - the sender from the message line

	integer*4	senderLen					! [W]
	! - the sender length, or zero if there was no sender

	character*(*)	tid						! [W]
	! - the transaction id from the message line

	integer*4	tidLen						! [W]
	! - the transaction id length, or zero if there was no tid

	character*(*)	ts						! [W]
	! - a timestamp from the message line

	integer*4	tsLen						! [W]
	! - the timestamp length, or zero if there was no timeStamp

	character*(*)	token( MAX_TOKENS )				! [W]
	! - the tokens extracted from the message line

	integer*4	tokenCount					! [W]
	! - the number of tokens extracted from the message line

	character*(*)	value( MAX_VALUES, MAX_TOKENS )			! [W]
	! - the values extracted from the message line

	integer*4	valueLen( MAX_VALUES, MAX_TOKENS )		! [W]
	! - the length of values extracted from the message line
	! - a 'valueLen' for an extracted 'value' will always be 
	!   greater than zero.

	integer*4	valueCount( MAX_TOKENS )			! [W]
	! - the # of values for a token

! Function Value:
!
!	- returns success or failure

! Routines used:

	logical*4	aeqSsp_autoUtil_parseCase

! Author:	Mike Bergsma		4/93
!		Ab Initio Software
!
! Modifications:
!
!	1/7/97	Michael Bergsma
!	- added timeStamp (ts) and transaction id (tid), 2 new and optional 
!	  header fields that are modifiable by the caller

! Local Variables:

	integer*4	i

! Code:

	if ( aeqSsp_autoUtil_parseCase (message, messageLen,
     &					delimiter,
     &		  			target, targetLen,
     &		  			mode, modeLen,
     &		  			method, methodLen,
     &		  			sender, senderLen,
     &					tid, tidLen,
     &					ts, tsLen,
     &	   				token, tokenCount, 
     &	   				value, valueLen,
     &					valueCount ) ) then

	  call str$upcase ( method(:methodLen), method(:methodLen) ) 
	  i = 1
	  do while ( i .le. tokenCount )
	    call str$upcase ( token(i), token(i) ) 
	    i = i + 1
	  enddo

	  aeqSsp_autoUtil_parse = .true.

	else

	  aeqSsp_autoUtil_parse = .false.

	endif

	return

	end

!++ AEQSSP_AUTOUTIL_PARSECASE
!
	logical*4 function aeqSsp_autoUtil_parseCase (	message, messageLen,
     &							delimiter,
     &		  					target, targetLen,
     &		  					mode, modeLen,
     &		  					method, methodLen,
     &		  					sender, senderLen,
     &							tid, tidLen,
     &							ts, tsLen,
     &	   						token, tokenCount, 
     &	   						value, valueLen,
     &							valueCount )

! 	Parses a message of the format:
!
!	|<target>|<mode>|<method>|<sender>||<token>|<value>|<value>|...

	implicit none

! Include files:

	include	'AEQ_SSP:AUTODEF'

! Arguments:

	character*(*) 	message						! [R]
	! - the message line

	integer*4	messageLen					! [R]
	! - the length of the message

	character*1	delimiter					! [W]
	! - the delimiter from the message line

	character*(*)	target						! [W]
	! - the target from the message line

	integer*4	targetLen					! [W]
	! - the target length, or zero if there was no target

	character*(*)	mode						! [W]
	! - the mode from the message line

	integer*4	modeLen						! [W]
	! - the mode length, or zero if there was no mode

	character*(*)	method						! [W]
	! - the method from the message line

	integer*4	methodLen					! [W]
	! - the method length, or zero if there was no method

	character*(*)	sender						! [W]
	! - the sender from the message line

	integer*4	senderLen					! [W]
	! - the sender length, or zero if there was no sender

	character*(*)	tid						! [W]
	! - the transaction id from the message line

	integer*4	tidLen						! [W]
	! - the transaction id length, or zero if there was no tid

	character*(*)	ts						! [W]
	! - a timestamp from the message line

	integer*4	tsLen						! [W]
	! - the timestamp length, or zero if there was no timeStamp

	character*(*)	token( MAX_TOKENS )				! [W]
	! - the tokens extracted from the message line

	integer*4	tokenCount					! [W]
	! - the number of tokens extracted from the message line

	character*(*)	value( MAX_VALUES, MAX_TOKENS )			! [W]
	! - the values extracted from the message line

	integer*4	valueLen( MAX_VALUES, MAX_TOKENS )		! [W]
	! - the length of values extracted from the message line
	! - a 'valueLen' for an extracted 'value' will always be 
	!   greater than zero.

	integer*4	valueCount( MAX_TOKENS )			! [W]
	! - the # of values for a token

! Function Value:
!
!	- returns success or failure

! Routines used:

	integer*4	aeqSsp_autoUtil_left_justify
	character*(DATETIME_SIZE)	aeqssp_autoutil_timeStamp	
	character*8	aeqSsp_autoutil_random8

! Author:	Mike Bergsma		4/93
!		Ab Initio Software
!
! Modifications:
!
!	1/7/97	Michael Bergsma
!	- added timestamp (ts) and transaction id (tid), 2 new and optional 
!	  header fields that is modifiable by the caller

! Local Variables:

	parameter	EXPECT_TARGET	= 0
	parameter	EXPECT_MODE	= 1
	parameter	EXPECT_METHOD	= 2
	parameter	EXPECT_SENDER	= 3
	parameter	EXPECT_FIELD	= 4
	parameter	EXPECT_TOKEN	= 5
	parameter	EXPECT_VALUE	= 6

	integer*4	i,k,
     &			startByte,
     &			endByte,
     &			elementLen,
     &			fieldCount,

     &			state

	character*(VALUE_SIZE)	element

	! Initialize

	! Initalize components of message
	delimiter = ' '
	target = ' '
	targetLen = 0
	mode = ' '
	modeLen = 0
	method = ' '
	methodLen = 0
	sender = ' '
	senderLen = 0
	tokenCount = 0
	fieldCount = 0
	tid = aeqSsp_autoutil_random8 ()
	tidLen = 8
	ts = aeqSsp_autoUtil_timeStamp( )
	tsLen = DATETIME_SIZE

	if ( messageLen .gt. 0 ) then
	
	  ! Remove anything that is not printable
	  call aeqSsp_autoUtil_ghostBuster( message(:messageLen) )

	  startByte = 1

	  ! Extract delimiter
	  delimiter = message(1:1)

	  startByte = startByte + 1

	  state = EXPECT_TARGET
	  do while ( startByte .le. messageLen )
	    
	    ! Search for ending position of next element
	    i = INDEX( message( startByte:messageLen ), delimiter )
	    
	    if ( i .ge. 1 ) then
	      ! Next delimiter found in message
	      ! Set endByte to last character before delimiter
	      endByte = startByte + i - 2
	    else
	      ! No delimiters.  Set endByte to last character in message
	      endByte = messageLen
	    endif

	    ! Extract next element
	    if ( endByte .ge. startByte ) then
	      ! An element exists
	      element = message(startByte:endByte)
	      elementLen = endByte - startByte + 1
	      if ( state .ne. EXPECT_VALUE ) then
	        elementLen = aeqSsp_autoUtil_left_justify( element )
	      endif	
	    else
	      ! Element is NULL
	      element = ' '
	      elementLen = 0 
	    endif	

	    startByte = endByte + 2

	    ! Parse element according to the expected state
	    if ( state .eq. EXPECT_TARGET ) then

	      if ( elementLen .gt. 0 ) then
	        call str$upcase ( element(:elementLen), element(:elementLen) )
		k = INDEX ( element, '#' ) 
		if ( k .gt. 0 ) elementLen = k-1
	        target = element(1:elementLen)
	        targetLen = MIN( elementLen, TARGET_SIZE )
	      endif
	      state = EXPECT_MODE

	    elseif ( state .eq. EXPECT_MODE ) then

	      if ( elementLen .gt. 0 ) then
	        call str$upcase ( element(:elementLen), element(:elementLen) )
	        mode = element
	        modeLen = MIN( elementLen, MODE_SIZE )
	      endif
	      state = EXPECT_METHOD

	    elseif ( state .eq. EXPECT_METHOD ) then

	      if ( elementLen .gt. 0 ) then
	        method = element
	        methodLen = MIN( elementLen, METHOD_SIZE )
	      endif
	      state = EXPECT_SENDER

	    elseif ( state .eq. EXPECT_SENDER ) then

	      if ( elementLen .gt. 0 ) then
	        call str$upcase ( element(:elementLen), element(:elementLen) )
	        sender = element
	        senderLen = MIN( elementLen, SENDER_SIZE )
	      endif
	      state = EXPECT_FIELD

	    elseif ( state .eq. EXPECT_FIELD ) then

	      if ( elementLen .gt. 0 .and. fieldCount .lt. MAX_FIELDS ) then
		! Header field. 
		fieldCount = fieldCount + 1
	        if ( fieldCount .eq. FIELD_TID ) then
		  tid = element(:elementLen)
		  tidLen = MIN ( elementLen, FIELD_SIZE )
		elseif ( fieldCount .eq. FIELD_TIMESTAMP ) then
		  ts = element(:elementLen)
		  tsLen = MIN ( elementLen, FIELD_SIZE )
	        endif
	      else
		state = EXPECT_TOKEN
	      endif

	    elseif ( state .eq. EXPECT_TOKEN ) then

	      if ( elementLen .gt. 0 ) then

		k = tokenCount 
		if ( k .lt. MAX_TOKENS ) then
		  k = k + 1
		  tokenCount = k
		  token(k) = element
		  valueCount(k) = 0
	          state = EXPECT_VALUE
		else
		  ! No more room
		  startByte = messageLen + 1
		endif

	      endif	

	    elseif ( state .eq. EXPECT_VALUE ) then

	      if ( elementLen .gt. 0 ) then
		k = valueCount(tokenCount) 
		if ( k .lt. MAX_VALUES ) then
		  k = k + 1
		  valueCount(tokenCount) = k
		  value( k, tokenCount ) = element
		  valueLen( k, tokenCount ) = MIN( elementLen, VALUE_SIZE )
		endif
	      else
		state = EXPECT_TOKEN
	      endif	

	    endif 

	  enddo

	endif

	aeqSsp_autoUtil_parseCase = (	targetLen .gt. 0 .and.
     &		  		  	senderLen .gt. 0 .and.
     &		  		  	methodLen .gt. 0 .and. 
     &		  		  	modeLen   .gt. 0 ) 

	return
	end

!++ AEQSSP_AUTOUTIL_UNPARSE
!
	logical*4 function aeqSsp_autoUtil_unparse(	delimiter,
     &							target,
     &							mode,
     &							method,
     &							sender,
     &							tid,
     &							ts,
     &	   						token, tokenCount, 
     &	   						value, 
     &							valueLen, valueCount, 
     &							message, messageLen )

! 	Creates a message of the format:
!
!	|<target>|<mode>|<method>|<sender>||<token>|<value>|<value>|...

	implicit none

! Include files:

	include	'AEQ_SSP:AUTODEF'

! Argumets:

	character*1	delimiter					! [R]
	! - delimiter to use

	character*(*)	target						! [R]
	! - target of message
	
	character*(*)	mode						! [R]
	! - mode of message ( QUERY, EVENT, REPLY )

	character*(*)	method						! [R]
	! - method of message

	character*(*)	sender						! [R]
	! - sender of message

	character*(*)	tid						! [W]
	! - the transaction id from the message line

	character*(*)	ts						! [W]
	! - the timeStamp from the message line

	character*(*)	token( MAX_TOKENS )				! [R]
	! - the tokens for the message line

	integer*4	tokenCount					! [R]
	! - the number of tokens for the message line

	character*(*)	value( MAX_VALUES, MAX_TOKENS )			! [R]
	! - the values for the tokens

	integer*4	valueLen( MAX_VALUES, MAX_TOKENS )		! [R]
	! - the length of values 
	! - if the valueLen for a valid value is zero, that value will
	!   be made a blank with length = 1

	integer*4	valueCount( MAX_TOKENS )			! [R]
	! - the value count per token

	character*(*) 	message						! [W]
	! - the message line

	integer*4	messageLen					! [W]
	! - the length of the message line

! Function value:
!
!	- returns success or failure

! Routines Used:

	integer*4	sys$fao
	integer*4 	aeqSsp_autoUtil_left_justify

! Author:	Mike Bergsma		4/93
!		Ab Initio Software
!
! Modifications:
!
!	29-Jun-1995	Mike Bergsma
!	- messages now have three delimeters at the end
!
!	1/7/97	Michael Bergsma
!	- added timestamp (ts) and transaction id (tid), 2 new and optional 
!	  header fields that is modifiable by the caller

! Local Variables:

	integer*4	i,j,
     &			status,
     &			tokenLen,
     &			segmentLen,
     &			valLen

	character*(VALUE_SIZE) 		val
	character*(VALUE_SIZE)		segment

! Code:

	aeqSsp_autoUtil_unparse = .false.

	! Create the header fields
	status = sys$fao(
     &			'!1AS!AS!1AS!AS!1AS!AS!1AS!AS!1AS!AS!1AS!AS!1AS',
     &			messageLen,
     &			message,
     &			delimiter, 
     &			target, delimiter,
     &			mode, delimiter,
     &			method, delimiter,
     &			sender, delimiter,
     &			tid, delimiter,
     &			ts, delimiter )

	if ( .not. status ) call lib$signal( %val( status ) )

	! Build token and value string.
	do i=1,tokenCount

	  tokenLen = aeqSsp_autoUtil_left_justify( token(i) )

	  if ( tokenLen .gt. 0 ) then
	    call sys$fao(	'!1AS!AS!1AS',
     &	  			segmentLen,
     &				segment,
     &				delimiter,
     &				token(i)(:tokenLen),
     &				delimiter )
	  else
	    segment = delimiter
	    segmentLen = 1
	  endif

	  if ( messageLen + segmentLen .gt. MESSAGE_SIZE ) goto 999

	  message(messageLen+1:) = segment(:segmentLen)
	  messageLen = messageLen + segmentLen

	  do j=1,valueCount(i)

	    valLen = valueLen(j,i)

	    if ( valLen .gt. 0 ) then
	      val = value(j,i)(:valueLen(j,i))
	    else
	      val = ' '
	      valLen = 1
	    endif

	    call sys$fao(	'!AS!1AS',
     &				segmentLen,
     &				segment,
     &				val(:valLen),
     &				delimiter )
	
	    if ( messageLen + segmentLen .gt. MESSAGE_SIZE ) goto 999

	    message(messageLen+1:) = segment(:segmentLen)
	    messageLen = messageLen + segmentLen

	  enddo

	enddo

	! Add final delimiters
	message(messageLen+1:) = delimiter
	messageLen = messageLen + 1
	if ( messageLen + segmentLen .gt. MESSAGE_SIZE ) goto 999

	message(messageLen+1:) = delimiter
	messageLen = messageLen + 1
	if ( messageLen + segmentLen .gt. MESSAGE_SIZE ) goto 999

	aeqSsp_autoUtil_unparse = .true.

999	continue

	return

	end

!++ AEQSSP_AUTOUTIL_PUTADDRESS
!
	subroutine aeqSsp_autoUtil_putAddress(	target, targetLen,
     &		  				instance, instanceLen,
     &		  				object, objectLen,
     &		  				node, nodeLen )

! 	Creates an address of the format "instance.object@node" 

	implicit none

! Include files:

	include	'AEQ_SSP:AUTODEF'

! Arguments:

	character*(*) 	target						! [W]
	! - the target to translate

	integer*4	targetLen					! [W]
	! - the instance length, or zero if there was no instance

	character*(*)	instance				       	! [R]
	! - the instance of the target

	integer*4	instanceLen					! [R]
	! - the instance length, or zero if there was no instance

	character*(*)	object	    					! [R]
	! - the object of the target

	integer*4	objectLen			     		! [R]
	! - the object length, or zero if there was no object

	character*(*)	node						! [R]
	! - the node of the target

	integer*4	nodeLen						! [R]
	! - the node length, or zero if there was no node

! Routines used:

! Author:	Mike Bergsma		4/95
!		Ab Initio Software
!
! Modifications:
!

	! Initialize result
	target = ' '
	targetLen = 0

	! If instance is specified, add it.
	if ( instanceLen .gt. 0 ) then

	  target = instance(:instanceLen)//'.'//object(:objectLen)
	  targetLen = instanceLen + 1 + objectLen

	else

	  target = object(:objectLen)
	  targetLen = objectLen

	endif

	! If node is specified (along with instance), add it.
	if ( nodeLen .gt. 0 .and. targetLen .gt. 0 ) then

	  target(targetLen+1:) = '@'//node(:nodeLen)
	  targetLen = targetLen + 1 + nodeLen 

	endif

	return

	end

!++ AEQSSP_AUTOUTIL_GETADDRESS
!
	logical*4 function aeqSsp_autoUtil_getAddress(	target, targetLen,
     &		  					instance, instanceLen,
     &		  					object, objectLen,
     &		  					node, nodeLen )

! 	Parses a message of the format "instance.object@node" into its
!	respective elements
! 	If only the object name is given, then the logical 
!	AUTO_<object> is checked for the full address.

	implicit none

! Include files:

	include	'AEQ_SSP:AUTODEF'

! Arguments:

	character*(*) 	target						! [R/W]
	! - the target to translate

	integer*4	targetLen					! [R/W]
	! - the instance length, or zero if there was no instance

	character*(*)	instance				       	! [W]
	! - the instance of the target

	integer*4	instanceLen					! [W]
	! - the instance length, or zero if there was no instance

	character*(*)	object	    					! [W]
	! - the object of the target

	integer*4	objectLen			     		! [W]
	! - the object length, or zero if there was no object

	character*(*)	node						! [W]
	! - the node of the target

	integer*4	nodeLen						! [W]
	! - the node length, or zero if there was no node

! Function value:
!
!	- returns success or failure

! Routines used:

	integer*4	aeqSsp_autoUtil_left_justify
	integer*4	aeqSsp_autoUtil_node

! Author:	Mike Bergsma		4/93
!		Ab Initio Software
!
! Modifications:
!
!	Mike Bergsma	1/10/95
!	- Address format changed from 
!	  instance@object.node to instance.object@node

! Local Variables:

	integer*4	i,
     &			elementLen,
     &			bufferLen

	character*(TARGET_SIZE)	element

	character*60	buffer

	logical*4	isParsed


! Code:

	! Initialize

	! No target
	instance = ' '
	instanceLen = 0
	object = ' '
	objectLen = 0
	node = ' '
	nodeLen = 0
	isParsed = .false.

	! Remove anything that's not printable
	element = target 
	elementLen = targetLen
	call aeqSsp_autoUtil_ghostBuster( element(:elementLen ) )

	do while ( .not. isParsed )

	  ! Get node name, if specified
	  if ( elementLen .gt. 0 ) then

	    i = INDEX ( element(:elementLen), '@' )
	    
	    if ( i .ge. 1 .and. i .lt. elementLen ) then

	      node = element(i+1:)
	      nodeLen = aeqSsp_autoUtil_left_justify( node )
	      call str$upcase( node(:nodeLen), node(:nodeLen) )
	      elementLen = i-1

	    endif

	  endif

	  ! Get object name, if specified
	  if ( elementLen .gt. 0 ) then

	    i = INDEX ( element(:elementLen), '.' )

	    if ( i .ge. 1 ) then

	      ! Both instance and object are specified

	      if ( i .lt. elementLen ) then

	        ! Get object name
	        object = element(i+1:elementLen)
	        objectLen = aeqSsp_autoUtil_left_justify( object )
	        call str$upcase( object(:objectLen), object(:objectLen) )

	      endif

	      elementLen = i-1

	      if ( elementLen .gt. 0 ) then

	        ! Get instance name, which is what is left over
	        instance = element(:elementLen)
	        instanceLen = aeqSsp_autoUtil_left_justify( instance )
	        call str$upcase( instance(:instanceLen), instance(:instanceLen))
	        elementLen = 0

	      endif

	    else

	      ! Only object is specified
	      object = element(:elementLen)
	      objectLen = aeqSsp_autoUtil_left_justify( object )
	      call str$upcase( object(:objectLen), object(:objectLen))
	      elementLen = 0

	    endif

	  endif

	  if (	objectLen .gt. 0 .and.
     &		instanceLen .eq. 0 .and.
     &		nodeLen .eq. 0 ) then

	    ! Only the object was specified

	    ! Translate AUTO_<object>
	    call Lib$Sys_TrnLog (	'AUTO_'//object(:objectLen),
     &					bufferLen, buffer,, )

	    if ( buffer(:bufferLen) .ne. 'AUTO_'//object(:objectLen) .and.
     &	         buffer(:bufferLen) .ne. object(:objectLen) ) then

	      ! There is a logical non-recusive definition for AUTO_<target>
	      element = buffer
	      elementLen = aeqSsp_autoUtil_left_justify( element )

	      instanceLen = 0	
	      objectLen = 0	
	      nodeLen = 0	
	      goto 100	

	    endif

	  endif

	  ! If object was not specified, make it a wild card
	  if ( objectLen .eq. 0 ) then

	    object = '*' 
	    objectLen = 1

	  endif

	  ! If node not specified, set to local node
	  if ( nodeLen .eq. 0 ) nodeLen = aeqSsp_autoUtil_node ( node )

	  isParsed = .true.

100	  continue

	enddo

	! Re-construct the target
	
	call aeqSsp_autoutil_putAddress (	target, targetLen,
     &						instance, instanceLen,
     &						object, objectLen,
     &						node, nodeLen )

	aeqSsp_autoUtil_getAddress = .true.

	return

	end

!++ AEQSSP_AUTOUTIL_TRANSLATE
!
	logical*4 function aeqSsp_autoUtil_translate (	target, targetLen,
     &		  					instance, instanceLen,
     &		  					object, objectLen,
     &		  					node, nodeLen )
	
! 	Like aeqSsp_autoUtil_getAddress, but will return .false. if
!	the AUTO_<target> logical does not translate

	implicit none

! Include files:

	include	'AEQ_SSP:AUTODEF'

! Arguments:

	character*(*) 	target						! [R/W]
	! - the target to translate

	integer*4	targetLen					! [R/W]
	! - the instance length, or zero if there was no instance

	character*(*)	instance					! [W]
	! - the instance of the target

	integer*4	instanceLen					! [W]
	! - the instance length, or zero if there was no instance

	character*(*)	object	    					! [W]
	! - the object of the target

	integer*4	objectLen			     		! [W]
	! - the object length, or zero if there was no object

	character*(*)	node						! [W]
	! - the node of the target

	integer*4	nodeLen						! [W]
	! - the node length, or zero if there was no node

! Function value:
!
!	- returns success or failure

! Routines used:

	logical*4	aeqSsp_autoUtil_getAddress
	integer*4 	aeqSsp_autoUtil_collapse

! Author:	Mike Bergsma		4/93
!		Ab Initio Software
!
! Modifications:
!
!	Mike Bergsma	1/10/95
!	- Address format changed from 
!	  instance@object.node to instance.object@node

! Local Variables:

	integer*4			bufferLen
	character*60			buffer

! Code:

	! Assume no translate
	aeqSsp_autoUtil_translate = .false.
	if ( targetLen .eq. 0 ) return

	! Translate AUTO_<target>
	call Lib$Sys_TrnLog (	'AUTO_'//target(:targetLen), 
     &				bufferLen, buffer,, )

	if ( buffer(:bufferLen) .ne. 'AUTO_'//target(:targetLen) ) then

	  ! There is a logical definition for AUTO_<target>
	  target = buffer
	  targetLen = aeqSsp_autoUtil_collapse ( target )

	  ! Parse it up.
	  aeqSsp_autoUtil_translate = 
     &	  	aeqSsp_autoUtil_getAddress(	target,
     &						targetLen,
     &						instance,
     &						instanceLen,
     &						object,
     &						objectLen,
     &						node,
     &						nodeLen )


	endif

	return

	end

!++ AEQSSP_AUTOUTIL_STRIP
!
	integer*4 function aeqSsp_autoUtil_strip( name )

	implicit none

	! Strips name of all characters that are not alphanumeric or contain
	! the underscore or dash character.

! Arguments:

	character*(*)	name				! [R/W]
	! - name to be read and stripped

! Function value:

	! Length of name

! Routines used:

! Author:		Michael Bergsma		4/27/92
!			Ab Initio Software
!
! Modifications:
!
!	Mike Bergsma	4/18/94
!	- allow lowercase alpha
!
!	Mike Bergsma	6/2/99
!	- allow dash

! Local Variables:

	integer*4	i,j,k

! Code:

	j = 0
	k = LEN( name )
	do i=1,k

	  if ( ( name(i:i) .ge. '0' .and. name(i:i) .le. '9' ) .or.
     &	       ( name(i:i) .ge. 'A' .and. name(i:i) .le. 'Z' ) .or.
     &	       ( name(i:i) .ge. 'a' .and. name(i:i) .le. 'z' ) .or.
     &	       ( name(i:i) .eq. '_' ) .or. ( name(i:i) .eq. '-' ) ) then

	    ! Keep
	    j = j + 1

	    if ( j .lt. i ) name(j:j) = name(i:i)

	  endif

	enddo
	
	if ( j .lt. k ) name(j+1:k) = ' '

	aeqSsp_autoUtil_strip = j

	return

	end

!++ AEQSSP_AUTOUTIL_COLLAPSE
!
	integer*4 function aeqSsp_autoUtil_collapse( name )

	implicit none

	! Strips name of all characters that are non printable or spaces

! Arguments:

	character*(*)	name				! [R/W]
	! - name to be read and collapsed

! Function value:

	! Length of name

! Routines used:

! Author:		Michael Bergsma		4/27/92

! Local Variables:

	integer*4	i,j,k

! Code:

	j = 0
	k = LEN( name )
	do i=1,k

	  if ( name(i:i) .gt. ' ' ) then

	    ! Keep
	    j = j + 1

	    if ( j .lt. i ) name(j:j) = name(i:i)

	  endif

	enddo
	
	if ( j .lt. k ) name(j+1:k) = ' '

	aeqSsp_autoUtil_collapse = j

	return

	end

!++ AEQSSP_AUTOUTIL_LEFT_JUSTIFY
!
	integer*4 function aeqSsp_autoUtil_left_justify( name )

	implicit none

	! Left justifies and trims trailing blanks and nulls from name.

! Arguments:

	character*(*)	name				! [R/W]

! Function value:

	! Length of name

! Routines used:

! Author:		Michael Bergsma		4/27/92

! Local Variables:

	integer*4	i,k,s,e,n

! Code:

	n = LEN( name )
	s = 1
	do while( s .le. n .and. name(s:s) .le. ' ' )
	  s = s + 1
	enddo

	e = n
	do while( e .ge. 1 .and. name(e:e) .le. ' ' )
	  e = e - 1
	enddo

	k = 0
	do i=s,e
	  k = k + 1
	  if ( k .lt. i ) name(k:k) = name(i:i)
	enddo

	if ( k .lt. n ) name(k+1:n) = ' '

	aeqSsp_autoUtil_left_justify = k
	
	return

	end

!++ AEQSSP_AUTOUTIL_CHECK
!
	integer*4 function aeqSsp_autoUtil_check ( status, iostatus )

	! Check status arguments, and maybe log an error message.

	implicit none

! Include files:

	include	'AEQ_SSP:AUTODEF'

! Arguments:

	integer*4	status				! [R]
	! - system service status

	integer*2	ioStatus			! [R]
	! - i/o status (eg: from SYS$QIOW)
	! - send 1 if there is no ioStatus

! Function value:
!
!	- returns 1 if both status and iostatus are success 
!	  or returns -1 otherwise 

! Routines Used:

! Author:	Michael Bergsma		9/3/92
!		Ab Initio Software

! Local Variables:

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
	  call aeqSsp_autoUtil_log( message(:msgLen) )

	endif

	if ( .not. ioStatus ) then

	  call sys$getmsg(	%val(ioStatus),
     &				msgLen,
     &				message,
     &				,
     &				outadr )
	  call aeqSsp_autoUtil_log( message(:msgLen) )

	endif

	aeqSsp_autoUtil_check = ( status .and. ioStatus )

	return
	end

!++ AEQSSP_AUTOUTIL_ERROR
!
	subroutine aeqSsp_autoUtil_error ()

	! Reports on most recent FORTRAN error

	implicit none

! Include Files:

	include	'($FORIOSDEF)'

! Arguments

! Routines Used:

! Author:	Michael Bergsma		9/3/92
!		Ab Initio Software

! Local variables

	integer*4	fnum,			!  FORTRAN error number
     &			rmssts,			!  RMS error status
     &			rmsstv,			!  RMS status value
     &			iunit,			!  FORTRAN logical unit number
     &			condval,     		!  Error condition value
     &			msglen,
     &			args 

	character*80	msgBuf

! Code
	! Call the FORTRAN-supplied error sensing routine
 	! ERRSNS to return particulars of an error
	! from the run-time I/O support package

	call errsns(fnum,rmssts,rmsstv,iunit,condval)

	!If there was an error other than End of File
	if ( fnum .ne. 0 .and. fnum .ne. FOR$IOS_ENDDURREA ) then

	  call sys$getmsg(	%val(condval),
     &				msgLen,
     &				msgBuf,
     &				%val(15),
     &				args )


	  call aeqSsp_autoUtil_log ( msgBuf(:msgLen) )

	endif

	return

	end

!++ AEQSSP_AUTOUTIL_LOGMESSAGE
!
	subroutine aeqSsp_autoUtil_logMessage ( message, lun )

	! Log a message to the file identified by fileUnit

	implicit none

! Include files:

	include	'AEQ_SSP:AUTODEF'

! Arguments:

	character*(*)	message					! [R]
	! - text of message

	integer*4	lun					! [R]
	! - logical unit number

! Routines Used:

	character*(DATETIME_SIZE)	aeqssp_autoutil_timeStamp	

! Author:	Michael Bergsma		9/3/92
!		Ab Initio Software

! Modifications:
!

! Local Variables:

	parameter	MESSAGE_AREA = 80 - DATETIME_SIZE - 1 
	! Message segments
	! 80 characters wide with N bytes reserved for timestamp

	integer*4	messageLen,
     &			i,j,s,e

	character*(DATETIME_SIZE)	timeStamp

! Code:

	! Get time stamp
	timeStamp = aeqSsp_autoUtil_timeStamp( )

	! How long is the message?
	messageLen = LEN ( message )

	! How many lines will it take to print the message?
	j = ( (messageLen-1) / MESSAGE_AREA ) + 1 

	! Output the lines.
	do i=1,j
	  ! Get start position
	  s = (i-1)*MESSAGE_AREA + 1
	  ! Get end position
	  e = MIN ( s + MESSAGE_AREA - 1, messageLen )
	  ! Output the line
	  if ( lun .eq. 6 ) then
	    write ( lun, 9000 ) timeStamp//':'//message(s:e)
	  else
	    write ( lun, 9001 ) timeStamp//':'//message(s:e)
	  endif
	  ! The timestamp only goes on the first line.
	  if ( i .eq. 1 ) timeStamp = ' '
	enddo

	return

9000	format ( ' ', a )
9001	format ( a )

	end

!++ AEQSSP_AUTOUTIL_LOG
!
	subroutine aeqSsp_autoUtil_log ( message )

	! Log a message to stdout

	implicit none

! Include files:

	include	'AEQ_SSP:AUTODEF'

! Arguments:

	character*(*) message					! [R]
	! - text of message

! Routines Used:

! Author:	Michael Bergsma		9/3/92
!		Ab Initio Software

! Modifications:
!
!	7-Oct-93	Michael Bergsma
!	- added timestamp and esthetics to messages
!
!	Mike Bergsma		4/18/94
!	- use aeqSsp_autoUtil_timeStamp for timeStamp
!
!	Mike Bergsma		1/18/95
!	- use aeqSsp_autoUtil_logMessage to do the work

! Local Variables:

! Code:

	call aeqSsp_autoUtil_logMessage ( message, 6 )
	return

	end

!++ AEQSSP_AUTOUTIL_APPENDLOG
!
	subroutine aeqSsp_autoUtil_appendLog ( fileLog, message )

	! Log a message to fileLog, opening and closing the
	! file each time a message is written.
	! This allows the log file to be examined while still
	! in use.

	implicit none

! Include files:

	include	'AEQ_SSP:AUTODEF'

! Arguments:

	character*(*) filelog					! [R]
	! - log file name

	character*(*) message					! [R]
	! - text of message

! Routines Used:

! Author:	Michael Bergsma		9/3/92
!		Ab Initio Software

! Modifications:
!
!	Mike Bergsma		4/18/94
!	- use aeqSsp_autoUtil_timeStamp for timeStamp

! Local Variables:

	integer*4	ios

! Code:

	open (	unit		= LOG_UNIT,
     &		file		= fileLog,
     &		form		= 'FORMATTED',
     &		status		= 'UNKNOWN',
     &		access		= 'APPEND',
     &		carriageControl = 'LIST',
     &		iostat		= ios )

	if ( ios .eq. 0 ) then

	  call aeqSsp_autoUtil_logMessage ( 
     &	  '------------------------------------------------------------------',
     &	  LOG_UNIT )
	  call aeqSsp_autoUtil_logMessage ( message, LOG_UNIT )
	  close ( unit = LOG_UNIT )

	else

	  call aeqSsp_autoUtil_error ()

	endif

	return

	end

!++ AEQSSP_AUTOUTIL_OPENLOG
!
	subroutine aeqSsp_autoUtil_openLog ( fileLog )

	implicit none

! Include files:

	include	'AEQ_SSP:AUTODEF'

! Arguments:

	character*(*) fileLog				! [R]
	! - name of log file

! Routines Used:

! Author:	Michael Bergsma		1/95
!		Ab Initio Software

! Modifications:
!
! Local Variables:

	integer*4	ios

! Code:

	open (	unit		= LOG_UNIT,
     &		file		= fileLog,
     &		form		= 'FORMATTED',
     &		status		= 'NEW',
     &		access		= 'SEQUENTIAL',
     &		carriageControl = 'LIST',
     &		iostat		= ios )

	if ( ios .eq. 0 ) then

	  call aeqSsp_autoUtil_logMessage ( 
     &	  '------------------------------------------------------------------',
     &	  LOG_UNIT )

	else

	  call aeqSsp_autoUtil_error ()

	endif

	return

	end

!++ AEQSSP_AUTOUTIL_CLOSELOG
!
	subroutine aeqSsp_autoUtil_closeLog ( )

	implicit none

! Include files:

	include	'AEQ_SSP:AUTODEF'

! Arguments:

! Routines Used:

	integer*4 	aeqSsp_autoUtil_left_justify

! Author:	Michael Bergsma		1/95
!		Ab Initio Software

! Modifications:
!
! Local Variables:

	integer*4	fileLen,
     &			lineLen,
     &			ios

	character*132 	filename

	character*240 	line

! Code:

	! Get log file '.TMP' name
	inquire ( unit=LOG_UNIT, name=filename )
	fileLen = aeqSsp_autoUtil_left_justify ( filename )

	! Append to automation '.LOG' 
	open(	unit		= 1,
     &	     	name 		= '.LOG;0',
     &		defaultfile	= filename(:filelen),
     &		status		= 'UNKNOWN',
     &		access		= 'APPEND',
     &		form		= 'FORMATTED',
     & 	     	carriagecontrol	= 'LIST',
     &		iostat		= ios )

	if ( ios .ne. 0 ) then

	  ! Probably another process is running this routine, thus
	  ! the file is locked.  Just start a new file. 
	  open(	unit		= 1,
     &	     	name 		= '.LOG;0',
     &		defaultfile	= filename(:filelen),
     &		status		= 'NEW',
     &		access		= 'SEQUENTIAL',
     &		form		= 'FORMATTED',
     & 	     	carriagecontrol	= 'LIST',
     &		err		= 999 )

	endif

 	rewind( LOG_UNIT )
	do while ( .true. )

	  read ( LOG_UNIT, end=100, fmt=9000 ) lineLen, line
	  if ( lineLen .gt. 0 ) then
	    write ( 1, 9001 ) line(:lineLen)
	  else
	    write ( 1, 9001 ) ' '
	  endif

	enddo
100	continue

	close ( unit=1 ) 
	close ( unit=LOG_UNIT, dispose='DELETE', err=999 )
	goto 1000

999	continue
	close ( LOG_UNIT )

1000	continue

9000	format ( q, a )
9001	format ( a )

	return

	end

!++ AEQSSP_AUTOUTIL_GHOSTBUSTER
!
	subroutine aeqSsp_autoUtil_ghostBuster( string )

	implicit none

!	Replaces ghost characters with spaces.

! Include files:


! Arguments:

	character*(*)	string					! [R/W]
	! - string to bust

! Routines used:

! Author:	Mike Bergsma		4/93
!		Ab Initio Software
!
! Modifications:

! Local Variables:

	integer*4	i

! Code:

	do i=1,LEN(string)
	  if ( string(i:i) .lt. ' ' ) string(i:i) = ' '
	enddo

	return
	end

!++ AEQSSP_AUTOUTIL_INITDTCONTEXT

	integer*4 function aeqSsp_autoUtil_initDTcontext( context )

! 	Initialized date/time context for all date/time functions
!	used by supporting aeqSsp_autoUtil_* functions

	implicit none

! Include files:

	include '($LIBDTDEF)'		
	include	'AEQ_SSP:AUTODEF'

! Arguments:

	integer*4	context				! [W]
	! - context for date formatting. 

! Function value:
!
!	- returns status from function call

! Routines used:

	integer*4	lib$init_date_time_context

! Author:	Michael Bergsma		22-Jun-1992
!
! Copyright:	Ab Initio Software
!

! Local Variables:

! Code:

	aeqSsp_autoUtil_initDTcontext = 
     &		lib$init_date_time_context(	context,
     &						LIB$K_OUTPUT_FORMAT,
     &						TIMESTAMP_FORMAT )

	return

	end

!++ AEQSSP_AUTOUTIL_TIMESTAMP

	character*15 function aeqSsp_autoUtil_timeStamp( )

	implicit none

	! Returns the current time as a sortable character string of
	! the format:
	!
	!	YYYYMMDD:HHMMSS
	
! Include files:

	include	'AEQ_SSP:AUTODEF'

! Arguments:

! Function value:

	! Returns string

! Routines used:

	integer*4	sys$bintim
	logical*4	aeqSsp_autoUtil_check
	character*(DATETIME_SIZE)	aeqSsp_autoUtil_dateString

! Author:	Michael Bergsma		22-Jun-1992
!
! Copyright:	Ab Initio Software
!
! Modifications:
!
!	Mike Bergsma		4/18/94
!	- date/time context is not longer passed. 
!
!	Mike Bergsma		4/18/94
!	- get current time with '-- ::', don't use '-- ::00' otherwise
!	  the seconds get truncated

! Local Variables:

	integer*4	status,
     &			currentTime(2)

! Code:

	status = sys$bintim( '-- ::', currentTime )
	if ( .not. aeqSsp_autoUtil_check( status, 1 ) )
     &  call aeqSsp_autoutil_log ( 'Failed to get binary time stamp' )

	! Format current time into special sortable string.
	aeqSsp_autoUtil_timeStamp = 
     &		aeqSsp_autoUtil_dateString( currentTime )

 	return

	end

!++ AEQSSP_AUTOUTIL_DATESTRING

	character*15 function aeqSsp_autoUtil_dateString( dateTime )

	implicit none

	! Returns the specified time as a sortable character string of
	! the format:
	!
	!	YYYYMMDD:HHMMSSCC
	
! Include files:

	include '($LIBDTDEF)'		

! Arguments:

	integer*4	dateTime(2)			! [R]
	! - absolute time

! Function value:

	! Returns string

! Routines used:

	integer*4	lib$format_date_time
	logical*4	aeqSsp_autoUtil_check

! Author:	Michael Bergsma		22-Jun-1992
!
! Copyright:	Ab Initio Software
!
! Modifications:
!
!	Mike Bergsma		4/18/94
!	- date/time context is not longer passed. Maintained locally.

! Local Variables:

	integer*4	status
	integer*2	dateStringLen

	character*20	dateString

	integer*4	context		! context for date formatting. 
	data		context/0/

! Code:

	if ( context .eq. 0 ) call aeqSsp_autoUtil_initDTcontext ( context )

	! Use system service to get time
	status = lib$format_date_time(	dateString,
     &					dateTime,
     &					context,
     &					dateStringLen,
     &					LIB$M_DATE_FIELDS+LIB$M_TIME_FIELDS )
	if ( .not. aeqSsp_autoUtil_check( status, 1 ) )
     &  call aeqSsp_autoutil_log ( 'Failed to format date time string' )

	aeqSsp_autoUtil_dateString = dateString(1:8)//':'//dateString(10:15)

 	return

	end

!++ AEQSSP_AUTOUTIL_DATESTRING2

	subroutine aeqSsp_autoUtil_dateString2( dateTime, dateString )

	implicit none

	! Formats the specified time as a sortable character string of
	! the format:
	!
	!	YYYYMMDD:HHMMSSCC
	
! Include files:

	include '($LIBDTDEF)'		
	include	'AEQ_SSP:AUTODEF'


! Arguments:

	integer*4	dateTime						! [R]
	! - promis time

	character*(DATETIME_SIZE)	dateString			! [W]

! Function value:

	! None

! Routines used:

	integer*4	gut_cnv32to64
	logical*4	aeqSsp_autoUtil_check
	character*(DATETIME_SIZE)	aeqSsp_autoUtil_dateString

! Author:	Michael Bergsma		22-Jun-1992
!
! Copyright:	Ab Initio Software
!
! Modifications:
!
!	Mike Bergsma		4/18/94
!	- date/time context is not longer passed. Maintained locally.

! Local Variables:

	integer*4	status,
     &		vmsTime(2)

! Code:

	status = gut_cnv32to64 ( dateTime, vmsTime )
	if ( .not. aeqSsp_autoUtil_check( status, 1 ) )
     &  call aeqSsp_autoutil_log ( 'Failed to convert PROMIS time' )

	! Format current time into special sortable string.
	dateString = aeqSsp_autoUtil_dateString( vmsTime )

 	return

	end


!++ AEQSSP_AUTOUTIL_NODE
!
	integer*4 function aeqSsp_autoUtil_node( name )

	implicit none

	! Returns the VAX node name

! Include Files:
	include	'($DVIDEF)'

! Arguments:

	character*(*)	name				! [R/W]

! Function value:

	! Length of name

! Routines used:
	
	integer*4	sys$getdvi
	logical*4	aeqSsp_autoUtil_check

! Author:		Michael Bergsma		4/27/92

! Local Variables:

	integer*4	status,
     &			nameLen

	integer*2	iosb(4)

	structure /itemlist/
	  integer*2	bufLen
	  integer*2	itemCode
	  integer*4	bufAddr
	  integer*4	lenAddr
	  integer*4	terminator
	end structure
	record /itemList/ itmLst
	
! Code:

	itmLst.bufLen = LEN( name )
	itmLst.itemCode = DVI$_HOST_NAME
	itmLst.bufAddr = %loc( name )
	itmLst.lenAddr = %loc( nameLen )
	itmLst.terminator = 0

	nameLen = 0
	status = sys$getdvi(,,'NL:',itmlst,iosb,,,)
	if ( .not. aeqSsp_autoUtil_check( status, iosb(1) ) )
     &  call aeqSsp_autoutil_log ( 'Failed to host name (getdvi)' )

	aeqSsp_autoUtil_node = nameLen
	
	return

	end


!++ AEQSSP_AUTOUTIL_GETPID
!+
	character*8 function aeqSsp_autoutil_getPid ()

	implicit none

	! Returns the VMS PID

! Include Files:

	include '($JPIDEF)'	

! Arguments:

! Function value:

	! 8 character PID, formated as hex

! Routines used:
	
	integer*4	sys$getjpiw
	integer*4	sys$fao
	logical*4	aeqSsp_autoUtil_check

! Author:		Michael Bergsma		3/16/95

! Local Variables:

	integer*4	status,
     &			pid,
     &			dummy

	character*8	pid_s

	structure /itmlst/
	 union
	  map
	    integer*2	buflen
	    integer*2	itmcode
	    integer*4	bufaddr
	    integer*4	retlen
	  end map
	  map
	    integer*4	listend
	  end map
	 end union
	end structure

	record /itmlst/	jpilist(2)

! Code:

	jpilist(1).buflen = 4
	jpilist(1).itmcode = JPI$_PID
	jpilist(1).bufaddr = %LOC (pid)
	jpilist(1).retlen = %LOC (dummy)
	jpilist(2).listend = 0

	status = sys$getjpiw (,,,jpilist,,,)
	if ( .not. aeqSsp_autoUtil_check( status, 1 ) )
     &  call aeqSsp_autoutil_log ( 'Failed to get PID (getjpiw)' )

	status = sys$fao ( '!XL', dummy, pid_s, %VAL(pid) )
	call aeqSsp_autoUtil_check( status, 1 ) 
	aeqSsp_autoUtil_getPid = pid_s

	end

!++ AEQSSP_AUTOUTIL_RANDOM8
!+
	character*8 function aeqSsp_autoutil_Random8 ()

	implicit none

	! Returns the a 8 character hexadecimal random value string

! Include Files:

! Arguments:

! Function value:

	! 8 character random value, formated as hex

! Routines used:
	
	integer*4	sys$fao
	integer*4	sys$bintim
	logical*4	aeqSsp_autoUtil_check

! Author:		Michael Bergsma		3/16/95

! Local Variables:

	character*8	random8

	integer*4	random_i
	real*4		random_r
	equivalence ( random_i, random_r ) 

	integer*4	status,
     &			dummy,
     &			seed(2)
	data		seed / 0,0 /
! Code:

	! Create seed if first time in routine.
	if ( seed(1) .eq. 0 ) then
	  status = sys$bintim( '-- ::', seed )
	  if ( .not. aeqSsp_autoUtil_check( status, 1 ) )
     &    call aeqSsp_autoutil_log ( 'Failed to get binary time stamp' )
	endif

	random_r = RAN ( seed(1) )
	status = sys$fao ( '!XL', dummy, random8, %VAL(random_i) )
	aeqSsp_autoUtil_random8 = random8

	end

!++ AEQSSP_AUTOUTIL_MATCH 
!
	logical*4 function aeqssp_autoutil_match ( token, pattern )

	implicit none

	! Matches wildcard patterns.
	!
	! Example: token 'foobar' matches patterns 
	! 	   foobar, *, foo*, *bar, *oba*, and fo*ar

! Include Files:

! Arguments:

	character*(*)	token			! [R]
	! - token to match

	character*(*)	pattern			! [R]
	! - pattern to compare token with

! Function value:

	! .true. if matched, otherwise .false.

! Routines used:
	
! Author:		Michael Bergsma		10/16/95
!
! Modification:
!	3/30/96		M. Bergsma
!	- 'foo' was not matching 'foo*'
!	- also "new and improved"

! Local Variables:

	logical*4	wildCard 
	integer*4	t,p,m,
     &			patternLen,
     &			tokenLen,
     &			subStringLen

	character*80	subString
  
! Code:
	wildCard = .false.
	patternLen = LEN ( pattern )
	tokenLen = LEN ( token )

	! Check for case when no wildcards are involved
	m = INDEX ( pattern, '*' ) 
	if ( m .eq. 0 ) then
	  aeqssp_autoutil_match = ( token .eq. pattern ) 
	  return 
	endif

	! Scan from left to right in token
	t = 1	! Token ptr
	p = 1	! Pattern ptr

	do while ( p .le. patternLen )

	  ! Is the next character in the pattern a '*' wildcard?
	  if ( pattern(p:p) .eq. '*' ) then

	    ! Encountered a wildcard in pattern. Set a flag.
	    wildCard = .true.	
      
	    ! Advance to first non-wildcard character in the pattern
	    do while ( p .le. patternLen .and. pattern(p:p) .eq. '*' ) 
	      p = p + 1
	    enddo
      
	    ! If the pattern ends with a wildcard, the rest of the token matches
	    if ( p .gt. patternLen ) then
	      aeqssp_autoutil_match = .true.
	      return 
	    endif
    
	  endif

	  ! If the token has ended, then we're done - no match
	  if ( t .gt. tokenLen ) then
	    aeqssp_autoutil_match = .false.
	    return 
	  endif

	  ! Extract the next substring to match from the pattern
	  substring = pattern(p:)
	  subStringLen = patternLen - p + 1
    
	  ! Truncate substring before the first occurrance of another wildcard.
	  m = INDEX ( substring, '*' )
	  if ( m .ne. 0 ) then
	    substring(m:) = ' '
	    substringLen = m - 1
	  endif

	  ! Search for the pattern substring in the token
	  ! If it does not exist, then we're done - no match.
	  m = INDEX ( token(t:), substring(1:substringLen) )
	  if ( m .eq. 0 ) then
	    aeqssp_autoutil_match = .false.
	    return 
	  endif

	  ! If the substring was not preceeded by a '*' in the pattern,
	  ! then the search is anchored; the token must begin with the substring.
	  if ( .not. wildCard .and. m .ne. 1 ) then
	    aeqssp_autoutil_match = .false.
	    return 
	  endif
    
	  ! Advance to the next position in the token
	  ! = Current position + substring offset + substring length 
	  t = t + m + subStringLen - 1

	  ! Advance to the next position in the pattern 
	  ! = Current position + substring length
	  p = p + subStringLen
    
	enddo
  
	! Pattern has ended.

	! If the token has also ended, then its a match, otherwise not.
	if ( t .gt. tokenLen ) then
	  aeqssp_autoutil_match = .true.
	else
	  aeqssp_autoutil_match = .false.
	endif
	  
	return 

	end
