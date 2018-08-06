/**********************************************************************************************//**
 * \file	WndServiceAccuboard.h.
 *
 * Declares the window service accuboard class
 **************************************************************************************************/

#pragma once
#include "WndService.h"
#include "KbrdBtn.h"

// CWndServiceAccuboard

class CWndServiceAccuboard : public CWndService
{
	DECLARE_DYNAMIC(CWndServiceAccuboard)

public:
	CWndServiceAccuboard();
	virtual ~CWndServiceAccuboard();

	//virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);
	////virtual BOOL PreTranslateMessage(MSG* pMsg);

	//void Show(bool bShow);
	void Show(bool bShow);

	void Init();

protected:
	void Draw();

private:
	CBmp* m_pcUp;
	CBmp* m_pcDw;
	//CBmp* m_pcDis;

	CDialog*   m_pDlg;

	CKbrdBtn* m_pcAccuTest;

	WORD m_wACCU_DAT_STATE;
	WORD m_wACCU_DAT_REMAIN;
	WORD m_wACCU_DAT_PERCENT;
	WORD m_wACCU_DAT_CAPACITY;
	WORD m_wACCU_DAT_VOLTAGE;
	WORD m_wACCU_DAT_DISCHARGECURRENT;
	WORD m_wACCU_DAT_CHARGECURRENT;
	WORD m_wACCU_DAT_TEMPERATURE;
	WORD m_wACCU_DAT_CYCLES;
	WORD m_wACCU_DAT_VERSION;
	WORD m_wACCU_DAT_CHECKSUM;

	

	//int m_iPARST0_Data;
	int m_iOpMinBatt;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAccuTest();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


