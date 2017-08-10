#pragma once
#include "PushBtn.h"
#include "Bmp.h"
#include "MVEvent.h"
#include "MVModel.h"
#include "MenuBtn.h"
#include "PresetMenuBtn.h"
#include "DTUpDwnBtn.h"
#include "SelectSetupBtn.h"

// CSubViewCalFlowSensor

class CSubViewCalFlowSensor : public CWnd
{
	DECLARE_DYNAMIC(CSubViewCalFlowSensor)

public:
	CSubViewCalFlowSensor();
	virtual ~CSubViewCalFlowSensor();

	virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);
	bool CreateWnd(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void SetOneButtonDepressed(int btnID);
	void SetAllButtonUnpressed();

	void SetOneButtonPreset_NeoPed(int btnID);
	void SetOneButtonDepressed_NeoPed(int btnID);
	void showNeoPedMessage(int btnID);

	void Init();
	void Show(BOOL bShow);

	void ShowFlowBtns(bool disable);

	void DrawFlowSensor(bool bStatic=false, bool bText=false);
	
	void SetFlowSensorText(CStringW szTextTop, CStringW szTextBot);
	void RefreshFlowSensorText(CStringW szTextTop, CStringW szTextBot);

	void RefreshFlowSensorValues(int iHval,int iLval,int iHdac,int iLdac);
	void SetFlowSensorValues(int iHval,int iLval,int iHdac,int iLdac);

	void SetCalState(eFlowSensCalibrationstate state=FS_UNKNOWN,bool bRedraw=false);
	bool CheckFlowSensorState(bool bRedraw=false, bool bWrite=true, bool bCalibrating=false, bool bDrawError=false);
	void SetCalibrationOk(bool bCalibrated);
	void SetCalRunning(bool state);



	void StartFlowSensorCheckThread();
	void StopFlowSensorCheckThread();
	friend UINT CCheckFlowSensorThread(LPVOID pc);

	DWORD CheckFlowSensor();
	void CalibrateFlowSensor();
	void resetFlowCalAlarms();

	bool IsCalibrationActive();

protected:
	void DrawStatic();
	void DrawCalText();
	CMVModel *getModel();

private:
	// Attributes
	CMVModel* m_pModel;


	HDC			m_hDC;
	HBITMAP		m_hBmp;
	HBITMAP		m_hBmpPrev;
	LONG		m_lX;
	LONG		m_lY;
	LONG		m_lXo;
	LONG		m_lYo;

	CBmp* m_pcMenu_Up;
	CBmp* m_pcMenu_Dw;
	CBmp* m_pcMenu_Dis;
	CPushBtn* m_pcFlowOnOff;
	CPushBtn* m_pcFlowCalExt;
	//CPushBtn* m_pcFlowCalInt;

	CPresetMenuBtn* m_pcNeonatal;
	CPresetMenuBtn* m_pcPediatric;

	CBmp* m_pcNeonatal_Up;
	CBmp* m_pcNeonatal_Dw;
	CBmp* m_pcNeonatal_Fc;

	CBmp* m_pcPediatric_Up;
	CBmp* m_pcPediatric_Dw;
	CBmp* m_pcPediatric_Fc;
	CBmp* m_pcPediatric_Dis;

	CMenuBtn* m_pcMenuCO2;
	CMenuBtn* m_pcMenuSpO2;
	CMenuBtn* m_pcMenuOxi;

	CSelectSetupBtn* m_pbtnBodyweight;

	CBmp* m_pcStatic_Up;
	CBmp* m_pcStatic_Dw;
	CBmp* m_pcStatic_Fc;

	CBmp* m_pcMenuCO2_UP;
	CBmp* m_pcMenuCO2_DW;
	CBmp* m_pcMenuCO2_FC;
	CBmp* m_pcMenuCO2_DIS;

	CBmp* m_pcMenuSPO2_UP;
	CBmp* m_pcMenuSPO2_DW;
	CBmp* m_pcMenuSPO2_FC;
	CBmp* m_pcMenuSPO2_DIS;

	CBmp* m_pcMenuOxi_UP;
	CBmp* m_pcMenuOxi_DW;
	CBmp* m_pcMenuOxi_FC;
	CBmp* m_pcMenuOxi_DIS;

	CList<CMenuBtn* , CMenuBtn *> m_plMenuBtn;

	HDC		m_hdcStatic;
	HDC		m_hdcTmp;
	HBITMAP m_hbmpStatic;
	HBITMAP m_hbmpTmp;

	CDialog*   m_pDlg;
	/*TCHAR m_pszTextFlowSensorTop[MAX_PATH];
	TCHAR m_pszTextFlowSensorBot[MAX_PATH];*/

	CStringW m_szTextFlowSensorTop;
	CStringW m_szTextFlowSensorBot;
	int m_iFlowHValue;
	int m_iFlowLValue;
	int m_iFlowHDAC;
	int m_iFlowLDAC;


	CBmp* m_pcRegionTopLeCorner;
	CBmp* m_pcRegionTopRiCorner;
	CBmp* m_pcRegionBotLeCorner;
	CBmp* m_pcRegionBotRiCorner;

	CBmp* m_pcOk;
	CBmp* m_pcExclamation;
	CBmp* m_pcCalibration;
	CBmp* m_pcWait;
	CBmp* m_pcWait1;
	CBmp* m_pcWait2;
	CBmp* m_pcWait3;
	CBmp* m_pcWait4;
	CBmp* m_pcWait5;
	CBmp* m_pcWait6;
	CBmp* m_pcWait7;
	CBmp* m_pcWait8;

	int m_iWaitCount;
	bool m_bCalRunning;
	bool m_bCkeckRunning;
	//bool m_bNeoPedConfirmed;

	eFlowSensCalibrationstate m_eFlowCalState;

	CStringW m_szLastFlowCal;

	int m_iBodyweight;

	bool m_bExit;
	bool m_bFlowsensor;
	bool m_bInfantFlow;

	bool m_bCalActive;
	bool m_bDoFlowCheck;
	CWinThread*	m_pcwtCheckFlow;
	HANDLE m_hThreadCheckFlow;
	
	bool m_bFlowSensorCalibrated;
	bool m_bFlowCheckStarted;
	bool m_bCancelFlowCal;
	bool m_bFlowCalStarted;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedO2();
	afx_msg void OnBnClickedCO2();
	afx_msg void OnBnClickedSPO2();
	afx_msg void OnBnClickedBodyweight();
};


