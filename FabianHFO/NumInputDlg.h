/**********************************************************************************************//**
 * \file	NumInputDlg.h.
 *
 * Declares the number input Dialog
 **************************************************************************************************/

#pragma once
#include "afxwin.h"
#include "MVModel.h"

// CNumInputDlg dialog

class CNumInputDlg : public CDialog
{
	DECLARE_DYNAMIC(CNumInputDlg)

public:
	CNumInputDlg(CWnd* pParent /*= NULL*/,eOpTime module, UINT& inputNum);   // standard constructor
	virtual ~CNumInputDlg();
	//virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//virtual BOOL PreTranslateMessage(MSG* pMsg);

// Dialog Data
	enum { IDD = IDD_NUMINPUT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	TCHAR m_pszFontName[32];
	HFONT CreateFontHandle(int nPixHeight, TCHAR* pszFacename, LONG lFontWeight, int iOrientation);

	void Key(int c);

	CMVModel *getModel();

	DECLARE_MESSAGE_MAP()

protected:
	CMVModel* m_pModel;
	HFONT m_hfont;
	HFONT m_hfontButton;
	eOpTime m_opTime;

	CButton m_bn0;
	CButton m_bn1;
	CButton m_bn2;
	CButton m_bn3;
	CButton m_bn4;
	CButton m_bn5;
	CButton m_bn6;
	CButton m_bn7;
	CButton m_bn8;
	CButton m_bn9;
	CButton m_bnDel;
	CButton m_bnOk;
	CButton m_bnCancel;
	CStatic m_InfoText;
	CEdit m_ebString1;
	CString m_szEdit1;

	CEdit *m_ebpFocused;

	UINT &m_pInputNum;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton0();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnEnSetfocusEdit1();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
};
