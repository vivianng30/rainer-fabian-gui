// InterfaceDIO.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "InterfaceDIO.h"
#include "dio_sdk.h"

////////////////////////////////////////////////////////////////////////////////////////
extern "C" BOOL InterruptInitialize(DWORD idInt, HANDLE hEvent, LPVOID pvData, DWORD cbData);
extern "C" VOID InterruptDone( DWORD idInt ); 
extern "C" VOID InterruptDisable( DWORD idInt ); 


#define SYSINTR_FIRMWARE    (8+8)

/**********************************************************************************************//**
 * A macro that defines sysintr pifirq
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

#define SYSINTR_PIFIRQ      (SYSINTR_FIRMWARE+15)

/**********************************************************************************************//**
 * A macro that defines sysintr extio
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

#define SYSINTR_EXTIO       (SYSINTR_FIRMWARE+16)


#define TRIGGER_EDGE	1	// NetDCU6 only (extern interrupts via dio port pins)

/**********************************************************************************************//**
 * A macro that defines trigger level
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

#define TRIGGER_LEVEL	2	// NetDCUX

/**********************************************************************************************//**
 * A macro that defines trigger
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

#define TRIGGER TRIGGER_EDGE

CInterfaceDIO* CInterfaceDIO::theDIOInterface=0;

/**********************************************************************************************//**
 * Initializes a new instance of the CInterfaceDIO class
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

CInterfaceDIO::CInterfaceDIO(void)
{
	m_hDIO=INVALID_HANDLE_VALUE;
	//m_hIsrEvent=INVALID_HANDLE_VALUE;
	//InitializeCriticalSection(&cs);

	InitializeCriticalSection(&csIFDIO);
	
}

/**********************************************************************************************//**
 * Finalizes an instance of the CInterfaceDIO class
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

CInterfaceDIO::~CInterfaceDIO(void)
{
	//StopDIOThread();

	if(m_hDIO!=INVALID_HANDLE_VALUE)
		CloseHandle(m_hDIO);

	DeleteCriticalSection(&csIFDIO);

	/*if(m_hIsrEvent!=INVALID_HANDLE_VALUE)
		CloseHandle(m_hIsrEvent);*/
}

/**********************************************************************************************//**
 * Gets the instance
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	Null if it fails, else the instance.
 **************************************************************************************************/

CInterfaceDIO* CInterfaceDIO::GetInstance()
{
	if(theDIOInterface == 0)
	{
		theDIOInterface = new CInterfaceDIO;
		//theDIOInterface->Init();
	}
	return theDIOInterface;
}

/**********************************************************************************************//**
 * Destroys the instance
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CInterfaceDIO::DestroyInstance()
{
	delete theDIOInterface;
	theDIOInterface = NULL;
}

/**********************************************************************************************//**
 * Initializes this instance
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

BOOL CInterfaceDIO::Init()
{
	m_hDIO=CreateFile(_T("DIO1:"),GENERIC_READ|GENERIC_WRITE,0,NULL, OPEN_EXISTING,0,NULL);

	if( m_hDIO == INVALID_HANDLE_VALUE )
		return false;
	return true;
}

/**********************************************************************************************//**
 * Reads a pin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param [in,out]	pbyData	If non-null, information describing the pby.
 * \param 		  	nPort  	The port.
 * \param 		  	chPin  	The pin.
 *
 * \return	The pin.
 **************************************************************************************************/

DWORD CInterfaceDIO::ReadPin (BYTE* pbyData,int nPort,BYTE chPin)
{
	DWORD dwReturn=1;

	EnterCriticalSection(&csIFDIO);

	//SetFilePointer(m_hDIO,nPort,NULL,FILE_BEGIN);

	if(!DeviceIoControl(m_hDIO, IOCTL_DIO_GET_PIN, &chPin, sizeof(BYTE), &pbyData, sizeof(BYTE),NULL, NULL )) 
	{
		ERRORMSG(1,(_T("DIO: Error while read pin data, error %d\n\r"), GetLastError())); 
		dwReturn=0;
	}

	LeaveCriticalSection(&csIFDIO);

	return dwReturn;
}

/**********************************************************************************************//**
 * Reads a pin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	nPort	The port.
 * \param	chPin	The pin.
 *
 * \return	The pin.
 **************************************************************************************************/

BYTE CInterfaceDIO::ReadPin (int nPort,BYTE chPin)
{
	BYTE pbyData=0;

	EnterCriticalSection(&csIFDIO);

	//SetFilePointer(m_hDIO,nPort,NULL,FILE_BEGIN);

	if(!DeviceIoControl(m_hDIO, IOCTL_DIO_GET_PIN, &chPin, sizeof(BYTE), &pbyData, sizeof(BYTE),NULL, NULL )) 
	{
		ERRORMSG(1,(_T("DIO: Error while read pin data, error %d\n\r"), GetLastError())); 
		//dwReturn=0;
	}

	LeaveCriticalSection(&csIFDIO);

	return pbyData;
}

/**********************************************************************************************//**
 * Writes a pin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	chPin	The pin.
 *
 * \return	A DWORD.
 **************************************************************************************************/

DWORD CInterfaceDIO::WritePin (BYTE chPin)
{
	DWORD dwReturn=1;

	EnterCriticalSection(&csIFDIO);

	//SetFilePointer(m_hDIO,nPort,NULL,FILE_BEGIN);

	// DeviceIoControl( hDIO, IOCTL_DIO_SET_PIN, 
	//                          &chPin, sizeof(BYTE), 
	//                          NULL, 0, NULL );

	if(!DeviceIoControl(m_hDIO, IOCTL_DIO_SET_PIN, &chPin, sizeof(BYTE), NULL, NULL,NULL, NULL )) 
		//if(!DeviceIoControl(m_hDIO, IOCTL_DIO_SET_PIN, &chPin, sizeof(BYTE), 0, sizeof(BYTE),NULL, NULL )) 
	{
		ERRORMSG(1,(_T("DIO: Error while write pin data, error %d\n\r"), GetLastError())); 
		dwReturn=0;
	}

	LeaveCriticalSection(&csIFDIO);

	return dwReturn;
}

/**********************************************************************************************//**
 * Clears the pin described by chPin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	chPin	The pin.
 *
 * \return	A DWORD.
 **************************************************************************************************/

DWORD CInterfaceDIO::ClearPin (BYTE chPin)
{
	DWORD dwReturn=1;

	EnterCriticalSection(&csIFDIO);

	//SetFilePointer(m_hDIO,nPort,NULL,FILE_BEGIN);

	// DeviceIoControl( hDIO, IOCTL_DIO_SET_PIN, 
	//                          &chPin, sizeof(BYTE), 
	//                          NULL, 0, NULL );

	if(!DeviceIoControl(m_hDIO, IOCTL_DIO_CLR_PIN, &chPin, sizeof(BYTE), NULL, NULL,NULL, NULL )) 
		//if(!DeviceIoControl(m_hDIO, IOCTL_DIO_SET_PIN, &chPin, sizeof(BYTE), 0, sizeof(BYTE),NULL, NULL )) 
	{
		ERRORMSG(1,(_T("DIO: Error while read pin data, error %d\n\r"), GetLastError())); 
		dwReturn=0;
	}

	LeaveCriticalSection(&csIFDIO);

	return dwReturn;
}

/**********************************************************************************************//**
 * Reads the given pby data
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param [in,out]	pbyData	If non-null, the pby data to read.
 *
 * \return	A DWORD.
 **************************************************************************************************/

DWORD CInterfaceDIO::Read (/*int nPort,*/BYTE* pbyData)
{	
	DWORD dwRead;
	DWORD dwReturn;
	int nPort=2;

	EnterCriticalSection(&csIFDIO);

	SetFilePointer(m_hDIO,nPort,NULL,FILE_BEGIN);
	dwReturn=ReadFile(m_hDIO,pbyData,1,&dwRead,NULL);

	LeaveCriticalSection(&csIFDIO);

	if(dwRead!=1)
		dwReturn=0;
	return dwReturn;
}

/**********************************************************************************************//**
 * Reads 3 bit
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param [in,out]	pbyData	If non-null, information describing the pby.
 *
 * \return	The 3 bit.
 **************************************************************************************************/

DWORD CInterfaceDIO::Read3Bit (/*int nPort,*/BYTE* pbyData)
{	
	DWORD dwRead;
	DWORD dwReturn;
	int nPort=1;

	EnterCriticalSection(&csIFDIO);

	SetFilePointer(m_hDIO,nPort,NULL,FILE_BEGIN);
	dwReturn=ReadFile(m_hDIO,pbyData,1,&dwRead,NULL);

	LeaveCriticalSection(&csIFDIO);

	*pbyData&=0x07;

	if(dwRead!=1)
		dwReturn=0;
	return dwReturn;
}

/**********************************************************************************************//**
 * Writes the given by data
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	byData	The by data to write.
 *
 * \return	A DWORD.
 **************************************************************************************************/

DWORD CInterfaceDIO::Write (/*int nPort,*/BYTE byData)
{	
	DWORD dwWritten;
	DWORD dwReturn;
	int nPort=0;

	EnterCriticalSection(&csIFDIO);

	SetFilePointer(m_hDIO,nPort,NULL,FILE_BEGIN);
	dwReturn=WriteFile(m_hDIO,&byData,1,&dwWritten,NULL);

	LeaveCriticalSection(&csIFDIO);

	if(dwWritten!=1)
		dwReturn=0;
	return dwReturn;
}

/**********************************************************************************************//**
 * Writes a 3 bit
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	bPin11	True to pin 11.
 * \param	bPin13	True to pin 13.
 * \param	bPin15	True to pin 15.
 *
 * \return	A DWORD.
 **************************************************************************************************/

DWORD CInterfaceDIO::Write3Bit (bool bPin11,bool bPin13,bool bPin15)
{
	BYTE byData;
	DWORD dwWritten;
	DWORD dwReturn;
	int nPort=1; // Bit 0,1,2

	byData=0x00;

	if(bPin11)
		byData|=0x01;
	if(bPin13)
		byData|=0x02;
	if(bPin15)
		byData|=0x04;

	EnterCriticalSection(&csIFDIO);

	SetFilePointer(m_hDIO,nPort,NULL,FILE_BEGIN);
	dwReturn=WriteFile(m_hDIO,&byData,1,&dwWritten,NULL);

	LeaveCriticalSection(&csIFDIO);

	if(dwWritten!=1)
		dwReturn=0;
	return dwReturn;
}
