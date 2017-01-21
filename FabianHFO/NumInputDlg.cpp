// NumInputDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "NumInputDlg.h"

#define FOCUSTIMER			WM_USER + 0x6000

// CNumInputDlg dialog

IMPLEMENT_DYNAMIC(CNumInputDlg, CDialog)

CNumInputDlg::CNumInputDlg(CWnd* pParent /*=NULL*/,eOpTime opTime, UINT& inputNum)
	: CDialog(CNumInputDlg::IDD, pParent)
	, m_szEdit1(_T(""))
	, m_pInputNum(inputNum)
{
	m_pModel=NULL;
	m_ebpFocused=NULL;
	m_opTime=opTime;
	//m_pInputNum=inputNum;

	m_pszFontName[0]=0x0000;
	m_hfont=NULL;
	m_hfontButton=NULL;
}

CNumInputDlg::~CNumInputDlg()
{
	DeleteObject(m_hfont);
	DeleteObject(m_hfontButton);
}

void CNumInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON0, m_bn0);
	DDX_Control(pDX, IDC_BUTTON1, m_bn1);
	DDX_Control(pDX, IDC_BUTTON2, m_bn2);
	DDX_Control(pDX, IDC_BUTTON3, m_bn3);
	DDX_Control(pDX, IDC_BUTTON4, m_bn4);
	DDX_Control(pDX, IDC_BUTTON5, m_bn5);
	DDX_Control(pDX, IDC_BUTTON6, m_bn6);
	DDX_Control(pDX, IDC_BUTTON7, m_bn7);
	DDX_Control(pDX, IDC_BUTTON8, m_bn8);
	DDX_Control(pDX, IDC_BUTTON9, m_bn9);
	DDX_Control(pDX, IDC_BUTTON_DEL, m_bnDel);
	DDX_Control(pDX, IDOK, m_bnOk);
	DDX_Control(pDX, IDCANCEL, m_bnCancel);
	DDX_Control(pDX, IDC_STATICPWDLG, m_InfoText);
	DDX_Control(pDX, IDC_EDIT1, m_ebString1);
	DDX_Text(pDX, IDC_EDIT1, m_szEdit1);
}


BEGIN_MESSAGE_MAP(CNumInputDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON0, &CNumInputDlg::OnBnClickedButton0)
	ON_BN_CLICKED(IDC_BUTTON1, &CNumInputDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CNumInputDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CNumInputDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CNumInputDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CNumInputDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CNumInputDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CNumInputDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CNumInputDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CNumInputDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CNumInputDlg::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDOK, &CNumInputDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CNumInputDlg::OnBnClickedCancel)
	ON_EN_SETFOCUS(IDC_EDIT1, &CNumInputDlg::OnEnSetfocusEdit1)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CNumInputDlg message handlers
BOOL CNumInputDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//MB_ABORTRETRYIGNORE   The message box contains three pushbuttons: Abort, Retry, and Ignore. 
	//MB_OK   The message box contains one pushbutton: OK. 
	//MB_OKCANCEL   The message box contains two pushbuttons: OK and Cancel. 
	//MB_RETRYCANCEL   The message box contains two pushbuttons: Retry and Cancel. 
	//MB_YESNO   The message box contains two pushbuttons: Yes and No. 
	//MB_YESNOCANCEL   The message box contains three pushbuttons: Yes, No, and Cancel. 

	SetWindowPos(&this->wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

	 //m_brush.CreateSolidBrush(BACKGND);

	//CClientDC dc(this);

	CRect rcDlg, rcRes;
	GetClientRect(rcDlg);

	CDC* pDC = this->GetDC();


	_tcscpy_s(m_pszFontName,_countof(m_pszFontName),getModel()->GetFontFace());
	m_hfont=CreateFontHandle(pDC,16,m_pszFontName,FW_BOLD,0);
	m_hfontButton=CreateFontHandle(pDC,18,m_pszFontName,FW_BOLD,0);

	CString szCaption=_T("Input panel: ");
	switch(m_opTime)
	{
	case OPTIME_DEVICE:
		{
			szCaption=_T("Operating time device");
		}
		break;
	case OPTIME_BATTERY:
		{
			szCaption=_T("Operating time battery");
		}
		break;
	case OPTIME_HFO:
		{
			szCaption=_T("Operating time HFO module");
		}
		break;
	default:
		{
			szCaption=_T("Input panel:");
		}
		break;
	}
	CString szInfo=_T("Opereating time in minutes!");

	this->SetWindowText(szCaption);

	m_InfoText.SetFont(CFont::FromHandle(m_hfontButton));
	m_InfoText.SetWindowText(szInfo);

	
	m_bnOk.SetFont(CFont::FromHandle(m_hfontButton));
	m_bnCancel.SetFont(CFont::FromHandle(m_hfontButton));
	m_bn0.SetFont(CFont::FromHandle(m_hfontButton));
	m_bn1.SetFont(CFont::FromHandle(m_hfontButton));
	m_bn2.SetFont(CFont::FromHandle(m_hfontButton));
	m_bn3.SetFont(CFont::FromHandle(m_hfontButton));
	m_bn4.SetFont(CFont::FromHandle(m_hfontButton));
	m_bn5.SetFont(CFont::FromHandle(m_hfontButton));
	m_bn6.SetFont(CFont::FromHandle(m_hfontButton));
	m_bn7.SetFont(CFont::FromHandle(m_hfontButton));
	m_bn8.SetFont(CFont::FromHandle(m_hfontButton));
	m_bn9.SetFont(CFont::FromHandle(m_hfontButton));
	m_bnDel.SetFont(CFont::FromHandle(m_hfontButton));

	/*UINT iNum=m_pInputNum;
	m_szEdit1.Format(_T("%d"),iNum);*/
	m_ebString1.SetFont(CFont::FromHandle(m_hfontButton));
	m_ebString1.SetWindowText(m_szEdit1);
	
	SetTimer(FOCUSTIMER, 100, NULL);

	ReleaseDC(pDC);

	return TRUE;   // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
// CNumInputDlg message handlers
// **************************************************************************
// 
// **************************************************************************
CMVModel *CNumInputDlg::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}


//LRESULT CNumInputDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
//{
//	if (message == WM_SYSCOMMAND &&  ((wParam & SC_SIZE) == SC_SIZE || (wParam & SC_MOVE) == SC_MOVE)    ) 
//	{
//		return TRUE; // Ignore if move or size
//	}
//	return CWnd::WindowProc(message, wParam, lParam);
////}
//BOOL CNumInputDlg::PreTranslateMessage(MSG* pMsg) 
//{
//	switch(pMsg->message)
//	{
//	case WM_MOUSEMOVE:
//		{
//			return 1;
//		}
//		break;
//	default:
//		break;
//	}
//	return CWnd::PreTranslateMessage(pMsg);
//}
// **************************************************************************
// 
// **************************************************************************
HFONT CNumInputDlg::CreateFontHandle(CDC* pDC, int nPixHeight, TCHAR* pszFacename, LONG lFontWeight, int iOrientation) 
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
void CNumInputDlg::OnBnClickedButton0()
{
	Key('0');
}

void CNumInputDlg::OnBnClickedButton1()
{
	Key('1');
}

void CNumInputDlg::OnBnClickedButton2()
{
	Key('2');
}

void CNumInputDlg::OnBnClickedButton3()
{
	Key('3');
}

void CNumInputDlg::OnBnClickedButton4()
{
	Key('4');
}

void CNumInputDlg::OnBnClickedButton5()
{
	Key('5');
}

void CNumInputDlg::OnBnClickedButton6()
{
	Key('6');
}

void CNumInputDlg::OnBnClickedButton7()
{
	Key('7');
}

void CNumInputDlg::OnBnClickedButton8()
{
	Key('8');
}

void CNumInputDlg::OnBnClickedButton9()
{
	Key('9');
}

void CNumInputDlg::OnBnClickedButtonDel()
{
	Key(-1);
}

void CNumInputDlg::OnBnClickedOk()
{
	m_ebString1.GetWindowText(m_szEdit1);
	//CString szInputKey=m_szEdit1;
	UINT iNum=_wtoi(m_szEdit1);
	m_pInputNum=iNum;
	//GetParent()->PostMessage(WM_INPUT_NUMBER,iNum);
	OnOK();

}

void CNumInputDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CNumInputDlg::Key(int c)
{
	if(c == -1)
	{
		if(m_ebpFocused)
			m_ebpFocused->SendMessage(WM_CHAR, VK_BACK, 1);
	}
	else
	{
		if(m_ebpFocused)
			m_ebpFocused->SendMessage(WM_CHAR, c, 1);
	}
	//ClearInfo();
	if(m_ebpFocused)
	{
		m_ebpFocused->SetFocus();
	}

}
void CNumInputDlg::OnEnSetfocusEdit1()
{
	m_ebpFocused=&m_ebString1;
}
void CNumInputDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==FOCUSTIMER)
	{
		KillTimer(FOCUSTIMER);
		m_ebString1.SetFocus();
		/*DWORD dwSel = m_ebString1.GetSel();
		m_ebString1.SetSel(HIWORD(dwSel)-1, HIWORD(dwSel)-1,true);*/
		//m_ebString1.SetSel(1, 1,true);
	}

	CDialog::OnTimer(nIDEvent);
}

//************************************
// Method:    OnDestroy
// FullName:  CNumInputDlg::OnDestroy
// Access:    protected 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CNumInputDlg::OnDestroy()
{
	KillTimer(FOCUSTIMER);

	CDialog::OnDestroy();
}
