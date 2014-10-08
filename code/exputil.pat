>BS
	  !TXT TYPEMISMATCH '!AS is not of the required type !AS'
	  rejectReason = Txt_Fetch(TX_TYPEMISMATCH,
     &				eqpId(:Gut_TrimBlks(eqpId)),
     &		contextData.recpRec.eqpType(:Gut_TrimBlks(contextData.recpRec.eqpType)))
	  go to 1000

	else	! eqpType is OK

>BE
>IS
!!! Paul R. Neveu	14-Feb-2001
!!! Comment out this section of code.  It was incorrectly returning Not Available when capability = ' '
!!! and it is never executed when capability <> ' '.
!!!
>IE
>CS									!#13310-B
	   ! Is eqp not available for this lottype?
           count = Wct_EqpAccess_SelectEqpc (eqpId,
     &              contextData.recpRec.capability, ! capability,
     &                                         'T', ! any capabilityAvail,
     &                contextData.actlRec.location, ! location,
     &                			       '*', ! any prodArea,
     &                 contextData.actlRec.lotType, ! lotType,
     &                                         eqpcArr)

	   if (count .le. 0) then
	      !TXT EQPNOAVAIL1 'Eqp !AS is not available for lot !AS'
	      rejectReason = Txt_Fetch(TX_EQPNOAVAIL1,
     &				eqpId(:Gut_TrimBlks(eqpId)),
     &				lotId(:Gut_TrimBlks(lotId)))

	      go to 1000
	   endif
									!#13310-E
>CE
