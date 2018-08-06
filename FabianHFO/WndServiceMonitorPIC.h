/**********************************************************************************************//**
 * \file	WndServiceMonitorPIC.h.
 *
 * Declares the window service monitor picture class
 **************************************************************************************************/

#pragma once
#include "WndService.h"

// CWndServiceMonitorPIC

class CWndServiceMonitorPIC : public CWndService
{
	DECLARE_DYNAMIC(CWndServiceMonitorPIC)

public:
	CWndServiceMonitorPIC();
	virtual ~CWndServiceMonitorPIC();

	//virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);
	////virtual BOOL PreTranslateMessage(MSG* pMsg);

	//void Show(bool bShow);

	void Init();

protected:
	void Draw();

private:
	
	MAINBOARD_DATA m_dataMainboard;

	//BYTE m_bChargestate;
	//BYTE m_bRemainingAccu;
	//SHORT m_sUBATT_Data;
	WORD m_wBattStatMinutes;
	WORD m_wBattStatPercents;
	int m_iOXYGEN_Data;
	int m_iPARST0_Data;
	int m_iADC0_I_Data;
	int m_iADC0_I_Offs;
	int m_iADC0_I_Scl;
	int m_iADC1_E_Data;
	int m_iADC1_E_Offs;
	int m_iADC1_E_Scl;
	int m_iPPROX_Data;
	int m_iPPROX_Offs;
	int m_iPPROX_Scl;
	int m_iOXY1_Data;
	int m_iOXY1_Offs;
	int m_iOXY1_Scl;
	int m_iOXY2_Data;
	int m_iOXY2_Offs;
	int m_iOXY2_Scl;
	int m_iP_AIR_Data;
	int m_iP_OXY_Data;
	int m_iP_MIX_Data;

	int m_iVCC5_Data;
	int m_iVCC24_Data;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


