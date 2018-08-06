/*++
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.
Copyright (c) 2005 F&S Elektronik Systeme GmbH

Module Name:  

    parintfsdk.h

Abstract:

    This module contains the constant, type and structure definitions for the
    NetDCU3-WINCE Parallel Interface (PARINTF.DLL) device driver.

Notes: 


--*/
#ifndef _PARINTFSDK_H__
#define _PARINTFSDK_H__

#ifdef __cplusplus
extern "C" {
#endif


// We'll need some defines
#include "WINIOCTL.h"

// New IOControlCode values

#define FILE_DEVICE_PARINTF		0x00008004


// Installs a event that will be triggered if a interrupt occurs
#define IOCTL_PARINTF_INIT_IRQ    \
    CTL_CODE(FILE_DEVICE_PARINTF, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)
// Example:
// HANDLE hEvent;
// DWORD dwSysIntr;
// hEVent = CreateEvent(...);
// DeviceIoControl( hOpenCotext, IOCTL_PARINTF_INIT_IRQ, &hEvent, sizeof(hEvent), &dwSysIntr, sizeof(DWORD), NULL, NULL );


// Release dwIntID after usage
#define IOCTL_PARINTF_RELEASE_SYSINTR  \
    CTL_CODE(FILE_DEVICE_PARINTF, 0x802, METHOD_BUFFERED, FILE_ANY_ACCESS)
// Example:
// HANDLE hEvent;
// DWORD dwSysIntr; // returned from IOCTL_PARINTF_INIT_IRQ
// hEVent = CreateEvent(...);
// DeviceIoControl( hOpenCotext, IOCTL_PARINTF_RELEASE_SYSINTR, &dwSysIntr, sizeof(DWORD), NULL, 0, NULL, NULL );


#define IOCTL_PARINTF_READ    \
    CTL_CODE(FILE_DEVICE_PARINTF, 0x803, METHOD_BUFFERED, FILE_ANY_ACCESS)
// Example:
// PARINTFRW cData;
// hEVent = CreateEvent(...);
// DeviceIoControl( hOpenCotext, IOCTL_PARINTF_READ, &cData, sizeof(cData), NULL, 0, NULL, NULL );

#define IOCTL_PARINTF_WRITE    \
    CTL_CODE(FILE_DEVICE_PARINTF, 0x804, METHOD_BUFFERED, FILE_ANY_ACCESS)
// Example:
// PARINTFRW cData;
// hEVent = CreateEvent(...);
// DeviceIoControl( hOpenCotext, IOCTL_PARINTF_WRITE, &cData, sizeof(cData), NULL, 0, NULL, NULL );

typedef struct tagPARINTFRW
{
	BYTE chAddress;
	BYTE chData;
} PARINTFRW;

#ifdef __cplusplus
}
#endif

#endif/*_PARINTFSDK_H__*/
