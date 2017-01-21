#pragma once
#include "MVModel.h"
#include "afxwin.h"

// CPasswordDlg dialog

class CPasswordDlg : public CDialog
{
	DECLARE_DYNAMIC(CPasswordDlg)

public:
	CPasswordDlg(CWnd* pParent = NULL,eModule module=MOD_NONE);   // standard constructor
	virtual ~CPasswordDlg();

// Dialog Data
	enum { IDD = IDD_PASSWRD_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	TCHAR m_pszFontName[32];
	HFONT CreateFontHandle(CDC* pDC, int nPixHeight, TCHAR* pszFacename, LONG lFontWeight, int iOrientation);

	void Key(int c);

	CMVModel *getModel();

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
protected:
	eModule m_eModule;
	CMVModel* m_pModel;

	//CBrush m_brush;
	HFONT m_hfont;
	HFONT m_hfontButton;

	
	afx_msg void OnBnClickedCancel();
	
	CButton m_bnOk;
	CButton m_bnCancel;
	CButton m_bnHex0;
	CButton m_bnHex1;
	CButton m_bnHex2;
	CButton m_bnHex3;
	CButton m_bnHex4;
	CButton m_bnHex5;
	CButton m_bnHex6;
	CButton m_bnHex7;
	CButton m_bnHex8;
	CButton m_bnHex9;
	CButton m_bnHexA;
	CButton m_bnHexB;
	CButton m_bnHexC;
	CButton m_bnHexD;
	CButton m_bnHexE;
	CButton m_bnHexF;
	CButton m_bnDel;

	CEdit m_ebHexString1;
	CEdit m_ebHexString2;
	CEdit m_ebHexString3;
	CEdit m_ebHexString4;
	CEdit m_ebHexString5;

	CEdit *m_ebpFocused;

	//afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedOk();
	CString m_szEdit1;
	CString m_szEdit2;
	CString m_szEdit3;
	CString m_szEdit4;
	CString m_szEdit5;
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
	afx_msg void OnBnClickedButtona();
	afx_msg void OnBnClickedButtonb();
	afx_msg void OnBnClickedButtonc();
	afx_msg void OnBnClickedButtond();
	afx_msg void OnBnClickedButtone();
	afx_msg void OnBnClickedButtonf();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnEnSetfocusEdit1();
	afx_msg void OnEnSetfocusEdit2();
	afx_msg void OnEnSetfocusEdit3();
	afx_msg void OnEnSetfocusEdit4();
	afx_msg void OnEnSetfocusEdit5();
	/*afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);*/
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	CStatic m_InfoText;
	CStatic m_labelDevID;
};
