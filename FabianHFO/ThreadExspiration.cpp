// ThreadExspiration.cpp : implementation file
//

#include "stdafx.h"
#include "FabianLib.h"
#include "ThreadExspiration.h"
#include "MVModel.h"

extern CEvent g_eventExspiration;

// CThreadExspiration
CThreadExspiration* CThreadExspiration::theThreadExspiration=0;

CThreadExspiration::CThreadExspiration()
{
	m_pModel = NULL;

	m_bDoExspirationThread=false;
	m_pcwtExspirationThread=NULL;
	m_hThreadExspiration=INVALID_HANDLE_VALUE;
	

}

//************************************
// Method:    ~CThreadExspiration
// FullName:  CThreadExspiration::~CThreadExspiration
// Access:    protected 
// Returns:   
// Qualifier:
//
// 2015/06/19: checked for correct closing of thread
//************************************
CThreadExspiration::~CThreadExspiration()
{
	stopExspirationThread();

	if(m_pcwtExspirationThread!=NULL)
	{
		delete m_pcwtExspirationThread;
		m_pcwtExspirationThread=NULL;
		if(m_hThreadExspiration!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadExspiration);
			m_hThreadExspiration=INVALID_HANDLE_VALUE;
		}
	}
}
//=============================================================================
/**
 * @brief Get the instance of CThreadExspiration thread (singleton).
 *
 * @return the instance of CThreadExspiration thread
 *
 **/
//=============================================================================
CThreadExspiration* CThreadExspiration::getInstance()
{
	if(theThreadExspiration == 0)
	{
		theThreadExspiration = new CThreadExspiration;
	}
	return theThreadExspiration;
}

//=============================================================================
/**
 * @brief Destroys the instance of Flowsensor thread (singleton).
 **/
//=============================================================================
void CThreadExspiration::destroyInstance()
{
	if(theThreadExspiration != NULL)
	{
		delete theThreadExspiration;
		theThreadExspiration = NULL;
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
CMVModel *CThreadExspiration::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}


// CThreadMonitor message handlers
// **************************************************************************
// 
// **************************************************************************
void CThreadExspiration::startExspirationThread( void )
{
	m_bDoExspirationThread=true;

	if(m_pcwtExspirationThread!=NULL)
	{
		delete m_pcwtExspirationThread;
		m_pcwtExspirationThread=NULL;

		if(m_hThreadExspiration!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadExspiration);
			m_hThreadExspiration=INVALID_HANDLE_VALUE;
		}

	}

	m_pcwtExspirationThread=AfxBeginThread(ExspirationThread,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	m_hThreadExspiration=m_pcwtExspirationThread->m_hThread;
	m_pcwtExspirationThread->m_bAutoDelete = FALSE; 
	m_pcwtExspirationThread->ResumeThread();
}

//************************************
// Method:    stopExspirationThread
// FullName:  CThreadExspiration::stopExspirationThread
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: void
//
// 2015/06/19: checked for correct closing of thread
//************************************
void CThreadExspiration::stopExspirationThread( void )
{
	if(m_bDoExspirationThread)
	{
		m_bDoExspirationThread=false;
		g_eventExspiration.SetEvent();

		if (WaitForSingleObject(m_pcwtExspirationThread->m_hThread,1000) == WAIT_TIMEOUT)
		{
			theApp.getLog()->WriteLine(_T("#THR:Exspirationa"));
			if(!TerminateThread(m_pcwtExspirationThread,0))
			{
				theApp.getLog()->WriteLine(_T("#THR:Exspirationb"));
			}
		}
	}
}

// **************************************************************************
// 
// **************************************************************************
static UINT ExspirationThread( LPVOID pc )
{
	try
	{
		((CThreadExspiration*)pc)->Exspirationstart();
	}
	catch (CException* e)
	{
		TCHAR   szCause[255];
		e->GetErrorMessage(szCause, 255);

		CString errorStr=_T("");
		errorStr.Format(_T("ExspirationThread: %s"),szCause);

		theApp.ReportException(errorStr);

		e->Delete();
	}
	catch(...)
	{
		theApp.ReportException(_T("ExspirationThread"));

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_EXCEPTION);
	}
	//((CThreadExspiration*)pc)->Exspirationstart();
	return TRUE;
}
//************************************
// Method:    Exspirationstart
// FullName:  CThreadExspiration::Exspirationstart
// Access:    private 
// Returns:   DWORD
// Qualifier:
// Parameter: void
//************************************
DWORD CThreadExspiration::Exspirationstart(void) 
{
	CeSetThreadPriority(m_pcwtExspirationThread->m_hThread,130);

	do
	{
		if (::WaitForSingleObject(g_eventExspiration, INFINITE) == WAIT_OBJECT_0)
		{
			if(m_bDoExspirationThread)
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_EXSPIRATION);
			}
		}
	}while(m_bDoExspirationThread);

	theApp.getLog()->WriteLine(_T("#THR:026"));

	return 0;
}
