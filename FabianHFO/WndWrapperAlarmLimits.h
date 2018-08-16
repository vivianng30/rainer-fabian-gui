#pragma once
#include "wndwrapper.h"
#include "ViewAlarmLimit.h"

class CWndWrapperAlarmLimits :
	public CWndWrapper
{
public:
	CWndWrapperAlarmLimits(void);
	virtual ~CWndWrapperAlarmLimits(void);

	//virtual abstract
	bool showView(bool bRedraw=false);
	BOOL destroyView();
	int GetDlgCtrlID();

	void SetViewFocus();
	bool SetNextFocus(){return false;}
	bool SetPrevFocus(){return false;}
protected:
	CViewAlarmLimit m_vAlarmLimits;
};
