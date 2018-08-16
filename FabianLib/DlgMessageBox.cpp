// TlsMessageBox.cpp : implementation file
//

#include "stdafx.h"
//#include "FabianHFO.h"
#include "DlgMessageBox.h"
#include "colour.h"

/**********************************************************************************************//**
 * A macro that defines color txtbtnup
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

#define COLOR_TXTBTNUP				0x00000000

/**********************************************************************************************//**
 * A macro that defines color txtbtndw
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

#define COLOR_TXTBTNDW				0x00FFFFFF

/**********************************************************************************************//**
 * A macro that defines color txtbtnfc
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

#define COLOR_TXTBTNFC				0x00000000

extern HFONT g_hf14AcuMed;
extern HFONT g_hf11AcuBold;
extern HFONT g_hf17AcuBold;
extern HFONT g_hf9AcuBold;

/**********************************************************************************************//**
 * CDlgMessageBox dialog
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CDlgMessageBox, CDialog)

/**********************************************************************************************//**
 * Initializes a new instance of the CDlgMessageBox class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	pParent   	If non-null, the parent.
 * \param 		  	sCaption  	The caption.
 * \param 		  	sText	  	The text.
 * \param 		  	iFlags	  	Zero-based index of the flags.
 * \param 		  	iRESSOURCE	Zero-based index of the ressource.
 * \param 		  	iHelpID   	Identifier for the help.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Finalizes an instance of the CDlgMessageBox class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

CDlgMessageBox::~CDlgMessageBox()
{
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
}

/**********************************************************************************************//**
 * Gets the model
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	Null if it fails, else the model.
 **************************************************************************************************/

CMVModel *CDlgMessageBox::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}

/**********************************************************************************************//**
 * Exchanges data to/from the controls in this dialog
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	pDX	If non-null, an object that manages the data exchange operation.
 **************************************************************************************************/

void CDlgMessageBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICTURE, m_pcRessource);
}


BEGIN_MESSAGE_MAP(CDlgMessageBox, CDialog)
	ON_BN_CLICKED(IDC_BTN_MSGBOX_OK, &CDlgMessageBox::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_MSGBOX_CANCEL, &CDlgMessageBox::OnBnClickedCancel)
	//ON_BN_CLICKED(IDC_RETRY, &CDlgMessageBox::OnBnClickedRetry)
	ON_BN_CLICKED(IDC_BTN_MSGBOX_YES, &CDlgMessageBox::OnBnClickedYes)
	ON_BN_CLICKED(IDC_BTN_MSGBOX_NO, &CDlgMessageBox::OnBnClickedNo)
	ON_WM_PAINT()
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * Initializes this dialog and the controls within it
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

BOOL CDlgMessageBox::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//_tcscpy_s(m_pszFontName,_countof(m_pszFontName),getModel()->GetFontFace());

	//MoveWindow(180,150,440,295); //Fabian HFO
	//MoveWindow(103,95,440,295);	//Fabian EVO

	int iXpos=(GetSystemMetrics(SM_CXSCREEN)-440)/2;
	int iYpos=(GetSystemMetrics(SM_CYSCREEN)-295)/2;

	MoveWindow(iXpos,iYpos,440,295);
	SetWindowPos(&this->wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

	CClientDC dc(this);

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
			m_pcYes->ShowWindow(SW_SHOW);
			m_pcNo->ShowWindow(SW_SHOW);
			m_pcOk->ShowWindow(SW_HIDE);
			m_pcCancel->ShowWindow(SW_HIDE);
		}
		break;
	case MB_OK:
		{
			m_pcYes->ShowWindow(SW_HIDE);
			m_pcNo->ShowWindow(SW_HIDE);
			m_pcOk->ShowWindow(SW_SHOW);
			m_pcCancel->ShowWindow(SW_HIDE);
		}
		break;
	case MB_OKCANCEL:
		{
			m_pcYes->ShowWindow(SW_HIDE);
			m_pcNo->ShowWindow(SW_HIDE);
			m_pcOk->ShowWindow(SW_SHOW);
			m_pcCancel->ShowWindow(SW_SHOW);
		}
		break;
	}

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

	return TRUE;   // return TRUE unless you set the focus to a control
}

/**********************************************************************************************//**
 * Executes the button clicked ok action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CDlgMessageBox::OnBnClickedOk()
{
	OnOK();
}

/**********************************************************************************************//**
 * Executes the button clicked cancel action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CDlgMessageBox::OnBnClickedCancel()
{
	OnCancel();
}

//void CDlgMessageBox::OnBnClickedRetry()
//{
//	EndDialog(IDRETRY);
//}

/**********************************************************************************************//**
 * Executes the button clicked yes action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CDlgMessageBox::OnBnClickedYes()
{
	EndDialog(IDYES);
}

/**********************************************************************************************//**
 * Executes the button clicked no action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CDlgMessageBox::OnBnClickedNo()
{
	EndDialog(IDNO);
}

/**********************************************************************************************//**
 * Draws this instance
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CDlgMessageBox::Draw()
{
	RECT rcCl;
	RECT rc;
	GetClientRect(&rcCl);

	CClientDC dc(this);
	HDC hdcMem=CreateCompatibleDC(dc.m_hDC);
	HBITMAP hBmpMem=CreateCompatibleBitmap(dc.m_hDC,rcCl.right,rcCl.bottom);
	HBITMAP hBmpMemPrev=(HBITMAP)SelectObject(hdcMem,hBmpMem);

	CBrush cbrBack(BACKGND);
	
	HBRUSH hbrprev=(HBRUSH)SelectObject(hdcMem,cbrBack);
	int nBkMode=SetBkMode(hdcMem,TRANSPARENT);
	HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf14AcuMed);
	int tc=SetTextColor(hdcMem,RGB(40,40,140));

	Rectangle(hdcMem, 0, 0, rcCl.right,rcCl.bottom);


	m_pcDlgLeftTop->Draw(hdcMem,0,0);
	m_pcDlgLeft->Draw(hdcMem,0,75);
	m_pcDlgLeft->Draw(hdcMem,0,135);
	m_pcDlgLeft->Draw(hdcMem,0,195);
	m_pcDlgLeftBot->Draw(hdcMem,0,255);

	m_pcDlgTop->Draw(hdcMem,40,0);
	m_pcDlgTop->Draw(hdcMem,130,0);
	m_pcDlgTop->Draw(hdcMem,220,0);
	m_pcDlgTop->Draw(hdcMem,310,0);

	m_pcDlgRightTop->Draw(hdcMem,400,0);
	m_pcDlgRight->Draw(hdcMem,400,75);
	m_pcDlgRight->Draw(hdcMem,400,135);
	m_pcDlgRight->Draw(hdcMem,400,195);
	m_pcDlgRightBot->Draw(hdcMem,400,255);

	m_pcDlgBot->Draw(hdcMem,40,255);
	m_pcDlgBot->Draw(hdcMem,130,255);
	m_pcDlgBot->Draw(hdcMem,220,255);
	m_pcDlgBot->Draw(hdcMem,310,255);

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

	SetBkMode(hdcMem,nBkMode);

	SelectObject(hdcMem,hbrprev);

	SetTextColor(hdcMem,tc);
	SelectObject(hdcMem,hBmpMemPrev);
	SelectObject(hdcMem,hPrevFont);
	DeleteObject(hBmpMem);
	DeleteDC(hdcMem);

}

/**********************************************************************************************//**
 * Paints this window
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CDlgMessageBox::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	Draw();
}
