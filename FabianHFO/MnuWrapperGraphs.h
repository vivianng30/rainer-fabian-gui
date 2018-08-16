#pragma once
#include "mnuwrapper.h"
#include "WndMenuGraphs.h"

class CMnuWrapperGraphs :
	public CMnuWrapper
{
public:
	CMnuWrapperGraphs(void);
	virtual ~CMnuWrapperGraphs(void);

	//virtual funcs
	BOOL createMenu(CWnd* pParentWnd=NULL, CMVView *parentView=NULL);
	BOOL destroyView();
	int GetDlgCtrlID();
	void Show(BOOL bShow);

	void FlowSensorStateChanged();

protected:
	CWndMenuGraphs m_menuGraphs;
};
