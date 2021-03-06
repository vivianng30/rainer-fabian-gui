/**********************************************************************************************//**
 * \file	WndSubSettingsBTBforVT.h.
 *
 * Declares the window sub settings bt bfor vt class
 **************************************************************************************************/

#pragma once
#include "WndSubSettings.h"
#include "BitmapSlider.h"


// CWndSubSettingsBTBforVT

class CWndSubSettingsBTBforVT : public CWndSubSettings
{
	DECLARE_DYNAMIC(CWndSubSettingsBTBforVT)

public:
	CWndSubSettingsBTBforVT(UINT viewFlag);
	virtual ~CWndSubSettingsBTBforVT();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	void Initialize();
	void Draw();


protected:

	CBitmapSlider*	m_slider;

	bool m_bUseTveBTB;

protected:
	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg LRESULT OnMyMessage(WPARAM wParam, LPARAM lParam);
	//afx_msg void OnBnClicked1();
	//afx_msg void OnBnClicked2();
	//afx_msg void OnBnClicked3();
	//afx_msg void OnBnClicked4();
	//afx_msg void OnBnClicked5();
	//afx_msg void OnBnClicked6();
	//afx_msg void OnBnClicked7();
};

