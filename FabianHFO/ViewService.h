/**********************************************************************************************//**
 * \file	ViewService.h.
 *
 * Declares the view service class
 **************************************************************************************************/

#pragma once
#include "MVView.h"
#include "WndServiceMenu.h"
//#include "WndMenuServiceSelection.h"

#include "WndServiceSystem.h"
#include "WndServiceAccuboard.h"
#include "WndServiceCalibration.h"
#include "WndServiceDisplay.h"
#include "WndServiceFlowboard.h"
#include "WndServiceMonitorPIC.h"
#include "WndServiceSettings.h"
#include "WndServiceValves.h"
#include "WndServiceInterfaces.h"
#include "WndServiceNetwork.h"
#include "WndServiceLicensing.h"
#include "WndServiceMaintenance.h"

class CViewService :public CMVView
{
public:
	CViewService(int ViewID = 0);
	virtual ~CViewService(void);

	virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);

	virtual bool CreateView();
	virtual bool Initialize();
	virtual void Open();
	virtual void Close();

	virtual void Show(bool bRedraw);
	virtual void Hide();

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	virtual void SetViewFocus();
	virtual bool SetNextFocus();
	virtual bool SetPrevFocus();


	//Observer
	virtual void NotifyEvent(CMVEvent* pEvent);
	//virtual void NotifyVentModeChanged();
	//virtual void NotifyViewStateChanged();


protected:
	void setCurServiceWnd(eServiceView serviceWnd);
	eServiceView getCurServiceWnd();

	bool CreateWndSystem();
	bool DestroyWndSystem();
	void ShowWndSystem(bool show);

	bool CreateWndDisplay();
	bool DestroyWndDisplay();
	void ShowWndDisplay(bool show);

	bool CreateWndSettings();
	bool DestroyWndSettings();
	void ShowWndSettings(bool show);

	bool CreateWndCalibration();
	bool DestroyWndCalibration();
	void ShowWndCalibration(bool show);

	bool CreateWndFlowboard();
	bool DestroyWndFlowboard();
	void ShowWndFlowboard(bool show);

	bool CreateWndAccuboard();
	bool DestroyWndAccuboard();
	void ShowWndAccuboard(bool show);

	bool CreateWndValves();
	bool DestroyWndValves();
	void ShowWndValves(bool show);

	bool CreateWndMonitorPIC();
	bool DestroyWndMonitorPIC();
	void ShowWndMonitorPIC(bool show);

	bool CreateWndInterfaces();
	bool DestroyWndInterfaces();
	void ShowWndInterfaces(bool show);

	bool CreateWndNetwork();
	bool DestroyWndNetwork();
	void ShowWndNetwork(bool show);

	bool CreateWndLicensing();
	bool DestroyWndLicensing();
	void ShowWndLicensing(bool show);

	bool CreateWndServiceMenu();
	bool DestroyWndServiceMenu();
	void ShowWndServiceMenu(bool bShow);

	bool CreateWndMaintenance();
	bool DestroyWndMaintenance();
	void ShowWndMaintenance(bool show);
	
	//void Draw();

protected:
	
	CRITICAL_SECTION	csServiceWnd;

	CWndServiceSystem *m_pcWndSystem;
	CWndServiceDisplay *m_pcWndDisplay;
	CWndServiceSettings *m_pcWndSettings;
	CWndServiceCalibration *m_pcWndCalibration;
	CWndServiceFlowboard *m_pcWndFlowboard;
	CWndServiceAccuboard *m_pcWndAccuboard;
	CWndServiceValves *m_pcWndValves;
	CWndServiceMonitorPIC *m_pcWndMonitorPIC;
	CWndServiceInterfaces *m_pcWndInterfaces;
	CWndServiceNetwork *m_pcWndNetwork;
	CWndServiceLicensing *m_pcWndLicensing;
	CWndServiceMaintenance *m_pcWndMaintenance;
	


	CWndServiceMenu *m_pWndServiceMenu;

	eServiceView m_eCurServiceWnd;


public:
	DECLARE_MESSAGE_MAP()
	//afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	//afx_msg void OnTimer(UINT_PTR nIDEvent);
};




