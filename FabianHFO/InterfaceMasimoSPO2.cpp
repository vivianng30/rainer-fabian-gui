/**********************************************************************************************//**
 * \file	InterfaceMasimoSPO2.cpp.
 *
 * Implements the interface masimo spo 2 class
 **************************************************************************************************/

#include "StdAfx.h"
#include "InterfaceMasimoSPO2.h"
#include "MVModel.h"
#include "MVViewHandler.h"

/**********************************************************************************************//**
 * Initializes a new instance of the CInterfaceMasimoSPO2 class
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

CInterfaceMasimoSPO2::CInterfaceMasimoSPO2(void)
{
	m_iSpO2=0;
	m_iPulseRate=0;
	m_iPerfusionIndex=0;

	fdwRecvBufLen = 0;
	feRecvState = SPO2_MASIMO_SOM;
	fwCalcChkSum = 0;
	fwReadChkSum=0;
	m_byCmdBuffer[0]=0x0000;
	
	faRecvBuf[0]=0x0000;
	faDataBuf[0]=0x0000;
	faWaveBuf[0]=0x0000;

	m_iProductID=0;
	m_iVersionDSP=0;
	m_iVersionMCU=0;

	fwDataSequence=0;
	fwPrevSequence=0;//rkuNEWFIX

	m_cLastWaveValW1=0;
	m_cLastWaveValW0=0;

	m_iEXCEPTIONS2=0;
	m_iSENSORFAILURE=0;

	m_bProcessing=false;
	
	m_bFastSATon=false;
	m_eSPO2SensitivityMode=SPO2_SENSITIVITY_NORMAL;
	m_eSPO2alarmdelay=SPO2_ADELAY_10;
	m_eSPO2averagingTime=SPO2_AVERAGING_8;

	m_cfgBoardMode=0;						
	m_cfgDigitalWaveformConfiguration_WC0=0;
	m_cfgDigitalWaveformConfiguration_WC1=0;
	m_cfgSpO2AveragingTime=(BYTE)m_eSPO2averagingTime;
	m_cfgAlgorithmMode=0;					
	m_cfgWaveformMode=0;	

	switch(m_eSPO2SensitivityMode)
	{
	case SPO2_SENSITIVITY_NORMAL:
		{
			//setAlgorithmMode(true, m_bFastSATon, false);
			m_cfgAlgorithmMode=getModel()->setBitOfByte(m_cfgAlgorithmMode, 0);
		}
		break;
	case SPO2_SENSITIVITY_MAXIMUM:
		{
			//setAlgorithmMode(false, m_bFastSATon, false);
		}
		break;
	case SPO2_SENSITIVITY_APOD:
		{
			//setAlgorithmMode(true, m_bFastSATon, true);
			m_cfgAlgorithmMode=getModel()->setBitOfByte(m_cfgAlgorithmMode, 0);
			m_cfgAlgorithmMode=getModel()->setBitOfByte(m_cfgAlgorithmMode, 2);
		}
		break;
	}

	if(m_bFastSATon)
		m_cfgAlgorithmMode=getModel()->setBitOfByte(m_cfgAlgorithmMode, 1);

	m_bCheck_BoardMode=false;
	m_bCheck_DigitalWaveformConfigurationWC0=false;
	m_bCheck_SpO2AveragingTime=false;
	m_bCheck_AlgorithmMode=false;
	m_bCheck_tWaveformMode=false;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CInterfaceMasimoSPO2 class
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

CInterfaceMasimoSPO2::~CInterfaceMasimoSPO2(void)
{
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

int CInterfaceMasimoSPO2::Init(BYTE com)
{
	m_bSensorConnectedToUSB=false;

	m_bFastSATon=getModel()->getCONFIG()->isFastSATon();
	m_eSPO2SensitivityMode=getModel()->getCONFIG()->getSensitivityMode();
	m_eSPO2alarmdelay=getModel()->getCONFIG()->getSPO2alarmDelay();
	m_eSPO2averagingTime=getModel()->getCONFIG()->getSPO2averagingTime();

	m_cfgSpO2AveragingTime=(BYTE)m_eSPO2averagingTime;

	//m_iCOMport=com;

	//LONG    lLastError = ERROR_SUCCESS;

	//// Attempt to open the serial port (COM1)
	//switch(com)
	//{
	//case 9:
	//	{
	//		lLastError = Open(_T("COM9:"),0,0);
	//	}
	//	break;
	//default:
	//	{
	//		//error
	//		return 1;
	//	}
	//	break;
	//}
	//

	//if (lLastError != ERROR_SUCCESS)
	//	return ShowError(GetLastError(), _T("Unable to open COM-port"));

	//// Setup the serial port (9600,8N1, which is the default setting)
	//lLastError = Setup(CSerial::EBaud9600,CSerial::EData8,CSerial::EParNone,CSerial::EStop1);
	//if (lLastError != ERROR_SUCCESS)
	//	return ShowError(GetLastError(), _T("Unable to set COM-port setting"));


	startSPO2initThread();

	//StartListener();

	return 0;
}

/**********************************************************************************************//**
 * Deinits
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	bReinit	True to reinitialize.
 *
 * \return	An int.
 **************************************************************************************************/

int CInterfaceMasimoSPO2::Deinit(bool bReinit)
{
	//DWORD dwStart=GetTickCount();
	
	//DEBUGMSG(TRUE, (TEXT("CInterfaceMasimoSPO2::Deinit()\r\n")));
	//m_binitialized=false;
	m_bProcessing=false;
	m_bSensorConnectedToUSB=false;
	
	StopListener(-1);

	/*DWORD dwEnd=GetTickCount();
	DEBUGMSG(TRUE, (TEXT("CInterfaceMasimoSPO2::Deinit1 %d\r\n"),dwEnd-dwStart));*/
	/*dwEnd=GetTickCount();
	DEBUGMSG(TRUE, (TEXT("CInterfaceMasimoSPO2::Deinit2 %d\r\n"),dwEnd-dwStart));*/
	Sleep(0);
	stopSPO2initThread();

	stopSPO2checkThread();

	stopSPO2sendThread();

	/*dwEnd=GetTickCount();
	DEBUGMSG(TRUE, (TEXT("CInterfaceMasimoSPO2::Deinit3 %d\r\n"),dwEnd-dwStart));*/

	getModel()->getDATAHANDLER()->resetSPO2MessureData();

	/*dwEnd=GetTickCount();
	DEBUGMSG(TRUE, (TEXT("CInterfaceMasimoSPO2::Deinit4 %d\r\n"),dwEnd-dwStart));*/

	getModel()->getSPI()->ResetSPO2Value();

	/*dwEnd=GetTickCount();
	DEBUGMSG(TRUE, (TEXT("CInterfaceMasimoSPO2::Deinit5 %d\r\n"),dwEnd-dwStart));*/

	m_iSpO2=0;
	m_iPulseRate=0;

	//getModel()->getDATAHANDLER()->setMessureDataAVG(ALINK_MSMNT_SPO2_PR,m_iPulseRate);
	//getModel()->getDATAHANDLER()->setMessureDataAVG(ALINK_MSMNT_SPO2,m_iSpO2);
	getModel()->getDATAHANDLER()->writeSpO2SIQ(0);

	/*dwEnd=GetTickCount();
	DEBUGMSG(TRUE, (TEXT("CInterfaceMasimoSPO2::Deinit6 %d\r\n"),dwEnd-dwStart));*/

	/*if (getModel()->getAcuLink()!=NULL)
	{
		getModel()->getAcuLink()->SetMessurementData(ALINK_MSMNT_ETCO2,ALINK_NOTVALID);
	}*/
	if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_SPO2GRAPHS)
		g_eventSPO2Data.SetEvent();

	//StopListener(-1);

	/*dwEnd=GetTickCount();
	DEBUGMSG(TRUE, (TEXT("CInterfaceMasimoSPO2::Deinit7 %d\r\n"),dwEnd-dwStart));*/

	Close();

	if(!bReinit)
		theApp.WriteLog(_T("#SPO2: DEINIT"));
	//DEBUGMSG(TRUE, (TEXT("#SPO2: DEINIT\r\n")));
	/*dwEnd=GetTickCount();
	DEBUGMSG(TRUE, (TEXT("CInterfaceMasimoSPO2::Deinit8 %d\r\n"),dwEnd-dwStart));*/
	return 0;
}

/**********************************************************************************************//**
 * Opens the com
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CInterfaceMasimoSPO2::OpenCOM()
{
	bool bRes=false;


	LONG    lLastError = Open(_T("COM9:"),0,0);
	

	if (lLastError != ERROR_SUCCESS)
	{
		if(ERROR_ALREADY_INITIALIZED==lLastError)
		{
			
		}
		//return ShowError(GetLastError(), _T("Unable to open COM-port"));
		return bRes;
	}

	// Setup the serial port (9600,8N1, which is the default setting)
	lLastError = Setup(CSerial::EBaud9600,CSerial::EData8,CSerial::EParNone,CSerial::EStop1);
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

	DEBUGMSG(TRUE, (TEXT("#SPO2: OpenCOM\r\n")));
	theApp.WriteLog(_T("#SPO2: OpenCOM"));

	return bRes;

}

/**********************************************************************************************//**
 * Shows the error
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	lError			The error.
 * \param	lptszMessage	Message describing the lptsz.
 *
 * \return	An int.
 **************************************************************************************************/

int CInterfaceMasimoSPO2::ShowError (LONG lError, LPCTSTR lptszMessage)
{
	// Generate a message text
	TCHAR tszMessage[256];
	wsprintf(tszMessage,_T("#HFO:0111: %s (code %d)"), lptszMessage, lError);

	theApp.WriteLog(tszMessage);
	// Display message-box and return with an error-code
	//::MessageBox(0,tszMessage,_T("Listener"), MB_ICONSTOP|MB_OK);
	return 1;
}
//void CInterfaceMasimoSPO2::StartupSequenz()
//{
//	StartListener();
//	
//	setBoardMode(SPO2_MASIMO_HOSTCMD_BOARDMODE_NORMAL);
//}

/**********************************************************************************************//**
 * Initializes the sequenz
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CInterfaceMasimoSPO2::InitializeSequenz()
{
	//StartListener();

	setBoardMode(SPO2_MASIMO_HOSTCMD_BOARDMODE_NORMAL);
	//setDigitalWaveformConfiguration_WC0(SPO2_MASIMO_HOSTCMD_WAVEFORM_SOURCE_RAWIR, SPO2_MASIMO_HOSTCMD_WAVEFORM_SCALING_CONTINUOUS_AUTO, SPO2_MASIMO_HOSTCMD_WAVEFORM_DATASIZE_8BIT);
	//setDigitalWaveformConfiguration_WC1(SPO2_MASIMO_HOSTCMD_WAVEFORM_SOURCE_RAWIR, SPO2_MASIMO_HOSTCMD_WAVEFORM_SCALING_CONTINUOUS_AUTO, SPO2_MASIMO_HOSTCMD_WAVEFORM_DATASIZE_8BIT);
	//setDigitalWaveformConfigurationWC0(SPO2_MASIMO_HOSTCMD_WAVEFORM_SOURCE_RAWIR, 0x00, SPO2_MASIMO_HOSTCMD_WAVEFORM_DATASIZE_8BIT);
	sendDigitalWaveformConfigurations();

	//setSpO2AveragingTime(SPO2_MASIMO_HOSTCMD_SPO2AVERAGINGTIME_8);
	setSpO2AveragingTime(m_cfgSpO2AveragingTime);

	switch(m_eSPO2SensitivityMode)
	{
	case SPO2_SENSITIVITY_NORMAL:
		{
			setAlgorithmMode(true, m_bFastSATon, false);
		}
		break;
	case SPO2_SENSITIVITY_MAXIMUM:
		{
			setAlgorithmMode(false, m_bFastSATon, false);
		}
		break;
	case SPO2_SENSITIVITY_APOD:
		{
			setAlgorithmMode(true, m_bFastSATon, true);
		}
		break;
	}

	setWaveformMode(SPO2_MASIMO_HOSTCMD_WAVEFORMMODE_RESPSIG_FILTEREDOUT);

	startSPO2checkThread();
}

/**********************************************************************************************//**
 * Check module state
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CInterfaceMasimoSPO2::checkModuleState()
{
	checkLastSpO2Cmd();
}

/**********************************************************************************************//**
 * Performs the message action
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CInterfaceMasimoSPO2::PerformMsg()
{
	CString szError=_T("");
	bool bNewData=false;
	
	updateLastSpO2Cmd();

#ifndef SIMULATION_ENTREK
	if(fwPrevSequence<62)
	{
		if(fwDataSequence==0 && fwPrevSequence==0)
		{
			//startup phase
		}
		else if(fwDataSequence!=fwPrevSequence+1)
		{
			DEBUGMSG(TRUE, (TEXT("ERROR seq %d %d\r\n"),fwPrevSequence,fwDataSequence));
			CStringW sz=_T("");
			sz.Format(_T("#SPO:1seq %d %d"),fwPrevSequence,fwDataSequence);
			theApp.WriteLog(sz);

			/*if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_REINIT_SPO2_MODULE);*/
		}
		fwPrevSequence=fwDataSequence;
	}
	else
	{
		if(fwDataSequence!=0 && fwPrevSequence!=62)
		{
			DEBUGMSG(TRUE, (TEXT("ERROR seq %d %d\r\n"),fwPrevSequence,fwDataSequence));
			CStringW sz=_T("");
			sz.Format(_T("#SPO:2seq %d %d"),fwPrevSequence,fwDataSequence);
			theApp.WriteLog(sz);

			/*if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_REINIT_SPO2_MODULE);*/
		}
		else if(fwDataSequence==SPO2_MASIMO_SLAVECMD_BOARD_FAILURECODES && fwPrevSequence==SPO2_MASIMO_SLAVECMD_BOARD_FAILURECODES)
		{
			DEBUGMSG(TRUE, (TEXT("ERROR SPO2_MASIMO_SLAVECMD_BOARD_FAILURECODES\r\n")));
			theApp.WriteLog(_T("#SPO2_MASIMO_SLAVECMD_BOARD_FAILURECODES"));
		}
		fwPrevSequence=fwDataSequence;
	}
#endif

	switch(fwDataSequence)
	{
		//slave messages
		case SPO2_MASIMO_SLAVECMD_SPO2:
			{
				//PBUFSPO2 _pBuf;
				SHORT iSpO2=MAKEWORD(faDataBuf[1],faDataBuf[0]);
				//DEBUGMSG(TRUE, (TEXT("SPO2_MASIMO_SLAVECMD_SPO2\r\n")));
				if(m_iSpO2!=iSpO2)
				{
					bNewData=true;
					m_iSpO2=iSpO2;
					getModel()->getDATAHANDLER()->setMessureDataAVG(ALINK_MSMNT_SPO2,m_iSpO2);
				}
				if(bNewData && getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_SPO2GRAPHS)
					g_eventSPO2Data.SetEvent();

				if(getModel()->getPRICOThread())
				{
					if(getModel()->getPRICOThread()->isPRICOalgorithmRunning())
					{
						getModel()->getPRICOThread()->setMeasuredSpO2Value(m_iSpO2);
					}
				}
			}
			break;
		case SPO2_MASIMO_SLAVECMD_PULSERATE:
			{
				SHORT iPulseRate=MAKEWORD(faDataBuf[1],faDataBuf[0]);
				
				if(m_iPulseRate!=iPulseRate)
				{
					bNewData=true;
					m_iPulseRate=iPulseRate;
					getModel()->getDATAHANDLER()->setMessureDataAVG(ALINK_MSMNT_SPO2_PR,m_iPulseRate);
				}
				if(bNewData  && getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_SPO2GRAPHS)
					g_eventSPO2Data.SetEvent();
			}
			break;
		case SPO2_MASIMO_SLAVECMD_PERFUSIONINDEX:
			{
				SHORT iPerfusionIndex=MAKEWORD(faDataBuf[1],faDataBuf[0]);
				
				if(m_iPerfusionIndex!=iPerfusionIndex)
				{
					bNewData=true;
					m_iPerfusionIndex=iPerfusionIndex;
					getModel()->getDATAHANDLER()->setMessureDataAVG(ALINK_MSMNT_SPO2_PI,m_iPerfusionIndex);
				}
				if(bNewData  && getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_SPO2GRAPHS)
					g_eventSPO2Data.SetEvent();
			}
			break;
		case SPO2_MASIMO_SLAVECMD_EXCEPTIONS1:
			{
			}
			break;
		case SPO2_MASIMO_SLAVECMD_SENSORTYPE:
			{

			}
			break;
		case SPO2_MASIMO_SLAVECMD_WAVEFORMMODE:
			{
				
			}
			break;
		case SPO2_MASIMO_SLAVECMD_EXCEPTIONS2:
			{
				SHORT iError=MAKEWORD(faDataBuf[1],faDataBuf[0]);

				m_iEXCEPTIONS2=MAKEWORD(faDataBuf[1],faDataBuf[0]);

				checkEXCEPTIONS2();
			}
			break;
		case SPO2_MASIMO_SLAVECMD_PRODUCTIDENTIFICATION:
			{
				WORD iProductID=MAKEWORD(faDataBuf[1],faDataBuf[0]);

				if(m_iProductID!=iProductID)
				{
					m_iProductID=iProductID;
				}
			}
			break;
		case SPO2_MASIMO_SLAVECMD_DSP_FIRMWARE:
			{
				WORD iTempDSP=MAKEWORD(faDataBuf[1],faDataBuf[0]);
				if(m_iVersionDSP!=iTempDSP)
				{
					m_iVersionDSP=iTempDSP;
					/*WORD iPatch = (iTempDSP & 0xF000) >> 12;
					WORD iMinor = (iTempDSP & 0x0F00) >> 8;
					WORD iMajor = (iTempDSP & 0x00F0) >> 4;
					WORD iVersion = (iTempDSP & 0x000F);
					
					m_szVersionDSP.Format(_T("DSP: V%d.%d.%d.%d"),iVersion, iMajor,iMinor,iPatch);*/
				}
				
			}
			break;
		case SPO2_MASIMO_SLAVECMD_MCU_FIRMWARE:
			{
				WORD iTempMCU=MAKEWORD(faDataBuf[1],faDataBuf[0]);
				if(m_iVersionMCU!=iTempMCU)
				{
					m_iVersionMCU=iTempMCU;
					/*WORD iPatch = (iTempMCU & 0xF000) >> 12;
					WORD iMinor = (iTempMCU & 0x0F00) >> 8;
					WORD iMajor = (iTempMCU & 0x00F0) >> 4;
					WORD iVersion = (iTempMCU & 0x000F);

					m_szVersionMCU.Format(_T("MCU: V%d.%d.%d.%d"),iVersion, iMajor,iMinor,iPatch);*/
				}

			
			}
			break;
		case SPO2_MASIMO_SLAVECMD_HARDWAREREVISION:
			{
				
			}
			break;
		case SPO2_MASIMO_SLAVECMD_SENSORFAILURE:
			{
				SHORT iError=MAKEWORD(faDataBuf[1],faDataBuf[0]);

				m_iSENSORFAILURE=MAKEWORD(faDataBuf[1],faDataBuf[0]);

				checkSENSORFAILURE();
			}
			break;
		case SPO2_MASIMO_SLAVECMD_BOARDMODE:
			{
				SHORT iMode=MAKEWORD(faDataBuf[1],faDataBuf[0]);

				if(m_cfgBoardMode!=iMode)
				{
					SendSPO2Command(SPO2_MASIMO_HOSTCMD_BOARDMODE, m_cfgBoardMode, 0x00, 0x00, 0x00, 0x00, 0x00);
				}

				
			}
			break;
		case SPO2_MASIMO_SLAVECMD_WAVEFORM_0_1:
			{
				BYTE iConfig0=faDataBuf[0];
				BYTE iConfig1=faDataBuf[1];

				bool bResend=false;

				if(m_cfgDigitalWaveformConfiguration_WC0!=iConfig0)
				{
					 bResend=true;
				}
				else if(m_cfgDigitalWaveformConfiguration_WC1!=iConfig1)
				{
					bResend=true;
				}

				if(bResend)
				{
					sendDigitalWaveformConfigurations();
					//SendSPO2Command(SPO2_MASIMO_HOSTCMD_WAVEFORMCONFIG, m_cfgDigitalWaveformConfiguration_WC0,m_cfgDigitalWaveformConfiguration_WC1);
				}

				
			}
			break;
		case SPO2_MASIMO_SLAVECMD_WAVEFORM_2_3:
			{

			}
			break;
		case SPO2_MASIMO_SLAVECMD_WAVEFORM_4_5:
			{

			}
			break;
		case SPO2_MASIMO_SLAVECMD_SPO2AVERAGINGTIME:
			{
				SHORT iMode=MAKEWORD(faDataBuf[1],faDataBuf[0]);

				if(m_cfgSpO2AveragingTime!=iMode)
				{
					SendSPO2Command(SPO2_MASIMO_HOSTCMD_SPO2AVERAGINGTIME, m_cfgSpO2AveragingTime);
				}
			}
			break;
		case SPO2_MASIMO_SLAVECMD_ALGORITHMMODE:
			{
				BYTE iMode=faDataBuf[1];

				if(m_cfgAlgorithmMode!=iMode)
				{
					SendSPO2Command(SPO2_MASIMO_HOSTCMD_ALGORITHMMODE, 0x00, m_cfgAlgorithmMode);
				}
			}
			break;
		case SPO2_MASIMO_SLAVECMD_PVI:
			{

			}
			break;
		case SPO2_MASIMO_SLAVECMD_ALARMSPO2:
			{

			}
			break;
		case SPO2_MASIMO_SLAVECMD_DIAGNOSTIC_FAILURECODES:
			{

			}
			break;
		case SPO2_MASIMO_SLAVECMD_BOARD_FAILURECODES:
			{

			}
			break;
		default:
			{
				

			}
			break;
	}

	SHORT iTempfaWaveBuf0=faWaveBuf[0]*(-100);

	if(m_cLastWaveValW0!=iTempfaWaveBuf0)
	{
		doInterpolationW0(iTempfaWaveBuf0);
	}

	double dbTempW1=faWaveBuf[1];
	SHORT iTempfaWaveBuf1=(SHORT)((dbTempW1/127)*100);

	if(m_cLastWaveValW1!=iTempfaWaveBuf1)
	{
		m_cLastWaveValW1=iTempfaWaveBuf1;
		//DEBUGMSG(TRUE, (TEXT("IQ %d\r\n"),m_cLastWaveValW1));
		if(isStateOk() && m_cLastWaveValW1>0)
			getModel()->getDATAHANDLER()->writeSpO2SIQ(m_cLastWaveValW1);
		/*else if(!m_bProcessing)
			getModel()->getDATAHANDLER()->writeSpO2SIQ(0);*/
	}

	
	
	
	return true;
}

/**********************************************************************************************//**
 * Gets exception 2
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The exception 2.
 **************************************************************************************************/

SHORT CInterfaceMasimoSPO2::get_EXCEPTION2()
{
	return m_iEXCEPTIONS2;
}

/**********************************************************************************************//**
 * Check sensorfailure
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CInterfaceMasimoSPO2::checkSENSORFAILURE()
{
	if(m_iSENSORFAILURE & BIT0)	// Flow-Sensor nicht angeschlossen
	{
		/*if(		getALARMHANDLER()->CanSetAlarm_FLOW_SENSOR_NOTCONNECTED()
			&&	getDATAHANDLER()->IsFlowSensorStateOff()==false)
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_FLOW_SENSOR_NOTCONNECTED);
		}*/
	}
}

/**********************************************************************************************//**
 * Check exceptions 2
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CInterfaceMasimoSPO2::checkEXCEPTIONS2()
{
	bool bCheckSensor=false;
	bool bSensorNotConnected=false;
	bool bProcessingActive=false;
	bool bPulseSearch=false;

	if(m_iEXCEPTIONS2 & BIT0)	// no sensor connected
	{
		bSensorNotConnected=true;
	}

	if(m_iEXCEPTIONS2 & BIT1)	// defective sensor
	{
		if(getModel()->getALARMHANDLER()->ALARM_Sens_SPO2_SENSORFAULTY->getAlarmState()==AS_NONE)
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_SPO2_SENSORFAULTY);
		}
	}

	if(m_iEXCEPTIONS2 & BIT2)	// low perfusion
	{
		bCheckSensor=true;
	}

	if(m_iEXCEPTIONS2 & BIT3)	// pulse search
	{
		bPulseSearch=true;
	}

	if(m_iEXCEPTIONS2 & BIT4)	// interference detected
	{
		bCheckSensor=true;
	}

	if(m_iEXCEPTIONS2 & BIT5)	// sensor off patient
	{
		bCheckSensor=true;
	}

	if(m_iEXCEPTIONS2 & BIT6)	// too much ambient light
	{
		bCheckSensor=true;
	}

	if(m_iEXCEPTIONS2 & BIT7)	// unrecognized sensor
	{
		bCheckSensor=true;
	}

	if(m_iEXCEPTIONS2 & BIT10)	// low signal IQ
	{
		bCheckSensor=true;
	}

	if(m_iEXCEPTIONS2 & BIT11)	// processing active
	{
		bProcessingActive=true;
	}

	if(m_iEXCEPTIONS2 & BIT12)	// no cable connected
	{
		bSensorNotConnected=true;
	}

	if(m_iEXCEPTIONS2 & BIT13)	// no adhesive sensor connected
	{
		bCheckSensor=true;
	}

	//if(m_iEXCEPTIONS2 & BIT14)	// demo mode
	//{
	//}

	/*if(m_iEXCEPTIONS2==0)
	{
		
	}*/

	bool bSetEvent=false;

	if(bSensorNotConnected)
	{
		if(getModel()->getALARMHANDLER()->ALARM_Sens_SPO2_MODULE_NOTCONNECTED->getAlarmState()==AS_NONE)
		{
			m_bSensorConnectedToModule=false;
			
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_SPO2_MODULE_NOTCONNECTED);

			m_iSpO2=0;
			m_iPulseRate=0;
			m_iPerfusionIndex=0;

			getModel()->getDATAHANDLER()->setMessureDataAVG(ALINK_MSMNT_SPO2_PR,m_iPulseRate);
			getModel()->getDATAHANDLER()->setMessureDataAVG(ALINK_MSMNT_SPO2,m_iSpO2);
			getModel()->getDATAHANDLER()->setMessureDataAVG(ALINK_MSMNT_SPO2_PI,m_iPerfusionIndex);

			bSetEvent=true;
			//g_eventSPO2Data.SetEvent();
		}
	}
	else
	{
		m_bSensorConnectedToModule=true;

		if(getModel()->getALARMHANDLER()->ALARM_Sens_SPO2_MODULE_NOTCONNECTED->getAlarmState()==AS_ACTIVE && bSensorNotConnected==false && m_bSensorConnectedToUSB==true)
		{
			getModel()->getALARMHANDLER()->deleteAlarm(AL_Sens_SPO2_MODULE_NOTCONNECTED);

			bSetEvent=true;
			//g_eventSPO2Data.SetEvent();
		}

		if(bCheckSensor)
		{
			if(getModel()->getALARMHANDLER()->ALARM_Sens_SPO2_CHECKSENSOR->getAlarmState()==AS_NONE)
			{
				m_iSpO2=0;
				m_iPerfusionIndex=0;
				m_iPulseRate=0;

				getModel()->getDATAHANDLER()->setMessureDataAVG(ALINK_MSMNT_SPO2_PR,m_iPulseRate);
				getModel()->getDATAHANDLER()->setMessureDataAVG(ALINK_MSMNT_SPO2,m_iSpO2);
				getModel()->getDATAHANDLER()->setMessureDataAVG(ALINK_MSMNT_SPO2_PI,m_iPerfusionIndex);

				bSetEvent=true;
				//g_eventSPO2Data.SetEvent();

				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_SPO2_CHECKSENSOR);
			}
		}
		else if(getModel()->getALARMHANDLER()->ALARM_Sens_SPO2_CHECKSENSOR->getAlarmState()==AS_ACTIVE)
		{
			getModel()->getALARMHANDLER()->deleteAlarm(AL_Sens_SPO2_CHECKSENSOR);

			bSetEvent=true;
			//g_eventSPO2Data.SetEvent();
			
		}
	}

	if(		bCheckSensor==false
		&&	bSensorNotConnected==false
		&&	bPulseSearch==false
		&&	bProcessingActive==true
		&& m_iSpO2>0)
	{
		m_bProcessing=true;
	}
	else
	{
		if(m_bProcessing)
			getModel()->getDATAHANDLER()->writeSpO2SIQ(0);
		m_bProcessing=false;
	}
	
	if(bSetEvent  && getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_SPO2GRAPHS)
		g_eventSPO2Data.SetEvent();
}

/**********************************************************************************************//**
 * Executes the interpolation w0 operation
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	iTempfaWaveBuf	Buffer for temp wave data.
 **************************************************************************************************/

void CInterfaceMasimoSPO2::doInterpolationW0(SHORT iTempfaWaveBuf)
{
	SHORT iDiff=((iTempfaWaveBuf-m_cLastWaveValW0)/16);
	
	//SHORT iTemp=m_cLastWaveValW0;
	PBUFSPO2 _pBuf;
	_pBuf.iValSPO2=m_cLastWaveValW0;

	EnterCriticalSection(&getModel()->getDATAHANDLER()->csSPO2DataBuffer);

	for(int i=0;i<16;i++)
	{
		//faSpO2Interpolation[i]=_pBuf.iValSPO2;
		getModel()->getDATAHANDLER()->m_rbufSPO2.write(_pBuf);
		_pBuf.iValSPO2+=iDiff;
	}
	LeaveCriticalSection(&getModel()->getDATAHANDLER()->csSPO2DataBuffer);
	

	//getModel()->getDATAHANDLER()->writeSPO2waveData(faWaveBuf[0]*(-1));

	/*
	
	*/
	/*PBUFSPO2 _pBuf;
	_pBuf.iValSPO2=(faWaveBuf[0]*(-1));
	getModel()->getDATAHANDLER()->writeSPO2Buffer(_pBuf);*/

	m_cLastWaveValW0=iTempfaWaveBuf;
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

void CInterfaceMasimoSPO2::OnEvent(EEvent eEvent, EError eError)
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
				case SPO2_MASIMO_SOM:

					fwCalcChkSum = 0;
					fwReadChkSum=0;

					switch (faRecvBuf[i])
					{
					case SPO2_MASIMO_MSG_SOM:
						feRecvState = SPO2_MASIMO_SEQ;
						break;

					default:
						// forget the rest
						feRecvState = SPO2_MASIMO_SOM;
						return;
					} // switch (faRecvBuf[i])
					break;

				case SPO2_MASIMO_SEQ:
					
					fwCalcChkSum += c;
					fwDataSequence = c;
					feRecvState = SPO2_MASIMO_D1;
					break;

				case SPO2_MASIMO_D1:

					fwCalcChkSum += c;
					feRecvState = SPO2_MASIMO_D2;
					faDataBuf[0]=c;
					break;

				case SPO2_MASIMO_D2:

					fwCalcChkSum += c;
					feRecvState = SPO2_MASIMO_W0;
					faDataBuf[1]=c;
					break;

				case SPO2_MASIMO_W0:

					fwCalcChkSum += c;
					feRecvState = SPO2_MASIMO_W1;
					faWaveBuf[0]=c;
					break;
				case SPO2_MASIMO_W1:

					fwCalcChkSum += c;
					feRecvState = SPO2_MASIMO_W2;
					faWaveBuf[1]=c;
					break;

				case SPO2_MASIMO_W2:

					fwCalcChkSum += c;
					feRecvState = SPO2_MASIMO_W3;
					faWaveBuf[2]=c;
					break;

				case SPO2_MASIMO_W3:

					fwCalcChkSum += c;
					feRecvState = SPO2_MASIMO_W4;
					faWaveBuf[3]=c;
					break;

				case SPO2_MASIMO_W4:

					fwCalcChkSum += c;
					feRecvState = SPO2_MASIMO_W5;
					faWaveBuf[4]=c;
					break;

				case SPO2_MASIMO_W5:

					fwCalcChkSum += c;
					feRecvState = SPO2_MASIMO_W6;
					faWaveBuf[5]=c;
					break;

				case SPO2_MASIMO_W6:

					fwCalcChkSum += c;
					feRecvState = SPO2_MASIMO_W7;
					faWaveBuf[6]=c;
					break;

				case SPO2_MASIMO_W7:

					fwCalcChkSum += c;
					feRecvState = SPO2_MASIMO_CHKSUM;
					faWaveBuf[7]=c;
					break;

				case SPO2_MASIMO_CHKSUM:

					fwReadChkSum = c;
					if(fwReadChkSum == fwCalcChkSum)
					{
						this->PerformMsg();
					}
					else
					{
						
					}


					feRecvState = SPO2_MASIMO_EOM;
					break;

				case SPO2_MASIMO_EOM:

					switch (faRecvBuf[i])
					{
					case SPO2_MASIMO_MSG_EOM:
						feRecvState = SPO2_MASIMO_SOM;
						break;

					default:
						// forget the rest
						feRecvState = SPO2_MASIMO_SOM;
						return;
					} // switch (faRecvBuf[i])
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

//void CInterfaceMasimoSPO2::setModuleConfig(BYTE byte0, BYTE byte1, BYTE byte2, BYTE byte3, BYTE byte4, BYTE byte5, BYTE byte6)
//{
//	/*BYTE byte2=LOBYTE(value);
//	BYTE byte1=HIBYTE(value);*/
//	SendSPO2Command(byte0,,byte0, byte1, byte2);
//}
//void CInterfaceMasimoSPO2::getModuleConfig(BYTE byte0)
//{
//	//SendSPO2Command(cmd,byte0);
//}

/**********************************************************************************************//**
 * Sends a spo2 command
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	byte0	The byte 0.
 * \param	byte1	The first byte.
 * \param	byte2	The second byte.
 * \param	byte3	The third byte.
 * \param	byte4	The fourth byte.
 * \param	byte5	The fifth byte.
 * \param	byte6	The byte 6.
 *
 * \return	A DWORD.
 **************************************************************************************************/

DWORD CInterfaceMasimoSPO2::SendSPO2Command(BYTE byte0, BYTE byte1, BYTE byte2, BYTE byte3, BYTE byte4, BYTE byte5, BYTE byte6) 
{
	//if(getModel()->getDATAHANDLER()->IsCO2interfaceEnabled())
	{
		BYTE checksum=0;

		LPSPO2MSG msg = new SPO2MSG;

		BYTE byTmpBuffer[7];
		byTmpBuffer[0]=byte0;
		byTmpBuffer[1]=byte1;
		byTmpBuffer[2]=byte2;
		byTmpBuffer[3]=byte3;
		byTmpBuffer[4]=byte4;
		byTmpBuffer[5]=byte5;
		byTmpBuffer[6]=byte6;

		int iCount=0;
		msg->byBuffer[iCount]=0x02;
		iCount++;

		//int sum1 = 0;
		//int sum2 = 0;
		for(int i=0;i<NUM_DATABYTES_MASIMO_SPO2;i++)
		{
			msg->byBuffer[iCount]=byTmpBuffer[i];
			checksum += byTmpBuffer[i];
			//sum1 += byTmpBuffer[i];
			//sum2 += byTmpBuffer[i];
			iCount++;
		}

		
		//modulo 256 sum
		//sum1 %= 256;
		//sum2 &= 255;

		/*BYTE ch1 = sum1;
		BYTE ch2 = sum2;
		BYTE ch3 = checksum;*/

		

		msg->byBuffer[iCount]=checksum;
		iCount++;

		msg->byBuffer[iCount]=0x03;
		iCount++;

		msg->bySize=iCount;

		//int iCount=0;
		//msg->byBuffer[iCount]=0x85;xxxxxx
		//iCount++;

		//msg->byBuffer[iCount]=Anzahlbytes;	//command byte + data bytes
		//checksum ^= Anzahlbytes;
		//iCount++;

		//msg->byBuffer[iCount]=CO2_command;
		//checksum ^= CO2_command;
		//iCount++;

		//int iBytesReadin=Anzahlbytes-1;

		//for(int i=0;i<iBytesReadin;i++)
		//{
		//	if(byTmpBuffer[i]==0x80)
		//	{
		//		msg->byBuffer[iCount]=0x80;
		//		checksum ^= 0x80;
		//		iCount++;
		//		msg->byBuffer[iCount]=0x00;
		//		checksum ^= 0x00;
		//	}
		//	else if(byTmpBuffer[i]==0x85)
		//	{
		//		msg->byBuffer[iCount]=0x80;
		//		checksum ^= 0x80;
		//		iCount++;
		//		msg->byBuffer[iCount]=0x05;
		//		checksum ^= 0x05;
		//	}
		//	else
		//	{
		//		msg->byBuffer[iCount]=byTmpBuffer[i];
		//		checksum ^= byTmpBuffer[i];
		//	}
		//	iCount++;
		//}

		//msg->byBuffer[iCount]=checksum;
		//iCount++;


		//msg->bySize=iCount;


		send(msg);


	}
	return 0;
}

/**********************************************************************************************//**
 * Sets board mode
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	byte	The byte.
 **************************************************************************************************/

void CInterfaceMasimoSPO2::setBoardMode(BYTE byte)
{
	m_cfgBoardMode=byte;
	SendSPO2Command(SPO2_MASIMO_HOSTCMD_BOARDMODE, byte, 0x00, 0x00, 0x00, 0x00, 0x00);

	//timer??????
	m_bCheck_BoardMode=true;
}

/**********************************************************************************************//**
 * Sets digital waveform configuration wc0
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	waveSource  	The wave source.
 * \param	waveScaling 	The wave scaling.
 * \param	waveDataSize	Size of the wave data.
 **************************************************************************************************/

void CInterfaceMasimoSPO2::setDigitalWaveformConfiguration_WC0(WORD waveSource, BYTE waveScaling, BYTE waveDataSize)
{
	m_cfgDigitalWaveformConfiguration_WC0=0;
	m_cfgDigitalWaveformConfiguration_WC0+=waveSource << 4;
	m_cfgDigitalWaveformConfiguration_WC0+=waveScaling << 2;
	m_cfgDigitalWaveformConfiguration_WC0+=waveDataSize;
	//SendSPO2Command(SPO2_MASIMO_HOSTCMD_WAVEFORMCONFIG, m_cfgDigitalWaveformConfiguration_WC0);
	
	//timer??????
	m_bCheck_DigitalWaveformConfigurationWC0=true;
	
}

/**********************************************************************************************//**
 * Sets digital waveform configuration wc1
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	waveSource  	The wave source.
 * \param	waveScaling 	The wave scaling.
 * \param	waveDataSize	Size of the wave data.
 **************************************************************************************************/

void CInterfaceMasimoSPO2::setDigitalWaveformConfiguration_WC1(WORD waveSource, BYTE waveScaling, BYTE waveDataSize)
{
	m_cfgDigitalWaveformConfiguration_WC1=0;
	m_cfgDigitalWaveformConfiguration_WC1+=waveSource << 4;
	m_cfgDigitalWaveformConfiguration_WC1+=waveScaling << 2;
	m_cfgDigitalWaveformConfiguration_WC1+=waveDataSize;
	//SendSPO2Command(SPO2_MASIMO_HOSTCMD_WAVEFORMCONFIG, m_cfgDigitalWaveformConfiguration_WC0);

	//timer??????
	m_bCheck_DigitalWaveformConfigurationWC0=true;

}

/**********************************************************************************************//**
 * Sends the digital waveform configurations
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CInterfaceMasimoSPO2::sendDigitalWaveformConfigurations()
{
	setDigitalWaveformConfiguration_WC0(SPO2_MASIMO_HOSTCMD_WAVEFORM_SOURCE_RAWIR, SPO2_MASIMO_HOSTCMD_WAVEFORM_SCALING_CONTINUOUS_AUTO, SPO2_MASIMO_HOSTCMD_WAVEFORM_DATASIZE_8BIT);
	setDigitalWaveformConfiguration_WC1(SPO2_MASIMO_HOSTCMD_WAVEFORM_SOURCE_SIGNALIQ, SPO2_MASIMO_HOSTCMD_WAVEFORM_SCALING_NONE, SPO2_MASIMO_HOSTCMD_WAVEFORM_DATASIZE_8BIT);

	SendSPO2Command(SPO2_MASIMO_HOSTCMD_WAVEFORMCONFIG, m_cfgDigitalWaveformConfiguration_WC0,m_cfgDigitalWaveformConfiguration_WC1);
}

/**********************************************************************************************//**
 * Sets spo2 averaging time
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	byte	The byte.
 **************************************************************************************************/

void CInterfaceMasimoSPO2::setSpO2AveragingTime(BYTE byte)
{
	m_cfgSpO2AveragingTime=byte;
	SendSPO2Command(SPO2_MASIMO_HOSTCMD_SPO2AVERAGINGTIME, m_cfgSpO2AveragingTime);	//

	DEBUGMSG(TRUE, (TEXT("xxxxxxxxxx SpO2AveragingTime %d\r\n"),m_cfgSpO2AveragingTime));
	m_bCheck_SpO2AveragingTime=true;
	
}

/**********************************************************************************************//**
 * Sets algorithm mode
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	bit0	True to bit 0.
 * \param	bit1	True to bit 1.
 * \param	bit2	True to bit 2.
 **************************************************************************************************/

void CInterfaceMasimoSPO2::setAlgorithmMode(bool bit0, bool bit1, bool bit2)
{
	m_cfgAlgorithmMode=0;

	if(bit0)
		m_cfgAlgorithmMode=getModel()->setBitOfByte(m_cfgAlgorithmMode, 0);
	if(bit1)
		m_cfgAlgorithmMode=getModel()->setBitOfByte(m_cfgAlgorithmMode, 1);
	if(bit2)
		m_cfgAlgorithmMode=getModel()->setBitOfByte(m_cfgAlgorithmMode, 2);

	SendSPO2Command(SPO2_MASIMO_HOSTCMD_ALGORITHMMODE, 0x00, m_cfgAlgorithmMode);	//

	m_bCheck_AlgorithmMode=true;
}

/**********************************************************************************************//**
 * Sets waveform mode
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	byte	The byte.
 **************************************************************************************************/

void CInterfaceMasimoSPO2::setWaveformMode(BYTE byte)
{
	m_cfgWaveformMode=byte;
	SendSPO2Command(SPO2_MASIMO_HOSTCMD_WAVEFORMMODE, m_cfgWaveformMode);	//

	//timer??????
	m_bCheck_tWaveformMode=true;
}

/**********************************************************************************************//**
 * Query if this instance is state ok
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if state ok, false if not.
 **************************************************************************************************/

bool CInterfaceMasimoSPO2::isStateOk()
{
	return m_bProcessing;
}

/**********************************************************************************************//**
 * Sets fast sat on
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	bFastSATon	True to fast sat on.
 **************************************************************************************************/

void CInterfaceMasimoSPO2::set_FastSATon(bool bFastSATon)
{
	m_bFastSATon=bFastSATon;
	
	switch(m_eSPO2SensitivityMode)
	{
	case SPO2_SENSITIVITY_NORMAL:
		{
			setAlgorithmMode(true, m_bFastSATon, false);
		}
		break;
	case SPO2_SENSITIVITY_MAXIMUM:
		{
			setAlgorithmMode(false, m_bFastSATon, false);
		}
		break;
	case SPO2_SENSITIVITY_APOD:
		{
			setAlgorithmMode(true, m_bFastSATon, true);
		}
		break;
	}

	getModel()->getCONFIG()->setFastSATon(m_bFastSATon);
}

/**********************************************************************************************//**
 * Sets sensitivity mode
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	mode	The mode.
 **************************************************************************************************/

void CInterfaceMasimoSPO2::set_SensitivityMode(eSPO2sensitivitymode mode)
{
	m_eSPO2SensitivityMode=mode;

	switch(m_eSPO2SensitivityMode)
	{
	case SPO2_SENSITIVITY_NORMAL:
		{
			setAlgorithmMode(true, m_bFastSATon, false);
		}
		break;
	case SPO2_SENSITIVITY_MAXIMUM:
		{
			setAlgorithmMode(false, m_bFastSATon, false);
		}
		break;
	case SPO2_SENSITIVITY_APOD:
		{
			setAlgorithmMode(true, m_bFastSATon, true);
		}
		break;
	} 

	getModel()->getCONFIG()->setSensitivityMode(m_eSPO2SensitivityMode);
}

/**********************************************************************************************//**
 * Sets spo2averaging time
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	avtime	The avtime.
 **************************************************************************************************/

void CInterfaceMasimoSPO2::set_SPO2averagingTime(eSPO2averagingtime avtime)
{
	m_eSPO2averagingTime=avtime;

	setSpO2AveragingTime((BYTE)m_eSPO2averagingTime);

	getModel()->getCONFIG()->setSPO2averagingTime(m_eSPO2averagingTime);
}

/**********************************************************************************************//**
 * Sets spo2alarm delay
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	delay	The delay.
 **************************************************************************************************/

void CInterfaceMasimoSPO2::set_SPO2alarmDelay(eSPO2alarmdelay delay)
{
	m_eSPO2alarmdelay=delay;

	getModel()->getCONFIG()->setSPO2alarmDelay(m_eSPO2alarmdelay);
}

/**********************************************************************************************//**
 * Gets product identifier
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The product identifier.
 **************************************************************************************************/

WORD CInterfaceMasimoSPO2::get_ProductID()
{
	return m_iProductID;
}

/**********************************************************************************************//**
 * Gets version mcu
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The version mcu.
 **************************************************************************************************/

WORD CInterfaceMasimoSPO2::get_VersionMCU()
{
	return m_iVersionMCU;
}

/**********************************************************************************************//**
 * Gets version DSP
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The version DSP.
 **************************************************************************************************/

WORD CInterfaceMasimoSPO2::get_VersionDSP()
{
	return m_iVersionDSP;
}