#pragma once
#include "MenuBtn.h"
#include "PushBtn.h"
#include "MVView.h"
#include "UpDwnBtn.h"

// CWndGraphViewSelection

class CWndGraphViewSelection : public CWnd
{
	DECLARE_DYNAMIC(CWndGraphViewSelection)

public:
	CWndGraphViewSelection(CMVView *parentView);
	virtual ~CWndGraphViewSelection();

	void Init();
	void Show(BOOL bShow);


	virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
protected:
	CMVModel *getModel();

	/*void ShowMenuBtn();
	void HideMenuBtn();*/

	void SetOneButtonDepressed(int btnID);
	void SetAllButtonUnpressed();

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

	bool m_bPopupBtn;

	CMenuBtn* m_pcWaveView;
	CMenuBtn* m_pcLoopView;
	CMenuBtn* m_pcCO2View;
	CMenuBtn* m_pcSPO2View;
	/*CMenuBtn* m_pcEMGView;
	CMenuBtn* m_pcCLFIO2View;*/
	CMenuBtn* m_pcTrendView;
	
	CUpDwnBtn* m_pcSelView;

	CBmp* m_pcMenu_Up;
	CBmp* m_pcMenu_Dw;
	CBmp* m_pcMenu_Dis;

	CBmp* m_pcSelView_Up;
	CBmp* m_pcSelView_Dw;

	CBmp* m_pcMenu_Graph_Up;
	CBmp* m_pcMenu_Graph_Dw;
	CBmp* m_pcMenu_Graph_Dis;

	CBmp* m_pcMenu_Loop_Up;
	CBmp* m_pcMenu_Loop_Dw;
	CBmp* m_pcMenu_Loop_Dis;

	CBmp* m_pcMenu_Trend_Up;
	CBmp* m_pcMenu_Trend_Dw;
	CBmp* m_pcMenu_Trend_Dis;

	CList<CMenuBtn* , CMenuBtn *> m_plMenuBtn;

	CMVModel *m_pModel;
protected:
	//{{AFX_MSG(CWndMenuTrend)
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnKillFocus(CWnd* pNewWnd);
	//afx_msg void OnSetFocus(CWnd* pOldWnd);
};


