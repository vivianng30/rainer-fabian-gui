// FabianHFO.h : main header file for the FabianHFO application
//
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
	static void ReportErrorException(CString lpszFunction);
	
	
	Logfile *getLog();
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
