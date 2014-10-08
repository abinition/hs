>BS
!B-DEV51-1290
	flagmask(11) = UAF_M_WINDOWEDMENU
!E-DEV51-1290
	flagmask(12) = UAF_M_NOCMDRCL
>BE
>IS
!B-HS!
	flagmask(13) = UAF_M_HYPERSCRIPT
!E-HS!
>IE
>ES
							!E-#9244
	! Initialize text table; used to validate flag value input by user and 
	! output in the screen header.
>EE
>BS
!B-DEV51-1290
	!TXT AUTHFLAGS11 'WINDOWED MENU'
	flagtable(11) = txt_Fetch(TX_AUTHFLAGS11)	!WINDOWED MENU
!E-DEV51-1290

	flagtable(12) = 'NOCOMMAND RECALL'
>BE
>IS
!B-HS!
	flagtable(13) = 'HYPERSCRIPT'
!E-HS!
>IE
>ES
	!Initialize the disable flag

	!TXT DISABLE 'DISABLED '
	disable = txt_fetch(TX_DISABLE)
>EE
