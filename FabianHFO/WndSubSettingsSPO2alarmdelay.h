/**********************************************************************************************//**
 * \file	WndSubSettingsSPO2alarmdelay.h.
 *
 * Declares the window sub settings spo 2alarmdelay class
 **************************************************************************************************/

#pragma once
#include "WndSubSettings.h"

// CWndSubSettingsSPO2alarmdelay

class CWndSubSettingsSPO2alarmdelay : public CWndSubSettings
{
	DECLARE_DYNAMIC(CWndSubSettingsSPO2alarmdelay)

public:
	CWndSubSettingsSPO2alarmdelay(UINT viewFlag);
	virtual ~CWndSubSettingsSPO2alarmdelay();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	void Initialize();
	void Draw();

	void SetAlarmDelay(UINT btn);

protected:
	eSPO2alarmdelay m_eSPO2alarmdelay;

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
	//afx_msg void OnBnClicked5();
	//afx_msg void OnBnClicked6();
	//afx_msg void OnBnClicked7();
};