/*
 * Modifications:
 *
 * $Log: mapidata.c,v $
 * Revision 1.1  2005/01/25 05:43:42  bergsma
 * Add MAPI (NSAS Nokia support methods for Outlook forwarding)
 *
 *
 */

#define EPM_REQUEST_LEN 156    // Taken from Ethereal traces

unsigned char pOutlookDIRData[EPM_REQUEST_LEN] = {
	0x05,0x00,0x00,0x03,0x10,0x00,0x00,0x00,  0x9C,0x00,0x00,0x00,0x01,0x00,0x00,0x00,
	0x84,0x00,0x00,0x00,0x00,0x00,0x03,0x00,  0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,
	0x4B,0x00,0x00,0x00,0x4B,0x00,0x00,0x00,  0x05,0x00,0x13,0x00,0x0D,0x18,0x5A,0xCC,
	0xF5,0x64,0x42,0x1A,0x10,0x8C,0x59,0x08,  0x00,0x2B,0x2F,0x84,0x26,0x38,0x00,0x02,
	0x00,0x00,0x00,0x13,0x00,0x0D,0x04,0x5D,  0x88,0x8A,0xEB,0x1C,0xC9,0x11,0x9F,0xE8,
	0x08,0x00,0x2B,0x10,0x48,0x60,0x02,0x00,  0x02,0x00,0x00,0x00,0x01,0x00,0x0B,0x02,
	0x00,0x00,0x00,0x01,0x00,0x07,0x02,0x00,  0x00,0x87,0x01,0x00,0x09,0x04,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x04,0x00,0x00,0x00 };


unsigned char pOutlook2003RFRData[EPM_REQUEST_LEN] = {
	0x05,0x00,0x00,0x03,0x10,0x00,0x00,0x00,0x9C,0x00,
	0x00,0x00,0x01,0x00,0x00,0x00,0x84,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x01,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x02,0x00,0x00,0x00,0x4B,0x00,0x00,0x00,0x4B,0x00,0x00,0x00,0x05,0x00,
	0x13,0x00,0x0D,0xE0,0xF5,0x44,0x15,0x3C,0x61,0xD1,0x11,0x93,0xDF,0x00,0xC0,0x4F,
	0xD7,0xBD,0x09,0x01,0x00,0x02,0x00,0x00,0x00,0x13,0x00,0x0D,0x04,0x5D,0x88,0x8A,
	0xEB,0x1C,0xC9,0x11,0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60,0x02,0x00,0x02,0x00,
	0x00,0x00,0x01,0x00,0x0B,0x02,0x00,0x00,0x00,0x01,0x00,0x07,0x02,0x00,0x00,0x87,
	0x01,0x00,0x09,0x04,0x00,0x0A,0x00,0x1D,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x00,
	0x00,0x00 };

unsigned char pOutlookSTOREData[EPM_REQUEST_LEN] = {
	0x05,0x00,0x00,0x03,0x10,0x00,0x00,0x00,0x9C,0x00,
	0x00,0x00,0x01,0x00,0x00,0x00,0x84,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x01,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x02,0x00,0x00,0x00,0x4B,0x00,0x00,0x00,0x4B,0x00,0x00,0x00,0x05,0x00,
	0x13,0x00,0x0D,0x00,0xDB,0xF1,0xA4,0x47,0xCA,0x67,0x10,0xB3,0x1F,0x00,0xDD,0x01,
	0x06,0x62,0xDA,0x00,0x00,0x02,0x00,0x51,0x00,0x13,0x00,0x0D,0x04,0x5D,0x88,0x8A,
	0xEB,0x1C,0xC9,0x11,0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60,0x02,0x00,0x02,0x00,
	0x00,0x00,0x01,0x00,0x0B,0x02,0x00,0x00,0x00,0x01,0x00,0x07,0x02,0x00,0x00,0x87,
	0x01,0x00,0x09,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x00,
	0x00,0x00 };


// Luckily, the DCE RPC Bind data is the same for all Windows platforms (or so it seems).

// This is all broken up because virus scanner programs like Stinger.exe and McAfee
//   think we've got a virus if it's one continuous packet.
#define DCE_RPC_BIND_DATA_LEN 0x48
#define DCE_RPC_BIND_DATA_LEN1 0x10
#define DCE_RPC_BIND_DATA_LEN2 0x10
#define DCE_RPC_BIND_DATA_LEN3 0x10
#define DCE_RPC_BIND_DATA_LEN4 0x10
#define DCE_RPC_BIND_DATA_LEN5 0x8
unsigned char pRPCBindData1[DCE_RPC_BIND_DATA_LEN1] = {
	0x05,0x00,0x0B,0x03,0x10,0x00,0x00,0x00,0x48,0x00,0x00,0x00,0x01,0x00,0x00,0x00,
	};
char filler1[0x1] = { 1 };
unsigned char pRPCBindData2[DCE_RPC_BIND_DATA_LEN2] = {
	0xD0,0x16,0xD0,0x16,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x01,0x00
	};
unsigned char pRPCBindData3[DCE_RPC_BIND_DATA_LEN3] = {
	0x08,0x83,0xAF,0xE1,0x1F,0x5D,0xC9,0x11,0x91,0xA4,0x08,0x00,0x2B,0x14,0xA0,0xFA,
	};
unsigned char pRPCBindData4[DCE_RPC_BIND_DATA_LEN4] = {
	0x03,0x00,0x00,0x00,0x04,0x5D,0x88,0x8A,0xEB,0x1C,0xC9,0x11,0x9F,0xE8,0x08,0x00,
	};
char filler5[0x1] = { 5 };
unsigned char pRPCBindData5[DCE_RPC_BIND_DATA_LEN5] = {
	0x2B,0x10,0x48,0x60,0x02,0x00,0x00,0x00
	};

