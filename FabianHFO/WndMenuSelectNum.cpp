// WndMenuSelectNum.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "WndMenuSelectNum.h"
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
 * CWndMenuSelectNum
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CWndMenuSelectNum, CWnd)

/**********************************************************************************************//**
 * Initializes a new instance of the CWndMenuSelectNum class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	parentView	If non-null, the parent view.
 **************************************************************************************************/

CWndMenuSelectNum::CWndMenuSelectNum(CMVView *parentView)
{

	m_parentView=parentView;

	m_lX				= 0;
	m_lY				= 0;
	m_lXo				= 0;
	m_lYo				= 0;

	m_hDC=NULL;
	m_hBmp=NULL;
	m_hBmpPrev=NULL;

	m_pcMenu_Up=NULL;
	m_pcMenu_Dw=NULL;
	m_pcNumeric_Up=NULL;
	m_pcNumeric_Dw=NULL;

	m_pModel = NULL;

	m_pcMenuNum=NULL;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CWndMenuSelectNum class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

CWndMenuSelectNum::~CWndMenuSelectNum()
{
	/*delete m_pcMenu1;
	m_pcMenu1=NULL;
	delete m_pcMenu2;
	m_pcMenu2=NULL;
	delete m_pcMenu3;
	m_pcMenu3=NULL;
	delete m_pcMenu4;
	m_pcMenu4=NULL;
	delete m_pcMenu5;
	m_pcMenu5=NULL;*/
	delete m_pcMenuNum;
	m_pcMenuNum=NULL;

	delete m_pcMenu_Up;
	m_pcMenu_Up=NULL;
	delete m_pcMenu_Dw;
	m_pcMenu_Dw=NULL;
	//delete m_pcMenu_Fc;
	//m_pcMenu_Fc=NULL;


	delete m_pcNumeric_Up;
	m_pcNumeric_Up=NULL;
	delete m_pcNumeric_Dw;
	m_pcNumeric_Dw=NULL;
	//delete m_pcNumeric_Fc;
	//m_pcNumeric_Fc=NULL;
}

/**********************************************************************************************//**
 * Gets the model
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	Null if it fails, else the model.
 **************************************************************************************************/

CMVModel *CWndMenuSelectNum::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}


BEGIN_MESSAGE_MAP(CWndMenuSelectNum, CWnd)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	/*ON_BN_CLICKED(IDC_BTN_SETUP1, &CWndMenuSelectNum::OnBnClickedMenu1)
	ON_BN_CLICKED(IDC_BTN_SETUP2, &CWndMenuSelectNum::OnBnClickedMenu2)
	ON_BN_CLICKED(IDC_BTN_SETUP3, &CWndMenuSelectNum::OnBnClickedMenu3)
	ON_BN_CLICKED(IDC_BTN_SETUP4, &CWndMenuSelectNum::OnBnClickedMenu4)
	ON_BN_CLICKED(IDC_BTN_SETUP5, &CWndMenuSelectNum::OnBnClickedMenu5)*/
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * CWndMenuSetup message handlers
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

BOOL CWndMenuSelectNum::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext*) 
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

void CWndMenuSelectNum::Init()
{
	CClientDC dc(this);

	BTN btn;
	
	m_pcMenu_Up		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_5MENU_UP);
	m_pcMenu_Dw		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_5MENU_DW);
	//m_pcMenu_Fc		= new CBmp(theApp.m_hInstance,hdc,	IDB_MENUBTN_5FC);


	m_pcNumeric_Up		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_NUM_BACK_UP);
	m_pcNumeric_Dw		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_NUM_BACK_DW);
	//m_pcNumeric_Fc		= new CBmp(theApp.m_hInstance,hdc,	IDB_BTN_NUMMENUBCK_FC);

	//Menu Buttons
	//btn.wID					= IDC_BTN_SETUP1;	
	//btn.poPosition.x		= 3;
	//btn.poPosition.y		= 0;//553;
	//btn.pcBmpUp				= m_pcMenu_Up;
	//btn.pcBmpDown			= m_pcMenu_Dw;
	//btn.pcBmpFocus			= m_pcMenu_Up;
	//btn.pcBmpDisabled		= m_pcMenu_Up;
	//btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	//CStringW szLang = getModel()->GetLanguageString(IDS_MENU_LANGUAGE);

	//m_pcMenu1=new CMenuBtn(btn,COLOR_TXTBTNUP);
	//m_pcMenu1->Create(this,g_hf14AcuMed,0);
	//m_pcMenu1->SetText(szLang,szLang);
	//m_pcMenu1->ShowWindow(SW_HIDE);
	//m_plMenuBtn.AddTail(m_pcMenu1);



	//btn.wID					= IDC_BTN_SETUP2;	
	//btn.poPosition.x		= 120;//151;
	//btn.poPosition.y		= 0;
	//btn.pcBmpUp				= m_pcMenu_Up;
	//btn.pcBmpDown			= m_pcMenu_Dw;
	//btn.pcBmpFocus			= m_pcMenu_Up;
	//btn.pcBmpDisabled		= m_pcMenu_Up;
	//btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	//szLang = getModel()->GetLanguageString(IDS_MENU_DATETIME);

	//m_pcMenu2=new CMenuBtn(btn,COLOR_TXTBTNUP);
	//m_pcMenu2->Create(this,g_hf14AcuMed,0);
	//m_pcMenu2->SetText(szLang,szLang);
	//m_pcMenu2->ShowWindow(SW_HIDE);
	//m_plMenuBtn.AddTail(m_pcMenu2);

	//btn.wID					= IDC_BTN_SETUP3;	
	//btn.poPosition.x		= 237;//299;
	//btn.poPosition.y		= 0;//553;
	//btn.pcBmpUp				= m_pcMenu_Up;
	//btn.pcBmpDown			= m_pcMenu_Dw;
	//btn.pcBmpFocus			= m_pcMenu_Up;
	//btn.pcBmpDisabled		= m_pcMenu_Up;
	//btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	//szLang = getModel()->GetLanguageString(IDS_MENU_SETTINGS);

	//m_pcMenu3=new CMenuBtn(btn,COLOR_TXTBTNUP);
	//m_pcMenu3->Create(this,g_hf14AcuMed,0);
	//m_pcMenu3->SetText(szLang,szLang);
	//m_pcMenu3->ShowWindow(SW_HIDE);
	//m_plMenuBtn.AddTail(m_pcMenu3);

	//btn.wID					= IDC_BTN_SETUP4;	
	//btn.poPosition.x		= 354;//447;
	//btn.poPosition.y		= 0;//553;
	//btn.pcBmpUp				= m_pcMenu_Up;
	//btn.pcBmpDown			= m_pcMenu_Dw;
	//btn.pcBmpFocus			= m_pcMenu_Up;
	//btn.pcBmpDisabled		= m_pcMenu_Up;
	//btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	//szLang = getModel()->GetLanguageString(IDS_MENU_PATIENT);

	//m_pcMenu4=new CMenuBtn(btn,COLOR_TXTBTNUP);
	//m_pcMenu4->Create(this,g_hf14AcuMed,0);
	//m_pcMenu4->SetText(szLang,szLang);
	//m_pcMenu4->ShowWindow(SW_HIDE);
	//m_plMenuBtn.AddTail(m_pcMenu4);

	//btn.wID					= IDC_BTN_SETUP5;	
	//btn.poPosition.x		= 471;
	//btn.poPosition.y		= 0;//553;
	//btn.pcBmpUp				= m_pcMenu_Up;
	//btn.pcBmpDown			= m_pcMenu_Dw;
	//btn.pcBmpFocus			= m_pcMenu_Up;
	//btn.pcBmpDisabled		= m_pcMenu_Up;
	//btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	//szLang = getModel()->GetLanguageString(IDS_MENU_SYSTEM);

	//m_pcMenu5=new CMenuBtn(btn,COLOR_TXTBTNUP);
	//m_pcMenu5->Create(this,g_hf14AcuMed,0);
	//m_pcMenu5->SetText(szLang,szLang);
	//m_pcMenu5->ShowWindow(SW_HIDE);
	//m_plMenuBtn.AddTail(m_pcMenu5);

	btn.wID					= IDC_BTN_MENU_NUMERIC;	
	btn.poPosition.x		= 625;
	btn.poPosition.y		= 0;//553;
	btn.pcBmpUp				= m_pcNumeric_Up;
	btn.pcBmpDown			= m_pcNumeric_Dw;
	btn.pcBmpFocus			= m_pcNumeric_Up;
	btn.pcBmpDisabled		= m_pcNumeric_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcMenuNum=new CPushBtn(btn,COLOR_TXTBTNUP);
	m_pcMenuNum->Create(this,g_hf14AcuMed,0);
	m_pcMenuNum->SetText(_T(""));
	m_pcMenuNum->ShowWindow(SW_HIDE);

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

void CWndMenuSelectNum::Show(BOOL bShow)
{

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

void CWndMenuSelectNum::ShowMenuBtn()
{
	if(m_pcMenuNum)
		m_pcMenuNum->ShowWindow(SW_SHOW);

	//switch(getModel()->getCONFIG()->GetSetupView())
	//{
	//case SETUP_LANG:
	//	{
	//		SetOneButtonDepressed(IDC_BTN_SETUP1);
	//	}
	//	break;
	//case SETUP_DATETIME:
	//	{
	//		SetOneButtonDepressed(IDC_BTN_SETUP2);
	//	}
	//	break;
	//case SETUP_SETTINGS:
	//	{
	//		SetOneButtonDepressed(IDC_BTN_SETUP3);
	//	}
	//	break;
	//case SETUP_PATIENT:
	//	{
	//		SetOneButtonDepressed(IDC_BTN_SETUP4);
	//	}
	//	break;
	//case SETUP_INFO:
	//	{
	//		SetOneButtonDepressed(IDC_BTN_SETUP5);
	//	}
	//	break;
	//default:
	//	break;
	//}


}

/**********************************************************************************************//**
 * Hides the menu button
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndMenuSelectNum::HideMenuBtn()
{
	if(m_pcMenuNum)
		m_pcMenuNum->ShowWindow(SW_HIDE);
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

LRESULT CWndMenuSelectNum::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
{
	switch(message)
	{
	case WM_MENUBUTTONDOWN:
		{

			switch(wParam)
			{
			case IDC_BTN_MENU_NUMERIC:
				{
					BnClickedNumeric();
					return 1;
				}
				break;
			default:
				{
					//SetOneButtonDepressed(wParam);
				}
				break;
			}
		}
		break;
	}
	return CWnd::WindowProc(message, wParam, lParam);
}

//void CWndMenuSelectNum::OnBnClickedMenu1()
//{
//	/*CMVEventUI event(CMVEventUI::EV_BN_SETUP1);
//	getModel()->Trigger(&event);*/
//}
//void CWndMenuSelectNum::OnBnClickedMenu2()
//{
//	
//}
//void CWndMenuSelectNum::OnBnClickedMenu3()
//{
//	
//}
//void CWndMenuSelectNum::OnBnClickedMenu4()
//{
//	
//}
//void CWndMenuSelectNum::OnBnClickedMenu5()
//{
//	
//}

/**********************************************************************************************//**
 * Button clicked numeric
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndMenuSelectNum::BnClickedNumeric()
{
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_NUMERIC);
}

//void CWndMenuSelectNum::SetOneButtonDepressed(int btnID)
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

//void CWndMenuSelectNum::SetAllButtonUnpressed()
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

void CWndMenuSelectNum::OnPaint()
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

void CWndMenuSelectNum::OnDestroy() 
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

