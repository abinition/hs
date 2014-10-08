//***************************************************************
// From the book "Win32 System Services: The Heart of Windows 95
// and Windows NT"
// by Marshall Brain
// Published by Prentice Hall
//
// Copyright 1995, by Prentice Hall.
//
// This code removes a service from the Services applet in the
// Control Panel.
//***************************************************************

// remove.cpp

#include <windows.h>
#include <iostream.h>

void ErrorHandler(char *s, DWORD err)
{
	cout << s << endl;
	cout << "Error number: " << err << endl;
	ExitProcess(err);
}

void main(int argc, char *argv[])
{
	SC_HANDLE service, scm;
	BOOL success;
	SERVICE_STATUS status;

	if (argc != 2)
	{
		cout << "Usage:\n";
		cout << "   remove service_name\n";
		return;
	}
	
	cout << "Starting...\n";
	// Open a connection to the SCM
	scm = OpenSCManager(0, 0,
		SC_MANAGER_CREATE_SERVICE);
	if (!scm)
		ErrorHandler("In OpenScManager",
			GetLastError());

	// Get the service's handle
	service = OpenService(
		scm, argv[1],
		SERVICE_ALL_ACCESS | DELETE);
	if (!service)
		ErrorHandler("In OpenService",
			GetLastError());
	
	// Stop the service if necessary
	success = QueryServiceStatus(service, &status);
	if (!success)
		ErrorHandler("In QueryServiceStatus",
			GetLastError());
	if (status.dwCurrentState != SERVICE_STOPPED)
	{
		cout << "Stopping service...\n";
		success = ControlService(service,
			SERVICE_CONTROL_STOP, 
			&status);
		if (!success)
			ErrorHandler("In ControlService",
				GetLastError());
		Sleep(500);
	}
	
	// Remove the service
	success = DeleteService(service);
	if (success)
		cout << "Service removed\n";
	else
		ErrorHandler("In DeleteService",
			GetLastError());

	// Clean up
	CloseServiceHandle(service);
	CloseServiceHandle(scm);
	cout << "Ending...\n";
}
