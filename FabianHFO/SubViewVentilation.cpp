// SubViewVentilation.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "SubViewVentilation.h"
#include "DlgMessageBox.h"
#include "TlsFile.h"

#define COLOR_TXTBTNUP				0x00000000
#define COLOR_TXTBTNDW				0x00FFFFFF
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


IMPLEMENT_DYNAMIC(CSubViewVentilation, CWnd)

CSubViewVentilation::CSubViewVentilation()
{
	m_pModel=NULL;
	m_hDC=NULL;
	m_hBmp=NULL;
	m_hBmpPrev=NULL;
	m_lX=0;
	m_lY=0;
	m_lXo=0;
	m_lYo=0;

	m_pcRegionTopLeCorner=NULL;
	m_pcRegionTopRiCorner=NULL;
	m_pcRegionBotLeCorner=NULL;
	m_pcRegionBotRiCorner=NULL;

	m_pcSetupVent_Up=NULL;
	m_pcSetupVent_Dw=NULL;
	m_pcSetupVent_Fc=NULL;
	m_pcSetupVent_Dis=NULL;

	m_pcDefaults_Up=NULL;
	m_pcDefaults_Dw=NULL;
	m_pcDefaults_Fc=NULL;
	m_pcDefaults=NULL;

	m_pcHospital_Up=NULL;
	m_pcHospital_Dw=NULL;
	m_pcHospital_Fc=NULL;
	m_pcHospital=NULL;

	m_iCurPara=IDC_BTN_SETUP_AUTOOXYCAL;//IDC_BTN_SETUP_RANGE;

	m_pDlg = NULL;

	m_iManBreathTime=getModel()->getCONFIG()->GetManBreathTime();
	//m_byFlushTime=getModel()->getDATAHANDLER()->GetCurO2FlushTime();
	m_eModeITime=getModel()->getCONFIG()->GetIERelationMode();
	m_eRefFlowSensor=getModel()->getCONFIG()->GetRefFlowSensor();
	m_eTriggerType=getModel()->getDATAHANDLER()->getTriggerOptionCONV();
	m_eTubeSet=getModel()->getDATAHANDLER()->GetTubeSet();
	m_ePressureUnit=getModel()->getCONFIG()->GetPressureUnit();
	if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
	{
		m_iEFlow=getModel()->getDATAHANDLER()->PARADATA()->GetEFLOWPara_IPPV();
	}
	else
	{
		m_iEFlow=getModel()->getDATAHANDLER()->PARADATA()->GetEFLOWPara_TRIGGER();
	}
	
	m_eAutoOxyCal=getModel()->getCONFIG()->GetAutoOxyCal();
	m_bHFOManBreathEnabled=getModel()->getCONFIG()->IsHFOManBreathEnabled();
	m_iPatientAlarmDelay=getModel()->getCONFIG()->getCurPatientAlarmDelay();
	m_bPpsvAsDeltaPEEPValue=getModel()->getCONFIG()->isPpsvAsDeltaPEEPValue();
	m_eLeakComp=getModel()->getCONFIG()->getLeakCompensation();
	m_bUseTveBTB=getModel()->getCONFIG()->useTveBTB();

	m_kUp					= VK_UP;//up dw revers
	m_kDown					= VK_DOWN;
	m_kSpace				= VK_SPACE;

	
	m_pcLeakComp=NULL;
	m_pcFuncITime=NULL;
	m_pcManBreathTime=NULL;
	m_pcTrigger=NULL;
	m_pcTubeSet=NULL;
	m_pcPressureUnit=NULL;
	m_pcEFlow=NULL;
	m_pcPatientAlarmDelay=NULL;
	m_pcAutoOxyCal=NULL;
	m_pcHFOManBreath=NULL;
	m_pcPPSVasDeltaPEEPValue=NULL;
	m_pcBreathToBreathVT=NULL;

	m_dwLastSetupTimer=0;
}

CSubViewVentilation::~CSubViewVentilation()
{
	if(m_pcRegionTopLeCorner)
		delete m_pcRegionTopLeCorner;
	m_pcRegionTopLeCorner=NULL;
	if(m_pcRegionTopRiCorner)
		delete m_pcRegionTopRiCorner;
	m_pcRegionTopRiCorner=NULL;
	if(m_pcRegionBotLeCorner)
		delete m_pcRegionBotLeCorner;
	m_pcRegionBotLeCorner=NULL;
	if(m_pcRegionBotRiCorner)
		delete m_pcRegionBotRiCorner;
	m_pcRegionBotRiCorner=NULL;

	if(m_pcSetupVent_Up)
		delete m_pcSetupVent_Up;
	if(m_pcSetupVent_Dw)
		delete m_pcSetupVent_Dw;
	if(m_pcSetupVent_Fc)
		delete m_pcSetupVent_Fc;
	if(m_pcSetupVent_Dis)
		delete m_pcSetupVent_Dis;
	m_pcSetupVent_Up=NULL;
	m_pcSetupVent_Dw=NULL;
	m_pcSetupVent_Fc=NULL;
	m_pcSetupVent_Dis=NULL;

	if(m_pcDefaults_Up)
		delete m_pcDefaults_Up;
	if(m_pcDefaults_Dw)
		delete m_pcDefaults_Dw;
	if(m_pcDefaults_Fc)
		delete m_pcDefaults_Fc;
	if(m_pcDefaults)
		delete m_pcDefaults;

	m_pcDefaults_Up=NULL;
	m_pcDefaults_Dw=NULL;
	m_pcDefaults_Fc=NULL;
	m_pcDefaults=NULL;

	if(m_pcHospital_Up)
		delete m_pcHospital_Up;
	if(m_pcHospital_Dw)
		delete m_pcHospital_Dw;
	if(m_pcHospital_Fc)
		delete m_pcHospital_Fc;
	if(m_pcHospital)
		delete m_pcHospital;

	m_pcHospital_Up=NULL;
	m_pcHospital_Dw=NULL;
	m_pcHospital_Fc=NULL;
	m_pcHospital=NULL;

	if(m_pcLeakComp)
		delete m_pcLeakComp;
	m_pcLeakComp=NULL;
	if(m_pcFuncITime)
		delete m_pcFuncITime;
	m_pcFuncITime=NULL;
	if(m_pcManBreathTime)
		delete m_pcManBreathTime;
	m_pcManBreathTime=NULL;
	if(m_pcTrigger)
		delete m_pcTrigger;
	m_pcTrigger=NULL;
	if(m_pcTubeSet)
		delete m_pcTubeSet;
	m_pcTubeSet=NULL;
	if(m_pcPressureUnit)
		delete m_pcPressureUnit;
	m_pcPressureUnit=NULL;
	if(m_pcEFlow)
		delete m_pcEFlow;
	m_pcEFlow=NULL;

	if(m_pcAutoOxyCal)
		delete m_pcAutoOxyCal;
	m_pcAutoOxyCal=NULL;
	if(m_pcPPSVasDeltaPEEPValue)
		delete m_pcPPSVasDeltaPEEPValue;
	m_pcPPSVasDeltaPEEPValue=NULL;

	if(m_pcBreathToBreathVT)
		delete m_pcBreathToBreathVT;
	m_pcBreathToBreathVT=NULL;

	if(m_pcPatientAlarmDelay)
		delete m_pcPatientAlarmDelay;
	m_pcPatientAlarmDelay=NULL;
	if(m_pcHFOManBreath)
		delete m_pcHFOManBreath;
	m_pcHFOManBreath=NULL;

}


BEGIN_MESSAGE_MAP(CSubViewVentilation, CWnd)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_SETUP_LEAKCOMP, &CSubViewVentilation::OnBnClickedLeakComp)
	ON_BN_CLICKED(IDC_BTN_SETUP_FUNCITIME, &CSubViewVentilation::OnBnClickedModeIT)
	ON_BN_CLICKED(IDC_BTN_SETUP_MANBREATHTIME, &CSubViewVentilation::OnBnClickedManBreathTime)
	ON_BN_CLICKED(IDC_BTN_SETUP_PATALARMDELAY, &CSubViewVentilation::OnBnClickedPatAlarmDelay)
	ON_BN_CLICKED(IDC_BTN_SETUP_PPSVABSOLUTE, &CSubViewVentilation::OnBnClickedPPSVasDeltaPEEPValue)
	ON_BN_CLICKED(IDC_BTN_SETUP_BTBVT, &CSubViewVentilation::OnBnClickedBTBforVT)
	ON_BN_CLICKED(IDC_BTN_SETUP_VOLUMETRIG, &CSubViewVentilation::OnBnClickedVolumeTrigger)
	ON_BN_CLICKED(IDC_BTN_SETUP_TUBESET, &CSubViewVentilation::OnBnClickedTubeSet)
	ON_BN_CLICKED(IDC_BTN_SETUP_PRESSUREUNIT, &CSubViewVentilation::OnBnClickedPressureUnit)
	ON_BN_CLICKED(IDC_BTN_SETUP_EFLOW, &CSubViewVentilation::OnBnClickedEFlow)
	ON_BN_CLICKED(IDC_BTN_SETUP_AUTOOXYCAL, &CSubViewVentilation::OnBnClickedAutoOxyCal)
	ON_BN_CLICKED(IDC_BTN_SETUP_HFOMANBREATH, &CSubViewVentilation::OnBnClickedHFOManBreath)
	ON_BN_CLICKED(IDC_BTN_SETUP_DEFAULTS, &CSubViewVentilation::OnBnClickedDefaults)
	ON_BN_CLICKED(IDC_BTN_SETUP_HOSPITAL, &CSubViewVentilation::OnBnClickedHospital)
END_MESSAGE_MAP()



// CSubViewVentilation message handlers


// **************************************************************************
// 
// **************************************************************************
CMVModel *CSubViewVentilation::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}

// **************************************************************************
// 
// **************************************************************************
bool CSubViewVentilation::CreateWnd(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext)
{
	if (!Create(pParentWnd, rc, nID, pContext))
	{
		return false;
	}

	return true;
}

// **************************************************************************
// 
// **************************************************************************
BOOL CSubViewVentilation::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext) 
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

		//CBrush cbrBack(RGB(230,230,200));
		CBrush cbrBack(BACKGND);
		HBRUSH hbrprev=(HBRUSH)SelectObject(m_hDC,cbrBack);
		HPEN hpenprev=(HPEN)SelectObject(m_hDC, (HPEN)GetStockObject(NULL_PEN));	

		Rectangle(m_hDC, 0, 0, m_lX, m_lY);

		m_pcDefaults_Up		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_FACTORY_UP);
		m_pcDefaults_Dw		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_FACTORY_DW);
		m_pcDefaults_Fc		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_FACTORY_FC);
		
		m_pcHospital_Up		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_HOSPITAL_UP);
		m_pcHospital_Dw		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_HOSPITAL_DW);
		m_pcHospital_Fc		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_HOSPITAL_FC);


		m_pcSetupVent_Up		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_SETUPVENT_UP);
		m_pcSetupVent_Dw		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_SETUPVENT_DW);
		m_pcSetupVent_Fc		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_SETUPVENT_FC);
		m_pcSetupVent_Dis		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_SETUPVENT_DIS);

		m_pcRegionTopLeCorner		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_SETUP_LE_TOP_CORNER);
		m_pcRegionTopRiCorner		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_SETUP_RI_TOP_CORNER);
		m_pcRegionBotLeCorner		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_SETUP_LE_BOT_CORNER);
		m_pcRegionBotRiCorner		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_SETUP_RI_BOT_CORNER);

		CStringW szText=_T("");


		BTN btn;

		//###################################1. AutoOxyCal 21 // 21 & 100 ###########################
		btn.wID					= IDC_BTN_SETUP_AUTOOXYCAL;	
		btn.poPosition.x		= 21;
		btn.poPosition.y		= 30;
		btn.pcBmpUp				= m_pcSetupVent_Up;
		btn.pcBmpDown			= m_pcSetupVent_Dw;
		btn.pcBmpFocus			= m_pcSetupVent_Fc;
		btn.pcBmpDisabled		= m_pcSetupVent_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pcAutoOxyCal=new CSelectSetupBtn(btn,COLOR_TXTBTNUP,true);
		m_pcAutoOxyCal->Create(this,g_hf8AcuBold,0);
		m_pcAutoOxyCal->SetText(getModel()->GetLanguageString(IDS_TXT_AUTOCAL_OXY)+_T(":"));

		if(m_eAutoOxyCal==CAL_21)
		{
			m_pcAutoOxyCal->SetValueText(getModel()->GetLanguageString(IDS_TXT_21PERC));
		}
		else
		{
			m_pcAutoOxyCal->SetValueText(getModel()->GetLanguageString(IDS_TXT_21_100PERC));
		}
		m_pcAutoOxyCal->ShowWindow(SW_SHOW);
		m_plBtn.AddTail(m_pcAutoOxyCal);


		//################################### 2. Leakage compensation ########################### //pro, added
		btn.wID					= IDC_BTN_SETUP_LEAKCOMP;
		btn.poPosition.x		= 21;
		btn.poPosition.y		= 94;
		btn.pcBmpUp				= m_pcSetupVent_Up;
		btn.pcBmpDown			= m_pcSetupVent_Dw;
		btn.pcBmpFocus			= m_pcSetupVent_Fc;
		btn.pcBmpDisabled		= m_pcSetupVent_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pcLeakComp=new CSelectSetupBtn(btn,COLOR_TXTBTNUP,true);
		m_pcLeakComp->Create(this,g_hf8AcuBold,0);

		m_pcLeakComp->SetText(getModel()->GetLanguageString(IDS_TXT_LEAKCOMP)+_T(":"));

		/*if(TRUE==m_bLeakCompOff)
		{
			m_pcLeakComp->SetValueText(getModel()->GetLanguageString(IDS_TXT_DISABLED));
		}
		else
		{
			m_pcLeakComp->SetValueText(getModel()->GetLanguageString(IDS_TXT_ENABLED));
		}*/
		switch(m_eLeakComp)
		{
		/*case LC_OFF:
			{
				m_pcLeakComp->SetValueText(getModel()->GetLanguageString(IDS_TXT_OFF));
			}
			break;*/
		case LC_LOW:
			{
				m_pcLeakComp->SetValueText(getModel()->GetLanguageString(IDS_TXT_SENS_LOW));
			}
			break;
		case LC_MIDDLE:
			{
				m_pcLeakComp->SetValueText(getModel()->GetLanguageString(IDS_TXT_SENS_MID));
			}
			break;
		case LC_HIGH:
			{
				m_pcLeakComp->SetValueText(getModel()->GetLanguageString(IDS_TXT_SENS_HIGH));
			}
			break;
		}

		if(getModel()->getDATAHANDLER()->isNMODELicenseAvailable()==true) //Leak - license dependency
		{
			m_pcLeakComp->ShowWindow(SW_SHOW);
			m_plBtn.AddTail(m_pcLeakComp);
		} 
		else
		{
			m_pcLeakComp->ShowWindow(SW_HIDE);
		}


		//################################### 3. PRESSUREUNIT ###########################
		btn.wID					= IDC_BTN_SETUP_PRESSUREUNIT;	
		btn.poPosition.x		= 21;
		btn.poPosition.y		= 158;
		btn.pcBmpUp				= m_pcSetupVent_Up;
		btn.pcBmpDown			= m_pcSetupVent_Dw;
		btn.pcBmpFocus			= m_pcSetupVent_Fc;
		btn.pcBmpDisabled		= m_pcSetupVent_Dis;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pcPressureUnit=new CSelectSetupBtn(btn,COLOR_TXTBTNUP,true);
		m_pcPressureUnit->Create(this,g_hf8AcuBold,0);
		m_pcPressureUnit->SetText(getModel()->GetLanguageString(IDS_TXT_PRESSUREUNIT)+_T(":"));
		//m_pcPressureUnit->SetText(_T("Unit of pressure:"));
		if(m_ePressureUnit==PUNIT_MBAR)
		{
			m_pcPressureUnit->SetValueText(getModel()->GetLanguageString(IDS_UNIT_MBAR));
		}
		else 
		{
			m_pcPressureUnit->SetValueText(getModel()->GetLanguageString(IDS_UNIT_CMH2O));
		}
		m_pcPressureUnit->ShowWindow(SW_SHOW);

		m_plBtn.AddTail(m_pcPressureUnit);

		//################################### 4. TUBESET ###########################
		btn.wID					= IDC_BTN_SETUP_TUBESET;	
		btn.poPosition.x		= 21;
		btn.poPosition.y		= 222;
		btn.pcBmpUp				= m_pcSetupVent_Up;
		btn.pcBmpDown			= m_pcSetupVent_Dw;
		btn.pcBmpFocus			= m_pcSetupVent_Fc;
		btn.pcBmpDisabled		= m_pcSetupVent_Dis;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pcTubeSet=new CSelectSetupBtn(btn,COLOR_TXTBTNUP,true);
		m_pcTubeSet->Create(this,g_hf8AcuBold,0);
		m_pcTubeSet->SetText(getModel()->GetLanguageString(IDS_TXT_NIV)+_T(" ")+getModel()->GetLanguageString(IDS_TXT_TUBESET)+_T(":"));
		if(m_eTubeSet==TUBE_MEDIJET)
		{
			m_pcTubeSet->SetValueText(getModel()->GetLanguageString(IDS_TXT_MEDIJET));
		}
		else if(m_eTubeSet==TUBE_INFANTFLOW)
		{
			m_pcTubeSet->SetValueText(getModel()->GetLanguageString(IDS_TXT_INFANTFLOW));
		}
		else //TUBE_INFANTFLOW_LP
		{
			m_pcTubeSet->SetValueText(getModel()->GetLanguageString(IDS_TXT_INFANTFLOWLP));
		}
		m_pcTubeSet->ShowWindow(SW_SHOW);

		if(getModel()->getCONFIG()->GetTubeConfig()!=TUBECFG_SELECTABLE)
		{
			m_pcTubeSet->EnableWindow(FALSE);
		}
		else
		{
			m_plBtn.AddTail(m_pcTubeSet);
		}

		//################################### 5. MANBREATHTIME ###########################
		btn.wID					= IDC_BTN_SETUP_MANBREATHTIME;	
		btn.poPosition.x		= 21;
		btn.poPosition.y		= 286;
		btn.pcBmpUp				= m_pcSetupVent_Up;
		btn.pcBmpDown			= m_pcSetupVent_Dw;
		btn.pcBmpFocus			= m_pcSetupVent_Fc;
		btn.pcBmpDisabled		= m_pcSetupVent_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pcManBreathTime=new CSelectSetupBtn(btn,COLOR_TXTBTNUP,true);
		m_pcManBreathTime->Create(this,g_hf8AcuBold,0);
		m_pcManBreathTime->SetText(getModel()->GetLanguageString(IDS_TXT_MANBREATHTIME)+_T(":"));
		CStringW szManBreath = _T("");
		szManBreath.Format(_T("%d %s"),m_iManBreathTime,getModel()->GetLanguageString(IDS_UNIT_SECONDS));
		m_pcManBreathTime->SetValueText(szManBreath);
		m_pcManBreathTime->ShowWindow(SW_SHOW);
		m_plBtn.AddTail(m_pcManBreathTime);

		//################################### 6. Patient alarms alarm delay ###########################
		btn.wID					= IDC_BTN_SETUP_PATALARMDELAY;	
		btn.poPosition.x		= 21;
		btn.poPosition.y		= 350;
		btn.pcBmpUp				= m_pcSetupVent_Up;
		btn.pcBmpDown			= m_pcSetupVent_Dw;
		btn.pcBmpFocus			= m_pcSetupVent_Fc;
		btn.pcBmpDisabled		= m_pcSetupVent_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pcPatientAlarmDelay=new CSelectSetupBtn(btn,COLOR_TXTBTNUP,true);
		m_pcPatientAlarmDelay->Create(this,g_hf8AcuBold,0);
		m_pcPatientAlarmDelay->SetText(getModel()->GetLanguageString(IDS_TXT_ALAMDELAY)+_T(":"));

		szText.Format(_T("%d %s"), m_iPatientAlarmDelay,getModel()->GetLanguageString(IDS_UNIT_SECONDS));
		m_pcPatientAlarmDelay->SetValueText(szText);
		m_pcPatientAlarmDelay->ShowWindow(SW_SHOW);
		m_plBtn.AddTail(m_pcPatientAlarmDelay);

		//################################### 7. Enable HOF Manbreath ###########################
		btn.wID					= IDC_BTN_SETUP_HFOMANBREATH;	
		btn.poPosition.x		= 21;
		btn.poPosition.y		=414;
		btn.pcBmpUp				= m_pcSetupVent_Up;
		btn.pcBmpDown			= m_pcSetupVent_Dw;
		btn.pcBmpFocus			= m_pcSetupVent_Fc;
		btn.pcBmpDisabled		= m_pcSetupVent_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pcHFOManBreath=new CSelectSetupBtn(btn,COLOR_TXTBTNUP,true);
		m_pcHFOManBreath->Create(this,g_hf8AcuBold,0);
		m_pcHFOManBreath->SetText(getModel()->GetLanguageString(IDS_TXT_HFOMANBREATH)+_T(":"));

		if(m_bHFOManBreathEnabled)
		{
			m_pcHFOManBreath->SetValueText(getModel()->GetLanguageString(IDS_TXT_ENABLED));
		}
		else
		{
			m_pcHFOManBreath->SetValueText(getModel()->GetLanguageString(IDS_TXT_DISABLED));

		}
		m_pcHFOManBreath->ShowWindow(SW_SHOW);
		m_plBtn.AddTail(m_pcHFOManBreath);

		



		//################################### 8. TRIGGERMODE ###########################
		btn.wID					= IDC_BTN_SETUP_VOLUMETRIG;	
		btn.poPosition.x		= 416;
		btn.poPosition.y		= 30;
		btn.pcBmpUp				= m_pcSetupVent_Up;
		btn.pcBmpDown			= m_pcSetupVent_Dw;
		btn.pcBmpFocus			= m_pcSetupVent_Fc;
		btn.pcBmpDisabled		= m_pcSetupVent_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pcTrigger=new CSelectSetupBtn(btn,COLOR_TXTBTNUP,true);
		m_pcTrigger->Create(this,g_hf8AcuBold,0);
		m_pcTrigger->SetText(getModel()->GetLanguageString(IDS_TXT_TRIGMODE)+_T(":"));
		switch(m_eTriggerType)
		{
		case TRIGGER_VOLUME:
			{
				m_pcTrigger->SetValueText(getModel()->GetLanguageString(IDS_TXT_VOLTRIGGER));
			}
			break;
		case TRIGGER_FLOW:
			{
				m_pcTrigger->SetValueText(getModel()->GetLanguageString(IDS_TXT_FLOWTRIGGER));
			}
			break;
		case TRIGGER_PRESSURE:
			{
				m_pcTrigger->SetValueText(getModel()->GetLanguageString(IDS_TXT_PRETRIGGER));
			}
			break;
		}
		m_pcTrigger->ShowWindow(SW_SHOW);
		m_plBtn.AddTail(m_pcTrigger);
		
		/*if(		(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF
			||	getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_MANOFF)
			&&	getModel()->getDATAHANDLER()->getTriggerOptionCONV()==TRIGGER_PRESSURE)
		{
			m_pcTrigger->EnableWindow(FALSE);
		}
		else
		{
			m_pcTrigger->EnableWindow(TRUE);
			m_plBtn.AddTail(m_pcTrigger);
		}*/

		//################################### 9. EFLOW ###########################
		btn.wID					= IDC_BTN_SETUP_EFLOW;	
		btn.poPosition.x		= 416;
		btn.poPosition.y		= 94;
		btn.pcBmpUp				= m_pcSetupVent_Up;
		btn.pcBmpDown			= m_pcSetupVent_Dw;
		btn.pcBmpFocus			= m_pcSetupVent_Fc;
		btn.pcBmpDisabled		= m_pcSetupVent_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pcEFlow=new CSelectSetupBtn(btn,COLOR_TXTBTNUP,true);
		m_pcEFlow->Create(this,g_hf8AcuBold,0);
		m_pcEFlow->SetText(getModel()->GetLanguageString(IDS_PARA_EFLOW)+_T(":"));

		szText=_T("");
		szText.Format(_T("%0.0f %s"),CTlsFloat::Round(((double)m_iEFlow)/1000, 0),getModel()->GetLanguageString(IDS_UNIT_LMIN));
		m_pcEFlow->SetValueText(szText);
		m_pcEFlow->ShowWindow(SW_SHOW);
		m_plBtn.AddTail(m_pcEFlow);


		//################################### 10. Ratio ###########################
		btn.wID					= IDC_BTN_SETUP_FUNCITIME;	
		btn.poPosition.x		= 416;
		btn.poPosition.y		= 158;
		btn.pcBmpUp				= m_pcSetupVent_Up;
		btn.pcBmpDown			= m_pcSetupVent_Dw;
		btn.pcBmpFocus			= m_pcSetupVent_Fc;
		btn.pcBmpDisabled		= m_pcSetupVent_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pcFuncITime=new CSelectSetupBtn(btn,COLOR_TXTBTNUP,true);
		m_pcFuncITime->Create(this,g_hf8AcuBold,0);
		m_pcFuncITime->SetText(getModel()->GetLanguageString(IDS_TXT_RATIOITIME)+_T(":"));
		if(m_eModeITime==RM_SET_ItoBPM)
		{
			m_pcFuncITime->SetValueText(getModel()->GetLanguageString(IDS_TXT_BPMITIME));
		}
		else if(m_eModeITime==RM_SET_ItoE)
		{
			m_pcFuncITime->SetValueText(getModel()->GetLanguageString(IDS_TXT_ITIMEETIME));
		}
		m_pcFuncITime->ShowWindow(SW_SHOW);
		m_plBtn.AddTail(m_pcFuncITime);


		//################################### 11. PPSV as Delta PEEP ###########################
		btn.wID					= IDC_BTN_SETUP_PPSVABSOLUTE;	
		btn.poPosition.x		= 416;
		btn.poPosition.y		= 222;
		btn.pcBmpUp				= m_pcSetupVent_Up;
		btn.pcBmpDown			= m_pcSetupVent_Dw;
		btn.pcBmpFocus			= m_pcSetupVent_Fc;
		btn.pcBmpDisabled		= m_pcSetupVent_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pcPPSVasDeltaPEEPValue=new CSelectSetupBtn(btn,COLOR_TXTBTNUP,true);
		m_pcPPSVasDeltaPEEPValue->Create(this,g_hf8AcuBold,0);
		
		//m_pcPPSVasDeltaPEEPValue->SetText(_T("Ppsv setting:"));
		m_pcPPSVasDeltaPEEPValue->SetText(getModel()->GetLanguageString(IDS_TXT_PPSV_SETTING)
			+_T(":"));

		if(m_bPpsvAsDeltaPEEPValue)
		{
			//m_pcPPSVasDeltaPEEPValue->SetValueText(_T("above PEEP value"));
			m_pcPPSVasDeltaPEEPValue->SetValueText(getModel()->GetLanguageString(IDS_TXT_PPSV_ABOVEPEEP));
		}
		else
		{
			//m_pcPPSVasDeltaPEEPValue->SetValueText(_T("absolute value"));
			m_pcPPSVasDeltaPEEPValue->SetValueText(getModel()->GetLanguageString(IDS_TXT_PPSV_ABSOLUTE));
		}

		m_pcPPSVasDeltaPEEPValue->ShowWindow(SW_SHOW);
		m_plBtn.AddTail(m_pcPPSVasDeltaPEEPValue);


		//################################### BTBforVT ###########################
		btn.wID					= IDC_BTN_SETUP_BTBVT;	
		btn.poPosition.x		= 416;
		btn.poPosition.y		= 286;
		btn.pcBmpUp				= m_pcSetupVent_Up;
		btn.pcBmpDown			= m_pcSetupVent_Dw;
		btn.pcBmpFocus			= m_pcSetupVent_Fc;
		btn.pcBmpDisabled		= m_pcSetupVent_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pcBreathToBreathVT=new CSelectSetupBtn(btn,COLOR_TXTBTNUP,true);
		m_pcBreathToBreathVT->Create(this,g_hf8AcuBold,0);
		m_pcBreathToBreathVT->SetText(getModel()->GetLanguageString(IDS_TXT_VTASBTB)+_T(":"));
		if(m_bUseTveBTB)
		{
			m_pcBreathToBreathVT->SetValueText(getModel()->GetLanguageString(IDS_TXT_ENABLED));
		}
		else
		{
			m_pcBreathToBreathVT->SetValueText(getModel()->GetLanguageString(IDS_TXT_DISABLED));
		}
		m_pcBreathToBreathVT->ShowWindow(SW_SHOW);
		m_plBtn.AddTail(m_pcBreathToBreathVT);

		//################################### 12. Button Reset Hospital###########################
		btn.wID					= IDC_BTN_SETUP_HOSPITAL;	
		btn.poPosition.x		= 610;
		btn.poPosition.y		= 346;//364;//368;
		btn.pcBmpUp				= m_pcHospital_Up;
		btn.pcBmpDown			= m_pcHospital_Dw;
		btn.pcBmpFocus			= m_pcHospital_Fc;
		btn.pcBmpDisabled		= m_pcHospital_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;
		m_pcHospital = new CSelectSetupBtn(btn,COLOR_TXTBTNUP,false);
		m_pcHospital->Create(this,g_hf8AcuBold,40);
		m_pcHospital->SetText(getModel()->GetLanguageString(IDS_MENU_RESET));
		//bool btest=false;
		if(CTlsFile::Exists(_T("\\FFSDISK\\hospital.dat")))
		{
			m_pcHospital->ShowWindow(SW_SHOW);
			m_plBtn.AddTail(m_pcHospital);
		}
		else
		{
			m_pcHospital->ShowWindow(SW_HIDE);
		}
		

		//################################### 13. Button Reset Factory###########################
		btn.wID					= IDC_BTN_SETUP_DEFAULTS;	
		btn.poPosition.x		= 610;
		btn.poPosition.y		= 410;
		btn.pcBmpUp				= m_pcDefaults_Up;
		btn.pcBmpDown			= m_pcDefaults_Dw;
		btn.pcBmpFocus			= m_pcDefaults_Fc;
		btn.pcBmpDisabled		= m_pcDefaults_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;
		m_pcDefaults = new CSelectSetupBtn(btn,COLOR_TXTBTNUP,false);
		m_pcDefaults->Create(this,g_hf8AcuBold,40);
		m_pcDefaults->SetText(getModel()->GetLanguageString(IDS_MENU_RESET));
		//m_pcDefaults->SetText(getModel()->GetLanguageString(IDS_BTN_DEFAULTS));
		

		if(getModel()->getCONFIG()->IsEnableFactoryActive())
		{
			m_pcDefaults->ShowWindow(SW_SHOW);
			m_plBtn.AddTail(m_pcDefaults);
		}
		else
		{
			m_pcDefaults->ShowWindow(SW_HIDE);
		}
	

		SelectObject(m_hDC,hbrprev);
		SelectObject(m_hDC,hpenprev);

		//cbrBack.DeleteObject();//rkuNEWFIX

		return 1;
	}
	else
		return 0;
}






/***************************************************************************/
//     
//**************************************************************************/
void CSubViewVentilation::Show(bool bShow)
{
	if(bShow)
	{
		ShowWindow(SW_SHOW);
		//SetWindowPos(NULL,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_SHOWWINDOW);

		Draw();

		if(GetParent())
			GetParent()->PostMessage(WM_SET_SETUPTIMER);
		SetOneButtonFocused(getModel()->getCONFIG()->GetLastSelectedSVSettingBtns());
	}
	else
	{
		ShowWindow(SW_HIDE);
		//SetWindowPos(NULL,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_HIDEWINDOW);
	}
}

//************************************
// Method:    OnDestroy
// FullName:  CSubViewVentilation::OnDestroy
// Access:    public 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CSubViewVentilation::OnDestroy()
{
	if(m_pDlg)
	{
		m_pDlg->EndDialog(IDCLOSE);
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_REDRAW_VIEW);
	}
	m_pDlg = NULL;

	m_plBtn.RemoveAll();

	CWnd::OnDestroy();

	if(m_hBmp)
	{
		SelectObject(m_hDC,m_hBmpPrev);
		DeleteObject(m_hBmp);
	}
	if(m_hDC)
		DeleteDC(m_hDC);
}



// **************************************************************************
// 
// **************************************************************************
void CSubViewVentilation::Draw()
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
	HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf17AcuBold);
	int tc=SetTextColor(hdcMem,0x0000000);

	Rectangle(hdcMem, 0, 0, m_lX, m_lY);

	CBrush cbrFill;
	cbrFill.CreateSolidBrush(RGB(181,178,181));
	SelectObject(hdcMem,cbrFill);

	Rectangle(hdcMem, 10, 30, 395, 465);
	Rectangle(hdcMem, 30, 10, 375, 30);
	Rectangle(hdcMem, 30, 440, 375, 485);
	if(m_pcRegionTopLeCorner)
		m_pcRegionTopLeCorner->Draw(hdcMem,10,10);
	if(m_pcRegionTopRiCorner)
		m_pcRegionTopRiCorner->Draw(hdcMem,375,10);
	if(m_pcRegionBotLeCorner)
		m_pcRegionBotLeCorner->Draw(hdcMem,10,465);
	if(m_pcRegionBotRiCorner)
		m_pcRegionBotRiCorner->Draw(hdcMem,375,465);



	Rectangle(hdcMem, 405, 30, 790, 465);
	Rectangle(hdcMem, 425, 10, 770, 30);
	Rectangle(hdcMem, 425, 440, 770, 485);
	if(m_pcRegionTopLeCorner)
		m_pcRegionTopLeCorner->Draw(hdcMem,405,10);
	if(m_pcRegionTopRiCorner)
		m_pcRegionTopRiCorner->Draw(hdcMem,770,10);
	if(m_pcRegionBotLeCorner)
		m_pcRegionBotLeCorner->Draw(hdcMem,405,465);
	if(m_pcRegionBotRiCorner)
		m_pcRegionBotRiCorner->Draw(hdcMem,770,465);

	//CDC* pDCMem=CDC::FromHandle(hdcMem);

	CPen penLine;
	penLine.CreatePen(PS_SOLID,1,RGB(140,140,140));
	SelectObject(hdcMem,(HPEN)penLine);
	CBrush cbrRound(RGB(200,200,200));
	SelectObject(hdcMem,cbrRound);
	
	//#############o2 cal
	RoundRect(hdcMem,20,20,385,80,20,20);

	if(getModel()->getDATAHANDLER()->isNMODELicenseAvailable()==true) //Leak - license dependency
	{
		//#############leak comp
		RoundRect(hdcMem,20,84,385,144,20,20);
	}

	//#############unit pressure
	RoundRect(hdcMem,20,148,385,208,20,20);

	//#############tube set
	RoundRect(hdcMem,20,212,385,272,20,20);

	//#############max time man breath
	RoundRect(hdcMem,20,276,385,336,20,20);

	//#############alarm delay
	RoundRect(hdcMem,20,340,385,400,20,20);

	//#############man breath HFO
	RoundRect(hdcMem,20,404,385,464,20,20);


	//##################################################
	//#############trigger mode
	RoundRect(hdcMem,415,20,780,80,20,20);

	//#############E-Flow
	RoundRect(hdcMem,415,84,780,144,20,20);

	//#############Ratio I-Time
	RoundRect(hdcMem,415,148,780,208,20,20);

	//#############absolute PPSV
	RoundRect(hdcMem,415,212,780,272,20,20);

	//#############absolute BTB fof VT
	RoundRect(hdcMem,415,276,780,336,20,20);



	//#############hospital
	RoundRect(hdcMem,415,340,780,400,20,20);

	//#############factory
	RoundRect(hdcMem,415,404,780,464,20,20);

	//RoundRect(hdcMem, 415, 350, 780, 410,20,20);
	//RoundRect(hdcMem, 415, 357, 780, 417,20,20);


	//#############Range,ManBreathHFO,TubeSet,UnitPressure
	//RoundRect(hdcMem, 20, 20, 385, 203,20,20);
	//MoveToEx(hdcMem, 21, 70, NULL);
	//LineTo(hdcMem, 384, 70);
	//MoveToEx(hdcMem, 21, 111, NULL);
	//LineTo(hdcMem, 384, 111);
	//MoveToEx(hdcMem, 21, 152, NULL);
	//LineTo(hdcMem, 384, 152);

	//#############Flush, AutoCal O2
	//RoundRect(hdcMem, 20, 207, 385, 308,20,20);
	//RoundRect(hdcMem, 20, 207, 385, 267,20,20);
	//MoveToEx(hdcMem, 21, 257, NULL);
	//LineTo(hdcMem, 384, 257);

	//#############Ratio, MaxTime
	//RoundRect(hdcMem, 20, 313-81, 385, 414-81,20,20);
	//MoveToEx(hdcMem, 21, 360-81, NULL);
	//LineTo(hdcMem, 384, 360-81);
	
	//#############Backup
	//RoundRect(hdcMem, 20, 419, 385, 479,20,20);

	//#####################################################
	


	//#############trigger + crit PSV
	//RoundRect(hdcMem, 415, 20, 780, 121,20,20);
	//MoveToEx(hdcMem, 416, 70, NULL);
	//LineTo(hdcMem, 779, 70);

	//#############delay Pmin + VLimit beep
	//RoundRect(hdcMem, 415, 166, 780, 225,20,20);
	/*RoundRect(hdcMem, 415, 124, 780, 225,20,20);
	MoveToEx(hdcMem, 416, 174, NULL);
	LineTo(hdcMem, 779, 174);*/

	//#############alarm delay
	//RoundRect(hdcMem, 415, 229, 780, 288,20,20);

	//#############absolute PPSV
	//RoundRect(hdcMem, 415, 293, 780, 353,20,20);
	

	//#############hospital
	//RoundRect(hdcMem, 415, 350, 780, 410,20,20);
	//RoundRect(hdcMem, 415, 357, 780, 417,20,20);

	SelectObject(hdcMem,g_hf8AcuBold);

	rc.left = 425;  
	rc.top = 340;  
	rc.right  = 760;  
	rc.bottom = 400;
	//DrawText(hdcMem,_T("Krankenhauseinstellungen"),-1,&rc,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	DrawText(hdcMem,getModel()->GetLanguageString(IDS_BTN_HOSPITALSET),-1,&rc,DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	if(CTlsFile::Exists(_T("\\FFSDISK\\hospital.dat"))==false)
	{
		//rc.left = 425;  
		//rc.top = 357;  
		//rc.right  = 760;  
		//rc.bottom = 417;
		//DrawText(hdcMem,_T("not available"),-1,&rc,DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
		DrawText(hdcMem,getModel()->GetLanguageString(IDS_TXT_NOTAVAILABLE),-1,&rc,DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
	}

	//########factory
	//RoundRect(hdcMem, 415, 420, 780, 480,20,20);
	//RoundRect(hdcMem, 415, 421, 780, 481,20,20);

	//SelectObject(hdcMem,g_hf8AcuBold);
	
	rc.top = 404;  
	rc.bottom = 464;
	DrawText(hdcMem,getModel()->GetLanguageString(IDS_BTN_DEFAULTS),-1,&rc,DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	if(getModel()->getCONFIG()->IsEnableFactoryActive()==false)
	{
		DrawText(hdcMem,getModel()->GetLanguageString(IDS_TXT_DISABLED),-1,&rc,DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
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




/***************************************************************************/
//     
//**************************************************************************/
void CSubViewVentilation::OnBnClickedDefaults()
{
	m_iCurPara=IDC_BTN_SETUP_DEFAULTS;


	CDlgMessageBox box(this,getModel()->GetLanguageString(IDS_MSG_CAPTION_FACTORY),getModel()->GetLanguageString(IDS_MSG_FACTORY),MB_YESNO,IDB_MSG_CONFIG);
	m_pDlg = &box;
	int iRes=box.DoModal();
	m_pDlg = NULL;

	if(iRes==IDCLOSE)
	{
		return;
	}

	if(GetParent())
		GetParent()->PostMessage(WM_KILL_SETUPTIMER);
	if(iRes==IDYES)
	{
		theApp.getLog()->WriteLine(L"***load default settings***");

		getModel()->getALARMHANDLER()->setAutoSilent(false,false,false);
		getModel()->getI2C()->SetFRAMDefaultFactoryValues();
		Sleep(200);
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_RELOAD_CONFIG);
	}
	if(GetParent())
		GetParent()->PostMessage(WM_SET_SETUPTIMER);

	this->SetFocus();

	SetOneButtonFocused(IDC_BTN_SETUP_DEFAULTS);

	UpdateWindow();
	Draw();
}

/***************************************************************************/
//     
//**************************************************************************/
void CSubViewVentilation::OnBnClickedHospital()
{
	m_iCurPara=IDC_BTN_SETUP_HOSPITAL;
	//CDlgMessageBox box(this,_T("Hospital Settings"),_T("This will set the parameter values to the hospital settings. Continue?"),MB_YESNO,IDB_MSG_CONFIG);
	CDlgMessageBox box(this,getModel()->GetLanguageString(IDS_BTN_HOSPITALSET),getModel()->GetLanguageString(IDS_MSG_HOSPITAL),MB_YESNO,IDB_MSG_CONFIG);
	m_pDlg = &box;
	int iRes=box.DoModal();
	m_pDlg = NULL;

	if(iRes==IDCLOSE)
	{
		return;
	}

	if(GetParent())
		GetParent()->PostMessage(WM_KILL_SETUPTIMER);
	if(iRes==IDYES)
	{
		theApp.getLog()->WriteLine(L"***load hospital settings***");
		getModel()->getALARMHANDLER()->setAutoSilent(false,false,false);

		
		getModel()->LoadHospitalSettings();
		if(getModel()->getLoadHospitalSettingsFailed()==true)
		{
			getModel()->setLoadHospitalSettingsFailed(false);
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_RELOAD_CONFIG_ERROR);
		}
		else
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_RELOAD_CONFIG);
		}
	}
	if(GetParent())
		GetParent()->PostMessage(WM_SET_SETUPTIMER);

	this->SetFocus();

	SetOneButtonFocused(IDC_BTN_SETUP_HOSPITAL);

	Draw();
	UpdateWindow();
	Invalidate(TRUE);
}

// **************************************************************************
// 
// **************************************************************************
LRESULT CSubViewVentilation::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
{
	//int iID = 0;
	switch(message)
	{
	case WM_OPEN_SETTING:
		{
			OpenSettingsView();
			return 1;
		}
		break;
	case WM_MENUBUTTONDOWN:
		{
			SetOneButtonDepressed(wParam);
			return 1;
		}
		break;
	case WM_BUTTON_VKUP:
		{
			m_iCurPara=wParam;
			return 1;
			
		}
		break;
	case WM_BUTTON_VKDOWN:
		{
			m_iCurPara=wParam;
			return 1;
		}
		break;
	}
	return CWnd::WindowProc(message, wParam, lParam);
}


// **************************************************************************
// 
// **************************************************************************
void CSubViewVentilation::OnBnClickedModeIT()
{
	SetButtonClicked(IDC_BTN_SETUP_FUNCITIME);
}
void CSubViewVentilation::OnBnClickedLeakComp()
{
	SetButtonClicked(IDC_BTN_SETUP_LEAKCOMP);
}
void CSubViewVentilation::OnBnClickedManBreathTime()
{
	SetButtonClicked(IDC_BTN_SETUP_MANBREATHTIME);
}
void CSubViewVentilation::OnBnClickedPatAlarmDelay()
{
	SetButtonClicked(IDC_BTN_SETUP_PATALARMDELAY);
}
void CSubViewVentilation::OnBnClickedVolumeTrigger()
{
	SetButtonClicked(IDC_BTN_SETUP_VOLUMETRIG);
}
void CSubViewVentilation::OnBnClickedPPSVasDeltaPEEPValue()
{
	SetButtonClicked(IDC_BTN_SETUP_PPSVABSOLUTE);
}
void CSubViewVentilation::OnBnClickedBTBforVT()
{
	SetButtonClicked(IDC_BTN_SETUP_BTBVT);
}
void CSubViewVentilation::OnBnClickedTubeSet()
{
	SetButtonClicked(IDC_BTN_SETUP_TUBESET);
}
void CSubViewVentilation::OnBnClickedPressureUnit()
{
	SetButtonClicked(IDC_BTN_SETUP_PRESSUREUNIT);
}
void CSubViewVentilation::OnBnClickedEFlow()
{
	SetButtonClicked(IDC_BTN_SETUP_EFLOW);
}
void CSubViewVentilation::OnBnClickedAutoOxyCal()
{
	SetButtonClicked(IDC_BTN_SETUP_AUTOOXYCAL);
}
void CSubViewVentilation::OnBnClickedHFOManBreath()
{
	SetButtonClicked(IDC_BTN_SETUP_HFOMANBREATH);
}


// **************************************************************************
// 
// **************************************************************************
void CSubViewVentilation::SetButtonClicked(int btnID)
{
	if(m_iCurPara==btnID)
	{
		eBtnState state = GetCurrentBtnState();
		

		if(state==BS_DOWN)
		{
			PostMessage(WM_OPEN_SETTING);
		}
	}
	else
	{
		SetOneButtonDepressed(btnID);
		PostMessage(WM_OPEN_SETTING);
	}
}

// **************************************************************************
// 
// **************************************************************************
void CSubViewVentilation::SetOneButtonDepressed(int btnID)
{
	POSITION pos;

	try
	{
		for( pos = m_plBtn.GetHeadPosition(); pos != NULL; )
		{
			CSelectSetupBtn* pBtn = m_plBtn.GetNext( pos );

			//int iT = pBtn->GetBtnId();

			if(pBtn->GetBtnId() != btnID)
			{
				//if(pBtn->GetState()==CPresetMenuBtn::DOWN)
				{
					pBtn->SetState(BS_UP);
				}
			}
			else
			{
				//pBtn->DrawDirectDown();
				pBtn->SetState(BS_DOWN);
				m_iCurPara=btnID;
				getModel()->getCONFIG()->SetLastSelectedSVSettingBtns(m_iCurPara);
			}
		}
	}
	catch (...)
	{
		theApp.ReportException(_T("CSubViewVentilation::SetOneButtonDepressed"));
	}

	if(GetParent())
		GetParent()->PostMessage(WM_SET_SETUPTIMER);
}

// **************************************************************************
// 
// **************************************************************************
void CSubViewVentilation::SetOneButtonFocused(int btnID)
{
	POSITION pos;

	try
	{
		for( pos = m_plBtn.GetHeadPosition(); pos != NULL; )
		{
			CSelectSetupBtn* pBtn = m_plBtn.GetNext( pos );

			//int iT = pBtn->GetBtnId();

			if(pBtn->GetBtnId() != btnID)
			{
				//if(pBtn->GetState()==CPresetMenuBtn::DOWN)
				{
					pBtn->SetState(BS_UP);
				}
			}
			else
			{
				pBtn->SetState(BS_FOCUSED);
				m_iCurPara=btnID;
				getModel()->getCONFIG()->SetLastSelectedSVSettingBtns(m_iCurPara);
			}
		}
	}
	catch (...)
	{
		theApp.ReportException(_T("CSubViewVentilation::SetOneButtonFocused"));
	}

	if(getModel()->isSafeTickCountDelayExpired(m_dwLastSetupTimer, 1000))
	{
		m_dwLastSetupTimer=GetTickCount();
		if(GetParent())
			GetParent()->PostMessage(WM_SET_SETUPTIMER);
	}
}

// **************************************************************************
// 
// **************************************************************************
void CSubViewVentilation::OpenSettingsView()
{
	if(GetParent())
		GetParent()->PostMessage(WM_SET_SETUPTIMER);

	if(m_iCurPara==IDC_BTN_SETUP_HOSPITAL)
		OnBnClickedHospital();
	else if(m_iCurPara==IDC_BTN_SETUP_DEFAULTS)
		OnBnClickedDefaults();
	else
	{
		if(GetParent())
			GetParent()->PostMessage(WM_OPEN_SETTING,m_iCurPara);
	}


}


// **************************************************************************
// 
// **************************************************************************
void CSubViewVentilation::SetNextButtonFocused()
{
	POSITION pos;

	pos = m_plBtn.GetHeadPosition();

	do {
		CSelectSetupBtn* pBtn  = m_plBtn.GetNext(pos);
		
		//int iT = pBtn->GetBtnId();

		

		if(m_iCurPara == pBtn->GetBtnId())
		{
			pBtn->SetState(BS_UP);

			if(pos == NULL)
			{
				pos = m_plBtn.GetHeadPosition();
				pBtn = m_plBtn.GetNext( pos );

				pBtn->SetState(BS_FOCUSED);
				m_iCurPara=pBtn->GetBtnId();
				getModel()->getCONFIG()->SetLastSelectedSVSettingBtns(m_iCurPara);
				break;
			}
			else
			{
				pBtn = m_plBtn.GetNext( pos );
				pBtn->SetState(BS_FOCUSED);
				m_iCurPara=pBtn->GetBtnId();
				getModel()->getCONFIG()->SetLastSelectedSVSettingBtns(m_iCurPara);
				break;
			}
		}
		

	} while (pos != NULL);

	if(getModel()->isSafeTickCountDelayExpired(m_dwLastSetupTimer, 1000))
	{
		m_dwLastSetupTimer=GetTickCount();
		if(GetParent())
			GetParent()->PostMessage(WM_SET_SETUPTIMER);
	}
}


// **************************************************************************
// 
// **************************************************************************
void CSubViewVentilation::SetPrevButtonFocused()
{
	POSITION pos;

	pos = m_plBtn.GetTailPosition();

	do {
		CSelectSetupBtn* pBtn  = m_plBtn.GetPrev(pos);

		//int iT = pBtn->GetBtnId();

		pBtn->SetState(BS_UP);

		if(m_iCurPara == pBtn->GetBtnId())
		{
			if(pos == NULL)
			{
				pos = m_plBtn.GetTailPosition();
				pBtn = m_plBtn.GetPrev( pos );

				pBtn->SetState(BS_FOCUSED);
				m_iCurPara=pBtn->GetBtnId();
				getModel()->getCONFIG()->SetLastSelectedSVSettingBtns(m_iCurPara);

				break;
			}
			else
			{
				pBtn = m_plBtn.GetPrev( pos );
				pBtn->SetState(BS_FOCUSED);
				m_iCurPara=pBtn->GetBtnId();
				getModel()->getCONFIG()->SetLastSelectedSVSettingBtns(m_iCurPara);

				break;
			}
		}


	} while (pos != NULL);

	if(getModel()->isSafeTickCountDelayExpired(m_dwLastSetupTimer, 1000))
	{
		m_dwLastSetupTimer=GetTickCount();
		if(GetParent())
			GetParent()->PostMessage(WM_SET_SETUPTIMER);
	}
}


// **************************************************************************
// 
// **************************************************************************
eBtnState CSubViewVentilation::GetCurrentBtnState()
{
	POSITION pos;

	pos = m_plBtn.GetHeadPosition();

	do {
		CSelectSetupBtn* pBtn  = m_plBtn.GetNext(pos);

		//int iT = pBtn->GetBtnId();

		if(m_iCurPara == pBtn->GetBtnId())
		{
			return pBtn->GetState();
		}


	} while (pos != NULL);

	return BS_NONE;
}

// **************************************************************************
// 
// **************************************************************************
BOOL CSubViewVentilation::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message)
	{
	case WM_KEYDOWN:
		{
			if(pMsg->wParam==VK_SPACE)
			{
				if(GetCurrentBtnState()==BS_FOCUSED)
				{
					SetOneButtonDepressed(m_iCurPara);
				}
				else if(GetCurrentBtnState()==BS_DOWN)
				{
					SetOneButtonFocused(m_iCurPara);
				}
				
			}
			else if(pMsg->wParam==VK_DOWN)
			{
				if(GetCurrentBtnState()==BS_FOCUSED)
					SetPrevButtonFocused();
				/*else if(GetCurrentBtnState()==BS_DOWN)
					SetPrevButtonValue();*/
				

				//return 1;
			}
			else if(pMsg->wParam==VK_UP)
			{
				if(GetCurrentBtnState()==BS_FOCUSED)
					SetNextButtonFocused();
				/*else if(GetCurrentBtnState()==BS_DOWN)
					SetNextButtonValue();*/
				

				
				//return 1;
			}
			else
			{
				//handle focus change manual, draw next focused button and return true
				//return 1;
			}
			break;
		}
		break;
	case WM_KEYUP:
		{
			if(pMsg->wParam==VK_SPACE)
			{
				if(GetCurrentBtnState()==BS_DOWN)
				{
					PostMessage(WM_OPEN_SETTING);
				}
				//return 1;
			}
			//else if(pMsg->wParam==VK_DOWN)
			//{
			//	if(GetCurrentBtnState()==BS_FOCUSED)
			//		SetPrevButtonFocused();
			//	/*else if(GetCurrentBtnState()==BS_DOWN)
			//		SetPrevButtonValue();*/
			//	

			//	//return 1;
			//}
			//else if(pMsg->wParam==VK_UP)
			//{
			//	if(GetCurrentBtnState()==BS_FOCUSED)
			//		SetNextButtonFocused();
			//	/*else if(GetCurrentBtnState()==BS_DOWN)
			//		SetNextButtonValue();*/
			//	

			//	
			//	//return 1;
			//}
			//else
			//{
			//	//handle focus change manual, draw next focused button and return true
			//	//return 1;
			//}
			break;
		}
		break;
	default:
		break;
	}
	return CWnd::PreTranslateMessage(pMsg);
}