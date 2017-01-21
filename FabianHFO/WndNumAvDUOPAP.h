#pragma once

#include "WndNumeric.h"



class CWndNumAvDUOPAP : public CWndNumeric
{
	DECLARE_DYNAMIC(CWndNumAvDUOPAP)

public:
	CWndNumAvDUOPAP();
	virtual ~CWndNumAvDUOPAP();

	bool DrawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter);
	

private:
	
	bool DrawFrames(CDC* pDC);
	bool DrawStaticText(CDC* pDC);
	bool DrawLimits(CDC* pDC);
	
protected:

	void StartFlowThread(void);
	void StopFlowThread( void );

	CRITICAL_SECTION	csDoThread;

	friend UINT CFlowThread(LPVOID pc);
	CWinThread*	m_pcwtFlowThread;
	DWORD DoFlow(void);
	bool m_bDoThread;
	CEvent eventDoFlow;
	HANDLE m_hThreadFlow;

	bool doThread();
	void startThread();
	void stopThread();

	CString m_szName1;
	CString m_szName2;
	CString m_szName3;
	CString m_szName4;
	CString m_szName5;

	CString m_szNameNote1;
	CString m_szNameNote2;

	CString m_szUnitPressure;
	CString m_szUnitLMIN;

	//{{AFX_MSG(CWndNumAvDUOPAP)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};





