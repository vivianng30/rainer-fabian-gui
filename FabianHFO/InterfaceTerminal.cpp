/**********************************************************************************************//**
 * \file	InterfaceTerminal.cpp.
 *
 * Implements the interface terminal class
 **************************************************************************************************/

#include "StdAfx.h"
#include "InterfaceTerminal.h"
#include "MVModel.h"
#include "MVViewHandler.h"

#include "InterfaceTerminalWave.h"
#include "InterfaceTerminalRemote.h"

#include "TlsStream.h"


CInterfaceTerminal* CInterfaceTerminal::theTerminalInterface=0;
CList<LPTERMINALMSG, LPTERMINALMSG>  CInterfaceTerminal::MSGSend;

/**********************************************************************************************//**
 * A macro that defines terminal notvalid
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

#define TERMINAL_NOTVALID -32765	//value NOTVALID

/**********************************************************************************************//**
 * Initializes a new instance of the CInterfaceTerminal class
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

CInterfaceTerminal::CInterfaceTerminal(void)
{
	InitializeCriticalSection(&csEnableSend);
	InitializeCriticalSection(&csMSGSend);

	//m_bStarted=true;
	m_bSendWaveData=false;

	/*fwReadChkSum=0;
	fwCalcChkSum = 0;
	feRecvState = TERM_STATE_SOM;
	fdwRecvBufLen = 0;
	faRecvBuf[0]=0x0000;
	faDataBuf[0]=0x0000;
	fwRecvDataBufCnt=0;
	fwRecvDataCnt=0;
	fwRecvDataSize=0;
	fwRecvDataCode=0;*/

	//faSendDataBuf[0]=0x0000;
	//fwSendDataSize=0;

	m_pcwtTerminalSendThread=NULL;
	m_hThreadTerminalSend=INVALID_HANDLE_VALUE;
	m_bDoTerminalSendThread=false;

	//m_iCntBuf=0;

	/*m_byDellaceAmp=0;
	m_byDellaceFreq=0;*/

	/*m_bMEASUREMENTS_BTB=false;
	m_bMEASUREMENTS_AVG=false;*/
	m_bSendContiniousMeasurements=false;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CInterfaceTerminal class
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

CInterfaceTerminal::~CInterfaceTerminal(void)
{
}

/**********************************************************************************************//**
 * Gets an instance
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iTYPE	Zero-based index of the type.
 *
 * \return	Null if it fails, else the instance.
 **************************************************************************************************/

CInterfaceTerminal* CInterfaceTerminal::GetInstance(BYTE iTYPE)
{
	if(theTerminalInterface == 0)
	{
		switch(iTYPE)
		{
			case ACL_TERMINAL_REMOTE:
			{
				theTerminalInterface = new CInterfaceTerminalRemote;
			}
			break;
			default://ACL_TERMINAL_WAVE
			{
				theTerminalInterface = new CInterfaceTerminalWave;
			}
			break;
		}
		
		//theTerminalInterface->Init();
	}
	return theTerminalInterface;
}

/**********************************************************************************************//**
 * Destroys the instance
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CInterfaceTerminal::DestroyInstance()
{
	if(theTerminalInterface != NULL)
	{
		delete theTerminalInterface;
		theTerminalInterface = NULL;
	}
}


//bool CInterfaceTerminal::Init()
//{
//	StartTerminalThread();
//
//	OpenCOM();
//
//	Write("fabianHFO");
////#define DUMP_TERMINALCMD
//
//#ifdef DUMP_TERMINALCMD
//	//sendData_Cmd(TERMINAL_GET_MEASUREMENTS_ONCE_BTB);			//0x00
//	//sendData_Cmd(TERMINAL_GET_MEASUREMENTS_CONTINIOUS_BTB);	//0x01
//	//sendData_Cmd(TERMINAL_GET_MEASUREMENTS_ONCE_AVG	);		//0x02
//	//sendData_Cmd(TERMINAL_GET_MEASUREMENTS_CONTINIOUS_AVG);	//0x03
//	//sendData_Cmd(TERMINAL_GET_WAVE_DATA);					//0x04
//	//sendData_Cmd(TERMINAL_GET_VENT_MODE);					//0x05//'O'
//	//sendData_Cmd(TERMINAL_GET_MODE_OPTION1);					//0x06//'R'
//	//sendData_Cmd(TERMINAL_GET_MODE_OPTION2);					//0x07//')'
//	//sendData_Cmd(TERMINAL_GET_VENT_RUNSTATE);					//0x09
//	//sendData_Cmd(TERMINAL_GET_STATE_VLimit);					//0x0A
//	//sendData_Cmd(TERMINAL_GET_STATE_VGarant);					//0x0B
//	//sendData_Cmd(TERMINAL_GET_PARAM_VentRange);				//0x0C
//	//sendData_Cmd(TERMINAL_GET_PARAM_IERatioHFO);				//0x0D
//	//sendData_Cmd(TERMINAL_GET_MANBREATHrunning);				//0x0E
//	//sendData_Cmd(TERMINAL_GET_PressureRiseCtrl);				//0x0F
//	//sendData_Cmd(TERMINAL_GET_PARAM_HFOFreqRec);				//0x10
//	//sendData_Cmd(TERMINAL_GET_PARAM_HFOFlow);					//0x11
//	//sendData_Cmd(TERMINAL_GET_LeakCompensation);				//0x12
//	//sendData_Cmd(TERMINAL_GET_TriggerOption);					//0x13
//	//sendData_Cmd(TERMINAL_GET_FOToscillationState);			//0x14
//	//sendData_Cmd(TERMINAL_GET_PARAM_PINSP);					//0x15
//	//sendData_Cmd(TERMINAL_GET_PARAM_PEEP);						//0x16
//	//sendData_Cmd(TERMINAL_GET_PARAM_PPSV);						//0x17
//	//sendData_Cmd(TERMINAL_GET_PARAM_BPM);						//0x18
//	//sendData_Cmd(TERMINAL_GET_PARAM_HFAmpl);					//0x19
//	//sendData_Cmd(TERMINAL_GET_PARAM_HFAmplMax);				//0x1A
//	//sendData_Cmd(TERMINAL_GET_PARAM_HFFreq);					//0x1B
//	//sendData_Cmd(TERMINAL_GET_PARAM_O2);						//0x1C
//	//sendData_Cmd(TERMINAL_GET_PARAM_IFlow);					//0x1D
//	//sendData_Cmd(TERMINAL_GET_PARAM_EFlow);					//0x1E
//	//sendData_Cmd(TERMINAL_GET_PARAM_Risetime);					//0x1F
//	//sendData_Cmd(TERMINAL_GET_PARAM_ITime);					//0x20
//	//sendData_Cmd(TERMINAL_GET_PARAM_ETime);					//0x21
//	//sendData_Cmd(TERMINAL_GET_PARAM_HFPMean);					//0x22
//	//sendData_Cmd(TERMINAL_GET_PARAM_HFPMeanRec);				//0x23
//	//sendData_Cmd(TERMINAL_GET_PARAM_VLimit);					//0x24
//	//sendData_Cmd(TERMINAL_GET_PARAM_VGarant);					//0x25
//	//sendData_Cmd(TERMINAL_GET_PARAM_AbortCriterionPSV);		//0x26
//	//sendData_Cmd(TERMINAL_GET_PARAM_TherapieFlow);				//0x27
//	//sendData_Cmd(TERMINAL_GET_PARAM_Trigger);					//0x28
//	//sendData_Cmd(TERMINAL_GET_PARAM_Flowmin);					//0x29
//	//sendData_Cmd(TERMINAL_GET_PARAM_CPAP);						//0x2A
//	//sendData_Cmd(TERMINAL_GET_PARAM_PManual);					//0x2B
//	//sendData_Cmd(TERMINAL_GET_PARAM_Backup);					//0x2C
//	//sendData_Cmd(TERMINAL_GET_PARAM_ITimeRec);					//0x2D
//	//sendData_Cmd(TERMINAL_GET_PARAM_ETIMERec);					//0x2E
//	//sendData_Cmd(TERMINAL_GET_PARAM_SPO2LOW);					//0x2F
//	//sendData_Cmd(TERMINAL_GET_PARAM_SPO2HIGH);					//0x30
//	//sendData_Cmd(TERMINAL_GET_PARAM_FIO2LOW);					//0x31
//	//sendData_Cmd(TERMINAL_GET_PARAM_FIO2HIGH);					//0x32
//	//sendData_Cmd(TERMINAL_GET_STATE_PRICO);						//0x33
//
//	//sendData_Cmd(TERMINAL_STOP_CONTINIOUS_MEASUREMENTS);		//0x50
//	//sendData_Cmd(TERMINAL_STOP_WAVE_DATA);						//0x51
//	
//	//sendData_BYTE(TERMINAL_SET_VENT_MODE, 1);						//0x52
//	//sendData_BYTE(TERMINAL_SET_VENT_MODE, 2);
//	//sendData_BYTE(TERMINAL_SET_VENT_MODE, 8);
//	//sendData_BYTE(TERMINAL_SET_VENT_MODE, 9);
//
//	//sendData_BYTE(TERMINAL_SET_VENT_RUNSTATE, xxx);				//0x55
//	//sendData_BYTE(TERMINAL_SET_STATE_VLimit, xxx);				//0x56
//	//sendData_BYTE(TERMINAL_SET_STATE_VGarant, 1);				//0x57
//	//sendData_BYTE(TERMINAL_SET_PARAM_VentRange, xxx);			//0x58
//	//sendData_BYTE(TERMINAL_SET_PARAM_IERatioHFO, xxx);			//0x59
//	//sendData_BYTE(TERMINAL_SET_MANBREATHrunning, xxx);			//0x5A
//	//sendData_BYTE(TERMINAL_SET_PressureRiseCtrl, xxx);			//0x5B
//	//sendData_SHORT(TERMINAL_SET_PARAM_HFOFreqRec, xxx);			//0x5C
//	//sendData_SHORT(TERMINAL_SET_PARAM_HFOFlow, xxx);				//0x5D
//	//sendData_BYTE(TERMINAL_SET_LeakCompensation, xxx);			//0x5E
//	//sendData_SHORT(TERMINAL_SET_PARAM_PINSP, 200);				//0x60
//	//sendData_SHORT(TERMINAL_SET_PARAM_PEEP, xxx);					//0x61
//	//sendData_SHORT(TERMINAL_SET_PARAM_PPSV, xxx);					//0x62
//	//sendData_SHORT(TERMINAL_SET_PARAM_BPM, xxx);					//0x63
//	//sendData_SHORT(TERMINAL_SET_PARAM_HFAmpl, xxx);				//0x64
//	//sendData_SHORT(TERMINAL_SET_PARAM_HFAmplMax, xxx);			//0x65
//	//sendData_SHORT(TERMINAL_SET_PARAM_HFFreq, xxx);				//0x66
//	//sendData_BYTE(TERMINAL_SET_PARAM_O2	, xxx);				//0x67
//	//sendData_SHORT(TERMINAL_SET_PARAM_IFlow, xxx);				//0x68
//	//sendData_SHORT(TERMINAL_SET_PARAM_EFlow, xxx);				//0x6A
//	//sendData_SHORT(TERMINAL_SET_PARAM_RiseTime, xxx);				//0x6B
//	//sendData_SHORT(TERMINAL_SET_PARAM_ITime	, xxx);			//0x6C
//	//sendData_SHORT(TERMINAL_SET_PARAM_ETime, xxx);				//0x6D
//	//sendData_SHORT(TERMINAL_SET_PARAM_HFPMean, xxx);				//0x6E
//	//sendData_SHORT(TERMINAL_SET_PARAM_HFPMeanRec, xxx);			//0x6F
//	//sendData_SHORT(TERMINAL_SET_PARAM_VLimit, xxx);				//0x70
//	//sendData_SHORT(TERMINAL_SET_PARAM_VGarant, xxx);				//0x71
//	//sendData_BYTE(TERMINAL_SET_PARAM_AbortCriterionPSV, xxx);	//0x72
//	//sendData_SHORT(TERMINAL_SET_PARAM_TherapieFlow, xxx);			//0x73
//	//sendData_BYTE(TERMINAL_SET_PARAM_Trigger, xxx);				//0x74
//	//sendData_SHORT(TERMINAL_SET_PARAM_Flowmin, xxx);				//0x55
//	//sendData_SHORT(TERMINAL_SET_PARAM_CPAP, xxx);					//0x76
//	//sendData_SHORT(TERMINAL_SET_PARAM_PManual, xxx);				//0x77
//	//sendData_BYTE(TERMINAL_SET_PARAM_Backup, xxx);				//0x78
//	//sendData_SHORT(TERMINAL_SET_PARAM_ITimeRec, xxx);				//0x79
//	//sendData_BYTE(TERMINAL_SET_PARAM_O2_FLUSH, xxx);				//0x7A
//	//sendData_BYTE(TERMINAL_SET_PARAM_SPO2LOW, xxx);				//0x7A
//	//sendData_BYTE(TERMINAL_SET_PARAM_SPO2HIGH, xxx);				//0x7B
//	//sendData_BYTE(TERMINAL_SET_PARAM_FIO2LOW, xxx);				//0x7C
//	//sendData_BYTE(TERMINAL_SET_PARAM_FIO2HIGH, xxx);				//0x7D
//	//sendData_BYTE(TERMINAL_SET_STATE_PRICO, xxx);				//0x7E
//	
//	//sendData_Cmd(TERMINAL_GET_MEASUREMENTS_CONTINIOUS_AVG);
//
//#endif
//
//	return 0;
//
//}

/**********************************************************************************************//**
 * Deinits this instance
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

//bool CInterfaceTerminal::OpenCOM()
//{
//	bool bRes=false;
//
//	
//	DWORD dwIndex=0;
//	LONG    lLastError =0;
//
//	if(getModel()->getCONFIG()->GetMainBoardVersion()>=MAINBOARD_30)
//		lLastError = Open(_T("COM2:"),0,0);
//	else	//MAINBOARD2_1
//		lLastError = Open(_T("COM1:"),0,0);
//
//
//
//	if (lLastError != ERROR_SUCCESS)
//	{
//		/*if(ERROR_ALREADY_INITIALIZED==lLastError)
//		{
//			
//		}*/
//		return bRes;
//	}
//
//	// Setup the serial port (9600,8N1, which is the default setting)
//	//lLastError = Setup(CSerial::EBaud115200,CSerial::EData8,CSerial::EParNone,CSerial::EStop1);
//	//lLastError = Setup(CSerial::EBaud256000,CSerial::EData8,CSerial::EParNone,CSerial::EStop1);
//	lLastError = Setup(CSerial::EBaud230400,CSerial::EData8,CSerial::EParNone,CSerial::EStop1);
//	if (lLastError != ERROR_SUCCESS)
//	{
//		DEBUGMSG(TRUE, (TEXT("OpenCOM Unable to set COM-port setting\r\n")));
//		//return ShowError(GetLastError(), _T("Unable to set COM-port setting"));
//		return bRes;
//	}
//	else
//	{
//		bRes=true;
//	}
//
//	DEBUGMSG(TRUE, (TEXT("OpenCOM success\r\n")));
//
//	if(bRes)
//	{
//		StartListener();	
//	}
//	return bRes;
//}

/**********************************************************************************************//**
 * Starts terminal thread
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CInterfaceTerminal::StartTerminalThread( void )
{
	m_bDoTerminalSendThread=true;

	if(m_pcwtTerminalSendThread!=NULL)
	{
		delete m_pcwtTerminalSendThread;
		m_pcwtTerminalSendThread=NULL;

		if(m_hThreadTerminalSend!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadTerminalSend);
			m_hThreadTerminalSend=INVALID_HANDLE_VALUE;
		}
	}

	m_pcwtTerminalSendThread=AfxBeginThread(CTerminalSendThread,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	m_hThreadTerminalSend=m_pcwtTerminalSendThread->m_hThread;
	m_pcwtTerminalSendThread->m_bAutoDelete = FALSE; 
	m_pcwtTerminalSendThread->ResumeThread();
}

/**********************************************************************************************//**
 * Stops terminal thread
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CInterfaceTerminal::StopTerminalThread( void )
{
	if(m_bDoTerminalSendThread)
	{
		m_bDoTerminalSendThread=false;

		if (WaitForSingleObject(m_pcwtTerminalSendThread->m_hThread,2000) == WAIT_TIMEOUT)
		{
			theApp.WriteLog(_T("#HFO:0213"));
			if(!TerminateThread(m_pcwtTerminalSendThread,0))
			{
			}
		}
	}
}




//#####################################SendThread########################################

/**********************************************************************************************//**
 * Terminal send thread
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	pc	The PC.
 *
 * \return	An UINT.
 **************************************************************************************************/

static UINT CTerminalSendThread( LPVOID pc )
{
	try
	{
		((CInterfaceTerminal*)pc)->SendTerminalData();
	}
	catch (CException* e)
	{
		TCHAR   szCause[255];
		e->GetErrorMessage(szCause, 255);

		CString errorStr=_T("");
		errorStr.Format(_T("CTerminalSendThread: %s"),szCause);

		theApp.ReportException(errorStr);

		e->Delete();
	}
	catch(...)
	{
		theApp.ReportException(_T("CTerminalSendThread"));

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_EXCEPTION);
	}
	//((CInterfaceTerminal*)pc)->SendTerminalData();
	return TRUE;
}

/**********************************************************************************************//**
 * Sends the terminal data
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	A DWORD.
 **************************************************************************************************/

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

	//setWaveDataActive();

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
			Sleep(10);
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

/**********************************************************************************************//**
 * Send this message
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	msg	The message.
 **************************************************************************************************/

void CInterfaceTerminal::send(LPTERMINALMSG msg)
{
	EnterCriticalSection(&csMSGSend);

	MSGSend.AddTail(msg);

	LeaveCriticalSection(&csMSGSend);

	//g_eventTerminalSendData.SetEvent();
}

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

/**********************************************************************************************//**
 * Sends a wave data
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iValPressure	Zero-based index of the value pressure.
 * \param	iValFlow		Zero-based index of the value flow.
 * \param	iValVolume  	Zero-based index of the value volume.
 * \param	iValCO2			Zero-based index of the value co 2.
 **************************************************************************************************/

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
	msg->byBuffer[2]=TERMINAL_WAVE_DATA; 
	
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

/**********************************************************************************************//**
 * Sends a wave data
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iValPressure	Zero-based index of the value pressure.
 * \param	iValFlow		Zero-based index of the value flow.
 * \param	iCO2			Zero-based index of the co 2.
 **************************************************************************************************/

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
//void CInterfaceTerminal::sendBuffer(LPTERMINALMSG msg) 
//{
//	/*LPTERMINALMSG msg = new TERMINALMSG;
//	UINT iCntBuf=3;
//
//	msg->byBuffer[0]=TERM_MSG_SOM;
//	msg->byBuffer[1]=command;
//	msg->byBuffer[2]=sizeBytes;
//	
//	iCntBuf++;
//	int iCnt=0;
//	for(iCnt;iCnt<sizeBytes;iCnt++)
//	{
//		msg->byBuffer[iCntBuf]=faSendDataBuf[iCnt];
//		iCntBuf++;
//	}*/
//
//	//if(m_bSendMonitorData)
//	//{
//		//LPTERMMSG msg = new TERMMSG;
//
//		
//
//		
//
//		/*Data is transmitted and received as data packets in the following format:
//			CMD - [data_byte0 ... data_byte19]
//		where:
//			CMD - command byte, 80h
//			data_byte0 = LOBYTE Pressure
//			data_byte1 = HIBYTE Pressure
//			data_byte2 = LOBYTE Flow
//			data_byte3 = HIBYTE Flow
//			data_byte4 = LOBYTE Pressure
//			data_byte5 = HIBYTE Pressure
//			data_byte6 = LOBYTE Flow
//			data_byte7 = HIBYTE Flow
//			... data_byte19 - data bytes 0 thru 19*/
//			
//
//		/*msg->byBuffer[0]=0x80;
//		msg->byBuffer[1]=m_byBuffer[0];
//		msg->byBuffer[2]=m_byBuffer[1];
//		msg->byBuffer[3]=m_byBuffer[2];
//		msg->byBuffer[4]=m_byBuffer[3];
//		msg->byBuffer[5]=m_byBuffer[4];
//		msg->byBuffer[6]=m_byBuffer[5];
//		msg->byBuffer[7]=m_byBuffer[6];
//		msg->byBuffer[8]=m_byBuffer[7];
//		msg->byBuffer[9]=m_byBuffer[8];
//		msg->byBuffer[10]=m_byBuffer[9];
//		msg->byBuffer[11]=m_byBuffer[10];
//		msg->byBuffer[12]=m_byBuffer[11];
//		msg->byBuffer[13]=m_byBuffer[12];
//		msg->byBuffer[14]=m_byBuffer[13];
//		msg->byBuffer[15]=m_byBuffer[14];
//		msg->byBuffer[16]=m_byBuffer[15];
//		msg->byBuffer[17]=m_byBuffer[16];
//		msg->byBuffer[18]=m_byBuffer[17];
//		msg->byBuffer[19]=m_byBuffer[18];
//		msg->byBuffer[20]=m_byBuffer[19];
//		
//		msg->bySize=21;*/
//		
//		/*Data is transmitted and received as data packets in the following format:
//			CMD - NBF - [data_byte0 ... data_byten] - CKS
//		where:
//		CMD - command byte, range 80h - FFh
//			NBF- number of bytes to follow this byte, including the checksum
//			data_byte0 ... data_byten - data bytes 0 thru n
//			CKS - checksum*/
//
//
//	
//
//		//CStringW szTemp=_T("");
//		/*szTemp.Format(_T("%X,%X,%X,%X,%X,%X,%X,%X,%X,%X,%X,%X,%X,%X,%X,%X,%X,%X,%X,%X,%X \n"), 
//								msg->byBuffer[0],msg->byBuffer[1],msg->byBuffer[2],msg->byBuffer[3],
//								msg->byBuffer[4],msg->byBuffer[5],msg->byBuffer[6],msg->byBuffer[7],
//								msg->byBuffer[8],msg->byBuffer[9],msg->byBuffer[10],msg->byBuffer[11],
//								msg->byBuffer[12],msg->byBuffer[13],msg->byBuffer[14],msg->byBuffer[15],
//								msg->byBuffer[16],msg->byBuffer[17],msg->byBuffer[18],msg->byBuffer[19],msg->byBuffer[20]);
//		*/
//		/*szTemp.Format(_T("#%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d"), 
//			m_byBuffer[0],m_byBuffer[1],m_byBuffer[2],m_byBuffer[3],
//			m_byBuffer[4],m_byBuffer[5],m_byBuffer[6],m_byBuffer[7],
//			m_byBuffer[8],m_byBuffer[9],m_byBuffer[10],m_byBuffer[11],
//			m_byBuffer[12],m_byBuffer[13],m_byBuffer[14],m_byBuffer[15],
//			m_byBuffer[16],m_byBuffer[17],m_byBuffer[18],m_byBuffer[19]);*/
//
//		//msg->szMessage=szTemp;
//		//theApp.WriteLog(szTemp);
//		//TRACE(szTemp);
//
//
//
//		
//
//		//if(m_comPort.m_hCommPort!=INVALID_HANDLE_VALUE )
//		//{
//		//	EnterCriticalSection(&csMSGSend);
//
//		//	//MSGSendBuf.AddTail(msg);
//		//	MSGSend.AddTail(szTemp);
//
//		//	LeaveCriticalSection(&csMSGSend);
//
//		//	g_eventTerminalSendData.SetEvent();
//		//}
//	//}
//	
//
//}

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

/**********************************************************************************************//**
 * Queries if the wave data is active
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	True if the wave data is active, false if not.
 **************************************************************************************************/

bool CInterfaceTerminal::isWaveDataActive()
{
	EnterCriticalSection(&csEnableSend);
	bool bSend=m_bSendWaveData;
	LeaveCriticalSection(&csEnableSend);
	return bSend;
}

/**********************************************************************************************//**
 * Sets wave data active
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CInterfaceTerminal::setWaveDataActive()
{
	EnterCriticalSection(&csEnableSend);
	m_bSendWaveData=true;
	LeaveCriticalSection(&csEnableSend);
	
}

/**********************************************************************************************//**
 * Queries if the measurement data is active
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	True if the measurement data is active, false if not.
 **************************************************************************************************/

bool CInterfaceTerminal::isMeasurementDataActive()
{
	return m_bSendContiniousMeasurements;
}
//void CInterfaceTerminal::sendData_Cmd(BYTE command)
//{
//	LPTERMINALMSG msg = new TERMINALMSG;
//	BYTE checksum=0;
//
//	msg->byBuffer[0]=TERM_MSG_SOM;
//	msg->byBuffer[1]=2; //size bytes
//	msg->byBuffer[2]=command; 
//
//	for(int i=1;i<=2;i++)
//	{
//		checksum += msg->byBuffer[i];
//	}
//
//	msg->byBuffer[3]=checksum;
//
//	msg->bySize=4;
//
//#ifdef DUMP_TERMINALCMD
//	DEBUGMSG(TRUE, (TEXT("Terminal CMD: %x %x %x %x  \r\n"),msg->byBuffer[0],msg->byBuffer[1],msg->byBuffer[2],msg->byBuffer[3]));
//#else
//	send(msg);
//#endif
//	
//}
//
//
//void CInterfaceTerminal::sendData_SHORT(BYTE command, SHORT val) 
//{
//	LPTERMINALMSG msg = new TERMINALMSG;
//	BYTE checksum=0;
//
//	msg->byBuffer[0]=TERM_MSG_SOM;
//	msg->byBuffer[1]=4; //size bytes
//	msg->byBuffer[2]=command; 
//	msg->byBuffer[3]=HIBYTE(val);
//	msg->byBuffer[4]=LOBYTE(val);
//
//	for(int i=1;i<=4;i++)
//	{
//		checksum += msg->byBuffer[i];
//	}
//	msg->byBuffer[5]=checksum;
//
//	msg->bySize=6;
//
//#ifdef DUMP_TERMINALCMD
//	DEBUGMSG(TRUE, (TEXT("Terminal CMD: %x %x %x %x %x %x\r\n"),msg->byBuffer[0],msg->byBuffer[1],msg->byBuffer[2],msg->byBuffer[3],msg->byBuffer[4],msg->byBuffer[5]));
//#else
//	send(msg);
//#endif
//}
//
//void CInterfaceTerminal::sendData_BYTE(BYTE command, BYTE val) 
//{
//	//SOM|NBF|CMD|DATA0|...|DATAn|CKS
//	//message header-0x02
//	//NBF: number of bytes to follow this byte, including the checksum
//	//CMD: command byte
//	//DATA byte 0 ... DATA byte n - data bytes 0 thru n
//	//CKS: checksum, is computed by adding the data bytes using unsigned modulo 256 arithmetic, beginning with NBF,CMD,DATAbyte 0...DATAbyte n
//
//	LPTERMINALMSG msg = new TERMINALMSG;
//	BYTE checksum=0;
//
//	msg->byBuffer[0]=TERM_MSG_SOM;
//	msg->byBuffer[1]=3; //size bytes
//	msg->byBuffer[2]=command; 
//	msg->byBuffer[3]=val;
//
//	for(int i=1;i<=3;i++)
//	{
//		checksum += msg->byBuffer[i];
//	}
//	msg->byBuffer[4]=checksum;
//
//	msg->bySize=5;
//
//#ifdef DUMP_TERMINALCMD
//	DEBUGMSG(TRUE, (TEXT("Terminal CMD: %x %x %x %x %x\r\n"),msg->byBuffer[0],msg->byBuffer[1],msg->byBuffer[2],msg->byBuffer[3],msg->byBuffer[4]));
//#else
//	send(msg);
//#endif
//}


//void CInterfaceTerminal::sendMessurementData()
//{
//	//DEBUGMSG(TRUE, (TEXT("sendMessurementData1\r\n")));
//	//SOM|NBF|CMD|DATA0|...|DATAn|CKS
//	//message header-0x02
//	//NBF: number of bytes to follow this byte, including the checksum
//	//CMD: command byte
//	//DATA byte 0 ... DATA byte n - data bytes 0 thru n
//	//CKS: checksum, is computed by adding the data bytes using unsigned modulo 256 arithmetic, beginning with NBF,CMD,DATAbyte 0...DATAbyte n
//
//	MESSURE_BLOCKDATA pMessureData;
//
//	if(m_bMEASUREMENTS_BTB)
//	{
//		//getModel()->getDATAHANDLER()->getBlockMessureDataBTB(&pMessureData);
//		pMessureData.m_iPmax=getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_P_PEAK);
//		pMessureData.m_iPmitt=getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_P_MEAN);
//		pMessureData.m_iPEEP=getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_PEEP);
//		pMessureData.m_iCompliance=getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_DYNCOMPL);
//		pMessureData.m_iC20C=getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_C20C);
//		pMessureData.m_iResistance=getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_RESISTANCE);
//		pMessureData.m_iMV=getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_MIN_VOL);
//		pMessureData.m_iTVI=getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_TVI);
//		pMessureData.m_iTVE=getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_TVE);
//		pMessureData.m_iTVEresp=getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_TVE_RESP);
//		pMessureData.m_iTVEpat=getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_TVE_PATIENT);
//		pMessureData.m_iHFAmpl=getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_HFO_AMP);
//		pMessureData.m_iTVEHFO=getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_TVE_HFO);
//		pMessureData.m_iDCO2=getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_DCO2);
//		pMessureData.m_iTrigVol=getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_TRIGGER);
//		pMessureData.m_iITimePSV=getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_T_INSP_PSV);
//		pMessureData.m_iBPM=getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_FREQ);
//		pMessureData.m_iBPMco2=getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_FREQETCO2);
//		pMessureData.m_iLeak=getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_LEAK);
//		pMessureData.m_iHFFreq=getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_HFO_FREQ);
//		pMessureData.m_iPercent=getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_ANTEIL_RESP);
//
//	}
//	else //if (m_bMEASUREMENTS_AVG)
//	{
//		pMessureData.m_iPmax=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_P_PEAK);
//		pMessureData.m_iPmitt=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_P_MEAN);
//		pMessureData.m_iPEEP=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_PEEP);
//		pMessureData.m_iCompliance=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_DYNCOMPL);
//		pMessureData.m_iC20C=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_C20C);
//		pMessureData.m_iResistance=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_RESISTANCE);
//		pMessureData.m_iMV=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_MIN_VOL);
//		pMessureData.m_iTVI=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_TVI);
//		pMessureData.m_iTVE=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_TVE);
//		pMessureData.m_iTVEresp=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_TVE_RESP);
//		pMessureData.m_iTVEpat=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_TVE_PATIENT);
//		pMessureData.m_iHFAmpl=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_HFO_AMP);
//		pMessureData.m_iTVEHFO=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_TVE_HFO);
//		pMessureData.m_iDCO2=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_DCO2);
//		pMessureData.m_iTrigVol=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_TRIGGER);
//		pMessureData.m_iITimePSV=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_T_INSP_PSV);
//		pMessureData.m_iBPM=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_FREQ);
//		pMessureData.m_iBPMco2=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_FREQETCO2);
//		pMessureData.m_iLeak=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_LEAK);
//		pMessureData.m_iHFFreq=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_HFO_FREQ);
//		pMessureData.m_iPercent=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_ANTEIL_RESP);
//		//getModel()->getDATAHANDLER()->getBlockMessureDataAVG(&pMessureData);
//	}
//
//	bool bCO2value=false;
//	bool bCO2module=false;
//	bool bSPO2module=false;
//	
//	if(getModel()->getCONFIG()->getCO2module()!=CO2MODULE_NONE)
//	{
//		bCO2module=true;
//	}
//
//	if(	bCO2module && getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==true)
//	{
//		bCO2value=true;
//	}
//
//	if(getModel()->getCONFIG()->getSPO2module()!=SPO2MODULE_NONE)
//	{
//		bSPO2module=true;
//	}
//
//	SHORT iETCO2=TERMINAL_NOTVALID;
//	SHORT iSPO2=TERMINAL_NOTVALID;
//	SHORT iPulseRate=TERMINAL_NOTVALID;
//	SHORT iPerfusionIndex=TERMINAL_NOTVALID;
//
//	if(bSPO2module)
//	{
//		iSPO2=getModel()->getDATAHANDLER()->getAVGMessureDataSPO2();
//		iPulseRate=getModel()->getDATAHANDLER()->getAVGMessureDataSpO2PulseRate();
//		iPerfusionIndex=getModel()->getDATAHANDLER()->getAVGMessureDataSpO2PerfusionIndex();
//	}
//
//	if(bCO2module && getModel()->getETCO2()!=NULL)
//	{
//		if(getModel()->getETCO2()->isETCO2ValueValid())
//			iETCO2=getModel()->getDATAHANDLER()->getAVGMessureDataETCO2();
//	}
//	
//	SHORT iBPM=pMessureData.m_iBPM;
//	SHORT iBPMco2=pMessureData.m_iBPMco2;
//	SHORT iLeak=pMessureData.m_iLeak;
//	SHORT iHFFreq=pMessureData.m_iHFFreq;
//	SHORT iPercent=pMessureData.m_iPercent;
//
//
//	if(bCO2value)
//	{
//		iBPM=TERMINAL_NOTVALID;
//	}
//	else
//	{
//		iBPMco2=TERMINAL_NOTVALID;
//	}
//
//	SHORT iOxyVal=getModel()->getDATAHANDLER()->getMessureDataO2();
//	SHORT iINSP_FLOW=TERMINAL_NOTVALID;
//	SHORT iEXP_FLOW=TERMINAL_NOTVALID;
//	SHORT iDEMAND_FLOW=TERMINAL_NOTVALID;
//
//
//	if(getModel()->getVMODEHANDLER()->activeModeIsNMODE())
//	{
//		iINSP_FLOW=getModel()->getDATAHANDLER()->GetInspFlowData();
//
//		iEXP_FLOW=getModel()->getDATAHANDLER()->GetExpFlowData();
//	}
//	else if(getModel()->getVMODEHANDLER()->activeModeIsCPAP())
//	{
//		iDEMAND_FLOW=getModel()->getDATAHANDLER()->GetDemandFlowData();
//	}
//
//
//	
//	
//
//	eVentMode eActiveVentMode;
//	if(getModel()->getCONFIG()->CurModeIsPresetMode()==false)
//		eActiveVentMode=getModel()->getCONFIG()->GetCurMode();
//	else
//		eActiveVentMode=getModel()->getCONFIG()->GetPrevMode();
//
//	switch(eActiveVentMode)
//	{
//	case VM_IPPV:
//		{
//			pMessureData.m_iTrigVol=TERMINAL_NOTVALID; 
//			pMessureData.m_iHFAmpl=TERMINAL_NOTVALID;
//			pMessureData.m_iTVEHFO=TERMINAL_NOTVALID;
//			pMessureData.m_iDCO2=TERMINAL_NOTVALID;
//			iHFFreq=TERMINAL_NOTVALID;
//			iPercent=TERMINAL_NOTVALID;
//			pMessureData.m_iTVEresp=TERMINAL_NOTVALID;
//			pMessureData.m_iTVEpat=TERMINAL_NOTVALID;
//		}
//		break;
//	case VM_SIPPV:
//	case VM_PSV:
//		{
//			pMessureData.m_iHFAmpl=TERMINAL_NOTVALID;
//			pMessureData.m_iTVEHFO=TERMINAL_NOTVALID;
//			pMessureData.m_iDCO2=TERMINAL_NOTVALID;
//			iHFFreq=TERMINAL_NOTVALID;
//			pMessureData.m_iTVEresp=TERMINAL_NOTVALID; 
//			pMessureData.m_iTVEpat=TERMINAL_NOTVALID; 
//			iPercent=TERMINAL_NOTVALID; 
//		}
//		break;
//	case VM_SIMV:
//	case VM_SIMVPSV:
//		{
//			pMessureData.m_iHFAmpl=TERMINAL_NOTVALID;
//			pMessureData.m_iTVEHFO=TERMINAL_NOTVALID;
//			pMessureData.m_iDCO2=TERMINAL_NOTVALID;
//			iHFFreq=TERMINAL_NOTVALID;
//		}
//		break;
//	case VM_CPAP:
//		{
//			pMessureData.m_iHFAmpl=TERMINAL_NOTVALID;
//			pMessureData.m_iTVEHFO=TERMINAL_NOTVALID;
//			pMessureData.m_iDCO2=TERMINAL_NOTVALID;
//			iHFFreq=TERMINAL_NOTVALID;
//			pMessureData.m_iTrigVol=TERMINAL_NOTVALID; 
//			pMessureData.m_iTVEresp=TERMINAL_NOTVALID; 
//			pMessureData.m_iTVEpat=TERMINAL_NOTVALID; 
//			iPercent=TERMINAL_NOTVALID; 
//		}
//		break;
//	case VM_DUOPAP:
//		{
//			pMessureData.m_iHFAmpl=TERMINAL_NOTVALID;
//			pMessureData.m_iTVEHFO=TERMINAL_NOTVALID;
//			pMessureData.m_iDCO2=TERMINAL_NOTVALID;
//			iHFFreq=TERMINAL_NOTVALID;
//			pMessureData.m_iTrigVol=TERMINAL_NOTVALID; 
//			pMessureData.m_iTVEresp=TERMINAL_NOTVALID; 
//			pMessureData.m_iTVEpat=TERMINAL_NOTVALID; 
//			iPercent=TERMINAL_NOTVALID; 
//		}
//		break;
//	case VM_NCPAP:
//		{
//			pMessureData.m_iHFAmpl=TERMINAL_NOTVALID;
//			pMessureData.m_iTVEHFO=TERMINAL_NOTVALID;
//			pMessureData.m_iDCO2=TERMINAL_NOTVALID;
//			iHFFreq=TERMINAL_NOTVALID;
//			pMessureData.m_iTrigVol=TERMINAL_NOTVALID; 
//			pMessureData.m_iTVEresp=TERMINAL_NOTVALID; 
//			pMessureData.m_iTVEpat=TERMINAL_NOTVALID; 
//			iPercent=TERMINAL_NOTVALID; 
//		}
//		break;
//	case VM_THERAPIE:
//		{
//			pMessureData.m_iHFAmpl=TERMINAL_NOTVALID;
//			pMessureData.m_iTVEHFO=TERMINAL_NOTVALID;
//			pMessureData.m_iDCO2=TERMINAL_NOTVALID;
//			iHFFreq=TERMINAL_NOTVALID;
//			pMessureData.m_iTrigVol=TERMINAL_NOTVALID; 
//			pMessureData.m_iTVEresp=TERMINAL_NOTVALID; 
//			pMessureData.m_iTVEpat=TERMINAL_NOTVALID; 
//			iPercent=TERMINAL_NOTVALID; 
//		}
//		break;
//	case VM_HFO:
//		{
//			iBPMco2=TERMINAL_NOTVALID;
//			iBPM=TERMINAL_NOTVALID;
//			pMessureData.m_iTrigVol=TERMINAL_NOTVALID; 
//			iPercent=TERMINAL_NOTVALID; 
//			pMessureData.m_iTVE =TERMINAL_NOTVALID;
//			pMessureData.m_iTVEresp=TERMINAL_NOTVALID; 
//			pMessureData.m_iTVEpat=TERMINAL_NOTVALID; 
//			pMessureData.m_iPEEP =TERMINAL_NOTVALID;
//		}
//		break;
//	default:
//		{
//
//		}
//		break;
//	}
//
//	LPTERMINALMSG msg = new TERMINALMSG;
//	BYTE checksum=0;
//
//	msg->byBuffer[0]=TERM_MSG_SOM;
//	msg->byBuffer[1]=61; //size bytes
//
//	if(m_bMEASUREMENTS_BTB)
//	{
//		msg->byBuffer[2]=TERMINAL_MEASUREMENTS_BTB; 
//	}
//	else //if (m_bMEASUREMENTS_AVG)
//	{
//		msg->byBuffer[2]=TERMINAL_MEASUREMENTS_AVG; 
//	}
//
//	msg->byBuffer[3]=(BYTE)eActiveVentMode;
//	msg->byBuffer[4]=HIBYTE(pMessureData.m_iPmax);
//	msg->byBuffer[5]=LOBYTE(pMessureData.m_iPmax);
//	msg->byBuffer[6]=HIBYTE(pMessureData.m_iPmitt);
//	msg->byBuffer[7]=LOBYTE(pMessureData.m_iPmitt);
//	msg->byBuffer[8]=HIBYTE(pMessureData.m_iPEEP);
//	msg->byBuffer[9]=LOBYTE(pMessureData.m_iPEEP);
//	msg->byBuffer[10]=HIBYTE(pMessureData.m_iCompliance);
//	msg->byBuffer[11]=LOBYTE(pMessureData.m_iCompliance);
//	msg->byBuffer[12]=HIBYTE(pMessureData.m_iC20C);
//	msg->byBuffer[13]=LOBYTE(pMessureData.m_iC20C);
//	msg->byBuffer[14]=HIBYTE(pMessureData.m_iResistance);
//	msg->byBuffer[15]=LOBYTE(pMessureData.m_iResistance);
//	msg->byBuffer[16]=HIBYTE(pMessureData.m_iMV);
//	msg->byBuffer[17]=LOBYTE(pMessureData.m_iMV);
//	msg->byBuffer[18]=HIBYTE(pMessureData.m_iTVI);
//	msg->byBuffer[19]=LOBYTE(pMessureData.m_iTVI);
//	msg->byBuffer[20]=HIBYTE(pMessureData.m_iTVE);
//	msg->byBuffer[21]=LOBYTE(pMessureData.m_iTVE);
//	msg->byBuffer[22]=HIBYTE(pMessureData.m_iTVEresp);
//	msg->byBuffer[23]=LOBYTE(pMessureData.m_iTVEresp);
//	msg->byBuffer[24]=HIBYTE(pMessureData.m_iTVEpat);
//	msg->byBuffer[25]=LOBYTE(pMessureData.m_iTVEpat);
//	msg->byBuffer[26]=HIBYTE(pMessureData.m_iHFAmpl);
//	msg->byBuffer[27]=LOBYTE(pMessureData.m_iHFAmpl);
//	msg->byBuffer[28]=HIBYTE(pMessureData.m_iTVEHFO);
//	msg->byBuffer[29]=LOBYTE(pMessureData.m_iTVEHFO);
//	msg->byBuffer[30]=HIBYTE(pMessureData.m_iDCO2);
//	msg->byBuffer[31]=LOBYTE(pMessureData.m_iDCO2);
//	msg->byBuffer[32]=HIBYTE(pMessureData.m_iTrigVol);
//	msg->byBuffer[33]=LOBYTE(pMessureData.m_iTrigVol);
//	msg->byBuffer[34]=HIBYTE(pMessureData.m_iITimePSV);
//	msg->byBuffer[35]=LOBYTE(pMessureData.m_iITimePSV);
//	msg->byBuffer[36]=HIBYTE(iSPO2);
//	msg->byBuffer[37]=LOBYTE(iSPO2);
//	msg->byBuffer[38]=HIBYTE(iPulseRate);
//	msg->byBuffer[39]=LOBYTE(iPulseRate);
//	msg->byBuffer[40]=HIBYTE(iPerfusionIndex);
//	msg->byBuffer[41]=LOBYTE(iPerfusionIndex);
//	msg->byBuffer[42]=HIBYTE(iETCO2);
//	msg->byBuffer[43]=LOBYTE(iETCO2);
//	msg->byBuffer[44]=HIBYTE(iBPM);
//	msg->byBuffer[45]=LOBYTE(iBPM);
//	msg->byBuffer[46]=HIBYTE(iBPMco2);
//	msg->byBuffer[47]=LOBYTE(iBPMco2);
//	msg->byBuffer[48]=HIBYTE(iLeak);
//	msg->byBuffer[49]=LOBYTE(iLeak);
//	msg->byBuffer[50]=HIBYTE(iHFFreq);
//	msg->byBuffer[51]=LOBYTE(iHFFreq);
//	msg->byBuffer[52]=HIBYTE(iPercent);
//	msg->byBuffer[53]=LOBYTE(iPercent);
//	msg->byBuffer[54]=HIBYTE(iOxyVal);
//	msg->byBuffer[55]=LOBYTE(iOxyVal);
//	msg->byBuffer[56]=HIBYTE(iINSP_FLOW);
//	msg->byBuffer[57]=LOBYTE(iINSP_FLOW);
//	msg->byBuffer[58]=HIBYTE(iEXP_FLOW);
//	msg->byBuffer[59]=LOBYTE(iEXP_FLOW);
//	msg->byBuffer[60]=HIBYTE(iDEMAND_FLOW);
//	msg->byBuffer[61]=LOBYTE(iDEMAND_FLOW);
//
//
//	for(int i=1;i<=61;i++)
//	{
//		checksum += msg->byBuffer[i];
//	}
//	msg->byBuffer[62]=checksum;
//
//	msg->bySize=63;
//
//	send(msg);
//
//	
//}