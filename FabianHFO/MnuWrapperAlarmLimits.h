#pragma once
#include "mnuwrapper.h"
#include "WndMenuAlarmLimits.h"

class CMnuWrapperAlarmLimits :
	public CMnuWrapper
{
public:
	CMnuWrapperAlarmLimits(void);
	virtual ~CMnuWrapperAlarmLimits(void);

	BOOL createMenu(CWnd* pParentWnd=NULL, CMVView *parentView=NULL);
	BOOL destroyView();
	int GetDlgCtrlID();
	void Show(BOOL bShow);

protected:
	CWndMenuAlarmLimits m_menuAlarmLimits;
};
