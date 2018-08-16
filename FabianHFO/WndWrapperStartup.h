#pragma once
#include "wndwrapper.h"
#include "ViewStartup.h"

class CWndWrapperStartup :
	public CWndWrapper
{
public:
	CWndWrapperStartup(void);
	virtual ~CWndWrapperStartup(void);

	//virtual abstract
	bool showView(bool bRedraw=false);
	BOOL destroyView();
	int GetDlgCtrlID();

protected:
	CViewStartup m_vStartup;
};
