! TST:AUTOCOM.FOR
!
!	Common area for AUTOROUTER.FOR routines
!
! Modifications:
!
!    $Log: autocom.for,v $
!    Revision 1.2  2003/01/16 14:32:07  bergsma
!    V3.1.0
!    Added Modification tags.
!
!
	integer*2			aeqSSP_autoCom_MBXchannel

	  ! The channel for the MBX_ROUTER write-only mailbox.
	  ! Messages from local objects are put into this mailbox and
	  ! AUTOROUTER will route them to their destination; either
	  ! to another local object via a mailbox (MBX_instance_object) or
	  ! to a remote object via a DECnet link


	integer*2			aeqSSP_autoCom_NETdevChannel
	integer*2			aeqSSP_autoCom_NETmbxChannel

	  ! The NETdevChannel is a channel to _NET: from which the AUTOROUTER 
	  ! declares itself a network object.  
	  ! The NETmbxChannel is a channel to MBX_NET, associated with 
	  ! _NET:, in which the AUTOROUTER receives network commands.  


	integer*4			aeqSsp_autoCom_garbageTimer(2)
	integer*4			aeqSsp_autoCom_garbageInterval(2)

	  ! These variables are used to manage MBX_instance_object mailboxes,
	  ! deleting them when they have had no activity for a certain length
	  ! of time

	
	logical*4			aeqSsp_autoCom_shutdown

	  ! If true, then shutdown AUTOROUTER


	character*(NODE_SIZE)		aeqSsp_autoCom_localNode
	integer*4			aeqSsp_autoCom_localNodeLen

	  ! Local node info

	structure /net_struct/
	  integer*2			channel
	  integer*2			unit
	  character*(NODE_SIZE)		node
	  character*(MESSAGE_SIZE)	message
	  integer*2			iosb(4)
	end structure
	record /net_struct/ 		aeqSSP_autoCom_net(MAX_NODES)

	  ! Contains channel names for all remote nodes that the AUTOROUTER
	  ! has connected to.


	structure /mbx_struct/
	  integer*2			channel
	  character*(TARGET_SIZE)	target
	  logical*4			isTemporary
	  character*(DATETIME_SIZE)	timeStamp
	end structure
	record /mbx_struct/ 		aeqSSP_autoCom_mbx(MAX_TARGETS)

	  ! Contains channel names for all local mailboxes that the AUTOROUTER
	  ! has connected to.


	common 	/aeqSsp_autoCom/	aeqSSP_autoCom_MBXchannel,
     &					aeqSSP_autoCom_NETdevChannel,
     &					aeqSSP_autoCom_NETmbxChannel,
     &					aeqSsp_autoCom_shutdown,
     &					aeqSsp_autoCom_garbageTimer,
     &					aeqSsp_autoCom_garbageInterval,
     &					aeqSsp_autoCom_localNodeLen,
     &					aeqSsp_autoCom_localNode,
     &					aeqSSP_autoCom_net,
     &					aeqSSP_autoCom_mbx
