// InterfaceTerminalWave.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "InterfaceTerminalWave.h"
#include "MVModel.h"
#include "MVViewHandler.h"

/**********************************************************************************************//**
 * CInterfaceTerminalWave
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CInterfaceTerminalWave, CWnd)

/**********************************************************************************************//**
 * Initializes a new instance of the CInterfaceTerminalWave class
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

CInterfaceTerminalWave::CInterfaceTerminalWave():
CInterfaceTerminal()
{

}

/**********************************************************************************************//**
 * Finalizes an instance of the CInterfaceTerminalWave class
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

CInterfaceTerminalWave::~CInterfaceTerminalWave()
{
}


//BEGIN_MESSAGE_MAP(CInterfaceTerminalWave, CInterfaceTerminal)
//END_MESSAGE_MAP()

/**********************************************************************************************//**
 * CInterfaceTerminalWave message handlers
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CInterfaceTerminalWave::Init()
{
	StartTerminalThread();

	OpenCOM();

	Write("fabianHFO");

	setWaveDataActive();

	return 0;
}

/**********************************************************************************************//**
 * Opens the com
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CInterfaceTerminalWave::OpenCOM()
{
	bool bRes=false;

	DWORD dwIndex=0;
	LONG    lLastError =0;

	if(getModel()->getCONFIG()->GetMainBoardVersion()>=MAINBOARD_30)
		lLastError = Open(_T("COM2:"),0,0);
	else	//MAINBOARD2_1
		lLastError = Open(_T("COM1:"),0,0);

	if (lLastError != ERROR_SUCCESS)
	{
		if(ERROR_ALREADY_INITIALIZED==lLastError)
		{

		}
		return bRes;
	}

	// Setup the serial port (9600,8N1, which is the default setting)
	lLastError = Setup(CSerial::EBaud115200,CSerial::EData8,CSerial::EParNone,CSerial::EStop1);
	if (lLastError != ERROR_SUCCESS)
	{
		DEBUGMSG(TRUE, (TEXT("OpenCOM Unable to set COM-port setting\r\n")));
		//return ShowError(GetLastError(), _T("Unable to set COM-port setting"));
		return bRes;
	}
	else
	{
		bRes=true;
	}

	DEBUGMSG(TRUE, (TEXT("OpenCOM success\r\n")));

	if(bRes)
	{
		StartListener();
	}
	return bRes;

}

/**********************************************************************************************//**
 * Executes the event action
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	eEvent	The event.
 * \param	eError	The error.
 **************************************************************************************************/

void CInterfaceTerminalWave::OnEvent(EEvent eEvent, EError eError)
{
	//nothing to do here yet
}