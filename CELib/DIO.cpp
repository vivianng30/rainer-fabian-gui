// CDIO.cpp : implementation file
/***************************************************************************************/
/***************************************************************************************/
/**                                                                                   **/
/**    F&S Elektronik Systeme GmbH                                                    **/
/**                                                                                   **/
/**    Project:      intern                                                           **/
/**    Filename:     DIO.cpp                                                          **/
/**    Author:       Christian Zutter                                                 **/
/**    Release:      0.0                                                              **/
/**                                                                                   **/
/**    History                                                                        **/
/**    Date          Author              Modification                                 **/
/**    19.12.05      C. Zutter           first edition                                **/
/**								                                                      **/
/**                                                                                   **/
/**                                                                                   **/
/***************************************************************************************/
/***************************************************************************************/
#include "stdafx.h"
/*- files at project folder -----------------------------------------------------------*/
#include "DIO.h"
#include "dio_sdk.h"

/*- Preprocessor directives and compiler switches -------------------------------------*/
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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


/////////////////////////////////////////////////////////////////////////////////////////
// CDIO
// **************************************************************************
// 
// **************************************************************************
CDIO::CDIO()
{
	InitializeCriticalSection(&csDIO);

	m_hDIO=INVALID_HANDLE_VALUE;
	m_hIsrEvent=INVALID_HANDLE_VALUE;
	m_dwSysIntr=0;
}
	
bool CDIO::Init(void)
{
	m_hDIO=CreateFile(_T("DIO1:"),GENERIC_READ|GENERIC_WRITE,0,NULL, OPEN_EXISTING,0,NULL);

	if( m_hDIO == INVALID_HANDLE_VALUE )
		return false;
	return true;
}

// **************************************************************************
// 
// **************************************************************************
CDIO::~CDIO()
{
	if(m_hDIO!=INVALID_HANDLE_VALUE)
		CloseHandle(m_hDIO);

	

	if(m_hIsrEvent!=INVALID_HANDLE_VALUE)
		CloseHandle(m_hIsrEvent);

	DeleteCriticalSection(&csDIO);
}

// **************************************************************************
// 
// **************************************************************************
DWORD CDIO::ReadPin (BYTE* pbyData, int, BYTE chPin)
{
	DWORD dwReturn=1;

	EnterCriticalSection(&csDIO);

	//SetFilePointer(m_hDIO,nPort,NULL,FILE_BEGIN);

	if(!DeviceIoControl(m_hDIO, IOCTL_DIO_GET_PIN, &chPin, sizeof(BYTE), &pbyData, sizeof(BYTE),NULL, NULL )) 
	{
		ERRORMSG(1,(_T("DIO: Error while read pin data, error %d\n\r"), GetLastError())); 
		dwReturn=0;
	}

	LeaveCriticalSection(&csDIO);

	return dwReturn;
}

BYTE CDIO::ReadPin (int,BYTE chPin)
{
	BYTE pbyData=0;

	EnterCriticalSection(&csDIO);

	//SetFilePointer(m_hDIO,nPort,NULL,FILE_BEGIN);

	if(!DeviceIoControl(m_hDIO, IOCTL_DIO_GET_PIN, &chPin, sizeof(BYTE), &pbyData, sizeof(BYTE),NULL, NULL )) 
	{
		ERRORMSG(1,(_T("DIO: Error while read pin data, error %d\n\r"), GetLastError())); 
		//dwReturn=0;
	}

	LeaveCriticalSection(&csDIO);

	return pbyData;
}

// **************************************************************************
// 
// **************************************************************************
DWORD CDIO::WritePin (BYTE chPin)
{
	DWORD dwReturn=1;

	EnterCriticalSection(&csDIO);

	//SetFilePointer(m_hDIO,nPort,NULL,FILE_BEGIN);

	// DeviceIoControl( hDIO, IOCTL_DIO_SET_PIN, 
		//                          &chPin, sizeof(BYTE), 
		//                          NULL, 0, NULL );

	if(!DeviceIoControl(m_hDIO, IOCTL_DIO_SET_PIN, &chPin, sizeof(BYTE), NULL, NULL,NULL, NULL )) 
	//if(!DeviceIoControl(m_hDIO, IOCTL_DIO_SET_PIN, &chPin, sizeof(BYTE), 0, sizeof(BYTE),NULL, NULL )) 
	{
		ERRORMSG(1,(_T("DIO: Error while read pin data, error %d\n\r"), GetLastError())); 
		dwReturn=0;
	}

	LeaveCriticalSection(&csDIO);

	return dwReturn;
}

// **************************************************************************
// 
// **************************************************************************
DWORD CDIO::ClearPin (BYTE chPin)
{
	DWORD dwReturn=1;

	EnterCriticalSection(&csDIO);

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

	LeaveCriticalSection(&csDIO);

	return dwReturn;
}

// **************************************************************************
// 
// **************************************************************************
DWORD CDIO::Read (/*int nPort,*/BYTE* pbyData)
{	
	DWORD dwRead;
	DWORD dwReturn;
	int nPort=2;

	EnterCriticalSection(&csDIO);

	SetFilePointer(m_hDIO,nPort,NULL,FILE_BEGIN);
	dwReturn=ReadFile(m_hDIO,pbyData,1,&dwRead,NULL);

	LeaveCriticalSection(&csDIO);

	if(dwRead!=1)
		dwReturn=0;
	return dwReturn;
}

DWORD CDIO::Read3Bit (/*int nPort,*/BYTE* pbyData)
{	
	DWORD dwRead;
	DWORD dwReturn;
	int nPort=1;

	EnterCriticalSection(&csDIO);

	SetFilePointer(m_hDIO,nPort,NULL,FILE_BEGIN);
	dwReturn=ReadFile(m_hDIO,pbyData,1,&dwRead,NULL);

	LeaveCriticalSection(&csDIO);

	*pbyData&=0x07;

	if(dwRead!=1)
		dwReturn=0;
	return dwReturn;
}

/***************************************************************************************/
//---------------------------------------------------------------------------------------
DWORD CDIO::Write (/*int nPort,*/BYTE byData)
{	
	DWORD dwWritten;
	DWORD dwReturn;
	int nPort=0;

	EnterCriticalSection(&csDIO);

	SetFilePointer(m_hDIO,nPort,NULL,FILE_BEGIN);
	dwReturn=WriteFile(m_hDIO,&byData,1,&dwWritten,NULL);

	LeaveCriticalSection(&csDIO);

	if(dwWritten!=1)
		dwReturn=0;
	return dwReturn;
}

DWORD CDIO::Write3Bit (bool bPin11,bool bPin13,bool bPin15)
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

	EnterCriticalSection(&csDIO);

	SetFilePointer(m_hDIO,nPort,NULL,FILE_BEGIN);
	dwReturn=WriteFile(m_hDIO,&byData,1,&dwWritten,NULL);

	LeaveCriticalSection(&csDIO);

	if(dwWritten!=1)
		dwReturn=0;
	return dwReturn;
}
// **************************************************************************
// 
// **************************************************************************
bool CDIO::CreateISR(void)
{
	m_hIsrEvent=CreateEvent( NULL, FALSE, FALSE, _T("ISR") );

	DWORD dwIRQ = 11; // Port 1 pin 10
	m_dwSysIntr=SYSINTR_EXTIO;

#if(TRIGGER==TRIGGER_EDGE)
	if(!DeviceIoControl(m_hDIO, IOCTL_DIO_REQUEST_SYSINTR, &dwIRQ, sizeof(DWORD), &m_dwSysIntr, sizeof(DWORD), NULL, NULL ) )
		return false;
#endif
	if(!InterruptInitialize(m_dwSysIntr,m_hIsrEvent,NULL,0))
		return false;
	return true;
}

bool CDIO::WaitISR(void)
{
	DWORD dwReturn=WaitForSingleObject(m_hIsrEvent,INFINITE);
#if(TRIGGER==TRIGGER_LEVEL)
	Sleep(100); // TODO
#endif
	if(dwReturn==WAIT_OBJECT_0)
		return true;
	return false;
}

void CDIO::ISRDone(void)
{
	InterruptDone(m_dwSysIntr);
}

bool CDIO::EndISR(void)
{
	DeviceIoControl( m_hDIO, IOCTL_DIO_RELEASE_SYSINTR, &m_dwSysIntr, sizeof(DWORD), NULL, 0, NULL, NULL );

	if(m_hIsrEvent!=INVALID_HANDLE_VALUE)
		CloseHandle(m_hIsrEvent);
	return true;
}

bool CDIO::SetISR(void)
{
	if(SetEvent(m_hIsrEvent))
		return true;
	return false;
}
	
