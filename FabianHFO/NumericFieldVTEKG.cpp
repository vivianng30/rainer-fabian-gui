// NumericFieldVTEKG.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "NumericFieldVTEKG.h"

/**********************************************************************************************//**
 * CNumericFieldVTEKG
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CNumericFieldVTEKG, CWnd)

/**********************************************************************************************//**
 * Initializes a new instance of the CNumericFieldVTEKG class
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	size	The size.
 **************************************************************************************************/

CNumericFieldVTEKG::CNumericFieldVTEKG(eNumericSize size):
CNumericField(size)
{
	/*if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
	{
		m_szName=getModel()->GetLanguageString(IDS_PARA_VTEHFOKG);
	}
	else
	{
		m_szName=getModel()->GetLanguageString(IDS_PARA_VTEKG);
	}*/
	m_szName=getModel()->GetLanguageString(IDS_PARA_V);
	m_szNameNote=getModel()->GetLanguageString(IDS_PARA_TE);
	m_szUnit=_T("[ml/kg]");
}

/**********************************************************************************************//**
 * Finalizes an instance of the CNumericFieldVTEKG class
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

CNumericFieldVTEKG::~CNumericFieldVTEKG()
{
}


BEGIN_MESSAGE_MAP(CNumericFieldVTEKG, CWnd)
	//{{AFX_MSG_MAP(CNumericField)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * CNumericFieldVTEKG message handlers
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CNumericFieldVTEKG::OnDestroy() 
{
	CNumericField::OnDestroy();
}

/**********************************************************************************************//**
 * Draw data
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	bData	  	True to data.
 * \param	bFrames   	True to frames.
 * \param	bText	  	True to text.
 * \param	bLimits   	True to limits.
 * \param	bFlowmeter	True to flowmeter.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CNumericFieldVTEKG::drawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter)
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
	int iTVE=0;
	if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
	{
		iTVE=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_TVE_HFO)*100;
	}
	else
	{
		iTVE=getModel()->getDATAHANDLER()->getMessureDataTVE()*100;
	}
	int iBW=getModel()->getDATAHANDLER()->getBodyweight();
	double iTVEKG=0;
	if(iBW!=0)
		iTVEKG=(double)iTVE/iBW;

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

	if(iBW==0)
		wsprintf(psz,_T("--"));
	else
	{
		//wsprintf(psz,_T("%0.1f"), CTlsFloat::Round(((double)iTVEKG)/10, 1));
		wsprintf(psz,_T("%0.1f"), CTlsFloat::Round(iTVEKG, 1));
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

//bool CNumericFieldVTEKG::drawFrames(CDC* pDC)
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

/**********************************************************************************************//**
 * Draw static text
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param [in,out]	pDC	If non-null, the device-context.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CNumericFieldVTEKG::drawStaticText(CDC* pDC)
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

		CSize sz1, szNote, szVTE, szBW;

		pDC->DrawText(m_szName,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		sz1 = pDC->GetTextExtent(m_szName);

		SelectObject(hdc,g_hf7AcuBold);

		rc.top = 13;
		rc.left = 17+sz1.cx;
		pDC->DrawText(m_szNameNote,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		szNote = pDC->GetTextExtent(m_szNameNote);

		

		szVTE=sz1.cx+szNote.cx;
		SelectObject(hdc,g_hf13AcuBold);

		CString csBW=_T("");
		rc.top = 9;
		rc.left = 19+szVTE.cx;
		if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
		{
			csBW=getModel()->GetLanguageString(IDS_PARA_VTEHFOKG);
		}
		else
		{
			csBW=getModel()->GetLanguageString(IDS_PARA_VTEKG);
		}
		pDC->DrawText(csBW,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		szBW = pDC->GetTextExtent(csBW);

		SelectObject(hdc,g_hf7AcuNorm);

		rc.top = 10;
		rc.left = 25+sz1.cx+szNote.cx+szBW.cx;
		pDC->DrawText(m_szUnit,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	}
	else //if(m_eSize==NUMERICSIZE_2)
	{
		SelectObject(hdc,g_hf17AcuBold);
		rc.top = 9;
		rc.bottom = m_lY;
		rc.left = 15; 
		rc.right = 190;

		CSize sz1, szNote, szVTE, szBW;

		pDC->DrawText(m_szName,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		sz1 = pDC->GetTextExtent(m_szName);

		SelectObject(hdc,g_hf8AcuBold);

		rc.top = 17;
		rc.left = 17+sz1.cx;
		pDC->DrawText(m_szNameNote,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		szNote = pDC->GetTextExtent(m_szNameNote);

		

		szVTE=sz1.cx+szNote.cx;
		SelectObject(hdc,g_hf17AcuBold);

		CString csBW=_T("");
		rc.top = 9;
		rc.left = 19+szVTE.cx;
		if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
		{
			csBW=getModel()->GetLanguageString(IDS_PARA_VTEHFOKG);
		}
		else
		{
			csBW=getModel()->GetLanguageString(IDS_PARA_VTEKG);
		}
		pDC->DrawText(csBW,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		szBW = pDC->GetTextExtent(csBW);

		SelectObject(hdc,g_hf8AcuNorm);

		rc.top = 11;
		rc.left = 25+sz1.cx+szNote.cx+szBW.cx;
		pDC->DrawText(m_szUnit,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	}



	SetTextColor(hdc,nPrevTxtColor);
	SetBkMode(hdc,nBkMode);
	SelectObject(hdc,hPrevFont);
	return true;
}

/**********************************************************************************************//**
 * Draw limits
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param [in,out]	pDC	If non-null, the device-context.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CNumericFieldVTEKG::drawLimits(CDC* pDC)
{
	return true;
}


