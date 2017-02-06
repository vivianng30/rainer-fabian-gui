// ThreadMonitor.cpp : implementation file
//

#include "stdafx.h"
#include "Fabianlib.h"
#include "ThreadMonitor.h"
#include "MVModel.h"

extern CEvent g_eventNewMonitorData;
#define REFRESH_MONITORDATA 1000

// CThreadMonitor
CThreadMonitor* CThreadMonitor::theThreadMonitor=0;

CThreadMonitor::CThreadMonitor()
{
	m_pModel = NULL;

	//m_ullLastATZData=0;
	m_bDoMonitorThread=false;
	m_pcwtMonitorThread=NULL;
	m_hThreadMonitor=INVALID_HANDLE_VALUE;
	

}

//************************************
// Method:    ~CThreadMonitor
// FullName:  CThreadMonitor::~CThreadMonitor
// Access:    protected 
// Returns:   
// Qualifier:
//
// 2015/06/19: checked for correct closing of thread
//************************************
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
//=============================================================================
/**
 * @brief Get the instance of Monitor thread (singleton).
 *
 * @return the instance of Monitor thread
 *
 **/
//=============================================================================
CThreadMonitor* CThreadMonitor::getInstance()
{
	if(theThreadMonitor == 0)
	{
		theThreadMonitor = new CThreadMonitor;
	}
	return theThreadMonitor;
}

//=============================================================================
/**
 * @brief Destroys the instance of Monitor thread (singleton).
 **/
//=============================================================================
void CThreadMonitor::destroyInstance()
{
	if(theThreadMonitor != NULL)
	{
		delete theThreadMonitor;
		theThreadMonitor = NULL;
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
CMVModel *CThreadMonitor::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}


// CThreadMonitor message handlers
// **************************************************************************
// 
// **************************************************************************
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

//************************************
// Method:    stopMonitorThread
// FullName:  CThreadMonitor::stopMonitorThread
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: void
//
// 2015/06/19: checked for correct closing of thread
//************************************
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

// **************************************************************************
// 
// **************************************************************************
static UINT MonitorThread( LPVOID pc )
{
	((CThreadMonitor*)pc)->MonitorData();
	return TRUE;
}
// **************************************************************************
// 
// **************************************************************************
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

	theApp.getLog()->WriteLine(_T("#THR:023"));

	return 0;
}
