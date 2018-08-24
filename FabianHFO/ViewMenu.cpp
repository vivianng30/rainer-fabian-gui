// ViewMenu.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "ViewMenu.h"

/**********************************************************************************************//**
 * A macro that defines menumodechange
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

#define MENUMODECHANGE	45000

/**********************************************************************************************//**
 * CViewMenu
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	iViewID	Identifier for the view.
 **************************************************************************************************/

CViewMenu::CViewMenu(int iViewID):
CMVView(iViewID)
{
	m_bExit=false;

	m_pWndMenuMenu=NULL;

	m_pcSubViewMenu=NULL;

	m_pcWndSubScreenlocktime=NULL;
	m_pcWndSubVentRange=NULL;
	m_pcWndSubRatioItime=NULL;
	m_pcWndSubManbreathTime=NULL;
	m_pcWndSubPatAlarmDelay=NULL;
	m_pcWndSubVolTrigger=NULL;
	m_pcWndSubTubeset=NULL;
	m_pcWndSubPpsvAbsolute=NULL;
	m_pcWndSubBTBforVT=NULL;
	m_pcWndSubEFlow=NULL;
	m_pcWndSubAutoOxyCal=NULL;
	m_pcWndSubHFOManBreath=NULL;
	m_pcWndSubCO2Module=NULL;
	m_pcWndSubCO2Units=NULL;
	m_pcWndSubBaroPressure=NULL;
	m_pcWndSubAutoOnPump=NULL;
	m_pcWndSubPressureUnit=NULL;
	m_pcWndSubSPO2Module=NULL;
	m_pcWndSubSPO2sensitivity=NULL;
	m_pcWndSubSPO2averagingtime=NULL;
	m_pcWndSubSPO2alarmdelay=NULL;
	m_pcWndSubLeakComp=NULL;

	m_viewFlag=0;

	m_pcSubViewFlowSensor=NULL;
	m_pcSubViewOxySensor=NULL;
	m_pcSubViewService=NULL;
	m_pcSubViewSystem=NULL;
	m_pcSubViewPatient=NULL;
	m_pcSubViewVentilation=NULL;
	m_pcSubViewDateTime=NULL;
	m_pcSubViewLanguage=NULL;
	m_pcSubViewDisplay=NULL;
	m_pcSubViewTools=NULL;
	m_pcSubViewCO2=NULL;
	m_pcSubViewVideo=NULL;
	m_pcSubViewNetwork=NULL;
	m_pcSubViewSPO2=NULL;

	m_eCurView=MV_CALIBRATION;
	m_ePrevView=MV_CALIBRATION;

	//m_bCO2FeatureAvailable=getModel()->getDATAHANDLER()->isCO2LicenseAvailable();

	m_bVideoRunning=false;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CViewMenu class
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

CViewMenu::~CViewMenu()
{
}


BEGIN_MESSAGE_MAP(CViewMenu, CMVView)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * CViewMenue message handlers
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::CreateView()
{
	//RECT rcLd={5,50,570,535};
	RECT rcLd={0,41,800,551};

	if (!Create(AfxGetApp()->GetMainWnd(),rcLd,IDC_VIEW_SETUP))
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

BOOL CViewMenu::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext) 
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

		//CBrush cbrBack(RGB(255,0,0));
		CBrush cbrBack(BACKGND);
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

/**********************************************************************************************//**
 * Initializes this instance
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::Initialize()
{
	bool result=CreateWndMenuMenu();

	if(result)
		result=CreateSubViewFlowSensor();
	/*if(result)
		result=CreateSubViewOxySensor();*/
	ShowSubViewFlowSensor(true);
	//ShowSubViewOxySensor(true);
	getModel()->SetStateCalibrationView(true);
	
	ShowWndMenuMenu(true,getModel()->GetLanguageString(IDS_BTN_MNU_CAL),true,false);

	PostMessage(WM_SET_SETUPTIMER);

	return result;

}

/**********************************************************************************************//**
 * Opens this instance
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewMenu::Open()
{
	PostMessage(WM_SET_SETUPTIMER);
}

/**********************************************************************************************//**
 * Closes this instance
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewMenu::Close()
{
	if(getModel()->getCONFIG()->IsAutoScreenlockActive())
	{
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_SET_AUTOSCREENLOG_TIMER);
	}
	else
	{
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_KILL_AUTOSCREENLOG_TIMER);
	}
}

/**********************************************************************************************//**
 * Shows
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	bRedraw	True to redraw.
 **************************************************************************************************/

void CViewMenu::Show(bool bRedraw)
{
	if(m_bExit)
		return;

	CStringW csMenuText=_T("");
	bool bMenuBtnOpen=true;

	if(bRedraw)
	{
		if(m_viewFlag!=0)
		{
			switch(m_viewFlag)
			{
			case  IDC_BTN_SETUP_SENSITIVITYMODE:
				{
					ShowWndSubSettingSPO2sensitivity(true);
				}
				break;
			case  IDC_BTN_SETUP_SPO2AVERAGING:
				{
					ShowWndSubSettingSPO2averagingtime(true);
				}
				break;
			case  IDC_BTN_SETUP_SPO2ALARMDELAY:
				{
					ShowWndSubSettingSPO2alarmdelay(true);
				}
				break;
			case  IDC_BTN_SETUP_SPO2MODULE:
				{
					ShowWndSubSettingSPO2Module(true);
				}
				break;
			case  IDC_BTN_SETUP_CO2MODULE:
				{
					ShowWndSubSettingCO2Module(true);
				}
				break;
			case  IDC_BTN_SETUP_CO2UNITS:
				{
					ShowWndSubSettingCO2Units(true);
				}
				break;
			case  IDC_BTN_SETUP_BAROPRESSURE:
				{
					ShowWndSubSettingBaroPressure(true);
				}
				break;
			case  IDC_BTN_SETUP_AUTOONPUMP:
				{
					ShowWndSubSettingAutoOnPump(true);
				}
				break;
			case  IDC_BTN_SETUP_ETCO2AVG:
				{

				}
				break;
			case IDC_BTN_SCREENLOCKTIME:
				{
					ShowWndSubSettingScreenlocktime(true);
				}
				break;
			case IDC_BTN_SETUP_RANGE:
				{
					ShowWndSubSettingVentRange(true);
				}
				break;
			case IDC_BTN_SETUP_FUNCITIME:
				{
					ShowWndSubSettingRatioItime(true);
				}
				break;
			case IDC_BTN_SETUP_MANBREATHTIME:
				{
					ShowWndSubSettingManbreathTime(true);
				}
				break;
			case IDC_BTN_SETUP_PATALARMDELAY:
				{
					ShowWndSubSettingSubPatAlarmDelay(true);
				}
				break;
			case IDC_BTN_SETUP_LEAKCOMP:
				{
					ShowWndSubSettingLeakComp(true);
				}
				break;
			case IDC_BTN_SETUP_VOLUMETRIG:
				{
					ShowWndSubSettingVolTrigger(true);
				}
				break;
			case IDC_BTN_SETUP_PPSVABSOLUTE:
				{
					ShowWndSubSettingPpsvAbsolute(true);
				}
				break;
			case IDC_BTN_SETUP_BTBVT:
				{
					ShowWndSubSettingBTBforVT(true);
				}
				break;
			case IDC_BTN_SETUP_TUBESET:
				{
					ShowWndSubSettingTubeset(true);
				}
				break;
			case IDC_BTN_SETUP_PRESSUREUNIT:
				{
					ShowWndSubSettingPressureUnit(true);
				}
				break;
			case IDC_BTN_SETUP_EFLOW:
				{
					ShowWndSubSettingEFlow(true);
				}
				break;
			case IDC_BTN_SETUP_AUTOOXYCAL:
				{
					ShowWndSubSettingAutoOxyCal(true);
				}
				break;
			case IDC_BTN_SETUP_HFOMANBREATH:
				{
					ShowWndSubSettingHFOManBreath(true);
				}
				break;
			}
		}
		else
		{
			switch(m_eCurView)
			{
			case MV_MENU:
				{
					theApp.WriteLog(_T("MV_MENU"));

					ShowSubViewMenu(true);
					csMenuText=getModel()->GetLanguageString(IDS_TXT_SELMENU);
				}
				break;
			case MV_CALIBRATION:
				{
					ShowSubViewFlowSensor(true);
					//ShowSubViewOxySensor(true);
					csMenuText=getModel()->GetLanguageString(IDS_BTN_MNU_CAL);
				}
				break;
			default:
				{
					theApp.WriteLog(_T("MV_D"));
					OpenMenuSelect();
					csMenuText=getModel()->GetLanguageString(IDS_TXT_SELMENU);
				}
				break;
			}
		}
		
	}
	else
	{
		//if(getModel()->getALARMHANDLER()->isActiveAlarmType(AT_SENSOR))
		//{
		//	if(m_viewFlag!=0)
		//	{
		//		CloseSettingsView(m_viewFlag);
		//	}

		//	switch(m_eCurView)
		//	{
		//	case MV_MENU:
		//		{
		//			DestroySubViewMenu();
		//		}
		//		break;
		//	case MV_CALIBRATION:
		//		{
		//			//DestroySubViewFlowSensor();
		//			//DestroySubViewOxySensor();
		//		}
		//		break;
		//	case MV_DISPLAY:
		//		{
		//			DestroySubViewDisplay();
		//		}
		//		break;
		//	case MV_VENTILATION:
		//		{
		//			DestroySubViewVentilation();
		//		}
		//		break;
		//	case MV_PATIENTDATA:
		//		{
		//			DestroySubViewPatient();
		//		}
		//		break;
		//	case MV_LANGUAGE:
		//		{
		//			DestroySubViewLanguage();
		//		}
		//		break;
		//	case MV_DATETIME:
		//		{
		//			DestroySubViewDateTime();
		//		}
		//		break;
		//	case MV_TOOLS:
		//		{
		//			DestroySubViewTools();
		//		}
		//		break;
		//	case MV_SYSTEM:
		//		{
		//			DestroySubViewSystem();
		//		}
		//		break;
		//	case MV_SERVICEMODE:
		//		{
		//			DestroySubViewService();
		//		}
		//		break;
		//	case MV_VIDEO:
		//		{
		//			DestroySubViewVideo();
		//		}
		//		break;
		//	case MV_NETWORK:
		//		{
		//			DestroySubViewNetwork();
		//		}
		//		break;
		//	//case MV_HELP:
		//	//	{
		//	//		//DestroySubViewHelp();
		//	//	}
		//	//	break;
		//	case MV_O2:
		//		{
		//			DestroySubViewOxySensor();
		//		}
		//		break;
		//	case MV_CO2:
		//		{
		//			DestroySubViewCO2();
		//		}
		//		break;
		//	case MV_SPO2:
		//		{
		//			DestroySubViewSPO2();
		//		}
		//		break;
		//	default:
		//		{
		//		}
		//		break;
		//	}

		//	if(m_eCurView!=MV_CALIBRATION)
		//	{
		//		getModel()->SetStateCalibrationView(true);
		//		m_eCurView=MV_CALIBRATION;
		//		CreateSubViewFlowSensor();
		//		//CreateSubViewOxySensor();
		//		ShowSubViewFlowSensor(true);
		//		//ShowSubViewOxySensor(true);
		//	}

		//	csMenuText=getModel()->GetLanguageString(IDS_BTN_MNU_CAL);
		//	ShowWndMenuMenu(true,csMenuText,bMenuBtnOpen,false);
		//}
		//else
		{
			if(m_viewFlag!=0)
			{
				CloseSettingsView(m_viewFlag);
			}
			else
			{
				switch(m_eCurView)
				{
				case MV_O2:
					{
						DestroySubViewOxySensor();

						getModel()->SetStateCalibrationView(true);
						m_eCurView=MV_CALIBRATION;
						CreateSubViewFlowSensor();
						ShowSubViewFlowSensor(true);
						csMenuText=getModel()->GetLanguageString(IDS_BTN_MNU_CAL);
						ShowWndMenuMenu(true,csMenuText,bMenuBtnOpen,false);
					}
					break;
				case MV_CO2:
					{
						DestroySubViewCO2();

						getModel()->SetStateCalibrationView(true);
						m_eCurView=MV_CALIBRATION;
						CreateSubViewFlowSensor();
						ShowSubViewFlowSensor(true);
						csMenuText=getModel()->GetLanguageString(IDS_BTN_MNU_CAL);
						ShowWndMenuMenu(true,csMenuText,bMenuBtnOpen,false);
					}
					break;
				case MV_SPO2:
					{
						DestroySubViewSPO2();

						getModel()->SetStateCalibrationView(true);
						m_eCurView=MV_CALIBRATION;
						CreateSubViewFlowSensor();
						ShowSubViewFlowSensor(true);
						csMenuText=getModel()->GetLanguageString(IDS_BTN_MNU_CAL);
						ShowWndMenuMenu(true,csMenuText,bMenuBtnOpen,false);
					}
					break;
				case MV_MENU:
					{
						ShowMenu(MV_CALIBRATION);
						csMenuText=getModel()->GetLanguageString(IDS_BTN_MNU_CAL);

						ShowWndMenuMenu(true,csMenuText,bMenuBtnOpen,false);
					}
					break;
				case MV_CALIBRATION:
					{
						DestroySubViewFlowSensor();
						//DestroySubViewOxySensor();

						OpenMenuSelect();
						csMenuText=getModel()->GetLanguageString(IDS_TXT_SELMENU);

						ShowWndMenuMenu(true,csMenuText,bMenuBtnOpen,false);
					}
					break;
				default:
					{
					}
					break;
				}
			}
		}
	}
	
	PostMessage(WM_SET_SETUPTIMER);
}

/**********************************************************************************************//**
 * Shows the menu
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	menu	The menu.
 **************************************************************************************************/

void CViewMenu::ShowMenu(eMenuView menu)
{
	if(m_bExit)
		return;

	//CClientDC dc(this);
	CStringW csMenuText=_T("");
	
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->SetFocus();

	DestroySubViewMenu();
	DestroySettingsView();

	
		
	Invalidate();
	UpdateWindow();
	//SetWindowPos(NULL,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_SHOWWINDOW);


	switch(menu)
	{
	case MV_CALIBRATION:
		{
			getModel()->SetStateCalibrationView(true);
			m_eCurView=MV_CALIBRATION;
			CreateSubViewFlowSensor();
			//CreateSubViewOxySensor();
			ShowSubViewFlowSensor(true);
			//ShowSubViewOxySensor(true);
			csMenuText=getModel()->GetLanguageString(IDS_BTN_MNU_CAL);
		}
		break;
	case MV_DISPLAY:
		{
			getModel()->SetStateCalibrationView(false);
			m_eCurView=MV_DISPLAY;
			CreateSubViewDisplay();
			ShowSubViewDisplay(true);
			csMenuText=getModel()->GetLanguageString(IDS_BTN_MNU_DIS);
		}
		break;
	case MV_VENTILATION:
		{
			getModel()->SetStateCalibrationView(false);
			m_eCurView=MV_VENTILATION;
			CreateSubViewVentilation();
			ShowSubViewVentilation(true);
			csMenuText=getModel()->GetLanguageString(IDS_BTN_MNU_VENT);
		}
		break;
	case MV_PATIENTDATA:
		{
			getModel()->SetStateCalibrationView(false);
			m_eCurView=MV_PATIENTDATA;
			CreateSubViewPatient();
			ShowSubViewPatient(true);
			csMenuText=getModel()->GetLanguageString(IDS_BTN_MNU_PAT);
		}
		break;
	case MV_LANGUAGE:
		{
			getModel()->SetStateCalibrationView(false);
			m_eCurView=MV_LANGUAGE;
			CreateSubViewLanguage();
			ShowSubViewLanguage(true);
			csMenuText=getModel()->GetLanguageString(IDS_BTN_MNU_LANG);
		}
		break;
	case MV_DATETIME:
		{
			getModel()->SetStateCalibrationView(false);
			m_eCurView=MV_DATETIME;
			CreateSubViewDateTime();
			ShowSubViewDateTime(true);
			csMenuText=getModel()->GetLanguageString(IDS_BTN_MNU_DATETIME);
		}
		break;
	case MV_TOOLS:
		{
			getModel()->SetStateCalibrationView(false);
			m_eCurView=MV_TOOLS;
			CreateSubViewTools();
			ShowSubViewTools(true);
			csMenuText=getModel()->GetLanguageString(IDS_BTN_MNU_TOOLS);
		}
		break;
	case MV_SYSTEM:
		{
			getModel()->SetStateCalibrationView(false);
			m_eCurView=MV_SYSTEM;
			CreateSubViewSystem();
			ShowSubViewSystem(true);
			csMenuText=getModel()->GetLanguageString(IDS_BTN_MNU_INFO);
		}
		break;
	case MV_SERVICEMODE:
		{
			getModel()->SetStateCalibrationView(false);
			m_eCurView=MV_SERVICEMODE;
			CreateSubViewService();
			ShowSubViewService(true);
			csMenuText=getModel()->GetLanguageString(IDS_BTN_MNU_SERVICE);
		}
		break;
	case MV_VIDEO:
		{
			getModel()->SetStateCalibrationView(false);
			m_eCurView=MV_VIDEO;
			CreateSubViewVideo();
			ShowSubViewVideo(true);
			csMenuText=getModel()->GetLanguageString(IDS_BTN_MNU_VIDEO);
		}
		break;
	case MV_NETWORK:
		{
			getModel()->SetStateCalibrationView(false);
			m_eCurView=MV_NETWORK;
			CreateSubViewNetwork();
			ShowSubViewNetwork(true);
			csMenuText=getModel()->GetLanguageString(IDS_BTN_MNU_PDMS);
		}
		break;
	//case MV_HELP:
	//	{
	//		getModel()->SetStateCalibrationView(false);
	//		//m_eCurView=MV_HELP;
	//		//CreateSubViewHelp();
	//		//ShowSubViewHelp(true);
	//		csMenuText=getModel()->GetLanguageString(IDS_BTN_MNU_HELP);
	//	}
	//	break;
	case MV_O2:
		{
			DestroySubViewFlowSensor();
			getModel()->SetStateCalibrationView(false);
			m_eCurView=MV_O2;
			CreateSubViewOxySensor();
			ShowSubViewOxySensor(true);
			csMenuText=getModel()->GetLanguageString(IDS_BTN_MNU_O2);
		}
		break;
	case MV_CO2:
		{
			DestroySubViewFlowSensor();
			getModel()->SetStateCalibrationView(false);
			m_eCurView=MV_CO2;
			CreateSubViewCO2();
			ShowSubViewCO2(true);
			csMenuText=getModel()->GetLanguageString(IDS_BTN_MNU_CO2);
		}
		break;
	case MV_SPO2:
		{
			DestroySubViewFlowSensor();
			getModel()->SetStateCalibrationView(false);
			m_eCurView=MV_SPO2;
			CreateSubViewSPO2();
			ShowSubViewSPO2(true);
			csMenuText=getModel()->GetLanguageString(IDS_BTN_MNU_SPO2);
		}
		break;
	}

	ShowWndMenuMenu(true,csMenuText,true,false);

	PostMessage(WM_SET_SETUPTIMER);
}

/**********************************************************************************************//**
 * Hides this instance
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewMenu::Hide()
{
	this->ShowWindow(SW_HIDE);

	ShowSubViewFlowSensor(false);
	ShowSubViewOxySensor(false);
	ShowSubViewCO2(false);
	ShowSubViewSPO2(false);
	ShowSubViewDisplay(false);
	ShowSubViewLanguage(false);
	ShowSubViewDateTime(false);
	ShowSubViewVentilation(false);
	ShowSubViewPatient(false);
	ShowSubViewSystem(false);
	ShowSubViewService(false);
	ShowSubViewTools(false);
	ShowSubViewVideo(false);
	ShowSubViewNetwork(false);

	ShowWndMenuMenu(false,_T(""),true,false);
}

/**********************************************************************************************//**
 * Paints this window
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewMenu::OnPaint()
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
 * \date	23.02.2018
 **************************************************************************************************/

void CViewMenu::OnDestroy()
{
	m_bExit=true;

	KillTimer(SETUPTIMER);

	/*if(getModel()->IsStartupFlag()
		&&	getModel()->getCONFIG()->GetCurMode()!=VM_NCPAP 
		&&	getModel()->getCONFIG()->GetCurMode()!=VM_DUOPAP)
		getModel()->DeleteStartupFlag();*/

	getModel()->SetStateCalibrationView(false);

	DestroyWndMenuMenu();

	DestroySubViewMenu();
	DestroySettingsView();

	DestroyWndSubSettingScreenlocktime();
	DestroyWndSubSettingVentRange();
	DestroyWndSubSettingRatioItime();
	//DestroyWndSubSettingFlushtime();
	DestroyWndSubSettingManbreathTime();
	DestroyWndSubSettingVolTrigger();
	DestroyWndSubSettingPpsvAbsolute();
	DestroyWndSubSettingTubeset();
	//DestroyWndSubSettingVolgarFilter();
	//DestroyWndSubSettingAbortPSV();
	DestroyWndSubSettingEFlow();
	DestroyWndSubSettingAutoOxyCal();
	/*DestroyWndSubSettingPatData();
	DestroyWndSubSettingTriggerSound();*/
	DestroyWndSubSettingHFOManBreath();
	DestroyWndSubSettingCO2Module();
	DestroyWndSubSettingCO2Units();
	DestroyWndSubSettingBaroPressure();
	DestroyWndSubSettingAutoOnPump();
	//DestroyWndSubSettingVlimitVgarant();
	//DestroyWndSubSettingSubPminDelay();
	DestroyWndSubSettingSubPatAlarmDelay();
	DestroyWndSubSettingPressureUnit();
	//DestroyWndSubSettingVlimitSound();
	//DestroyWndSubSettingBackupSound();
	DestroyWndSubSettingSPO2Module();
	DestroyWndSubSettingSPO2sensitivity();
	DestroyWndSubSettingSPO2averagingtime();
	DestroyWndSubSettingSPO2alarmdelay();
	DestroyWndSubSettingBTBforVT();
	

	DestroySubViewFlowSensor();
	DestroySubViewOxySensor();
	DestroySubViewDisplay();
	DestroySubViewLanguage();
	DestroySubViewDateTime();
	DestroySubViewVentilation();
	DestroySubViewPatient();
	DestroySubViewSystem();
	DestroySubViewService();
	DestroySubViewTools();
	DestroySubViewCO2();
	DestroySubViewSPO2();
	DestroySubViewVideo();
	DestroySubViewNetwork();

	if(getModel()->IsO2calFlag()
		&&	getModel()->getCONFIG()->GetCurMode()!=VM_NCPAP 
		&&	getModel()->getCONFIG()->GetCurMode()!=VM_DUOPAP
		&&	getModel()->getCONFIG()->GetCurMode()!=VM_THERAPIE)
		getModel()->DeleteO2calFlag();

	CMVView::OnDestroy();
}

/**********************************************************************************************//**
 * Executes the timer action
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	nIDEvent	The identifier event.
 **************************************************************************************************/

void CViewMenu::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==SETUPTIMER)
	{
		if(		true==getModel()->IsO2calFlag()
			&&	m_eCurView==MV_CALIBRATION)
		{
			KillTimer(SETUPTIMER);
		}
		else
		{
			KillTimer(SETUPTIMER);

			if(AfxGetApp())
			{
				AfxGetApp()->GetMainWnd()->SetFocus();
				AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_NUMERIC);
			}
		}
		
		
	}

	CWnd::OnTimer(nIDEvent);
}

/**********************************************************************************************//**
 * Sets view focus
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewMenu::SetViewFocus()
{
	if(m_bExit)
		return;
	
	if(m_viewFlag!=0)
	{
		switch(m_viewFlag)
		{
		case IDC_BTN_SETUP_LEAKCOMP:
			{
				if(m_pcWndSubLeakComp)
					m_pcWndSubLeakComp->SetFocus();
			}
			break;
		case  IDC_BTN_SETUP_SENSITIVITYMODE:
			{
				if(m_pcWndSubSPO2sensitivity)
					m_pcWndSubSPO2sensitivity->SetFocus();
			}
			break;
		case  IDC_BTN_SETUP_SPO2AVERAGING:
			{
				if(m_pcWndSubSPO2averagingtime)
					m_pcWndSubSPO2averagingtime->SetFocus();
			}
			break;
		case  IDC_BTN_SETUP_SPO2ALARMDELAY:
			{
				if(m_pcWndSubSPO2alarmdelay)
					m_pcWndSubSPO2alarmdelay->SetFocus();
			}
			break;
		case  IDC_BTN_SETUP_SPO2MODULE:
			{
				if(m_pcWndSubSPO2Module)
					m_pcWndSubSPO2Module->SetFocus();
			}
			break;
		case  IDC_BTN_SETUP_CO2MODULE:
			{
				if(m_pcWndSubCO2Module)
					m_pcWndSubCO2Module->SetFocus();
			}
			break;
		case  IDC_BTN_SETUP_CO2UNITS:
			{
				if(m_pcWndSubCO2Units)
					m_pcWndSubCO2Units->SetFocus();
			}
			break;
		case  IDC_BTN_SETUP_BAROPRESSURE:
			{
				if(m_pcWndSubBaroPressure)
					m_pcWndSubBaroPressure->SetFocus();
			}
			break;
		case  IDC_BTN_SETUP_AUTOONPUMP:
			{
				if(m_pcWndSubAutoOnPump)
					m_pcWndSubAutoOnPump->SetFocus();
			}
			break;
		case  IDC_BTN_SETUP_ETCO2AVG:
			{
				
			}
			break;
		case IDC_BTN_SCREENLOCKTIME:
			{
				if(m_pcWndSubScreenlocktime)
					m_pcWndSubScreenlocktime->SetFocus();
			}
			break;
		case IDC_BTN_SETUP_RANGE:
			{
				if(m_pcWndSubVentRange)
					m_pcWndSubVentRange->SetFocus();
			}
			break;
		case IDC_BTN_SETUP_FUNCITIME:
			{
				if(m_pcWndSubRatioItime)
					m_pcWndSubRatioItime->SetFocus();
			}
			break;
		case IDC_BTN_SETUP_MANBREATHTIME:
			{
				if(m_pcWndSubManbreathTime)
					m_pcWndSubManbreathTime->SetFocus();
			}
			break;
		case IDC_BTN_SETUP_VOLUMETRIG:
			{
				if(m_pcWndSubVolTrigger)
					m_pcWndSubVolTrigger->SetFocus();
			}
			break;
		case IDC_BTN_SETUP_PPSVABSOLUTE:
			{
				if(m_pcWndSubPpsvAbsolute)
					m_pcWndSubPpsvAbsolute->SetFocus();
			}
			break;
		case IDC_BTN_SETUP_BTBVT:
			{
				if(m_pcWndSubBTBforVT)
					m_pcWndSubBTBforVT->SetFocus();
			}
			break;
		case IDC_BTN_SETUP_TUBESET:
			{
				if(m_pcWndSubTubeset)
					m_pcWndSubTubeset->SetFocus();
			}
			break;
		case IDC_BTN_SETUP_PRESSUREUNIT:
			{
				if(m_pcWndSubPressureUnit)
					m_pcWndSubPressureUnit->SetFocus();
			}
			break;
		case IDC_BTN_SETUP_HFOMANBREATH:
			{
				if(m_pcWndSubHFOManBreath)
					m_pcWndSubHFOManBreath->SetFocus();
			}
			break;
		case IDC_BTN_SETUP_EFLOW:
			{
				if(m_pcWndSubEFlow)
					m_pcWndSubEFlow->SetFocus();
			}
			break;
		case IDC_BTN_SETUP_PATALARMDELAY:
			{
				if(m_pcWndSubPatAlarmDelay)
					m_pcWndSubPatAlarmDelay->SetFocus();
			}
			break;
		case IDC_BTN_SETUP_AUTOOXYCAL:
			{
				if(m_pcWndSubAutoOxyCal)
					m_pcWndSubAutoOxyCal->SetFocus();
			}
			break;
		default:
			{
				this->SetFocus();
			}
			break;
		}
	}
	else
	{
		switch(m_eCurView)
		{
		case MV_MENU:
			{
				if(m_pcSubViewMenu)
					m_pcSubViewMenu->SetFocus();
			}
			break;
		case MV_CALIBRATION:
			{
				//this->SetFocus();
				if(m_pcSubViewFlowSensor)
					m_pcSubViewFlowSensor->SetFocus();
			}
			break;
		case MV_DISPLAY:
			{
				if(m_pcSubViewDisplay)
					m_pcSubViewDisplay->SetFocus();
			}
			break;
		case MV_VENTILATION:
			{
				if(m_pcSubViewVentilation)
					m_pcSubViewVentilation->SetFocus();
			}
			break;
		case MV_PATIENTDATA:
			{
				if(m_pcSubViewPatient)
					m_pcSubViewPatient->SetFocus();
			}
			break;
		case MV_LANGUAGE:
			{
				if(m_pcSubViewLanguage)
					m_pcSubViewLanguage->SetFocus();
			}
			break;
		case MV_DATETIME:
			{
				if(m_pcSubViewDateTime)
					m_pcSubViewDateTime->SetFocus();
			}
			break;
		case MV_TOOLS:
			{
				if(m_pcSubViewTools)
					m_pcSubViewTools->SetFocus();
			}
			break;
		case MV_SYSTEM:
			{
				if(m_pcSubViewSystem)
					m_pcSubViewSystem->SetFocus();
			}
			break;
		case MV_SERVICEMODE:
			{
				if(m_pcSubViewService)
					m_pcSubViewService->SetFocus();
			}
			break;
		case MV_VIDEO:
			{
				if(m_pcSubViewVideo)
					m_pcSubViewVideo->SetFocus();
			}
			break;
		case MV_NETWORK:
			{
				if(m_pcSubViewNetwork)
					m_pcSubViewNetwork->SetFocus();
			}
			break;
		//case MV_HELP:
		//	{
		//		//if(m_pcWndLanguage)
		//		//	m_pcWndLanguage->SetFocus();
		//	}
		//	break;
		case MV_O2:
			{
				if(m_pcSubViewOxySensor)
					m_pcSubViewOxySensor->SetFocus();
			}
			break;
		case MV_CO2:
			{
				if(m_pcSubViewCO2)
					m_pcSubViewCO2->SetFocus();
			}
			break;
		case MV_SPO2:
			{
				if(m_pcSubViewSPO2)
					m_pcSubViewSPO2->SetFocus();
			}
			break;

		default:
			{
				this->SetFocus();
			}
			break;
		}
	}
	
	PostMessage(WM_SET_SETUPTIMER);

}

/**********************************************************************************************//**
 * Notifies an event
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param [in,out]	pEvent	If non-null, the event.
 **************************************************************************************************/

void CViewMenu::NotifyEvent(CMVEvent* pEvent)
{
	if(m_bExit)
		return;

	switch(pEvent->GetET())
	{
	case CMVEvent::ET_ALARM:
		{
			CMVEventAlarm* pUIEvent = (CMVEventAlarm*)pEvent;
			switch(pUIEvent->GetEventType())
			{
			case CMVEventAlarm::EV_ALARMSTATE_CHANGED:
				{
					if(getModel()->getALARMHANDLER()->getActiveAlarm()==AL_Sens_FLOW_SENSOR_NOTCONNECTED)
					{
						KillTimer(SETUPTIMER);
						if(m_pcSubViewFlowSensor)
						{
							m_pcSubViewFlowSensor->CheckFlowSensorState(true);
						}
					}
					else if(getModel()->getALARMHANDLER()->getActiveAlarm()==AL_Sens_FLOW_SENSOR_DEFECT)
					{
						KillTimer(SETUPTIMER);
						if(m_pcSubViewFlowSensor)
						{
							m_pcSubViewFlowSensor->CheckFlowSensorState(true);
						}
					}
					else if(getModel()->getALARMHANDLER()->getActiveAlarm()==AL_Sens_FLOW_SENSOR_CLEANING)
					{
						KillTimer(SETUPTIMER);
						if(m_pcSubViewFlowSensor)
						{
							m_pcSubViewFlowSensor->CheckFlowSensorState(true);
						}
					}
					else if(getModel()->getALARMHANDLER()->getActiveAlarm()==AL_Sens_O2_SENSOR_DEFECT)
					{
						KillTimer(SETUPTIMER);
						if(m_pcSubViewOxySensor)
						{
							m_pcSubViewOxySensor->CheckOxySensorState(true);
						}
					}
					else if(getModel()->getALARMHANDLER()->getActiveAlarm()==AL_Sens_O2_SENSOR_USED)
					{
						KillTimer(SETUPTIMER);
						if(m_pcSubViewOxySensor)
						{
							m_pcSubViewOxySensor->CheckOxySensorState(true);
						}
					}
					else if(getModel()->getALARMHANDLER()->getActiveAlarm()==AL_Sens_O2_VALUE_INCORRECT)
					{
						KillTimer(SETUPTIMER);
						if(m_pcSubViewOxySensor)
						{
							m_pcSubViewOxySensor->CheckOxySensorState(true);
						}
					}
					else if(getModel()->getALARMHANDLER()->getActiveAlarm()==AL_Sens_O2_NOT_CALIBRATED)
					{
						KillTimer(SETUPTIMER);
						if(m_pcSubViewOxySensor)
						{
							m_pcSubViewOxySensor->CheckOxySensorState(true);
						}
					}
					else if(getModel()->getALARMHANDLER()->getActiveAlarm()==AL_Sens_PRICO_FiO2outOfRange)
					{
						KillTimer(SETUPTIMER);
						if(m_pcSubViewOxySensor)
						{
							m_pcSubViewOxySensor->CheckOxySensorState(true);
						}
					}
					else
					{
						if(m_pcSubViewFlowSensor)
						{
							if(		m_pcSubViewFlowSensor->IsCalibrationActive()==false
								&&	getModel()->getDATAHANDLER()->GetOxyCalRunning()==false)
								PostMessage(WM_SET_SETUPTIMER);
						}
						else
						{
							if(	getModel()->getDATAHANDLER()->GetOxyCalRunning()==false)
								PostMessage(WM_SET_SETUPTIMER);
						}




						if(		m_pcSubViewFlowSensor 
							&& (m_pcSubViewFlowSensor->IsCalibrationActive()==false))
						{
							m_pcSubViewFlowSensor->CheckFlowSensorState(true);
						}

						if(m_pcSubViewOxySensor)
						{
							m_pcSubViewOxySensor->CheckOxySensorState(true,getModel()->getDATAHANDLER()->GetOxyCalRunning());
						}
					}
					if(m_pcSubViewFlowSensor)
						m_pcSubViewFlowSensor->PostMessage(WM_ALARMSTATE_CHANGED);
					if(m_pcSubViewOxySensor)
						m_pcSubViewOxySensor->PostMessage(WM_ALARMSTATE_CHANGED);
				}
				break;
			default:
				{

				}
				break;
			}
		}
		break;
	case CMVEvent::ET_CONTROL:
		{
			CMVEventControl* pCtrlEvent = (CMVEventControl*)pEvent;
			switch(pCtrlEvent->GetEventType())
			{
			case CMVEventControl::EV_CONTROL_SYSTEMSTATE_CHANGED://FLOW1
				{
					if(m_pcSubViewFlowSensor)
					{
						m_pcSubViewFlowSensor->CheckFlowSensorState(true);
					}
				}
				break;
			case CMVEventControl::EV_CONTROL_FLOWSENSORSTATE:
				{
					if(		getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF
						||	getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_MANOFF
						||	getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_AUTOOFF)
					{
						if(m_pcSubViewFlowSensor)
						{
							m_pcSubViewFlowSensor->SetCalState(FS_OFF,true);
						}
					}
					else
					{
						if(m_pcSubViewFlowSensor)
						{
							m_pcSubViewFlowSensor->CheckFlowSensorState(true);
						}
					}
					if(m_pcSubViewFlowSensor)
						m_pcSubViewFlowSensor->PostMessage(WM_ALARMSTATE_CHANGED);
					PostMessage(WM_SET_SETUPTIMER);
				}
				break;
			case CMVEventControl::EV_CONTROL_O2SENSORSTATE:
				{
					if(getModel()->getDATAHANDLER()->GetO2SensorState()==OXYSENS_OFF)
					{
						if(m_pcSubViewOxySensor)
						{
							m_pcSubViewOxySensor->SetCalState(OS_OFF,true);
						}
					}
					else
					{
						if(m_pcSubViewOxySensor)
						{
							m_pcSubViewOxySensor->CheckOxySensorState(true);
						}
					}
					PostMessage(WM_SET_SETUPTIMER);
				}
				break;
			case CMVEventControl::EV_CONTROL_O2CALSTART:
				{
					if(m_pcSubViewOxySensor)
					{
						m_pcSubViewOxySensor->SetCalRunning(OXYCAL_RUNNING);
						m_pcSubViewOxySensor->SetCalState(OS_CALIBRATING,true);
						m_pcSubViewOxySensor->EnableOnOFF(FALSE);
					}
					if(m_pcSubViewFlowSensor)
					{
						m_pcSubViewFlowSensor->ShowFlowBtns(true);
					}
					KillTimer(SETUPTIMER);
				}
				break;
			case CMVEventControl::EV_CONTROL_O2CALSTOP:
				{
					if(m_pcSubViewOxySensor)
					{
						m_pcSubViewOxySensor->UpdateCalTime();
						m_pcSubViewOxySensor->SetCalRunning(OXYCAL_OFF);
						m_pcSubViewOxySensor->CheckOxySensorState(true,getModel()->getDATAHANDLER()->GetOxyCalRunning());
						m_pcSubViewOxySensor->SetCalState(getModel()->getDATAHANDLER()->GetOxySensorCalState(),true);
						m_pcSubViewOxySensor->EnableOnOFF(TRUE);
					}

					if(m_pcSubViewFlowSensor)
						m_pcSubViewFlowSensor->ShowFlowBtns(false);
					PostMessage(WM_SET_SETUPTIMER);
				}
				break;
			default:
				{

				}
				break;
			}
		}
		break;
	case CMVEvent::ET_UIEVENT:
		{
			CMVEventUI* pUIEvent = (CMVEventUI*)pEvent;
			switch(pUIEvent->GetEventType())
			{
			case CMVEventUI::EV_RELOAD_CONFIG:
				{
					PostMessage(WM_SET_SETUPTIMER);
				}
				break;
			case CMVEventUI::EV_BN_MENUBUTTON_OPEN:
				{
					PostMessage(WM_SET_SETUPTIMER);

					OpenMenuSelect();
				}
				break;
			case CMVEventUI::EV_BN_MENUBUTTON_CLOSE:
				{
					ShowMenu(m_ePrevView);

					PostMessage(WM_SET_SETUPTIMER);
				}
				break;
			case CMVEventUI::EV_BN_MENU_MOVELEFT:
				{
					PostMessage(WM_SET_SETUPTIMER);
				}
				break;
			case CMVEventUI::EV_BN_MENU_MOVERIGHT:
				{
					PostMessage(WM_SET_SETUPTIMER);
				}
				break;
			default:
				break;
			}
		}
		break;
	}
}

/**********************************************************************************************//**
 * Opens menu select
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewMenu::OpenMenuSelect()
{
	theApp.WriteLog(_T("OMS-S"));

	if(m_bExit)
		return;

	ShowWndMenuMenu(true,getModel()->GetLanguageString(IDS_TXT_SELMENU),false,false);

	Sleep(0);
	try
	{
		DestroySubViewFlowSensor();
		DestroySubViewOxySensor();
		DestroySubViewLanguage();
		DestroySubViewDateTime();
		DestroySubViewVentilation();
		DestroySubViewPatient();
		DestroySubViewSystem();
		DestroySubViewService();
		DestroySubViewDisplay();
		DestroySubViewTools();
		DestroySubViewCO2();
		DestroySubViewSPO2();
		DestroySubViewVideo();
		DestroySubViewNetwork();


	}
	catch (...)
	{
		theApp.ReportException(_T("CViewMenu::OpenMenuSelect"));
	}

	Sleep(0);

	DestroySettingsView();

	getModel()->SetStateCalibrationView(false);

	CreateSubViewMenu();
	ShowSubViewMenu(true);


	if(m_eCurView!=MV_MENU)
		m_ePrevView=m_eCurView;
	m_eCurView=MV_MENU;

	this->ShowWindow(SW_SHOW);

	theApp.WriteLog(_T("OMS-E"));
	PostMessage(WM_SET_SETUPTIMER);

}

/**********************************************************************************************//**
 * Language changed
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewMenu::LanguageChanged()
{
	if(m_bExit)
		return;

	CStringW csMenuText=_T("");
	switch(m_eCurView)
	{
	case MV_MENU:
		{
			csMenuText=getModel()->GetLanguageString(IDS_TXT_SELMENU);
		}
		break;
	case MV_CALIBRATION:
		{
			csMenuText=getModel()->GetLanguageString(IDS_BTN_MNU_CAL);

		}
		break;
	case MV_DISPLAY:
		{
			csMenuText=getModel()->GetLanguageString(IDS_BTN_MNU_DIS);

		}
		break;
	case MV_VENTILATION:
		{
			csMenuText=getModel()->GetLanguageString(IDS_BTN_MNU_VENT);
			/*if(m_pcSubViewVentilation)
				m_pcSubViewVentilation->NotifyLanguageChanged();*/
		}
		break;
	case MV_PATIENTDATA:
		{
			csMenuText=getModel()->GetLanguageString(IDS_BTN_MNU_PAT);

		}
		break;
	case MV_LANGUAGE:
		{
			if(m_pcSubViewLanguage)
				m_pcSubViewLanguage->NotifyLanguageChanged();

			csMenuText=getModel()->GetLanguageString(IDS_BTN_MNU_LANG);
		}
		break;
	case MV_DATETIME:
		{
			csMenuText=getModel()->GetLanguageString(IDS_BTN_MNU_DATETIME);

		}
		break;
	case MV_TOOLS:
		{
			csMenuText=getModel()->GetLanguageString(IDS_BTN_MNU_TOOLS);

		}
		break;
	case MV_SYSTEM:
		{
			csMenuText=getModel()->GetLanguageString(IDS_BTN_MNU_INFO);

		}
		break;
	case MV_SERVICEMODE:
		{
			csMenuText=getModel()->GetLanguageString(IDS_BTN_MNU_SERVICE);

		}
		break;
	case MV_O2:
		{
			csMenuText=getModel()->GetLanguageString(IDS_BTN_MNU_O2);
		}
		break;
	case MV_CO2:
		{
			csMenuText=getModel()->GetLanguageString(IDS_BTN_MNU_CO2);

		}
		break;
	case MV_VIDEO:
		{
			csMenuText=getModel()->GetLanguageString(IDS_BTN_MNU_VIDEO);

		}
		break;
	case MV_NETWORK:
		{
			csMenuText=getModel()->GetLanguageString(IDS_BTN_MNU_PDMS);

		}
		break;
	/*case MV_HELP:
		{
			csMenuText=getModel()->GetLanguageString(IDS_BTN_MNU_HELP);
		}
		break;*/
	case MV_SPO2:
		{
			csMenuText=getModel()->GetLanguageString(IDS_BTN_MNU_SPO2);

		}
		break;

	}

	try
	{
		if(m_pWndMenuMenu)
		{
			m_pWndMenuMenu->SetMenuText(csMenuText);
			//m_pWndMenuMenu->NotifyLanguageChanged();
		}
	}
	catch (...)
	{
		theApp.ReportException(_T("CViewMenu::NotifyLanguageChanged"));
	}
	Invalidate();
	UpdateWindow();

	PostMessage(WM_SET_SETUPTIMER);
}

/**********************************************************************************************//**
 * Notifies the language changed
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewMenu::NotifyLanguageChanged()
{
	if(m_bExit)
		return;

	PostMessage(WM_LANGUAGE_CHANGED);
	
	
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

LRESULT CViewMenu::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
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
	case WM_BN_BACK:
		{
			if(!m_bExit)
			{
				if(m_eCurView==MV_CALIBRATION || m_eCurView==MV_MENU)
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_NUMERIC);
				}
				else if(m_viewFlag!=0)
				{
					CloseSettingsView(m_viewFlag);
				}
				else if(m_eCurView==MV_O2 || m_eCurView==MV_CO2 || m_eCurView==MV_SPO2)
				{
					Show(false);
				}
				else /*if(m_viewFlag==0 && m_eCurView!=0)*/
				{
					OpenMenuSelect();
				}
			}
			return 1;
		}
		break;
	case WM_OPEN_SETTING:
		{
			if(!m_bExit)
				OpenSettingsView((UINT) wParam);
			return 1;
		}
		break;
	case WM_CLOSE_SETTING:
		{
			/*if(!m_bExit)
				CloseSettingsView((UINT) wParam);*/
			if(!m_bExit)
				CloseSettingsView(m_viewFlag);
			return 1;
		}
		break;
	//case WM_FLOWCAL_START:
	//	{
	//		if(m_pcSubViewOxySensor && !m_bExit)
	//		{
	//			m_pcSubViewOxySensor->ShowOxyBtns(true);
	//		}
	//		KillTimer(SETUPTIMER);
	//		//PostMessage(WM_SET_SETUPTIMER);
	//		return 1;
	//	}
	//	break;
	//case WM_FLOWCAL_STOP:
	//	{
	//		if(m_pcSubViewOxySensor && !m_bExit)
	//		{
	//			m_pcSubViewOxySensor->ShowOxyBtns(false);
	//		}
	//		PostMessage(WM_SET_SETUPTIMER);
	//		return 1;
	//	}
	//	break;
	case WM_OXYCAL_START:
		{
			if(m_pcSubViewFlowSensor && !m_bExit)
			{
				m_pcSubViewFlowSensor->ShowFlowBtns(true);
			}
			KillTimer(SETUPTIMER);
			return 1;
		}
		break;
	case WM_OXYCAL_STOP:
		{
			if(m_pcSubViewFlowSensor && !m_bExit)
			{
				m_pcSubViewFlowSensor->ShowFlowBtns(false);
			}
			PostMessage(WM_SET_SETUPTIMER);
			return 1;
		}
		break;
	case WM_MENU_CALIBRATION:
		{
			//DestroySubViewMenu();
			ShowMenu(MV_CALIBRATION);
			return 1;
		}
		break;
	case WM_MENU_DISPLAY:
		{
			ShowMenu(MV_DISPLAY);
			return 1;
		}
		break;
	case WM_MENU_VENTILATION:
		{
			ShowMenu(MV_VENTILATION);
			return 1;
		}
		break;
	case WM_MENU_PATIENTDATA:
		{
			ShowMenu(MV_PATIENTDATA);
			return 1;
		}
		break;
	case WM_MENU_LANGUAGE:
		{
			ShowMenu(MV_LANGUAGE);
			return 1;
		}
		break;
	case WM_MENU_DATETIME:
		{
			ShowMenu(MV_DATETIME);
			return 1;
		}
		break;
	case WM_MENU_INFO:
		{
			ShowMenu(MV_SYSTEM);
			return 1;
		}
		break;
	case WM_MENU_SERVICE:
		{
			ShowMenu(MV_SERVICEMODE);
			return 1;
		}
		break;
	case WM_MENU_TOOLS:
		{
			ShowMenu(MV_TOOLS);
			return 1;
		}
		break;
	case WM_MENU_VIDEO:
		{
			ShowMenu(MV_VIDEO);
			return 1;
		}
		break;
	case WM_MENU_NETWORK:
		{
			ShowMenu(MV_NETWORK);
			return 1;
		}
		break;
	/*case WM_MENU_HELP:
		{
			ShowMenu(MV_HELP);
			return 1;
		}
		break;*/
	case WM_MENU_O2:
		{
			ShowMenu(MV_O2);
			return 1;
		}
		break;
	case WM_MENU_CO2:
		{
			ShowMenu(MV_CO2);
			return 1;
		}
		break;
	case WM_MENU_SPO2:
		{
			ShowMenu(MV_SPO2);
			return 1;
		}
		break;
	case WM_LANGUAGE_CHANGED:
		{
			LanguageChanged();
			return 1;
		}
		break;
	//case WM_PASSWORD_ASK:
	//	{
	//		SetTimer(SETUPTIMER, MENUMODECHANGE, NULL);

	//		DestroyWndMenuSetup();
	//		DestroyWndSysInfo();
	//		CreateWndPassword();
	//		ShowWndPassword(true);
	//		//m_pcWndPassword->SetFocus();
	//	}
	//	break;
	case WM_PASSWORD_OK:
		{
			KillTimer(SETUPTIMER);
			if(!m_bExit)
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SERVICEMODE);
			}
			return 1;
		}
		break;
	//case WM_PASSWORD_ERR:
	//	{
	//		//DestroyWndPassword();
	//		//if(CreateWndMenuSetup())
	//		//	ShowWndMenuSetup(true);
	//		//CreateWndSysInfo();
	//		//ShowWndSysInfo(true);
	//		////m_pcWndSysInfo->SetFocus();

	//		//SetTimer(SETUPTIMER, MENUMODECHANGE, NULL);
	//	}
	//	break;
	case WM_VIDEO_START:
		{
			getModel()->SetVideoRunning(true);
			m_bVideoRunning=true;
			KillTimer(SETUPTIMER);

			return 1;
		}
		break;
	case WM_VIDEO_STOP:
		{
			getModel()->SetVideoRunning(false);
			m_bVideoRunning=false;
			SetTimer(SETUPTIMER, MENUMODECHANGE, NULL);

			return 1;
		}
		break;
	case WM_SET_SETUPTIMER:
		{
			KillTimer(SETUPTIMER);

			if(!m_bVideoRunning)
			{
				if(getModel()->getALARMHANDLER()->isActiveAlarmType(AT_SENSOR)==false)
				{
					SetTimer(SETUPTIMER, MENUMODECHANGE, NULL);
				}
			}
			
			return 1;
		}
		break;
	case WM_KILL_SETUPTIMER:
		{
			KillTimer(SETUPTIMER);
			return 1;
		}
		break;
	case WM_SCREENLOCK:
		{
			PostMessage(WM_SET_SETUPTIMER);

			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_BN_SCREENLOCK);
			return 1;
		}
		break;
	default:
		break;
	}
	return CWnd::WindowProc(message, wParam, lParam);
}

/**********************************************************************************************//**
 * Stops a video
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewMenu::StopVideo()
{
	if(m_bVideoRunning)
	{
		if(m_pcSubViewVideo)
			m_pcSubViewVideo->StopVideo();
		getModel()->SetVideoRunning(false);
		m_bVideoRunning=false;
		SetTimer(SETUPTIMER, MENUMODECHANGE, NULL);
	}
}

/**********************************************************************************************//**
 * Opens settings view
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	viewFlag	The view flag.
 **************************************************************************************************/

void CViewMenu::OpenSettingsView(UINT viewFlag)
{
	if(m_bExit)
		return;

	CStringW csMenuText=_T("");

	switch(m_eCurView)
	{
	case MV_MENU:
		{
		}
		break;
	case MV_CALIBRATION:
		{
		}
		break;
	case MV_DISPLAY:
		{
			DestroySubViewDisplay();
			csMenuText=getModel()->GetLanguageString(IDS_BTN_MNU_DIS);
		}
		break;
	case MV_VENTILATION:
		{
			DestroySubViewVentilation();
			csMenuText=getModel()->GetLanguageString(IDS_BTN_MNU_VENT);
		}
		break;
	case MV_PATIENTDATA:
		{
			DestroySubViewPatient();
			csMenuText=getModel()->GetLanguageString(IDS_BTN_MNU_PAT);
		}
		break;
	case MV_LANGUAGE:
		{
			DestroySubViewLanguage();
			csMenuText=getModel()->GetLanguageString(IDS_BTN_MNU_LANG);
		}
		break;
	case MV_DATETIME:
		{
			DestroySubViewDateTime();
			csMenuText=getModel()->GetLanguageString(IDS_BTN_MNU_DATETIME);
		}
		break;
	case MV_TOOLS:
		{
			DestroySubViewTools();
			csMenuText=getModel()->GetLanguageString(IDS_BTN_MNU_TOOLS);
		}
		break;
	case MV_SYSTEM:
		{
			DestroySubViewSystem();
			csMenuText=getModel()->GetLanguageString(IDS_BTN_MNU_INFO);
		}
		break;
	case MV_SERVICEMODE:
		{
			DestroySubViewService();
			csMenuText=getModel()->GetLanguageString(IDS_BTN_MNU_SERVICE);
		}
		break;
	case MV_VIDEO:
		{
			DestroySubViewVideo();
			csMenuText=getModel()->GetLanguageString(IDS_BTN_MNU_VIDEO);
		}
		break;
	case MV_NETWORK:
		{
			DestroySubViewNetwork();
			csMenuText=getModel()->GetLanguageString(IDS_BTN_MNU_PDMS);
		}
		break;
	//case MV_HELP:
	//	{
	//		//DestroySubViewHelp();
	//	}
	//	break;
	case MV_O2:
		{
			DestroySubViewOxySensor();
			csMenuText=getModel()->GetLanguageString(IDS_BTN_MNU_O2);
		}
		break;
	case MV_CO2:
		{
			DestroySubViewCO2();
			csMenuText=getModel()->GetLanguageString(IDS_BTN_MNU_CO2);
		}
		break;
	case MV_SPO2:
		{
			DestroySubViewSPO2();
			csMenuText=getModel()->GetLanguageString(IDS_BTN_MNU_SPO2);
		}
		break;
	}


	m_viewFlag=viewFlag;
	switch(viewFlag)
	{
	case IDC_BTN_SETUP_LEAKCOMP:
		{
			DestroyWndSubSettingLeakComp();
			CreateWndSubSettingLeakComp(viewFlag);
			ShowWndSubSettingLeakComp(true);
		}
		break;
	case  IDC_BTN_SETUP_SENSITIVITYMODE:
		{
			DestroyWndSubSettingSPO2sensitivity();
			CreateWndSubSettingSPO2sensitivity(viewFlag);
			ShowWndSubSettingSPO2sensitivity(true);
			PostMessage(WM_SET_SETUPTIMER);
		}
		break;
	case  IDC_BTN_SETUP_SPO2AVERAGING:
		{
			DestroyWndSubSettingSPO2averagingtime();
			CreateWndSubSettingSPO2averagingtime(viewFlag);
			ShowWndSubSettingSPO2averagingtime(true);
			PostMessage(WM_SET_SETUPTIMER);
		}
		break;
	case  IDC_BTN_SETUP_SPO2ALARMDELAY:
		{
			DestroyWndSubSettingSPO2alarmdelay();
			CreateWndSubSettingSPO2alarmdelay(viewFlag);
			ShowWndSubSettingSPO2alarmdelay(true);
			PostMessage(WM_SET_SETUPTIMER);
		}
		break;
	case  IDC_BTN_SETUP_SPO2MODULE:
		{
			DestroyWndSubSettingSPO2Module();
			CreateWndSubSettingSPO2Module(viewFlag);
			ShowWndSubSettingSPO2Module(true);
			PostMessage(WM_SET_SETUPTIMER);
		}
		break;
	case  IDC_BTN_SETUP_CO2MODULE:
		{
			DestroyWndSubSettingCO2Module();
			CreateWndSubSettingCO2Module(viewFlag);
			ShowWndSubSettingCO2Module(true);
			PostMessage(WM_SET_SETUPTIMER);
		}
		break;
	case  IDC_BTN_SETUP_CO2UNITS:
		{
			DestroyWndSubSettingCO2Units();
			CreateWndSubSettingCO2Units(viewFlag);
			ShowWndSubSettingCO2Units(true);
			PostMessage(WM_SET_SETUPTIMER);
		}
		break;
	case  IDC_BTN_SETUP_BAROPRESSURE:
		{
			DestroyWndSubSettingBaroPressure();
			CreateWndSubSettingBaroPressure(viewFlag);
			ShowWndSubSettingBaroPressure(true);
			PostMessage(WM_SET_SETUPTIMER);
		}
		break;
	case  IDC_BTN_SETUP_AUTOONPUMP:
		{
			DestroyWndSubSettingAutoOnPump();
			CreateWndSubSettingAutoOnPump(viewFlag);
			ShowWndSubSettingAutoOnPump(true);
			PostMessage(WM_SET_SETUPTIMER);
		}
		break;
	case  IDC_BTN_SETUP_ETCO2AVG:
		{

		}
		break;
	case IDC_BTN_SCREENLOCKTIME:
		{
			DestroyWndSubSettingScreenlocktime();
			CreateWndSubSettingScreenlocktime(viewFlag);
			ShowWndSubSettingScreenlocktime(true);
			PostMessage(WM_SET_SETUPTIMER);
		}
		break;
	case IDC_BTN_SETUP_RANGE:
		{
			DestroyWndSubSettingVentRange();
			CreateWndSubSettingVentRange(viewFlag);
			ShowWndSubSettingVentRange(true);
			PostMessage(WM_SET_SETUPTIMER);
		}
		break;
	case IDC_BTN_SETUP_FUNCITIME:
		{
			DestroyWndSubSettingRatioItime();
			CreateWndSubSettingRatioItime(viewFlag);
			ShowWndSubSettingRatioItime(true);
			PostMessage(WM_SET_SETUPTIMER);
		}
		break;
	case IDC_BTN_SETUP_MANBREATHTIME:
		{
			DestroyWndSubSettingManbreathTime();
			CreateWndSubSettingManbreathTime(viewFlag);
			ShowWndSubSettingManbreathTime(true);
			PostMessage(WM_SET_SETUPTIMER);
		}
		break;
	case IDC_BTN_SETUP_PATALARMDELAY:
		{
			DestroyWndSubSettingSubPatAlarmDelay();
			CreateWndSubSettingSubPatAlarmDelay(viewFlag);
			ShowWndSubSettingSubPatAlarmDelay(true);
			PostMessage(WM_SET_SETUPTIMER);
		}
		break;
	case IDC_BTN_SETUP_VOLUMETRIG:
		{
			DestroyWndSubSettingVolTrigger();
			CreateWndSubSettingVolTrigger(viewFlag);
			ShowWndSubSettingVolTrigger(true);
			PostMessage(WM_SET_SETUPTIMER);
		}
		break;
	case IDC_BTN_SETUP_PPSVABSOLUTE:
		{
			DestroyWndSubSettingPpsvAbsolute();
			CreateWndSubSettingPpsvAbsolute(viewFlag);
			ShowWndSubSettingPpsvAbsolute(true);
			PostMessage(WM_SET_SETUPTIMER);
		}
		break;
	case IDC_BTN_SETUP_BTBVT:
		{
			DestroyWndSubSettingBTBforVT();
			CreateWndSubSettingBTBforVT(viewFlag);
			ShowWndSubSettingBTBforVT(true);
			PostMessage(WM_SET_SETUPTIMER);
		}
		break;
	case IDC_BTN_SETUP_TUBESET:
		{
			DestroyWndSubSettingTubeset();
			CreateWndSubSettingTubeset(viewFlag);
			ShowWndSubSettingTubeset(true);
			PostMessage(WM_SET_SETUPTIMER);
		}
		break;
	case IDC_BTN_SETUP_PRESSUREUNIT:
		{
			DestroyWndSubSettingPressureUnit();
			CreateWndSubSettingPressureUnit(viewFlag);
			ShowWndSubSettingPressureUnit(true);
			PostMessage(WM_SET_SETUPTIMER);
		}
		break;
	case IDC_BTN_SETUP_EFLOW:
		{
			DestroyWndSubSettingEFlow();
			CreateWndSubSettingEFlow(viewFlag);
			ShowWndSubSettingEFlow(true);
			PostMessage(WM_SET_SETUPTIMER);
		}
		break;
	case IDC_BTN_SETUP_AUTOOXYCAL:
		{
			DestroyWndSubSettingAutoOxyCal();
			CreateWndSubSettingAutoOxyCal(viewFlag);
			ShowWndSubSettingAutoOxyCal(true);
			PostMessage(WM_SET_SETUPTIMER);
		}
		break;
	case IDC_BTN_SETUP_HFOMANBREATH:
		{
			DestroyWndSubSettingHFOManBreath();
			CreateWndSubSettingHFOManBreath(viewFlag);
			ShowWndSubSettingHFOManBreath(true);
			PostMessage(WM_SET_SETUPTIMER);
		}
		break;
	}

	ShowWndMenuMenu(true, csMenuText, false, true);
	
}

/**********************************************************************************************//**
 * Closes settings view
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	viewFlag	The view flag.
 **************************************************************************************************/

void CViewMenu::CloseSettingsView(UINT viewFlag)
{
	if(m_bExit)
		return;

	switch(viewFlag)
	{
	case  IDC_BTN_SETUP_SENSITIVITYMODE:
		{
			DestroyWndSubSettingSPO2sensitivity();
		}
		break;
	case  IDC_BTN_SETUP_SPO2AVERAGING:
		{
			DestroyWndSubSettingSPO2averagingtime();
		}
		break;
	case  IDC_BTN_SETUP_SPO2ALARMDELAY:
		{
			DestroyWndSubSettingSPO2alarmdelay();
		}
		break;
	case  IDC_BTN_SETUP_SPO2MODULE:
		{
			DestroyWndSubSettingSPO2Module();
		}
		break;
	case  IDC_BTN_SETUP_CO2MODULE:
		{
			DestroyWndSubSettingCO2Module();
		}
		break;
	case  IDC_BTN_SETUP_CO2UNITS:
		{
			DestroyWndSubSettingCO2Units();
		}
		break;
	case  IDC_BTN_SETUP_BAROPRESSURE:
		{
			DestroyWndSubSettingBaroPressure();
		}
		break;
	case  IDC_BTN_SETUP_AUTOONPUMP:
		{
			DestroyWndSubSettingAutoOnPump();
		}
		break;
	case IDC_BTN_SCREENLOCKTIME:
		{
			DestroyWndSubSettingScreenlocktime();
		}
		break;
	case IDC_BTN_SETUP_RANGE:
		{
			DestroyWndSubSettingVentRange();
		}
		break;
	case IDC_BTN_SETUP_FUNCITIME:
		{
			DestroyWndSubSettingRatioItime();
		}
		break;
	case IDC_BTN_SETUP_MANBREATHTIME:
		{
			DestroyWndSubSettingManbreathTime();
		}
		break;
	case IDC_BTN_SETUP_LEAKCOMP:
		{
			DestroyWndSubSettingLeakComp();
		}
		break;
	case IDC_BTN_SETUP_PATALARMDELAY:
		{
			DestroyWndSubSettingSubPatAlarmDelay();
		}
		break;
	case IDC_BTN_SETUP_VOLUMETRIG:
		{
			DestroyWndSubSettingVolTrigger();
		}
		break;
	case IDC_BTN_SETUP_PPSVABSOLUTE:
		{
			DestroyWndSubSettingPpsvAbsolute();
		}
		break;
	case IDC_BTN_SETUP_BTBVT:
		{
			DestroyWndSubSettingBTBforVT();
		}
		break;
	case IDC_BTN_SETUP_TUBESET:
		{
			DestroyWndSubSettingTubeset();
		}
		break;
	case IDC_BTN_SETUP_PRESSUREUNIT:
		{
			DestroyWndSubSettingPressureUnit();
		}
		break;
	case IDC_BTN_SETUP_EFLOW:
		{
			DestroyWndSubSettingEFlow();
		}
		break;
	case IDC_BTN_SETUP_AUTOOXYCAL:
		{
			DestroyWndSubSettingAutoOxyCal();
		}
		break;
	case IDC_BTN_SETUP_HFOMANBREATH:
		{
			DestroyWndSubSettingHFOManBreath();
		}
		break;
	}
	m_viewFlag=0;
	ShowMenu(m_eCurView);

	PostMessage(WM_SET_SETUPTIMER);

}

/**********************************************************************************************//**
 * Destroys the settings view
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewMenu::DestroySettingsView()
{
	if(m_viewFlag==0)
		return;

	switch(m_viewFlag)
	{
	case IDC_BTN_SETUP_LEAKCOMP:
		{
			DestroyWndSubSettingLeakComp();
		}
		break;
	case  IDC_BTN_SETUP_SENSITIVITYMODE:
		{
			DestroyWndSubSettingSPO2sensitivity();
		}
		break;
	case  IDC_BTN_SETUP_SPO2AVERAGING:
		{
			DestroyWndSubSettingSPO2averagingtime();
		}
		break;
	case  IDC_BTN_SETUP_SPO2ALARMDELAY:
		{
			DestroyWndSubSettingSPO2alarmdelay();
		}
		break;
	case  IDC_BTN_SETUP_SPO2MODULE:
		{
			DestroyWndSubSettingSPO2Module();
		}
		break;
	case  IDC_BTN_SETUP_CO2MODULE:
		{
			DestroyWndSubSettingCO2Module();
		}
		break;
	case  IDC_BTN_SETUP_CO2UNITS:
		{
			DestroyWndSubSettingCO2Units();
		}
		break;
	case  IDC_BTN_SETUP_BAROPRESSURE:
		{
			DestroyWndSubSettingBaroPressure();
		}
		break;
	case  IDC_BTN_SETUP_AUTOONPUMP:
		{
			DestroyWndSubSettingAutoOnPump();
		}
		break;
	case IDC_BTN_SCREENLOCKTIME:
		{
			DestroyWndSubSettingScreenlocktime();
		}
		break;
	case IDC_BTN_SETUP_RANGE:
		{
			DestroyWndSubSettingVentRange();
		}
		break;
	case IDC_BTN_SETUP_FUNCITIME:
		{
			DestroyWndSubSettingRatioItime();
		}
		break;
	case IDC_BTN_SETUP_MANBREATHTIME:
		{
			DestroyWndSubSettingManbreathTime();
		}
		break;
	case IDC_BTN_SETUP_PATALARMDELAY:
		{
			DestroyWndSubSettingSubPatAlarmDelay();
		}
		break;
	case IDC_BTN_SETUP_VOLUMETRIG:
		{
			DestroyWndSubSettingVolTrigger();
		}
		break;
	case IDC_BTN_SETUP_PPSVABSOLUTE:
		{
			DestroyWndSubSettingPpsvAbsolute();
		}
		break;
	case IDC_BTN_SETUP_BTBVT:
		{
			DestroyWndSubSettingBTBforVT();
		}
		break;
	case IDC_BTN_SETUP_TUBESET:
		{
			DestroyWndSubSettingTubeset();
		}
		break;
	case IDC_BTN_SETUP_PRESSUREUNIT:
		{
			DestroyWndSubSettingPressureUnit();
		}
		break;
	case IDC_BTN_SETUP_EFLOW:
		{
			DestroyWndSubSettingEFlow();
		}
		break;
	case IDC_BTN_SETUP_AUTOOXYCAL:
		{
			DestroyWndSubSettingAutoOxyCal();
		}
		break;
	case IDC_BTN_SETUP_HFOMANBREATH:
		{
			DestroyWndSubSettingHFOManBreath();
		}
		break;
	}
	m_viewFlag=0;

	PostMessage(WM_SET_SETUPTIMER);
}

/**********************************************************************************************//**
 * Creates window menu
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::CreateWndMenuMenu()
{
	if(m_pWndMenuMenu==NULL && m_lX>-1)
	{
		m_pWndMenuMenu = new CWndMenuMenu(this);
		RECT rcLd={0,551,800,600};

		//RECT rcLd={477,0,640,435};
		if(m_pWndMenuMenu->Create(AfxGetApp()->GetMainWnd(),rcLd,IDC_VIEW_SETUPMENU))
		{
			m_pWndMenuMenu->Init();

			//m_pWndMenuMenu->

			return true;
		}
	}
	return false;
}

/**********************************************************************************************//**
 * Destroys the window menu
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::DestroyWndMenuMenu()
{
	if(m_pWndMenuMenu)
	{
		m_pWndMenuMenu->DestroyWindow();
		delete m_pWndMenuMenu;
		m_pWndMenuMenu=NULL;
	}
	return false;
}

/**********************************************************************************************//**
 * Shows the window menu
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	bShow		  	True to show, false to hide.
 * \param	txt			  	The text.
 * \param	bMenuBtnOpen  	True to menu button open.
 * \param	bMenuBtnReturn	True to menu button return.
 **************************************************************************************************/

void CViewMenu::ShowWndMenuMenu(bool bShow, CStringW txt, bool bMenuBtnOpen, bool bMenuBtnReturn)
{
	if(m_pWndMenuMenu)
	{
		m_pWndMenuMenu->Show(bShow);
		m_pWndMenuMenu->SetMenuText(txt);

		/*if(bMenuBtnOpen)
			m_pWndMenuMenu->PostMessage(WM_SETMENUBTN_OPEN);
		else if(!bMenuBtnReturn)
			m_pWndMenuMenu->PostMessage(WM_SETMENUBTN_CLOSE);
		else
			m_pWndMenuMenu->PostMessage(WM_SETMENUBTN_RETURN);*/
	}
}

/**********************************************************************************************//**
 * Creates sub view menu
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::CreateSubViewMenu()
{
	if(m_pcSubViewMenu==NULL)
	{
		m_pcSubViewMenu = new CSubViewMenuSelect();

		//RECT rcLd={0,0,564,150};
		RECT rcLd={0,0,800,510};
		if(m_pcSubViewMenu->CreateWnd(this,rcLd,IDC_VIEW_CAL_FLOW))
		{
			//m_pcSubViewMenu->Init();
			return true;
		}
	}
	return false;
}

/**********************************************************************************************//**
 * Destroys the sub view menu
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::DestroySubViewMenu()
{
	if(m_pcSubViewMenu)
	{
		m_pcSubViewMenu->DestroyWindow();
		delete m_pcSubViewMenu;
		m_pcSubViewMenu=NULL;
	}
	return false;
}

/**********************************************************************************************//**
 * Shows the sub view menu
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	bShow	True to show, false to hide.
 **************************************************************************************************/

void CViewMenu::ShowSubViewMenu(bool bShow)
{
	if(m_pcSubViewMenu)
	{
		m_pcSubViewMenu->Show(bShow,m_eCurView);
		m_pcSubViewMenu->SetFocus();
	}


}

/**********************************************************************************************//**
 * Creates sub view flow sensor
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::CreateSubViewFlowSensor()
{
	if(m_pcSubViewFlowSensor==NULL)
	{
		m_pcSubViewFlowSensor = new CSubViewCalFlowSensor();

		//RECT rcLd={0,0,800,255};
		RECT rcLd={0,0,800,510};
		if(m_pcSubViewFlowSensor->CreateWnd(this,rcLd,IDC_VIEW_CAL_FLOW))
		{
			m_pcSubViewFlowSensor->Init();
			return true;
		}
	}
	return false;
}

/**********************************************************************************************//**
 * Shows the sub view flow sensor
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	show	True to show, false to hide.
 **************************************************************************************************/

void CViewMenu::ShowSubViewFlowSensor(bool show)
{
	if(m_pcSubViewFlowSensor)
		m_pcSubViewFlowSensor->Show(show);


}

/**********************************************************************************************//**
 * Destroys the sub view flow sensor
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::DestroySubViewFlowSensor()
{
	if(m_pcSubViewFlowSensor)
	{
		m_pcSubViewFlowSensor->DestroyWindow();
		delete m_pcSubViewFlowSensor;
		m_pcSubViewFlowSensor=NULL;
	}
	return false;
}

/**********************************************************************************************//**
 * Creates sub view display
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::CreateSubViewDisplay()
{
	if(m_pcSubViewDisplay==NULL)
	{
		m_pcSubViewDisplay = new CSubViewDisplay();
		//RECT rcLd={m_lX,m_lY,m_lX+m_lCx,m_lY+m_lCy};

		RECT rcLd={0,0,800,510};
		//RECT rcLd={5,50,790,535};
		if(m_pcSubViewDisplay->CreateWnd(this,rcLd,IDC_VIEW_SETUP_DISPLAY))
		{
			return true;
		}
	}
	return false;
}

/**********************************************************************************************//**
 * Destroys the sub view display
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::DestroySubViewDisplay()
{
	if(m_pcSubViewDisplay)
	{
		m_pcSubViewDisplay->DestroyWindow();
		delete m_pcSubViewDisplay;
		m_pcSubViewDisplay=NULL;
	}
	return false;
}

/**********************************************************************************************//**
 * Shows the sub view display
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	show	True to show, false to hide.
 **************************************************************************************************/

void CViewMenu::ShowSubViewDisplay(bool show)
{
	if(m_pcSubViewDisplay)
	{
		m_pcSubViewDisplay->Show(show);

		m_pcSubViewDisplay->SetFocus();
	}

}

/**********************************************************************************************//**
 * Creates sub view language
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::CreateSubViewLanguage()
{
	if(m_pcSubViewLanguage==NULL)
	{
		m_pcSubViewLanguage = new CSubViewLanguage();
		//RECT rcLd={m_lX,m_lY,m_lX+m_lCx,m_lY+m_lCy};

		RECT rcLd={0,0,800,510};
		//RECT rcLd={5,50,790,535};
		if(m_pcSubViewLanguage->CreateWnd(this,rcLd,IDC_VIEW_SETUP_LANGUAGE))
		{
			return true;
		}
	}
	return false;
}

/**********************************************************************************************//**
 * Destroys the sub view language
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::DestroySubViewLanguage()
{
	if(m_pcSubViewLanguage)
	{
		m_pcSubViewLanguage->DestroyWindow();
		delete m_pcSubViewLanguage;
		m_pcSubViewLanguage=NULL;
	}
	return false;
}

/**********************************************************************************************//**
 * Shows the sub view language
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	show	True to show, false to hide.
 **************************************************************************************************/

void CViewMenu::ShowSubViewLanguage(bool show)
{
	if(m_pcSubViewLanguage)
	{
		m_pcSubViewLanguage->Show(show);

		m_pcSubViewLanguage->SetFocus();
	}

}

/**********************************************************************************************//**
 * Creates sub view date time
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::CreateSubViewDateTime()
{
	if(m_pcSubViewDateTime==NULL)
	{
		m_pcSubViewDateTime = new CSubViewDateTime();
		//RECT rcLd={m_lX,m_lY,m_lX+m_lCx,m_lY+m_lCy};

		RECT rcLd={0,0,800,510};
		//RECT rcLd={0,0,565,485};
		if(m_pcSubViewDateTime->CreateWnd(this,rcLd,IDC_VIEW_SETUP_DATETIME))
		{
			return true;
		}
	}
	return false;
}

/**********************************************************************************************//**
 * Destroys the sub view date time
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::DestroySubViewDateTime()
{
	if(m_pcSubViewDateTime)
	{
		m_pcSubViewDateTime->DestroyWindow();
		delete m_pcSubViewDateTime;
		m_pcSubViewDateTime=NULL;
	}
	return false;
}

/**********************************************************************************************//**
 * Shows the sub view date time
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	show	True to show, false to hide.
 **************************************************************************************************/

void CViewMenu::ShowSubViewDateTime(bool show)
{
	if(m_pcSubViewDateTime)
	{
		m_pcSubViewDateTime->Show(show);

		m_pcSubViewDateTime->SetFocus();
	}

}

/**********************************************************************************************//**
 * Creates sub view ventilation
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::CreateSubViewVentilation()
{
	if(m_pcSubViewVentilation==NULL)
	{
		m_pcSubViewVentilation = new CSubViewVentilation();
		//RECT rcLd={m_lX,m_lY,m_lX+m_lCx,m_lY+m_lCy};

		RECT rcLd={0,0,800,510};
		//RECT rcLd={0,0,565,485};
		if(m_pcSubViewVentilation->CreateWnd(this,rcLd,IDC_VIEW_SETUP_SETTINGS))
		{
			return true;
		}
	}
	return false;
}

/**********************************************************************************************//**
 * Destroys the sub view ventilation
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::DestroySubViewVentilation()
{
	if(m_pcSubViewVentilation)
	{
		m_pcSubViewVentilation->DestroyWindow();
		delete m_pcSubViewVentilation;
		m_pcSubViewVentilation=NULL;
	}
	return false;
}

/**********************************************************************************************//**
 * Shows the sub view ventilation
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	show	True to show, false to hide.
 **************************************************************************************************/

void CViewMenu::ShowSubViewVentilation(bool show)
{
	if(m_pcSubViewVentilation)
	{
		m_pcSubViewVentilation->Show(show);

		m_pcSubViewVentilation->SetFocus();
	}

}

/**********************************************************************************************//**
 * Creates sub view patient
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::CreateSubViewPatient()
{
	if(m_pcSubViewPatient==NULL)
	{
		m_pcSubViewPatient = new CSubViewPatient();
		//RECT rcLd={m_lX,m_lY,m_lX+m_lCx,m_lY+m_lCy};

		RECT rcLd={0,0,800,510};
		//RECT rcLd={0,0,565,485};
		if(m_pcSubViewPatient->CreateWnd(this,rcLd,IDC_VIEW_SETUP_PATIENT))
		{
			return true;
		}
	}
	return false;
}

/**********************************************************************************************//**
 * Destroys the sub view patient
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::DestroySubViewPatient()
{
	if(m_pcSubViewPatient)
	{
		m_pcSubViewPatient->DestroyWindow();
		delete m_pcSubViewPatient;
		m_pcSubViewPatient=NULL;
	}
	return false;
}

/**********************************************************************************************//**
 * Shows the sub view patient
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	show	True to show, false to hide.
 **************************************************************************************************/

void CViewMenu::ShowSubViewPatient(bool show)
{
	if(m_pcSubViewPatient)
	{
		m_pcSubViewPatient->Show(show);

		m_pcSubViewPatient->SetFocus();
	}

}

/**********************************************************************************************//**
 * Creates sub view system
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::CreateSubViewSystem()
{
	if(m_pcSubViewSystem==NULL)
	{
		m_pcSubViewSystem = new CSubViewSystem();
		//RECT rcLd={m_lX,m_lY,m_lX+m_lCx,m_lY+m_lCy};

		RECT rcLd={0,0,800,510};
		//RECT rcLd={0,0,565,485};
		if(m_pcSubViewSystem->CreateWnd(this,rcLd,IDC_VIEW_SETUP_SYSINFO))
		{
			return true;
		}
	}
	return false;
}

/**********************************************************************************************//**
 * Destroys the sub view system
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::DestroySubViewSystem()
{
	if(m_pcSubViewSystem)
	{
		m_pcSubViewSystem->DestroyWindow();
		delete m_pcSubViewSystem;
		m_pcSubViewSystem=NULL;
	}
	return false;
}

/**********************************************************************************************//**
 * Shows the sub view system
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	show	True to show, false to hide.
 **************************************************************************************************/

void CViewMenu::ShowSubViewSystem(bool show)
{
	if(m_pcSubViewSystem)
	{
		m_pcSubViewSystem->Show(show);

		m_pcSubViewSystem->SetFocus();
	}

}

/**********************************************************************************************//**
 * Creates sub view service
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::CreateSubViewService()
{
	if(m_pcSubViewService==NULL)
	{
		m_pcSubViewService = new CSubViewService();
		//RECT rcLd={m_lX,m_lY,m_lX+m_lCx,m_lY+m_lCy};

		//RECT rcLd={250,125,550,475};
		RECT rcLd={0,0,800,510};
		//RECT rcLd={0,0,565,485};
		if(m_pcSubViewService->CreateWnd(this,rcLd,IDC_VIEW_PASSWORD))
		{
			return true;
		}
	}
	return false;
}

/**********************************************************************************************//**
 * Destroys the sub view service
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::DestroySubViewService()
{
	if(m_pcSubViewService)
	{
		m_pcSubViewService->DestroyWindow();
		delete m_pcSubViewService;
		m_pcSubViewService=NULL;
	}
	return false;
}

/**********************************************************************************************//**
 * Shows the sub view service
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	show	True to show, false to hide.
 **************************************************************************************************/

void CViewMenu::ShowSubViewService(bool show)
{
	if(m_pcSubViewService)
	{
		m_pcSubViewService->Show(show);

		m_pcSubViewService->SetFocus();
	}

}

/**********************************************************************************************//**
 * Creates sub view tools
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::CreateSubViewTools()
{
	if(m_pcSubViewTools==NULL)
	{
		m_pcSubViewTools = new CSubViewTools();
		//RECT rcLd={m_lX,m_lY,m_lX+m_lCx,m_lY+m_lCy};

		//RECT rcLd={250,125,550,475};
		RECT rcLd={0,0,800,510};
		//RECT rcLd={0,0,565,485};
		if(m_pcSubViewTools->CreateWnd(this,rcLd,IDC_VIEW_SETUP_TOOLS))
		{
			return true;
		}
	}
	return false;
}

/**********************************************************************************************//**
 * Destroys the sub view tools
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::DestroySubViewTools()
{
	if(m_pcSubViewTools)
	{
		m_pcSubViewTools->DestroyWindow();
		delete m_pcSubViewTools;
		m_pcSubViewTools=NULL;
	}
	return false;
}

/**********************************************************************************************//**
 * Shows the sub view tools
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	show	True to show, false to hide.
 **************************************************************************************************/

void CViewMenu::ShowSubViewTools(bool show)
{
	if(m_pcSubViewTools)
	{
		m_pcSubViewTools->Show(show);

		m_pcSubViewTools->SetFocus();
	}

}

/**********************************************************************************************//**
 * Creates sub view video
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::CreateSubViewVideo()
{
	if(m_pcSubViewVideo==NULL)
	{
		m_pcSubViewVideo = new CSubViewVideo();
		//RECT rcLd={m_lX,m_lY,m_lX+m_lCx,m_lY+m_lCy};

		//RECT rcLd={250,125,550,475};
		RECT rcLd={0,0,800,510};
		//RECT rcLd={0,0,565,485};
		if(m_pcSubViewVideo->CreateWnd(this,rcLd,IDC_VIEW_SETUP_VIDEO))
		{
			return true;
		}
	}
	return false;
}

/**********************************************************************************************//**
 * Destroys the sub view video
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::DestroySubViewVideo()
{
	if(m_pcSubViewVideo)
	{
		m_pcSubViewVideo->DestroyWindow();
		delete m_pcSubViewVideo;
		m_pcSubViewVideo=NULL;
	}
	return false;
}

/**********************************************************************************************//**
 * Shows the sub view video
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	show	True to show, false to hide.
 **************************************************************************************************/

void CViewMenu::ShowSubViewVideo(bool show)
{
	if(m_pcSubViewVideo)
	{
		m_pcSubViewVideo->Show(show);

		m_pcSubViewVideo->SetFocus();
	}

}

/**********************************************************************************************//**
 * Creates sub view network
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::CreateSubViewNetwork()
{
	if(m_pcSubViewNetwork==NULL)
	{
		m_pcSubViewNetwork = new CSubViewNetwork();
		//RECT rcLd={m_lX,m_lY,m_lX+m_lCx,m_lY+m_lCy};

		//RECT rcLd={250,125,550,475};
		RECT rcLd={0,0,800,510};
		//RECT rcLd={0,0,565,485};
		if(m_pcSubViewNetwork->CreateWnd(this,rcLd,IDC_VIEW_SETUP_NETWORK))
		{
			return true;
		}
	}
	return false;
}

/**********************************************************************************************//**
 * Destroys the sub view network
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::DestroySubViewNetwork()
{
	if(m_pcSubViewNetwork)
	{
		m_pcSubViewNetwork->DestroyWindow();
		delete m_pcSubViewNetwork;
		m_pcSubViewNetwork=NULL;
	}
	return false;
}

/**********************************************************************************************//**
 * Shows the sub view network
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	show	True to show, false to hide.
 **************************************************************************************************/

void CViewMenu::ShowSubViewNetwork(bool show)
{
	if(m_pcSubViewNetwork)
	{
		m_pcSubViewNetwork->Show(show);

		m_pcSubViewNetwork->SetFocus();
	}

}

/**********************************************************************************************//**
 * Creates sub view oxy sensor
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::CreateSubViewOxySensor()
{
	if(m_pcSubViewOxySensor==NULL)
	{
		m_pcSubViewOxySensor = new CSubViewCalOxySensor();

		RECT rcLd={0,0,800,510};
		//RECT rcLd={0,240,800,510};
		if(m_pcSubViewOxySensor->CreateWnd(this,rcLd,IDC_VIEW_CAL_FLOW))
		{
			m_pcSubViewOxySensor->Init();
			return true;
		}
	}
	return false;
}

/**********************************************************************************************//**
 * Shows the sub view oxy sensor
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	show	True to show, false to hide.
 **************************************************************************************************/

void CViewMenu::ShowSubViewOxySensor(bool show)
{
	if(m_pcSubViewOxySensor)
		m_pcSubViewOxySensor->Show(show);
}

/**********************************************************************************************//**
 * Destroys the sub view oxy sensor
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::DestroySubViewOxySensor()
{
	if(m_pcSubViewOxySensor)
	{
		m_pcSubViewOxySensor->DestroyWindow();
		delete m_pcSubViewOxySensor;
		m_pcSubViewOxySensor=NULL;
	}
	return false;
}

/**********************************************************************************************//**
 * Creates sub view co 2
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::CreateSubViewCO2()
{
	if(m_pcSubViewCO2==NULL)
	{
		m_pcSubViewCO2 = new CSubViewCO2();
		//RECT rcLd={m_lX,m_lY,m_lX+m_lCx,m_lY+m_lCy};

		//RECT rcLd={250,125,550,475};
		RECT rcLd={0,0,800,510};
		//RECT rcLd={0,0,565,485};
		if(m_pcSubViewCO2->CreateWnd(this,rcLd,IDC_VIEW_SETUP_CO2))
		{
			return true;
		}
	}
	return false;
}

/**********************************************************************************************//**
 * Destroys the sub view co 2
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::DestroySubViewCO2()
{
	if(m_pcSubViewCO2)
	{
		m_pcSubViewCO2->DestroyWindow();
		delete m_pcSubViewCO2;
		m_pcSubViewCO2=NULL;
	}
	return false;
}

/**********************************************************************************************//**
 * Shows the sub view co 2
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	show	True to show, false to hide.
 **************************************************************************************************/

void CViewMenu::ShowSubViewCO2(bool show)
{
	if(m_pcSubViewCO2)
	{
		m_pcSubViewCO2->Show(show);

		m_pcSubViewCO2->SetFocus();
	}

}

/**********************************************************************************************//**
 * Creates sub view spo 2
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::CreateSubViewSPO2()
{
	if(m_pcSubViewSPO2==NULL)
	{
		m_pcSubViewSPO2 = new CSubViewSPO2();
		//RECT rcLd={m_lX,m_lY,m_lX+m_lCx,m_lY+m_lCy};

		//RECT rcLd={250,125,550,475};
		RECT rcLd={0,0,800,510};
		//RECT rcLd={0,0,565,485};
		if(m_pcSubViewSPO2->CreateWnd(this,rcLd,IDC_VIEW_SETUP_SPO2))
		{
			return true;
		}
	}
	return false;
}

/**********************************************************************************************//**
 * Destroys the sub view spo 2
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::DestroySubViewSPO2()
{
	if(m_pcSubViewSPO2)
	{
		m_pcSubViewSPO2->DestroyWindow();
		delete m_pcSubViewSPO2;
		m_pcSubViewSPO2=NULL;
	}
	return false;
}

/**********************************************************************************************//**
 * Shows the sub view spo 2
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	show	True to show, false to hide.
 **************************************************************************************************/

void CViewMenu::ShowSubViewSPO2(bool show)
{
	if(m_pcSubViewSPO2)
	{
		m_pcSubViewSPO2->Show(show);

		m_pcSubViewSPO2->SetFocus();
	}

}

/**********************************************************************************************//**
 * Executes the erase bkgnd action
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param [in,out]	pDC	If non-null, the device-context.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

BOOL CViewMenu::OnEraseBkgnd(CDC* pDC)
{
	CBrush cbrBack(BACKGND);
	//CBrush cbrBack(RGB(255,255,255));
	CRect rc;
	GetClientRect(rc);

	pDC->FillRect(&rc, &cbrBack);

	//DeleteObject(cbrBack);


	return TRUE;

	//return CMVView::OnEraseBkgnd(pDC);
}

/**********************************************************************************************//**
 * Creates window sub setting screenlocktime
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	viewFlag	The view flag.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::CreateWndSubSettingScreenlocktime(UINT viewFlag)
{
	if(m_pcWndSubScreenlocktime==NULL)
	{
		m_pcWndSubScreenlocktime = new CWndSubSettingsScreenlocktime(viewFlag);
		//RECT rcLd={m_lX,m_lY,m_lX+m_lCx,m_lY+m_lCy};

		RECT rcLd={0,0,800,510};
		//RECT rcLd={0,0,565,485};
		if(m_pcWndSubScreenlocktime->CreateWnd(this,rcLd,IDC_VIEW_SETUP_SCREENLOCKTIME))
		{
			return true;
		}
	}
	return false;
}

/**********************************************************************************************//**
 * Destroys the window sub setting screenlocktime
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::DestroyWndSubSettingScreenlocktime()
{
	if(m_pcWndSubScreenlocktime)
	{
		m_pcWndSubScreenlocktime->DestroyWindow();
		delete m_pcWndSubScreenlocktime;
		m_pcWndSubScreenlocktime=NULL;
	}
	return false;
}

/**********************************************************************************************//**
 * Shows the window sub setting screenlocktime
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	show	True to show, false to hide.
 **************************************************************************************************/

void CViewMenu::ShowWndSubSettingScreenlocktime(bool show)
{
	if(m_pcWndSubScreenlocktime)
	{
		m_pcWndSubScreenlocktime->Show(show);
		m_pcWndSubScreenlocktime->SetFocus();
	}
}

/**********************************************************************************************//**
 * Destroys the window sub setting vent range
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::DestroyWndSubSettingVentRange()
{
	if(m_pcWndSubVentRange)
	{
		m_pcWndSubVentRange->DestroyWindow();
		delete m_pcWndSubVentRange;
		m_pcWndSubVentRange=NULL;
	}
	return false;
}

/**********************************************************************************************//**
 * Creates window sub setting vent range
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	viewFlag	The view flag.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::CreateWndSubSettingVentRange(UINT viewFlag)
{
	if(m_pcWndSubVentRange==NULL)
	{
		m_pcWndSubVentRange = new CWndSubSettingsVentRange(viewFlag);
		//RECT rcLd={m_lX,m_lY,m_lX+m_lCx,m_lY+m_lCy};

		RECT rcLd={0,0,800,510};
		//RECT rcLd={0,0,565,485};
		if(m_pcWndSubVentRange->CreateWnd(this,rcLd,IDC_VIEW_SETUP_VENTRANGE))
		{
			return true;
		}
	}
	return false;
}

/**********************************************************************************************//**
 * Shows the window sub setting vent range
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	show	True to show, false to hide.
 **************************************************************************************************/

void CViewMenu::ShowWndSubSettingVentRange(bool show)
{
	if(m_pcWndSubVentRange)
	{
		m_pcWndSubVentRange->Show(show);
		m_pcWndSubVentRange->SetFocus();
	}
}

/**********************************************************************************************//**
 * Destroys the window sub setting ratio itime
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::DestroyWndSubSettingRatioItime()
{
	if(m_pcWndSubRatioItime)
	{
		m_pcWndSubRatioItime->DestroyWindow();
		delete m_pcWndSubRatioItime;
		m_pcWndSubRatioItime=NULL;
	}
	return false;
}

/**********************************************************************************************//**
 * Creates window sub setting ratio itime
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	viewFlag	The view flag.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::CreateWndSubSettingRatioItime(UINT viewFlag)
{
	if(m_pcWndSubRatioItime==NULL)
	{
		m_pcWndSubRatioItime = new CWndSubSettingsRatioItime(viewFlag);
		//RECT rcLd={m_lX,m_lY,m_lX+m_lCx,m_lY+m_lCy};

		RECT rcLd={0,0,800,510};
		//RECT rcLd={0,0,565,485};
		if(m_pcWndSubRatioItime->CreateWnd(this,rcLd,IDC_VIEW_SETUP_RATIOITIME))
		{
			return true;
		}
	}
	return false;
}

/**********************************************************************************************//**
 * Shows the window sub setting ratio itime
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	show	True to show, false to hide.
 **************************************************************************************************/

void CViewMenu::ShowWndSubSettingRatioItime(bool show)
{
	if(m_pcWndSubRatioItime)
	{
		m_pcWndSubRatioItime->Show(show);
		m_pcWndSubRatioItime->SetFocus();
	}
}

/**********************************************************************************************//**
 * Destroys the window sub setting manbreath time
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::DestroyWndSubSettingManbreathTime()
{
	if(m_pcWndSubManbreathTime)
	{
		m_pcWndSubManbreathTime->DestroyWindow();
		delete m_pcWndSubManbreathTime;
		m_pcWndSubManbreathTime=NULL;
	}
	return false;
}

/**********************************************************************************************//**
 * Creates window sub setting manbreath time
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	viewFlag	The view flag.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::CreateWndSubSettingManbreathTime(UINT viewFlag)
{
	if(m_pcWndSubManbreathTime==NULL)
	{
		m_pcWndSubManbreathTime = new CWndSubSettingsManbreathTime(viewFlag);
		//RECT m_pcWndSubManbreathTime={m_lX,m_lY,m_lX+m_lCx,m_lY+m_lCy};

		RECT rcLd={0,0,800,510};
		//RECT rcLd={0,0,565,485};
		if(m_pcWndSubManbreathTime->CreateWnd(this,rcLd,IDC_VIEW_SETUP_MANBREATHTIME))
		{
			return true;
		}
	}
	return false;
}

/**********************************************************************************************//**
 * Shows the window sub setting manbreath time
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	show	True to show, false to hide.
 **************************************************************************************************/

void CViewMenu::ShowWndSubSettingManbreathTime(bool show)
{
	if(m_pcWndSubManbreathTime)
	{
		m_pcWndSubManbreathTime->Show(show);
		m_pcWndSubManbreathTime->SetFocus();
	}
}

/**********************************************************************************************//**
 * Destroys the window sub setting sub pattern alarm delay
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::DestroyWndSubSettingSubPatAlarmDelay()
{
	if(m_pcWndSubPatAlarmDelay)
	{
		m_pcWndSubPatAlarmDelay->DestroyWindow();
		delete m_pcWndSubPatAlarmDelay;
		m_pcWndSubPatAlarmDelay=NULL;
	}
	return false;
}

/**********************************************************************************************//**
 * Creates window sub setting sub pattern alarm delay
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	viewFlag	The view flag.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::CreateWndSubSettingSubPatAlarmDelay(UINT viewFlag)
{
	if(m_pcWndSubPatAlarmDelay==NULL)
	{
		m_pcWndSubPatAlarmDelay = new CWndSubSettingsPatAlarmDelay(viewFlag);
		//RECT m_pcWndSubManbreathTime={m_lX,m_lY,m_lX+m_lCx,m_lY+m_lCy};

		RECT rcLd={0,0,800,510};
		if(m_pcWndSubPatAlarmDelay->CreateWnd(this,rcLd,IDC_VIEW_SETUP_PATALARMDELAY))
		{
			return true;
		}
	}
	return false;
}

/**********************************************************************************************//**
 * Shows the window sub setting sub pattern alarm delay
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	show	True to show, false to hide.
 **************************************************************************************************/

void CViewMenu::ShowWndSubSettingSubPatAlarmDelay(bool show)
{
	if(m_pcWndSubPatAlarmDelay)
	{
		m_pcWndSubPatAlarmDelay->Show(show);
		m_pcWndSubPatAlarmDelay->SetFocus();
	}
}

/**********************************************************************************************//**
 * Destroys the window sub setting ppsv absolute
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::DestroyWndSubSettingPpsvAbsolute()
{
	if(m_pcWndSubPpsvAbsolute)
	{
		m_pcWndSubPpsvAbsolute->DestroyWindow();
		delete m_pcWndSubPpsvAbsolute;
		m_pcWndSubPpsvAbsolute=NULL;
	}
	return false;
}

/**********************************************************************************************//**
 * Creates window sub setting ppsv absolute
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	viewFlag	The view flag.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::CreateWndSubSettingPpsvAbsolute(UINT viewFlag)
{
	if(m_pcWndSubPpsvAbsolute==NULL)
	{
		m_pcWndSubPpsvAbsolute = new CWndSubSettingsPpsvAbsolute(viewFlag);
		//RECT rcLd={m_lX,m_lY,m_lX+m_lCx,m_lY+m_lCy};

		RECT rcLd={0,0,800,510};
		//RECT rcLd={0,0,565,485};
		if(m_pcWndSubPpsvAbsolute->CreateWnd(this,rcLd,IDC_VIEW_SETUP_PSVABSOLUTE))
		{
			return true;
		}
	}
	return false;
}

/**********************************************************************************************//**
 * Shows the window sub setting ppsv absolute
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	show	True to show, false to hide.
 **************************************************************************************************/

void CViewMenu::ShowWndSubSettingPpsvAbsolute(bool show)
{
	if(m_pcWndSubPpsvAbsolute)
	{
		m_pcWndSubPpsvAbsolute->Show(show);
		m_pcWndSubPpsvAbsolute->SetFocus();
	}
}

/**********************************************************************************************//**
 * Destroys the window sub setting bt bfor vt
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::DestroyWndSubSettingBTBforVT()
{
	if(m_pcWndSubBTBforVT)
	{
		m_pcWndSubBTBforVT->DestroyWindow();
		delete m_pcWndSubBTBforVT;
		m_pcWndSubBTBforVT=NULL;
	}
	return false;
}

/**********************************************************************************************//**
 * Creates window sub setting bt bfor vt
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	viewFlag	The view flag.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::CreateWndSubSettingBTBforVT(UINT viewFlag)
{
	if(m_pcWndSubBTBforVT==NULL)
	{
		m_pcWndSubBTBforVT = new CWndSubSettingsBTBforVT(viewFlag);
		//RECT rcLd={m_lX,m_lY,m_lX+m_lCx,m_lY+m_lCy};

		RECT rcLd={0,0,800,510};
		//RECT rcLd={0,0,565,485};
		if(m_pcWndSubBTBforVT->CreateWnd(this,rcLd,IDC_VIEW_SETUP_BTBVT))
		{
			return true;
		}
	}
	return false;
}

/**********************************************************************************************//**
 * Shows the window sub setting bt bfor vt
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	show	True to show, false to hide.
 **************************************************************************************************/

void CViewMenu::ShowWndSubSettingBTBforVT(bool show)
{
	if(m_pcWndSubBTBforVT)
	{
		m_pcWndSubBTBforVT->Show(show);
		m_pcWndSubBTBforVT->SetFocus();
	}
}

/**********************************************************************************************//**
 * Destroys the window sub setting volume trigger
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::DestroyWndSubSettingVolTrigger()
{
	if(m_pcWndSubVolTrigger)
	{
		m_pcWndSubVolTrigger->DestroyWindow();
		delete m_pcWndSubVolTrigger;
		m_pcWndSubVolTrigger=NULL;
	}
	return false;
}

/**********************************************************************************************//**
 * Creates window sub setting volume trigger
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	viewFlag	The view flag.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::CreateWndSubSettingVolTrigger(UINT viewFlag)
{
	if(m_pcWndSubVolTrigger==NULL)
	{
		m_pcWndSubVolTrigger = new CWndSubSettingsVolTrigger(viewFlag);
		//RECT rcLd={m_lX,m_lY,m_lX+m_lCx,m_lY+m_lCy};

		RECT rcLd={0,0,800,510};
		//RECT rcLd={0,0,565,485};
		if(m_pcWndSubVolTrigger->CreateWnd(this,rcLd,IDC_VIEW_SETUP_VOLTRIGGER))
		{
			return true;
		}
	}
	return false;
}

/**********************************************************************************************//**
 * Shows the window sub setting volume trigger
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	show	True to show, false to hide.
 **************************************************************************************************/

void CViewMenu::ShowWndSubSettingVolTrigger(bool show)
{
	if(m_pcWndSubVolTrigger)
	{
		m_pcWndSubVolTrigger->Show(show);
		m_pcWndSubVolTrigger->SetFocus();
	}
}

/**********************************************************************************************//**
 * Destroys the window sub setting tubeset
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::DestroyWndSubSettingTubeset()
{
	if(m_pcWndSubTubeset)
	{
		m_pcWndSubTubeset->DestroyWindow();
		delete m_pcWndSubTubeset;
		m_pcWndSubTubeset=NULL;
	}
	return false;
}

/**********************************************************************************************//**
 * Creates window sub setting tubeset
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	viewFlag	The view flag.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::CreateWndSubSettingTubeset(UINT viewFlag)
{
	if(m_pcWndSubTubeset==NULL)
	{
		m_pcWndSubTubeset = new CWndSubSettingsTubeset(viewFlag);
		//RECT rcLd={m_lX,m_lY,m_lX+m_lCx,m_lY+m_lCy};

		RECT rcLd={0,0,800,510};
		//RECT rcLd={0,0,565,485};
		if(m_pcWndSubTubeset->CreateWnd(this,rcLd,IDC_VIEW_SETUP_TUBESET))
		{
			return true;
		}
	}
	return false;
}

/**********************************************************************************************//**
 * Shows the window sub setting tubeset
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	show	True to show, false to hide.
 **************************************************************************************************/

void CViewMenu::ShowWndSubSettingTubeset(bool show)
{
	if(m_pcWndSubTubeset)
	{
		m_pcWndSubTubeset->Show(show);
		m_pcWndSubTubeset->SetFocus();
	}
}

/**********************************************************************************************//**
 * Destroys the window sub setting e flow
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::DestroyWndSubSettingEFlow()
{
	if(m_pcWndSubEFlow)
	{
		m_pcWndSubEFlow->DestroyWindow();
		delete m_pcWndSubEFlow;
		m_pcWndSubEFlow=NULL;
	}
	return false;
}

/**********************************************************************************************//**
 * Creates window sub setting e flow
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	viewFlag	The view flag.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::CreateWndSubSettingEFlow(UINT viewFlag)
{
	if(m_pcWndSubEFlow==NULL)
	{
		m_pcWndSubEFlow = new CWndSubSettingsEFlow(viewFlag);
		//RECT rcLd={m_lX,m_lY,m_lX+m_lCx,m_lY+m_lCy};

		RECT rcLd={0,0,800,510};
		//RECT rcLd={0,0,565,485};
		if(m_pcWndSubEFlow->CreateWnd(this,rcLd,IDC_VIEW_SETUP_EFLOW))
		{
			return true;
		}
	}
	return false;
}

/**********************************************************************************************//**
 * Shows the window sub setting e flow
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	show	True to show, false to hide.
 **************************************************************************************************/

void CViewMenu::ShowWndSubSettingEFlow(bool show)
{
	if(m_pcWndSubEFlow)
	{
		m_pcWndSubEFlow->Show(show);
		m_pcWndSubEFlow->SetFocus();
	}
}

/**********************************************************************************************//**
 * Destroys the window sub setting automatic oxy calendar
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::DestroyWndSubSettingAutoOxyCal()
{
	if(m_pcWndSubAutoOxyCal)
	{
		m_pcWndSubAutoOxyCal->DestroyWindow();
		delete m_pcWndSubAutoOxyCal;
		m_pcWndSubAutoOxyCal=NULL;
	}
	return false;
}

/**********************************************************************************************//**
 * Creates window sub setting automatic oxy calendar
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	viewFlag	The view flag.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::CreateWndSubSettingAutoOxyCal(UINT viewFlag)
{
	if(m_pcWndSubAutoOxyCal==NULL)
	{
		m_pcWndSubAutoOxyCal = new CWndSubSettingsAutoOxyCal(viewFlag);
		//RECT rcLd={m_lX,m_lY,m_lX+m_lCx,m_lY+m_lCy};

		RECT rcLd={0,0,800,510};
		//RECT rcLd={0,0,565,485};
		if(m_pcWndSubAutoOxyCal->CreateWnd(this,rcLd,IDC_VIEW_SETUP_AUTOOXYCAL))
		{
			return true;
		}
	}
	return false;
}

/**********************************************************************************************//**
 * Shows the window sub setting automatic oxy calendar
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	show	True to show, false to hide.
 **************************************************************************************************/

void CViewMenu::ShowWndSubSettingAutoOxyCal(bool show)
{
	if(m_pcWndSubAutoOxyCal)
	{
		m_pcWndSubAutoOxyCal->Show(show);
		m_pcWndSubAutoOxyCal->SetFocus();
	}
}

/**********************************************************************************************//**
 * Destroys the window sub setting hfo manager breath
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::DestroyWndSubSettingHFOManBreath()
{
	if(m_pcWndSubHFOManBreath)
	{
		m_pcWndSubHFOManBreath->DestroyWindow();
		delete m_pcWndSubHFOManBreath;
		m_pcWndSubHFOManBreath=NULL;
	}
	return false;
}

/**********************************************************************************************//**
 * Creates window sub setting hfo manager breath
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	viewFlag	The view flag.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::CreateWndSubSettingHFOManBreath(UINT viewFlag)
{
	if(m_pcWndSubHFOManBreath==NULL)
	{
		m_pcWndSubHFOManBreath = new CWndSubSettingsHFOManBreath(viewFlag);
		//RECT rcLd={m_lX,m_lY,m_lX+m_lCx,m_lY+m_lCy};

		RECT rcLd={0,0,800,510};
		//RECT rcLd={0,0,565,485};
		if(m_pcWndSubHFOManBreath->CreateWnd(this,rcLd,IDC_VIEW_SETUP_HFOMANBREATH))
		{
			return true;
		}
	}
	return false;
}

/**********************************************************************************************//**
 * Shows the window sub setting hfo manager breath
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	show	True to show, false to hide.
 **************************************************************************************************/

void CViewMenu::ShowWndSubSettingHFOManBreath(bool show)
{
	if(m_pcWndSubHFOManBreath)
	{
		m_pcWndSubHFOManBreath->Show(show);
		m_pcWndSubHFOManBreath->SetFocus();
	}
}

/**********************************************************************************************//**
 * Destroys the window sub setting spo 2sensitivity
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::DestroyWndSubSettingSPO2sensitivity()
{
	if(m_pcWndSubSPO2sensitivity)
	{
		m_pcWndSubSPO2sensitivity->DestroyWindow();
		delete m_pcWndSubSPO2sensitivity;
		m_pcWndSubSPO2sensitivity=NULL;
	}
	return false;
}

/**********************************************************************************************//**
 * Creates window sub setting spo 2sensitivity
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	viewFlag	The view flag.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::CreateWndSubSettingSPO2sensitivity(UINT viewFlag)
{
	if(m_pcWndSubSPO2sensitivity==NULL)
	{
		m_pcWndSubSPO2sensitivity = new CWndSubSettingsSPO2sensitivity(viewFlag);
		//RECT rcLd={m_lX,m_lY,m_lX+m_lCx,m_lY+m_lCy};

		RECT rcLd={0,0,800,510};
		//RECT rcLd={0,0,565,485};
		if(m_pcWndSubSPO2sensitivity->CreateWnd(this,rcLd,IDC_VIEW_SETUP_SPO2SENSITIVITY))
		{
			return true;
		}
	}
	return false;
}

/**********************************************************************************************//**
 * Shows the window sub setting spo 2sensitivity
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	show	True to show, false to hide.
 **************************************************************************************************/

void CViewMenu::ShowWndSubSettingSPO2sensitivity(bool show)
{
	if(m_pcWndSubSPO2sensitivity)
	{
		m_pcWndSubSPO2sensitivity->Show(show);
		m_pcWndSubSPO2sensitivity->SetFocus();
	}
}

/**********************************************************************************************//**
 * Destroys the window sub setting spo 2averagingtime
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::DestroyWndSubSettingSPO2averagingtime()
{
	if(m_pcWndSubSPO2averagingtime)
	{
		m_pcWndSubSPO2averagingtime->DestroyWindow();
		delete m_pcWndSubSPO2averagingtime;
		m_pcWndSubSPO2averagingtime=NULL;
	}
	return false;
}

/**********************************************************************************************//**
 * Creates window sub setting spo 2averagingtime
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	viewFlag	The view flag.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::CreateWndSubSettingSPO2averagingtime(UINT viewFlag)
{
	if(m_pcWndSubSPO2averagingtime==NULL)
	{
		m_pcWndSubSPO2averagingtime = new CWndSubSettingsSPO2averagingtime(viewFlag);
		//RECT rcLd={m_lX,m_lY,m_lX+m_lCx,m_lY+m_lCy};

		RECT rcLd={0,0,800,510};
		//RECT rcLd={0,0,565,485};
		if(m_pcWndSubSPO2averagingtime->CreateWnd(this,rcLd,IDC_VIEW_SETUP_SPO2AVERAGING))
		{
			return true;
		}
	}
	return false;
}

/**********************************************************************************************//**
 * Shows the window sub setting spo 2averagingtime
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	show	True to show, false to hide.
 **************************************************************************************************/

void CViewMenu::ShowWndSubSettingSPO2averagingtime(bool show)
{
	if(m_pcWndSubSPO2averagingtime)
	{
		m_pcWndSubSPO2averagingtime->Show(show);
		m_pcWndSubSPO2averagingtime->SetFocus();
	}
}

/**********************************************************************************************//**
 * Destroys the window sub setting spo 2alarmdelay
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::DestroyWndSubSettingSPO2alarmdelay()
{
	if(m_pcWndSubSPO2alarmdelay)
	{
		m_pcWndSubSPO2alarmdelay->DestroyWindow();
		delete m_pcWndSubSPO2alarmdelay;
		m_pcWndSubSPO2alarmdelay=NULL;
	}
	return false;
}

/**********************************************************************************************//**
 * Creates window sub setting spo 2alarmdelay
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	viewFlag	The view flag.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::CreateWndSubSettingSPO2alarmdelay(UINT viewFlag)
{
	if(m_pcWndSubSPO2alarmdelay==NULL)
	{
		m_pcWndSubSPO2alarmdelay = new CWndSubSettingsSPO2alarmdelay(viewFlag);
		//RECT rcLd={m_lX,m_lY,m_lX+m_lCx,m_lY+m_lCy};

		RECT rcLd={0,0,800,510};
		//RECT rcLd={0,0,565,485};
		if(m_pcWndSubSPO2alarmdelay->CreateWnd(this,rcLd,IDC_VIEW_SETUP_SPO2ALARMDELAY))
		{
			return true;
		}
	}
	return false;
}

/**********************************************************************************************//**
 * Shows the window sub setting spo 2alarmdelay
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	show	True to show, false to hide.
 **************************************************************************************************/

void CViewMenu::ShowWndSubSettingSPO2alarmdelay(bool show)
{
	if(m_pcWndSubSPO2alarmdelay)
	{
		m_pcWndSubSPO2alarmdelay->Show(show);
		m_pcWndSubSPO2alarmdelay->SetFocus();
	}
}

/**********************************************************************************************//**
 * Destroys the window sub setting spo 2 module
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::DestroyWndSubSettingSPO2Module()
{
	if(m_pcWndSubSPO2Module)
	{
		//m_pcWndSubSPO2Module->writeData();
		m_pcWndSubSPO2Module->DestroyWindow();
		delete m_pcWndSubSPO2Module;
		m_pcWndSubSPO2Module=NULL;
	}
	return false;
}

/**********************************************************************************************//**
 * Creates window sub setting spo 2 module
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	viewFlag	The view flag.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::CreateWndSubSettingSPO2Module(UINT viewFlag)
{
	if(m_pcWndSubSPO2Module==NULL)
	{
		m_pcWndSubSPO2Module = new CWndSubSettingsSPO2Module(viewFlag);
		//RECT rcLd={m_lX,m_lY,m_lX+m_lCx,m_lY+m_lCy};

		RECT rcLd={0,0,800,510};
		//RECT rcLd={0,0,565,485};
		if(m_pcWndSubSPO2Module->CreateWnd(this,rcLd,IDC_VIEW_SETUP_SPO2MODULE))
		{
			return true;
		}
	}
	return false;
}

/**********************************************************************************************//**
 * Shows the window sub setting spo 2 module
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	show	True to show, false to hide.
 **************************************************************************************************/

void CViewMenu::ShowWndSubSettingSPO2Module(bool show)
{
	if(m_pcWndSubSPO2Module)
	{
		m_pcWndSubSPO2Module->Show(show);
		m_pcWndSubSPO2Module->SetFocus();
	}
}

/**********************************************************************************************//**
 * Destroys the window sub setting co 2 module
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::DestroyWndSubSettingCO2Module()
{
	if(m_pcWndSubCO2Module)
	{
		//m_pcWndSubCO2Module->writeData();
		m_pcWndSubCO2Module->DestroyWindow();
		delete m_pcWndSubCO2Module;
		m_pcWndSubCO2Module=NULL;
	}
	return false;
}

/**********************************************************************************************//**
 * Creates window sub setting co 2 module
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	viewFlag	The view flag.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::CreateWndSubSettingCO2Module(UINT viewFlag)
{
	if(m_pcWndSubCO2Module==NULL)
	{
		m_pcWndSubCO2Module = new CWndSubSettingsCO2Module(viewFlag);
		//RECT rcLd={m_lX,m_lY,m_lX+m_lCx,m_lY+m_lCy};

		RECT rcLd={0,0,800,510};
		//RECT rcLd={0,0,565,485};
		if(m_pcWndSubCO2Module->CreateWnd(this,rcLd,IDC_VIEW_SETUP_CO2MODULE))
		{
			return true;
		}
	}
	return false;
}

/**********************************************************************************************//**
 * Shows the window sub setting co 2 module
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	show	True to show, false to hide.
 **************************************************************************************************/

void CViewMenu::ShowWndSubSettingCO2Module(bool show)
{
	if(m_pcWndSubCO2Module)
	{
		m_pcWndSubCO2Module->Show(show);
		m_pcWndSubCO2Module->SetFocus();
	}
}

/**********************************************************************************************//**
 * Destroys the window sub setting leak component
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::DestroyWndSubSettingLeakComp()
{
	if(m_pcWndSubLeakComp)
	{
		m_pcWndSubLeakComp->DestroyWindow();
		delete m_pcWndSubLeakComp;
		m_pcWndSubLeakComp=NULL;
	}
	return false;
}

/**********************************************************************************************//**
 * Creates window sub setting leak component
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	viewFlag	The view flag.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::CreateWndSubSettingLeakComp(UINT viewFlag)
{
	if(m_pcWndSubLeakComp==NULL)
	{
		m_pcWndSubLeakComp = new CWndSubSettingsLeakComp(viewFlag);

		RECT rcLd={0,0,800,510};
		if(m_pcWndSubLeakComp->CreateWnd(this,rcLd,IDC_VIEW_SETUP_LEAKCOMP))
		{
			return true;
		}
	}
	return false;
}

/**********************************************************************************************//**
 * Shows the window sub setting leak component
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	show	True to show, false to hide.
 **************************************************************************************************/

void CViewMenu::ShowWndSubSettingLeakComp(bool show)
{
	if(m_pcWndSubLeakComp)
	{
		m_pcWndSubLeakComp->Show(show);
		m_pcWndSubLeakComp->SetFocus();
	}
}

/**********************************************************************************************//**
 * Destroys the window sub setting co 2 units
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::DestroyWndSubSettingCO2Units()
{
	if(m_pcWndSubCO2Units)
	{
		m_pcWndSubCO2Units->DestroyWindow();
		delete m_pcWndSubCO2Units;
		m_pcWndSubCO2Units=NULL;
	}
	return false;
}

/**********************************************************************************************//**
 * Creates window sub setting co 2 units
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	viewFlag	The view flag.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::CreateWndSubSettingCO2Units(UINT viewFlag)
{
	if(m_pcWndSubCO2Units==NULL)
	{
		m_pcWndSubCO2Units = new CWndSubSettingsCO2Units(viewFlag);
		//RECT rcLd={m_lX,m_lY,m_lX+m_lCx,m_lY+m_lCy};

		RECT rcLd={0,0,800,510};
		//RECT rcLd={0,0,565,485};
		if(m_pcWndSubCO2Units->CreateWnd(this,rcLd,IDC_VIEW_SETUP_CO2UNITS))
		{
			return true;
		}
	}
	return false;
}

/**********************************************************************************************//**
 * Shows the window sub setting co 2 units
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	show	True to show, false to hide.
 **************************************************************************************************/

void CViewMenu::ShowWndSubSettingCO2Units(bool show)
{
	if(m_pcWndSubCO2Units)
	{
		m_pcWndSubCO2Units->Show(show);
		m_pcWndSubCO2Units->SetFocus();
	}
}

/**********************************************************************************************//**
 * Destroys the window sub setting baro pressure
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::DestroyWndSubSettingBaroPressure()
{
	if(m_pcWndSubBaroPressure)
	{
		m_pcWndSubBaroPressure->DestroyWindow();
		delete m_pcWndSubBaroPressure;
		m_pcWndSubBaroPressure=NULL;
	}
	return false;
}

/**********************************************************************************************//**
 * Creates window sub setting baro pressure
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	viewFlag	The view flag.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::CreateWndSubSettingBaroPressure(UINT viewFlag)
{
	if(m_pcWndSubBaroPressure==NULL)
	{
		m_pcWndSubBaroPressure = new CWndSubSettingsBaroPressure(viewFlag);
		//RECT rcLd={m_lX,m_lY,m_lX+m_lCx,m_lY+m_lCy};

		RECT rcLd={0,0,800,510};
		//RECT rcLd={0,0,565,485};
		if(m_pcWndSubBaroPressure->CreateWnd(this,rcLd,IDC_VIEW_SETUP_BAROPRESSURE))
		{
			return true;
		}
	}
	return false;
}

/**********************************************************************************************//**
 * Shows the window sub setting baro pressure
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	show	True to show, false to hide.
 **************************************************************************************************/

void CViewMenu::ShowWndSubSettingBaroPressure(bool show)
{
	if(m_pcWndSubBaroPressure)
	{
		m_pcWndSubBaroPressure->Show(show);
		m_pcWndSubBaroPressure->SetFocus();
	}
}

/**********************************************************************************************//**
 * Destroys the window sub setting automatic on pump
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::DestroyWndSubSettingAutoOnPump()
{
	if(m_pcWndSubAutoOnPump)
	{
		m_pcWndSubAutoOnPump->DestroyWindow();
		delete m_pcWndSubAutoOnPump;
		m_pcWndSubAutoOnPump=NULL;
	}
	return false;
}

/**********************************************************************************************//**
 * Creates window sub setting automatic on pump
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	viewFlag	The view flag.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::CreateWndSubSettingAutoOnPump(UINT viewFlag)
{
	if(m_pcWndSubAutoOnPump==NULL)
	{
		m_pcWndSubAutoOnPump = new CWndSubSettingsAutoOnPump(viewFlag);
		//RECT rcLd={m_lX,m_lY,m_lX+m_lCx,m_lY+m_lCy};

		RECT rcLd={0,0,800,510};
		//RECT rcLd={0,0,565,485};
		if(m_pcWndSubAutoOnPump->CreateWnd(this,rcLd,IDC_VIEW_SETUP_AUTOONPUMPN))
		{
			return true;
		}
	}
	return false;
}

/**********************************************************************************************//**
 * Shows the window sub setting automatic on pump
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	show	True to show, false to hide.
 **************************************************************************************************/

void CViewMenu::ShowWndSubSettingAutoOnPump(bool show)
{
	if(m_pcWndSubAutoOnPump)
	{
		m_pcWndSubAutoOnPump->Show(show);
		m_pcWndSubAutoOnPump->SetFocus();
	}
}

/**********************************************************************************************//**
 * Destroys the window sub setting pressure unit
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::DestroyWndSubSettingPressureUnit()
{
	if(m_pcWndSubPressureUnit)
	{
		m_pcWndSubPressureUnit->DestroyWindow();
		delete m_pcWndSubPressureUnit;
		m_pcWndSubPressureUnit=NULL;
	}
	return false;
}

/**********************************************************************************************//**
 * Creates window sub setting pressure unit
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	viewFlag	The view flag.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewMenu::CreateWndSubSettingPressureUnit(UINT viewFlag)
{
	if(m_pcWndSubPressureUnit==NULL)
	{
		m_pcWndSubPressureUnit = new CWndSubSettingsPressureUnit(viewFlag);
		//RECT rcLd={m_lX,m_lY,m_lX+m_lCx,m_lY+m_lCy};

		RECT rcLd={0,0,800,510};
		//RECT rcLd={0,0,565,485};
		if(m_pcWndSubPressureUnit->CreateWnd(this,rcLd,IDC_VIEW_SETUP_PRESSUREUNIT))
		{
			return true;
		}
	}
	return false;
}

/**********************************************************************************************//**
 * Shows the window sub setting pressure unit
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	show	True to show, false to hide.
 **************************************************************************************************/

void CViewMenu::ShowWndSubSettingPressureUnit(bool show)
{
	if(m_pcWndSubPressureUnit)
	{
		m_pcWndSubPressureUnit->Show(show);
		m_pcWndSubPressureUnit->SetFocus();
	}
}

/**********************************************************************************************//**
// End of ViewMenu.cpp
 **************************************************************************************************/

