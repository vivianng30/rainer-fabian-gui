// SubViewCalFlowSensor.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "SubViewCalFlowSensor.h"
#include "globDefs.h"
#include "MenuBtn.h"
#include "DlgMessageBox.h"

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
extern HFONT g_hf70Bold;

#define TIMECHANGE	50
// CSubViewCalFlowSensor

IMPLEMENT_DYNAMIC(CSubViewCalFlowSensor, CWnd)

CSubViewCalFlowSensor::CSubViewCalFlowSensor()
{
	m_pModel=NULL;
	m_pDlg = NULL;

	m_pcRegionTopLeCorner=NULL;
	m_pcRegionTopRiCorner=NULL;
	m_pcRegionBotLeCorner=NULL;
	m_pcRegionBotRiCorner=NULL;

	m_hDC=NULL;
	m_hBmp=NULL;
	m_hBmpPrev=NULL;
	m_lX				= 0;
	m_lY				= 0;
	m_lXo				= 0;
	m_lYo				= 0;

	m_hdcStatic=NULL;
	m_hdcTmp=NULL;
	m_hbmpStatic=NULL;
	m_hbmpTmp=NULL;

	m_pcNeonatal=NULL;
	m_pcPediatric=NULL;

	//m_bNeoPedConfirmed=false;
	m_pbtnBodyweight=NULL;
	//m_pcBodyweightUp=NULL;
	//m_pcBodyweightDwn=NULL;
	/*m_pcUp_Up=NULL;
	m_pcUp_Dw=NULL;
	m_pcDw_Up=NULL;
	m_pcDw_Dw=NULL;*/

	m_pcStatic_Up=NULL;
	m_pcStatic_Dw=NULL;
	m_pcStatic_Fc=NULL;

	m_pcNeonatal_Up=NULL;
	m_pcNeonatal_Dw=NULL;
	m_pcNeonatal_Fc=NULL;

	m_pcPediatric_Up=NULL;
	m_pcPediatric_Dw=NULL;
	m_pcPediatric_Fc=NULL;
	m_pcPediatric_Dis=NULL;

	m_iFlowHValue=0;
	m_iFlowLValue=0;

	m_iFlowHDAC=0;
	m_iFlowLDAC=0;

	m_pcOk=NULL;
	m_pcExclamation=NULL;
	m_pcCalibration=NULL;
	m_pcWait=NULL;
	m_pcWait1= NULL;
	m_pcWait2= NULL;
	m_pcWait3= NULL;
	m_pcWait4= NULL;
	m_pcWait5= NULL;
	m_pcWait6= NULL;
	m_pcWait7= NULL;
	m_pcWait8= NULL;
	m_iWaitCount=0;

	m_bCkeckRunning=false;
	m_bCalRunning=false;
	//m_bFlowSensorOk=true;
	m_eFlowCalState=FS_UNKNOWN;

	m_szLastFlowCal=_T("");

	m_szTextFlowSensorTop=_T("");
	m_szTextFlowSensorBot=_T("");

	m_pcMenu_Dis=NULL;
	m_pcMenu_Up=NULL;
	m_pcMenu_Dw=NULL;

	m_pcFlowOnOff=NULL;
	m_pcFlowCalExt=NULL;

	m_pcMenuCO2=NULL;
	m_pcMenuSpO2=NULL;
	m_pcMenuOxi=NULL;

	m_pcMenuCO2_UP=NULL;
	m_pcMenuCO2_DW=NULL;
	m_pcMenuCO2_FC=NULL;
	m_pcMenuCO2_DIS=NULL;

	m_pcMenuSPO2_UP=NULL;
	m_pcMenuSPO2_DW=NULL;
	m_pcMenuSPO2_FC=NULL;
	m_pcMenuSPO2_DIS=NULL;

	m_pcMenuOxi_UP=NULL;
	m_pcMenuOxi_DW=NULL;
	m_pcMenuOxi_FC=NULL;
	m_pcMenuOxi_DIS=NULL;

	m_bCalActive=false;

	m_bCancelFlowCal=false;
	m_bFlowCheckStarted=false;
	m_bFlowSensorCalibrated=false;
	m_bFlowCalStarted=false;

	m_bExit=false;

	m_pcwtCheckFlow=NULL;
	m_hThreadCheckFlow=INVALID_HANDLE_VALUE;
	m_bDoFlowCheck=false;

	m_bFlowsensor=getModel()->IsFlowsensorFlag();
	if(		getModel()->getCONFIG()->GetCurMode()==VM_NCPAP 
		||	getModel()->getCONFIG()->GetCurMode()==VM_DUOPAP
		||	getModel()->getCONFIG()->GetCurMode()==VM_THERAPIE)
	{
		m_bInfantFlow=true;
	}
	else
	{
		m_bInfantFlow=false;
	}

	m_iBodyweight=0;
	//m_iCounter=0;
	//m_eTimeChanger=TC_OFF;
}

CSubViewCalFlowSensor::~CSubViewCalFlowSensor()
{
	if(m_pcwtCheckFlow!=NULL)
	{
		delete m_pcwtCheckFlow;
		m_pcwtCheckFlow=NULL;

		if(m_hThreadCheckFlow!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadCheckFlow);
			m_hThreadCheckFlow=INVALID_HANDLE_VALUE;
		}
	}

	delete m_pbtnBodyweight;
	m_pbtnBodyweight=NULL;

	/*delete m_pcBodyweightUp;
	m_pcBodyweightUp=NULL;
	delete m_pcBodyweightDwn;
	m_pcBodyweightDwn=NULL;*/

	delete m_pcStatic_Up;
	m_pcStatic_Up=NULL;
	delete m_pcStatic_Dw;
	m_pcStatic_Dw=NULL;
	delete m_pcStatic_Fc;
	m_pcStatic_Fc=NULL;

	/*delete m_pcUp_Up;
	delete m_pcUp_Dw;
	delete m_pcDw_Up;
	delete m_pcDw_Dw;

	m_pcUp_Up=NULL;
	m_pcUp_Dw=NULL;
	m_pcDw_Up=NULL;
	m_pcDw_Dw=NULL;*/

	delete m_pcNeonatal;
	m_pcNeonatal=NULL;
	delete m_pcPediatric;
	m_pcPediatric=NULL;

	delete m_pcNeonatal_Up;
	m_pcNeonatal_Up=NULL;
	delete m_pcNeonatal_Dw;
	m_pcNeonatal_Dw=NULL;
	delete m_pcNeonatal_Fc;
	m_pcNeonatal_Fc=NULL;

	delete m_pcPediatric_Up;
	m_pcPediatric_Up=NULL;
	delete m_pcPediatric_Dw;
	m_pcPediatric_Dw=NULL;
	delete m_pcPediatric_Fc;
	m_pcPediatric_Fc=NULL;
	delete m_pcPediatric_Dis;
	m_pcPediatric_Dis=NULL;


	if(m_pcExclamation)
		delete m_pcExclamation;
	m_pcExclamation=NULL;

	if(m_pcOk)
		delete m_pcOk;
	m_pcOk=NULL;

	if(m_pcCalibration)
		delete m_pcCalibration;
	m_pcCalibration=NULL;

	delete m_pcRegionTopLeCorner;
	m_pcRegionTopLeCorner=NULL;
	delete m_pcRegionTopRiCorner;
	m_pcRegionTopRiCorner=NULL;
	delete m_pcRegionBotLeCorner;
	m_pcRegionBotLeCorner=NULL;
	delete m_pcRegionBotRiCorner;
	m_pcRegionBotRiCorner=NULL;

	delete m_pcMenuCO2;
	m_pcMenuCO2=NULL;

	delete m_pcMenuSpO2;
	m_pcMenuSpO2=NULL;

	delete m_pcMenuOxi;
	m_pcMenuOxi=NULL;

	delete m_pcMenuCO2_UP;
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
	m_pcMenuSPO2_DIS=NULL;

	delete m_pcMenuOxi_UP;
	m_pcMenuOxi_UP=NULL;
	delete m_pcMenuOxi_DW;
	m_pcMenuOxi_DW=NULL;
	delete m_pcMenuOxi_FC;
	m_pcMenuOxi_FC=NULL;
	delete m_pcMenuOxi_DIS;
	m_pcMenuOxi_DIS=NULL;

	delete m_pcWait;
	m_pcWait=NULL;
	delete m_pcWait1;
	m_pcWait1=NULL;
	delete m_pcWait2;
	m_pcWait2=NULL;
	delete m_pcWait3;
	m_pcWait3=NULL;
	delete m_pcWait4;
	m_pcWait4=NULL;
	delete m_pcWait5;
	m_pcWait5=NULL;
	delete m_pcWait6;
	m_pcWait6=NULL;
	delete m_pcWait7;
	m_pcWait7=NULL;
	delete m_pcWait8;
	m_pcWait8=NULL;

	delete m_pcFlowOnOff;
	m_pcFlowOnOff=NULL;
	delete m_pcFlowCalExt;
	m_pcFlowCalExt=NULL;
	/*delete m_pcFlowCalInt;
	m_pcFlowCalInt=NULL;*/

	delete m_pcMenu_Up;
	m_pcMenu_Up=NULL;
	delete m_pcMenu_Dw;
	m_pcMenu_Dw=NULL;
	delete m_pcMenu_Dis;
	m_pcMenu_Dis=NULL;
}


BEGIN_MESSAGE_MAP(CSubViewCalFlowSensor, CWnd)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTNCAL_OXI, &CSubViewCalFlowSensor::OnBnClickedO2)
	ON_BN_CLICKED(IDC_BTNCAL_CO2, &CSubViewCalFlowSensor::OnBnClickedCO2)
	ON_BN_CLICKED(IDC_BTNCAL_SPO2, &CSubViewCalFlowSensor::OnBnClickedSPO2)
	ON_BN_CLICKED(IDC_BTN_SETUP_BODYWEIGHT, &CSubViewCalFlowSensor::OnBnClickedBodyweight)
END_MESSAGE_MAP()



// CSubViewCalFlowSensor message handlers
// **************************************************************************
// 
// **************************************************************************
CMVModel *CSubViewCalFlowSensor::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}

// **************************************************************************
// 
// **************************************************************************
bool CSubViewCalFlowSensor::CreateWnd(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext)
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
BOOL CSubViewCalFlowSensor::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext) 
{
	m_lX=rc.right-rc.left;
	m_lY=rc.bottom-rc.top;

	CStringW  lpszClassName=AfxRegisterWndClass(NULL); 
	if(CWnd::Create(lpszClassName,NULL,WS_CHILD|WS_VISIBLE,rc,pParentWnd,nID))
	{
		CClientDC dc(this);

		m_pcRegionTopLeCorner		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_SETUP_LE_TOP_CORNER);
		m_pcRegionTopRiCorner		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_SETUP_RI_TOP_CORNER);
		m_pcRegionBotLeCorner		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_SETUP_LE_BOT_CORNER);
		m_pcRegionBotRiCorner		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_SETUP_RI_BOT_CORNER);

		//m_pcUp_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETUP_UPUP);
		//m_pcUp_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETUP_UPDW);
		//m_pcUp_Fc= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETUP_UPFC);
		//m_pcDw_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETUP_DWUP);
		//m_pcDw_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETUP_DWDW);
		//m_pcDw_Fc= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETUP_DWFC);

		m_pcStatic_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_VALUE_UP);
		m_pcStatic_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_VALUE_DW);
		m_pcStatic_Fc= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_VALUE_FC);

		m_pcNeonatal_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETNEO_UP);
		m_pcNeonatal_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETNEO_DW);
		m_pcNeonatal_Fc= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETNEO_FC);

		m_pcPediatric_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETPED_UP);
		m_pcPediatric_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETPED_DW);
		m_pcPediatric_Fc= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETPED_FC);
		m_pcPediatric_Dis= new CBmp(theApp.m_hInstance,dc.m_hDC, IDB_BTN_SETPED_DIS);

		m_pcMenuCO2_UP= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTNMNU_CO2_UP);
		m_pcMenuCO2_DW= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTNMNU_CO2_DW);
		m_pcMenuCO2_FC= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTNMNU_CO2_FC);
		m_pcMenuCO2_DIS= new CBmp(theApp.m_hInstance,dc.m_hDC, IDB_BTNMNU_CO2_DIS);

		m_pcMenuSPO2_UP= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTNMNU_SPO2_UP);
		m_pcMenuSPO2_DW= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTNMNU_SPO2_DW);
		m_pcMenuSPO2_FC= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTNMNU_SPO2_FC);
		m_pcMenuSPO2_DIS= new CBmp(theApp.m_hInstance,dc.m_hDC, IDB_BTNMNU_SPO2_DIS);

		m_pcMenuOxi_UP= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTNMNU_O2_UP);
		m_pcMenuOxi_DW= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTNMNU_O2_DW);
		m_pcMenuOxi_FC= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTNMNU_O2_FC);
		m_pcMenuOxi_DIS= new CBmp(theApp.m_hInstance,dc.m_hDC, IDB_BTNMNU_O2_DIS);


		m_hDC=CreateCompatibleDC(dc.m_hDC);
		m_hBmp=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
		m_hBmpPrev=(HBITMAP)SelectObject(m_hDC,m_hBmp);

		//CBrush cbrBack(RGB(255,0,0));
		CBrush cbrBack(RGB(230,230,255));
		CBrush cbrFill;
		cbrFill.CreateSolidBrush(RGB(181,178,181));
		//cbrFill.CreateSolidBrush(RGB(200,200,200));
		HBRUSH hbrprev=(HBRUSH)SelectObject(m_hDC,cbrBack);
		HPEN hpenprev=(HPEN)SelectObject(m_hDC, (HPEN)GetStockObject(NULL_PEN));	

		Rectangle(m_hDC, 0, 0, m_lX, m_lY);

		RECT rcCl,rcTop,rcBot;
		GetClientRect(&rcCl);

		rcTop.left = 25; 
		rcTop.right = 775; 
		rcTop.top = 5; 
		rcTop.bottom = 25; 

		rcBot.left = 25; 
		rcBot.right = 775; 
		rcBot.top = 330; 
		rcBot.bottom = 350; 

		rcCl.left = 5; 
		rcCl.right = 795; 
		rcCl.top = 25; 
		rcCl.bottom = 330; 

		FillRect(m_hDC,&rcCl,cbrFill);
		FillRect(m_hDC,&rcTop,cbrFill);
		FillRect(m_hDC,&rcBot,cbrFill);

		m_pcRegionTopLeCorner->Draw(m_hDC,5,5);
		m_pcRegionTopRiCorner->Draw(m_hDC,775,5);
		m_pcRegionBotLeCorner->Draw(m_hDC,5,330);
		m_pcRegionBotRiCorner->Draw(m_hDC,775,330);

		m_hdcTmp = CreateCompatibleDC(m_hDC);
		m_hbmpTmp = CreateCompatibleBitmap(m_hDC,m_lX,m_lY);
		SelectObject(m_hdcTmp, m_hbmpTmp);

		m_hdcStatic = CreateCompatibleDC(m_hDC);
		m_hbmpStatic = CreateCompatibleBitmap(m_hDC, m_lX, m_lY);
		SelectObject(m_hdcStatic, m_hbmpStatic);

		// delete background
		HPEN hpenprevTmp=(HPEN)SelectObject(m_hdcTmp, (HPEN)GetStockObject(NULL_PEN));							
		HBRUSH hbrprevTmp=(HBRUSH)SelectObject(m_hdcTmp,cbrBack); 
		Rectangle(m_hdcTmp, 0, 0, m_lX, m_lY);

		FillRect(m_hdcTmp,&rcCl,cbrFill);
		FillRect(m_hdcTmp,&rcTop,cbrFill);
		FillRect(m_hdcTmp,&rcBot,cbrFill);
		m_pcRegionTopLeCorner->Draw(m_hdcTmp,5,5);
		m_pcRegionTopRiCorner->Draw(m_hdcTmp,775,5);
		m_pcRegionBotLeCorner->Draw(m_hdcTmp,5,330);
		m_pcRegionBotRiCorner->Draw(m_hdcTmp,775,330);

		HPEN hpenprevStat=(HPEN)SelectObject(m_hdcStatic, (HPEN)GetStockObject(NULL_PEN));	
		HBRUSH hbrprevStat=(HBRUSH)SelectObject(m_hdcStatic,cbrBack); 
		Rectangle(m_hdcStatic, 0, 0, m_lX, m_lY);
		

		FillRect(m_hdcStatic,&rcCl,cbrFill);
		FillRect(m_hdcStatic,&rcTop,cbrFill);
		FillRect(m_hdcStatic,&rcBot,cbrFill);
		m_pcRegionTopLeCorner->Draw(m_hdcStatic,5,5);
		m_pcRegionTopRiCorner->Draw(m_hdcStatic,775,5);
		m_pcRegionBotLeCorner->Draw(m_hdcStatic,5,330);
		m_pcRegionBotRiCorner->Draw(m_hdcStatic,775,330);

		SelectObject(m_hdcTmp, hpenprevTmp);
		SelectObject(m_hdcTmp,hbrprevTmp);
		SelectObject(m_hdcStatic, hpenprevStat);
		SelectObject(m_hdcStatic,hbrprevStat);
		SelectObject(m_hDC,hpenprev);
		SelectObject(m_hDC,hbrprev);
		//cbrBack.DeleteObject();//rkuNEWFIX


		return 1;
	}
	else
		return 0;
}


void CSubViewCalFlowSensor::Init()
{
	CClientDC dc(this);

	m_pcMenu_Up		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PAT_UP);
	m_pcMenu_Dw		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PAT_DW);
	m_pcMenu_Dis		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PAT_DIS);

	m_pcOk = new CBmp(theApp.m_hInstance,dc.m_hDC,IDB_OK_UP);
	m_pcExclamation = new CBmp(theApp.m_hInstance,dc.m_hDC,IDB_EXCLAMATION);
	m_pcCalibration = new CBmp(theApp.m_hInstance,dc.m_hDC,IDB_CALSENSOR);
	m_pcWait=NULL;
	m_pcWait1= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_WAIT30_GREY1);
	m_pcWait2= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_WAIT30_GREY2);
	m_pcWait3= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_WAIT30_GREY3);
	m_pcWait4= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_WAIT30_GREY4);
	m_pcWait5= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_WAIT30_GREY5);
	m_pcWait6= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_WAIT30_GREY6);
	m_pcWait7= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_WAIT30_GREY7);
	m_pcWait8= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_WAIT30_GREY8);

	//DWORD dwStyleNoTab = WS_CHILD|WS_VISIBLE|BS_OWNERDRAW|BS_MULTILINE;

	BTN btn;

	btn.wID					= IDC_BTN_CALFLOW_ONOFF;	
	btn.poPosition.x		= 620;
	btn.poPosition.y		= 185;//150;
	btn.pcBmpUp				= m_pcMenu_Up;
	btn.pcBmpDown			= m_pcMenu_Dw;
	btn.pcBmpFocus			= m_pcMenu_Up;
	btn.pcBmpDisabled		= m_pcMenu_Dis;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;

	m_pcFlowOnOff=new CPushBtn(btn,COLOR_TXTBTNUP,true);
	m_pcFlowOnOff->Create(this,g_hf10AcuBold,0);
	if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false)
	{
		m_pcFlowOnOff->SetText(getModel()->GetLanguageString(IDS_MENU_CAL1),getModel()->GetLanguageString(IDS_MENU_CAL2));
	}
	else
	{
		m_pcFlowOnOff->SetText(getModel()->GetLanguageString(IDS_MENU_CAL3),getModel()->GetLanguageString(IDS_MENU_CAL4));
	}
	m_pcFlowOnOff->ShowWindow(SW_HIDE);



	btn.wID					= IDC_BTN_CALFLOW_CALEXT;	
	btn.poPosition.x		= 620;//151;
	btn.poPosition.y		= 125;//50;
	btn.pcBmpUp				= m_pcMenu_Up;
	btn.pcBmpDown			= m_pcMenu_Dw;
	btn.pcBmpFocus			= m_pcMenu_Up;
	btn.pcBmpDisabled		= m_pcMenu_Dis;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcFlowCalExt=new CPushBtn(btn,COLOR_TXTBTNUP,false);
	m_pcFlowCalExt->Create(this,g_hf11AcuBold,0);
	//m_pcFlowCalExt->SetText(getModel()->GetLanguageString(IDS_MENU_FLOWCAL_EXT_TOP),getModel()->GetLanguageString(IDS_MENU_FLOWCAL_EXT_BOT));
	m_pcFlowCalExt->SetText(getModel()->GetLanguageString(IDS_MENU_FLOWCAL));
	m_pcFlowCalExt->ShowWindow(SW_HIDE);


	//CStringW cs=_T("");
	
	//Menu Buttons
	/*****************************O2***************************/
	btn.wID					= IDC_BTNCAL_OXI;	
	btn.poPosition.x		= 128;
	btn.poPosition.y		= 370;//320;
	btn.pcBmpUp				= m_pcMenuOxi_UP;
	btn.pcBmpDown			= m_pcMenuOxi_DW;
	btn.pcBmpFocus			= m_pcMenuOxi_FC;
	btn.pcBmpDisabled		= m_pcMenuOxi_DIS;
	btn.dwFormat			= DT_BOTTOM|DT_SINGLELINE|DT_CENTER;

	m_pcMenuOxi=new CMenuBtn(btn,COLOR_TXTBTNUP);
	m_pcMenuOxi->Create(this,g_hf13AcuBold,0,10);
	m_pcMenuOxi->SetText(getModel()->GetLanguageString(IDS_BTN_MNU_O2),getModel()->GetLanguageString(IDS_BTN_MNU_O2));
	m_plMenuBtn.AddTail(m_pcMenuOxi);

	/*****************************CO2***************************/
	btn.wID					= IDC_BTNCAL_CO2;	
	btn.poPosition.x		= 328;
	btn.poPosition.y		= 370;//320;
	btn.pcBmpUp				= m_pcMenuCO2_UP;
	btn.pcBmpDown			= m_pcMenuCO2_DW;
	btn.pcBmpFocus			= m_pcMenuCO2_FC;
	btn.pcBmpDisabled		= m_pcMenuCO2_DIS;
	//btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;
	btn.dwFormat			= DT_BOTTOM|DT_SINGLELINE|DT_CENTER;

	m_pcMenuCO2=new CMenuBtn(btn,COLOR_TXTBTNUP);
	m_pcMenuCO2->Create(this,g_hf13AcuBold,0,10);
	m_pcMenuCO2->SetText(getModel()->GetLanguageString(IDS_BTN_MNU_CO2),getModel()->GetLanguageString(IDS_BTN_MNU_CO2));
	if(getModel()->getCONFIG()->isCO2REGISTRYenabled()==false)
	{
		m_pcMenuCO2->ShowWindow(SW_HIDE);
	}
	else
	{	
		m_plMenuBtn.AddTail(m_pcMenuCO2);
	}

	/*****************************SPO2***************************/
	btn.wID					= IDC_BTNCAL_SPO2;	
	btn.poPosition.x		= 528;
	btn.poPosition.y		= 370;//320;
	btn.pcBmpUp				= m_pcMenuSPO2_UP;
	btn.pcBmpDown			= m_pcMenuSPO2_DW;
	btn.pcBmpFocus			= m_pcMenuSPO2_FC;
	btn.pcBmpDisabled		= m_pcMenuSPO2_DIS;
	btn.dwFormat			= DT_BOTTOM|DT_SINGLELINE|DT_CENTER;

	m_pcMenuSpO2=new CMenuBtn(btn,COLOR_TXTBTNUP);
	m_pcMenuSpO2->Create(this,g_hf13AcuBold,0,10);
	m_pcMenuSpO2->SetText(getModel()->GetLanguageString(IDS_BTN_MNU_SPO2),getModel()->GetLanguageString(IDS_BTN_MNU_SPO2));
	if(getModel()->getCONFIG()->isSPO2REGISTRYenabled()==false)
	{
		m_pcMenuSpO2->ShowWindow(SW_HIDE);
	}
	else
	{	
		m_plMenuBtn.AddTail(m_pcMenuSpO2);
	}

	
	//+++++++++++++++++Neo/Pediatric++++++++++++++++++++++++++++++
	btn.wID					= IDC_BTN_NEO;	
	btn.poPosition.x		= 22;
	btn.poPosition.y		= 110;
	btn.pcBmpUp				= m_pcNeonatal_Up;
	btn.pcBmpDown			= m_pcNeonatal_Dw;
	btn.pcBmpFocus			= m_pcNeonatal_Fc;
	btn.pcBmpDisabled		= m_pcNeonatal_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;

	m_pcNeonatal=new CPresetMenuBtn(btn,COLOR_TXTBTNUP);
	m_pcNeonatal->Create(this,g_hf9AcuBold,0);
	m_pcNeonatal->SetText(_T(""));

	

	btn.wID					= IDC_BTN_PED;	
	btn.poPosition.x		= 22;
	btn.poPosition.y		= 165;
	btn.pcBmpUp				= m_pcPediatric_Up;
	btn.pcBmpDown			= m_pcPediatric_Dw;
	btn.pcBmpFocus			= m_pcPediatric_Fc;
	btn.pcBmpDisabled		= m_pcPediatric_Dis;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;

	m_pcPediatric=new CPresetMenuBtn(btn,COLOR_TXTBTNUP);
	m_pcPediatric->Create(this,g_hf9AcuBold,0);
	m_pcPediatric->SetText(_T(""));

	if(false==getModel()->getCONFIG()->useNeoPed())
	{
		m_pcPediatric->SetBtnState(CPresetMenuBtn::UP);
		m_pcNeonatal->SetBtnState(CPresetMenuBtn::DOWN);

		m_pcPediatric->EnableWindow(FALSE);
	}
	else
	{
		m_pcPediatric->EnableWindow(TRUE);

		if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		{
			m_pcPediatric->SetBtnState(CPresetMenuBtn::UP);
			m_pcNeonatal->SetBtnState(CPresetMenuBtn::DOWN);
		}
		else
		{
			m_pcPediatric->SetBtnState(CPresetMenuBtn::DOWN);
			m_pcNeonatal->SetBtnState(CPresetMenuBtn::UP);
		}
	}

	CStringW strBodyweight=_T("--");
	m_iBodyweight=getModel()->getDATAHANDLER()->GetBodyweight();

	CString szUnit=_T("g");
	if(m_iBodyweight>=BODYWEIGHTUNITSWITCH)
	{
		szUnit=_T("kg");
	}

	if(m_iBodyweight>0)
	{
		if(m_iBodyweight>=BODYWEIGHTUNITSWITCH)
			strBodyweight.Format(_T("%0.1f %s"),CTlsFloat::Round(((double)m_iBodyweight)/1000, 1),szUnit);
		else
			strBodyweight.Format(_T("%d %s"),m_iBodyweight,szUnit);
	}

	

	//*******************bodyweight*****************************
	btn.wID					= IDC_BTN_SETUP_BODYWEIGHT;	
	btn.poPosition.x		= 62;
	btn.poPosition.y		= 234;
	btn.pcBmpUp				= m_pcStatic_Up;
	btn.pcBmpDown			= m_pcStatic_Dw;
	btn.pcBmpFocus			= m_pcStatic_Fc;
	btn.pcBmpDisabled		= m_pcStatic_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;

	m_pbtnBodyweight=new CSelectSetupBtn(btn,COLOR_TXTBTNUP);
	m_pbtnBodyweight->Create(this,g_hf23AcuBold,0);
	m_pbtnBodyweight->SetText(strBodyweight);
	m_pbtnBodyweight->ShowWindow(SW_SHOW);

	//+++++++++++++++++bodyweight up++++++++++++++++++++++++++++++
	//m_pcBodyweightUp=NULL;
	/*btn.wID					= IDC_BTN_SETUP_NEXTUP;	
	btn.poPosition.x		= 123;
	btn.poPosition.y		= 222;
	btn.pcBmpUp				= m_pcUp_Up;
	btn.pcBmpDown			= m_pcUp_Dw;
	btn.pcBmpFocus			= m_pcUp_Up;
	btn.pcBmpDisabled		= m_pcUp_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;

	m_pcBodyweightUp=new CDTUpDwnBtn(btn,COLOR_TXTBTNUP);
	m_pcBodyweightUp->Create(this,g_hf21AcuBold,0);
	m_pcBodyweightUp->SetText(_T(""));
	m_pcBodyweightUp->ShowWindow(SW_SHOW);*/


	//+++++++++++++++++bodyweight up++++++++++++++++++++++++++++++
	//m_pcBodyweightDwn=NULL;
	/*btn.wID					= IDC_BTN_SETUP_NEXTDW;	
	btn.poPosition.x		= 123;
	btn.poPosition.y		= 264;
	btn.pcBmpUp				= m_pcDw_Up;
	btn.pcBmpDown			= m_pcDw_Dw;
	btn.pcBmpFocus			= m_pcDw_Up;
	btn.pcBmpDisabled		= m_pcDw_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;

	m_pcBodyweightDwn=new CDTUpDwnBtn(btn,COLOR_TXTBTNUP);
	m_pcBodyweightDwn->Create(this,g_hf21AcuBold,0);
	m_pcBodyweightDwn->SetText(_T(""));
	m_pcBodyweightDwn->ShowWindow(SW_SHOW);*/


	//TESTtrigger
	//m_pcPediatric->ShowWindow(SW_HIDE);
		

	if(		getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF
		||	getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_MANOFF
		||	getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_AUTOOFF)
	{
		CStringW sz=_T("");
		SetFlowSensorText(getModel()->GetLanguageString(IDS_TXT_SENSOROFF),sz);
		SetFlowSensorValues(-70,-70,0,0);

		m_eFlowCalState=FS_OFF;
	}
	else
	{
		CheckFlowSensorState();
	}

	SHORT iFlowA=-70;
	SHORT iFlowB=-70;

	SHORT iHighDAC=0;
	SHORT iLowDAC=0;

	switch(m_eFlowCalState)
	{
	case FS_OK:
	case FS_CLEAN://???????????????????????????????????????
		{
			iFlowA=getModel()->getSPI()->Read_FLOWCAL_DEV_A();
			iFlowB=getModel()->getSPI()->Read_FLOWCAL_DEV_B();

			iHighDAC=getModel()->getSPI()->Read_DAC_FLOW_INSP();
			iLowDAC=getModel()->getSPI()->Read_DAC_FLOW_EXH();
		}
		break;
	case FS_FLOW_B_DEFECT:
		{
			iFlowA=getModel()->getSPI()->Read_FLOWCAL_DEV_A();
			iHighDAC=getModel()->getSPI()->Read_DAC_FLOW_INSP();
		}
		break;
	case FS_FLOW_A_DEFECT:
		{
			iFlowB=getModel()->getSPI()->Read_FLOWCAL_DEV_B();
			iLowDAC=getModel()->getSPI()->Read_DAC_FLOW_EXH();
		}
		break;
	default:
		{

		}
		break;
	}

	
	SetFlowSensorValues(iFlowA,iFlowB,iHighDAC,iLowDAC);


	CStringW szTimeLastFlowCal = getModel()->getCONFIG()->GetLastCalFlow();
	COleDateTime dtTimeLastFlowCal;

	if(szTimeLastFlowCal == _T(""))
	{
		//m_szLastFlowCal=getModel()->GetLanguageString(IDS_TXT_NOTCALIBRATED);
		m_szLastFlowCal=getModel()->GetLanguageString(IDS_TXT_UNKNOWN);
	}
	else
	{
		if(false==dtTimeLastFlowCal.ParseDateTime(szTimeLastFlowCal,LOCALE_NOUSEROVERRIDE))
		{
			m_szLastFlowCal=getModel()->GetLanguageString(IDS_TXT_UNKNOWN);
		}
		else
		{
			m_szLastFlowCal.Format(_T("%02d.%02d.%04d %02d:%02d:%02d"),
				dtTimeLastFlowCal.GetDay(),
				dtTimeLastFlowCal.GetMonth(),
				dtTimeLastFlowCal.GetYear(),
				dtTimeLastFlowCal.GetHour(),
				dtTimeLastFlowCal.GetMinute(),
				dtTimeLastFlowCal.GetSecond());
		}
	}

	DrawFlowSensor(true,true);

	if(getModel()->getDATAHANDLER()->GetOxyCalRunning()==true)
	{
		ShowFlowBtns(true);
	}
	else
		ShowFlowBtns(false);


	SetTimer(FLOWSENS_VALUE, 100, NULL);

	
}


void CSubViewCalFlowSensor::OnPaint()
{
	CPaintDC dc(this);
	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,m_hDC,0,0,SRCCOPY);
}

void CSubViewCalFlowSensor::OnDestroy()
{
	m_bExit=true;
	StopFlowSensorCheckThread();
	getModel()->getDATAHANDLER()->SetFlowSensorCalibrating(false);

	//KillTimer(CHANGETIMER);
	KillTimer(FLOWSENS_VALUE);

	if(m_pDlg)
	{
		m_pDlg->EndDialog(IDCLOSE);
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_REDRAW_VIEW);
	}
	m_pDlg = NULL;

	m_plMenuBtn.RemoveAll();

	// **********************************************************************
	CWnd::OnDestroy();

	if(m_hBmp)
	{
		SelectObject(m_hDC,m_hBmpPrev);
		DeleteObject(m_hBmp);
	}
	if(m_hDC)
		DeleteDC(m_hDC);

	if(m_hdcStatic)
		DeleteDC(m_hdcStatic);

	if(m_hdcTmp)
		DeleteDC(m_hdcTmp);

	if(m_hbmpStatic)
		DeleteObject(m_hbmpStatic);

	if(m_hbmpTmp)
		DeleteObject(m_hbmpTmp);
}

// **************************************************************************
// 
// **************************************************************************
void CSubViewCalFlowSensor::Show(BOOL bShow) 
{
	if(this->IsWindowVisible() == bShow)
	{
		return;
	}
	if(bShow)
	{
		ShowWindow(SW_SHOW);
	}
	else
	{
		ShowWindow(SW_HIDE);
	}
}


// **************************************************************************
// 
// **************************************************************************
void CSubViewCalFlowSensor::SetFlowSensorText(CStringW szTextTop, CStringW szTextBot)
{
	m_szTextFlowSensorTop=szTextTop;
	m_szTextFlowSensorBot=szTextBot;
}

// **************************************************************************
// 
// **************************************************************************
void CSubViewCalFlowSensor::RefreshFlowSensorText(CStringW szTextTop, CStringW szTextBot)
{
	if(m_bExit)
		return;

	m_szTextFlowSensorTop=szTextTop;
	m_szTextFlowSensorBot=szTextBot;

	DrawFlowSensor(false,true);
}



// **************************************************************************
// 
// **************************************************************************
void CSubViewCalFlowSensor::SetFlowSensorValues(int iHval,int iLval,int iHdac,int iLdac)
{
	if(m_bExit)
		return;
	m_iFlowHValue = iHval;
	m_iFlowLValue = iLval;

	m_iFlowHDAC=iHdac;
	m_iFlowLDAC=iLdac;
}
// **************************************************************************
// 
// **************************************************************************
void CSubViewCalFlowSensor::RefreshFlowSensorValues(int iHval,int iLval,int iHdac,int iLdac)
{
	if(m_bExit)
		return;
	m_iFlowHValue = iHval;
	m_iFlowLValue = iLval;

	m_iFlowHDAC=iHdac;
	m_iFlowLDAC=iLdac;
	DrawFlowSensor(false,true);
}


// **************************************************************************
// 
// **************************************************************************
void CSubViewCalFlowSensor::DrawFlowSensor(bool bStatic, bool bText)
{
	if(m_bExit)
		return;

	CClientDC dc(this);

	if(bStatic)
		DrawStatic();

	BitBlt(m_hdcTmp, 0, 0, m_lX, m_lY,m_hdcStatic , 0, 0, SRCCOPY);

	if(bText)
		DrawCalText();

	CDC* pDCTemp=CDC::FromHandle(m_hdcTmp);
	
	HPEN hpenprev=(HPEN)SelectObject(m_hdcTmp,(HPEN)GetStockObject(NULL_PEN));

	CPen cpenRed;//rkuNEWFIX
	CPen cpenGreen;//rkuNEWFIX

	cpenRed.CreatePen(PS_SOLID,4,RGB(255,0,0));
	cpenGreen.CreatePen(PS_SOLID,4,RGB(0,255,0));
	
	int nBkMode=SetBkMode(m_hdcTmp,TRANSPARENT);
	HFONT hPrevFont=(HFONT)SelectObject(m_hdcTmp,g_hf10AcuBold);
	int tc=SetTextColor(m_hdcTmp,0x0000000);

	CStringW cs = _T("");

	if(getModel()->getDATAHANDLER()->showFlowAC())
	{
		RECT rc;
		rc.left = 577;  
		rc.top = 50;  
		rc.right  = 617;  
		rc.bottom = 72;
		cs.Format(_T("%d"), m_iFlowHDAC);
		pDCTemp->DrawText(cs,-1,&rc,DT_RIGHT|DT_TOP|DT_SINGLELINE);

		rc.left = 577;  
		rc.top = 82;  
		rc.right  = 617;  
		rc.bottom = 107;
		cs.Format(_T("%d"), m_iFlowLDAC);
		pDCTemp->DrawText(cs,-1,&rc,DT_RIGHT|DT_TOP|DT_SINGLELINE);
	}
	



	if(m_iFlowHValue==0)
		SelectObject(m_hdcTmp,(HPEN)cpenGreen);
	else
		SelectObject(m_hdcTmp,(HPEN)cpenRed);
	MoveToEx(m_hdcTmp,699+m_iFlowHValue, 47, NULL);	//Mitte= 452, rechts/links = +-70
	LineTo(m_hdcTmp, 699+m_iFlowHValue, 72);

	if(m_iFlowLValue==0)
		SelectObject(m_hdcTmp,(HPEN)cpenGreen);
	else
		SelectObject(m_hdcTmp,(HPEN)cpenRed);
	MoveToEx(m_hdcTmp,699+m_iFlowLValue, 72, NULL);	//Mitte
	LineTo(m_hdcTmp, 699+m_iFlowLValue, 97);


	SelectObject(m_hdcTmp,(HPEN)GetStockObject(NULL_PEN));

	if(m_bCalRunning)
	{
		//hourglass 
		if(m_iWaitCount>8)
			m_iWaitCount=1;

		switch(m_iWaitCount)
		{
		case 1:
			{
				m_pcWait=m_pcWait1;
			}
			break;
		case 2:
			{
				m_pcWait=m_pcWait2;
			}
			break;
		case 3:
			{
				m_pcWait=m_pcWait3;
			}
			break;
		case 4:
			{
				m_pcWait=m_pcWait4;
			}
			break;
		case 5:
			{
				m_pcWait=m_pcWait5;
			}
			break;
		case 6:
			{
				m_pcWait=m_pcWait6;
			}
			break;
		case 7:
			{
				m_pcWait=m_pcWait7;
			}
			break;
		case 8:
			{
				m_pcWait=m_pcWait8;
			}
			break;
		default:
			{
				m_pcWait=NULL;
			}
			break;
		}

		if(m_pcWait)
			m_pcWait->Draw(m_hdcTmp,240,78);
	}
	else
	{
		if(m_eFlowCalState==FS_OK || m_eFlowCalState==FS_CALIBRATED)
		{
			if(m_pcOk)
				m_pcOk->Draw(m_hdcTmp,240,71);
		}
		else
		{
			if(m_pcExclamation)
				m_pcExclamation->Draw(m_hdcTmp,240,65);
		}
	}

		
	BitBlt(dc.m_hDC, 0, 0, m_lX, m_lY, m_hdcTmp, 0, 0, SRCCOPY);
	BitBlt(m_hDC, 0, 0, m_lX, m_lY, m_hdcTmp, 0, 0, SRCCOPY);	

	SetTextColor(m_hdcTmp,tc);
	SetBkMode(m_hdcTmp,nBkMode);

	SelectObject(m_hdcTmp,hPrevFont);
	SelectObject(m_hdcTmp,hpenprev);

	cpenRed.DeleteObject();//rkuNEWFIX
	cpenGreen.DeleteObject();//rkuNEWFIX

}


// **************************************************************************
// 
// **************************************************************************
void CSubViewCalFlowSensor::DrawStatic()
{
	if(m_bExit)
		return;

	CDC* pDCStatic=CDC::FromHandle(m_hdcStatic);

	HFONT hPrevFont=(HFONT)SelectObject(m_hdcStatic,g_hf25AcuMed);
	int nBkMode=SetBkMode(m_hdcStatic,TRANSPARENT);
	int nPrevTxtColor=SetTextColor(m_hdcStatic,RGB(0,0,0));

	RECT rc;
	//memcpy(&rc,&m_rcClient,sizeof(RECT));

	rc.top = 20;
	rc.bottom = m_lY;
	rc.left = 20; 
	rc.right = m_lX;

	CStringW sz=getModel()->GetLanguageString(IDS_TXT_FLOWSENSOR);
	pDCStatic->DrawText(sz,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	CSize sz1;
	sz1 = pDCStatic->GetTextExtent(sz);

	/*rc.top = 20;
	rc.bottom = m_lY;
	rc.left = 20+sz1.cx+40; 
	rc.right = m_lX;

	if(getModel()->getCONFIG()->GetRefFlowSensor()==RF_NTPD)
	{
		pDC->DrawText(_T("(") + getModel()->GetLanguageString(IDS_TXT_REF) + _T(": ") + getModel()->GetLanguageString(IDS_TXT_NTPD) + _T(")"),&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	}
	else
	{
		pDC->DrawText(_T("(") + getModel()->GetLanguageString(IDS_TXT_REF) + _T(": ") + getModel()->GetLanguageString(IDS_TXT_BTPS) + _T(")"),&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	}*/


	if(m_pcCalibration)
		m_pcCalibration->Draw(m_hdcStatic,627,40);
	
	CPen penLine;
	penLine.CreatePen(PS_SOLID,1,RGB(140,140,140));
	HPEN hpenprev=(HPEN)SelectObject(m_hdcStatic,(HPEN)penLine);
	CBrush cbrRound(RGB(200,200,200));
	HBRUSH hbrprev=(HBRUSH)SelectObject(m_hdcStatic,cbrRound);

	RoundRect(m_hdcStatic, 15, 70, 210, 335,20,20);

	rc.top = 80;
	rc.bottom = m_lY;
	rc.left = 35; 
	rc.right = m_lX;

	SelectObject(m_hdcStatic,g_hf11AcuBold);
	//sz=_T("Range");
	sz=getModel()->GetLanguageString(IDS_TXT_RANGE);
	pDCStatic->DrawText(sz,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

	rc.top = 130;
	rc.bottom = m_lY;
	rc.left = 115; 
	rc.right = m_lX;

	SelectObject(m_hdcStatic,g_hf8AcuBold);
	sz=_T("300g - 10kg");
	pDCStatic->DrawText(sz,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

	rc.top = 185;
	rc.bottom = m_lY;
	rc.left = 115; 
	rc.right = m_lX;

	sz=_T("> 10kg");
	pDCStatic->DrawText(sz,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);


	MoveToEx(m_hdcStatic, 62, 232, NULL);
	LineTo(m_hdcStatic, 162, 232);

	MoveToEx(m_hdcStatic, 62, 295, NULL);
	LineTo(m_hdcStatic, 162, 295);

	rc.left = 62;  
	rc.top = 300;  
	rc.right  = 162;  
	rc.bottom = 315;
	//DrawText(m_hdcStatic,_T("bodyweight"),-1,&rc,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	DrawText(m_hdcStatic,getModel()->GetLanguageString(IDS_TXT_BODYWEIGHT),-1,&rc,DT_CENTER|DT_VCENTER|DT_SINGLELINE);//todo FOTLANGUAGE


	SelectObject(m_hdcStatic,hbrprev);
	SelectObject(m_hdcStatic,hpenprev);

	penLine.DeleteObject();

	SelectObject(m_hdcStatic,hPrevFont);
	SetBkMode(m_hdcStatic,nBkMode);
	SetTextColor(m_hdcStatic,nPrevTxtColor);
}

// **************************************************************************
// 
// **************************************************************************
void CSubViewCalFlowSensor::DrawCalText()
{
	if(m_bExit)
		return;

	CDC* pDCTemp=CDC::FromHandle(m_hdcTmp);

	HFONT hPrevFont=(HFONT)SelectObject(m_hdcTmp,g_hf19AcuMed);
	int nBkMode=SetBkMode(m_hdcTmp,TRANSPARENT);
	int nPrevTxtColor=SetTextColor(m_hdcTmp,RGB(239,181,0));

	if(		/*m_bCalRunning 
		||	*/m_eFlowCalState==FS_UNKNOWN
		||	m_eFlowCalState==FS_OFF
		||	m_eFlowCalState==FS_NOTCONNECTED
		||	m_eFlowCalState==FS_FLOW_DEFECT
		||	m_eFlowCalState==FS_FLOW_A_DEFECT
		||	m_eFlowCalState==FS_FLOW_B_DEFECT
		||	m_eFlowCalState==FS_FLOW_AB_DEFECT
		||	m_eFlowCalState==FS_CLEAN
		||	m_eFlowCalState==FS_NOT_CALIBRATED
		||	m_eFlowCalState==FS_CHECKING
		||	m_eFlowCalState==FS_CALIBRATING
		||	m_eFlowCalState==FS_CAL_CNCL
		||	m_eFlowCalState==FS_CAL_NOTPOSSIBLE)
	{
		pDCTemp->SetTextColor(RGB(255,0,0));
	}
	else
		pDCTemp->SetTextColor(RGB(0,255,0));

	RECT rc;

	//test
	//m_szTextFlowSensorBot=getModel()->GetLanguageString(IDS_TXT_REPLACECAL);
	if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_AUTOOFF)
	{
		m_szTextFlowSensorBot=_T("nebulizer is running");
	}

	if(m_szTextFlowSensorBot==_T(""))
	{
		rc.top = 80;
		rc.bottom = m_lY;
		rc.left = 310; 
		rc.right = m_lX;
		pDCTemp->DrawText(m_szTextFlowSensorTop,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	}
	else
	{
		rc.top = 65;
		rc.bottom = m_lY;
		rc.left = 310; 
		rc.right = m_lX;
		pDCTemp->DrawText(m_szTextFlowSensorTop,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

		rc.top = 95;
		rc.bottom = m_lY;
		rc.left = 310; 
		rc.right = m_lX;
		pDCTemp->DrawText(m_szTextFlowSensorBot,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	}

	SelectObject(m_hdcTmp,g_hf11AcuBold);
	SetTextColor(m_hdcTmp,RGB(0,0,0));

	rc.top = 180;
	rc.bottom = m_lY;
	rc.left = 240; 
	rc.right = m_lX;

	CStringW sz=getModel()->GetLanguageString(IDS_TXT_LASTFLOWCAL);
	//CStringW sz=_T("Last calibration:");
	sz+=_T(" ");
	sz+=m_szLastFlowCal;

	if(!m_bFlowsensor)
	{
		pDCTemp->DrawText(sz,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	}
	else if(m_bInfantFlow)
	{
		pDCTemp->DrawText(sz,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	}
	else
	{
		rc.top = 150;
		rc.bottom = m_lY;
		rc.left = 240;//50; 
		rc.right = m_lX;
		pDCTemp->DrawText(sz,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

		sz=getModel()->GetLanguageString(IDS_TXT_CHECKFLOWSENSOR);

		if(m_pcExclamation)
			m_pcExclamation->Draw(m_hdcTmp,240,170);

		SelectObject(m_hdcTmp,g_hf19AcuMed);
		pDCTemp->SetTextColor(RGB(255,0,0));

		//rc.top = 190;
		rc.top = 185;
		rc.bottom = m_lY;
		rc.left = 310; 
		//rc.right = m_lX;
		rc.right = 580;
		pDCTemp->DrawText(sz,&rc,DT_TOP|DT_WORDBREAK|DT_LEFT);

	}

	SelectObject(m_hdcTmp,hPrevFont);
	SetBkMode(m_hdcTmp,nBkMode);
	SetTextColor(m_hdcTmp,nPrevTxtColor);
}


// **************************************************************************
// 
// **************************************************************************
void CSubViewCalFlowSensor::SetCalibrationOk(bool bCalibrated)
{
	if(m_bExit)
		return;

	if(bCalibrated)
		m_eFlowCalState=FS_CALIBRATED;
	else
		m_eFlowCalState=FS_OK;

	SYSTEMTIME st;
	GetLocalTime(&st);
	COleDateTime dtTimeLastCalFlow(st);

	CStringW szTimeLastCalFlow = dtTimeLastCalFlow.Format(LOCALE_NOUSEROVERRIDE,LANG_USER_DEFAULT);

	getModel()->getCONFIG()->SetLastCalFlow(szTimeLastCalFlow);

	if(false==dtTimeLastCalFlow.ParseDateTime(szTimeLastCalFlow,LOCALE_NOUSEROVERRIDE))
	{
		//m_szLast21Cal=_T("- error -");
		m_szLastFlowCal=getModel()->GetLanguageString(IDS_TXT_UNKNOWN);
		theApp.getLog()->WriteLine(_T("***last Flowcal: unknown***"));
	}
	else
	{
		m_szLastFlowCal.Format(_T("%02d.%02d.%04d %02d:%02d:%02d"),
			dtTimeLastCalFlow.GetDay(),
			dtTimeLastCalFlow.GetMonth(),
			dtTimeLastCalFlow.GetYear(),
			dtTimeLastCalFlow.GetHour(),
			dtTimeLastCalFlow.GetMinute(),
			dtTimeLastCalFlow.GetSecond());
		CStringW szLastFlowCal=_T("***last Flowcal: ");
		szLastFlowCal+=m_szLastFlowCal;
		szLastFlowCal+=_T("***");
		theApp.getLog()->WriteLine(szLastFlowCal);
	}

	CStringW sz=_T("");
	SetFlowSensorText(getModel()->GetLanguageString(IDS_TXT_CALIBRATED),sz);

	DrawFlowSensor(false,true);
}

// **************************************************************************
// 
// **************************************************************************
void CSubViewCalFlowSensor::SetCalState(eFlowSensCalibrationstate state,bool bRedraw)
{
	if(m_bExit)
		return;

	m_eFlowCalState=state;
	getModel()->getDATAHANDLER()->SetFlowSensorCalState(state);

	CStringW csSensorTextTop=getModel()->GetLanguageString(IDS_TXT_UNKNOWN);
	CStringW csSensorTextBot=_T("");

	switch(m_eFlowCalState)
	{
	case FS_OK:
		{
			csSensorTextTop=getModel()->GetLanguageString(IDS_TXT_CONNECTED);
		}
		break;
	case FS_OFF:
		{
			csSensorTextTop=getModel()->GetLanguageString(IDS_TXT_SENSOROFF);
		}
		break;
	case FS_NOTCONNECTED:
		{
			csSensorTextTop=getModel()->GetLanguageString(IDS_TXT_NOTCONNECTED);
		}
		break;
	case FS_FLOW_DEFECT:
		{
			csSensorTextTop=getModel()->GetLanguageString(IDS_TXT_DEFECTSENS_TOP);
			csSensorTextTop+=_T(" ");
			csSensorTextTop+=getModel()->GetLanguageString(IDS_TXT_DEFECTSENS_BOT);
		}
		break;
	case FS_FLOW_A_DEFECT:
		{
			//csSensorText=_T("Hitzdraht A defekt!");
			csSensorTextTop=getModel()->GetLanguageString(IDS_ERR_FLOW_DEF);
			//csSensorTextTop=getModel()->GetLanguageString(IDS_ERR_FLOWA_DEF);
		}
		break;
	case FS_FLOW_B_DEFECT:
		{
			//csSensorText=_T("Hitzdraht B defekt!");
			csSensorTextTop=getModel()->GetLanguageString(IDS_ERR_FLOW_DEF);
			//csSensorTextTop=getModel()->GetLanguageString(IDS_ERR_FLOWB_DEF);
		}
		break;
	case FS_FLOW_AB_DEFECT:
		{
			//csSensorText=_T("beide Hitzdrähte defekt!");
			csSensorTextTop=getModel()->GetLanguageString(IDS_ERR_FLOW_DEF);
			//csSensorTextTop=getModel()->GetLanguageString(IDS_ERR_FLOWAB_DEF);
		}
		break;
	case FS_CLEAN:
		{
			//csSensorText=_T("reinigen!");
			csSensorTextTop=getModel()->GetLanguageString(IDS_TXT_CLEAN);
			csSensorTextBot=getModel()->GetLanguageString(IDS_TXT_REPLACECAL);
		}
		break;
	case FS_CAL_CNCL:
		{
			csSensorTextTop=getModel()->GetLanguageString(IDS_TXT_CALCNCL);
		}
		break;
	case FS_NOT_CALIBRATED:
		{
			csSensorTextTop=getModel()->GetLanguageString(IDS_TXT_NOTCALIBRATED);
		}
		break;
	case FS_CALIBRATING:
		{
			csSensorTextTop=getModel()->GetLanguageString(IDS_TXT_CALRUNNING);
		}
		break;
	case FS_CHECKING:
		{
			csSensorTextTop=getModel()->GetLanguageString(IDS_TXT_CHCKSENSOR);
		}
		break;
	case FS_CALIBRATED:
		{
			csSensorTextTop=getModel()->GetLanguageString(IDS_TXT_CALIBRATED);
		}
		break;
	case FS_CAL_NOTPOSSIBLE:
		{
			csSensorTextTop=getModel()->GetLanguageString(IDS_TXT_CAL_NOTPOSSIBLE);
		}
		break;
	case FS_UNKNOWN:
	default:
		break;
	}

	SetFlowSensorText(csSensorTextTop,csSensorTextBot);

	if(bRedraw)
	{
		DrawFlowSensor(false,true);
	}
}

// **************************************************************************
// 
// **************************************************************************
bool CSubViewCalFlowSensor::CheckFlowSensorState(bool bRedraw, bool bWrite, bool bCalibrating, bool bDrawError)
{
	if(m_bExit)
		return false;

	if(m_bCkeckRunning)
		return 0;

	bool bRes=false;
	eFlowSensCalibrationstate eFlowCalState=FS_CHECKING;

	int iState=getModel()->getSPI()->Read_STATUS_FLOWSENSOR();
	/*Bit 1 = 1   Sensor nicht angeschlossen
	Bit 4 = 1   Hitzdraht A defekt
	Bit 5 = 1   Hitzdraht B defekt
	Bit 6 = 1   Sensor reinigen*/

	if(iState & BIT0)
	{
		eFlowCalState=FS_OFF;
	}
	else if(iState & BIT1)
	{
		eFlowCalState=FS_NOTCONNECTED;
	}
	else if(iState & BIT2)
	{
		eFlowCalState=FS_CAL_NOTPOSSIBLE;
	}
	else if(iState & BIT3)
	{
		eFlowCalState=FS_NOT_CALIBRATED;
	}
	else if((iState & BIT4)&&(iState & BIT5))
	{
		eFlowCalState=FS_FLOW_AB_DEFECT;
	}
	else if(iState & BIT4)
	{
		eFlowCalState=FS_FLOW_A_DEFECT;
	}
	else if(iState & BIT5)
	{
		eFlowCalState=FS_FLOW_B_DEFECT;
	}
	else if(iState & BIT6)
	{
		eFlowCalState=FS_CLEAN;
	}
	else
	{
		bRes=true;
		if(bCalibrating)
			eFlowCalState=FS_CALIBRATED;
		else
			eFlowCalState=FS_OK;
	}
	
	if(bWrite)
		SetCalState(eFlowCalState,bRedraw);
	else if(bDrawError && !bRes)
		SetCalState(eFlowCalState,true);
	

	return bRes;
}

// **************************************************************************
// 
// **************************************************************************
void CSubViewCalFlowSensor::SetCalRunning(bool state)
{
	if(m_bExit)
		return;
	if(state)
	{
		m_iWaitCount=1;
		KillTimer(FLOWSENS_VALUE);
		SetTimer(FLOWSENS_VALUE, 150, NULL);
	}
	else
	{
		m_iWaitCount=0;
		KillTimer(FLOWSENS_VALUE);
		SetTimer(FLOWSENS_VALUE, 100, NULL);
	}

	m_bCalRunning=state;
}

// **************************************************************************
// 
// **************************************************************************
void CSubViewCalFlowSensor::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==FLOWSENS_VALUE)
	{
		if(m_iWaitCount>0)
		{
			m_iWaitCount++;
		}
		else if(!m_bExit)
		{
			SHORT iFlowA=getModel()->getSPI()->Read_FLOWCAL_DEV_A();
			SHORT iFlowB=getModel()->getSPI()->Read_FLOWCAL_DEV_B();

			SHORT iHighDAC=getModel()->getSPI()->Read_DAC_FLOW_INSP();
			SHORT iLowDAC=getModel()->getSPI()->Read_DAC_FLOW_EXH();

			SetFlowSensorValues(iFlowA,iFlowB,iHighDAC,iLowDAC);
		}

		if(!m_bExit)
			DrawFlowSensor(false,true);
		
	}
	/*else if(nIDEvent==CHANGETIMER)
	{
		switch(m_eTimeChanger)
		{
		case TC_BODYWEIGHT_UP:
			{
				if(m_iBodyweight<BODYWEIGHTMAXIMUM)
				{
					if(m_iBodyweight==0)
					{
						m_iBodyweight=BODYWEIGHTMINIMUM;
					}
					else if(m_iBodyweight>=BODYWEIGHTRANGE2)
					{
						m_iBodyweight+=STEP3_BODYWEIGHT;
					}
					else if(m_iBodyweight>=BODYWEIGHTRANGE1)
					{
						m_iBodyweight+=STEP2_BODYWEIGHT;
					}
					else
					{
						m_iBodyweight+=STEP1_BODYWEIGHT;
					}

					CStringW strBodyweight;
					CString szUnit=_T("g");
					if(m_iBodyweight>=BODYWEIGHTUNITSWITCH)
					{
						szUnit=_T("kg");
					}

					if(m_iBodyweight>=BODYWEIGHTUNITSWITCH)
						strBodyweight.Format(_T("%0.1f %s"),CTlsFloat::Round(((double)m_iBodyweight)/1000, 1),szUnit);
					else
						strBodyweight.Format(_T("%d %s"),m_iBodyweight,szUnit);
					m_pbtnBodyweight->RefreshText(strBodyweight);
					getModel()->getDATAHANDLER()->SetBodyweight(m_iBodyweight);

				}
				else
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_ENDRANGE);
				}
			}
			break;
		case TC_BODYWEIGHT_DW:
			{
				CStringW strBodyweight=_T("--");
				if(m_iBodyweight>BODYWEIGHTMINIMUM)
				{
					if(m_iBodyweight<=BODYWEIGHTRANGE1)
					{
						m_iBodyweight-=STEP1_BODYWEIGHT;
					}
					else if(m_iBodyweight<=BODYWEIGHTRANGE2)
					{
						m_iBodyweight-=STEP2_BODYWEIGHT;
					}
					else
					{
						m_iBodyweight-=STEP3_BODYWEIGHT;
					}

					CString szUnit=_T("g");
					if(m_iBodyweight>=BODYWEIGHTUNITSWITCH)
					{
						szUnit=_T("kg");
					}

					if(m_iBodyweight>=BODYWEIGHTUNITSWITCH)
						strBodyweight.Format(_T("%0.1f %s"),CTlsFloat::Round(((double)m_iBodyweight)/1000, 1),szUnit);
					else
						strBodyweight.Format(_T("%d %s"),m_iBodyweight,szUnit);
					m_pbtnBodyweight->RefreshText(strBodyweight);
					getModel()->getDATAHANDLER()->SetBodyweight(m_iBodyweight);

				}
				else
				{
					m_iBodyweight=0;
					m_pbtnBodyweight->RefreshText(strBodyweight);
					getModel()->getDATAHANDLER()->SetBodyweight(m_iBodyweight);
				}
			}
			break;
		default:
			{

			}
			break;
		}

		m_iCounter++;
	}*/

	CWnd::OnTimer(nIDEvent);
}



LRESULT CSubViewCalFlowSensor::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	/*case WM_TIMEBTN_DOWN:
		{
			switch(wParam)
			{
			case IDC_BTN_SETUP_NEXTUP:
				{
					m_iCounter=0;
					m_eTimeChanger=TC_BODYWEIGHT_UP;
					SetTimer(CHANGETIMER,TIMECHANGE,NULL);
					m_pbtnBodyweight->SetState(BS_DOWN);

					if(GetParent())
						GetParent()->PostMessage(WM_SET_SETUPTIMER);
					return 1;
				}
				break;
			case IDC_BTN_SETUP_NEXTDW:
				{
					m_iCounter=0;
					m_eTimeChanger=TC_BODYWEIGHT_DW;
					SetTimer(CHANGETIMER,TIMECHANGE,NULL);
					m_pbtnBodyweight->SetState(BS_DOWN);

					if(GetParent())
						GetParent()->PostMessage(WM_SET_SETUPTIMER);
					return 1;
				}
				break;
			default:
				break;
			}
		}
		break;
	case WM_TIMEBTN_UP:
		{
			switch(wParam)
			{
			case IDC_BTN_SETUP_NEXTUP:
				{
					if(m_iCounter==0)
					{						
						if(m_iBodyweight<BODYWEIGHTMAXIMUM)
						{
							if(m_iBodyweight==0)
							{
								m_iBodyweight=BODYWEIGHTMINIMUM;
							}
							else if(m_iBodyweight>=BODYWEIGHTRANGE2)
							{
								m_iBodyweight+=STEP3_BODYWEIGHT;
							}
							else if(m_iBodyweight>=BODYWEIGHTRANGE1)
							{
								m_iBodyweight+=STEP2_BODYWEIGHT;
							}
							else
							{
								m_iBodyweight+=STEP1_BODYWEIGHT;
							}
							
							CStringW strBodyweight;

							CString szUnit=_T("g");
							if(m_iBodyweight>=BODYWEIGHTUNITSWITCH)
							{
								szUnit=_T("kg");
							}

							if(m_iBodyweight>=BODYWEIGHTUNITSWITCH)
								strBodyweight.Format(_T("%0.1f %s"),CTlsFloat::Round(((double)m_iBodyweight)/1000, 1),szUnit);
							else
								strBodyweight.Format(_T("%d %s"),m_iBodyweight,szUnit);
							m_pbtnBodyweight->RefreshText(strBodyweight);
							getModel()->getDATAHANDLER()->SetBodyweight(m_iBodyweight);
						}
						else
						{
							if(AfxGetApp())
								AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_ENDRANGE);
						}
					}
					m_iCounter=0;
					m_eTimeChanger=TC_OFF;
					KillTimer(CHANGETIMER);

					if(GetParent())
						GetParent()->PostMessage(WM_SET_SETUPTIMER);
					return 1;
				}
				break;
			case IDC_BTN_SETUP_NEXTDW:
				{
					if(m_iCounter==0)
					{
						CStringW strBodyweight=_T("--");
						if(m_iBodyweight>BODYWEIGHTMINIMUM)
						{
							if(m_iBodyweight<=BODYWEIGHTRANGE1)
							{
								m_iBodyweight-=STEP1_BODYWEIGHT;
							}
							else if(m_iBodyweight<=BODYWEIGHTRANGE2)
							{
								m_iBodyweight-=STEP2_BODYWEIGHT;
							}
							else
							{
								m_iBodyweight-=STEP3_BODYWEIGHT;
							}

							CString szUnit=_T("g");
							if(m_iBodyweight>=BODYWEIGHTUNITSWITCH)
							{
								szUnit=_T("kg");
							}

							if(m_iBodyweight>=BODYWEIGHTUNITSWITCH)
								strBodyweight.Format(_T("%0.1f %s"),CTlsFloat::Round(((double)m_iBodyweight)/1000, 1),szUnit);
							else
								strBodyweight.Format(_T("%d %s"),m_iBodyweight,szUnit);
							m_pbtnBodyweight->RefreshText(strBodyweight);
							getModel()->getDATAHANDLER()->SetBodyweight(m_iBodyweight);
						}
						else
						{
							m_iBodyweight=0;
							m_pbtnBodyweight->RefreshText(strBodyweight);
							getModel()->getDATAHANDLER()->SetBodyweight(m_iBodyweight);
						}
					}
					m_iCounter=0;
					m_eTimeChanger=TC_OFF;
					KillTimer(CHANGETIMER);

					if(GetParent())
						GetParent()->PostMessage(WM_SET_SETUPTIMER);
					return 1;
				}
				break;
			default:
				break;
			}
		}
		break;*/
	case WM_ALARMSTATE_CHANGED:
		{
			if(!m_bExit)
			{
				if(getModel()->getDATAHANDLER()->GetOxyCalRunning()==true)
				{
					ShowFlowBtns(true);
				}
				else
					ShowFlowBtns(false);
			}

			return 1;
		}
		break;
	case WM_MENUBUTTONDOWN:
		{
			switch(wParam)
			{
			case IDC_BTN_NEO:
			case IDC_BTN_PED:
				{
					if(lParam==CPresetMenuBtn::UP)
					{
						showNeoPedMessage(wParam);
						//SetOneButtonPreset_Freshgas(wParam);
					}
					return 1;
				}
				break;
			case IDC_BTN_CALFLOW_ONOFF:
				{
					if(!m_bExit)
					{
						if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF)
						{
							getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_ON);
							if(m_pcFlowOnOff)
								m_pcFlowOnOff->RefreshText(getModel()->GetLanguageString(IDS_MENU_CAL1),getModel()->GetLanguageString(IDS_MENU_CAL2));
						}
						else if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_MANOFF)
						{
							getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_ON);
							if(m_pcFlowOnOff)
								m_pcFlowOnOff->RefreshText(getModel()->GetLanguageString(IDS_MENU_CAL1),getModel()->GetLanguageString(IDS_MENU_CAL2));
						}
						else if(getModel()->getDATAHANDLER()->GetFlowSensorState()!=FLOWSENSOR_AUTOOFF)
						{
							getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_MANOFF);
							if(m_pcFlowOnOff)
								m_pcFlowOnOff->RefreshText(getModel()->GetLanguageString(IDS_MENU_CAL3),getModel()->GetLanguageString(IDS_MENU_CAL4));

							if(AfxGetApp())
								AfxGetApp()->GetMainWnd()->PostMessage(WM_DEL_BACKUP);
						}

						if(getModel()->getDATAHANDLER()->GetOxyCalRunning()==true)
						{
							ShowFlowBtns(true);
						}
						else
							ShowFlowBtns(false);

						if(GetParent())
							GetParent()->PostMessage(WM_SET_SETUPTIMER);
					}
					
					return 1;
				}
				break;
			case IDC_BTN_CALFLOW_CALEXT:
				{
					if(		getModel()->getALARMHANDLER()->ALARM_Sens_FLOW_SENSOR_DEFECT->getAlarmState()!=AS_ACTIVE
						/*&&	getModel()->getALARMHANDLER()->ALARM_Sens_FLOW_SENSOR_CLEANING->getAlarmState()!=AS_ACTIVE*/
						&&	getModel()->getALARMHANDLER()->ALARM_Sens_FLOW_SENSOR_NOTCONNECTED->getAlarmState()!=AS_ACTIVE
						&& !m_bExit)
					{
						if(		(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF
							||	getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_MANOFF)
							&& getModel()->getDATAHANDLER()->GetFlowSensorState()!=FLOWSENSOR_AUTOOFF)
						{
							getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_ON);
							if(m_pcFlowOnOff)
								m_pcFlowOnOff->RefreshText(getModel()->GetLanguageString(IDS_MENU_CAL1),getModel()->GetLanguageString(IDS_MENU_CAL2));

						}
						PostMessage(WM_CALBTN_DOWN,wParam);
					}
					
					return 1;
				}
				break;
			}
		}
		break;
	case WM_CALBTN_DOWN:
		{
			switch(wParam)
			{
			case IDC_BTN_CALFLOW_CALEXT:
				{
					if(!m_bExit)
					{
						if(		!m_bFlowCheckStarted
							&&	getModel()->getDATAHANDLER()->GetOxyCalRunning()==false
							&&	getModel()->getDATAHANDLER()->IsFlowSensorCalibrating()==false)
						{
							if(GetParent())
								GetParent()->PostMessage(WM_KILL_SETUPTIMER);

							m_bCancelFlowCal=false;
							m_bFlowCheckStarted=true;

							if(m_pcFlowOnOff)
								m_pcFlowOnOff->ShowWindow(SW_SHOW);
							if(m_pcFlowOnOff)
								m_pcFlowOnOff->EnableWindow(FALSE);

							getModel()->getDATAHANDLER()->SetFlowSensorCalibrating(true);
							
							StartFlowSensorCheckThread();
						}
						else if(getModel()->getDATAHANDLER()->GetOxyCalRunning()==false)
						{
							m_bCancelFlowCal=true;
							StopFlowSensorCheckThread();
							//m_bDoFlowCheck=false;

							if(GetParent())
								GetParent()->PostMessage(WM_SET_SETUPTIMER);
						}
					}
					
					return 1;
				}
				break;
			}

			
		}
		break;
	}
	return CWnd::WindowProc(message, wParam, lParam);
}

// **************************************************************************
// 
// **************************************************************************
void CSubViewCalFlowSensor::showNeoPedMessage(int btnID)
{
	SetOneButtonPreset_NeoPed(btnID);
	CStringW szText=_T("");

	if(btnID==IDC_BTN_NEO)
	{
		//szText=_T("This will set the patient range to NEONATAL. Disconnect patient before changing. Flow sensor calibration is required after changing! Continue?");
		szText=getModel()->GetLanguageString(IDS_TXT_SETNEONATAL);
	}
	else if(btnID==IDC_BTN_PED)
	{
		//szText=_T("This will set the patient range to PEDIATRIC. Disconnect patient before changing. Flow sensor calibration is required after changing! Continue?");
		szText=getModel()->GetLanguageString(IDS_TXT_SETPEDIATRIC);
	}

	CDlgMessageBox box(this,getModel()->GetLanguageString(IDS_TXT_VENTRANGE),szText,MB_YESNO,IDB_MSG_CONFIG);
	m_pDlg = &box;
	int iRes=box.DoModal();
	m_pDlg = NULL;

	if(iRes==IDYES)
	{
		SetOneButtonDepressed_NeoPed(btnID);
	}
	else
	{
		if(btnID==IDC_BTN_NEO)
		{
			m_pcNeonatal->SetBtnState(CPresetMenuBtn::UP,true);
		}
		else if(btnID==IDC_BTN_PED)
		{
			m_pcPediatric->SetBtnState(CPresetMenuBtn::UP,true);
		}
	}
	
}
void CSubViewCalFlowSensor::SetOneButtonDepressed_NeoPed(int btnID)
{
	if(btnID==IDC_BTN_NEO)
	{
		m_pcNeonatal->SetBtnState(CPresetMenuBtn::DOWN);
		m_pcPediatric->SetBtnState(CPresetMenuBtn::UP,true);
			

		getModel()->getCONFIG()->SetVentRange(NEONATAL);
	}
	else if(btnID==IDC_BTN_PED)
	{
		m_pcNeonatal->SetBtnState(CPresetMenuBtn::UP,true);
		m_pcPediatric->SetBtnState(CPresetMenuBtn::DOWN);

		getModel()->getCONFIG()->SetVentRange(PEDIATRIC);
	}

	getModel()->getDATAHANDLER()->SetBodyweight(0,true);
	m_pbtnBodyweight->RefreshText(_T("--"));
	//m_bNeoPedConfirmed=false;
}
void CSubViewCalFlowSensor::SetOneButtonPreset_NeoPed(int btnID)
{

	if(btnID==IDC_BTN_NEO)
	{
		m_pcNeonatal->SetBtnState(CPresetMenuBtn::PRESET);
	}
	else if(btnID==IDC_BTN_PED)
	{
		m_pcPediatric->SetBtnState(CPresetMenuBtn::PRESET);
	}

}

// **************************************************************************
// 
// **************************************************************************
void CSubViewCalFlowSensor::ShowFlowBtns(bool disable)
{
	if(m_bExit)
		return;
	if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_AUTOOFF)
	{
		if(m_pcFlowOnOff)
			m_pcFlowOnOff->ShowWindow(SW_SHOW);
		if(m_pcFlowOnOff)
			m_pcFlowOnOff->EnableWindow(FALSE);

		if(m_pcFlowCalExt)
			m_pcFlowCalExt->ShowWindow(SW_SHOW);
		if(m_pcFlowCalExt)
			m_pcFlowCalExt->EnableWindow(FALSE);
	}
	else if(getModel()->getCONFIG()->GetCurMode()==VM_THERAPIE
		||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_THERAPIE
		/*||	getModel()->getCONFIG()->GetCurMode()==VM_NCPAP
		||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_NCPAP*/
		/*||	getModel()->getCONFIG()->GetCurMode()==VM_DUOPAP
		||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_DUOPAP*/)//rku,PRETRIGGER
	{
		if(m_pcFlowOnOff)
			m_pcFlowOnOff->ShowWindow(SW_SHOW);
		if(m_pcFlowOnOff)
			m_pcFlowOnOff->EnableWindow(FALSE);

		if(m_pcFlowCalExt)
			m_pcFlowCalExt->ShowWindow(SW_SHOW);
		if(m_pcFlowCalExt)
			m_pcFlowCalExt->EnableWindow(FALSE);
	}
	else if(	(getModel()->getCONFIG()->GetCurMode()==VM_NCPAP
				||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_NCPAP
				||	getModel()->getCONFIG()->GetCurMode()==VM_DUOPAP
				||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_DUOPAP)
				&& (getModel()->getDATAHANDLER()->GetTubeSet()==TUBE_MEDIJET))//rku,PRETRIGGER
	{
		if(m_pcFlowOnOff)
			m_pcFlowOnOff->ShowWindow(SW_SHOW);
		if(m_pcFlowOnOff)
			m_pcFlowOnOff->EnableWindow(FALSE);

		if(m_pcFlowCalExt)
			m_pcFlowCalExt->ShowWindow(SW_SHOW);
		if(m_pcFlowCalExt)
			m_pcFlowCalExt->EnableWindow(FALSE);
	}
	else if(disable)
	{
		if(m_pcFlowOnOff)
			m_pcFlowOnOff->ShowWindow(SW_SHOW);
		if(m_pcFlowOnOff)
			m_pcFlowOnOff->EnableWindow(FALSE);
		
		if(m_pcFlowCalExt)
			m_pcFlowCalExt->ShowWindow(SW_SHOW);
		if(m_pcFlowCalExt)
			m_pcFlowCalExt->EnableWindow(FALSE);
	}
	else
	{
		if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false)
		{
			if(m_bCalActive)
			{
				if(m_pcFlowOnOff)
					m_pcFlowOnOff->ShowWindow(SW_SHOW);
				if(m_pcFlowOnOff)
					m_pcFlowOnOff->EnableWindow(FALSE);
			}
			else
			{
				if(m_pcFlowOnOff)
					m_pcFlowOnOff->ShowWindow(SW_SHOW);
				if(m_pcFlowOnOff)
					m_pcFlowOnOff->EnableWindow(TRUE);
			}

			if(m_pcFlowCalExt)
				m_pcFlowCalExt->ShowWindow(SW_SHOW);
			if(m_pcFlowCalExt)
				m_pcFlowCalExt->EnableWindow(TRUE);
		}
		else
		{
			if(m_pcFlowOnOff)
				m_pcFlowOnOff->ShowWindow(SW_SHOW);
			if(m_pcFlowOnOff)
				m_pcFlowOnOff->EnableWindow(TRUE);

			if(m_pcFlowCalExt)
				m_pcFlowCalExt->ShowWindow(SW_SHOW);
			if(m_pcFlowCalExt)
				m_pcFlowCalExt->EnableWindow(FALSE);
		}
	}
}


// **************************************************************************
// 
// **************************************************************************
void CSubViewCalFlowSensor::StartFlowSensorCheckThread()
{
	m_bDoFlowCheck=true;

	if(m_pcwtCheckFlow!=NULL)
	{
		delete m_pcwtCheckFlow;
		m_pcwtCheckFlow=NULL;

		if(m_hThreadCheckFlow!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadCheckFlow);
			m_hThreadCheckFlow=INVALID_HANDLE_VALUE;
		}
	}

	m_pcwtCheckFlow=AfxBeginThread(CCheckFlowSensorThread,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	m_hThreadCheckFlow=m_pcwtCheckFlow->m_hThread;
	m_pcwtCheckFlow->m_bAutoDelete = FALSE; 
	m_pcwtCheckFlow->ResumeThread();
}
void CSubViewCalFlowSensor::StopFlowSensorCheckThread()
{
	if(m_bDoFlowCheck)
	{
		m_bDoFlowCheck=false;

		if (WaitForSingleObject(m_pcwtCheckFlow->m_hThread,3000) == WAIT_TIMEOUT)
		{
			theApp.getLog()->WriteLine(_T("#THR:027a"));
			if(!TerminateThread(m_pcwtCheckFlow,0))
			{
				theApp.getLog()->WriteLine(_T("#THR:027b"));
			}
		}
	}
}
static UINT CCheckFlowSensorThread( LPVOID pc )
{
	((CSubViewCalFlowSensor*)pc)->CheckFlowSensor();
	return TRUE;
}

DWORD CSubViewCalFlowSensor::CheckFlowSensor()
{
	if(m_bExit)
		return 0;

	resetFlowCalAlarms();
	SetCalState(FS_CHECKING,true);

	m_bCalActive=true;
	m_bCkeckRunning=true;
	
	int iState=0;
	bool bCheck=true;
	bool bFlowSensorOk=true;

	Sleep(100);
	getModel()->getSPI()->Send_FLOWSENS_CMND(CMD_CHECKFLOWSENSOR);

	SHORT iHighVal=-70;
	SHORT iLowVal=-70;
	SHORT iHighDAC=0;
	SHORT iLowDAC=0;

	BYTE iCheckStart=0;

	//do
	while(m_bDoFlowCheck && !m_bExit && !m_bFlowCalStarted && bFlowSensorOk)
	{	
		Sleep(100);

		if(iCheckStart>20 && bFlowSensorOk && m_bDoFlowCheck && !m_bExit)
		{
			CalibrateFlowSensor();
		}
		else if(m_bDoFlowCheck && !m_bExit)
		{
			if(bCheck)
			{
				iState=getModel()->getSPI()->Read_FLOWSENS_CMND();

				if(iState==0)
				{
					m_bCkeckRunning=false;
					bCheck=false;
					bFlowSensorOk=CheckFlowSensorState(false,false,false,true);
				}
			}

			iHighVal=getModel()->getSPI()->Read_FLOWCAL_DEV_A();
			iLowVal=getModel()->getSPI()->Read_FLOWCAL_DEV_B();
			iHighDAC=getModel()->getSPI()->Read_DAC_FLOW_INSP();
			iLowDAC=getModel()->getSPI()->Read_DAC_FLOW_EXH();

			RefreshFlowSensorValues(iHighVal,iLowVal,iHighDAC,iLowDAC);
		}
		iCheckStart++;
	}

	if(m_bFlowsensor)
	{
		m_bFlowsensor=false;
		getModel()->DeleteFlowsensorFlag();
	}

	if(!m_bExit)
	{
		SetCalRunning(false);
	}

	if(m_bFlowSensorCalibrated && !m_bExit)
	{
		SetCalibrationOk(true);
		m_bFlowSensorCalibrated=false;

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_FLOWSENSOR_CALIBRATED);
	}
	else if(!m_bExit)
	{
		if(m_bCancelFlowCal || (m_bFlowCalStarted==false && m_bDoFlowCheck==false))
		{
			SetCalState(FS_CAL_CNCL,true);
			m_bCancelFlowCal=false;
		}
		else
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_DOUBLESIGNAL);
		}
	}
	
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_CLEAR_TEXT);

	getModel()->getDATAHANDLER()->SetFlowSensorCalibrating(false);

	m_bDoFlowCheck=false;
	m_bFlowCheckStarted=false;
	m_bCancelFlowCal=false;
	m_bFlowCalStarted=false;

	if(!m_bExit)
	{
		if(GetParent())
		{
			GetParent()->PostMessage(WM_SET_SETUPTIMER);
		}

		if(m_pcFlowOnOff)
			m_pcFlowOnOff->ShowWindow(SW_SHOW);
		if(m_pcFlowOnOff)
			m_pcFlowOnOff->EnableWindow(TRUE);

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_FLOWCAL_FINNISHED);
	}

	m_bCalActive=false;

	theApp.getLog()->WriteLine(_T("#THR:flowcal finished"));

	return 0;
}
void CSubViewCalFlowSensor::resetFlowCalAlarms()
{
	getModel()->getALARMHANDLER()->setCalibrationSilent();
	getModel()->getALARMHANDLER()->deleteLowerPrioAlarms(AP_4);

	if(getModel()->getALARMHANDLER()->ALARM_Sens_FLOW_SENSOR_DEFECT->getAlarmState()!=AS_ACTIVE)
	{
		getModel()->getALARMHANDLER()->deleteAlarm(AL_Sens_FLOW_SENSOR_DEFECT);
	}
	if(getModel()->getALARMHANDLER()->ALARM_Sens_FLOW_SENSOR_CLEANING->getAlarmState()!=AS_ACTIVE)
	{
		getModel()->getALARMHANDLER()->deleteAlarm(AL_Sens_FLOW_SENSOR_CLEANING);
	}
	if(getModel()->getALARMHANDLER()->ALARM_Sens_FLOW_SENSOR_NOTCONNECTED->getAlarmState()!=AS_ACTIVE)
	{
		getModel()->getALARMHANDLER()->deleteAlarm(AL_Sens_FLOW_SENSOR_NOTCONNECTED);
	}
}
void CSubViewCalFlowSensor::CalibrateFlowSensor()
{
	if(m_bExit)
		return;

	SHORT iHighVal=0;
	SHORT iLowVal=0;
	SHORT iHighDAC=0;
	SHORT iLowDAC=0;
	int iState=CMD_FLOWCALIBRATION;

	SetCalRunning(true);

	SetCalState(FS_CALIBRATING,false);
	m_bFlowCalStarted=true;

	if(AfxGetApp())
	{
		AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_CLEAR_TEXT);
		AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
	}

	getModel()->getSPI()->Send_FLOWSENS_CMND(iState);

	BYTE iTimeout=0;
	while((iTimeout<50)&&(iState==CMD_FLOWCALIBRATION) && !m_bExit)
	{
		Sleep(100);	

		iState=getModel()->getSPI()->Read_FLOWSENS_CMND();

		iHighVal=getModel()->getSPI()->Read_FLOWCAL_DEV_A();
		iLowVal=getModel()->getSPI()->Read_FLOWCAL_DEV_B();
		iHighDAC=getModel()->getSPI()->Read_DAC_FLOW_INSP();
		iLowDAC=getModel()->getSPI()->Read_DAC_FLOW_EXH();

		RefreshFlowSensorValues(iHighVal,iLowVal,iHighDAC,iLowDAC);
		iTimeout++;
	}

	
	if(iState==0 && !m_bExit)	//zurückgesetzt?
	{
		bool bFlowSensorOk=CheckFlowSensorState(true,true,true);
		
		if(bFlowSensorOk)
		{
			m_bFlowSensorCalibrated=true;
		}
		else
		{
			m_bFlowSensorCalibrated=false;
		}
		iHighVal=getModel()->getSPI()->Read_FLOWCAL_DEV_A();
		iLowVal=getModel()->getSPI()->Read_FLOWCAL_DEV_B();
		iHighDAC=getModel()->getSPI()->Read_DAC_FLOW_INSP();
		iLowDAC=getModel()->getSPI()->Read_DAC_FLOW_EXH();

		SetFlowSensorValues(iHighVal,iLowVal,iHighDAC,iLowDAC);	//sollten jetzt grün und in der mitte sein
	}
}

bool CSubViewCalFlowSensor::IsCalibrationActive()
{
	return m_bCalActive;
}


// **************************************************************************
// 
// **************************************************************************
void CSubViewCalFlowSensor::SetOneButtonDepressed(int btnID)
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

// **************************************************************************
// 
// **************************************************************************
void CSubViewCalFlowSensor::SetAllButtonUnpressed()
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

// **************************************************************************
// 
// **************************************************************************
void CSubViewCalFlowSensor::OnBnClickedCO2()
{
	if(GetParent())
		GetParent()->PostMessage(WM_MENU_CO2);
}

// **************************************************************************
// 
// **************************************************************************
void CSubViewCalFlowSensor::OnBnClickedSPO2()
{
	if(GetParent())
		GetParent()->PostMessage(WM_MENU_SPO2);
}
// **************************************************************************
// 
// **************************************************************************
void CSubViewCalFlowSensor::OnBnClickedO2()
{
	if(GetParent())
		GetParent()->PostMessage(WM_MENU_O2);
}

void CSubViewCalFlowSensor::OnBnClickedBodyweight()
{
	if(m_pbtnBodyweight->GetState()==BS_UP)
		m_pbtnBodyweight->SetState(BS_DOWN);
	else if(m_pbtnBodyweight->GetState()==BS_FOCUSED)
		m_pbtnBodyweight->SetState(BS_DOWN);
	else if(m_pbtnBodyweight->GetState()==BS_DOWN)
	{
		m_pbtnBodyweight->SetState(BS_FOCUSED);
		getModel()->getDATAHANDLER()->SetBodyweight(m_iBodyweight,true);
	}
}

BOOL CSubViewCalFlowSensor::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message)
	{
	case WM_KEYDOWN:
		{
			if(pMsg->wParam==VK_SPACE)
			{
				eBtnState eState = m_pbtnBodyweight->GetState();
				
				if(eState==BS_NONE)
				{
					m_pbtnBodyweight->SetState(BS_FOCUSED);
				}
				else if(eState==BS_UP)
				{
					m_pbtnBodyweight->SetState(BS_DOWN);
				}
				else if(eState==BS_FOCUSED)
				{
					m_pbtnBodyweight->SetState(BS_DOWN);
				}
				else if(eState==BS_DOWN)
				{
					m_pbtnBodyweight->SetState(BS_FOCUSED);
					getModel()->getDATAHANDLER()->SetBodyweight(m_iBodyweight,true);
				}

				if(GetParent())
				{
					GetParent()->PostMessage(WM_SET_SETUPTIMER);
				}
				return 1;
			}
			else if(pMsg->wParam==VK_DOWN)
			{
				if(m_pbtnBodyweight->GetState()==BS_DOWN)
				{
					CStringW strBodyweight=_T("--");
					if(m_iBodyweight>BODYWEIGHTMINIMUM)
					{
						if(m_iBodyweight<=BODYWEIGHTRANGE1)
						{
							m_iBodyweight-=STEP1_BODYWEIGHT;
						}
						else if(m_iBodyweight<=BODYWEIGHTRANGE2)
						{
							m_iBodyweight-=STEP2_BODYWEIGHT;
						}
						else if(m_iBodyweight<=BODYWEIGHTRANGE3)
						{
							m_iBodyweight-=STEP3_BODYWEIGHT;
						}
						else
						{
							m_iBodyweight-=STEP4_BODYWEIGHT;
						}

						CString szUnit=_T("g");
						if(m_iBodyweight>=BODYWEIGHTUNITSWITCH)
						{
							szUnit=_T("kg");
						}

						if(m_iBodyweight>=BODYWEIGHTUNITSWITCH)
							strBodyweight.Format(_T("%0.1f %s"),CTlsFloat::Round(((double)m_iBodyweight)/1000, 1),szUnit);
						else
							strBodyweight.Format(_T("%d %s"),m_iBodyweight,szUnit);
						m_pbtnBodyweight->RefreshText(strBodyweight);
						getModel()->getDATAHANDLER()->SetBodyweight(m_iBodyweight,false);
					}
					else
					{
						m_iBodyweight=0;
						m_pbtnBodyweight->RefreshText(strBodyweight);
						getModel()->getDATAHANDLER()->SetBodyweight(m_iBodyweight,false);
					}
				}
				/*if(getModel()->isSafeTickCountDelayExpired(m_dwLastSetupTimer, 1000))
				{
					m_dwLastSetupTimer=GetTickCount();
					if(GetParent())
						GetParent()->PostMessage(WM_SET_SETUPTIMER);
				}*/
			}
			else if(pMsg->wParam==VK_UP)
			{
				if(m_pbtnBodyweight->GetState()==BS_DOWN)
				{
					if(m_iBodyweight<BODYWEIGHTMAXIMUM)
					{
						if(m_iBodyweight==0)
						{
							m_iBodyweight=BODYWEIGHTMINIMUM;
						}
						else if(m_iBodyweight>=BODYWEIGHTRANGE3)
						{
							m_iBodyweight+=STEP4_BODYWEIGHT;
						}
						else if(m_iBodyweight>=BODYWEIGHTRANGE2)
						{
							m_iBodyweight+=STEP3_BODYWEIGHT;
						}
						else if(m_iBodyweight>=BODYWEIGHTRANGE1)
						{
							m_iBodyweight+=STEP2_BODYWEIGHT;
						}
						else
						{
							m_iBodyweight+=STEP1_BODYWEIGHT;
						}

						CStringW strBodyweight;

						CString szUnit=_T("g");
						if(m_iBodyweight>=BODYWEIGHTUNITSWITCH)
						{
							szUnit=_T("kg");
						}

						if(m_iBodyweight>=BODYWEIGHTUNITSWITCH)
							strBodyweight.Format(_T("%0.1f %s"),CTlsFloat::Round(((double)m_iBodyweight)/1000, 1),szUnit);
						else
							strBodyweight.Format(_T("%d %s"),m_iBodyweight,szUnit);
						m_pbtnBodyweight->RefreshText(strBodyweight);
						getModel()->getDATAHANDLER()->SetBodyweight(m_iBodyweight,false);
					}
					else
					{
						if(AfxGetApp())
							AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_ENDRANGE);
					}
				}
				/*if(getModel()->isSafeTickCountDelayExpired(m_dwLastSetupTimer, 1000))
				{
					m_dwLastSetupTimer=GetTickCount();
					if(GetParent())
						GetParent()->PostMessage(WM_SET_SETUPTIMER);
				}*/
			}
			else
			{
				//handle focus change manual, draw next focused button and return true
				//return 1;
			}
			break;
		}
		break;
	default:
		break;
	}
	return CWnd::PreTranslateMessage(pMsg);
}