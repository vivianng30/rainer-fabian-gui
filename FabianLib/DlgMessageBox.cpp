// TlsMessageBox.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "DlgMessageBox.h"
#include "colour.h"

#define COLOR_TXTBTNUP				0x00000000
#define COLOR_TXTBTNDW				0x00FFFFFF
#define COLOR_TXTBTNFC				0x00000000

extern HFONT g_hf14AcuMed;
extern HFONT g_hf11AcuBold;
extern HFONT g_hf17AcuBold;
extern HFONT g_hf9AcuBold;
//extern HFONT g_hf21AcuBold;

// CDlgMessageBox dialog

IMPLEMENT_DYNAMIC(CDlgMessageBox, CDialog)

CDlgMessageBox::CDlgMessageBox(CWnd* pParent /*=NULL*/,CStringW sCaption, CStringW sText, UINT iFlags, UINT iRESSOURCE, UINT iHelpID)
	: CDialog(CDlgMessageBox::IDD, pParent)
{
	m_pModel=NULL;

	m_iHelpID=iHelpID;
	m_iFlags=iFlags;
	m_szTextMsg=sText;
	m_szCaption = sCaption;

	//_tcscpy_s(m_pszFontName,_countof(m_pszFontName),getModel()->GetFontFace());
	m_iRessource=iRESSOURCE;

	m_pcDlgLeft=NULL;
	m_pcDlgLeftTop=NULL;
	m_pcDlgLeftBot=NULL;
	m_pcDlgRight=NULL;
	m_pcDlgRightTop=NULL;
	m_pcDlgRightBot=NULL;
	m_pcDlgTop=NULL;
	m_pcDlgBot=NULL;

	m_pcYes=NULL;
	m_pcNo=NULL;
	m_pcOk=NULL;
	m_pcCancel=NULL;
	//m_pcRetry=NULL;

	m_pcBtnUp=NULL;
	m_pcBtnDwn=NULL;
	m_pcBtnDis=NULL;
}

CDlgMessageBox::~CDlgMessageBox()
{
	//getModel()->SetMainthreadPending(false);

	delete m_pcDlgLeft;
	delete m_pcDlgLeftTop;
	delete m_pcDlgLeftBot;
	delete m_pcDlgRight;
	delete m_pcDlgRightTop;
	delete m_pcDlgRightBot;
	delete m_pcDlgTop;
	delete m_pcDlgBot;

	m_pcDlgLeft=NULL;
	m_pcDlgLeftTop=NULL;
	m_pcDlgLeftBot=NULL;
	m_pcDlgRight=NULL;
	m_pcDlgRightTop=NULL;
	m_pcDlgRightBot=NULL;
	m_pcDlgTop=NULL;
	m_pcDlgBot=NULL;

	delete m_pcYes;
	delete m_pcNo;
	delete m_pcOk;
	delete m_pcCancel;
	
	m_pcYes=NULL;
	m_pcNo=NULL;
	m_pcOk=NULL;
	m_pcCancel=NULL;
	//m_pcRetry=NULL;

	delete m_pcBtnUp;
	delete m_pcBtnDwn;
	delete m_pcBtnDis;

	m_pcBtnUp=NULL;
	m_pcBtnDwn=NULL;
	m_pcBtnDis=NULL;

	/*DeleteObject(m_brushBACKGND);
	DeleteObject(m_brushTITLE);
	DeleteObject(m_hfont);
	DeleteObject(m_hfontButton);*/
}

// **************************************************************************
// 
// **************************************************************************
CMVModel *CDlgMessageBox::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}

void CDlgMessageBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICTURE, m_pcRessource);
	/*DDX_Control(pDX, IDC_BTN_MSGBOX_YES, (CButton)m_pcYes);
	DDX_Control(pDX, IDC_BTN_MSGBOX_NO, m_pcNo);
	DDX_Control(pDX, IDC_BTN_MSGBOX_OK, m_pcOk);
	DDX_Control(pDX, IDC_BTN_MSGBOX_CANCEL, m_pcCancel);*/
	//DDX_Control(pDX, IDC_TXTCAPTION, m_szStaticTextCaption);
}


BEGIN_MESSAGE_MAP(CDlgMessageBox, CDialog)
	ON_BN_CLICKED(IDC_BTN_MSGBOX_OK, &CDlgMessageBox::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_MSGBOX_CANCEL, &CDlgMessageBox::OnBnClickedCancel)
	//ON_BN_CLICKED(IDC_RETRY, &CDlgMessageBox::OnBnClickedRetry)
	ON_BN_CLICKED(IDC_BTN_MSGBOX_YES, &CDlgMessageBox::OnBnClickedYes)
	ON_BN_CLICKED(IDC_BTN_MSGBOX_NO, &CDlgMessageBox::OnBnClickedNo)
	//ON_WM_CTLCOLOR()
	ON_WM_PAINT()
END_MESSAGE_MAP()

//INT_PTR CDlgMessageBox::DoModal() 
//{
//	CDialogTemplate dlt;
//	int             nResult;
//
//	// load dialog template
//	if (!dlt.Load(MAKEINTRESOURCE(CDlgMessageBox::IDD))) return -1;
//
//	// set your own font, for example "Arial", 10 pts. 
//	BOOL bFont=dlt.SetFont(m_pszFontName, 15);
//
//	// get pointer to the modified dialog template
//	LPSTR pdata = (LPSTR)GlobalLock(dlt.m_hTemplate);
//
//	// let MFC know that you are using your own template
//	m_lpszTemplateName = NULL;
//	InitModalIndirect(pdata);
//
//	// display dialog box
//	nResult = CDialog::DoModal();
//
//	// unlock memory object
//	GlobalUnlock(dlt.m_hTemplate);
//
//	return nResult;	
//}


BOOL CDlgMessageBox::OnInitDialog() 
{
	//LOGFONT lf; 

	CDialog::OnInitDialog();

	//_tcscpy_s(m_pszFontName,_countof(m_pszFontName),getModel()->GetFontFace());

	//MoveWindow(0,0,440,295);
	MoveWindow(180,150,440,295);

	SetWindowPos(&this->wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

	/* m_brushBACKGND.CreateSolidBrush(BACKGND);
	 m_brushTITLE.CreateSolidBrush(TITLEBLUE);*/

	 CDC* pDC = this->GetDC();

	 CClientDC dc(this);

	/* m_hfont=CreateFontHandle(pDC,16,m_pszFontName,FW_BOLD,0);
	 m_hfontButton=CreateFontHandle(pDC,18,m_pszFontName,FW_BOLD,0);*/

	 m_pcDlgLeft= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_DLG_LEFT);
	 m_pcDlgLeftTop= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_DLG_LEFT_TOP);
	 m_pcDlgLeftBot= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_DLG_LEFT_BOT);
	 m_pcDlgRight= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_DLG_RIGHT);
	 m_pcDlgRightTop= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_DLG_RIGHT_TOP);
	 m_pcDlgRightBot= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_DLG_RIGHT_BOT);
	 m_pcDlgTop= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_DLG_TOP);
	 m_pcDlgBot= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_DLG_BOT);

	 m_pcBtnUp		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_4MENU_UP);
	 m_pcBtnDwn		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_4MENU_DW);
	 m_pcBtnDis		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_4MENU_DIS);



	 BTN btn;

	 btn.wID				= IDC_BTN_MSGBOX_YES;	
	 btn.poPosition.x		= 40;
	 btn.poPosition.y		= 230;
	 btn.pcBmpUp			= m_pcBtnUp;
	 btn.pcBmpDown			= m_pcBtnDwn;
	 btn.pcBmpFocus			= m_pcBtnUp;
	 btn.pcBmpDisabled		= m_pcBtnDis;
	 btn.dwFormat			= DT_VCENTER|DT_CENTER;
	 m_pcYes=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	 m_pcYes->Create(this,g_hf14AcuMed,0);
	 m_pcYes->SetText(getModel()->GetLanguageString(IDS_MSGBOX_YES));
	 m_pcYes->ShowWindow(SW_HIDE);

	 btn.wID				= IDC_BTN_MSGBOX_OK;	
	 btn.poPosition.x		= 147;//40;
	 btn.poPosition.y		= 230;
	 btn.pcBmpUp			= m_pcBtnUp;
	 btn.pcBmpDown			= m_pcBtnDwn;
	 btn.pcBmpFocus			= m_pcBtnUp;
	 btn.pcBmpDisabled		= m_pcBtnDis;
	 btn.dwFormat			= DT_VCENTER|DT_CENTER;
	 m_pcOk=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	 m_pcOk->Create(this,g_hf14AcuMed,0);
	 m_pcOk->SetText(getModel()->GetLanguageString(IDS_MSGBOX_OK));
	 m_pcOk->ShowWindow(SW_HIDE);

	 //btn.wID					= IDRETRY;	
	 //btn.poPosition.x		= 360;//;586
	 //btn.poPosition.y		= 92;
	 //btn.pcBmpUp			= m_pcBtnUp;
	 //btn.pcBmpDown			= m_pcBtnDwn;
	 //btn.pcBmpFocus			= m_pcBtnUp;
	 //btn.pcBmpDisabled		= m_pcBtnDis;
	 //btn.dwFormat			= DT_VCENTER|DT_CENTER;
	 //m_pcRetry=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	 //m_pcRetry->Create(this,m_hfontButton,0);
	 //m_pcRetry->SetText(getModel()->GetLanguageString(IDS_MSGBOX_RETRY));
	 //m_pcRetry->ShowWindow(SW_HIDE);

	 btn.wID				= IDC_BTN_MSGBOX_NO;	
	 btn.poPosition.x		= 255;
	 btn.poPosition.y		= 230;
	 btn.pcBmpUp			= m_pcBtnUp;
	 btn.pcBmpDown			= m_pcBtnDwn;
	 btn.pcBmpFocus			= m_pcBtnUp;
	 btn.pcBmpDisabled		= m_pcBtnDis;
	 btn.dwFormat			= DT_VCENTER|DT_CENTER;
	 m_pcNo=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	 m_pcNo->Create(this,g_hf14AcuMed,0);
	 m_pcNo->SetText(getModel()->GetLanguageString(IDS_MSGBOX_NO));
	 m_pcNo->ShowWindow(SW_HIDE);

	 btn.wID				= IDC_BTN_MSGBOX_CANCEL;	
	 btn.poPosition.x		= 255;
	 btn.poPosition.y		= 230;
	 btn.pcBmpUp			= m_pcBtnUp;
	 btn.pcBmpDown			= m_pcBtnDwn;
	 btn.pcBmpFocus			= m_pcBtnUp;
	 btn.pcBmpDisabled		= m_pcBtnDis;
	 btn.dwFormat			= DT_VCENTER|DT_CENTER;
	 m_pcCancel=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	 m_pcCancel->Create(this,g_hf14AcuMed,0);
	 m_pcCancel->SetText(getModel()->GetLanguageString(IDS_MSGBOX_CANCEL));
	 m_pcCancel->ShowWindow(SW_HIDE);

	 switch(m_iHelpID)
	 {
	 case HLP_0001:
		 {
			m_szCaption+=_T(" - Enable factory settings");
			m_szTextMsg=_T("Enable factory settings:\n   - shows/hides the factory button in  the ventilation menu\n\nreset values:\n   - resets only the parameter settings to factory values\n\nreset complete:\n   - resets the complete device to factory settings, a system reboot will be performed");
		 }
		 break;
	 case HLP_0002:
		 {
			 m_szCaption+=_T(" - Enable auto alarm limits");
			 m_szTextMsg=_T("Shows/hides the buttons 'Autoset all' and 'Autoset single' of the alarm limit menu.");
		 }
		 break;
	 case HLP_0003:
		 {

		 }
		 break;
	 default:
		 break;
	 }

	switch(m_iFlags)
	{
	case MB_YESNO:
		{
			//m_bnYes.SetWindowText(_T("Ja"));
			//m_bnNo.SetWindowText(_T("Nein"));
			/*m_bnYes.SetWindowText(getModel()->GetLanguageString(IDS_MSGBOX_YES));
			m_bnNo.SetWindowText(getModel()->GetLanguageString(IDS_MSGBOX_NO));
			m_bnOk.ShowWindow(SW_HIDE);
			m_bnRetry.ShowWindow(SW_HIDE);
			m_bnCancel.ShowWindow(SW_HIDE);*/

			m_pcYes->ShowWindow(SW_SHOW);
			m_pcNo->ShowWindow(SW_SHOW);
			m_pcOk->ShowWindow(SW_HIDE);
			m_pcCancel->ShowWindow(SW_HIDE);
		}
		break;
	case MB_OK:
		{
			//m_bnYes.ShowWindow(SW_HIDE);
			//m_bnNo.ShowWindow(SW_HIDE);
			////m_bnOk.SetWindowText(_T("Ok"));
			//m_bnOk.SetWindowText(getModel()->GetLanguageString(IDS_MSGBOX_OK));
			//m_bnRetry.ShowWindow(SW_HIDE);
			//m_bnCancel.ShowWindow(SW_HIDE);

			m_pcYes->ShowWindow(SW_HIDE);
			m_pcNo->ShowWindow(SW_HIDE);
			m_pcOk->ShowWindow(SW_SHOW);
			m_pcCancel->ShowWindow(SW_HIDE);
		}
		break;
	case MB_OKCANCEL:
		{
			//m_bnYes.ShowWindow(SW_HIDE);
			//m_bnNo.ShowWindow(SW_HIDE);
			////m_bnOk.SetWindowText(_T("Ok"));
			//m_bnOk.SetWindowText(getModel()->GetLanguageString(IDS_MSGBOX_OK));
			//m_bnRetry.ShowWindow(SW_HIDE);
			////m_bnCancel.SetWindowText(_T("Abbrechen"));
			//m_bnCancel.SetWindowText(getModel()->GetLanguageString(IDS_MSGBOX_CANCEL));

			m_pcYes->ShowWindow(SW_HIDE);
			m_pcNo->ShowWindow(SW_HIDE);
			m_pcOk->ShowWindow(SW_SHOW);
			m_pcCancel->ShowWindow(SW_SHOW);
		}
		break;
	}
	
	//m_szStaticTextMsg.SetWindowText(m_szTextMsg);
	/*m_szStaticTextCaption.SetWindowText(m_szCaption);
	this->SetWindowText(m_szCaption);*/
	

	CRect rcDlg, rcRes;
	GetClientRect(rcDlg);

	if(m_iRessource!=0)
	{
		m_pcRessource.SetBitmap(::LoadBitmap(NULL, MAKEINTRESOURCE(m_iRessource)));//(AfxGetInstanceHandle(),(LPCTSTR) m_iRessource));
		m_pcRessource.GetClientRect(rcRes);
		m_pcRessource.MoveWindow(((rcDlg.right-rcRes.right)/2),55,rcRes.right,rcRes.bottom);
	}
	else
	{
		m_pcRessource.ShowWindow(SW_HIDE);
	}

	



	ReleaseDC(pDC);

	return TRUE;   // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgMessageBox::OnBnClickedOk()
{
	OnOK();
}

void CDlgMessageBox::OnBnClickedCancel()
{
	OnCancel();
}

//void CDlgMessageBox::OnBnClickedRetry()
//{
//	EndDialog(IDRETRY);
//}

void CDlgMessageBox::OnBnClickedYes()
{
	EndDialog(IDYES);
}

void CDlgMessageBox::OnBnClickedNo()
{
	EndDialog(IDNO);
}

//HBRUSH CDlgMessageBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//{
//	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
//
//	//// TODO:  Change any attributes of the DC here
//
//	//// TODO:  Return a different brush if the default is not desired
//	//return hbr;
//	int iID=0;
//	
//
//	switch (nCtlColor) 
//	{ 
//	case CTLCOLOR_STATIC:  // Für alle Statics           
//		{
//			iID=pWnd->GetDlgCtrlID();
//			if(iID==IDC_TXTCAPTION)
//			{
//				pDC->SetBkMode(TRANSPARENT);
//				//pDC->SetBkColor(TITLEBLUE);
//				hbr= m_brushTITLE;
//			}
//			else
//			{
//				pDC->SetBkColor(BACKGND);
//				hbr= m_brushBACKGND;
//			}
//		}
//		break;
//
//	//case CTLCOLOR_MSGBOX:     // Für den Dialog             
//	//	return (HBRUSH)m_brush;
//	//case CTLCOLOR_DLG:     // Für den Dialog             
//	//	return (HBRUSH)m_brush; 
//
//	default: 
//		{
//			hbr= m_brushBACKGND;
//		}
//		break;
//	} 
//
//
//	return hbr;
//}


// **************************************************************************
// 
// **************************************************************************
void CDlgMessageBox::Draw()
{
	RECT rcCl;
	RECT rc;
	GetClientRect(&rcCl);

	CClientDC dc(this);
	HDC hdcMem=CreateCompatibleDC(dc.m_hDC);
	HBITMAP hBmpMem=CreateCompatibleBitmap(dc.m_hDC,rcCl.right,rcCl.bottom);
	HBITMAP hBmpMemPrev=(HBITMAP)SelectObject(hdcMem,hBmpMem);


	//CBrush cbrBack(RGB(151,158,151));
	CBrush cbrBack(BACKGND);
	/*CBrush cbrTitle;
	cbrTitle.CreateSolidBrush(TITLEBLUE);*/
	
	HBRUSH hbrprev=(HBRUSH)SelectObject(hdcMem,cbrBack);
	//HPEN hpenprev=(HPEN)SelectObject(hdcMem, (HPEN)GetStockObject(NULL_PEN));	
	int nBkMode=SetBkMode(hdcMem,TRANSPARENT);
	HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf14AcuMed);
	int tc=SetTextColor(hdcMem,RGB(40,40,140));

	Rectangle(hdcMem, 0, 0, rcCl.right,rcCl.bottom);

	/*SelectObject(hdcMem,cbrTitle);
	Rectangle(hdcMem, 0, 0, rcCl.right,30);*/

	m_pcDlgLeftTop->Draw(hdcMem,0,0);
	m_pcDlgLeft->Draw(hdcMem,0,75);
	m_pcDlgLeft->Draw(hdcMem,0,135);
	m_pcDlgLeft->Draw(hdcMem,0,195);
	m_pcDlgLeftBot->Draw(hdcMem,0,255);

	m_pcDlgTop->Draw(hdcMem,40,0);
	m_pcDlgTop->Draw(hdcMem,130,0);
	m_pcDlgTop->Draw(hdcMem,220,0);
	m_pcDlgTop->Draw(hdcMem,310,0);
	//m_pcDlgTop->Draw(hdcMem,400,0);

	m_pcDlgRightTop->Draw(hdcMem,400,0);
	m_pcDlgRight->Draw(hdcMem,400,75);
	m_pcDlgRight->Draw(hdcMem,400,135);
	m_pcDlgRight->Draw(hdcMem,400,195);
	m_pcDlgRightBot->Draw(hdcMem,400,255);

	m_pcDlgBot->Draw(hdcMem,40,255);
	m_pcDlgBot->Draw(hdcMem,130,255);
	m_pcDlgBot->Draw(hdcMem,220,255);
	m_pcDlgBot->Draw(hdcMem,310,255);
	//m_pcDlgBot->Draw(hdcMem,400,255);


	rc.left = 25;  
	rc.top = 13;  
	rc.right  = 440;  
	rc.bottom = 50;
	DrawText(hdcMem,m_szCaption,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	
	SetTextColor(hdcMem,RGB(0,0,0));

	if(m_iHelpID==0)
	{
		SelectObject(hdcMem,g_hf11AcuBold);
		rc.left = 30;  
		rc.top = 135;  
		rc.right  = 410;  
		rc.bottom = 280;

		DrawText(hdcMem,m_szTextMsg,-1,&rc,DT_CENTER|DT_TOP|DT_WORDBREAK);
	}
	else
	{
		SelectObject(hdcMem,g_hf9AcuBold);
		rc.left = 20;  
		rc.top = 60;  
		rc.right  = 420;  
		rc.bottom = 225;

		DrawText(hdcMem,m_szTextMsg,-1,&rc,DT_LEFT|DT_TOP|DT_WORDBREAK);
	}
	
	

	BitBlt(dc.m_hDC,0,0,rcCl.right,rcCl.bottom,hdcMem,0,0,SRCCOPY);

	//SetTextColor(hdcMem,tc);
	SetBkMode(hdcMem,nBkMode);

	SelectObject(hdcMem,hbrprev);
	//SelectObject(hdcMem,hpenprev);

	//DeleteObject(cbrBack);

	SetTextColor(hdcMem,tc);
	SelectObject(hdcMem,hBmpMemPrev);
	SelectObject(hdcMem,hPrevFont);
	DeleteObject(hBmpMem);
	DeleteDC(hdcMem);

}
void CDlgMessageBox::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Fügen Sie hier Ihren Meldungsbehandlungscode ein.
	//BitBlt(dc.m_hDC,0,0,m_lX,m_lY,m_hDC,0,0,SRCCOPY);

	Draw();
}
