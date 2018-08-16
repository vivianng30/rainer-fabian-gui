#pragma once
#include "wndwrapper.h"
#include "ViewShutdown.h"

class CWndWrapperShutdown :
	public CWndWrapper
{
public:
	CWndWrapperShutdown(void);
	virtual ~CWndWrapperShutdown(void);

	//virtual abstract
	bool showView(bool bRedraw=false);
	BOOL destroyView();
	int GetDlgCtrlID();

protected:
	CViewShutdown m_vShutdown;
};
