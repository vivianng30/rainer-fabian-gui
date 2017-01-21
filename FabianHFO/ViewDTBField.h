#pragma once
#include "MVView.h"
#include "Bmp.h"
#include "XAnalogClock.h"

class CViewDTBField :public CMVView
{
public:
	CViewDTBField(int ViewID = 0);
	virtual ~CViewDTBField(void);

	virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);

	virtual bool CreateView();
	virtual bool Initialize();
	virtual void Open();
	virtual void Close();

	virtual void Show();
	virtual void Hide();;
	/*virtual void SetViewFocus(){SetFocus();}
	virtual bool SetNextFocus(){return false;}
	virtual bool SetPrevFocus(){return false;}*/

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	//Observer
	virtual void NotifyEvent(CMVEvent* pEvent);
	virtual void NotifyLanguageChanged();
	virtual void NotifyViewStateChanged();
	virtual void NotifyVentModeChanged();
	virtual void NotifyParaBtnEvent(CMVEvent* pEvent);

	//void DrawInfoText(CStringW cs,bool bResetTxtTimer=false);

	void SetTimeO2Flush(CStringW szTime);
	void StopO2Flush();

	void SetTimeUntilStartVent(int iCountDown);
	void StopTimeUntilStartVent();

	void SetTimeAlarmSilent(int iCountDown);
	void StopAlarmSilent();

	void SetTimeUntilStopVent(int iCountDown);
	void StopTimeUntilStopVent();

	void SetTimeUntilTurnOff(int iCountDown);
	void StopTimeUntilTurnOff();

	//void SetOxyCalRunning(bool state);

	void setPSVapnoe(bool bState);
	void SetCPAPBackup(bool bState);

	void setMaintenanceFlag(bool bState);

	void setPRICOrunning(bool state);
protected:

	void drawPRICOradar(CDC* pDC);

	void DrawLine(bool bDrawStateLine,bool bDrawInfoLine,bool bDrawNumericLine);
	void DrawNumericLine(HDC hdc);
	void DrawInfoLine(HDC hdc);
	/*void DrawInfoLineBackgnd(HDC hdc);
	void DrawInfoLineText(HDC hdc);*/
	void DrawStateLine(HDC hdc);
	//void DrawTimeline(HDC hdc);

	bool CheckAkkuState();
	bool CheckDateChanged();

	

	CStringW GetActiveAlarmText();
	
protected:
	CRITICAL_SECTION csInfo;

	HDC		m_hdcStatusLine;
	HDC		m_hdcInfoLine;
	HDC		m_hdcNumericLine;
	HBITMAP m_hbmpStatusLine;
	HBITMAP m_hbmpInfoLine;
	HBITMAP m_hbmpNumericLine;

	//PRICO radar
	CBmp* m_pcPRICO;
	CBmp* m_pcPRICO1;
	CBmp* m_pcPRICO2;
	CBmp* m_pcPRICO3;
	CBmp* m_pcPRICO4;
	CBmp* m_pcPRICO5;
	CBmp* m_pcPRICO6;
	CBmp* m_pcPRICO7;
	CBmp* m_pcPRICO8;
	CBmp* m_pcPRICO9;
	CBmp* m_pcPRICO10;
	CBmp* m_pcPRICO11;
	CBmp* m_pcPRICO12;
	CBmp* m_pcPRICO13;
	CBmp* m_pcPRICO14;
	CBmp* m_pcPRICO15;
	CBmp* m_pcPRICO16;
	CBmp* m_pcPRICO17;
	CBmp* m_pcPRICO18;
	CBmp* m_pcPRICO19;
	CBmp* m_pcPRICO20;
	CBmp* m_pcPRICO21;
	CBmp* m_pcPRICO22;
	CBmp* m_pcPRICO23;
	CBmp* m_pcPRICO24;

	//State Line########################################
	CBmp* m_pcACCU;
	CBmp* m_pcACCU_0;
	CBmp* m_pcACCU_20;
	CBmp* m_pcACCU_40;
	CBmp* m_pcACCU_60;
	CBmp* m_pcACCU_80;
	CBmp* m_pcACCU_100;
	CBmp* m_pcACCULOAD_20;
	CBmp* m_pcACCULOAD_40;
	CBmp* m_pcACCULOAD_60;
	CBmp* m_pcACCULOAD_80;
	CBmp* m_pcACCULOAD_100;
	CBmp* m_pcNoManATZ;
	CBmp* m_pcPatientData;
	CBmp* m_pcNurse;
	CBmp* m_pcNurseActive;
	CBmp* m_pcPediatric;
	CBmp* m_pcNeonatal;
	CBmp* m_pcBatState;
	CBmp* m_pcGreyNumHeader;
	CBmp* m_pcYellowNumHeader;
	//CBmp* m_pcNebulizer;
	CBmp* m_pcMaintenance;

	bool m_bAccuPower;
	bool m_bNetPower;
	bool m_bExtrenalPower;

	bool m_bPatData;
	bool m_bDrawRemainingTime;
	bool m_bNurscallAlarm;
	bool m_bNurscallActive;
	bool m_bDrawNursecallRed;
	bool m_bACCUfull;
	bool m_bACCUcharging;
	UINT m_iCntAccuCharge;
	int m_iBattStatMinutes;
	int m_iBattStatPercents;

	//Info Line################################################
	CBmp* m_pcScreenlockInfoline;
	CBmp* m_pcGreyInfoline;
	CBmp* m_pcRedInfoline;
	CBmp* m_pcOrangeInfoline;
	CBmp* m_pcYellowInfoline;
	CBmp* m_pcStartHFO_red;
	CBmp* m_pcStartHFO_grey;
	CBmp* m_pcStartHFO_yellow;
	CBmp* m_pcStartHFO_orange;
	CBmp* m_pcWarningRed;
	CBmp* m_pcWarningGrey;

	CStringW m_szInfolineText;//m_szHeaderText;
	CStringW m_szInfoText;
	CStringW m_szCounterText;//m_szCounter;
	bool m_bDrawInfoText;

	bool m_bShowPresureVal;
	int m_iAir;
	int m_iOxy;
	int m_iPPROX_Data;
	int m_iP_MIX_Data;

	CStringW m_szOxyFlushTime;//m_szO2FlushTime;
	bool m_bCountDownOxyFlush;//m_bCountDownO2Flush

	int m_iCounterStartVent;//m_iCounterStartTime;
	bool m_bCountDownStartVent;//m_bCountDownStartvent

	int m_iCounterStopVent;//m_iCounterUntilStopvent
	bool m_bCountDownStopVent;//m_bCountDownStopvent
	
	int m_iCounterTurnOff;//m_iCounterUntilTurnoff
	bool m_bCountDownTurnOff;//m_bCountDownTurnoff

	bool m_bActiveHighAlarm;
	bool m_bActiveMediumAlarm;
	bool m_bActiveLowAlarm;
	/*bool m_bSignalTubusDisconAlarm;
	bool m_bSignalPatientAlarm;*/
	bool m_bSignalAlarm;
	CString m_szSigaledAlarmText;

	/*bool m_bSignalHighAlarm;
	bool m_bSignalMediumAlarm;
	bool m_bSignalLowAlarm;*/
	bool m_bNebulizerRunning;
	bool m_bDrawNebulizer;
	bool m_bManualBreathRunning;
	bool m_bExhalValvCalRunning;
	bool m_bSTANDBYMode;
	bool m_bOxyCalRunning;
	bool m_bScreenlockActive;
	bool m_bPSVapnoe;
	bool m_bCPAPBackup;
	bool m_bPSVapnoeTimer;
	//bool m_bHFOstopped;
	bool m_bAlarmLogTimer;
	bool m_bFlowCalNeeded;

	bool m_bDrawRedInfoline;
	bool m_bDrawOrangeInfoline;
	bool m_bDrawYellowInfoline;
	
	bool m_bMaintenanceNeeded;

	/*bool m_bDrawRedLine;
	bool m_bDrawOrangeInfoline;
	bool m_bPSVapnoeTimer;
	bool m_bChangePSVapnoeColor;*/
	
	

	//Numeric Line####################################################################
	CXAnalogClock m_Clock;
	CBmp* m_pcAlarmOffGrey;
	CBmp* m_pcAlarmOffYellow;
	CBmp* m_pcAlarmSilentGrey;
	CBmp* m_pcAlarmSilentYellow;


	CBmp* m_pcAlarmSignaled;
	//CBmp* m_pcAlarmLog;

	int m_iCounterASilent;
	bool m_bCountDownASilent;
	bool m_bDrawYellowNumHeader;
	bool m_bSystemASilent;
	bool m_bServiceASilent;
	//bool m_bDrawAlarmLogNumHeader;

	int m_iPrevDay;

	BYTE m_iPRICOCount;
	bool m_bPRICOrunning;
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
};