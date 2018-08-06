// InterfaceFSBus.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "InterfaceFSBus.h"
#include "MVModel.h"

CInterfaceFSBus* CInterfaceFSBus::theFSBusInterface=0;

/**********************************************************************************************//**
 * Initializes a new instance of the CInterfaceFSBus class
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

CInterfaceFSBus::CInterfaceFSBus(void)
{
	m_bDoPIFThread=false;

	m_hPIF = INVALID_HANDLE_VALUE;

	//m_iCnt=0;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CInterfaceFSBus class
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

CInterfaceFSBus::~CInterfaceFSBus(void)
{
	if( m_hPIF != INVALID_HANDLE_VALUE )
	{
		CloseHandle(m_hPIF);
	}
	m_hPIF = INVALID_HANDLE_VALUE;

}

/**********************************************************************************************//**
 * Gets the instance
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	Null if it fails, else the instance.
 **************************************************************************************************/

CInterfaceFSBus* CInterfaceFSBus::GetInstance()
{
	if(theFSBusInterface == 0)
	{
		theFSBusInterface = new CInterfaceFSBus;
		//theFSBusInterface->Init();
	}
	return theFSBusInterface;
}

/**********************************************************************************************//**
 * Destroys the instance
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CInterfaceFSBus::DestroyInstance()
{
	delete theFSBusInterface;
	theFSBusInterface = NULL;
}

/**********************************************************************************************//**
 * Initializes this instance
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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
}

/**********************************************************************************************//**
 * Sends a data
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param [in,out]	data	If non-null, the data.
 *
 * \return	A DWORD.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Sends a data
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param [in,out]	pData		  	If non-null, the data.
 * \param 		  	dwBytesToWrite	The bytes to write.
 *
 * \return	A DWORD.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Gets driver information
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The driver information.
 **************************************************************************************************/

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