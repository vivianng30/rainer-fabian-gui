#pragma once
#include "NumericField.h"

// CNumericFieldVTEKG

class CNumericFieldVTEKG : public CNumericField
{
	DECLARE_DYNAMIC(CNumericFieldVTEKG)

public:
	CNumericFieldVTEKG(eNumericSize size);
	virtual ~CNumericFieldVTEKG();

	bool drawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter);
private:

	//bool drawFrames(CDC* pDC);
	bool drawStaticText(CDC* pDC);
	bool drawLimits(CDC* pDC);

protected:
	//{{AFX_MSG(CNumericFieldVTEKG)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

