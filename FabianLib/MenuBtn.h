#pragma once
#include "Btn.h"
#include "MVModel.h"

extern HFONT g_hf9AcuBold;
extern HFONT g_hf14AcuMed;

// CMenuBtn

class CMenuBtn : public CBtn
{
	DECLARE_DYNAMIC(CMenuBtn)

public:
	CMenuBtn(BTN btn,COLORREF cr);
	virtual ~CMenuBtn();

	bool IsDepressed( void );
	bool Depress(bool bDown);
	void DrawDirectUp(void);
	virtual void SetText(TCHAR* pszText,TCHAR* pszTextDown,int nNbr=-1);
	void RefreshText(TCHAR* pszText,TCHAR* pszTextDown,int nNbr=-1);
	void SetText(CStringW& pszText,CStringW& pszTextDown,int nNbr=-1);
	void RefreshText(CStringW& pszText,CStringW& pszTextDown,int nNbr=-1);
	//inline void SetFocusDownBmp(CBmp* pcBmpFocusDown){m_pcBmpFocusDown=pcBmpFocusDown;};

	int GetBtnId();

protected:
	bool isSafeTickCountDelayExpired(DWORD oldTickCount, UINT delay=0);//used for (dwLastTickCount+DELAY<getTickCount64())

	void Draw(int nState,int AddInfo=0);
	// Attributes
private:
	DWORD m_dwLastMenudwn;

	bool m_bDepressed;
	bool m_bLMouseButtonDown;
	bool m_bLButtonDown;
	CBmp* m_pcBmpFocusDown;

	
protected:
	TCHAR m_pszTextDown[MAX_PATH];
	//CMVViewHandler* m_pViewHandler;
	// Operations
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMenuBtn)
public:
	//	virtual BOOL Create(CWnd* pParentWnd, HFONT hFont);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	// Implementation
public:

	// Generated message map functions
protected:
	//{{AFX_MSG(CMenuBtn)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnSetFocus(CWnd* pOldWnd);
};


