// MessageBoxOxyCal.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "globdefs.h"
#include "MessageBoxProxCal.h"
#include "colour.h"




// CMessageBoxOxyCal dialog

IMPLEMENT_DYNAMIC(CMessageBoxProxCal, CDialog)

CMessageBoxProxCal::CMessageBoxProxCal(CWnd* pParent /*=NULL*/,CStringW sCaption, CStringW sText, UINT iFlags)
	: CDialog(CMessageBoxProxCal::IDD, pParent)
{
	m_pModel=NULL;

	m_pszFontName[0]=0x0000;
	m_hfont=NULL;
	m_hfontButton=NULL;
	m_iFlags=iFlags;
	m_szTextMsg=sText;
	m_szCaption = sCaption;

	m_iPressure=0;
	m_iPProxADC=0;
}

CMessageBoxProxCal::~CMessageBoxProxCal()
{
	DeleteObject(m_brush);
	DeleteObject(m_hfont);
	DeleteObject(m_hfontButton);
}

// **************************************************************************
// 
// **************************************************************************
CMVModel *CMessageBoxProxCal::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}

void CMessageBoxProxCal::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_OXY0, m_btn0mbar);
	DDX_Control(pDX, IDC_BTN_OXY60, m_btn60mbar);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_TEXTMSG, m_szStaticTextMsg);
	DDX_Control(pDX, IDC_CALPIC, m_pcRessource);
	DDX_Control(pDX, IDC_PROXCAL_PROXADC, m_szPProxADC);
	DDX_Control(pDX, IDC_PROXCAL_PRESSURE, m_szPressure);
}


BEGIN_MESSAGE_MAP(CMessageBoxProxCal, CDialog)
	ON_BN_CLICKED(IDC_BTN_OXY0, &CMessageBoxProxCal::OnBnClickedBtnOxy0)
	ON_BN_CLICKED(IDC_BTN_OXY60, &CMessageBoxProxCal::OnBnClickedBtnOxy60)
	ON_BN_CLICKED(IDCANCEL, &CMessageBoxProxCal::OnBnClickedCancel)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMessageBoxProxCal message handlers
BOOL CMessageBoxProxCal::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetWindowPos(&this->wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

	m_brush.CreateSolidBrush(BACKGND);

	CClientDC dc(this);

	CRect rcDlg, rcRes;
	GetClientRect(rcDlg);

	_tcscpy_s(m_pszFontName,_countof(m_pszFontName),getModel()->GetFontFace());

	CDC* pDC = this->GetDC();
	m_hfont=CreateFontHandle(pDC,16,m_pszFontName,FW_BOLD,0);
	m_hfontButton=CreateFontHandle(pDC,18,m_pszFontName,FW_BOLD,0);

	m_szStaticTextMsg.SetFont(CFont::FromHandle(m_hfont));

	m_btn0mbar.SetFont(CFont::FromHandle(m_hfontButton));
	m_btn60mbar.SetFont(CFont::FromHandle(m_hfontButton));
	m_btnCancel.SetFont(CFont::FromHandle(m_hfontButton));

	m_szPProxADC.SetFont(CFont::FromHandle(m_hfont));
	m_szPressure.SetFont(CFont::FromHandle(m_hfont));

	
	ReleaseDC(pDC);

	switch(m_iFlags)
	{
	case MB_CAL_0MBAR:
		{
			m_pcRessource.SetBitmap(::LoadBitmap(NULL, MAKEINTRESOURCE(IDB_MSG_CAL0MBAR)));//(AfxGetInstanceHandle(),(LPCTSTR) m_iRessource));
			m_pcRessource.GetClientRect(rcRes);
			m_pcRessource.MoveWindow(((rcDlg.right-rcRes.right)/2),10,rcRes.right,rcRes.bottom);

			//m_btn0mbar.GetDC()->SetTextColor(RGB(0,255,0));
			m_btn0mbar.SetWindowText(_T("0 mbar"));
			m_btn0mbar.EnableWindow(true);
			m_btn60mbar.SetWindowText(_T("60 mbar"));
			m_btn60mbar.EnableWindow(false);
		}
		break;
	case MB_CAL_60MBAR:
		{
			m_pcRessource.SetBitmap(::LoadBitmap(NULL, MAKEINTRESOURCE(IDB_MSG_CAL60MBAR)));//(AfxGetInstanceHandle(),(LPCTSTR) m_iRessource));
			m_pcRessource.GetClientRect(rcRes);
			m_pcRessource.MoveWindow(((rcDlg.right-rcRes.right)/2),10,rcRes.right,rcRes.bottom);

			m_btn0mbar.SetWindowText(_T("0 mbar"));
			m_btn0mbar.EnableWindow(false);
			m_btn60mbar.SetWindowText(_T("60 mbar"));
			m_btn60mbar.EnableWindow(true);
		}
		break;
		}

	m_szPProxADC.SetWindowText(_T("PPROX ADC:"));
	m_szPressure.SetWindowText(_T("Pressure:"));

	m_szStaticTextMsg.SetWindowText(m_szTextMsg);
	this->SetWindowText(m_szCaption);

	m_btn60mbar.ShowWindow(SW_HIDE);

	SetTimer(PROXPRESSURECALTIMER, 250, NULL);

	return TRUE;   // return TRUE unless you set the focus to a control
}

void CMessageBoxProxCal::OnBnClickedBtnOxy0()
{
	KillTimer(PROXPRESSURECALTIMER);
	OnOK();
}

void CMessageBoxProxCal::OnBnClickedBtnOxy60()
{
	KillTimer(PROXPRESSURECALTIMER);
	OnOK();
}

void CMessageBoxProxCal::OnBnClickedCancel()
{
	KillTimer(PROXPRESSURECALTIMER);
	OnCancel();
}

HBRUSH CMessageBoxProxCal::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (nCtlColor) 
	{ 
	case CTLCOLOR_STATIC:  // Für alle Statics           
		pDC->SetBkColor(BACKGND); 

		//case CTLCOLOR_MSGBOX:     // Für den Dialog             
		//	return (HBRUSH)m_brush;
		//case CTLCOLOR_DLG:     // Für den Dialog             
		//	return (HBRUSH)m_brush; 

	default: 
		hbr= m_brush;
	} 

	return hbr;
}

// **************************************************************************
// 
// **************************************************************************
HFONT CMessageBoxProxCal::CreateFontHandle(CDC* pDC, int nPixHeight, TCHAR* pszFacename, LONG lFontWeight, int iOrientation) 
{
	// Standard settings
	LOGFONT lf;
	lf.lfWidth = 0; 
	lf.lfEscapement = iOrientation; 
	lf.lfOrientation = iOrientation; 
	lf.lfItalic = FALSE; 
	lf.lfUnderline = FALSE; 
	lf.lfStrikeOut = FALSE; 
	lf.lfCharSet = DEFAULT_CHARSET; 
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS; 
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS; 
	//lf.lfQuality = DEFAULT_QUALITY; 
	lf.lfQuality = ANTIALIASED_QUALITY; 
	//lf.lfQuality = CLEARTYPE_QUALITY;
	lf.lfPitchAndFamily = FF_ROMAN;
	// To modify
	lf.lfWeight=lFontWeight;
	_tcscpy_s(lf.lfFaceName,_countof(lf.lfFaceName),pszFacename);
	//lf.lfHeight = -((nPixHeight*pDC->GetDeviceCaps(LOGPIXELSY))/72); // in Pixel; 
	lf.lfHeight = nPixHeight; // in Pixel;

	return CreateFontIndirect( &lf );
}
void CMessageBoxProxCal::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==PROXPRESSURECALTIMER)
	{
		m_iPressure=getModel()->getDATAHANDLER()->getMessureDataPRESSURE();
		m_iPProxADC=getModel()->getSPI()->Read_P_PROX_ADC();

		CStringW szText;
		szText.Format(_T("%s %d"), _T("PPROX ADC:"), m_iPProxADC);
		m_szPProxADC.SetWindowText(szText);
		m_szPProxADC.UpdateWindow();
		szText.Format(_T("%s %0.1f"), _T("Pressure:"), CTlsFloat::Round(((double)m_iPressure)/10, 1));
		m_szPressure.SetWindowText(szText);
		m_szPressure.UpdateWindow();
	}

	CDialog::OnTimer(nIDEvent);
}
