#pragma once
#include "ParaBtn.h"
#include "Bmp.h"
// CSubParaBtn

class CSubParaBtn : public CParaBtn
{
	DECLARE_DYNAMIC(CSubParaBtn)

public:
	CSubParaBtn(BTN btn, int nOffset=0,bool bScrollOver=true);
	virtual ~CSubParaBtn();
	virtual BOOL Create(CWnd* pParentWnd,DWORD dwStyle, fVALUE v);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	 void RefreshBtn(bool bTempValue=false);
	 void SetParaValue(int v,bool bRedraw=false);

	void TriggerBreath();
	void SetTimerRunning(bool state);


	

	// Operations
public:
	virtual void Draw(int nState);

protected:
	void DrawSUBPARA_I_E(int nState);
	void DrawSUBPARA_O2REAL(int nState);
	void DrawSUBPARA_O2FLUSH(int nState);
	void DrawSUBPARA_BIASFLOW(int nState);
	void DrawSUBPARA_PSVTIME(int nState);
	void DrawSUBPARA_DEMFLOW(int nState);
	void DrawSUBPARA_BPM(int nState);
	void DrawSUBPARA_ETIME(int nState);
	void DrawSUBPARA_LEAKCOMP(int nState);
	void DrawSUBPARA_TRIGGER(int nState);
	void DrawSUBPARA_TRIGGER_DUOPAP(int nState);
	void DrawSUBPARA_TRIGGER_NCPAP(int nState);
	void DrawSUBPARA_TRIGGER_CPAP(int nState);
	void DrawSUBPARA_TRIGGER_DEFAULT(int nState);
	void DrawDEFAULT(int nState);

	int GetTextColorStateDependend(int nState);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUDParaBtn)
public:
	//virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

protected:
	CBmp* m_pcWait;
	CBmp* m_pcWait1;
	CBmp* m_pcWait2;
	CBmp* m_pcWait3;
	CBmp* m_pcWait4;
	CBmp* m_pcWait5;
	CBmp* m_pcWait6;
	CBmp* m_pcWait7;
	CBmp* m_pcWait8;

	bool m_bTriggeredBreath;
	
	int m_iWaitCount;
	bool m_bCalRunning;

	bool m_bTempValue;
	// Generated message map functions
protected:
	//{{AFX_MSG(CParaBtn_ITIME)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	/*afx_msg void OnSetFocus(CWnd* pOldWnd);*/
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


