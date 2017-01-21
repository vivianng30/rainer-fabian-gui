// InterfaceFSBus.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "InterfaceFSBus.h"
#include "MVModel.h"

CInterfaceFSBus* CInterfaceFSBus::theFSBusInterface=0;

CInterfaceFSBus::CInterfaceFSBus(void)
{
	m_bDoPIFThread=false;

	m_hPIF = INVALID_HANDLE_VALUE;

	//m_iCnt=0;
}

CInterfaceFSBus::~CInterfaceFSBus(void)
{
	if( m_hPIF != INVALID_HANDLE_VALUE )
	{
		CloseHandle(m_hPIF);
	}
	m_hPIF = INVALID_HANDLE_VALUE;

}

// **************************************************************************
// 
// **************************************************************************
CInterfaceFSBus* CInterfaceFSBus::GetInstance()
{
	if(theFSBusInterface == 0)
	{
		theFSBusInterface = new CInterfaceFSBus;
		//theFSBusInterface->Init();
	}
	return theFSBusInterface;
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceFSBus::DestroyInstance()
{
	delete theFSBusInterface;
	theFSBusInterface = NULL;
}

// **************************************************************************
// 
// **************************************************************************
BOOL CInterfaceFSBus::Init()
{
	m_hPIF = CreateFile( _T("PIF1:"),
		GENERIC_WRITE, 0,
		NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL );
	if( m_hPIF == INVALID_HANDLE_VALUE )
	{
		return(FALSE);
	}

	return true;

	//test fmea
	//return(FALSE);
}


// **************************************************************************
// 
// **************************************************************************
DWORD CInterfaceFSBus::SendData(unsigned char* data)
{
	if(m_hPIF==INVALID_HANDLE_VALUE)
		return false;

	//unsigned char data[3] = { 0x1, 0x2, 0x3 };
	DWORD dwBytesWrite = 1;
	WriteFile( m_hPIF, data, 3,
		&dwBytesWrite, NULL );
	if( dwBytesWrite != 3 )
	{
		// Error
		//getModel()->SetPIFavailability(FALSE);
	}

	return dwBytesWrite;
}

// **************************************************************************
// 
// **************************************************************************
DWORD CInterfaceFSBus::SendData(BYTE*  pData, DWORD dwBytesToWrite)
{
	if(m_hPIF==INVALID_HANDLE_VALUE)
		return false;

	//unsigned char data[3] = { 0x1, 0x2, 0x3 };
	DWORD dwBytesWritten = 0;
	WriteFile( m_hPIF, pData, dwBytesToWrite,
		&dwBytesWritten, NULL );
	/*if(m_iCnt==8)
		dwBytesWritten++;
	m_iCnt++;*/
	if( dwBytesWritten != dwBytesToWrite )
	{
		// Error
		//ERRORMSG(1,(_T("CInterfaceFSBus::SendData: dwBytesToWrite %d dwBytesWritten %d\n\r"),dwBytesToWrite,dwBytesWritten));
		//getModel()->SetPIFavailability(FALSE);
	}

	return dwBytesWritten;
}

// **************************************************************************
// 	Write address/data to FS-BUS
// **************************************************************************
//DWORD CInterfaceFSBus::SendPIFData(PARINTFRW* cData)
//{
//	if(m_hPIF==INVALID_HANDLE_VALUE)
//		return false;
//
//	DWORD dwBytesReturend;
//	//cData.chAddress = 0x20;
//	if( !DeviceIoControl( m_hPIF,
//		IOCTL_PARINTF_WRITE,
//		&cData, sizeof(cData), NULL, 0,
//		&dwBytesReturend, NULL ) )
//	{
//		DWORD dwLastError = GetLastError();
//	}
//
//	return dwBytesReturend;
//}

// **************************************************************************
// Read data from FS-BUS without Address
// **************************************************************************
//DWORD CInterfaceFSBus::ReadData(unsigned char* data)
//{
//	if(m_hPIF==INVALID_HANDLE_VALUE)
//		return false;
//
//	//unsigned char data[3] = { 0x1, 0x2, 0x3 };
//	DWORD dwRead;
//	ReadFile( m_hPIF, data, 3, &dwRead, NULL );
//	if( dwRead != 3 )
//	{
//		// Error
//	}
//
//	return dwRead;
//}

// **************************************************************************
// Read data from FS-BUS without Address
// **************************************************************************
//DWORD CInterfaceFSBus::ReadData(BYTE*  pData, DWORD dwBytesToRead)
//{
//	if(m_hPIF==INVALID_HANDLE_VALUE)
//		return false;
//
//	//unsigned char data[3] = { 0x1, 0x2, 0x3 };
//	DWORD dwRead;
//	ReadFile( m_hPIF, pData, dwBytesToRead, &dwRead, NULL );
//	if( dwRead != 3 )
//	{
//		// Error
//	}
//
//	return dwRead;
//}

// **************************************************************************
// Read 1 Byte from FS-BUS
// **************************************************************************
//DWORD CInterfaceFSBus::ReadPIFData(PARINTFRW* cData)
//{
//	if(m_hPIF==INVALID_HANDLE_VALUE)
//		return false;
//
//	DWORD dwBytesReturend;
//	//cData->chAddress = 0x20;
//	//cData->chData = 1;
//	if( !DeviceIoControl( m_hPIF,
//		IOCTL_PARINTF_READ,
//		&cData, sizeof(cData), NULL, 0,
//		&dwBytesReturend, NULL ) )
//	{
//		DWORD dwLastError = GetLastError();
//	}
//
//	return dwBytesReturend;
//}


// **************************************************************************
// treiberversionen auslesen
// **************************************************************************
CStringW CInterfaceFSBus::GetDriverInfo()
{
	CStringW szDrive;
	DRIVER_INFO cInfo;
	//if (!DeviceIoControl(hDevice, IOCTL_DRIVER_GETINFO, NULL, 0,
	//	&cInfo, sizeof(cInfo), NULL, NULL))
	//{
	//	cInfo.wVerMajor = 1; /* Command failed: this is V1.x */
	//	cInfo.wVerMinor = 0;
	//}
	//printf("NSPI driver V%d.%d", cInfo.wVerMajor, cInfo.wVerMinor);


	if(!DeviceIoControl(m_hPIF, IOCTL_DRIVER_GETINFO, NULL, 0,&cInfo, sizeof(cInfo), NULL, NULL))
	{
		cInfo.wVerMajor = 1; /* Command failed: this is V1.x */
		cInfo.wVerMinor = 0;
	}


	szDrive.Format(_T("V%d.%d"), cInfo.wVerMajor,cInfo.wVerMinor);

	return szDrive;

}