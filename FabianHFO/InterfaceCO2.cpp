// InterfaceETCO2.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "InterfaceCO2.h"
#include "MVModel.h"
#include "acuLink.h"

#include "InterfaceCO2_CAPNOSTAT.h"
#include "InterfaceCO2_MICROPOD.h"




CInterfaceCO2* CInterfaceCO2::theETCO2Interface=0;
BYTE CInterfaceCO2::m_iCO2module=0;
CList<LPCO2MSG, LPCO2MSG>  CInterfaceCO2::MSGSendETCO2;


// CInterfaceCO2
CInterfaceCO2::CInterfaceCO2()
{
	InitializeCriticalSection(&csMSGSendETCO2);
	InitializeCriticalSection(&csUpdateDataCO2);
	InitializeCriticalSection(&csErrorDataCO2);

	m_com=3;
	m_dwLastCO2Cmd=0;

	//m_iAmbientPressure=0;

	m_iCO2module=0;
	m_bStandby=false;

	m_bInit=true;

	m_pcwtETCO2SendThread=NULL;
	m_hThreadETCO2Send=INVALID_HANDLE_VALUE;
	m_bDoETCO2SendThread=false;

	m_pcwtETCO2InitThread=NULL;
	m_hThreadETCO2Init=INVALID_HANDLE_VALUE;
	m_bDoETCO2InitThread=false;

	m_pcwtETCO2checkThread=NULL;
	m_hThreadETCO2check=INVALID_HANDLE_VALUE;
	m_bDoETCO2checkThread=false;
	
	//m_bInitialized=false;
	m_bStartup=false;
	//m_bConnected=false;

	m_iBPM=0;
	m_iETCO2=0;
	m_iFICO2=0;

	m_szVersion=_T("");

	m_byStateBytes=0xFF;
	m_byExtendedStateBytes=0xFF;

	

	/*m_iMaxOpHRSCalibration=0;
	m_bCalibrationIsDue=false;
	m_iRemainServiceHrs=0;
	m_bServiceIsDue=false;
	m_bSystemDateOk=false;*/
	m_bVersion=false;
	m_bPumpStateOn=true;
	m_bCO2ValueValid=false;
	m_bETCO2ValueValid=false;
	m_bFreqValuevalid=false;
	m_bFICO2ValueValid=false;

	
}

CInterfaceCO2::~CInterfaceCO2()
{

	if(m_pcwtETCO2checkThread!=NULL)
	{
		delete m_pcwtETCO2checkThread;
		m_pcwtETCO2checkThread=NULL;

		if(m_hThreadETCO2check!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadETCO2check);
			m_hThreadETCO2check=INVALID_HANDLE_VALUE;
		}
	}

	if(m_pcwtETCO2SendThread!=NULL)
	{
		delete m_pcwtETCO2SendThread;
		m_pcwtETCO2SendThread=NULL;

		if(m_hThreadETCO2Send!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadETCO2Send);
			m_hThreadETCO2Send=INVALID_HANDLE_VALUE;
		}
	}

	
	if(m_pcwtETCO2InitThread!=NULL)
	{
		delete m_pcwtETCO2InitThread;
		m_pcwtETCO2InitThread=NULL;

		if(m_hThreadETCO2Init!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadETCO2Init);
			m_hThreadETCO2Init=INVALID_HANDLE_VALUE;
		}
	}

	try
	{
		if(MSGSendETCO2.GetCount()>0)
		{
			POSITION pos = MSGSendETCO2.GetHeadPosition();
			if(pos==NULL)
			{

			}
			else
			{
				LPCO2MSG szTemp = MSGSendETCO2.GetAt(pos);
				MSGSendETCO2.RemoveAt(pos);
				delete (void *)szTemp;
			}
		}
		CSerialEx::StopListener(0);
		CSerialEx::Close();
	}
	catch (...)
	{
		CSerialEx::StopListener(0);
		CSerialEx::Close();
	}
	

	

	DeleteCriticalSection(&csMSGSendETCO2);
	DeleteCriticalSection(&csUpdateDataCO2);
	DeleteCriticalSection(&csErrorDataCO2);
}

int CInterfaceCO2::ShowError (LONG lError, LPCTSTR lptszMessage)
{
	// Generate a message text
	TCHAR tszMessage[256];
	wsprintf(tszMessage,_T("#HFO:0111: %s (code %d)"), lptszMessage, lError);

	theApp.getLog()->WriteLine(tszMessage);
	// Display message-box and return with an error-code
	//::MessageBox(0,tszMessage,_T("Listener"), MB_ICONSTOP|MB_OK);
	return 1;
}
CString CInterfaceCO2::get_ModuleVersion()
{
	return m_szVersion;
}
// **************************************************************************
// 
// **************************************************************************
CInterfaceCO2* CInterfaceCO2::GetInstance(BYTE iCO2module)
{
	if(theETCO2Interface == 0)
	{
		m_iCO2module=iCO2module;
		if(m_iCO2module==CO2MODULE_CAPNOSTAT)
			theETCO2Interface = new CInterfaceCO2_CAPNOSTAT;
		else if(m_iCO2module==CO2MODULE_MICROPOD)
			theETCO2Interface = new CInterfaceCO2_MICROPOD;
	}
	return theETCO2Interface;
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceCO2::DestroyInstance()
{
	if(theETCO2Interface != NULL)
	{
		delete theETCO2Interface;
		theETCO2Interface = NULL;
	}
}

//=============================================================================
/**
 * @brief Get the instance of the model (singleton).
 *
 * @return the instance of the model
 *
 **/
//=============================================================================
//CMVModel *CInterfaceCO2::getModel()
//{
//	if(m_pModel==NULL)
//		m_pModel=CMVModel::GetInstance();
//	return m_pModel;
//}
int CInterfaceCO2::get_MaxOpHRSCalibration()
{
	return 0;
}
int CInterfaceCO2::get_RemainServiceHrs()
{
	return 0;
}
 COleDateTime CInterfaceCO2::get_CalibrationDateTime()
{
	COleDateTime dtTimeLastTrend;
	return dtTimeLastTrend;
}
void CInterfaceCO2::get_LastCalDate()
{

}
bool CInterfaceCO2::isPumpOn()
{
	return true;
}
bool CInterfaceCO2::isCO2ValueValid()
{
	return true;
}
bool CInterfaceCO2::isETCO2ValueValid()
{
	return true;
}
bool CInterfaceCO2::isFreqValueValid()
{
	return true;
}
bool CInterfaceCO2::isFICO2ValueValid()
{
	return true;
}
void CInterfaceCO2::changeCO2Unit(eCO2unit unitNew,eCO2unit unitOld)
{

}
void CInterfaceCO2::CheckModuleState()
{

}
void CInterfaceCO2::turnOff()
{

}
void CInterfaceCO2::set_GasCompensation(BYTE byO2Compensation, BYTE byBalanceGas, SHORT iAnestAgent)
{

}
void CInterfaceCO2::get_GasCompensation()
{

}
void CInterfaceCO2::set_BaroPressure(SHORT val)
{

}
void CInterfaceCO2::get_BaroPressure()
{

}
void CInterfaceCO2::set_PatientMode(BYTE val)
{

}
void CInterfaceCO2::get_PatientMode()
{

}
void CInterfaceCO2::set_MeasurementMode(BYTE val)
{

}
void CInterfaceCO2::get_MeasurementMode()
{

}
void CInterfaceCO2::set_PumpStateOn(bool bGetExState)
{

}
void CInterfaceCO2::set_PumpStateOff(bool bGetExState)
{

}
void CInterfaceCO2::set_Configuration()
{

}
void CInterfaceCO2::set_StandbyMode(bool state)
{

}
bool CInterfaceCO2::get_StandbyMode()
{
	return false;
}
void CInterfaceCO2::set_restartBreathAlgorithm()
{

}

bool CInterfaceCO2::isCalibrationDue()
{
	return false;
}
bool CInterfaceCO2::isServiceDue()
{
	return false;
}
bool CInterfaceCO2::isSystemDateOk()
{
	return true;
}
void CInterfaceCO2::retryConnection()
{
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_RETRYCO2);
}
void CInterfaceCO2::zeroCalibration()

{
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceCO2::startETCO2Thread(void)
{
	//###########send thread####################
	m_bDoETCO2SendThread=true;

	if(m_pcwtETCO2SendThread!=NULL)
	{
		delete m_pcwtETCO2SendThread;
		m_pcwtETCO2SendThread=NULL;

		if(m_hThreadETCO2Send!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadETCO2Send);
			m_hThreadETCO2Send=INVALID_HANDLE_VALUE;
		}
	}

	m_pcwtETCO2SendThread=AfxBeginThread(CETCO2SendThread,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	m_hThreadETCO2Send=m_pcwtETCO2SendThread->m_hThread;
	m_pcwtETCO2SendThread->m_bAutoDelete = FALSE; 
	m_pcwtETCO2SendThread->ResumeThread();

	//###########initialization thread####################
	m_bDoETCO2InitThread=true;

	if(m_pcwtETCO2InitThread!=NULL)
	{
		delete m_pcwtETCO2InitThread;
		m_pcwtETCO2InitThread=NULL;

		if(m_hThreadETCO2Init!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadETCO2Init);
			m_hThreadETCO2Init=INVALID_HANDLE_VALUE;
		}
	}

	m_pcwtETCO2InitThread=AfxBeginThread(CETCO2InitThread,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	m_hThreadETCO2Init=m_pcwtETCO2InitThread->m_hThread;
	m_pcwtETCO2InitThread->m_bAutoDelete = FALSE; 
	m_pcwtETCO2InitThread->ResumeThread();
}

//************************************
// Method:    stopETCO2Thread
// FullName:  CInterfaceCO2::stopETCO2Thread
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: void
//
// 2015/06/19: checked for correct closing of thread
//************************************
void CInterfaceCO2::stopETCO2Thread(void)
{
	if(m_bDoETCO2SendThread)
	{
		m_bDoETCO2SendThread=false;

		g_eventETCO2SendData.SetEvent();

		if (WaitForSingleObject(m_pcwtETCO2SendThread->m_hThread,500) == WAIT_TIMEOUT)
		{
			theApp.getLog()->WriteLine(_T("#THR:006a"));
			if(!TerminateThread(m_pcwtETCO2SendThread,0))
			{
				theApp.getLog()->WriteLine(_T("#THR:006b"));
			}
		}
	}

	if(m_bDoETCO2InitThread)
	{
		m_bDoETCO2InitThread=false;

		if (WaitForSingleObject(m_pcwtETCO2InitThread->m_hThread,2000) == WAIT_TIMEOUT)
		{
			theApp.getLog()->WriteLine(_T("#THR:007a"));
			if(!TerminateThread(m_pcwtETCO2InitThread,0))
			{
				theApp.getLog()->WriteLine(_T("#THR:007b"));
			}
		}
	}

	int iCnt=MSGSendETCO2.GetCount();
	while(iCnt>0)
	{
		POSITION pos = MSGSendETCO2.GetHeadPosition();
		if(pos==NULL)
		{
			iCnt=0;
		}
		else
		{
			LPCO2MSG szTemp = MSGSendETCO2.GetAt(pos);
			MSGSendETCO2.RemoveAt(pos);
			delete (void *)szTemp;

			iCnt=MSGSendETCO2.GetCount();
		}
		Sleep(0);
	}
	/*if(MSGSendETCO2.GetCount()>0)
	{
		POSITION pos = MSGSendETCO2.GetHeadPosition();
		if(pos==NULL)
		{

		}
		else
		{
			LPCO2MSG szTemp = MSGSendETCO2.GetAt(pos);
			MSGSendETCO2.RemoveAt(pos);
			delete (void *)szTemp;
		}
	}*/
}


// **************************************************************************
// 
// **************************************************************************
void CInterfaceCO2::startETCO2checkThread(void)
{
	//###########check thread####################
	m_bDoETCO2checkThread=true;

	if(m_pcwtETCO2checkThread!=NULL)
	{
		delete m_pcwtETCO2checkThread;
		m_pcwtETCO2checkThread=NULL;

		if(m_hThreadETCO2check!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadETCO2check);
			m_hThreadETCO2check=INVALID_HANDLE_VALUE;
		}
	}

	m_pcwtETCO2checkThread=AfxBeginThread(CETCO2checkThread,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	m_hThreadETCO2check=m_pcwtETCO2checkThread->m_hThread;
	m_pcwtETCO2checkThread->m_bAutoDelete = FALSE; 
	m_pcwtETCO2checkThread->ResumeThread();
}

//************************************
// Method:    stopETCO2checkThread
// FullName:  CInterfaceCO2::stopETCO2checkThread
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: void
//
// 2015/06/19: checked for correct closing of thread
//************************************
void CInterfaceCO2::stopETCO2checkThread(void)
{
	if(m_bDoETCO2checkThread)
	{
		m_bDoETCO2checkThread=false;
		g_eventCO2CheckData.SetEvent();

		if(m_pcwtETCO2checkThread!=NULL)
		{
			if (WaitForSingleObject(m_pcwtETCO2checkThread->m_hThread,500) == WAIT_TIMEOUT)
			{
				theApp.getLog()->WriteLine(_T("#THR:008a"));
				if(!TerminateThread(m_pcwtETCO2checkThread,0))
				{
					theApp.getLog()->WriteLine(_T("#THR:008b"));
				}
			}
		}
	}
}

//#####################################SendThread########################################
//************************************
// Method:    CETCO2SendThread
// FullName:  CETCO2SendThread
// Access:    public static 
// Returns:   UINT
// Qualifier:
// Parameter: LPVOID pc
//
// 2015/06/19: checked for correct closing of thread
//************************************
static UINT CETCO2SendThread( LPVOID pc )
{
	((CInterfaceCO2*)pc)->SendETCO2Data();
	return TRUE;
}

//************************************
// Method:    SendETCO2Data
// FullName:  CInterfaceCO2::SendETCO2Data
// Access:    protected 
// Returns:   DWORD
// Qualifier:
// Parameter: void
//
// 2015/06/19: checked for correct closing of thread
//************************************
DWORD CInterfaceCO2::SendETCO2Data(void) 
{
	CeSetThreadPriority(m_pcwtETCO2SendThread->m_hThread,130);

	WORD iCnt=0;
	while(iCnt<20 && m_bDoETCO2SendThread)
	{
		Sleep(50);
		iCnt++;
	}

	if(!m_bDoETCO2SendThread)
	{
		return 0;
	}

	bool bWrite=false;
	do
	{
		DWORD dw = ::WaitForSingleObject(g_eventETCO2SendData, INFINITE);

		if(!m_bDoETCO2SendThread)
			continue;

		EnterCriticalSection(&csMSGSendETCO2);
		POSITION pos = MSGSendETCO2.GetHeadPosition();
		LeaveCriticalSection(&csMSGSendETCO2);
		while(pos!=NULL && m_bDoETCO2SendThread)
		{

			EnterCriticalSection(&csMSGSendETCO2);
			LPCO2MSG szTemp = MSGSendETCO2.GetAt(pos);
			Write(szTemp->byBuffer, szTemp->bySize);
			MSGSendETCO2.RemoveAt(pos);
			delete (void *)szTemp;

			pos = MSGSendETCO2.GetHeadPosition();
			LeaveCriticalSection(&csMSGSendETCO2);
		}

	}while(m_bDoETCO2SendThread);

	theApp.getLog()->WriteLine(_T("#THR:006"));
	
	return 0;
}


//#####################################CheckThread########################################
//************************************
// Method:    CETCO2checkThread
// FullName:  CETCO2checkThread
// Access:    public static 
// Returns:   UINT
// Qualifier:
// Parameter: LPVOID pc
//
// 2015/06/19: checked for correct closing of thread
//************************************
static UINT CETCO2checkThread( LPVOID pc )
{
	((CInterfaceCO2*)pc)->ETCO2check();
	return TRUE;
}

//************************************
// Method:    ETCO2check
// FullName:  CInterfaceCO2::ETCO2check
// Access:    protected 
// Returns:   DWORD
// Qualifier:
// Parameter: void
//
// 2015/06/19: checked for correct closing of thread
//************************************
DWORD CInterfaceCO2::ETCO2check(void) 
{
	CeSetThreadPriority(m_pcwtETCO2checkThread->m_hThread,130);


	WORD iCnt=0;
	
	if(!m_bDoETCO2checkThread)
	{
		return 0;
	}

	DWORD dwWait=CHECKETCO2;

	do
	{
		DWORD dw = ::WaitForSingleObject(g_eventCO2CheckData, dwWait);
		if(m_bDoETCO2checkThread)
			CheckModuleState();
	}while(m_bDoETCO2checkThread);

	theApp.getLog()->WriteLine(_T("#THR:008"));
	
	return 0;
}

//#############################################################################
//************************************
// Method:    CETCO2InitThread
// FullName:  CETCO2InitThread
// Access:    public static 
// Returns:   UINT
// Qualifier:
// Parameter: LPVOID pc
//
// 2015/06/19: checked for correct closing of thread
//************************************
static UINT CETCO2InitThread( LPVOID pc )
{
	((CInterfaceCO2*)pc)->ETCO2Init();
	return TRUE;
}

//************************************
// Method:    ETCO2Init
// FullName:  CInterfaceCO2::ETCO2Init
// Access:    protected 
// Returns:   DWORD
// Qualifier:
// Parameter: void
//
// 2015/06/19: checked for correct closing of thread
//************************************
DWORD CInterfaceCO2::ETCO2Init(void) 
{
	CeSetThreadPriority(m_pcwtETCO2InitThread->m_hThread,130);

	m_bInit=true;
	m_bStartup=false;
	WORD iCnt=0;
	BYTE iInitCount=0;

	if(!m_bDoETCO2InitThread)
	{
		return 0;
	}

	do
	{
		if(!m_bDoETCO2InitThread)
			continue;

		StartupSequenz();
		
		iCnt=0;
		while(iCnt<200 && m_bDoETCO2InitThread && m_bStartup==false)
		{
			Sleep(10);
			iCnt++;
		}

		if(m_bInit)
		{
			m_bInit=false;
			getModel()->setCO2inprogress(false);
		}

		iInitCount++;
		if(iInitCount>=10)
		{
			iInitCount=0;
			if(getModel()->getALARMHANDLER()->CanSetAlarm_CO2_MODULE_NOTCONNECTED())
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_CO2_MODULE_NOTCONNECTED);
			}
		}
	}while(m_bDoETCO2InitThread && m_bStartup==false);

	if(m_bStartup)
	{
		getModel()->setCO2inprogress(false);
		InitializeSequenz();//command 84h
	}

	theApp.getLog()->WriteLine(_T("#THR:007"));

	return 0;
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceCO2::send(LPCO2MSG msg)
{
	EnterCriticalSection(&csMSGSendETCO2);

	MSGSendETCO2.AddTail(msg);

	LeaveCriticalSection(&csMSGSendETCO2);

	g_eventETCO2SendData.SetEvent();

	/*CStringW szTemp=_T("");
	szTemp.Format(_T("%X,%X,%X,%X,%X,%X,%X,%X,%X,%X,%X,%X,%X size: %d\n"), 
		msg->byBuffer[0],msg->byBuffer[1],msg->byBuffer[2],msg->byBuffer[3],
		msg->byBuffer[4],msg->byBuffer[5],msg->byBuffer[6],msg->byBuffer[7],
		msg->byBuffer[8],msg->byBuffer[9],msg->byBuffer[10],msg->byBuffer[11],
		msg->byBuffer[12],msg->bySize);




	*/
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceCO2::UpdateLastCO2Cmd()
{
	EnterCriticalSection(&csUpdateDataCO2);
	m_dwLastCO2Cmd=GetTickCount();
	LeaveCriticalSection(&csUpdateDataCO2);


}
void CInterfaceCO2::CheckLastCO2Cmd()
{
	EnterCriticalSection(&csUpdateDataCO2);
	DWORD temp=m_dwLastCO2Cmd;
	LeaveCriticalSection(&csUpdateDataCO2);

	if(isSafeTickCountDelayExpired(temp,CO2SENSORCHECKTIME))
	{
		if(getModel()->getALARMHANDLER()->ALARM_Sens_CO2_MODULE_NOTCONNECTED->getAlarmState()==AS_NONE)
		{
			//if(m_bTryOnceMore)
			{
				//DEBUGMSG(TRUE, (TEXT("WM_SETALARM_CO2_MODULE_NOTCONNECTED\r\n")));
				//m_bTryOnceMore=false;
				m_bETCO2ValueValid=false;
				m_bFICO2ValueValid=false;
				m_bFreqValuevalid=false;
				m_bCO2ValueValid=false;
				m_bPumpStateOn=true;
				//theApp.getLog()->WriteLine(_T("###CO2_MODULE_NOTCONNECTED"));
				//theApp.getLog()->SaveLog();
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_CO2_MODULE_NOTCONNECTED);

				getExtendedStateBytes();

				if(m_byStateBytes!=0xFF || m_byExtendedStateBytes!=0xFF)
				{
					m_byStateBytes=0xFF;
					m_byExtendedStateBytes=0xFF;
					g_eventCO2Data.SetEvent();

					//set_restartBreathAlgorithm();
					set_Configuration();
				}
				retryConnection();
			}
			//else
			//{
			//	DEBUGMSG(TRUE, (TEXT("WM_REINIT_CO2_MODULE\r\n")));

			//	theApp.getLog()->WriteLine(_T("####retryCO2module"));
			//	theApp.getLog()->SaveLog();
			//	m_bTryOnceMore=true;
			//	/*UpdateLastCO2Cmd();
			//	*/

			//	/*if(AfxGetApp())
			//		AfxGetApp()->GetMainWnd()->PostMessage(WM_REINIT_CO2_MODULE);*/

			//	retryConnection();
			//}
		}
	}
	else if(getModel()->getALARMHANDLER()->ALARM_Sens_CO2_MODULE_NOTCONNECTED->getAlarmState()==AS_ACTIVE)
	{
		getModel()->getALARMHANDLER()->deleteAlarm(AL_Sens_CO2_MODULE_NOTCONNECTED);
		set_Configuration();

		m_bFICO2ValueValid=false;
		m_bETCO2ValueValid=false;
		m_bFreqValuevalid=false;
		m_bCO2ValueValid=false;
		m_bPumpStateOn=true;
		/*if(if(m_byStateBytes==0xFF || m_byExtendedStateBytes==0xFF))
		{
			m_bConnected=true;
			g_eventCO2Data.SetEvent();
		}*/
	}
}

//bool CInterfaceCO2::IsConnected()
//{
//	return m_bConnected;
//}

//bool CInterfaceCO2::writeBufferStarted()
//{
//	return m_bStartWriteBuffer;
//}