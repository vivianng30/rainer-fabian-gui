#pragma once
#include "WndService.h"
#include "BitmapSlider.h"
#include "KbrdBtn.h"

// CWndServiceSettings

class CWndServiceSettings : public CWndService
{
	DECLARE_DYNAMIC(CWndServiceSettings)

public:
	CWndServiceSettings();
	virtual ~CWndServiceSettings();

	//virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);
	////virtual BOOL PreTranslateMessage(MSG* pMsg);

	//void Show(bool bShow);

	void Init();

protected:
	void Draw();

private:
	CBitmapSlider*	m_sliderEnableNurse;
	CBitmapSlider*	m_sliderEnableNeoPed;
	CBitmapSlider*	m_sliderEnableFactory;
	CBitmapSlider*	m_sliderEnableAutolimit;

	CDialog*   m_pDlg;

	CBmp* m_pcTest_Up;
	CBmp* m_pcTest_Dw;
	CBmp* m_pcTest_Dis;
	/*CKbrdBtn* m_pcTest;*/

	CKbrdBtn* m_pcSetFactory;
	CKbrdBtn* m_pcEraseFRAM;
	CKbrdBtn* m_pcSetAsHospital;
	CKbrdBtn* m_pcDeleteCurHospital;
	CKbrdBtn* m_pcSaveHospitalToUSB;
	CKbrdBtn* m_pcLoadHospitalFromUSB;
	CKbrdBtn* m_pcRestoreHospital;
	CKbrdBtn* m_pcInstallReg;

	bool m_bRegAvailable;
	bool m_bUSBavailable;
	bool m_bUSBDataAvailable;
	bool m_bFFSDIDKDataAvailable;

	CBmp* m_pcNurse;
	CBmp* m_pcHelp_Up;
	CBmp* m_pcHelp_Dw;

	CKbrdBtn* m_pcHelpFactory;
	CKbrdBtn* m_pcHelpAutoAL;

	bool m_bUseNeoPed;
	bool m_bNurscallActive;
	bool m_bEnableFactory;
	bool m_bEnableAutolimit;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnMyMessage(WPARAM wParam, LPARAM lParam);
	//afx_msg void OnBnClickedTestNursecall();
	afx_msg void OnBnClickedSetHospital();
	afx_msg void OnBnClickedDeleteHospital();
	afx_msg void OnBnClickedSaveHospital();
	afx_msg void OnBnClickedLoadHospital();
	afx_msg void OnBnClickedRestoreHospital();
	afx_msg void OnBnClickedRestoreFactory();
	afx_msg void OnBnClickedEraseFRAM();
	afx_msg void OnBnClickedHelpFactory();
	afx_msg void OnBnClickedHelpAutoAL();
	afx_msg void OnBnClickedRegInstall();
};


