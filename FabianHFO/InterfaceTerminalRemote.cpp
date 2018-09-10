//=============================================================================
/** 
* \file InterfaceTerminalRemote.cpp
* \brief Functions data exchange for research only!.
* \author Rainer Kuehner
* \date 2013-01-21 Creation of file
*
*	SOM|NBF|CMD|DATA0|...|DATAn|CKS
*	message header-0x02
*	NBF: number of bytes to follow this byte, including the checksum
*	CMD: command byte
*	DATA byte 0 ... DATA byte n - data bytes 0 thru n
*	CKS: checksum, is computed by adding the data bytes using unsigned modulo 256 arithmetic, beginning with NBF,CMD,DATAbyte 0...DATAbyte n
*
*       Copyright (c) 2013 Ing.Buero Rainer Kuehner
*                      Illerkirchberg
*
**/
//=============================================================================
#include "stdafx.h"
#include "FabianHFO.h"
#include "InterfaceTerminalRemote.h"
#include "MVModel.h"
#include "MVViewHandler.h"

/**********************************************************************************************//**
 * CInterfaceTerminalRemote
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

#define TERMINAL_NOTVALID -32765	//value NOTVALID

/**********************************************************************************************//**
 * Initializes a new instance of the InterfaceTerminalRemote class
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CInterfaceTerminalRemote, CWnd)

/**********************************************************************************************//**
 * Initializes a new instance of the CInterfaceTerminalRemote class
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

CInterfaceTerminalRemote::CInterfaceTerminalRemote():
CInterfaceTerminal()
{
	m_bMEASUREMENTS_BTB=false;
	m_bMEASUREMENTS_AVG=false;
	m_bSendContiniousMeasurements=false;

	fwReadChkSum=0;
	fwCalcChkSum = 0;
	feRecvState = TERM_STATE_SOM;
	fdwRecvBufLen = 0;
	faRecvBuf[0]=0x0000;
	faDataBuf[0]=0x0000;
	fwRecvDataBufCnt=0;
	fwRecvDataCnt=0;
	fwRecvDataSize=0;
	fwRecvDataCode=0;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CInterfaceTerminalRemote class
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

CInterfaceTerminalRemote::~CInterfaceTerminalRemote()
{
}

//BEGIN_MESSAGE_MAP(CInterfaceTerminalRemote, CInterfaceTerminal)
//END_MESSAGE_MAP()

/**********************************************************************************************//**
 * CInterfaceTerminalRemote message handlers
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CInterfaceTerminalRemote::Init()
{
	StartTerminalThread();

	OpenCOM();

	Write("fabianHFO");
	//#define DUMP_TERMINALCMD

#ifdef DUMP_TERMINALCMD
	//sendData_Cmd(TERMINAL_GET_MEASUREMENTS_ONCE_BTB);			//0x00
	//sendData_Cmd(TERMINAL_GET_MEASUREMENTS_CONTINIOUS_BTB);	//0x01
	//sendData_Cmd(TERMINAL_GET_MEASUREMENTS_ONCE_AVG	);		//0x02
	//sendData_Cmd(TERMINAL_GET_MEASUREMENTS_CONTINIOUS_AVG);	//0x03
	//sendData_Cmd(TERMINAL_GET_WAVE_DATA);					//0x04
	//sendData_Cmd(TERMINAL_GET_VENT_MODE);					//0x05//'O'
	//sendData_Cmd(TERMINAL_GET_MODE_OPTION1);					//0x06//'R'
	//sendData_Cmd(TERMINAL_GET_MODE_OPTION2);					//0x07//')'
	//sendData_Cmd(TERMINAL_GET_VENT_RUNSTATE);					//0x09
	//sendData_Cmd(TERMINAL_GET_STATE_VLimit);					//0x0A
	//sendData_Cmd(TERMINAL_GET_STATE_VGarant);					//0x0B
	//sendData_Cmd(TERMINAL_GET_PARAM_VentRange);				//0x0C
	//sendData_Cmd(TERMINAL_GET_PARAM_IERatioHFO);				//0x0D
	//sendData_Cmd(TERMINAL_GET_MANBREATHrunning);				//0x0E
	//sendData_Cmd(TERMINAL_GET_PressureRiseCtrl);				//0x0F
	//sendData_Cmd(TERMINAL_GET_PARAM_HFOFreqRec);				//0x10
	//sendData_Cmd(TERMINAL_GET_PARAM_HFOFlow);					//0x11
	//sendData_Cmd(TERMINAL_GET_LeakCompensation);				//0x12
	//sendData_Cmd(TERMINAL_GET_TriggerOption);					//0x13
	//sendData_Cmd(TERMINAL_GET_FOToscillationState);			//0x14
	//sendData_Cmd(TERMINAL_GET_PARAM_PINSP);					//0x15
	//sendData_Cmd(TERMINAL_GET_PARAM_PEEP);						//0x16
	//sendData_Cmd(TERMINAL_GET_PARAM_PPSV);						//0x17
	//sendData_Cmd(TERMINAL_GET_PARAM_BPM);						//0x18
	//sendData_Cmd(TERMINAL_GET_PARAM_HFAmpl);					//0x19
	//sendData_Cmd(TERMINAL_GET_PARAM_HFAmplMax);				//0x1A
	//sendData_Cmd(TERMINAL_GET_PARAM_HFFreq);					//0x1B
	//sendData_Cmd(TERMINAL_GET_PARAM_O2);						//0x1C
	//sendData_Cmd(TERMINAL_GET_PARAM_IFlow);					//0x1D
	//sendData_Cmd(TERMINAL_GET_PARAM_EFlow);					//0x1E
	//sendData_Cmd(TERMINAL_GET_PARAM_Risetime);					//0x1F
	//sendData_Cmd(TERMINAL_GET_PARAM_ITime);					//0x20
	//sendData_Cmd(TERMINAL_GET_PARAM_ETime);					//0x21
	//sendData_Cmd(TERMINAL_GET_PARAM_HFPMean);					//0x22
	//sendData_Cmd(TERMINAL_GET_PARAM_HFPMeanRec);				//0x23
	//sendData_Cmd(TERMINAL_GET_PARAM_VLimit);					//0x24
	//sendData_Cmd(TERMINAL_GET_PARAM_VGarant);					//0x25
	//sendData_Cmd(TERMINAL_GET_PARAM_AbortCriterionPSV);		//0x26
	//sendData_Cmd(TERMINAL_GET_PARAM_TherapieFlow);				//0x27
	//sendData_Cmd(TERMINAL_GET_PARAM_Trigger);					//0x28
	//sendData_Cmd(TERMINAL_GET_PARAM_Flowmin);					//0x29
	//sendData_Cmd(TERMINAL_GET_PARAM_CPAP);						//0x2A
	//sendData_Cmd(TERMINAL_GET_PARAM_PManual);					//0x2B
	//sendData_Cmd(TERMINAL_GET_PARAM_Backup);					//0x2C
	//sendData_Cmd(TERMINAL_GET_PARAM_ITimeRec);					//0x2D
	//sendData_Cmd(TERMINAL_GET_PARAM_ETIMERec);					//0x2E
	//sendData_Cmd(TERMINAL_GET_PARAM_SPO2LOW);					//0x2F
	//sendData_Cmd(TERMINAL_GET_PARAM_SPO2HIGH);					//0x30
	//sendData_Cmd(TERMINAL_GET_PARAM_FIO2LOW);					//0x31
	//sendData_Cmd(TERMINAL_GET_PARAM_FIO2HIGH);					//0x32
	//sendData_Cmd(TERMINAL_GET_STATE_PRICO);						//0x33

	//sendData_Cmd(TERMINAL_STOP_CONTINIOUS_MEASUREMENTS);		//0x50
	//sendData_Cmd(TERMINAL_STOP_WAVE_DATA);						//0x51

	//sendData_BYTE(TERMINAL_SET_VENT_MODE, 1);						//0x52
	//sendData_BYTE(TERMINAL_SET_VENT_MODE, 2);
	//sendData_BYTE(TERMINAL_SET_VENT_MODE, 8);
	//sendData_BYTE(TERMINAL_SET_VENT_MODE, 9);

	//sendData_BYTE(TERMINAL_SET_VENT_RUNSTATE, xxx);				//0x55
	//sendData_BYTE(TERMINAL_SET_STATE_VLimit, xxx);				//0x56
	//sendData_BYTE(TERMINAL_SET_STATE_VGarant, 1);				//0x57
	//sendData_BYTE(TERMINAL_SET_PARAM_VentRange, xxx);			//0x58
	//sendData_BYTE(TERMINAL_SET_PARAM_IERatioHFO, xxx);			//0x59
	//sendData_BYTE(TERMINAL_SET_MANBREATHrunning, xxx);			//0x5A
	//sendData_BYTE(TERMINAL_SET_PressureRiseCtrl, xxx);			//0x5B
	//sendData_SHORT(TERMINAL_SET_PARAM_HFOFreqRec, xxx);			//0x5C
	//sendData_SHORT(TERMINAL_SET_PARAM_HFOFlow, xxx);				//0x5D
	//sendData_BYTE(TERMINAL_SET_LeakCompensation, xxx);			//0x5E
	//sendData_SHORT(TERMINAL_SET_PARAM_PINSP, 200);				//0x60
	//sendData_SHORT(TERMINAL_SET_PARAM_PEEP, xxx);					//0x61
	//sendData_SHORT(TERMINAL_SET_PARAM_PPSV, xxx);					//0x62
	//sendData_SHORT(TERMINAL_SET_PARAM_BPM, xxx);					//0x63
	//sendData_SHORT(TERMINAL_SET_PARAM_HFAmpl, xxx);				//0x64
	//sendData_SHORT(TERMINAL_SET_PARAM_HFAmplMax, xxx);			//0x65
	//sendData_SHORT(TERMINAL_SET_PARAM_HFFreq, xxx);				//0x66
	//sendData_BYTE(TERMINAL_SET_PARAM_O2	, xxx);				//0x67
	//sendData_SHORT(TERMINAL_SET_PARAM_IFlow, xxx);				//0x68
	//sendData_SHORT(TERMINAL_SET_PARAM_EFlow, xxx);				//0x6A
	//sendData_SHORT(TERMINAL_SET_PARAM_RiseTime, xxx);				//0x6B
	//sendData_SHORT(TERMINAL_SET_PARAM_ITime	, xxx);			//0x6C
	//sendData_SHORT(TERMINAL_SET_PARAM_ETime, xxx);				//0x6D
	//sendData_SHORT(TERMINAL_SET_PARAM_HFPMean, xxx);				//0x6E
	//sendData_SHORT(TERMINAL_SET_PARAM_HFPMeanRec, xxx);			//0x6F
	//sendData_SHORT(TERMINAL_SET_PARAM_VLimit, xxx);				//0x70
	//sendData_SHORT(TERMINAL_SET_PARAM_VGarant, xxx);				//0x71
	//sendData_BYTE(TERMINAL_SET_PARAM_AbortCriterionPSV, xxx);	//0x72
	//sendData_SHORT(TERMINAL_SET_PARAM_TherapieFlow, xxx);			//0x73
	//sendData_BYTE(TERMINAL_SET_PARAM_Trigger, xxx);				//0x74
	//sendData_SHORT(TERMINAL_SET_PARAM_Flowmin, xxx);				//0x55
	//sendData_SHORT(TERMINAL_SET_PARAM_CPAP, xxx);					//0x76
	//sendData_SHORT(TERMINAL_SET_PARAM_PManual, xxx);				//0x77
	//sendData_BYTE(TERMINAL_SET_PARAM_Backup, xxx);				//0x78
	//sendData_SHORT(TERMINAL_SET_PARAM_ITimeRec, xxx);				//0x79
	//sendData_BYTE(TERMINAL_SET_PARAM_O2_FLUSH, xxx);				//0x7A
	//sendData_BYTE(TERMINAL_SET_PARAM_SPO2LOW, xxx);				//0x7A
	//sendData_BYTE(TERMINAL_SET_PARAM_SPO2HIGH, xxx);				//0x7B
	//sendData_BYTE(TERMINAL_SET_PARAM_FIO2LOW, xxx);				//0x7C
	//sendData_BYTE(TERMINAL_SET_PARAM_FIO2HIGH, xxx);				//0x7D
	//sendData_BYTE(TERMINAL_SET_STATE_PRICO, xxx);				//0x7E

	//sendData_Cmd(TERMINAL_GET_MEASUREMENTS_CONTINIOUS_AVG);

#endif

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

bool CInterfaceTerminalRemote::OpenCOM()
{
	bool bRes=false;
	
	LONG    lLastError =0;

	if(getModel()->getCONFIG()->GetMainBoardVersion()>=MAINBOARD_30)
		lLastError = Open(_T("COM2:"),0,0);
	else	//MAINBOARD2_1
		lLastError = Open(_T("COM1:"),0,0);

	if (lLastError != ERROR_SUCCESS)
	{
		/*if(ERROR_ALREADY_INITIALIZED==lLastError)
		{
			
		}*/
		return bRes;
	}

	// Setup the serial port (9600,8N1, which is the default setting)
	lLastError = Setup(CSerial::EBaud230400,CSerial::EData8,CSerial::EParNone,CSerial::EStop1);
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

void CInterfaceTerminalRemote::OnEvent(EEvent eEvent, EError eError)
{
	//SOM|NBF|CMD|DATA0|...|DATAn|CKS
	//message header-0x02
	//NBF: number of bytes to follow this byte, including the checksum
	//CMD: command byte
	//DATA byte 0 ... DATA byte n - data bytes 0 thru n
	//CKS: checksum, is computed by adding the data bytes using unsigned modulo 256 arithmetic, beginning with NBF,CMD,DATAbyte 0...DATAbyte n

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
				case TERM_STATE_SOM:

					fwRecvDataCnt=0;
					fwCalcChkSum = 0;
					fwReadChkSum=0;
					fwRecvDataSize=0;
					fwRecvDataCode=0;
					fwRecvDataBufCnt=0;

					switch (faRecvBuf[i])
					{
					case TERM_MSG_SOM:
						feRecvState = TERM_STATE_DATA_SIZE;
						//DEBUGMSG(TRUE, (TEXT("TERM_MSG_SOM\r\n")));
						break;

					default:
						// forget the rest
						feRecvState = TERM_STATE_SOM;
						DEBUGMSG(TRUE, (TEXT("OnEvent5\r\n")));
						return;
					} // switch (faRecvBuf[i])
					break;

				case TERM_STATE_DATA_SIZE:

					fwCalcChkSum += c;
					fwRecvDataSize = c;
					feRecvState = TERM_STATE_CODE;
					//DEBUGMSG(TRUE, (TEXT("TERM_STATE_DATA_SIZE\r\n")));
					break;

				case TERM_STATE_CODE:
					
					fwCalcChkSum += c;
					fwRecvDataCode = c;
					fwRecvDataCnt++;
					if(fwRecvDataSize==2)
					{
						feRecvState = TERM_STATE_CHKSUM;
						//DEBUGMSG(TRUE, (TEXT("TERM_STATE_CODE size==2\r\n")));
					}
					else
					{
						feRecvState = TERM_STATE_DATA;
						//DEBUGMSG(TRUE, (TEXT("TERM_STATE_CODE\r\n")));
					}
					
					break;

				case TERM_STATE_DATA:

					//DEBUGMSG(TRUE, (TEXT("TERM_STATE_DATA\r\n")));
					fwCalcChkSum += c;
					faDataBuf[fwRecvDataBufCnt] = c;
					fwRecvDataBufCnt++;
					fwRecvDataCnt++;
					feRecvState = TERM_STATE_DATA;

					if(fwRecvDataCnt>=(fwRecvDataSize-1))
						feRecvState = TERM_STATE_CHKSUM;

					break;


				/*case TERM_STATE_DATA1:

					fwCalcChkSum += c;
					faDataBuf[0] = c;
					fwDataCnt++;
					feRecvState = TERM_STATE_DATA2;

					break;

				case TERM_STATE_DATA2:

					fwCalcChkSum += c;
					faDataBuf[1] = c;
					fwDataCnt++;

					feRecvState = TERM_STATE_CHKSUM;

					break;*/

				case TERM_STATE_CHKSUM:

					//DEBUGMSG(TRUE, (TEXT("TERM_STATE_CHKSUM\r\n")));
					fwReadChkSum = c;
					if(fwReadChkSum == fwCalcChkSum)
					{
						//DEBUGMSG(TRUE, (TEXT("checksum ok\r\n")));
						//TRACE(_T("checksum ok \n"));
						this->PerformMsg();
					}
					else
					{
						//checksum error
						DEBUGMSG(TRUE, (TEXT("checksum error\r\n")));
					}

					feRecvState = TERM_STATE_SOM;
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
 * Performs the message action
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CInterfaceTerminalRemote::PerformMsg()
{
	CString szError=_T("");
	
	switch(fwRecvDataCode)
	{
		//slave messages
		case TERMINAL_GET_MEASUREMENTS_ONCE_BTB:
			{
				DEBUGMSG(TRUE, (TEXT("TERM_GET_MEASUREMENTS_ONCE_BTB\r\n")));
				m_bSendContiniousMeasurements=false;
				m_bMEASUREMENTS_BTB=true;
				sendMessurementData();
				m_bMEASUREMENTS_BTB=false;
				
			}
			break;
		case TERMINAL_GET_MEASUREMENTS_CONTINIOUS_BTB:
			{
				DEBUGMSG(TRUE, (TEXT("TERM_GET_MEASUREMENTS_CONTINIOUS_BTB\r\n")));
				m_bMEASUREMENTS_AVG=false;
				m_bMEASUREMENTS_BTB=true;

				m_bSendContiniousMeasurements=true;
			}
			break;
		case TERMINAL_GET_MEASUREMENTS_ONCE_AVG:
			{
				DEBUGMSG(TRUE, (TEXT("TERM_GET_MEASUREMENTS_ONCE_AVG\r\n")));
				m_bSendContiniousMeasurements=false;
				m_bMEASUREMENTS_AVG=true;
				sendMessurementData();
				m_bMEASUREMENTS_AVG=false;
			}
			break;
		case TERMINAL_GET_MEASUREMENTS_CONTINIOUS_AVG:
			{
				DEBUGMSG(TRUE, (TEXT("TERM_GET_MEASUREMENTS_CONTINIOUS_AVG\r\n")));
				m_bMEASUREMENTS_BTB=false;
				m_bMEASUREMENTS_AVG=true;

				m_bSendContiniousMeasurements=true;
			}
			break;
		case TERMINAL_STOP_CONTINIOUS_MEASUREMENTS:
			{
				DEBUGMSG(TRUE, (TEXT("TERM_STOP_CONTINIOUS_MEASUREMENTS\r\n")));
				m_bSendContiniousMeasurements=false;
				m_bMEASUREMENTS_BTB=false;
				m_bMEASUREMENTS_AVG=false;
			}
			break;
		case TERMINAL_GET_WAVE_DATA:
			{
				DEBUGMSG(TRUE, (TEXT("TERM_GET_WAVE_DATA\r\n")));
				EnterCriticalSection(&csEnableSend);
				m_bSendWaveData=true;
				LeaveCriticalSection(&csEnableSend);
			}
			break;
		case TERMINAL_STOP_WAVE_DATA:
			{
				DEBUGMSG(TRUE, (TEXT("TERM_STOP_WAVE_DATA\r\n")));
				EnterCriticalSection(&csEnableSend);
				m_bSendWaveData=false;
				LeaveCriticalSection(&csEnableSend);
			}
			break;
		case TERMINAL_GET_VENT_MODE:
			{
				DEBUGMSG(TRUE, (TEXT("TERM_GET_VENT_MODE\r\n")));
				eVentMode eActiveVentMode;
				if(getModel()->getCONFIG()->CurModeIsPresetMode()==false)
					eActiveVentMode=getModel()->getCONFIG()->GetCurMode();
				else
					eActiveVentMode=getModel()->getCONFIG()->GetPrevMode();

				sendData_BYTE(TERMINAL_VENT_MODE, (BYTE)eActiveVentMode);
			}
			break;
		case TERMINAL_SET_VENT_MODE:
			{
				DEBUGMSG(TRUE, (TEXT("TERM_SET_VENT_MODE1\r\n")));
				BYTE state=faDataBuf[0];

				/*if(getModel()->getVIEWHANDLER()->getViewState()!=VS_PARA)
				{
					getModel()->getVIEWHANDLER()->changeViewState(VS_PARA,VSS_GRAPH_SINGLE_LINEGRAPHS);
					Sleep(400);
				}*/
				setVentMode(state);
				/*Sleep(400);
				setVentMode(state);xxx*/
			}
			break;
		case TERMINAL_GET_MODE_OPTION1:
			{
				sendModeOption1();
			}
			break;
		case TERMINAL_GET_VENT_RUNSTATE:
			{
				if(VENT_STOPPED==getModel()->GetVentRunState())//return 0
				{
					sendData_BYTE(TERMINAL_VENT_RUNSTATE, 0);
				}
				else if(VENT_RUNNING==getModel()->GetVentRunState())//return 1
				{
					sendData_BYTE(TERMINAL_VENT_RUNSTATE, 1);
				}
				else//VENT_STANDBY, return 2
				{
					sendData_BYTE(TERMINAL_VENT_RUNSTATE, 2);
				}
			}
			break;
		case TERMINAL_SET_VENT_RUNSTATE:
			{
				BYTE iVal=faDataBuf[0];
				setVent_RunState(iVal);
			}
			break;
		case TERMINAL_GET_STATE_VGarant:
			{
				if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
				{
					if(getModel()->getDATAHANDLER()->PARADATA()->IsVGarantStateOn_IPPV())
						sendData_BYTE(TERMINAL_STATE_VGarant, 1);
					else
						sendData_BYTE(TERMINAL_STATE_VGarant, 0);
				}
				else
				{
					if(getModel()->getDATAHANDLER()->PARADATA()->IsVGarantStateOn_TRIGGER())
						sendData_BYTE(TERMINAL_STATE_VGarant, 1);
					else
						sendData_BYTE(TERMINAL_STATE_VGarant, 0);
				}
			}
			break;
		case TERMINAL_SET_STATE_VGarant:
			{
				BYTE iVal=faDataBuf[0];
				setState_VGarant(iVal);
			}
			break;
		case TERMINAL_GET_STATE_VLimit:
			{
				if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
				{
					if(getModel()->getDATAHANDLER()->PARADATA()->IsVLimitParamOn_IPPV())
						sendData_BYTE(TERMINAL_STATE_VLimit, 1);
					else
						sendData_BYTE(TERMINAL_STATE_VLimit, 0);
				}
				else
				{
					if(getModel()->getDATAHANDLER()->PARADATA()->IsVLimitParamOn_TRIGGER())
						sendData_BYTE(TERMINAL_STATE_VLimit, 1);
					else
						sendData_BYTE(TERMINAL_STATE_VLimit, 0);
				}
			}
			break;
		case TERMINAL_SET_STATE_VLimit:
			{
				BYTE iVal=faDataBuf[0];
				setState_VLimit(iVal);
			}
			break;
		case TERMINAL_GET_PARAM_VentRange:
			{
				sendData_BYTE(TERMINAL_PARAM_VentRange, (BYTE)getModel()->getCONFIG()->GetVentRange());
			}
			break;
		case TERMINAL_SET_PARAM_VentRange:
			{
				BYTE state=faDataBuf[0];
				setVentRange(state);
			}
			break;
		case TERMINAL_GET_PARAM_IERatioHFO:
			{
				sendData_BYTE(TERMINAL_PARAM_IERatioHFO, (BYTE)getModel()->getDATAHANDLER()->GetCurrentIERatioParaHFO());//RIE_1_3=0, RIE_1_2=1,RIE_1_1=2
			}
			break;
		case TERMINAL_SET_PARAM_IERatioHFO:
			{
				BYTE state=faDataBuf[0];
				setParam_IERatioHFO(state);
			}
			break;
		case TERMINAL_GET_MANBREATHrunning:
			{
				if(getModel()->isMANBREATHrunning())//0=false, 1=true
					sendData_BYTE(TERMINAL_MANBREATHrunning, 1);
				else
					sendData_BYTE(TERMINAL_MANBREATHrunning, 0);
			}
			break;
		case TERMINAL_SET_MANBREATHrunning:
			{
				BYTE state=faDataBuf[0];
				set_MANBREATHrunning(state);
			}
			break;
		case TERMINAL_GET_MODE_OPTION2:
			{
				sendModeOption2();
			}
			break;
		case TERMINAL_GET_PressureRiseCtrl:
			{
				eCurveForm form = getModel()->getCONFIG()->GetCurPressureRiseCtrl();

				switch(form)
				{
				case CURVE_IFLOW:
					{
						sendData_BYTE(TERMINAL_PressureRiseCtrl, 0);
					}
					break;
				case CURVE_LINEAR:
					{
						sendData_BYTE(TERMINAL_PressureRiseCtrl, 1);
					}
					break;
				case CURVE_AUTOFLOW: 
					{
						sendData_BYTE(TERMINAL_PressureRiseCtrl, 2);
					}
					break;
				}
			}
			break;
		case TERMINAL_SET_PressureRiseCtrl:
			{
				BYTE state=faDataBuf[0];
				set_PressureRiseCtrl(state);
			}
			break;
		case TERMINAL_GET_PARAM_HFOFreqRec:
			{
				if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
				{
					sendData_SHORT(TERMINAL_PARAM_HFOFreqRec, (SHORT)getModel()->getDATAHANDLER()->GetCurrentFreqRecPara());
				}
				else
				{
					sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
				}
				
			}
			break;
		case TERMINAL_SET_PARAM_HFOFreqRec:
			{
				WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);
				setParam_HFOFreqRec(iVal);
			}
			break;
		case TERMINAL_GET_PARAM_HFOFlow:
			{
				if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
				{
					sendData_SHORT(TERMINAL_PARAM_HFOFlow, (SHORT)getModel()->getDATAHANDLER()->GetCurrentHFFlowPara());
				}
				else
				{
					sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
				}
			}
			break;
		case TERMINAL_SET_PARAM_HFOFlow:
			{
				WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);
				setParam_HFOFlow(iVal);
			}
			break;
		case TERMINAL_GET_LeakCompensation:
			{
				eLeakCompensation eLeakCompOff=getModel()->getCONFIG()->getLeakCompensation();
				switch(eLeakCompOff)
				{
				case LC_LOW:
					{
						sendData_BYTE(TERMINAL_LeakCompensation, 0);
					}
					break;
				case LC_MIDDLE:
					{
						sendData_BYTE(TERMINAL_LeakCompensation, 1);
					}
					break;
				case LC_HIGH:
					{
						sendData_BYTE(TERMINAL_LeakCompensation, 2);
					}
					break;
				}
			}
			break;
		case TERMINAL_SET_LeakCompensation:
			{
				BYTE iVal=faDataBuf[0];
				setLeakCompensation(iVal);
			}
			break;
		case TERMINAL_GET_TriggerOption:
			{
				eTriggereType trigger = TRIGGER_FLOW;
				if(		getModel()->getCONFIG()->GetCurMode()==VM_DUOPAP
					||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_DUOPAP)
				{
					trigger=getModel()->getDATAHANDLER()->getTriggerOptionDUOPAP();
				}
				else if(	getModel()->getCONFIG()->GetCurMode()==VM_NCPAP
					||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_NCPAP)
				{
					trigger=getModel()->getDATAHANDLER()->getTriggerOptionNCPAP();
				}
				else
				{
					trigger=getModel()->getDATAHANDLER()->getTriggerOptionCONV();
				}
				switch(trigger)
				{
				case TRIGGER_VOLUME:
					{
						sendData_BYTE(TERMINAL_TriggerOption, 0);
					}
					break;
				case TRIGGER_FLOW:
					{
						sendData_BYTE(TERMINAL_TriggerOption, 1);
					}
					break;
				case TRIGGER_PRESSURE:
					{
						sendData_BYTE(TERMINAL_TriggerOption, 2);
					}
					break;
				}
			}
			break;
		/*case TERMINAL_SET_TriggerOption:
			{
				BYTE iVal=faDataBuf[0];
				setTriggerOption(iVal);
			}
			break;*/
		case TERMINAL_GET_FOToscillationState:
			{
				if(true==getModel()->getDATAHANDLER()->getFOToscillationState())
				{
					sendData_BYTE(TERMINAL_FOToscillationState, 1);
				}
				else
				{
					sendData_BYTE(TERMINAL_FOToscillationState, 0);
				}
			}
			break;
		case TERMINAL_GET_PARAM_PINSP:
			{
				sendData_SHORT(TERMINAL_PARAM_PINSP, (SHORT)getModel()->getDATAHANDLER()->GetActiveModePINSPPara());
			}
			break;
		case TERMINAL_SET_PARAM_PINSP:
			{
				WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);
				setParam_PINSP(iVal);
			}
			break;
		case TERMINAL_GET_PARAM_PEEP:
			{
				if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
					sendData_SHORT(TERMINAL_PARAM_PEEP, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetPEEPPara_IPPV());
				else
					sendData_SHORT(TERMINAL_PARAM_PEEP, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetPEEPPara_TRIGGER());
			}
			break;
		case TERMINAL_SET_PARAM_PEEP:
			{
				WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);
				setParam_PEEP(iVal);
			}
			break;
		case TERMINAL_GET_PARAM_PPSV:
			{
				sendData_SHORT(TERMINAL_PARAM_PPSV, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetPpsvPara());
			}
			break;
		case TERMINAL_SET_PARAM_PPSV:
			{
				WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);
				setParam_PPSV(iVal);
			}
			break;
		case TERMINAL_GET_PARAM_BPM:
			{
				sendData_SHORT(TERMINAL_PARAM_BPM, (SHORT)getModel()->getDATAHANDLER()->GetCurrentITimePara());
			}
			break;
		case TERMINAL_SET_PARAM_BPM:
			{
				WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);
				setParam_BPM(iVal);
			}
			break;
		case TERMINAL_GET_PARAM_HFAmpl:
			{
				WORD iHFAmpl=0;
				iHFAmpl=(int)getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLPara();

				sendData_SHORT(TERMINAL_PARAM_HFAmpl, (SHORT)iHFAmpl);
			}
			break;
		case TERMINAL_SET_PARAM_HFAmpl:
			{
				WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);
				setParam_HFAmpl(iVal);
			}
			break;
		case TERMINAL_GET_PARAM_HFAmplMax:
			{
				WORD iHFAmpl=iHFAmpl=(int)getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLmaxPara();

				sendData_SHORT(TERMINAL_PARAM_HFAmplMax, (SHORT)iHFAmpl);
			}
			break;
		case TERMINAL_SET_PARAM_HFAmplMax:
			{
				WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);
				setParam_HFAmplMax(iVal);
			}
			break;
		case TERMINAL_GET_PARAM_HFFreq:
			{
				sendData_SHORT(TERMINAL_PARAM_HFFreq, getModel()->getDATAHANDLER()->PARADATA()->GetHFFreqPara());
			}
			break;
		case TERMINAL_SET_PARAM_HFFreq:
			{
				WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);
				setParam_HFFreq(iVal);
			}
			break;
		case TERMINAL_GET_PARAM_O2:
			{
				BYTE iO2=0;
				if(getModel()->isO2FlushActive())
					iO2=getModel()->getDATAHANDLER()->PARADATA()->GetO2FlushPara();
				else
					iO2=getModel()->getDATAHANDLER()->PARADATA()->GetO2Para();
				sendData_BYTE(TERMINAL_PARAM_O2, iO2);
			}
			break;
		case TERMINAL_SET_PARAM_O2:
			{
				BYTE iVal=faDataBuf[0];
				setParam_FiO2(iVal);
			}
			break;
		case TERMINAL_GET_PARAM_IFlow:
			{
				if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
					sendData_SHORT(TERMINAL_PARAM_IFlow, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetIFlowPara_IPPV());
				else
					sendData_SHORT(TERMINAL_PARAM_IFlow, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetIFlowPara_TRIGGER());
			}
			break;
		case TERMINAL_SET_PARAM_IFlow:
			{
				WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);
				setParam_IFlow(iVal);
			}
			break;
		case TERMINAL_GET_PARAM_EFlow:
			{
				if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
					sendData_SHORT(TERMINAL_PARAM_EFlow, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetEFLOWPara_IPPV());
				else
					sendData_SHORT(TERMINAL_PARAM_EFlow, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetEFLOWPara_TRIGGER());
			}
			break;
		case TERMINAL_SET_PARAM_EFlow:
			{
				WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);
				setParam_EFlow(iVal);
			}
			break;
		case TERMINAL_GET_PARAM_Risetime:
			{
				if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
					sendData_SHORT(TERMINAL_PARAM_Risetime, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetRisetimePara_IPPV());
				else
					sendData_SHORT(TERMINAL_PARAM_Risetime, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetRisetimePara_TRIGGER());
			}
			break;
		case TERMINAL_SET_PARAM_RiseTime:
			{
				WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);
				setParam_RiseTime(iVal);
			}
			break;
		case TERMINAL_GET_PARAM_ITime:
			{
				sendData_SHORT(TERMINAL_PARAM_ITime, (SHORT)getModel()->getDATAHANDLER()->GetCurrentITimePara());
			}
			break;
		case TERMINAL_SET_PARAM_ITime:
			{
				WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);
				setParam_ITime(iVal);
			}
			break;
		case TERMINAL_GET_PARAM_ETime:
			{
				sendData_SHORT(TERMINAL_PARAM_ETime, (SHORT)getModel()->getDATAHANDLER()->GetCurrentETimePara());
			}
			break;
		case TERMINAL_SET_PARAM_ETime:
			{
				WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);
				setParam_ETime(iVal);
			}
			break;
		case TERMINAL_GET_PARAM_HFPMean:
			{
				sendData_SHORT(TERMINAL_PARAM_HFPMean, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanPara());
			}
			break;
		case TERMINAL_SET_PARAM_HFPMean:
			{
				WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);
				setParam_HFPMean(iVal);
			}
			break;
		case TERMINAL_GET_PARAM_HFPMeanRec:
			{
				sendData_SHORT(TERMINAL_PARAM_HFPMeanRec, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanRecPara());
			}
			break;
		case TERMINAL_SET_PARAM_HFPMeanRec:
			{
				if(getModel()->getDATAHANDLER()->isLUNGRECLicenseAvailable())
				{
					WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);
					setParam_HFPMeanRec(iVal);
				}
				else
				{
					sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
				}
			}
			break;
		case TERMINAL_GET_PARAM_VLimit:
			{
				if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
					sendData_SHORT(TERMINAL_PARAM_VLimit, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetVLimitParam_IPPV());
				else
					sendData_SHORT(TERMINAL_PARAM_VLimit, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetVLimitParam_TRIGGER());
			}
			break;
		case TERMINAL_SET_PARAM_VLimit:
			{
				WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);
				setParam_VLimit(iVal);
			}
			break;
		case TERMINAL_GET_PARAM_VGarant:
			{
				if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
					sendData_SHORT(TERMINAL_PARAM_VGarant, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetVGarantPara_IPPV());
				else
					sendData_SHORT(TERMINAL_PARAM_VGarant, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetVGarantPara_TRIGGER());
			}
			break;
		case TERMINAL_SET_PARAM_VGarant:
			{
				if(getModel()->getDATAHANDLER()->isVGUARANTLicenseAvailable())
				{
					WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);
					setParam_VGarant(iVal);
				}
				else
				{
					sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
				}
			}
			break;
		case TERMINAL_GET_PARAM_AbortCriterionPSV:
			{
				sendData_BYTE(TERMINAL_PARAM_AbortCriterionPSV, (BYTE)getModel()->getCONFIG()->GetPercentAbortCriterionPSV());
			}
			break;
		case TERMINAL_SET_PARAM_AbortCriterionPSV:
			{
				BYTE iVal=faDataBuf[0];
				setParam_AbortCriterionPSV(iVal);
			}
			break;
		case TERMINAL_GET_PARAM_TherapieFlow:
			{
				sendData_SHORT(TERMINAL_PARAM_TherapieFlow, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetTherapieFLOWPara());
			}
			break;
		case TERMINAL_SET_PARAM_TherapieFlow:
			{
				WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);
				setParam_TherapieFlow(iVal);
			}
			break;
		case TERMINAL_GET_PARAM_Trigger:
			{
				sendData_BYTE(TERMINAL_PARAM_Trigger, getModel()->getDATAHANDLER()->GetCurrentTriggerPara());
			}
			break;
		case TERMINAL_SET_PARAM_Trigger:
			{
				BYTE iVal=faDataBuf[0];
				setParam_Trigger(iVal);
			}
			break;
		case TERMINAL_GET_PARAM_Flowmin:
			{
				sendData_SHORT(TERMINAL_PARAM_Flowmin, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetFlowminPara());
			}
			break;
		case TERMINAL_SET_PARAM_Flowmin://only in CPAP
			{
				WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);
				setParam_Flowmin(iVal);
			}
			break;
		case TERMINAL_GET_PARAM_CPAP://CPAP,NCPAP and DUOPAP
			{
				switch(getModel()->getCONFIG()->GetCurMode())
				{
				case VM_CPAP:
					{
						sendData_SHORT(TERMINAL_PARAM_CPAP, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetCPAPPara());
					}
					break;
				case VM_NCPAP:
					{
						sendData_SHORT(TERMINAL_PARAM_CPAP, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetCPAPNMODEPara());
					}
					break;
				case VM_DUOPAP:
					{
						sendData_SHORT(TERMINAL_PARAM_CPAP, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetCPAPNMODEPara());
					}
					break;
				default:
					sendData_Cmd(TERM_PARAM_NOSUPPORT);
					break;
				}
			}
			break;
		case TERMINAL_SET_PARAM_CPAP:
			{
				WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);
				setParam_CPAP(iVal);
			}
			break;
		case TERMINAL_GET_PARAM_PManual:
			{
				switch(getModel()->getCONFIG()->GetCurMode())
				{
				case VM_CPAP:
					{
						sendData_SHORT(TERMINAL_PARAM_PManual, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetPManualCPAPPara());
					}
					break;
				case VM_NCPAP:
					{
						sendData_SHORT(TERMINAL_PARAM_PManual, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetPManualNMODEPara());
					}
					break;
				case VM_HFO:
					{
						sendData_SHORT(TERMINAL_PARAM_PManual, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetPManualHFOPara());
					}
					break;
				default:
					sendData_Cmd(TERM_PARAM_NOSUPPORT);
					break;
				}
			}
			break;
		case TERMINAL_SET_PARAM_PManual:
			{
				WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);
				setParam_PManual(iVal);
			}
			break;
		case TERMINAL_GET_PARAM_Backup:
			{
				sendData_BYTE(TERMINAL_PARAM_Backup, getModel()->getDATAHANDLER()->PARADATA()->GetBackupPara());
			}
			break;
		case TERMINAL_SET_PARAM_Backup:
			{
				BYTE iVal=faDataBuf[0];
				setParam_Backup(iVal);
			}
			break;		
		case TERMINAL_GET_PARAM_ITimeRec:
			{
				sendData_SHORT(TERMINAL_PARAM_ITimeRec, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetITimeRecPara());
			}
			break;
		case TERMINAL_SET_PARAM_ITimeRec:
			{
				WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);
				setParam_ITimeRec(iVal);
			}
			break;
		case TERMINAL_GET_PARAM_ETIMERec:
			{
				int iETIMERec=0;
				if(getModel()->getDATAHANDLER()->isLUNGRECLicenseAvailable()==false && getModel()->getDATAHANDLER()->PARADATA()->GetFreqRecPara()!=0)
				{
					iETIMERec=1;
				}
				else
				{
					iETIMERec=getModel()->getDATAHANDLER()->GetHF_ETIME_REC(getModel()->getDATAHANDLER()->PARADATA()->GetFreqRecPara());
				}
				sendData_SHORT(TERMINAL_PARAM_ETIMERec, (SHORT)iETIMERec);
			}
			break;
		/*case TERMINAL_SET_PARAM_ETIMERec:
			{
				WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);
				setParam_ETimeRec(iVal);
			}
			break;*/
		/*case TERMINAL_GET_PARAM_FreqRec:
			{
				int iFreqRec=0;;

				if(getModel()->getDATAHANDLER()->isLUNGRECLicenseAvailable()==false && getModel()->getDATAHANDLER()->PARADATA()->GetFreqRecPara()!=0)
				{
					iFreqRec=0;
				}
				else
				{
					iFreqRec=getModel()->getDATAHANDLER()->PARADATA()->GetFreqRecPara();
				}
				sendData_SHORT(TERMINAL_PARAM_FreqRec, (SHORT)iFreqRec);
			}
			break;
		case TERMINAL_SET_PARAM_FreqRec:
			{
				WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);
				setParam_FreqRec(iVal);
			}
			break;*/
		case TERMINAL_SET_PARAM_O2_FLUSH:
			{
				BYTE iVal=faDataBuf[0];
				setParam_O2_FLUSH(iVal);
			}
			break;
		case TERMINAL_GET_PARAM_SPO2LOW:
			{
				if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
				{
					sendData_BYTE(TERMINAL_PARAM_SPO2LOW, getModel()->getDATAHANDLER()->getPRICO_SPO2lowRange());
				}
				else
				{
					sendData_Cmd(TERM_PARAM_NOSUPPORT);
				}
			}
			break;
		case TERMINAL_SET_PARAM_SPO2LOW:
			{
				BYTE iVal=faDataBuf[0];
				setParam_SPO2LOW(iVal);
			}
			break;
		case TERMINAL_GET_PARAM_SPO2HIGH:
			{
				if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
				{
					sendData_BYTE(TERMINAL_PARAM_SPO2HIGH, getModel()->getDATAHANDLER()->getPRICO_SPO2highRange());
				}
				else
				{
					sendData_Cmd(TERM_PARAM_NOSUPPORT);
				}
			}
			break;
		case TERMINAL_SET_PARAM_SPO2HIGH:
			{
				BYTE iVal=faDataBuf[0];
				setParam_SPO2HIGH(iVal);
			}
			break;
		case TERMINAL_GET_PARAM_FIO2LOW:
			{
				if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
				{
					sendData_BYTE(TERMINAL_PARAM_FIO2LOW, getModel()->getDATAHANDLER()->PARADATA()->GetITimeRecPara());
				}
				else
				{
					sendData_Cmd(TERM_PARAM_NOSUPPORT);
				}
			}
			break;
		case TERMINAL_SET_PARAM_FIO2LOW:
			{
				BYTE iVal=faDataBuf[0];
				setParam_FIO2LOW(iVal);
			}
			break;
		case TERMINAL_GET_PARAM_FIO2HIGH:
			{
				if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
				{
					sendData_BYTE(TERMINAL_PARAM_FIO2HIGH, getModel()->getDATAHANDLER()->getPRICO_FIO2highRange());
				}
				else
				{
					sendData_Cmd(TERM_PARAM_NOSUPPORT);
				}
			}
			break;
		case TERMINAL_SET_PARAM_FIO2HIGH:
			{
				BYTE iVal=faDataBuf[0];
				setParam_FIO2HIGH(iVal);
			}
			break;
		case TERMINAL_GET_STATE_PRICO:
			{
				if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
				{
					if(getModel()->getDATAHANDLER()->getPRICOState()==true)
						sendData_BYTE(TERMINAL_STATE_PRICO, 1);
					else
						sendData_BYTE(TERMINAL_STATE_PRICO, 0);
				}
				else
				{
					sendData_Cmd(TERM_PARAM_NOSUPPORT);
				}
			}
			break;
		case TERMINAL_SET_STATE_PRICO:
			{
				BYTE iVal=faDataBuf[0];
				setState_PRICO(iVal);
			}
			break;
		default:
			{
				

			}
			break;
	}


	
	return true;
}

/**********************************************************************************************//**
 * Sends the messurement data
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CInterfaceTerminalRemote::sendMessurementData()
{
	//DEBUGMSG(TRUE, (TEXT("sendMessurementData1\r\n")));
	//SOM|NBF|CMD|DATA0|...|DATAn|CKS
	//message header-0x02
	//NBF: number of bytes to follow this byte, including the checksum
	//CMD: command byte
	//DATA byte 0 ... DATA byte n - data bytes 0 thru n
	//CKS: checksum, is computed by adding the data bytes using unsigned modulo 256 arithmetic, beginning with NBF,CMD,DATAbyte 0...DATAbyte n

	MESSURE_BLOCKDATA pMessureData;

	if(m_bMEASUREMENTS_BTB)
	{
		//getModel()->getDATAHANDLER()->getBlockMessureDataBTB(&pMessureData);
		pMessureData.m_iPmax=getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_P_PEAK);
		pMessureData.m_iPmitt=getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_P_MEAN);
		pMessureData.m_iPEEP=getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_PEEP);
		pMessureData.m_iCompliance=getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_DYNCOMPL);
		pMessureData.m_iC20C=getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_C20C);
		pMessureData.m_iResistance=getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_RESISTANCE);
		pMessureData.m_iMV=getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_MIN_VOL);
		pMessureData.m_iTVI=getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_TVI);
		pMessureData.m_iTVE=getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_TVE);
		pMessureData.m_iTVEresp=getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_TVE_RESP);
		pMessureData.m_iTVEpat=getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_TVE_PATIENT);
		pMessureData.m_iHFAmpl=getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_HFO_AMP);
		pMessureData.m_iTVEHFO=getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_TVE_HFO);
		pMessureData.m_iDCO2=getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_DCO2);
		pMessureData.m_iTrigVol=getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_TRIGGER);
		pMessureData.m_iITimePSV=getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_T_INSP_PSV);
		pMessureData.m_iBPM=getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_FREQ);
		pMessureData.m_iBPMco2=getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_FREQETCO2);
		pMessureData.m_iLeak=getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_LEAK);
		pMessureData.m_iHFFreq=getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_HFO_FREQ);
		pMessureData.m_iPercent=getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_ANTEIL_RESP);

	}
	else //if (m_bMEASUREMENTS_AVG)
	{
		pMessureData.m_iPmax=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_P_PEAK);
		pMessureData.m_iPmitt=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_P_MEAN);
		pMessureData.m_iPEEP=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_PEEP);
		pMessureData.m_iCompliance=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_DYNCOMPL);
		pMessureData.m_iC20C=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_C20C);
		pMessureData.m_iResistance=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_RESISTANCE);
		pMessureData.m_iMV=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_MIN_VOL);
		pMessureData.m_iTVI=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_TVI);
		pMessureData.m_iTVE=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_TVE);
		pMessureData.m_iTVEresp=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_TVE_RESP);
		pMessureData.m_iTVEpat=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_TVE_PATIENT);
		pMessureData.m_iHFAmpl=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_HFO_AMP);
		pMessureData.m_iTVEHFO=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_TVE_HFO);
		pMessureData.m_iDCO2=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_DCO2);
		pMessureData.m_iTrigVol=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_TRIGGER);
		pMessureData.m_iITimePSV=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_T_INSP_PSV);
		pMessureData.m_iBPM=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_FREQ);
		pMessureData.m_iBPMco2=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_FREQETCO2);
		pMessureData.m_iLeak=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_LEAK);
		pMessureData.m_iHFFreq=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_HFO_FREQ);
		pMessureData.m_iPercent=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_ANTEIL_RESP);
		//getModel()->getDATAHANDLER()->getBlockMessureDataAVG(&pMessureData);
	}

	bool bCO2value=false;
	bool bCO2module=false;
	bool bSPO2module=false;

	if(getModel()->getCONFIG()->getCO2module()!=CO2MODULE_NONE)
	{
		bCO2module=true;
	}

	if(	bCO2module && getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==true)
	{
		bCO2value=true;
	}

	if(getModel()->getCONFIG()->getSPO2module()!=SPO2MODULE_NONE)
	{
		bSPO2module=true;
	}

	SHORT iETCO2=TERMINAL_NOTVALID;
	SHORT iSPO2=TERMINAL_NOTVALID;
	SHORT iPulseRate=TERMINAL_NOTVALID;
	SHORT iPerfusionIndex=TERMINAL_NOTVALID;

	if(bSPO2module)
	{
		iSPO2=getModel()->getDATAHANDLER()->getAVGMessureDataSPO2();
		iPulseRate=getModel()->getDATAHANDLER()->getAVGMessureDataSpO2PulseRate();
		iPerfusionIndex=getModel()->getDATAHANDLER()->getAVGMessureDataSpO2PerfusionIndex();
	}

	if(bCO2module && getModel()->getETCO2()!=NULL)
	{
		if(getModel()->getETCO2()->isETCO2ValueValid())
			iETCO2=getModel()->getDATAHANDLER()->getAVGMessureDataETCO2();
	}

	SHORT iBPM=pMessureData.m_iBPM;
	SHORT iBPMco2=pMessureData.m_iBPMco2;
	SHORT iLeak=pMessureData.m_iLeak;
	SHORT iHFFreq=pMessureData.m_iHFFreq;
	SHORT iPercent=pMessureData.m_iPercent;


	if(bCO2value)
	{
		iBPM=TERMINAL_NOTVALID;
	}
	else
	{
		iBPMco2=TERMINAL_NOTVALID;
	}

	SHORT iOxyVal=getModel()->getDATAHANDLER()->getMessureDataO2();
	SHORT iINSP_FLOW=TERMINAL_NOTVALID;
	SHORT iEXP_FLOW=TERMINAL_NOTVALID;
	SHORT iDEMAND_FLOW=TERMINAL_NOTVALID;


	if(getModel()->getVMODEHANDLER()->activeModeIsNMODE())
	{
		iINSP_FLOW=getModel()->getDATAHANDLER()->GetInspFlowData();

		iEXP_FLOW=getModel()->getDATAHANDLER()->GetExpFlowData();
	}
	else if(getModel()->getVMODEHANDLER()->activeModeIsCPAP())
	{
		iDEMAND_FLOW=getModel()->getDATAHANDLER()->GetDemandFlowData();
	}





	eVentMode eActiveVentMode;
	if(getModel()->getCONFIG()->CurModeIsPresetMode()==false)
		eActiveVentMode=getModel()->getCONFIG()->GetCurMode();
	else
		eActiveVentMode=getModel()->getCONFIG()->GetPrevMode();

	switch(eActiveVentMode)
	{
	case VM_IPPV:
		{
			pMessureData.m_iTrigVol=TERMINAL_NOTVALID; 
			pMessureData.m_iHFAmpl=TERMINAL_NOTVALID;
			pMessureData.m_iTVEHFO=TERMINAL_NOTVALID;
			pMessureData.m_iDCO2=TERMINAL_NOTVALID;
			iHFFreq=TERMINAL_NOTVALID;
			iPercent=TERMINAL_NOTVALID;
			pMessureData.m_iTVEresp=TERMINAL_NOTVALID;
			pMessureData.m_iTVEpat=TERMINAL_NOTVALID;
		}
		break;
	case VM_SIPPV:
	case VM_PSV:
		{
			pMessureData.m_iHFAmpl=TERMINAL_NOTVALID;
			pMessureData.m_iTVEHFO=TERMINAL_NOTVALID;
			pMessureData.m_iDCO2=TERMINAL_NOTVALID;
			iHFFreq=TERMINAL_NOTVALID;
			pMessureData.m_iTVEresp=TERMINAL_NOTVALID; 
			pMessureData.m_iTVEpat=TERMINAL_NOTVALID; 
			iPercent=TERMINAL_NOTVALID; 
		}
		break;
	case VM_SIMV:
	case VM_SIMVPSV:
		{
			pMessureData.m_iHFAmpl=TERMINAL_NOTVALID;
			pMessureData.m_iTVEHFO=TERMINAL_NOTVALID;
			pMessureData.m_iDCO2=TERMINAL_NOTVALID;
			iHFFreq=TERMINAL_NOTVALID;
		}
		break;
	case VM_CPAP:
		{
			pMessureData.m_iHFAmpl=TERMINAL_NOTVALID;
			pMessureData.m_iTVEHFO=TERMINAL_NOTVALID;
			pMessureData.m_iDCO2=TERMINAL_NOTVALID;
			iHFFreq=TERMINAL_NOTVALID;
			pMessureData.m_iTrigVol=TERMINAL_NOTVALID; 
			pMessureData.m_iTVEresp=TERMINAL_NOTVALID; 
			pMessureData.m_iTVEpat=TERMINAL_NOTVALID; 
			iPercent=TERMINAL_NOTVALID; 
		}
		break;
	case VM_DUOPAP:
		{
			pMessureData.m_iHFAmpl=TERMINAL_NOTVALID;
			pMessureData.m_iTVEHFO=TERMINAL_NOTVALID;
			pMessureData.m_iDCO2=TERMINAL_NOTVALID;
			iHFFreq=TERMINAL_NOTVALID;
			pMessureData.m_iTrigVol=TERMINAL_NOTVALID; 
			pMessureData.m_iTVEresp=TERMINAL_NOTVALID; 
			pMessureData.m_iTVEpat=TERMINAL_NOTVALID; 
			iPercent=TERMINAL_NOTVALID; 
		}
		break;
	case VM_NCPAP:
		{
			pMessureData.m_iHFAmpl=TERMINAL_NOTVALID;
			pMessureData.m_iTVEHFO=TERMINAL_NOTVALID;
			pMessureData.m_iDCO2=TERMINAL_NOTVALID;
			iHFFreq=TERMINAL_NOTVALID;
			pMessureData.m_iTrigVol=TERMINAL_NOTVALID; 
			pMessureData.m_iTVEresp=TERMINAL_NOTVALID; 
			pMessureData.m_iTVEpat=TERMINAL_NOTVALID; 
			iPercent=TERMINAL_NOTVALID; 
		}
		break;
	case VM_THERAPIE:
		{
			pMessureData.m_iHFAmpl=TERMINAL_NOTVALID;
			pMessureData.m_iTVEHFO=TERMINAL_NOTVALID;
			pMessureData.m_iDCO2=TERMINAL_NOTVALID;
			iHFFreq=TERMINAL_NOTVALID;
			pMessureData.m_iTrigVol=TERMINAL_NOTVALID; 
			pMessureData.m_iTVEresp=TERMINAL_NOTVALID; 
			pMessureData.m_iTVEpat=TERMINAL_NOTVALID; 
			iPercent=TERMINAL_NOTVALID; 
		}
		break;
	case VM_HFO:
		{
			iBPMco2=TERMINAL_NOTVALID;
			iBPM=TERMINAL_NOTVALID;
			pMessureData.m_iTrigVol=TERMINAL_NOTVALID; 
			iPercent=TERMINAL_NOTVALID; 
			pMessureData.m_iTVE =TERMINAL_NOTVALID;
			pMessureData.m_iTVEresp=TERMINAL_NOTVALID; 
			pMessureData.m_iTVEpat=TERMINAL_NOTVALID; 
			pMessureData.m_iPEEP =TERMINAL_NOTVALID;
		}
		break;
	default:
		{

		}
		break;
	}

	LPTERMINALMSG msg = new TERMINALMSG;
	BYTE checksum=0;

	msg->byBuffer[0]=TERM_MSG_SOM;
	msg->byBuffer[1]=61; //size bytes

	if(m_bMEASUREMENTS_BTB)
	{
		msg->byBuffer[2]=TERMINAL_MEASUREMENTS_BTB; 
	}
	else //if (m_bMEASUREMENTS_AVG)
	{
		msg->byBuffer[2]=TERMINAL_MEASUREMENTS_AVG; 
	}

	msg->byBuffer[3]=(BYTE)eActiveVentMode;
	msg->byBuffer[4]=HIBYTE(pMessureData.m_iPmax);
	msg->byBuffer[5]=LOBYTE(pMessureData.m_iPmax);
	msg->byBuffer[6]=HIBYTE(pMessureData.m_iPmitt);
	msg->byBuffer[7]=LOBYTE(pMessureData.m_iPmitt);
	msg->byBuffer[8]=HIBYTE(pMessureData.m_iPEEP);
	msg->byBuffer[9]=LOBYTE(pMessureData.m_iPEEP);
	msg->byBuffer[10]=HIBYTE(pMessureData.m_iCompliance);
	msg->byBuffer[11]=LOBYTE(pMessureData.m_iCompliance);
	msg->byBuffer[12]=HIBYTE(pMessureData.m_iC20C);
	msg->byBuffer[13]=LOBYTE(pMessureData.m_iC20C);
	msg->byBuffer[14]=HIBYTE(pMessureData.m_iResistance);
	msg->byBuffer[15]=LOBYTE(pMessureData.m_iResistance);
	msg->byBuffer[16]=HIBYTE(pMessureData.m_iMV);
	msg->byBuffer[17]=LOBYTE(pMessureData.m_iMV);
	msg->byBuffer[18]=HIBYTE(pMessureData.m_iTVI);
	msg->byBuffer[19]=LOBYTE(pMessureData.m_iTVI);
	msg->byBuffer[20]=HIBYTE(pMessureData.m_iTVE);
	msg->byBuffer[21]=LOBYTE(pMessureData.m_iTVE);
	msg->byBuffer[22]=HIBYTE(pMessureData.m_iTVEresp);
	msg->byBuffer[23]=LOBYTE(pMessureData.m_iTVEresp);
	msg->byBuffer[24]=HIBYTE(pMessureData.m_iTVEpat);
	msg->byBuffer[25]=LOBYTE(pMessureData.m_iTVEpat);
	msg->byBuffer[26]=HIBYTE(pMessureData.m_iHFAmpl);
	msg->byBuffer[27]=LOBYTE(pMessureData.m_iHFAmpl);
	msg->byBuffer[28]=HIBYTE(pMessureData.m_iTVEHFO);
	msg->byBuffer[29]=LOBYTE(pMessureData.m_iTVEHFO);
	msg->byBuffer[30]=HIBYTE(pMessureData.m_iDCO2);
	msg->byBuffer[31]=LOBYTE(pMessureData.m_iDCO2);
	msg->byBuffer[32]=HIBYTE(pMessureData.m_iTrigVol);
	msg->byBuffer[33]=LOBYTE(pMessureData.m_iTrigVol);
	msg->byBuffer[34]=HIBYTE(pMessureData.m_iITimePSV);
	msg->byBuffer[35]=LOBYTE(pMessureData.m_iITimePSV);
	msg->byBuffer[36]=HIBYTE(iSPO2);
	msg->byBuffer[37]=LOBYTE(iSPO2);
	msg->byBuffer[38]=HIBYTE(iPulseRate);
	msg->byBuffer[39]=LOBYTE(iPulseRate);
	msg->byBuffer[40]=HIBYTE(iPerfusionIndex);
	msg->byBuffer[41]=LOBYTE(iPerfusionIndex);
	msg->byBuffer[42]=HIBYTE(iETCO2);
	msg->byBuffer[43]=LOBYTE(iETCO2);
	msg->byBuffer[44]=HIBYTE(iBPM);
	msg->byBuffer[45]=LOBYTE(iBPM);
	msg->byBuffer[46]=HIBYTE(iBPMco2);
	msg->byBuffer[47]=LOBYTE(iBPMco2);
	msg->byBuffer[48]=HIBYTE(iLeak);
	msg->byBuffer[49]=LOBYTE(iLeak);
	msg->byBuffer[50]=HIBYTE(iHFFreq);
	msg->byBuffer[51]=LOBYTE(iHFFreq);
	msg->byBuffer[52]=HIBYTE(iPercent);
	msg->byBuffer[53]=LOBYTE(iPercent);
	msg->byBuffer[54]=HIBYTE(iOxyVal);
	msg->byBuffer[55]=LOBYTE(iOxyVal);
	msg->byBuffer[56]=HIBYTE(iINSP_FLOW);
	msg->byBuffer[57]=LOBYTE(iINSP_FLOW);
	msg->byBuffer[58]=HIBYTE(iEXP_FLOW);
	msg->byBuffer[59]=LOBYTE(iEXP_FLOW);
	msg->byBuffer[60]=HIBYTE(iDEMAND_FLOW);
	msg->byBuffer[61]=LOBYTE(iDEMAND_FLOW);


	for(int i=1;i<=61;i++)
	{
		checksum += msg->byBuffer[i];
	}
	msg->byBuffer[62]=checksum;

	msg->bySize=63;

	send(msg);


}

/**********************************************************************************************//**
 * Sends a data command
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	command	The command.
 **************************************************************************************************/

void CInterfaceTerminalRemote::sendData_Cmd(BYTE command)
{
	LPTERMINALMSG msg = new TERMINALMSG;
	BYTE checksum=0;

	msg->byBuffer[0]=TERM_MSG_SOM;
	msg->byBuffer[1]=2; //size bytes
	msg->byBuffer[2]=command; 

	for(int i=1;i<=2;i++)
	{
		checksum += msg->byBuffer[i];
	}

	msg->byBuffer[3]=checksum;

	msg->bySize=4;

#ifdef DUMP_TERMINALCMD
	DEBUGMSG(TRUE, (TEXT("Terminal CMD: %x %x %x %x  \r\n"),msg->byBuffer[0],msg->byBuffer[1],msg->byBuffer[2],msg->byBuffer[3]));
#else
	send(msg);
#endif

}

/**********************************************************************************************//**
 * Sends a data short
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	command	The command.
 * \param	val	   	The value.
 **************************************************************************************************/

void CInterfaceTerminalRemote::sendData_SHORT(BYTE command, SHORT val) 
{
	LPTERMINALMSG msg = new TERMINALMSG;
	BYTE checksum=0;

	msg->byBuffer[0]=TERM_MSG_SOM;
	msg->byBuffer[1]=4; //size bytes
	msg->byBuffer[2]=command; 
	msg->byBuffer[3]=HIBYTE(val);
	msg->byBuffer[4]=LOBYTE(val);

	for(int i=1;i<=4;i++)
	{
		checksum += msg->byBuffer[i];
	}
	msg->byBuffer[5]=checksum;

	msg->bySize=6;

#ifdef DUMP_TERMINALCMD
	DEBUGMSG(TRUE, (TEXT("Terminal CMD: %x %x %x %x %x %x\r\n"),msg->byBuffer[0],msg->byBuffer[1],msg->byBuffer[2],msg->byBuffer[3],msg->byBuffer[4],msg->byBuffer[5]));
#else
	send(msg);
#endif
}

/**********************************************************************************************//**
 * Sends a data byte
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	command	The command.
 * \param	val	   	The value.
 **************************************************************************************************/

void CInterfaceTerminalRemote::sendData_BYTE(BYTE command, BYTE val) 
{
	//SOM|NBF|CMD|DATA0|...|DATAn|CKS
	//message header-0x02
	//NBF: number of bytes to follow this byte, including the checksum
	//CMD: command byte
	//DATA byte 0 ... DATA byte n - data bytes 0 thru n
	//CKS: checksum, is computed by adding the data bytes using unsigned modulo 256 arithmetic, beginning with NBF,CMD,DATAbyte 0...DATAbyte n

	LPTERMINALMSG msg = new TERMINALMSG;
	BYTE checksum=0;

	msg->byBuffer[0]=TERM_MSG_SOM;
	msg->byBuffer[1]=3; //size bytes
	msg->byBuffer[2]=command; 
	msg->byBuffer[3]=val;

	for(int i=1;i<=3;i++)
	{
		checksum += msg->byBuffer[i];
	}
	msg->byBuffer[4]=checksum;

	msg->bySize=5;

#ifdef DUMP_TERMINALCMD
	DEBUGMSG(TRUE, (TEXT("Terminal CMD: %x %x %x %x %x\r\n"),msg->byBuffer[0],msg->byBuffer[1],msg->byBuffer[2],msg->byBuffer[3],msg->byBuffer[4]));
#else
	send(msg);
#endif
}

/**********************************************************************************************//**
 * Sets leak compensation
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CInterfaceTerminalRemote::setLeakCompensation(BYTE iVal)
{
	if(iVal<LC_LOW || iVal>LC_HIGH)
	{
		sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
	}
	else
	{
		getModel()->getCONFIG()->setLeakCompensation((eLeakCompensation)iVal);
		if(AfxGetApp() != NULL)
			AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
	}
}

/**********************************************************************************************//**
 * Sets parameter hfo flow
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CInterfaceTerminalRemote::setParam_HFOFlow(WORD iVal)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_HFO:
		{
			fVALUE fv;
			fv.iValue=iVal;

			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFFlowMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFFlowMaxPara();

			if(fv.iValue>fv.iUpperLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else if(fv.iValue<fv.iLowerLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else
			{
				getModel()->getDATAHANDLER()->SetCurrentHFFlowPara(fv.iValue);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
			}
		}
		break;
	default:
		sendData_Cmd(TERM_PARAM_NOSUPPORT);
		break;
	}
}

/**********************************************************************************************//**
 * Sets parameter hfo frequency recruitment
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CInterfaceTerminalRemote::setParam_HFOFreqRec(WORD iVal)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_HFO:
		{
			fVALUE fv;
			fv.iValue=iVal;
			if(getModel()->getDATAHANDLER()->isLUNGRECLicenseAvailable())
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetFreqRecMinPara();
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetFreqRecMaxPara();

				if(fv.iValue>fv.iUpperLimit)
				{
					sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
				}
				else if(fv.iValue<fv.iLowerLimit)
				{
					sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
				}
				else
				{
					getModel()->getDATAHANDLER()->SetCurrentFreqRecPara(fv.iValue);
					if(AfxGetApp() != NULL)
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
				}
			}
			else
			{
				sendData_Cmd(TERM_PARAM_NOSUPPORT);
			}
		}
		break;
	default:
		sendData_Cmd(TERM_PARAM_NOSUPPORT);
		break;
	}
}

/**********************************************************************************************//**
 * Sets parameter pinsp
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CInterfaceTerminalRemote::setParam_PINSP(WORD iVal)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
		{
			fVALUE fv;
			fv.iValue=iVal;
			
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPINSPMinPara_IPPV();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPINSPMaxPara_IPPV();
			
			if(fv.iValue>fv.iUpperLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else if(fv.iValue<fv.iLowerLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else
			{
				getModel()->getDATAHANDLER()->SetCurrentPINSPPara(fv.iValue);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
			}
		}
		break;
	case VM_SERVICE:
	case VM_SIMV:
	case VM_SIPPV:
	case VM_SIMVPSV:
	case VM_PSV:
		{
			fVALUE fv;
			fv.iValue=iVal;
			
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPINSPMinPara_TRIGGER();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPINSPMaxPara_TRIGGER();
			
			if(fv.iValue>fv.iUpperLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else if(fv.iValue<fv.iLowerLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else
			{
				getModel()->getDATAHANDLER()->SetCurrentPINSPPara(fv.iValue);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
			}
		}
		break;
	default:
		sendData_Cmd(TERM_PARAM_NOSUPPORT);
		break;
	}
}

/**********************************************************************************************//**
 * Sets vent mode
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CInterfaceTerminalRemote::setVentMode(BYTE state)
{
	if(state>VM_NONE && state<VM_SERVICE)
	{
		switch(state)
		{
		case VM_IPPV:
			{
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_TERMINAL_IPPV);
			}
			break;
		case VM_SIPPV:
			{
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_TERMINAL_SIPPV);
			}
			break;
		case VM_SIMV:
			{
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_TERMINAL_SIMV);
			}
			break;
		case VM_SIMVPSV:
			{
				if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_MANOFF)
				{
					sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
				}
				else
				{
					if(AfxGetApp() != NULL)
						AfxGetApp()->GetMainWnd()->PostMessage(WM_TERMINAL_SIMVPSV);
				}
			}
			break;
		case VM_PSV:
			{
				if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_MANOFF)
				{
					sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
				}
				else
				{
					if(AfxGetApp() != NULL)
						AfxGetApp()->GetMainWnd()->PostMessage(WM_TERMINAL_PSV);
				}
			}
			break;
		case VM_CPAP:
			{
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_TERMINAL_CPAP);
			}
			break;
		case VM_NCPAP:
			{
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_TERMINAL_NCPAP);
			}
			break;
		case VM_DUOPAP:
			{
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_TERMINAL_DUOPAP);
			}
			break;
		case VM_HFO:
			{
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_TERMINAL_HFO);
			}
			break;
		case VM_THERAPIE:
			{
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_TERMINAL_THERAPY);
			}
			break;
		default:
			sendData_Cmd(TERM_PARAM_NOSUPPORT);
			break;
		}
	}
	else
		sendData_Cmd(TERM_PARAM_NOSUPPORT);
}

/**********************************************************************************************//**
 * Sets pressure rise control
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CInterfaceTerminalRemote::set_PressureRiseCtrl(BYTE iVal)
{
	if(iVal<CURVE_IFLOW || iVal>CURVE_AUTOFLOW)
	{
		sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
	}
	else
	{
		getModel()->getCONFIG()->SetCurPressureRiseCtrl((eCurveForm)iVal);
		getModel()->Send_MODE_OPTION2();

		if(AfxGetApp() != NULL)
			AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
	}
}

/**********************************************************************************************//**
 * Sets manbreath running
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CInterfaceTerminalRemote::set_MANBREATHrunning(BYTE iVal)
{
	if(iVal<0 || iVal>1)
	{
		sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
	}
	else
	{
		eVentMode eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();
		if(		getModel()->getCONFIG()->IsHFOManBreathEnabled()==false 
			&&	getModel()->getVMODEHANDLER()->activeModeIsHFO())
		{
			sendData_Cmd(TERM_PARAM_NOSUPPORT);
		}
		else if(		getModel()->getCONFIG()->IsHFOManBreathEnabled()==true 
			&&	getModel()->getVMODEHANDLER()->activeModeIsHFO()
			&&	getModel()->getDATAHANDLER()->isLUNGRECLicenseAvailable()==true 
			&&	getModel()->getDATAHANDLER()->PARADATA()->GetFreqRecPara()!=0)
		{
			sendData_Cmd(TERM_PARAM_NOSUPPORT);
		}
		else if(eActiveVentMode==VM_THERAPIE)
		{
			sendData_Cmd(TERM_PARAM_NOSUPPORT);
		}
		else if(getModel()->GetVentRunState()==VENT_RUNNING)
		{
			if(iVal==0)
			{
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_DEACTIVATE_MANBREATH);
			}
			else
			{
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_ACTIVATE_MANBREATH);
			}
		}
		else
		{
			sendData_Cmd(TERM_PARAM_NOSUPPORT);
		}
	}
}

/**********************************************************************************************//**
 * Sets parameter IE ratio hfo
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CInterfaceTerminalRemote::setParam_IERatioHFO(BYTE iVal)
{
	if(iVal<RIE_1_3 || iVal>RIE_1_1)
	{
		sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
	}
	else
	{
		fVALUE fv;
		fv.iValue=iVal;

		fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetIERatioMinPara();
		fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetIERatioMaxPara();

		if(fv.iValue>fv.iUpperLimit)
		{
			sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
		}
		else if(fv.iValue<fv.iLowerLimit)
		{
			sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
		}
		else
		{
			getModel()->getDATAHANDLER()->SetCurrentIERatioParaHFO((eRatioIE)iVal);
			if(AfxGetApp() != NULL)
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
		}
		
	}
}

/**********************************************************************************************//**
 * Sets vent range
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CInterfaceTerminalRemote::setVentRange(BYTE iVal)
{
	if(iVal<NEONATAL || iVal>PEDIATRIC)
	{
		sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
	}
	else
	{
		getModel()->getCONFIG()->SetVentRange(iVal);
	}
}

/**********************************************************************************************//**
 * Sets parameter eflow
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CInterfaceTerminalRemote::setParam_EFlow(WORD iVal)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
		{
			fVALUE fv;
			fv.iValue=iVal;

			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetEFLOWMinPara_IPPV();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetEFLOWMaxPara_IPPV();

			if(fv.iValue>fv.iUpperLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else if(fv.iValue<fv.iLowerLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else
			{
				getModel()->getDATAHANDLER()->SetCurrentEFlowPara(fv.iValue);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
			}
		}
		break;
	case VM_SIPPV:
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_PSV:
		{
			fVALUE fv;
			fv.iValue=iVal;

			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetEFLOWMinPara_TRIGGER();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetEFLOWMaxPara_TRIGGER();

			if(fv.iValue>fv.iUpperLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else if(fv.iValue<fv.iLowerLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else
			{
				getModel()->getDATAHANDLER()->SetCurrentEFlowPara(fv.iValue);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
			}
		}
		break;
	default:
		sendData_Cmd(TERM_PARAM_NOSUPPORT);
		break;
	}
}

/**********************************************************************************************//**
 * Sets parameter itime
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CInterfaceTerminalRemote::setParam_ITime(WORD iVal)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
		{
			fVALUE fv;
			fv.iValue=iVal;
			
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetITimeMinPara_IPPV();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetITimeMaxPara_IPPV();

			if(fv.iValue>fv.iUpperLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else if(fv.iValue<fv.iLowerLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else
			{
				getModel()->getDATAHANDLER()->SetCurrentITimePara(fv.iValue);
				if(getModel()->getCONFIG()->GetIERelationMode()==RM_SET_ItoBPM)
				{
					getModel()->getDATAHANDLER()->CalculateETime_ITimeChanged(fv.iValue,false);
				}
				else
				{
					getModel()->getDATAHANDLER()->CalculateBPM_ITimeChanged(fv.iValue,false);
				}
				getModel()->getDATAHANDLER()->SetCurrentTempITime(fv.iValue);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
			}
		}
		break;
	case VM_SIPPV:
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_PSV:
		{
			fVALUE fv;
			fv.iValue=iVal;
			
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetITimeMinPara_TRIGGER();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetITimeMaxPara_TRIGGER();

			if(fv.iValue>fv.iUpperLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else if(fv.iValue<fv.iLowerLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else
			{
				getModel()->getDATAHANDLER()->SetCurrentITimePara(fv.iValue);
				if(getModel()->getCONFIG()->GetIERelationMode()==RM_SET_ItoBPM)
				{
					getModel()->getDATAHANDLER()->CalculateETime_ITimeChanged(fv.iValue,false);
				}
				else
				{
					getModel()->getDATAHANDLER()->CalculateBPM_ITimeChanged(fv.iValue,false);
				}
				getModel()->getDATAHANDLER()->SetCurrentTempITime(fv.iValue);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
			}
		}
		break;
	case VM_DUOPAP:
		{
			fVALUE fvITIME;
			fVALUE fvETIME;

			fvITIME.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetITimeNMODEMinPara();
			fvITIME.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetITimeNMODEMaxPara();
			fvITIME.iValue=iVal;
			fvETIME.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetETIMENMODEPara();

			if(fvITIME.iValue>fvITIME.iUpperLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else if(fvITIME.iValue<fvITIME.iLowerLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else
			{
				getModel()->getDATAHANDLER()->SetCurrentITimePara(fvITIME.iValue);
				if(getModel()->getCONFIG()->GetIERelationMode()==RM_SET_ItoBPM)
				{
					getModel()->getDATAHANDLER()->CalculateETime_ITimeChanged(fvITIME.iValue,false);
				}
				else
				{
					getModel()->getDATAHANDLER()->CalculateBPM_ITimeChanged(fvITIME.iValue,false);
				}
				getModel()->getDATAHANDLER()->SetCurrentTempETime(fvETIME.iValue);
				getModel()->getDATAHANDLER()->SetCurrentTempITime(fvITIME.iValue);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
			}
		}
		break;
	default:
		sendData_Cmd(TERM_PARAM_NOSUPPORT);
		break;
	}
}

/**********************************************************************************************//**
 * Sets parameter etime
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CInterfaceTerminalRemote::setParam_ETime(WORD iVal)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
		{
			fVALUE fv;
			fv.iValue=iVal;

			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetETIMEMinPara_IPPV();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetETIMEMaxPara_IPPV();

			if(fv.iValue>fv.iUpperLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else if(fv.iValue<fv.iLowerLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else
			{
				getModel()->getDATAHANDLER()->SetCurrentETimePara(fv.iValue);
				getModel()->getDATAHANDLER()->CalculateBPM_ETimeChanged(fv.iValue, false);
				getModel()->getDATAHANDLER()->SetCurrentTempETime(fv.iValue);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
			}
		}
		break;
	case VM_SIPPV:
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_PSV:
		{
			fVALUE fv;
			fv.iValue=iVal;

			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetETIMEMinPara_TRIGGER();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetETIMEMaxPara_TRIGGER();

			if(fv.iValue>fv.iUpperLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else if(fv.iValue<fv.iLowerLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else
			{
				getModel()->getDATAHANDLER()->SetCurrentETimePara(fv.iValue);
				getModel()->getDATAHANDLER()->CalculateBPM_ETimeChanged(fv.iValue, false);
				getModel()->getDATAHANDLER()->SetCurrentTempETime(fv.iValue);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
			}
		}
		break;
	case VM_DUOPAP:
		{
			fVALUE fv;
			fv.iValue=iVal;

			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetETIMENMODEMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetETIMENMODEMaxPara();

			if(fv.iValue>fv.iUpperLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else if(fv.iValue<fv.iLowerLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else
			{
				getModel()->getDATAHANDLER()->SetCurrentETimePara(fv.iValue);
				getModel()->getDATAHANDLER()->CalculateBPM_ETimeChanged(fv.iValue, false);
				getModel()->getDATAHANDLER()->SetCurrentTempETime(fv.iValue);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
			}
		}
		break;
	default:
		sendData_Cmd(TERM_PARAM_NOSUPPORT);
		break;
	}
}

/**********************************************************************************************//**
 * Sets parameter rise time
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CInterfaceTerminalRemote::setParam_RiseTime(WORD iVal)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
		{
			fVALUE fv;
			fv.iValue=iVal;
			int iITIME=0;

			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetRisetimeMinPara_IPPV();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetRisetimeMaxPara_IPPV();
			iITIME= getModel()->getDATAHANDLER()->PARADATA()->GetITimePara_IPPV();

			if(fv.iValue>fv.iUpperLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else if(fv.iValue<fv.iLowerLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else if(fv.iValue>iITIME)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else
			{
				getModel()->getDATAHANDLER()->SetCurrentRisetimePara(fv.iValue);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
			}
		}
		break;
	case VM_SIPPV:
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_PSV:
		{
			fVALUE fv;
			fv.iValue=iVal;
			int iITIME=0;

			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetRisetimeMinPara_TRIGGER();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetRisetimeMaxPara_TRIGGER();
			iITIME= getModel()->getDATAHANDLER()->PARADATA()->GetITimePara_TRIGGER();

			if(fv.iValue>fv.iUpperLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else if(fv.iValue<fv.iLowerLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else if(fv.iValue>iITIME)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else
			{
				getModel()->getDATAHANDLER()->SetCurrentRisetimePara(fv.iValue);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
			}
		}
		break;
	default:
		sendData_Cmd(TERM_PARAM_NOSUPPORT);
		break;
	}
}

/**********************************************************************************************//**
 * Sets parameter abort criterion psv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CInterfaceTerminalRemote::setParam_AbortCriterionPSV(BYTE iVal)
{
	if(iVal>MAX_ABORTCRITERION_PSV)
	{
		sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
	}
	else if(iVal<MIN_ABORTCRITERION_PSV)
	{
		sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
	}
	else
	{
		getModel()->getCONFIG()->SetPercentAbortCriterionPSV(iVal);
		getModel()->Send_ABORT_CRITERIONPSV(iVal,false,true);
		if(AfxGetApp() != NULL)
			AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
	}
}

/**********************************************************************************************//**
 * Sets parameter vgarant
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CInterfaceTerminalRemote::setParam_VGarant(WORD iVal)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
		{
			fVALUE fv;
			fv.iValue=iVal;

			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetVGarantMinPara_IPPV();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetVGarantMaxPara_IPPV();

			if(fv.iValue>fv.iUpperLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else if(fv.iValue<fv.iLowerLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else
			{
				getModel()->getDATAHANDLER()->SetCurrentVGarantParamData(fv.iValue);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
			}
		}
		break;
	case VM_SIPPV:
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_PSV:
		{
			fVALUE fv;
			fv.iValue=iVal;

			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetVGarantMinPara_TRIGGER();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetVGarantMaxPara_TRIGGER();

			if(fv.iValue>fv.iUpperLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else if(fv.iValue<fv.iLowerLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else
			{
				getModel()->getDATAHANDLER()->SetCurrentVGarantParamData(fv.iValue);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
			}
		}
		break;
	case VM_HFO:
		{
			fVALUE fv;
			fv.iValue=iVal;

			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFVGarantMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFVGarantMaxPara();

			if(fv.iValue>fv.iUpperLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else if(fv.iValue<fv.iLowerLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else
			{
				getModel()->getDATAHANDLER()->SetCurrentVGarantParamData(fv.iValue);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
			}
		}
		break;
	default:
		sendData_Cmd(TERM_PARAM_NOSUPPORT);
		break;
	}
}

/**********************************************************************************************//**
 * Sets state prico
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CInterfaceTerminalRemote::setState_PRICO(BYTE iVal)
{
	if(getModel()->getCONFIG()->getSPO2module()==SPO2MODULE_MASIMO)
	{
		if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
		{
			if(iVal<0 || iVal>1)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else
			{
				if(iVal==0)
				{
					getModel()->getDATAHANDLER()->setPRICOoff();
				}
				else
				{
					getModel()->getDATAHANDLER()->setPRICOon();
				}

				if(getModel()->getALARMHANDLER()->isPRICOAutoTurnedOff())
				{
					getModel()->getALARMHANDLER()->resetPRICOAutoTurnedOff();
				}
			}
		}
		else
		{
			sendData_Cmd(TERM_PARAM_NOSUPPORT);
		}
	}
	else
	{
		sendData_Cmd(TERM_PARAM_NOSUPPORT);
	}
}

/**********************************************************************************************//**
 * Sets parameter fio2 high
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CInterfaceTerminalRemote::setParam_FIO2HIGH(BYTE iVal)
{
	if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
	{
		fVALUE fv;
		fv.iValue=iVal;

		fv.iLowerLimit=getModel()->getDATAHANDLER()->getPRICO_FIO2lowRange();
		fv.iUpperLimit=100;

		if(fv.iValue>fv.iUpperLimit)
		{
			sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
		}
		else if(fv.iValue<fv.iLowerLimit)
		{
			sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
		}
		else
		{
			getModel()->getDATAHANDLER()->setPRICO_FIO2highRange(fv.iValue);
			if(AfxGetApp() != NULL)
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
		}
	}
	else
	{
		sendData_Cmd(TERM_PARAM_NOSUPPORT);
	}
	
}

/**********************************************************************************************//**
 * Sets parameter fio2 low
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CInterfaceTerminalRemote::setParam_FIO2LOW(BYTE iVal)
{
	if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
	{
		fVALUE fv;
		fv.iValue=iVal;

		fv.iLowerLimit=21;
		fv.iUpperLimit=getModel()->getDATAHANDLER()->getPRICO_FIO2highRange();

		if(fv.iValue>fv.iUpperLimit)
		{
			sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
		}
		else if(fv.iValue<fv.iLowerLimit)
		{
			sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
		}
		else
		{
			getModel()->getDATAHANDLER()->setPRICO_FIO2lowRange(fv.iValue);
			if(AfxGetApp() != NULL)
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
		}
	}
	else
	{
		sendData_Cmd(TERM_PARAM_NOSUPPORT);
	}
}

/**********************************************************************************************//**
 * Sets parameter spo2 high
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CInterfaceTerminalRemote::setParam_SPO2HIGH(BYTE iVal)
{
	if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
	{
		fVALUE fv;
		fv.iValue=iVal;

		eAlarmLimitState limitstateSPO2Max = getModel()->getALARMHANDLER()->getAlimitState_SPO2maxLimit();

		fv.iLowerLimit=getModel()->getDATAHANDLER()->getPRICO_SPO2lowRange();
		if(limitstateSPO2Max==AL_OFF || limitstateSPO2Max==AL_CALC)
			fv.iUpperLimit=100;
		else
			fv.iUpperLimit=CTlsFloat::Round(((double)getModel()->getALARMHANDLER()->getAlimitSPO2max())/10, 0);

		if(fv.iValue>fv.iUpperLimit)
		{
			sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
		}
		else if(fv.iValue<fv.iLowerLimit)
		{
			sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
		}
		else
		{
			getModel()->getDATAHANDLER()->setPRICO_SPO2highRange(fv.iValue);
			if(AfxGetApp() != NULL)
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
		}
	}
	else
	{
		sendData_Cmd(TERM_PARAM_NOSUPPORT);
	}
}

/**********************************************************************************************//**
 * Sets parameter spo2 low
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CInterfaceTerminalRemote::setParam_SPO2LOW(BYTE iVal)
{
	if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
	{
		fVALUE fv;
		fv.iValue=iVal;

		eAlarmLimitState limitstateSPO2Max = getModel()->getALARMHANDLER()->getAlimitState_SPO2maxLimit();
		eAlarmLimitState limitstateSPO2Min = getModel()->getALARMHANDLER()->getAlimitState_SPO2minLimit();

		if(limitstateSPO2Max==AL_OFF || limitstateSPO2Max==AL_CALC)
			fv.iUpperLimit=100;
		else
			fv.iUpperLimit=CTlsFloat::Round(((double)getModel()->getALARMHANDLER()->getAlimitSPO2max())/10, 0);

		if(limitstateSPO2Min==AL_OFF || limitstateSPO2Min==AL_CALC)
			fv.iLowerLimit=0;
		else
			fv.iLowerLimit=CTlsFloat::Round(((double)getModel()->getALARMHANDLER()->getAlimitSPO2min())/10, 0);

		if(fv.iValue>fv.iUpperLimit)
		{
			sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
		}
		else if(fv.iValue<fv.iLowerLimit)
		{
			sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
		}
		else
		{
			getModel()->getDATAHANDLER()->setPRICO_SPO2lowRange(fv.iValue);
			if(AfxGetApp() != NULL)
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
		}
	}
	else
	{
		sendData_Cmd(TERM_PARAM_NOSUPPORT);
	}
}

/**********************************************************************************************//**
 * Sets parameter o2 flush
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CInterfaceTerminalRemote::setParam_O2_FLUSH(BYTE iVal)
{
	fVALUE fv;
	fv.iValue=iVal;

	fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetO2FlushMinPara();
	fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetO2FlushMaxPara();

	if(fv.iValue<(getModel()->getDATAHANDLER()->PRESET()->GetO2Para()+MAXDIFF_O2_FLUSH))
	{
		sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
	}
	else if(fv.iValue>fv.iUpperLimit)
	{
		sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
	}
	else if(fv.iValue<fv.iLowerLimit)
	{
		sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
	}
	else
	{
		getModel()->getDATAHANDLER()->SetCurrentO2FlushPara(fv.iValue);
		if(AfxGetApp() != NULL)
			AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
	}
}

//void CInterfaceTerminalRemote::setParam_ETimeRec(WORD iVal)
//{
//	switch(getModel()->getCONFIG()->GetCurMode())
//	{
//	case VM_HFO:
//		{
//			fVALUE fv;
//			fv.iValue=iVal;
//			if(getModel()->getDATAHANDLER()->isLUNGRECLicenseAvailable())
//			{
//				fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetFreqRecMinPara();
//				fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetFreqRecMaxPara();
//
//				if(fv.iValue>fv.iUpperLimit)
//				{
//					sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
//				}
//				else if(fv.iValue<fv.iLowerLimit)
//				{
//					sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
//				}
//				else
//				{
//					getModel()->getDATAHANDLER()->SetCurrentFreqRecPara(fv.iValue);
//					if(AfxGetApp() != NULL)
//						AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
//				}
//			}
//			else
//			{
//				sendData_Cmd(TERM_PARAM_NOSUPPORT);
//			}
//		}
//		break;
//	default:
//		sendData_Cmd(TERM_PARAM_NOSUPPORT);
//		break;
//	}
//}

/**********************************************************************************************//**
 * Sets parameter itime recruitment
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CInterfaceTerminalRemote::setParam_ITimeRec(WORD iVal)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_HFO:
		{
			fVALUE fv;
			fv.iValue=iVal;
			if(getModel()->getDATAHANDLER()->isLUNGRECLicenseAvailable())
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetITimeRecMinPara();
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetITimeRecMaxPara();

				if(fv.iValue>fv.iUpperLimit)
				{
					sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
				}
				else if(fv.iValue<fv.iLowerLimit)
				{
					sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
				}
				else
				{
					getModel()->getDATAHANDLER()->SetCurrentITimeRecPara(fv.iValue);
					if(AfxGetApp() != NULL)
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
				}
			}
			else
			{
				sendData_Cmd(TERM_PARAM_NOSUPPORT);
			}
		}
		break;
	default:
		sendData_Cmd(TERM_PARAM_NOSUPPORT);
		break;
	}
}

/**********************************************************************************************//**
 * Sets parameter backup
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CInterfaceTerminalRemote::setParam_Backup(BYTE iVal)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_CPAP:
		{
			fVALUE fv;
			fv.iValue=iVal;

			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetBackupMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetBackupMaxPara();

			if(fv.iValue>fv.iUpperLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else if(fv.iValue<fv.iLowerLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else
			{
				getModel()->getDATAHANDLER()->SetCurrentBackupPara(fv.iValue);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
			}
		}
		break;
	default:
		sendData_Cmd(TERM_PARAM_NOSUPPORT);
		break;
	}
}

/**********************************************************************************************//**
 * Sets parameter pmanual
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CInterfaceTerminalRemote::setParam_PManual(WORD iVal)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_CPAP:
		{
			fVALUE fv;
			fv.iValue=iVal;

			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPManualMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPManualMaxPara();

			if(fv.iValue>fv.iUpperLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else if(fv.iValue<fv.iLowerLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else
			{
				getModel()->getDATAHANDLER()->SetCurrentPManPara(fv.iValue);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
			}
		}
		break;
	case VM_NCPAP:
		{
			fVALUE fv;
			fv.iValue=iVal;

			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPManualNMODEMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPManualNMODEMaxPara();

			if(fv.iValue>fv.iUpperLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else if(fv.iValue<fv.iLowerLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else
			{
				getModel()->getDATAHANDLER()->SetCurrentPManPara(fv.iValue);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
			}
		}
		break;
	case VM_HFO:
		{
			fVALUE fv;
			fv.iValue=iVal;

			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPManualMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPManualMaxPara();

			WORD iPmean=getModel()->getDATAHANDLER()->PRESET()->GetHFPMeanPara();
			if(fv.iValue<iPmean+5)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else if(fv.iValue>fv.iUpperLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else if(fv.iValue<fv.iLowerLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else
			{
				getModel()->getDATAHANDLER()->SetPManualHFOParadata(fv.iValue);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
			}
		}
		break;
	default:
		sendData_Cmd(TERM_PARAM_NOSUPPORT);
		break;
	}
}

/**********************************************************************************************//**
 * Sets parameter cpap
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CInterfaceTerminalRemote::setParam_CPAP(WORD iVal)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_CPAP:
		{
			fVALUE fv;
			fv.iValue=iVal;

			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetCPAPMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetCPAPMaxPara();

			if(fv.iValue>fv.iUpperLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else if(fv.iValue<fv.iLowerLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else
			{
				getModel()->getDATAHANDLER()->SetCurrentCPAPPara(fv.iValue);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
			}
		}
		break;
	case VM_NCPAP:
	case VM_DUOPAP:
		{
			fVALUE fv;
			fv.iValue=iVal;

			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetCPAPNMODEMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetCPAPNMODEMaxPara();

			if(fv.iValue>fv.iUpperLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else if(fv.iValue<fv.iLowerLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else
			{
				getModel()->getDATAHANDLER()->SetCurrentCPAPPara(fv.iValue);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
			}
		}
		break;
	default:
		sendData_Cmd(TERM_PARAM_NOSUPPORT);
		break;
	}
}

/**********************************************************************************************//**
 * Sets parameter flowmin
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CInterfaceTerminalRemote::setParam_Flowmin(WORD iVal)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_CPAP:
		{
			fVALUE fv;
			fv.iValue=iVal;

			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetFlowminMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetFlowminMaxPara();

			if(fv.iValue>fv.iUpperLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else if(fv.iValue<fv.iLowerLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else
			{
				getModel()->getDATAHANDLER()->SetFlowminParadata(fv.iValue);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
			}
		}
		break;
	default:
		sendData_Cmd(TERM_PARAM_NOSUPPORT);
		break;
	}
}

/**********************************************************************************************//**
 * Sets parameter trigger
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CInterfaceTerminalRemote::setParam_Trigger(BYTE iVal)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_DUOPAP:
		{
			fVALUE fv;
			fv.iValue=iVal;

			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetTriggerDUOPAPMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetTriggerDUOPAPMaxPara();

			if(fv.iValue>fv.iUpperLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else if(fv.iValue<fv.iLowerLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else
			{
				getModel()->getDATAHANDLER()->SetCurrentTriggerPara(fv.iValue);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
			}
		}
		break;
	case VM_NCPAP:
		{
			fVALUE fv;
			fv.iValue=iVal;

			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetTriggerNCPAPMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetTriggerNCPAPMaxPara();

			if(fv.iValue!=TRIGGER_SENSITIVITY_AUTO)
			{
				fv.iValue=TRIGGER_SENSITIVITY_AUTO;
				getModel()->getDATAHANDLER()->SetCurrentTriggerPara(fv.iValue);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else
			{
				fv.iValue=TRIGGER_SENSITIVITY_AUTO;
				getModel()->getDATAHANDLER()->SetCurrentTriggerPara(fv.iValue);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
			}
		}
		break;
	case VM_CPAP:
		{
			fVALUE fv;
			fv.iValue=iVal;

			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetTriggerCPAPMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetTriggerCPAPMaxPara();

			if(fv.iValue>fv.iUpperLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else if(fv.iValue<fv.iLowerLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else
			{
				getModel()->getDATAHANDLER()->SetCurrentTriggerPara(fv.iValue);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
			}
		}
		break;
	case VM_SIPPV:
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_PSV:
		{
			fVALUE fv;
			fv.iValue=iVal;

			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetTriggerCONVMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetTriggerCONVMaxPara();

			if(fv.iValue>fv.iUpperLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else if(fv.iValue<fv.iLowerLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else
			{
				getModel()->getDATAHANDLER()->SetCurrentTriggerPara(fv.iValue);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
			}
		}
		break;
	default:
		sendData_Cmd(TERM_PARAM_NOSUPPORT);
		break;
	}
}

/**********************************************************************************************//**
 * Sets parameter therapy flow
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CInterfaceTerminalRemote::setParam_TherapieFlow(WORD iVal)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_THERAPIE:
		{
			fVALUE fv;
			fv.iValue=iVal;

			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetTherapieFlowMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetTherapieFlowMaxPara();

			if(fv.iValue>fv.iUpperLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else if(fv.iValue<fv.iLowerLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else
			{
				getModel()->getDATAHANDLER()->SetCurrentTherapieFlowParaData(fv.iValue);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
			}
		}
		break;
	default:
		sendData_Cmd(TERM_PARAM_NOSUPPORT);
		break;
	}
}

/**********************************************************************************************//**
 * Sets parameter vlimit
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CInterfaceTerminalRemote::setParam_VLimit(WORD iVal)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
		{
			fVALUE fv;
			fv.iValue=iVal;

			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetVLimitMinPara_IPPV();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetVLimitMaxPara_IPPV();

			if(fv.iValue>fv.iUpperLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else if(fv.iValue<fv.iLowerLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else
			{
				getModel()->getDATAHANDLER()->SetCurrentVLimitParam(fv.iValue,getModel()->getDATAHANDLER()->IsCurrentModeVLimitStateOn(),true);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
			}
		}
		break;
	case VM_SIPPV:
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_PSV:
		{
			fVALUE fv;
			fv.iValue=iVal;

			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetVLimitMinPara_TRIGGER();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetVLimitMaxPara_TRIGGER();

			if(fv.iValue>fv.iUpperLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else if(fv.iValue<fv.iLowerLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else
			{
				getModel()->getDATAHANDLER()->SetCurrentVLimitParam(fv.iValue,getModel()->getDATAHANDLER()->IsCurrentModeVLimitStateOn(),true);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
			}
		}
		break;
	default:
		sendData_Cmd(TERM_PARAM_NOSUPPORT);
		break;
	}
}

/**********************************************************************************************//**
 * Sets parameter iflow
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CInterfaceTerminalRemote::setParam_IFlow(WORD iVal)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
		{
			fVALUE fv;
			fv.iValue=iVal;

			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetIFlowMinPara_IPPV();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetIFlowMaxPara_IPPV();

			if(fv.iValue>fv.iUpperLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else if(fv.iValue<fv.iLowerLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else
			{
				getModel()->getDATAHANDLER()->SetCurrentIFlowPara(fv.iValue);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
			}
		}
		break;
	case VM_SIPPV:
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_PSV:
		{
			fVALUE fv;
			fv.iValue=iVal;

			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetIFlowMinPara_TRIGGER();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetIFlowMaxPara_TRIGGER();

			if(fv.iValue>fv.iUpperLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else if(fv.iValue<fv.iLowerLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else
			{
				getModel()->getDATAHANDLER()->SetCurrentIFlowPara(fv.iValue);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
			}
		}
		break;
	default:
		sendData_Cmd(TERM_PARAM_NOSUPPORT);
		break;
	}
}

/**********************************************************************************************//**
 * Sets parameter fio2
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CInterfaceTerminalRemote::setParam_FiO2(BYTE iVal)
{
	fVALUE fv;
	fv.iValue=iVal;

	fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetO2MinPara();
	fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetO2MaxPara();

	if(fv.iValue>fv.iUpperLimit)
	{
		sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
	}
	else if(fv.iValue<fv.iLowerLimit)
	{
		sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
	}
	else
	{
		getModel()->getDATAHANDLER()->SetCurrentO2Para(fv.iValue);

		if(AfxGetApp() != NULL)
			AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
	}
}

/**********************************************************************************************//**
 * Sets vent run state
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CInterfaceTerminalRemote::setVent_RunState(BYTE iVal)
{
	bool bTriggerVENT_STOPPED=false;
	bool bTriggerVENT_RUNNING=false;
	bool bTriggerVENT_STANDBY=false;

	switch(iVal)
	{
	case VENT_STOPPED:
		getModel()->getALARMHANDLER()->setAlarmSilent(false,true);
		getModel()->SetVentRunState(VENT_STOPPED);
		bTriggerVENT_STOPPED=true;
		break;
	case VENT_RUNNING:
		getModel()->SetVentRunState(VENT_RUNNING);
		getModel()->getALARMHANDLER()->setAutoSilent(false,true,false);
		bTriggerVENT_RUNNING=true;
		break;
	case VENT_STANDBY:
		getModel()->SetVentRunState(VENT_STANDBY);
		getModel()->getALARMHANDLER()->setAlarmSilent();
		bTriggerVENT_STANDBY=true;
		break;
	default:
		sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
		break;
	}

	if(bTriggerVENT_STOPPED)
	{
		CMVEventControl eventCtrl(CMVEventControl::EV_CONTROL_STOP_VENTILATION);
		getModel()->triggerEvent(&eventCtrl);
	}
	else if(bTriggerVENT_RUNNING)
	{
		CMVEventControl eventCtrl(CMVEventControl::EV_CONTROL_START_VENTILATION);
		getModel()->triggerEvent(&eventCtrl);
	}
	else if(bTriggerVENT_STANDBY)
	{
		CMVEventControl eventCtrl(CMVEventControl::EV_CONTROL_STANDBY);
		getModel()->triggerEvent(&eventCtrl);
	}
}

/**********************************************************************************************//**
 * Sets state vlimit
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CInterfaceTerminalRemote::setState_VLimit(BYTE iVal)
{
	if(false==getModel()->getDATAHANDLER()->isVLIMITLicenseAvailable())
	{
		sendData_Cmd(TERM_PARAM_NOSUPPORT);
	}
	else if(getModel()->getCONFIG()->GetVolumeControl()!=VCTRL_VLIMIT)
	{
		sendData_Cmd(TERM_PARAM_NOSUPPORT);
	}
	else
	{
		if(getModel()->getVMODEHANDLER()->curModeIsVolumeSupportMode())
		{
			switch(iVal)
			{
			case 0:
				getModel()->getDATAHANDLER()->SetCurrentVLimitParam(getModel()->getDATAHANDLER()->PARADATA()->GetVLimitParam_TRIGGER(),false,true);

				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
				break;
			case 1:
				getModel()->getDATAHANDLER()->SetCurrentVLimitParam(getModel()->getDATAHANDLER()->PARADATA()->GetVLimitParam_TRIGGER(),true,true);

				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
				break;
			default:
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
				break;
			}
		}
		else
		{
			sendData_Cmd(TERM_PARAM_NOSUPPORT);
		}
		
	}
	
}

/**********************************************************************************************//**
 * Sets state vgarant
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CInterfaceTerminalRemote::setState_VGarant(BYTE iVal)
{
	if(false==getModel()->getDATAHANDLER()->isVGUARANTLicenseAvailable())
	{
		sendData_Cmd(TERM_PARAM_NOSUPPORT);
	}
	else if(getModel()->getCONFIG()->GetVolumeControl()!=VCTRL_VGARANT)
	{
		sendData_Cmd(TERM_PARAM_NOSUPPORT);
	}
	else
	{
		if(getModel()->getVMODEHANDLER()->curModeIsVolumeSupportMode())
		{
			switch(iVal)
			{
			case 0:
				getModel()->getDATAHANDLER()->ChangeVgarantState(VOLUMEGAR_OFF);

				/*if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);*/
				break;
			case 1:
				getModel()->getDATAHANDLER()->ChangeVgarantState(VOLUMEGAR_ON);

				/*if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);*/
				break;
			default:
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
				break;
			}
		}
		else
		{
			sendData_Cmd(TERM_PARAM_NOSUPPORT);
		}
		
	}
}

/**********************************************************************************************//**
 * Sets parameter hf frequency
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CInterfaceTerminalRemote::setParam_HFFreq(WORD iVal)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_HFO:
		{
			fVALUE fv;
			fv.iValue=iVal;

			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFFreqMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFFreqMaxPara();

			if(fv.iValue>fv.iUpperLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else if(fv.iValue<fv.iLowerLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else
			{
				getModel()->getDATAHANDLER()->SetCurrentHFFreqPara(fv.iValue);

				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
			}
		}
		break;
	default:
		sendData_Cmd(TERM_PARAM_NOSUPPORT);
		break;
	}
}

/**********************************************************************************************//**
 * Sets parameter hf pmean recruitment
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CInterfaceTerminalRemote::setParam_HFPMeanRec(WORD iVal)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_HFO:
		{
			fVALUE fv;
			fv.iValue=iVal;

			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanRecMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanRecMaxPara();

			if(fv.iValue>fv.iUpperLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else if(fv.iValue<fv.iLowerLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else
			{
				getModel()->getDATAHANDLER()->SetCurrentHFPMeanRecPara(fv.iValue);

				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
			}
		}
		break;
	default:
		sendData_Cmd(TERM_PARAM_NOSUPPORT);
		break;
	}
}

/**********************************************************************************************//**
 * Sets parameter hf pmean
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CInterfaceTerminalRemote::setParam_HFPMean(WORD iVal)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_HFO:
		{
			fVALUE fv;
			fv.iValue=iVal;

			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanMaxPara();

			if(fv.iValue>fv.iUpperLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else if(fv.iValue<fv.iLowerLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else
			{
				getModel()->getDATAHANDLER()->SetCurrentPMeanPara(fv.iValue);

				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
			}
		}
		break;
	default:
		sendData_Cmd(TERM_PARAM_NOSUPPORT);
		break;
	}
}

/**********************************************************************************************//**
 * Sets parameter hf amp
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CInterfaceTerminalRemote::setParam_HFAmpl(WORD iVal)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_HFO:
		{
			fVALUE fv;
			fv.iValue=iVal;

			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLMaxPara();

			if(fv.iValue>fv.iUpperLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else if(fv.iValue<fv.iLowerLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else
			{
				getModel()->getDATAHANDLER()->SetCurrentHFAMPLPara(fv.iValue);

				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
			}
		}
		break;
	default:
		sendData_Cmd(TERM_PARAM_NOSUPPORT);
		break;
	}
}

/**********************************************************************************************//**
 * Sets parameter hf amp maximum
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CInterfaceTerminalRemote::setParam_HFAmplMax(WORD iVal)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_HFO:
		{
			fVALUE fv;
			fv.iValue=iVal;

			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLMaxPara();

			if(fv.iValue>fv.iUpperLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else if(fv.iValue<fv.iLowerLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else
			{
				getModel()->getDATAHANDLER()->SetCurrentHFAMPLmaxPara(fv.iValue);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
			}
		}
		break;
	default:
		sendData_Cmd(TERM_PARAM_NOSUPPORT);
		break;
	}
}

/**********************************************************************************************//**
 * Sets parameter bpm
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CInterfaceTerminalRemote::setParam_BPM(WORD iVal)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
		{
			fVALUE fv;
			fv.iValue=iVal;

			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetBPMMinPara_IPPV();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetBPMMaxPara_IPPV();

			if(fv.iValue>fv.iUpperLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else if(fv.iValue<fv.iLowerLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else
			{
				getModel()->getDATAHANDLER()->SetCurrentBPMPara(fv.iValue);
				getModel()->getDATAHANDLER()->CalculateETime_BPMChanged(fv.iValue, false);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
			}
		}
		break;
	case VM_SIPPV:
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_PSV:
		{
			fVALUE fv;
			fv.iValue=iVal;

			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetBPMMinPara_TRIGGER();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetBPMMaxPara_TRIGGER();

			if(fv.iValue>fv.iUpperLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else if(fv.iValue<fv.iLowerLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else
			{
				getModel()->getDATAHANDLER()->SetCurrentBPMPara(fv.iValue);
				getModel()->getDATAHANDLER()->CalculateETime_BPMChanged(fv.iValue, false);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
			}
		}
		break;
	case VM_DUOPAP:
		{
			fVALUE fv;
			fv.iValue=iVal;

			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetBPMNMODEMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetBPMNMODEMaxPara();

			if(fv.iValue>fv.iUpperLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else if(fv.iValue<fv.iLowerLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else
			{
				getModel()->getDATAHANDLER()->SetCurrentBPMPara(fv.iValue);
				getModel()->getDATAHANDLER()->CalculateETime_BPMChanged(fv.iValue, false);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
			}
		}
		break;
	default:
		sendData_Cmd(TERM_PARAM_NOSUPPORT);
		break;
	}

	
}

/**********************************************************************************************//**
 * Sets parameter ppsv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CInterfaceTerminalRemote::setParam_PPSV(WORD iVal)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_SIMVPSV:
		{
			fVALUE fv;
			fv.iValue=iVal;

			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPpsvMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPpsvMaxPara();

			SHORT iPINSP=getModel()->getDATAHANDLER()->PARADATA()->GetPINSPPara_TRIGGER();
			SHORT iPEEP=getModel()->getDATAHANDLER()->PARADATA()->GetPEEPPara_TRIGGER();

			if(fv.iValue>iPINSP)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else if(fv.iValue<iPEEP+20)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else if(fv.iValue>fv.iUpperLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else if(fv.iValue<fv.iLowerLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else
			{
				getModel()->getDATAHANDLER()->SetCurrentPpsvPara(fv.iValue);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
			}
		}
		break;
	case VM_PSV:
		{
			fVALUE fv;
			fv.iValue=iVal;

			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPpsvMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPpsvMaxPara();

			SHORT iPINSP=getModel()->getDATAHANDLER()->PARADATA()->GetPINSPPara_TRIGGER();
			if(fv.iValue>iPINSP)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else if(fv.iValue<(20+getModel()->getDATAHANDLER()->PARADATA()->GetPEEPPara_TRIGGER()))
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else if(fv.iValue>fv.iUpperLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else if(fv.iValue<fv.iLowerLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else
			{
				getModel()->getDATAHANDLER()->SetCurrentPpsvPara(fv.iValue);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
			}
		}
		break;
	default:
		sendData_Cmd(TERM_PARAM_NOSUPPORT);
		break;
	}
}

/**********************************************************************************************//**
 * Sets parameter peep
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CInterfaceTerminalRemote::setParam_PEEP(WORD iVal)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
		{
			fVALUE fv;
			fv.iValue=iVal;

			if(getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_LINEAR)
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPEEPMinPara_RISETIME();
			}
			else
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPEEPMinPara_IPPV();
			}
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPEEPMaxPara_IPPV();

			if(fv.iValue>fv.iUpperLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else if(fv.iValue<fv.iLowerLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else
			{
				getModel()->getDATAHANDLER()->SetCurrentPEEPPara(fv.iValue);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
			}
		}
		break;
	case VM_SERVICE:
	case VM_SIMV:
	case VM_SIPPV:
	case VM_SIMVPSV:
	case VM_PSV:
		{
			fVALUE fv;
			fv.iValue=iVal;

			if(getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_LINEAR)
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPEEPMinPara_RISETIME();
			}
			else
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPEEPMinPara_TRIGGER();
			}
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPEEPMaxPara_TRIGGER();

			if(fv.iValue>fv.iUpperLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else if(fv.iValue<fv.iLowerLimit)
			{
				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
			}
			else
			{
				getModel()->getDATAHANDLER()->SetCurrentPEEPPara(fv.iValue);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
			}
		}
		break;
	default:
		sendData_Cmd(TERM_PARAM_NOSUPPORT);
		break;
	}
}

/**********************************************************************************************//**
 * Sends the mode option 1
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CInterfaceTerminalRemote::sendModeOption1()
{
	SHORT wMode=0;

	if(VENT_RUNNING!=getModel()->GetVentRunState())
		wMode=getModel()->setBitOfWord(wMode, STARTSTOPVENT_BIT);

	if(getModel()->getDATAHANDLER()->IsActiveModeVGarantStateOn()==true)
	{
		wMode=getModel()->setBitOfWord(wMode, PARAVGARANTY_BIT);
	}

	if(getModel()->getDATAHANDLER()->IsActiveModeVLimitStateOn()==true)
	{
		wMode=getModel()->setBitOfWord(wMode, PARAVLIMIT_BIT);
	}

	if(PEDIATRIC==getModel()->getCONFIG()->GetVentRange())
		wMode=getModel()->setBitOfWord(wMode, VENTRANGE_BIT);

	if(true==getModel()->getDATAHANDLER()->IsFlowSensorCalibrating())
		wMode=getModel()->setBitOfWord(wMode, FLOWCALA_BIT);

	if(true==getModel()->getCONFIG()->getBTPS())
	{
		wMode=getModel()->setBitOfWord(wMode, BTPS_BIT);
		//DEBUGMSG(TRUE, (TEXT("send BTPS true\r\n")));
	}
	/*else
	{
		DEBUGMSG(TRUE, (TEXT("send BTPS false\r\n")));
	}*/

	if(true==getModel()->getDATAHANDLER()->GetExhalValvCalMode())
		wMode=getModel()->setBitOfWord(wMode, AVABGLEICH_BIT);

	/*if(false==getCONFIG()->GetVolumeTrigger())
		wMode=setBitOfWord(wMode, TRIGGERMODE_BIT);*/

	if(true==getModel()->getDATAHANDLER()->GetO21SensorCalState())
	{
		//DEBUGMSG(TRUE, (TEXT("OXYCAL21_BIT\r\n")));
		wMode=getModel()->setBitOfWord(wMode, OXYCAL21_BIT);
	}

	if(true==getModel()->getDATAHANDLER()->GetO100SensorCalState())
	{
		//DEBUGMSG(TRUE, (TEXT("OXYCAL100_BIT\r\n")));
		wMode=getModel()->setBitOfWord(wMode, OXYCAL100_BIT);
	}

	eTubeSet eTube=getModel()->getDATAHANDLER()->GetTubeSet();
	switch(eTube)
	{
	case TUBE_INFANTFLOW:
		{
			//DEBUGMSG(TRUE, (TEXT("Send_MODE_OPTION1 TUBE_INFANTFLOW\r\n")));
		}
		break;
	case TUBE_MEDIJET:
		{
			wMode=getModel()->setBitOfWord(wMode, TUBESET10_BIT);
			//DEBUGMSG(TRUE, (TEXT("Send_MODE_OPTION1 TUBE_MEDIJET\r\n")));
		}
		break;
	case TUBE_INFANTFLOW_LP:
		{
			wMode=getModel()->setBitOfWord(wMode, TUBESET11_BIT);
			//DEBUGMSG(TRUE, (TEXT("Send_MODE_OPTION1 TUBE_INFANTFLOW_LP\r\n")));
		}
		break;
	}

	if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
	{
		eRatioIE ratio = getModel()->getDATAHANDLER()->GetCurrentIERatioParaHFO();

		switch(ratio)
		{
		case RIE_1_3:
			{
				//DEBUGMSG(TRUE, (TEXT("Send IERATIO 1:3\r\n")));
			}
			break;
		case RIE_1_2:
			{
				wMode=getModel()->setBitOfWord(wMode, IERATIO12_BIT);
				//DEBUGMSG(TRUE, (TEXT("Send IERATIO 1:2\r\n")));
			}
			break;
		case RIE_1_1:
			{
				wMode=getModel()->setBitOfWord(wMode, IERATIO13_BIT);
				//DEBUGMSG(TRUE, (TEXT("Send IERATIO 1:1\r\n")));
			}
			break;
		}
	}
	else if(getModel()->getCONFIG()->GetCurMode()==VM_SERVICE && getModel()->getCONFIG()->GetPrevMode()==VM_HFO)
	{
		eRatioIE ratio = getModel()->getDATAHANDLER()->GetCurrentIERatioParaHFO();

		switch(ratio)
		{
		case RIE_1_3:
			{
				//DEBUGMSG(TRUE, (TEXT("Send IERATIO 1:3\r\n")));
			}
			break;
		case RIE_1_2:
			{
				wMode=getModel()->setBitOfWord(wMode, IERATIO12_BIT);
				//DEBUGMSG(TRUE, (TEXT("Send IERATIO 1:2\r\n")));
			}
			break;
		case RIE_1_1:
			{
				wMode=getModel()->setBitOfWord(wMode, IERATIO13_BIT);
				//DEBUGMSG(TRUE, (TEXT("Send IERATIO 1:1\r\n")));
			}
			break;
		}
	}

	if(true==getModel()->getDATAHANDLER()->IsVentilBurnRunning())
		wMode=getModel()->setBitOfWord(wMode, VENTILBURN14_BIT);

	//Man. ATZ
	if(true==getModel()->isMANBREATHrunning())
	{
		wMode=getModel()->setBitOfWord(wMode, MANBREAT15_BIT);
	}
	
	sendData_SHORT(TERMINAL_MODE_OPTION1, wMode);
}

/**********************************************************************************************//**
 * Sends the mode option 2
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CInterfaceTerminalRemote::sendModeOption2()
{
	SHORT wMode=0;

	eCurveForm form = getModel()->getCONFIG()->GetCurPressureRiseCtrl();

	switch(form)
	{
	case CURVE_IFLOW:
		{
			//DEBUGMSG(TRUE, (TEXT("Send_MODE_OPTION2 CURVE_IFLOW\r\n")));
		}
		break;
	case CURVE_LINEAR:
		{
			wMode=getModel()->setBitOfWord(wMode, MODOPT2_CURVEFORM1_BIT);
			//DEBUGMSG(TRUE, (TEXT("Send_MODE_OPTION2 CURVE_LINEAR\r\n")));
		}
		break;
	case CURVE_AUTOFLOW: 
		{
			wMode=getModel()->setBitOfWord(wMode, MODOPT2_CURVEFORM2_BIT);
			//DEBUGMSG(TRUE, (TEXT("Send_MODE_OPTION2 CURVE_AUTOFLOW\r\n")));
		}
		break;
	}



	if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
	{
		if(getModel()->getDATAHANDLER()->GetCurrentFreqRecPara()!=0)
		{
			wMode=getModel()->setBitOfWord(wMode, MODOPT2_HFORECRUITMENT_BIT);
		}
	}

	if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
	{
		if(getModel()->getDATAHANDLER()->PARADATA()->GetHFFlowPara()!=0)
		{
			wMode=getModel()->setBitOfWord(wMode, MODOPT2_HFOFLOWMANUAL_BIT);
		}
	}

	/*if(getDATAHANDLER()->isNebulizerOn()==true)
	{
		wMode=setBitOfWord(wMode, MODOPT2_NEBULIZER_BIT);
		
	}*/

	eLeakCompensation eLeakCompOff=getModel()->getCONFIG()->getLeakCompensation();
	switch(eLeakCompOff)
	{
	//case LC_OFF:
	//	{
	//		//
	//	}
	//	break;
	case LC_LOW:
		{
			wMode=getModel()->setBitOfWord(wMode, MODOPT2_LEAKCOMPENSATION1_BIT);
		}
		break;
	case LC_MIDDLE:
		{
			wMode=getModel()->setBitOfWord(wMode, MODOPT2_LEAKCOMPENSATION2_BIT);
		}
		break;
	case LC_HIGH:
		{
			wMode=getModel()->setBitOfWord(wMode, MODOPT2_LEAKCOMPENSATION1_BIT);
			wMode=getModel()->setBitOfWord(wMode, MODOPT2_LEAKCOMPENSATION2_BIT);
		}
		break;
	}

	if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
	{
		if(getModel()->getCONFIG()->getFreshgasExt()==true)
		{
			wMode=getModel()->setBitOfWord(wMode, MODOPT2_FRESHGASEXT_BIT);
			//DEBUGMSG(TRUE, (TEXT("Send_MODE_OPTION2 FreshgasExt\r\n")));
		}
		else
		{
			//DEBUGMSG(TRUE, (TEXT("Send_MODE_OPTION2 FreshgasInt\r\n")));
		}
	}
	else if(getModel()->getVMODEHANDLER()->activeModeIsNMODE())
	{
		wMode=getModel()->setBitOfWord(wMode, MODOPT2_FRESHGASEXT_BIT);
	}

	eTriggereType trigger = TRIGGER_FLOW;
	if(		getModel()->getCONFIG()->GetCurMode()==VM_DUOPAP
		||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_DUOPAP)
	{
		trigger=getModel()->getDATAHANDLER()->getTriggerOptionDUOPAP();
	}
	else if(	getModel()->getCONFIG()->GetCurMode()==VM_NCPAP
		||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_NCPAP)
	{
		trigger=getModel()->getDATAHANDLER()->getTriggerOptionNCPAP();
	}
	/*else if(	getCONFIG()->GetCurMode()==VM_CPAP
		||	getCONFIG()->GetCurMode()==VM_PRE_CPAP)
	{
		trigger=getDATAHANDLER()->getTriggerOption_CPAP();
	}*/
	else
	{
		trigger=getModel()->getDATAHANDLER()->getTriggerOptionCONV();
	}
	switch(trigger)
	{
	case TRIGGER_VOLUME:
		{
		}
		break;
	case TRIGGER_FLOW:
		{
			wMode=getModel()->setBitOfWord(wMode, MODOPT2_TRIGGERFLOW_BIT);
		}
		break;
	case TRIGGER_PRESSURE:
		{
			wMode=getModel()->setBitOfWord(wMode, MODOPT2_TRIGGERPRESSURE_BIT);
		}
		break;
	}
	
	if(true==getModel()->getDATAHANDLER()->getFOToscillationState())
	{
		wMode=getModel()->setBitOfWord(wMode, MODOPT2_FOTRUN_BIT);//BIT9
	}

	sendData_SHORT(TERMINAL_MODE_OPTION2, wMode);
}