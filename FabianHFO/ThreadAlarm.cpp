// ThreadAlarm.cpp : implementation file
//

#include "stdafx.h"
#include "FabianLib.h"
#include "ThreadAlarm.h"
#include "MVModel.h"

extern CEvent g_eventNewAlarmData;

// CThreadAlarm
CThreadAlarm* CThreadAlarm::theThreadAlarm=0;

CThreadAlarm::CThreadAlarm()
{
	m_pModel = NULL;

	m_bDoAlarmThread=false;
	m_pcwtAlarmThread=NULL;
	m_hThreadAlarm=INVALID_HANDLE_VALUE;
	
	m_bCheckAlarmData=false;
}

//************************************
// Method:    ~CThreadAlarm
// FullName:  CThreadAlarm::~CThreadAlarm
// Access:    protected 
// Returns:   
// Qualifier:
//
// 2015/06/19: checked for correct closing of thread
//************************************
CThreadAlarm::~CThreadAlarm()
{
	stopAlarmThread();

	if(m_pcwtAlarmThread!=NULL)
	{
		delete m_pcwtAlarmThread;
		m_pcwtAlarmThread=NULL;
		if(m_hThreadAlarm!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadAlarm);
			m_hThreadAlarm=INVALID_HANDLE_VALUE;
		}
	}
}
//=============================================================================
/**
 * @brief Get the instance of Alarm thread (singleton).
 *
 * @return the instance of Alarm thread
 *
 **/
//=============================================================================
CThreadAlarm* CThreadAlarm::getInstance()
{
	if(theThreadAlarm == 0)
	{
		theThreadAlarm = new CThreadAlarm;
	}
	return theThreadAlarm;
}

//=============================================================================
/**
 * @brief Destroys the instance of Alarm thread (singleton).
 **/
//=============================================================================
void CThreadAlarm::destroyInstance()
{
	if(theThreadAlarm != NULL)
	{
		delete theThreadAlarm;
		theThreadAlarm = NULL;
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
CMVModel *CThreadAlarm::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}


// CThreadMonitor message handlers
// **************************************************************************
// 
// **************************************************************************
void CThreadAlarm::startAlarmThread( void )
{
	m_bDoAlarmThread=true;

	if(m_pcwtAlarmThread!=NULL)
	{
		delete m_pcwtAlarmThread;
		m_pcwtAlarmThread=NULL;

		if(m_hThreadAlarm!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadAlarm);
			m_hThreadAlarm=INVALID_HANDLE_VALUE;
		}

	}

	m_pcwtAlarmThread=AfxBeginThread(AlarmThread,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	m_hThreadAlarm=m_pcwtAlarmThread->m_hThread;
	m_pcwtAlarmThread->m_bAutoDelete = FALSE; 
	m_pcwtAlarmThread->ResumeThread();
}

//************************************
// Method:    stopAlarmThread
// FullName:  CThreadAlarm::stopAlarmThread
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: void
//
// 2015/06/19: checked for correct closing of thread
//************************************
void CThreadAlarm::stopAlarmThread( void )
{
	if(m_bDoAlarmThread)
	{
		m_bDoAlarmThread=false;
		g_eventNewAlarmData.SetEvent();

		if (WaitForSingleObject(m_pcwtAlarmThread->m_hThread,1000) == WAIT_TIMEOUT)
		{
			theApp.getLog()->WriteLine(_T("#THR:Alarm"));
			if(!TerminateThread(m_pcwtAlarmThread,0))
			{
				theApp.getLog()->WriteLine(_T("#THR:Alarmb"));
			}
		}
	}
}

// **************************************************************************
// 
// **************************************************************************
static UINT AlarmThread( LPVOID pc )
{
	try
	{
		((CThreadAlarm*)pc)->AlarmData();
	}
	catch (CException* e)
	{
		TCHAR   szCause[255];
		e->GetErrorMessage(szCause, 255);

		CString errorStr=_T("");
		errorStr.Format(_T("AlarmThread: %s"),szCause);

		theApp.ReportException(errorStr);

		e->Delete();
	}
	catch(...)
	{
		theApp.ReportException(_T("AlarmThread"));

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_EXCEPTION);
	}
	//((CThreadAlarm*)pc)->AlarmData();
	return TRUE;
}
// **************************************************************************
// 
// **************************************************************************
DWORD CThreadAlarm::AlarmData(void) 
{
	CeSetThreadPriority(m_pcwtAlarmThread->m_hThread,130);

	do
	{
		if (::WaitForSingleObject(g_eventNewAlarmData, INFINITE) == WAIT_OBJECT_0)
		{
			if(m_bDoAlarmThread && getModel()->isSPIavailabel())//newSPI
			{
				//checkhere
				if(getModel()->getSPI()->NewAlarmData())
				{
					int iAlarmState=getModel()->getDATAHANDLER()->GetAlarmStateBytes();
					getModel()->getDATAHANDLER()->ResetAlarmStateBytes();
					getModel()->getSPI()->ResetAlarmData();

					getModel()->ParseAlarmStateBytes(iAlarmState);
				}
				else if(m_bCheckAlarmData)
				{
					int iAlarmState=getModel()->getDATAHANDLER()->GetAlarmStateBytes();
					getModel()->getDATAHANDLER()->ResetAlarmStateBytes();

					getModel()->ParseAlarmStateBytes(iAlarmState);
					m_bCheckAlarmData=false;
				}
				else
				{
					getModel()->getDATAHANDLER()->ResetAlarmStateBytes();
					getModel()->getSPI()->ResetAlarmData();
					m_bCheckAlarmData=false;
				}
			}

		}
	}while(m_bDoAlarmThread);

	//theApp.getLog()->WriteLine(_T("#THR:024"));

	return 0;
}


void CThreadAlarm::setCheckAlarmData(bool bState)
{
	m_bCheckAlarmData=bState;
}