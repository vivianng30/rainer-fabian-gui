// WndServiceFlowboard.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "WndServiceFlowboard.h"


// CWndServiceFlowboard

IMPLEMENT_DYNAMIC(CWndServiceFlowboard, CWnd)

CWndServiceFlowboard::CWndServiceFlowboard():
CWndService()
{
	/*m_pcFlowCorr_Up=NULL;
	m_pcFlowCorr_Dw=NULL;
	m_pcFlowCorr=NULL;

	m_iFCOR=0;*/
	m_iADC0_I_Data=0;
	m_iADC0_I_Offs=0;
	m_iADC0_I_Scl=0;
	m_iADC0_I_Dac=0;
	m_iADC1_E_Data=0;
	m_iADC1_E_Offs=0;
	m_iADC1_E_Scl=0;
	m_iADC1_E_Dac=0;
	m_iFSCAL_Offs=0;
	m_iFlow=0;
	m_iPressure=0;
	m_iVolume=0;
	m_iOxy=0;
	m_iBLENDERstate=0;
}

CWndServiceFlowboard::~CWndServiceFlowboard()
{
}


BEGIN_MESSAGE_MAP(CWndServiceFlowboard, CWnd)
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CWndServiceFlowboard message handlers
void CWndServiceFlowboard::Init()
{
	CClientDC dc(this);

	//m_pcFlowCorr_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_FG_UP);
	//m_pcFlowCorr_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_FG_DW);

	////m_dbFCOR=1.0f;
	//m_iFCOR=getModel()->getSPI()->Read_FLOW_CORFACTOR();

	//
	//if(m_iFCOR<500 || m_iFCOR>1500)
	//	m_iFCOR=1000;

	BTN btn;

	
	//VALUE v;

	//v.nLowerLimit=500;
	//v.nUpperLimit=1500;

	//v.nValue=m_iFCOR;
	////v.tText[0]=0;

	//btn.wID					= IDC_BTN_SERVICE_FLOWCORR;	
	//btn.poPosition.x		= 650;//50;
	//btn.poPosition.y		= 35;//300;
	//btn.pcBmpUp				= m_pcFlowCorr_Up;
	//btn.pcBmpDown			= m_pcFlowCorr_Dw;
	//btn.pcBmpFocus			= m_pcFlowCorr_Up;
	//btn.pcBmpDisabled		= m_pcFlowCorr_Up;
	//btn.dwFormat			= DT_VCENTER|DT_CENTER;
	//m_pcFlowCorr=new CUDBtn(btn,COLOR_TXTBTNUP,false);
	//m_pcFlowCorr->Create(this,g_hf7AcuNorm,g_hf27AcuBold,v);
	//m_pcFlowCorr->SetText(_T("Flow Corr"),false);

	m_pcMenuBack->ShowWindow(SW_SHOW);

	SetTimer(SERVICETIMER, 800, NULL);

}

void CWndServiceFlowboard::Draw()
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

	CStringW cs = _T("");
	
	//**************************DATA,OFFS,SCL**********************************//
	/*rc.left = 20;  
	rc.top = 35;  
	rc.right  = 570;  
	rc.bottom = 60;
	FillRect(hdcMem,&rc,cbrWhite);

	
	rc.left = 200;  
	rc.top = 40;  
	rc.right  = 600;  
	rc.bottom = 500;
	cs = _T("ADC");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 280;  
	cs = _T("OFFS");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 360;  
	cs = _T("SCL");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 440;  
	cs = _T("DAC");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);*/

	//**************************ADC0/I**********************************//
	/*rc.left = 60;  
	rc.top = 65;  
	cs = _T("I-Flow");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 200;  
	cs.Format(_T("%d"), m_iADC0_I_Data);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 280;  
	cs.Format(_T("%d"), m_iADC0_I_Offs);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 360;  
	cs.Format(_T("%d"), m_iADC0_I_Scl);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 440;  
	cs.Format(_T("%d"), m_iADC0_I_Dac);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);*/


	//**************************ADC1/E**********************************//
	/*rc.left = 60;  
	rc.top = 85;  
	cs = _T("E-Flow");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 200;  
	cs.Format(_T("%d"), m_iADC1_E_Data);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 280;  
	cs.Format(_T("%d"), m_iADC1_E_Offs);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 360;  
	cs.Format(_T("%d"), m_iADC1_E_Scl);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 440;  
	cs.Format(_T("%d"), m_iADC1_E_Dac);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);*/


	//**************************FSCAL**********************************//
	/*rc.left = 60;  
	rc.top = 105;  
	rc.right  = 230;  
	cs = _T("FSCAL");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 200;  
	rc.right  = 600;  
	cs.Format(_T("%d"), m_iFSCAL_Offs);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);*/


	//**************************Messwerte**********************************//
	rc.left = 20;  
	rc.top = 35;//155;  
	rc.right  = 570;  
	rc.bottom = 60;//180;
	FillRect(hdcMem,&rc,cbrWhite);


	rc.left = 40;  
	rc.top = 40;//160;  
	rc.right  = 600;  
	rc.bottom = 500;
	cs = _T("Measured data");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


	rc.left = 60; 
	rc.top = 70;//190;  
	cs = _T("Flow:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.right  = 270;
	cs.Format(_T("%0.1f"), CTlsFloat::Round(((double)m_iFlow)/G_FACTOR_FLOW, 1));//Achtung, tatsächlicher Meß-Wert ist um Faktor 2 größer
	DrawText(hdcMem,cs,-1,&rc,DT_RIGHT|DT_TOP|DT_SINGLELINE);

	rc.left = 290; 
	rc.right  = 400;
	cs = _T("l/min");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


	rc.left = 60;  
	rc.top = 90;//210;  
	cs = _T("Pressure:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.right  = 270;
	cs.Format(_T("%0.1f"), CTlsFloat::Round(((double)m_iPressure)/10, 1));
	DrawText(hdcMem,cs,-1,&rc,DT_RIGHT|DT_TOP|DT_SINGLELINE);

	rc.left = 290; 
	rc.right  = 400;
	cs = _T("mbar");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


	rc.left = 60;  
	rc.top = 110;//230;  
	cs = _T("Volume:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.right  = 270;
	cs.Format(_T("%0.1f"), CTlsFloat::Round(((double)m_iVolume)/10, 1));
	DrawText(hdcMem,cs,-1,&rc,DT_RIGHT|DT_TOP|DT_SINGLELINE);

	rc.left = 290;
	rc.right  = 400;
	cs = _T("ml");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


	rc.left = 60;  
	rc.top = 130;//250;  
	cs = _T("Oxy:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.right  = 270;
	if(m_iOxy==-1)
	{
		cs = _T("--");
	}
	else
	{
		//cs.Format(_T("%d"), m_iOxy/10);
		cs.Format(_T("%0.1f"), CTlsFloat::Round(((double)m_iOxy)/10, 1));
	}
	DrawText(hdcMem,cs,-1,&rc,DT_RIGHT|DT_TOP|DT_SINGLELINE);

	rc.left = 290; 
	rc.right  = 400;
	cs = _T("percent");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);




	//**************************Blender state**********************************//
	rc.left = 20;  
	rc.top = 180;//300;  
	rc.right  = 570;  
	rc.bottom = 205;//325;
	FillRect(hdcMem,&rc,cbrWhite);


	rc.left = 40;  
	rc.top = 185;//305;  
	rc.right  = 600;  
	rc.bottom = 500;
	cs = _T("Blender state / Exhalation Valve");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	BOOL bBit=FALSE;

	rc.left = 60; 
	rc.top = 215;//335;  
	cs = _T("Flow_AIR:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	if(m_iBLENDERstate & BIT0)
		bBit=TRUE;
	else
		bBit=FALSE;

	rc.right  = 270;
	cs.Format(_T("%d"), bBit);
	DrawText(hdcMem,cs,-1,&rc,DT_RIGHT|DT_TOP|DT_SINGLELINE);


	rc.left = 60;  
	rc.top = 235;//355;  
	cs = _T("Flow_Oxi:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	if(m_iBLENDERstate & BIT1)
		bBit=TRUE;
	else
		bBit=FALSE;

	rc.right  = 270;
	cs.Format(_T("%d"), bBit);
	DrawText(hdcMem,cs,-1,&rc,DT_RIGHT|DT_TOP|DT_SINGLELINE);


	rc.left = 60;  
	rc.top = 255;//375;  
	cs = _T("Communication:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	if(m_iBLENDERstate & BIT2)
		bBit=TRUE;
	else
		bBit=FALSE;

	rc.right  = 270;
	cs.Format(_T("%d"), bBit);
	DrawText(hdcMem,cs,-1,&rc,DT_RIGHT|DT_TOP|DT_SINGLELINE);



	rc.left = 60;  
	rc.top = 275;//395;  
	cs = _T("Hotwire AIR:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	if(m_iBLENDERstate & BIT3)
		bBit=TRUE;
	else
		bBit=FALSE;

	rc.right  = 270;
	cs.Format(_T("%d"), bBit);
	DrawText(hdcMem,cs,-1,&rc,DT_RIGHT|DT_TOP|DT_SINGLELINE);

	

	rc.left = 60;  
	rc.top = 295;//415;  
	cs = _T("Hotwire Oxi:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	if(m_iBLENDERstate & BIT4)
		bBit=TRUE;
	else
		bBit=FALSE;

	rc.right  = 270;
	cs.Format(_T("%d"), bBit);
	DrawText(hdcMem,cs,-1,&rc,DT_RIGHT|DT_TOP|DT_SINGLELINE);



	rc.left = 60;  
	rc.top = 315;//435;  
	cs = _T("Checksum state:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	if(m_iBLENDERstate & BIT5)
		bBit=TRUE;
	else
		bBit=FALSE;

	rc.right  = 270;
	cs.Format(_T("%d"), bBit);
	DrawText(hdcMem,cs,-1,&rc,DT_RIGHT|DT_TOP|DT_SINGLELINE);

	rc.left = 60;  
	rc.top = 335;//455;  
	cs = _T("Self Test:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	if(m_iBLENDERstate & BIT6)
		bBit=TRUE;
	else
		bBit=FALSE;

	rc.right  = 270;
	cs.Format(_T("%d"), bBit);
	DrawText(hdcMem,cs,-1,&rc,DT_RIGHT|DT_TOP|DT_SINGLELINE);



	/****************** Exh Valve************************************/
	rc.right  = 600;  
	rc.bottom = 500;
	rc.left = 340;  
	rc.top = 215;//335;  
	cs = _T("Exh. Valve:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	if(m_iBLENDERstate & BIT8)
		bBit=TRUE;
	else
		bBit=FALSE;

	rc.right  = 510;
	cs.Format(_T("%d"), bBit);
	DrawText(hdcMem,cs,-1,&rc,DT_RIGHT|DT_TOP|DT_SINGLELINE);



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


//************************************
// Method:    OnDestroy
// FullName:  CWndServiceFlowboard::OnDestroy
// Access:    public 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CWndServiceFlowboard::OnDestroy()
{
	KillTimer(SERVICETIMER);

	CWndService::OnDestroy();	
}


// **************************************************************************
// 
// **************************************************************************
void CWndServiceFlowboard::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==SERVICETIMER)
	{
		//m_dbFCOR=1.0f;
		//m_dbFCOR=getModel()->getSPI()->Read_FLOW_CORFACTOR();
		m_iFSCAL_Offs=0;
		m_iADC0_I_Data=getModel()->getSPI()->Read_FLOW_INSP_ADC();
		m_iADC0_I_Offs=getModel()->getSPI()->Read_SYST_OFFSET_FLOW_INSP();
		m_iADC0_I_Scl=getModel()->getSPI()->Read_SYST_SCALE_FLOW_INSP();
		m_iADC0_I_Dac=getModel()->getSPI()->Read_DAC_FLOW_INSP();

		m_iADC1_E_Data=getModel()->getSPI()->Read_FLOW_EXH_ADC();
		m_iADC1_E_Offs=getModel()->getSPI()->Read_SYST_OFFSET_FLOW_EXH();
		m_iADC1_E_Scl=getModel()->getSPI()->Read_SYST_SCALE_FLOW_EXH();
		m_iADC1_E_Dac=getModel()->getSPI()->Read_DAC_FLOW_EXH();

		m_iFlow=getModel()->getDATAHANDLER()->getMessureDataFLOW();
		m_iPressure=getModel()->getDATAHANDLER()->getMessureDataPRESSURE();
		m_iVolume=getModel()->getDATAHANDLER()->getMessureDataVOLUME();
		m_iOxy=getModel()->getDATAHANDLER()->getMessureDataO2();
		
		if(getModel()->getSERIAL()->GetBLENDER_StatusByte())
			m_iBLENDERstate=getModel()->getDATAHANDLER()->GetMSTATUS_BlenderPIC();

		Draw();

	}

	CWnd::OnTimer(nIDEvent);
}




//LRESULT CWndServiceFlowboard::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
//{
//	switch(message)
//	{
//	case WM_UPDATE_UDBTN:
//		{
//			switch(lParam)
//			{
//			case IDC_BTN_SERVICE_FLOWCORR:
//				{
//					int iVal=wParam;
//					getModel()->getSPI()->Send_FLOW_CORFACTOR(iVal);
//					return 1;
//				}
//				break;
//			}
//		}
//		break;
//	}
//	return CWndService::WindowProc(message, wParam, lParam);
//}
