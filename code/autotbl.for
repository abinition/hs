! TST:AUTOTBL.FOR
!
!	Common area for AUTOMAN.FOR routine
!
! Modifications:
!
!    $Log: autotbl.for,v $
!    Revision 1.3  2005/09/25 20:10:40  bergsma
!    Up to V3.6.3
!    Add opcert code.
!    Changes to function VT2HTML
!    Bug in EAGAIN waiting for UNIX fifo's or network sockets
!    AUTOROUTER loigical must be uppercase on VMS
!
!    Revision 1.2  2003/01/16 14:32:07  bergsma
!    V3.1.0
!    Added Modification tags.
!
!
	integer*2			aeqSsp_autoTbl_inChannel,
     &					aeqSsp_autoTbl_outChannel

	integer*4			aeqSsp_autoTbl_connectTries,
     &					aeqSsp_autoTbl_AST_timer(2),
     &					aeqSsp_autoTbl_targetLen,
     &					aeqSsp_autoTbl_senderLen,
     &					aeqSsp_autoTbl_targetInstLen,
     &					aeqSsp_autoTbl_senderInstLen,
     &					aeqSsp_autoTbl_targetObjectLen,
     &					aeqSsp_autoTbl_senderObjectLen,
     &					aeqSsp_autoTbl_targetNodeLen,
     &					aeqSsp_autoTbl_senderNodeLen,
     &					aeqSsp_autoTbl_siteIndex,
     &					aeqSsp_autoTbl_waferIndex,
     &					aeqSsp_autoTbl_itemIndex,
     &					aeqSsp_autoTbl_parmSource,
     &					aeqSsp_autoTbl_routerLen,
     &					aeqSsp_autoTbl_mbxRouterLen


	logical*4			aeqSsp_autoTbl_msgPending,
     &					aeqSsp_autoTbl_sessionStatus,
     &					aeqSsp_autoTbl_connectStatus,
     &					aeqSsp_autoTbl_functionStatus,
     &					aeqSsp_autoTbl_hasLotid,
     &					aeqSsp_autoTbl_hasEnteredData,
     &					aeqSsp_autoTbl_canEnterData,
     &					aeqSsp_autoTbl_pageWait,
     &					aeqSsp_autoTbl_prompt,
     &					aeqSsp_autoTbl_debug,
     &					aeqSsp_autoTbl_halt,
     &					aeqSsp_autoTbl_log,
     &                                  aeqSsp_autoTbl_opcert

	character*4			aeqSsp_autoTbl_state
	character*15			aeqSsp_autoTbl_autoType
	character*(TARGET_SIZE)		aeqSsp_autoTbl_target
	character*(SENDER_SIZE)		aeqSsp_autoTbl_sender
	character*(INSTANCE_SIZE)	aeqSsp_autoTbl_targetInstance
	character*(INSTANCE_SIZE)	aeqSsp_autoTbl_senderInstance
	character*(OBJECT_SIZE)		aeqSsp_autoTbl_targetObject
	character*(OBJECT_SIZE)		aeqSsp_autoTbl_senderObject
	character*(NODE_SIZE)		aeqSsp_autoTbl_targetNode
	character*(NODE_SIZE)		aeqSsp_autoTbl_senderNode
	character*(VALUE_SIZE)		aeqSsp_autoTbl_text3
	character*(TARGET_SIZE)		aeqSsp_autoTbl_router
	character*(TARGET_SIZE)		aeqSsp_autoTbl_mbxRouter

	structure /hotkey_struct/
	  logical*4			isEnabled
	  character*8			keyName
	  integer*4			position
	  character*(METHOD_SIZE)	method
	end structure
	record	/hotkey_struct/		aeqSsp_autoTbl_hotkey(128)
	
	common 	/aeqSsp_autoTbl/	aeqSsp_autoTbl_inChannel,
     &					aeqSsp_autoTbl_outChannel,
     &					aeqSsp_autoTbl_msgPending,
     &					aeqSsp_autoTbl_sessionStatus,
     &					aeqSsp_autoTbl_connectStatus,
     &					aeqSsp_autoTbl_functionStatus,
     &					aeqSsp_autoTbl_parmSource,
     &					aeqSsp_autoTbl_hasLotid,
     &					aeqSsp_autoTbl_hasEnteredData,
     &					aeqSsp_autoTbl_canEnterData,
     &					aeqSsp_autoTbl_pageWait,
     &					aeqSsp_autoTbl_prompt,
     &					aeqSsp_autoTbl_debug,
     &					aeqSsp_autoTbl_halt,
     &					aeqSsp_autoTbl_log,
     &                                  aeqSsp_autoTbl_opcert,
     &					aeqSsp_autoTbl_AST_timer,
     &					aeqSsp_autoTbl_connectTries,
     &					aeqSsp_autoTbl_waferIndex,
     &					aeqSsp_autoTbl_siteIndex,
     &					aeqSsp_autoTbl_itemIndex,
     &					aeqSsp_autoTbl_state,
     &					aeqSsp_autoTbl_autoType,
     &					aeqSsp_autoTbl_targetLen,
     &					aeqSsp_autoTbl_senderLen,
     &					aeqSsp_autoTbl_targetInstLen,
     &					aeqSsp_autoTbl_senderInstLen,
     &					aeqSsp_autoTbl_targetObjectLen,
     &					aeqSsp_autoTbl_senderObjectLen,
     &					aeqSsp_autoTbl_targetNodeLen,
     &					aeqSsp_autoTbl_senderNodeLen,
     &					aeqSsp_autoTbl_target,
     &					aeqSsp_autoTbl_sender,
     &					aeqSsp_autoTbl_targetInstance,
     &					aeqSsp_autoTbl_senderInstance,
     &					aeqSsp_autoTbl_targetObject,
     &					aeqSsp_autoTbl_senderObject,
     &					aeqSsp_autoTbl_targetNode,
     &					aeqSsp_autoTbl_senderNode,
     &					aeqSsp_autoTbl_text3,
     &					aeqSsp_autoTbl_router,
     &					aeqSsp_autoTbl_routerLen,
     &					aeqSsp_autoTbl_mbxRouter,
     &					aeqSsp_autoTbl_mbxRouterLen,
     &					aeqSsp_autoTbl_hotkey
