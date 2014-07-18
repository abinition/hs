!*****************************************************************************!
!                    AUTOMAN Software Source Code.                            !
!                                                                             !
!          ***** Copyright: (c) 1993 Ab Initio Software                       !
!          ***** All rights reserved - Licensed Material.                     !
!          ***** Program property of Ab Initio Software                       !
!                                                                             !
!*****************************************************************************!
!++ AUTOMAN4.FOR

! The AUTOmation MANagement routines are distributed into three modules:
!
!	automan.for		! Automation communication routines
!	automan2.for		! General purpose automation utilities
!	automan3.for		! Automation User Interface utilities
!	automan4.for		! Automation Database Utilities

! Externally Callable Routines:

! For Internal Use Only:

! Produced by:	Ab Initio Software

! Author:	Michael Bergsma 	1993

! Modifications:
!
! $Log: automan4.for,v $
! Revision 1.14  2004/11/19 03:44:05  bergsma
! Avoid subscript [:0] errors in keys.
!
! Revision 1.13  2004/10/16 04:28:33  bergsma
! 1. For date keys, use dayTimToInternal instead of DateToInternal
! 2. Use file TRANSACT to point to FIL_TLOG:TRANSACT.DAT.
! The TRANSACT file can then be a logical, useful for TLOGFEED
! recovery.
!
! Revision 1.12  2004/07/23 18:42:26  bergsma
! fixed bug where pget/pnext/pclose/popen buffer slots collide between
! PROMIS files when pack/unpack operations are involved
!
! Revision 1.11  2004/05/15 02:06:37  bergsma
! Added callback for tut_output when doing pexec()
!
! Revision 1.10  2003/04/04 16:39:51  bergsma
! Add our own closeLog functions because the PROMIS one asks questions.
!
! Revision 1.9  2003/01/30 22:46:22  bergsma
! Typo, extra semicolon, programmer thinking in 'C'
!
! Revision 1.8  2003/01/29 18:41:33  bergsma
! Go back to using fil_check for opening PROMIS files, except TLOG.
!
! Revision 1.7  2003/01/28 02:01:02  bergsma
! When opening a PROMIS isam file,
! change "fil_check()" with "status =" and check status afterwards.
!
! Revision 1.6  2003/01/16 14:32:07  bergsma
! V3.1.0
! Added Modification tags.
!
!
!
!-

!++ AEQSSP_AUTOMAN_CHECKFILEFIELD
!
	logical*4 function aeqSsp_autoMan_checkFileField ( 	fileField,
     &								fileName, 
     &								structName,
     &								fieldName,
     &								fieldInfo,
     &								statusText,
     &								statusTextLen )

	implicit none

! Description:

	! Check syntax of fileField 
	!
	! Syntax:	file.fieldName
	!		file.fieldName(index)
	!		file.structure.fieldName
	!		file.structure.fieldName(index)
	!		file.structure(index).fieldName(index)
	!		file.structure(index).fieldName

! Include files:

	include	'FIL:TYPDEF'
	include	'FIL:FILDEF'
	include	'AEQ_SSP:AUTODEF'
	include	'AEQ_SSP:AUTOFIL'

! Arguments:

	character*(*)	fileField				! [R]
	! - database field string

	record /PROMIS_file/	fileName			! [W]

	record /field/		structName			! [W]

	record /field/		fieldName			! [W]

	record /PROMIS_field/	fieldInfo			! [W]

	character*(*)	statusText				! [W]
	! - reason should function fail

	integer*4	statusTextLen				! [W]
	! - length of statusText

! Routines Used:

	logical*4	aeqSsp_autoMan_parseField
	logical*4	aeqSsp_autoMan_checkField

! Local Variables:


! Code:
!
	! Assume failure
	aeqSsp_autoMan_checkFileField = .false.

	! Parse field
	if ( .not. aeqSsp_autoMan_parseField (	fileField,
     &						fileName,
     &						structName,
     &						fieldName,
     &						statusText,
     &						statusTextLen ) ) then
	
	  ! Exit

	! Check field
	elseif ( aeqSsp_autoMan_checkField (	fileName,
     &						structName,
     &						fieldName,
     &						fieldInfo,
     &						statusText,
     &						statusTextLen ) ) then

	  aeqSsp_autoMan_checkFileField = .true.

	endif

	return

	end

!++ AEQSSP_AUTOMAN_GETTRFIELD
!
	logical*4 function aeqSsp_autoMan_getTRfield(	trField,
     &							result,
     &							resultLen,
     &							statusText,
     &							statusTextLen )

	implicit none

! Description:

	! Get value of tracking field from ACTL, TRES, RECP, or TEST
	! global buffers

! Include files:

	include	'FIL:TYPDEF'
	include	'FIL:FILDEF'
	include	'AEQ_SSP:AUTODEF'
	include	'AEQ_SSP:AUTOFIL'

! Arguments:

	character*(*)	trField					! [R]
	! - tracking field string

	character*(*)	result					! [W]
	! - put the result here, converted into a string

	integer*4	resultLen				! [W]
	! - length of result

	character*(*)	statusText				! [W]
	! - reason should function fail

	integer*4	statusTextLen				! [W]
	! - length of statusText

! Routines Used:

	logical*4	aeqSsp_autoMan_checkFileField
	logical*4	aeqSsp_autoMan_extractTRfield
	logical*4	aeqSsp_autoMan_formatField
	integer*4	gut_trimblks

! Local Variables:

	record /PROMIS_file/	fileName
	record /field/		structName
	record /field/		fieldName
	record /PROMIS_field/	fieldInfo

! Code:
!
	! Assume failure
	aeqSsp_autoMan_getTRfield = .false.

	! Check field
	if ( .not. aeqSsp_autoMan_checkFileField ( TRfield(2:),
     &						   fileName, 
     &						   structName,
     &						   fieldName,
     &						   fieldInfo,
     &						   statusText,
     &						   statusTextLen ) ) then

	  ! Exit

	! Extract field
	elseif ( .not. aeqSsp_autoMan_extractTRfield(	fileName,
     &                          			fieldInfo,
     &							statusText,
     &							statusTextLen ) ) then

	  ! Exit

	! Format field
	elseif ( .not. aeqSsp_autoMan_formatField (	fieldInfo,
     &                          			result,
     &                          			resultLen ) ) then
	
	  ! Exit
	  statusText = '% Failed to convert internal value to ascii'
	  statusTextLen = gut_trimblks ( statusText )

	else

	  aeqSsp_autoMan_getTRfield = .true.

	endif

	return

	end

!++ AEQSSP_AUTOMAN_GETDBFIELD
!
	logical*4 function aeqSsp_autoMan_getDBfield(	dbField,
     &							result,
     &							resultLen,
     &							statusText,
     &							statusTextLen )

	implicit none

! Description:

	! Get value of database field from record buffer of previously
	! opened file

! Include files:

	include	'FIL:TYPDEF'
	include	'FIL:FILDEF'
	include	'AEQ_SSP:AUTODEF'
	include	'AEQ_SSP:AUTOFIL'

! Arguments:

	character*(*)	dbField					! [R]
	! - database field string

	character*(*)	result					! [W]
	! - put the result here, converted into a string

	integer*4	resultLen				! [W]
	! - length of result

	character*(*)	statusText				! [W]
	! - reason should function fail

	integer*4	statusTextLen				! [W]
	! - length of statusText

! Routines Used:

	logical*4	aeqSsp_autoMan_checkFileField
	logical*4	aeqSsp_autoMan_extractDBfield
	logical*4	aeqSsp_autoMan_formatField
	integer*4	gut_trimblks

! Local Variables:

	record /PROMIS_file/	fileName
	record /field/		structName
	record /field/		fieldName
	record /PROMIS_field/	fieldInfo

! Code:
!
	! Assume failure
	aeqSsp_autoMan_getDBfield = .false.

	! Check field
	if ( .not. aeqSsp_autoMan_checkFileField ( dbField,
     &						   fileName, 
     &						   structName,
     &						   fieldName,
     &						   fieldInfo,
     &						   statusText,
     &						   statusTextLen ) ) then

	  ! Exit

	! Extract field
	elseif ( .not. aeqSsp_autoMan_extractDBfield(	fileName,
     &                          			fieldInfo,
     &							statusText,
     &							statusTextLen ) ) then

	  ! Exit

	! Format field
	elseif ( .not. aeqSsp_autoMan_formatField (	fieldInfo,
     &                          			result,
     &                          			resultLen ) ) then
	
	  ! Exit
	  statusText = '% Failed to convert internal value to ascii'
	  statusTextLen = gut_trimblks ( statusText )

	else

	  aeqSsp_autoMan_getDBfield = .true.

	endif

	return

	end

!++ AEQSSP_AUTOMAN_PARSEFIELD 
!
	logical*4 function aeqSsp_autoMan_parseField (	dbField,
     &							fileName,
     &							structName,
     &							fieldName,
     &							statusText,
     &							statusTextLen )

	implicit none

! Include files:

	include	'FIL:TYPDEF'
	include	'FIL:FILDEF'
	include	'AEQ_SSP:AUTODEF'
	include	'AEQ_SSP:AUTOFIL'
	include	'GUT:GUTDEF'

! Arguments:

	character*(*)		dbField				! [R]
	! - database field

	record /PROMIS_file/	fileName			! [W]
	! - filename parsed from dbField

	record /field/		structName			! [W]
	! - structure info extracted from dbField

	record /field/		fieldName			! [W]
	! - field info extracted from dbField

	character*(*)		statusText			! [W]
	! - reason if parse fails

	integer*4	statusTextLen				! [W]
	! - length of statusText

! Routines Used:

	integer*4	gut_spanc
	integer*4	gut_trimblks
	integer*4	fil_tbl_fileIndex

! Modifications:

	! Mike Bergsma	12/22/94
	! 	- don't parse "$file.field", rather just "file.field"
	!
	! Mike Bergsma 3/28/98
	!	- allow () or [] as subscript delimiters

! Local Variables:

	parameter	ALPHA = GUT__UPPER
	parameter	ALPHANUMERIC = GUT__UPPER + GUT__DIGIT
	parameter	NUMERIC = GUT__DIGIT
	parameter	NAME = GUT__UPPER + GUT__DIGIT + GUT__ANY
	parameter	TOO_HIGH = 1000

	integer*4	k,
     &			fieldStart,
     &			fieldEnd,
     &			structureStart,
     &			structureEnd,
     &			bracketStart,
     &			bracketEnd,
     &			idx,
     &			mark,
     &			fieldStringLen

	logical*4	status

	character*3	number
	character*1	openDelim,
     &			closeDelim

	character*(TOKEN_SIZE)	fieldString

! Code:
!
	! Copy field
	fieldString = dbField
	fieldStringLen = gut_trimblks ( fieldString )
	call gut_uppercase( fieldString )

	! Initialize components
	status = .true.
	fileName.string = fieldString
	structName.component = ' '
	structName.componentLen = 0
	structName.componentIndex = 0
	fieldName.component = ' '
	fieldName.componentLen = 0
	fieldName.componentIndex = 0

	if ( fieldStringLen .lt. 6 ) then

	  ! Must be at least 6 characters, eg: ACTL.X
	  statusText = '% Field name was not specified as "file.field"'
	  statusTextLen = gut_trimblks ( statusText )
	  status = .false.

	elseif ( 0 .ne. gut_spanc( filename.string, ALPHA, ' ' ) ) then

	  statusText = '% PROMIS file was not specified as 4 alpha characters'
	  statusTextLen = gut_trimblks ( statusText )
	  status = .false.

	elseif ( fil_tbl_fileIndex ( fileName.id, fileName.index ) .ne. 
     &		 FIL__NORMAL ) then

	  statusText = '% No such PROMIS file named '//fileName.string
	  statusTextLen = gut_trimblks ( statusText )
	  status = .false.

	elseif ( fieldString(5:5) .ne. '.' ) then

	  statusText = '% Field name separator "." not found after file name'
	  statusTextLen = gut_trimblks ( statusText )
	  status = .false.

	else

	  ! So far so good.

	  ! Check if any structures are specified
	  !
	  ! eg: structure "di" in test.di(3).prompt
	  !
	  structureStart = 6
	  mark = INDEX( fieldString(structureStart:fieldStringLen), '.' )
	  structureEnd = structureStart + mark - 2
               
	  if ( structureEnd .ge. structureStart ) then

	    ! Structure found as fieldString(structureStart:structureEnd)
		
	    ! Extract any structure indexes
	    closeDelim = fieldString(structureEnd:structureEnd)
	    if ( closeDelim .eq. ')' .or. closeDelim .eq. ']' ) then

	      if ( closeDelim .eq. ')' ) then
		openDelim = '('
	      else
		openDelim = '['
	      endif
	 
	      ! One is present
	      bracketEnd = structureEnd 

	      ! Get start of index
	      mark = INDEX( fieldString(structureStart:structureEnd), 
     &	                    openDelim )
	      bracketStart = structureStart + mark - 1

	      if ( mark .eq. 0 ) then

	  	statusText = '% Invalid record subscript expression'
	        statusTextLen = gut_trimblks ( statusText )
	        status = .false.

	      else

		! Check if a valid number between '() or []'
		if ( bracketStart+1 .gt. bracketEnd-1 ) then

	  	  statusText = '% No subscript specified for record'
		  statusTextLen = gut_trimblks ( statusText )
	          status = .false.
	
		! Check for number between the brackets
	        elseif ( 0 .ne. gut_spanc(
     &				fieldString( bracketStart+1:bracketEnd-1),
     &				NUMERIC, ' ' ) ) then
			
		  statusText = 
     &			'% Non-numeric subscript specified for record'
	  	  statusTextLen = gut_trimblks ( statusText )
	          status = .false.

		else

		  ! Check that number is greater than 0 and less than 1000
		  k = bracketEnd - bracketStart - 1
		  number = fieldString(bracketStart+1:bracketEnd-1)
		  if ( k .eq. 1 ) then
		    read( number(1:1), '(I1)' ) idx
		  elseif ( k .eq. 2 ) then
		    read( number(1:2), '(I2)' ) idx
		  elseif ( k .eq. 3 ) then
		    read( number(1:3), '(I3)' ) idx
		  else
	            idx = TOO_HIGH
		  endif	

		endif

	      endif
			
	    else ! No index brackets () in structure were found

	      ! Implicit index of 1
	      bracketStart = structureEnd + 1
	      idx = 1

	    endif ! Check for indexes

	    ! Check that structure field is specified
	    !
	    ! eg: field "prompt" in test.di(3).prompt
	    !
	    if ( bracketStart-1 .lt. structureStart ) then

	      statusText = '% Record field name not specified'
	      statusTextLen = gut_trimblks ( statusText )
	      status = .false.

	    ! Check that structure field is alphanumerics
	    elseif ( 0 .ne. gut_spanc(
     &	     		fieldString(structureStart:bracketStart-1), 
     &			ALPHANUMERIC, ' ' ) ) then

	      statusText = '% Record field name is not alphanumeric'
	      statusTextLen = gut_trimblks ( statusText )
	      status = .false.

	    ! Check structure index 
	    !
	    ! eg: index "3" in test.di(3).prompt
	    !
	    elseif ( idx .le. 0 .or. idx .gt. 999 ) then

	      statusText = '% Record subscript is not between 1 and 999'
	      statusTextLen = gut_trimblks ( statusText )
	      status = .false.

	    else

	      ! Load structure field in component array 
	      structName.component = fieldString(structureStart:bracketStart-1)
	      structName.componentLen = bracketStart-structureStart
	      structName.componentIndex = idx

	    endif
	
	  else

	    ! No structure. Point to end of 4 character file name
	    structureEnd = 4

	  endif ! Structure field is present

	  ! Now check for field name.
	  fieldStart = structureEnd + 2
	  fieldEnd = fieldStringLen

	  if ( fieldEnd .lt. fieldStart ) then

	    statusText = '% Field name not specified'
	    statusTextLen = gut_trimblks ( statusText )
	    status = .false.
		
	  else

	    ! Field found as fieldString(fieldStart:fieldEnd)
		
	    ! Get any indexes
	    closeDelim = fieldString(fieldEnd:fieldEnd)
	    if ( closeDelim .eq. ')' .or. closeDelim .eq. ']' ) then

	      if ( closeDelim .eq. ')' ) then
		openDelim = '('
	      else
		openDelim = '['
	      endif
	 
	      ! One is present
	      bracketEnd = fieldEnd 

	      ! Get start of index
	      mark = INDEX( fieldString(fieldStart:fieldEnd), openDelim )
	      bracketStart = fieldStart + mark - 1

	      if ( mark .eq. 0 ) then

		statusText = '% Field subscript expression not valid'
	  	statusTextLen = gut_trimblks ( statusText )
	        status = .false.

	      else

		! Check if a valid number between '()'
		if ( bracketStart+1 .gt. bracketEnd-1 ) then

	  	  statusText = '% No field subscript specified'
	  	  statusTextLen = gut_trimblks ( statusText )
	          status = .false.
	
		! Check for number between the brackets
	        elseif ( 	0 .ne. gut_spanc(
     &				fieldString( bracketStart+1:bracketEnd-1),
     &				NUMERIC, ' ' ) ) then
			
		  statusText = '% Non-numeric subscript in field'
	  	  statusTextLen = gut_trimblks ( statusText )
	          status = .false.

		else

		  ! Check that number is greater than 0 and less than 1000
		  k = bracketEnd - bracketStart - 1
		  number = fieldString(bracketStart+1:bracketEnd-1)
		  if ( k .eq. 1 ) then
		    read( number(1:1), '(I1)' ) idx
		  elseif ( k .eq. 2 ) then
		    read( number(1:2), '(I2)' ) idx
		  elseif ( k .eq. 3 ) then
		    read( number(1:3), '(I3)' ) idx
		  else
		    idx = TOO_HIGH
		  endif	

		endif

	      endif
			
	    else ! No index brackets '()' found in field

	      ! Implicit index of 1
	      bracketStart = fieldEnd + 1
	      idx = 1

	    endif ! Check for indexes

	    ! Adjust end of field to not include ()
	    fieldEnd = bracketStart - 1

	    ! Check that field is specified
	    if ( fieldEnd .lt. fieldStart ) then

	      statusText = '% Field name not specified'
	      statusTextLen = gut_trimblks ( statusText )
	      status = .false.

	    ! Check that field is alphanumerics
	    elseif ( 	0 .ne. gut_spanc(
     &			fieldString(fieldStart:fieldEnd), 
     &			ALPHANUMERIC, ' ' ) ) then

	      statusText = '% Field subscript is not alphanumeric'
	      statusTextLen = gut_trimblks ( statusText )
	      status = .false.

	    ! Check field index.
	    elseif ( idx .le. 0 .or. idx .gt. 999 ) then

	      statusText = '% Field subscript value is not between 1 and 999'
	      statusTextLen = gut_trimblks ( statusText )
	      status = .false.

	    else

	      ! Load field in component array 
	      fieldName.component = fieldString(fieldStart:fieldEnd)
	      fieldName.componentLen = fieldEnd-fieldStart+1
	      fieldName.componentIndex = idx

	    endif
	
	  endif ! Field is present

	endif 

	aeqSsp_autoMan_parseField = status

	return

	end

!++ AEQSSP_AUTOMAN_CHECKFIELD 
!
	logical*4 function aeqSsp_autoMan_checkField (	fileName,
     &							structName,
     &							fieldName,
     &							fieldInfo,
     &							statusText,
     &							statusTextLen )

	implicit none

! Description:

! Include files:

	include	'FIL:TYPDEF'
	include	'FIL:FILDEF'
	include	'AEQ_SSP:AUTODEF'
	include	'AEQ_SSP:AUTOFIL'

! Arguments:

	record /PROMIS_file/	fileName		! [R]
	! - file components

	record /field/		structName		! [R]
	! - structure components

	record /field/		fieldName		! [R]
	! - field components

	record /PROMIS_field/	fieldInfo		! [W]
	! - field descriptor

	character*(*)		statusText		! [W]
	! - reason if field is invalid

	integer*4	statusTextLen				! [W]
	! - length of statusText

! Routines used:

        logical*4	aeqSsp_autoMan_fieldInfo 
	integer*4	gut_trimblks
	integer*4	gut_scantable 

! Local Variables:

	character*4	fileNames(FIL__MAXFILES)

	integer*4	i,
     &			status,
     &			offset,
     &			fieldId,
     &                  recordId,
     &			dataType,
     &                  elementLen,
     &                  numElements,
     &			numFiles

	record /PROMIS_file/	recordName

! Code:

	! Assume failure
	status = .false.

	! Check for special TBLS records
	recordId = 0
	if ( ( fileName.string .eq. 'TBLS' .or.
     &	       fileName.string .eq. 'TLOG' ) .and.
     &       structName.componentLen .eq. 4 ) then

	  numfiles = FIL__MAXFILES
	  call fil_tbl_listRecordValues ( filename.id,
     &					  0,
     &					  %ref(filenames),
     &					  numFiles )

	   ! Find the record id, by name.
	  i = gut_scantable ( structName.component(1:4),
     &				      filenames, numFiles )
	  if ( i .gt. 0 ) then
	    recordId = i
	    ! Disable checking the structure field because
	    ! it has been integrated within the variable
	    ! recordId.  This is unique to TBLS and TLOG
	    structname.componentLen = 0
	  endif
        endif

	! Initialize offset to the beginning of the file record
	! buffer that will contain the key field.
	offset = 0

	! Check validity of structure field, if any.
        if ( .not. aeqSsp_autoMan_fieldInfo (	structName,
     &                          		filename.id,
     &						recordId,
     &						fieldId,
     &                          		offset,
     &                          		dataType,
     &                          		elementLen,
     &                          		numElements ) ) then

	  ! Invalid structure field
          if ( fieldId .eq. 0 ) then
            ! Reason was invalid field name
            statusText = '% Invalid structure name '//
     &			structName.component(:structName.componentLen)//
     &			' in field'
	    statusTextLen = gut_trimblks ( statusText )

	  elseif ( numElements .eq. 0 ) then
            ! Reason was index value is out of bounds
            statusText = '% Structure '//
     &			structName.component(:structName.componentLen)//
     &			 ' index is out-of-bounds in field'
	    statusTextLen = gut_trimblks ( statusText )

	  elseif ( dataType .ne. FIL__DTSTRUCT ) then
            ! Reason was field is not a structure
            statusText = '% Structure '//
     &			structName.component(:structName.componentLen)//
     &			 ' is not a valid structure in field'
	    statusTextLen = gut_trimblks ( statusText )

	  else
            ! gut_assert - no more possiblities for error
	    call tut_output('Program bug exists - notify programmer')
	    call gut_assert(.false.)

          endif

	! Check validity of field
	elseif ( .not. aeqSsp_autoMan_fieldInfo(fieldName,
     &                          		fileName.id,
     &						recordId,
     &                          		fieldId,
     &                          		offset,
     &                          		dataType,
     &                          		elementLen,
     &                          		numElements ) ) then

	  ! Check field
	  if ( fieldId .eq. 0 ) then
	    ! Reason was invalid field name
            statusText = '% Invalid field '//
     &			fieldName.component(:fieldName.componentLen)
	    statusTextLen = gut_trimblks ( statusText )

	  elseif ( numElements .eq. 0 ) then
	    ! Reason was index value is out of bounds
            statusText = '% Field '//
     &			fieldName.component(:fieldName.componentLen)//
     &			' index is out-of-bounds'
	    statusTextLen = gut_trimblks ( statusText )

	  elseif ( dataType .eq. FIL__DTSTRUCT ) then
            ! Reason was field is a structure
            statusText = '% Field '//
     &			fieldName.component(:fieldName.componentLen)//
     &			' is really a structure'
	    statusTextLen = gut_trimblks ( statusText )

	  else
            ! gut_assert - no more possiblities for error
	    call tut_output('Program bug exists - notify programmer')
	    call gut_assert(.false.)

          endif

	else

	  status = .true.
	  fieldInfo.type = dataType
	  fieldInfo.id = fieldId
	  fieldInfo.length = elementLen
	  fieldInfo.offset = offset

	endif

	aeqSsp_autoMan_checkField = status

	return

	end

!++ AEQSSP_AUTOMAN_EXTRACTTRFIELD
!
	logical*4 function aeqSsp_autoMan_extractTRfield (	fileName,
     &                          				fieldInfo,
     &								statusText,
     &								statusTextLen )

	implicit none

! Include files:

	include	'FIL:TYPDEF'
	include	'FIL:FILDEF'
	include	'LOT:ACTLBUFF'	
	include 'LOT:RECPBUFF'	
	include	'FIL:TESTCDEF'	
	include	'FIL:TRESCDEF'	
	include	'AEQ_SSP:AUTODEF'
	include	'AEQ_SSP:AUTOFIL'

! Arguments:

	record /PROMIS_file/	fileName		! [R]
	! - PROMIS file to extract from

	record /PROMIS_field/       fieldInfo		! [R/W]
	! - info about field and space to put extracted data

	character*(*)		statusText		! [W]
	! - reason if extract not possible

	integer*4	statusTextLen				! [W]
	! - length of statusText

! Routines used:

	integer*4	gut_trimblks

! Local variables:

	integer*4	startByte,
     &			endByte

! Code:

	aeqSsp_autoMan_extractTRfield = .true.

	startByte = fieldInfo.offset + 1
	endByte = fieldInfo.offset + fieldInfo.length

	if ( fileName.string .eq. 'ACTL' .and.
     &       actl.lotid(1:1) .ne. CHAR(0) ) then

	  fieldInfo.a_string = actl.cbuffer(startByte:endByte)

	elseif ( fileName.string .eq. 'RECP' .and. 
     &		 recp.recpid(1:1) .ne. CHAR(0) ) then

	  fieldInfo.a_string = recp.cbuffer(startByte:endByte)

	elseif ( fileName.string .eq. 'TRES' .and. 
     &		 tres.testopno(1:1) .ne. CHAR(0) ) then

	  fieldInfo.a_string = tres.cbuffer(startByte:endByte)

	elseif ( fileName.string .eq. 'TEST' .and. 
     &		 test.testopno(1:1) .ne. CHAR(0) ) then

	  fieldInfo.a_string = test.cbuffer(startByte:endByte)

	else

	  statusText = '% No tracking access from buffer '//fileName.string
	  statusTextLen = gut_trimblks ( statusText )
	  aeqSsp_autoMan_extractTRfield = .false.

	endif

	return

	end

!++ AEQSSP_AUTOMAN_EXTRACTDBFIELD
!
	logical*4 function aeqSsp_autoMan_extractDBfield (	fileName,
     &                          				fieldInfo,
     &								statusText,
     &								statusTextLen )

	implicit none

! Include files:

	include	'FIL:TYPDEF'
	include	'FIL:FILDEF'
	include	'AEQ_SSP:AUTODEF'
	include	'AEQ_SSP:AUTOFIL'

! Arguments:

	record /PROMIS_file/	fileName		! [R]
	! - PROMIS file to extract from

	record /PROMIS_field/       fieldInfo		! [R/W]
	! - info about field and space to put extracted data

	character*(*)		statusText		! [W]
	! - reason if extract not possible

	integer*4	statusTextLen				! [W]
	! - length of statusText

! Routines used:

	integer*4	gut_trimblks

! Local variables:

	logical*4	found

	integer*4	startByte,
     &			endByte,
     &			select

! Code:

	aeqSsp_autoMan_extractDBfield = .false.

	! First see if buffer is available
	found = .false.
	select = 1
	do while ( .not. found .and. select .le. MAX_AUTO_FILES )

	  if ( filename.id .eq. aeqSsp_autoFil_fileId(select) ) then
	    found = .true.
	  else
	    select = select + 1
	  endif	

	enddo	  

	if ( found ) then

	  startByte = fieldInfo.offset + 1
	  endByte = fieldInfo.offset + fieldInfo.length
	  fieldInfo.a_string = aeqSsp_autoFil_file(select).cbuffer(startByte:endByte)
	  aeqSsp_autoMan_extractDBfield = .true.

	else

	  statusText = '% Cannot access this field until a record is read'
	  statusTextLen = gut_trimblks ( statusText )

	endif

	return

	end

!++ AEQSSP_AUTOMAN_FORMATFIELD
!
	logical*4 function aeqSsp_autoMan_formatField (	fieldInfo,
     &                          			result,
     &                          			resultLen )

	implicit none

! Include files:

	include	'FIL:TYPDEF'
	include	'FIL:FILDEF'
	include	'AEQ_SSP:AUTODEF'
	include	'AEQ_SSP:AUTOFIL'

! Arguments:

	record	/PROMIS_field/ 	fieldInfo			! [R]
	! - data to format

	character*(*)		result				! [W]
	! - converted (formatted) data

	integer*4		resultLen			! [W]
	! - length of formatted data


! Routines used:

	character*(20)	gut_cnvDayTim
	integer*4	aeqSsp_autoUtil_left_justify

! Local variables:


! Code:
	
	aeqSsp_autoMan_formatField = .false.

	if (	fieldInfo.Type .eq. FIL__DTINTEGER .and. 
     &	  	fieldInfo.Length .eq. 2 ) then

	  ! Word
	  write( result, '(I7)', err = 100 ) fieldInfo.a_word

	elseif (	fieldInfo.Type .eq. FIL__DTINTEGER .and. 
     &			fieldInfo.Length .eq. 4 ) then

	  ! Long
	  write( result, '(I12)', err = 100 ) fieldInfo.a_long

	elseif (	fieldInfo.Type .eq. FIL__DTREAL .and.
     &			fieldInfo.Length .eq. 4 ) then

	  ! Real
	  write( result, '(G15.7)', err = 100 ) fieldInfo.a_float

	elseif (	fieldInfo.Type .eq. FIL__DTREAL .and.
     &			fieldInfo.Length .eq. 8 ) then

	  ! Double
	  write( result, '(G15.7)', err = 100 ) fieldInfo.a_double

	elseif (	fieldInfo.Type .eq. FIL__DTBYTE .and. 
     &		   	fieldInfo.Length .eq. 1 ) then

	  ! Byte
	  write( result, '(I5)', err = 100 ) fieldInfo.a_byte

	elseif (	fieldInfo.Type .eq. FIL__DTDATE .and. 
     &			fieldInfo.Length .eq. 4 ) then

	  ! Date
	  write( result, '(A20)', err = 100 ) gut_cnvDayTim(fieldInfo.a_long)

	else ! fieldInfo.Type .eq. FIL__DTSTRING 

	  ! String
	  result = fieldInfo.a_string(:fieldInfo.Length)

	endif

	! Trim
	resultLen = aeqSsp_autoUtil_left_justify ( result )

	! Don't return 0 length result.
	if ( resultLen .eq. 0 ) then
	  result = ' '
	  resultLen = 1
	endif

	aeqSsp_autoMan_formatField = .true.

100	continue

	return

	end

!++ AEQSSP_AUTOMAN_UNFORMATFIELD
!
	logical*4 function aeqSsp_autoMan_unformatField ( fieldInfo, source )

	implicit none

! Include files:

	include	'FIL:TYPDEF'
	include	'FIL:FILDEF'
	include	'AEQ_SSP:AUTODEF'
	include	'AEQ_SSP:AUTOFIL'

! Arguments:

	record	/PROMIS_field/ 	fieldInfo			! [R/W]
	! - data to unformat

	character*(*)		source				! [R]
	! source of data in string format


! Routines used:

	logical*4	gut_dateCvt_dayTimToInternal 
	!!!logical*4	gut_dateCvt_dateToInternal 
	integer*4	aeqSsp_autoUtil_left_justify 

! Local variables:

	character*(VALUE_SIZE)	sourceStr
	integer*4		sl

! Code:
	
	aeqSsp_autoMan_unformatField = .false.

	sourceStr = source
	if ( fieldInfo.Type .eq. FIL__DTSTRING ) then
	  sl = LEN ( sourceStr )
	else 
	  sl = aeqSsp_autoUtil_left_justify ( sourceStr )
	endif

	if ( s1 .eq. 0 ) goto 100 

	if (	fieldInfo.Type .eq. FIL__DTINTEGER .and. 
     &	  	fieldInfo.Length .eq. 2 ) then

	  ! Word
	  read ( sourceStr(1:sl), '(I)', err = 100 ) fieldInfo.a_word

	elseif (	fieldInfo.Type .eq. FIL__DTINTEGER .and. 
     &			fieldInfo.Length .eq. 4 ) then

	  ! Long
	  read ( sourceStr(1:sl), '(I)', err = 100 ) fieldInfo.a_long

	elseif (	fieldInfo.Type .eq. FIL__DTREAL .and.
     &			fieldInfo.Length .eq. 4 ) then

	  ! Real
	  read ( sourceStr(1:sl), '(G)', err = 100 ) fieldInfo.a_float

	elseif (	fieldInfo.Type .eq. FIL__DTREAL .and.
     &			fieldInfo.Length .eq. 8 ) then

	  ! Double
	  read ( sourceStr(1:sl), '(G)', err = 100 ) fieldInfo.a_double

	elseif (	fieldInfo.Type .eq. FIL__DTBYTE .and. 
     &		   	fieldInfo.Length .eq. 1 ) then

	  ! Byte
	  read ( sourceStr(1:sl), '(I)', err = 100 ) fieldInfo.a_byte

	elseif (	fieldInfo.Type .eq. FIL__DTDATE .and. 
     &			fieldInfo.Length .eq. 4 ) then

	  ! Date
	  !!!if ( .not. gut_dateCvt_dateToInternal ( sourceStr(1:sl), 
	  if ( .not. gut_dateCvt_dayTimToInternal ( sourceStr(1:sl), 
     &						  fieldInfo.a_long ) ) goto 100

	else ! fieldInfo.Type .eq. FIL__DTSTRING 

	  ! String
	  fieldInfo.a_string(:fieldInfo.Length) = sourceStr(1:sl)

	endif

	aeqSsp_autoMan_unformatField = .true.

100	continue

	return

	end

!++ AEQSSP_AUTOMAN_FIELDINFO
!
	logical*4 function aeqSsp_autoMan_fieldInfo(	fieldName,
     &							fileId,
     &							recordId,
     &							fieldId,
     &							offset,
     &							dataType,
     &							elementLen,
     &							numElements ) 

	implicit none

! Description:

	! Get info about a field or structure in the PROMIS data base.
	!
	! Inputs:
	!	1. the string name for a PROMIS data base field or structure
	! 	2. an array index (if any), otherwise must = 1
	!	3. the PROMIS file Id containing the field or structure
	!	4. the byte offset - from the beginning of the database
	!	   record - of the parent record which contains the 
	!	   field or structure.
	!
	!	   In most cases, the parent record is (implicitly)
	!	   the entire database record, therefore the offset is 0.
	!	   However, in cases where the field is contained within 
	!	   a structure, the offset may be greater than zero.
	!
	!	   For example, the field "PROMPT" in TEST.DI(3).PROMPT
	!	   is contained in the 3rd element of the structure "DI". 
	!	   Therefore, the offset of "DI(3)" within the TEST 
	!	   record must be supplied as the offset argument when
	!	   calling this subroutine for the "PROMPT" field.
	!	   How is this offset value determined?  By calling this
	!	   same subroutine for the structure "DI(3)", and using
	!	   the value of the offset argument as updated by this routine.
	!
	!	   Example: call
	!
	!		! Get info on TEST.DI(3).PROMPT
	!		! Initialize offset to 0
	!		offset = 0
	!		call aeqSsp_autoMan_field( 'DI', 3, FIL__TEST,
	!					    offset, ... )
	!		! Offset is now for DI(3)
	!		call aeqSsp_autoMan_field( 'PROMPT', 1, FIL__TEST,
	!					    offset, ... )
	!
	! Outputs:
	!	1. The field or structure id
	!	2. The offset of the field or structure relative to the 
	!	   beginning of the database record. 
	!	3. The ending byte of the field or structure.
	!	4. The PROMIS 'dataType' 
	!	5. The length of the field or structure
	!	6. The number of array elements, or 1 if not an array.

! Include files:
	include	'FIL:TYPDEF'
	include	'FIL:FILDEF'
	include	'AEQ_SSP:AUTODEF'
	include	'AEQ_SSP:AUTOFIL'

! Arguments:
	record /field/	fieldName				! [R]
	! - field or structure components

	integer*4	fileId					! [R]
	! - file id

	integer*4	recordId				! [R]
	! - record id, if TBLS file

	integer*4	fieldId					! [W]
	! - field ID

	integer*4	offset					! [W]
	! - offset of field in file (or structure)
	
	integer*4	dataType				! [W]
	! - datatype of field (or structure)

	integer*4	elementLen				! [W]
	! - element length of field (or structure)

	integer*4	numElements				! [W]
	! - number of elements in field (or structure)

! Routines used:
	integer*4	Fil_Tbl_recFieldInfo

! Local variables:
	integer*4	status,
     &			fieldStart
	
! Code: 

	! Assume success
	aeqSsp_autoMan_fieldInfo = .true.

	if ( fieldName.componentLen .eq. 0 ) return

	! Get the basic info
	status = Fil_Tbl_RecFieldInfo(	
     &				fileId,
     &				recordId,
     & 				fieldName.component(:fieldName.componentLen),
     & 				dataType, 
     &				fieldStart,
     &				elementLen,
     &				numElements, 
     &				fieldId )

	if ( status .ne. FIL__NORMAL ) then

	  ! Set fieldId to 0 indicating non-existant field
	  aeqSsp_autoMan_fieldInfo = .false.
	  fieldId = 0

	elseif ( fieldName.componentIndex .gt. numElements ) then

	  ! Set numElements to 0 indicating index was out-of-bounds
	  aeqSsp_autoMan_fieldInfo = .false.
	  numElements = 0

	else

	  ! Calculate new offset
	  ! Offset = offset to structure + offset within structure + index
	  offset = 	offset + 
     &			(fieldStart-1) + 
     &			((fieldName.componentIndex-1) * elementLen) 

	endif

	return

	end

!+ AEQSSP_AUTOMAN_OPENFILE
!
	logical*4 function aeqssp_automan_openFile ( fileName,
     &						     statusText,
     &						     statusTextLen )

	implicit none

! Description:

	! Open a file

! Include files:

	include	'FIL:TYPDEF'
	include	'FIL:FILDEF'
	include	'AEQ_SSP:AUTODEF'
	include	'AEQ_SSP:AUTOFIL'
	include 	'FIL:tlogdef.for'
	include 	'FIL:tlogpadef.for'
	include 	'FIL:tblspdef.for'

! Arguments:

	character*(*)	fileName				! [R]
	character*(*)		statusText		! [W]
	integer*4	statusTextLen				! [W]

! Routines used:

	logical*4	fil_tbl_validFile
	integer*4	fil_open
	integer*4	fil_open_case
	integer*4	aeqSsp_automan_openTlog
	integer*4	gut_trimblks

! Local Variables:

	integer*4		select
	integer*4		status
	logical*4		found

	record /PROMIS_file/	file

! Code:

	! Assume fail
	aeqSsp_autoMan_openFile = .false.

	file.string = fileName 

	! Make sure its a valid dile
	if ( fil_tbl_validFile ( file.Id ) .ne. FIL__NORMAL ) then

	  statusText = '% File "'//fileName//'" is not a valid file'
	  statusTextLen = gut_trimblks ( statusText )
	  return

	endif

	! First see if fileId is already in the common area table
	found = .false.
	select = 1
	do while ( .not. found .and. select .le. MAX_AUTO_FILES )

	  if ( file.id .eq. aeqSsp_autoFil_fileId(select) ) then
	    found = .true.
	  else
	    select = select + 1
	  endif	

	enddo	  

	if ( .not. found ) then
            
	  ! If a buffer is currently not allocated to this file, then find
	  ! a new buffer if one is available.
	  select = 1
	  do while ( .not. found .and. select .le. MAX_AUTO_FILES )

	    if ( aeqSsp_autoFil_fileId(select) .eq. 0 ) then
	      found = .true.
	    else
	      select = select + 1
	    endif

	  enddo
	  
        endif

	if ( .not. found ) then

	  statusText = 
     &		'% Maximum 20 files are in use. Please close files not in use'
	  statusTextLen = gut_trimblks ( statusText )
	  return

	endif

	if ( file.Id .eq. FIL__TLOG ) then

	  status = aeqSsp_automan_openTlog()

	  if ( status .ne. FIL__NORMAL ) then

	     aeqSsp_autoFil_fileId(select) = file.id
	     aeqSsp_autoFil_isFresh(select) = .false.
	     aeqSsp_autoFil_isOpen(select) = .false.
	     aeqSsp_autoFil_packStart(select) = 0
	     statusText = '% Failed to open TLOG'
	     statusTextLen = gut_trimblks ( statusText )
	     aeqSsp_autoMan_openFile = .false.
	     return

	  endif
	  
	elseif ( file.Id .eq. FIL__TBLS ) then

	  call fil_check ( fil_open_case (file.id,FIL__LOCALREADONLY,'MAIN') )

	else

	   call fil_check ( fil_open ( file.id, FIL__LOCALREADONLY ) )

	endif

	aeqSsp_autoFil_fileId(select) = file.id
	aeqSsp_autoFil_isFresh(select) = .false.
	aeqSsp_autoFil_isOpen(select) = .true.
	aeqSsp_autoFil_packStart(select) = 0
	aeqSsp_autoMan_openFile = .true.
	
	return

	end

!+ AEQSSP_AUTOMAN_CLOSEFILE
!
	logical*4 function aeqssp_automan_closeFile ( fileName,
     &						      statusText,
     &						      statusTextLen )

	implicit none

! Description:

	! Close a file

! Include files:

	include	'FIL:TYPDEF'
	include	'FIL:FILDEF'
	include	'AEQ_SSP:AUTODEF'
	include	'AEQ_SSP:AUTOFIL'
	include 	'FIL:tlogdef.for'
	include 	'FIL:tlogpadef.for'
	include 	'FIL:tblspdef.for'

! Arguments:

	character*(*)	fileName				! [R]
	character*(*)		statusText		! [W]
	integer*4	statusTextLen				! [W]

! Routines used:

	integer*4	gut_trimblks
	logical*4	fil_tbl_validFile
	integer*4	fil_close
	integer*4	fil_close_case
	integer*4	aeqSsp_automan_closeTlog

! Local Variables:

	integer*4		select

	logical*4		found

	record /PROMIS_file/	file

! Code:

	! Assume fail
	aeqSsp_autoMan_closeFile = .false.

	file.string = fileName 

	! Make sure its a valid dile
	if ( fil_tbl_validFile ( file.Id ) .ne. FIL__NORMAL ) then

	  statusText = '% File "'//fileName//'" is not a valid file'
	  statusTextLen = gut_trimblks ( statusText )
	  return

	endif

	! First see if fileId is already in the common area table
	found = .false.
	select = 1
	do while ( .not. found .and. select .le. MAX_AUTO_FILES )

	  if ( file.id .eq. aeqSsp_autoFil_fileId(select) ) then
	    found = .true.
	  else
	    select = select + 1
	  endif	

	enddo	  

	if ( .not. found ) then
            
	  ! If a buffer is currently not allocated to this file, then find
	  ! a new buffer if one is available.
	  select = 1
	  do while ( .not. found .and. select .le. MAX_AUTO_FILES )

	    if ( aeqSsp_autoFil_fileId(select) .eq. 0 ) then
	      found = .true.
	    else
	      select = select + 1
	    endif

	  enddo
	  
        endif

	if ( .not. found ) then

	  statusText = 
     &		'% Maximum 20 files are in use. Please close files not in use'
	  statusTextLen = gut_trimblks ( statusText )
	  return

	endif

	if ( file.Id .eq. FIL__TLOG ) then
	  call fil_check ( aeqSsp_automan_closeTlog() )
	elseif ( file.Id .eq. FIL__TBLS ) then
	  call fil_check ( fil_close (file.id ) )
	else
     	  call fil_check ( fil_close ( file.id ) )
	endif

	aeqSsp_autoFil_fileId(select) = 0
	aeqSsp_autoFil_isFresh(select) = .false.
	aeqSsp_autoFil_isOpen(select) = .false.
        aeqSsp_autoFil_packStart(select) = 0 

	aeqSsp_autoMan_closeFile = .true.
	return

	end

!+ AEQSSP_AUTOMAN_GETREC
!
	logical*4 function aeqssp_automan_getrec (keyName,
     &							comparison,
     &							keyValue,
     &							statusText,
     &							statusTextLen )

	implicit none

! Description:

	! Given a file key, comparison, and value, attempt to read the 
	! file record into the first available space in the common area.


! Include files:

	include	'FIL:TYPDEF'
	include	'FIL:FILDEF'
	include	'AEQ_SSP:AUTODEF'
	include	'AEQ_SSP:AUTOFIL'
	include 	'FIL:tlogdef.for'
	include 	'FIL:tlogpadef.for'

! Arguments:

	character*(*)	keyName				! [R]
	integer*4	comparison			! [R]
	character*(*)	keyValue			! [R]
	character*(*)	statusText			! [R]
	integer*4	statusTextLen				! [W]

! Routines used:

	logical*4	aeqSsp_autoMan_checkFileField 
	logical*4	aeqSsp_autoMan_unformatField 
	logical*4	fil_tbl_validKey 
	integer*4	fil_getRec
	integer*4	fil_open
	integer*4	fil_open_case
	integer*4	gut_trimblks
	integer*4	aeqSsp_automan_openTlog
	integer*4	aeqSsp_automan_getTlogRec 
	integer*4	aeqSsp_automan_getTlogLastRec
! Local Variables:

	integer*4		status,
     &				select

	logical*4		found,
     & 				isTLOG,
     & 				isTBLS

	record /PROMIS_file/	fileName
	record /field/		structName
	record /field/		fieldName
	record /PROMIS_field/	fieldInfo

! Code:

	! Assume read will fail
	aeqSsp_automan_getrec = .false.
	
	! Check field
	if ( .not. aeqSsp_autoMan_checkFileField ( 	keyName,
     &						   	fileName, 
     &						   	structName,
     &						   	fieldName,
     &							fieldInfo,
     &						   	statusText,
     &							statusTextLen ) ) then

	  ! Exit

	! Make sure its also a key field
	elseif ( fil_tbl_validKey (	filename.id,
     &					fieldInfo.id ) .ne. FIL__NORMAL ) then


	  statusText = '% Field "'//keyName//'" is not a valid key field'
	  statusTextLen = gut_trimblks ( statusText )

	elseif ( .not. aeqSsp_autoMan_unformatField ( fieldInfo, keyValue ) ) then
	  
	  statusText = '% Invalid key value "'//keyValue//'"'
	  statusTextLen = gut_trimblks ( statusText )

	else

	  ! First see if fileId is already in the common area table
	  found = .false.
	  select = 1
	  do while ( .not. found .and. select .le. MAX_AUTO_FILES )

	    if ( filename.id .eq. aeqSsp_autoFil_fileId(select) ) then
	      found = .true.
	    else
	      select = select + 1
	    endif	

	  enddo	  

	  if ( .not. found ) then
            
	    ! If a buffer is currently not allocated to this file, then find
	    ! a new buffer if one is available.
	    select = 1
	    do while ( .not. found .and. select .le. MAX_AUTO_FILES )

	      if ( aeqSsp_autoFil_fileId(select) .eq. 0 ) then
		found = .true.
	      else
	        select = select + 1
	      endif

	    enddo
	  
          endif

	  if ( .not. found ) then
	     
	     statusText = '% Maximum 20 files are in use. Please close files not in use'
	     statusTextLen = gut_trimblks ( statusText )

	  else

	    if ( filename.string .eq. 'TLOG' ) then
	      isTLOG = .true.
	    else	
	      isTLOG = .false.
	    endif

	    if ( filename.string .eq. 'TBLS' ) then
	      isTBLS = .true.
	    else	
	      isTBLS = .false.
	    endif

	    if ( .not. aeqSsp_autoFil_isOpen(select) ) then

	       if ( isTLOG ) then

		  status = aeqSsp_automan_openTlog()
		  
		  if ( status .ne. FIL__NORMAL ) then
		     
		     aeqSsp_autoFil_fileId(select) = filename.id
		     aeqSsp_autoFil_isFresh(select) = .false.
		     aeqSsp_autoFil_isOpen(select) = .false.
		     aeqSsp_autoFil_packStart(select) = 0
		     statusText = '% Failed to open TLOG'
		     statusTextLen = gut_trimblks ( statusText )
		     aeqSsp_autoMan_getrec = .false.
		     return
		     
		  endif
		  
	       else if ( isTBLS ) then
		  
		  call fil_check ( fil_open_case ( filename.id, FIL__LOCALREADONLY, 'MAIN') )
		  
	       else
		  
		  call fil_check ( fil_open ( filename.id, FIL__LOCALREADONLY ) )
		  
	       endif
	       
	    endif
	    
	    aeqSsp_autoFil_fileId(select) = filename.id
	    aeqSsp_autoFil_isFresh(select) = .false.
	    aeqSsp_autoFil_isOpen(select) = .true.
	    aeqSsp_autoFil_packStart(select) = 0

	    if ( isTLOG ) then

	       if ( fieldinfo.a_long .eq . TLOG__LASTRECNUM ) then

		  status = aeqSsp_automan_getTlogLastRec( aeqSsp_autoFil_file(select).buffer ) 

	       else

		  status = aeqSsp_automan_getTlogRec ( 
     &			       	fieldinfo.a_long, 
     &				comparison,  
     &				aeqSsp_autoFil_file(select).buffer )
	       endif 
	       
	    else
	       
	       if ( fieldInfo.type .eq. FIL__DTSTRING ) then
		  status = fil_getRec (	filename.id, 
     &					fieldInfo.id,
     &			       		fieldinfo.a_string(:fieldInfo.length), 
     &					comparison,
     &			       		FIL__NOLOCK, 
     &					aeqSsp_autoFil_file(select).buffer ) 
	       else

		  status = fil_getRec (	filename.id, 
     &					fieldInfo.id,
     &			       		fieldinfo.a_byte, 
     &					comparison,
     &			       		FIL__NOLOCK, 
     &					aeqSsp_autoFil_file(select).buffer ) 
	       endif
	       
	    endif
	    
	    if ( status .eq. FIL__RNF ) then
	       
	       ! Unsuccessful read. Deallocate buffer.
	       statusText = '% Record not found'
	       statusTextLen = gut_trimblks ( statusText )
	       aeqSsp_autoFil_isFresh(select) = .false.
	       
	    else
	       
	      ! Successful read.
	       aeqSsp_autoFil_isFresh(select) = .true.
	       aeqSsp_automan_getrec = .true.
	       
	    endif
	    
	 endif
	 
	endif

	return

	end

!+ AEQSSP_AUTOMAN_GETNEXT
!
	logical*4 function aeqSsp_automan_getnext ( 	keyName, 
     &							statusText,
     &							statusTextLen )

	implicit none

! Description:

	! Given a file key, get the next record


! Include files:

	include	'FIL:TYPDEF'
	include	'FIL:FILDEF'
	include 'FIL_APP:FILCOM'
	include	'AEQ_SSP:AUTODEF'
	include	'AEQ_SSP:AUTOFIL'

! Arguments:

	character*(*)	keyName				! [R]
	character*(*)	statusText			! [R]
	integer*4	statusTextLen				! [W]

! Routines used:

	logical*4	aeqSsp_autoMan_checkFileField 
	logical*4	fil_tbl_validKey 
	integer*4	fil_getNext
	integer*4	gut_trimblks
	integer*4	aeqSSp_automan_getTlogNext 

! Local Variables:

	integer*4		select,
     &				status

	logical*4		found,
     &				isTLOG

	record /PROMIS_file/	fileName
	record /field/		structName
	record /field/		fieldName
	record /PROMIS_field/	fieldInfo


! Code:

	! Assume read will fail
	aeqSsp_automan_getnext = .false.

	! Check field
	if ( .not. aeqSsp_autoMan_checkFileField ( 	keyName,
     &						   	fileName, 
     &						   	structName,
     &						   	fieldName,
     &							fieldInfo,
     &						   	statusText,
     &							statusTextLen ) ) then

	  ! Exit

	! Make sure its also a key field
	elseif ( fil_tbl_validKey (	filename.id,
     &					fieldInfo.id ) .ne. FIL__NORMAL ) then


	  statusText = '% Field "'//keyName//'" is not a valid key field'
	  statusTextLen = gut_trimblks ( statusText )

	else

	  ! See if fileId is already in the common area table
	  found = .false.
	  select = 1
	  do while ( .not. found .and. select .le. MAX_AUTO_FILES )

	    if ( filename.id .eq. aeqSsp_autoFil_fileId(select) ) then
	      found = .true.
	    else
	      select = select + 1
	    endif	

	  enddo	  

	  if ( .not. found ) then

	    statusText = '% Cannot get next record - no current record exists'
	    statusTextLen = gut_trimblks ( statusText )

	  else 

	    if ( filename.string .eq. 'TLOG' ) then
	      isTLOG = .true.
	    else	
	      isTLOG = .false.
	    endif

	    if ( isTLOG ) then

	      status = aeqSSp_automan_getTlogNext (
     &				 	aeqSsp_autoFil_file(select).buffer ) 
	
	    else

	      if ( fil_filCom_access ( filename.index ) .eq. FIL__CLOSED ) then

	        call tut_output('File was closed by PROMIS menu function. '//
     &				'Re-issue pget() before using pnext()' )
		status = FIL__RNF
 
	      else
	
	        status = fil_getNext (	filename.id, 
     &				 	FIL__NOLOCK, 
     &				 	aeqSsp_autoFil_file(select).buffer ) 

	      endif

	    endif

	    if ( status .eq. FIL__RNF .or. status .eq. FIL__EOF ) then

	      ! Unsuccessful read. Deallocate buffer.
	      aeqSsp_autoFil_isFresh (select) = .false.
	      statusText = '% Next record not found'
	      statusTextLen = gut_trimblks ( statusText )

	    else

	      ! Successful read.
	      aeqSsp_autoFil_isFresh (select) = .true.
	      aeqSsp_automan_getnext = .true.

	    endif

	  endif

	endif

	return

	end

!++ AEQSSP_AUTOMAN_GETSYMBOL
!
	logical*4 function aeqSsp_autoMan_getSymbol (	symbol,
     &							result,
     &							resultLen,
     &							statusText,
     &							statusTextLen )

	implicit none

! Description:

	! Get value of PROMIS symbol

! Include files:


! Arguments:

	character*(*)	symbol					! [R]
	! - database field string

	character*(*)	result					! [W]
	! - put the result here, converted into a string

	integer*4	resultLen				! [W]
	! - length of result

	character*(*)	statusText				! [W]
	! - reason should function fail

	integer*4	statusTextLen				! [W]
	! - length of statusText

! Routines Used:

	integer*4	Lib$Get_Symbol
	integer*4	gut_trimblks

! Local Variables:

! Code:
!
	! Translate the symbol
	resultLen = 0 
	if ( Lib$Get_Symbol (	'PCL_'//symbol(2:),
     &		    		result,
     &		    		resultLen ) ) then


	  aeqSsp_autoMan_getsymbol = .true.

	else

	  statusText = '% PROMIS symbol '//symbol//' is not defined'
	  statusTextLen = gut_trimblks ( statusText )
	  aeqSsp_autoMan_getsymbol = .false.

	endif


	return

	end

!++ AEQSSP_AUTOMAN_GETPARAMETER
!
	logical*4 function aeqSsp_autoMan_getParameter (	parm,
     &						      		result,
     &								resultLen,
     &								statusText,
     &								statusTextLen )

	implicit none

! Description:

	! Get value of PROMIS parameter

! Include files:

	include	'FIL:TYPDEF'
	include	'TST:TSTDEF'	
	include	'TST:ENTDEF'	
	include	'TST:PARMDEF'	

! Arguments:

	character*(*)	parm					! [R]
	! - parameter field string

	character*(*)	result					! [W]
	! - put the result here, converted into a string

	integer*4	resultLen				! [W]
	! - length of result

	character*(*)	statusText				! [W]
	! - reason should function fail

	integer*4	statusTextLen				! [W]
	! - length of statusText

! Routines Used:

	logical*4	aeqSsp_automan_getParm
	integer*4	gut_trimblks

! Local Variables:

	character*(TYP__PARMNMSZ)	parmName
	character*(TYP__PARMVALSZ)	parmVal
	integer*4			parmSources

! Code:
!
	! Determine what are sources should be
	!
	! 1. ACTL, RECP 		- for lot track-in
	!
	! 2. ACTL, RECP, TEST		- for enter lot data
	!
	! 3. TEST			- for gather general data
	!

	parmName = parm
	call gut_uppercase( parmName )
	parmSources = 	TST__PARMSOURCEACTL +
     &			TST__PARMSOURCERECP +
     &	                TST__PARMSOURCETEST

	if ( aeqSsp_autoMan_getParm(	parmName,
     &			        	parmSources,
     &					parmVal ) ) then
	
	  result = parmVal
	  resultLen = gut_trimblks ( result )
	  aeqSsp_autoMan_getParameter = .true.

	else

	  statusText = '% PROMIS parameter '//parm//' is not defined'
	  statusTextLen = gut_trimblks ( statusText )
	  aeqSsp_autoMan_getParameter = .false.

	endif

	return

	end

!+ AEQSSP_AUTOMAN_GETDBRECORD
!
	logical*4 function aeqssp_automan_getDbRecord (	fileName,
     &							bufLen,
     &							pRecordBuf )

	implicit none

! Description:

	! Given a file name, return the current DATABASE  record buffer

! Include files:

	include	'FIL:TYPDEF'
	include	'FIL:FILDEF'
	include	'LOT:ACTLBUFF'	
	include 'LOT:RECPBUFF'	
	include	'FIL:TESTCDEF'	
	include	'FIL:TRESCDEF'	
	include	'AEQ_SSP:AUTODEF'
	include	'AEQ_SSP:AUTOFIL'

! Arguments:

	character*(*)   fileName			! [R]
	integer*4	bufLen				! [R]
	integer*4	pRecordBuf			! [W]

! Routines used:

! Local Variables:

	integer*4		select
	integer*4		newSelect
	integer*4		addr
	logical*4		found
	record /PROMIS_file/	file

! Code:

	aeqSsp_autoMan_getDbRecord = .false.

	file.string = fileName 

	! First see if buffer is available
	found = .false.
	select = 1
	do while ( .not. found .and. select .le. MAX_AUTO_FILES )

	  if ( file.Id .eq. aeqSsp_autoFil_fileId(select) ) then
	    found = .true.
	  else
	    select = select + 1
	  endif	

	enddo	  

	if ( found ) then

	  if ( aeqSsp_autoFil_isFresh(select) ) then

	    if ( aeqSsp_autoFil_packStart(select) .gt. 0 ) then

	      ! Record has not been upacked yet. Have to do it now.
	      ! Move to another location.
              newSelect = 1
              found = .false.
              do while ( .not. found .and.
     &			 newSelect .le. MAX_AUTO_FILES )
                if ( aeqSsp_autoFil_fileId(newSelect) .eq. 0 ) then
          	  found = .true.
	        else
		  newSelect = newSelect + 1
	        endif
	      enddo

 	      if ( found ) then

	        ! Unpack
		!call tut_output('DELAYED UNPACK')

 	        addr = %loc ( aeqSsp_autoFil_file(select).cbuffer )

                call Fil_PckUnp_UnPack( 
     &			%val(addr),
     &			aeqSsp_autoFil_packSize(select),
     &			aeqSsp_autoFil_packStart(select),
     &			aeqSsp_autoFil_file(newSelect).buffer,
     &			MAX_RECORDSIZE,
     &			aeqSsp_autoFil_packSize(select) )

		aeqSsp_autoFil_fileId(newselect) = file.Id
		aeqSsp_autoFil_isOpen(newselect) = .true.

		aeqSsp_autoFil_packStart(select) = 0 
		aeqSsp_autoFil_isOpen(select) = .false.
		aeqSsp_autoFil_isFresh(select) = .false.
		aeqSsp_autoFil_fileId(select) = 0

		select = newSelect

	      endif

	    endif

	    if ( found ) then
	      pRecordBuf = %loc( aeqSsp_autoFil_file(select).cbuffer ) 
	      aeqSsp_autoMan_getDbRecord = .true. 
	      aeqSsp_autoFil_isFresh(select) = .false.
	      aeqSsp_autoFil_packStart(select) = 0
            endif

	  endif

	endif
	return

	end

!+ AEQSSP_AUTOMAN_GETTRRECORD
!
	logical*4 function aeqssp_automan_getTrRecord (	fileName,
     &							bufLen,
     &							pRecordBuf )

	implicit none

! Description:

	! Given a file name, return the current TRACKING record buffer

! Include files:

	include	'FIL:TYPDEF'
	include	'FIL:FILDEF'
	include	'LOT:ACTLBUFF'	
	include 'LOT:RECPBUFF'	
	include	'FIL:TESTCDEF'	
	include	'FIL:TRESCDEF'	
	include	'AEQ_SSP:AUTODEF'
	include	'AEQ_SSP:AUTOFIL'

! Arguments:

	character*(*)   fileName			! [R]
	integer*4	bufLen				! [R]
	integer*4	pRecordBuf			! [W]

! Routines used:

! Local Variables:

! Code:

	aeqSsp_autoMan_getTrRecord = .false.

	if ( fileName .eq. 'ACTL' .and.
     &	     aeqSsp_autoFil_ACTL.buffer(1) .eq. 0 .and.
     &       actl.lotid(1:1) .ne. CHAR(0) ) then

	  ! Actl buffer is active.
	  aeqSsp_autoFil_ACTL.cbuffer = actl.cbuffer(1:bufLen)  
	  pRecordBuf = %loc( aeqSsp_autoFil_ACTL.cbuffer ) 
	  aeqSsp_autoMan_getTrRecord = .true. 

	elseif ( fileName .eq. 'RECP' .and. 
     &	         aeqSsp_autoFil_RECP.buffer(1) .eq. 0 .and.
     &		 recp.recpid(1:1) .ne. CHAR(0) ) then

	  aeqSsp_autoFil_RECP.cbuffer = recp.cbuffer(1:bufLen)  
	  pRecordBuf = %loc( aeqSsp_autoFil_RECP.cbuffer ) 
	  aeqSsp_autoMan_getTrRecord = .true. 

	elseif ( fileName .eq. 'TRES' .and. 
     &	         aeqSsp_autoFil_TRES.buffer(1) .eq. 0 .and.
     &		 tres.testopno(1:1) .ne. CHAR(0) ) then

	  aeqSsp_autoFil_TRES.cbuffer = tres.cbuffer(1:bufLen)  
	  pRecordBuf = %loc( aeqSsp_autoFil_TRES.cbuffer ) 
	  aeqSsp_autoMan_getTrRecord = .true. 

	elseif ( fileName .eq. 'TEST' .and. 
     &	         aeqSsp_autoFil_TEST.buffer(1) .eq. 0 .and.
     &		 test.testopno(1:1) .ne. CHAR(0) ) then

	  aeqSsp_autoFil_TEST.cbuffer = test.cbuffer(1:bufLen)  
	  pRecordBuf = %loc( aeqSsp_autoFil_TEST.cbuffer ) 
	  aeqSsp_autoMan_getTrRecord = .true. 

	endif

	return

	end

!+ AEQSSP_AUTOMAN_INITDBFILES
!
	subroutine aeqSsp_automan_initDbFiles ( )

	implicit none

! Description:

	! Initialize the file buffer common area

! Include files:

	include	'FIL:TYPDEF'
	include	'FIL:FILDEF'
	include	'AEQ_SSP:AUTODEF'
	include	'AEQ_SSP:AUTOFIL'

! Arguments:

! Routines used:

! Local Variables:

	integer*4	i

! Code:

	do i=1,MAX_AUTO_FILES 
	  aeqSsp_autoFil_fileId(i) = 0
	  aeqSsp_autoFil_isFresh (i) = .false.
	  aeqSsp_autoFil_isOpen(i) = .false.
          aeqSsp_autoFil_packStart(i) = 0
	enddo	  

	return

	end

!+ AEQSSP_AUTOMAN_INITTRFILES
!
	subroutine aeqSsp_automan_initTrFiles ( )

	implicit none

! Description:

	! Initialize the tracking buffer common area

! Include files:

	include	'FIL:TYPDEF'
	include	'FIL:FILDEF'
	include	'AEQ_SSP:AUTODEF'
	include	'AEQ_SSP:AUTOFIL'

! Arguments:



! Local Variables:

! Code:

	aeqSsp_autoFil_ACTL.buffer(1) = 0 
	aeqSsp_autoFil_TRES.buffer(1) = 0 
	aeqSsp_autoFil_TEST.buffer(1) = 0 
	aeqSsp_autoFil_RECP.buffer(1) = 0 
	return

	end

!+ AEQSSP_AUTOMAN_CLOSEFILES
!
	subroutine aeqSsp_automan_closeFiles ( )

	implicit none

! Description:

	! Close any files opened by HyperScript

! Include files:

	include	'FIL:TYPDEF'
	include	'FIL:FILDEF'
	include	'AEQ_SSP:AUTODEF'
	include	'AEQ_SSP:AUTOFIL'

! Arguments:

! Routines used:

! Local Variables:

	integer*4	i
	integer*4	fileId

! Code:

	do i=1,MAX_AUTO_FILES 
	  fileId = aeqSsp_autoFil_fileId(i) 
	  if ( fileId .ne. 0 ) then
	    if ( aeqSsp_autoFil_isOpen(i) ) 
     &	      call fil_close ( fileId )
	    aeqSsp_autoFil_fileId(i) = 0
	    aeqSsp_autoFil_isFresh (i) = .false.
	    aeqSsp_autoFil_isOpen(i) = .false.
            aeqSsp_autoFil_packStart(i) = 0
          endif
	enddo	  

	return

	end


!++ AEQSSP_AUTOMAN_SNAP
!
	subroutine aeqSsp_autoMan_snap (	l,
     &						result,
     &						resultLen )

	implicit none

! Description:

	! Get line of snap buffer

! Include files:

	include	'TUT:TUTDEF'
	include 'TUT:SNAPDEF'
	include	'TUT:SNAPCOM'	

! Arguments:

	integer*4	l					! [R]
	! Screen line

	character*(*)	result					! [W]
	! - put the result here, converted into a string

	integer*4	resultLen				! [W]
	! - length of result

! Routines Used:

	integer*4	gut_trimblks

! Local Variables:

	integer*4	line
! Code:
!
	line = ICHAR(tut_snapcom_screenmap(l:l))
	result = tut_snapcom_snapbuf( line ).text 
	resultLen = tut_snapcom_snapbuf( line ).length

	return

	end

!+ AEQSSP_AUTOMAN_ENABLECALLBACK
!
	subroutine aeqSsp_automan_enableCallback ()

	external aeqSsp_automan_putline        ! callback to receive tut output

	include	'TUT:TUTDEF'
	include	'TUT:CBDEF'

	call Tut_Callback_Enable(TUT_CBDEF_RN_OUTPUT, aeqSsp_automan_putline, 0)

	return
	end

!+ AEQSSP_AUTOMAN_DISABLECALLBACK
!
	subroutine aeqSsp_automan_disableCallback ()

	external aeqSsp_automan_putline

	call Tut_Callback_Disable(0, aeqSsp_automan_putline )

	return

	end

!+ AEQSSP_AUTOMAN_PUTLINE
!
	logical*4 function aeqSsp_automan_putline ( string, length, intArg, context )
!
! Description:
!
!	! Call back for tut_output
!
!
	include	'TUT:TUTDEF'
	include	'TUT:CBDEF'

	character*(*) string    ! [R] String to be displayed
	integer*4 length        ! [R] Number of characters to be displayed
	integer*4 intArg        ! [R] Address of integer supplied in call to Tut_Callback_Enable()
	record /Tut_CbDef_Context/ context ! [R] Current context values

	call gHyp_promis_putline ( string, %VAL(length) )

	aeqssp_automan_putline = .true.

	return

	end

!+ AEQSSP_AUTOMAN_UNPACK
!
	logical*4 function aeqssp_automan_unpack ( fileid, 
     &						   recid, 
     &						   bufflen,
     &						   buffer )

	implicit none

! Description:

	! Delayed Unpack

! Include files:

	include	'FIL:TYPDEF'
	include	'FIL:FILDEF'
	include	'AEQ_SSP:AUTODEF'
	include	'AEQ_SSP:AUTOFIL'

! Arguments:

	integer*4	fileId				! [R]
	! - name of PROMIS file where buffer is

	integer*4	recId				! [R]
	! - name of PROMIS file to unpack

	integer*4	bufflen				! [R]
	! The length of the region to unpack

	integer*4	buffer(bufflen)			! [R]
	! - the buffer region to unpack

! Routines used:

! Local Variables:

	integer*4	packstart,
     &			addr,
     &			select 

	logical*4	found

	record /PROMIS_file/ file

! Code:
	
	aeqssp_automan_unpack = .true.

	call Fil_Tbl_PackStart( recId, packstart )

	! Locate a slot to unpack the buffer.
	select = 1
	found = .false.
	do while ( .not. found .and. select .le. MAX_AUTO_FILES )

	  if ( aeqSsp_autoFil_fileId(select) .eq. recId ) then
	    found = .true.
	  else
	    select = select + 1
	  endif	

	enddo

        if ( .not. found ) then

          select = 1
          do while (	.not. found .and.
     &			 select .le. MAX_AUTO_FILES )

            if ( aeqSsp_autoFil_fileId(select) .eq. 0 ) then
              found = .true.
            else
              select = select + 1
            endif

          enddo

        endif

	file.id = recId
 	if ( .not. found ) then
	  aeqssp_automan_unpack = .false.
	  return
	endif

	! Save the pack start and pack size components.
        aeqSsp_autoFil_packStart(select) = packStart
        aeqSsp_autoFil_packSize(select) = bufflen 

	! Actually unpacking is delayed, see aeqSsp_automan_getDBrecord
	! Move the buffer. 
	call gut_movebuff (	bufflen, 
     &				buffer,
     &				aeqSsp_autoFil_file(select).buffer ) 

	!if ( packStart .gt. 0 ) then
	!  call tut_output('RECORD is PACKED')
	!endif

	! Update 
	aeqSsp_autoFil_fileId(select) = recid
	aeqSsp_autoFil_isFresh(select) = .true.
	aeqSsp_autoFil_isOpen(select) = .false.

	return
	end

	integer*4 function aeqSsp_automan_getTlogRec( recnum, 
     &						      comparison,
     &						      buffer )
!
	include 'FIL:basicdef.for'
	include 'FIL:filmsg.for'
	include 'MDL:rmsdef.for'
	include 'FIL_CMT:cmtmsg.for'
	include 'AEQ_SSP:AUTODEF'
	include 'AEQ_SSP:AUTOFIL'
	include 'FIL:tlogdef.for'
	include 'FIL:tlogpadef.for'
!
!   Arguments
!
	integer*4	recnum			! READ
	integer*4	comparison 		! READ
        byte            buffer ( TLOG__MAXRECSIZ ) ! WRITE
!
!   Function value: Either FIL__NORMAL or FIL__RNF
!
!   Routines used:

	external	Fil_Check
	external	Fil_Rms_GetRec
	external	Lib$Signal

!   Internal Variables:

	integer*4	status(2),
     &			bufflen


	record	/TLOG_record/ tlogRec

!   Code:

	bufflen = TLOG__MAXRECSIZ

      call Fil_Rms_GetRec( 	aeqSsp_autofil_tlogRab, 
     &				0, 
     &				TLOG_S_RECNUM,
     &				recnum,
     &				comparison,
     &				FIL__NOLOCK,
     &				tlogRec.buffer, 
     &				bufflen,
     &				status )

	if ( status(1) ) then
	  !!!call tut_output('GETREC: NORMAL ')
	  aeqSsp_autofil_tlogNextValid = .true.
	  aeqSsp_autofil_tlogNextRec = tlog.recnum + 1
	  aeqssp_automan_GetTlogRec = FIL__NORMAL
	  call gut_movebuff ( bufflen, tlogRec.buffer, buffer )

	else if ( status(1) .eq. RMS$_RNF ) then
	  !!!call tut_output('GETREC: RNF')
	  aeqSsp_autofil_tlogNextValid = .false.
	  aeqSsp_autofil_tlogNextRec = recnum
  	  aeqssp_automan_GetTlogRec = FIL__RNF

	else if ( .not. status(1) ) then
	  !!!call tut_output('GETREC: IO ERROR')
	  call Lib$Signal( %val(CMT__TRANSIO), %val(0),
     &				%val(status(1)), %val(status(2)) )
	  aeqSsp_autofil_tlogAlarm = .true.
  	  aeqssp_automan_GetTlogRec = status(1)

	endif

	end

!++
	integer*4 function aeqSsp_automan_getTlogNext( buffer )
!
!	Read the next transaction record into the buffer
!
	implicit	none
!
!   Includes:
!
	include 'FIL:basicdef.for'
	include 'FIL:filmsg.for'
	include 'MDL:rmsdef.for'
	include 'FIL_CMT:cmtmsg.for'
	include	'AEQ_SSP:AUTODEF'
	include	'AEQ_SSP:AUTOFIL'
	include 'FIL:tlogdef.for'
	include 'FIL:tlogpadef.for'
!
!   Arguments:

        byte            buffer ( TLOG__MAXRECSIZ ) ! WRITE

!   Function value: Either FIL__NORMAL or FIL__EOF
!
!   Routines used:
!
	external	Fil_Check
	external	Fil_Rms_GetRec
	external	Fil_Rms_GetNext
	external	Lib$Signal
!
!
!   Internal Variables:

	integer*4	status(2),
     &			bufflen


	record	/TLOG_record/ tlogRec

!   Code:

	aeqSsp_automan_getTlogNext = FIL__NORMAL
	bufflen = TLOG__MAXRECSIZ

! 	Do a sequential read if we can, otherwise to a random read

	if ( .not. aeqSsp_autofil_tlogNextValid ) then
	!!!call tut_output('GETNEXT:getrec')

    	 call Fil_Rms_GetRec( aeqSsp_autofil_tlogRab, 0, TLOG_S_RECNUM,
     &				 aeqSsp_autofil_tlogNextRec,
     &				 FIL__CMP_EQ, FIL__NOLOCK,
     &				 tlogRec.buffer, bufflen,
     &				 status)
	else
	!!!call tut_output('GETNEXT:getnext')
    	 call Fil_Rms_GetNext( aeqSsp_autofil_tlogRab, FIL__NOLOCK,
     &				tlogRec.buffer, bufflen, status)

	endif

	! Turn various flavors of success status into FIL__EOF. RMS$_OK_RLK 
	! seems to happen if the record is being actively written by the 
	! network side of the secondary comtask.

	if (   status(1) .eq. RMS$_EOF .or. 		!watch for selected ones
     &     status(1) .eq. RMS$_RLK .or.
     &     status(1) .eq. RMS$_RNF .or.
     &     status(1) .eq. RMS$_OK_RLK .or.		!new and likely
     &     status(1) .eq. RMS$_OK_RNF .or.		!new and unlikely
     &     status(1) .eq. RMS$_OK_RRL ) then 		!new and unlikely
	!!!call tut_output('GETNEXT:EOF')

	    aeqSsp_automan_getTlogNext = FIL__EOF	!pretend its EOF
	    aeqSsp_autofil_tlogNextValid = .false.		!read random next time

	else if ( status(1) ) then			!any other ok is ok
	!!!call tut_output('GETNEXT:NORMAL')

	    aeqSsp_automan_getTlogNext = FIL__NORMAL
	    aeqSsp_autofil_tlogNextRec = tlog.recnum
	    aeqSsp_autofil_tlogNextRec = aeqSsp_autofil_tlogNextRec + 1
	    aeqSsp_autofil_tlogNextValid = .true.
	    call gut_movebuff ( bufflen, tlogRec.buffer, buffer )

	else if ( .not. status(1) ) then
		!!!call tut_output('GETNEXT:IO ERROR')

	    call Lib$Signal( %val(CMT__TRANSIO), %val(0),
     &				%val(status(1)), %val(status(2)) )
	  aeqSsp_automan_getTlogNext = status(1)

	endif

	end

!+
	integer*4 function aeqSsp_automan_getTlogLastRec ( buffer )

	! Do a binary search on the transaction file to determine the last
	! record number in the file. If file is empty return TLOG__FIRSTRECNUM - 1

	implicit none

! Include files:

	include 'FIL:basicdef.for'
	include 'MDL:rmsdef.for'
	include 'FIL:filmsg.for'
	include 'AEQ_SSP:AUTODEF'
	include 'AEQ_SSP:AUTOFIL'
	include 'FIL:tlogdef.for'
	include 'FIL:tlogpadef.for'

!   Arguments:

        byte            buffer ( TLOG__MAXRECSIZ ) ! WRITE

! Function value:	Record number of last record in the file

! Routines Used:

	integer*4	Fil_Rms_Rewind
	integer*4	Fil_Rms_GetNext
	integer*4	Fil_Rms_GetRec
	integer*4	Fil_Rms_FindRec
	!subroutine	Lib$Signal


!   Modifications:
!-
! Local variables:

	integer*4 status,firstrec,thisrec,lastrec,bufflen

	record	/TLOG_record/ tlogRec

!Code:
	call Fil_Rms_Rewind (aeqSsp_autoFil_tlogRab, 0 )

	bufflen = TLOG__MAXRECSIZ
	status = Fil_Rms_GetNext (aeqSsp_autoFil_tlogRab, FIL__NOLOCK,
     &					tlogRec.buffer, bufflen)

	! If an empty file
	if ( .not. status ) then

	    firstrec = TLOG__FIRSTRECNUM
	    aeqSsp_automan_getTlogLastRec = FIL__RNF
	!!!call tut_output('LASTREC: RNFu');

	else
	!!!call tut_output('LASTREC: NORMAL');

	    aeqSsp_automan_getTlogLastRec = FIL__NORMAL

	    firstrec = tlogRec.recnum+1
	    lastrec  = TLOG__LASTRECNUM

	    do while ( firstrec .lt. lastrec )

		thisrec = firstrec/2 + lastrec/2
		bufflen = TLOG__MAXRECSIZ
		status = Fil_Rms_GetRec( aeqSsp_autofil_tlogRab, 0, 4, thisrec,
     &					FIL__CMP_EQ, FIL__NOLOCK,
     &					tlogRec.buffer, bufflen)

		if ( status .eq. RMS$_RNF ) then
		    lastrec = thisrec
		else if ( status ) then
		    firstrec = thisrec+1
		else
		    call lib$signal(%val(status))
		endif
	    enddo

	    call gut_movebuff ( bufflen, tlogRec.buffer, buffer )

	endif

	aeqSsp_autoFil_tlogNextRec = firstrec
	aeqSsp_autoFil_tlogNextValid = .false.

	end

	integer*4 function aeqSsp_automan_OpenTlog( )
!
!	Open the transaction log.
!
	implicit	none
!
!   Parameters:
!
	include 'MDL:rmsdef.for'
	include 'MDL:rabdef.for'
	include 'FIL:basicdef.for'
	include 'FIL:filmsg.for'
	include 'FIL_CMT:cmtmsg.for'
	include 'AEQ_SSP:AUTODEF'
	include 'AEQ_SSP:AUTOFIL'
	include 'FIL:tlogdef.for'
	include 'FIL:tlogpadef.for'

!
!   Function value: Success if was opened otherwise rms code why not
!

!
!   Routines used:
!
	external	Fil_Rms_Open
	external	Fil_Rms_Connect
	external	Fil_Rms_SetRop

!   Internal Variables:

	integer*4	status(2),
     &			sharecode

!   Code:

	! Try to open existing file
	sharecode = FIL__LOCALREADONLY
	call Fil_Rms_Open( aeqSsp_autofil_tlogFab, ' ', sharecode,
     &			.true., 'TRANSACT', status)

	if ( status(1) ) then

	    ! Connect synchronously with 5 buffers
	    call Fil_Rms_Connect( aeqSsp_autofil_tlogFab, 
     &				aeqSsp_autofil_tlogRab,
     &				FIL__LOCALREADONLY, .false., 5, status)
	endif

	! Check the status
	if ( .not. status(1) ) then

	   if ( status(1) .eq. RMS$_FLK ) then

	    ! Can happen when new tlog is opened from a PROMIS backup operation
	      aeqSsp_autofil_tlogNextValid = .false.
	      aeqSsp_automan_openTlog = FIL__EOF
	      aeqSsp_autofil_tlogOpen = .false.

	   else

	      call Lib$Signal( %val(CMT__TRANSIO), %val(0),
     &				%val(status(1)), %val(status(2)) )
	      aeqSsp_autofil_tlogNextValid = .false.
	      aeqSsp_autofil_tlogOpen = .false.
	      aeqSsp_automan_openTlog = status(1)

	   endif

	else
 
	   aeqSsp_autofil_tlogNextValid = .true.
	   aeqSsp_autofil_tlogOpen = .true.
	   aeqSsp_automan_openTlog = FIL__NORMAL
	   
	endif


	end

!++
	subroutine aeqSsp_automan_closeTlog()
!
!   Close the transaction log
!
	implicit	none
!
! 	Include files:
!
	include 'MDL:rmsdef.for'
	include 'MDL:rabdef.for'
	include 'FIL:basicdef.for'
	include 'FIL:filmsg.for'
	include 'FIL_CMT:cmtmsg.for'
	include 'AEQ_SSP:AUTODEF'
	include 'AEQ_SSP:AUTOFIL'
	include 'FIL:tlogdef.for'
	include 'FIL:tlogpadef.for'
!
!   Routines used:
!
	external	Fil_Rms_Disconnect
	external	Fil_Rms_Close
!
!   Internal variables
!
	integer*4	status(2)
!
!   Code:

	if (.not. aeqSsp_autofil_tlogOpen) return
	aeqSsp_autofil_tlogOpen = .false.

	! Disconnect the master rab

	call Fil_Rms_Disconnect (aeqSsp_autofil_tlogRab, status)

	! Close the file
	!!!call tut_output('CLOSE: DISCONNECT');

	if (status(1)) call Fil_Rms_Close (aeqSsp_autoFil_tlogFab, status)

	call Gut_Check (status(1))
	!!!call tut_output('CLOSE: NORMAL');

	end

!++
	subroutine aeqSsp_automan_closeLog()
!
!   Close the terminal log

        include 'TUT:TUTDEF.FOR'
        include 'CFC:LUNDEF.FOR'
        include 'TUT:tutcom.for'

        if ( ( tut_flags .and. TUT_M_TERMLOG ) .ne. 0 ) then

          tut_flags = tut_flags .and. .not. TUT_M_TERMLOG
          close(LUN__TERMLOG,status='KEEP')

         endif
	
        end
