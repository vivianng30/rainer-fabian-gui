/**********************************************************************************************//**
 * \file	WndSubSettingsAbortPSV.h.
 *
 * Declares the window sub settings abort psv class
 **************************************************************************************************/

#pragma once
#include "WndSubSettings.h"
#include "DTUpDwnBtn.h"
#include "SelectSetupBtn.h"
// CWndSubSettingsAbortPSV

class CWndSubSettingsAbortPSV : public CWndSubSettings
{
	DECLARE_DYNAMIC(CWndSubSettingsAbortPSV)

public:
	CWndSubSettingsAbortPSV(UINT viewFlag);
	virtual ~CWndSubSettingsAbortPSV();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
protected:
	void Initialize();
	void Draw();

	//void SetAutoScreenlockTime(UINT btn);

protected:
	CBmp* m_pcUp_Up;
	CBmp* m_pcUp_Dw;
	CBmp* m_pcDw_Up;
	CBmp* m_pcDw_Dw;

	CBmp* m_pcStatic_Up;
	CBmp* m_pcStatic_Dw;
	CBmp* m_pcStatic_Fc;

	CSelectSetupBtn* m_pbtnValue;

	CDTUpDwnBtn* m_pcNextUp;
	CDTUpDwnBtn* m_pcNextDwn;

	int m_iValue;
	int m_iCounter;
	eTimeChanger m_eTimeChanger;

protected:
	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedValue();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

