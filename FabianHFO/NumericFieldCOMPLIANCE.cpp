// NumericFieldCOMPLIANCE.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "NumericFieldCOMPLIANCE.h"


// CNumericFieldCOMPLIANCE

IMPLEMENT_DYNAMIC(CNumericFieldCOMPLIANCE, CWnd)

CNumericFieldCOMPLIANCE::CNumericFieldCOMPLIANCE(eNumericSize size):
CNumericField(size)
{
	m_szName=getModel()->GetLanguageString(IDS_PARA_C);
	m_szNameNote=_T("");
	if(getModel()->getCONFIG()->GetPressureUnit()==PUNIT_MBAR)
	{
		m_szUnit=_T("[")+getModel()->GetLanguageString(IDS_UNIT_MLMBAR)+_T("]");
	}
	else
	{
		m_szUnit=_T("[")+getModel()->GetLanguageString(IDS_UNIT_MLCMH2O)+_T("]");
	}
}

CNumericFieldCOMPLIANCE::~CNumericFieldCOMPLIANCE()
{
}


BEGIN_MESSAGE_MAP(CNumericFieldCOMPLIANCE, CWnd)
	//{{AFX_MSG_MAP(CNumericField)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



// CNumericFieldCOMPLIANCE message handlers

//************************************
// Method:    OnDestroy
// FullName:  CNumericFieldCOMPLIANCE::OnDestroy
// Access:    protected 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CNumericFieldCOMPLIANCE::OnDestroy() 
{
	CNumericField::OnDestroy();
}

// **************************************************************************
// 
// **************************************************************************
bool CNumericFieldCOMPLIANCE::drawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter)
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

	/*if(bLimits)
	{
		drawLimits(pDCStatic);
	}*/

	BitBlt(hdcMem, 0, 0, m_lX, m_lY,m_hdcStatic , 0, 0, SRCCOPY);

	HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf34BoldNum);
	int nBkMode=SetBkMode(hdcMem,TRANSPARENT);
	int nPrevTxtColor=SetTextColor(hdcMem,RGB(0,0,255));

	TCHAR psz[MAX_PATH];

	//###########################################################################
	WORD iC=getModel()->getDATAHANDLER()->getAVGMessureDataCompliance();

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

	BYTE iBPM=getModel()->getDATAHANDLER()->getAVGMessureDataBPM();

	if(iBPM==0)
	{
		wsprintf(psz,_T("%s"), _T("--"));
	}
	else if(iC==0)
		wsprintf(psz,_T("%d"), 0);
	else
		wsprintf(psz,_T("%0.2f"), CTlsFloat::Round(((double)iC)/100, 2));
	
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
//bool CNumericFieldCOMPLIANCE::drawFrames(CDC* pDC)
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
bool CNumericFieldCOMPLIANCE::drawStaticText(CDC* pDC)
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

		SelectObject(hdc,g_hf7AcuNorm);

		rc.top = 10;
		rc.left = 25+sz1.cx;
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

		SelectObject(hdc,g_hf15AcuMed);

		rc.top = 8;
		rc.left = 25+sz1.cx;
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
bool CNumericFieldCOMPLIANCE::drawLimits(CDC* pDC)
{
	
	return true;
}

