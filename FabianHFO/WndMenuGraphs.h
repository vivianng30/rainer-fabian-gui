/**********************************************************************************************//**
 * \file	WndMenuGraphs.h.
 *
 * Declares the window menu graphs class
 **************************************************************************************************/

#pragma once
#include "MenuBtn.h"
#include "PushBtn.h"
#include "UpDwnBtn.h"
#include "MVView.h"


// CWndMenuGraphs
class CWndMenuGraphs : public CWnd
{
	DECLARE_DYNAMIC(CWndMenuGraphs)

public:
	CWndMenuGraphs(CMVView *parentView);
	virtual ~CWndMenuGraphs();

	void Init();
	void Show(BOOL bShow);
	
	//test inbiolab
#ifdef INBIOLAB_VERSION
	void Draw();
#endif
	
	virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	void FlowSensorStateChanged();
	//bool IsButtonPushed(int nIDButton );

protected:

	CMVModel *getModel();

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

	//test inbiolab
#ifdef INBIOLAB_VERSION
	CUpDwnBtn* m_pcSelView;
#else
	CMenuBtn* m_pcMenuWave;
	CMenuBtn* m_pcMenuLoops;
	CMenuBtn* m_pcMenuCO2;
	CMenuBtn* m_pcMenuSPO2;
	CMenuBtn* m_pcMenuFOT;
	CMenuBtn* m_pcMenuTrend;

	CBmp* m_pcMenu_Graph_Up;
	CBmp* m_pcMenu_Graph_Dw;
	CBmp* m_pcMenu_Graph_Dis;

	CBmp* m_pcMenu_Loop_Up;
	CBmp* m_pcMenu_Loop_Dw;
	CBmp* m_pcMenu_Loop_Dis;

	CBmp* m_pcMenu_Trend_Up;
	CBmp* m_pcMenu_Trend_Dw;
	CBmp* m_pcMenu_Trend_Dis;
#endif

	
	

	

	CUpDwnBtn* m_pcMenuFreeze;
	CPushBtn* m_pcMenuSave;

	CBmp* m_pcMenu_Save_Up;
	CBmp* m_pcMenu_Save_Dw;
	CBmp* m_pcMenu_Save_Dis;

	CBmp* m_pcMenu_Freeze_Up;
	CBmp* m_pcMenu_Freeze_Dw;
	CBmp* m_pcMenu_Freeze_Dis;

	CBmp* m_pcMenu_Up;
	CBmp* m_pcMenu_Dw;
	
	
	//test inbiolab
#ifdef INBIOLAB_VERSION
	CBmp* m_pcSelView_Up;
	CBmp* m_pcSelView_Dw;
	bool m_bViewMenu;
#else
	CBmp* m_pcMenu_Dis;
#endif


	CList<CMenuBtn* , CMenuBtn *> m_plMenuBtn;

	CMVModel *m_pModel;

	
	bool m_bFREEZE;
	
protected:
	//{{AFX_MSG(CWndMenuGraphs)
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	//afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};




