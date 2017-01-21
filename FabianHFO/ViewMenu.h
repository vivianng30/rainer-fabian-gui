#pragma once
#include "MVView.h"
#include "WndMenuMenu.h"
#include "SubViewCalOxySensor.h"
#include "SubViewCalFlowSensor.h"
#include "SubViewDisplay.h"
#include "SubViewLanguage.h"
#include "SubViewSystem.h"
#include "SubViewDateTime.h"
#include "SubViewVentilation.h"
#include "SubViewPatient.h"
#include "SubViewService.h"
#include "SubViewTools.h"
#include "SubViewMenuSelect.h"
#include "SubViewCO2.h"
#include "SubViewVideo.h"
#include "SubViewNetwork.h"
#include "SubViewSPO2.h"
#include "WndSubSettingsScreenlocktime.h"

#include "WndSubSettingsLeakComp.h"
#include "WndSubSettingsSPO2Module.h"
#include "WndSubSettingsSPO2sensitivity.h"
#include "WndSubSettingsSPO2averagingtime.h"
#include "WndSubSettingsSPO2alarmdelay.h"
#include "WndSubSettingsPpsvAbsolute.h"
#include "WndSubSettingsVentRange.h"
#include "WndSubSettingsRatioItime.h"
#include "WndSubSettingsManbreathTime.h"
#include "WndSubSettingsVolTrigger.h"
#include "WndSubSettingsTubeset.h"
#include "WndSubSettingsAutoOxyCal.h"
#include "WndSubSettingsHFOManBreath.h"
#include "WndSubSettingsCO2Module.h"
#include "WndSubSettingsCO2Units.h"
#include "WndSubSettingsBaroPressure.h"
#include "WndSubSettingsPminDelay.h"
#include "WndSubSettingsPressureUnit.h"
#include "WndSubSettingsAutoOnPump.h"
#include "WndSubSettingsPatAlarmDelay.h"
#include "WndSubSettingsEFlow.h"

// CViewMenue

class CViewMenu : public CMVView
{
	

public:
	CViewMenu(int ViewID = 0);
	virtual ~CViewMenu();

	virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);

	virtual bool CreateView();
	virtual bool Initialize();
	virtual void Open();
	virtual void Close();

	virtual void Show(bool bRedraw);
	virtual void Hide();

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	virtual void SetViewFocus();
	virtual bool SetNextFocus(){return false;}
	virtual bool SetPrevFocus(){return false;}

	////Observer
	virtual void NotifyEvent(CMVEvent* pEvent);
	virtual void NotifyLanguageChanged();

	void StopVideo();

	//void RedrawView();

protected:
	bool CreateWndMenuMenu();
	bool DestroyWndMenuMenu();
	void ShowWndMenuMenu(bool bShow, CStringW txt, bool bMenuBtnOpen, bool bMenuBtnReturn);

	bool CreateSubViewFlowSensor();
	bool DestroySubViewFlowSensor();
	void ShowSubViewFlowSensor(bool show);

	bool CreateSubViewOxySensor();
	bool DestroySubViewOxySensor();
	void ShowSubViewOxySensor(bool show);

	bool CreateSubViewDisplay();
	bool DestroySubViewDisplay();
	void ShowSubViewDisplay(bool show);

	bool CreateSubViewLanguage();
	bool DestroySubViewLanguage();
	void ShowSubViewLanguage(bool show);

	bool CreateSubViewDateTime();
	bool DestroySubViewDateTime();
	void ShowSubViewDateTime(bool show);

	bool CreateSubViewVentilation();
	bool DestroySubViewVentilation();
	void ShowSubViewVentilation(bool bShow);

	bool CreateSubViewPatient();
	bool DestroySubViewPatient();
	void ShowSubViewPatient(bool bShow);

	bool CreateSubViewSystem();
	bool DestroySubViewSystem();
	void ShowSubViewSystem(bool bShow);

	bool CreateSubViewService();
	bool DestroySubViewService();
	void ShowSubViewService(bool bShow);

	bool CreateSubViewTools();
	bool DestroySubViewTools();
	void ShowSubViewTools(bool show);

	bool CreateSubViewVideo();
	bool DestroySubViewVideo();
	void ShowSubViewVideo(bool show);

	bool CreateSubViewNetwork();
	bool DestroySubViewNetwork();
	void ShowSubViewNetwork(bool show);

	bool CreateSubViewCO2();
	bool DestroySubViewCO2();
	void ShowSubViewCO2(bool show);

	bool CreateSubViewSPO2();
	bool DestroySubViewSPO2();
	void ShowSubViewSPO2(bool show);

	//Menu buttons
	bool CreateSubViewMenu();
	bool DestroySubViewMenu();
	void ShowSubViewMenu(bool bShow);

	void OpenSettingsView(UINT viewFlag);
	void CloseSettingsView(UINT viewFlag);
	void DestroySettingsView();

	bool DestroyWndSubSettingScreenlocktime();
	bool CreateWndSubSettingScreenlocktime(UINT viewFlag);
	void ShowWndSubSettingScreenlocktime(bool show);

	bool DestroyWndSubSettingVentRange();
	bool CreateWndSubSettingVentRange(UINT viewFlag);
	void ShowWndSubSettingVentRange(bool show);

	bool DestroyWndSubSettingRatioItime();
	bool CreateWndSubSettingRatioItime(UINT viewFlag);
	void ShowWndSubSettingRatioItime(bool show);

	bool DestroyWndSubSettingLeakComp();
	bool CreateWndSubSettingLeakComp(UINT viewFlag);
	void ShowWndSubSettingLeakComp(bool show);

	bool DestroyWndSubSettingManbreathTime();
	bool CreateWndSubSettingManbreathTime(UINT viewFlag);
	void ShowWndSubSettingManbreathTime(bool show);

	bool DestroyWndSubSettingSubPatAlarmDelay();
	bool CreateWndSubSettingSubPatAlarmDelay(UINT viewFlag);
	void ShowWndSubSettingSubPatAlarmDelay(bool show);

	bool DestroyWndSubSettingVolTrigger();
	bool CreateWndSubSettingVolTrigger(UINT viewFlag);
	void ShowWndSubSettingVolTrigger(bool show);

	bool DestroyWndSubSettingPpsvAbsolute();
	bool CreateWndSubSettingPpsvAbsolute(UINT viewFlag);
	void ShowWndSubSettingPpsvAbsolute(bool show);
	

	bool DestroyWndSubSettingTubeset();
	bool CreateWndSubSettingTubeset(UINT viewFlag);
	void ShowWndSubSettingTubeset(bool show);

	bool DestroyWndSubSettingEFlow();
	bool CreateWndSubSettingEFlow(UINT viewFlag);
	void ShowWndSubSettingEFlow(bool show);


	bool DestroyWndSubSettingAutoOxyCal();
	bool CreateWndSubSettingAutoOxyCal(UINT viewFlag);
	void ShowWndSubSettingAutoOxyCal(bool show);

	bool DestroyWndSubSettingHFOManBreath();
	bool CreateWndSubSettingHFOManBreath(UINT viewFlag);
	void ShowWndSubSettingHFOManBreath(bool show);

	bool DestroyWndSubSettingCO2Module();
	bool CreateWndSubSettingCO2Module(UINT viewFlag);
	void ShowWndSubSettingCO2Module(bool show);

	bool DestroyWndSubSettingSPO2Module();
	bool CreateWndSubSettingSPO2Module(UINT viewFlag);
	void ShowWndSubSettingSPO2Module(bool show);

	bool DestroyWndSubSettingSPO2sensitivity();
	bool CreateWndSubSettingSPO2sensitivity(UINT viewFlag);
	void ShowWndSubSettingSPO2sensitivity(bool show);

	bool DestroyWndSubSettingSPO2averagingtime();
	bool CreateWndSubSettingSPO2averagingtime(UINT viewFlag);
	void ShowWndSubSettingSPO2averagingtime(bool show);

	bool DestroyWndSubSettingSPO2alarmdelay();
	bool CreateWndSubSettingSPO2alarmdelay(UINT viewFlag);
	void ShowWndSubSettingSPO2alarmdelay(bool show);



	bool DestroyWndSubSettingCO2Units();
	bool CreateWndSubSettingCO2Units(UINT viewFlag);
	void ShowWndSubSettingCO2Units(bool show);

	bool DestroyWndSubSettingBaroPressure();
	bool CreateWndSubSettingBaroPressure(UINT viewFlag);
	void ShowWndSubSettingBaroPressure(bool show);

	bool DestroyWndSubSettingAutoOnPump();
	bool CreateWndSubSettingAutoOnPump(UINT viewFlag);
	void ShowWndSubSettingAutoOnPump(bool show);

	bool DestroyWndSubSettingPressureUnit();
	bool CreateWndSubSettingPressureUnit(UINT viewFlag);
	void ShowWndSubSettingPressureUnit(bool show);

	
	void OpenMenuSelect();
	void ShowMenu(eMenuView menu);
	void LanguageChanged();

protected:
	bool m_bExit;

	CWndMenuMenu *m_pWndMenuMenu;

	 CSubViewMenuSelect *m_pcSubViewMenu;

	CSubViewCalFlowSensor *m_pcSubViewFlowSensor;
	CSubViewCalOxySensor *m_pcSubViewOxySensor;
	CSubViewDisplay *m_pcSubViewDisplay;
	CSubViewService *m_pcSubViewService;
	CSubViewSystem *m_pcSubViewSystem;
	CSubViewPatient *m_pcSubViewPatient;
	CSubViewVentilation *m_pcSubViewVentilation;
	CSubViewDateTime *m_pcSubViewDateTime;
	CSubViewLanguage *m_pcSubViewLanguage;
	CSubViewTools *m_pcSubViewTools;
	CSubViewCO2 *m_pcSubViewCO2;
	CSubViewVideo *m_pcSubViewVideo;
	CSubViewNetwork *m_pcSubViewNetwork;
	CSubViewSPO2 *m_pcSubViewSPO2;


	CWndSubSettingsScreenlocktime *m_pcWndSubScreenlocktime;

	CWndSubSettingsVentRange *m_pcWndSubVentRange;
	CWndSubSettingsRatioItime *m_pcWndSubRatioItime;
	CWndSubSettingsManbreathTime *m_pcWndSubManbreathTime;
	CWndSubSettingsVolTrigger *m_pcWndSubVolTrigger;
	CWndSubSettingsPpsvAbsolute *m_pcWndSubPpsvAbsolute;
	CWndSubSettingsTubeset *m_pcWndSubTubeset;
	CWndSubSettingsEFlow *m_pcWndSubEFlow;
	CWndSubSettingsAutoOxyCal *m_pcWndSubAutoOxyCal;
	CWndSubSettingsHFOManBreath *m_pcWndSubHFOManBreath;
	CWndSubSettingsPatAlarmDelay *m_pcWndSubPatAlarmDelay;
	CWndSubSettingsLeakComp *m_pcWndSubLeakComp;
	CWndSubSettingsCO2Module *m_pcWndSubCO2Module;
	CWndSubSettingsCO2Units *m_pcWndSubCO2Units;
	CWndSubSettingsBaroPressure *m_pcWndSubBaroPressure;
	CWndSubSettingsPressureUnit *m_pcWndSubPressureUnit;
	CWndSubSettingsAutoOnPump *m_pcWndSubAutoOnPump;
	CWndSubSettingsSPO2Module *m_pcWndSubSPO2Module;
	CWndSubSettingsSPO2sensitivity *m_pcWndSubSPO2sensitivity;
	CWndSubSettingsSPO2averagingtime *m_pcWndSubSPO2averagingtime;
	CWndSubSettingsSPO2alarmdelay *m_pcWndSubSPO2alarmdelay;
	
	eMenuView m_eCurView;
	eMenuView m_ePrevView;

	UINT m_viewFlag;

	bool m_bVideoRunning;

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


