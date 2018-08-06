/**********************************************************************************************//**
 * \file	SubViewCO2.h.
 *
 * Declares the sub view co 2 class
 **************************************************************************************************/

#pragma once
#include "globDefs.h"
#include "MVModel.h"
#include "PushBtn.h"
//#include "KbrdBtn.h"
#include "colour.h"
#include "SelectSetupBtn.h"
#include "BitmapSlider.h"
// CSubViewCO2

class CSubViewCO2 : public CWnd
{
	DECLARE_DYNAMIC(CSubViewCO2)

public:
	CSubViewCO2();
	virtual ~CSubViewCO2();
	virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);
	bool CreateWnd(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void Show(bool bShow);

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

protected:
	//void Initialize();
	void Draw();

	void SetOneButtonDepressed(int btnID);
	void SetOneButtonFocused(int btnID);
	void SetNextButtonFocused();
	void SetPrevButtonFocused();
	eBtnState GetCurrentBtnState();
	void SetButtonClicked(int btnID);

	void OpenSettingsView();

	CMVModel *getModel();

private:
	CMVModel* m_pModel;


	DWORD m_dwLastSetupTimer;
	int m_iCurPara;

	WORD m_kUp;
	WORD m_kDown;
	WORD m_kSpace;

	// Attributes
	HDC			m_hDC;
	HBITMAP		m_hBmp;
	HBITMAP		m_hBmpPrev;
	LONG		m_lX;
	LONG		m_lY;
	LONG		m_lXo;
	LONG		m_lYo;

	CBmp* m_pcZero_Up;
	CBmp* m_pcZero_Dw;
	CBmp* m_pcZero_Fc;
	CSelectSetupBtn* m_pcZero;

	CBmp* m_pcRegionTopLeCorner;
	CBmp* m_pcRegionTopRiCorner;
	CBmp* m_pcRegionBotLeCorner;
	CBmp* m_pcRegionBotRiCorner;

	//CBmp* m_pcSetupVent_Dis;
	CBmp* m_pcSetupVent_Up;
	CBmp* m_pcSetupVent_Dw;
	CBmp* m_pcSetupVent_Fc;
	CBmp* m_pcSetupVent_Dis;

	CDialog*   m_pDlg;

	CList<CSelectSetupBtn* , CSelectSetupBtn *> m_plBtn;

	CSelectSetupBtn* m_pcModuleEnabled;
	CSelectSetupBtn* m_pcCO2units;
	CSelectSetupBtn* m_pcBarometricP;
	CSelectSetupBtn* m_pcAutoOnPump;
	//CSelectSetupBtn* m_pcETCO2avg;

	//CBitmapSlider*	m_sliderAutoOn;

	bool m_bCO2disabled;
	bool m_bPumpStateOn;

	BYTE m_byExtendedStateBytes;
	BYTE m_byStateBytes;
	BYTE m_byCO2Module;

	CString m_szModuleVersion;

	int m_iMaxOpHRSCalibration;
	COleDateTime m_dtCalibrationDateTime;
	int m_iRemainServiceHrs;
	bool m_bSystemDateOk;
	bool m_bCalibrationIsDue;
	bool m_bServiceIsDue;

	BYTE m_iAutoOnPump;

	//bool m_bCO2ModuleEnabled;
	eCO2unit m_eCO2units;
	SHORT m_iBarometricP;
	//SHORT m_iO2Compensation;
	//eETCO2avg m_eETCO2avg;

	bool m_bInprogress;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedCO2module();
	afx_msg void OnBnClickedCO2units();
	afx_msg void OnBnClickedBarometricPressure();
	afx_msg void OnBnClickedAutoOnPump();
	afx_msg void OnBnClickedETCO2avg();
	afx_msg void OnBnClickedZeroCal();
	//afx_msg LRESULT OnMyMessage(WPARAM wParam, LPARAM lParam);
};


