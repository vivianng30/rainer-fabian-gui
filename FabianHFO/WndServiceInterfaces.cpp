/**********************************************************************************************//**
 * \file	WndServiceInterfaces.cpp.
 *
 * Implements the window service interfaces class
 **************************************************************************************************/

#include "StdAfx.h"
#include "FabianHFO.h"
#include "WndServiceInterfaces.h"
#include "DlgMessageBox.h"
#include "TlsFile.h"

/**********************************************************************************************//**
 * A macro that defines ioctl HAL reboot
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

#define IOCTL_HAL_REBOOT  0x101003C
extern "C" BOOL KernelIoControl(DWORD,LPVOID,DWORD,LPVOID,DWORD,LPDWORD);

/**********************************************************************************************//**
 * CWndServiceInterfaces
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CWndServiceInterfaces, CWnd)

/**********************************************************************************************//**
 * Initializes a new instance of the CWndServiceInterfaces class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

CWndServiceInterfaces::CWndServiceInterfaces():
CWndService()
{
	m_szNSPIinfo=_T("");
	m_szI2Cinfo=_T("");
	m_szPIFinfo=_T("");

	m_pcUp=NULL;
	m_pcDw=NULL;
	m_pcDis=NULL;

	m_pDlg = NULL;
	//m_pcEraseFRAM=NULL;
	m_pcLogfile=NULL;
	m_pcCloseApp=NULL;
	//m_pcInstallUpdate=NULL;
	//m_pcEnableHFO=NULL;

	m_bUSBavailable=false;
	m_bLOGavailable=false;
	m_bCopyLOGrunning=false;
	m_bCopyLOGfinnished=false;
	m_bCopyLOGsuccess=false;

	m_dataMainboard.m_szConductorVersion=_T("");
	m_dataMainboard.m_iConductorChecksum=0;
	m_dataMainboard.m_iMonitorChecksum=0;
	m_dataMainboard.m_iMonitorVersion_x=0;
	m_dataMainboard.m_iMonitorVersion_y=0;
	m_dataMainboard.m_iMonitorVersion_z=0;
	m_dataMainboard.m_iHfoChecksum=0;
	m_dataMainboard.m_szBlenderVersion=_T("");

	getModel()->getDATAHANDLER()->getMainboardData(&m_dataMainboard);
	m_iSERST0_Data=getModel()->getDATAHANDLER()->GetMSTATUS_ConductorPIC();
	m_byI2CWatchdogState=getModel()->getI2C()->ReadMCP_Port0();
}

/**********************************************************************************************//**
 * Finalizes an instance of the CWndServiceInterfaces class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

CWndServiceInterfaces::~CWndServiceInterfaces()
{
	delete m_pcLogfile;
	m_pcLogfile=NULL;

	delete m_pcCloseApp;
	m_pcCloseApp=NULL;

	/*delete m_pcInstallUpdate;
	m_pcInstallUpdate=NULL;*/

	/*delete m_pcEnableHFO;
	m_pcEnableHFO=NULL;*/
	
	delete m_pcUp;
	m_pcUp=NULL;
	delete m_pcDw;
	m_pcDw=NULL;
	delete m_pcDis;
	m_pcDis=NULL;
}


BEGIN_MESSAGE_MAP(CWndServiceInterfaces, CWnd)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//ON_BN_CLICKED(IDC_BTN_ERASEFRAM, &CWndServiceInterfaces::OnBnClickedEraseFRAM)
	ON_BN_CLICKED(IDC_BTN_CLOSEAPP, &CWndServiceInterfaces::OnBnClickedExitApp)
	ON_BN_CLICKED(IDC_BTN_LOGFILES, &CWndServiceInterfaces::OnBnClickedLogfiles)
	//ON_BN_CLICKED(IDC_BTN_INSTALLER, &CWndServiceInterfaces::OnBnClickedInstaller)
	//ON_BN_CLICKED(IDC_BTN_ENABLEHFO, &CWndServiceInterfaces::OnBnClickedEnableHFO)
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * CWndServiceInterfaces message handlers
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndServiceInterfaces::Init()
{
	CClientDC dc(this);

	m_pcUp= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PAT_UP);
	m_pcDw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PAT_DW);
	m_pcDis= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PAT_DIS);

	/*if(getModel()->getCONFIG()->IsHFOFeatureAvailable()==true)
		m_bHFOenabled=true;
	else
		m_bHFOenabled=false;*/

	BTN btn;

	/*btn.wID					= IDC_BTN_INSTALLER;	
	btn.poPosition.x		= 270;
	btn.poPosition.y		= 220;
	btn.pcBmpUp				= m_pcUp;
	btn.pcBmpDown			= m_pcDw;
	btn.pcBmpFocus			= m_pcUp;
	btn.pcBmpDisabled		= m_pcDis;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcInstallUpdate=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcInstallUpdate->Create(this,g_hf10AcuBold,0);
	m_pcInstallUpdate->SetText(_T("start Installer"));*/


	/*btn.wID					= IDC_BTN_ERASEFRAM;	
	btn.poPosition.x		= 270;
	btn.poPosition.y		= 310;
	btn.pcBmpUp				= m_pcUp;
	btn.pcBmpDown			= m_pcDw;
	btn.pcBmpFocus			= m_pcUp;
	btn.pcBmpDisabled		= m_pcDis;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcEraseFRAM=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcEraseFRAM->Create(this,g_hf10AcuBold,0);
	m_pcEraseFRAM->SetText(_T("set defaults"));*/

	btn.wID					= IDC_BTN_LOGFILES;	
	btn.poPosition.x		= 270;
	btn.poPosition.y		= 380;
	btn.pcBmpUp				= m_pcUp;
	btn.pcBmpDown			= m_pcDw;
	btn.pcBmpFocus			= m_pcUp;
	btn.pcBmpDisabled		= m_pcDis;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcLogfile=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcLogfile->Create(this,g_hf10AcuBold,0);
	m_pcLogfile->SetText(_T("save to USB"));
	if (CTlsFile::Exists(_T("\\Hard Disk")))
	{
		m_bUSBavailable=true;
	}
	else
	{
		m_bUSBavailable=false;
	}

	if (CTlsFile::Exists(_T( "\\sdcard\\Logs\\alarmlog.txt" )))
	{
		m_bLOGavailable=true;
	}
	else if (CTlsFile::Exists(_T( "\\FFSDisk\\Logs\\alarmlog.txt" )))
	{
		m_bLOGavailable=true;
	}
	else
	{
		m_bLOGavailable=false;
	}
	
	


	btn.wID					= IDC_BTN_CLOSEAPP;	
	btn.poPosition.x		= 40;
	btn.poPosition.y		= 480;
	btn.pcBmpUp				= m_pcUp;
	btn.pcBmpDown			= m_pcDw;
	btn.pcBmpFocus			= m_pcUp;
	btn.pcBmpDisabled		= m_pcDis;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcCloseApp=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcCloseApp->Create(this,g_hf10AcuBold,0);
	m_pcCloseApp->SetText(_T("exit app"));

	




	/*btn.wID					= IDC_BTN_ENABLEHFO;	
	btn.poPosition.x		= 40;
	btn.poPosition.y		= 400;
	btn.pcBmpUp				= m_pcUp;
	btn.pcBmpDown			= m_pcDw;
	btn.pcBmpFocus			= m_pcUp;
	btn.pcBmpDisabled		= m_pcDis;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcEnableHFO=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcEnableHFO->Create(this,g_hf10AcuBold,0);
	if(m_bHFOenabled)
		m_pcEnableHFO->SetText(_T("disable HFO"));
	else
		m_pcEnableHFO->SetText(_T("enable HFO"));*/
	

#ifndef _DEBUG
	m_pcCloseApp->ShowWindow(SW_HIDE);
#endif


	m_pcMenuBack->ShowWindow(SW_SHOW);

	m_szNSPIinfo=getModel()->getSPI()->GetDriverInfo();
	m_szI2Cinfo=getModel()->getI2C()->GetDriverInfo();
	

	m_szPIFinfo=getModel()->getPIF()->GetDriverInfo();

	SetTimer(SERVICETIMER, 1000, NULL);

}

/**********************************************************************************************//**
 * Draws this instance
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndServiceInterfaces::Draw()
{
	RECT rcCl;
	RECT rc;
	GetClientRect(&rcCl);

	CClientDC dc(this);
	HDC hdcMem=CreateCompatibleDC(dc.m_hDC);
	HBITMAP hBmpMem=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
	HBITMAP hBmpMemPrev=(HBITMAP)SelectObject(hdcMem,hBmpMem);


	CBrush cbrBack(RGB(181,178,181));
	HBRUSH hbrprev=(HBRUSH)SelectObject(hdcMem,cbrBack);
	HPEN hpenprev=(HPEN)SelectObject(hdcMem, (HPEN)GetStockObject(NULL_PEN));	
	int nBkMode=SetBkMode(hdcMem,TRANSPARENT);
	HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf8AcuBold);
	int tc=SetTextColor(hdcMem,0x0000000);

	Rectangle(hdcMem, 0, 0, m_lX, m_lY);
	
	rc.left = 40;  
	rc.top = 40;  
	rc.right  = 280;  
	rc.bottom = 500;
	CStringW cs = _T("NSPI driver: ");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 280;  
	rc.top = 40;  
	rc.right  = 600;  
	rc.bottom = 500;
	cs=m_szNSPIinfo;
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


	rc.left = 40;  
	rc.top = 70;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("I2C driver: ");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 280;  
	rc.top = 70;  
	rc.right  = 600;  
	rc.bottom = 500;
	cs=m_szI2Cinfo;
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


	rc.left = 40;  
	rc.top = 100;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("PIF driver: ");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 280;  
	rc.top = 100;  
	rc.right  = 600;  
	rc.bottom = 500;
	cs=m_szPIFinfo;
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 40;  
	rc.top = 130;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("Interfaces:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 280;  
	rc.top = 130;  
	rc.right  = 800;  
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


	//**************************Watchdog**********************************//

	rc.left = 40;  
	rc.top = 160;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("I2C-Watchdog state:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	BOOL bWDST_UI=m_byI2CWatchdogState & WDST_0;
	BOOL bWD_AL=m_byI2CWatchdogState & WD_AL;
	BOOL bWDST_CONTROLLER=m_byI2CWatchdogState & WDST_1;
	BOOL bWDST_MONITOR=m_byI2CWatchdogState & WDST_2;
	BOOL bWDST_ALARMTON=m_byI2CWatchdogState & WDST_3;


	rc.left = 280;  
	rc.top = 160;  
	rc.right  = 780;  
	rc.bottom = 500;

	cs.Format(_T("UI: %d, WD_AL: %d, Controler: %d, Monitor: %d, Alarmton: %d"), bWDST_UI, bWD_AL, bWDST_CONTROLLER, bWDST_MONITOR, bWDST_ALARMTON);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);



//**************************Monitor PIC**********************************//
	rc.left = 40;  
	rc.top = 190;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("Version Monitor PIC:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 280;  
	rc.top = 190;  
	rc.right  = 780;  
	rc.bottom = 500;
	//cs.Format(_T("%X%X%X"), m_dataMainboard.m_iVersion_x, m_dataMainboard.m_iVersion_y, m_dataMainboard.m_iVersion_z);
	cs.Format(_T("%d.%d.%d, Checksum: %X"), 
		m_dataMainboard.m_iMonitorVersion_x, 
		m_dataMainboard.m_iMonitorVersion_y, 
		m_dataMainboard.m_iMonitorVersion_z, 
		m_dataMainboard.m_iMonitorChecksum);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	
	
	//**************************Steuer PIC**********************************//
	rc.left = 40;  
	rc.top = 220;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("Version Conductor PIC:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 280;  
	rc.top = 220;  
	rc.right  = 780;  
	rc.bottom = 500;
	//cs.Format(_T("%X%X%X"), m_dataMainboard.m_iVersion_x, m_dataMainboard.m_iVersion_y, m_dataMainboard.m_iVersion_z);
	cs.Format(_T("%s, Checksum: %X"), 
		m_dataMainboard.m_szConductorVersion, 
		m_dataMainboard.m_iConductorChecksum);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);



	//**************************SERST0**********************************//
	rc.left = 40;  
	rc.top = 250;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("State Conductor-PIC:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	BOOL bIPRESSUREBIT=m_iSERST0_Data & IPRESSUREBIT;
	BOOL bDISCONNECTIONBIT=m_iSERST0_Data & DISCONNECTIONBIT;
	BOOL bCHECKSUMERRORBIT=m_iSERST0_Data & CHECKSUMERRORBIT;


	rc.left = 280;  
	rc.top = 250;  
	rc.right  = 780;  
	rc.bottom = 500;

	cs.Format(_T("Inspiration pressure: %d, Disconnection: %d, Checksum failure: %d"), bIPRESSUREBIT, bDISCONNECTIONBIT, bCHECKSUMERRORBIT);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
	

	//**************************Blender PIC**********************************//
	rc.left = 40;  
	rc.top = 280;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("Version Blender PIC:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 280;  
	rc.top = 280;  
	rc.right  = 780;  
	rc.bottom = 500;
	cs.Format(_T("%s"), 
		m_dataMainboard.m_szBlenderVersion);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);




	//rc.left = 40;  
	//rc.top = 330;  
	//rc.right  = 280;  
	//rc.bottom = 500;
	//cs = _T("Factory defaults:");
	//DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 40;  
	rc.top = 400;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("LogFiles:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
	

	if(m_bUSBavailable && m_bLOGavailable)
	{
		m_pcLogfile->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pcLogfile->ShowWindow(SW_HIDE);

		if(m_bUSBavailable==false)
		{
			cs = _T("- no USB stick available -");
		}
		else
		{
			cs = _T("- no logfiles available -");
		}
		rc.left = 280;  
		rc.top = 400;  
		rc.right  = 780;  
		rc.bottom = 500;
		
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
	}

	if(m_bCopyLOGrunning)
	{
		rc.left = 500;  
		rc.top = 400;  
		rc.right  = 780;  
		rc.bottom = 500;

		cs = _T("... saving, please wait...");
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
	}
	else if(m_bCopyLOGfinnished)
	{
		rc.left = 500;  
		rc.top = 400;  
		rc.right  = 780;  
		rc.bottom = 500;

		m_bCopyLOGfinnished=false;

		if(m_bCopyLOGsuccess)
			cs = _T("- Logfiles saved -");
		else
			cs = _T("- ERROR saving logfiles! -");
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
	}



	//dc.BitBlt(0,0,m_lX,m_lY,CDC::FromHandle(hdcMem),0,0,SRCCOPY);
	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,hdcMem,0,0,SRCCOPY);

	SetTextColor(hdcMem,tc);
	SetBkMode(hdcMem,nBkMode);

	SelectObject(hdcMem,hbrprev);
	SelectObject(hdcMem,hpenprev);

	//DeleteObject(cbrBack);

	SelectObject(hdcMem,hBmpMemPrev);
	SelectObject(hdcMem,hPrevFont);
	DeleteObject(hBmpMem);
	DeleteDC(hdcMem);
}

/**********************************************************************************************//**
 * Executes the destroy action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndServiceInterfaces::OnDestroy()
{
	KillTimer(SERVICETIMER);

	if(m_pDlg)
	{
		m_pDlg->EndDialog(IDCLOSE);
	}
	m_pDlg = NULL;

	CWndService::OnDestroy();

}

/**********************************************************************************************//**
 * Executes the timer action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	nIDEvent	The identifier event.
 **************************************************************************************************/

void CWndServiceInterfaces::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==SERVICETIMER)
	{
		getModel()->getDATAHANDLER()->getMainboardData(&m_dataMainboard);
		m_iSERST0_Data=getModel()->getDATAHANDLER()->GetMSTATUS_ConductorPIC();
		m_byI2CWatchdogState=getModel()->getI2C()->ReadMCP_Port0();

		if (CTlsFile::Exists(_T("\\Hard Disk")))
		{
			m_bUSBavailable=true;
		}
		else
		{
			m_bUSBavailable=false;
		}

		if (CTlsFile::Exists(_T( "\\sdcard\\Logs\\alarmlog.txt" )))
		{
			m_bLOGavailable=true;
		}
		else if (CTlsFile::Exists(_T( "\\FFSDisk\\Logs\\alarmlog.txt" )))
		{
			m_bLOGavailable=true;
		}
		else
		{
			m_bLOGavailable=false;
		}


		Draw();

	}

	CWnd::OnTimer(nIDEvent);
}

/**********************************************************************************************//**
 * Executes the button clicked logfiles action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndServiceInterfaces::OnBnClickedLogfiles()
{
	CString szMessage=_T("");

	if(m_bLOGavailable)
	{
		CDlgMessageBox box(this,_T("Save log files?"),_T("Do you want to save the log files to USB?"),MB_YESNO,IDB_MSG_USB);
		int iRes=box.DoModal();
		//int iRes=AfxMessageBox(_T("Do you want to copy the log files to USB?"), MB_ICONQUESTION | MB_YESNO);
		if(iRes==IDYES)
		{
			m_bCopyLOGrunning=true;
			KillTimer(SERVICETIMER);
			theApp.getLog()->WriteLine(_T("**** copy logfile to USB via service ****"));
			
			CString szVersion=_T("Version Controller PIC: ");
			szVersion+=m_dataMainboard.m_szConductorVersion;
			theApp.getLog()->WriteLine(szVersion);

			szVersion.Format(_T("Version Monitor PIC: %d.%d.%d"), 
				m_dataMainboard.m_iMonitorVersion_x, 
				m_dataMainboard.m_iMonitorVersion_y, 
				m_dataMainboard.m_iMonitorVersion_z);
			theApp.getLog()->WriteLine(szVersion);
			
			/*szVersion=_T("Version HFO PIC: ");
			szVersion+=m_szActHFOPIC_Version;
			lfSystemLogfile->WriteLine(szVersion);*/
			
			szVersion=_T("Version fabianHFO: ");
			szVersion+=getModel()->GetMainVersion();
			theApp.getLog()->WriteLine(szVersion);

			szVersion=_T("Version Languagefiles: ");
			szVersion+=getModel()->GetLanguageString(IDS_VERSION_LANGUAGE);
			theApp.getLog()->WriteLine(szVersion);

			getModel()->getALARMHANDLER()->setLogTxt(_T("**** copy logfile to USB via menu ****"));
			
			
			Invalidate();
			UpdateWindow();
			Draw();

			bool bErrorCopy=false;
			bool bSDavailable=false;
			CString sPathFromSD = _T("\\sdcard\\");
			if(CTlsFile::Exists(sPathFromSD)==true)
			{
				bSDavailable=true;
				sPathFromSD = _T("\\sdcard\\Logs");
			}

			CString sPathFromFFSDISK = _T("\\FFSDISK\\");
			if(CTlsFile::Exists(sPathFromFFSDISK)==true)
			{
				sPathFromFFSDISK = _T("\\FFSDISK\\Logs");
			}


			CString sLogPathToFFSDISK = _T("\\Hard Disk\\") ;
			sLogPathToFFSDISK+=getModel()->GetUniqueID();
			sLogPathToFFSDISK+=_T("-Logs") ;
			if(!CTlsFile::Exists(sLogPathToFFSDISK))
			{
				CTlsFile::MkDir(sLogPathToFFSDISK);
			}
			sLogPathToFFSDISK+=_T("\\FFSDISK");
			if(!CTlsFile::Exists(sLogPathToFFSDISK))
			{
				CTlsFile::MkDir(sLogPathToFFSDISK);
			}
			sLogPathToFFSDISK+=_T("\\");

			Sleep(0);

			CString sLogPathToSD = _T("\\Hard Disk\\") ;
			if(bSDavailable)
			{
				sLogPathToSD+=getModel()->GetUniqueID();
				sLogPathToSD+=_T("-Logs\\SDCARD");
				if(!CTlsFile::Exists(sLogPathToSD))
				{
					CTlsFile::MkDir(sLogPathToSD);
				}
				sLogPathToSD+=_T("\\");
			}


			WIN32_FIND_DATA FileData;
			HANDLE hSearch = ::FindFirstFile(sPathFromFFSDISK + _T("\\*.*"), &FileData);
			//copy logs from FFSDISK
			if(hSearch != INVALID_HANDLE_VALUE)
			{
				do
				{
					CString sFile = sPathFromFFSDISK + _T("\\") + FileData.cFileName;
					CString sNewFile = sLogPathToFFSDISK;
					sNewFile+= FileData.cFileName;

					szMessage=_T("copy file: ");
					szMessage+=FileData.cFileName;


					if (CTlsFile::Copy(sFile, sNewFile , true)==false)
					{
						bErrorCopy=true;
					}
					Sleep(0);
				} while(::FindNextFile(hSearch, &FileData));

				FindClose(hSearch);
			}

			//copy logs from SD
			if(bSDavailable)
			{
				hSearch = ::FindFirstFile(sPathFromSD + _T("\\*.*"), &FileData);
				if(hSearch != INVALID_HANDLE_VALUE)
				{
					do
					{
						CString sFile = sPathFromSD + _T("\\") + FileData.cFileName;
						CString sNewFile = sLogPathToSD;
						sNewFile+= FileData.cFileName;

						szMessage=_T("copy file: ");
						szMessage+=FileData.cFileName;


						if (CTlsFile::Copy(sFile, sNewFile , true)==false)
						{
							bErrorCopy=true;
						}
						Sleep(0);
					} while(::FindNextFile(hSearch, &FileData));

					FindClose(hSearch);
				}
			}
			
			if(bErrorCopy)
			{
				m_bCopyLOGsuccess=false;
				theApp.getLog()->WriteLine(_T("#HFO:0280"));
				getModel()->getALARMHANDLER()->setLogTxt(_T("#HFO:0280"));
			}
			else
			{
				m_bCopyLOGsuccess=true;
				theApp.getLog()->WriteLine(_T("SUCCESS: files copied"));
				getModel()->getALARMHANDLER()->setLogTxt(_T("SUCCESS: files copied"));
			}

			m_bCopyLOGrunning=false;
			m_bCopyLOGfinnished=true;

			SetTimer(SERVICETIMER, 1000, NULL);

			Invalidate();
			UpdateWindow();
			Draw();
		}
	}
}

/**********************************************************************************************//**
 * Executes the button clicked exit application action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndServiceInterfaces::OnBnClickedExitApp()
{
	theApp.getLog()->WriteLine(_T("#HFO:0281"));

	getModel()->StartInstaller();
	getModel()->SetAccuTurnoff();
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_VENT_TURNOFF);
	return;
}



//void CWndServiceInterfaces::OnBnClickedEnableHFO()
//{
//	m_bHFOenabled=!m_bHFOenabled;
//	getModel()->getCONFIG()->SetHFOstate(m_bHFOenabled);
//
//	if(m_bHFOenabled)
//	{
//		m_pcEnableHFO->RefreshText(_T("disable HFO"));
//	}
//	else
//	{
//		m_pcEnableHFO->RefreshText(_T("enable HFO"));
//	}
//}

//void CWndServiceInterfaces::OnBnClickedEraseFRAM()
//{
//	CDlgMessageBox box(this,_T("Default Factory Settings"),_T("This will set the system to the default factory settings and a RESTART will be initiated. Continue?"),MB_YESNO,IDB_MSG_CONFIG);
//	m_pDlg = &box;
//	int iRes=box.DoModal();
//	m_pDlg = NULL;
//
//	if(iRes==IDCLOSE)
//	{
//		return;
//	}
//
//	if(iRes==IDYES)
//	{
//		theApp.getLog()->WriteLine(_T("*** Service mode CWndServiceInterfaces::OnBnClickedEraseFRAM() ***"));
//
//		getModel()->getI2C()->SetFRAMDefaultFactoryConfig();
//		//KernelIoControl(IOCTL_HAL_REBOOT,NULL,0,NULL,0,NULL);
//
//		CDlgMessageBox box2(this,_T("Reboot device"),_T("Please reboot the device manually (turn off and on) to make the settings effect!"),MB_YESNO,IDB_MSG_CONFIG);
//		m_pDlg = &box2;
//		int iRes=box2.DoModal();
//		m_pDlg = NULL;
//
//		if(iRes==IDCLOSE)
//		{
//			return;
//		}
//	}
//
//	UpdateWindow();
//	Draw();
//
//	/*CTlsMessageBox box2(this,_T("Restart System"),_T("You have to restart the system to make the changes effective. Do you want to restart now?"),MB_YESNO);
//	m_pDlg = &box2;
//	int iRes2=box2.DoModal();
//	m_pDlg = NULL;
//
//	if(iRes==IDYES)
//	{
//		KernelIoControl(IOCTL_HAL_REBOOT,NULL,0,NULL,0,NULL);
//	}*/
//}
