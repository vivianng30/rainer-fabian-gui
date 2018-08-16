#pragma once
#include "mnuwrapper.h"
#include "WndMenuTrendtype.h"

class CMnuWrapperTrendtype :
	public CMnuWrapper
{
public:
	CMnuWrapperTrendtype(void);
	virtual ~CMnuWrapperTrendtype(void);

	//virtual funcs
	BOOL createMenu(CWnd* pParentWnd=NULL, CMVView *parentView=NULL);
	BOOL destroyView();
	int GetDlgCtrlID();
	void Show(BOOL bShow);

	void preCreateInit(UINT bSelTrend);
	void ShowTrendType(bool bShow, UINT iTrendType, UINT bSelTrend);

protected:
	CWndMenuTrendtype m_menuTrendtype;
	UINT m_iCurBtnSelTrend;
};
