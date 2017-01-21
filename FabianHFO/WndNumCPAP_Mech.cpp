// WndNumCPAP_Mech.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "WndNumCPAP_Mech.h"


// CWndNumCPAP_Mech

IMPLEMENT_DYNAMIC(CWndNumCPAP_Mech, CWnd)

CWndNumCPAP_Mech::CWndNumCPAP_Mech():
CWndNumeric()
{
	m_szName1=getModel()->GetLanguageString(IDS_PARA_P);
	m_szName2=getModel()->GetLanguageString(IDS_PARA_MV);
	m_szName3=getModel()->GetLanguageString(IDS_PARA_V);
	m_szName4=getModel()->GetLanguageString(IDS_PARA_FREQ);
	m_szName5=getModel()->GetLanguageString(IDS_PARA_LEAK);

	m_szNameNote1=getModel()->GetLanguageString(IDS_PARA_MITT);
	m_szNameNote2=getModel()->GetLanguageString(IDS_PARA_EXP);
	m_szNameNote3=getModel()->GetLanguageString(IDS_PARA_TE);

	if(getModel()->getCONFIG()->GetPressureUnit()==PUNIT_MBAR)
	{
		m_szUnitPressure=_T("[")+getModel()->GetLanguageString(IDS_UNIT_MBAR)+_T("]");
	}
	else
	{
		m_szUnitPressure=_T("[")+getModel()->GetLanguageString(IDS_UNIT_CMH2O)+_T("]");
	}
	if(getModel()->GetLanguageID()==LAN_CHINESE)
	{
		m_szUnitLiter=_T("[")+getModel()->GetLanguageString(IDS_UNIT_LMIN)+_T("]");
	}
	else
	{
		m_szUnitLiter=_T("[")+getModel()->GetLanguageString(IDS_UNIT_LITER)+_T("]");
	}
	m_szUnitMilliliter=_T("[")+getModel()->GetLanguageString(IDS_UNIT_MILLILITER)+_T("]");
	m_szUnitBpm=_T("[")+getModel()->GetLanguageString(IDS_UNIT_BPM)+_T("]");
	m_szUnitPercent=_T("[")+getModel()->GetLanguageString(IDS_UNIT_PERCENT)+_T("]");
}

CWndNumCPAP_Mech::~CWndNumCPAP_Mech()
{
}


BEGIN_MESSAGE_MAP(CWndNumCPAP_Mech, CWnd)
	//{{AFX_MSG_MAP(CWndNumCPAP_Mech)
	ON_WM_DESTROY()
	ON_WM_TIMER()
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


// CWndNumCPAP_Mech message handlers
//************************************
// Method:    OnDestroy
// FullName:  CWndNumCPAP_Mech::OnDestroy
// Access:    protected 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CWndNumCPAP_Mech::OnDestroy() 
{
	CWndNumeric::OnDestroy();
}



// **************************************************************************
// 
// **************************************************************************
bool CWndNumCPAP_Mech::DrawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter)
{
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

	if(false==m_bGraphCursor)
	{
		//###########################################################################
		WORD iMV=getModel()->getDATAHANDLER()->getAVGMessureDataMV();
		rc.top = 201;
		rc.bottom = 250;
		rc.left = 15; 
		rc.right = 175;
		if(iMV==0)
			wsprintf(psz,_T("%d"), 0);
		else
		{
			if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
				wsprintf(psz,_T("%0.2f"),CTlsFloat::Round(((double)iMV)/1000, 2));
			else
				wsprintf(psz,_T("%0.1f"),CTlsFloat::Round(((double)iMV)/1000, 1));
		}
		DrawText(hdcMem,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);

		//###########################################################################
		WORD iTVE=getModel()->getDATAHANDLER()->getMessureDataTVE();
		rc.top = 263;
		rc.bottom = 312;
		rc.left = 15; 
		rc.right = 175;
		if(iTVE==0)
			wsprintf(psz,_T("%d"), 0);
		else
			wsprintf(psz,_T("%0.1f"), CTlsFloat::Round(((double)iTVE)/10, 1));
		DrawText(hdcMem,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);

		//###########################################################################
		BYTE iBPM=getModel()->getDATAHANDLER()->getAVGMessureDataBPM();
		rc.top = 325;
		rc.bottom = 374;
		rc.left = 15; 
		rc.right = 175;
		wsprintf(psz,_T("%d"),iBPM);
		DrawText(hdcMem,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);
	}
	
	//###########################################################################
	BYTE iLeak=getModel()->getDATAHANDLER()->getAVGMessureDataLeak();
	rc.top = 449;
	rc.bottom = 498;
	rc.left = 15; 
	rc.right = 175;
	wsprintf(psz,_T("%d"),iLeak);
	DrawText(hdcMem,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);

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
bool CWndNumCPAP_Mech::DrawFrames(CDC* pDC)
{
	HDC hdc = *pDC;
	HBRUSH hPrevBrush = (HBRUSH)SelectObject(m_hDC, (HBRUSH)GetStockObject(WHITE_BRUSH));	
	HPEN hPrevPen = (HPEN)SelectObject(m_hDC, (HPEN)GetStockObject(WHITE_PEN));

	if(m_pcNumPara1er)
		m_pcNumPara1er->Draw(hdc,0,67);

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
		if(m_pcNumPara1er)
		{
			m_pcNumPara1er->Draw(hdc,0,191);
			m_pcNumPara1er->Draw(hdc,0,253);
			m_pcNumPara1er->Draw(hdc,0,315);
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
	{
		m_pcNumPara1er->Draw(hdc,0,439);
	}

	SelectObject(hdc, hPrevBrush);	
	SelectObject(hdc, hPrevPen);

	return true;
}

// **************************************************************************
// 
// **************************************************************************
bool CWndNumCPAP_Mech::DrawStaticText(CDC* pDC)
{
	HDC hdc = *pDC;
	HFONT hPrevFont=(HFONT)SelectObject(hdc,g_hf11AcuBold);
	int nBkMode=SetBkMode(hdc,TRANSPARENT);
	int nPrevTxtColor=SetTextColor(hdc,RGB(0,0,0));
	RECT rc;

	rc.top = 10;
	rc.bottom = m_lY;
	rc.left = 0; 
	rc.right = 190;

	SelectObject(hdc,g_hf13AcuBold);

	CSize sz1,sz2,sz3,sz4,sz5,sz6,sz7,sz8,sz9;

	rc.left = 15;

	rc.top = 71;
	pDC->DrawText(m_szName1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	sz2 = pDC->GetTextExtent(m_szName1);

	if(false==m_bGraphCursor)
	{
		rc.top = 195;
		pDC->DrawText(m_szName2,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		sz4 = pDC->GetTextExtent(m_szName2);

		rc.top = 257;
		pDC->DrawText(m_szName3,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		sz5 = pDC->GetTextExtent(m_szName3);

		rc.top = 319;
		pDC->DrawText(m_szName4,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		sz6 = pDC->GetTextExtent(m_szName4);
	}
	
	rc.top = 443;
	pDC->DrawText(m_szName5,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	sz8 = pDC->GetTextExtent(m_szName5);
	
	SelectObject(hdc,g_hf7AcuBold);

	CSize sz2Note,sz4Note,sz5Note,sz7Note;
	
	rc.top = 75;
	rc.left = 17+sz2.cx;
	pDC->DrawText(m_szNameNote1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	sz2Note = pDC->GetTextExtent(m_szNameNote1);

	if(false==m_bGraphCursor)
	{
		rc.top = 199;
		rc.left = 17+sz4.cx;
		pDC->DrawText(m_szNameNote2,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		sz4Note = pDC->GetTextExtent(m_szNameNote2);

		rc.top = 261;
		rc.left = 17+sz5.cx;
		pDC->DrawText(m_szNameNote3,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		sz5Note = pDC->GetTextExtent(m_szNameNote3);
	}

	SelectObject(hdc,g_hf8AcuNorm);

	rc.bottom = 488;
	rc.top = 72;
	rc.left = 25+sz2.cx+sz2Note.cx;
	pDC->DrawText(m_szUnitPressure,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

	if(false==m_bGraphCursor)
	{
		rc.top = 196;
		rc.left = 25+sz4.cx+sz4Note.cx;
		pDC->DrawText(m_szUnitLiter,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

		rc.top = 258;
		rc.left = 25+sz5.cx+sz5Note.cx;
		pDC->DrawText(m_szUnitMilliliter,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

		rc.top = 320;
		rc.left = 25+sz6.cx;
		pDC->DrawText(m_szUnitBpm,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	}
	
	rc.top = 444;
	rc.left = 25+sz8.cx;
	pDC->DrawText(m_szUnitPercent,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	
	SetTextColor(hdc,nPrevTxtColor);
	SetBkMode(hdc,nBkMode);
	SelectObject(hdc,hPrevFont);
	return true;
}


// **************************************************************************
// 
// **************************************************************************
bool CWndNumCPAP_Mech::DrawLimits(CDC* pDC)
{
	HDC hdc = *pDC;

	int nBkMode=SetBkMode(hdc,TRANSPARENT);
	HFONT hPrevFont=(HFONT)SelectObject(hdc,g_hf11AcuBold);
	int nPrevTxtColor=SetTextColor(hdc,RGB(200,0,0));

	RECT rc;	
	TCHAR psz[MAX_PATH];

	rc.left = 0;
	rc.right = 187;

	if(false==m_bGraphCursor)
	{
		//######################MV Max/Min########################
		if(m_pcLimitHigh_Up)
			m_pcLimitHigh_Up->Draw(hdc,120,197);//right-60, top+2
		if(m_pcLimitLow_Up)
			m_pcLimitLow_Up->Draw(hdc,120,222);//right-60, bottom-16

		rc.top = 195;
		rc.bottom = 238;
		if(getModel()->getALARMHANDLER()->getAlimitState_MVmaxLimit() == AL_CALC)
		{
			SelectObject(hdc,g_hf9AcuBold);
			pDC->DrawText(getModel()->GetLanguageString(IDS_TXT_AUTO),&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);
		}
		else if(getModel()->getALARMHANDLER()->getAlimitState_MVmaxLimit() == AL_OFF)
		{
			SelectObject(hdc,g_hf9AcuBold);
			pDC->DrawText(getModel()->GetLanguageString(IDS_TXT_OFF),&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);
		}
		else if(getModel()->getALARMHANDLER()->getAlimitState_MVmaxLimit() != AL_OFF)
		{
			SelectObject(hdc,g_hf11AcuBoldNum);
			if(getModel()->getALARMHANDLER()->getAlimitMVmax()==0)
				wsprintf(psz,_T("%d"), 0);
			else
			{
				if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
				{
					if(getModel()->getALARMHANDLER()->getAlimitMVmax()>=1000)
					{
						wsprintf(psz,_T("%0.1f"), CTlsFloat::Round(((double)getModel()->getALARMHANDLER()->getAlimitMVmax())/1000, 1));
					}
					else
					{
						wsprintf(psz,_T("%0.2f"), CTlsFloat::Round(((double)getModel()->getALARMHANDLER()->getAlimitMVmax())/1000, 2));
					}
				}
				else
					wsprintf(psz,_T("%0.1f"),CTlsFloat::Round(((double)getModel()->getALARMHANDLER()->getAlimitMVmax())/1000, 1));
			}

			pDC->DrawText(psz,&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);
		}

		if(getModel()->getALARMHANDLER()->getAlimitState_MVminLimit() == AL_CALC)
		{
			SelectObject(hdc,g_hf9AcuBold);
			pDC->DrawText(getModel()->GetLanguageString(IDS_TXT_AUTO),&rc,DT_BOTTOM|DT_SINGLELINE|DT_RIGHT);
		}
		else if(getModel()->getALARMHANDLER()->getAlimitState_MVminLimit() == AL_OFF)
		{
			SelectObject(hdc,g_hf9AcuBold);
			pDC->DrawText(getModel()->GetLanguageString(IDS_TXT_OFF),&rc,DT_BOTTOM|DT_SINGLELINE|DT_RIGHT);

			//rku MVLOW
			if(m_pcWarningYellow)
				m_pcWarningYellow->Draw(hdc,143,rc.bottom-15);
		}
		else if(getModel()->getALARMHANDLER()->getAlimitState_MVminLimit() != AL_OFF)
		{
			SelectObject(hdc,g_hf11AcuBoldNum);
			if(getModel()->getALARMHANDLER()->getAlimitMVmin()==0)
				wsprintf(psz,_T("%d"), 0);
			else
			{
				if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
				{
					if(getModel()->getALARMHANDLER()->getAlimitMVmin()>=1000)
					{
						wsprintf(psz,_T("%0.1f"), CTlsFloat::Round(((double)getModel()->getALARMHANDLER()->getAlimitMVmin())/1000, 1));
					}
					else
					{
						wsprintf(psz,_T("%0.2f"), CTlsFloat::Round(((double)getModel()->getALARMHANDLER()->getAlimitMVmin())/1000, 2));
					}
				}
				else
					wsprintf(psz,_T("%0.1f"),CTlsFloat::Round(((double)getModel()->getALARMHANDLER()->getAlimitMVmin())/1000, 1));
			}

			pDC->DrawText(psz,&rc,DT_BOTTOM|DT_SINGLELINE|DT_RIGHT);
		}

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
	
	//################Leak max##################
	if(m_pcLimitHigh_Up)
		m_pcLimitHigh_Up->Draw(hdc,120,445);//right-60, top+2

	rc.top = 443;
	rc.bottom = 486;
	if(getModel()->getALARMHANDLER()->getAlimitState_LeakmaxLimit() == AL_CALC)
	{
		SelectObject(hdc,g_hf9AcuBold);
		pDC->DrawText(getModel()->GetLanguageString(IDS_TXT_AUTO),&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);
	}
	else if(getModel()->getALARMHANDLER()->getAlimitState_LeakmaxLimit() == AL_OFF)
	{
		SelectObject(hdc,g_hf9AcuBold);
		pDC->DrawText(getModel()->GetLanguageString(IDS_TXT_OFF),&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);
	}
	else if(getModel()->getALARMHANDLER()->getAlimitState_LeakmaxLimit() != AL_OFF)
	{
		SelectObject(hdc,g_hf11AcuBoldNum);
		wsprintf(psz,_T("%d"),getModel()->getALARMHANDLER()->getAlimitLeakmax());
		pDC->DrawText(psz,&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);
	}

	SetTextColor(hdc,nPrevTxtColor);
	SetBkMode(hdc,nBkMode);
	SelectObject(hdc,hPrevFont);
	return true;
}

