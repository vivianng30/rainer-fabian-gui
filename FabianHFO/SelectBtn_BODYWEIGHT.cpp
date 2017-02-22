// SelectBtn_BODYWEIGHT.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "SelectBtn_BODYWEIGHT.h"


// CSelectBtn_BODYWEIGHT

IMPLEMENT_DYNAMIC(CSelectBtn_BODYWEIGHT, CSelectSetupBtn)

CSelectBtn_BODYWEIGHT::CSelectBtn_BODYWEIGHT(BTN &btn,COLORREF cr,bool bValue):
CSelectSetupBtn(btn,cr,bValue)
{

}

CSelectBtn_BODYWEIGHT::~CSelectBtn_BODYWEIGHT()
{
}


BEGIN_MESSAGE_MAP(CSelectBtn_BODYWEIGHT, CSelectSetupBtn)
	//{{AFX_MSG_MAP(CSelectSetupBtn)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



// CSelectBtn_BODYWEIGHT message handlers


void CSelectBtn_BODYWEIGHT::Draw(int nState/*,TCHAR* psz*/)
{
	if(m_hFont)
	{
		HFONT hPrevFont=(HFONT)SelectObject(m_hDC,m_hFont);
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
			/*if(m_btn.wID==IDC_BTN_MENU_NUMERIC)
			{
			m_pViewHandler->BtnPushed(m_btn.wID,0);
			}*/
			break;
		case BTN_DW_DIS:
		default:
			nTxtColor=m_btncr.crTxtDisabled;
			break;
		}

		int nPrevTxtColor=SetTextColor(m_hDC,nTxtColor);

		RECT rc;
		memcpy(&rc,&m_rcClient,sizeof(RECT));

		/*rc.left = rc.left+20;
		rc.right = rc.right-20;*/

		if(m_btn.dwFormat&DT_RIGHT)
			rc.right-=m_nXOffset;
		else //if(!(m_btn.dwFormat&DT_CENTER)) // left
			rc.left+=m_nXOffset;

				


		if(m_bValue)
		{
			rc.left = rc.left+10;
			rc.right = rc.right-20;

			DrawText(m_hDC,m_pszText,-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_LEFT);

			SetTextColor(m_hDC,RGB(0,0,130));
			DrawText(m_hDC,m_pszValueText,-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_RIGHT);
		}
		else
		{
			DrawText(m_hDC,m_pszText,-1,&rc,m_btn.dwFormat);
		}

		

		

		if(m_nNbr!=-1)
		{
			rc.left=0;
			rc.right=m_btn.pcBmpUp->Height(); // quaudrat!
			TCHAR pszNbr[8];
			wsprintf(pszNbr,_T("%d"),m_nNbr);
			DrawText(m_hDC,pszNbr,-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_CENTER);	
		}


		if(m_bDrawOK)
		{
			CBmp* pcOK=NULL;

			switch(GetState())
			{
			case BS_UP:
				{
					pcOK=m_pcOkUp;
				}
				break;
			case BS_FOCUSED:
				{
					pcOK=m_pcOkFc;
				}
				break;
			case BS_DOWN:
				{
					pcOK=m_pcOkDw;
				}
				break;
			}

			if(pcOK)
				pcOK->Draw(m_hDC,20,2);
		}

		if(m_pbmpFlag!=NULL)
		{
			m_pbmpFlag->Draw(m_hDC,350,7);
		}

		SetTextColor(m_hDC,nPrevTxtColor);
		SetBkMode(m_hDC,nBkMode);
		SelectObject(m_hDC,hPrevFont);
	}
}