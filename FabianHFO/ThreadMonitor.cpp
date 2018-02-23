// ThreadMonitor.cpp : implementation file
//

#include "stdafx.h"
#include "FabianLib.h"
#include "ThreadMonitor.h"
#include "MVModel.h"

extern CEvent g_eventNewMonitorData;

/**********************************************************************************************//**
 * A macro that defines refresh monitordata
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

#define REFRESH_MONITORDATA 1000

// CThreadMonitor
CThreadMonitor* CThreadMonitor::theThreadMonitor=0;

/**********************************************************************************************//**
 * Initializes a new instance of the CThreadMonitor class
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

CThreadMonitor::CThreadMonitor()
{
	m_pModel = NULL;

	//m_ullLastATZData=0;
	m_bDoMonitorThread=false;
	m_pcwtMonitorThread=NULL;
	m_hThreadMonitor=INVALID_HANDLE_VALUE;
	

}

/**********************************************************************************************//**
 * Finalizes an instance of the CThreadMonitor class
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

CThreadMonitor::~CThreadMonitor()
{
	stopMonitorThread();

	if(m_pcwtMonitorThread!=NULL)
	{
		delete m_pcwtMonitorThread;
		m_pcwtMonitorThread=NULL;
		if(m_hThreadMonitor!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadMonitor);
			m_hThreadMonitor=INVALID_HANDLE_VALUE;
		}
	}
}

/**********************************************************************************************//**
 * Gets the instance
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	Null if it fails, else the instance.
 **************************************************************************************************/

CThreadMonitor* CThreadMonitor::getInstance()
{
	if(theThreadMonitor == 0)
	{
		theThreadMonitor = new CThreadMonitor;
	}
	return theThreadMonitor;
}

/**********************************************************************************************//**
 * Destroys the instance
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CThreadMonitor::destroyInstance()
{
	if(theThreadMonitor != NULL)
	{
		delete theThreadMonitor;
		theThreadMonitor = NULL;
	}
}

/**********************************************************************************************//**
 * Gets the model
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	Null if it fails, else the model.
 **************************************************************************************************/

CMVModel *CThreadMonitor::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}

/**********************************************************************************************//**
 * Starts monitor thread
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CThreadMonitor::startMonitorThread( void )
{
	m_bDoMonitorThread=true;

	if(m_pcwtMonitorThread!=NULL)
	{
		delete m_pcwtMonitorThread;
		m_pcwtMonitorThread=NULL;

		if(m_hThreadMonitor!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadMonitor);
			m_hThreadMonitor=INVALID_HANDLE_VALUE;
		}

	}

	m_pcwtMonitorThread=AfxBeginThread(MonitorThread,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	m_hThreadMonitor=m_pcwtMonitorThread->m_hThread;
	m_pcwtMonitorThread->m_bAutoDelete = FALSE; 
	m_pcwtMonitorThread->ResumeThread();
}

/**********************************************************************************************//**
 * Stops monitor thread
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CThreadMonitor::stopMonitorThread( void )
{
	if(m_bDoMonitorThread)
	{
		m_bDoMonitorThread=false;
		g_eventNewMonitorData.SetEvent();

		if (WaitForSingleObject(m_pcwtMonitorThread->m_hThread,1000) == WAIT_TIMEOUT)
		{
			theApp.getLog()->WriteLine(_T("#THR:023a"));
			if(!TerminateThread(m_pcwtMonitorThread,0))
			{
				theApp.getLog()->WriteLine(_T("#THR:023b"));
			}
		}
	}
}

/**********************************************************************************************//**
 * Monitor thread
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	pc	The PC.
 *
 * \return	An UINT.
 **************************************************************************************************/

static UINT MonitorThread( LPVOID pc )
{
	try
	{
		((CThreadMonitor*)pc)->MonitorData();
	}
	catch (CException* e)
	{
		TCHAR   szCause[255];
		e->GetErrorMessage(szCause, 255);

		CString errorStr=_T("");
		errorStr.Format(_T("MonitorThread: %s"),szCause);

		theApp.ReportException(errorStr);

		e->Delete();
	}
	catch(...)
	{
		theApp.ReportException(_T("MonitorThread"));

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_EXCEPTION);
	}
	//((CThreadMonitor*)pc)->MonitorData();
	return TRUE;
}

/**********************************************************************************************//**
 * Monitor data
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	A DWORD.
 **************************************************************************************************/

DWORD CThreadMonitor::MonitorData(void) 
{
	CeSetThreadPriority(m_pcwtMonitorThread->m_hThread,130);

	do
	{
		if (::WaitForSingleObject(g_eventNewMonitorData, INFINITE) == WAIT_OBJECT_0)
		{
			if(m_bDoMonitorThread)
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_NEWMONITORDATA);
			}
		}
	}while(m_bDoMonitorThread);

	//theApp.getLog()->WriteLine(_T("#THR:023"));

	return 0;
}
