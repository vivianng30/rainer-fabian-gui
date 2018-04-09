/**********************************************************************************************//**
 * \file	InterfaceCO2_MICROPOD.cpp.
 *
 * Implements the interface co 2 micropod class
 **************************************************************************************************/

#include "StdAfx.h"
#include "InterfaceCO2_MICROPOD.h"
#include "MVModel.h"

/**********************************************************************************************//**
 * Initializes a new instance of the CInterfaceCO2_MICROPOD class
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

CInterfaceCO2_MICROPOD::CInterfaceCO2_MICROPOD(void)
{
	m_cLastCO2WaveVal=0;
	InitializeCriticalSection(&csLastCalCheck);

	m_iBPM=0;
	m_iETCO2=0;
	m_iFICO2=0;
	
	m_iAmbientPressure=0;
	m_iMaxOpHRSCalibration=-1;
	m_iRemainServiceHrs=-1;
	m_bCalibrationIsDue=false;
	m_bServiceIsDue=false;
	m_bSystemDateOk=true;

	fdwRecvBufLen = 0;
	feRecvState = CO2_MICROPOD_HEADER;
	fwCalcChkSum = 0;
	fwDataSize = 0;
	fwReadChkSum=0;
	m_byCmdBuffer[0]=0x0000;
	m_byLength=0;
	m_dwLastCalCheck=0;

	m_szVersion=_T("");
	m_bVersion=false;

	m_byStateBytes=0xFF;
	m_byExtendedStateBytes=0xFF;

	m_bPumpStateOn=true;
	m_bCO2ValueValid=false;
	m_bETCO2ValueValid=false;
	m_bFreqValuevalid=false;
	m_bFICO2ValueValid=false;



	m_byConfigBuffer[0]=CO2_MICROPOD_MODULECONFIG_BTPS;
	m_byConfigBuffer[1]=CO2_MICROPOD_MODULECONFIG_PEAKPICK;
	m_byConfigBuffer[2]=CO2_MICROPOD_MODULECONFIG_DISTINGBREATHMSG;
	m_byConfigBuffer[3]=CO2_MICROPOD_MODULECONFIG_CO2UNITS;
	m_byConfigBuffer[4]=CO2_MICROPOD_MODULECONFIG_PERIODICBREATHMSG;
	m_byConfigBuffer[5]=CO2_MICROPOD_MODULECONFIG_WAVEMEASINTERVAL;
	m_byConfigBuffer[6]=CO2_MICROPOD_MODULECONFIG_AUTOSTANDBY;
	m_byConfigBuffer[7]=CO2_MICROPOD_MODULECONFIG_PATIENTMODE;
	m_byConfigBuffer[8]=CO2_MICROPOD_MODULECONFIG_CO2RANGE;
	m_byConfigBuffer[9]=CO2_MICROPOD_MODULECONFIG_MEASUREMODE;
	m_byConfigBuffer[10]=CO2_MICROPOD_MODULECONFIG_PERIODICMESSAGE_CONTCO2NUM;
	m_byConfigBuffer[11]=CO2_MICROPOD_MODULECONFIG_PERIODICMESSAGE_CONTPERCSPONT;
	m_byConfigBuffer[12]=CO2_MICROPOD_MODULECONFIG_GETOPMODE;
	m_byConfigBuffer[13]=CO2_MICROPOD_MODULECONFIG_GETAMBIENTPRESSURE;


	m_iConfigBufferCnt=0;

	m_cfgBTPS=2;				//BTPS
	m_cfgPEAKPICK=3;			//20sec
	m_cfgDISTINGBREATHMSG=0;	//disabled
	m_cfgCO2UNITS=1;			//mmHg
	m_cfgPERIODICBREATHMSG=1;	//1 second
	m_cfgWAVEMEASINTERVAL=2;	//50ms
	//m_cfgWAVEMEASINTERVAL=1;	//25ms
	m_cfgAUTOSTANDBY=0;			//disabled
	m_cfgPATIENTMODE=2;			//NEONATAL
	m_cfgCO2RANGE=2;			//0-150mmHg
	m_cfgMEASUREMODE=1;			//normal
	m_cfgCONTCO2NUM=0;			//disabled
	m_cfgCONTPERCSPONT=0;		//disabled
	m_cfgOPERATINGMODE=CO2_MICROPOD_OPMODE_MEASUREMENT;

	//dwCmdBytesRead = 0;
	m_countCmdBuf=0;
	m_iCmdLength=0;
	m_bCmdHeader=false;
	m_checksumCalc=0;
	m_crcReadIn=0;

	m_iCmdIdx=0;

	byBreak=0;

	m_dtCalibrationDateTime.SetStatus(COleDateTime::null);

	faRecvBuf[0]=0x0000;
	faDataBuf[0]=0x0000;

	fwDataCnt=0;
	fwDataCode=0;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CInterfaceCO2_MICROPOD class
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

CInterfaceCO2_MICROPOD::~CInterfaceCO2_MICROPOD(void)
{
	DeleteCriticalSection(&csLastCalCheck);
}

/**********************************************************************************************//**
 * Turn off
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CInterfaceCO2_MICROPOD::turnOff()
{
	stopETCO2checkThread();
	stopETCO2Thread();
	
	StopListener(-1);
	Close();
}

/**********************************************************************************************//**
 * Initializes this instance
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	com	The com.
 *
 * \return	An int.
 **************************************************************************************************/

int CInterfaceCO2_MICROPOD::Init(BYTE com)
{
	DEBUGMSG(TRUE, (TEXT("CInterfaceCO2_MICROPOD::Init\r\n")));
	getModel()->setCO2inprogress(true);
	m_iAmbientPressure=getModel()->getCONFIG()->GetCO2BaroPressure();
	m_com=com;
	m_iBPM=0;
	m_iETCO2=0;
	m_iFICO2=0;

	m_byStateBytes=0xFF;
	m_byExtendedStateBytes=0xFF;

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
	//if(getModel()->getCONFIG()->GetMainBoardVersion()>=MAINBOARD3_0)
	//	lLastError = Open(_T("COM3:"),0,0,true);
	//else	//MAINBOARD2_1
	//	lLastError = Open(_T("COM1:"),0,0,true);
	
	if (lLastError != ERROR_SUCCESS)
		return ShowError(GetLastError(), _T("Unable to open COM-port"));

	// Setup the serial port (9600,8N1, which is the default setting)
	//lLastError = Setup(CSerial::EBaud19200,CSerial::EData8,CSerial::EParNone,CSerial::EStop1);
	lLastError = Setup(CSerial::EBaud9600,CSerial::EData8,CSerial::EParNone,CSerial::EStop1);
	if (lLastError != ERROR_SUCCESS)
		return ShowError(GetLastError(), _T("Unable to set COM-port setting"));



	startETCO2Thread();

	DEBUGMSG(TRUE, (TEXT("CInterfaceCO2_MICROPOD::Init2\r\n")));
	return 0;
}

/**********************************************************************************************//**
 * Gets the deinit
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	An int.
 **************************************************************************************************/

int CInterfaceCO2_MICROPOD::Deinit()
{
	m_bStartup=false;
	if(IsOpen())
		set_PumpStateOff(false);
	Sleep(20);
	
	stopETCO2checkThread();
	stopETCO2Thread();

	getModel()->getDATAHANDLER()->resetCO2MessureData();
	getModel()->getSPI()->ResetCO2Value();
	m_iBPM=0;
	m_iETCO2=0;
	m_iFICO2=0;

	m_byStateBytes=0xFF;
	m_byExtendedStateBytes=0xFF;

	
	m_countCmdBuf=0;
	m_iCmdLength=0;
	m_bCmdHeader=false;
	m_checksumCalc=0;
	m_crcReadIn=0;

	m_iCmdIdx=0;

	byBreak=0;

	if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff())
	{
		getModel()->getDATAHANDLER()->setMessureDataAVG(ALINK_MSMNT_FREQETCO2,0);
		getModel()->getDATAHANDLER()->setMessureDataBTB(ALINK_MSMNT_FREQETCO2,0);
	}
	getModel()->getDATAHANDLER()->setMessureDataAVG(ALINK_MSMNT_ETCO2,m_iETCO2);
	getModel()->getDATAHANDLER()->setMessureDataAVG(ALINK_MSMNT_FICO2,m_iFICO2);
	
	StopListener(-1);
	Close();
	
	if (getModel()->getAcuLink()!=NULL)
	{
		getModel()->getAcuLink()->sendAllMeasurementData();
	}

	g_eventCO2Data.SetEvent();

	DEBUGMSG(TRUE, (TEXT("CInterfaceCO2_MICROPOD::Deinit2\r\n")));
	//getModel()->setCO2inprogress(false);
	return 0;
}

/**********************************************************************************************//**
 * Startup sequenz
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CInterfaceCO2_MICROPOD::StartupSequenz()
{
	//DEBUGMSG(TRUE, (TEXT("CInterfaceCO2_MICROPOD::StartupSequenz\r\n")));
	StartListener();
	getModuleVersion();
}

/**********************************************************************************************//**
 * Initializes the sequenz
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CInterfaceCO2_MICROPOD::InitializeSequenz()
{
	if (getModel()->getAcuLink()!=NULL)
	{
		getModel()->getAcuLink()->sendAllMeasurementData();
	}
	getModel()->getDATAHANDLER()->resetCO2MessureData();
	getModel()->getSPI()->ResetCO2Value();

	set_PumpStateOn();
	get_BaroPressure();

	if(m_bVersion==false)
		getModuleVersion();

	//m_byMODULESETUPbyte1=0x01;
	//m_byMODULESETUPbyte2=0x02;
	////m_byMODULESETUPbyte2=0x22;
	////m_byMODULESETUPbyte2=0x12;

	//m_byMODULESETUPbyte3=0x00;
	//m_byMODULESETUPbyte4=0x21;//Neonatal + CO2range 0-150mmHg
	//setModuleSetup(m_byMODULESETUPbyte1, m_byMODULESETUPbyte2, m_byMODULESETUPbyte3, m_byMODULESETUPbyte4);



	m_cfgBTPS=2;				//BTPS
	m_cfgPEAKPICK=3;			//20sec
	m_cfgDISTINGBREATHMSG=0;	//disabled
	m_cfgCO2UNITS=1;			//mmHg
	m_cfgPERIODICBREATHMSG=1;	//1 second
	m_cfgWAVEMEASINTERVAL=2;	//50ms
	//m_cfgWAVEMEASINTERVAL=1;	//25ms
	m_cfgAUTOSTANDBY=0;			//disabled
	m_cfgPATIENTMODE=2;			//NEONATAL
	m_cfgCO2RANGE=2;			//0-150mmHg
	m_cfgMEASUREMODE=1;			//normal
	m_cfgCONTCO2NUM=0;			//disabled
	m_cfgCONTPERCSPONT=0;		//disabled

	//setModuleConfig(CO2_MICROPOD_MODULECONFIG_BTPS, 2);
	//setModuleConfig(CO2_MICROPOD_MODULECONFIG_PEAKPICK, 3);
	// setModuleConfig(CO2_MICROPOD_MODULECONFIG_DISTINGBREATHMSG, 0);
	//setModuleConfig(CO2_MICROPOD_MODULECONFIG_CO2UNITS, 1);
	setModuleConfig(CO2_MICROPOD_MODULECONFIG_PERIODICBREATHMSG, m_cfgPERIODICBREATHMSG);	//1 second
	setModuleConfig(CO2_MICROPOD_MODULECONFIG_WAVEMEASINTERVAL, m_cfgWAVEMEASINTERVAL);	//50ms
	//setModuleConfig(CO2_MICROPOD_MODULECONFIG_AUTOSTANDBY, 0);

	if(		getModel()->getCONFIG()->GetVentRange()==NEONATAL
		/*||	getModel()->getDATAHANDLER()->activeModeIsHFO()*/)
	{
		set_PatientMode(2);
	}
	else
	{
		set_PatientMode(3);
	}

	setModuleConfig(CO2_MICROPOD_MODULECONFIG_CO2RANGE, 2);	//0-150mmHg
	
	//if(getModel()->getDATAHANDLER()->activeModeIsHFO())
	//{
	//	set_MeasurementMode(CO2_MICROPOD_MEASUREMODE_CONTCO2);	//ContCO2
	//}
	//else
	{
		set_MeasurementMode(CO2_MICROPOD_MEASUREMODE_NORMAL);	//normal
	}
	
	getRemainTimeToCal();
	get_LastCalDate();
	getServiceHrs();

	startETCO2checkThread();

	//getModel()->setCO2inprogress(false);
}

/**********************************************************************************************//**
 * Check module state
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CInterfaceCO2_MICROPOD::CheckModuleState()
{
	
		
	BYTE cmd=m_byConfigBuffer[m_iConfigBufferCnt];

	if(		cmd==CO2_MICROPOD_MODULECONFIG_BTPS
		||	cmd==CO2_MICROPOD_MODULECONFIG_PEAKPICK
		||	cmd==CO2_MICROPOD_MODULECONFIG_DISTINGBREATHMSG
		||	cmd==CO2_MICROPOD_MODULECONFIG_CO2UNITS
		||	cmd==CO2_MICROPOD_MODULECONFIG_PERIODICBREATHMSG
		||	cmd==CO2_MICROPOD_MODULECONFIG_WAVEMEASINTERVAL
		||	cmd==CO2_MICROPOD_MODULECONFIG_AUTOSTANDBY
		||	cmd==CO2_MICROPOD_MODULECONFIG_PATIENTMODE
		||	cmd==CO2_MICROPOD_MODULECONFIG_CO2RANGE
		||	cmd==CO2_MICROPOD_MODULECONFIG_MEASUREMODE
		||	cmd==CO2_MICROPOD_MODULECONFIG_PERIODICMESSAGE_CONTCO2NUM
		||	cmd==CO2_MICROPOD_MODULECONFIG_PERIODICMESSAGE_CONTPERCSPONT)
	{
		getModuleConfig(m_byConfigBuffer[m_iConfigBufferCnt]);
	}
	else if(cmd==CO2_MICROPOD_MODULECONFIG_GETOPMODE)
	{
		getOperatingMode();
	}
	else if(cmd==CO2_MICROPOD_MODULECONFIG_GETAMBIENTPRESSURE)
	{
		get_BaroPressure();
	}

	EnterCriticalSection(&csLastCalCheck);
	DWORD temp=m_dwLastCalCheck;
	LeaveCriticalSection(&csLastCalCheck);

	if(isSafeTickCountDelayExpired(temp, 360000))
	{
		getRemainTimeToCal();
		get_LastCalDate();
		getServiceHrs();
	}
	
	m_iConfigBufferCnt++;

	if(m_iConfigBufferCnt>=MAXMICROPODMODULECONFIG)
		m_iConfigBufferCnt=0;

	//DEBUGMSG(TRUE, (TEXT("CheckLastCO2Cmd\r\n")));
	CheckLastCO2Cmd();
}

/**********************************************************************************************//**
 * Performs the message action
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CInterfaceCO2_MICROPOD::PerformMsg()
{
	bool bResult = true;
	CString szError=_T("");
	bool bNewData=false;
	
	switch(fwDataCode)
	{
		//slave messages
		case CO2_MICROPOD_SLAVECMD_WAVEMSG:
			{
				//test mainwnd
				//PBUFCO2 _pBuf;
				bool bValuevalid=true;
				bool bFilterLineNotCon=false;
				double iVal=0;
				BYTE byNewState=faDataBuf[3];
				BYTE byOldState=getStateBytes();
				if(byOldState!=byNewState)
				{
					setStateBytes(byNewState);

					if(byNewState & BIT0)
					{
						//invalid CO2 value
						bValuevalid=false;
						bNewData=true;
					}
					if(byNewState & BIT1)
					{
						//initialization
						bValuevalid=false;
						bNewData=true;
					}
					if(byNewState & BIT2)
					{
						//CO2 value over-range
						bValuevalid=false;
						bNewData=true;
					}
					if(byNewState & BIT3)
					{
						//SFM request
					}
					if(byNewState & BIT4)
					{
						//SFM in progress
						bValuevalid=false;
						bNewData=true;
					}
					if(byNewState & BIT5)
					{
						//purging in progress
						bValuevalid=false;
						bNewData=true;
					}
					if(byNewState & BIT6)
					{
						//filter line not connected
						bValuevalid=false;
						bNewData=true;
						bFilterLineNotCon=true;
					}
					if(byNewState & BIT7)
					{
						//fault bit -> check extended status
						getExtendedState();
						bValuevalid=false;
					}

					
					if(bFilterLineNotCon)
					{
						if(getModel()->getALARMHANDLER()->CanSetAlarm_CO2_FILTERLINE_NOTCONNECTED())
						//if(getModel()->getALARMHANDLER()->ALARM_Sens_CO2_FILTERLINE_NOTCONNECTED->getAlarmState()==AS_NONE)
						{
							if(getModel()->getALARMHANDLER()->ALARM_Sens_CO2_CHECKSAMPLINGLINE->getAlarmState()!=AS_NONE)
							{
								getModel()->getALARMHANDLER()->deleteAlarm(AL_Sens_CO2_CHECKSAMPLINGLINE, true);
							}
							if(getModel()->getALARMHANDLER()->ALARM_Sens_CO2_CHECKAIRWAYADAPTER->getAlarmState()!=AS_NONE)
							{
								getModel()->getALARMHANDLER()->deleteAlarm(AL_Sens_CO2_CHECKAIRWAYADAPTER, true);
							}
							if(getModel()->getALARMHANDLER()->ALARM_Sens_CO2_SENSORFAULTY->getAlarmState()!=AS_NONE)
							{
								getModel()->getALARMHANDLER()->deleteAlarm(AL_Sens_CO2_SENSORFAULTY, true);
							}

							m_bETCO2ValueValid=false;
							m_bFreqValuevalid=false;
							m_bCO2ValueValid=false;
							m_bFICO2ValueValid=false;
							
							if(AfxGetApp())
								AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_CO2_FILTERLINE_NOTCONNECTED);


							bNewData=true;
						}
					}
					else if(getModel()->getALARMHANDLER()->ALARM_Sens_CO2_FILTERLINE_NOTCONNECTED->getAlarmState()!=AS_NONE)
					{
						

						getModel()->getALARMHANDLER()->deleteAlarm(AL_Sens_CO2_FILTERLINE_NOTCONNECTED);
						bNewData=true;
						if(false==isPumpOn())
							set_PumpStateOn();
					}
				}

				SHORT iTempfaWaveBuf=0;
				
				if(bValuevalid)
				{
					iVal=(faDataBuf[1]+((double)faDataBuf[2]/256))*100;

					eCO2unit eCO2unit=getModel()->getCONFIG()->GetCO2unit();
					switch(eCO2unit)
					{
					case CO2_UNIT_KPA://mmHg -> kPa
						{
							//_pBuf.iValCO2=CTlsFloat::Round((iVal*0.15), 0);
							iTempfaWaveBuf=(SHORT)CTlsFloat::Round(((double)iVal/7.5), 0);
						}
						break;
					case CO2_UNIT_PERCENT://mmHg -> Vol%
						{
							iTempfaWaveBuf=(SHORT)CTlsFloat::Round((iVal*100)/m_iAmbientPressure, 0);
						}
						break;
					case CO2_UNIT_MMHG:
					default:
						{
							iTempfaWaveBuf=(SHORT)iVal;
						}
						break;
					}
				}
				else
				{
					iTempfaWaveBuf=0;
				}

				if(m_bCO2ValueValid!=bValuevalid)
				{
					
					m_bCO2ValueValid=bValuevalid;
					bNewData=true;
				}

				//every 25ms -> see m_cfgWAVEMEASINTERVAL
				if(m_cLastCO2WaveVal!=iTempfaWaveBuf)
				{
					doInterpolationWave(iTempfaWaveBuf);
				}
				
				//test without interpolation
				/*EnterCriticalSection(&getModel()->getDATAHANDLER()->csCO2DataBuffer);
				getModel()->getDATAHANDLER()->m_rbufCO2.write(iTempfaWaveBuf);
				LeaveCriticalSection(&getModel()->getDATAHANDLER()->csCO2DataBuffer);*/

				if(bNewData)
					g_eventCO2Data.SetEvent();

			}
			break;
		case CO2_MICROPOD_SLAVECMD_CURMODCONFIG:
			{
				WORD sValue=0;
				switch(faDataBuf[0])
				{
				case CO2_MICROPOD_MODULECONFIG_BTPS:
					{
						//TRACE(_T("CO2_MICROPOD_MODULECONFIG_BTPS \n"));
						sValue=MAKEWORD(faDataBuf[2],faDataBuf[1]);

						if(sValue!=m_cfgBTPS)
						{
							szError.Format(_T("#HFO:0112: %d, slave: %d"),m_cfgBTPS,sValue);
							theApp.WriteLog(szError);

							setModuleConfig(CO2_MICROPOD_MODULECONFIG_BTPS, m_cfgBTPS);
						}
					}
					break;
				case CO2_MICROPOD_MODULECONFIG_PEAKPICK:
					{
						//TRACE(_T("CO2_MICROPOD_MODULECONFIG_PEAKPICK \n"));
						sValue=MAKEWORD(faDataBuf[2],faDataBuf[1]);

						if(sValue!=m_cfgPEAKPICK)
						{
							szError.Format(_T("#HFO:0113: %d, slave: %d"),m_cfgPEAKPICK,sValue);
							theApp.WriteLog(szError);

							setModuleConfig(CO2_MICROPOD_MODULECONFIG_PEAKPICK, m_cfgPEAKPICK);
						}
					}
					break;
				case CO2_MICROPOD_MODULECONFIG_DISTINGBREATHMSG:
					{
						//TRACE(_T("CO2_MICROPOD_MODULECONFIG_DISTINGBREATHMSG \n"));
						sValue=MAKEWORD(faDataBuf[2],faDataBuf[1]);

						if(sValue!=m_cfgDISTINGBREATHMSG)
						{
							szError.Format(_T("#HFO:0114: %d, slave: %d"),m_cfgDISTINGBREATHMSG,sValue);
							theApp.WriteLog(szError);

							setModuleConfig(CO2_MICROPOD_MODULECONFIG_DISTINGBREATHMSG, m_cfgDISTINGBREATHMSG);
						}
					}
					break;
				case CO2_MICROPOD_MODULECONFIG_CO2UNITS:
					{
						//TRACE(_T("CO2_MICROPOD_MODULECONFIG_CO2UNITS \n"));
						sValue=MAKEWORD(faDataBuf[2],faDataBuf[1]);

						if(sValue!=m_cfgCO2UNITS)
						{
							szError.Format(_T("#HFO:0115: %d, slave: %d"),m_cfgCO2UNITS,sValue);
							theApp.WriteLog(szError);

							setModuleConfig(CO2_MICROPOD_MODULECONFIG_CO2UNITS, m_cfgCO2UNITS);
						}
					}
					break;
				case CO2_MICROPOD_MODULECONFIG_PERIODICBREATHMSG:
					{
						//TRACE(_T("CO2_MICROPOD_MODULECONFIG_PERIODICBREATHMSG \n"));
						sValue=MAKEWORD(faDataBuf[2],faDataBuf[1]);

						if(sValue!=m_cfgPERIODICBREATHMSG)
						{
							szError.Format(_T("#HFO:0116: %d, slave: %d"),m_cfgPERIODICBREATHMSG,sValue);
							theApp.WriteLog(szError);

							setModuleConfig(CO2_MICROPOD_MODULECONFIG_PERIODICBREATHMSG, m_cfgPERIODICBREATHMSG);
						}
					}
					break;
				case CO2_MICROPOD_MODULECONFIG_WAVEMEASINTERVAL:
					{
						//TRACE(_T("CO2_MICROPOD_MODULECONFIG_WAVEMEASINTERVAL \n"));
						sValue=MAKEWORD(faDataBuf[2],faDataBuf[1]);

						if(sValue!=m_cfgWAVEMEASINTERVAL)
						{
							szError.Format(_T("#HFO:0117: %d, slave: %d"),m_cfgWAVEMEASINTERVAL,sValue);
							theApp.WriteLog(szError);

							setModuleConfig(CO2_MICROPOD_MODULECONFIG_WAVEMEASINTERVAL, m_cfgWAVEMEASINTERVAL);
						}
					}
					break;
				case CO2_MICROPOD_MODULECONFIG_AUTOSTANDBY:
					{
						//TRACE(_T("CO2_MICROPOD_MODULECONFIG_AUTOSTANDBY \n"));
						sValue=MAKEWORD(faDataBuf[2],faDataBuf[1]);

						if(sValue!=m_cfgAUTOSTANDBY)
						{
							szError.Format(_T("#HFO:0118: %d, slave: %d"),m_cfgAUTOSTANDBY,sValue);
							theApp.WriteLog(szError);

							setModuleConfig(CO2_MICROPOD_MODULECONFIG_AUTOSTANDBY, m_cfgAUTOSTANDBY);
						}
					}
					break;
				case CO2_MICROPOD_MODULECONFIG_PATIENTMODE:
					{
						//TRACE(_T("CO2_MICROPOD_MODULECONFIG_PATIENTMODE \n"));
						sValue=MAKEWORD(faDataBuf[2],faDataBuf[1]);

						if(sValue!=m_cfgPATIENTMODE)
						{
							szError.Format(_T("#HFO:0119: %d, slave: %d"),m_cfgPATIENTMODE,sValue);
							theApp.WriteLog(szError);

							setModuleConfig(CO2_MICROPOD_MODULECONFIG_PATIENTMODE, m_cfgPATIENTMODE);
						}
					}
					break;
				case CO2_MICROPOD_MODULECONFIG_CO2RANGE:
					{
						//TRACE(_T("CO2_MICROPOD_MODULECONFIG_CO2RANGE \n"));
						sValue=MAKEWORD(faDataBuf[2],faDataBuf[1]);

						if(sValue!=m_cfgCO2RANGE)
						{
							szError.Format(_T("#HFO:0120: %d, slave: %d"),m_cfgCO2RANGE,sValue);
							theApp.WriteLog(szError);

							setModuleConfig(CO2_MICROPOD_MODULECONFIG_CO2RANGE, m_cfgCO2RANGE);
						}
					}
					break;
				case CO2_MICROPOD_MODULECONFIG_MEASUREMODE:
					{
						//TRACE(_T("CO2_MICROPOD_MODULECONFIG_MEASUREMODE \n"));
						sValue=MAKEWORD(faDataBuf[2],faDataBuf[1]);

						if(sValue!=m_cfgMEASUREMODE)
						{
							szError.Format(_T("#HFO:0121: %d, slave: %d"),m_cfgMEASUREMODE,sValue);
							theApp.WriteLog(szError);

							setModuleConfig(CO2_MICROPOD_MODULECONFIG_MEASUREMODE, m_cfgMEASUREMODE);
						}
					}
					break;
				case CO2_MICROPOD_MODULECONFIG_PERIODICMESSAGE_CONTCO2NUM:
					{
						//TRACE(_T("CO2_MICROPOD_MODULECONFIG_PERIODICMESSAGE_CONTCO2NUM \n"));
						sValue=MAKEWORD(faDataBuf[2],faDataBuf[1]);

						if(sValue!=m_cfgCONTCO2NUM)
						{
							szError.Format(_T("#HFO:0122: %d, slave: %d"),m_cfgCONTCO2NUM,sValue);
							theApp.WriteLog(szError);

							setModuleConfig(CO2_MICROPOD_MODULECONFIG_PERIODICMESSAGE_CONTCO2NUM, m_cfgCONTCO2NUM);
						}
					}
					break;
				case CO2_MICROPOD_MODULECONFIG_PERIODICMESSAGE_CONTPERCSPONT:
					{
						//TRACE(_T("CO2_MICROPOD_MODULECONFIG_PERIODICMESSAGE_CONTPERCSPONT \n"));
						sValue=MAKEWORD(faDataBuf[2],faDataBuf[1]);

						if(sValue!=m_cfgCONTPERCSPONT)
						{
							szError.Format(_T("#HFO:0123: %d, slave: %d"),m_cfgCONTPERCSPONT,sValue);
							theApp.WriteLog(szError);

							setModuleConfig(CO2_MICROPOD_MODULECONFIG_PERIODICMESSAGE_CONTPERCSPONT, m_cfgCONTPERCSPONT);
						}
					}
					break;
				}

			}
			break;
		case CO2_MICROPOD_SLAVECMD_CURMODSETUP:
			{
				//Byte1
				//if(pData[2]!=m_byMODULESETUPbyte1)
				//{
				//	bError=true;
				//	szError.Format(_T("ERROR MicroPod: ParseCO2Command CO2_MICROPOD_SLAVECMD_CURMODSETUP byte1 host: %d, slave: %d"),m_byMODULESETUPbyte1,pData[2]);
				//	theApp.WriteLog(szError);
				//}
				//else if(pData[3]!=m_byMODULESETUPbyte2)
				//{
				//	bError=true;
				//	szError.Format(_T("ERROR MicroPod: ParseCO2Command CO2_MICROPOD_SLAVECMD_CURMODSETUP byte2 host: %d, slave: %d"),m_byMODULESETUPbyte2,pData[3]);
				//	theApp.WriteLog(szError);
				//}
				//else if(pData[4]!=m_byMODULESETUPbyte3)
				//{
				//	bError=true;
				//	szError.Format(_T("ERROR MicroPod: ParseCO2Command CO2_MICROPOD_SLAVECMD_CURMODSETUP byte3 host: %d, slave: %d"),m_byMODULESETUPbyte3,pData[4]);
				//	theApp.WriteLog(szError);
				//}

				//if(m_byMODULESETUPbyte4 & BIT0)	
				//{
				//	if((pData[5] & BIT0)==false)
				//	{
				//		//error
				//		bError=true;
				//		szError.Format(_T("ERROR MicroPod: ParseCO2Command CO2_MICROPOD_SLAVECMD_CURMODSETUP byte4 bit0 host: %d, slave: %d"),m_byMODULESETUPbyte4,pData[5]);
				//		theApp.WriteLog(szError);
				//	}
				//}
				//else
				//{
				//	if((pData[5] & BIT0)==true)
				//	{
				//		//error
				//		bError=true;
				//		szError.Format(_T("ERROR MicroPod: ParseCO2Command CO2_MICROPOD_SLAVECMD_CURMODSETUP byte4 bit0 host: %d, slave: %d"),m_byMODULESETUPbyte4,pData[5]);
				//		theApp.WriteLog(szError);
				//	}
				//}
				//
				//if(m_byMODULESETUPbyte4 & BIT5)	
				//{
				//	if((pData[5] & BIT5)==false)
				//	{
				//		//error
				//		bError=true;
				//		szError.Format(_T("ERROR MicroPod: ParseCO2Command CO2_MICROPOD_SLAVECMD_CURMODSETUP byte4 bit5 host: %d, slave: %d"),m_byMODULESETUPbyte4,pData[5]);
				//		theApp.WriteLog(szError);
				//	}
				//}
				//else
				//{
				//	if((pData[5] & BIT5)==true)
				//	{
				//		//error
				//		bError=true;
				//		szError.Format(_T("ERROR MicroPod: ParseCO2Command CO2_MICROPOD_SLAVECMD_CURMODSETUP byte4 bit5 host: %d, slave: %d"),m_byMODULESETUPbyte4,pData[5]);
				//		theApp.WriteLog(szError);
				//	}
				//}

				//if(bError)
				//{
				//	setModuleSetup(m_byMODULESETUPbyte1, m_byMODULESETUPbyte2, m_byMODULESETUPbyte3, m_byMODULESETUPbyte4);
				//}
				
			}
			break;
		/*case CO2_MICROPOD_SLAVECMD_BREATHMSG_REPLY:
			{
				
			}
			break;*/
		case CO2_MICROPOD_SLAVECMD_EXTSTATE:
			{
				BYTE by2=faDataBuf[0];
				BYTE by3=faDataBuf[1];
				bool bNewData=false;
				bool bMalfunktion=false;
				bool bOcclusion=false;

				BYTE byOldState=getExtendedStateBytes();
				if(byOldState!=by2)
				{
					setExtendedStateBytes(by2);
					if(by2 & BIT0)
					{
						//Check calibration
						bNewData=true;
					}
					if(by2 & BIT1)
					{
						//Check flow
						bNewData=true;
						
					}
					if(by2 & BIT2)
					{
						//Occlusion in gas input line
						bNewData=true;
						bOcclusion=true;
					}
					if(by2 & BIT3)
					{
						//Temperature out of range
						bNewData=true;
						
					}
					if(by2 & BIT7)
					{
						//malfunction, service code is set
						bNewData=true;
						bMalfunktion=true;
					}
					if(bMalfunktion)
					{
						if(getModel()->getALARMHANDLER()->CanSetAlarm_CO2_SENSORFAULTY())
						//if(getModel()->getALARMHANDLER()->ALARM_Sens_CO2_SENSORFAULTY->getAlarmState()==AS_NONE)
						{
							m_bETCO2ValueValid=false;
							m_bFreqValuevalid=false;
							m_bCO2ValueValid=false;
							m_bFICO2ValueValid=false;

							if(AfxGetApp())
								AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_CO2_SENSORFAULTY);
							bNewData=true;
							
						}
					}
					else if(getModel()->getALARMHANDLER()->ALARM_Sens_CO2_SENSORFAULTY->getAlarmState()!=AS_NONE)
					{
						getModel()->getALARMHANDLER()->deleteAlarm(AL_Sens_CO2_SENSORFAULTY);
						bNewData=true;
					}
					if(bOcclusion)
					{
						DEBUGMSG(TRUE, (TEXT("Occlusion\r\n")));
						if(getModel()->getALARMHANDLER()->CanSetAlarm_CO2_CHECKAIRWAYADAPTER())
						//if(getModel()->getALARMHANDLER()->ALARM_Sens_CO2_CHECKAIRWAYADAPTER->getAlarmState()==AS_NONE)
						{
							m_bETCO2ValueValid=false;
							m_bFreqValuevalid=false;
							m_bCO2ValueValid=false;
							m_bFICO2ValueValid=false;

							if(AfxGetApp())
								AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_CO2_CHECKAIRWAYADAPTER);
							bNewData=true;
						}
					}
					else if(getModel()->getALARMHANDLER()->ALARM_Sens_CO2_CHECKAIRWAYADAPTER->getAlarmState()!=AS_NONE)
					{
						DEBUGMSG(TRUE, (TEXT("delete Occlusion\r\n")));
						getModel()->getALARMHANDLER()->deleteAlarm(AL_Sens_CO2_CHECKAIRWAYADAPTER);
						bNewData=true;
					}
					/*else
					{
				
						DEBUGMSG(TRUE, (TEXT("no Occlusion\r\n")));
					}*/
				}

				
				if(by3 & BIT0)
				{
					//pump off
					if(m_bPumpStateOn)
					{
						theApp.WriteLog(_T("*** CO2 pump: off ***"));
						bNewData=true;
					}
					
					m_bPumpStateOn=false; // on / standby ????
					DEBUGMSG(TRUE, (TEXT("pump off\r\n")));

					/*if(getModel()->getALARMHANDLER()->getState_CO2_PUMPOFF()==AS_NONE)
					{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_CO2_PUMPOFF);
					}*/

					m_bETCO2ValueValid=false;
					m_bCO2ValueValid=false;
					m_bFreqValuevalid=false;
					m_bFICO2ValueValid=false;

				}
				else
				{
					if(m_bPumpStateOn==false)
					{
						theApp.WriteLog(_T("*** CO2 pump: on ***"));
						m_bPumpStateOn=true;
						bNewData=true;
					}
					
					/*if(getModel()->getALARMHANDLER()->getState_CO2_PUMPOFF()==AS_ACTIVE)
					{
						getModel()->getALARMHANDLER()->deleteAlarm(AL_Sens_CO2_PUMPOFF);
					}*/
				}

				if(bNewData)
					g_eventCO2Data.SetEvent();
				
			}
			break;
		case CO2_MICROPOD_SLAVECMD_IPI:
			{
			}
			break;
		case CO2_MICROPOD_SLAVECMD_REMHRSTOSERVICE:
			{
				EnterCriticalSection(&csLastCalCheck);
				m_dwLastCalCheck=GetTickCount();
				LeaveCriticalSection(&csLastCalCheck);

				WORD sValue=MAKEWORD(faDataBuf[1],faDataBuf[0]);
				//if(sValue!=m_iRemainServiceHrs)
				{
					m_iRemainServiceHrs=sValue;
					if(m_iRemainServiceHrs==0)
					{
						m_bServiceIsDue=true;
					}
				}
			}
			break;
		case CO2_MICROPOD_SLAVECMD_CALDATEMSG:
			{
				EnterCriticalSection(&csLastCalCheck);
				m_dwLastCalCheck=GetTickCount();
				LeaveCriticalSection(&csLastCalCheck);
				m_dtCalibrationDateTime.SetDate(faDataBuf[2]+1900,faDataBuf[1],faDataBuf[0]);
			}
			break;
		case CO2_MICROPOD_SLAVECMD_MAXOPHRSCALIBRATION:
			{
				EnterCriticalSection(&csLastCalCheck);
				m_dwLastCalCheck=GetTickCount();
				LeaveCriticalSection(&csLastCalCheck);
				WORD sValue=MAKEWORD(faDataBuf[1],faDataBuf[0]);
				m_iMaxOpHRSCalibration=sValue;
				if(m_iMaxOpHRSCalibration==0)
				{
					m_bCalibrationIsDue=true;
				}
				else
				{
					m_bCalibrationIsDue=false;
				}
			}
			break;
		case CO2_MICROPOD_SLAVECMD_CURAMBIENTPRES:
			{
				//TRACE(_T("CO2_MICROPOD_SLAVECMD_CURAMBIENTPRES \n"));
				WORD sValue=MAKEWORD(faDataBuf[1],faDataBuf[0]);
				if(sValue!=m_iAmbientPressure)
				{
					m_iAmbientPressure=sValue;
					getModel()->getCONFIG()->SetCO2BaroPressure(m_iAmbientPressure);
				}

			}
			break;
		case CO2_MICROPOD_SLAVECMD_BREATHMSG_DETECTED:
			{
				SHORT iETCO2=(SHORT)faDataBuf[0]*10;
				SHORT iFICO2=(SHORT)faDataBuf[1]*10;
				BYTE iBPM=faDataBuf[2];

				if(m_cfgMEASUREMODE==CO2_MICROPOD_MEASUREMODE_NORMAL)
				{
					if(m_bCO2ValueValid==false)
						m_bCO2ValueValid=true;

					if(m_iETCO2!=iETCO2)
					{
						if(iETCO2==2550 || iETCO2==1510 || iBPM==255)
						{
							m_bETCO2ValueValid=false;
						}
						else
						{
							m_bETCO2ValueValid=true;

							eCO2unit eCO2unit=getModel()->getCONFIG()->GetCO2unit();
							switch(eCO2unit)
							{
							case CO2_UNIT_KPA://mmHg -> kPa
								{
									iETCO2=(SHORT)CTlsFloat::Round(((double)iETCO2/7.5), 0);
									//iFiCO2=CTlsFloat::Round(((double)iFiCO2/7.5), 0);
								}
								break;
							case CO2_UNIT_PERCENT://mmHg -> Vol%
								{
									iETCO2=(SHORT)CTlsFloat::Round(((double)iETCO2*100)/m_iAmbientPressure, 0);
									//iFiCO2=CTlsFloat::Round(((double)iFiCO2*100)/getModel()->getCONFIG()->GetCO2BaroPressure(), 0);
								}
								break;
							case CO2_UNIT_MMHG:
							default:
								{

								}
								break;
							}
						}

						bNewData=true;
						m_iETCO2=iETCO2;
						getModel()->getDATAHANDLER()->setMessureDataAVG(ALINK_MSMNT_ETCO2,m_iETCO2);
					}

					if(m_iFICO2!=iFICO2)
					{
						DEBUGMSG(TRUE, (TEXT("FICO2 %d\r\n"),iFICO2));
						if(iFICO2==2550 || iFICO2==1510 || iBPM==255)
						{
							m_bFICO2ValueValid=false;
						}
						else
						{
							m_bFICO2ValueValid=true;

							eCO2unit eCO2unit=getModel()->getCONFIG()->GetCO2unit();
							switch(eCO2unit)
							{
							case CO2_UNIT_KPA://mmHg -> kPa
								{
									iFICO2=(SHORT)CTlsFloat::Round(((double)iFICO2/7.5), 0);
									//iFiCO2=CTlsFloat::Round(((double)iFiCO2/7.5), 0);
								}
								break;
							case CO2_UNIT_PERCENT://mmHg -> Vol%
								{
									iFICO2=(SHORT)CTlsFloat::Round(((double)iFICO2*100)/m_iAmbientPressure, 0);
									//iFiCO2=CTlsFloat::Round(((double)iFiCO2*100)/getModel()->getCONFIG()->GetCO2BaroPressure(), 0);
								}
								break;
							case CO2_UNIT_MMHG:
							default:
								{

								}
								break;
							}
						}

						bNewData=true;
						m_iFICO2=iFICO2;
						getModel()->getDATAHANDLER()->setMessureDataAVG(ALINK_MSMNT_FICO2,m_iFICO2);
					}

					if(m_iBPM!=iBPM)
					{
						bNewData=true;

						if(iBPM>=0xFF)
							m_bFreqValuevalid=false;
						else
						{
							m_bFreqValuevalid=true;

							/*if(m_bPumpStateOn==false)
							{
								m_bPumpStateOn=true;
								bNewData=true;
							}*/
						}

						m_iBPM=iBPM;

						//DEBUGMSG(TRUE, (TEXT("set BPM %d\r\n"),m_iBPM));

						getModel()->getDATAHANDLER()->setMessureDataAVG(ALINK_MSMNT_FREQETCO2,iBPM);
						getModel()->getDATAHANDLER()->setMessureDataBTB(ALINK_MSMNT_FREQETCO2,iBPM);

					}


					if(bNewData)
						g_eventCO2Data.SetEvent();
				}

			}
			break;
		case CO2_MICROPOD_SLAVECMD_BREATHMSG_REPLY:
			{
				SHORT iETCO2=(SHORT)faDataBuf[0]*10;
				SHORT iFICO2=(SHORT)faDataBuf[1]*10;
				BYTE iBPM=faDataBuf[2];

				if(m_cfgMEASUREMODE==CO2_MICROPOD_MEASUREMODE_NORMAL)
				{
					if(m_iETCO2!=iETCO2)
					{
						if(iETCO2==2550 || iETCO2==1510 || iBPM==255)
						{
							m_bETCO2ValueValid=false;
						}
						else
						{
							m_bETCO2ValueValid=true;

							eCO2unit eCO2unit=getModel()->getCONFIG()->GetCO2unit();
							switch(eCO2unit)
							{
							case CO2_UNIT_KPA://mmHg -> kPa
								{
									iETCO2=(SHORT)CTlsFloat::Round(((double)iETCO2/7.5), 0);
									//iFiCO2=CTlsFloat::Round(((double)iFiCO2/7.5), 0);
								}
								break;
							case CO2_UNIT_PERCENT://mmHg -> Vol%
								{
									iETCO2=(SHORT)CTlsFloat::Round(((double)iETCO2*100)/m_iAmbientPressure, 0);
									//iFiCO2=CTlsFloat::Round(((double)iFiCO2*100)/getModel()->getCONFIG()->GetCO2BaroPressure(), 0);
								}
								break;
							case CO2_UNIT_MMHG:
							default:
								{

								}
								break;
							}
						}

						bNewData=true;
						m_iETCO2=iETCO2;
						getModel()->getDATAHANDLER()->setMessureDataAVG(ALINK_MSMNT_ETCO2,m_iETCO2);
					}

					if(m_iFICO2!=iFICO2)
					{
						DEBUGMSG(TRUE, (TEXT("FICO2 #2 %d\r\n"),iFICO2));

						if(iFICO2==2550 || iFICO2==1510 || iBPM==255)
						{
							m_bFICO2ValueValid=false;
						}
						else
						{
							m_bFICO2ValueValid=true;

							eCO2unit eCO2unit=getModel()->getCONFIG()->GetCO2unit();
							switch(eCO2unit)
							{
							case CO2_UNIT_KPA://mmHg -> kPa
								{
									iFICO2=(SHORT)CTlsFloat::Round(((double)iFICO2/7.5), 0);
									//iFiCO2=CTlsFloat::Round(((double)iFiCO2/7.5), 0);
								}
								break;
							case CO2_UNIT_PERCENT://mmHg -> Vol%
								{
									iFICO2=(SHORT)CTlsFloat::Round(((double)iFICO2*100)/m_iAmbientPressure, 0);
									//iFiCO2=CTlsFloat::Round(((double)iFiCO2*100)/getModel()->getCONFIG()->GetCO2BaroPressure(), 0);
								}
								break;
							case CO2_UNIT_MMHG:
							default:
								{

								}
								break;
							}
						}

						bNewData=true;
						m_iFICO2=iFICO2;
						getModel()->getDATAHANDLER()->setMessureDataAVG(ALINK_MSMNT_FICO2,m_iFICO2);
					}

					if(m_iBPM!=iBPM)
					{
						bNewData=true;

						if(iBPM>=0xFF)
							m_bFreqValuevalid=false;
						else
						{
							m_bFreqValuevalid=true;

							/*if(m_bPumpStateOn==false)
							{
								m_bPumpStateOn=true;
								bNewData=true;
							}*/
						}

						m_iBPM=iBPM;

						//DEBUGMSG(TRUE, (TEXT("set BPM %d\r\n"),m_iBPM));

						getModel()->getDATAHANDLER()->setMessureDataAVG(ALINK_MSMNT_FREQETCO2,iBPM);
						getModel()->getDATAHANDLER()->setMessureDataBTB(ALINK_MSMNT_FREQETCO2,iBPM);

					}
					

					if(bNewData)
						g_eventCO2Data.SetEvent();
				}

			}
			break;
		//case CO2_MICROPOD_SLAVECMD_BREATHMSG_DETECTED:
		//	{
		//		TRACE(_T("CO2_MICROPOD_SLAVECMD_BREATHMSG_DETECTED \n"));
		//		SHORT iETCO2=(SHORT)pData[2]*10;
		//		BYTE i2=pData[3];

		//		if(m_cfgMEASUREMODE==CO2_MICROPOD_MEASUREMODE_NORMAL)
		//		{
		//			eCO2unit eCO2unit=getModel()->getCONFIG()->GetCO2unit();
		//			switch(eCO2unit)
		//			{
		//			case CO2_UNIT_KPA://mmHg -> kPa
		//				{
		//					iETCO2=CTlsFloat::Round(((double)iETCO2/7.5), 0);
		//				}
		//				break;
		//			case CO2_UNIT_PERCENT://mmHg -> Vol%
		//				{
		//					iETCO2=CTlsFloat::Round(((double)iETCO2*100)/getModel()->getCONFIG()->GetCO2BaroPressure(), 0);
		//				}
		//				break;
		//			case CO2_UNIT_MMHG:
		//			default:
		//				{

		//				}
		//				break;
		//			}

		//			if(m_iETCO2!=iETCO2)
		//			{
		//				bNewData=true;
		//				m_iETCO2=iETCO2;
		//				getModel()->getDATAHANDLER()->setAVGMessureDataETCO2(m_iETCO2);
		//			}

		//			if(bNewData)
		//				g_eventCO2Data.SetEvent();
		//		}

		//		
		//	}
		//	break;
		case CO2_MICROPOD_SLAVECMD_VERSION:
			{
				//TRACE(_T("CO2_MICROPOD_SLAVECMD_VERSION \n"));

				//BYTE byLen=pData[0];
				if(fwDataSize==62)
				{
					CString szVersion(faDataBuf);

					m_szVersion=szVersion.Left(56);
					//m_szVersion=szVersion.Mid(0,56);
					CString szSelfTest=szVersion.Mid(57,4);

					if(szSelfTest==_T("Pass"))
					{
						m_bVersion=true;
					}
					else
					{
						//error
						if(getModel()->getALARMHANDLER()->CanSetAlarm_CO2_SENSORFAULTY())
						//if(getModel()->getALARMHANDLER()->ALARM_Sens_CO2_SENSORFAULTY->getAlarmState()==AS_NONE)
						{
							if(AfxGetApp())
								AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_CO2_SENSORFAULTY);
							m_bETCO2ValueValid=false;
							m_bFreqValuevalid=false;
							m_bCO2ValueValid=false;
							m_bFICO2ValueValid=false;
							g_eventCO2Data.SetEvent();
						}
					}
				}
			}
			break;
		case CO2_MICROPOD_SLAVECMD_CMDRECEIVEDMSG:
			{
				/*BYTE byCmdCode=faDataBuf[0];
				BYTE byCmdAckn=faDataBuf[1];*/

				
			}
			break;
		case CO2_MICROPOD_SLAVECMD_CUROPMODE:
			{

				if(faDataBuf[0]!=m_cfgOPERATINGMODE)
				{
					szError.Format(_T("#HFO:0124: %d, slave: %d"),m_cfgOPERATINGMODE,faDataBuf[0]);
					theApp.WriteLog(szError);

					setOperatingMode(m_cfgOPERATINGMODE);
				}

				
			}
			break;
		default:
			{
				bResult = false;
				
				/*CStringW szTest=_T("");
				szTest.Format(_T("%X\n"), fwDataCode);*/
				
			}
			break;
	}

	
	return bResult;
}

/**********************************************************************************************//**
 * Executes the event action
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	eEvent	The event.
 * \param	eError	The error.
 **************************************************************************************************/

void CInterfaceCO2_MICROPOD::OnEvent(EEvent eEvent, EError eError)
{
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
				case CO2_MICROPOD_HEADER:
					
					fwDataCnt=0;
					fwCalcChkSum = 0;
					fwReadChkSum=0;
					fwDataSize=0;
					fwDataCode=0;
					
					switch (faRecvBuf[i])
					{
					case MICROPOD_MSG_HEADER:
						feRecvState = CO2_MICROPOD_DATA_SIZE;
						break;

					default:
						// forget the rest
						feRecvState = CO2_MICROPOD_HEADER;
						return;
					} // switch (faRecvBuf[i])
					break;

				case CO2_MICROPOD_DATA_SIZE:
					
					fwCalcChkSum ^= c;
					switch (c)
					{
					case 0x80:
						feRecvState = CO2_MICROPOD_DATA_SIZE1;
						break;

					default:
						fwDataSize = c;
						feRecvState = CO2_MICROPOD_CODE;
						break;
					} // switch (c)
					break;

				case CO2_MICROPOD_DATA_SIZE1:
					fwCalcChkSum ^= c;
					
					switch (c)
					{
					case 0x00:
						fwDataSize = 0x80;
						feRecvState = CO2_MICROPOD_CODE;
						break;

					case 0x05:
						fwDataSize = 0x85;
						feRecvState = CO2_MICROPOD_CODE;
						break;

					default:
						feRecvState = CO2_MICROPOD_HEADER;
						return;
					} // switch (fdwReqType)
					break;

				case CO2_MICROPOD_CODE:
					fwCalcChkSum ^= c;
					switch (c)
					{
					case 0x80:
						feRecvState = CO2_MICROPOD_CODE1;
						break;

					default:
						fwDataCode = c;
						feRecvState = CO2_MICROPOD_DATA;
						break;
					} // switch (c)
					break;

				case CO2_MICROPOD_CODE1:
					fwCalcChkSum ^= c;

					switch (c)
					{
					case 0x00:
						fwDataCode = 0x80;
						feRecvState = CO2_MICROPOD_DATA;
						break;

					case 0x05:
						fwDataCode = 0x85;
						feRecvState = CO2_MICROPOD_DATA;
						break;

					default:
						feRecvState = CO2_MICROPOD_HEADER;
						return;
					} // switch (fdwReqType)
					break;

				case CO2_MICROPOD_DATA:
					fwCalcChkSum ^= c;
					switch (c)
					{
					case 0x80:
						feRecvState = CO2_MICROPOD_DATA1;
						break;

					default:
						faDataBuf[fwDataCnt] = c;
						fwDataCnt++;
						feRecvState = CO2_MICROPOD_DATA;
						break;
					} // switch (c)
					
					if(fwDataCnt>=(fwDataSize-1))
						feRecvState = CO2_MICROPOD_CHKSUM;
					break;

				case CO2_MICROPOD_DATA1:
					fwCalcChkSum ^= c;

					switch (c)
					{
					case 0x00:
						faDataBuf[fwDataCnt] = 0x80;
						fwDataCnt++;
						break;

					case 0x05:
						faDataBuf[fwDataCnt] = 0x85;
						fwDataCnt++;
						break;

					default:
						feRecvState = CO2_MICROPOD_HEADER;
						return;
					} // switch (fdwReqType)

					if(fwDataCnt>=(fwDataSize-1))
						feRecvState = CO2_MICROPOD_CHKSUM;
					else
						feRecvState = CO2_MICROPOD_DATA;
					break;

				case CO2_MICROPOD_CHKSUM:
					switch (c)
					{
					case 0x80:
						feRecvState = CO2_MICROPOD_CHKSUM1;
						break;

					default:
						fwReadChkSum = c;
						if(fwReadChkSum == fwCalcChkSum)
						{
							if(this->PerformMsg())
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
						feRecvState = CO2_MICROPOD_HEADER;
						break;
					} // switch (c)
					break;

				case CO2_MICROPOD_CHKSUM1:
					
					switch (c)
					{
					case 0x00:
						fwReadChkSum = 0x80;
						break;

					case 0x05:
						fwReadChkSum = 0x85;
						break;

					default:
						feRecvState = CO2_MICROPOD_HEADER;
						return;
					} // switch (fdwReqType)

					if(fwReadChkSum == fwCalcChkSum)
					{
						if(this->PerformMsg())
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
					feRecvState = CO2_MICROPOD_HEADER;
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

/**********************************************************************************************//**
 * Executes the interpolation wave operation
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	iTempfaWaveBuf	Buffer for temp wave data.
 **************************************************************************************************/

void CInterfaceCO2_MICROPOD::doInterpolationWave(SHORT iTempfaWaveBuf)
{
	if(false==getModel()->getSPI()->isSPIrunning())
		return;

	//DEBUGMSG(TRUE, (TEXT("wave %d\r\n"),iTempfaWaveBuf));
	SHORT iDiff=((iTempfaWaveBuf-m_cLastCO2WaveVal)/48);
	
	EnterCriticalSection(&getModel()->getDATAHANDLER()->csCO2DataBuffer);
	for(int i=0;i<48;i++)
	{
		getModel()->getDATAHANDLER()->m_rbufCO2.write(m_cLastCO2WaveVal);
		m_cLastCO2WaveVal+=iDiff;
	}
	LeaveCriticalSection(&getModel()->getDATAHANDLER()->csCO2DataBuffer);

	m_cLastCO2WaveVal=iTempfaWaveBuf;
}

//bool CInterfaceCO2_MICROPOD::ValidateMessage(BYTE* pData, size_t iLen)
//{
//	bool bResult=false;
//
//
//	/*BYTE checksum=0;
//	BYTE size=iLen-2;
//
//	for(int i = 1; i <= size; i++)
//	{   
//		checksum ^= pData[i];
//	}
//
//	if(pData[iLen-1]==checksum)
//		bResult=true;
//	else
//		int iSTop=0;*/
//
//
//	
//
//	int i = 0;
//
//	//for(int i = 0; i < iLen; i++)
//	{   
//		if(pData[i]==0x85)
//		{
//			BYTE checksum=0;
//			BYTE crc=0;
//
//			i++;
//			BYTE length=pData[i];
//			checksum ^= pData[i];
//			
//			i++;
//
//			for(int iLen=0; iLen < length; iLen++)
//			{   
//				if(pData[i]==0x80)
//				{
//					checksum ^= pData[i];
//					i++;
//					checksum ^= pData[i];
//				}
//				else
//				{
//					checksum ^= pData[i];
//				}
//				
//				i++;
//			}
//
//			if(pData[i]==0x80)
//			{
//				i++;
//				if(pData[i]==0x05)
//				{
//					crc=0x85;
//				}
//				else
//				{
//					crc=0x80;
//				}
//			}
//			else
//			{
//				crc=pData[i];
//			}
//
//			if(crc==checksum)
//			{
//				bResult=true;
//			}
//			else
//			{
//				int iStop=0;
//			}
//			//break;
//		}
//	}
//	
//	return bResult;
//}

/**********************************************************************************************//**
 * Change co2 unit
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	unitNew	The unit new.
 * \param	unitOld	The unit old.
 **************************************************************************************************/

void CInterfaceCO2_MICROPOD::changeCO2Unit(eCO2unit unitNew,eCO2unit unitOld)
{
	int iAlimitETCO2max=getModel()->getALARMHANDLER()->getAlimitETCO2max();
	int iAlimitETCO2min=getModel()->getALARMHANDLER()->getAlimitETCO2min();

	iAlimitETCO2max=(int)(CTlsFloat::Round(((double)iAlimitETCO2max)/10, 0))*10;
	iAlimitETCO2min=(int)(CTlsFloat::Round(((double)iAlimitETCO2min)/10, 0))*10;


	//switch(eCO2unit)
	//{
	//case CO2_UNIT_KPA://mmHg -> kPa
	//	{
	//		//_pBuf.iValCO2=CTlsFloat::Round((iVal*0.15), 0);
	//		_pBuf.iValCO2=CTlsFloat::Round(((double)iVal/7.5), 0);
	//	}
	//	break;
	//case CO2_UNIT_PERCENT://mmHg -> Vol%
	//	{
	//		_pBuf.iValCO2=CTlsFloat::Round((iVal*100)/getModel()->getCONFIG()->GetCO2BaroPressure(), 0);
	//	}
	//	break;
	//case CO2_UNIT_MMHG:
	//default:
	//	{
	//		_pBuf.iValCO2=iVal;
	//	}
	//	break;
	//}

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
				iAlimitETCO2max=(int)CTlsFloat::Round(((double)iAlimitETCO2max*m_iAmbientPressure)/100, 0);
				iAlimitETCO2min=(int)CTlsFloat::Round(((double)iAlimitETCO2min*m_iAmbientPressure)/100, 0);
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
				iAlimitETCO2max=(int)CTlsFloat::Round((((double)iAlimitETCO2max*100)/m_iAmbientPressure)*7.5, 0);
				iAlimitETCO2min=(int)CTlsFloat::Round((((double)iAlimitETCO2min*100)/m_iAmbientPressure)*7.5, 0);
			}
		}
		break;
	case CO2_UNIT_PERCENT:
		{
			if(unitOld==CO2_UNIT_KPA)//kPa -> Vol%
			{
				iAlimitETCO2max=(int)CTlsFloat::Round(((double)iAlimitETCO2max*750)/m_iAmbientPressure, 0);
				iAlimitETCO2min=(int)CTlsFloat::Round(((double)iAlimitETCO2min*750)/m_iAmbientPressure, 0);
			}
			else if(unitOld==CO2_UNIT_MMHG)//mmHg -> Vol%   OK
			{
				iAlimitETCO2max=(int)CTlsFloat::Round(((double)iAlimitETCO2max*100)/m_iAmbientPressure, 0);
				iAlimitETCO2min=(int)CTlsFloat::Round(((double)iAlimitETCO2min*100)/m_iAmbientPressure, 0);
			}
		}
		break;
	}
	iAlimitETCO2max=(int)(CTlsFloat::Round(((double)iAlimitETCO2max)/10, 0))*10;
	iAlimitETCO2min=(int)(CTlsFloat::Round(((double)iAlimitETCO2min)/10, 0))*10;
	getModel()->getALARMHANDLER()->setAlimitETCO2max(iAlimitETCO2max);
	getModel()->getALARMHANDLER()->setAlimitETCO2min(iAlimitETCO2min);

	getModel()->getI2C()->WriteConfigByte(CO2UNIT_8, (int)unitNew);

	if(getModel()->getAcuLink()!=NULL)
	{
		getModel()->getAcuLink()->setParaData(ALINK_SETT_UNIT_CO2,(int)unitNew);
	}


	getBreathMessage();
}

/**********************************************************************************************//**
 * Sets gas compensation
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	byO2Compensation	The by o2 compensation.
 * \param	byBalanceGas		The by balance gas.
 * \param	iAnestAgent			Zero-based index of the anest agent.
 **************************************************************************************************/

void CInterfaceCO2_MICROPOD::set_GasCompensation(BYTE byO2Compensation, BYTE byBalanceGas, SHORT iAnestAgent)
{
	if(getModel()->getAcuLink()!=NULL)
	{
		getModel()->getAcuLink()->setParaData(ALINK_SETT_O2COMPENSATION_CO2,ALINK_NOTVALID);
	}
}

/**********************************************************************************************//**
 * Gets gas compensation
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CInterfaceCO2_MICROPOD::get_GasCompensation()
{

}

/**********************************************************************************************//**
 * Sets patient mode
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	val	The value.
 **************************************************************************************************/

void CInterfaceCO2_MICROPOD::set_PatientMode(BYTE val)
{
	m_cfgPATIENTMODE=val;
	setModuleConfig(CO2_MICROPOD_MODULECONFIG_PATIENTMODE, val);
}

/**********************************************************************************************//**
 * Gets patient mode
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CInterfaceCO2_MICROPOD::get_PatientMode()
{

}

/**********************************************************************************************//**
 * Sets measurement mode
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	val	The value.
 **************************************************************************************************/

void CInterfaceCO2_MICROPOD::set_MeasurementMode(BYTE val)
{
	m_cfgMEASUREMODE=val;
	setModuleConfig(CO2_MICROPOD_MODULECONFIG_MEASUREMODE, val);
	if(m_cfgMEASUREMODE==CO2_MICROPOD_MEASUREMODE_NORMAL)
	{
		m_cfgCONTCO2NUM=0;			//disabled
		m_cfgCONTPERCSPONT=0;		//disabled
		setModuleConfig(CO2_MICROPOD_MODULECONFIG_PERIODICMESSAGE_CONTCO2NUM, m_cfgCONTCO2NUM);
		setModuleConfig(CO2_MICROPOD_MODULECONFIG_PERIODICMESSAGE_CONTPERCSPONT, m_cfgCONTPERCSPONT);
	}
	else
	{
		m_cfgCONTCO2NUM=1;			//enabled
		m_cfgCONTPERCSPONT=1;		//enabled
		setModuleConfig(CO2_MICROPOD_MODULECONFIG_PERIODICMESSAGE_CONTCO2NUM, m_cfgCONTCO2NUM);
		setModuleConfig(CO2_MICROPOD_MODULECONFIG_PERIODICMESSAGE_CONTPERCSPONT, m_cfgCONTPERCSPONT);
	}
	//set_restartBreathAlgorithm();
	
}

/**********************************************************************************************//**
 * Gets measurement mode
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CInterfaceCO2_MICROPOD::get_MeasurementMode()
{

}

/**********************************************************************************************//**
 * Sets baro pressure
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	val	The value.
 **************************************************************************************************/

void CInterfaceCO2_MICROPOD::set_BaroPressure(SHORT val)
{
	/*if(getModel()->getAcuLink()!=NULL)
	{
		getModel()->getAcuLink()->setParaData(ALINK_SETT_BAROPRESSURE_CO2,(int)val);
	}*/
}

/**********************************************************************************************//**
 * Gets baro pressure
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CInterfaceCO2_MICROPOD::get_BaroPressure()
{
	SendCO2Command(CO2_MICROPOD_HOSTCMD_GETAMBIENTPRESSURE,0x01);
}

/**********************************************************************************************//**
 * Gets remain time to calibration
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CInterfaceCO2_MICROPOD::getRemainTimeToCal()
{
	SendCO2Command(CO2_MICROPOD_HOSTCMD_GETMAXTOCAL,0x01);
}

/**********************************************************************************************//**
 * Gets the last calibration date
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CInterfaceCO2_MICROPOD::get_LastCalDate()
{
	SendCO2Command(CO2_MICROPOD_HOSTCMD_GETCALDATE,0x01);
}

/**********************************************************************************************//**
 * Gets service hrs
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CInterfaceCO2_MICROPOD::getServiceHrs()
{
	SendCO2Command(CO2_MICROPOD_HOSTCMD_GETSERVICEHOURS,0x01);
}

/**********************************************************************************************//**
 * Resets the state bytes
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CInterfaceCO2_MICROPOD::resetStateBytes()
{
	EnterCriticalSection(&csErrorDataCO2);
	m_byStateBytes=0xFF;
	m_byExtendedStateBytes=0xFF;
	LeaveCriticalSection(&csErrorDataCO2);
}

/**********************************************************************************************//**
 * Sets state bytes
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CInterfaceCO2_MICROPOD::setStateBytes(BYTE state)
{
	EnterCriticalSection(&csErrorDataCO2);
	m_byStateBytes=state;	
	LeaveCriticalSection(&csErrorDataCO2);
}

/**********************************************************************************************//**
 * Gets state bytes
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The state bytes.
 **************************************************************************************************/

BYTE CInterfaceCO2_MICROPOD::getStateBytes()
{
	EnterCriticalSection(&csErrorDataCO2);
	BYTE byTemp = m_byStateBytes;
	LeaveCriticalSection(&csErrorDataCO2);
	return byTemp;
}

/**********************************************************************************************//**
 * Query if this instance is state ok
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if state ok, false if not.
 **************************************************************************************************/

bool CInterfaceCO2_MICROPOD::isStateOk()
{
	//EnterCriticalSection(&csErrorDataCO2);
	//BYTE state=m_byStateBytes;
	//LeaveCriticalSection(&csErrorDataCO2);

	bool bRes=true;

	if(false==isPumpOn())
		bRes=false;

	if(m_cfgOPERATINGMODE!=CO2_MICROPOD_OPMODE_MEASUREMENT)
		bRes=false;

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

/**********************************************************************************************//**
 * Sets extended state bytes
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CInterfaceCO2_MICROPOD::setExtendedStateBytes(BYTE state)
{
	EnterCriticalSection(&csErrorDataCO2);
	m_byExtendedStateBytes=state;
	LeaveCriticalSection(&csErrorDataCO2);
}

/**********************************************************************************************//**
 * Gets extended state bytes
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The extended state bytes.
 **************************************************************************************************/

BYTE CInterfaceCO2_MICROPOD::getExtendedStateBytes()
{
	EnterCriticalSection(&csErrorDataCO2);
	BYTE byTemp = m_byExtendedStateBytes;
	LeaveCriticalSection(&csErrorDataCO2);
	return byTemp;
}

/**********************************************************************************************//**
 * Query if this instance is extended state ok
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if extended state ok, false if not.
 **************************************************************************************************/

bool CInterfaceCO2_MICROPOD::isExtendedStateOk()
{
	/*EnterCriticalSection(&csErrorDataCO2);
	BYTE state=m_byExtendedStateBytes;
	LeaveCriticalSection(&csErrorDataCO2);*/

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

/**********************************************************************************************//**
 * Sends a co2 command
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	CO2_command	The co 2 command.
 * \param	Anzahlbytes	The anzahlbytes.
 * \param	byte0	   	The byte 0.
 * \param	byte1	   	The first byte.
 * \param	byte2	   	The second byte.
 * \param	byte3	   	The third byte.
 * \param	byte4	   	The fourth byte.
 * \param	byte5	   	The fifth byte.
 * \param	byte6	   	The byte 6.
 *
 * \return	A DWORD.
 **************************************************************************************************/

DWORD CInterfaceCO2_MICROPOD::SendCO2Command(BYTE CO2_command, BYTE Anzahlbytes, BYTE byte0, BYTE byte1, BYTE byte2, BYTE byte3, BYTE byte4, BYTE byte5, BYTE byte6) 
{
	//if(getModel()->getDATAHANDLER()->IsCO2interfaceEnabled())
	{
		BYTE checksum=0;
		
		LPCO2MSG msg = new CO2MSG;

		BYTE byTmpBuffer[7];
		byTmpBuffer[0]=byte0;
		byTmpBuffer[1]=byte1;
		byTmpBuffer[2]=byte2;
		byTmpBuffer[3]=byte3;
		byTmpBuffer[4]=byte4;
		byTmpBuffer[5]=byte5;
		byTmpBuffer[6]=byte6;


		int iCount=0;
		msg->byBuffer[iCount]=0x85;
		iCount++;

		msg->byBuffer[iCount]=Anzahlbytes;	//command byte + data bytes
		checksum ^= Anzahlbytes;
		iCount++;

		msg->byBuffer[iCount]=CO2_command;
		checksum ^= CO2_command;
		iCount++;

		int iBytesReadin=Anzahlbytes-1;
		
		for(int i=0;i<iBytesReadin;i++)
		{
			if(byTmpBuffer[i]==0x80)
			{
				msg->byBuffer[iCount]=0x80;
				checksum ^= 0x80;
				iCount++;
				msg->byBuffer[iCount]=0x00;
				checksum ^= 0x00;
			}
			else if(byTmpBuffer[i]==0x85)
			{
				msg->byBuffer[iCount]=0x80;
				checksum ^= 0x80;
				iCount++;
				msg->byBuffer[iCount]=0x05;
				checksum ^= 0x05;
			}
			else
			{
				msg->byBuffer[iCount]=byTmpBuffer[i];
				checksum ^= byTmpBuffer[i];
			}
			iCount++;
		}

		msg->byBuffer[iCount]=checksum;
		iCount++;


		msg->bySize=iCount;


		send(msg);

		
	}
	return 0;
}

/**********************************************************************************************//**
 * Sets module configuration
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	byte0	The byte 0.
 * \param	value	The value.
 **************************************************************************************************/

void CInterfaceCO2_MICROPOD::setModuleConfig(BYTE byte0, WORD value)
{
	BYTE byte2=LOBYTE(value);
	BYTE byte1=HIBYTE(value);
	SendCO2Command(CO2_MICROPOD_HOSTCMD_SETMODULECONFIG,0x04,byte0, byte1, byte2);
}

/**********************************************************************************************//**
 * Gets module configuration
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	byte0	The byte 0.
 **************************************************************************************************/

void CInterfaceCO2_MICROPOD::getModuleConfig(BYTE byte0)
{
	SendCO2Command(CO2_MICROPOD_HOSTCMD_GETMODULCONFIG,0x02,byte0);
}

/**********************************************************************************************//**
 * Sets module setup
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	byte0	The byte 0.
 * \param	byte1	The first byte.
 * \param	byte2	The second byte.
 * \param	byte3	The third byte.
 **************************************************************************************************/

void CInterfaceCO2_MICROPOD::setModuleSetup(BYTE byte0, BYTE byte1, BYTE byte2, BYTE byte3)
{
	SendCO2Command(CO2_MICROPOD_HOSTCMD_SETMODULESETUP,0x05,byte0, byte1, byte2, byte3);
}

/**********************************************************************************************//**
 * Gets module setup
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CInterfaceCO2_MICROPOD::getModuleSetup()
{
	SendCO2Command(CO2_MICROPOD_HOSTCMD_GETMODULESETUP,0x01);
}

/**********************************************************************************************//**
 * Gets module version
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CInterfaceCO2_MICROPOD::getModuleVersion()
{
	SendCO2Command(CO2_MICROPOD_HOSTCMD_GETVERSION,0x01);
}

/**********************************************************************************************//**
 * Gets breath message
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CInterfaceCO2_MICROPOD::getBreathMessage()
{
	SendCO2Command(CO2_MICROPOD_HOSTCMD_GETBREATHMSG,0x01);
}

/**********************************************************************************************//**
 * Gets extended state
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CInterfaceCO2_MICROPOD::getExtendedState()
{
	SendCO2Command(CO2_MICROPOD_HOSTCMD_GETEXTSTATE,0x01);
}

/**********************************************************************************************//**
 * Gets operating mode
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CInterfaceCO2_MICROPOD::getOperatingMode()
{
	SendCO2Command(CO2_MICROPOD_HOSTCMD_GETOPMODE,0x01);
}

/**********************************************************************************************//**
 * Sets operating mode
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	byte0	The byte 0.
 **************************************************************************************************/

void CInterfaceCO2_MICROPOD::setOperatingMode(BYTE byte0)
{
	SendCO2Command(CO2_MICROPOD_HOSTCMD_CHANGEOPMODE,0x02,byte0);
}

/**********************************************************************************************//**
 * Query if this instance is pump on
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if pump on, false if not.
 **************************************************************************************************/

bool CInterfaceCO2_MICROPOD::isPumpOn()
{
	return m_bPumpStateOn;
}

/**********************************************************************************************//**
 * Query if this instance is co2 value valid
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if the co2 value is valid, false if not.
 **************************************************************************************************/

bool CInterfaceCO2_MICROPOD::isCO2ValueValid()
{
	return m_bCO2ValueValid;
}

/**********************************************************************************************//**
 * Query if this instance is etco2 value valid
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if the etco2 value is valid, false if not.
 **************************************************************************************************/

bool CInterfaceCO2_MICROPOD::isETCO2ValueValid()
{
	return m_bETCO2ValueValid;
}

/**********************************************************************************************//**
 * Query if this instance is frequency value valid
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if the frequency value is valid, false if not.
 **************************************************************************************************/

bool CInterfaceCO2_MICROPOD::isFreqValueValid()
{
	return m_bFreqValuevalid;
}

/**********************************************************************************************//**
 * Query if this instance is fico2 value valid
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if the fico2 value is valid, false if not.
 **************************************************************************************************/

bool CInterfaceCO2_MICROPOD::isFICO2ValueValid()
{
	return m_bFICO2ValueValid;
}

/**********************************************************************************************//**
 * Sets the configuration
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CInterfaceCO2_MICROPOD::set_Configuration()
{
	//m_iAmbientPressure=getModel()->getCONFIG()->GetCO2BaroPressure();
	m_dtCalibrationDateTime.SetStatus(COleDateTime::null);
	m_iMaxOpHRSCalibration=-1;
	m_iRemainServiceHrs=-1;
	m_bCalibrationIsDue=false;
	m_bServiceIsDue=false;
	m_bSystemDateOk=true;


	getModel()->getDATAHANDLER()->resetCO2MessureData();
	getModel()->getSPI()->ResetCO2Value();

	set_restartBreathAlgorithm();

	if(m_bVersion==false)
		getModuleVersion();

	setModuleConfig(CO2_MICROPOD_MODULECONFIG_WAVEMEASINTERVAL, m_cfgWAVEMEASINTERVAL);	//25ms
	setModuleConfig(CO2_MICROPOD_MODULECONFIG_BTPS, m_cfgBTPS);
	setModuleConfig(CO2_MICROPOD_MODULECONFIG_PEAKPICK, m_cfgPEAKPICK);
	setModuleConfig(CO2_MICROPOD_MODULECONFIG_DISTINGBREATHMSG, m_cfgDISTINGBREATHMSG);
	setModuleConfig(CO2_MICROPOD_MODULECONFIG_CO2UNITS, m_cfgCO2UNITS);
	setModuleConfig(CO2_MICROPOD_MODULECONFIG_PERIODICBREATHMSG, m_cfgPERIODICBREATHMSG);	//1 second
	setModuleConfig(CO2_MICROPOD_MODULECONFIG_AUTOSTANDBY, m_cfgAUTOSTANDBY);

	if(		getModel()->getCONFIG()->GetVentRange()==NEONATAL
		/*||	getModel()->getDATAHANDLER()->activeModeIsHFO()*/)
	{
		set_PatientMode(2);
	}
	else
	{
		set_PatientMode(3);
	}

	setModuleConfig(CO2_MICROPOD_MODULECONFIG_CO2RANGE, m_cfgCO2RANGE);	//0-150mmHg

	//if(getModel()->getDATAHANDLER()->activeModeIsHFO())
	//{
	//	set_MeasurementMode(CO2_MICROPOD_MEASUREMODE_CONTCO2);	//ContCO2
	//}
	//else
	{
		set_MeasurementMode(CO2_MICROPOD_MEASUREMODE_NORMAL);	//normal
	}

	getRemainTimeToCal();
	get_LastCalDate();
	getServiceHrs();
}

/**********************************************************************************************//**
 * Sets standby mode
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	state	True to state.
 **************************************************************************************************/

void CInterfaceCO2_MICROPOD::set_StandbyMode(bool state)
{
	bool bChanged=false;
	if(m_bStandby!=state)
		bChanged=true;
	m_bStandby=state;
	if(state)
	{
		m_cfgOPERATINGMODE=CO2_MICROPOD_OPMODE_STANDBY;
		set_restartBreathAlgorithm();
	}
	else
		m_cfgOPERATINGMODE=CO2_MICROPOD_OPMODE_MEASUREMENT;

	setOperatingMode(m_cfgOPERATINGMODE);

	if(bChanged)
		g_eventCO2Data.SetEvent();
}

/**********************************************************************************************//**
 * Gets standby mode
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CInterfaceCO2_MICROPOD::get_StandbyMode()
{
	return m_bStandby;
}

/**********************************************************************************************//**
 * Sets pump state on
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	bGetExState	True to get ex state.
 **************************************************************************************************/

void CInterfaceCO2_MICROPOD::set_PumpStateOn(bool bGetExState)
{
	SendCO2Command(CO2_MICROPOD_HOSTCMD_PUMPOFF,0x02,0x02);
	if(bGetExState)
		getExtendedState();
}

/**********************************************************************************************//**
 * Sets pump state off
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	bGetExState	True to get ex state.
 **************************************************************************************************/

void CInterfaceCO2_MICROPOD::set_PumpStateOff(bool bGetExState)
{
	DEBUGMSG(TRUE, (TEXT("set_PumpStateOff\r\n")));
	SendCO2Command(CO2_MICROPOD_HOSTCMD_PUMPOFF,0x02,0x01);
	if(bGetExState)
		getExtendedState();
}

/**********************************************************************************************//**
 * Sets restart breath algorithm
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CInterfaceCO2_MICROPOD::set_restartBreathAlgorithm()
{
	SendCO2Command(CO2_MICROPOD_HOSTCMD_RESTARTBREATHALG,0x01);
}

/**********************************************************************************************//**
 * Query if this instance is calibration due
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if calibration due, false if not.
 **************************************************************************************************/

bool CInterfaceCO2_MICROPOD::isCalibrationDue()
{
	return m_bCalibrationIsDue;
}

/**********************************************************************************************//**
 * Gets maximum operation hrs calibration
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The maximum operation hrs calibration.
 **************************************************************************************************/

int CInterfaceCO2_MICROPOD::get_MaxOpHRSCalibration()
{
	return m_iMaxOpHRSCalibration;
}

/**********************************************************************************************//**
 * Gets calibration date time
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The calibration date time.
 **************************************************************************************************/

COleDateTime CInterfaceCO2_MICROPOD::get_CalibrationDateTime()
{
	return m_dtCalibrationDateTime;
}

/**********************************************************************************************//**
 * Gets remain service hrs
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The remain service hrs.
 **************************************************************************************************/

int CInterfaceCO2_MICROPOD::get_RemainServiceHrs()
{
	return m_iRemainServiceHrs;
}

/**********************************************************************************************//**
 * Query if this instance is service due
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if service due, false if not.
 **************************************************************************************************/

bool CInterfaceCO2_MICROPOD::isServiceDue()
{
	return m_bServiceIsDue;
}

/**********************************************************************************************//**
 * Query if this instance is system date ok
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if system date ok, false if not.
 **************************************************************************************************/

bool CInterfaceCO2_MICROPOD::isSystemDateOk()
{
	return m_bSystemDateOk;
}
