/**********************************************************************************************//**
 * \file	NumericFieldHFOAMP.h.
 *
 * Declares the numeric field hfoamp class
 **************************************************************************************************/

#pragma once
#include "NumericField.h"

// CNumericFieldHFOAMP

class CNumericFieldHFOAMP : public CNumericField
{
	DECLARE_DYNAMIC(CNumericFieldHFOAMP)

public:
	CNumericFieldHFOAMP(eNumericSize size);
	virtual ~CNumericFieldHFOAMP();

	bool drawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter);
private:

	//bool drawFrames(CDC* pDC);
	bool drawStaticText(CDC* pDC);
	bool drawLimits(CDC* pDC);

protected:
	//{{AFX_MSG(CNumericFieldHFOAMP)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



