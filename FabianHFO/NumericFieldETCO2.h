/**********************************************************************************************//**
 * \file	NumericFieldETCO2.h.
 *
 * Declares the numeric field etco 2 class
 **************************************************************************************************/

#pragma once
#include "NumericField.h"

// CNumericFieldETCO2

class CNumericFieldETCO2 : public CNumericField
{
	DECLARE_DYNAMIC(CNumericFieldETCO2)

public:
	CNumericFieldETCO2(eNumericSize size);
	virtual ~CNumericFieldETCO2();

	bool drawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter);
private:

	//bool drawFrames(CDC* pDC);
	bool drawStaticText(CDC* pDC);
	bool drawLimits(CDC* pDC);

protected:
	//{{AFX_MSG(CNumericFieldETCO2)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


