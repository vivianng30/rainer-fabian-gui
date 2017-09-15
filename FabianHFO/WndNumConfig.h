#pragma once
#include "WndNumeric.h"
#include "NumericField.h"

// CWndNumConfig

class CWndNumConfig : public CWndNumeric
{
	DECLARE_DYNAMIC(CWndNumConfig)

public:
	CWndNumConfig(NUMERICINI* pbufNumeric);
	virtual ~CWndNumConfig();
	bool DrawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter);
	void SetGraphCursor(bool bDraw, bool bMoveXscale, bool bMoveYscale);
	void init();

private:

	bool DrawFrames(CDC* pDC);
	bool drawCursor();
	bool DrawStaticText(CDC* pDC);
	bool DrawLimits(CDC* pDC);

	void createNumericFields(BYTE block);
	void createNumericField1();
	void createNumericField2();
	void createNumericField3();
	void createNumericField4();
	void createNumericField5();
	void createNumericField6();
	void createNumericField7();
	void createNumericField8();
	CNumericField* createNewNumericField(eNumericType numType, eNumericSize numSize);
	SHORT getBottomNumPositionYaxes(eNumericSize numSize);

protected:

	NUMERICINI* m_pbufNumeric;

	CArray<CNumericField* , CNumericField *> m_pNumFieldArray;

	BYTE m_iSizeComplete;
	SHORT m_iCurrentNumPositionYaxes;

	bool m_bExit;
	
	//{{AFX_MSG(CWndNumIPPV_Mech)
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
