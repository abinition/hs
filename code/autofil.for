! AEQ_SSP:AUTOFIL.FOR
!
! Modifications:
!
! $Log: autofil.for,v $
! Revision 1.3  2005/04/22 19:23:50  bergsma
! When reading TLOG records, use only one slot in aeqssp_fildata, this prevents
! an overflow in the array.
!
! Revision 1.2  2002/09/16 15:16:47  bergsma
! Added packstart and packsize so that unpacking a PROMIS record could be
! delayed as long as possible.
!
!
!
	parameter	MAX_RECORDSIZE = 32108

	structure / field /
	  character*60	component
	  integer*4	componentLen
	  integer*4	componentIndex
	end structure

	structure / PROMIS_file /
	  union
	     map
		character*4	string
	     end map
	     map
		integer*4	id
	     end map
	  end union
	  integer*4		index
	end structure

	structure / PROMIS_field /
	  integer*4		type
	  integer*4		id
	  integer*4		length
	  integer*4		offset
	  union
	    map
	      character*(VALUE_SIZE)	a_string
	    end map
	    map
	      byte              a_byte
	    end map
	    map
	      integer*2		a_word
	    end map
	    map
	      integer*4		a_long
	    end map
	    map
	      real*4		a_float
	    end map
	    map
	      real*8		a_double
	    end map
	    map
	     logical*4		a_logical
	    end map
	  end union
	end structure

	structure /PROMIS_record/
	  union
	    map
	      byte            buffer( MAX_RECORDSIZE )
	    end map
	    map
	      character*(MAX_RECORDSIZE) cbuffer
	    end map
	  end union
	end structure

	! File storage
	integer*4		aeqSsp_autoFil_numFiles
	integer*4		aeqSsp_autoFil_fileId( MAX_AUTO_FILES )
	logical*4		aeqSsp_autoFil_isOpen ( MAX_AUTO_FILES )
	logical*4		aeqSsp_autoFil_isFresh ( MAX_AUTO_FILES )
	logical*4		aeqSsp_autoFil_isFromTLOG ( MAX_AUTO_FILES )
        integer*4               aeqSsp_autoFil_packStart ( MAX_AUTO_FILES )
        integer*4               aeqSsp_autoFil_packSize ( MAX_AUTO_FILES )


	record /PROMIS_record/ 	aeqSsp_autoFil_file( MAX_AUTO_FILES )
	record /PROMIS_record/ 	aeqSsp_autoFil_ACTL 
	record /PROMIS_record/ 	aeqSsp_autoFil_RECP 
	record /PROMIS_record/ 	aeqSsp_autoFil_TEST 
	record /PROMIS_record/ 	aeqSsp_autoFil_TRES 

	! TLOG stuff
	! Rab and fab for the tlog
	integer*4	aeqSsp_autoFil_tlogFab
	integer*4	aeqSsp_autoFil_tlogRab
	integer*4	aeqSsp_autoFil_tlogRab2

	! Next tlog record to apply
	integer*4	aeqSsp_autofil_tlogNextRec

	! Whether we can do a next or must do a getrec
	logical*1	aeqSsp_autofil_tlogNextValid

	! Has error occurred on the file
	logical*1	aeqSsp_autofil_tlogAlarm	/.false./

	! Is Tlog currently open
	logical*1	aeqSsp_autofil_tlogOpen		/.false./

	common /autofil/	aeqSsp_autoFil_numFiles,
     &				aeqSsp_autoFil_fileId,
     &				aeqSsp_autoFil_isOpen,
     &                          aeqSsp_autoFil_packStart,
     &                          aeqSsp_autoFil_packSize,
     &				aeqSsp_autoFil_isFresh,
     &				aeqSsp_autoFil_isFromTLOG,
     &				aeqSsp_autoFil_file,
     &				aeqSsp_autoFil_ACTL,
     &                          aeqSsp_autoFil_RECP,
     &                          aeqSsp_autoFil_TEST,
     &                          aeqSsp_autoFil_TRES,
     &				aeqSsp_autofil_tlogFab,
     &				aeqSsp_autofil_tlogRab,
     &				aeqSsp_autofil_tlogRab2,
     &				aeqSsp_autofil_tlogNextRec,
     &				aeqSsp_autofil_tlogNextValid,
     &				aeqSsp_autofil_tlogAlarm,
     &				aeqSsp_autofil_tlogOpen
