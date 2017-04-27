#include "StdAfx.h"
#include "InterfaceSerial.h"
#include "MVModel.h"
#include "acuLink.h"

//extern CEvent g_eventAcknowledge;
//extern CEvent g_eventCOMCheckData;
extern CEvent g_eventCOM;

extern CEvent g_evBLENDER_VERSION;	//H
extern CEvent g_evCOM_VERSION; //Y
extern CEvent g_evCOM_CAL_PRESS_OFFSET; //G
extern CEvent g_evCOM_CAL_PRESS_SCALE;	 //B
extern CEvent g_evCOM_CHECKSUM;	//N
extern CEvent g_evCOM_STATUS; //L
extern CEvent g_evCOM_VENT_MODE; //O
extern CEvent g_evCOM_M_INSP_FLOW;	 //X
extern CEvent g_evCOM_M_EXP_FLOW;	 //x
extern CEvent g_evCOM_M_DEMAND_FLOW;	 //b
extern CEvent g_evBLENDER_STATUS; //l
extern CEvent eventDoTestTimerFunctions;

#define ERRORCNT_CHECK	1

CInterfaceSerial* CInterfaceSerial::theSerialInterface=0;


CInterfaceSerial::CInterfaceSerial(void)
{
	
	m_bDoReceiveThread=false;
	m_bDoSendThread=false;
	m_bDoCheckThread=false;


	m_bStarted=true;
	//m_bCheckMstate=false;
	m_dwLastCheckMstate=0;

	m_iCALPRESSSCALE=0;
	m_iCALPRESSOFFSET=0;
	m_bCALPRESSOFFSET=false;
	m_bCALPRESSSCALE=false;

	m_iM_INSP_FLOW=0;
	m_iM_EXP_FLOW=0;

	m_iM_DEMAND_FLOW=0;

	m_iErrorCountCheckThread=0;
	m_iErrorCountSendThread=0;

	m_bSerialDisconnection=false;
	/*m_bSerialINSPIRATIONTUBE=false;
	m_bSerialEXSPIRATIONTUBE=false;*/
	m_bSerialTUBE_OCCLUSION=false;

	/*m_iError_evCOM_VERSION=0;
	m_iError_evCOM_CAL_PRESS_OFFSET=0;
	m_iError_evCOM_CAL_PRESS_SCALE=0;
	m_iError_evCOM_CHECKSUM=0;
	m_iError_evCOM_STATUS=0;
	m_iError_evCOM_M_INSP_FLOW=0;
	m_iError_evCOM_M_EXP_FLOW=0;
	m_iError_evCOM_M_DEMAND_FLOW=0;*/

	m_pcwtSerialCheckThread=NULL;
	m_pcwtSerialReceiveThread=NULL;
	m_pcwtSerialSendThread=NULL;

	m_hThreadSerialReceive=INVALID_HANDLE_VALUE;
	m_hThreadSerialSend=INVALID_HANDLE_VALUE;
	m_hThreadSerialCheck=INVALID_HANDLE_VALUE;

	m_szCtrlVersion=_T("0.0.0");
}

CInterfaceSerial::~CInterfaceSerial(void)
{
	
}

// **************************************************************************
// 
// **************************************************************************
CInterfaceSerial* CInterfaceSerial::GetInstance()
{
	if(theSerialInterface == 0)
	{
		theSerialInterface = new CInterfaceSerial;
	}
	return theSerialInterface;
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceSerial::DestroyInstance()
{
	if(theSerialInterface != NULL)
	{
		delete theSerialInterface;
		theSerialInterface = NULL;
	}
}


// **************************************************************************
// 
// **************************************************************************
bool CInterfaceSerial::Init()
{
	InitializeCriticalSection(&csMSGSend);

	InitializeCriticalSection(&csReceiveThread);
	InitializeCriticalSection(&csSendThread);
	InitializeCriticalSection(&csCheckThread);

	//TEST LOCK SERIAL
	//InitializeCriticalSection(&csTestLock);

	DWORD dwIndex=0;

	if(getModel()->getCONFIG()->GetMainBoardVersion()>=MAINBOARD_30)
		dwIndex=1;
	else	//MAINBOARD2_1
		dwIndex=2;

	if (m_comPort.Open(dwIndex,9600)!=0)
	{
		theApp.getLog()->WriteLine(_T("#HFO:0009"));
		return false;
	}
	else
	{
		m_comPort.FlushBuffer();

		StartSerialThread();
		return true;
	}


	//check for COM
	//m_szCtrlVersion=_T("0.0.0");
	//UINT iCOM=0;
	//if(getModel()->getCONFIG()->GetMainBoardVersion()>=MAINBOARD3_0)//check for MAINBOARD3_0
	//{
	//	//try COM 1
	//	if(m_comPort.Open(SERIAL_COM1,9600))
	//	{
	//		//check for identifier "ctl"
	//		m_szCtrlVersion=getModel()->getSERIAL()->GetM_VERSION(true);
	//		
	//		if(0==m_szCtrlVersion.Find(_T('ctl')))
	//		{
	//			iCOM=SERIAL_COM1;
	//			m_szCtrlVersion = m_szCtrlVersion.Right(m_szCtrlVersion.GetLength()-3);
	//			getModel()->getCONFIG()->setMainBoardVersion(MAINBOARD3_0);

	//			DEBUGMSG(TRUE, (TEXT("SERIAL_COM1 found, MAINBOARD3_0\r\n")));
	//		}
	//		else
	//		{
	//			m_comPort.Close();
	//		}
	//	}
	//}
	//

	//if(iCOM==0)
	//{
	//	if(m_comPort.Open(SERIAL_COM2,9600))//check for MAINBOARD2_1
	//	{
	//		//check for identifier "ctl"
	//		m_szCtrlVersion=getModel()->getSERIAL()->GetM_VERSION(true);

	//		if(0==m_szCtrlVersion.Find(_T('ctl')))
	//		{
	//			iCOM=SERIAL_COM2;
	//			m_szCtrlVersion = m_szCtrlVersion.Right(m_szCtrlVersion.GetLength()-3);
	//			getModel()->getCONFIG()->setMainBoardVersion(MAINBOARD2_1);

	//			DEBUGMSG(TRUE, (TEXT("SERIAL_COM2 found, MAINBOARD2_1\r\n")));
	//		}
	//		else
	//		{
	//			m_comPort.Close();
	//		}
	//	}

	//	if(iCOM==0)
	//	{
	//		if(m_comPort.Open(SERIAL_COM1,9600))//check for MAINBOARD3_0
	//		{
	//			//check for identifier "ctl"
	//			m_szCtrlVersion=getModel()->getSERIAL()->GetM_VERSION(true);

	//			if(0==m_szCtrlVersion.Find(_T('ctl')))
	//			{
	//				iCOM=SERIAL_COM1;
	//				m_szCtrlVersion = m_szCtrlVersion.Right(m_szCtrlVersion.GetLength()-3);
	//				getModel()->getCONFIG()->setMainBoardVersion(MAINBOARD3_0);

	//				DEBUGMSG(TRUE, (TEXT("SERIAL_COM1 found, MAINBOARD3_0\r\n")));
	//			}
	//			else
	//			{
	//				m_comPort.Close();
	//			}
	//		}
	//	}
	//	
	//}

	//if(iCOM==0)//not found yet
	//{
	//	if(m_comPort.Open(SERIAL_COM2,9600))//try COM 2 without identifier "ctl"
	//	{
	//		//check for identifier "ctl"
	//		m_szCtrlVersion=getModel()->getSERIAL()->GetM_VERSION(true);

	//		if(m_szCtrlVersion!=_T("0.0.0"))
	//		{
	//			if(getModel()->getSERIAL()->GetM_StatusByte())//to be sure check for status byte
	//			{
	//				iCOM=SERIAL_COM2;
	//				m_szCtrlVersion = m_szCtrlVersion.Right(m_szCtrlVersion.GetLength()-3);
	//				getModel()->getCONFIG()->setMainBoardVersion(MAINBOARD2_1);
	//			}
	//			else
	//			{
	//				m_comPort.Close();
	//			}
	//			
	//		}
	//		else
	//		{
	//			m_comPort.Close();
	//		}
	//		
	//	}
	//}
	//
	//if (iCOM==0)
	//{
	//	theApp.getLog()->WriteLine(_T("ERROR - OpenCom CInterfaceSerial"));
	//	return false;
	//}
	//else
	//{
	//	StartSerialThread();
	//	return true;
	//}
	
}

//************************************
// Method:    Deinit
// FullName:  CInterfaceSerial::Deinit
// Access:    public 
// Returns:   bool
// Qualifier:
//
// 2015/06/19: checked for correct closing of thread
//************************************
bool CInterfaceSerial::Deinit()
{
	getModel()->setSERIALavailable(FALSE);
	StopSerialThread();

	if(m_pcwtSerialCheckThread!=NULL)
	{
		delete m_pcwtSerialCheckThread;
		m_pcwtSerialCheckThread=NULL;

		if(m_hThreadSerialCheck!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadSerialCheck);
			m_hThreadSerialCheck=INVALID_HANDLE_VALUE;
		}
	}

	if(m_pcwtSerialReceiveThread!=NULL)
	{
		delete m_pcwtSerialReceiveThread;
		m_pcwtSerialReceiveThread=NULL;

		if(m_hThreadSerialReceive!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadSerialReceive);
			m_hThreadSerialReceive=INVALID_HANDLE_VALUE;
		}
	}

	if(m_pcwtSerialSendThread!=NULL)
	{
		delete m_pcwtSerialSendThread;
		m_pcwtSerialSendThread=NULL;

		if(m_hThreadSerialSend!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadSerialSend);
			m_hThreadSerialSend=INVALID_HANDLE_VALUE;
		}
	}

	EnterCriticalSection(&csMSGSend);
	MSGSend.RemoveAll();
	LeaveCriticalSection(&csMSGSend);

	m_comPort.FlushBuffer();
	m_comPort.Close();

	DeleteCriticalSection(&csMSGSend);

	DeleteCriticalSection(&csReceiveThread);
	DeleteCriticalSection(&csSendThread);
	DeleteCriticalSection(&csCheckThread);

	return true;
}

// **************************************************************************
// 
// **************************************************************************
bool CInterfaceSerial::IsAvailable()
{
	if (m_comPort.m_hCommPort==INVALID_HANDLE_VALUE)
	{
		return false;
	}
	else
	{
		return true;
	}
}

// **************************************************************************
// 
// **************************************************************************
BOOL CInterfaceSerial::FlushBuffer()
{
	return m_comPort.FlushBuffer();
}


// **************************************************************************
// 
// **************************************************************************
void CInterfaceSerial::StartSerialThread( void )
{
	startReceiveThread();

	if(m_pcwtSerialReceiveThread!=NULL)
	{
		delete m_pcwtSerialReceiveThread;
		m_pcwtSerialReceiveThread=NULL;

		if(m_hThreadSerialReceive!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadSerialReceive);
			m_hThreadSerialReceive=INVALID_HANDLE_VALUE;
		}
	}
	//DEBUGMSG(TRUE, (TEXT("StartSerialThread2\r\n")));

	m_pcwtSerialReceiveThread=AfxBeginThread(CSerialReceiveThread,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	m_hThreadSerialReceive=m_pcwtSerialReceiveThread->m_hThread;
	m_pcwtSerialReceiveThread->m_bAutoDelete = FALSE; 
	m_pcwtSerialReceiveThread->ResumeThread();

	//DEBUGMSG(TRUE, (TEXT("StartSerialThread3\r\n")));

	startSendThread();
	//m_pcwtSerialSendThread=AfxBeginThread(CSerialSendThread,this);

	if(m_pcwtSerialSendThread!=NULL)
	{
		delete m_pcwtSerialSendThread;
		m_pcwtSerialSendThread=NULL;

		if(m_hThreadSerialSend!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadSerialSend);
			m_hThreadSerialSend=INVALID_HANDLE_VALUE;
		}
	}

	//DEBUGMSG(TRUE, (TEXT("StartSerialThread4\r\n")));

	m_pcwtSerialSendThread=AfxBeginThread(CSerialSendThread,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	m_hThreadSerialSend=m_pcwtSerialSendThread->m_hThread;
	m_pcwtSerialSendThread->m_bAutoDelete = FALSE; 
	m_pcwtSerialSendThread->ResumeThread();

	startCheckThread();
	//m_pcwtSerialCheckThread=AfxBeginThread(CSerialCheckThread,this);

	//DEBUGMSG(TRUE, (TEXT("StartSerialThread5\r\n")));

	if(m_pcwtSerialCheckThread!=NULL)
	{
		delete m_pcwtSerialCheckThread;
		m_pcwtSerialCheckThread=NULL;

		if(m_hThreadSerialCheck!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadSerialCheck);
			m_hThreadSerialCheck=INVALID_HANDLE_VALUE;
		}
	}

	//DEBUGMSG(TRUE, (TEXT("StartSerialThread6\r\n")));

	m_pcwtSerialCheckThread=AfxBeginThread(CSerialCheckThread,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	m_hThreadSerialCheck=m_pcwtSerialCheckThread->m_hThread;
	m_pcwtSerialCheckThread->m_bAutoDelete = FALSE; 
	m_pcwtSerialCheckThread->ResumeThread();

	//DEBUGMSG(TRUE, (TEXT("StartSerialThread7\r\n")));
}

//************************************
// Method:    StopSerialThread
// FullName:  CInterfaceSerial::StopSerialThread
// Access:    protected 
// Returns:   void
// Qualifier:
// Parameter: void
//
// 2015/06/19: checked for correct closing of thread
//************************************
void CInterfaceSerial::StopSerialThread( void )
{
	if(doSendThread())
	{
		stopSendThread();

		if (WaitForSingleObject(m_pcwtSerialSendThread->m_hThread,2000) == WAIT_TIMEOUT)
		{
			DEBUGMSG(TRUE, (TEXT("TerminateThread 1 !!!!!!!!!!!!!!\r\n")));
			theApp.getLog()->WriteLine(_T("#THR:011a"));
			
			if(!TerminateThread(m_pcwtSerialSendThread,0))
			{
				theApp.getLog()->WriteLine(_T("#THR:011b"));
			}
		}
	}

	if(doCheckThread())
	{
		stopCheckThread();
		//g_eventCOMCheckData.SetEvent();

		eventCheckPara.SetEvent();

		//DEBUGMSG(TRUE, (TEXT("StopSerialThread 1\r\n")));
		if (WaitForSingleObject(m_pcwtSerialCheckThread->m_hThread,1000) == WAIT_TIMEOUT)
		{
			DEBUGMSG(TRUE, (TEXT("TerminateThread 2 !!!!!!!!!!!!!!\r\n")));
			theApp.getLog()->WriteLine(_T("#THR:012a"));

			if(!TerminateThread(m_pcwtSerialCheckThread,0))
			{
				theApp.getLog()->WriteLine(_T("#THR:012b"));
			}
		}
	}
	

	if(doReceiveThread())
	{
		DWORD dwCommModemStatus;

		stopReceiveThread();
		
		GetCommMask(m_comPort.m_hCommPort,&dwCommModemStatus);
		BOOL fSuccess = SetCommMask (m_comPort.m_hCommPort,dwCommModemStatus);

		PurgeComm(m_comPort.m_hCommPort, PURGE_RXABORT | PURGE_RXCLEAR);

		if (WaitForSingleObject(m_pcwtSerialReceiveThread->m_hThread,1000) == WAIT_TIMEOUT)
		{
			DEBUGMSG(TRUE, (TEXT("TerminateThread 3 !!!!!!!!!!!!!!\r\n")));
			theApp.getLog()->WriteLine(_T("#THR:010a"));
			
			if(!TerminateThread(m_pcwtSerialReceiveThread,0))
			{
				theApp.getLog()->WriteLine(_T("#THR:010b"));
			}
		}
	}
}

/**********************************************************************************************//**
 * @fn	bool CInterfaceSerial::doReceiveThread()
 *
 * @brief	Executes the receive thread operation.
 *
 * @author	Rainer Kuehner
 * @date	19.04.2016
 *
 * @return	true if it succeeds, false if it fails.
 **************************************************************************************************/
bool CInterfaceSerial::doReceiveThread()
{
	bool bRes=false;
	EnterCriticalSection(&csReceiveThread);
	bRes=m_bDoReceiveThread;
	LeaveCriticalSection(&csReceiveThread);
	return bRes;
}
void CInterfaceSerial::startReceiveThread()
{
	EnterCriticalSection(&csReceiveThread);
	m_bDoReceiveThread=true;
	LeaveCriticalSection(&csReceiveThread);
}
void CInterfaceSerial::stopReceiveThread()
{
	EnterCriticalSection(&csReceiveThread);
	m_bDoReceiveThread=false;
	LeaveCriticalSection(&csReceiveThread);
}

/**********************************************************************************************//**
 * @fn	bool CInterfaceSerial::doSendThread()
 *
 * @brief	Executes the send thread operation.
 *
 * @author	Rainer Kuehner
 * @date	19.04.2016
 *
 * @return	true if it succeeds, false if it fails.
 **************************************************************************************************/
bool CInterfaceSerial::doSendThread()
{
	bool bRes=false;
	EnterCriticalSection(&csSendThread);
	bRes=m_bDoSendThread;
	LeaveCriticalSection(&csSendThread);
	return bRes;
}
void CInterfaceSerial::startSendThread()
{
	EnterCriticalSection(&csSendThread);
	m_bDoSendThread=true;
	LeaveCriticalSection(&csSendThread);
}
void CInterfaceSerial::stopSendThread()
{
	EnterCriticalSection(&csSendThread);
	m_bDoSendThread=false;
	LeaveCriticalSection(&csSendThread);
}

/**********************************************************************************************//**
 * @fn	bool CInterfaceSerial::doCheckThread()
 *
 * @brief	Executes the check thread operation.
 *
 * @author	Rainer Kuehner
 * @date	19.04.2016
 *
 * @return	true if it succeeds, false if it fails.
 **************************************************************************************************/
bool CInterfaceSerial::doCheckThread()
{
	bool bRes=false;
	EnterCriticalSection(&csCheckThread);
	bRes=m_bDoCheckThread;
	LeaveCriticalSection(&csCheckThread);
	return bRes;
}
void CInterfaceSerial::startCheckThread()
{
	EnterCriticalSection(&csCheckThread);
	m_bDoCheckThread=true;
	LeaveCriticalSection(&csCheckThread);
}
void CInterfaceSerial::stopCheckThread()
{
	EnterCriticalSection(&csCheckThread);
	m_bDoCheckThread=false;
	LeaveCriticalSection(&csCheckThread);
}

//#####################################CheckThread########################################
//************************************
// Method:    CSerialCheckThread
// FullName:  CSerialCheckThread
// Access:    public static 
// Returns:   UINT
// Qualifier:
// Parameter: LPVOID pc
//
// 2015/06/19: checked for correct closing of thread
//************************************
static UINT CSerialCheckThread( LPVOID pc )
{
	((CInterfaceSerial*)pc)->CheckSerialData();
	return TRUE;
}

//************************************
// Method:    CheckSerialData
// FullName:  CInterfaceSerial::CheckSerialData
// Access:    protected 
// Returns:   DWORD
// Qualifier:
// Parameter: void
//
// 2015/06/19: checked for correct closing of thread
//************************************
DWORD CInterfaceSerial::CheckSerialData(void) 
{
	WORD iCnt=0;
	while(iCnt<20 && doCheckThread())//newCOMCHECK
	{
		Sleep(500);
		iCnt++;
	}

	if(!doCheckThread())
	{
		return 0;
	}

	const int CMD_IPPV=10;//newVG 
	CStringW szIPPV[CMD_IPPV];
	szIPPV[0] = _T("Oq");	// 	VentMode Lesen
	szIPPV[1] = _T("Wq");	// 	I-Zeit Lesen
	szIPPV[2] = _T("Eq");	// 	E-Zeit Lesen
	szIPPV[3] = _T("Uq");	// 	I_FLOW Lesen
	szIPPV[4] = _T("Aq");	// 	E-Flow Lesen
	szIPPV[5] = _T("Tq");	// 	P_INSP Lesen
	szIPPV[6] = _T("Zq");	// 	PEEP Lesen
	szIPPV[7] = _T("Dq");	// 	O2 Lesen
	szIPPV[8] = _T("Rq");	// 	M_MODE_OPTION
	szIPPV[9] = _T(">q");	// 	P-MAX VG lesen//newVG 

	const int CMD_TRIG=10;//newVG 
	CStringW szTRIG[CMD_TRIG];
	szTRIG[0] = _T("Oq");	// 	VentMode Lesen
	szTRIG[1] = _T("Wq");	// 	I-Zeit Lesen
	szTRIG[2] = _T("Eq");	// 	E-Zeit Lesen
	szTRIG[3] = _T("Uq");	// 	I_FLOW Lesen
	szTRIG[4] = _T("Aq");	// 	E-Flow Lesen
	szTRIG[5] = _T("Tq");	// 	P_INSP Lesen
	szTRIG[6] = _T("Zq");	// 	PEEP Lesen
	szTRIG[7] = _T("Dq");	// 	O2 Lesen
	szTRIG[8] = _T("Rq");	// 	M_MODE_OPTION
	szTRIG[9] = _T(">q");	// 	P-MAX VG lesen//newVG 

	const int CMD_SIMVPSV=11;//newVG 
	CStringW szSIMVPSV[CMD_SIMVPSV];
	szSIMVPSV[0] = _T("Oq");	// 	VentMode Lesen
	szSIMVPSV[1] = _T("Wq");	// 	I-Zeit Lesen
	szSIMVPSV[2] = _T("Eq");	// 	E-Zeit Lesen
	szSIMVPSV[3] = _T("Uq");	// 	I_FLOW Lesen
	szSIMVPSV[4] = _T("Aq");	// 	E-Flow Lesen
	szSIMVPSV[5] = _T("Tq");	// 	P_INSP Lesen
	szSIMVPSV[6] = _T("Zq");	// 	PEEP Lesen
	szSIMVPSV[7] = _T("Cq");	// 	P_PSV Lesen
	szSIMVPSV[8] = _T("Dq");	// 	O2 Lesen
	szSIMVPSV[9] = _T("Rq");	// 	M_MODE_OPTION
	szSIMVPSV[10] = _T(">q");	// 	P-MAX VG lesen//newVG 

	const int CMD_PSV=10;//newVG 
	CStringW szPSV[CMD_PSV];
	szPSV[0] = _T("Oq");	// 	VentMode Lesen
	szPSV[1] = _T("Wq");	// 	I-Zeit Lesen
	szPSV[2] = _T("Eq");	// 	E-Zeit Lesen
	szPSV[3] = _T("Uq");	// 	I_FLOW Lesen
	szPSV[4] = _T("Aq");	// 	E-Flow Lesen
	szPSV[5] = _T("Cq");	// 	P_PSV Lesen
	szPSV[6] = _T("Zq");	// 	PEEP Lesen
	szPSV[7] = _T("Dq");	// 	O2 Lesen
	szPSV[8] = _T("Rq");	// 	M_MODE_OPTION
	szPSV[9] = _T(">q");	// 	P-MAX VG lesen//newVG 

	const int CMD_CPAP=6;
	CStringW szCPAP[CMD_CPAP];
	szCPAP[0] = _T("Oq");	// 	VentMode Lesen
	szCPAP[1] = _T("Aq");	// 	E-Flow Lesen
	szCPAP[2] = _T("Tq");	// 	P_INSP Lesen
	szCPAP[3] = _T("Zq");	// 	PEEP Lesen
	szCPAP[4] = _T("Dq");	// 	O2 Lesen
	szCPAP[5] = _T("Rq");// 	M_MODE_OPTION

	const int CMD_NCPAP=5;
	CStringW szNCPAP[CMD_NCPAP];
	szNCPAP[0] = _T("Oq");	// 	VentMode Lesen
	szNCPAP[1] = _T("Tq");	// 	P_INSP Lesen
	szNCPAP[2] = _T("Zq");	// 	PEEP Lesen
	szNCPAP[3] = _T("Dq");	// 	O2 Lesen
	szNCPAP[4] = _T("Rq");// 	M_MODE_OPTION
	
	const int CMD_DUOPAP=7;
	CStringW szDUOPAP[CMD_DUOPAP];
	szDUOPAP[0] = _T("Oq");	// 	VentMode Lesen
	szDUOPAP[1] = _T("Wq");	// 	I-Zeit Lesen
	szDUOPAP[2] = _T("Eq");	// 	E-Zeit Lesen
	szDUOPAP[3] = _T("Tq");	// 	P_INSP Lesen
	szDUOPAP[4] = _T("Zq");	// 	PEEP Lesen
	szDUOPAP[5] = _T("Dq");// 	O2 Lesen
	szDUOPAP[6] = _T("Rq");// 	M_MODE_OPTION

	const int CMD_HF=6;
	CStringW szHF[CMD_HF];
	szHF[0] = _T("Oq");	// 	VentMode Lesen
	szHF[1] = _T("Fq");	// 	HF-Freq Lesen
	szHF[2] = _T("Jq");	// 	HF->Pmitt Lesen
	szHF[3] = _T("Mq");	// 	HF-Ampl Lesen
	szHF[4] = _T("Dq");// 	O2 Lesen
	szHF[5] = _T("Rq");// 	M_MODE_OPTION

	const int CMD_THERAPY=4;
	CStringW szTHERAPY[CMD_THERAPY];
	szTHERAPY[0] = _T("Oq");	// 	VentMode Lesen
	szTHERAPY[1] = _T("Aq");	// 	E-Flow Lesen
	szTHERAPY[2] = _T("Dq");	// 	O2 Lesen
	szTHERAPY[3] = _T("Rq");	// 	M_MODE_OPTION

	DWORD WAIT=1000;
	DWORD dwWait=0;
	DWORD dwDiff = 0;
	DWORD dwStart=0;
	DWORD dwEnd=0;
	int iCmnd=0;
	bool bModeChanged=false;
	DWORD dwLastModeChanged=0;
	CStringW szCurCmd=_T("");
	m_iErrorCountCheckThread=0;

	eVentMode mode = getModel()->getCONFIG()->GetCurMode();

	do
	{
		dwEnd=GetTickCount();
		if(dwEnd>=dwStart)
			dwDiff=dwEnd-dwStart;
		else
			dwDiff=0;

		if(dwDiff<WAIT)
		{
			dwWait=WAIT-dwDiff;
		}
		else
		{
			dwWait=1;
		}

		if(!doCheckThread())
		{
			return 0;
		}

		DWORD dw = ::WaitForSingleObject(eventCheckPara, dwWait);

		dwStart = GetTickCount();

		if(!doCheckThread())
		{
			return 0;
		}

		switch(dw)
		{
		case WAIT_OBJECT_0:
			break;
		default:
			{
				if(!doCheckThread())
				{
					return 0;
				}
				else//if(m_bDoSerialCheckThread)
				{
					if(FALSE==getModel()->isSERIALavailable())
					{
						continue;
					}

					if(isSafeTickCountDelayExpired(m_dwLastCheckMstate, 1000))
					{
						GetM_StatusByte();
						m_dwLastCheckMstate=GetTickCount();
					}

					eVentSilentState silentState=getModel()->getALARMHANDLER()->getAlarmSilentState();
					if(		silentState==ASTATE_SYSTEM_SILENT
						||	silentState==ASTATE_SILENT)
					{
						continue;
					}

					if(		getModel()->getDATAHANDLER()->GetOxyCalRunning()==true
						||	getModel()->getDATAHANDLER()->GetO21SensorCalState()==true
						||	getModel()->getDATAHANDLER()->GetO100SensorCalState()==true)
						continue;

					if(getModel()->getCONFIG()->CurModeIsPresetMode())
					{
						continue;
					}

					if(mode != getModel()->getCONFIG()->GetCurMode() 
						&& getModel()->getCONFIG()->CurModeIsPresetMode()==false)
					{
						bModeChanged=true;
						dwLastModeChanged=GetTickCount();
						mode = getModel()->getCONFIG()->GetCurMode();
						iCmnd=0;
						m_iErrorCountCheckThread=0;
					}

					if(bModeChanged)
					{
						if(!isSafeTickCountDelayExpired(dwLastModeChanged, 5000))
						{
							continue;
						}
						else
						{
							bModeChanged=false;
						}
					}

					switch(mode)
					{
					case VM_IPPV:
						{
							szCurCmd=szIPPV[iCmnd];
						}
						break;
					case VM_SIMVPSV:
						{
							szCurCmd=szSIMVPSV[iCmnd];
						}
						break;
					case VM_PSV:
						{
							szCurCmd=szPSV[iCmnd];
						}
						break;
					case VM_SIPPV:
					case VM_SIMV:
						{
							szCurCmd=szTRIG[iCmnd];
						}
						break;
					case VM_CPAP:
						{
							szCurCmd=szCPAP[iCmnd];
						}
						break;
					case VM_NCPAP:
						{
							szCurCmd=szNCPAP[iCmnd];
						}
						break;
					case VM_DUOPAP:
						{
							szCurCmd=szDUOPAP[iCmnd];
						}
						break;
					case VM_HFO:
						{
							szCurCmd=szHF[iCmnd];
						}
					break;
 					case VM_THERAPIE:
 						{
 							szCurCmd=szTHERAPY[iCmnd];
 						}
 						break;
					default:
						continue;
						break;
					}

					BYTE chResult=0;
					if(szCurCmd!=_T("")&& doCheckThread())
					{
						chResult=sendSerialCommand(szCurCmd, false);
					}

					//DEBUGMSG(TRUE, (szCurCmd));

					//next command to be checked
					if(0==chResult)
						iCmnd++;

					//newVG 
					//if (::WaitForSingleObject(g_eventCOMCheckData, 2000) == WAIT_OBJECT_0) 
					//{
					//	if(m_bDoSerialCheckThread)
					//	{
					//		//next command to be checked
					//		if(0==chResult)
					//			iCmnd++;
					//	}
					//}
					//else
					//{
					//	//send again same command as previous, do nothing here

					//	//if(m_iErrorCountCheckThread>4 && m_bDoSerialCheckThread)
					//	//{
					//	//	/*m_iErrorCountCheckThread=0;

					//	//	eVentSilentState silentState=getModel()->getALARMHANDLER()->getAlarmSilentState();
					//	//	if(		silentState!=ASTATE_SYSTEM_SILENT
					//	//		&&	silentState!=ASTATE_SILENT)
					//	//	{
					//	//		CStringW szError=_T("");

					//	//		szError.Format(_T("ERROR checking COM values, VentMode: %d Cmnd: %s\r\n"), (int)mode ,szCurCmd);
					//	//		theApp.log->WriteLine(szError);
					//	//		DEBUGMSG(TRUE, (szError));

					//	//		getModel()->SetSERIALavailability(FALSE);
					//	//		EnterCriticalSection(&csMSGSend);
					//	//		MSGSend.RemoveAll();
					//	//		LeaveCriticalSection(&csMSGSend);

					//	//		SetErrorCode((int)mode, iCmnd);

					//	//		if(AfxGetApp())
					//	//			AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_COM_NOREINIT);
					//	//	}*/

					//	//	iCmnd=0;
					//	//}
					//	//else
					//	//{
					//	//	//m_iErrorCountCheckThread++;
					//	//	
					//	//}
					//}

					

					switch(mode)
					{
					case VM_IPPV:
						{
							if(iCmnd>=CMD_IPPV)
								iCmnd=0;
						}
						break;
					case VM_SIMVPSV:
						{
							if(iCmnd>=CMD_SIMVPSV)
								iCmnd=0;
						}
						break;
					case VM_PSV:
						{
							if(iCmnd>=CMD_PSV)
								iCmnd=0;
						}
						break;
					case VM_SIPPV:
					case VM_SIMV:
						{
							if(iCmnd>=CMD_TRIG)
								iCmnd=0;
						}
						break;
					case VM_CPAP:
						{
							if(iCmnd>=CMD_CPAP)
								iCmnd=0;
						}
						break;
					case VM_NCPAP:
						{
							if(iCmnd>=CMD_NCPAP)
								iCmnd=0;
						}
						break;
					case VM_DUOPAP:
						{
							if(iCmnd>=CMD_DUOPAP)
								iCmnd=0;
						}
						break;
					case VM_HFO:
						{
							if(iCmnd>=CMD_HF)
								iCmnd=0;
						}
						break;
					case VM_THERAPIE:
						{
							if(iCmnd>=CMD_THERAPY)
								iCmnd=0;
						}
						break;
					default:
						break;
					}
				}
			}
			break;
		}
	}while(doCheckThread() && m_comPort.m_hCommPort!=INVALID_HANDLE_VALUE);

	DEBUGMSG(TRUE, (TEXT("END CheckSerialData\r\n")));
	theApp.getLog()->WriteLine(_T("#THR:012"));

	return 0;
}



//void CInterfaceSerial::SetErrorCode(int mode, int cmd)
//{
//	int iERROR=0;
//
//	switch(mode)
//	{
//	case VM_IPPV:
//		{
//			if(cmd==0)
//			{
//				iERROR=ERRC_COM_CHECK_VENTMODE;
//			}
//			else if(cmd==1)
//			{
//				iERROR=ERRC_COM_CHECK_ITIME;
//			}
//			else if(cmd==2)
//			{
//				iERROR=ERRC_COM_CHECK_ETIME;
//			}
//			else if(cmd==3)
//			{
//				iERROR=ERRC_COM_CHECK_IFLOW;
//			}
//			else if(cmd==4)
//			{
//				iERROR=ERRC_COM_CHECK_EFLOW;
//			}
//			else if(cmd==5)
//			{
//				iERROR=ERRC_COM_CHECK_PINSP;
//			}
//			else if(cmd==6)
//			{
//				iERROR=ERRC_COM_CHECK_PEEP;
//			}
//			else if(cmd==7)
//			{
//				iERROR=ERRC_COM_CHECK_O2;
//			}
//			else if(cmd==8)
//			{
//				iERROR=ERRC_COM_CHECK_MODEOPTION;
//			}
//			
//		}
//		break;
//	case VM_SIMVPSV:
//	case VM_PSV:
//		{
//			if(cmd==0)
//			{
//				iERROR=ERRC_COM_CHECK_VENTMODE;
//			}
//			else if(cmd==1)
//			{
//				iERROR=ERRC_COM_CHECK_ITIME;
//			}
//			else if(cmd==2)
//			{
//				iERROR=ERRC_COM_CHECK_ETIME;
//			}
//			else if(cmd==3)
//			{
//				iERROR=ERRC_COM_CHECK_IFLOW;
//			}
//			else if(cmd==4)
//			{
//				iERROR=ERRC_COM_CHECK_EFLOW;
//			}
//			else if(cmd==5)
//			{
//				iERROR=ERRC_COM_CHECK_PINSP;
//			}
//			else if(cmd==6)
//			{
//				iERROR=ERRC_COM_CHECK_PEEP;
//			}
//			else if(cmd==7)
//			{
//				iERROR=ERRC_COM_CHECK_PPSV;
//			}
//			else if(cmd==8)
//			{
//				iERROR=ERRC_COM_CHECK_O2;
//			}
//			else if(cmd==9)
//			{
//				iERROR=ERRC_COM_CHECK_MODEOPTION;
//			}
//		}
//		break;
//	case VM_SIPPV:
//	case VM_SIMV:
//		{
//			if(cmd==0)
//			{
//				iERROR=ERRC_COM_CHECK_VENTMODE;
//			}
//			else if(cmd==1)
//			{
//				iERROR=ERRC_COM_CHECK_ITIME;
//			}
//			else if(cmd==2)
//			{
//				iERROR=ERRC_COM_CHECK_ETIME;
//			}
//			else if(cmd==3)
//			{
//				iERROR=ERRC_COM_CHECK_IFLOW;
//			}
//			else if(cmd==4)
//			{
//				iERROR=ERRC_COM_CHECK_EFLOW;
//			}
//			else if(cmd==5)
//			{
//				iERROR=ERRC_COM_CHECK_PINSP;
//			}
//			else if(cmd==6)
//			{
//				iERROR=ERRC_COM_CHECK_PEEP;
//			}
//			else if(cmd==7)
//			{
//				iERROR=ERRC_COM_CHECK_O2;
//			}
//			else if(cmd==8)
//			{
//				iERROR=ERRC_COM_CHECK_MODEOPTION;
//			}
//		}
//		break;
//	case VM_CPAP:
//		{
//			if(cmd==0)
//			{
//				iERROR=ERRC_COM_CHECK_VENTMODE;
//			}
//			else if(cmd==1)
//			{
//				iERROR=ERRC_COM_CHECK_EFLOW;
//			}
//			else if(cmd==2)
//			{
//				iERROR=ERRC_COM_CHECK_PINSP;
//			}
//			else if(cmd==3)
//			{
//				iERROR=ERRC_COM_CHECK_PEEP;
//			}
//			else if(cmd==4)
//			{
//				iERROR=ERRC_COM_CHECK_O2;
//			}
//			else if(cmd==5)
//			{
//				iERROR=ERRC_COM_CHECK_MODEOPTION;
//			}
//		}
//		break;
//	case VM_NCPAP:
//		{
//			if(cmd==0)
//			{
//				iERROR=ERRC_COM_CHECK_VENTMODE;
//			}
//			else if(cmd==1)
//			{
//				iERROR=ERRC_COM_CHECK_PINSP;
//			}
//			else if(cmd==2)
//			{
//				iERROR=ERRC_COM_CHECK_PEEP;
//			}
//			else if(cmd==3)
//			{
//				iERROR=ERRC_COM_CHECK_O2;
//			}
//			else if(cmd==4)
//			{
//				iERROR=ERRC_COM_CHECK_MODEOPTION;
//			}
//		}
//		break;
//	case VM_DUOPAP:
//		{
//			if(cmd==0)
//			{
//				iERROR=ERRC_COM_CHECK_VENTMODE;
//			}
//			else if(cmd==1)
//			{
//				iERROR=ERRC_COM_CHECK_ITIME;
//			}
//			else if(cmd==2)
//			{
//				iERROR=ERRC_COM_CHECK_ETIME;
//			}
//			else if(cmd==3)
//			{
//				iERROR=ERRC_COM_CHECK_PINSP;
//			}
//			else if(cmd==4)
//			{
//				iERROR=ERRC_COM_CHECK_PEEP;
//			}
//			else if(cmd==5)
//			{
//				iERROR=ERRC_COM_CHECK_O2;
//			}
//			else if(cmd==6)
//			{
//				iERROR=ERRC_COM_CHECK_MODEOPTION;
//			}
//		}
//		break;
//	case VM_HFO:
//		{
//			if(cmd==0)
//			{
//				iERROR=ERRC_COM_CHECK_VENTMODE;
//			}
//			else if(cmd==1)
//			{
//				iERROR=ERRC_COM_CHECK_HFFREQ;
//			}
//			else if(cmd==2)
//			{
//				iERROR=ERRC_COM_CHECK_HFPMEAN;
//			}
//			else if(cmd==3)
//			{
//				iERROR=ERRC_COM_CHECK_HFAMP;
//			}
//			else if(cmd==4)
//			{
//				iERROR=ERRC_COM_CHECK_O2;
//			}
//			else if(cmd==5)
//			{
//				iERROR=ERRC_COM_CHECK_MODEOPTION;
//			}
//			
//		}
//		break;
//	case VM_THERAPIE:
//		{
//			if(cmd==0)
//			{
//				iERROR=ERRC_COM_CHECK_VENTMODE;
//			}
//			else if(cmd==1)
//			{
//				iERROR=ERRC_COM_CHECK_EFLOW;
//			}
//			else if(cmd==2)
//			{
//				iERROR=ERRC_COM_CHECK_O2;
//			}
//			else if(cmd==3)
//			{
//				iERROR=ERRC_COM_CHECK_MODEOPTION;
//			}
//		}
//		break;
//	default:
//		break;
//	}
//
//	getModel()->getDATAHANDLER()->SetCOMErrorCode(iERROR);
//
//	
//}

//#####################################ReceiveThread########################################
//************************************
// Method:    CSerialReceiveThread
// FullName:  CSerialReceiveThread
// Access:    public static 
// Returns:   UINT
// Qualifier:
// Parameter: LPVOID pc
//
// 2015/06/19: checked for correct closing of thread
//************************************
static UINT CSerialReceiveThread( LPVOID pc )
{
	((CInterfaceSerial*)pc)->ReceiveSerialData();
	return TRUE;
}

//************************************
// Method:    ReceiveSerialData
// FullName:  CInterfaceSerial::ReceiveSerialData
// Access:    protected 
// Returns:   DWORD
// Qualifier:
// Parameter: void
//
// 2015/06/19: checked for correct closing of thread
//************************************
DWORD CInterfaceSerial::ReceiveSerialData(void) 
{
	CeSetThreadPriority(m_pcwtSerialReceiveThread->m_hThread,130);

	const int JUNK = 32;
	char temp[JUNK] = {0};

	DWORD dwCommModemStatus;

	if (m_comPort.m_hCommPort == INVALID_HANDLE_VALUE) 
	{
		// Handle the error. 
		theApp.getLog()->WriteLine(_T("#HFO:0012"));
		return 0;
	}

	// Set the event mask. 
	SetCommMask (m_comPort.m_hCommPort, EV_RXCHAR | EV_CTS | EV_DSR | EV_BREAK | EV_ERR);

	while (m_comPort.m_hCommPort != INVALID_HANDLE_VALUE  && doReceiveThread()) 
	{
		// Wait for an event to occur for the port.
		WaitCommEvent (m_comPort.m_hCommPort, &dwCommModemStatus, 0);

		if(doReceiveThread()==true)
		{
			SetCommMask (m_comPort.m_hCommPort, EV_RXCHAR | EV_CTS | EV_DSR | EV_BREAK | EV_ERR);

			if ((dwCommModemStatus & EV_RXCHAR) && doReceiveThread()) 
			{
				int size = m_comPort.ReadCommand(temp,sizeof(temp)-1);

				if (size > 0 && doReceiveThread())
				{
					if(ParseControllerCommand(CTlsBlob((BYTE*)temp,size))==true && doReceiveThread())
					{
						g_eventCOM.SetEvent();
					}
				}
			}
		}
		
		Sleep(0);
	}

	return 0;
}

//#####################################SendThread########################################
//************************************
// Method:    CSerialSendThread
// FullName:  CSerialSendThread
// Access:    public static 
// Returns:   UINT
// Qualifier:
// Parameter: LPVOID pc
//
// 2015/06/19: checked for correct closing of thread
//************************************
static UINT CSerialSendThread( LPVOID pc )
{
	((CInterfaceSerial*)pc)->SendSerialData();
	return TRUE;
}

//************************************
// Method:    SendSerialData
// FullName:  CInterfaceSerial::SendSerialData
// Access:    protected 
// Returns:   DWORD
// Qualifier:
// Parameter: void
//
// 2015/06/19: checked for correct closing of thread
//************************************
DWORD CInterfaceSerial::SendSerialData(void) 
{
	CeSetThreadPriority(m_pcwtSerialSendThread->m_hThread,130);

	CStringW szData=_T("");
	m_iErrorCountSendThread=0;
	UINT iListSize=0;
	CStringW szError=_T("");
	bool bPrint=true;

	do
	{
		EnterCriticalSection(&csMSGSend);
		iListSize=MSGSend.GetSize();
		if(iListSize>0)
		{
			szData=MSGSend.GetHead();
			MSGSend.RemoveHead();
		}
		LeaveCriticalSection(&csMSGSend);

		/*if(iListSize>100 && bPrint)
		{
			bPrint=false;
			DEBUGMSG(TRUE, (TEXT("send size > 100\r\n")));
			
			szError.Format(_T("!!!SendSize: %d!!!"),iListSize);
			theApp.getLog()->WriteLine(szError);
		}*/
		
		if(iListSize>0)
		{
			//TEST LOCK SERIAL
			//EnterCriticalSection(&csTestLock);
			//DEBUGMSG(TRUE, (TEXT("%s\r\n"),szData));
			m_comPort.Write(szData);
			//TEST LOCK SERIAL
			//LeaveCriticalSection(&csTestLock);

			DWORD dw = ::WaitForSingleObject(g_eventCOM, 500);

			////TEST LOCK SERIAL
			////EnterCriticalSection(&csTestLock);
			//m_comPort.Write(szData);
			////TEST LOCK SERIAL
			////LeaveCriticalSection(&csTestLock);

			if(doSendThread())
			{
				switch(dw)
				{
				case WAIT_OBJECT_0:
					m_iErrorCountSendThread=0;
					//EnterCriticalSection(&csMSGSend);//newVG
					//iListSize=MSGSend.GetSize();
					//if(iListSize>0)
					//{
					//	MSGSend.RemoveHead();
					//}
					//LeaveCriticalSection(&csMSGSend);
					if(getModel()->getDATAHANDLER()->getCOMErrorCode() & ERRC_COM_SEND_DATA)//newVG
					{
						getModel()->getDATAHANDLER()->checkCOMresetLastSendError(szData);
					}
					break;
				case WAIT_TIMEOUT:
				case WAIT_FAILED:
				case WAIT_ABANDONED:
				default:
					m_iErrorCountSendThread++;
					DEBUGMSG(TRUE, (TEXT("ERROR send data\r\n")));

					if(m_iErrorCountSendThread>3)
					{
						m_iErrorCountSendThread=0;

						/*CStringW szError=_T("");
						szError.Format(_T("#HFO:0013: %s"),szData);
						theApp.getLog()->WriteLine(szError);*/

						EnterCriticalSection(&csMSGSend);
						MSGSend.RemoveAll();
						LeaveCriticalSection(&csMSGSend);
						m_comPort.FlushBuffer();
						
						if(!getModel()->getDATAHANDLER()->IsTrendDataLoading() 
							&& !getModel()->getDATAHANDLER()->IsSavingTrendToUSBactiv())
						{//newVG
							/*eVentSilentState silentState=getModel()->getALARMHANDLER()->getAlarmSilentState();
							if(silentState!=ASTATE_SYSTEM_SILENT  && getModel()->isSERIALavailable()==TRUE)
							{
								CStringW szError=_T("");
								szError.Format(_T("#HFO:0013: %s"),szData);
								theApp.getLog()->WriteLine(szError);

								DEBUGMSG(TRUE, (TEXT("WM_SETALARM_IF_COM_REINIT 2\r\n")));
								if(AfxGetApp())
									AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_COM_REINIT);
							}*/
							
							eVentSilentState silentState=getModel()->getALARMHANDLER()->getAlarmSilentState();
							if(silentState!=ASTATE_SYSTEM_SILENT  && getModel()->isSERIALavailable()==TRUE)
							{
								CStringW szError=_T("");
								szError.Format(_T("#HFO:0013: %s"),szData);
								theApp.getLog()->WriteLine(szError);

								if(getModel()->getDATAHANDLER()->getCOMErrorCode()==ERRC_COM_SEND_DATA
									&&	true==getModel()->getDATAHANDLER()->checkCOMlastSendError(szData))//newVG
								{
									if(getModel()->getALARMHANDLER()->CanSetAlarm_IF_COM())
									{
										if(AfxGetApp())
											AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_COM);
									}
									
								}
								else
								{
									bool bAlarm=getModel()->getDATAHANDLER()->setCOMLastSendError(szData);//newVG

									DEBUGMSG(TRUE, (TEXT("WM_SETALARM_IF_COM_REINIT 2\r\n")));
									if(AfxGetApp() && bAlarm)
										AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_COM_REINIT);
								}
								
							}
						}
					}
					else
					{
						//resend
						EnterCriticalSection(&csMSGSend);
						MSGSend.AddTail(szData);
						LeaveCriticalSection(&csMSGSend);

						CStringW szError=_T("");
						szError.Format(_T("#COMSendAgain: %s"),szData);
						theApp.getLog()->WriteLine(szError);
						DEBUGMSG(TRUE, (szError));
					}
					Sleep(10);
					break;
				}
			}
		}
		Sleep(10);

	}while(doSendThread() && m_comPort.m_hCommPort!=INVALID_HANDLE_VALUE);

	DEBUGMSG(TRUE, (TEXT("END SendSerialData\r\n")));

	return 0;
}



// **************************************************************************
// 
// **************************************************************************
BYTE CInterfaceSerial::sendSerialCommand(CStringW data, bool bIgnorCOMstate) 
{
	BYTE chReturn=0;
#ifndef SIMULATION_NOSERIAL
	if(m_comPort.m_hCommPort!=INVALID_HANDLE_VALUE )
	{
		if((TRUE==getModel()->isSERIALavailable())
			|| bIgnorCOMstate)
		{
			//DEBUGMSG(TRUE, (TEXT("%s\r\n"),data));
			EnterCriticalSection(&csMSGSend);
			MSGSend.AddTail(data);
			LeaveCriticalSection(&csMSGSend);
		}
		else
		{
			chReturn=1;
		}
	}
	else if(TRUE==getModel()->isSERIALavailable())
	{
		getModel()->setSERIALavailable(FALSE);
		EnterCriticalSection(&csMSGSend);
		MSGSend.RemoveAll();
		LeaveCriticalSection(&csMSGSend);

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_COM_REINIT);
	}
	else
	{
		chReturn=2;
	}
#endif
	
	Sleep(0);
	



	//UINT32 checksum;
	///* calc checksum for msg */
	//checksum = 0;
	//for (int i=0; i < total_length; i++)
	//	checksum = checksum + bfr_ptr[i];

	///* Append checksum to the end of the message. Do not update the
	//* "length" field of the message header.
	//*/
	//bfr_ptr[total_length] = (BYTE) ((checksum >> 24) & 0xff);
	//bfr_ptr[total_length+1] = (BYTE) ((checksum >> 16) & 0xff);
	//bfr_ptr[total_length+2] = (BYTE) ((checksum >> 8) & 0xff);
	//bfr_ptr[total_length+3] = (BYTE) ((checksum >> 0) & 0xff);



	//check
	///* calc checksum for msg */
	//checksum_calc = 0;
	//total_length = header_size + length;
	//for (i=0; i < total_length; i++)
	//	checksum_calc = checksum_calc + ((UINT32) bfr_ptr[i]);

	//checksum_recv = (UINT32) 0;
	//checksum_recv = (UINT32) (checksum_recv | ((UINT32) bfr_ptr[total_length] << 24));
	//checksum_recv = (UINT32) (checksum_recv | ((UINT32) bfr_ptr[total_length+1] << 16));
	//checksum_recv = (UINT32) (checksum_recv | ((UINT32) bfr_ptr[total_length+2] << 8));
	//checksum_recv = (UINT32) (checksum_recv | ((UINT32) bfr_ptr[total_length+3] << 0));

	///* Poll for user interrupt */
	//SIGINT_POLL
	//	/* Compare Checksums */
	//	if (checksum_calc != checksum_recv) {
	//		(void) reset_comm_serial ((INT32) -1, (INT32) -1);
	//		send_nack(port_base);
	//		goto	again;	/* retry */
	//	}



	/* Pseudo-C */ 

	/*int i; 
	MeinTyp mein_element; 
	unsigned char *ptr = (unsigned char*)mein_element; 
	unsigned char xorChecksum=0; 

	for( i=0; i < sizeof(MeinTyp); i++ ) 
	xorChecksum ^= ptr[i]; 

	Ersetze das ^= durch += und du hast eine Additionschecksumme*/






	/*char t[]="hallo";

	for(int i=0;i<sizeof(t)-1;i++)
	{
	chsum=chsum^t[i];
	}
	int in=(int)chsum;
	CStringW str; 
	str.Format("%d",in);*/

	//Wenn ich dich richtig verstanden habe, willst du die Zahl nur als Hex ausgeben.anstatt:

	//str.Format("%d",in);

	//einfach

	//	str.Format("%x", in); // Kleine Hex-Buchstaben

	//oder 

	//	str.Format("%x", in); // Grosse Hex-Buchstaben




	//m_comPort.Write(data);

	return chReturn;
}




// **************************************************************************
// 
// **************************************************************************
void CInterfaceSerial::Send_START_MAN_BREATH()
{
	TCHAR psz[MAX_PATH];
	wsprintf(psz,_T("n"));//Sonderkommando ohne "q"
	if(0==sendSerialCommand(psz))
		theApp.getLog()->WriteLine(_T("START_MAN_BREATH"));
	//DEBUGMSG(TRUE, (TEXT("Send_START_MAN_BREATH\r\n")));
}
// **************************************************************************
// 
// **************************************************************************
void CInterfaceSerial::Send_STOP_MAN_BREATH()
{
	TCHAR psz[MAX_PATH];
	wsprintf(psz,_T("p"));//Sonderkommando ohne "q"
	if(0==sendSerialCommand(psz))
		theApp.getLog()->WriteLine(_T("STOP_MAN_BREATH"));
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceSerial::Send_HARDWARE_CONFIG(WORD wHardwareConfig)
{
	TCHAR psz[MAX_PATH];
	wsprintf(psz,_T("k%dq"),(int)wHardwareConfig);
	if(0==sendSerialCommand(psz))
		theApp.getLog()->WriteLine(psz);
}


// **************************************************************************
// 
// **************************************************************************
void CInterfaceSerial::Send_MODE_OPTION1(WORD wMode)
{
	TCHAR psz[MAX_PATH];
	wsprintf(psz,_T("r%dq"),wMode);
	if(0==sendSerialCommand(psz))
		theApp.getLog()->WriteLine(psz);
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceSerial::Send_VENT_MODE(eVentMode mode)
{
	TCHAR psz[MAX_PATH];
	wsprintf(psz,_T("o%dq"),(int)mode);
	if(0==sendSerialCommand(psz))
		theApp.getLog()->WriteLine(psz);

}



// **************************************************************************
// 
// **************************************************************************
void CInterfaceSerial::Send_PARAVAL_PINSP(int val)//newVG
{
	TCHAR psz[MAX_PATH];
	wsprintf(psz,_T("t%dq"),val);
	if(0==sendSerialCommand(psz))
		theApp.getLog()->WriteLine(psz);

	//if(val%10!=0)//check test
	//{
	//	int iStop=0;
	//}
	//DEBUGMSG(TRUE, (TEXT("Send_PARAVAL_PINSP %d\r\n"),val));
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceSerial::Send_PARAVAL_PMAXVG(int val)//newVG
{
	TCHAR psz[MAX_PATH];
	wsprintf(psz,_T("<%dq"),val);
	if(0==sendSerialCommand(psz))
		theApp.getLog()->WriteLine(psz);

	//DEBUGMSG(TRUE, (TEXT("Send_PARAVAL_PMAXVG %d\r\n"),val));
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceSerial::Send_PARAVAL_P_PSV(int val)
{
	TCHAR psz[MAX_PATH];
	wsprintf(psz,_T("c%dq"),val);
	if(0==sendSerialCommand(psz))
		theApp.getLog()->WriteLine(psz);

	//DEBUGMSG(TRUE, (TEXT("Send_PARAVAL_P_PSV %d\r\n"),val));
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceSerial::Send_PARAVAL_PEEP(int val)
{
	//DEBUGMSG(TRUE, (TEXT("Send_PARAVAL_PEEP %d\r\n"),val));
	TCHAR psz[MAX_PATH];
	wsprintf(psz,_T("z%dq"),val);
	if(0==sendSerialCommand(psz))
		theApp.getLog()->WriteLine(psz);
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceSerial::Send_PARAVAL_OXY_RATIO(int val)
{
	//DEBUGMSG(TRUE, (TEXT("Send_PARAVAL_OXY_RATIO %d\r\n"),val));
	TCHAR psz[MAX_PATH];
	wsprintf(psz,_T("d%dq"),val);
	if(0==sendSerialCommand(psz))
		theApp.getLog()->WriteLine(psz);
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceSerial::Send_PARAVAL_INSP_FLOW(int val)
{
	TCHAR psz[MAX_PATH];
	wsprintf(psz,_T("u%dq"),val);
	if(0==sendSerialCommand(psz))
		theApp.getLog()->WriteLine(psz);
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceSerial::Send_PARAVAL_RISETIME(int val)
{
	TCHAR psz[MAX_PATH];

	wsprintf(psz,_T("[%dq"),val);
	if(0==sendSerialCommand(psz))
		theApp.getLog()->WriteLine(psz);
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceSerial::Send_PARAVAL_EXH_FLOW(int val)
{
	TCHAR psz[MAX_PATH];
	wsprintf(psz,_T("a%dq"),val);
	if(0==sendSerialCommand(psz))
		theApp.getLog()->WriteLine(psz);

	DEBUGMSG(TRUE, (TEXT("Send_PARAVAL_EXH_FLOW %d\r\n"),val));
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceSerial::Send_PARAVAL_INSP_TIME(int val)
{
	TCHAR psz[MAX_PATH];
	wsprintf(psz,_T("w%dq"),val);
	if(0==sendSerialCommand(psz))
		theApp.getLog()->WriteLine(psz);
}


// **************************************************************************
// 
// **************************************************************************
void CInterfaceSerial::Send_PARAVAL_EXH_TIME(int val)
{
	TCHAR psz[MAX_PATH];
	wsprintf(psz,_T("e%dq"),val);
	if(0==sendSerialCommand(psz))
		theApp.getLog()->WriteLine(psz);
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceSerial::Send_PARAVAL_HF_AMPL(int val)
{
	//AmplitudeCorrectionFactor
	/*double iAmpCorFactor = getModel()->getDATAHANDLER()->getAmpCorFactor(getModel()->getDATAHANDLER()->PARADATA()->GetHFFreqPara());
	if(iAmpCorFactor==0)
		iAmpCorFactor=1;
	val=(SHORT)(double)val/iAmpCorFactor;*/

	TCHAR psz[MAX_PATH];
	wsprintf(psz,_T("m%dq"),val);
	if(0==sendSerialCommand(psz))
		theApp.getLog()->WriteLine(psz);
	DEBUGMSG(TRUE, (TEXT("Send_PARAVAL_HF_AMPL %d\r\n"),val));
}
// **************************************************************************
// 
// **************************************************************************
void CInterfaceSerial::Send_PARAVAL_HF_FREQ(int val)
{
	TCHAR psz[MAX_PATH];
	wsprintf(psz,_T("f%dq"),val);
	if(0==sendSerialCommand(psz))
		theApp.getLog()->WriteLine(psz);
	DEBUGMSG(TRUE, (TEXT("Send_PARAVAL_HF_FREQ %d\r\n"),val));
}
// **************************************************************************
// 
// **************************************************************************
void CInterfaceSerial::Send_PARAVAL_HF_PMITT(int val)
{
	TCHAR psz[MAX_PATH];
	wsprintf(psz,_T("j%dq"),val);
	if(0==sendSerialCommand(psz))
		theApp.getLog()->WriteLine(psz);
	DEBUGMSG(TRUE, (TEXT("Send_PARAVAL_HF_PMITT %d\r\n"),val));
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceSerial::Send_PARAVAL_HF_PMEANREC(int val)
{
	TCHAR psz[MAX_PATH];
	wsprintf(psz,_T("!%dq"),val);
	if(0==sendSerialCommand(psz))
		theApp.getLog()->WriteLine(psz);
	DEBUGMSG(TRUE, (TEXT("Send_PARAVAL_HF_PMEANREC %d\r\n"),val));
}
// **************************************************************************
// 
// **************************************************************************
void CInterfaceSerial::Send_PARAVAL_HF_FREQ_REC(int val)
{
	int iETIME=getModel()->getDATAHANDLER()->GetHF_ETIME_REC(val);

	Send_PARAVAL_EXH_TIME(iETIME/1000);

	DEBUGMSG(TRUE, (TEXT("Send_PARAVAL_HF_FREQ_REC %d\r\n"),val));
}
// **************************************************************************
// 
// **************************************************************************
void CInterfaceSerial::Send_PARAVAL_HF_ITIME_REC(int val)
{
	Send_PARAVAL_INSP_TIME(val);

	DEBUGMSG(TRUE, (TEXT("Send_PARAVAL_HF_ITIME_REC %d\r\n"),val));
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceSerial::Send_EXP_VALVE(int val)
{
	TCHAR psz[MAX_PATH];
	wsprintf(psz,_T(":%dq"),val);
	if(0==sendSerialCommand(psz))
		theApp.getLog()->WriteLine(psz);
}
// **************************************************************************
// 
// **************************************************************************
void CInterfaceSerial::Send_OPEN_VALVE(int val)
{
	TCHAR psz[MAX_PATH];
	wsprintf(psz,_T(";%dq"),val);
	if(0==sendSerialCommand(psz))
		theApp.getLog()->WriteLine(psz);
}

//// **************************************************************************
//// 
//// **************************************************************************
//void CInterfaceSerial::Send_SET_FG_BIT(int val) //newVG
//{
//	TCHAR psz[MAX_PATH];
//	wsprintf(psz,_T(">%dq"),val);
//	if(0==sendSerialCommand(psz))
//		theApp.getLog()->WriteLine(psz);
//}
//// **************************************************************************
//// 
//// **************************************************************************
//void CInterfaceSerial::Send_CLEAR_FG_BIT(int val) //newVG
//{
//	TCHAR psz[MAX_PATH];
//	wsprintf(psz,_T("?%dq"),val);
//	if(0==sendSerialCommand(psz))
//		theApp.getLog()->WriteLine(psz);
//}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceSerial::Send_CAL_PRESS_PROX(int val)
{
	TCHAR psz[MAX_PATH];
	wsprintf(psz,_T("g%dq"),val);
	if(0==sendSerialCommand(psz))
		theApp.getLog()->WriteLine(psz);
}


// **************************************************************************
// 
// **************************************************************************
void CInterfaceSerial::Send_OXYGEN_CORR(int val)
{
	TCHAR psz[MAX_PATH];
	wsprintf(psz,_T("i%dq"),val);

	sendSerialCommand(psz);
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceSerial::Send_AUTOFLOW_CORR(BYTE val)
{
	TCHAR psz[MAX_PATH];
	wsprintf(psz,_T("S%dq"),val);

	sendSerialCommand(psz);
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceSerial::Send_VOLUME_CORR(BYTE val)
{
	TCHAR psz[MAX_PATH];
	wsprintf(psz,_T("s%dq"),val);

	sendSerialCommand(psz);
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceSerial::Send_PARAVAL_APNOE_TIME(int val)
{
	TCHAR psz[MAX_PATH];
	wsprintf(psz,_T("v%dq"),val);
	if(0==sendSerialCommand(psz))
		theApp.getLog()->WriteLine(psz);
	//DEBUGMSG(TRUE, (TEXT("Send_PARAVAL_APNOE_TIME %d\r\n"),val));
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceSerial::Send_MODE_OPTION2(WORD wMode)
{
	TCHAR psz[MAX_PATH];
	wsprintf(psz,_T("(%dq"),wMode);
	if(0==sendSerialCommand(psz))
		theApp.getLog()->WriteLine(psz);
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceSerial::Send_FOTconv_AMP_FREQ(int val)
{

	//FOT02
	/*bits 0, 1 für Amplitude / AUS:

	0bxx00    AUS
	0bxx01    2,5 mb
	obxx10    5 mb
	0bxx11    7,5 mb

	bits 2, 3 für Frequenz:

	0b00xx    5 Hz
	0b01xx    10 Hz
	0b10xx    15 Hz*/


	TCHAR psz[MAX_PATH];
	wsprintf(psz,_T("l%dq"),val);
	if(0==sendSerialCommand(psz))
		theApp.getLog()->WriteLine(psz);

	//DEBUGMSG(TRUE, (TEXT("Send_FOT_AMP_FREQ %d\r\n"),val));
}

// **************************************************************************
// 
// **************************************************************************
BOOL CInterfaceSerial::GetM_StatusByte()
{
	BOOL bResult=FALSE;

	BYTE bResultSend=sendSerialCommand(_T("Lq"));
	if(0==bResultSend)
	{

		DWORD dw = ::WaitForSingleObject(g_evCOM_STATUS, 500);

		switch(dw)
		{
		case WAIT_OBJECT_0:
			{
				//resetCOMerror(COM_STATUS);
				bResult=TRUE;
			}
			break;
		case WAIT_TIMEOUT:
		case WAIT_FAILED:
		case WAIT_ABANDONED:
		default:
			//setCOMerror(COM_STATUS);
			break;
		}
	}

	return bResult;
}

// **************************************************************************
// 
// **************************************************************************
int CInterfaceSerial::GetM_CHECKSUM(bool bIgnorCOMstate)
{
	int iResult=0;

	BYTE bResultSend=sendSerialCommand(_T("Nq"),bIgnorCOMstate);

	if(0==bResultSend)
	{
		DWORD dw = ::WaitForSingleObject(g_evCOM_CHECKSUM, 500);

		switch(dw)
		{
		case WAIT_OBJECT_0:
			{
				//iResult=resetCOMerror(COM_CHECKSUM);
				iResult=getModel()->getDATAHANDLER()->GetControllerPIC_Checksum();
			}
			break;
		case WAIT_TIMEOUT:
		case WAIT_FAILED:
		case WAIT_ABANDONED:
		default:
			/*if(!bIgnorCOMstate)
				setCOMerror(COM_CHECKSUM);*/
			break;
		}
	}

	return iResult;
}

// **************************************************************************
// 
// **************************************************************************
int CInterfaceSerial::GetM_INSP_FLOW()
{
	int iResult=0;

	BYTE bResultSend=sendSerialCommand(_T("Xq"));

	if(0==bResultSend)
	{
		DWORD dw = ::WaitForSingleObject(g_evCOM_M_INSP_FLOW, 500);

		switch(dw)
		{
		case WAIT_OBJECT_0:
			{
				//iResult=resetCOMerror(COM_M_INSP_FLOW);
				iResult=m_iM_INSP_FLOW;
			}
			break;
		case WAIT_TIMEOUT:
		case WAIT_FAILED:
		case WAIT_ABANDONED:
		default:
			{
				//setCOMerror(COM_M_INSP_FLOW);
			}
			break;
		}
	}

	return iResult;
}
// **************************************************************************
// 
// **************************************************************************
int CInterfaceSerial::GetM_EXP_FLOW()
{
	int iResult=0;

	BYTE bResultSend=sendSerialCommand(_T("xq"));

	if(0==bResultSend)
	{
		DWORD dw = ::WaitForSingleObject(g_evCOM_M_EXP_FLOW, 500);

		switch(dw)
		{
		case WAIT_OBJECT_0:
			{
				//iResult=resetCOMerror(COM_M_EXP_FLOW);
				iResult=m_iM_EXP_FLOW;
			}
			break;
		case WAIT_TIMEOUT:
		case WAIT_FAILED:
		case WAIT_ABANDONED:
		default:
			{
				//setCOMerror(COM_M_EXP_FLOW);
			}
			break;
		}
	}

	return iResult;
}

// **************************************************************************
// 
// **************************************************************************
int CInterfaceSerial::GetM_DEMAND_FLOW()
{
	int iResult=0;

	BYTE bResultSend=sendSerialCommand(_T("bq"));

	if(0==bResultSend)
	{
		DWORD dw = ::WaitForSingleObject(g_evCOM_M_DEMAND_FLOW, 500);

		switch(dw)
		{
		case WAIT_OBJECT_0:
			{
				//iResult=resetCOMerror(COM_M_DEMAND_FLOW);
				iResult=m_iM_DEMAND_FLOW;
			}
			break;
		case WAIT_TIMEOUT:
		case WAIT_FAILED:
		case WAIT_ABANDONED:
		default:
			{
				//setCOMerror(COM_M_DEMAND_FLOW);
			}
			break;
		}
	}

	return iResult;
}

// **************************************************************************
// 
// **************************************************************************
int CInterfaceSerial::GetM_CAL_PRESS_SCALE()
{
	int iResult=0;

	BYTE bResultSend=sendSerialCommand(_T("Bq"));

	if(0==bResultSend)
	{
		DWORD dw = ::WaitForSingleObject(g_evCOM_CAL_PRESS_SCALE, 500);

		switch(dw)
		{
		case WAIT_OBJECT_0:
			{
				//iResult=resetCOMerror(COM_CAL_PRESS_SCALE);
				iResult=m_iCALPRESSSCALE;
			}
			break;
		case WAIT_TIMEOUT:
		case WAIT_FAILED:
		case WAIT_ABANDONED:
		default:
			{
				//setCOMerror(COM_CAL_PRESS_SCALE);
			}
			break;
		}
	}

	return iResult;
}
// **************************************************************************
// 
// **************************************************************************
int CInterfaceSerial::GetM_CAL_PRESS_OFFSET()
{
	int iResult=0;

	BYTE bResultSend=sendSerialCommand(_T("Gq"));

	if(0==bResultSend)
	{
		DWORD dw = ::WaitForSingleObject(g_evCOM_CAL_PRESS_OFFSET, 500);

		switch(dw)
		{
		case WAIT_OBJECT_0:
			{
				//iResult=resetCOMerror(COM_CAL_PRESS_OFFSET);
				iResult=m_iCALPRESSOFFSET;
			}
			break;
		case WAIT_TIMEOUT:
		case WAIT_FAILED:
		case WAIT_ABANDONED:
		default:
			//setCOMerror(COM_CAL_PRESS_OFFSET);
			break;
		}
	}

	return iResult;
}

// **************************************************************************
// 
// **************************************************************************
CStringW CInterfaceSerial::GetM_VERSION()
{
	CStringW szResult=_T("0.0.0");

	BYTE bResultSend=sendSerialCommand(_T("Yq"),false);

	if(0==bResultSend)
	{
		DWORD dw = ::WaitForSingleObject(g_evCOM_VERSION, 500);

		switch(dw)
		{
		case WAIT_OBJECT_0:
			//resetCOMerror(COM_VERSION);
			szResult=getModel()->getDATAHANDLER()->GetControllerPIC_Version();
			break;
		case WAIT_TIMEOUT:
		case WAIT_FAILED:
		case WAIT_ABANDONED:
		default:
			//setCOMerror(COM_VERSION);
			break;
		}
	}

	return szResult;
}

// **************************************************************************
// 
// **************************************************************************
CStringW CInterfaceSerial::GetHFO_VERSION()
{
	CStringW szResult=_T("0.0.0");

	if(getModel()->getI2C()!=NULL)
	{
		getModel()->getI2C()->InitMCP_HfoPIC();
		FlushBuffer();
	}

	BYTE bResultSend=sendSerialCommand(_T("Yq"),false);

	if(0==bResultSend)
	{
		DWORD dw = ::WaitForSingleObject(g_evCOM_VERSION, 500);

		switch(dw)
		{
		case WAIT_OBJECT_0:
			//resetCOMerror(COM_VERSION);
			szResult=getModel()->getDATAHANDLER()->GetHFOPIC_Version();
			break;
		case WAIT_TIMEOUT:
		case WAIT_FAILED:
		case WAIT_ABANDONED:
		default:
			//setCOMerror(COM_VERSION);
			break;
		}
	}

	if(getModel()->getI2C()!=NULL)
	{
		getModel()->getI2C()->InitMCP_ControlerPIC();
		FlushBuffer();
	}

	return szResult;
}

// **************************************************************************
// 
// **************************************************************************
CStringW CInterfaceSerial::GetBLENDER_VERSION()
{
	CStringW szResult=_T("0.0");

	BYTE bResultSend=sendSerialCommand(_T("Hq"));

	if(0==bResultSend)
	{
		DWORD dw = ::WaitForSingleObject(g_evBLENDER_VERSION, 500);

		switch(dw)
		{
		case WAIT_OBJECT_0:
			szResult=getModel()->getDATAHANDLER()->GetBlenderPIC_Version();
			break;
		case WAIT_TIMEOUT:
		case WAIT_FAILED:
		case WAIT_ABANDONED:
		default:


			break;
		}
	}


	return szResult;
}

// **************************************************************************
// 
// **************************************************************************
BOOL CInterfaceSerial::GetBLENDER_StatusByte()
{
	BOOL bResult=FALSE;

	BYTE bResultSend=sendSerialCommand(_T("Iq"));

	if(0==bResultSend)
	{
		DWORD dw = ::WaitForSingleObject(g_evBLENDER_STATUS, 500);

		switch(dw)
		{
		case WAIT_OBJECT_0:
			//m_iError_evCOM_STATUS=0;
			bResult=TRUE;
			break;
		case WAIT_TIMEOUT:
		case WAIT_FAILED:
		case WAIT_ABANDONED:
		default:
			break;
		}
	}

	return bResult;
}
// **************************************************************************
// 
// **************************************************************************
int CInterfaceSerial::GetHFO_CHECKSUM()
{
	int iResult=0;

	if(getModel()->getI2C()!=NULL)
	{
		getModel()->getI2C()->InitMCP_HfoPIC();
		FlushBuffer();
	}

	BYTE bResultSend=sendSerialCommand(_T("Nq"));

	if(0==bResultSend)
	{
		DWORD dw = ::WaitForSingleObject(g_evCOM_CHECKSUM, 500);

		switch(dw)
		{
		case WAIT_OBJECT_0:
			{
				//iResult=resetCOMerror(COM_CHECKSUM);
				iResult=getModel()->getDATAHANDLER()->GetHFOPIC_Checksum();
			}
			break;
		case WAIT_TIMEOUT:
		case WAIT_FAILED:
		case WAIT_ABANDONED:
		default:
			/*if(!bIgnorCOMstate)
				setCOMerror(COM_CHECKSUM);*/
			break;
		}
	}

	
	if(getModel()->getI2C()!=NULL)
	{
		getModel()->getI2C()->InitMCP_ControlerPIC();
		FlushBuffer();
	}


	return iResult;
}
//int CInterfaceSerial::resetCOMerror(eCOMerror error)
//{
//	int iResult=0;
//	switch(error)
//	{
//	case COM_M_EXP_FLOW:
//		{
//			m_iError_evCOM_M_EXP_FLOW=0;
//			iResult=m_iM_EXP_FLOW;
//		}
//		break;
//	case COM_VERSION:
//		{
//			m_iError_evCOM_VERSION=0;
//		}
//		break;
//	case COM_STATUS:
//		{
//			m_iError_evCOM_STATUS=0;
//			iResult=1;
//		}
//		break;
//	case COM_CHECKSUM:
//		{
//			m_iError_evCOM_CHECKSUM=0;
//			iResult=getModel()->getDATAHANDLER()->GetControllerPIC_Checksum();
//		}
//		break;
//	case COM_M_INSP_FLOW:
//		{
//			m_iError_evCOM_M_INSP_FLOW=0;
//			iResult=m_iM_INSP_FLOW;
//		}
//		break;
//	case COM_M_DEMAND_FLOW:
//		{
//			m_iError_evCOM_M_DEMAND_FLOW=0;
//			iResult=m_iM_DEMAND_FLOW;
//		}
//		break;
//	case COM_CAL_PRESS_SCALE:
//		{
//			m_iError_evCOM_CAL_PRESS_SCALE=0;
//			iResult=m_iCALPRESSSCALE;
//		}
//		break;
//	case COM_CAL_PRESS_OFFSET:
//		{
//			m_iError_evCOM_CAL_PRESS_OFFSET=0;
//			iResult=m_iCALPRESSOFFSET;
//		}
//		break;
//	default:
//		{
//		}
//		break;
//	}
//	return iResult;
//}
//void CInterfaceSerial::setCOMerror(eCOMerror error)
//{
//	if(getModel()->isSERIALavailable()==FALSE)
//	{
//		return;
//	}
//
//	bool bAlarm=false;
//	switch(error)
//	{
//	case COM_M_EXP_FLOW:
//		{
//			m_iError_evCOM_M_EXP_FLOW++;
//
//			if(m_iError_evCOM_M_EXP_FLOW>3)
//			{
//				m_iError_evCOM_M_EXP_FLOW=0;
//				theApp.getLog()->WriteLine(_T("#HFO:0004"));
//			}
//		}
//		break;
//	case COM_VERSION:
//		{
//			m_iError_evCOM_VERSION++;
//
//			if(m_iError_evCOM_VERSION>3)
//			{
//				m_iError_evCOM_VERSION=0;
//				theApp.getLog()->WriteLine(_T("#HFO:0008"));
//			}
//		}
//		break;
//	case COM_STATUS:
//		{
//			m_iError_evCOM_STATUS++;
//
//			if(m_iError_evCOM_STATUS>3)
//			{
//				m_iError_evCOM_STATUS=0;
//				theApp.getLog()->WriteLine(_T("#HFO:0001"));
//				bAlarm=true;
//			}
//		}
//		break;
//	case COM_CHECKSUM:
//		{
//			m_iError_evCOM_CHECKSUM++;
//
//			if(m_iError_evCOM_CHECKSUM>3)
//			{
//				m_iError_evCOM_CHECKSUM=0;
//				theApp.getLog()->WriteLine(_T("#HFO:0002"));
//				bAlarm=true;
//			}
//		}
//		break;
//	case COM_M_INSP_FLOW:
//		{
//			m_iError_evCOM_M_INSP_FLOW++;
//
//			if(m_iError_evCOM_M_INSP_FLOW>3)
//			{
//				m_iError_evCOM_M_INSP_FLOW=0;
//				theApp.getLog()->WriteLine(_T("#HFO:0003"));
//			}
//		}
//		break;
//	case COM_M_DEMAND_FLOW:
//		{
//			m_iError_evCOM_M_DEMAND_FLOW++;
//
//			if(m_iError_evCOM_M_DEMAND_FLOW>3)
//			{
//				m_iError_evCOM_M_DEMAND_FLOW=0;
//				theApp.getLog()->WriteLine(_T("#HFO:0005"));
//
//			}
//		}
//		break;
//	case COM_CAL_PRESS_SCALE:
//		{
//			m_iError_evCOM_CAL_PRESS_SCALE++;
//
//			if(m_iError_evCOM_CAL_PRESS_SCALE>3)
//			{
//				m_iError_evCOM_CAL_PRESS_SCALE=0;
//				theApp.getLog()->WriteLine(_T("#HFO:0006"));
//			}
//		}
//		break;
//	case COM_CAL_PRESS_OFFSET:
//		{
//			m_iError_evCOM_CAL_PRESS_OFFSET++;
//
//			if(m_iError_evCOM_CAL_PRESS_OFFSET>3)
//			{
//				m_iError_evCOM_CAL_PRESS_OFFSET=0;
//				theApp.getLog()->WriteLine(_T("#HFO:0007"));
//			}
//		}
//		break;
//	default:
//		{
//		}
//		break;
//	}
//
//	if(bAlarm)
//	{
//
//		eVentSilentState silentState=getModel()->getALARMHANDLER()->getAlarmSilentState();
//		if(		silentState!=ASTATE_SYSTEM_SILENT
//			&&	silentState!=ASTATE_SILENT)
//		{
//			DEBUGMSG(TRUE, (TEXT("WM_SETALARM_IF_COM_REINIT 4\r\n")));
//			if(AfxGetApp())
//				AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_COM_REINIT);
//			/*getModel()->SetSERIALavailability(FALSE);
//			EnterCriticalSection(&csMSGSend);
//			MSGSend.RemoveAll();
//			LeaveCriticalSection(&csMSGSend);
//
//			if(AfxGetApp())
//				AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_COM_NOREINIT);xxxxxxxxxxxxxxxxxxx*/
//		}
//	}
//}

// **************************************************************************
// 
// **************************************************************************
bool CInterfaceSerial::ParseControllerCommand(CTlsBlob bl)
{
	bool bResult = true;
	unsigned char* psz = bl.GetBuffer();

	if(psz[0] == 'I')
	{
		CheckBlenderStatusBits(bl);

		g_evBLENDER_STATUS.SetEvent();
	}
	else if(psz[0] == 'H')
	{
		CStringW sz= bl.GetString();
		sz = sz.Mid(1,3);
		getModel()->getDATAHANDLER()->SetBlenderPIC_Version(sz);

		g_evBLENDER_VERSION.SetEvent();
	}
	else if(psz[0] == 'Y')
	{
		CStringW sz= bl.GetString();
		sz = sz.Right(sz.GetLength()-1);
		if(getModel()->getI2C()->GetMCPstate()==MCP_CONTROLLER)
			getModel()->getDATAHANDLER()->SetControllerPIC_Version(sz);
		else if(getModel()->getI2C()->GetMCPstate()==MCP_HFO)
			getModel()->getDATAHANDLER()->SetHFOPIC_Version(sz);

		g_evCOM_VERSION.SetEvent();
	}
	else if(psz[0] == 'B')
	{
		m_iCALPRESSSCALE=GetCOMValue(bl);
		getModel()->getDATAHANDLER()->SetControllerPIC_CALPRESSSCALE(m_iCALPRESSSCALE);

		m_bCALPRESSSCALE=true;

		g_evCOM_CAL_PRESS_SCALE.SetEvent();
	}
	else if(psz[0] == 'G')
	{
		m_iCALPRESSOFFSET= GetCOMValue(bl);
		getModel()->getDATAHANDLER()->SetControllerPIC_CALPRESSOFFSET(m_iCALPRESSOFFSET);

		m_bCALPRESSOFFSET=true;

		g_evCOM_CAL_PRESS_OFFSET.SetEvent();
	}
	else if(psz[0] == 'N')
	{

		int iT= GetCOMValue(bl);

		if(getModel()->getI2C()->GetMCPstate()==MCP_CONTROLLER)
			getModel()->getDATAHANDLER()->SetControllerPIC_Checksum(iT);
		else if(getModel()->getI2C()->GetMCPstate()==MCP_HFO)
			getModel()->getDATAHANDLER()->SetHFOPIC_Checksum(iT);

		//DEBUGMSG(TRUE, (TEXT("Set g_evCOM_CHECKSUM\r\n")));
		g_evCOM_CHECKSUM.SetEvent();
	}
	else if(psz[0] == 'L')
	{
		CheckStatusBits(bl);

		g_evCOM_STATUS.SetEvent();
	}
	else if(psz[0] == 'X')
	{
		m_iM_INSP_FLOW=GetCOMValue(bl);
		if(m_iM_INSP_FLOW<0)
			m_iM_INSP_FLOW=0;
		getModel()->getDATAHANDLER()->SetInspFlowData(m_iM_INSP_FLOW);

		g_evCOM_M_INSP_FLOW.SetEvent();
	}
	else if(psz[0] == 'x')
	{
		m_iM_EXP_FLOW=GetCOMValue(bl);
		if(m_iM_EXP_FLOW<0)
			m_iM_EXP_FLOW=0;
		getModel()->getDATAHANDLER()->SetExpFlowData(m_iM_EXP_FLOW);

		g_evCOM_M_EXP_FLOW.SetEvent();
	}
	else if(psz[0] == 'b')
	{
		m_iM_DEMAND_FLOW=GetCOMValue(bl);
		if(m_iM_DEMAND_FLOW<0)
			m_iM_DEMAND_FLOW=0;
		getModel()->getDATAHANDLER()->SetDemandFlowData(m_iM_DEMAND_FLOW);

		g_evCOM_M_DEMAND_FLOW.SetEvent();
	}
	else if(psz[0] == 'a'/* && iLen==1*/)
	{
	}
	else if(psz[0] == 'O')
	{
		if(false==getModel()->getCONFIG()->CurModeIsPresetMode())
		{
			CStringW sToken = bl.GetString();
			sToken = sToken.Right(sToken.GetLength()-1);
			sToken = CTlsString::RemoveLeadingZeroes(sToken);

			int iTemp = _wtoi(sToken);

			if(getModel()->getCONFIG()->GetCurMode()==iTemp)
			{
				//newVG
				//m_iErrorCountCheckThread=0;
				//g_eventCOMCheckData.SetEvent();
				
				if(		getModel()->getDATAHANDLER()->getCOMErrorCode() & ERRC_COM_CHECK_DATA
					&&	getModel()->getALARMHANDLER()->ALARM_SysFail_IF_COM->getAlarmState()==AS_ACTIVE)
				{
					getModel()->getALARMHANDLER()->setStateOfAlarm(AL_SysFail_IF_COM, AS_SIGNALED);
				}
			}
			else
			{
				if(m_iErrorCountCheckThread>ERRORCNT_CHECK)
				{
					DEBUGMSG(TRUE, (TEXT("#COM ERRORCNT_CHECK\r\n")));
					//error
					if(		getModel()->getDATAHANDLER()->getCOMErrorCode() == 0
						&&	getModel()->getALARMHANDLER()->ALARM_SysFail_IF_COM->getAlarmState()!=AS_ACTIVE
						&&	getModel()->isSERIALavailable()==TRUE)
					{
						getModel()->getDATAHANDLER()->setCOMErrorCode(ERRC_COM_CHECK_DATA);
						if(AfxGetApp())
							AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_COM_REINIT);
					}
					m_iErrorCountCheckThread=0;
				}
				else
				{
					DEBUGMSG(TRUE, (TEXT("#COM ERRORCNT1 ++\r\n")));
					m_iErrorCountCheckThread++;
				}
				CStringW szError=_T("");
				szError.Format(_T("#Serial CMD:O CtlL: %d GUI: %d"), iTemp,getModel()->getCONFIG()->GetCurMode());
				theApp.getLog()->WriteLine(szError);
			}
		}
	}
	else if(psz[0] == 'R')
	{
		if(false==getModel()->getCONFIG()->CurModeIsPresetMode())
		{
			CStringW szError=_T("");
			CStringW sToken = bl.GetString();
			sToken = sToken.Right(sToken.GetLength()-1);
			sToken = CTlsString::RemoveLeadingZeroes(sToken);

			WORD wTemp = (WORD)_wtoi(sToken);

			bool bError=false;

			if((wTemp & BIT0))
			{
				if(VENT_RUNNING==getModel()->GetVentRunState())
				{
					bError=true;
					szError.Format(_T("#Serial CMD: R CtlL: %d GUI: VENT_RUNNING"), wTemp);
					theApp.getLog()->WriteLine(szError);
				}			
			}
			else
			{
				if(VENT_RUNNING!=getModel()->GetVentRunState())
				{
					bError=true;
					szError.Format(_T("#Serial CMD: R CtlL: %d GUI: VENT_RUNNING!"), wTemp);
					theApp.getLog()->WriteLine(szError);
				}
			}

			if((wTemp & BIT1))
			{
				if(getModel()->getDATAHANDLER()->IsActiveModeVGarantStateOn()==false)
				{
					bError=true;
					szError.Format(_T("#Serial CMD: R CtlL: %d GUI: VG off"), wTemp);
					theApp.getLog()->WriteLine(szError);
				}
				
			}
			else
			{
				if(getModel()->getDATAHANDLER()->IsActiveModeVGarantStateOn()==true)
				{
					bError=true;
					szError.Format(_T("#Serial CMD: R CtlL: %d GUI: VG on"), wTemp);
					theApp.getLog()->WriteLine(szError);
				}
			}

			if((wTemp & BIT2))
			{
				if(false==getModel()->getDATAHANDLER()->IsActiveModeVLimitStateOn())
				{
					bError=true;
					szError.Format(_T("#Serial CMD: R CtlL: %d GUI: VL off"), wTemp);
					theApp.getLog()->WriteLine(szError);
				}		
			}
			else
			{
				if(true==getModel()->getDATAHANDLER()->IsActiveModeVLimitStateOn())
				{
					bError=true;
					szError.Format(_T("#Serial CMD: R CtlL: %d GUI: VL on"), wTemp);
					theApp.getLog()->WriteLine(szError);
				}
			}


			/*if(getModel()->getVMODEHANDLER()->activeModeIsTriggerMode())
			{
				if((wTemp & BIT7))
				{
					if(true==getModel()->getCONFIG()->GetVolumeTrigger())
					{
						bError=true;
						szError.Format(_T("#Serial CMD: R CtlL: %d GUI: VolumeTrigger true"), wTemp);
						theApp.getLog()->WriteLine(szError);
					}
				}
				else
				{
					if(false==getModel()->getCONFIG()->GetVolumeTrigger())
					{
						bError=true;
						szError.Format(_T("#Serial CMD: R CtlL: %d GUI: VolumeTrigger false"), wTemp);
						theApp.getLog()->WriteLine(szError);
					}
				}
			}*/

			if((wTemp & BIT8))
			{
				if(false==getModel()->getDATAHANDLER()->GetO21SensorCalState())
				{
					bError=true;
					szError.Format(_T("#Serial CMD: R CtlL: %d GUI: O21SensorCalState false"), wTemp);
					theApp.getLog()->WriteLine(szError);
				}
			}
			else
			{
				if(true==getModel()->getDATAHANDLER()->GetO21SensorCalState())
				{
					bError=true;
					szError.Format(_T("#Serial CMD: R CtlL: %d GUI: O21SensorCalState true"), wTemp);
					theApp.getLog()->WriteLine(szError);
				}
			}

			if((wTemp & BIT9))
			{
				if(false==getModel()->getDATAHANDLER()->GetO100SensorCalState())
				{
					bError=true;
					szError.Format(_T("#Serial CMD: R CtlL: %d GUI: O100SensorCalState false"), wTemp);
					theApp.getLog()->WriteLine(szError);
				}
			}
			else
			{
				if(true==getModel()->getDATAHANDLER()->GetO100SensorCalState())
				{
					bError=true;
					szError.Format(_T("#Serial CMD: R CtlL: %d GUI: O100SensorCalState true"), wTemp);
					theApp.getLog()->WriteLine(szError);
				}
			}

			if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
			{
				eRatioIE ratio = getModel()->getDATAHANDLER()->GetCurrentIERatioParaHFO();
				switch(ratio)
				{
				case RIE_1_3:
					{
						if(wTemp & BIT12)
						{
							szError.Format(_T("#Serial CMD: R CtlL: %d GUI: IERatioParaHFO RIE_1_3"), wTemp);
							theApp.getLog()->WriteLine(szError);
							bError=true;
						}

						if(wTemp & BIT13)
						{
							szError.Format(_T("#Serial CMD: R CtlL: %d GUI: IERatioParaHFO RIE_1_3"), wTemp);
							theApp.getLog()->WriteLine(szError);
							bError=true;
						}
					}
					break;
				case RIE_1_2:
					{
						if((wTemp & BIT12)==false)
						{
							bError=true;
							szError.Format(_T("#Serial CMD: R CtlL: %d GUI: IERatioParaHFO RIE_1_2"), wTemp);
							theApp.getLog()->WriteLine(szError);
						}

						if(wTemp & BIT13)
						{
							bError=true;
							szError.Format(_T("#Serial CMD: R CtlL: %d GUI: IERatioParaHFO RIE_1_2"), wTemp);
							theApp.getLog()->WriteLine(szError);
						}
					}
					break;
				case RIE_1_1:
					{
						if((wTemp & BIT13)==false)
						{
							bError=true;
							szError.Format(_T("#Serial CMD: R CtlL: %d GUI: IERatioParaHFO RIE_1_1"), wTemp);
							theApp.getLog()->WriteLine(szError);
						}

						if(wTemp & BIT12)
						{
							bError=true;
							szError.Format(_T("#Serial CMD: R CtlL: %d GUI: IERatioParaHFO RIE_1_1"), wTemp);
							theApp.getLog()->WriteLine(szError);
						}
					}
					break;
				}
			}

			if(!bError)
			{
				//newVG
				//m_iErrorCountCheckThread=0;
				//g_eventCOMCheckData.SetEvent();
				
				if(		getModel()->getDATAHANDLER()->getCOMErrorCode() & ERRC_COM_CHECK_DATA
					//&&	getModel()->getDATAHANDLER()->getCOMErrorCommand() & ERRC_COM_MODE_OPTION1
					&&	getModel()->getALARMHANDLER()->ALARM_SysFail_IF_COM->getAlarmState()==AS_ACTIVE)
				{
					getModel()->getALARMHANDLER()->setStateOfAlarm(AL_SysFail_IF_COM, AS_SIGNALED);
				}
			}
			else
			{
				if(m_iErrorCountCheckThread>ERRORCNT_CHECK)
				{
					DEBUGMSG(TRUE, (TEXT("#COM ERRORCNT_CHECK\r\n")));
					//error
					if(		getModel()->getDATAHANDLER()->getCOMErrorCode() == 0
						&&	getModel()->getALARMHANDLER()->ALARM_SysFail_IF_COM->getAlarmState()!=AS_ACTIVE
						&&	getModel()->isSERIALavailable()==TRUE)
					{
						getModel()->getDATAHANDLER()->setCOMErrorCode(ERRC_COM_CHECK_DATA);
						if(AfxGetApp())
							AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_COM_REINIT);
					}
					m_iErrorCountCheckThread=0;
				}
				else
				{
					DEBUGMSG(TRUE, (TEXT("#COM ERRORCNT2 ++\r\n")));
					m_iErrorCountCheckThread++;
				}
				CStringW szError=_T("");
				szError.Format(_T("#Serial CMD:R CtlL: %d"), wTemp);
				theApp.getLog()->WriteLine(szError);
			}
			
		}
	}
	else if(psz[0] == 'W')
	{
		if(false==getModel()->getCONFIG()->CurModeIsPresetMode())
		{
			CStringW sToken = bl.GetString();
			sToken = sToken.Right(sToken.GetLength()-1);
			sToken = CTlsString::RemoveLeadingZeroes(sToken);

			int iTemp = _wtoi(sToken);

			if(getModel()->getDATAHANDLER()->GetCurrentITimePara()==iTemp)
			{
				//newVG
				//m_iErrorCountCheckThread=0;
				//g_eventCOMCheckData.SetEvent();
				
				if(		getModel()->getDATAHANDLER()->getCOMErrorCode() & ERRC_COM_CHECK_DATA
					//&&	getModel()->getDATAHANDLER()->getCOMErrorCommand() & ERRC_COM_ITIME
					&&	getModel()->getALARMHANDLER()->ALARM_SysFail_IF_COM->getAlarmState()==AS_ACTIVE)
				{
					getModel()->getALARMHANDLER()->setStateOfAlarm(AL_SysFail_IF_COM, AS_SIGNALED);
				}
			}
			else
			{
				if(m_iErrorCountCheckThread>ERRORCNT_CHECK)
				{
					DEBUGMSG(TRUE, (TEXT("#COM ERRORCNT_CHECK\r\n")));
					//error
					if(		getModel()->getDATAHANDLER()->getCOMErrorCode() == 0
						&&	getModel()->getALARMHANDLER()->ALARM_SysFail_IF_COM->getAlarmState()!=AS_ACTIVE
						&&	getModel()->isSERIALavailable()==TRUE)
					{
						getModel()->getDATAHANDLER()->setCOMErrorCode(ERRC_COM_CHECK_DATA);
						if(AfxGetApp())
							AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_COM_REINIT);
					}
					m_iErrorCountCheckThread=0;
				}
				else
				{
					DEBUGMSG(TRUE, (TEXT("#COM ERRORCNT3 ++\r\n")));
					m_iErrorCountCheckThread++;
				}
				CStringW szError=_T("");
				szError.Format(_T("#Serial CMD: W CtlL: %d GUI: %d"), iTemp,getModel()->getDATAHANDLER()->GetCurrentITimePara());
				theApp.getLog()->WriteLine(szError);
			}
		}
	}
	else if(psz[0] == 'E')
	{
		if(false==getModel()->getCONFIG()->CurModeIsPresetMode())
		{
			CStringW sToken = bl.GetString();
			sToken = sToken.Right(sToken.GetLength()-1);
			sToken = CTlsString::RemoveLeadingZeroes(sToken);

			int iTemp = _wtoi(sToken);

			if(getModel()->getDATAHANDLER()->GetCurrentETimePara()==iTemp)
			{
				//newVG
				//m_iErrorCountCheckThread=0;
				//g_eventCOMCheckData.SetEvent();

				if(		getModel()->getDATAHANDLER()->getCOMErrorCode() & ERRC_COM_CHECK_DATA
					//&&	getModel()->getDATAHANDLER()->getCOMErrorCommand() & ERRC_COM_ETIME
					&&	getModel()->getALARMHANDLER()->ALARM_SysFail_IF_COM->getAlarmState()==AS_ACTIVE)
				{
					getModel()->getALARMHANDLER()->setStateOfAlarm(AL_SysFail_IF_COM, AS_SIGNALED);
				}
			}
			else
			{
				if(m_iErrorCountCheckThread>ERRORCNT_CHECK)
				{
					DEBUGMSG(TRUE, (TEXT("#COM ERRORCNT_CHECK\r\n")));
					//error
					if(		getModel()->getDATAHANDLER()->getCOMErrorCode() == 0
						&&	getModel()->getALARMHANDLER()->ALARM_SysFail_IF_COM->getAlarmState()!=AS_ACTIVE
						&&	getModel()->isSERIALavailable()==TRUE)
					{
						getModel()->getDATAHANDLER()->setCOMErrorCode(ERRC_COM_CHECK_DATA);
						if(AfxGetApp())
							AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_COM_REINIT);
					}
					m_iErrorCountCheckThread=0;
				}
				else
				{
					DEBUGMSG(TRUE, (TEXT("#COM ERRORCNT4 ++\r\n")));
					m_iErrorCountCheckThread++;
				}

				CStringW szError=_T("");
				szError.Format(_T("#Serial CMD: E CtlL: %d GUI: %d"), iTemp,getModel()->getDATAHANDLER()->GetCurrentETimePara());
				theApp.getLog()->WriteLine(szError);
			}
		}
	}
	else if(psz[0] == 'U')
	{
		if(false==getModel()->getCONFIG()->CurModeIsPresetMode())
		{
			CStringW sToken = bl.GetString();
			sToken = sToken.Right(sToken.GetLength()-1);
			sToken = CTlsString::RemoveLeadingZeroes(sToken);

			int iTemp = _wtoi(sToken);

			if(getModel()->getDATAHANDLER()->GetCurrentIFlowPara()==iTemp)
			{
				//newVG
				//m_iErrorCountCheckThread=0;
				//g_eventCOMCheckData.SetEvent();

				if(		getModel()->getDATAHANDLER()->getCOMErrorCode() & ERRC_COM_CHECK_DATA
					//&&	getModel()->getDATAHANDLER()->getCOMErrorCommand() & ERRC_COM_IFLOW
					&&	getModel()->getALARMHANDLER()->ALARM_SysFail_IF_COM->getAlarmState()==AS_ACTIVE)
				{
					getModel()->getALARMHANDLER()->setStateOfAlarm(AL_SysFail_IF_COM, AS_SIGNALED);
				}
			}
			else
			{
				if(m_iErrorCountCheckThread>ERRORCNT_CHECK)
				{
					DEBUGMSG(TRUE, (TEXT("#COM ERRORCNT_CHECK\r\n")));
					//error
					if(		getModel()->getDATAHANDLER()->getCOMErrorCode() == 0
						&&	getModel()->getALARMHANDLER()->ALARM_SysFail_IF_COM->getAlarmState()!=AS_ACTIVE
						&&	getModel()->isSERIALavailable()==TRUE)
					{
						getModel()->getDATAHANDLER()->setCOMErrorCode(ERRC_COM_CHECK_DATA);
						if(AfxGetApp())
							AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_COM_REINIT);
					}
					m_iErrorCountCheckThread=0;
				}
				else
				{
					DEBUGMSG(TRUE, (TEXT("#COM ERRORCNT5 ++\r\n")));
					m_iErrorCountCheckThread++;
				}

				CStringW szError=_T("");
				szError.Format(_T("#Serial CMD: U CtlL: %d GUI: %d"), iTemp,getModel()->getDATAHANDLER()->GetCurrentIFlowPara());
				theApp.getLog()->WriteLine(szError);
			}
		}
	}
	else if(psz[0] == ']')
	{
		if(false==getModel()->getCONFIG()->CurModeIsPresetMode())
		{
			CStringW sToken = bl.GetString();
			sToken = sToken.Right(sToken.GetLength()-1);
			sToken = CTlsString::RemoveLeadingZeroes(sToken);

			int iTemp = _wtoi(sToken);

			int iRiseTime=getModel()->getDATAHANDLER()->GetCurrentRisetimePara();

			if(iRiseTime==iTemp)
			{
				//newVG
				//m_iErrorCountCheckThread=0;
				//g_eventCOMCheckData.SetEvent();

				if(		getModel()->getDATAHANDLER()->getCOMErrorCode() & ERRC_COM_CHECK_DATA
					//&&	getModel()->getDATAHANDLER()->getCOMErrorCommand() & ERRC_COM_RISETIME
					&&	getModel()->getALARMHANDLER()->ALARM_SysFail_IF_COM->getAlarmState()==AS_ACTIVE)
				{
					getModel()->getALARMHANDLER()->setStateOfAlarm(AL_SysFail_IF_COM, AS_SIGNALED);
				}
			}
			else
			{
				if(m_iErrorCountCheckThread>ERRORCNT_CHECK)
				{
					DEBUGMSG(TRUE, (TEXT("#COM ERRORCNT_CHECK\r\n")));
					//error
					if(		getModel()->getDATAHANDLER()->getCOMErrorCode() == 0
						&&	getModel()->getALARMHANDLER()->ALARM_SysFail_IF_COM->getAlarmState()!=AS_ACTIVE
						&&	getModel()->isSERIALavailable()==TRUE)
					{
						getModel()->getDATAHANDLER()->setCOMErrorCode(ERRC_COM_CHECK_DATA);
						if(AfxGetApp())
							AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_COM_REINIT);
					}
					m_iErrorCountCheckThread=0;
				}
				else
				{
					DEBUGMSG(TRUE, (TEXT("#COM ERRORCNT6 ++\r\n")));
					m_iErrorCountCheckThread++;
				}

				CStringW szError=_T("");
				szError.Format(_T("#Serial CMD: ] CtlL: %d GUI: %d"), iTemp,iRiseTime);
				theApp.getLog()->WriteLine(szError);
			}
		}
	}
	else if(psz[0] == 'A')
	{
		if(false==getModel()->getCONFIG()->CurModeIsPresetMode())
		{
			CStringW sToken = bl.GetString();
			sToken = sToken.Right(sToken.GetLength()-1);
			sToken = CTlsString::RemoveLeadingZeroes(sToken);

			int iTemp = _wtoi(sToken);

			int iFlow=0;
			if(getModel()->getCONFIG()->GetCurMode()==VM_CPAP)
			{
				iFlow=getModel()->getDATAHANDLER()->PARADATA()->GetFlowminPara();
			}
			else if(getModel()->getCONFIG()->GetCurMode()==VM_THERAPIE)
			{
				iFlow=getModel()->getDATAHANDLER()->PARADATA()->GetTherapieFLOWPara();
			}
			else
			{
				iFlow=getModel()->getDATAHANDLER()->PARADATA()->GetEFLOWPara_TRIGGER();
			}

			if(iFlow==iTemp)
			{
				//newVG
				//m_iErrorCountCheckThread=0;
				//g_eventCOMCheckData.SetEvent();

				if(		getModel()->getDATAHANDLER()->getCOMErrorCode() & ERRC_COM_CHECK_DATA
					//&&	getModel()->getDATAHANDLER()->getCOMErrorCommand() & ERRC_COM_EFLOW
					&&	getModel()->getALARMHANDLER()->ALARM_SysFail_IF_COM->getAlarmState()==AS_ACTIVE)
				{
					getModel()->getALARMHANDLER()->setStateOfAlarm(AL_SysFail_IF_COM, AS_SIGNALED);
				}
			}
			else
			{
				if(m_iErrorCountCheckThread>ERRORCNT_CHECK)
				{
					DEBUGMSG(TRUE, (TEXT("#COM ERRORCNT_CHECK\r\n")));
					//error
					if(		getModel()->getDATAHANDLER()->getCOMErrorCode() == 0
						&&	getModel()->getALARMHANDLER()->ALARM_SysFail_IF_COM->getAlarmState()!=AS_ACTIVE
						&&	getModel()->isSERIALavailable()==TRUE)
					{
						getModel()->getDATAHANDLER()->setCOMErrorCode(ERRC_COM_CHECK_DATA);
						if(AfxGetApp())
							AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_COM_REINIT);
					}
					m_iErrorCountCheckThread=0;
				}
				else
				{
					DEBUGMSG(TRUE, (TEXT("#COM ERRORCNT7 ++\r\n")));
					m_iErrorCountCheckThread++;
				}

				CStringW szError=_T("");
				szError.Format(_T("#Serial CMD: A CtlL: %d GUI: %d"), iTemp,iFlow);
				theApp.getLog()->WriteLine(szError);

			}
		}
	}
	else if(psz[0] == 'F')
	{
		if(false==getModel()->getCONFIG()->CurModeIsPresetMode())
		{
			CStringW sToken = bl.GetString();
			sToken = sToken.Right(sToken.GetLength()-1);
			sToken = CTlsString::RemoveLeadingZeroes(sToken);

			int iTemp = _wtoi(sToken);

			if(getModel()->getDATAHANDLER()->GetCurrentHFFreqPara()==iTemp)
			{
				//newVG
				//m_iErrorCountCheckThread=0;
				//g_eventCOMCheckData.SetEvent();

				if(		getModel()->getDATAHANDLER()->getCOMErrorCode() & ERRC_COM_CHECK_DATA
					//&&	getModel()->getDATAHANDLER()->getCOMErrorCommand() & ERRC_COM_HFFREQ
					&&	getModel()->getALARMHANDLER()->ALARM_SysFail_IF_COM->getAlarmState()==AS_ACTIVE)
				{
					getModel()->getALARMHANDLER()->setStateOfAlarm(AL_SysFail_IF_COM, AS_SIGNALED);
				}
			}
			else
			{
				if(m_iErrorCountCheckThread>ERRORCNT_CHECK)
				{
					DEBUGMSG(TRUE, (TEXT("#COM ERRORCNT_CHECK\r\n")));
					//error
					if(		getModel()->getDATAHANDLER()->getCOMErrorCode() == 0
						&&	getModel()->getALARMHANDLER()->ALARM_SysFail_IF_COM->getAlarmState()!=AS_ACTIVE
						&&	getModel()->isSERIALavailable()==TRUE)
					{
						getModel()->getDATAHANDLER()->setCOMErrorCode(ERRC_COM_CHECK_DATA);
						if(AfxGetApp())
							AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_COM_REINIT);
					}
					m_iErrorCountCheckThread=0;
				}
				else
				{
					DEBUGMSG(TRUE, (TEXT("#COM ERRORCNT8 ++\r\n")));
					m_iErrorCountCheckThread++;
				}

				CStringW szError=_T("");
				szError.Format(_T("#Serial CMD: F CtlL: %d GUI: %d"), iTemp,getModel()->getDATAHANDLER()->GetCurrentHFFreqPara());
				theApp.getLog()->WriteLine(szError);
			}
		}
	}
	else if(psz[0] == 'J')
	{
		if(false==getModel()->getCONFIG()->CurModeIsPresetMode())
		{
			CStringW sToken = bl.GetString();
			sToken = sToken.Right(sToken.GetLength()-1);
			sToken = CTlsString::RemoveLeadingZeroes(sToken);

			int iTemp = _wtoi(sToken);

			if(getModel()->getDATAHANDLER()->GetCurrentPmeanPara()==iTemp)
			{
				//newVG
				//m_iErrorCountCheckThread=0;
				//g_eventCOMCheckData.SetEvent();

				if(		getModel()->getDATAHANDLER()->getCOMErrorCode() & ERRC_COM_CHECK_DATA
					//&&	getModel()->getDATAHANDLER()->getCOMErrorCommand() & ERRC_COM_HFPMEAN
					&&	getModel()->getALARMHANDLER()->ALARM_SysFail_IF_COM->getAlarmState()==AS_ACTIVE)
				{
					getModel()->getALARMHANDLER()->setStateOfAlarm(AL_SysFail_IF_COM, AS_SIGNALED);
				}
			}
			else
			{
				if(m_iErrorCountCheckThread>ERRORCNT_CHECK)
				{
					DEBUGMSG(TRUE, (TEXT("#COM ERRORCNT_CHECK\r\n")));
					//error
					if(		getModel()->getDATAHANDLER()->getCOMErrorCode() == 0
						&&	getModel()->getALARMHANDLER()->ALARM_SysFail_IF_COM->getAlarmState()!=AS_ACTIVE
						&&	getModel()->isSERIALavailable()==TRUE)
					{
						getModel()->getDATAHANDLER()->setCOMErrorCode(ERRC_COM_CHECK_DATA);
						if(AfxGetApp())
							AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_COM_REINIT);
					}
					m_iErrorCountCheckThread=0;
				}
				else
				{
					DEBUGMSG(TRUE, (TEXT("#COM ERRORCNT9 ++\r\n")));
					m_iErrorCountCheckThread++;
				}

				CStringW szError=_T("");
				szError.Format(_T("#Serial CMD: J CtlL: %d GUI: %d"), iTemp,getModel()->getDATAHANDLER()->GetCurrentPmeanPara());
				theApp.getLog()->WriteLine(szError);
			}
		}
	}
	//else if(psz[0] == 'K')
	//{
	//	if(false==getModel()->getCONFIG()->CurModeIsPresetMode())
	//	{
	//		CStringW sToken = bl.GetString();
	//		sToken = sToken.Right(sToken.GetLength()-1);
	//		sToken = CTlsString::RemoveLeadingZeroes(sToken);

	//		int iTemp = _wtoi(sToken);

	//		if(getModel()->getDATAHANDLER()->GetCurrentIERatioParaHFO()==iTemp)
	//		{
	//			//newVG g_eventCOMCheckData.SetEvent();
	//		}
	//	}
	//}
	else if(psz[0] == 'M')
	{
		if(false==getModel()->getCONFIG()->CurModeIsPresetMode())
		{
			CStringW sToken = bl.GetString();
			sToken = sToken.Right(sToken.GetLength()-1);
			sToken = CTlsString::RemoveLeadingZeroes(sToken);

			int iTemp = _wtoi(sToken);

			WORD wHFAMPL=0;
			if(getModel()->getDATAHANDLER()->PARADATA()->IsHFVGarantStateOn())
			{
				wHFAMPL=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLmaxPara();
				
				//AmplitudeCorrectionFactor
				/*double iAmpCorFactor = getModel()->getDATAHANDLER()->getAmpCorFactor(getModel()->getDATAHANDLER()->PARADATA()->GetHFFreqPara());
				if(iAmpCorFactor==0)
					iAmpCorFactor=1;
				wHFAMPL=(SHORT)(double)wHFAMPL/iAmpCorFactor;*/
			}
			else
			{
				wHFAMPL=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLPara();
				
				//AmplitudeCorrectionFactor
				/*double iPmitt=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanPara();
				double iAmpCorFactor = getModel()->getDATAHANDLER()->getAmpCorFactor(getModel()->getDATAHANDLER()->PARADATA()->GetHFFreqPara());
				if(iAmpCorFactor==0)
					iAmpCorFactor=1;
				wHFAMPL=(SHORT)(double)wHFAMPL/iAmpCorFactor;*/
			}

			if(wHFAMPL==iTemp)
			{
				//newVG
				//m_iErrorCountCheckThread=0;
				//g_eventCOMCheckData.SetEvent();

				if(		getModel()->getDATAHANDLER()->getCOMErrorCode() & ERRC_COM_CHECK_DATA
					//&&	getModel()->getDATAHANDLER()->getCOMErrorCommand() & ERRC_COM_HFAMP
					&&	getModel()->getALARMHANDLER()->ALARM_SysFail_IF_COM->getAlarmState()==AS_ACTIVE)
				{
					getModel()->getALARMHANDLER()->setStateOfAlarm(AL_SysFail_IF_COM, AS_SIGNALED);
				}
			}
			else
			{
				if(m_iErrorCountCheckThread>ERRORCNT_CHECK)
				{
					DEBUGMSG(TRUE, (TEXT("#COM ERRORCNT_CHECK\r\n")));
					//error
					if(		getModel()->getDATAHANDLER()->getCOMErrorCode() == 0
						&&	getModel()->getALARMHANDLER()->ALARM_SysFail_IF_COM->getAlarmState()!=AS_ACTIVE
						&&	getModel()->isSERIALavailable()==TRUE)
					{
						getModel()->getDATAHANDLER()->setCOMErrorCode(ERRC_COM_CHECK_DATA);
						if(AfxGetApp())
							AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_COM_REINIT);
					}
					m_iErrorCountCheckThread=0;
				}
				else
				{
					DEBUGMSG(TRUE, (TEXT("#COM ERRORCNT10 ++\r\n")));
					m_iErrorCountCheckThread++;
				}

				CStringW szError=_T("");
				szError.Format(_T("#Serial CMD: M CtlL: %d GUI: %d"), iTemp,wHFAMPL);
				theApp.getLog()->WriteLine(szError);
			}
		}
	}
	else if(psz[0] == 'T')
	{
		bool bFOTrun=false;
		if(getModel()->getFOTThread())
		{
			if(getModel()->getFOTThread()->getFOTstate()!=FOT_OFF)
			{
				bFOTrun=true;
			}
		}

		if(false==getModel()->getCONFIG()->CurModeIsPresetMode() && false==bFOTrun)
		{
			CStringW sToken = bl.GetString();
			sToken = sToken.Right(sToken.GetLength()-1);
			sToken = CTlsString::RemoveLeadingZeroes(sToken);

			int iTemp = _wtoi(sToken);

			//test
			/*if(m_bResetTestFlag==false)
				iTemp=200;*/

			int iPIP=getModel()->getDATAHANDLER()->GetActiveModePINSPPara(); //newVG

			if(iPIP==iTemp)
			{
				//newVG
				//m_iErrorCountCheckThread=0;
				//g_eventCOMCheckData.SetEvent();

				//newVG
				if(		getModel()->getDATAHANDLER()->getCOMErrorCode() & ERRC_COM_CHECK_DATA
					//&&	getModel()->getDATAHANDLER()->getCOMErrorCommand() & ERRC_COM_PINSP
					&&	getModel()->getALARMHANDLER()->ALARM_SysFail_IF_COM->getAlarmState()==AS_ACTIVE)
				{
					getModel()->getALARMHANDLER()->setStateOfAlarm(AL_SysFail_IF_COM, AS_SIGNALED);
				}
			}
			else
			{
				if(m_iErrorCountCheckThread>ERRORCNT_CHECK)
				{
					DEBUGMSG(TRUE, (TEXT("#COM ERRORCNT_CHECK\r\n")));
					//error
					if(		getModel()->getDATAHANDLER()->getCOMErrorCode() == 0
						&&	getModel()->getALARMHANDLER()->ALARM_SysFail_IF_COM->getAlarmState()!=AS_ACTIVE
						&&	getModel()->isSERIALavailable()==TRUE)
					{
						getModel()->getDATAHANDLER()->setCOMErrorCode(ERRC_COM_CHECK_DATA);
						if(AfxGetApp())
							AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_COM_REINIT);
					}
					m_iErrorCountCheckThread=0;
				}
				else
				{
					//DEBUGMSG(TRUE, (TEXT("#COM ERRORCNT11 ++\r\n")));
					DEBUGMSG(TRUE, (TEXT("#COM ERRORCNT11 ++ PIC%d DEV%d\r\n"),iTemp,iPIP));
					m_iErrorCountCheckThread++;
				}

				CStringW szError=_T("");
				szError.Format(_T("#Serial CMD: T CtlL: %d GUI: %d"), iTemp,iPIP);
				theApp.getLog()->WriteLine(szError);

			}
		}
	}
	else if(psz[0] == '>')//newVG
	{
		if(false==getModel()->getCONFIG()->CurModeIsPresetMode())
		{
			CStringW sToken = bl.GetString();
			sToken = sToken.Right(sToken.GetLength()-1);
			sToken = CTlsString::RemoveLeadingZeroes(sToken);

			int iTemp = _wtoi(sToken);

			int iPMAX=getModel()->getDATAHANDLER()->GetActiveModePMAXVGPara();

			if(iPMAX==iTemp)
			{
				//newVG
				//m_iErrorCountCheckThread=0;
				//g_eventCOMCheckData.SetEvent();

				if(		getModel()->getDATAHANDLER()->getCOMErrorCode() & ERRC_COM_CHECK_DATA
					//&&	getModel()->getDATAHANDLER()->getCOMErrorCommand() & ERRC_COM_PMAXVG
					&&	getModel()->getALARMHANDLER()->ALARM_SysFail_IF_COM->getAlarmState()==AS_ACTIVE)
				{
					getModel()->getALARMHANDLER()->setStateOfAlarm(AL_SysFail_IF_COM, AS_SIGNALED);
				}
			}
			else
			{
				if(m_iErrorCountCheckThread>ERRORCNT_CHECK)
				{
					DEBUGMSG(TRUE, (TEXT("#COM ERRORCNT_CHECK\r\n")));
					//error
					if(		getModel()->getDATAHANDLER()->getCOMErrorCode() == 0
						&&	getModel()->getALARMHANDLER()->ALARM_SysFail_IF_COM->getAlarmState()!=AS_ACTIVE
						&&	getModel()->isSERIALavailable()==TRUE)
					{
						getModel()->getDATAHANDLER()->setCOMErrorCode(ERRC_COM_CHECK_DATA);
						if(AfxGetApp())
							AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_COM_REINIT);
					}
					m_iErrorCountCheckThread=0;
				}
				else
				{
					DEBUGMSG(TRUE, (TEXT("#COM ERRORCNT12 ++\r\n")));
					m_iErrorCountCheckThread++;
				}

				CStringW szError=_T("");
				szError.Format(_T("#Serial CMD: > CtlL: %d GUI: %d"), iTemp,iPMAX);
				theApp.getLog()->WriteLine(szError);
			}
		}
	}
	else if(psz[0] == 'Z')
	{
		if(false==getModel()->getCONFIG()->CurModeIsPresetMode())
		{
			CStringW sToken = bl.GetString();
			sToken = sToken.Right(sToken.GetLength()-1);
			sToken = CTlsString::RemoveLeadingZeroes(sToken);

			int iTemp = _wtoi(sToken);

			if(getModel()->getDATAHANDLER()->GetCurrentPEEPPara()==iTemp)
			{
				//newVG
				//m_iErrorCountCheckThread=0;
				//g_eventCOMCheckData.SetEvent();

				if(		getModel()->getDATAHANDLER()->getCOMErrorCode() & ERRC_COM_CHECK_DATA
					//&&	getModel()->getDATAHANDLER()->getCOMErrorCommand() & ERRC_COM_PEEP
					&&	getModel()->getALARMHANDLER()->ALARM_SysFail_IF_COM->getAlarmState()==AS_ACTIVE)
				{
					getModel()->getALARMHANDLER()->setStateOfAlarm(AL_SysFail_IF_COM, AS_SIGNALED);
				}
			}
			else
			{
				if(m_iErrorCountCheckThread>ERRORCNT_CHECK)
				{
					DEBUGMSG(TRUE, (TEXT("#COM ERRORCNT_CHECK\r\n")));
					//error
					if(		getModel()->getDATAHANDLER()->getCOMErrorCode() == 0
						&&	getModel()->getALARMHANDLER()->ALARM_SysFail_IF_COM->getAlarmState()!=AS_ACTIVE
						&&	getModel()->isSERIALavailable()==TRUE)
					{
						getModel()->getDATAHANDLER()->setCOMErrorCode(ERRC_COM_CHECK_DATA);
						if(AfxGetApp())
							AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_COM_REINIT);
					}
					m_iErrorCountCheckThread=0;
				}
				else
				{
					DEBUGMSG(TRUE, (TEXT("#COM ERRORCNT13 ++\r\n")));
					m_iErrorCountCheckThread++;
				}


				CStringW szError=_T("");
				szError.Format(_T("#Serial CMD: Z CtlL: %d GUI: %d"), iTemp,getModel()->getDATAHANDLER()->GetCurrentPEEPPara());
				theApp.getLog()->WriteLine(szError);
			}
		}
	}
	else if(psz[0] == 'C')
	{
		if(false==getModel()->getCONFIG()->CurModeIsPresetMode())
		{
			CStringW sToken = bl.GetString();
			sToken = sToken.Right(sToken.GetLength()-1);
			sToken = CTlsString::RemoveLeadingZeroes(sToken);

			int iTemp = _wtoi(sToken);

			if(getModel()->getDATAHANDLER()->GetCurrentPpsvPara()==iTemp)
			{
				//newVG
				//m_iErrorCountCheckThread=0;
				//g_eventCOMCheckData.SetEvent();

				if(		getModel()->getDATAHANDLER()->getCOMErrorCode() & ERRC_COM_CHECK_DATA
					//&&	getModel()->getDATAHANDLER()->getCOMErrorCommand() & ERRC_COM_PPSV
					&&	getModel()->getALARMHANDLER()->ALARM_SysFail_IF_COM->getAlarmState()==AS_ACTIVE)
				{
					getModel()->getALARMHANDLER()->setStateOfAlarm(AL_SysFail_IF_COM, AS_SIGNALED);
				}
			}
			else
			{
				if(m_iErrorCountCheckThread>ERRORCNT_CHECK)
				{
					DEBUGMSG(TRUE, (TEXT("#COM ERRORCNT_CHECK\r\n")));
					//error
					if(		getModel()->getDATAHANDLER()->getCOMErrorCode() == 0
						&&	getModel()->getALARMHANDLER()->ALARM_SysFail_IF_COM->getAlarmState()!=AS_ACTIVE
						&&	getModel()->isSERIALavailable()==TRUE)
					{
						getModel()->getDATAHANDLER()->setCOMErrorCode(ERRC_COM_CHECK_DATA);
						if(AfxGetApp())
							AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_COM_REINIT);
					}
					m_iErrorCountCheckThread=0;
				}
				else
				{
					DEBUGMSG(TRUE, (TEXT("#COM ERRORCNT14 ++\r\n")));
					m_iErrorCountCheckThread++;
				}

				CStringW szError=_T("");
				szError.Format(_T("#Serial CMD: C CtlL: %d GUI: %d"), iTemp,getModel()->getDATAHANDLER()->GetCurrentPpsvPara());
				theApp.getLog()->WriteLine(szError);
			}
		}
	}
	else if(psz[0] == 'D')
	{
		/*if(		getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true
			&&	getModel()->getPRICOThread()->isPRICOrunning())
		{
			
		}*/

		bool bPRICOrunning=false;
		if(getModel()->getPRICOThread())//PRICO04
		{
			if(getModel()->getPRICOThread()->isPRICOalgorithmRunning())
			{
				bPRICOrunning=true;
			}
		}

		if(false==getModel()->getCONFIG()->CurModeIsPresetMode() && bPRICOrunning==false)
		{
			CStringW sToken = bl.GetString();
			sToken = sToken.Right(sToken.GetLength()-1);
			sToken = CTlsString::RemoveLeadingZeroes(sToken);

			int iTemp = _wtoi(sToken);

			int iCurVal=0;
			if(getModel()->isO2FlushActive())
			{
				iCurVal=getModel()->getDATAHANDLER()->GetCurrentO2FlushPara();
			}
			else
			{
				iCurVal=getModel()->getDATAHANDLER()->GetCurrentO2Para();
			}
			if(iCurVal==iTemp)
			{
				//newVG
				//m_iErrorCountCheckThread=0;
				//g_eventCOMCheckData.SetEvent();

				if(		getModel()->getDATAHANDLER()->getCOMErrorCode() & ERRC_COM_CHECK_DATA
					//&&	getModel()->getDATAHANDLER()->getCOMErrorCommand() & ERRC_COM_O2
					&&	getModel()->getALARMHANDLER()->ALARM_SysFail_IF_COM->getAlarmState()==AS_ACTIVE)
				{
					getModel()->getALARMHANDLER()->setStateOfAlarm(AL_SysFail_IF_COM, AS_SIGNALED);
				}
			}
			else
			{
				if(m_iErrorCountCheckThread>ERRORCNT_CHECK)
				{
					DEBUGMSG(TRUE, (TEXT("#COM ERRORCNT_CHECK\r\n")));
					//error
					if(		getModel()->getDATAHANDLER()->getCOMErrorCode() == 0
						&&	getModel()->getALARMHANDLER()->ALARM_SysFail_IF_COM->getAlarmState()!=AS_ACTIVE
						&&	getModel()->isSERIALavailable()==TRUE)
					{
						getModel()->getDATAHANDLER()->setCOMErrorCode(ERRC_COM_CHECK_DATA);
						if(AfxGetApp())
							AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_COM_REINIT);
					}
					m_iErrorCountCheckThread=0;
				}
				else
				{
					DEBUGMSG(TRUE, (TEXT("#COM ERRORCNT15 ++\r\n")));
					m_iErrorCountCheckThread++;
				}

				CStringW szError=_T("");
				szError.Format(_T("#Serial CMD: D CtlL: %d GUI: %d"), iTemp,iCurVal);
				theApp.getLog()->WriteLine(szError);

			}
		}

	}
	else if(psz[0] == 'c')
	{
		bResult = false;
	}
	else
	{
		bResult = false;

		m_comPort.FlushBuffer();
	}


	//while(psz[iLen-1] == '\r' || psz[iLen-1] == '\n')
	//	iLen--;

	//if(psz[0] != '$')
	//{
	//	// Ungültiger Datensatz
	//	/*CTlsLogfile::WriteLine(TEXT("Ungültiger Datensatz1"));
	//	CTlsLogfile::WriteLine(psz);*/
	//	return false;
	//}

	//if(psz[iLen-3] != '*')
	//{
	//	// Ungültiger Datensatz
	//	/*CTlsLogfile::WriteLine(TEXT("Ungültiger Datensatz2"));
	//	CTlsLogfile::WriteLine(psz);*/
	//	return false;
	//}

	//unsigned char cCRC = 0;
	//for(int idx = 1; idx < iLen-3; idx++)
	//{
	//	cCRC ^= psz[idx];
	//}

	//if(   CTlsHex::ToHex(cCRC >> 4)   != psz[iLen-2]
	//|| CTlsHex::ToHex(cCRC & 0x0F) != psz[iLen-1])
	//{
	//	// CRC-Fehler
	//	/*CTlsLogfile::WriteLine(TEXT("HandleNMEA -> CRC-Fehler"));*/
	//	return false;
	//}

	//// Auslesen
	//CStringW sToken;
	//int iPos = 0;
	//Split(bl, iPos, sToken);

	//if(sToken == "$GPGGA")
	//{
	//	/*CTlsLogfile::WriteLine(TEXT("Datensatz GPGGA"));
	//	CTlsLogfile::WriteLine(psz);*/
	//	return HandleGPGGA(bl, iPos);
	//}
	//else if (sToken == "$GPRMC")
	//{
	//	/*CTlsLogfile::WriteLine(TEXT("Datensatz GPRMC"));
	//	CTlsLogfile::WriteLine(psz);*/
	//	return HandleGPRMC(bl, iPos);
	//}
	//else if (sToken == "$GPGSA") // Active satellites
	//{

	//	return true;
	//}
	//else if (sToken == "$GPGSV") // Satellites in view
	//{
	//	return true;
	//}
	////  else ...


	return bResult;
}

bool CInterfaceSerial::IsSerialDisconnection()
{
	return m_bSerialDisconnection;
}

//bool CInterfaceSerial::IsSerial_P_EXSPIRATION()
//{
//	return m_bSerialEXSPIRATIONTUBE;
//}
//bool CInterfaceSerial::IsSerial_P_INSPIRATION()
//{
//	return m_bSerialINSPIRATIONTUBE;
//}
bool CInterfaceSerial::IsSerial_TUBE_OCCLUSION()
{
	return m_bSerialTUBE_OCCLUSION;
}

// **************************************************************************
// 
// **************************************************************************
bool CInterfaceSerial::CheckBlenderStatusBits(CTlsBlob bl)
{
	//int iLen = bl.GetBufferLen();
	unsigned char* psz = bl.GetBuffer();

	if(psz[0] == 'I')// 	BLENDER_STATUS
	{
		CStringW sToken = bl.GetString();
		sToken = sToken.Right(sToken.GetLength()-1);
		sToken = CTlsString::RemoveLeadingZeroes(sToken);

		WORD byTemp = (WORD)_wtoi(sToken);

		getModel()->getDATAHANDLER()->SetMSTATUS_BlenderPIC((int)byTemp);
	}
	
	
	return true;
}


// **************************************************************************
// 
// **************************************************************************
bool CInterfaceSerial::CheckStatusBits(CTlsBlob bl)
{
	//int iLen = bl.GetBufferLen();
	unsigned char* psz = bl.GetBuffer();

	if(psz[0] == 'L')// 	M_STATUS
	{
		CStringW sToken = bl.GetString();
		sToken = sToken.Right(sToken.GetLength()-1);
		sToken = CTlsString::RemoveLeadingZeroes(sToken);

		WORD byTemp = (WORD)_wtoi(sToken);

		getModel()->getDATAHANDLER()->SetMSTATUS_ConductorPIC((int)byTemp);
		
		if(byTemp & PSVAPNOE)
		{
			if(		getModel()->getCONFIG()->GetCurMode()==VM_PSV 
				||	(getModel()->getCONFIG()->CurModeIsPresetMode()==true && getModel()->getCONFIG()->GetPrevMode()==VM_PSV))
			{
				if(getModel()->getDATAHANDLER()->isPSVapnoe()==false && getModel()->isMANBREATHrunning()==false)
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PSV_APNOE);
				}
			}
		}
		else
		{
			if(		getModel()->getCONFIG()->GetCurMode()==VM_PSV 
				||	(getModel()->getCONFIG()->CurModeIsPresetMode()==true && getModel()->getCONFIG()->GetPrevMode()==VM_PSV))
			{
				if(getModel()->getDATAHANDLER()->isPSVapnoe()==true)
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PSV_APNOE_END);
				}
			}
		}

		if(byTemp & IPRESSUREBIT)
		{
			getModel()->getDATAHANDLER()->SetPinspNotReachedFlag(true);
		}
		else
		{
			getModel()->getDATAHANDLER()->SetPinspNotReachedFlag(false);
		}

		if(getModel()->getCONFIG()->GetCurMode()!=VM_SERVICE)
		{
			if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
			{
				if(byTemp & HFOSTOPPED)
				{
					if(getModel()->getDATAHANDLER()->IsHFOrunning())
						getModel()->getDATAHANDLER()->SetHFOrunning(false);
				}
				else
				{
					if(getModel()->getDATAHANDLER()->IsHFOrunning()==false)
						getModel()->getDATAHANDLER()->SetHFOrunning(true);
				}
			}
			else
			{
				if(getModel()->getDATAHANDLER()->IsHFOrunning())
					getModel()->getDATAHANDLER()->SetHFOrunning(false);
			}
		}
		

		if(byTemp & DISCONNECTIONBIT)
		{
			m_bSerialDisconnection=true;
			if(	getModel()->getALARMHANDLER()->CanSetAlarm_DISCONNECTION())
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SERIAL_SETALARM_DISCONNECTION);
			}
		}
		else
		{
			if(		(getModel()->getALARMHANDLER()->ALARM_DISCONNECTION->getAlarmState()==AS_ACTIVE)
				&&	(getModel()->IsSPIDisconnection()==false))
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SERIAL_DELALARM_DISCONNECTION);
			}
			
			
			m_bSerialDisconnection=false;
		}
		
		if(byTemp & MIXER_AV_BIT)
		{
			if(	getModel()->getALARMHANDLER()->CanSetAlarm_MIXER_AVCAL())
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SERIAL_SETALARM_MIXER_AVCAL);
			}
		}
		else
		{
			if(getModel()->getALARMHANDLER()->ALARM_SysFail_MIXER_AVCAL->getAlarmState()==AS_ACTIVE)
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SERIAL_DELALARM_MIXER_AVCAL);
			}
		}

		if(byTemp & CHECKSUMERRORBIT)
		{
			getModel()->getDATAHANDLER()->SetConPICChecksumError(true);
			
			if(getModel()->getALARMHANDLER()->CanSetAlarm_ChecksumConPIC())
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SERIAL_SETALARM_CHECKSUMCONPIC);
			}
		}
		else
		{
			if(	getModel()->getALARMHANDLER()->ALARM_SysFail_ChecksumConPIC->getAlarmState()==AS_ACTIVE)
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SERIAL_DELALARM_CHECKSUMCONPIC);
			}
		}

		if(byTemp & TUBEOCCLUSIONBIT)
		{
			//DEBUGMSG(TRUE, (TEXT("serial TUBEOCCLUSIONBIT\r\n")));
			m_bSerialTUBE_OCCLUSION=true;
			if(getModel()->getALARMHANDLER()->CanSetAlarm_TUBE_OCCLUSION())
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_TUBE_OCCLUSION);
				//DEBUGMSG(TRUE, (TEXT("WM_SETALARM_TUBE_OCCLUSION SERIAL ###\r\n")));
			}
		}
		else
		{
			if((getModel()->getALARMHANDLER()->ALARM_SysAl_TUBE_OCCLUSION->getAlarmState()==AS_ACTIVE)
				&&	(getModel()->IsSPI_TUBE_OCCLUSION()==false))
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SERIAL_DELALARM_TUBE_OCCLUSION);
				//DEBUGMSG(TRUE, (TEXT("WM_SERIAL_DELALARM_TUBE_OCCLUSION SERIAL ###\r\n")));
			}
			m_bSerialTUBE_OCCLUSION=false;
		}
	}
	
	return true;
}



// **************************************************************************
// 
// **************************************************************************
int CInterfaceSerial::GetCOMValue(CTlsBlob bl)
{
	CStringW sToken = bl.GetString();
	sToken = sToken.Right(sToken.GetLength()-1);
	sToken = CTlsString::RemoveLeadingZeroes(sToken);

	int iRes = _wtoi(sToken);

	return iRes;
}

//TEST LOCK SERIAL
//void CInterfaceSerial::lockTest()
//{
//	EnterCriticalSection(&csTestLock);
//
//	//LeaveCriticalSection(&csTestLock);
//}