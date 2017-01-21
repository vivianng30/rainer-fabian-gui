#pragma once
#include "WndService.h"
#include "KbrdBtn.h"
#include "UDBtn.h"
#include "BitmapSlider.h"
// CWndServiceCalibration



class CWndServiceCalibration : public CWndService
{
	DECLARE_DYNAMIC(CWndServiceCalibration)

public:
	CWndServiceCalibration();
	virtual ~CWndServiceCalibration();

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);


	void Init();


	

protected:
	void Draw();

	void checkVentRunState();

private:
	CBmp* m_pcFlowCorr_Up;
	CBmp* m_pcFlowCorr_Dw;
	CUDBtn* m_pcFlowCorrNEO;
	CUDBtn* m_pcFlowCorrPED;
	//pro, new - start
	CUDBtn* m_pcAltitude;
	//pro, new - end

	CBitmapSlider*	m_sliderEnableBTPS;

	CBmp* m_pcUp;
	CBmp* m_pcDw;
	CBmp* m_pcDis;

	CDialog*   m_pDlg;

	int m_iFCOR_NEO;
	int m_iFCOR_PED;
	int m_iWaitCount;

	//pro, new - start
	WORD m_iAltitude;
	//pro, new - end	

	bool m_bUseBTPS;
	bool m_bUpdateData;
	bool m_bSysCalRunning;
	CStringW m_csSysCal;

	CStringW m_csProxCal;

	bool m_bPProx0CalRunning;
	bool m_bPProx60CalRunning;
	

	CBmp* m_pcWait;
	CBmp* m_pcWait1;
	CBmp* m_pcWait2;
	CBmp* m_pcWait3;
	CBmp* m_pcWait4;
	CBmp* m_pcWait5;
	CBmp* m_pcWait6;
	CBmp* m_pcWait7;
	CBmp* m_pcWait8;

	CKbrdBtn* m_pcSysCal;
	CKbrdBtn* m_pcPProx0;
	CKbrdBtn* m_pcPProx60;
	CKbrdBtn* m_pcExhCal;

	SHORT m_iPressure;
	int m_iPProxADC;
	int m_iPProxMonSCL;
	int m_iPProxMonOFFS;
	int m_iPProxConSCL;
	int m_iPProxConOFFS;

	CStringW m_szLast0mbarCal;
	CStringW m_szLast60mbarCal;
	int m_iADC0_I_Data;
	int m_iADC0_I_Offs;
	int m_iADC0_I_Scl;
	int m_iADC0_I_Dac;
	int m_iADC1_E_Data;
	int m_iADC1_E_Offs;
	int m_iADC1_E_Scl;
	int m_iADC1_E_Dac;

	short m_iCntCheckVentState;

	bool m_bProx0mbarCalibrated;

	double CalcCorrectionFactor(double iAltitude);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnMyMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedSysCal();
	afx_msg void OnBnClickedProx0();
	afx_msg void OnBnClickedProx60();
	afx_msg void OnBnClickedExhCal();
};