#pragma once
#include "stdafx.h"
#include "MVView.h"

class CMnuWrapper
{
public:
	CMnuWrapper(void);
	virtual ~CMnuWrapper(void);

	virtual BOOL createMenu(CWnd* pParentWnd=NULL, CMVView *parentView=NULL)=0;
	virtual BOOL destroyView()=0;
	virtual int GetDlgCtrlID()=0;
	virtual void Show(BOOL bShow)=0;

	virtual void PostMessage(UINT Msg,WPARAM wParam=0,LPARAM lParam=0);

protected:
	CWnd* pMenu;
};
