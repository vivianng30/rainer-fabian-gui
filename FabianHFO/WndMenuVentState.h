/**********************************************************************************************//**
 * \file	WndMenuVentState.h.
 *
 * Declares the window menu vent state class
 **************************************************************************************************/

#pragma once
#include "PresetMenuBtn.h"
#include "MVView.h"


// CWndMenuVentState


class CWndMenuVentState : public CWnd
{
	DECLARE_DYNAMIC(CWndMenuVentState)

public:
	CWndMenuVentState();
	virtual ~CWndMenuVentState();

	void Init();
	void Show(BOOL bShow);

	BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	BOOL PreTranslateMessage(MSG* pMsg);

	void SetAllButtonUnpressed();
	void SetOneButtonDepressed(int btnID);
	void SetOneButtonPreset(int btnID);
	void SetButtonState(int btnID, CPresetMenuBtn::eBtnState state);
	void ResetButtonPresetstate();


protected:

	void ShowMenuBtn();
	void HideMenuBtn();

	CMVModel *getModel();

	CPresetMenuBtn m_menuIPPV;
	CPresetMenuBtn m_menuSIPPV;
	CPresetMenuBtn m_menuSIMV;
	CPresetMenuBtn m_menuSIMVPSV;
	CPresetMenuBtn m_menuPSV;
	CPresetMenuBtn m_menuHFO;
	CPresetMenuBtn m_menuCPAP;
	CPresetMenuBtn m_menuNCPAP;
	CPresetMenuBtn m_menuDUOPAP;
	CPresetMenuBtn m_menuTherapy;

private:
	// Attributes
	HDC			m_hDC;
	HBITMAP		m_hBmp;
	HBITMAP		m_hBmpPrev;
	LONG		m_lX;
	LONG		m_lY;
	LONG		m_lXo;
	LONG		m_lYo;

	CBmp* m_pcMenu_Up;
	CBmp* m_pcMenu_Dw;
	CBmp* m_pcMenu_Fc;
	CBmp* m_pcMenu_Dis;

	CList<CPresetMenuBtn* , CPresetMenuBtn *> m_plMenuBtn;

	CMVModel *m_pModel;
protected:
	//{{AFX_MSG(CWndMenuVentState)
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


