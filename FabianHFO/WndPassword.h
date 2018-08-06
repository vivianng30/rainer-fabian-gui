/**********************************************************************************************//**
 * \file	WndPassword.h.
 *
 * Declares the window password class
 **************************************************************************************************/

#pragma once
#include "afxwin.h"
#include "KbrdBtn.h"
#include "PushBtn.h"
#include "globDefs.h"
#include "MVModel.h"
#include "colour.h"

class CWndPassword : public CWnd
{
	DECLARE_DYNAMIC(CWndPassword)

public:
	CWndPassword();
	virtual ~CWndPassword();

	virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);
	//bool CreateWnd(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	void Show(bool bShow);



protected:
	void Draw();
	void Key(int c);
	void ClearInfo();

	void BnClickedOk();
	void BnClickedCancel();

	CMVModel *getModel();

private:
	CMVModel* m_pModel;

	CStringW m_szPSSW;
	CStringW m_szInfo;

	bool m_bInfoClean;


	// Attributes
	HDC			m_hDC;
	HBITMAP		m_hBmp;
	HBITMAP		m_hBmpPrev;
	LONG		m_lX;
	LONG		m_lY;
	LONG		m_lXo;
	LONG		m_lYo;

	CBmp* m_pc_Up;
	CBmp* m_pc_Dw;

	CBmp* m_pcKbrdAlphanum_Up;
	CBmp* m_pcKbrdAlphanum_Dw;
	CBmp* m_pcKbrdDel_Up;
	CBmp* m_pcKbrdDel_Dw;

	CEdit* m_pEditPwd;

	CPushBtn* m_pcOk;
	CPushBtn* m_pcCancel;

	CKbrdBtn* m_pcKbrd_Del;

	CKbrdBtn* m_pcKbrd_1;
	CKbrdBtn* m_pcKbrd_2;
	CKbrdBtn* m_pcKbrd_3;
	CKbrdBtn* m_pcKbrd_4;
	CKbrdBtn* m_pcKbrd_5;
	CKbrdBtn* m_pcKbrd_6;
	CKbrdBtn* m_pcKbrd_7;
	CKbrdBtn* m_pcKbrd_8;
	CKbrdBtn* m_pcKbrd_9;
	CKbrdBtn* m_pcKbrd_0;

	CKbrdBtn* m_pcKbrd_a;
	CKbrdBtn* m_pcKbrd_b;
	CKbrdBtn* m_pcKbrd_c;
	CKbrdBtn* m_pcKbrd_d;
	CKbrdBtn* m_pcKbrd_e;
	CKbrdBtn* m_pcKbrd_f;



protected:
	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnPaint();
	afx_msg void OnDestroy();
	//afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedDel();
	afx_msg void OnBnClicked_1();
	afx_msg void OnBnClicked_2();
	afx_msg void OnBnClicked_3();
	afx_msg void OnBnClicked_4();
	afx_msg void OnBnClicked_5();
	afx_msg void OnBnClicked_6();
	afx_msg void OnBnClicked_7();
	afx_msg void OnBnClicked_8();
	afx_msg void OnBnClicked_9();
	afx_msg void OnBnClicked_0();
	afx_msg void OnBnClicked_a();
	afx_msg void OnBnClicked_b();
	afx_msg void OnBnClicked_c();
	afx_msg void OnBnClicked_d();
	afx_msg void OnBnClicked_e();
	afx_msg void OnBnClicked_f();
};



