// WndParaSettings.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "WndParaSettings.h"
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
 * A macro that defines color txtsubbtndw
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

#define COLOR_TXTSUBBTNDW			0x00000000

/**********************************************************************************************//**
 * A macro that defines color txtbtnfc
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

#define COLOR_TXTBTNFC				0x00000000

/**********************************************************************************************//**
 * A macro that defines color txtbtndis
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

#define COLOR_TXTBTNDIS				0x00828282


extern HFONT g_hf3AcuNorm;
extern HFONT g_hf4AcuNorm;
extern HFONT g_hf5AcuNorm;
extern HFONT g_hf6AcuNorm;
extern HFONT g_hf7AcuNorm;
extern HFONT g_hf7AcuBold;
extern HFONT g_hf8AcuNorm;
extern HFONT g_hf8AcuBold;
extern HFONT g_hf9AcuBold;
extern HFONT g_hf10AcuBold;
extern HFONT g_hf11AcuBold;
extern HFONT g_hf13AcuBold;
extern HFONT g_hf14AcuMed;
extern HFONT g_hf14AcuBold;
extern HFONT g_hf15AcuMed;
//extern HFONT g_hf24Normal;
//extern HFONT g_hf24Medium;
extern HFONT g_hf17AcuBold;
extern HFONT g_hf19AcuMed;
extern HFONT g_hf21AcuBold;
extern HFONT g_hf23AcuBold;
extern HFONT g_hf25AcuMed;
extern HFONT g_hf27AcuBold;
extern HFONT g_hf31AcuBold;
extern HFONT g_hf33AcuBold;
//extern HFONT g_hf42Bold;
extern HFONT g_hf43AcuBold;
extern HFONT g_hf53AcuBold;
extern HFONT g_hf70Bold;

/**********************************************************************************************//**
 * CWndParaSettings
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CWndParaSettings, CWnd)

/**********************************************************************************************//**
 * Initializes a new instance of the CWndParaSettings class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

CWndParaSettings::CWndParaSettings()
{
	m_pModel=NULL;

	m_enumMessage=PARAWARN_NONE;

	m_hDC=NULL;
	m_hBmp=NULL;
	m_hBmpPrev=NULL;
	m_lX=0;
	m_lY=0;
	m_lXo=0;
	m_lYo=0;

	m_pcExclamation=NULL;
	m_pcPara_Confirm=NULL;
	m_pcPara_Confirm_yes=NULL;
	m_pcPara_Confirm_no=NULL;
	m_bBiasFlowConfirmed=false;

	/*m_pcPara_OnOff=NULL;
	m_pcPara_OnOff_Up=NULL;
	m_pcPara_OnOff_Dw=NULL;*/

	m_pcParaMenu_Up=NULL;
	m_pcParaMenu_Dw=NULL;
	m_pcParaMenu=NULL;

	m_pcPara_UpT=NULL;
	m_pcPara_FcT=NULL;
	m_pcPara_DwT=NULL;
	m_pcPara_UpB=NULL;
	m_pcPara_FcB=NULL;
	m_pcPara_DwB=NULL;
	m_pcPara_DisB=NULL;

	m_pcPara_PCurve=NULL;
	m_pcPara_AbortPSV=NULL;
	m_pcPara_ManBreath=NULL;
	m_pcPara_FlushTime=NULL;
	m_pcPara_FREQ_REC=NULL;
	m_pcPara_EFLOW=NULL;
	m_pcPara_LeakCompensation=NULL;

	m_pcSelSetting_Up=NULL;
	m_pcSelSetting_Dw=NULL;
	m_pcSelSetting_Fc=NULL;
	//m_pcSelSetting_Dis=NULL;

	/*m_pcSIMVPSV_ON=NULL;
	m_pcSIMVPSV_OFF=NULL;*/
	m_pcFreshgasInt=NULL;
	m_pcFreshgasExt=NULL;
	m_pcVolumeGuarantee=NULL;
	m_pcVolumeLimit=NULL;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CWndParaSettings class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

CWndParaSettings::~CWndParaSettings()
{
	if(m_pcExclamation)
		delete m_pcExclamation;
	m_pcExclamation=NULL;

	if(m_pcPara_Confirm)
		delete m_pcPara_Confirm;
	m_pcPara_Confirm=NULL;
	if(m_pcPara_Confirm_yes)
		delete m_pcPara_Confirm_yes;
	m_pcPara_Confirm_yes=NULL;
	if(m_pcPara_Confirm_no)
		delete m_pcPara_Confirm_no;
	m_pcPara_Confirm_no=NULL;

	delete m_pcVolumeGuarantee;
	m_pcVolumeGuarantee=NULL;
	delete m_pcVolumeLimit;
	m_pcVolumeLimit=NULL;

	/*if(m_pcPara_OnOff)
		delete m_pcPara_OnOff;
	m_pcPara_OnOff=NULL;

	if(m_pcPara_OnOff_Up)
		delete m_pcPara_OnOff_Up;
	m_pcPara_OnOff_Up=NULL;
	if(m_pcPara_OnOff_Dw)
		delete m_pcPara_OnOff_Dw;
	m_pcPara_OnOff_Dw=NULL;*/

	delete m_pcFreshgasInt;
	m_pcFreshgasInt=NULL;
	delete m_pcFreshgasExt;
	m_pcFreshgasExt=NULL;

	delete m_pcSelSetting_Up;
	m_pcSelSetting_Up=NULL;
	delete m_pcSelSetting_Dw;
	m_pcSelSetting_Dw=NULL;
	delete m_pcSelSetting_Fc;
	m_pcSelSetting_Fc=NULL;
	//delete m_pcSelSetting_Dis;
	//m_pcSelSetting_Dis=NULL;
	
	delete m_pcParaMenu_Up;
	m_pcParaMenu_Up=NULL;

	delete m_pcParaMenu_Dw;
	m_pcParaMenu_Dw=NULL;

	delete m_pcParaMenu;
	m_pcParaMenu=NULL;

	delete m_pcPara_PCurve;
	m_pcPara_PCurve=NULL;

	delete m_pcPara_EFLOW;
	m_pcPara_EFLOW=NULL;

	delete m_pcPara_AbortPSV;
	m_pcPara_AbortPSV=NULL;

	delete m_pcPara_FREQ_REC;
	m_pcPara_FREQ_REC=NULL;

	delete m_pcPara_ManBreath;
	m_pcPara_ManBreath=NULL;

	delete m_pcPara_FlushTime;
	m_pcPara_FlushTime=NULL;

	delete m_pcPara_LeakCompensation;
	m_pcPara_LeakCompensation=NULL;

	delete m_pcPara_UpT;
	m_pcPara_UpT=NULL;
	delete m_pcPara_FcT;
	m_pcPara_FcT=NULL;
	delete m_pcPara_DwT;
	m_pcPara_DwT=NULL;
	delete m_pcPara_UpB;
	m_pcPara_UpB=NULL;
	delete m_pcPara_FcB;
	m_pcPara_FcB=NULL;
	delete m_pcPara_DwB;
	m_pcPara_DwB=NULL;
	delete m_pcPara_DisB;
	m_pcPara_DisB=NULL;
}

/**********************************************************************************************//**
 * Gets the model
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	Null if it fails, else the model.
 **************************************************************************************************/

CMVModel *CWndParaSettings::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}

BEGIN_MESSAGE_MAP(CWndParaSettings, CWnd)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	/*ON_BN_CLICKED(IDC_BTN_SIMVPSV_ON, &CWndParaSettings::OnBnClickedShowSIMVPSV)
	ON_BN_CLICKED(IDC_BTN_SIMVPSV_OFF, &CWndParaSettings::OnBnClickedHideSIMVPSV)
	ON_BN_CLICKED(IDC_BTN_SHOW_VOLUMEGUARANTEE, &CWndParaSettings::OnBnClickedVG)
	ON_BN_CLICKED(IDC_BTN_SHOW_VOLUMELIMIT, &CWndParaSettings::OnBnClickedVL)
	ON_BN_CLICKED(IDC_BTN_FRESHGAS_INT, &CWndParaSettings::OnBnClickedFreshgasInt)
	ON_BN_CLICKED(IDC_BTN_FRESHGAS_EXT, &CWndParaSettings::OnBnClickedFreshgasExt)*/
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * CWndParaSettings message handlers
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

BOOL CWndParaSettings::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext*) 
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

		CBrush cbrGrey(RGB(181,178,181));
		CBrush cbrBack(BACKGND);
		/*CPen pen;
		pen.CreatePen(PS_SOLID,2,BLACK);*/
		HBRUSH hbrprev=(HBRUSH)SelectObject(m_hDC,cbrBack);
		HPEN hpenprev=(HPEN)SelectObject(m_hDC, (HPEN)GetStockObject(NULL_PEN));	

		Rectangle(m_hDC, 0, 0, m_lX, m_lY);

		SelectObject(m_hDC,cbrGrey);
		Rectangle(m_hDC, 0, 1, m_lX, m_lY-1);

		/*CDC* pDC=CDC::FromHandle(m_hDC);
		pDC->SelectObject(&pen);
		pDC->MoveTo(0 , 2);
		pDC->LineTo(551,2);
		pDC->MoveTo(0 , 228);
		pDC->LineTo(551,228);*/
	
		/*m_pcPara_OnOff_Up		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PARA_ONOFF_UP);
		m_pcPara_OnOff_Dw		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PARA_ONOFF_DW);*/

		m_pcSelSetting_Up		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SELSETTING_UP);
		m_pcSelSetting_Dw		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SELSETTING_DW);
		m_pcSelSetting_Fc		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SELSETTING_FC);
		//m_pcSelSetting_Dis		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SELSETTING_DIS);

		m_pcParaMenu_Up		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PARAMENUBCK_UP);
		m_pcParaMenu_Dw		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PARAMENUBCK_DW);

		m_pcPara_UpT		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PARASET_UP_TOP);
		m_pcPara_FcT		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PARASET_FC_TOP);
		m_pcPara_DwT		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PARASET_DW_TOP);
		m_pcPara_UpB		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PARASET_UP_BOT);
		m_pcPara_FcB		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PARASET_FC_BOT);
		m_pcPara_DwB		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PARASET_DW_BOT);
		m_pcPara_DisB		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PARASET_DIS_BOT);

		m_pcExclamation = new CBmp(theApp.m_hInstance,dc.m_hDC,IDB_EXCLAMATION_LGHTGREY);
		m_pcPara_Confirm_yes= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_SELECT_YES);
		m_pcPara_Confirm_no= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_SELECT_NO);

		DWORD dwStyleNoTab = WS_CHILD/*|WS_VISIBLE*/|BS_OWNERDRAW|WS_TABSTOP;
		BTN btn;
		fVALUE fv;

		btn.wID					= IDC_BTN_PARAMENUBCK;	
		btn.poPosition.x		= 551;
		btn.poPosition.y		= 0;
		btn.pcBmpUp				= m_pcParaMenu_Up;
		btn.pcBmpDown			= m_pcParaMenu_Dw;
		btn.pcBmpFocus			= m_pcParaMenu_Up;
		btn.pcBmpDisabled		= m_pcParaMenu_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pcParaMenu=new CUpDwnBtn(btn,COLOR_TXTBTNUP,false,true);
		m_pcParaMenu->Create(this,g_hf7AcuBold,0);
		m_pcParaMenu->SetText(getModel()->GetLanguageString(IDS_TXT_SETTINGS),getModel()->GetLanguageString(IDS_TXT_SETTINGS));
		m_pcParaMenu->ShowWindow(SW_SHOW);

		m_pcParaMenu->Depress(true);



		//Parameter Button------Time Manbreath---------------------------------
		btn.wID					= IDC_BTN_PARA_MANBREATH;	
		btn.poPosition.x		= 20;
		btn.poPosition.y		= 25;
		btn.pcBmpUp				= m_pcPara_UpT;
		btn.pcBmpDown			= m_pcPara_DwT;
		btn.pcBmpFocus			= m_pcPara_FcT;
		btn.pcBmpDisabled		= m_pcPara_UpT;
		btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;


		fv.iLowerLimit=MIN_TIME_MANBREATH;
		fv.iUpperLimit=MAX_TIME_MANBREATH;

		fv.iValue=getModel()->getCONFIG()->GetManBreathTime();
		if(fv.iValue>fv.iUpperLimit)
			fv.iValue=fv.iUpperLimit;
		else if(fv.iValue<fv.iLowerLimit)
			fv.iValue=fv.iLowerLimit;
		//fv.tText[0]=0;
		m_pcPara_ManBreath=new CParaBtn_ManBreath(btn,0,false);
		m_pcPara_ManBreath->Create(this,dwStyleNoTab,fv);
		m_pcPara_ManBreath->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
		m_pcPara_ManBreath->SetNameText(_T(""));
		m_pcPara_ManBreath->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_SECONDS));
		//m_plParaBtn.AddTail(m_pcPara_IFLOW);


		//Parameter Button------O2-Flush Time---------------------------------
		btn.wID					= IDC_BTN_PARA_FLUSHTIME;	
		btn.poPosition.x		= 115;
		btn.poPosition.y		= 25;
		btn.pcBmpUp				= m_pcPara_UpT;
		btn.pcBmpDown			= m_pcPara_DwT;
		btn.pcBmpFocus			= m_pcPara_FcT;
		btn.pcBmpDisabled		= m_pcPara_UpT;
		btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;


		fv.iLowerLimit=0;
		fv.iUpperLimit=120;

		fv.iValue=getModel()->getDATAHANDLER()->GetCurO2FlushTime();
		if(fv.iValue>fv.iUpperLimit)
			fv.iValue=fv.iUpperLimit;
		else if(fv.iValue<fv.iLowerLimit)
			fv.iValue=fv.iLowerLimit;
		//fv.tText[0]=0;
		m_pcPara_FlushTime=new CParaBtn_FlushTime(btn,0,false);
		m_pcPara_FlushTime->Create(this,dwStyleNoTab,fv);
		m_pcPara_FlushTime->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
		m_pcPara_FlushTime->SetNameText(getModel()->GetLanguageString(IDS_TXT_FLUSHTIME));
		m_pcPara_FlushTime->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_SECONDS));
		//m_plParaBtn.AddTail(m_pcPara_IFLOW);

		

		//Parameter Button------Frequenz HFO Recruitment---------------------------------
		btn.wID					= IDC_BTN_PARA_BPM_REC;	
		btn.poPosition.x		= 210;
		btn.poPosition.y		= 25;
		btn.pcBmpUp				= m_pcPara_UpT;
		btn.pcBmpDown			= m_pcPara_DwT;
		btn.pcBmpFocus			= m_pcPara_FcT;
		btn.pcBmpDisabled		= m_pcPara_UpT;
		btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;


		fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetFreqRecMinPara();
		fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetFreqRecMaxPara();
		fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetFreqRecPara();

		if(fv.iValue>fv.iUpperLimit)
			fv.iValue=fv.iUpperLimit;
		else if(fv.iValue<fv.iLowerLimit)
			fv.iValue=fv.iLowerLimit;
		//fv.tText[0]=0;
		m_pcPara_FREQ_REC=new CParaBtn_FREQ_REC(btn,0,false);
		m_pcPara_FREQ_REC->Create(this,dwStyleNoTab,fv);
		m_pcPara_FREQ_REC->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
		//m_pcPara_FREQ_REC->SetNameText(getModel()->GetLanguageString(IDS_PARA_FREQREC));
		m_pcPara_FREQ_REC->SetNameText(getModel()->GetLanguageString(IDS_PARA_FREQ));
		m_pcPara_FREQ_REC->SetNameNoteText(getModel()->GetLanguageString(IDS_PARA_REC));
		if(fv.iValue>=60)
			m_pcPara_FREQ_REC->SetUnitText(_T("1/")+getModel()->GetLanguageString(IDS_UNIT_MIN));
		else
			m_pcPara_FREQ_REC->SetUnitText(_T("1/")+getModel()->GetLanguageString(IDS_UNIT_HRS));
		

		//Parameter Button------Leak Compensation---------------------------------
		btn.wID					= IDC_BTN_PARA_LEAKCOMP;	
		btn.poPosition.x		= 20;
		btn.poPosition.y		= 120;
		btn.pcBmpUp				= m_pcPara_UpB;
		btn.pcBmpDown			= m_pcPara_DwB;
		btn.pcBmpFocus			= m_pcPara_FcB;
		btn.pcBmpDisabled		= m_pcPara_UpB;
		btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;


		fv.iLowerLimit=LC_LOW;
		fv.iUpperLimit=LC_HIGH;

		fv.iValue=getModel()->getCONFIG()->getLeakCompensation();
		if(fv.iValue>fv.iUpperLimit)
			fv.iValue=fv.iUpperLimit;
		else if(fv.iValue<fv.iLowerLimit)
			fv.iValue=fv.iLowerLimit;
		m_pcPara_LeakCompensation=new CParaBtn_LeakCompensation(btn,0,false);
		m_pcPara_LeakCompensation->Create(this,dwStyleNoTab,fv);
		m_pcPara_LeakCompensation->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
		m_pcPara_LeakCompensation->SetNameText(_T(""));
		m_pcPara_LeakCompensation->SetUnitText(_T(""));

		//Parameter Button------Abortcrit. PSV---------------------------------
		btn.wID					= IDC_BTN_PARA_ABORTPSV;	
		btn.poPosition.x		= 20;
		btn.poPosition.y		= 120;
		btn.pcBmpUp				= m_pcPara_UpB;
		btn.pcBmpDown			= m_pcPara_DwB;
		btn.pcBmpFocus			= m_pcPara_FcB;
		btn.pcBmpDisabled		= m_pcPara_UpB;
		btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;


		fv.iLowerLimit=MIN_ABORTCRITERION_PSV;
		fv.iUpperLimit=MAX_ABORTCRITERION_PSV;

		fv.iValue=getModel()->getCONFIG()->GetPercentAbortCriterionPSV();
		if(fv.iValue>fv.iUpperLimit)
			fv.iValue=fv.iUpperLimit;
		else if(fv.iValue<fv.iLowerLimit)
			fv.iValue=fv.iLowerLimit;
		//fv.tText[0]=0;
		m_pcPara_AbortPSV=new CParaBtn_AbortPSV(btn,0,false);
		m_pcPara_AbortPSV->Create(this,dwStyleNoTab,fv);
		m_pcPara_AbortPSV->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
		m_pcPara_AbortPSV->SetNameText(_T(""));
		m_pcPara_AbortPSV->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_PERCENT));
		//m_plParaBtn.AddTail(m_pcPara_IFLOW);

		//-------------------------------------------------------------
		//Parameter Button------E-Flow---------------------------------
		btn.wID					= IDC_BTN_PARA_EFLOW;	
		btn.poPosition.x		= 115;
		btn.poPosition.y		= 120;
		btn.pcBmpUp				= m_pcPara_UpB;
		btn.pcBmpDown			= m_pcPara_DwB;
		btn.pcBmpFocus			= m_pcPara_FcB;
		btn.pcBmpDisabled		= m_pcPara_DisB;
		btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

		if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetEFLOWMinPara_IPPV();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetEFLOWMaxPara_IPPV();
		}
		else
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetEFLOWMinPara_TRIGGER();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetEFLOWMaxPara_TRIGGER();
		}
		fv.iValue=getModel()->getDATAHANDLER()->GetCurrentEFlowPara();
		if(fv.iValue>fv.iUpperLimit)
			fv.iValue=fv.iUpperLimit;
		else if(fv.iValue<fv.iLowerLimit)
			fv.iValue=fv.iLowerLimit;
		
		m_pcPara_EFLOW=new CParaBtn_EFLOW(btn,0,false);
		m_pcPara_EFLOW->Create(this,dwStyleNoTab,fv);
		m_pcPara_EFLOW->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
		m_pcPara_EFLOW->SetNameText(getModel()->GetLanguageString(IDS_PARA_EFLOW));
		m_pcPara_EFLOW->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_LMIN));
		if(fv.iValue>getModel()->getDATAHANDLER()->GetCurrentEFlowMaxKey())
		{
			m_pcPara_EFLOW->SetWarning();
		}
		else
		{
			m_pcPara_EFLOW->SetWarning(false);
		}

		//Parameter Button------Pressure Curve---------------------------------
		btn.wID					= IDC_BTN_PARA_PCURVE;	
		btn.poPosition.x		= 210;//115;
		btn.poPosition.y		= 120;
		btn.pcBmpUp				= m_pcPara_UpB;
		btn.pcBmpDown			= m_pcPara_DwB;
		btn.pcBmpFocus			= m_pcPara_FcB;
		btn.pcBmpDisabled		= m_pcPara_UpB;
		btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;


		fv.iLowerLimit=CURVE_IFLOW;
		fv.iUpperLimit=CURVE_LINEAR;
		//fv.iUpperLimit=CURVE_AUTOFLOW; //TODO_AUTOIFLOW
		
		fv.iValue=(int)getModel()->getCONFIG()->GetCurPressureRiseCtrl();
		if(fv.iValue>fv.iUpperLimit)
			fv.iValue=fv.iUpperLimit;
		else if(fv.iValue<fv.iLowerLimit)
			fv.iValue=fv.iLowerLimit;
		//fv.tText[0]=0;
		m_pcPara_PCurve=new CParaBtn_PCurve(btn,0,false);
		m_pcPara_PCurve->Create(this,dwStyleNoTab,fv);
		m_pcPara_PCurve->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
		m_pcPara_PCurve->SetNameText(getModel()->GetLanguageString(IDS_BTN_PRESSURERISE));
		m_pcPara_PCurve->SetUnitText(_T(""));



		//+++++++++++++++++PSV delta++++++++++++++++++++++++++++++
		//btn.wID					= IDC_BTN_SIMVPSV_ON;	
		//btn.poPosition.x		= 321;
		//btn.poPosition.y		= 8;
		//btn.pcBmpUp				= m_pcSelSetting_Up;
		//btn.pcBmpDown			= m_pcSelSetting_Dw;
		//btn.pcBmpFocus			= m_pcSelSetting_Up;
		//btn.pcBmpDisabled		= m_pcSelSetting_Up;
		//btn.dwFormat			= DT_VCENTER|DT_CENTER;

		//CString szTop=_T("show");
		//CString szBot=_T("SIMV+PSV");

		//m_pcSIMVPSV_ON=new CDwnBtn(btn,COLOR_TXTBTNUP,true,false);
		//m_pcSIMVPSV_ON->Create(this,g_hf9AcuBold,0);
		//m_pcSIMVPSV_ON->SetText(szTop,szBot,szTop,szBot);
		//m_pcSIMVPSV_ON->ShowWindow(SW_HIDE);
		////m_pcSIMVPSV_ON->Depress(true);



		//btn.wID					= IDC_BTN_SIMVPSV_OFF;	
		//btn.poPosition.x		= 321;
		//btn.poPosition.y		= 58;
		//btn.pcBmpUp				= m_pcSelSetting_Up;
		//btn.pcBmpDown			= m_pcSelSetting_Dw;
		//btn.pcBmpFocus			= m_pcSelSetting_Up;
		//btn.pcBmpDisabled		= m_pcSelSetting_Up;
		//btn.dwFormat			= DT_VCENTER|DT_CENTER;

		//szTop=_T("hide");
		//szBot=_T("SIMV+PSV");
		//m_pcSIMVPSV_OFF=new CDwnBtn(btn,COLOR_TXTBTNUP,true,false);
		//m_pcSIMVPSV_OFF->Create(this,g_hf9AcuBold,0);
		//m_pcSIMVPSV_OFF->SetText(szTop,szBot,szTop,szBot);
		//m_pcSIMVPSV_OFF->ShowWindow(SW_HIDE);
		////m_pcFreshgasInt->Depress(false);

		//-------------------------------------------------------------
		//Parameter Button------PRICO---------------------------------
		/*btn.wID					= IDC_BTN_PRICO_ONOFF;	
		btn.poPosition.x		= 327;
		btn.poPosition.y		= 45;
		btn.pcBmpUp				= m_pcPara_OnOff_Up;
		btn.pcBmpDown			= m_pcPara_OnOff_Dw;
		btn.pcBmpFocus			= m_pcPara_OnOff_Up;
		btn.pcBmpDisabled		= m_pcPara_OnOff_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pcPara_OnOff=new CUpDwnBtn(btn,COLOR_TXTBTNUP,false,true);
		m_pcPara_OnOff->Create(this,g_hf7AcuBold,0);
		m_pcPara_OnOff->SetText(_T(""),_T(""));
		if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
			m_pcPara_OnOff->ShowWindow(SW_SHOW);
		else
			m_pcPara_OnOff->ShowWindow(SW_HIDE);

		if(getModel()->getDATAHANDLER()->getPRICOState()==true)
			m_pcPara_OnOff->Depress(true);
		else
			m_pcPara_OnOff->Depress(false);*/

		//+++++++++++++++++fresh gas selection HFO++++++++++++++++++++++++++++++
		btn.wID					= IDC_BTN_FRESHGAS_INT;	
		btn.poPosition.x		= 436;
		btn.poPosition.y		= 8;
		btn.pcBmpUp				= m_pcSelSetting_Up;
		btn.pcBmpDown			= m_pcSelSetting_Dw;
		btn.pcBmpFocus			= m_pcSelSetting_Fc;
		btn.pcBmpDisabled		= m_pcSelSetting_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		/*CString szTop=_T("Bias Flow");
		CString szBot=_T("intern");*/
		

		//m_pcFreshgasInt=new CDwnBtn(btn,COLOR_TXTBTNUP,true,false);
		m_pcFreshgasInt=new CPresetMenuBtn(btn,COLOR_TXTBTNUP);
		m_pcFreshgasInt->Create(this,g_hf9AcuBold,0);
		m_pcFreshgasInt->SetTextTwoLines(getModel()->GetLanguageString(IDS_BTN_BIAS_INT1),getModel()->GetLanguageString(IDS_BTN_BIAS_INT2));

		if(getModel()->getCONFIG()->isBiasFlowActive()==true && true==getModel()->getDATAHANDLER()->isHFOLicenseAvailable())
		{
			m_pcFreshgasInt->ShowWindow(SW_SHOW);
		}
		else
		{
			m_pcFreshgasInt->ShowWindow(SW_HIDE);
		}



		btn.wID					= IDC_BTN_FRESHGAS_EXT;	
		btn.poPosition.x		= 436;
		btn.poPosition.y		= 58;
		btn.pcBmpUp				= m_pcSelSetting_Up;
		btn.pcBmpDown			= m_pcSelSetting_Dw;
		btn.pcBmpFocus			= m_pcSelSetting_Fc;
		btn.pcBmpDisabled		= m_pcSelSetting_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		//szTop=_T("Bias Flow");
		//szBot=_T("extern");
		//m_pcFreshgasExt=new CDwnBtn(btn,COLOR_TXTBTNUP,true,false);
		m_pcFreshgasExt=new CPresetMenuBtn(btn,COLOR_TXTBTNUP);
		m_pcFreshgasExt->Create(this,g_hf9AcuBold,0);
		m_pcFreshgasExt->SetTextTwoLines(getModel()->GetLanguageString(IDS_BTN_BIAS_EXT1),getModel()->GetLanguageString(IDS_BTN_BIAS_EXT2));
		
		if(getModel()->getCONFIG()->isBiasFlowActive()==true && true==getModel()->getDATAHANDLER()->isHFOLicenseAvailable())
		{
			m_pcFreshgasExt->ShowWindow(SW_SHOW);
		}
		else
		{
			m_pcFreshgasExt->ShowWindow(SW_HIDE);
		}

		
		//+++++++++++++++++volume guarantee / volume limit++++++++++++++++++++++++++++++
		btn.wID					= IDC_BTN_SHOW_VOLUMEGUARANTEE;	
		btn.poPosition.x		= 436;
		btn.poPosition.y		= 122;
		btn.pcBmpUp				= m_pcSelSetting_Up;
		btn.pcBmpDown			= m_pcSelSetting_Dw;
		btn.pcBmpFocus			= m_pcSelSetting_Fc;
		btn.pcBmpDisabled		= m_pcSelSetting_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		/*szTop=_T("Volume");
		szBot=_T("guarantee");*/

		//m_pcVolumeGuarantee=new CDwnBtn(btn,COLOR_TXTBTNUP,true,false);
		m_pcVolumeGuarantee=new CPresetMenuBtn(btn,COLOR_TXTBTNUP);
		m_pcVolumeGuarantee->Create(this,g_hf9AcuBold,0);
		m_pcVolumeGuarantee->SetTextTwoLines(getModel()->GetLanguageString(IDS_BTN_VG1),getModel()->GetLanguageString(IDS_BTN_VG2));
		m_pcVolumeGuarantee->ShowWindow(SW_HIDE);
		//m_pcVolumeGuarantee->Depress(true);



		btn.wID					= IDC_BTN_SHOW_VOLUMELIMIT;	
		btn.poPosition.x		= 436;
		btn.poPosition.y		= 172;
		btn.pcBmpUp				= m_pcSelSetting_Up;
		btn.pcBmpDown			= m_pcSelSetting_Dw;
		btn.pcBmpFocus			= m_pcSelSetting_Fc;
		btn.pcBmpDisabled		= m_pcSelSetting_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		//szTop=_T("Volume");
		//szBot=_T("limit");
		//m_pcVolumeLimit=new CDwnBtn(btn,COLOR_TXTBTNUP,true,false);
		m_pcVolumeLimit=new CPresetMenuBtn(btn,COLOR_TXTBTNUP);
		m_pcVolumeLimit->Create(this,g_hf9AcuBold,0);
		m_pcVolumeLimit->SetTextTwoLines(getModel()->GetLanguageString(IDS_BTN_VL1),getModel()->GetLanguageString(IDS_BTN_VL2));
		m_pcVolumeLimit->ShowWindow(SW_HIDE);
		//m_pcVolumeLimit->Depress(false);



		btn.wID					= IDC_BTN_CONFIRM_BIASFLOW;	
		btn.poPosition.x		= 320;//20;
		btn.poPosition.y		= 140;
		btn.pcBmpUp				= m_pcPara_Confirm_no;
		btn.pcBmpDown			= m_pcPara_Confirm_yes;
		btn.pcBmpFocus			= m_pcPara_Confirm_no;
		btn.pcBmpDisabled		= m_pcPara_Confirm_no;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pcPara_Confirm=new CUpDwnBtn(btn,COLOR_TXTBTNUP,false,false);
		m_pcPara_Confirm->Create(this,g_hf21AcuBold,0);
		m_pcPara_Confirm->SetText(_T(""),_T(""));
		m_pcPara_Confirm->ShowWindow(SW_HIDE);
		
		if(getModel()->getCONFIG()->isBiasFlowActive()==true)
		{
			if(getModel()->getCONFIG()->getFreshgasExt()==true)
			{
				m_pcFreshgasInt->SetBtnState(CPresetMenuBtn::UP);
				m_pcFreshgasExt->SetBtnState(CPresetMenuBtn::DOWN);
			}
			else
			{
				m_pcFreshgasInt->SetBtnState(CPresetMenuBtn::DOWN);
				m_pcFreshgasExt->SetBtnState(CPresetMenuBtn::UP);
			}
		}

		if(getModel()->getDATAHANDLER()->isVLIMITLicenseAvailable() 
				&& getModel()->getDATAHANDLER()->isVGUARANTLicenseAvailable())
		{
			m_pcVolumeGuarantee->ShowWindow(SW_SHOW);
			//m_pcVolumeGuarantee->EnableWindow(TRUE);

			m_pcVolumeLimit->ShowWindow(SW_SHOW);
			//m_pcVolumeLimit->EnableWindow(TRUE);

			if(getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VGARANT)
			{
				m_pcVolumeGuarantee->SetBtnState(CPresetMenuBtn::DOWN);
				m_pcVolumeLimit->SetBtnState(CPresetMenuBtn::UP);
			}
			else if(getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VLIMIT)
			{
				m_pcVolumeGuarantee->SetBtnState(CPresetMenuBtn::UP);
				m_pcVolumeLimit->SetBtnState(CPresetMenuBtn::DOWN);
			}
		}
		else
		{
			m_pcVolumeGuarantee->ShowWindow(SW_HIDE);
			m_pcVolumeLimit->ShowWindow(SW_HIDE);
			//m_pcVolumeGuarantee->ShowWindow(SW_SHOW);
			//m_pcVolumeGuarantee->EnableWindow(FALSE);

			//m_pcVolumeLimit->ShowWindow(SW_SHOW);
			//m_pcVolumeLimit->EnableWindow(FALSE);
		}

		if(getModel()->getCONFIG()->GetCurMode()==VM_PRE_HFO 
			||	getModel()->getCONFIG()->GetCurMode()==VM_HFO)
		{
			m_pcPara_PCurve->ShowWindow(SW_SHOW);
			m_pcPara_PCurve->EnableWindow(FALSE);
			//m_pcPara_PCurve->ShowWindow(SW_HIDE);
		}
		else
		{
			//TODO_AUTOIFLOW
			if(false==getModel()->getCONFIG()->isRISETIMEREGISTRYenabled())
			{
				m_pcPara_PCurve->ShowWindow(SW_SHOW);
				m_pcPara_PCurve->EnableWindow(FALSE);
			}
			else
			{
				m_pcPara_PCurve->ShowWindow(SW_SHOW);
				m_pcPara_PCurve->EnableWindow(TRUE);
			}
		}

		if(getModel()->getVMODEHANDLER()->activeModeHasPSV())
		{
			m_pcPara_AbortPSV->ShowWindow(SW_SHOW);
			//m_pcPara_AbortPSV->EnableWindow(TRUE);
		}
		else
		{
			//m_pcPara_AbortPSV->ShowWindow(SW_SHOW);
			//m_pcPara_AbortPSV->EnableWindow(FALSE);
			m_pcPara_AbortPSV->ShowWindow(SW_HIDE);
		}

		if(getModel()->getCONFIG()->GetCurMode()==VM_PRE_HFO 
			||	getModel()->getCONFIG()->GetCurMode()==VM_HFO)
		{
			m_pcPara_EFLOW->ShowWindow(SW_HIDE);
			//m_pcPara_EFLOW->ShowWindow(SW_SHOW);
			//m_pcPara_EFLOW->EnableWindow(FALSE);
		}
		else if(	getModel()->getVMODEHANDLER()->curModeIsConventionalMode()//EFLOW1
				&&	getModel()->getCONFIG()->getShowEFlowInSettings()==true
				/*&&	getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_IFLOW*/)
		{
			m_pcPara_EFLOW->ShowWindow(SW_SHOW);
			if(getModel()->getCONFIG()->IsEFLOWequalILFOW()&&getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_IFLOW)
			{
				m_pcPara_EFLOW->EnableWindow(FALSE);
			}
			else
			{
				m_pcPara_EFLOW->EnableWindow(TRUE);
			}
		}
		else
		{
			//m_pcPara_EFLOW->ShowWindow(SW_SHOW);
			//m_pcPara_EFLOW->EnableWindow(FALSE);
			m_pcPara_EFLOW->ShowWindow(SW_HIDE);
		}

		if(getModel()->getCONFIG()->GetCurMode()==VM_PRE_NCPAP 
			||	getModel()->getCONFIG()->GetCurMode()==VM_NCPAP
			||  getModel()->getCONFIG()->GetCurMode()==VM_PRE_DUOPAP 
			||	getModel()->getCONFIG()->GetCurMode()==VM_DUOPAP)
		{
			m_pcPara_LeakCompensation->ShowWindow(SW_SHOW);
		}
		else
		{
			m_pcPara_LeakCompensation->ShowWindow(SW_HIDE);
		}

		SelectObject(m_hDC,hpenprev);
		SelectObject(m_hDC,hbrprev);
		

		return 1;
	}
	else
		return 0;
}

/**********************************************************************************************//**
 * Shows
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	bShow	True to show, false to hide.
 **************************************************************************************************/

void CWndParaSettings::Show(bool bShow)
{
	m_bBiasFlowConfirmed=false;

	if(m_pcPara_FREQ_REC)
	{
		m_pcPara_FREQ_REC->ClearArrows();
	}

	if(bShow)
	{
		Draw();

		if(m_enumMessage==PARAWARN_NONE)
		{
			if(getModel()->getCONFIG()->GetCurMode()==VM_PRE_HFO 
				||	getModel()->getCONFIG()->GetCurMode()==VM_HFO)
			{
				//m_pcPara_PCurve->ShowWindow(SW_SHOW);
				//m_pcPara_PCurve->EnableWindow(FALSE);
				m_pcPara_PCurve->ShowWindow(SW_HIDE);
			}
			//else if(NEONATAL==getModel()->getCONFIG()->GetVentRange())//RISE_NEO
			//{
			//	m_pcPara_PCurve->ShowWindow(SW_HIDE);
			//}
			else
			{
				//TODO_AUTOIFLOW
				if(false==getModel()->getCONFIG()->isRISETIMEREGISTRYenabled())
				{
					m_pcPara_PCurve->ShowWindow(SW_HIDE);
					//m_pcPara_PCurve->ShowWindow(SW_SHOW);
					//m_pcPara_PCurve->EnableWindow(FALSE);
				}
				else
				{
					m_pcPara_PCurve->ShowWindow(SW_SHOW);
					//m_pcPara_PCurve->EnableWindow(TRUE);
				}
			}

			if(getModel()->getVMODEHANDLER()->activeModeHasPSV())
			{
				m_pcPara_AbortPSV->ShowWindow(SW_SHOW);
				//m_pcPara_AbortPSV->EnableWindow(TRUE);
			}
			else
			{
				m_pcPara_AbortPSV->ShowWindow(SW_HIDE);
				//m_pcPara_AbortPSV->ShowWindow(SW_SHOW);
				//m_pcPara_AbortPSV->EnableWindow(FALSE);
			}

			if(getModel()->getCONFIG()->GetCurMode()==VM_PRE_HFO 
				||	getModel()->getCONFIG()->GetCurMode()==VM_HFO)
			{
				m_pcPara_EFLOW->ShowWindow(SW_HIDE);
				//m_pcPara_EFLOW->ShowWindow(SW_SHOW);
				//m_pcPara_EFLOW->EnableWindow(FALSE);
			}
			else if(	getModel()->getVMODEHANDLER()->curModeIsConventionalMode()//EFLOW1
					&&	getModel()->getCONFIG()->getShowEFlowInSettings()==true
					/*&&	getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_IFLOW*/)
			{
				m_pcPara_EFLOW->ShowWindow(SW_SHOW);
				if(getModel()->getCONFIG()->IsEFLOWequalILFOW()&&getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_IFLOW)
				{
					fVALUE fv;
					fv.iValue=getModel()->getDATAHANDLER()->GetCurrentEFlowPara();
					if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
					{
						fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetEFLOWMinPara_IPPV();
						fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetEFLOWMaxPara_IPPV();
					}
					else
					{
						fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetEFLOWMinPara_TRIGGER();
						fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetEFLOWMaxPara_TRIGGER();
					}
					m_pcPara_EFLOW->SetValue(fv);
					m_pcPara_EFLOW->EnableWindow(FALSE);
				}
				else
				{
					m_pcPara_EFLOW->EnableWindow(TRUE);
				}
			}
			else
			{
				m_pcPara_EFLOW->ShowWindow(SW_HIDE);
				//m_pcPara_EFLOW->ShowWindow(SW_SHOW);
				//m_pcPara_EFLOW->EnableWindow(FALSE);
			}

			m_pcPara_ManBreath->ShowWindow(SW_SHOW);
			m_pcPara_FlushTime->ShowWindow(SW_SHOW);

			if((getModel()->getCONFIG()->GetCurMode()==VM_PRE_HFO 
				||	getModel()->getCONFIG()->GetCurMode()==VM_HFO)
				&& getModel()->getDATAHANDLER()->isLUNGRECLicenseAvailable()
				&& getModel()->getDATAHANDLER()->isHFOLicenseAvailable())
			{
				m_pcPara_FREQ_REC->ShowWindow(SW_SHOW);
				//m_pcPara_FREQ_REC->EnableWindow(TRUE);
			}
			else
			{
				m_pcPara_FREQ_REC->ShowWindow(SW_HIDE);
				//m_pcPara_FREQ_REC->ShowWindow(SW_SHOW);
				//m_pcPara_FREQ_REC->EnableWindow(FALSE);
			}
			m_pcPara_Confirm->ShowWindow(SW_HIDE);

			if(getModel()->getCONFIG()->GetCurMode()==VM_PRE_NCPAP 
				||	getModel()->getCONFIG()->GetCurMode()==VM_NCPAP
				||  getModel()->getCONFIG()->GetCurMode()==VM_PRE_DUOPAP 
				||	getModel()->getCONFIG()->GetCurMode()==VM_DUOPAP)
			{
				m_pcPara_LeakCompensation->ShowWindow(SW_SHOW);
			}
			else
			{
				m_pcPara_LeakCompensation->ShowWindow(SW_HIDE);
			}

		}
		else
		{
			m_pcPara_PCurve->ShowWindow(SW_HIDE);
			m_pcPara_AbortPSV->ShowWindow(SW_HIDE);
			m_pcPara_ManBreath->ShowWindow(SW_HIDE);
			m_pcPara_FlushTime->ShowWindow(SW_HIDE);
			m_pcPara_FREQ_REC->ShowWindow(SW_HIDE);
			m_pcPara_EFLOW->ShowWindow(SW_HIDE);
			m_pcPara_LeakCompensation->ShowWindow(SW_HIDE);

			m_pcPara_Confirm->ShowWindow(SW_SHOW);
		}

		if(getModel()->getDATAHANDLER()->isVLIMITLicenseAvailable() 
			&& getModel()->getDATAHANDLER()->isVGUARANTLicenseAvailable())
		{
			m_pcVolumeGuarantee->ShowWindow(SW_SHOW);
			//m_pcVolumeGuarantee->EnableWindow(TRUE);

			m_pcVolumeLimit->ShowWindow(SW_SHOW);
			//m_pcVolumeLimit->EnableWindow(TRUE);
		}
		else
		{
			/*m_pcVolumeGuarantee->ShowWindow(SW_SHOW);
			m_pcVolumeGuarantee->EnableWindow(FALSE);

			m_pcVolumeLimit->ShowWindow(SW_SHOW);
			m_pcVolumeLimit->EnableWindow(FALSE);*/
			m_pcVolumeGuarantee->ShowWindow(SW_HIDE);
			m_pcVolumeLimit->ShowWindow(SW_HIDE);
		}


		if(getModel()->getCONFIG()->isBiasFlowActive()==true && true==getModel()->getDATAHANDLER()->isHFOLicenseAvailable())
		{
			m_pcFreshgasInt->ShowWindow(SW_SHOW);
			m_pcFreshgasExt->ShowWindow(SW_SHOW);
		}
		else
		{
			m_pcFreshgasInt->ShowWindow(SW_HIDE);
			m_pcFreshgasExt->ShowWindow(SW_HIDE);
		}
		
		this->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pcVolumeGuarantee->ShowWindow(SW_HIDE);
		m_pcVolumeLimit->ShowWindow(SW_HIDE);
		m_pcFreshgasInt->ShowWindow(SW_HIDE);
		m_pcFreshgasExt->ShowWindow(SW_HIDE);

		m_pcPara_PCurve->ShowWindow(SW_HIDE);
		m_pcPara_AbortPSV->ShowWindow(SW_HIDE);
		m_pcPara_ManBreath->ShowWindow(SW_HIDE);
		m_pcPara_FlushTime->ShowWindow(SW_HIDE);
		m_pcPara_FREQ_REC->ShowWindow(SW_HIDE);
		m_pcPara_EFLOW->ShowWindow(SW_HIDE);
		m_pcPara_LeakCompensation->ShowWindow(SW_HIDE);
		this->ShowWindow(SW_HIDE);
	}
}

/**********************************************************************************************//**
 * Paints this window
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndParaSettings::OnPaint()
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
 * \date	26.02.2018
 **************************************************************************************************/

void CWndParaSettings::OnDestroy()
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

LRESULT CWndParaSettings::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
{
	switch(message)
	{
	case WM_MENUBUTTONDOWN:
		{
			switch(wParam)
			{
			/*case IDC_BTN_SIMVPSV_ON:
				{
					OnBnClickedShowSIMVPSV();
					return 1;
				}
				break;
			case IDC_BTN_SIMVPSV_OFF:
				{
					OnBnClickedHideSIMVPSV();
					return 1;
				}
				break;*/
			case IDC_BTN_CONFIRM_BIASFLOW:
				{
					m_bBiasFlowConfirmed=!m_bBiasFlowConfirmed;
					/*if(m_bBiasFlowConfirmed)
					{
						m_pcPara_Confirm->Depress(true);
					}
					else
					{
						m_pcPara_Confirm->Depress(false);
					}*/
					return 1;
				}
				break;
			/*case IDC_BTN_PRICO_ONOFF:
				{
					if(getModel()->getDATAHANDLER()->getPRICOState()==true)
					{
						m_pcPara_OnOff->Depress(false);
						getModel()->getDATAHANDLER()->setPRICOState(false);
					}
					else
					{
						m_pcPara_OnOff->Depress(true);
						getModel()->getDATAHANDLER()->setPRICOState(true);
					}
				}
				break;*/
			case IDC_BTN_FRESHGAS_INT:
			case IDC_BTN_FRESHGAS_EXT:
				{
					if(lParam==CPresetMenuBtn::UP)
					{
						showFreshgasMessage(wParam);
						//SetOneButtonPreset_Freshgas(wParam);
					}
					else if(lParam==CPresetMenuBtn::PRESET)
					{ 
						if(m_bBiasFlowConfirmed)
						{
							SetOneButtonDepressed_Freshgas(wParam);
						}
						else
						{
							//CMVEventInfotext event(CMVEventInfotext::EV_TIMETEXT, _T("- please confirm the warning first -"), 3500);
							CMVEventInfotext event(CMVEventInfotext::EV_TIMETEXT, getModel()->GetLanguageString(IDS_TXT_CONFIRM_WARNING), 3500);
							getModel()->triggerEvent(&event);

							getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
							Sleep(300);
							getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
						}
					}
					else //if(lParam==CPresetMenuBtn::DOWN)
					{
						SetOneButtonDepressed_Freshgas(wParam);
					}
					return 1;
				}
				break;
			case IDC_BTN_SHOW_VOLUMEGUARANTEE:
			case IDC_BTN_SHOW_VOLUMELIMIT:
				{
					if(lParam==CPresetMenuBtn::UP)
						SetOneButtonPreset_Volume(wParam);
					else if(lParam==CPresetMenuBtn::PRESET)
						SetOneButtonDepressed_Volume(wParam);
					else //if(lParam==CPresetMenuBtn::DOWN)
						SetOneButtonDepressed_Volume(wParam);
					/*OnBnClickedVL();
					OnBnClickedVG();*/
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
			case IDC_BTN_PARAMENUBCK:
				{
					if(GetParent())
						GetParent()->PostMessage(WM_MENUBUTTONUP,IDC_BTN_PARAMENUBCK);
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
	/*case WM_PCURVE_CHANGED:
		{
			Show(true);
		}
		break;*/
	}
	return CWnd::WindowProc(message, wParam, lParam);
}

/**********************************************************************************************//**
 * Sets view focus
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	iBtn	Zero-based index of the button.
 **************************************************************************************************/

void CWndParaSettings::SetViewFocus(int iBtn)
{
	switch(iBtn)
	{
	case IDC_BTN_PARA_LEAKCOMP:
		{
			if(m_pcPara_LeakCompensation)
			{
				if(m_pcPara_LeakCompensation->IsWindowVisible())
					m_pcPara_LeakCompensation->SetFocus();
			}
		}
		break;
	case IDC_BTN_PARA_MANBREATH:
		{
			if(m_pcPara_ManBreath)
			{
				if(m_pcPara_ManBreath->IsWindowVisible())
					m_pcPara_ManBreath->SetFocus();
			}

		}
		break;
	case IDC_BTN_PARA_PCURVE:
		{
			if(m_pcPara_PCurve)
			{
				if(m_pcPara_PCurve->IsWindowVisible())
					m_pcPara_PCurve->SetFocus();
			}
			
		}
		break;
	case IDC_BTN_PARA_EFLOW:
		{
			if(m_pcPara_EFLOW)
			{
				if(true==getModel()->getCONFIG()->IsEFLOWequalILFOW()&&getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_IFLOW)
				{
					if(m_pcPara_ManBreath)
					{
						if(m_pcPara_ManBreath->IsWindowVisible())
						{
							m_pcPara_ManBreath->SetFocus();
							getModel()->getCONFIG()->SetLastSelectedParaViewBtns(m_pcPara_ManBreath->GetDlgCtrlID());
						}
						else
						{
							int iID=0;

							CWnd* pWnd = GetNextDlgTabItem(m_pcPara_ManBreath,FALSE);
							if(pWnd==NULL)
								return;

							iID=pWnd->GetDlgCtrlID();

							pWnd->SetFocus();
							getModel()->getCONFIG()->SetLastSelectedParaViewBtns(iID);
						}
					}
				}
				else
				{
					if(m_pcPara_EFLOW->IsWindowVisible())
						m_pcPara_EFLOW->SetFocus();
				}
			}
		}
		break;
	case IDC_BTN_PARA_ABORTPSV:
		{
			if(m_pcPara_AbortPSV)
			{
				if(m_pcPara_AbortPSV->IsWindowVisible())
					m_pcPara_AbortPSV->SetFocus();
			}
		}
		break;
	case IDC_BTN_PARA_BPM_REC:
		{
			if(m_pcPara_FREQ_REC)
			{
				if(m_pcPara_FREQ_REC->IsWindowVisible())
					m_pcPara_FREQ_REC->SetFocus();
			}

		}
		break;
	case IDC_BTN_PARA_FLUSHTIME:
		{
			if(m_pcPara_FlushTime)
			{
				if(m_pcPara_FlushTime->IsWindowVisible())
					m_pcPara_FlushTime->SetFocus();
			}
			
		}
		break;
	default:
		{
			if(m_pcPara_ManBreath)
			{
				if(m_pcPara_ManBreath->IsWindowVisible())
				{
					m_pcPara_ManBreath->SetFocus();
					getModel()->getCONFIG()->SetLastSelectedParaViewBtns(m_pcPara_ManBreath->GetDlgCtrlID());
				}
				else
				{
					int iID=0;

					CWnd* pWnd = GetNextDlgTabItem(m_pcPara_ManBreath,FALSE);
					if(pWnd==NULL)
						return;

					iID=pWnd->GetDlgCtrlID();

					pWnd->SetFocus();
					getModel()->getCONFIG()->SetLastSelectedParaViewBtns(iID);
				}
			}
			
		}
		break;
	}

	if(GetParent())
		GetParent()->PostMessage(WM_SET_PARATIMER);
	
}

/**********************************************************************************************//**
 * Sets next focus
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CWndParaSettings::SetNextFocus()
{
	CWnd* pFocWnd = CWnd::GetFocus();
	if(pFocWnd==NULL)
		return false;
	int iID=pFocWnd->GetDlgCtrlID();
	CWnd* pWnd = GetNextDlgTabItem(pFocWnd,FALSE);
	if(pWnd==NULL)
		return false;


	if(		pFocWnd->GetDlgCtrlID()>pWnd->GetDlgCtrlID()
		||	IDC_BTN_PARA_PCURVE<pWnd->GetDlgCtrlID())
		return false;

	iID=pWnd->GetDlgCtrlID();
	pFocWnd->CheckDlgButton(pFocWnd->GetDlgCtrlID(),BST_UNCHECKED);

	pWnd->SetFocus();
	getModel()->getCONFIG()->SetLastSelectedParaViewBtns(iID);


	return true;
}

/**********************************************************************************************//**
 * Sets previous focus
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CWndParaSettings::SetPrevFocus()
{
	CWnd* pFocWnd = CWnd::GetFocus();
	if(pFocWnd==NULL)
		return false;
	//iID=pFocWnd->GetDlgCtrlID();
	CWnd* pWnd = GetNextDlgTabItem(pFocWnd,TRUE);
	if(pWnd==NULL)
		return false;


	if(pFocWnd->GetDlgCtrlID()<pWnd->GetDlgCtrlID())
		return false;

	int iID=pWnd->GetDlgCtrlID();
	pFocWnd->CheckDlgButton(pFocWnd->GetDlgCtrlID(),BST_UNCHECKED);

	pWnd->SetFocus();
	getModel()->getCONFIG()->SetLastSelectedParaViewBtns(iID);

	return true;
}

/**********************************************************************************************//**
 * Draws this instance
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndParaSettings::Draw()
{
	RECT rcCl;
	RECT rc;
	GetClientRect(&rcCl);

	CClientDC dc(this);
	HDC hdcMem=CreateCompatibleDC(dc.m_hDC);
	HBITMAP hBmpMem=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
	HBITMAP hBmpMemPrev=(HBITMAP)SelectObject(hdcMem,hBmpMem);

	/*CBrush cbrBack(BACKGND);
	HBRUSH hbrprev=(HBRUSH)SelectObject(hdcMem,cbrBack);
	HPEN hpenprev=(HPEN)SelectObject(hdcMem, (HPEN)GetStockObject(NULL_PEN));	
	int nBkMode=SetBkMode(hdcMem,TRANSPARENT);
	HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf17AcuBold);
	int tc=SetTextColor(hdcMem,0x0000000);

	Rectangle(hdcMem, 0, 0, m_lX, m_lY);*/
	



	CBrush cbrGrey(RGB(181,178,181));
	CBrush cbrBack(BACKGND);
	/*CPen pen;
	pen.CreatePen(PS_SOLID,2,BLACK);*/
	HBRUSH hbrprev=(HBRUSH)SelectObject(hdcMem,cbrBack);
	HPEN hpenprev=(HPEN)SelectObject(hdcMem, (HPEN)GetStockObject(NULL_PEN));
	int nBkMode=SetBkMode(hdcMem,TRANSPARENT);
	HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf9AcuBold);
	int tc=SetTextColor(hdcMem,0x0000000);

	Rectangle(hdcMem, 0, 0, m_lX, m_lY);

	SelectObject(hdcMem,cbrGrey);
	Rectangle(hdcMem, 0, 1, m_lX, m_lY-1);




	CPen penLine;
	penLine.CreatePen(PS_SOLID,1,RGB(140,140,140));
	SelectObject(hdcMem,(HPEN)penLine);
	CBrush cbrRound(RGB(200,200,200));
	SelectObject(hdcMem,cbrRound);
	
	//SIMV+PSV
	//RoundRect(hdcMem, 315, 3, 425, 113,20,20);
	//if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true /*&& SpO2*/)
	//{
	//	RoundRect(hdcMem, 315, 3, 425, 113,20,20);

	//	rc.left = 315;  
	//	rc.top = 5;  
	//	rc.right  = 425;  
	//	rc.bottom = 45;
	//	DrawText(hdcMem,_T("PRICO"),-1,&rc,DT_CENTER|DT_VCENTER|DT_SINGLELINE);


	//}
	
	if(getModel()->getCONFIG()->isBiasFlowActive()==true && true==getModel()->getDATAHANDLER()->isHFOLicenseAvailable())
	{
		RoundRect(hdcMem, 430, 3, 540, 113,20,20);
	}
	
	if(getModel()->getDATAHANDLER()->isVLIMITLicenseAvailable() 
		&& getModel()->getDATAHANDLER()->isVGUARANTLicenseAvailable())
	{
		RoundRect(hdcMem, 430, 117, 540, 227,20,20);
	}
	
	if(m_enumMessage==PARAWARN_FRESHGAS_INT)
	{
		RoundRect(hdcMem, 10, 10, 420, 220,20,20);

		if(m_pcExclamation)
			m_pcExclamation->Draw(hdcMem,20,15);

		rc.left = 100;  
		rc.top = 25;  
		rc.right  = 400;  
		rc.bottom = 200;
		DrawText(hdcMem,getModel()->GetLanguageString(IDS_TXT_FRESHGAS_INT1),-1,&rc,DT_LEFT|DT_TOP|DT_WORDBREAK);
		
		SetTextColor(hdcMem,0x00000ff);

		rc.left = 25;  
		rc.top = 155;  
		rc.right  = 300;  
		rc.bottom = 300;
		//DrawText(hdcMem,_T("Please read user manual!"),-1,&rc,DT_LEFT|DT_TOP|DT_WORDBREAK);
		DrawText(hdcMem,getModel()->GetLanguageString(IDS_TXT_FRESHGAS_INT2),-1,&rc,DT_LEFT|DT_TOP|DT_WORDBREAK);

		SetTextColor(hdcMem,0x0000000);
		SelectObject(hdcMem,g_hf8AcuBold);

		rc.left = 310;  
		rc.top = 195;  
		rc.right  = 395;  
		rc.bottom = 300;
		DrawText(hdcMem,getModel()->GetLanguageString(IDS_BTN_CONFIRM),-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

	}
	else if(m_enumMessage==PARAWARN_FRESHGAS_EXT)
	{
		RoundRect(hdcMem, 10, 10, 420, 220,20,20);

		if(m_pcExclamation)
			m_pcExclamation->Draw(hdcMem,20,15);

		rc.left = 100;  
		rc.top = 25;  
		rc.right  = 400;  
		rc.bottom = 200;
		//DrawText(hdcMem,_T("Attention: Select this option if your NO application device is using a flow sensor for NO dosage calculation. It is mandatory to connect left port FG/nCPAP with inspiratory limb and flow sensor of NO device in between."),-1,&rc,DT_LEFT|DT_TOP|DT_WORDBREAK);
		DrawText(hdcMem,getModel()->GetLanguageString(IDS_TXT_FRESHGAS_EXT1),-1,&rc,DT_LEFT|DT_TOP|DT_WORDBREAK);

		SetTextColor(hdcMem,0x00000ff);

		rc.left = 25;  
		rc.top = 155;  
		rc.right  = 300;  
		rc.bottom = 300;
		//DrawText(hdcMem,_T("!! If not connected with inspiratory limb, no freshgas is delivered to patient !!"),-1,&rc,DT_LEFT|DT_TOP|DT_WORDBREAK);
		DrawText(hdcMem,getModel()->GetLanguageString(IDS_TXT_FRESHGAS_EXT2),-1,&rc,DT_LEFT|DT_TOP|DT_WORDBREAK);

		SetTextColor(hdcMem,0x0000000);
		SelectObject(hdcMem,g_hf8AcuBold);

		rc.left = 310;  
		rc.top = 195;  
		rc.right  = 395;  
		rc.bottom = 300;
		DrawText(hdcMem,getModel()->GetLanguageString(IDS_BTN_CONFIRM),-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);
	}

	//RoundRect(hdcMem, 430, 117, 540, 227,20,20);

	//dc.BitBlt(0,0,m_lX,m_lY,CDC::FromHandle(hdcMem),0,0,SRCCOPY);
	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,hdcMem,0,0,SRCCOPY);
	BitBlt(m_hDC,0,0,m_lX,m_lY,hdcMem,0,0,SRCCOPY);

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

//void CWndParaSettings::OnBnClickedShowSIMVPSV()
//{
//	m_pcSIMVPSV_OFF->Depress(false);
//}
//void CWndParaSettings::OnBnClickedHideSIMVPSV()
//{
//	m_pcSIMVPSV_ON->Depress(false);
//}

//void CWndParaSettings::OnBnClickedVG()
//{
//	m_pcVolumeLimit->Depress(false);
//	getModel()->getCONFIG()->SetVolumeControl(VCTRL_VGARANT);
//	getModel()->getDATAHANDLER()->SetCurrentVLimitParam(getModel()->getDATAHANDLER()->GetCurrentVLimitPara(),false,true);
//}
//void CWndParaSettings::OnBnClickedVL()
//{
//	m_pcVolumeGuarantee->Depress(false);
//	getModel()->getCONFIG()->SetVolumeControl(VCTRL_VLIMIT);
//	//getModel()->getDATAHANDLER()->SetCurrentVGarantParamData(getModel()->getDATAHANDLER()->GetCurrentVGarantParaData(),false,true);
//	getModel()->getDATAHANDLER()->SetActiveModeVolumeGarantyState(false,true,true);
//}

//void CWndParaSettings::OnBnClickedFreshgasInt()
//{
//	m_pcFreshgasExt->Depress(false);
//	getModel()->getCONFIG()->setFreshgasExt(false);
//	getModel()->Send_MODE_OPTION2();
//}
//void CWndParaSettings::OnBnClickedFreshgasExt()
//{
//	m_pcFreshgasInt->Depress(false);
//	getModel()->getCONFIG()->setFreshgasExt(true);
//	getModel()->Send_MODE_OPTION2();
//}

//FRESHGAS
//void CWndParaSettings::SetAllButtonUnpressed_Freshgas()
//{
//	m_pcFreshgasInt->SetBtnState(CPresetMenuBtn::UP);
//	m_pcFreshgasExt->SetBtnState(CPresetMenuBtn::UP);
//}

/**********************************************************************************************//**
 * Shows the freshgas message
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	btnID	Identifier for the button.
 **************************************************************************************************/

void CWndParaSettings::showFreshgasMessage(int btnID)
{
	if(btnID==IDC_BTN_FRESHGAS_INT)
	{
		m_enumMessage=PARAWARN_FRESHGAS_INT;
	}
	else if(btnID==IDC_BTN_FRESHGAS_EXT)
	{
		m_enumMessage=PARAWARN_FRESHGAS_EXT;
	}
	SetOneButtonPreset_Freshgas(btnID);

	setVG_VL();

	Show(true);
}

/**********************************************************************************************//**
 * Sets one button depressed freshgas
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	btnID	Identifier for the button.
 **************************************************************************************************/

void CWndParaSettings::SetOneButtonDepressed_Freshgas(int btnID)
{
	m_enumMessage=PARAWARN_NONE;

	if(btnID==IDC_BTN_FRESHGAS_INT)
	{
		m_pcFreshgasInt->SetBtnState(CPresetMenuBtn::DOWN);
		if(m_pcFreshgasExt->GetBtnState()!=CPresetMenuBtn::UP)
			m_pcFreshgasExt->SetBtnState(CPresetMenuBtn::UP);

		getModel()->getCONFIG()->setFreshgasExt(false);
		getModel()->Send_MODE_OPTION2();
	}
	else if(btnID==IDC_BTN_FRESHGAS_EXT)
	{
		if(m_pcFreshgasInt->GetBtnState()!=CPresetMenuBtn::UP)
			m_pcFreshgasInt->SetBtnState(CPresetMenuBtn::UP);
		m_pcFreshgasExt->SetBtnState(CPresetMenuBtn::DOWN);
		
		getModel()->getCONFIG()->setFreshgasExt(true);
		getModel()->Send_MODE_OPTION2();
	}

	m_bBiasFlowConfirmed=false;
	m_pcPara_Confirm->Depress(false);

	setVG_VL();

	Show(true);
}

/**********************************************************************************************//**
 * Sets one button preset freshgas
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	btnID	Identifier for the button.
 **************************************************************************************************/

void CWndParaSettings::SetOneButtonPreset_Freshgas(int btnID)
{
	//m_enumMessage=PARAWARN_NONE;

	if(btnID==IDC_BTN_FRESHGAS_INT)
	{
		m_pcFreshgasInt->SetBtnState(CPresetMenuBtn::PRESET);
	}
	else if(btnID==IDC_BTN_FRESHGAS_EXT)
	{
		m_pcFreshgasExt->SetBtnState(CPresetMenuBtn::PRESET);
	}
	
	//Show(true);
}
//void CWndParaSettings::SetButtonState_Freshgas(int btnID, CPresetMenuBtn::eBtnState state)
//{	
//}
//void CWndParaSettings::ResetButtonPresetstate_Freshgas()
//{
//}

//VOLUME
//void CWndParaSettings::SetAllButtonUnpressed_Volume()
//{
//	m_pcVolumeGuarantee->SetBtnState(CPresetMenuBtn::UP);
//	m_pcVolumeLimit->SetBtnState(CPresetMenuBtn::UP);
//}

/**********************************************************************************************//**
 * Sets one button depressed volume
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	btnID	Identifier for the button.
 **************************************************************************************************/

void CWndParaSettings::SetOneButtonDepressed_Volume(int btnID)
{
	m_enumMessage=PARAWARN_NONE;

	if(btnID==IDC_BTN_SHOW_VOLUMELIMIT)
	{
		m_pcVolumeLimit->SetBtnState(CPresetMenuBtn::DOWN);
		if(m_pcVolumeGuarantee->GetBtnState()!=CPresetMenuBtn::UP)
			m_pcVolumeGuarantee->SetBtnState(CPresetMenuBtn::UP);

		getModel()->getCONFIG()->SetVolumeControl(VCTRL_VLIMIT);
		//getModel()->getDATAHANDLER()->SetActiveModeVolumeGarantyState(false);//EFLOW1
		getModel()->getDATAHANDLER()->setVLIMITControlEnabled();//EFLOW1

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_PARABN_VLIMIT);
	}
	else if(btnID==IDC_BTN_SHOW_VOLUMEGUARANTEE)
	{
		if(m_pcVolumeLimit->GetBtnState()!=CPresetMenuBtn::UP)
			m_pcVolumeLimit->SetBtnState(CPresetMenuBtn::UP);
		m_pcVolumeGuarantee->SetBtnState(CPresetMenuBtn::DOWN);

		getModel()->getCONFIG()->SetVolumeControl(VCTRL_VGARANT);
		//getModel()->getDATAHANDLER()->SetCurrentVLimitParam(getModel()->getDATAHANDLER()->GetCurrentVLimitPara(),false,true);//EFLOW1
		getModel()->getDATAHANDLER()->setVGARANTControlEnabled();//EFLOW1

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_PARABN_VGARANT);
	}

	setBiasFlow();

	Show(true);
}

/**********************************************************************************************//**
 * Sets one button preset volume
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	btnID	Identifier for the button.
 **************************************************************************************************/

void CWndParaSettings::SetOneButtonPreset_Volume(int btnID)
{
	m_enumMessage=PARAWARN_NONE;

	if(btnID==IDC_BTN_SHOW_VOLUMELIMIT)
	{
		m_pcVolumeLimit->SetBtnState(CPresetMenuBtn::PRESET);
	}
	else if(btnID==IDC_BTN_SHOW_VOLUMEGUARANTEE)
	{
		m_pcVolumeGuarantee->SetBtnState(CPresetMenuBtn::PRESET);
	}

	setBiasFlow();

	Show(true);
}
//void CWndParaSettings::SetButtonState_Volume(int btnID, CPresetMenuBtn::eBtnState state)
//{
//
//}
//void CWndParaSettings::ResetButtonPresetstate_Volume()
//{
//
//}

/**********************************************************************************************//**
 * Sets bias flow
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndParaSettings::setBiasFlow()
{
	if(getModel()->getCONFIG()->isBiasFlowActive()==true)
	{
		if(getModel()->getCONFIG()->getFreshgasExt()==true)
		{
			m_pcFreshgasInt->SetBtnState(CPresetMenuBtn::UP);
			m_pcFreshgasExt->SetBtnState(CPresetMenuBtn::DOWN);
		}
		else
		{
			m_pcFreshgasInt->SetBtnState(CPresetMenuBtn::DOWN);
			m_pcFreshgasExt->SetBtnState(CPresetMenuBtn::UP);
		}
	}
}

/**********************************************************************************************//**
 * Sets vg vl
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndParaSettings::setVG_VL()
{
	if(getModel()->getDATAHANDLER()->isVLIMITLicenseAvailable() 
		&& getModel()->getDATAHANDLER()->isVGUARANTLicenseAvailable())
	{
		m_pcVolumeGuarantee->ShowWindow(SW_SHOW);
		m_pcVolumeGuarantee->EnableWindow(TRUE);

		m_pcVolumeLimit->ShowWindow(SW_SHOW);
		m_pcVolumeLimit->EnableWindow(TRUE);

		if(getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VGARANT)
		{
			m_pcVolumeGuarantee->SetBtnState(CPresetMenuBtn::DOWN);
			m_pcVolumeLimit->SetBtnState(CPresetMenuBtn::UP);
		}
		else if(getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VLIMIT)
		{
			m_pcVolumeGuarantee->SetBtnState(CPresetMenuBtn::UP);
			m_pcVolumeLimit->SetBtnState(CPresetMenuBtn::DOWN);
		}
	}
}