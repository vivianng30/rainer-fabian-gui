#pragma once
#include "WndSubSettings.h"

// CWndSubSettingsSPO2averagingtime

class CWndSubSettingsSPO2averagingtime : public CWndSubSettings
{
	DECLARE_DYNAMIC(CWndSubSettingsSPO2averagingtime)

public:
	CWndSubSettingsSPO2averagingtime(UINT viewFlag);
	virtual ~CWndSubSettingsSPO2averagingtime();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	void Initialize();
	void Draw();

	void SetAveragingTime(UINT btn);

protected:
	eSPO2averagingtime m_eSPO2averagingTime;

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
	afx_msg void OnBnClicked6();
	afx_msg void OnBnClicked7();
};