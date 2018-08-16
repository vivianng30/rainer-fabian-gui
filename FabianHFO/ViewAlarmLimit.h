/**********************************************************************************************//**
 * \file	ViewAlarmLimit.h.
 *
 * Declares the view alarm limit class
 **************************************************************************************************/

#pragma once
#include "MVView.h"
#include "MnuWrapperAlarmLimits.h"
#include "ALimitBtn.h"
#include "SelectSetupBtn.h"



class CViewAlarmLimit :public CMVView
{
public:
	CViewAlarmLimit(int ViewID = 0);
	virtual ~CViewAlarmLimit(void);

	virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);

	virtual bool CreateView();
	virtual bool Initialize();
	virtual void Open();
	virtual void Close();

	virtual void Show();
	virtual void Hide();


	virtual void SetViewFocus();
	virtual bool SetNextFocus();
	virtual bool SetPrevFocus();

	//virtual void Trigger(int btnID=0,int iGroupId=0);
	//virtual void SetFocusTo(int iID);

	//virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//Observer
	virtual void NotifyEvent(CMVEvent* pEvent);
	virtual void NotifyDataChanged();
	virtual void NotifyExspirationDataChanged();
	virtual void NotifyAlarmLimitChanged();
	virtual void NotifyCalculateAlarmLimit();
	virtual void NotifyVentModeChanged();
	virtual void NotifyParaBtnEvent(CMVEvent* pEvent);
	//virtual void NotifyViewFocusChanged(int iViewID);

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	

protected:
	bool CreateWndMenuAlarmLimits();
	void ShowWndMenuAlarmLimits(bool bShow);

	void checkAlarmState();

	//void DrawData(CDC* pDC, bool bFrames, bool bLabel, bool bValue);
	void DrawData(bool bFrames, bool bLabel);
	
	void DrawLabel(CDC* pDC);
	void drawLabel_ETCO2(CDC* pDC);
	void drawLabel_SPO2(CDC* pDC);
	void drawLabel_VENTILATION(CDC* pDC);
	void drawLabel_HFO(CDC* pDC);
	void drawLabel_CPAP(CDC* pDC);
	void drawLabel_NCPAP(CDC* pDC);
	void drawLabel_DUOPAP(CDC* pDC);
	void drawLabel_CONVENTIONAL(CDC* pDC);


	
	void DrawFrames(CDC* pDC);
	void drawFrameETCO2(CDC* pDC);
	void drawFrameSPO2(CDC* pDC);
	void drawFrameTHERAPIE(CDC* pDC);
	void drawFrameHFO(CDC* pDC);
	void drawFrameCPAP(CDC* pDC);
	void drawFrameNCPAP(CDC* pDC);
	void drawFrameDUOPAP(CDC* pDC);
	void drawFrameCONVENTIONAL(CDC* pDC);

	void DrawData_etCO2(HDC hdc);
	void DrawData_SpO2(HDC hdc);
	void DrawData_Vent(HDC hdc);
	void drawDataVentilation_HFO(HDC hdc);
	void drawDataVentilation_CPAP(HDC hdc);
	void drawDataVentilation_NCPAP(HDC hdc);
	void drawDataVentilation_DUOPAP(HDC hdc);
	void drawDataVentilation_CONVENTIONAL(HDC hdc);

	void RedrawAlarmLimits();
	
	void CalculateAlarmLimit();

	void DrawAutoState();

	bool IsButtonDepressed();

	bool isActicePatientAlarmLimit();
	void RefreshAlarmStates();

	void RedrawView();

	void showALimitButtons();
	void showALimitButtons_IPPV();
	void showALimitButtons_SIPPV();
	void showALimitButtons_SIMV();
	void showALimitButtons_SIMVPSV();
	void showALimitButtons_PSV();
	void showALimitButtons_CPAP();
	void showALimitButtons_NCPAP();
	void showALimitButtons_DUOPAP();
	void showALimitButtons_HFO();
	void showALimitButtons_THERAPIE();

protected:
	HDC		m_hdcStatic;
	HBITMAP m_hbmpStatic;

	eVentMode m_eCurVentMode;

	eALimitNumeric m_eCurALimitNumeric;

	CBmp* m_pcNumInfoTop;
	CBmp* m_pcNumInfoBot;

	CBmp* m_pcAlarmlimitPara1;
	CBmp* m_pcAlarmlimitPara2;

	CBmp* m_pcBtnAlarm_Up;
	CBmp* m_pcBtnAlarm_Dw;
	CBmp* m_pcBtnAlarm_Fc;
	
	CBmp* m_pcStatic_Up;
	CBmp* m_pcStatic_Dw;
	CBmp* m_pcStatic_Fc;

	CALimitBtn* m_pcAlarmLimit_MVhi;
	CALimitBtn* m_pcAlarmLimit_MVlow;
	CALimitBtn* m_pcAlarmLimit_PIPmax;
	CALimitBtn* m_pcAlarmLimit_PIPmin;
	CALimitBtn* m_pcAlarmLimit_PEEP;
	CALimitBtn* m_pcAlarmLimit_BPM;
	CALimitBtn* m_pcAlarmLimit_Leak;
	CALimitBtn* m_pcAlarmLimit_Apnoe;
	CALimitBtn* m_pcAlarmLimit_MAPmax;
	CALimitBtn* m_pcAlarmLimit_MAPmin;
	CALimitBtn* m_pcAlarmLimit_DCO2hi;
	CALimitBtn* m_pcAlarmLimit_DCO2lo;
	CALimitBtn* m_pcAlarmLimit_ETCO2hi;
	CALimitBtn* m_pcAlarmLimit_ETCO2lo;

	CALimitBtn* m_pcAlarmLimit_FICO2hi;
	CALimitBtn* m_pcAlarmLimit_FICO2lo;

	CALimitBtn* m_pcAlarmLimit_SPO2hi;
	CALimitBtn* m_pcAlarmLimit_SPO2lo;
	CALimitBtn* m_pcAlarmLimit_PulseRatehi;
	CALimitBtn* m_pcAlarmLimit_PulseRatelo;
	CALimitBtn* m_pcAlarmLimit_SPO2_PIlo;
	CALimitBtn* m_pcAlarmLimit_SPO2_SIQ;

	CSelectSetupBtn* m_pbtnValueDelay;

	CMnuWrapperAlarmLimits *m_pWndMenuAlarmLimits;

	int m_iValueDelay;
	int m_iFocusedWnd;
	bool m_bMenuPressed;

	bool m_bAlarmSet;

	//CList<CALimitBtn* , CALimitBtn *> m_plBtn;

	int m_iCurPara;

	bool m_bExit;

	//bool m_bSignalAlarm;

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedValue();
};
