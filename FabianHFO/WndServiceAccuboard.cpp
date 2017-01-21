// WndServiceAccuboard.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "WndServiceAccuboard.h"
#include "DlgMessageBox.h"


// CWndServiceAccuboard

IMPLEMENT_DYNAMIC(CWndServiceAccuboard, CWnd)

CWndServiceAccuboard::CWndServiceAccuboard():
CWndService()
{
	m_pcUp=NULL;
	m_pcDw=NULL;
	m_pcAccuTest=NULL;

	m_wACCU_DAT_STATE=0;
	m_wACCU_DAT_REMAIN=0;
	m_wACCU_DAT_PERCENT=0;
	m_wACCU_DAT_CAPACITY=0;
	m_wACCU_DAT_VOLTAGE=0;
	m_wACCU_DAT_DISCHARGECURRENT=0;
	m_wACCU_DAT_CHARGECURRENT=0;
	m_wACCU_DAT_TEMPERATURE=0;
	m_wACCU_DAT_CYCLES=0;
	m_wACCU_DAT_VERSION=0;
	m_wACCU_DAT_CHECKSUM=0;



	//m_iPARST0_Data=0;
	m_iOpMinBatt=getModel()->getDATAHANDLER()->getOpTimeBatteryMin(false);

	m_pDlg = NULL;
}

CWndServiceAccuboard::~CWndServiceAccuboard()
{
	delete m_pcUp;
	m_pcUp=NULL;
	delete m_pcDw;
	m_pcDw=NULL;
	/*delete m_pcDis;
	m_pcDis=NULL;*/
	delete m_pcAccuTest;
	m_pcAccuTest=NULL;
}


BEGIN_MESSAGE_MAP(CWndServiceAccuboard, CWnd)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_ACCU_TEST, &CWndServiceAccuboard::OnBnClickedAccuTest)
END_MESSAGE_MAP()



// CWndServiceAccuboard message handlers
void CWndServiceAccuboard::Init()
{
	CClientDC dc(this);

	m_pcUp= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETUP_TODAY_UP);
	m_pcDw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETUP_TODAY_DW);
	//m_pcDis= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETUP_TODAY_D);

	BTN btn;

	btn.wID					= IDC_BTN_ACCU_TEST;	
	btn.poPosition.x		= 220;
	btn.poPosition.y		= 465;
	btn.pcBmpUp				= m_pcUp;
	btn.pcBmpDown			= m_pcDw;
	btn.pcBmpFocus			= m_pcUp;
	btn.pcBmpDisabled		= m_pcUp;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcAccuTest=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcAccuTest->Create(this,g_hf15AcuMed,0);
	m_pcAccuTest->SetText(_T("start test"));
	m_pcAccuTest->ShowWindow(SW_HIDE);

	


	m_pcMenuBack->ShowWindow(SW_SHOW);

	


	
	
	if(getModel()->getI2C()->IsI2C_ACCUavailability())
	{
		m_wACCU_DAT_STATE=getModel()->getI2C()->ReadAccuWord(ACCU_DAT_STATE);
		Sleep(2);
		m_wACCU_DAT_REMAIN=getModel()->getI2C()->ReadAccuWord(ACCU_DAT_REMAIN);
		Sleep(2);
		m_wACCU_DAT_PERCENT=getModel()->getI2C()->ReadAccuWord(ACCU_DAT_PERCENT);
		Sleep(2);
		m_wACCU_DAT_CAPACITY=getModel()->getI2C()->ReadAccuWord(ACCU_DAT_CAPACITY);
		Sleep(2);
		m_wACCU_DAT_VOLTAGE=getModel()->getI2C()->ReadAccuWord(ACCU_DAT_VOLTAGE);
		Sleep(2);
		m_wACCU_DAT_DISCHARGECURRENT=getModel()->getI2C()->ReadAccuWord(ACCU_DAT_DISCHARGECURRENT);
		Sleep(2);
		m_wACCU_DAT_CHARGECURRENT=getModel()->getI2C()->ReadAccuWord(ACCU_DAT_CHARGECURRENT);
		Sleep(2);
		m_wACCU_DAT_TEMPERATURE=getModel()->getI2C()->ReadAccuWord(ACCU_DAT_TEMPERATURE);
		Sleep(2);
		m_wACCU_DAT_CYCLES=getModel()->getI2C()->ReadAccuWord(ACCU_DAT_CYCLES);
		Sleep(2);
		m_wACCU_DAT_VERSION=getModel()->getI2C()->ReadAccuWord(ACCU_DAT_VERSION);
		Sleep(2);
		m_wACCU_DAT_CHECKSUM=getModel()->getI2C()->ReadAccuWord(ACCU_DAT_CHECKSUM);

	}

	
	

	


	SetTimer(SERVICETIMER, 2000, NULL);

}
void CWndServiceAccuboard::Show(bool bShow)
{
	if(bShow)
	{
		SetWindowPos(NULL,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_SHOWWINDOW);
		m_iOpMinBatt=getModel()->getDATAHANDLER()->getOpTimeBatteryMin(false);

		Draw();
	}
	else
	{
		SetWindowPos(NULL,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_HIDEWINDOW);
	}


}
void CWndServiceAccuboard::Draw()
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

	

	//**************************Version**********************************//
	rc.left = 40;  
	rc.top = 40;  
	rc.right  = 280;  
	rc.bottom = 500;
	CStringW cs = _T("Version Accu-PIC:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 280;  
	rc.top = 40;  
	rc.right  = 600;  
	rc.bottom = 500;
	int iXVal=HIBYTE(m_wACCU_DAT_VERSION);
	int iYVal=LOBYTE(m_wACCU_DAT_VERSION);
	cs.Format(_T("%d.%d"), iXVal, iYVal);
	//cs = _T("xxx");
	DrawText(hdcMem,cs,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

	//**************************Checksum**********************************//
	rc.left = 40;  
	rc.top = 65;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("Checksum:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 280;  
	rc.top = 65;  
	rc.right  = 600;  
	rc.bottom = 500;
	cs.Format(_T("%X"), m_wACCU_DAT_CHECKSUM);
	//cs = _T("xxx");
	DrawText(hdcMem,cs,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);


	//**************************UBATT**********************************//
	rc.left = 40;  
	rc.top = 90;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("Remaining time battery:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 280;  
	rc.top = 90;  
	rc.right  = 600;  
	rc.bottom = 500;
	cs.Format(_T("%d min"), m_wACCU_DAT_REMAIN);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	
	rc.left = 40;  
	rc.top = 115;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("Charging state battery:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
	

	rc.left = 280;  
	rc.top = 115;  
	rc.right  = 600;  
	rc.bottom = 500;
	cs.Format(_T("%d percent"), m_wACCU_DAT_PERCENT);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	//**************************Accu supply**********************************//
	rc.left = 40;  
	rc.top = 140;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("Accu-State:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	BOOL bACCU_STATE_ACCUOP=FALSE;
	BOOL bACCU_STATE_MAINOP=FALSE;
	BOOL bACCU_STATE_EXTRNOP=FALSE;
	BOOL bACCU_STATE_CHARGE=FALSE;
	BOOL bACCU_STATE_FULL=FALSE;
	BOOL bACCU_STATE_NEW=FALSE;
	BOOL bACCU_STATE_HOT=FALSE;
	BOOL bACCU_STATE_COLD=FALSE;
	BOOL bACCU_STATE_CALPHASE1=FALSE;
	BOOL bACCU_STATE_CALPHASE2=FALSE;
	BOOL bACCU_STATE_CALPHASE3=FALSE;
	BOOL bACCU_STATE_CALPHASE4=FALSE;
	BOOL bACCU_STATE_CALSUCCESS=FALSE;
	BOOL bACCU_STATE_CALFAIL=FALSE;
	BOOL bACCU_STATE_SHUTDOWN=FALSE;
	BOOL bACCU_STATE_CHECKSUMFAIL=FALSE;

	if(m_wACCU_DAT_STATE & BIT0)
		bACCU_STATE_ACCUOP=TRUE;

	if(m_wACCU_DAT_STATE & BIT1)
		bACCU_STATE_MAINOP=TRUE;

	if(m_wACCU_DAT_STATE & BIT2)
		bACCU_STATE_EXTRNOP=TRUE;

	if(m_wACCU_DAT_STATE & BIT3)
		bACCU_STATE_CHARGE=TRUE;

	if(m_wACCU_DAT_STATE & BIT4)
		bACCU_STATE_FULL=TRUE;

	if(m_wACCU_DAT_STATE & BIT5)
		bACCU_STATE_NEW=TRUE;

	if(m_wACCU_DAT_STATE & BIT6)
		bACCU_STATE_HOT=TRUE;

	if(m_wACCU_DAT_STATE & BIT7)
		bACCU_STATE_COLD=TRUE;

	if(m_wACCU_DAT_STATE & BIT8)
		bACCU_STATE_CALPHASE1=TRUE;

	if(m_wACCU_DAT_STATE & BIT9)
		bACCU_STATE_CALPHASE2=TRUE;

	if(m_wACCU_DAT_STATE & BIT10)
		bACCU_STATE_CALPHASE3=TRUE;

	if(m_wACCU_DAT_STATE & BIT11)
		bACCU_STATE_CALPHASE4=TRUE;

	if(m_wACCU_DAT_STATE & BIT12)
		bACCU_STATE_CALSUCCESS=TRUE;

	if(m_wACCU_DAT_STATE & BIT13)
		bACCU_STATE_CALFAIL=TRUE;

	if(m_wACCU_DAT_STATE & BIT14)
		bACCU_STATE_SHUTDOWN=TRUE;

	if(m_wACCU_DAT_STATE & BIT15)
		bACCU_STATE_CHECKSUMFAIL=TRUE;

	rc.left = 280;  
	rc.top = 140;  
	rc.right  = 600;  
	rc.bottom = 500;
	cs.Format(_T("ACCUOP: %d, MAINOP: %d, EXTRNOP: %d"), 
		bACCU_STATE_ACCUOP,
		bACCU_STATE_MAINOP,
		bACCU_STATE_EXTRNOP);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 280;  
	rc.top = 160;  
	rc.right  = 600;  
	rc.bottom = 500;
	cs.Format(_T("CHARGE: %d FULL: %d NEW: %d, HOT: %d"), 
		bACCU_STATE_CHARGE,
		bACCU_STATE_FULL,
		bACCU_STATE_NEW,
		bACCU_STATE_HOT);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 280;  
	rc.top = 180;  
	rc.right  = 600;  
	rc.bottom = 500;
	cs.Format(_T("COLD: %d, PH1: %d, PH2: %d, PH3: %d"), 
		bACCU_STATE_COLD,
		bACCU_STATE_CALPHASE1,
		bACCU_STATE_CALPHASE2,
		bACCU_STATE_CALPHASE3);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


	rc.left = 280;  
	rc.top = 200;  
	rc.right  = 600;  
	rc.bottom = 500;
	cs.Format(_T("PH4: %d, SUCC: %d, FAIL: %d, SHUTD: %d, CSM: %d"), 
		bACCU_STATE_CALPHASE4,
		bACCU_STATE_CALSUCCESS,
		bACCU_STATE_CALFAIL,
		bACCU_STATE_SHUTDOWN,
		bACCU_STATE_CHECKSUMFAIL);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	//**************************Operating hours accu**********************************//
	rc.left = 40;  
	rc.top = 225;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("Operating hours battery:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


	int iOpHrs = m_iOpMinBatt/60;
	if(iOpHrs>0)
		m_iOpMinBatt=m_iOpMinBatt-(iOpHrs*60);

	rc.left = 280;  
	rc.top = 225;  
	rc.right  = 600;  
	rc.bottom = 500;
	//cs = _T("200 h");
	cs.Format(_T("%d%s %d%s"),iOpHrs,_T(" h"),m_iOpMinBatt,_T(" min"));
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);



	//**************************Voltage battery**********************************//
	rc.left = 40;  
	rc.top = 250;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("Voltage battery: [V]");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 280;  
	rc.top = 250;  
	rc.right  = 600;  
	rc.bottom = 500;
	cs.Format(_T("%0.1f"), CTlsFloat::Round(((double)m_wACCU_DAT_VOLTAGE)/1000, 1));
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


	//**************************Capacitance battery**********************************//
	rc.left = 40;  
	rc.top = 275;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("Capacitance battery: [Ah]");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 280;  
	rc.top = 275;  
	rc.right  = 600;  
	rc.bottom = 500;
	cs.Format(_T("%0.3f"), CTlsFloat::Round(((double)m_wACCU_DAT_CAPACITY)/1000, 3));
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	//**************************Discharge current**********************************//
	rc.left = 40;  
	rc.top = 300;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("Discharge current: [mA]");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 280;  
	rc.top = 300;  
	rc.right  = 600;  
	rc.bottom = 500;
	cs.Format(_T("%d"), m_wACCU_DAT_DISCHARGECURRENT);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


	//**************************Charge current**********************************//
	rc.left = 40;  
	rc.top = 325;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("Charge current: [mA]");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 280;  
	rc.top = 325;  
	rc.right  = 600;  
	rc.bottom = 500;
	cs.Format(_T("%d"), m_wACCU_DAT_CHARGECURRENT);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	//**************************Temperature**********************************//
	rc.left = 40;  
	rc.top = 350;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("Temperature: [°C]");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 280;  
	rc.top = 350;  
	rc.right  = 600;  
	rc.bottom = 500;
	cs.Format(_T("%0.1f"), CTlsFloat::Round(((double)m_wACCU_DAT_TEMPERATURE)/10, 1));
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	//**************************Cycles**********************************//
	rc.left = 40;  
	rc.top = 375;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("Cycles:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 280;  
	rc.top = 375;  
	rc.right  = 600;  
	rc.bottom = 500;
	cs.Format(_T("%0.1f"), CTlsFloat::Round(((double)m_wACCU_DAT_CYCLES)/10, 1));
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


	//**************************BUTTONS**********************************//
	//CDC* pDCMem=CDC::FromHandle(hdcMem);
	/*CPen penLine;
	penLine.CreatePen(PS_SOLID,1,RGB(140,140,140));
	SelectObject(hdcMem,(HPEN)penLine);
	CBrush cbrRound(RGB(200,200,200));
	SelectObject(hdcMem,cbrRound);
	RoundRect(hdcMem, 10, 400, 580, 515,20,20);

	rc.left = 20;  
	rc.top = 425;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("Operating hours:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);*/

	/*rc.left = 20;  
	rc.top = 475;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("Capacitance of battery:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);*/



	//dc.BitBlt(0,0,m_lX,m_lY,CDC::FromHandle(hdcMem),0,0,SRCCOPY);
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

//************************************
// Method:    OnDestroy
// FullName:  CWndServiceAccuboard::OnDestroy
// Access:    public 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CWndServiceAccuboard::OnDestroy()
{
	KillTimer(SERVICETIMER);

	if(m_pDlg)
	{
		m_pDlg->EndDialog(IDCLOSE);
	}
	m_pDlg = NULL;

	CWndService::OnDestroy();
}

// **************************************************************************
// 
// **************************************************************************
void CWndServiceAccuboard::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==SERVICETIMER)
	{
		if(getModel()->getI2C()->IsI2C_ACCUavailability())
		{
			m_wACCU_DAT_STATE=getModel()->getI2C()->ReadAccuWord(ACCU_DAT_STATE);
			Sleep(2);
			m_wACCU_DAT_REMAIN=getModel()->getI2C()->ReadAccuWord(ACCU_DAT_REMAIN);
			Sleep(2);
			m_wACCU_DAT_PERCENT=getModel()->getI2C()->ReadAccuWord(ACCU_DAT_PERCENT);
			Sleep(2);
			m_wACCU_DAT_CAPACITY=getModel()->getI2C()->ReadAccuWord(ACCU_DAT_CAPACITY);
			Sleep(2);
			m_wACCU_DAT_VOLTAGE=getModel()->getI2C()->ReadAccuWord(ACCU_DAT_VOLTAGE);
			Sleep(2);
			m_wACCU_DAT_DISCHARGECURRENT=getModel()->getI2C()->ReadAccuWord(ACCU_DAT_DISCHARGECURRENT);
			Sleep(2);
			m_wACCU_DAT_CHARGECURRENT=getModel()->getI2C()->ReadAccuWord(ACCU_DAT_CHARGECURRENT);
			Sleep(2);
			m_wACCU_DAT_TEMPERATURE=getModel()->getI2C()->ReadAccuWord(ACCU_DAT_TEMPERATURE);
			Sleep(2);
			m_wACCU_DAT_CYCLES=getModel()->getI2C()->ReadAccuWord(ACCU_DAT_CYCLES);
			Sleep(2);
			m_wACCU_DAT_VERSION=getModel()->getI2C()->ReadAccuWord(ACCU_DAT_VERSION);
			Sleep(2);
			m_wACCU_DAT_CHECKSUM=getModel()->getI2C()->ReadAccuWord(ACCU_DAT_CHECKSUM);
		}
		else
		{
			m_wACCU_DAT_STATE=0;
			m_wACCU_DAT_REMAIN=0;
			m_wACCU_DAT_PERCENT=0;
			m_wACCU_DAT_CAPACITY=0;
			m_wACCU_DAT_VOLTAGE=0;
			m_wACCU_DAT_DISCHARGECURRENT=0;
			m_wACCU_DAT_CHARGECURRENT=0;
			m_wACCU_DAT_TEMPERATURE=0;
			m_wACCU_DAT_CYCLES=0;
			m_wACCU_DAT_VERSION=0;
			m_wACCU_DAT_CHECKSUM=0;
		}
		


		//m_iPARST0_Data=getModel()->getSPI()->Read_STATUS();
		m_iOpMinBatt=getModel()->getDATAHANDLER()->getOpTimeBatteryMin(false);
		

		//m_iVCC5_Data=getModel()->getSPI()->Read_VCC_5_ADC();
		//m_iVCC24_Data=getModel()->getSPI()->Read_VCC_24_BLEND_ADC();


		Draw();

	}

	CWnd::OnTimer(nIDEvent);
}

// **************************************************************************
// 
// **************************************************************************
void CWndServiceAccuboard::OnBnClickedAccuTest()
{
	//int iRes=0;

	CDlgMessageBox box(this,_T("Battery capacitance"),_T("- not implemented yet -"),MB_YESNO,IDB_MSG_STOP);
	m_pDlg = &box;
	int iRes=box.DoModal();
	m_pDlg = NULL;

	if(iRes==IDCLOSE)
	{
		return;
	}

	UpdateWindow();
	Draw();
	return;

	//if(getModel()->getDATAHANDLER()->IsAccuSupply()==true)
	//{
	//	CDlgMessageBox box1(this,_T("NOT POSSIBLE"),_T("Capacitance test is not possible. You have to connect power supply first!"),MB_OK);
	//	m_pDlg = &box1;
	//	iRes=box1.DoModal();
	//	m_pDlg = NULL;

	//	UpdateWindow();
	//	Draw();

	//	return;
	//}
	//CDlgMessageBox box(this,_T("Battery capacitance"),_T("This will start the battery capacitance test. Continue?"),MB_YESNO);
	////CDlgMessageBox box(this,_T("Battery capacitance"),_T("- not implemented yet -"),MB_YESNO);
	//m_pDlg = &box;
	//iRes=box.DoModal();
	//m_pDlg = NULL;

	//
	//
	//if(iRes==IDCLOSE)
	//{
	//	return;
	//}

	//UpdateWindow();
	//Draw();
	//if(iRes==IDYES)
	//{

	//}
}
