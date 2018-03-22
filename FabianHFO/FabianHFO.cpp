// FabianHFO.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "MainFrm.h"
#include "TlsFile.h"
#include "TlsRegistry.h"
//#include <iostream>
//#include <exception>

#ifdef _DEBUG

/**********************************************************************************************//**
 * A macro that defines new
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

#define new DEBUG_NEW
#endif


//CString CFabianHFOApp::exception;
Logfile *CFabianHFOApp::log=NULL;
// CFabianHFOApp
BEGIN_MESSAGE_MAP(CFabianHFOApp, CWinApp)
END_MESSAGE_MAP()


//struct MyException : public exception
//{
//	const char * logExeption () const throw ()
//	{
//		return "C++ Exception";
//	}
//};

/**********************************************************************************************//**
 * CFabianHFOApp construction
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

CFabianHFOApp::CFabianHFOApp()
	: CWinApp()
{
	// Place all significant initialization in InitInstance
	m_bEXITState=false;
	m_bScreenLock=false;
	m_bAutoScreenLockActive=false;
	m_dwLastAutoScreenTimer=0;


	m_bNetDCU9=false;
	m_bNetDCU11=false;

	if(CTlsRegistry::IsValid(_T("HKLM\\Drivers\\Display\\PXA27x\\Mode20")))
	{
		CTlsRegistry regDisplay(_T("HKLM\\Drivers\\Display\\PXA27x\\Mode20"),true);

		if(regDisplay.ReadDWORD(_T("PPL"), 800)==800)
		{
			CTlsRegistry regPlatform(_T("HKLM\\Platform"),true);

			if(regPlatform.ReadString(_T("Name"))==_T("NetDCU11"))
				m_bNetDCU11=true;
		}
		else
		{
			AfxMessageBox(_T("Wrong display resolution!"));
		}
	}
	else if(CTlsRegistry::IsValid(_T("HKLM\\Drivers\\Display\\SMIVGX\\Mode20")))
	{
		CTlsRegistry regPlatform(_T("HKLM\\Platform"),true);

		if(regPlatform.ReadString(_T("Name"))==_T("NetDCU9"))
			m_bNetDCU9=true;
	}
}

/**********************************************************************************************//**
 * Destructor
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

CFabianHFOApp::~CFabianHFOApp()
{
	delete log;
	log=NULL;
}

/**********************************************************************************************//**
 * Query if this instance is net dcu 9
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if net dcu 9, false if not.
 **************************************************************************************************/

bool CFabianHFOApp::IsNetDCU9()
{
	return m_bNetDCU9;
}

/**********************************************************************************************//**
 * Query if this instance is net dcu 11
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if net dcu 11, false if not.
 **************************************************************************************************/

bool CFabianHFOApp::IsNetDCU11()
{
	return m_bNetDCU11;
}

/**********************************************************************************************//**
 * Gets exit state
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CFabianHFOApp::GetEXITState()
{
	return m_bEXITState;
}

/**********************************************************************************************//**
 * Sets exit state
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CFabianHFOApp::SetEXITState()
{
	m_bEXITState=true;
}

// The one and only CFabianHFOApp object
CFabianHFOApp theApp;   ///< the application

/**********************************************************************************************//**
 * CFabianHFOApp initialization
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

BOOL CFabianHFOApp::InitInstance()
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Acutronic MedicalSystems AG"));
	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object

	

	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// create and load the frame with its resources
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL, NULL);


	// The one and only window has been initialized, so show and update it
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	return TRUE;
}

/**********************************************************************************************//**
 * Initializes the log
 *
 * \author	Rainer Kühner
 * \date	22.03.2018
 **************************************************************************************************/

void CFabianHFOApp::initLog()
{
	CStringA aPathLog = "";
	CStringW wPathLog = _T("\\sdcard\\");
	if(CTlsFile::Exists(wPathLog)==true)
	{
		aPathLog = "\\sdcard\\Logs\\systemlog.txt";
	}
	else
	{
		aPathLog = "\\FFSDISK\\Logs\\systemlog.txt";
	}


	log = new Logfile(aPathLog);
	log->WriteLine(_T(""));
	log->WriteLine(_T("*******************"));
	log->WriteLine(_T("**** systemlog ****"));
	log->writeDate();
}

/**********************************************************************************************//**
 * Opens the log
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CFabianHFOApp::OpenLog()
{
	if(log==NULL)
	{
		CStringA aPathLog = "";
		CStringW wPathLog = _T("\\sdcard\\");
		if(CTlsFile::Exists(wPathLog)==true)
		{
			aPathLog = "\\sdcard\\Logs\\systemlog.txt";
		}
		else
		{
			aPathLog = "\\FFSDISK\\Logs\\systemlog.txt";
		}
		log = new Logfile(aPathLog);
		log->WriteLine(_T(""));
		log->WriteLine(_T("*******************"));
		log->WriteLine(_T("!!!! REOPEN systemlog !!!!"));
		log->writeDate();
	}
}

/**********************************************************************************************//**
 * Closes the log
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CFabianHFOApp::CloseLog()
{
	log->Exit();
	
	delete log;
	log=NULL;
}

/**********************************************************************************************//**
 * Gets the log
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	Null if it fails, else the log.
 **************************************************************************************************/

Logfile *CFabianHFOApp::getLog()
{
	if(log==NULL)
	{
		CStringA aPathLog = "";
		CStringW wPathLog = _T("\\sdcard\\");
		if(CTlsFile::Exists(wPathLog)==true)
		{
			aPathLog = "\\sdcard\\Logs\\systemlog.txt";
		}
		else
		{
			aPathLog = "\\FFSDISK\\Logs\\systemlog.txt";
		}
		log = new Logfile(aPathLog);
	}
	return log;
}

/**********************************************************************************************//**
 * Writes a log error
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	lError			The error.
 * \param	lptszMessage	Message describing the lptsz.
 *
 * \return	An int.
 **************************************************************************************************/

int CFabianHFOApp::writeLogError (LONG lError, LPCTSTR lptszMessage)
{
	// Generate a message text
	TCHAR tszMessage[256];
	wsprintf(tszMessage,_T("#HFO: %s (code %d)"), lptszMessage, lError);

	getLog()->WriteLine(tszMessage);
	
	return 1;
}

/**********************************************************************************************//**
 * Reports an exception
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	lpszFunction	The function.
 **************************************************************************************************/

void CFabianHFOApp::ReportException(CString lpszFunction)
{
	// Retrieve the system error message for the last-error code

	CString szError=_T("");
	szError.Format(_T("EXCEPTION: %s failed with error #%d"),lpszFunction,GetLastError());

	CFabianHFOApp *pApp = NULL;
	(CFabianHFOApp *)pApp = (CFabianHFOApp *)AfxGetApp();
	if (pApp != NULL && pApp->log!=NULL)
	{
		pApp->log->WriteLine(szError);
	}
}

//void CFabianHFOApp::ReportException(CString message)
//{
//	try
//	{
//		CFabianHFOApp *pApp = NULL;
//		(CFabianHFOApp *)pApp = (CFabianHFOApp *)AfxGetApp();
//		if (pApp != NULL && pApp->log!=NULL)
//		{
//			pApp->log->WriteLine(message);
//			//pApp->log->SaveLog();
//		}
//	}
//	catch (...)
//	{
//		throw;
//	}
//}

/**********************************************************************************************//**
 * Sets automatic screenlock active
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	state	True to state.
 **************************************************************************************************/

void CFabianHFOApp::SetAutoScreenlockActive(bool state)
{
	m_bAutoScreenLockActive=state;
	if(state)
	{
		log->WriteLine(_T("**** auto screenlock enabled ****"));
	}
	else
	{
		log->WriteLine(_T("**** auto screenlock disabled ****"));
	}
}

/**********************************************************************************************//**
 * Queries if the automatic screenlock is active
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if the automatic screenlock is active, false if not.
 **************************************************************************************************/

bool CFabianHFOApp::IsAutoScreenlockActive()
{
	return m_bAutoScreenLockActive;
}

/**********************************************************************************************//**
 * Sets a screenlock
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	state	True to state.
 **************************************************************************************************/

void CFabianHFOApp::SetScreenlock(bool state)
{
	m_bScreenLock=state;
	if(state)
	{
		log->WriteLine(_T("**** screenlock enabled ****"));
		DEBUGMSG(TRUE, (TEXT("screenlock enabled\r\n")));
	}
	else
	{
		log->WriteLine(_T("**** screenlock disabled ****"));
		DEBUGMSG(TRUE, (TEXT("screenlock disabled\r\n")));
	}
}

/**********************************************************************************************//**
 * Query if this instance is screenlock
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if screenlock, false if not.
 **************************************************************************************************/

bool CFabianHFOApp::IsScreenlock()
{
	return m_bScreenLock;
}

//int CFabianHFOApp::filter(unsigned int code, struct _EXCEPTION_POINTERS *ep) {
//
//	//puts("in filter.");
//
//	if (code == EXCEPTION_ACCESS_VIOLATION) {
//
//		//puts("caught AV as expected.");
//
//		return EXCEPTION_EXECUTE_HANDLER;
//
//	}
//
//	else {
//
//		//puts("didn't catch AV, unexpected.");
//
//		return EXCEPTION_CONTINUE_SEARCH;
//
//	};
//
//}

/**********************************************************************************************//**
 * Translates all messages before they are processed by the main message loop
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param [in,out]	pMsg	If non-null, the message.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

BOOL CFabianHFOApp::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message)
	{
	case WM_LBUTTONDBLCLK:
		{
			if(m_bScreenLock)
			{
				if(m_pMainWnd)
					m_pMainWnd->PostMessage(WM_PIF_SIGNAL);
				return true;
			}
			else if(m_bAutoScreenLockActive)
			{
				if(isSafeTickCountDelayExpired(m_dwLastAutoScreenTimer, 1000))
				{
					m_dwLastAutoScreenTimer=GetTickCount();
					if(m_pMainWnd)
						m_pMainWnd->PostMessage(WM_SET_AUTOSCREENLOG_TIMER);
				}
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			if(m_bScreenLock)
			{
				if(m_pMainWnd)
					m_pMainWnd->PostMessage(WM_PIF_SIGNAL);
				return true;
			}
			else if(m_bAutoScreenLockActive)
			{
				if(isSafeTickCountDelayExpired(m_dwLastAutoScreenTimer, 1000))
				{
					m_dwLastAutoScreenTimer=GetTickCount();
					if(m_pMainWnd)
						m_pMainWnd->PostMessage(WM_SET_AUTOSCREENLOG_TIMER);
				}
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			if(m_bScreenLock)
			{
				return true;
			}
		}
		break;
	/*case WM_MOUSEMOVE:
		{
			return 1;
		}
		break;*/
	case WM_KEYDOWN:
		{
			if(pMsg->wParam==VK_SPACE)
			{
				if(m_bScreenLock)
				{
					m_bScreenLock=false;
					log->WriteLine(_T("**** screenlock disabled VK_SPACE****"));
					DEBUGMSG(TRUE, (TEXT("screenlock disabled\r\n")));
					if(m_pMainWnd)
					{
						m_pMainWnd->PostMessage(WM_SCREENFREE);
						m_pMainWnd->SetFocus();
					}
					return true;
				}
				else if(m_bAutoScreenLockActive)
				{
					if(isSafeTickCountDelayExpired(m_dwLastAutoScreenTimer, 1000))
					{
						m_dwLastAutoScreenTimer=GetTickCount();
						if(m_pMainWnd)
							m_pMainWnd->PostMessage(WM_SET_AUTOSCREENLOG_TIMER);
					}
				}
			}
			else if(pMsg->wParam==VK_UP)
			{
				if(m_bAutoScreenLockActive)
				{
					if(isSafeTickCountDelayExpired(m_dwLastAutoScreenTimer, 1000))
					{
						m_dwLastAutoScreenTimer=GetTickCount();
						if(m_pMainWnd)
							m_pMainWnd->PostMessage(WM_SET_AUTOSCREENLOG_TIMER);
					}
				}
			}
			else if(pMsg->wParam==VK_DOWN)
			{
				if(m_bAutoScreenLockActive)
				{
					if(isSafeTickCountDelayExpired(m_dwLastAutoScreenTimer, 1000))
					{
						m_dwLastAutoScreenTimer=GetTickCount();
						if(m_pMainWnd)
							m_pMainWnd->PostMessage(WM_SET_AUTOSCREENLOG_TIMER);
					}
				}
			}
		}
		break;
	default:
		break;
	}
	return CWinApp::PreTranslateMessage(pMsg);
}

/**********************************************************************************************//**
 * rkuTICKCOUNT
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	oldTickCount	Number of old ticks.
 * \param	delay			The delay.
 *
 * \return	True if safe tick count delay expired, false if not.
 **************************************************************************************************/

bool CFabianHFOApp::isSafeTickCountDelayExpired(DWORD oldTickCount, UINT delay)////used to check if old tick count plus delay is still lower than actual tickCount, (dwLastTickCount+DELAY<getTickCount64())
{
	bool bExpired=false;
	DWORD curTickCount=GetTickCount();

	if(curTickCount<oldTickCount)//overrun
	{
		DWORD diff=0xFFFFFFFF-oldTickCount;

		if(diff+delay<curTickCount)
		{
			bExpired=true;
		}
	}
	else
	{
		if(oldTickCount+delay<curTickCount)
		{
			bExpired=true;
		}
	}
	return bExpired;
}