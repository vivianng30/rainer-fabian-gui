#include "StdAfx.h"
#include "InterfaceCO2_MASIMO.h"
#include "MVModel.h"

CInterfaceCO2_MASIMO::CInterfaceCO2_MASIMO(void)
{
	m_iETCO2=0;
	m_iFICO2=0;
	m_iBPM=0;

	m_szVersion=_T("");
	m_bVersion=false;

	m_bPumpStateOn=true;
	m_bCO2ValueValid=false;
	m_bETCO2ValueValid=false;
	m_bFreqValuevalid=false;
	m_bFICO2ValueValid=false;
}

CInterfaceCO2_MASIMO::~CInterfaceCO2_MASIMO(void)
{
}

int CInterfaceCO2_MASIMO::Init(BYTE com)
{
	DEBUGMSG(TRUE, (TEXT("CInterfaceCO2_CAPNOSTAT::Init\r\n")));
	getModel()->setCO2inprogress(true);
	m_com=com;
	m_iETCO2=0;
	m_iFICO2=0;
	m_iBPM=0;

	//CSerialPort serial;
	LONG    lLastError = ERROR_SUCCESS;

	// Attempt to open the serial port (COM1)
	switch(m_com)
	{
	case 1:
		{
			theApp.WriteLog(_T("#CO2:open COM1"));
			lLastError = Open(_T("COM1:"),0,0);
		}
		break;
	case 3:
		{
			theApp.WriteLog(_T("#CO2:open COM3"));
			lLastError = Open(_T("COM3:"),0,0);
		}
		break;
	default:
		{
			//error
			return 1;
		}
		break;
	}


	if (lLastError != ERROR_SUCCESS)
		return ShowError(GetLastError(), _T("Unable to open COM-port"));

	// Setup the serial port (9600,8N1, which is the default setting)
	lLastError = Setup(CSerial::EBaud19200,CSerial::EData8,CSerial::EParNone,CSerial::EStop1);
	if (lLastError != ERROR_SUCCESS)
		return ShowError(GetLastError(), _T("Unable to set COM-port setting"));



	//startETCO2Thread();

	DEBUGMSG(TRUE, (TEXT("CInterfaceCO2_CAPNOSTAT::Init2\r\n")));

	return 0;
}
int CInterfaceCO2_MASIMO::Deinit()
{
	m_bStartup=false;

	Sleep(20);

	stopETCO2checkThread();
	stopETCO2Thread();

	m_iETCO2=0;
	m_iFICO2=0;
	m_iBPM=0;
	//m_iFICO2=0;

	if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff())
	{
		getModel()->getDATAHANDLER()->setMessureDataAVG(ALINK_MSMNT_FREQETCO2,0);
		getModel()->getDATAHANDLER()->setMessureDataBTB(ALINK_MSMNT_FREQETCO2,0);
	}
	getModel()->getDATAHANDLER()->setMessureDataAVG(ALINK_MSMNT_ETCO2,m_iETCO2);
	getModel()->getDATAHANDLER()->setMessureDataAVG(ALINK_MSMNT_FICO2,m_iFICO2);

	StopListener(-1);
	Close();

	if (getModel()->getAcuLink()==NULL)
	{
		getModel()->getAcuLink()->sendAllMeasurementData();
	}
	g_eventCO2Data.SetEvent();

	DEBUGMSG(TRUE, (TEXT("CInterfaceCO2_CAPNOSTAT::Deinit2\r\n")));

	//getModel()->setCO2inprogress(false);

	return 0;
}
void CInterfaceCO2_MASIMO::StartupSequenz()
{
	StartListener();
}
void CInterfaceCO2_MASIMO::InitializeSequenz()
{

}
bool CInterfaceCO2_MASIMO::PerformMsg()
{
	bool bResult = true;
	//todo
	return bResult;
}
void CInterfaceCO2_MASIMO::resetStateBytes()
{
	EnterCriticalSection(&csErrorDataCO2);
	m_byStateBytes=0xFF;
	m_byExtendedStateBytes=0xFF;
	LeaveCriticalSection(&csErrorDataCO2);
}
void CInterfaceCO2_MASIMO::setStateBytes(BYTE state)
{
	EnterCriticalSection(&csErrorDataCO2);
	m_byStateBytes=state;	
	LeaveCriticalSection(&csErrorDataCO2);
}
BYTE CInterfaceCO2_MASIMO::getStateBytes()
{
	EnterCriticalSection(&csErrorDataCO2);
	BYTE byTemp = m_byStateBytes;
	LeaveCriticalSection(&csErrorDataCO2);
	return byTemp;
}
bool CInterfaceCO2_MASIMO::isStateOk()
{
	return false;
}

void CInterfaceCO2_MASIMO::setExtendedStateBytes(BYTE state)
{
	EnterCriticalSection(&csErrorDataCO2);
	m_byExtendedStateBytes=state;
	LeaveCriticalSection(&csErrorDataCO2);
}
BYTE CInterfaceCO2_MASIMO::getExtendedStateBytes()
{
	EnterCriticalSection(&csErrorDataCO2);
	BYTE byTemp = m_byExtendedStateBytes;
	LeaveCriticalSection(&csErrorDataCO2);
	return byTemp;
}
bool CInterfaceCO2_MASIMO::isExtendedStateOk()
{
	bool bRes=true;
	return bRes;
}

