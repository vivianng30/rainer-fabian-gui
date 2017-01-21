#pragma once
#include "NumericField.h"

// CNumericFieldFREQ

class CNumericFieldFREQ : public CNumericField
{
	DECLARE_DYNAMIC(CNumericFieldFREQ)

public:
	CNumericFieldFREQ(eNumericSize size);
	virtual ~CNumericFieldFREQ();

	bool drawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter);
private:

	//bool drawFrames(CDC* pDC);
	bool drawStaticText(CDC* pDC);
	bool drawLimits(CDC* pDC);

protected:
	//{{AFX_MSG(CNumericFieldFREQ)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



