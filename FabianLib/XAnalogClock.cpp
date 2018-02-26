// XAnalogClock.cpp  Version 1.0
//
// Author:  Hans Dietrich
//          hdietrich@gmail.com
//
// Description:
//     XAnalogClock implements CXAnalogClock, a skinnable analog clock control.
//
// History
//     Version 1.0 - 2006 March 23
//     - Initial public release
//
// Public APIs:
//          NAME                           DESCRIPTION
//     ---------------------    -------------------------------------------
//     GetTime()                Get currently displayed time
//     GetWindowSize()          Get control size
//     IsClockRunning()         Returns run state of clock
//     SetBackgroundColor()     Set background color
//     SetBitmaps()             Set bitmap resource IDs
//     SetGmtAdjust()           Not currently used
//     SetHourHandColor()       Set color of hour hand
//     SetMinuteHandColor()     Set color of minute hand
//     SetSecondHandColor()     Set color of second hand
//     SetTime()                Set starting time (if system time not used)
//     SetTransparentColor()    Set transparent color
//     ShowDate()               Show/hide date
//     ShowSecondHand()         Show/hide second hand
//     Run()                    Start the clock
//     Stop()                   Stop the clock
//     UseSystemTime()          Use system time, or user-supplied time (see
//                              SetTime()
//
// License:
//     This software is released into the public domain.  You are free to use
//     it in any way you like, except that you may not sell this source code.
//
//     This software is provided "as is" with no expressed or implied warranty.
//     I accept no liability for any damage or loss of business that this
//     software may cause.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XAnalogClock.h"
#include "math.h"
#include "WuLine.h"
#include "globDefs.h"

#ifdef _DEBUG

/**********************************************************************************************//**
 * A macro that defines new
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__; ///< this file[]
#endif

/**********************************************************************************************//**
 * /////////////////////////////////////////////////////////////////////////////
 *  some definitions
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

#define DATE_BOX_WIDTH		18

/**********************************************************************************************//**
 * A macro that defines date box height
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

#define DATE_BOX_HEIGHT		14

/**********************************************************************************************//**
 * A macro that defines date box xpos
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

#define DATE_BOX_XPOS		67

/**********************************************************************************************//**
 * A macro that defines date box ypos
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

#define DATE_BOX_YPOS		43

/**********************************************************************************************//**
 * #define SECOND_HAND_LENGTH	35.0
 * #define MINUTE_HAND_LENGTH	39.0
 * #define HOUR_HAND_LENGTH	28.0
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

#define SECOND_HAND_LENGTH	17.0

/**********************************************************************************************//**
 * A macro that defines minute hand length
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

#define MINUTE_HAND_LENGTH	17.0

/**********************************************************************************************//**
 * A macro that defines hour hand length
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

#define HOUR_HAND_LENGTH	11.0

/**********************************************************************************************//**
 * A macro that defines second hand color
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

#define SECOND_HAND_COLOR	RGB(220,20,60)

/**********************************************************************************************//**
 * A macro that defines minute hand color
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

#define MINUTE_HAND_COLOR	RGB(0,0,0)

/**********************************************************************************************//**
 * A macro that defines hour hand color
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

#define HOUR_HAND_COLOR		RGB(0,0,0)

/**********************************************************************************************//**
 * A macro that defines pi
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

#define PI					3.1415926535


BEGIN_MESSAGE_MAP(CXAnalogClock, CStatic)
	//{{AFX_MSG_MAP(CXAnalogClock)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * Initializes a new instance of the CXAnalogClock class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

CXAnalogClock::CXAnalogClock()
{
	m_nPrevMinute     = -1;
	m_bShowDate       = TRUE;
	m_bShowSecondHand = TRUE;
	m_bUseSystemTime  = TRUE;
	m_bStopped        = TRUE;
	//m_time            = CTime::GetCurrentTime();

	SYSTEMTIME st;
	GetLocalTime(&st);
	COleDateTime dtTime(st);
	m_dtTime		  =dtTime;
	//m_dtTime		  =COleDateTime::GetCurrentTime();

	//m_time            = CTime(2006, 1, 15, 8, 40, 0);
	m_dGmtAdjust      = 0.0;
	m_rgbBackground   = RGB(255,0,255);
	m_rgbTransparent  = RGB(255,0,255);
	m_rgbSecondHand   = SECOND_HAND_COLOR;
	m_rgbMinuteHand   = MINUTE_HAND_COLOR;
	m_rgbHourHand     = HOUR_HAND_COLOR;
	m_nFaceBitmapId   = 0;
	//m_nDateBitmapId   = 0;

	m_iPrevDay=GetDay();
}

/**********************************************************************************************//**
 * Finalizes an instance of the CXAnalogClock class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

CXAnalogClock::~CXAnalogClock()
{
}

/**********************************************************************************************//**
 * Pre subclass window
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CXAnalogClock::PreSubclassWindow()
{
	CClientDC dc(NULL);
	LoadBitmaps(&dc);

	CStatic::PreSubclassWindow();
}

/**********************************************************************************************//**
 * Gets window size
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	The window size.
 **************************************************************************************************/

CSize CXAnalogClock::GetWindowSize()
{
	CClientDC dc(NULL);
	LoadBitmaps(&dc);

	return m_cdcClockFace.m_sizeBitmap;
}

/**********************************************************************************************//**
 * Loads the bitmaps
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	pDC	If non-null, the device-context.
 **************************************************************************************************/

void CXAnalogClock::LoadBitmaps(CDC *pDC)
{
	if ((m_nFaceBitmapId == 0) /*|| (m_nDateBitmapId == 0)*/)
	{
		return;
	}

	// load clockface.bmp
	if (!m_cdcClockFace.IsValid())
		VERIFY(m_cdcClockFace.LoadBitmap(m_nFaceBitmapId, pDC));

	// load date.bmp
	/*if (!m_cdcDate.IsValid())
		VERIFY(m_cdcDate.LoadBitmap(m_nDateBitmapId, pDC));*/

	if (!m_cdcPrevious.IsValid())
		VERIFY(m_cdcPrevious.LoadBitmap(m_nFaceBitmapId, pDC));
}

/**********************************************************************************************//**
 * Paints this window
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CXAnalogClock::OnPaint()
{
	if ((m_nFaceBitmapId == 0) /*|| (m_nDateBitmapId == 0)*/)
	{
		return;
	}

	CPaintDC dc(this); // device context for painting

	LoadBitmaps(&dc);

	if (m_bUseSystemTime)
	{
		//m_time = CTime::GetCurrentTime();
		//m_dtTime		  =COleDateTime::GetCurrentTime();
		SYSTEMTIME st;
		GetLocalTime(&st);
		COleDateTime dtTime(st);
		m_dtTime=dtTime;
	}

	CRect rect;
	GetClientRect(&rect);

	if ((rect.Width() < m_cdcClockFace.m_sizeBitmap.cx) ||
		(rect.Height() < m_cdcClockFace.m_sizeBitmap.cy))
	{
		
	}

	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	CBitmap *pOldBitmap = memDC.SelectObject(&bitmap);

	// fill in background color
	CBrush brush(m_rgbBackground);
	memDC.FillRect(&rect, &brush);

	//if (m_nPrevMinute != GetMinute())
	{
		// draw clock face
		BitBlt(memDC.m_hDC,
			0, 0,
			m_cdcClockFace.m_sizeBitmap.cx, m_cdcClockFace.m_sizeBitmap.cy,
			m_cdcClockFace.m_dcMemory.m_hDC,
			0, 0,
			SRCCOPY);

		/*if (m_bShowDate)
			PaintDate(&memDC);*/

		PaintMinuteAndHourHands(&memDC);

		// save clock face & hands for this minute
		m_cdcPrevious.m_dcMemory.BitBlt(0, 0,
			m_cdcClockFace.m_sizeBitmap.cx, m_cdcClockFace.m_sizeBitmap.cy,
			&memDC, 0, 0, SRCCOPY);

		m_nPrevMinute = GetMinute();
	}
	//else
	//{
	//	// minute hasn't changed, so use previous clock face & hands
	//	memDC.BitBlt(0, 0,
	//		rect.Width(), rect.Height(),
	//		&m_cdcPrevious.m_dcMemory,
	//		0, 0, SRCCOPY);
	//}

	if (m_bShowSecondHand)
		PaintSecondHand(&memDC);

	// finally blt to screen
	BitBlt(dc.m_hDC,
		0, 0,
		rect.Width(), rect.Height(),
		memDC.m_hDC,
		0, 0,
		SRCCOPY);

	memDC.SelectObject(pOldBitmap);

	//DeleteObject(brush);//rkuNEWFIX

	// Do not call CStatic::OnPaint() for painting messages
}

/**********************************************************************************************//**
 * Converts a dTime to the degrees
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	dTime	The time.
 *
 * \return	The given data converted to the degrees.
 **************************************************************************************************/

float CXAnalogClock::ConvertToDegrees(float dTime)
{
	float degrees = (float)(90. - dTime * 6.0);
	if (degrees < 0.0)
		degrees = (float)360. + degrees;
	return degrees;
}

//void CXAnalogClock::PaintDate(CDC *pDC)
//{
//	LOGFONT lf;
//	CFont *pFont = GetFont();
//	if (pFont == NULL)
//		pFont = GetParent()->GetFont();
//	if (pFont)
//	{
//		pFont->GetLogFont(&lf);
//		//_tcscpy(lf.lfFaceName, _T("MS Sans Serif"));
//		_tcscpy(lf.lfFaceName, _T("Arial"));
//		//lf.lfWeight = FW_BOLD;
//		if (lf.lfHeight < 0)
//			lf.lfHeight += 1;
//		else
//			lf.lfHeight -= 1;
//
//		CFont font;
//		font.CreateFontIndirect(&lf);
//
//		CString strDate = _T("");
//		strDate.Format(_T("%2d"), GetDay());
//
//		CRect rect(0, 0, DATE_BOX_WIDTH, DATE_BOX_HEIGHT);
//
//		// create temporary dc to hold the date numerals
//		CDCBuffer cdcTemp;
//		VERIFY(cdcTemp.LoadBitmap(m_nDateBitmapId, pDC));
//		CFont *pOldFont = cdcTemp.m_dcMemory.SelectObject(&font);
//		CBrush brush(RGB(255,0,255));
//		cdcTemp.m_dcMemory.FillRect(&rect, &brush);
//		cdcTemp.m_dcMemory.SetBkColor(RGB(255,0,255));
//		cdcTemp.m_dcMemory.ExtTextOut(3, 1, 0, &rect, strDate, NULL);
//
//		// draw date box template
//		/*pDC->BitBlt(DATE_BOX_XPOS, DATE_BOX_YPOS, DATE_BOX_WIDTH, DATE_BOX_HEIGHT,
//			&m_cdcDate.m_dcMemory, 0, 0, SRCCOPY);*/
//
//		// overlay with date numerals
//		BitBlt(pDC->m_hDC,
//			DATE_BOX_XPOS+1, DATE_BOX_YPOS-1,
//			DATE_BOX_WIDTH, DATE_BOX_HEIGHT,
//			cdcTemp.m_dcMemory.m_hDC,
//			0, 0,
//			SRCCOPY);
//
//		cdcTemp.m_dcMemory.SelectObject(pOldFont);
//
//		DeleteObject(brush);
//
//	}
//}

/**********************************************************************************************//**
 * Paints the minute and hour hands described by pDC
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	pDC	If non-null, the device-context.
 **************************************************************************************************/

void CXAnalogClock::PaintMinuteAndHourHands(CDC *pDC)
{
	///////////////////////////////////////////////////////////////////////////
	// draw minute hand
	int minute = GetMinute();

	float degrees, radians, cosine, sine;
	short x1, y1, x2, y2;

	degrees = ConvertToDegrees((float)minute);
	radians = (float) ((PI * degrees)/180.);
	cosine = (float)cos(radians);
	sine = (float)sin(radians);

	x1 = (short)(MINUTE_HAND_LENGTH * cosine + (float)m_cdcClockFace.m_sizeBitmap.cx/2);
	y1 = (short)(-MINUTE_HAND_LENGTH * sine + (float)m_cdcClockFace.m_sizeBitmap.cy/2);
	//x2 = (short)(5.0 * cosine + (float)m_cdcClockFace.m_sizeBitmap.cx/2);
	//y2 = (short)(-5.0 * sine + (float)m_cdcClockFace.m_sizeBitmap.cy/2);
	x2 = (short)(1.0 * cosine + (float)m_cdcClockFace.m_sizeBitmap.cx/2);
	y2 = (short)(-1.0 * sine + (float)m_cdcClockFace.m_sizeBitmap.cy/2);

	if (((minute >= 10) && (minute <= 20)) || ((minute >= 40) && (minute <= 50)))
	{
		x1 = (short)(MINUTE_HAND_LENGTH * cosine + (float)m_cdcClockFace.m_sizeBitmap.cx/2);
		y1 = (short)(-MINUTE_HAND_LENGTH * sine + (float)m_cdcClockFace.m_sizeBitmap.cy/2);
		DrawWuLine(pDC, x1, y1, x2, y2, m_rgbMinuteHand);
		x1 = (short)((MINUTE_HAND_LENGTH-4) * cosine + (float)m_cdcClockFace.m_sizeBitmap.cx/2);
		y1 = (short)((-MINUTE_HAND_LENGTH+4) * sine + (float)m_cdcClockFace.m_sizeBitmap.cy/2);
		DrawWuLine(pDC, x1, y1, x2, y2-1, m_rgbMinuteHand);
		DrawWuLine(pDC, x1, y1, x2, y2-0, m_rgbMinuteHand);
		DrawWuLine(pDC, x1, y1, x2, y2+0, m_rgbMinuteHand);
		DrawWuLine(pDC, x1, y1, x2, y2+1, m_rgbMinuteHand);
	}
	else
	{
		x1 = (short)(MINUTE_HAND_LENGTH * cosine + (float)m_cdcClockFace.m_sizeBitmap.cx/2);
		y1 = (short)(-MINUTE_HAND_LENGTH * sine + (float)m_cdcClockFace.m_sizeBitmap.cy/2);
		DrawWuLine(pDC, x1, y1, x2, y2, m_rgbMinuteHand);
		x1 = (short)((MINUTE_HAND_LENGTH-4) * cosine + (float)m_cdcClockFace.m_sizeBitmap.cx/2);
		y1 = (short)((-MINUTE_HAND_LENGTH+4) * sine + (float)m_cdcClockFace.m_sizeBitmap.cy/2);
		DrawWuLine(pDC, x1, y1, x2-1, y2, m_rgbMinuteHand);
		DrawWuLine(pDC, x1, y1, x2-0, y2, m_rgbMinuteHand);
		DrawWuLine(pDC, x1, y1, x2+0, y2, m_rgbMinuteHand);
		DrawWuLine(pDC, x1, y1, x2+1, y2, m_rgbMinuteHand);
	}

	///////////////////////////////////////////////////////////////////////////
	// draw hour hand
	int hour = GetHour();
	if (hour > 12)
		hour -= 12;
	hour = hour*5 + GetMinute()/12;
	degrees = ConvertToDegrees((float)hour);
	radians = (float) ((PI * degrees)/180.);
	cosine = (float)cos(radians);
	sine = (float)sin(radians);

	x1 = (short)(HOUR_HAND_LENGTH * cosine + (float)m_cdcClockFace.m_sizeBitmap.cx/2);
	y1 = (short)(-HOUR_HAND_LENGTH * sine + (float)m_cdcClockFace.m_sizeBitmap.cy/2);
	//x2 = (short)(5.0 * cosine + (float)m_cdcClockFace.m_sizeBitmap.cx/2);
	//y2 = (short)(-5.0 * sine + (float)m_cdcClockFace.m_sizeBitmap.cy/2);

	x2 = (short)(1.0 * cosine + (float)m_cdcClockFace.m_sizeBitmap.cx/2);
	y2 = (short)(-1.0 * sine + (float)m_cdcClockFace.m_sizeBitmap.cy/2);

	if (((hour >= 10) && (hour <= 20)) || ((hour >= 40) && (hour <= 50)))
	{
		DrawWuLine(pDC, x1, y1, x2, y2, m_rgbHourHand);
		x1 = (short)((HOUR_HAND_LENGTH-4) * cosine + (float)m_cdcClockFace.m_sizeBitmap.cx/2);
		y1 = (short)((-HOUR_HAND_LENGTH+4) * sine + (float)m_cdcClockFace.m_sizeBitmap.cy/2);
		DrawWuLine(pDC, x1, y1, x2, y2-1, m_rgbHourHand);
		DrawWuLine(pDC, x1, y1, x2, y2-0, m_rgbHourHand);
		DrawWuLine(pDC, x1, y1, x2, y2+0, m_rgbHourHand);
		DrawWuLine(pDC, x1, y1, x2, y2+1, m_rgbHourHand);
	}
	else
	{
		DrawWuLine(pDC, x1, y1, x2, y2, m_rgbHourHand);
		x1 = (short)((HOUR_HAND_LENGTH-4) * cosine + (float)m_cdcClockFace.m_sizeBitmap.cx/2);
		y1 = (short)((-HOUR_HAND_LENGTH+4) * sine + (float)m_cdcClockFace.m_sizeBitmap.cy/2);
		DrawWuLine(pDC, x1, y1, x2-1, y2, m_rgbHourHand);
		DrawWuLine(pDC, x1, y1, x2-0, y2, m_rgbHourHand);
		DrawWuLine(pDC, x1, y1, x2+0, y2, m_rgbHourHand);
		DrawWuLine(pDC, x1, y1, x2+1, y2, m_rgbHourHand);
	}
}

/**********************************************************************************************//**
 * Paints the second hand described by pDC
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	pDC	If non-null, the device-context.
 **************************************************************************************************/

void CXAnalogClock::PaintSecondHand(CDC *pDC)
{
	if (m_bShowSecondHand)
	{
		// draw second hand
		float degrees, radians, cosine, sine;
		short x1, y1, x2, y2;

		degrees = ConvertToDegrees((float)GetSecond());
		radians = (float)((PI * degrees)/180.);
		cosine = (float)cos(radians);
		sine = (float)sin(radians);

		x1 = (short)(SECOND_HAND_LENGTH * cosine + (float)m_cdcClockFace.m_sizeBitmap.cx/2);
		y1 = (short)(-SECOND_HAND_LENGTH * sine + (float)m_cdcClockFace.m_sizeBitmap.cy/2);
		//x2 = (short)(5.0 * cosine + (float)m_cdcClockFace.m_sizeBitmap.cx/2);
		//y2 = (short)(-5.0 * sine + (float)m_cdcClockFace.m_sizeBitmap.cy/2);

		x2 = (short)(1.0 * cosine + (float)m_cdcClockFace.m_sizeBitmap.cx/2);
		y2 = (short)(-1.0 * sine + (float)m_cdcClockFace.m_sizeBitmap.cy/2);

		DrawWuLine(pDC, x1, y1, x2, y2, m_rgbSecondHand);
		DrawWuLine(pDC, x1+1, y1, x2+1, y2, m_rgbSecondHand);
	}
}

/**********************************************************************************************//**
 * Executes the erase bkgnd action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	pDC	If non-null, the device-context.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

BOOL CXAnalogClock::OnEraseBkgnd(CDC* pDC)
{
	return CStatic::OnEraseBkgnd(pDC);
}

/**********************************************************************************************//**
 * Executes the timer action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	nIDEvent	The identifier event.
 **************************************************************************************************/

void CXAnalogClock::OnTimer(UINT nIDEvent)
{
	if (!m_bUseSystemTime)
	{
		/*CTimeSpan ts(0, 0, 0, 1);
		m_time = m_time + ts;*/

		COleDateTimeSpan ts(0, 0, 0, 1);
		m_dtTime = m_dtTime + ts;
	}
	if(m_iPrevDay!=GetDay())
	{
		m_iPrevDay=GetDay();
		if(GetParent())
			GetParent()->PostMessage(WM_DATE_CHANGED);
	}
	Invalidate(FALSE);
	CStatic::OnTimer(nIDEvent);
}

/**********************************************************************************************//**
 * Runs this instance
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CXAnalogClock::Run()
{
	m_nPrevMinute = -1;
	SetTimer(1, 1000, NULL);
	Invalidate();
	UpdateWindow();
	m_bStopped = FALSE;
}

/**********************************************************************************************//**
 * Stops this instance
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CXAnalogClock::Stop()
{
	KillTimer(1);
	m_bStopped = TRUE;
}

/**********************************************************************************************//**
 * Gets the second
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	The second.
 **************************************************************************************************/

int CXAnalogClock::GetSecond()
{
	//return m_time.GetSecond();
	return m_dtTime.GetSecond();
}

/**********************************************************************************************//**
 * Gets the minute
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	The minute.
 **************************************************************************************************/

int CXAnalogClock::GetMinute()
{
	//return m_time.GetMinute();
	return m_dtTime.GetMinute();
}

/**********************************************************************************************//**
 * Gets the hour
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	The hour.
 **************************************************************************************************/

int CXAnalogClock::GetHour()
{
	//int hour = m_time.GetHour();
	int hour = m_dtTime.GetHour();
	if (hour > 12)
		hour -= 12;
	return hour;
}

/**********************************************************************************************//**
 * Gets the day
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	The day.
 **************************************************************************************************/

int CXAnalogClock::GetDay()
{
	//int day = m_time.GetDay();
	int day = m_dtTime.GetDay();
	return day;
}

/**********************************************************************************************//**
 * Translates all messages before they are processed by the main message loop
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	pMsg	If non-null, the message.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

BOOL CXAnalogClock::PreTranslateMessage(MSG* pMsg)
{
	return CStatic::PreTranslateMessage(pMsg);
}

/**********************************************************************************************//**
 * Executes the destroy action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CXAnalogClock::OnDestroy() 
{
	Stop();
	CStatic::OnDestroy();
}
