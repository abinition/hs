/*
 * Modifications:
 *
 * $Log: secshead.h,v $
 * Revision 1.3  2004-04-29 02:04:40  bergsma
 * Device id is sWORD instead of int.
 *
 * Revision 1.2  2002/11/14 01:40:45  bergsma
 * Removed ^M occurances and added Modification section.
 *
 */
struct secsHeader_t 
{
  sLOGICAL rBit ;			/* R bit = 1 */
  sLOGICAL isReplyExpected ;		/* W bit = 1 */
  sLOGICAL isFirstBlock ;		/* Blocknum = 0 or 1 */
  sLOGICAL isLastBlock ;		/* E bit = 1 */
  sLOGICAL isPrimaryMsg ;		/* Function is odd */
  sBYTE	   PType ;
  sBYTE	   SType ;
  sWORD	deviceId ;
  unsigned short	messageId ;
  unsigned short	blockNum ;
  unsigned short	SID ;
  unsigned short	TID ;
} ;
