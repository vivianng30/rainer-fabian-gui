/**********************************************************************************************//**
 * \file	ParaBtn_VLIMIT.cpp.
 *
 * Implements the para button vlimit class
 **************************************************************************************************/

#include "StdAfx.h"
#include "ParaBtn_VLIMIT.h"
#include "globDefs.h"

extern HFONT g_hf7AcuNorm;
extern HFONT g_hf14AcuBold;
extern HFONT g_hf31AcuBold;
extern HFONT g_hf8AcuBold;
extern HFONT g_hf23AcuBold;

/**********************************************************************************************//**
 * Initializes a new instance of the CParaBtn_VLIMIT class
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param [in,out]	btn		   	The button.
 * \param 		  	nOffset	   	The offset.
 * \param 		  	bScrollOver	True to scroll over.
 **************************************************************************************************/

CParaBtn_VLIMIT::CParaBtn_VLIMIT(BTN &btn, int nOffset,bool bScrollOver):
CParaBtn(btn,nOffset,bScrollOver)
{
	m_bOff=true;

	//m_iCountUDP=0;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CParaBtn_VLIMIT class
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

CParaBtn_VLIMIT::~CParaBtn_VLIMIT(void)
{
	
}

BEGIN_MESSAGE_MAP(CParaBtn_VLIMIT, CButton)
	//{{AFX_MSG_MAP(CParaBtn_VLIMIT)
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

void CParaBtn_VLIMIT::Draw(int nState)
{
	if(!m_bDisableText)
	{
		//if(g_hf31AcuBold )
		{
			CDC* pDC=CDC::FromHandle(m_hDC);
			HFONT hPrevFont=(HFONT)SelectObject(m_hDC,g_hf31AcuBoldNum);
			int nBkMode=SetBkMode(m_hDC,TRANSPARENT);
			int nTxtColor;
			COLORREF	crTxtInactive=0x00828282;

			switch(nState)
			{
			case BTN_DW_FOCUS:
				//if(getModel()->getDATAHANDLER()->IsCurrentModeVLimitStateOn())
					nTxtColor=m_btncr.crTxtFocus;
				//else
				//	nTxtColor=crTxtInactive;
				break;
			case BTN_DW_UP:
				//if(getModel()->getDATAHANDLER()->IsCurrentModeVLimitStateOn())
					nTxtColor=m_btncr.crTxtUp;
				//else
				//	nTxtColor=crTxtInactive;
				break;
			case BTN_DW_DW:
				nTxtColor=m_btncr.crTxtDown;
				//nTxtColorSubBtn=m_btncr.crSubTxtDown;
				break;
			case BTN_DW_DIS:
			default:
				nTxtColor=m_btncr.crTxtDisabled;
				//nTxtColorSubBtn=m_btncr.crTxtDisabled;
				break;
			}
			int nPrevTxtColor=SetTextColor(m_hDC,nTxtColor);

			RECT rc;
			memcpy(&rc,&m_rcClient,sizeof(RECT));

			rc.top = 5;

			if(m_btn.dwFormat&DT_RIGHT)
				rc.right-=m_nOffset;
			else if(!(m_btn.dwFormat&DT_CENTER)) // left
				rc.left+=m_nOffset;

			TCHAR psz[MAX_PATH];

			if(_tcslen(m_pszValueText))
			{
				if(m_bTextOnly)
					wsprintf(psz,_T("%s"),m_pszValueText);
				else
				{
					if(m_v.iValue!=0)
						wsprintf(psz,_T("%0.1f %s"), CTlsFloat::Round(((double)m_v.iValue)/10, 1),m_pszValueText);
					else
						wsprintf(psz,_T("%d %s"), 0,m_pszValueText);
				}
			}
			else
			{
				if(m_v.iValue>=100)
					wsprintf(psz,_T("%0.0f"),CTlsFloat::Round(((double)m_v.iValue)/10, 0));
				else
				{
					if(m_v.iValue!=0)
						wsprintf(psz,_T("%0.1f"),CTlsFloat::Round(((double)m_v.iValue)/10, 1));
					else
						wsprintf(psz,_T("%d"),0);
				}
			}

			DrawText(m_hDC,psz,-1,&rc,m_btn.dwFormat);

			CRect rcName;


			SelectObject(m_hDC,g_hf14AcuBold);

			CSize TextSize1 = pDC->GetTextExtent(m_pszNameText);

			SelectObject(m_hDC,g_hf8AcuBold);

			CSize TextSize2 = pDC->GetTextExtent(m_pszNameNoteText);

			SelectObject(m_hDC,g_hf14AcuBold);

			int iDiff=(m_rcClient.right-TextSize1.cx-TextSize2.cx)/2;

			rcName.top = m_rcClient.top;
			rcName.bottom = 30;
			rcName.left = m_rcClient.left;
			rcName.right = rcName.left+iDiff+TextSize1.cx;

			DrawText(m_hDC,m_pszNameText,-1,&rcName,DT_VCENTER|DT_SINGLELINE|DT_RIGHT);

			SelectObject(m_hDC,g_hf8AcuBold);
			rcName.bottom = 35;
			rcName.left = rcName.right;
			rcName.right = m_rcClient.right;
			DrawText(m_hDC,m_pszNameNoteText,-1,&rcName,DT_VCENTER|DT_SINGLELINE|DT_LEFT);

			RECT rcUnit;
			rcUnit.top = m_rcClient.top;
			rcUnit.bottom = m_rcClient.bottom-5;
			rcUnit.left = m_rcClient.left;
			rcUnit.right = m_rcClient.right;

			SelectObject(m_hDC,g_hf7AcuNorm);
			//SetTextColor(m_hDC,RGB(0,0,0));

			if(m_bEndOfRange)
				DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_ENDRANGE),-1,&rcUnit,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
			else
			{
				DrawText(m_hDC,m_pszUnitText,-1,&rcUnit,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);

				if(m_bDrawWarning && m_bHasFocus /*&& !m_bAutoState*/)
				{
					if(m_pcWarning_Fc)
						m_pcWarning_Fc->Draw(m_hDC,60,65);
				}
				else if(m_bDrawWarning && m_bDepressed /*&& !m_bAutoState*/)
				{
					if(m_pcWarning_Dw)
						m_pcWarning_Dw->Draw(m_hDC,60,65);
				}
				else if(m_bDrawWarning && m_bSignaled /*&& !m_bAutoState*/)
				{
					if(m_pcWarning_Red)
						m_pcWarning_Red->Draw(m_hDC,60,65);
				}
				else if(m_bDrawWarning /*&& !m_bAutoState*/)
				{
					if(		getModel()->getDATAHANDLER()->IsCurrentModeVLimitStateOn()==true 
						&&	getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false)
					{
						if(m_pcWarning_Up)
							m_pcWarning_Up->Draw(m_hDC,60,65);
					}
					else
					{
						if(m_pcWarning_Dis)
							m_pcWarning_Dis->Draw(m_hDC,60,65);
					}

				}

				if(m_bDrawKey && m_bDepressed /*&& !m_bAutoState*/)
				{
					if(m_pcKey)
						m_pcKey->Draw(m_hDC,10,65);
				}
			}

			m_bEndOfRange=false;

			SetTextColor(m_hDC,nPrevTxtColor);
			SetBkMode(m_hDC,nBkMode);
			SelectObject(m_hDC,hPrevFont);
		}
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

void CParaBtn_VLIMIT::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if( m_bLMouseButtonDown )
	{
		if(nChar==m_kDown)	
		{
			if(m_bDepressed)
			{
				if(m_v.iValue>m_v.iLowerLimit)
				{
					int dTemp = m_v.iValue;

					if(dTemp<=30)
					{
						dTemp=dTemp-1;
					}
					else if(dTemp<=50)
					{
						dTemp=dTemp-2;
					}
					else if(dTemp<=100)
					{
						dTemp=dTemp-5;
					}
					else if(dTemp<=200)
					{
						dTemp=dTemp-10;
					}
					else if(dTemp<=500)
					{
						dTemp=dTemp-20;
					}
					else if(dTemp<=1000)
					{
						dTemp=dTemp-50;
					}
					else if(dTemp<=2000)
					{
						dTemp=dTemp-100;
					}
					else if(dTemp<=5000)
					{
						dTemp=dTemp-200;
					}
					else
					{
						dTemp=dTemp-500;
					}

					if(dTemp<m_v.iLowerLimit)
					{
						dTemp=m_v.iLowerLimit;
						m_bEndOfRange = true;
					}

					m_v.iValue=dTemp;

					if(dTemp<=getModel()->getDATAHANDLER()->GetCurrentVlimitMaxKey())
					{
						m_bKeyValueAccepted=false;
						/*if(m_bDrawKey)
						{
							if(GetParent())
								GetParent()->PostMessage(WM_VLIMIT_CLEARKEY);
						}*/
						m_bDrawKey=false;
						m_bDrawWarning=false;
					}

					m_bWaitConfirm=false;
					m_bKeyBeep=TRUE;

				}
				else if(m_bScrollOver)
					m_v.iValue=m_v.iUpperLimit;
				else
				{
					m_bEndOfRange = true;
				}
				
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

					if(dTemp>=5000)
					{
						dTemp=dTemp+500;
					}
					else if(dTemp>=2000)
					{
						dTemp=dTemp+200;
					}
					else if(dTemp>=1000)
					{
						dTemp=dTemp+100;
					}
					else if(dTemp>=500)
					{
						dTemp=dTemp+50;
					}
					else if(dTemp>=200)
					{
						dTemp=dTemp+20;
					}
					else if(dTemp>=100)
					{
						dTemp=dTemp+10;
					}
					else if(dTemp>=50)
					{
						dTemp=dTemp+5;
					}
					else if(dTemp>=30)
					{
						dTemp=dTemp+2;
					}
					else if(dTemp>=10)
					{
						dTemp=dTemp+1;
					}
					else 
					{
						dTemp=dTemp+10;
					}

					if(dTemp>m_v.iUpperLimit)
					{
						dTemp=m_v.iUpperLimit;
						m_bEndOfRange = true;
					}
					else if(dTemp<m_v.iLowerLimit)
					{
						dTemp=m_v.iLowerLimit;
					}


					if(dTemp>getModel()->getDATAHANDLER()->GetCurrentVlimitMaxKey())
					{
						if(m_bKeyValueAccepted==false)
						{
							m_bWaitConfirm=true;
							m_bDrawKey=true;
							if(GetParent())
								GetParent()->PostMessage(WM_VLIMIT_SETKEY,m_bKeyBeep);
							m_bKeyBeep=FALSE;
						}
						else
						{
							m_v.iValue=dTemp;
						}
						m_bDrawWarning=true;
					}
					else
					{
						m_v.iValue=dTemp;
					}
				}
				else if(m_bScrollOver)
					m_v.iValue=m_v.iLowerLimit;
				else
				{
					m_bEndOfRange = true;
				}
				
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
		CParaBtn::OnKeyUp(nChar,nRepCnt,nFlags);
	}
	m_bLMouseButtonDown = false;
}
