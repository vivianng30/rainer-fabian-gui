#pragma once
#include "wndwrapper.h"
#include "ViewFullscreenMsg.h"

class CWndWrapperFullscreenMsg :
	public CWndWrapper
{
public:
	CWndWrapperFullscreenMsg(void);
	virtual ~CWndWrapperFullscreenMsg(void);

	//virtual abstract
	bool showView(bool bRedraw=false);
	BOOL destroyView();
	int GetDlgCtrlID();

protected:
	CViewFullscreenMsg m_vFullscreenMsg;
};
