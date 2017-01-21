#pragma once
#include "NumericField.h"

// CNumericFieldVTEMAND

class CNumericFieldVTEMAND : public CNumericField
{
	DECLARE_DYNAMIC(CNumericFieldVTEMAND)

public:
	CNumericFieldVTEMAND(eNumericSize size);
	virtual ~CNumericFieldVTEMAND();

	bool drawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter);
private:

	//bool drawFrames(CDC* pDC);
	bool drawStaticText(CDC* pDC);
	bool drawLimits(CDC* pDC);

protected:
	//{{AFX_MSG(CNumericFieldVTEMAND)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



