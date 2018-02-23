// NumericFieldFICO2.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "NumericFieldFICO2.h"

/**********************************************************************************************//**
 * CNumericFieldFICO2
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CNumericFieldFICO2, CWnd)

/**********************************************************************************************//**
 * Initializes a new instance of the CNumericFieldFICO2 class
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	size	The size.
 **************************************************************************************************/

CNumericFieldFICO2::CNumericFieldFICO2(eNumericSize size):
CNumericField(size)
{
	m_szName=getModel()->GetLanguageString(IDS_PARA_FICO);
	m_szNameNote=_T("2");
	
	eCO2unit eUnit=getModel()->getCONFIG()->GetCO2unit();
	switch(eUnit)
	{
	case CO2_UNIT_KPA:
		{
			m_szUnit =_T(" [")+getModel()->GetLanguageString(IDS_UNIT_KPA)+_T("]");
		}
		break;
	case CO2_UNIT_PERCENT:
		{
			m_szUnit =_T(" [")+getModel()->GetLanguageString(IDS_UNIT_VOLPERCENT)+_T("]");
		}
		break;
	case CO2_UNIT_MMHG:
	default:
		{
			m_szUnit =_T(" [")+getModel()->GetLanguageString(IDS_UNIT_MMHG)+_T("]");
		}
		break;
	}
}

/**********************************************************************************************//**
 * Finalizes an instance of the CNumericFieldFICO2 class
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

CNumericFieldFICO2::~CNumericFieldFICO2()
{
}


BEGIN_MESSAGE_MAP(CNumericFieldFICO2, CWnd)
	//{{AFX_MSG_MAP(CNumericField)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * CNumericFieldFICO2 message handlers
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CNumericFieldFICO2::OnDestroy() 
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

bool CNumericFieldFICO2::drawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter)
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
	SHORT iFICO2=0;
	bool bFICO2ValueValid=false;

	if(getModel()->getETCO2()!=NULL)
	{
		iFICO2=getModel()->getDATAHANDLER()->getAVGMessureDataFICO2();
		bFICO2ValueValid=getModel()->getETCO2()->isFICO2ValueValid();
	}

	if(bFICO2ValueValid)
	{
		wsprintf(psz,_T("%0.1f"), ((double)iFICO2)/10);
	}
	else
	{
		wsprintf(psz,_T("%s"), _T("--"));
	}

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


//bool CNumericFieldFICO2::drawFrames(CDC* pDC)
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

bool CNumericFieldFICO2::drawStaticText(CDC* pDC)
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

bool CNumericFieldFICO2::drawLimits(CDC* pDC)
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
	
	if(getModel()->getETCO2()!=NULL)
	{
		BYTE byCO2Module=getModel()->getCONFIG()->getCO2module();
		if(byCO2Module==CO2MODULE_MICROPOD)
		{
			//################FICO2 Max + Min##################
			eAlarmLimitState limitstateFIco2Max = getModel()->getALARMHANDLER()->getAlimitState_FICO2maxLimit();
			eAlarmLimitState limitstateFIco2Min = getModel()->getALARMHANDLER()->getAlimitState_FICO2minLimit();
			if(limitstateFIco2Max == AL_CALC)
			{
				SelectObject(hdc,g_hf9AcuBold);
				pDC->DrawText(getModel()->GetLanguageString(IDS_TXT_AUTO),&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);
			}
			else if(limitstateFIco2Max == AL_OFF)
			{
				SelectObject(hdc,g_hf9AcuBold);
				pDC->DrawText(getModel()->GetLanguageString(IDS_TXT_OFF),&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);
			}
			else if(limitstateFIco2Max != AL_OFF)
			{
				SelectObject(hdc,g_hf11AcuBoldNum);
				wsprintf(psz,_T("%0.0f"),CTlsFloat::Round(((double)getModel()->getALARMHANDLER()->getAlimitFICO2max())/10, 0));
				pDC->DrawText(psz,&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);
			}

			if(limitstateFIco2Min == AL_CALC)
			{
				SelectObject(hdc,g_hf9AcuBold);
				pDC->DrawText(getModel()->GetLanguageString(IDS_TXT_AUTO),&rc,DT_BOTTOM|DT_SINGLELINE|DT_RIGHT);
			}
			else if(limitstateFIco2Min == AL_OFF)
			{
				SelectObject(hdc,g_hf9AcuBold);
				pDC->DrawText(getModel()->GetLanguageString(IDS_TXT_OFF),&rc,DT_BOTTOM|DT_SINGLELINE|DT_RIGHT);
			}
			else if(limitstateFIco2Min != AL_OFF)
			{
				SelectObject(hdc,g_hf11AcuBoldNum);
				wsprintf(psz,_T("%0.0f"),CTlsFloat::Round(((double)getModel()->getALARMHANDLER()->getAlimitFICO2min())/10, 0));
				pDC->DrawText(psz,&rc,DT_BOTTOM|DT_SINGLELINE|DT_RIGHT);
			}
		}
		else if(byCO2Module==CO2MODULE_CAPNOSTAT)
		{
			//################ETCO2 Max + Min##################
			
			eAlarmLimitState limitstateMax = getModel()->getALARMHANDLER()->getAlimitState_FICO2maxLimit();
			eAlarmLimitState limitstateMin = getModel()->getALARMHANDLER()->getAlimitState_FICO2minLimit();
			if(limitstateMax == AL_CALC)
			{
				SelectObject(hdc,g_hf9AcuBold);
				pDC->DrawText(getModel()->GetLanguageString(IDS_TXT_AUTO),&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);
			}
			else if(limitstateMax == AL_OFF)
			{
				SelectObject(hdc,g_hf9AcuBold);
				pDC->DrawText(getModel()->GetLanguageString(IDS_TXT_OFF),&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);
			}
			else if(limitstateMax != AL_OFF)
			{
				SelectObject(hdc,g_hf11AcuBoldNum);
				wsprintf(psz,_T("%0.0f"),CTlsFloat::Round(((double)getModel()->getALARMHANDLER()->getAlimitFICO2max())/10, 0));
				pDC->DrawText(psz,&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);
			}

			if(limitstateMin == AL_CALC)
			{
				SelectObject(hdc,g_hf9AcuBold);
				pDC->DrawText(getModel()->GetLanguageString(IDS_TXT_AUTO),&rc,DT_BOTTOM|DT_SINGLELINE|DT_RIGHT);
			}
			else if(limitstateMin == AL_OFF)
			{
				SelectObject(hdc,g_hf9AcuBold);
				pDC->DrawText(getModel()->GetLanguageString(IDS_TXT_OFF),&rc,DT_BOTTOM|DT_SINGLELINE|DT_RIGHT);
			}
			else if(limitstateMin != AL_OFF)
			{
				SelectObject(hdc,g_hf11AcuBoldNum);
				wsprintf(psz,_T("%0.0f"),CTlsFloat::Round(((double)getModel()->getALARMHANDLER()->getAlimitFICO2min())/10, 0));
				pDC->DrawText(psz,&rc,DT_BOTTOM|DT_SINGLELINE|DT_RIGHT);
			}
		}
	}
	else
	{
		pDC->DrawText(_T("--"),&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);
		pDC->DrawText(_T("--"),&rc,DT_BOTTOM|DT_SINGLELINE|DT_RIGHT);
	}
	
	

	SetTextColor(hdc,nPrevTxtColor);
	SetBkMode(hdc,nBkMode);
	SelectObject(hdc,hPrevFont);
	return true;
}


