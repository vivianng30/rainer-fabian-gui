/**********************************************************************************************//**
 * \file	LimitBtn.cpp.
 *
 * Implements the limit button class
 **************************************************************************************************/

#include "StdAfx.h"
#include "LimitBtn.h"

// CLimitBtn.cpp: Implementierungsdatei
//
#include "globDefs.h"
#include "MVViewHandler.h"
#include "WndDataSPO2.h"

// CParaBtn

CWndDataSPO2* CLimitBtn::WNDDATASPO2=0;

/**********************************************************************************************//**
 * Initializes a new instance of the LimitBtn class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CLimitBtn, CButton)

/**********************************************************************************************//**
 * Initializes a new instance of the CLimitBtn class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	parent	   	If non-null, the parent.
 * \param [in,out]	btn		   	The button.
 * \param 		  	nOffset	   	The offset.
 * \param 		  	bScrollOver	True to scroll over.
 **************************************************************************************************/

CLimitBtn::CLimitBtn(CWndDataSPO2 *parent, BTN &btn, int nOffset,bool bScrollOver)
{
	// Init state
	
	WNDDATASPO2=parent;

	m_pModel=NULL;

	m_hBm=NULL;
	m_hBmPrev=NULL;

	m_v.iValue;
	m_v.iUpperLimit;
	m_v.iLowerLimit;
	//m_v.tText[0]=0x0000;
	//m_iCountUDP=0;

	m_dwPushDelta=0;

	m_iPrevValue=0;
	m_bLastParaState=false;

	m_pcBmpFocusDown=NULL;

	m_bTurnedOff=false;
	m_bHasFocus=false;
	m_bDepressed=false;
	m_bLMouseButtonDown=false;
	m_bLButtonDown=false;
	
	m_bLimited=false;
	m_bEndOfRange = false;
	m_bOff=false;

	m_kUp					= VK_UP;
	m_kDown					= VK_DOWN;
	m_kSpace				= VK_SPACE;
	//m_kSpace				= VK_RETURN;

	memcpy(&m_btn,&btn,sizeof(BTN));
	// Default colors
	m_btncr.crTxtUp			= 0x00000000;
	m_btncr.crTxtDown		= 0x00000000;
	m_btncr.crTxtDisabled	= 0x00c0c0c0;;		
	m_btncr.crTxtFocus		= 0x00000000;	
	//
	m_hDC=NULL;
	//
	m_pszValueText[0]=0x0000;
	m_pszNameText[0]=0x0000;
	m_pszNameNoteText[0]=0x0000;
	m_pszUnitText[0]=0x0000;
	
	m_bTextOnly=false;
	m_nOffset=nOffset;
	m_bDisableText=false;
	m_bScrollOver=bScrollOver;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CLimitBtn class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

CLimitBtn::~CLimitBtn()
{
	if(m_hDC)
	{
		SelectObject(m_hDC,m_hBmPrev);
		DeleteObject(m_hBm);
		DeleteDC(m_hDC);
		//CDC::FromHandle(m_hDC)->DeleteDC();
	}
}

/**********************************************************************************************//**
 * Gets the model
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	Null if it fails, else the model.
 **************************************************************************************************/

CMVModel *CLimitBtn::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}


BEGIN_MESSAGE_MAP(CLimitBtn, CButton)
	//{{AFX_MSG_MAP(CLimitBtn)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_KILLFOCUS()
	/*ON_WM_SETFOCUS()*/
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * CLimitBtn-Meldungshandler
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	pParentWnd	If non-null, the parent window.
 * \param 		  	dwStyle   	The style.
 * \param 		  	v		  	A fVALUE to process.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

BOOL CLimitBtn::Create(CWnd* pParentWnd,DWORD dwStyle, fVALUE v) 
{
	SIZE sz;
	sz.cx=m_btn.pcBmpUp->Width();
	sz.cy=m_btn.pcBmpUp->Height();
	CRect cRect(m_btn.poPosition,sz);

	//if(CButton::Create(_T("btn"),WS_CHILD|WS_VISIBLE|BS_OWNERDRAW|WS_TABSTOP,cRect,pParentWnd,m_btn.wID))
	if(CButton::Create(_T("btn"),dwStyle,cRect,pParentWnd,m_btn.wID))
	{
		CClientDC dc(this);

		m_hDC=CreateCompatibleDC(dc.m_hDC);
		m_hBm=CreateCompatibleBitmap(dc.m_hDC,m_btn.pcBmpUp->Width(),m_btn.pcBmpUp->Height());
		m_hBmPrev=(HBITMAP)SelectObject(m_hDC,m_hBm);

		
		m_rcClient.left=m_rcClient.top=0;
		m_rcClient.right=sz.cx;
		m_rcClient.bottom=sz.cy;

		memcpy(&m_v,&v,sizeof(fVALUE));

		return 1;
	}
	return 0;
}

/**********************************************************************************************//**
 * Sets the bitmaps
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	pcBmpUp		 	If non-null, the PC bitmap up.
 * \param [in,out]	pcBmpDown	 	If non-null, the PC bitmap down.
 * \param [in,out]	pcBmpDisabled	If non-null, the PC bitmap disabled.
 * \param [in,out]	pcBmpFocus   	If non-null, the PC bitmap focus.
 * \param 		  	bSignaled	 	True if signaled.
 * \param 		  	bRedraw		 	True to redraw.
 **************************************************************************************************/

void CLimitBtn::SetBitmaps(CBmp* pcBmpUp,CBmp* pcBmpDown,CBmp* pcBmpDisabled,CBmp* pcBmpFocus,bool,bool bRedraw)
{
	if(m_btn.pcBmpUp!=pcBmpUp)
	{
		m_btn.pcBmpUp=pcBmpUp;
		m_btn.pcBmpDown=pcBmpDown;
		m_btn.pcBmpDisabled=pcBmpDisabled;
		m_btn.pcBmpFocus=pcBmpFocus;
		if(bRedraw)
		{
			Invalidate();
			UpdateWindow();
		}
		
	}
}

/**********************************************************************************************//**
 * Query if this instance is depressed
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	True if depressed, false if not.
 **************************************************************************************************/

bool CLimitBtn::IsDepressed( void )
{
	// Return the buttons state
	return m_bDepressed;
}

/**********************************************************************************************//**
 * Depress this instance
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CLimitBtn::Depress()
{
	// Set the buttons state
	if(m_bDepressed)
	{
		m_v.iValue=m_iPrevValue;
		m_bOff=m_bLastParaState;

		WriteCurrentValue();

		if(m_btn.wID==IDC_BTN_LIMIT_FIO2LOW)//PRICO02
		{
			//getModel()->getVIEWHANDLER()->setFIO2LOWvalue(m_v.iValue);
			WNDDATASPO2->setFIO2LOWvalue((BYTE) m_v.iValue);
		}
		else if(m_btn.wID==IDC_BTN_LIMIT_FIO2HIGH)//PRICO02
		{
			WNDDATASPO2->setFIO2HIGHvalue((BYTE) m_v.iValue);
		}
		else if(m_btn.wID==IDC_BTN_LIMIT_SPO2LOW)//PRICO02
		{
			WNDDATASPO2->setSPO2LOWvalue((BYTE) m_v.iValue);
		}
		else if(m_btn.wID==IDC_BTN_LIMIT_SPO2HIGH)//PRICO02
		{
			WNDDATASPO2->setSPO2HIGHvalue((BYTE) m_v.iValue);
		}

		if(GetParent())
			GetParent()->PostMessage(WM_SET_PARATIMER,m_btn.wID);
	}
	else
	{
		m_iPrevValue=m_v.iValue;
		m_bLastParaState=m_bOff;
		if(GetParent())
			GetParent()->PostMessage(WM_SET_PARATIMER);
	}
	m_bDepressed = false;
	m_bTurnedOff=false;

	Invalidate(false);
	return m_bDepressed;
}

/**********************************************************************************************//**
 * Sets the colors
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	crTxtUp			The carriage return text up.
 * \param	crTxtDown   	The carriage return text down.
 * \param	crSubTxtDown	The carriage return sub text down.
 * \param	crTxtFocus  	The carriage return text focus.
 * \param	crDisabel   	The carriage return disabel.
 * \param	bRefresh		True to refresh.
 **************************************************************************************************/

void CLimitBtn::SetColors(COLORREF crTxtUp,COLORREF crTxtDown,COLORREF crSubTxtDown,COLORREF crTxtFocus, COLORREF crDisabel, bool bRefresh)
{
	m_btncr.crTxtUp			= crTxtUp;
	m_btncr.crTxtDown		= crTxtDown;
	m_btncr.crTxtDisabled	= crDisabel;		
	m_btncr.crTxtFocus		= crTxtFocus;
	m_btncr.crSubTxtDown	= crSubTxtDown;

	if(bRefresh)
	{
		Invalidate();
		UpdateWindow();
	}
}

/**********************************************************************************************//**
 * Sets para value
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	v	An int to process.
 **************************************************************************************************/

void CLimitBtn::SetParaValue(int v)
{
	bool bRedraw=false;
	if(m_v.iValue!=v)
		bRedraw=true;

	m_v.iValue=v;
	if(bRedraw)
	{
		Invalidate();
		UpdateWindow();
	}

}

/**********************************************************************************************//**
 * Sets a value
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	v	   	A fVALUE to process.
 * \param	bRedraw	True to redraw.
 **************************************************************************************************/

void CLimitBtn::SetValue(fVALUE v, bool bRedraw) 
{
	memcpy(&m_v,&v,sizeof(fVALUE));

	if(bRedraw)
	{
		//Invalidate();
		//UpdateWindow();
		RefreshBtn();
	}
}

/**********************************************************************************************//**
 * Refresh button state
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CLimitBtn::RefreshBtnState()
{
	CClientDC dc(this);
	bool bStateChanged=false;

	/*if(		m_btn.wID==IDC_BTN_PARA_ITIME
		||	m_btn.wID==IDC_BTN_PARA_ETIME
		||	m_btn.wID==IDC_BTN_PARA_BPM)
	{
		
	}*/

	if(bStateChanged)
	{
		Invalidate();
		UpdateWindow();
	}

	
}

/**********************************************************************************************//**
 * Refresh button
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CLimitBtn::RefreshBtn()
{
	CClientDC dc(this);

	/*if(		m_btn.wID==IDC_BTN_PARA_ITIME
		||	m_btn.wID==IDC_BTN_PARA_ETIME
		||	m_btn.wID==IDC_BTN_PARA_BPM)
	{
		
	}*/

	Invalidate();
	UpdateWindow();
	
}

/**********************************************************************************************//**
 * Disables the value text
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	bDisable	True to disable, false to enable.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CLimitBtn::DisableValueText(bool bDisable)
{
	m_bDisableText=bDisable;
	Invalidate();
	UpdateWindow();
	return true;
}

/**********************************************************************************************//**
 * Sets value text
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	pszText  	If non-null, the text.
 * \param 		  	bTextOnly	True to text only.
 **************************************************************************************************/

void CLimitBtn::SetValueText(TCHAR* pszText,bool bTextOnly) 
{
	_tcscpy_s(m_pszValueText,_countof(m_pszValueText),pszText);
	m_bTextOnly=bTextOnly;
}

/**********************************************************************************************//**
 * Sets value text
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	szText   	The text.
 * \param 		  	bTextOnly	True to text only.
 **************************************************************************************************/

void CLimitBtn::SetValueText(CStringW& szText,bool bTextOnly) 
{
	_tcscpy_s(m_pszValueText,_countof(m_pszValueText),szText);
	m_bTextOnly=bTextOnly;
}

/**********************************************************************************************//**
 * Sets name text
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	pszText	If non-null, the text.
 **************************************************************************************************/

void CLimitBtn::SetNameText(TCHAR* pszText)
{
	_tcscpy_s(m_pszNameText,_countof(m_pszNameText),pszText);
}

/**********************************************************************************************//**
 * Sets name text
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	szText	The text.
 **************************************************************************************************/

void CLimitBtn::SetNameText(CStringW& szText)
{
	_tcscpy_s(m_pszNameText,_countof(m_pszNameText),szText);
}

/**********************************************************************************************//**
 * Sets name note text
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	pszText 	If non-null, the text.
 * \param 		  	bRefresh	True to refresh.
 **************************************************************************************************/

void CLimitBtn::SetNameNoteText(TCHAR* pszText, bool bRefresh)
{
	_tcscpy_s(m_pszNameNoteText,_countof(m_pszNameNoteText),pszText);

	if(bRefresh)
	{
		RefreshBtn();
	}
}

/**********************************************************************************************//**
 * Sets name note text
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	szText  	The text.
 * \param 		  	bRefresh	True to refresh.
 **************************************************************************************************/

void CLimitBtn::SetNameNoteText(CStringW& szText, bool bRefresh)
{
	_tcscpy_s(m_pszNameNoteText,_countof(m_pszNameNoteText),szText);

	if(bRefresh)
	{
		RefreshBtn();
	}
}

/**********************************************************************************************//**
 * Sets unit text
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	pszText	If non-null, the text.
 **************************************************************************************************/

void CLimitBtn::SetUnitText(TCHAR* pszText)
{
	_tcscpy_s(m_pszUnitText,_countof(m_pszUnitText),pszText);
}

/**********************************************************************************************//**
 * Sets unit text
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	szText	The text.
 **************************************************************************************************/

void CLimitBtn::SetUnitText(CStringW& szText)
{
	_tcscpy_s(m_pszUnitText,_countof(m_pszUnitText),szText);
}

/**********************************************************************************************//**
 * Refresh value text
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	szText   	The text.
 * \param 		  	bTextOnly	True to text only.
 **************************************************************************************************/

void CLimitBtn::RefreshValueText(CStringW& szText,bool bTextOnly) 
{
	SetValueText(szText,bTextOnly);
	Invalidate();
	UpdateWindow();
}

/**********************************************************************************************//**
 * Sets the limits
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	nLower	The lower.
 * \param	nUpper	The upper.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CLimitBtn::SetLimits(int nLower,int nUpper)
{
	m_v.iLowerLimit=nLower;
	m_v.iUpperLimit=nUpper;
	return true;
}

/**********************************************************************************************//**
 * Draw direct up
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	bCheckValueChanged	True if check value changed.
 **************************************************************************************************/

void CLimitBtn::DrawDirectUp(bool)
{
	m_bDepressed=false;
	m_bTurnedOff=false;
	
	CClientDC dc(this);

	m_btn.pcBmpUp->Draw(m_hDC);
	Draw(BTN_DW_UP);
	BitBlt(dc.m_hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
}

/**********************************************************************************************//**
 * Draw item
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	lpDrawItemStruct	The draw item structure.
 **************************************************************************************************/

void CLimitBtn::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	int iID = m_btn.wID;
	if (m_bDepressed && !(lpDrawItemStruct->itemState & ODS_DISABLED))
	{
		// Button down
		if(lpDrawItemStruct->itemState & ODS_FOCUS)	
		{
			m_bHasFocus=false;
			m_btn.pcBmpDown->Draw(m_hDC);
			Draw(BTN_DW_DW);
			BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
		}
		else
		{
			m_bHasFocus=false;
			m_btn.pcBmpUp->Draw(m_hDC);
			Draw(BTN_DW_UP);
			BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
		}
	}
	else if(lpDrawItemStruct->itemState & ODS_DISABLED )
	{
		// Button disabled
		if(!m_bDepressed)//up
		{
			m_btn.pcBmpDisabled->Draw(m_hDC);
			//m_btn.pcBmpUp->Draw(m_hDC);
			Draw(BTN_DW_DIS);
			BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
		}
		else // down
		{
			m_btn.pcBmpDisabled->Draw(m_hDC);
			//m_btn.pcBmpUp->Draw(m_hDC);
			Draw(BTN_DW_DIS);
			BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
		}
	}
	else	
	{
		// Button up
		if(lpDrawItemStruct->itemState & ODS_FOCUS)
		{
			m_bHasFocus=true;
			m_btn.pcBmpFocus->Draw(m_hDC);
			Draw(BTN_DW_FOCUS);
			BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
		}
		else
		{
			m_bHasFocus=false;
			m_btn.pcBmpUp->Draw(m_hDC);
			Draw(BTN_DW_UP);
			BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
		}
	}	
}

/**********************************************************************************************//**
 * Draws
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	nState	The state.
 **************************************************************************************************/

void CLimitBtn::Draw(int nState)
{
	if(!m_bDisableText)
	{
		HFONT hPrevFont=(HFONT)SelectObject(m_hDC,g_hf31AcuBoldNum);
		int nTxtColor;

		switch(nState)
		{
		case BTN_DW_FOCUS:
			nTxtColor=m_btncr.crTxtFocus;
			break;
		case BTN_DW_UP:
			nTxtColor=m_btncr.crTxtUp;
			break;
		case BTN_DW_DW:
			nTxtColor=m_btncr.crTxtDown;
			break;
		case BTN_DW_DIS:
		default:
			nTxtColor=m_btncr.crTxtDisabled;
			break;
		}
		int nPrevTxtColor=SetTextColor(m_hDC,nTxtColor);
		int nBkMode=SetBkMode(m_hDC,TRANSPARENT);

		RECT rc;
		memcpy(&rc,&m_rcClient,sizeof(RECT));

		if(m_btn.dwFormat&DT_RIGHT)
			rc.right-=m_nOffset;
		else if(!(m_btn.dwFormat&DT_CENTER)) // left
			rc.left+=m_nOffset;

		TCHAR psz[MAX_PATH];
		wsprintf(psz,_T("%d"),m_v.iValue);
		DrawText(m_hDC,psz,-1,&rc,m_btn.dwFormat);

		rc.top = m_rcClient.top;
		rc.bottom = 30;
		SelectObject(m_hDC,g_hf14AcuBold);
		DrawText(m_hDC,m_pszNameText,-1,&rc,m_btn.dwFormat);

		rc.bottom = m_rcClient.bottom-8;
		SelectObject(m_hDC,g_hf7AcuNorm);
		DrawText(m_hDC,m_pszUnitText,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);

		SetTextColor(m_hDC,nPrevTxtColor);
		SetBkMode(m_hDC,nBkMode);
		SelectObject(m_hDC,hPrevFont);
	}
}

/**********************************************************************************************//**
 * Translates all messages before they are processed by the main message loop
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	pMsg	If non-null, the message.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

BOOL CLimitBtn::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message)
	{
	case WM_LBUTTONDBLCLK:
	case WM_LBUTTONDOWN:
		{
			/*if(GetParent())
				GetParent()->PostMessage(WM_SET_PARATIMER);

			if(		m_btn.wID==IDC_BTN_LIMIT_SPO2LOW
				||	m_btn.wID==IDC_BTN_LIMIT_SPO2HIGH
				||	m_btn.wID==IDC_BTN_LIMIT_FIO2LOW
				||	m_btn.wID==IDC_BTN_LIMIT_FIO2HIGH)
			{
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_CURRENT_VIEWID,IDC_VIEW_GRAPH,0);
			}
			else
			{
				
			}*/
			
		}
		break;
	default:
		{

		}
		break;
	}
	return CButton::PreTranslateMessage(pMsg);
}

/**********************************************************************************************//**
 * Gets a button
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	pnValue	If non-null, the pn value.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CLimitBtn::GetButton(int* pnValue) 
{
	if(pnValue)
	{
		*pnValue=m_v.iValue;
	}
	return m_bDepressed;
}

/**********************************************************************************************//**
 * Sets ud keys
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	kUP   	The up.
 * \param	kDown 	The down.
 * \param	kSpace	The space.
 **************************************************************************************************/

void CLimitBtn::SetUDKeys(WORD kUP, WORD kDown, WORD kSpace)
{
	m_kUp=kUP;
	m_kDown=kDown;
	m_kSpace=kSpace;
}

/**********************************************************************************************//**
 * Gets button identifier
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	The button identifier.
 **************************************************************************************************/

int CLimitBtn::GetBtnId()
{
	return m_btn.wID;
}

/**********************************************************************************************//**
 * Executes the l button double clock action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	nFlags	The flags.
 * \param	point 	The point.
 **************************************************************************************************/

void CLimitBtn::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	DWORD lParam;
	WORD  po[2]={(WORD)point.x,(WORD)point.y};
	memcpy(&lParam,po,sizeof(DWORD));

	SendMessage(WM_LBUTTONDOWN,nFlags,lParam);
	CButton::OnLButtonDblClk(nFlags, point);
}

/**********************************************************************************************//**
 * Executes the l button down action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	nFlags	The flags.
 * \param	point 	The point.
 **************************************************************************************************/

void CLimitBtn::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(!m_bLMouseButtonDown)
	{
		//getModel()->getCONFIG()->SetLastSelectedParaViewBtns(m_btn.wID);

		m_bLMouseButtonDown = true;
		CButton::OnLButtonDown(nFlags, point);
	}
}

/**********************************************************************************************//**
 * Executes the l button up action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	nFlags	The flags.
 * \param	point 	The point.
 **************************************************************************************************/

void CLimitBtn::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if( m_bLMouseButtonDown )
	{
		if(ChildWindowFromPoint(point)==FromHandle(this->m_hWnd))
		{
			if(m_bDepressed)
			{
				/*if(m_btn.wID==IDC_BTN_LIMIT_SPO2LOW)
				{
					
				}
				else if(m_btn.wID==IDC_BTN_LIMIT_SPO2HIGH)
				{
				
				}
				else if(m_btn.wID==IDC_BTN_LIMIT_FIO2LOW)
				{
					
				}
				else if(m_btn.wID==IDC_BTN_LIMIT_FIO2HIGH)
				{
					
				}*/
				
				WriteCurrentValue();
				if(GetParent())
					GetParent()->PostMessage(WM_SET_PARATIMER,m_btn.wID);
			}
			else
			{
				m_iPrevValue=m_v.iValue;
				m_bLastParaState=m_bOff;
				if(GetParent())
					GetParent()->PostMessage(WM_SET_PARATIMER);
			}
			m_bDepressed = !m_bDepressed;

			if(!m_bDepressed)
				m_bTurnedOff=false;
			
		}
		CButton::OnLButtonUp(nFlags, point);
	}
	m_bLMouseButtonDown = false;
}

/**********************************************************************************************//**
 * Executes the key down action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	nChar  	The character.
 * \param	nRepCnt	Number of reps.
 * \param	nFlags 	The flags.
 **************************************************************************************************/

void CLimitBtn::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if( !m_bLMouseButtonDown /*&& !m_bAutoState*/)
	{
		m_bLMouseButtonDown = true;
		if(nChar==32 /*&& !m_bAutoState*/)	//VK_SPACE
		{
			if(!isSafeTickCountDelayExpired(m_dwPushDelta, BTN_DELAY))
			{
				return;
			}

			m_dwPushDelta=GetTickCount();

			if(m_bDepressed)
			{
				/*if(m_bWaitConfirm)
				{
					m_iPrevValue=m_v.iValue;
					m_bLastParaState=m_bOff;

					m_bKeyBeep=TRUE;
					m_bWaitConfirm=false;
					m_bKeyValueAccepted=true;
					m_bDrawKey=false;
					if(		m_btn.wID==IDC_BTN_PARA_ITIME
						||	m_btn.wID==IDC_BTN_PARA_ETIME
						||	m_btn.wID==IDC_BTN_PARA_BPM)
					{
						if(GetParent())
							GetParent()->PostMessage(WM_IE_VALUE_CONFIRMED);
					}
				}
				else*/
				{
					/*if(m_btn.wID==IDC_BTN_LIMIT_SPO2LOW)
					{
						
					}
					else if(m_btn.wID==IDC_BTN_LIMIT_SPO2HIGH)
					{
						
					}
					else if(m_btn.wID==IDC_BTN_LIMIT_FIO2LOW)
					{
						
					}
					else if(m_btn.wID==IDC_BTN_LIMIT_FIO2HIGH)
					{
						
					}*/
					
					WriteCurrentValue();
					if(GetParent())
						GetParent()->PostMessage(WM_SET_PARATIMER,m_btn.wID);
					m_bDepressed = !m_bDepressed;

					/*if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->SetFocus();*/
				}
			}
			else
			{
				m_iPrevValue=m_v.iValue;
				m_bLastParaState=m_bOff;
				if(GetParent())
					GetParent()->PostMessage(WM_SET_PARATIMER);

				m_bDepressed = !m_bDepressed;
			}
			
			

			if(!m_bDepressed)
				m_bTurnedOff=false;
		}
		CButton::OnKeyDown(nChar,nRepCnt,nFlags);
	}
}

/**********************************************************************************************//**
 * Executes the key up action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	nChar  	The character.
 * \param	nRepCnt	Number of reps.
 * \param	nFlags 	The flags.
 **************************************************************************************************/

void CLimitBtn::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	m_dwPushDelta=GetTickCount();

	if(m_bDepressed)
	{
		if(GetParent())
			GetParent()->PostMessage(WM_SET_PARATIMER,m_btn.wID);
	}

	/*switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_PRE_IPPV:
	case VM_PRE_PSV:
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_SIPPV:
	case VM_PRE_CPAP:
	case VM_PRE_DUOPAP:
	case VM_PRE_NCPAP:
	case VM_PRE_HFO:
		if(AfxGetApp() != NULL)
			AfxGetApp()->GetMainWnd()->PostMessage(WM_VENTMODE_HOME);
		break;
	default:
		break;
	}*/
	CButton::OnKeyUp(nChar,nRepCnt,nFlags);
	
}

/**********************************************************************************************//**
 * Executes the kill focus action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	pNewWnd	If non-null, the new window.
 **************************************************************************************************/

void CLimitBtn::OnKillFocus(CWnd* pNewWnd)
{
	CButton::OnKillFocus(pNewWnd);
	Depress();
}

/**********************************************************************************************//**
 * Query if this instance is off
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	True if off, false if not.
 **************************************************************************************************/

bool CLimitBtn::IsOff()
{
	return m_bOff;
}

/**********************************************************************************************//**
 * Sets off state
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	state	True to state.
 **************************************************************************************************/

void CLimitBtn::SetOffState(bool state)
{
	if(m_bOff==state)
		return;
	m_bOff=state;
	Invalidate();
	UpdateWindow();
}

/**********************************************************************************************//**
 * Writes the current value
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CLimitBtn::WriteCurrentValue()
{
	if(theApp.IsScreenlock())
	{
		if(AfxGetApp() != NULL)
			AfxGetApp()->GetMainWnd()->PostMessage(WM_KILL_AUTOSCREENLOG_TIMER);
	}

	switch(m_btn.wID)
	{
	case IDC_BTN_LIMIT_SPO2LOW:
		{
			getModel()->getDATAHANDLER()->setPRICO_SPO2lowRange((BYTE) m_v.iValue);
			CString sz=_T("");
			sz.Format(_T("#PRICO SpO2 low:%d, "), m_v.iValue);
			theApp.WriteLog(sz);
		}
		break;
	case IDC_BTN_LIMIT_SPO2HIGH:
		{
			getModel()->getDATAHANDLER()->setPRICO_SPO2highRange((BYTE) m_v.iValue);
			CString sz=_T("");
			sz.Format(_T("#PRICO SpO2 high:%d, "), m_v.iValue);
			theApp.WriteLog(sz);
		}
		break;
	case IDC_BTN_LIMIT_FIO2LOW:
		{
			getModel()->getDATAHANDLER()->setPRICO_FIO2lowRange((BYTE) m_v.iValue);
			CString sz=_T("");
			sz.Format(_T("#PRICO FiO2 low:%d, "), m_v.iValue);
			theApp.WriteLog(sz);
		}
		break;
	case IDC_BTN_LIMIT_FIO2HIGH:
		{
			getModel()->getDATAHANDLER()->setPRICO_FIO2highRange((BYTE) m_v.iValue);
			CString sz=_T("");
			sz.Format(_T("#PRICO FiO2 high:%d, "), m_v.iValue);
			theApp.WriteLog(sz);
		}
		break;
	default:
		break;
	}
}

/**********************************************************************************************//**
 * Query if 'oldTickCount' is safe tick count delay expired
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	oldTickCount	Number of old ticks.
 * \param	delay			The delay.
 *
 * \return	True if safe tick count delay expired, false if not.
 **************************************************************************************************/

bool CLimitBtn::isSafeTickCountDelayExpired(DWORD oldTickCount, UINT delay)////used to check if old tick count plus delay is still lower than actual tickCount, (dwLastTickCount+DELAY<getTickCount64())
{
	bool bExpired=false;
	DWORD curTickCount=GetTickCount();

	if(curTickCount<oldTickCount)//overrun
	{
		DWORD diff=0xFFFFFFFF-oldTickCount;

		if(diff+delay<curTickCount)
		{
			bExpired=true;
		}
	}
	else
	{
		if(oldTickCount+delay<curTickCount)
		{
			bExpired=true;
		}
	}
	return bExpired;
}