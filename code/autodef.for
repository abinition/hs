! TST:AUTODEF.FOR 
!
!	Include file for AUTOSERVER.FOR, AUTOCLIENT.FOR, AUTOUTIL.FOR
! 
	parameter	AUTOMAN_VERSION = 'AM V3.4-0'
!
! Modifications:
!
!	$Log: autodef.for,v $
!	Revision 1.26  2004/11/19 03:51:40  bergsma
!	V 3.4.0
!	
!	Revision 1.25  2004/11/02 22:59:06  bergsma
!	V 3.3.3
!	
!	Revision 1.24  2004/10/27 18:24:07  bergsma
!	HS 3.3.2
!	1. Fix bug with SQL read when str size is > 512 bytes, use _data_ blobs
!	2. Fix bug with XML output, forgetting ";" in unicode output.
!	3. In |TOKEN|VALUE|VALUE| part of message, use unparse on TOKEN
!	as well as VALUE.
!	4. Add utility function util_breakStream.
!	
!	Revision 1.23  2004/10/16 04:26:32  bergsma
!	AM V3.3-1
!	
!	Revision 1.22  2004/07/23 18:39:44  bergsma
!	Version 3.3.0
!	
!	Revision 1.21  2004/07/01 02:02:55  bergsma
!	Version 3.2.3
!	
!	Revision 1.20  2004/06/13 14:02:02  bergsma
!	Version 3.2.2
!	
!	Revision 1.19  2004/05/15 02:11:21  bergsma
!	Version 3.2.1
!	
!	Revision 1.18  2004/02/29 14:03:28  bergsma
!	Version 3.2
!	
!	Revision 1.17  2003/11/16 21:02:09  bergsma
!	Version 3.1.7
!	
!	Revision 1.16  2003/07/27 01:01:47  bergsma
!	Version 3.1.6
!	
!	Revision 1.15  2003/06/05 22:14:18  bergsma
!	Version 3.1.5
!	
!	Revision 1.14  2003/05/16 05:22:45  bergsma
!	Version 3.1.4
!	
!	Revision 1.13  2003/04/23 22:52:24  bergsma
!	Version 3.1.3
!	
!	Revision 1.12  2003/04/16 01:46:52  bergsma
!	Version 3.1.2. See release notes.
!	
!	Revision 1.11  2003/04/11 02:18:51  bergsma
!	Missing tab..
!	
!	Revision 1.10  2003/04/04 16:38:49  bergsma
!	V3.1.1
!	
!	Revision 1.9  2003/01/14 02:31:08  bergsma
!	Relabel Automan version to "AM 3.1-0a"
!	
!	Revision 1.8  2002/11/12 20:24:08  bergsma
!	Change to batch.for, added call to fil_open(FIL__OPER,FIL__READONLY)
!	
!	Revision 1.7  2002/10/27 14:31:16  bergsma
!	Revision 3.0.6
!	
!	Revision 1.6  2002/10/12 02:24:53  bergsma
!	Version 3.0.5a
!	
!	Revision 1.5  2002/09/16 15:20:38  bergsma
!	For new HyperScript revision HS-3.04
!	
!	Revision 1.4  2002/09/09 20:39:34  bergsma
!	New version HyperScript 3.0-3
!	
!	Revision 1.3  2002/09/04 23:14:06  bergsma
!	Update AutoMan Revision.  Revision is 'a', but the HyperScript
!	version is the prefix, so AM 3.0-2a means HS 3.0-2 and AutoMan 'a'.
!	
!	Revision 1.2  2002/09/03 21:39:53  bergsma
!	Revision change, AM 3.0-1, parallel with HS 3.0-1
!	
!

	! Event flags
	parameter	EFN_ATTN = 32
	parameter	EFN_DNET_IN = 33
	parameter	EFN_DNET_OUT = 34
	parameter	EFN_MBX_IN = 35
	parameter	EFN_MBX_OUT = 36
	parameter	EFN_NET = 37
	parameter	EFN_TIMER = 38
	parameter	EFN_LOG = 39
	parameter	EFN_WAITFR = 40

	! Mailbox names
	parameter	ROUTER		= 'ROUTER'
	parameter	MBX		= 'MBX_'
	parameter	MBX_ROUTER	= MBX//ROUTER
	parameter	MBX_NET		= 'MBX_NET'

	! NETWORK
	parameter	MAX_NODES = 10
	parameter	MAX_NCB = 110	! From SYS$EXAMPLES:DB_SERVER.C

	! Logical unit numbers
	parameter	LOG_UNIT = 94

	! Retry timer and number of retries (total of 15x3=45 seconds)
	parameter	CONNECT_TIMER = '0 00:00:03.00'
	parameter	CONNECT_TRIES = 15

	! Every GARBAGE_TIMER time, we check to see if temporary mailboxes have
	! been inactive for more than GARBAGE_INTERVAL time.
	parameter	GARBAGE_TIMER    = '0 00:10:00.00'
	parameter	GARBAGE_INTERVAL = '0 00:30:00.00'
	parameter	TIMESTAMP_FORMAT = '|!Y4!MN0!D0|!H04!M0!S0|'
	parameter	DATETIME_SIZE =	15
	parameter	DATE_SIZE =	8
	parameter	TIME_SIZE =	6

	! Mailbox protections
	parameter	P_WRITE_ONLY 		= 1
	parameter	P_READ_ONLY 		= 2
	parameter	P_NO_ACCESS 		= 4
	parameter	P_READ_WRITE 		= 0

	parameter	P_RO = P_READ_WRITE +			! System
     &			       (P_READ_WRITE * 16) +		! Owner
     &			       (P_READ_ONLY * 16*16) +		! Group
     &			       (P_READ_ONLY * 16*16*16) 	! World

	parameter	P_WO = P_READ_WRITE +			! System
     &			       (P_READ_WRITE * 16) +		! Owner
     &			       (P_WRITE_ONLY * 16*16) +		! Group
     &			       (P_WRITE_ONLY * 16*16*16) 	! World

	parameter	P_RW = P_READ_WRITE +			! System
     &			       (P_READ_WRITE * 16) +		! Owner
     &			       (P_READ_WRITE * 16*16) +		! Group
     &			       (P_READ_WRITE * 16*16*16) 	! World

	! Mailbox sizing
	parameter	MAX_ROUTER_MESSAGES = 16
	parameter	MAX_INSTANCE_MESSAGES = 4
	parameter	MAX_MESSAGES = MAX_INSTANCE_MESSAGES 
	parameter	MAX_TARGETS  = 64

	! Message sizing
	parameter	MESSAGE_SIZE 	= 5120
	parameter	MAX_BUFFER_SIZE = MESSAGE_SIZE*2
	parameter	MAX_TOKENS	= 32
	parameter	MAX_VALUES 	= 128
	parameter	MAX_FIELDS	= 2

	! Message component sizing
	parameter	INSTANCE_SIZE = 32
	parameter	OBJECT_SIZE = 120
	parameter	NODE_SIZE = 48
	parameter	TARGET_SIZE = INSTANCE_SIZE + OBJECT_SIZE + NODE_SIZE + 2
	parameter	MODE_SIZE   = 10
	parameter	METHOD_SIZE = 32
	parameter	SENDER_SIZE = TARGET_SIZE
	parameter	TOKEN_SIZE  = 64
	parameter	VALUE_SIZE  = 256
	parameter	FIELD_SIZE  = 20

	! Header FIELD designations
	parameter	FIELD_TID		= 1
	parameter	FIELD_TIMESTAMP		= 2

	! Message delimiter
	parameter	DEFAULT_DELIMITER = '|'

	! User interface component positions and sizing
	parameter	MESSAGE_START_COL	= 3
	parameter	MESSAGE_LINE_LEN	= 80 - MESSAGE_START_COL - 2
	parameter	HEADER_ROW_1		= 1
	parameter	HEADER_ROW_2		= 2
	parameter	HEADER_ROW_3		= 3
	parameter	HEADER_ROW_4		= 4
	parameter	HEADER_ROW_5		= 5
	parameter	TOP_LINE_ROW		= 6
	parameter	MESSAGE_START_ROW	= 7
	parameter	MESSAGE_END_ROW		= MESSAGE_START_ROW + 5
	parameter	MAX_MESSAGE_LINES	= 
     &		( MESSAGE_END_ROW - MESSAGE_START_ROW + 1 )
	parameter	MAX_MESSAGE_LEN		= 
     &		( MAX_MESSAGE_LINES * MESSAGE_LINE_LEN )
	parameter	SECOND_LINE_ROW		= MESSAGE_END_ROW + 1
	parameter	COMMAND_ROW		= SECOND_LINE_ROW + 1
	parameter	THIRD_LINE_ROW		= COMMAND_ROW + 1
	parameter	MESSAGE2_START_ROW	= THIRD_LINE_ROW + 1
	parameter	MESSAGE2_END_ROW	= MESSAGE2_START_ROW + 3
	parameter	MAX_MESSAGE2_LINES	= 
     &		( MESSAGE2_END_ROW - MESSAGE2_START_ROW + 1 )
	parameter	MAX_MESSAGE2_LEN		= 
     &		( MAX_MESSAGE2_LINES * MESSAGE_LINE_LEN )
	parameter	BOTTOM_LINE_ROW		= MESSAGE2_END_ROW + 1

	! Data collection special index identifiers
	parameter	INDEX_UNDEFINED = 0
	parameter	INDEX_UNSPECIFIED = -1

	! Batch trackin
	parameter	MAX_BATCH_LOTS = 20
	parameter	MAX_INIT_PARMS = 10

	! Database
	parameter	MAX_AUTO_FILES = 20

