// ThreadAlarm.cpp : implementation file
//

#include "stdafx.h"
#include "FabianLib.h"
#include "ThreadAlarm.h"
#include "MVModel.h"

extern CEvent g_eventNewAlarmData;

// CThreadAlarm
CThreadAlarm* CThreadAlarm::theThreadAlarm=0;

/**********************************************************************************************//**
 * Initializes a new instance of the CThreadAlarm class
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

CThreadAlarm::CThreadAlarm()
{
	m_pModel = NULL;

	m_bDoAlarmThread=false;
	m_pcwtAlarmThread=NULL;
	m_hThreadAlarm=INVALID_HANDLE_VALUE;
	
	m_bCheckAlarmData=false;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CThreadAlarm class
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Gets the instance
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	Null if it fails, else the instance.
 **************************************************************************************************/

CThreadAlarm* CThreadAlarm::getInstance()
{
	if(theThreadAlarm == 0)
	{
		theThreadAlarm = new CThreadAlarm;
	}
	return theThreadAlarm;
}

/**********************************************************************************************//**
 * Destroys the instance
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CThreadAlarm::destroyInstance()
{
	if(theThreadAlarm != NULL)
	{
		delete theThreadAlarm;
		theThreadAlarm = NULL;
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

CMVModel *CThreadAlarm::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}

/**********************************************************************************************//**
 * Starts alarm thread
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Stops alarm thread
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CThreadAlarm::stopAlarmThread( void )
{
	if(m_bDoAlarmThread)
	{
		m_bDoAlarmThread=false;
		g_eventNewAlarmData.SetEvent();

		if (WaitForSingleObject(m_pcwtAlarmThread->m_hThread,1000) == WAIT_TIMEOUT)
		{
			theApp.WriteLog(_T("#THR:Alarm"));
			if(!TerminateThread(m_pcwtAlarmThread,0))
			{
				theApp.WriteLog(_T("#THR:Alarmb"));
			}
		}
	}
}

/**********************************************************************************************//**
 * Alarm thread
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	pc	The PC.
 *
 * \return	An UINT.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Alarm data
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	A DWORD.
 **************************************************************************************************/

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

	//theApp.WriteLog(_T("#THR:024"));

	return 0;
}

/**********************************************************************************************//**
 * Sets check alarm data
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	bState	True to state.
 **************************************************************************************************/

void CThreadAlarm::setCheckAlarmData(bool bState)
{
	m_bCheckAlarmData=bState;
}