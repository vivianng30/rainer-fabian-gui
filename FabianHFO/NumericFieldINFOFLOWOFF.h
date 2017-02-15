#pragma once
#include "NumericField.h"

// CNumericFieldINFOFLOWOFF

class CNumericFieldINFOFLOWOFF : public CNumericField
{
	DECLARE_DYNAMIC(CNumericFieldINFOFLOWOFF)

public:
	CNumericFieldINFOFLOWOFF(eNumericSize size);
	virtual ~CNumericFieldINFOFLOWOFF();

	bool drawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter);
private:

	bool drawFrames(CDC* pDC);
	bool drawStaticText(CDC* pDC);
	bool drawLimits(CDC* pDC);

protected:
	//{{AFX_MSG(CNumericFieldINFOFLOWOFF)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};