/**********************************************************************************************//**
 * \file	WndSubSettingsTubeset.h.
 *
 * Declares the window sub settings tubeset class
 **************************************************************************************************/

#pragma once
#include "WndSubSettings.h"

// CWndSubSettingsTubeset

class CWndSubSettingsTubeset : public CWndSubSettings
{
	DECLARE_DYNAMIC(CWndSubSettingsTubeset)

public:
	CWndSubSettingsTubeset(UINT viewFlag);
	virtual ~CWndSubSettingsTubeset();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	void Initialize();
	void Draw();

	void SetTubeSet(UINT btn);

protected:

	eTubeSet m_eTubeSet;

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

