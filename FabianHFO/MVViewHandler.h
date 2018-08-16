//=============================================================================
/** 
* \file MVViewHandler.h
* \class MVViewHandler
* \brief View-Controller of MVC (Model-View-Controller software architecture).
* \author Rainer Kuehner
* \date 2013-01-21 Creation of file
*
* The view handler controls the states of the graphical user interface depending
* on ventilation mode or alarm state.
*
*       Copyright (c) 2013 Ing.Buero Rainer Kuehner
*                      Illerkirchberg
*
**/
//=============================================================================
#pragma once
#include "afxwin.h"
#include "MVModel.h"
#include "MVView.h"

#include "WndWrapper.h"
#include "WndWrapperDTBField.h"
#include "WndWrapperParaBtn.h"
#include "WndWrapperPatData.h"
#include "WndWrapperStartup.h"
#include "WndWrapperDiagramms.h"
#include "WndWrapperNumerics.h"
#include "WndWrapperService.h"
#include "WndWrapperAlarmLimits.h"
#include "WndWrapperSystemAlarm.h"
#include "WndWrapperTrends.h"
#include "WndWrapperAlarmLog.h"
#include "WndWrapperFullscreenMsg.h"
#include "WndWrapperMenu.h"
#include "WndWrapperShutdown.h"



class CMVViewHandler
{
	//singleton
	static CMVViewHandler* theViewHandler;

	CMVViewHandler();
	~CMVViewHandler(void);

public:
	static CMVViewHandler* GetInstance();
	static void DestroyInstance();

	void Init();

	void RedrawCurViews();

	bool OpenPatDataView();
	bool ClosePatDataView();
	bool OpenStartupView();
	bool CloseStartupView();
	bool OpenLogView();
	bool CloseLogView();
	bool OpenTrendView(bool bRedraw=false);
	bool CloseTrendView();
	bool OpenSystemAlarmView();
	bool CloseSystemAlarmView();
	bool OpenMenuView(bool bRedraw=false);
	bool CloseMenuView();
	bool OpenServiceView(bool bRedraw=false);
	bool CloseServiceView();
	bool OpenNumericView();
	bool CloseNumericView();
	bool OpenParaBtnView();
	bool CloseParaBtnView();
	bool OpenDiagrammView();
	bool CloseDiagrammView();
	bool OpenDTBFieldView();
	bool CloseDTBFieldView();
	bool OpenAlarmLimitsView();
	bool CloseAlarmLimitsView();
	bool OpenFullscreenMsgView();
	bool CloseFullscreenMsgView();
	bool OpenShutdownView();
	bool CloseShutdownView();

	void UpdateServiceView();

	void BtnPushed(int btnID, int iGroupId);
	void SetFocusToPrimaryView();

	void SetViewVKUP();
	void SetViewVKDOWN();

	void SetNextFocus();
	void SetPrevFocus();

	void SetFocusToNextView();
	void SetFocusToPrevView();

	void CheckCurrentView(int iViewID);

	void SetALIMITTIMER();

	void UpdateLimitData();
	
	void drawFOTsteps();

	void drawMeasuredFiO2Value();
	void setSIQdata();
	void setPRICOrunning(bool state);
	void SetTriggeredBreath();
	void setPSVapnoe(bool bState);
	void SetCPAPBackup(bool bState);

	void setMaintenanceFlag(bool bState);

	void UpdateCO2InfoData(bool resetAvailable);

	void SetCurrentFocusedView(CWndWrapper *pView);

	void setPpsvValue(int iValPPSV,bool bSend);
	void setPEEPvalue(int iValPEEP,bool bSend);

	void setPmeanDifference(int iValPmean,bool bSend);
	void setPmeanRecDifference(int iValPmeanRec,bool bSend);

	void refreshO2Flush();
	void DrawO2FlushTime(int iO2FlushTime);
	void O2FlushChanged();

	void redrawGraph();
	void checkFOTalarms();
	void redrawFOTmenu();
	void drawFOTtime(BYTE iCountFOTimer);
	void setFOTrunning(bool state);
	void updateFOTPmeanPara();
	void updateFOTPEEPPara();

	void redrawINFO();

	void HFPMEANRECFlushChanged(int iVal);

	void paraDataChanged();

	void ITIMEChanged();
	void ETIMEChanged();
	void BPMChanged();

	void SetOxyHourglass(bool state);

	void StopDiagramm(bool bStopFreeze=false);

	void DrawCountDownStartTime(int iCountDownStart);
	void StopCountDownStartTime();

	void DrawCountDownAlarmSilent(int iCountDown);
	void StopCountDownAlarmSilent();

	void DrawCountTimeUntilStop(int iCountDown);
	void StopCountTimeUntilStop();

	void DrawCountTimeUntilOff(int iCountDown);
	void StopCountTimeUntilOff();

	void StopVideo();

	//Observer
	void viewStateChanged();

	void changeViewState(eViewState state,eViewSubState substate,bool bStopFreeze=false);
	void changeToPrevViewState();
	bool canChangeViewStateAlarmDependend();

	eViewState getViewState();
	void setViewState(eViewState state,eViewSubState substate);
	eViewSubState getViewSubState();

	eViewState getPrevViewState();
	void setPrevViewState(eViewState state,eViewSubState substate);
	eViewSubState getPrevViewSubState();

	eViewState getPrevGraphState();
	void setPrevGraphState();
	eViewSubState getPrevGraphSubState();

	void DrawViews(bool bRedraw);
	
protected:
	
	CMVModel *getModel();

	CWndWrapper* GetPrimaryActiveView();
	void SetPrimaryActiveView(CWndWrapper *pWrapper);

	CWndWrapper* GetCurrentFocusedView();
	
	void drawView_SHUTDOWN();
	void drawView_SYSFAIL();
	void drawView_BATTERY();
	void drawView_PATDATA();
	void drawView_TREND(bool bRedraw);
	void drawView_PARA(bool bRedraw);
	void drawView_GRAPH();
	void drawView_SERVICE(bool bRedraw);
	void drawView_ALARM_SYS();
	void drawView_SETUP(bool bRedraw);
	void drawView_ALARM_LIMIT(bool bRedraw);

protected:
	CRITICAL_SECTION	csViewState;
	CRITICAL_SECTION	csViewDiagramm;
	CRITICAL_SECTION	csViewAlarmLimits;
	CRITICAL_SECTION	csViewDTBField;
	CRITICAL_SECTION	csViewParaBtn;
	CRITICAL_SECTION	csViewTrend;
	CRITICAL_SECTION	csViewPatData;
	CRITICAL_SECTION	csViewFullscreenMsg;
	CRITICAL_SECTION	csViewSystemAlarm;
	CRITICAL_SECTION	csViewMenu;
	CRITICAL_SECTION	csViewService;
	CRITICAL_SECTION	csViewLog;
	CRITICAL_SECTION	csViewNumeric;

	CMVModel* m_pModel;

	CWndWrapper *m_vPrimaryActiveView;
	CWndWrapper *m_vCurrentFocusedView;

	CWndWrapperDTBField *m_vwDTBField;
	CWndWrapperParaBtn *m_vwParaBtn;
	CWndWrapperPatData *m_vwPatData;
	CWndWrapperStartup *m_vwStartup;
	CWndWrapperDiagramms *m_vwDiagramm;
	CWndWrapperNumerics *m_vwNumericWnd;
	CWndWrapperService *m_vwService;
	CWndWrapperAlarmLimits *m_vwAlarmLimits;
	CWndWrapperSystemAlarm* m_vwSystemAlarm;
	CWndWrapperTrends *m_vwTrend;
	CWndWrapperAlarmLog *m_vwLog;
	CWndWrapperFullscreenMsg *m_vwFullscreenMsg;
	CWndWrapperMenu *m_vwMenu;
	CWndWrapperShutdown *m_vwShutdown;

	eViewState m_eViewState;
	eViewSubState m_eViewSubState;

	eViewState m_ePrevViewState;
	eViewSubState m_ePrevViewSubState;

	eViewState m_ePrevGraphState;
	eViewSubState m_ePrevGraphSubState;

	DWORD m_dwLastTrendMoveLeft;
	DWORD m_dwLastTrendCntLeft;
	DWORD m_dwLastTrendMoveRight;
	DWORD m_dwLastTrendCntRight;

	UINT m_iTrendLeft;
	UINT m_iTrendRight;
	
};
