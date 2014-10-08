// hserv.cpp

#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

// Global variables

// The name of the service
char *SERVICE_NAME = "booths";

// Environment variables
char *pAUTOROUTER  ;
char *pAUTOHOST  ;
char *pAUTOBIN  ;
char *pAUTORUN  ;
char *pAUTOLOG  ;
char *pAUTOPORT ;

// Event used to hold ServiceMain from completing
HANDLE terminateEvent = NULL;

// Handle used to communicate status info with
// the SCM. Created by RegisterServiceCtrlHandler
SERVICE_STATUS_HANDLE serviceStatusHandle;

// Flags holding current state of service
BOOL resumeService = FALSE;
BOOL pauseService = FALSE;
BOOL runningService = FALSE;

DWORD currentState = SERVICE_STOPPED ;
DWORD previousState = currentState ;

// Process handle for the actual work
HANDLE processHandle = 0;

STARTUPINFO si;
PROCESS_INFORMATION pi;
  
char logFile[256] ;
FILE *pLog = NULL ;

void ErrorHandler(char *s, DWORD err)
{
  std::cout << s << std::endl;
  std::cout << "Error number: " << err << std::endl;
	ExitProcess(err);
}

DWORD dwGetRegString(HKEY sysKey, char *szKey, char *szString, unsigned char *szBuf, DWORD *pdwBufSize)
{
    LONG lStatus ;
    DWORD Type = REG_SZ ;
    HKEY hKey;

    szBuf[0] = 0;

    if ((lStatus = RegOpenKey(sysKey, szKey, &hKey)) != ERROR_SUCCESS)  {
	std::cout  
	      << "dwGetRegString: ERROR: " 
	      << lStatus 
	      << "(0x"
	      << lStatus
	      << ") from RegOpenKey of "
	      << szKey
	      << "\\" 
	      << szString 
	      << "\n" ;

	return lStatus;
    }

    if ((lStatus = RegQueryValueEx(hKey, szString, NULL, &Type, szBuf, pdwBufSize)) != ERROR_SUCCESS)  {
	std::cout  
	      << "dwGetRegString: ERROR: " 
	      << lStatus 
	      << "(0x"
	      << lStatus
	      << ") from RegQueryValueEx getting key "
	      << szKey
	      << "\\" 
	      << szString 
	      << "\n" ;

	return lStatus;
    }

    RegCloseKey(hKey) ;
    std::cout  
	  << "dwGetRegString: Key = '"
	  << szKey
	  << "\\"
	  << szString
	  << "', value = '"
	  << szBuf
	  << "'\n" ;

    return 0;
}

// Initializes the service by starting its thread
#define MAX_PATH_SIZE 256
#define MAX_CMD_SIZE 256

BOOL InitService()
{
  char *pAUTOROUTER_DEFAULT = "router" ;
  char *pAUTOHOST_DEFAULT = "localhost" ;
  char *pAUTOBIN_DEFAULT = "\\local\\bin" ;
  char *pAUTORUN_DEFAULT = "\\local\\run" ;
  char *pAUTOLOG_DEFAULT = "\\local\\log" ;
  char *pAUTOPORT_DEFAULT = "7001" ;

  char AUTOROUTER[MAX_PATH_SIZE+1] ;
  char AUTOHOST[MAX_PATH_SIZE+1] ;
  char AUTOBIN[MAX_PATH_SIZE+1] ;
  char AUTORUN[MAX_PATH_SIZE+1] ;
  char AUTOLOG[MAX_PATH_SIZE+1] ;
  char AUTOPORT[15] ;

  pAUTOROUTER = AUTOROUTER ;
  pAUTOHOST = AUTOHOST ;
  pAUTOBIN = AUTOBIN ;
  pAUTORUN = AUTORUN ;
  pAUTOLOG = AUTOLOG ;
  pAUTOPORT = AUTOPORT ;

  char path[MAX_PATH_SIZE+1] ;
  char cmd[MAX_CMD_SIZE+1] ;

  DWORD dwBytes = MAX_PATH_SIZE ;

  ZeroMemory( &si, sizeof(si) );
  si.cb = sizeof(si);

  path[0] = '\0' ;
  dwBytes = MAX_PATH_SIZE ;
  dwGetRegString(	HKEY_LOCAL_MACHINE, 
			"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment", "PATH", (unsigned char*)path, &dwBytes) ;

  dwBytes = MAX_PATH_SIZE ;
  if ( dwGetRegString(	HKEY_LOCAL_MACHINE, 
			"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment", "AUTOLOG", (unsigned char*)pAUTOLOG, &dwBytes) != 0) 
    pAUTOLOG = pAUTOLOG_DEFAULT ;

  dwBytes = MAX_PATH_SIZE ;
  if ( dwGetRegString(	HKEY_LOCAL_MACHINE, 
			"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment", "AUTORUN", (unsigned char*)pAUTORUN, &dwBytes) != 0) 
    pAUTORUN = pAUTORUN_DEFAULT ;

  dwBytes = MAX_PATH_SIZE ;
  if ( dwGetRegString(	HKEY_LOCAL_MACHINE, 
			"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment", "AUTOBIN", (unsigned char*)pAUTOBIN, &dwBytes) != 0) 
    pAUTOBIN = pAUTOBIN_DEFAULT ;

  dwBytes = MAX_PATH_SIZE ;
  if ( dwGetRegString(	HKEY_LOCAL_MACHINE, 
			"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment", "AUTOPORT", (unsigned char*)pAUTOPORT, &dwBytes) != 0) 
    pAUTOPORT = pAUTOPORT_DEFAULT ;

  dwBytes = MAX_PATH_SIZE ;
  if ( dwGetRegString(	HKEY_LOCAL_MACHINE, 
			"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment", "AUTOROUTER", (unsigned char*)pAUTOROUTER, &dwBytes) != 0) 
    pAUTOROUTER = pAUTOROUTER_DEFAULT ;

  /* For AUTOHOST, if there is no env variable, then don't even add the -n switch, don't overwrite pAUTOROUTER */
  dwBytes = MAX_PATH_SIZE ;
  if ( dwGetRegString(	HKEY_LOCAL_MACHINE, 
			"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment", "AUTOHOST", (unsigned char*)pAUTOHOST, &dwBytes) != 0) 
    pAUTOHOST = NULL ;


  dwBytes = MAX_PATH_SIZE ;
  sprintf ( logFile, "%s\\booths.log", pAUTOLOG ) ;
  pLog = fopen ( logFile, "a+" ) ;

  fprintf ( pLog, "PATH is %s\n",path );
  fprintf ( pLog, "AUTOBIN is %s\n",pAUTOBIN) ;
  fprintf ( pLog, "AUTORUN is %s\n",pAUTORUN) ;
  fprintf ( pLog, "AUTOLOG is %s\n",pAUTOLOG) ;
  fprintf ( pLog, "AUTOPORT is %s\n",pAUTOPORT) ;
  fprintf ( pLog, "AUTOROUTER is %s\n",pAUTOROUTER) ;
  if ( pAUTOHOST ) fprintf ( pLog, "AUTOHOST is %s\n",pAUTOHOST) ;

  sprintf ( path, 
	    "%s\\%s.exe", 
	    pAUTOBIN,
	    pAUTOROUTER ) ;

  fprintf ( pLog, "Path to HyperScript is %s\n",path) ;

  if ( pAUTOHOST )
    sprintf ( cmd, 
	    "%s.exe -rt %s -u %s -n %s -f %s\\router.hyp -l %s\\router.log", 
	    pAUTOROUTER,
	    pAUTOROUTER,
	    pAUTOHOST,
	    pAUTOPORT, 
	    pAUTORUN, 
	    pAUTOLOG ) ;
  else
    sprintf ( cmd, 
	    "%s.exe -rt %s -n %s -f %s\\router.hyp -l %s\\router.log", 
	    pAUTOROUTER,
	    pAUTOROUTER,
	    pAUTOPORT, 
	    pAUTORUN, 
	    pAUTOLOG ) ;

  fprintf ( pLog, "Executing command '%s'\n", cmd ) ;

  if( !CreateProcess( 
        path,		  // Name of exe 
        cmd,		  // Command line.args 
        NULL,             // Process handle not inheritable. 
        NULL,             // Thread handle not inheritable. 
        FALSE,            // Set handle inheritance to FALSE. 
        DETACHED_PROCESS, // Detached process. 
        NULL,             // Use parent's environment block. 
        NULL,             // Use parent's starting directory. 
        &si,              // Pointer to STARTUPINFO structure.
        &pi ) ) {         // Pointer to PROCESS_INFORMATION structure.
    fprintf ( pLog, "Failed to create hyperscript process\n" ) ;
    fclose ( pLog ) ;
    pLog = NULL ;
    return FALSE ;
  }

  fprintf ( pLog, "HyperScript running\n" ) ;
  fclose ( pLog ) ;
  pLog = NULL ;
  processHandle = pi.hProcess ;
  runningService = TRUE;
  return TRUE;
}

// Resumes a paused service
VOID ResumeService()
{
	resumeService=FALSE;
	if ( pLog ) fprintf ( pLog, "resumeService();\n" ) ;
	//ResumeThread(threadHandle);
}

// Pauses the service
VOID PauseService()
{
	pauseService = TRUE;
	if ( pLog ) fprintf ( pLog, "pauseService();\n" ) ;
	//SuspendThread(threadHandle);
}

VOID terminateRouter()
{
	OVERLAPPED      mailslotOverlapped ;
	unsigned long nBytes ;
        char mailslot[MAX_PATH_SIZE+1] ;
        char msg[MAX_PATH_SIZE+1] ;

	memset( &mailslotOverlapped, 0, sizeof (OVERLAPPED) ) ;
	mailslotOverlapped.hEvent = CreateEvent ( 0, TRUE, FALSE, 0 ) ;
	mailslotOverlapped.Internal = 1 ;

	if ( pLog ) fclose ( pLog ) ;

	pLog = fopen ( logFile, "a+" ) ;
	fprintf ( pLog, "Opening mailslot to tell router to terminate\n" ) ;
	fclose ( pLog ) ;

        sprintf ( mailslot, 
	    "\\\\.\\mailslot\\%s",
	    pAUTOROUTER ) ;

	// Open the mailslot to the router 
	HANDLE s = CreateFile( 
		  mailslot,
                  (GENERIC_WRITE | GENERIC_READ),
                  (FILE_SHARE_WRITE | FILE_SHARE_READ),
                  NULL,                        
                  OPEN_EXISTING,              
                  (FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED),  
                  NULL);          

	pLog = fopen ( logFile, "a+" ) ;
        if ( s != INVALID_HANDLE_VALUE ) {

	  fprintf ( pLog, "Requesting router to terminate\n" ) ;

          sprintf ( msg, "|%s|event|TERMINATE|god|||", pAUTOROUTER ) ;
	  int status = WriteFile( 
			(HANDLE) s, 
			msg,        
			strlen ( msg ), 
			&nBytes, 
			(LPOVERLAPPED) &mailslotOverlapped ) ;

	  if ( status ) CloseHandle ( s ) ;
	  fprintf ( pLog, "Completed\n" ) ;
	}
	else 
	  fprintf ( pLog, "could not open mailslot\n" ) ;

	fclose ( pLog ) ;
	pLog = NULL ;
} ;

// Stops the service by allowing ServiceMain to
// complete
VOID StopService() 
{
	runningService=FALSE;
	// Set the event that is holding ServiceMain
	// so that ServiceMain can return
	if ( pLog ) fprintf ( pLog, "stopService();\n" ) ;
	terminateRouter() ;
	SetEvent(terminateEvent);
}

// This function consolidates the activities of 
// updating the service status with
// SetServiceStatus
BOOL SendStatusToSCM (DWORD dwCurrentState,
	DWORD dwWin32ExitCode, 
	DWORD dwServiceSpecificExitCode,
	DWORD dwCheckPoint,
	DWORD dwWaitHint)
{
	BOOL success;
	SERVICE_STATUS serviceStatus;

	// Fill in all of the SERVICE_STATUS fields
	serviceStatus.dwServiceType =
		SERVICE_WIN32_OWN_PROCESS;
	serviceStatus.dwCurrentState = dwCurrentState;

	// If in the process of something, then accept
	// no control events, else accept anything
	if (dwCurrentState == SERVICE_START_PENDING)
		serviceStatus.dwControlsAccepted = 0;
	else
		serviceStatus.dwControlsAccepted = 
			SERVICE_ACCEPT_STOP |
			SERVICE_ACCEPT_PAUSE_CONTINUE |
			SERVICE_ACCEPT_SHUTDOWN;

	// if a specific exit code is defined, set up
	// the win32 exit code properly
	if (dwServiceSpecificExitCode == 0)
		serviceStatus.dwWin32ExitCode =
			dwWin32ExitCode;
	else
		serviceStatus.dwWin32ExitCode = 
			ERROR_SERVICE_SPECIFIC_ERROR;
	serviceStatus.dwServiceSpecificExitCode =
		dwServiceSpecificExitCode;

	serviceStatus.dwCheckPoint = dwCheckPoint;
	serviceStatus.dwWaitHint = dwWaitHint;

	// Pass the status record to the SCM
	success = SetServiceStatus (serviceStatusHandle,
		&serviceStatus);
	if (!success)
		StopService();

	return success;
}

// Dispatches events received from the service 
// control manager

VOID ServiceCtrlHandler (DWORD controlCode) 
{
	//DWORD currentState = 0;
	BOOL success;

	if ( !pLog ) pLog = fopen ( logFile, "a+" ) ;

	switch(controlCode)
	{
		// There is no START option because
		// ServiceMain gets called on a start

		// Stop the service
		case SERVICE_CONTROL_STOP:
			fprintf ( pLog, "cmd = SERVICE_STOP_PENDING\n" ) ;
			previousState = currentState ;
			currentState = SERVICE_STOP_PENDING;
			fprintf ( pLog, "currentstate = %d\n",currentState ) ;

			// Tell the SCM what's happening
			success = SendStatusToSCM(
				SERVICE_STOP_PENDING,
				NO_ERROR, 0, 1, 5000);
			// Not much to do if not successful

			// Stop the service
			StopService();
			return;

		// Pause the service
		case SERVICE_CONTROL_PAUSE:
			fprintf ( pLog, "req = SERVICE_CONTROL_PAUSE\n" ) ;
			if (runningService && !pauseService) {
				fprintf ( pLog, "cmd = SERVICE_CONTROL_PAUSE\n" ) ;
				// Tell the SCM what's happening
				success = SendStatusToSCM(
					SERVICE_PAUSE_PENDING,
					NO_ERROR, 0, 1, 1000);
				PauseService();
				previousState = currentState ;
				currentState = SERVICE_PAUSED;
				fprintf ( pLog, "currentstate = %d\n",currentState ) ;
			}
			break;

		// Resume from a pause
		case SERVICE_CONTROL_CONTINUE:
			fprintf ( pLog, "req = SERVICE_CONTROL_CONTINUE\n" ) ;
			if (runningService && pauseService)
			{
				fprintf ( pLog, "cmd = SERVICE_CONTROL_CONTINUE\n" ) ;
				// Tell the SCM what's happening
				success = SendStatusToSCM(
					SERVICE_CONTINUE_PENDING,
					NO_ERROR, 0, 1, 1000);
					ResumeService();
  					previousState = currentState ;
					currentState = SERVICE_RUNNING;
				fprintf ( pLog, "currentstate = %d\n",currentState ) ;
			}
			break;

		// Update current status
		case SERVICE_CONTROL_INTERROGATE:

		   if (runningService) {
		     if ( currentState != previousState ) {
		       fprintf ( pLog, "cmd = SERVICE_INTERRGATE, set state=RUNNING\n" ) ;
		       previousState = currentState ;
		       fprintf ( pLog, "currentstate = %d\n",SERVICE_RUNNING ) ;
		     }
	 	     currentState = SERVICE_RUNNING;
		   }
		   else {
		     if ( currentState != previousState ) {
		       fprintf ( pLog, "cmd = SERVICE_INTERRGATE, set state=STOP_PENDING\n" ) ;
		       previousState = currentState ;
		       fprintf ( pLog, "currentstate = %d\n",SERVICE_STOP_PENDING ) ;
		     }
		     currentState = SERVICE_STOP_PENDING;
		   }
		  // it will fall to bottom and send status
		  break;

		// Do nothing in a shutdown. Could do cleanup
		// here but it must be very quick.
		case SERVICE_CONTROL_SHUTDOWN:
			fprintf ( pLog, "req = SERVICE_SHUTDOWN\n" ) ;
			// Do nothing on shutdown
			return;
		default:
			fprintf ( pLog, "req = %d\n" ) ;
 			break;
	}
	SendStatusToSCM(currentState, NO_ERROR,	0, 0, 0);

	fclose ( pLog ) ;
	pLog = NULL ;

}



// Handle an error from ServiceMain by cleaning up
// and telling SCM that the service didn't start.
VOID terminate(DWORD error)
{
	if ( !pLog ) pLog = fopen ( logFile, "a+" ) ;

	fprintf ( pLog, "terminate(%d);\n", error ) ;
	if (terminateEvent) {
		// Likely set by from StopService() ;
		CloseHandle(terminateEvent);
	}

	// Send a message to the scm to tell about
	// stopage
	if (serviceStatusHandle) {
	  fprintf ( pLog, "Requesting SERVICE_STOPPED\n" ) ;
	  pLog = NULL ;
	  SendStatusToSCM(SERVICE_STOPPED, error, 0, 0, 0);
	}

	fclose ( pLog ) ;
	pLog = NULL ;		

	// If the thread is still defined, terminate it
	if (processHandle) {
	   TerminateProcess(processHandle,0);
	}

	// Do not need to close serviceStatusHandle
}

// ServiceMain is called when the SCM wants to
// start the service. When it returns, the service
// has stopped. It therefore waits on an event
// just before the end of the function, and
// that event gets set when it is time to stop. 
// It also returns on any error because the
// service cannot start if there is an eror.
VOID ServiceMain(DWORD argc, LPTSTR *argv) 
{
	BOOL success;

	// immediately call Registration function
	serviceStatusHandle =
		RegisterServiceCtrlHandler(
			SERVICE_NAME,
			(LPHANDLER_FUNCTION) ServiceCtrlHandler);
	if (!serviceStatusHandle)
	{
		terminate(GetLastError());
		return;
	}

	// Notify SCM of progress
	success = SendStatusToSCM(
		SERVICE_START_PENDING,
		NO_ERROR, 0, 1, 5000);
	if (!success)
	{
		terminate(GetLastError()); 
		return;
	}

	// create the termination event
	terminateEvent = CreateEvent (0, TRUE, FALSE,
		0);
	if (!terminateEvent)
	{
		terminate(GetLastError());
		return;
	}

	// Notify SCM of progress
	success = SendStatusToSCM(
		SERVICE_START_PENDING,
		NO_ERROR, 0, 2, 1000);
	if (!success)
	{
		terminate(GetLastError()); 
		return;
	}

	// Check for startup params
	if (argc == 2)
	{
		int temp = atoi(argv[1]);
	}

	// Notify SCM of progress
	success = SendStatusToSCM(
		SERVICE_START_PENDING,
		NO_ERROR, 0, 3, 5000);
	if (!success)
	{
		terminate(GetLastError()); 
		return;
	}

	// Start the service itself
	success = InitService();
	if (!success)
	{
		terminate(GetLastError());
		return;
	}

	// The service is now running. 
	// Notify SCM of progress
	success = SendStatusToSCM(
		SERVICE_RUNNING,
		NO_ERROR, 0, 0, 0);
	if (!success)
	{
		terminate(GetLastError()); 
		return;
	}

	// Wait for stop signal, and then terminate
	WaitForSingleObject (terminateEvent, INFINITE);

	terminate(0);
}


VOID main(VOID)
{
	SERVICE_TABLE_ENTRY serviceTable[] = 
	{ 
	{ SERVICE_NAME,
		(LPSERVICE_MAIN_FUNCTION) ServiceMain},
	{ NULL, NULL }
	};
	BOOL success;

	// Register with the SCM
	success = 
		StartServiceCtrlDispatcher(serviceTable);
	if (!success)
		ErrorHandler("In StartServiceCtrlDispatcher",
			GetLastError());
}
