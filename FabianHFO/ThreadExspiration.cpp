// ThreadExspiration.cpp : implementation file
//

#include "stdafx.h"
#include "FabianLib.h"
#include "ThreadExspiration.h"
#include "MVModel.h"

extern CEvent g_eventExspiration;

// CThreadExspiration
CThreadExspiration* CThreadExspiration::theThreadExspiration=0;

/**********************************************************************************************//**
 * Initializes a new instance of the CThreadExspiration class
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

CThreadExspiration::CThreadExspiration()
{
	m_pModel = NULL;

	m_bDoExspirationThread=false;
	m_pcwtExspirationThread=NULL;
	m_hThreadExspiration=INVALID_HANDLE_VALUE;
	

}

/**********************************************************************************************//**
 * Finalizes an instance of the CThreadExspiration class
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Gets the instance
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	Null if it fails, else the instance.
 **************************************************************************************************/

CThreadExspiration* CThreadExspiration::getInstance()
{
	if(theThreadExspiration == 0)
	{
		theThreadExspiration = new CThreadExspiration;
	}
	return theThreadExspiration;
}

/**********************************************************************************************//**
 * Destroys the instance
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CThreadExspiration::destroyInstance()
{
	if(theThreadExspiration != NULL)
	{
		delete theThreadExspiration;
		theThreadExspiration = NULL;
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

CMVModel *CThreadExspiration::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}

/**********************************************************************************************//**
 * Starts exspiration thread
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Stops exspiration thread
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Exspiration thread
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	pc	The PC.
 *
 * \return	An UINT.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Gets the exspirationstart
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	A DWORD.
 **************************************************************************************************/

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

	//theApp.getLog()->WriteLine(_T("#THR:026"));

	return 0;
}
