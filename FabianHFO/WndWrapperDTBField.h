#pragma once
#include "wndwrapper.h"
#include "ViewDTBField.h"

class CWndWrapperDTBField :
	public CWndWrapper
{
public:
	CWndWrapperDTBField(void);
	virtual ~CWndWrapperDTBField(void);

	bool showView(bool bRedraw=false);
	BOOL destroyView();
	int GetDlgCtrlID();

	void SetTimeO2Flush(int iO2FlushTime);
	void SetTimeUntilStartVent(int iCountDown);
	void SetTimeAlarmSilent(int iCountDown);
	void SetTimeUntilStopVent(int iCountDown);
	void SetTimeUntilTurnOff(int iCountDown);

protected:
	CViewDTBField m_vDTBField;
};
