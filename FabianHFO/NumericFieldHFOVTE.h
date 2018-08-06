/**********************************************************************************************//**
 * \file	NumericFieldHFOVTE.h.
 *
 * Declares the numeric field hfovte class
 **************************************************************************************************/

#pragma once
#include "NumericField.h"

// CNumericFieldHFOVTE

class CNumericFieldHFOVTE : public CNumericField
{
	DECLARE_DYNAMIC(CNumericFieldHFOVTE)

public:
	CNumericFieldHFOVTE(eNumericSize size);
	virtual ~CNumericFieldHFOVTE();

	bool drawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter);
private:

	//bool drawFrames(CDC* pDC);
	bool drawStaticText(CDC* pDC);
	bool drawLimits(CDC* pDC);

protected:
	//{{AFX_MSG(CNumericFieldHFOVTE)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


