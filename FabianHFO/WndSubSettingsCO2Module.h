#pragma once
#include "WndSubSettings.h"
#include "BitmapSlider.h"

// CWndSubSettingsCO2Module

class CWndSubSettingsCO2Module : public CWndSubSettings
{
	DECLARE_DYNAMIC(CWndSubSettingsCO2Module)

public:
	CWndSubSettingsCO2Module(UINT viewFlag);
	virtual ~CWndSubSettingsCO2Module();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//void writeData();

protected:
	void Initialize();
	void Draw();

	void SetModule(UINT btn);

protected:

	BYTE m_byCO2Module;

protected:
	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnBnClicked1();
	afx_msg void OnBnClicked2();
	afx_msg void OnBnClicked3();
};


