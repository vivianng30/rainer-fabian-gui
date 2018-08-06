// SubViewVideo.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "SubViewVideo.h"
#include "N9VideoIn.h"
#include "DlgMessageBox.h"

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

/**********************************************************************************************//**
 * A macro that defines error pixels
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

#define ERROR_PIXELS 62

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
extern HFONT g_hf70Bold;

/**********************************************************************************************//**
 * CSubViewVideo
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CSubViewVideo, CWnd)

/**********************************************************************************************//**
 * Initializes a new instance of the CSubViewVideo class
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

CSubViewVideo::CSubViewVideo()
{
	m_pModel=NULL;

	m_pDlg = NULL;

	m_lX=0;
	m_lY=0;
	m_lXo=0;
	m_lYo=0;

	m_hDC = NULL;
	m_hBmp= NULL;
	m_hBmpPrev= NULL;

	m_pcRegionTopLeCorner=NULL;
	m_pcRegionTopRiCorner=NULL;
	m_pcRegionBotLeCorner=NULL;
	m_pcRegionBotRiCorner=NULL;

	m_pcStop_Up=NULL;
	m_pcStop_Dw=NULL;
	m_pcStart_Up=NULL;
	m_pcStart_Dw=NULL;

	m_pcStartStop=NULL;

	m_pcNoVideo=NULL;

	m_bExit=false;

	m_posX.Format(_T("%d"), 145);
	//m_posY.Format(_T("%d"), 130);
	m_posY.Format(_T("%d"), 83);
	/*m_sizeX.Format(_T("%d"), 530);
	m_sizeY.Format(_T("%d"), 400);*/

	m_sizeX.Format(_T("%d"), 630);
	m_sizeY.Format(_T("%d"), 430);


	/*m_posX.Format(_T("%d"), 157);
	m_posY.Format(_T("%d"), 70);
	m_sizeX.Format(_T("%d"), 610);
	m_sizeY.Format(_T("%d"), 460);*/


	m_bVideoRunning=false;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CSubViewVideo class
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

CSubViewVideo::~CSubViewVideo()
{
	delete m_pcRegionTopLeCorner;
	m_pcRegionTopLeCorner=NULL;
	delete m_pcRegionTopRiCorner;
	m_pcRegionTopRiCorner=NULL;
	delete m_pcRegionBotLeCorner;
	m_pcRegionBotLeCorner=NULL;
	delete m_pcRegionBotRiCorner;
	m_pcRegionBotRiCorner=NULL;

	delete m_pcNoVideo;
	m_pcNoVideo=NULL;

	delete m_pcStartStop;
	delete m_pcStop_Up;
	delete m_pcStop_Dw;
	delete m_pcStart_Up;
	delete m_pcStart_Dw;

	m_pcStop_Up=NULL;
	m_pcStop_Dw=NULL;
	m_pcStart_Up=NULL;
	m_pcStart_Dw=NULL;
	m_pcStartStop=NULL;
}


BEGIN_MESSAGE_MAP(CSubViewVideo, CWnd)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_STARTSTOP_VIDEO, &CSubViewVideo::OnBnClickedStartStopVideo)
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * CSubViewVideo message handlers
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	Null if it fails, else the model.
 **************************************************************************************************/

CMVModel *CSubViewVideo::getModel()
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

bool CSubViewVideo::CreateWnd(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext)
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

BOOL CSubViewVideo::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext) 
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

void CSubViewVideo::Initialize()
{
	CClientDC dc(this);

	m_pcRegionTopLeCorner		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_SETUP_LE_TOP_CORNER);
	m_pcRegionTopRiCorner		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_SETUP_RI_TOP_CORNER);
	m_pcRegionBotLeCorner		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_SETUP_LE_BOT_CORNER);
	m_pcRegionBotRiCorner		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_SETUP_RI_BOT_CORNER);

	m_pcStop_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_VIDEOSTOP_UP);
	m_pcStop_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_VIDEOSTOP_DW);
	m_pcStart_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_VIDEOPLAY_UP);
	m_pcStart_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_VIDEOPLAY_DW);

	

	m_pcNoVideo= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_PIC_NOVIDEO);
	

	BTN btn;

	btn.wID					= IDC_BTN_STARTSTOP_VIDEO;	
	btn.poPosition.x		= 30;//470;
	btn.poPosition.y		= 200;//30;
	btn.pcBmpUp				= m_pcStart_Up;
	btn.pcBmpDown			= m_pcStart_Dw;
	btn.pcBmpFocus			= m_pcStart_Up;
	btn.pcBmpDisabled		= m_pcStart_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcStartStop=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcStartStop->Create(this,g_hf14AcuMed,0);
	m_pcStartStop->SetText(_T(""));
	//m_pcStartStop->SetText(getModel()->GetLanguageString(IDS_TXT_SAVE));
	m_pcStartStop->ShowWindow(SW_SHOW);

	

}

/**********************************************************************************************//**
 * Shows
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	bShow	True to show, false to hide.
 **************************************************************************************************/

void CSubViewVideo::Show(bool bShow)
{
	if(bShow)
	{
		ShowWindow(SW_SHOW);
		Draw();

		if(!m_bVideoRunning)
		{
			if(GetParent())
				GetParent()->PostMessage(WM_SET_SETUPTIMER);
		}
	}
	else
	{
		ShowWindow(SW_HIDE);
	}
}

/**********************************************************************************************//**
 * Executes the destroy action
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CSubViewVideo::OnDestroy()
{
	KillTimer(USBCHECKTIMER);

	m_bExit=true;

	if(m_pDlg)
	{
		m_pDlg->EndDialog(IDCLOSE);
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_REDRAW_VIEW);
	}
	m_pDlg = NULL;
	
	if(m_bVideoRunning)
	{
		if(GetParent())
			GetParent()->PostMessage(WM_VIDEO_STOP);
		VID_HideOverlay();
	}

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

void CSubViewVideo::Draw()
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
	//int tc=SetTextColor(hdcMem,0x0000000);
	int tc=SetTextColor(hdcMem,RGB(255,0,0));

	Rectangle(hdcMem, 0, 0, m_lX, m_lY);

	CBrush cbrFill;
	cbrFill.CreateSolidBrush(RGB(181,178,181));
	SelectObject(hdcMem,cbrFill);

	Rectangle(hdcMem, 10, 30, 790, 485);
	Rectangle(hdcMem, 30, 10, 770, 30);
	Rectangle(hdcMem, 30, 485, 770, 505);
	if(m_pcRegionTopLeCorner)
		m_pcRegionTopLeCorner->Draw(hdcMem,10,10);
	if(m_pcRegionTopRiCorner)
		m_pcRegionTopRiCorner->Draw(hdcMem,770,10);
	if(m_pcRegionBotLeCorner)
		m_pcRegionBotLeCorner->Draw(hdcMem,10,485);
	if(m_pcRegionBotRiCorner)
		m_pcRegionBotRiCorner->Draw(hdcMem,770,485);

	

	CPen penLine;
	penLine.CreatePen(PS_SOLID,1,RGB(140,140,140));
	SelectObject(hdcMem,(HPEN)penLine);
	//CBrush cbrRound(RGB(200,200,200));
	CBrush cbrRound(RGB(255,255,255));
	SelectObject(hdcMem,cbrRound);
	//RoundRect(hdcMem, 110, 20, 670, 80,20,20);

	Rectangle(hdcMem, 145, 43, 775, 468);

	m_pcNoVideo->Draw(hdcMem,350,150);
	
	rc.left = 0;  
	rc.left = 145;  
	rc.top = 50;  
	//rc.right  = 800;  
	rc.right  = 775;  
	rc.bottom = 500;
	//CStringW cs = _T("!!! Ventilaton will be stopped while video capture is running !!!");
	CStringW cs = getModel()->GetLanguageString(IDS_TXT_VIDEO_STOPVENT);
	//DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);
	DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);


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
 * Executes the button clicked start stop video action
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CSubViewVideo::OnBnClickedStartStopVideo()
{
	if(!m_bVideoRunning)
	{
		if(GetParent())
		{
			GetParent()->PostMessage(WM_VIDEO_START);
			GetParent()->PostMessage(WM_KILL_SETUPTIMER);
		}

		CDlgMessageBox box(this,_T("Video capture"),_T("This will stop the ventilation. Continue?"),MB_YESNO,IDB_MSG_STOP);
		m_pDlg = &box;
		int iRes=box.DoModal();
		m_pDlg = NULL;
	

		UpdateWindow();
		Draw();

		if(iRes==IDYES)
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_VENT_STANDBY);
			m_bVideoRunning=true;

			

			RECT sr, tr;
			DWORD sx, sy;

			m_pcStartStop->SetBitmaps(m_pcStop_Up,
				m_pcStop_Dw,
				m_pcStop_Up,
				m_pcStop_Up);

			UpdateData(TRUE);

			
			sr.left   = 0;
			sr.top    = 0;
			sr.right  = _tcstol(m_sizeX, NULL, 0);
			sr.bottom = _tcstol(m_sizeY, NULL, 0);
			tr.left   = _tcstol(m_posX, NULL, 0);
			tr.top    = _tcstol(m_posY, NULL, 0);
			tr.right  = _tcstol(m_sizeX, NULL, 0) + tr.left;
			tr.bottom = _tcstol(m_sizeY, NULL, 0) + tr.top;
			sx = _tcstol(m_sizeX, NULL, 0);
			sy = _tcstol(m_sizeY, NULL, 0);
			////VID_SetZoom(sx,sy, TRUE);
			////VID_SetZoom(sx,sy, FALSE);
			//VID_ResetScaler();
			///*VID_SetDefault();
			//VID_SetInput(CVBS);*/
			//VID_CreateOverlay(sx, sy);
			
			VID_SetZoom(sx,sy, FALSE);
			VID_ResetScaler();
			VID_SetInput(CVBS);
			if(float(sx/sy)<1.7f)
			{
				// We assume resolution is 5:3 
				VID_CreateOverlay(640-ERROR_PIXELS, 320);
			}
			else
				VID_CreateOverlay(480-ERROR_PIXELS, 272);


			VID_ShowOverlay(&sr, &tr);
		}
		else if(iRes==IDNO)
		{
			if(GetParent())
			{
				GetParent()->PostMessage(WM_VIDEO_STOP);
			}
			
		}
		
	}
	else
	{
		m_pcStartStop->SetBitmaps(m_pcStart_Up,
			m_pcStart_Dw,
			m_pcStart_Up,
			m_pcStart_Up);
		//m_pcStartStop->RefreshText(_T("start"));
		VID_HideOverlay();

		CDlgMessageBox box(this,_T("Ventilation"),_T("Do you want to restart the ventilation?"),MB_YESNO,IDB_MSG_STOP);
		m_pDlg = &box;
		int iRes=box.DoModal();
		m_pDlg = NULL;


		UpdateWindow();
		Draw();

		

		if(iRes==IDYES)
		{
			if(GetParent())
				GetParent()->PostMessage(WM_VIDEO_STOP);
			m_bVideoRunning=false;
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_VENT_RUN);
			if(GetParent())
				GetParent()->PostMessage(WM_SET_SETUPTIMER);
		}
		else  if(iRes==IDNO)
		{
			m_bVideoRunning=false;
			if(GetParent())
			{
				GetParent()->PostMessage(WM_VIDEO_STOP);
			}
			
		}

		
	}
	

	
}

/**********************************************************************************************//**
 * Stops a video
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CSubViewVideo::StopVideo()
{
	m_bVideoRunning=false;


	m_pcStartStop->SetBitmaps(m_pcStart_Up,
		m_pcStart_Dw,
		m_pcStart_Up,
		m_pcStart_Up);

	VID_HideOverlay();

	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_VIDEO_STOP);
}
