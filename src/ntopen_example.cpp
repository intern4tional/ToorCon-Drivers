// OpenASystemDriver.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>
#include <winternl.h>
#include <ntddkbd.h>

#pragma comment(lib, "ntdll")

#define YOUR_DEVICE_SHORT_NAME L"DeviceName"
#define YOUR_DEVICE_TYPE 0x9999
#define YOUR_DEVICE_NAME (L"\\Device\\" YOUR_DEVICE_SHORT_NAME)

std::string GetLastErrorAsString()
{
	//Get the error message, if any.
	DWORD errorMessageID = ::GetLastError();
	if (errorMessageID == 0)
		return std::string(); //No error message has been recorded

	LPSTR messageBuffer = nullptr;
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

	std::string message(messageBuffer, size);

	//Free the buffer.
	LocalFree(messageBuffer);

	return message;
}

int main()
{
	NTSTATUS status;
	HANDLE deviceHandle = INVALID_HANDLE_VALUE;
	UNICODE_STRING objectName;
	OBJECT_ATTRIBUTES objectAttributes;
	IO_STATUS_BLOCK isb;

	RtlInitUnicodeString(&objectName, YOUR_DEVICE_NAME);

	InitializeObjectAttributes(
		&objectAttributes,
		&objectName,
		OBJ_CASE_INSENSITIVE,
		NULL,
		NULL
	);

	status = NtOpenFile(
		&deviceHandle,
		FILE_GENERIC_READ | FILE_GENERIC_WRITE,
		&objectAttributes,
		&isb,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		FILE_NON_DIRECTORY_FILE
	);

	if (deviceHandle == INVALID_HANDLE_VALUE)
	{
		std::cout << "Unable to get handle on device.  GetLastError: " << GetLastError() << std::endl;
		std::cout << "GetLastError: " << GetLastErrorAsString() << std::endl;
	}
	else if (NT_SUCCESS(status))
	{
		std::cout << "Handle is valid and NT Status = success." << std::endl;
	}

	NtClose(deviceHandle);
}