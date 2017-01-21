// WndNumFlowOff.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "WndNumFlowOff_Av.h"
// CWndNumFlowOff_Av

IMPLEMENT_DYNAMIC(CWndNumFlowOff_Av, CWnd)

CWndNumFlowOff_Av::CWndNumFlowOff_Av():
CWndNumeric()
{
	m_szName1=getModel()->GetLanguageString(IDS_PARA_PIP);
	m_szName2=getModel()->GetLanguageString(IDS_PARA_P);
	m_szName3=getModel()->GetLanguageString(IDS_PARA_PEEP);
	m_szName4=getModel()->GetLanguageString(IDS_PARA_FREQ);
	m_szName5=getModel()->GetLanguageString(IDS_PARA_DUOFREQ);
	m_szName6=getModel()->GetLanguageString(IDS_TXT_FLOWSENSOR_OFF);

	//m_szNameNote1=getModel()->GetLanguageString(IDS_PARA_MAX);
	m_szNameNote2=getModel()->GetLanguageString(IDS_PARA_MITT);

	if(getModel()->getCONFIG()->GetPressureUnit()==PUNIT_MBAR)
	{
		m_szUnitPressure=_T("[")+getModel()->GetLanguageString(IDS_UNIT_MBAR)+_T("]");
	}
	else
	{
		m_szUnitPressure=_T("[")+getModel()->GetLanguageString(IDS_UNIT_CMH2O)+_T("]");
	}
	m_szUnitBPM=_T("[")+getModel()->GetLanguageString(IDS_UNIT_BPM)+_T("]");


	if(getModel()->getCONFIG()->GetCurMode()==VM_CPAP)
	{
		m_bCPAP=true;
	}
	else if(getModel()->getCONFIG()->CurModeIsPresetMode() && getModel()->getCONFIG()->GetPrevMode()==VM_CPAP)
	{
		m_bCPAP=true;
	}
	else
		m_bCPAP=false;
}

CWndNumFlowOff_Av::~CWndNumFlowOff_Av()
{
}


BEGIN_MESSAGE_MAP(CWndNumFlowOff_Av, CWnd)
	//{{AFX_MSG_MAP(CWndNumFlowOff_Av)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CURSORBTN_UP, &CWndNumeric::OnBnClickedCursorUp)
	ON_BN_CLICKED(IDC_CURSORBTN_DOWN, &CWndNumeric::OnBnClickedCursorDwn)
	ON_BN_CLICKED(IDC_CURSORBTN_LEFT, &CWndNumeric::OnBnClickedCursorLeft)
	ON_BN_CLICKED(IDC_CURSORBTN_RIGHT, &CWndNumeric::OnBnClickedCursorRight)
	ON_BN_CLICKED(IDC_CURSORBTN_XUP, &CWndNumeric::OnBnClickedMoveXUp)
	ON_BN_CLICKED(IDC_CURSORBTN_XDOWN, &CWndNumeric::OnBnClickedMoveXDown)
	ON_BN_CLICKED(IDC_CURSORBTN_YLEFT, &CWndNumeric::OnBnClickedMoveYLeft)
	ON_BN_CLICKED(IDC_CURSORBTN_YRIGHT, &CWndNumeric::OnBnClickedMoveYRight)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



// CWndNumFlowOff_Av message handlers
//************************************
// Method:    OnDestroy
// FullName:  CWndNumFlowOff_Av::OnDestroy
// Access:    protected 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CWndNumFlowOff_Av::OnDestroy() 
{
	CWndNumeric::OnDestroy();
}


// **************************************************************************
// 
// **************************************************************************
bool CWndNumFlowOff_Av::DrawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter)
{
	bool bTempCPAP=false;
	if(getModel()->getCONFIG()->GetCurMode()==VM_CPAP)
	{
		bTempCPAP=true;
	}
	else if(getModel()->getCONFIG()->CurModeIsPresetMode() && getModel()->getCONFIG()->GetPrevMode()==VM_CPAP)
	{
		bTempCPAP=true;
	}
	else
		bTempCPAP=false;

	if(m_bCPAP!=bTempCPAP)
		m_bCPAP=bTempCPAP;

	EnterCriticalSection(&csDraw);
	bool bReturn = true;
	
	RECT rc={0,0,m_lX,m_lY};

	CClientDC dc(this);
	HDC hdcMem=CreateCompatibleDC(dc.m_hDC);
	HBITMAP hBmpMem=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
	HBITMAP hBmpMemPrev=(HBITMAP)SelectObject(hdcMem,hBmpMem);

	CDC* pDCStatic=CDC::FromHandle(m_hdcStatic);

	if(bFrames)
	{
		DrawFrames(pDCStatic);
	}

	if(bText)
	{
		DrawStaticText(pDCStatic);
	}

	if(bLimits)
	{
		DrawLimits(pDCStatic);
	}

	BitBlt(hdcMem, 0, 0, m_lX, m_lY,m_hdcStatic , 0, 0, SRCCOPY);

	HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf33AcuBoldNum);
	int nBkMode=SetBkMode(hdcMem,TRANSPARENT);
	int nPrevTxtColor=SetTextColor(hdcMem,RGB(0,0,255));

	TCHAR psz[MAX_PATH];

	if(getModel()->getCONFIG()->GetCurMode()==VM_IPPV)
	{
		//###########################################################################
		SHORT iPmax=getModel()->getDATAHANDLER()->getBTBMessureDataPINSP();
		rc.top = 5;
		rc.bottom = 64;
		rc.left = 15; 
		rc.right = 175;
		if(iPmax==0)
			wsprintf(psz,_T("%d"), 0);
		else
			wsprintf(psz,_T("%0.0f"),CTlsFloat::Round(((double)iPmax)/10, 0));
		DrawText(hdcMem,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);

		//###########################################################################
		SHORT iPmitt=getModel()->getDATAHANDLER()->getAVGMessureDataPmitt();
		rc.top = 77;
		rc.bottom = 126;
		rc.left = 15; 
		rc.right = 175;
		if(iPmitt==0)
			wsprintf(psz,_T("%d"), 0);
		else
			wsprintf(psz,_T("%0.1f"),CTlsFloat::Round(((double)iPmitt)/10, 1));
		DrawText(hdcMem,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);

		//###########################################################################
		SHORT iPEEP=getModel()->getDATAHANDLER()->getAVGMessureDataPEEP();
		rc.top = 139;
		rc.bottom = 188;
		rc.left = 15; 
		rc.right = 175;
		if(iPEEP==0)
			wsprintf(psz,_T("%d"), 0);
		else
		{
			wsprintf(psz,_T("%0.1f"), ((double)iPEEP)/10);
		}

		DrawText(hdcMem,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);

		if(false==m_bGraphCursor)
		{
			if(		getModel()->getCONFIG()->GetCO2module()!=CO2MODULE_NONE
				&& getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==true
				&&	getModel()->getCONFIG()->GetCurMode()!=VM_HFO)
			{
				rc.top = 325;
				rc.bottom = 374;
				rc.left = 15; 
				rc.right = 175;
				if(getModel()->getETCO2()!=NULL)
				{
					if(getModel()->getETCO2()->isFreqValueValid())
					{
						BYTE iBPM=getModel()->getDATAHANDLER()->getAVGMessureDataBPM();
						wsprintf(psz,_T("%d"),iBPM);
						DrawText(hdcMem,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);
					}
					else
					{
						DrawText(hdcMem,_T("--"),-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);
					}
				}
				else
				{
					DrawText(hdcMem,_T("--"),-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);
				}
			}
		}
	}
	else
	{
		if(!m_bCPAP)
		{
			//###########################################################################
			SHORT iPmax=getModel()->getDATAHANDLER()->getBTBMessureDataPINSP();
			rc.top = 5;
			rc.bottom = 64;
			rc.left = 15; 
			rc.right = 175;
			if(iPmax==0)
				wsprintf(psz,_T("%d"), 0);
			else
				wsprintf(psz,_T("%0.0f"),CTlsFloat::Round(((double)iPmax)/10, 0));
			DrawText(hdcMem,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);
		}

		//###########################################################################
		SHORT iPmitt=getModel()->getDATAHANDLER()->getAVGMessureDataPmitt();
		rc.top = 77;
		rc.bottom = 126;
		rc.left = 15; 
		rc.right = 175;
		if(iPmitt==0)
			wsprintf(psz,_T("%d"), 0);
		else
			wsprintf(psz,_T("%0.1f"),CTlsFloat::Round(((double)iPmitt)/10, 1));
		DrawText(hdcMem,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);

		if(!m_bCPAP)
		{
			//###########################################################################
			SHORT iPEEP=getModel()->getDATAHANDLER()->getAVGMessureDataPEEP();
			rc.top = 139;
			rc.bottom = 188;
			rc.left = 15; 
			rc.right = 175;
			if(iPEEP==0)
				wsprintf(psz,_T("%d"), 0);
			else
			{
				wsprintf(psz,_T("%0.1f"), ((double)iPEEP)/10);
			}

			DrawText(hdcMem,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);
		}


		if(false==m_bGraphCursor)
		{
			if(		getModel()->getCONFIG()->GetCO2module()!=CO2MODULE_NONE
				&& getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==true
				&&	getModel()->getCONFIG()->GetCurMode()!=VM_HFO)
			{
				rc.top = 325;
				rc.bottom = 374;
				rc.left = 15; 
				rc.right = 175;
				if(getModel()->getETCO2()!=NULL)
				{
					if(getModel()->getETCO2()->isFreqValueValid())
					{
						BYTE iBPM=getModel()->getDATAHANDLER()->getAVGMessureDataBPM();
						wsprintf(psz,_T("%d"),iBPM);
						DrawText(hdcMem,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);
					}
					else
					{
						DrawText(hdcMem,_T("--"),-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);
					}
				}
				else
				{
					DrawText(hdcMem,_T("--"),-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);
				}
			}
		}
	}

	BitBlt(m_hDC, 0, 0, m_lX, m_lY,hdcMem , 0, 0, SRCCOPY);
	BitBlt(dc.m_hDC, 0, 0, m_lX, m_lY, hdcMem, 0, 0, SRCCOPY);

	SetTextColor(hdcMem,nPrevTxtColor);
	SetBkMode(hdcMem,nBkMode);
	
	SelectObject(hdcMem,hPrevFont);
	SelectObject(hdcMem,hBmpMemPrev);
	
	DeleteObject(hBmpMem);
	DeleteDC(hdcMem);

	LeaveCriticalSection(&csDraw);

	return bReturn;
}


// **************************************************************************
// 
// **************************************************************************
bool CWndNumFlowOff_Av::DrawFrames(CDC* pDC)
{
	HDC hdc = *pDC;
	HBRUSH hPrevBrush = (HBRUSH)SelectObject(m_hDC, (HBRUSH)GetStockObject(WHITE_BRUSH));	
	HPEN hPrevPen = (HPEN)SelectObject(m_hDC, (HPEN)GetStockObject(WHITE_PEN));

	if(m_pcNumPara1er)
	{
		if(!m_bCPAP)
			m_pcNumPara1er->Draw(hdc,0,5);
		
		m_pcNumPara1er->Draw(hdc,0,67);
		
		if(!m_bCPAP)
			m_pcNumPara1er->Draw(hdc,0,129);
	}

	RECT rc;
	if(false==m_bGraphCursor)
	{
		if(m_pbtnCursorUp)
			m_pbtnCursorUp->ShowWindow(SW_HIDE);
		if(m_pbtnCursorDown)
			m_pbtnCursorDown->ShowWindow(SW_HIDE);
		if(m_pbtnCursorLeft)
			m_pbtnCursorLeft->ShowWindow(SW_HIDE);
		if(m_pbtnCursorRight)
			m_pbtnCursorRight->ShowWindow(SW_HIDE);

		if(m_pbtnXmoveUp)
			m_pbtnXmoveUp->ShowWindow(SW_HIDE);
		if(m_pbtnXmoveDown)
			m_pbtnXmoveDown->ShowWindow(SW_HIDE);
		if(m_pbtnYmoveLeft)
			m_pbtnYmoveLeft->ShowWindow(SW_HIDE);
		if(m_pbtnYmoveRight)
			m_pbtnYmoveRight->ShowWindow(SW_HIDE);

		rc.left = 0;  
		rc.top = 191;  
		rc.right  = 190;  
		rc.bottom = 446;

		CBrush br(BACKGND);
		FillRect(hdc,&rc,br);

		if(		getModel()->getCONFIG()->GetCO2module()!=CO2MODULE_NONE
			&& getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==true)
		{
			switch(getModel()->getCONFIG()->GetCurMode())
			{
			case VM_IPPV:
			case VM_SIMV:
			case VM_SIMVPSV:
			case VM_SIPPV:
			case VM_PSV:
			case VM_CPAP:
			case VM_NCPAP:
			case VM_THERAPIE:
			case VM_DUOPAP:
				{
					if(m_pcNumPara1er)
						m_pcNumPara1er->Draw(hdc,0,315);
				}
				break;
			default:
				{
					
				}
				break;
			}
		}
	}
	else
	{
		if(m_pcNumInfoTop)
			m_pcNumInfoTop->Draw(hdc,0,191);
		if(m_pcNumInfoBot)
			m_pcNumInfoBot->Draw(hdc,0,419);

		rc.left = 0;  
		rc.top = 208;  
		rc.right  = 190;  
		rc.bottom = 419;

		CBrush br(RGB(176,176,177));
		FillRect(hdc,&rc,br);

		if(m_pbtnCursorUp)
			m_pbtnCursorUp->MoveWindow(70,217,51,68);
		if(m_pbtnCursorDown)
			m_pbtnCursorDown->MoveWindow(70,338,51,68);
		if(m_pbtnCursorLeft)
			m_pbtnCursorLeft->MoveWindow(2,286,68,51);
		if(m_pbtnCursorRight)
			m_pbtnCursorRight->MoveWindow(121,286,68,51);
		if(m_pbtnCursorUp)
			m_pbtnCursorUp->ShowWindow(SW_SHOW);
		if(m_pbtnCursorDown)
			m_pbtnCursorDown->ShowWindow(SW_SHOW);
		if(m_pbtnCursorLeft)
			m_pbtnCursorLeft->ShowWindow(SW_SHOW);
		if(m_pbtnCursorRight)
			m_pbtnCursorRight->ShowWindow(SW_SHOW);
		if(m_pcGraphCursor_pic)
			m_pcGraphCursor_pic->Draw(hdc,74,289);

		
	}
	
	if(m_pcNumPara1er)
		m_pcNumPara1er->Draw(hdc,0,439);

	SelectObject(hdc, hPrevBrush);	
	SelectObject(hdc, hPrevPen);

	return true;
}

// **************************************************************************
// 
// **************************************************************************
bool CWndNumFlowOff_Av::DrawStaticText(CDC* pDC)
{
	HDC hdc = *pDC;
	HFONT hPrevFont=(HFONT)SelectObject(hdc,g_hf11AcuBold);
	int nBkMode=SetBkMode(hdc,TRANSPARENT);
	int nPrevTxtColor=SetTextColor(hdc,RGB(0,0,0));
	RECT rc;
	CStringW szHeader = _T("");

	rc.top = 9;
	rc.bottom = m_lY;
	rc.left = 15; 
	rc.right = 190;

	SelectObject(hdc,g_hf13AcuBold);

	CSize sz1,sz2,sz3,sz6,sz7;

	if(!m_bCPAP)
	{
		pDC->DrawText(m_szName1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		sz1 = pDC->GetTextExtent(m_szName1);
	}
	
	rc.top = 71;
	pDC->DrawText(m_szName2,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	sz2 = pDC->GetTextExtent(m_szName2);

	rc.top = 133;
	if(!m_bCPAP)
	{
		pDC->DrawText(m_szName3,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		sz3 = pDC->GetTextExtent(m_szName3);
	}

	if(		getModel()->getCONFIG()->GetCO2module()!=CO2MODULE_NONE
		&& getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==true
		&&	false==m_bGraphCursor
		&&	getModel()->getCONFIG()->GetCurMode()!=VM_HFO)
	{
		switch(getModel()->getCONFIG()->GetCurMode())
		{
		case VM_IPPV:
		case VM_SIMV:
		case VM_SIMVPSV:
		case VM_SIPPV:
		case VM_PSV:
		case VM_CPAP:
		case VM_NCPAP:
		case VM_THERAPIE:
			{
				rc.top = 319;
				pDC->DrawText(m_szName4,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
				sz6 = pDC->GetTextExtent(m_szName4);
			}
			break;
		case VM_DUOPAP:
			{
				rc.top = 319;
				pDC->DrawText(m_szName5,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
				sz6 = pDC->GetTextExtent(m_szName5);
			}
			break;
		default:
			{
			}
			break;
		}
	}
	
	SetTextColor(hdc,RGB(255,0,0));

	rc.top = 455;
	pDC->DrawText(m_szName6,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

	SetTextColor(hdc,RGB(0,0,0));
	SelectObject(hdc,g_hf7AcuBold);

	CSize /*sz1Note,*/sz2Note,sz7Note;

	/*if(!m_bCPAP)
	{
		rc.top = 13;
		rc.left = 17+sz1.cx;
		pDC->DrawText(m_szNameNote1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		sz1Note = pDC->GetTextExtent(m_szNameNote1);
	}*/

	rc.top = 75;
	rc.left = 17+sz2.cx;
	pDC->DrawText(m_szNameNote2,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	sz2Note = pDC->GetTextExtent(m_szNameNote2);

	SelectObject(hdc,g_hf7AcuNorm);

	rc.top = 10;
	if(!m_bCPAP)
	{
		rc.left = 25+sz1.cx;
		//rc.left = 25+sz1.cx+sz1Note.cx;
		pDC->DrawText(m_szUnitPressure,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	}
	
	rc.top = 72;
	rc.left = 25+sz2.cx+sz2Note.cx;
	pDC->DrawText(m_szUnitPressure,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

	rc.top = 132;
	if(!m_bCPAP)
	{
		rc.left = 25+sz3.cx;
		pDC->DrawText(m_szUnitPressure,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	}

	if(		getModel()->getCONFIG()->GetCO2module()!=CO2MODULE_NONE
		&& getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==true
		&&	false==m_bGraphCursor
		&&	getModel()->getCONFIG()->GetCurMode()!=VM_HFO)
	{
		switch(getModel()->getCONFIG()->GetCurMode())
		{
		case VM_IPPV:
		case VM_SIMV:
		case VM_SIMVPSV:
		case VM_SIPPV:
		case VM_PSV:
		case VM_CPAP:
		case VM_NCPAP:
		case VM_THERAPIE:
		case VM_DUOPAP:
			{
				rc.top = 320;
				rc.left = 25+sz6.cx;
				pDC->DrawText(m_szUnitBPM,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
			}
			break;
		default:
			{
			}
			break;
		}
		
	}

	SetTextColor(hdc,nPrevTxtColor);
	SetBkMode(hdc,nBkMode);
	SelectObject(hdc,hPrevFont);
	return true;
}


// **************************************************************************
// 
// **************************************************************************
bool CWndNumFlowOff_Av::DrawLimits(CDC* pDC)
{
	HDC hdc = *pDC;
	int nBkMode=SetBkMode(hdc,TRANSPARENT);
	HFONT hPrevFont=(HFONT)SelectObject(hdc,g_hf11AcuBold);
	int nPrevTxtColor=SetTextColor(hdc,RGB(200,0,0));
	RECT rc;	
	TCHAR psz[MAX_PATH];

	//################Pmax Max##################
	if(!m_bCPAP)
	{
		if(m_pcLimitHigh_Up)
			m_pcLimitHigh_Up->Draw(hdc,120,11);
		if(m_pcLimitLow_Up)
			m_pcLimitLow_Up->Draw(hdc,120,44);

		rc.top = 9;
		rc.bottom = 59;
		rc.left = 0;
		rc.right = 187;
		if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit() == AL_CALC)
		{
			SelectObject(hdc,g_hf9AcuBold);
			pDC->DrawText(getModel()->GetLanguageString(IDS_TXT_AUTO),&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);
		}
		else if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit() == AL_OFF)
		{
			SelectObject(hdc,g_hf9AcuBold);
			pDC->DrawText(getModel()->GetLanguageString(IDS_TXT_OFF),&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);
		}
		else 
		{
			SelectObject(hdc,g_hf11AcuBoldNum);
			if(getModel()->getALARMHANDLER()->getAlimitPIPmax()==0)
				wsprintf(psz,_T("%d"), 0);
			else
				wsprintf(psz,_T("%0.0f"), CTlsFloat::Round(((double)getModel()->getALARMHANDLER()->getAlimitPIPmax())/10, 0));
			pDC->DrawText(psz,&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);

			if(getModel()->getALARMHANDLER()->getAlimitPIPmin()==0)
				wsprintf(psz,_T("%d"), 0);
			else
				wsprintf(psz,_T("%0.0f"), CTlsFloat::Round(((double)getModel()->getALARMHANDLER()->getAlimitPIPmin())/10, 0));
			pDC->DrawText(psz,&rc,DT_BOTTOM|DT_SINGLELINE|DT_RIGHT);

			if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit() == AL_AUTO)
			{
				rc.bottom = 39;
				SelectObject(hdc,g_hf9AcuBold);
				pDC->DrawText(getModel()->GetLanguageString(IDS_TXT_AUTO),&rc,DT_BOTTOM|DT_SINGLELINE|DT_RIGHT);
			}
		}
	}
	
	//#########################PEEP Min##########################
	if(!m_bCPAP)
	{
		if(m_pcLimitLow_Up)
			m_pcLimitLow_Up->Draw(hdc,120,167);//right-60, bottom-16

		rc.top = 133;
		rc.bottom = 183;
		if(getModel()->getALARMHANDLER()->getAlimitState_PEEPminLimit() == AL_CALC)
		{
			SelectObject(hdc,g_hf9AcuBold);
			pDC->DrawText(getModel()->GetLanguageString(IDS_TXT_AUTO),&rc,DT_BOTTOM|DT_SINGLELINE|DT_RIGHT);
		}
		else if(getModel()->getALARMHANDLER()->getAlimitState_PEEPminLimit() == AL_OFF)
		{
			SelectObject(hdc,g_hf9AcuBold);
			pDC->DrawText(getModel()->GetLanguageString(IDS_TXT_OFF),&rc,DT_BOTTOM|DT_SINGLELINE|DT_RIGHT);
		}
		else 
		{
			SelectObject(hdc,g_hf11AcuBoldNum);
			if(getModel()->getALARMHANDLER()->getAlimitPEEPmin()==0)
				wsprintf(psz,_T("%d"), 0);
			else
				wsprintf(psz,_T("%0.1f"), CTlsFloat::Round(((double)getModel()->getALARMHANDLER()->getAlimitPEEPmin())/10, 1));
			pDC->DrawText(psz,&rc,DT_BOTTOM|DT_SINGLELINE|DT_RIGHT);

			if(getModel()->getALARMHANDLER()->getAlimitState_PEEPminLimit() == AL_AUTO)
			{
				SelectObject(hdc,g_hf9AcuBold);
				pDC->DrawText(getModel()->GetLanguageString(IDS_TXT_AUTO),&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);
			}
		}
	}

	if(		getModel()->getCONFIG()->GetCO2module()!=CO2MODULE_NONE
		&& getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==true
		&&	getModel()->getCONFIG()->GetCurMode()!=VM_HFO
		&&	false==m_bGraphCursor)
	{
		//################BPM max##################
		if(m_pcLimitHigh_Up)
			m_pcLimitHigh_Up->Draw(hdc,120,321);//right-60, top+2

		rc.top = 319;
		rc.bottom = 362;
		if(getModel()->getALARMHANDLER()->getAlimitState_BPMmaxLimit() == AL_CALC)
		{
			SelectObject(hdc,g_hf9AcuBold);
			pDC->DrawText(getModel()->GetLanguageString(IDS_TXT_AUTO),&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);
		}
		else if(getModel()->getALARMHANDLER()->getAlimitState_BPMmaxLimit() == AL_OFF)
		{
			SelectObject(hdc,g_hf9AcuBold);
			pDC->DrawText(getModel()->GetLanguageString(IDS_TXT_OFF),&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);
		}
		else if(getModel()->getALARMHANDLER()->getAlimitState_BPMmaxLimit() != AL_OFF)
		{
			SelectObject(hdc,g_hf11AcuBoldNum);
			wsprintf(psz,_T("%d"), getModel()->getALARMHANDLER()->getAlimitBPMmax());
			pDC->DrawText(psz,&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);
		}
	}

	SetTextColor(hdc,nPrevTxtColor);
	SetBkMode(hdc,nBkMode);
	SelectObject(hdc,hPrevFont);
	return true;
}
