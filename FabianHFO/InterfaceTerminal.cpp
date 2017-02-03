//=============================================================================
/** 
* \file InterfaceTerminal.cpp
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
#include "StdAfx.h"
#include "InterfaceTerminal.h"
#include "MVModel.h"

#include "TlsStream.h"

//extern CEvent g_eventTerminalSendData;

CInterfaceTerminal* CInterfaceTerminal::theTerminalInterface=0;
CList<LPTERMINALMSG, LPTERMINALMSG>  CInterfaceTerminal::MSGSend;

#define TERMINAL_NOTVALID -32765	//value NOTVALID

CInterfaceTerminal::CInterfaceTerminal(void)
{
	InitializeCriticalSection(&csEnableSend);
	InitializeCriticalSection(&csMSGSend);

	m_bStarted=true;
	m_bSendWaveData=false;

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

	faSendDataBuf[0]=0x0000;
	fwSendDataSize=0;

	/*m_pcwtTerminalThread=NULL;
	m_bDoTerminalThread=false;

	m_hThreadTerminal=INVALID_HANDLE_VALUE;*/

	//m_pcwtTerminalReceiveThread=NULL;
	m_pcwtTerminalSendThread=NULL;

	m_hThreadTerminalSend=INVALID_HANDLE_VALUE;
	//m_hThreadTerminalReceive=INVALID_HANDLE_VALUE;

	//m_bDoTerminalReceiveThread=false;
	m_bDoTerminalSendThread=false;

	m_iCntBuf=0;

	m_byDellaceAmp=0;
	m_byDellaceFreq=0;

	m_bMEASUREMENTS_BTB=false;
	m_bMEASUREMENTS_AVG=false;
	m_bSendContiniousMeasurements=false;
}

CInterfaceTerminal::~CInterfaceTerminal(void)
{
}

/***************************************************************************/
//     Purpose: 
//**************************************************************************/
CInterfaceTerminal* CInterfaceTerminal::GetInstance()
{
	if(theTerminalInterface == 0)
	{
		theTerminalInterface = new CInterfaceTerminal;
		//theTerminalInterface->Init();
	}
	return theTerminalInterface;
}

/***************************************************************************/
//     Purpose: 
//**************************************************************************/
void CInterfaceTerminal::DestroyInstance()
{
	if(theTerminalInterface != NULL)
	{
		delete theTerminalInterface;
		theTerminalInterface = NULL;
	}
}


/***************************************************************************/
//     Purpose: 
//**************************************************************************/
bool CInterfaceTerminal::Init()
{
	StartTerminalThread();

	OpenCOM();

	Write("fabianHFO");

	return 0;

}

/***************************************************************************/
//     Purpose: 
//**************************************************************************/
bool CInterfaceTerminal::Deinit()
{

	StopTerminalThread();


	if(MSGSend.GetCount()>0)
	{
		POSITION pos = MSGSend.GetHeadPosition();
		if(pos==NULL)
		{

		}
		else
		{
			LPTERMINALMSG szTemp = MSGSend.GetAt(pos);
			MSGSend.RemoveAt(pos);
			delete (void *)szTemp;
		}
	}

	StopListener(-1);
	Close();

	DeleteCriticalSection(&csEnableSend);
	DeleteCriticalSection(&csMSGSend);
	return true;
}

bool CInterfaceTerminal::OpenCOM()
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


/***************************************************************************/
//     Purpose: 
//**************************************************************************/
void CInterfaceTerminal::StartTerminalThread( void )
{
	

	m_bDoTerminalSendThread=true;

	if(m_pcwtTerminalSendThread!=NULL)
	{
		delete m_pcwtTerminalSendThread;
		m_pcwtTerminalSendThread=NULL;

		if(m_hThreadTerminalSend!=INVALID_HANDLE_VALUE)
		{
			HRESULT res=CloseHandle(m_hThreadTerminalSend);
			m_hThreadTerminalSend=INVALID_HANDLE_VALUE;
		}
	}

	m_pcwtTerminalSendThread=AfxBeginThread(CTerminalSendThread,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	m_hThreadTerminalSend=m_pcwtTerminalSendThread->m_hThread;
	m_pcwtTerminalSendThread->m_bAutoDelete = FALSE; 
	m_pcwtTerminalSendThread->ResumeThread();
}

/***************************************************************************/
//     Purpose: 
//**************************************************************************/
void CInterfaceTerminal::StopTerminalThread( void )
{


	if(m_bDoTerminalSendThread)
	{
		m_bDoTerminalSendThread=false;

		//g_eventTerminalSendData.SetEvent();
		
		if (WaitForSingleObject(m_pcwtTerminalSendThread->m_hThread,2000) == WAIT_TIMEOUT)
		{
			theApp.getLog()->WriteLine(_T("#HFO:0213"));
			if(!TerminateThread(m_pcwtTerminalSendThread,0))
			{
			}
		}
	}
}



void CInterfaceTerminal::OnEvent(EEvent eEvent, EError eError)
{
	//SOM|NBF|CMD|DATA0|...|DATAn|CKS
	//message header-0x02
	//NBF: number of bytes to follow this byte, including the checksum
	//CMD: command byte
	//DATA byte 0 ... DATA byte n - data bytes 0 thru n
	//CKS: checksum, is computed by adding the data bytes using unsigned modulo 256 arithmetic, beginning with NBF,CMD,DATAbyte 0...DATAbyte n

	//WORD i;
	//BYTE c;
	//// Handle data receive event
	//if (eEvent & CSerial::EEventRecv)
	//{
	//	do
	//	{
	//		// Read data from the COM-port
	//		Read(faRecvBuf, sizeof(faRecvBuf)-1,&fdwRecvBufLen);

	//		for (i = 0; i < fdwRecvBufLen; i++)
	//		{
	//			c = faRecvBuf[i];
	//			
	//			switch (feRecvState)
	//			{
	//			case TERM_STATE_SOM:

	//				fwRecvDataCnt=0;
	//				fwCalcChkSum = 0;
	//				fwReadChkSum=0;
	//				fwRecvDataSize=0;
	//				fwRecvDataCode=0;
	//				fwRecvDataBufCnt=0;

	//				switch (faRecvBuf[i])
	//				{
	//				case TERM_MSG_SOM:
	//					//feRecvState = TERM_STATE_DATA_SIZE;
	//					feRecvState = TERM_STATE_DATA_SIZE;
	//					//DEBUGMSG(TRUE, (TEXT("TERM_MSG_SOM\r\n")));
	//					break;

	//				default:
	//					// forget the rest
	//					feRecvState = TERM_STATE_SOM;
	//					DEBUGMSG(TRUE, (TEXT("OnEvent5\r\n")));
	//					return;
	//				} // switch (faRecvBuf[i])
	//				break;

	//			case TERM_STATE_DATA_SIZE:

	//				fwCalcChkSum += c;
	//				fwRecvDataSize = c;
	//				feRecvState = TERM_STATE_CODE;
	//				//DEBUGMSG(TRUE, (TEXT("TERM_STATE_DATA_SIZE\r\n")));
	//				break;

	//			case TERM_STATE_CODE:
	//				
	//				fwCalcChkSum += c;
	//				fwRecvDataCode = c;
	//				fwRecvDataCnt++;
	//				if(fwRecvDataSize==2)
	//				{
	//					feRecvState = TERM_STATE_CHKSUM;
	//					//DEBUGMSG(TRUE, (TEXT("TERM_STATE_CODE size==2\r\n")));
	//				}
	//				else
	//				{
	//					feRecvState = TERM_STATE_DATA;
	//					//DEBUGMSG(TRUE, (TEXT("TERM_STATE_CODE\r\n")));
	//				}
	//				
	//				
	//				break;

	//			case TERM_STATE_DATA:

	//				//DEBUGMSG(TRUE, (TEXT("TERM_STATE_DATA\r\n")));
	//				fwCalcChkSum += c;
	//				faDataBuf[fwRecvDataBufCnt] = c;
	//				fwRecvDataBufCnt++;
	//				fwRecvDataCnt++;
	//				feRecvState = TERM_STATE_DATA;

	//				if(fwRecvDataCnt>=(fwRecvDataSize-1))
	//					feRecvState = TERM_STATE_CHKSUM;

	//				break;


	//			/*case TERM_STATE_DATA1:

	//				fwCalcChkSum += c;
	//				faDataBuf[0] = c;
	//				fwDataCnt++;
	//				feRecvState = TERM_STATE_DATA2;

	//				break;

	//			case TERM_STATE_DATA2:

	//				fwCalcChkSum += c;
	//				faDataBuf[1] = c;
	//				fwDataCnt++;

	//				feRecvState = TERM_STATE_CHKSUM;

	//				break;*/

	//			case TERM_STATE_CHKSUM:

	//				//DEBUGMSG(TRUE, (TEXT("TERM_STATE_CHKSUM\r\n")));
	//				fwReadChkSum = c;
	//				if(fwReadChkSum == fwCalcChkSum)
	//				{
	//					//DEBUGMSG(TRUE, (TEXT("checksum ok\r\n")));
	//					//TRACE(_T("checksum ok \n"));
	//					this->PerformMsg();
	//				}
	//				else
	//				{
	//					//checksum error
	//					DEBUGMSG(TRUE, (TEXT("checksum error\r\n")));
	//				}

	//				feRecvState = TERM_STATE_SOM;
	//				break;

	//			default:
	//				return;
	//			} // switch (feRecvState)
	//		} // for i
	//	}
	//	while (fdwRecvBufLen > 0);

	//}

	return;
}

bool CInterfaceTerminal::PerformMsg()
{
	CString szError=_T("");
	bool bNewData=false;
	
	
	switch(fwRecvDataCode)
	{
		//slave messages
		case TERM_GET_MEASUREMENTS_ONCE_BTB:
			{
				DEBUGMSG(TRUE, (TEXT("TERM_GET_MEASUREMENTS_ONCE_BTB\r\n")));
				m_bSendContiniousMeasurements=false;
				m_bMEASUREMENTS_BTB=true;
				sendMessurementData();
				m_bMEASUREMENTS_BTB=false;
				
			}
			break;
		case TERM_GET_MEASUREMENTS_CONTINIOUS_BTB:
			{
				DEBUGMSG(TRUE, (TEXT("TERM_GET_MEASUREMENTS_CONTINIOUS_BTB\r\n")));
				m_bMEASUREMENTS_AVG=false;
				m_bMEASUREMENTS_BTB=true;

				m_bSendContiniousMeasurements=true;
			}
			break;
		case TERM_GET_MEASUREMENTS_ONCE_AVG:
			{
				DEBUGMSG(TRUE, (TEXT("TERM_GET_MEASUREMENTS_ONCE_AVG\r\n")));
				m_bSendContiniousMeasurements=false;
				m_bMEASUREMENTS_AVG=true;
				sendMessurementData();
				m_bMEASUREMENTS_AVG=false;
			}
			break;
		case TERM_GET_MEASUREMENTS_CONTINIOUS_AVG:
			{
				DEBUGMSG(TRUE, (TEXT("TERM_GET_MEASUREMENTS_CONTINIOUS_AVG\r\n")));
				m_bMEASUREMENTS_BTB=false;
				m_bMEASUREMENTS_AVG=true;

				m_bSendContiniousMeasurements=true;
			}
			break;
		case TERM_STOP_CONTINIOUS_MEASUREMENTS:
			{
				DEBUGMSG(TRUE, (TEXT("TERM_STOP_CONTINIOUS_MEASUREMENTS\r\n")));
				m_bSendContiniousMeasurements=false;
				m_bMEASUREMENTS_BTB=false;
				m_bMEASUREMENTS_AVG=false;
			}
			break;
		case TERM_GET_WAVE_DATA:
			{
				DEBUGMSG(TRUE, (TEXT("TERM_GET_WAVE_DATA\r\n")));
				EnterCriticalSection(&csEnableSend);
				m_bSendWaveData=true;
				LeaveCriticalSection(&csEnableSend);
			}
			break;
		case TERM_STOP_WAVE_DATA:
			{
				DEBUGMSG(TRUE, (TEXT("TERM_STOP_WAVE_DATA\r\n")));
				EnterCriticalSection(&csEnableSend);
				m_bSendWaveData=false;
				LeaveCriticalSection(&csEnableSend);
			}
			break;
		case TERM_GET_VENT_MODE:
			{
				DEBUGMSG(TRUE, (TEXT("TERM_GET_VENT_MODE\r\n")));
				eVentMode eActiveVentMode;
				if(getModel()->getCONFIG()->CurModeIsPresetMode()==false)
					eActiveVentMode=getModel()->getCONFIG()->GetCurMode();
				else
					eActiveVentMode=getModel()->getCONFIG()->GetPrevMode();

				sendData_BYTE(TERM_VENT_MODE, (BYTE)eActiveVentMode);
			}
			break;
		case TERM_GET_MODE_OPTION:
			{
				sendModeOption();
			}
			break;
		case TERM_GET_MODE_OPTION2:
			{
				sendModeOption2();
			}
			break;
		/*case TERM_GET_HW_CONFIG:
			{
				sendData_SHORT(TERM_HW_CONFIG, (SHORT)getModel()->getDATAHANDLER()->getHWconfig());
			}
			break;*/
		case TERM_GET_PARAM_IFlow:
			{
				if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
					sendData_SHORT(TERM_PARAM_IFlow, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetIFlowPara_IPPV());
				else
					sendData_SHORT(TERM_PARAM_IFlow, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetIFlowPara_TRIGGER());
			}
			break;
		case TERM_GET_PARAM_EFlow:
			{
				if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
					sendData_SHORT(TERM_PARAM_EFlow, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetEFLOWPara_IPPV());
				else
					sendData_SHORT(TERM_PARAM_EFlow, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetEFLOWPara_TRIGGER());
			}
			break;
		case TERM_GET_PARAM_TherapieFlow:
			{
				sendData_SHORT(TERM_PARAM_TherapieFlow, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetTherapieFLOWPara());
			}
			break;
		case TERM_GET_PARAM_Risetime:
			{
				if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
					sendData_SHORT(TERM_PARAM_Risetime, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetRisetimePara_IPPV());
				else
					sendData_SHORT(TERM_PARAM_Risetime, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetRisetimePara_TRIGGER());
			}
			break;
		case TERM_GET_PARAM_PEEP:
			{
				if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
					sendData_SHORT(TERM_PARAM_PEEP, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetPEEPPara_IPPV());
				else
					sendData_SHORT(TERM_PARAM_PEEP, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetPEEPPara_TRIGGER());
			}
			break;
		case TERM_GET_PARAM_PINSP:
			{
				sendData_SHORT(TERM_PARAM_PINSP, (SHORT)getModel()->getDATAHANDLER()->GetActiveModePINSPPara());//newVG
			}
			break;
		case TERM_GET_PARAM_ITime:
			{
				if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
					sendData_SHORT(TERM_PARAM_ITime, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetITimePara_IPPV());
				else
					sendData_SHORT(TERM_PARAM_ITime, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetITimePara_TRIGGER());
			}
			break;
		case TERM_GET_PARAM_ETime:
			{
				if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
					sendData_SHORT(TERM_PARAM_ETime, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetETIMEPara_IPPV());
				else
					sendData_SHORT(TERM_PARAM_ETime, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetETIMEPara_TRIGGER());
			}
			break;
		case TERM_GET_PARAM_BPM:
			{
				if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
					sendData_SHORT(TERM_PARAM_BPM, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetBPMPara_IPPV());
				else
					sendData_SHORT(TERM_PARAM_BPM, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetBPMPara_TRIGGER());
			}
			break;
		case TERM_GET_PARAM_O2:
			{
				BYTE iO2=0;
				if(getModel()->isO2FlushActive())
					iO2=getModel()->getDATAHANDLER()->PARADATA()->GetO2FlushPara();
				else
					iO2=getModel()->getDATAHANDLER()->PARADATA()->GetO2Para();
				sendData_BYTE(TERM_PARAM_O2, iO2);
			}
			break;
		case TERM_GET_PARAM_VLimit:
			{
				if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
					sendData_SHORT(TERM_PARAM_VLimit, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetVLimitParam_IPPV());
				else
					sendData_SHORT(TERM_PARAM_VLimit, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetVLimitParam_TRIGGER());
			}
			break;
		case TERM_GET_PARAM_VGarant:
			{
				if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
					sendData_SHORT(TERM_PARAM_VGarant, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetVGarantPara_IPPV());
				else
					sendData_SHORT(TERM_PARAM_VGarant, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetVGarantPara_TRIGGER());
			}
			break;
		case TERM_GET_PARAM_PPSV:
			{
				sendData_SHORT(TERM_PARAM_PPSV, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetPpsvPara());
			}
			break;
		case TERM_GET_PARAM_Trigger:
			{
				sendData_BYTE(TERM_PARAM_Trigger, getModel()->getDATAHANDLER()->GetCurrentTriggerPara());
			}
			break;
		case TERM_GET_PARAM_AbortCriterionPSV:
			{
				sendData_SHORT(TERM_PARAM_AbortCriterionPSV, (SHORT)getModel()->getCONFIG()->GetPercentAbortCriterionPSV());
			}
			break;
		case TERM_GET_PARAM_Flowmin:
			{
				sendData_SHORT(TERM_PARAM_Flowmin, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetFlowminPara());
			}
			break;
		case TERM_GET_PARAM_CPAP:
			{
				sendData_SHORT(TERM_PARAM_CPAP, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetCPAPPara());
			}
			break;
		case TERM_GET_PARAM_PManual:
			{
				//sendData_SHORT(TERM_PARAM_PManual, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetPManualPara());
			}
			break;
		case TERM_GET_PARAM_Backup:
			{
				sendData_BYTE(TERM_PARAM_Backup, getModel()->getDATAHANDLER()->PARADATA()->GetBackupPara());
			}
			break;
		case TERM_GET_PARAM_CPAPnmode:
			{
				sendData_SHORT(TERM_PARAM_CPAPnmode, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetCPAPNMODEPara());
			}
			break;
		case TERM_GET_PARAM_PManualnmode:
			{
				sendData_SHORT(TERM_PARAM_PManualnmode, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetPManualNMODEPara());
			}
			break;
		case TERM_GET_PARAM_ITimeNMODE:
			{
				sendData_SHORT(TERM_PARAM_ITimeNMODE, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetITimeNMODEPara());
			}
			break;
		case TERM_GET_PARAM_ETimeNMODE:
			{
				sendData_SHORT(TERM_PARAM_ETimeNMODE, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetETIMENMODEPara());
			}
			break;
		case TERM_GET_PARAM_BPMNMODE:
			{
				sendData_SHORT(TERM_PARAM_BPMNMODE, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetBPMNMODEPara());
			}
			break;
		case TERM_GET_PARAM_HFAmpl:
			{
				WORD iHFAmpl=0;
				if(getModel()->getDATAHANDLER()->IsActiveModeVGarantStateOn())
					iHFAmpl=(int)getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLmaxPara();
				else
					iHFAmpl=(int)getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLPara();

				sendData_SHORT(TERM_PARAM_HFAmpl, (SHORT)iHFAmpl);
			}
			break;
		case TERM_GET_PARAM_HFFreq:
			{
				sendData_BYTE(TERM_PARAM_HFFreq, getModel()->getDATAHANDLER()->PARADATA()->GetHFFreqPara());
			}
			break;
		case TERM_GET_PARAM_HFPMean:
			{
				sendData_SHORT(TERM_PARAM_HFPMean, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanPara());
			}
			break;
		case TERM_GET_PARAM_ITimeRec:
			{
				sendData_SHORT(TERM_PARAM_ITimeRec, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetITimeRecPara());
			}
			break;
		case TERM_GET_PARAM_ETIMERec:
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
				sendData_SHORT(TERM_PARAM_ETIMERec, (SHORT)iETIMERec);
			}
			break;
		case TERM_GET_PARAM_FreqRec:
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
				sendData_SHORT(TERM_PARAM_FreqRec, (SHORT)iFreqRec);
			}
			break;
		case TERM_GET_PARAM_HFPMeanRec:
			{
				sendData_SHORT(TERM_PARAM_HFPMeanRec, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanRecPara());
			}
		case TERM_GET_PARAM_HFFlow:
			{
				sendData_SHORT(TERM_PARAM_HFFlow, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetHFFlowPara());
			}
			break;
		case TERM_GET_PARAM_HFVGarant:
			{
				sendData_SHORT(TERM_PARAM_HFVGarant, (SHORT)getModel()->getDATAHANDLER()->PARADATA()->GetHFVGarantPara());
			}
			break;
		//case TERM_SET_VENT_MODE:
		//	{
		//		DEBUGMSG(TRUE, (TEXT("TERM_SET_VENT_MODE1\r\n")));
		//		BYTE state=faDataBuf[0];
		//		if(state>VM_NONE && state<VM_SERVICE)
		//		{
		//			DEBUGMSG(TRUE, (TEXT("TERM_SET_VENT_MODE2\r\n")));
		//			getModel()->VentModeChanged((eVentMode)state);
		//			/*if(AfxGetApp() != NULL)
		//				AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);*/
		//		}
		//	}
		//	break;
		/*case TERM_SET_MODE_OPTION1:
			{

			}
			break;
		case TERM_SET_MODE_OPTION2:
			{

			}
			break;*/
		case TERM_SET_PARAM_IFlow:
			{
				WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);
				//SHORT iVal2=MAKEWORD(faDataBuf[0],faDataBuf[1]);

				WORD iLowerLimit=0;
				WORD iUpperLimit=0;

				if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
				{
					iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetIFlowMinPara_IPPV();
					iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetIFlowMaxPara_IPPV();
				}
				else
				{
					iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetIFlowMinPara_TRIGGER();
					iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetIFlowMaxPara_TRIGGER();
				}
				if(iVal<=iUpperLimit && iVal>=iLowerLimit)
				{
					getModel()->getDATAHANDLER()->SetCurrentIFlowPara(iVal);
					if(AfxGetApp() != NULL)
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);

					//sendData_Cmd(TERM_PARAM_ACKN);
				}
				else
				{
					sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
				}
			}
			break;
		case TERM_SET_PARAM_EFlow:
			{
				WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);

				WORD iLowerLimit=0;
				WORD iUpperLimit=0;

				if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
				{
					iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetEFLOWMinPara_IPPV();
					iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetEFLOWMaxPara_IPPV();
				}
				else
				{
					iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetEFLOWMinPara_TRIGGER();
					iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetEFLOWMaxPara_TRIGGER();
				}

				if(iVal<=iUpperLimit && iVal>=iLowerLimit)
				{
					getModel()->getDATAHANDLER()->SetEFlowParaData(iVal);
					if(AfxGetApp() != NULL)
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);

					//sendData_Cmd(TERM_PARAM_ACKN);
				}
				else
				{
					sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
				}
			}
			break;
		case TERM_SET_PARAM_TherapieFlow:
			{
				WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);

				WORD iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetTherapieFlowMinPara();
				WORD iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetTherapieFlowMaxPara();

				if(iVal<=iUpperLimit && iVal>=iLowerLimit)
				{
					getModel()->getDATAHANDLER()->SetCurrentTherapieFlowParaData(iVal);
					if(AfxGetApp() != NULL)
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);

					//sendData_Cmd(TERM_PARAM_ACKN);
				}
				else
				{
					sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
				}
			}
			break;
		case TERM_SET_PARAM_RiseTime:
			{
				WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);

				WORD iLowerLimit=0;
				WORD iUpperLimit=0;

				if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
				{
					iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetRisetimeMinPara_IPPV();
					iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetRisetimeMaxPara_IPPV();
				}
				else
				{
					iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetRisetimeMinPara_TRIGGER();
					iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetRisetimeMaxPara_TRIGGER();
				}
				if(iVal<=iUpperLimit && iVal>=iLowerLimit)
				{
					getModel()->getDATAHANDLER()->SetCurrentRisetimePara(iVal);
					if(AfxGetApp() != NULL)
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);

					//sendData_Cmd(TERM_PARAM_ACKN);
				}
				else
				{
					sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
				}
			}
			break;
		case TERM_SET_PARAM_PEEP:
			{
				//WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);

				//WORD iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPPSVMinPara();
				//WORD iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPPSVMaxPara();

				//if(iVal<=iUpperLimit && iVal>=iLowerLimit)
				//{
				//	getModel()->getDATAHANDLER()->SetCurrentPEEPPara(iVal);
				//	if(AfxGetApp() != NULL)
				//		AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);

				//	//sendData_Cmd(TERM_PARAM_ACKN);
				//}
				//else
				//{
				//	sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
				//}
				
				sendData_Cmd(TERM_PARAM_NOSUPPORT);
			}
			break;
		case TERM_SET_PARAM_PINSP:
			{
				//WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);

				//WORD iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPInspMinPara();
				//WORD iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPInspMaxPara();

				//if(iVal<=iUpperLimit && iVal>=iLowerLimit)
				//{
				//	getModel()->getDATAHANDLER()->SetCurrentPinspPara(iVal);
				//	if(AfxGetApp() != NULL)
				//		AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);

				//	//sendData_Cmd(TERM_PARAM_ACKN);
				//}
				//else
				//{
				//	sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
				//}

				sendData_Cmd(TERM_PARAM_NOSUPPORT);
			}
			break;
		case TERM_SET_PARAM_ITime:
			{
				WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);

				WORD iLowerLimit=0;
				WORD iUpperLimit=0;

				if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
				{
					iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetITimeMinPara_IPPV();
					iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetITimeMaxPara_IPPV();
				}
				else
				{
					iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetITimeMinPara_TRIGGER();
					iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetITimeMaxPara_TRIGGER();
				}
				if(iVal<=iUpperLimit && iVal>=iLowerLimit)
				{
					getModel()->getDATAHANDLER()->SetCurrentITimePara(iVal);
					if(getModel()->getCONFIG()->GetIERelationMode()==RM_SET_ItoBPM)
					{
						getModel()->getDATAHANDLER()->CalculateETime_ITimeChanged(iVal,false);
					}
					else
					{
						getModel()->getDATAHANDLER()->CalculateBPM_ITimeChanged(iVal,false);
					}
					getModel()->getDATAHANDLER()->SetCurrentTempITime(iVal);
					if(AfxGetApp() != NULL)
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);

					//sendData_Cmd(TERM_PARAM_ACKN);
				}
				else
				{
					sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
				}
			}
			break;
		case TERM_SET_PARAM_ETime:
			{
				WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);

				WORD iLowerLimit=0;
				WORD iUpperLimit=0;

				if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
				{
					iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetETIMEMinPara_IPPV();
					iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetETIMEMaxPara_IPPV();
				}
				else
				{
					iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetETIMEMinPara_TRIGGER();
					iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetETIMEMaxPara_TRIGGER();
				}
				if(iVal<=iUpperLimit && iVal>=iLowerLimit)
				{
					getModel()->getDATAHANDLER()->SetCurrentETimePara(iVal);
					getModel()->getDATAHANDLER()->CalculateBPM_ETimeChanged(iVal, false);
					getModel()->getDATAHANDLER()->SetCurrentTempETime(iVal);
					if(AfxGetApp() != NULL)
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);

					//sendData_Cmd(TERM_PARAM_ACKN);
				}
				else
				{
					sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
				}
			}
			break;
		case TERM_SET_PARAM_BPM:
			{
				WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);

				WORD iLowerLimit=0;
				WORD iUpperLimit=0;

				if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
				{
					iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetBPMMinPara_IPPV();
					iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetBPMMaxPara_IPPV();
				}
				else
				{
					iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetBPMMinPara_TRIGGER();
					iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetBPMMaxPara_TRIGGER();

				}
				if(iVal<=iUpperLimit && iVal>=iLowerLimit)
				{
					getModel()->getDATAHANDLER()->SetCurrentBPMPara(iVal);
					getModel()->getDATAHANDLER()->CalculateETime_BPMChanged(iVal, false);
					if(AfxGetApp() != NULL)
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);

					//sendData_Cmd(TERM_PARAM_ACKN);
				}
				else
				{
					sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
				}
			}
			break;
		case TERM_SET_PARAM_O2:
			{
				WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);

				WORD iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetO2MinPara();
				WORD iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetO2MaxPara();

				if(iVal<=iUpperLimit && iVal>=iLowerLimit)
				{
					getModel()->getDATAHANDLER()->SetCurrentO2Para((BYTE)iVal);
					if(AfxGetApp() != NULL)
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);

					//sendData_Cmd(TERM_PARAM_ACKN);
				}
				else
				{
					sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
				}	
			}
			break;
		case TERM_SET_PARAM_O2_FLUSH:
			{
				WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);

				WORD iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetO2FlushMinPara();
				WORD iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetO2FlushMaxPara();

				

				if(iVal<=iUpperLimit && iVal>=iLowerLimit)
				{
					if(iVal<(getModel()->getDATAHANDLER()->PARADATA()->GetO2Para()+MAXDIFF_O2_FLUSH))
					{
						sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
					}
					else
					{
						getModel()->getDATAHANDLER()->SetCurrentO2FlushPara((BYTE)iVal);
						if(AfxGetApp() != NULL)
							AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);
					}

					//sendData_Cmd(TERM_PARAM_ACKN);
				}
				else
				{
					sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
				}
			}
			break;
		//case TERM_SET_PARAM_VLimit:
		//	{
		//		WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);

		//		WORD iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetXXXMinPara();
		//		WORD iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetXXXMaxPara();

		//		if(iVal<=iUpperLimit && iVal>=iLowerLimit)
		//		{
		//			getModel()->getDATAHANDLER()->SetCurrentXXXPara(iVal);
		//			if(AfxGetApp() != NULL)
		//				AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);

		//			//sendData_Cmd(TERM_PARAM_ACKN);
		//		}
		//		else
		//		{
		//			sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
		//		}
		//	}
		//	break;
		//case TERM_SET_PARAM_VGarant:
		//	{
		//		if(getModel()->getDATAHANDLER()->isVGUARANTLicenseAvailable())
		//		{
		//			WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);

		//			WORD iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetVGarantMinPara();
		//			WORD iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetVGarantMaxPara();

		//			if(iVal<=iUpperLimit && iVal>=iLowerLimit)
		//			{
		//				if(iVal==0)
		//					getModel()->getDATAHANDLER()->SetCurrentVGarantParamData(iVal,false,true);
		//				else
		//					getModel()->getDATAHANDLER()->SetCurrentVGarantParamData(iVal,true,true);

		//				if(AfxGetApp() != NULL)
		//					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);

		//				//sendData_Cmd(TERM_PARAM_ACKN);
		//			}
		//			else
		//			{
		//				sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
		//			}
		//		}
		//		else
		//		{
		//			sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
		//		}
		//		
		//	}
		//	break;
		case TERM_SET_PARAM_PPSV:
			{
				WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);

				WORD iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPpsvMinPara();
				WORD iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPpsvMaxPara();

				SHORT iPEEP=0;
				SHORT iPINSP=0;

				if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
				{
					iPEEP=getModel()->getDATAHANDLER()->PARADATA()->GetPEEPPara_IPPV();
					iPINSP=getModel()->getDATAHANDLER()->PARADATA()->GetPINSPPara_IPPV();
				}
				else
				{
					iPEEP=getModel()->getDATAHANDLER()->PARADATA()->GetPEEPPara_TRIGGER();
					iPINSP=getModel()->getDATAHANDLER()->PARADATA()->GetPINSPPara_TRIGGER();
				}
				

				if(iVal>iPINSP)
				{
					sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
				}
				else if(iVal<iPEEP+20)
				{
					sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
				}
				else if(iVal<=iUpperLimit && iVal>=iLowerLimit)
				{
					getModel()->getDATAHANDLER()->SetCurrentPpsvPara(iVal);
					if(AfxGetApp() != NULL)
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);

					//sendData_Cmd(TERM_PARAM_ACKN);
				}
				else
				{
					sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
				}

				sendData_Cmd(TERM_PARAM_NOSUPPORT);
			}
			break;
		case TERM_SET_PARAM_Trigger:
			{
				WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);

				WORD iLowerLimit=0;
				WORD iUpperLimit=0;
				if(		getModel()->getCONFIG()->GetCurMode()==VM_DUOPAP
					||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_DUOPAP
					||	getModel()->getCONFIG()->GetCurMode()==VM_NCPAP
					||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_NCPAP)
				{
					iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetTriggerNMODEMinPara();
					iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetTriggerNMODEMaxPara();
				}
				else
				{
					iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetTriggerCONVMinPara();
					iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetTriggerCONVMaxPara();
				}

				if(iVal<=iUpperLimit && iVal>=iLowerLimit)
				{
					getModel()->getDATAHANDLER()->SetCurrentTriggerPara((BYTE)iVal);
					if(AfxGetApp() != NULL)
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);

					//sendData_Cmd(TERM_PARAM_ACKN);
				}
				else
				{
					sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
				}
			}
			break;
		case TERM_SET_PARAM_AbortCriterionPSV:
			{
				//WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);

				//WORD iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetXXXMinPara();
				//WORD iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetXXXMaxPara();

				//if(iVal<=iUpperLimit && iVal>=iLowerLimit)
				//{
				//	getModel()->getDATAHANDLER()->SetCurrentXXXPara(iVal);
				//	if(AfxGetApp() != NULL)
				//		AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);

				//	//sendData_Cmd(TERM_PARAM_ACKN);
				//}
				//else
				//{
				//	sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
				//}

				sendData_Cmd(TERM_PARAM_NOSUPPORT);
			}
			break;
		case TERM_SET_PARAM_Flowmin:
			{
				WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);

				WORD iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetFlowminMinPara();
				WORD iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetFlowminMaxPara();

				if(iVal<=iUpperLimit && iVal>=iLowerLimit)
				{
					getModel()->getDATAHANDLER()->SetFlowMinParadata(iVal);
					if(AfxGetApp() != NULL)
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);

					//sendData_Cmd(TERM_PARAM_ACKN);
				}
				else
				{
					sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
				}
			}
			break;
		case TERM_SET_PARAM_CPAP:
			{
				WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);

				WORD iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetCPAPMinPara();
				WORD iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetCPAPMaxPara();

				if(iVal<=iUpperLimit && iVal>=iLowerLimit)
				{
					getModel()->getDATAHANDLER()->SetCurrentCPAPPara(iVal);
					if(AfxGetApp() != NULL)
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);

					//sendData_Cmd(TERM_PARAM_ACKN);
				}
				else
				{
					sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
				}
			}
			break;
		case TERM_SET_PARAM_PManual:
			{
				WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);

				WORD iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPManualMinPara();
				WORD iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPManualMaxPara();

				if(iVal<=iUpperLimit && iVal>=iLowerLimit)
				{
					getModel()->getDATAHANDLER()->SetCurrentPManPara(iVal);
					if(AfxGetApp() != NULL)
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);

					//sendData_Cmd(TERM_PARAM_ACKN);
				}
				else
				{
					sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
				}
			}
			break;
		case TERM_SET_PARAM_Backup:
			{
				WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);

				WORD iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetBackupMinPara();
				WORD iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetBackupMaxPara();

				if(iVal<=iUpperLimit && iVal>=iLowerLimit)
				{
					getModel()->getDATAHANDLER()->SetCurrentBackupPara((BYTE)iVal);
					if(AfxGetApp() != NULL)
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);

					//sendData_Cmd(TERM_PARAM_ACKN);
				}
				else
				{
					sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
				}
			}
			break;
		case TERM_SET_PARAM_CPAPnmode:
			{
				WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);

				WORD iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetCPAPNMODEMinPara();
				WORD iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetCPAPNMODEMaxPara();

				if(iVal<=iUpperLimit && iVal>=iLowerLimit)
				{
					getModel()->getDATAHANDLER()->SetCurrentCPAPPara(iVal);
					if(AfxGetApp() != NULL)
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);

					//sendData_Cmd(TERM_PARAM_ACKN);
				}
				else
				{
					sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
				}
			}
			break;
		case TERM_SET_PARAM_PManualnmode:
			{
				WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);

				WORD iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPManualNMODEMinPara();
				WORD iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPManualNMODEMaxPara();

				if(iVal<=iUpperLimit && iVal>=iLowerLimit)
				{
					getModel()->getDATAHANDLER()->SetCurrentPManPara(iVal);
					if(AfxGetApp() != NULL)
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);

					//sendData_Cmd(TERM_PARAM_ACKN);
				}
				else
				{
					sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
				}
			}
			break;
		case TERM_SET_PARAM_ITimeNMODE:
			{
				WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);

				WORD iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetITimeNMODEMinPara();
				WORD iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetITimeNMODEMaxPara();

				if(iVal<=iUpperLimit && iVal>=iLowerLimit)
				{
					getModel()->getDATAHANDLER()->SetCurrentITimePara(iVal);
					if(getModel()->getCONFIG()->GetIERelationMode()==RM_SET_ItoBPM)
					{
						getModel()->getDATAHANDLER()->CalculateETime_ITimeChanged(iVal,false);
					}
					else
					{
						getModel()->getDATAHANDLER()->CalculateBPM_ITimeChanged(iVal,false);
					}
					getModel()->getDATAHANDLER()->SetCurrentTempITime(iVal);
					if(AfxGetApp() != NULL)
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);

					//sendData_Cmd(TERM_PARAM_ACKN);
				}
				else
				{
					sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
				}
			}
			break;
		case TERM_SET_PARAM_ETimeNMODE:
			{
				WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);

				WORD iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetETIMENMODEMinPara();
				WORD iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetETIMENMODEMaxPara();

				if(iVal<=iUpperLimit && iVal>=iLowerLimit)
				{
					getModel()->getDATAHANDLER()->SetCurrentETimePara(iVal);
					getModel()->getDATAHANDLER()->CalculateBPM_ETimeChanged(iVal, false);
					getModel()->getDATAHANDLER()->SetCurrentTempETime(iVal);
					if(AfxGetApp() != NULL)
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);

					//sendData_Cmd(TERM_PARAM_ACKN);
				}
				else
				{
					sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
				}
			}
			break;
		case TERM_SET_PARAM_BPMNMODE:
			{
				WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);

				WORD iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetBPMNMODEMinPara();
				WORD iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetBPMNMODEMaxPara();

				if(iVal<=iUpperLimit && iVal>=iLowerLimit)
				{
					getModel()->getDATAHANDLER()->SetCurrentBPMPara(iVal);
					getModel()->getDATAHANDLER()->CalculateETime_BPMChanged(iVal, false);
					if(AfxGetApp() != NULL)
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);

					//sendData_Cmd(TERM_PARAM_ACKN);
				}
				else
				{
					sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
				}
			}
			break;
		case TERM_SET_PARAM_HFAmpl:
			{
				WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);

				WORD iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLMinPara();
				WORD iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLMaxPara();

				if(iVal<=iUpperLimit && iVal>=iLowerLimit)
				{
					if(getModel()->getDATAHANDLER()->IsActiveModeVGarantStateOn())
						getModel()->getDATAHANDLER()->SetCurrentHFAMPLmaxPara(iVal);
					else
						getModel()->getDATAHANDLER()->SetCurrentHFAMPLPara(iVal);
					if(AfxGetApp() != NULL)
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);

					//sendData_Cmd(TERM_PARAM_ACKN);
				}
				else
				{
					sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
				}
			}
			break;
		case TERM_SET_PARAM_HFFreq:
			{
				WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);

				WORD iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFFreqMinPara();
				WORD iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFFreqMaxPara();

				if(iVal<=iUpperLimit && iVal>=iLowerLimit)
				{
					getModel()->getDATAHANDLER()->SetCurrentHFFreqPara((BYTE)iVal);
					if(AfxGetApp() != NULL)
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);

					//sendData_Cmd(TERM_PARAM_ACKN);
				}
				else
				{
					sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
				}
			}
			break;
		case TERM_SET_PARAM_HFPMean:
			{
				WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);

				WORD iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanMinPara();
				WORD iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanMaxPara();

				if(iVal<=iUpperLimit && iVal>=iLowerLimit)
				{
					getModel()->getDATAHANDLER()->SetCurrentPMeanPara(iVal);
					if(AfxGetApp() != NULL)
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);

					//sendData_Cmd(TERM_PARAM_ACKN);
				}
				else
				{
					sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
				}
			}
			break;
		case TERM_SET_PARAM_ITimeRec:
			{
				if(getModel()->getDATAHANDLER()->isLUNGRECLicenseAvailable())
				{
					WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);

					WORD iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetITimeRecMinPara();
					WORD iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetITimeRecMaxPara();

					if(iVal<=iUpperLimit && iVal>=iLowerLimit)
					{
						getModel()->getDATAHANDLER()->SetCurrentITimeRecPara(iVal);
						if(AfxGetApp() != NULL)
							AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);

						//sendData_Cmd(TERM_PARAM_ACKN);
					}
					else
					{
						sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
					}
				}
				else
				{
					sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
				}

				
			}
			break;
		case TERM_SET_PARAM_ETIMERec:
			{
				/*if(getModel()->getDATAHANDLER()->isLUNGRECLicenseAvailable())
				{
				}*/

				//WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);

				//WORD iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetXXXMinPara();
				//WORD iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetXXXMaxPara();

				//if(iVal<=iUpperLimit && iVal>=iLowerLimit)
				//{
				//	getModel()->getDATAHANDLER()->SetCurrentXXXPara(iVal);
				//	if(AfxGetApp() != NULL)
				//		AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);

				//	//sendData_Cmd(TERM_PARAM_ACKN);
				//}
				//else
				//{
				//	sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
				//}

				sendData_Cmd(TERM_PARAM_NOSUPPORT);
			}
			break;
		case TERM_SET_PARAM_FreqRec:
			{
				if(getModel()->getDATAHANDLER()->isLUNGRECLicenseAvailable())
				{
					WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);

					WORD iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetFreqRecMinPara();
					WORD iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetFreqRecMaxPara();

					if(iVal<=iUpperLimit && iVal>=iLowerLimit)
					{
						getModel()->getDATAHANDLER()->SetCurrentFreqRecPara(iVal);
						if(AfxGetApp() != NULL)
							AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);

						//sendData_Cmd(TERM_PARAM_ACKN);
					}
					else
					{
						sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
					}
				}
				else
				{
					sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
				}
				
			}
			break;
		case TERM_SET_PARAM_HFPMeanRec:
			{
				if(getModel()->getDATAHANDLER()->isLUNGRECLicenseAvailable())
				{
					WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);

					WORD iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanRecMinPara();
					WORD iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanRecMaxPara();

					if(iVal<=iUpperLimit && iVal>=iLowerLimit)
					{
						getModel()->getDATAHANDLER()->SetCurrentHFPMeanRecPara(iVal);
						if(AfxGetApp() != NULL)
							AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);

						//sendData_Cmd(TERM_PARAM_ACKN);
					}
					else
					{
						sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
					}
				}
				else
				{
					sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
				}
				
			}
			break;
		case TERM_SET_PARAM_HFFlow:
			{
				WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);

				WORD iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFFlowMinPara();
				WORD iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFFlowMaxPara();

				if(iVal<=iUpperLimit && iVal>=iLowerLimit)
				{
					getModel()->getDATAHANDLER()->SetCurrentHFFlowPara(iVal);
					if(AfxGetApp() != NULL)
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);

					//sendData_Cmd(TERM_PARAM_ACKN);
				}
				else
				{
					sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
				}
			}
			break;
		case TERM_SET_PARAM_HFVGarant:
			{
				//WORD iVal=MAKEWORD(faDataBuf[1],faDataBuf[0]);

				//WORD iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetXXXMinPara();
				//WORD iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetXXXMaxPara();

				//if(iVal<=iUpperLimit && iVal>=iLowerLimit)
				//{
				//	getModel()->getDATAHANDLER()->SetCurrentXXXPara(iVal);
				//	if(AfxGetApp() != NULL)
				//		AfxGetApp()->GetMainWnd()->PostMessage(WM_PARADATA_CHANGED);

				//	//sendData_Cmd(TERM_PARAM_ACKN);
				//}
				//else
				//{
				//	sendData_Cmd(TERM_PARAM_OUTOFFRANGE);
				//}

				sendData_Cmd(TERM_PARAM_NOSUPPORT);
			}
			break;
		default:
			{
				

			}
			break;
	}


	
	return true;
}

void CInterfaceTerminal::sendModeOption()
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

	if(true==getModel()->getDATAHANDLER()->GetExhalValvCalMode())
		wMode=getModel()->setBitOfWord(wMode, AVABGLEICH_BIT);

	/*if(false==getModel()->getCONFIG()->GetVolumeTrigger())
		wMode=getModel()->setBitOfWord(wMode, TRIGGERMODE_BIT);*/
	eTriggereType trigger = TRIGGER_FLOW;
	if(		getModel()->getCONFIG()->GetCurMode()==VM_DUOPAP
		||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_DUOPAP
		||	getModel()->getCONFIG()->GetCurMode()==VM_NCPAP
		||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_NCPAP)
	{
		trigger=getModel()->getDATAHANDLER()->getTriggerOptionNMODE();
	}
	else
	{
		trigger=getModel()->getDATAHANDLER()->getTriggerOptionCONV();//&& getModel()->getVMODEHANDLER()->activeModeIsTriggerMode()
	}
	switch(trigger)
	{
	case TRIGGER_VOLUME:
		{
			//DEBUGMSG(TRUE, (TEXT("Send_MODE_OPTION2 TRIGGER_VOLUME\r\n")));
		}
		break;
	case TRIGGER_FLOW:
		{
			getModel()->setBitOfWord(wMode, MODOPT2_TRIGGERFLOW_BIT);
			//DEBUGMSG(TRUE, (TEXT("Send_MODE_OPTION2 TRIGGER_FLOW\r\n")));
		}
		break;
	case TRIGGER_PRESSURE:
		{
			getModel()->setBitOfWord(wMode, MODOPT2_TRIGGERPRESSURE_BIT);
			//DEBUGMSG(TRUE, (TEXT("Send_MODE_OPTION2 MODOPT2_TRIGGERPRESSURE_BIT\r\n")));
		}
		break;
	}


	if(true==getModel()->getDATAHANDLER()->GetO21SensorCalState())
	{
		wMode=getModel()->setBitOfWord(wMode, OXYCAL21_BIT);
	}

	if(true==getModel()->getDATAHANDLER()->GetO100SensorCalState())
		wMode=getModel()->setBitOfWord(wMode, OXYCAL100_BIT);

	eTubeSet eTube=getModel()->getDATAHANDLER()->GetTubeSet();
	switch(eTube)
	{
	/*case TUBE_INFANTFLOW:
		{
			wMode=setBitOfWord(wMode, TUBESET11_BIT);
		}
		break;*/
	case TUBE_MEDIJET:
		{
			wMode=getModel()->setBitOfWord(wMode, TUBESET10_BIT);
		}
		break;
	case TUBE_INFANTFLOW_LP:
		{
			wMode=getModel()->setBitOfWord(wMode, TUBESET11_BIT);
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
			}
			break;
		case RIE_1_2:
			{
				wMode=getModel()->setBitOfWord(wMode, IERATIO12_BIT);
			}
			break;
		case RIE_1_1:
			{
				wMode=getModel()->setBitOfWord(wMode, IERATIO13_BIT);
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
			}
			break;
		case RIE_1_2:
			{
				wMode=getModel()->setBitOfWord(wMode, IERATIO12_BIT);
			}
			break;
		case RIE_1_1:
			{
				wMode=getModel()->setBitOfWord(wMode, IERATIO13_BIT);
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
	
	sendData_SHORT(TERM_MODE_OPTION, wMode);
}

void CInterfaceTerminal::sendModeOption2()
{
	SHORT wMode=0;

	eCurveForm form = getModel()->getCONFIG()->GetCurPressureRiseCtrl();

	switch(form)
	{
	case CURVE_IFLOW:
		{
		}
		break;
	case CURVE_LINEAR:
		{
			wMode=getModel()->setBitOfWord(wMode, MODOPT2_CURVEFORM1_BIT);
		}
		break;
	/*case CURVE_AUTOFLOW:
		{
			wMode=getModel()->setBitOfWord(wMode, MODOPT2_CURVEFORM2_BIT);
		}
		break;*/
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

	/*if(getModel()->getDATAHANDLER()->isNebulizerOn()==true)
	{
		wMode=getModel()->setBitOfWord(wMode, MODOPT2_NEBULIZER_BIT);
	}*/
	/*if(getModel()->getCONFIG()->isLeakCompOff())
	{
		wMode=getModel()->setBitOfWord(wMode, MODOPT2_LEAKCOMPENSATION_BIT);
	}*/

	eLeakCompensation eLeakCompOff=getModel()->getCONFIG()->getLeakCompOff();
	switch(eLeakCompOff)
	{
	case LC_OFF:
		{
			//
		}
		break;
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

	sendData_SHORT(TERM_MODE_OPTION2, wMode);
}

//#####################################CheckThread########################################
/***************************************************************************/
//     Purpose: 
//**************************************************************************/
//static UINT CTerminalThread( LPVOID pc )
//{
//	((CInterfaceTerminal*)pc)->TerminalData();
//	/*CloseHandle(((CInterfaceTerminal*)pc)->m_pcwtTerminalThread->m_hThread);
//	((CInterfaceTerminal*)pc)->m_pcwtTerminalThread=NULL;*/
//	return true;
//}

/***************************************************************************/
//     Purpose: 
//**************************************************************************/
//DWORD CInterfaceTerminal::TerminalData(void) 
//{
//	//CeSetThreadPriority(m_pcwtTerminalThread->m_hThread,150);
//
//	WORD iCnt=0;
//	while(iCnt<10 && m_bDoTerminalThread)
//	{
//		Sleep(500);
//		iCnt++;
//	}
//
//	DWORD timeout = 0;
//
//	DWORD r;
//	const int JUNK = 32;
//	char temp[JUNK] = {0};
//
//
//
//	CStringW szTest = _T("0");
//
//
//	do
//	{
//		WaitCommEvent(m_comPort.m_hCommPort,&r,NULL);
//
//		int size = m_comPort.ReadCommand(temp,sizeof(temp)-1);
//
//
//		if (size > 0)
//		{
//
//			CStringW str(temp);
//
//			if(ParseTerminalCommand(CTlsBlob((BYTE*)temp,size))==true)
//			{
//				//SendSerialAcknowledge();
//			}
//		}
//	}while(m_bDoTerminalThread);
//
//	
//
//	return 0;
//}



//#####################################ReceiveThread########################################
/***************************************************************************/
//     Purpose: 
//**************************************************************************/
//static UINT CTerminalReceiveThread( LPVOID pc )
//{
//	((CInterfaceTerminal*)pc)->ReceiveTerminalData();
//	return true;
//}

/***************************************************************************/
//     Purpose: 
//**************************************************************************/
//DWORD CInterfaceTerminal::ReceiveTerminalData(void) 
//{
//	CeSetThreadPriority(m_pcwtTerminalReceiveThread->m_hThread,130);
//
//	//DWORD r;
//	const int JUNK = 32;
//	char temp[JUNK] = {0};
//
//	//OVERLAPPED o;
//	//BOOL fSuccess;
//	DWORD dwCommModemStatus;
//
//	// Set the event mask. 
//
//	if (m_comPort.m_hCommPort == INVALID_HANDLE_VALUE) 
//	{
//		// Handle the error. 
//		TRACE(_T("ReceiveTerminalData: m_comPort.m_hCommPort == INVALID_HANDLE_VALUE.\n"));
//		theApp.getLog()->WriteLine(_T("ReceiveTerminalData: m_comPort.m_hCommPort == INVALID_HANDLE_VALUE."));
//		return 0;
//	}
//
//	SetCommMask (m_comPort.m_hCommPort, EV_RXCHAR | EV_CTS | EV_DSR | EV_BREAK);
//
//
//
//	while (m_comPort.m_hCommPort != INVALID_HANDLE_VALUE  && m_bDoTerminalReceiveThread) 
//	{
//		// Wait for an event to occur for the port.
//		WaitCommEvent (m_comPort.m_hCommPort, &dwCommModemStatus, 0);
//
//		// Re-specify the set of events to be monitored for the port.
//		SetCommMask (m_comPort.m_hCommPort, EV_RXCHAR | EV_CTS | EV_DSR | EV_BREAK);
//
//		if ((dwCommModemStatus & EV_RXCHAR) && m_bDoTerminalReceiveThread) 
//		{
//			int size = m_comPort.ReadCommand(temp,sizeof(temp)-1);
//
//			if (size > 0 && m_bDoTerminalReceiveThread)
//			{
//				if(ParseTerminalCommand(CTlsBlob((BYTE*)temp,size))==true && m_bDoTerminalReceiveThread)
//				{
//
//					/*g_eventCOM.SetEvent();
//					if(FALSE==getModel()->IsSERIALavailabel())
//					{
//						getModel()->SetSERIALavailability(TRUE);
//						getModel()->getALARMHANDLER()->setAutoSilent();
//					}*/
//				}
//			}
//		}
//	}
//
//
//
//	TRACE(_T("end thread TerminalReceiveThread()\r"));
//
//	
//	return 0;
//}

//#####################################SendThread########################################
/***************************************************************************/
//     Purpose: 
//**************************************************************************/
static UINT CTerminalSendThread( LPVOID pc )
{
	((CInterfaceTerminal*)pc)->SendTerminalData();
	return TRUE;
}

/***************************************************************************/
//     Purpose: 
//**************************************************************************/
DWORD CInterfaceTerminal::SendTerminalData(void) 
{
	CeSetThreadPriority(m_pcwtTerminalSendThread->m_hThread,130);

	WORD iCnt=0;
	while(iCnt<20 && m_bDoTerminalSendThread)//newCOMCHECK
	{
		Sleep(500);
		iCnt++;
	}

	if(!m_bDoTerminalSendThread)
	{
		return 0;
	}

	CStringW szTest=_T("");

	//m_bSendWaveData=true;

	setWaveDataActive();

	do
	{
		//DWORD dw = ::WaitForSingleObject(g_eventTerminalSendData, INFINITE);

		if(m_bDoTerminalSendThread==false)
			continue;

		EnterCriticalSection(&csMSGSend);
		POSITION pos = MSGSend.GetHeadPosition();
		if(!pos)
		{
			LeaveCriticalSection(&csMSGSend);
		}
		else
		{
			LPTERMINALMSG szTemp = MSGSend.GetAt(pos);
			Write(szTemp->byBuffer, szTemp->bySize);

			MSGSend.RemoveAt(pos);
			delete (void *)szTemp;

			pos = MSGSend.GetHeadPosition();

			LeaveCriticalSection(&csMSGSend);
		}
		Sleep(1);

		/*EnterCriticalSection(&csMSGSend);
		POSITION pos = MSGSend.GetHeadPosition();
		LeaveCriticalSection(&csMSGSend);
		while(pos!=NULL)
		{
			EnterCriticalSection(&csMSGSend);

			LPTERMINALMSG szTemp = MSGSend.GetAt(pos);
			Write(szTemp->byBuffer, szTemp->bySize);

			MSGSend.RemoveAt(pos);
			delete (void *)szTemp;

			pos = MSGSend.GetHeadPosition();

			LeaveCriticalSection(&csMSGSend);
		}*/
	}while(m_bDoTerminalSendThread);

	
	return 0;
}

void CInterfaceTerminal::send(LPTERMINALMSG msg)
{
	EnterCriticalSection(&csMSGSend);

	MSGSend.AddTail(msg);

	LeaveCriticalSection(&csMSGSend);

	//g_eventTerminalSendData.SetEvent();
}

/***************************************************************************/
//     Purpose: 
//**************************************************************************/
//DWORD CInterfaceTerminal::SendTerminalCommand(CStringW data) 
//{
//	//if(m_comPort.m_hCommPort!=INVALID_HANDLE_VALUE )
//	//{
//	//	//if(TRUE==getModel()->IsTERMINALavailability())
//	//	{
//	//		EnterCriticalSection(&csMSGSend);
//
//	//		MSGSend.AddTail(data);
//	//		//TRACE(_T("SendSerialCommand: %s\r"),data);
//
//	//		LeaveCriticalSection(&csMSGSend);
//
//	//		g_eventTerminalSendData.SetEvent();
//	//	}
//	//}
//
//	return 0;
//}

void CInterfaceTerminal::sendWaveData(SHORT  iValPressure,SHORT  iValFlow,SHORT  iValVolume,SHORT  iValCO2) 
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
	msg->byBuffer[1]=10;
	msg->byBuffer[2]=TERM_WAVE_DATA; 
	
	msg->byBuffer[3]=HIBYTE(iValPressure);
	msg->byBuffer[4]=LOBYTE(iValPressure);
	msg->byBuffer[5]=HIBYTE(iValFlow);
	msg->byBuffer[6]=LOBYTE(iValFlow);
	msg->byBuffer[7]=HIBYTE(iValVolume);
	msg->byBuffer[8]=LOBYTE(iValVolume);
	msg->byBuffer[9]=HIBYTE(iValCO2);
	msg->byBuffer[10]=LOBYTE(iValCO2);

	for(int i=1;i<=10;i++)
	{
		checksum += msg->byBuffer[i];
	}
	msg->byBuffer[11]=checksum;

	msg->bySize=12;
	
	send(msg);

}

void CInterfaceTerminal::sendWaveData(SHORT  iValPressure,SHORT  iValFlow, SHORT  iCO2) //FOT-TERM1
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
	msg->byBuffer[1]=7;
	msg->byBuffer[2]=HIBYTE(iValPressure);
	msg->byBuffer[3]=LOBYTE(iValPressure);
	msg->byBuffer[4]=HIBYTE(iValFlow);
	msg->byBuffer[5]=LOBYTE(iValFlow);
	msg->byBuffer[6]=HIBYTE(iCO2);
	msg->byBuffer[7]=LOBYTE(iCO2);

	for(int i=1;i<=7;i++)
	{
		checksum += msg->byBuffer[i];
	}
	msg->byBuffer[8]=checksum;

	msg->bySize=9;

	send(msg);

}
//void CInterfaceTerminal::SendToTerminal(SHORT  iValPressure,SHORT  iValFlow,SHORT  iValVolume,SHORT  iValCO2) 
//{
//	/*BYTE byDB1pressure=LOBYTE(iValPressure);
//	BYTE byDB2pressure=HIBYTE(iValPressure);
//
//	BYTE byDB1FLow=LOBYTE(iValFlow);
//	BYTE byDB2FLow=HIBYTE(iValFlow);*/
//
//	/*iValPressure=255;
//	iValFlow=-2000;*/
//
//	/*m_byBuffer[m_iCntBuf]=iValPressure;
//	m_iCntBuf++;
//	m_byBuffer[m_iCntBuf]=iValFlow;
//	m_iCntBuf++;*/
//
//	/*m_byBuffer[m_iCntBuf]=LOBYTE(iValPressure);
//	m_iCntBuf++;
//	m_byBuffer[m_iCntBuf]=HIBYTE(iValPressure);
//	m_iCntBuf++;
//	m_byBuffer[m_iCntBuf]=LOBYTE(iValFlow);
//	m_iCntBuf++;
//	m_byBuffer[m_iCntBuf]=HIBYTE(iValFlow);
//	m_iCntBuf++;*/
//
//	if(m_iCntBuf>=20)
//	{
//		m_iCntBuf=0;
//		//SendBuffer();
//
//		/*SHORT iRes=0;
//		BYTE low=LOBYTE(iValFlow);
//		BYTE high=HIBYTE(iValFlow);
//		iRes=MAKEWORD(low,high);
//		int iStop=0;*/
//	}
//	
//
//	//SendBuffer(0x80, 0x05, byDB1pressure, byDB2pressure, byDB1FLow, byDB2FLow);
//
//	/*SHORT iRes=0;
//	iRes=MAKEWORD(byDB1pressure,byDB2pressure);
//	iRes=MAKEWORD(byDB1FLow,byDB2FLow);
//	
//	int iStop=0;*/
//}

//void CInterfaceTerminal::SendBuffer(BYTE CO2_command, BYTE Anzahlbytes, BYTE byte0, BYTE byte1, BYTE byte2, BYTE byte3, BYTE byte4) 
void CInterfaceTerminal::sendBuffer(LPTERMINALMSG msg) 
{
	/*LPTERMINALMSG msg = new TERMINALMSG;
	UINT iCntBuf=3;

	msg->byBuffer[0]=TERM_MSG_SOM;
	msg->byBuffer[1]=command;
	msg->byBuffer[2]=sizeBytes;
	
	iCntBuf++;
	int iCnt=0;
	for(iCnt;iCnt<sizeBytes;iCnt++)
	{
		msg->byBuffer[iCntBuf]=faSendDataBuf[iCnt];
		iCntBuf++;
	}*/

	//if(m_bSendMonitorData)
	//{
		//LPTERMMSG msg = new TERMMSG;

		

		

		/*Data is transmitted and received as data packets in the following format:
			CMD - [data_byte0 ... data_byte19]
		where:
			CMD - command byte, 80h
			data_byte0 = LOBYTE Pressure
			data_byte1 = HIBYTE Pressure
			data_byte2 = LOBYTE Flow
			data_byte3 = HIBYTE Flow
			data_byte4 = LOBYTE Pressure
			data_byte5 = HIBYTE Pressure
			data_byte6 = LOBYTE Flow
			data_byte7 = HIBYTE Flow
			... data_byte19 - data bytes 0 thru 19*/
			

		/*msg->byBuffer[0]=0x80;
		msg->byBuffer[1]=m_byBuffer[0];
		msg->byBuffer[2]=m_byBuffer[1];
		msg->byBuffer[3]=m_byBuffer[2];
		msg->byBuffer[4]=m_byBuffer[3];
		msg->byBuffer[5]=m_byBuffer[4];
		msg->byBuffer[6]=m_byBuffer[5];
		msg->byBuffer[7]=m_byBuffer[6];
		msg->byBuffer[8]=m_byBuffer[7];
		msg->byBuffer[9]=m_byBuffer[8];
		msg->byBuffer[10]=m_byBuffer[9];
		msg->byBuffer[11]=m_byBuffer[10];
		msg->byBuffer[12]=m_byBuffer[11];
		msg->byBuffer[13]=m_byBuffer[12];
		msg->byBuffer[14]=m_byBuffer[13];
		msg->byBuffer[15]=m_byBuffer[14];
		msg->byBuffer[16]=m_byBuffer[15];
		msg->byBuffer[17]=m_byBuffer[16];
		msg->byBuffer[18]=m_byBuffer[17];
		msg->byBuffer[19]=m_byBuffer[18];
		msg->byBuffer[20]=m_byBuffer[19];
		
		msg->bySize=21;*/
		
		/*Data is transmitted and received as data packets in the following format:
			CMD - NBF - [data_byte0 ... data_byten] - CKS
		where:
		CMD - command byte, range 80h - FFh
			NBF- number of bytes to follow this byte, including the checksum
			data_byte0 ... data_byten - data bytes 0 thru n
			CKS - checksum*/


	

		//CStringW szTemp=_T("");
		/*szTemp.Format(_T("%X,%X,%X,%X,%X,%X,%X,%X,%X,%X,%X,%X,%X,%X,%X,%X,%X,%X,%X,%X,%X \n"), 
								msg->byBuffer[0],msg->byBuffer[1],msg->byBuffer[2],msg->byBuffer[3],
								msg->byBuffer[4],msg->byBuffer[5],msg->byBuffer[6],msg->byBuffer[7],
								msg->byBuffer[8],msg->byBuffer[9],msg->byBuffer[10],msg->byBuffer[11],
								msg->byBuffer[12],msg->byBuffer[13],msg->byBuffer[14],msg->byBuffer[15],
								msg->byBuffer[16],msg->byBuffer[17],msg->byBuffer[18],msg->byBuffer[19],msg->byBuffer[20]);
		*/
		/*szTemp.Format(_T("#%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d"), 
			m_byBuffer[0],m_byBuffer[1],m_byBuffer[2],m_byBuffer[3],
			m_byBuffer[4],m_byBuffer[5],m_byBuffer[6],m_byBuffer[7],
			m_byBuffer[8],m_byBuffer[9],m_byBuffer[10],m_byBuffer[11],
			m_byBuffer[12],m_byBuffer[13],m_byBuffer[14],m_byBuffer[15],
			m_byBuffer[16],m_byBuffer[17],m_byBuffer[18],m_byBuffer[19]);*/

		//msg->szMessage=szTemp;
		//theApp.getLog()->WriteLine(szTemp);
		//TRACE(szTemp);



		

		//if(m_comPort.m_hCommPort!=INVALID_HANDLE_VALUE )
		//{
		//	EnterCriticalSection(&csMSGSend);

		//	//MSGSendBuf.AddTail(msg);
		//	MSGSend.AddTail(szTemp);

		//	LeaveCriticalSection(&csMSGSend);

		//	g_eventTerminalSendData.SetEvent();
		//}
	//}
	

}

//void CInterfaceTerminal::AddChecksum(short num, BYTE buf[])
//{
//	char checksum;
//	short i;
//	checksum = 0;
//	for (i = 0; i < num; i++)
//		checksum = (char) (checksum + buf[i]);
//	checksum = (char) ((-checksum) & 0x7F);
//	buf[num]=checksum;
//}


//
//		
//		if(psz[0] == 'S')
//		{
//			feRecvState = TERM_NONE;
//			if(m_bSendMonitorData)
//				return false;
//
//			if(AfxGetMainWnd())
//				AfxGetMainWnd()->SendMessage(WM_SCREENSHOT);
//
//			return false;
//		}
//		else if(psz[0] == 'B')
//		{
//			feRecvState = TERM_NONE;
//			if(m_bSendMonitorData)
//				return false;
//			SendTerminalCommand(_T("\r\n"));
//			//CStringW sz= bl.GetString();
//			//int iVal=getModel()->getDATAHANDLER()->GetMessureDataMV();
//			szSend.Format(_T("%d\r\n"),getModel()->getDATAHANDLER()->getBTBMessureDataMV());
//			SendTerminalCommand(szSend);
//			/*szSend.Format(_T("%d\r\n"),0);
//			SendTerminalCommand(szSend);
//			szSend.Format(_T("%d\r\n"),0);
//			SendTerminalCommand(szSend);*/
//			szSend.Format(_T("%d\r\n"),getModel()->getDATAHANDLER()->getAVGMessureDataTVE());
//			SendTerminalCommand(szSend);
//			szSend.Format(_T("%d\r\n"),getModel()->getDATAHANDLER()->getAVGMessureDataTVEpat());
//			SendTerminalCommand(szSend);
//			szSend.Format(_T("%d\r\n"),getModel()->getDATAHANDLER()->getAVGMessureDataTVEresp());
//			SendTerminalCommand(szSend);
//			szSend.Format(_T("%d\r\n"),getModel()->getDATAHANDLER()->getBTBMessureDataLeak());
//			SendTerminalCommand(szSend);
//			szSend.Format(_T("%d\r\n"),getModel()->getDATAHANDLER()->getCurBTBMessureDataBPM());
//			SendTerminalCommand(szSend);
//			szSend.Format(_T("%d\r\n"),getModel()->getDATAHANDLER()->getBTBMessureDataPINSP()/10);
//			SendTerminalCommand(szSend);
//			szSend.Format(_T("%d\r\n"),getModel()->getDATAHANDLER()->getBTBMessureDataPmitt()/10);
//			SendTerminalCommand(szSend);
//			szSend.Format(_T("%d\r\n"),getModel()->getDATAHANDLER()->getBTBMessureDataPEEP()/10);
//			SendTerminalCommand(szSend);
//			int iVal=getModel()->getDATAHANDLER()->getMessureDataO2();
//			if(iVal<0)
//			iVal=0;
//			szSend.Format(_T("%d\r\n"),iVal);
//			SendTerminalCommand(szSend);
//			szSend.Format(_T("%d\r\n"),getModel()->getDATAHANDLER()->getBTBMessureDataCompliance());
//			SendTerminalCommand(szSend);
//			szSend.Format(_T("%d\r\n"),getModel()->getDATAHANDLER()->getBTBMessureDataC20C());
//			SendTerminalCommand(szSend);
//			szSend.Format(_T("%d\r\n"),getModel()->getDATAHANDLER()->getBTBMessureDataResistance());
//			SendTerminalCommand(szSend);
//			SendTerminalCommand(_T("\r\n"));
//			SendTerminalCommand(_T(">"));
//
//			return false;
//		}
//		else if(psz[0] == 'R')
//		{
//			feRecvState = TERM_NONE;
//			if(m_bSendMonitorData)
//				return false;
//			SendTerminalCommand(_T("\r\n"));
//			//CStringW sz= bl.GetString();
//			//int iVal=getModel()->getDATAHANDLER()->GetMessureDataMV();
//			szSend.Format(_T("%d\r\n"),getModel()->getCONFIG()->GetCurMode());
//			SendTerminalCommand(szSend);
//			/*szSend.Format(_T("%d\r\n"),xxx);
//			m_comPort.Write(szSend);*/
//			szSend.Format(_T("%d\r\n"),getModel()->getDATAHANDLER()->GetCurrentO2Para());
//			SendTerminalCommand(szSend);
//			szSend.Format(_T("%d\r\n"),getModel()->getDATAHANDLER()->GetCurrentITimePara());
//			SendTerminalCommand(szSend);
//			szSend.Format(_T("%d\r\n"),getModel()->getDATAHANDLER()->GetCurrentETimePara());
//			SendTerminalCommand(szSend);
//			/*if(getModel()->getDATAHANDLER()->IsCurrentVGarantStateOn())
//				szSend.Format(_T("%d\r\n"),getModel()->getDATAHANDLER()->GetPINSPVolGPara());
//			else
//				szSend.Format(_T("%d\r\n"),getModel()->getDATAHANDLER()->GetPINSPPara());*/
//			szSend.Format(_T("%d\r\n"),getModel()->getDATAHANDLER()->GetCurrentPinspPara());
//			SendTerminalCommand(szSend);
//			szSend.Format(_T("%d\r\n"),getModel()->getDATAHANDLER()->GetCurrentPEEPPara());
//			SendTerminalCommand(szSend);
//			szSend.Format(_T("%d\r\n"),getModel()->getDATAHANDLER()->GetCurrentIFlowPara());
//			SendTerminalCommand(szSend);
//			szSend.Format(_T("%d\r\n"),getModel()->getDATAHANDLER()->GetCurrentEFlowParaData());
//			SendTerminalCommand(szSend);
//			SendTerminalCommand(_T("\r\n"));
//			SendTerminalCommand(_T(">"));
//
//			return false;
//		}


bool CInterfaceTerminal::isWaveDataActive()
{
	EnterCriticalSection(&csEnableSend);
	bool bSend=m_bSendWaveData;
	LeaveCriticalSection(&csEnableSend);
	return bSend;
}

void CInterfaceTerminal::setWaveDataActive()
{
	EnterCriticalSection(&csEnableSend);
	m_bSendWaveData=true;
	LeaveCriticalSection(&csEnableSend);
	
}
 
bool CInterfaceTerminal::isMeasurementDataActive()
{
	return m_bSendContiniousMeasurements;
}
void CInterfaceTerminal::sendData_Cmd(BYTE command)
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

	send(msg);
}


void CInterfaceTerminal::sendData_SHORT(BYTE command, SHORT val) 
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

	send(msg);
}

void CInterfaceTerminal::sendData_BYTE(BYTE command, BYTE val) 
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

	send(msg);
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceTerminal::sendMessurementData()
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
	
	
//getMessureDataBTB(NUMT_PPEAK);
	

	bool bCO2value=false;
	bool bCO2module=false;
	bool bSPO2module=false;
	
	if(getModel()->getCONFIG()->GetCO2module()!=CO2MODULE_NONE)
	{
		bCO2module=true;
	}

	if(	bCO2module && getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==true)
	{
		bCO2value=true;
	}

	if(getModel()->getCONFIG()->GetSPO2module()!=SPO2MODULE_NONE)
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
		msg->byBuffer[2]=TERM_MEASUREMENTS_BTB; 
	}
	else //if (m_bMEASUREMENTS_AVG)
	{
		msg->byBuffer[2]=TERM_MEASUREMENTS_AVG; 
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