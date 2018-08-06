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
/***            N a t i v e   S P I   D e v i c e   D r i v e r            ***/
/***                                                                       ***/
/***                                                                       ***/
/*****************************************************************************/
/*** File:     nspiio.h                                                    ***/
/*** Authors:  Hartmut Keller                                              ***/
/*** Created:  14.03.2005                                                  ***/
/*** Modified: 28.09.2006 21:06:40 (HK)                                    ***/
/***                                                                       ***/
/*** Description:                                                          ***/
/*** Include file for SPI IOCTLs. You have to include this file to use the ***/
/*** NSPI driver in your own applications.                                 ***/
/***                                                                       ***/
/*** Modification History:                                                 ***/
/*** 07.04.2006 HK: Change from SPI to NSPI                                ***/
/*****************************************************************************/

/*-----------------------------------------------------------------------------
  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.

  Copyright (c) 2005 F&S Elektronik Systeme GmbH
-----------------------------------------------------------------------------*/

#ifndef __NSPIIO_H__
#define __NSPIIO_H__

#include "WINIOCTL.h"                     /* CTL_CODE(), METHOD_BUFFERED,... */

/* -------------------- Exported Definitions ------------------------------- */

/* With each clock cycle, the SPI transfer sends one bit on the MOSI line and
   receives one bit on the MISO line. Therefore sending and receiving is done
   at the same time. After one byte is sent, also a byte is received. This
   allows the following transmissions:

   1. Send-only: the received bytes are meaningless and therefore discarded.
          --> IOCTL_NSPI_SEND
   2. Receive-only: the sent data bytes are ignored at the device, therefore
      don't matter. Usually the value 0xFF is used as dummy value.
          --> IOCTL_NSPI_RECEIVE
   3. Send and receive: both data directions carry meaningful data.
      3a. Independent transfer: the data bytes to send are taken from one
          place and the received data bytes are stored at a different place.
          --> IOCTL_NSPI_TRANSFER
      3b. Replacing transfer: the received data bytes are stored at the same
          loaction as the bytes to send, replacing them one after the other.
          --> IOCTL_NSPI_EXCHANGE

   Table of parameters for DeviceIoControl():

   Transfer type        IN-data               OUT-data before / after
   ----------------------------------------------------------------
   IOCTL_NSPI_SEND      Command & send data   (unused)  / (unused)
   IOCTL_NSPI_SEND      Command               Send data / Send data
   IOCTL_NSPI_RECEIVE   Command               (unused)  / Received data
   IOCTL_NSPI_TRANSFER  Command & send data   (unused)  / Received data
   IOCTL_NSPI_EXCHANGE  Command               Send data / Received data

   Most SPI devices require some command bytes to determine what to do before
   transmitting the actual data. This is a send-only phase, i.e. the bytes
   received during this phase are discarded. If the device does not require
   command bytes, the command part may be left empty.

   When using IOCTL_NSPI_TRANSFER, the command size is determined by the
   difference of the IN-data and OUT-data array sizes. For example if 10 bytes
   go in and 8 bytes go out, the command size is 2 bytes. 

   When using IOCTL_NSPI_SEND, you can either send the data as part of the
   command or as separate data in the OUT-array. This will generate the same
   output to the device, but it allows easier data handling in some cases.*/
   
/* New IOControlCode values */
#define FILE_DEVICE_NSPI    0x0000800A

/* Send command and data to SPI device */
#define IOCTL_NSPI_SEND \
    CTL_CODE(FILE_DEVICE_NSPI, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)

/* Send command to SPI device and then receive data from SPI device */
#define IOCTL_NSPI_RECEIVE \
    CTL_CODE(FILE_DEVICE_NSPI, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)

/* Send command and data to SPI device, receive data from SPI device */
#define IOCTL_NSPI_TRANSFER \
    CTL_CODE(FILE_DEVICE_NSPI, 0x802, METHOD_BUFFERED, FILE_ANY_ACCESS)

/* Like IOCTL_SPI_TRANSFER, but replace send data with receive data */
#define IOCTL_NSPI_EXCHANGE \
    CTL_CODE(FILE_DEVICE_NSPI, 0x803, METHOD_BUFFERED, FILE_ANY_ACCESS)


#endif /*!__NSPIIO_H__*/
