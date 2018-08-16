#pragma once
#include "wndwrapper.h"
#include "ViewSystemAlarm.h"

class CWndWrapperSystemAlarm :
	public CWndWrapper
{
public:
	CWndWrapperSystemAlarm(void);
	virtual ~CWndWrapperSystemAlarm(void);

	//virtual abstract
	bool showView(bool bRedraw=false);
	BOOL destroyView();
	int GetDlgCtrlID();

protected:
	CViewSystemAlarm m_vSystemAlarm;
};
