/**********************************************************************************************//**
 * \file	NumericFieldSHAREMV.h.
 *
 * Declares the numeric field sharemv class
 **************************************************************************************************/

#pragma once
#include "NumericField.h"

// CNumericFieldSHAREMV

class CNumericFieldSHAREMV : public CNumericField
{
	DECLARE_DYNAMIC(CNumericFieldSHAREMV)

public:
	CNumericFieldSHAREMV(eNumericSize size);
	virtual ~CNumericFieldSHAREMV();

	bool drawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter);
private:

	//bool drawFrames(CDC* pDC);
	bool drawStaticText(CDC* pDC);
	bool drawLimits(CDC* pDC);

protected:
	//{{AFX_MSG(CNumericFieldSHAREMV)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



