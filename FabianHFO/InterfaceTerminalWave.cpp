// InterfaceTerminalWave.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "InterfaceTerminalWave.h"
#include "MVModel.h"
#include "MVViewHandler.h"

// CInterfaceTerminalWave

IMPLEMENT_DYNAMIC(CInterfaceTerminalWave, CWnd)

CInterfaceTerminalWave::CInterfaceTerminalWave():
CInterfaceTerminal()
{

}

CInterfaceTerminalWave::~CInterfaceTerminalWave()
{
}


//BEGIN_MESSAGE_MAP(CInterfaceTerminalWave, CInterfaceTerminal)
//END_MESSAGE_MAP()



// CInterfaceTerminalWave message handlers

bool CInterfaceTerminalWave::Init()
{
	StartTerminalThread();

	OpenCOM();

	Write("fabianHFO");

	setWaveDataActive();

	return 0;
}

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

void CInterfaceTerminalWave::OnEvent(EEvent eEvent, EError eError)
{
}