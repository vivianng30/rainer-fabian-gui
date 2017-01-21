#pragma once
#include "WndSubSettings.h"
#include "DTUpDwnBtn.h"
#include "SelectSetupBtn.h"
#include "UpDwnBtn.h"
// CWndSubSettingsEFlow

class CWndSubSettingsEFlow : public CWndSubSettings
{
	DECLARE_DYNAMIC(CWndSubSettingsEFlow)

public:
	CWndSubSettingsEFlow(UINT viewFlag);
	virtual ~CWndSubSettingsEFlow();
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

	CUpDwnBtn* m_pcPara_Select;
	CBmp* m_pcPara_Select_yes;
	CBmp* m_pcPara_Select_no;

	int m_iValue;
	int m_iCounter;
	eTimeChanger m_eTimeChanger;

	int m_iLowerLimit;
	int m_iUpperLimit;

protected:
	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedValue();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

