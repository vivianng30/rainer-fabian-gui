/**********************************************************************************************//**
 * \file	WndSubSettingsVlimitVgarant.h.
 *
 * Declares the window sub settings vlimit vgarant class
 **************************************************************************************************/

#pragma once
#include "WndSubSettings.h"

// CWndSubSettingsVlimitVgarant

class CWndSubSettingsVlimitVgarant : public CWndSubSettings
{
	DECLARE_DYNAMIC(CWndSubSettingsVlimitVgarant)

public:
	CWndSubSettingsVlimitVgarant(UINT viewFlag);
	virtual ~CWndSubSettingsVlimitVgarant();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	void Initialize();
	void Draw();

	void SetControl(UINT btn);

protected:

	eVolumeControl m_eVControl;

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
