/**********************************************************************************************//**
 * \file	WndMenuTrend.h.
 *
 * Declares the window menu trend class
 **************************************************************************************************/

#pragma once
//#include "NumBtn.h"
#include "MenuBtn.h"
#include "PushBtn.h"
#include "MVView.h"
// CWndMenuGraphs

class CWndMenuTrend : public CWnd
{
	DECLARE_DYNAMIC(CWndMenuTrend)

public:
	CWndMenuTrend();
	virtual ~CWndMenuTrend();

	void Init(CMVView *parentView);
	void Show(BOOL bShow);


	virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

protected:
	CMVModel *getModel();

	void ShowMenuBtn();
	void HideMenuBtn();

	//void SetOneButtonDepressed(int btnID);
	//void SetAllButtonUnpressed();


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


	CPushBtn* m_pcMenu1;
	CPushBtn* m_pcMenu2;
	CPushBtn* m_pcMenu3;
	//CPushBtn* m_pcMenu3;
	CPushBtn* m_pcMenu4;
	CPushBtn* m_pcMenu5;

	//CPushBtn* m_pcMenuNum;

	//CNumBtn* m_pcMenu5;
	CBmp* m_pcMenu_Up;
	CBmp* m_pcMenu_Dw;
	CBmp* m_pcMenu_Dis;
	/*CBmp* m_pcNumeric_Up;
	CBmp* m_pcNumeric_Dw;*/

	CBmp* m_pcMenu_IncrUp;
	CBmp* m_pcMenu_IncrDw;
	CBmp* m_pcMenu_DecrUp;
	CBmp* m_pcMenu_DecrDw;
	CBmp* m_pcMenu_MoveLeftUp;
	CBmp* m_pcMenu_MoveLeftDw;
	CBmp* m_pcMenu_MoveRightUp;
	CBmp* m_pcMenu_MoveRightDw;

	/*CBmp* m_pcSettings_Up;
	CBmp* m_pcSettings_Dw;*/
	//CBmp* m_pcMenu_Fc;

	//CBmp* m_pcNumeric_Up;
	//CBmp* m_pcNumeric_Dw;
	//CBmp* m_pcNumeric_Fc;


	//CList<CMenuBtn* , CMenuBtn *> m_plMenuBtn;

	CMVModel *m_pModel;
protected:
	//{{AFX_MSG(CWndMenuTrend)
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};






