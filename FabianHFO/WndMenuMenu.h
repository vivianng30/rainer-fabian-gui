/**********************************************************************************************//**
 * \file	WndMenuMenu.h.
 *
 * Declares the window menu class
 **************************************************************************************************/

#pragma once
#include "PushBtn.h"
#include "MVView.h"

// CWndMenuMenu

class CWndMenuMenu : public CWnd
{
	DECLARE_DYNAMIC(CWndMenuMenu)

public:
	CWndMenuMenu();
	virtual ~CWndMenuMenu();

	virtual void Init(CMVView *parentView);
	virtual void Show(BOOL bShow);


	virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void Draw();
	void SetMenuText(CStringW txt, bool bRedraw=true);

	//void NotifyLanguageChanged();
protected:

	CMVModel *getModel();

	void ShowMenuBtn();
	void HideMenuBtn();

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


	CPushBtn* m_pcMoveLeft;
	CPushBtn* m_pcMoveRight;
	//CPushBtn* m_pcMenu;
	CPushBtn* m_pcBack;


	CBmp* m_pcMenu_Up;
	CBmp* m_pcMenu_Dw;
	//CBmp* m_pcMenu_Dis;

	CBmp* m_pcMenuLeft_Up;
	CBmp* m_pcMenuLeft_Dw;
	CBmp* m_pcMenuLeft_Dis;

	CBmp* m_pcMenuRight_Up;
	CBmp* m_pcMenuRight_Dw;
	CBmp* m_pcMenuRight_Dis;

	CBmp* m_pcNumeric_Up;
	CBmp* m_pcNumeric_Dw;

	CBmp* m_pcRegionTopLeCorner;
	CBmp* m_pcRegionTopRiCorner;
	CBmp* m_pcRegionBotLeCorner;
	CBmp* m_pcRegionBotRiCorner;

	bool m_bMenuView;

	CStringW m_csMenuText;

	CMVModel *m_pModel;

	
protected:
	//{{AFX_MSG(CWndMenuCalibration)
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};


