// removebooths.cpp

#include <windows.h>
#include <iostream>

void ErrorHandler(char *s, DWORD err)
{
  std::cout << s << std::endl;
  std::cout << "Error number: " << err << std::endl;
  ExitProcess(err);
}

void main()
{
  SC_HANDLE service, scm;
  BOOL success;
  SERVICE_STATUS status;
	
  std::cout << "Removing Boot HyperScript AutoRouter...\n";

  // Open a connection to the SCM
  scm = OpenSCManager(0, 0, SC_MANAGER_CREATE_SERVICE);
  if (!scm)
    ErrorHandler("In OpenScManager", GetLastError());

  // Get the service's handle
  service = OpenService(
		scm,
		"booths",
		SERVICE_ALL_ACCESS | DELETE);
  if (!service)
    ErrorHandler("In OpenService", GetLastError());
	
  // Stop the service if necessary
  success = QueryServiceStatus(service, &status);
  if (!success)
    ErrorHandler("In QueryServiceStatus", GetLastError());

  if (status.dwCurrentState != SERVICE_STOPPED) {

    std::cout << "Stopping service...\n";
    success = ControlService(
		service,
		SERVICE_CONTROL_STOP, 
		&status);
    if (!success)
      ErrorHandler("In ControlService", GetLastError());
    Sleep(500);
  }
	
  // Remove the service
  success = DeleteService(service);
  if (success)
    std::cout << "Service removed\n";
  else
    ErrorHandler("In DeleteService", GetLastError());

  // Clean up
  CloseServiceHandle(service);
  CloseServiceHandle(scm);
  std::cout << "Ending...\n";
}
