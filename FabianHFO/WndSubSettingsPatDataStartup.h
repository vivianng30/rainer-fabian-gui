/**********************************************************************************************//**
 * \file	WndSubSettingsPatDataStartup.h.
 *
 * Declares the window sub settings pattern data startup class
 **************************************************************************************************/

#pragma once
#include "WndSubSettings.h"
#include "BitmapSlider.h"

// CWndSubSettingsPatDataStartup

class CWndSubSettingsPatDataStartup : public CWndSubSettings
{
	DECLARE_DYNAMIC(CWndSubSettingsPatDataStartup)

public:
	CWndSubSettingsPatDataStartup(UINT viewFlag);
	virtual ~CWndSubSettingsPatDataStartup();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	void Initialize();
	void Draw();

	//void SetAskStartup(UINT btn);

protected:

	CBitmapSlider*	m_slider;

	bool m_bAskStartup;

protected:
	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg LRESULT OnMyMessage(WPARAM wParam, LPARAM lParam);
	//afx_msg void OnBnClicked1();
	//afx_msg void OnBnClicked2();
	//afx_msg void OnBnClicked3();
	//afx_msg void OnBnClicked4();
	//afx_msg void OnBnClicked5();
	//afx_msg void OnBnClicked6();
	//afx_msg void OnBnClicked7();
};
