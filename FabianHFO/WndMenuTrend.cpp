// WndMenuTrend.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "WndMenuTrend.h"
#include "LangAdmin.h"
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


//global font objects
//extern HFONT g_hf15AcuMed;

//extern CLangAdmin* g_pGlobalLanguageStrings;

/**********************************************************************************************//**
 * CWndMenuTrend
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CWndMenuTrend, CWnd)

/**********************************************************************************************//**
 * Initializes a new instance of the CWndMenuTrend class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	parentView	If non-null, the parent view.
 **************************************************************************************************/

CWndMenuTrend::CWndMenuTrend()
{
	m_lX				= 0;
	m_lY				= 0;
	m_lXo				= 0;
	m_lYo				= 0;

	m_hDC=NULL;
	m_hBmp=NULL;
	m_hBmpPrev=NULL;

	m_pcMenu1=NULL;
	m_pcMenu2=NULL;
	m_pcMenu3=NULL;
	m_pcMenu4=NULL;
	m_pcMenu5=NULL;

	//m_pcMenuNum=NULL;

	m_pcMenu_Dis=NULL;
	m_pcMenu_Up=NULL;
	m_pcMenu_Dw=NULL;
	m_pcMenu_IncrUp=NULL;
	m_pcMenu_IncrDw=NULL;
	m_pcMenu_DecrUp=NULL;
	m_pcMenu_DecrDw=NULL;
	m_pcMenu_MoveLeftUp=NULL;
	m_pcMenu_MoveLeftDw=NULL;
	m_pcMenu_MoveRightUp=NULL;
	m_pcMenu_MoveRightDw=NULL;

	/*m_pcNumeric_Up=NULL;
	m_pcNumeric_Dw=NULL;*/

	m_pModel = NULL;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CWndMenuTrend class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

CWndMenuTrend::~CWndMenuTrend()
{
	delete m_pcMenu1;
	m_pcMenu1=NULL;
	delete m_pcMenu2;
	m_pcMenu2=NULL;
	delete m_pcMenu3;
	m_pcMenu3=NULL;
	delete m_pcMenu4;
	m_pcMenu4=NULL;
	delete m_pcMenu5;
	m_pcMenu5=NULL;
	/*delete m_pcMenuNum;
	m_pcMenuNum=NULL;*/

	delete m_pcMenu_Dis;
	m_pcMenu_Dis=NULL;
	delete m_pcMenu_Up;
	m_pcMenu_Up=NULL;
	delete m_pcMenu_Dw;
	m_pcMenu_Dw=NULL;
	delete m_pcMenu_IncrUp;
	m_pcMenu_IncrUp=NULL;
	delete m_pcMenu_IncrDw;
	m_pcMenu_IncrDw=NULL;
	delete m_pcMenu_DecrUp;
	m_pcMenu_DecrUp=NULL;
	delete m_pcMenu_DecrDw;
	m_pcMenu_DecrDw=NULL;
	delete m_pcMenu_MoveLeftUp;
	m_pcMenu_MoveLeftUp=NULL;
	delete m_pcMenu_MoveLeftDw;
	m_pcMenu_MoveLeftDw=NULL;
	delete m_pcMenu_MoveRightUp;
	m_pcMenu_MoveRightUp=NULL;
	delete m_pcMenu_MoveRightDw;
	m_pcMenu_MoveRightDw=NULL;
	/*delete m_pcNumeric_Up;
	m_pcNumeric_Up=NULL;
	delete m_pcNumeric_Dw;
	m_pcNumeric_Dw=NULL;*/
}

/**********************************************************************************************//**
 * Gets the model
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	Null if it fails, else the model.
 **************************************************************************************************/

CMVModel *CWndMenuTrend::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}


BEGIN_MESSAGE_MAP(CWndMenuTrend, CWnd)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	/*ON_BN_CLICKED(IDC_BTN_TREND1, &CWndMenuTrend::OnBnClickedMenu1)
	ON_BN_CLICKED(IDC_BTN_TREND2, &CWndMenuTrend::OnBnClickedMenu2)
	ON_BN_CLICKED(IDC_BTN_TREND3, &CWndMenuTrend::OnBnClickedMenu3)
	ON_BN_CLICKED(IDC_BTN_TREND4, &CWndMenuTrend::OnBnClickedMenu4)
	ON_BN_CLICKED(IDC_BTN_TREND5, &CWndMenuTrend::OnBnClickedMenu5)*/
	//ON_BN_CLICKED(IDC_BTN_MENU_NUMERIC, &CWndMenuTrend::OnBnClickedMenu5)
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * CWndMenuGraphs message handlers
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

BOOL CWndMenuTrend::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext) 
{
	m_lX=rc.right-rc.left;
	m_lY=rc.bottom-rc.top;

	CStringW  lpszClassName=AfxRegisterWndClass(NULL); 
	if(CWnd::Create(lpszClassName,NULL,WS_CHILD/*|WS_VISIBLE*/,rc,pParentWnd,nID))
	{
		CClientDC dc(this);

		m_hDC=CreateCompatibleDC(dc.m_hDC);
		m_hBmp=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
		m_hBmpPrev=(HBITMAP)SelectObject(m_hDC,m_hBmp);

		CBrush cbrBack(BACKGND);
		//CBrush cbrBack(RGB(200,200,200));
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

void CWndMenuTrend::Init(CMVView *parentView)
{
	m_parentView=parentView;

	CClientDC dc(this);

	DWORD dwStyleNoTab = WS_CHILD|WS_VISIBLE|BS_OWNERDRAW;
	//DWORD dwStyleNoTab = WS_CHILD|WS_VISIBLE|BS_OWNERDRAW|WS_TABSTOP;

	BTN btn;

	m_pcMenu_Up		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_5MENU_REFRESH_UP);
	m_pcMenu_Dw		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_5MENU_REFRESH_DW);
	
	//m_pcMenu_Up		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_5MENU_UP);
	//m_pcMenu_Dw		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_5MENU_DW);
	m_pcMenu_Dis		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_5MENU_DIS);

	m_pcMenu_IncrUp		= new CBmp(theApp.m_hInstance,dc.m_hDC,IDB_BTN_5MENU_INCR_UP);
	m_pcMenu_IncrDw		= new CBmp(theApp.m_hInstance,dc.m_hDC,IDB_BTN_5MENU_INCR_DW);
	m_pcMenu_DecrUp		= new CBmp(theApp.m_hInstance,dc.m_hDC,IDB_BTN_5MENU_DECR_UP);
	m_pcMenu_DecrDw		= new CBmp(theApp.m_hInstance,dc.m_hDC,IDB_BTN_5MENU_DECR_DW);
	m_pcMenu_MoveLeftUp		= new CBmp(theApp.m_hInstance,dc.m_hDC,IDB_BTN_5MENU_MOVEL_UP);
	m_pcMenu_MoveLeftDw		= new CBmp(theApp.m_hInstance,dc.m_hDC,IDB_BTN_5MENU_MOVEL_DW);
	m_pcMenu_MoveRightUp		= new CBmp(theApp.m_hInstance,dc.m_hDC,IDB_BTN_5MENU_MOVER_UP);
	m_pcMenu_MoveRightDw		= new CBmp(theApp.m_hInstance,dc.m_hDC,IDB_BTN_5MENU_MOVER_DW);

	/*m_pcNumeric_Up		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_NUM_BACK_UP);
	m_pcNumeric_Dw		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_NUM_BACK_DW);*/

	//Menu Buttons
	btn.wID					= IDC_BTN_TREND1;	
	btn.poPosition.x		= 3;
	btn.poPosition.y		= 0;//553;
	btn.pcBmpUp				= m_pcMenu_IncrUp;
	btn.pcBmpDown			= m_pcMenu_IncrDw;
	btn.pcBmpFocus			= m_pcMenu_IncrUp;
	btn.pcBmpDisabled		= m_pcMenu_IncrUp;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcMenu1=new CPushBtn(btn,COLOR_TXTBTNUP);
	m_pcMenu1->Create(this,g_hf14AcuMed,0);
	m_pcMenu1->SetText(_T(""),_T(""));
	m_pcMenu1->ShowWindow(SW_HIDE);
	//m_plMenuBtn.AddTail(m_pcMenu1);



	btn.wID					= IDC_BTN_TREND2;	
	btn.poPosition.x		= 120;
	btn.poPosition.y		= 0;//553;
	btn.pcBmpUp				= m_pcMenu_DecrUp;
	btn.pcBmpDown			= m_pcMenu_DecrDw;
	btn.pcBmpFocus			= m_pcMenu_DecrUp;
	btn.pcBmpDisabled		= m_pcMenu_DecrUp;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcMenu2=new CPushBtn(btn,COLOR_TXTBTNUP);
	m_pcMenu2->Create(this,g_hf14AcuMed,0);
	m_pcMenu2->SetText(_T(""),_T(""));
	m_pcMenu2->ShowWindow(SW_HIDE);
	//m_plMenuBtn.AddTail(m_pcMenu2);

	btn.wID					= IDC_BTN_TREND3;	
	btn.poPosition.x		= 237;
	btn.poPosition.y		= 0;//553;
	btn.pcBmpUp				= m_pcMenu_MoveLeftUp;
	btn.pcBmpDown			= m_pcMenu_MoveLeftDw;
	btn.pcBmpFocus			= m_pcMenu_MoveLeftUp;
	btn.pcBmpDisabled		= m_pcMenu_MoveLeftUp;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcMenu3=new CPushBtn(btn,COLOR_TXTBTNUP);
	m_pcMenu3->Create(this,g_hf14AcuMed,0);
	m_pcMenu3->SetText(_T(""),_T(""));
	m_pcMenu3->ShowWindow(SW_HIDE);
	//m_plMenuBtn.AddTail(m_pcMenu3);

	btn.wID					= IDC_BTN_TREND4;	
	btn.poPosition.x		= 354;
	btn.poPosition.y		= 0;//553;
	btn.pcBmpUp				= m_pcMenu_MoveRightUp;
	btn.pcBmpDown			= m_pcMenu_MoveRightDw;
	btn.pcBmpFocus			= m_pcMenu_MoveRightUp;
	btn.pcBmpDisabled		= m_pcMenu_MoveRightUp;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	/*m_pcMenu4=new CMenuBtn(btn,COLOR_TXTBTNUP,GRP_GRAPH_BTN);
	m_pcMenu4->Create(this,g_hf14AcuMed,0);
	m_pcMenu4->SetText(_T("Settings"),_T("Settings"));
	m_pcMenu4->ShowWindow(SW_HIDE);
	m_plMenuBtn.AddTail(m_pcMenu4);*/

	m_pcMenu4=new CPushBtn(btn,COLOR_TXTBTNUP);
	m_pcMenu4->Create(this,g_hf14AcuMed,0);
	m_pcMenu4->SetText(_T(""),_T(""));
	m_pcMenu4->ShowWindow(SW_HIDE);




	btn.wID					= IDC_BTN_TREND5;	
	btn.poPosition.x		= 471;
	btn.poPosition.y		= 0;//553;
	btn.pcBmpUp				= m_pcMenu_Up;
	btn.pcBmpDown			= m_pcMenu_Dw;
	btn.pcBmpFocus			= m_pcMenu_Up;
	btn.pcBmpDisabled		= m_pcMenu_Dis;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	

	m_pcMenu5=new CPushBtn(btn,COLOR_TXTBTNUP);
	m_pcMenu5->Create(this,g_hf14AcuMed,0);
	//m_pcMenu5->SetText(getModel()->GetLanguageString(IDS_MENU_SETTINGS),getModel()->GetLanguageString(IDS_MENU_SETTINGS));
	m_pcMenu5->SetText(_T(""),_T(""));
	m_pcMenu5->ShowWindow(SW_HIDE);


	//btn.wID					= IDC_BTN_MENU_NUMERIC;	
	//btn.poPosition.x		= 625;
	//btn.poPosition.y		= 0;//553;
	//btn.pcBmpUp				= m_pcNumeric_Up;
	//btn.pcBmpDown			= m_pcNumeric_Dw;
	//btn.pcBmpFocus			= m_pcNumeric_Up;
	//btn.pcBmpDisabled		= m_pcNumeric_Up;
	//btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	//m_pcMenuNum=new CPushBtn(btn,COLOR_TXTBTNUP);
	//m_pcMenuNum->Create(this,g_hf14AcuMed,0);
	//m_pcMenuNum->SetText(_T(""));
	//m_pcMenuNum->ShowWindow(SW_HIDE);

	ShowMenuBtn();

}

/**********************************************************************************************//**
 * Shows
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	bShow	True to show, false to hide.
 **************************************************************************************************/

void CWndMenuTrend::Show(BOOL bShow)
{
	/*if(this->IsWindowVisible() == bShow)
	{
		return;
	}*/

	if(bShow==TRUE)
		this->ShowWindow(SW_SHOW);
	else
		this->ShowWindow(SW_HIDE);
}

/**********************************************************************************************//**
 * Shows the menu button
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndMenuTrend::ShowMenuBtn()
{
	if(m_pcMenu1)
		m_pcMenu1->ShowWindow(SW_SHOW);
	if(m_pcMenu2)
		m_pcMenu2->ShowWindow(SW_SHOW);
	if(m_pcMenu3)
		m_pcMenu3->ShowWindow(SW_SHOW);
	if(m_pcMenu4)
		m_pcMenu4->ShowWindow(SW_SHOW);
	if(m_pcMenu5)
		m_pcMenu5->ShowWindow(SW_SHOW);
	//m_pcMenu5->EnableWindow(FALSE);
	/*if(m_pcMenuNum)
		m_pcMenuNum->ShowWindow(SW_SHOW);*/

	//SetOneButtonDepressed(IDC_BTN_TREND1);
}

/**********************************************************************************************//**
 * Hides the menu button
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndMenuTrend::HideMenuBtn()
{
	if(m_pcMenu1)
		m_pcMenu1->ShowWindow(SW_HIDE);
	if(m_pcMenu2)
		m_pcMenu2->ShowWindow(SW_HIDE);
	if(m_pcMenu3)
		m_pcMenu3->ShowWindow(SW_HIDE);
	if(m_pcMenu4)
		m_pcMenu4->ShowWindow(SW_HIDE);
	if(m_pcMenu5)
		m_pcMenu5->ShowWindow(SW_HIDE);
	/*if(m_pcMenuNum)
		m_pcMenuNum->ShowWindow(SW_HIDE);*/
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

LRESULT CWndMenuTrend::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
{
	switch(message)
	{
	case WM_MENUBUTTONDOWN:
		{
			switch(wParam)
			{
			case IDC_BTN_TREND1:
				{

					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_TREND1);

					//SetTimer(TRENDBUTTON, 1000, NULL);
					return 1;
				}
				break;
			case IDC_BTN_TREND2:
				{

					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_TREND2);
					return 1;
				}
				break;
			case IDC_BTN_TREND3:
				{
					
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_TREND3);

					SetTimer(TRENDLEFT, 700, NULL);
					return 1;
				}
				break;
			case IDC_BTN_TREND4:
				{

					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_TREND4);

					SetTimer(TRENDRIGHT, 700, NULL);
					return 1;
				}
				break;
			case IDC_BTN_TREND5:
				{

					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_TREND5);
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
	case WM_MENUBUTTONUP:
		{
			switch(wParam)
			{
			case IDC_BTN_TREND1:
				{
					//KillTimer(TRENDBUTTON);
				}
				break;
			case IDC_BTN_TREND2:
				{
					//KillTimer(TRENDBUTTON);
				}
				break;
			case IDC_BTN_TREND3:
				{
					KillTimer(TRENDLEFT);
					return 1;
				}
				break;
			case IDC_BTN_TREND4:
				{
					KillTimer(TRENDRIGHT);
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

//void CWndMenuTrend::OnBnClickedMenu1()
//{
//	CMVEventUI event(CMVEventUI::EV_BN_TREND1);
//	getModel()->Trigger(&event);
//}
//void CWndMenuTrend::OnBnClickedMenu2()
//{
//	CMVEventUI event(CMVEventUI::EV_BN_TREND2);
//	getModel()->Trigger(&event);
//}
//void CWndMenuTrend::OnBnClickedMenu3()
//{
//	CMVEventUI event(CMVEventUI::EV_BN_TREND3);
//	getModel()->Trigger(&event);
//}
//void CWndMenuTrend::OnBnClickedMenu4()
//{
//	CMVEventUI event(CMVEventUI::EV_BN_TREND4);
//	getModel()->Trigger(&event);
//}
//void CWndMenuTrend::OnBnClickedMenu5()
//{
//	CMVEventUI event(CMVEventUI::EV_BN_TREND5);
//	getModel()->Trigger(&event);
//}

//void CWndMenuTrend::SetOneButtonDepressed(int btnID)
//{
//	POSITION pos;
//
//	for( pos = m_plMenuBtn.GetHeadPosition(); pos != NULL; )
//	{
//		CMenuBtn* pUDBtn = m_plMenuBtn.GetNext( pos );
//
//		if(pUDBtn->GetBtnId() != btnID)
//		{
//			if(pUDBtn->IsDepressed())
//			{
//				pUDBtn->Depress(false);
//			}
//		}
//		else
//		{
//			pUDBtn->Depress(true);
//		}
//	}
//}

//void CWndMenuTrend::SetAllButtonUnpressed()
//{
//	POSITION pos;
//
//	for( pos = m_plMenuBtn.GetHeadPosition(); pos != NULL; )
//	{
//		CMenuBtn* pUDBtn = m_plMenuBtn.GetNext( pos );
//
//		if(pUDBtn->IsDepressed())
//		{
//			pUDBtn->Depress(false);
//		}
//	}
//}

/**********************************************************************************************//**
 * Paints this window
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndMenuTrend::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Fügen Sie hier Ihren Meldungsbehandlungscode ein.
	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,m_hDC,0,0,SRCCOPY);


	// CMVView::OnPaint() soll zum Zeichnen von Meldungen nicht aufgerufen werden.
}

/**********************************************************************************************//**
 * Executes the destroy action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndMenuTrend::OnDestroy() 
{
	KillTimer(TRENDLEFT);
	KillTimer(TRENDRIGHT);

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
 * Executes the timer action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	nIDEvent	The identifier event.
 **************************************************************************************************/

void CWndMenuTrend::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==TRENDLEFT)
	{
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_TREND3);
	}
	else if(nIDEvent==TRENDRIGHT)
	{
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_TREND4);
	}

	CWnd::OnTimer(nIDEvent);
}
