#pragma once
#include "mnuwrapper.h"
#include "WndMenuVentState.h"

class CMnuWrapperVentState :
	public CMnuWrapper
{
public:
	CMnuWrapperVentState(void);
	virtual ~CMnuWrapperVentState(void);

	//virtual funcs
	BOOL createMenu(CWnd* pParentWnd=NULL, CMVView *parentView=NULL);
	BOOL destroyView();
	int GetDlgCtrlID();
	void Show(BOOL bShow);

	void ResetButtonPresetstate();

protected:
	CWndMenuVentState m_menuVentState;
};
