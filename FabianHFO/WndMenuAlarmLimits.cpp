// WndMenuAlarmLimits.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "WndMenuAlarmLimits.h"
#include "LangAdmin.h"
#include "globDefs.h"
#include "MVViewHandler.h"

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
//extern HFONT g_hf13AcuBold;

//extern CLangAdmin* g_pGlobalLanguageStrings;

/**********************************************************************************************//**
 * CWndMenuAlarmLimits
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CWndMenuAlarmLimits, CWnd)

/**********************************************************************************************//**
 * Initializes a new instance of the CWndMenuAlarmLimits class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	parentView	If non-null, the parent view.
 **************************************************************************************************/

CWndMenuAlarmLimits::CWndMenuAlarmLimits(CMVView *parentView)
{
	m_pModel = NULL;

	m_parentView=parentView;

	//m_bIsLoud = false;

	m_hDC = NULL;
	m_hBmp = NULL;
	m_hBmpPrev = NULL;
	m_pcMenu2 = NULL;
	m_pcMenu3 = NULL;
	m_pcMenu5 = NULL;
	m_pcMenuNum = NULL;

	m_eAlarmVolume=getModel()->getSOUND()->GetAlarmVolume();

	//if(m_eAlarmVolume==ALARMMEDIUM)
	//{
	//	m_pcMenu3->SetBitmaps(m_pcAlMedium_Up,m_pcAlMedium_Dw,m_pcAlMedium_Up,m_pcAlMedium_Up);
	//}
	//else if(m_eAlarmVolume==ALARMLOUD)
	//{
	//	m_pcMenu3->SetBitmaps(m_pcAlLoud_Up,m_pcAlLoud_Dw,m_pcAlLoud_Up,m_pcAlLoud_Up);
	//}
	//else //ALARMLOUD
	//{
	//	m_pcMenu3->SetBitmaps(m_pcAlSilent_Up,m_pcAlSilent_Dw,m_pcAlSilent_Up,m_pcAlSilent_Up);
	//}

	m_lX				= 0;
	m_lY				= 0;
	m_lXo				= 0;
	m_lYo				= 0;

	m_pcMenu_AutosetUp=NULL;
	m_pcMenu_AutosetDw=NULL;

	m_pcMenu_Up=NULL;
	m_pcMenu_Dw=NULL;
	//m_pcAlOff_Up=NULL;
	//m_pcAlOff_Dw=NULL;
	m_pcAlLoud_Up=NULL;
	m_pcAlLoud_Dw=NULL;
	m_pcAlMedium_Up=NULL;
	m_pcAlMedium_Dw=NULL;
	m_pcAlSilent_Up=NULL;
	m_pcAlSilent_Dw=NULL;
	m_pcNumeric_Up=NULL;
	m_pcNumeric_Dw=NULL;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CWndMenuAlarmLimits class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

CWndMenuAlarmLimits::~CWndMenuAlarmLimits()
{
	/*delete m_pcMenu1;
	m_pcMenu1=NULL;*/
	delete m_pcMenu2;
	m_pcMenu2=NULL;
	delete m_pcMenu3;
	m_pcMenu3=NULL;
	/*delete m_pcMenu4;
	m_pcMenu4=NULL;*/
	delete m_pcMenu5;
	m_pcMenu5=NULL;
	delete m_pcMenuNum;
	m_pcMenuNum=NULL;

	delete m_pcMenu_Up;
	m_pcMenu_Up=NULL;
	delete m_pcMenu_Dw;
	m_pcMenu_Dw=NULL;
	//delete m_pcMenu_Fc;

	/*delete m_pcAlOff_Up;
	m_pcAlOff_Up=NULL;
	delete m_pcAlOff_Dw;
	m_pcAlOff_Dw=NULL;*/

	delete m_pcAlLoud_Up;
	m_pcAlLoud_Up=NULL;
	delete m_pcAlLoud_Dw;
	m_pcAlLoud_Dw=NULL;

	delete m_pcAlSilent_Up;
	m_pcAlSilent_Up=NULL;
	delete m_pcAlSilent_Dw;
	m_pcAlSilent_Dw=NULL;

	delete m_pcAlMedium_Up;
	m_pcAlMedium_Up=NULL;
	delete m_pcAlMedium_Dw;
	m_pcAlMedium_Dw=NULL;

	delete m_pcNumeric_Up;
	m_pcNumeric_Up=NULL;
	delete m_pcNumeric_Dw;
	m_pcNumeric_Dw=NULL;
	//delete m_pcNumeric_Fc;

	delete m_pcMenu_AutosetUp;
	m_pcMenu_AutosetUp=NULL;

	delete m_pcMenu_AutosetDw;
	m_pcMenu_AutosetDw=NULL;

	
	
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

CMVModel *CWndMenuAlarmLimits::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}


BEGIN_MESSAGE_MAP(CWndMenuAlarmLimits, CWnd)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	/*ON_BN_CLICKED(IDC_BTN_ALARM1, &CWndMenuAlarmLimits::OnBnClickedMenu1)
	ON_BN_CLICKED(IDC_BTN_ALARM2, &CWndMenuAlarmLimits::OnBnClickedMenu2)
	ON_BN_CLICKED(IDC_BTN_ALARM3, &CWndMenuAlarmLimits::OnBnClickedMenu3)
	ON_BN_CLICKED(IDC_BTN_ALARM4, &CWndMenuAlarmLimits::OnBnClickedMenu4)
	ON_BN_CLICKED(IDC_BTN_ALARM5, &CWndMenuAlarmLimits::OnBnClickedMenu5)*/
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * CWndMenuAlarmLimits message handlers
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

BOOL CWndMenuAlarmLimits::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext) 
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
		//DeleteObject(cbrBack);

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

void CWndMenuAlarmLimits::Init()
{
	CClientDC dc(this);

	//DWORD dwStyleNoTab = WS_CHILD|WS_VISIBLE|BS_OWNERDRAW|WS_TABSTOP;

	BTN btn;

	m_pcMenu_Up		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_5MENU_UP);
	m_pcMenu_Dw		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_5MENU_DW);
	//m_pcMenu_Fc		= new CBmp(theApp.m_hInstance,hdc,	IDB_MENUBTN_5FC);

	m_pcMenu_AutosetUp		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_AUTOSET_UP);
	m_pcMenu_AutosetDw		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_AUTOSET_DW);

	//m_pcAlOff_Up		= new CBmp(theApp.m_hInstance,hdc,	IDB_BTN_5MENU_AOFF_UP);
	//m_pcAlOff_Dw		= new CBmp(theApp.m_hInstance,hdc,	IDB_BTN_5MENU_AOFF_DW);

	m_pcAlLoud_Up		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_5MENU_ALOUD_UP);
	m_pcAlLoud_Dw		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_5MENU_ALOUD_DW);

	m_pcAlMedium_Up		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_5MENU_AMEDIUM_UP);
	m_pcAlMedium_Dw		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_5MENU_AMEDIUM_DW);

	m_pcAlSilent_Up		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_5MENU_ASILENT_UP);
	m_pcAlSilent_Dw		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_5MENU_ASILENT_DW);


	/*m_pcNumeric_Up		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_NUM_BACK_UP);
	m_pcNumeric_Dw		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_NUM_BACK_DW);*/
	m_pcNumeric_Up		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_ALIMITNUMERIC_UP);
	m_pcNumeric_Dw		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_ALIMITNUMERIC_DW);

	//Menu Buttons
	//btn.wID					= IDC_BTN_ALARM1;	
	//btn.poPosition.x		= 3;
	//btn.poPosition.y		= 0;//553;
	//btn.pcBmpUp				= m_pcMenu_Up;
	//btn.pcBmpDown			= m_pcMenu_Dw;
	//btn.pcBmpFocus			= m_pcMenu_Up;
	//btn.pcBmpDisabled		= m_pcMenu_Up;
	//btn.dwFormat			= DT_VCENTER|DT_CENTER;

	//CStringW szLang1 = getModel()->GetLanguageString(IDS_MENU_AUTOSET);

	//m_pcMenu1=new CAlarmBtn(btn,COLOR_TXTBTNUP,true);
	//m_pcMenu1->Create(this,g_hf11AcuBold,0);
	////m_pcMenu1->SetText(_T("Autoset"),_T("all"));
	//m_pcMenu1->SetText(getModel()->GetLanguageString(IDS_MENU_AUTOSET),getModel()->GetLanguageString(IDS_MENU_ALL));
	//m_pcMenu1->ShowWindow(SW_HIDE);
	//m_plMenuBtn.AddTail(m_pcMenu1);



	btn.wID					= IDC_BTN_ALIMI_AUTOSET;	
	btn.poPosition.x		= 120;
	btn.poPosition.y		= 0;//553;
	btn.pcBmpUp				= m_pcMenu_AutosetUp;
	btn.pcBmpDown			= m_pcMenu_AutosetDw;
	btn.pcBmpFocus			= m_pcMenu_AutosetUp;
	btn.pcBmpDisabled		= m_pcMenu_AutosetUp;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcMenu2=new CAlarmBtn(btn,COLOR_TXTBTNUP,true);
	m_pcMenu2->Create(this,g_hf11AcuBold,0);
	CString szEmpty=_T("");
	m_pcMenu2->SetText(getModel()->GetLanguageString(IDS_MENU_AUTOSET),szEmpty);
	m_pcMenu2->ShowWindow(SW_HIDE);
	//m_plMenuBtn.AddTail(m_pcMenu2);

	btn.wID					= IDC_BTN_ALARM3;	
	btn.poPosition.x		= 237;
	btn.poPosition.y		= 0;//553;

	if(getModel()->getSOUND()->GetAlarmVolume()==ALARMSILENT)
	{
		btn.pcBmpUp				= m_pcAlSilent_Up;
		btn.pcBmpDown			= m_pcAlSilent_Dw;
		btn.pcBmpFocus			= m_pcAlSilent_Up;
		btn.pcBmpDisabled		= m_pcAlSilent_Up;
	}
	else if(getModel()->getSOUND()->GetAlarmVolume()==ALARMMEDIUM)
	{
		btn.pcBmpUp				= m_pcAlMedium_Up;
		btn.pcBmpDown			= m_pcAlMedium_Dw;
		btn.pcBmpFocus			= m_pcAlMedium_Up;
		btn.pcBmpDisabled		= m_pcAlMedium_Up;
	}
	else
	{
		btn.pcBmpUp				= m_pcAlLoud_Up;
		btn.pcBmpDown			= m_pcAlLoud_Dw;
		btn.pcBmpFocus			= m_pcAlLoud_Up;
		btn.pcBmpDisabled		= m_pcAlLoud_Up;
	}
	
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcMenu3=new CAlarmBtn(btn,COLOR_TXTBTNUP);
	m_pcMenu3->Create(this,g_hf14AcuMed,0);
	m_pcMenu3->SetText(_T(""));
	m_pcMenu3->ShowWindow(SW_HIDE);
	//m_plMenuBtn.AddTail(m_pcMenu3);

	//btn.wID					= IDC_BTN_ALARM4;	
	//btn.poPosition.x		= 354;
	//btn.poPosition.y		= 0;//553;
	//btn.pcBmpUp				= m_pcMenu_Up;
	//btn.pcBmpDown			= m_pcMenu_Dw;
	//btn.pcBmpFocus			= m_pcMenu_Up;
	//btn.pcBmpDisabled		= m_pcMenu_Up;
	//btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	//m_pcMenu4=new CAlarmBtn(btn,COLOR_TXTBTNUP,true);
	//m_pcMenu4->Create(this,g_hf11AcuBold,0);
	//m_pcMenu4->SetText(getModel()->GetLanguageString(IDS_MENU_ALARM),getModel()->GetLanguageString(IDS_MENU_RESET));
	//m_pcMenu4->ShowWindow(SW_HIDE);

	btn.wID					= IDC_BTN_ALARM5;	
	btn.poPosition.x		= 471;
	btn.poPosition.y		= 0;//553;
	btn.pcBmpUp				= m_pcMenu_Up;
	btn.pcBmpDown			= m_pcMenu_Dw;
	btn.pcBmpFocus			= m_pcMenu_Up;
	btn.pcBmpDisabled		= m_pcMenu_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcMenu5=new CAlarmBtn(btn,COLOR_TXTBTNUP);
	m_pcMenu5->Create(this,g_hf14AcuMed,0);
	if(getModel()->getVIEWHANDLER()->getViewSubState()!=VSS_ALIMIT_LOGLIST)
	{
		m_pcMenu5->SetText(getModel()->GetLanguageString(IDS_MENU_SHOWLOG));
	}
	else
	{
		m_pcMenu5->SetText(getModel()->GetLanguageString(IDS_MENU_CLOSELOG));
	}
	
	m_pcMenu5->ShowWindow(SW_HIDE);
	//m_plMenuBtn.AddTail(m_pcMenu5);

	btn.wID					= IDC_BTN_MENU_NUMERIC;	
	btn.poPosition.x		= 625;
	btn.poPosition.y		= 0;//553;
	btn.pcBmpUp				= m_pcNumeric_Up;
	btn.pcBmpDown			= m_pcNumeric_Dw;
	btn.pcBmpFocus			= m_pcNumeric_Up;
	btn.pcBmpDisabled		= m_pcNumeric_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcMenuNum=new CPushBtn(btn,COLOR_TXTBTNUP,true);
	m_pcMenuNum->Create(this,g_hf14AcuMed,0);
	eALimitNumeric eCurALimitNumeric=getModel()->getCONFIG()->getCurALimitNumeric();

	bool bNumericChanged=false;
	if(		eCurALimitNumeric==ALIMITNUM_ETCO2
		&&	getModel()->getCONFIG()->getCO2module()==CO2MODULE_NONE)
	{
		eCurALimitNumeric=ALIMITNUM_VENTILATION;
		getModel()->getCONFIG()->setCurALimitNumeric(eCurALimitNumeric);
		bNumericChanged=true;
	}
	else if(	eCurALimitNumeric==ALIMITNUM_SPO2
			&&	getModel()->getCONFIG()->getSPO2module()==SPO2MODULE_NONE)
	{
		eCurALimitNumeric=ALIMITNUM_VENTILATION;
		getModel()->getCONFIG()->setCurALimitNumeric(eCurALimitNumeric);
		bNumericChanged=true;
	}
	switch(eCurALimitNumeric)
	{
	case ALIMITNUM_ETCO2:
		{
			if(getModel()->getCONFIG()->getSPO2module()!=SPO2MODULE_NONE)
			{
				CString szEmpty=_T("");
				m_pcMenuNum->SetText(getModel()->GetLanguageString(IDS_MENU_ALIMIT_SPO2),szEmpty);
				//m_pcMenuNum->SetText(_T("show SPO2"),_T(""));
			}
			else
			{
				CString szEmpty=_T("");
				m_pcMenuNum->SetText(getModel()->GetLanguageString(IDS_MENU_ALIMIT_VENT),szEmpty);
				//m_pcMenuNum->SetText(_T("show ventilation"),_T(""));
			}
		}
		break;
	case ALIMITNUM_SPO2:
		{
			CString szEmpty=_T("");
			m_pcMenuNum->SetText(getModel()->GetLanguageString(IDS_MENU_ALIMIT_VENT),szEmpty);
			//m_pcMenuNum->SetText(_T("show ventilation"),_T(""));
		}
		break;
	default:
		{
			if(getModel()->getCONFIG()->getCO2module()!=CO2MODULE_NONE)
			{	
				CString szEmpty=_T("");
				m_pcMenuNum->SetText(getModel()->GetLanguageString(IDS_MENU_ALIMIT_CO2),szEmpty);
				//m_pcMenuNum->SetText(_T("show etCO2"),_T(""));
			}
			else if(getModel()->getCONFIG()->getSPO2module()!=SPO2MODULE_NONE)
			{
				CString szEmpty=_T("");
				m_pcMenuNum->SetText(getModel()->GetLanguageString(IDS_MENU_ALIMIT_SPO2),szEmpty);
				//m_pcMenuNum->SetText(_T("show SPO2"),_T(""));
			}
			else
				m_pcMenuNum->SetText(_T(""),_T(""));
		}
		break;
	}
	
	m_pcMenuNum->ShowWindow(SW_HIDE);


	if(m_eAlarmVolume==ALARMMEDIUM)
	{
		m_pcMenu3->SetBitmaps(m_pcAlMedium_Up,m_pcAlMedium_Dw,m_pcAlMedium_Up,m_pcAlMedium_Up);
	}
	else if(m_eAlarmVolume==ALARMLOUD)
	{
		m_pcMenu3->SetBitmaps(m_pcAlLoud_Up,m_pcAlLoud_Dw,m_pcAlLoud_Up,m_pcAlLoud_Up);
	}
	else //ALARMLOUD
	{
		m_pcMenu3->SetBitmaps(m_pcAlSilent_Up,m_pcAlSilent_Dw,m_pcAlSilent_Up,m_pcAlSilent_Up);
	}

	ShowMenuBtn();

	if(bNumericChanged)
	{
		if(m_parentView)
			m_parentView->PostMessage(WM_EV_ALIMIT_NUMERIC);
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

void CWndMenuAlarmLimits::Show(BOOL bShow)
{
	if(this->IsWindowVisible() == bShow)
	{
		return;
	}

	if(bShow==TRUE)
	{
		this->ShowWindow(SW_SHOW);
		ShowMenuBtn();
	}
	else
	{
		this->ShowWindow(SW_HIDE);
		HideMenuBtn();
	}

	
}

/**********************************************************************************************//**
 * Shows the menu button
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndMenuAlarmLimits::ShowMenuBtn()
{
	if(getModel()->getCONFIG()->IsEnableAutolimitActive())
	{
		/*if(m_pcMenu1)
			m_pcMenu1->ShowWindow(SW_SHOW);*/
		if(m_pcMenu2)
			m_pcMenu2->ShowWindow(SW_SHOW);
	}
	else
	{
		/*if(m_pcMenu1)
			m_pcMenu1->ShowWindow(SW_HIDE);*/
		if(m_pcMenu2)
			m_pcMenu2->ShowWindow(SW_HIDE);
	}
	
	if(m_pcMenu3)
		m_pcMenu3->ShowWindow(SW_SHOW);
	/*if(m_pcMenu4)
		m_pcMenu4->ShowWindow(SW_SHOW);*/
	if(m_pcMenu5)
		m_pcMenu5->ShowWindow(SW_SHOW);

	if(		getModel()->getCONFIG()->getCO2module()==CO2MODULE_NONE
		&&	getModel()->getCONFIG()->getSPO2module()==SPO2MODULE_NONE)
	{
		if(m_pcMenuNum)
			m_pcMenuNum->ShowWindow(SW_HIDE);
	}
	else
	{
		if(m_pcMenuNum)
			m_pcMenuNum->ShowWindow(SW_SHOW);
	}
	
}

/**********************************************************************************************//**
 * Hides the menu button
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndMenuAlarmLimits::HideMenuBtn()
{
	/*if(m_pcMenu1)
		m_pcMenu1->ShowWindow(SW_HIDE);*/
	if(m_pcMenu2)
		m_pcMenu2->ShowWindow(SW_HIDE);
	if(m_pcMenu3)
		m_pcMenu3->ShowWindow(SW_HIDE);
	/*if(m_pcMenu4)
		m_pcMenu4->ShowWindow(SW_HIDE);*/
	if(m_pcMenu5)
		m_pcMenu5->ShowWindow(SW_HIDE);
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

LRESULT CWndMenuAlarmLimits::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
{
	switch(message)
	{
	case WM_MENUBUTTONDOWN:
		{
			switch(wParam)
			{
			/*case IDC_BTN_ALARM1:
				{
					OnBnClickedMenu1();
					return 1;
						
				}
				break;*/
			case IDC_BTN_ALIMI_AUTOSET:
				{
					OnBnClickedMenu2();
					return 1;

				}
				break;
			case IDC_BTN_ALARM3:
				{
					OnBnClickedMenu3();
					return 1;

				}
				break;
			/*case IDC_BTN_ALARM4:
				{
					OnBnClickedMenu4();
					return 1;

				}
				break;*/
			case IDC_BTN_ALARM5:
				{
					OnBnClickedMenu5();
					return 1;
				}
				break;
			case IDC_BTN_MENU_NUMERIC:
				{
					//BnMenuNum();
					/*if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_NUMERIC);*/
					OnBnALimitNumeric();
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
			//case IDC_BTN_ALARM1:
			case IDC_BTN_ALIMI_AUTOSET:
			case IDC_BTN_ALARM3:
			case IDC_BTN_MENU_NUMERIC:
			case IDC_BTN_ALARM5:
				{
					if(m_parentView)
						m_parentView->PostMessage(WM_ALIMIT_SETFOCUS);
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
	case WM_ALIMIT_LOGLIST:
		{
			if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_ALIMIT_LOGLIST)
			{
				if(m_pcMenu5)
					m_pcMenu5->RefreshText(getModel()->GetLanguageString(IDS_MENU_CLOSELOG));
			}
			else
			{
				if(m_pcMenu5)
					m_pcMenu5->RefreshText(getModel()->GetLanguageString(IDS_MENU_SHOWLOG));
			}
			return 1;
		}
		break;
	case WM_ALIMIT_REFRESH:
		{
			refreshCurALimitNumeric();
		}
		break;
	}
	return CWnd::WindowProc(message, wParam, lParam);
}

/**********************************************************************************************//**
 * Refresh current a limit numeric
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndMenuAlarmLimits::refreshCurALimitNumeric()
{
	eALimitNumeric eCurALimitNumeric=getModel()->getCONFIG()->getCurALimitNumeric();

	if(		eCurALimitNumeric==ALIMITNUM_ETCO2
		&&	getModel()->getCONFIG()->getCO2module()==CO2MODULE_NONE)
	{
		eCurALimitNumeric=ALIMITNUM_VENTILATION;
		getModel()->getCONFIG()->setCurALimitNumeric(eCurALimitNumeric);
	}
	else if(	eCurALimitNumeric==ALIMITNUM_SPO2
		&&	getModel()->getCONFIG()->getSPO2module()==SPO2MODULE_NONE)
	{
		eCurALimitNumeric=ALIMITNUM_VENTILATION;
		getModel()->getCONFIG()->setCurALimitNumeric(eCurALimitNumeric);
	}
	switch(eCurALimitNumeric)
	{
	case ALIMITNUM_ETCO2:
		{
			if(getModel()->getCONFIG()->getSPO2module()!=SPO2MODULE_NONE)
			{
				CString szEmpty=_T("");
				m_pcMenuNum->SetText(getModel()->GetLanguageString(IDS_MENU_ALIMIT_SPO2),szEmpty);
				//m_pcMenuNum->RefreshText(_T("show SPO2"),_T(""));
			}
			else
			{
				CString szEmpty=_T("");
				m_pcMenuNum->SetText(getModel()->GetLanguageString(IDS_MENU_ALIMIT_VENT),szEmpty);
				//m_pcMenuNum->RefreshText(_T("show ventilation"),_T(""));
			}
		}
		break;
	case ALIMITNUM_SPO2:
		{
			CString szEmpty=_T("");
			m_pcMenuNum->SetText(getModel()->GetLanguageString(IDS_MENU_ALIMIT_VENT),szEmpty);
			//m_pcMenuNum->RefreshText(_T("show ventilation"),_T(""));
		}
		break;
	default:
		{
			if(getModel()->getCONFIG()->getCO2module()!=CO2MODULE_NONE)
			{
				CString szEmpty=_T("");
				m_pcMenuNum->SetText(getModel()->GetLanguageString(IDS_MENU_ALIMIT_CO2),szEmpty);
				//m_pcMenuNum->RefreshText(_T("show etCO2"),_T(""));
			}
			else if(getModel()->getCONFIG()->getSPO2module()!=SPO2MODULE_NONE)
			{
				CString szEmpty=_T("");
				m_pcMenuNum->SetText(getModel()->GetLanguageString(IDS_MENU_ALIMIT_SPO2),szEmpty);
				//m_pcMenuNum->RefreshText(_T("show SPO2"),_T(""));
			}
			else
				m_pcMenuNum->RefreshText(_T(""),_T(""));
		}
		break;
	}
}

//void CWndMenuAlarmLimits::OnBnClickedMenu1()
//{
//	/*if(AfxGetApp())
//		AfxGetApp()->GetMainWnd()->PostMessage(WM_CALCULATE_ALL_AUTOLIMIT);*/
//}

/**********************************************************************************************//**
 * Executes the button clicked menu 2 action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndMenuAlarmLimits::OnBnClickedMenu2()
{
	/*CMVEventUI event(CMVEventUI::EV_BN_ALIMIT2);
	getModel()->Trigger(&event);*/
	m_parentView->PostMessage(WM_ALIMIT_AUTOSET_SINGLE);
}

/**********************************************************************************************//**
 * Executes the button clicked menu 3 action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndMenuAlarmLimits::OnBnClickedMenu3()
{

	if(m_eAlarmVolume==ALARMSILENT)
	{
		m_pcMenu3->SetBitmaps(m_pcAlMedium_Up,m_pcAlMedium_Dw,m_pcAlMedium_Up,m_pcAlMedium_Up);
		m_eAlarmVolume=ALARMMEDIUM;
		getModel()->getSOUND()->SetAlarmVolume(ALARMMEDIUM);
	}
	else if(m_eAlarmVolume==ALARMMEDIUM)
	{
		m_pcMenu3->SetBitmaps(m_pcAlLoud_Up,m_pcAlLoud_Dw,m_pcAlLoud_Up,m_pcAlLoud_Up);
		m_eAlarmVolume=ALARMLOUD;
		getModel()->getSOUND()->SetAlarmVolume(ALARMLOUD);
	}
	else //ALARMLOUD
	{
		m_pcMenu3->SetBitmaps(m_pcAlSilent_Up,m_pcAlSilent_Dw,m_pcAlSilent_Up,m_pcAlSilent_Up);
		m_eAlarmVolume=ALARMSILENT;
		getModel()->getSOUND()->SetAlarmVolume(ALARMSILENT);
	}
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_DOUBLESIGNAL);

	//m_parentView->PostMessage(WM_ALIMIT_SETFOCUS);
}

//void CWndMenuAlarmLimits::OnBnClickedMenu4()
//{
//	if(AfxGetApp())
//		AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_RESET_ALARMS);
//}

/**********************************************************************************************//**
 * Executes the button clicked menu 5 action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndMenuAlarmLimits::OnBnClickedMenu5()
{
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_ALIMIT5);
}

/**********************************************************************************************//**
 * Executes the button a limit numeric action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndMenuAlarmLimits::OnBnALimitNumeric()
{
	eALimitNumeric eCurALimitNumeric=getModel()->getCONFIG()->getCurALimitNumeric();
	switch(eCurALimitNumeric)
	{
	case ALIMITNUM_ETCO2:
		{
			if(getModel()->getCONFIG()->getSPO2module()!=SPO2MODULE_NONE)
			{
				eCurALimitNumeric=ALIMITNUM_SPO2;
				CString szEmpty=_T("");
				m_pcMenuNum->SetText(getModel()->GetLanguageString(IDS_MENU_ALIMIT_VENT),szEmpty);
				//m_pcMenuNum->RefreshText(_T("show ventilation"),_T(""));
			}
			else
			{
				eCurALimitNumeric=ALIMITNUM_VENTILATION;
				CString szEmpty=_T("");
				m_pcMenuNum->SetText(getModel()->GetLanguageString(IDS_MENU_ALIMIT_CO2),szEmpty);
				//m_pcMenuNum->RefreshText(_T("show etCO2"),_T(""));
			}
		}
		break;
	case ALIMITNUM_SPO2:
		{
			if(getModel()->getCONFIG()->getCO2module()!=CO2MODULE_NONE)
			{
				eCurALimitNumeric=ALIMITNUM_VENTILATION;
				CString szEmpty=_T("");
				m_pcMenuNum->SetText(getModel()->GetLanguageString(IDS_MENU_ALIMIT_CO2),szEmpty);
				//m_pcMenuNum->RefreshText(_T("show etCO2"),_T(""));
			}
			else
			{
				eCurALimitNumeric=ALIMITNUM_VENTILATION;
				CString szEmpty=_T("");
				m_pcMenuNum->SetText(getModel()->GetLanguageString(IDS_MENU_ALIMIT_SPO2),szEmpty);
				//m_pcMenuNum->RefreshText(_T("show SPO2"),_T(""));
			}
		}
		break;
	default:
		{
			if(getModel()->getCONFIG()->getCO2module()!=CO2MODULE_NONE)
			{
				if(getModel()->getCONFIG()->getSPO2module()!=SPO2MODULE_NONE)
				{
					CString szEmpty=_T("");
					m_pcMenuNum->SetText(getModel()->GetLanguageString(IDS_MENU_ALIMIT_SPO2),szEmpty);
					//m_pcMenuNum->RefreshText(_T("show SPO2"),_T(""));
				}
				else
				{
					CString szEmpty=_T("");
					m_pcMenuNum->SetText(getModel()->GetLanguageString(IDS_MENU_ALIMIT_VENT),szEmpty);
					//m_pcMenuNum->RefreshText(_T("show ventilation"),_T(""));
				}
				
				eCurALimitNumeric=ALIMITNUM_ETCO2;
			}
			else if(getModel()->getCONFIG()->getSPO2module()!=SPO2MODULE_NONE)
			{
				eCurALimitNumeric=ALIMITNUM_SPO2;
				CString szEmpty=_T("");
				m_pcMenuNum->SetText(getModel()->GetLanguageString(IDS_MENU_ALIMIT_VENT),szEmpty);
				//m_pcMenuNum->RefreshText(_T("show ventilation"),_T(""));
			}
		}
		break;
	}

	getModel()->getCONFIG()->setCurALimitNumeric(eCurALimitNumeric);

	if(m_parentView)
		m_parentView->PostMessage(WM_EV_ALIMIT_NUMERIC);
}

/**********************************************************************************************//**
 * Paints this window
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndMenuAlarmLimits::OnPaint()
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

void CWndMenuAlarmLimits::OnDestroy() 
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



