// WndNumHFO_Mech.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "WndNumHFO_Mech.h"

// CWndNumHFO_Mech

IMPLEMENT_DYNAMIC(CWndNumHFO_Mech, CWnd)

CWndNumHFO_Mech::CWndNumHFO_Mech():
CWndNumeric()
{
	m_szName1=getModel()->GetLanguageString(IDS_PARA_PIP);
	m_szName2=getModel()->GetLanguageString(IDS_PARA_P);
	m_szName3=getModel()->GetLanguageString(IDS_PARA_HFO);
	m_szName4=getModel()->GetLanguageString(IDS_PARA_MV);
	m_szName5=getModel()->GetLanguageString(IDS_PARA_V);
	m_szName6=getModel()->GetLanguageString(IDS_PARA_HFO_FREQ);

	//m_szNameNote1=getModel()->GetLanguageString(IDS_PARA_MAX);
	m_szNameNote2=getModel()->GetLanguageString(IDS_PARA_MITT);
	m_szNameNote3=getModel()->GetLanguageString(IDS_PARA_AMPL);
	m_szNameNote4=getModel()->GetLanguageString(IDS_PARA_EXP);
	m_szNameNote5=getModel()->GetLanguageString(IDS_PARA_TE) + _T(" ") + getModel()->GetLanguageString(IDS_PARA_HFO);
	m_szNameNote6=getModel()->GetLanguageString(IDS_PARA_HFFREQ_NOTE);

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
	m_szUnitHz=_T("[")+getModel()->GetLanguageString(IDS_UNIT_HZ)+_T("]");
}

CWndNumHFO_Mech::~CWndNumHFO_Mech()
{
}


BEGIN_MESSAGE_MAP(CWndNumHFO_Mech, CWnd)
	//{{AFX_MSG_MAP(CWndNumHFO_Mech)
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
	//ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




// CWndNumHFO_Mech message handlers
//************************************
// Method:    OnDestroy
// FullName:  CWndNumHFO_Mech::OnDestroy
// Access:    protected 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CWndNumHFO_Mech::OnDestroy() 
{
	CWndNumeric::OnDestroy();
}



// **************************************************************************
// 
// **************************************************************************
bool CWndNumHFO_Mech::DrawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter)
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
	SHORT iPmax=getModel()->getDATAHANDLER()->getBTBMessureDataPINSP();
	rc.top = 5;
	rc.bottom = 64;
	rc.left = 15; 
	rc.right = 175;
	if(iPmax!=0)
		wsprintf(psz,_T("%0.0f"), CTlsFloat::Round(((double)iPmax)/10, 0));
	else
		wsprintf(psz,_T("%d"),0);
	DrawText(hdcMem,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);

	//###########################################################################
	SHORT iPmitt=getModel()->getDATAHANDLER()->getAVGMessureDataPmitt();
	rc.top = 77;
	rc.bottom = 126;
	rc.left = 15; 
	rc.right = 175;
	if(iPmitt!=0)
		wsprintf(psz,_T("%0.1f"),CTlsFloat::Round(((double)iPmitt)/10, 1));
	else
		wsprintf(psz,_T("%d"),0);
	DrawText(hdcMem,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);

	//###########################################################################
	WORD iHFAmpl=getModel()->getDATAHANDLER()->getAVGMessureDataHFAmpl();
	rc.top = 139;
	rc.bottom = 188;
	rc.left = 15; 
	rc.right = 175;
	if(iHFAmpl!=0)
		wsprintf(psz,_T("%0.0f"),CTlsFloat::Round(((double)iHFAmpl)/10, 0));
	else
		wsprintf(psz,_T("%d"),0);
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
			{
				if(iMV>=1000)
					wsprintf(psz,_T("%0.1f"),CTlsFloat::Round(((double)iMV)/1000, 1));
				else
					wsprintf(psz,_T("%0.2f"),CTlsFloat::Round(((double)iMV)/1000, 2));
			}
			else
				wsprintf(psz,_T("%0.1f"),CTlsFloat::Round(((double)iMV)/1000, 1));
		}
		DrawText(hdcMem,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);

		//###########################################################################
		WORD iTVEHFO=getModel()->getDATAHANDLER()->getAVGMessureDataTVEHFO();
		rc.top = 263;
		rc.bottom = 312;
		rc.left = 15; 
		if(iTVEHFO==0)
			wsprintf(psz,_T("%d"),0);
		else
			wsprintf(psz,_T("%0.1f"), CTlsFloat::Round(((double)iTVEHFO)/10, 1));
		DrawText(hdcMem,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);

		//###########################################################################
		BYTE iHFFreq=getModel()->getDATAHANDLER()->getAVGMessureDataHFFreq();
		rc.top = 325;
		rc.bottom = 374;
		rc.left = 15; 
		rc.right = 175;
		wsprintf(psz,_T("%d"),iHFFreq );
		DrawText(hdcMem,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);
	}
	
	//###########################################################################
	WORD iDCO2=getModel()->getDATAHANDLER()->getAVGMessureDataDCO2();
	rc.top = 449;
	rc.bottom = 498;
	rc.left = 15; 
	wsprintf(psz,_T("%d"),iDCO2);
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
bool CWndNumHFO_Mech::DrawFrames(CDC* pDC)
{
	HDC hdc = *pDC;
	HBRUSH hPrevBrush = (HBRUSH)SelectObject(hdc, (HBRUSH)GetStockObject(WHITE_BRUSH));	
	HPEN hPrevPen = (HPEN)SelectObject(hdc, (HPEN)GetStockObject(WHITE_PEN));

	if(m_pcNumPara1er)
	{
		m_pcNumPara1er->Draw(hdc,0,5);
		m_pcNumPara1er->Draw(hdc,0,67);
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

		if(m_pcGraphCursor_pic)
			m_pcGraphCursor_pic->Draw(hdc,74,267);
		if(m_pbtnCursorUp)
			m_pbtnCursorUp->ShowWindow(SW_SHOW);
		if(m_pbtnCursorDown)
			m_pbtnCursorDown->ShowWindow(SW_SHOW);
		if(m_pbtnCursorLeft)
			m_pbtnCursorLeft->ShowWindow(SW_SHOW);
		if(m_pbtnCursorRight)
			m_pbtnCursorRight->ShowWindow(SW_SHOW);

		if(m_bMoveXscale)
		{
			SelectObject(hdc, (HPEN)GetStockObject(BLACK_PEN));
			MoveToEx(hdc, 5, 388, NULL);
			LineTo(hdc, 195, 388);

			if(m_pbtnYmoveLeft)
				m_pbtnYmoveLeft->ShowWindow(SW_HIDE);
			if(m_pbtnYmoveRight)
				m_pbtnYmoveRight->ShowWindow(SW_HIDE);
			if(m_pbtnXmoveUp)
				m_pbtnXmoveUp->ShowWindow(SW_SHOW);
			if(m_pbtnXmoveDown)
				m_pbtnXmoveDown->ShowWindow(SW_SHOW);
			if(m_pcMoveXscale_pic)
				m_pcMoveXscale_pic->Draw(hdc,80,400);
		}
		else if(m_bMoveYscale)
		{
			SelectObject(hdc, (HPEN)GetStockObject(BLACK_PEN));
			MoveToEx(hdc, 5, 388, NULL);
			LineTo(hdc, 195, 388);

			if(m_pbtnXmoveUp)
				m_pbtnXmoveUp->ShowWindow(SW_HIDE);
			if(m_pbtnXmoveDown)
				m_pbtnXmoveDown->ShowWindow(SW_HIDE);
			if(m_pbtnYmoveLeft)
				m_pbtnYmoveLeft->ShowWindow(SW_SHOW);
			if(m_pbtnYmoveRight)
				m_pbtnYmoveRight->ShowWindow(SW_SHOW);
			if(m_pcMoveYscale_pic)
				m_pcMoveYscale_pic->Draw(hdc,80,400);
		}
		else
		{
			if(m_pbtnXmoveUp)
				m_pbtnXmoveUp->ShowWindow(SW_HIDE);
			if(m_pbtnXmoveDown)
				m_pbtnXmoveDown->ShowWindow(SW_HIDE);
			if(m_pbtnYmoveLeft)
				m_pbtnYmoveLeft->ShowWindow(SW_HIDE);
			if(m_pbtnYmoveRight)
				m_pbtnYmoveRight->ShowWindow(SW_HIDE);
		}
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
bool CWndNumHFO_Mech::DrawStaticText(CDC* pDC)
{
	HDC hdc = *pDC;
	HFONT hPrevFont=(HFONT)SelectObject(hdc,g_hf11AcuBold);
	int nBkMode=SetBkMode(hdc,TRANSPARENT);
	int nPrevTxtColor=SetTextColor(hdc,RGB(0,0,0));
	
	RECT rc;
	rc.left = 0; 
	rc.right = 190;
	rc.top = 10;
	rc.bottom = m_lY;

	SelectObject(hdc,g_hf13AcuBold);

	CSize sz1,sz2,sz3,sz4,sz5,sz6,sz7,sz8,sz9;

	rc.top = 9;
	rc.left = 15;
	pDC->DrawText(m_szName1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	sz1 = pDC->GetTextExtent(m_szName1);

	rc.top = 71;
	pDC->DrawText(m_szName2,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	sz2 = pDC->GetTextExtent(m_szName2);

	rc.top = 133;
	pDC->DrawText(m_szName3,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	sz3 = pDC->GetTextExtent(m_szName3);

	if(false==m_bGraphCursor)
	{
		rc.top = 195;
		pDC->DrawText(m_szName4,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		sz4 = pDC->GetTextExtent(m_szName4);

		rc.top = 257;
		pDC->DrawText(m_szName5,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		sz5 = pDC->GetTextExtent(m_szName5);

		rc.top = 319;
		pDC->DrawText(m_szName6,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		sz6 = pDC->GetTextExtent(m_szName6);
	}
	
	rc.top = 443;
	pDC->DrawText(_T("DCO"),&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	sz8 = pDC->GetTextExtent(_T("DCO"));

	SelectObject(hdc,g_hf7AcuBold);

	CSize sz2Note,sz3Note,sz4Note,sz5Note,sz6Note,sz7Note,sz8Note,sz9Note;

	/*rc.top = 13;
	rc.left = 17+sz1.cx;
	pDC->DrawText(m_szNameNote1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	sz1Note = pDC->GetTextExtent(m_szNameNote1);*/

	rc.top = 75;
	rc.left = 17+sz2.cx;
	pDC->DrawText(m_szNameNote2,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	sz2Note = pDC->GetTextExtent(m_szNameNote2);

	if(m_szNameNote3!=_T("0"))// for asian language
	{
		rc.top = 137;
		rc.left = 17+sz3.cx;
		pDC->DrawText(m_szNameNote3,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		sz3Note = pDC->GetTextExtent(m_szNameNote3);
	}

	if(false==m_bGraphCursor)
	{
		rc.top = 199;
		rc.left = 17+sz4.cx;
		pDC->DrawText(m_szNameNote4,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		sz4Note = pDC->GetTextExtent(m_szNameNote4);

		rc.top = 261;
		rc.left = 17+sz5.cx;
		pDC->DrawText(m_szNameNote5,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		sz5Note = pDC->GetTextExtent(m_szNameNote5);

		if(m_szNameNote6!=_T("0"))// for asian language
		{
			rc.top = 323;
			rc.left = 17+sz6.cx;
			pDC->DrawText(m_szNameNote6,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
			sz6Note = pDC->GetTextExtent(m_szNameNote6);
		}
	}
	
	rc.top = 447;
	rc.left = 17+sz8.cx;
	pDC->DrawText(_T("2"),&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	sz8Note = pDC->GetTextExtent(_T("2"));

	SelectObject(hdc,g_hf7AcuNorm);

	rc.top = 10;
	rc.left = 25+sz1.cx;
	pDC->DrawText(m_szUnitPressure,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	
	rc.top = 72;
	rc.left = 25+sz2.cx+sz2Note.cx;
	pDC->DrawText(m_szUnitPressure,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

	rc.top = 134;
	rc.left = 25+sz3.cx+sz3Note.cx;
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
		rc.left = 25+sz6.cx+sz6Note.cx;
		pDC->DrawText(m_szUnitHz,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	}
	
	rc.top = 444;
	rc.left = 25+sz8.cx+sz8Note.cx;
	pDC->DrawText(_T("[ml  /sec]"),&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

	// [ml2/s]
	//SelectObject(hdc,g_hf8AcuBold);
	SelectObject(hdc,g_hf5AcuNorm);
	rc.top = 441;
	//rc.left = 25+sz8.cx+sz8Note.cx+27;
	rc.left = 25+sz8.cx+sz8Note.cx+14;
	pDC->DrawText(_T("2"),&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

	SetTextColor(hdc,nPrevTxtColor);
	SetBkMode(hdc,nBkMode);
	SelectObject(hdc,hPrevFont);
	return true;
}




// **************************************************************************
// 
// **************************************************************************
bool CWndNumHFO_Mech::DrawLimits(CDC* pDC)
{
	HDC hdc = *pDC;
	int nBkMode=SetBkMode(hdc,TRANSPARENT);
	HFONT hPrevFont=(HFONT)SelectObject(hdc,g_hf11AcuBold);
	int nPrevTxtColor=SetTextColor(hdc,RGB(200,0,0));
	RECT rc;	
	TCHAR psz[MAX_PATH];

	//################Pmax Max##################
	if(m_pcLimitHigh_Up)
		m_pcLimitHigh_Up->Draw(hdc,120,11);//right-60, top+2

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

		if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit() == AL_AUTO)
		{
			SelectObject(hdc,g_hf9AcuBold);
			pDC->DrawText(getModel()->GetLanguageString(IDS_TXT_AUTO),&rc,DT_BOTTOM|DT_SINGLELINE|DT_RIGHT);
		}
	}

	if(false==m_bGraphCursor)
	{
		//################MV Max + Min##################
		if(m_pcLimitHigh_Up)
			m_pcLimitHigh_Up->Draw(hdc,120,197);//right-60, top+2
		if(m_pcLimitLow_Up)
			m_pcLimitLow_Up->Draw(hdc,120,229);//right-60, bottom-16

		rc.top = 195;
		rc.bottom = 245;
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
			if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
			{
				if(getModel()->getALARMHANDLER()->getAlimitMVmax()==0)
					wsprintf(psz,_T("%d"), 0);
				else
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
			}
			else
			{
				if(getModel()->getALARMHANDLER()->getAlimitMVmax()==0)
					wsprintf(psz,_T("%d"), 0);
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
	

	//################DCO2 Max + Min##################
	if(m_pcLimitHigh_Up)
		m_pcLimitHigh_Up->Draw(hdc,120,445);//right-60, top+2
	if(m_pcLimitLow_Up)
		m_pcLimitLow_Up->Draw(hdc,120,477);//right-60, bottom-16

	rc.top = 443;
	rc.bottom = 493;
	if(getModel()->getALARMHANDLER()->getAlimitState_DCO2maxLimit() == AL_CALC)
	{
		SelectObject(hdc,g_hf9AcuBold);
		pDC->DrawText(getModel()->GetLanguageString(IDS_TXT_AUTO),&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);
	}
	else if(getModel()->getALARMHANDLER()->getAlimitState_DCO2maxLimit() == AL_OFF)
	{
		SelectObject(hdc,g_hf9AcuBold);
		pDC->DrawText(getModel()->GetLanguageString(IDS_TXT_OFF),&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);
	}
	else if(getModel()->getALARMHANDLER()->getAlimitState_DCO2maxLimit() != AL_OFF)
	{
		if(getModel()->getALARMHANDLER()->getAlimitDCO2max()>=10000)
			SelectObject(hdc,g_hf10AcuBold);
		else
			SelectObject(hdc,g_hf11AcuBoldNum);
		wsprintf(psz,_T("%d"),getModel()->getALARMHANDLER()->getAlimitDCO2max());
		pDC->DrawText(psz,&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);
	}

	if(getModel()->getALARMHANDLER()->getAlimitState_DCO2minLimit() == AL_CALC)
	{
		SelectObject(hdc,g_hf9AcuBold);
		pDC->DrawText(getModel()->GetLanguageString(IDS_TXT_AUTO),&rc,DT_BOTTOM|DT_SINGLELINE|DT_RIGHT);
	}
	else if(getModel()->getALARMHANDLER()->getAlimitState_DCO2minLimit() == AL_OFF)
	{
		SelectObject(hdc,g_hf9AcuBold);
		pDC->DrawText(getModel()->GetLanguageString(IDS_TXT_OFF),&rc,DT_BOTTOM|DT_SINGLELINE|DT_RIGHT);
	}
	else if(getModel()->getALARMHANDLER()->getAlimitState_DCO2minLimit() != AL_OFF)
	{
		if(getModel()->getALARMHANDLER()->getAlimitDCO2max()>=10000)
			SelectObject(hdc,g_hf10AcuBold);
		else
			SelectObject(hdc,g_hf11AcuBoldNum);
		wsprintf(psz,_T("%d"), getModel()->getALARMHANDLER()->getAlimitDCO2min());

		pDC->DrawText(psz,&rc,DT_BOTTOM|DT_SINGLELINE|DT_RIGHT);
	}

	SetTextColor(hdc,nPrevTxtColor);
	SetBkMode(hdc,nBkMode);
	SelectObject(hdc,hPrevFont);
	return true;
}

