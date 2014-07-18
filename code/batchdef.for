!++ BATCHDEF.FOR
!
! Modifications:
!
!    $Log: batchdef.for,v $
!    Revision 1.2  2003-01-16 14:32:07  bergsma
!    V3.1.0
!    Added Modification tags.
!
!
!
	parameter	MAX__BATCH_LOTS	= 20

	structure /batch_struct/

	  integer*4			lotCount
	  integer*4			firstlot
	  logical*4			isAddtoBatch
	  character*(TYP__LOTIDSZ)	template
	  character*(TYP__LOTTYPESZ)	templateLotType
	  character*(TYP__LOTIDSZ)	lotid(MAX__BATCH_LOTS)
	  character*(TYP__LOTTYPESZ)	lottype(MAX__BATCH_LOTS)
	  integer*4			numComps(MAX__BATCH_LOTS)
	  integer*4			priority(MAX__BATCH_LOTS)
	  character*(TYP__PARTIDSZ)	partid(MAX__BATCH_LOTS)
	  character*(TYP__PRCDIDSZ)	procid(MAX__BATCH_LOTS)
	  character*(TYP__PINSINSTNUMSZ) step(MAX__BATCH_LOTS)
	  character*(TYP__STAGEIDSZ)	stage(MAX__BATCH_LOTS)
	  character*(TYP__RECPIDSZ)	recpid(MAX__BATCH_LOTS)
	  character*(VALUE_SIZE)	route(MAX__BATCH_LOTS)
	  logical*4			hasAltRecipe(MAX__BATCH_LOTS)
	  integer*4			numAltRecipe(MAX__BATCH_LOTS)
	  logical*4			isTrackedIn(MAX__BATCH_LOTS)

	  integer*4			numParms
	  character*(TYP__PARMNMSZ)	parmName(TYP__MAXRECPPARMS) 
	  character*(TYP__PARMVALSZ) 	parmVal( MAX__BATCH_LOTS,
     &						 TYP__MAXRECPPARMS )
	  logical*4			parmIsHard(TYP__MAXRECPPARMS )
	  logical*4			parmIsInit(TYP__MAXRECPPARMS )

	  character*6			basis		! WAFERS or LOTS
	  character*(TYP__USERIDSZ) 	userId
	  character*(TYP__PRODAREASZ)	prodarea
	  character*(TYP__LOCATIONIDSZ) curLocation
	  character*(TYP__RECPIDSZ)	mainRecpid
	  character*(TYP__COMMENTSZ)	title		! From recp.descr
	  character*(TYP__EQPIDSZ)	eqpid
	  character*(TYP__EQPTYPSZ)	eqpType
	  character*(TYP__CAPABILITYSZ)	eqpCapability
	  character*(TYP__EQPSTATUSSZ)	eqpStatus
	  logical*4			eqpBsyEnabled
	  character*(TYP__QTYATTRSZ)	capAttr
	  real*4			maxProcCap
	  real*4			remCap
	  real*4			loadSize

	  logical*4			log
	  character*60			saveOption
	  integer*4			saveOptionLen
	  logical*4			sendevent

	end structure

