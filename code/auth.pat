>BS
	value(9) = UAF_M_NOPASSCHANGE
	value(10) = UAF_M_USESYSTEMMAIL
	value(11) = UAF_M_WINDOWEDMENU				!DEV51-1290
	value(12) = UAF_M_NOCMDRCL				!#5715
>BE
>IS
!B-HS!
	value(13) = UAF_M_HYPERSCRIPT
!E-HS!
>IE
>ES									!E-#9244
        !TXT AUTHFLAGS1 'SUPERUSER'
	table(1) = txt_fetch(TX_AUTHFLAGS1)		!SUPERUSER
        !TXT AUTHFLAGS2 'GENERIC'
>EE
>BS
	!TXT AUTHFLAGS11 'WINDOWED MENU'					!DEV51-1290
	table(11) = txt_Fetch(TX_AUTHFLAGS11)		!WINDOWED MENUS		!DEV51-1290
	!TXT AUTHFLAGS12 'NOCOMMAND RECALL'					!#5715
	table(12) = txt_Fetch(TX_AUTHFLAGS12)		!No command recall	!#5715
>BE
>IS
!B-HS!
	table(13) = 'HYPERSCRIPT'
!E-HS!
>IE
>ES
	do while ( .true. )
>EE
>BS
	masks(9) = UAF_M_NOPASSCHANGE
	masks(10)= UAF_M_USESYSTEMMAIL
	masks(11)= UAF_M_WINDOWEDMENU		!DEV51-1290
	masks(12)= UAF_M_NOCMDRCL		!#5715
>BE
>IS
!B-HS!
	masks(13) = UAF_M_HYPERSCRIPT
!E-HS!	
>IE
>ES									!E-#9244
        !TXT FLAGSNAME1 'Superuser'
	names(1) = txt_fetch(TX_FLAGSNAME1)	
        !TXT FLAGSNAME2 'Generic'
>EE
>BS
	names(11) = txt_fetch(TX_FLAGSNAME11)			!DEV51-1290
	!TXT FLAGSNAME12 'No Command Recall'			!#5715
	names(12) = txt_fetch(TX_FLAGSNAME12)			!#5715
>BE
>IS
!B-HS!
	names(13) = 'HyperScript'
!E-HS!
>IE
>ES
	linelen = 0
        !TXT FLAGSTITLE 'Flags             :'
	header = txt_fetch(TX_FLAGSTITLE)
>EE
