>BS
        !TXT COMMENTFORABORTSTEP 'Enter comment for aborted step'
	call Lot_AddCommnt_GetComments (actl,
     &					Txt_Fetch(TX_COMMENTFORABORTSTEP),
     &					time)
>BE
>IS
!B-HS!
        call aeqSsp_autoMan_abort ( lot_lotCom_userId )
!E-HS!
>IE
>ES
	! Get the time stamp
	call Gut_Check (Gut_Currentim (time) )
>EE
