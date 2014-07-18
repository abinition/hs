/*****************************************************************************!
!                HyperScript Software Source Code.                            !
!                                                                             !
!          ***** Copyright: (c) 1994 Ab Initio Software                       !
!          ***** All rights reserved - Licensed Material.                     !
!          ***** Program property of Ab Initio Software                       !
!                                                                             !
!****************************************************************************/
/*
 * Modifications:
 *
 *   $Log: mapi.c,v $
 *   Revision 1.1  2005/01/25 05:43:42  bergsma
 *
 */

/**********************	HYPERSCRIPT INTERFACE ********************************/

#ifdef AS_MAPI

#include "auto.h"	/* System Interface and Function Prototypes */

#define	BYTE_ORDER	LITTLE_ENDIAN

#define	RPC_TYPE_REQUEST	0
#define RPC_TYPE_RESPONSE	2

#define RPC_OPERATION_MAP	3

typedef struct _rpc_header  {
    unsigned char majorversion ;
    unsigned char minorversion ;
    unsigned char type ;
    unsigned char flags ;
    unsigned int datarepresentation ;
    unsigned short cn_frag_len ;
    unsigned short cn_auth_len ;
    unsigned int cn_call_id ;
    unsigned int cn_alloc_hint ;
    unsigned short cn_ctx_id ;
    unsigned short opnum ;
    unsigned char handle[20] ;
    unsigned int numtowers ;
    unsigned char mapdata ;
} rpc_header_struct;

#if BYTE_ORDER == LITTLE_ENDIAN
#define	letohs(x)	x
#define letohl(x)	x
#else
#error add macros for big endian machine
#endif

/*
 * This is raw packet data we'll send to the Exchange server's port mapper to obtain
 * the ports it's listening on.  Then we'll register these ports with the client
 * machine's local portmapper.
 */

static char *gszExchangeDIR_UUID =       "f5cc5a18-4264-101a-8c59-08002b2f8426";
static char *gszExchangeRFR_UUID =       "1544f5e0-613c-11d1-93df-00c04fd7bd09"; 
static char *gszExchangeSTORE_UUID =     "a4f1db00-ca47-1067-b31e-00dd010662da";   // Exchange Server STORE ADMIN Interface 
static char *gszRPC_UUID =               "8A885D04-1CEB-11C9-9FE8-08002B104860";   // Id of the whole RPC world

/*
 * unsigned char pRPCBindData[DCE_RPC_BIND_DATA_LEN] = 
 *	05 00 0B 03 10 00 00 00 48 00 00 00 01 00 00 00
 *	D0 16 D0 16 00 00 00 00 01 00 00 00 00 00 01 00
 *	08 83 AF E1 1F 5D C9 11 91 A4 08 00 2B 14 A0 FA
 *	03 00 00 00 04 5D 88 8A EB 1C C9 11 9F E8 08 00
 *	2B 10 48 60 02 00 00 00      

 * This is an example of what Bind Ack data looks like
 * unsigned char pRPCBindAckData[] = 
 * 00000030                    05 00 0C 03 10 00 00 00 3C 00       ........<.
 * 00000040  00 00 01 00 00 00 D0 16 D0 16 37 00 01 00 04 00 ..........7.....
 * 00000050  31 33 35 00 00 00 01 00 00 00 00 00 00 00 04 5D 135............]
 * 00000060  88 8A EB 1C C9 11 9F E8 08 00 2B 10 48 60 02 00 ..........+.H`..
 * 00000070  00 00                                           ..              
 */

#include "mapidata.c"

int iFishOutPort(const unsigned char *packet, int length);
int iFindThisEndpoint(char *szServerAddr, char *szUUID, int *piPortNumber);
UINT iHookIntoPortMapperThread(void *ptr);

/* These are the values to pass to the thread that'll be started.*/
typedef struct {
    int iExchangeDIRPort;
    int iExchangeRFRPort;
    int iExchangeSTOREPort;
} HookStruct;


/*
extern char *gszExchangeDIR_UUID;
extern char *gszExchangeRFR_UUID;
extern char *gszExchangeSTORE_UUID;
extern char *gszRPC_UUID;
*/

/* from: http://www.tburke.net/info/reskittools/topics/rpcdump_examples.htm
 ncacn_ip_tcp(Connection-oriented TCP/IP)
 [f5cc59b4-4264-101a-8c59-08002b2f8426] MS Exchange Directory DRS Interface :YES
 [f5cc5a7c-4264-101a-8c59-08002b2f8426] MS Exchange Directory XDS Interface :YES
 [f5cc5a18-4264-101a-8c59-08002b2f8426] MS Exchange Directory NSP Interface :YES
 [a4f1db00-ca47-1067-b31f-00dd010662da] Exchange Server STORE EMSMDB Interface :YES
 [a4f1db00-ca47-1067-b31e-00dd010662da] Exchange Server STORE ADMIN Interface :YES
 [89742ace-a9ed-11cf-9c0c-08002be7ae86] Exchange Server STORE ADMIN Interface :YES
 [99e64010-b032-11d0-97a4-00c04fd6551d] Exchange Server STORE ADMIN Interface :YES
*/

#define NCACN_IP_TCP ((unsigned char *)"ncacn_ip_tcp")
#define INTERFACE_DESC1 ((unsigned char *)"NSAS PF Exchange RFR")
#define INTERFACE_DESC2 ((unsigned char *)"NSAS PF Exchange DIR")
#define INTERFACE_DESC3 ((unsigned char *)"NSAS PF Exchange STORE")



int iStartPortMapperHook(
	int iExchangeRFRPort,
	int iExchangeDIRPort,
	int iExchangeSTOREPort)
/*
// Purpose : Hook into the Endpoint mapper via RPC.
//
// Returns : 0 -- Okay
//           # -- Win32 error (check szErrorText buffer)
*/

{
    HookStruct *psPorts;
    HANDLE hThread;
    ULONG ulThreadId;
    int rc;

    psPorts = (HookStruct *)malloc(sizeof(*psPorts));

    psPorts->iExchangeRFRPort = iExchangeRFRPort;
    psPorts->iExchangeDIRPort = iExchangeDIRPort;
    psPorts->iExchangeSTOREPort = iExchangeSTOREPort;

    /* Start a thread that will register itself to the port mapper, then merely wait.  The thing
     *	has to wait or else the port mapper will delete it from its tables of registered endpoints.
     */
    hThread = CreateThread (
		NULL,			    // SecurityAttributes,
		(16*1024),		    // Thread stack size.
		(LPTHREAD_START_ROUTINE)iHookIntoPortMapperThread,  // StartFunction,
		psPorts,		    // Parmeter to pass to the start function
		0L,			    // CreationFlags,
		&ulThreadId);		    // ThreadId

    rc = GetLastError();

    /*gHyp_util_logWarning(  "SPMH: Returning rc = %s (%d)\n", pstrGetErrorString(rc), rc);*/
    return rc;
}

int iFindThisEndpoint(char *szServerAddr, char *szUUID, int *piPortNumber)
{
  /*
//
// Purpose : Searchs the endpoints of the port mapper on the given server for the given UUID.
//	        Returns the given endpoint (if found).  If not found, piPortNumber will point to
//		0xFFFFFFFF, otherwise the endpoint (the port number) is returned in *piPortNumber.
//
// Returns : 0 -- Okay
//          -1 -- No errors, but we didn't find the requested endpoint
//           # -- Win32 error (check szErrorText buffer)
//
///////////////////////////////////////////////////////////////////////////////////////////////////
  */

    RPC_STATUS rc = -1;
    unsigned char *pszStringBinding = NULL;
    int index = 0;
    handle_t EPMap_IfHandle;

    *piPortNumber = -1;

    /*gHyp_util_logWarning(  "FTE: Searching for endpoint UUID = %s on server %s\n", szUUID, szServerAddr);*/

    /* Concatenate the elements of the string binding into the proper sequence. */
    rc = RpcStringBindingCompose(
				 NULL,
				 NCACN_IP_TCP,
                                 (unsigned char *)szServerAddr,
                                 NULL,
				 NULL,
                                 &pszStringBinding);
    /*gHyp_util_logWarning(  "FTE: rc = %d, pszStringBinding = %s\n", rc, pszStringBinding);*/

    /* Set the binding handle that will be used to bind to the server. */
    if (rc = RpcBindingFromStringBinding(pszStringBinding, &EPMap_IfHandle)) {
	/*gHyp_util_logWarning(  "FTE: RpcBindingFromStringBinding returned %d (0x%x)\n", rc, rc);*/
	goto LFuncExit;
    }

    if (rc = RpcStringFree(&pszStringBinding))
	/*gHyp_util_logWarning(  "RpcStringFree returned 0x%x\n", rc);  // This shouldn't happen!!*/

    if (1) {
	RPC_EP_INQ_HANDLE hHandle;
	RPC_BINDING_HANDLE hBinding;
	RPC_IF_ID hIfId;
	UUID sUUID;
	unsigned char *annotation;
	unsigned char *szUUIDLocal, *szUUIDLocal1;

	rc = RpcMgmtEpEltInqBegin(EPMap_IfHandle,
				    RPC_C_EP_ALL_ELTS,
				    NULL,		    // IfId
				    0,			    // VersOption
				    NULL,
				    &hHandle);

	if (rc) {
	    /*gHyp_util_logWarning(  "FTE: RpcMgmtEpEltInqBegin returned %d (0x%x)\n", rc, rc);*/
	    goto LFuncExit;
	}

	index = 0;
	while (1) {
	    unsigned char *pstr;

	    unsigned char *ObjectUuid, *ProtSeq, *NetworkAddr, *EndPoint, *NetworkOptions;
	    // This opnum = 0x2.
	    // I need to find opnum = 0x3.
	    if (rc = RpcMgmtEpEltInqNext(hHandle, &hIfId, &hBinding, &sUUID, &annotation))
	    {
		if (rc != RPC_X_NO_MORE_ENTRIES)
		    gHyp_util_logWarning(  "FTE: RpcMgmtEpEltInqNext returned %d (0x%x)\n", rc, rc);
		else
		    rc = 0;
		break;
	    }

	    RpcBindingToStringBinding(hBinding, &pstr);
	    UuidToString(&sUUID, &szUUIDLocal);
	    UuidToString(&hIfId.Uuid, &szUUIDLocal1);
	    RpcStringBindingParse(pstr, &ObjectUuid, &ProtSeq, &NetworkAddr, &EndPoint, &NetworkOptions);

	    // Look for the UUIDs (endpoints) Exchange has registers it's ports on.
	    if ((stricmp((const char *)ProtSeq, (const char *)NCACN_IP_TCP) == 0) && 
		(stricmp((const char *)szUUIDLocal1, (const char *)szUUID) == 0)) {
		
		// Found one!
		gHyp_util_logWarning(  "FTE:[%d] hIfId = %s\n", index, pstr);
		gHyp_util_logWarning(  "FTE:  hIfId.Uuid = %s, szUUID = %s\n", szUUIDLocal1, szUUIDLocal);
		if ((annotation) && (annotation[0])) {
		    gHyp_util_logWarning(  "FTE:  annotation = %s\n", annotation);
		}

		*piPortNumber = atoi((const char *)EndPoint);
		break;
	    }

	    index++;
	    RpcStringFree(&ObjectUuid);
	    RpcStringFree(&ProtSeq);
	    RpcStringFree(&NetworkAddr);
	    RpcStringFree(&EndPoint);
	    RpcBindingFree((void **)&NetworkOptions);
	    RpcStringFree(&pstr);
	    RpcStringFree(&annotation);
	    RpcStringFree(&szUUIDLocal);
	    RpcStringFree(&szUUIDLocal1);
	    RpcBindingFree(&hBinding);
	}

	rc = RpcMgmtEpEltInqDone(&hHandle);
    }

    // We're done.  Do some clean up
    if (rc = RpcBindingFree(&EPMap_IfHandle))
	gHyp_util_logWarning(  "FTE: RpcBindingFree returned 0x%x\n", rc);

    rc = 0;

LFuncExit:

    if ((rc == 0) && (*piPortNumber == -1)) {
	gHyp_util_logWarning(  "FTE: Didn't find UUID %s\n", szUUID);
	rc = -1;
    }

    gHyp_util_logWarning(  "FTE: Returning rc = %d (0x%x), *piPortNumber = %d\n", rc, rc, *piPortNumber);
    return rc;

}  // end fcn iFindThisEndpoint

/*
// Sample taken from:
//	    \Microsoft SDK\Samples\netds\rpc\filerep\filerepsecure\filerepservice.cpp
*/

RPC_STATUS __stdcall RpcServerIfCallback ( IN void *Interface, IN void *Context ) 
{
    ULONG ulAuthnLevel;
    ULONG ulAuthnSvc;

    // JTL 12/08/04:
    //	Near as I can tell this isn't being called ever, even when Outlook starts up.
    gHyp_util_logInfo(  "RpcServerIfCallback: Interface = 0x%x, Context = 0x%x\n", Interface, Context);

    // Get client security info.
    if (RpcBindingInqAuthClient(Context,
				NULL,
				NULL,
				&ulAuthnLevel,
				&ulAuthnSvc,
				NULL) != RPC_S_OK) {    
	gHyp_util_logInfo(  "RpcServerIfCallback: returning RPC_S_ACCESS_DENIED\n");
        return RPC_S_ACCESS_DENIED;
    }

    // Make sure the client has adequate security measures and uses the expected
    // security provider.
    if (ulAuthnLevel != RPC_C_AUTHN_LEVEL_PKT_PRIVACY || ulAuthnSvc != RPC_C_AUTHN_GSS_KERBEROS) {

        return RPC_S_ACCESS_DENIED;
    }

    return RPC_S_OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
UINT iHookIntoPortMapperThread(void *ptr)
//
// Purpose : Hook into the Endpoint mapper via RPC.
//
// Returns : 0 -- Okay
//           # -- Win32 error (check szErrorText buffer)
//
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    int rc = -1;
    RPC_BINDING_VECTOR * pBindingVector = NULL;
    unsigned int    fEndpoint           = 0;
    unsigned int    cMaxCalls           = 20;
    unsigned int    cMinCalls           = 1;

    // We'll fill the endpoint (port number) in dynamically.
    static RPC_PROTSEQ_ENDPOINT Endpoint1[] = {{ NCACN_IP_TCP, (unsigned char *) ""}};
    static const RPC_SERVER_INTERFACE ept___RpcServerInterface1 =
    {
	sizeof(RPC_SERVER_INTERFACE),
	{{0x1544f5e0,0x613c,0x11d1,{0x93,0xdf,0x00,0xc0,0x4f,0xd7,0xbd,0x09}},{1,0}},
	{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
	0,
	1,
	Endpoint1,
	0,
	0,
	0
    };
    RPC_IF_HANDLE b1544f5e_IfHandle1 = (RPC_IF_HANDLE)& ept___RpcServerInterface1;

    // We'll fill the endpoint (port number) in dynamically.
    static RPC_PROTSEQ_ENDPOINT Endpoint2[] = {{NCACN_IP_TCP, (unsigned char *) ""}};
    static const RPC_SERVER_INTERFACE ept___RpcServerInterface2 =
    {
	sizeof(RPC_SERVER_INTERFACE),
	{{0xf5cc5a18,0x4264,0x101a,{0x8c,0x59,0x08,0x00,0x2b,0x2f,0x84,0x26}},{56,0}},
	{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
	0,
	1,
	Endpoint2,
	0,
	0,
	0
    };
    RPC_IF_HANDLE bF5CC5A18_IfHandle2 = (RPC_IF_HANDLE)& ept___RpcServerInterface2;

    // We'll fill the endpoint (port number) in dynamically.
    static RPC_PROTSEQ_ENDPOINT Endpoint3[] = {{NCACN_IP_TCP, (unsigned char *) ""}};
    static const RPC_SERVER_INTERFACE ept___RpcServerInterface3 =
    {
	sizeof(RPC_SERVER_INTERFACE),
	{{0xa4f1db00,0xca47,0x1067,{0xb3,0x1f,0x00,0xdd,0x01,0x06,0x62,0xda}},{0,81}},
	{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
	0,
	1,
	Endpoint3,
	0,
	0,
	0
    };
    RPC_IF_HANDLE bA4F1DB00_IfHandle3 = (RPC_IF_HANDLE)& ept___RpcServerInterface3;

    char szExchangeDIRPort[32], szExchangeRFRPort[32], szExchangeSTOREPort[32];

    RPC_BINDING_VECTOR sBindingVector;
    RPC_BINDING_HANDLE hBinding;
    unsigned char *pszStringBinding;
    unsigned char *szServerAddr;

    rc = 0;
    sprintf(szExchangeRFRPort, "%d", ((HookStruct *)ptr)->iExchangeRFRPort);
    Endpoint1->Endpoint = (unsigned char *)szExchangeRFRPort;

    sprintf(szExchangeDIRPort, "%d", ((HookStruct *)ptr)->iExchangeDIRPort);
    Endpoint2->Endpoint = (unsigned char *)szExchangeDIRPort;

    sprintf(szExchangeSTOREPort, "%d", ((HookStruct *)ptr)->iExchangeSTOREPort);
    Endpoint3->Endpoint = (unsigned char *)szExchangeSTOREPort;

    gHyp_util_logWarning(  "HIPM: szExchangeDIRPort = %s, szExchangeRFRPort = %s, szExchangeSTOREPort = %s\n",
	szExchangeDIRPort, szExchangeRFRPort, szExchangeSTOREPort);

    if (rc = RpcServerUseProtseqEp(NCACN_IP_TCP, cMaxCalls, (unsigned char *)"5678", NULL)) {
	gHyp_util_logWarning(  "HIPM: RpcServerUseProtseqEpEx returned %d (0x%x)\n", rc, rc);
	goto LFuncExit;
    }


    szServerAddr = (unsigned char *)NULL;
 
    sBindingVector.Count = 1;

    // jtljtl Fix this -- the fcn should fail if one of these fails...
    RpcStringBindingCompose(NULL, NCACN_IP_TCP, szServerAddr, (unsigned char *)szExchangeRFRPort, NULL, &pszStringBinding);
    RpcBindingFromStringBinding(pszStringBinding, &hBinding);
    RpcStringFree(&pszStringBinding);
    sBindingVector.BindingH[0] = hBinding;

    // Register the service interfaces.

    // The server interface has a security callback RpcServerIfCallback.
    // The RPC run time will automatically reject unauthenticated calls to
    // this interface.

    // Sample which shows this:
    //	    \Microsoft SDK\Samples\netds\rpc\filerep\filerepsecure\filerepservice.cpp(325):

    rc = RpcServerRegisterIfEx(b1544f5e_IfHandle1,
				   NULL,
				   NULL,
				   RPC_IF_ALLOW_CALLBACKS_WITH_NO_AUTH,
				   RPC_C_LISTEN_MAX_CALLS_DEFAULT,
				   &RpcServerIfCallback);

    if (rc != RPC_S_OK){
	gHyp_util_logWarning(  "HIPM: ERROR: Status = %d (0x%x) from RpcServerRegisterIfEx of interface: %s\n", rc, rc, gszExchangeRFR_UUID);
	goto LFuncExit;
    }

    if (rc = RpcEpRegister(b1544f5e_IfHandle1, &sBindingVector, NULL, INTERFACE_DESC1)) {
	gHyp_util_logWarning(  "HIPM: ERROR: Status = %d (0x%x) from RpcEpRegister1 of interface: %s\n", rc, rc, gszExchangeRFR_UUID);
	goto LFuncExit;
    }
    gHyp_util_logWarning(  "HIPM:   %s [Port = %s]\n", gszExchangeRFR_UUID, szExchangeRFRPort);

    fEndpoint = 1;

    RpcStringBindingCompose(NULL, NCACN_IP_TCP, szServerAddr, (unsigned char *)szExchangeDIRPort, NULL, &pszStringBinding);
    RpcBindingFromStringBinding(pszStringBinding, &hBinding);
    RpcStringFree(&pszStringBinding);
    sBindingVector.BindingH[0] = hBinding;

    rc = RpcServerRegisterIfEx(bF5CC5A18_IfHandle2,
				   NULL,
				   NULL,
				   RPC_IF_ALLOW_CALLBACKS_WITH_NO_AUTH,
				   RPC_C_LISTEN_MAX_CALLS_DEFAULT,
				   &RpcServerIfCallback);
    if (rc != RPC_S_OK){
	gHyp_util_logWarning(  "HIPM: ERROR: Status = %d (0x%x) from RpcServerRegisterIfEx of interface: %s\n", rc, rc, gszExchangeDIR_UUID);
	goto LFuncExit;
    }

    if (rc = RpcEpRegister(bF5CC5A18_IfHandle2, &sBindingVector, NULL, INTERFACE_DESC2)) {
	gHyp_util_logWarning(  "HIPM: ERROR: Status = %d (0x%x) from RpcEpRegister1 of interface: %s\n", rc, rc, gszExchangeDIR_UUID);
	goto LFuncExit;
    }
    gHyp_util_logWarning(  "HIPM:   %s [Port = %s]\n", gszExchangeDIR_UUID, szExchangeDIRPort);

    RpcStringBindingCompose(NULL, NCACN_IP_TCP, szServerAddr, (unsigned char *)szExchangeSTOREPort, NULL, &pszStringBinding);
    RpcBindingFromStringBinding(pszStringBinding, &hBinding);
    RpcStringFree(&pszStringBinding);
    sBindingVector.BindingH[0] = hBinding;

    rc = RpcServerRegisterIfEx(bA4F1DB00_IfHandle3,
				   NULL,
				   NULL,
				   RPC_IF_ALLOW_CALLBACKS_WITH_NO_AUTH,
				   RPC_C_LISTEN_MAX_CALLS_DEFAULT,
				   &RpcServerIfCallback);
    if (rc != RPC_S_OK){
	gHyp_util_logWarning(  "HIPM: ERROR: Status = %d (0x%x) from RpcServerRegisterIfEx of interface: %s\n", rc, rc, gszExchangeSTORE_UUID);
	goto LFuncExit;
    }

    if (rc = RpcEpRegister(bA4F1DB00_IfHandle3, &sBindingVector, NULL, INTERFACE_DESC3)) {
	gHyp_util_logWarning(  "HIPM: Status2 = %d (0x%x) from RpcEpRegister of interface: %s\n", rc, rc, gszExchangeSTORE_UUID);
	goto LFuncExit;
    }
    gHyp_util_logWarning(  "HIPM:   %s [Port = %s]\n", gszExchangeSTORE_UUID, szExchangeSTOREPort);

    gHyp_util_logWarning(  "HIPM: Calling RpcServerListen\n");
    if (rc = RpcServerListen(cMinCalls, cMaxCalls, FALSE)) {
	gHyp_util_logWarning(  "HIPM: RpcServerListen returned rc = %d (0x%x)\n", rc, rc);
	goto LFuncExit;
    }

    rc = 0;

LFuncExit:

    if ( fEndpoint )
    {
	int rc;    // Remember the rc from above
        rc = RpcEpUnregister(bF5CC5A18_IfHandle2, pBindingVector, NULL);
	gHyp_util_logWarning(  "HIPM: RpcEpUnregister1 returned rc = %d (0x%x)\n", rc, rc);
        rc = RpcEpUnregister(b1544f5e_IfHandle1, pBindingVector, NULL);
	gHyp_util_logWarning(  "HIPM: RpcEpUnregister2 returned rc = %d (0x%x)\n", rc, rc);
        rc = RpcEpUnregister(bA4F1DB00_IfHandle3, pBindingVector, NULL);
	gHyp_util_logWarning(  "HIPM: RpcEpUnregister3 returned rc = %d (0x%x)\n", rc, rc);
    }

    if ( pBindingVector )
    {
	int rc;
        rc = RpcBindingVectorFree(&pBindingVector);
        gHyp_util_logWarning(  "HIPM: RpcBindingVectorFree returned 0x%x\n", rc);
    }

    gHyp_util_logWarning(  "HIPM: Returning rc = %d (0x%x)\n", rc, rc);
    return rc;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
int iGetEPMBindData(
	char *pBuffer,      // OUT: Data gets stuffed here
	int *pSize)         // IN/OUT:  Size of buffer on input, bytes copied on output
//
// Purpose : Returns the raw data of an Endpoint Mapper Bind request.
//
// Returns :  0 -- Okay, *pSize will have number of bytes copied
//           !0 -- Buffer too small; *pSize will have the size of buffer that would be necessary
//
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    if (*pSize < DCE_RPC_BIND_DATA_LEN) {
	*pSize = DCE_RPC_BIND_DATA_LEN;
	return 1;
    }

    *pSize = DCE_RPC_BIND_DATA_LEN;

    // This is all broken up because virus scanner programs like Stinger.exe and McAfee
    //   think we've got a virus if it's one continuous packet.
    memcpy(pBuffer, pRPCBindData1, sizeof(pRPCBindData1));
    pBuffer += sizeof(pRPCBindData1);
    memcpy(pBuffer, pRPCBindData2, sizeof(pRPCBindData2));
    pBuffer += sizeof(pRPCBindData2);
    memcpy(pBuffer, pRPCBindData3, sizeof(pRPCBindData3));
    pBuffer += sizeof(pRPCBindData3);
    memcpy(pBuffer, pRPCBindData4, sizeof(pRPCBindData4));
    pBuffer += sizeof(pRPCBindData4);
    memcpy(pBuffer, pRPCBindData5, sizeof(pRPCBindData5));
    pBuffer += sizeof(pRPCBindData5);

    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
short iCheckEPMBindAck(char *pBuffer, int iSize)
//
// Purpose : 
//
// Returns : -1 -- This is not an EPM bind Ack (reply)
//            0 -- Okay, and the reply indicates that the bind was successful (Accepted)
//            # -- The data is a bind reply, but it wasn't successful.  This will hold the status
//                 code from the server.
//
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    rpc_header_struct *hdr = (rpc_header_struct *) pBuffer;
//    unsigned char *pstr;

    if (iSize != hdr->cn_frag_len)
	return -1;

    // The RPC UUID should be at offset 0x2b
//    UuidToString((UUID *)(pBuffer+0x2b), &pstr);
//    if (stricmp((char *)pstr, gszRPC_UUID) != 0) {
//	gHyp_util_logError (  "iCheckEPMBindAck: ERROR -- Expecting RPC UUID '%s'\n", gszRPC_UUID);
//	gHyp_util_logError (  "iCheckEPMBindAck: ERROR --                got '%s'\n", pstr);
//	goto LFuncExit;
//    }

    if (hdr->type != 12) {
	gHyp_util_logError (  "iCheckEPMBindAck: ERROR -- Expecting type 12, got 0x%x\n", hdr->type);
	return -1;
    }

    // The Ack result will be 24 bytes from the end of the data.
    
    return *(pBuffer+(hdr->cn_frag_len - 24));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
int iGetEPMData(
	char *pBuffer,      // OUT: Data gets stuffed here
	int *pSize,         // IN/OUT:  Size of buffer on input, bytes copied on output
	int iRequest)       // IN:  0 = Outlook DIR interface
	                    //      1 = Outlook 2003 RFR interface
			    //      2 = Outlook STORE interface
//
// Purpose : Returns the raw data of an Endpoint Mapper request (the MAP or ept_map request)
//             for the purpose of retrieving the ports MS Exchange is listening on.
//
// Returns :  0 -- Okay, *pSize will have number of bytes copied
//           !0 -- Buffer too small; *pSize will have the size of buffer that would be necessary
//
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    unsigned char *CopyMe;

    if (*pSize < EPM_REQUEST_LEN) {
	*pSize = EPM_REQUEST_LEN;
	return 1;
    }

    // All the data is of the same number of bytes (at least currently (2-10-2004)).
    switch (iRequest) {
	case 0 : CopyMe = pOutlookDIRData;      break;
	case 1 : CopyMe = pOutlook2003RFRData;  break;
	case 2 : CopyMe = pOutlookSTOREData;    break;
	default :
	    gHyp_util_logError (  "GetEPMData: ERROR -- invalid iRequest %d (0x%x)\n", iRequest, iRequest);
	    break;
    }

    memcpy(pBuffer, CopyMe, EPM_REQUEST_LEN);
    *pSize = EPM_REQUEST_LEN;
    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
short siGetPortFromEPMData(
	char *pBuffer,      // IN: Data returned from the port mapper
	int iBytes,         // IN: Number of bytes in the pBuffer
	int iRequest)       // IN:  0 = Outlook DIR interface
	                    //      1 = Outlook 2003 RFR interface
			    //      2 = Outlook STORE interface
//
// Purpose : Fishes into the packet for the port number.  This routine does its best to ensure
//             the packet is indeed a port mapper reply.
//
// Returns :  # -- The port number
//           -1 -- Buffer too small
//           -2 -- Data is not a port mapper reply
//
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    rpc_header_struct *hdr = (rpc_header_struct *) pBuffer;
//    unsigned char *pstr, *pstr1, *pCheckMe;
    short rc = -2;
    int rc1;

    if (iBytes < hdr->cn_frag_len) {
	gHyp_util_logError (  "siGetPortFromEPMData: ERROR -- Expecting hdr len = %d, got %d\n", iBytes, hdr->cn_frag_len);
	return -1;
    }

    // Check the RPC packet type
    if (hdr->type != 2) {
	gHyp_util_logError (  "siGetPortFromEPMData: ERROR -- Unexpected hdr->type = %d, (0x%x)\n", hdr->type, hdr->type);
	return -2;
    }

    // The last four bytes is the return code.
    if (rc1 = *(int *)(pBuffer + (hdr->cn_frag_len-4)))  {
	gHyp_util_logError (  "siGetPortFromEPMData: ERROR -- RPC return code = %d (0x%x)\n", rc1, rc1);
	return -2;
    }

    // Do some checking to ensure the data is indeed from the endpoint mapper.

    // The RPC UUID should be at offset 88
//    UuidToString((UUID *)(pBuffer+88), &pstr);
//    if (stricmp((char *)pstr, gszRPC_UUID) != 0) {
//	gHyp_util_logError (  "siGetPortFromEPMData: ERROR -- Expecting RPC UUID '%s'\n", gszRPC_UUID);
//	gHyp_util_logError (  "siGetPortFromEPMData: ERROR --                got '%s'\n", pstr);
//	goto LFuncExit;
//    }

    // Now check the UUID the caller is expecting:
//    switch (iRequest) {
//	case 0 : pCheckMe = (unsigned char *)gszExchangeDIR_UUID; break;
//	case 1 : pCheckMe = (unsigned char *)gszExchangeRFR_UUID; break;
//	case 2 : pCheckMe = (unsigned char *)gszExchangeSTORE_UUID; break;
//	default :
//	    gHyp_util_logError (  "siGetPortFromEPMData: ERROR -- invalid iRequest %d (0x%x)\n", iRequest, iRequest);
//	    break;
//    }

    // The given UUID should be at offset 74.
//    UuidToString((UUID *)(pBuffer+74), &pstr1);
//    if (stricmp((char *)pstr1, (const char *)pCheckMe) != 0) {
//	gHyp_util_logError (  "siGetPortFromEPMData: ERROR -- Expecting UUID '%s'\n", pCheckMe);
//	gHyp_util_logError (  "siGetPortFromEPMData: ERROR --            got '%s'\n", pstr1);
//	goto LFuncExit;
//    }

    rc = iFishOutPort((const unsigned char *)pBuffer, iBytes);
    //*(short *)(pBuffer + EPM_RESPONSE_PORT_OFFSET);
    //rc = ntohs(rc);

//LFuncExit:
    
//    RpcStringFree(&pstr);
//    RpcStringFree(&pstr1);
    return rc;
}

int iFishOutPort(const unsigned char *packet, int length)
{
    rpc_header_struct *hdr = (rpc_header_struct *) packet ;
    unsigned char *offset ;
    short port = 0 ;
    int addr = 0 ;
    int i; 
    int j ; 
    int numtowers ;
    short lhslen ;
    short rhslen ;
    short numfloors ;
    char protoid ;

    // Start pulling the packet apart to locate the port.
    if ((int) hdr ->type != RPC_TYPE_RESPONSE)  		/* only work on responses */
	return -1;

    numtowers = letohl(hdr ->numtowers) ;
    offset = &hdr ->mapdata ;			/* get pointer to tower data */
    offset += 4 ;				/* jump past maxcount */
    offset += 4 ;				/* jump past offset */
    offset += 4 ;				/* jump past the actual count */

    gHyp_util_logError (  "iFishOutPort: numtowers = %d\n", numtowers);

    // num referent IDs = num towers...
    offset += 4 * numtowers;			// Jump passed the referent IDs.

    for (i=0; i < numtowers; i++)  {			/* for all the towers start looking for a port */

	// JTL -- I just put this in there because this routine was overstepping it's bounds
	// on some packets.  It needs some additional checks on the packet flags or something.
	if ((offset - packet) >  length) {
	    gHyp_util_logError (  "ERROR: Exceeded packet length %d > %d\n", offset - packet, length);
	    return -1;
	}

	offset += 4 ;					/* move past 1st length */
	offset += 4 ;					/* move past 2nd length */
	numfloors = letohs(*offset) ;			/* get the number of floors */

	offset += 2 ;
	for (j=0; j < numfloors; j++)  {
	    if ((offset - packet) >  length) {    // Another sanity check
		gHyp_util_logError (  "ERROR: Exceeded packet length %d > %d\n", offset - packet, length);
		return -1;
	    }

	    lhslen = letohs(*offset) ;
	    offset += 2 ;
	    protoid = *offset ;				/* protocol */
	    offset += lhslen ;				/* move past LHS info which includes the protoid */
	    rhslen = letohs(*offset) ;			/* RHS length */
	    offset += 2 ;					/* move past RHS length */

	    switch(protoid)  {
	      case 0x0D:
		break ;

	      case 0x1F: 					/* TCP this one is always big endian (network order) */
	      case 0x07: 					/* TCP this one is always big endian (network order) */
		port = ntohs((short) (*((short *)(offset)))) ;
		goto LFuncExit;

	      case 0x08: 					/* UDP this one is always big endian */
		port = ntohs((short) (*((short *)(offset)))) ;
		goto LFuncExit;
		
	      case 0x09: 					/* IP this one is always big endian */
		addr = ntohl(*((int *) offset)) ;		/* get IP address */
		gHyp_util_logInfo("IP Address: %08X\n", addr) ;
		break;
		
	      case 0x0f:					/* \\PIPE\xxx   named pipe */
		break;
		
	      case 0x10:					/* PIPENAME  named pipe */
		break;
		
	      case 0x11:					/* \\NETBIOS   netbios name */
		break;

	      default:
		break ;
	    }

	    offset += rhslen ;
	}
	offset += 1 ;
    }

LFuncExit:

    gHyp_util_logWarning(  "iFishOutPort: returning port = %d (0x%x)\n", port, port);
    return((int) port) ;
}


#if 0
///////////////////////////////////////////////////////////////////////////////////////////////////
int iIsRPCData(char *buffer, int bytes)
//
// Purpose : See return values.
//
// Returns : 0 -- The buffer is an RPC message data.
//           1 -- It's not
//
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    rpc_header_struct *hdr = (rpc_header_struct *) buffer;

    // jtljtl This definitely needs work...

    // I don't really know how to tell this...
    if ((hdr->majorversion < 20) &&
	(hdr->minorversion < 20) &&
	((int) letohs(hdr->cn_auth_len) < 1024) &&
	((int) letohs(hdr->opnum) < 100) &&
	((int) letohs(hdr->type) < 50))
	return 1;

    // Make sure the length isn't longer than the data we have...
    if (hdr->cn_frag_len >= bytes)
	return 0;

    return 0;

}

///////////////////////////////////////////////////////////////////////////////////////////////////
int iMoreThanOneRPCMsg(char *buffer, int bytes)
//
// Purpose : See return values.
//
// Returns : 0 -- There is not more than 1 RPC msg in the buffer.
//           1 -- There is.
//
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    rpc_header_struct *hdr1 = (rpc_header_struct *) buffer;

    if (!iIsRPCData(buffer, bytes))
	return 0;

    // If the
    if (hdr1->cn_frag_len >= bytes)
	return 0;



    // Otherwise
    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
int iSplitRPCMessage(char *buffer, int bytes, char *buf1, int *pbuf1_bytes, char *buf2, int *pbuf2_bytes)
//
// Purpose : Split the buffer into its two RPC messages.
//
// Returns : 1 -- This is not RPC data
//           0 -- Okay
//          -1 -- Error
//
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    rpc_header_struct *hdr = (rpc_header_struct *) buffer;

    if (!iIsRPCData(buffer, bytes))
	return 0;


    return -1;
}

#endif


void gHyp_mapi_getPort ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE )
{

/*
  Purpose : Does the RPC dance to determine what port exchange is running on.
 
               Outlook 2003 uses UUID 1544f5e0-613c-11d1-93df-00c04fd7bd09  (RFR)
		    This is the "MS Exchange Directory RFR Interface"
               Outlook 2000 uses UUID f5cc5a18-4264-101a-8c59-08002b2f8426
		    This is the "MS NT Directory NSP Interface"
               Exchange STORE interface uses UUID a4f1db00-ca47-1067-b31f-00dd010662da
                  It's annotation is: "Exchange 2003 Server STORE EMSMDB Interface"

               The endpoint mapper itself is at: e1af8308-5d1f-11c9-91a4-08002b14a0fa

           Exchange (2003) offers them both as endpoints.  Such is the need for the szUUID parm.

 Purpose : Does the RPC dance to determine what port exchange is running on.

 Returns : 0 -- Okay
           # -- Win32 error (check szErrorText buffer)
*/

  sFrame        *pFrame = gHyp_instance_frame ( pAI ) ;
  sParse        *pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;

    sData
      *pResult,
      *pData,
      *pValue;

    char
      *pBuf;
    
    int
      service,
      iSize,
      iPortNumber,
      argCount = gHyp_parse_argCount ( pParse ) ;

    /* Assume success */
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 2 )
      gHyp_instance_error ( pAI, STATUS_ARGUMENT,
        "Invalid args. Usage: mapi_getPort ( buffer, service )");

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    service = gHyp_data_getInt ( pData, gHyp_data_getSubScript(pData), TRUE) ;

    if ( service < 0 || service > 2 ) {
      gHyp_instance_warning ( pAI,  STATUS_BOUNDS, "Service must be 0, 1, or 2" ) ;
      gHyp_instance_pushSTATUS ( pAI, pStack) ;
      return ;
    }

    pData = gHyp_stack_popRdata ( pStack, pAI ) ;
    pValue = gHyp_data_getVariable ( pData ) ;
    if ( !pValue ) pValue = pData ;

    pBuf = gHyp_data_buffer ( pValue, 0 ) ;
    iSize = gHyp_data_bufferLen ( pValue, 0 ) ;

    iPortNumber = siGetPortFromEPMData( pBuf, iSize, service) ;

    if ( iPortNumber <= 1024 ) 
      gHyp_instance_warning ( pAI, STATUS_ARGUMENT,
	"Got bad port = %d (0x%x) siGetPortFromEPMData getting iRequest = %d\n",
	iPortNumber, iPortNumber, service);

    pResult = gHyp_data_new ( "_EPM_bind_data_" ) ;
    gHyp_data_setInt ( pResult, iPortNumber ) ;
    gHyp_stack_push ( pStack, pResult ) ;
  }

}

void gHyp_mapi_register ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE )
{

  sFrame        *pFrame = gHyp_instance_frame ( pAI ) ;
  sParse        *pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;

    int
      iExchangeDIRPort = 29222,
      iExchangeRFRPort = 29223,
      iExchangeSTOREPort = 29224,
      argCount = gHyp_parse_argCount ( pParse ) ;

    /* Assume success */
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 0 )
      gHyp_instance_error ( pAI, STATUS_ARGUMENT,
        "Invalid args. Usage: mapi_register ( )");

    iStartPortMapperHook(iExchangeRFRPort, iExchangeDIRPort, iExchangeSTOREPort);

    gHyp_instance_pushSTATUS ( pAI, pStack) ;
  }
}

#endif
