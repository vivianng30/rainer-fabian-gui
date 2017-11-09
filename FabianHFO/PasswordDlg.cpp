// PasswordDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "PasswordDlg.h"
#include "colour.h"

// CPasswordDlg dialog
#define FOCUSTIMER			WM_USER + 0x6000


IMPLEMENT_DYNAMIC(CPasswordDlg, CDialog)

CPasswordDlg::CPasswordDlg(CWnd* pParent /*=NULL*/,eModule module)
	: CDialog(CPasswordDlg::IDD, pParent)
	,m_eModule(module)
	, m_szEdit1(_T(""))
	, m_szEdit2(_T(""))
	, m_szEdit3(_T(""))
	, m_szEdit4(_T(""))
	, m_szEdit5(_T(""))
{
	m_pModel=NULL;
	m_ebpFocused=NULL;

	m_pszFontName[0]=0x0000;
	m_hfont=NULL;
	m_hfontButton=NULL;
}

CPasswordDlg::~CPasswordDlg()
{
	DeleteObject(m_hfont);
	DeleteObject(m_hfontButton);
}

void CPasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_bnOk);
	DDX_Control(pDX, IDCANCEL, m_bnCancel);
	DDX_Control(pDX, IDC_BUTTON0, m_bnHex0);
	DDX_Control(pDX, IDC_BUTTON1, m_bnHex1);
	DDX_Control(pDX, IDC_BUTTON2, m_bnHex2);
	DDX_Control(pDX, IDC_BUTTON3, m_bnHex3);
	DDX_Control(pDX, IDC_BUTTON4, m_bnHex4);
	DDX_Control(pDX, IDC_BUTTON5, m_bnHex5);
	DDX_Control(pDX, IDC_BUTTON6, m_bnHex6);
	DDX_Control(pDX, IDC_BUTTON7, m_bnHex7);
	DDX_Control(pDX, IDC_BUTTON8, m_bnHex8);
	DDX_Control(pDX, IDC_BUTTON9, m_bnHex9);
	DDX_Control(pDX, IDC_BUTTONA, m_bnHexA);
	DDX_Control(pDX, IDC_BUTTONB, m_bnHexB);
	DDX_Control(pDX, IDC_BUTTONC, m_bnHexC);
	DDX_Control(pDX, IDC_BUTTOND, m_bnHexD);
	DDX_Control(pDX, IDC_BUTTONE, m_bnHexE);
	DDX_Control(pDX, IDC_BUTTONF, m_bnHexF);
	DDX_Control(pDX, IDC_BUTTON_DEL, m_bnDel);
	DDX_Control(pDX, IDC_EDIT1, m_ebHexString1);
	DDX_Control(pDX, IDC_EDIT2, m_ebHexString2);
	DDX_Control(pDX, IDC_EDIT3, m_ebHexString3);
	DDX_Control(pDX, IDC_EDIT4, m_ebHexString4);
	DDX_Control(pDX, IDC_EDIT5, m_ebHexString5);
	DDX_Text(pDX, IDC_EDIT1, m_szEdit1);
	DDV_MaxChars(pDX, m_szEdit1, 4);
	DDX_Text(pDX, IDC_EDIT2, m_szEdit2);
	DDV_MaxChars(pDX, m_szEdit2, 4);
	DDX_Text(pDX, IDC_EDIT3, m_szEdit3);
	DDV_MaxChars(pDX, m_szEdit3, 4);
	DDX_Text(pDX, IDC_EDIT4, m_szEdit4);
	DDV_MaxChars(pDX, m_szEdit4, 4);
	DDX_Text(pDX, IDC_EDIT5, m_szEdit5);
	DDV_MaxChars(pDX, m_szEdit5, 4);
	DDX_Control(pDX, IDC_STATICPWDLG, m_InfoText);
	DDX_Control(pDX, IDC_STATICPWDLG2, m_labelDevID);
}


BEGIN_MESSAGE_MAP(CPasswordDlg, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CPasswordDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CPasswordDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON0, &CPasswordDlg::OnBnClickedButton0)
	ON_BN_CLICKED(IDC_BUTTON1, &CPasswordDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPasswordDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CPasswordDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CPasswordDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CPasswordDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CPasswordDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CPasswordDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CPasswordDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CPasswordDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTONA, &CPasswordDlg::OnBnClickedButtona)
	ON_BN_CLICKED(IDC_BUTTONB, &CPasswordDlg::OnBnClickedButtonb)
	ON_BN_CLICKED(IDC_BUTTONC, &CPasswordDlg::OnBnClickedButtonc)
	ON_BN_CLICKED(IDC_BUTTOND, &CPasswordDlg::OnBnClickedButtond)
	ON_BN_CLICKED(IDC_BUTTONE, &CPasswordDlg::OnBnClickedButtone)
	ON_BN_CLICKED(IDC_BUTTONF, &CPasswordDlg::OnBnClickedButtonf)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CPasswordDlg::OnBnClickedButtonDel)
	ON_EN_SETFOCUS(IDC_EDIT1, &CPasswordDlg::OnEnSetfocusEdit1)
	ON_EN_SETFOCUS(IDC_EDIT2, &CPasswordDlg::OnEnSetfocusEdit2)
	ON_EN_SETFOCUS(IDC_EDIT3, &CPasswordDlg::OnEnSetfocusEdit3)
	ON_EN_SETFOCUS(IDC_EDIT4, &CPasswordDlg::OnEnSetfocusEdit4)
	ON_EN_SETFOCUS(IDC_EDIT5, &CPasswordDlg::OnEnSetfocusEdit5)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

BOOL CPasswordDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//MB_ABORTRETRYIGNORE   The message box contains three pushbuttons: Abort, Retry, and Ignore. 
	//MB_OK   The message box contains one pushbutton: OK. 
	//MB_OKCANCEL   The message box contains two pushbuttons: OK and Cancel. 
	//MB_RETRYCANCEL   The message box contains two pushbuttons: Retry and Cancel. 
	//MB_YESNO   The message box contains two pushbuttons: Yes and No. 
	//MB_YESNOCANCEL   The message box contains three pushbuttons: Yes, No, and Cancel. 

	SetWindowPos(&this->wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

	CRect rcDlg, rcRes;
	GetClientRect(rcDlg);

	CDC* pDC = this->GetDC();


	_tcscpy_s(m_pszFontName,_countof(m_pszFontName),getModel()->GetFontFace());
	m_hfont=CreateFontHandle(pDC,16,m_pszFontName,FW_BOLD,0);
	this->SetFont(CFont::FromHandle(m_hfont));
	m_hfontButton=CreateFontHandle(pDC,18,m_pszFontName,FW_BOLD,0);

	CString szCaption=_T("License key: ");
	CString szInfo=_T("Insert key: ");
	CString szMode=_T("");

	switch(m_eModule)
	{
	case MOD_MASTER:
		{
			szMode=_T("MASTER2");
		}
		break;
	case MOD_HFO:
		{
			szMode=_T("HFO");
		}
		break;
	/*case MOD_CO2:
		{
			szMode=_T("CO2");
		}
		break;*/
	case MOD_NMODE:
		{
			szMode=_T("NMode");
		}
		break;
	case MOD_VGUARANTY:
		{
			szMode=_T("Volume guaranty");
		}
		break;
	case MOD_VLIMIT:
		{
			szMode=_T("Volume limit");
		}
		break;
	case MOD_LUNGREC:
		{
			szMode=_T("Lung recruitment");
		}
		break;
	case MOD_TREND:
		{
			szMode=_T("Trend");
		}
		break;
	case MOD_THERAPY:
		{
			szMode=_T("O2 Therapy");
		}
		break;
	case MOD_PRICO:
		{
			szMode=_T("PRICO");
		}
		break;
	case MOD_FOT:
		{
			szMode=_T("FOT");
		}
		break;
	case MOD_NIVTRIGGER:
		{
			szMode=_T("NIV trigger");
		}
		break;
	default:
		{
			szMode=_T("- ERROR -");
		}
		break;
	}

	szCaption+=szMode;
	szInfo+=szMode;

	this->SetWindowText(szCaption);

	m_InfoText.SetFont(CFont::FromHandle(m_hfontButton));
	m_labelDevID.SetFont(CFont::FromHandle(m_hfontButton));

	m_InfoText.SetWindowText(szInfo);

	CStringW szDev =getModel()->GetLanguageString(IDS_TXT_DEVID);

	szDev+=_T(": ");
	szDev+=getModel()->GetUniqueID();
	m_labelDevID.SetWindowText(szDev);

	

	
	m_bnOk.SetFont(CFont::FromHandle(m_hfontButton));
	m_bnCancel.SetFont(CFont::FromHandle(m_hfontButton));
	m_bnHex0.SetFont(CFont::FromHandle(m_hfontButton));
	m_bnHex1.SetFont(CFont::FromHandle(m_hfontButton));
	m_bnHex2.SetFont(CFont::FromHandle(m_hfontButton));
	m_bnHex3.SetFont(CFont::FromHandle(m_hfontButton));
	m_bnHex4.SetFont(CFont::FromHandle(m_hfontButton));
	m_bnHex5.SetFont(CFont::FromHandle(m_hfontButton));
	m_bnHex6.SetFont(CFont::FromHandle(m_hfontButton));
	m_bnHex7.SetFont(CFont::FromHandle(m_hfontButton));
	m_bnHex8.SetFont(CFont::FromHandle(m_hfontButton));
	m_bnHex9.SetFont(CFont::FromHandle(m_hfontButton));
	m_bnHexA.SetFont(CFont::FromHandle(m_hfontButton));
	m_bnHexB.SetFont(CFont::FromHandle(m_hfontButton));
	m_bnHexC.SetFont(CFont::FromHandle(m_hfontButton));
	m_bnHexD.SetFont(CFont::FromHandle(m_hfontButton));
	m_bnHexE.SetFont(CFont::FromHandle(m_hfontButton));
	m_bnHexF.SetFont(CFont::FromHandle(m_hfontButton));
	m_bnDel.SetFont(CFont::FromHandle(m_hfontButton));

	m_ebHexString1.SetFont(CFont::FromHandle(m_hfontButton));
	m_ebHexString2.SetFont(CFont::FromHandle(m_hfontButton));
	m_ebHexString3.SetFont(CFont::FromHandle(m_hfontButton));
	m_ebHexString4.SetFont(CFont::FromHandle(m_hfontButton));
	m_ebHexString5.SetFont(CFont::FromHandle(m_hfontButton));

	SetTimer(FOCUSTIMER, 100, NULL);

	ReleaseDC(pDC);

	return TRUE;   // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
// CPasswordDlg message handlers
// **************************************************************************
// 
// **************************************************************************
CMVModel *CPasswordDlg::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}

// **************************************************************************
// 
// **************************************************************************
HFONT CPasswordDlg::CreateFontHandle(CDC* pDC, int nPixHeight, TCHAR* pszFacename, LONG lFontWeight, int iOrientation) 
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

void CPasswordDlg::OnBnClickedCancel()
{
	OnCancel();
}

void CPasswordDlg::OnBnClickedOk()
{
	if(		m_ebHexString1.LineLength()!=4
		&&	m_ebHexString2.LineLength()!=4
		&&	m_ebHexString3.LineLength()!=4
		&&	m_ebHexString4.LineLength()!=4
		&&	m_ebHexString5.LineLength()!=4)
	{
		m_InfoText.SetWindowText(_T("- key not comlete -"));
		m_ebpFocused->SetFocus();
		return;
	}


	// check key
	m_ebHexString1.GetWindowText(m_szEdit1);
	m_ebHexString2.GetWindowText(m_szEdit2);
	m_ebHexString3.GetWindowText(m_szEdit3);
	m_ebHexString4.GetWindowText(m_szEdit4);
	m_ebHexString5.GetWindowText(m_szEdit5);
	CString szInputKey=m_szEdit1+_T("-")+m_szEdit2+_T("-")+m_szEdit3+_T("-")+m_szEdit4+_T("-")+m_szEdit5;
	
	if(encryptKey(m_eModule)==szInputKey)
	{
		getModel()->getDATAHANDLER()->delDemoLicense(m_eModule, true);
		OnOK();
	}
	else if(encryptDemoKey(m_eModule)==szInputKey)
	{
		COleDateTime dtLicenseTimestamp = getModel()->getDATAHANDLER()->GetdemoTimestamp(m_eModule);
		
		if(dtLicenseTimestamp.GetStatus() != COleDateTime::valid)//already set, could not be written once more
		{
			getModel()->getDATAHANDLER()->setDemoLicense(m_eModule);
			OnOK();
		}
		else
		{
			m_InfoText.SetWindowText(_T("- already set -"));
			m_ebHexString1.SetWindowText(_T(""));
			m_ebHexString2.SetWindowText(_T(""));
			m_ebHexString3.SetWindowText(_T(""));
			m_ebHexString4.SetWindowText(_T(""));
			m_ebHexString5.SetWindowText(_T(""));
		}
	}
	else
	{
		m_InfoText.SetWindowText(_T("- wrong key -"));
		m_ebHexString1.SetWindowText(_T(""));
		m_ebHexString2.SetWindowText(_T(""));
		m_ebHexString3.SetWindowText(_T(""));
		m_ebHexString4.SetWindowText(_T(""));
		m_ebHexString5.SetWindowText(_T(""));

	}
}

void CPasswordDlg::OnBnClickedButton0()
{
	Key('0');
}

void CPasswordDlg::OnBnClickedButton1()
{
	Key('1');
}

void CPasswordDlg::OnBnClickedButton2()
{
	Key('2');
}

void CPasswordDlg::OnBnClickedButton3()
{
	Key('3');
}

void CPasswordDlg::OnBnClickedButton4()
{
	Key('4');
}

void CPasswordDlg::OnBnClickedButton5()
{
	Key('5');
}

void CPasswordDlg::OnBnClickedButton6()
{
	Key('6');
}

void CPasswordDlg::OnBnClickedButton7()
{
	Key('7');
}

void CPasswordDlg::OnBnClickedButton8()
{
	Key('8');
}

void CPasswordDlg::OnBnClickedButton9()
{
	Key('9');
}

void CPasswordDlg::OnBnClickedButtona()
{
	Key('A');
}

void CPasswordDlg::OnBnClickedButtonb()
{
	Key('B');
}

void CPasswordDlg::OnBnClickedButtonc()
{
	Key('C');
}

void CPasswordDlg::OnBnClickedButtond()
{
	Key('D');
}

void CPasswordDlg::OnBnClickedButtone()
{
	Key('E');
}

void CPasswordDlg::OnBnClickedButtonf()
{
	Key('F');
}

void CPasswordDlg::OnBnClickedButtonDel()
{
	Key(-1);
}

void CPasswordDlg::Key(int c)
{
	if(c == -1)
	{
		int iLen=m_ebpFocused->LineLength();
		if(iLen==0)
		{
			switch(m_ebpFocused->GetDlgCtrlID())
			{
			case IDC_EDIT1:
				{
					
				}
				break;
			case IDC_EDIT2:
				{
					m_ebHexString1.SetFocus();
				}
				break;
			case IDC_EDIT3:
				{
					m_ebHexString2.SetFocus();
				}
				break;
			case IDC_EDIT4:
				{
					m_ebHexString3.SetFocus();
				}
				break;
			case IDC_EDIT5:
				{
					m_ebHexString4.SetFocus();
				}
				break;
			default:
				{
					m_ebHexString5.SetFocus();
				}
				break;
			}

			if(m_ebpFocused)
				m_ebpFocused->SendMessage(WM_CHAR, VK_BACK, 1);
		}
		else
		{
			if(m_ebpFocused)
				m_ebpFocused->SendMessage(WM_CHAR, VK_BACK, 1);
		}
	}
	else
	{
		if(m_ebpFocused)
			m_ebpFocused->SendMessage(WM_CHAR, c, 1);
	}
	
	if(m_ebpFocused)
	{
		int iLen=m_ebpFocused->LineLength();
		if(iLen>=4)
		{
			switch(m_ebpFocused->GetDlgCtrlID())
			{
			case IDC_EDIT1:
				{
					m_ebHexString2.SetFocus();
				}
				break;
			case IDC_EDIT2:
				{
					m_ebHexString3.SetFocus();
				}
				break;
			case IDC_EDIT3:
				{
					m_ebHexString4.SetFocus();
				}
				break;
			case IDC_EDIT4:
				{
					m_ebHexString5.SetFocus();
				}
				break;
			case IDC_EDIT5:
				{
					m_ebHexString5.SetFocus();
				}
				break;
			default:
				{
					m_ebHexString1.SetFocus();
				}
				break;
			}
		}
		else
		{
			m_ebpFocused->SetFocus();
		}
	}
}
void CPasswordDlg::OnEnSetfocusEdit1()
{
	m_ebpFocused=&m_ebHexString1;
}

void CPasswordDlg::OnEnSetfocusEdit2()
{
	m_ebpFocused=&m_ebHexString2;
}

void CPasswordDlg::OnEnSetfocusEdit3()
{
	m_ebpFocused=&m_ebHexString3;
}

void CPasswordDlg::OnEnSetfocusEdit4()
{
	m_ebpFocused=&m_ebHexString4;
}

void CPasswordDlg::OnEnSetfocusEdit5()
{
	m_ebpFocused=&m_ebHexString5;
}


void CPasswordDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==FOCUSTIMER)
	{
		KillTimer(FOCUSTIMER);
		m_ebHexString1.SetFocus();
	}

	CDialog::OnTimer(nIDEvent);
}

//************************************
// Method:    OnDestroy
// FullName:  CPasswordDlg::OnDestroy
// Access:    protected 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CPasswordDlg::OnDestroy()
{
	KillTimer(FOCUSTIMER);

	CDialog::OnDestroy();
}


CString CPasswordDlg::encryptKey(eModule module)
{
	CString szEncryptedFormattedKey=_T("");
	switch(m_eModule)
	{
	case MOD_MASTER:
		{
			szEncryptedFormattedKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(getModel()->getDATAHANDLER()->encryptKey(MOD_MASTER));
		}
		break;
	case MOD_HFO:
		{
			szEncryptedFormattedKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(getModel()->getDATAHANDLER()->encryptKey(MOD_HFO));
		}
		break;
	case MOD_NMODE:
		{
			szEncryptedFormattedKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(getModel()->getDATAHANDLER()->encryptKey(MOD_NMODE));
		}
		break;
	case MOD_VGUARANTY:
		{
			szEncryptedFormattedKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(getModel()->getDATAHANDLER()->encryptKey(MOD_VGUARANTY));
		}
		break;
	case MOD_VLIMIT:
		{
			szEncryptedFormattedKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(getModel()->getDATAHANDLER()->encryptKey(MOD_VLIMIT));
		}
		break;
	case MOD_LUNGREC:
		{
			szEncryptedFormattedKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(getModel()->getDATAHANDLER()->encryptKey(MOD_LUNGREC));
		}
		break;
	case MOD_TREND:
		{
			szEncryptedFormattedKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(getModel()->getDATAHANDLER()->encryptKey(MOD_TREND));
		}
		break;
	case MOD_THERAPY:
		{
			szEncryptedFormattedKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(getModel()->getDATAHANDLER()->encryptKey(MOD_THERAPY));
		}
		break;
	case MOD_PRICO:
		{
			szEncryptedFormattedKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(getModel()->getDATAHANDLER()->encryptKey(MOD_PRICO));
		}
		break;
	case MOD_FOT:
		{
			szEncryptedFormattedKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(getModel()->getDATAHANDLER()->encryptKey(MOD_FOT));
		}
		break;
	case MOD_NIVTRIGGER:
		{
			szEncryptedFormattedKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(getModel()->getDATAHANDLER()->encryptKey(MOD_NIVTRIGGER));
		}
		break;
	default:
		{
			m_InfoText.SetWindowText(_T("- ERROR: wrong mode -"));
		}
		break;
	}
	return szEncryptedFormattedKey;
}
CString CPasswordDlg::encryptDemoKey(eModule module)
{
	CString szEncryptedFormattedKey=_T("");
	switch(m_eModule)
	{
	case MOD_HFO:
		{
			szEncryptedFormattedKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(getModel()->getDATAHANDLER()->encryptDEMOKey(MOD_HFO));
		}
		break;
	case MOD_NMODE:
		{
			szEncryptedFormattedKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(getModel()->getDATAHANDLER()->encryptDEMOKey(MOD_NMODE));
		}
		break;
	case MOD_VGUARANTY:
		{
			szEncryptedFormattedKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(getModel()->getDATAHANDLER()->encryptDEMOKey(MOD_VGUARANTY));
		}
		break;
	case MOD_VLIMIT:
		{
			szEncryptedFormattedKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(getModel()->getDATAHANDLER()->encryptDEMOKey(MOD_VLIMIT));
		}
		break;
	case MOD_LUNGREC:
		{
			szEncryptedFormattedKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(getModel()->getDATAHANDLER()->encryptDEMOKey(MOD_LUNGREC));
		}
		break;
	case MOD_TREND:
		{
			szEncryptedFormattedKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(getModel()->getDATAHANDLER()->encryptDEMOKey(MOD_TREND));
		}
		break;
	case MOD_THERAPY:
		{
			szEncryptedFormattedKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(getModel()->getDATAHANDLER()->encryptDEMOKey(MOD_THERAPY));
		}
		break;
	case MOD_PRICO:
		{
			szEncryptedFormattedKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(getModel()->getDATAHANDLER()->encryptDEMOKey(MOD_PRICO));
		}
		break;
	case MOD_FOT:
		{
			szEncryptedFormattedKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(getModel()->getDATAHANDLER()->encryptDEMOKey(MOD_FOT));
		}
		break;
	case MOD_NIVTRIGGER:
		{
			szEncryptedFormattedKey=getModel()->getDATAHANDLER()->getFormattedEncryptKey(getModel()->getDATAHANDLER()->encryptDEMOKey(MOD_NIVTRIGGER));
		}
		break;
	default:
		{
			m_InfoText.SetWindowText(_T("- ERROR: wrong mode -"));
		}
		break;
	}
	return szEncryptedFormattedKey;
}