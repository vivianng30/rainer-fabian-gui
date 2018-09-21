// NumericFieldTAU.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "NumericFieldTAU.h"

/**********************************************************************************************//**
 * CNumericFieldTAU
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CNumericFieldTAU, CWnd)

/**********************************************************************************************//**
 * Initializes a new instance of the CNumericFieldTAU class
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	size	The size.
 **************************************************************************************************/

CNumericFieldTAU::CNumericFieldTAU(eNumericSize size):
CNumericField(size)
{
	//m_szName=getModel()->GetLanguageString(IDS_PARA_TAU);
	m_szName=_T("Time Constant");
	m_szNameNote=_T("");
	//m_szUnit=_T("[")+getModel()->GetLanguageString(IDS_UNIT_MLLPS)+_T("]");
	m_szUnit=_T("[")+getModel()->GetLanguageString(IDS_UNIT_SECONDS)+_T("]");

	//m_pcTauSize1	= new CBmp(theApp.m_hInstance,m_hDC,	IDB_NUM_TAU1);
	//m_pcTauSize2	= new CBmp(theApp.m_hInstance,m_hDC,	IDB_NUM_TAU2);

	
}

/**********************************************************************************************//**
 * Finalizes an instance of the CNumericFieldTAU class
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

CNumericFieldTAU::~CNumericFieldTAU()
{
}


BEGIN_MESSAGE_MAP(CNumericFieldTAU, CWnd)
	//{{AFX_MSG_MAP(CNumericField)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * CNumericFieldTAU message handlers
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CNumericFieldTAU::OnDestroy() 
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

bool CNumericFieldTAU::drawData(bool, bool bFrames, bool bText, bool, bool)
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
	WORD iR=getModel()->getDATAHANDLER()->getAVGMessureDataResistance();
	WORD iC=getModel()->getDATAHANDLER()->getAVGMessureDataCompliance();
	double iTau=(((double)iC/100)*((double)iR/10))/1000;

	//if(m_eSize==NUMERICSIZE_1)
	{
		SelectObject(hdcMem,g_hf33AcuBoldNum);
		rc.top = 0;
		rc.bottom = 64;
		rc.left = 15; 
		rc.right = 175;
	}
	//else //if(m_eSize==NUMERICSIZE_2)
	//{
	//	SelectObject(hdcMem,g_hf70BoldNum);
	//	rc.top = 0;
	//	rc.bottom = 119;
	//	rc.left = 15; 
	//	rc.right = 175;
	//}

	BYTE iBPM=0;

	if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
	{
		getModel()->getDATAHANDLER()->getAVGMessureDataHFFreq();
	}
	else
	{
		iBPM=getModel()->getDATAHANDLER()->getAVGMessureDataBPM();

	}

	if(iBPM==0)
	{
		wsprintf(psz,_T("%s"), _T("--"));
	}
	else
	{
		wsprintf(psz,_T("%0.2f"), CTlsFloat::Round(((double)iTau), 2));
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

//bool CNumericFieldTAU::drawFrames(CDC* pDC)
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

bool CNumericFieldTAU::drawStaticText(CDC* pDC)
{
	HDC hdc = *pDC;
	HFONT hPrevFont=(HFONT)SelectObject(hdc,g_hf11AcuBold);
	int nBkMode=SetBkMode(hdc,TRANSPARENT);
	int nPrevTxtColor=SetTextColor(hdc,RGB(0,0,0));
	RECT rc;

	if(m_eSize==NUMERICSIZE_1)
	{
		SelectObject(hdc,g_hf10AcuBold);
		rc.top = 9;
		rc.bottom = m_lY;
		rc.left = 15; 
		rc.right = 190;
		pDC->DrawText(m_szName,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

		if(m_pcTauSize1)
			m_pcTauSize1->Draw(hdc,126,7);

		
		SelectObject(hdc,g_hf7AcuNorm);

		rc.top = 10;
		rc.left = 160;//45;
		//rc.left = 25+sz1.cx;
		pDC->DrawText(m_szUnit,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	}
	//else //if(m_eSize==NUMERICSIZE_2)
	//{
	//	SelectObject(hdc,g_hf17AcuBold);
	//	rc.top = 9;
	//	rc.bottom = m_lY;
	//	rc.left = 15; 
	//	rc.right = 190;

	//	pDC->DrawText(m_szName,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

	//	SelectObject(hdc,g_hf15AcuMed);

	//	rc.top = 8;
	//	//rc.left = 25+sz1.cx;
	//	rc.left = 53;
	//	pDC->DrawText(m_szUnit,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	//}



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

bool CNumericFieldTAU::drawLimits(CDC*)
{
	
	return true;
}

