>BS
	! set up entry flags, prompt to begin entry or stop:
	call Tst_EnterData_Setup (finished,changed,time,
     &	    			  oldAuditStatus)			!#5800
>BE
>IS
!B-HS!
	call aeqSsp_autoMan_dcop( finished )

!E-HS!
>IE
>ES
	status = tst_tstWcom_startentarray(TST_ENTDEF_CL_STATUS)

100	continue	! come back here if we re-test
	olddata = tst_tstWcom_startentarray(TST_ENTDEF_CL_DATA)
>EE
