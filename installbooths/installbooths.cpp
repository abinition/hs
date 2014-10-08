// installbooths.cpp

#include <windows.h>
#include <iostream>
#include <stdio.h>

void ErrorHandler(char *s, DWORD err)
{
  std::cout << s << std::endl;
  std::cout << "Error number: " << err << std::endl;
  ExitProcess(err);
}

#define MAX_PATH_SIZE 108
void main()
{
  SC_HANDLE newService, scm;

  char *pAUTOBIN_DEFAULT = "\\local\\bin" ;

  char *pAUTOBIN ;
  char path[MAX_PATH_SIZE+1] ;

  pAUTOBIN  = getenv ( "AUTOBIN" ) ;
  if ( pAUTOBIN == NULL ) pAUTOBIN = pAUTOBIN_DEFAULT ;

  sprintf ( path, 
	    "%s\\booths", 
	    pAUTOBIN ) ;

  std::cout << "Installing " << path << "\n";
	
  // open a connection to the SCM
  scm = OpenSCManager(0, 0, SC_MANAGER_CREATE_SERVICE);
  if (!scm)
    ErrorHandler("In OpenScManager",GetLastError());

  // Install the new service
  newService = CreateService(
		scm,
		"booths",
		"Boot HyperScript AutoRouter",
		SERVICE_ALL_ACCESS,
		SERVICE_WIN32_OWN_PROCESS,
		SERVICE_DEMAND_START,
		SERVICE_ERROR_NORMAL,
		path,
		0, 0, 0, 0, 0);
  if (!newService)
    ErrorHandler("In CreateService", GetLastError());
  else
    std::cout << "Service installed\n";

  // clean up
  CloseServiceHandle(newService);
  CloseServiceHandle(scm);
  std::cout << "Ending...\n";
}
