#pragma once
#include "wndwrapper.h"
#include "ViewDiagramm.h"

class CWndWrapperDiagramms :
	public CWndWrapper
{
public:
	CWndWrapperDiagramms(void);
	virtual ~CWndWrapperDiagramms(void);

	//virtual abstract
	bool showView(bool bRedraw=false);
	BOOL destroyView();
	int GetDlgCtrlID();

	void StopDiagramm(bool bStopFreeze=false);

protected:
	CViewDiagramm m_vDiagramm;
};
