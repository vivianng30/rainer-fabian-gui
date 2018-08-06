//=============================================================================
/** 
* \class CDlgProxPressure60
* 		 
* \brief Dialog for proximal pressure calibration.
* 		 
* \author Rainer Kuehner
* \date 2013-01-21 Creation of file
*
**/
//=============================================================================
#pragma once
#include "afxwin.h"
#include "MVModel.h"
#include "KbrdBtn.h"
#include "SelectSetupBtn.h"
#include "DTUpDwnBtn.h"
#include "UpDwnBtn.h"

// CDlgProxPressure60 dialog

class CDlgProxPressure60 : public CDialog
{
	DECLARE_DYNAMIC(CDlgProxPressure60)

public:
	CDlgProxPressure60(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgProxPressure60();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

// Dialog Data
	enum { IDD = IDD_DLGPROXP60CAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	HFONT CreateFontHandle(CDC* pDC, int nPixHeight, TCHAR* pszFacename, LONG lFontWeight, int iOrientation);
	CMVModel *getModel();

	void Draw();
	void setProcPressureCal60();

	DECLARE_MESSAGE_MAP()

private:
	CMVModel* m_pModel;
	TCHAR m_pszFontName[32];
	CKbrdBtn* m_pcSave;
	//CKbrdBtn* m_pcRunBreath;
	CKbrdBtn* m_pcCancel;

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

	CStringW m_szCaption;

	CBrush m_brush;
	HFONT m_hfont;
	//HFONT m_hfontButton;

	int m_iFlags;
	int m_iPressure;
	int m_iPProxADC;

	CBmp* m_pcUp_Up;
	CBmp* m_pcUp_Dw;
	CBmp* m_pcDw_Up;
	CBmp* m_pcDw_Dw;

	CBmp* m_pcStatic_Up;
	CBmp* m_pcStatic_Dw;
	CBmp* m_pcStatic_Fc;

	CSelectSetupBtn* m_pbtnValue;

	CDTUpDwnBtn* m_pcNextUp;
	CDTUpDwnBtn* m_pcNextDwn;

	CUpDwnBtn* m_pcPara_Select;
	CBmp* m_pcPara_Select_yes;
	CBmp* m_pcPara_Select_no;

	int m_iLowerLimit;
	int m_iUpperLimit;
	int m_iPressureDiff;
	int m_iCounter;
	eTimeChanger m_eTimeChanger;
	DWORD m_dwLastSetupTimer;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedValue();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedCancel();
	//afx_msg void OnBnClickedRunBreath();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CStatic m_szPressure;
	CStatic m_szPProxADC;
};
