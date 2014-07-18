>BS
	     if (Uaf_CheckSmod(UAF__TMS)) then
		talkOn = .true.
		tmsStatus = Tms_TrackIn_Tally (actl, talkOn, tmsStatusArray,
     &								dummy) !#10683
	     end if
!DEV51-1295/TEST51-313 End
>BE
>IS
!B-HS!
	     call aeqSsp_autoMan_recipe( lot_lotCom_userId, 'TRACKIN' )
!E-HS!
>IE
>ES
	     ! Update actl and set lot state to RUNNING
>EE
