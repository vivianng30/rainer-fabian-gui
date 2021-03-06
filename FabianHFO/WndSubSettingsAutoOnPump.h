#pragma once
#include "WndSubSettings.h"

// CWndSubSettingsAutoOnPump

class CWndSubSettingsAutoOnPump : public CWndSubSettings
{
	DECLARE_DYNAMIC(CWndSubSettingsAutoOnPump)

public:
	CWndSubSettingsAutoOnPump(UINT viewFlag);
	virtual ~CWndSubSettingsAutoOnPump();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	void Initialize();
	void Draw();

	void SetTimePumpAutoOn(UINT btn);

protected:
	BYTE m_iCO2TimePumpAutoOn;

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


