#pragma once
#include "wndwrapper.h"
#include "ViewLog.h"

class CWndWrapperAlarmLog :
	public CWndWrapper
{
public:
	CWndWrapperAlarmLog(void);
	virtual ~CWndWrapperAlarmLog(void);

	//virtual abstract
	bool showView(bool bRedraw=false);
	BOOL destroyView();
	int GetDlgCtrlID();

protected:
	CViewLog m_vLog;
};
