/**********************************************************************************************//**
 * \file	ParaBtn_IFLOW.cpp.
 *
 * Implements the para button iflow class
 **************************************************************************************************/

#include "StdAfx.h"
#include "ParaBtn_IFLOW.h"
#include "globDefs.h"

extern HFONT g_hf7AcuNorm;
extern HFONT g_hf14AcuBold;
extern HFONT g_hf31AcuBold;

/**********************************************************************************************//**
 * Initializes a new instance of the CParaBtn_IFLOW class
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	btn		   	The button.
 * \param	nOffset	   	The offset.
 * \param	bScrollOver	True to scroll over.
 **************************************************************************************************/

CParaBtn_IFLOW::CParaBtn_IFLOW(BTN btn, int nOffset,bool bScrollOver):
CParaBtn(btn,nOffset,bScrollOver)
{
	
}

/**********************************************************************************************//**
 * Finalizes an instance of the CParaBtn_IFLOW class
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

CParaBtn_IFLOW::~CParaBtn_IFLOW(void)
{

}

BEGIN_MESSAGE_MAP(CParaBtn_IFLOW, CButton)
	//{{AFX_MSG_MAP(CParaBtn_IFLOW)
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
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	nState	The state.
 **************************************************************************************************/

void CParaBtn_IFLOW::Draw(int nState)
{
	if(!m_bDisableText)
	{
		//if(g_hf31AcuBold )
		{
			HFONT hPrevFont=(HFONT)SelectObject(m_hDC,g_hf31AcuBoldNum);
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

			
			if(m_v.iValue>=10000)
			{
				wsprintf(psz,_T("%0.0f"), CTlsFloat::Round(((double)m_v.iValue)/1000, 0));
			}
			else
			{
				if(m_v.iValue!=0)
					wsprintf(psz,_T("%0.1f"),CTlsFloat::Round(((double)m_v.iValue)/1000, 1));
				else
					wsprintf(psz,_T("%d"),0);
			}
				

			//test
			//int iT = m_btn.wID;
			DrawText(m_hDC,psz,-1,&rc,m_btn.dwFormat);

			//RECT rcName;
			CRect rcName;


			rcName.top = m_rcClient.top;
			rcName.bottom = 30;
			rcName.left = m_rcClient.left;
			rcName.right = m_rcClient.right;

			SelectObject(m_hDC,g_hf14AcuBold);

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
			else
			{
				DrawText(m_hDC,m_pszUnitText,-1,&rcUnit,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);

				if(m_bDrawWarning && m_bHasFocus)
				{
					if(m_pcWarning_Fc)
						m_pcWarning_Fc->Draw(m_hDC,65,65);
				}
				else if(m_bDrawWarning && m_bDepressed)
				{
					if(m_pcWarning_Dw)
						m_pcWarning_Dw->Draw(m_hDC,65,65);
				}
				else if(m_bDrawWarning && m_bSignaled)
				{
					if(m_pcWarning_Red)
						m_pcWarning_Red->Draw(m_hDC,65,65);
				}
				else if(m_bDrawWarning)
				{
					if(m_pcWarning_Up)
						m_pcWarning_Up->Draw(m_hDC,65,65);
				}

				if(m_bDrawKey && m_bDepressed)
				{
					if(m_pcKey)
						m_pcKey->Draw(m_hDC,10,65);
				}
				else if(m_bDrawAlarmArrowUp && m_bHasFocus)
				{
					pcArrow=m_pcAlarmArrowUp_FC;

					if(pcArrow)
						pcArrow->Draw(m_hDC,10,65);
				}
				else if(m_bDrawAlarmArrowUp && m_bDepressed)
				{
					pcArrow=m_pcAlarmArrowUp_DW;

					if(pcArrow)
						pcArrow->Draw(m_hDC,10,65);
				}
				else if(m_bDrawAlarmArrowUp  && !m_bHasFocus && !m_bDepressed /*&& !m_bAutoState*/)
				{
					if(m_bSignaled)
						pcArrow=m_pcArrowUp_RED;
					else
						pcArrow=m_pcAlarmArrowUp_UP;

					if(pcArrow)
						pcArrow->Draw(m_hDC,10,65);
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
					pcArrow=m_pcArrowUp_UP;

					if(pcArrow)
						pcArrow->Draw(m_hDC,10,65);
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
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	nChar  	The character.
 * \param	nRepCnt	Number of reps.
 * \param	nFlags 	The flags.
 **************************************************************************************************/

void CParaBtn_IFLOW::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
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

					if(dTemp<=5000)
					{
						if(!isSafeTickCountDelayExpired(m_dwSpeedDelta, FASTENCODE))
							dTemp=(dTemp/500)*500-500;
						else
							dTemp=dTemp-100;
					}
					else if(dTemp<=10000)
					{
						if(!isSafeTickCountDelayExpired(m_dwSpeedDelta, FASTENCODE))
							dTemp=(dTemp/1000)*1000-1000;
						else
							dTemp=dTemp-500;
					}
					else
					{
						if(!isSafeTickCountDelayExpired(m_dwSpeedDelta, FASTENCODE))
							dTemp=(dTemp/2000)*2000-2000;
						else
							dTemp=dTemp-1000;
					}
					if(dTemp<m_v.iLowerLimit)
					{
						dTemp=m_v.iLowerLimit;
						m_bEndOfRange = true;
					}

					m_v.iValue=dTemp;

					m_dwSpeedDelta=GetTickCount();

					if(getModel()->getCONFIG()->GetVentRange()==NEONATAL && dTemp<=getModel()->getDATAHANDLER()->GetCurrentIFlowMaxKey())
					{
						m_bKeyValueAccepted=false;
						/*if(m_bDrawKey)
						{
							if(GetParent())
								GetParent()->PostMessage(WM_PINSP_CLEARKEY);
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

					if(dTemp>=10000)
					{
						if(!isSafeTickCountDelayExpired(m_dwSpeedDelta, FASTENCODE))
							dTemp=(dTemp/2000)*2000+2000;
						else
							dTemp=dTemp+1000;
					}
					else if(dTemp>=5000)
					{
						if(!isSafeTickCountDelayExpired(m_dwSpeedDelta, FASTENCODE))
							dTemp=(dTemp/1000)*1000+1000;
						else
							dTemp=dTemp+500;
					}
					else
					{
						if(!isSafeTickCountDelayExpired(m_dwSpeedDelta, FASTENCODE))
							dTemp=(dTemp/500)*500+500;
						else
							dTemp=dTemp+100;
					}

					if(dTemp>m_v.iUpperLimit)
					{
						dTemp=m_v.iUpperLimit;
						m_bEndOfRange = true;
					}

					if(getModel()->getCONFIG()->GetVentRange()==NEONATAL && dTemp>getModel()->getDATAHANDLER()->GetCurrentIFlowMaxKey())
					{
						if(m_bKeyValueAccepted==false)
						{
							m_bWaitConfirm=true;
							m_bDrawKey=true;
							if(GetParent())
								GetParent()->PostMessage(WM_IFLOW_SETKEY, m_bKeyBeep);
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

					//m_v.iValue=dTemp;
					m_dwSpeedDelta=GetTickCount();
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
