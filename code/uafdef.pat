>BS
	parameter	UAF_M_NOCMDRCL = 8192		!VISIBLE	!#5715
	! disallow command recall for this user				!#5715
>BE
>IS
!B-HS!
	parameter	UAF_M_HYPERSCRIPT = 16384
	! turn on Hyperscript access	
>IE
>CS
	parameter 	UAF__VISIBLEFLAGS = 12				!#5715
>CE
>IS
	parameter	UAF__VISIBLEFLAGS = 13
!E-HS!
>IE
>ES
	! this paramter must equal the current number of flags
	! which can be set by the user authorization functions
>EE
