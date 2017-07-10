#pragma once
#include "globDefs.h"
#include "MVModel.h"
#include "PushBtn.h"
//#include "KbrdBtn.h"
#include "colour.h"
#include "SelectSetupBtn.h"
#include "BitmapSlider.h"

// CSubViewSPO2

class CSubViewSPO2 : public CWnd
{
	DECLARE_DYNAMIC(CSubViewSPO2)

public:
	CSubViewSPO2();
	virtual ~CSubViewSPO2();
	virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);
	bool CreateWnd(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void Show(bool bShow);

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

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

	//CDialog*   m_pDlg;

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

	/*CBmp* m_pcZero_Up;
	CBmp* m_pcZero_Dw;
	CBmp* m_pcZero_Fc;
	CSelectSetupBtn* m_pcZero;*/

	CBmp* m_pcRegionTopLeCorner;
	CBmp* m_pcRegionTopRiCorner;
	CBmp* m_pcRegionBotLeCorner;
	CBmp* m_pcRegionBotRiCorner;

	//CBmp* m_pcSetupVent_Dis;
	CBmp* m_pcSetupVent_Up;
	CBmp* m_pcSetupVent_Dw;
	CBmp* m_pcSetupVent_Fc;
	CBmp* m_pcSetupVent_Dis;

	CBmp* m_pcSetupVentDis_Dw;
	CBmp* m_pcSetupVentDis_Fc;

	CDialog*   m_pDlg;

	CList<CSelectSetupBtn* , CSelectSetupBtn *> m_plBtn;

	CSelectSetupBtn* m_pcModuleEnabled;
	CSelectSetupBtn* m_pcSensitivityMode;
	CSelectSetupBtn* m_pcFastSAT;
	CSelectSetupBtn* m_pcSensitivity;
	CSelectSetupBtn* m_pcAalarmdelay;
	CSelectSetupBtn* m_pcAveragingTime;
	//CSelectSetupBtn* m_pcSIQlimit;

	BYTE m_bySPO2Module;

	//SHORT m_iSpO2SIQlimit;
	bool m_bFastSATon;
	eSPO2sensitivitymode m_eSensitivityMode;
	eSPO2alarmdelay m_eSPO2alarmdelay;
	eSPO2averagingtime m_eSPO2averagingTime;

	CBitmapSlider*	m_sliderFastSAT;
	
	WORD m_iProductID;
	WORD m_iVersionDSP;
	WORD m_iVersionMCU;

	CString m_szModuleVersion;

	SHORT m_iEXCEPTIONS2;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedSPO2module();
	afx_msg void OnBnClickedSensitivityMode();
	afx_msg void OnBnClickedFastSAT();
	afx_msg void OnBnClickedSPO2alarmdelay();
	afx_msg void OnBnClickedSPO2averaging();
	//afx_msg void OnBnClickedSIQlimit();
	afx_msg LRESULT OnMyMessage(WPARAM wParam, LPARAM lParam);
};


