/**********************************************************************************************//**
 * \file	WndServiceDisplay.h.
 *
 * Declares the window service display class
 **************************************************************************************************/

#pragma once
#include "WndService.h"
#include "KbrdBtn.h"
#include "PushBtn.h"

// CWndServiceDisplay

class CWndServiceDisplay : public CWndService
{
	DECLARE_DYNAMIC(CWndServiceDisplay)

public:
	CWndServiceDisplay();
	virtual ~CWndServiceDisplay();

	//virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);
	////virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);


	//void Show(bool bShow);

	void Init();

protected:
	void Draw();

	//void BnClickedCalibration();
	void BnClickedIncrease();
	void BnClickedDecrease();


private:
	CBmp* m_pcCal_Up;
	CBmp* m_pcCal_Dw;
	CKbrdBtn* m_pcCal;

	CBmp* m_pcLeft_Up;
	CBmp* m_pcLeft_Dw;
	CBmp* m_pcRight_Up;
	CBmp* m_pcRight_Dw;
	CPushBtn* m_pcLeft;
	CPushBtn* m_pcRight;

	/*CBmp* m_pcSave_Up;
	CBmp* m_pcSave_Dw;
	CBmp* m_pcSave_Red;
	CNumBtn* m_pcSave;*/

	bool m_bContrastChanged;

	CDialog*   m_pDlg;

	//CSliderCtrl	*m_pctlSlider;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedCalibrate();
	//afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};


