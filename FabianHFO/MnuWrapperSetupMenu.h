#pragma once
#include "mnuwrapper.h"
#include "WndMenuMenu.h"

class CMnuWrapperSetupMenu :
	public CMnuWrapper
{
public:
	CMnuWrapperSetupMenu(void);
	virtual ~CMnuWrapperSetupMenu(void);

	//virtual funcs
	BOOL createMenu(CWnd* pParentWnd=NULL, CMVView *parentView=NULL);
	BOOL destroyView();
	int GetDlgCtrlID();
	void Show(BOOL bShow);
	
	void SetMenuText(CStringW txt, bool bRedraw=true);


protected:
	CWndMenuMenu m_menuSetupMenu;
};
