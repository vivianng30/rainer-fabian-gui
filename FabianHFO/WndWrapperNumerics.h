#pragma once
#include "wndwrapper.h"
#include "ViewNumericWnd.h"

class CWndWrapperNumerics :
	public CWndWrapper
{
public:
	CWndWrapperNumerics(void);
	virtual ~CWndWrapperNumerics(void);

	//virtual abstract
	bool showView(bool bRedraw=false);
	BOOL destroyView();
	int GetDlgCtrlID();

protected:
	CViewNumericWnd m_vNumericWnd;
};
