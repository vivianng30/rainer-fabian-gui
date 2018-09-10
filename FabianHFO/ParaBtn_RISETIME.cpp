// ParaBtn_RISETIME.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "ParaBtn_RISETIME.h"
#include "globDefs.h"

extern HFONT g_hf7AcuNorm;
extern HFONT g_hf9AcuBold;
extern HFONT g_hf31AcuBold;

/**********************************************************************************************//**
 * CParaBtn_RISETIME
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CParaBtn_RISETIME, CWnd)

/**********************************************************************************************//**
 * Initializes a new instance of the CParaBtn_RISETIME class
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	btn		   	The button.
 * \param	nOffset	   	The offset.
 * \param	bScrollOver	True to scroll over.
 **************************************************************************************************/

CParaBtn_RISETIME::CParaBtn_RISETIME(BTN btn, int nOffset,bool bScrollOver):
CParaBtn(btn,nOffset,bScrollOver)
{

}

/**********************************************************************************************//**
 * Finalizes an instance of the CParaBtn_RISETIME class
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

CParaBtn_RISETIME::~CParaBtn_RISETIME()
{
}


BEGIN_MESSAGE_MAP(CParaBtn_RISETIME, CButton)
	//{{AFX_MSG_MAP(CParaBtn_RISETIME)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * CParaBtn_RISETIME message handlers
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	nState	The state.
 **************************************************************************************************/

void CParaBtn_RISETIME::Draw(int nState)
{
	if(!m_bDisableText)
	{
		//if(g_hf31AcuBold )
		{
			HFONT hPrevFont=(HFONT)SelectObject(m_hDC,g_hf31AcuBold);
			int nBkMode=SetBkMode(m_hDC,TRANSPARENT);
			int nTxtColor;
			//int nTxtColorSubBtn;

			CBmp* pcArrow=NULL;

			switch(nState)
			{
			case BTN_DW_FOCUS:
				nTxtColor=m_btncr.crTxtFocus;
				//nTxtColorSubBtn=m_btncr.crTxtFocus;
				break;
			case BTN_DW_UP:
				nTxtColor=m_btncr.crTxtUp;
				//nTxtColorSubBtn=m_btncr.crTxtUp;
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


			if(m_v.iValue>=5000)
				wsprintf(psz,_T("%0.1f"), CTlsFloat::Round(((double)m_v.iValue)/1000, 1));
			else
			{
				if(m_v.iValue!=0)
					wsprintf(psz,_T("%0.2f"),CTlsFloat::Round(((double)m_v.iValue)/1000, 2));
				else
					wsprintf(psz,_T("%d"),0);
			}


			DrawText(m_hDC,psz,-1,&rc,m_btn.dwFormat);

			//RECT rcName;
			CRect rcName;


			rcName.top = m_rcClient.top;
			rcName.bottom = 30;
			rcName.left = m_rcClient.left;
			rcName.right = m_rcClient.right;

			SelectObject(m_hDC,g_hf9AcuBold);

			DrawText(m_hDC,m_pszNameText,-1,&rcName,m_btn.dwFormat);

			RECT rcUnit;
			rcUnit.top = m_rcClient.top;
			rcUnit.bottom = m_rcClient.bottom-5;
			rcUnit.left = m_rcClient.left;
			rcUnit.right = m_rcClient.right;

			SelectObject(m_hDC,g_hf7AcuNorm);
			//SetTextColor(m_hDC,RGB(0,0,0));

			if(m_bEndOfRange)
				DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_ENDRANGE),-1,&rcUnit,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
			else if(m_bLimited)
				DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_LIMITTED),-1,&rcUnit,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
			else
			{
				DrawText(m_hDC,m_pszUnitText,-1,&rcUnit,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);

				if(m_bDrawWarning && m_bHasFocus)
				{
					if(m_pcWarning_Fc)
						m_pcWarning_Fc->Draw(m_hDC,60,65);
				}
				else if(m_bDrawWarning && m_bDepressed)
				{
					if(m_pcWarning_Dw)
						m_pcWarning_Dw->Draw(m_hDC,60,65);
				}
				else if(m_bDrawWarning && m_bSignaled)
				{
					if(m_pcWarning_Red)
						m_pcWarning_Red->Draw(m_hDC,60,65);
				}
				else if(m_bDrawWarning)
				{
					if(m_pcWarning_Up)
						m_pcWarning_Up->Draw(m_hDC,60,65);
				}

				if(m_bDrawKey && m_bDepressed)
				{
					//if(m_bDepressed)
					{
						if(m_pcKey)
							m_pcKey->Draw(m_hDC,10,65);
					}
				}
				else if(m_bDrawArrowDown && m_bHasFocus)
				{
					pcArrow=m_pcArrowDw_FC;

					if(pcArrow)
						pcArrow->Draw(m_hDC,10,65);
				}
				else if(m_bDrawArrowDown && m_bDepressed)
				{
					pcArrow=m_pcArrowDw_DW;

					if(pcArrow)
						pcArrow->Draw(m_hDC,10,65);
				}
				else if(m_bDrawArrowDown  && !m_bHasFocus && !m_bDepressed)
				{
					if(m_v.iValue>m_v.iLowerLimit)
					{
						if(m_bSignaled)
							pcArrow=m_pcArrowDw_RED;
						else
							pcArrow=m_pcArrowDw_UP;

						if(pcArrow)
							pcArrow->Draw(m_hDC,10,65);
					}

				}
				else if(m_bDrawArrowUp && m_bHasFocus)
				{
					pcArrow=m_pcArrowUp_FC;

					if(pcArrow)
						pcArrow->Draw(m_hDC,10,65);
				}
				else if(m_bDrawArrowUp && m_bDepressed)
				{
					pcArrow=m_pcArrowUp_DW;

					if(pcArrow)
						pcArrow->Draw(m_hDC,10,65);
				}
				else if(m_bDrawArrowUp  && !m_bHasFocus && !m_bDepressed)
				{
					if(m_bSignaled)
						pcArrow=m_pcArrowUp_RED;
					else
						pcArrow=m_pcArrowUp_UP;

					if(pcArrow)
						pcArrow->Draw(m_hDC,10,65);
				}
			}

			m_bEndOfRange=false;
			m_bLimited=false;

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

void CParaBtn_RISETIME::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
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
					//int iITIME=0;

					if(dTemp<=1000)
					{
						dTemp=dTemp-10;
					}
					else if(dTemp<=2000)
					{
						dTemp=dTemp-20;
					}
					else 
					{
						dTemp=dTemp-50;
					}

					/*if(	getModel()->getCONFIG()->CurModeIsPresetMode()==false)
					{
						if(getModel()->getCONFIG()->GetCurMode()==VM_IPPV)
						{
							iITIME= getModel()->getDATAHANDLER()->PARADATA()->GetITimePara_IPPV();
						}
						else
						{
							iITIME= getModel()->getDATAHANDLER()->PARADATA()->GetITimePara_TRIGGER();
						}
					}
					else
					{
						if(getModel()->getCONFIG()->GetCurMode()==VM_PRE_IPPV)
						{
							iITIME= getModel()->getDATAHANDLER()->PRESET()->GetITimePara_IPPV();
						}
						else
						{
							iITIME= getModel()->getDATAHANDLER()->PRESET()->GetITimePara_TRIGGER();
						}
					}*/

					/*if(dTemp<iITIME)
					{
						m_bLimited=true;

						if(GetParent())
							GetParent()->PostMessage(WM_ITIME_LIMITED);
					}
					else*/
					{
						m_v.iValue=dTemp;
						if(GetParent())
							GetParent()->PostMessage(WM_RISETIME_CHANGED);
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
					int iITIME=0;
					
					if(dTemp>=2000)
					{
						dTemp=dTemp+50;

						
					}
					else if(dTemp>=1000)
					{
						dTemp=dTemp+20;

						
					}
					else
					{
						dTemp=dTemp+10;
					}

					if(	getModel()->getCONFIG()->CurModeIsPresetMode()==false)
					{
						if(getModel()->getCONFIG()->GetCurMode()==VM_IPPV)
						{
							iITIME= getModel()->getDATAHANDLER()->PARADATA()->GetITimePara_IPPV();
						}
						else
						{
							iITIME= getModel()->getDATAHANDLER()->PARADATA()->GetITimePara_TRIGGER();
						}
					}
					else
					{
						if(getModel()->getCONFIG()->GetCurMode()==VM_PRE_IPPV)
						{
							iITIME= getModel()->getDATAHANDLER()->PRESET()->GetITimePara_IPPV();
						}
						else
						{
							iITIME= getModel()->getDATAHANDLER()->PRESET()->GetITimePara_TRIGGER();
						}
					}


					if(dTemp<m_v.iLowerLimit)
					{
						dTemp=m_v.iLowerLimit;
						m_bEndOfRange = true;
						m_v.iValue=dTemp;
					}
					else if(dTemp>iITIME)
					{
						m_bLimited=true;

						if(GetParent())
							GetParent()->PostMessage(WM_RISETIME_LIMITED);
					}
					else
					{
						m_v.iValue=dTemp;
						
					}

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
		CParaBtn::OnKeyUp(nChar,nRepCnt,nFlags);
	}
	m_bLMouseButtonDown = false;
}


