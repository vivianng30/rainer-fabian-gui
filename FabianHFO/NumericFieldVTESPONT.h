#pragma once
#include "NumericField.h"

// CNumericFieldVTESPONT

class CNumericFieldVTESPONT : public CNumericField
{
	DECLARE_DYNAMIC(CNumericFieldVTESPONT)

public:
	CNumericFieldVTESPONT(eNumericSize size);
	virtual ~CNumericFieldVTESPONT();

	bool drawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter);
private:

	//bool drawFrames(CDC* pDC);
	bool drawStaticText(CDC* pDC);
	bool drawLimits(CDC* pDC);

protected:
	//{{AFX_MSG(CNumericFieldVTESPONT)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


