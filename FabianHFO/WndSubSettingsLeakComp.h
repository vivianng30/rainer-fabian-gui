#pragma once
#include "WndSubSettings.h"


class CWndSubSettingsLeakComp : public CWndSubSettings
{
	DECLARE_DYNAMIC(CWndSubSettingsLeakComp)

public:
	CWndSubSettingsLeakComp(UINT viewFlag);
	virtual ~CWndSubSettingsLeakComp();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	void Initialize();
	void Draw();

	void setLeakCompOff(UINT btn);

protected:
	eLeakCompensation m_eLeakCompensation;

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

