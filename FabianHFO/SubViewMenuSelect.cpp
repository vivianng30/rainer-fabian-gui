// SubViewMenuSelect.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "SubViewMenuSelect.h"

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
extern HFONT g_hf15AcuMed;
extern HFONT g_hf17AcuBold;
extern HFONT g_hf19AcuMed;
extern HFONT g_hf21AcuBold;
extern HFONT g_hf23AcuBold;

/**********************************************************************************************//**
 * CSubViewMenuSelect
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CSubViewMenuSelect, CWnd)

/**********************************************************************************************//**
 * Initializes a new instance of the CSubViewMenuSelect class
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

CSubViewMenuSelect::CSubViewMenuSelect()
{
	m_pModel=NULL;

	m_lX=0;
	m_lY=0;
	m_lXo=0;
	m_lYo=0;

	m_hDC = NULL;
	m_hBmp= NULL;
	m_hBmpPrev= NULL;

	m_pcMenuCalibration=NULL;
	m_pcMenuDisplay=NULL;
	m_pcMenuVentilation=NULL;
	m_pcMenuPatient=NULL;
	m_pcMenuLanguage=NULL;
	m_pcMenuDate=NULL;
	m_pcMenuInfo=NULL;
	m_pcMenuService=NULL;
	m_pcMenuTools=NULL;
	//m_pcMenuHelp=NULL;
	//m_pcMenuCO2=NULL;
	//m_pcMenuVideo=NULL;
	m_pcMenuNetwork=NULL;
	//m_pcMenuSpO2=NULL;

	m_pcMenuCalibration_UP=NULL;
	m_pcMenuCalibration_DW=NULL;
	m_pcMenuCalibration_FC=NULL;

	m_pcMenuDisplay_UP=NULL;
	m_pcMenuDisplay_DW=NULL;
	m_pcMenuDisplay_FC=NULL;

	m_pcMenuVentilation_UP=NULL;
	m_pcMenuVentilation_DW=NULL;
	m_pcMenuVentilation_FC=NULL;

	m_pcMenuPatient_UP=NULL;
	m_pcMenuPatient_DW=NULL;
	m_pcMenuPatient_FC=NULL;

	m_pcMenuLanguage_UP=NULL;
	m_pcMenuLanguage_DW=NULL;
	m_pcMenuLanguage_FC=NULL;

	m_pcMenuDate_UP=NULL;
	m_pcMenuDate_DW=NULL;
	m_pcMenuDate_FC=NULL;

	m_pcMenuInfo_UP=NULL;
	m_pcMenuInfo_DW=NULL;
	m_pcMenuInfo_FC=NULL;

	m_pcMenuService_UP=NULL;
	m_pcMenuService_DW=NULL;
	m_pcMenuService_FC=NULL;

	m_pcMenuTools_UP=NULL;
	m_pcMenuTools_DW=NULL;
	m_pcMenuTools_FC=NULL;
	m_pcMenuTools_DIS=NULL;

	m_pcMenuHelp_UP=NULL;
	m_pcMenuHelp_DW=NULL;
	m_pcMenuHelp_FC=NULL;
	m_pcMenuHelp_DIS=NULL;

	/*m_pcMenuCO2_UP=NULL;
	m_pcMenuCO2_DW=NULL;
	m_pcMenuCO2_FC=NULL;
	m_pcMenuCO2_DIS=NULL;

	m_pcMenuSPO2_UP=NULL;
	m_pcMenuSPO2_DW=NULL;
	m_pcMenuSPO2_FC=NULL;
	m_pcMenuSPO2_DIS=NULL;*/

	/*m_pcMenuVideo_UP=NULL;
	m_pcMenuVideo_DW=NULL;
	m_pcMenuVideo_FC=NULL;
	m_pcMenuVideo_DIS=NULL;*/

	m_pcMenuNetwork_UP=NULL;
	m_pcMenuNetwork_DW=NULL;
	m_pcMenuNetwork_FC=NULL;
	m_pcMenuNetwork_DIS=NULL;

	m_iCurrentField=0;

	m_dwLastSetupTimer=0;

	//m_bCO2FeatureAvailable=getModel()->getDATAHANDLER()->isCO2LicenseAvailable();
	m_bCO2FeatureAvailable=true;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CSubViewMenuSelect class
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

CSubViewMenuSelect::~CSubViewMenuSelect()
{
	delete m_pcMenuCalibration;
	m_pcMenuCalibration=NULL;
	delete m_pcMenuDisplay;
	m_pcMenuDisplay=NULL;
	delete m_pcMenuVentilation;
	m_pcMenuVentilation=NULL;
	delete m_pcMenuPatient;
	m_pcMenuPatient=NULL;
	delete m_pcMenuLanguage;
	m_pcMenuLanguage=NULL;
	delete m_pcMenuDate;
	m_pcMenuDate=NULL;
	delete m_pcMenuInfo;
	m_pcMenuInfo=NULL;
	delete m_pcMenuService;
	m_pcMenuService=NULL;
	delete m_pcMenuTools;
	m_pcMenuTools=NULL;
	/*delete m_pcMenuHelp;
	m_pcMenuHelp=NULL;*/
	/*delete m_pcMenuCO2;
	m_pcMenuCO2=NULL;*/
	/*delete m_pcMenuVideo;
	m_pcMenuVideo=NULL;*/
	delete m_pcMenuNetwork;
	m_pcMenuNetwork=NULL;
	/*delete m_pcMenuSpO2;
	m_pcMenuSpO2=NULL;*/

	delete m_pcMenuCalibration_UP;
	m_pcMenuCalibration_UP=NULL;
	delete m_pcMenuCalibration_DW;
	m_pcMenuCalibration_DW=NULL;
	delete m_pcMenuCalibration_FC;
	m_pcMenuCalibration_FC=NULL;

	delete m_pcMenuDisplay_UP;
	m_pcMenuDisplay_UP=NULL;
	delete m_pcMenuDisplay_DW;
	m_pcMenuDisplay_DW=NULL;
	delete m_pcMenuDisplay_FC;
	m_pcMenuDisplay_FC=NULL;

	delete m_pcMenuVentilation_UP;
	m_pcMenuVentilation_UP=NULL;
	delete m_pcMenuVentilation_DW;
	m_pcMenuVentilation_DW=NULL;
	delete m_pcMenuVentilation_FC;
	m_pcMenuVentilation_FC=NULL;

	delete m_pcMenuPatient_UP;
	m_pcMenuPatient_UP=NULL;
	delete m_pcMenuPatient_DW;
	m_pcMenuPatient_DW=NULL;
	delete m_pcMenuPatient_FC;
	m_pcMenuPatient_FC=NULL;

	delete m_pcMenuLanguage_UP;
	m_pcMenuLanguage_UP=NULL;
	delete m_pcMenuLanguage_DW;
	m_pcMenuLanguage_DW=NULL;
	delete m_pcMenuLanguage_FC;
	m_pcMenuLanguage_FC=NULL;

	delete m_pcMenuDate_UP;
	m_pcMenuDate_UP=NULL;
	delete m_pcMenuDate_DW;
	m_pcMenuDate_DW=NULL;
	delete m_pcMenuDate_FC;
	m_pcMenuDate_FC=NULL;

	delete m_pcMenuInfo_UP;
	m_pcMenuInfo_UP=NULL;
	delete m_pcMenuInfo_DW;
	m_pcMenuInfo_DW=NULL;
	delete m_pcMenuInfo_FC;
	m_pcMenuInfo_FC=NULL;

	delete m_pcMenuService_UP;
	m_pcMenuService_UP=NULL;
	delete m_pcMenuService_DW;
	m_pcMenuService_DW=NULL;
	delete m_pcMenuService_FC;
	m_pcMenuService_FC=NULL;

	delete m_pcMenuTools_UP;
	m_pcMenuTools_UP=NULL;
	delete m_pcMenuTools_DW;
	m_pcMenuTools_DW=NULL;
	delete m_pcMenuTools_FC;
	m_pcMenuTools_FC=NULL;
	delete m_pcMenuTools_DIS;
	m_pcMenuTools_DIS=NULL;

	delete m_pcMenuHelp_UP;
	m_pcMenuHelp_UP=NULL;
	delete m_pcMenuHelp_DW;
	m_pcMenuHelp_DW=NULL;
	delete m_pcMenuHelp_FC;
	m_pcMenuHelp_FC=NULL;
	delete m_pcMenuHelp_DIS;
	m_pcMenuHelp_DIS=NULL;

	/*delete m_pcMenuCO2_UP;
	m_pcMenuCO2_UP=NULL;
	delete m_pcMenuCO2_DW;
	m_pcMenuCO2_DW=NULL;
	delete m_pcMenuCO2_FC;
	m_pcMenuCO2_FC=NULL;
	delete m_pcMenuCO2_DIS;
	m_pcMenuCO2_DIS=NULL;

	delete m_pcMenuSPO2_UP;
	m_pcMenuSPO2_UP=NULL;
	delete m_pcMenuSPO2_DW;
	m_pcMenuSPO2_DW=NULL;
	delete m_pcMenuSPO2_FC;
	m_pcMenuSPO2_FC=NULL;
	delete m_pcMenuSPO2_DIS;
	m_pcMenuSPO2_DIS=NULL;*/

	/*delete m_pcMenuVideo_UP;
	m_pcMenuVideo_UP=NULL;
	delete m_pcMenuVideo_DW;
	m_pcMenuVideo_DW=NULL;
	delete m_pcMenuVideo_FC;
	m_pcMenuVideo_FC=NULL;
	delete m_pcMenuVideo_DIS;
	m_pcMenuVideo_DIS=NULL;*/

	delete m_pcMenuNetwork_UP;
	m_pcMenuNetwork_UP=NULL;
	delete m_pcMenuNetwork_DW;
	m_pcMenuNetwork_DW=NULL;
	delete m_pcMenuNetwork_FC;
	m_pcMenuNetwork_FC=NULL;
	delete m_pcMenuNetwork_DIS;
	m_pcMenuNetwork_DIS=NULL;
}


BEGIN_MESSAGE_MAP(CSubViewMenuSelect, CWnd)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTNMNU_CAL, &CSubViewMenuSelect::OnBnClickedCalibration)
	ON_BN_CLICKED(IDC_BTNMNU_DISPL, &CSubViewMenuSelect::OnBnClickedDisplay)
	ON_BN_CLICKED(IDC_BTNMNU_VENT, &CSubViewMenuSelect::OnBnClickedVentilation)
	ON_BN_CLICKED(IDC_BTNMNU_PAT, &CSubViewMenuSelect::OnBnClickedPatientdata)
	ON_BN_CLICKED(IDC_BTNMNU_LANG, &CSubViewMenuSelect::OnBnClickedLanguage)
	ON_BN_CLICKED(IDC_BTNMNU_DATE, &CSubViewMenuSelect::OnBnClickedDate)
	ON_BN_CLICKED(IDC_BTNMNU_INFO, &CSubViewMenuSelect::OnBnClickedInfo)
	ON_BN_CLICKED(IDC_BTNMNU_SERVICE, &CSubViewMenuSelect::OnBnClickedService)
	ON_BN_CLICKED(IDC_BTNMNU_TOOLS, &CSubViewMenuSelect::OnBnClickedTools)
	//ON_BN_CLICKED(IDC_BTNMNU_HELP, &CSubViewMenuSelect::OnBnClickedHelp)
	//ON_BN_CLICKED(IDC_BTNMNU_CO2, &CSubViewMenuSelect::OnBnClickedCO2)
	//ON_BN_CLICKED(IDC_BTNMNU_SPO2, &CSubViewMenuSelect::OnBnClickedSPO2)
	ON_BN_CLICKED(IDC_BTNMNU_VIDEO, &CSubViewMenuSelect::OnBnClickedVideo)
	ON_BN_CLICKED(IDC_BTNMNU_NETWORK, &CSubViewMenuSelect::OnBnClickedNetwork)
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * CSubViewMenuSelect message handlers
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	Null if it fails, else the model.
 **************************************************************************************************/

CMVModel *CSubViewMenuSelect::getModel()
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

bool CSubViewMenuSelect::CreateWnd(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext)
{
	if (!Create(pParentWnd, rc, nID, pContext))
	{
		return false;
	}


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

BOOL CSubViewMenuSelect::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext) 
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

		//CBrush cbrBack(BACKGND);
		CBrush cbrBack;
		cbrBack.CreateSolidBrush(BACKGND);
		HBRUSH hbrprev=(HBRUSH)SelectObject(m_hDC,cbrBack);
		HPEN hpenprev=(HPEN)SelectObject(m_hDC, (HPEN)GetStockObject(NULL_PEN));	

		Rectangle(m_hDC, 0, 0, m_lX, m_lY);

		m_pcMenuCalibration_UP= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTNMNU_CAL_UP);
		m_pcMenuCalibration_DW= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTNMNU_CAL_DW);
		m_pcMenuCalibration_FC= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTNMNU_CAL_FC);
		
		m_pcMenuDisplay_UP= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTNMNU_DISPL_UP);
		m_pcMenuDisplay_DW= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTNMNU_DISPL_DW);
		m_pcMenuDisplay_FC= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTNMNU_DISPL_FC);

		m_pcMenuVentilation_UP= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTNMNU_VENT_UP);
		m_pcMenuVentilation_DW= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTNMNU_VENT_DW);
		m_pcMenuVentilation_FC= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTNMNU_VENT_FC);

		m_pcMenuPatient_UP= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTNMNU_PAT_UP);
		m_pcMenuPatient_DW= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTNMNU_PAT_DW);
		m_pcMenuPatient_FC= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTNMNU_PAT_FC);

		m_pcMenuLanguage_UP= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTNMNU_LANG_UP);
		m_pcMenuLanguage_DW= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTNMNU_LANG_DW);
		m_pcMenuLanguage_FC= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTNMNU_LANG_FC);

		m_pcMenuDate_UP= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTNMNU_DATE_UP);
		m_pcMenuDate_DW= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTNMNU_DATE_DW);
		m_pcMenuDate_FC= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTNMNU_DATE_FC);

		m_pcMenuInfo_UP= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTNMNU_INFO_UP);
		m_pcMenuInfo_DW= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTNMNU_INFO_DW);
		m_pcMenuInfo_FC= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTNMNU_INFO_FC);

		m_pcMenuService_UP= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTNMNU_SERVICE_UP);
		m_pcMenuService_DW= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTNMNU_SERVICE_DW);
		m_pcMenuService_FC= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTNMNU_SERVICE_FC);

		m_pcMenuTools_UP= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTNMNU_TOOLS_UP);
		m_pcMenuTools_DW= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTNMNU_TOOLS_DW);
		m_pcMenuTools_FC= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTNMNU_TOOLS_FC);
		m_pcMenuTools_DIS= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTNMNU_TOOLS_DIS);

		m_pcMenuHelp_UP= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTNMNU_HELP_UP);
		m_pcMenuHelp_DW= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTNMNU_HELP_DW);
		m_pcMenuHelp_FC= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTNMNU_HELP_FC);
		m_pcMenuHelp_DIS= new CBmp(theApp.m_hInstance,m_hDC, IDB_BTNMNU_HELP_DIS);

		/*m_pcMenuCO2_UP= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTNMNU_CO2_UP);
		m_pcMenuCO2_DW= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTNMNU_CO2_DW);
		m_pcMenuCO2_FC= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTNMNU_CO2_FC);
		m_pcMenuCO2_DIS= new CBmp(theApp.m_hInstance,m_hDC, IDB_BTNMNU_CO2_DIS);

		m_pcMenuSPO2_UP= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTNMNU_SPO2_UP);
		m_pcMenuSPO2_DW= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTNMNU_SPO2_DW);
		m_pcMenuSPO2_FC= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTNMNU_SPO2_FC);
		m_pcMenuSPO2_DIS= new CBmp(theApp.m_hInstance,m_hDC, IDB_BTNMNU_SPO2_DIS);*/

		/*m_pcMenuVideo_UP= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTNMNU_VIDEO_UP);
		m_pcMenuVideo_DW= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTNMNU_VIDEO_DW);
		m_pcMenuVideo_FC= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTNMNU_VIDEO_FC);
		m_pcMenuVideo_DIS= new CBmp(theApp.m_hInstance,m_hDC, IDB_BTNMNU_VIDEO_DIS);*/

		m_pcMenuNetwork_UP= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTNMNU_NETWORK_UP);
		m_pcMenuNetwork_DW= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTNMNU_NETWORK_DW);
		m_pcMenuNetwork_FC= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTNMNU_NETWORK_FC);
		m_pcMenuNetwork_DIS= new CBmp(theApp.m_hInstance,m_hDC, IDB_BTNMNU_NETWORK_DIS);

		

		HFONT hPrevFont=(HFONT)SelectObject(m_hDC,g_hf21AcuBold);
		int nBkMode=SetBkMode(m_hDC,TRANSPARENT);

		
		CStringW cs=_T("");
		BTN btn;

		//Menu Buttons
		/*****************************CALIBRATION***************************/
		btn.wID					= IDC_BTNMNU_CAL;	
		btn.poPosition.x		= 44;
		btn.poPosition.y		= 30;
		btn.pcBmpUp				= m_pcMenuCalibration_UP;
		btn.pcBmpDown			= m_pcMenuCalibration_DW;
		btn.pcBmpFocus			= m_pcMenuCalibration_FC;
		btn.pcBmpDisabled		= m_pcMenuCalibration_UP;
		btn.dwFormat			= DT_BOTTOM|DT_SINGLELINE|DT_CENTER;

		m_pcMenuCalibration=new CMenuBtn(btn,COLOR_TXTBTNUP);
		m_pcMenuCalibration->Create(this,g_hf13AcuBold,0,10);
		m_pcMenuCalibration->SetText(getModel()->GetLanguageString(IDS_BTN_MNU_CAL),getModel()->GetLanguageString(IDS_BTN_MNU_CAL));
		//m_pcMenuCalibration->ShowWindow(SW_HIDE);
		m_plMenuBtn.AddTail(m_pcMenuCalibration);
		

		/*****************************DISPLAY***************************/
		btn.wID					= IDC_BTNMNU_DISPL;	
		btn.poPosition.x		= 233;
		btn.poPosition.y		= 30;
		btn.pcBmpUp				= m_pcMenuDisplay_UP;
		btn.pcBmpDown			= m_pcMenuDisplay_DW;
		btn.pcBmpFocus			= m_pcMenuDisplay_FC;
		btn.pcBmpDisabled		= m_pcMenuDisplay_UP;
		btn.dwFormat			= DT_BOTTOM|DT_SINGLELINE|DT_CENTER;

		m_pcMenuDisplay=new CMenuBtn(btn,COLOR_TXTBTNUP);
		m_pcMenuDisplay->Create(this,g_hf13AcuBold,0,10);
		m_pcMenuDisplay->SetText(getModel()->GetLanguageString(IDS_BTN_MNU_DIS),getModel()->GetLanguageString(IDS_BTN_MNU_DIS));
		//m_pcMenuDisplay->ShowWindow(SW_HIDE);
		m_plMenuBtn.AddTail(m_pcMenuDisplay);



		/*****************************VENTILATION***************************/
		btn.wID					= IDC_BTNMNU_VENT;	
		btn.poPosition.x		= 422;
		btn.poPosition.y		= 30;
		btn.pcBmpUp				= m_pcMenuVentilation_UP;
		btn.pcBmpDown			= m_pcMenuVentilation_DW;
		btn.pcBmpFocus			= m_pcMenuVentilation_FC;
		btn.pcBmpDisabled		= m_pcMenuVentilation_UP;
		btn.dwFormat			= DT_BOTTOM|DT_SINGLELINE|DT_CENTER;

		m_pcMenuVentilation=new CMenuBtn(btn,COLOR_TXTBTNUP);
		m_pcMenuVentilation->Create(this,g_hf13AcuBold,0,10);
		m_pcMenuVentilation->SetText(getModel()->GetLanguageString(IDS_BTN_MNU_VENT),getModel()->GetLanguageString(IDS_BTN_MNU_VENT));
		//m_pcMenuVentilation->ShowWindow(SW_HIDE);
		m_plMenuBtn.AddTail(m_pcMenuVentilation);


		/*****************************PATIENTDATA***************************/
		btn.wID					= IDC_BTNMNU_PAT;	
		btn.poPosition.x		= 611;
		btn.poPosition.y		= 30;
		btn.pcBmpUp				= m_pcMenuPatient_UP;
		btn.pcBmpDown			= m_pcMenuPatient_DW;
		btn.pcBmpFocus			= m_pcMenuPatient_FC;
		btn.pcBmpDisabled		= m_pcMenuPatient_UP;
		btn.dwFormat			= DT_BOTTOM|DT_SINGLELINE|DT_CENTER;

		m_pcMenuPatient=new CMenuBtn(btn,COLOR_TXTBTNUP);
		m_pcMenuPatient->Create(this,g_hf13AcuBold,0,10);
		m_pcMenuPatient->SetText(getModel()->GetLanguageString(IDS_BTN_MNU_PAT),getModel()->GetLanguageString(IDS_BTN_MNU_PAT));
		//m_pcMenuPatient->ShowWindow(SW_HIDE);
		m_plMenuBtn.AddTail(m_pcMenuPatient);


		/*****************************LANGUAGE***************************/
		btn.wID					= IDC_BTNMNU_LANG;	
		btn.poPosition.x		= 44;
		btn.poPosition.y		= 150;
		btn.pcBmpUp				= m_pcMenuLanguage_UP;
		btn.pcBmpDown			= m_pcMenuLanguage_DW;
		btn.pcBmpFocus			= m_pcMenuLanguage_FC;
		btn.pcBmpDisabled		= m_pcMenuLanguage_UP;
		btn.dwFormat			= DT_BOTTOM|DT_SINGLELINE|DT_CENTER;

		m_pcMenuLanguage=new CMenuBtn(btn,COLOR_TXTBTNUP);
		m_pcMenuLanguage->Create(this,g_hf13AcuBold,0,10);
		m_pcMenuLanguage->SetText(getModel()->GetLanguageString(IDS_BTN_MNU_LANG),getModel()->GetLanguageString(IDS_BTN_MNU_LANG));
		//m_pcMenuLanguage->ShowWindow(SW_HIDE);
		m_plMenuBtn.AddTail(m_pcMenuLanguage);


		/*****************************DATETIME***************************/
		btn.wID					= IDC_BTNMNU_DATE;	
		btn.poPosition.x		= 233;
		btn.poPosition.y		= 150;
		btn.pcBmpUp				= m_pcMenuDate_UP;
		btn.pcBmpDown			= m_pcMenuDate_DW;
		btn.pcBmpFocus			= m_pcMenuDate_FC;
		btn.pcBmpDisabled		= m_pcMenuDate_UP;
		btn.dwFormat			= DT_BOTTOM|DT_SINGLELINE|DT_CENTER;

		m_pcMenuDate=new CMenuBtn(btn,COLOR_TXTBTNUP);
		m_pcMenuDate->Create(this,g_hf13AcuBold,0,10);
		m_pcMenuDate->SetText(getModel()->GetLanguageString(IDS_BTN_MNU_DATETIME),getModel()->GetLanguageString(IDS_BTN_MNU_DATETIME));
		//m_pcMenuDate->ShowWindow(SW_HIDE);
		m_plMenuBtn.AddTail(m_pcMenuDate);



		/*****************************TOOLS***************************/
		btn.wID					= IDC_BTNMNU_TOOLS;	
		btn.poPosition.x		= 422;
		btn.poPosition.y		= 150;
		btn.pcBmpUp				= m_pcMenuTools_UP;
		btn.pcBmpDown			= m_pcMenuTools_DW;
		btn.pcBmpFocus			= m_pcMenuTools_FC;
		btn.pcBmpDisabled		= m_pcMenuTools_DIS;
		btn.dwFormat			= DT_BOTTOM|DT_SINGLELINE|DT_CENTER;

		m_pcMenuTools=new CMenuBtn(btn,COLOR_TXTBTNUP);
		m_pcMenuTools->Create(this,g_hf13AcuBold,0,10);
		m_pcMenuTools->SetText(getModel()->GetLanguageString(IDS_BTN_MNU_TOOLS),getModel()->GetLanguageString(IDS_BTN_MNU_TOOLS));
		//m_pcMenuTools->EnableWindow(FALSE);
		//m_pcMenuTools->ShowWindow(SW_HIDE);
		m_plMenuBtn.AddTail(m_pcMenuTools);


		/*****************************INFO***************************/
		btn.wID					= IDC_BTNMNU_INFO;	
		btn.poPosition.x		= 611;
		btn.poPosition.y		= 150;
		btn.pcBmpUp				= m_pcMenuInfo_UP;
		btn.pcBmpDown			= m_pcMenuInfo_DW;
		btn.pcBmpFocus			= m_pcMenuInfo_FC;
		btn.pcBmpDisabled		= m_pcMenuInfo_UP;
		btn.dwFormat			= DT_BOTTOM|DT_SINGLELINE|DT_CENTER;

		m_pcMenuInfo=new CMenuBtn(btn,COLOR_TXTBTNUP);
		m_pcMenuInfo->Create(this,g_hf13AcuBold,0,10);
		m_pcMenuInfo->SetText(getModel()->GetLanguageString(IDS_BTN_MNU_INFO),getModel()->GetLanguageString(IDS_BTN_MNU_INFO));
		//m_pcMenuInfo->ShowWindow(SW_HIDE);
		m_plMenuBtn.AddTail(m_pcMenuInfo);


		


		/*****************************CO2***************************/
		//btn.wID					= IDC_BTNMNU_CO2;	
		//btn.poPosition.x		= 233;
		//btn.poPosition.y		= 270;
		//btn.pcBmpUp				= m_pcMenuCO2_UP;
		//btn.pcBmpDown			= m_pcMenuCO2_DW;
		//btn.pcBmpFocus			= m_pcMenuCO2_FC;
		//btn.pcBmpDisabled		= m_pcMenuCO2_DIS;
		////btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;
		//btn.dwFormat			= DT_BOTTOM|DT_SINGLELINE|DT_CENTER;

		//m_pcMenuCO2=new CMenuBtn(btn,COLOR_TXTBTNUP);
		//m_pcMenuCO2->Create(this,g_hf13AcuBold,0,10);
		//m_pcMenuCO2->SetText(getModel()->GetLanguageString(IDS_BTN_MNU_CO2),getModel()->GetLanguageString(IDS_BTN_MNU_CO2));
		//m_plMenuBtn.AddTail(m_pcMenuCO2);


		/*****************************Video***************************/
		//btn.wID					= IDC_BTNMNU_VIDEO;	
		//btn.poPosition.x		= 44;//422;
		//btn.poPosition.y		= 270;
		//btn.pcBmpUp				= m_pcMenuVideo_UP;
		//btn.pcBmpDown			= m_pcMenuVideo_DW;
		//btn.pcBmpFocus			= m_pcMenuVideo_FC;
		//btn.pcBmpDisabled		= m_pcMenuVideo_DIS;
		////btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;
		//btn.dwFormat			= DT_BOTTOM|DT_SINGLELINE|DT_CENTER;

		//m_pcMenuVideo=new CMenuBtn(btn,COLOR_TXTBTNUP);
		//m_pcMenuVideo->Create(this,g_hf13AcuBold,0,10);
		//m_pcMenuVideo->SetText(getModel()->GetLanguageString(IDS_BTN_MNU_VIDEO),getModel()->GetLanguageString(IDS_BTN_MNU_VIDEO));
		//if(theApp.IsNetDCU9())
		//{
		//	m_plMenuBtn.AddTail(m_pcMenuVideo);
		//}
		//else
		//{
		//	m_pcMenuVideo->EnableWindow(FALSE);
		//	//m_pcMenuVideo->ShowWindow(SW_HIDE);
		//}


		/*****************************NETWORK***************************/
		btn.wID					= IDC_BTNMNU_NETWORK;	
		btn.poPosition.x		= 44;//233;//611;
		btn.poPosition.y		= 270;
		btn.pcBmpUp				= m_pcMenuNetwork_UP;
		btn.pcBmpDown			= m_pcMenuNetwork_DW;
		btn.pcBmpFocus			= m_pcMenuNetwork_FC;
		btn.pcBmpDisabled		= m_pcMenuNetwork_DIS;
		//btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;
		btn.dwFormat			= DT_BOTTOM|DT_SINGLELINE|DT_CENTER;

		m_pcMenuNetwork=new CMenuBtn(btn,COLOR_TXTBTNUP);
		m_pcMenuNetwork->Create(this,g_hf13AcuBold,0,10);
		m_pcMenuNetwork->SetText(getModel()->GetLanguageString(IDS_BTN_MNU_PDMS),getModel()->GetLanguageString(IDS_BTN_MNU_PDMS));
		//m_pcMenuNetwork->EnableWindow(FALSE);
		//m_plMenuBtn.AddTail(m_pcMenuNetwork);
		if(/*getModel()->getAcuLink()!=NULL 
			&& */getModel()->getCONFIG()->GetPDMSprotocol()!=ACL_NOPDMS)
		{
			m_plMenuBtn.AddTail(m_pcMenuNetwork);
		}
		else
		{
			m_pcMenuNetwork->EnableWindow(FALSE);
			//m_pcMenuVideo->ShowWindow(SW_HIDE);
		}


		/*****************************SPO2***************************/
		//btn.wID					= IDC_BTNMNU_SPO2;	
		//btn.poPosition.x		= 44;
		//btn.poPosition.y		= 270;
		//btn.pcBmpUp				= m_pcMenuSPO2_UP;
		//btn.pcBmpDown			= m_pcMenuSPO2_DW;
		//btn.pcBmpFocus			= m_pcMenuSPO2_FC;
		//btn.pcBmpDisabled		= m_pcMenuSPO2_DIS;
		////btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;
		//btn.dwFormat			= DT_BOTTOM|DT_SINGLELINE|DT_CENTER;

		//m_pcMenuSpO2=new CMenuBtn(btn,COLOR_TXTBTNUP);
		//m_pcMenuSpO2->Create(this,g_hf13AcuBold,0,10);
		//m_pcMenuSpO2->SetText(getModel()->GetLanguageString(IDS_BTN_MNU_SPO2),getModel()->GetLanguageString(IDS_BTN_MNU_SPO2));
		////m_pcMenuHelp->EnableWindow(FALSE);
		////m_pcMenuSpO2->ShowWindow(SW_HIDE);
		//m_plMenuBtn.AddTail(m_pcMenuSpO2);

		/*****************************HELP***************************/
		//btn.wID					= IDC_BTNMNU_HELP;	
		//btn.poPosition.x		= 611;
		//btn.poPosition.y		= 390;
		//btn.pcBmpUp				= m_pcMenuHelp_UP;
		//btn.pcBmpDown			= m_pcMenuHelp_DW;
		//btn.pcBmpFocus			= m_pcMenuHelp_FC;
		//btn.pcBmpDisabled		= m_pcMenuHelp_DIS;
		////btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;
		//btn.dwFormat			= DT_BOTTOM|DT_SINGLELINE|DT_CENTER;

		//m_pcMenuHelp=new CMenuBtn(btn,COLOR_TXTBTNUP);
		//m_pcMenuHelp->Create(this,g_hf13AcuBold,0,10);
		//m_pcMenuHelp->SetText(getModel()->GetLanguageString(IDS_BTN_MNU_HELP),getModel()->GetLanguageString(IDS_BTN_MNU_HELP));
		////m_pcMenuHelp->EnableWindow(FALSE);
		//m_pcMenuHelp->ShowWindow(SW_HIDE);
		////m_plMenuBtn.AddTail(m_pcMenuHelp);




		/*****************************SERVICE***************************/
		btn.wID					= IDC_BTNMNU_SERVICE;	
		btn.poPosition.x		= 611;//44;
		btn.poPosition.y		= 390;//270;
		btn.pcBmpUp				= m_pcMenuService_UP;
		btn.pcBmpDown			= m_pcMenuService_DW;
		btn.pcBmpFocus			= m_pcMenuService_FC;
		btn.pcBmpDisabled		= m_pcMenuService_UP;
		btn.dwFormat			= DT_BOTTOM|DT_SINGLELINE|DT_CENTER;

		m_pcMenuService=new CMenuBtn(btn,COLOR_TXTBTNUP);
		m_pcMenuService->Create(this,g_hf13AcuBold,0,10);
		m_pcMenuService->SetText(getModel()->GetLanguageString(IDS_BTN_MNU_SERVICE),getModel()->GetLanguageString(IDS_BTN_MNU_SERVICE));
		//m_pcMenuService->ShowWindow(SW_HIDE);
		m_plMenuBtn.AddTail(m_pcMenuService);

		

		SetBkMode(m_hDC,nBkMode);
		SelectObject(m_hDC,hPrevFont);
		SelectObject(m_hDC,hbrprev);
		SelectObject(m_hDC,hpenprev);
	
		

		return 1;
	}
	else
		return 0;
}

/**********************************************************************************************//**
 * Sets one button depressed
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	btnID	Identifier for the button.
 **************************************************************************************************/

void CSubViewMenuSelect::SetOneButtonDepressed(int btnID)
{
	POSITION pos;

	try
	{
		for( pos = m_plMenuBtn.GetHeadPosition(); pos != NULL; )
		{
			CMenuBtn* pUDBtn = m_plMenuBtn.GetNext( pos );

			if(pUDBtn->GetBtnId() != btnID)
			{
				if(pUDBtn->IsDepressed())
				{
					pUDBtn->Depress(false);
				}
			}
			else
			{
				pUDBtn->Depress(true);
			}
		}
	}
	catch (...)
	{
		theApp.ReportException(_T("CSubViewMenuSelect::SetOneButtonDepressed"));
	}
	

	if(GetParent())
		GetParent()->PostMessage(WM_SET_SETUPTIMER);


}

/**********************************************************************************************//**
 * Sets all button unpressed
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CSubViewMenuSelect::SetAllButtonUnpressed()
{
	POSITION pos;

	for( pos = m_plMenuBtn.GetHeadPosition(); pos != NULL; )
	{
		CMenuBtn* pUDBtn = m_plMenuBtn.GetNext( pos );

		if(pUDBtn->IsDepressed())
		{
			pUDBtn->Depress(false);
		}

	}
	if(GetParent())
		GetParent()->PostMessage(WM_SET_SETUPTIMER);
}

/**********************************************************************************************//**
 * Sets one button focused
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	btnID	Identifier for the button.
 **************************************************************************************************/

void CSubViewMenuSelect::SetOneButtonFocused(int btnID)
{
	if(getModel()->isSafeTickCountDelayExpired(m_dwLastSetupTimer, 1000))
	{
		m_dwLastSetupTimer=GetTickCount();
		if(GetParent())
			GetParent()->PostMessage(WM_SET_SETUPTIMER);
	}
}

/**********************************************************************************************//**
 * Sets next button focused
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CSubViewMenuSelect::SetNextButtonFocused()
{
	if(getModel()->isSafeTickCountDelayExpired(m_dwLastSetupTimer, 1000))
	{
		m_dwLastSetupTimer=GetTickCount();
		if(GetParent())
			GetParent()->PostMessage(WM_SET_SETUPTIMER);
	}
}

/**********************************************************************************************//**
 * Sets previous button focused
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CSubViewMenuSelect::SetPrevButtonFocused()
{
	if(getModel()->isSafeTickCountDelayExpired(m_dwLastSetupTimer, 1000))
	{
		m_dwLastSetupTimer=GetTickCount();
		if(GetParent())
			GetParent()->PostMessage(WM_SET_SETUPTIMER);
	}
}

/**********************************************************************************************//**
 * Executes the button clicked calibration action
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CSubViewMenuSelect::OnBnClickedCalibration()
{
	if(GetParent())
		GetParent()->PostMessage(WM_MENU_CALIBRATION);
}

/**********************************************************************************************//**
 * Executes the button clicked display action
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CSubViewMenuSelect::OnBnClickedDisplay()
{
	if(GetParent())
		GetParent()->PostMessage(WM_MENU_DISPLAY);
}

/**********************************************************************************************//**
 * Executes the button clicked ventilation action
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CSubViewMenuSelect::OnBnClickedVentilation()
{
	if(GetParent())
		GetParent()->PostMessage(WM_MENU_VENTILATION);
}

/**********************************************************************************************//**
 * Executes the button clicked patientdata action
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CSubViewMenuSelect::OnBnClickedPatientdata()
{
	if(GetParent())
		GetParent()->PostMessage(WM_MENU_PATIENTDATA);
}

/**********************************************************************************************//**
 * Executes the button clicked language action
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CSubViewMenuSelect::OnBnClickedLanguage()
{
	if(GetParent())
		GetParent()->PostMessage(WM_MENU_LANGUAGE);
}

/**********************************************************************************************//**
 * Executes the button clicked date action
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CSubViewMenuSelect::OnBnClickedDate()
{
	if(GetParent())
		GetParent()->PostMessage(WM_MENU_DATETIME);
}

/**********************************************************************************************//**
 * Executes the button clicked information action
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CSubViewMenuSelect::OnBnClickedInfo()
{
	if(GetParent())
		GetParent()->PostMessage(WM_MENU_INFO);
}

/**********************************************************************************************//**
 * Executes the button clicked service action
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CSubViewMenuSelect::OnBnClickedService()
{
	if(GetParent())
		GetParent()->PostMessage(WM_MENU_SERVICE);
}

/**********************************************************************************************//**
 * Executes the button clicked tools action
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CSubViewMenuSelect::OnBnClickedTools()
{
	if(GetParent())
		GetParent()->PostMessage(WM_MENU_TOOLS);
}

/**********************************************************************************************//**
 * Executes the button clicked video action
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CSubViewMenuSelect::OnBnClickedVideo()
{
	if(GetParent())
		GetParent()->PostMessage(WM_MENU_VIDEO);
}

/**********************************************************************************************//**
 * Executes the button clicked network action
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CSubViewMenuSelect::OnBnClickedNetwork()
{
	if(GetParent())
		GetParent()->PostMessage(WM_MENU_NETWORK);
}

/**********************************************************************************************//**
 * Window proc
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	message	The message.
 * \param	wParam 	The wParam field of the message.
 * \param	lParam 	The lParam field of the message.
 *
 * \return	A LRESULT.
 **************************************************************************************************/

LRESULT CSubViewMenuSelect::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
{
	switch(message)
	{
	case WM_MENUBUTTONDOWN:
		{
			SetOneButtonDepressed(wParam);
			return 1;
		}
		break;
	default:
		{
			
		}
		break;
	}
	return CWnd::WindowProc(message, wParam, lParam);
}

/**********************************************************************************************//**
 * Translates all messages before they are processed by the main message loop
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param [in,out]	pMsg	If non-null, the message.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

BOOL CSubViewMenuSelect::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message)
	{
	case WM_KEYDOWN:
		if(pMsg->wParam==VK_SPACE)
		{
			if(GetParent())
				GetParent()->PostMessage(WM_SET_SETUPTIMER);
			return 1;
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
 * Shows
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	bShow	True to show, false to hide.
 * \param	view 	The view.
 **************************************************************************************************/

void CSubViewMenuSelect::Show(bool bShow , eMenuView view)
{
	if(bShow)
	{
		ShowWindow(SW_SHOW);
		if(GetParent())
			GetParent()->PostMessage(WM_SET_SETUPTIMER);

		switch(view)
		{
		case MV_CALIBRATION:
			{
				SetOneButtonDepressed(IDC_BTNMNU_CAL);
			}
			break;
		case MV_DISPLAY:
			{
				SetOneButtonDepressed(IDC_BTNMNU_DISPL);
			}
			break;
		case MV_VENTILATION:
			{
				SetOneButtonDepressed(IDC_BTNMNU_VENT);
			}
			break;
		case MV_PATIENTDATA:
			{
				SetOneButtonDepressed(IDC_BTNMNU_PAT);
			}
			break;
		case MV_LANGUAGE:
			{
				SetOneButtonDepressed(IDC_BTNMNU_LANG);
			}
			break;
		case MV_DATETIME:
			{
				SetOneButtonDepressed(IDC_BTNMNU_DATE);
			}
			break;
		case MV_TOOLS:
			{
				SetOneButtonDepressed(IDC_BTNMNU_TOOLS);
			}
			break;
		case MV_SYSTEM:
			{
				SetOneButtonDepressed(IDC_BTNMNU_INFO);
			}
			break;
		case MV_SERVICEMODE:
			{
				SetOneButtonDepressed(IDC_BTNMNU_SERVICE);
			}
			break;
		/*case MV_CO2:
			{
				SetOneButtonDepressed(IDC_BTNMNU_CO2);
			}
			break;
		case MV_SPO2:
			{
				SetOneButtonDepressed(IDC_BTNMNU_SPO2);
			}
			break;*/
		case MV_VIDEO:
			{
				SetOneButtonDepressed(IDC_BTNMNU_VIDEO);
			}
			break;
		case MV_NETWORK:
			{
				SetOneButtonDepressed(IDC_BTNMNU_NETWORK);
			}
			break;
		/*case MV_HELP:
			{
				SetOneButtonDepressed(IDC_BTNMNU_HELP);
			}
			break;*/
		default:
			{
				
			}
			break;
		}
	}
	else
	{
		ShowWindow(SW_HIDE);
		//SetWindowPos(NULL,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_HIDEWINDOW);
	}
}

/**********************************************************************************************//**
 * Paints this window
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CSubViewMenuSelect::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Fügen Sie hier Ihren Meldungsbehandlungscode ein.
	// CWnd::OnPaint() soll zum Zeichnen von Meldungen nicht aufgerufen werden.
	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,m_hDC,0,0,SRCCOPY);
}

/**********************************************************************************************//**
 * Executes the destroy action
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CSubViewMenuSelect::OnDestroy()
{
	KillTimer(CHANGETIMER);

	m_plMenuBtn.RemoveAll();

	CWnd::OnDestroy();

	if(m_hBmp)
	{
		SelectObject(m_hDC,m_hBmpPrev);
		DeleteObject(m_hBmp);
	}
	if(m_hDC)
		DeleteDC(m_hDC);
}
