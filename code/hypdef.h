/*****************************************************************************!
!                HyperScript Software Source Code.                            !
!                                                                             !
!          ***** Copyright: (c) 1994 Ab Initio Software                       !
!          ***** All rights reserved - Licensed Material.                     !
!          ***** Program property of Ab Initio Software                       !
!                                                                             !
!*****************************************************************************/

#ifndef __HYPDEF_H_
#define __HYPDEF_H_

#define 	VERSION_HYPERSCRIPT	    "3.9.4"
#define 	VERSION_BUILD		    "140115"
#define 	VERSION_HYPERSCRIPT_BUILD   "3.9.4-140115"

/* Modification history:
 *
 * $Log: hypdef.h,v $
 * Revision 1.147  2013-05-21 17:46:26  bergsma
 * Add secs_map & secp_unmap.  Deal with 64-bit systems where long and
 * int datatypes are 32 bit.  HS long,ulong,and int are 32 bit.
 *
 * Revision 1.146  2013-05-15 16:42:31  bergsma
 * Fix memory issue with gpEnv/joutput
 *
 * Revision 1.145  2013-02-14 07:03:59  bergsma
 * HS3.9.4-130214
 *
 * Revision 1.144  2013-02-06 04:11:30  bergsma
 * HS.3.9.4-130204
 *
 * Revision 1.143  2013-01-08 00:59:39  bergsma
 * Endianess fix for map/unmap
 *
 * Revision 1.142  2013-01-01 22:32:39  bergsma
 * 3.9.4-130101
 *
 * Revision 1.140  2012-08-11 00:16:16  bergsma
 * V3.9.4 120810
 *
 * Revision 1.139  2012-07-20 16:36:58  bergsma
 * Increased MAX_CONNECT_TRIES from 3 to 5.
 * Version 3.9.4-120707
 *
 * Revision 1.138  2012-06-03 15:09:52  bergsma
 * Let strtok have its own 2KB of memory.  For PROMIS tlog.
 *
 * Revision 1.137  2012-05-01 17:52:41  bergsma
 * New buiild 120430
 *
 * Revision 1.136  2012-02-14 19:41:26  bergsma
 * Completed fix of ENQ contention
 *
 * Revision 1.135  2012-02-09 22:52:49  bergsma
 * Release candidate HS3.9.4-120209 - fix replyMessage[outgoingDepth]
 *
 * Revision 1.134  2012-01-29 21:18:20  bergsma
 * Build 3.9.4 - 120202
 *
 * Revision 1.133  2012-01-19 03:31:27  bergsma
 * Build 120118
 *
 * Revision 1.132  2012-01-16 16:56:28  bergsma
 * More fixes for enq contention in gHyp_instance_replyMessage
 *
 * Revision 1.131  2012-01-08 01:27:17  bergsma
 * Version 120107
 *
 * Revision 1.130  2011-12-24 03:53:19  bergsma
 * New revision
 *
 * Revision 1.129  2011-09-29 08:44:46  bergsma
 * Fix issue with replyMessage and ENQ contention
 *
 * Revision 1.128  2011-09-20 05:36:12  bergsma
 * Version update.
 *
 * Revision 1.127  2011-09-02 21:12:41  bergsma
 * V 3.9.5
 *
 * Revision 1.126  2011-07-03 16:35:19  bergsma
 * Added load_csv and csv
 *
 * Revision 1.125  2011-06-03 23:16:53  bergsma
 * New Build
 *
 * Revision 1.124  2011-05-30 17:06:24  bergsma
 * Build 5/30/2011
 *
 * Revision 1.123  2011-05-19 22:11:18  bergsma
 * 110505
 *
 * Revision 1.122  2011-04-10 17:17:07  bergsma
 * Build HS 3.9.4 07-April-2011
 *
 * Revision 1.121  2011-03-06 21:44:07  bergsma
 * Change MAX_RESEND from 1000 to 3.   Why was it sooo high?
 *
 * Revision 1.120  2011-02-24 23:55:34  bergsma
 * Change some PROTOCOL debugs to DIAGNOSTIC debugs.
 * Get HSDOM working.
 *
 * Revision 1.119  2011-02-24 05:12:27  bergsma
 * Adjusting values for MAX_OUTPUT_LENGTH and MAX_INPUT_LENGTH
 *
 * Revision 1.118  2011-02-24 03:13:32  bergsma
 * Adjusting values for MAX_OUTPUT_LENGTH and MAX_INPUT_LENGTH
 *
 * Revision 1.117  2011-02-20 00:59:02  bergsma
 * no message
 *
 * Revision 1.116  2011-02-19 23:05:36  bergsma
 * New standards:
 * MAX_OUTPUT_LENGTH	2000  (allow for formatting characters)
 * MAX_INPUT_LENGTH	2048  (external - vi cannot display more than this)
 * MAX_LOG_LENGTH	1024   (log files)
 * MAX_TERMINAL_LENGTH	MAX_LOG_LENGTH (terminal screen)
 * MAX_BUFFER_FIFO         5120  (fifos)
 * MAX_MESSAGE_SIZE        5120  (tcp)
 * MAX_BUFFER_LENGTH       MAX_MESSAGE_LENGTH *2  (tcp buffering)
 * PORT_READ,_SIZE             4096  (ideal tcp block read/write size)
 * PORT_WRITE_SIZE             4096
 *
 * Revision 1.115  2011-01-08 21:31:59  bergsma
 * Placeholders for +=, -=, *=, /=, %=.
 * Support of expression function calls.  Ie  object.count().
 * Increased HSMS message size to 64K.
 *
 * Revision 1.114  2010-07-05 16:01:01  bergsma
 * Build rev
 *
 * Revision 1.113  2010-06-26 06:36:42  bergsma
 * Build rev,
 *
 * Revision 1.112  2010-05-27 03:58:46  bergsma
 * New build.
 *
 * Revision 1.111  2010-04-13 21:09:05  bergsma
 * New Build 100413
 *
 * Revision 1.110  2010-04-08 16:24:31  bergsma
 * Recordid == "D" special handling, fix 2, initialize buffer to clear garbage.
 *
 * Revision 1.109  2010-04-06 14:10:44  bergsma
 * Handle PROMIS TLOG "D" records.
 *
 * Revision 1.108  2010-03-17 08:18:28  bergsma
 * Version increase to 3.9.1
 *
 * Revision 1.107  2010-03-05 06:18:00  bergsma
 * New Build 100223
 *
 * Revision 1.106  2010-02-01 22:27:18  bergsma
 * Builld 100130, HS 3.9.0
 *
 * Revision 1.105  2010-01-28 04:26:34  bergsma
 * Build 3.9 1/25/2010
 *
 * Revision 1.104  2010-01-16 18:34:11  bergsma
 * Increase input stream for XML.
 *
 * Revision 1.103  2010-01-15 08:31:32  bergsma
 * Increase XML input buffers for parsing
 *
 * Revision 1.102  2010-01-15 08:30:42  bergsma
 * Increase XML input buffers for parsing
 *
 * Revision 1.101  2010-01-08 02:44:57  bergsma
 * Added ssl_md5(), enhanced ssl_digest.
 * Fixed urldecode, missing ":"
 * Enabled object calls, ie:  text.strtok( ) and the like...
 *
 * Revision 1.100  2009-12-24 15:56:55  bergsma
 * More fixes for handling form data via HTTP
 *
 * Revision 1.99  2009-12-14 16:16:25  bergsma
 * V 3.9.0  Build 091214 : WWW Production Version.  XML <--> HS
 *
 * Revision 1.98  2009-12-08 20:47:14  bergsma
 * Increment build number.
 *
 * Revision 1.97  2009-11-17 16:05:30  bergsma
 * Build on 11/17/09, contains XML<-->HS conversion fixes,
 * plus also fixes ENQ Contention
 *
 * Revision 1.96  2009-10-30 21:03:13  bergsma
 * Upgraded.
 *
 * Revision 1.95  2009-10-22 15:57:13  bergsma
 * Increase TOKEN_SIZE to 256
 *
 * Revision 1.94  2009-10-20 16:31:38  bergsma
 * 3.8.9-091005 for BATCH function change (again) for corrupted WIP
 * tables.
 *
 * Revision 1.93  2009-10-09 13:26:03  bergsma
 * Germany - October 2009 - Updates
 *
 * Revision 1.92  2009-09-21 05:18:33  bergsma
 * HS 3.8.8
 *
 * Revision 1.91  2009-06-23 23:21:12  bergsma
 * HS 3.8.6 PF Milestone
 *
 * Revision 1.90  2009-06-14 13:01:43  bergsma
 * Post HS_385 Fixes - some functions such as port_binary, port_eagain,
 * were not actually working (enabled).
 *
 * Revision 1.89  2009-06-12 05:03:44  bergsma
 * HS 385 Final Checkin and TAG - Force PIPE_BUF to be 5120
 *
 * Revision 1.88  2009-04-09 19:57:21  bergsma
 * Fixing issues with HTTP and buffer sizing
 *
 * Revision 1.87  2009-04-07 19:49:57  bergsma
 * Fix problem when HTTP messages that have incomplete headers
 * are found at the end of the incoming buffer.
 *
 * Revision 1.86  2009-04-02 06:35:13  bergsma
 * Port reads and writes are 4K, Http reads and wites are 5K,
 *
 * Revision 1.85  2009-03-13 07:48:16  bergsma
 * GD refinements.
 * Added BUILD_VERSION
 *
 * Revision 1.84  2009-03-06 18:01:15  bergsma
 * Added VERSION_BUILD. this is a date format YYMMDD
 *
 * Revision 1.83  2008-11-30 22:33:23  bergsma
 * V 3.8.4
 *
 * Make -x use less frame space.
 *
 * Revision 1.82  2008-07-07 15:22:59  bergsma
 * Update to HS 3.8.3
 *
 * Revision 1.81  2008-05-25 02:30:40  bergsma
 * OCIlobRead function problems
 *
 * Revision 1.80  2008-05-19 03:29:05  bergsma
 * For ORACLE clob, allow maximum 128K input buffer.
 *
 * Revision 1.79  2008-05-19 03:25:16  bergsma
 * no message
 *
 * Revision 1.78  2008-05-19 00:29:03  bergsma
 * For ORACLE clob, allow maximum 512K input buffer.
 *
 * Revision 1.77  2008-05-18 23:54:50  bergsma
 * For ORACLE clob, allow maximum 64K input buffer.
 *
 * Revision 1.76  2008-05-12 17:36:39  bergsma
 * Increase MAX_SECSII_CODE size, as the following error was encountered
 * with the amt-p when doing a S1F3 empty list
 * 20080511:112408:amt-p2_equip:%ERROR: SecsII code size of 262144 exceeds maximum size of 131072"
 *
 * Revision 1.75  2008-05-06 02:14:15  bergsma
 * Increase initial heap size
 *
 * Revision 1.74  2008-03-05 22:55:43  bergsma
 * Functions UNARY_RAD2DEG and UNARY_DEG2RAD
 *
 * Revision 1.73  2008-02-12 23:34:48  bergsma
 * VS 2008 update
 *
 * Revision 1.72  2007-09-03 06:21:09  bergsma
 * HS3.8.1
 *
 * Revision 1.71  2007-07-11 18:34:09  mhohimer
 * added defaults for tagIndex lengths: MAX_TAG_INDEX_SIZE, TAG_INDEX_BUFLEN
 *
 * Revision 1.70  2007-05-26 22:08:43  bergsma
 * ADD -w "Ward" flag
 *
 * Revision 1.69  2007-05-26 01:47:07  bergsma
 * FRAME DEPTH of 64 is just a little to low.  Make it 128.
 *
 * Revision 1.68  2007-05-08 01:27:49  bergsma
 * Make giMaxFrameDepth variable, tied to -x option.
 *
 * Revision 1.67  2007-03-19 05:32:08  bergsma
 * New functions: min, max, pow, hypot, abs, acos, asin, atan,  ceil, cos,
 *  floor, log10, logN, sin, sqrt, tan, exp printf, sprintf, fprintf
 *
 * Revision 1.66  2007-03-15 01:11:10  bergsma
 * Increase MAX_SQL_STMT_LENGTH to 16K
 * Added CRC_BUFLEN
 * Added SQL_DATETIME_SIZE
 *
 * Revision 1.65  2007-02-01 00:29:59  bergsma
 * Increase size of HSMS buffer and max message size.
 *
 * Revision 1.64  2006-10-27 17:23:41  bergsma
 * HS 3.8.0   Support in UNIX for passing socket descriptors.
 *
 * Revision 1.63  2006/09/16 20:07:26  bergsma
 * HS 3.7.1
 *
 * Revision 1.62  2006/08/17 05:04:06  bergsma
 * Increase frame depth from 256 to 512
 *
 * Revision 1.61  2006/07/17 16:45:26  bergsma
 * Double the frame depth from 128 to 256.
 *
 * Revision 1.60  2006/06/05 19:14:40  bergsma
 * Double allowed size of SECS II messages.
 *
 * Revision 1.59  2006/05/07 18:33:28  bergsma
 * TCP getAddrByname fix.  HS 3.6.8
 *
 * Revision 1.58  2006/04/04 15:00:44  bergsma
 * HS 3.6.7.
 *
 * Revision 1.57  2006/03/07 06:16:24  bergsma
 * Increase expression size from 128 to 256
 *
 * Revision 1.56  2006/01/17 15:59:26  bergsma
 * PORT write size should be 4K, not 5K.  Same as PORT read size.
 *
 * Revision 1.55  2006/01/16 18:56:36  bergsma
 * HS 3.6.6
 * 1. Save query timeout events.  Don't let queries repeat indefinitely.
 * 2. Rework DEBUG_DIAGNOSTIC debugging.  Less overhead.
 *
 * Revision 1.54  2005/12/17 20:40:09  bergsma
 * HS 3.6.5
 *
 * Revision 1.53  2005/10/25 16:40:56  bergsma
 * V3.6.4
 *
 * Revision 1.52  2005/09/25 20:10:40  bergsma
 * Up to V3.6.3
 * Add opcert code.
 * Changes to function VT2HTML
 * Bug in EAGAIN waiting for UNIX fifo's or network sockets
 * AUTOROUTER loigical must be uppercase on VMS
 *
 * Revision 1.51  2005/09/13 03:45:11  bergsma
 * Create _hangup_status_, _connect_status_, and _pipe_status_ variables.
 * Use arrays to hold multiple HUP, CONNECT, and PIPE signals
 *
 * Revision 1.50  2005/09/08 12:44:53  bergsma
 * Sq statement max size from 5120 to 10240
 *
 * Revision 1.48  2005/06/12 16:46:21  bergsma
 * HS 3.6.1
 *
 * Revision 1.47  2005/05/10 17:43:14  bergsma
 * HS 3.6.0
 *
 * Revision 1.46  2005/04/22 19:28:03  bergsma
 * HS 3.5.7  Required to proper TLOGFEED operation.
 *
 * Revision 1.45  2005/04/13 13:45:54  bergsma
 * HS 3.5.6
 * Added sql_toexternal.
 * Fixed handling of strings ending with bs (odd/even number of backslashes)
 * Better handling of exception condition.
 *
 * Revision 1.44  2005/04/03 17:36:19  bergsma
 * HS 3.54  (FIX OF FLOATING POINT OVERFLOW IN TLOGFEED).
 * 1. Don't delete LISting files.
 * 2. PackStart in aeqssp_autofil not being cleared - was causing an
 * unpack operation when not required.
 *
 * Revision 1.43  2005/03/30 16:45:09  bergsma
 * HS 3.5.4.   In VMS, do not set the process name if the name is already set to the
 * correct HS concept name. NB:  In VMS 7.3-2 process names are case sensitive.
 *
 * Revision 1.42  2005/03/30 04:07:05  bergsma
 * Backslash should be externalized in unparsestring.
 * Added signal handler for standalone HS for VMS systems.
 *
 * Revision 1.41  2005/03/29 16:50:46  bergsma
 * V 3.5.2
 * Fix traceback in PROMIS exithandler when HS duplicate process name.
 * Functions chop() and remove() were reversed.
 *
 * Revision 1.40  2005/03/14 16:56:25  bergsma
 * Part of 3.5-0. PROMIS Result size is 256
 *
 * Revision 1.39  2005/03/09 03:46:44  bergsma
 * V3.5.0  New functions appendval, insertval, scopeof.
 *
 * Revision 1.38  2005/02/25 04:00:51  bergsma
 * HS 3.4.5
 * Make mailslot read/writes non-blocking.
 *
 * Revision 1.37  2005/02/15 07:04:51  bergsma
 * V3.4.4
 * No end of line at end of file.
 *
 * Revision 1.36  2005/01/31 05:55:04  bergsma
 * V 3.4.3
 *
 * Revision 1.35  2005/01/25 05:57:44  bergsma
 * PORT_WRITE_SIZE > PORT_READ_SIZE, so that SSL buffers
 * are complete when port-forwarding.
 *
 * Revision 1.34  2005/01/10 21:08:46  bergsma
 * V 3.4.1
 *
 * Revision 1.33  2004/12/26 17:21:24  jbergsma
 * Avoid multiple includes
 *
 * Revision 1.32  2004/11/19 03:47:22  bergsma
 * HS 3.4.0.   Added list indexing with {}.
 *
 * Revision 1.31  2004/11/02 22:58:37  bergsma
 * V 3.3.3
 *
 * Revision 1.30  2004/10/27 18:24:07  bergsma
 * HS 3.3.2
 * 1. Fix bug with SQL read when str size is > 512 bytes, use _data_ blobs
 * 2. Fix bug with XML output, forgetting ";" in unicode output.
 * 3. In |TOKEN|VALUE|VALUE| part of message, use unparse on TOKEN
 * as well as VALUE.
 * 4. Add utility function util_breakStream.
 *
 * Revision 1.29  2004/09/18 21:51:49  bergsma
 * SSL buffer size can be 16K
 * SSL timeout increased from 1 to 1.5 seconds
 * New functions ssl_setSession and ssl_getSession, for Session Id's
 *
 * Revision 1.28  2004/07/23 18:39:44  bergsma
 * Version 3.3.0
 *
 * Revision 1.27  2004/07/01 02:02:55  bergsma
 * Version 3.2.3
 *
 * Revision 1.26  2004/06/13 14:02:02  bergsma
 * Version 3.2.2
 *
 * Revision 1.25  2004/05/15 02:11:22  bergsma
 * Version 3.2.1
 *
 * Revision 1.24  2004/04/29 08:35:18  bergsma
 * Version 3.2.0
 *
 * Revision 1.23  2003/11/16 21:01:31  bergsma
 * Version 3.1.7
 *
 * Revision 1.22  2003/07/27 01:01:14  bergsma
 * Allow SECS II blocks to 65K.
 *
 * Revision 1.21  2003/06/05 21:54:23  bergsma
 * Added new typedef 'unicode', UTF-16.
 * Required for new unicode type:
 * 1. New data type in data object
 * 2. Special unicode output for xdescribe function
 * 3. New TYPE_UNICODE in hypdef.h
 * 4. New token 0uNNNNN format, like 0xNNNNN and 0oNNNNN
 * 5. New function unicode()
 * 6. New SECS II data type.
 *
 * Revision 1.20  2003/05/16 05:22:56  bergsma
 * Version 3.1.4
 *
 * Revision 1.19  2003/04/23 22:52:06  bergsma
 * Needed to separate MAX_OUTPUT_LENGTH from PROMIS restriction of
 * MAX_TERMINAL_LENGTH, which can only be 256.  This limitation caused
 * problems in code where MAX_OUTPUT_LENGTH was used.
 *
 * Revision 1.17  2003/04/11 02:01:24  bergsma
 * Define The Idle Interval To 3 Minutes, From 1 Minute.
 *
 * Revision 1.16  2003/04/07 02:09:54  bergsma
 * Increase MAX_FRAME_DEPTH to 128 from 64
 *
 * Revision 1.15  2003/04/04 16:35:55  bergsma
 * Increased size of PROMIS result to 120, same as symbuffer in gettoken.for and
 * inquire.for.
 * Increased VALUE_SIZE from 256 to 512.
 *
 * Revision 1.14  2003/03/04 01:42:56  bergsma
 * Increase allowed size of SQL statement.
 *
 * Revision 1.13  2003/02/17 09:58:36  bergsma
 * Let MAX_SECSII_CODE go up to 32K bytes.
 *
 * Revision 1.12  2003/01/12 07:08:57  bergsma
 * V3.1.0
 * New parameters FRAME_GLOBAL_MSGARGS, PARSE_SUBVARIABLE, STATUS_RESTRICTED,
 * STATUS_TYPE, COND_SPECIAL (unused)
 *
 * Revision 1.11  2002/11/20 20:39:21  bergsma
 * Version 3.1.0, proper handling of hierarchial instances.
 *
 * Revision 1.10  2002/11/14 01:40:45  bergsma
 * Removed ^M occurances and added Modification section.
 *
 * Revision 1.9  2002/11/12 20:24:26  bergsma
 * Version 3.0.7
 *
 * Revision 1.8  2002/10/27 14:31:29  bergsma
 * Revision 3.0.6
 *
 * Revision 1.7  2002/10/12 02:24:36  bergsma
 * Version 3.0.5
 *
 * Revision 1.6  2002/10/12 02:22:15  bergsma
 * Change EVENT time types to bit masks.
 *
 * Revision 1.5  2002/09/16 15:27:48  bergsma
 * Version 3.0.4
 *
 * Revision 1.4  2002/09/09 20:43:50  bergsma
 * Version 3.0-3.
 * 1. Mainly improvements to tcp.c and router.c for better IP and
 * DNS resolution.
 * 2. Timestamp on "Timer Signal Received" message.
 * 3. DecIncomningDepth when aborting a query, in frame.c
 * 4. Return FALSE rather than TRUE in gHyp_concept_write()
 *
 * Revision 1.3  2002/09/04 23:20:52  bergsma
 * Revision 3.0-2
 *
 * Revision 1.2  2002/09/03 21:22:38  bergsma
 * Change version to 3.0.1.
 * Remove release notes from version 2 cvs
 *
 */

#define		MAX_STACK_DEPTH		128  		    /* HyperScript stack */
#define		MAX_EXPRESSION		MAX_STACK_DEPTH * 2 /* Twice the expression length */
#define		MAX_FRAME_DEPTH		MAX_STACK_DEPTH	    /* Same as stack depth */

#define		MAX_JMP_LEVEL		63	/* 0-31 levels for setjmp */
#define		DEFAULT_TIMEOUT		45	/* 45 seconds */
#define		MAX_ALARM_SECONDS	60*60*24/* 1 day */
#define		MAX_LIFETIME_SECONDS	MAX_ALARM_SECONDS * 7
#define		MAX_TIMEOUT_SECONDS	MAX_ALARM_SECONDS
#define		MAX_SLEEP_SECONDS	MAX_ALARM_SECONDS
#define		MAX_CONNECT_TRIES	5
#define		MAX_RESEND		3	
#define		MAX_REWAIT		1000
#define		RETRY_INTERVAL		2
#define         DEATH_GRACE_PERIOD      30
#define		FRAME_DEPTH_NULL	MAX_FRAME_DEPTH+1
#define		MAX_SIGNALS		8
#define		MAX_PROMIS_RESULT_SIZE	256
#define		MAX_REPLY_DEPTH		32
#define		MAX_QUEUE_DEPTH		32 
#define		MAX_HASH_TABLE_SIZE	257
#define		HEARTBEAT_INTERVAL	60 * 10   /* 10 minutes */
#define		IDLE_INTERVAL		180	  /* 3 min */
#define		CONNECT_TIMEOUT		5	  /* 5 seconds */


#ifdef AS_UNIX
#define		MAX_PATH_SIZE		108
#elif defined AS_WINDOWS
#define		MAX_PATH_SIZE           MAX_PATH
#else
#define		MAX_PATH_SIZE		120
#endif

#ifndef PIPE_BUF 
#define		PIPE_BUF		5120
#endif

/* UNIX fifo max size */
#define		MAX_BUFFER_FIFO		5120

/* MAX_MESSAGE_SIZE is the maximum allowed number of bytes that can
 * be read at any time from the network socket, fifo, or mailbox channel.
 */
#define		MAX_MESSAGE_SIZE	5120

/* MAX_BUFFER_SIZE is the buffer space allocated to hold the contents
 * of the read operation.  It is twice the size as MAX_MESSAGE_SIZE because
 * the read can start an an offset in the buffer as far forward as
 * MAX_MESSAGE_SIZE-5. 
 * Explanation: If there are multple messages in a read, and the last message
 * is truncated by the limitation of the bytes allowed in the read, 
 * ie: MAX_MESSAGE_SIZE, then the last message is left justified in the 
 * buffer space and the next read will lay down its bytes from the 
 * truncated end of the last message.
 */
#define		MAX_BUFFER_SIZE		MAX_MESSAGE_SIZE*2

/* Ab Initio Message components */
#define 	INSTANCE_SIZE 		32
#define 	OBJECT_SIZE 		MAX_PATH_SIZE
#define 	CONCEPT_SIZE 		OBJECT_SIZE
#define 	HOST_SIZE 		48
#define 	TARGET_SIZE 		INSTANCE_SIZE+OBJECT_SIZE+HOST_SIZE+2
#define 	MODE_SIZE   		10
#define 	METHOD_SIZE 		32
#define 	SENDER_SIZE 		TARGET_SIZE
#define 	TOKEN_SIZE  		256
#define 	VALUE_SIZE  		512
#define		INTERNAL_VALUE_SIZE	VALUE_SIZE/4

/* The next 4 values are critical, they are always going to be 4K and 5K */
#define		PORT_READ_SIZE	  	4096	
#define		PORT_WRITE_SIZE	  	4096	
#define		HTTP_READ_SIZE	  	4096	
#define		HTTP_WRITE_SIZE	  	4096	


#define 	FIELD_SIZE  		20
#define 	DEFAULT_DELIMITER 	'|'

// OVERFLOW READ SIZE is used for SSL, somewhere it is written
// that an SSL operation needs at least 16K in overflow for the
// engines I/O buffers.
#define		OVERFLOW_READ_SIZE	16*1024

// STREAM READ SIZE is used for parsing XML streams or other
// places where the entire chuck of data that must be assimulated
// comes from a buffer that is with the compiler value for SIZE_MAX,
// which doesn't have to be over 64K.  Therefore, wince STREAM_READ_SIZE
// is often specified as STREAM_READ_SIZE*4, that is 16*1K=64K

// that an SSL operation needs at least 16K in overflow for the
// engines I/O buffers.
#define		STREAM_READ_SIZE	16*1024

#define		CRC_BUFLEN (1 << 16)
#define     MAX_TAG_INDEX_SIZE 65535 /* a.k.a. curly-bracket-index size */
#define     TAG_INDEX_BUFLEN 5+2 /* length of MAX_TAG_INDEX_SIZE plus '{' and '}' */ 


/* Minimum message size, must contain space for TARGET, MODE, METHOD, SENDER,
 * 7 delimiters, and a null terminatror: 
 * ie: "|target|mode|method|sender|tid|ts||||\0"
 */
#define		MIN_MESSAGE_SIZE  	\
		MAX (TARGET_SIZE*2+MODE_SIZE+METHOD_SIZE+FIELD_SIZE*2+10, 256 )

/* For VMS mailboxes, how many MAX_MESSAGE_SIZE messages can be buffered
 * at one time in the MBX_ROUTER and MBX_client mailboxes.
 */
#define		MAX_ROUTER_MESSAGES	16
#define		MAX_INSTANCE_MESSAGES 	8

/* Other buffer sizes for i/o devices */
#define		MAX_INPUT_LENGTH	4096  /* Maximum raw input data */
#define		MAX_OUTPUT_LENGTH	512   /* Easy short lines to read */
#define		MAX_LOG_LENGTH		1024  /* Must be > MAX_OUTPUT_LENGTH */
#define		MAX_STREAM_LENGTH	5120  /* Through pipes, like for jeval */
#define		MAX_STRTOK_LENGTH      20480  /* Internal only */
#define		MAX_COMMAND_LENGTH	2048  /* Editor's like vi don't want > 2048 */
#define		MAX_TRACE_LENGTH	132   /* For program listings */

#ifdef AS_PROMIS
#define		MAX_TERMINAL_LENGTH	256
#else
#define		MAX_TERMINAL_LENGTH	MAX_LOG_LENGTH
#endif

#ifdef AS_SSL
/* SSL */
#define		SSL_ERROR_BUF_SIZE	120 
#define		SSL_BUFFER_SIZE		HTTP_READ_SIZE*2
#define		SSL_WAIT_INCREMENT	100 /* milliseconds */
#define		SSL_TIMEOUT		15000 /* milliseconds - 1.5 seconds */
#endif

/* Compiled HyperScript:
 *
 * 	Allocate memory for DEFAULT_HYPER_CODE instructions, then if more
 *	is needed, re-allocate memory up to MAX_HYPER_CODE by doubling the
 *	previous allocation, starting at 16.
 *	ie:  16->32->64->128->256->512->1024->2048->4096->8192->16K->32K->64K
 */
#define		DEFAULT_HYPER_CODE	16
#define		MAX_HYPER_CODE		DEFAULT_HYPER_CODE << 12

/* HyperScript data */
#define		VARIABLE_HASH_TABLE_SIZE	23
#define		TOKEN_HASH_TABLE_SIZE		257
#define		KEYWORD_HASH_TABLE_SIZE		57

/* Instance states */
#define		STATE_IDLE		0	/* Idling		     */
#define		STATE_QUERY		1	/* Querying		     */
#define		STATE_PARSE		2	/* Parsing		     */
#define		STATE_EXECUTE		3	/* Executing		     */
#define		STATE_DEREFERENCE      	4	/* Dereference		     */
#define		STATE_SLEEP      	5	/* Sleeping		     */

/* Event types */
#define		EVENT_HEARTBEAT		1
#define		EVENT_ALARM		2
#define		EVENT_TIMEOUT		4
#define		EVENT_DEATH		8
#define		EVENT_WAKEUP		16

/* Handler types */
#define		HANDLER_MESSAGE		0     	/* When message arrives */
#define		HANDLER_ERROR         	1	/* When error condition */
#define		HANDLER_DEATH		2      	/* When death condition */
#define		HANDLER_TIMEOUT		3      	/* When timeout condition */
#define		HANDLER_ALARM	        4	/* When alarm condition */
#define		HANDLER_INTERRUPT	5    	/* When interrupt condition */
#define		HANDLER_HANGUP		6    	/* When interrupt condition */
#define		HANDLER_PIPE		7      	/* When pipe condition */
#define		HANDLER_DEREFERENCE	8  	/* When dereferencing */
#define		HANDLER_CONNECT		9	/* When incoming connection */
#define		NUM_HANDLER_TYPES	10

/* Data object types */
#define		DATA_OBJECT_NULL	0
#define		DATA_OBJECT_METHOD	1
#define		DATA_OBJECT_LABEL	2
#define		DATA_OBJECT_INSTANCE	3
#define		DATA_OBJECT_CHANNEL	4
#define		DATA_OBJECT_SECS1	5
#define		DATA_OBJECT_HSMS	6
#define		DATA_OBJECT_PORT	7
#define		DATA_OBJECT_HTTP	8

/* Hyperscript expression ranking */
#define		RANK_OPERAND		0
#define		RANK_UNARY		1
#define		RANK_POSTUNARY		2
#define		RANK_BINARY		3
#define		RANK_OPEN		4
#define		RANK_CLOSE		5

/* Hyperscript functional token types */
#define		TOKEN_NULL		0	
/* OPERANDS: RANK TRANSITION = 0->1 	        Represented as sData type   */
#define		TOKEN_CONSTANT		1	/* Integer, float, ...      */
#define		TOKEN_LITERAL		2	/* Double quoted string     */
#define		TOKEN_UNIDENTIFIED	3	/* Uninitialized variable   */
#define		TOKEN_RAW		4	/* Backquote raw string     */
#define		TOKEN_VARIABLE    	5	/* Variable                 */
#define		TOKEN_REFERENCE		6	/* Reference to Variable    */
/* UNARY: RANK TRANSITION = 0->0 */
#define		TOKEN_KEYWORD		7	/* if, while, break,... */
#define		TOKEN_FUNCTION		8	/* function() */
#define		TOKEN_TYPECAST		9	/* (type) */
#define		TOKEN_PREINC		10	/* ++   */
#define		TOKEN_PREDEC		11	/* --   */
#define		TOKEN_POS		12	/* +	*/
#define		TOKEN_NEG		13	/* -	*/
#define		TOKEN_COMPLEMENT	14	/* ~    */
#define		TOKEN_NOT		15	/* !	*/
#define		TOKEN_REF		16	/* &	*/
#define		TOKEN_DEREF		17	/* *	*/
/* POSTUNARY: RANK TRANSISTION = 1->1 */
#define		TOKEN_POSTINC		18	/* ++   */
#define		TOKEN_POSTDEC		19	/* --   */
/* CLOSE: RANK TRANSITION = RIGHT SHIFT, 1->1 		*/
#define		TOKEN_EEXP		20	/* )	*/
#define		TOKEN_ESUB		21	/* ]	*/
#define		TOKEN_ELIST		22	/* }	*/
/* OPEN: RANK TRANSITION = LEFT SHIFT, 1->1 		*/
/* All tokens past this point can be followed by a unary operator */
#define		TOKEN_BEXP		23	/* (	*/
#define		TOKEN_BLIST		24	/* {	*/
#define		TOKEN_BSUB		25	/* [	*/
#define		TOKEN_EOS		26	/* ;	*/
/* BINARY: RANK TRANSITION = 1->0 	        	*/               
#define		TOKEN_LIST		27	/* ,	*/
#define		TOKEN_POW		28	/* **   */
#define		TOKEN_MUL		29	/* *	*/
#define		TOKEN_DIV		30	/* /	*/
#define		TOKEN_MOD		31	/* %	*/
#define		TOKEN_ADD		32	/* +	*/
#define		TOKEN_SUB		33	/* -	*/
#define		TOKEN_LT		34	/* <	*/
#define		TOKEN_LE		35	/* <=	*/
#define		TOKEN_GT		36	/* >	*/
#define		TOKEN_GE		37	/* >=	*/
#define		TOKEN_EQ		38	/* ==	*/
#define		TOKEN_NE		39	/* !=	*/
#define		TOKEN_AND		40	/* &&	*/
#define		TOKEN_OR		41	/* ||	*/
#define		TOKEN_BITAND		42	/* &	*/
#define		TOKEN_BITXOR		43	/* ^	*/
#define		TOKEN_BITOR		44	/* |	*/
#define		TOKEN_ASSIGN		45	/* =	*/
#define		TOKEN_CONDITION		46	/* ?	*/
#define		TOKEN_EVAL		47	/* :	*/
#define		TOKEN_DOT		48      /* .    */
#define		TOKEN_POINTER		49      /* ->   */
#define		TOKEN_ASSIGN_ADD	50	/* +=	*/
#define		TOKEN_ASSIGN_SUB	51	/* -=	*/
#define		TOKEN_ASSIGN_MUL	52	/* *=	*/
#define		TOKEN_ASSIGN_DIV	53	/* /=	*/
#define		TOKEN_ASSIGN_MOD	54	/* %=	*/
                                         
/* Binary and unary math operators that are not tokens 
 *
 */
#define		BINARY_MAX	      60
#define		BINARY_MIN	      61
#define		BINARY_HYPOT	      62
#define		BINARY_POW	      63

#define		UNARY_ABS	      70
#define		UNARY_ACOS	      71
#define		UNARY_ASIN	      72
#define		UNARY_ATAN	      73
#define		UNARY_CEIL	      74
#define		UNARY_COS	      75
#define		UNARY_FLOOR	      76
#define		UNARY_LOG10	      77
#define		UNARY_LOGN	      78
#define		UNARY_SIN	      79
#define		UNARY_SQRT	      80
#define		UNARY_TAN	      81
#define		UNARY_EXP	      82
#define		UNARY_RAD2DEG	      83
#define		UNARY_DEG2RAD	      84

/* Precedence assignments 
 *
 * Higher precedence tokens are appended to lower precendence tokens
 * at end of expresssion. 
 * Lower or equal precedence tokens remove and execute higher precedence 
 * tokens from end of expression.  Equal precedence tokens are thus
 * evaluated from left to right.  For operators of equal precendence that 
 * need to be evaluated in right to left order, there needs to be both an 
 * input precendence and an expression precedence, where the input precedence
 * is set higher than the expression precedence so that the operator is
 * not removed from the end of the expression.
 *
 * The following tokens (operators) require right-to-left evaluation:
 *	comma:		,
 *	assign:		=
 *	unary:		++ -- + - ! * & fn
 *	open:		( [ {
 *
 * Note that the "->" operator has unusual behavior.  As an input token, the
 * "->" operator gets its behavior converted as follows:
 *     "ptr->obj"  becomes the same as "*ptr.obj"
 * Example:
 *    a.b.c = 99 ;
 *    ptr = &a.b ;
 *    if ( ptr->c == *ptr.c ) put "Identical expressions" ;
 *
 */
#define 	PRECEDENCE_RIGHT	0	/* 	] ) } 		*/
#define 	PRECEDENCE_LEFT		1	/* 	[ ( { 		*/
#define 	PRECEDENCE_EOS		2	/* 	; 		*/

#define 	PRECEDENCE_EMPTY	3	/*   empty expression	*/

#define 	PRECEDENCE_LIST		4	/* 	,		*/
#define 	PRECEDENCE_INPUT_LIST   5	/* 	,		*/
#define 	PRECEDENCE_ASSIGN	6	/* 	=		*/
#define 	PRECEDENCE_INPUT_ASSIGN	7	/* 	=		*/
#define 	PRECEDENCE_CONDITION	8	/* 	?		*/
#define 	PRECEDENCE_EVAL		9	/* 	:		*/
/*#define 	PRECEDENCE_ANDOR_OWL	10	* 	and or		*/
#define 	PRECEDENCE_ANDOR	10	/* 	&& ||		*/
#define 	PRECEDENCE_BITOR	11	/* 	| 		*/
#define 	PRECEDENCE_BITXOR	12	/* 	^ 		*/
#define 	PRECEDENCE_BITAND	13	/* 	& 		*/
#define 	PRECEDENCE_EQNE		14	/* 	== != 		*/
#define 	PRECEDENCE_GTLT		15	/* 	> >= <=	< 	*/
#define 	PRECEDENCE_ADDSUB	16	/* 	+ - 		*/
#define 	PRECEDENCE_MULDIVMOD	17	/* 	* / %		*/
#define 	PRECEDENCE_POW		18	/* 	**		*/
#define 	PRECEDENCE_UNARY	19	/* 	++ -- + - ! * & fn ~ (not some only) */
#define 	PRECEDENCE_INPUT_UNARY	20	/*	++ -- + - ! * &	fn ~ (not some only) */
#define		PRECEDENCE_POST_UNARY	21	/*	++, --		*/
#define         PRECEDENCE_INPUT_PTR    22      /*      ->              */
#define         PRECEDENCE_POINTER      23      /*      -> (same as DOT)*/
#define         PRECEDENCE_DOT          23      /*      .               */
#define		PRECEDENCE_DEREF	24	/*	*               */
#define		PRECEDENCE_INPUT_DEREF	25	/*	*               */
#define 	PRECEDENCE_INPUT_LEFT	26	/* 	( { [		*/
#define 	PRECEDENCE_OPERAND	27	/* 	var, lit, const	*/
#define 	PRECEDENCE_INPUT_OPERAND 28	/* 	var, lit, const	*/

/* Statement types */
#define 	STATEMENT_PROGRAM	0
#define 	STATEMENT_CALL		1
#define 	STATEMENT_LOOP		2
#define 	STATEMENT_DEREFERENCE	3

/* Frame global, local, and loop-control flags */
#define		FRAME_GLOBAL_TRUE	1
#define		FRAME_GLOBAL_HANDLER	2
#define		FRAME_GLOBAL_DEREFERENCE 4
#define		FRAME_GLOBAL_DOTCREATE	8
#define		FRAME_GLOBAL_MESSAGE	16
#define		FRAME_GLOBAL_MSGARGS	32

#define		FRAME_LOCAL_DATA	1
#define		FRAME_LOCAL_GLOBAL	2

#define		FRAME_LOOP_WHILE	1
#define		FRAME_LOOP_DO		2
#define		FRAME_LOOP_FOR		4
#define		FRAME_LOOP_FOR_EXP1	8
#define		FRAME_LOOP_FOR_EXP2	16
#define		FRAME_LOOP_FOR_EXP3	32

/* Parse flags */
#define         PARSE_VARIABLE_DEF      1
#define		PARSE_POINTER_DEREF	2
#define         PARSE_FUNCTION_CALL	4
#define		PARSE_INDEX_CALL	8
#define         PARSE_METHOD_CALL       16
#define		PARSE_METHOD_DEF     	32
#define		PARSE_METHOD_DEFERRED	64
#define		PARSE_METHOD_DEPRECATED	128
#define		PARSE_SUBVARIABLE	256
#define		PARSE_LIST_CALL		512
#define         PARSE_OBJECT_CALL	1024

#define         EXPR_FUNCTION_CALL      1


/* State transition parameters for Hyperscript grammer */
#define		G_PROGRAM_STMT		0

#define		G_STMT_LIST		1	
#define		G_LIST_END		2	

#define		G_STMT_EXP		3
#define		G_FUNC_BEXP		4

#define		G_STMT_IF		5
#define		G_IF_BEXP		6
#define		G_IF_EXP		7
#define		G_IF_ELSE		8

#define		G_STMT_WHILE		9
#define		G_WHILE_BEXP		10
#define		G_WHILE_EXP		11

#define		G_STMT_DO		12

#define		G_STMT_FOR		13
#define		G_FOR_BEXP		14
#define		G_FOR_EXP1		15
#define		G_FOR_EXP2		16
#define		G_FOR_EXP3		17

#define		G_STMT_BREAK		18
#define		G_STMT_CONTINUE		19
#define		G_STMT_RETURN		20
                                         
#define		G_STMT_GOTO		21
#define		G_GOTO_LABEL		22
#define		G_STMT_LABEL		23
#define		G_STMT_METHODDEF	24
#define		G_STMT_METHODCALL	25
#define		G_STMT_ON_CONDITION	26
#define		G_STMT_DEREFERENCE	27
#define		G_STMT_EOS		28

/* Lexical masks for finding identifiers and constants */
#define		LEX_NONE		0
#define		LEX_DIGIT		1
#define		LEX_ALPHA		2
#define		LEX_UBAR		4
#define		LEX_DOLLAR		8
#define		LEX_COLON		16
#define		LEX_DOT			32
#define		LEX_EXP			64
#define		LEX_EXP_PLUSMINUS	128
#define		LEX_EXP_DIGIT		256
#define		LEX_HEX			512
#define		LEX_OCTAL      		1024
#define		LEX_UNICODE    		2048
#define		LEX_DASH		4096

/* Definitions for data types.*/
#define		TYPE_NULL		0
#define		TYPE_LIST		1
#define		TYPE_STRING		2

#define		TYPE_BYTE       	3
#define		TYPE_UBYTE		4
#define		TYPE_BOOLEAN            5
#define		TYPE_BINARY             6

#define		TYPE_CHAR		7
#define		TYPE_UCHAR		8
#define		TYPE_ATTR		9

#define		TYPE_SHORT      	10
#define		TYPE_USHORT		11

#define		TYPE_LONG	        12		
#define		TYPE_ULONG	        13		
#define		TYPE_INTEGER            14

#define		TYPE_FLOAT		15
#define		TYPE_DOUBLE		16

#define		TYPE_HEX                17
#define		TYPE_OCTAL              18
#define		TYPE_UNICODE            19

#define		TYPE_HANDLE		20

#define		TYPE_DATETIME		21
#define		TYPE_RAW		22

/* HyperScript status values */
#define		STATUS_ACKNOWLEDGE	"$ACKNOWLEDGE"
#define		STATUS_TIMEDOUT		"%TIMEOUT"
#define		STATUS_ALARM		"%ALARM"
#define		STATUS_DEATH		"%DEATH"
#define		STATUS_MESSAGE		"%MESSAGE"
#define		STATUS_INTERRUPT	"%INTERRUPT"
#define		STATUS_HANGUP		"%HANGUP"
#define		STATUS_PARSE		"%PARSE"
#define		STATUS_EXPRESSION	"%EXPRESSION"
#define		STATUS_BRANCH		"%BRANCH"
#define		STATUS_IDENTIFIER	"%IDENTIFIER"
#define		STATUS_ARGUMENT		"%ARGUMENT"
#define		STATUS_METHOD		"%METHOD"
#define		STATUS_BOUNDS		"%BOUNDS"
#define		STATUS_FILE		"%FILE"
#define		STATUS_IO		"%IO"
#define		STATUS_EOF		"%EOF"
#define		STATUS_TARGET		"%TARGET"
#define		STATUS_UNDEFINED	"%UNDEFINED"
#define		STATUS_INVALID		"%INVALID"
#define		STATUS_UNSUPPORTED	"%UNSUPPORTED"
#define		STATUS_REJECTED		"%REJECTED"
#define		STATUS_SQL		"%SQL"
#define         STATUS_PIPE             "%PIPE"
#define         STATUS_HSMS             "%HSMS"
#define		STATUS_SECS		"%SECS"
#define		STATUS_SECS1		"%SECS1"
#define		STATUS_PORT		"%PORT"
#define		STATUS_PROMIS		"%PROMIS"
#define		STATUS_CGI		"%CGI"
#define		STATUS_XML		"%XML"
#define		STATUS_RESTRICTED	"%RESTRICTED"
#define		STATUS_TYPE		"%TYPE"
#define		STATUS_SSL		"%SSL"
#define		STATUS_CHANNEL		"%CHANNEL"
#define		STATUS_SOCKET		"%SOCKET"
#define		STATUS_HANDLE		"%HANDLE"
#define		STATUS_HTTP		"%HTTP"
#define		STATUS_NULL		"%NULL"
#define		STATUS_CONNECT		"%CONNECT"


/* Debug states */
#define		DEBUG_TRACE		1
#define		DEBUG_DIAGNOSTICS      	2
#define		DEBUG_PARSE		4
#define		DEBUG_INFIX_EXPR     	8
#define		DEBUG_POSTFIX_EXEC	16
#define		DEBUG_FRAME		32
#define		DEBUG_STACK		64
#define		DEBUG_PROTOCOL		128
#define		DEBUG_STATE		256
#define		DEBUG_HEAP		512
#define		DEBUG_SQL		1024	
#define		MAX_DEBUG_LEVEL		1024+512+256+128+64+32+16+8+4+2+1

/* RUN flags */
#define		RUN_COMPILEONLY		1
#define		RUN_QUIET   		2
#define		RUN_VERIFY   		4
#define		RUN_INTERACTIVE		8
#define		RUN_ROOT		16
#define		RUN_PROMIS		32
#define		RUN_RESTRICTED		64

/* HTTP definitions */
#define		HTTP_EXPECT_HEADER    0
#define		HTTP_EXPECT_CONTENT   1

/* SECS definitions */
#define		SECS_EXPECT_RECV_ENQ	0
#define		SECS_EXPECT_SEND_EOT	1
#define		SECS_EXPECT_RECV_BLK	2
#define		SECS_EXPECT_SEND_ACK	3
#define		SECS_EXPECT_SEND_NAK	4
#define		SECS_EXPECT_RECV_GBG	5
#define		SECS_EXPECT_SECSII	6

#define		SECS_EXPECT_SEND_ENQ	7
#define		SECS_EXPECT_RECV_EOT	8
#define		SECS_EXPECT_SEND_BLK	9
#define		SECS_EXPECT_RECV_ACK	10

#define		SECS_EXPECT_SEND_EOT2	11
#define		SECS_EXPECT_SEND_S9F1	12
#define		SECS_EXPECT_SEND_S9F3	13
#define		SECS_EXPECT_SEND_S9F5	14
#define		SECS_EXPECT_SEND_S9F7	15
#define		SECS_EXPECT_SEND_S9F9	16
#define		SECS_EXPECT_SEND_S9F11	17
#define		SECS_EXPECT_SEND_S9F13	18

#define		HSMS_EXPECT_RECV_CONNECT	0
#define		HSMS_EXPECT_SEND_ACCEPT		1
#define		HSMS_EXPECT_RECV_SELECTREQ	2
#define		HSMS_EXPECT_SEND_SELECTRSP	3
#define		HSMS_EXPECT_RECV_DATA		4

#define		HSMS_EXPECT_SEND_CONNECT	5
#define		HSMS_EXPECT_RECV_ACCEPT		6
#define		HSMS_EXPECT_SEND_SELECTREQ	7
#define		HSMS_EXPECT_RECV_SELECTRSP	8
#define		HSMS_EXPECT_SEND_DATA		9

#define		HSMS_EXPECT_SEND_DESELECTRSP	10
#define		HSMS_EXPECT_RECV_DESELECTRSP	11
#define		HSMS_EXPECT_SEND_LINKTESTRSP	12
#define		HSMS_EXPECT_RECV_LINKTESTRSP	13
#define		HSMS_EXPECT_SEND_REJECTRSP	14
#define		HSMS_EXPECT_RECV_REJECTRSP	15
#define		HSMS_EXPECT_SEND_SEPARATERSP	16
#define		HSMS_EXPECT_RECV_SEPARATERSP	17

#define		HSMS_ACTIVE		0
#define		HSMS_PASSIVE		1

#define		SECS_CHAR_EOT		4
#define		SECS_CHAR_ENQ		5
#define		SECS_CHAR_ACK		6
#define		SECS_CHAR_NAK		21

#define		SECS_DEFAULT_T1		0.5 /* .1 to 1.0 */
#define		SECS_DEFAULT_T2		5  /* 3 - 10 */
#define		SECS_DEFAULT_T3		30
#define		SECS_DEFAULT_T4		10
#define		SECS_DEFAULT_RTY	3
#define		SECS_MAX_FRAME_ERRORS	256

#define		HSMS_DEFAULT_T3		30
#define		HSMS_DEFAULT_T5		10
#define		HSMS_DEFAULT_T6	        10
#define		HSMS_DEFAULT_T7		10
#define		HSMS_DEFAULT_T8		10

#define		HSMS_STYPE_DATAMSG	0
#define		HSMS_STYPE_SELECTREQ	1
#define		HSMS_STYPE_SELECTRSP	2
#define		HSMS_STYPE_DESELECTREQ	3
#define		HSMS_STYPE_DESELECTRSP	4
#define		HSMS_STYPE_LINKTESTREQ	5
#define		HSMS_STYPE_LINKTESTRSP	6
#define		HSMS_STYPE_REJECTREQ	7
#define		HSMS_STYPE_REJECTRSP	8
#define		HSMS_STYPE_SEPARATEREQ	9
#define		HSMS_STYPE_SEPARATERSP	10

#define		MAX_EVENTS FD_SETSIZE
#define		MAX_DEVICES MAX_EVENTS*4

#define		DEVICE_HASH_TABLE_SIZE	23

#define		DEVICE_SECS		1 
#define		DEVICE_PORT		2
#define		DEVICE_HTTP		3

#define		MAX_SECS_DEVICE_ID	65535
#define		MAX_SECS1_BLKSIZE	254
#define		MAX_SECS2_BLKSIZE	244
#define		MAX_SECS1_BUFFER	257
#define		DEFAULT_SECSII_CODE	256
#define		MAX_SECSII_CODE		DEFAULT_SECSII_CODE << 10
#define		SECS_INCOMING		0
#define		SECS_OUTGOING		1
#define		SECS_MINIMUM_HSMS_PORT	4000
 
#define		MAX_HSMS_MESSAGE_SIZE	65536 /*40000*/ /*32768*/
#define		MAX_HSMS_BUFFER_SIZE	MAX_BUFFER_SIZE
#define		NULL_DEVICEID		0xffff

#define		SECS_HEADER_LENGTH	0
#define		SECS_HEADER_UPPERID	1
#define		SECS_HEADER_LOWERID	2
#define		SECS_HEADER_STREAM	3
#define		SECS_HEADER_FUNCTION	4
#define		SECS_HEADER_UPPERBLK	5
#define		SECS_HEADER_LOWERBLK	6
#define		SECS_HEADER_UPPERSRC	7
#define		SECS_HEADER_LOWERSRC	8
#define		SECS_HEADER_UPPERTID	9
#define		SECS_HEADER_LOWERTID	10

#define		HSMS_HEADER_LENGTH	0
#define		HSMS_HEADER_UPPERID	4
#define		HSMS_HEADER_LOWERID	5
#define		HSMS_HEADER_STREAM	6
#define		HSMS_HEADER_FUNCTION	7
#define		HSMS_HEADER_PTYPE	8
#define		HSMS_HEADER_STYPE	9
#define		HSMS_HEADER_UPPERSRC	10
#define		HSMS_HEADER_LOWERSRC	11
#define		HSMS_HEADER_UPPERTID	12
#define		HSMS_HEADER_LOWERTID	13

#define		SECS_ITEM_LIST		0x00
#define		SECS_ITEM_BINARY	0x20
#define		SECS_ITEM_BOOLEAN	0x24
#define		SECS_ITEM_ASCII		0x40
#define		SECS_ITEM_JIS8		0x44
#define		SECS_ITEM_SQUAD		0x60
#define		SECS_ITEM_SBYTE		0x64
#define		SECS_ITEM_SWORD       	0x68
#define		SECS_ITEM_SLONG		0x70
#define		SECS_ITEM_DOUBLE	0x80
#define		SECS_ITEM_FLOAT		0x90
#define		SECS_ITEM_UQUAD		0xA0
#define		SECS_ITEM_UBYTE		0xA4
#define		SECS_ITEM_UWORD       	0xA8
#define		SECS_ITEM_ULONG		0xB0


#ifndef ABS
#define ABS(x) ((x)>=0?(x):-(x))
#endif
#ifndef MIN
#define	MIN(a,b) (((a)<(b))?(a):(b))
#endif
#ifndef MAX
#define	MAX(a,b) (((a)>(b))?(a):(b))
#endif

#ifndef	FALSE
#define FALSE	0
#endif	
#ifndef	TRUE
#define TRUE	1
#endif	

					
/* setjmp/longjmp values */
#define		COND_SPECIAL		2
#define		COND_NORMAL		1
#define		COND_SILENT		0
#define		COND_ERROR		-1
#define		COND_FATAL		-2

#ifndef AS_WINDOWS
#define INVALID_SOCKET		(SOCKET)-1
#define SOCKET_ERROR		-1
#define INVALID_HANDLE_VALUE	(HANDLE)-1
#endif

/* Mailbox protections - VMS only */
#define		P_WRITE_ONLY 		1
#define		P_READ_ONLY 		2
#define		P_NO_ACCESS 		4
#define		P_READ_WRITE 		0

#define		P_RO 	P_READ_WRITE + \
			(P_READ_WRITE * 16) + \
			(P_READ_ONLY * 16*16) + \
			(P_READ_ONLY * 16*16*16) 

#define		P_WO 	P_READ_WRITE + \
			(P_READ_WRITE * 16) + \
			(P_WRITE_ONLY * 16*16) + \
			(P_WRITE_ONLY * 16*16*16)

#define		P_RW 	P_READ_WRITE + \
     			(P_READ_WRITE * 16) + \
     			(P_READ_WRITE * 16*16) + \
			(P_READ_WRITE * 16*16*16) 	

#define 	MAX_TOKENS		20	/* FORTRAN only */	
#define		MAX_VALUES		100	/* FORTRAN only */
#define		MAX_FIELDS		10
#define		FIELD_TID		1
#define		FIELD_TIMESTAMP		2
#define		DATETIME_SIZE		15
#define		DATE_SIZE		8
#define		TIME_SIZE		6
/*
 * CLIENT device naming conventions
 *
 *			FIFO (Unix)		MBX (VMS)
 *			------------------	--------------------
 * AUTO_FIFO		$AUTOFIFO (env)		N/A
 *
 * AUTO_SPOOL		$AUTOSPOOL (env)	AUTOSPOOL (logical)
 *
 * AUTO_LOG		$AUTOLOG (env)		AUTOLOG (logical)
 *
 * AUTO_UNIX		$AUTOUNIX		N/A
 *
 * AUTO_ROUTER		$AUTOROUTER		AUTOROUTER
 */

/* Socket hash table size */
#define		CHANNEL_HASH_TABLE_SIZE	7
#define		ALIAS_HASH_TABLE_SIZE	7

/* Socket functions */
#define		HASH_FIND		0
#define		HASH_CREATE		1
#define		HASH_DELETE		2
#define		HASH_UPDATE		3

/* Parameter for gHyp_channel_hash */
#define		CHANNEL_FIND		FALSE
#define		CHANNEL_CREATE		TRUE

/**************************** Flags *********************/
/* [A] Socket types */
#define         SOCKET_NONE             0
#define         SOCKET_FIFO		1
#define         SOCKET_TCP		2
#define		SOCKET_SERIAL		4
#define		SOCKET_LISTEN		8
#define		SOCKET_DMBX		16
#define		SOCKET_UNIX		32
#define		SOCKET_UNIX_LISTEN	64

/* [B] Socket protocols */
#define		PROTOCOL_SECS1		1*256
#define		PROTOCOL_HSMS		2*256
#define		PROTOCOL_NONE		4*256
#define		PROTOCOL_AI		8*256
#define		PROTOCOL_XML		16*256
#define		PROTOCOL_HTTP		32*256

#define		MAX_PORT_NUMBER		65535
/* [C] Masks */
#define		MASK_SOCKET   (	SOCKET_FIFO+\
				SOCKET_TCP+\
				SOCKET_SERIAL+\
				SOCKET_LISTEN+\
				SOCKET_DMBX+\
				SOCKET_UNIX+\
				SOCKET_UNIX_LISTEN )

#define		MASK_PROTOCOL (	PROTOCOL_SECS1+\
				PROTOCOL_HSMS+\
				PROTOCOL_NONE+\
				PROTOCOL_AI+\
				PROTOCOL_XML+\
				PROTOCOL_HTTP)

/***********************************************************************************/

/* TYPES of messages */
#define		MESSAGE_EVENT		0
#define		MESSAGE_QUERY		1
#define		MESSAGE_REPLY		2

/* States while parsing an Ab Initio Message */
#define		EXPECT_TARGET		0
#define		EXPECT_MODE		1
#define		EXPECT_METHOD		2
#define		EXPECT_SENDER		3
#define		EXPECT_FIELD		4
#define		EXPECT_TOKEN		5
#define		EXPECT_VALUE		6

/* SQL bounds */
#define         MAX_SQL_ITEMS           256
#define		MAX_SQL_STMT_LENGTH	MAX_INPUT_LENGTH*4
#define		MAX_SQL_USERNAME_LENGTH	64
#define		MAX_SQL_PASSWORD_LENGTH	128
#define		MAX_SQL_DATABASE_LENGTH	128
#define		MAX_SQL_SERVER_LENGTH	128
#define		MAX_SQL_NAME_LENGTH	MAX_SQL_DATABASE_LENGTH	
#define		MAX_SQL_USER_LENGTH	MAX_SQL_USERNAME_LENGTH
#define		SQL_DATETIME_SIZE	1+4+1+2+1+2+1+2+1+2+1+2+1
#define		MAX_SQL_BUFFER_SIZE	16 * 1024

/* Macros */

#define		isODD(i)	abs ( i - ((i/2)*i) ) 
#define		isEVEN(i)	!(isODD(i))

#define		makeDSCz(d,s) struct dsc_t d = { strlen(s), 14, 1, s }
#define		makeDSCs(d,s) struct dsc_t d = { sizeof(s)-1, 14, 1, s }
#define		makeDSCn(d,s,n) struct dsc_t d = { n, 14, 1, s }

#define		STATUS_READY 1
#ifdef AS_WINDOWS
#define IsSocketReady(lpOverlapped) ((lpOverlapped)->Internal == STATUS_READY)
#else
#define IsSocketReady(lpOverlapped) ((lpOverlapped)->ioStatus == STATUS_READY)
#endif

#endif /* __HYPDEF_H_ */

