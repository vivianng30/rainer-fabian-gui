#pragma once
#include "WndSubSettings.h"

// CWndSubSettingsPressureUnit

class CWndSubSettingsPressureUnit : public CWndSubSettings
{
	DECLARE_DYNAMIC(CWndSubSettingsPressureUnit)

public:
	CWndSubSettingsPressureUnit(UINT viewFlag);
	virtual ~CWndSubSettingsPressureUnit();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	void Initialize();
	void Draw();

	void SetUnit(UINT btn);

protected:

	ePressureUnit m_ePressureUnit;

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
