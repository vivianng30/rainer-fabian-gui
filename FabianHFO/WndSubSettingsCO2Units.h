/**********************************************************************************************//**
 * \file	WndSubSettingsCO2Units.h.
 *
 * Declares the window sub settings co 2 units class
 **************************************************************************************************/

#pragma once
#include "WndSubSettings.h"

// CWndSubSettingsCO2Units

class CWndSubSettingsCO2Units : public CWndSubSettings
{
	DECLARE_DYNAMIC(CWndSubSettingsCO2Units)

public:
	CWndSubSettingsCO2Units(UINT viewFlag);
	virtual ~CWndSubSettingsCO2Units();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	void Initialize();
	void Draw();

	void SetCO2Units(UINT btn);

protected:

	eCO2unit m_eCO2units;

protected:
	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	//afx_msg LRESULT OnMyMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClicked1();
	afx_msg void OnBnClicked2();
	afx_msg void OnBnClicked3();
	//afx_msg void OnBnClicked4();
	//afx_msg void OnBnClicked5();
	//afx_msg void OnBnClicked6();
	//afx_msg void OnBnClicked7();
};
