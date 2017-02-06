// ThreadFlowsensor.cpp : implementation file
//

#include "stdafx.h"
#include "Fabianlib.h"
#include "ThreadFlowsensor.h"
#include "MVModel.h"

extern CEvent g_eventNewFlowsensorData;

// CThreadFlowsensor
CThreadFlowsensor* CThreadFlowsensor::theThreadFlowsensor=0;

CThreadFlowsensor::CThreadFlowsensor()
{
	m_pModel = NULL;

	m_bDoFlowsensorThread=false;
	m_pcwtFlowsensorThread=NULL;
	m_hThreadFlowsensor=INVALID_HANDLE_VALUE;
	
	m_bCheckFlowsensorData=false;
}

//************************************
// Method:    ~CThreadFlowsensor
// FullName:  CThreadFlowsensor::~CThreadFlowsensor
// Access:    protected 
// Returns:   
// Qualifier:
//
// 2015/06/19: checked for correct closing of thread
//************************************
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
//=============================================================================
/**
 * @brief Get the instance of Flowsensor thread (singleton).
 *
 * @return the instance of Flowsensor thread
 *
 **/
//=============================================================================
CThreadFlowsensor* CThreadFlowsensor::getInstance()
{
	if(theThreadFlowsensor == 0)
	{
		theThreadFlowsensor = new CThreadFlowsensor;
	}
	return theThreadFlowsensor;
}

//=============================================================================
/**
 * @brief Destroys the instance of Flowsensor thread (singleton).
 **/
//=============================================================================
void CThreadFlowsensor::destroyInstance()
{
	if(theThreadFlowsensor != NULL)
	{
		delete theThreadFlowsensor;
		theThreadFlowsensor = NULL;
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
CMVModel *CThreadFlowsensor::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}


// CThreadMonitor message handlers
// **************************************************************************
// 
// **************************************************************************
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

//************************************
// Method:    stopFlowsensorThread
// FullName:  CThreadFlowsensor::stopFlowsensorThread
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: void
//
// 2015/06/19: checked for correct closing of thread
//************************************
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

// **************************************************************************
// 
// **************************************************************************
static UINT FlowsensorThread( LPVOID pc )
{
	((CThreadFlowsensor*)pc)->FlowsensorData();
	return TRUE;
}
// **************************************************************************
// 
// **************************************************************************
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

	theApp.getLog()->WriteLine(_T("#THR:025"));

	return 0;
}

void CThreadFlowsensor::setCheckFlowsensorData(bool bState)
{
	m_bCheckFlowsensorData=bState;
}