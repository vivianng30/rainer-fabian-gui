// WndNumIPPV_BtB.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "WndNumIPPV_BtB.h"
// CWndNumIPPV_BtB

IMPLEMENT_DYNAMIC(CWndNumIPPV_BtB, CWnd)

CWndNumIPPV_BtB::CWndNumIPPV_BtB():
CWndNumeric()
{
	m_szName1=getModel()->GetLanguageString(IDS_PARA_PIP);
	m_szName2=getModel()->GetLanguageString(IDS_PARA_P);
	m_szName3=getModel()->GetLanguageString(IDS_PARA_PEEP);
	m_szName4=getModel()->GetLanguageString(IDS_PARA_MV);
	m_szName5=getModel()->GetLanguageString(IDS_PARA_V);
	m_szName6=getModel()->GetLanguageString(IDS_PARA_LEAK);

	//m_szNameNote1=getModel()->GetLanguageString(IDS_PARA_MAX);
	m_szNameNote2=getModel()->GetLanguageString(IDS_PARA_MITT);
	m_szNameNote3=getModel()->GetLanguageString(IDS_PARA_EXP);
	m_szNameNote4=getModel()->GetLanguageString(IDS_PARA_TE);

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
	m_szUnitPercent=_T("[")+getModel()->GetLanguageString(IDS_UNIT_PERCENT)+_T("]");

}

CWndNumIPPV_BtB::~CWndNumIPPV_BtB()
{

}


BEGIN_MESSAGE_MAP(CWndNumIPPV_BtB, CWnd)
	//{{AFX_MSG_MAP(CWndNumIPPV_BtB)
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


// CWndNumIPPV_BtB message handlers
//************************************
// Method:    OnDestroy
// FullName:  CWndNumIPPV_BtB::OnDestroy
// Access:    protected 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CWndNumIPPV_BtB::OnDestroy() 
{
	CWndNumeric::OnDestroy();
}


// **************************************************************************
// 
// **************************************************************************
bool CWndNumIPPV_BtB::DrawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter)
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
	if(iPmax==0)
		wsprintf(psz,_T("%d"), 0);
	else
	{
		wsprintf(psz,_T("%0.0f"), ((double)iPmax)/10);
	}
	DrawText(hdcMem,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);

	//###########################################################################
	SHORT iPmitt=getModel()->getDATAHANDLER()->getAVGMessureDataPmitt();
	rc.top = 77;
	rc.bottom = 126;
	rc.left = 15; 
	rc.right = 175;
	if(iPmitt!=0)
	{
		wsprintf(psz,_T("%0.1f"),CTlsFloat::Round(((double)iPmitt)/10, 1));
	}
	else
		wsprintf(psz,_T("%d"),0);
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

	SelectObject(hdcMem,g_hf70BoldNum);

	if(false==m_bGraphCursor)
	{
		//###########################################################################
		//WORD iMV=getModel()->getDATAHANDLER()->getBTBMessureDataMV();
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
		{
			wsprintf(psz,_T("%0.1f"), ((double)iTVE)/10);
		}

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
bool CWndNumIPPV_BtB::DrawFrames(CDC* pDC)
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
		m_pcNumPara1er->Draw(hdc,0,439);

	SelectObject(hdc, hPrevBrush);	
	SelectObject(hdc, hPrevPen);

	return true;
}

// **************************************************************************
// 
// **************************************************************************
bool CWndNumIPPV_BtB::DrawStaticText(CDC* pDC)
{
	HDC hdc = *pDC;
	HFONT hPrevFont=(HFONT)SelectObject(hdc,g_hf13AcuBold);
	int nBkMode=SetBkMode(hdc,TRANSPARENT);
	int nPrevTxtColor=SetTextColor(hdc,RGB(0,0,0));
	RECT rc;

	rc.top = 9;
	rc.bottom = m_lY;
	rc.left = 15; 
	rc.right = 190;

	CSize sz1,sz2,sz3,sz4,sz6,sz8;

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

	SelectObject(hdc,g_hf7AcuBold);

	CSize sz2Note,sz4Note,sz6Note;

	/*rc.top = 13;
	rc.left = 17+sz1.cx;
	pDC->DrawText(m_szNameNote1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	sz1Note = pDC->GetTextExtent(m_szNameNote1);*/

	rc.top = 75;
	rc.left = 17+sz2.cx;
	pDC->DrawText(m_szNameNote2,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	sz2Note = pDC->GetTextExtent(m_szNameNote2);

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

	rc.top = 10;
	rc.left = 25+sz1.cx;
	pDC->DrawText(m_szUnitPressure,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

	rc.top = 72;
	rc.left = 25+sz2.cx+sz2Note.cx;
	pDC->DrawText(m_szUnitPressure,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

	rc.top = 134;
	rc.left = 25+sz3.cx;
	pDC->DrawText(m_szUnitPressure,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

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
bool CWndNumIPPV_BtB::DrawLimits(CDC* pDC)
{
	HDC hdc = *pDC;
	int nBkMode=SetBkMode(hdc,TRANSPARENT);
	HFONT hPrevFont=(HFONT)SelectObject(hdc,g_hf11AcuBold);
	int nPrevTxtColor=SetTextColor(hdc,RGB(200,0,0));
	RECT rc;	
	TCHAR psz[MAX_PATH];

	//################Pmax Max##################
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
	else if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit() != AL_OFF)
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

	//################PEEP Min##################
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

	if(false==m_bGraphCursor)
	{
		//################MV Min Max##################
		rc.top = 244;
		rc.bottom = 296;
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
