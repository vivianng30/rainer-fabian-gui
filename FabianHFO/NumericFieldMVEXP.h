/**********************************************************************************************//**
 * \file	NumericFieldMVEXP.h.
 *
 * Declares the numeric field mvexp class
 **************************************************************************************************/

#pragma once
#include "NumericField.h"

// CNumericFieldMVEXP

class CNumericFieldMVEXP : public CNumericField
{
	DECLARE_DYNAMIC(CNumericFieldMVEXP)

public:
	CNumericFieldMVEXP(eNumericSize size);
	virtual ~CNumericFieldMVEXP();

	bool drawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter);
private:

	//bool drawFrames(CDC* pDC);
	bool drawStaticText(CDC* pDC);
	bool drawLimits(CDC* pDC);

protected:
	//{{AFX_MSG(CNumericFieldMVEXP)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


