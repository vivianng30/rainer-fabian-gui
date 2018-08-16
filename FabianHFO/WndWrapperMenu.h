#pragma once
#include "wndwrapper.h"
#include "ViewMenu.h"

class CWndWrapperMenu :
	public CWndWrapper
{
public:
	CWndWrapperMenu(void);
	virtual ~CWndWrapperMenu(void);

	//virtual abstract
	bool showView(bool bRedraw=false);
	BOOL destroyView();
	int GetDlgCtrlID();

	void SetViewFocus();
	bool SetNextFocus(){return false;}
	bool SetPrevFocus(){return false;}

protected:
	CViewMenu m_vMenu;
};
