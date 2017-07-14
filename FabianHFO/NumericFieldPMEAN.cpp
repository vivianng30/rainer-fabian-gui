// NumericFieldPMEAN.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "NumericFieldPMEAN.h"


// CNumericFieldPMEAN

IMPLEMENT_DYNAMIC(CNumericFieldPMEAN, CWnd)

CNumericFieldPMEAN::CNumericFieldPMEAN(eNumericSize size):
CNumericField(size)
{
	m_szName=getModel()->GetLanguageString(IDS_PARA_P);
	m_szNameNote=getModel()->GetLanguageString(IDS_PARA_MITT);
	if(getModel()->getCONFIG()->GetPressureUnit()==PUNIT_MBAR)
	{
		m_szUnit=_T("[")+getModel()->GetLanguageString(IDS_UNIT_MBAR)+_T("]");
	}
	else
	{
		m_szUnit=_T("[")+getModel()->GetLanguageString(IDS_UNIT_CMH2O)+_T("]");
	}
}

CNumericFieldPMEAN::~CNumericFieldPMEAN()
{
}


BEGIN_MESSAGE_MAP(CNumericFieldPMEAN, CWnd)
	//{{AFX_MSG_MAP(CNumericField)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



// CNumericFieldPMEAN message handlers

//************************************
// Method:    OnDestroy
// FullName:  CNumericFieldPMEAN::OnDestroy
// Access:    protected 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CNumericFieldPMEAN::OnDestroy() 
{
	CNumericField::OnDestroy();
}

// **************************************************************************
// 
// **************************************************************************
bool CNumericFieldPMEAN::drawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter)
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
		drawFrames(pDCStatic);
	}

	if(bText)
	{
		drawStaticText(pDCStatic);
	}

	eVentMode eActiveVentMode=getModel()->getALARMHANDLER()->getSafeActiveVentMode();
	if(bLimits && eActiveVentMode==VM_HFO)
	{
		drawLimits(pDCStatic);
	}

	BitBlt(hdcMem, 0, 0, m_lX, m_lY,m_hdcStatic , 0, 0, SRCCOPY);

	HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf34BoldNum);
	int nBkMode=SetBkMode(hdcMem,TRANSPARENT);
	int nPrevTxtColor=SetTextColor(hdcMem,RGB(0,0,255));

	TCHAR psz[MAX_PATH];

	//###########################################################################
	SHORT iPmitt=getModel()->getDATAHANDLER()->getAVGMessureDataPmitt();

	if(m_eSize==NUMERICSIZE_1)
	{
		SelectObject(hdcMem,g_hf33AcuBoldNum);
		rc.top = 0;
		rc.bottom = 64;
		rc.left = 15; 
		rc.right = 175;
	}
	else //if(m_eSize==NUMERICSIZE_2)
	{
		SelectObject(hdcMem,g_hf70BoldNum);
		rc.top = 0;
		rc.bottom = 119;
		rc.left = 15; 
		rc.right = 175;
	}

	if(iPmitt==0)
		wsprintf(psz,_T("%d"), 0);
	else
	{
		//wsprintf(psz,_T("%0.0f"), ((double)iPmitt)/10);
		wsprintf(psz,_T("%0.1f"),CTlsFloat::Round(((double)iPmitt)/10, 1));
	}
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
//bool CNumericFieldPMEAN::drawFrames(CDC* pDC)
//{
//	HDC hdc = *pDC;
//	HBRUSH hPrevBrush = (HBRUSH)SelectObject(hdc, (HBRUSH)GetStockObject(WHITE_BRUSH));	
//	HPEN hPrevPen = (HPEN)SelectObject(hdc, (HPEN)GetStockObject(WHITE_PEN));
//
//	/*if(m_pcNumPara3er)
//		m_pcNumPara3er->Draw(hdc,0,5);*/
//
//	RECT rc;
//
//	
//
//	SelectObject(hdc, hPrevBrush);	
//	SelectObject(hdc, hPrevPen);
//
//	return true;
//}

// **************************************************************************
// 
// **************************************************************************
bool CNumericFieldPMEAN::drawStaticText(CDC* pDC)
{
	HDC hdc = *pDC;
	HFONT hPrevFont=(HFONT)SelectObject(hdc,g_hf13AcuBold);
	int nBkMode=SetBkMode(hdc,TRANSPARENT);
	int nPrevTxtColor=SetTextColor(hdc,RGB(0,0,0));
	RECT rc;

	if(m_eSize==NUMERICSIZE_1)
	{
		SelectObject(hdc,g_hf13AcuBold);
		rc.top = 9;
		rc.bottom = m_lY;
		rc.left = 15; 
		rc.right = 190;

		CSize sz1;

		pDC->DrawText(m_szName,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		sz1 = pDC->GetTextExtent(m_szName);

		SelectObject(hdc,g_hf7AcuBold);

		CSize szNote;

		rc.top = 13;
		rc.left = 17+sz1.cx;
		pDC->DrawText(m_szNameNote,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		szNote = pDC->GetTextExtent(m_szNameNote);

		SelectObject(hdc,g_hf7AcuNorm);

		rc.top = 10;
		rc.left = 25+sz1.cx+szNote.cx;
		pDC->DrawText(m_szUnit,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	}
	else //if(m_eSize==NUMERICSIZE_2)
	{
		SelectObject(hdc,g_hf17AcuBold);
		rc.top = 9;
		rc.bottom = m_lY;
		rc.left = 15; 
		rc.right = 190;

		CSize sz1;

		pDC->DrawText(m_szName,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		sz1 = pDC->GetTextExtent(m_szName);

		SelectObject(hdc,g_hf8AcuBold);

		CSize szNote;

		rc.top = 17;
		rc.left = 17+sz1.cx;
		pDC->DrawText(m_szNameNote,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		szNote = pDC->GetTextExtent(m_szNameNote);

		SelectObject(hdc,g_hf15AcuMed);

		rc.top = 8;
		rc.left = 25+sz1.cx+szNote.cx;
		pDC->DrawText(m_szUnit,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	}


	SetTextColor(hdc,nPrevTxtColor);
	SetBkMode(hdc,nBkMode);
	SelectObject(hdc,hPrevFont);
	return true;
}


// **************************************************************************
// 
// **************************************************************************
bool CNumericFieldPMEAN::drawLimits(CDC* pDC)
{
	HDC hdc = *pDC;
	int nBkMode=SetBkMode(hdc,TRANSPARENT);
	HFONT hPrevFont=(HFONT)SelectObject(hdc,g_hf11AcuBold);
	int nPrevTxtColor=SetTextColor(hdc,RGB(200,0,0));
	RECT rc;	
	TCHAR psz[MAX_PATH];

	//eVentMode eActiveVentMode=getModel()->getALARMHANDLER()->getSafeActiveVentMode();

	if(m_eSize==NUMERICSIZE_1)
	{
		drawHighLimit(pDC);
		drawLowLimit(pDC);

		rc.top = 9;
		rc.bottom = 59;
		rc.left = 0;
		rc.right = 187;
	}
	else //if(m_eSize==NUMERICSIZE_2)
	{
		rc.top = 58;
		rc.bottom = 110;
		rc.left = 0;
		rc.right = 187;
	}

	if(getModel()->getALARMHANDLER()->getAlimitState_MAPmaxLimit() == AL_CALC)
	{
		SelectObject(hdc,g_hf9AcuBold);
		pDC->DrawText(getModel()->GetLanguageString(IDS_TXT_AUTO),&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);
	}
	else if(getModel()->getALARMHANDLER()->getAlimitState_MAPmaxLimit() == AL_OFF)
	{
		SelectObject(hdc,g_hf9AcuBold);
		pDC->DrawText(getModel()->GetLanguageString(IDS_TXT_OFF),&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);
	}
	else if(getModel()->getALARMHANDLER()->getAlimitState_MAPmaxLimit() != AL_OFF)
	{
		SelectObject(hdc,g_hf11AcuBoldNum);
		if(getModel()->getALARMHANDLER()->getAlimitMAPmax()==0)
			wsprintf(psz,_T("%d"), 0);
		else
			wsprintf(psz,_T("%0.1f"), CTlsFloat::Round(((double)getModel()->getALARMHANDLER()->getAlimitMAPmax())/10, 1));
		pDC->DrawText(psz,&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);

		if(getModel()->getALARMHANDLER()->getAlimitMAPmin()==0)
			wsprintf(psz,_T("%d"), 0);
		else
			wsprintf(psz,_T("%0.1f"), CTlsFloat::Round(((double)getModel()->getALARMHANDLER()->getAlimitMAPmin())/10, 1));
		pDC->DrawText(psz,&rc,DT_BOTTOM|DT_SINGLELINE|DT_RIGHT);


		//if(getModel()->getALARMHANDLER()->getAlimitState_MAPmaxLimit() == AL_AUTO)
		//{
		//	if(m_eSize==NUMERICSIZE_1)
		//	{
		//		rc.bottom = 39;
		//	}
		//	else //if(m_eSize==NUMERICSIZE_2)
		//	{
		//		rc.bottom = 88;
		//	}
		//	SelectObject(hdc,g_hf9AcuBold);
		//	pDC->DrawText(getModel()->GetLanguageString(IDS_TXT_AUTO),&rc,DT_BOTTOM|DT_SINGLELINE|DT_RIGHT);
		//}
	}


	SetTextColor(hdc,nPrevTxtColor);
	SetBkMode(hdc,nBkMode);
	SelectObject(hdc,hPrevFont);
	return true;
}

