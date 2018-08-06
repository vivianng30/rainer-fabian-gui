/**********************************************************************************************//**
 * \file	NumericFieldHFOFREQ.h.
 *
 * Declares the numeric field hfofreq class
 **************************************************************************************************/

#pragma once
#include "NumericField.h"

// CNumericFieldHFOFREQ

class CNumericFieldHFOFREQ : public CNumericField
{
	DECLARE_DYNAMIC(CNumericFieldHFOFREQ)

public:
	CNumericFieldHFOFREQ(eNumericSize size);
	virtual ~CNumericFieldHFOFREQ();

	bool drawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter);
private:

	//bool drawFrames(CDC* pDC);
	bool drawStaticText(CDC* pDC);
	bool drawLimits(CDC* pDC);

protected:
	//{{AFX_MSG(CNumericFieldHFOFREQ)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


