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

/**********************************************************************************************//**
 * CInterfaceCO2
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Finalizes an instance of the CInterfaceCO2 class
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

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
		theApp.ReportException(_T("CInterfaceCO2::~CInterfaceCO2()"));

		CSerialEx::StopListener(0);
		CSerialEx::Close();
	}
	

	

	DeleteCriticalSection(&csMSGSendETCO2);
	DeleteCriticalSection(&csUpdateDataCO2);
	DeleteCriticalSection(&csErrorDataCO2);
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

int CInterfaceCO2::ShowError (LONG lError, LPCTSTR lptszMessage)
{
	// Generate a message text
	TCHAR tszMessage[256];
	wsprintf(tszMessage,_T("#HFO:0111: %s (code %d)"), lptszMessage, lError);

	theApp.WriteLog(tszMessage);
	// Display message-box and return with an error-code
	//::MessageBox(0,tszMessage,_T("Listener"), MB_ICONSTOP|MB_OK);
	return 1;
}

/**********************************************************************************************//**
 * Gets module version
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The module version.
 **************************************************************************************************/

CString CInterfaceCO2::get_ModuleVersion()
{
	return m_szVersion;
}

/**********************************************************************************************//**
 * Gets an instance
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	iCO2module	Zero-based index of the co2module.
 *
 * \return	Null if it fails, else the instance.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Destroys the instance
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CInterfaceCO2::DestroyInstance()
{
	if(theETCO2Interface != NULL)
	{
		delete theETCO2Interface;
		theETCO2Interface = NULL;
	}
}

/**********************************************************************************************//**
 * Gets maximum operation hrs calibration
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The maximum operation hrs calibration.
 **************************************************************************************************/

int CInterfaceCO2::get_MaxOpHRSCalibration()
{
	return 0;
}

/**********************************************************************************************//**
 * Gets remain service hrs
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The remain service hrs.
 **************************************************************************************************/

int CInterfaceCO2::get_RemainServiceHrs()
{
	return 0;
}

 /**********************************************************************************************//**
  * Gets calibration date time
  *
  * \author	Rainer Kühner
  * \date	21.02.2018
  *
  * \return	The calibration date time.
  **************************************************************************************************/

 COleDateTime CInterfaceCO2::get_CalibrationDateTime()
{
	COleDateTime dtTimeLastTrend;
	return dtTimeLastTrend;
}

/**********************************************************************************************//**
 * Gets the last calibration date
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CInterfaceCO2::get_LastCalDate()
{

}

/**********************************************************************************************//**
 * Query if this instance is pump on
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if pump on, false if not.
 **************************************************************************************************/

bool CInterfaceCO2::isPumpOn()
{
	return true;
}

/**********************************************************************************************//**
 * Query if this instance is co2 value valid
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if the co2 value is valid, false if not.
 **************************************************************************************************/

bool CInterfaceCO2::isCO2ValueValid()
{
	return true;
}

/**********************************************************************************************//**
 * Query if this instance is etco2 value valid
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if the etco2 value is valid, false if not.
 **************************************************************************************************/

bool CInterfaceCO2::isETCO2ValueValid()
{
	return true;
}

/**********************************************************************************************//**
 * Query if this instance is frequency value valid
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if the frequency value is valid, false if not.
 **************************************************************************************************/

bool CInterfaceCO2::isFreqValueValid()
{
	return true;
}

/**********************************************************************************************//**
 * Query if this instance is fico2 value valid
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if the fico2 value is valid, false if not.
 **************************************************************************************************/

bool CInterfaceCO2::isFICO2ValueValid()
{
	return true;
}

/**********************************************************************************************//**
 * Change co2 unit
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	unitNew	The unit new.
 * \param	unitOld	The unit old.
 **************************************************************************************************/

void CInterfaceCO2::changeCO2Unit(eCO2unit, eCO2unit)
{

}

/**********************************************************************************************//**
 * Check module state
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CInterfaceCO2::CheckModuleState()
{

}

/**********************************************************************************************//**
 * Turn off
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CInterfaceCO2::turnOff()
{

}

/**********************************************************************************************//**
 * Sets gas compensation
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	byO2Compensation	The by o 2 compensation.
 * \param	byBalanceGas		The by balance gas.
 * \param	iAnestAgent			Zero-based index of the anest agent.
 **************************************************************************************************/

void CInterfaceCO2::set_GasCompensation(BYTE, BYTE, SHORT)
{

}

/**********************************************************************************************//**
 * Gets gas compensation
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CInterfaceCO2::get_GasCompensation()
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

void CInterfaceCO2::set_BaroPressure(SHORT)
{

}

/**********************************************************************************************//**
 * Gets baro pressure
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CInterfaceCO2::get_BaroPressure()
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

void CInterfaceCO2::set_PatientMode(BYTE)
{

}

/**********************************************************************************************//**
 * Gets patient mode
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CInterfaceCO2::get_PatientMode()
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

void CInterfaceCO2::set_MeasurementMode(BYTE)
{

}

/**********************************************************************************************//**
 * Gets measurement mode
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CInterfaceCO2::get_MeasurementMode()
{

}

/**********************************************************************************************//**
 * Sets pump state on
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	bGetExState	True to get ex state.
 **************************************************************************************************/

void CInterfaceCO2::set_PumpStateOn(bool)
{

}

/**********************************************************************************************//**
 * Sets pump state off
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	bGetExState	True to get ex state.
 **************************************************************************************************/

void CInterfaceCO2::set_PumpStateOff(bool)
{

}

/**********************************************************************************************//**
 * Sets the configuration
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CInterfaceCO2::set_Configuration()
{

}

/**********************************************************************************************//**
 * Sets standby mode
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	state	True to state.
 **************************************************************************************************/

void CInterfaceCO2::set_StandbyMode(bool)
{

}

/**********************************************************************************************//**
 * Gets standby mode
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CInterfaceCO2::get_StandbyMode()
{
	return false;
}

/**********************************************************************************************//**
 * Sets restart breath algorithm
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CInterfaceCO2::set_restartBreathAlgorithm()
{

}

/**********************************************************************************************//**
 * Query if this instance is calibration due
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if calibration due, false if not.
 **************************************************************************************************/

bool CInterfaceCO2::isCalibrationDue()
{
	return false;
}

/**********************************************************************************************//**
 * Query if this instance is service due
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if service due, false if not.
 **************************************************************************************************/

bool CInterfaceCO2::isServiceDue()
{
	return false;
}

/**********************************************************************************************//**
 * Query if this instance is system date ok
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if system date ok, false if not.
 **************************************************************************************************/

bool CInterfaceCO2::isSystemDateOk()
{
	return true;
}

/**********************************************************************************************//**
 * Retry connection
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CInterfaceCO2::retryConnection()
{
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_RETRYCO2);
}

/**********************************************************************************************//**
 * Zero calibration
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CInterfaceCO2::zeroCalibration()

{
}

/**********************************************************************************************//**
 * Starts etco2 thread
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Stops etco2 thread
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CInterfaceCO2::stopETCO2Thread(void)
{
	if(m_bDoETCO2SendThread)
	{
		m_bDoETCO2SendThread=false;

		g_eventETCO2SendData.SetEvent();

		if (WaitForSingleObject(m_pcwtETCO2SendThread->m_hThread,500) == WAIT_TIMEOUT)
		{
			theApp.WriteLog(_T("#THR:006a"));
			if(!TerminateThread(m_pcwtETCO2SendThread,0))
			{
				theApp.WriteLog(_T("#THR:006b"));
			}
		}
	}

	if(m_bDoETCO2InitThread)
	{
		m_bDoETCO2InitThread=false;

		if (WaitForSingleObject(m_pcwtETCO2InitThread->m_hThread,2000) == WAIT_TIMEOUT)
		{
			theApp.WriteLog(_T("#THR:007a"));
			if(!TerminateThread(m_pcwtETCO2InitThread,0))
			{
				theApp.WriteLog(_T("#THR:007b"));
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

/**********************************************************************************************//**
 * Starts etco2check thread
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Stops etco2check thread
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

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
				theApp.WriteLog(_T("#THR:008a"));
				if(!TerminateThread(m_pcwtETCO2checkThread,0))
				{
					theApp.WriteLog(_T("#THR:008b"));
				}
			}
		}
	}
}

/**********************************************************************************************//**
 * etco2 send thread
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	pc	The PC.
 *
 * \return	An UINT.
 **************************************************************************************************/

static UINT CETCO2SendThread( LPVOID pc )
{
	try
	{
		((CInterfaceCO2*)pc)->SendETCO2Data();
	}
	catch (CException* e)
	{
		TCHAR   szCause[255];
		e->GetErrorMessage(szCause, 255);

		CString errorStr=_T("");
		errorStr.Format(_T("CETCO2SendThread: %s"),szCause);

		theApp.ReportException(errorStr);

		e->Delete();
	}
	catch(...)
	{
		theApp.ReportException(_T("CETCO2SendThread"));

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_EXCEPTION);
	}

	//((CInterfaceCO2*)pc)->SendETCO2Data();
	return TRUE;
}

/**********************************************************************************************//**
 * Sends the etco2 data
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	A DWORD.
 **************************************************************************************************/

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

	do
	{
		::WaitForSingleObject(g_eventETCO2SendData, INFINITE);

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

	//theApp.WriteLog(_T("#THR:006"));
	
	return 0;
}


//#####################################CheckThread########################################

/**********************************************************************************************//**
 * etco2check thread
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	pc	The PC.
 *
 * \return	An UINT.
 **************************************************************************************************/

static UINT CETCO2checkThread( LPVOID pc )
{
	try
	{
		((CInterfaceCO2*)pc)->ETCO2check();
	}
	catch (CException* e)
	{
		TCHAR   szCause[255];
		e->GetErrorMessage(szCause, 255);

		CString errorStr=_T("");
		errorStr.Format(_T("CETCO2checkThread: %s"),szCause);

		theApp.ReportException(errorStr);

		e->Delete();
	}
	catch(...)
	{
		theApp.ReportException(_T("CETCO2checkThread"));

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_EXCEPTION);
	}

	//((CInterfaceCO2*)pc)->ETCO2check();
	return TRUE;
}

/**********************************************************************************************//**
 * Etco2check
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	A DWORD.
 **************************************************************************************************/

DWORD CInterfaceCO2::ETCO2check(void) 
{
	CeSetThreadPriority(m_pcwtETCO2checkThread->m_hThread,130);


	if(!m_bDoETCO2checkThread)
	{
		return 0;
	}

	DWORD dwWait=CHECKETCO2;

	do
	{
		::WaitForSingleObject(g_eventCO2CheckData, dwWait);
		if(m_bDoETCO2checkThread)
			CheckModuleState();
	}while(m_bDoETCO2checkThread);

	//theApp.WriteLog(_T("#THR:008"));
	
	return 0;
}

/**********************************************************************************************//**
 * etco2 initialize thread
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	pc	The PC.
 *
 * \return	An UINT.
 **************************************************************************************************/

static UINT CETCO2InitThread( LPVOID pc )
{
	try
	{
		((CInterfaceCO2*)pc)->ETCO2Init();
	}
	catch (CException* e)
	{
		TCHAR   szCause[255];
		e->GetErrorMessage(szCause, 255);

		CString errorStr=_T("");
		errorStr.Format(_T("CETCO2InitThread: %s"),szCause);

		theApp.ReportException(errorStr);

		e->Delete();
	}
	catch(...)
	{
		theApp.ReportException(_T("CETCO2InitThread"));

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_EXCEPTION);
	}

	//((CInterfaceCO2*)pc)->ETCO2Init();
	return TRUE;
}

/**********************************************************************************************//**
 * Etco2 initialize
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	A DWORD.
 **************************************************************************************************/

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

	//theApp.WriteLog(_T("#THR:007"));

	return 0;
}

/**********************************************************************************************//**
 * Send this message
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	msg	The message.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Updates the last co2 command
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CInterfaceCO2::UpdateLastCO2Cmd()
{
	EnterCriticalSection(&csUpdateDataCO2);
	m_dwLastCO2Cmd=GetTickCount();
	LeaveCriticalSection(&csUpdateDataCO2);


}

/**********************************************************************************************//**
 * Check last co2 command
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

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
				//theApp.WriteLog(_T("###CO2_MODULE_NOTCONNECTED"));
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

			//	theApp.WriteLog(_T("####retryCO2module"));
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

