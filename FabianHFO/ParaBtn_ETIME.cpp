// ParaBtn_ETIME.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "ParaBtn_ETIME.h"
#include "StdAfx.h"
#include "globDefs.h"
#include "ViewParaBtn.h"

//global font objects
extern HFONT g_hf7AcuNorm;
extern HFONT g_hf8AcuBold;
extern HFONT g_hf14AcuBold;
extern HFONT g_hf31AcuBold;

CViewParaBtn* CParaBtn_ETIME::VIEWPARAPTN=0;

/**********************************************************************************************//**
 * Initializes a new instance of the CParaBtn_ETIME class
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param [in,out]	parent	   	If non-null, the parent.
 * \param 		  	btn		   	The button.
 * \param 		  	nOffset	   	The offset.
 * \param 		  	bScrollOver	True to scroll over.
 **************************************************************************************************/

CParaBtn_ETIME::CParaBtn_ETIME(CViewParaBtn* parent, BTN btn, int nOffset,bool bScrollOver):
CParaBtn(btn,nOffset,bScrollOver)
{
	VIEWPARAPTN=parent;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CParaBtn_ETIME class
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

CParaBtn_ETIME::~CParaBtn_ETIME(void)
{

}

BEGIN_MESSAGE_MAP(CParaBtn_ETIME, CButton)
	//{{AFX_MSG_MAP(CParaBtn_ETIME)
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

void CParaBtn_ETIME::Draw(int nState)
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
			//rc.bottom = 94;

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

			if(		getModel()->getCONFIG()->GetCurMode()==VM_DUOPAP
				||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_DUOPAP)
			{
				if(VIEWPARAPTN->getITimeDisplayedValue()>600)
				{
					CDC* pDC=CDC::FromHandle(m_hDC);
					//CRect rcName;
					SelectObject(m_hDC,g_hf14AcuBold);
					CSize TextSize1 = pDC->GetTextExtent(_T("T"));

					SelectObject(m_hDC,g_hf8AcuBold);
					//CSize TextSize2 = pDC->GetTextExtent(getModel()->GetLanguageString(IDS_PARA_ITIMEHIGH));
					CSize TextSize2 = pDC->GetTextExtent(_T("low"));

					SelectObject(m_hDC,g_hf14AcuBold);
					int iDiff=(m_rcClient.right-TextSize1.cx-TextSize2.cx)/2;
					rcName.top = m_rcClient.top;
					rcName.bottom = 30;
					rcName.left = m_rcClient.left;
					rcName.right = rcName.left+iDiff+TextSize1.cx;
					DrawText(m_hDC,_T("T"),-1,&rcName,DT_VCENTER|DT_SINGLELINE|DT_RIGHT);

					SelectObject(m_hDC,g_hf8AcuBold);
					rcName.bottom = 35;
					rcName.left = rcName.right;
					rcName.right = m_rcClient.right;
					//DrawText(m_hDC,getModel()->GetLanguageString(IDS_PARA_ITIMEHIGH),-1,&rcName,DT_VCENTER|DT_SINGLELINE|DT_LEFT);
					DrawText(m_hDC,_T("low"),-1,&rcName,DT_VCENTER|DT_SINGLELINE|DT_LEFT);
				}
				else
				{
					DrawText(m_hDC,m_pszNameText,-1,&rcName,m_btn.dwFormat);
				}
			}
			else
			{
				DrawText(m_hDC,m_pszNameText,-1,&rcName,m_btn.dwFormat);
			}


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
					pcArrow=m_pcArrowDw_UP;

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
			m_bLimited=false;


			/*if(m_bDrawArrowDown)
			{
				pcArrow=m_pcArrow_Dw;

				if(pcArrow)
					pcArrow->Draw(m_hDC,10,65);
			}
			else if(m_bDrawArrowUp)
			{
				pcArrow=m_pcArrow_Up;

				if(pcArrow)
					pcArrow->Draw(m_hDC,10,65);
			}*/

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

void CParaBtn_ETIME::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	//CParaBtn::OnKeyUp(nChar,nRepCnt,nFlags);
	bool bLimitsOk=true;

	if( m_bLMouseButtonDown )
	{

		if(nChar==m_kDown)	
		{
			if(m_bDepressed)
			{
				if(m_v.iValue>m_v.iLowerLimit)
				{
					int dTemp = m_v.iValue;
					int dSaveVal = m_v.iValue;

					if(m_bDrawWarning==false && m_bKeyValueAccepted==false)
					{
						getModel()->getDATAHANDLER()->SetSaveIERelationValue_ITIME(getModel()->getDATAHANDLER()->GetCurrentITimePara());
						getModel()->getDATAHANDLER()->SetSaveIERelationValue_ETIME(dSaveVal);
					}
						

					if(dTemp<=1000)
					{
						dTemp=dTemp-10;
					}
					else if(dTemp<=2000)
					{
						dTemp=dTemp-20;
					}
					else if(dTemp<=5000)
					{
						dTemp=dTemp-50;
					}
					else if(dTemp<=10000)
					{
						dTemp=dTemp-100;
					}
					else if(dTemp<=20000)
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

					bLimitsOk=getModel()->getDATAHANDLER()->CalculateBPM_ETimeChanged(dTemp, true);


					if(bLimitsOk)
					{
						getModel()->getDATAHANDLER()->SetCurrentTempETime(dTemp);

						int iIE = getModel()->getDATAHANDLER()->GetCurrentIERelation100();
						if(iIE<100)
						{
							if(m_bKeyValueAccepted==false)
							{
								getModel()->getDATAHANDLER()->SetCurrentTempETime(getModel()->getDATAHANDLER()->GetSaveIERelationValue_ETIME());
								getModel()->getDATAHANDLER()->SetCurrentTempITime(getModel()->getDATAHANDLER()->GetSaveIERelationValue_ITIME());
								m_bWaitConfirm=true;
								m_bDrawKey=true;
								if(GetParent())
									GetParent()->PostMessage(WM_ETIME_SETKEY,m_bKeyBeep);
								m_bKeyBeep=FALSE;
							}
							else
							{
								m_v.iValue=dTemp;
								if(GetParent())
									GetParent()->PostMessage(WM_ETIME_TMPCHANGED);
							}
							m_bDrawWarning=true;
						}
						else
						{
							m_v.iValue=dTemp;
							if(GetParent())
								GetParent()->PostMessage(WM_ETIME_TMPCHANGED);

							if(m_bDrawWarning)
							{
								if(GetParent())
									GetParent()->PostMessage(WM_ETIME_CLEAR_WARNING);
							}

							if(m_bKeyValueAccepted)
							{
								if(GetParent())
									GetParent()->PostMessage(WM_IE_VALUE_NOTCONFIRMED);
							}
							m_bKeyValueAccepted=false;
							/*if(m_bDrawKey)
							{
								if(GetParent())
									GetParent()->PostMessage(WM_ETIME_CLEARKEY);
							}*/
							m_bDrawKey=false;
							m_bDrawWarning=false;

							m_bWaitConfirm=false;
							m_bKeyBeep=TRUE;
						}
					}
					else
					{
						m_bLimited=true;
						if(GetParent())
							GetParent()->PostMessage(WM_ETIME_LIMITED);
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
					int dSaveVal = m_v.iValue;

					if(m_bDrawWarning==false && m_bKeyValueAccepted==false)
					{
						getModel()->getDATAHANDLER()->SetSaveIERelationValue_ITIME(getModel()->getDATAHANDLER()->GetCurrentITimePara());
						getModel()->getDATAHANDLER()->SetSaveIERelationValue_ETIME(dSaveVal);
					}

					if(dTemp>=20000)
					{
						dTemp=dTemp+500;

						
					}
					else if(dTemp>=10000)
					{
						dTemp=dTemp+200;
					}
					else if(dTemp>=5000)
					{
						dTemp=dTemp+100;
					}
					else if(dTemp>=2000)
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

					if(dTemp>m_v.iUpperLimit)
					{
						dTemp=m_v.iUpperLimit;
						m_bEndOfRange = true;
					}

					bLimitsOk=getModel()->getDATAHANDLER()->CalculateBPM_ETimeChanged(dTemp, true);


					if(bLimitsOk)
					{
						getModel()->getDATAHANDLER()->SetCurrentTempETime(dTemp);
						
						int iIE = getModel()->getDATAHANDLER()->GetCurrentIERelation100();
						if(iIE<100)
						{
							if(m_bKeyValueAccepted==false)
							{
								getModel()->getDATAHANDLER()->SetCurrentTempETime(getModel()->getDATAHANDLER()->GetSaveIERelationValue_ETIME());
								getModel()->getDATAHANDLER()->SetCurrentTempITime(getModel()->getDATAHANDLER()->GetSaveIERelationValue_ITIME());
								m_bWaitConfirm=true;
								m_bDrawKey=true;
								if(GetParent())
									GetParent()->PostMessage(WM_ETIME_SETKEY, m_bKeyBeep);
								m_bKeyBeep=FALSE;
							}
							else
							{
								m_v.iValue=dTemp;
								if(GetParent())
									GetParent()->PostMessage(WM_ETIME_TMPCHANGED);
							}
							m_bDrawWarning=true;
						}
						else
						{
							m_v.iValue=dTemp;
							if(GetParent())
								GetParent()->PostMessage(WM_ETIME_TMPCHANGED);

							m_bKeyValueAccepted=false;
							/*if(m_bDrawKey)
							{
								if(GetParent())
									GetParent()->PostMessage(WM_ETIME_CLEARKEY);
							}*/
							m_bDrawKey=false;
							m_bDrawWarning=false;

							m_bWaitConfirm=false;
							m_bKeyBeep=TRUE;
						}
					}
					else
					{
						m_bLimited=true;
						if(GetParent())
							GetParent()->PostMessage(WM_ETIME_LIMITED);
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
