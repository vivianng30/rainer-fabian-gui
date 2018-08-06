//=============================================================================
/** 
* \class CLimitBtn_SPO2high
* 		 
* \brief Button for limit of SpO2 high value.
* 		 
* \author Rainer Kuehner
* \date 2017-10-23 Creation of file
*
**/
//=============================================================================
#pragma once
#include "LimitBtn.h"

class CLimitBtn_SPO2high :public CLimitBtn
{
public:
	explicit CLimitBtn_SPO2high(CWndDataSPO2 *parent, BTN btn, int nOffset=0,bool bScrollOver=true);
	virtual ~CLimitBtn_SPO2high();

public:

	// Operations
public:
	virtual void Draw(int nState);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUDParaBtn)
public:
	//virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL



	// Generated message map functions
protected:
	//{{AFX_MSG(CLimitBtn_SPO2high)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};



