#pragma once


#pragma once
#include "MVView.h"
#include "PushBtn.h"
//#include "WndAlarmLog.h"
//#include "NumBtn.h"


class CViewSystemAlarm :public CMVView
{
public:
	CViewSystemAlarm(int ViewID = 0);
	virtual ~CViewSystemAlarm(void);

	virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);

	virtual bool CreateView();
	virtual bool Initialize();
	virtual void Open();
	virtual void Close();

	virtual void Show();
	virtual void Hide();

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	//Observer
	virtual void NotifyEvent(CMVEvent* pEvent);

	/*virtual void SetViewFocus(){SetFocus();}
	virtual bool SetNextFocus(){return false;}
	virtual bool SetPrevFocus(){return false;}*/

protected:

	void DrawAlarms();
	void RefreshAlarmStates();
	void DrawPic(HDC hdc);
	void DrawCheckpoints(HDC hdc);

	void DrawAlarmClockActive(HDC hdc, int nX, int nY);
	void DrawAlarmClockInactive(HDC hdc, int nX, int nY);

	/*bool CreateWndAlarmLog();
	bool DestroyWndAlarmLog();
	void ShowWndAlarmLog(bool bShow);*/

protected:

	//CWndAlarmLog *m_pWndAlarmLog;

	CBmp* m_pcAVCal;
	CBmp* m_pcAVCalAlarm;
	CBmp* m_pcAVCalSignal;
	CBmp* m_pcSysAlarm;
	CBmp* m_pcMixerAlarm;
	CBmp* m_pcMixerSignal;
	//CBmp* m_pcUseError;
	CBmp* m_pcAClockGrey;
	CBmp* m_pcAClockRed;
	CBmp* m_pcNumeric_Up;
	CBmp* m_pcNumeric_Dw;

	//bool m_bLogActive;

	CPushBtn* m_pcMenuNum;
	CPushBtn* m_pcMenuSens;

	CBmp* m_pcMenu_Up;
	CBmp* m_pcMenu_Dw;
	CBmp* m_pcMenu_Dis;

	bool m_bDrawO2SensorBtn;


public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};





