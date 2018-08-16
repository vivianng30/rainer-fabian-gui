#pragma once
#include "mnuwrapper.h"
#include "WndMenuTrend.h"

class CMnuWrapperTrend :
	public CMnuWrapper
{
public:
	CMnuWrapperTrend(void);
	virtual ~CMnuWrapperTrend(void);

	//virtual funcs
	BOOL createMenu(CWnd* pParentWnd=NULL, CMVView *parentView=NULL);
	BOOL destroyView();
	int GetDlgCtrlID();
	void Show(BOOL bShow);



protected:
	CWndMenuTrend m_menuTrend;
};
