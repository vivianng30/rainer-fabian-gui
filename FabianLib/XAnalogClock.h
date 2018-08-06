// XAnalogClock.h  Version 1.0
//
// Author:  Hans Dietrich
//          hdietrich@gmail.com
//
// This software is released into the public domain.  You are free to use
// it in any way you like, except that you may not sell this source code.
//
// This software is provided "as is" with no expressed or implied warranty.
// I accept no liability for any damage or loss of business that this
// software may cause.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef XANALOGCLOCK_H
#define XANALOGCLOCK_H

#include "CDCBuffer.h"

/////////////////////////////////////////////////////////////////////////////
// CXAnalogClock window

class CXAnalogClock : public CStatic
{
// Construction
public:
	CXAnalogClock();
	virtual ~CXAnalogClock();

// Attributes
public:
	//CTime GetTime() { return m_time; }
	COleDateTime GetTime() { return m_dtTime; }
	CSize GetWindowSize();
	BOOL IsClockRunning() { return !m_bStopped; }
	void SetBackgroundColor(COLORREF rgb) { m_rgbBackground = rgb; }
	void SetBitmaps(UINT nFaceBitmapId, bool bRedraw)
	{
		if (m_cdcClockFace.IsValid())
			m_cdcClockFace.DeleteObject();
		/*if (m_cdcDate.IsValid())
			m_cdcDate.DeleteObject();*/
		if (m_cdcPrevious.IsValid())
			m_cdcPrevious.DeleteObject();

		m_nFaceBitmapId = nFaceBitmapId;
		//m_nDateBitmapId = nDateBitmapId;

		if(bRedraw)
		{
			Invalidate();
			UpdateWindow();
		}
	}
	void SetGmtAdjust(float dAdjust) { m_dGmtAdjust = dAdjust; }
	void SetHourHandColor(COLORREF rgb) { m_rgbHourHand = rgb; }
	void SetMinuteHandColor(COLORREF rgb) { m_rgbMinuteHand = rgb; }
	void SetSecondHandColor(COLORREF rgb) { m_rgbSecondHand = rgb; }
	//void SetTime(CTime& t) { m_time = t; }
	void SetTime(COleDateTime& t) { m_dtTime = t; }
	void SetTransparentColor(COLORREF rgb) { m_rgbTransparent = rgb; }
	void ShowDate(BOOL bShowDate) { m_bShowDate = bShowDate; }
	void ShowSecondHand(BOOL bShowSecondHand) { m_bShowSecondHand = bShowSecondHand; }
	void UseSystemTime(BOOL bUseSystemTime) { m_bUseSystemTime = bUseSystemTime; }

// Operations
public:
	void Run();
	void Stop();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXAnalogClock)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
protected:
	int GetDay();
	int GetHour();
	int GetMinute();
	int GetSecond();

	float ConvertToDegrees(float dTime);
	void LoadBitmaps(CDC *pDC);
	//void PaintDate(CDC *pDC);
	void PaintMinuteAndHourHands(CDC *pDC);
	void PaintSecondHand(CDC *pDC);

	CDCBuffer	m_cdcClockFace;
	//CDCBuffer	m_cdcDate;
	CDCBuffer	m_cdcPrevious;
	int			m_nPrevMinute;
	BOOL		m_bShowDate;
	BOOL		m_bShowSecondHand;
	BOOL		m_bUseSystemTime;
	BOOL		m_bStopped;
	float		m_dGmtAdjust;
	//CTime		m_time;
	COleDateTime m_dtTime;
	COLORREF	m_rgbBackground;
	COLORREF	m_rgbTransparent;
	COLORREF	m_rgbSecondHand;
	COLORREF	m_rgbMinuteHand;
	COLORREF	m_rgbHourHand;
	UINT		m_nFaceBitmapId;
	//UINT		m_nDateBitmapId;

	int m_iPrevDay;

	// Generated message map functions
protected:
	//{{AFX_MSG(CXAnalogClock)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif //XANALOGCLOCK_H
