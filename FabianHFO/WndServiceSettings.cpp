// WndServiceSettings.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "WndServiceSettings.h"
#include "TlsFile.h"
#include "DlgMessageBox.h"
#include "TlsRegistry.h"

#define IOCTL_HAL_REBOOT  0x101003C
extern "C" BOOL KernelIoControl(DWORD,LPVOID,DWORD,LPVOID,DWORD,LPDWORD);


// CWndServiceNursecall

IMPLEMENT_DYNAMIC(CWndServiceSettings, CWnd)

CWndServiceSettings::CWndServiceSettings():
CWndService()
{
	m_sliderEnableNurse=NULL;
	m_sliderEnableNeoPed=NULL;
	m_sliderEnableFactory=NULL;
	m_sliderEnableAutolimit=NULL;
	m_bNurscallActive=false;
	m_bEnableFactory=false;
	m_bEnableAutolimit=false;
	m_bUseNeoPed=false;
	m_pcNurse=NULL;
	m_pcHelp_Up=NULL;
	m_pcHelp_Dw=NULL;

	m_pDlg = NULL;

	m_pcTest_Up=NULL;
	m_pcTest_Dw=NULL;
	m_pcTest_Dis=NULL;
	/*m_pcTest=NULL;*/
	m_pcSetFactory=NULL;
	m_pcEraseFRAM=NULL;
	m_pcSetAsHospital=NULL;
	m_pcDeleteCurHospital=NULL;
	m_pcSaveHospitalToUSB=NULL;
	m_pcLoadHospitalFromUSB=NULL;
	m_pcRestoreHospital=NULL;
	m_pcHelpFactory=NULL;
	m_pcHelpAutoAL=NULL;
	m_pcInstallReg=NULL;

	if(CTlsFile::Exists(_T("\\Hard Disk")))
	{
		m_bUSBavailable=true;
		if(CTlsFile::Exists(_T("\\Hard Disk\\Hospital\\hospital.dat")))
			m_bUSBDataAvailable=true;
		else
			m_bUSBDataAvailable=false;

		if(CTlsFile::Exists(_T("\\Hard Disk\\DEVELOPMENT")))
		{
			CString sPath = _T("\\Hard Disk\\DEVELOPMENT");
			WIN32_FIND_DATA FileData;
			HANDLE hSearch = ::FindFirstFile(sPath + _T("\\*.reg"), &FileData);
			if(hSearch != INVALID_HANDLE_VALUE)
			{
				m_bRegAvailable=true;
				FindClose(hSearch);
			}
			else
				m_bRegAvailable=false;
		}
		else
			m_bRegAvailable=false;
	}
	else
	{
		m_bRegAvailable=false;
		m_bUSBavailable=false;
		m_bUSBDataAvailable=false;
	}



	if(CTlsFile::Exists(_T("\\FFSDISK\\hospital.dat")))
	{
		m_bFFSDIDKDataAvailable=true;
	}
	else
	{
		m_bFFSDIDKDataAvailable=false;
	}

	
}

CWndServiceSettings::~CWndServiceSettings()
{
	if(m_sliderEnableNeoPed)
		delete m_sliderEnableNeoPed;
	m_sliderEnableNeoPed=NULL;

	if(m_sliderEnableNurse)
		delete m_sliderEnableNurse;
	m_sliderEnableNurse=NULL;


	if(m_sliderEnableFactory)
		delete m_sliderEnableFactory;
	m_sliderEnableFactory=NULL;

	if(m_sliderEnableAutolimit)
		delete m_sliderEnableAutolimit;
	m_sliderEnableAutolimit=NULL;

	if(m_pcNurse)
		delete m_pcNurse;
	m_pcNurse=NULL;

	if(m_pcHelp_Up)
		delete m_pcHelp_Up;
	m_pcHelp_Up=NULL;

	if(m_pcHelp_Dw)
		delete m_pcHelp_Dw;
	m_pcHelp_Dw=NULL;

	delete m_pcTest_Dis;
	delete m_pcTest_Up;
	delete m_pcTest_Dw;
	//delete m_pcTest;

	delete m_pcSetAsHospital;
	delete m_pcDeleteCurHospital;
	delete m_pcSaveHospitalToUSB;
	delete m_pcLoadHospitalFromUSB;
	delete m_pcRestoreHospital;
	delete m_pcSetFactory;
	delete m_pcEraseFRAM;
	delete m_pcHelpFactory;
	delete m_pcHelpAutoAL;
	delete m_pcInstallReg;


	m_pcTest_Dis=NULL;
	m_pcTest_Up=NULL;
	m_pcTest_Dw=NULL;
	//m_pcTest=NULL;

	m_pcEraseFRAM=NULL;
	m_pcSetFactory=NULL;
	m_pcSetAsHospital=NULL;
	m_pcDeleteCurHospital=NULL;
	m_pcSaveHospitalToUSB=NULL;
	m_pcLoadHospitalFromUSB=NULL;
	m_pcRestoreHospital=NULL;
	m_pcHelpFactory=NULL;
	m_pcHelpAutoAL=NULL;
	m_pcInstallReg=NULL;
}


BEGIN_MESSAGE_MAP(CWndServiceSettings, CWnd)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_MESSAGE(WM_BITMAPSLIDER_MOVING, OnMyMessage)
	//ON_BN_CLICKED(IDC_BTN_SERVICE_TESTNURSECALL, &CWndServiceSettings::OnBnClickedTestNursecall)
	ON_BN_CLICKED(IDC_BTN_SERVICE_SETHOSPITAL, &CWndServiceSettings::OnBnClickedSetHospital)
	ON_BN_CLICKED(IDC_BTN_SERVICE_DELHOSPITAL, &CWndServiceSettings::OnBnClickedDeleteHospital)
	ON_BN_CLICKED(IDC_BTN_SERVICE_SAVEHOSPITAL, &CWndServiceSettings::OnBnClickedSaveHospital)
	ON_BN_CLICKED(IDC_BTN_SERVICE_LOADHOSPITAL, &CWndServiceSettings::OnBnClickedLoadHospital)
	ON_BN_CLICKED(IDC_BTN_SERVICE_RESTOREHOSPITAL, &CWndServiceSettings::OnBnClickedRestoreHospital)
	ON_BN_CLICKED(IDC_BTN_SETUP_DEFAULTS, &CWndServiceSettings::OnBnClickedRestoreFactory)
	ON_BN_CLICKED(IDC_BTN_ERASEFRAM, &CWndServiceSettings::OnBnClickedEraseFRAM)
	ON_BN_CLICKED(IDC_BTN_HELP_SRVC_FACTORY, &CWndServiceSettings::OnBnClickedHelpFactory)
	ON_BN_CLICKED(IDC_BTN_HELP_SRVC_AUTOAL, &CWndServiceSettings::OnBnClickedHelpAutoAL)
	ON_BN_CLICKED(IDC_BTN_REGINSTALLER, &CWndServiceSettings::OnBnClickedRegInstall)
END_MESSAGE_MAP()



// CWndServiceNursecall message handlers
void CWndServiceSettings::Init()
{
	CClientDC dc(this);

	m_pcTest_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN160_LG_UP);
	m_pcTest_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN160_LG_DW);
	m_pcTest_Dis= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN160_LG_DIS);

	m_pcNurse		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_PIC_NURSE32);

	m_pcHelp_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_HELP_UP_LIGHTGREY);
	m_pcHelp_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_HELP_DW_LIGHTGREY);

	BTN btn;

	//btn.wID					= IDC_BTN_SERVICE_TESTNURSECALL;	
	//btn.poPosition.x		= 500;//;586
	//btn.poPosition.y		= 30;
	//btn.pcBmpUp				= m_pcTest_Up;
	//btn.pcBmpDown			= m_pcTest_Dw;
	//btn.pcBmpFocus			= m_pcTest_Up;
	//btn.pcBmpDisabled		= m_pcTest_Up;
	//btn.dwFormat			= DT_VCENTER|DT_CENTER;
	//m_pcTest=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	//m_pcTest->Create(this,g_hf15AcuMed,0);
	//m_pcTest->SetText(_T("activate alarm"));
	//m_pcTest->ShowWindow(SW_SHOW);



	m_bNurscallActive=getModel()->getCONFIG()->IsNursecallActive();

	m_sliderEnableNurse = new CBitmapSlider();
	m_sliderEnableNurse->Create(_T(""),WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_NOTIFY|BS_OWNERDRAW, CRect(180,35,329,80), 
		this,IDC_SLD_NURSECALL);
	m_sliderEnableNurse->SetBitmapChannel( IDB_SLD_CHAN_GREY, NULL );
	m_sliderEnableNurse->SetBitmapThumb( IDB_SLD_THUMB_GREY, IDB_SLD_THUMB_ACT_GREY);
	m_sliderEnableNurse->SetRange( 0, 1 );
	if(m_bNurscallActive)
	{
		m_sliderEnableNurse->SetPos( 1 );
		//m_pcTest->EnableWindow(TRUE);
	}
	else
	{
		m_sliderEnableNurse->SetPos( 0 );
		//m_pcTest->EnableWindow(FALSE);
	}
	m_sliderEnableNurse->SetMargin( 5, 0, 6, 0 );
	m_sliderEnableNurse->DrawFocusRect( FALSE );

	m_bUseNeoPed=getModel()->getCONFIG()->useNeoPed();

	m_sliderEnableNeoPed = new CBitmapSlider();
	m_sliderEnableNeoPed->Create(_T(""),WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_NOTIFY|BS_OWNERDRAW, CRect(590,35,739,80), 
		this,IDC_SLD_USENEOPED);
	m_sliderEnableNeoPed->SetBitmapChannel( IDB_SLD_CHAN_GREY, NULL );
	m_sliderEnableNeoPed->SetBitmapThumb( IDB_SLD_THUMB_GREY, IDB_SLD_THUMB_ACT_GREY);
	m_sliderEnableNeoPed->SetRange( 0, 1 );
	if(m_bUseNeoPed)
	{
		m_sliderEnableNeoPed->SetPos( 1 );
	}
	else
	{
		m_sliderEnableNeoPed->SetPos( 0 );
	}
	m_sliderEnableNeoPed->SetMargin( 5, 0, 6, 0 );
	m_sliderEnableNeoPed->DrawFocusRect( FALSE );


	m_bEnableFactory=getModel()->getCONFIG()->IsEnableFactoryActive();

	m_sliderEnableFactory = new CBitmapSlider();
	m_sliderEnableFactory->Create(_T(""),WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_NOTIFY|BS_OWNERDRAW, CRect(180,330,329,375), 
		this,IDC_SLD_FACTORY);
	m_sliderEnableFactory->SetBitmapChannel( IDB_SLD_CHAN_GREY, NULL );
	m_sliderEnableFactory->SetBitmapThumb( IDB_SLD_THUMB_GREY, IDB_SLD_THUMB_ACT_GREY);
	m_sliderEnableFactory->SetRange( 0, 1 );
	if(m_bEnableFactory)
	{
		m_sliderEnableFactory->SetPos( 1 );
	}
	else
	{
		m_sliderEnableFactory->SetPos( 0 );
	}
	m_sliderEnableFactory->SetMargin( 5, 0, 6, 0 );
	m_sliderEnableFactory->DrawFocusRect( FALSE );


	m_bEnableAutolimit=getModel()->getCONFIG()->IsEnableAutolimitActive();

	m_sliderEnableAutolimit = new CBitmapSlider();
	m_sliderEnableAutolimit->Create(_T(""),WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_NOTIFY|BS_OWNERDRAW, CRect(180,390,329,425), 
		this,IDC_SLD_AUTOLIMIT);
	m_sliderEnableAutolimit->SetBitmapChannel( IDB_SLD_CHAN_GREY, NULL );
	m_sliderEnableAutolimit->SetBitmapThumb( IDB_SLD_THUMB_GREY, IDB_SLD_THUMB_ACT_GREY);
	m_sliderEnableAutolimit->SetRange( 0, 1 );
	if(m_bEnableAutolimit)
	{
		m_sliderEnableAutolimit->SetPos( 1 );
	}
	else
	{
		m_sliderEnableAutolimit->SetPos( 0 );
	}
	m_sliderEnableAutolimit->SetMargin( 5, 0, 6, 0 );
	m_sliderEnableAutolimit->DrawFocusRect( FALSE );


	btn.wID					= IDC_BTN_SERVICE_SETHOSPITAL;	
	btn.poPosition.x		= 200;//;586
	btn.poPosition.y		= 120;
	btn.pcBmpUp				= m_pcTest_Up;
	btn.pcBmpDown			= m_pcTest_Dw;
	btn.pcBmpFocus			= m_pcTest_Up;
	btn.pcBmpDisabled		= m_pcTest_Dis;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcSetAsHospital=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcSetAsHospital->Create(this,g_hf15AcuMed,0);
	m_pcSetAsHospital->SetText(_T("set current"));
	m_pcSetAsHospital->ShowWindow(SW_SHOW);

	btn.wID					= IDC_BTN_SERVICE_DELHOSPITAL;	
	btn.poPosition.x		= 590;//;586
	btn.poPosition.y		= 120;
	btn.pcBmpUp				= m_pcTest_Up;
	btn.pcBmpDown			= m_pcTest_Dw;
	btn.pcBmpFocus			= m_pcTest_Up;
	btn.pcBmpDisabled		= m_pcTest_Dis;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcDeleteCurHospital=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcDeleteCurHospital->Create(this,g_hf15AcuMed,0);
	m_pcDeleteCurHospital->SetText(_T("delete settings"));
	m_pcDeleteCurHospital->ShowWindow(SW_SHOW);
	if(m_bFFSDIDKDataAvailable)
		m_pcDeleteCurHospital->EnableWindow(TRUE);
	else
	{
		m_pcDeleteCurHospital->RefreshText(_T("not available"));
		m_pcDeleteCurHospital->EnableWindow(FALSE);
	}


	btn.wID					= IDC_BTN_SERVICE_SAVEHOSPITAL;	
	btn.poPosition.x		= 200;//;586
	btn.poPosition.y		= 180;
	btn.pcBmpUp				= m_pcTest_Up;
	btn.pcBmpDown			= m_pcTest_Dw;
	btn.pcBmpFocus			= m_pcTest_Up;
	btn.pcBmpDisabled		= m_pcTest_Dis;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcSaveHospitalToUSB=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcSaveHospitalToUSB->Create(this,g_hf15AcuMed,0);
	m_pcSaveHospitalToUSB->SetText(_T("save to USB"));
	m_pcSaveHospitalToUSB->ShowWindow(SW_SHOW);
	if(m_bFFSDIDKDataAvailable && m_bUSBavailable)
		m_pcSaveHospitalToUSB->EnableWindow(TRUE);
	else
	{
		m_pcSaveHospitalToUSB->RefreshText(_T("not available"));
		m_pcSaveHospitalToUSB->EnableWindow(FALSE);
	}


	btn.wID					= IDC_BTN_SERVICE_LOADHOSPITAL;	
	btn.poPosition.x		= 590;//;586
	btn.poPosition.y		= 180;
	btn.pcBmpUp				= m_pcTest_Up;
	btn.pcBmpDown			= m_pcTest_Dw;
	btn.pcBmpFocus			= m_pcTest_Up;
	btn.pcBmpDisabled		= m_pcTest_Dis;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcLoadHospitalFromUSB=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcLoadHospitalFromUSB->Create(this,g_hf15AcuMed,0);
	m_pcLoadHospitalFromUSB->SetText(_T("load from USB"));
	m_pcLoadHospitalFromUSB->ShowWindow(SW_SHOW);
	if(m_bUSBavailable && m_bUSBDataAvailable)
		m_pcLoadHospitalFromUSB->EnableWindow(TRUE);
	else
	{
		m_pcLoadHospitalFromUSB->RefreshText(_T("not available"));
		m_pcLoadHospitalFromUSB->EnableWindow(FALSE);
	}

	btn.wID					= IDC_BTN_SERVICE_RESTOREHOSPITAL;	
	btn.poPosition.x		= 200;//;586
	btn.poPosition.y		= 240;
	btn.pcBmpUp				= m_pcTest_Up;
	btn.pcBmpDown			= m_pcTest_Dw;
	btn.pcBmpFocus			= m_pcTest_Up;
	btn.pcBmpDisabled		= m_pcTest_Dis;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcRestoreHospital=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcRestoreHospital->Create(this,g_hf15AcuMed,0);
	m_pcRestoreHospital->SetText(_T("restore"));
	m_pcRestoreHospital->ShowWindow(SW_SHOW);
	if(m_bFFSDIDKDataAvailable)
		m_pcRestoreHospital->EnableWindow(TRUE);
	else
	{
		m_pcRestoreHospital->RefreshText(_T("not available"));
		m_pcRestoreHospital->EnableWindow(FALSE);
	}

	btn.wID					= IDC_BTN_SETUP_DEFAULTS;	
	btn.poPosition.x		= 350;//
	btn.poPosition.y		= 327;
	btn.pcBmpUp				= m_pcTest_Up;
	btn.pcBmpDown			= m_pcTest_Dw;
	btn.pcBmpFocus			= m_pcTest_Up;
	btn.pcBmpDisabled		= m_pcTest_Dis;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcSetFactory=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcSetFactory->Create(this,g_hf10AcuBold,0);
	m_pcSetFactory->SetText(_T("reset values"));
	m_pcSetFactory->ShowWindow(SW_SHOW);


	btn.wID					= IDC_BTN_ERASEFRAM;	
	btn.poPosition.x		= 530;//
	btn.poPosition.y		= 327;
	btn.pcBmpUp				= m_pcTest_Up;
	btn.pcBmpDown			= m_pcTest_Dw;
	btn.pcBmpFocus			= m_pcTest_Up;
	btn.pcBmpDisabled		= m_pcTest_Dis;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcEraseFRAM=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcEraseFRAM->Create(this,g_hf10AcuBold,0);
	m_pcEraseFRAM->SetText(_T("reset complete"));


	btn.wID					= IDC_BTN_HELP_SRVC_FACTORY;	
	btn.poPosition.x		= 710;//
	btn.poPosition.y		= 327;
	btn.pcBmpUp				= m_pcHelp_Up;
	btn.pcBmpDown			= m_pcHelp_Dw;
	btn.pcBmpFocus			= m_pcHelp_Up;
	btn.pcBmpDisabled		= m_pcHelp_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcHelpFactory=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcHelpFactory->Create(this,g_hf10AcuBold,0);
	m_pcHelpFactory->SetText(_T(""));
	m_pcHelpFactory->ShowWindow(SW_SHOW);


	btn.wID					= IDC_BTN_HELP_SRVC_AUTOAL;	
	btn.poPosition.x		= 710;//
	btn.poPosition.y		= 382;
	btn.pcBmpUp				= m_pcHelp_Up;
	btn.pcBmpDown			= m_pcHelp_Dw;
	btn.pcBmpFocus			= m_pcHelp_Up;
	btn.pcBmpDisabled		= m_pcHelp_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcHelpAutoAL=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcHelpAutoAL->Create(this,g_hf10AcuBold,0);
	m_pcHelpAutoAL->SetText(_T(""));
	m_pcHelpAutoAL->ShowWindow(SW_SHOW);



	btn.wID					= IDC_BTN_REGINSTALLER;	
	btn.poPosition.x		= 130;
	btn.poPosition.y		= 477;
	btn.pcBmpUp				= m_pcTest_Up;
	btn.pcBmpDown			= m_pcTest_Dw;
	btn.pcBmpFocus			= m_pcTest_Up;
	btn.pcBmpDisabled		= m_pcTest_Dis;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcInstallReg=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcInstallReg->Create(this,g_hf15AcuMed,0);
	m_pcInstallReg->SetText(_T("load registry"));
	m_pcInstallReg->ShowWindow(SW_SHOW);

	if(m_bRegAvailable)
		m_pcInstallReg->EnableWindow(TRUE);
	else
	{
		m_pcInstallReg->RefreshText(_T("not available"));
		m_pcInstallReg->EnableWindow(FALSE);
	}

	m_pcMenuBack->ShowWindow(SW_SHOW);

	SetTimer(USBCHECKTIMER,2000,NULL);
}

void CWndServiceSettings::Draw()
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
	HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf9AcuBold);
	int tc=SetTextColor(hdcMem,0x0000000);

	Rectangle(hdcMem, 0, 0, m_lX, m_lY);

	if(CTlsFile::Exists(_T("\\Hard Disk")))
	{
		m_bUSBavailable=true;
		if(CTlsFile::Exists(_T("\\Hard Disk\\Hospital\\hospital.dat")))
			m_bUSBDataAvailable=true;
		else
			m_bUSBDataAvailable=false;

	}
	else
	{
		m_bUSBavailable=false;
		m_bUSBDataAvailable=false;
	}

	

	if(CTlsFile::Exists(_T("\\FFSDISK\\hospital.dat")))
	{
		m_bFFSDIDKDataAvailable=true;
	}
	else
	{
		m_bFFSDIDKDataAvailable=false;
	}

	if(m_bFFSDIDKDataAvailable)
	{
		m_pcDeleteCurHospital->EnableWindow(TRUE);
		m_pcRestoreHospital->EnableWindow(TRUE);

		m_pcRestoreHospital->RefreshText(_T("restore"));
		m_pcDeleteCurHospital->RefreshText(_T("delete settings"));
	}
	else
	{
		m_pcRestoreHospital->RefreshText(_T("not available"));
		m_pcDeleteCurHospital->RefreshText(_T("not available"));

		m_pcDeleteCurHospital->EnableWindow(FALSE);
		m_pcRestoreHospital->EnableWindow(FALSE);
	}


	if(m_bFFSDIDKDataAvailable && m_bUSBavailable)
	{
		m_pcSaveHospitalToUSB->EnableWindow(TRUE);
		m_pcSaveHospitalToUSB->RefreshText(_T("save to USB"));
	}
	else
	{
		m_pcSaveHospitalToUSB->RefreshText(_T("not available"));
		m_pcSaveHospitalToUSB->EnableWindow(FALSE);
	}

	if(m_bRegAvailable)
	{
		m_pcInstallReg->EnableWindow(TRUE);
		m_pcInstallReg->RefreshText(_T("load registry"));
	}
	else
	{
		m_pcInstallReg->RefreshText(_T("not available"));
		m_pcInstallReg->EnableWindow(FALSE);
	}

	if(m_bUSBavailable && m_bUSBDataAvailable)
	{
		m_pcLoadHospitalFromUSB->EnableWindow(TRUE);
		m_pcLoadHospitalFromUSB->RefreshText(_T("load from USB"));
	}
	else
	{
		m_pcLoadHospitalFromUSB->RefreshText(_T("not available"));
		m_pcLoadHospitalFromUSB->EnableWindow(FALSE);
	}

		

	//CDC* pDCMem=CDC::FromHandle(hdcMem);
	CPen penLine;
	penLine.CreatePen(PS_SOLID,1,RGB(140,140,140));
	SelectObject(hdcMem,(HPEN)penLine);
	CBrush cbrRound(RGB(200,200,200));
	SelectObject(hdcMem,cbrRound);
	
	//############Nursecall#####################################
	RoundRect(hdcMem, 10, 15, 395, 100,20,20);

	if(m_bNurscallActive)
		m_pcNurse->Draw(hdcMem,340,33);

	rc.left = 40;  
	rc.top = 45;  
	rc.right  = 260;  
	rc.bottom = 500;
	CStringW cs = _T("Nursecall:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


	//############Neo and Ped#####################################
	RoundRect(hdcMem, 405, 15, 770, 100,20,20);

	rc.left = 425;  
	rc.top = 45;  
	rc.right  = 800;  
	rc.bottom = 500;
	cs = _T("NEO+PED:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	/*rc.left = 425;  
	rc.top = 55;  
	rc.right  = 800;  
	rc.bottom = 500;
	cs = _T("(no average values)");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);*/

	//############USB hospital settings#####################################
	RoundRect(hdcMem, 10, 110, 770, 300,20,20);
	rc.left = 40;  
	rc.top = 125;  
	rc.right  = 400;  
	rc.bottom = 500;
	cs = _T("Set current settings");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
	rc.left = 40;  
	rc.top = 145;  
	rc.right  = 400;  
	rc.bottom = 500;
	cs = _T("as hospital settings:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 420;  
	rc.top = 125;  
	rc.right  = 800;  
	rc.bottom = 500;
	cs = _T("Delete current");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
	rc.left = 420;  
	rc.top = 145;  
	rc.right  = 800;  
	rc.bottom = 500;
	cs = _T("hospital settings:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 40;  
	rc.top = 185;  
	rc.right  = 400;  
	rc.bottom = 500;
	cs = _T("Save hospital");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
	rc.left = 40;  
	rc.top = 205;  
	rc.right  = 400;  
	rc.bottom = 500;
	cs = _T("settings to USB:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 420;  
	rc.top = 185;  
	rc.right  = 800;  
	rc.bottom = 500;
	cs = _T("Load hospital settings");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
	rc.left = 420;  
	rc.top = 205;  
	rc.right  = 800;  
	rc.bottom = 500;
	cs = _T("from USB:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 40;  
	rc.top = 245;  
	rc.right  = 400;  
	rc.bottom = 500;
	cs = _T("Set device to");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
	rc.left = 40;  
	rc.top = 265;  
	rc.right  = 400;  
	rc.bottom = 500;
	cs = _T("hospital settings:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


	//############settings#####################################
	RoundRect(hdcMem, 10, 310, 770, 460,20,20);

	rc.left = 40;  
	rc.top = 335;  
	rc.right  = 260;  
	rc.bottom = 500;
	cs = _T("Enable factory");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 40;  
	rc.top = 355;  
	rc.right  = 260;  
	rc.bottom = 500;
	cs = _T("settings:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 40;  
	rc.top = 390;  
	rc.right  = 260;  
	rc.bottom = 500;
	cs = _T("Enable auto");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 40;  
	rc.top = 410;  
	rc.right  = 260;  
	rc.bottom = 500;
	cs = _T("alarm limits:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	//############REGISTRY#####################################
	RoundRect(hdcMem, 10, 470, 315, 530,20,20);

	rc.left = 40;  
	rc.top = 470;  
	rc.right  = 260;  
	rc.bottom = 530;
	cs = _T("Registry:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_VCENTER|DT_SINGLELINE);

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

//************************************
// Method:    OnDestroy
// FullName:  CWndServiceSettings::OnDestroy
// Access:    public 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CWndServiceSettings::OnDestroy()
{
	if(m_pDlg)
	{
		m_pDlg->EndDialog(IDCLOSE);
	}
	m_pDlg = NULL;

	KillTimer(USBCHECKTIMER);

	CWndService::OnDestroy();
}
LRESULT CWndServiceSettings::OnMyMessage(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case IDC_SLD_AUTOLIMIT:
		{
			if(lParam==1)
			{
				m_bEnableAutolimit=true;
				//m_pcTest->EnableWindow(TRUE);
			}
			else
			{
				m_bEnableAutolimit=false;
				//m_pcTest->EnableWindow(FALSE);
			}

			getModel()->getCONFIG()->SetEnableAutolimit(m_bEnableAutolimit);

			if(m_bEnableAutolimit)
				theApp.getLog()->WriteLine(_T("*** Service mode EnableAutolimit==true ***"));
			else
				theApp.getLog()->WriteLine(_T("*** Service mode EnableAutolimit==false ***"));

			Draw();
		}
		break;
	case IDC_SLD_FACTORY:
		{
			if(lParam==1)
			{
				m_bEnableFactory=true;
			}
			else
			{
				m_bEnableFactory=false;
			}

			getModel()->getCONFIG()->SetEnableFactory(m_bEnableFactory);

			if(m_bEnableFactory)
				theApp.getLog()->WriteLine(_T("*** Service mode EnableFactory==true ***"));
			else
				theApp.getLog()->WriteLine(_T("*** Service mode EnableFactory==false ***"));

			Draw();
		}
		break;
	case IDC_SLD_NURSECALL:
		{
			if(lParam==1)
			{
				m_bNurscallActive=true;
			}
			else
			{
				m_bNurscallActive=false;
			}

			getModel()->getCONFIG()->SetNursecallActive(m_bNurscallActive);

			

			Draw();
		}
		break;
	case IDC_SLD_USENEOPED:
		{
			if(lParam==1)
			{
				m_bUseNeoPed=true;
			}
			else
			{
				m_bUseNeoPed=false;
			}

			getModel()->getCONFIG()->setUseNeoPed(m_bUseNeoPed);

			Draw();
		}
		break;
	}
	return 1;
}

//void CWndServiceSettings::OnBnClickedTestNursecall()
//{
//	if(getModel()->getCONFIG()->IsNursecallActive())
//	{
//		getModel()->getDATAHANDLER()->SetNurscallAlarm(true);
//		if(AfxGetApp())
//			AfxGetApp()->GetMainWnd()->PostMessage(WM_SYSTEMSTATE_CHANGED);
//	}
//}

void CWndServiceSettings::OnBnClickedSetHospital()
{
	KillTimer(USBCHECKTIMER);
	CStringW sFile=_T("\\FFSDISK\\hospital.dat");
	CDlgMessageBox box(this,_T("Question"),_T("Do you want to save the current ventilation settings as the hospital settings for this device? Existing hospital settings will be deleted!"),MB_YESNO,IDB_MSG_HOSPITAL);
	m_pDlg = &box;
	int iRes=box.DoModal();
	m_pDlg = NULL;

	if(iRes==IDCLOSE)
	{
		return;
	}

	if(iRes==IDYES)
	{
		if(CTlsFile::Exists(sFile))
		{
			CTlsFile::Delete(sFile);
			Sleep(1000);
		}
		getModel()->SaveHospitalSettings();
	}
	
	Draw();

	SetTimer(USBCHECKTIMER,2000,NULL);
}
void CWndServiceSettings::OnBnClickedDeleteHospital()
{
	KillTimer(USBCHECKTIMER);
	CStringW sFile=_T("\\FFSDISK\\hospital.dat");
	CDlgMessageBox box(this,_T("Question"),_T("Do you really want to delete the hospital setting from this device?"),MB_YESNO,IDB_MSG_STOP);
	m_pDlg = &box;
	int iRes=box.DoModal();
	m_pDlg = NULL;

	if(iRes==IDCLOSE)
	{
		return;
	}

	if(iRes==IDYES && CTlsFile::Exists(sFile))
	{
		CTlsFile::Delete(sFile);
		Sleep(1000);
	}
	
	Draw();

	SetTimer(USBCHECKTIMER,2000,NULL);
}
void CWndServiceSettings::OnBnClickedSaveHospital()
{
	KillTimer(USBCHECKTIMER);
	CStringW fileFrom=_T("\\FFSDISK\\hospital.dat");
	CStringW fileTo=_T("\\Hard Disk\\Hospital\\hospital.dat");

	CString folder = _T("\\Hard Disk\\Hospital") ;

	CDlgMessageBox box(this,_T("Question"),_T("Do you want to save the current hospital settings of this device to the USB? Existing hospital settings on the USB will be deleted!"),MB_YESNO,IDB_MSG_STOP);
	m_pDlg = &box;
	int iRes=box.DoModal();
	m_pDlg = NULL;

	if(iRes==IDCLOSE)
	{
		return;
	}

	if(iRes==IDYES)
	{
		if(!CTlsFile::Exists(folder))
		{
			CTlsFile::MkDir(folder);
		}

		if(CTlsFile::Exists(fileTo))
		{
			if(iRes==IDYES)
			{
				CTlsFile::Delete(fileTo);
				Sleep(1000);
			}
		}

		if(CTlsFile::Copy(fileFrom, fileTo, true)==false)
		{
			//return false;
		}
	}

	Draw();

	SetTimer(USBCHECKTIMER,2000,NULL);
}
void CWndServiceSettings::OnBnClickedLoadHospital()
{
	KillTimer(USBCHECKTIMER);
	CStringW fileTo=_T("\\FFSDISK\\hospital.dat");
	CStringW fileFrom=_T("\\Hard Disk\\Hospital\\hospital.dat");

	CDlgMessageBox box2(this,_T("Question"),_T("Do you want to load the hospital settings from USB to this device? Existing hospital settings on this device will be deleted!?"),MB_YESNO,IDB_MSG_STOP);
	m_pDlg = &box2;
	int iRes=box2.DoModal();
	m_pDlg = NULL;

	if(iRes==IDCLOSE)
	{
		return;
	}

	if(iRes==IDYES)
	{
		if(CTlsFile::Exists(fileTo))
		{
			CTlsFile::Delete(fileTo);

			Sleep(1000);
		}

		if(CTlsFile::Copy(fileFrom, fileTo, true)==true)
		{
			getModel()->LoadHospitalSettings();
			if(AfxGetApp())
			{
				//AfxGetApp()->GetMainWnd()->PostMessage(WM_LANGUAGE_CHANGED);
				AfxGetApp()->GetMainWnd()->PostMessage(WM_RELOAD_CONFIG);
			}

		}
	}

	
	
	Draw();

	UpdateWindow();
	Invalidate(TRUE);

	SetTimer(USBCHECKTIMER,2000,NULL);
	
}

// **************************************************************************
// 
// **************************************************************************
void CWndServiceSettings::OnBnClickedHelpFactory()
{
	CDlgMessageBox box(this,_T("Help"),_T(""),MB_OK,0,HLP_0001);
	m_pDlg = &box;
	int iRes=box.DoModal();
	m_pDlg = NULL;

	Draw();
	UpdateWindow();
	Invalidate(TRUE);
}
// **************************************************************************
// 
// **************************************************************************
void CWndServiceSettings::OnBnClickedHelpAutoAL()
{
	CDlgMessageBox box(this,_T("Help"),_T(""),MB_OK,0,HLP_0002);
	m_pDlg = &box;
	int iRes=box.DoModal();
	m_pDlg = NULL;

	Draw();
	UpdateWindow();
	Invalidate(TRUE);
}
// **************************************************************************
// 
// **************************************************************************
void CWndServiceSettings::OnBnClickedRegInstall()
{
	CDlgMessageBox box2(this,_T("Question"),_T("Do you want to load the REGISTRY settings?"),MB_YESNO,IDB_MSG_STOP);
	m_pDlg = &box2;
	int iRes=box2.DoModal();
	m_pDlg = NULL;

	if(iRes==IDYES)
	{
		bool bError=false;

		CString sPath = _T("\\Hard Disk\\DEVELOPMENT");
		CString szState=_T("");
		WIN32_FIND_DATA FileData;
		HANDLE hSearch = ::FindFirstFile(sPath + _T("\\*.reg"), &FileData);
		if(hSearch != INVALID_HANDLE_VALUE)
		{
			do
			{
				CString sFileName = FileData.cFileName;

				szState=_T("#REGISTRY INSTALL: file '")+sPath + _T("\\") + FileData.cFileName;
				theApp.getLog()->WriteLine(szState);

				int iResult = CTlsRegistry::Import(sPath + _T("\\") + FileData.cFileName);
				if(iResult != 0)
				{
					CString szTemp=_T("Install of registry file failed: ");
					szTemp+=FileData.cFileName;
					CDlgMessageBox box2(this,_T("ERROR"),szTemp,MB_OK);
					int iRes=box2.DoModal();

					bError=true;
				}

			} while(::FindNextFile(hSearch, &FileData));

			FindClose(hSearch);
		}
	}

	
	Draw();
	UpdateWindow();
	Invalidate(TRUE);
}

// **************************************************************************
// 
// **************************************************************************
void CWndServiceSettings::OnBnClickedEraseFRAM()
{
	CDlgMessageBox box(this,_T("Default Factory Configuration"),_T("This will set the system to the default factory configuration and a RESTART will be initiated. Continue?"),MB_YESNO,IDB_MSG_CONFIG);
	m_pDlg = &box;
	int iRes=box.DoModal();
	m_pDlg = NULL;

	if(iRes==IDCLOSE)
	{
		return;
	}

	if(iRes==IDYES)
	{
		theApp.getLog()->WriteLine(_T("#HFO:0283"));

		getModel()->getI2C()->SetFRAMDefaultFactoryConfig();
		
		CDlgMessageBox box2(this,_T("Restart System"),_T("The system will restart now to make the changes effective!"),MB_OK,IDB_MSG_CONFIG);
		m_pDlg = &box2;
		int iRes2=box2.DoModal();
		m_pDlg = NULL;

		//rku, change reboot !!!
		getModel()->getDATAHANDLER()->saveOpTime();//newOPtime
		Sleep(500);
		KernelIoControl(IOCTL_HAL_REBOOT,NULL,0,NULL,0,NULL);
	}

	UpdateWindow();
	Draw();

	

}
void CWndServiceSettings::OnBnClickedRestoreFactory()
{
	CDlgMessageBox box(this,getModel()->GetLanguageString(IDS_MSG_CAPTION_FACTORY),getModel()->GetLanguageString(IDS_MSG_FACTORY),MB_YESNO,IDB_MSG_CONFIG);
	m_pDlg = &box;
	int iRes=box.DoModal();
	m_pDlg = NULL;

	if(iRes==IDCLOSE)
	{
		return;
	}

	if(iRes==IDYES)
	{
		theApp.getLog()->WriteLine(L"***load default***");

		getModel()->getI2C()->SetFRAMDefaultFactoryValues();
		Sleep(200);
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_RELOAD_CONFIG);
	}
	this->SetFocus();

	UpdateWindow();
	Draw();
}

void CWndServiceSettings::OnBnClickedRestoreHospital()
{
	KillTimer(USBCHECKTIMER);
	CDlgMessageBox box(this,_T("Question"),_T("Do you want to load the hospital settings on this device?"),MB_YESNO,IDB_MSG_STOP);
	m_pDlg = &box;
	int iRes=box.DoModal();
	m_pDlg = NULL;

	if(iRes==IDCLOSE)
	{
		return;
	}

	if(iRes==IDYES)
	{
		getModel()->LoadHospitalSettings();
		if(AfxGetApp())
		{
			//AfxGetApp()->GetMainWnd()->PostMessage(WM_LANGUAGE_CHANGED);
			AfxGetApp()->GetMainWnd()->PostMessage(WM_RELOAD_CONFIG);
		}
	}

	Draw();

	UpdateWindow();
	Invalidate(TRUE);

	SetTimer(USBCHECKTIMER,2000,NULL);
}

// **************************************************************************
// 
// **************************************************************************
void CWndServiceSettings::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==USBCHECKTIMER)
	{
		bool bRedraw=false;
		if(CTlsFile::Exists(_T("\\Hard Disk")))
		{
			if(m_bUSBavailable == false)
				bRedraw=true;
			m_bUSBavailable=true;
			if(CTlsFile::Exists(_T("\\Hard Disk\\hospital.dat")))
			{
				if(m_bUSBDataAvailable == false)
					bRedraw=true;
				m_bUSBDataAvailable=true;
			}
			else
			{
				if(m_bUSBDataAvailable == true)
					bRedraw=true;
				m_bUSBDataAvailable=false;
			}

			if(CTlsFile::Exists(_T("\\Hard Disk\\DEVELOPMENT")))
			{
				CString sPath = _T("\\Hard Disk\\DEVELOPMENT");
				WIN32_FIND_DATA FileData;
				HANDLE hSearch = ::FindFirstFile(sPath + _T("\\*.reg"), &FileData);
				if(hSearch != INVALID_HANDLE_VALUE)
				{
					if(m_bRegAvailable == false)
						bRedraw=true;
					m_bRegAvailable=true;
				}
				else
				{
					if(m_bRegAvailable == true)
						bRedraw=true;
					m_bRegAvailable=false;
				}
			}
			else
			{
				if(m_bRegAvailable == true)
					bRedraw=true;
				m_bRegAvailable=false;
			}
		}
		else
		{
			if(m_bUSBavailable == true || m_bUSBDataAvailable == true)
				bRedraw=true;
			m_bUSBavailable=false;
			m_bUSBDataAvailable=false;

			if(m_bRegAvailable == true)
				bRedraw=true;
			m_bRegAvailable=false;
		}

		if(CTlsFile::Exists(_T("\\FFSDISK\\hospital.dat")))
		{
			if(m_bFFSDIDKDataAvailable == false)
				bRedraw=true;
			m_bFFSDIDKDataAvailable=true;
		}
		else
		{
			if(m_bFFSDIDKDataAvailable == true)
				bRedraw=true;
			m_bFFSDIDKDataAvailable=false;
		}

		if(bRedraw)
			Draw();

	}

	CWnd::OnTimer(nIDEvent);
}