// WndMenuMenu.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "WndMenuMenu.h"
#include "globDefs.h"

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

/**********************************************************************************************//**
 * CWndMenuMenu
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CWndMenuMenu, CWnd)

/**********************************************************************************************//**
 * Initializes a new instance of the CWndMenuMenu class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	parentView	If non-null, the parent view.
 **************************************************************************************************/

CWndMenuMenu::CWndMenuMenu(CMVView *parentView)
{
	m_parentView=parentView;

	m_csMenuText=_T("");

	m_hDC=NULL;
	m_hBmp=NULL;
	m_hBmpPrev=NULL;

	m_lX				= 0;
	m_lY				= 0;
	m_lXo				= 0;
	m_lYo				= 0;

	m_pcMoveLeft=NULL;
	m_pcMoveRight=NULL;
	//m_pcMenu=NULL;
	m_pcBack=NULL;
	//m_pcMenu_Dis=NULL;
	m_pcMenu_Up=NULL;
	m_pcMenu_Dw=NULL;
	m_pcNumeric_Up=NULL;
	m_pcNumeric_Dw=NULL;

	m_pcMenuLeft_Up=NULL;
	m_pcMenuLeft_Dw=NULL;
	m_pcMenuLeft_Dis=NULL;

	m_pcMenuRight_Up=NULL;
	m_pcMenuRight_Dw=NULL;
	m_pcMenuRight_Dis=NULL;

	m_pModel = NULL;

	m_bMenuView=false;

	m_pcRegionTopLeCorner=NULL;
	m_pcRegionTopRiCorner=NULL;
	m_pcRegionBotLeCorner=NULL;
	m_pcRegionBotRiCorner=NULL;

	
}

/**********************************************************************************************//**
 * Finalizes an instance of the CWndMenuMenu class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

CWndMenuMenu::~CWndMenuMenu()
{
	delete m_pcRegionTopLeCorner;
	m_pcRegionTopLeCorner=NULL;
	delete m_pcRegionTopRiCorner;
	m_pcRegionTopRiCorner=NULL;
	delete m_pcRegionBotLeCorner;
	m_pcRegionBotLeCorner=NULL;
	delete m_pcRegionBotRiCorner;
	m_pcRegionBotRiCorner=NULL;


	delete m_pcMoveLeft;
	m_pcMoveLeft=NULL;
	delete m_pcMoveRight;
	m_pcMoveRight=NULL;
	/*delete m_pcMenu;
	m_pcMenu=NULL;*/
	delete m_pcBack;
	m_pcBack=NULL;

	delete m_pcMenu_Up;
	m_pcMenu_Up=NULL;
	delete m_pcMenu_Dw;
	m_pcMenu_Dw=NULL;
	/*delete m_pcMenu_Dis;
	m_pcMenu_Dis=NULL;*/

	delete m_pcNumeric_Up;
	m_pcNumeric_Up=NULL;
	delete m_pcNumeric_Dw;
	m_pcNumeric_Dw=NULL;
	
	delete m_pcMenuLeft_Up;
	m_pcMenuLeft_Up=NULL;
	delete m_pcMenuLeft_Dw;
	m_pcMenuLeft_Dw=NULL;
	delete m_pcMenuLeft_Dis;
	m_pcMenuLeft_Dis=NULL;

	delete m_pcMenuRight_Up;
	m_pcMenuRight_Up=NULL;
	delete m_pcMenuRight_Dw;
	m_pcMenuRight_Dw=NULL;
	delete m_pcMenuRight_Dis;
	m_pcMenuRight_Dis=NULL;
}


BEGIN_MESSAGE_MAP(CWndMenuMenu, CWnd)
	ON_WM_PAINT()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * CWndMenuMenu message handlers
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	Null if it fails, else the model.
 **************************************************************************************************/

CMVModel *CWndMenuMenu::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}

/**********************************************************************************************//**
 * Creates the Window instance that will be represented by this instance
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	pParentWnd	If non-null, the parent window.
 * \param 		  	rc		  	The rectangle.
 * \param 		  	nID		  	The identifier.
 * \param [in,out]	pContext  	If non-null, the context.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

BOOL CWndMenuMenu::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext) 
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

		//CBrush cbrBack(RGB(151,158,151));
		CBrush cbrBack(BACKGND);
		HBRUSH hbrprev=(HBRUSH)SelectObject(m_hDC,cbrBack);
		HPEN hpenprev=(HPEN)SelectObject(m_hDC, (HPEN)GetStockObject(NULL_PEN));	

		Rectangle(m_hDC, 0, 0, m_lX, m_lY);

		SelectObject(m_hDC,hpenprev);
		SelectObject(m_hDC,hbrprev);
		//cbrBack.DeleteObject();//rkuNEWFIX

		return 1;
	}
	else
		return 0;
}

/**********************************************************************************************//**
 * Initializes this instance
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndMenuMenu::Init()
{

	CClientDC dc(this);

	BTN btn;

	m_pcRegionTopLeCorner		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_SETUP_LE_TOP_CORNER);
	m_pcRegionTopRiCorner		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_SETUP_RI_TOP_CORNER);
	m_pcRegionBotLeCorner		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_SETUP_LE_BOT_CORNER);
	m_pcRegionBotRiCorner		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_SETUP_RI_BOT_CORNER);


	m_pcMenu_Up		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_NUMERIC_UP);
	m_pcMenu_Dw		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_NUMERIC_DW);
	//m_pcMenu_Dis	= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_4MENU_DIS);

	m_pcMenuLeft_Up		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_MOVEMENUL_UP);
	m_pcMenuLeft_Dw		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_MOVEMENUL_DWN);
	m_pcMenuLeft_Dis	= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_MOVEMENUL_DIS);

	m_pcMenuRight_Up	= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_MOVEMENUR_UP);
	m_pcMenuRight_Dw	= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_MOVEMENUR_DWN);
	m_pcMenuRight_Dis	= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_MOVEMENUR_DIS);

	m_pcNumeric_Up		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_NUM_BACK_UP);
	m_pcNumeric_Dw		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_NUM_BACK_DW);
	//m_pcNumeric_Fc		= new CBmp(theApp.m_hInstance,hdc,	IDB_BTN_NUMMENUBCK_FC);

	//Menu Buttons
	//btn.wID					= IDC_BTN_MENUBUTTON;	
	//btn.poPosition.x		= 15;
	//btn.poPosition.y		= 0;//553;
	//btn.pcBmpUp				= m_pcMenu_Up;
	//btn.pcBmpDown			= m_pcMenu_Dw;
	//btn.pcBmpFocus			= m_pcMenu_Up;
	//btn.pcBmpDisabled		= m_pcMenu_Up;
	//btn.dwFormat			= DT_VCENTER|DT_CENTER;

	//m_pcMenu=new CPushBtn(btn,COLOR_TXTBTNUP,true);
	//m_pcMenu->Create(this,g_hf11AcuBold,0);
	//if(m_bMenuView==false)
	//{
	//	m_pcMenu->SetText(getModel()->GetLanguageString(IDS_MENU_OPEN_TOP),getModel()->GetLanguageString(IDS_MENU_OPEN_BOT));
	//}
	//else
	//{
	//	m_pcMenu->SetText(getModel()->GetLanguageString(IDS_MENU_CLOSE_TOP),getModel()->GetLanguageString(IDS_MENU_CLOSE_BOT));
	//}
	//m_pcMenu->ShowWindow(SW_HIDE);



	btn.wID					= IDC_BTN_MENU_MOVELEFT;	
	btn.poPosition.x		= 190;
	btn.poPosition.y		= 0;
	btn.pcBmpUp				= m_pcMenuLeft_Up;
	btn.pcBmpDown			= m_pcMenuLeft_Dw;
	btn.pcBmpFocus			= m_pcMenuLeft_Up;
	btn.pcBmpDisabled		= m_pcMenuLeft_Dis;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcMoveLeft=new CPushBtn(btn,COLOR_TXTBTNUP);
	m_pcMoveLeft->Create(this,g_hf14AcuMed,0);
	m_pcMoveLeft->SetText(_T(""),_T(""));
	m_pcMoveLeft->ShowWindow(SW_HIDE);


	btn.wID					= IDC_BTN_MENU_MOVERIGHT;	
	btn.poPosition.x		= 530;
	btn.poPosition.y		= 0;
	btn.pcBmpUp				= m_pcMenuRight_Up;
	btn.pcBmpDown			= m_pcMenuRight_Dw;
	btn.pcBmpFocus			= m_pcMenuRight_Up;
	btn.pcBmpDisabled		= m_pcMenuRight_Dis;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcMoveRight=new CPushBtn(btn,COLOR_TXTBTNUP,true);
	m_pcMoveRight->Create(this,g_hf11AcuBold,0);
	m_pcMoveRight->SetText(_T(""),_T(""));
	m_pcMoveRight->ShowWindow(SW_HIDE);



	btn.wID					= IDC_BTN_MENU_NUMERIC;	
	btn.poPosition.x		= 625;
	btn.poPosition.y		= 0;//553;
	btn.pcBmpUp				= m_pcNumeric_Up;
	btn.pcBmpDown			= m_pcNumeric_Dw;
	btn.pcBmpFocus			= m_pcNumeric_Up;
	btn.pcBmpDisabled		= m_pcNumeric_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcBack=new CPushBtn(btn,COLOR_TXTBTNUP);
	m_pcBack->Create(this,g_hf14AcuMed,0);
	m_pcBack->SetText(_T(""));
	m_pcBack->ShowWindow(SW_HIDE);

	ShowMenuBtn();

	

}

/**********************************************************************************************//**
 * Sets menu text
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	txt	   	The text.
 * \param	bRedraw	True to redraw.
 **************************************************************************************************/

void CWndMenuMenu::SetMenuText(CStringW txt, bool bRedraw)
{
	m_csMenuText=txt;

	if(bRedraw)
	{
		Draw();
	}

}

/**********************************************************************************************//**
 * Shows
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	bShow	True to show, false to hide.
 **************************************************************************************************/

void CWndMenuMenu::Show(BOOL bShow)
{
	if(this->IsWindowVisible() == bShow)
	{
		//Draw();
		return;
	}

	if(bShow==TRUE)
	{
		this->ShowWindow(SW_SHOW);
	}
	else
		this->ShowWindow(SW_HIDE);

}

/**********************************************************************************************//**
 * Shows the menu button
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndMenuMenu::ShowMenuBtn()
{
	//Draw();

	//if(m_bMultiPages)
	/*{
		m_pcMoveLeft->ShowWindow(SW_SHOW);
		m_pcMoveRight->ShowWindow(SW_SHOW);
		m_pcMoveLeft->EnableWindow(TRUE);
		m_pcMoveRight->EnableWindow(TRUE);
	}*/
	//else
	{
		if(m_pcMoveLeft)
			m_pcMoveLeft->ShowWindow(SW_HIDE);
		if(m_pcMoveRight)
			m_pcMoveRight->ShowWindow(SW_HIDE);
		//m_pcMoveLeft->EnableWindow(FALSE);
		//m_pcMoveRight->EnableWindow(FALSE);
	}

	/*if(m_pcMenu)
		m_pcMenu->ShowWindow(SW_SHOW);*/
	if(m_pcBack)
		m_pcBack->ShowWindow(SW_SHOW);
}

/**********************************************************************************************//**
 * Hides the menu button
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndMenuMenu::HideMenuBtn()
{
	if(m_pcMoveLeft)
		m_pcMoveLeft->ShowWindow(SW_HIDE);
	if(m_pcMoveRight)
		m_pcMoveRight->ShowWindow(SW_HIDE);
	if(m_pcBack)
		m_pcBack->ShowWindow(SW_HIDE);
	/*if(m_pcMenu)
		m_pcMenu->ShowWindow(SW_HIDE);*/
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

LRESULT CWndMenuMenu::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
{
	switch(message)
	{
	//case WM_SETMENUBTN_RETURN:
	//	{
	//		m_bMenuView=false;
	//		/*if(m_pcMenu)
	//			m_pcMenu->ShowWindow(SW_SHOW);*/
	//		if(m_pcBack)
	//			m_pcBack->ShowWindow(SW_SHOW);
	//		return 1;
	//	}
	//	break;
	//case WM_SETMENUBTN_OPEN:
	//	{
	//		if(m_pcBack)
	//		{
	//			m_pcBack->DrawDirectUp();
	//			m_pcBack->ShowWindow(SW_SHOW);
	//		}
	//		
	//		m_bMenuView=false;
	//		/*if(m_pcMenu)
	//		{
	//			m_pcMenu->RefreshText(getModel()->GetLanguageString(IDS_MENU_OPEN_TOP),getModel()->GetLanguageString(IDS_MENU_OPEN_BOT));
	//			m_pcMenu->ShowWindow(SW_SHOW);
	//		}*/
	//		return 1;
	//	}
	//	break;
	//case WM_SETMENUBTN_CLOSE:
	//	{
	//		if(m_pcBack)
	//		{
	//			m_pcBack->DrawDirectUp();
	//			m_pcBack->ShowWindow(SW_SHOW);
	//		}
	//		m_bMenuView=true;
	//		/*if(m_pcMenu)
	//		{
	//			m_pcMenu->RefreshText(getModel()->GetLanguageString(IDS_MENU_CLOSE_TOP),getModel()->GetLanguageString(IDS_MENU_CLOSE_BOT));
	//			m_pcMenu->ShowWindow(SW_SHOW);
	//		}*/
	//		return 1;
	//	}
	//	break;
	case WM_MENUBUTTONDOWN:
		{
			switch(wParam)
			{
			/*case IDC_BTN_MENUBUTTON:
				{
					if(m_bMenuView==true)
					{
						m_bMenuView=false;
						
						if(AfxGetApp())
							AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_MENUBUTTON_CLOSE);

						if(m_pcMenu)
							m_pcMenu->RefreshText(getModel()->GetLanguageString(IDS_MENU_OPEN_TOP),getModel()->GetLanguageString(IDS_MENU_OPEN_BOT));


					}
					else
					{
						m_bMenuView=true;
						
						if(AfxGetApp())
							AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_MENUBUTTON_OPEN);

						if(m_pcMenu)
							m_pcMenu->RefreshText(getModel()->GetLanguageString(IDS_MENU_CLOSE_TOP),getModel()->GetLanguageString(IDS_MENU_CLOSE_BOT));

					}
					return 1;

					
				}
				break;*/
			case IDC_BTN_MENU_MOVELEFT:
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_MENU_MOVELEFT);
					return 1;
				}
				break;
			case IDC_BTN_MENU_MOVERIGHT:
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_MENU_MOVERIGHT);
					return 1;
				}
				break;
			case IDC_BTN_MENU_NUMERIC:
				{
					if(m_parentView)
						m_parentView->PostMessage(WM_BN_BACK);
					/*if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_NUMERIC);*/
					return 1;
				}
				break;
			}
		}
		break;
	}
	return CWnd::WindowProc(message, wParam, lParam);
}

/**********************************************************************************************//**
 * Paints this window
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndMenuMenu::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Fügen Sie hier Ihren Meldungsbehandlungscode ein.
	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,m_hDC,0,0,SRCCOPY);

	Draw();

	// CMVView::OnPaint() soll zum Zeichnen von Meldungen nicht aufgerufen werden.
}

/**********************************************************************************************//**
 * Executes the destroy action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndMenuMenu::OnDestroy() 
{
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
 * Translates all messages before they are processed by the main message loop
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	pMsg	If non-null, the message.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

BOOL CWndMenuMenu::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message)
	{
	case WM_KEYDOWN:
		if(pMsg->wParam==VK_SPACE)
		{
			return 1;
		}
		else if(pMsg->wParam==VK_UP)
		{
			return 1;
		}
		else if(pMsg->wParam==VK_DOWN)
		{
			return 1;
		}
		else
		{
			//handle focus change manual, draw next focused button and return true
			//return 1;
		}
		break;
	default:
		{

		}
		break;
	}
	return CWnd::PreTranslateMessage(pMsg);
}

/**********************************************************************************************//**
 * Draws this instance
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndMenuMenu::Draw()
{
	RECT rcCl;
	RECT rc;
	GetClientRect(&rcCl);

	CClientDC dc(this);
	HDC hdcMem=CreateCompatibleDC(dc.m_hDC);
	HBITMAP hBmpMem=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
	HBITMAP hBmpMemPrev=(HBITMAP)SelectObject(hdcMem,hBmpMem);


	//CBrush cbrBack(RGB(151,158,151));
	CBrush cbrBack(BACKGND);
	CBrush cbrGrey,cbrLightBlue,cbrDarkBlue;
	cbrGrey.CreateSolidBrush(RGB(176,176,177));
	cbrLightBlue.CreateSolidBrush(RGB(190,190,255));
	cbrDarkBlue.CreateSolidBrush(RGB(85,95,255));
	HBRUSH hbrprev=(HBRUSH)SelectObject(hdcMem,cbrBack);
	HPEN hpenprev=(HPEN)SelectObject(hdcMem, (HPEN)GetStockObject(NULL_PEN));	
	int nBkMode=SetBkMode(hdcMem,TRANSPARENT);
	HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf15AcuMed);
	int tc=SetTextColor(hdcMem,0x0000000);

	Rectangle(hdcMem, 0, 0, m_lX, m_lY);

	//CDC* pDCMem=CDC::FromHandle(hdcMem);


	//CStringW cs = _T("Test");
	//m_csMenuText=_T("Test");

	

	//if(m_bMultiPages)
	//{
	//	rc.left = 268;  
	//	rc.top = 0;  
	//	rc.right  = 532;  
	//	rc.bottom = 48;
	//	FillRect(hdcMem,&rc,cbrGrey);
	//	//SelectObject(hdcMem, (HBRUSH)cbrLightBlue);
	//	//int iLeft=380;
	//	//int iTop=10;
	//	//int iRight=390;
	//	//int iBottom=20;
	//	//Ellipse(hdcMem,iLeft,iTop,iRight,iBottom);

	//	//iLeft=410;
	//	//iTop=10;
	//	//iRight=420;
	//	//iBottom=20;
	//	//Ellipse(hdcMem,iLeft,iTop,iRight,iBottom);

	//	//SelectObject(hdcMem, (HBRUSH)cbrDarkBlue);
	//	////Mitte
	//	//iLeft=395;
	//	//iTop=10;
	//	//iRight=405;
	//	//iBottom=20;
	//	//Ellipse(hdcMem,iLeft,iTop,iRight,iBottom);




	//	rc.left = 270;  
	//	rc.top = 0;  
	//	rc.right  = 530;  
	//	rc.bottom = 43;
	//	DrawText(hdcMem,m_csMenuText,-1,&rc,DT_CENTER|DT_BOTTOM|DT_SINGLELINE);

	//}
	//else
	{
		SelectObject(hdcMem,cbrGrey);

		Rectangle(hdcMem, 190, 20, 210, 27);
		Rectangle(hdcMem, 210, 0, 589, 47);
		Rectangle(hdcMem, 589, 20, 609, 27);
		if(m_pcRegionTopLeCorner)
			m_pcRegionTopLeCorner->Draw(hdcMem,190,0);
		if(m_pcRegionTopRiCorner)
			m_pcRegionTopRiCorner->Draw(hdcMem,589,0);
		if(m_pcRegionBotLeCorner)
			m_pcRegionBotLeCorner->Draw(hdcMem,190,27);
		if(m_pcRegionBotRiCorner)
			m_pcRegionBotRiCorner->Draw(hdcMem,589,27);


		rc.left = 270;  
		rc.top = 0;  
		rc.right  = 530;  
		rc.bottom = 48;
		DrawText(hdcMem,m_csMenuText,-1,&rc,DT_CENTER|DT_VCENTER|DT_SINGLELINE);

	}
	

	

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

