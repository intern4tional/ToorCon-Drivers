// enumDrivers.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include <Psapi.h>

#define ARRAY_SIZE 1024

int main()
{
	LPVOID drivers[ARRAY_SIZE];
	DWORD cbNeeded;
	int cDrivers, i;

	if (EnumDeviceDrivers(drivers, sizeof(drivers), &cbNeeded) && cbNeeded < sizeof(drivers))
	{
		TCHAR szDriver[ARRAY_SIZE];
		cDrivers = cbNeeded / sizeof(drivers[0]);

		std::cout << "There are " << cDrivers << " drivers." << std::endl;
		for (i = 0; i < cDrivers; i++)
		{
			if (GetDeviceDriverFileName(drivers[i], szDriver, sizeof(szDriver) / sizeof(szDriver[0])))
			{
				_tprintf(TEXT("%d: %s\n"), i + 1, szDriver);
			}
		}
	}
	else
	{
		_tprintf(TEXT("EnumDeviceDrivers failed; array size needed is %d\n"), cbNeeded / sizeof(LPVOID));
		return 1;
	}
	return 0;
}