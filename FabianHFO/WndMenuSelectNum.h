#pragma once
#include "PushBtn.h"
#include "MenuBtn.h"
#include "MVView.h"
// CWndMenuSelectNum

class CWndMenuSelectNum : public CWnd
{
	DECLARE_DYNAMIC(CWndMenuSelectNum)

public:
	CWndMenuSelectNum(CMVView *parentView);
	virtual ~CWndMenuSelectNum();

	void Init();
	void Show(BOOL bShow);


	virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	void SetOneButtonDepressed(int btnID);

protected:
	CMVModel *getModel();

	void ShowMenuBtn();
	void HideMenuBtn();


	void SetAllButtonUnpressed();

	void BnClickedNumeric();


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

	/*CMenuBtn* m_pcMenu1;
	CMenuBtn* m_pcMenu2;
	CMenuBtn* m_pcMenu3;
	CMenuBtn* m_pcMenu4;
	CMenuBtn* m_pcMenu5;*/

	CPushBtn* m_pcMenuNum;

	CBmp* m_pcMenu_Up;
	CBmp* m_pcMenu_Dw;
	//CBmp* m_pcMenu_Fc;

	CBmp* m_pcNumeric_Up;
	CBmp* m_pcNumeric_Dw;
	//CBmp* m_pcNumeric_Fc;


	//CList<CMenuBtn* , CMenuBtn *> m_plMenuBtn;

	CMVModel *m_pModel;
protected:
	//{{AFX_MSG(CWndMenuSelectNum)
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	/*afx_msg void OnBnClickedMenu1();
	afx_msg void OnBnClickedMenu2();
	afx_msg void OnBnClickedMenu3();
	afx_msg void OnBnClickedMenu4();
	afx_msg void OnBnClickedMenu5();*/
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};




