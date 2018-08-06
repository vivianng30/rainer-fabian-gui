/*****************************************************************************/
/***                                                                       ***/
/***                                                                       ***/
/***               D r i v e r  I n f o - H e a d e r  f i l e             ***/
/***                                                                       ***/
/***                                f o r                                  ***/
/***                                                                       ***/
/***              N e t D C U / P i c o M O D / P i c o C O M              ***/
/***                                                                       ***/
/***                                                                       ***/
/*****************************************************************************/
/*** File:     fs_driverinfo.h                                             ***/
/*** Authors:  Martin Kiepfer                                              ***/
/*** Created:  10.10.2008                                                  ***/
/*** Modified: 13.10.2008 09:10:12 (MK)                                    ***/
/***                                                                       ***/
/*** Description:                                                          ***/
/*** Header file for the driver info IO-Control that is common for all de- ***/
/*** vice drivers.                                                         ***/
/***                                                                       ***/
/*** Modification History:                                                 ***/
/*** 10.10.08: First Version                                               ***/
/*****************************************************************************/
/*** THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY ***/
/*** KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE   ***/
/*** IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR ***/
/*** PURPOSE.                                                              ***/
/*** Copyright (c) 2008  F&S Elektronik Systeme GmbH                       ***/
/*****************************************************************************/

#ifndef __FS_DRIVERINFO_H__
#define __FS_DRIVERINFO_H__

#include <WINIOCTL.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Structure to hold the driver information */
typedef struct tagDRIVER_INFO
{
    WORD wVerMajor;
    WORD wVerMinor;
    DWORD dwTemp[15];
} DRIVER_INFO, *PDRIVER_INFO;


/* IO-Control code definition  */
#define FILE_DEVICE_FSDRIVER 0x000080F0

// IOCTL_DIO_GETINFO:
// Read driver info I/O's
// DRIVER_INFO cInfo;
// DeviceIoControl( hDIO, IOCTL_DRIVER_GETINFO, 
//                  NULL, 0, 
//                  &cInfo, sizeof(DRIVER_INFO), NULL );
#define IOCTL_DRIVER_GETINFO CTL_CODE(FILE_DEVICE_FSDRIVER, 1, METHOD_BUFFERED, FILE_ANY_ACCESS)


#ifdef __cplusplus
}
#endif


#endif /*__FS_DRIVERINFO_H__*/
