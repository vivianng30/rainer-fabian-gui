#pragma once
#include "afxwin.h"
#include "MVModel.h"
#include "KbrdBtn.h"

// CDlgMessageBox dialog

class CDlgMessageBox : public CDialog
{
	DECLARE_DYNAMIC(CDlgMessageBox)

public:
	CDlgMessageBox(CWnd* pParent = NULL, CStringW sCaption = _T(""), CStringW sText = _T(""), UINT iFlags = 0, UINT iRESSOURCE = 0, UINT iHelpID = 0);   // standard constructor
	virtual ~CDlgMessageBox();

protected:
	
// Dialog Data
	enum { IDD = IDD_TLSMESSAGEBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	
	CMVModel *getModel();

	void Draw();

	DECLARE_MESSAGE_MAP()
public:
	
private:
	CMVModel* m_pModel;

	CKbrdBtn* m_pcYes;
	CKbrdBtn* m_pcNo;
	CKbrdBtn* m_pcOk;
	CKbrdBtn* m_pcCancel;

	CStringW m_szTextMsg;
	CStringW m_szCaption;	

	UINT m_iFlags;
	UINT m_iHelpID;

	CBmp* m_pcBtnUp;
	CBmp* m_pcBtnDwn;
	CBmp* m_pcBtnDis;

	CBmp* m_pcDlgLeft;
	CBmp* m_pcDlgLeftTop;
	CBmp* m_pcDlgLeftBot;
	CBmp* m_pcDlgRight;
	CBmp* m_pcDlgRightTop;
	CBmp* m_pcDlgRightBot;
	CBmp* m_pcDlgTop;
	CBmp* m_pcDlgBot;

	UINT m_iRessource;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	//afx_msg void OnBnClickedRetry();
	afx_msg void OnBnClickedYes();
	afx_msg void OnBnClickedNo();
	CStatic m_pcRessource;
	afx_msg void OnPaint();
};
