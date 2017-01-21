// WndNumSIMVPSV_Mech.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "WndNumSIMVPSV_Mech.h"


// CWndNumSIMVPSV_Mech

IMPLEMENT_DYNAMIC(CWndNumSIMVPSV_Mech, CWnd)

CWndNumSIMVPSV_Mech::CWndNumSIMVPSV_Mech():
CWndNumeric()
{
	m_szName1=getModel()->GetLanguageString(IDS_PARA_PIP);
	m_szName2=getModel()->GetLanguageString(IDS_PARA_P);
	m_szName3=getModel()->GetLanguageString(IDS_PARA_PEEP);
	m_szName4=getModel()->GetLanguageString(IDS_PARA_C);
	m_szName5=getModel()->GetLanguageString(IDS_PARA_C);
	m_szName6=getModel()->GetLanguageString(IDS_PARA_R);
	m_szName7=getModel()->GetLanguageString(IDS_PARA_MV);
	m_szName8=getModel()->GetLanguageString(IDS_PARA_PROZANTEIL);
	m_szName9=getModel()->GetLanguageString(IDS_PARA_V);
	m_szName10=getModel()->GetLanguageString(IDS_PARA_FREQ);
	m_szName11=getModel()->GetLanguageString(IDS_PARA_LEAK);

	//m_szNameNote1=getModel()->GetLanguageString(IDS_PARA_MAX);
	m_szNameNote2=getModel()->GetLanguageString(IDS_PARA_MITT);
	m_szNameNote3=getModel()->GetLanguageString(IDS_PARA_EXP);
	if(getModel()->GetLanguageID()==LAN_JAPANESE)
	{
		m_szNameNote4=_T("");
	}
	else
	{
		m_szNameNote4=getModel()->GetLanguageString(IDS_PARA_RESP);
	}
	m_szNameNote5=getModel()->GetLanguageString(IDS_PARA_TE) + _T(" ") + getModel()->GetLanguageString(IDS_PARA_RESP);
	m_szNameNote6=getModel()->GetLanguageString(IDS_PARA_TE) + _T(" ") + getModel()->GetLanguageString(IDS_PARA_PAT);

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
	m_szUnitBPM=_T("[")+getModel()->GetLanguageString(IDS_UNIT_BPM)+_T("]");
	m_szUnitPercent=_T("[")+getModel()->GetLanguageString(IDS_UNIT_PERCENT)+_T("]");

}

CWndNumSIMVPSV_Mech::~CWndNumSIMVPSV_Mech()
{
}


BEGIN_MESSAGE_MAP(CWndNumSIMVPSV_Mech, CWnd)
	//{{AFX_MSG_MAP(CWndNumSIMVPSV_Mech)
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



// CWndNumSIMVPSV_Mech message handlers
//************************************
// Method:    OnDestroy
// FullName:  CWndNumSIMVPSV_Mech::OnDestroy
// Access:    protected 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CWndNumSIMVPSV_Mech::OnDestroy() 
{
	CWndNumeric::OnDestroy();
}




// **************************************************************************
// 
// **************************************************************************
bool CWndNumSIMVPSV_Mech::DrawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter)
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

	{
		if(iC==0)
			wsprintf(psz,_T("%d"), 0);
		else
			wsprintf(psz,_T("%0.1f"), CTlsFloat::Round(((double)iC)/10, 1));
	}
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

	SelectObject(hdcMem,g_hf33AcuBoldNum);

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
		BYTE iAnteil=getModel()->getDATAHANDLER()->getAVGMessureDataPercent();
		rc.top = 263;
		rc.bottom = 312;
		rc.left = 15; 
		rc.right = 175;
		wsprintf(psz,_T("%d"),iAnteil);
		DrawText(hdcMem,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);

		//###########################################################################
		WORD iTVEresp=getModel()->getDATAHANDLER()->getMessureDataTVEresp();
		rc.top = 325;
		rc.bottom = 374;
		rc.left = 15; 
		rc.right = 175;
		if(iTVEresp==0)
			wsprintf(psz,_T("%d"), 0);
		else
		{
			if(iTVEresp>=1000)
				wsprintf(psz,_T("%0.0f"),CTlsFloat::Round(((double)iTVEresp)/10, 1));
			else
				wsprintf(psz,_T("%0.1f"), CTlsFloat::Round(((double)iTVEresp)/10, 1));
		}
		DrawText(hdcMem,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);

		//###########################################################################
		WORD iTVEpat=getModel()->getDATAHANDLER()->getMessureDataTVEpat();
		rc.top = 325;
		rc.bottom = 374;
		rc.left = 105; 
		rc.right = 180;
		if(iTVEpat==0)
			wsprintf(psz,_T("%d"), 0);
		else
		{
			if(iTVEpat>=1000)
				wsprintf(psz,_T("%0.0f"),CTlsFloat::Round(((double)iTVEpat)/10, 1));
			else
				wsprintf(psz,_T("%0.1f"), CTlsFloat::Round(((double)iTVEpat)/10, 1));
		}
		DrawText(hdcMem,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);

		//###########################################################################
		BYTE iBPM=getModel()->getDATAHANDLER()->getAVGMessureDataBPM();
		rc.top = 387;
		rc.bottom = 436;
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
bool CWndNumSIMVPSV_Mech::DrawFrames(CDC* pDC)
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

		if(m_pcNumPara1er)
		{
			m_pcNumPara1er->Draw(hdc,0,191);
			m_pcNumPara1er->Draw(hdc,0,253);
			m_pcNumPara1er->Draw(hdc,0,315);
		}

		//CPen* pTempPen = NULL;
		//CPen origPen;
		//CPen penLine(PS_SOLID,3,RGB(200,200,200));
		//pTempPen = (CPen*)pDC->SelectObject(&penLine);
		//// Save original pen.
		//origPen.FromHandle((HPEN)pTempPen);

		CPen penLine(PS_SOLID,3,RGB(200,200,200));
		HPEN hPrevPen = (HPEN)SelectObject(hdc, (HPEN)penLine);

		MoveToEx(hdc, 98, 315, NULL);
		LineTo(hdc, 98, 372);

		if(m_pcNumPara1er)
			m_pcNumPara1er->Draw(hdc,0,377);

		// Reselect original brush into device context.
		/*pDC->SelectObject(&origPen);
		penLine.DeleteObject();*/
		SelectObject(hdc, hPrevPen);
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
bool CWndNumSIMVPSV_Mech::DrawStaticText(CDC* pDC)
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
	pDC->DrawText(m_szName4,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

	rc.top = 71;
	pDC->DrawText(m_szName5,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

	rc.top = 133;
	pDC->DrawText(m_szName6,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

	rc.left = 135;
	rc.top = 71;
	pDC->DrawText(_T("/")+m_szName5,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

	rc.left = 15;
	CSize sz4,sz5,sz6,sz7,sz8,sz9;

	if(false==m_bGraphCursor)
	{
		rc.top = 195;
		pDC->DrawText(m_szName7,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		sz4 = pDC->GetTextExtent(m_szName7);

		rc.top = 257;
		pDC->DrawText(m_szName8,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		sz5 = pDC->GetTextExtent(m_szName8);

		rc.top = 319;
		pDC->DrawText(m_szName9,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		sz6 = pDC->GetTextExtent(m_szName9);

		rc.left = 105;
		pDC->DrawText(m_szName9,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		sz7 = pDC->GetTextExtent(m_szName9);

		rc.left = 15;
		rc.top = 381;
		pDC->DrawText(m_szName10,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		sz8 = pDC->GetTextExtent(m_szName10);
	}

	SelectObject(hdc,g_hf13AcuBold);

	rc.top = 443;
	pDC->DrawText(m_szName11,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	sz9 = pDC->GetTextExtent(m_szName11);

	
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

	CSize sz4Note,sz5Note,sz6Note,sz7Note;

	

	//MVexp
	if(false==m_bGraphCursor)
	{
		rc.top = 199;
		rc.left = 17+sz4.cx;
		pDC->DrawText(m_szNameNote3,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		sz4Note = pDC->GetTextExtent(m_szNameNote3);

		rc.top = 261;
		rc.left = 17+sz5.cx;
		pDC->DrawText(m_szNameNote4,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		sz5Note = pDC->GetTextExtent(m_szNameNote4);

		rc.top = 323;
		rc.left = 17+sz6.cx;
		pDC->DrawText(m_szNameNote5,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		sz6Note = pDC->GetTextExtent(m_szNameNote5);

		rc.top = 323;
		rc.left =105+sz7.cx;
		pDC->DrawText(m_szNameNote6,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		sz7Note = pDC->GetTextExtent(m_szNameNote6);
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
	rc.bottom = m_lY;

	if(false==m_bGraphCursor)
	{
		rc.top = 196;
		rc.left = 25+sz4.cx+sz4Note.cx;
		pDC->DrawText(m_szUnitLiter,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

		/*rc.top = 258;
		rc.left = 25+sz5.cx+sz5Note.cx;
		pDC->DrawText(m_wnl.szUnit5,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);*/

		rc.top = 320;
		rc.left = 25+sz7.cx+sz7Note.cx;
		rc.right-=5;
		pDC->DrawText(m_szUnitMilliliter,&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);

		rc.right+=5;
		rc.top = 382;
		rc.left = 25+sz8.cx;
		pDC->DrawText(m_szUnitBPM,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	}

	rc.top = 444;
	rc.left = 25+sz9.cx;
	pDC->DrawText(m_szUnitPercent,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

	SetTextColor(hdc,nPrevTxtColor);
	SetBkMode(hdc,nBkMode);
	SelectObject(hdc,hPrevFont);
	return true;
}


// **************************************************************************
// 
// **************************************************************************
bool CWndNumSIMVPSV_Mech::DrawLimits(CDC* pDC)
{
	HDC hdc = *pDC;
	int nBkMode=SetBkMode(hdc,TRANSPARENT);
	HFONT hPrevFont=(HFONT)SelectObject(hdc,g_hf11AcuBold);
	int nPrevTxtColor=SetTextColor(hdc,RGB(200,0,0));
	RECT rc;	
	TCHAR psz[MAX_PATH];

	rc.top = 9;
	rc.bottom = 52;
	rc.left = 0;
	rc.right = 187;

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
		//#####################BPM Max########################
		if(m_pcLimitHigh_Up)
			m_pcLimitHigh_Up->Draw(hdc,120,383);//right-60, top+2
		rc.top = 381;
		rc.bottom = 431;
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
			wsprintf(psz,_T("%d"),getModel()->getALARMHANDLER()->getAlimitBPMmax());
			pDC->DrawText(psz,&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);
		}
		
	}
	

	//#####################Leak Max########################
	if(m_pcLimitHigh_Up)
		m_pcLimitHigh_Up->Draw(hdc,120,445);//right-60, top+2

	rc.top = 443;
	rc.bottom = 493;
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


