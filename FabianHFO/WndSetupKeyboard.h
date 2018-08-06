/**********************************************************************************************//**
 * \file	WndSetupKeyboard.h.
 *
 * Declares the window setup keyboard class
 **************************************************************************************************/

#pragma once
#include "KbrdBtn.h"
#include "PushBtn.h"
#include "UpDwnBtn.h"
#include "globDefs.h"
#include "MVModel.h"
#include "colour.h"

class CWndSetupKeyboard : public CWnd
{
	DECLARE_DYNAMIC(CWndSetupKeyboard)

public:
	CWndSetupKeyboard();
	virtual ~CWndSetupKeyboard();

	virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	void Show(bool bShow);

	void SetEditWnd(CWnd* rEdit, eKeyRange range, int iID);


protected:
	//void Draw();
	void Key(int c);

	void RedrawButtons();

	void BnClickedFocusLeft();
	void BnClickedFocusRight();
	void BnClickedFocusUp();
	void BnClickedFocusDown();

	CMVModel *getModel();

private:
	CMVModel* m_pModel;

	CWnd* m_pEdit;

	int m_iID;

	eKeyRange m_eKeyRange;

	// Attributes
	HDC			m_hDC;
	HBITMAP		m_hBmp;
	HBITMAP		m_hBmpPrev;
	LONG		m_lX;
	LONG		m_lY;
	LONG		m_lXo;
	LONG		m_lYo;

	CBmp* m_pcKbrdAlphanum_Up;
	CBmp* m_pcKbrdAlphanum_Dw;
	CBmp* m_pcKbrdSpace_Up;
	CBmp* m_pcKbrdSpace_Dw;
	CBmp* m_pcKbrdDel_Up;
	CBmp* m_pcKbrdDel_Dw;
	CBmp* m_pcKbrdShift_Up;
	CBmp* m_pcKbrdShift_Dw;

	CKbrdBtn* m_pcKbrd_Del;
	CKbrdBtn* m_pcKbrd_Space;
	CUpDwnBtn* m_pcKbrd_Shift;

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
	CKbrdBtn* m_pcKbrd_sz;

	CKbrdBtn* m_pcKbrd_q;
	CKbrdBtn* m_pcKbrd_w;
	CKbrdBtn* m_pcKbrd_e;
	CKbrdBtn* m_pcKbrd_r;
	CKbrdBtn* m_pcKbrd_t;
	CKbrdBtn* m_pcKbrd_z;
	CKbrdBtn* m_pcKbrd_u;
	CKbrdBtn* m_pcKbrd_i;
	CKbrdBtn* m_pcKbrd_o;
	CKbrdBtn* m_pcKbrd_p;
	CKbrdBtn* m_pcKbrd_ue;
	//CBtn* m_pcKbrd_plus;

	CKbrdBtn* m_pcKbrd_a;
	CKbrdBtn* m_pcKbrd_s;
	CKbrdBtn* m_pcKbrd_d;
	CKbrdBtn* m_pcKbrd_f;
	CKbrdBtn* m_pcKbrd_g;
	CKbrdBtn* m_pcKbrd_h;
	CKbrdBtn* m_pcKbrd_j;
	CKbrdBtn* m_pcKbrd_k;
	CKbrdBtn* m_pcKbrd_l;
	CKbrdBtn* m_pcKbrd_oe;
	CKbrdBtn* m_pcKbrd_ae;
	//CBtn* m_pcKbrd_sharp;

	CKbrdBtn* m_pcKbrd_y;
	CKbrdBtn* m_pcKbrd_x;
	CKbrdBtn* m_pcKbrd_c;
	CKbrdBtn* m_pcKbrd_v;
	CKbrdBtn* m_pcKbrd_b;
	CKbrdBtn* m_pcKbrd_n;
	CKbrdBtn* m_pcKbrd_m;
	/*CBtn* m_pcKbrd_semicol;
	CBtn* m_pcKbrd_point;
	CBtn* m_pcKbrd_dash;*/

	CBmp* m_pc_Left_Up;
	CBmp* m_pc_Left_Dw;
	CBmp* m_pc_Right_Up;
	CBmp* m_pc_Right_Dw;
	CBmp* m_pc_Up_Up;
	CBmp* m_pc_Up_Dw;
	CBmp* m_pc_Down_Up;
	CBmp* m_pc_Down_dw;


	CPushBtn* m_pcFocusLeft;
	CPushBtn* m_pcFocusRight;
	CPushBtn* m_pcFocusUp;
	CPushBtn* m_pcFocusDown;

	//CStringList* m_pcsLangNames;


protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	//afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedDel();
	afx_msg void OnBnClickedSpace();
	afx_msg void OnBnClickedShift();
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
	afx_msg void OnBnClicked_sz();
	afx_msg void OnBnClicked_q();
	afx_msg void OnBnClicked_w();
	afx_msg void OnBnClicked_e();
	afx_msg void OnBnClicked_r();
	afx_msg void OnBnClicked_t();
	afx_msg void OnBnClicked_z();
	afx_msg void OnBnClicked_u();
	afx_msg void OnBnClicked_i();
	afx_msg void OnBnClicked_o();
	afx_msg void OnBnClicked_p();
	afx_msg void OnBnClicked_ue();
	afx_msg void OnBnClicked_a();
	afx_msg void OnBnClicked_s();
	afx_msg void OnBnClicked_d();
	afx_msg void OnBnClicked_f();
	afx_msg void OnBnClicked_g();
	afx_msg void OnBnClicked_h();
	afx_msg void OnBnClicked_j();
	afx_msg void OnBnClicked_k();
	afx_msg void OnBnClicked_l();
	afx_msg void OnBnClicked_oe();
	afx_msg void OnBnClicked_ae();
	afx_msg void OnBnClicked_y();
	afx_msg void OnBnClicked_x();
	afx_msg void OnBnClicked_c();
	afx_msg void OnBnClicked_v();
	afx_msg void OnBnClicked_b();
	afx_msg void OnBnClicked_n();
	afx_msg void OnBnClicked_m();
};



