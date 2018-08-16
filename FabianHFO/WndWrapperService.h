#pragma once
#include "wndwrapper.h"
#include "ViewService.h"

class CWndWrapperService :
	public CWndWrapper
{
public:
	CWndWrapperService(void);
	virtual ~CWndWrapperService(void);

	//virtual abstract
	bool showView(bool bRedraw=false);
	BOOL destroyView();
	int GetDlgCtrlID();

protected:
	CViewService m_vService;
};
