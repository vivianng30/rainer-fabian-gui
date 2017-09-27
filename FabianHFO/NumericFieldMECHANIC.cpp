// NumericFieldMECHANIC.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "NumericFieldMECHANIC.h"


// CNumericFieldMECHANIC

IMPLEMENT_DYNAMIC(CNumericFieldMECHANIC, CWnd)

CNumericFieldMECHANIC::CNumericFieldMECHANIC(eNumericSize size):
CNumericField(size)
{
	//m_szName1=getModel()->GetLanguageString(IDS_PARA_P);
	m_szName1=getModel()->GetLanguageString(IDS_PARA_PIP);
	m_szName2=getModel()->GetLanguageString(IDS_PARA_P);
	m_szName3=getModel()->GetLanguageString(IDS_PARA_PEEP);
	m_szName7=getModel()->GetLanguageString(IDS_PARA_C);
	m_szName8=getModel()->GetLanguageString(IDS_PARA_R);

	//m_szNameNote1=getModel()->GetLanguageString(IDS_PARA_MAX);
	m_szNameNote2=getModel()->GetLanguageString(IDS_PARA_MITT);

	//m_szNameNote1=_T("");


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

	m_szName=_T("");
	m_szNameNote=_T("");
	m_szUnit=_T("");
}

CNumericFieldMECHANIC::~CNumericFieldMECHANIC()
{
}


BEGIN_MESSAGE_MAP(CNumericFieldMECHANIC, CWnd)
	//{{AFX_MSG_MAP(CNumericField)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



// CNumericFieldMECHANIC message handlers

//************************************
// Method:    OnDestroy
// FullName:  CNumericFieldMECHANIC::OnDestroy
// Access:    protected 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CNumericFieldMECHANIC::OnDestroy() 
{
	CNumericField::OnDestroy();
}

// **************************************************************************
// 
// **************************************************************************
bool CNumericFieldMECHANIC::drawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter)
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
	
	if(iBPM==0)
	{
		wsprintf(psz,_T("%s"), _T("--"));
	}
	else if(iC20C==0)
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
	
	if(iBPM==0)
	{
		wsprintf(psz,_T("%s"), _T("--"));
	}
	else if(iR==0)
		wsprintf(psz,_T("%d"), 0);
	else
		wsprintf(psz,_T("%0.0f"), CTlsFloat::Round(((double)iR)/10, 0));
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
//bool CNumericFieldMECHANIC::drawFrames(CDC* pDC)
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
bool CNumericFieldMECHANIC::drawStaticText(CDC* pDC)
{
	HDC hdc = *pDC;
	HFONT hPrevFont=(HFONT)SelectObject(hdc,g_hf13AcuBold);
	int nBkMode=SetBkMode(hdc,TRANSPARENT);
	int nPrevTxtColor=SetTextColor(hdc,RGB(0,0,0));
	RECT rc;

	SelectObject(hdc,g_hf13AcuBold);
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

	SelectObject(hdc,g_hf7AcuNorm);

	rc.top = 9;
	rc.bottom = 54;
	rc.left = 10;
	rc.right = 188;
	pDC->DrawText(m_szUnitMLMBAR,&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);

	rc.top = 133;
	rc.bottom = 178;
	pDC->DrawText(m_szUnitMBARLPS,&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);


	/*CSize sz1;

	pDC->DrawText(m_szName,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	sz1 = pDC->GetTextExtent(m_szName);

	SelectObject(hdc,g_hf7AcuBold);*/

	

	SetTextColor(hdc,nPrevTxtColor);
	SetBkMode(hdc,nBkMode);
	SelectObject(hdc,hPrevFont);
	return true;
}


// **************************************************************************
// 
// **************************************************************************
bool CNumericFieldMECHANIC::drawLimits(CDC* pDC)
{
	
	return true;
}

