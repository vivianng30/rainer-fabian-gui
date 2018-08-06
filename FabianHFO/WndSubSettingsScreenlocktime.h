/**********************************************************************************************//**
 * \file	WndSubSettingsScreenlocktime.h.
 *
 * Declares the window sub settings screenlocktime class
 **************************************************************************************************/

#pragma once
#include "WndSubSettings.h"

// CWndSubSettingsScreenlocktime

class CWndSubSettingsScreenlocktime : public CWndSubSettings
{
	DECLARE_DYNAMIC(CWndSubSettingsScreenlocktime)

public:
	CWndSubSettingsScreenlocktime(UINT viewFlag);
	virtual ~CWndSubSettingsScreenlocktime();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	void Initialize();
	void Draw();

	void SetAutoScreenlockTime(UINT btn);

protected:
	eAutoScreenlock m_eAutoSreenlock;

protected:
	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	//afx_msg LRESULT OnMyMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClicked1();
	afx_msg void OnBnClicked2();
	afx_msg void OnBnClicked3();
	afx_msg void OnBnClicked4();
	afx_msg void OnBnClicked5();
	//afx_msg void OnBnClicked6();
	//afx_msg void OnBnClicked7();
};


