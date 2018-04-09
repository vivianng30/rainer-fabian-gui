//=============================================================================
/** 
* \class CFabianHFOApp
* 		 
* \brief The fabianHFO application.
* 		 
* \author Rainer Kuehner
* \date 2013-01-21 Creation of file
*
* The data controller handles all internal data and send commands to its associated 
* views to change the view's presentation of the model.
*
**/
//=============================================================================
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#ifdef STANDARDSHELL_UI_MODEL
#include "resource.h"
#endif


#include "logfile.h"
#include "globDefs.h"

// CFabianHFOApp:
// See FabianHFO.cpp for the implementation of this class
//

class CFabianHFOApp : public CWinApp
{
public:
	CFabianHFOApp();
	~CFabianHFOApp();

	//static void ReportException(CString message);
	static void ReportException(CString lpszFunction);
	
	void initLog();
	
	
	void WriteLog(CStringW inhalt);
	bool CheckLogDate();
	int writeLogError (LONG lError, LPCTSTR lptszMessage);
	
	void OpenLog();
	void CloseLog();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	bool GetEXITState();
	void SetEXITState();
	void SetScreenlock(bool state);
	bool IsScreenlock();
	bool IsAutoScreenlockActive();
	void SetAutoScreenlockActive(bool state);

	bool IsNetDCU9();
	bool IsNetDCU11();
	//int filter(unsigned int code, struct _EXCEPTION_POINTERS *ep);
protected:
	bool isSafeTickCountDelayExpired(DWORD oldTickCount, UINT delay=0);//used for (dwLastTickCount+DELAY<getTickCount64())

	Logfile *getLog();
// Implementation
private:
	bool m_bScreenLock;
	bool m_bAutoScreenLockActive;
	DWORD m_dwLastAutoScreenTimer;

	bool m_bNetDCU9;
	bool m_bNetDCU11;

	bool m_bEXITState;

	static Logfile *log;

	
public:
	

	DECLARE_MESSAGE_MAP()
};

extern CFabianHFOApp theApp;
