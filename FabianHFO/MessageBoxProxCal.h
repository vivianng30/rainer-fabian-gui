/**********************************************************************************************//**
 * \class	CMessageBoxProxCal
 *
 * \brief	Message box for Proxymal Pressure Calibration.
 * 			
 * \author	Rainer Kuehner
 * \date 2017-10-23 Creation of file
 * 			
 **************************************************************************************************/
#pragma once
#include "afxwin.h"
#include "MVModel.h"
//#include "globdefs.h"

//flags for Messagebox
#define	MB_CAL_0MBAR	0
#define	MB_CAL_60MBAR	1

// CMessageBoxProxCal dialog

class CMessageBoxProxCal : public CDialog
{
	DECLARE_DYNAMIC(CMessageBoxProxCal)

public:
	CMessageBoxProxCal(CWnd* pParent = NULL, CStringW sCaption = _T(""), CStringW sText = _T(""), UINT iFlags = 0);   // standard constructor
	virtual ~CMessageBoxProxCal();

	HFONT CreateFontHandle(CDC* pDC, int nPixHeight, TCHAR* pszFacename, LONG lFontWeight, int iOrientation);
private:
	CMVModel *getModel();

// Dialog Data
	enum { IDD = IDD_OXYCALMSG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
protected:
	TCHAR m_pszFontName[32];
	CMVModel* m_pModel;

	CButton m_btn0mbar;
	CButton m_btn60mbar;
	CButton m_btnCancel;
	CStatic m_szStaticTextMsg;
	CStringW m_szTextMsg;
	CStringW m_szCaption;

	CBrush m_brush;
	HFONT m_hfont;
	HFONT m_hfontButton;

	int m_iFlags;
	int m_iPressure;
	int m_iPProxADC;


public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnOxy0();
	afx_msg void OnBnClickedBtnOxy60();
	afx_msg void OnBnClickedCancel();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CStatic m_pcRessource;
	CStatic m_szPProxADC;
	CStatic m_szPressure;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
