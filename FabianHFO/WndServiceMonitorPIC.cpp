// WndServiceMonitorPIC.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "WndServiceMonitorPIC.h"

/**********************************************************************************************//**
 * CWndServiceMonitorPIC
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CWndServiceMonitorPIC, CWnd)

/**********************************************************************************************//**
 * Initializes a new instance of the CWndServiceMonitorPIC class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

CWndServiceMonitorPIC::CWndServiceMonitorPIC():
CWndService()
{
	
	m_dataMainboard.m_iConductorChecksum=0;
	m_dataMainboard.m_iMonitorChecksum=0;
	m_dataMainboard.m_iMonitorVersion_x=0;
	m_dataMainboard.m_iMonitorVersion_y=0;
	m_dataMainboard.m_iMonitorVersion_z=0;
	m_dataMainboard.m_iHfoChecksum=0;

	getModel()->getDATAHANDLER()->getMainboardData(&m_dataMainboard);

	m_iADC0_I_Data=0;
	m_iADC0_I_Offs=0;
	m_iADC0_I_Scl=0;
	m_iADC1_E_Data=0;
	m_iADC1_E_Offs=0;
	m_iADC1_E_Scl=0;
	m_iPPROX_Data=0;//
	m_iPPROX_Offs=0;
	m_iPPROX_Scl=0;
	m_iOXY1_Data=0;
	m_iOXY1_Offs=0;
	m_iOXY1_Scl=0;
	m_iOXY2_Data=0;
	m_iOXY2_Offs=0;
	m_iOXY2_Scl=0;
	m_iP_AIR_Data=0;//
	m_iP_OXY_Data=0;//
	m_iP_MIX_Data=0;//
	m_wBattStatMinutes=0;
	m_wBattStatPercents=0;
	m_iOXYGEN_Data=0;
	m_iPARST0_Data=0;
	m_iVCC5_Data=0;
	m_iVCC24_Data=0;
	
}

/**********************************************************************************************//**
 * Finalizes an instance of the CWndServiceMonitorPIC class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

CWndServiceMonitorPIC::~CWndServiceMonitorPIC()
{
}


BEGIN_MESSAGE_MAP(CWndServiceMonitorPIC, CWnd)
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * CWndServiceMonitorPIC message handlers
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndServiceMonitorPIC::Init()
{
	m_pcMenuBack->ShowWindow(SW_SHOW);

	SetTimer(SERVICETIMER, 500, NULL);
}

/**********************************************************************************************//**
 * Draws this instance
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndServiceMonitorPIC::Draw()
{
	RECT rcCl;
	RECT rc;
	GetClientRect(&rcCl);

	CClientDC dc(this);
	HDC hdcMem=CreateCompatibleDC(dc.m_hDC);
	HBITMAP hBmpMem=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
	HBITMAP hBmpMemPrev=(HBITMAP)SelectObject(hdcMem,hBmpMem);


	CBrush cbrBack(RGB(181,178,181));
	CBrush cbrWhite(RGB(255,255,255));
	HBRUSH hbrprev=(HBRUSH)SelectObject(hdcMem,cbrBack);
	HPEN hpenprev=(HPEN)SelectObject(hdcMem, (HPEN)GetStockObject(NULL_PEN));	
	int nBkMode=SetBkMode(hdcMem,TRANSPARENT);
	HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf8AcuBold);
	int tc=SetTextColor(hdcMem,0x0000000);

	Rectangle(hdcMem, 0, 0, m_lX, m_lY);

	//**************************Monitor PIC**********************************//
	rc.left = 40;  
	rc.top = 10;  
	rc.right  = 280;  
	rc.bottom = 500;
	CStringW cs = _T("Version Monitor PIC:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 280;  
	rc.top = 10;  
	rc.right  = 600;  
	rc.bottom = 500;
	cs.Format(_T("%d.%d.%d, Checksum: %X"), 
		m_dataMainboard.m_iMonitorVersion_x, 
		m_dataMainboard.m_iMonitorVersion_y, 
		m_dataMainboard.m_iMonitorVersion_z, 
		m_dataMainboard.m_iMonitorChecksum);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	//**************************UBATT**********************************//
	rc.left = 40;  
	rc.top = 30;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("ACCU remaining time:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 280;  
	rc.right  = 600;  
	rc.bottom = 500;
	cs.Format(_T("%d min"), m_wBattStatMinutes);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


	rc.left = 40;  
	rc.top = 50;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("ACCU charging state:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 280;  
	rc.right  = 600;  
	rc.bottom = 500;
	cs.Format(_T("%d percent"), m_wBattStatPercents);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	//**************************OXYGEN**********************************//
	rc.left = 40;  
	rc.top = 70;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("OXYGEN");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	
	rc.left = 280;  
	rc.right  = 600;  
	rc.bottom = 500;
	if(m_iOXYGEN_Data==-1)
	{
		cs = _T("--");
	}
	else
	{
		//cs.Format(_T("%d"), m_iOXYGEN_Data/10);
		cs.Format(_T("%0.1f"), CTlsFloat::Round(((double)m_iOXYGEN_Data)/10, 1));
	}
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);



	//**************************ADC,OFFS,SCL**********************************//
	rc.left = 20;  
	rc.top = 100;  
	rc.right  = 570;  
	rc.bottom = 125;
	FillRect(hdcMem,&rc,cbrWhite);


	rc.left = 250;  
	rc.top = 105;  
	rc.right  = 600;  
	rc.bottom = 500;
	cs = _T("ADC");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 330;  
	cs = _T("OFFS");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 410;  
	cs = _T("SCL");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	//**************************ADC0/I**********************************//
	rc.left = 110;  
	rc.top = 130;  
	rc.right  = 240;  
	cs = _T("I-Flow..............................................");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 250; 
	rc.right  = 600;
	cs.Format(_T("%d"), m_iADC0_I_Data);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 330;  
	cs.Format(_T("%d"), m_iADC0_I_Offs);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 410;  
	cs.Format(_T("%d"), m_iADC0_I_Scl);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


	//**************************ADC1/E**********************************//
	rc.left = 110;  
	rc.top = 150; 
	rc.right  = 240;
	cs = _T("E-Flow..................................................");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 250; 
	rc.right  = 600;
	cs.Format(_T("%d"), m_iADC1_E_Data);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 330;  
	cs.Format(_T("%d"), m_iADC1_E_Offs);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 410;  
	cs.Format(_T("%d"), m_iADC1_E_Scl);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


	//**************************PPROX**********************************//
	rc.left = 110;  
	rc.top = 170;  
	rc.right  = 240;
	cs = _T("PPROX.........................................");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 250; 
	rc.right  = 600;
	cs.Format(_T("%d"), m_iPPROX_Data);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 330;  
	cs.Format(_T("%d"), m_iPPROX_Offs);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 410;  
	cs.Format(_T("%d"), m_iPPROX_Scl);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	//**************************P_MIX**********************************//
	rc.left = 110;  
	rc.top = 190; 
	rc.right  = 240;
	cs = _T("P-insp........................................");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 250;  
	rc.right  = 600;
	cs.Format(_T("%d"), m_iP_MIX_Data);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	//**************************OXY1**********************************//
	rc.left = 110;  
	rc.top = 210; 
	rc.right  = 240;
	cs = _T("OXY1.........................................");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 250; 
	rc.right  = 600;
	cs.Format(_T("%d"), m_iOXY1_Data);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 330;  
	cs.Format(_T("%d"), m_iOXY1_Offs);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 410;  
	cs.Format(_T("%d"), m_iOXY1_Scl);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	//**************************OXY2**********************************//
	rc.left = 110;  
	rc.top = 230; 
	rc.right  = 240;
	cs = _T("OXY2.........................................");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 250; 
	rc.right  = 600;
	cs.Format(_T("%d"), m_iOXY2_Data);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 330;  
	cs.Format(_T("%d"), m_iOXY2_Offs);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 410;  
	cs.Format(_T("%d"), m_iOXY2_Scl);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	//**************************VCC5**********************************//
	rc.left = 110;  
	rc.top = 250; 
	rc.right  = 240;
	cs = _T("VCC5.........................................");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 250; 
	rc.right  = 600;
	cs.Format(_T("%d"), m_iVCC5_Data);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	//**************************VCC24**********************************//
	rc.left = 110;  
	rc.top = 270;  
	rc.right  = 240;
	cs = _T("VCC24.........................................");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 250;  
	rc.right  = 600;
	cs.Format(_T("%d"), m_iVCC24_Data);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);



	//**************************P_AIR**********************************//
	rc.left = 110;  
	rc.top = 290; 
	rc.right  = 240;
	cs = _T("P-AIR.........................................");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 250;  
	rc.right  = 600;
	cs.Format(_T("%d"), m_iP_AIR_Data);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	//**************************P_OXY**********************************//
	rc.left = 110;  
	rc.top = 310; 
	rc.right  = 240;
	cs = _T("P-OXY.........................................");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 250; 
	rc.right  = 600;
	cs.Format(_T("%d"), m_iP_OXY_Data);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	



	//**************************PARST0**********************************//
	rc.left = 20;  
	rc.top = 355;  
	rc.right  = 570;  
	rc.bottom = 380;
	FillRect(hdcMem,&rc,cbrWhite);
	
	rc.left = 40;  
	rc.top = 360;  
	rc.right  = 280;  
	rc.bottom = 530;
	cs = _T("State Monitor PIC:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	
	BOOL bBit=FALSE;

	rc.left = 60;  
	rc.top = 390;  
	rc.right  = 600;
	if(m_iPARST0_Data & BIT0)
		bBit=TRUE;
	else
		bBit=FALSE;

	cs.Format(_T("%s %d"), _T("00 P-InspTube:"), bBit);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	if(m_iPARST0_Data & BIT1)
		bBit=TRUE;
	else
		bBit=FALSE;

	rc.top = 410;  
	cs.Format(_T("%s %d"), _T("01 P-ExhTube:"), bBit);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	if(m_iPARST0_Data & BIT2)
		bBit=TRUE;
	else
		bBit=FALSE;

	rc.top = 430;  
	cs.Format(_T("%s %d"), _T("02 P-Proximal:"), bBit);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	if(m_iPARST0_Data & BIT3)
		bBit=TRUE;
	else
		bBit=FALSE;

	rc.top = 450;  
	cs.Format(_T("%s %d"), _T("03 P-In-Air:"), bBit);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	if(m_iPARST0_Data & BIT4)
		bBit=TRUE;
	else
		bBit=FALSE;

	rc.top = 470;  
	cs.Format(_T("%s %d"), _T("04 P-In-Oxy:"), bBit);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	if(m_iPARST0_Data & BIT5)
		bBit=TRUE;
	else
		bBit=FALSE;

	rc.top = 490;  
	cs.Format(_T("%s %d"), _T("05 Fan defect:"), bBit);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	if(m_iPARST0_Data & BIT6)
		bBit=TRUE;
	else
		bBit=FALSE;

	rc.left = 230;  
	rc.top = 390;  
	cs.Format(_T("%s %d"), _T("06 Disconnection:"), bBit);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	if(m_iPARST0_Data & BIT7)
		bBit=TRUE;
	else
		bBit=FALSE;

	rc.top = 410;  
	cs.Format(_T("%s %d"), _T("07 O2 value false:"), bBit);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	if(m_iPARST0_Data & BIT8)
		bBit=TRUE;
	else
		bBit=FALSE;

	rc.top = 430;  
	cs.Format(_T("%s %d"), _T("08 O2 sensor def:"), bBit);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	if(m_iPARST0_Data & BIT9)
		bBit=TRUE;
	else
		bBit=FALSE;

	rc.top = 450;  
	cs.Format(_T("%s %d"), _T("09 Volume not reached:"), bBit);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	if(m_iPARST0_Data & BIT10)
		bBit=TRUE;
	else
		bBit=FALSE;

	rc.top = 470;  
	cs.Format(_T("%s %d"), _T("10 Voltage control:"), bBit);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	if(m_iPARST0_Data & BIT11)
		bBit=TRUE;
	else
		bBit=FALSE;

	rc.top = 490; 
	cs.Format(_T("%s %d"), _T("11 Relais defect:"), bBit);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	if(m_iPARST0_Data & BIT12)
		bBit=TRUE;
	else
		bBit=FALSE;

	rc.left = 400;  
	rc.top = 390;  
	cs.Format(_T("%s %d"), _T("12 Checksum failure:"), bBit);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	if(m_iPARST0_Data & BIT13)
		bBit=TRUE;
	else
		bBit=FALSE;

	rc.top = 410;  
	cs.Format(_T("%s %d"), _T("13 Tubus blocked:"), bBit);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	if(m_iPARST0_Data & BIT14)
		bBit=TRUE;
	else
		bBit=FALSE;

	rc.top = 430;  
	cs.Format(_T("%s %d"), _T("14 Apnoe:"), bBit);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	if(m_iPARST0_Data & BIT15)
		bBit=TRUE;
	else
		bBit=FALSE;

	rc.top = 450;  
	cs.Format(_T("%s %d"), _T("15 Volume limited:"), bBit);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	



	//dc.BitBlt(0,0,m_lX,m_lY,CDC::FromHandle(hdcMem),0,0,SRCCOPY);
	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,hdcMem,0,0,SRCCOPY);

	SetTextColor(hdcMem,tc);
	SetBkMode(hdcMem,nBkMode);

	SelectObject(hdcMem,hbrprev);
	SelectObject(hdcMem,hpenprev);

	//DeleteObject(cbrBack);
	//DeleteObject(cbrWhite);

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

void CWndServiceMonitorPIC::OnDestroy()
{
	KillTimer(SERVICETIMER);

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

void CWndServiceMonitorPIC::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==SERVICETIMER)
	{
		m_iADC0_I_Data=getModel()->getSPI()->Read_FLOW_INSP_ADC();
		m_iADC0_I_Offs=getModel()->getSPI()->Read_SYST_OFFSET_FLOW_INSP();
		m_iADC0_I_Scl=getModel()->getSPI()->Read_SYST_SCALE_FLOW_INSP();
		m_iADC1_E_Data=getModel()->getSPI()->Read_FLOW_EXH_ADC();
		m_iADC1_E_Offs=getModel()->getSPI()->Read_SYST_OFFSET_FLOW_EXH();
		m_iADC1_E_Scl=getModel()->getSPI()->Read_SYST_SCALE_FLOW_EXH();
		m_iPPROX_Data=getModel()->getSPI()->Read_P_PROX_ADC();
		m_iPPROX_Offs=getModel()->getSPI()->Read_CAL_PRESS_OFFSET();
		m_iPPROX_Scl=getModel()->getSPI()->Read_CAL_PRESS_SCALE();
		m_iOXY1_Data=getModel()->getSPI()->Read_OXY1_ADC();
		m_iOXY1_Offs=0;
		m_iOXY1_Scl=0;
		m_iOXY2_Data=getModel()->getSPI()->Read_OXY2_ADC();
		m_iOXY2_Offs=0;
		m_iOXY2_Scl=0;
		m_iP_AIR_Data=getModel()->getSPI()->Read_DRM_1_ADC();
		m_iP_OXY_Data=getModel()->getSPI()->Read_DRM_2_ADC();
		m_iP_MIX_Data=getModel()->getSPI()->Read_DRM_3_ADC();
		//m_sUBATT_Data=getModel()->getDATAHANDLER()->GetBatteryStat();
		m_wBattStatMinutes=getModel()->getDATAHANDLER()->GetBatteryStatMinutes();
		m_wBattStatPercents=getModel()->getDATAHANDLER()->GetBatteryStatPercent();
		if(m_wBattStatPercents>100)
			m_wBattStatPercents=100;

		//m_bRemainingAccu=LOBYTE(m_sUBATT_Data);
		//m_bChargestate=HIBYTE(m_sUBATT_Data);


		m_iOXYGEN_Data=getModel()->getDATAHANDLER()->getMessureDataO2();
		m_iPARST0_Data=getModel()->getSPI()->Read_STATUS();

		m_iVCC5_Data=getModel()->getSPI()->Read_VCC_5_ADC();
		m_iVCC24_Data=getModel()->getSPI()->Read_VCC_24_BLEND_ADC();
	

		Draw();

	}

	CWnd::OnTimer(nIDEvent);
}