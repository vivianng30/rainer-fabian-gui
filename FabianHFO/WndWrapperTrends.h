#pragma once
#include "wndwrapper.h"
#include "ViewTrend.h"

class CWndWrapperTrends :
	public CWndWrapper
{
public:
	CWndWrapperTrends(void);
	virtual ~CWndWrapperTrends(void);

	//virtual abstract
	bool showView(bool bRedraw=false);
	BOOL destroyView();
	int GetDlgCtrlID();

protected:
	CViewTrend m_vTrend;
};
