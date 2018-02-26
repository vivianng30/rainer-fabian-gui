/**********************************************************************************************//**
 * \file	NumericFieldVTI.h.
 *
 * Declares the numeric field vti class
 **************************************************************************************************/

#pragma once
#include "NumericField.h"

// CNumericFieldVTI

class CNumericFieldVTI : public CNumericField
{
	DECLARE_DYNAMIC(CNumericFieldVTI)

public:
	CNumericFieldVTI(eNumericSize size);
	virtual ~CNumericFieldVTI();

	bool drawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter);
private:

	//bool drawFrames(CDC* pDC);
	bool drawStaticText(CDC* pDC);
	bool drawLimits(CDC* pDC);

protected:
	//{{AFX_MSG(CNumericFieldVTI)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



