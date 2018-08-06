/**********************************************************************************************//**
 * \file	WndServiceFlowboard.h.
 *
 * Declares the window service flowboard class
 **************************************************************************************************/

#pragma once
#include "WndService.h"
#include "KbrdBtn.h"
//#include "UDBtn.h"

// CWndServiceFlowboard

class CWndServiceFlowboard : public CWndService
{
	DECLARE_DYNAMIC(CWndServiceFlowboard)

public:
	CWndServiceFlowboard();
	virtual ~CWndServiceFlowboard();

	//virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);
	//virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	//void Show(bool bShow);

	void Init();

protected:
	void Draw();

private:
	/*CBmp* m_pcFlowCorr_Up;
	CBmp* m_pcFlowCorr_Dw;
	CUDBtn* m_pcFlowCorr;*/

	//int m_iFCOR;
	int m_iFSCAL_Offs;
	int m_iADC0_I_Data;
	int m_iADC0_I_Offs;
	int m_iADC0_I_Scl;
	int m_iADC0_I_Dac;
	int m_iADC1_E_Data;
	int m_iADC1_E_Offs;
	int m_iADC1_E_Scl;
	int m_iADC1_E_Dac;
	SHORT m_iFlow; //Achtung, tatsächlicher Meß-Wert ist um Faktor 2 größer
	SHORT m_iPressure;
	SHORT m_iVolume;
	SHORT m_iOxy;
	int m_iBLENDERstate;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


