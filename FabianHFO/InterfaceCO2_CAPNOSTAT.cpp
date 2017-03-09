#include "StdAfx.h"
#include "InterfaceCO2_CAPNOSTAT.h"
#include "MVModel.h"

CInterfaceCO2_CAPNOSTAT::CInterfaceCO2_CAPNOSTAT()
{
	m_iETCO2=0;
	m_iFICO2=0;
	m_iBPM=0;
	//m_iFICO2=0;


	m_cLastCO2WaveVal=0;
	//faetCO2Interpolation[0]=0x0000;

	m_byStateBytes=0xFF;
	m_byExtendedStateBytes=0xFF;

	m_szVersion=_T("");
	m_bVersion=false;

	feRecvState = CO2_CAPNOSTAT_CMD;

	m_bPumpStateOn=true;
	m_bCO2ValueValid=false;
	m_bETCO2ValueValid=false;
	m_bFreqValuevalid=false;
	m_bFICO2ValueValid=false;

	m_iConfigBufferCnt=0;
	m_cfgPEAKPICK=10;			
	m_cfgCO2UNITS=(BYTE)CInterfaceCO2::getModel()->getCONFIG()->GetCO2unit();	
	m_cfgGasTemp=350;
	m_cfgNOBREATHDETECTED=20;
	m_cfgOPERATINGMODE=0;
	m_cfgZEROGASTYPE=1;
	if(CInterfaceCO2::getModel()->isO2FlushActive())
		m_cfgGasCompensation=CInterfaceCO2::getModel()->getDATAHANDLER()->PARADATA()->GetO2FlushPara();
	else
		m_cfgGasCompensation=CInterfaceCO2::getModel()->getDATAHANDLER()->PARADATA()->GetO2Para();

	m_byConfigBuffer[0]=CO2_CAPNOSTAT_ISB_GASTEMPERATURE;
	m_byConfigBuffer[1]=CO2_CAPNOSTAT_ISB_ETCO2_TIMEPERIOD;
	m_byConfigBuffer[2]=CO2_CAPNOSTAT_ISB_NOBREATHDETECTED_TIMEOUT;
	m_byConfigBuffer[3]=CO2_CAPNOSTAT_ISB_CURRENT_CO2UNITS;
	m_byConfigBuffer[4]=CO2_CAPNOSTAT_ISB_SLEEPMODE;
	m_byConfigBuffer[5]=CO2_CAPNOSTAT_ISB_ZEROGASTYPE;
	m_byConfigBuffer[6]=CO2_CAPNOSTAT_ISB_GAS_COMPENSATIONS;
	


	fwCalcChkSum=0;
	fwReadChkSum=0;

	faRecvBuf[0]=0x0000;
	faDataBuf[0]=0x0000;

	fdwRecvBufLen=0;
	fwDataCnt=0;

	fwDataSize=0;
	fwDataCode=0;
}

CInterfaceCO2_CAPNOSTAT::~CInterfaceCO2_CAPNOSTAT()
{
}

//=============================================================================
/**
 * @brief Get the instance of the model (singleton).
 *
 * @return the instance of the model
 *
 **/
//=============================================================================
//CMVModel *CInterfaceCO2_CAPNOSTAT::getModel()
//{
//	if(m_pModel==NULL)
//		m_pModel=CMVModel::GetInstance();
//	return m_pModel;
//}

//int CInterfaceCO2_CAPNOSTAT::ShowError (LONG lError, LPCTSTR lptszMessage)
//{
//	// Generate a message text
//	TCHAR tszMessage[256];
//	wsprintf(tszMessage,_T("%s\n(error code %d)"), lptszMessage, lError);
//
//	// Display message-box and return with an error-code
//	::MessageBox(0,tszMessage,_T("Listener"), MB_ICONSTOP|MB_OK);
//	return 1;
//}

int CInterfaceCO2_CAPNOSTAT::Init(BYTE com)
{
	DEBUGMSG(TRUE, (TEXT("CInterfaceCO2_CAPNOSTAT::Init\r\n")));
	getModel()->setCO2inprogress(true);
	m_com=com;
	m_iETCO2=0;
	m_iFICO2=0;
	m_iBPM=0;
	//m_iFICO2=0;

	m_byStateBytes=0xFF;
	m_byExtendedStateBytes=0xFF;

	//CSerialPort serial;
	LONG    lLastError = ERROR_SUCCESS;

	// Attempt to open the serial port (COM1)
	switch(m_com)
	{
	case 1:
		{
			theApp.getLog()->WriteLine(_T("#CO2:open COM1"));
			lLastError = Open(_T("COM1:"),0,0);
		}
		break;
	case 3:
		{
			theApp.getLog()->WriteLine(_T("#CO2:open COM3"));
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
	//if(getModel()->getCONFIG()->GetMainBoardVersion()>=MAINBOARD3_0)
	//	lLastError = Open(_T("COM3:"),0,0,true);
	//else	//MAINBOARD2_1
	//	lLastError = Open(_T("COM1:"),0,0,true);
	
	if (lLastError != ERROR_SUCCESS)
		return ShowError(GetLastError(), _T("Unable to open COM-port"));

	// Setup the serial port (9600,8N1, which is the default setting)
	lLastError = Setup(CSerial::EBaud19200,CSerial::EData8,CSerial::EParNone,CSerial::EStop1);
	if (lLastError != ERROR_SUCCESS)
		return ShowError(GetLastError(), _T("Unable to set COM-port setting"));

	// Setup handshaking (default is no handshaking)
	//lLastError = SetupHandshaking(CSerial::EHandshakeOff);
	/*if (lLastError != ERROR_SUCCESS)
		return ShowError(GetLastError(), _T("Unable to set COM-port handshaking"));*/

	// Register only for the receive event
	/*lLastError = SetMask(CSerial::EEventBreak |
		CSerial::EEventCTS   |
		CSerial::EEventDSR   |
		CSerial::EEventError |
		CSerial::EEventRing  |
		CSerial::EEventRLSD  |
		CSerial::EEventRcvEv);
	if (lLastError != ERROR_SUCCESS)
		return ShowError(GetLastError(), _T("Unable to set COM-port event mask"));*/

	//lLastError = SetEventChar('#', true);
	//lLastError = SetEventChar(CO2_CMD_STOPCONTINUOUS, true);

	/*if (lLastError != ERROR_SUCCESS)
		return ShowError(GetLastError(), _T("Unable to set event character"));*/

	/*StartListener();*/

	/*getModel()->getDATAHANDLER()->resetCO2MessureData();
	getModel()->getSPI()->ResetCO2Value();*/

	startETCO2Thread();

	DEBUGMSG(TRUE, (TEXT("CInterfaceCO2_CAPNOSTAT::Init2\r\n")));

	return 0;
}

//************************************
// Method:    Deinit
// FullName:  CInterfaceCO2_CAPNOSTAT::Deinit
// Access:    public 
// Returns:   int
// Qualifier:
//
// 2015/06/19: checked for correct closing of thread
//************************************
int CInterfaceCO2_CAPNOSTAT::Deinit()
{
	m_bStartup=false;
	
	stopETCO2checkThread();

	if(IsOpen())
		Send_StopContinousMode();
	Sleep(20);
	stopETCO2Thread();

	m_iETCO2=0;
	m_iFICO2=0;
	m_iBPM=0;
	//m_iFICO2=0;

	m_byStateBytes=0xFF;
	m_byExtendedStateBytes=0xFF;
	
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
		getModel()->getAcuLink()->setMeasurementData(ALINK_MSMNT_ETCO2,ALINK_NOTVALID);
		getModel()->getAcuLink()->setMeasurementData(ALINK_MSMNT_FREQETCO2,ALINK_NOTVALID);//NEWACULINK
		getModel()->getAcuLink()->setMeasurementData(ALINK_MSMNT_FICO2,ALINK_NOTVALID);//NEWACULINK
	}
	g_eventCO2Data.SetEvent();

	DEBUGMSG(TRUE, (TEXT("CInterfaceCO2_CAPNOSTAT::Deinit2\r\n")));

	//getModel()->setCO2inprogress(false);

	return 0;
}

void CInterfaceCO2_CAPNOSTAT::StartupSequenz()
{
	//DEBUGMSG(TRUE, (TEXT("CInterfaceCO2_CAPNOSTAT::StartupSequenz\r\n")));
	StartListener();

	getModuleVersion();
	Send_StopContinousMode();

	//DEBUGMSG(TRUE, (TEXT("CInterfaceCO2_CAPNOSTAT::StartupSequenz2\r\n")));
	//getModel()->setCO2inprogress(false);
}

void CInterfaceCO2_CAPNOSTAT::InitializeSequenz()
{
	
	DEBUGMSG(TRUE, (TEXT("InitializeSequenz1\r\n")));
	if (getModel()->getAcuLink()==NULL)
	{
		getModel()->getAcuLink()->setMeasurementData(ALINK_MSMNT_ETCO2,ALINK_NOTVALID);
		getModel()->getAcuLink()->setMeasurementData(ALINK_MSMNT_FREQETCO2,ALINK_NOTVALID);//NEWACULINK
		getModel()->getAcuLink()->setMeasurementData(ALINK_MSMNT_FICO2,ALINK_NOTVALID);//NEWACULINK
	}
	getModel()->getDATAHANDLER()->resetCO2MessureData();
	getModel()->getSPI()->ResetCO2Value();

	Send_BaroPressure(getModel()->getCONFIG()->GetCO2BaroPressure());
	
	if(getModel()->isO2FlushActive())
		m_cfgGasCompensation=getModel()->getDATAHANDLER()->PARADATA()->GetO2FlushPara();
	else
		m_cfgGasCompensation=getModel()->getDATAHANDLER()->PARADATA()->GetO2Para();
	Send_GasCompensation(m_cfgGasCompensation);

	Send_ETCO2TimePeriod(m_cfgPEAKPICK);
	m_cfgCO2UNITS=(BYTE)getModel()->getCONFIG()->GetCO2unit();	
	Send_CurrentCO2Units(m_cfgCO2UNITS);

	Send_ETCO2TimePeriod(m_cfgPEAKPICK);			
	BYTE byte2=LOBYTE(m_cfgGasTemp);
	BYTE byte1=HIBYTE(m_cfgGasTemp);
	SendCO2Command(CO2_CAPNOSTAT_CMD_GET_SET_SENSOR_SETTINGS,0x04,CO2_CAPNOSTAT_ISB_GASTEMPERATURE,byte1, byte2);
	SendCO2Command(CO2_CAPNOSTAT_CMD_GET_SET_SENSOR_SETTINGS,0x03,CO2_CAPNOSTAT_ISB_NOBREATHDETECTED_TIMEOUT,m_cfgNOBREATHDETECTED);
	if(m_cfgOPERATINGMODE==CO2_CAPNOSTAT_OPMODE_SLEEPMODE1)
	{
		set_StandbyMode(true);
	}
	else
	{
		set_StandbyMode(false);
	}
	SendCO2Command(CO2_CAPNOSTAT_CMD_GET_SET_SENSOR_SETTINGS,0x03,CO2_CAPNOSTAT_ISB_ZEROGASTYPE,m_cfgZEROGASTYPE);
	

	Send_CO2WaveformDataMode();

	if(m_bVersion==false)
		getModuleVersion();

	startETCO2checkThread();
	DEBUGMSG(TRUE, (TEXT("InitializeSequenz2\r\n")));

	//getModel()->setCO2inprogress(false);
}

DWORD CInterfaceCO2_CAPNOSTAT::SendCO2Command(BYTE CO2_command, BYTE Anzahlbytes, BYTE byte0, BYTE byte1, BYTE byte2, BYTE byte3, BYTE byte4) 
{
	//if(getModel()->getDATAHANDLER()->IsCO2interfaceEnabled())
	{
		LPCO2MSG msg = new CO2MSG;

		msg->byBuffer[0]=CO2_command;
		msg->byBuffer[1]=Anzahlbytes;
		msg->byBuffer[2]=byte0;
		msg->byBuffer[3]=byte1;
		msg->byBuffer[4]=byte2;
		msg->byBuffer[5]=byte3;
		msg->byBuffer[6]=byte4;
		AddChecksum(Anzahlbytes+1, msg->byBuffer);

		msg->bySize=Anzahlbytes+2;


		send(msg);
	}
	return 0;
}

void CInterfaceCO2_CAPNOSTAT::AddChecksum(short num, BYTE buf[])
{
	char checksum;
	short i;
	checksum = 0;
	for (i = 0; i < num; i++)
		checksum = (char) (checksum + buf[i]);
	checksum = (char) ((-checksum) & 0x7F);
	buf[num]=checksum;
	//return checksum;
}

BOOL CInterfaceCO2_CAPNOSTAT::IsChecksumValid(BYTE *lpBuf, short num)
{
	char checksum;
	checksum = *lpBuf;
	lpBuf++;
	num--;
	for (; num > 0; num--) {
		if (*lpBuf >= 0x80)
			return FALSE; // invalid data byte
		checksum = (char) (checksum + *lpBuf);
		lpBuf++;
	}
	checksum &= 0x7F;
	if (checksum == 0)
		return TRUE;
	else return FALSE; // invalid checksum
}

BYTE CInterfaceCO2_CAPNOSTAT::GetChecksum(BYTE Anzahlbytes, BYTE *buf)
{
	BYTE checksum;
	short i;
	checksum = 0;
	for (i = 0; i < Anzahlbytes; i++)
		checksum = (BYTE) (checksum + buf[i]);
	checksum = (BYTE) ((-checksum) & 0x7F);
	return checksum;
}


// **************************************************************************
// 
// **************************************************************************
void CInterfaceCO2_CAPNOSTAT::Send_StopContinousMode()
{
	SendCO2Command(CO2_CAPNOSTAT_CMD_STOPCONTINUOUS,0x01);
	getModel()->getDATAHANDLER()->resetCO2MessureData();
	getModel()->getSPI()->ResetCO2Value();
	m_iETCO2=0;
	m_iFICO2=0;
	m_iBPM=0;
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceCO2_CAPNOSTAT::Send_CO2WaveformDataMode()
{
	SendCO2Command(CO2_CAPNOSTAT_CMD_CO2_WAVEFORMDATAMODE,0x02,0x00);
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceCO2_CAPNOSTAT::Send_CapnostatZero()
{
	SendCO2Command(CO2_CAPNOSTAT_CMD_CAPNOSTAT_ZERO,0x01);
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceCO2_CAPNOSTAT::zeroCalibration()
{
	Send_CapnostatZero();
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceCO2_CAPNOSTAT::Read_BaroPressure()
{
	SendCO2Command(CO2_CAPNOSTAT_CMD_GET_SET_SENSOR_SETTINGS,0x02,CO2_CAPNOSTAT_ISB_BAROMETRIC_PRESSURE);
}
void CInterfaceCO2_CAPNOSTAT::Send_BaroPressure(SHORT val)
{
	BYTE byDB1=(val/128) & 0x7F;
	BYTE byDB2=val & 0x7F;
	SendCO2Command(CO2_CAPNOSTAT_CMD_GET_SET_SENSOR_SETTINGS,0x04,CO2_CAPNOSTAT_ISB_BAROMETRIC_PRESSURE,byDB1,byDB2);
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceCO2_CAPNOSTAT::Read_GasTemperature()
{
	SendCO2Command(CO2_CAPNOSTAT_CMD_GET_SET_SENSOR_SETTINGS,0x02,CO2_CAPNOSTAT_ISB_GASTEMPERATURE);
}
void CInterfaceCO2_CAPNOSTAT::Send_GasTemperature(SHORT val)
{
	BYTE byDB1=(val/128) & 0x7F;
	BYTE byDB2=val & 0x7F;
	SendCO2Command(CO2_CAPNOSTAT_CMD_GET_SET_SENSOR_SETTINGS,0x04,CO2_CAPNOSTAT_ISB_GASTEMPERATURE,byDB1,byDB2);
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceCO2_CAPNOSTAT::Read_ETCO2TimePeriod()
{
	SendCO2Command(CO2_CAPNOSTAT_CMD_GET_SET_SENSOR_SETTINGS,0x02,CO2_CAPNOSTAT_ISB_ETCO2_TIMEPERIOD);
}
void CInterfaceCO2_CAPNOSTAT::Send_ETCO2TimePeriod(BYTE val)
{
	SendCO2Command(CO2_CAPNOSTAT_CMD_GET_SET_SENSOR_SETTINGS,0x03,CO2_CAPNOSTAT_ISB_ETCO2_TIMEPERIOD,val);
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceCO2_CAPNOSTAT::Read_NoBreathDetectedTimeout()
{
	SendCO2Command(CO2_CAPNOSTAT_CMD_GET_SET_SENSOR_SETTINGS,0x02,CO2_CAPNOSTAT_ISB_NOBREATHDETECTED_TIMEOUT);
}
void CInterfaceCO2_CAPNOSTAT::Send_NoBreathDetectedTimeout(BYTE val)
{
	SendCO2Command(CO2_CAPNOSTAT_CMD_GET_SET_SENSOR_SETTINGS,0x03,CO2_CAPNOSTAT_ISB_NOBREATHDETECTED_TIMEOUT,val);
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceCO2_CAPNOSTAT::Read_CurrentCO2Units()
{
	SendCO2Command(CO2_CAPNOSTAT_CMD_GET_SET_SENSOR_SETTINGS,0x02,CO2_CAPNOSTAT_ISB_CURRENT_CO2UNITS);
}
void CInterfaceCO2_CAPNOSTAT::Send_CurrentCO2Units(BYTE val)
{
	SendCO2Command(CO2_CAPNOSTAT_CMD_GET_SET_SENSOR_SETTINGS,0x03,CO2_CAPNOSTAT_ISB_CURRENT_CO2UNITS,val);
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceCO2_CAPNOSTAT::Read_SleepMode()
{
	SendCO2Command(CO2_CAPNOSTAT_CMD_GET_SET_SENSOR_SETTINGS,0x02,CO2_CAPNOSTAT_ISB_SLEEPMODE);
}
void CInterfaceCO2_CAPNOSTAT::Send_SleepMode(BYTE val)
{
	SendCO2Command(CO2_CAPNOSTAT_CMD_GET_SET_SENSOR_SETTINGS,0x03,CO2_CAPNOSTAT_ISB_SLEEPMODE,val);
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceCO2_CAPNOSTAT::Read_ZeroGasType()
{
	SendCO2Command(CO2_CAPNOSTAT_CMD_GET_SET_SENSOR_SETTINGS,0x02,CO2_CAPNOSTAT_ISB_ZEROGASTYPE);
}
void CInterfaceCO2_CAPNOSTAT::Send_ZeroGasType(BYTE val)
{
	SendCO2Command(CO2_CAPNOSTAT_CMD_GET_SET_SENSOR_SETTINGS,0x03,CO2_CAPNOSTAT_ISB_ZEROGASTYPE,val);
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceCO2_CAPNOSTAT::Read_GasCompensation()
{
	SendCO2Command(CO2_CAPNOSTAT_CMD_GET_SET_SENSOR_SETTINGS,0x02,CO2_CAPNOSTAT_ISB_GAS_COMPENSATIONS);
}
void CInterfaceCO2_CAPNOSTAT::Send_GasCompensation(BYTE byO2Compensation, BYTE byBalanceGas, SHORT iAnestAgent)
{
	BYTE byDB3=(iAnestAgent/128) & 0x7F;
	BYTE byDB4=iAnestAgent & 0x7F;
	SendCO2Command(CO2_CAPNOSTAT_CMD_GET_SET_SENSOR_SETTINGS,0x06,CO2_CAPNOSTAT_ISB_GAS_COMPENSATIONS,byO2Compensation,byBalanceGas,byDB3,byDB4);
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceCO2_CAPNOSTAT::CheckModuleState()
{
	BYTE cmd=m_byConfigBuffer[m_iConfigBufferCnt];

	if(		cmd==CO2_CAPNOSTAT_ISB_GASTEMPERATURE
		||	cmd==CO2_CAPNOSTAT_ISB_ETCO2_TIMEPERIOD
		||	cmd==CO2_CAPNOSTAT_ISB_NOBREATHDETECTED_TIMEOUT
		||	cmd==CO2_CAPNOSTAT_ISB_CURRENT_CO2UNITS
		||	cmd==CO2_CAPNOSTAT_ISB_SLEEPMODE
		||	cmd==CO2_CAPNOSTAT_ISB_ZEROGASTYPE
		||	cmd==CO2_CAPNOSTAT_ISB_GAS_COMPENSATIONS)
	{
		SendCO2Command(CO2_CAPNOSTAT_CMD_GET_SET_SENSOR_SETTINGS,0x02,m_byConfigBuffer[m_iConfigBufferCnt]);
	}

	m_iConfigBufferCnt++;

	if(m_iConfigBufferCnt>=MAXCAPNOSTATMODULECONFIG)
		m_iConfigBufferCnt=0;

	CheckLastCO2Cmd();
}

// **************************************************************************
// 
// **************************************************************************
bool CInterfaceCO2_CAPNOSTAT::PerformMsg()
{
	bool bResult = true;
	CString szError=_T("");
	bool bNewData=false;

	switch(fwDataCode)
	{
		case CO2_CAPNOSTAT_CMD_CO2_WAVEFORMDATAMODE:
			{
				bool bValuevalid=true;
				//int iSYNCcnt=faDataBuf[0];
				//PBUFCO2 _pBuf;
				SHORT iVal=0;

				if(fwDataSize>4)//DPI data available
				{
					switch(faDataBuf[3])
					{
					case CO2_CAPNOSTAT_DPI_STATUS_ERRORS:
						{
							BYTE byNewState=faDataBuf[8];
							BYTE byOldState=getStateBytes();
							bool bMalfunktion=false;
							bool bCheckAirway=false;
							bool bCheckSampling=false;

							if(byOldState!=byNewState)
							{
								setStateBytes(byNewState);
								bNewData=true;

								switch(m_byStateBytes)
								{
								case 0xFF:
									{
										//invalid CO2 value
										bValuevalid=false;
									}
									break;
								case CO2_CAPNOSTAT_ERROR_PRIO_SENSOROVERTEMP:
									{
										//invalid CO2 value
										bValuevalid=false;
									}
									break;
								case CO2_CAPNOSTAT_ERROR_PRIO_SENSORFAULTY:
									{
										//invalid CO2 value
										bValuevalid=false;
										bMalfunktion=true;
									}
									break;
								case CO2_CAPNOSTAT_ERROR_PRIO_NOMESSAGE:
									{
										//invalid CO2 value
										bValuevalid=false;
										Send_BaroPressure(getModel()->getCONFIG()->GetCO2BaroPressure());

										if(getModel()->isO2FlushActive())
											m_cfgGasCompensation=getModel()->getDATAHANDLER()->PARADATA()->GetO2FlushPara();
										else
											m_cfgGasCompensation=getModel()->getDATAHANDLER()->PARADATA()->GetO2Para();

										Send_GasCompensation(m_cfgGasCompensation);
									}
									break;
								case CO2_CAPNOSTAT_ERROR_PRIO_SLEEPMODE:
									{
										//invalid CO2 value
										bValuevalid=false;
										
										if(m_bStandby==false)
										{
											m_bStandby=true;

										}
									}
									break;
								case CO2_CAPNOSTAT_ERROR_PRIO_ZEROINPROGRESS:
									{
										//invalid CO2 value
										bValuevalid=false;
									}
									break;
								case CO2_CAPNOSTAT_ERROR_PRIO_WARMUP:
									{
										//invalid CO2 value
										bValuevalid=false;
									}
									break;
								case CO2_CAPNOSTAT_ERROR_PRIO_ZEROREQUIRED:
									{
										//invalid CO2 value
										bValuevalid=false;
										bCheckAirway=true;
									}
									break;
								case CO2_CAPNOSTAT_ERROR_PRIO_CO2OUTOFRANGE:
									{
										//invalid CO2 value
										bValuevalid=false;
									}
									break;
								case CO2_CAPNOSTAT_ERROR_PRIO_CHECKAIRWAY:
									{
										//invalid CO2 value
										bValuevalid=false;
										bCheckAirway=true;
									}
									break;
								case CO2_CAPNOSTAT_ERROR_PRIO_CHECKSAMPLING:
									{
										//invalid CO2 value
										bValuevalid=false;
										bCheckSampling=true;
									}
									break;
								default:
									{
										
									}
									break;
								}

								if(bMalfunktion)
								{
									if(getModel()->getALARMHANDLER()->CanSetAlarm_CO2_SENSORFAULTY())
									//if(getModel()->getALARMHANDLER()->ALARM_Sens_CO2_SENSORFAULTY->getAlarmState()==AS_NONE)
									{
										m_bFICO2ValueValid=false;
										m_bETCO2ValueValid=false;
										if(AfxGetApp())
											AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_CO2_SENSORFAULTY);
										bNewData=true;
									}
								}
								else if(getModel()->getALARMHANDLER()->ALARM_Sens_CO2_SENSORFAULTY->getAlarmState()==AS_ACTIVE)
								{
									getModel()->getALARMHANDLER()->deleteAlarm(AL_Sens_CO2_SENSORFAULTY);
									bNewData=true;
								}

								if(bCheckAirway)
								{
									if(getModel()->getALARMHANDLER()->CanSetAlarm_CO2_CHECKAIRWAYADAPTER())
									//if(getModel()->getALARMHANDLER()->ALARM_Sens_CO2_CHECKAIRWAYADAPTER->getAlarmState()==AS_NONE)
									{
										m_bFICO2ValueValid=false;
										m_bETCO2ValueValid=false;
										if(AfxGetApp())
											AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_CO2_CHECKAIRWAYADAPTER);
										bNewData=true;
									}
								}
								else if(getModel()->getALARMHANDLER()->ALARM_Sens_CO2_CHECKAIRWAYADAPTER->getAlarmState()==AS_ACTIVE)
								{
									getModel()->getALARMHANDLER()->deleteAlarm(AL_Sens_CO2_CHECKAIRWAYADAPTER);
									bNewData=true;
								}

								if(bCheckSampling)
								{
									if(getModel()->getALARMHANDLER()->CanSetAlarm_CO2_CHECKSAMPLINGLINE())
									//if(getModel()->getALARMHANDLER()->ALARM_Sens_CO2_CHECKSAMPLINGLINE->getAlarmState()==AS_NONE)
									{
										m_bFICO2ValueValid=false;
										m_bETCO2ValueValid=false;
										if(AfxGetApp())
											AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_CO2_CHECKSAMPLINGLINE);
										bNewData=true;
									}
								}
								else if(getModel()->getALARMHANDLER()->ALARM_Sens_CO2_CHECKSAMPLINGLINE->getAlarmState()==AS_ACTIVE)
								{
									getModel()->getALARMHANDLER()->deleteAlarm(AL_Sens_CO2_CHECKSAMPLINGLINE);
									bNewData=true;
								}
								
							}
						}
						break;
					case CO2_CAPNOSTAT_DPI_ETCO2:
						{
							SHORT iValue=(faDataBuf[4]*128)+faDataBuf[5];
							if(m_iETCO2!=iValue)
							{
								if(iValue==0)
								{
									m_bETCO2ValueValid=false;

								}
								else
								{
									m_bETCO2ValueValid=true;
									
								}
								m_iETCO2=iValue;

								getModel()->getDATAHANDLER()->setMessureDataAVG(ALINK_MSMNT_ETCO2,m_iETCO2);
								bNewData=true;
							}
						}
						break;
					case CO2_CAPNOSTAT_DPI_RESP_RATE:
						{
							BYTE iBPM=(faDataBuf[4]*128)+faDataBuf[5];

							if(m_iBPM!=iBPM)
							{
								bNewData=true;

								if(iBPM==0)
								{
									m_bFreqValuevalid=false;
								}
								else
									m_bFreqValuevalid=true;

								m_iBPM=iBPM;

								getModel()->getDATAHANDLER()->setMessureDataAVG(ALINK_MSMNT_FREQETCO2,iBPM);
								getModel()->getDATAHANDLER()->setMessureDataBTB(ALINK_MSMNT_FREQETCO2,iBPM);

							}
							/*else if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false)
							{
								m_bFreqValuevalid=true;
							}*/
						}
						break;
					case CO2_CAPNOSTAT_DPI_INSPIRED_CO2:
						{
							SHORT iValue=(faDataBuf[4]*128)+faDataBuf[5];
							if(m_iFICO2!=iValue)
							{
								if(iValue==0)
								{
									m_bFICO2ValueValid=false;

								}
								else
								{
									m_bFICO2ValueValid=true;

								}
								m_iFICO2=iValue;

								getModel()->getDATAHANDLER()->setMessureDataAVG(ALINK_MSMNT_FICO2,m_iFICO2);
								bNewData=true;
							}
						}
						break;
					/*case CO2_CAPNOSTAT_DPI_BREATH_FLAG:
						break;
					case CO2_CAPNOSTAT_DPI_HARDWARESTATE:
						{
							BYTE byNewState=faDataBuf[8];
							
							
						}
						break;*/
					default:
						break;
					}
				}

				SHORT iTempfaWaveBuf=0;
				if(bValuevalid)
				{
					iVal=(SHORT)((128*faDataBuf[1])+faDataBuf[2])-1000;
					if(iVal==-10)
					{
						iVal=0;
						bValuevalid=false;
					}
					iTempfaWaveBuf=iVal;
				}
				
				if(m_bCO2ValueValid!=bValuevalid)
				{

					m_bCO2ValueValid=bValuevalid;
					bNewData=true;
				}

				if(m_cLastCO2WaveVal!=iTempfaWaveBuf)
				{
					doInterpolationWave(iTempfaWaveBuf);
				}

				/*PBUFCO2 _pBuf;
				_pBuf.iValCO2=iTempfaWaveBuf;
				getModel()->getDATAHANDLER()->writeCO2Buffer(_pBuf);*/

				if(bNewData)
					g_eventCO2Data.SetEvent();
			}
			break;
		case CO2_CAPNOSTAT_CMD_CAPNOSTAT_ZERO:
			{
				
			}
			break;
		case CO2_CAPNOSTAT_CMD_GET_SET_SENSOR_SETTINGS:
			{
				WORD sValue=0;

				if(fwDataSize>2)
				{
					switch(faDataBuf[0])
					{
					case CO2_CAPNOSTAT_ISB_GASTEMPERATURE:
						{
							sValue=MAKEWORD(faDataBuf[2],faDataBuf[1]);

							if(sValue!=m_cfgGasTemp)
							{
								BYTE byte2=LOBYTE(m_cfgGasTemp);
								BYTE byte1=HIBYTE(m_cfgGasTemp);

								szError.Format(_T("#HFO:0097: %d, slave: %d"),m_cfgGasTemp,sValue);
								theApp.getLog()->WriteLine(szError);

								SendCO2Command(CO2_CAPNOSTAT_CMD_GET_SET_SENSOR_SETTINGS,0x04,CO2_CAPNOSTAT_ISB_GASTEMPERATURE,byte1, byte2);
							}
						}
						break;
					case CO2_CAPNOSTAT_ISB_ETCO2_TIMEPERIOD:
						{
							//TRACE(_T("CO2_CAPNOSTAT_ISB_ETCO2_TIMEPERIOD \n"));
							sValue=faDataBuf[1];

							if(sValue!=m_cfgPEAKPICK)
							{
								szError.Format(_T("#HFO:0098: %d, slave: %d"),m_cfgPEAKPICK,sValue);
								theApp.getLog()->WriteLine(szError);

								Send_ETCO2TimePeriod(m_cfgPEAKPICK);
							}
						}
						break;
					case CO2_CAPNOSTAT_ISB_NOBREATHDETECTED_TIMEOUT:
						{
							//TRACE(_T("CO2_CAPNOSTAT_ISB_NOBREATHDETECTED_TIMEOUT \n"));
							sValue=faDataBuf[1];

							if(sValue!=m_cfgNOBREATHDETECTED)
							{
								szError.Format(_T("#HFO:0099: %d, slave: %d"),m_cfgNOBREATHDETECTED,sValue);
								theApp.getLog()->WriteLine(szError);

								SendCO2Command(CO2_CAPNOSTAT_CMD_GET_SET_SENSOR_SETTINGS,0x03,CO2_CAPNOSTAT_ISB_NOBREATHDETECTED_TIMEOUT,m_cfgNOBREATHDETECTED);
							}
						}
						break;
					case CO2_CAPNOSTAT_ISB_CURRENT_CO2UNITS:
						{
							//TRACE(_T("CO2_CAPNOSTAT_ISB_CURRENT_CO2UNITS \n"));
							sValue=faDataBuf[1];

							if(sValue!=m_cfgCO2UNITS)
							{
								szError.Format(_T("#HFO:0100: %d, slave: %d"),m_cfgCO2UNITS,sValue);
								theApp.getLog()->WriteLine(szError);

								Send_CurrentCO2Units(m_cfgCO2UNITS);
							}
						}
						break;
					case CO2_CAPNOSTAT_ISB_SLEEPMODE:
						{
							//TRACE(_T("CO2_CAPNOSTAT_ISB_SLEEPMODE \n"));
							sValue=faDataBuf[1];

							if(sValue!=m_cfgOPERATINGMODE)
							{
								szError.Format(_T("#HFO:0101: %d, slave: %d"),m_cfgOPERATINGMODE,sValue);
								theApp.getLog()->WriteLine(szError);

								//Send_SleepMode(m_cfgOPERATINGMODE);

								if(m_cfgOPERATINGMODE==CO2_CAPNOSTAT_OPMODE_SLEEPMODE1)
								{
									set_StandbyMode(true);
								}
								else
								{
									set_StandbyMode(false);
								}
							}
						}
						break;
					case CO2_CAPNOSTAT_ISB_ZEROGASTYPE:
						{
							//TRACE(_T("CO2_CAPNOSTAT_ISB_ZEROGASTYPE \n"));
							sValue=faDataBuf[1];

							if(sValue!=m_cfgZEROGASTYPE)
							{
								szError.Format(_T("#HFO:0102: %d, slave: %d"),m_cfgZEROGASTYPE,sValue);
								theApp.getLog()->WriteLine(szError);

								SendCO2Command(CO2_CAPNOSTAT_CMD_GET_SET_SENSOR_SETTINGS,0x03,CO2_CAPNOSTAT_ISB_ZEROGASTYPE,m_cfgZEROGASTYPE);
							}
						}
						break;
					case CO2_CAPNOSTAT_ISB_GAS_COMPENSATIONS:
						{
							//TRACE(_T("CO2_CAPNOSTAT_ISB_GAS_COMPENSATIONS \n"));
							sValue=faDataBuf[1];

							if(sValue!=m_cfgGasCompensation)
							{
								szError.Format(_T("#HFO:0103: %d, slave: %d"),m_cfgGasCompensation,sValue);
								theApp.getLog()->WriteLine(szError);

								if(getModel()->isO2FlushActive())
									m_cfgGasCompensation=getModel()->getDATAHANDLER()->PARADATA()->GetO2FlushPara();
								else
									m_cfgGasCompensation=getModel()->getDATAHANDLER()->PARADATA()->GetO2Para();

								Send_GasCompensation(m_cfgGasCompensation);
							}
						}
						break;
					}
				}
			}
			break;
		case CO2_CAPNOSTAT_CMD_CO2_O2_WAVEFORMMODE:
			{
				
			}
			break;
		case CO2_CAPNOSTAT_CMD_NACK_ERROR:
			{
				
			}
			break;
		case CO2_CAPNOSTAT_CMD_STOPCONTINUOUS:
			{
			}
			break;
		case CO2_CAPNOSTAT_CMD_GET_SOFTWARE_REVISION:
			{
				int iRF=faDataBuf[0];
				if(iRF==0 && fwDataSize>1)
				{
					CString szVersion(&faDataBuf[1]);
					m_szVersion=szVersion.Left(fwDataSize-2);
					
					m_bVersion=true;
				}
			}
			break;
		case CO2_CAPNOSTAT_CMD_SENSOR_CAPABILITIES:
			{
			}
			break;
		case CO2_CAPNOSTAT_CMD_RESET_NOBREATH_DETECTEDFLAG:
			{
			}
			break;
		case CO2_CAPNOSTAT_CMD_RESET_CAPNOSTAT:
			{
			}
			break;
		default:
			{
				bResult = false;
				
				
			}
			break;
	}

	
	
	return bResult;
}


// **************************************************************************
// 
// **************************************************************************
void CInterfaceCO2_CAPNOSTAT::doInterpolationWave(SHORT iTempfaWaveBuf)
{
	if(false==getModel()->getSPI()->isSPIrunning())
		return;
	
	SHORT iDiff=((iTempfaWaveBuf-m_cLastCO2WaveVal)/8);
	

	//DEBUGMSG(TRUE, (TEXT("%d %d\r\n"),iTempfaWaveBuf,iDiff));
	//DEBUGMSG(TRUE, (TEXT("wave %d\r\n"),iTempfaWaveBuf));
	//DEBUGMSG(TRUE, (TEXT("diff %d\r\n"),iDiff));


	EnterCriticalSection(&getModel()->getDATAHANDLER()->csCO2DataBuffer);
	for(int i=0;i<8;i++)
	{
		getModel()->getDATAHANDLER()->m_rbufCO2.write(m_cLastCO2WaveVal);
		m_cLastCO2WaveVal+=iDiff;
	}
	//test
	//getModel()->getDATAHANDLER()->m_rbufCO2.write(iValCO2);
	LeaveCriticalSection(&getModel()->getDATAHANDLER()->csCO2DataBuffer);
	
	m_cLastCO2WaveVal=iTempfaWaveBuf;

	//DEBUGMSG(TRUE, (TEXT("%d\r\n"),m_cLastCO2WaveVal));
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceCO2_CAPNOSTAT::OnEvent(EEvent eEvent, EError eError)
{
	/*int i;
	WORD lwValue;
	CString str;
	char buffer[64];
	static int idx = 0;*/

	WORD i;
	BYTE c;
	// Handle data receive event
	if (eEvent & CSerial::EEventRecv)
	{
		do
		{
			// Read data from the COM-port
			Read(faRecvBuf, sizeof(faRecvBuf)-1,&fdwRecvBufLen);
			for (i = 0; i < fdwRecvBufLen; i++)
			{
				c = faRecvBuf[i];
				switch (feRecvState)
				{
				case CO2_CAPNOSTAT_CMD:
					
					fwDataCnt=0;
					fwCalcChkSum = 0;
					fwReadChkSum=0;
					fwDataSize=0;
					fwDataCode=0;
					
					if(c>=0x80 && c<=0xFF)
					{
						fwDataCode = c;
						feRecvState = CO2_CAPNOSTAT_NBF;
						fwCalcChkSum = fwCalcChkSum+c;
					}
					else
					{
						feRecvState = CO2_CAPNOSTAT_CMD;
					}
					break;

				case CO2_CAPNOSTAT_NBF:
					
					if(c>=0x80 && c<=0xFF) //command byte
					{
						fwDataCnt=0;
						fwCalcChkSum = 0;
						fwReadChkSum=0;
						fwDataSize=0;
						
						fwDataCode = c;
						feRecvState = CO2_CAPNOSTAT_NBF;
						fwCalcChkSum = fwCalcChkSum+c;
					}
					else
					{
						fwDataSize = c;
						if(c>1)
							feRecvState = CO2_CAPNOSTAT_DATA;
						else
							feRecvState = CO2_CAPNOSTAT_CHKSUM;
						fwCalcChkSum = fwCalcChkSum+c;
					}
					
					break;

				case CO2_CAPNOSTAT_DATA:
					
					if(c>=0x80 && c<=0xFF) //command byte
					{
						fwDataCnt=0;
						fwCalcChkSum = 0;
						fwReadChkSum=0;
						fwDataSize=0;
						
						fwDataCode = c;
						feRecvState = CO2_CAPNOSTAT_NBF;
						fwCalcChkSum = fwCalcChkSum+c;
					}
					else
					{
						faDataBuf[fwDataCnt] = c;
						fwDataCnt++;
						feRecvState = CO2_CAPNOSTAT_DATA;
						fwCalcChkSum = fwCalcChkSum+c;

						if(fwDataCnt>=(fwDataSize-1))
							feRecvState = CO2_CAPNOSTAT_CHKSUM;
					}
					
					break;

				case CO2_CAPNOSTAT_CHKSUM:

					if(c>=0x80 && c<=0xFF) //command byte
					{
						fwDataCnt=0;
						fwCalcChkSum = 0;
						fwReadChkSum=0;
						fwDataSize=0;
						
						fwDataCode = c;
						feRecvState = CO2_CAPNOSTAT_NBF;
						fwCalcChkSum = fwCalcChkSum+c;
					}
					else
					{
						int iTest=fwDataSize;
						fwReadChkSum = c;
						fwCalcChkSum = ((-fwCalcChkSum) & 0x7F);
						/*fwCalcChkSum &= 0x7F;*/
						if(fwReadChkSum == fwCalcChkSum && fwDataSize>0)
						{
							if(PerformMsg())
							{
								if(m_bStartup==false)
								{
									m_bStartup=true;

								}
								UpdateLastCO2Cmd();
							}
						}
						else
						{
							//checksum error
						}
						feRecvState = CO2_CAPNOSTAT_CMD;
					}
					break;

				default:
					return;
				} // switch (feRecvState)
			} // for i

			
		}
		while (fdwRecvBufLen > 0);

		
	}

	return;
}


// **************************************************************************
// 
// **************************************************************************
bool CInterfaceCO2_CAPNOSTAT::ValidateMessage(BYTE* pData, size_t iLen)
{
	bool bResult=false;
	if(pData[0]>=0x80 && pData[0]<=0xFF)//commando starts
	{ 
		//SHORT iNBF=pData[1];
		if(iLen-2==pData[1])
		{
			if(IsChecksumValid(pData, iLen))
			{
				bResult=true;
			}
			else
			{
				bResult=false;
			}
		}
	}

	return bResult;
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceCO2_CAPNOSTAT::changeCO2Unit(eCO2unit unitNew,eCO2unit unitOld)
{
	m_cfgCO2UNITS=unitNew;

	Send_StopContinousMode();
	Send_CurrentCO2Units((BYTE) unitNew);
	
	int iAlimitETCO2max=getModel()->getALARMHANDLER()->getAlimitETCO2max();
	int iAlimitETCO2min=getModel()->getALARMHANDLER()->getAlimitETCO2min();

	iAlimitETCO2max=(int)(CTlsFloat::Round(((double)iAlimitETCO2max)/10, 0))*10;
	iAlimitETCO2min=(int)(CTlsFloat::Round(((double)iAlimitETCO2min)/10, 0))*10;

	switch(unitNew)
	{
	case CO2_UNIT_MMHG:
		{
			if(unitOld==CO2_UNIT_KPA) //kPa -> mmHg
			{
				iAlimitETCO2max=(int)CTlsFloat::Round(((double)iAlimitETCO2max*7.5), 0);
				iAlimitETCO2min=(int)CTlsFloat::Round(((double)iAlimitETCO2min*7.5), 0);
			}
			else if(unitOld==CO2_UNIT_PERCENT)	//Vol% -> mmHg
			{
				iAlimitETCO2max=(int)CTlsFloat::Round(((double)iAlimitETCO2max*getModel()->getCONFIG()->GetCO2BaroPressure())/100, 0);
				iAlimitETCO2min=(int)CTlsFloat::Round(((double)iAlimitETCO2min*getModel()->getCONFIG()->GetCO2BaroPressure())/100, 0);
			}
		}
		break;
	case CO2_UNIT_KPA:
		{
			if(unitOld==CO2_UNIT_MMHG)//mmHg -> kPa
			{
				iAlimitETCO2max=(int)CTlsFloat::Round(((double)iAlimitETCO2max/7.5), 0);
				iAlimitETCO2min=(int)CTlsFloat::Round(((double)iAlimitETCO2min/7.5), 0);
			}
			else if(unitOld==CO2_UNIT_PERCENT)//Vol% -> kPa
			{
				iAlimitETCO2max=(int)CTlsFloat::Round((((double)iAlimitETCO2max*100)/getModel()->getCONFIG()->GetCO2BaroPressure())*7.5, 0);
				iAlimitETCO2min=(int)CTlsFloat::Round((((double)iAlimitETCO2min*100)/getModel()->getCONFIG()->GetCO2BaroPressure())*7.5, 0);
			}
		}
		break;
	case CO2_UNIT_PERCENT:
		{
			if(unitOld==CO2_UNIT_KPA)//kPa -> Vol%
			{
				iAlimitETCO2max=(int)CTlsFloat::Round(((double)iAlimitETCO2max*750)/getModel()->getCONFIG()->GetCO2BaroPressure(), 0);
				iAlimitETCO2min=(int)CTlsFloat::Round(((double)iAlimitETCO2min*750)/getModel()->getCONFIG()->GetCO2BaroPressure(), 0);
			}
			else if(unitOld==CO2_UNIT_MMHG)//mmHg -> Vol%   OK
			{
				iAlimitETCO2max=(int)CTlsFloat::Round(((double)iAlimitETCO2max*100)/getModel()->getCONFIG()->GetCO2BaroPressure(), 0);
				iAlimitETCO2min=(int)CTlsFloat::Round(((double)iAlimitETCO2min*100)/getModel()->getCONFIG()->GetCO2BaroPressure(), 0);
			}
		}
		break;
	}
	iAlimitETCO2max=(int)(CTlsFloat::Round(((double)iAlimitETCO2max)/10, 0))*10;
	iAlimitETCO2min=(int)(CTlsFloat::Round(((double)iAlimitETCO2min)/10, 0))*10;
	getModel()->getALARMHANDLER()->setAlimitETCO2max(iAlimitETCO2max);
	getModel()->getALARMHANDLER()->setAlimitETCO2min(iAlimitETCO2min);


	getModel()->getI2C()->WriteConfigByte(CO2UNIT_8, (int)unitNew);

	Send_CO2WaveformDataMode();
	Read_CurrentCO2Units();

	if(getModel()->getAcuLink()!=NULL)
	{
		getModel()->getAcuLink()->setParaData(ALINK_SETT_UNIT_CO2,(int)unitNew);
	}
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceCO2_CAPNOSTAT::set_GasCompensation(BYTE byO2Compensation, BYTE byBalanceGas, SHORT iAnestAgent)
{
	Send_GasCompensation(byO2Compensation, byBalanceGas, iAnestAgent);
	
	if(getModel()->getAcuLink()!=NULL)
	{
		getModel()->getAcuLink()->setParaData(ALINK_SETT_O2COMPENSATION_CO2,(int)byO2Compensation);
	}
}
void CInterfaceCO2_CAPNOSTAT::get_GasCompensation()
{
	Read_GasCompensation();
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceCO2_CAPNOSTAT::set_BaroPressure(SHORT val)
{
	Send_BaroPressure(val);
	
}
void CInterfaceCO2_CAPNOSTAT::get_BaroPressure()
{

}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceCO2_CAPNOSTAT::resetStateBytes()
{
	EnterCriticalSection(&csErrorDataCO2);
	m_byStateBytes=0xFF;
	m_byExtendedStateBytes=0xFF;
	LeaveCriticalSection(&csErrorDataCO2);
}
void CInterfaceCO2_CAPNOSTAT::setStateBytes(BYTE state)
{
	EnterCriticalSection(&csErrorDataCO2);
	m_byStateBytes=state;

	switch(m_byStateBytes)
	{
	case CO2_CAPNOSTAT_ERROR_PRIO_SENSOROVERTEMP:
		{
			//HARDWARE ERROR
			theApp.writeLogError(GetLastError(), _T("CO2_CAPNOSTAT: CO2_CAPNOSTAT_ERROR_PRIO_SENSOROVERTEMP"));
		}
		break;
	case CO2_CAPNOSTAT_ERROR_PRIO_SENSORFAULTY:
		{
			//HARDWARE ERROR
			theApp.writeLogError(GetLastError(), _T("CO2_CAPNOSTAT: CO2_CAPNOSTAT_ERROR_PRIO_SENSORFAULTY"));
		}
		break;
	case CO2_CAPNOSTAT_ERROR_PRIO_NOMESSAGE:
		{
			Send_BaroPressure(getModel()->getCONFIG()->GetCO2BaroPressure());
			if(getModel()->isO2FlushActive())
				m_cfgGasCompensation=getModel()->getDATAHANDLER()->PARADATA()->GetO2FlushPara();
			else
				m_cfgGasCompensation=getModel()->getDATAHANDLER()->PARADATA()->GetO2Para();

			Send_GasCompensation(m_cfgGasCompensation);
		}
		break;
	case CO2_CAPNOSTAT_ERROR_PRIO_SLEEPMODE:
	case CO2_CAPNOSTAT_ERROR_PRIO_ZEROINPROGRESS:
	case CO2_CAPNOSTAT_ERROR_PRIO_WARMUP:
	case CO2_CAPNOSTAT_ERROR_PRIO_ZEROREQUIRED:
	case CO2_CAPNOSTAT_ERROR_PRIO_CO2OUTOFRANGE:
	case CO2_CAPNOSTAT_ERROR_PRIO_CHECKAIRWAY:
	case CO2_CAPNOSTAT_ERROR_PRIO_CHECKSAMPLING:
	default:
		{

		}
		break;
	}
	
	LeaveCriticalSection(&csErrorDataCO2);
}
BYTE CInterfaceCO2_CAPNOSTAT::getStateBytes()
{
	EnterCriticalSection(&csErrorDataCO2);
	BYTE byTemp = m_byStateBytes;
	LeaveCriticalSection(&csErrorDataCO2);
	return byTemp;
}
bool CInterfaceCO2_CAPNOSTAT::isStateOk()
{
	EnterCriticalSection(&csErrorDataCO2);
	BYTE state=m_byStateBytes;
	LeaveCriticalSection(&csErrorDataCO2);

	bool bRes=true;

	switch(state)
	{
	case 0xFF:
		{
			bRes=false;
		}
		break;
	case CO2_CAPNOSTAT_ERROR_PRIO_SENSOROVERTEMP:
	case CO2_CAPNOSTAT_ERROR_PRIO_SENSORFAULTY:
	case CO2_CAPNOSTAT_ERROR_PRIO_NOMESSAGE:
	case CO2_CAPNOSTAT_ERROR_PRIO_SLEEPMODE:
	case CO2_CAPNOSTAT_ERROR_PRIO_ZEROINPROGRESS:
	case CO2_CAPNOSTAT_ERROR_PRIO_WARMUP:
	case CO2_CAPNOSTAT_ERROR_PRIO_ZEROREQUIRED:
	case CO2_CAPNOSTAT_ERROR_PRIO_CO2OUTOFRANGE:
	case CO2_CAPNOSTAT_ERROR_PRIO_CHECKAIRWAY:
	case CO2_CAPNOSTAT_ERROR_PRIO_CHECKSAMPLING:
		{
			bRes=false;
		}
		break;
	default:
		{
			bRes=true;
		}
		break;
	}

	return bRes;
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceCO2_CAPNOSTAT::setExtendedStateBytes(BYTE state)
{
	/*EnterCriticalSection(&csErrorDataCO2);
	m_byExtendedStateBytes=state;

	switch(m_byExtendedStateBytes)
	{
	default:
		{

		}
		break;
	}

	LeaveCriticalSection(&csErrorDataCO2);*/
}
BYTE CInterfaceCO2_CAPNOSTAT::getExtendedStateBytes()
{
	EnterCriticalSection(&csErrorDataCO2);
	BYTE byTemp = m_byExtendedStateBytes;
	LeaveCriticalSection(&csErrorDataCO2);
	return byTemp;
}
bool CInterfaceCO2_CAPNOSTAT::isExtendedStateOk()
{
	EnterCriticalSection(&csErrorDataCO2);
	BYTE state=m_byExtendedStateBytes;
	LeaveCriticalSection(&csErrorDataCO2);

	bool bRes=true;

	//switch(state)
	//{
	///*case 0xFF:
	//case CO2_ERROR_PRIO_SENSOROVERTEMP:
	//case CO2_ERROR_PRIO_SENSORFAULTY:
	//case CO2_ERROR_PRIO_NOMESSAGE:
	//case CO2_ERROR_PRIO_SLEEPMODE:
	//case CO2_ERROR_PRIO_ZEROINPROGRESS:
	//case CO2_ERROR_PRIO_WARMUP:
	//case CO2_ERROR_PRIO_ZEROREQUIRED:
	//case CO2_ERROR_PRIO_CO2OUTOFRANGE:
	//case CO2_ERROR_PRIO_CHECKAIRWAY:
	//case CO2_ERROR_PRIO_CHECKSAMPLING:
	//	{
	//		bRes=false;
	//	}
	//	break;*/
	//default:
	//	{
	//		bRes=true;
	//	}
	//	break;
	//}

	return bRes;
}

// **************************************************************************
// 
// **************************************************************************
bool CInterfaceCO2_CAPNOSTAT::isPumpOn()
{
	return m_bPumpStateOn;
}
// **************************************************************************
// 
// **************************************************************************
bool CInterfaceCO2_CAPNOSTAT::isCO2ValueValid()
{
	return m_bCO2ValueValid;
}

bool CInterfaceCO2_CAPNOSTAT::isETCO2ValueValid()
{
	return m_bETCO2ValueValid;
}
bool CInterfaceCO2_CAPNOSTAT::isFreqValueValid()
{
	return m_bFreqValuevalid;
}
bool CInterfaceCO2_CAPNOSTAT::isFICO2ValueValid()
{
	return m_bFICO2ValueValid;
}
// **************************************************************************
// 
// **************************************************************************
void CInterfaceCO2_CAPNOSTAT::set_Configuration()
{
	Send_StopContinousMode();

	getModel()->getDATAHANDLER()->resetCO2MessureData();
	getModel()->getSPI()->ResetCO2Value();

	Send_BaroPressure(getModel()->getCONFIG()->GetCO2BaroPressure());

	if(getModel()->isO2FlushActive())
		m_cfgGasCompensation=getModel()->getDATAHANDLER()->PARADATA()->GetO2FlushPara();
	else
		m_cfgGasCompensation=getModel()->getDATAHANDLER()->PARADATA()->GetO2Para();
	Send_GasCompensation(m_cfgGasCompensation);

	Send_ETCO2TimePeriod(m_cfgPEAKPICK);
	m_cfgCO2UNITS=(BYTE)getModel()->getCONFIG()->GetCO2unit();	
	Send_CurrentCO2Units(m_cfgCO2UNITS);

	Send_ETCO2TimePeriod(m_cfgPEAKPICK);			
	BYTE byte2=LOBYTE(m_cfgGasTemp);
	BYTE byte1=HIBYTE(m_cfgGasTemp);
	SendCO2Command(CO2_CAPNOSTAT_CMD_GET_SET_SENSOR_SETTINGS,0x04,CO2_CAPNOSTAT_ISB_GASTEMPERATURE,byte1, byte2);
	SendCO2Command(CO2_CAPNOSTAT_CMD_GET_SET_SENSOR_SETTINGS,0x03,CO2_CAPNOSTAT_ISB_NOBREATHDETECTED_TIMEOUT,m_cfgNOBREATHDETECTED);
	if(m_cfgOPERATINGMODE==CO2_CAPNOSTAT_OPMODE_SLEEPMODE1)
	{
		set_StandbyMode(true);
	}
	else
	{
		set_StandbyMode(false);
	}
	SendCO2Command(CO2_CAPNOSTAT_CMD_GET_SET_SENSOR_SETTINGS,0x03,CO2_CAPNOSTAT_ISB_ZEROGASTYPE,m_cfgZEROGASTYPE);


	Send_CO2WaveformDataMode();

	if(m_bVersion==false)
		getModuleVersion();
}

void CInterfaceCO2_CAPNOSTAT::set_StandbyMode(bool state)
{
	bool bChanged=false;
	if(m_bStandby!=state)
		bChanged=true;
	m_bStandby=state;
	if(state)
	{
		m_cfgOPERATINGMODE=CO2_CAPNOSTAT_OPMODE_SLEEPMODE1;
		set_restartBreathAlgorithm();
	}
	else
		m_cfgOPERATINGMODE=CO2_CAPNOSTAT_OPMODE_NORMAL;

	Send_SleepMode(m_cfgOPERATINGMODE);

	if(bChanged)
		g_eventCO2Data.SetEvent();
}
bool CInterfaceCO2_CAPNOSTAT::get_StandbyMode()
{
	return m_bStandby;
}

//CString CInterfaceCO2_CAPNOSTAT::get_ModuleVersion()
//{
//	return m_szVersion;
//}

void CInterfaceCO2_CAPNOSTAT::getModuleVersion()
{
	SendCO2Command(CO2_CAPNOSTAT_CMD_GET_SOFTWARE_REVISION,0x02,0x00);
}

//void CInterfaceCO2_CAPNOSTAT::retryConnection()
//{
//	DEBUGMSG(TRUE, (TEXT("CInterfaceCO2_CAPNOSTAT::retryConnection()\r\n")));
//	Deinit();
//	Init(m_com);
//	//getModuleVersion();
//}


void CInterfaceCO2_CAPNOSTAT::set_restartBreathAlgorithm()
{
	//SendCO2Command(xxx,0x02,0x00);
}

//void CInterfaceCO2_CAPNOSTAT::startWriteBuffer()
//{
//	m_bStartWriteBuffer=true;
//}