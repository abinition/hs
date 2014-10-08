	program test
	implicit none
	call cfc_initimage()        
	call UAF_WRKCTR_ATTACHNONLEAD()
	do while ( .true. )
          call Lot_Tkin_TrackIn
	enddo
        !!!call TST_ENTERDATA()
	end