#include "StdAfx.h"
#include "FabianHFO.h"
#include "InterfaceSPO2.h"
#include "MVModel.h"
#include "acuLink.h"

#include "InterfaceMasimoSPO2.h"

CInterfaceSPO2* CInterfaceSPO2::theSPO2Interface=0;
BYTE CInterfaceSPO2::m_iSPO2module=0;
CList<LPSPO2MSG, LPSPO2MSG>  CInterfaceSPO2::MSGSendSPO2;


CInterfaceSPO2::CInterfaceSPO2(void)
{
	InitializeCriticalSection(&csMSGSendSPO2);
	InitializeCriticalSection(&csUpdateDataSpO2);
	//InitializeCriticalSection(&csErrorDataCO2);

	m_iSpO2=0;
	m_iPulseRate=0;
	m_iPerfusionIndex=0;
	m_bSensorConnectedToUSB=false;
	m_bSensorConnectedToModule=true;
	m_iSPO2module=0;
	m_bStartup=false;

	m_pcwtSPO2SendThread=NULL;
	m_hThreadSPO2Send=INVALID_HANDLE_VALUE;
	m_bDoSPO2SendThread=false;

	m_pcwtSPO2InitThread=NULL;
	m_hThreadSPO2Init=INVALID_HANDLE_VALUE;
	m_bDoSPO2InitThread=false;

	m_pcwtSPO2checkThread=NULL;
	m_hThreadSPO2check=INVALID_HANDLE_VALUE;
	m_bDoSPO2checkThread=false;

	m_dwLastSpO2Cmd=0;
}

CInterfaceSPO2::~CInterfaceSPO2(void)
{
	if(m_pcwtSPO2checkThread!=NULL)
	{
		delete m_pcwtSPO2checkThread;
		m_pcwtSPO2checkThread=NULL;

		if(m_hThreadSPO2check!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadSPO2check);
			m_hThreadSPO2check=INVALID_HANDLE_VALUE;
		}
	}

	if(m_pcwtSPO2SendThread!=NULL)
	{
		delete m_pcwtSPO2SendThread;
		m_pcwtSPO2SendThread=NULL;

		if(m_hThreadSPO2Send!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadSPO2Send);
			m_hThreadSPO2Send=INVALID_HANDLE_VALUE;
		}
	}


	if(m_pcwtSPO2InitThread!=NULL)
	{
		delete m_pcwtSPO2InitThread;
		m_pcwtSPO2InitThread=NULL;

		if(m_hThreadSPO2Init!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadSPO2Init);
			m_hThreadSPO2Init=INVALID_HANDLE_VALUE;
		}
	}

	if(MSGSendSPO2.GetCount()>0)
	{
		POSITION pos = MSGSendSPO2.GetHeadPosition();
		if(pos==NULL)
		{

		}
		else
		{
			LPSPO2MSG szTemp = MSGSendSPO2.GetAt(pos);
			MSGSendSPO2.RemoveAt(pos);
			delete (void *)szTemp;
		}
	}

	StopListener(-1);
	Close();

	DeleteCriticalSection(&csMSGSendSPO2);
	DeleteCriticalSection(&csUpdateDataSpO2);
	//DeleteCriticalSection(&csErrorDataSPO2);
}

// **************************************************************************
// 
// **************************************************************************
CInterfaceSPO2* CInterfaceSPO2::GetInstance(BYTE iSPO2module)
{
	if(theSPO2Interface == 0)
	{
		m_iSPO2module=iSPO2module;
		if(m_iSPO2module==SPO2MODULE_MASIMO)
			theSPO2Interface = new CInterfaceMasimoSPO2;
		/*else if(m_iCO2module==CO2MODULE_MICROPOD)
			theETCO2Interface = new CInterfaceCO2_MICROPOD;*/
	}
	return theSPO2Interface;
}

WORD CInterfaceSPO2::get_ProductID()
{
	return 0;
}
WORD CInterfaceSPO2::get_VersionMCU()
{
	return 0;
}
WORD CInterfaceSPO2::get_VersionDSP()
{
	return 0;
}
SHORT CInterfaceSPO2::get_EXCEPTION2()
{
	return 0;
}
void CInterfaceSPO2::checkModuleState()
{

}
void CInterfaceSPO2::set_FastSATon(bool bFastSATon)
{

}
void CInterfaceSPO2::set_SensitivityMode(eSPO2sensitivitymode mode)
{

}
void CInterfaceSPO2::set_SPO2averagingTime(eSPO2averagingtime avtime)
{

}
void CInterfaceSPO2::set_SPO2alarmDelay(eSPO2alarmdelay delay)
{

}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceSPO2::DestroyInstance()
{
	if(theSPO2Interface != NULL)
	{
		delete theSPO2Interface;
		theSPO2Interface = NULL;
	}
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceSPO2::startSPO2initThread(void)
{
	try
	{
		//###########initialization thread####################
		m_bDoSPO2InitThread=true;

		if(m_pcwtSPO2InitThread!=NULL)
		{
			delete m_pcwtSPO2InitThread;
			m_pcwtSPO2InitThread=NULL;

			if(m_hThreadSPO2Init!=INVALID_HANDLE_VALUE)
			{
				CloseHandle(m_hThreadSPO2Init);
				m_hThreadSPO2Init=INVALID_HANDLE_VALUE;
			}
		}

		m_pcwtSPO2InitThread=AfxBeginThread(CSPO2InitThread,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
		m_hThreadSPO2Init=m_pcwtSPO2InitThread->m_hThread;
		m_pcwtSPO2InitThread->m_bAutoDelete = FALSE; 
		m_pcwtSPO2InitThread->ResumeThread();
	}
	catch (...)
	{
		theApp.ReportException(_T("THR:SPO2 startSPO2initThread"));

		if(m_pcwtSPO2InitThread!=NULL)
		{
			delete m_pcwtSPO2InitThread;
			m_pcwtSPO2InitThread=NULL;

			if(m_hThreadSPO2Init!=INVALID_HANDLE_VALUE)
			{
				CloseHandle(m_hThreadSPO2Init);
				m_hThreadSPO2Init=INVALID_HANDLE_VALUE;
			}
		}

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_REINIT_SPO2_MODULE);
	}
	
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceSPO2::stopSPO2initThread(void)
{

	if(m_bDoSPO2InitThread)
	{
		m_bDoSPO2InitThread=false;

		g_eventCO2Startup.SetEvent();

		if (WaitForSingleObject(m_pcwtSPO2InitThread->m_hThread,2000) == WAIT_TIMEOUT)
		{
			//theApp.getLog()->WriteLine(_T("#HFO:0211"));
			theApp.getLog()->WriteLine(_T("#THR:016a"));
			if(!TerminateThread(m_pcwtSPO2InitThread,0))
			{
				theApp.getLog()->WriteLine(_T("#THR:016b"));
			}
		}
	}
}


// **************************************************************************
// 
// **************************************************************************
void CInterfaceSPO2::startSPO2sendThread(void)
{
	//###########send thread####################
	m_bDoSPO2SendThread=true;

	if(m_pcwtSPO2SendThread!=NULL)
	{
		delete m_pcwtSPO2SendThread;
		m_pcwtSPO2SendThread=NULL;

		if(m_hThreadSPO2Send!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadSPO2Send);
			m_hThreadSPO2Send=INVALID_HANDLE_VALUE;
		}
	}

	m_pcwtSPO2SendThread=AfxBeginThread(CSPO2SendThread,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	m_hThreadSPO2Send=m_pcwtSPO2SendThread->m_hThread;
	m_pcwtSPO2SendThread->m_bAutoDelete = FALSE; 
	m_pcwtSPO2SendThread->ResumeThread();
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceSPO2::stopSPO2sendThread(void)
{
	if(m_bDoSPO2SendThread)
	{
		m_bDoSPO2SendThread=false;

		g_eventSPO2SendData.SetEvent();

		if (WaitForSingleObject(m_pcwtSPO2SendThread->m_hThread,500) == WAIT_TIMEOUT)
		{
			//theApp.getLog()->WriteLine(_T("#HFO:0210"));
			theApp.getLog()->WriteLine(_T("#THR:015a"));
			if(!TerminateThread(m_pcwtSPO2SendThread,0))
			{
				theApp.getLog()->WriteLine(_T("#THR:015b"));
			}
		}
	}

	int iCnt=MSGSendSPO2.GetCount();
	//DEBUGMSG(TRUE, (TEXT("MSGSendSPO2 count %d\r\n"),iCnt));
	while(iCnt>0)
	{
		POSITION pos = MSGSendSPO2.GetHeadPosition();
		if(pos==NULL)
		{
			iCnt=0;
		}
		else
		{
			LPSPO2MSG szTemp = MSGSendSPO2.GetAt(pos);
			MSGSendSPO2.RemoveAt(pos);
			delete (void *)szTemp;

			iCnt=MSGSendSPO2.GetCount();
		}
		Sleep(0);
	}
	/*if(MSGSendSPO2.GetCount()>0)
	{
		POSITION pos = MSGSendSPO2.GetHeadPosition();
		if(pos==NULL)
		{

		}
		else
		{
			LPSPO2MSG szTemp = MSGSendSPO2.GetAt(pos);
			MSGSendSPO2.RemoveAt(pos);
			delete (void *)szTemp;
		}
	}*/


}
// **************************************************************************
// 
// **************************************************************************
void CInterfaceSPO2::startSPO2checkThread(void)
{
	//###########check thread####################
	m_bDoSPO2checkThread=true;

	if(m_pcwtSPO2checkThread!=NULL)
	{
		delete m_pcwtSPO2checkThread;
		m_pcwtSPO2checkThread=NULL;

		if(m_hThreadSPO2check!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadSPO2check);
			m_hThreadSPO2check=INVALID_HANDLE_VALUE;
		}
	}

	m_pcwtSPO2checkThread=AfxBeginThread(CSPO2checkThread,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	m_hThreadSPO2check=m_pcwtSPO2checkThread->m_hThread;
	m_pcwtSPO2checkThread->m_bAutoDelete = FALSE; 
	m_pcwtSPO2checkThread->ResumeThread();
}

void CInterfaceSPO2::stopSPO2checkThread(void)
{
	if(m_bDoSPO2checkThread)
	{
		m_bDoSPO2checkThread=false;

		g_eventSPO2CheckData.SetEvent();

		if (WaitForSingleObject(m_pcwtSPO2checkThread->m_hThread,500) == WAIT_TIMEOUT)
		{
			//theApp.getLog()->WriteLine(_T("#HFO:0212"));
			theApp.getLog()->WriteLine(_T("#THR:017a"));	
			if(!TerminateThread(m_pcwtSPO2checkThread,0))
			{
				theApp.getLog()->WriteLine(_T("#THR:017b"));
			}
		}
	}


}

//#####################################CheckThread########################################
// **************************************************************************
// 
// **************************************************************************
static UINT CSPO2checkThread( LPVOID pc )
{
	try
	{
		((CInterfaceSPO2*)pc)->SPO2check();
	}
	catch (CException* e)
	{
		TCHAR   szCause[255];
		e->GetErrorMessage(szCause, 255);

		CString errorStr=_T("");
		errorStr.Format(_T("CSPO2checkThread: %s"),szCause);

		theApp.ReportException(errorStr);

		e->Delete();
	}
	catch(...)
	{
		theApp.ReportException(_T("CSPO2checkThread"));

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_EXCEPTION);
	}
	//((CInterfaceSPO2*)pc)->SPO2check();
	return TRUE;
}

// **************************************************************************
// 
// **************************************************************************
DWORD CInterfaceSPO2::SPO2check(void) 
{
	CeSetThreadPriority(m_pcwtSPO2checkThread->m_hThread,130);

	WORD iCnt=0;
	while(iCnt<20 && m_bDoSPO2checkThread)
	{
		Sleep(50);
		iCnt++;
	}

	if(!m_bDoSPO2checkThread)
	{
		return 0;
	}

	DWORD dwWait=CHECKSPO2;

	do
	{
		DWORD dw = ::WaitForSingleObject(g_eventSPO2CheckData, dwWait);

		if(!m_bDoSPO2checkThread)
			continue;

		checkModuleState();

	}while(m_bDoSPO2checkThread);

	theApp.getLog()->WriteLine(_T("#THR:017"));
	DEBUGMSG(TRUE, (TEXT("#THR:017\r\n")));
	return 0;
}

//#####################################SendThread########################################
// **************************************************************************
// 
// **************************************************************************
static UINT CSPO2SendThread( LPVOID pc )
{
	try
	{
		((CInterfaceSPO2*)pc)->SendSPO2Data();
	}
	catch (CException* e)
	{
		TCHAR   szCause[255];
		e->GetErrorMessage(szCause, 255);

		CString errorStr=_T("");
		errorStr.Format(_T("CSPO2SendThread: %s"),szCause);

		theApp.ReportException(errorStr);

		e->Delete();
	}
	catch(...)
	{
		theApp.ReportException(_T("CSPO2SendThread"));

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_EXCEPTION);
	}
	//((CInterfaceSPO2*)pc)->SendSPO2Data();
	return TRUE;
}

// **************************************************************************
// 
// **************************************************************************
DWORD CInterfaceSPO2::SendSPO2Data(void) 
{
	CeSetThreadPriority(m_pcwtSPO2SendThread->m_hThread,130);

	/*WORD iCnt=0;
	while(iCnt<20 && m_bDoSPO2SendThread)
	{
		Sleep(50);
		iCnt++;
	}*/
	//Sleep(100);


	if(!m_bDoSPO2SendThread)
	{
		return 0;
	}

	do
	{
		
		DWORD dw = ::WaitForSingleObject(g_eventSPO2SendData, INFINITE);

		if(!m_bDoSPO2SendThread)
			continue;

		//INT_PTR temp=MSGSendCO2.GetCount();
		EnterCriticalSection(&csMSGSendSPO2);
		POSITION pos = MSGSendSPO2.GetHeadPosition();
		LeaveCriticalSection(&csMSGSendSPO2);
		while(pos!=NULL && m_bDoSPO2SendThread)
		{
			EnterCriticalSection(&csMSGSendSPO2);

			LPSPO2MSG szTemp = MSGSendSPO2.GetAt(pos);
			Write(szTemp->byBuffer, szTemp->bySize);

			//DEBUGMSG(TRUE, (TEXT("SpO2 send data\r\n")));

			/*CStringW szTest=_T("");
			szTest.Format(_T("send %X,%X,%X,%X,%X,%X,%X,%X,%X,%X,%X,%X,%X size: %d\n"), 
				szTemp->byBuffer[0],szTemp->byBuffer[1],szTemp->byBuffer[2],szTemp->byBuffer[3],
				szTemp->byBuffer[4],szTemp->byBuffer[5],szTemp->byBuffer[6],szTemp->byBuffer[7],
				szTemp->byBuffer[8],szTemp->byBuffer[9],szTemp->byBuffer[10],szTemp->byBuffer[11],
				szTemp->byBuffer[12],szTemp->bySize);
			*/
			
			MSGSendSPO2.RemoveAt(pos);
			delete (void *)szTemp;

			pos = MSGSendSPO2.GetHeadPosition();

			LeaveCriticalSection(&csMSGSendSPO2);
		}

	}while(m_bDoSPO2SendThread);

	theApp.getLog()->WriteLine(_T("#THR:015"));
	DEBUGMSG(TRUE, (TEXT("#THR:015\r\n")));
	
	return 0;
}

//#####################################SendThread########################################
// **************************************************************************
// 
// **************************************************************************
static UINT CSPO2InitThread( LPVOID pc )
{
	try
	{
		((CInterfaceSPO2*)pc)->SPO2Init();
	}
	catch (CException* e)
	{
		TCHAR   szCause[255];
		e->GetErrorMessage(szCause, 255);

		CString errorStr=_T("");
		errorStr.Format(_T("CSPO2InitThread: %s"),szCause);

		theApp.ReportException(errorStr);

		e->Delete();
	}
	catch(...)
	{
		theApp.ReportException(_T("CSPO2InitThread"));

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_EXCEPTION);
	}
	//((CInterfaceSPO2*)pc)->SPO2Init();
	return TRUE;
}

// **************************************************************************
// 
// **************************************************************************
DWORD CInterfaceSPO2::SPO2Init(void) 
{
	CeSetThreadPriority(m_pcwtSPO2InitThread->m_hThread,130);

	m_bStartup=false;

	//Sleep(5000);

	//WORD iCnt=0;
	

	//Sleep(50);

	if(!m_bDoSPO2InitThread)
	{
		return 0;
	}

	DWORD WAIT=150;
	DWORD dwWait=0;
	DWORD dwDiff = 0;
	DWORD dwStart=GetTickCount();
	DWORD dwEnd=0;

	WORD iCntAlarm=0;

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

		DWORD dw = ::WaitForSingleObject(g_eventCO2Startup, dwWait);
		//WaitForSingleObject(g_eventCO2Startup, INFINITE);

		if(!m_bDoSPO2InitThread)
			continue;

		if(OpenCOM())
		{
			StartListener();

			startSPO2sendThread();
			DEBUGMSG(TRUE, (TEXT("#startSPO2sendThread\r\n")));


			m_bSensorConnectedToUSB=true;
			m_bStartup=true;
			//StartupSequenz();
			InitializeSequenz();
			DEBUGMSG(TRUE, (TEXT("#InitializeSequenz\r\n")));
		}
		else
		{
			iCntAlarm++;

			if(iCntAlarm>=30 && m_bDoSPO2InitThread)
			{
				iCntAlarm=0;
				if(getModel()->getALARMHANDLER()->ALARM_Sens_SPO2_MODULE_NOTCONNECTED->getAlarmState()!=AS_ACTIVE)
				{
					m_bSensorConnectedToUSB=false;

					if(getModel()->getALARMHANDLER()->CanSetAlarm_SPO2_MODULE_NOTCONNECTED())
					{
						if(AfxGetApp())
							AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_SPO2_MODULE_NOTCONNECTED);
					}

					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_REINIT_SPO2_MODULE);

					m_bDoSPO2InitThread=false;
				}
			}
		}
	}while(m_bDoSPO2InitThread && m_bStartup==false);

	//theApp.getLog()->WriteLine(_T("#THR:016"));
	DEBUGMSG(TRUE, (TEXT("#THR:016\r\n")));
	//if(m_bStartup)
	//	InitializeSequenz();//command 84h

	
	return 0;
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceSPO2::updateLastSpO2Cmd()
{
	if(isSafeTickCountDelayExpired(m_dwLastSpO2Cmd, 1000))//rkuNEWFIX 
	{
		EnterCriticalSection(&csUpdateDataSpO2);
		m_dwLastSpO2Cmd=GetTickCount();
		LeaveCriticalSection(&csUpdateDataSpO2);
	}
}

void CInterfaceSPO2::checkLastSpO2Cmd()
{
#ifndef SIMULATION_ENTREK

	EnterCriticalSection(&csUpdateDataSpO2);
	DWORD temp=m_dwLastSpO2Cmd;
	LeaveCriticalSection(&csUpdateDataSpO2);

	if(isSafeTickCountDelayExpired(temp, SPO2SENSORCHECKTIME))
	{
		if(getModel()->getALARMHANDLER()->ALARM_Sens_SPO2_MODULE_NOTCONNECTED->getAlarmState()==AS_NONE)
		{
			m_bSensorConnectedToUSB=false;

			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_SPO2_MODULE_NOTCONNECTED);

			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_REINIT_SPO2_MODULE);
		}
		else if(getModel()->getALARMHANDLER()->ALARM_Sens_SPO2_MODULE_NOTCONNECTED->getAlarmState()==AS_ACTIVE && m_bSensorConnectedToUSB==true)
		{
			//DEBUGMSG(TRUE, (TEXT("WM_REINIT_SPO2_MODULE m_bSensorConnectedToUSB\r\n")));
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_REINIT_SPO2_MODULE);
		}
	}
	else if(getModel()->getALARMHANDLER()->ALARM_Sens_SPO2_MODULE_NOTCONNECTED->getAlarmState()==AS_ACTIVE && m_bSensorConnectedToUSB==false && m_bSensorConnectedToModule==true)
	{
		getModel()->getALARMHANDLER()->deleteAlarm(AL_Sens_SPO2_MODULE_NOTCONNECTED);
	}
#endif
}


bool CInterfaceSPO2::isSensorConnectedToUSB()
{
	return m_bSensorConnectedToUSB;
}

bool CInterfaceSPO2::isSensorConnectedToModule()
{
	return m_bSensorConnectedToModule;
}
// **************************************************************************
// 
// **************************************************************************
void CInterfaceSPO2::send(LPSPO2MSG msg)
{
	EnterCriticalSection(&csMSGSendSPO2);
	MSGSendSPO2.AddTail(msg);
	LeaveCriticalSection(&csMSGSendSPO2);

	g_eventSPO2SendData.SetEvent();

	/*CStringW szTemp=_T("");
	szTemp.Format(_T("%X,%X,%X,%X,%X,%X,%X,%X,%X,%X,%X,%X,%X size: %d\n"), 
		msg->byBuffer[0],msg->byBuffer[1],msg->byBuffer[2],msg->byBuffer[3],
		msg->byBuffer[4],msg->byBuffer[5],msg->byBuffer[6],msg->byBuffer[7],
		msg->byBuffer[8],msg->byBuffer[9],msg->byBuffer[10],msg->byBuffer[11],
		msg->byBuffer[12],msg->bySize);

	*/
}