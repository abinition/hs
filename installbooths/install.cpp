//***************************************************************
// From the book "Win32 System Services: The Heart of Windows 95
// and Windows NT"
// by Marshall Brain
// Published by Prentice Hall
//
// Copyright 1995, by Prentice Hall.
//
// This code installs a service.
//***************************************************************

// install.cpp

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
	SC_HANDLE newService, scm;

	if (argc != 4)
	{
		cout << "Usage:\n";
		cout << "   install service_name \
service_label executable\n";
		cout << "           service_name is the \
name used internally by the SCM\n";
		cout << "           service_label is the \
name that appears in the Services applet\n";
		cout << "              (for multiple \
words, put them in double quotes)\n";
		cout << "           executable is the \
full path to the EXE\n";
		cout << endl;
		return;
	}

	cout << "Starting...\n";
	// open a connection to the SCM
	scm = OpenSCManager(0, 0,
		SC_MANAGER_CREATE_SERVICE);
	if (!scm)
		ErrorHandler("In OpenScManager",
			GetLastError());

	// Install the new service
	newService = CreateService(
		scm, argv[1], // eg "beep_srv"
		argv[2],      // eg "Beep Service"
		SERVICE_ALL_ACCESS,
		SERVICE_WIN32_OWN_PROCESS,
		SERVICE_DEMAND_START,
		SERVICE_ERROR_NORMAL,
		argv[3],      // eg "c:\winnt\xxx.exe"
		0, 0, 0, 0, 0);
	if (!newService)
		ErrorHandler("In CreateService",
			GetLastError());
	else
		cout << "Service installed\n";

	// clean up
	CloseServiceHandle(newService);
	CloseServiceHandle(scm);
	cout << "Ending...\n";
}
