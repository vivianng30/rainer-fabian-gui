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
#include "ViewStartup.h"
#include "ViewDiagramm.h"
#include "ViewTrend.h"
#include "ViewParaBtn.h"
#include "ViewAlarmLimit.h"
#include "ViewNumericWnd.h"
#include "ViewSystemAlarm.h"
#include "ViewService.h"
#include "ViewDTBField.h"
#include "ViewLog.h"
#include "ViewPatData.h"
#include "ViewFullscreenMsg.h"
//#include "ViewSelectNum.h"
#include "ViewMenu.h"
#include "ViewShutdown.h"
//#include "ViewNebulizer.h"



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
	/*bool OpenSetupView();
	bool CloseSetupView();*/
	bool OpenMenuView(bool bRedraw=false);
	bool CloseMenuView();
	bool OpenServiceView(bool bRedraw=false);
	bool CloseServiceView();
	//bool OpenCalibrationView();
	//bool CloseCalibrationView();
	bool OpenNumericView();
	bool CloseNumericView();
	bool OpenParaBtnView();
	bool CloseParaBtnView();
	/*bool OpenGraphView();
	bool CloseGraphView();*/
	bool OpenDiagrammView();
	bool CloseDiagrammView();
	bool OpenDTBFieldView();
	bool CloseDTBFieldView();
	bool OpenAlarmLimitsView();
	bool CloseAlarmLimitsView();
	bool OpenFullscreenMsgView();
	bool CloseFullscreenMsgView();
	//bool OpenSelectNumericView();
	//bool CloseSelectNumericView();
	bool OpenShutdownView();
	bool CloseShutdownView();

	//bool OpenNebulizerView();
	//bool CloseNebulizerView();

	void UpdateServiceView();

	void BtnPushed(int btnID, int iGroupId);
	//void SetFocus(CMVView* view);
	void SetFocusToPrimaryView();

	void SetViewVKUP();
	void SetViewVKDOWN();

	void SetNextFocus();
	void SetPrevFocus();

	void SetFocusToNextView();
	void SetFocusToPrevView();

	/*void SetCurrActiveView(CMVView* view);
	CMVView* GetCurrActiveView();*/
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

	//void RedrawDiagrammData(int iDiagrammID);

	void UpdateCO2InfoData(bool resetAvailable);
	//void CO2PumpOn();

	void SetCurrentFocusedView(CMVView *pView);

	void setPpsvValue(int iValPPSV,bool bSend);
	void setPEEPvalue(int iValPEEP,bool bSend);

	//SHORT getPpsvDisplayedValue();
	//SHORT getPEEPdisplayedValue();

	//void setPmeanHFO(int iValPmean, bool bSendPmeanDif, bool bSendPmanual);//PMAN1
	void setPmeanDifference(int iValPmean,bool bSend);//PMAN1
	void setPmeanRecDifference(int iValPmeanRec,bool bSend);//PMAN1

	//void setOxyDiffParaBTN(int iValOxy);//PRICO03
	//void setOxyPPRICO(int iValOxy);//PRICO04

	//void setPMAXVG(SHORT iValPMAXVG);//newVG

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

	void lungRecStateChanged();
	void ITIMEChanged();
	void ETIMEChanged();
	void BPMChanged();

	//void RefreshParaView();

	//void ParaButtonTurnDown(WPARAM wParam, LPARAM lParam);

	//void SetOxyCalRunning(bool state);
	void SetOxyHourglass(bool state);

	//void SetFlowData(int valueInsp, int valueExsp);

	void StopDiagramm();

	void DrawCountDownStartTime(int iCountDownStart);
	void StopCountDownStartTime();

	void DrawCountDownAlarmSilent(int iCountDown);
	void StopCountDownAlarmSilent();

	void DrawCountTimeUntilStop(int iCountDown);
	void StopCountTimeUntilStop();

	void DrawCountTimeUntilOff(int iCountDown);
	void StopCountTimeUntilOff();

	//void CheckPatData();

	void StopVideo();

	/*void runNebulizer();
	void stopNebulizer();
	void startNebulizerDelay();
	void closeNebulizer();
	void setNebulizerCounter(UINT iCnt);
	void setNebulizerDelayCounter(UINT iCnt);
	void redrawNebulizer();
	void checkAlarmState();*/

	//Observer
	void viewStateChanged();
	//void VentModeChanged();
	//virtual void NotifyEvent(CMVEvent* pEvent);

	void changeViewState(eViewState state,eViewSubState substate);
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

	//void startTest();
	
protected:
	
	CMVModel *getModel();

	//BOOL InitViewWindows();
	/*BOOL CreateDTBView();
	BOOL CreateParaView();
	BOOL CreateCalibrationView();
	BOOL CreateSetupView();
	BOOL CreateAlarmLimitsView();
	BOOL CreateGraphView();
	BOOL CreateTrendView();
	BOOL CreateSysalarmView();
	BOOL CreateDisconnectionView();*/
	CMVView* GetPrimaryActiveView();
	void SetPrimaryActiveView(CMVView *pView);

	CMVView* GetCurrentFocusedView();
	
	
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

	CViewStartup *m_vStartup;
	CViewDTBField *m_vDTBField;
	CViewDiagramm *m_vDiagramm;
	CViewParaBtn *m_vParaBtn;
	CViewNumericWnd *m_vNumericWnd;
	CViewService *m_vService;
	CViewAlarmLimit *m_vAlarmLimits;
	CViewSystemAlarm* m_vSystemAlarm;
	CViewTrend *m_vTrend;
	CViewLog *m_vLog;
	CViewPatData *m_vPatData;
	CViewFullscreenMsg *m_vFullscreenMsg;
	//CViewSelectNum *m_vSelectNumeric;
	CViewMenu *m_vMenu;
	CViewShutdown *m_vShutdown;
	//CViewNebulizer *m_vNebulizer;

	CMVView *m_vPrimaryActiveView;
	CMVView *m_vCurrentFocusedView;

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
