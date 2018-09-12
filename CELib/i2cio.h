/*****************************************************************************/
/***                                                                       ***/
/***      NN    NN             tt     DDDDDD       CCCCCC   UU     UU      ***/
/***      NNN   NN             tt     DD    DD    CC    CC  UU     UU      ***/
/***      NNNN  NN    eeee   tttttt   DD     DD  CC         UU     UU      ***/
/***      NN NN NN  ee    ee   tt     DD     DD  CC         UU     UU      ***/
/***      NN  NNNN  eeeeeeee   tt     DD     DD  CC         UU     UU      ***/
/***      NN   NNN  ee         tt tt  DD    DD    CC    CC  UU     UU      ***/
/***      NN    NN    eeeee     ttt   DDDDDD       CCCCCC     UUUUU        ***/
/***                                                                       ***/
/*****************************************************************************/
/***                                                                       ***/
/***                                                                       ***/
/***            N a t i v e   I 2 C   D e v i c e   D r i v e r            ***/
/***                                                                       ***/
/***                                                                       ***/
/*****************************************************************************/
/*** File:     ni2cio.h                                                    ***/
/*** Authors:  Hartmut Keller                                              ***/
/*** Created:  02.12.2005                                                  ***/
/*** Modified: 20.05.2009 18:31:51 (HK)                                    ***/
/***                                                                       ***/
/*** Description:                                                          ***/
/*** Include file for I2C IOCTLs. You have to include this file to use the ***/
/*** NI2C driver in your own applications.                                 ***/
/***                                                                       ***/
/*** Modification History:                                                 ***/
/*** 20.05.09 HK: Use V2.0 interface from NI2C (keep the NI2C prefixes).   ***/
/*****************************************************************************/

/*-----------------------------------------------------------------------------
  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.

  Copyright (c) 2009 F&S Elektronik Systeme GmbH
-----------------------------------------------------------------------------*/

#ifndef __NI2CIO_H__
#define __NI2CIO_H__

#include "WINIOCTL.h"                     /* CTL_CODE(), METHOD_BUFFERED,... */

/* -------------------- Exported Definitions ------------------------------- */

/* New IOControlCode values */
#define FILE_DEVICE_NI2C     0x00008037

/* Schedule a transmission request */
#define IOCTL_NI2C_SCHEDULE \
    (DWORD) CTL_CODE(FILE_DEVICE_NI2C, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)

/* Wait until a transmission result is available and return it */
#define IOCTL_NI2C_GET_RESULT \
    (DWORD) CTL_CODE(FILE_DEVICE_NI2C, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)

/* Wait until a transmission result is available and discard it */
#define IOCTL_NI2C_SKIP_RESULT \
    CTL_CODE(FILE_DEVICE_NI2C, (DWORD) 0x802, METHOD_BUFFERED, FILE_ANY_ACCESS)

/* Check if the result of a transmission request is available */
#define IOCTL_NI2C_CHECK_RESULT \
    CTL_CODE(FILE_DEVICE_NI2C, 0x803, METHOD_BUFFERED, FILE_ANY_ACCESS)

/* Get the current speed of the I2C bus */
#define IOCTL_NI2C_GET_CLKFREQ \
    CTL_CODE(FILE_DEVICE_NI2C, 0x804, METHOD_BUFFERED, FILE_ANY_ACCESS)

/* Set the current speed of the I2C bus */
#define IOCTL_NI2C_SET_CLKFREQ \
    CTL_CODE(FILE_DEVICE_NI2C, 0x805, METHOD_BUFFERED, FILE_ANY_ACCESS)

/* Schedule a transmission request and wait for result */ 
#define IOCTL_NI2C_TRANSFER \
    CTL_CODE(FILE_DEVICE_NI2C, 0x806, METHOD_BUFFERED, FILE_ANY_ACCESS)

/* Status flags used in NI2C_MSG_HEADER */
enum NI2C_FLAGS
{
    NI2C_FLAGS_LASTBYTE_ACK = 0x01,       /* Receive: Send ACK on last byte;
                                             Transmit: Got ACK on last byte */
    NI2C_FLAGS_DATA_NAK = 0x02,           /* No ACK when sending data */
    NI2C_FLAGS_DEVICE_NAK = 0x04,         /* No ACK when talking to device */
    NI2C_FLAGS_ARBITRATION_LOST = 0x08,   /* Lost arbitration */
    NI2C_FLAGS_TIMEOUT = 0x80,            /* Timeout on I2C bus */
};


/* -------------------- Exported Types ------------------------------------- */

/* A transmission request defines a group of messages to be sent and/or
   received on the I2C bus. Each message can individually send or receive
   to/from any device address with any length. The driver will use RESTART
   between the messages and therefore handles all messages of the transmission
   request in one go, without letting other tasks interrupt the transfer. This
   allows time critical transfers on one hand, but on the other hand can also
   block the bus for quite some time. So try to be fair and split transfers in
   different requests whenever possible.

   A transmission request consists of two parts:
     1. An array of message headers, defining the parameters of the messages.
        Each message header describes the 7-bit address of the device to
        communicate with, the transfer direction (send/receive as the eighth
        bit of the address), and the message length. On receiving messages you
        can determine by setting a flag whether the last received byte should
        be acknowledged or not. 
     2. A byte array containing the concatenated bytes of all messages. For
        receiving messages you have to provide as many dummy bytes with
        arbitrary content.

   Example:
   --------
   msg1: Send three bytes 0x01, 0x02, 0x03 to device 0x40
   msg2: Receive two bytes from device 0x40, don't send ACK on last byte
   msg3: Send two bytes 0x04, 0x05 to device 0x78
   msg4: Receive three bytes from device 0x78, send ACK on last byte


   Message Array[]:  chDevAddr chFlags wLen
   -----------------------------------------------------------
        0            0x40      0x00    0x0003 (msg1, send)
        1            0x41      0x00    0x0002 (msg2, receive, no ACK)
        2            0x78      0x00    0x0002 (msg3, send)
        3            0x79      0x01    0x0003 (msg4, receive, ACK)

   Byte Array[]:     Content
   -----------------------------------------------------------
        0            0x01 (msg1, 1st byte, send)
        1            0x02 (msg1, 2nd byte, send)
        2            0x03 (msg1, 3rd byte, send)
        3            0x00 (msg2, 1st dummy byte, receive)
        4            0x00 (msg2, 2nd dummy byte, receive)
        5            0x04 (msg3, 1st byte, send)
        6            0x05 (msg3, 2nd byte, send)
        7            0x00 (msg4, 1st dummy byte, receive)
        8            0x00 (msg4, 2nd dummy byte, receive)
        9            0x00 (msg4, 3rd dummy byte, receive)

   A transmission request can be scheduled by IOCTL_NI2C_SCHEDULE. This means
   the request is stored in the driver and transferred asynchronously. The
   call returns immediately. By using IOCTL_NI2C_GET_RESULT and the same
   parameters, the result can be obtained later when the transfer is finished.
   After return, the special flags entry in the message headers is valid and
   reports the transfer status of each message individually. And also the byte
   array now contains the received bytes.

   You can schedule several requests in a row with IOCTL_NI2C_GET_RESULT before
   obtaining the results with IOCTL_NI2C_SCHEDULE. But please note that you
   will get the results in the same sequence as you had scheduled the requests
   before and it is important to provide the same parameters with the result
   call as with the corresponding schedule call, or IOCTL_NI2C_GET_RESULT will
   fail.

   In many cases, the asynchronous scheduling of requests and getting the
   results is not required. Therefore starting from V3.0, the driver also
   supports a combined command IOCTL_NI2C_TRANSFER, that schedules a request
   and waits for the result in one go. However this is simply the same as
   issuing IOCTL_NI2C_SCHEDULE and IOCTL_NI2C_GET_RESULT in direct sequence
   and using the same data arrays. */

typedef struct NI2C_MSG_HEADER
{
    unsigned char chDevAddr;              /* Bit 7..1: Device address
                                             Bit 0: 0: Send, 1: Receive */
    unsigned char chFlags;                /* See NI2C_FLAGS above */
    unsigned short wLen;                  /* Message length */
} NI2C_MSG_HEADER, *PNI2C_MSG_HEADER;

#endif /*!__NI2CIO_H__*/

