#pragma once
#include "SelectSetupBtn.h"

// CSelectBtn_BODYWEIGHT

class CSelectBtn_BODYWEIGHT : public CSelectSetupBtn
{
	DECLARE_DYNAMIC(CSelectBtn_BODYWEIGHT)

public:
	CSelectBtn_BODYWEIGHT(BTN &btn,COLORREF cr=0x00000000, bool bValue = false);
	virtual ~CSelectBtn_BODYWEIGHT();

	void Draw(int nState/*, TCHAR* psz=NULL*/);
protected:
	DECLARE_MESSAGE_MAP()
};


