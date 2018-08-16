/**********************************************************************************************//**
 * \file	WndMenuAlarmLimits.h.
 *
 * Declares the window menu alarm limits class
 **************************************************************************************************/

#pragma once
#include "AlarmBtn.h"
#include "PushBtn.h"
#include "MVView.h"
// CWndMenuCalibration

class CWndMenuAlarmLimits : public CWnd
{
	DECLARE_DYNAMIC(CWndMenuAlarmLimits)

public:
	CWndMenuAlarmLimits(void);
	virtual ~CWndMenuAlarmLimits();

	virtual void Init(CMVView *parentView);
	virtual void Show(BOOL bShow);

	
	virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	
protected:

	CMVModel *getModel();

	void ShowMenuBtn();
	void HideMenuBtn();

	void refreshCurALimitNumeric();
	//void BnMenuNum();

	//void OnBnClickedMenu1();
	void OnBnClickedMenu2();
	void OnBnClickedMenu3();
	//void OnBnClickedMenu4();
	void OnBnClickedMenu5();

	void OnBnALimitNumeric();


private:
	CMVView *m_parentView;

	// Attributes
	HDC			m_hDC;
	HBITMAP		m_hBmp;
	HBITMAP		m_hBmpPrev;
	LONG		m_lX;
	LONG		m_lY;
	LONG		m_lXo;
	LONG		m_lYo;

	eAlarmLoudness m_eAlarmVolume;


	//CAlarmBtn* m_pcMenu1;
	CAlarmBtn* m_pcMenu2;
	CAlarmBtn* m_pcMenu3;
	//CAlarmBtn* m_pcMenu4;
	CAlarmBtn* m_pcMenu5;

	CPushBtn* m_pcMenuNum;

	CBmp* m_pcMenu_AutosetUp;
	CBmp* m_pcMenu_AutosetDw;

	CBmp* m_pcMenu_Up;
	CBmp* m_pcMenu_Dw;
	//CBmp* m_pcAlOff_Up;
	//CBmp* m_pcAlOff_Dw;
	CBmp* m_pcAlLoud_Up;
	CBmp* m_pcAlLoud_Dw;
	CBmp* m_pcAlMedium_Up;
	CBmp* m_pcAlMedium_Dw;
	CBmp* m_pcAlSilent_Up;
	CBmp* m_pcAlSilent_Dw;
	//CBmp* m_pcMenu_Fc;
	CBmp* m_pcNumeric_Up;
	CBmp* m_pcNumeric_Dw;
	//CBmp* m_pcNumeric_Fc;


	//CList<CAlarmBtn* , CAlarmBtn *> m_plMenuBtn;

	CMVModel *m_pModel;
protected:
	//{{AFX_MSG(CWndMenuAlarmLimits)
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	/*afx_msg void OnBnClickedMenu1();
	afx_msg void OnBnClickedMenu2();
	afx_msg void OnBnClickedMenu3();
	afx_msg void OnBnClickedMenu4();
	afx_msg void OnBnClickedMenu5();*/
	//afx_msg void OnBnClickedMenuNum();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};





