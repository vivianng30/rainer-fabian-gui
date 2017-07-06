#include "StdAfx.h"
#include "FabianHFO.h"
#include "ViewParaBtn.h"
#include "LangAdmin.h"
#include "globDefs.h"
#include "MVViewHandler.h"

#include "DIBSectionLite.h"

#define COLOR_TXTBTNUP				0x00000000
#define COLOR_TXTBTNDW				0x00FFFFFF
#define COLOR_TXTSUBBTNDW			0x00000000
#define COLOR_TXTBTNFC				0x00000000
#define COLOR_TXTBTNDIS				0x00828282
//#define COLOR_TXTBTNSIG				0x000000FF

#define TIME_PARATIMER 45000

/**=================================================================================================
 * \fn CViewParaBtn::CViewParaBtn(int iViewID)
 *
 * \brief Initializes a new instance of the CViewParaBtn class.
 *
 * \author Rainer
 * \date 04 Dez 2015
 *
 * \param iViewID Identifier for the view.
 *===============================================================================================**/
CViewParaBtn::CViewParaBtn(int iViewID):
CMVView(iViewID)
{
	/** \brief The initialize critical section. */
	InitializeCriticalSection(&csDraw);
	m_bExit=false;
	m_bViewPraBtnShown=false;

	m_pcWndParaSettings=NULL;

	m_iOldPPSV=0;
	m_iOldPINSP=0;

	m_bPsvApnoe=getModel()->getDATAHANDLER()->isPSVapnoe();

	m_pcPara_HFFLOW=0;
	m_pcPara_RISETIME=NULL;
	m_pcPara_IFLOW=NULL;
	m_pcPara_PEEP=NULL;
	m_pcPara_PINSP=NULL;
	m_pcPara_PDUO=NULL;
	m_pcPara_PMANUAL=NULL;
	m_pcPara_VLIMIT=NULL;
	//m_pcPara_TRIGGER=NULL;
	m_pcPara_TRIGGERCONV=NULL;
	m_pcPara_TRIGGERNMODE=NULL;
	m_pcPara_BPM=NULL;
	m_pcPara_ITIME=NULL;
	m_pcPara_ETIME=NULL;
	m_pcPara_O2=NULL;
	m_pcPara_O2Flush=NULL;
	m_pcPara_VGARANT=NULL;
	m_pcPara_BACKUP=NULL;
	m_pcPara_CPAP=NULL;
	m_pcPara_FLOWMIN=NULL;
	m_pcPara_PMITT=NULL;
	m_pcPara_PPSV=NULL;
	m_pcPara_HFAMPL=NULL;
	m_pcPara_HFFREQ=NULL;
	m_pcPara_IERatio=NULL;
	m_pcPara_FLOW=NULL;
	m_pcPara_PMAXI=NULL;
	m_pcPara_AMPMAXI=NULL;

	m_pcPara_ITIME_REC=NULL;
	m_pcPara_PMEAN_REC=NULL;

	m_pcSubPara_TrigThreshold=NULL;
	m_pcSubPara_IE=NULL;
	m_pcSubPara_DemandFlow=NULL;
	m_pcSubPara_ETime=NULL;
	m_pcSubPara_PSVTIME=NULL;
	m_pcSubPara_BPM=NULL;
	m_pcSubPara_O2Flush=NULL;
	m_pcSubPara_O2act=NULL;
	m_pcSubPara_LEAKCOMP=NULL;
	m_pcSubPara_BIASFLOW=NULL;

	m_pcPara_UpT=NULL;
	m_pcPara_FcT=NULL;
	m_pcPara_DwT=NULL;
	m_pcPara_UpB=NULL;
	m_pcPara_DisB=NULL;
	m_pcPara_DisT=NULL;
	m_pcPara_FcB=NULL;
	m_pcPara_DwB=NULL;
	m_pcPara_SigT=NULL;
	m_pcPara_SigB=NULL;
	m_pcSubPara_Up=NULL;
	m_pcSubPara_Dw=NULL;
	m_pcSubPara_Sig=NULL;
	m_pcSubPara_Trig=NULL;

	m_pcParaGroup_UpT=NULL;
	m_pcParaGroup_FcT=NULL;
	m_pcParaGroup_DwT=NULL;
	m_pcParaGroup_SigT=NULL;
	m_pcParaGroup_UpB=NULL;
	m_pcParaGroup_FcB=NULL;
	m_pcParaGroup_DwB=NULL;
	m_pcParaGroup_SigB=NULL;

	m_pcPara_OnOff=NULL;
	m_pcPara_OnOff_Up=NULL;
	m_pcPara_OnOff_Dw=NULL;

	m_pcParaMenu_Up=NULL;
	m_pcParaMenu_Dw=NULL;
	m_pcParaMenu=NULL;
	m_pcLungRec=NULL;

	m_pWndMenuVentState=NULL;

	//m_bBPM_SetKey=false;
	//m_bCPAP_SetKey=false;
	//m_bETIME_SetKey=false;
	//m_bITIME_SetKey=false;
	//m_bPDUO_SetKey=false;
	//m_bPEEP_SetKey=false;
	//m_bPINSP_SetKey=false;
	//m_bPMAX_SetKey=false;
	//m_bPPSV_SetKey=false;
	////m_bPMan_SetKey=false;
	//m_bPMEAN_SetKey=false;
	//m_bHFamp_SetKey=false;
	//m_bHFfreq_SetKey=false;
	//m_bVLIMIT_SetKey=false;
	//m_bVGARANT_SetKey=false;
	//m_bPMEANREC_SetKey=false;
	//m_bTHERAPYFLOW_SetKey=false;
	
	m_bSettingsActive=false;
}

/**=================================================================================================
 * \fn CViewParaBtn::~CViewParaBtn(void)
 *
 * \brief Finalizes an instance of the CViewParaBtn class.
 *
 * \author Rainer
 * \date 04 Dez 2015
 *===============================================================================================**/
CViewParaBtn::~CViewParaBtn(void)
{
	/** \brief The delete critical section. */
	DeleteCriticalSection(&csDraw);
}


BEGIN_MESSAGE_MAP(CViewParaBtn, CMVView)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/**=================================================================================================
 * \fn bool CViewParaBtn::CreateView()
 *
 * \brief Creates the view.
 *
 * \author Rainer
 * \date 04 Dez 2015
 *
 * \return true if it succeeds, false if it fails.
 *===============================================================================================**/
bool CViewParaBtn::CreateView()
{
	RECT rcLd1={0,220,600,500};

	if (!Create(AfxGetApp()->GetMainWnd(),rcLd1,IDC_VIEW_PARABTN))
	{
		return false;
	}
	
	Initialize();
	Open();
	
	return true;
}

/**=================================================================================================
 * \fn BOOL CViewParaBtn::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext)
 *
 * \brief Creates the Window instance that will be represented by this object.
 *
 * \author Rainer
 * \date 04 Dez 2015
 *
 * \param pParentWnd The parent window.
 * \param rc		 The rectangle.
 * \param nID		 The identifier.
 * \param pContext   The context.
 *
 * \return true if it succeeds, false if it fails.
 *===============================================================================================**/
BOOL CViewParaBtn::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext) 
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
		//CBrush cbrBack(RED);
		HBRUSH hbrprev=(HBRUSH)SelectObject(m_hDC,cbrBack);
		HPEN hpenprev = (HPEN)SelectObject(m_hDC, (HPEN)GetStockObject(NULL_PEN));	


		Rectangle(m_hDC, 0, 0, m_lX, m_lY);

		SelectObject(m_hDC,hpenprev);
		SelectObject(m_hDC,hbrprev);
		//cbrBack.DeleteObject();//rkuNEWFIX

		return 1;
	}
	else
		return 0;
}

/**=================================================================================================
 * \fn bool CViewParaBtn::Initialize()
 *
 * \brief Initializes this object.
 *
 * \author Rainer
 * \date 04 Dez 2015
 *
 * \return true if it succeeds, false if it fails.
 *===============================================================================================**/
bool CViewParaBtn::Initialize()
{
	bool result = false;
	CClientDC dc(this);

	DWORD dwStyleNoTab = WS_CHILD/*|WS_VISIBLE*/|BS_OWNERDRAW|WS_TABSTOP;

	BTN btn;
	fVALUE fv;

	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);

	m_pcPara_OnOff_Up		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PARA_ONOFF_UP);
	m_pcPara_OnOff_Dw		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PARA_ONOFF_DW);
	

	m_pcPara_UpT		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PARA_UP_TOP);
	m_pcPara_FcT		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PARA_FC_TOP);
	m_pcPara_DwT		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PARA_DW_TOP);
	m_pcPara_DisT		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PARA_DIS_TOP);
	m_pcPara_UpB		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PARA_UP_BOT);
	m_pcPara_FcB		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PARA_FC_BOT);
	m_pcPara_DwB		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PARA_DW_BOT);
	m_pcPara_DisB		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PARA_DIS_BOT);
	m_pcPara_SigT		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PARA_SIG_TOP);
	m_pcPara_SigB		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PARA_SIG_BOT);

	m_pcSubPara_Up		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SUBPARA_UP);
	m_pcSubPara_Dw		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SUBPARA_DW);
	m_pcSubPara_Sig		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SUBPARA_SIG);
	m_pcSubPara_Trig	= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SUBPARA_TRIG);

	m_pcParaMenu_Up		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PARAMENU_UP);
	m_pcParaMenu_Dw		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PARAMENU_DW);
	

	m_pcParaGroup_UpT		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_GROUP_UP_TOP);
	m_pcParaGroup_FcT		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_GROUP_FC_TOP);
	m_pcParaGroup_DwT		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_GROUP_DW_TOP);
	m_pcParaGroup_SigT		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_GROUP_SIG_TOP);

	m_pcParaGroup_UpB		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_GROUP_UP_BOT);
	m_pcParaGroup_FcB		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_GROUP_FC_BOT);
	m_pcParaGroup_DwB		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_GROUP_DW_BOT);
	m_pcParaGroup_SigB		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_GROUP_SIG_BOT);

	m_pcLungRec		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_PIC_GROUP_TOP_BOT);

	btn.wID					= IDC_BTN_PARAMENU;	
	btn.poPosition.x		= 0;
	btn.poPosition.y		= 20;
	btn.pcBmpUp				= m_pcParaMenu_Up;
	btn.pcBmpDown			= m_pcParaMenu_Dw;
	btn.pcBmpFocus			= m_pcParaMenu_Up;
	btn.pcBmpDisabled		= m_pcParaMenu_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	
	m_pcParaMenu=new CUpDwnBtn(btn,COLOR_TXTBTNUP,false,true);
	m_pcParaMenu->Create(this,g_hf7AcuBold,0);
	m_pcParaMenu->SetText(getModel()->GetLanguageString(IDS_TXT_SETTINGS),getModel()->GetLanguageString(IDS_TXT_SETTINGS));
	m_pcParaMenu->ShowWindow(SW_SHOW);

	

	//-------------------------------------------------------------
	//Parameter Button------I-Flow---------------------------------
	btn.wID					= IDC_BTN_PARA_IFLOW;	
	btn.poPosition.x		= 55;
	btn.poPosition.y		= 20;
	btn.pcBmpUp				= m_pcPara_UpT;
	btn.pcBmpDown			= m_pcPara_DwT;
	btn.pcBmpFocus			= m_pcPara_FcT;
	btn.pcBmpDisabled		= m_pcPara_UpT;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
	{
		fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetIFlowMinPara_IPPV();
		fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetIFlowMaxPara_IPPV();
		fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetIFlowPara_IPPV();
	}
	else
	{
		fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetIFlowMinPara_TRIGGER();
		fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetIFlowMaxPara_TRIGGER();
		fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetIFlowPara_TRIGGER();
	}
	
	
	if(fv.iValue>fv.iUpperLimit)
		fv.iValue=fv.iUpperLimit;
	else if(fv.iValue<fv.iLowerLimit)
		fv.iValue=fv.iLowerLimit;
	m_pcPara_IFLOW=new CParaBtn_IFLOW(btn,0,false);
	m_pcPara_IFLOW->Create(this,dwStyleNoTab,fv);
	m_pcPara_IFLOW->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
	m_pcPara_IFLOW->SetNameText(getModel()->GetLanguageString(IDS_PARA_IFLOW));
	m_pcPara_IFLOW->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_LMIN));
	m_plParaBtn.AddTail(m_pcPara_IFLOW);
	
	//-------------------------------------------------------------
	//Parameter Button------RiseTime---------------------------------
	btn.wID					= IDC_BTN_PARA_RISETIME;	
	btn.poPosition.x		= 55;
	btn.poPosition.y		= 20;
	btn.pcBmpUp				= m_pcPara_UpT;
	btn.pcBmpDown			= m_pcPara_DwT;
	btn.pcBmpFocus			= m_pcPara_FcT;
	btn.pcBmpDisabled		= m_pcPara_UpT;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
	{
		fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetRisetimeMinPara_IPPV();
		fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetRisetimeMaxPara_IPPV();
		fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetRisetimePara_IPPV();
	}
	else
	{
		fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetRisetimeMinPara_TRIGGER();
		fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetRisetimeMaxPara_TRIGGER();
		fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetRisetimePara_TRIGGER();
	}

	if(fv.iValue>fv.iUpperLimit)
		fv.iValue=fv.iUpperLimit;
	else if(fv.iValue<fv.iLowerLimit)
		fv.iValue=fv.iLowerLimit;
	m_pcPara_RISETIME=new CParaBtn_RISETIME(btn,0,false);
	m_pcPara_RISETIME->Create(this,dwStyleNoTab,fv);
	m_pcPara_RISETIME->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
	m_pcPara_RISETIME->SetNameText(getModel()->GetLanguageString(IDS_PARA_RISETIME));
	m_pcPara_RISETIME->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_SECONDS));
	m_plParaBtn.AddTail(m_pcPara_RISETIME);

	
	//-------------------------------------------------------------
	//Parameter Button------PMEAN_RECRUITMENT---------------------------------
	btn.wID					= IDC_BTN_PARA_PMEAN_REC;	
	btn.poPosition.x		= 55;
	btn.poPosition.y		= 20;
	btn.pcBmpUp				= m_pcParaGroup_UpT;
	btn.pcBmpDown			= m_pcParaGroup_DwT;
	btn.pcBmpFocus			= m_pcParaGroup_FcT;
	btn.pcBmpDisabled		= m_pcParaGroup_UpT;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanRecMinPara();
	fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanRecMaxPara();

	fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanRecPara();
	if(fv.iValue>fv.iUpperLimit)
		fv.iValue=fv.iUpperLimit;
	else if(fv.iValue<fv.iLowerLimit)
		fv.iValue=fv.iLowerLimit;
	m_pcPara_PMEAN_REC=new CParaBtn_PMEAN_REC(this,btn,0,false);
	m_pcPara_PMEAN_REC->Create(this,dwStyleNoTab,fv);
	m_pcPara_PMEAN_REC->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
	m_pcPara_PMEAN_REC->SetNameText(getModel()->GetLanguageString(IDS_PARA_P));
	m_pcPara_PMEAN_REC->SetNameNoteText(getModel()->GetLanguageString(IDS_PARA_MITT)+_T(" ")+getModel()->GetLanguageString(IDS_PARA_REC));
	if(getModel()->getCONFIG()->GetPressureUnit()==PUNIT_MBAR)
	{
		m_pcPara_PMEAN_REC->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_MBAR));
	}
	else
	{
		m_pcPara_PMEAN_REC->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_CMH2O));
	}
	m_plParaBtn.AddTail(m_pcPara_PMEAN_REC);

	//-------------------------------------------------------------
	//Parameter Button------E-Flow---------------------------------
	//btn.wID					= IDC_BTN_PARA_EFLOW;	
	//btn.poPosition.x		= 145;
	//btn.poPosition.y		= 20;
	//btn.pcBmpUp				= m_pcPara_UpT;
	//btn.pcBmpDown			= m_pcPara_DwT;
	//btn.pcBmpFocus			= m_pcPara_FcT;
	//btn.pcBmpDisabled		= m_pcPara_UpT;
	//btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	/*if(getModel()->getDATAHANDLER()->activeModeIsIPPV())
	{
		fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetEFLOWMinPara_IPPV();
		fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetEFLOWMaxPara_IPPV();
	}
	else
	{
		fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetEFLOWMinPara_TRIGGER();
		fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetEFLOWMaxPara_TRIGGER();
	}
	fv.iValue=getModel()->getDATAHANDLER()->GetCurrentEFlowParaData();*/
	//if(fv.iValue>fv.iUpperLimit)
	//	fv.iValue=fv.iUpperLimit;
	//else if(fv.iValue<fv.iLowerLimit)
	//	fv.iValue=fv.iLowerLimit;
	////fv.tText[0]=0;
	//m_pcPara_EFLOW=new CParaBtn_EFLOW(btn,0,false);
	//m_pcPara_EFLOW->Create(this,dwStyleNoTab,fv);
	//m_pcPara_EFLOW->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
	//m_pcPara_EFLOW->SetNameText(getModel()->GetLanguageString(IDS_PARA_EFLOW));
	//m_pcPara_EFLOW->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_LMIN));
	//m_plParaBtn.AddTail(m_pcPara_EFLOW);
	

	//-------------------------------------------------------------
	//Parameter Button------Flow min---------------------------------
	btn.wID					= IDC_BTN_PARA_FLOWMIN;	
	btn.poPosition.x		= 145;
	btn.poPosition.y		= 20;
	btn.pcBmpUp				= m_pcPara_UpT;
	btn.pcBmpDown			= m_pcPara_DwT;
	btn.pcBmpFocus			= m_pcPara_FcT;
	btn.pcBmpDisabled		= m_pcPara_UpT;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetFlowminMinPara();
	fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetFlowminMaxPara();

	fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetFlowminPara();
	if(fv.iValue>fv.iUpperLimit)
		fv.iValue=fv.iUpperLimit;
	else if(fv.iValue<fv.iLowerLimit)
		fv.iValue=fv.iLowerLimit;
	m_pcPara_FLOWMIN=new CParaBtn_FLOWMIN(btn,0,false);
	m_pcPara_FLOWMIN->Create(this,dwStyleNoTab,fv);
	m_pcPara_FLOWMIN->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
	m_pcPara_FLOWMIN->SetNameText(getModel()->GetLanguageString(IDS_PARA_FLOW));
	m_pcPara_FLOWMIN->SetNameNoteText(getModel()->GetLanguageString(IDS_PARA_MIN));
	m_pcPara_FLOWMIN->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_LMIN));
	m_plParaBtn.AddTail(m_pcPara_FLOWMIN);

	//-------------------------------------------------------------
	//Parameter Button------PEEP---------------------------------
	btn.wID					= IDC_BTN_PARA_PEEP;	
	btn.poPosition.x		= 235;
	btn.poPosition.y		= 20;
	btn.pcBmpUp				= m_pcPara_UpT;
	btn.pcBmpDown			= m_pcPara_DwT;
	btn.pcBmpFocus			= m_pcPara_FcT;
	btn.pcBmpDisabled		= m_pcPara_UpT;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	
	if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
	{
		if(getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_LINEAR)
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPEEPMinPara_RISETIME();
		}
		else
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPEEPMinPara_IPPV();
		}
		fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPEEPMaxPara_IPPV();
		fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetPEEPPara_IPPV();
	}
	else
	{
		if(getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_LINEAR)
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPEEPMinPara_RISETIME();
		}
		else
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPEEPMinPara_TRIGGER();
		}
		fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPEEPMaxPara_TRIGGER();
		fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetPEEPPara_TRIGGER();
	}
	
	if(fv.iValue>fv.iUpperLimit)
		fv.iValue=fv.iUpperLimit;
	else if(fv.iValue<fv.iLowerLimit)
		fv.iValue=fv.iLowerLimit;
	m_pcPara_PEEP=new CParaBtn_PEEP(this,btn,0,false);
	m_pcPara_PEEP->Create(this,dwStyleNoTab,fv);
	m_pcPara_PEEP->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
	m_pcPara_PEEP->SetNameText(getModel()->GetLanguageString(IDS_PARA_PEEP));
	if(getModel()->getCONFIG()->GetPressureUnit()==PUNIT_MBAR)
	{
		m_pcPara_PEEP->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_MBAR));
	}
	else
	{
		m_pcPara_PEEP->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_CMH2O));
	}
	m_plParaBtn.AddTail(m_pcPara_PEEP);
	

	//-------------------------------------------------------------
	//Parameter Button------HF Flow---------------------------------
	btn.wID					= IDC_BTN_PARA_HFFLOW;	
	btn.poPosition.x		= 145;//235;
	btn.poPosition.y		= 20;
	btn.pcBmpUp				= m_pcPara_UpT;
	btn.pcBmpDown			= m_pcPara_DwT;
	btn.pcBmpFocus			= m_pcPara_FcT;
	btn.pcBmpDisabled		= m_pcPara_UpT;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;


	fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFFlowMinPara();
	fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFFlowMaxPara();

	fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetHFFlowPara();
	if(fv.iValue>fv.iUpperLimit)
		fv.iValue=fv.iUpperLimit;
	else if(fv.iValue<fv.iLowerLimit && fv.iValue!=0)
		fv.iValue=fv.iLowerLimit;
	m_pcPara_HFFLOW=new CParaBtn_HFFLOW(btn,0,false);
	m_pcPara_HFFLOW->Create(this,dwStyleNoTab,fv);
	m_pcPara_HFFLOW->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
	m_pcPara_HFFLOW->SetNameText(getModel()->GetLanguageString(IDS_PARA_FLOW));
	m_pcPara_HFFLOW->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_LMIN));
	m_plParaBtn.AddTail(m_pcPara_HFFLOW);


	//-------------------------------------------------------------
	//Parameter Button------CPAP---------------------------------
	btn.wID					= IDC_BTN_PARA_CPAP;	
	btn.poPosition.x		= 235;
	btn.poPosition.y		= 20;
	btn.pcBmpUp				= m_pcPara_UpT;
	btn.pcBmpDown			= m_pcPara_DwT;
	btn.pcBmpFocus			= m_pcPara_FcT;
	btn.pcBmpDisabled		= m_pcPara_UpT;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	if(getModel()->getVMODEHANDLER()->activeModeIsNMODE())
	{
		fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetCPAPNMODEMinPara();
		fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetCPAPNMODEMaxPara();
		fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetCPAPNMODEPara();
	}
	else
	{
		fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetCPAPMinPara();
		fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetCPAPMaxPara();
		fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetCPAPPara();//GetPEEPPara_IPPV
	}

	if(fv.iValue>fv.iUpperLimit)
		fv.iValue=fv.iUpperLimit;
	else if(fv.iValue<fv.iLowerLimit)
		fv.iValue=fv.iLowerLimit;
	m_pcPara_CPAP=new CParaBtn_CPAP(btn,0,false);
	m_pcPara_CPAP->Create(this,dwStyleNoTab,fv);
	m_pcPara_CPAP->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
	m_pcPara_CPAP->SetNameText(getModel()->GetLanguageString(IDS_PARA_CPAP));
	if(getModel()->getCONFIG()->GetPressureUnit()==PUNIT_MBAR)
	{
		m_pcPara_CPAP->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_MBAR));
	}
	else
	{
		m_pcPara_CPAP->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_CMH2O));
	}
	m_plParaBtn.AddTail(m_pcPara_CPAP);


	//-------------------------------------------------------------
	//Parameter Button------P Insp---------------------------------
	btn.wID					= IDC_BTN_PARA_PINSP;	
	btn.poPosition.x		= 325;
	btn.poPosition.y		= 20;
	btn.pcBmpUp				= m_pcPara_UpT;
	btn.pcBmpDown			= m_pcPara_DwT;
	btn.pcBmpFocus			= m_pcPara_FcT;
	btn.pcBmpDisabled		= m_pcPara_UpT;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;
	
	if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
	{
		fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPINSPMinPara_IPPV();
		fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPINSPMaxPara_IPPV();
	}
	else
	{
		fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPINSPMinPara_TRIGGER();
		fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPINSPMaxPara_TRIGGER();
	}

	fv.iValue=getModel()->getDATAHANDLER()->GetCurrentPINSPPara();
	if(fv.iValue>fv.iUpperLimit)
		fv.iValue=fv.iUpperLimit;
	else if(fv.iValue<fv.iLowerLimit)
		fv.iValue=fv.iLowerLimit;
	m_pcPara_PINSP=new CParaBtn_PINSP(btn,0,false);
	m_pcPara_PINSP->Create(this,dwStyleNoTab,fv);
	m_pcPara_PINSP->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
	m_pcPara_PINSP->SetNameText(getModel()->GetLanguageString(IDS_PARA_P));

	if(getModel()->getVMODEHANDLER()->modeIsPSVmode())//newVG
	{
		m_pcPara_PINSP->SetNameNoteText(getModel()->GetLanguageString(IDS_PARA_BACKUP));
	}
	else
	{
		m_pcPara_PINSP->SetNameNoteText(getModel()->GetLanguageString(IDS_PARA_INSP));
	}

	if(getModel()->getCONFIG()->GetPressureUnit()==PUNIT_MBAR)
	{
		m_pcPara_PINSP->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_MBAR));
	}
	else
	{
		m_pcPara_PINSP->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_CMH2O));
	}
	m_plParaBtn.AddTail(m_pcPara_PINSP);

	//-------------------------------------------------------------
	//Parameter Button------P Duo---------------------------------
	btn.wID					= IDC_BTN_PARA_PDUO;	
	btn.poPosition.x		= 325;
	btn.poPosition.y		= 20;
	btn.pcBmpUp				= m_pcPara_UpT;
	btn.pcBmpDown			= m_pcPara_DwT;
	btn.pcBmpFocus			= m_pcPara_FcT;
	btn.pcBmpDisabled		= m_pcPara_UpT;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPManualNMODEMinPara();
	fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPManualNMODEMaxPara();
	fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetPManualNMODEPara();
	if(fv.iValue>fv.iUpperLimit)
		fv.iValue=fv.iUpperLimit;
	else if(fv.iValue<fv.iLowerLimit)
		fv.iValue=fv.iLowerLimit;
	m_pcPara_PDUO=new CParaBtn_PDUO(btn,0,false);
	m_pcPara_PDUO->Create(this,dwStyleNoTab,fv);
	m_pcPara_PDUO->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
	m_pcPara_PDUO->SetNameText(getModel()->GetLanguageString(IDS_PARA_DUO_P));
	m_pcPara_PDUO->SetNameNoteText(getModel()->GetLanguageString(IDS_PARA_DUO));
	if(getModel()->getCONFIG()->GetPressureUnit()==PUNIT_MBAR)
	{
		m_pcPara_PDUO->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_MBAR));
	}
	else
	{
		m_pcPara_PDUO->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_CMH2O));
	}
	m_plParaBtn.AddTail(m_pcPara_PDUO);

	//-------------------------------------------------------------
	//Parameter Button------HF Ampl---------------------------------
	btn.wID					= IDC_BTN_PARA_HFAMPL;	
	btn.poPosition.x		= 235;//415;
	btn.poPosition.y		= 20;
	btn.pcBmpUp				= m_pcPara_UpT;
	btn.pcBmpDown			= m_pcPara_DwT;
	btn.pcBmpFocus			= m_pcPara_FcT;
	btn.pcBmpDisabled		= m_pcPara_UpT;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLMinPara();
	fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLMaxPara();

	fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLPara();
	if(fv.iValue>fv.iUpperLimit)
		fv.iValue=fv.iUpperLimit;
	else if(fv.iValue<fv.iLowerLimit)
		fv.iValue=fv.iLowerLimit;
	m_pcPara_HFAMPL=new CParaBtn_HFAMPL(btn,0,false);
	m_pcPara_HFAMPL->Create(this,dwStyleNoTab,fv);
	m_pcPara_HFAMPL->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
	m_pcPara_HFAMPL->SetNameText(_T("Amp"));
	if(getModel()->getCONFIG()->GetPressureUnit()==PUNIT_MBAR)
	{
		m_pcPara_HFAMPL->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_MBAR));
	}
	else
	{
		m_pcPara_HFAMPL->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_CMH2O));
	}
	m_plParaBtn.AddTail(m_pcPara_HFAMPL);

	//-------------------------------------------------------------
	//Parameter Button------P Man---------------------------------
	btn.wID					= IDC_BTN_PARA_PMANUAL;	
	btn.poPosition.x		= 325;
	btn.poPosition.y		= 20;
	btn.pcBmpUp				= m_pcPara_UpT;
	btn.pcBmpDown			= m_pcPara_DwT;
	btn.pcBmpFocus			= m_pcPara_FcT;
	btn.pcBmpDisabled		= m_pcPara_UpT;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	if(getModel()->getVMODEHANDLER()->activeModeIsNMODE())
	{
		fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPManualNMODEMinPara();
		fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPManualNMODEMaxPara();
		fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetPManualNMODEPara();
	}
	else if(getModel()->getVMODEHANDLER()->activeModeIsCPAP())
	{
		fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPManualMinPara();
		fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPManualMaxPara();
		fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetPManualCPAPPara();
	}
	else
	{
		fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPManualMinPara();
		fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPManualMaxPara();
		fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetPManualHFOPara();
	}
	//else//PMAN1
	//{
	//	if(getModel()->getDATAHANDLER()->isHFOManBreathAvailable())
	//		//if(getModel()->getCONFIG()->IsHFOManBreathEnabled())
	//	{
	//		fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanPara();
	//	}
	//	else
	//	{
	//		fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPManualMinPara();
	//	}
	//	fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPManualMaxPara();
	//	fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetPManualHFOPara();

	//	if(fv.iValue<getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanPara()+10)
	//	{
	//		fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanPara()+10;
	//		getModel()->getDATAHANDLER()->SetPManualHFOParadata(fv.iValue);
	//	}
	//}
	
	if(fv.iValue>fv.iUpperLimit)
		fv.iValue=fv.iUpperLimit;
	else if(fv.iValue<fv.iLowerLimit)
		fv.iValue=fv.iLowerLimit;
	m_pcPara_PMANUAL=new CParaBtn_PMANUAL(btn,0,false);
	m_pcPara_PMANUAL->Create(this,dwStyleNoTab,fv);
	m_pcPara_PMANUAL->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
	m_pcPara_PMANUAL->SetNameText(getModel()->GetLanguageString(IDS_PARA_P));
	m_pcPara_PMANUAL->SetNameNoteText(getModel()->GetLanguageString(IDS_PARA_MANUAL));
	if(getModel()->getCONFIG()->GetPressureUnit()==PUNIT_MBAR)
	{
		m_pcPara_PMANUAL->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_MBAR));
	}
	else
	{
		m_pcPara_PMANUAL->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_CMH2O));
	}
	m_plParaBtn.AddTail(m_pcPara_PMANUAL);

	//-------------------------------------------------------------
	//Parameter Button------Flow min---------------------------------
	btn.wID					= IDC_BTN_PARA_THERAPYFLOW;	
	btn.poPosition.x		= 415;
	btn.poPosition.y		= 20;
	btn.pcBmpUp				= m_pcPara_UpT;
	btn.pcBmpDown			= m_pcPara_DwT;
	btn.pcBmpFocus			= m_pcPara_FcT;
	btn.pcBmpDisabled		= m_pcPara_UpT;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetTherapieFlowMinPara();
	fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetTherapieFlowMaxPara();

	fv.iValue=getModel()->getDATAHANDLER()->GetFlowminParaData();
	if(fv.iValue>fv.iUpperLimit)
		fv.iValue=fv.iUpperLimit;
	else if(fv.iValue<fv.iLowerLimit)
		fv.iValue=fv.iLowerLimit;
	m_pcPara_FLOW=new CParaBtn_FLOW(btn,0,false);
	m_pcPara_FLOW->Create(this,dwStyleNoTab,fv);
	m_pcPara_FLOW->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
	m_pcPara_FLOW->SetNameText(getModel()->GetLanguageString(IDS_PARA_FLOW));
	m_pcPara_FLOW->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_LMIN));
	m_plParaBtn.AddTail(m_pcPara_FLOW);

	//-------------------------------------------------------------
	//Parameter Button------P PSV---------------------------------
	btn.wID					= IDC_BTN_PARA_PPSV;	
	btn.poPosition.x		= 415;
	btn.poPosition.y		= 20;
	btn.pcBmpUp				= m_pcPara_UpT;
	btn.pcBmpDown			= m_pcPara_DwT;
	btn.pcBmpFocus			= m_pcPara_FcT;
	btn.pcBmpDisabled		= m_pcPara_UpT;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPpsvMinPara();
	fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPpsvMaxPara();

	fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetPpsvPara();
	if(fv.iValue>fv.iUpperLimit)
		fv.iValue=fv.iUpperLimit;
	else if(fv.iValue<fv.iLowerLimit)
		fv.iValue=fv.iLowerLimit;
	m_pcPara_PPSV=new CParaBtn_PPSV(this,btn,0,false);
	m_pcPara_PPSV->Create(this,dwStyleNoTab,fv);
	m_pcPara_PPSV->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
	m_pcPara_PPSV->SetNameText(getModel()->GetLanguageString(IDS_PARA_P));
	m_pcPara_PPSV->SetNameNoteText(getModel()->GetLanguageString(IDS_PARA_PSV));
	if(getModel()->getCONFIG()->GetPressureUnit()==PUNIT_MBAR)
	{
		m_pcPara_PPSV->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_MBAR)+_T(" ")+getModel()->GetLanguageString(IDS_UNIT_OVERPEEP));
	}
	else
	{
		m_pcPara_PPSV->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_CMH2O)+_T(" ")+getModel()->GetLanguageString(IDS_UNIT_OVERPEEP));
	}
	m_plParaBtn.AddTail(m_pcPara_PPSV);
	

	//-------------------------------------------------------------
	//Parameter Button------P Mitt---------------------------------
	btn.wID					= IDC_BTN_PARA_PMITT;	
	btn.poPosition.x		= 415;
	btn.poPosition.y		= 20;
	btn.pcBmpUp				= m_pcPara_UpT;
	btn.pcBmpDown			= m_pcPara_DwT;
	btn.pcBmpFocus			= m_pcPara_FcT;
	btn.pcBmpDisabled		= m_pcPara_UpT;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanMinPara();
	fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanMaxPara();

	fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanPara();
	if(fv.iValue>fv.iUpperLimit)
		fv.iValue=fv.iUpperLimit;
	else if(fv.iValue<fv.iLowerLimit)
		fv.iValue=fv.iLowerLimit;
	m_pcPara_PMITT=new CParaBtn_PMITT(this,btn,0,false);
	m_pcPara_PMITT->Create(this,dwStyleNoTab,fv);
	m_pcPara_PMITT->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
	m_pcPara_PMITT->SetNameText(getModel()->GetLanguageString(IDS_PARA_P));
	m_pcPara_PMITT->SetNameNoteText(getModel()->GetLanguageString(IDS_PARA_MITT));
	if(getModel()->getCONFIG()->GetPressureUnit()==PUNIT_MBAR)
	{
		m_pcPara_PMITT->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_MBAR));
	}
	else
	{
		m_pcPara_PMITT->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_CMH2O));
	}
	m_plParaBtn.AddTail(m_pcPara_PMITT);

	if(getModel()->getDATAHANDLER()->isVGUARANTLicenseAvailable())
	{
		btn.wID					= IDC_BTN_PARA_VG_PMAXI;	
		btn.poPosition.x		= 505;
		btn.poPosition.y		= 20;
		btn.pcBmpUp				= m_pcPara_UpT;
		btn.pcBmpDown			= m_pcPara_DwT;
		btn.pcBmpFocus			= m_pcPara_FcT;
		btn.pcBmpDisabled		= m_pcPara_UpT;

		btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

		if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPMAXMinPara_IPPV();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPMAXMaxPara_IPPV();
			fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetPmaxVolGPara_IPPV();
		}
		else
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPMAXMinPara_TRIGGER();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPMAXMaxPara_TRIGGER();
			fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetPmaxVolGPara_TRIGGER();
		}
		
		if(fv.iValue>fv.iUpperLimit)
			fv.iValue=fv.iUpperLimit;
		else if(fv.iValue<fv.iLowerLimit)
			fv.iValue=fv.iLowerLimit;
		m_pcPara_PMAXI=new CParaBtn_PMAX(btn,0,false);
		m_pcPara_PMAXI->Create(this,dwStyleNoTab,fv);
		m_pcPara_PMAXI->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
		m_pcPara_PMAXI->SetNameText(getModel()->GetLanguageString(IDS_PARA_P));
		m_pcPara_PMAXI->SetNameNoteText(_T(""));
		if(getModel()->getCONFIG()->GetPressureUnit()==PUNIT_MBAR)
		{
			m_pcPara_PMAXI->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_MBAR));
		}
		else
		{
			m_pcPara_PMAXI->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_CMH2O));
		}

		m_pcPara_PMAXI->SetOffState(!getModel()->getDATAHANDLER()->IsCurrentModeVGarantStateOn());
		m_pcPara_PMAXI->ShowWindow(SW_HIDE);
		m_plParaBtn.AddTail(m_pcPara_PMAXI);



		btn.wID					= IDC_BTN_PARA_VG_AMPMAXI;	
		btn.poPosition.x		= 505;
		btn.poPosition.y		= 20;
		btn.pcBmpUp				= m_pcPara_UpT;
		btn.pcBmpDown			= m_pcPara_DwT;
		btn.pcBmpFocus			= m_pcPara_FcT;
		btn.pcBmpDisabled		= m_pcPara_UpT;

		btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

		fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLMinPara();
		fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLMaxPara();

		fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLmaxPara();

		if(fv.iValue>fv.iUpperLimit)
			fv.iValue=fv.iUpperLimit;
		else if(fv.iValue<fv.iLowerLimit)
			fv.iValue=fv.iLowerLimit;
		m_pcPara_AMPMAXI=new CParaBtn_AMPMAX(btn,0,false);
		m_pcPara_AMPMAXI->Create(this,dwStyleNoTab,fv);
		m_pcPara_AMPMAXI->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
		m_pcPara_AMPMAXI->SetNameText(getModel()->GetLanguageString(IDS_PARA_P));
		m_pcPara_AMPMAXI->SetNameNoteText(_T(""));
		if(getModel()->getCONFIG()->GetPressureUnit()==PUNIT_MBAR)
		{
			m_pcPara_AMPMAXI->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_MBAR));
		}
		else
		{
			m_pcPara_AMPMAXI->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_CMH2O));
		}

		m_pcPara_AMPMAXI->SetOffState(!getModel()->getDATAHANDLER()->IsCurrentModeVGarantStateOn());
		m_pcPara_AMPMAXI->ShowWindow(SW_HIDE);
		m_plParaBtn.AddTail(m_pcPara_AMPMAXI);
	}
	
	/**************************************************************/
	//-------------------------------------------------------------
	//Parameter Button------Trigger---------------------------------
	btn.wID					= IDC_BTN_PARA_TRIGCONV;	
	btn.poPosition.x		= 55;
	btn.poPosition.y		= 165;
	btn.pcBmpUp				= m_pcPara_UpB;
	btn.pcBmpDown			= m_pcPara_DwB;
	btn.pcBmpFocus			= m_pcPara_FcB;
	btn.pcBmpDisabled		= m_pcPara_UpB;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetTriggerCONVMinPara();
	fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetTriggerCONVMaxPara();

	fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetTriggerCONVPara();
	if(fv.iValue>fv.iUpperLimit)
		fv.iValue=fv.iUpperLimit;
	else if(fv.iValue<fv.iLowerLimit)
		fv.iValue=fv.iLowerLimit;
	m_pcPara_TRIGGERCONV=new CParaBtn_TRIGGER(btn,0,false);
	m_pcPara_TRIGGERCONV->Create(this,dwStyleNoTab,fv);
	m_pcPara_TRIGGERCONV->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
	m_pcPara_TRIGGERCONV->SetNameText(getModel()->GetLanguageString(IDS_PARA_TRIGGER));
	m_pcPara_TRIGGERCONV->SetUnitText(_T(""));
	m_plParaBtn.AddTail(m_pcPara_TRIGGERCONV);


	/**************************************************************/
	//-------------------------------------------------------------
	//Parameter Button------Trigger---------------------------------
	btn.wID					= IDC_BTN_PARA_TRIGNMODE;	
	btn.poPosition.x		= 55;
	btn.poPosition.y		= 165;
	btn.pcBmpUp				= m_pcPara_UpB;
	btn.pcBmpDown			= m_pcPara_DwB;
	btn.pcBmpFocus			= m_pcPara_FcB;
	btn.pcBmpDisabled		= m_pcPara_DisB;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetTriggerNMODEMinPara();
	fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetTriggerNMODEMaxPara();

	fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetTriggerNMODEPara();
	if(fv.iValue>fv.iUpperLimit)
		fv.iValue=fv.iUpperLimit;
	else if(fv.iValue<fv.iLowerLimit)
		fv.iValue=fv.iLowerLimit;
	m_pcPara_TRIGGERNMODE=new CParaBtn_TRIGGERNMODE(btn,0,false);
	m_pcPara_TRIGGERNMODE->Create(this,dwStyleNoTab,fv);
	m_pcPara_TRIGGERNMODE->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
	m_pcPara_TRIGGERNMODE->SetNameText(getModel()->GetLanguageString(IDS_PARA_TRIGGER));
	m_pcPara_TRIGGERNMODE->SetUnitText(_T(""));
	m_plParaBtn.AddTail(m_pcPara_TRIGGERNMODE);

	//-------------------------------------------------------------
	//Parameter Button------ITIME Recruitment---------------------------------
	btn.wID					= IDC_BTN_PARA_ITIME_REC;	
	btn.poPosition.x		= 55;
	btn.poPosition.y		= 165;
	btn.pcBmpUp				= m_pcParaGroup_UpB;
	btn.pcBmpDown			= m_pcParaGroup_DwB;
	btn.pcBmpFocus			= m_pcParaGroup_FcB;
	btn.pcBmpDisabled		= m_pcParaGroup_UpB;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetITimeRecMinPara();
	fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetITimeRecMaxPara();
	fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetITimeRecPara();

	if(fv.iValue>fv.iUpperLimit)
		fv.iValue=fv.iUpperLimit;
	else if(fv.iValue<fv.iLowerLimit)
		fv.iValue=fv.iLowerLimit;
	m_pcPara_ITIME_REC=new CParaBtn_ITIME_REC(btn,0,false);
	m_pcPara_ITIME_REC->Create(this,dwStyleNoTab,fv);
	m_pcPara_ITIME_REC->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
	m_pcPara_ITIME_REC->SetNameText(getModel()->GetLanguageString(IDS_PARA_ITIME));
	m_pcPara_ITIME_REC->SetNameNoteText(getModel()->GetLanguageString(IDS_PARA_REC));
	m_pcPara_ITIME_REC->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_SECONDS));
	m_plParaBtn.AddTail(m_pcPara_ITIME_REC);

	//-------------------------------------------------------------
	//Parameter Button------I-Time---------------------------------
	btn.wID					= IDC_BTN_PARA_ITIME;	
	btn.poPosition.x		= 145;
	btn.poPosition.y		= 165;
	btn.pcBmpUp				= m_pcPara_UpB;
	btn.pcBmpDown			= m_pcPara_DwB;
	btn.pcBmpFocus			= m_pcPara_FcB;
	btn.pcBmpDisabled		= m_pcPara_UpB;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	if(getModel()->getVMODEHANDLER()->activeModeIsNMODE())
	{
		fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetITimeNMODEMinPara();
		fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetITimeNMODEMaxPara();
		fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetITimeNMODEPara();
	}
	else
	{
		if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetITimeMinPara_IPPV();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetITimeMaxPara_IPPV();
			fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetITimePara_IPPV();
		}
		else
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetITimeMinPara_TRIGGER();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetITimeMaxPara_TRIGGER();
			fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetITimePara_TRIGGER();
		}
	}
	
	if(fv.iValue>fv.iUpperLimit)
		fv.iValue=fv.iUpperLimit;
	else if(fv.iValue<fv.iLowerLimit)
		fv.iValue=fv.iLowerLimit;
	m_pcPara_ITIME=new CParaBtn_ITIME(btn,0,false);
	m_pcPara_ITIME->Create(this,dwStyleNoTab,fv);
	m_pcPara_ITIME->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
	m_pcPara_ITIME->SetNameText(getModel()->GetLanguageString(IDS_PARA_ITIME));
	m_pcPara_ITIME->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_SECONDS));
	m_plParaBtn.AddTail(m_pcPara_ITIME);

	//-------------------------------------------------------------
	//Parameter Button------Backup---------------------------------
	btn.wID					= IDC_BTN_PARA_BACKUP;	
	btn.poPosition.x		= 145;
	btn.poPosition.y		= 165;
	btn.pcBmpUp				= m_pcPara_UpB;
	btn.pcBmpDown			= m_pcPara_DwB;
	btn.pcBmpFocus			= m_pcPara_FcB;
	btn.pcBmpDisabled		= m_pcPara_UpB;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetBackupMinPara();
	fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetBackupMaxPara();

	fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetBackupPara();
	if(fv.iValue>fv.iUpperLimit)
		fv.iValue=fv.iUpperLimit;
	else if(fv.iValue<fv.iLowerLimit)
		fv.iValue=fv.iLowerLimit;
	m_pcPara_BACKUP=new CParaBtn_BACKUP(btn,0,false);
	m_pcPara_BACKUP->Create(this,dwStyleNoTab,fv);
	m_pcPara_BACKUP->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
	m_pcPara_BACKUP->SetNameText(getModel()->GetLanguageString(IDS_PARA_BACKUP));
	m_pcPara_BACKUP->SetUnitText(_T(""));
	m_plParaBtn.AddTail(m_pcPara_BACKUP);

	//-------------------------------------------------------------
	//Parameter Button------I/E Ratio HFO---------------------------------
	btn.wID					= IDC_BTN_PARA_IERATIO;	
	btn.poPosition.x		= 145;
	btn.poPosition.y		= 165;
	btn.pcBmpUp				= m_pcPara_UpB;
	btn.pcBmpDown			= m_pcPara_DwB;
	btn.pcBmpFocus			= m_pcPara_FcB;
	btn.pcBmpDisabled		= m_pcPara_UpB;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetIERatioMinPara();
	fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetIERatioMaxPara();

	fv.iValue=(int)getModel()->getDATAHANDLER()->PARADATA()->GetIERatioParaHFO();
	if(fv.iValue>fv.iUpperLimit)
		fv.iValue=fv.iUpperLimit;
	else if(fv.iValue<fv.iLowerLimit)
		fv.iValue=fv.iLowerLimit;
	m_pcPara_IERatio=new CParaBtn_IERatio(btn,0,false);
	m_pcPara_IERatio->Create(this,dwStyleNoTab,fv);
	m_pcPara_IERatio->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
	m_pcPara_IERatio->SetNameText(getModel()->GetLanguageString(IDS_PARA_IE));
	m_pcPara_IERatio->SetUnitText(_T(""));
	m_plParaBtn.AddTail(m_pcPara_IERatio);


	//-------------------------------------------------------------
	//Parameter Button------BPM---------------------------------
	btn.wID					= IDC_BTN_PARA_BPM;	
	btn.poPosition.x		= 235;
	btn.poPosition.y		= 165;
	btn.pcBmpUp				= m_pcPara_UpB;
	btn.pcBmpDown			= m_pcPara_DwB;
	btn.pcBmpFocus			= m_pcPara_FcB;
	btn.pcBmpDisabled		= m_pcPara_UpB;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	if(getModel()->getVMODEHANDLER()->activeModeIsNMODE())
	{
		fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetBPMNMODEMinPara();
		fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetBPMNMODEMaxPara();
		fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetBPMNMODEPara();
	}
	else
	{
		if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetBPMMinPara_IPPV();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetBPMMaxPara_IPPV();
			fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetBPMPara_IPPV();
		}
		else
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetBPMMinPara_TRIGGER();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetBPMMaxPara_TRIGGER();
			fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetBPMPara_TRIGGER();
		}
		
	}
	
	if(fv.iValue>fv.iUpperLimit)
		fv.iValue=fv.iUpperLimit;
	else if(fv.iValue<fv.iLowerLimit)
		fv.iValue=fv.iLowerLimit;
	m_pcPara_BPM=new CParaBtn_BPM(btn,0,false);
	m_pcPara_BPM->Create(this,dwStyleNoTab,fv);
	m_pcPara_BPM->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
	//m_pcPara_BPM->SetNameText(getModel()->GetLanguageString(IDS_PARA_FREQ));
	
	if(getModel()->getVMODEHANDLER()->modeIsPSVmode())//newVG
	{
		m_pcPara_BPM->SetNameText(_T(""));
		m_pcPara_BPM->SetNameNoteText(getModel()->GetLanguageString(IDS_PARA_FREQBACKUP));
	}
	else
	{
		m_pcPara_BPM->SetNameText(getModel()->GetLanguageString(IDS_PARA_FREQ));
		m_pcPara_BPM->SetNameNoteText(_T(""));
	}
	m_pcPara_BPM->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_BPM));
	m_plParaBtn.AddTail(m_pcPara_BPM);

	
	//-------------------------------------------------------------
	//Parameter Button------HF Freq---------------------------------
	btn.wID					= IDC_BTN_PARA_HFFREQ;	
	btn.poPosition.x		= 235;
	btn.poPosition.y		= 165;
	btn.pcBmpUp				= m_pcPara_UpB;
	btn.pcBmpDown			= m_pcPara_DwB;
	btn.pcBmpFocus			= m_pcPara_FcB;
	btn.pcBmpDisabled		= m_pcPara_UpB;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFFreqMinPara();
	fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFFreqMaxPara();

	fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetHFFreqPara();
	if(fv.iValue>fv.iUpperLimit)
		fv.iValue=fv.iUpperLimit;
	else if(fv.iValue<fv.iLowerLimit)
		fv.iValue=fv.iLowerLimit;
	m_pcPara_HFFREQ=new CParaBtn_HFFREQ(btn,0,false);
	m_pcPara_HFFREQ->Create(this,dwStyleNoTab,fv);
	m_pcPara_HFFREQ->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
	m_pcPara_HFFREQ->SetNameText(getModel()->GetLanguageString(IDS_PARA_HF_FREQ));
	m_pcPara_HFFREQ->SetNameNoteText(getModel()->GetLanguageString(IDS_PARA_HFFREQ_NOTE));
	m_pcPara_HFFREQ->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_HZ));
	m_plParaBtn.AddTail(m_pcPara_HFFREQ);


	//-------------------------------------------------------------
	//Parameter Button------E-Time---------------------------------
	btn.wID					= IDC_BTN_PARA_ETIME;	
	btn.poPosition.x		= 235;
	btn.poPosition.y		= 165;
	btn.pcBmpUp				= m_pcPara_UpB;
	btn.pcBmpDown			= m_pcPara_DwB;
	btn.pcBmpFocus			= m_pcPara_FcB;
	btn.pcBmpDisabled		= m_pcPara_UpB;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
	{
		fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetETIMEMinPara_IPPV();
		fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetETIMEMaxPara_IPPV();
		fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetETIMEPara_IPPV();
	}
	else
	{
		fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetETIMEMinPara_TRIGGER();
		fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetETIMEMaxPara_TRIGGER();
		fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetETIMEPara_TRIGGER();
	}
	
	if(fv.iValue>fv.iUpperLimit)
		fv.iValue=fv.iUpperLimit;
	else if(fv.iValue<fv.iLowerLimit)
		fv.iValue=fv.iLowerLimit;
	m_pcPara_ETIME=new CParaBtn_ETIME(this,btn,0,false);
	m_pcPara_ETIME->Create(this,dwStyleNoTab,fv);
	m_pcPara_ETIME->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
	m_pcPara_ETIME->SetNameText(getModel()->GetLanguageString(IDS_PARA_ETIME));
	m_pcPara_ETIME->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_SECONDS));
	m_plParaBtn.AddTail(m_pcPara_ETIME);


	//-------------------------------------------------------------
	//Parameter Button------O2---------------------------------
	btn.wID					= IDC_BTN_PARA_O2;	
	btn.poPosition.x		= 325;
	btn.poPosition.y		= 165;
	btn.pcBmpUp				= m_pcPara_UpB;
	btn.pcBmpDown			= m_pcPara_DwB;
	btn.pcBmpFocus			= m_pcPara_FcB;
	btn.pcBmpDisabled		= m_pcPara_UpB;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetO2MinPara();
	fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetO2MaxPara();

	fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetO2Para();
	if(fv.iValue>fv.iUpperLimit)
		fv.iValue=fv.iUpperLimit;
	else if(fv.iValue<fv.iLowerLimit)
		fv.iValue=fv.iLowerLimit;
	m_pcPara_O2=new CParaBtn_O2(this,btn,0,false);
	m_pcPara_O2->Create(this,dwStyleNoTab,fv);
	m_pcPara_O2->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
	m_pcPara_O2->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_PERCENT));
	m_plParaBtn.AddTail(m_pcPara_O2);


	//-------------------------------------------------------------
	//Parameter Button------O2 Flush---------------------------------
	btn.wID					= IDC_BTN_PARA_O2FLUSH;	
	btn.poPosition.x		= 415;
	btn.poPosition.y		= 165;
	btn.pcBmpUp				= m_pcPara_UpB;
	btn.pcBmpDown			= m_pcPara_DwB;
	btn.pcBmpFocus			= m_pcPara_FcB;
	btn.pcBmpDisabled		= m_pcPara_UpB;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetO2FlushMinPara();
	fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetO2FlushMaxPara();

	fv.iValue=getModel()->getDATAHANDLER()->GetCurrentO2FlushPara();
	if(fv.iValue>fv.iUpperLimit)
		fv.iValue=fv.iUpperLimit;
	else if(fv.iValue<fv.iLowerLimit)
		fv.iValue=fv.iLowerLimit;
	m_pcPara_O2Flush=new CParaBtn_O2Flush(btn,0,false);
	m_pcPara_O2Flush->Create(this,dwStyleNoTab,fv);
	m_pcPara_O2Flush->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
	m_pcPara_O2Flush->SetNameText(getModel()->GetLanguageString(IDS_PARA_O2));
	m_pcPara_O2Flush->SetNameNoteText(getModel()->GetLanguageString(IDS_PARA_FLUSH));
	m_pcPara_O2Flush->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_PERCENT));

	m_plParaBtn.AddTail(m_pcPara_O2Flush);


	if(getModel()->getDATAHANDLER()->isVGUARANTLicenseAvailable())
	{
		btn.wID					= IDC_BTN_PARA_VG_VL;	
		btn.poPosition.x		= 505;
		btn.poPosition.y		= 165;
		btn.pcBmpUp				= m_pcPara_UpB;
		btn.pcBmpDown			= m_pcPara_DwB;
		btn.pcBmpFocus			= m_pcPara_FcB;
		btn.pcBmpDisabled		= m_pcPara_UpB;

		btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

		if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFVGarantMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFVGarantMaxPara();

			fv.iValue=getModel()->getDATAHANDLER()->GetCurrentVGarantParaData();
		}
		else
		{
			if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetVGarantMinPara_IPPV();
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetVGarantMaxPara_IPPV();
			}
			else
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetVGarantMinPara_TRIGGER();
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetVGarantMaxPara_TRIGGER();
			}

			fv.iValue=getModel()->getDATAHANDLER()->GetCurrentVGarantParaData();
		}

		if(fv.iValue>fv.iUpperLimit)
			fv.iValue=fv.iUpperLimit;
		else if(fv.iValue<fv.iLowerLimit)
			fv.iValue=fv.iLowerLimit;
		m_pcPara_VGARANT=new CParaBtn_VGARANT(btn,0,false);
		m_pcPara_VGARANT->Create(this,dwStyleNoTab,fv);
		m_pcPara_VGARANT->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
		m_pcPara_VGARANT->SetNameText(getModel()->GetLanguageString(IDS_PARA_V));
		m_pcPara_VGARANT->SetNameNoteText(getModel()->GetLanguageString(IDS_PARA_GARANT));
		m_pcPara_VGARANT->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_MILLILITER));

		m_pcPara_VGARANT->SetOffState(!getModel()->getDATAHANDLER()->IsCurrentModeVGarantStateOn());
		m_pcPara_VGARANT->ShowWindow(SW_HIDE);
		m_plParaBtn.AddTail(m_pcPara_VGARANT);
	}

	//-------------------------------------------------------------
	//Parameter Button------V Limit---------------------------------
	if(getModel()->getDATAHANDLER()->isVLIMITLicenseAvailable())
	{
		btn.wID					= IDC_BTN_PARA_VG_VL;	
		btn.poPosition.x		= 505;
		btn.poPosition.y		= 165;
		btn.pcBmpUp				= m_pcPara_UpB;
		btn.pcBmpDown			= m_pcPara_DwB;
		btn.pcBmpFocus			= m_pcPara_FcB;
		btn.pcBmpDisabled		= m_pcPara_UpB;
		btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

		if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetVLimitMinPara_IPPV();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetVLimitMaxPara_IPPV();
		}
		else
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetVLimitMinPara_TRIGGER();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetVLimitMaxPara_TRIGGER();
		}

		fv.iValue=getModel()->getDATAHANDLER()->GetCurrentVLimitPara();
		if(fv.iValue>fv.iUpperLimit)
			fv.iValue=fv.iUpperLimit;
		else if(fv.iValue<fv.iLowerLimit)
			fv.iValue=fv.iLowerLimit;
		m_pcPara_VLIMIT=new CParaBtn_VLIMIT(btn,0,false);
		m_pcPara_VLIMIT->Create(this,dwStyleNoTab,fv);
		m_pcPara_VLIMIT->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
		m_pcPara_VLIMIT->SetNameText(getModel()->GetLanguageString(IDS_PARA_V));
		m_pcPara_VLIMIT->SetNameNoteText(getModel()->GetLanguageString(IDS_PARA_LIMIT));
		m_pcPara_VLIMIT->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_MILLILITER));
		m_pcPara_VLIMIT->SetOffState(!getModel()->getDATAHANDLER()->IsCurrentModeVLimitStateOn());
		m_pcPara_VLIMIT->ShowWindow(SW_HIDE);
		m_plParaBtn.AddTail(m_pcPara_VLIMIT);
	}

	//-------------------------------------------------------------
	//Parameter Button------V Garant---------------------------------
	btn.wID					= IDC_BTN_PARA_ONOFF;	
	btn.poPosition.x		= 505;//513;
	btn.poPosition.y		= 105;//178;
	btn.pcBmpUp				= m_pcPara_OnOff_Up;
	btn.pcBmpDown			= m_pcPara_OnOff_Dw;
	btn.pcBmpFocus			= m_pcPara_OnOff_Up;
	btn.pcBmpDisabled		= m_pcPara_OnOff_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;

	m_pcPara_OnOff=new CUpDwnBtn(btn,COLOR_TXTBTNUP,false,true);
	m_pcPara_OnOff->Create(this,g_hf7AcuBold,0);
	m_pcPara_OnOff->SetText(_T(""),_T(""));
	m_pcPara_OnOff->ShowWindow(SW_SHOW);

	if(getModel()->getCONFIG()->GetCurMode()==VM_PRE_HFO 
		||	getModel()->getCONFIG()->GetCurMode()==VM_HFO)
	{
		if(getModel()->getDATAHANDLER()->IsCurrentModeVGarantStateOn()==true)
			m_pcPara_OnOff->Depress(true);
		else
			m_pcPara_OnOff->Depress(false);
	}
	else
	{
		if(		getModel()->getDATAHANDLER()->isVGUARANTLicenseAvailable() 
			&&	getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VGARANT
			&&	m_pcPara_VGARANT)
		{
			if(getModel()->getDATAHANDLER()->IsCurrentModeVGarantStateOn()==true)
				m_pcPara_OnOff->Depress(true);
			else
				m_pcPara_OnOff->Depress(false);
		}
		else if(		getModel()->getDATAHANDLER()->isVLIMITLicenseAvailable() 
			&&	getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VLIMIT
			&&	m_pcPara_VLIMIT)
		{
			if(getModel()->getDATAHANDLER()->IsCurrentModeVLimitStateOn()==true)
				m_pcPara_OnOff->Depress(true);
			else
				m_pcPara_OnOff->Depress(false);
		}
	}

	if(getModel()->getCONFIG()->GetCurMode()==VM_PRE_HFO 
		||	getModel()->getCONFIG()->GetCurMode()==VM_HFO)
	{
		if(getModel()->getDATAHANDLER()->isVGUARANTLicenseAvailable())
		{
			if(m_pcPara_VLIMIT)
				m_pcPara_VLIMIT->ShowWindow(SW_HIDE);
			if(m_pcPara_VGARANT)
				m_pcPara_VGARANT->ShowWindow(SW_SHOW);
			if(m_pcPara_AMPMAXI)
				m_pcPara_AMPMAXI->ShowWindow(SW_SHOW);
			if(m_pcPara_PMAXI)
				m_pcPara_PMAXI->ShowWindow(SW_HIDE);
			if(m_pcPara_OnOff)
				m_pcPara_OnOff->ShowWindow(SW_SHOW);
		}
	}
	else if(		getModel()->getDATAHANDLER()->isVGUARANTLicenseAvailable() 
		&&	getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VGARANT)
	{
		if(m_pcPara_VLIMIT)
			m_pcPara_VLIMIT->ShowWindow(SW_HIDE);
		if(m_pcPara_VGARANT)
			m_pcPara_VGARANT->ShowWindow(SW_SHOW);
		if(m_pcPara_AMPMAXI)
			m_pcPara_AMPMAXI->ShowWindow(SW_HIDE);
		if(m_pcPara_PMAXI)
			m_pcPara_PMAXI->ShowWindow(SW_SHOW);
		if(m_pcPara_OnOff)
			m_pcPara_OnOff->ShowWindow(SW_SHOW);
	}
	else if(		getModel()->getDATAHANDLER()->isVLIMITLicenseAvailable() 
		&&	getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VLIMIT)
	{
		if(m_pcPara_VLIMIT)
			m_pcPara_VLIMIT->ShowWindow(SW_SHOW);
		if(m_pcPara_VGARANT)
			m_pcPara_VGARANT->ShowWindow(SW_HIDE);
		if(m_pcPara_AMPMAXI)
			m_pcPara_AMPMAXI->ShowWindow(SW_HIDE);
		if(m_pcPara_PMAXI)
			m_pcPara_PMAXI->ShowWindow(SW_HIDE);
		if(m_pcPara_OnOff)
			m_pcPara_OnOff->ShowWindow(SW_SHOW);
	}
	
	
	//-------------------------------------------------------------
	//SubParameter Button------TriggerVol---------------------------------
	btn.wID					= IDC_BTN_SUBPARA_TRIGGER;	
	btn.poPosition.x		= 55;
	btn.poPosition.y		= 105;
	btn.pcBmpUp				= m_pcSubPara_Up;
	btn.pcBmpDown			= m_pcSubPara_Up;
	btn.pcBmpFocus			= m_pcSubPara_Up;
	btn.pcBmpDisabled		= m_pcSubPara_Trig;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	fv.iValue=getModel()->getDATAHANDLER()->getAVGMessureDataTrigger();
	m_pcSubPara_TrigThreshold=new CSubParaBtn(btn,0,false);
	m_pcSubPara_TrigThreshold->Create(this,dwStyleNoTab,fv);
	m_pcSubPara_TrigThreshold->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
	
	if(		getModel()->getCONFIG()->GetCurMode()==VM_DUOPAP
		||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_DUOPAP)
	{
		m_pcSubPara_TrigThreshold->SetNameText(_T(""));
		m_pcSubPara_TrigThreshold->SetUnitText(_T(""));
	}
	else
	{
		if(getModel()->getDATAHANDLER()->getTriggerOptionCONV()==TRIGGER_VOLUME)
		{
			m_pcSubPara_TrigThreshold->SetNameText(getModel()->GetLanguageString(IDS_PARA_TRIGVOL));
			m_pcSubPara_TrigThreshold->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_MILLILITER));
		}
		else if(getModel()->getDATAHANDLER()->getTriggerOptionCONV()==TRIGGER_FLOW)
		{
			m_pcSubPara_TrigThreshold->SetNameText(getModel()->GetLanguageString(IDS_PARA_TRIGFLOW));
			m_pcSubPara_TrigThreshold->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_LMIN));
		}
		else //pressure trigger
		{
			m_pcSubPara_TrigThreshold->SetNameText(_T(""));
			m_pcSubPara_TrigThreshold->SetUnitText(_T(""));
		}
	}
	

	//-------------------------------------------------------------
	//SubParameter Button------LEAKCOMP----------------------------
	//pro, added
	btn.wID					= IDC_BTN_SUBPARA_LEAKCOMP;	
	btn.poPosition.x		= 505;//55;
	btn.poPosition.y		= 105;
	btn.pcBmpUp				= m_pcSubPara_Up;
	btn.pcBmpDown			= m_pcSubPara_Up;
	btn.pcBmpFocus			= m_pcSubPara_Up;
	btn.pcBmpDisabled		= m_pcSubPara_Sig;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	fv.iValue=0; 
	m_pcSubPara_LEAKCOMP=new CSubParaBtn(btn,0,false);
	m_pcSubPara_LEAKCOMP->Create(this,dwStyleNoTab,fv);
	m_pcSubPara_LEAKCOMP->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
	m_pcSubPara_LEAKCOMP->SetNameText(getModel()->GetLanguageString(IDS_PARA_LEAKCOMP));
	m_pcSubPara_LEAKCOMP->SetUnitText(_T(""));

	//-------------------------------------------------------------
	//SubParameter Button------LEAKCOMP----------------------------
	//pro, added
	btn.wID					= IDC_BTN_SUBPARA_BIASFLOW;	
	btn.poPosition.x		= 145;
	btn.poPosition.y		= 105;
	btn.pcBmpUp				= m_pcSubPara_Up;
	btn.pcBmpDown			= m_pcSubPara_Up;
	btn.pcBmpFocus			= m_pcSubPara_Up;
	btn.pcBmpDisabled		= m_pcSubPara_Sig;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	fv.iValue=0; 
	m_pcSubPara_BIASFLOW=new CSubParaBtn(btn,0,false);
	m_pcSubPara_BIASFLOW->Create(this,dwStyleNoTab,fv);
	m_pcSubPara_BIASFLOW->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
	m_pcSubPara_BIASFLOW->SetNameText(_T(""));
	m_pcSubPara_BIASFLOW->SetUnitText(_T(""));

	//-------------------------------------------------------------
	//SubParameter Button------IE---------------------------------
	btn.wID					= IDC_BTN_SUBPARA_I_E;	
	btn.poPosition.x		= 145;
	btn.poPosition.y		= 105;
	btn.pcBmpUp				= m_pcSubPara_Up;
	btn.pcBmpDown			= m_pcSubPara_Up;
	btn.pcBmpFocus			= m_pcSubPara_Up;
	btn.pcBmpDisabled		= m_pcSubPara_Sig;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	fv.iValue=getModel()->getDATAHANDLER()->getAVGMessureDataITimePSV();
	m_pcSubPara_IE=new CSubParaBtn(btn,0,false);
	m_pcSubPara_IE->Create(this,dwStyleNoTab,fv);
	m_pcSubPara_IE->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
	m_pcSubPara_IE->SetNameText(getModel()->GetLanguageString(IDS_PARA_IE));
	m_pcSubPara_IE->SetUnitText(_T(""));

	//-------------------------------------------------------------
	//SubParameter Button------Demand FLow---------------------------------
	btn.wID					= IDC_BTN_SUBPARA_DEMFLOW;	
	btn.poPosition.x		= 145;
	btn.poPosition.y		= 105;
	btn.pcBmpUp				= m_pcSubPara_Up;
	btn.pcBmpDown			= m_pcSubPara_Up;
	btn.pcBmpFocus			= m_pcSubPara_Up;
	btn.pcBmpDisabled		= m_pcSubPara_Sig;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	fv.iValue=getModel()->getDATAHANDLER()->GetDemandFlowData();
	m_pcSubPara_DemandFlow=new CSubParaBtn(btn,0,false);
	m_pcSubPara_DemandFlow->Create(this,dwStyleNoTab,fv);
	m_pcSubPara_DemandFlow->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
	m_pcSubPara_DemandFlow->SetNameText(getModel()->GetLanguageString(IDS_PARA_DEMFLOW));
	m_pcSubPara_DemandFlow->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_LMIN));


	//-------------------------------------------------------------
	//SubParameter Button------PSV-TIME---------------------------------
	btn.wID					= IDC_BTN_SUBPARA_PSVTIME;	
	btn.poPosition.x		= 145;
	btn.poPosition.y		= 105;
	btn.pcBmpUp				= m_pcSubPara_Up;
	btn.pcBmpDown			= m_pcSubPara_Up;
	btn.pcBmpFocus			= m_pcSubPara_Up;
	btn.pcBmpDisabled		= m_pcSubPara_Sig;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	fv.iValue=getModel()->getDATAHANDLER()->getAVGMessureDataITimePSV();
	m_pcSubPara_PSVTIME=new CSubParaBtn(btn,0,false);
	m_pcSubPara_PSVTIME->Create(this,dwStyleNoTab,fv);
	m_pcSubPara_PSVTIME->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
	m_pcSubPara_PSVTIME->SetNameText(getModel()->GetLanguageString(IDS_PARA_PSVTIME));
	m_pcSubPara_PSVTIME->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_SECONDS));

	
	//-------------------------------------------------------------
	//SubParameter Button------BPM---------------------------------
	btn.wID					= IDC_BTN_SUBPARA_BPM;	
	btn.poPosition.x		= 235;
	btn.poPosition.y		= 105;
	btn.pcBmpUp				= m_pcSubPara_Up;
	btn.pcBmpDown			= m_pcSubPara_Up;
	btn.pcBmpFocus			= m_pcSubPara_Up;
	btn.pcBmpDisabled		= m_pcSubPara_Sig;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	fv.iValue=getModel()->getDATAHANDLER()->GetCurrentBPMPara();
	m_pcSubPara_BPM=new CSubParaBtn(btn,0,false);
	m_pcSubPara_BPM->Create(this,dwStyleNoTab,fv);
	m_pcSubPara_BPM->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
	//m_pcSubPara_BPM->SetNameText(getModel()->GetLanguageString(IDS_PARA_FREQ));
	m_pcSubPara_BPM->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_BPM));
	if(getModel()->getVMODEHANDLER()->modeIsPSVmode())//newVG
	{
		m_pcSubPara_BPM->SetNameText(getModel()->GetLanguageString(IDS_PARA_FREQBACKUP));
	}
	else
	{
		m_pcSubPara_BPM->SetNameText(getModel()->GetLanguageString(IDS_PARA_FREQ));
	}

	//-------------------------------------------------------------
	//SubParameter Button------ETIME---------------------------------
	btn.wID					= IDC_BTN_SUBPARA_ETIME;	
	btn.poPosition.x		= 235;
	btn.poPosition.y		= 105;
	btn.pcBmpUp				= m_pcSubPara_Up;
	btn.pcBmpDown			= m_pcSubPara_Up;
	btn.pcBmpFocus			= m_pcSubPara_Up;
	btn.pcBmpDisabled		= m_pcSubPara_Sig;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	fv.iValue=getModel()->getDATAHANDLER()->GetCurrentETimePara();
	m_pcSubPara_ETime=new CSubParaBtn(btn,0,false);
	m_pcSubPara_ETime->Create(this,dwStyleNoTab,fv);
	m_pcSubPara_ETime->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
	m_pcSubPara_ETime->SetNameText(getModel()->GetLanguageString(IDS_PARA_ETIME));
	m_pcSubPara_ETime->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_SECONDS));


	//-------------------------------------------------------------
	//SubParameter Button------O2 Ist---------------------------------
	btn.wID					= IDC_BTN_SUBPARA_O2REAL;	
	btn.poPosition.x		= 325;
	btn.poPosition.y		= 105;
	btn.pcBmpUp				= m_pcSubPara_Up;
	btn.pcBmpDown			= m_pcSubPara_Up;
	btn.pcBmpFocus			= m_pcSubPara_Up;
	btn.pcBmpDisabled		= m_pcSubPara_Trig;//m_pcSubPara_Sig;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	fv.iValue=getModel()->getDATAHANDLER()->getMessureDataO2();
	m_pcSubPara_O2act=new CSubParaBtn(btn,0,false);
	m_pcSubPara_O2act->Create(this,dwStyleNoTab,fv);
	m_pcSubPara_O2act->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
	m_pcSubPara_O2act->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_PERCENT));

	//-------------------------------------------------------------
	//SubParameter Button------O2FLUSH---------------------------------
	btn.wID					= IDC_BTN_SUBPARA_O2FLUSH;	
	btn.poPosition.x		= 415;
	btn.poPosition.y		= 105;
	btn.pcBmpUp				= m_pcSubPara_Up;
	btn.pcBmpDown			= m_pcSubPara_Up;
	btn.pcBmpFocus			= m_pcSubPara_Up;
	btn.pcBmpDisabled		= m_pcSubPara_Trig;//m_pcSubPara_Sig;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	fv.iValue=getModel()->getDATAHANDLER()->GetCurO2FlushTime();

	m_pcSubPara_O2Flush=new CSubParaBtn(btn,0,false);
	m_pcSubPara_O2Flush->Create(this,dwStyleNoTab,fv);
	m_pcSubPara_O2Flush->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
	m_pcSubPara_O2Flush->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_SECONDS));

	
	result = CreateWndMenuVentState();

	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);

	return result;
}

/**=================================================================================================
 * \fn void CViewParaBtn::Open()
 *
 * \brief Opens this object.
 *
 * \author Rainer
 * \date 04 Dez 2015
 *===============================================================================================**/
void CViewParaBtn::Open()
{
	m_bViewPraBtnShown=false;
	ShowParaBtn(false);
	ShowWindow(SW_SHOW);
	UpdateWindow();
	Invalidate();
	m_bViewPraBtnShown=true;

	SetOxyHourglass(getModel()->getDATAHANDLER()->GetOxyCalRunning());
}

void CViewParaBtn::Close()
{

}

/**=================================================================================================
 * \fn void CViewParaBtn::Show()
 *
 * \brief Shows this object.
 *
 * \author Rainer
 * \date 04 Dez 2015
 *===============================================================================================**/
void CViewParaBtn::Show()
{
	if(m_bSettingsActive)
	{
		m_bViewPraBtnShown=false;
		HideParaBtn();
		Draw();
		ShowParaSettings(true);
		if(m_pcParaMenu)
			m_pcParaMenu->ShowWindow(SW_HIDE);

		m_bViewPraBtnShown=true;

		switch(getModel()->getVIEWHANDLER()->getViewState())
		{
		case VS_ALARM_LIMIT:
			{
				ShowWndMenuVentState(false);
			}
			break;
		default:
			{
				ShowWndMenuVentState(true);
			}
			break;
		}

		
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->SetFocus();
		//SetViewFocus();
	}
	else
	{
		m_bViewPraBtnShown=false;
		ShowParaSettings(false);
		Draw();

		ShowParaBtn();
		if(m_pcParaMenu)
			m_pcParaMenu->ShowWindow(SW_SHOW);
		if(m_pcParaMenu->IsDepressed())
		{
			m_pcParaMenu->Depress(false);
		}

		SetOxyHourglass(getModel()->getDATAHANDLER()->GetOxyCalRunning());
		m_bViewPraBtnShown=true;

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->SetFocus();
	}
	
}

/**=================================================================================================
 * \fn BOOL CViewParaBtn::PreTranslateMessage(MSG* pMsg)
 *
 * \brief Translates all messages before they are processed by the main message loop.
 *
 * \author Rainer
 * \date 04 Dez 2015
 *
 * \param pMsg The message.
 *
 * \return true if it succeeds, false if it fails.
 *===============================================================================================**/
BOOL CViewParaBtn::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message)
	{
	case WM_KEYDOWN:
		{
			if(pMsg->wParam==VK_SPACE)
			{
				SetTimer(PARATIMER, TIME_PARATIMER, NULL);
				if(getModel()->getVIEWHANDLER()->getViewState()==VS_ALARM_LIMIT)
				{
					getModel()->getVIEWHANDLER()->SetALIMITTIMER();
				}
			}
			else if(pMsg->wParam==VK_UP)
			{
				SetTimer(PARATIMER, TIME_PARATIMER, NULL);
				if(getModel()->getVIEWHANDLER()->getViewState()==VS_ALARM_LIMIT)
				{
					getModel()->getVIEWHANDLER()->SetALIMITTIMER();
				}
			}
			else if(pMsg->wParam==VK_DOWN)
			{
				SetTimer(PARATIMER, TIME_PARATIMER, NULL);
				if(getModel()->getVIEWHANDLER()->getViewState()==VS_ALARM_LIMIT)
				{
					getModel()->getVIEWHANDLER()->SetALIMITTIMER();
				}
			}
			
			break;
		}
		break;
	default:
		break;
	}
	return CWnd::PreTranslateMessage(pMsg);
}

/**=================================================================================================
 * \fn bool CViewParaBtn::SetNextFocus()
 *
 * \brief Sets next focus of parameter buttons.
 *
 * \author Rainer
 * \date 04 Dez 2015
 *
 * \return true if it succeeds, false if it fails.
 *===============================================================================================**/
bool CViewParaBtn::SetNextFocus()
{
	if(m_bSettingsActive)
	{
		m_pcWndParaSettings->SetNextFocus();
		return true;
	}
	
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	
	CWnd* pFocWnd = CWnd::GetFocus();
	if(pFocWnd==NULL)
	{
		/** \brief The leave critical section. */
		LeaveCriticalSection(&csDraw);
		return false;
	}
	int iID=pFocWnd->GetDlgCtrlID();
	CWnd* pWnd = GetNextDlgTabItem(pFocWnd,FALSE);
	if(pWnd==NULL)
	{
		/** \brief The leave critical section. */
		LeaveCriticalSection(&csDraw);
		return false;
	}

	iID=pWnd->GetDlgCtrlID();

	if(		pFocWnd->GetDlgCtrlID()>pWnd->GetDlgCtrlID()
		||	IDC_BTN_PARA_VG_VL<pWnd->GetDlgCtrlID())
	{
		/** \brief The leave critical section. */
		LeaveCriticalSection(&csDraw);
		return false;
	}

	iID=pWnd->GetDlgCtrlID();
	pFocWnd->CheckDlgButton(pFocWnd->GetDlgCtrlID(),BST_UNCHECKED);
	
	pWnd->SetFocus();
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);

	getModel()->getCONFIG()->SetLastSelectedParaViewBtns(iID);

	return true;
}

/**=================================================================================================
 * \fn bool CViewParaBtn::SetPrevFocus()
 *
 * \brief Sets previous focus to parameter buttons.
 *
 * \author Rainer
 * \date 04 Dez 2015
 *
 * \return true if it succeeds, false if it fails.
 *===============================================================================================**/
bool CViewParaBtn::SetPrevFocus()
{
	if(m_bSettingsActive)
	{
		m_pcWndParaSettings->SetPrevFocus();
		return true;
	}
	
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);

	CWnd* pFocWnd = CWnd::GetFocus();
	if(pFocWnd==NULL)
	{
		/** \brief The leave critical section. */
		LeaveCriticalSection(&csDraw);
		return false;
	}
	
	CWnd* pWnd = GetNextDlgTabItem(pFocWnd,TRUE);
	if(pWnd==NULL)
	{
		/** \brief The leave critical section. */
		LeaveCriticalSection(&csDraw);
		return false;
	}

	if(pFocWnd->GetDlgCtrlID()<pWnd->GetDlgCtrlID())
	{
		/** \brief The leave critical section. */
		LeaveCriticalSection(&csDraw);
		return false;
	}

	int iID=pWnd->GetDlgCtrlID();
	pFocWnd->CheckDlgButton(pFocWnd->GetDlgCtrlID(),BST_UNCHECKED);
	
	pWnd->SetFocus();

	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);

	getModel()->getCONFIG()->SetLastSelectedParaViewBtns(iID);

	return true;
}

void CViewParaBtn::SetViewFocusWndParaSettings()
{
	switch(getModel()->getCONFIG()->GetLastSelectedParaViewBtns())
	{
	case IDC_BTN_PARA_MANBREATH:
		{
			if(m_pcWndParaSettings)
				m_pcWndParaSettings->SetViewFocus(IDC_BTN_PARA_MANBREATH);
		}
		break;
	case IDC_BTN_PARA_FLUSHTIME:
		{
			if(m_pcWndParaSettings)
				m_pcWndParaSettings->SetViewFocus(IDC_BTN_PARA_FLUSHTIME);
		}
		break;
	case IDC_BTN_PARA_BPM_REC:
		{
			if(m_pcWndParaSettings)
				m_pcWndParaSettings->SetViewFocus(IDC_BTN_PARA_BPM_REC);
		}
		break;
	case IDC_BTN_PARA_ABORTPSV:
		{
			if(m_pcWndParaSettings)
				m_pcWndParaSettings->SetViewFocus(IDC_BTN_PARA_ABORTPSV);
		}
		break;
	case IDC_BTN_PARA_LEAKCOMP:
		{
			if(m_pcWndParaSettings)
				m_pcWndParaSettings->SetViewFocus(IDC_BTN_PARA_LEAKCOMP);
		}
		break;
	case IDC_BTN_PARA_EFLOW:
		{
			if(m_pcWndParaSettings)
				m_pcWndParaSettings->SetViewFocus(IDC_BTN_PARA_EFLOW);
		}
		break;
	case IDC_BTN_PARA_PCURVE:
		{
			if(m_pcWndParaSettings)
				m_pcWndParaSettings->SetViewFocus(IDC_BTN_PARA_PCURVE);
		}
		break;
	default:
		{
			if(m_pcWndParaSettings)
				m_pcWndParaSettings->SetViewFocus(IDC_BTN_PARA_MANBREATH);
		}
		break;
	}
}
/**=================================================================================================
 * \fn void CViewParaBtn::SetViewFocus()
 *
 * \brief Sets view focus.
 *
 * \author Rainer
 * \date 07 Dez 2015
 *===============================================================================================**/
void CViewParaBtn::SetViewFocus()
{
	if(m_bSettingsActive)
	{
		SetViewFocusWndParaSettings();
		return;
	}

	bool bPreviousFocus=false;
	int iCtrlID=0;

	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	switch(getModel()->getCONFIG()->GetLastSelectedParaViewBtns())
	{
	case IDC_BTN_PARA_LEAKCOMP:
	case IDC_BTN_PARA_MANBREATH:
	case IDC_BTN_PARA_FLUSHTIME:
	case IDC_BTN_PARA_ABORTPSV:
	case IDC_BTN_PARA_PCURVE:
	case IDC_BTN_PARA_BPM_REC:
		{
			bool bPara_IFLOW=false;
			if(m_pcPara_IFLOW)
			{
				if(m_pcPara_IFLOW->IsWindowVisible())
					bPara_IFLOW=true;
			}
			if(bPara_IFLOW)
			{
				m_pcPara_IFLOW->SetFocus();
				iCtrlID=m_pcPara_IFLOW->GetDlgCtrlID();
			}
			else if(m_pcPara_IFLOW)
			{
				CWnd* pWnd = GetNextDlgTabItem(m_pcPara_IFLOW,FALSE);
				if(pWnd!=NULL)
				{
					iCtrlID=pWnd->GetDlgCtrlID();

					pWnd->SetFocus();
				}
			}
		}
		break;
	case IDC_BTN_PARA_IFLOW:
		{
			if(m_pcPara_IFLOW)
				m_pcPara_IFLOW->SetFocus();
		}
		break;
	case IDC_BTN_PARA_RISETIME:
		{
			if(m_pcPara_RISETIME)
				m_pcPara_RISETIME->SetFocus();
		}
		break;
	/*case IDC_BTN_PARA_PLATEAU:
		{
			if(m_pcPara_Plateau)
				m_pcPara_Plateau->SetFocus();
		}
		break;*/
	/*case IDC_BTN_PARA_EFLOW:
		{
			if(m_pcPara_EFLOW)
				m_pcPara_EFLOW->SetFocus();
		}
		break;*/
	case IDC_BTN_PARA_FLOWMIN:
		{
			if(m_pcPara_FLOWMIN)
				m_pcPara_FLOWMIN->SetFocus();
		}
		break;
	case IDC_BTN_PARA_THERAPYFLOW:
		{
			if(m_pcPara_FLOW)
				m_pcPara_FLOW->SetFocus();
		}
		break;
	case IDC_BTN_PARA_PEEP:
		{
			if(m_pcPara_PEEP)
				m_pcPara_PEEP->SetFocus();
		}
		break;
	case IDC_BTN_PARA_HFAMPL:
		{
			if(m_pcPara_HFAMPL)
				m_pcPara_HFAMPL->SetFocus();
		}
		break;
	case IDC_BTN_PARA_CPAP:
		{
			if(m_pcPara_CPAP)
				m_pcPara_CPAP->SetFocus();
		}
		break;
	case IDC_BTN_PARA_VG_PMAXI:
		{
			if(m_pcPara_PMAXI)
				m_pcPara_PMAXI->SetFocus();
		}
		break;
	case IDC_BTN_PARA_VG_AMPMAXI:
		{
			if(m_pcPara_AMPMAXI)
				m_pcPara_AMPMAXI->SetFocus();
		}
		break;
	case IDC_BTN_PARA_PINSP:
		{
			if(m_pcPara_PINSP)
				m_pcPara_PINSP->SetFocus();
		}
		break;
	case IDC_BTN_PARA_PDUO:
		{
			if(m_pcPara_PDUO)
				m_pcPara_PDUO->SetFocus();
		}
		break;
	case IDC_BTN_PARA_PMANUAL:
		{
			if(m_pcPara_PMANUAL)
				m_pcPara_PMANUAL->SetFocus();
		}
		break;
	case IDC_BTN_PARA_PMITT:
		{
			if(m_pcPara_PMITT)
				m_pcPara_PMITT->SetFocus();
		}
		break;
	case IDC_BTN_PARA_PPSV:
		{
			//TESTVERSION PSV
			if(m_pcPara_PPSV)
				m_pcPara_PPSV->SetFocus();
		}
		break;
	case IDC_BTN_PARA_VG_VL:
		{
			if((getModel()->getCONFIG()->GetCurMode()==VM_PRE_HFO 
				||	getModel()->getCONFIG()->GetCurMode()==VM_HFO)
				&& getModel()->getDATAHANDLER()->isHFOLicenseAvailable()
				&& m_bSettingsActive==false)
			{
				if(m_pcPara_VGARANT)
					m_pcPara_VGARANT->SetFocus();
				else
					bPreviousFocus=true;

			}
			else if(getModel()->getCONFIG()->GetCurMode()!=VM_PRE_HFO 
				&&	getModel()->getCONFIG()->GetCurMode()!=VM_HFO)
			{
				if(getModel()->getDATAHANDLER()->isVLIMITLicenseAvailable() 
					&& getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VLIMIT)
				{
					if(m_pcPara_VLIMIT)
						m_pcPara_VLIMIT->SetFocus();
					else
						bPreviousFocus=true;
				}
				else if(getModel()->getDATAHANDLER()->isVGUARANTLicenseAvailable() 
					&& getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VGARANT)
				{
					if(m_pcPara_VGARANT)
						m_pcPara_VGARANT->SetFocus();
					else
						bPreviousFocus=true;
				}
				else
				{
					bool bPara_IFLOW=false;
					if(m_pcPara_IFLOW)
					{
						if(m_pcPara_IFLOW->IsWindowVisible())
							bPara_IFLOW=true;
					}

					if(bPara_IFLOW)
					{
						m_pcPara_IFLOW->SetFocus();
						iCtrlID=m_pcPara_IFLOW->GetDlgCtrlID();
					}
					else if(m_pcPara_IFLOW)
					{
						CWnd* pWnd = GetNextDlgTabItem(m_pcPara_IFLOW,FALSE);
						if(pWnd!=NULL)
						{
							iCtrlID=pWnd->GetDlgCtrlID();

							pWnd->SetFocus();
						}
					}
				}
			}
		}
		break;
	case IDC_BTN_PARA_TRIGCONV:
		{
			if(m_pcPara_TRIGGERCONV)
				m_pcPara_TRIGGERCONV->SetFocus();
		}
		break;
	case IDC_BTN_PARA_TRIGNMODE:
		{
			if(m_pcPara_TRIGGERNMODE)
				m_pcPara_TRIGGERNMODE->SetFocus();
		}
		break;
	case IDC_BTN_PARA_ITIME:
		{
			if(m_pcPara_ITIME)
				m_pcPara_ITIME->SetFocus();
		}
		break;
	case IDC_BTN_PARA_BACKUP:
		{
			if(m_pcPara_BACKUP)
				m_pcPara_BACKUP->SetFocus();
		}
		break;
	case IDC_BTN_PARA_BPM:
		{
			if(m_pcPara_BPM)
				m_pcPara_BPM->SetFocus();
		}
		break;
	case IDC_BTN_PARA_IERATIO:
		{
			if(m_pcPara_IERatio)
				m_pcPara_IERatio->SetFocus();
		}
		break;
	case IDC_BTN_PARA_HFFREQ:
		{
			if(m_pcPara_HFFREQ)
				m_pcPara_HFFREQ->SetFocus();
		}
		break;
	case IDC_BTN_PARA_ITIME_REC:
		{
			if(m_pcPara_ITIME_REC)
				m_pcPara_ITIME_REC->SetFocus();
		}
		break;
	case IDC_BTN_PARA_PMEAN_REC:
		{
			if(m_pcPara_PMEAN_REC)
				m_pcPara_PMEAN_REC->SetFocus();
		}
		break;
	case IDC_BTN_PARA_HFFLOW:
		{
			if(m_pcPara_HFFLOW)
				m_pcPara_HFFLOW->SetFocus();
		}
		break;
	case IDC_BTN_PARA_ETIME:
		{
			if(m_pcPara_ETIME)
				m_pcPara_ETIME->SetFocus();
		}
		break;
	case IDC_BTN_PARA_O2:
		{
			if( m_pcPara_O2)
				m_pcPara_O2->SetFocus();
		}
		break;
	case IDC_BTN_PARA_O2FLUSH:
		{
			if(m_pcPara_O2Flush)
				m_pcPara_O2Flush->SetFocus();
		}
		break;
	default:
		{
			bool bPara_IFLOW=false;
			if(m_pcPara_IFLOW)
			{
				if(m_pcPara_IFLOW->IsWindowVisible())
					bPara_IFLOW=true;
			}

			if(bPara_IFLOW)
			{
				m_pcPara_IFLOW->SetFocus();
				iCtrlID=m_pcPara_IFLOW->GetDlgCtrlID();
			}
			else if(m_pcPara_IFLOW)
			{
				CWnd* pWnd = GetNextDlgTabItem(m_pcPara_IFLOW,FALSE);
				if(pWnd!=NULL)
				{
					iCtrlID=pWnd->GetDlgCtrlID();

					pWnd->SetFocus();
				}
			}
		}
		break;
	}
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);

	if(bPreviousFocus)
	{
		SetPrevFocus();
	}
	else if(iCtrlID!=0)
	{
		getModel()->getCONFIG()->SetLastSelectedParaViewBtns(iCtrlID);
	}

	SetTimer(PARATIMER, TIME_PARATIMER, NULL);
	if(getModel()->getVIEWHANDLER()->getViewState()==VS_ALARM_LIMIT)
	{
		getModel()->getVIEWHANDLER()->SetALIMITTIMER();
	}
}

// **************************************************************************
// 
// **************************************************************************
bool CViewParaBtn::CreateWndMenuVentState()
{
	// **********************************************************************
	// Init. line diagram
	if(m_pWndMenuVentState==NULL && m_lX>-1)
	{
		m_pWndMenuVentState = new CWndMenuVentState(this);
		//RECT rcLd={0,504,800,600};
		RECT rcLd={0,500,610,600};

		//RECT rcLd={477,0,640,435};
		if(m_pWndMenuVentState->Create(AfxGetApp()->GetMainWnd(),rcLd,IDC_VIEW_VENTMENU))
		{
			m_pWndMenuVentState->Init();

			return true;
		}
	}
	return false;
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::ShowWndMenuVentState(bool bShow)
{
	if(m_pWndMenuVentState)
	{
		m_pWndMenuVentState->Show(bShow);
	}
}

// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::ShowParaSettings(bool bState)
{
	if(bState)
	{
		CreateWndParaSettings();
		ShowWndParaSettings();
	}
	else
	{
		DestroyWndParaSettings();
	}
}

bool CViewParaBtn::CreateWndParaSettings()
{
	if(m_pcWndParaSettings==NULL)
	{
		m_pcWndParaSettings = new CWndParaSettings();

		RECT rcLd={0,20,590,250};
		//RECT rcLd={0,20,590,250};
		if(m_pcWndParaSettings->Create(this,rcLd,IDC_VIEW_PARA_SETTINGS))
		{
			return true;
		}
	}
	return false;
}

void CViewParaBtn::ShowWndParaSettings()
{
	if(m_pcWndParaSettings)
		m_pcWndParaSettings->Show(true);
}
void CViewParaBtn::DestroyWndParaSettings()
{
	try
	{
		if(m_pcWndParaSettings)
		{
			m_pcWndParaSettings->DestroyWindow();
		}
	}
	catch (...)
	{
		theApp.ReportException(_T("CViewParaBtn::DestroyWndParaSettings"));

		delete m_pcWndParaSettings;
		m_pcWndParaSettings=NULL;
	}

	if(m_pcWndParaSettings)
        delete m_pcWndParaSettings;
	m_pcWndParaSettings=NULL;

}

// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::Draw()
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
	HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf8AcuBold);
	int tc=SetTextColor(hdcMem,0x0ffffff);

	CBrush cbrOrange(RGB(255,200,0));

	Rectangle(hdcMem, 0, 0, m_lX, m_lY);

	if((getModel()->getCONFIG()->GetCurMode()==VM_PRE_HFO 
		||	getModel()->getCONFIG()->GetCurMode()==VM_HFO)
		&& getModel()->getDATAHANDLER()->isHFOLicenseAvailable()
		&& m_bSettingsActive==false)
	{
		SelectObject(hdcMem,cbrOrange);

		if(getModel()->getDATAHANDLER()->isLUNGRECLicenseAvailable())
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(m_pcLungRec)
				m_pcLungRec->Draw(hdcMem,50,15);

			rc.left = 55;  
			rc.top = 120;  
			rc.right  = 140;  
			rc.bottom = 165;
			DrawText(hdcMem,getModel()->GetLanguageString(IDS_TXT_LUNGREC),-1,&rc,DT_CENTER|DT_WORDBREAK|DT_TOP);
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);
		}
	}

	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,hdcMem,0,0,SRCCOPY);
	BitBlt(m_hDC,0,0,m_lX,m_lY,hdcMem,0,0,SRCCOPY);

	SetTextColor(hdcMem,tc);
	SetBkMode(hdcMem,nBkMode);

	SelectObject(hdcMem,hbrprev);
	SelectObject(hdcMem,hpenprev);

	//penLine.DeleteObject();

	SelectObject(hdcMem,hBmpMemPrev);
	SelectObject(hdcMem,hPrevFont);
	DeleteObject(hBmpMem);
	DeleteDC(hdcMem);
}

// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::ShowParaBtn(bool bVentModeChanged)
{
	if(m_bExit)
		return;

	switch(getModel()->getVIEWHANDLER()->getViewState())
	{
	case VS_ALARM_LIMIT:
		{
			ShowWndMenuVentState(false);
		}
		break;
	default:
		{
			ShowWndMenuVentState(true);
		}
		break;
	}

	if(m_bSettingsActive)
	{
		m_bSettingsActive=false;
		ShowParaSettings(false);
		m_pcParaMenu->ShowWindow(SW_SHOW);
		if(m_pcParaMenu->IsDepressed())
		{
			m_pcParaMenu->Depress(false);
		}

		SetOxyHourglass(getModel()->getDATAHANDLER()->GetOxyCalRunning());
	}


	//DEBUGMSG(TRUE, (TEXT("EnterCriticalSection(&csDraw)\r\n")));
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(		(getModel()->getCONFIG()->GetCurMode()==VM_PRE_HFO 
		||	getModel()->getCONFIG()->GetCurMode()==VM_HFO)
		&&	getModel()->getDATAHANDLER()->isLUNGRECLicenseAvailable()
		&&	getModel()->getDATAHANDLER()->isHFOLicenseAvailable()
		&&	m_bSettingsActive==false)
	{
		if(getModel()->getDATAHANDLER()->isVGUARANTLicenseAvailable())
		{
			if(m_pcPara_OnOff)
				m_pcPara_OnOff->ShowWindow(SW_SHOW);

			if(getModel()->getDATAHANDLER()->IsCurrentModeVGarantStateOn()==true)
				m_pcPara_OnOff->Depress(true);
			else
				m_pcPara_OnOff->Depress(false);
		}
		else
		{
			if(m_pcPara_OnOff)
				m_pcPara_OnOff->ShowWindow(SW_HIDE);
		}
	}
	else
	{
		if(m_pcPara_ITIME_REC)
			m_pcPara_ITIME_REC->ShowWindow(SW_HIDE);
		if(m_pcPara_PMEAN_REC)
			m_pcPara_PMEAN_REC->ShowWindow(SW_HIDE);

		if(m_bSettingsActive==false)
		{
			if(getModel()->getVMODEHANDLER()->curModeIsVolumeSupportMode())
			{
				if(getModel()->getDATAHANDLER()->isVGUARANTLicenseAvailable()
					|| getModel()->getDATAHANDLER()->isVLIMITLicenseAvailable())
				{
					if(m_pcPara_OnOff)
						m_pcPara_OnOff->ShowWindow(SW_SHOW);

					if(getModel()->getCONFIG()->GetCurMode()==VM_PRE_HFO 
						||	getModel()->getCONFIG()->GetCurMode()==VM_HFO)
					{
						if(getModel()->getDATAHANDLER()->IsCurrentModeVGarantStateOn()==true)
							m_pcPara_OnOff->Depress(true);
						else
							m_pcPara_OnOff->Depress(false);
					}
					else if(		getModel()->getDATAHANDLER()->isVGUARANTLicenseAvailable() 
						&&	getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VGARANT
						&&	m_pcPara_VGARANT)
					{
						if(getModel()->getDATAHANDLER()->IsCurrentModeVGarantStateOn()==true)
							m_pcPara_OnOff->Depress(true);
						else
							m_pcPara_OnOff->Depress(false);
					}
					else if(		getModel()->getDATAHANDLER()->isVLIMITLicenseAvailable() 
						&&	getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VLIMIT
						&&	m_pcPara_VLIMIT)
					{
						if(getModel()->getDATAHANDLER()->IsCurrentModeVLimitStateOn()==true)
							m_pcPara_OnOff->Depress(true);
						else
							m_pcPara_OnOff->Depress(false);
					}
					else
					{
						m_pcPara_OnOff->Depress(false);
					}
				}
				else
				{
					if(m_pcPara_OnOff)
						m_pcPara_OnOff->ShowWindow(SW_HIDE);
				}
			}
			else
			{
				if(m_pcPara_OnOff)
					m_pcPara_OnOff->ShowWindow(SW_HIDE);
			}	
		}
		else
		{
			if(m_pcPara_OnOff)
				m_pcPara_OnOff->ShowWindow(SW_HIDE);
		}
	}

	

	fVALUE fv;

	if(m_pcPara_PPSV)
	{
		m_pcPara_PPSV->ClearArrows();
	}
	if(m_pcPara_PEEP)
	{
		m_pcPara_PEEP->ClearArrows();
	}
	if(m_pcPara_CPAP)
	{
		m_pcPara_CPAP->ClearArrows();
	}
	if(m_pcPara_PMANUAL)
	{
		m_pcPara_PMANUAL->ClearArrows();
	}
	if(m_pcPara_PDUO)
	{
		m_pcPara_PDUO->ClearArrows();
	}
	if(m_pcPara_PINSP)
	{
		m_pcPara_PINSP->ClearArrows();
	}
	if(m_pcPara_PMAXI)
	{
		m_pcPara_PMAXI->ClearArrows();
	}
	if(m_pcPara_AMPMAXI)
	{
		m_pcPara_AMPMAXI->ClearArrows();
	}
	if(m_pcPara_HFAMPL)
	{
		m_pcPara_HFAMPL->ClearArrows();
	}
	if(m_pcPara_ITIME_REC)
	{
		m_pcPara_ITIME_REC->ClearArrows();
	}

	if(		getModel()->getALARMHANDLER()->getAlarmSilentState()!=ASTATE_SILENT
		&&	getModel()->getALARMHANDLER()->getAlarmSilentState()!=ASTATE_SYSTEM_SILENT)
	{
		bool bSetIFlowArrow=false;
		if(getModel()->getALARMHANDLER()->ALARM_SysLimit_Pinsp_NotReached->getAlarmState()!=AS_NONE)
		{
			bSetIFlowArrow=true;
		}

		if(getModel()->getALARMHANDLER()->ALARM_SysLimit_Vlimitted->getAlarmState()!=AS_NONE)
		{
			if(m_pcPara_PINSP)
				m_pcPara_PINSP->SetAlarmArrowDown();
		}
		else
		{
			if(m_pcPara_PINSP)
				m_pcPara_PINSP->SetAlarmArrowDown(false);
		}

		if(getModel()->getALARMHANDLER()->ALARM_SysLimit_Vgaranty->getAlarmState()!=AS_NONE)
		{
			if(getModel()->getVGarantyAlarmFlag()==VGAL_LOW)
			{
				int iState=getModel()->getDATAHANDLER()->getStatus2();

				if(iState&BIT6)
				{
					bSetIFlowArrow=true;
					if(m_pcPara_PMAXI)
						m_pcPara_PMAXI->SetAlarmArrowUp(false);
					if(m_pcPara_AMPMAXI)
						m_pcPara_AMPMAXI->SetAlarmArrowUp(false);
				}
				else
				{
					int iUpperLimit=0;
					int iValue=0;
					if(getModel()->getCONFIG()->GetCurMode()==VM_PRE_HFO 
						||	getModel()->getCONFIG()->GetCurMode()==VM_HFO)
					{
						iUpperLimit=getModel()->getDATAHANDLER()->GetCurrentHFAMPLmaxPara();
						iValue=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLmaxPara();

						if(iValue<iUpperLimit)
						{
							if(m_pcPara_AMPMAXI)
								m_pcPara_AMPMAXI->SetAlarmArrowUp();
						}
						else
						{
							if(m_pcPara_AMPMAXI)
								m_pcPara_AMPMAXI->SetAlarmArrowUp(false);
						}
					}
					else
					{
						if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
						{
							iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPMAXMaxPara_IPPV();
							
						}
						else
						{
							iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPMAXMaxPara_TRIGGER();
						}		
						iValue=getModel()->getDATAHANDLER()->GetCurrentPmaxVolGPara();

						if(iValue<iUpperLimit)
						{
							if(m_pcPara_PMAXI)
								m_pcPara_PMAXI->SetAlarmArrowUp();
						}
						else
						{
							if(m_pcPara_PMAXI)
								m_pcPara_PMAXI->SetAlarmArrowUp(false);
						}
					}

					
				}
			}
		}
		else
		{
			if(m_pcPara_PMAXI)
				m_pcPara_PMAXI->SetAlarmArrowUp(false);
			if(m_pcPara_AMPMAXI)
				m_pcPara_AMPMAXI->SetAlarmArrowUp(false);
			
		}

		if(bSetIFlowArrow)
		{
			int iUpperLimit=0;
			if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
			{
				iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetIFlowMaxPara_IPPV();

			}
			else
			{
				iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetIFlowMaxPara_TRIGGER();
			}		
			int iValue=getModel()->getDATAHANDLER()->GetCurrentIFlowPara();
			if(m_pcPara_IFLOW)
				m_pcPara_IFLOW->SetAlarmArrowUp(true);
		}
		else
		{
			if(m_pcPara_IFLOW)
				m_pcPara_IFLOW->SetAlarmArrowUp(false);
		}
	}

	eVentMode curMode = getModel()->getCONFIG()->GetCurMode();

	if(		getModel()->getDATAHANDLER()->IsCurrentModeVGarantStateOn()==true 
		&&	getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false)
	{
		if(getModel()->getCONFIG()->GetCurMode()==VM_PRE_HFO 
			||	getModel()->getCONFIG()->GetCurMode()==VM_HFO)
		{
			if(m_pcPara_AMPMAXI)
				m_pcPara_AMPMAXI->SetBitmaps(m_pcPara_SigT,m_pcPara_DwT,m_pcPara_SigT,m_pcPara_FcT,true,false);
		}
		else
		{
			if(m_pcPara_PMAXI)
				m_pcPara_PMAXI->SetBitmaps(m_pcPara_SigT,m_pcPara_DwT,m_pcPara_SigT,m_pcPara_FcT,true,false);
			
		}
		if(m_pcPara_VGARANT)
			m_pcPara_VGARANT->SetBitmaps(m_pcPara_UpB,m_pcPara_DwB,m_pcPara_UpB,m_pcPara_FcB,false,false);
	
	}
	else
	{
		if(getModel()->getCONFIG()->GetCurMode()==VM_PRE_HFO 
			||	getModel()->getCONFIG()->GetCurMode()==VM_HFO)
		{
			if(m_pcPara_AMPMAXI)
				m_pcPara_AMPMAXI->SetBitmaps(m_pcPara_DisT,m_pcPara_DwT,m_pcPara_UpT,m_pcPara_FcT,false,false);
		}
		else
		{
			if(m_pcPara_PMAXI)
				m_pcPara_PMAXI->SetBitmaps(m_pcPara_DisT,m_pcPara_DwT,m_pcPara_UpT,m_pcPara_FcT,false,false);
		}
		
		if(m_pcPara_VGARANT)
			m_pcPara_VGARANT->SetBitmaps(m_pcPara_DisB,m_pcPara_DwB,m_pcPara_DisB,m_pcPara_FcB,false,false);
	}

	if(		getModel()->getDATAHANDLER()->IsCurrentModeVLimitStateOn()==true 
		&&	getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false)
	{
		if(m_pcPara_VLIMIT)
			m_pcPara_VLIMIT->SetBitmaps(m_pcPara_UpB,m_pcPara_DwB,m_pcPara_UpB,m_pcPara_FcB,false,false);
	}
	else
	{
		if(m_pcPara_VLIMIT)
			m_pcPara_VLIMIT->SetBitmaps(m_pcPara_DisB,m_pcPara_DwB,m_pcPara_DisB,m_pcPara_FcB,false,false);
	}

	if(m_pcSubPara_O2Flush)
	{
		fv.iValue=getModel()->getDATAHANDLER()->GetCurO2FlushTime();
		m_pcSubPara_O2Flush->SetValue(fv);
	}

	
	if(getModel()->isO2FlushActive())//PRICO04
	{
		if(m_pcPara_O2)
			m_pcPara_O2->EnableWindow(FALSE);

		if(m_pcPara_O2Flush)
			m_pcPara_O2Flush->EnableWindow(TRUE);
	}
	else if(getModel()->getPRICOThread())
	{
		if(getModel()->getPRICOThread()->isPRICOalgorithmRunning())
		{
			if(m_pcPara_O2)
				m_pcPara_O2->EnableWindow(TRUE);

			if(m_pcPara_O2Flush)
				m_pcPara_O2Flush->EnableWindow(TRUE);

			if(getModel()->getDATAHANDLER()->GetCurO2FlushTime()==0)
			{
				if(m_pcPara_O2Flush)
				{
					m_pcPara_O2Flush->SetOffState(true);
				}
			}
			else
			{
				if(m_pcPara_O2Flush)
				{
					m_pcPara_O2Flush->SetOffState(false);
				}
			}

			if(m_pcPara_O2)
				m_pcPara_O2->SetBitmaps(m_pcPara_DisB,m_pcPara_DwB,m_pcPara_UpB,m_pcPara_FcB,false,false);
		}
		else
		{
			if(getModel()->isO2FlushActive())
			{
				if(m_pcPara_O2)
					m_pcPara_O2->EnableWindow(FALSE);
			}
			else
			{
				if(m_pcPara_O2)
					m_pcPara_O2->EnableWindow(TRUE);
			}
			

			if(getModel()->getDATAHANDLER()->GetCurO2FlushTime()==0)
			{
				if(m_pcPara_O2Flush)
				{
					m_pcPara_O2Flush->SetOffState(true);
					m_pcPara_O2Flush->EnableWindow(FALSE);
				}
			}
			else
			{
				if(m_pcPara_O2Flush)
				{
					m_pcPara_O2Flush->SetOffState(false);
					m_pcPara_O2Flush->EnableWindow(TRUE);
				}
			}
			
			if(m_pcPara_O2)
				m_pcPara_O2->SetBitmaps(m_pcPara_UpB,m_pcPara_DwB,m_pcPara_UpB,m_pcPara_FcB,false,false);
		}
	}
	else
	{
		if(m_pcPara_O2)
			m_pcPara_O2->EnableWindow(TRUE);

		if(getModel()->getDATAHANDLER()->GetCurO2FlushTime()==0)
		{
			if(m_pcPara_O2Flush)
			{
				m_pcPara_O2Flush->SetOffState(true);
				m_pcPara_O2Flush->EnableWindow(FALSE);
			}
		}
		else
		{
			if(m_pcPara_O2Flush)
			{
				m_pcPara_O2Flush->SetOffState(false);
				m_pcPara_O2Flush->EnableWindow(TRUE);
			}
		}
	}

	if(bVentModeChanged && getModel()->getVMODEHANDLER()->curModeIsConventionalMode())//newVG
	{
		if(getModel()->getVMODEHANDLER()->modeIsPSVmode())
		{
			if(m_pcPara_PINSP)
				m_pcPara_PINSP->SetNameNoteText(getModel()->GetLanguageString(IDS_PARA_BACKUP),true);
		}
		else
		{
			if(m_pcPara_PINSP)
				m_pcPara_PINSP->SetNameNoteText(getModel()->GetLanguageString(IDS_PARA_INSP),true);
		}
	}

	//DEBUGMSG(TRUE, (TEXT("LeaveCriticalSection(&csDraw)\r\n")));
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);

	Draw();

	
	switch(curMode)
	{
	case VM_IPPV:
		{
			load_IPPVmode();
			show_IPPVmode();
		}
		break;
	case VM_PRE_IPPV:
		{
			load_PRE_IPPVmode();
			show_PRE_IPPVmode();
		}
		break;
	case VM_PSV:
		{
			load_PSVmode();
			show_PSVmode();
		}
		break;
	case VM_PRE_PSV:
		{
			load_PRE_PSVmode();
			show_PRE_PSVmode();
		}
		break;
	case VM_SIMV:
		{
			load_SIMVmode();
			show_SIMVmode();
		}
		break;
	case VM_PRE_SIMV:
		{
			load_PRE_SIMVmode();
			show_PRE_SIMVmode();
		}
		break;
	case VM_SIMVPSV:
		{
			load_SIMVPSVmode();
			show_SIMVPSVmode();
		}
		break;
	case VM_PRE_SIMVPSV:
		{
			load_PRE_SIMVPSVmode();
			show_PRE_SIMVPSVmode();
		}
		break;
	case VM_SIPPV:
		{
			load_SIPPVmode();
			show_SIPPVmode();
		}
		break;
	case VM_PRE_SIPPV:
		{
			load_PRE_SIPPVmode();
			show_PRE_SIPPVmode();
		}
		break;
	case VM_CPAP:
		{
			load_CPAPmode();
			show_CPAPmode();
		}
		break;
	case VM_PRE_CPAP:
		{
			load_PRE_CPAPmode();
			show_PRE_CPAPmode();
		}
		break;
	case VM_NCPAP:
		{
			load_NCPAPmode();
			show_NCPAPmode();
		}
		break;
	case VM_PRE_NCPAP:
		{
			load_PRE_NCPAPmode();
			show_PRE_NCPAPmode();
		}
		break;
	case VM_THERAPIE:
		{
			load_THERAPYmode();
			show_THERAPYmode();
		}
		break;
	case VM_PRE_THERAPIE:
		{
			load_PRE_THERAPYmode();
			show_PRE_THERAPYmode();
		}
		break;
	case VM_DUOPAP:
		{
			load_DUOPAPmode();
			show_DUOPAPmode();
		}
		break;
	case VM_PRE_DUOPAP:
		{
			load_PRE_DUOPAPmode();
			show_PRE_DUOPAPmode();
		}
		break;
	case VM_HFO:
		{
			load_HFOmode();
			show_HFOmode();
		}
		break;
	case VM_PRE_HFO:
		{
			load_PRE_HFOmode();
			show_PRE_HFOmode();
		}
		break;
	default:
		{
		}
		break;

	}

	
	
}

// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_IPPVmode()
{
	if(getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_IFLOW)
	{
		load_IFLOWpara(false,false);
	}
	else if(getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_LINEAR)
	{
		load_RISETIMEpara(false,false);
	}
	
	load_PEEPpara(false,false);
	load_PMAXpara(false,false);
	load_PINSPpara(false,false);

	if(getModel()->getDATAHANDLER()->isVLIMITLicenseAvailable() && getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VLIMIT && m_pcPara_VLIMIT)
	{
		load_VLIMITpara(false,false);
	}

	load_ETIMEpara(false,false);
	load_BPMpara(false,false);
	load_ITIMEpara(false,false);
	load_FIO2para(false);
	load_FIO2FLUSHpara(false);

	if(		getModel()->getDATAHANDLER()->isVGUARANTLicenseAvailable() 
		&&	getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VGARANT)
	{
		load_VGUARANTpara(false,false);
	}
}

// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::show_IPPVmode()
{
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_ITIME)
		m_pcPara_ITIME->SetBitmaps(m_pcPara_UpB,m_pcPara_DwB,m_pcPara_UpB,m_pcPara_FcB,false,true);

	if(getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_IFLOW)
	{
		if(m_pcPara_RISETIME)
			m_pcPara_RISETIME->ShowWindow(SW_HIDE);
		if(m_pcPara_IFLOW)
			m_pcPara_IFLOW->ShowWindow(SW_SHOW);
	}
	else if(getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_LINEAR)
	{
		if(m_pcPara_IFLOW)
			m_pcPara_IFLOW->ShowWindow(SW_HIDE);
		if(m_pcPara_RISETIME)
			m_pcPara_RISETIME->ShowWindow(SW_SHOW);
	}
	else
	{
		if(m_pcPara_IFLOW)
			m_pcPara_IFLOW->ShowWindow(SW_HIDE);
		if(m_pcPara_RISETIME)
			m_pcPara_RISETIME->ShowWindow(SW_HIDE);
	}

	if(m_pcPara_FLOWMIN)
		m_pcPara_FLOWMIN->ShowWindow(SW_HIDE);
	if(m_pcPara_PEEP)
		m_pcPara_PEEP->ShowWindow(SW_SHOW);
	if(m_pcPara_CPAP)
		m_pcPara_CPAP->ShowWindow(SW_HIDE);
	if(m_pcPara_PINSP)
		m_pcPara_PINSP->ShowWindow(SW_SHOW);
	if(m_pcPara_PDUO)
		m_pcPara_PDUO->ShowWindow(SW_HIDE);
	if(m_pcPara_PMANUAL)
		m_pcPara_PMANUAL->ShowWindow(SW_HIDE);
	if(m_pcPara_PMITT)
		m_pcPara_PMITT->ShowWindow(SW_HIDE);
	if(m_pcPara_PPSV)
		m_pcPara_PPSV->ShowWindow(SW_HIDE);
	if(m_pcPara_HFAMPL)
		m_pcPara_HFAMPL->ShowWindow(SW_HIDE);
	if(m_pcPara_HFFREQ)
		m_pcPara_HFFREQ->ShowWindow(SW_HIDE);
	if(m_pcPara_TRIGGERCONV)
		m_pcPara_TRIGGERCONV->ShowWindow(SW_HIDE);
	if(m_pcPara_TRIGGERNMODE)
		m_pcPara_TRIGGERNMODE->ShowWindow(SW_HIDE);
	if(m_pcPara_ITIME_REC)
		m_pcPara_ITIME_REC->ShowWindow(SW_HIDE);
	if(m_pcPara_PMEAN_REC)
		m_pcPara_PMEAN_REC->ShowWindow(SW_HIDE);
	if(m_pcPara_HFFLOW)
		m_pcPara_HFFLOW->ShowWindow(SW_HIDE);

	if(m_pcSubPara_TrigThreshold)
		m_pcSubPara_TrigThreshold->ShowWindow(SW_HIDE);

	if(m_pcSubPara_LEAKCOMP) 
		m_pcSubPara_LEAKCOMP->ShowWindow(SW_HIDE);

	if(m_pcSubPara_BIASFLOW) 
		m_pcSubPara_BIASFLOW->ShowWindow(SW_HIDE);

	if(m_pcPara_AMPMAXI)
		m_pcPara_AMPMAXI->ShowWindow(SW_HIDE);

	if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==true)
	{
		if(m_pcPara_VLIMIT)
			m_pcPara_VLIMIT->ShowWindow(SW_HIDE);
		if(m_pcPara_VGARANT)
			m_pcPara_VGARANT->ShowWindow(SW_HIDE);
		if(m_pcPara_PMAXI)
			m_pcPara_PMAXI->ShowWindow(SW_HIDE);
		if(m_pcPara_OnOff)
			m_pcPara_OnOff->ShowWindow(SW_HIDE);
	}
	else
	{
		if(		getModel()->getDATAHANDLER()->isVGUARANTLicenseAvailable() 
			&&	getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VGARANT)
		{
			if(m_pcPara_VLIMIT)
				m_pcPara_VLIMIT->ShowWindow(SW_HIDE);
			if(m_pcPara_VGARANT)
				m_pcPara_VGARANT->ShowWindow(SW_SHOW);
			if(m_pcPara_PMAXI)
				m_pcPara_PMAXI->ShowWindow(SW_SHOW);
			if(m_pcPara_OnOff)
				m_pcPara_OnOff->ShowWindow(SW_SHOW);
		}
		else if(		getModel()->getDATAHANDLER()->isVLIMITLicenseAvailable()
			&&	getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VLIMIT)
		{
			if(m_pcPara_VLIMIT)
				m_pcPara_VLIMIT->ShowWindow(SW_SHOW);
			if(m_pcPara_VGARANT)
				m_pcPara_VGARANT->ShowWindow(SW_HIDE);
			if(m_pcPara_PMAXI)
				m_pcPara_PMAXI->ShowWindow(SW_HIDE);
			if(m_pcPara_OnOff)
				m_pcPara_OnOff->ShowWindow(SW_SHOW);
		}
		else
		{
			if(m_pcPara_VLIMIT)
				m_pcPara_VLIMIT->ShowWindow(SW_HIDE);
			if(m_pcPara_VGARANT)
				m_pcPara_VGARANT->ShowWindow(SW_HIDE);
			if(m_pcPara_PMAXI)
				m_pcPara_PMAXI->ShowWindow(SW_HIDE);
			if(m_pcPara_OnOff)
				m_pcPara_OnOff->ShowWindow(SW_HIDE);
		}
	}

	if(getModel()->getCONFIG()->GetIERelationMode()==RM_SET_ItoBPM)
	{
		if(m_pcPara_BPM)
			m_pcPara_BPM->ShowWindow(SW_SHOW);
		if(m_pcPara_ETIME)
			m_pcPara_ETIME->ShowWindow(SW_HIDE);
		if(m_pcSubPara_BPM)
			m_pcSubPara_BPM->ShowWindow(SW_HIDE);
		if(m_pcSubPara_ETime)
		{
			m_pcSubPara_ETime->SetNameText(getModel()->GetLanguageString(IDS_PARA_ETIME));
			m_pcSubPara_ETime->ShowWindow(SW_SHOW);
			m_pcSubPara_ETime->RefreshBtn();
		}
	}
	else
	{
		if(m_pcPara_BPM)
			m_pcPara_BPM->ShowWindow(SW_HIDE);
		if(m_pcPara_ETIME)
			m_pcPara_ETIME->ShowWindow(SW_SHOW);
		if(m_pcSubPara_BPM)
		{
			m_pcSubPara_BPM->SetNameText(getModel()->GetLanguageString(IDS_PARA_FREQ));
			m_pcSubPara_BPM->ShowWindow(SW_SHOW);
			m_pcSubPara_BPM->RefreshBtn();
		}
		if(m_pcSubPara_ETime)
			m_pcSubPara_ETime->ShowWindow(SW_HIDE);
	}
	int iIE = getModel()->getDATAHANDLER()->GetCurrentIERelation100();
	if(iIE<100)
	{
		if(m_pcPara_BPM)
			m_pcPara_BPM->SetWarning(true);
		if(m_pcPara_ETIME)
			m_pcPara_ETIME->SetWarning(true);
		if(m_pcPara_ITIME)
			m_pcPara_ITIME->SetWarning(true);
	}
	else
	{
		if(m_pcPara_BPM)
			m_pcPara_BPM->SetWarning(false);
		if(m_pcPara_ETIME)
			m_pcPara_ETIME->SetWarning(false);
		if(m_pcPara_ITIME)
			m_pcPara_ITIME->SetWarning(false);
	}
	if(m_pcPara_BACKUP)
		m_pcPara_BACKUP->ShowWindow(SW_HIDE);
	if(m_pcPara_ITIME)
		m_pcPara_ITIME->ShowWindow(SW_SHOW);
	if(m_pcPara_IERatio)
		m_pcPara_IERatio->ShowWindow(SW_HIDE);
	if(m_pcSubPara_IE)
		m_pcSubPara_IE->ShowWindow(SW_SHOW);
	if(m_pcSubPara_IE)
		m_pcSubPara_IE->RefreshBtn();
	if(m_pcSubPara_DemandFlow)
		m_pcSubPara_DemandFlow->ShowWindow(SW_HIDE);
	if(m_pcSubPara_PSVTIME)
		m_pcSubPara_PSVTIME->ShowWindow(SW_HIDE);
	if(m_pcSubPara_O2Flush)
		m_pcSubPara_O2Flush->ShowWindow(SW_SHOW);
	if(m_pcSubPara_O2act)
		m_pcSubPara_O2act->ShowWindow(SW_SHOW);
	if(m_pcPara_O2)
		m_pcPara_O2->ShowWindow(SW_SHOW);
	if(m_pcPara_O2Flush)
		m_pcPara_O2Flush->ShowWindow(SW_SHOW);
	if(m_pcPara_FLOW)
		m_pcPara_FLOW->ShowWindow(SW_HIDE);
	
	if(m_pWndMenuVentState)
		m_pWndMenuVentState->ResetButtonPresetstate();

	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}

// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_PRE_IPPVmode()
{
	if(getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_IFLOW)
	{
		load_IFLOWpara(false,true);
	}
	else if(getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_LINEAR)
	{
		load_RISETIMEpara(false,true);
	}
	
	load_PEEPpara(false,true);
	load_PMAXpara(false,true);
	load_PINSPpara(false,true);
	
	if(getModel()->getDATAHANDLER()->isVLIMITLicenseAvailable() && getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VLIMIT && m_pcPara_VLIMIT)
	{
		load_VLIMITpara(false,true);
	}
	
	load_ETIMEpara(false,true);
	load_BPMpara(false,true);
	load_ITIMEpara(false,true);
	load_FIO2para(true);
	load_FIO2FLUSHpara(true);
	
	if(		getModel()->getDATAHANDLER()->isVGUARANTLicenseAvailable() 
		&&	getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VGARANT)
	{
		load_VGUARANTpara(false,true);
	}
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::show_PRE_IPPVmode()
{
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_ITIME)
		m_pcPara_ITIME->SetBitmaps(m_pcPara_UpB,m_pcPara_DwB,m_pcPara_UpB,m_pcPara_FcB,false,true);

	if(getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_IFLOW)
	{
		if(m_pcPara_RISETIME)
			m_pcPara_RISETIME->ShowWindow(SW_HIDE);
		if(m_pcPara_IFLOW)
			m_pcPara_IFLOW->ShowWindow(SW_SHOW);
	}
	else if(getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_LINEAR)
	{
		if(m_pcPara_IFLOW)
			m_pcPara_IFLOW->ShowWindow(SW_HIDE);
		if(m_pcPara_RISETIME)
			m_pcPara_RISETIME->ShowWindow(SW_SHOW);
	}
	else
	{
		if(m_pcPara_IFLOW)
			m_pcPara_IFLOW->ShowWindow(SW_HIDE);
		if(m_pcPara_RISETIME)
			m_pcPara_RISETIME->ShowWindow(SW_HIDE);
	}

	if(m_pcPara_FLOWMIN)
		m_pcPara_FLOWMIN->ShowWindow(SW_HIDE);
	if(m_pcPara_PEEP)
		m_pcPara_PEEP->ShowWindow(SW_SHOW);
	if(m_pcPara_CPAP)
		m_pcPara_CPAP->ShowWindow(SW_HIDE);
	if(m_pcPara_PINSP)
		m_pcPara_PINSP->ShowWindow(SW_SHOW);
	if(m_pcPara_PDUO)
		m_pcPara_PDUO->ShowWindow(SW_HIDE);
	if(m_pcPara_PMANUAL)
		m_pcPara_PMANUAL->ShowWindow(SW_HIDE);
	if(m_pcPara_PMITT)
		m_pcPara_PMITT->ShowWindow(SW_HIDE);
	if(m_pcPara_PPSV)
		m_pcPara_PPSV->ShowWindow(SW_HIDE);
	if(m_pcPara_HFAMPL)
		m_pcPara_HFAMPL->ShowWindow(SW_HIDE);
	if(m_pcPara_HFFREQ)
		m_pcPara_HFFREQ->ShowWindow(SW_HIDE);
	if(m_pcPara_ITIME_REC)
		m_pcPara_ITIME_REC->ShowWindow(SW_HIDE);
	if(m_pcPara_PMEAN_REC)
		m_pcPara_PMEAN_REC->ShowWindow(SW_HIDE);
	if(m_pcPara_HFFLOW)
		m_pcPara_HFFLOW->ShowWindow(SW_HIDE);
	if(m_pcPara_TRIGGERCONV)
		m_pcPara_TRIGGERCONV->ShowWindow(SW_HIDE);
	if(m_pcPara_TRIGGERNMODE)
		m_pcPara_TRIGGERNMODE->ShowWindow(SW_HIDE);
	if(m_pcSubPara_TrigThreshold)
		m_pcSubPara_TrigThreshold->ShowWindow(SW_HIDE);
	if(m_pcSubPara_LEAKCOMP) 
		m_pcSubPara_LEAKCOMP->ShowWindow(SW_HIDE);
	if(m_pcSubPara_BIASFLOW) 
		m_pcSubPara_BIASFLOW->ShowWindow(SW_HIDE);
	if(m_pcPara_AMPMAXI)
		m_pcPara_AMPMAXI->ShowWindow(SW_HIDE);

	if(		getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_MANOFF
		||	getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_AUTOOFF)
	{
		if(m_pcPara_VLIMIT)
			m_pcPara_VLIMIT->ShowWindow(SW_HIDE);
		if(m_pcPara_VGARANT)
			m_pcPara_VGARANT->ShowWindow(SW_HIDE);
		if(m_pcPara_PMAXI)
			m_pcPara_PMAXI->ShowWindow(SW_HIDE);
		if(m_pcPara_OnOff)
			m_pcPara_OnOff->ShowWindow(SW_HIDE);
	}
	else
	{
		if(		getModel()->getDATAHANDLER()->isVGUARANTLicenseAvailable() 
			&&	getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VGARANT)
		{
			if(m_pcPara_VLIMIT)
				m_pcPara_VLIMIT->ShowWindow(SW_HIDE);
			if(m_pcPara_VGARANT)
				m_pcPara_VGARANT->ShowWindow(SW_SHOW);
			if(m_pcPara_PMAXI)
				m_pcPara_PMAXI->ShowWindow(SW_SHOW);
			if(m_pcPara_OnOff)
				m_pcPara_OnOff->ShowWindow(SW_SHOW);
		}
		else if(		getModel()->getDATAHANDLER()->isVLIMITLicenseAvailable()
			&&	getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VLIMIT)
		{
			if(m_pcPara_VLIMIT)
				m_pcPara_VLIMIT->ShowWindow(SW_SHOW);
			if(m_pcPara_VGARANT)
				m_pcPara_VGARANT->ShowWindow(SW_HIDE);
			if(m_pcPara_PMAXI)
				m_pcPara_PMAXI->ShowWindow(SW_HIDE);
			if(m_pcPara_OnOff)
				m_pcPara_OnOff->ShowWindow(SW_SHOW);
		}
		else
		{
			if(m_pcPara_VLIMIT)
				m_pcPara_VLIMIT->ShowWindow(SW_HIDE);
			if(m_pcPara_VGARANT)
				m_pcPara_VGARANT->ShowWindow(SW_HIDE);
			if(m_pcPara_PMAXI)
				m_pcPara_PMAXI->ShowWindow(SW_HIDE);
			if(m_pcPara_OnOff)
				m_pcPara_OnOff->ShowWindow(SW_HIDE);
		}
	}

	if(getModel()->getCONFIG()->GetIERelationMode()==RM_SET_ItoBPM)
	{
		if(m_pcPara_BPM)
			m_pcPara_BPM->ShowWindow(SW_SHOW);
		if(m_pcPara_ETIME)
			m_pcPara_ETIME->ShowWindow(SW_HIDE);
		if(m_pcSubPara_BPM)
			m_pcSubPara_BPM->ShowWindow(SW_HIDE);
		if(m_pcSubPara_ETime)
		{
			m_pcSubPara_ETime->SetNameText(getModel()->GetLanguageString(IDS_PARA_ETIME));
			m_pcSubPara_ETime->ShowWindow(SW_SHOW);
			m_pcSubPara_ETime->RefreshBtn();
		}
	}
	else
	{
		if(m_pcPara_BPM)
			m_pcPara_BPM->ShowWindow(SW_HIDE);
		if(m_pcPara_ETIME)
			m_pcPara_ETIME->ShowWindow(SW_SHOW);
		if(m_pcSubPara_BPM)
		{
			m_pcSubPara_BPM->SetNameText(getModel()->GetLanguageString(IDS_PARA_FREQ));
			m_pcSubPara_BPM->ShowWindow(SW_SHOW);
			m_pcSubPara_BPM->RefreshBtn();
		}
		if(m_pcSubPara_ETime)
			m_pcSubPara_ETime->ShowWindow(SW_HIDE);
	}
	int iIE = getModel()->getDATAHANDLER()->GetCurrentIERelation100();
	if(iIE<100)
	{
		if(m_pcPara_BPM)
			m_pcPara_BPM->SetWarning(true);
		if(m_pcPara_ETIME)
			m_pcPara_ETIME->SetWarning(true);
		if(m_pcPara_ITIME)
			m_pcPara_ITIME->SetWarning(true);
	}
	else
	{
		if(m_pcPara_BPM)
			m_pcPara_BPM->SetWarning(false);
		if(m_pcPara_ETIME)
			m_pcPara_ETIME->SetWarning(false);
		if(m_pcPara_ITIME)
			m_pcPara_ITIME->SetWarning(false);
	}
	if(m_pcPara_BACKUP)
		m_pcPara_BACKUP->ShowWindow(SW_HIDE);
	if(m_pcPara_ITIME)
		m_pcPara_ITIME->ShowWindow(SW_SHOW);
	if(m_pcPara_IERatio)
		m_pcPara_IERatio->ShowWindow(SW_HIDE);
	if(m_pcSubPara_IE)
		m_pcSubPara_IE->ShowWindow(SW_SHOW);
	if(m_pcSubPara_DemandFlow)
		m_pcSubPara_DemandFlow->ShowWindow(SW_HIDE);
	if(m_pcSubPara_IE)
		m_pcSubPara_IE->RefreshBtn();
	if(m_pcSubPara_PSVTIME)
		m_pcSubPara_PSVTIME->ShowWindow(SW_HIDE);
	if(m_pcSubPara_O2Flush)
		m_pcSubPara_O2Flush->ShowWindow(SW_SHOW);
	if(m_pcSubPara_O2act)
		m_pcSubPara_O2act->ShowWindow(SW_SHOW);
	if(m_pcPara_O2)
		m_pcPara_O2->ShowWindow(SW_SHOW);
	if(m_pcPara_O2Flush)
		m_pcPara_O2Flush->ShowWindow(SW_SHOW);
	if(m_pcPara_FLOW)
		m_pcPara_FLOW->ShowWindow(SW_HIDE);
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_SIPPVmode()
{
	if(getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_IFLOW)
	{
		load_IFLOWpara(true,false);
	}
	else if(getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_LINEAR)
	{
		load_RISETIMEpara(true,false);
	}
	
	load_PEEPpara(true,false);
	load_PMAXpara(true,false);
	load_PINSPpara(true,false);

	if(getModel()->getDATAHANDLER()->isVLIMITLicenseAvailable() && getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VLIMIT && m_pcPara_VLIMIT)
	{
		load_VLIMITpara(true,false);
	}
	
	if(		getModel()->getDATAHANDLER()->isVGUARANTLicenseAvailable() 
		&&	getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VGARANT)
	{
		load_VGUARANTpara(true,false);
	}

	load_TRIGGERCONVpara(false);
	load_ETIMEpara(true,false);
	load_BPMpara(true,false);
	load_ITIMEpara(true,false);
	load_FIO2para(false);
	load_FIO2FLUSHpara(false);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::show_SIPPVmode()
{
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_ITIME)
		m_pcPara_ITIME->SetBitmaps(m_pcPara_UpB,m_pcPara_DwB,m_pcPara_UpB,m_pcPara_FcB,false,true);

	if(getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_IFLOW)
	{
		if(m_pcPara_RISETIME)
			m_pcPara_RISETIME->ShowWindow(SW_HIDE);
		if(m_pcPara_IFLOW)
			m_pcPara_IFLOW->ShowWindow(SW_SHOW);
	}
	else if(getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_LINEAR)
	{
		if(m_pcPara_IFLOW)
			m_pcPara_IFLOW->ShowWindow(SW_HIDE);
		if(m_pcPara_RISETIME)
			m_pcPara_RISETIME->ShowWindow(SW_SHOW);
	}
	else
	{
		if(m_pcPara_IFLOW)
			m_pcPara_IFLOW->ShowWindow(SW_HIDE);
		if(m_pcPara_RISETIME)
			m_pcPara_RISETIME->ShowWindow(SW_HIDE);
	}

	if(m_pcPara_FLOWMIN)
		m_pcPara_FLOWMIN->ShowWindow(SW_HIDE);
	if(m_pcPara_PEEP)
		m_pcPara_PEEP->ShowWindow(SW_SHOW);
	if(m_pcPara_CPAP)
		m_pcPara_CPAP->ShowWindow(SW_HIDE);
	if(m_pcPara_PINSP)
		m_pcPara_PINSP->ShowWindow(SW_SHOW);
	if(m_pcPara_PDUO)
		m_pcPara_PDUO->ShowWindow(SW_HIDE);
	if(m_pcPara_PMANUAL)
		m_pcPara_PMANUAL->ShowWindow(SW_HIDE);
	if(m_pcPara_PMITT)
		m_pcPara_PMITT->ShowWindow(SW_HIDE);
	if(m_pcPara_PPSV)
		m_pcPara_PPSV->ShowWindow(SW_HIDE);
	if(m_pcPara_HFAMPL)
		m_pcPara_HFAMPL->ShowWindow(SW_HIDE);
	if(m_pcPara_HFFREQ)
		m_pcPara_HFFREQ->ShowWindow(SW_HIDE);
	if(m_pcPara_ITIME_REC)
		m_pcPara_ITIME_REC->ShowWindow(SW_HIDE);
	if(m_pcPara_PMEAN_REC)
		m_pcPara_PMEAN_REC->ShowWindow(SW_HIDE);
	if(m_pcPara_HFFLOW)
		m_pcPara_HFFLOW->ShowWindow(SW_HIDE);
	if(m_pcPara_TRIGGERCONV)
		m_pcPara_TRIGGERCONV->ShowWindow(SW_SHOW);
	if(m_pcPara_TRIGGERNMODE)
		m_pcPara_TRIGGERNMODE->ShowWindow(SW_HIDE);
	if(m_pcSubPara_TrigThreshold)
		m_pcSubPara_TrigThreshold->ShowWindow(SW_SHOW);
	if(m_pcSubPara_TrigThreshold)
		m_pcSubPara_TrigThreshold->RefreshBtn();
	if(m_pcSubPara_LEAKCOMP) 
		m_pcSubPara_LEAKCOMP->ShowWindow(SW_HIDE);
	if(m_pcSubPara_BIASFLOW) 
		m_pcSubPara_BIASFLOW->ShowWindow(SW_HIDE);
	if(m_pcPara_AMPMAXI)
		m_pcPara_AMPMAXI->ShowWindow(SW_HIDE);

	if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==true)
	{
		if(m_pcPara_VLIMIT)
			m_pcPara_VLIMIT->ShowWindow(SW_HIDE);
		if(m_pcPara_VGARANT)
			m_pcPara_VGARANT->ShowWindow(SW_HIDE);
		if(m_pcPara_PMAXI)
			m_pcPara_PMAXI->ShowWindow(SW_HIDE);
		if(m_pcPara_OnOff)
			m_pcPara_OnOff->ShowWindow(SW_HIDE);
	}
	else
	{
		if(		getModel()->getDATAHANDLER()->isVGUARANTLicenseAvailable() 
			&&	getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VGARANT)
		{
			if(m_pcPara_VLIMIT)
				m_pcPara_VLIMIT->ShowWindow(SW_HIDE);
			if(m_pcPara_VGARANT)
				m_pcPara_VGARANT->ShowWindow(SW_SHOW);
			if(m_pcPara_PMAXI)
				m_pcPara_PMAXI->ShowWindow(SW_SHOW);
			if(m_pcPara_OnOff)
				m_pcPara_OnOff->ShowWindow(SW_SHOW);
		}
		else if(		getModel()->getDATAHANDLER()->isVLIMITLicenseAvailable()
			&&	getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VLIMIT)
		{
			if(m_pcPara_VLIMIT)
				m_pcPara_VLIMIT->ShowWindow(SW_SHOW);
			if(m_pcPara_VGARANT)
				m_pcPara_VGARANT->ShowWindow(SW_HIDE);
			if(m_pcPara_PMAXI)
				m_pcPara_PMAXI->ShowWindow(SW_HIDE);
			if(m_pcPara_OnOff)
				m_pcPara_OnOff->ShowWindow(SW_SHOW);
		}
		else
		{
			if(m_pcPara_VLIMIT)
				m_pcPara_VLIMIT->ShowWindow(SW_HIDE);
			if(m_pcPara_VGARANT)
				m_pcPara_VGARANT->ShowWindow(SW_HIDE);
			if(m_pcPara_PMAXI)
				m_pcPara_PMAXI->ShowWindow(SW_HIDE);
			if(m_pcPara_OnOff)
				m_pcPara_OnOff->ShowWindow(SW_HIDE);
		}
	}

	if(getModel()->getCONFIG()->GetIERelationMode()==RM_SET_ItoBPM)
	{
		if(m_pcPara_BPM)
			m_pcPara_BPM->ShowWindow(SW_SHOW);
		if(m_pcPara_ETIME)
			m_pcPara_ETIME->ShowWindow(SW_HIDE);
		if(m_pcSubPara_BPM)
			m_pcSubPara_BPM->ShowWindow(SW_HIDE);
		if(m_pcSubPara_ETime)
		{
			m_pcSubPara_ETime->SetNameText(getModel()->GetLanguageString(IDS_PARA_ETIME));
			m_pcSubPara_ETime->ShowWindow(SW_SHOW);
			m_pcSubPara_ETime->RefreshBtn();
		}
	}
	else
	{
		if(m_pcPara_BPM)
			m_pcPara_BPM->ShowWindow(SW_HIDE);
		if(m_pcPara_ETIME)
			m_pcPara_ETIME->ShowWindow(SW_SHOW);
		if(m_pcSubPara_BPM)
		{
			m_pcSubPara_BPM->SetNameText(getModel()->GetLanguageString(IDS_PARA_FREQ));
			m_pcSubPara_BPM->ShowWindow(SW_SHOW);
			m_pcSubPara_BPM->RefreshBtn();
		}
		if(m_pcSubPara_ETime)
			m_pcSubPara_ETime->ShowWindow(SW_HIDE);
	}
	int iIE = getModel()->getDATAHANDLER()->GetCurrentIERelation100();
	if(iIE<100)
	{
		if(m_pcPara_BPM)
			m_pcPara_BPM->SetWarning(true);
		if(m_pcPara_ETIME)
			m_pcPara_ETIME->SetWarning(true);
		if(m_pcPara_ITIME)
			m_pcPara_ITIME->SetWarning(true);
	}
	else
	{
		if(m_pcPara_BPM)
			m_pcPara_BPM->SetWarning(false);
		if(m_pcPara_ETIME)
			m_pcPara_ETIME->SetWarning(false);
		if(m_pcPara_ITIME)
			m_pcPara_ITIME->SetWarning(false);
	}
	if(m_pcPara_BACKUP)
		m_pcPara_BACKUP->ShowWindow(SW_HIDE);
	if(m_pcPara_ITIME)
		m_pcPara_ITIME->ShowWindow(SW_SHOW);
	if(m_pcPara_IERatio)
		m_pcPara_IERatio->ShowWindow(SW_HIDE);
	if(m_pcSubPara_IE)
		m_pcSubPara_IE->ShowWindow(SW_SHOW);
	if(m_pcSubPara_IE)
		m_pcSubPara_IE->RefreshBtn();
	if(m_pcSubPara_DemandFlow)
		m_pcSubPara_DemandFlow->ShowWindow(SW_HIDE);
	if(m_pcSubPara_PSVTIME)
		m_pcSubPara_PSVTIME->ShowWindow(SW_HIDE);
	if(m_pcSubPara_O2Flush)
		m_pcSubPara_O2Flush->ShowWindow(SW_SHOW);
	if(m_pcSubPara_O2act)
		m_pcSubPara_O2act->ShowWindow(SW_SHOW);
	if(m_pcPara_O2)
		m_pcPara_O2->ShowWindow(SW_SHOW);
	if(m_pcPara_O2Flush)
		m_pcPara_O2Flush->ShowWindow(SW_SHOW);
	if(m_pcPara_FLOW)
		m_pcPara_FLOW->ShowWindow(SW_HIDE);
	if(m_pWndMenuVentState)
		m_pWndMenuVentState->ResetButtonPresetstate();
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_PRE_SIPPVmode()
{
	if(getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_IFLOW)
	{
		load_IFLOWpara(true,true);
	}
	else if(getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_LINEAR)
	{
		load_RISETIMEpara(true,true);
	}
	
	load_PEEPpara(true,true);
	load_PMAXpara(true,true);
	load_PINSPpara(true,true);
	
	if(getModel()->getDATAHANDLER()->isVLIMITLicenseAvailable() && getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VLIMIT)
	{
		load_VLIMITpara(true,true);
	}

	if(		getModel()->getDATAHANDLER()->isVGUARANTLicenseAvailable() 
		&&	getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VGARANT)
	{
		load_VGUARANTpara(true,true);
	}

	load_TRIGGERCONVpara(true);
	load_ETIMEpara(true,true);
	load_BPMpara(true,true);
	load_ITIMEpara(true,true);
	load_FIO2para(true);
	load_FIO2FLUSHpara(true);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::show_PRE_SIPPVmode()
{
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_ITIME)
		m_pcPara_ITIME->SetBitmaps(m_pcPara_UpB,m_pcPara_DwB,m_pcPara_UpB,m_pcPara_FcB,false,true);

	if(getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_IFLOW)
	{
		if(m_pcPara_RISETIME)
			m_pcPara_RISETIME->ShowWindow(SW_HIDE);
		if(m_pcPara_IFLOW)
			m_pcPara_IFLOW->ShowWindow(SW_SHOW);
	}
	else if(getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_LINEAR)
	{
		if(m_pcPara_IFLOW)
			m_pcPara_IFLOW->ShowWindow(SW_HIDE);
		if(m_pcPara_RISETIME)
			m_pcPara_RISETIME->ShowWindow(SW_SHOW);
	}
	else
	{
		if(m_pcPara_IFLOW)
			m_pcPara_IFLOW->ShowWindow(SW_HIDE);
		if(m_pcPara_RISETIME)
			m_pcPara_RISETIME->ShowWindow(SW_HIDE);
	}

	if(m_pcPara_FLOWMIN)
		m_pcPara_FLOWMIN->ShowWindow(SW_HIDE);
	if(m_pcPara_PEEP)
		m_pcPara_PEEP->ShowWindow(SW_SHOW);
	if(m_pcPara_CPAP)
		m_pcPara_CPAP->ShowWindow(SW_HIDE);
	if(m_pcPara_PINSP)
		m_pcPara_PINSP->ShowWindow(SW_SHOW);
	if(m_pcPara_PDUO)
		m_pcPara_PDUO->ShowWindow(SW_HIDE);
	if(m_pcPara_PMANUAL)
		m_pcPara_PMANUAL->ShowWindow(SW_HIDE);
	if(m_pcPara_PMITT)
		m_pcPara_PMITT->ShowWindow(SW_HIDE);
	if(m_pcPara_PPSV)
		m_pcPara_PPSV->ShowWindow(SW_HIDE);
	if(m_pcPara_HFAMPL)
		m_pcPara_HFAMPL->ShowWindow(SW_HIDE);
	if(m_pcPara_HFFREQ)
		m_pcPara_HFFREQ->ShowWindow(SW_HIDE);
	if(m_pcPara_ITIME_REC)
		m_pcPara_ITIME_REC->ShowWindow(SW_HIDE);
	if(m_pcPara_PMEAN_REC)
		m_pcPara_PMEAN_REC->ShowWindow(SW_HIDE);
	if(m_pcPara_HFFLOW)
		m_pcPara_HFFLOW->ShowWindow(SW_HIDE);
	if(m_pcPara_TRIGGERCONV)
		m_pcPara_TRIGGERCONV->ShowWindow(SW_SHOW);
	if(m_pcPara_TRIGGERNMODE)
		m_pcPara_TRIGGERNMODE->ShowWindow(SW_HIDE);
	if(m_pcSubPara_TrigThreshold)
		m_pcSubPara_TrigThreshold->ShowWindow(SW_SHOW);
	if(m_pcSubPara_TrigThreshold)
		m_pcSubPara_TrigThreshold->RefreshBtn();
	if(m_pcSubPara_LEAKCOMP) 
		m_pcSubPara_LEAKCOMP->ShowWindow(SW_HIDE);
	if(m_pcSubPara_BIASFLOW) 
		m_pcSubPara_BIASFLOW->ShowWindow(SW_HIDE);
	if(m_pcPara_AMPMAXI)
		m_pcPara_AMPMAXI->ShowWindow(SW_HIDE);

	if(		getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_MANOFF
		||	getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_AUTOOFF)
	{
		if(m_pcPara_VLIMIT)
			m_pcPara_VLIMIT->ShowWindow(SW_HIDE);
		if(m_pcPara_VGARANT)
			m_pcPara_VGARANT->ShowWindow(SW_HIDE);
		if(m_pcPara_PMAXI)
			m_pcPara_PMAXI->ShowWindow(SW_HIDE);
		if(m_pcPara_OnOff)
			m_pcPara_OnOff->ShowWindow(SW_HIDE);
	}
	else
	{
		if(		getModel()->getDATAHANDLER()->isVGUARANTLicenseAvailable() 
			&&	getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VGARANT)
		{
			if(m_pcPara_VLIMIT)
				m_pcPara_VLIMIT->ShowWindow(SW_HIDE);
			if(m_pcPara_VGARANT)
				m_pcPara_VGARANT->ShowWindow(SW_SHOW);
			if(m_pcPara_PMAXI)
				m_pcPara_PMAXI->ShowWindow(SW_SHOW);
			if(m_pcPara_OnOff)
				m_pcPara_OnOff->ShowWindow(SW_SHOW);
		}
		else if(		getModel()->getDATAHANDLER()->isVLIMITLicenseAvailable()
			&&	getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VLIMIT)
		{
			if(m_pcPara_VLIMIT)
				m_pcPara_VLIMIT->ShowWindow(SW_SHOW);
			if(m_pcPara_VGARANT)
				m_pcPara_VGARANT->ShowWindow(SW_HIDE);
			if(m_pcPara_PMAXI)
				m_pcPara_PMAXI->ShowWindow(SW_HIDE);
			if(m_pcPara_OnOff)
				m_pcPara_OnOff->ShowWindow(SW_SHOW);
		}
		else
		{
			if(m_pcPara_VLIMIT)
				m_pcPara_VLIMIT->ShowWindow(SW_HIDE);
			if(m_pcPara_VGARANT)
				m_pcPara_VGARANT->ShowWindow(SW_HIDE);
			if(m_pcPara_PMAXI)
				m_pcPara_PMAXI->ShowWindow(SW_HIDE);
			if(m_pcPara_OnOff)
				m_pcPara_OnOff->ShowWindow(SW_HIDE);
		}
	}

	if(getModel()->getCONFIG()->GetIERelationMode()==RM_SET_ItoBPM)
	{
		if(m_pcPara_BPM)
			m_pcPara_BPM->ShowWindow(SW_SHOW);
		if(m_pcPara_ETIME)
			m_pcPara_ETIME->ShowWindow(SW_HIDE);
		if(m_pcSubPara_BPM)
			m_pcSubPara_BPM->ShowWindow(SW_HIDE);
		if(m_pcSubPara_ETime)
		{
			m_pcSubPara_ETime->SetNameText(getModel()->GetLanguageString(IDS_PARA_ETIME));
			m_pcSubPara_ETime->ShowWindow(SW_SHOW);
			m_pcSubPara_ETime->RefreshBtn();
		}
	}
	else
	{
		if(m_pcPara_BPM)
			m_pcPara_BPM->ShowWindow(SW_HIDE);
		if(m_pcPara_ETIME)
			m_pcPara_ETIME->ShowWindow(SW_SHOW);
		if(m_pcSubPara_BPM)
		{
			m_pcSubPara_BPM->SetNameText(getModel()->GetLanguageString(IDS_PARA_FREQ));
			m_pcSubPara_BPM->ShowWindow(SW_SHOW);
			m_pcSubPara_BPM->RefreshBtn();
		}
		if(m_pcSubPara_ETime)
			m_pcSubPara_ETime->ShowWindow(SW_HIDE);
	}
	int iIE = getModel()->getDATAHANDLER()->GetCurrentIERelation100();
	if(iIE<100)
	{
		if(m_pcPara_BPM)
			m_pcPara_BPM->SetWarning(true);
		if(m_pcPara_ETIME)
			m_pcPara_ETIME->SetWarning(true);
		if(m_pcPara_ITIME)
			m_pcPara_ITIME->SetWarning(true);
	}
	else
	{
		if(m_pcPara_BPM)
			m_pcPara_BPM->SetWarning(false);
		if(m_pcPara_ETIME)
			m_pcPara_ETIME->SetWarning(false);
		if(m_pcPara_ITIME)
			m_pcPara_ITIME->SetWarning(false);
	}
	if(m_pcPara_BACKUP)
		m_pcPara_BACKUP->ShowWindow(SW_HIDE);
	if(m_pcPara_ITIME)
		m_pcPara_ITIME->ShowWindow(SW_SHOW);
	if(m_pcPara_IERatio)
		m_pcPara_IERatio->ShowWindow(SW_HIDE);
	if(m_pcSubPara_IE)
		m_pcSubPara_IE->ShowWindow(SW_SHOW);
	if(m_pcSubPara_IE)
		m_pcSubPara_IE->RefreshBtn();
	if(m_pcSubPara_DemandFlow)
		m_pcSubPara_DemandFlow->ShowWindow(SW_HIDE);
	if(m_pcSubPara_PSVTIME)
		m_pcSubPara_PSVTIME->ShowWindow(SW_HIDE);
	if(m_pcSubPara_O2Flush)
		m_pcSubPara_O2Flush->ShowWindow(SW_SHOW);
	if(m_pcSubPara_O2act)
		m_pcSubPara_O2act->ShowWindow(SW_SHOW);
	if(m_pcPara_O2)
		m_pcPara_O2->ShowWindow(SW_SHOW);
	if(m_pcPara_O2Flush)
		m_pcPara_O2Flush->ShowWindow(SW_SHOW);
	if(m_pcPara_FLOW)
		m_pcPara_FLOW->ShowWindow(SW_HIDE);
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_SIMVmode()
{
	if(getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_IFLOW)
	{
		load_IFLOWpara(true,false);
	}
	else if(getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_LINEAR)
	{
		load_RISETIMEpara(true,false);
	}
	
	load_PEEPpara(true,false);
	load_PMAXpara(true,false);
	load_PINSPpara(true,false);
	
	if(getModel()->getDATAHANDLER()->isVLIMITLicenseAvailable() && getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VLIMIT)
	{
		load_VLIMITpara(true,false);
	}

	if(		getModel()->getDATAHANDLER()->isVGUARANTLicenseAvailable() 
		&&	getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VGARANT)
	{
		load_VGUARANTpara(true,false);
	}

	load_TRIGGERCONVpara(false);
	load_ETIMEpara(true,false);
	load_BPMpara(true,false);
	load_ITIMEpara(true,false);
	load_FIO2para(false);
	load_FIO2FLUSHpara(false);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::show_SIMVmode()
{
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_ITIME)
		m_pcPara_ITIME->SetBitmaps(m_pcPara_UpB,m_pcPara_DwB,m_pcPara_UpB,m_pcPara_FcB,false,true);

	if(getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_IFLOW)
	{
		if(m_pcPara_RISETIME)
			m_pcPara_RISETIME->ShowWindow(SW_HIDE);
		if(m_pcPara_IFLOW)
			m_pcPara_IFLOW->ShowWindow(SW_SHOW);
	}
	else if(getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_LINEAR)
	{
		if(m_pcPara_IFLOW)
			m_pcPara_IFLOW->ShowWindow(SW_HIDE);
		if(m_pcPara_RISETIME)
			m_pcPara_RISETIME->ShowWindow(SW_SHOW);
	}
	else
	{
		if(m_pcPara_IFLOW)
			m_pcPara_IFLOW->ShowWindow(SW_HIDE);
		if(m_pcPara_RISETIME)
			m_pcPara_RISETIME->ShowWindow(SW_HIDE);
	}

	if(m_pcPara_FLOWMIN)
		m_pcPara_FLOWMIN->ShowWindow(SW_HIDE);
	if(m_pcPara_PEEP)
		m_pcPara_PEEP->ShowWindow(SW_SHOW);
	if(m_pcPara_CPAP)
		m_pcPara_CPAP->ShowWindow(SW_HIDE);
	if(m_pcPara_PINSP)
		m_pcPara_PINSP->ShowWindow(SW_SHOW);
	if(m_pcPara_PDUO)
		m_pcPara_PDUO->ShowWindow(SW_HIDE);
	if(m_pcPara_PMANUAL)
		m_pcPara_PMANUAL->ShowWindow(SW_HIDE);
	if(m_pcPara_PMITT)
		m_pcPara_PMITT->ShowWindow(SW_HIDE);
	if(m_pcPara_PPSV)
		m_pcPara_PPSV->ShowWindow(SW_HIDE);
	if(m_pcPara_HFAMPL)
		m_pcPara_HFAMPL->ShowWindow(SW_HIDE);
	if(m_pcPara_HFFREQ)
		m_pcPara_HFFREQ->ShowWindow(SW_HIDE);
	if(m_pcPara_ITIME_REC)
		m_pcPara_ITIME_REC->ShowWindow(SW_HIDE);
	if(m_pcPara_PMEAN_REC)
		m_pcPara_PMEAN_REC->ShowWindow(SW_HIDE);
	if(m_pcPara_HFFLOW)
		m_pcPara_HFFLOW->ShowWindow(SW_HIDE);
	if(m_pcPara_TRIGGERCONV)
		m_pcPara_TRIGGERCONV->ShowWindow(SW_SHOW);
	if(m_pcPara_TRIGGERNMODE)
		m_pcPara_TRIGGERNMODE->ShowWindow(SW_HIDE);
	if(m_pcSubPara_TrigThreshold)
		m_pcSubPara_TrigThreshold->ShowWindow(SW_SHOW);
	if(m_pcSubPara_TrigThreshold)
		m_pcSubPara_TrigThreshold->RefreshBtn();
	if(m_pcSubPara_LEAKCOMP) 
		m_pcSubPara_LEAKCOMP->ShowWindow(SW_HIDE);
	if(m_pcSubPara_BIASFLOW) 
		m_pcSubPara_BIASFLOW->ShowWindow(SW_HIDE);
	if(m_pcPara_AMPMAXI)
		m_pcPara_AMPMAXI->ShowWindow(SW_HIDE);

	if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==true)
	{
		if(m_pcPara_VLIMIT)
			m_pcPara_VLIMIT->ShowWindow(SW_HIDE);
		if(m_pcPara_VGARANT)
			m_pcPara_VGARANT->ShowWindow(SW_HIDE);
		if(m_pcPara_PMAXI)
			m_pcPara_PMAXI->ShowWindow(SW_HIDE);
		if(m_pcPara_OnOff)
			m_pcPara_OnOff->ShowWindow(SW_HIDE);
	}
	else
	{
		if(		getModel()->getDATAHANDLER()->isVGUARANTLicenseAvailable() 
			&&	getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VGARANT)
		{
			if(m_pcPara_VLIMIT)
				m_pcPara_VLIMIT->ShowWindow(SW_HIDE);
			if(m_pcPara_VGARANT)
				m_pcPara_VGARANT->ShowWindow(SW_SHOW);
			if(m_pcPara_PMAXI)
				m_pcPara_PMAXI->ShowWindow(SW_SHOW);
			if(m_pcPara_OnOff)
				m_pcPara_OnOff->ShowWindow(SW_SHOW);
		}
		else if(		getModel()->getDATAHANDLER()->isVLIMITLicenseAvailable()
			&&	getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VLIMIT)
		{
			if(m_pcPara_VLIMIT)
				m_pcPara_VLIMIT->ShowWindow(SW_SHOW);
			if(m_pcPara_VGARANT)
				m_pcPara_VGARANT->ShowWindow(SW_HIDE);
			if(m_pcPara_PMAXI)
				m_pcPara_PMAXI->ShowWindow(SW_HIDE);
			if(m_pcPara_OnOff)
				m_pcPara_OnOff->ShowWindow(SW_SHOW);
		}
		else
		{
			if(m_pcPara_VLIMIT)
				m_pcPara_VLIMIT->ShowWindow(SW_HIDE);
			if(m_pcPara_VGARANT)
				m_pcPara_VGARANT->ShowWindow(SW_HIDE);
			if(m_pcPara_PMAXI)
				m_pcPara_PMAXI->ShowWindow(SW_HIDE);
			if(m_pcPara_OnOff)
				m_pcPara_OnOff->ShowWindow(SW_HIDE);
		}
	}

	if(getModel()->getCONFIG()->GetIERelationMode()==RM_SET_ItoBPM)
	{
		if(m_pcPara_BPM)
			m_pcPara_BPM->ShowWindow(SW_SHOW);
		if(m_pcPara_ETIME)
			m_pcPara_ETIME->ShowWindow(SW_HIDE);
		if(m_pcSubPara_BPM)
			m_pcSubPara_BPM->ShowWindow(SW_HIDE);
		if(m_pcSubPara_ETime)
		{
			m_pcSubPara_ETime->SetNameText(getModel()->GetLanguageString(IDS_PARA_ETIME));
			m_pcSubPara_ETime->ShowWindow(SW_SHOW);
			m_pcSubPara_ETime->RefreshBtn();
		}
	}
	else
	{
		if(m_pcPara_BPM)
			m_pcPara_BPM->ShowWindow(SW_HIDE);
		if(m_pcPara_ETIME)
			m_pcPara_ETIME->ShowWindow(SW_SHOW);
		if(m_pcSubPara_BPM)
		{
			m_pcSubPara_BPM->SetNameText(getModel()->GetLanguageString(IDS_PARA_FREQ));
			m_pcSubPara_BPM->ShowWindow(SW_SHOW);
			m_pcSubPara_BPM->RefreshBtn();
		}
		if(m_pcSubPara_ETime)
			m_pcSubPara_ETime->ShowWindow(SW_HIDE);
	}
	int iIE = getModel()->getDATAHANDLER()->GetCurrentIERelation100();
	if(iIE<100)
	{
		if(m_pcPara_BPM)
			m_pcPara_BPM->SetWarning(true);
		if(m_pcPara_ETIME)
			m_pcPara_ETIME->SetWarning(true);
		if(m_pcPara_ITIME)
			m_pcPara_ITIME->SetWarning(true);
	}
	else
	{
		if(m_pcPara_BPM)
			m_pcPara_BPM->SetWarning(false);
		if(m_pcPara_ETIME)
			m_pcPara_ETIME->SetWarning(false);
		if(m_pcPara_ITIME)
			m_pcPara_ITIME->SetWarning(false);
	}
	if(m_pcPara_BACKUP)
		m_pcPara_BACKUP->ShowWindow(SW_HIDE);
	if(m_pcPara_ITIME)
		m_pcPara_ITIME->ShowWindow(SW_SHOW);
	if(m_pcPara_IERatio)
		m_pcPara_IERatio->ShowWindow(SW_HIDE);
	if(m_pcSubPara_IE)
		m_pcSubPara_IE->ShowWindow(SW_SHOW);
	if(m_pcSubPara_IE)
		m_pcSubPara_IE->RefreshBtn();
	if(m_pcSubPara_DemandFlow)
		m_pcSubPara_DemandFlow->ShowWindow(SW_HIDE);
	if(m_pcSubPara_PSVTIME)
		m_pcSubPara_PSVTIME->ShowWindow(SW_HIDE);
	if(m_pcSubPara_O2Flush)
		m_pcSubPara_O2Flush->ShowWindow(SW_SHOW);
	if(m_pcSubPara_O2act)
		m_pcSubPara_O2act->ShowWindow(SW_SHOW);
	if(m_pcPara_O2)
		m_pcPara_O2->ShowWindow(SW_SHOW);
	if(m_pcPara_O2Flush)
		m_pcPara_O2Flush->ShowWindow(SW_SHOW);
	if(m_pcPara_FLOW)
		m_pcPara_FLOW->ShowWindow(SW_HIDE);
	if(m_pWndMenuVentState)
		m_pWndMenuVentState->ResetButtonPresetstate();
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_PRE_SIMVmode()
{
	if(getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_IFLOW)
	{
		load_IFLOWpara(true,true);
	}
	else if(getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_LINEAR)
	{
		load_RISETIMEpara(true,true);
	}
	load_PEEPpara(true,true);
	load_PMAXpara(true,true);
	load_PINSPpara(true,true);
	
	if(getModel()->getDATAHANDLER()->isVLIMITLicenseAvailable() && getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VLIMIT)
	{
		load_VLIMITpara(true,true);
	}

	if(		getModel()->getDATAHANDLER()->isVGUARANTLicenseAvailable() 
		&&	getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VGARANT)
	{
		load_VGUARANTpara(true,true);
	}

	load_TRIGGERCONVpara(true);
	load_ETIMEpara(true,true);
	load_BPMpara(true,true);
	load_ITIMEpara(true,true);
	load_FIO2para(true);
	load_FIO2FLUSHpara(true);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::show_PRE_SIMVmode()
{
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_ITIME)
		m_pcPara_ITIME->SetBitmaps(m_pcPara_UpB,m_pcPara_DwB,m_pcPara_UpB,m_pcPara_FcB,false,true);

	if(getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_IFLOW)
	{
		if(m_pcPara_RISETIME)
			m_pcPara_RISETIME->ShowWindow(SW_HIDE);
		if(m_pcPara_IFLOW)
			m_pcPara_IFLOW->ShowWindow(SW_SHOW);
	}
	else if(getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_LINEAR)
	{
		if(m_pcPara_IFLOW)
			m_pcPara_IFLOW->ShowWindow(SW_HIDE);
		if(m_pcPara_RISETIME)
			m_pcPara_RISETIME->ShowWindow(SW_SHOW);
	}
	else
	{
		if(m_pcPara_IFLOW)
			m_pcPara_IFLOW->ShowWindow(SW_HIDE);
		if(m_pcPara_RISETIME)
			m_pcPara_RISETIME->ShowWindow(SW_HIDE);
	}

	if(m_pcPara_FLOWMIN)
		m_pcPara_FLOWMIN->ShowWindow(SW_HIDE);
	if(m_pcPara_PEEP)
		m_pcPara_PEEP->ShowWindow(SW_SHOW);
	if(m_pcPara_CPAP)
		m_pcPara_CPAP->ShowWindow(SW_HIDE);
	if(m_pcPara_PINSP)
		m_pcPara_PINSP->ShowWindow(SW_SHOW);
	if(m_pcPara_PDUO)
		m_pcPara_PDUO->ShowWindow(SW_HIDE);
	if(m_pcPara_PMANUAL)
		m_pcPara_PMANUAL->ShowWindow(SW_HIDE);
	if(m_pcPara_PMITT)
		m_pcPara_PMITT->ShowWindow(SW_HIDE);
	if(m_pcPara_PPSV)
		m_pcPara_PPSV->ShowWindow(SW_HIDE);
	if(m_pcPara_HFAMPL)
		m_pcPara_HFAMPL->ShowWindow(SW_HIDE);
	if(m_pcPara_HFFREQ)
		m_pcPara_HFFREQ->ShowWindow(SW_HIDE);
	if(m_pcPara_ITIME_REC)
		m_pcPara_ITIME_REC->ShowWindow(SW_HIDE);
	if(m_pcPara_PMEAN_REC)
		m_pcPara_PMEAN_REC->ShowWindow(SW_HIDE);
	if(m_pcPara_HFFLOW)
		m_pcPara_HFFLOW->ShowWindow(SW_HIDE);
	if(m_pcPara_TRIGGERCONV)
		m_pcPara_TRIGGERCONV->ShowWindow(SW_SHOW);
	if(m_pcPara_TRIGGERNMODE)
		m_pcPara_TRIGGERNMODE->ShowWindow(SW_HIDE);
	if(m_pcSubPara_TrigThreshold)
		m_pcSubPara_TrigThreshold->ShowWindow(SW_SHOW);
	if(m_pcSubPara_TrigThreshold)
		m_pcSubPara_TrigThreshold->RefreshBtn();
	if(m_pcSubPara_LEAKCOMP) 
		m_pcSubPara_LEAKCOMP->ShowWindow(SW_HIDE);
	if(m_pcSubPara_BIASFLOW) 
		m_pcSubPara_BIASFLOW->ShowWindow(SW_HIDE);
	if(m_pcPara_AMPMAXI)
		m_pcPara_AMPMAXI->ShowWindow(SW_HIDE);

	if(		getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_MANOFF
		||	getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_AUTOOFF)
	{
		if(m_pcPara_VLIMIT)
			m_pcPara_VLIMIT->ShowWindow(SW_HIDE);
		if(m_pcPara_VGARANT)
			m_pcPara_VGARANT->ShowWindow(SW_HIDE);
		if(m_pcPara_PMAXI)
			m_pcPara_PMAXI->ShowWindow(SW_HIDE);
		if(m_pcPara_OnOff)
			m_pcPara_OnOff->ShowWindow(SW_HIDE);
	}
	else
	{
		if(		getModel()->getDATAHANDLER()->isVGUARANTLicenseAvailable() 
			&&	getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VGARANT)
		{
			if(m_pcPara_VLIMIT)
				m_pcPara_VLIMIT->ShowWindow(SW_HIDE);
			if(m_pcPara_VGARANT)
				m_pcPara_VGARANT->ShowWindow(SW_SHOW);
			if(m_pcPara_PMAXI)
				m_pcPara_PMAXI->ShowWindow(SW_SHOW);
			if(m_pcPara_OnOff)
				m_pcPara_OnOff->ShowWindow(SW_SHOW);
		}
		else if(		getModel()->getDATAHANDLER()->isVLIMITLicenseAvailable()
			&&	getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VLIMIT)
		{
			if(m_pcPara_VLIMIT)
				m_pcPara_VLIMIT->ShowWindow(SW_SHOW);
			if(m_pcPara_VGARANT)
				m_pcPara_VGARANT->ShowWindow(SW_HIDE);
			if(m_pcPara_PMAXI)
				m_pcPara_PMAXI->ShowWindow(SW_HIDE);
			if(m_pcPara_OnOff)
				m_pcPara_OnOff->ShowWindow(SW_SHOW);
		}
		else
		{
			if(m_pcPara_VLIMIT)
				m_pcPara_VLIMIT->ShowWindow(SW_HIDE);
			if(m_pcPara_VGARANT)
				m_pcPara_VGARANT->ShowWindow(SW_HIDE);
			if(m_pcPara_PMAXI)
				m_pcPara_PMAXI->ShowWindow(SW_HIDE);
			if(m_pcPara_OnOff)
				m_pcPara_OnOff->ShowWindow(SW_HIDE);
		}
	}

	if(getModel()->getCONFIG()->GetIERelationMode()==RM_SET_ItoBPM)
	{
		if(m_pcPara_BPM)
			m_pcPara_BPM->ShowWindow(SW_SHOW);
		if(m_pcPara_ETIME)
			m_pcPara_ETIME->ShowWindow(SW_HIDE);
		if(m_pcSubPara_BPM)
			m_pcSubPara_BPM->ShowWindow(SW_HIDE);
		if(m_pcSubPara_ETime)
		{
			m_pcSubPara_ETime->SetNameText(getModel()->GetLanguageString(IDS_PARA_ETIME));
			m_pcSubPara_ETime->ShowWindow(SW_SHOW);
			m_pcSubPara_ETime->RefreshBtn();
		}
	}
	else
	{
		if(m_pcPara_BPM)
			m_pcPara_BPM->ShowWindow(SW_HIDE);
		if(m_pcPara_ETIME)
			m_pcPara_ETIME->ShowWindow(SW_SHOW);
		if(m_pcSubPara_BPM)
		{
			m_pcSubPara_BPM->SetNameText(getModel()->GetLanguageString(IDS_PARA_FREQ));
			m_pcSubPara_BPM->ShowWindow(SW_SHOW);
			m_pcSubPara_BPM->RefreshBtn();
		}
		if(m_pcSubPara_ETime)
			m_pcSubPara_ETime->ShowWindow(SW_HIDE);
	}
	int iIE = getModel()->getDATAHANDLER()->GetCurrentIERelation100();
	if(iIE<100)
	{
		if(m_pcPara_BPM)
			m_pcPara_BPM->SetWarning(true);
		if(m_pcPara_ETIME)
			m_pcPara_ETIME->SetWarning(true);
		if(m_pcPara_ITIME)
			m_pcPara_ITIME->SetWarning(true);
	}
	else
	{
		if(m_pcPara_BPM)
			m_pcPara_BPM->SetWarning(false);
		if(m_pcPara_ETIME)
			m_pcPara_ETIME->SetWarning(false);
		if(m_pcPara_ITIME)
			m_pcPara_ITIME->SetWarning(false);
	}
	if(m_pcPara_BACKUP)
		m_pcPara_BACKUP->ShowWindow(SW_HIDE);
	if(m_pcPara_ITIME)
		m_pcPara_ITIME->ShowWindow(SW_SHOW);
	if(m_pcPara_IERatio)
		m_pcPara_IERatio->ShowWindow(SW_HIDE);
	if(m_pcSubPara_IE)
		m_pcSubPara_IE->ShowWindow(SW_SHOW);
	if(m_pcSubPara_DemandFlow)
		m_pcSubPara_DemandFlow->ShowWindow(SW_HIDE);
	if(m_pcSubPara_IE)
		m_pcSubPara_IE->RefreshBtn();
	if(m_pcSubPara_PSVTIME)
		m_pcSubPara_PSVTIME->ShowWindow(SW_HIDE);
	if(m_pcSubPara_O2Flush)
		m_pcSubPara_O2Flush->ShowWindow(SW_SHOW);
	if(m_pcSubPara_O2act)
		m_pcSubPara_O2act->ShowWindow(SW_SHOW);
	if(m_pcPara_O2)
		m_pcPara_O2->ShowWindow(SW_SHOW);
	if(m_pcPara_O2Flush)
		m_pcPara_O2Flush->ShowWindow(SW_SHOW);
	if(m_pcPara_FLOW)
		m_pcPara_FLOW->ShowWindow(SW_HIDE);
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_PSVmode()
{
	if(getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_IFLOW)
	{
		load_IFLOWpara(true,false);
	}
	else if(getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_LINEAR)
	{
		load_RISETIMEpara(true,false);
	}

	load_PEEPpara(true,false);
	load_PINSPpara(true,false);//newVG
	load_PMAXpara(true,false);
	load_PPSVpara(false,false);
	
	if(getModel()->getDATAHANDLER()->isVLIMITLicenseAvailable() && getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VLIMIT)
	{
		load_VLIMITpara(true,false);
	}

	load_TRIGGERCONVpara(false);
	load_ETIMEpara(true,false);
	load_BPMpara(true,false);
	load_ITIMEpara(true,false);
	load_FIO2para(false);
	load_FIO2FLUSHpara(false);

	if(		getModel()->getDATAHANDLER()->isVGUARANTLicenseAvailable() 
		&&	getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VGARANT)
	{
		load_VGUARANTpara(true,false);
	}
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::show_PSVmode()
{
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_ITIME)
		m_pcPara_ITIME->SetBitmaps(m_pcPara_SigB,m_pcPara_DwB,m_pcPara_SigB,m_pcPara_FcB,true,true);

	if(getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_IFLOW)
	{
		if(m_pcPara_RISETIME)
			m_pcPara_RISETIME->ShowWindow(SW_HIDE);
		if(m_pcPara_IFLOW)
			m_pcPara_IFLOW->ShowWindow(SW_SHOW);
	}
	else if(getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_LINEAR)
	{
		if(m_pcPara_IFLOW)
			m_pcPara_IFLOW->ShowWindow(SW_HIDE);
		if(m_pcPara_RISETIME)
			m_pcPara_RISETIME->ShowWindow(SW_SHOW);
	}
	else
	{
		if(m_pcPara_IFLOW)
			m_pcPara_IFLOW->ShowWindow(SW_HIDE);
		if(m_pcPara_RISETIME)
			m_pcPara_RISETIME->ShowWindow(SW_HIDE);
	}

	if(m_pcPara_FLOWMIN)
		m_pcPara_FLOWMIN->ShowWindow(SW_HIDE);
	if(m_pcPara_PEEP)
		m_pcPara_PEEP->ShowWindow(SW_SHOW);
	if(m_pcPara_CPAP)
		m_pcPara_CPAP->ShowWindow(SW_HIDE);
	if(m_pcPara_PDUO)
		m_pcPara_PDUO->ShowWindow(SW_HIDE);
	if(m_pcPara_PMANUAL)
		m_pcPara_PMANUAL->ShowWindow(SW_HIDE);
	if(m_pcPara_PMITT)
		m_pcPara_PMITT->ShowWindow(SW_HIDE);
	if(m_pcPara_PINSP)
		m_pcPara_PINSP->ShowWindow(SW_SHOW);//newVG
	if(m_pcPara_PPSV)
		m_pcPara_PPSV->ShowWindow(SW_SHOW);
	if(m_pcPara_HFAMPL)
		m_pcPara_HFAMPL->ShowWindow(SW_HIDE);
	if(m_pcPara_HFFREQ)
		m_pcPara_HFFREQ->ShowWindow(SW_HIDE);
	if(m_pcPara_ITIME_REC)
		m_pcPara_ITIME_REC->ShowWindow(SW_HIDE);
	if(m_pcPara_PMEAN_REC)
		m_pcPara_PMEAN_REC->ShowWindow(SW_HIDE);
	if(m_pcPara_HFFLOW)
		m_pcPara_HFFLOW->ShowWindow(SW_HIDE);
	if(m_pcPara_TRIGGERCONV)
		m_pcPara_TRIGGERCONV->ShowWindow(SW_SHOW);
	if(m_pcPara_TRIGGERNMODE)
		m_pcPara_TRIGGERNMODE->ShowWindow(SW_HIDE);
	if(m_pcSubPara_TrigThreshold)
		m_pcSubPara_TrigThreshold->ShowWindow(SW_SHOW);
	if(m_pcSubPara_TrigThreshold)
		m_pcSubPara_TrigThreshold->RefreshBtn();
	if(m_pcSubPara_LEAKCOMP) 
		m_pcSubPara_LEAKCOMP->ShowWindow(SW_HIDE);
	if(m_pcSubPara_BIASFLOW) 
		m_pcSubPara_BIASFLOW->ShowWindow(SW_HIDE);
	if(m_pcPara_AMPMAXI)
		m_pcPara_AMPMAXI->ShowWindow(SW_HIDE);

	if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==true)
	{
		if(m_pcPara_VLIMIT)
			m_pcPara_VLIMIT->ShowWindow(SW_HIDE);
		if(m_pcPara_VGARANT)
			m_pcPara_VGARANT->ShowWindow(SW_HIDE);
		if(m_pcPara_PMAXI)
			m_pcPara_PMAXI->ShowWindow(SW_HIDE);
		if(m_pcPara_OnOff)
			m_pcPara_OnOff->ShowWindow(SW_HIDE);
	}
	else
	{
		if(		getModel()->getDATAHANDLER()->isVGUARANTLicenseAvailable() 
			&&	getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VGARANT)
		{
			if(m_pcPara_VLIMIT)
				m_pcPara_VLIMIT->ShowWindow(SW_HIDE);
			if(m_pcPara_VGARANT)
				m_pcPara_VGARANT->ShowWindow(SW_SHOW);
			if(m_pcPara_PMAXI)
				m_pcPara_PMAXI->ShowWindow(SW_SHOW);
			if(m_pcPara_OnOff)
				m_pcPara_OnOff->ShowWindow(SW_SHOW);
		}
		else if(		getModel()->getDATAHANDLER()->isVLIMITLicenseAvailable()
			&&	getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VLIMIT)
		{
			if(m_pcPara_VLIMIT)
				m_pcPara_VLIMIT->ShowWindow(SW_SHOW);
			if(m_pcPara_VGARANT)
				m_pcPara_VGARANT->ShowWindow(SW_HIDE);
			if(m_pcPara_PMAXI)
				m_pcPara_PMAXI->ShowWindow(SW_HIDE);
			if(m_pcPara_OnOff)
				m_pcPara_OnOff->ShowWindow(SW_SHOW);
		}
		else
		{
			if(m_pcPara_VLIMIT)
				m_pcPara_VLIMIT->ShowWindow(SW_HIDE);
			if(m_pcPara_VGARANT)
				m_pcPara_VGARANT->ShowWindow(SW_HIDE);
			if(m_pcPara_PMAXI)
				m_pcPara_PMAXI->ShowWindow(SW_HIDE);
			if(m_pcPara_OnOff)
				m_pcPara_OnOff->ShowWindow(SW_HIDE);
		}
	}

	if(getModel()->getCONFIG()->GetIERelationMode()==RM_SET_ItoBPM)
	{
		if(m_pcPara_BPM)
			m_pcPara_BPM->ShowWindow(SW_SHOW);
		if(m_pcPara_ETIME)
			m_pcPara_ETIME->ShowWindow(SW_HIDE);
		if(m_pcSubPara_BPM)
			m_pcSubPara_BPM->ShowWindow(SW_HIDE);
		if(m_pcSubPara_ETime)
		{
			m_pcSubPara_ETime->SetNameText(getModel()->GetLanguageString(IDS_PARA_ETIME));
			m_pcSubPara_ETime->ShowWindow(SW_SHOW);
			m_pcSubPara_ETime->RefreshBtn();
		}
	}
	else
	{
		if(m_pcPara_BPM)
			m_pcPara_BPM->ShowWindow(SW_HIDE);
		if(m_pcPara_ETIME)
			m_pcPara_ETIME->ShowWindow(SW_SHOW);
		if(m_pcSubPara_BPM)
		{
			m_pcSubPara_BPM->SetNameText(getModel()->GetLanguageString(IDS_PARA_FREQBACKUP));
			m_pcSubPara_BPM->ShowWindow(SW_SHOW);
			m_pcSubPara_BPM->RefreshBtn();
		}
		if(m_pcSubPara_ETime)
			m_pcSubPara_ETime->ShowWindow(SW_HIDE);
	}
	int iIE = getModel()->getDATAHANDLER()->GetCurrentIERelation100();
	if(iIE<100)
	{
		if(m_pcPara_BPM)
			m_pcPara_BPM->SetWarning(true);
		if(m_pcPara_ETIME)
			m_pcPara_ETIME->SetWarning(true);
		if(m_pcPara_ITIME)
			m_pcPara_ITIME->SetWarning(true);
	}
	else
	{
		if(m_pcPara_BPM)
			m_pcPara_BPM->SetWarning(false);
		if(m_pcPara_ETIME)
			m_pcPara_ETIME->SetWarning(false);
		if(m_pcPara_ITIME)
			m_pcPara_ITIME->SetWarning(false);
	}
	if(m_pcPara_BACKUP)
		m_pcPara_BACKUP->ShowWindow(SW_HIDE);
	if(m_pcPara_ITIME)
		m_pcPara_ITIME->ShowWindow(SW_SHOW);
	if(m_pcPara_IERatio)
		m_pcPara_IERatio->ShowWindow(SW_HIDE);
	if(m_pcSubPara_IE)
		m_pcSubPara_IE->ShowWindow(SW_HIDE);
	if(m_pcSubPara_DemandFlow)
		m_pcSubPara_DemandFlow->ShowWindow(SW_HIDE);
	if(m_pcSubPara_PSVTIME)
		m_pcSubPara_PSVTIME->ShowWindow(SW_SHOW);
	if(m_pcSubPara_O2Flush)
		m_pcSubPara_O2Flush->ShowWindow(SW_SHOW);
	if(m_pcSubPara_O2act)
		m_pcSubPara_O2act->ShowWindow(SW_SHOW);
	if(m_pcPara_O2)
		m_pcPara_O2->ShowWindow(SW_SHOW);
	if(m_pcPara_O2Flush)
		m_pcPara_O2Flush->ShowWindow(SW_SHOW);
	if(m_pcPara_FLOW)
		m_pcPara_FLOW->ShowWindow(SW_HIDE);
	if(m_pWndMenuVentState)
		m_pWndMenuVentState->ResetButtonPresetstate();
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_PRE_PSVmode()
{
	if(getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_IFLOW)
	{
		load_IFLOWpara(true,true);
	}
	else if(getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_LINEAR)
	{
		load_RISETIMEpara(true,true);
	}
	load_PEEPpara(true,true);
	load_PINSPpara(true,true);//newVG
	load_PMAXpara(true,true);
	load_PPSVpara(false,true);

	if(getModel()->getDATAHANDLER()->isVLIMITLicenseAvailable() && getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VLIMIT)
	{
		load_VLIMITpara(true,true);
	}

	load_TRIGGERCONVpara(true);
	load_ETIMEpara(true,true);
	load_BPMpara(true,true);
	load_ITIMEpara(true,true);
	load_FIO2para(true);
	load_FIO2FLUSHpara(true);

	if(		getModel()->getDATAHANDLER()->isVGUARANTLicenseAvailable() 
		&&	getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VGARANT)
	{
		load_VGUARANTpara(true,true);
	}
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::show_PRE_PSVmode()
{
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_ITIME)
		m_pcPara_ITIME->SetBitmaps(m_pcPara_SigB,m_pcPara_DwB,m_pcPara_SigB,m_pcPara_FcB,true,true);

	if(getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_IFLOW)
	{
		if(m_pcPara_RISETIME)
			m_pcPara_RISETIME->ShowWindow(SW_HIDE);
		if(m_pcPara_IFLOW)
			m_pcPara_IFLOW->ShowWindow(SW_SHOW);
	}
	else if(getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_LINEAR)
	{
		if(m_pcPara_IFLOW)
			m_pcPara_IFLOW->ShowWindow(SW_HIDE);
		if(m_pcPara_RISETIME)
			m_pcPara_RISETIME->ShowWindow(SW_SHOW);
	}
	else
	{
		if(m_pcPara_IFLOW)
			m_pcPara_IFLOW->ShowWindow(SW_HIDE);
		if(m_pcPara_RISETIME)
			m_pcPara_RISETIME->ShowWindow(SW_HIDE);
	}

	if(m_pcPara_FLOWMIN)
		m_pcPara_FLOWMIN->ShowWindow(SW_HIDE);
	if(m_pcPara_PEEP)
		m_pcPara_PEEP->ShowWindow(SW_SHOW);
	if(m_pcPara_CPAP)
		m_pcPara_CPAP->ShowWindow(SW_HIDE);
	if(m_pcPara_PDUO)
		m_pcPara_PDUO->ShowWindow(SW_HIDE);
	if(m_pcPara_PMANUAL)
		m_pcPara_PMANUAL->ShowWindow(SW_HIDE);
	if(m_pcPara_PMITT)
		m_pcPara_PMITT->ShowWindow(SW_HIDE);
	if(m_pcPara_PINSP)
		m_pcPara_PINSP->ShowWindow(SW_SHOW);//newVG
	if(m_pcPara_PPSV)
		m_pcPara_PPSV->ShowWindow(SW_SHOW);
	if(m_pcPara_HFAMPL)
		m_pcPara_HFAMPL->ShowWindow(SW_HIDE);
	if(m_pcPara_HFFREQ)
		m_pcPara_HFFREQ->ShowWindow(SW_HIDE);
	if(m_pcPara_ITIME_REC)
		m_pcPara_ITIME_REC->ShowWindow(SW_HIDE);
	if(m_pcPara_PMEAN_REC)
		m_pcPara_PMEAN_REC->ShowWindow(SW_HIDE);
	if(m_pcPara_HFFLOW)
		m_pcPara_HFFLOW->ShowWindow(SW_HIDE);
	if(m_pcPara_TRIGGERCONV)
		m_pcPara_TRIGGERCONV->ShowWindow(SW_SHOW);
	if(m_pcPara_TRIGGERNMODE)
		m_pcPara_TRIGGERNMODE->ShowWindow(SW_HIDE);
	if(m_pcSubPara_TrigThreshold)
		m_pcSubPara_TrigThreshold->ShowWindow(SW_SHOW);
	if(m_pcSubPara_TrigThreshold)
		m_pcSubPara_TrigThreshold->RefreshBtn();
	if(m_pcSubPara_LEAKCOMP) 
		m_pcSubPara_LEAKCOMP->ShowWindow(SW_HIDE);
	if(m_pcSubPara_BIASFLOW) 
		m_pcSubPara_BIASFLOW->ShowWindow(SW_HIDE);
	if(m_pcPara_AMPMAXI)
		m_pcPara_AMPMAXI->ShowWindow(SW_HIDE);

	if(		getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_MANOFF
		||	getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_AUTOOFF)
	{
		if(m_pcPara_VLIMIT)
			m_pcPara_VLIMIT->ShowWindow(SW_HIDE);
		if(m_pcPara_VGARANT)
			m_pcPara_VGARANT->ShowWindow(SW_HIDE);
		if(m_pcPara_PMAXI)
			m_pcPara_PMAXI->ShowWindow(SW_HIDE);
		if(m_pcPara_OnOff)
			m_pcPara_OnOff->ShowWindow(SW_HIDE);
	}
	else
	{
		if(		getModel()->getDATAHANDLER()->isVGUARANTLicenseAvailable() 
			&&	getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VGARANT)
		{
			if(m_pcPara_VLIMIT)
				m_pcPara_VLIMIT->ShowWindow(SW_HIDE);
			if(m_pcPara_VGARANT)
				m_pcPara_VGARANT->ShowWindow(SW_SHOW);
			if(m_pcPara_PMAXI)
				m_pcPara_PMAXI->ShowWindow(SW_SHOW);
			if(m_pcPara_OnOff)
				m_pcPara_OnOff->ShowWindow(SW_SHOW);
		}
		else if(		getModel()->getDATAHANDLER()->isVLIMITLicenseAvailable()
			&&	getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VLIMIT)
		{
			if(m_pcPara_VLIMIT)
				m_pcPara_VLIMIT->ShowWindow(SW_SHOW);
			if(m_pcPara_VGARANT)
				m_pcPara_VGARANT->ShowWindow(SW_HIDE);
			if(m_pcPara_PMAXI)
				m_pcPara_PMAXI->ShowWindow(SW_HIDE);
			if(m_pcPara_OnOff)
				m_pcPara_OnOff->ShowWindow(SW_SHOW);
		}
		else
		{
			if(m_pcPara_VLIMIT)
				m_pcPara_VLIMIT->ShowWindow(SW_HIDE);
			if(m_pcPara_VGARANT)
				m_pcPara_VGARANT->ShowWindow(SW_HIDE);
			if(m_pcPara_PMAXI)
				m_pcPara_PMAXI->ShowWindow(SW_HIDE);
			if(m_pcPara_OnOff)
				m_pcPara_OnOff->ShowWindow(SW_HIDE);
		}
	}

	if(getModel()->getCONFIG()->GetIERelationMode()==RM_SET_ItoBPM)
	{
		if(m_pcPara_BPM)
			m_pcPara_BPM->ShowWindow(SW_SHOW);
		if(m_pcPara_ETIME)
			m_pcPara_ETIME->ShowWindow(SW_HIDE);
		if(m_pcSubPara_BPM)
			m_pcSubPara_BPM->ShowWindow(SW_HIDE);
		if(m_pcSubPara_ETime)
		{
			m_pcSubPara_ETime->SetNameText(getModel()->GetLanguageString(IDS_PARA_ETIME));
			m_pcSubPara_ETime->ShowWindow(SW_SHOW);
			m_pcSubPara_ETime->RefreshBtn();
		}
	}
	else
	{
		if(m_pcPara_BPM)
			m_pcPara_BPM->ShowWindow(SW_HIDE);
		if(m_pcPara_ETIME)
			m_pcPara_ETIME->ShowWindow(SW_SHOW);
		if(m_pcSubPara_BPM)
		{
			m_pcSubPara_BPM->SetNameText(getModel()->GetLanguageString(IDS_PARA_FREQBACKUP));
			m_pcSubPara_BPM->ShowWindow(SW_SHOW);
			m_pcSubPara_BPM->RefreshBtn();
		}
		if(m_pcSubPara_ETime)
			m_pcSubPara_ETime->ShowWindow(SW_HIDE);
	}
	int iIE = getModel()->getDATAHANDLER()->GetCurrentIERelation100();
	if(iIE<100)
	{
		if(m_pcPara_BPM)
			m_pcPara_BPM->SetWarning(true);
		if(m_pcPara_ETIME)
			m_pcPara_ETIME->SetWarning(true);
		if(m_pcPara_ITIME)
			m_pcPara_ITIME->SetWarning(true);
	}
	else
	{
		if(m_pcPara_BPM)
			m_pcPara_BPM->SetWarning(false);
		if(m_pcPara_ETIME)
			m_pcPara_ETIME->SetWarning(false);
		if(m_pcPara_ITIME)
			m_pcPara_ITIME->SetWarning(false);
	}
	if(m_pcPara_BACKUP)
		m_pcPara_BACKUP->ShowWindow(SW_HIDE);
	if(m_pcPara_ITIME)
		m_pcPara_ITIME->ShowWindow(SW_SHOW);
	if(m_pcPara_IERatio)
		m_pcPara_IERatio->ShowWindow(SW_HIDE);
	if(m_pcSubPara_IE)
		m_pcSubPara_IE->ShowWindow(SW_HIDE);
	if(m_pcSubPara_DemandFlow)
		m_pcSubPara_DemandFlow->ShowWindow(SW_HIDE);
	if(m_pcSubPara_PSVTIME)
		m_pcSubPara_PSVTIME->ShowWindow(SW_SHOW);
	if(m_pcSubPara_O2Flush)
		m_pcSubPara_O2Flush->ShowWindow(SW_SHOW);
	if(m_pcSubPara_O2act)
		m_pcSubPara_O2act->ShowWindow(SW_SHOW);
	if(m_pcPara_O2)
		m_pcPara_O2->ShowWindow(SW_SHOW);
	if(m_pcPara_O2Flush)
		m_pcPara_O2Flush->ShowWindow(SW_SHOW);
	if(m_pcPara_FLOW)
		m_pcPara_FLOW->ShowWindow(SW_HIDE);
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_SIMVPSVmode()
{
	if(getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_IFLOW)
	{
		load_IFLOWpara(true,false);
	}
	else if(getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_LINEAR)
	{
		load_RISETIMEpara(true,false);
	}
	load_PEEPpara(true,false);
	load_PMAXpara(true,false);
	load_PINSPpara(true,false);
	load_PPSVpara(true,false);
	
	if(getModel()->getDATAHANDLER()->isVLIMITLicenseAvailable() && getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VLIMIT)
	{
		load_VLIMITpara(true,false);
	}

	load_ETIMEpara(true,false);
	load_TRIGGERCONVpara(false);
	load_BPMpara(true,false);
	load_ITIMEpara(true,false);
	load_FIO2para(false);
	load_FIO2FLUSHpara(false);
	
	if(		getModel()->getDATAHANDLER()->isVGUARANTLicenseAvailable() 
		&&	getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VGARANT)
	{
		load_VGUARANTpara(true,false);
	}
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::show_SIMVPSVmode()
{
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_ITIME)
		m_pcPara_ITIME->SetBitmaps(m_pcPara_SigB,m_pcPara_DwB,m_pcPara_SigB,m_pcPara_FcB,true,true);

	if(getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_IFLOW)
	{
		if(m_pcPara_RISETIME)
			m_pcPara_RISETIME->ShowWindow(SW_HIDE);
		if(m_pcPara_IFLOW)
			m_pcPara_IFLOW->ShowWindow(SW_SHOW);
	}
	else if(getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_LINEAR)
	{
		if(m_pcPara_IFLOW)
			m_pcPara_IFLOW->ShowWindow(SW_HIDE);
		if(m_pcPara_RISETIME)
			m_pcPara_RISETIME->ShowWindow(SW_SHOW);
	}
	else
	{
		if(m_pcPara_IFLOW)
			m_pcPara_IFLOW->ShowWindow(SW_HIDE);
		if(m_pcPara_RISETIME)
			m_pcPara_RISETIME->ShowWindow(SW_HIDE);
	}

	if(m_pcPara_FLOWMIN)
		m_pcPara_FLOWMIN->ShowWindow(SW_HIDE);
	if(m_pcPara_PEEP)
		m_pcPara_PEEP->ShowWindow(SW_SHOW);
	if(m_pcPara_CPAP)
		m_pcPara_CPAP->ShowWindow(SW_HIDE);
	if(m_pcPara_PINSP)
		m_pcPara_PINSP->ShowWindow(SW_SHOW);
	if(m_pcPara_PDUO)
		m_pcPara_PDUO->ShowWindow(SW_HIDE);
	if(m_pcPara_PMANUAL)
		m_pcPara_PMANUAL->ShowWindow(SW_HIDE);
	if(m_pcPara_PMITT)
		m_pcPara_PMITT->ShowWindow(SW_HIDE);
	if(m_pcPara_PPSV)
		m_pcPara_PPSV->ShowWindow(SW_SHOW);
	if(m_pcPara_HFAMPL)
		m_pcPara_HFAMPL->ShowWindow(SW_HIDE);
	if(m_pcPara_HFFREQ)
		m_pcPara_HFFREQ->ShowWindow(SW_HIDE);
	if(m_pcPara_ITIME_REC)
		m_pcPara_ITIME_REC->ShowWindow(SW_HIDE);
	if(m_pcPara_PMEAN_REC)
		m_pcPara_PMEAN_REC->ShowWindow(SW_HIDE);
	if(m_pcPara_HFFLOW)
		m_pcPara_HFFLOW->ShowWindow(SW_HIDE);
	if(m_pcPara_TRIGGERCONV)
		m_pcPara_TRIGGERCONV->ShowWindow(SW_SHOW);
	if(m_pcPara_TRIGGERNMODE)
		m_pcPara_TRIGGERNMODE->ShowWindow(SW_HIDE);
	if(m_pcSubPara_TrigThreshold)
		m_pcSubPara_TrigThreshold->ShowWindow(SW_SHOW);
	if(m_pcSubPara_TrigThreshold)
		m_pcSubPara_TrigThreshold->RefreshBtn();
	if(m_pcSubPara_LEAKCOMP) 
		m_pcSubPara_LEAKCOMP->ShowWindow(SW_HIDE);
	if(m_pcSubPara_BIASFLOW) 
		m_pcSubPara_BIASFLOW->ShowWindow(SW_HIDE);
	if(m_pcPara_AMPMAXI)
		m_pcPara_AMPMAXI->ShowWindow(SW_HIDE);

	if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==true)
	{
		if(m_pcPara_VLIMIT)
			m_pcPara_VLIMIT->ShowWindow(SW_HIDE);
		if(m_pcPara_VGARANT)
			m_pcPara_VGARANT->ShowWindow(SW_HIDE);
		if(m_pcPara_PMAXI)
			m_pcPara_PMAXI->ShowWindow(SW_HIDE);
		if(m_pcPara_OnOff)
			m_pcPara_OnOff->ShowWindow(SW_HIDE);
	}
	else
	{
		if(		getModel()->getDATAHANDLER()->isVGUARANTLicenseAvailable() 
			&&	getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VGARANT)
		{
			if(m_pcPara_VLIMIT)
				m_pcPara_VLIMIT->ShowWindow(SW_HIDE);
			if(m_pcPara_VGARANT)
				m_pcPara_VGARANT->ShowWindow(SW_SHOW);
			if(m_pcPara_PMAXI)
				m_pcPara_PMAXI->ShowWindow(SW_SHOW);
			if(m_pcPara_OnOff)
				m_pcPara_OnOff->ShowWindow(SW_SHOW);
		}
		else if(		getModel()->getDATAHANDLER()->isVLIMITLicenseAvailable()
			&&	getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VLIMIT)
		{
			if(m_pcPara_VLIMIT)
				m_pcPara_VLIMIT->ShowWindow(SW_SHOW);
			if(m_pcPara_VGARANT)
				m_pcPara_VGARANT->ShowWindow(SW_HIDE);
			if(m_pcPara_PMAXI)
				m_pcPara_PMAXI->ShowWindow(SW_HIDE);
			if(m_pcPara_OnOff)
				m_pcPara_OnOff->ShowWindow(SW_SHOW);
		}
		else
		{
			if(m_pcPara_VLIMIT)
				m_pcPara_VLIMIT->ShowWindow(SW_HIDE);
			if(m_pcPara_VGARANT)
				m_pcPara_VGARANT->ShowWindow(SW_HIDE);
			if(m_pcPara_PMAXI)
				m_pcPara_PMAXI->ShowWindow(SW_HIDE);
			if(m_pcPara_OnOff)
				m_pcPara_OnOff->ShowWindow(SW_HIDE);
		}
	}

	if(getModel()->getCONFIG()->GetIERelationMode()==RM_SET_ItoBPM)
	{
		if(m_pcPara_BPM)
			m_pcPara_BPM->ShowWindow(SW_SHOW);
		if(m_pcPara_ETIME)
			m_pcPara_ETIME->ShowWindow(SW_HIDE);
		if(m_pcSubPara_BPM)
			m_pcSubPara_BPM->ShowWindow(SW_HIDE);
		if(m_pcSubPara_ETime)
		{
			m_pcSubPara_ETime->SetNameText(getModel()->GetLanguageString(IDS_PARA_ETIME));
			m_pcSubPara_ETime->ShowWindow(SW_SHOW);
			m_pcSubPara_ETime->RefreshBtn();
		}
	}
	else
	{
		if(m_pcPara_BPM)
			m_pcPara_BPM->ShowWindow(SW_HIDE);
		if(m_pcPara_ETIME)
			m_pcPara_ETIME->ShowWindow(SW_SHOW);
		if(m_pcSubPara_BPM)
		{
			m_pcSubPara_BPM->SetNameText(getModel()->GetLanguageString(IDS_PARA_FREQ));
			m_pcSubPara_BPM->ShowWindow(SW_SHOW);
			m_pcSubPara_BPM->RefreshBtn();
		}
		if(m_pcSubPara_ETime)
			m_pcSubPara_ETime->ShowWindow(SW_HIDE);
	}
	int iIE = getModel()->getDATAHANDLER()->GetCurrentIERelation100();
	if(iIE<100)
	{
		if(m_pcPara_BPM)
			m_pcPara_BPM->SetWarning(true);
		if(m_pcPara_ETIME)
			m_pcPara_ETIME->SetWarning(true);
		if(m_pcPara_ITIME)
			m_pcPara_ITIME->SetWarning(true);
	}
	else
	{
		if(m_pcPara_BPM)
			m_pcPara_BPM->SetWarning(false);
		if(m_pcPara_ETIME)
			m_pcPara_ETIME->SetWarning(false);
		if(m_pcPara_ITIME)
			m_pcPara_ITIME->SetWarning(false);
	}
	if(m_pcPara_BACKUP)
		m_pcPara_BACKUP->ShowWindow(SW_HIDE);
	if(m_pcPara_ITIME)
		m_pcPara_ITIME->ShowWindow(SW_SHOW);
	if(m_pcPara_IERatio)
		m_pcPara_IERatio->ShowWindow(SW_HIDE);
	if(m_pcSubPara_IE)
		m_pcSubPara_IE->ShowWindow(SW_HIDE);
	if(m_pcSubPara_DemandFlow)
		m_pcSubPara_DemandFlow->ShowWindow(SW_HIDE);
	if(m_pcSubPara_PSVTIME)
		m_pcSubPara_PSVTIME->ShowWindow(SW_SHOW);
	if(m_pcSubPara_O2Flush)
		m_pcSubPara_O2Flush->ShowWindow(SW_SHOW);
	if(m_pcSubPara_O2act)
		m_pcSubPara_O2act->ShowWindow(SW_SHOW);
	if(m_pcPara_O2)
		m_pcPara_O2->ShowWindow(SW_SHOW);
	if(m_pcPara_O2Flush)
		m_pcPara_O2Flush->ShowWindow(SW_SHOW);
	if(m_pcPara_FLOW)
		m_pcPara_FLOW->ShowWindow(SW_HIDE);
	if(m_pWndMenuVentState)
		m_pWndMenuVentState->ResetButtonPresetstate();
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_PRE_SIMVPSVmode()
{
	if(getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_IFLOW)
	{
		load_IFLOWpara(true,true);
	}
	else if(getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_LINEAR)
	{
		load_RISETIMEpara(true,true);
	}
	load_PEEPpara(true,true);
	load_PMAXpara(true,true);
	load_PINSPpara(true,true);
	load_PPSVpara(true,true);
	
	if(getModel()->getDATAHANDLER()->isVLIMITLicenseAvailable() && getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VLIMIT && m_pcPara_VLIMIT)
	{
		load_VLIMITpara(true,true);
	}

	load_TRIGGERCONVpara(true);	
	load_ETIMEpara(true,true);
	load_BPMpara(true,true);
	load_ITIMEpara(true,true);
	load_FIO2para(true);
	load_FIO2FLUSHpara(true);
	
	if(		getModel()->getDATAHANDLER()->isVGUARANTLicenseAvailable() 
		&&	getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VGARANT)
	{
		load_VGUARANTpara(true,true);
	}
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::show_PRE_SIMVPSVmode()
{
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_ITIME)
		m_pcPara_ITIME->SetBitmaps(m_pcPara_SigB,m_pcPara_DwB,m_pcPara_SigB,m_pcPara_FcB,true,true);

	if(getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_IFLOW)
	{
		if(m_pcPara_RISETIME)
			m_pcPara_RISETIME->ShowWindow(SW_HIDE);
		if(m_pcPara_IFLOW)
			m_pcPara_IFLOW->ShowWindow(SW_SHOW);
	}
	else if(getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_LINEAR)
	{
		if(m_pcPara_IFLOW)
			m_pcPara_IFLOW->ShowWindow(SW_HIDE);
		if(m_pcPara_RISETIME)
			m_pcPara_RISETIME->ShowWindow(SW_SHOW);
	}
	else
	{
		if(m_pcPara_IFLOW)
			m_pcPara_IFLOW->ShowWindow(SW_HIDE);
		if(m_pcPara_RISETIME)
			m_pcPara_RISETIME->ShowWindow(SW_HIDE);
	}

	if(m_pcPara_FLOWMIN)
		m_pcPara_FLOWMIN->ShowWindow(SW_HIDE);
	if(m_pcPara_PEEP)
		m_pcPara_PEEP->ShowWindow(SW_SHOW);
	if(m_pcPara_CPAP)
		m_pcPara_CPAP->ShowWindow(SW_HIDE);
	if(m_pcPara_PINSP)
		m_pcPara_PINSP->ShowWindow(SW_SHOW);
	if(m_pcPara_PDUO)
		m_pcPara_PDUO->ShowWindow(SW_HIDE);
	if(m_pcPara_PMANUAL)
		m_pcPara_PMANUAL->ShowWindow(SW_HIDE);
	if(m_pcPara_PMITT)
		m_pcPara_PMITT->ShowWindow(SW_HIDE);
	if(m_pcPara_PPSV)
		m_pcPara_PPSV->ShowWindow(SW_SHOW);
	if(m_pcPara_HFAMPL)
		m_pcPara_HFAMPL->ShowWindow(SW_HIDE);
	if(m_pcPara_HFFREQ)
		m_pcPara_HFFREQ->ShowWindow(SW_HIDE);
	if(m_pcPara_ITIME_REC)
		m_pcPara_ITIME_REC->ShowWindow(SW_HIDE);
	if(m_pcPara_PMEAN_REC)
		m_pcPara_PMEAN_REC->ShowWindow(SW_HIDE);
	if(m_pcPara_HFFLOW)
		m_pcPara_HFFLOW->ShowWindow(SW_HIDE);
	if(m_pcPara_TRIGGERCONV)
		m_pcPara_TRIGGERCONV->ShowWindow(SW_SHOW);
	if(m_pcPara_TRIGGERNMODE)
		m_pcPara_TRIGGERNMODE->ShowWindow(SW_HIDE);
	if(m_pcSubPara_TrigThreshold)
		m_pcSubPara_TrigThreshold->ShowWindow(SW_SHOW);
	if(m_pcSubPara_TrigThreshold)
		m_pcSubPara_TrigThreshold->RefreshBtn();
	if(m_pcSubPara_LEAKCOMP) 
		m_pcSubPara_LEAKCOMP->ShowWindow(SW_HIDE);
	if(m_pcSubPara_BIASFLOW) 
		m_pcSubPara_BIASFLOW->ShowWindow(SW_HIDE);
	if(m_pcPara_AMPMAXI)
		m_pcPara_AMPMAXI->ShowWindow(SW_HIDE);

	if(		getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_MANOFF
		||	getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_AUTOOFF)
	{
		if(m_pcPara_VLIMIT)
			m_pcPara_VLIMIT->ShowWindow(SW_HIDE);
		if(m_pcPara_VGARANT)
			m_pcPara_VGARANT->ShowWindow(SW_HIDE);
		if(m_pcPara_PMAXI)
			m_pcPara_PMAXI->ShowWindow(SW_HIDE);
		if(m_pcPara_OnOff)
			m_pcPara_OnOff->ShowWindow(SW_HIDE);
	}
	else
	{
		if(		getModel()->getDATAHANDLER()->isVGUARANTLicenseAvailable() 
			&&	getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VGARANT)
		{
			if(m_pcPara_VLIMIT)
				m_pcPara_VLIMIT->ShowWindow(SW_HIDE);
			if(m_pcPara_VGARANT)
				m_pcPara_VGARANT->ShowWindow(SW_SHOW);
			if(m_pcPara_PMAXI)
				m_pcPara_PMAXI->ShowWindow(SW_SHOW);
			if(m_pcPara_OnOff)
				m_pcPara_OnOff->ShowWindow(SW_SHOW);
		}
		else if(		getModel()->getDATAHANDLER()->isVLIMITLicenseAvailable()
			&&	getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VLIMIT)
		{
			if(m_pcPara_VLIMIT)
				m_pcPara_VLIMIT->ShowWindow(SW_SHOW);
			if(m_pcPara_VGARANT)
				m_pcPara_VGARANT->ShowWindow(SW_HIDE);
			if(m_pcPara_PMAXI)
				m_pcPara_PMAXI->ShowWindow(SW_HIDE);
			if(m_pcPara_OnOff)
				m_pcPara_OnOff->ShowWindow(SW_SHOW);
		}
		else
		{
			if(m_pcPara_VLIMIT)
				m_pcPara_VLIMIT->ShowWindow(SW_HIDE);
			if(m_pcPara_VGARANT)
				m_pcPara_VGARANT->ShowWindow(SW_HIDE);
			if(m_pcPara_PMAXI)
				m_pcPara_PMAXI->ShowWindow(SW_HIDE);
			if(m_pcPara_OnOff)
				m_pcPara_OnOff->ShowWindow(SW_HIDE);
		}
	}

	if(getModel()->getCONFIG()->GetIERelationMode()==RM_SET_ItoBPM)
	{
		if(m_pcPara_BPM)
			m_pcPara_BPM->ShowWindow(SW_SHOW);
		if(m_pcPara_ETIME)
			m_pcPara_ETIME->ShowWindow(SW_HIDE);
		if(m_pcSubPara_BPM)
			m_pcSubPara_BPM->ShowWindow(SW_HIDE);
		if(m_pcSubPara_ETime)
		{
			m_pcSubPara_ETime->SetNameText(getModel()->GetLanguageString(IDS_PARA_ETIME));
			m_pcSubPara_ETime->ShowWindow(SW_SHOW);
			m_pcSubPara_ETime->RefreshBtn();
		}
	}
	else
	{
		if(m_pcPara_BPM)
			m_pcPara_BPM->ShowWindow(SW_HIDE);
		if(m_pcPara_ETIME)
			m_pcPara_ETIME->ShowWindow(SW_SHOW);
		if(m_pcSubPara_BPM)
		{
			m_pcSubPara_BPM->SetNameText(getModel()->GetLanguageString(IDS_PARA_FREQ));
			m_pcSubPara_BPM->ShowWindow(SW_SHOW);
			m_pcSubPara_BPM->RefreshBtn();
		}
		if(m_pcSubPara_ETime)
			m_pcSubPara_ETime->ShowWindow(SW_HIDE);
	}
	int iIE = getModel()->getDATAHANDLER()->GetCurrentIERelation100();
	if(iIE<100)
	{
		if(m_pcPara_BPM)
			m_pcPara_BPM->SetWarning(true);
		if(m_pcPara_ETIME)
			m_pcPara_ETIME->SetWarning(true);
		if(m_pcPara_ITIME)
			m_pcPara_ITIME->SetWarning(true);
	}
	else
	{
		if(m_pcPara_BPM)
			m_pcPara_BPM->SetWarning(false);
		if(m_pcPara_ETIME)
			m_pcPara_ETIME->SetWarning(false);
		if(m_pcPara_ITIME)
			m_pcPara_ITIME->SetWarning(false);
	}
	if(m_pcPara_BACKUP)
		m_pcPara_BACKUP->ShowWindow(SW_HIDE);
	if(m_pcPara_ITIME)
		m_pcPara_ITIME->ShowWindow(SW_SHOW);
	if(m_pcPara_IERatio)
		m_pcPara_IERatio->ShowWindow(SW_HIDE);
	if(m_pcSubPara_IE)
		m_pcSubPara_IE->ShowWindow(SW_HIDE);
	if(m_pcSubPara_DemandFlow)
		m_pcSubPara_DemandFlow->ShowWindow(SW_HIDE);
	if(m_pcSubPara_PSVTIME)
		m_pcSubPara_PSVTIME->ShowWindow(SW_SHOW);
	if(m_pcSubPara_O2Flush)
		m_pcSubPara_O2Flush->ShowWindow(SW_SHOW);
	if(m_pcSubPara_O2act)
		m_pcSubPara_O2act->ShowWindow(SW_SHOW);
	if(m_pcPara_O2)
		m_pcPara_O2->ShowWindow(SW_SHOW);
	if(m_pcPara_O2Flush)
		m_pcPara_O2Flush->ShowWindow(SW_SHOW);
	if(m_pcPara_FLOW)
		m_pcPara_FLOW->ShowWindow(SW_HIDE);
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_CPAPmode()
{
	load_TRIGGERCONVpara(false);
	load_FLOWMINpara(false);	
	load_CPAPpara(false);
	load_PMANUALpara(false,false);
	load_BACKUPpara(false);
	load_FIO2para(false);
	load_FIO2FLUSHpara(false);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::show_CPAPmode()
{
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==true)
	{
		if(m_pcPara_BACKUP)
			m_pcPara_BACKUP->ShowWindow(SW_SHOW);

		if(m_pcPara_TRIGGERCONV)
			m_pcPara_TRIGGERCONV->ShowWindow(SW_SHOW);
		if(m_pcPara_TRIGGERNMODE)
			m_pcPara_TRIGGERNMODE->ShowWindow(SW_HIDE);
		if(m_pcSubPara_TrigThreshold)
			m_pcSubPara_TrigThreshold->ShowWindow(SW_SHOW);
		if(m_pcSubPara_TrigThreshold)
			m_pcSubPara_TrigThreshold->RefreshBtn();
	}
	else
	{
		if(m_pcPara_BACKUP)
			m_pcPara_BACKUP->ShowWindow(SW_SHOW);

		if(m_pcPara_TRIGGERCONV)
			m_pcPara_TRIGGERCONV->ShowWindow(SW_HIDE);
		if(m_pcPara_TRIGGERNMODE)
			m_pcPara_TRIGGERNMODE->ShowWindow(SW_HIDE);
		if(m_pcSubPara_TrigThreshold)
			m_pcSubPara_TrigThreshold->ShowWindow(SW_HIDE);
	}

	if(m_pcPara_ITIME)
		m_pcPara_ITIME->SetBitmaps(m_pcPara_UpB,m_pcPara_DwB,m_pcPara_UpB,m_pcPara_FcB,false,true);
	if(m_pcPara_IFLOW)
		m_pcPara_IFLOW->ShowWindow(SW_HIDE);
	if(m_pcPara_RISETIME)
		m_pcPara_RISETIME->ShowWindow(SW_HIDE);
	if(m_pcPara_FLOWMIN)
		m_pcPara_FLOWMIN->ShowWindow(SW_SHOW);
	if(m_pcPara_PEEP)
		m_pcPara_PEEP->ShowWindow(SW_HIDE);
	if(m_pcPara_CPAP)
		m_pcPara_CPAP->ShowWindow(SW_SHOW);
	if(m_pcPara_PINSP)
		m_pcPara_PINSP->ShowWindow(SW_HIDE);
	if(m_pcPara_PMANUAL)
		m_pcPara_PMANUAL->ShowWindow(SW_SHOW);
	if(m_pcPara_PDUO)
		m_pcPara_PDUO->ShowWindow(SW_HIDE);
	if(m_pcPara_PMITT)
		m_pcPara_PMITT->ShowWindow(SW_HIDE);
	if(m_pcPara_PPSV)
		m_pcPara_PPSV->ShowWindow(SW_HIDE);
	if(m_pcPara_HFAMPL)
		m_pcPara_HFAMPL->ShowWindow(SW_HIDE);
	if(m_pcPara_HFFREQ)
		m_pcPara_HFFREQ->ShowWindow(SW_HIDE);
	if(m_pcPara_ITIME_REC)
		m_pcPara_ITIME_REC->ShowWindow(SW_HIDE);
	if(m_pcPara_PMEAN_REC)
		m_pcPara_PMEAN_REC->ShowWindow(SW_HIDE);
	if(m_pcPara_HFFLOW)
		m_pcPara_HFFLOW->ShowWindow(SW_HIDE);
	if(m_pcPara_VLIMIT)
		m_pcPara_VLIMIT->ShowWindow(SW_HIDE);
	if(m_pcPara_VGARANT)
		m_pcPara_VGARANT->ShowWindow(SW_HIDE);
	if(m_pcPara_PMAXI)
		m_pcPara_PMAXI->ShowWindow(SW_HIDE);
	if(m_pcPara_AMPMAXI)
		m_pcPara_AMPMAXI->ShowWindow(SW_HIDE);
	if(m_pcPara_OnOff)
		m_pcPara_OnOff->ShowWindow(SW_HIDE);
	if(m_pcSubPara_LEAKCOMP) 
		m_pcSubPara_LEAKCOMP->ShowWindow(SW_HIDE);
	if(m_pcSubPara_BIASFLOW) 
		m_pcSubPara_BIASFLOW->ShowWindow(SW_HIDE);
	if(m_pcPara_BPM)
		m_pcPara_BPM->ShowWindow(SW_HIDE);
	if(m_pcPara_ETIME)
		m_pcPara_ETIME->ShowWindow(SW_HIDE);
	if(m_pcSubPara_BPM)
		m_pcSubPara_BPM->ShowWindow(SW_HIDE);
	if(m_pcSubPara_ETime)
		m_pcSubPara_ETime->ShowWindow(SW_HIDE);
	if(m_pcPara_ITIME)
		m_pcPara_ITIME->ShowWindow(SW_HIDE);
	if(m_pcPara_IERatio)
		m_pcPara_IERatio->ShowWindow(SW_HIDE);
	if(m_pcSubPara_IE)
		m_pcSubPara_IE->ShowWindow(SW_HIDE);
	if(m_pcSubPara_DemandFlow)
		m_pcSubPara_DemandFlow->ShowWindow(SW_SHOW);
	if(m_pcSubPara_PSVTIME)
		m_pcSubPara_PSVTIME->ShowWindow(SW_HIDE);
	if(m_pcSubPara_O2Flush)
		m_pcSubPara_O2Flush->ShowWindow(SW_SHOW);
	if(m_pcSubPara_O2act)
		m_pcSubPara_O2act->ShowWindow(SW_SHOW);
	if(m_pcPara_O2)
		m_pcPara_O2->ShowWindow(SW_SHOW);
	if(m_pcPara_O2Flush)
		m_pcPara_O2Flush->ShowWindow(SW_SHOW);
	if(m_pcPara_FLOW)
		m_pcPara_FLOW->ShowWindow(SW_HIDE);
	if(m_pWndMenuVentState)
		m_pWndMenuVentState->ResetButtonPresetstate();
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_PRE_CPAPmode()
{
	load_TRIGGERCONVpara(true);
	load_FLOWMINpara(true);	
	load_CPAPpara(true);
	load_PMANUALpara(false,true);
	load_BACKUPpara(false);
	load_FIO2para(true);
	load_FIO2FLUSHpara(true);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::show_PRE_CPAPmode()
{
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==true)
	{
		if(m_pcPara_BACKUP)
			m_pcPara_BACKUP->ShowWindow(SW_SHOW);

		if(m_pcPara_TRIGGERCONV)
			m_pcPara_TRIGGERCONV->ShowWindow(SW_SHOW);
		if(m_pcPara_TRIGGERNMODE)
			m_pcPara_TRIGGERNMODE->ShowWindow(SW_HIDE);
		if(m_pcSubPara_TrigThreshold)
			m_pcSubPara_TrigThreshold->ShowWindow(SW_SHOW);
		if(m_pcSubPara_TrigThreshold)
			m_pcSubPara_TrigThreshold->RefreshBtn();
	}
	else
	{
		if(m_pcPara_BACKUP)
			m_pcPara_BACKUP->ShowWindow(SW_SHOW);

		if(m_pcPara_TRIGGERCONV)
			m_pcPara_TRIGGERCONV->ShowWindow(SW_HIDE);
		if(m_pcPara_TRIGGERNMODE)
			m_pcPara_TRIGGERNMODE->ShowWindow(SW_HIDE);
		if(m_pcSubPara_TrigThreshold)
			m_pcSubPara_TrigThreshold->ShowWindow(SW_HIDE);
	}

	if(m_pcPara_ITIME)
		m_pcPara_ITIME->SetBitmaps(m_pcPara_UpB,m_pcPara_DwB,m_pcPara_UpB,m_pcPara_FcB,false,true);

	if(m_pcPara_IFLOW)
		m_pcPara_IFLOW->ShowWindow(SW_HIDE);
	if(m_pcPara_RISETIME)
		m_pcPara_RISETIME->ShowWindow(SW_HIDE);
	if(m_pcPara_FLOWMIN)
		m_pcPara_FLOWMIN->ShowWindow(SW_SHOW);
	if(m_pcPara_PEEP)
		m_pcPara_PEEP->ShowWindow(SW_HIDE);
	if(m_pcPara_CPAP)
		m_pcPara_CPAP->ShowWindow(SW_SHOW);
	if(m_pcPara_PMAXI)
		m_pcPara_PMAXI->ShowWindow(SW_HIDE);
	if(m_pcPara_AMPMAXI)
		m_pcPara_AMPMAXI->ShowWindow(SW_HIDE);
	if(m_pcPara_PINSP)
		m_pcPara_PINSP->ShowWindow(SW_HIDE);
	if(m_pcPara_PMANUAL)
		m_pcPara_PMANUAL->ShowWindow(SW_SHOW);
	if(m_pcPara_PDUO)
		m_pcPara_PDUO->ShowWindow(SW_HIDE);
	if(m_pcPara_PMITT)
		m_pcPara_PMITT->ShowWindow(SW_HIDE);
	if(m_pcPara_PPSV)
		m_pcPara_PPSV->ShowWindow(SW_HIDE);
	if(m_pcPara_HFAMPL)
		m_pcPara_HFAMPL->ShowWindow(SW_HIDE);
	if(m_pcPara_HFFREQ)
		m_pcPara_HFFREQ->ShowWindow(SW_HIDE);
	if(m_pcPara_ITIME_REC)
		m_pcPara_ITIME_REC->ShowWindow(SW_HIDE);
	if(m_pcPara_PMEAN_REC)
		m_pcPara_PMEAN_REC->ShowWindow(SW_HIDE);
	if(m_pcPara_HFFLOW)
		m_pcPara_HFFLOW->ShowWindow(SW_HIDE);
	if(m_pcPara_VLIMIT)
		m_pcPara_VLIMIT->ShowWindow(SW_HIDE);
	if(m_pcPara_VGARANT)
		m_pcPara_VGARANT->ShowWindow(SW_HIDE);
	if(m_pcPara_OnOff)
		m_pcPara_OnOff->ShowWindow(SW_HIDE);
	if(m_pcSubPara_LEAKCOMP) 
		m_pcSubPara_LEAKCOMP->ShowWindow(SW_HIDE);
	if(m_pcSubPara_BIASFLOW) 
		m_pcSubPara_BIASFLOW->ShowWindow(SW_HIDE);
	if(m_pcPara_BPM)
		m_pcPara_BPM->ShowWindow(SW_HIDE);
	if(m_pcPara_ETIME)
		m_pcPara_ETIME->ShowWindow(SW_HIDE);
	if(m_pcSubPara_BPM)
		m_pcSubPara_BPM->ShowWindow(SW_HIDE);
	if(m_pcSubPara_ETime)
		m_pcSubPara_ETime->ShowWindow(SW_HIDE);
	if(m_pcPara_ITIME)
		m_pcPara_ITIME->ShowWindow(SW_HIDE);
	if(m_pcPara_IERatio)
		m_pcPara_IERatio->ShowWindow(SW_HIDE);
	if(m_pcSubPara_IE)
		m_pcSubPara_IE->ShowWindow(SW_HIDE);
	if(m_pcSubPara_DemandFlow)
		m_pcSubPara_DemandFlow->ShowWindow(SW_SHOW);
	if(m_pcSubPara_PSVTIME)
		m_pcSubPara_PSVTIME->ShowWindow(SW_HIDE);
	if(m_pcSubPara_O2Flush)
		m_pcSubPara_O2Flush->ShowWindow(SW_SHOW);
	if(m_pcSubPara_O2act)
		m_pcSubPara_O2act->ShowWindow(SW_SHOW);
	if(m_pcPara_O2)
		m_pcPara_O2->ShowWindow(SW_SHOW);
	if(m_pcPara_O2Flush)
		m_pcPara_O2Flush->ShowWindow(SW_SHOW);
	if(m_pcPara_FLOW)
		m_pcPara_FLOW->ShowWindow(SW_HIDE);
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_NCPAPmode()
{
	load_TRIGGERNMODEpara(false);
	load_NCPAP_CPAPpara(false);
	load_PMANUALpara(true,false);
	load_FIO2para(false);
	load_FIO2FLUSHpara(false);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::show_NCPAPmode()
{
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_ITIME)
		m_pcPara_ITIME->SetBitmaps(m_pcPara_UpB,m_pcPara_DwB,m_pcPara_UpB,m_pcPara_FcB,false,true);

	if(m_pcPara_IFLOW)
		m_pcPara_IFLOW->ShowWindow(SW_HIDE);
	if(m_pcPara_RISETIME)
		m_pcPara_RISETIME->ShowWindow(SW_HIDE);
	if(m_pcPara_FLOWMIN)
		m_pcPara_FLOWMIN->ShowWindow(SW_HIDE);
	if(m_pcPara_PEEP)
		m_pcPara_PEEP->ShowWindow(SW_HIDE);
	if(m_pcPara_CPAP)
		m_pcPara_CPAP->ShowWindow(SW_SHOW);
	if(m_pcPara_PMAXI)
		m_pcPara_PMAXI->ShowWindow(SW_HIDE);
	if(m_pcPara_AMPMAXI)
		m_pcPara_AMPMAXI->ShowWindow(SW_HIDE);
	if(m_pcPara_PINSP)
		m_pcPara_PINSP->ShowWindow(SW_HIDE);
	if(m_pcPara_PMANUAL)
		m_pcPara_PMANUAL->ShowWindow(SW_SHOW);
	if(m_pcPara_PDUO)
		m_pcPara_PDUO->ShowWindow(SW_HIDE);
	if(m_pcPara_PMITT)
		m_pcPara_PMITT->ShowWindow(SW_HIDE);
	if(m_pcPara_PPSV)
		m_pcPara_PPSV->ShowWindow(SW_HIDE);
	if(m_pcPara_HFAMPL)
		m_pcPara_HFAMPL->ShowWindow(SW_HIDE);
	if(m_pcPara_HFFREQ)
		m_pcPara_HFFREQ->ShowWindow(SW_HIDE);
	if(m_pcPara_ITIME_REC)
		m_pcPara_ITIME_REC->ShowWindow(SW_HIDE);
	if(m_pcPara_PMEAN_REC)
		m_pcPara_PMEAN_REC->ShowWindow(SW_HIDE);
	if(m_pcPara_HFFLOW)
		m_pcPara_HFFLOW->ShowWindow(SW_HIDE);
	if(m_pcPara_VLIMIT)
		m_pcPara_VLIMIT->ShowWindow(SW_HIDE);
	if(m_pcPara_VGARANT)
		m_pcPara_VGARANT->ShowWindow(SW_HIDE);
	if(m_pcPara_OnOff)
		m_pcPara_OnOff->ShowWindow(SW_HIDE);
	if(m_pcPara_TRIGGERCONV)
		m_pcPara_TRIGGERCONV->ShowWindow(SW_HIDE);
	if(m_pcSubPara_LEAKCOMP) 
		m_pcSubPara_LEAKCOMP->ShowWindow(SW_SHOW);
	if(m_pcSubPara_BIASFLOW) 
		m_pcSubPara_BIASFLOW->ShowWindow(SW_HIDE);
	if(m_pcPara_BPM)
		m_pcPara_BPM->ShowWindow(SW_HIDE);
	if(m_pcPara_ETIME)
		m_pcPara_ETIME->ShowWindow(SW_HIDE);
	if(m_pcSubPara_BPM)
		m_pcSubPara_BPM->ShowWindow(SW_HIDE);
	if(m_pcSubPara_ETime)
		m_pcSubPara_ETime->ShowWindow(SW_HIDE);
	if(m_pcPara_BACKUP)
		m_pcPara_BACKUP->ShowWindow(SW_HIDE);
	if(m_pcPara_ITIME)
		m_pcPara_ITIME->ShowWindow(SW_HIDE);
	if(m_pcPara_IERatio)
		m_pcPara_IERatio->ShowWindow(SW_HIDE);
	if(m_pcSubPara_IE)
		m_pcSubPara_IE->ShowWindow(SW_HIDE);
	if(m_pcSubPara_DemandFlow)
		m_pcSubPara_DemandFlow->ShowWindow(SW_HIDE);
	if(m_pcSubPara_PSVTIME)
		m_pcSubPara_PSVTIME->ShowWindow(SW_HIDE);
	
	if(m_pcSubPara_TrigThreshold)
	{
		if(true==getModel()->getCONFIG()->isNIVTRIGGERREGISTRYenabled())
		{
			m_pcSubPara_TrigThreshold->ShowWindow(SW_SHOW);
			m_pcSubPara_TrigThreshold->RefreshBtn();
		}
		else
			m_pcSubPara_TrigThreshold->ShowWindow(SW_HIDE);
	}
	
	if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==true 
		&&	getModel()->getDATAHANDLER()->GetTubeSet()!=TUBE_MEDIJET)
	{
		if(m_pcPara_TRIGGERNMODE)
			m_pcPara_TRIGGERNMODE->EnableWindow(FALSE);
		getModel()->getDATAHANDLER()->SetTriggerNMODEdisabled();
	}
	else
	{
		if(m_pcPara_TRIGGERNMODE)
			m_pcPara_TRIGGERNMODE->EnableWindow(TRUE);
		getModel()->getDATAHANDLER()->SetTriggerNMODEenabled();
	}
	if(m_pcPara_TRIGGERNMODE)
	{
		if(true==getModel()->getCONFIG()->isNIVTRIGGERREGISTRYenabled())
			m_pcPara_TRIGGERNMODE->ShowWindow(SW_SHOW);
		else
			m_pcPara_TRIGGERNMODE->ShowWindow(SW_HIDE);
	}
	if(m_pcSubPara_O2Flush)
		m_pcSubPara_O2Flush->ShowWindow(SW_SHOW);
	if(m_pcSubPara_O2act)
		m_pcSubPara_O2act->ShowWindow(SW_SHOW);
	if(m_pcPara_O2)
		m_pcPara_O2->ShowWindow(SW_SHOW);
	if(m_pcPara_O2Flush)
		m_pcPara_O2Flush->ShowWindow(SW_SHOW);
	if(m_pcPara_FLOW)
		m_pcPara_FLOW->ShowWindow(SW_HIDE);
	if(m_pWndMenuVentState)
		m_pWndMenuVentState->ResetButtonPresetstate();
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_PRE_NCPAPmode()
{
	load_TRIGGERNMODEpara(true);
	load_NCPAP_CPAPpara(true);
	load_PMANUALpara(true,true);
	load_FIO2para(true);
	load_FIO2FLUSHpara(true);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::show_PRE_NCPAPmode()
{
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_ITIME)
		m_pcPara_ITIME->SetBitmaps(m_pcPara_UpB,m_pcPara_DwB,m_pcPara_UpB,m_pcPara_FcB,false,true);

	if(m_pcPara_IFLOW)
		m_pcPara_IFLOW->ShowWindow(SW_HIDE);
	if(m_pcPara_RISETIME)
		m_pcPara_RISETIME->ShowWindow(SW_HIDE);
	if(m_pcPara_FLOWMIN)
		m_pcPara_FLOWMIN->ShowWindow(SW_HIDE);
	if(m_pcPara_PEEP)
		m_pcPara_PEEP->ShowWindow(SW_HIDE);
	if(m_pcPara_CPAP)
		m_pcPara_CPAP->ShowWindow(SW_SHOW);
	if(m_pcPara_PMAXI)
		m_pcPara_PMAXI->ShowWindow(SW_HIDE);
	if(m_pcPara_AMPMAXI)
		m_pcPara_AMPMAXI->ShowWindow(SW_HIDE);
	if(m_pcPara_PINSP)
		m_pcPara_PINSP->ShowWindow(SW_HIDE);
	if(m_pcPara_PMANUAL)
		m_pcPara_PMANUAL->ShowWindow(SW_SHOW);
	if(m_pcPara_PDUO)
		m_pcPara_PDUO->ShowWindow(SW_HIDE);
	if(m_pcPara_PMITT)
		m_pcPara_PMITT->ShowWindow(SW_HIDE);
	if(m_pcPara_PPSV)
		m_pcPara_PPSV->ShowWindow(SW_HIDE);
	if(m_pcPara_HFAMPL)
		m_pcPara_HFAMPL->ShowWindow(SW_HIDE);
	if(m_pcPara_HFFREQ)
		m_pcPara_HFFREQ->ShowWindow(SW_HIDE);
	if(m_pcPara_ITIME_REC)
		m_pcPara_ITIME_REC->ShowWindow(SW_HIDE);
	if(m_pcPara_PMEAN_REC)
		m_pcPara_PMEAN_REC->ShowWindow(SW_HIDE);
	if(m_pcPara_HFFLOW)
		m_pcPara_HFFLOW->ShowWindow(SW_HIDE);
	if(m_pcPara_VLIMIT)
		m_pcPara_VLIMIT->ShowWindow(SW_HIDE);
	if(m_pcPara_VGARANT)
		m_pcPara_VGARANT->ShowWindow(SW_HIDE);
	if(m_pcPara_OnOff)
		m_pcPara_OnOff->ShowWindow(SW_HIDE);
	if(m_pcPara_TRIGGERCONV)
		m_pcPara_TRIGGERCONV->ShowWindow(SW_HIDE);
	
	if(m_pcSubPara_BIASFLOW) 
		m_pcSubPara_BIASFLOW->ShowWindow(SW_HIDE);
	if(m_pcPara_BPM)
		m_pcPara_BPM->ShowWindow(SW_HIDE);
	if(m_pcPara_ETIME)
		m_pcPara_ETIME->ShowWindow(SW_HIDE);
	if(m_pcSubPara_BPM)
		m_pcSubPara_BPM->ShowWindow(SW_HIDE);
	if(m_pcSubPara_ETime)
		m_pcSubPara_ETime->ShowWindow(SW_HIDE);
	if(m_pcPara_BACKUP)
		m_pcPara_BACKUP->ShowWindow(SW_HIDE);
	if(m_pcPara_ITIME)
		m_pcPara_ITIME->ShowWindow(SW_HIDE);
	if(m_pcPara_IERatio)
		m_pcPara_IERatio->ShowWindow(SW_HIDE);
	if(m_pcSubPara_IE)
		m_pcSubPara_IE->ShowWindow(SW_HIDE);
	if(m_pcSubPara_DemandFlow)
		m_pcSubPara_DemandFlow->ShowWindow(SW_HIDE);
	if(m_pcSubPara_PSVTIME)
		m_pcSubPara_PSVTIME->ShowWindow(SW_HIDE);

	if(m_pcSubPara_TrigThreshold)
	{
		if(true==getModel()->getCONFIG()->isNIVTRIGGERREGISTRYenabled())
		{
			m_pcSubPara_TrigThreshold->ShowWindow(SW_SHOW);
			m_pcSubPara_TrigThreshold->RefreshBtn();
		}
		else
			m_pcSubPara_TrigThreshold->ShowWindow(SW_HIDE);
	}

	if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==true 
		&&	getModel()->getDATAHANDLER()->GetTubeSet()!=TUBE_MEDIJET)
	{
		if(m_pcPara_TRIGGERNMODE)
			m_pcPara_TRIGGERNMODE->EnableWindow(FALSE);
		getModel()->getDATAHANDLER()->SetTriggerNMODEdisabled();
	}
	else
	{
		if(m_pcPara_TRIGGERNMODE)
			m_pcPara_TRIGGERNMODE->EnableWindow(TRUE);
		getModel()->getDATAHANDLER()->SetTriggerNMODEenabled();
	}
	if(m_pcPara_TRIGGERNMODE)
	{
		if(true==getModel()->getCONFIG()->isNIVTRIGGERREGISTRYenabled())
			m_pcPara_TRIGGERNMODE->ShowWindow(SW_SHOW);
		else
			m_pcPara_TRIGGERNMODE->ShowWindow(SW_HIDE);
	}
	if(m_pcSubPara_LEAKCOMP) 
		m_pcSubPara_LEAKCOMP->ShowWindow(SW_SHOW);
	if(m_pcSubPara_O2Flush)
		m_pcSubPara_O2Flush->ShowWindow(SW_SHOW);
	if(m_pcSubPara_O2act)
		m_pcSubPara_O2act->ShowWindow(SW_SHOW);
	if(m_pcPara_O2)
		m_pcPara_O2->ShowWindow(SW_SHOW);
	if(m_pcPara_O2Flush)
		m_pcPara_O2Flush->ShowWindow(SW_SHOW);
	if(m_pcPara_FLOW)
		m_pcPara_FLOW->ShowWindow(SW_HIDE);
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_DUOPAPmode()
{
	load_TRIGGERNMODEpara(false);
	load_DUOPAP_CPAPpara(false);
	load_PDUOpara(false);
	load_ITIMENMODEpara(false);
	load_ETIMENMODEpara(false);
	load_BPMNMODEpara(false);
	load_FIO2para(false);
	load_FIO2FLUSHpara(false);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::show_DUOPAPmode()
{
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_ITIME)
		m_pcPara_ITIME->SetBitmaps(m_pcPara_UpB,m_pcPara_DwB,m_pcPara_UpB,m_pcPara_FcB,false,true);

	if(m_pcPara_IFLOW)
		m_pcPara_IFLOW->ShowWindow(SW_HIDE);
	if(m_pcPara_RISETIME)
		m_pcPara_RISETIME->ShowWindow(SW_HIDE);
	if(m_pcPara_FLOWMIN)
		m_pcPara_FLOWMIN->ShowWindow(SW_HIDE);
	if(m_pcPara_PEEP)
		m_pcPara_PEEP->ShowWindow(SW_HIDE);
	if(m_pcPara_CPAP)
		m_pcPara_CPAP->ShowWindow(SW_SHOW);
	if(m_pcPara_PINSP)
		m_pcPara_PINSP->ShowWindow(SW_HIDE);
	if(m_pcPara_PDUO)
		m_pcPara_PDUO->ShowWindow(SW_SHOW);
	if(m_pcPara_PMANUAL)
		m_pcPara_PMANUAL->ShowWindow(SW_HIDE);
	if(m_pcPara_PMITT)
		m_pcPara_PMITT->ShowWindow(SW_HIDE);
	if(m_pcPara_PPSV)
		m_pcPara_PPSV->ShowWindow(SW_HIDE);
	if(m_pcPara_HFAMPL)
		m_pcPara_HFAMPL->ShowWindow(SW_HIDE);
	if(m_pcPara_HFFREQ)
		m_pcPara_HFFREQ->ShowWindow(SW_HIDE);
	if(m_pcPara_ITIME_REC)
		m_pcPara_ITIME_REC->ShowWindow(SW_HIDE);
	if(m_pcPara_PMEAN_REC)
		m_pcPara_PMEAN_REC->ShowWindow(SW_HIDE);
	if(m_pcPara_HFFLOW)
		m_pcPara_HFFLOW->ShowWindow(SW_HIDE);
	if(m_pcPara_VLIMIT)
		m_pcPara_VLIMIT->ShowWindow(SW_HIDE);
	if(m_pcPara_VGARANT)
		m_pcPara_VGARANT->ShowWindow(SW_HIDE);
	if(m_pcPara_PMAXI)
		m_pcPara_PMAXI->ShowWindow(SW_HIDE);
	if(m_pcPara_AMPMAXI)
		m_pcPara_AMPMAXI->ShowWindow(SW_HIDE);
	if(m_pcPara_OnOff)
		m_pcPara_OnOff->ShowWindow(SW_HIDE);
	if(m_pcSubPara_LEAKCOMP) 
		m_pcSubPara_LEAKCOMP->ShowWindow(SW_SHOW);
	if(m_pcSubPara_BIASFLOW) 
		m_pcSubPara_BIASFLOW->ShowWindow(SW_HIDE);

	if(m_pcSubPara_TrigThreshold)
	{
		if(true==getModel()->getCONFIG()->isNIVTRIGGERREGISTRYenabled())
		{
			m_pcSubPara_TrigThreshold->ShowWindow(SW_SHOW);
			m_pcSubPara_TrigThreshold->RefreshBtn();
		}
		else
			m_pcSubPara_TrigThreshold->ShowWindow(SW_HIDE);
	}

	if(m_pcPara_TRIGGERCONV)
		m_pcPara_TRIGGERCONV->ShowWindow(SW_HIDE);
	if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==true 
		&&	getModel()->getDATAHANDLER()->GetTubeSet()!=TUBE_MEDIJET)
	{
		if(m_pcPara_TRIGGERNMODE)
			m_pcPara_TRIGGERNMODE->EnableWindow(FALSE);
		getModel()->getDATAHANDLER()->SetTriggerNMODEdisabled();
	}
	else if(getModel()->getDATAHANDLER()->PARADATA()->GetITimeNMODEPara()>600)
	{
		if(m_pcPara_TRIGGERNMODE)
			m_pcPara_TRIGGERNMODE->EnableWindow(FALSE);
		getModel()->getDATAHANDLER()->SetTriggerNMODEdisabled();
	}
	else
	{
		if(m_pcPara_TRIGGERNMODE)
			m_pcPara_TRIGGERNMODE->EnableWindow(TRUE);
		getModel()->getDATAHANDLER()->SetTriggerNMODEenabled();
	}
	if(m_pcPara_TRIGGERNMODE)
	{
		if(true==getModel()->getCONFIG()->isNIVTRIGGERREGISTRYenabled())
			m_pcPara_TRIGGERNMODE->ShowWindow(SW_SHOW);
		else
			m_pcPara_TRIGGERNMODE->ShowWindow(SW_HIDE);
	}

	if(m_pcPara_ITIME)
		m_pcPara_ITIME->ShowWindow(SW_SHOW);

	if(getModel()->getCONFIG()->GetIERelationMode()==RM_SET_ItoBPM)
	{
		if(m_pcPara_BPM)
			m_pcPara_BPM->ShowWindow(SW_SHOW);
		if(getModel()->getDATAHANDLER()->PARADATA()->GetTriggerNMODEPara()!=MAXRANGE_TRIGGER_NMODE_OFF)
		{
			m_pcPara_BPM->SetNameText(_T(""));
			m_pcPara_BPM->SetNameNoteText(getModel()->GetLanguageString(IDS_PARA_FREQBACKUP),true);
		}
		else
		{
			m_pcPara_BPM->SetNameText(getModel()->GetLanguageString(IDS_PARA_FREQ));
			m_pcPara_BPM->SetNameNoteText(_T(""),true);
		}
		if(m_pcPara_ETIME)
			m_pcPara_ETIME->ShowWindow(SW_HIDE);
		if(m_pcSubPara_BPM)
			m_pcSubPara_BPM->ShowWindow(SW_HIDE);
		if(m_pcSubPara_ETime)
		{
			m_pcSubPara_ETime->SetNameText(getModel()->GetLanguageString(IDS_PARA_DUO_ETIME));
			m_pcSubPara_ETime->ShowWindow(SW_SHOW);
			m_pcSubPara_ETime->RefreshBtn();
		}
	}
	else
	{
		if(m_pcPara_BPM)
			m_pcPara_BPM->ShowWindow(SW_HIDE);
		if(m_pcPara_ETIME)
			m_pcPara_ETIME->ShowWindow(SW_SHOW);
		if(m_pcSubPara_BPM)
		{
			if(getModel()->getDATAHANDLER()->PARADATA()->GetTriggerNMODEPara()!=MAXRANGE_TRIGGER_NMODE_OFF)
				m_pcSubPara_BPM->SetNameText(getModel()->GetLanguageString(IDS_PARA_FREQBACKUP));
			else
				m_pcSubPara_BPM->SetNameText(getModel()->GetLanguageString(IDS_PARA_DUOFREQ));

			m_pcSubPara_BPM->ShowWindow(SW_SHOW);
			m_pcSubPara_BPM->RefreshBtn();
		}
		if(m_pcSubPara_ETime)
			m_pcSubPara_ETime->ShowWindow(SW_HIDE);
	}
	int iIE = getModel()->getDATAHANDLER()->GetCurrentIERelation100();
	if(iIE<100)
	{
		if(m_pcPara_BPM)
			m_pcPara_BPM->SetWarning(true);
		if(m_pcPara_ETIME)
			m_pcPara_ETIME->SetWarning(true);
		if(m_pcPara_ITIME)
			m_pcPara_ITIME->SetWarning(true);
	}
	else
	{
		if(m_pcPara_BPM)
			m_pcPara_BPM->SetWarning(false);
		if(m_pcPara_ETIME)
			m_pcPara_ETIME->SetWarning(false);
		if(m_pcPara_ITIME)
			m_pcPara_ITIME->SetWarning(false);
	}
	if(m_pcPara_BACKUP)
		m_pcPara_BACKUP->ShowWindow(SW_HIDE);
	if(m_pcPara_IERatio)
		m_pcPara_IERatio->ShowWindow(SW_HIDE);
	if(m_pcSubPara_IE)
		m_pcSubPara_IE->ShowWindow(SW_SHOW);
	if(m_pcSubPara_IE)
		m_pcSubPara_IE->RefreshBtn();
	if(m_pcSubPara_DemandFlow)
		m_pcSubPara_DemandFlow->ShowWindow(SW_HIDE);
	if(m_pcSubPara_PSVTIME)
		m_pcSubPara_PSVTIME->ShowWindow(SW_HIDE);
	if(m_pcSubPara_O2Flush)
		m_pcSubPara_O2Flush->ShowWindow(SW_SHOW);
	if(m_pcSubPara_O2act)
		m_pcSubPara_O2act->ShowWindow(SW_SHOW);
	if(m_pcPara_O2)
		m_pcPara_O2->ShowWindow(SW_SHOW);
	if(m_pcPara_O2Flush)
		m_pcPara_O2Flush->ShowWindow(SW_SHOW);
	if(m_pcPara_FLOW)
		m_pcPara_FLOW->ShowWindow(SW_HIDE);
	if(m_pWndMenuVentState)
		m_pWndMenuVentState->ResetButtonPresetstate();
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_PRE_DUOPAPmode()
{
	load_TRIGGERNMODEpara(true);
	load_DUOPAP_CPAPpara(true);
	load_PDUOpara(true);
	load_ITIMENMODEpara(true);
	load_ETIMENMODEpara(true);
	load_BPMNMODEpara(true);
	load_FIO2para(true);
	load_FIO2FLUSHpara(true);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::show_PRE_DUOPAPmode()
{
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_ITIME)
		m_pcPara_ITIME->SetBitmaps(m_pcPara_UpB,m_pcPara_DwB,m_pcPara_UpB,m_pcPara_FcB,false,true);

	if(m_pcPara_IFLOW)
		m_pcPara_IFLOW->ShowWindow(SW_HIDE);
	if(m_pcPara_RISETIME)
		m_pcPara_RISETIME->ShowWindow(SW_HIDE);
	if(m_pcPara_FLOWMIN)
		m_pcPara_FLOWMIN->ShowWindow(SW_HIDE);
	if(m_pcPara_PEEP)
		m_pcPara_PEEP->ShowWindow(SW_HIDE);
	if(m_pcPara_CPAP)
		m_pcPara_CPAP->ShowWindow(SW_SHOW);
	if(m_pcPara_PMAXI)
		m_pcPara_PMAXI->ShowWindow(SW_HIDE);
	if(m_pcPara_AMPMAXI)
		m_pcPara_AMPMAXI->ShowWindow(SW_HIDE);
	if(m_pcPara_PINSP)
		m_pcPara_PINSP->ShowWindow(SW_HIDE);
	if(m_pcPara_PDUO)
		m_pcPara_PDUO->ShowWindow(SW_SHOW);
	if(m_pcPara_PMANUAL)
		m_pcPara_PMANUAL->ShowWindow(SW_HIDE);
	if(m_pcPara_PMITT)
		m_pcPara_PMITT->ShowWindow(SW_HIDE);
	if(m_pcPara_PPSV)
		m_pcPara_PPSV->ShowWindow(SW_HIDE);
	if(m_pcPara_HFAMPL)
		m_pcPara_HFAMPL->ShowWindow(SW_HIDE);
	if(m_pcPara_HFFREQ)
		m_pcPara_HFFREQ->ShowWindow(SW_HIDE);
	if(m_pcPara_ITIME_REC)
		m_pcPara_ITIME_REC->ShowWindow(SW_HIDE);
	if(m_pcPara_PMEAN_REC)
		m_pcPara_PMEAN_REC->ShowWindow(SW_HIDE);
	if(m_pcPara_HFFLOW)
		m_pcPara_HFFLOW->ShowWindow(SW_HIDE);
	if(m_pcPara_VLIMIT)
		m_pcPara_VLIMIT->ShowWindow(SW_HIDE);
	if(m_pcPara_VGARANT)
		m_pcPara_VGARANT->ShowWindow(SW_HIDE);
	if(m_pcPara_OnOff)
		m_pcPara_OnOff->ShowWindow(SW_HIDE);
	if(m_pcSubPara_LEAKCOMP) 
		m_pcSubPara_LEAKCOMP->ShowWindow(SW_SHOW);
	if(m_pcSubPara_BIASFLOW) 
		m_pcSubPara_BIASFLOW->ShowWindow(SW_HIDE);

	if(m_pcPara_TRIGGERCONV)
		m_pcPara_TRIGGERCONV->ShowWindow(SW_HIDE);
	if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==true 
		&&	getModel()->getDATAHANDLER()->GetTubeSet()!=TUBE_MEDIJET)
	{
		if(m_pcPara_TRIGGERNMODE)
			m_pcPara_TRIGGERNMODE->EnableWindow(FALSE);
		getModel()->getDATAHANDLER()->SetTriggerNMODEdisabled();
	}
	else if(getModel()->getDATAHANDLER()->PRESET()->GetITimeNMODEPara()>600)
	{
		if(m_pcPara_TRIGGERNMODE)
			m_pcPara_TRIGGERNMODE->EnableWindow(FALSE);
		getModel()->getDATAHANDLER()->SetTriggerNMODEdisabled();
	}
	else
	{
		if(m_pcPara_TRIGGERNMODE)
			m_pcPara_TRIGGERNMODE->EnableWindow(TRUE);
		getModel()->getDATAHANDLER()->SetTriggerNMODEenabled();
	}
	if(m_pcPara_TRIGGERNMODE)
	{
		if(true==getModel()->getCONFIG()->isNIVTRIGGERREGISTRYenabled())
			m_pcPara_TRIGGERNMODE->ShowWindow(SW_SHOW);
		else
			m_pcPara_TRIGGERNMODE->ShowWindow(SW_HIDE);
	}

	if(m_pcSubPara_TrigThreshold)
	{
		if(true==getModel()->getCONFIG()->isNIVTRIGGERREGISTRYenabled())
		{
			m_pcSubPara_TrigThreshold->ShowWindow(SW_SHOW);
			m_pcSubPara_TrigThreshold->RefreshBtn();
		}
		else
			m_pcSubPara_TrigThreshold->ShowWindow(SW_HIDE);
	}

	if(m_pcPara_ITIME)
		m_pcPara_ITIME->ShowWindow(SW_SHOW);

	if(getModel()->getCONFIG()->GetIERelationMode()==RM_SET_ItoBPM)
	{
		if(m_pcPara_BPM)
			m_pcPara_BPM->ShowWindow(SW_SHOW);
		if(getModel()->getDATAHANDLER()->PRESET()->GetTriggerNMODEPara()!=MAXRANGE_TRIGGER_NMODE_OFF)
		{
			m_pcPara_BPM->SetNameText(_T(""));
			m_pcPara_BPM->SetNameNoteText(getModel()->GetLanguageString(IDS_PARA_FREQBACKUP),true);
		}
		else
		{
			m_pcPara_BPM->SetNameText(getModel()->GetLanguageString(IDS_PARA_FREQ));
			m_pcPara_BPM->SetNameNoteText(_T(""),true);
		}
		if(m_pcPara_ETIME)
			m_pcPara_ETIME->ShowWindow(SW_HIDE);
		if(m_pcSubPara_BPM)
			m_pcSubPara_BPM->ShowWindow(SW_HIDE);
		if(m_pcSubPara_ETime)
		{
			m_pcSubPara_ETime->SetNameText(getModel()->GetLanguageString(IDS_PARA_DUO_ETIME));
			m_pcSubPara_ETime->ShowWindow(SW_SHOW);
			m_pcSubPara_ETime->RefreshBtn();
		}
	}
	else
	{
		if(m_pcPara_BPM)
			m_pcPara_BPM->ShowWindow(SW_HIDE);
		if(m_pcPara_ETIME)
			m_pcPara_ETIME->ShowWindow(SW_SHOW);
		if(m_pcSubPara_BPM)
		{
			if(getModel()->getDATAHANDLER()->PRESET()->GetTriggerNMODEPara()!=MAXRANGE_TRIGGER_NMODE_OFF)
				m_pcSubPara_BPM->SetNameText(getModel()->GetLanguageString(IDS_PARA_FREQBACKUP));
			else
				m_pcSubPara_BPM->SetNameText(getModel()->GetLanguageString(IDS_PARA_DUOFREQ));
			m_pcSubPara_BPM->ShowWindow(SW_SHOW);
			m_pcSubPara_BPM->RefreshBtn();
		}
		if(m_pcSubPara_ETime)
			m_pcSubPara_ETime->ShowWindow(SW_HIDE);
	}
	int iIE = getModel()->getDATAHANDLER()->GetCurrentIERelation100();
	if(iIE<100)
	{
		if(m_pcPara_BPM)
			m_pcPara_BPM->SetWarning(true);
		if(m_pcPara_ETIME)
			m_pcPara_ETIME->SetWarning(true);
		if(m_pcPara_ITIME)
			m_pcPara_ITIME->SetWarning(true);
	}
	else
	{
		if(m_pcPara_BPM)
			m_pcPara_BPM->SetWarning(false);
		if(m_pcPara_ETIME)
			m_pcPara_ETIME->SetWarning(false);
		if(m_pcPara_ITIME)
			m_pcPara_ITIME->SetWarning(false);
	}
	if(m_pcPara_BACKUP)
		m_pcPara_BACKUP->ShowWindow(SW_HIDE);
	if(m_pcPara_IERatio)
		m_pcPara_IERatio->ShowWindow(SW_HIDE);
	if(m_pcSubPara_IE)
		m_pcSubPara_IE->ShowWindow(SW_SHOW);
	if(m_pcSubPara_IE)
		m_pcSubPara_IE->RefreshBtn();
	if(m_pcSubPara_DemandFlow)
		m_pcSubPara_DemandFlow->ShowWindow(SW_HIDE);
	if(m_pcSubPara_PSVTIME)
		m_pcSubPara_PSVTIME->ShowWindow(SW_HIDE);
	if(m_pcSubPara_O2Flush)
		m_pcSubPara_O2Flush->ShowWindow(SW_SHOW);
	if(m_pcSubPara_O2act)
		m_pcSubPara_O2act->ShowWindow(SW_SHOW);
	if(m_pcPara_O2)
		m_pcPara_O2->ShowWindow(SW_SHOW);
	if(m_pcPara_O2Flush)
		m_pcPara_O2Flush->ShowWindow(SW_SHOW);
	if(m_pcPara_FLOW)
		m_pcPara_FLOW->ShowWindow(SW_HIDE);
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_THERAPYmode()
{
	load_FLOWpara(false);
	load_FIO2para(false);
	load_FIO2FLUSHpara(false);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::show_THERAPYmode()
{
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_ITIME)
		m_pcPara_ITIME->SetBitmaps(m_pcPara_UpB,m_pcPara_DwB,m_pcPara_UpB,m_pcPara_FcB,false,true);

	if(m_pcPara_IFLOW)
		m_pcPara_IFLOW->ShowWindow(SW_HIDE);
	if(m_pcPara_FLOWMIN)
		m_pcPara_FLOWMIN->ShowWindow(SW_HIDE);
	if(m_pcPara_FLOW)
		m_pcPara_FLOW->ShowWindow(SW_SHOW);
	if(m_pcPara_PEEP)
		m_pcPara_PEEP->ShowWindow(SW_HIDE);
	if(m_pcPara_CPAP)
		m_pcPara_CPAP->ShowWindow(SW_HIDE);
	if(m_pcPara_PMAXI)
		m_pcPara_PMAXI->ShowWindow(SW_HIDE);
	if(m_pcPara_AMPMAXI)
		m_pcPara_AMPMAXI->ShowWindow(SW_HIDE);
	if(m_pcPara_PINSP)
		m_pcPara_PINSP->ShowWindow(SW_HIDE);
	if(m_pcPara_PMANUAL)
		m_pcPara_PMANUAL->ShowWindow(SW_HIDE);
	if(m_pcPara_PDUO)
		m_pcPara_PDUO->ShowWindow(SW_HIDE);
	if(m_pcPara_PPSV)
		m_pcPara_PPSV->ShowWindow(SW_HIDE);
	if(m_pcPara_VLIMIT)
		m_pcPara_VLIMIT->ShowWindow(SW_HIDE);
	if(m_pcPara_VGARANT)
		m_pcPara_VGARANT->ShowWindow(SW_HIDE);
	if(m_pcPara_OnOff)
		m_pcPara_OnOff->ShowWindow(SW_HIDE);
	if(m_pcPara_TRIGGERCONV)
		m_pcPara_TRIGGERCONV->ShowWindow(SW_HIDE);
	if(m_pcPara_TRIGGERNMODE)
		m_pcPara_TRIGGERNMODE->ShowWindow(SW_HIDE);
	if(m_pcSubPara_TrigThreshold)
		m_pcSubPara_TrigThreshold->ShowWindow(SW_HIDE);
	if(m_pcSubPara_LEAKCOMP) 
		m_pcSubPara_LEAKCOMP->ShowWindow(SW_HIDE);
	if(m_pcSubPara_BIASFLOW) 
		m_pcSubPara_BIASFLOW->ShowWindow(SW_HIDE);
	if(m_pcPara_BPM)
		m_pcPara_BPM->ShowWindow(SW_HIDE);
	if(m_pcPara_ETIME)
		m_pcPara_ETIME->ShowWindow(SW_HIDE);
	if(m_pcSubPara_BPM)
		m_pcSubPara_BPM->ShowWindow(SW_HIDE);
	if(m_pcSubPara_ETime)
		m_pcSubPara_ETime->ShowWindow(SW_HIDE);
	if(m_pcPara_BACKUP)
		m_pcPara_BACKUP->ShowWindow(SW_HIDE);
	if(m_pcPara_ITIME)
		m_pcPara_ITIME->ShowWindow(SW_HIDE);
	if(m_pcSubPara_IE)
		m_pcSubPara_IE->ShowWindow(SW_HIDE);
	if(m_pcSubPara_DemandFlow)
		m_pcSubPara_DemandFlow->ShowWindow(SW_HIDE);
	if(m_pcSubPara_PSVTIME)
		m_pcSubPara_PSVTIME->ShowWindow(SW_HIDE);
	if(m_pcSubPara_O2Flush)
		m_pcSubPara_O2Flush->ShowWindow(SW_SHOW);
	if(m_pcSubPara_O2act)
		m_pcSubPara_O2act->ShowWindow(SW_SHOW);
	if(m_pcPara_O2)
		m_pcPara_O2->ShowWindow(SW_SHOW);
	if(m_pcPara_O2Flush)
		m_pcPara_O2Flush->ShowWindow(SW_SHOW);
	if(m_pcPara_RISETIME)
		m_pcPara_RISETIME->ShowWindow(SW_HIDE);
	if(m_pcPara_PMITT)
		m_pcPara_PMITT->ShowWindow(SW_HIDE);
	if(m_pcPara_HFAMPL)
		m_pcPara_HFAMPL->ShowWindow(SW_HIDE);
	if(m_pcPara_HFFREQ)
		m_pcPara_HFFREQ->ShowWindow(SW_HIDE);
	if(m_pcPara_ITIME_REC)
		m_pcPara_ITIME_REC->ShowWindow(SW_HIDE);
	if(m_pcPara_PMEAN_REC)
		m_pcPara_PMEAN_REC->ShowWindow(SW_HIDE);
	if(m_pcPara_HFFLOW)
		m_pcPara_HFFLOW->ShowWindow(SW_HIDE);
	if(m_pcPara_IERatio)
		m_pcPara_IERatio->ShowWindow(SW_HIDE);

	if(m_pWndMenuVentState)
		m_pWndMenuVentState->ResetButtonPresetstate();
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_PRE_THERAPYmode()
{
	load_FLOWpara(true);
	load_FIO2para(true);
	load_FIO2FLUSHpara(true);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::show_PRE_THERAPYmode()
{
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_ITIME)
		m_pcPara_ITIME->SetBitmaps(m_pcPara_UpB,m_pcPara_DwB,m_pcPara_UpB,m_pcPara_FcB,false,true);

	if(m_pcPara_IFLOW)
		m_pcPara_IFLOW->ShowWindow(SW_HIDE);
	if(m_pcPara_FLOWMIN)
		m_pcPara_FLOWMIN->ShowWindow(SW_HIDE);
	if(m_pcPara_FLOW)
		m_pcPara_FLOW->ShowWindow(SW_SHOW);
	if(m_pcPara_PEEP)
		m_pcPara_PEEP->ShowWindow(SW_HIDE);
	if(m_pcPara_CPAP)
		m_pcPara_CPAP->ShowWindow(SW_HIDE);
	if(m_pcPara_PMAXI)
		m_pcPara_PMAXI->ShowWindow(SW_HIDE);
	if(m_pcPara_AMPMAXI)
		m_pcPara_AMPMAXI->ShowWindow(SW_HIDE);
	if(m_pcPara_PINSP)
		m_pcPara_PINSP->ShowWindow(SW_HIDE);
	if(m_pcPara_PMANUAL)
		m_pcPara_PMANUAL->ShowWindow(SW_HIDE);
	if(m_pcPara_PDUO)
		m_pcPara_PDUO->ShowWindow(SW_HIDE);
	if(m_pcPara_PPSV)
		m_pcPara_PPSV->ShowWindow(SW_HIDE);
	if(m_pcPara_VLIMIT)
		m_pcPara_VLIMIT->ShowWindow(SW_HIDE);
	if(m_pcPara_VGARANT)
		m_pcPara_VGARANT->ShowWindow(SW_HIDE);
	if(m_pcPara_OnOff)
		m_pcPara_OnOff->ShowWindow(SW_HIDE);
	if(m_pcPara_TRIGGERCONV)
		m_pcPara_TRIGGERCONV->ShowWindow(SW_HIDE);
	if(m_pcPara_TRIGGERNMODE)
		m_pcPara_TRIGGERNMODE->ShowWindow(SW_HIDE);
	if(m_pcSubPara_TrigThreshold)
		m_pcSubPara_TrigThreshold->ShowWindow(SW_HIDE);
	if(m_pcSubPara_LEAKCOMP) 
		m_pcSubPara_LEAKCOMP->ShowWindow(SW_HIDE);
	if(m_pcSubPara_BIASFLOW) 
		m_pcSubPara_BIASFLOW->ShowWindow(SW_HIDE);
	if(m_pcPara_BPM)
		m_pcPara_BPM->ShowWindow(SW_HIDE);
	if(m_pcPara_ETIME)
		m_pcPara_ETIME->ShowWindow(SW_HIDE);
	if(m_pcSubPara_BPM)
		m_pcSubPara_BPM->ShowWindow(SW_HIDE);
	if(m_pcSubPara_ETime)
		m_pcSubPara_ETime->ShowWindow(SW_HIDE);
	if(m_pcPara_BACKUP)
		m_pcPara_BACKUP->ShowWindow(SW_HIDE);
	if(m_pcPara_ITIME)
		m_pcPara_ITIME->ShowWindow(SW_HIDE);
	if(m_pcSubPara_IE)
		m_pcSubPara_IE->ShowWindow(SW_HIDE);
	if(m_pcSubPara_DemandFlow)
		m_pcSubPara_DemandFlow->ShowWindow(SW_HIDE);
	if(m_pcSubPara_PSVTIME)
		m_pcSubPara_PSVTIME->ShowWindow(SW_HIDE);
	if(m_pcSubPara_O2Flush)
		m_pcSubPara_O2Flush->ShowWindow(SW_SHOW);
	if(m_pcSubPara_O2act)
		m_pcSubPara_O2act->ShowWindow(SW_SHOW);
	if(m_pcPara_O2)
		m_pcPara_O2->ShowWindow(SW_SHOW);
	if(m_pcPara_O2Flush)
		m_pcPara_O2Flush->ShowWindow(SW_SHOW);
	if(m_pcPara_RISETIME)
		m_pcPara_RISETIME->ShowWindow(SW_HIDE);
	if(m_pcPara_PMITT)
		m_pcPara_PMITT->ShowWindow(SW_HIDE);
	if(m_pcPara_HFAMPL)
		m_pcPara_HFAMPL->ShowWindow(SW_HIDE);
	if(m_pcPara_HFFREQ)
		m_pcPara_HFFREQ->ShowWindow(SW_HIDE);
	if(m_pcPara_ITIME_REC)
		m_pcPara_ITIME_REC->ShowWindow(SW_HIDE);
	if(m_pcPara_PMEAN_REC)
		m_pcPara_PMEAN_REC->ShowWindow(SW_HIDE);
	if(m_pcPara_HFFLOW)
		m_pcPara_HFFLOW->ShowWindow(SW_HIDE);
	if(m_pcPara_IERatio)
		m_pcPara_IERatio->ShowWindow(SW_HIDE);
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_HFOmode()
{
	load_HF_PMITTpara(false);
	if(getModel()->getCONFIG()->IsHFOManBreathEnabled())
		load_HF_PMANUALpara(false);
	load_HF_PMAXpara(false);
	load_HFAMPLpara(false);
	load_HFFREQpara(false);
	load_ITIMERECpara(false);
	load_HFFLOWpara(false);
	
	if(getModel()->getDATAHANDLER()->isLUNGRECLicenseAvailable())
		load_HF_PMEANRECpara(false);
	
	load_FIO2para(false);
	load_FIO2FLUSHpara(false);

	if(getModel()->getDATAHANDLER()->isVGUARANTLicenseAvailable())
	{
		load_HF_VGUARANTpara(false);
	}

	load_HF_IERatiopara(false);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::show_HFOmode()
{
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_ITIME)
		m_pcPara_ITIME->SetBitmaps(m_pcPara_UpB,m_pcPara_DwB,m_pcPara_UpB,m_pcPara_FcB,false,true);

	if(m_pcPara_IFLOW)
		m_pcPara_IFLOW->ShowWindow(SW_HIDE);
	if(m_pcPara_RISETIME)
		m_pcPara_RISETIME->ShowWindow(SW_HIDE);
	if(m_pcPara_FLOWMIN)
		m_pcPara_FLOWMIN->ShowWindow(SW_HIDE);
	if(m_pcPara_PEEP)
		m_pcPara_PEEP->ShowWindow(SW_HIDE);
	if(m_pcPara_CPAP)
		m_pcPara_CPAP->ShowWindow(SW_HIDE);
	if(m_pcPara_PINSP)
		m_pcPara_PINSP->ShowWindow(SW_HIDE);
	if(m_pcPara_PDUO)
		m_pcPara_PDUO->ShowWindow(SW_HIDE);
	if(m_pcPara_PMAXI)
		m_pcPara_PMAXI->ShowWindow(SW_HIDE);
	
	
	//PMAN1
	/*if(getModel()->getDATAHANDLER()->isHFOManBreathAvailable())
	{
		if(m_pcPara_PMANUAL)
			m_pcPara_PMANUAL->ShowWindow(SW_SHOW);
	}
	else
	{
		if(m_pcPara_PMANUAL)
			m_pcPara_PMANUAL->ShowWindow(SW_HIDE);
	}*/
	if(getModel()->getCONFIG()->IsHFOManBreathEnabled())
	{
		if(		getModel()->getDATAHANDLER()->isLUNGRECLicenseAvailable()==true 
			&&	getModel()->getDATAHANDLER()->PARADATA()->GetFreqRecPara()!=0)
		{
			if(m_pcPara_PMANUAL)
				m_pcPara_PMANUAL->ShowWindow(SW_HIDE);
		}
		else
		{
			if(m_pcPara_PMANUAL)
				m_pcPara_PMANUAL->ShowWindow(SW_SHOW);
		}
	}
	else
	{
		if(m_pcPara_PMANUAL)
			m_pcPara_PMANUAL->ShowWindow(SW_HIDE);
	}

	if(m_pcPara_VLIMIT)
		m_pcPara_VLIMIT->ShowWindow(SW_HIDE);
	if(m_pcPara_TRIGGERCONV)
		m_pcPara_TRIGGERCONV->ShowWindow(SW_HIDE);
	if(m_pcPara_TRIGGERNMODE)
		m_pcPara_TRIGGERNMODE->ShowWindow(SW_HIDE);
	if(m_pcSubPara_TrigThreshold)
		m_pcSubPara_TrigThreshold->ShowWindow(SW_HIDE);
	if(m_pcSubPara_LEAKCOMP) 
		m_pcSubPara_LEAKCOMP->ShowWindow(SW_HIDE);
	if(m_pcSubPara_BIASFLOW) 
		m_pcSubPara_BIASFLOW->ShowWindow(SW_SHOW);
	if(m_pcPara_BPM)
		m_pcPara_BPM->ShowWindow(SW_HIDE);
	if(m_pcPara_ETIME)
		m_pcPara_ETIME->ShowWindow(SW_HIDE);
	if(m_pcSubPara_BPM)
		m_pcSubPara_BPM->ShowWindow(SW_HIDE);
	if(m_pcSubPara_ETime)
		m_pcSubPara_ETime->ShowWindow(SW_HIDE);
	if(m_pcSubPara_O2Flush)
		m_pcSubPara_O2Flush->ShowWindow(SW_SHOW);

	if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==true)
	{
		if(getModel()->getDATAHANDLER()->isVGUARANTLicenseAvailable())
		{
			if(m_pcPara_VGARANT)
				m_pcPara_VGARANT->ShowWindow(SW_HIDE);
			if(m_pcPara_AMPMAXI)
				m_pcPara_AMPMAXI->ShowWindow(SW_HIDE);
			if(m_pcPara_OnOff)
				m_pcPara_OnOff->ShowWindow(SW_HIDE);
		}
	}
	else
	{
		if(	getModel()->getDATAHANDLER()->isVGUARANTLicenseAvailable())
		{
			if(m_pcPara_VGARANT)
				m_pcPara_VGARANT->ShowWindow(SW_SHOW);
			if(m_pcPara_AMPMAXI)
				m_pcPara_AMPMAXI->ShowWindow(SW_SHOW);
			if(m_pcPara_OnOff)
				m_pcPara_OnOff->ShowWindow(SW_SHOW);
		}
		else
		{
			if(m_pcPara_VGARANT)
				m_pcPara_VGARANT->ShowWindow(SW_HIDE);
			if(m_pcPara_AMPMAXI)
				m_pcPara_AMPMAXI->ShowWindow(SW_HIDE);
			if(m_pcPara_OnOff)
				m_pcPara_OnOff->ShowWindow(SW_HIDE);
		}
	}

	if(m_pcPara_O2Flush)
		m_pcPara_O2Flush->ShowWindow(SW_SHOW);
	if(m_pcPara_BACKUP)
		m_pcPara_BACKUP->ShowWindow(SW_HIDE);
	if(m_pcPara_ITIME)
		m_pcPara_ITIME->ShowWindow(SW_HIDE);
	if(m_pcPara_IERatio)
		m_pcPara_IERatio->ShowWindow(SW_SHOW);
	if(m_pcSubPara_IE)
		m_pcSubPara_IE->ShowWindow(SW_HIDE);
	if(m_pcSubPara_DemandFlow)
		m_pcSubPara_DemandFlow->ShowWindow(SW_HIDE);
	if(m_pcSubPara_PSVTIME)
		m_pcSubPara_PSVTIME->ShowWindow(SW_HIDE);
	if(m_pcSubPara_O2act)
		m_pcSubPara_O2act->ShowWindow(SW_SHOW);
	if(m_pcPara_O2)
		m_pcPara_O2->ShowWindow(SW_SHOW);
	if(m_pcPara_PMITT)
		m_pcPara_PMITT->ShowWindow(SW_SHOW);
	if(m_pcPara_PPSV)
		m_pcPara_PPSV->ShowWindow(SW_HIDE);
	if(m_pcPara_HFAMPL)
		m_pcPara_HFAMPL->ShowWindow(SW_SHOW);
	if(m_pcPara_HFFREQ)
		m_pcPara_HFFREQ->ShowWindow(SW_SHOW);
	if(getModel()->getDATAHANDLER()->isLUNGRECLicenseAvailable())
	{
		if(m_pcPara_ITIME_REC)
			m_pcPara_ITIME_REC->ShowWindow(SW_SHOW);
		if(m_pcPara_PMEAN_REC)
			m_pcPara_PMEAN_REC->ShowWindow(SW_SHOW);
	}
	else
	{
		if(m_pcPara_ITIME_REC)
			m_pcPara_ITIME_REC->ShowWindow(SW_HIDE);
		if(m_pcPara_PMEAN_REC)
			m_pcPara_PMEAN_REC->ShowWindow(SW_HIDE);
	}

	if(m_pcPara_HFFLOW)
		m_pcPara_HFFLOW->ShowWindow(SW_SHOW);
	if(m_pcPara_FLOW)
		m_pcPara_FLOW->ShowWindow(SW_HIDE);

	if(m_pWndMenuVentState)
		m_pWndMenuVentState->ResetButtonPresetstate();
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_PRE_HFOmode()
{
	load_HF_PMITTpara(true);
	if(getModel()->getCONFIG()->IsHFOManBreathEnabled())
		load_HF_PMANUALpara(true);
	load_HF_PMAXpara(true);
	load_HFAMPLpara(true);
	load_HFFREQpara(true);
	load_ITIMERECpara(true);
	load_HFFLOWpara(true);
	
	if(getModel()->getDATAHANDLER()->isLUNGRECLicenseAvailable())
		load_HF_PMEANRECpara(true);
	
	load_FIO2para(true);
	load_FIO2FLUSHpara(true);

	if(getModel()->getDATAHANDLER()->isVGUARANTLicenseAvailable())
	{
		load_HF_VGUARANTpara(true);
	}

	load_HF_IERatiopara(true);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::show_PRE_HFOmode()
{
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_ITIME)
		m_pcPara_ITIME->SetBitmaps(m_pcPara_UpB,m_pcPara_DwB,m_pcPara_UpB,m_pcPara_FcB,false,true);

	if(m_pcPara_IFLOW)
		m_pcPara_IFLOW->ShowWindow(SW_HIDE);
	if(m_pcPara_RISETIME)
		m_pcPara_RISETIME->ShowWindow(SW_HIDE);
	if(m_pcPara_FLOWMIN)
		m_pcPara_FLOWMIN->ShowWindow(SW_HIDE);
	if(m_pcPara_PEEP)
		m_pcPara_PEEP->ShowWindow(SW_HIDE);
	if(m_pcPara_CPAP)
		m_pcPara_CPAP->ShowWindow(SW_HIDE);
	if(m_pcPara_PINSP)
		m_pcPara_PINSP->ShowWindow(SW_HIDE);
	if(m_pcPara_PDUO)
		m_pcPara_PDUO->ShowWindow(SW_HIDE);
	if(m_pcPara_PMAXI)
		m_pcPara_PMAXI->ShowWindow(SW_HIDE);
	
	//if(getModel()->getDATAHANDLER()->isHFOManBreathAvailable())//PMAN1
	//{
	//	if(m_pcPara_PMANUAL)
	//		m_pcPara_PMANUAL->ShowWindow(SW_SHOW);
	//}
	//else
	//{
	//	if(m_pcPara_PMANUAL)
	//		m_pcPara_PMANUAL->ShowWindow(SW_HIDE);
	//}
	
	if(getModel()->getCONFIG()->IsHFOManBreathEnabled())
	{
		if(		getModel()->getDATAHANDLER()->isLUNGRECLicenseAvailable()==true 
			&&	getModel()->getDATAHANDLER()->PARADATA()->GetFreqRecPara()!=0)
		{
			if(m_pcPara_PMANUAL)
				m_pcPara_PMANUAL->ShowWindow(SW_HIDE);
		}
		else
		{
			if(m_pcPara_PMANUAL)
				m_pcPara_PMANUAL->ShowWindow(SW_SHOW);
		}
	}
	else
	{
		if(m_pcPara_PMANUAL)
			m_pcPara_PMANUAL->ShowWindow(SW_HIDE);
	}

	if(m_pcPara_VLIMIT)
		m_pcPara_VLIMIT->ShowWindow(SW_HIDE);
	if(m_pcPara_TRIGGERCONV)
		m_pcPara_TRIGGERCONV->ShowWindow(SW_HIDE);
	if(m_pcPara_TRIGGERNMODE)
		m_pcPara_TRIGGERNMODE->ShowWindow(SW_HIDE);
	if(m_pcSubPara_TrigThreshold)
		m_pcSubPara_TrigThreshold->ShowWindow(SW_HIDE);
	if(m_pcSubPara_LEAKCOMP) 
		m_pcSubPara_LEAKCOMP->ShowWindow(SW_HIDE);
	if(m_pcSubPara_BIASFLOW) 
		m_pcSubPara_BIASFLOW->ShowWindow(SW_SHOW);
	if(m_pcPara_BPM)
		m_pcPara_BPM->ShowWindow(SW_HIDE);
	if(m_pcPara_ETIME)
		m_pcPara_ETIME->ShowWindow(SW_HIDE);
	if(m_pcSubPara_BPM)
		m_pcSubPara_BPM->ShowWindow(SW_HIDE);
	if(m_pcSubPara_ETime)
		m_pcSubPara_ETime->ShowWindow(SW_HIDE);
	if(m_pcSubPara_O2Flush)
		m_pcSubPara_O2Flush->ShowWindow(SW_SHOW);
	if(m_pcPara_O2Flush)
		m_pcPara_O2Flush->ShowWindow(SW_SHOW);
	if(m_pcPara_BACKUP)
		m_pcPara_BACKUP->ShowWindow(SW_HIDE);
	if(m_pcPara_ITIME)
		m_pcPara_ITIME->ShowWindow(SW_HIDE);
	if(m_pcPara_IERatio)
		m_pcPara_IERatio->ShowWindow(SW_SHOW);
	if(m_pcSubPara_IE)
		m_pcSubPara_IE->ShowWindow(SW_HIDE);
	if(m_pcSubPara_DemandFlow)
		m_pcSubPara_DemandFlow->ShowWindow(SW_HIDE);
	if(m_pcSubPara_PSVTIME)
		m_pcSubPara_PSVTIME->ShowWindow(SW_HIDE);
	if(m_pcSubPara_O2act)
		m_pcSubPara_O2act->ShowWindow(SW_SHOW);
	if(m_pcPara_O2)
		m_pcPara_O2->ShowWindow(SW_SHOW);
	if(m_pcPara_PMITT)
		m_pcPara_PMITT->ShowWindow(SW_SHOW);
	if(m_pcPara_PPSV)
		m_pcPara_PPSV->ShowWindow(SW_HIDE);
	if(m_pcPara_HFAMPL)
		m_pcPara_HFAMPL->ShowWindow(SW_SHOW);
	if(m_pcPara_HFFREQ)
		m_pcPara_HFFREQ->ShowWindow(SW_SHOW);
	if(getModel()->getDATAHANDLER()->isLUNGRECLicenseAvailable())
	{
		if(m_pcPara_ITIME_REC)
			m_pcPara_ITIME_REC->ShowWindow(SW_SHOW);
		if(m_pcPara_PMEAN_REC)
			m_pcPara_PMEAN_REC->ShowWindow(SW_SHOW);
	}
	else
	{
		if(m_pcPara_ITIME_REC)
			m_pcPara_ITIME_REC->ShowWindow(SW_HIDE);
		if(m_pcPara_PMEAN_REC)
			m_pcPara_PMEAN_REC->ShowWindow(SW_HIDE);
	}
	if(m_pcPara_HFFLOW)
		m_pcPara_HFFLOW->ShowWindow(SW_SHOW);

	if(		getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_MANOFF
		||	getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_AUTOOFF)
	{
		if(m_pcPara_VGARANT)
			m_pcPara_VGARANT->ShowWindow(SW_HIDE);
		if(m_pcPara_AMPMAXI)
			m_pcPara_AMPMAXI->ShowWindow(SW_HIDE);
		if(m_pcPara_OnOff)
			m_pcPara_OnOff->ShowWindow(SW_HIDE);
	}
	else
	{
		if(getModel()->getDATAHANDLER()->isVGUARANTLicenseAvailable())
		{
			if(m_pcPara_VGARANT)
				m_pcPara_VGARANT->ShowWindow(SW_SHOW);
			if(m_pcPara_AMPMAXI)
				m_pcPara_AMPMAXI->ShowWindow(SW_SHOW);
			if(m_pcPara_OnOff)
				m_pcPara_OnOff->ShowWindow(SW_SHOW);
		}
		else
		{
			if(m_pcPara_VGARANT)
				m_pcPara_VGARANT->ShowWindow(SW_HIDE);
			if(m_pcPara_AMPMAXI)
				m_pcPara_AMPMAXI->ShowWindow(SW_HIDE);
			if(m_pcPara_OnOff)
				m_pcPara_OnOff->ShowWindow(SW_HIDE);
		}
	}
	if(m_pcPara_FLOW)
		m_pcPara_FLOW->ShowWindow(SW_HIDE);
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_FIO2para(bool bPRESET)
{
	fVALUE fv;
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_O2)
	{
		if(bPRESET)
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PRESET()->GetO2MinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PRESET()->GetO2MaxPara();
			fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetO2Para();
		}
		else
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetO2MinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetO2MaxPara();
			fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetO2Para();

		}
		if(fv.iValue>fv.iUpperLimit)
		{
			fv.iValue=fv.iUpperLimit;
			getModel()->getDATAHANDLER()->SetCurrentO2Para(fv.iValue);
		}
		else if(fv.iValue<fv.iLowerLimit)
		{
			fv.iValue=fv.iLowerLimit;
			getModel()->getDATAHANDLER()->SetCurrentO2Para(fv.iValue);
		}
		m_pcPara_O2->SetValue(fv, true);
	}
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
	
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_FIO2FLUSHpara(bool bPRESET)
{
	fVALUE fv;
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_O2Flush)
	{
		if(bPRESET)
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PRESET()->GetO2FlushMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PRESET()->GetO2FlushMaxPara();
			fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetO2FlushPara();
			if(fv.iValue<(getModel()->getDATAHANDLER()->PRESET()->GetO2Para()+MAXDIFF_O2_FLUSH))
			{
				fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetO2Para()+MAXDIFF_O2_FLUSH;
				getModel()->getDATAHANDLER()->SetCurrentO2FlushPara(fv.iValue);
			}
		}
		else
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetO2FlushMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetO2FlushMaxPara();
			fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetO2FlushPara();
			if(fv.iValue<(getModel()->getDATAHANDLER()->PARADATA()->GetO2Para()+MAXDIFF_O2_FLUSH))
			{
				fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetO2Para()+MAXDIFF_O2_FLUSH;
				getModel()->getDATAHANDLER()->SetCurrentO2FlushPara(fv.iValue);
			}
		}
		if(fv.iValue>fv.iUpperLimit)
		{
			fv.iValue=fv.iUpperLimit;
			getModel()->getDATAHANDLER()->SetCurrentO2FlushPara(fv.iValue);
		}
		else if(fv.iValue<fv.iLowerLimit)
		{
			fv.iValue=fv.iLowerLimit;
			getModel()->getDATAHANDLER()->SetCurrentO2FlushPara(fv.iValue);
		}
		m_pcPara_O2Flush->SetValue(fv, true);
	}
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_HF_VGUARANTpara(bool bPRESET)
{
	fVALUE fv;
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_VGARANT)
	{
		if(bPRESET)
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PRESET()->GetHFVGarantMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PRESET()->GetHFVGarantMaxPara();
			fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetHFVGarantPara();
		}
		else
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFVGarantMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFVGarantMaxPara();
			fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetHFVGarantPara();
		}
		if(fv.iValue>fv.iUpperLimit)
		{
			fv.iValue=fv.iUpperLimit;
			getModel()->getDATAHANDLER()->SetCurrentVGarantParamData(fv.iValue);
		}
		else if(fv.iValue<fv.iLowerLimit && fv.iValue!=0)
		{
			fv.iValue=fv.iLowerLimit;
			getModel()->getDATAHANDLER()->SetCurrentVGarantParamData(fv.iValue);
		}
		if(fv.iValue>getModel()->getDATAHANDLER()->GetCurrentVgarantMaxKey())
		{
			m_pcPara_VGARANT->SetWarning(true);
		}
		else
		{
			m_pcPara_VGARANT->SetWarning(false);
		}
		m_pcPara_VGARANT->SetValue(fv, true);
		m_pcPara_VGARANT->SetOffState(!getModel()->getDATAHANDLER()->IsCurrentModeVGarantStateOn());
	}
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_VGUARANTpara(bool bTrigger,bool bPRESET)
{
	fVALUE fv;
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_VGARANT)
	{
		if(bTrigger)
		{
			if(bPRESET)
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PRESET()->GetVGarantMinPara_TRIGGER();
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PRESET()->GetVGarantMaxPara_TRIGGER();
				fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetVGarantPara_TRIGGER();
			}
			else
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetVGarantMinPara_TRIGGER();
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetVGarantMaxPara_TRIGGER();
				fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetVGarantPara_TRIGGER();
			}
		}
		else
		{
			if(bPRESET)
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PRESET()->GetVGarantMinPara_IPPV();
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PRESET()->GetVGarantMaxPara_IPPV();
				fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetVGarantPara_IPPV();
			}
			else
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetVGarantMinPara_IPPV();
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetVGarantMaxPara_IPPV();
				fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetVGarantPara_IPPV();
			}
		}
		if(fv.iValue>fv.iUpperLimit)
		{
			fv.iValue=fv.iUpperLimit;
			getModel()->getDATAHANDLER()->SetCurrentVGarantParamData(fv.iValue);
		}
		else if(fv.iValue<fv.iLowerLimit && fv.iValue!=0)
		{
			fv.iValue=fv.iLowerLimit;
			getModel()->getDATAHANDLER()->SetCurrentVGarantParamData(fv.iValue);
		}
		if(fv.iValue>getModel()->getDATAHANDLER()->GetCurrentVgarantMaxKey())
		{
			m_pcPara_VGARANT->SetWarning(true);
		}
		else
		{
			m_pcPara_VGARANT->SetWarning(false);
		}
		m_pcPara_VGARANT->SetValue(fv, true);
		m_pcPara_VGARANT->SetOffState(!getModel()->getDATAHANDLER()->IsCurrentModeVGarantStateOn());
	}
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_VLIMITpara(bool bTrigger,bool bPRESET)
{
	fVALUE fv;
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_VLIMIT)
	{
		if(bTrigger)
		{
			if(bPRESET)
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PRESET()->GetVLimitMinPara_TRIGGER();
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PRESET()->GetVLimitMaxPara_TRIGGER();
				fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetVLimitParam_TRIGGER();
			}
			else
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetVLimitMinPara_TRIGGER();
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetVLimitMaxPara_TRIGGER();
				fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetVLimitParam_TRIGGER();
			}
		}
		else
		{
			if(bPRESET)
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PRESET()->GetVLimitMinPara_IPPV();
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PRESET()->GetVLimitMaxPara_IPPV();
				fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetVLimitParam_IPPV();
			}
			else
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetVLimitMinPara_IPPV();
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetVLimitMaxPara_IPPV();
				fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetVLimitParam_IPPV();
			}
		}
		if(fv.iValue>fv.iUpperLimit)
		{
			fv.iValue=fv.iUpperLimit;
			getModel()->getDATAHANDLER()->SetCurrentVLimitParam(fv.iValue,getModel()->getDATAHANDLER()->IsCurrentModeVLimitStateOn(),true);
		}
		else if(fv.iValue<fv.iLowerLimit && fv.iValue!=0)
		{
			fv.iValue=fv.iLowerLimit;
			getModel()->getDATAHANDLER()->SetCurrentVLimitParam(fv.iValue,getModel()->getDATAHANDLER()->IsCurrentModeVLimitStateOn(),true);
		}
		if(fv.iValue>getModel()->getDATAHANDLER()->GetCurrentVlimitMaxKey())
		{
			m_pcPara_VLIMIT->SetWarning(true);
		}
		else
		{
			m_pcPara_VLIMIT->SetWarning(false);
		}
		m_pcPara_VLIMIT->SetValue(fv, true);
		m_pcPara_VLIMIT->SetOffState(!getModel()->getDATAHANDLER()->IsCurrentModeVLimitStateOn());
	}
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_HF_PMITTpara(bool bPRESET)
{
	fVALUE fv;
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_PMITT)
	{
		WORD iHFPMeanMaxKey=0;
		if(bPRESET)
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PRESET()->GetHFPMeanMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PRESET()->GetHFPMeanMaxPara();
			fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetHFPMeanPara();
			iHFPMeanMaxKey=getModel()->getDATAHANDLER()->PRESET()->GetHFPMeanMaxKey();
		}
		else
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanMaxPara();
			fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanPara();
			iHFPMeanMaxKey=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanMaxKey();
		}

		if(fv.iValue>fv.iUpperLimit)
		{
			fv.iValue=fv.iUpperLimit;
			getModel()->getDATAHANDLER()->SetCurrentPMeanPara(fv.iValue);
		}
		else if(fv.iValue<fv.iLowerLimit)
		{
			fv.iValue=fv.iLowerLimit;
			getModel()->getDATAHANDLER()->SetCurrentPMeanPara(fv.iValue);
		}
		if(fv.iValue>iHFPMeanMaxKey)
		{
			m_pcPara_PMITT->SetWarning();
		}
		else
		{
			m_pcPara_PMITT->SetWarning(false);
		}
		m_pcPara_PMITT->SetValue(fv, true);
	}
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_FLOWpara(bool bPRESET)
{
	fVALUE fv;
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_FLOW)
	{
		if(bPRESET)
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PRESET()->GetTherapieFlowMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PRESET()->GetTherapieFlowMaxPara();
			fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetTherapieFLOWPara();
		}
		else
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetTherapieFlowMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetTherapieFlowMaxPara();
			fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetTherapieFLOWPara();
		}

		if(fv.iValue>fv.iUpperLimit)
		{
			fv.iValue=fv.iUpperLimit;
			getModel()->getDATAHANDLER()->SetCurrentTherapieFlowParaData(fv.iValue);
		}
		else if(fv.iValue<fv.iLowerLimit)
		{
			fv.iValue=fv.iLowerLimit;
			getModel()->getDATAHANDLER()->SetCurrentTherapieFlowParaData(fv.iValue);
		}

		if(fv.iValue>getModel()->getDATAHANDLER()->GetCurrentTherapieFlowMaxKey())
		{
			m_pcPara_FLOW->SetWarning();
		}
		else
		{
			m_pcPara_FLOW->SetWarning(false);
		}

		m_pcPara_FLOW->SetValue(fv, true);
	}
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_PDUOpara(bool bPRESET)
{
	fVALUE fv;
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_PDUO)
	{
		if(bPRESET)
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PRESET()->GetPManualNMODEMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PRESET()->GetPManualNMODEMaxPara();

			if(getModel()->getDATAHANDLER()->PRESET()->GetPManualNMODEPara()<getModel()->getDATAHANDLER()->PRESET()->GetPManualNMODEMinPara())
				getModel()->getDATAHANDLER()->PRESET()->SetPManualNMODEPara(getModel()->getDATAHANDLER()->PRESET()->GetPManualNMODEMinPara(),false,false);
			else if(getModel()->getDATAHANDLER()->PRESET()->GetPManualNMODEPara()>getModel()->getDATAHANDLER()->PRESET()->GetPManualNMODEMaxPara())
				getModel()->getDATAHANDLER()->PRESET()->SetPManualNMODEPara(getModel()->getDATAHANDLER()->PRESET()->GetPManualNMODEMaxPara(),false,false);

			fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetPManualNMODEPara();
		}
		else
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPManualNMODEMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPManualNMODEMaxPara();

			if(getModel()->getDATAHANDLER()->PARADATA()->GetPManualNMODEPara()<getModel()->getDATAHANDLER()->PARADATA()->GetPManualNMODEMinPara())
				getModel()->getDATAHANDLER()->SetPManualNMODEParadata(getModel()->getDATAHANDLER()->PARADATA()->GetPManualNMODEMinPara());
			else if(getModel()->getDATAHANDLER()->PARADATA()->GetPManualNMODEPara()>getModel()->getDATAHANDLER()->PARADATA()->GetPManualNMODEMaxPara())
				getModel()->getDATAHANDLER()->SetPManualNMODEParadata(getModel()->getDATAHANDLER()->PARADATA()->GetPManualNMODEMaxPara());

			fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetPManualNMODEPara();
		}

		if(fv.iValue>fv.iUpperLimit)
		{
			fv.iValue=fv.iUpperLimit;
			getModel()->getDATAHANDLER()->SetCurrentPManPara(fv.iValue);
		}
		else if(fv.iValue<fv.iLowerLimit)
		{
			fv.iValue=fv.iLowerLimit;
			getModel()->getDATAHANDLER()->SetCurrentPManPara(fv.iValue);
		}
		if(fv.iValue>getModel()->getDATAHANDLER()->GetCurrentPManualMaxKey())
		{
			m_pcPara_PDUO->SetWarning();
		}
		m_pcPara_PDUO->SetValue(fv, true);
	}
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_DUOPAP_CPAPpara(bool bPRESET)
{
	fVALUE fv;
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_CPAP)
	{
		if(bPRESET)
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PRESET()->GetCPAPNMODEMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PRESET()->GetCPAPNMODEMaxPara();

			if(getModel()->getDATAHANDLER()->PRESET()->GetCPAPNMODEPara()<getModel()->getDATAHANDLER()->PRESET()->GetCPAPNMODEMinPara())
				getModel()->getDATAHANDLER()->PRESET()->SetCPAPNMODEPara(getModel()->getDATAHANDLER()->PRESET()->GetCPAPNMODEMinPara(),false,false);
			else if(getModel()->getDATAHANDLER()->PRESET()->GetCPAPNMODEPara()>getModel()->getDATAHANDLER()->PRESET()->GetCPAPNMODEMaxPara())
				getModel()->getDATAHANDLER()->PRESET()->SetCPAPNMODEPara(getModel()->getDATAHANDLER()->PRESET()->GetCPAPNMODEMaxPara(),false,false);

			fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetCPAPNMODEPara();
		}
		else
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetCPAPNMODEMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetCPAPNMODEMaxPara();

			if(getModel()->getDATAHANDLER()->PARADATA()->GetCPAPNMODEPara()<getModel()->getDATAHANDLER()->PARADATA()->GetCPAPNMODEMinPara())
				getModel()->getDATAHANDLER()->SetCPAPNMODEParadata(getModel()->getDATAHANDLER()->PARADATA()->GetCPAPNMODEMinPara());
			else if(getModel()->getDATAHANDLER()->PARADATA()->GetCPAPNMODEPara()>getModel()->getDATAHANDLER()->PARADATA()->GetCPAPNMODEMaxPara())
				getModel()->getDATAHANDLER()->SetCPAPNMODEParadata(getModel()->getDATAHANDLER()->PARADATA()->GetCPAPNMODEMaxPara());

			fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetCPAPNMODEPara();
		}
		if(fv.iValue>fv.iUpperLimit)
		{
			fv.iValue=fv.iUpperLimit;
			getModel()->getDATAHANDLER()->SetCurrentCPAPPara(fv.iValue);
		}
		else if(fv.iValue<fv.iLowerLimit)
		{
			fv.iValue=fv.iLowerLimit;
			getModel()->getDATAHANDLER()->SetCurrentCPAPPara(fv.iValue);
		}
		if(fv.iValue>getModel()->getDATAHANDLER()->GetCurrentCPAPMaxKey())
		{
			m_pcPara_CPAP->SetWarning();
		}
		else
		{
			m_pcPara_CPAP->SetWarning(false);
		}
		m_pcPara_CPAP->SetNameText(getModel()->GetLanguageString(IDS_PARA_DUO_CPAP));
		m_pcPara_CPAP->SetValue(fv, true);
	}
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_NCPAP_CPAPpara(bool bPRESET)
{
	fVALUE fv;
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_CPAP)
	{
		if(bPRESET)
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PRESET()->GetCPAPNMODEMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PRESET()->GetCPAPNMODEMaxPara();

			if(getModel()->getDATAHANDLER()->PRESET()->GetCPAPNMODEPara()<getModel()->getDATAHANDLER()->PRESET()->GetCPAPNMODEMinPara())
				getModel()->getDATAHANDLER()->PRESET()->SetCPAPNMODEPara(getModel()->getDATAHANDLER()->PRESET()->GetCPAPNMODEMinPara(),false,false);
			else if(getModel()->getDATAHANDLER()->PRESET()->GetCPAPNMODEPara()>getModel()->getDATAHANDLER()->PRESET()->GetCPAPNMODEMaxPara())
				getModel()->getDATAHANDLER()->PRESET()->SetCPAPNMODEPara(getModel()->getDATAHANDLER()->PRESET()->GetCPAPNMODEMaxPara(),false,false);

			fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetCPAPNMODEPara();
		}
		else
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetCPAPNMODEMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetCPAPNMODEMaxPara();

			if(getModel()->getDATAHANDLER()->PARADATA()->GetCPAPNMODEPara()<getModel()->getDATAHANDLER()->PARADATA()->GetCPAPNMODEMinPara())
				getModel()->getDATAHANDLER()->SetCPAPNMODEParadata(getModel()->getDATAHANDLER()->PARADATA()->GetCPAPNMODEMinPara());
			else if(getModel()->getDATAHANDLER()->PARADATA()->GetCPAPNMODEPara()>getModel()->getDATAHANDLER()->PARADATA()->GetCPAPNMODEMaxPara())
				getModel()->getDATAHANDLER()->SetCPAPNMODEParadata(getModel()->getDATAHANDLER()->PARADATA()->GetCPAPNMODEMaxPara());

			fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetCPAPNMODEPara();
		}
		if(fv.iValue>fv.iUpperLimit)
		{
			fv.iValue=fv.iUpperLimit;
			getModel()->getDATAHANDLER()->SetCurrentCPAPPara(fv.iValue);
		}
		else if(fv.iValue<fv.iLowerLimit)
		{
			fv.iValue=fv.iLowerLimit;
			getModel()->getDATAHANDLER()->SetCurrentCPAPPara(fv.iValue);
		}
		if(fv.iValue>getModel()->getDATAHANDLER()->GetCurrentCPAPMaxKey())
		{
			m_pcPara_CPAP->SetWarning();
		}
		else
		{
			m_pcPara_CPAP->SetWarning(false);
		}
		m_pcPara_CPAP->SetNameText(getModel()->GetLanguageString(IDS_PARA_CPAP));
		m_pcPara_CPAP->SetValue(fv, true);
	}
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_HF_PMANUALpara(bool bPRESET)
{
	fVALUE fv;
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_PMANUAL)
	{
		bool bLimited=false;
		if(bPRESET)
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PRESET()->GetPManualMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PRESET()->GetPManualMaxPara();
			fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetPManualHFOPara();
			if(fv.iValue>fv.iUpperLimit)
			{
				fv.iValue=fv.iUpperLimit;
				bLimited=true;
				//getModel()->getDATAHANDLER()->PRESET()->SetPManualHFOPara(fv.iValue,false,false);
			}
			else if(fv.iValue<fv.iLowerLimit)
			{
				fv.iValue=fv.iLowerLimit;
				bLimited=true;
				//getModel()->getDATAHANDLER()->PRESET()->SetPManualHFOPara(fv.iValue,false,false);
			}

			WORD iPmean=getModel()->getDATAHANDLER()->PRESET()->GetHFPMeanPara();
			if(fv.iValue<iPmean+5)
			{
				if(iPmean%10==0)
					fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetHFPMeanPara()+10;
				else
					fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetHFPMeanPara()+5;
				bLimited=true;
				//getModel()->getDATAHANDLER()->PRESET()->SetPManualHFOPara(fv.iValue,false,false);
			}

			if(bLimited)
				getModel()->getDATAHANDLER()->PRESET()->SetPManualHFOPara(fv.iValue,false,false);

			if(fv.iValue>getModel()->getDATAHANDLER()->PRESET()->GetPManualMaxKey())
			{
				m_pcPara_PMANUAL->SetWarning();
			}
			else
			{
				m_pcPara_PMANUAL->SetWarning(false);
			}
			m_pcPara_PMANUAL->SetValue(fv, true);
		}
		else
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPManualMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPManualMaxPara();
			fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetPManualHFOPara();
			if(fv.iValue>fv.iUpperLimit)
			{
				fv.iValue=fv.iUpperLimit;
				bLimited=true;
				//getModel()->getDATAHANDLER()->SetPManualHFOParadata(fv.iValue);
			}
			else if(fv.iValue<fv.iLowerLimit)
			{
				fv.iValue=fv.iLowerLimit;
				bLimited=true;
				//getModel()->getDATAHANDLER()->SetPManualHFOParadata(fv.iValue);
			}


			WORD iPmean=getModel()->getDATAHANDLER()->PRESET()->GetHFPMeanPara();
			if(fv.iValue<iPmean+5)
			{
				if(iPmean%10==0)
					fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetHFPMeanPara()+10;
				else
					fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetHFPMeanPara()+5;
				bLimited=true;
				//getModel()->getDATAHANDLER()->SetPManualHFOParadata(fv.iValue);
			}

			if(bLimited)
				getModel()->getDATAHANDLER()->SetPManualHFOParadata(fv.iValue);

			if(fv.iValue>getModel()->getDATAHANDLER()->PARADATA()->GetPManualMaxKey())
			{
				m_pcPara_PMANUAL->SetWarning();
			}
			else
			{
				m_pcPara_PMANUAL->SetWarning(false);
			}
			m_pcPara_PMANUAL->SetValue(fv, true);
		}
	}
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_PMANUALpara(bool bNMODE,bool bPRESET)
{
	fVALUE fv;
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_PMANUAL)
	{
		if(bNMODE)
		{
			if(bPRESET)
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PRESET()->GetPManualNMODEMinPara();
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PRESET()->GetPManualNMODEMaxPara();
				if(getModel()->getDATAHANDLER()->PRESET()->GetPManualNMODEPara()<getModel()->getDATAHANDLER()->PRESET()->GetPManualNMODEMinPara())
					getModel()->getDATAHANDLER()->PRESET()->SetPManualNMODEPara(getModel()->getDATAHANDLER()->PRESET()->GetPManualNMODEMinPara(),false,false);
				else if(getModel()->getDATAHANDLER()->PRESET()->GetPManualNMODEPara()>getModel()->getDATAHANDLER()->PRESET()->GetPManualNMODEMaxPara())
					getModel()->getDATAHANDLER()->PRESET()->SetPManualNMODEPara(getModel()->getDATAHANDLER()->PRESET()->GetPManualNMODEMaxPara(),false,false);
				fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetPManualNMODEPara();
			}
			else
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPManualNMODEMinPara();
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPManualNMODEMaxPara();
				if(getModel()->getDATAHANDLER()->PARADATA()->GetPManualNMODEPara()<getModel()->getDATAHANDLER()->PARADATA()->GetPManualNMODEMinPara())
					getModel()->getDATAHANDLER()->SetPManualNMODEParadata(getModel()->getDATAHANDLER()->PARADATA()->GetPManualNMODEMinPara());
				else if(getModel()->getDATAHANDLER()->PARADATA()->GetPManualNMODEPara()>getModel()->getDATAHANDLER()->PARADATA()->GetPManualNMODEMaxPara())
					getModel()->getDATAHANDLER()->SetPManualNMODEParadata(getModel()->getDATAHANDLER()->PARADATA()->GetPManualNMODEMaxPara());
				fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetPManualNMODEPara();
			}
		}
		else
		{
			if(bPRESET)
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PRESET()->GetPManualMinPara();
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PRESET()->GetPManualMaxPara();
				if(getModel()->getDATAHANDLER()->PRESET()->GetPManualCPAPPara()<getModel()->getDATAHANDLER()->PRESET()->GetPManualMinPara())
					getModel()->getDATAHANDLER()->PRESET()->SetPManualCPAPPara(getModel()->getDATAHANDLER()->PRESET()->GetPManualMinPara(),false,false);
				else if(getModel()->getDATAHANDLER()->PRESET()->GetPManualCPAPPara()>getModel()->getDATAHANDLER()->PRESET()->GetPManualMaxPara())
					getModel()->getDATAHANDLER()->PRESET()->SetPManualCPAPPara(getModel()->getDATAHANDLER()->PRESET()->GetPManualMaxPara(),false,false);
				fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetPManualCPAPPara();
			}
			else
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPManualMinPara();
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPManualMaxPara();
				if(getModel()->getDATAHANDLER()->PARADATA()->GetPManualCPAPPara()<getModel()->getDATAHANDLER()->PARADATA()->GetPManualMinPara())
					getModel()->getDATAHANDLER()->SetPManualCPAPParadata(getModel()->getDATAHANDLER()->PARADATA()->GetPManualMinPara());
				else if(getModel()->getDATAHANDLER()->PARADATA()->GetPManualCPAPPara()>getModel()->getDATAHANDLER()->PARADATA()->GetPManualMaxPara())
					getModel()->getDATAHANDLER()->SetPManualCPAPParadata(getModel()->getDATAHANDLER()->PARADATA()->GetPManualMaxPara());
				fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetPManualCPAPPara();
			}
		}
		
		if(fv.iValue>fv.iUpperLimit)
		{
			fv.iValue=fv.iUpperLimit;
			getModel()->getDATAHANDLER()->SetCurrentPManPara(fv.iValue);
		}
		else if(fv.iValue<fv.iLowerLimit)
		{
			fv.iValue=fv.iLowerLimit;
			getModel()->getDATAHANDLER()->SetCurrentPManPara(fv.iValue);
		}
		if(fv.iValue>getModel()->getDATAHANDLER()->GetCurrentPManualMaxKey())
		{
			m_pcPara_PMANUAL->SetWarning();
		}
		else
		{
			m_pcPara_PMANUAL->SetWarning(false);
		}
		m_pcPara_PMANUAL->SetValue(fv, true);
	}
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_BACKUPpara(bool bPRESET)
{
	fVALUE fv;
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_BACKUP)
	{
		if(bPRESET)
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PRESET()->GetBackupMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PRESET()->GetBackupMaxPara();
			fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetBackupPara();
		}
		else
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetBackupMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetBackupMaxPara();
			fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetBackupPara();
		}
		if(fv.iValue>fv.iUpperLimit)
		{
			fv.iValue=fv.iUpperLimit;
			getModel()->getDATAHANDLER()->SetCurrentBackupPara(fv.iValue);
		}
		else if(fv.iValue<fv.iLowerLimit)
		{
			fv.iValue=fv.iLowerLimit;
			getModel()->getDATAHANDLER()->SetCurrentBackupPara(fv.iValue);
		}
		m_pcPara_BACKUP->SetValue(fv, true);
	}
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_CPAPpara(bool bPRESET)
{
	fVALUE fv;
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_CPAP)
	{
		if(bPRESET)
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PRESET()->GetCPAPMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PRESET()->GetCPAPMaxPara();
			fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetCPAPPara();
		}
		else
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetCPAPMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetCPAPMaxPara();
			fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetCPAPPara();
		}
		if(fv.iValue>fv.iUpperLimit)
		{
			fv.iValue=fv.iUpperLimit;
			getModel()->getDATAHANDLER()->SetCurrentCPAPPara(fv.iValue);
		}
		else if(fv.iValue<fv.iLowerLimit)
		{
			fv.iValue=fv.iLowerLimit;
			getModel()->getDATAHANDLER()->SetCurrentCPAPPara(fv.iValue);
		}
		if(fv.iValue>getModel()->getDATAHANDLER()->GetCurrentCPAPMaxKey())
		{
			m_pcPara_CPAP->SetWarning();
		}
		else
		{
			m_pcPara_CPAP->SetWarning(false);
		}
		m_pcPara_CPAP->SetNameText(getModel()->GetLanguageString(IDS_PARA_CPAP));
		m_pcPara_CPAP->SetValue(fv, true);
	}
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_FLOWMINpara(bool bPRESET)
{
	fVALUE fv;
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_FLOWMIN)
	{
		if(bPRESET)
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PRESET()->GetFlowminMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PRESET()->GetFlowminMaxPara();
			fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetFlowminPara();
		}
		else
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetFlowminMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetFlowminMaxPara();
			fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetFlowminPara();
		}
		if(fv.iValue>fv.iUpperLimit)
		{
			fv.iValue=fv.iUpperLimit;
			getModel()->getDATAHANDLER()->SetFlowminParadata(fv.iValue);
		}
		else if(fv.iValue<fv.iLowerLimit)
		{
			fv.iValue=fv.iLowerLimit;
			getModel()->getDATAHANDLER()->SetFlowminParadata(fv.iValue);
		}
		m_pcPara_FLOWMIN->SetValue(fv, true);
	}
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_BPMNMODEpara(bool bPRESET)
{
	fVALUE fv;
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_BPM)
	{
		if(bPRESET)
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PRESET()->GetBPMNMODEMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PRESET()->GetBPMNMODEMaxPara();
			fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetBPMNMODEPara();
		}
		else
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetBPMNMODEMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetBPMNMODEMaxPara();
			fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetBPMNMODEPara();
		}
		if(fv.iValue>fv.iUpperLimit)
		{
			fv.iValue=fv.iUpperLimit;
			getModel()->getDATAHANDLER()->SetCurrentBPMPara(fv.iValue);
			getModel()->getDATAHANDLER()->CalculateETime_BPMChanged(fv.iValue, false);
		}
		else if(fv.iValue<fv.iLowerLimit)
		{
			fv.iValue=fv.iLowerLimit;
			getModel()->getDATAHANDLER()->SetCurrentBPMPara(fv.iValue);
			getModel()->getDATAHANDLER()->CalculateETime_BPMChanged(fv.iValue, false);
		}
		m_pcPara_BPM->SetNameText(getModel()->GetLanguageString(IDS_PARA_DUOFREQ));
		m_pcPara_BPM->SetNameNoteText(_T(""));
		m_pcPara_BPM->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_DUO_BPM));
		m_pcPara_BPM->SetValue(fv, true);
	}
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_ITIMERECpara(bool bPRESET)
{
	fVALUE fv;
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_ITIME_REC && getModel()->getDATAHANDLER()->isLUNGRECLicenseAvailable())
	{
		if(bPRESET)
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PRESET()->GetITimeRecMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PRESET()->GetITimeRecMaxPara();
			fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetITimeRecPara();
		}
		else
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetITimeRecMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetITimeRecMaxPara();
			fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetITimeRecPara();
		}
		if(fv.iValue>fv.iUpperLimit)
		{
			fv.iValue=fv.iUpperLimit;
			getModel()->getDATAHANDLER()->SetCurrentITimeRecPara(fv.iValue);
		}
		else if(fv.iValue<fv.iLowerLimit)
		{
			fv.iValue=fv.iLowerLimit;
			getModel()->getDATAHANDLER()->SetCurrentITimeRecPara(fv.iValue);
		}
		m_pcPara_ITIME_REC->SetValue(fv, true);
	}
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_ITIMENMODEpara(bool bPRESET)
{
	fVALUE fvITIME;
	fVALUE fvETIME;
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_ITIME)
	{
		if(bPRESET)
		{
			fvITIME.iLowerLimit=getModel()->getDATAHANDLER()->PRESET()->GetITimeNMODEMinPara();
			fvITIME.iUpperLimit=getModel()->getDATAHANDLER()->PRESET()->GetITimeNMODEMaxPara();
			fvITIME.iValue=getModel()->getDATAHANDLER()->PRESET()->GetITimeNMODEPara();
			fvETIME.iValue=getModel()->getDATAHANDLER()->PRESET()->GetETIMENMODEPara();
		}
		else
		{
			fvITIME.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetITimeNMODEMinPara();
			fvITIME.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetITimeNMODEMaxPara();
			fvITIME.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetITimeNMODEPara();
			fvETIME.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetETIMENMODEPara();
		}
		if(fvITIME.iValue>fvITIME.iUpperLimit)
		{
			fvITIME.iValue=fvITIME.iUpperLimit;
			getModel()->getDATAHANDLER()->SetCurrentITimePara(fvITIME.iValue);
			if(getModel()->getCONFIG()->GetIERelationMode()==RM_SET_ItoBPM)
			{
				getModel()->getDATAHANDLER()->CalculateETime_ITimeChanged(fvITIME.iValue,false);
			}
			else
			{
				getModel()->getDATAHANDLER()->CalculateBPM_ITimeChanged(fvITIME.iValue,false);
			}
		}
		else if(fvITIME.iValue<fvITIME.iLowerLimit)
		{
			fvITIME.iValue=fvITIME.iLowerLimit;
			getModel()->getDATAHANDLER()->SetCurrentITimePara(fvITIME.iValue);
			if(getModel()->getCONFIG()->GetIERelationMode()==RM_SET_ItoBPM)
			{
				getModel()->getDATAHANDLER()->CalculateETime_ITimeChanged(fvITIME.iValue,false);
			}
			else
			{
				getModel()->getDATAHANDLER()->CalculateBPM_ITimeChanged(fvITIME.iValue,false);
			}
		}
		getModel()->getDATAHANDLER()->SetCurrentTempETime(fvETIME.iValue);
		getModel()->getDATAHANDLER()->SetCurrentTempITime(fvITIME.iValue);
		m_pcPara_ITIME->SetNameText(getModel()->GetLanguageString(IDS_PARA_DUO_ITIME));
		m_pcPara_ITIME->SetValue(fvITIME, true);
	}
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_ETIMENMODEpara(bool bPRESET)
{
	fVALUE fv;
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_ETIME)
	{
		if(bPRESET)
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PRESET()->GetETIMENMODEMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PRESET()->GetETIMENMODEMaxPara();
			fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetETIMENMODEPara();
		}
		else
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetETIMENMODEMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetETIMENMODEMaxPara();
			fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetETIMENMODEPara();
		}
		if(fv.iValue>fv.iUpperLimit)
		{
			fv.iValue=fv.iUpperLimit;
			getModel()->getDATAHANDLER()->SetCurrentETimePara(fv.iValue);
			getModel()->getDATAHANDLER()->CalculateBPM_ETimeChanged(fv.iValue, false);
		}
		else if(fv.iValue<fv.iLowerLimit)
		{
			fv.iValue=fv.iLowerLimit;
			getModel()->getDATAHANDLER()->SetCurrentETimePara(fv.iValue);
			getModel()->getDATAHANDLER()->CalculateBPM_ETimeChanged(fv.iValue, false);
		}
		getModel()->getDATAHANDLER()->SetCurrentTempETime(fv.iValue);
		m_pcPara_ETIME->SetNameText(getModel()->GetLanguageString(IDS_PARA_DUO_ETIME));
		m_pcPara_ETIME->SetValue(fv, true);
		
	}
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_TRIGGERCONVpara(bool bPRESET)
{
	fVALUE fv;
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_TRIGGERCONV)
	{
		if(bPRESET)
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PRESET()->GetTriggerCONVMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PRESET()->GetTriggerCONVMaxPara();
			fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetTriggerCONVPara();
		}
		else
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetTriggerCONVMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetTriggerCONVMaxPara();
			fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetTriggerCONVPara();
		}
		if(fv.iValue>fv.iUpperLimit)
		{
			fv.iValue=fv.iUpperLimit;
			getModel()->getDATAHANDLER()->SetCurrentTriggerPara(fv.iValue);
		}
		else if(fv.iValue<fv.iLowerLimit)
		{
			fv.iValue=fv.iLowerLimit;
			getModel()->getDATAHANDLER()->SetCurrentTriggerPara(fv.iValue);
		}
		m_pcPara_TRIGGERCONV->SetValue(fv, true);
	}
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_TRIGGERNMODEpara(bool bPRESET)
{
	fVALUE fv;
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_TRIGGERNMODE)
	{
		if(bPRESET)
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PRESET()->GetTriggerNMODEMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PRESET()->GetTriggerNMODEMaxPara();
			fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetTriggerNMODEPara();
		}
		else
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetTriggerNMODEMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetTriggerNMODEMaxPara();
			fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetTriggerNMODEPara();
		}
		if(fv.iValue>fv.iUpperLimit)
		{
			fv.iValue=fv.iUpperLimit;
			getModel()->getDATAHANDLER()->SetCurrentTriggerPara(fv.iValue);
		}
		else if(fv.iValue<fv.iLowerLimit)
		{
			fv.iValue=fv.iLowerLimit;
			getModel()->getDATAHANDLER()->SetCurrentTriggerPara(fv.iValue);
		}
		m_pcPara_TRIGGERNMODE->SetValue(fv, true);
	}
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_ITIMEpara(bool bTrigger,bool bPRESET)
{
	fVALUE fv;
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_ITIME)
	{
		if(bTrigger)
		{
			if(bPRESET)
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PRESET()->GetITimeMinPara_TRIGGER();
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PRESET()->GetITimeMaxPara_TRIGGER();
				fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetITimePara_TRIGGER();
			}
			else
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetITimeMinPara_TRIGGER();
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetITimeMaxPara_TRIGGER();
				fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetITimePara_TRIGGER();
			}
		}
		else
		{
			if(bPRESET)
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PRESET()->GetITimeMinPara_IPPV();
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PRESET()->GetITimeMaxPara_IPPV();
				fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetITimePara_IPPV();
			}
			else
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetITimeMinPara_IPPV();
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetITimeMaxPara_IPPV();
				fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetITimePara_IPPV();
			}
		}
		if(fv.iValue>fv.iUpperLimit)
		{
			fv.iValue=fv.iUpperLimit;
			getModel()->getDATAHANDLER()->SetCurrentITimePara(fv.iValue);
			if(getModel()->getCONFIG()->GetIERelationMode()==RM_SET_ItoBPM)
			{
				getModel()->getDATAHANDLER()->CalculateETime_ITimeChanged(fv.iValue,false);
			}
			else
			{
				getModel()->getDATAHANDLER()->CalculateBPM_ITimeChanged(fv.iValue,false);
			}
		}
		else if(fv.iValue<fv.iLowerLimit)
		{
			fv.iValue=fv.iLowerLimit;
			getModel()->getDATAHANDLER()->SetCurrentITimePara(fv.iValue);
			if(getModel()->getCONFIG()->GetIERelationMode()==RM_SET_ItoBPM)
			{
				getModel()->getDATAHANDLER()->CalculateETime_ITimeChanged(fv.iValue,false);
			}
			else
			{
				getModel()->getDATAHANDLER()->CalculateBPM_ITimeChanged(fv.iValue,false);
			}
		}
		getModel()->getDATAHANDLER()->SetCurrentTempITime(fv.iValue);
		m_pcPara_ITIME->SetNameText(getModel()->GetLanguageString(IDS_PARA_ITIME));
		m_pcPara_ITIME->SetValue(fv, true);
	}
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_ETIMEpara(bool bTrigger,bool bPRESET)
{
	fVALUE fv;
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_ETIME)
	{
		if(bTrigger)
		{
			if(bPRESET)
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PRESET()->GetETIMEMinPara_TRIGGER();
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PRESET()->GetETIMEMaxPara_TRIGGER();
				fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetETIMEPara_TRIGGER();
			}
			else
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetETIMEMinPara_TRIGGER();
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetETIMEMaxPara_TRIGGER();
				fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetETIMEPara_TRIGGER();
			}
		}
		else
		{
			if(bPRESET)
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PRESET()->GetETIMEMinPara_IPPV();
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PRESET()->GetETIMEMaxPara_IPPV();
				fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetETIMEPara_IPPV();
			}
			else
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetETIMEMinPara_IPPV();
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetETIMEMaxPara_IPPV();
				fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetETIMEPara_IPPV();
			}
		}
		if(fv.iValue>fv.iUpperLimit)
		{
			fv.iValue=fv.iUpperLimit;
			getModel()->getDATAHANDLER()->SetCurrentETimePara(fv.iValue);
			getModel()->getDATAHANDLER()->CalculateBPM_ETimeChanged(fv.iValue, false);
		}
		else if(fv.iValue<fv.iLowerLimit)
		{
			fv.iValue=fv.iLowerLimit;
			getModel()->getDATAHANDLER()->SetCurrentETimePara(fv.iValue);
			getModel()->getDATAHANDLER()->CalculateBPM_ETimeChanged(fv.iValue, false);
		}
		m_pcPara_ETIME->SetNameText(getModel()->GetLanguageString(IDS_PARA_ETIME));
		m_pcPara_ETIME->SetValue(fv, true);
		getModel()->getDATAHANDLER()->SetCurrentTempETime(fv.iValue);
	}
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_BPMpara(bool bTrigger,bool bPRESET)
{
	fVALUE fv;
	
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_BPM)
	{
		if(bTrigger)
		{
			if(bPRESET)
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PRESET()->GetBPMMinPara_TRIGGER();
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PRESET()->GetBPMMaxPara_TRIGGER();
				fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetBPMPara_TRIGGER();
			}
			else
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetBPMMinPara_TRIGGER();
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetBPMMaxPara_TRIGGER();
				fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetBPMPara_TRIGGER();
			}
		}
		else
		{
			if(bPRESET)
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PRESET()->GetBPMMinPara_IPPV();
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PRESET()->GetBPMMaxPara_IPPV();
				fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetBPMPara_IPPV();
			}
			else
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetBPMMinPara_IPPV();
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetBPMMaxPara_IPPV();
				fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetBPMPara_IPPV();
			}
		}
		if(fv.iValue>fv.iUpperLimit)
		{
			fv.iValue=fv.iUpperLimit;
			getModel()->getDATAHANDLER()->SetCurrentBPMPara(fv.iValue);
			getModel()->getDATAHANDLER()->CalculateETime_BPMChanged(fv.iValue, false);
		}
		else if(fv.iValue<fv.iLowerLimit)
		{
			fv.iValue=fv.iLowerLimit;
			getModel()->getDATAHANDLER()->SetCurrentBPMPara(fv.iValue);
			getModel()->getDATAHANDLER()->CalculateETime_BPMChanged(fv.iValue, false);
		}
		if(getModel()->getVMODEHANDLER()->modeIsPSVmode())//newVG
		{
			m_pcPara_BPM->SetNameText(_T(""));
			m_pcPara_BPM->SetNameNoteText(getModel()->GetLanguageString(IDS_PARA_FREQBACKUP));
		}
		else if(getModel()->getVMODEHANDLER()->activeModeIsDUOPAP())
		{
			if(getModel()->getDATAHANDLER()->PARADATA()->GetTriggerNMODEPara()!=MAXRANGE_TRIGGER_NMODE_OFF)
			{
				m_pcPara_BPM->SetNameText(_T(""));
				m_pcPara_BPM->SetNameNoteText(getModel()->GetLanguageString(IDS_PARA_FREQBACKUP),true);
			}
			else
			{
				m_pcPara_BPM->SetNameText(getModel()->GetLanguageString(IDS_PARA_FREQ));
				m_pcPara_BPM->SetNameNoteText(_T(""),true);
			}
		}
		else
		{
			m_pcPara_BPM->SetNameText(getModel()->GetLanguageString(IDS_PARA_FREQ));
			m_pcPara_BPM->SetNameNoteText(_T(""));
		}
		
		m_pcPara_BPM->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_BPM));
		m_pcPara_BPM->SetValue(fv, true);
	}
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_PINSPpara(bool bTrigger,bool bPRESET)
{
	fVALUE fv;
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_PINSP)
	{
		if(bTrigger)
		{
			if(bPRESET)
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PRESET()->GetPINSPMinPara_TRIGGER();
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PRESET()->GetPINSPMaxPara_TRIGGER();
				fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetPINSPPara_TRIGGER();
			}
			else
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPINSPMinPara_TRIGGER();
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPINSPMaxPara_TRIGGER();
				fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetPINSPPara_TRIGGER();
			}
		}
		else
		{
			if(bPRESET)
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PRESET()->GetPINSPMinPara_IPPV();
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PRESET()->GetPINSPMaxPara_IPPV();
				fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetPINSPPara_IPPV();
			}
			else
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPINSPMinPara_IPPV();
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPINSPMaxPara_IPPV();
				fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetPINSPPara_IPPV();
			}
		}
		if(fv.iValue>fv.iUpperLimit)
		{
			fv.iValue=fv.iUpperLimit;
			getModel()->getDATAHANDLER()->SetCurrentPINSPPara(fv.iValue);
		}
		else if(fv.iValue<fv.iLowerLimit)
		{
			fv.iValue=fv.iLowerLimit;
			getModel()->getDATAHANDLER()->SetCurrentPINSPPara(fv.iValue);
		}
		if(fv.iValue>getModel()->getDATAHANDLER()->GetCurrentPInspMaxKey())
		{
			m_pcPara_PINSP->SetWarning();
		}
		else
		{
			m_pcPara_PINSP->SetWarning(false);
		}
		m_pcPara_PINSP->SetValue(fv, true);
	}
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_HF_IERatiopara(bool bPRESET)
{
	fVALUE fv;
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_IERatio)
	{
		if(bPRESET)
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PRESET()->GetIERatioMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PRESET()->GetIERatioMaxPara();
			fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetIERatioParaHFO();
		}
		else
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetIERatioMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetIERatioMaxPara();
			fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetIERatioParaHFO();
		}
		if(fv.iValue>fv.iUpperLimit)
		{
			fv.iValue=fv.iUpperLimit;
			getModel()->getDATAHANDLER()->SetCurrentIERatioParaHFO((eRatioIE)fv.iValue);
		}
		else if(fv.iValue<fv.iLowerLimit)
		{
			fv.iValue=fv.iLowerLimit;
			getModel()->getDATAHANDLER()->SetCurrentIERatioParaHFO((eRatioIE)fv.iValue);
		}
		m_pcPara_IERatio->SetValue(fv, true);

	}
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_HF_PMEANRECpara(bool bPRESET)
{
	fVALUE fv;
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_PMEAN_REC)
	{
		if(bPRESET)
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PRESET()->GetHFPMeanRecMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PRESET()->GetHFPMeanRecMaxPara();
			fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetHFPMeanRecPara();
		}
		else
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanRecMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanRecMaxPara();
			fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanRecPara();
		}
		if(fv.iValue>fv.iUpperLimit)
		{
			fv.iValue=fv.iUpperLimit;
			getModel()->getDATAHANDLER()->SetCurrentHFPMeanRecPara(fv.iValue);
		}
		else if(fv.iValue<fv.iLowerLimit)
		{
			fv.iValue=fv.iLowerLimit;
			getModel()->getDATAHANDLER()->SetCurrentHFPMeanRecPara(fv.iValue);
		}
		if(fv.iValue-MAXDIFF_PMEANREC<getModel()->getDATAHANDLER()->GetCurrentPmeanPara())
		{
			fv.iValue=getModel()->getDATAHANDLER()->GetCurrentPmeanPara()+MAXDIFF_PMEANREC;
			getModel()->getDATAHANDLER()->SetCurrentHFPMeanRecPara(fv.iValue);
		}
		m_pcPara_PMEAN_REC->SetValue(fv, true);

	}
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_HFFLOWpara(bool bPRESET)
{
	fVALUE fv;
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_HFFLOW)
	{
		if(bPRESET)
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PRESET()->GetHFFlowMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PRESET()->GetHFFlowMaxPara();
			fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetHFFlowPara();
		}
		else
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFFlowMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFFlowMaxPara();
			fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetHFFlowPara();
		}
		if(fv.iValue>fv.iUpperLimit)
		{
			fv.iValue=fv.iUpperLimit;
			getModel()->getDATAHANDLER()->SetCurrentHFFlowPara(fv.iValue);
		}
		else if(fv.iValue<fv.iLowerLimit && fv.iValue!=0)
		{
			fv.iValue=fv.iLowerLimit;
			getModel()->getDATAHANDLER()->SetCurrentHFFlowPara(fv.iValue);
		}
		m_pcPara_HFFLOW->SetValue(fv, true);

	}
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_HFFREQpara(bool bPRESET)
{
	fVALUE fv;
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_HFFREQ)
	{
		if(bPRESET)
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PRESET()->GetHFFreqMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PRESET()->GetHFFreqMaxPara();
			fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetHFFreqPara();
		}
		else
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFFreqMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFFreqMaxPara();
			fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetHFFreqPara();
		}
		if(fv.iValue>fv.iUpperLimit)
		{
			fv.iValue=fv.iUpperLimit;
			getModel()->getDATAHANDLER()->SetCurrentHFFreqPara(fv.iValue);
		}
		else if(fv.iValue<fv.iLowerLimit)
		{
			fv.iValue=fv.iLowerLimit;
			getModel()->getDATAHANDLER()->SetCurrentHFFreqPara(fv.iValue);
		}
		m_pcPara_HFFREQ->SetValue(fv, true);
		
	}
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_HFAMPLpara(bool bPRESET)
{
	fVALUE fv;
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_HFAMPL)
	{
		//WORD iHFampMaxKey=0;
		if(bPRESET)
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PRESET()->GetHFAMPLMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PRESET()->GetHFAMPLMaxPara();
			fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetHFAMPLPara();
			//iHFampMaxKey=getModel()->getDATAHANDLER()->PRESET()->GetHFampMaxKey();
		}
		else
		{
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLMaxPara();
			fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLPara();
			//iHFampMaxKey=getModel()->getDATAHANDLER()->PARADATA()->GetHFampMaxKey();
		}
		if(fv.iValue>fv.iUpperLimit)
		{
			fv.iValue=fv.iUpperLimit;
			getModel()->getDATAHANDLER()->SetCurrentHFAMPLPara(fv.iValue);
		}
		else if(fv.iValue<fv.iLowerLimit)
		{
			fv.iValue=fv.iLowerLimit;
			getModel()->getDATAHANDLER()->SetCurrentHFAMPLPara(fv.iValue);
		}
		/*if(fv.iValue>iHFampMaxKey)
		{
			m_pcPara_HFAMPL->SetWarning();
		}
		else
		{
			m_pcPara_HFAMPL->SetWarning(false);
		}*/
		m_pcPara_HFAMPL->SetValue(fv, true);
	}
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_HF_PMAXpara(bool bPRESET)
{
	fVALUE fv;
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_AMPMAXI)
	{
		if(bPRESET)
		{
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PRESET()->GetHFAMPLMaxPara();
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PRESET()->GetHFAMPLMinPara();
			fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetHFAMPLmaxPara();
		}
		else
		{
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLMaxPara();
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLMinPara();
			fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLmaxPara();
		}
		if(fv.iValue>fv.iUpperLimit)
		{
			fv.iValue=fv.iUpperLimit;
			getModel()->getDATAHANDLER()->SetCurrentPmaxVolGPara(fv.iValue);
		}
		else if(fv.iValue<fv.iLowerLimit)
		{
			fv.iValue=fv.iLowerLimit;
			getModel()->getDATAHANDLER()->SetCurrentPmaxVolGPara(fv.iValue);
		}
		
		m_pcPara_AMPMAXI->SetValue(fv, true);
	}
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_PPSVpara(bool bSIMV, bool bPRESET)
{
	fVALUE fv;
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_PPSV)
	{
		if(bSIMV)
		{
			if(bPRESET)
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PRESET()->GetPpsvMinPara();
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PRESET()->GetPpsvMaxPara();
				SHORT iPINSP=getModel()->getDATAHANDLER()->PRESET()->GetPINSPPara_TRIGGER();
				if(getModel()->getDATAHANDLER()->PRESET()->GetPpsvPara()>iPINSP)
				{
					getModel()->getDATAHANDLER()->PRESET()->SetPpsvPara(iPINSP,false,false);
				}
				SHORT iPEEP=getModel()->getDATAHANDLER()->PRESET()->GetPEEPPara_TRIGGER();
				if(getModel()->getDATAHANDLER()->PRESET()->GetPpsvPara()<iPEEP+20)
				{
					getModel()->getDATAHANDLER()->PRESET()->SetPpsvPara(iPEEP+20,false,false);
				}
				fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetPpsvPara();
			}
			else
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPpsvMinPara();
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPpsvMaxPara();

				SHORT iPINSP=getModel()->getDATAHANDLER()->PRESET()->GetPINSPPara_TRIGGER();
				iPINSP=getModel()->getDATAHANDLER()->PARADATA()->GetPINSPPara_TRIGGER();
				if(getModel()->getDATAHANDLER()->PARADATA()->GetPpsvPara()>iPINSP)
				{
					getModel()->getDATAHANDLER()->SetPpsvParadata(iPINSP);
				}

				SHORT iPEEP=getModel()->getDATAHANDLER()->PARADATA()->GetPEEPPara_TRIGGER();
				if(getModel()->getDATAHANDLER()->PARADATA()->GetPpsvPara()<iPEEP+20)
				{
					getModel()->getDATAHANDLER()->SetPpsvParadata(iPEEP+20);
				}

				fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetPpsvPara();
			}
		}
		else
		{
			if(bPRESET)
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PRESET()->GetPpsvMinPara();
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PRESET()->GetPpsvMaxPara();

				SHORT iPINSP=getModel()->getDATAHANDLER()->PRESET()->GetPINSPPara_TRIGGER();//newVG
				if(getModel()->getDATAHANDLER()->PRESET()->GetPpsvPara()>iPINSP)
				{
					getModel()->getDATAHANDLER()->PRESET()->SetPpsvPara(iPINSP,false,false);
				}
				if(getModel()->getDATAHANDLER()->PRESET()->GetPpsvPara()<(20+getModel()->getDATAHANDLER()->PRESET()->GetPEEPPara_TRIGGER()))
				{
					getModel()->getDATAHANDLER()->SetCurrentPpsvPara((20+getModel()->getDATAHANDLER()->PRESET()->GetPEEPPara_TRIGGER()));
				}
				fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetPpsvPara();
			}
			else
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPpsvMinPara();
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPpsvMaxPara();

				SHORT iPINSP=getModel()->getDATAHANDLER()->PARADATA()->GetPINSPPara_TRIGGER();//newVG
				if(getModel()->getDATAHANDLER()->PARADATA()->GetPpsvPara()>iPINSP)
				{
					getModel()->getDATAHANDLER()->SetPpsvParadata(iPINSP);
				}

				if(getModel()->getDATAHANDLER()->PARADATA()->GetPpsvPara()<(20+getModel()->getDATAHANDLER()->PARADATA()->GetPEEPPara_TRIGGER()))
				{
					getModel()->getDATAHANDLER()->SetCurrentPpsvPara((20+getModel()->getDATAHANDLER()->PARADATA()->GetPEEPPara_TRIGGER()));
				}
				fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetPpsvPara();
			}
		}

		if(fv.iValue>fv.iUpperLimit)
		{
			fv.iValue=fv.iUpperLimit;
			getModel()->getDATAHANDLER()->SetCurrentPpsvPara(fv.iValue);
		}
		else if(fv.iValue<fv.iLowerLimit)
		{
			fv.iValue=fv.iLowerLimit;
			getModel()->getDATAHANDLER()->SetCurrentPpsvPara(fv.iValue);
		}
		if(fv.iValue>getModel()->getDATAHANDLER()->GetCurrentPpsvMaxKey())
		{
			m_pcPara_PPSV->SetWarning();
		}
		else
		{
			m_pcPara_PPSV->SetWarning(false);
		}

		m_pcPara_PPSV->SetValue(fv, true);
	}
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_PMAXpara(bool bTrigger,bool bPRESET)
{
	fVALUE fv;
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_PMAXI)
	{
		if(bTrigger)
		{
			if(bPRESET)
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PRESET()->GetPMAXMinPara_TRIGGER();
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PRESET()->GetPMAXMaxPara_TRIGGER();
				fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetPmaxVolGPara_TRIGGER();
			}
			else
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPMAXMinPara_TRIGGER();
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPMAXMaxPara_TRIGGER();
				fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetPmaxVolGPara_TRIGGER();
			}
		}
		else
		{
			if(bPRESET)
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PRESET()->GetPMAXMinPara_IPPV();
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PRESET()->GetPMAXMaxPara_IPPV();
				fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetPmaxVolGPara_IPPV();
			}
			else
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPMAXMinPara_IPPV();
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPMAXMaxPara_IPPV();
				fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetPmaxVolGPara_IPPV();
			}
		}
		if(fv.iValue>fv.iUpperLimit)
		{
			fv.iValue=fv.iUpperLimit;
			getModel()->getDATAHANDLER()->SetCurrentPmaxVolGPara(fv.iValue);
		}
		else if(fv.iValue<fv.iLowerLimit)
		{
			fv.iValue=fv.iLowerLimit;
			getModel()->getDATAHANDLER()->SetCurrentPmaxVolGPara(fv.iValue);
		}

		if(fv.iValue<getModel()->getDATAHANDLER()->GetCurrentPEEPPara()+DIFF_PEEP_PINSP && getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VGARANT 
			&& getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_ON)//rku, PMAX check
		{
			if(getModel()->getDATAHANDLER()->GetCurrentPEEPPara()%10==0)
			{
				fv.iValue=getModel()->getDATAHANDLER()->GetCurrentPEEPPara()+DIFF_PEEP_PINSP;
			}
			else
			{
				fv.iValue=getModel()->getDATAHANDLER()->GetCurrentPEEPPara()+DIFF_PEEP_PINSP+5;
			}

			getModel()->getDATAHANDLER()->SetCurrentPmaxVolGPara(fv.iValue);
		}

		if(fv.iValue>getModel()->getDATAHANDLER()->GetCurrentPInspMaxKey())
		{
			m_pcPara_PMAXI->SetWarning();
		}
		else
		{
			m_pcPara_PMAXI->SetWarning(false);
		}
		m_pcPara_PMAXI->SetValue(fv, true);

	}
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_PEEPpara(bool bTrigger,bool bPRESET)
{
	fVALUE fv;
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_PEEP)
	{
		if(bTrigger)
		{
			if(bPRESET)
			{
				if(getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_LINEAR)
				{
					fv.iLowerLimit=getModel()->getDATAHANDLER()->PRESET()->GetPEEPMinPara_RISETIME();
				}
				else
				{
					fv.iLowerLimit=getModel()->getDATAHANDLER()->PRESET()->GetPEEPMinPara_TRIGGER();
				}
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PRESET()->GetPEEPMaxPara_TRIGGER();
				fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetPEEPPara_TRIGGER();
			}
			else
			{
				if(getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_LINEAR)
				{
					fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPEEPMinPara_RISETIME();
				}
				else
				{
					fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPEEPMinPara_TRIGGER();
				}
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPEEPMaxPara_TRIGGER();
				fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetPEEPPara_TRIGGER();
			}
		}
		else
		{
			if(bPRESET)
			{
				if(getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_LINEAR)
				{
					fv.iLowerLimit=getModel()->getDATAHANDLER()->PRESET()->GetPEEPMinPara_RISETIME();
				}
				else
				{
					fv.iLowerLimit=getModel()->getDATAHANDLER()->PRESET()->GetPEEPMinPara_IPPV();
				}
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PRESET()->GetPEEPMaxPara_IPPV();
				fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetPEEPPara_IPPV();
			}
			else
			{
				if(getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_LINEAR)
				{
					fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPEEPMinPara_RISETIME();
				}
				else
				{
					fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPEEPMinPara_IPPV();
				}
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPEEPMaxPara_IPPV();
				fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetPEEPPara_IPPV();
			}
		}

		if(fv.iValue>fv.iUpperLimit)
		{
			fv.iValue=fv.iUpperLimit;
			getModel()->getDATAHANDLER()->SetCurrentPEEPPara(fv.iValue);
		}
		else if(fv.iValue<fv.iLowerLimit)
		{
			fv.iValue=fv.iLowerLimit;
			getModel()->getDATAHANDLER()->SetCurrentPEEPPara(fv.iValue);
		}
		if(fv.iValue>getModel()->getDATAHANDLER()->GetCurrentPeepMaxKey())
		{
			m_pcPara_PEEP->SetWarning();
		}
		else if(fv.iValue<getModel()->getDATAHANDLER()->GetCurrentPeepMinKey())
		{
			m_pcPara_PEEP->SetWarning();
		}
		else
		{
			m_pcPara_PEEP->SetWarning(false);
		}
		m_pcPara_PEEP->SetValue(fv, true);
	}
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
	
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_RISETIMEpara(bool bTrigger,bool bPRESET)
{
	fVALUE fv;
	int iITIME=0;

	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_RISETIME)
	{
		if(bTrigger)
		{
			if(bPRESET)
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PRESET()->GetRisetimeMinPara_TRIGGER();
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PRESET()->GetRisetimeMaxPara_TRIGGER();
				fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetRisetimePara_TRIGGER();
				iITIME= getModel()->getDATAHANDLER()->PRESET()->GetITimePara_TRIGGER();
			}
			else
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetRisetimeMinPara_TRIGGER();
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetRisetimeMaxPara_TRIGGER();
				fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetRisetimePara_TRIGGER();
				iITIME= getModel()->getDATAHANDLER()->PARADATA()->GetITimePara_TRIGGER();
			}
		}
		else
		{
			if(bPRESET)
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PRESET()->GetRisetimeMinPara_IPPV();
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PRESET()->GetRisetimeMaxPara_IPPV();
				fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetRisetimePara_IPPV();
				iITIME= getModel()->getDATAHANDLER()->PRESET()->GetITimePara_IPPV();
			}
			else
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetRisetimeMinPara_IPPV();
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetRisetimeMaxPara_IPPV();
				fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetRisetimePara_IPPV();
				iITIME= getModel()->getDATAHANDLER()->PARADATA()->GetITimePara_IPPV();
			}
		}

		if(fv.iValue>fv.iUpperLimit)
		{
			fv.iValue=fv.iUpperLimit;
			getModel()->getDATAHANDLER()->SetCurrentRisetimePara(fv.iValue);
		}
		else if(fv.iValue<fv.iLowerLimit)
		{
			fv.iValue=fv.iLowerLimit;
			getModel()->getDATAHANDLER()->SetCurrentRisetimePara(fv.iValue);
		}
		else if(fv.iValue>iITIME)
		{
			fv.iValue=iITIME;
			getModel()->getDATAHANDLER()->SetCurrentRisetimePara(fv.iValue);
		}
		m_pcPara_RISETIME->SetValue(fv, true);
	}
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::load_IFLOWpara(bool bTrigger,bool bPRESET)
{
	fVALUE fv;

	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_IFLOW)
	{
		if(bTrigger)
		{
			if(bPRESET)
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PRESET()->GetIFlowMinPara_TRIGGER();
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PRESET()->GetIFlowMaxPara_TRIGGER();
				fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetIFlowPara_TRIGGER();
			}
			else
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetIFlowMinPara_TRIGGER();
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetIFlowMaxPara_TRIGGER();
				fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetIFlowPara_TRIGGER();
			}
		}
		else
		{
			if(bPRESET)
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PRESET()->GetIFlowMinPara_IPPV();
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PRESET()->GetIFlowMaxPara_IPPV();
				fv.iValue=getModel()->getDATAHANDLER()->PRESET()->GetIFlowPara_IPPV();
			}
			else
			{
				fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetIFlowMinPara_IPPV();
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetIFlowMaxPara_IPPV();
				fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetIFlowPara_IPPV();
			}
		}

		if(fv.iValue>fv.iUpperLimit)
		{
			fv.iValue=fv.iUpperLimit;
			getModel()->getDATAHANDLER()->SetCurrentIFlowPara(fv.iValue);
		}
		else if(fv.iValue<fv.iLowerLimit)
		{
			fv.iValue=fv.iLowerLimit;
			getModel()->getDATAHANDLER()->SetCurrentIFlowPara(fv.iValue);
		}
		if(fv.iValue>getModel()->getDATAHANDLER()->GetCurrentIFlowMaxKey())
		{
			m_pcPara_IFLOW->SetWarning();
		}
		else
		{
			m_pcPara_IFLOW->SetWarning(false);
		}
		m_pcPara_IFLOW->SetValue(fv, true);
	}
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
	
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::Hide()
{
	this->ShowWindow(SW_HIDE);
	ShowWndMenuVentState(false);

}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::HideParaBtn()
{
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_RISETIME)
		m_pcPara_RISETIME->ShowWindow(SW_HIDE);
	if(m_pcPara_IFLOW)
		m_pcPara_IFLOW->ShowWindow(SW_HIDE);
	/*if(m_pcPara_EFLOW)
		m_pcPara_EFLOW->ShowWindow(SW_HIDE);*/
	if(m_pcPara_PEEP)
		m_pcPara_PEEP->ShowWindow(SW_HIDE);
	if(m_pcPara_PINSP)
		m_pcPara_PINSP->ShowWindow(SW_HIDE);
	if(m_pcPara_PDUO)
		m_pcPara_PDUO->ShowWindow(SW_HIDE);
	if(m_pcPara_PMANUAL)
		m_pcPara_PMANUAL->ShowWindow(SW_HIDE);
	if(m_pcPara_VLIMIT)
		m_pcPara_VLIMIT->ShowWindow(SW_HIDE);
	if(m_pcPara_TRIGGERCONV)
		m_pcPara_TRIGGERCONV->ShowWindow(SW_HIDE);
	if(m_pcPara_TRIGGERNMODE)
		m_pcPara_TRIGGERNMODE->ShowWindow(SW_HIDE);
	if(m_pcPara_BPM)
		m_pcPara_BPM->ShowWindow(SW_HIDE);
	if(m_pcPara_ITIME)
		m_pcPara_ITIME->ShowWindow(SW_HIDE);
	if(m_pcPara_ETIME)
		m_pcPara_ETIME->ShowWindow(SW_HIDE);
	if(m_pcPara_O2)
		m_pcPara_O2->ShowWindow(SW_HIDE);
	if(m_pcPara_VGARANT)
		m_pcPara_VGARANT->ShowWindow(SW_HIDE);
	if(m_pcPara_PMAXI)
		m_pcPara_PMAXI->ShowWindow(SW_HIDE);
	if(m_pcPara_AMPMAXI)
		m_pcPara_AMPMAXI->ShowWindow(SW_HIDE);
	if(m_pcPara_OnOff)
		m_pcPara_OnOff->ShowWindow(SW_HIDE);
	if(m_pcPara_BACKUP)
		m_pcPara_BACKUP->ShowWindow(SW_HIDE);
	if(m_pcPara_CPAP)
		m_pcPara_CPAP->ShowWindow(SW_HIDE);
	if(m_pcPara_FLOWMIN)
		m_pcPara_FLOWMIN->ShowWindow(SW_HIDE);
	if(m_pcPara_FLOW)
		m_pcPara_FLOW->ShowWindow(SW_HIDE);
	if(m_pcPara_O2Flush)
		m_pcPara_O2Flush->ShowWindow(SW_HIDE);
	if(m_pcPara_PMITT)
		m_pcPara_PMITT->ShowWindow(SW_HIDE);
	if(m_pcPara_PPSV)
		m_pcPara_PPSV->ShowWindow(SW_HIDE);
	if(m_pcPara_HFAMPL)
		m_pcPara_HFAMPL->ShowWindow(SW_HIDE);
	if(m_pcPara_HFFREQ)
		m_pcPara_HFFREQ->ShowWindow(SW_HIDE);
	if(m_pcPara_IERatio)
		m_pcPara_IERatio->ShowWindow(SW_HIDE);
	if(m_pcPara_ITIME_REC)
		m_pcPara_ITIME_REC->ShowWindow(SW_HIDE);
	/*if(m_pcPara_FREQ_REC)
		m_pcPara_FREQ_REC->ShowWindow(SW_HIDE);*/
	if(m_pcPara_PMEAN_REC)
		m_pcPara_PMEAN_REC->ShowWindow(SW_HIDE);
	if(m_pcPara_HFFLOW)
		m_pcPara_HFFLOW->ShowWindow(SW_HIDE);

	if(m_pcSubPara_TrigThreshold)
		m_pcSubPara_TrigThreshold->ShowWindow(SW_HIDE);
	if(m_pcSubPara_LEAKCOMP) 
		m_pcSubPara_LEAKCOMP->ShowWindow(SW_HIDE);
	if(m_pcSubPara_BIASFLOW) 
		m_pcSubPara_BIASFLOW->ShowWindow(SW_HIDE);
	if(m_pcSubPara_IE)
		m_pcSubPara_IE->ShowWindow(SW_HIDE);
	if(m_pcSubPara_DemandFlow)
		m_pcSubPara_DemandFlow->ShowWindow(SW_HIDE);
	if(m_pcSubPara_ETime)
		m_pcSubPara_ETime->ShowWindow(SW_HIDE);
	if(m_pcSubPara_PSVTIME)
		m_pcSubPara_PSVTIME->ShowWindow(SW_HIDE);
	if(m_pcSubPara_BPM)
		m_pcSubPara_BPM->ShowWindow(SW_HIDE);
	if(m_pcSubPara_O2act)
		m_pcSubPara_O2act->ShowWindow(SW_HIDE);
	if(m_pcSubPara_O2Flush)
		m_pcSubPara_O2Flush->ShowWindow(SW_HIDE);
	/*if(m_pcSubPara_TVI)
		m_pcSubPara_TVI->ShowWindow(SW_HIDE);*/
	/*if(m_pcSubPara_VG_VL)
		m_pcSubPara_VG_VL->ShowWindow(SW_HIDE);*/
	if(m_pcPara_OnOff)
		m_pcPara_OnOff->ShowWindow(SW_HIDE);
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}

// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::SetOneButtonDepressed(int btnID)
{
	//csView.Lock();
	POSITION pos;
	try
	{
		/** \brief The enter critical section. */
		EnterCriticalSection(&csDraw);
		for( pos = m_plParaBtn.GetHeadPosition(); pos != NULL; )
		{
			CParaBtn* pParaBtn = ( CParaBtn* ) m_plParaBtn.GetNext( pos );
			if(pParaBtn->GetBtnId() != btnID)
			{
				if(pParaBtn->GetButton())
				{
					pParaBtn->DrawDirectUp(false);
				}
			}
		}
		/** \brief The leave critical section. */
		LeaveCriticalSection(&csDraw);
	}
	catch (...)
	{
		/** \brief The leave critical section. */
		LeaveCriticalSection(&csDraw);

		theApp.ReportException(_T("CViewParaBtn::SetOneButtonDepressed"));
	}
}

// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::SetAllButtonUnpressed()
{
	//csView.Lock();
	POSITION pos;
	try
	{
		/** \brief The enter critical section. */
		EnterCriticalSection(&csDraw);
		for( pos = m_plParaBtn.GetHeadPosition(); pos != NULL; )
		{
			CParaBtn* pParaBtn = ( CParaBtn* ) m_plParaBtn.GetNext( pos );
			if(pParaBtn->IsDepressed())
			{
				pParaBtn->Depress();
				break;
			}

		}
		/** \brief The leave critical section. */
		LeaveCriticalSection(&csDraw);
	}
	catch (...)
	{
		/** \brief The leave critical section. */
		LeaveCriticalSection(&csDraw);

		theApp.ReportException(_T("CViewParaBtn::SetAllButtonUnpressed"));
	}
}

// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::NotifyVentModeChanged()
{
	m_bViewPraBtnShown=false;
	ShowParaBtn(true);
	m_bViewPraBtnShown=true;
}

// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::NotifyViewFocusChanged(int iViewID)
{
	if(iViewID!=IDC_VIEW_PARABTN)
	{
		KillTimer(PARATIMER);
	}
}


// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,m_hDC,0,0,SRCCOPY);
}

void CViewParaBtn::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==PARATIMER)
	{
		KillTimer(PARATIMER);
		SetAllButtonUnpressed();

		/** \brief The enter critical section. */
		EnterCriticalSection(&csDraw);
		if(m_pcPara_PPSV)
			m_pcPara_PPSV->SetArrowDown(false);
		if(m_pcPara_PEEP)
			m_pcPara_PEEP->SetArrowDown(false);
		if(m_pcPara_CPAP)
			m_pcPara_CPAP->SetArrowDown(false);
		if(m_pcPara_PMANUAL)
			m_pcPara_PMANUAL->SetArrowDown(false);
		if(m_pcPara_PDUO)
			m_pcPara_PDUO->SetArrowDown(false);
		if(m_pcPara_ITIME_REC)
			m_pcPara_ITIME_REC->SetArrowDown(false);
	
		if(getModel()->getALARMHANDLER()->ALARM_SysLimit_Vlimitted->getAlarmState()==AS_NONE)
		{
			//I-Flow Arrow up
			if(m_pcPara_PINSP)
				m_pcPara_PINSP->SetAlarmArrowDown(false);
		}
		if(m_pcPara_PPSV)
			m_pcPara_PPSV->SetArrowUp(false);
		if(m_pcPara_PEEP)
			m_pcPara_PEEP->SetArrowUp(false);
		if(m_pcPara_CPAP)
			m_pcPara_CPAP->SetArrowUp(false);
		if(m_pcPara_PMANUAL)
			m_pcPara_PMANUAL->SetArrowUp(false);
		if(m_pcPara_PDUO)
			m_pcPara_PDUO->SetArrowUp(false);

		bool bSetIFlowArrow=false;

		if(getModel()->getALARMHANDLER()->ALARM_SysLimit_Vgaranty->getAlarmState()==AS_NONE)
		{
			if(m_pcPara_PPSV)
				m_pcPara_PPSV->SetAlarmArrowUp(false);
			if(m_pcPara_PMAXI)
				m_pcPara_PMAXI->SetAlarmArrowUp(false);
			if(m_pcPara_AMPMAXI)
				m_pcPara_AMPMAXI->SetAlarmArrowUp(false);
		}
		else
		{
			int iState=getModel()->getDATAHANDLER()->getStatus2();
			if(iState&BIT6)
				bSetIFlowArrow=true;
		}
		if(getModel()->getALARMHANDLER()->ALARM_SysLimit_Pinsp_NotReached->getAlarmState()!=AS_NONE)
		{
			bSetIFlowArrow=true;
		}

		if(!bSetIFlowArrow)
		{
			if(m_pcPara_IFLOW)
				m_pcPara_IFLOW->SetAlarmArrowUp(false);
		}
		/** \brief The leave critical section. */
		LeaveCriticalSection(&csDraw);
		
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->SetFocus();
	}
	

	CWnd::OnTimer(nIDEvent);
}


LRESULT CViewParaBtn::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
{
	int iID = 0;
	switch(message)
	{
	case WM_SHOWPARABTN:
		{
			//rku cs1
			m_bViewPraBtnShown=false;
			ShowParaBtn();
			m_bViewPraBtnShown=true;
			return 1;
		}
	case WM_REFRESHOXIFLUSH:
		{
			//rku cs1
			refreshO2Flush();
			return 1;
		}
	case WM_OXIFLUSHTIME:
		{
			//rku cs1
			SetO2FlushTime((int)wParam);
			return 1;
		}
	case WM_SETVIEWFOCUS:
		{
			//rku cs1
			SetViewFocus();
			return 1;
		}
		break;
	case WM_SETNEXTFOCUS:
		{
			//rku cs1
			SetNextFocus();
			return 1;
		}
		break;
	case WM_SETPREVFOCUS:
		{
			//rku cs1
			SetPrevFocus();
			return 1;
		}
		break;
	case WM_SETOXYHOURGLASS_TRUE:
		{
			//rku cs1
			SetOxyHourglass(true);
			return 1;
		}
		break;
	case WM_SETOXYHOURGLASS_FALSE:
		{
			//rku cs1
			SetOxyHourglass(false);
			return 1;
		}
		break;
	case WM_SETPSVAPNOE_TRUE:
		{
			//rku cs1
			setPSVapnoe(true);
			return 1;
		}
		break;
	case WM_SETPSVAPNOE_FALSE:
		{
			//rku cs1
			setPSVapnoe(false);
			return 1;
		}
		break;
	case WM_SETTRIGGEREDBREATH:
		{
			//rku cs1
			SetTriggeredBreath();
			return 1;
		}
		break;
	case WM_SETPRICO_TRUE:
		{
			//rku cs1
			setPRICOrunning(true);
			return 1;
		}
		break;
	case WM_SETPRICO_FALSE:
		{
			//rku cs1
			setPRICOrunning(false);
			return 1;
		}
		break;
	case WM_REDRAW_VIEW:
		{
			m_bViewPraBtnShown=false;
			ShowParaBtn(false);
			m_bViewPraBtnShown=true;
		}
		break;
	case WM_MENUBUTTONDOWN:
		{
			switch(wParam)
			{
			case IDC_BTN_PARA_ONOFF:
				{
					if(getModel()->getCONFIG()->GetCurMode()==VM_HFO)
					{
						if(getModel()->getDATAHANDLER()->IsCurrentModeVGarantStateOn()==true)
						{
							getModel()->getDATAHANDLER()->ChangeVgarantState(VOLUMEGAR_OFF);
						}
						else
						{
							getModel()->getDATAHANDLER()->ChangeVgarantState(VOLUMEGAR_ON);
						}
					}
					else if(getModel()->getCONFIG()->GetCurMode()==VM_PRE_HFO)
					{
						if(getModel()->getDATAHANDLER()->IsCurrentModeVGarantStateOn()==true)
						{
							getModel()->getDATAHANDLER()->ChangeVgarantState(VOLUMEGAR_PRESET_OFF);
						}
						else
						{
							getModel()->getDATAHANDLER()->ChangeVgarantState(VOLUMEGAR_PRESET_ON);
						}
					}
					else if(		getModel()->getDATAHANDLER()->isVGUARANTLicenseAvailable() 
						&&	getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VGARANT)
					{
						if(getModel()->getDATAHANDLER()->IsCurrentModeVGarantStateOn()==true)
						{
							if(getModel()->getCONFIG()->CurModeIsPresetMode())
								getModel()->getDATAHANDLER()->ChangeVgarantState(VOLUMEGAR_PRESET_OFF);
							else
								getModel()->getDATAHANDLER()->ChangeVgarantState(VOLUMEGAR_OFF);
						}
						else
						{
							if(getModel()->getCONFIG()->CurModeIsPresetMode())
								getModel()->getDATAHANDLER()->ChangeVgarantState(VOLUMEGAR_PRESET_ON);
							else
								getModel()->getDATAHANDLER()->ChangeVgarantState(VOLUMEGAR_ON);
						}
					}
					else if(		getModel()->getDATAHANDLER()->isVLIMITLicenseAvailable() 
						&&	getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VLIMIT)
					{
						if(getModel()->getDATAHANDLER()->IsCurrentModeVLimitStateOn()==true)
						{
							/*if(getModel()->getCONFIG()->CurModeIsPresetMode())
								getModel()->getDATAHANDLER()->ChangeVgarantState(VOLUMEGAR_PRESET_OFF);
							else
								getModel()->getDATAHANDLER()->ChangeVgarantState(VOLUMEGAR_OFF);*/
							getModel()->getDATAHANDLER()->SetCurrentVLimitParam(getModel()->getDATAHANDLER()->GetCurrentVLimitPara(),false,true);
						}
						else
						{
							/*f(getModel()->getCONFIG()->CurModeIsPresetMode())
								getModel()->getDATAHANDLER()->ChangeVgarantState(VOLUMEGAR_PRESET_ON);
							else
								getModel()->getDATAHANDLER()->ChangeVgarantState(VOLUMEGAR_ON);*/
							getModel()->getDATAHANDLER()->SetCurrentVLimitParam(getModel()->getDATAHANDLER()->GetCurrentVLimitPara(),true,true);
						}
					}

					if(getModel()->getFOTThread())
					{
						if(getModel()->getFOTThread()->getFOTstate()!=FOT_OFF)
						{
							if(AfxGetApp() != NULL)
								AfxGetApp()->GetMainWnd()->PostMessage(WM_STOP_FOT);
						}
					}

					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->SetFocus();

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
			case IDC_BTN_PARAMENU:
				{
					if(!m_bSettingsActive)
						m_bSettingsActive=true;
					Show();
					return 1;
				}
				break;
			case IDC_BTN_PARAMENUBCK:
				{
					if(m_bSettingsActive)
						m_bSettingsActive=false;
					Show();
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
	case WM_DATA_CHANGED:
		{
			int iValue;
			
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);

			if(		m_pcSubPara_O2act 
				&&	m_pcSubPara_O2act->IsWindowVisible() 
				&&	getModel()->getDATAHANDLER()->GetOxyCalRunning()==false
				&& m_bViewPraBtnShown)
			{
				iValue=getModel()->getDATAHANDLER()->getMessureDataO2();
				m_pcSubPara_O2act->SetParaValue(iValue);
			}

			if(		m_pcSubPara_TrigThreshold 
				&&	getModel()->getVMODEHANDLER()->activeModeIsCONVTrigger()
				&&	m_bViewPraBtnShown)
			{
				if(getModel()->getDATAHANDLER()->getTriggerOptionCONV()==TRIGGER_PRESSURE)
				{
					m_pcSubPara_TrigThreshold->SetNameText(_T(""));
					m_pcSubPara_TrigThreshold->SetUnitText(_T(""));

					m_pcSubPara_TrigThreshold->SetParaValue(1);
				}
				else
				{
					if(getModel()->getDATAHANDLER()->getTriggerOptionCONV()==TRIGGER_VOLUME)
					{
						m_pcSubPara_TrigThreshold->SetNameText(getModel()->GetLanguageString(IDS_PARA_TRIGVOL));
						m_pcSubPara_TrigThreshold->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_MILLILITER));
					}
					else if(getModel()->getDATAHANDLER()->getTriggerOptionCONV()==TRIGGER_FLOW)
					{
						m_pcSubPara_TrigThreshold->SetNameText(getModel()->GetLanguageString(IDS_PARA_TRIGFLOW));
						m_pcSubPara_TrigThreshold->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_LMIN));
					}

					iValue=getModel()->getDATAHANDLER()->getAVGMessureDataTrigger();
					m_pcSubPara_TrigThreshold->SetParaValue(iValue);
				}
			}
			else if(		m_pcSubPara_TrigThreshold 
				&&	getModel()->getVMODEHANDLER()->activeModeIsNMODETrigger()
				&&	m_bViewPraBtnShown)
			{
				if(getModel()->getDATAHANDLER()->getTriggerOptionNMODE()==TRIGGER_PRESSURE)
				{
					m_pcSubPara_TrigThreshold->SetNameText(_T(""));
					m_pcSubPara_TrigThreshold->SetUnitText(_T(""));

					m_pcSubPara_TrigThreshold->SetParaValue(1);
				}
				else
				{
					if(getModel()->getDATAHANDLER()->getTriggerOptionNMODE()==TRIGGER_VOLUME)
					{
						m_pcSubPara_TrigThreshold->SetNameText(getModel()->GetLanguageString(IDS_PARA_TRIGVOL));
						m_pcSubPara_TrigThreshold->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_MILLILITER));
					}
					else if(getModel()->getDATAHANDLER()->getTriggerOptionNMODE()==TRIGGER_FLOW)
					{
						m_pcSubPara_TrigThreshold->SetNameText(getModel()->GetLanguageString(IDS_PARA_TRIGFLOW));
						m_pcSubPara_TrigThreshold->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_LMIN));
					}

					iValue=getModel()->getDATAHANDLER()->getAVGMessureDataTrigger();
					m_pcSubPara_TrigThreshold->SetParaValue(iValue);
				}
			}

			if(		getModel()->getCONFIG()->GetCurMode()==VM_PSV
				||	getModel()->getCONFIG()->GetCurMode()==VM_SIMVPSV
				||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_PSV
				||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_SIMVPSV)
			{
				if(		m_pcSubPara_IE 
					&&	m_pcSubPara_IE->IsWindowVisible()
					&&	m_bViewPraBtnShown)
				{
					iValue=getModel()->getDATAHANDLER()->getAVGMessureDataITimePSV();
					m_pcSubPara_IE->SetParaValue(iValue);
				}
				else if(	m_pcSubPara_PSVTIME 
					&&	m_pcSubPara_PSVTIME->IsWindowVisible()
					&&	m_bViewPraBtnShown)
				{
					iValue=getModel()->getDATAHANDLER()->getAVGMessureDataITimePSV();
					m_pcSubPara_PSVTIME->SetParaValue(iValue);
				}
			}
			
			if(	getModel()->getVMODEHANDLER()->activeModeIsCPAP())
			{
				if(		m_pcSubPara_DemandFlow 
					&&	m_pcSubPara_DemandFlow->IsWindowVisible()
					&&	m_bViewPraBtnShown)
				{
					iValue=getModel()->getDATAHANDLER()->GetDemandFlowData();
					m_pcSubPara_DemandFlow->SetParaValue(iValue);
				}
			}
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);

			return 1;
		}
		break;
	case WM_SET_PARATIMER:
		{
			//KillTimer(PARATIMER);
			SetTimer(PARATIMER, TIME_PARATIMER, NULL);
			if(getModel()->getVIEWHANDLER()->getViewState()==VS_ALARM_LIMIT)
			{
				getModel()->getVIEWHANDLER()->SetALIMITTIMER();
			}
			return 1;
		}
		break;
	case WM_KILL_PARATIMER:
		{
			KillTimer(PARATIMER);
			return 1;
		}
		break;
	/*case WM_FREQREC_LIMITED:
		{
			CMVEventInfotext event(CMVEventInfotext::EV_TIMETEXT, _T("Limitted by recruitment I-Time"), 3500);
			getModel()->triggerEvent(&event);

			getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
			Sleep(300);
			getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL); 
			return 1;
		}
		break;
	case WM_ITIMEREC_LIMITED:
		{
			CMVEventInfotext event(CMVEventInfotext::EV_TIMETEXT, _T("Limitted by recruitment frequency"), 3500);
			getModel()->triggerEvent(&event);

			getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
			Sleep(300);
			getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL); 
			return 1;
		}
		break;*/
	case WM_PEEP_LIMITED:
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(m_pcPara_PINSP)
				m_pcPara_PINSP->SetArrowUp();
			if(FALSE==getModel()->getCONFIG()->isPpsvAsDeltaPEEPValue())
			{
				if(m_pcPara_PPSV)
					m_pcPara_PPSV->SetArrowUp();
			}
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);
			
			return 1;
		}
		break;
	case WM_PEEP_LIMITED_PMAXI: //newVG
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(m_pcPara_PMAXI)
				m_pcPara_PMAXI->SetArrowUp();
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);

			return 1;
		}
		break;
	//case WM_PMAX_LIMITED_PINSP: //newVG
	//	{
	//		/** \brief The enter critical section. */
	//		EnterCriticalSection(&csDraw);
	//		if(m_pcPara_PINSP)
	//			m_pcPara_PINSP->SetArrowDown();
	//		if(getModel()->getDATAHANDLER()->GetCurrentPpsvPara() == getModel()->getDATAHANDLER()->GetCurrentPINSPPara())//newVG
	//		{
	//			if(m_pcPara_PPSV)
	//				m_pcPara_PPSV->SetArrowDown();
	//		}
	//		/** \brief The leave critical section. */
	//		LeaveCriticalSection(&csDraw);
	//		
	//		return 1;
	//	}
	//	break;
	case WM_PINSP_LIMITED_PMAXI: //newVG
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(m_pcPara_PMAXI)
				m_pcPara_PMAXI->SetArrowUp();
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);
			
			return 1;
		}
		break;
	case WM_CPAP_LIMITED:
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(m_pcPara_PDUO)
				m_pcPara_PDUO->SetArrowUp();
			if(m_pcPara_PMANUAL)
				m_pcPara_PMANUAL->SetArrowUp();
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);
			return 1;
		}
		break;
	case WM_PEEP_LIMITEDPSV:
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(m_pcPara_PPSV)
				m_pcPara_PPSV->SetArrowDown();
			if(m_pcPara_PINSP)
				m_pcPara_PINSP->SetArrowUp();
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);
			return 1;
		}
		break;
	case WM_SET_PSVWARNING:
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(m_pcPara_PPSV)
				m_pcPara_PPSV->SetWarning(true);
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);
			return 1;
		}
		break;
	case WM_CLEAR_PSVWARNING:
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(m_pcPara_PPSV)
				m_pcPara_PPSV->SetWarning(false);
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);
			return 1;
		}
		break;
	case WM_SET_PEEPWARNING:
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(m_pcPara_PEEP)
				m_pcPara_PEEP->SetWarning(true);
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);
			return 1;
		}
		break;
	case WM_CLEAR_PEEPWARNING:
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(m_pcPara_PEEP)
				m_pcPara_PEEP->SetWarning(false);
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);
			return 1;
		}
		break;
	case WM_PEEP_CHANGED:
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(m_pcPara_PINSP)
				m_pcPara_PINSP->SetArrowUp(false);

			if(m_pcPara_PMAXI)
				m_pcPara_PMAXI->SetArrowUp(false); //newVG
			
			if(m_pcPara_PPSV)
				m_pcPara_PPSV->SetArrowUp(false);
			if(m_pcPara_PPSV)
				m_pcPara_PPSV->SetArrowDown(false);
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);
			
			return 1;
		}
		break;
	case WM_CPAP_CHANGED:
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(m_pcPara_PDUO)
				m_pcPara_PDUO->SetArrowUp(false);
			if(m_pcPara_PMANUAL)
				m_pcPara_PMANUAL->SetArrowUp(false);
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);
			return 1;
		}
		break;
	case WM_PPSV_CHANGED:
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(m_pcPara_PMAXI)
				m_pcPara_PMAXI->SetArrowUp(false);
			if(m_pcPara_PINSP)
				m_pcPara_PINSP->SetArrowUp(false);
			if(m_pcPara_PEEP)
				m_pcPara_PEEP->SetArrowDown(false);
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);
			return 1;
		}
		break;
	case WM_PPSV_CLEAR_PEEP:
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(m_pcPara_PEEP)
				m_pcPara_PEEP->SetArrowDown(false);
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);
			return 1;
		}
		break;
	case WM_PINSP_CLEAR_PMAXI://newVG
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(m_pcPara_PMAXI)
				m_pcPara_PMAXI->SetArrowUp(false);
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);
			return 1;
		}
		break;
	case WM_PPSV_CLEAR_PINSP:
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(m_pcPara_PINSP)
				m_pcPara_PINSP->SetArrowUp(false);
			if(m_pcPara_PMAXI)
				m_pcPara_PMAXI->SetArrowUp(false);
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);
			return 1;
		}
		break;
	case WM_PINSP_CLEAR_PPSV: //newVG
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(m_pcPara_PPSV)
				m_pcPara_PPSV->SetArrowDown(false);
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);
			return 1;
		}
		break;
	case WM_PMEAN_CHANGED:
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(m_pcPara_PMANUAL)
				m_pcPara_PMANUAL->SetArrowUp(false);
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);

			setPmeanDiff(wParam,false);
			return 1;


			//m_bPMEAN_SetKey=false;//PMAN1
			///** \brief The enter critical section. */
			//EnterCriticalSection(&csDraw);
			//if(m_pcPara_PMANUAL)
			//	m_pcPara_PMANUAL->SetArrowUp(false);
			///** \brief The leave critical section. */
			//LeaveCriticalSection(&csDraw);

			//setPmeanHFO(wParam,false,true);//PMAN1
			////SetPmeanDiff(wParam,false);//PMAN1
			//return 1;
		}
		break;
	case WM_PMAN_CHANGED:
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(m_pcPara_PMITT)
				m_pcPara_PMITT->SetArrowDown(false);
			if(m_pcPara_CPAP)
				m_pcPara_CPAP->SetArrowDown(false);
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);
			return 1;
		}
		break;
	case WM_ITIMEREC_CHANGED:
		{
			return 1;
		}
		break;
	case WM_BPMREC_CHANGED:
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(m_pcPara_ITIME_REC)
				m_pcPara_ITIME_REC->SetArrowDown(false);
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);
			return 1;
		}
		break;
	case WM_PINSP_LIMITED:
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(m_pcPara_PEEP)
				m_pcPara_PEEP->SetArrowDown();
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);
			return 1;
		}
		break;
	case WM_PINSP_LIMITEDPSV:
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(m_pcPara_PPSV)
				m_pcPara_PPSV->SetArrowDown();
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);
			return 1;
		}
		break;
	case WM_PPSV_LIMITED_UP:
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(m_pcPara_PINSP)
				m_pcPara_PINSP->SetArrowUp();
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);
			
			return 1;
		}
		break;
	case WM_PPSV_LIMITED_DW:
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(m_pcPara_PEEP)
				m_pcPara_PEEP->SetArrowDown();
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);

			return 1;
		}
		break;
	case WM_PINSP_CHANGED:
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(m_pcPara_PEEP)
				m_pcPara_PEEP->SetArrowDown(false);
			if(m_pcPara_PPSV)
				m_pcPara_PPSV->SetArrowDown(false);
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);
			return 1;
		}
		break;
	case WM_PMAXI_CHANGED:
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(m_pcPara_PEEP) //newVG
				m_pcPara_PEEP->SetArrowDown(false);
			//if(m_pcPara_PINSP) //newVG
			//	m_pcPara_PINSP->SetArrowDown(false);
			//if(m_pcPara_PPSV) //newVG
			//	m_pcPara_PPSV->SetArrowDown(false);
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);
			return 1;
		}
		break;
	case WM_PDUO_LIMITED:
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(m_pcPara_CPAP)
				m_pcPara_CPAP->SetArrowDown();
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);
			return 1;
		}
		break;
	case WM_PMEAN_LIMITED:
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(m_pcPara_PMANUAL)
				m_pcPara_PMANUAL->SetArrowUp();
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);
			return 1;
		}
		break;
	case WM_PMAN_LIMITED:
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
			{
				if(m_pcPara_PMITT)
					m_pcPara_PMITT->SetArrowDown();
			}
			else
			{
				if(m_pcPara_CPAP)
					m_pcPara_CPAP->SetArrowDown();
			}
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);
			return 1;

		}
		break;
	case WM_PMANUAL_LIMITED_PMEAN:
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(m_pcPara_PMITT)
				m_pcPara_PMITT->SetArrowDown();
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);
			return 1;

		}
		break;
	case WM_PDUO_CHANGED:
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(m_pcPara_CPAP)
				m_pcPara_CPAP->SetArrowDown(false);
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);
			return 1;
		}
		break;
	case WM_RISETIME_LIMITED:
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(m_pcPara_ITIME)
				m_pcPara_ITIME->SetArrowUp();
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);
			return 1;
		}
		break;
	case WM_ETIME_LIMITED:
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(m_pcPara_ITIME)
				m_pcPara_ITIME->SetArrowDown();
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);
			return 1;
		}
		break;
	case WM_BPM_LIMITED:
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(m_pcPara_ITIME)
				m_pcPara_ITIME->SetArrowDown();
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);
			return 1;
		}
		break;
	case WM_O2FLUSH_LIMITED:
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(m_pcPara_O2)
				m_pcPara_O2->SetArrowDown();
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);
			return 1;
		}
		break;
	case WM_O2FLUSH_CHANGED:
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(m_pcPara_O2)
				m_pcPara_O2->SetArrowDown(false);
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);
			return 1;
		}
		break;
	case WM_PMEANREC_LIMITED:
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(m_pcPara_PMITT)
				m_pcPara_PMITT->SetArrowDown();
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);
			return 1;
		}
		break;
	case WM_PMEANREC_CHANGED:
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(m_pcPara_PMITT)
				m_pcPara_PMITT->SetArrowDown(false);
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);
			return 1;
		}
		break;
	case WM_ITIME_RISETIME_LIMITED:
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(m_pcPara_RISETIME)
				m_pcPara_RISETIME->SetArrowDown();
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);
			return 1;
		}
		break;
	case WM_ITIME_LIMITED:
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(getModel()->getCONFIG()->GetIERelationMode()==RM_SET_ItoBPM)
			{
				if(m_pcPara_BPM)
					m_pcPara_BPM->SetArrowDown();
			}
			else
			{
				if(m_pcPara_ETIME)
					m_pcPara_ETIME->SetArrowDown();
			}
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);
			return 1;
		}
		break;
	case WM_IE_VALUE_CONFIRMED:
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(m_pcPara_BPM)
				m_pcPara_BPM->SetKeyValueAccepted(true);
			if(m_pcPara_ETIME)
				m_pcPara_ETIME->SetKeyValueAccepted(true);
			if(m_pcPara_ITIME)
				m_pcPara_ITIME->SetKeyValueAccepted(true);
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);
			return 1;
		}
		break;
	case WM_IE_VALUE_NOTCONFIRMED:
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(m_pcPara_BPM)
				m_pcPara_BPM->SetKeyValueAccepted(false);
			if(m_pcPara_ETIME)
				m_pcPara_ETIME->SetKeyValueAccepted(false);
			if(m_pcPara_ITIME)
				m_pcPara_ITIME->SetKeyValueAccepted(false);
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);
			return 1;
		}
		break;
	case WM_ITIME_CHANGED:
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(getModel()->getCONFIG()->GetIERelationMode()==RM_SET_ItoBPM)
			{
				if(m_pcSubPara_ETime)
					m_pcSubPara_ETime->RefreshBtn();
				if(m_pcSubPara_IE)
					m_pcSubPara_IE->RefreshBtn();
				if(m_pcPara_BPM)
					m_pcPara_BPM->SetArrowDown(false);
				if(m_pcPara_BPM)
					m_pcPara_BPM->RefreshBtnState();
			}
			else
			{
				if(m_pcSubPara_BPM)
					m_pcSubPara_BPM->RefreshBtn();
				if(m_pcSubPara_IE)
					m_pcSubPara_IE->RefreshBtn();
				if(m_pcPara_ETIME)
					m_pcPara_ETIME->SetArrowDown(false);
				if(m_pcPara_ETIME)
					m_pcPara_ETIME->RefreshBtnState();
			}
			if(m_pcPara_RISETIME)
				m_pcPara_RISETIME->SetArrowDown(false);
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);
			return 1;
		}
		break;
	case WM_UPDATE_ETIME:
		{
			if(getModel()->getCONFIG()->GetCurMode()==VM_DUOPAP
				||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_DUOPAP)
			{
				/** \brief The enter critical section. */
				EnterCriticalSection(&csDraw);
				/*if(m_pcSubPara_ETime)
					m_pcSubPara_ETime->RefreshBtn(true);*/
				if(m_pcPara_ETIME)
					m_pcPara_ETIME->RefreshBtn();
				/** \brief The leave critical section. */
				LeaveCriticalSection(&csDraw);
			}
			
			return 1;
		}
		break;
	case WM_ITIME_TMPCHANGED:
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(getModel()->getCONFIG()->GetIERelationMode()==RM_SET_ItoBPM)
			{
				if(m_pcSubPara_ETime)
					m_pcSubPara_ETime->RefreshBtn(true);
				if(m_pcSubPara_IE)
					m_pcSubPara_IE->RefreshBtn();
				if(m_pcPara_BPM)
					m_pcPara_BPM->SetArrowDown(false);
				if(m_pcPara_BPM)
					m_pcPara_BPM->RefreshBtnState();
			}
			else
			{
				if(m_pcSubPara_BPM)
					m_pcSubPara_BPM->RefreshBtn();
				if(m_pcSubPara_IE)
					m_pcSubPara_IE->RefreshBtn();
				if(m_pcPara_ETIME)
					m_pcPara_ETIME->SetArrowDown(false);
				if(m_pcPara_ETIME)
					m_pcPara_ETIME->RefreshBtnState();
			}
			if(m_pcPara_RISETIME)
				m_pcPara_RISETIME->SetArrowDown(false);
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);
			return 1;
		}
		break;
	case WM_RISETIME_CHANGED:
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(m_pcPara_ITIME)
				m_pcPara_ITIME->SetArrowUp(false);
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);
			return 1;
		}
		break;
	case WM_ETIME_CHANGED:
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(m_pcSubPara_BPM)
				m_pcSubPara_BPM->RefreshBtn();
			if(m_pcSubPara_IE)
				m_pcSubPara_IE->RefreshBtn();
			if(m_pcPara_ITIME)
				m_pcPara_ITIME->SetArrowDown(false);
			if(m_pcPara_ITIME)
				m_pcPara_ITIME->RefreshBtnState();
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);
			return 1;
		}
		break;
	case WM_ETIME_TMPCHANGED:
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(m_pcSubPara_BPM)
				m_pcSubPara_BPM->RefreshBtn(true);
			if(m_pcSubPara_IE)
				m_pcSubPara_IE->RefreshBtn();
			if(m_pcPara_ITIME)
				m_pcPara_ITIME->SetArrowDown(false);
			if(m_pcPara_ITIME)
				m_pcPara_ITIME->RefreshBtnState();
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);
			return 1;
		}
		break;
	case WM_BPM_CHANGED:
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(m_pcSubPara_ETime)
				m_pcSubPara_ETime->RefreshBtn();
			if(m_pcSubPara_IE)
				m_pcSubPara_IE->RefreshBtn();
			if(m_pcPara_ITIME)
				m_pcPara_ITIME->SetArrowDown(false);
			if(m_pcPara_ITIME)
				m_pcPara_ITIME->RefreshBtnState();
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);
			return 1;
		}
		break;
	case WM_BPM_TMPCHANGED:
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(m_pcSubPara_ETime)
				m_pcSubPara_ETime->RefreshBtn(true);
			if(m_pcSubPara_IE)
				m_pcSubPara_IE->RefreshBtn();
			if(m_pcPara_ITIME)
				m_pcPara_ITIME->SetArrowDown(false);
			if(m_pcPara_ITIME)
				m_pcPara_ITIME->RefreshBtnState();
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);
			return 1;
		}
		break;
	case WM_O2_CHANGED:
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(m_pcPara_O2Flush)
				m_pcPara_O2Flush->CheckValue(wParam);
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);
			return 1;
		}
		break;
	case WM_ITIME_CLEAR_WARNING:
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(m_pcPara_BPM)
				m_pcPara_BPM->RefreshBtnState();
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);
			return 1;
		}
		break;
	case WM_ETIME_CLEAR_WARNING:
		{
			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			if(m_pcPara_ITIME)
				m_pcPara_ITIME->RefreshBtnState();
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);
			return 1;
		}
		break;
	case WM_CPAP_SETKEY:
		{
			//if(m_bCPAP_SetKey==false)
			{
				//m_bCPAP_SetKey=true;
				if(wParam==TRUE)
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
				}

				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_CONFIRMRANGE);
			}
			return 1;
		}
		break;
	/*case WM_CPAP_CLEARKEY:
		{
			m_bCPAP_SetKey=false;
			return 1;
		}
		break;*/
	case WM_ITIME_SETKEY:
		{
			//if(m_bITIME_SetKey==false)
			{
				//m_bITIME_SetKey=true;
				if(wParam==TRUE)
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
				}
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_CONFIRMRANGE);
			}
			return 1;
		}
		break;
	/*case WM_ITIME_CLEARKEY:
		{
			m_bITIME_SetKey=false;
			return 1;
		}
		break;*/
	case WM_ETIME_SETKEY:
		{
			//if(m_bETIME_SetKey==false)
			{
				//m_bETIME_SetKey=true;
				if(wParam==TRUE)
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
				}
				
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_CONFIRMRANGE);
			}
			return 1;
		}
		break;
	/*case WM_ETIME_CLEARKEY:
		{
			m_bETIME_SetKey=false;
			return 1;
		}
		break;*/
	case WM_PDUO_SETKEY:
		{
			//if(m_bPDUO_SetKey==false)
			{
				//m_bPDUO_SetKey=true;
				if(wParam==TRUE)
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
				}
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_CONFIRMRANGE);
			}
			return 1;
		}
		break;
	/*case WM_PDUO_CLEARKEY:
		{
			m_bPDUO_SetKey=false;
		}
		break;*/
	/*case WM_THERAPYFLOW_CLEARKEY:
		{
			m_bTHERAPYFLOW_SetKey=false;
		}
		break;*/
	case WM_THERAPYFLOW_SETKEY:
		{
			//if(m_bPDUO_SetKey==false)
			{
				//m_bTHERAPYFLOW_SetKey=true;
				if(wParam==TRUE)
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
				}
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_CONFIRMRANGE);
			}
			return 1;
		}
		break;
	case WM_PMAN_SETKEY:
		{
			//if(m_bPMan_SetKey==false)
			{
				//m_bPMan_SetKey=true;
				if(wParam==TRUE)
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
				}
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_CONFIRMRANGE);
			}
			return 1;
		}
		break;
	/*case WM_PMAN_CLEARKEY:
		{
			m_bPMan_SetKey=false;
			return 1;
		}
		break;*/
	case WM_PEEP_SETKEY_LOW:
		{
			//if(m_bPEEP_SetKey==false)
			{
				//m_bPEEP_SetKey=true;
				if(wParam==TRUE)
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
				}
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_CONFIRMLOWRANGE);
			}
			return 1;
		}
		break;
	case WM_PEEP_SETKEY_HIGH:
		{
			//if(m_bPEEP_SetKey==false)
			{
				//m_bPEEP_SetKey=true;
				if(wParam==TRUE)
				{
					if(wParam==TRUE)
					{
						if(AfxGetApp())
							AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
					}
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_CONFIRMHIGHRANGE);
				}
			}
			return 1;
		}
		break;
	/*case WM_PEEP_CLEARKEY:
		{
			m_bPEEP_SetKey=false;
			return 1;
		}
		break;*/
	case WM_PMEANREC_SETKEY:
		{
			//if(m_bPMEAN_SetKey==false)
			{
				//m_bPMEANREC_SetKey=true;
				if(wParam==TRUE)
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
				}
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_CONFIRMRANGE);
			}
			return 1;
		}
		break;
	/*case WM_PMEANREC_CLEARKEY:
		{
			m_bPMEANREC_SetKey=false;
			return 1;
		}
		break;*/
	case WM_PMEAN_SETKEY:
		{
			//if(m_bPMEAN_SetKey==false)
			{
				//m_bPMEAN_SetKey=true;
				if(wParam==TRUE)
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
				}
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_CONFIRMRANGE);
			}
			return 1;
		}
		break;
	/*case WM_PMEAN_CLEARKEY:
		{
			m_bPMEAN_SetKey=false;
			return 1;
		}
		break;*/
	//case WM_HFAMP_SETKEY:
	//	{
	//		//if(m_bHFamp_SetKey==false)
	//		{
	//			m_bHFamp_SetKey=true;
	//			if(wParam==TRUE)
	//			{
	//				if(AfxGetApp())
	//					AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
	//			}
	//			if(AfxGetApp())
	//				AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_CONFIRMRANGE);
	//		}
	//		return 1;
	//	}
	//	break;
	//case WM_HFAMP_CLEARKEY:
	//	{
	//		m_bHFamp_SetKey=false;
	//		return 1;
	//	}
	//	break;
	case WM_HFFREQ_SETKEY:
		{
			//if(m_bHFfreq_SetKey==false)
			{
				//m_bHFfreq_SetKey=true;
				if(wParam==TRUE)
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
				}
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_CONFIRMRANGE);
			}
			return 1;
		}
		break;
	/*case WM_HFFREQ_CLEARKEY:
		{
			m_bHFfreq_SetKey=false;
			return 1;
		}
		break;*/
	case WM_PPSV_SETKEY:
		{
			//if(m_bPPSV_SetKey==false)
			{
				//m_bPPSV_SetKey=true;
				if(wParam==TRUE)
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
				}
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_CONFIRMRANGE);
			}
			return 1;
		}
		break;
	/*case WM_PPSV_CLEARKEY:
		{
			m_bPPSV_SetKey=false;
			return 1;
		}
		break;*/
	case WM_PMAXI_SETKEY:
		{
			//if(m_bPINSP_SetKey==false)
			{
				//m_bPMAX_SetKey=true;
				if(wParam==TRUE)
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
				}
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_CONFIRMRANGE);
			}
			return 1;
		}
		break;
	case WM_AMPMAXI_SETKEY:
		{
			//if(m_bPINSP_SetKey==false)
			{
				//m_bPMAX_SetKey=true;
				if(wParam==TRUE)
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
				}
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_CONFIRMRANGE);
			}
			return 1;
		}
		break;
	/*case WM_PMAX_CLEARKEY:
		{
			m_bPMAX_SetKey=false;
			return 1;
		}
		break;*/
	case WM_PINSP_SETKEY:
		{
			//if(m_bPINSP_SetKey==false)
			{
				//m_bPINSP_SetKey=true;
				if(wParam==TRUE)
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
				}
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_CONFIRMRANGE);
			}
			return 1;
		}
		break;
	case WM_EFLOW_SETKEY:
		{
			if(wParam==TRUE)
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
			}
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_CONFIRMRANGE);
			return 1;
		}
		break;
	case WM_IFLOW_SETKEY:
		{
			if(wParam==TRUE)
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
			}
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_CONFIRMRANGE);
			return 1;
		}
		break;
	/*case WM_PINSP_CLEARKEY:
		{
			m_bPINSP_SetKey=false;
			return 1;
		}
		break;*/
	case WM_VGARANT_SETKEY:
		{
			//if(m_bVGARANT_SetKey==false)
			{
				//m_bVGARANT_SetKey=true;
				if(wParam==TRUE)
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
				}
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_CONFIRMRANGE);
			}
			return 1;
		}
		break;
	/*case WM_VGARANT_CLEARKEY:
		{
			m_bVGARANT_SetKey=false;
			return 1;
		}
		break;*/
	case WM_VLIMIT_SETKEY:
		{
			//if(m_bVLIMIT_SetKey==false)
			{
				//
				//m_bVLIMIT_SetKey=true;
				if(wParam==TRUE)
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
				}
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_CONFIRMRANGE);
			}
			return 1;
		}
		break;
	/*case WM_VLIMIT_CLEARKEY:
		{
			m_bVLIMIT_SetKey=false;
			return 1;
		}
		break;*/
	case WM_BPM_SETKEY:
		{
			//if(m_bBPM_SetKey==false)
			{
				//m_bBPM_SetKey=true;
				if(wParam==TRUE)
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
				}
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_CONFIRMRANGE);
			}
			return 1;
		}
		break;
	/*case WM_BPM_CLEARKEY:
		{
			m_bBPM_SetKey=false;
			return 1;
		}
		break;*/
	case WM_BPM_CLEAR_WARNING:
		{
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);if(m_pcPara_ITIME)
				m_pcPara_ITIME->RefreshBtnState();
			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);
			return 1;
		}
		break;
	case WM_PARABUTTON_VKUP:
		{
			if(m_bSettingsActive)
			{
				m_pcWndParaSettings->SetNextFocus();
				return 1;
			}

			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);

			CWnd* pFocWnd = CWnd::GetFocus();
			if(pFocWnd==NULL)
			{
				/** \brief The leave critical section. */
				LeaveCriticalSection(&csDraw);
				break;
			}
			iID=pFocWnd->GetDlgCtrlID();
			CWnd* pWnd = GetNextDlgTabItem(pFocWnd,FALSE);
			if(pWnd==NULL)
			{
				/** \brief The leave critical section. */
				LeaveCriticalSection(&csDraw);
				break;
			}


			if(		pFocWnd->GetDlgCtrlID()>pWnd->GetDlgCtrlID()
				||	IDC_BTN_PARA_VG_VL<pWnd->GetDlgCtrlID())
			{
				/** \brief The leave critical section. */
				LeaveCriticalSection(&csDraw);
				break;
			}

			iID=pWnd->GetDlgCtrlID();
			pFocWnd->CheckDlgButton(pFocWnd->GetDlgCtrlID(),BST_UNCHECKED);
			pWnd->SetFocus();

			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);

			getModel()->getCONFIG()->SetLastSelectedParaViewBtns(iID);

			return 1;
		}
		break;
	case WM_PARABUTTON_VKDOWN:
		{
			if(m_bSettingsActive)
			{
				m_pcWndParaSettings->SetPrevFocus();
				return 1;
			}

			/** \brief The enter critical section. */
			EnterCriticalSection(&csDraw);
			CWnd* pFocWnd = CWnd::GetFocus();
			if(pFocWnd==NULL)
			{
				/** \brief The leave critical section. */
				LeaveCriticalSection(&csDraw);
				break;
			}

			CWnd* pWnd = GetNextDlgTabItem(pFocWnd,TRUE);
			if(pWnd==NULL)
			{
				/** \brief The leave critical section. */
				LeaveCriticalSection(&csDraw);
				break;
			}

			if(pFocWnd->GetDlgCtrlID()<pWnd->GetDlgCtrlID())
			{
				/** \brief The leave critical section. */
				LeaveCriticalSection(&csDraw);
				break;
			}

			iID=pWnd->GetDlgCtrlID();
			pFocWnd->CheckDlgButton(pFocWnd->GetDlgCtrlID(),BST_UNCHECKED);
			pWnd->SetFocus();

			/** \brief The leave critical section. */
			LeaveCriticalSection(&csDraw);

			getModel()->getCONFIG()->SetLastSelectedParaViewBtns(iID);

			return 1;
		}
		break;
	}
	return CWnd::WindowProc(message, wParam, lParam);
}

// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::NotifyParaBtnEvent(CMVEvent* pEvent)
{
	//csView.Lock();
	switch(pEvent->GetET())
	{
	case CMVEvent::ET_UIEVENT:
		{
			CMVEventUI* pUIEvent = (CMVEventUI*)pEvent;
			switch(pUIEvent->GetEventType())
			{
			case CMVEventUI::EV_PARABN_HFAMPL:
				{
					fVALUE fv;
					/** \brief The enter critical section. */
					EnterCriticalSection(&csDraw);
					if(m_pcPara_HFAMPL)
					{
						fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLMinPara();
						fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLMaxPara();
						fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLPara();
						/*if(fv.iValue>getModel()->getDATAHANDLER()->PARADATA()->GetHFampMaxKey())
						{
							m_pcPara_HFAMPL->SetWarning();
						}
						else
						{
							m_pcPara_HFAMPL->SetWarning(false);
						}*/
						m_pcPara_HFAMPL->SetValue(fv, true);

					}
					/** \brief The leave critical section. */
					LeaveCriticalSection(&csDraw);

				}
				break;
			case CMVEventUI::EV_PARABN_VLIMIT_ENABLED:
				{
					/** \brief The enter critical section. */
					EnterCriticalSection(&csDraw);
					if(m_pcPara_VLIMIT)
					{
						fVALUE fv;
						m_pcPara_VLIMIT->GetValue(&fv);
						if(fv.iValue>getModel()->getDATAHANDLER()->GetCurrentVlimitMaxKey())
						{
							m_pcPara_VLIMIT->SetWarning(true);
						}
						else
						{
							m_pcPara_VLIMIT->SetWarning(false);
						}

						if(m_pcPara_VLIMIT)
							m_pcPara_VLIMIT->SetBitmaps(m_pcPara_UpB,m_pcPara_DwB,m_pcPara_UpB,m_pcPara_FcB,false,false);

						m_pcPara_VLIMIT->RefreshBtn();
					}
					/** \brief The leave critical section. */
					LeaveCriticalSection(&csDraw);
					
				}
				break;
			case CMVEventUI::EV_PARABN_VLIMIT_DISABLED:
				{
					/** \brief The enter critical section. */
					EnterCriticalSection(&csDraw);
					if(m_pcPara_VLIMIT)
					{
						fVALUE fv;
						m_pcPara_VLIMIT->GetValue(&fv);
						if(fv.iValue>getModel()->getDATAHANDLER()->GetCurrentVlimitMaxKey())
						{
							m_pcPara_VLIMIT->SetWarning(true);
						}
						else
						{
							m_pcPara_VLIMIT->SetWarning(false);
						}
						if(m_pcPara_VLIMIT)
							m_pcPara_VLIMIT->SetBitmaps(m_pcPara_DisB,m_pcPara_DwB,m_pcPara_DisB,m_pcPara_FcB,false,false);

						m_pcPara_VLIMIT->RefreshBtn();
					}
					/** \brief The leave critical section. */
					LeaveCriticalSection(&csDraw);
				}
				break;
			case CMVEventUI::EV_VOLUMEGARANT_ON:
			case CMVEventUI::EV_VOLUMEGARANT_OFF:
			case CMVEventUI::EV_VOLUMEGARANT_PRESET_ON:
			case CMVEventUI::EV_VOLUMEGARANT_PRESET_OFF:
			case CMVEventUI::EV_PARABN_VLIMIT_AUTO_ON:
			case CMVEventUI::EV_PARABN_VLIMIT_AUTO_OFF:
			case CMVEventUI::EV_PARABN_VOLUMEGARANT_AUTO_ON:
			case CMVEventUI::EV_PARABN_VOLUMEGARANT_AUTO_OFF:
				{
					PostMessage(WM_REDRAW_VIEW);
				}
				break;
			default:
				break;
			}
		}

	}
}


//************************************
// Method:    OnDestroy
// FullName:  CViewParaBtn::OnDestroy
// Access:    protected 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CViewParaBtn::OnDestroy()
{
	m_bExit=true;

	KillTimer(PARATIMER);

	m_plParaBtn.RemoveAll();

	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);

	if(m_pcPara_PMAXI)
		delete m_pcPara_PMAXI;
	m_pcPara_PMAXI=NULL;

	if(m_pcPara_AMPMAXI)
		delete m_pcPara_AMPMAXI;
	m_pcPara_AMPMAXI=NULL;

	if(m_pcPara_RISETIME)
		delete m_pcPara_RISETIME;
	m_pcPara_RISETIME=NULL;

	if(m_pcPara_IFLOW)
		delete m_pcPara_IFLOW;
	m_pcPara_IFLOW=NULL;

	/*if(m_pcPara_EFLOW)
		delete m_pcPara_EFLOW;
	m_pcPara_EFLOW=NULL;*/

	if(m_pcPara_PEEP)
		delete m_pcPara_PEEP;
	m_pcPara_PEEP=NULL;

	if(m_pcPara_PINSP)
		delete m_pcPara_PINSP;
	m_pcPara_PINSP=NULL;

	if(m_pcPara_PDUO)
		delete m_pcPara_PDUO;
	m_pcPara_PDUO=NULL;

	if(m_pcPara_PMANUAL)
		delete m_pcPara_PMANUAL;
	m_pcPara_PMANUAL=NULL;

	if(m_pcPara_VLIMIT)
		delete m_pcPara_VLIMIT;
	m_pcPara_VLIMIT=NULL;

	if(m_pcPara_TRIGGERCONV)
		delete m_pcPara_TRIGGERCONV;
	m_pcPara_TRIGGERCONV=NULL;

	if(m_pcPara_TRIGGERNMODE)
		delete m_pcPara_TRIGGERNMODE;
	m_pcPara_TRIGGERNMODE=NULL;

	if(m_pcPara_BPM)
		delete m_pcPara_BPM;
	m_pcPara_BPM=NULL;

	if(m_pcPara_ITIME)
		delete m_pcPara_ITIME;
	m_pcPara_ITIME=NULL;

	if(m_pcPara_ETIME)
		delete m_pcPara_ETIME;
	m_pcPara_ETIME=NULL;

	if(m_pcPara_O2)
		delete m_pcPara_O2;
	m_pcPara_O2=NULL;
	if(m_pcPara_O2Flush)
		delete m_pcPara_O2Flush;
	m_pcPara_O2Flush=NULL;

	if(m_pcPara_VGARANT)
		delete m_pcPara_VGARANT;
	m_pcPara_VGARANT=NULL;

	if(m_pcPara_BACKUP)
		delete  m_pcPara_BACKUP;
	m_pcPara_BACKUP=NULL;

	if(m_pcPara_CPAP)
		delete  m_pcPara_CPAP;
	m_pcPara_CPAP=NULL;

	if(m_pcPara_FLOWMIN)
		delete  m_pcPara_FLOWMIN;
	m_pcPara_FLOWMIN=NULL;

	if(m_pcPara_FLOW)
		delete m_pcPara_FLOW;
	m_pcPara_FLOW=NULL;

	if(m_pcPara_PMITT)
		delete  m_pcPara_PMITT;
	m_pcPara_PMITT=NULL;

	if(m_pcPara_PPSV)
		delete m_pcPara_PPSV;
	m_pcPara_PPSV=NULL;

	if(m_pcPara_HFAMPL)
		delete  m_pcPara_HFAMPL;
	m_pcPara_HFAMPL=NULL;

	if(m_pcPara_HFFREQ)
		delete  m_pcPara_HFFREQ;
	m_pcPara_HFFREQ=NULL;

	if(m_pcPara_IERatio)
		delete  m_pcPara_IERatio;
	m_pcPara_IERatio=NULL;


	if(m_pcPara_ITIME_REC)
		delete  m_pcPara_ITIME_REC;
	m_pcPara_ITIME_REC=NULL;

	/*if(m_pcPara_FREQ_REC)
		delete  m_pcPara_FREQ_REC;
	m_pcPara_FREQ_REC=NULL;*/

	if(m_pcPara_PMEAN_REC)
		delete  m_pcPara_PMEAN_REC;
	m_pcPara_PMEAN_REC=NULL;

	if(m_pcPara_HFFLOW)
		delete  m_pcPara_HFFLOW;
	m_pcPara_HFFLOW=NULL;


	if(m_pcSubPara_TrigThreshold)
		delete m_pcSubPara_TrigThreshold;
	m_pcSubPara_TrigThreshold=NULL;

	if(m_pcSubPara_LEAKCOMP)
		delete m_pcSubPara_LEAKCOMP;
	m_pcSubPara_LEAKCOMP=NULL;

	if(m_pcSubPara_BIASFLOW)
		delete m_pcSubPara_BIASFLOW;
	m_pcSubPara_BIASFLOW=NULL;

	if(m_pcSubPara_IE)
		delete m_pcSubPara_IE;
	m_pcSubPara_IE=NULL;

	if(m_pcSubPara_DemandFlow)
		delete m_pcSubPara_DemandFlow;
	m_pcSubPara_DemandFlow=NULL;

	if(m_pcSubPara_PSVTIME)
		delete m_pcSubPara_PSVTIME;
	m_pcSubPara_PSVTIME=NULL;

	if(m_pcSubPara_BPM)
		delete m_pcSubPara_BPM;
	m_pcSubPara_BPM=NULL;

	if(m_pcSubPara_ETime)
		delete m_pcSubPara_ETime;
	m_pcSubPara_ETime=NULL;
	/*if(m_pcSubPara_PSV)
		delete m_pcSubPara_PSV;*/

	if(m_pcSubPara_O2Flush)
		delete m_pcSubPara_O2Flush;
	m_pcSubPara_O2Flush=NULL;
	if(m_pcSubPara_O2act)
		delete m_pcSubPara_O2act;
	m_pcSubPara_O2act=NULL;
	/*if(m_pcSubPara_TVI)
		delete m_pcSubPara_TVI;
	m_pcSubPara_TVI=NULL;*/
	/*if(m_pcSubPara_VG_VL)
		delete m_pcSubPara_VG_VL;
	m_pcSubPara_VG_VL=NULL;*/

	if(m_pcPara_UpT)
		delete m_pcPara_UpT;
	m_pcPara_UpT=NULL;
	if(m_pcPara_FcT)
		delete m_pcPara_FcT;
	m_pcPara_FcT=NULL;
	if(m_pcPara_DwT)
		delete m_pcPara_DwT;
	m_pcPara_DwT=NULL;
	if(m_pcPara_UpB)
		delete m_pcPara_UpB;
	m_pcPara_UpB=NULL;

	if(m_pcPara_SigT)
		delete m_pcPara_SigT;
	m_pcPara_SigT=NULL;

	if(m_pcPara_DisB)
		delete m_pcPara_DisB;
	m_pcPara_DisB=NULL;

	if(m_pcPara_DisT)
		delete m_pcPara_DisT;
	m_pcPara_DisT=NULL;

	//VG_VL_NEW
	if(m_pcPara_OnOff)
		delete m_pcPara_OnOff;
	m_pcPara_OnOff=NULL;

	if(m_pcPara_OnOff_Up)
		delete m_pcPara_OnOff_Up;
	m_pcPara_OnOff_Up=NULL;
	if(m_pcPara_OnOff_Dw)
		delete m_pcPara_OnOff_Dw;
	m_pcPara_OnOff_Dw=NULL;
	/*if(m_pcPara_OnOff_Fc)
		delete m_pcPara_OnOff_Fc;
	m_pcPara_OnOff_Fc=NULL;*/


	if(m_pcParaGroup_UpT)
		delete m_pcParaGroup_UpT;
	m_pcParaGroup_UpT=NULL;
	if(m_pcParaGroup_FcT)
		delete m_pcParaGroup_FcT;
	m_pcParaGroup_FcT=NULL;
	if(m_pcParaGroup_DwT)
		delete m_pcParaGroup_DwT;
	m_pcParaGroup_DwT=NULL;
	if(m_pcParaGroup_SigT)
		delete m_pcParaGroup_SigT;
	m_pcParaGroup_SigT=NULL;

	if(m_pcParaGroup_UpB)
		delete m_pcParaGroup_UpB;
	m_pcParaGroup_UpB=NULL;
	if(m_pcParaGroup_FcB)
		delete m_pcParaGroup_FcB;
	m_pcParaGroup_FcB=NULL;
	if(m_pcParaGroup_DwB)
		delete m_pcParaGroup_DwB;
	m_pcParaGroup_DwB=NULL;
	if(m_pcParaGroup_SigB)
		delete m_pcParaGroup_SigB;
	m_pcParaGroup_SigB=NULL;
	
	if(m_pcLungRec)
		delete m_pcLungRec;
	m_pcLungRec=NULL;

	/*if(m_pcVGorange)
		delete m_pcVGorange;
	m_pcVGorange=NULL;

	if(m_pcVLorange)
		delete m_pcVLorange;
	m_pcVLorange=NULL;*/


	if(m_pcParaMenu_Up)
		delete m_pcParaMenu_Up;
	m_pcParaMenu_Up=NULL;
	
	if(m_pcParaMenu_Dw)
		delete m_pcParaMenu_Dw;
	m_pcParaMenu_Dw=NULL;

	if(m_pcParaMenu)
		delete m_pcParaMenu;
	m_pcParaMenu=NULL;

	if(m_pcPara_SigB)
		delete m_pcPara_SigB;
	m_pcPara_SigB=NULL;

	if(m_pcPara_FcB)
		delete m_pcPara_FcB;
	m_pcPara_FcB=NULL;
	if(m_pcPara_DwB)
		delete m_pcPara_DwB;
	m_pcPara_DwB=NULL;
	if(m_pcSubPara_Up)
		delete m_pcSubPara_Up;
	m_pcSubPara_Up=NULL;
	if(m_pcSubPara_Dw)
		delete m_pcSubPara_Dw;
	m_pcSubPara_Dw=NULL;
	if(m_pcSubPara_Sig)
		delete m_pcSubPara_Sig;
	m_pcSubPara_Sig=NULL;
	if(m_pcSubPara_Trig)
		delete m_pcSubPara_Trig;
	m_pcSubPara_Trig=NULL;

	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);

	if(m_pWndMenuVentState)
	{
		m_pWndMenuVentState->DestroyWindow();
		delete m_pWndMenuVentState;
	}
	m_pWndMenuVentState=NULL;

	DestroyWndParaSettings();

	CMVView::OnDestroy();
}

// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::NotifyEvent(CMVEvent* pEvent)
{
	switch(pEvent->GetET())
	{
	case CMVEvent::ET_CONTROL:
		{
			CMVEventControl* pCtrlEvent = (CMVEventControl*)pEvent;
			switch(pCtrlEvent->GetEventType())
			{
			case CMVEventControl::EV_CONTROL_SYSTEMSTATE_CHANGED:
				{
					try
					{
						Show();
					}
					catch (...)
					{
						theApp.ReportException(_T("CViewParaBtn::NotifyEvent EV_CONTROL_SYSTEMSTATE_CHANGED"));
					}
				}
				break;
			case CMVEventControl::EV_CONTROL_FLOWSENSORSTATE:
				{
					try
					{
						m_bViewPraBtnShown=false;
						ShowParaBtn(false);
						m_bViewPraBtnShown=true;
					}
					catch (...)
					{
						theApp.ReportException(_T("CViewParaBtn::NotifyEvent EV_CONTROL_FLOWSENSORSTATE"));
					}
				}
				break;
			case CMVEventControl::EV_CONTROL_O2SENSORSTATE:
				{
					try
					{
						m_bViewPraBtnShown=false;
						ShowParaBtn(false);
						m_bViewPraBtnShown=true;
					}
					catch (...)
					{
						theApp.ReportException(_T("CViewParaBtn::NotifyEvent EV_CONTROL_O2SENSORSTATE"));
					}
				}
				break;
			case CMVEventControl::EV_CONTROL_POWERSTATE_CHANGED:
				{
					try
					{
						SetAllButtonUnpressed();

						/** \brief The enter critical section. */
						EnterCriticalSection(&csDraw);


						fVALUE fvAmp;
						fVALUE fvMax;
						fvAmp.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLPara();
						fvAmp.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLMinPara();
						fvAmp.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLMaxPara();
						
						if(fvAmp.iValue>fvAmp.iUpperLimit)
						{
							fvAmp.iValue=fvAmp.iUpperLimit;
							getModel()->getDATAHANDLER()->SetCurrentHFAMPLPara(fvAmp.iValue);

						}
						else if(fvAmp.iValue<fvAmp.iLowerLimit)
						{
							fvAmp.iValue=fvAmp.iLowerLimit;
							getModel()->getDATAHANDLER()->SetCurrentHFAMPLPara(fvAmp.iValue);
						}

						

						/*if(fvAmp.iValue>getModel()->getDATAHANDLER()->PARADATA()->GetHFampMaxKey())
						{
							if(m_pcPara_HFAMPL)
								m_pcPara_HFAMPL->SetWarning();
						}
						else
						{
							if(m_pcPara_HFAMPL)
								m_pcPara_HFAMPL->SetWarning(false);
						}*/
						if(m_pcPara_HFAMPL)
							m_pcPara_HFAMPL->SetValue(fvAmp, true);

						//##################P max / HF amp max
						if(getModel()->getCONFIG()->GetCurMode()==VM_PRE_HFO 
							||	getModel()->getCONFIG()->GetCurMode()==VM_HFO)
						{
							fvMax.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLMinPara();
							fvMax.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLMaxPara();

							fvMax.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLmaxPara();
							if(fvMax.iValue>fvMax.iUpperLimit)
							{
								fvMax.iValue=fvMax.iUpperLimit;
								getModel()->getDATAHANDLER()->SetCurrentPmaxVolGPara(fvMax.iValue);
							}
							else if(fvMax.iValue<fvMax.iLowerLimit)
							{
								fvMax.iValue=fvMax.iLowerLimit;
								getModel()->getDATAHANDLER()->SetCurrentPmaxVolGPara(fvMax.iValue);
							}

							if(m_pcPara_AMPMAXI)
								m_pcPara_AMPMAXI->SetValue(fvMax, true);
						}
						else
						{
							if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
							{
								fvMax.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPMAXMinPara_IPPV();
								fvMax.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPMAXMaxPara_IPPV();
								fvMax.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetPmaxVolGPara_IPPV();
							}
							else
							{
								fvMax.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPMAXMinPara_TRIGGER();
								fvMax.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPMAXMaxPara_TRIGGER();
								fvMax.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetPmaxVolGPara_TRIGGER();
							}	
							if(fvMax.iValue>fvMax.iUpperLimit)
							{
								fvMax.iValue=fvMax.iUpperLimit;
								getModel()->getDATAHANDLER()->SetCurrentPmaxVolGPara(fvMax.iValue);
							}
							else if(fvMax.iValue<fvMax.iLowerLimit)
							{
								fvMax.iValue=fvMax.iLowerLimit;
								getModel()->getDATAHANDLER()->SetCurrentPmaxVolGPara(fvMax.iValue);
							}

							if(m_pcPara_PMAXI)
								m_pcPara_PMAXI->SetValue(fvMax, true);
						}
						
						/** \brief The leave critical section. */
						LeaveCriticalSection(&csDraw);
					}
					catch (...)
					{
						/** \brief The leave critical section. */
						LeaveCriticalSection(&csDraw);

						theApp.ReportException(_T("CViewParaBtn::NotifyEvent EV_CONTROL_POWERSTATE_CHANGED"));
					}
				}
				break;
			default:
				{

				}
				break;
			}
		}
		break;
	case CMVEvent::ET_ALARM://++++++++++++++++++++++++++++++System-AlarmeP+++++++++++++++++++++++++++++++++++++++++
		{
			CMVEventAlarm* pUIEvent = (CMVEventAlarm*)pEvent;
			switch(pUIEvent->GetEventType())
			{
			case CMVEventAlarm::EV_ALARMSTATE_CHANGED:
				{
					if(		getModel()->getALARMHANDLER()->getAlarmSilentState()!=ASTATE_SILENT
						&&	getModel()->getALARMHANDLER()->getAlarmSilentState()!=ASTATE_SYSTEM_SILENT)
					{
						/** \brief The enter critical section. */
						EnterCriticalSection(&csDraw);

						bool bSetIFlowArrow=false;
						if(getModel()->getALARMHANDLER()->ALARM_SysLimit_Pinsp_NotReached->getAlarmState()!=AS_NONE)
						{
							bSetIFlowArrow=true;
						}

						if(getModel()->getALARMHANDLER()->ALARM_SysLimit_Vlimitted->getAlarmState()!=AS_NONE)
						{
							if(m_pcPara_PINSP)
								m_pcPara_PINSP->SetAlarmArrowDown();
						}
						else
						{
							if(m_pcPara_PINSP)
								m_pcPara_PINSP->SetAlarmArrowDown(false);
						}

						if(getModel()->getALARMHANDLER()->ALARM_SysLimit_Vgaranty->getAlarmState()!=AS_NONE)
						{
							if(getModel()->getVGarantyAlarmFlag()==VGAL_LOW)
							{
								int iState=getModel()->getDATAHANDLER()->getStatus2();

								if(iState&BIT6)
								{
									bSetIFlowArrow=true;
									if(m_pcPara_AMPMAXI)
										m_pcPara_AMPMAXI->SetAlarmArrowUp(false);
									if(m_pcPara_PMAXI)
										m_pcPara_PMAXI->SetAlarmArrowUp(false);
								}
								else if(false==getModel()->getCONFIG()->CurModeIsPresetMode())
								{
									int iUpperLimit=0;
									int iValue=0;
									if(getModel()->getCONFIG()->GetCurMode()==VM_PRE_HFO 
										||	getModel()->getCONFIG()->GetCurMode()==VM_HFO)
									{
										iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLMaxPara();
										iValue=getModel()->getDATAHANDLER()->GetCurrentHFAMPLmaxPara();

										if(iValue<iUpperLimit)
										{
											if(m_pcPara_AMPMAXI)
												m_pcPara_AMPMAXI->SetAlarmArrowUp();
										}
										else
										{
											if(m_pcPara_AMPMAXI)
												m_pcPara_AMPMAXI->SetAlarmArrowUp(false);
										}
									}
									else
									{
										if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
										{
											iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPMAXMaxPara_IPPV();

										}
										else
										{
											iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPMAXMaxPara_TRIGGER();
										}		
										iValue=getModel()->getDATAHANDLER()->GetCurrentPmaxVolGPara();

										if(iValue<iUpperLimit)
										{
											if(m_pcPara_PMAXI)
												m_pcPara_PMAXI->SetAlarmArrowUp();
										}
										else
										{
											if(m_pcPara_PMAXI)
												m_pcPara_PMAXI->SetAlarmArrowUp(false);
										}
									}
									
								}
								else //if(getModel()->getCONFIG()->CurModeIsPresetMode())
								{
									if(m_pcPara_AMPMAXI)
										m_pcPara_AMPMAXI->SetAlarmArrowUp(false);

									if(m_pcPara_PMAXI)
										m_pcPara_PMAXI->SetAlarmArrowUp(false);
								}
							}
						}
						else
						{
							if(m_pcPara_AMPMAXI)
								m_pcPara_AMPMAXI->SetAlarmArrowUp(false);

							if(m_pcPara_PMAXI)
								m_pcPara_PMAXI->SetAlarmArrowUp(false);

						}

						if(bSetIFlowArrow)
						{
							if(m_pcPara_IFLOW)
								m_pcPara_IFLOW->SetAlarmArrowUp(true);
						}
						else
						{
							if(m_pcPara_IFLOW)
								m_pcPara_IFLOW->SetAlarmArrowUp(false);
						}
						/** \brief The leave critical section. */
						LeaveCriticalSection(&csDraw);
					}
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
}

// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::refreshO2Flush()
{
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcSubPara_O2Flush)
	{
		m_pcSubPara_O2Flush->RefreshBtn();
	}
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
	ShowParaBtn(false);
}



// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::SetO2FlushTime(int iO2FlushTime)
{
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcSubPara_O2Flush)
	{
		fVALUE fv;
		fv.iValue=iO2FlushTime;
		m_pcSubPara_O2Flush->SetValue(fv, true);
	}
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::setPRICOrunning(bool state)
{
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(		m_pcSubPara_O2act 
		&&	m_pcSubPara_O2act->IsWindowVisible() 
		&&	getModel()->getDATAHANDLER()->GetOxyCalRunning()==false
		&& m_bViewPraBtnShown)
	{
		int iValue=getModel()->getDATAHANDLER()->getMessureDataO2();
		m_pcSubPara_O2act->SetParaValue(iValue,true);
	}

	if(state)
	{
		/*if(m_pcPara_O2)
			m_pcPara_O2->EnableWindow(FALSE);

		if(m_pcPara_O2Flush)
			m_pcPara_O2Flush->EnableWindow(FALSE);*/
		if(m_pcPara_O2)
			m_pcPara_O2->SetBitmaps(m_pcPara_DisB,m_pcPara_DwB,m_pcPara_UpB,m_pcPara_FcB,false,true);
	}
	else
	{
		/*if(m_pcPara_O2)
			m_pcPara_O2->EnableWindow(TRUE);

		if(getModel()->getDATAHANDLER()->GetCurO2FlushTime()==0)
		{
			if(m_pcPara_O2Flush)
			{
				m_pcPara_O2Flush->SetOffState(true);
				m_pcPara_O2Flush->EnableWindow(FALSE);
			}
		}
		else
		{
			if(m_pcPara_O2Flush)
			{
				m_pcPara_O2Flush->SetOffState(false);
				m_pcPara_O2Flush->EnableWindow(TRUE);
			}
		}*/

		/*if(m_pcPara_O2Flush)
			m_pcPara_O2Flush->EnableWindow(FALSE);*/
		if(m_pcPara_O2)
			m_pcPara_O2->SetBitmaps(m_pcPara_UpB,m_pcPara_DwB,m_pcPara_UpB,m_pcPara_FcB,false,true);
	}
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);

}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::setPSVapnoe(bool bState)
{
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_bPsvApnoe!=bState)
	{
		m_bPsvApnoe=bState;

		bool bSetIFlowArrow=false;

		if(getModel()->getALARMHANDLER()->ALARM_SysLimit_Vgaranty->getAlarmState()!=AS_NONE)
		{
			if(getModel()->getVGarantyAlarmFlag()==VGAL_LOW)
			{
				int iState=getModel()->getDATAHANDLER()->getStatus2();

				if(getModel()->getVMODEHANDLER()->activeModeIsVM_PSV())
				{
					if(m_pcPara_PINSP)
						m_pcPara_PINSP->SetAlarmArrowUp(false);

					if(iState&BIT6)
					{
						bSetIFlowArrow=true;
						
						if(m_pcPara_PPSV)
							m_pcPara_PPSV->SetAlarmArrowUp(false);
						if(m_pcPara_PINSP)
							m_pcPara_PINSP->SetAlarmArrowUp(false);
					}
					else
					{
						if(bState)
						{
							if(m_pcPara_PPSV)
								m_pcPara_PPSV->SetAlarmArrowUp(false);
							if(m_pcPara_PINSP)
								m_pcPara_PINSP->SetAlarmArrowUp();
						}
						else
						{
							if(m_pcPara_PPSV)
								m_pcPara_PPSV->SetAlarmArrowUp();
							if(m_pcPara_PINSP)
								m_pcPara_PINSP->SetAlarmArrowUp(false);
						}
					}
				}
				else
				{	
					if(m_pcPara_PPSV)
						m_pcPara_PPSV->SetAlarmArrowUp(false);

					if(iState&BIT6)
					{
						bSetIFlowArrow=true;
						if(m_pcPara_PINSP)
							m_pcPara_PINSP->SetAlarmArrowUp(false);
					}
					else
					{
						if(m_pcPara_PINSP)
							m_pcPara_PINSP->SetAlarmArrowUp();
					}
				}

				if(m_pcPara_HFAMPL)
					m_pcPara_HFAMPL->SetAlarmArrowUp();
			}
		}
		else
		{
			if(m_pcPara_PPSV)
				m_pcPara_PPSV->SetAlarmArrowUp(false);
			if(m_pcPara_PINSP)
				m_pcPara_PINSP->SetAlarmArrowUp(false);
			if(m_pcPara_HFAMPL)
				m_pcPara_HFAMPL->SetAlarmArrowUp(false);
		}

		if(getModel()->getALARMHANDLER()->ALARM_SysLimit_Pinsp_NotReached->getAlarmState()!=AS_NONE)
		{
			bSetIFlowArrow=true;
		}

		if(bSetIFlowArrow)
		{
			if(m_pcPara_IFLOW)
				m_pcPara_IFLOW->SetAlarmArrowUp(true);
		}
		else
		{
			if(m_pcPara_IFLOW)
				m_pcPara_IFLOW->SetAlarmArrowUp(false);
		}
	}
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}

// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::SetTriggeredBreath()
{
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(		m_pcSubPara_TrigThreshold 
		&&	getModel()->getVMODEHANDLER()->activeModeIsTrigger()
		&&	m_pcSubPara_TrigThreshold->IsWindowVisible())
	{
		m_pcSubPara_TrigThreshold->TriggerBreath();
	}
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}

// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::SetOxyHourglass(bool state)
{
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcSubPara_O2act && m_pcSubPara_O2act->IsWindowVisible())
	{
		m_pcSubPara_O2act->SetTimerRunning(state);
	}
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}

// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::NotifyDataChanged()
{
	PostMessage(WM_DATA_CHANGED);
}

// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::setOxyDiffParaBTN(int iValOxy)//PRICO04
{
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_O2Flush)
		m_pcPara_O2Flush->CheckValue(iValOxy);
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}

//void CViewParaBtn::setOxyPPRICO(int iValOxy)//PRICO04
//{
//	/** \brief The enter critical section. */
//	EnterCriticalSection(&csDraw);
//	fVALUE fv;
//	fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetO2MinPara();
//	fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetO2MaxPara();
//	fv.iValue=iValOxy;
//
//	if(m_pcPara_O2)
//		m_pcPara_O2->SetValue(fv, true);
//
//	if(m_pcPara_O2Flush)
//		m_pcPara_O2Flush->CheckValue(iValOxy);
//	/** \brief The leave critical section. */
//	LeaveCriticalSection(&csDraw);
//}

//void CViewParaBtn::setPmeanHFO(int iValPmean, bool bSendPmeanDif, bool bSendPmanual)//PMAN1
//{
//	if(true==getModel()->getDATAHANDLER()->isHFOManBreathAvailable())
//	{
//		setPmanualHFO(iValPmean,bSendPmanual);
//	}
//
//	if(true==getModel()->getDATAHANDLER()->isLUNGRECLicenseAvailable())
//	{
//		setPmeanDiff(iValPmean,bSendPmeanDif);
//	}
//}
//void CViewParaBtn::setPmanualHFO(int iValPmean, bool bSendPmanual)//PMAN1
//{
//	DEBUGMSG(TRUE, (TEXT("CViewParaBtn::setPmanual\r\n")));
//	/** \brief The enter critical section. */
//	EnterCriticalSection(&csDraw);
//	//int iTemp=iValPmean+getModel()->getDATAHANDLER()->getPmeanDifference();
//	fVALUE fvPmanual;
//	if(m_pcPara_PMANUAL)
//		m_pcPara_PMANUAL->GetValue(&fvPmanual);
//
//	//if(getModel()->getDATAHANDLER()->PARADATA()->GetPManualPara()<iValPmean+10)
//	if(fvPmanual.iValue<iValPmean+10)
//	{
//		fvPmanual.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPManualMaxPara();
//
//		if(iValPmean<=fvPmanual.iUpperLimit)
//		{
//			fvPmanual.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanPara();
//			fvPmanual.iValue=iValPmean;
//
//			if(m_pcPara_PMANUAL)
//				m_pcPara_PMANUAL->SetValue(fvPmanual,true);
//		}
//		else
//		{
//			bSendPmanual=false;
//		}
//	}
//	else
//	{
//		bSendPmanual=false;
//	}
//
//	/** \brief The leave critical section. */
//	LeaveCriticalSection(&csDraw);
//
//	if(bSendPmanual)
//		getModel()->getDATAHANDLER()->SetPManualHFOParadata(fvPmanual.iValue);
//}
// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::setPmeanDiff(int iValPmean, bool bSend)//PMAN1
{
	//DEBUGMSG(TRUE, (TEXT("CViewParaBtn::setPmeanDiff\r\n")));
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	int iTemp=iValPmean+getModel()->getDATAHANDLER()->getPmeanDifference();
	
	if(getModel()->getDATAHANDLER()->isLUNGRECLicenseAvailable()==true)
	{
		fVALUE fvPmeanRec;

		fvPmeanRec.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanRecMinPara ();
		fvPmeanRec.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanRecMaxPara();

		if(iTemp<=fvPmeanRec.iUpperLimit)
		{
			fvPmeanRec.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanRecMinPara();
			fvPmeanRec.iValue=iTemp;

			if(fvPmeanRec.iValue>getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanRecMaxKey())
			{
				if(m_pcPara_PMEAN_REC)
					m_pcPara_PMEAN_REC->SetWarning(true);
			}
			else if(fvPmeanRec.iValue<=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanRecMaxKey())
			{
				if(m_pcPara_PMEAN_REC)
					m_pcPara_PMEAN_REC->SetWarning(false);
			}

			if(m_pcPara_PMEAN_REC)
				m_pcPara_PMEAN_REC->SetValue(fvPmeanRec,true);
		}
		else
		{
			bSend=false;
		}
	}
	

	if(getModel()->getCONFIG()->IsHFOManBreathEnabled())//PMAN2
	{
		fVALUE fvPman;
		if(getModel()->getCONFIG()->GetCurMode()==VM_PRE_HFO)
			fvPman.iValue=getModel()->getDATAHANDLER()->PRESET()->GetPManualHFOPara();
		else
			fvPman.iValue=getModel()->getDATAHANDLER()->PARADATA()->GetPManualHFOPara();

		
		if(iValPmean+5>fvPman.iValue)
		{
			//if(iValPmean%10==0)
			{
				fvPman.iValue=iValPmean+10;
			}
			/*else
			{
				fvPman.iValue=iValPmean+5;
			}*/
			fvPman.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPManualMinPara();
			fvPman.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPManualMaxPara();

			if(fvPman.iValue>getModel()->getDATAHANDLER()->GetCurrentPManualMaxKey())
			{
				if(m_pcPara_PMANUAL)
					m_pcPara_PMANUAL->SetWarning(true);
			}
			/*else
			{
				if(m_pcPara_PMANUAL)
					m_pcPara_PMANUAL->SetWarning(false);
			}*/
			if(m_pcPara_PMANUAL)
				m_pcPara_PMANUAL->SetValue(fvPman,true);

			getModel()->getDATAHANDLER()->SetCurrentPManPara(fvPman.iValue);
		}
	}
	
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);

	if(getModel()->getDATAHANDLER()->isLUNGRECLicenseAvailable()==true)
	{
		if(bSend)
			getModel()->getDATAHANDLER()->SetCurrentHFPMeanRecPara(iTemp);
	}
	
}

void CViewParaBtn::setPmeanRecDiff(int iValPmeanRec, bool bSend)//PMAN1
{
	int iDifference=iValPmeanRec-getModel()->getDATAHANDLER()->GetCurrentPmeanPara();
	if(iDifference<MAXDIFF_PMEANREC)
		iDifference=MAXDIFF_PMEANREC;

	getModel()->getDATAHANDLER()->setPmeanRecDifference(iDifference);
}

// **************************************************************************
// 
// **************************************************************************
SHORT CViewParaBtn::getITimeDisplayedValue()
{
	fVALUE fv;
	fv.iValue=0;

	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_ITIME)
	{
		m_pcPara_ITIME->GetValue(&fv);
	}
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
	return fv.iValue;
}
// **************************************************************************
// 
// **************************************************************************
SHORT CViewParaBtn::getPpsvDisplayedValue()
{
	fVALUE fv;
	fv.iValue=0;
	
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_PPSV)
	{
		m_pcPara_PPSV->GetValue(&fv);
	}
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
	return fv.iValue;
}

SHORT CViewParaBtn::getPEEPdisplayedValue()
{
	fVALUE fv;
	fv.iValue=0;
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_PEEP)
	{
		m_pcPara_PEEP->GetValue(&fv);
	}
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
	return fv.iValue;
}

// **************************************************************************
// 
// **************************************************************************
void CViewParaBtn::setPEEPvalue(int iValPEEP, bool bSend)
{
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(TRUE==getModel()->getCONFIG()->isPpsvAsDeltaPEEPValue())
	{
		if(		getModel()->getCONFIG()->GetCurMode()==VM_PSV
			||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_PSV
			||	getModel()->getCONFIG()->GetCurMode()==VM_SIMVPSV
			||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_SIMVPSV)//newVG
		{
			fVALUE fvPpsv;
			if(m_pcPara_PPSV)
				m_pcPara_PPSV->GetValue(&fvPpsv);
			int iPEEP_PPSVdiff=getModel()->getDATAHANDLER()->getPEEP_PpsvDifference();
			
			//int iCurPPSV=getModel()->getDATAHANDLER()->GetCurrentPpsvPara(); //newVG

			if(bSend && fvPpsv.iValue!=iValPEEP+iPEEP_PPSVdiff)
			{
				getModel()->getDATAHANDLER()->SetCurrentPpsvPara(iValPEEP+iPEEP_PPSVdiff);
				if(fvPpsv.iValue!=iValPEEP+iPEEP_PPSVdiff)
				{
					fvPpsv.iValue=iValPEEP+iPEEP_PPSVdiff;
					if(m_pcPara_PPSV)
						m_pcPara_PPSV->SetValue(fvPpsv,true);
				}
			}

		}
		else if(		getModel()->getCONFIG()->GetCurMode()==VM_PSV
			||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_PSV)
		{
			fVALUE fvPpsv;
			if(m_pcPara_PPSV)
				m_pcPara_PPSV->GetValue(&fvPpsv);
			int iPEEP_PPSVdiff=getModel()->getDATAHANDLER()->getPEEP_PpsvDifference();
			
			//int iCurPPSV=getModel()->getDATAHANDLER()->GetCurrentPpsvPara(); //newVG

			if(bSend && fvPpsv.iValue!=iValPEEP+iPEEP_PPSVdiff)
			{
				getModel()->getDATAHANDLER()->SetCurrentPpsvPara(iValPEEP+iPEEP_PPSVdiff);
				if(fvPpsv.iValue!=iValPEEP+iPEEP_PPSVdiff)
				{
					fvPpsv.iValue=iValPEEP+iPEEP_PPSVdiff;
					if(m_pcPara_PPSV)
						m_pcPara_PPSV->SetValue(fvPpsv,true);
				}
			}
		}
	}
	else
	{
		if(		getModel()->getCONFIG()->GetCurMode()==VM_PSV
			||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_PSV
			||	getModel()->getCONFIG()->GetCurMode()==VM_SIMVPSV
			||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_SIMVPSV)//newVG
		{
			fVALUE fvPpsv;
			if(m_pcPara_PPSV)
				m_pcPara_PPSV->GetValue(&fvPpsv);
			int iPEEP_PPSVdiff=getModel()->getDATAHANDLER()->getPEEP_PpsvDifference();
			int fPinsp = getModel()->getDATAHANDLER()->GetCurrentPINSPPara();

			if(iValPEEP+iPEEP_PPSVdiff>fPinsp)
			{
				if(bSend)
				{
					int iDiff=fvPpsv.iValue-iValPEEP;
					getModel()->getDATAHANDLER()->setPEEP_PpsvDifference(iDiff);
				}
			}
			else if(fvPpsv.iValue==iValPEEP+iPEEP_PPSVdiff)
			{
				int iCurPPSV=getModel()->getDATAHANDLER()->GetCurrentPpsvPara();

				if(bSend && fvPpsv.iValue!=iCurPPSV)
				{
					getModel()->getDATAHANDLER()->SetCurrentPpsvPara(fvPpsv.iValue);
				}
			}
			else
			{
				fvPpsv.iValue=iValPEEP+iPEEP_PPSVdiff;
				if(m_pcPara_PPSV)
					m_pcPara_PPSV->SetValue(fvPpsv,true);

				if(bSend)
				{
					getModel()->getDATAHANDLER()->SetCurrentPpsvPara(fvPpsv.iValue);
				}
			}
		}
		else if(		getModel()->getCONFIG()->GetCurMode()==VM_PSV
			||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_PSV)
		{
			fVALUE fvPpsv;
			if(m_pcPara_PPSV)
				m_pcPara_PPSV->GetValue(&fvPpsv);
			int iPEEP_PPSVdiff=getModel()->getDATAHANDLER()->getPEEP_PpsvDifference();
			int fPpsv = getModel()->getDATAHANDLER()->GetCurrentPpsvPara();

			if(fvPpsv.iValue==iValPEEP+iPEEP_PPSVdiff)
			{
				int iCurPPSV=getModel()->getDATAHANDLER()->GetCurrentPpsvPara();

				if(bSend && fvPpsv.iValue!=iCurPPSV)
				{
					getModel()->getDATAHANDLER()->SetCurrentPpsvPara(fvPpsv.iValue);
				}
			}
			else
			{
				fvPpsv.iValue=iValPEEP+iPEEP_PPSVdiff;
				if(m_pcPara_PPSV)
					m_pcPara_PPSV->SetValue(fvPpsv,true);

				if(bSend)
				{
					getModel()->getDATAHANDLER()->SetCurrentPpsvPara(fvPpsv.iValue);
				}
			}
		}
	}
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);
}
void CViewParaBtn::setPpsvValue(int iValPPSV, bool bWriteConfig)
{

	fVALUE fvPEEP;
	/** \brief The enter critical section. */
	EnterCriticalSection(&csDraw);
	if(m_pcPara_PEEP)
		m_pcPara_PEEP->GetValue(&fvPEEP);

	int iDifference=iValPPSV-fvPEEP.iValue;
	if(iDifference<DIFF_PEEP_PINSP)
	{
		iDifference=DIFF_PEEP_PINSP;
	}
	/** \brief The leave critical section. */
	LeaveCriticalSection(&csDraw);

	getModel()->getDATAHANDLER()->setPEEP_PpsvDifference(iDifference);

}







