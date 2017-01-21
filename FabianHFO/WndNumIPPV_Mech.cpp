// WndNumIPPV_Mech.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "WndNumIPPV_Mech.h"
// CWndNumIPPV_Mech

IMPLEMENT_DYNAMIC(CWndNumIPPV_Mech, CWnd)

CWndNumIPPV_Mech::CWndNumIPPV_Mech():
CWndNumeric()
{
	m_szName1=getModel()->GetLanguageString(IDS_PARA_PIP);
	m_szName2=getModel()->GetLanguageString(IDS_PARA_P);
	m_szName3=getModel()->GetLanguageString(IDS_PARA_PEEP);
	m_szName4=getModel()->GetLanguageString(IDS_PARA_MV);
	m_szName5=getModel()->GetLanguageString(IDS_PARA_V);
	m_szName6=getModel()->GetLanguageString(IDS_PARA_LEAK);
	m_szName7=getModel()->GetLanguageString(IDS_PARA_C);
	m_szName8=getModel()->GetLanguageString(IDS_PARA_R);

	//m_szNameNote1=getModel()->GetLanguageString(IDS_PARA_MAX);
	m_szNameNote2=getModel()->GetLanguageString(IDS_PARA_MITT);
	m_szNameNote3=getModel()->GetLanguageString(IDS_PARA_EXP);
	m_szNameNote4=getModel()->GetLanguageString(IDS_PARA_TE);

	if(getModel()->getCONFIG()->GetPressureUnit()==PUNIT_MBAR)
	{
		m_szUnitMLMBAR=_T("[")+getModel()->GetLanguageString(IDS_UNIT_MLMBAR)+_T("]");
	}
	else
	{
		m_szUnitMLMBAR=_T("[")+getModel()->GetLanguageString(IDS_UNIT_MLCMH2O)+_T("]");
	}

	if(getModel()->getCONFIG()->GetPressureUnit()==PUNIT_MBAR)
	{
		m_szUnitMBARLPS=_T("[")+getModel()->GetLanguageString(IDS_UNIT_MBARLPS)+_T("]");
	}
	else
	{
		m_szUnitMBARLPS=_T("[")+getModel()->GetLanguageString(IDS_UNIT_CMH2OLPS)+_T("]");
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
	m_szUnitPercent=_T("[")+getModel()->GetLanguageString(IDS_UNIT_PERCENT)+_T("]");
}

CWndNumIPPV_Mech::~CWndNumIPPV_Mech()
{

}


BEGIN_MESSAGE_MAP(CWndNumIPPV_Mech, CWnd)
	//{{AFX_MSG_MAP(CWndNumIPPV_Mech)
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



// CWndNumIPPV_Mech message handlers
//************************************
// Method:    OnDestroy
// FullName:  CWndNumIPPV_Mech::OnDestroy
// Access:    protected 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CWndNumIPPV_Mech::OnDestroy() 
{
	CWndNumeric::OnDestroy();
}

// **************************************************************************
// 
// **************************************************************************
bool CWndNumIPPV_Mech::DrawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter)
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

	HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf34BoldNum);
	int nBkMode=SetBkMode(hdcMem,TRANSPARENT);
	int nPrevTxtColor=SetTextColor(hdcMem,RGB(0,0,255));

	TCHAR psz[MAX_PATH];

	//###########################################################################
	SHORT iPmax=getModel()->getDATAHANDLER()->getBTBMessureDataPINSP();
	rc.top = 5;
	rc.bottom = 59;
	rc.left = 10; 
	rc.right = 105;
	if(iPmax==0)
		wsprintf(psz,_T("%d"), 0);
	else
		wsprintf(psz,_T("%0.0f"), CTlsFloat::Round(((double)iPmax)/10, 0));
	DrawText(hdcMem,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);

	//###########################################################################
	WORD iC=getModel()->getDATAHANDLER()->getAVGMessureDataCompliance();
	rc.top = 5;
	rc.bottom = 59;
	rc.left = 105; 
	rc.right = 210;
	if(iC==0)
		wsprintf(psz,_T("%d"), 0);
	else
		wsprintf(psz,_T("%0.1f"), CTlsFloat::Round(((double)iC)/10, 1));
	DrawText(hdcMem,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);

	//###########################################################################
	SHORT iPmitt=getModel()->getDATAHANDLER()->getAVGMessureDataPmitt();
	rc.top = 77;
	rc.bottom = 121;
	rc.left = 10; 
	rc.right = 105;
	if(iPmitt==0)
		wsprintf(psz,_T("%d"), 0);
	else
		wsprintf(psz,_T("%0.1f"),CTlsFloat::Round(((double)iPmitt)/10, 1));
	DrawText(hdcMem,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);

	//###########################################################################
	WORD iC20C=getModel()->getDATAHANDLER()->getAVGMessureDataC20C();
	rc.top = 77;
	rc.bottom = 121;
	rc.left = 105; 
	rc.right = 210;
	if(iC20C==0)
		wsprintf(psz,_T("%d"), 0);
	else
		wsprintf(psz,_T("%0.1f"), CTlsFloat::Round(((double)iC20C)/10, 1));
	DrawText(hdcMem,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);

	//###########################################################################
	SHORT iPEEP=getModel()->getDATAHANDLER()->getAVGMessureDataPEEP();
	rc.top = 139;
	rc.bottom = 183;
	rc.left = 10; 
	rc.right = 105;
	if(iPEEP==0)
		wsprintf(psz,_T("%d"), 0);
	else
	{
		wsprintf(psz,_T("%0.1f"), ((double)iPEEP)/10);
	}
	DrawText(hdcMem,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);

	//###########################################################################
	WORD iR=getModel()->getDATAHANDLER()->getAVGMessureDataResistance();
	rc.top = 139;
	rc.bottom = 183;
	rc.left = 105; 
	rc.right = 210;
	if(iR==0)
		wsprintf(psz,_T("%d"), 0);
	else
		wsprintf(psz,_T("%0.0f"), CTlsFloat::Round(((double)iR)/10, 0));
	DrawText(hdcMem,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);

	if(false==m_bGraphCursor)
	{
		SelectObject(hdcMem,g_hf70BoldNum);

		//###########################################################################
		WORD iMV=getModel()->getDATAHANDLER()->getAVGMessureDataMV();
		rc.top = 201;
		rc.bottom = 305;
		rc.left = 15; 
		rc.right = 175;
		if(iMV==0)
			wsprintf(psz,_T("%d"), 0);
		else
		{
			if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
			{
				if(iMV>=10000)
				{
					wsprintf(psz,_T("%0.1f"), ((double)iMV)/1000);
				}
				else
				{
					wsprintf(psz,_T("%0.2f"), ((double)iMV)/1000);
				}
			}
			else
			{
				wsprintf(psz,_T("%0.1f"),((double)iMV)/1000);
			}
		}
		DrawText(hdcMem,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);

		//###########################################################################
		WORD iTVE=getModel()->getDATAHANDLER()->getMessureDataTVE();
		rc.top = 325;
		rc.bottom = 429;
		rc.left = 15; 
		rc.right = 175;
		if(iTVE==0)
			wsprintf(psz,_T("%d"), 0);
		else
			wsprintf(psz,_T("%0.1f"), CTlsFloat::Round(((double)iTVE)/10, 1));
		DrawText(hdcMem,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);
		
	}

	SelectObject(hdcMem,g_hf33AcuBoldNum);

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
bool CWndNumIPPV_Mech::DrawFrames(CDC* pDC)
{
	HDC hdc = *pDC;
	HBRUSH hPrevBrush = (HBRUSH)SelectObject(hdc, (HBRUSH)GetStockObject(WHITE_BRUSH));	
	HPEN hPrevPen = (HPEN)SelectObject(hdc, (HPEN)GetStockObject(WHITE_PEN));

	if(m_pcNumPara3er)
		m_pcNumPara3er->Draw(hdc,0,5);

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
		if(m_pcNumPara2er)
		{
			m_pcNumPara2er->Draw(hdc,0,191);
			m_pcNumPara2er->Draw(hdc,0,315);
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
bool CWndNumIPPV_Mech::DrawStaticText(CDC* pDC)
{
	HDC hdc = *pDC;
	HFONT hPrevFont=(HFONT)SelectObject(hdc,g_hf11AcuBold);
	int nBkMode=SetBkMode(hdc,TRANSPARENT);
	int nPrevTxtColor=SetTextColor(hdc,RGB(0,0,0));
	RECT rc;

	SelectObject(hdc,g_hf11AcuBold);

	rc.top = 9;
	rc.bottom = m_lY;
	rc.left = 10; 
	rc.right = 190;
	pDC->DrawText(m_szName1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

	rc.top = 71;
	pDC->DrawText(m_szName2,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

	rc.top = 133;
	pDC->DrawText(m_szName3,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

	rc.top = 9;
	rc.left = 105;
	pDC->DrawText(m_szName7,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

	rc.top = 71;
	pDC->DrawText(m_szName7,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	
	rc.top = 133;
	pDC->DrawText(m_szName8,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

	rc.left = 135;
	rc.top = 71;
	pDC->DrawText(_T("/")+m_szName7,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	
	rc.left = 15;
	CSize sz4,sz6,sz8;

	if(false==m_bGraphCursor)
	{
		SelectObject(hdc,g_hf17AcuBold);

		rc.top = 204;
		pDC->DrawText(m_szName4,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		sz4 = pDC->GetTextExtent(m_szName4);

		rc.top = 328;
		pDC->DrawText(m_szName5,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		sz6 = pDC->GetTextExtent(m_szName5);
	}
	

	SelectObject(hdc,g_hf13AcuBold);

	rc.top = 443;
	pDC->DrawText(m_szName6,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	sz8 = pDC->GetTextExtent(m_szName6);
	rc.top = 505;

	SelectObject(hdc,g_hf7AcuBold);

	/*rc.top = 14;
	rc.left = 23;
	pDC->DrawText(m_szNameNote1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);*/

	rc.top = 76;
	rc.left = 23;
	pDC->DrawText(m_szNameNote2,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

	rc.top = 76;
	rc.left = 119;
	pDC->DrawText(_T("20"),&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

	CSize sz4Note,sz6Note;

	if(false==m_bGraphCursor)
	{
		SelectObject(hdc,g_hf8AcuBold);

		rc.top = 212;
		rc.left = 17+sz4.cx;
		pDC->DrawText(m_szNameNote3,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		sz4Note = pDC->GetTextExtent(m_szNameNote3);

		rc.top = 336;
		rc.left = 17+sz6.cx;
		pDC->DrawText(m_szNameNote4,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		sz6Note = pDC->GetTextExtent(m_szNameNote4);
	}
	

	SelectObject(hdc,g_hf7AcuNorm);

	rc.top = 9;
	rc.bottom = 54;
	rc.left = 10;
	int iOldRight = rc.right;
	rc.right = 188;
	pDC->DrawText(m_szUnitMLMBAR,&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);
	
	rc.top = 133;
	rc.bottom = 178;
	pDC->DrawText(m_szUnitMBARLPS,&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);

	rc.top = 0;
	rc.right = iOldRight;

	if(false==m_bGraphCursor)
	{
		SelectObject(hdc,g_hf15AcuMed);

		rc.top = 204;
		rc.bottom = 240;
		rc.left = 25+sz4.cx+sz4Note.cx;
		pDC->DrawText(m_szUnitLiter,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

		rc.bottom = 364;
		rc.top = 328;
		rc.left = 25+sz6.cx+sz6Note.cx;
		pDC->DrawText(m_szUnitMilliliter,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	}
	
	SelectObject(hdc,g_hf8AcuNorm);

	rc.bottom = m_lY;
	rc.right = 190;

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
bool CWndNumIPPV_Mech::DrawLimits(CDC* pDC)
{
	HDC hdc = *pDC;
	int nBkMode=SetBkMode(hdc,TRANSPARENT);
	HFONT hPrevFont=(HFONT)SelectObject(hdc,g_hf11AcuBold);
	int nPrevTxtColor=SetTextColor(hdc,RGB(200,0,0));
	RECT rc;	
	TCHAR psz[MAX_PATH];

	if(false==m_bGraphCursor)
	{
		//################MV Min Max##################
		rc.top = 244;
		rc.bottom = 296;
		rc.left = 0;
		rc.right = 187;
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
		else 
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
	}
	
	//#####################Leak Max########################
	if(m_pcLimitHigh_Up)
		m_pcLimitHigh_Up->Draw(hdc,120,449);//right-60, top+2

	rc.top = 447;
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
