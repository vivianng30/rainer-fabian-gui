/**********************************************************************************************//**
 * \file	NumericFieldDEMANDFLOW.h.
 *
 * Declares the numeric field demandflow class
 **************************************************************************************************/

#pragma once
#include "NumericField.h"

// CNumericFieldDEMANDFLOW

class CNumericFieldDEMANDFLOW : public CNumericField
{
	DECLARE_DYNAMIC(CNumericFieldDEMANDFLOW)

public:
	CNumericFieldDEMANDFLOW(eNumericSize size);
	virtual ~CNumericFieldDEMANDFLOW();

	bool drawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter);
private:

	//bool drawFrames(CDC* pDC);
	bool drawStaticText(CDC* pDC);
	bool drawLimits(CDC* pDC);

protected:
	//{{AFX_MSG(CNumericFieldDEMANDFLOW)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



