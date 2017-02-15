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
	//CNumericField* m_pNumField;

	/*CNumericField* m_pNumField1;
	CNumericField* m_pNumField2;
	CNumericField* m_pNumField3;
	CNumericField* m_pNumField4;
	CNumericField* m_pNumField5;
	CNumericField* m_pNumField6;
	CNumericField* m_pNumField7;
	CNumericField* m_pNumField8;*/

	/*eNumericType m_type1;
	eNumericType m_type2;
	eNumericType m_type3;
	eNumericType m_type4;
	eNumericType m_type5;
	eNumericType m_type6;
	eNumericType m_type7;
	eNumericType m_type8;

	eNumericSize m_size1;
	eNumericSize m_size2;
	eNumericSize m_size3;
	eNumericSize m_size4;
	eNumericSize m_size5;
	eNumericSize m_size6;
	eNumericSize m_size7;
	eNumericSize m_size8;*/
	
	/*CString m_szName1;
	CString m_szName2;
	CString m_szName3;
	CString m_szName4;
	CString m_szName5;
	CString m_szName6;
	CString m_szName7;
	CString m_szName8;

	CString m_szNameNote1;
	CString m_szNameNote2;
	CString m_szNameNote3;
	CString m_szNameNote4;
	CString m_szNameNote5;
	CString m_szNameNote6;
	CString m_szNameNote7;
	CString m_szNameNote8;

	CString m_szUnit1;
	CString m_szUnit2;
	CString m_szUnit3;
	CString m_szUnit4;
	CString m_szUnit5;
	CString m_szUnit6;
	CString m_szUnit7;
	CString m_szUnit8;*/
	//{{AFX_MSG(CWndNumIPPV_Mech)
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
