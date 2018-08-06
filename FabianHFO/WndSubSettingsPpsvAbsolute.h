/**********************************************************************************************//**
 * \file	WndSubSettingsPpsvAbsolute.h.
 *
 * Declares the window sub settings ppsv absolute class
 **************************************************************************************************/

#pragma once
#include "WndSubSettings.h"

// CWndSubSettingsPpsvAbsolute

class CWndSubSettingsPpsvAbsolute : public CWndSubSettings
{
	DECLARE_DYNAMIC(CWndSubSettingsPpsvAbsolute)

public:
	CWndSubSettingsPpsvAbsolute(UINT viewFlag);
	virtual ~CWndSubSettingsPpsvAbsolute();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	void Initialize();
	void Draw();

	void SetPPSVasDeltaPEEPValue(UINT btn);

protected:
	BOOL m_bPPSVasDeltaPEEPValue;

protected:
	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	//afx_msg LRESULT OnMyMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClicked1();
	afx_msg void OnBnClicked2();
	//afx_msg void OnBnClicked3();
	//afx_msg void OnBnClicked4();
	//afx_msg void OnBnClicked5();
	//afx_msg void OnBnClicked6();
	//afx_msg void OnBnClicked7();
};

