// NumericFieldFREQ.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "NumericFieldFREQ.h"


// CNumericFieldFREQ

IMPLEMENT_DYNAMIC(CNumericFieldFREQ, CWnd)

CNumericFieldFREQ::CNumericFieldFREQ(eNumericSize size):
CNumericField(size)
{
	m_szName=getModel()->GetLanguageString(IDS_PARA_FREQ);
	m_szNameNote=_T("");
	m_szUnit=_T("[")+getModel()->GetLanguageString(IDS_UNIT_BPM)+_T("]");
}

CNumericFieldFREQ::~CNumericFieldFREQ()
{
}


BEGIN_MESSAGE_MAP(CNumericFieldFREQ, CWnd)
	//{{AFX_MSG_MAP(CNumericField)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



// CNumericFieldFREQ message handlers

//************************************
// Method:    OnDestroy
// FullName:  CNumericFieldFREQ::OnDestroy
// Access:    protected 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CNumericFieldFREQ::OnDestroy() 
{
	CNumericField::OnDestroy();
}

// **************************************************************************
// 
// **************************************************************************
bool CNumericFieldFREQ::drawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter)
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
	if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==true && eActiveVentMode==VM_IPPV)
		bLimits=false;

	if(bLimits)
	{
		drawLimits(pDCStatic);
	}

	BitBlt(hdcMem, 0, 0, m_lX, m_lY,m_hdcStatic , 0, 0, SRCCOPY);

	HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf34BoldNum);
	int nBkMode=SetBkMode(hdcMem,TRANSPARENT);
	int nPrevTxtColor=SetTextColor(hdcMem,RGB(0,0,255));

	TCHAR psz[MAX_PATH];

	//###########################################################################
	BYTE iBPM=getModel()->getDATAHANDLER()->getAVGMessureDataBPM();

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

	if(getModel()->getVMODEHANDLER()->activeModeIsNCPAP() && getModel()->getDATAHANDLER()->PARADATA()->GetTriggerNMODEPara()==MAXRANGE_TRIGGER_NMODE_OFF)
	{
		DrawText(hdcMem,_T("--"),-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);
	}
	else
	{
		wsprintf(psz,_T("%d"),iBPM );
		DrawText(hdcMem,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);
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
//bool CNumericFieldFREQ::drawFrames(CDC* pDC)
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
bool CNumericFieldFREQ::drawStaticText(CDC* pDC)
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

		/*SelectObject(hdc,g_hf7AcuBold);

		CSize szNote;

		rc.top = 13;
		rc.left = 17+sz1.cx;
		pDC->DrawText(m_szNameNote,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		szNote = pDC->GetTextExtent(m_szNameNote);*/

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

		SelectObject(hdc,g_hf8AcuBold);

		/*CSize szNote;

		rc.top = 17;
		rc.left = 17+sz1.cx;
		pDC->DrawText(m_szNameNote,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		szNote = pDC->GetTextExtent(m_szNameNote);*/

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
bool CNumericFieldFREQ::drawLimits(CDC* pDC)
{
	HDC hdc = *pDC;
	int nBkMode=SetBkMode(hdc,TRANSPARENT);
	HFONT hPrevFont=(HFONT)SelectObject(hdc,g_hf11AcuBold);
	int nPrevTxtColor=SetTextColor(hdc,RGB(200,0,0));
	RECT rc;	
	TCHAR psz[MAX_PATH];

	if(m_eSize==NUMERICSIZE_1)
	{
		drawHighLimit(pDC);
		
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
	

	SetTextColor(hdc,nPrevTxtColor);
	SetBkMode(hdc,nBkMode);
	SelectObject(hdc,hPrevFont);
	return true;
}
