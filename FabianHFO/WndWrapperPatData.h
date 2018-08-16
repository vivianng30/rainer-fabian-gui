#pragma once
#include "wndwrapper.h"
#include "ViewPatData.h"

class CWndWrapperPatData :
	public CWndWrapper
{
public:
	CWndWrapperPatData(void);
	virtual ~CWndWrapperPatData(void);

	//virtual abstract
	bool showView(bool bRedraw=false);
	BOOL destroyView();
	int GetDlgCtrlID();

protected:
	CViewPatData m_vPatData;
};
