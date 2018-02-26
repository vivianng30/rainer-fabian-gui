/**********************************************************************************************//**
 * \file	WndSubSettingsAutoOxyCal.h.
 *
 * Declares the window sub settings automatic oxy calendar class
 **************************************************************************************************/

#pragma once
#include "WndSubSettings.h"

// CWndSubSettingsAutoOxyCal

class CWndSubSettingsAutoOxyCal : public CWndSubSettings
{
	DECLARE_DYNAMIC(CWndSubSettingsAutoOxyCal)

public:
	CWndSubSettingsAutoOxyCal(UINT viewFlag);
	virtual ~CWndSubSettingsAutoOxyCal();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	void Initialize();
	void Draw();

	void SetAutoOxyCal(UINT btn);

protected:
	eAutoOxyCal m_eAutoOxyCal;

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

