#pragma once
#include "Bmp.h"
#include "colour.h"
#include "globDefs.h"
#include "MVModel.h"

// CWndWaitHourglass

class CWndWaitHourglass : public CWnd
{
	DECLARE_DYNAMIC(CWndWaitHourglass)

public:
	CWndWaitHourglass();
	virtual ~CWndWaitHourglass();

	//bool CreateWnd(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CString szMessage=_T(""));

	void Show(BOOL bShow);
	void SetMessage(CStringW szText);
	/*bool StartTimer();
	bool StopTimer();*/


	void startWaitThread();
	void stopWaitThread();

	CMVModel *getModel();

private:
	CMVModel* m_pModel;

	HDC			m_hDC;
	HBITMAP		m_hBmp;
	HBITMAP		m_hBmpPrev;
	LONG		m_lX;
	LONG		m_lY;
	LONG		m_lXo;
	LONG		m_lYo;

	CBmp* m_pcWait;
	CBmp* m_pcWait1;
	CBmp* m_pcWait2;
	CBmp* m_pcWait3;
	CBmp* m_pcWait4;
	CBmp* m_pcWait5;
	CBmp* m_pcWait6;
	CBmp* m_pcWait7;
	CBmp* m_pcWait8;
	//CBmp* m_pcWait9;

	int m_iWaitCount;

	CString m_szMessage;

	friend UINT CWaitThread(LPVOID pc);
	CWinThread*	m_pcwtWaitThread;
	DWORD WaitHourglass(void);
	bool m_bDoWaitThread;
	HANDLE m_hThreadWait;

protected:
	DECLARE_MESSAGE_MAP()
	
	//{{AFX_MSG(CWndMenuTrend)
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	//afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG
};


