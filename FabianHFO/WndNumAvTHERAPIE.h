#pragma once

#include "WndNumeric.h"

// CWndNumAvTHERAPIE

class CWndNumAvTHERAPIE : public CWndNumeric
{
	DECLARE_DYNAMIC(CWndNumAvTHERAPIE)

public:
	CWndNumAvTHERAPIE();
	virtual ~CWndNumAvTHERAPIE();

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

	int m_iOffset;

	CString m_szName1;
	CString m_szName2;
	CString m_szName3;
	CString m_szName4;

	CString m_szNameNote1;

	CString m_szUnitPressure;
	CString m_szUnitLMIN;

	//{{AFX_MSG(CWndNumAvNCPAP)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
