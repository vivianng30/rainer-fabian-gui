/*****************************************************************************/
/***                                                                       ***/
/***                                                                       ***/
/***                   D i g i t a l - I O   D r i v e r                   ***/
/***                                                                       ***/
/***                                f o r                                  ***/
/***                                                                       ***/
/***              N e t D C U / P i c o M O D / P i c o C O M              ***/
/***                                                                       ***/
/***                                                                       ***/
/*****************************************************************************/
/*** File:     dio_sdk.h                                                   ***/
/*** Author:   H. Froelich, H. Keller, D. Kuhne                            ***/
/*** Created:  12.02.2004                                                  ***/
/*** Modified: 26.01.2009 16:26:43 (MK)                                    ***/
/***                                                                       ***/
/*** Description:                                                          ***/
/*** This module contains the constant, type and structure definitions for ***/
/*** the Digital I/O Interface device driver.                              ***/
/***                                                                       ***/
/*** Modification History:                                                 ***/
/*** 2008-11-20: add: WAIT_IRQ struct.					   ***/
/*** 2008-05-27: add: IOCTLs for Interrupt functionality		   ***/
/*****************************************************************************/
/*** THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY ***/
/*** KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE   ***/
/*** IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR ***/
/*** PURPOSE.                                                              ***/
/*** Copyright (c) 2004 F&S Elektronik Systeme GmbH.  All rights reserved. ***/
/*****************************************************************************/
#ifndef __DIOSDK_H__
#define __DIOSDK_H__

#ifdef __cplusplus
extern "C" {
#endif


// We'll need some defines
#include "WINIOCTL.h"

#define FILE_DEVICE_DIO		0x00008007

// IOCTL_DIO_REQUEST_IRQ:
// Request and initialize an IO interrupt.
//
//    DWORD dwIRQ = 16; // Pin 5 is IRQ 16 (see DeviceDriver documentation)
//    DeviceIoControl( hDIO, IOCTL_DIO_REQUEST_IRQ, 
//                     &dwIRQ, sizeof(DWORD),
//                     dwWaitState, sizeof(DWORD), NULL );
#define IOCTL_DIO_REQUEST_IRQ   CTL_CODE(FILE_DEVICE_DIO, 41, METHOD_BUFFERED, FILE_ANY_ACCESS)

// IOCTL_DIO_RELEASE_IRQ:
// Release an interrupt, previously registered with IOCTL_DIO_REQUEST_IRQ.
//
//    DWORD dwIRQ = 16; // Pin 5 is IRQ 16 (see DeviceDriver documentation)
//    DeviceIoControl( hDIO, IOCTL_DIO_RELEASE_IRQ, 
//                     &dwIRQ, sizeof(DWORD),
//                     dwWaitState, sizeof(DWORD), NULL );
#define IOCTL_DIO_RELEASE_IRQ   CTL_CODE(FILE_DEVICE_DIO, 40, METHOD_BUFFERED, FILE_ANY_ACCESS)

// IOCTL_DIO_WAIT_IRQ:
// Wait for an interrupt event. 
// WaitState is optional and stores the event type (return value of
// WaitForSingleObject()).
//
//    DeviceIoControl( hDIO, IOCTL_DIO_INTDONE_IRQ, 
//                     &cWaitIrq, sizeof(WAITIRQ),
//                     dwWaitState, sizeof(DWORD), NULL );
#define IOCTL_DIO_WAIT_IRQ      CTL_CODE(FILE_DEVICE_DIO, 42, METHOD_BUFFERED, FILE_ANY_ACCESS)

// IOCTL_DIO_INTDONE_IRQ:
// Acknolidge interrupt to re-enable it
//
//    DWORD dwIRQ = 16; // Pin 5 is IRQ 16 (see DeviceDriver documentation)
//    WAITIRQ cWait = {5, 0, INVINITE};
//    DeviceIoControl( hDIO, IOCTL_DIO_INTDONE_IRQ, 
//                     &dwIRQ, sizeof(DWORD), 
//                     NULL, 0, NULL );
#define IOCTL_DIO_INTDONE_IRQ   CTL_CODE(FILE_DEVICE_DIO, 43, METHOD_BUFFERED, FILE_ANY_ACCESS)

// Deprecated IO-Controls
// These definitions are only available for compatibility purposes.  They will
// *not* be usable within an usermode application.
#define IOCTL_DIO_REQUEST_SYSINTR   CTL_CODE(FILE_DEVICE_DIO, 38, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_DIO_RELEASE_SYSINTR   CTL_CODE(FILE_DEVICE_DIO, 54, METHOD_BUFFERED, FILE_ANY_ACCESS)


// IOCTL_DIO_SET_PIN
//
//    BYTE chPin = 5;
//    DeviceIoControl( hDIO, IOCTL_DIO_SET_PIN, 
//                     &chPin, sizeof(BYTE), 
//                     NULL, 0, NULL );
#define IOCTL_DIO_SET_PIN (DWORD) CTL_CODE(FILE_DEVICE_DIO, 60, METHOD_BUFFERED, FILE_ANY_ACCESS)

// IOCTL_DIO_CLR_PIN
//
//    BYTE chPin = 5;
//    DeviceIoControl( hDIO, IOCTL_DIO_CLR_PIN, 
//                     &chPin, sizeof(BYTE), 
//                     NULL, 0, NULL );
#define IOCTL_DIO_GET_PIN (DWORD) CTL_CODE(FILE_DEVICE_DIO, 61, METHOD_BUFFERED, FILE_ANY_ACCESS)

// IOCTL_DIO_CLR_PIN
//
//    BYTE chPin = 5;
//    BYTE chReturn;
//    DeviceIoControl( hDIO, IOCTL_DIO_SET_PIN, 
//                     &chPin, sizeof(BYTE), 
//                     &chReturn, sizeof(BYTE), NULL );
#define IOCTL_DIO_CLR_PIN (DWORD) CTL_CODE(FILE_DEVICE_DIO, 62, METHOD_BUFFERED, FILE_ANY_ACCESS)


// IOCTL_DIO_REINIT:
// Read registry and init digital I/O's
//
//    DeviceIoControl( hDIO, IOCTL_DIO_REINIT, 
//                     NULL, 0, 
//                     NULL, 0, NULL );
#define IOCTL_DIO_REINIT CTL_CODE(FILE_DEVICE_DIO, 63, METHOD_BUFFERED, FILE_ANY_ACCESS)

// IOCTL_DIO_GETINFO:
// Read registry and init digital I/O's
//
//    DIO_INFO cInfo;
//    DeviceIoControl( hDIO, IOCTL_DIO_GETINFO, 
//                     NULL, 0, 
//                     &cInfo, sizeof(DIO_INFO), NULL );
#define IOCTL_DIO_GETINFO CTL_CODE(FILE_DEVICE_DIO, 64, METHOD_BUFFERED, FILE_ANY_ACCESS)

// Driver information structure
typedef struct tagDIO_INFO
{
    WORD wVerMajor;
    WORD wVerMinor;
    DWORD dwTemp[15];
} DIO_INFO;

// Information required to wait for an interrupt
typedef struct _WAITIRQ
{
    DWORD dwIOPin;
    BOOL bType;
    DWORD dwTimeOut;
}WAITIRQ, *PWAITIRQ;


#ifdef __cplusplus
}
#endif

#endif/*__DIOSDK_H__*/
