#pragma once
#include "NumericField.h"

// CNumericFieldTRIGGER

class CNumericFieldTRIGGER : public CNumericField
{
	DECLARE_DYNAMIC(CNumericFieldTRIGGER)

public:
	CNumericFieldTRIGGER(eNumericSize size);
	virtual ~CNumericFieldTRIGGER();

	bool drawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter);
private:

	//bool drawFrames(CDC* pDC);
	bool drawStaticText(CDC* pDC);
	bool drawLimits(CDC* pDC);

protected:
	//{{AFX_MSG(CNumericFieldTRIGGER)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



