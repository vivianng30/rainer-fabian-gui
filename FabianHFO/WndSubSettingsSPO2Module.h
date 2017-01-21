#pragma once
#include "WndSubSettings.h"
#include "BitmapSlider.h"

// CWndSubSettingsSPO2Module

class CWndSubSettingsSPO2Module : public CWndSubSettings
{
	DECLARE_DYNAMIC(CWndSubSettingsSPO2Module)

public:
	CWndSubSettingsSPO2Module(UINT viewFlag);
	virtual ~CWndSubSettingsSPO2Module();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void writeData();

protected:
	void Initialize();
	void Draw();

	void SetModule(UINT btn);

protected:

	BYTE m_bySPO2Module;

protected:
	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnBnClicked1();
	afx_msg void OnBnClicked2();
	//afx_msg void OnBnClicked3();
};


