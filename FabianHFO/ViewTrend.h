/**********************************************************************************************//**
 * \file	ViewTrend.h.
 *
 * Declares the view trend class
 **************************************************************************************************/

#pragma once
#include "MVView.h"
#include "WndMenuTrend.h"
#include "WndMenuTrendtype.h"
#include "WndTrendTimeaxis.h"
#include "TrendDiagramm.h"
#include "SelectTrendBtn.h"
#include "UpDwnBtn.h"
#include "WndWaitHourglass.h"


class CViewTrend :public CMVView
{
public:
	enum eTrend {
		TREND0,		// 0,
		TREND1,		// 1, 
		TREND2,		// 2,
		TREND3		// 3, 
			};

	CViewTrend(int ViewID = 0);
	virtual ~CViewTrend(void);

	virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	virtual bool CreateView();
	virtual bool Initialize();

	virtual void Show(bool bRedraw=false);
	virtual void Hide();

	virtual void SetViewFocus();
	virtual bool SetNextFocus();
	virtual bool SetPrevFocus();

	//Observer
	virtual void NotifyEvent(CMVEvent* pEvent);

protected:
	bool CreateWndMenuTrend();
	void ShowWndMenuTrend(bool bShow);
	bool DestroyWndMenuTrend();

	bool CreateWndMenuTrendtype(UINT bSelTrend);
	void ShowWndMenuTrendtype(bool bShow, UINT iTrendType, UINT bSelTrend);
	bool DestroyWndMenuTrendtype();

	bool CreateWndTrendTimeaxis();
	void ShowWndTrendTimeaxis(bool bShow);
	bool DestroyWndTrendTimeaxis();

	bool CreateWndHourglass();
	void ShowWndHourglass(bool bShow);
	bool DestroyWndHourglass();

	bool CreateTrend1Diagram(int x, int y, int nWidth, int nHeight);
	void DeleteTrend1Diagram(void);
	void ShowTrend1Diagram(bool bShow);

	bool CreateTrend2Diagram(int x, int y, int nWidth, int nHeight);
	void DeleteTrend2Diagram(void);
	void ShowTrend2Diagram(bool bShow);

	bool CreateTrend3Diagram(int x, int y, int nWidth, int nHeight);
	void DeleteTrend3Diagram(void);
	void ShowTrend3Diagram(bool bShow);

	void DrawCursor(int iTrend);

	void newTrendType(UINT type);
	void newTrendType1();
	void newTrendType2();
	void newTrendType3();

	void SetOneButtonDepressed(int btnID);
	void SetAllButtonUnpressed();

	void DrawTrends();
	void DrawTrend1();
	void DrawTrend2();
	void DrawTrend3();

	CStringW GetNameOfTrendType(UINT type);
	CStringW GetUnitOfTrendType(UINT type);
	int GetMaxScaleOfTrendType(UINT type);

	void loadAllTrendData();
	void loadTrendData(BYTE type, eTrend trend);
	
	bool IncreaseTimeSpan();
	bool DecreaseTimeSpan();

	bool m_bLoadDataRunning;


protected:
	CRITICAL_SECTION csTrends;

	HDC		m_hdcStatic;
	HBITMAP m_hbmpStatic;
	HBITMAP		m_hBmpPrevStatic;

	CWndWaitHourglass *m_pWndHourglass;
		
	CWndMenuTrendtype *m_pWndMenuTrendtype;
	CWndMenuTrend *m_pWndMenuTrend;
	CWndTrendTimeaxis *m_pWndTrendTimeaxis;

	bool m_bExit;

	bool m_bWndMenuTrendtype;

	CList<CSelectTrendBtn* , CSelectTrendBtn *> m_plSelectTrendBtn;

	CSelectTrendBtn* m_pcSelectTrend1;
	CSelectTrendBtn* m_pcSelectTrend2;
	CSelectTrendBtn* m_pcSelectTrend3;

	int m_iCurFocusedWnd;
	bool m_bGraphSelected;

	UINT m_iCurBtnSelTrend;

	CBmp* m_pcSelectTrend_Up;
	CBmp* m_pcSelectTrend_Dw;

	CBmp* m_pcNumPara2er;

	CTrendDiagramm *m_pcTrend1Diagram;
	CTrendDiagramm *m_pcTrend2Diagram;
	CTrendDiagramm *m_pcTrend3Diagram;

	COleDateTime m_dtStartTime;
	COleDateTime m_dtOffsetTime;

	UINT m_iTypeTrend1;
	UINT m_iTypeTrend2;
	UINT m_iTypeTrend3;

	UINT m_iTrendSpan;

	LPG_PLOTITEMSSTRUCT_TD m_pPlotItemsTrend1;
	LPG_FUNCTIONSTRUCT_TD m_lpfsTrend1;

	LPG_PLOTITEMSSTRUCT_TD m_pPlotItemsTrend2;
	LPG_FUNCTIONSTRUCT_TD m_lpfsTrend2;

	LPG_PLOTITEMSSTRUCT_TD m_pPlotItemsTrend3;
	LPG_FUNCTIONSTRUCT_TD m_lpfsTrend3;

	bool m_bFilledGraph;


	static WORD *m_pBufferTrend1;
	static WORD *m_pBufferTrend2;
	static WORD *m_pBufferTrend3;

	int m_iCountLockedMoveLeft;
	int m_iCountLockedMoveRight;

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

