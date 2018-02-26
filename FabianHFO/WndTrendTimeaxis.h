/**********************************************************************************************//**
 * \file	WndTrendTimeaxis.h.
 *
 * Declares the window trend timeaxis class
 **************************************************************************************************/

#pragma once
//#include "NumBtn.h"
//#include "MenuBtn.h"
#include "Bmp.h"
#include "MVView.h"
// CWndMenuGraphs

class CWndTrendTimeaxis : public CWnd
{
	DECLARE_DYNAMIC(CWndTrendTimeaxis)

public:
	CWndTrendTimeaxis(CMVView *parentView);
	virtual ~CWndTrendTimeaxis();

	void Init();
	void Show(BOOL bShow);


	virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//void SetComplTimeRange(COleDateTime startTime, COleDateTime endTime);
	//void SetCurrTimeRange(int iOffset, int iTimeSpan);
	void SetTimeRange(COleDateTime dtStartTime, COleDateTime dtEndTime,int iOffset, int iTimeSpan);

protected:

	void Draw();

	CMVModel *getModel();

	//void ShowMenuBtn();
	//void HideMenuBtn();

	//void SetOneButtonDepressed(int btnID);
	//void SetAllButtonUnpressed();


private:
	CMVView *m_parentView;
	CMVModel *m_pModel;

	// Attributes
	HDC			m_hDC;
	HBITMAP		m_hBmp;
	HBITMAP		m_hBmpPrev;
	LONG		m_lX;
	LONG		m_lY;
	LONG		m_lXo;
	LONG		m_lYo;

	COleDateTime m_dtStartTime;
	COleDateTime m_dtEndTime;

	int m_iOffsetMinutes;
	int m_iTimeSpanMinutes;
	double m_dFactor;
	
	CBmp* m_pcTimeAxis;
	
	int m_iXStartPos;
	//int m_iYStartPos;

	
protected:
	//{{AFX_MSG(CWndMenuTrend)
	afx_msg void OnDestroy();
	//afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};






