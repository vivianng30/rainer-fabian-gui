/**********************************************************************************************//**
 * \file	LimitBtn_FIO2high.cpp.
 *
 * Implements the limit button fio 2high class
 **************************************************************************************************/

#include "StdAfx.h"
#include "LimitBtn_FIO2high.h"
#include "globDefs.h"
#include "MVViewHandler.h"

//global font objects
extern HFONT g_hf7AcuNorm;
extern HFONT g_hf14AcuBold;
extern HFONT g_hf31AcuBold;
extern HFONT g_hf31AcuBoldNum;

/**********************************************************************************************//**
 * Initializes a new instance of the CLimitBtn_FIO2high class
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param [in,out]	parent	   	If non-null, the parent.
 * \param 		  	btn		   	The button.
 * \param 		  	nOffset	   	The offset.
 * \param 		  	bScrollOver	True to scroll over.
 **************************************************************************************************/

CLimitBtn_FIO2high::CLimitBtn_FIO2high(CWndDataSPO2 *parent, BTN btn, int nOffset,bool bScrollOver):
CLimitBtn(parent,btn,nOffset,bScrollOver)
{
	
}

/**********************************************************************************************//**
 * Finalizes an instance of the CLimitBtn_FIO2high class
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

CLimitBtn_FIO2high::~CLimitBtn_FIO2high(void)
{
	
}

BEGIN_MESSAGE_MAP(CLimitBtn_FIO2high, CButton)
	//{{AFX_MSG_MAP(CLimitBtn_FIO2high)
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
 * Draws
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	nState	The state.
 **************************************************************************************************/

void CLimitBtn_FIO2high::Draw(int nState)
{
	if(!m_bDisableText)
	{
		HFONT hPrevFont=(HFONT)SelectObject(m_hDC,g_hf14AcuBold);
		int nBkMode=SetBkMode(m_hDC,TRANSPARENT);
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

		RECT rc;
		memcpy(&rc,&m_rcClient,sizeof(RECT));

		//rc.top = 5;

		if(m_btn.dwFormat&DT_RIGHT)
			rc.right-=m_nOffset;
		else if(!(m_btn.dwFormat&DT_CENTER)) // left
			rc.left+=m_nOffset;

		TCHAR psz[MAX_PATH];

		wsprintf(psz,_T("%d"),m_v.iValue);

		DrawText(m_hDC,psz,-1,&rc,m_btn.dwFormat);

		//CRect rcName;

		//rcName.top = m_rcClient.top;
		//rcName.bottom = 30;
		//rcName.left = m_rcClient.left;
		//rcName.right = m_rcClient.right;

		//SelectObject(m_hDC,g_hf14AcuBold);
		//DrawText(m_hDC,m_pszNameText,-1,&rcName,m_btn.dwFormat);

		//RECT rcUnit;
		//rcUnit.top = m_rcClient.top;
		//rcUnit.bottom = m_rcClient.bottom-5;
		//rcUnit.left = m_rcClient.left;
		//rcUnit.right = m_rcClient.right;

		//SelectObject(m_hDC,g_hf7AcuNorm);

		//if(m_bEndOfRange)
		//	DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_ENDRANGE),-1,&rcUnit,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
		//else if(m_bLimited)
		//	DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_LIMITTED),-1,&rcUnit,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
		//else
		//{
		//	//DrawText(m_hDC,m_pszUnitText,-1,&rcUnit,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
		//}

		m_bEndOfRange=false;
		m_bLimited=false;

		SetTextColor(m_hDC,nPrevTxtColor);
		SetBkMode(m_hDC,nBkMode);
		SelectObject(m_hDC,hPrevFont);
	}
}

/**********************************************************************************************//**
 * Executes the key up action
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	nChar  	The character.
 * \param	nRepCnt	Number of reps.
 * \param	nFlags 	The flags.
 **************************************************************************************************/

void CLimitBtn_FIO2high::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if( m_bLMouseButtonDown )
	{
		if(nChar==m_kDown)	
		{
			if(m_bDepressed)
			{
				BYTE iFIO2low=getModel()->getDATAHANDLER()->getPRICO_FIO2lowRange();
				if(m_v.iValue>iFIO2low)
				{
					int dTemp = m_v.iValue;
					dTemp--;

					if(dTemp<=iFIO2low)
					{
						m_bLimited=true;
					}
					else
					{
						m_v.iValue=dTemp;
						WNDDATASPO2->setFIO2HIGHvalue(m_v.iValue);
					}
				}
				else if(m_bScrollOver)
					m_v.iValue=m_v.iUpperLimit;
				else
					m_bEndOfRange = true;

				CClientDC dc(this);
				m_btn.pcBmpDown->Draw(m_hDC);
				Draw(BTN_DW_DW);
				BitBlt(dc.m_hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
			}
			else
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_FOCUS_VKDOWN);
			}
		}
		else if(nChar==m_kUp)	
		{
			if(m_bDepressed)
			{
				if(m_v.iValue<m_v.iUpperLimit)
				{
					int dTemp = m_v.iValue;
					dTemp++;

					m_v.iValue=dTemp;
					WNDDATASPO2->setFIO2HIGHvalue(m_v.iValue);

				}
				else if(m_bScrollOver)
					m_v.iValue=m_v.iLowerLimit;
				else
					m_bEndOfRange = true;


				CClientDC dc(this);
				m_btn.pcBmpDown->Draw(m_hDC);
				Draw(BTN_DW_DW);
				BitBlt(dc.m_hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
			}
			else
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_FOCUS_VKUP);
			}
		}
		CLimitBtn::OnKeyUp(nChar,nRepCnt,nFlags);
	}
	m_bLMouseButtonDown = false;
}
