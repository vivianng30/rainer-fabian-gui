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

// EXTERN INTERRUPTS NetDCUX ///////////////////////////////////////////////////////////
#define SYSINTR_FIRMWARE    (8+8)
#define SYSINTR_PIFIRQ      (SYSINTR_FIRMWARE+15)
#define SYSINTR_EXTIO       (SYSINTR_FIRMWARE+16)

/////////////////////////////////////////////////////////////////////////////////////////
#define TRIGGER_EDGE	1	// NetDCU6 only (extern interrupts via dio port pins)
#define TRIGGER_LEVEL	2	// NetDCUX
#define TRIGGER TRIGGER_EDGE

CInterfaceDIO* CInterfaceDIO::theDIOInterface=0;


CInterfaceDIO::CInterfaceDIO(void)
{
	m_hDIO=INVALID_HANDLE_VALUE;
	//m_hIsrEvent=INVALID_HANDLE_VALUE;
	//InitializeCriticalSection(&cs);

	InitializeCriticalSection(&csIFDIO);
	
}

CInterfaceDIO::~CInterfaceDIO(void)
{
	//StopDIOThread();

	if(m_hDIO!=INVALID_HANDLE_VALUE)
		CloseHandle(m_hDIO);

	DeleteCriticalSection(&csIFDIO);

	/*if(m_hIsrEvent!=INVALID_HANDLE_VALUE)
		CloseHandle(m_hIsrEvent);*/
}

// **************************************************************************
// 
// **************************************************************************
CInterfaceDIO* CInterfaceDIO::GetInstance()
{
	if(theDIOInterface == 0)
	{
		theDIOInterface = new CInterfaceDIO;
		//theDIOInterface->Init();
	}
	return theDIOInterface;
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceDIO::DestroyInstance()
{
	delete theDIOInterface;
	theDIOInterface = NULL;
}

// **************************************************************************
// 
// **************************************************************************
BOOL CInterfaceDIO::Init()
{
	m_hDIO=CreateFile(_T("DIO1:"),GENERIC_READ|GENERIC_WRITE,0,NULL, OPEN_EXISTING,0,NULL);

	if( m_hDIO == INVALID_HANDLE_VALUE )
		return false;
	return true;
}

// **************************************************************************
// Read/write
// **************************************************************************
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

// **************************************************************************
// 
// **************************************************************************
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

// **************************************************************************
// 
// **************************************************************************
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

// **************************************************************************
// 
// **************************************************************************
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
