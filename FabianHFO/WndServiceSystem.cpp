// WndServiceSystem.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "WndServiceSystem.h"
#include "DlgMessageBox.h"
#include "TlsFile.h"
#include "NumInputDlg.h"

/**********************************************************************************************//**
 * CWndServiceSystem
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CWndServiceSystem, CWnd)

/**********************************************************************************************//**
 * Initializes a new instance of the CWndServiceSystem class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

CWndServiceSystem::CWndServiceSystem():
CWndService()
{
	m_pDlg = NULL;

	m_dataMainboard.m_iConductorChecksum=0;
	m_dataMainboard.m_iMonitorChecksum=0;
	m_dataMainboard.m_iMonitorVersion_x=0;
	m_dataMainboard.m_iMonitorVersion_y=0;
	m_dataMainboard.m_iMonitorVersion_z=0;
	m_dataMainboard.m_iHfoChecksum=0;
	m_dataMainboard.m_szBlenderVersion=_T("");
	
	/*m_pcAlarm_Up=NULL;
	m_pcAlarm_Dw=NULL;
	m_pcAlarm=NULL;

	m_pcResetOpTime=NULL;
	m_pcResetAccuOpTime=NULL;
	m_pcResetHFOOpTime=NULL;

	m_pcInstaller=NULL;*/

	//m_sliderPassword=NULL;
	//m_bServicePswrdDisabled=false;

	m_szKernel.Format(_T("%s %s %s"), getModel()->getCONFIG()->GetKernelVersion(),getModel()->getCONFIG()->GetKernelVersionDate(), getModel()->getCONFIG()->GetKernelAcuVersion());


	m_iSERST0_Data=getModel()->getDATAHANDLER()->GetMSTATUS_ConductorPIC();
	m_iMBDST0_Data=0;	

	getModel()->getDATAHANDLER()->getMainboardData(&m_dataMainboard);

	m_szUniqueID = getModel()->GetUniqueID();

	/*if(getModel()->getALARMHANDLER()->getAlarmSilentState()==ASTATE_SYSTEM_SILENT)
		m_bAlarmOn=false;
	else
		m_bAlarmOn=true;*/

	//m_iHFOSpringType=getModel()->getCONFIG()->GetHFOSpringType();
	m_iMainVersion=getModel()->getCONFIG()->GetMainBoardVersion();

	m_iSystemHWrevision=getModel()->getCONFIG()->getSystemHWrevision();
	m_eDevType=getModel()->getCONFIG()->getDevType();
	m_eFlowSensorType=getModel()->getCONFIG()->getFlowSensorType();
	m_eAVRevision=getModel()->getCONFIG()->getAVRevision();
	m_eHFRevision=getModel()->getCONFIG()->getHFModuleRevision();
}

/**********************************************************************************************//**
 * Finalizes an instance of the CWndServiceSystem class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

CWndServiceSystem::~CWndServiceSystem()
{
	/*if(m_sliderPassword)
		delete m_sliderPassword;
	m_sliderPassword=NULL;

	delete  m_pcAlarm_Up;
	delete  m_pcAlarm_Dw;
	delete  m_pcAlarm;
	delete m_pcResetOpTime;
	delete m_pcResetAccuOpTime;
	delete m_pcResetHFOOpTime;
	delete m_pcInstaller;


	m_pcAlarm_Up=NULL;
	m_pcAlarm_Dw=NULL;
	m_pcAlarm=NULL;
	m_pcResetOpTime=NULL;
	m_pcResetAccuOpTime=NULL;
	m_pcResetHFOOpTime=NULL;
	m_pcInstaller=NULL;*/
}

BEGIN_MESSAGE_MAP(CWndServiceSystem, CWnd)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	/*ON_BN_CLICKED(IDC_BTN_SERVICE_ALARM, &CWndServiceSystem::OnBnClickedAlarm)
	ON_BN_CLICKED(IDC_BTN_INSTALLER, &CWndServiceSystem::OnBnClickedInstaller)
	ON_BN_CLICKED(IDC_BTN_SERVICE_RESETOPTIME, &CWndServiceSystem::OnBnClickedResetOpTime)
	ON_BN_CLICKED(IDC_BTN_SERVICE_RESETOPTIMEBATTERY, &CWndServiceSystem::OnBnClickedResetOpTimeBattery)
	ON_BN_CLICKED(IDC_BTN_SERVICE_RESETOPTIMEHFO, &CWndServiceSystem::OnBnClickedResetOpTimeHFO)
	ON_MESSAGE(WM_BITMAPSLIDER_MOVING, OnMyMessage)*/
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * CWndServiceSystem message handlers
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndServiceSystem::Init()
{
	CClientDC dc(this);

	//m_pcAlarm_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETUP_TODAY_UP);
	//m_pcAlarm_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETUP_TODAY_DW);

	//BTN btn;

	//btn.wID					= IDC_BTN_SERVICE_RESETOPTIME;	
	//btn.poPosition.x		= 220;//
	//btn.poPosition.y		= 365;
	//btn.pcBmpUp				= m_pcAlarm_Up;
	//btn.pcBmpDown			= m_pcAlarm_Dw;
	//btn.pcBmpFocus			= m_pcAlarm_Up;
	//btn.pcBmpDisabled		= m_pcAlarm_Up;
	//btn.dwFormat			= DT_VCENTER|DT_CENTER;
	//m_pcResetOpTime=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	//m_pcResetOpTime->Create(this,g_hf15AcuMed,0);
	//m_pcResetOpTime->SetText(_T("device"));
	//m_pcResetOpTime->ShowWindow(SW_SHOW);

	//btn.wID					= IDC_BTN_SERVICE_RESETOPTIMEBATTERY;	
	//btn.poPosition.x		= 340;//320;
	//btn.poPosition.y		= 365;
	//btn.pcBmpUp				= m_pcAlarm_Up;
	//btn.pcBmpDown			= m_pcAlarm_Dw;
	//btn.pcBmpFocus			= m_pcAlarm_Up;
	//btn.pcBmpDisabled		= m_pcAlarm_Up;
	//btn.dwFormat			= DT_VCENTER|DT_CENTER;
	//m_pcResetAccuOpTime=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	//m_pcResetAccuOpTime->Create(this,g_hf15AcuMed,0);
	//m_pcResetAccuOpTime->SetText(_T("battery"));
	//m_pcResetAccuOpTime->ShowWindow(SW_SHOW);

	//btn.wID					= IDC_BTN_SERVICE_RESETOPTIMEHFO;	
	//btn.poPosition.x		= 460;//450;
	//btn.poPosition.y		= 365;
	//btn.pcBmpUp				= m_pcAlarm_Up;
	//btn.pcBmpDown			= m_pcAlarm_Dw;
	//btn.pcBmpFocus			= m_pcAlarm_Up;
	//btn.pcBmpDisabled		= m_pcAlarm_Up;
	//btn.dwFormat			= DT_VCENTER|DT_CENTER;
	//m_pcResetHFOOpTime=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	//m_pcResetHFOOpTime->Create(this,g_hf15AcuMed,0);
	//m_pcResetHFOOpTime->SetText(_T("hfo"));
	//m_pcResetHFOOpTime->ShowWindow(SW_SHOW);

	//btn.wID					= IDC_BTN_SERVICE_ALARM;	
	//btn.poPosition.x		= 220;
	//btn.poPosition.y		= 465;
	//btn.pcBmpUp				= m_pcAlarm_Up;
	//btn.pcBmpDown			= m_pcAlarm_Dw;
	//btn.pcBmpFocus			= m_pcAlarm_Up;
	//btn.pcBmpDisabled		= m_pcAlarm_Up;
	//btn.dwFormat			= DT_VCENTER|DT_CENTER;
	//m_pcAlarm=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	//m_pcAlarm->Create(this,g_hf15AcuMed,0);
	//if(m_bAlarmOn)
	//	m_pcAlarm->SetText(_T("turn off"));
	//else
	//	m_pcAlarm->SetText(_T("turn on"));
	//m_pcAlarm->ShowWindow(SW_SHOW);

	//btn.wID					= IDC_BTN_INSTALLER;	
	//btn.poPosition.x		= 220;//;586
	//btn.poPosition.y		= 415;
	//btn.pcBmpUp				= m_pcAlarm_Up;
	//btn.pcBmpDown			= m_pcAlarm_Dw;
	//btn.pcBmpFocus			= m_pcAlarm_Up;
	//btn.pcBmpDisabled		= m_pcAlarm_Up;
	//btn.dwFormat			= DT_VCENTER|DT_CENTER;
	//m_pcInstaller=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	//m_pcInstaller->Create(this,g_hf15AcuMed,0);
	//m_pcInstaller->SetText(_T("start"));
	//m_pcInstaller->ShowWindow(SW_SHOW);

	//m_bServicePswrdDisabled=getModel()->getCONFIG()->IsServicePasswordDisabled();

	//m_sliderPassword = new CBitmapSlider();
	//m_sliderPassword->Create(_T(""),WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_NOTIFY|BS_OWNERDRAW, CRect(400,455,549,500), 
	//	this,IDC_SLD_SERVICEPASSWRD);
	//m_sliderPassword->SetBitmapChannel( IDB_SLD_CHAN_GREY, NULL );
	//m_sliderPassword->SetBitmapThumb( IDB_SLD_THUMB_GREY, IDB_SLD_THUMB_ACT_GREY);
	//m_sliderPassword->SetRange( 0, 1 );
	//if(m_bServicePswrdDisabled)
	//{
	//	m_sliderPassword->SetPos( 0 );
	//}
	//else
	//{
	//	m_sliderPassword->SetPos( 1 );
	//}
	//m_sliderPassword->SetMargin( 5, 0, 6, 0 );
	//m_sliderPassword->DrawFocusRect( FALSE );

	m_pcMenuBack->ShowWindow(SW_SHOW);

	SetTimer(SERVICETIMER, 1000, NULL);
}

/**********************************************************************************************//**
 * Executes the destroy action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndServiceSystem::OnDestroy()
{
	KillTimer(SERVICETIMER);

	if(m_pDlg)
	{
		m_pDlg->EndDialog(IDCLOSE);
	}
	m_pDlg = NULL;

	CWndService::OnDestroy();	
}

//void CWndServiceSystem::OnBnClickedResetOpTime()
//{
//	UINT iNum=0;
//	
//	CNumInputDlg pwDlg(this,OPTIME_DEVICE,iNum);
//	m_pDlg = &pwDlg;
//	int iRes=pwDlg.DoModal();
//	m_pDlg = NULL;
//
//
//	if(iRes==IDOK)
//	{
//		getModel()->getDATAHANDLER()->setOpTimeDeviceMin(iNum);
//		/*DWORD iOpMinHFO=getModel()->getDATAHANDLER()->GetOpTimeHFO();
//		DWORD iOpMinBattery=getModel()->getDATAHANDLER()->GetOpTimeBattery();*/
//	}
//
//	UpdateWindow();
//	Draw();
//	/*CDlgMessageBox box(this,_T("Operating Time Device"),_T("This will reset the operating time of the device. Continue?"),MB_YESNO,IDB_MSG_STOP);
//	m_pDlg = &box;
//	int iRes=box.DoModal();
//	m_pDlg = NULL;
//
//	if(iRes==IDCLOSE)
//	{
//		return;
//	}
//
//	UpdateWindow();
//	Draw();
//	if(iRes==IDYES)
//	{
//		theApp.WriteLog(_T("*** Service mode ResetOpTimeDevice() ***"));
//
//		getModel()->getDATAHANDLER()->ResetOpTimeDevice();
//
//		UpdateWindow();
//		Draw();
//	}*/
//}
//
//void CWndServiceSystem::OnBnClickedResetOpTimeBattery()
//{
//	UINT iNum=0;
//
//	CNumInputDlg pwDlg(this,OPTIME_BATTERY,iNum);
//	m_pDlg = &pwDlg;
//	int iRes=pwDlg.DoModal();
//	m_pDlg = NULL;
//
//
//	if(iRes==IDOK)
//	{
//		getModel()->getDATAHANDLER()->setOpTimeBatteryMin(iNum);
//	}
//
//	UpdateWindow();
//	Draw();
//
//	/*CDlgMessageBox box(this,_T("Operating Time Battery"),_T("This will reset the operating time of the battery. Continue?"),MB_YESNO,IDB_MSG_STOP);
//	m_pDlg = &box;
//	int iRes=box.DoModal();
//	m_pDlg = NULL;
//
//	if(iRes==IDCLOSE)
//	{
//		return;
//	}
//
//	UpdateWindow();
//	Draw();
//	if(iRes==IDYES)
//	{
//		theApp.WriteLog(_T("*** Service mode ResetOpTimeBattery() ***"));
//		getModel()->getDATAHANDLER()->ResetOpTimeBattery();
//
//		UpdateWindow();
//		Draw();
//		
//	}*/
//}
//void CWndServiceSystem::OnBnClickedResetOpTimeHFO()
//{
//	UINT iNum=0;
//
//	CNumInputDlg pwDlg(this,OPTIME_HFO,iNum);
//	m_pDlg = &pwDlg;
//	int iRes=pwDlg.DoModal();
//	m_pDlg = NULL;
//
//
//	if(iRes==IDOK)
//	{
//		getModel()->getDATAHANDLER()->setOpTimeHFOMin(iNum);
//	}
//
//	UpdateWindow();
//	Draw();
//	/*CDlgMessageBox box(this,_T("Operating Time HFO Module"),_T("This will reset the operating time of the HFO module. Continue?"),MB_YESNO,IDB_MSG_STOP);
//	m_pDlg = &box;
//	int iRes=box.DoModal();
//	m_pDlg = NULL;
//
//	if(iRes==IDCLOSE)
//	{
//		return;
//	}
//
//	UpdateWindow();
//	Draw();
//	if(iRes==IDYES)
//	{
//		theApp.WriteLog(_T("*** Service mode ResetOpTimeHFO() ***"));
//		getModel()->getDATAHANDLER()->ResetOpTimeHFO();
//
//		UpdateWindow();
//		Draw();
//	}*/
//}

//void CWndServiceSystem::OnBnClickedInstaller()
//{
//	//if(CTlsFile::Exists(_T("\\Hard Disk\\FabianInstaller.exe")))
//	//{
//	//	CDlgMessageBox box(this,_T("UPDATE"),_T("This will start the Fabian Update, Ventilation will be stopped! Do not disconnect the USB-Setup-Stick! Continue?"),MB_YESNO,IDB_MSG_CONFIG);
//	//	m_pDlg = &box;
//	//	int iRes=box.DoModal();
//	//	m_pDlg = NULL;
//
//	//	if(iRes==IDCLOSE)
//	//	{
//	//		return;
//	//	}
//
//	//	if(iRes==IDYES)
//	//	{
//	//		//getModel()->StartInstaller();
//
//	//		theApp.WriteLog(_T("#HFO:0284"));
//	//		
//	//		CString file=_T("\\Hard Disk\\FabianInstaller.exe");
//	//		PROCESS_INFORMATION pi;
//	//		CreateProcess(file, 0, NULL, NULL, NULL, 0, NULL, NULL, NULL, &pi);
//
//	//		???????????????
//	//		CDlgMessageBox box(this,_T("UPDATE"),_T("Update ready to install, please reboot device manual!"),MB_OK,IDB_MSG_CONFIG);
//	//		m_pDlg = &box;
//	//		int iRes=box.DoModal();
//	//		m_pDlg = NULL;
//	//	}
//	//}
//
//	//old installer version
//	if(CTlsFile::Exists(_T("\\Hard Disk\\SETUP\\SetupFabian.exe")))
//	{
//		CDlgMessageBox box(this,_T("SETUP"),_T("This will start the FabianSetup, Ventilation will be stopped! Do not disconnect the USB-Setup-Stick! It will take a minute until the setup starts, continue?"),MB_YESNO,IDB_MSG_CONFIG);
//		m_pDlg = &box;
//		int iRes=box.DoModal();
//		m_pDlg = NULL;
//
//		if(iRes==IDCLOSE)
//		{
//			return;
//		}
//
//		if(iRes==IDYES)
//		{
//			getModel()->StartInstaller();
//
//			theApp.WriteLog(_T("#HFO:0284"));
//			getModel()->SetAccuTurnoff();
//			if(AfxGetApp())
//				AfxGetApp()->GetMainWnd()->PostMessage(WM_VENT_TURNOFF);
//		}
//	}
//}

//void CWndServiceSystem::OnBnClickedAlarm()
//{
//	if(m_bAlarmOn)
//	{
//		theApp.WriteLog(_T("#HFO:0285"));
//		m_bAlarmOn=false;
//		m_pcAlarm->RefreshText(_T("turn on"));
//
//		if(AfxGetApp())
//			AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_SYSALARM_SILENT);
//	}
//	else
//	{
//		theApp.WriteLog(_T("#HFO:0286"));
//		m_bAlarmOn=true;
//		m_pcAlarm->RefreshText(_T("turn off"));
//
//		if(AfxGetApp())
//			AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_SYSALARM_SILENT);
//	}
//
//	Draw();
//}

/**********************************************************************************************//**
 * Draws this instance
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndServiceSystem::Draw()
{
	RECT rcCl;
	RECT rc;
	GetClientRect(&rcCl);

	CClientDC dc(this);
	HDC hdcMem=CreateCompatibleDC(dc.m_hDC);
	HBITMAP hBmpMem=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
	HBITMAP hBmpMemPrev=(HBITMAP)SelectObject(hdcMem,hBmpMem);

	CBrush cbrWhite(RGB(255,255,255));
	CBrush cbrBack(RGB(181,178,181));
	HBRUSH hbrprev=(HBRUSH)SelectObject(hdcMem,cbrBack);
	HPEN hpenprev=(HPEN)SelectObject(hdcMem, (HPEN)GetStockObject(NULL_PEN));	

	Rectangle(hdcMem, 0, 0, m_lX, m_lY);

	int nBkMode=SetBkMode(hdcMem,TRANSPARENT);
	HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf9AcuBold);
	int tc=SetTextColor(hdcMem,0x0000000);
	
	/*************************Software Rev.**********************************/
	rc.left = 40;  
	rc.top = 15;  
	rc.right  = 280;  
	rc.bottom = 500;
	CStringW cs = _T("Software Rev.:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	cs = _T("fabianHFO ");
	cs += getModel()->GetBuildVersion();
	rc.left = 280;  
	rc.top = 15;  
	rc.right  = 780;  
	rc.bottom = 500;
	DrawText(hdcMem,cs,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

	/**************************Ident.Nr**********************************/
	rc.left = 40;  
	rc.top = 35;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("Ident-No:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 280;  
	rc.top = 35;  
	rc.right  = 780;  
	rc.bottom = 500;
	DrawText(hdcMem,m_szUniqueID,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	/**************************Betriebsstunden**********************************/
	rc.left = 40;  
	rc.top = 55;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("Operating hours:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	//getModel()->getDATAHANDLER()->RefreshOpTime(false);
	DWORD iOpMinDevice=getModel()->getDATAHANDLER()->getOpTimeDeviceMin(false);
	DWORD iOpMinHFO=getModel()->getDATAHANDLER()->getOpTimeHFOMin(false);
	DWORD iOpMinBattery=getModel()->getDATAHANDLER()->getOpTimeBatteryMin(false);
	int iOpHrsDevice = iOpMinDevice/60;
	if(iOpHrsDevice>0)
		iOpMinDevice=iOpMinDevice-(iOpHrsDevice*60);

	rc.left = 280;  
	rc.top = 55;  
	rc.right  = 780;  
	rc.bottom = 500;
	cs = _T("Device:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 450;  
	rc.top = 55;  
	rc.right  = 780;  
	rc.bottom = 500;
	cs.Format(_T("%d%s %d%s"),iOpHrsDevice,_T("h"),iOpMinDevice,_T("min"));
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 280;  
	rc.top = 75;  
	rc.right  = 780;  
	rc.bottom = 500;
	cs = _T("Battery:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	int iOpHrsBattery = iOpMinBattery/60;
	if(iOpHrsBattery>0)
		iOpMinBattery=iOpMinBattery-(iOpHrsBattery*60);

	rc.left = 450;  
	rc.top = 75;  
	rc.right  = 780;  
	rc.bottom = 500;
	cs.Format(_T("%d%s %d%s"),iOpHrsBattery,_T("h"),iOpMinBattery,_T("min"));
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 280;  
	rc.top = 95;  
	rc.right  = 780;  
	rc.bottom = 500;
	cs = _T("HFO module:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	int iOpHrsHFO = iOpMinHFO/60;
	if(iOpHrsHFO>0)
		iOpMinHFO=iOpMinHFO-(iOpHrsHFO*60);

	rc.left = 450;  
	rc.top = 95;  
	rc.right  = 780;  
	rc.bottom = 500;
	cs.Format(_T("%d%s %d%s"),iOpHrsHFO,_T("h"),iOpMinHFO,_T("min"));
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	/**************************Interfaces**********************************/
	rc.left = 40;  
	rc.top = 115;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("Interfaces:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 280;  
	rc.top = 115;  
	rc.right  = 780;  
	rc.bottom = 500;
	cs.Format(_T("COM:%d, SPI:%d, I2C-FRAM:%d, I2C-MCP:%d, I2C-ACCU:%d, DIO:%d, FSBUS:%d"), 
		(int)getModel()->isSERIALavailable(), 
		(int)getModel()->isSPIavailabel(), 
		(int)getModel()->getI2C()->IsI2C_FRAMavailability(),
		(int)getModel()->getI2C()->IsI2C_MCPavailability(), 
		(int)getModel()->getI2C()->IsI2C_ACCUavailability(),
		(int)getModel()->IsDIOavailability(), 
		(int)getModel()->IsPIFavailability());
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	/**************************Monitor PIC**********************************/
	rc.left = 40;  
	rc.top = 135;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("Version Monitor PIC:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 280;  
	rc.top = 135;  
	rc.right  = 780;  
	rc.bottom = 500;
	cs.Format(_T("%d.%d.%d, Checksum: %X"), 
		m_dataMainboard.m_iMonitorVersion_x, 
		m_dataMainboard.m_iMonitorVersion_y, 
		m_dataMainboard.m_iMonitorVersion_z, 
		m_dataMainboard.m_iMonitorChecksum);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
	
	/**************************Steuer PIC**********************************/
	rc.left = 40;  
	rc.top = 155;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("Version Conductor PIC:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 280;  
	rc.top = 155;  
	rc.right  = 780;  
	rc.bottom = 500;
	cs.Format(_T("%s, Checksum: %X"), 
		m_dataMainboard.m_szConductorVersion, 
		m_dataMainboard.m_iConductorChecksum);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	/**************************SERST0**********************************/
	rc.left = 40;  
	rc.top = 175;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("State Conductor-PIC:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	BOOL bIPRESSUREBIT=m_iSERST0_Data & IPRESSUREBIT;
	BOOL bDISCONNECTIONBIT=m_iSERST0_Data & DISCONNECTIONBIT;
	BOOL bCHECKSUMERRORBIT=m_iSERST0_Data & CHECKSUMERRORBIT;

	rc.left = 280;  
	rc.top = 175;  
	rc.right  = 780;  
	rc.bottom = 500;

	cs.Format(_T("Inspiration pressure: %d, Disconnection: %d, Checksum failure: %d"), bIPRESSUREBIT, bDISCONNECTIONBIT, bCHECKSUMERRORBIT);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	/**************************Blender PIC**********************************/
	rc.left = 40;  
	rc.top = 195;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("Version Blender PIC:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 280;  
	rc.top = 195;  
	rc.right  = 780;  
	rc.bottom = 500;
	cs.Format(_T("%s"), 
		m_dataMainboard.m_szBlenderVersion);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	/**************************HFO PIC**********************************/
	rc.left = 40;  
	rc.top = 215;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("Version HFO PIC:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 280;  
	rc.top = 215;  
	rc.right  = 780;  
	rc.bottom = 500;
	cs.Format(_T("%s"), 
		m_dataMainboard.m_szHfoVersion);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	/**************************Speicher Status**********************************/
	const char *divisor = "K";
	MEMORYSTATUS stat;
	GlobalMemoryStatus (&stat);

	rc.left = 40;  
	rc.top = 235;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("Tot PhysMem:");	//The amount of actual physical memory
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
	
	rc.left = 280;  
	rc.top = 235;  
	rc.right  = 780;  
	rc.bottom = 500;

	cs.Format(_T("%ld %sbytes"),stat.dwTotalPhys/1024, divisor);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 40;  
	rc.top = 255;  
	rc.right  = 280;  
	rc.bottom = 500;  
	cs = _T("Free PhysMem");	//The amount of physical memory currently available
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 280;  
	rc.top = 255;  
	rc.right  = 780;  
	rc.bottom = 500; 
	cs.Format(_T("%ld %sbytes"),stat.dwAvailPhys/1024, divisor);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	TCHAR*  DirectoryName = TEXT("\\FFSDISK");
	ULARGE_INTEGER   FreeBytesAvailableToCaller;
	ULARGE_INTEGER   TotalNumberOfBytes;
	ULARGE_INTEGER   TotalNumberOfFreeBytes;

	if (!GetDiskFreeSpaceEx(
		DirectoryName,
		&FreeBytesAvailableToCaller, 
		&TotalNumberOfBytes, 
		&TotalNumberOfFreeBytes )) 
	{
		FreeBytesAvailableToCaller.QuadPart = 0;
	}

	rc.left = 40;  
	rc.top = 275;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("Free FFSDISK");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 280;  
	rc.top = 275;  
	rc.right  = 780;  
	rc.bottom = 500;   
	//cs.Format(_T("%*ld kbytes"),7, FreeBytesAvailableToCaller.QuadPart/1024);
	//int iTest = FreeBytesAvailableToCaller.QuadPart/1024;
	//int iTest = TotalNumberOfBytes.QuadPart/1024;
	//iTest = TotalNumberOfFreeBytes.QuadPart/1024;
	cs.Format(_T("%ld kbytes"), FreeBytesAvailableToCaller.QuadPart/1024);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 40;  
	rc.top = 295;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("Total FFSDISK");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 280;  
	rc.top = 295;  
	rc.right  = 780;  
	rc.bottom = 500;   
	cs.Format(_T("%ld kbytes"),TotalNumberOfBytes.QuadPart/1024);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 40;  
	rc.top = 315;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("Version Kernel:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 280;  
	rc.top = 315;  
	rc.right  = 780;  
	rc.bottom = 500; 
	cs=m_szKernel;
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 40;  
	rc.top = 335;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("Hardware configuration: ");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 280;  
	rc.top = 335;  
	rc.right  = 780;  
	rc.bottom = 500;
	CString szTemp=_T("");
	int iMajor=(m_iMainVersion >> 4) & 0x000F;
	int iMinor=m_iMainVersion & 0x000F;
	szTemp.Format(_T("MB%d.%d,"), iMajor,iMinor);
	cs=szTemp;
	//DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	if(getModel()->getDATAHANDLER()->IsLEDdisplay())
	{
		cs+=_T("LED,");
	}
	else
	{
		cs+=_T("LCD,");
	}

	szTemp.Format(_T("SysHW%d,"), m_iSystemHWrevision);
	cs+=szTemp;

	if(m_eDevType==DEV_EVO)
	{
		cs+=_T("EVO,");
	}
	else if(m_eDevType==DEV_THERAPY)
	{
		cs+=_T("THERAPY,");
	}
	else if(m_eDevType==DEV_HFO)
	{
		cs+=_T("HFO,");
	}

	if(m_eFlowSensorType==FSENS_NEONATAL)
	{
		cs+=_T("FlowS:Neo,");
	}
	else if(m_eFlowSensorType==FSENS_PEDIATRIC)
	{
		cs+=_T("FlowS:Ped,");
	}

	if(m_eAVRevision==AVREV_1)
	{
		cs+=_T("ATG:Rev.1-3,");
	}
	else if(m_eAVRevision==AVREV_4)
	{
		cs+=_T("ATG:Rev.4,");
	}

	if(m_eHFRevision==HFREV_1)
	{
		cs+=_T("HF:Type1,");
	}
	else if(m_eHFRevision==HFREV_2)
	{
		cs+=_T("HF:Type2,");
	}

	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	SelectObject(hdcMem,g_hf10AcuBold);

	/**************************System Error Status**********************************/
	//CPen penLine;
	//penLine.CreatePen(PS_SOLID,1,RGB(140,140,140));
	//SelectObject(hdcMem,(HPEN)penLine);
	//CBrush cbrRound(RGB(200,200,200));
	//SelectObject(hdcMem,cbrRound);
	//RoundRect(hdcMem, 10, 350, 580, 515,20,20);

	//rc.left = 20;  
	//rc.top = 365;  
	//rc.right  = 280;  
	//rc.bottom = 500;
	//cs = _T("Set operating");
	//DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	//rc.left = 20;  
	//rc.top = 380;  
	//rc.right  = 280;  
	//rc.bottom = 500;
	//cs = _T("hours:");
	//DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	//rc.left = 20;  
	//rc.top = 425;  
	//rc.right  = 280;  
	//rc.bottom = 500;
	//cs = _T("FabianHFO Installer:");
	//DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	////if(CTlsFile::Exists(_T("\\Hard Disk\\SETUP\\SetupFabian.exe")))
	//if(CTlsFile::Exists(_T("\\Hard Disk\\FabianInstaller.exe")))
	//{
	//	m_pcInstaller->ShowWindow(SW_SHOW);
	//}
	///*else if(CTlsFile::Exists(_T("\\Hard Disk\\SetupFabian.exe")))
	//{
	//	m_pcInstaller->ShowWindow(SW_SHOW);
	//}*/
	//else
	//{
	//	m_pcInstaller->ShowWindow(SW_HIDE);

	//	rc.left = 220;  
	//	rc.top = 425;  
	//	rc.right  = 600;  
	//	rc.bottom = 500; 
	//	cs=_T("not available");
	//	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
	//}

	/**************************Password**********************************/
	/*rc.left = 400;  
	rc.top = 425;  
	rc.right  = 580;  
	rc.bottom = 500;
	cs = _T("Service-Password:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);*/

	/**************************System Alarm Status**********************************/
	/*rc.left = 170;  
	rc.top = 470;  
	rc.right  = 210;  
	rc.bottom = 495;
	FillRect(hdcMem,&rc,cbrWhite);
	
	rc.left = 20;  
	rc.top = 475;  
	rc.right  = 280;  
	rc.bottom = 505;
	cs = _T("System Alarm:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 170;  
	rc.top = 475;  
	rc.right  = 210;  
	rc.bottom = 500;
	if(m_bAlarmOn)
		cs = _T("on");
	else
		cs = _T("off");
	DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);*/
	
	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,hdcMem,0,0,SRCCOPY);

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

/**********************************************************************************************//**
 * Executes the timer action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	nIDEvent	The identifier event.
 **************************************************************************************************/

void CWndServiceSystem::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==SERVICETIMER)
	{
		m_iSERST0_Data=getModel()->getDATAHANDLER()->GetMSTATUS_ConductorPIC();
		m_iMBDST0_Data=0; //von I2C .... todo

		/*bool bTemp=m_bAlarmOn;
		if(getModel()->getALARMHANDLER()->getAlarmSilentState()==ASTATE_SYSTEM_SILENT)
			m_bAlarmOn=false;
		else
			m_bAlarmOn=true;

		if(bTemp!=m_bAlarmOn)
		{
			if(m_bAlarmOn)
			{
				m_pcAlarm->RefreshText(_T("turn on"));
			}
			else
			{
				m_pcAlarm->RefreshText(_T("turn off"));
			}
		}*/

		Draw();
	}

	CWnd::OnTimer(nIDEvent);
}

//LRESULT CWndServiceSystem::OnMyMessage(WPARAM wParam, LPARAM lParam)
//{
//	switch(wParam)
//	{
//	case IDC_SLD_SERVICEPASSWRD:
//		{
//			if(lParam==1)
//			{
//				m_bServicePswrdDisabled=false;
//			}
//			else
//			{
//				m_bServicePswrdDisabled=true;
//			}
//
//			getModel()->getCONFIG()->SetServicePasswordDisabled(m_bServicePswrdDisabled);
//			Draw();
//		}
//		break;
//	}
//	return 1;
//}