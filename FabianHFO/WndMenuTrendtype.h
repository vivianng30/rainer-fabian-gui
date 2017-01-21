#pragma once
//#include "NumBtn.h"
#include "MenuBtn.h"
#include "PushBtn.h"
#include "MVView.h"
// CWndMenuGraphs

class CWndMenuTrendtype : public CWnd
{
	DECLARE_DYNAMIC(CWndMenuTrendtype)

public:
	CWndMenuTrendtype(CMVView *parentView);
	virtual ~CWndMenuTrendtype();

	void Init();
	void Show(BOOL bShow, UINT iTrendType, UINT bSelTrend);
	void Draw();

	virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, UINT bSelTrend);

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

protected:
	CMVModel *getModel();

	void ShowMenuBtn();
	void HideMenuBtn();

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

	UINT m_iCurBtnSelTrend;

	CMenuBtn* m_pcTrendPINSP;
	CMenuBtn* m_pcTrendPMEAN;
	CMenuBtn* m_pcTrendFIO2;
	CMenuBtn* m_pcTrendVTE;
	CMenuBtn* m_pcTrendCOMPLIANCE;
	CMenuBtn* m_pcTrendCO2HFO;
	CMenuBtn* m_pcTrendMV;
	CMenuBtn* m_pcTrendHFAMP;

	CMenuBtn* m_pcTrendRSBI;
	CMenuBtn* m_pcTrendSHAREMVMAND;
	CMenuBtn* m_pcTrendRESISTANCE;
	CMenuBtn* m_pcTrendLEAK;
	CMenuBtn* m_pcTrendSPO2;
	CMenuBtn* m_pcTrendSPO2PI;
	CMenuBtn* m_pcTrendETCO2;
	CMenuBtn* m_pcTrendSPO2PR;

	//CNumBtn* m_pcMenu5;
	CBmp* m_pcMenu_Up;
	CBmp* m_pcMenu_Dw;

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






