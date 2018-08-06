

#ifndef __SPIIO_H__
#define __SPIIO_H__

#include "WINIOCTL.h"                     /* CTL_CODE(), METHOD_BUFFERED,... */

/* -------------------- Exported Definitions ------------------------------- */

/* New IOControlCode values */
#define FILE_DEVICE_SPI     0x0000800A

/* Read data from SPI device */
#define IOCTL_SPI_READ \
    CTL_CODE(FILE_DEVICE_SPI, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)

/* Transmit data to SPI device */
#define IOCTL_SPI_WRITE \
    CTL_CODE(FILE_DEVICE_SPI, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)

/* Do combined write and read acces on SPI device */
#define IOCTL_SPI_TRANSFER \
    CTL_CODE(FILE_DEVICE_SPI, 0x802, METHOD_BUFFERED, FILE_ANY_ACCESS)


/* -------------------- Exported Types ------------------------------------- */

/* If pointer to data is NULL, the according transfer does not take place;
   e.g. if pchReadBuf is NULL, no reading is done. The command is sent before
   the normal data exchange takes place. The data received while sending the
   command sequence is discarded. */
typedef struct
{
    unsigned char *pchCommandBuf;         /* Buffer with command data */
    unsigned char *pchWriteBuf;           /* Buffer with data to send */
    unsigned char *pchReadBuf;            /* Buffer for received data */
    unsigned short wCommandLen;           /* Number of bytes to write only */
    unsigned short wDataLen;              /* Number of bytes to read/write */
} SPI_DATA, *PSPI_DATA;

#endif /*!__SPIIO_H__*/
