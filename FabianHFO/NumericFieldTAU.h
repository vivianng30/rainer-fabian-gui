#pragma once
#include "NumericField.h"

// CNumericFieldTAU

class CNumericFieldTAU : public CNumericField
{
	DECLARE_DYNAMIC(CNumericFieldTAU)

public:
	CNumericFieldTAU(eNumericSize size);
	virtual ~CNumericFieldTAU();

	bool drawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter);
private:

	//bool drawFrames(CDC* pDC);
	bool drawStaticText(CDC* pDC);
	bool drawLimits(CDC* pDC);

private:
	//CBmp* m_pcTauSize1;
	//CBmp* m_pcTauSize2;

protected:
	//{{AFX_MSG(CNumericFieldTAU)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


