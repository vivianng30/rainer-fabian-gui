#pragma once
#include "DTUpDwnBtn.h"
#include "SelectSetupBtn.h"
#include "globDefs.h"
#include "colour.h"
#include "MVModel.h"

// CSubViewDateTime

class CSubViewDateTime : public CWnd
{
	DECLARE_DYNAMIC(CSubViewDateTime)

	/*struct TIME_ZONE_INFO
	{
		ULONG Bias;
		ULONG StandardBias;
		ULONG DaylightBias;
		SYSTEMTIME StandardDate;
		SYSTEMTIME DaylightDate;
	};*/


public:
	CSubViewDateTime();
	virtual ~CSubViewDateTime();

	bool CreateWnd(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);
	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	

	//bool DrawData(bool bFrames, bool bText);
	void SetOneButtonDepressed(int btnID);
	void SetAllButtonUnpressed();
	void SetOneButtonFocused(int btnID);
	void SetNextButtonFocused();
	void SetPrevButtonFocused();
	eBtnState GetCurrentBtnState();
	eBtnState GetBtnState(int btnID);

	void Show(bool bShow);

	//void Test();
	//void TimeZoneInfoToTimeZoneInformation( TIME_ZONE_INFORMATION& TimeZoneInfo1 , const TIME_ZONE_INFO& TimeZoneInfo2 );

protected:
	void Draw();
	CStringW GetMonthString(int iMonth);

	void DateTimeChanged();
	CMVModel *getModel();

		
private:
	CMVModel* m_pModel;

	DWORD m_dwLastSetupTimer;
	// Attributes
	HDC			m_hDC;
	HBITMAP		m_hBmp;
	HBITMAP		m_hBmpPrev;
	LONG		m_lX;
	LONG		m_lY;
	LONG		m_lXo;
	LONG		m_lYo;

	//CDialog*   m_pDlg;

	int m_iYear;
	int m_iMonth;
	int m_iDay;
	int m_iHour;
	int m_iMinute;

	COleDateTime m_dtDateTime;

	CBmp* m_pcRegionTopLeCorner;
	CBmp* m_pcRegionTopRiCorner;
	CBmp* m_pcRegionBotLeCorner;
	CBmp* m_pcRegionBotRiCorner;

	CBmp* m_pcSmallUp_Up;
	CBmp* m_pcSmallUp_Dw;
	CBmp* m_pcSmallDw_Up;
	CBmp* m_pcSmallDw_Dw;

	CBmp* m_pcOkGreen_Up;
	CBmp* m_pcOkGreen_Dw;
	CBmp* m_pcOkRed_Up;
	CBmp* m_pcOkRed_Dw;

	CBtn* m_pbtnOk;

	CBmp* m_pcStaticMonth_Up;
	CBmp* m_pcStaticMonth_Dw;
	CBmp* m_pcStaticMonth_Fc;
	CBmp* m_pcStaticYear_Up;
	CBmp* m_pcStaticYear_Dw;
	CBmp* m_pcStaticYear_Fc;
	CBmp* m_pcStaticTime_Up;
	CBmp* m_pcStaticTime_Dw;
	CBmp* m_pcStaticTime_Fc;

	//CBmp* m_pcTest;

	CSelectSetupBtn* m_pbtnDay;
	CSelectSetupBtn* m_pbtnMonth;
	CSelectSetupBtn* m_pbtnYear;
	CSelectSetupBtn* m_pbtnHour;
	CSelectSetupBtn* m_pbtnMinute;

	CDTUpDwnBtn* m_pcDayNextUp;
	CDTUpDwnBtn* m_pcDayNextDwn;

	CDTUpDwnBtn* m_pcMonthNextUp;
	CDTUpDwnBtn* m_pcMonthNextDwn;

	CDTUpDwnBtn* m_pcYearNextUp;
	CDTUpDwnBtn* m_pcYearNextDwn;

	CDTUpDwnBtn* m_pcHourNextUp;
	CDTUpDwnBtn* m_pcHourNextDwn;

	CDTUpDwnBtn* m_pcMinNextUp;
	CDTUpDwnBtn* m_pcMinNextDwn;

	CList<CSelectSetupBtn* , CSelectSetupBtn *> m_plStaticBtn;
	
	int m_iCounter;
	bool m_bBigSteps;
	eTimeChanger m_eTimeChanger;

	int m_iCurrentField;

	bool m_bDateTimeChanged;

	
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedSetDateTime();
	afx_msg void OnBnClickedDay();
	afx_msg void OnBnClickedMonth();
	afx_msg void OnBnClickedYear();
	afx_msg void OnBnClickedMinute();
	afx_msg void OnBnClickedHour();
};

