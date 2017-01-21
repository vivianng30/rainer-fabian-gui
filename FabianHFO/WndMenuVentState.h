#pragma once
#include "PresetMenuBtn.h"
//#include "MenuBtn.h"
//#include "Library/PushBtn.h"
#include "MVView.h"


// CWndMenuVentState


class CWndMenuVentState : public CWnd
{
	DECLARE_DYNAMIC(CWndMenuVentState)

public:
	CWndMenuVentState(CMVView *parentView);
	virtual ~CWndMenuVentState();

	void Init();
	void Show(BOOL bShow);



	virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void SetAllButtonUnpressed();
	void SetOneButtonDepressed(int btnID);
	void SetOneButtonPreset(int btnID);
	//void SetButton(int btnID);
	void SetButtonState(int btnID, CPresetMenuBtn::eBtnState state);
	void ResetButtonPresetstate();

	//int GetCurrentPressedBtn();
	//void SetCurrentPressedBtn(int btn);

	/*void OnBnClickedMenuIPPV();
	void OnBnClickedMenuSIPPV();
	void OnBnClickedMenuSIMV();
	void OnBnClickedMenuSIMVPSV();
	void OnBnClickedMenuPSV();
	void OnBnClickedMenuHFO();
	void OnBnClickedMenuCPAP();
	void OnBnClickedMenuNCPAP();
	void OnBnClickedMenuDUOPAP();*/



protected:

	void ShowMenuBtn();
	void HideMenuBtn();

	CMVModel *getModel();




	CPresetMenuBtn* m_pcMenuIPPV;
	CPresetMenuBtn* m_pcMenuSIPPV;
	CPresetMenuBtn* m_pcMenuSIMV;
	CPresetMenuBtn* m_pcMenuSIMVPSV;
	CPresetMenuBtn* m_pcMenuPSV;
	CPresetMenuBtn* m_pcMenuHFO;
	CPresetMenuBtn* m_pcMenuCPAP;
	CPresetMenuBtn* m_pcMenuNCPAP;
	CPresetMenuBtn* m_pcMenuDUOPAP;
	CPresetMenuBtn* m_pcMenuTherapy;
	//CNumBtn* m_pcMenuVolume;
	//CPresetMenuBtn* m_pcMenu10;
	
	//int m_iCurrentPressedBtn;

	//CNumBtn* m_pcMenuNumeric;

private:
	CMVView *m_parentView;

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

	/*CBmp* m_pcNumeric_Up;
	CBmp* m_pcNumeric_Dw;
	CBmp* m_pcNumeric_Fc;*/


	CList<CPresetMenuBtn* , CPresetMenuBtn *> m_plMenuBtn;

	CMVModel *m_pModel;
protected:
	//{{AFX_MSG(CWndMenuVentState)
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	//afx_msg void OnBnClickedMenuIPPV();
	//afx_msg void OnBnClickedMenuSIPPV();
	//afx_msg void OnBnClickedMenuSIMV();
	//afx_msg void OnBnClickedMenuSIMVPSV();
	//afx_msg void OnBnClickedMenuPSV();
	//afx_msg void OnBnClickedMenuHFO();
	//afx_msg void OnBnClickedMenuCPAP();
	//afx_msg void OnBnClickedMenuNCPAP();
	//afx_msg void OnBnClickedMenuDUOPAP();
	////afx_msg void OnBnClickedMenuVolume();
	//afx_msg void OnBnClickedMenu10();
	//afx_msg void OnBnClickedMenuNUMERIC();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


