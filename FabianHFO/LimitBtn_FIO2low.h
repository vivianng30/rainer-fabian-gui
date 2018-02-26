//=============================================================================
/** 
* \class CLimitBtn_FIO2low
* 		 
* \brief Button for limit of FiO2 low value.
* 		 
* \author Rainer Kuehner
* \date 2017-10-23 Creation of file
*
**/
//=============================================================================
#pragma once
#include "LimitBtn.h"

class CLimitBtn_FIO2low :public CLimitBtn
{
public:
	explicit CLimitBtn_FIO2low(CWndDataSPO2 *parent, BTN btn, int nOffset=0,bool bScrollOver=true);
	virtual ~CLimitBtn_FIO2low();

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
	//{{AFX_MSG(CLimitBtn_FIO2low)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};



