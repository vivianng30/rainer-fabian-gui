// InterfaceCOMport.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "InterfaceCOMport.h"
#include "MVModel.h"
//#include "acuLink.h"

#include "InterfaceCOMInbiolab.h"

CInterfaceCOMport* CInterfaceCOMport::theCOMInterface=0;
BYTE CInterfaceCOMport::m_iCOMmodule=0;
CList<LPCOMMSG, LPCOMMSG>  CInterfaceCOMport::MSGSendCOM;


CInterfaceCOMport::CInterfaceCOMport(void)
{
	InitializeCriticalSection(&csMSGSendCOM);
	InitializeCriticalSection(&csUpdateDataCOM);

	m_cLastWaveVal=0;
	m_bConnectedToCOM=false;
	m_iCOMmodule=0;
	m_bStartup=false;

	m_pcwtCOMSendThread=NULL;
	m_hThreadCOMSend=INVALID_HANDLE_VALUE;
	m_bDoCOMSendThread=false;

	m_pcwtCOMInitThread=NULL;
	m_hThreadCOMInit=INVALID_HANDLE_VALUE;
	m_bDoCOMInitThread=false;

	m_pcwtCOMcheckThread=NULL;
	m_hThreadCOMcheck=INVALID_HANDLE_VALUE;
	m_bDoCOMcheckThread=false;

	 m_dwLastCOMCmd=0;

	m_bTrigger=false;
}

CInterfaceCOMport::~CInterfaceCOMport(void)
{
	if(m_pcwtCOMcheckThread!=NULL)
	{
		delete m_pcwtCOMcheckThread;
		m_pcwtCOMcheckThread=NULL;

		if(m_hThreadCOMcheck!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadCOMcheck);
			m_hThreadCOMcheck=INVALID_HANDLE_VALUE;
		}
	}

	if(m_pcwtCOMSendThread!=NULL)
	{
		delete m_pcwtCOMSendThread;
		m_pcwtCOMSendThread=NULL;

		if(m_hThreadCOMSend!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadCOMSend);
			m_hThreadCOMSend=INVALID_HANDLE_VALUE;
		}
	}


	if(m_pcwtCOMInitThread!=NULL)
	{
		delete m_pcwtCOMInitThread;
		m_pcwtCOMInitThread=NULL;

		if(m_hThreadCOMInit!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadCOMInit);
			m_hThreadCOMInit=INVALID_HANDLE_VALUE;
		}
	}

	if(MSGSendCOM.GetCount()>0)
	{
		POSITION pos = MSGSendCOM.GetHeadPosition();
		if(pos==NULL)
		{

		}
		else
		{
			LPCOMMSG szTemp = MSGSendCOM.GetAt(pos);
			MSGSendCOM.RemoveAt(pos);
			delete (void *)szTemp;
		}
	}

	StopListener(-1);
	Close();

	DeleteCriticalSection(&csMSGSendCOM);
	DeleteCriticalSection(&csUpdateDataCOM);
	//DeleteCriticalSection(&csErrorDataSPO2);
}

// **************************************************************************
// 
// **************************************************************************
CInterfaceCOMport* CInterfaceCOMport::GetInstance(BYTE iCOMmodule)
{
	if(theCOMInterface == 0)
	{
		m_iCOMmodule=iCOMmodule;
		if(m_iCOMmodule==COMMODULE_INBIOLAB)
			theCOMInterface = new CInterfaceCOMInbiolab;
		/*else if(m_iCO2module==CO2MODULE_MICROPOD)
			theETCO2Interface = new CInterfaceCO2_MICROPOD;*/
	}
	return theCOMInterface;
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceCOMport::DestroyInstance()
{
	if(theCOMInterface != NULL)
	{
		delete theCOMInterface;
		theCOMInterface = NULL;
	}
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceCOMport::startCOMThread(void)
{
	//###########send thread####################
	m_bDoCOMSendThread=true;

	if(m_pcwtCOMSendThread!=NULL)
	{
		delete m_pcwtCOMSendThread;
		m_pcwtCOMSendThread=NULL;

		if(m_hThreadCOMSend!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadCOMSend);
			m_hThreadCOMSend=INVALID_HANDLE_VALUE;
		}
	}

	m_pcwtCOMSendThread=AfxBeginThread(CCOMSendThread,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	m_hThreadCOMSend=m_pcwtCOMSendThread->m_hThread;
	m_pcwtCOMSendThread->m_bAutoDelete = FALSE; 
	m_pcwtCOMSendThread->ResumeThread();

	//###########initialization thread####################
	m_bDoCOMInitThread=true;

	if(m_pcwtCOMInitThread!=NULL)
	{
		delete m_pcwtCOMInitThread;
		m_pcwtCOMInitThread=NULL;

		if(m_hThreadCOMInit!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadCOMInit);
			m_hThreadCOMInit=INVALID_HANDLE_VALUE;
		}
	}

	m_pcwtCOMInitThread=AfxBeginThread(CCOMInitThread,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	m_hThreadCOMInit=m_pcwtCOMInitThread->m_hThread;
	m_pcwtCOMInitThread->m_bAutoDelete = FALSE; 
	m_pcwtCOMInitThread->ResumeThread();
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceCOMport::stopCOMThread(void)
{
	if(m_bDoCOMSendThread)
	{
		m_bDoCOMSendThread=false;

		g_eventCOMIFSendData.SetEvent();

		if (WaitForSingleObject(m_pcwtCOMSendThread->m_hThread,500) == WAIT_TIMEOUT)
		{
			theApp.getLog()->WriteLine(_T("#THR:003a"));
			if(!TerminateThread(m_pcwtCOMSendThread,0))
			{
				theApp.getLog()->WriteLine(_T("#THR:003b"));
			}
		}
	}

	if(m_bDoCOMInitThread)
	{
		m_bDoCOMInitThread=false;

		if (WaitForSingleObject(m_pcwtCOMInitThread->m_hThread,2000) == WAIT_TIMEOUT)
		{
			theApp.getLog()->WriteLine(_T("#THR:004a"));
			if(!TerminateThread(m_pcwtCOMInitThread,0))
			{
				theApp.getLog()->WriteLine(_T("#THR:004b"));
			}
		}
	}

	if(MSGSendCOM.GetCount()>0)
	{
		POSITION pos = MSGSendCOM.GetHeadPosition();
		if(pos==NULL)
		{

		}
		else
		{
			LPCOMMSG szTemp = MSGSendCOM.GetAt(pos);
			MSGSendCOM.RemoveAt(pos);
			delete (void *)szTemp;
		}
	}


}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceCOMport::startCOMcheckThread(void)
{
	//###########check thread####################
	m_bDoCOMcheckThread=true;

	if(m_pcwtCOMcheckThread!=NULL)
	{
		delete m_pcwtCOMcheckThread;
		m_pcwtCOMcheckThread=NULL;

		if(m_hThreadCOMcheck!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadCOMcheck);
			m_hThreadCOMcheck=INVALID_HANDLE_VALUE;
		}
	}

	m_pcwtCOMcheckThread=AfxBeginThread(CCOMcheckThread,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	m_hThreadCOMcheck=m_pcwtCOMcheckThread->m_hThread;
	m_pcwtCOMcheckThread->m_bAutoDelete = FALSE; 
	m_pcwtCOMcheckThread->ResumeThread();
}

void CInterfaceCOMport::stopCOMcheckThread(void)
{
	if(m_bDoCOMcheckThread)
	{
		m_bDoCOMcheckThread=false;

		g_eventCOMIFCheckData.SetEvent();

		if (WaitForSingleObject(m_pcwtCOMcheckThread->m_hThread,500) == WAIT_TIMEOUT)
		{
			theApp.getLog()->WriteLine(_T("#THR:005a"));
			if(!TerminateThread(m_pcwtCOMcheckThread,0))
			{
				theApp.getLog()->WriteLine(_T("#THR:005b"));
			}
		}
	}


}

//#####################################CheckThread########################################
// **************************************************************************
// 
// **************************************************************************
static UINT CCOMcheckThread( LPVOID pc )
{
	try
	{
		((CInterfaceCOMport*)pc)->COMcheck();
	}
	catch (CException* e)
	{
		TCHAR   szCause[255];
		e->GetErrorMessage(szCause, 255);

		CString errorStr=_T("");
		errorStr.Format(_T("CCOMcheckThread: %s"),szCause);

		theApp.ReportException(errorStr);

		e->Delete();
	}
	catch(...)
	{
		theApp.ReportException(_T("CCOMcheckThread"));

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_EXCEPTION);
	}

	//((CInterfaceCOMport*)pc)->COMcheck();
	return TRUE;
}

// **************************************************************************
// 
// **************************************************************************
DWORD CInterfaceCOMport::COMcheck(void) 
{
	CeSetThreadPriority(m_pcwtCOMcheckThread->m_hThread,130);

	WORD iCnt=0;
	while(iCnt<40 && m_bDoCOMcheckThread)
	{
		Sleep(50);
		iCnt++;
	}

	if(!m_bDoCOMcheckThread)
	{
		return 0;
	}

	DWORD dwWait=CHECKCOM;

	do
	{
		DWORD dw = ::WaitForSingleObject(g_eventCOMIFCheckData, dwWait);

		if(!m_bDoCOMcheckThread)
			continue;

		checkModuleState();

	}while(m_bDoCOMcheckThread);

	theApp.getLog()->WriteLine(_T("#THR:005"));
	//DEBUGMSG(TRUE, (TEXT("end CInterfaceCOM::COMcheck\r\n")));
	return 0;
}

//#####################################SendThread########################################
// **************************************************************************
// 
// **************************************************************************
static UINT CCOMSendThread( LPVOID pc )
{
	try
	{
		((CInterfaceCOMport*)pc)->SendCOMData();
	}
	catch (CException* e)
	{
		TCHAR   szCause[255];
		e->GetErrorMessage(szCause, 255);

		CString errorStr=_T("");
		errorStr.Format(_T("CCOMSendThread: %s"),szCause);

		theApp.ReportException(errorStr);

		e->Delete();
	}
	catch(...)
	{
		theApp.ReportException(_T("CCOMSendThread"));

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_EXCEPTION);
	}

	//((CInterfaceCOMport*)pc)->SendCOMData();
	return TRUE;
}

// **************************************************************************
// 
// **************************************************************************
DWORD CInterfaceCOMport::SendCOMData(void) 
{
	CeSetThreadPriority(m_pcwtCOMSendThread->m_hThread,130);

	/*WORD iCnt=0;
	while(iCnt<20 && m_bDoSPO2SendThread)
	{
		Sleep(50);
		iCnt++;
	}*/
	//Sleep(100);


	if(!m_bDoCOMSendThread)
	{
		return 0;
	}

	do
	{
		
		DWORD dw = ::WaitForSingleObject(g_eventCOMIFSendData, INFINITE);

		if(!m_bDoCOMSendThread)
			continue;

		//INT_PTR temp=MSGSendCO2.GetCount();
		EnterCriticalSection(&csMSGSendCOM);
		POSITION pos = MSGSendCOM.GetHeadPosition();
		LeaveCriticalSection(&csMSGSendCOM);
		while(pos!=NULL && m_bDoCOMSendThread)
		{
			EnterCriticalSection(&csMSGSendCOM);

			LPCOMMSG szTemp = MSGSendCOM.GetAt(pos);
			Write(szTemp->byBuffer, szTemp->bySize);

			//DEBUGMSG(TRUE, (TEXT("SpO2 send data\r\n")));

			
			
			MSGSendCOM.RemoveAt(pos);
			delete (void *)szTemp;

			pos = MSGSendCOM.GetHeadPosition();

			LeaveCriticalSection(&csMSGSendCOM);
		}

	}while(m_bDoCOMSendThread);

	theApp.getLog()->WriteLine(_T("#THR:003"));

	
	return 0;
}

//#####################################SendThread########################################
// **************************************************************************
// 
// **************************************************************************
static UINT CCOMInitThread( LPVOID pc )
{
	try
	{
		((CInterfaceCOMport*)pc)->COMInit();
	}
	catch (CException* e)
	{
		TCHAR   szCause[255];
		e->GetErrorMessage(szCause, 255);

		CString errorStr=_T("");
		errorStr.Format(_T("CCOMInitThread: %s"),szCause);

		theApp.ReportException(errorStr);

		e->Delete();
	}
	catch(...)
	{
		theApp.ReportException(_T("CCOMInitThread"));

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_EXCEPTION);
	}

	//((CInterfaceCOMport*)pc)->COMInit();
	return TRUE;
}

// **************************************************************************
// 
// **************************************************************************
DWORD CInterfaceCOMport::COMInit(void) 
{
	CeSetThreadPriority(m_pcwtCOMInitThread->m_hThread,130);


	//m_bInitialized=false;
	m_bStartup=false;
	//m_bConnected=false;

	//Sleep(10000);

	WORD iCnt=0;
	/*while(iCnt<100 && m_bDoSPO2InitThread)
	{
		Sleep(50);
		iCnt++;
	}*/

	Sleep(100);

	if(!m_bDoCOMInitThread)
	{
		return 0;
	}

	WORD iCntAlarm=0;

	do
	{
		//WaitForSingleObject(g_eventCO2Startup, INFINITE);

		if(!m_bDoCOMInitThread)
			continue;

		if(OpenCOM())
		{
			m_bConnectedToCOM=true;
			m_bStartup=true;
			StartupSequenz();
		}
		else
		{
			iCnt=0;
			while(iCnt<20 && m_bDoCOMInitThread)
			{
				Sleep(50);
				iCnt++;
			}
			iCntAlarm++;

			if(iCntAlarm>=30)
			{
				iCntAlarm=0;
				
			}
		}
	}while(m_bDoCOMInitThread && m_bStartup==false);


	if(m_bStartup)
		InitializeSequenz();//command 84h

	theApp.getLog()->WriteLine(_T("#THR:004"));

	return 0;
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceCOMport::updateLastCOMCmd()
{
	EnterCriticalSection(&csUpdateDataCOM);
	m_dwLastCOMCmd=GetTickCount();
	LeaveCriticalSection(&csUpdateDataCOM);

	//DEBUGMSG(TRUE, (TEXT("Update %d "),m_dwLastCOMCmd));
}

void CInterfaceCOMport::checkLastCOMCmd()
{
	EnterCriticalSection(&csUpdateDataCOM);
	DWORD temp=m_dwLastCOMCmd;
	LeaveCriticalSection(&csUpdateDataCOM);

	if(isSafeTickCountDelayExpired(temp, 5000))
	{
		Deinit();
		Sleep(200);
		Init(9);
	}
}


bool CInterfaceCOMport::isConnectedToCOM()
{
	return m_bConnectedToCOM;
}

bool CInterfaceCOMport::isTrigger()
{
	return m_bTrigger;
}

void CInterfaceCOMport::checkModuleState()
{

}

//bool CInterfaceCOMport::isSensorConnectedToModule()
//{
//	return m_bSensorConnectedToModule;
//}
// **************************************************************************
// 
// **************************************************************************
void CInterfaceCOMport::send(LPCOMMSG msg)
{
	EnterCriticalSection(&csMSGSendCOM);
	MSGSendCOM.AddTail(msg);
	LeaveCriticalSection(&csMSGSendCOM);

	g_eventCOMIFSendData.SetEvent();

	/*CStringW szTemp=_T("");
	szTemp.Format(_T("%X,%X,%X,%X,%X,%X,%X,%X,%X,%X,%X,%X,%X size: %d\n"), 
		msg->byBuffer[0],msg->byBuffer[1],msg->byBuffer[2],msg->byBuffer[3],
		msg->byBuffer[4],msg->byBuffer[5],msg->byBuffer[6],msg->byBuffer[7],
		msg->byBuffer[8],msg->byBuffer[9],msg->byBuffer[10],msg->byBuffer[11],
		msg->byBuffer[12],msg->bySize);

	*/
}