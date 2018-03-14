// SubViewTools.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "SubViewTools.h"
#include "TlsFile.h"
#include "DlgMessageBox.h"
#include "TlsLogfile.h"

/**********************************************************************************************//**
 * A macro that defines color txtbtnup
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

#define COLOR_TXTBTNUP				0x00000000

/**********************************************************************************************//**
 * A macro that defines color txtbtndw
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

#define COLOR_TXTBTNDW				0x00FFFFFF

/**********************************************************************************************//**
 * A macro that defines color txtbtnfc
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

#define COLOR_TXTBTNFC				0x00000000

//global font objects
extern HFONT g_hf7AcuNorm;
extern HFONT g_hf7AcuBold;
extern HFONT g_hf8AcuNorm;
extern HFONT g_hf8AcuBold;
extern HFONT g_hf10AcuBold;
extern HFONT g_hf11AcuBold;
extern HFONT g_hf13AcuBold;
extern HFONT g_hf14AcuBold;
extern HFONT g_hf14AcuMed;
extern HFONT g_hf15AcuMed;
extern HFONT g_hf17AcuBold;
extern HFONT g_hf19AcuMed;
extern HFONT g_hf21AcuBold;
extern HFONT g_hf23AcuBold;
extern HFONT g_hf25AcuMed;
extern HFONT g_hf27AcuBold;
extern HFONT g_hf31AcuBold;
extern HFONT g_hf33AcuBold;
extern HFONT g_hf43AcuBold;
extern HFONT g_hf53AcuBold;

/**********************************************************************************************//**
 * CSubViewTools
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CSubViewTools, CWnd)

/**********************************************************************************************//**
 * Initializes a new instance of the CSubViewTools class
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

CSubViewTools::CSubViewTools()
{
	m_pModel=NULL;

	m_pDlg = NULL;

	m_hDC=NULL;
	m_hBmp=NULL;
	m_hBmpPrev=NULL;
	m_lX				= 0;
	m_lY				= 0;
	m_lXo				= 0;
	m_lYo				= 0;

	m_pWndHourglass=NULL;

	m_pcRegionTopLeCorner=NULL;
	m_pcRegionTopRiCorner=NULL;
	m_pcRegionBotLeCorner=NULL;
	m_pcRegionBotRiCorner=NULL;

	m_pcTrendToUSB=NULL;
	m_pcTrendToUSB_Up=NULL;
	m_pcTrendToUSB_Dw=NULL;
	m_pcLogToUSB=NULL;
	m_pcDevInfoToUSB=NULL;

	

	if(CTlsFile::Exists(_T("\\Hard Disk")))
	{
		m_bUSBavailable=true;
	}
	else
	{
		m_bUSBavailable=false;
	}

	m_pcwtWaitTrendUSBThread=NULL;
	m_hThreadWaitTrendUSB=INVALID_HANDLE_VALUE;
	m_bDoWaitTrendUSBThread=false;

	m_bExit=false;

	m_bSavingTrendToUSB=getModel()->getDATAHANDLER()->IsSavingTrendToUSBactiv();
}

/**********************************************************************************************//**
 * Finalizes an instance of the CSubViewTools class
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

CSubViewTools::~CSubViewTools()
{
	delete m_pcRegionTopLeCorner;
	m_pcRegionTopLeCorner=NULL;
	delete m_pcRegionTopRiCorner;
	m_pcRegionTopRiCorner=NULL;
	delete m_pcRegionBotLeCorner;
	m_pcRegionBotLeCorner=NULL;
	delete m_pcRegionBotRiCorner;
	m_pcRegionBotRiCorner=NULL;

	delete m_pcTrendToUSB;
	delete m_pcTrendToUSB_Up;
	delete m_pcTrendToUSB_Dw;
	delete m_pcLogToUSB;
	delete m_pcDevInfoToUSB;

	m_pcTrendToUSB=NULL;
	m_pcTrendToUSB_Up=NULL;
	m_pcTrendToUSB_Dw=NULL;
	m_pcLogToUSB=NULL;
	m_pcDevInfoToUSB=NULL;

	/*delete m_pcNumConfig;
	delete m_pcNumDefault;

	m_pcNumConfig=NULL;
	m_pcNumDefault=NULL;*/
}


BEGIN_MESSAGE_MAP(CSubViewTools, CWnd)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_TRENDTOUSB, &CSubViewTools::OnBnClickedTrendToUSB)
	ON_BN_CLICKED(IDC_BTN_LOGFILES, &CSubViewTools::OnBnClickedLogToUSB)
	ON_BN_CLICKED(IDC_BTN_DEVICEINFO, &CSubViewTools::OnBnClickedDevInfoToUSB)
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * CSubViewTools message handlers
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	Null if it fails, else the model.
 **************************************************************************************************/

CMVModel *CSubViewTools::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}

/**********************************************************************************************//**
 * Creates a window
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param [in,out]	pParentWnd	If non-null, the parent window.
 * \param 		  	rc		  	The rectangle.
 * \param 		  	nID		  	The identifier.
 * \param [in,out]	pContext  	If non-null, the context.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CSubViewTools::CreateWnd(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext)
{
	if (!Create(pParentWnd, rc, nID, pContext))
	{
		return false;
	}

	Initialize();

	return true;
}

/**********************************************************************************************//**
 * Creates the Window instance that will be represented by this instance
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param [in,out]	pParentWnd	If non-null, the parent window.
 * \param 		  	rc		  	The rectangle.
 * \param 		  	nID		  	The identifier.
 * \param [in,out]	pContext  	If non-null, the context.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

BOOL CSubViewTools::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext) 
{
	m_lX=rc.right-rc.left;
	m_lY=rc.bottom-rc.top;

	CStringW  lpszClassName=AfxRegisterWndClass(NULL); 
	if(CWnd::Create(lpszClassName,NULL,WS_CHILD|WS_VISIBLE,rc,pParentWnd,nID))
	{
		CClientDC dc(this);
		m_hDC=CreateCompatibleDC(dc.m_hDC);
		m_hBmp=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
		m_hBmpPrev=(HBITMAP)SelectObject(m_hDC,m_hBmp);

		//CBrush cbrBack(RGB(210,210,255));
		CBrush cbrBack(BACKGND);
		HBRUSH hbrprev=(HBRUSH)SelectObject(m_hDC,cbrBack);
		HPEN hpenprev=(HPEN)SelectObject(m_hDC, (HPEN)GetStockObject(NULL_PEN));	

		Rectangle(m_hDC, 0, 0, m_lX, m_lY);

		SelectObject(m_hDC,hbrprev);
		SelectObject(m_hDC,hpenprev);

		return 1;
	}
	else
		return 0;
}

/**********************************************************************************************//**
 * Initializes this instance
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CSubViewTools::Initialize()
{
	CClientDC dc(this);

	m_pcRegionTopLeCorner		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_SETUP_LE_TOP_CORNER);
	m_pcRegionTopRiCorner		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_SETUP_RI_TOP_CORNER);
	m_pcRegionBotLeCorner		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_SETUP_LE_BOT_CORNER);
	m_pcRegionBotRiCorner		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_SETUP_RI_BOT_CORNER);

	m_pcTrendToUSB_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN160_LG_UP);
	m_pcTrendToUSB_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN160_LG_DW);

	BTN btn;

	btn.wID					= IDC_BTN_TRENDTOUSB;	
	btn.poPosition.x		= 470;
	btn.poPosition.y		= 57;
	btn.pcBmpUp				= m_pcTrendToUSB_Up;
	btn.pcBmpDown			= m_pcTrendToUSB_Dw;
	btn.pcBmpFocus			= m_pcTrendToUSB_Up;
	btn.pcBmpDisabled		= m_pcTrendToUSB_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcTrendToUSB=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcTrendToUSB->Create(this,g_hf14AcuMed,0);
	//m_pcTrendToUSB->SetText(_T("save"));
	m_pcTrendToUSB->SetText(getModel()->GetLanguageString(IDS_TXT_SAVE));
	
	if(getModel()->getDATAHANDLER()->isTRENDLicenseAvailable())
	{
		m_pcTrendToUSB->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pcTrendToUSB->ShowWindow(SW_HIDE);
	}


	//btn.wID					= IDC_BTN_CONFIG_NUMERIC;	
	//btn.poPosition.x		= 330;
	//btn.poPosition.y		= 160;
	//btn.pcBmpUp				= m_pcTrendToUSB_Up;
	//btn.pcBmpDown			= m_pcTrendToUSB_Dw;
	//btn.pcBmpFocus			= m_pcTrendToUSB_Up;
	//btn.pcBmpDisabled		= m_pcTrendToUSB_Up;
	//btn.dwFormat			= DT_VCENTER|DT_CENTER;
	//m_pcNumConfig=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	//m_pcNumConfig->Create(this,g_hf14AcuMed,0);
	//m_pcNumConfig->SetText(_T("configure"));
	////m_pcNumConfig->SetText(getModel()->GetLanguageString(IDS_TXT_SAVE));
	////m_pcNumConfig->ShowWindow(SW_SHOW);
	//m_pcNumConfig->ShowWindow(SW_HIDE);


	//btn.wID					= IDC_BTN_DEFAULT_NUMERIC;	
	//btn.poPosition.x		= 500;
	//btn.poPosition.y		= 160;
	//btn.pcBmpUp				= m_pcTrendToUSB_Up;
	//btn.pcBmpDown			= m_pcTrendToUSB_Dw;
	//btn.pcBmpFocus			= m_pcTrendToUSB_Up;
	//btn.pcBmpDisabled		= m_pcTrendToUSB_Up;
	//btn.dwFormat			= DT_VCENTER|DT_CENTER;
	//m_pcNumDefault=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	//m_pcNumDefault->Create(this,g_hf14AcuMed,0);
	//m_pcNumDefault->SetText(_T("set default"));
	////m_pcNumDefault->SetText(getModel()->GetLanguageString(IDS_TXT_SAVE));
	////m_pcNumDefault->ShowWindow(SW_SHOW);
	//m_pcNumDefault->ShowWindow(SW_HIDE);


	btn.wID					= IDC_BTN_LOGFILES;	
	btn.poPosition.x		= 470;
	btn.poPosition.y		= 157;//267;
	btn.pcBmpUp				= m_pcTrendToUSB_Up;
	btn.pcBmpDown			= m_pcTrendToUSB_Dw;
	btn.pcBmpFocus			= m_pcTrendToUSB_Up;
	btn.pcBmpDisabled		= m_pcTrendToUSB_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcLogToUSB=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcLogToUSB->Create(this,g_hf14AcuMed,0);
	m_pcLogToUSB->SetText(getModel()->GetLanguageString(IDS_TXT_SAVE));
	m_pcLogToUSB->ShowWindow(SW_SHOW);
	

	//m_pcDevInfoToUSB
	btn.wID					= IDC_BTN_DEVICEINFO;	
	btn.poPosition.x		= 470;
	btn.poPosition.y		= 257;
	btn.pcBmpUp				= m_pcTrendToUSB_Up;
	btn.pcBmpDown			= m_pcTrendToUSB_Dw;
	btn.pcBmpFocus			= m_pcTrendToUSB_Up;
	btn.pcBmpDisabled		= m_pcTrendToUSB_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcDevInfoToUSB=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcDevInfoToUSB->Create(this,g_hf14AcuMed,0);
	m_pcDevInfoToUSB->SetText(getModel()->GetLanguageString(IDS_TXT_SAVE));
	m_pcDevInfoToUSB->ShowWindow(SW_SHOW);



	if(m_bSavingTrendToUSB)
	{
		if(GetParent())
			GetParent()->PostMessage(WM_KILL_SETUPTIMER);

		eventWaitTrendUSBfinish.ResetEvent();

		CreateWndHourglass();
		ShowWndHourglass(true);

		StartWaitTrendUSBThread();
	}
	else
	{
		SetTimer(USBCHECKTIMER,2000,NULL);
		if(GetParent())
			GetParent()->PostMessage(WM_SET_SETUPTIMER);
	}

}

/**********************************************************************************************//**
 * Shows
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	bShow	True to show, false to hide.
 **************************************************************************************************/

void CSubViewTools::Show(bool bShow)
{
	if(bShow)
	{
		ShowWindow(SW_SHOW);
		//SetWindowPos(NULL,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_SHOWWINDOW);


		Draw();

		if(GetParent())
			GetParent()->PostMessage(WM_SET_SETUPTIMER);
	}
	else
	{

		ShowWindow(SW_HIDE);
		//SetWindowPos(NULL,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_HIDEWINDOW);
	}
}

/**********************************************************************************************//**
 * Executes the destroy action
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CSubViewTools::OnDestroy()
{
	m_bExit=true;

	StopWaitTrendUSBThread();
	if(m_pcwtWaitTrendUSBThread!=NULL)
	{
		delete m_pcwtWaitTrendUSBThread;
		m_pcwtWaitTrendUSBThread=NULL;

		if(m_hThreadWaitTrendUSB!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadWaitTrendUSB);
			m_hThreadWaitTrendUSB=INVALID_HANDLE_VALUE;
		}
	}
	KillTimer(USBCHECKTIMER);


	if(m_pDlg)
	{
		m_pDlg->EndDialog(IDCLOSE);
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_REDRAW_VIEW);
	}
	m_pDlg = NULL;

	DestroyWndHourglass();

	CWnd::OnDestroy();

	if(m_hBmp)
	{
		SelectObject(m_hDC,m_hBmpPrev);
		DeleteObject(m_hBmp);
	}
	if(m_hDC)
		DeleteDC(m_hDC);
}

/**********************************************************************************************//**
 * Draws this instance
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CSubViewTools::Draw()
{
	RECT rcCl;
	RECT rc;
	GetClientRect(&rcCl);

	CClientDC dc(this);
	HDC hdcMem=CreateCompatibleDC(dc.m_hDC);
	HBITMAP hBmpMem=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
	HBITMAP hBmpMemPrev=(HBITMAP)SelectObject(hdcMem,hBmpMem);



	CBrush cbrBack(BACKGND);
	HBRUSH hbrprev=(HBRUSH)SelectObject(hdcMem,cbrBack);
	HPEN hpenprev=(HPEN)SelectObject(hdcMem, (HPEN)GetStockObject(NULL_PEN));	
	int nBkMode=SetBkMode(hdcMem,TRANSPARENT);
	HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf14AcuMed);
	int tc=SetTextColor(hdcMem,0x0000000);

	Rectangle(hdcMem, 0, 0, m_lX, m_lY);

	CBrush cbrFill;
	cbrFill.CreateSolidBrush(RGB(181,178,181));
	SelectObject(hdcMem,cbrFill);

	Rectangle(hdcMem, 100, 30, 680, 465);
	Rectangle(hdcMem, 120, 10, 660, 30);
	Rectangle(hdcMem, 120, 465, 660, 485);
	if(m_pcRegionTopLeCorner)
		m_pcRegionTopLeCorner->Draw(hdcMem,100,10);
	if(m_pcRegionTopRiCorner)
		m_pcRegionTopRiCorner->Draw(hdcMem,660,10);
	if(m_pcRegionBotLeCorner)
		m_pcRegionBotLeCorner->Draw(hdcMem,100,465);
	if(m_pcRegionBotRiCorner)
		m_pcRegionBotRiCorner->Draw(hdcMem,660,465);

	

	CPen penLine;
	penLine.CreatePen(PS_SOLID,1,RGB(140,140,140));
	SelectObject(hdcMem,(HPEN)penLine);
	CBrush cbrRound(RGB(200,200,200));
	CBrush cbrDarkRound(RGB(140,140,140));
	SelectObject(hdcMem,cbrRound);
	
	/**********************trend usb******************************/
	RoundRect(hdcMem, 110, 20, 670, 110,20,20);
	SelectObject(hdcMem,cbrDarkRound);
	RoundRect(hdcMem, 110, 20, 670, 50,20,20);
	SetTextColor(hdcMem,RGB(255,255,255));

	rc.left = 130;  
	rc.top = 25;  
	rc.right  = 420;  
	rc.bottom = 500;
	//cs = _T("Trends:");
	CStringW cs = getModel()->GetLanguageString(IDS_MENU_TREND);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	SetTextColor(hdcMem,0x0000000);

	rc.left = 130;  
	rc.top = 68;  
	rc.right  = 420;  
	rc.bottom = 500;
	//cs = _T("Save trends to USB:");
	cs = getModel()->GetLanguageString(IDS_TREND_SAVETOUSB);
	//DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	if(m_bUSBavailable && getModel()->getDATAHANDLER()->isTRENDLicenseAvailable())
	{
		m_pcTrendToUSB->ShowWindow(SW_SHOW);
	}
	else
	{

		m_pcTrendToUSB->ShowWindow(SW_HIDE);

		rc.left = 440;  
		rc.top = 68;  
		rc.right  = 660;  
		rc.bottom = 500; 
		//cs=_T("not available");
		cs=getModel()->GetLanguageString(IDS_TXT_USBNOTAVAILABLE);
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
	}


	/**********************Personalization******************************/
	//SelectObject(hdcMem,cbrRound);
	//RoundRect(hdcMem, 110, 120, 670, 220,20,20);

	//SelectObject(hdcMem,cbrDarkRound);

	//RoundRect(hdcMem, 110, 120, 670, 150,20,20);

	//SetTextColor(hdcMem,RGB(255,255,255));

	//rc.left = 0;  
	//rc.left = 130;  
	//rc.top = 125;  
	//rc.right  = 420;  
	//rc.bottom = 500;
	////cs = _T("Personalization:");
	//cs = getModel()->GetLanguageString(IDS_TXT_PERSONALIZATION);
	//DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	//SetTextColor(hdcMem,0x0000000);

	//rc.left = 0;  
	//rc.left = 130;  
	//rc.top = 170;  
	//rc.right  = 420;  
	//rc.bottom = 500;
	////cs = _T("Numerics:");
	//cs = getModel()->GetLanguageString(IDS_TXT_NUMERICBLOCKS);
	//DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	//rc.left = 440;  
	//rc.top = 170;  
	//rc.right  = 660;  
	//rc.bottom = 500; 
	//cs=getModel()->GetLanguageString(IDS_TXT_NOTAVAILABLE);
	//DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	/**********************log usb******************************/
	SelectObject(hdcMem,cbrRound);
	RoundRect(hdcMem, 110, 120/*230*/, 670, 210/*320*/,20,20);

	SelectObject(hdcMem,cbrDarkRound);
	RoundRect(hdcMem, 110, 120/*230*/, 670, 150/*260*/,20,20);
	
	SetTextColor(hdcMem,RGB(255,255,255));

	rc.left = 130;  
	rc.top = 125;//235;  
	rc.right  = 420;  
	rc.bottom = 500;
	cs = getModel()->GetLanguageString(IDS_TXT_LOGFILE);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	SetTextColor(hdcMem,0x0000000);

	rc.left = 130;  
	rc.top = 168;//278;  
	rc.right  = 420;  
	rc.bottom = 500;
	cs = getModel()->GetLanguageString(IDS_TXT_LOGUSB);
	//DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	if(m_bUSBavailable)
	{
		m_pcLogToUSB->ShowWindow(SW_SHOW);
	}
	else
	{

		m_pcLogToUSB->ShowWindow(SW_HIDE);

		rc.left = 440;  
		rc.top = 168;//278;  
		rc.right  = 660;  
		rc.bottom = 500; 
		//cs=_T("not available");
		cs=getModel()->GetLanguageString(IDS_TXT_USBNOTAVAILABLE);
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
	}

	/**********************deviceinfo usb******************************/
	SelectObject(hdcMem,cbrRound);
	RoundRect(hdcMem, 110, 220, 670, 310,20,20);

	SelectObject(hdcMem,cbrDarkRound);
	RoundRect(hdcMem, 110, 220, 670, 250,20,20);

	SetTextColor(hdcMem,RGB(255,255,255));

	rc.left = 130;  
	rc.top = 225; 
	rc.right  = 420;  
	rc.bottom = 500;
	cs = getModel()->GetLanguageString(IDS_TXT_DEVICEINFO);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	SetTextColor(hdcMem,0x0000000);

	rc.left = 130;  
	rc.top = 268;
	rc.right  = 420;  
	rc.bottom = 500;
	cs = getModel()->GetLanguageString(IDS_TXT_SAVEDEVICEINFO);
	//DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	if(m_bUSBavailable)
	{
		m_pcDevInfoToUSB->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pcDevInfoToUSB->ShowWindow(SW_HIDE);

		rc.left = 440;  
		rc.top = 268; 
		rc.right  = 660;  
		rc.bottom = 500; 
		//cs=_T("not available");
		cs=getModel()->GetLanguageString(IDS_TXT_USBNOTAVAILABLE);
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
	}

	//dc.BitBlt(0,0,m_lX,m_lY,CDC::FromHandle(hdcMem),0,0,SRCCOPY);
	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,hdcMem,0,0,SRCCOPY);

	SetTextColor(hdcMem,tc);
	SetBkMode(hdcMem,nBkMode);

	SelectObject(hdcMem,hbrprev);
	SelectObject(hdcMem,hpenprev);

	penLine.DeleteObject();

	SelectObject(hdcMem,hBmpMemPrev);
	SelectObject(hdcMem,hPrevFont);
	DeleteObject(hBmpMem);
	DeleteDC(hdcMem);

}

/**********************************************************************************************//**
 * Executes the timer action
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	nIDEvent	The identifier event.
 **************************************************************************************************/

void CSubViewTools::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==USBCHECKTIMER)
	{
		bool bRedraw=false;
		if(CTlsFile::Exists(_T("\\Hard Disk")))
		{
			if(m_bUSBavailable == false)
				bRedraw=true;
			m_bUSBavailable=true;
		}
		else
		{
			if(m_bUSBavailable == true)
				bRedraw=true;
			m_bUSBavailable=false;
		}


		if(bRedraw)
			Draw();

	}

	CWnd::OnTimer(nIDEvent);
}

/**********************************************************************************************//**
 * Executes the button clicked log to USB action
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CSubViewTools::OnBnClickedLogToUSB()
{
	if(GetParent())
		GetParent()->PostMessage(WM_KILL_SETUPTIMER);
	KillTimer(USBCHECKTIMER);

	CDlgMessageBox box(this,getModel()->GetLanguageString(IDS_TXT_LOGFILE),getModel()->GetLanguageString(IDS_TXT_SAVELOG),MB_YESNO,IDB_MSG_USB);
	m_pDlg = &box;
	int iRes=box.DoModal();
	m_pDlg = NULL;

	if(iRes==IDCLOSE)
	{
		return;
	}

	if(iRes==IDYES && !m_bExit)
	{
		getModel()->getDATAHANDLER()->getMainboardData(&m_dataMainboard);
		CString szMessage=_T("");
		
			
		CString szVersion=_T("Version Controller PIC: ");
		szVersion+=m_dataMainboard.m_szConductorVersion;
		theApp.getLog()->WriteLine(szVersion);

		szVersion.Format(_T("Version Monitor PIC: %d.%d.%d"), 
			m_dataMainboard.m_iMonitorVersion_x, 
			m_dataMainboard.m_iMonitorVersion_y, 
			m_dataMainboard.m_iMonitorVersion_z);
		theApp.getLog()->WriteLine(szVersion);
		
		szVersion=_T("Version fabianHFO: ");
		szVersion+=getModel()->GetBuildVersion();
		theApp.getLog()->WriteLine(szVersion);

		szVersion=_T("Version Languagefiles: ");
		szVersion+=getModel()->GetLanguageString(IDS_VERSION_LANGUAGE);
		theApp.getLog()->WriteLine(szVersion);

		//set flag that log will be copied
		theApp.getLog()->WriteLine(_T("**** copy logfile to USB via menu ****"));

		getModel()->getALARMHANDLER()->setLogTxt(_T("**** copy logfile to USB via menu ****"));
		

		bool bErrorCopy=false;
		bool bSDavailable=false;
		CString sPathFromSD = _T("\\sdcard\\");
		if(CTlsFile::Exists(sPathFromSD)==true)
		{
			bSDavailable=true;
			sPathFromSD = _T("\\sdcard\\Logs");
		}

		CString sPathFromFFSDISK = _T("\\FFSDISK\\");
		if(CTlsFile::Exists(sPathFromFFSDISK)==true)
		{
			sPathFromFFSDISK = _T("\\FFSDISK\\Logs");
		}

		
		CString sLogPathToFFSDISK = _T("\\Hard Disk\\") ;
		sLogPathToFFSDISK+=getModel()->GetUniqueID();
		sLogPathToFFSDISK+=_T("-Logs") ;
		if(!CTlsFile::Exists(sLogPathToFFSDISK))
		{
			CTlsFile::MkDir(sLogPathToFFSDISK);
		}
		sLogPathToFFSDISK+=_T("\\FFSDISK");
		if(!CTlsFile::Exists(sLogPathToFFSDISK))
		{
			CTlsFile::MkDir(sLogPathToFFSDISK);
		}
		sLogPathToFFSDISK+=_T("\\");

		Sleep(0);

		CString sLogPathToSD = _T("\\Hard Disk\\") ;
		if(bSDavailable)
		{
			sLogPathToSD+=getModel()->GetUniqueID();
			sLogPathToSD+=_T("-Logs\\SDCARD");
			if(!CTlsFile::Exists(sLogPathToSD))
			{
				CTlsFile::MkDir(sLogPathToSD);
			}
			sLogPathToSD+=_T("\\");
		}
		

		WIN32_FIND_DATA FileData;
		HANDLE hSearch = ::FindFirstFile(sPathFromFFSDISK + _T("\\*.*"), &FileData);
		//copy logs from FFSDISK
		if(hSearch != INVALID_HANDLE_VALUE)
		{
			do
			{
				CString sFile = sPathFromFFSDISK + _T("\\") + FileData.cFileName;
				CString sNewFile = sLogPathToFFSDISK;
				sNewFile+= FileData.cFileName;

				szMessage=_T("copy file: ");
				szMessage+=FileData.cFileName;
				

				if (CTlsFile::Copy(sFile, sNewFile , true)==false)
				{
					bErrorCopy=true;
				}
				Sleep(0);
			} while(::FindNextFile(hSearch, &FileData));

			FindClose(hSearch);
		}

		//copy logs from SD
		if(bSDavailable)
		{
			hSearch = ::FindFirstFile(sPathFromSD + _T("\\*.*"), &FileData);
			if(hSearch != INVALID_HANDLE_VALUE)
			{
				do
				{
					CString sFile = sPathFromSD + _T("\\") + FileData.cFileName;
					CString sNewFile = sLogPathToSD;
					sNewFile+= FileData.cFileName;

					szMessage=_T("copy file: ");
					szMessage+=FileData.cFileName;


					if (CTlsFile::Copy(sFile, sNewFile , true)==false)
					{
						bErrorCopy=true;
					}
					Sleep(0);
				} while(::FindNextFile(hSearch, &FileData));

				FindClose(hSearch);
			}
		}
		
		

		if(bErrorCopy)
		{
			theApp.getLog()->WriteLine(_T("#HFO:0263"));
			getModel()->getALARMHANDLER()->setLogTxt(_T("ERROR: could not copy file"));
		}
		else
		{
			getModel()->getALARMHANDLER()->setLogTxt(_T("SUCCESS: files copied"));

			CMVEventInfotext event(CMVEventInfotext::EV_TIMETEXT, getModel()->GetLanguageString(IDS_TXT_FILESCOPIED), 3500);
			getModel()->triggerEvent(&event);

			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
		}

	}

	if(!m_bExit)
	{
		Draw();

		UpdateWindow();
		Invalidate(TRUE);

		SetTimer(USBCHECKTIMER,2000,NULL);
		if(GetParent())
			GetParent()->PostMessage(WM_SET_SETUPTIMER);
	}
}

/**********************************************************************************************//**
 * Executes the button clicked trend to USB action
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CSubViewTools::OnBnClickedTrendToUSB()
{
	if(GetParent())
		GetParent()->PostMessage(WM_KILL_SETUPTIMER);
	KillTimer(USBCHECKTIMER);
	
	//CDlgMessageBox box(this,_T("Trends"),_T("Do you want to save the trend data to USB? Do not disconnect the USB stick while saving the trend data!"),MB_YESNO,IDB_MSG_USB);
	CDlgMessageBox box(this,getModel()->GetLanguageString(IDS_MENU_TREND),getModel()->GetLanguageString(IDS_TREND_MSGSAVEUSB),MB_YESNO,IDB_MSG_USB);
	m_pDlg = &box;
	int iRes=box.DoModal();
	m_pDlg = NULL;

	if(iRes==IDCLOSE)
	{
		return;
	}

	if(iRes==IDYES && !m_bExit)
	{
		eventWaitTrendUSBfinish.ResetEvent();

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TEXT_SAVINGTRENDS);

		CreateWndHourglass();
		ShowWndHourglass(true);

		StartWaitTrendUSBThread();
		
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_SAVETREND_TOUSB);
	}

	if(!m_bExit)
	{
		Draw();

		UpdateWindow();
		Invalidate(TRUE);

		/*SetTimer(USBCHECKTIMER,2000,NULL);
		if(GetParent())
		GetParent()->PostMessage(WM_SET_SETUPTIMER);*/
	}
	
}

/**********************************************************************************************//**
 * Executes the button clicked development information to USB action
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CSubViewTools::OnBnClickedDevInfoToUSB()
{
	if(GetParent())
		GetParent()->PostMessage(WM_KILL_SETUPTIMER);
	KillTimer(USBCHECKTIMER);

	CDlgMessageBox box(this,getModel()->GetLanguageString(IDS_TXT_DEVICEINFO),getModel()->GetLanguageString(IDS_TXT_SAVEINFO),MB_YESNO,IDB_MSG_USB);
	m_pDlg = &box;
	int iRes=box.DoModal();
	m_pDlg = NULL;

	if(iRes==IDCLOSE)
	{
		return;
	}

	if(iRes==IDYES && !m_bExit)
	{
		CString szToFile=_T("");

		CString sFile=_T("\\Hard Disk\\");
		sFile+=getModel()->GetUniqueID();
		sFile+=_T("-DeviceInfo.txt");

		if(CTlsFile::Exists(sFile))
		{
			CTlsFile::Delete(sFile);
		}

		CTlsLogfile * lfDeviceInfoFile = new CTlsLogfile(sFile);

		//**************************System Informations********************************
		SYSTEMTIME t;
		GetLocalTime(&t);

		CStringW dtStamp=_T("");
		dtStamp.Format(TEXT("%04d.%02d.%02d %02d:%02d:%02d"),t.wYear,t.wMonth,t.wDay, t.wHour, t.wMinute, t.wSecond);
		lfDeviceInfoFile->WriteLine(dtStamp,false);
		lfDeviceInfoFile->WriteLine(_T("System Informations:"),false);

		CString szID=_T("");
		szID+=getModel()->GetUniqueID();
		szToFile.Format(_T("- Device-ID: %s"), szID);
		lfDeviceInfoFile->WriteLine(szToFile,false);

		CString szTemp=_T("");
		BYTE iMainVersion=getModel()->getCONFIG()->GetMainBoardVersion();
		int iMajor=(iMainVersion >> 4) & 0x000F;
		int iMinor=iMainVersion & 0x000F;
		szToFile.Format(_T("- HW configuration: MB%d.%d,"), iMajor,iMinor);
		if(getModel()->getDATAHANDLER()->IsLEDdisplay())
		{
			szToFile+=_T("LED,");
		}
		else
		{
			szToFile+=_T("LCD,");
		}
		szTemp.Format(_T("SysHW%d,"), getModel()->getCONFIG()->getSystemHWrevision());
		szToFile+=szTemp;

		eDevType devType=getModel()->getCONFIG()->getDevType();
		if(devType==DEV_EVO)
		{
			szToFile+=_T("EVO,");
		}
		else if(devType==DEV_THERAPY)
		{
			szToFile+=_T("THERAPY,");
		}
		else if(devType==DEV_HFO)
		{
			szToFile+=_T("HFO,");
		}

		eFlowSensorType flowSensorType=getModel()->getCONFIG()->getFlowSensorType();
		if(flowSensorType==FSENS_NEONATAL)
		{
			szToFile+=_T("FlowS:Neo,");
		}
		else if(flowSensorType==FSENS_PEDIATRIC)
		{
			szToFile+=_T("FlowS:Ped,");
		}

		eAVRevision avRevision=getModel()->getCONFIG()->getAVRevision();
		if(avRevision==AVREV_1)
		{
			szToFile+=_T("ATG:Rev.1-3,");
		}
		else if(avRevision==AVREV_4)
		{
			szToFile+=_T("ATG:Rev.4,");
		}

		eHFModuleRevision hfRevision=getModel()->getCONFIG()->getHFModuleRevision();
		if(hfRevision==HFREV_1)
		{
			szToFile+=_T("HF:Type1,");
		}
		else if(hfRevision==HFREV_2)
		{
			szToFile+=_T("HF:Type2,");
		}
		lfDeviceInfoFile->WriteLine(szToFile,false);

		
		BYTE iPDMSProtocol=getModel()->getCONFIG()->GetPDMSprotocol();
		if(iPDMSProtocol==ACL_SERIAL_ASCII)
			szTemp = _T("Serial (no wave data)");
		else if(iPDMSProtocol==ACL_SERIAL_WAVE)
			szTemp = _T("Serial (with wave data)");
		else if(iPDMSProtocol==ACL_ETHERNET_WAVE)
			szTemp = _T("Ethernet (with wave data)");
		else if(iPDMSProtocol==ACL_ETHERNET)
			szTemp = _T("Ethernet (no wave data)");
		else if(iPDMSProtocol==ACL_SERIAL_IVOI)
			szTemp = _T("VueLink/IntelliBridge");
		else if(iPDMSProtocol==ACL_TERMINAL_REMOTE)
			szTemp = _T("Terminal Remote");
		else if(iPDMSProtocol==ACL_TERMINAL_WAVE)
			szTemp = _T("Terminal Wave");
		else
			szTemp = _T("--");

		szToFile.Format(_T("- PDMS protocol: %s"), szTemp);
		lfDeviceInfoFile->WriteLine(szToFile,false);

		//**************************SW Versions********************************
		lfDeviceInfoFile->WriteLine(_T(""),false);
		lfDeviceInfoFile->WriteLine(_T("SW Versions:"),false);

		szToFile.Format(_T("- GUI: %s"), getModel()->GetMainVersion());
		lfDeviceInfoFile->WriteLine(szToFile,false);

		if(iPDMSProtocol!=ACL_NOPDMS)
		{
			szTemp=getModel()->getAcuLink()->getExeVersion();
			szTemp+= _T(" / ");
			szTemp+=getModel()->getAcuLink()->getDllVersion();
			szToFile.Format(_T("- ACULINK version: %s"), szTemp);
			lfDeviceInfoFile->WriteLine(szToFile,false);
		}
		else
		{
			lfDeviceInfoFile->WriteLine(_T("- ACULINK version: none"),false);
		}

		getModel()->getDATAHANDLER()->getMainboardData(&m_dataMainboard);
		szToFile.Format(_T("- Monitor PIC: %d.%d.%d, Checksum: %X"), 
			m_dataMainboard.m_iMonitorVersion_x, 
			m_dataMainboard.m_iMonitorVersion_y, 
			m_dataMainboard.m_iMonitorVersion_z, 
			m_dataMainboard.m_iMonitorChecksum);
		lfDeviceInfoFile->WriteLine(szToFile,false);

		szToFile.Format(_T("- Conductor PIC: %s, Checksum: %X"), 
			m_dataMainboard.m_szConductorVersion, 
			m_dataMainboard.m_iConductorChecksum);
		lfDeviceInfoFile->WriteLine(szToFile,false);

		WORD wACCU_DAT_VERSION=0;
		WORD wACCU_DAT_CHECKSUM=0;
		if(getModel()->getI2C()->IsI2C_ACCUavailability())
		{
			wACCU_DAT_VERSION=getModel()->getI2C()->ReadAccuWord(ACCU_DAT_VERSION);
			wACCU_DAT_CHECKSUM=getModel()->getI2C()->ReadAccuWord(ACCU_DAT_CHECKSUM);
		}
		
		int iXVal=HIBYTE(wACCU_DAT_VERSION);
		int iYVal=LOBYTE(wACCU_DAT_VERSION);
		szToFile.Format(_T("- Accu PIC: %d.%d, Checksum: %X"), iXVal, iYVal,wACCU_DAT_CHECKSUM);
		lfDeviceInfoFile->WriteLine(szToFile,false);

		szToFile.Format(_T("- HFO PIC: %s"), m_dataMainboard.m_szHfoVersion);
		lfDeviceInfoFile->WriteLine(szToFile,false);

		szToFile.Format(_T("- Blender PIC: %s"), m_dataMainboard.m_szBlenderVersion);
		lfDeviceInfoFile->WriteLine(szToFile,false);

		szToFile.Format(_T("- Kernel: %s %s %s"), 
			getModel()->getCONFIG()->GetKernelVersion(),
			getModel()->getCONFIG()->GetKernelVersionDate(), 
			getModel()->getCONFIG()->GetKernelAcuVersion());
		lfDeviceInfoFile->WriteLine(szToFile,false);

		szToFile.Format(_T("- Language: %s"), getModel()->GetLanguageString(IDS_VERSION_LANGUAGE));
		lfDeviceInfoFile->WriteLine(szToFile,false);

		//**************************Operating time********************************
		lfDeviceInfoFile->WriteLine(_T(""),false);
		lfDeviceInfoFile->WriteLine(_T("Operating time:"),false);

		//- device: XY hrs XY min
		DWORD iOpMinDev=getModel()->getDATAHANDLER()->getOpTimeDeviceMin(false);
		int iOpHrsDev = iOpMinDev/60;
		if(iOpHrsDev>0)
			iOpMinDev=iOpMinDev-(iOpHrsDev*60);
		szToFile.Format(_T("- device: %d hrs %d min"),iOpHrsDev,iOpMinDev);
		lfDeviceInfoFile->WriteLine(szToFile,false);

		//	- battery: XY hrs XY min
		DWORD iOpMinBatt=getModel()->getDATAHANDLER()->getOpTimeBatteryMin(false);
		int iOpHrsBatt = iOpMinBatt/60;
		if(iOpHrsBatt>0)
			iOpMinBatt=iOpMinBatt-(iOpHrsBatt*60);
		szToFile.Format(_T("- battery: %d hrs %d min"),iOpHrsBatt,iOpMinBatt);
		lfDeviceInfoFile->WriteLine(szToFile,false);

		//	- HFO modul: XY hrs XY min
		DWORD iOpMinHFO=getModel()->getDATAHANDLER()->getOpTimeHFOMin(false);
		int iOpHrsHFO = iOpMinHFO/60;
		if(iOpHrsHFO>0)
			iOpMinHFO=iOpMinHFO-(iOpHrsHFO*60);
		szToFile.Format(_T("- HFO modul: %d hrs %d min"),iOpHrsHFO,iOpMinHFO);
		lfDeviceInfoFile->WriteLine(szToFile,false);

		//**************************Licenses********************************
		lfDeviceInfoFile->WriteLine(_T(""),false);
		lfDeviceInfoFile->WriteLine(_T("Licenses:"),false);

		CStringW szLicenseFile=_T("ML");
		szLicenseFile+=getModel()->GetUniqueID();
		szLicenseFile+=_T(".mlic");

		CStringW szFile=_T("");

		szFile=_T("\\FFSDISK\\")+szLicenseFile;
		

		bool bAcuLinkLicense=false;
		if(CTlsFile::Exists(szFile))
		{
			bAcuLinkLicense=true;
		}
		//- Aculink: Yes/No
		if(bAcuLinkLicense)
			lfDeviceInfoFile->WriteLine(_T("- AcuLink: yes"),false);
		else
			lfDeviceInfoFile->WriteLine(_T("- AcuLink: no"),false);

		if(getModel()->getCONFIG()->GetAcuLinkVersion()==ALINKVERS_3)
		{
			lfDeviceInfoFile->WriteLine(_T("- AcuLink: version 3.x"),false);
		}
		else
		{
			lfDeviceInfoFile->WriteLine(_T("- AcuLink: version 4.x"),false);
		}

		//- HFO: Yes/No
		if(getModel()->getDATAHANDLER()->isHFOLicenseAvailable())
		{
			szToFile=_T("- HFO: ");
			CStringA szEncryptedHFOKey=getModel()->getDATAHANDLER()->encryptKey(MOD_HFO);
			szToFile+=getModel()->getDATAHANDLER()->getFormattedEncryptKey(szEncryptedHFOKey);

			lfDeviceInfoFile->WriteLine(szToFile,false);
		}
		else
			lfDeviceInfoFile->WriteLine(_T("- HFO: no"),false);

		//- NMode: Yes/No
		if(getModel()->getDATAHANDLER()->isNMODELicenseAvailable())
		{
			szToFile=_T("- NMode: ");
			CStringA szEncryptedNMODEKey=getModel()->getDATAHANDLER()->encryptKey(MOD_NMODE);
			szToFile+=getModel()->getDATAHANDLER()->getFormattedEncryptKey(szEncryptedNMODEKey);

			lfDeviceInfoFile->WriteLine(szToFile,false);
		}
		else
			lfDeviceInfoFile->WriteLine(_T("- NMode: no"),false);

		//- VG: Yes/No
		if(getModel()->getDATAHANDLER()->isVGUARANTLicenseAvailable())
		{
			szToFile=_T("- VG: ");
			CStringA szEncryptedVGUARANTYKey=getModel()->getDATAHANDLER()->encryptKey(MOD_VGUARANTY);
			szToFile+=getModel()->getDATAHANDLER()->getFormattedEncryptKey(szEncryptedVGUARANTYKey);

			lfDeviceInfoFile->WriteLine(szToFile,false);
		}
		else
			lfDeviceInfoFile->WriteLine(_T("- VG: no"),false);

		//- VL: Yes/No
		if(getModel()->getDATAHANDLER()->isVLIMITLicenseAvailable())
		{
			szToFile=_T("- VL: ");
			CStringA szEncryptedVLIMITKey=getModel()->getDATAHANDLER()->encryptKey(MOD_VLIMIT);
			szToFile+=getModel()->getDATAHANDLER()->getFormattedEncryptKey(szEncryptedVLIMITKey);

			lfDeviceInfoFile->WriteLine(szToFile,false);
		}
		else
			lfDeviceInfoFile->WriteLine(_T("- VL: no"),false);

		//- Lung recruitment: Yes/No
		if(getModel()->getDATAHANDLER()->isLUNGRECLicenseAvailable())
		{
			szToFile=_T("- Lung recruitment: ");
			CStringA szEncryptedLUNGRECKey=getModel()->getDATAHANDLER()->encryptKey(MOD_LUNGREC);
			szToFile+=getModel()->getDATAHANDLER()->getFormattedEncryptKey(szEncryptedLUNGRECKey);

			lfDeviceInfoFile->WriteLine(szToFile,false);
		}
		else
			lfDeviceInfoFile->WriteLine(_T("- Lung recruitment: no"),false);

		//- Trends: Yes/No
		if(getModel()->getDATAHANDLER()->isTRENDLicenseAvailable())
		{
			szToFile=_T("- Trends: ");
			CStringA szEncryptedTRENDSKey=getModel()->getDATAHANDLER()->encryptKey(MOD_TREND);
			szToFile+=getModel()->getDATAHANDLER()->getFormattedEncryptKey(szEncryptedTRENDSKey);

			lfDeviceInfoFile->WriteLine(szToFile,false);
		}
		else
			lfDeviceInfoFile->WriteLine(_T("- Trends: no"),false);

		//- O2-Therapy: Yes/No
		if(getModel()->getDATAHANDLER()->isTHERAPYLicenseAvailable())
		{
			szToFile=_T("- O2-Therapy: ");
			CStringA szEncryptedTHERAPYKey=getModel()->getDATAHANDLER()->encryptKey(MOD_THERAPY);
			szToFile+=getModel()->getDATAHANDLER()->getFormattedEncryptKey(szEncryptedTHERAPYKey);

			lfDeviceInfoFile->WriteLine(szToFile,false);
		}
		else
			lfDeviceInfoFile->WriteLine(_T("- O2-Therapy: no"),false);

		//- PRICO: Yes/No
		if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable())
		{
			szToFile=_T("- PRICO: ");
			CStringA szEncryptedPRICOKey=getModel()->getDATAHANDLER()->encryptKey(MOD_PRICO);
			szToFile+=getModel()->getDATAHANDLER()->getFormattedEncryptKey(szEncryptedPRICOKey);

			lfDeviceInfoFile->WriteLine(szToFile,false);
		}
		else
			lfDeviceInfoFile->WriteLine(_T("- PRICO: no"),false);

		//- FOT: Yes/No
		if(getModel()->getDATAHANDLER()->isFOTLicenseAvailable())
		{
			szToFile=_T("- FOT: ");
			CStringA szEncryptedFOTKey=getModel()->getDATAHANDLER()->encryptKey(MOD_FOT);
			szToFile+=getModel()->getDATAHANDLER()->getFormattedEncryptKey(szEncryptedFOTKey);

			lfDeviceInfoFile->WriteLine(szToFile,false);
		}
		else
			lfDeviceInfoFile->WriteLine(_T("- FOT: no"),false);

		//NIV trigger
		if(getModel()->getDATAHANDLER()->isNIVTRIGGERLicenseAvailable())
		{
			szToFile=_T("- NIVTRIGGER: ");
			CStringA szEncryptedNIVTRIGGERKey=getModel()->getDATAHANDLER()->encryptKey(MOD_NIVTRIGGER);
			szToFile+=getModel()->getDATAHANDLER()->getFormattedEncryptKey(szEncryptedNIVTRIGGERKey);

			lfDeviceInfoFile->WriteLine(szToFile,false);
		}
		else
			lfDeviceInfoFile->WriteLine(_T("- NIVTRIGGER: no"),false);


		//**************************Service Date********************************
		lfDeviceInfoFile->WriteLine(_T(""),false);
		lfDeviceInfoFile->WriteLine(_T("Next service date:"),false);

		COleDateTime dtNextServiceDate = getModel()->getCONFIG()->getNextServiceDate();
		
		bool bServiceDateOn=false;
		if(dtNextServiceDate.GetStatus() == COleDateTime::valid)
		{
			bServiceDateOn=true;
		}

		if(bServiceDateOn)
		{
			szToFile.Format(TEXT("- %04d.%02d.%02d"),dtNextServiceDate.GetYear(),dtNextServiceDate.GetMonth(),dtNextServiceDate.GetDay());
			lfDeviceInfoFile->WriteLine(szToFile,false);
		}
		else
		{
			lfDeviceInfoFile->WriteLine(_T("- off"),false);
		}
		

		


		/*CString szVersion=_T("Version Controller PIC: ");
		szVersion+=m_dataMainboard.m_szConductorVersion;
		theApp.getLog()->WriteLine(szVersion);

		

		szVersion=_T("Version fabianHFO: ");
		szVersion+=getModel()->GetBuildVersion();
		theApp.getLog()->WriteLine(szVersion);

		szVersion=_T("Version Languagefiles: ");
		szVersion+=getModel()->GetLanguageString(IDS_VERSION_LANGUAGE);
		theApp.getLog()->WriteLine(szVersion);*/

		if(lfDeviceInfoFile)
		{
			delete lfDeviceInfoFile;
		}
		lfDeviceInfoFile=NULL;




		szTemp.Format(_T("- %s -"), getModel()->GetLanguageString(IDS_TXT_FILEWRITTEN));
		CMVEventInfotext event(CMVEventInfotext::EV_TIMETEXT, szTemp, 3500);
		getModel()->triggerEvent(&event);

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);




		



	}

	if(!m_bExit)
	{
		Draw();

		UpdateWindow();
		Invalidate(TRUE);

		SetTimer(USBCHECKTIMER,2000,NULL);
		if(GetParent())
			GetParent()->PostMessage(WM_SET_SETUPTIMER);
	}
}

/**********************************************************************************************//**
 * Starts wait trend USB thread
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CSubViewTools::StartWaitTrendUSBThread( void )
{
	m_bDoWaitTrendUSBThread=true;

	if(m_pcwtWaitTrendUSBThread!=NULL)
	{
		delete m_pcwtWaitTrendUSBThread;
		m_pcwtWaitTrendUSBThread=NULL;

		if(m_hThreadWaitTrendUSB!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadWaitTrendUSB);
			m_hThreadWaitTrendUSB=INVALID_HANDLE_VALUE;
		}
	}

	m_pcwtWaitTrendUSBThread=AfxBeginThread(CWaitTrendUSBThread,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	m_hThreadWaitTrendUSB=m_pcwtWaitTrendUSBThread->m_hThread;
	m_pcwtWaitTrendUSBThread->m_bAutoDelete = FALSE; 
	m_pcwtWaitTrendUSBThread->ResumeThread();
}

/**********************************************************************************************//**
 * Stops wait trend USB thread
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CSubViewTools::StopWaitTrendUSBThread( void )
{
	if(m_bDoWaitTrendUSBThread && m_pcwtWaitTrendUSBThread!=NULL)
	{
		m_bDoWaitTrendUSBThread=false;
		eventWaitTrendUSBfinish.SetEvent();

		if (WaitForSingleObject(m_pcwtWaitTrendUSBThread->m_hThread,1000) == WAIT_TIMEOUT)
		{
			theApp.getLog()->WriteLine(_T("#THR:030a"));
			if(!TerminateThread(m_pcwtWaitTrendUSBThread,0))
			{
				theApp.getLog()->WriteLine(_T("#THR:030b"));
			}
		}
	}
	
}

/**********************************************************************************************//**
 * Wait trend USB thread
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	pc	The PC.
 *
 * \return	An UINT.
 **************************************************************************************************/

static UINT CWaitTrendUSBThread( LPVOID pc )
{
	try
	{
		((CSubViewTools*)pc)->WaitTrendUSB();
	}
	catch (CException* e)
	{
		TCHAR   szCause[255];
		e->GetErrorMessage(szCause, 255);

		CString errorStr=_T("");
		errorStr.Format(_T("CWaitTrendUSBThread: %s"),szCause);

		theApp.ReportException(errorStr);

		e->Delete();
	}
	catch(...)
	{
		theApp.ReportException(_T("CWaitTrendUSBThread"));

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_EXCEPTION);
	}
	//((CSubViewTools*)pc)->WaitTrendUSB();
	return TRUE;
}

/**********************************************************************************************//**
 * Wait trend USB
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	A DWORD.
 **************************************************************************************************/

DWORD CSubViewTools::WaitTrendUSB(void) 
{
	do
	{
		DWORD dw = ::WaitForSingleObject(eventWaitTrendUSBfinish, INFINITE);

		switch(dw)
		{
		case WAIT_OBJECT_0:
			{
				if(m_bDoWaitTrendUSBThread)
				{
					if(!m_bExit && m_bDoWaitTrendUSBThread)
					{
						ShowWndHourglass(false);

						Draw();
						UpdateWindow();
						Invalidate(TRUE);

						SetTimer(USBCHECKTIMER,2000,NULL);
						if(GetParent())
							GetParent()->PostMessage(WM_SET_SETUPTIMER);
					}
					m_bDoWaitTrendUSBThread=false;
				}
			}
			break;
		default:
			break;
		}
	}while(m_bDoWaitTrendUSBThread);

	//theApp.getLog()->WriteLine(_T("#THR:030"));

	return 0;
}

/**********************************************************************************************//**
 * Creates window hourglass
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CSubViewTools::CreateWndHourglass()
{
	if(m_pWndHourglass==NULL)
	{
		m_pWndHourglass = new CWndWaitHourglass();

		RECT rcLd={250,180,550,380};
		//if(m_pWndHourglass->Create(this,rcLd,IDC_VIEW_HOURGLASS,_T("...saving trends...")))
		if(m_pWndHourglass->Create(this,rcLd,IDC_VIEW_HOURGLASS,getModel()->GetLanguageString(IDS_TREND_SAVEING)))
		{
			return true;
		}
	}
	return false;
}

/**********************************************************************************************//**
 * Destroys the window hourglass
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CSubViewTools::DestroyWndHourglass()
{
	if(m_pWndHourglass)
	{
		//m_pWndHourglass->StopTimer();
		m_pWndHourglass->DestroyWindow();
		delete m_pWndHourglass;
		m_pWndHourglass=NULL;
	}
	return false;
}

/**********************************************************************************************//**
 * Shows the window hourglass
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	bShow	True to show, false to hide.
 **************************************************************************************************/

void CSubViewTools::ShowWndHourglass(bool bShow)
{
	if(m_pWndHourglass)
	{
		/*if(bShow)
			m_pWndHourglass->StartTimer();
		else
			m_pWndHourglass->StopTimer();*/
		m_pWndHourglass->Show(bShow);
	}
}