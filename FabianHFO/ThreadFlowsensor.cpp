// ThreadFlowsensor.cpp : implementation file
//

#include "stdafx.h"
#include "FabianLib.h"
#include "ThreadFlowsensor.h"
#include "MVModel.h"

extern CEvent g_eventNewFlowsensorData;

// CThreadFlowsensor
CThreadFlowsensor* CThreadFlowsensor::theThreadFlowsensor=0;

/**********************************************************************************************//**
 * Initializes a new instance of the CThreadFlowsensor class
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

CThreadFlowsensor::CThreadFlowsensor()
{
	m_pModel = NULL;

	m_bDoFlowsensorThread=false;
	m_pcwtFlowsensorThread=NULL;
	m_hThreadFlowsensor=INVALID_HANDLE_VALUE;
	
	m_bCheckFlowsensorData=false;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CThreadFlowsensor class
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

CThreadFlowsensor::~CThreadFlowsensor()
{
	stopFlowsensorThread();

	if(m_pcwtFlowsensorThread!=NULL)
	{
		delete m_pcwtFlowsensorThread;
		m_pcwtFlowsensorThread=NULL;
		if(m_hThreadFlowsensor!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadFlowsensor);
			m_hThreadFlowsensor=INVALID_HANDLE_VALUE;
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

CThreadFlowsensor* CThreadFlowsensor::getInstance()
{
	if(theThreadFlowsensor == 0)
	{
		theThreadFlowsensor = new CThreadFlowsensor;
	}
	return theThreadFlowsensor;
}

/**********************************************************************************************//**
 * Destroys the instance
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CThreadFlowsensor::destroyInstance()
{
	if(theThreadFlowsensor != NULL)
	{
		delete theThreadFlowsensor;
		theThreadFlowsensor = NULL;
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

CMVModel *CThreadFlowsensor::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}

/**********************************************************************************************//**
 * Starts flowsensor thread
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CThreadFlowsensor::startFlowsensorThread( void )
{
	m_bDoFlowsensorThread=true;

	if(m_pcwtFlowsensorThread!=NULL)
	{
		delete m_pcwtFlowsensorThread;
		m_pcwtFlowsensorThread=NULL;

		if(m_hThreadFlowsensor!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadFlowsensor);
			m_hThreadFlowsensor=INVALID_HANDLE_VALUE;
		}

	}

	m_pcwtFlowsensorThread=AfxBeginThread(FlowsensorThread,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	m_hThreadFlowsensor=m_pcwtFlowsensorThread->m_hThread;
	m_pcwtFlowsensorThread->m_bAutoDelete = FALSE; 
	m_pcwtFlowsensorThread->ResumeThread();
}

/**********************************************************************************************//**
 * Stops flowsensor thread
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CThreadFlowsensor::stopFlowsensorThread( void )
{
	if(m_bDoFlowsensorThread)
	{
		m_bDoFlowsensorThread=false;
		g_eventNewFlowsensorData.SetEvent();

		if (WaitForSingleObject(m_pcwtFlowsensorThread->m_hThread,1000) == WAIT_TIMEOUT)
		{
			theApp.getLog()->WriteLine(_T("#THR:023a"));
			if(!TerminateThread(m_pcwtFlowsensorThread,0))
			{
				theApp.getLog()->WriteLine(_T("#THR:023b"));
			}
		}
	}
}

/**********************************************************************************************//**
 * Flowsensor thread
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	pc	The PC.
 *
 * \return	An UINT.
 **************************************************************************************************/

static UINT FlowsensorThread( LPVOID pc )
{
	try
	{
		((CThreadFlowsensor*)pc)->FlowsensorData();
	}
	catch (CException* e)
	{
		TCHAR   szCause[255];
		e->GetErrorMessage(szCause, 255);

		CString errorStr=_T("");
		errorStr.Format(_T("FlowsensorThread: %s"),szCause);

		theApp.ReportException(errorStr);

		e->Delete();
	}
	catch(...)
	{
		theApp.ReportException(_T("FlowsensorThread"));

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_EXCEPTION);
	}
	//((CThreadFlowsensor*)pc)->FlowsensorData();
	return TRUE;
}

/**********************************************************************************************//**
 * Flowsensor data
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	A DWORD.
 **************************************************************************************************/

DWORD CThreadFlowsensor::FlowsensorData(void) 
{
	CeSetThreadPriority(m_pcwtFlowsensorThread->m_hThread,130);

	do
	{
		if (::WaitForSingleObject(g_eventNewFlowsensorData, INFINITE) == WAIT_OBJECT_0)
		{
			if(m_bDoFlowsensorThread && getModel()->isSPIavailabel())//newSPI
			{
				if(getModel()->getSPI()->NewFlowsensorData())
				{
					int iState=getModel()->getDATAHANDLER()->GetFlowsensorStateBytes();
					getModel()->getDATAHANDLER()->ResetFlowsensorStateBytes();
					getModel()->getSPI()->ResetFlowsensorData();
					m_bCheckFlowsensorData=false;

					getModel()->ParseFlowsensorAlarmStateBytes(iState);

				}
				else if(m_bCheckFlowsensorData)
				{
					int iState=getModel()->getDATAHANDLER()->GetFlowsensorStateBytes();
					getModel()->getDATAHANDLER()->ResetFlowsensorStateBytes();

					getModel()->ParseFlowsensorAlarmStateBytes(iState);

					m_bCheckFlowsensorData=false;
				}
				else
				{
					getModel()->getDATAHANDLER()->ResetFlowsensorStateBytes();
					getModel()->getSPI()->ResetFlowsensorData();
					m_bCheckFlowsensorData=false;
				}
			}
		}

	}while(m_bDoFlowsensorThread);

	//theApp.getLog()->WriteLine(_T("#THR:025"));

	return 0;
}

/**********************************************************************************************//**
 * Sets check flowsensor data
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	bState	True to state.
 **************************************************************************************************/

void CThreadFlowsensor::setCheckFlowsensorData(bool bState)
{
	m_bCheckFlowsensorData=bState;
}