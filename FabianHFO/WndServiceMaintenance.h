//pro, created
#pragma once
#include "WndService.h"
#include "KbrdBtn.h"
#include "PushBtn.h"
#include "BitmapSlider.h"

#include "DTUpDwnBtn.h"
#include "SelectSetupBtn.h"
#include "globDefs.h"
#include "colour.h"

// CWndServiceMaintenance

class CWndServiceMaintenance : public CWndService
{
	DECLARE_DYNAMIC(CWndServiceMaintenance)

public:
	CWndServiceMaintenance();
	virtual ~CWndServiceMaintenance();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	void Init();
	void SetOneButtonDepressed(int btnID);
	void SetAllButtonUnpressed();
	void SetOneButtonFocused(int btnID);
	void SetNextButtonFocused();
	void SetPrevButtonFocused();
	eBtnState GetCurrentBtnState();
	eBtnState GetBtnState(int btnID);	

protected:
	void Draw();
	void DateTimeChanged();
	CStringW GetMonthString(int iMonth);	

private:
	CDialog*   m_pDlg;

	CBmp* m_pcAlarm_Up;
	CBmp* m_pcAlarm_Dw;

	CKbrdBtn* m_pcResetOpTime;
	CKbrdBtn* m_pcResetAccuOpTime;
	CKbrdBtn* m_pcResetHFOOpTime;
	CKbrdBtn* m_pcInstaller;

	CBitmapSlider*	m_sliderPassword;
	bool m_bServicePswrdDisabled;

	CBitmapSlider*	m_sliderAlarm;
	bool m_bAlarmOn;

	CBitmapSlider*	m_sliderDate;
	bool m_bServiceDateOn;

	WORD m_iYear;
	WORD m_iMonth;
	WORD m_iDay;

	COleDateTime m_dtNextServiceDate;

	CBmp* m_pcSmallUp_Up;
	CBmp* m_pcSmallUp_Dw;
	CBmp* m_pcSmallDw_Up;
	CBmp* m_pcSmallDw_Dw;

	CBmp* m_pcOkDisabled;
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

	CSelectSetupBtn* m_pbtnDay;
	CSelectSetupBtn* m_pbtnMonth;
	CSelectSetupBtn* m_pbtnYear;

	CDTUpDwnBtn* m_pcDayNextUp;
	CDTUpDwnBtn* m_pcDayNextDwn;

	CDTUpDwnBtn* m_pcMonthNextUp;
	CDTUpDwnBtn* m_pcMonthNextDwn;

	CDTUpDwnBtn* m_pcYearNextUp;
	CDTUpDwnBtn* m_pcYearNextDwn;

	CList<CSelectSetupBtn* , CSelectSetupBtn *> m_plStaticBtn;

	int m_iCurrentField;
	int m_iCounter;

	bool m_bBigSteps;
	bool m_bDateValidChanged;
	bool m_bDateChanged;
	bool m_bSetupAvailable;

	DWORD m_dwLastSetupTimer;

	eTimeChanger m_eTimeChanger;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedInstaller();
	afx_msg void OnBnClickedResetOpTime();
	afx_msg void OnBnClickedResetOpTimeBattery();
	afx_msg void OnBnClickedResetOpTimeHFO();
	afx_msg void OnBnClickedSetDate();
	afx_msg void OnBnClickedDay();
	afx_msg void OnBnClickedMonth();
	afx_msg void OnBnClickedYear();
	afx_msg LRESULT OnMyMessage(WPARAM wParam, LPARAM lParam);
};