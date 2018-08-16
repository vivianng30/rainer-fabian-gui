#pragma once
#include "mnuwrapper.h"
#include "WndTrendTimeaxis.h"

class CMnuWrapperTrendTimeaxis :
	public CMnuWrapper
{
public:
	CMnuWrapperTrendTimeaxis(void);
	virtual ~CMnuWrapperTrendTimeaxis(void);

	//virtual funcs
	BOOL createMenu(CWnd* pParentWnd=NULL, CMVView *parentView=NULL);
	BOOL destroyView();
	int GetDlgCtrlID();
	void Show(BOOL bShow);

	void SetTimeRange(COleDateTime dtStartTime, COleDateTime dtEndTime,int iOffset, int iTimeSpan);
protected:
	CWndTrendTimeaxis m_menuTimeaxis;
};
