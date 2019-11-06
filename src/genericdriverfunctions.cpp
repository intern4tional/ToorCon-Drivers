// genericdriverfunctions.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Presumes unicode is defined and wide strings will be required

#include <iostream>
#include <Windows.h>
#include <Winsvc.h>
#include <strsafe.h>

std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

bool installDeviceDriver(const std::string driverName, const std::string driverFileLocation)
{
	NTSTATUS status;
	SC_HANDLE serviceControlManagerHandle;
	SC_HANDLE serviceHandle;

	// Get a handle to the Service Control Manager
	serviceControlManagerHandle = OpenSCManager(
		NULL,
		NULL,
		SC_MANAGER_CREATE_SERVICE); // Required for installation of the driver

	if (!serviceControlManagerHandle)
	{
		return false; //failure
	}

	serviceHandle = CreateService(
		serviceControlManagerHandle,
		s2ws(driverName).c_str(),
		s2ws(driverName).c_str(),
		SERVICE_ALL_ACCESS,
		SERVICE_KERNEL_DRIVER,
		SERVICE_SYSTEM_START,
		SERVICE_ERROR_IGNORE,
		s2ws(driverFileLocation).c_str(),
		NULL,
		NULL,
		NULL,
		NULL,
		L""
	);

	if (!serviceHandle)
	{
		CloseServiceHandle(serviceControlManagerHandle);
		return false;
	}

	if (!StartService(serviceHandle, 0, NULL))
	{
		CloseServiceHandle(serviceHandle);
		CloseServiceHandle(serviceControlManagerHandle);
		return false;
	}

	CloseServiceHandle(serviceHandle);
	CloseServiceHandle(serviceControlManagerHandle);
	return true;
}

int main()
{
	std::string drivername = "some_name";
	std::string driverlocation = "some_location";
	installDeviceDriver(drivername, driverlocation);
}
