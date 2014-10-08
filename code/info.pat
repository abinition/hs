>BS
c   Code:

	call Uaf_User_Init

	Uaf_Info_MayEscape = ( Uaf_UsrCom_Prt.aut.flags
     &				.and. UAF_M_ESCAPE ) .ne. 0		!#9244
	end
>BE
>IS
!B-HS!

        logical*4 function Uaf_Info_HyperScript
!
!   May user xecute hyperscript
!
        implicit        none
!
!   Parameters:
!
	include 	'FIL:TypDef'
        include         'Fil:FilDef'
        include         'Fil:Authpdef'
        include         'Mep:MepDef'
        include         'Mep:CtxDef'
        include         'Uaf:UafDef'                         
c
c   Function value
c
        ! T/F is can esacpe
c
c   Routines used:

        external        Uaf_User_Init

c   Common blocks
c
        include         'Uaf:UsrCom'
c
c   Code:

	call Uaf_User_Init

        Uaf_Info_HyperScript = ( ( Uaf_UsrCom_Prt.aut.flags
     &                          .and. UAF_M_HYPERSCRIPT ) .ne. 0 ) .or.
     &			       ( ( Uaf_UsrCom_Prt.aut.flags
     &				.and. UAF_M_SUPERUSER ) .ne. 0 )
     
        end
!E-HS!
>IE
>ES
>EE
