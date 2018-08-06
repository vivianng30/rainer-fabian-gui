// WndServiceDisplay.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "WndServiceDisplay.h"
#include "TlsRegistry.h"
#include "DlgMessageBox.h"


#include <commctrl.h>

extern "C" BOOL WINAPI TouchCalibrate( void );

/**********************************************************************************************//**
 * CWndServiceDisplay
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CWndServiceDisplay, CWnd)

/**********************************************************************************************//**
 * Initializes a new instance of the CWndServiceDisplay class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

CWndServiceDisplay::CWndServiceDisplay():
CWndService()
{
	m_pcLeft=NULL;
	m_pcRight=NULL;
	m_pcLeft_Up=NULL;
	m_pcLeft_Dw=NULL;
	m_pcRight_Up=NULL;
	m_pcRight_Dw=NULL;
	m_pcCal_Up=NULL;
	m_pcCal_Dw=NULL;
	m_pcCal=NULL;
	/*m_pcSave_Up=NULL;
	m_pcSave_Dw=NULL;
	m_pcSave_Red=NULL;
	m_pcSave=NULL;*/

	m_bContrastChanged=false;

	m_pDlg = NULL;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CWndServiceDisplay class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

CWndServiceDisplay::~CWndServiceDisplay()
{
	delete m_pcLeft;
	m_pcLeft=NULL;
	delete m_pcRight;
	m_pcRight=NULL;
	delete m_pcLeft_Up;
	m_pcLeft_Up=NULL;
	delete m_pcLeft_Dw;
	m_pcLeft_Dw=NULL;
	delete m_pcRight_Up;
	m_pcRight_Up=NULL;
	delete m_pcRight_Dw;
	m_pcRight_Dw=NULL;
	delete  m_pcCal_Up;
	m_pcCal_Up=NULL;
	delete  m_pcCal_Dw;
	m_pcCal_Dw=NULL;
	delete  m_pcCal;
	m_pcCal=NULL;
	/*delete m_pcSave_Up;
	m_pcSave_Up=NULL;
	delete m_pcSave_Dw;
	m_pcSave_Dw=NULL;
	delete m_pcSave_Red;
	m_pcSave_Red=NULL;
	delete m_pcSave;
	m_pcSave=NULL;*/
	
}


BEGIN_MESSAGE_MAP(CWndServiceDisplay, CWnd)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_SERVICE_DISPLCAL, &CWndServiceDisplay::OnBnClickedCalibrate)
	//ON_WM_HSCROLL()
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * CWndServiceDisplay message handlers
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndServiceDisplay::Init()
{
	CClientDC dc(this);

	m_pcCal_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PAT_UP);
	m_pcCal_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PAT_DW);

	m_pcLeft_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_LEFT_UP);
	m_pcLeft_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_LEFT_DW);
	m_pcRight_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_RIGHT_UP);
	m_pcRight_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_RIGHT_DW);

	/*m_pcSave_Up		= new CBmp(theApp.m_hInstance,hdc,	IDB_BTN_PAT_UP);
	m_pcSave_Dw		= new CBmp(theApp.m_hInstance,hdc,	IDB_BTN_PAT_DW);
	m_pcSave_Red	= new CBmp(theApp.m_hInstance,hdc,	IDB_BTN_PAT_RED);*/

	//m_pctlSlider= new CSliderCtrl();

	//if(m_pctlSlider->Create(WS_CHILD|WS_VISIBLE|TBS_BOTH , CRect(60, 80, 320, 120), this, IDC_CONTARSTSLIDER))
	//{
	//	//DWORD dwContrast = GetContrast();
	//	//int iMin=dwContrast -20;
	//	//int iMax= __min(dwContrast+20,0x3FF);
	//	int iMin=0;
	//	int iMax= 4060;
	//	m_pctlSlider->SetRange( iMin, iMax );
	//	//m_pctlSlider->SetPos( GetContrast() );

	//	m_pctlSlider->SetPos( iMin );
	//}

	

	BTN btn;

	btn.wID					= IDC_BTN_CONTRAST_DECR;	
	btn.poPosition.x		= 140;
	btn.poPosition.y		= 80;
	btn.pcBmpUp				= m_pcLeft_Up;
	btn.pcBmpDown			= m_pcLeft_Dw;
	btn.pcBmpFocus			= m_pcLeft_Up;
	btn.pcBmpDisabled		= m_pcLeft_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcLeft=new CPushBtn(btn,COLOR_TXTBTNUP);
	m_pcLeft->Create(this,g_hf33AcuBold,0);
	m_pcLeft->SetText(_T("-"));
	m_pcLeft->ShowWindow(SW_SHOW);

	btn.wID					= IDC_BTN_CONTRAST_INCR;	
	btn.poPosition.x		= 240;
	btn.poPosition.y		= 80;
	btn.pcBmpUp				= m_pcRight_Up;
	btn.pcBmpDown			= m_pcRight_Dw;
	btn.pcBmpFocus			= m_pcRight_Up;
	btn.pcBmpDisabled		= m_pcRight_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcRight=new CPushBtn(btn,COLOR_TXTBTNUP);
	m_pcRight->Create(this,g_hf33AcuBold,0);
	m_pcRight->SetText(_T("+"));
	m_pcRight->ShowWindow(SW_SHOW);



	/*btn.wID					= IDC_BTN_SAVE_CONTRAST;	
	btn.poPosition.x		= 400;
	btn.poPosition.y		= 82;
	btn.pcBmpUp				= m_pcSave_Up;
	btn.pcBmpDown			= m_pcSave_Dw;
	btn.pcBmpFocus			= m_pcSave_Up;
	btn.pcBmpDisabled		= m_pcSave_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcSave=new CNumBtn(btn,COLOR_TXTBTNUP);
	m_pcSave->Create(this,g_hf15AcuMed,0);
	m_pcSave->SetText(_T("save"));
	m_pcSave->ShowWindow(SW_SHOW);*/


	btn.wID					= IDC_BTN_SERVICE_DISPLCAL;	
	btn.poPosition.x		= 140;
	btn.poPosition.y		= 170;
	btn.pcBmpUp				= m_pcCal_Up;
	btn.pcBmpDown			= m_pcCal_Dw;
	btn.pcBmpFocus			= m_pcCal_Up;
	btn.pcBmpDisabled		= m_pcCal_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcCal=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcCal->Create(this,g_hf15AcuMed,0);
	m_pcCal->SetText(_T("Calibration"));
	m_pcCal->ShowWindow(SW_SHOW);



	m_pcMenuBack->ShowWindow(SW_SHOW);
}

/**********************************************************************************************//**
 * Draws this instance
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndServiceDisplay::Draw()
{
	RECT rcCl;
	RECT rc;
	GetClientRect(&rcCl);

	CClientDC dc(this);
	HDC hdcMem=CreateCompatibleDC(dc.m_hDC);
	HBITMAP hBmpMem=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
	HBITMAP hBmpMemPrev=(HBITMAP)SelectObject(hdcMem,hBmpMem);


	CBrush cbrBack(RGB(181,178,181));
	HBRUSH hbrprev=(HBRUSH)SelectObject(hdcMem,cbrBack);
	HPEN hpenprev=(HPEN)SelectObject(hdcMem, (HPEN)GetStockObject(NULL_PEN));	
	int nBkMode=SetBkMode(hdcMem,TRANSPARENT);
	HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf10AcuBold);
	int tc=SetTextColor(hdcMem,0x0000000);

	Rectangle(hdcMem, 0, 0, m_lX, m_lY);

	
	
	//**************************Software Rev.**********************************//
	rc.left = 40;  
	rc.top = 95;  
	rc.right  = 280;  
	rc.bottom = 500;
	CStringW cs = _T("Brightness:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 40;  
	rc.top = 185;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("Touch:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	/*SelectObject(hdcMem,g_hf33AcuBold);

	rc.left = 30;  
	rc.top = 80;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("-");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 335;  
	rc.top = 80;  
	rc.right  = 440;  
	rc.bottom = 500;
	cs = _T("+");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);*/

	/*rc.left = 280;  
	rc.top = 40;  
	rc.right  = 600;  
	rc.bottom = 500;
	DrawText(hdcMem,getModel()->GetVersion(),-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);*/

	

	//if(TouchCalibrate()==FALSE)
	//	int iSt=0;//Error
	//else
	//	int iSt2=0;

	/*CTlsRegistry regGraphs(_T("HKLM\\HARDWARE\\DEVICEMAP\\KEYBD"),true);
	regGraphs.WriteDWORD(_T("Status"), 0);*/
	/*RegFlushKey(HKEY_LOCAL_MACHINE);*/
	//regGraphs.Flush();

	//dc.BitBlt(0,0,m_lX,m_lY,CDC::FromHandle(hdcMem),0,0,SRCCOPY);
	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,hdcMem,0,0,SRCCOPY);

	SetTextColor(hdcMem,tc);
	SetBkMode(hdcMem,nBkMode);

	SelectObject(hdcMem,hbrprev);
	SelectObject(hdcMem,hpenprev);

	//DeleteObject(cbrBack);

	SelectObject(hdcMem,hBmpMemPrev);
	SelectObject(hdcMem,hPrevFont);
	DeleteObject(hBmpMem);
	DeleteDC(hdcMem);
}

/**********************************************************************************************//**
 * Executes the destroy action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndServiceDisplay::OnDestroy()
{
	if(m_pDlg)
	{
		m_pDlg->EndDialog(IDCLOSE);
	}
	m_pDlg = NULL;

	CWndService::OnDestroy();
}

//void CWndServiceDisplay::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
//{
//	switch( pScrollBar->GetDlgCtrlID() )
//	{
//	case IDC_CONTARSTSLIDER:
//		//SetContrast( m_ctlSlider.GetPos() );
//		//getDATAHANDLER()->IncreaseContrast();
//
//		m_pcSave->SetBitmaps(m_pcSave_Red,m_pcSave_Dw,m_pcSave_Red,m_pcSave_Red);
//		break;
//	}
//
//	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
//}

/**********************************************************************************************//**
 * Executes the button clicked calibrate action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndServiceDisplay::OnBnClickedCalibrate()
{
	CDlgMessageBox box(this,getModel()->GetLanguageString(IDS_MSG_CAPTION_CALIBRATION),getModel()->GetLanguageString(IDS_MSG_CALIBRATION),MB_YESNO,IDB_MSG_INFO);
	m_pDlg = &box;
	int iRes=box.DoModal();
	m_pDlg = NULL;

	
	if(iRes==IDCLOSE)
	{
		//getModel()->SetMainthreadPending(false);
		return;
	}
	if(iRes==IDYES)
	{
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_CAL_DISPLAY);
	}

	UpdateWindow();
	Draw();
}

/**********************************************************************************************//**
 * Button clicked increase
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndServiceDisplay::BnClickedIncrease()
{
	getModel()->getDATAHANDLER()->IncreaseContrast();
}

/**********************************************************************************************//**
 * Button clicked decrease
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndServiceDisplay::BnClickedDecrease()
{
	getModel()->getDATAHANDLER()->DecreaseContrast();
}

/**********************************************************************************************//**
 * Window proc
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	message	The message.
 * \param	wParam 	The wParam field of the message.
 * \param	lParam 	The lParam field of the message.
 *
 * \return	A LRESULT.
 **************************************************************************************************/

LRESULT CWndServiceDisplay::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
{
	switch(message)
	{
	case WM_MENUBUTTONDOWN:
		{

			switch(wParam)
			{
			/*case IDC_BTN_SERVICE_DISPLCAL:
				{
					BnClickedCalibration();
					return 1;
				}
				break;*/
			case IDC_BTN_CONTRAST_INCR:
				{
					BnClickedIncrease();
					return 1;
				}
				break;
			case IDC_BTN_CONTRAST_DECR:
				{
					BnClickedDecrease();
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
	return CWndService::WindowProc(message, wParam, lParam);
}