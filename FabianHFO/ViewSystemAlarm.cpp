// ViewSystemAlarm.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "ViewSystemAlarm.h"
#include "LangAdmin.h"
#include "globDefs.h"

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
 * Initializes a new instance of the CViewSystemAlarm class
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	iViewID	Identifier for the view.
 **************************************************************************************************/

CViewSystemAlarm::CViewSystemAlarm(int iViewID):
CMVView(iViewID)
{
	//getModel()->AttachObserver(this);

	/*m_bLogActive=false;
	m_pWndAlarmLog=NULL;*/

	m_pcMenuNum=NULL;
	m_pcMenuSens=NULL;

	m_pcMenu_Up=NULL;
	m_pcMenu_Dw=NULL;
	m_pcMenu_Dis=NULL;

	m_pcSysAlarm=NULL;
	m_pcMixerAlarm=NULL;
	m_pcMixerSignal=NULL;
	m_pcAClockGrey=NULL;
	m_pcAClockRed=NULL;
	m_pcNumeric_Up=NULL;
	m_pcNumeric_Dw=NULL;
	m_pcAVCal=NULL;
	m_pcAVCalAlarm=NULL;
	m_pcAVCalSignal=NULL;

	m_bDrawO2SensorBtn=false;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CViewSystemAlarm class
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

CViewSystemAlarm::~CViewSystemAlarm(void)
{


	delete m_pcSysAlarm;
	m_pcSysAlarm=NULL;

	delete m_pcMixerAlarm;
	m_pcMixerAlarm=NULL;

	delete m_pcAVCalAlarm;
	m_pcAVCalAlarm=NULL;

	delete m_pcAVCal;
	m_pcAVCal=NULL;

	delete m_pcAVCalSignal;
	m_pcAVCalSignal=NULL;

	delete m_pcMixerSignal;
	m_pcMixerSignal=NULL;

	delete m_pcAClockGrey;
	m_pcAClockGrey=NULL;

	delete m_pcAClockRed;
	m_pcAClockRed=NULL;



	delete m_pcMenuSens;
	m_pcMenuSens=NULL;
	delete m_pcMenuNum;
	m_pcMenuNum=NULL;
	delete m_pcMenu_Up;
	m_pcMenu_Up=NULL;
	delete m_pcMenu_Dw;
	m_pcMenu_Dw=NULL;
	delete m_pcMenu_Dis;
	m_pcMenu_Dis=NULL;
	delete m_pcNumeric_Up;
	m_pcNumeric_Up=NULL;
	delete m_pcNumeric_Dw;
	m_pcNumeric_Dw=NULL;
}



BEGIN_MESSAGE_MAP(CViewSystemAlarm, CMVView)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * Creates the view
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewSystemAlarm::CreateView()
{
	//RECT rcLd={0,0,800,580};
	RECT rcLd={0,41,800,600};

	if (!Create(AfxGetApp()->GetMainWnd(),rcLd,IDC_VIEW_SYSALARM))
	{
		return false;
	}
	Initialize();

	DrawAlarms();

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

BOOL CViewSystemAlarm::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext*) 
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

		m_pcSysAlarm	= new CBmp(theApp.m_hInstance,m_hDC,IDB_PIC_SYSALARM);
		m_pcMixerAlarm	= new CBmp(theApp.m_hInstance,m_hDC,IDB_PIC_MIXERALARM);
		m_pcMixerSignal	= new CBmp(theApp.m_hInstance,m_hDC,IDB_PIC_MIXERSIGNAL);

		m_pcAVCal		= new CBmp(theApp.m_hInstance,m_hDC,IDB_PIC_AVCAL);
		m_pcAVCalAlarm	= new CBmp(theApp.m_hInstance,m_hDC,IDB_PIC_AVCALALARM);
		m_pcAVCalSignal	= new CBmp(theApp.m_hInstance,m_hDC,IDB_PIC_AVCALSIGNAL);
		//m_pcUseError	= new CBmp(theApp.m_hInstance,m_hDC,IDB_SYSAL_USE_ERROR);
		
		m_pcAClockGrey	= new CBmp(theApp.m_hInstance,m_hDC,IDB_ACLOCK_SYS_GREY);
		m_pcAClockRed	= new CBmp(theApp.m_hInstance,m_hDC,IDB_ACLOCK_SYS_RED);

		SelectObject(m_hDC,hpenprev);
		SelectObject(m_hDC,hbrprev);

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
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewSystemAlarm::Initialize()
{
	bool result=false;

	CClientDC dc(this);


	BTN btn;

	m_pcMenu_Up		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_4MENU_UP);
	m_pcMenu_Dw		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_4MENU_DW);
	m_pcMenu_Dis		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_4MENU_DIS);

	m_pcNumeric_Up		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_NUM_BACK_UP);
	m_pcNumeric_Dw		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_NUM_BACK_DW);
	//m_pcNumeric_Fc		= new CBmp(theApp.m_hInstance,hdc,	IDB_BTN_NUMMENUBCK_FC);

	//Menu Buttons
	btn.wID					= IDC_BTN_SYSALARM_O2;	
	btn.poPosition.x		= 480;
	btn.poPosition.y		= 505;//553;
	btn.pcBmpUp				= m_pcMenu_Up;
	btn.pcBmpDown			= m_pcMenu_Dw;
	btn.pcBmpFocus			= m_pcMenu_Up;
	btn.pcBmpDisabled		= m_pcMenu_Dis;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcMenuSens=new CPushBtn(btn,COLOR_TXTBTNUP,true);
	m_pcMenuSens->Create(this,g_hf13AcuBold,0);
	//m_pcMenuSens->SetText(_T("Turn off"),_T("O2 Sensor"));
	m_pcMenuSens->SetText(getModel()->GetLanguageString(IDS_MENU_CAL5),getModel()->GetLanguageString(IDS_MENU_CAL6));
	
	m_pcMenuSens->ShowWindow(SW_HIDE);


	btn.wID					= IDC_BTN_SYSALARM_BACK;	
	btn.poPosition.x		= 630;
	btn.poPosition.y		= 505;//553;
	btn.pcBmpUp				= m_pcNumeric_Up;
	btn.pcBmpDown			= m_pcNumeric_Dw;
	btn.pcBmpFocus			= m_pcNumeric_Up;
	btn.pcBmpDisabled		= m_pcNumeric_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcMenuNum=new CPushBtn(btn,COLOR_TXTBTNUP);
	m_pcMenuNum->Create(this,g_hf14AcuMed,0);
	m_pcMenuNum->SetText(_T(""));
	m_pcMenuNum->ShowWindow(SW_SHOW);

	

	return result;
}

/**********************************************************************************************//**
 * Opens this instance
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewSystemAlarm::Open()
{
}

/**********************************************************************************************//**
 * Closes this instance
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewSystemAlarm::Close()
{
}

/**********************************************************************************************//**
 * Shows this instance
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewSystemAlarm::Show()
{
	this->ShowWindow(SW_SHOW);

}

/**********************************************************************************************//**
 * Hides this instance
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewSystemAlarm::Hide()
{
	this->ShowWindow(SW_HIDE);
}

/**********************************************************************************************//**
 * Paints this window
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewSystemAlarm::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,m_hDC,0,0,SRCCOPY);
}

/**********************************************************************************************//**
 * Executes the destroy action
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewSystemAlarm::OnDestroy()
{
	KillTimer(SYSALARMTIMER);

	getModel()->getI2C()->DeleteI2CErrorCode();
	getModel()->getDATAHANDLER()->deleteAllCOMError();//newVG
	getModel()->getDATAHANDLER()->deleteSPIErrorCode();

	CMVView::OnDestroy();
}

/**********************************************************************************************//**
 * Refresh alarm states
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewSystemAlarm::RefreshAlarmStates()
{
	DrawAlarms();
}

/**********************************************************************************************//**
 * Draw alarms
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewSystemAlarm::DrawAlarms()
{
	RECT rc={0,0,m_lX,m_lY};

	CClientDC dc(this);
	HDC hdcMem=CreateCompatibleDC(m_hDC);
	HBITMAP hBmpMem=CreateCompatibleBitmap(m_hDC,m_lX,m_lY);
	HBITMAP hBmpMemPrev=(HBITMAP)SelectObject(hdcMem,hBmpMem);

	CBrush cbrBack(BACKGND);
	CBrush cbrAlarm(RGB(255,0,0));
	CBrush cbrWhite(RGB(255,255,255));
	HBRUSH hbrprev=(HBRUSH)SelectObject(hdcMem,cbrBack);
	HPEN hpenprev=(HPEN)SelectObject(hdcMem, (HPEN)GetStockObject(NULL_PEN));

	//HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf17AcuBold);
	HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf8AcuBold);
	int nBkMode=SetBkMode(hdcMem,TRANSPARENT);
	int nPrevTxtColor=SetTextColor(hdcMem,RGB(0,0,0));

	Rectangle(hdcMem, 0, 0, m_lX, m_lY);

	//CDC* pDCMem=CDC::FromHandle(hdcMem);
	//CDC* pDC=CDC::FromHandle(m_hDC);

	DrawPic(hdcMem);

	

	//system alarms
	/*tALARMSTATES alarmState;
	getModel()->getALARMHANDLER()->getAllAlarmStates(&alarmState);*/

	bool bDrawAvCal=true;
	eStateOfAlarm stateMixAv=getModel()->getALARMHANDLER()->ALARM_SysFail_MIXER_AVCAL->getAlarmState();

	if(stateMixAv==AS_ACTIVE)
	{
		int iBLENDERstate=getModel()->getDATAHANDLER()->GetMSTATUS_BlenderPIC();
		if(iBLENDERstate & BIT8)//AV calibration error
		{
			bDrawAvCal=false;
		}
	}
	else if(stateMixAv==AS_SIGNALED)
	{
		int iBLENDERstate=getModel()->getDATAHANDLER()->GetMSTATUS_BlenderPIC();
		if(iBLENDERstate & BIT8)//AV calibration error
		{
			bDrawAvCal=false;
		}
	}

	if(bDrawAvCal)
	{
		m_pcAVCal->Draw(hdcMem,440,173);
	}

	rc.top = 196;//398;
	rc.left = 420; 
	rc.right  = 496;  
	rc.bottom = 550;
	//DrawText(hdcMem,_T("Exh.Valve"),-1,&rc,DT_TOP|DT_SINGLELINE|DT_CENTER);
	DrawText(hdcMem,getModel()->GetLanguageString(IDS_TXT_EXHVALVE),-1,&rc,DT_TOP|DT_SINGLELINE|DT_CENTER);


	DrawCheckpoints(hdcMem);

	rc.left = 30;  
	rc.top = 450;  
	rc.right  = 450;  
	rc.bottom = 550;
	FillRect(hdcMem,&rc,cbrWhite);

	CStringW szStep=_T("");
	CStringW szStep1=_T("");
	CStringW szStep2=_T("");
	CStringW szStep3=_T("");
	CStringW szStep4=_T("");
	CStringW szStep5=_T("");
	CStringW szStep6=_T("");
	CStringW szStep7=_T("");

	//Eingangsdruck Air
	// AlarmState_P_IN_AIR **********************************************************
	eStateOfAlarm stateP_IN_AIR=getModel()->getALARMHANDLER()->ALARM_SysAl_P_IN_AIR->getAlarmState();
	if(stateP_IN_AIR==AS_ACTIVE
		/*||	alarmState.eSoA_P_IN_AIR==AS_SIGNALED*/)
	{
		//szStep1=_T("- Druckluftversorgung prüfen");
		szStep1=getModel()->GetLanguageString(IDS_TXT_CHECK_PINAIR);
	}

	//Eingangsdruck O2
	// AlarmState_P_IN_O2 **********************************************************
	eStateOfAlarm stateP_IN_O2=getModel()->getALARMHANDLER()->ALARM_SysAl_P_IN_O2->getAlarmState();
	if(stateP_IN_O2==AS_ACTIVE
		/*||	alarmState.eSoA_P_IN_O2==AS_SIGNALED*/)
	{
		//szStep2=_T("- O2-Versorgung überprüfen");
		szStep2=getModel()->GetLanguageString(IDS_TXT_CHECK_PINO2);
	}

	//O2-Sensor
	// AlarmState_O2_VALUE_INCORRECT **********************************************************
	/*if(		alarmState.eSoA_O2_VALUE_INCORRECT==AS_ACTIVE
		||	alarmState.eSoA_O2_VALUE_INCORRECT==AS_SIGNALED
		||	alarmState.eSoA_O2SENSOR_DEFECT==AS_ACTIVE
		||	alarmState.eSoA_O2SENSOR_DEFECT==AS_SIGNALED
		||	alarmState.eSoA_O2SENSOR_USED==AS_ACTIVE
		||	alarmState.eSoA_O2SENSOR_USED==AS_SIGNALED)
	{
		szStep3=_T("- Step3");
	}*/

	//Inspirationsschlauch
	//if(		alarmState.eSoA_P_INSPIRATION==AS_ACTIVE
	//	/*||	alarmState.eSoA_P_INSPIRATION==AS_SIGNALED*/)
	//{
	//	//szStep4=_T("- Inspirationsschlauch geknickt/eingeklemmt, Schlauch überprüfen");
	//	szStep4=getModel()->GetLanguageString(IDS_TXT_CHECK_PINSP);
	//}
	

	//Exspirationsschlauch
	//if(		alarmState.eSoA_P_EXSPIRATION==AS_ACTIVE
	//	/*||	alarmState.eSoA_P_EXSPIRATION==AS_SIGNALED*/)
	//{
	//	//szStep5=_T("- Exspirationsschlauch geknickt/eingeklemmt, Schlauch überprüfen");
	//	szStep5=getModel()->GetLanguageString(IDS_TXT_CHECK_PEX);
	//}

	//Eingangsdruck Air
	// AlarmState_P_IN_AIR **********************************************************
	eStateOfAlarm stateTUBE_OCCLUSION=getModel()->getALARMHANDLER()->ALARM_SysAl_TUBE_OCCLUSION->getAlarmState();
	if(stateTUBE_OCCLUSION==AS_ACTIVE
		/*||	alarmState.eSoA_P_PROXIMAL==AS_SIGNALED*/)
	{
		szStep6=getModel()->GetLanguageString(IDS_ALARM_TUBE_OCCLUSION);
	}

	//Mischer defekt
	// AlarmState_MIXER **********************************************************
	eStateOfAlarm stateMIXER_AVCAL=getModel()->getALARMHANDLER()->ALARM_SysFail_MIXER_AVCAL->getAlarmState();
	if(stateMIXER_AVCAL==AS_ACTIVE
		/*||	alarmState.eSoA_MIXER==AS_SIGNALED*/)
	{
		int iBLENDERstate=getModel()->getDATAHANDLER()->GetMSTATUS_BlenderPIC();
		if(iBLENDERstate & BIT8)//AV calibration error
		{
			szStep7.Format(_T("%s: code#%d"),getModel()->GetLanguageString(IDS_TXT_AV_CAL_FAIL),iBLENDERstate);
		}
		else
		{
			szStep7.Format(_T("%s: code#%d"),getModel()->GetLanguageString(IDS_TXT_CHECK_MIXER),iBLENDERstate);
			//szStep7=getModel()->GetLanguageString(IDS_TXT_CHECK_MIXER);
		}
		//szStep1=_T("- Druckluftversorgung prüfen");
		
	}

	szStep=szStep1;
	if(szStep2!=_T(""))
	{
		if(szStep!=_T(""))
			szStep+=_T("\r\n");
		szStep+=szStep2;
	}
	if(szStep3!=_T(""))
	{
		if(szStep!=_T(""))
			szStep+=_T("\r\n");
		szStep+=szStep3;
	}
	if(szStep4!=_T(""))
	{
		if(szStep!=_T(""))
			szStep+=_T("\r\n");
		szStep+=szStep4;
	}
	if(szStep5!=_T(""))
	{
		if(szStep!=_T(""))
			szStep+=_T("\r\n");
		szStep+=szStep5;
	}
	if(szStep6!=_T(""))
	{
		if(szStep!=_T(""))
			szStep+=_T("\r\n");
		szStep+=szStep6;
	}
	if(szStep7!=_T(""))
	{
		if(szStep!=_T(""))
			szStep+=_T("\r\n");
		szStep+=szStep7;
	}

	rc.left = 40;  
	rc.top = 454;  
	rc.right  = 450;  
	rc.bottom = 550;
	DrawText(hdcMem,szStep,-1,&rc,DT_TOP|DT_WORDBREAK|DT_LEFT);
	//pDCMem->DrawText(szStep,&rc,DT_TOP|DT_WORDBREAK|DT_LEFT);
	

	/*rc.left = 40;  
	rc.top = 471;  
	rc.right  = 450;  
	rc.bottom = 550;
	pDCMem->DrawText(szStep2,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

	rc.left = 40;  
	rc.top = 488;  
	rc.right  = 450;  
	rc.bottom = 550;
	pDCMem->DrawText(szStep3,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

	rc.left = 40;  
	rc.top = 505;  
	rc.right  = 450;  
	rc.bottom = 550;
	pDCMem->DrawText(szStep4,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

	rc.left = 40;  
	rc.top = 522;  
	rc.right  = 450;  
	rc.bottom = 550;
	pDCMem->DrawText(szStep5,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);*/
	


	
	/*rc.top = 15;
	rc.left = 0; 
	rc.right = 400;
	
	pDCMem->DrawText(getModel()->GetLanguageString(IDS_TXT_BEDIENFEHLER),&rc,DT_TOP|DT_SINGLELINE|DT_CENTER);

	rc.top = 15;
	rc.left = 400; 
	rc.right = 800;
	pDCMem->DrawText(getModel()->GetLanguageString(IDS_TXT_DEVERROR),&rc,DT_TOP|DT_SINGLELINE|DT_CENTER);*/
	
	SelectObject(hdcMem,g_hf11AcuBold);

	

	

	// AlarmState_DISCONNECTION **********************************************************
	//if(alarmState.eDISCONNECTION==AS_ACTIVE)
	//{
	//	DrawAlarmClockActive(pDCMem, 30, 48);

	//	SetBkMode(hdcMem,OPAQUE);
	//	SetBkColor(hdcMem,RGB(255,0,0));
	//}
	//else if(alarmState.eDISCONNECTION==AS_SIGNALED)
	//{
	//	DrawAlarmClockInactive(pDCMem, 30, 48);

	//	SetBkMode(hdcMem,OPAQUE);
	//	SetBkColor(hdcMem,RGB(209,209,209));
	//}
	//else 
	//{
	//	SetBkMode(hdcMem,TRANSPARENT);
	//	//SetBkColor(hdcMem,RGB(255,0,0));
	//}

	//rc.top = 50;
	////rc.bottom = 95;
	//rc.left = 50; 
	////rc.right = 175;
	//
	////pDCMem->DrawText(_T("Diskonnektion"),&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	//pDCMem->DrawText(getModel()->GetLanguageString(IDS_ALARM_DISCONNECTION),&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

	
	// AlarmState_P_IN_O2 **********************************************************
	if(stateP_IN_O2==AS_ACTIVE)
	{
		m_bDrawO2SensorBtn=true;
		DrawAlarmClockActive(hdcMem, 30, 286);
	}
	else if(stateP_IN_O2==AS_SIGNALED)
	{
		m_bDrawO2SensorBtn=true;
		DrawAlarmClockInactive(hdcMem, 30, 286);
	}

	//rc.top = 50;
	rc.top = 288;
	rc.left = 50; 
	//CString szTest=getModel()->GetLanguageString(IDS_ALARM_PO2IN);
	//pDCMem->DrawText(_T("Eingangsdruck Sauerstoffversorgung"),&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	DrawText(hdcMem,getModel()->GetLanguageString(IDS_ALARM_PO2IN),-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	//DrawText(hdcMem,_T("Eingangsdruck Sauerstoffversorgung"),-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	//pDCMem->DrawText(getModel()->GetLanguageString(IDS_ALARM_PO2IN),&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	


	// AlarmState_P_IN_AIR **********************************************************
	if(stateP_IN_AIR==AS_ACTIVE)
	{
		DrawAlarmClockActive(hdcMem, 30, 308);
	}
	else if(stateP_IN_AIR==AS_SIGNALED)
	{
		DrawAlarmClockInactive(hdcMem, 30, 308);
	}
	

	//rc.top = 94;
	rc.top = 310;
	rc.left = 50; 
	//pDCMem->DrawText(_T("Eingangsdruck Luftversorgung"),&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	DrawText(hdcMem,getModel()->GetLanguageString(IDS_ALARM_PAIRIN),-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	//pDCMem->DrawText(getModel()->GetLanguageString(IDS_ALARM_PAIRIN),&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	

	// AlarmState_P_INSPIRATION **********************************************************
	//if(alarmState.eSoA_P_INSPIRATION==AS_ACTIVE)
	//{
	//	DrawAlarmClockActive(hdcMem, 30, 330);
	//}
	//else if(alarmState.eSoA_P_INSPIRATION==AS_SIGNALED)
	//{
	//	DrawAlarmClockInactive(hdcMem, 30, 330);
	//}

	////rc.top = 116;
	//rc.top = 332;
	//rc.left = 50; 
	//DrawText(hdcMem,getModel()->GetLanguageString(IDS_ALARM_TUBEINSP),-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	////pDCMem->DrawText(getModel()->GetLanguageString(IDS_ALARM_TUBEINSP),&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);


	// AlarmState_P_EXSPIRATION **********************************************************
	//if(alarmState.eSoA_P_EXSPIRATION==AS_ACTIVE)
	//{
	//	DrawAlarmClockActive(hdcMem, 30, 352);
	//}
	//else if(alarmState.eSoA_P_EXSPIRATION==AS_SIGNALED)
	//{
	//	DrawAlarmClockInactive(hdcMem, 30, 352);
	//}

	////rc.top = 138;
	//rc.top = 354;
	//rc.left = 50; 
	//DrawText(hdcMem,getModel()->GetLanguageString(IDS_ALARM_TUBEEXSP),-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	////pDCMem->DrawText(getModel()->GetLanguageString(IDS_ALARM_TUBEEXSP),&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	


	// AlarmState_P_PROXIMAL **********************************************************
	if(stateTUBE_OCCLUSION==AS_ACTIVE)
	{
		DrawAlarmClockActive(hdcMem, 30, 330);
	}
	else if(stateTUBE_OCCLUSION==AS_SIGNALED)
	{
		DrawAlarmClockInactive(hdcMem, 30, 330);
	}

	//rc.top = 138;
	rc.top = 332;//376;
	rc.left = 50; 
	DrawText(hdcMem,getModel()->GetLanguageString(IDS_ALARM_TUBE_OCCLUSION),-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	//pDCMem->DrawText(getModel()->GetLanguageString(IDS_ALARM_PPROXIMAL),&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

	// AlarmState_MIXER **********************************************************
	
	if(stateMIXER_AVCAL==AS_ACTIVE)
	{
		
		int iBLENDERstate=getModel()->getDATAHANDLER()->GetMSTATUS_BlenderPIC();
		if(iBLENDERstate & BIT8)//AV calibration error
		{
			DrawAlarmClockActive(hdcMem, 30, 374);
		}
		else
		{
			DrawAlarmClockActive(hdcMem, 30, 352);
		}
	}
	else if(stateMIXER_AVCAL==AS_SIGNALED)
	{
		
		int iBLENDERstate=getModel()->getDATAHANDLER()->GetMSTATUS_BlenderPIC();
		if(iBLENDERstate & BIT8)//AV calibration error
		{
			DrawAlarmClockInactive(hdcMem, 30, 374);
		}
		else
		{
			DrawAlarmClockInactive(hdcMem, 30, 352);
		}
	}

	//rc.top = 138;
	rc.top = 354;//398;
	rc.left = 50; 
	DrawText(hdcMem,getModel()->GetLanguageString(IDS_ALARM_MIXER),-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

	rc.top = 376;//398;
	rc.left = 50; 
	DrawText(hdcMem,getModel()->GetLanguageString(IDS_TXT_AV_CAL_FAIL),-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);


	// AlarmState_RELAIS_DEFECT **********************************************************
	//if(alarmState.eRELAIS_DEFECT==AS_ACTIVE)
	//{
	//	DrawAlarmClockActive(pDCMem, 430, 92);

	//	SetBkMode(hdcMem,OPAQUE);
	//	SetBkColor(hdcMem,RGB(255,0,0));
	//}
	//else if(alarmState.eRELAIS_DEFECT==AS_SIGNALED)
	//{
	//	DrawAlarmClockInactive(pDCMem, 430, 92);

	//	SetBkMode(hdcMem,OPAQUE);
	//	SetBkColor(hdcMem,RGB(209,209,209));
	//}
	//else 
	//{
	//	SetBkMode(hdcMem,TRANSPARENT);
	//	//SetBkColor(hdcMem,RGB(255,0,0));
	//}

	//rc.top = 94;
	////rc.bottom = 95;
	//rc.left = 450; 
	////rc.right = 175;
	//pDCMem->DrawText(getModel()->GetLanguageString(IDS_ALARM_RELAIS_DEFECT),&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

	// AlarmState_P_PROXIMAL **********************************************************
	//if(alarmState.eP_PROXIMAL==AS_ACTIVE)
	//{
	//	DrawAlarmClockActive(pDCMem, 430, 114);

	//	SetBkMode(hdcMem,OPAQUE);
	//	SetBkColor(hdcMem,RGB(255,0,0));
	//}
	//else if(alarmState.eP_PROXIMAL==AS_SIGNALED)
	//{
	//	DrawAlarmClockInactive(pDCMem, 430, 114);

	//	SetBkMode(hdcMem,OPAQUE);
	//	SetBkColor(hdcMem,RGB(209,209,209));
	//}
	//else 
	//{
	//	SetBkMode(hdcMem,TRANSPARENT);
	//	//SetBkColor(hdcMem,RGB(255,0,0));
	//}

	//rc.top = 116;
	////rc.bottom = 95;
	//rc.left = 450; 
	////rc.right = 175;
	//pDCMem->DrawText(getModel()->GetLanguageString(IDS_ALARM_PPROXIMAL),&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

	// AlarmState_P_IN_MIXER **********************************************************
	//if(alarmState.ePIN_MIXER==AS_ACTIVE)
	//{
	//	DrawAlarmClockActive(pDCMem, 430, 136);

	//	SetBkMode(hdcMem,OPAQUE);
	//	SetBkColor(hdcMem,RGB(255,0,0));
	//}
	//else if(alarmState.ePIN_MIXER==AS_SIGNALED)
	//{
	//	DrawAlarmClockInactive(pDCMem, 430, 136);

	//	SetBkMode(hdcMem,OPAQUE);
	//	SetBkColor(hdcMem,RGB(209,209,209));
	//}
	//else 
	//{
	//	SetBkMode(hdcMem,TRANSPARENT);
	//	//SetBkColor(hdcMem,RGB(255,0,0));
	//}

	//rc.top = 138;
	////rc.bottom = 95;
	//rc.left = 450; 
	////rc.right = 175;
	//pDCMem->DrawText(getModel()->GetLanguageString(IDS_ALARM_PIN_MIXER),&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);


	

	//rc.top = 160;
	////rc.bottom = 95;
	//rc.left = 450; 
	////rc.right = 175;
	//pDCMem->DrawText(getModel()->GetLanguageString(IDS_ALARM_OVERPRESSURE),&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);


	// AlarmState_VOLTAGE **********************************************************
	//if(alarmState.eVOLTAGE==AS_ACTIVE)
	//{
	//	DrawAlarmClockActive(pDCMem, 430, 180);

	//	SetBkMode(hdcMem,OPAQUE);
	//	SetBkColor(hdcMem,RGB(255,0,0));
	//}
	//else if(alarmState.eVOLTAGE==AS_SIGNALED)
	//{
	//	DrawAlarmClockInactive(pDCMem, 430, 180);

	//	SetBkMode(hdcMem,OPAQUE);
	//	SetBkColor(hdcMem,RGB(209,209,209));
	//}
	//else 
	//{
	//	SetBkMode(hdcMem,TRANSPARENT);
	//	//SetBkColor(hdcMem,RGB(255,0,0));
	//}

	//rc.top = 182;
	////rc.bottom = 95;
	//rc.left = 450; 
	////rc.right = 175;
	//pDCMem->DrawText(getModel()->GetLanguageString(IDS_ALARM_VOLTAGE),&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);



	
	BitBlt(m_hDC, 0, 0, m_lX, m_lY,hdcMem , 0, 0, SRCCOPY);
	BitBlt(dc.m_hDC, 0, 0, m_lX, m_lY, hdcMem, 0, 0, SRCCOPY);




	SelectObject(hdcMem, hpenprev);	
	SelectObject(hdcMem,hbrprev);

	SetBkMode(hdcMem,nBkMode);
	SetTextColor(hdcMem,nPrevTxtColor);

	SelectObject(hdcMem,hBmpMemPrev);
	DeleteObject(hBmpMem);
	SelectObject(hdcMem,hPrevFont);
	DeleteDC(hdcMem);


	if(m_bDrawO2SensorBtn)
	{
		if(getModel()->getDATAHANDLER()->GetO2SensorState()==OXYSENS_OFF)
		{
			m_pcMenuSens->EnableWindow(FALSE);
			m_pcMenuSens->RefreshText(getModel()->GetLanguageString(IDS_MENU_CAL5),getModel()->GetLanguageString(IDS_TXT_NOTACTIVE));
		}
		m_pcMenuSens->ShowWindow(SW_SHOW);
	}
	else
		m_pcMenuSens->ShowWindow(SW_HIDE);
	
}

/**********************************************************************************************//**
 * Draw picture
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	hdc	The hdc.
 **************************************************************************************************/

void CViewSystemAlarm::DrawPic(HDC hdc)
{
	//HDC hdc = *pDC;

	if(m_pcSysAlarm)
		m_pcSysAlarm->Draw(hdc,150,20);

	

	/*m_pcSysAlarm->Draw(hdc,5,200);
	m_pcUseError->Draw(hdc,0,10);
	m_pcDeviceError->Draw(hdc,402,10);*/

	
}

/**********************************************************************************************//**
 * Draw alarm clock active
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	hdc	The hdc.
 * \param	nX 	The n x coordinate.
 * \param	nY 	The n y coordinate.
 **************************************************************************************************/

void CViewSystemAlarm::DrawAlarmClockActive(HDC hdc, int nX, int nY)
{
	//HDC hdc = *pDC;

	if(m_pcAClockRed)
		m_pcAClockRed->Draw(hdc,nX,nY);
	
}

/**********************************************************************************************//**
 * Draw alarm clock inactive
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	hdc	The hdc.
 * \param	nX 	The n x coordinate.
 * \param	nY 	The n y coordinate.
 **************************************************************************************************/

void CViewSystemAlarm::DrawAlarmClockInactive(HDC hdc, int nX, int nY)
{
	//HDC hdc = *pDC;

	if(m_pcAClockGrey)
		m_pcAClockGrey->Draw(hdc,nX,nY);

}

/**********************************************************************************************//**
 * Draw checkpoints
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	hdc	The hdc.
 **************************************************************************************************/

void CViewSystemAlarm::DrawCheckpoints(HDC hdc)
{
	//HDC hdc = *pDC;

	int nBkMode=SetBkMode(hdc,TRANSPARENT);
	HFONT hPrevFont=(HFONT)SelectObject(hdc,g_hf8AcuNorm);
	int nPrevTxtColor=SetTextColor(hdc,RGB(0,0,0));

	CBrush cbrGreen(RGB(0,255,0));
	CBrush cbrRed(RGB(255,0,0));
	CBrush cbrGrey(RGB(210,210,210));
	CPen penRed,penGrey;
	penRed.CreatePen(PS_SOLID,5,RGB(255,0,0));
	penGrey.CreatePen(PS_SOLID,5,RGB(210,210,210));
	HBRUSH hbrprev=(HBRUSH)SelectObject(hdc,cbrGreen);
	HPEN hpenprev=(HPEN)SelectObject(hdc, (HPEN)GetStockObject(BLACK_PEN));	

	RECT rc;	
	//TCHAR psz[MAX_PATH];

	//system alarms
	/*tALARMSTATE_ALL alarmState;
	getModel()->getDATAHANDLER()->ALARMS->getAllAlarmStates(&alarmState);*/
	

	//Eingangsdruck Air
	// AlarmState_P_IN_AIR **********************************************************
	eStateOfAlarm stateP_IN_AIR=getModel()->getALARMHANDLER()->ALARM_SysAl_P_IN_AIR->getAlarmState();
	if(stateP_IN_AIR==AS_ACTIVE)
	{
		SelectObject(hdc,cbrRed);
	}
	else if(stateP_IN_AIR==AS_SIGNALED)
	{
		SelectObject(hdc,cbrGrey);
	}
	else 
	{
		SelectObject(hdc,cbrGreen);
	}
	Rectangle(hdc,250,28,280,58);
	MoveToEx(hdc, 250, 57, NULL);
	LineTo(hdc, 280, 27);
	MoveToEx(hdc, 265, 58, NULL);
	LineTo(hdc, 265, 70);

	rc.top = 28;
	rc.bottom = 58;
	rc.left = 250;
	rc.right = 277;
	CStringW cs = getModel()->GetLanguageString(IDS_PARA_P);
	DrawText(hdc,cs,-1,&rc,DT_RIGHT|DT_BOTTOM|DT_SINGLELINE);

	//Eingangsdruck O2
	// AlarmState_P_IN_O2 **********************************************************
	eStateOfAlarm stateP_IN_O2=getModel()->getALARMHANDLER()->ALARM_SysAl_P_IN_O2->getAlarmState();
	if(stateP_IN_O2==AS_ACTIVE)
	{
		SelectObject(hdc,cbrRed);
	}
	else if(stateP_IN_O2==AS_SIGNALED)
	{
		SelectObject(hdc,cbrGrey);
	}
	else 
	{
		SelectObject(hdc,cbrGreen);
	}
	Rectangle(hdc,250,126,280,156);
	MoveToEx(hdc, 250, 155, NULL);
	LineTo(hdc, 280, 125);
	MoveToEx(hdc, 265, 126, NULL);
	LineTo(hdc, 265, 112);

	rc.top = 126;
	rc.bottom = 156;
	rc.left = 250;
	rc.right = 277;
	//cs = getModel()->GetLanguageString(IDS_PARA_P);
	DrawText(hdc,cs,-1,&rc,DT_RIGHT|DT_BOTTOM|DT_SINGLELINE);

	

	//O2-Sensor
	// AlarmState_O2_VALUE_INCORRECT **********************************************************
	/*if(		alarmState->eSoA_O2_VALUE_INCORRECT==AS_ACTIVE
		||	alarmState->eSoA_O2_VALUE_INCORRECT==AS_SIGNALED
		||	alarmState->eSoA_O2SENSOR_DEFECT==AS_ACTIVE
		||	alarmState->eSoA_O2SENSOR_DEFECT==AS_SIGNALED
		||	alarmState->eSoA_O2SENSOR_USED==AS_ACTIVE
		||	alarmState->eSoA_O2SENSOR_USED==AS_SIGNALED)
	{
		SelectObject(hdc,cbrRed);
	}
	else 
	{
		SelectObject(hdc,cbrGreen);
	}
	Rectangle(hdc,415,46,445,76);
	MoveToEx(hdc, 415, 75, NULL);
	LineTo(hdc, 445, 46);
	MoveToEx(hdc, 430, 76, NULL);
	LineTo(hdc, 430, 89);

	rc.top = 46;
	rc.bottom = 76;
	rc.left = 415;
	rc.right = 442;
	cs = _T("O2");
	DrawText(hdc,cs,-1,&rc,DT_RIGHT|DT_BOTTOM|DT_SINGLELINE);*/

	//Inspirationsschlauch
	/*if(		alarmState->eSoA_P_INSPIRATION==AS_ACTIVE
		||	alarmState->eSoA_P_EXSPIRATION==AS_ACTIVE)
	{
		SelectObject(hdc,cbrRed);
	}
	else if(	alarmState->eSoA_P_INSPIRATION==AS_SIGNALED
			||	alarmState->eSoA_P_EXSPIRATION==AS_SIGNALED)
	{
		SelectObject(hdc,cbrGrey);
	}
	else 
	{
		SelectObject(hdc,cbrGreen);
	}
	Rectangle(hdc,470,46,500,76);
	MoveToEx(hdc, 470, 75, NULL);
	LineTo(hdc, 500, 45);
	MoveToEx(hdc, 485, 76, NULL);
	LineTo(hdc, 485, 89);

	rc.top = 46;
	rc.bottom = 76;
	rc.left = 470;
	rc.right = 497;
	cs = _T("P");
	DrawText(hdc,cs,-1,&rc,DT_RIGHT|DT_BOTTOM|DT_SINGLELINE);*/

	//Exspirationsschlauch
	/*if(alarmState->eSoA_P_EXSPIRATION==AS_ACTIVE)
	{
		SelectObject(hdc,cbrRed);
	}
	else if(alarmState->eSoA_P_EXSPIRATION==AS_SIGNALED)
	{
		SelectObject(hdc,cbrGrey);
	}
	else 
	{
		SelectObject(hdc,cbrGreen);
	}
	

	
	Rectangle(hdc,610,89,640,119);
	MoveToEx(hdc, 610, 118, NULL);
	LineTo(hdc, 640, 88);
	MoveToEx(hdc, 625, 119, NULL);
	LineTo(hdc, 625, 130);

	rc.top = 89;
	rc.bottom = 119;
	rc.left = 610;
	rc.right = 637;
	cs = _T("P");
	DrawText(hdc,cs,-1,&rc,DT_RIGHT|DT_BOTTOM|DT_SINGLELINE);*/



	//Druckmessschlauch
	eStateOfAlarm stateTUBE_OCCLUSION=getModel()->getALARMHANDLER()->ALARM_SysAl_TUBE_OCCLUSION->getAlarmState();
	if(stateTUBE_OCCLUSION==AS_ACTIVE)
	{
		SelectObject(hdc,cbrRed);
	}
	else if(stateTUBE_OCCLUSION==AS_SIGNALED)
	{
		SelectObject(hdc,cbrGrey);
	}
	else 
	{
		SelectObject(hdc,cbrGreen);
	}
	Rectangle(hdc,470,46,500,76);
	MoveToEx(hdc, 470, 75, NULL);
	LineTo(hdc, 500, 45);
	MoveToEx(hdc, 485, 76, NULL);
	LineTo(hdc, 485, 89);

	rc.top = 46;
	rc.bottom = 76;
	rc.left = 470;
	rc.right = 497;
	cs = _T("P");
	DrawText(hdc,cs,-1,&rc,DT_RIGHT|DT_BOTTOM|DT_SINGLELINE);


	/*Rectangle(hdc,610,89,640,119);
	MoveToEx(hdc, 610, 118, NULL);
	LineTo(hdc, 640, 88);
	MoveToEx(hdc, 625, 119, NULL);
	LineTo(hdc, 625, 130);

	rc.top = 89;
	rc.bottom = 119;
	rc.left = 610;
	rc.right = 637;
	cs = _T("P");
	DrawText(hdc,cs,-1,&rc,DT_RIGHT|DT_BOTTOM|DT_SINGLELINE);*/

	Rectangle(hdc,470,123,500,153);
	MoveToEx(hdc, 470, 152, NULL);
	LineTo(hdc, 500, 122);
	MoveToEx(hdc, 500, 137, NULL);
	LineTo(hdc, 513, 137);

	rc.top = 132;
	rc.bottom = 153;
	rc.left = 470;
	rc.right = 497;
	cs = _T("P");
	DrawText(hdc,cs,-1,&rc,DT_RIGHT|DT_BOTTOM|DT_SINGLELINE);

	//Mischer/AV
	eStateOfAlarm stateMIXER_AVCAL=getModel()->getALARMHANDLER()->ALARM_SysFail_MIXER_AVCAL->getAlarmState();
	if(stateMIXER_AVCAL==AS_ACTIVE)
	{
		int iBLENDERstate=getModel()->getDATAHANDLER()->GetMSTATUS_BlenderPIC();
		if(iBLENDERstate & BIT8)//AV calibration error
		{
			if(m_pcAVCalAlarm)
				m_pcAVCalAlarm->Draw(hdc,440,173);
		}
		else
		{
			if(m_pcMixerAlarm)
				m_pcMixerAlarm->Draw(hdc,306,48);
		}

		
	}
	else if(stateMIXER_AVCAL==AS_SIGNALED)
	{
		int iBLENDERstate=getModel()->getDATAHANDLER()->GetMSTATUS_BlenderPIC();
		if(iBLENDERstate & BIT8)//AV calibration error
		{
			if(m_pcAVCalSignal)
				m_pcAVCalSignal->Draw(hdc,440,173);
		}
		else
		{
			if(m_pcMixerSignal)
				m_pcMixerSignal->Draw(hdc,306,48);
		}
	}
	


	

	//Eingangsdruck Mischer
	//if(		alarmState.ePIN_MIXER==AS_ACTIVE
	//	||	alarmState.ePIN_MIXER==AS_SIGNALED)
	//{
	//	SelectObject(hdc,cbrRed);
	//}
	//else 
	//{
	//	SelectObject(hdc,cbrGreen);
	//}
	//ptArray[0].x = 200;
	//ptArray[0].y = 277;
	//ptArray[1].x = 220;
	//ptArray[1].y = 262;
	//ptArray[2].x = 240;
	//ptArray[2].y = 277;
	//ptArray[3].x = 220;
	//ptArray[3].y = 292;
	//Polygon(hdc,ptArray,4);

	//rc.top = 262;
	//rc.bottom = 292;
	//rc.left = 200;
	//rc.right = 240;
	////cs = _T("P");
	//DrawText(hdc,cs,-1,&rc,DT_CENTER|DT_VCENTER|DT_SINGLELINE);

	//Relais
	/*if(		alarmState.eRELAIS_DEFECT==AS_ACTIVE
		||	alarmState.eRELAIS_DEFECT==AS_SIGNALED)
	{
		SelectObject(hdc,cbrRed);
	}
	else 
	{
		SelectObject(hdc,cbrGreen);
	}
	ptArray[0].x = 100;
	ptArray[0].y = 387;
	ptArray[1].x = 120;
	ptArray[1].y = 372;
	ptArray[2].x = 140;
	ptArray[2].y = 387;
	ptArray[3].x = 120;
	ptArray[3].y = 402;
	Polygon(hdc,ptArray,4);

	rc.top = 372;
	rc.bottom = 402;
	rc.left = 100;
	rc.right = 140;
	cs = getModel()->GetLanguageString(IDS_ALARM_REL);
	DrawText(hdc,cs,-1,&rc,DT_CENTER|DT_VCENTER|DT_SINGLELINE);*/

	//Spannung
	//if(		alarmState.eVOLTAGE==AS_ACTIVE
	//	||	alarmState.eVOLTAGE==AS_SIGNALED)
	//{
	//	SelectObject(hdc,cbrRed);
	//}
	//else 
	//{
	//	SelectObject(hdc,cbrGreen);
	//}
	//ptArray[0].x = 161;
	//ptArray[0].y = 387;
	//ptArray[1].x = 181;
	//ptArray[1].y = 372;
	//ptArray[2].x = 201;
	//ptArray[2].y = 387;
	//ptArray[3].x = 181;
	//ptArray[3].y = 402;
	//Polygon(hdc,ptArray,4);

	//rc.top = 372;
	//rc.bottom = 402;
	//rc.left = 161;
	//rc.right = 201;
	////cs = _T("~U");
	//cs = getModel()->GetLanguageString(IDS_ALARM_DC);
	//DrawText(hdc,cs,-1,&rc,DT_CENTER|DT_VCENTER|DT_SINGLELINE);






	

	SelectObject(hdc,g_hf8AcuBold);
	rc.top = 40;
	rc.bottom = 100;
	rc.left = 138;
	rc.right = 230;
	cs = getModel()->GetLanguageString(IDS_ALARM_AIR);
	DrawText(hdc,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

	rc.top = 130;
	rc.bottom = 190;
	rc.left = 138;
	rc.right = 230;
	cs = getModel()->GetLanguageString(IDS_ALARM_OXY);
	DrawText(hdc,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

	rc.top = 126;
	rc.bottom = 186;
	rc.left = 225;
	rc.right = 465;
	cs = getModel()->GetLanguageString(IDS_ALARM_BLENDER);
	DrawText(hdc,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);


	rc.top = 55;
	rc.bottom = 100;
	rc.left = 525;
	rc.right = 800;
	//cs = _T("Insp");
	cs = getModel()->GetLanguageString(IDS_TXT_INSP);
	DrawText(hdc,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.top = 120;
	rc.bottom = 640;
	rc.left = 525;
	rc.right = 800;
	//cs = _T("Prox");
	cs = getModel()->GetLanguageString(IDS_TXT_PROX);
	DrawText(hdc,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.top = 210;
	rc.bottom = 640;
	rc.left = 525;
	rc.right = 800;
	//cs = _T("Exp");
	cs = getModel()->GetLanguageString(IDS_TXT_EXP);
	DrawText(hdc,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


	/*rc.top = 408;
	rc.bottom = 435;
	rc.left = 90;
	rc.right = 374;
	if(		alarmState.eERROR_1==AS_ACTIVE
		||	alarmState.eERROR_1==AS_SIGNALED
		||	alarmState.eERROR_2==AS_ACTIVE
		||	alarmState.eERROR_2==AS_SIGNALED)
	{
		FillRect(hdc,&rc,cbrRed);
	}
	cs = getModel()->GetLanguageString(IDS_ALARM_CPU);
	DrawText(hdc,cs,-1,&rc,DT_CENTER|DT_VCENTER|DT_SINGLELINE);*/

	//Eingangsdruck Air
	// AlarmState_P_IN_AIR **********************************************************
	if(stateP_IN_AIR==AS_ACTIVE)
	{
		SelectObject(hdc, penRed);
		
		//draw X
		MoveToEx(hdc, 170, 58, NULL);
		LineTo(hdc, 195, 83);
		MoveToEx(hdc, 170, 83, NULL);
		LineTo(hdc, 195, 58);
	}
	else if(stateP_IN_AIR==AS_SIGNALED)
	{
		SelectObject(hdc, penGrey);

		//draw X
		MoveToEx(hdc, 170, 58, NULL);
		LineTo(hdc, 195, 83);
		MoveToEx(hdc, 170, 83, NULL);
		LineTo(hdc, 195, 58);
	}

	//Eingangsdruck O2
	// AlarmState_P_IN_O2 **********************************************************
	if(stateP_IN_O2==AS_ACTIVE)
	{
		SelectObject(hdc,penRed);

		//draw X
		MoveToEx(hdc, 170, 98, NULL);
		LineTo(hdc, 195, 123);
		MoveToEx(hdc, 170, 123, NULL);
		LineTo(hdc, 195, 98);
	}
	else if(stateP_IN_O2==AS_SIGNALED)
	{
		SelectObject(hdc,penGrey);

		//draw X
		MoveToEx(hdc, 170, 98, NULL);
		LineTo(hdc, 195, 123);
		MoveToEx(hdc, 170, 123, NULL);
		LineTo(hdc, 195, 98);
	}

	//Inspirationsschlauch
	if(stateTUBE_OCCLUSION==AS_ACTIVE)
	{
		SelectObject(hdc,penRed);

		//draw X
		MoveToEx(hdc, 547, 79, NULL);
		LineTo(hdc, 572, 104);
		MoveToEx(hdc, 547, 104, NULL);
		LineTo(hdc, 572, 79);
	}
	else if(stateTUBE_OCCLUSION==AS_SIGNALED)
	{
		SelectObject(hdc,penGrey);

		//draw X
		MoveToEx(hdc, 547, 79, NULL);
		LineTo(hdc, 572, 104);
		MoveToEx(hdc, 547, 104, NULL);
		LineTo(hdc, 572, 79);
	}

	//Exspirationsschlauch
	if(stateTUBE_OCCLUSION==AS_ACTIVE)
	{
		SelectObject(hdc,penRed);

		//draw X
		MoveToEx(hdc, 547, 171, NULL);
		LineTo(hdc, 572, 196);
		MoveToEx(hdc, 547, 196, NULL);
		LineTo(hdc, 572, 171);
	}
	else if(stateTUBE_OCCLUSION==AS_SIGNALED)
	{
		SelectObject(hdc,penGrey);

		//draw X
		MoveToEx(hdc, 547, 171, NULL);
		LineTo(hdc, 572, 196);
		MoveToEx(hdc, 547, 196, NULL);
		LineTo(hdc, 572, 171);
	}

	//Druckmessschlauch
	if(stateTUBE_OCCLUSION==AS_ACTIVE)
	{
		SelectObject(hdc,penRed);

		//draw X
		MoveToEx(hdc, 542, 126, NULL);
		LineTo(hdc, 567, 151);
		MoveToEx(hdc, 542, 151, NULL);
		LineTo(hdc, 567, 126);
	}
	else if(stateTUBE_OCCLUSION==AS_SIGNALED)
	{
		SelectObject(hdc,penGrey);

		//draw X
		MoveToEx(hdc, 542, 126, NULL);
		LineTo(hdc, 567, 151);
		MoveToEx(hdc, 542, 151, NULL);
		LineTo(hdc, 567, 126);
	}

	
	SetBkMode(hdc,nBkMode);
	SelectObject(hdc,hPrevFont);
	SetTextColor(hdc,nPrevTxtColor);
	SelectObject(hdc,hbrprev);
	SelectObject(hdc,hpenprev);

	penRed.DeleteObject();
	penGrey.DeleteObject();
}


//bool CViewSystemAlarm::CreateWndAlarmLog()
//{
//	if(m_pWndAlarmLog==NULL)
//	{
//
//
//		m_pWndAlarmLog = new CWndAlarmLog();
//		//RECT rcLd={m_lX,m_lY,m_lX+m_lCx,m_lY+m_lCy};
//
//		RECT rcLd={0,0,800,500};
//		//RECT rcLd={0,0,565,485};
//		if(m_pWndAlarmLog->CreateWnd(this,rcLd,IDC_VIEW_LOGLIST_LOG))
//		{
//
//
//			return true;
//		}
//
//
//	}
//	return false;
//}
//bool CViewSystemAlarm::DestroyWndAlarmLog()
//{
//	if(m_pWndAlarmLog)
//	{
//		m_pWndAlarmLog->DestroyWindow();
//		delete m_pWndAlarmLog;
//		m_pWndAlarmLog=NULL;
//	}
//	return false;
//}
//void CViewSystemAlarm::ShowWndAlarmLog(bool show)
//{
//	m_pWndAlarmLog->Show(show);
//
//
//}

/**********************************************************************************************//**
 * Notifies an event
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param [in,out]	pEvent	If non-null, the event.
 **************************************************************************************************/

void CViewSystemAlarm::NotifyEvent(CMVEvent* pEvent)
{
	switch(pEvent->GetET())
	{
	case CMVEvent::ET_ALARM:
		{
			CMVEventAlarm* pUIEvent = (CMVEventAlarm*)pEvent;
			switch(pUIEvent->GetEventType())
			{
			case CMVEventAlarm::EV_ALARMSTATE_CHANGED:
				{
					
					try
					{
						DrawAlarms();
					}
					catch (...)
					{
						theApp.ReportException(_T("CViewSystemAlarm::NotifyEvent EV_ALARMSTATE_CHANGED"));
					}
					

					if(getModel()->getALARMHANDLER()->isActiveAlarmType(AT_SYSALARM)==false)
					{
						SetTimer(SYSALARMTIMER,5000, NULL);
					}
					else
					{
						KillTimer(SYSALARMTIMER);
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

LRESULT CViewSystemAlarm::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
{
	switch(message)
	{
	case WM_SETVIEWFOCUS:
		{
			//rku cs1
			SetViewFocus();
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
	case WM_SETNEXTFOCUS:
		{
			//rku cs1
			SetNextFocus();
			return 1;
		}
		break;
	case WM_MENUBUTTONDOWN:
		{
			switch(wParam)
			{
			case IDC_BTN_SYSALARM_O2:
				{
					if(getModel()->getDATAHANDLER()->GetO2SensorState()!=OXYSENS_OFF)
					{
						getModel()->getDATAHANDLER()->SetO2SensorState(OXYSENS_OFF);
						/*CMVEventControl eventCtrl(CMVEventControl::EV_CONTROL_O2SENSORSTATE);
						getModel()->Trigger(&eventCtrl);*/

						if(AfxGetApp())
							AfxGetApp()->GetMainWnd()->PostMessage(WM_TRIGGER_O2SENSORSTATE);

						m_pcMenuSens->EnableWindow(FALSE);
						m_pcMenuSens->RefreshText(getModel()->GetLanguageString(IDS_MENU_CAL6),getModel()->GetLanguageString(IDS_TXT_NOTACTIVE));
						m_bDrawO2SensorBtn=true;

					}
					return 1;
				}
				break;
			case IDC_BTN_SYSALARM_BACK:
				{
					
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_NUMERIC);
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

/**********************************************************************************************//**
 * Executes the timer action
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	nIDEvent	The identifier event.
 **************************************************************************************************/

void CViewSystemAlarm::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==SYSALARMTIMER)
	{
		KillTimer(SYSALARMTIMER);

		if(		getModel()->getALARMHANDLER()->isActiveAlarmType(AT_SYSALARM)==false
			&&	getModel()->getALARMHANDLER()->isSignaledAlarmType(AT_SYSALARM)==false)
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_NUMERIC);
		}
	}

	CWnd::OnTimer(nIDEvent);
}