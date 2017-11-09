// DlgProxPressure60.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "DlgProxPressure60.h"
#include "colour.h"

#define COLOR_TXTBTNUP				0x00000000
#define COLOR_TXTBTNDW				0x00FFFFFF
#define COLOR_TXTBTNFC				0x00000000

extern HFONT g_hf14AcuMed;
extern HFONT g_hf11AcuBold;
extern HFONT g_hf17AcuBold;
extern HFONT g_hf9AcuBold;
extern HFONT g_hf21AcuBold;
extern HFONT g_hf33AcuBold;

#define TIMECHANGE	600


// CDlgProxPressure60 dialog

IMPLEMENT_DYNAMIC(CDlgProxPressure60, CDialog)

CDlgProxPressure60::CDlgProxPressure60(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgProxPressure60::IDD, pParent)
{
	m_pModel=NULL;

	m_pcSave=NULL;
	//m_pcRunBreath=NULL;
	m_pcCancel=NULL;

	m_pcDlgLeft=NULL;
	m_pcDlgLeftTop=NULL;
	m_pcDlgLeftBot=NULL;
	m_pcDlgRight=NULL;
	m_pcDlgRightTop=NULL;
	m_pcDlgRightBot=NULL;
	m_pcDlgTop=NULL;
	m_pcDlgBot=NULL;

	m_pcBtnUp=NULL;
	m_pcBtnDwn=NULL;
	m_pcBtnDis=NULL;

	m_hfont=NULL;
	m_szCaption = _T("60mbar Calibration");
	m_iPressure=0;
	m_iPProxADC=0;
	m_pszFontName[0]=0x0000;


	m_pcUp_Up=NULL;
	m_pcUp_Dw=NULL;
	m_pcDw_Up=NULL;
	m_pcDw_Dw=NULL;

	m_pcStatic_Up=NULL;
	m_pcStatic_Dw=NULL;
	m_pcStatic_Fc=NULL;

	m_pcPara_Select=NULL;
	m_pcPara_Select_yes=NULL;
	m_pcPara_Select_no=NULL;

	m_pbtnValue=NULL;

	m_pcNextUp=NULL;
	m_pcNextDwn=NULL;

	m_iPressureDiff=0;
	m_iCounter=0;
	m_eTimeChanger=TC_OFF;

	m_iLowerLimit=-60;
	m_iUpperLimit=60;

	m_dwLastSetupTimer=0;
}

CDlgProxPressure60::~CDlgProxPressure60()
{
	

	if(m_pcPara_Select)
		delete m_pcPara_Select;
	m_pcPara_Select=NULL;
	if(m_pcPara_Select_yes)
		delete m_pcPara_Select_yes;
	m_pcPara_Select_yes=NULL;
	if(m_pcPara_Select_no)
		delete m_pcPara_Select_no;
	m_pcPara_Select_no=NULL;

	delete m_pcStatic_Up;
	m_pcStatic_Up=NULL;
	delete m_pcStatic_Dw;
	m_pcStatic_Dw=NULL;
	delete m_pcStatic_Fc;
	m_pcStatic_Fc=NULL;

	delete m_pcUp_Up;
	delete m_pcUp_Dw;
	delete m_pcDw_Up;
	delete m_pcDw_Dw;

	m_pcUp_Up=NULL;
	m_pcUp_Dw=NULL;
	m_pcDw_Up=NULL;
	m_pcDw_Dw=NULL;

	delete m_pbtnValue;
	m_pbtnValue=NULL;

	delete m_pcNextUp;
	delete m_pcNextDwn;

	m_pcNextUp=NULL;
	m_pcNextDwn=NULL;

	delete m_pcSave;
	//delete m_pcRunBreath;
	delete m_pcCancel;

	m_pcSave=NULL;
	//m_pcRunBreath=NULL;
	m_pcCancel=NULL;

	delete m_pcBtnUp;
	delete m_pcBtnDwn;
	delete m_pcBtnDis;

	m_pcBtnUp=NULL;
	m_pcBtnDwn=NULL;
	m_pcBtnDis=NULL;

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

	DeleteObject(m_brush);
	DeleteObject(m_hfont);
}

void CDlgProxPressure60::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PRESSURE, m_szPressure);
	DDX_Control(pDX, IDC_PROXADC, m_szPProxADC);
}


BEGIN_MESSAGE_MAP(CDlgProxPressure60, CDialog)
	ON_BN_CLICKED(IDC_BTN_SETUP_YEAR, &CDlgProxPressure60::OnBnClickedValue)
	ON_BN_CLICKED(IDC_BTN_MSGBOX_SAVE, &CDlgProxPressure60::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BTN_MSGBOX_CANCEL, &CDlgProxPressure60::OnBnClickedCancel)
	//ON_BN_CLICKED(IDC_BTN_MSGBOX_RUN, &CDlgProxPressure60::OnBnClickedRunBreath)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDlgProxPressure60 message handlers
CMVModel *CDlgProxPressure60::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}

BOOL CDlgProxPressure60::OnInitDialog() 
{
	//LOGFONT lf; 

	CDialog::OnInitDialog();


	//MoveWindow(180,150,440,295);
	MoveWindow(135,122,530,355);

	SetWindowPos(&this->wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

	m_brush.CreateSolidBrush(BACKGND);

	_tcscpy_s(m_pszFontName,_countof(m_pszFontName),getModel()->GetFontFace());

	CClientDC dc(this);
	CDC* pDC = this->GetDC();

	m_hfont=CreateFontHandle(pDC,16,m_pszFontName,FW_BOLD,0);
	this->SetFont(CFont::FromHandle(m_hfont));
	m_szPProxADC.SetFont(CFont::FromHandle(m_hfont));
	m_szPressure.SetFont(CFont::FromHandle(m_hfont));
	
	m_szPProxADC.MoveWindow(270,255,180,20);
	m_szPressure.MoveWindow(130,255,130,20);

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

	m_pcUp_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETUP_UPUP);
	m_pcUp_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETUP_UPDW);
	m_pcDw_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETUP_DWUP);
	m_pcDw_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETUP_DWDW);

	m_pcStatic_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_VALUE_UP);
	m_pcStatic_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_VALUE_DW);
	m_pcStatic_Fc= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_VALUE_FC);

	m_pcPara_Select=NULL;
	m_pcPara_Select_yes= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SELECT_YES);
	m_pcPara_Select_no= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SELECT_NO);

	CStringW strVal;

	//getModel()->getDATAHANDLER()->setProcPressureCal60(600);

	if(m_iPressureDiff==0)
		strVal=_T("0.0");
	else
		strVal.Format(_T("%0.0f"),(double)m_iPressureDiff/100);

	BTN btn;

	btn.wID					= IDC_BTN_SETUP_YEAR;	
	btn.poPosition.x		= 215;
	btn.poPosition.y		= 120;
	btn.pcBmpUp				= m_pcStatic_Up;
	btn.pcBmpDown			= m_pcStatic_Dw;
	btn.pcBmpFocus			= m_pcStatic_Fc;
	btn.pcBmpDisabled		= m_pcStatic_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;

	m_pbtnValue=new CSelectSetupBtn(btn,COLOR_TXTBTNUP);
	m_pbtnValue->Create(this,g_hf33AcuBold,0);
	m_pbtnValue->SetText(strVal);
	m_pbtnValue->ShowWindow(SW_SHOW);


	btn.wID					= IDC_BTN_SETUP_NEXTUP;	
	btn.poPosition.x		= 340;
	btn.poPosition.y		= 108;
	btn.pcBmpUp				= m_pcUp_Up;
	btn.pcBmpDown			= m_pcUp_Dw;
	btn.pcBmpFocus			= m_pcUp_Up;
	btn.pcBmpDisabled		= m_pcUp_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;

	m_pcNextUp=new CDTUpDwnBtn(btn,COLOR_TXTBTNUP);
	m_pcNextUp->Create(this,g_hf21AcuBold,0);
	m_pcNextUp->SetText(_T(""));
	m_pcNextUp->ShowWindow(SW_SHOW);

	btn.wID					= IDC_BTN_SETUP_NEXTDW;	
	btn.poPosition.x		= 340;
	btn.poPosition.y		= 150;
	btn.pcBmpUp				= m_pcDw_Up;
	btn.pcBmpDown			= m_pcDw_Dw;
	btn.pcBmpFocus			= m_pcDw_Up;
	btn.pcBmpDisabled		= m_pcDw_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;

	m_pcNextDwn=new CDTUpDwnBtn(btn,COLOR_TXTBTNUP);
	m_pcNextDwn->Create(this,g_hf21AcuBold,0);
	m_pcNextDwn->SetText(_T(""));
	m_pcNextDwn->ShowWindow(SW_SHOW);

	btn.wID				= IDC_BTN_MSGBOX_SAVE;	
	btn.poPosition.x		= 100;
	btn.poPosition.y		= 280;
	btn.pcBmpUp				= m_pcBtnUp;
	btn.pcBmpDown			= m_pcBtnDwn;
	btn.pcBmpFocus			= m_pcBtnUp;
	btn.pcBmpDisabled		= m_pcBtnDis;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcSave=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcSave->Create(this,g_hf14AcuMed,0);
	m_pcSave->SetText(_T("Save"));
	m_pcSave->ShowWindow(SW_SHOW);

	

	//btn.wID				= IDC_BTN_MSGBOX_RUN;	
	//btn.poPosition.x		= 147;//40;
	//btn.poPosition.y		= 175;
	//btn.pcBmpUp			= m_pcBtnUp;
	//btn.pcBmpDown			= m_pcBtnDwn;
	//btn.pcBmpFocus			= m_pcBtnUp;
	//btn.pcBmpDisabled		= m_pcBtnDis;
	//btn.dwFormat			= DT_VCENTER|DT_CENTER;
	//m_pcRunBreath=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	//m_pcRunBreath->Create(this,g_hf14AcuMed,0);
	//m_pcRunBreath->SetText(_T("Run Breath"));
	//m_pcRunBreath->ShowWindow(SW_SHOW);

	btn.wID				= IDC_BTN_MSGBOX_CANCEL;	
	btn.poPosition.x		= 285;
	btn.poPosition.y		= 280;
	btn.pcBmpUp				= m_pcBtnUp;
	btn.pcBmpDown			= m_pcBtnDwn;
	btn.pcBmpFocus			= m_pcBtnUp;
	btn.pcBmpDisabled		= m_pcBtnDis;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcCancel=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcCancel->Create(this,g_hf14AcuMed,0);
	m_pcCancel->SetText(getModel()->GetLanguageString(IDS_MSGBOX_CANCEL));
	m_pcCancel->ShowWindow(SW_SHOW);

	m_szPProxADC.SetWindowText(_T("PPROX ADC:"));
	m_szPressure.SetWindowText(_T("Pressure:"));

	//this->SetWindowText(m_szCaption);

	
	ReleaseDC(pDC);

	SetTimer(PROXPRESSURECALTIMER, 250, NULL);
	return TRUE;
}

void CDlgProxPressure60::OnBnClickedCancel()
{
	KillTimer(PROXPRESSURECALTIMER);
	OnCancel();
}
void CDlgProxPressure60::OnBnClickedSave()
{
	setProcPressureCal60();

	KillTimer(PROXPRESSURECALTIMER);
	OnOK();
}
//void CDlgProxPressure60::OnBnClickedRunBreath()
//{
//	
//}
void CDlgProxPressure60::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Fügen Sie hier Ihren Meldungsbehandlungscode ein.
	//BitBlt(dc.m_hDC,0,0,m_lX,m_lY,m_hDC,0,0,SRCCOPY);

	Draw();
}
void CDlgProxPressure60::Draw()
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

	CBrush cbrFill;
	cbrFill.CreateSolidBrush(RGB(181,178,181));
	//cbrFill.CreateSolidBrush(RGB(150,150,150));
	SelectObject(hdcMem,cbrFill);

	/*SelectObject(hdcMem,cbrTitle);
	Rectangle(hdcMem, 0, 0, rcCl.right,30);*/

	m_pcDlgLeftTop->Draw(hdcMem,0,0);
	m_pcDlgLeft->Draw(hdcMem,0,75);
	m_pcDlgLeft->Draw(hdcMem,0,135);
	m_pcDlgLeft->Draw(hdcMem,0,195);
	m_pcDlgLeft->Draw(hdcMem,0,255);
	m_pcDlgLeftBot->Draw(hdcMem,0,315);

	m_pcDlgTop->Draw(hdcMem,40,0);
	m_pcDlgTop->Draw(hdcMem,130,0);
	m_pcDlgTop->Draw(hdcMem,220,0);
	m_pcDlgTop->Draw(hdcMem,310,0);
	m_pcDlgTop->Draw(hdcMem,400,0);

	m_pcDlgRightTop->Draw(hdcMem,490,0);
	m_pcDlgRight->Draw(hdcMem,490,75);
	m_pcDlgRight->Draw(hdcMem,490,135);
	m_pcDlgRight->Draw(hdcMem,490,195);
	m_pcDlgRight->Draw(hdcMem,490,255);
	m_pcDlgRightBot->Draw(hdcMem,490,315);

	m_pcDlgBot->Draw(hdcMem,40,315);
	m_pcDlgBot->Draw(hdcMem,130,315);
	m_pcDlgBot->Draw(hdcMem,220,315);
	m_pcDlgBot->Draw(hdcMem,310,315);
	m_pcDlgBot->Draw(hdcMem,400,315);


	rc.left = 25;  
	rc.top = 13;  
	rc.right  = 440;  
	rc.bottom = 50;
	DrawText(hdcMem,m_szCaption,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	CPen penLine;
	penLine.CreatePen(PS_SOLID,1,RGB(140,140,140));
	SelectObject(hdcMem,(HPEN)penLine);
	CBrush cbrRound(RGB(200,200,200));
	SelectObject(hdcMem,cbrRound);
	RoundRect(hdcMem, 130, 60, 400, 240,20,20);
	
	SetTextColor(hdcMem,RGB(0,0,0));

	MoveToEx(hdcMem, 195, 108, NULL);
	LineTo(hdcMem, 335, 108);

	MoveToEx(hdcMem, 195, 190, NULL);
	LineTo(hdcMem, 335, 190);

	SelectObject(hdcMem,g_hf14AcuMed);
	rc.left = 130;  
	rc.top = 60;  
	rc.right  = 400;  
	rc.bottom = 108;
	DrawText(hdcMem,_T("adjust 60mbar value +/-"),-1,&rc,DT_CENTER|DT_VCENTER|DT_SINGLELINE);

	/*if(m_iHelpID==0)
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
	}*/
	
	

	BitBlt(dc.m_hDC,0,0,rcCl.right,rcCl.bottom,hdcMem,0,0,SRCCOPY);

	//SetTextColor(hdcMem,tc);
	SetBkMode(hdcMem,nBkMode);

	SelectObject(hdcMem,hbrprev);
	//SelectObject(hdcMem,hpenprev);

	penLine.DeleteObject();

	SetTextColor(hdcMem,tc);
	SelectObject(hdcMem,hBmpMemPrev);
	SelectObject(hdcMem,hPrevFont);
	DeleteObject(hBmpMem);
	DeleteDC(hdcMem);
}
HFONT CDlgProxPressure60::CreateFontHandle(CDC* pDC, int nPixHeight, TCHAR* pszFacename, LONG lFontWeight, int iOrientation) 
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
void CDlgProxPressure60::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==CHANGETIMER)
	{
		switch(m_eTimeChanger)
		{
		case TC_PROXCAL_UP:
			{
				if(m_iPressureDiff<m_iUpperLimit)
				{
					m_iPressureDiff+=STEP_PROXCAL;

					CStringW strVal;

					if(m_iPressureDiff>0)
						strVal.Format(_T("+%0.1f"),(double)m_iPressureDiff/10);
					else
						strVal.Format(_T("%0.1f"),(double)m_iPressureDiff/10);
					m_pbtnValue->RefreshText(strVal);
					//setProcPressureCal60();
				}
				else
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_ENDRANGE);
				}

				if(getModel()->isSafeTickCountDelayExpired(m_dwLastSetupTimer, 1000))
				{
					m_dwLastSetupTimer=GetTickCount();
					if(GetParent())
						GetParent()->PostMessage(WM_SET_SETUPTIMER);
				}
			}
			break;
		case TC_PROXCAL_DW:
			{
				if(m_iPressureDiff>m_iLowerLimit)
				{
					m_iPressureDiff-=STEP_PROXCAL;

					CStringW strVal;

					if(m_iPressureDiff>0)
						strVal.Format(_T("+%0.1f"),(double)m_iPressureDiff/10);
					else
						strVal.Format(_T("%0.1f"),(double)m_iPressureDiff/10);
					m_pbtnValue->RefreshText(strVal);
					//setProcPressureCal60();
				}
				else
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_ENDRANGE);
				}

				if(getModel()->isSafeTickCountDelayExpired(m_dwLastSetupTimer, 1000))
				{
					m_dwLastSetupTimer=GetTickCount();
					if(GetParent())
						GetParent()->PostMessage(WM_SET_SETUPTIMER);
				}
			}
			break;
		default:
			{

			}
			break;
		}

		m_iCounter++;
	}
	else if(nIDEvent==PROXPRESSURECALTIMER)
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

HBRUSH CDlgProxPressure60::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (nCtlColor) 
	{ 
	case CTLCOLOR_STATIC:  // Für alle Statics           
		//pDC->SetTextColor(RGB(0, 0, 0)); 
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
BOOL CDlgProxPressure60::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message)
	{
	case WM_KEYDOWN:
		{
			if(pMsg->wParam==VK_SPACE)
			{
				/*int iID=0;
				CWnd* pFocWnd=NULL;
				pFocWnd = CWnd::GetFocus();
				if(pFocWnd==NULL)
					return 1;
				iID=pFocWnd->GetDlgCtrlID();*/

				eBtnState eState = m_pbtnValue->GetState();
				
				if(eState==BS_NONE)
				{
					m_pbtnValue->SetState(BS_FOCUSED);
				}
				else if(eState==BS_UP)
				{
					m_pbtnValue->SetState(BS_DOWN);
				}
				else if(eState==BS_FOCUSED)
				{
					m_pbtnValue->SetState(BS_DOWN);
				}
				else if(eState==BS_DOWN)
				{
					m_pbtnValue->SetState(BS_FOCUSED);
					setProcPressureCal60();
				}
				/*else if(iID==IDC_BTN_SETTING_BACK)
				{
					PostMessage(WM_MENUBUTTONDOWN,IDC_BTN_SETTING_BACK);
				}*/

				if(GetParent())
					GetParent()->PostMessage(WM_SET_SETUPTIMER);
				return 1;
			}
			else if(pMsg->wParam==VK_DOWN)
			{
				if(m_pbtnValue->GetState()==BS_DOWN)
				{
					if(m_iPressureDiff>m_iLowerLimit)
					{
						m_iPressureDiff-=STEP_PROXCAL;

						CStringW strVal;

						if(m_iPressureDiff>0)
							strVal.Format(_T("+%0.1f"),(double)m_iPressureDiff/10);
						else
							strVal.Format(_T("%0.1f"),(double)m_iPressureDiff/10);
						m_pbtnValue->RefreshText(strVal);
						//setProcPressureCal60();
					}
					else
					{
						if(AfxGetApp())
							AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_ENDRANGE);
					}
				}
				if(getModel()->isSafeTickCountDelayExpired(m_dwLastSetupTimer, 1000))
				{
					m_dwLastSetupTimer=GetTickCount();
					if(GetParent())
						GetParent()->PostMessage(WM_SET_SETUPTIMER);
				}
			}
			else if(pMsg->wParam==VK_UP)
			{
				if(m_pbtnValue->GetState()==BS_DOWN)
				{
					if(m_iPressureDiff<m_iUpperLimit)
					{
						m_iPressureDiff+=STEP_PROXCAL;

						CStringW strVal;

						if(m_iPressureDiff>0)
							strVal.Format(_T("+%0.1f"),(double)m_iPressureDiff/10);
						else
							strVal.Format(_T("%0.1f"),(double)m_iPressureDiff/10);
						m_pbtnValue->RefreshText(strVal);
						//setProcPressureCal60();
					}
					else
					{
						if(AfxGetApp())
							AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_ENDRANGE);
					}
				}
				if(getModel()->isSafeTickCountDelayExpired(m_dwLastSetupTimer, 1000))
				{
					m_dwLastSetupTimer=GetTickCount();
					if(GetParent())
						GetParent()->PostMessage(WM_SET_SETUPTIMER);
				}
			}
			else
			{
				//handle focus change manual, draw next focused button and return true
				//return 1;
			}
			break;
		}
		break;
	default:
		break;
	}
	return CWnd::PreTranslateMessage(pMsg);
}
// **************************************************************************
// 
// **************************************************************************
LRESULT CDlgProxPressure60::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
{
	switch(message)
	{
	case WM_TIMEBTN_DOWN:
		{
			switch(wParam)
			{
			case IDC_BTN_SETUP_NEXTUP:
				{
					m_iCounter=0;
					m_eTimeChanger=TC_PROXCAL_UP;
					SetTimer(CHANGETIMER,TIMECHANGE,NULL);
					m_pbtnValue->SetState(BS_DOWN);

					if(GetParent())
						GetParent()->PostMessage(WM_SET_SETUPTIMER);
					return 1;
				}
				break;
			case IDC_BTN_SETUP_NEXTDW:
				{
					m_iCounter=0;
					m_eTimeChanger=TC_PROXCAL_DW;
					SetTimer(CHANGETIMER,TIMECHANGE,NULL);
					m_pbtnValue->SetState(BS_DOWN);

					if(GetParent())
						GetParent()->PostMessage(WM_SET_SETUPTIMER);
					return 1;
				}
				break;
			default:
				break;
			}
		}
		break;
	case WM_TIMEBTN_UP:
		{
			switch(wParam)
			{
			case IDC_BTN_SETUP_NEXTUP:
				{
					if(m_iCounter==0)
					{
						if(m_iPressureDiff<m_iUpperLimit)
						{
							m_iPressureDiff+=STEP_PROXCAL;

							CStringW strVal;

							if(m_iPressureDiff>0)
								strVal.Format(_T("+%0.1f"),(double)m_iPressureDiff/10);
							else
								strVal.Format(_T("%0.1f"),(double)m_iPressureDiff/10);
							m_pbtnValue->RefreshText(strVal);
							//setProcPressureCal60();
						}
						else
						{
							if(AfxGetApp())
								AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_ENDRANGE);
						}
					}
					m_iCounter=0;
					m_eTimeChanger=TC_OFF;
					KillTimer(CHANGETIMER);

					if(GetParent())
						GetParent()->PostMessage(WM_SET_SETUPTIMER);
					return 1;
				}
				break;
			case IDC_BTN_SETUP_NEXTDW:
				{
					if(m_iCounter==0)
					{
						if(m_iPressureDiff>m_iLowerLimit)
						{
							m_iPressureDiff-=STEP_PROXCAL;

							CStringW strVal;

							if(m_iPressureDiff>0)
								strVal.Format(_T("+%0.1f"),(double)m_iPressureDiff/10);
							else
								strVal.Format(_T("%0.1f"),(double)m_iPressureDiff/10);
							m_pbtnValue->RefreshText(strVal);
							//setProcPressureCal60();
						}
						else
						{
							if(AfxGetApp())
								AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_ENDRANGE);
						}
					}
					m_iCounter=0;
					m_eTimeChanger=TC_OFF;
					KillTimer(CHANGETIMER);

					if(GetParent())
						GetParent()->PostMessage(WM_SET_SETUPTIMER);
					return 1;
				}
				break;
			default:
				break;
			}
		}
		break;
	case WM_MENUBUTTONDOWN:
		{
			switch(wParam)
			{
			case IDC_BTN_SELECT_EFLOW:
				{
					if(getModel()->getCONFIG()->getShowEFlowInSettings()==true)
					{
						getModel()->getCONFIG()->setShowEFlowInSettings(false);
					}
					else
					{
						getModel()->getCONFIG()->setShowEFlowInSettings(true);
					}
					return 1;
				}
				break;
			default:
				{

				}
				break;
			}
		}
		break;
	}
	return CWnd::WindowProc(message, wParam, lParam);
}
void CDlgProxPressure60::OnBnClickedValue()
{
	//SetOneButtonDepressed(IDC_BTN_SETUP_YEAR);
	if(m_pbtnValue->GetState()==BS_UP)
		m_pbtnValue->SetState(BS_DOWN);
	else if(m_pbtnValue->GetState()==BS_FOCUSED)
		m_pbtnValue->SetState(BS_DOWN);
	else if(m_pbtnValue->GetState()==BS_DOWN)
	{
		m_pbtnValue->SetState(BS_FOCUSED);
		setProcPressureCal60();
	}
}

void CDlgProxPressure60::setProcPressureCal60()
{
	//getModel()->getDATAHANDLER()->setProcPressureCal60(600+m_iPressureDiff);
	getModel()->getDATAHANDLER()->SetPINSPParadata_IPPV(600+m_iPressureDiff,true);

	DEBUGMSG(TRUE, (TEXT("setProcPressureCal60 %d\r\n"),600+m_iPressureDiff));
}
