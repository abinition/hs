!*****************************************************************************!
!                    AUTOMAN Software Source Code.                            !
!                                                                             !
!          ***** Copyright: (c) 1993 Ab Initio Software                       !
!          ***** All rights reserved - Licensed Material.                     !
!          ***** Program property of Ab Initio Software                       !
!                                                                             !
!*****************************************************************************!
!++ AUTOMAN5.FOR

! The AUTOmation MANagement routines are distributed into three modules:


! Externally Callable Routines:


! For Internal Use Only:

!	aeqSsp_autoMan_crembx		! Create a mailbox


! Produced by:	AbInition 

! Author:	Michael Bergsma 	2009

! Modifications:
!
!   $Log: automan5.for,v $
!   Revision 1.1  2009-06-12 05:11:49  bergsma
!   A FORTRAN way to do $CREMBX
!
!   
!

!++ AEQSSP_AUTOMAN_CREMBX
!
	integer*4 function aeqSsp_autoMan_crembx ( mbxName, maxmsg, bufquo, mbxChannel )

	implicit none

	! Create a mailbox on OpenVMS

! Include Files:

	include '($PSLDEF)'

! Arguments:

	character*(*)	mbxName					! [R]
	! - the name of the mailbox

	integer*4	maxmsg					! [R]
	! - the socket created

	integer*4	bufquo					! [R]
	! - the socket created		

	integer*4	mbxChannel				! [W]
	! - the socket created

! Function value:
!
!	- returns success or failure

! Routines Used:

	integer*4	sys$crembx

! Author:	Mike Bergsma		6/09
!		AbInition Inc
!
! Modifications:
!
!
! Local Variables:

	integer*4	status
        integer*2       channel ;
! Code:

        !call GHYP_UTIL_DEBUG ( '%s', mbxName ) 
        !call GHYP_UTIL_DEBUG ( '%d', maxmsg ) 
        !call GHYP_UTIL_DEBUG ( '%d', bufquo ) 
	! Enable write attention AST
	status = sys$crembx(	,		! Default argument
     &				%ref(channel),	        ! The new channel
     &				%val(maxmsg),		! Max message size
     &				%val(bufquo),		! Total buffer quota
     &				,		! Protection mask
     &				%val(PSL$C_USER),	! Mode
     &                    	mbxName )		! Mailbox name

        MBXchannel = channel 
        !call GHYP_UTIL_DEBUG ( '%d', channel ) 
 
!	if ( .not. aeqSsp_autoMan_check( status, 1 ) ) then
!         call aeqSsp_autoMan_outMessage( 
!    &		'% Error creating mailbox. '//mbxName//'.'//
!     &		'This is an internal program error. '//
!     &		'Please try logging completely out and then logging '//
!     &		'back in; or contact your facilitator.',
!     &		.true. )
!

	  aeqSsp_autoMan_crembx = status

	return

	end
