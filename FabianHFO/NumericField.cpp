// NumericField.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "NumericField.h"
#include "WuLine.h"

/**********************************************************************************************//**
 * CNumericField
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CNumericField, CWnd)

/**********************************************************************************************//**
 * Initializes a new instance of the CNumericField class
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	size	The size.
 **************************************************************************************************/

CNumericField::CNumericField(eNumericSize size)
{
	InitializeCriticalSection(&csDraw);

	m_eSize=size;

	m_pModel=NULL;

	m_hDC=NULL;
	m_hBmp=NULL;
	m_hBmpPrev=NULL;

	m_hdcStatic=NULL;
	m_hbmpStatic=NULL;

	m_pcNumPara1er=NULL;
	m_pcNumPara2er=NULL;
	m_pcNumPara3er=NULL;

	m_pcLimitHigh_Up=NULL;
	m_pcLimitLow_Up=NULL;

	m_pcNumInfoTop= NULL;
	m_pcNumInfoBot= NULL;

	m_pcTauSize1=NULL;
	//m_pcTauSize2=NULL;

	m_pcWarningYellow=NULL;

	m_lX				= 0;
	m_lY				= 0;
	m_lXo				= 0;
	m_lYo				= 0;

	m_bCursorField=false;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CNumericField class
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

CNumericField::~CNumericField()
{
	DeleteCriticalSection(&csDraw);
}


BEGIN_MESSAGE_MAP(CNumericField, CWnd)
	//{{AFX_MSG_MAP(CNumericField)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * CNumericField message handlers
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	Null if it fails, else the model.
 **************************************************************************************************/

CMVModel *CNumericField::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}

/**********************************************************************************************//**
 * Creates the Window instance that will be represented by this instance
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param [in,out]	pParentWnd	If non-null, the parent window.
 * \param 		  	rc		  	The rectangle.
 * \param 		  	nID		  	The identifier.
 * \param [in,out]	pContext  	If non-null, the context.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

BOOL CNumericField::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext*) 
{
	m_lX=rc.right-rc.left;
	m_lY=rc.bottom-rc.top;

	CStringW  lpszClassName=AfxRegisterWndClass(NULL); 
	if(CWnd::Create(lpszClassName,NULL,WS_CHILD|WS_VISIBLE,rc,pParentWnd,nID))
	{
		CClientDC dc(this);

		m_hDC=CreateCompatibleDC(dc.m_hDC);
		m_hBmp=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
		m_hBmpPrev=(HBITMAP)SelectObject(m_hDC,m_hBmp);

		CBrush cbrBack(BACKGND);
		//CBrush cbrBack(RGB(140,140,255));
		HBRUSH hbrprev=(HBRUSH)SelectObject(m_hDC,cbrBack);
		HPEN hpenprev=(HPEN)SelectObject(m_hDC, (HPEN)GetStockObject(NULL_PEN));	

		Rectangle(m_hDC, 0, 0, m_lX, m_lY);


		m_hdcStatic = CreateCompatibleDC(m_hDC);
		m_hbmpStatic = CreateCompatibleBitmap(m_hDC, m_lX, m_lY);
		SelectObject(m_hdcStatic, m_hbmpStatic);

		HPEN hpenprevStatic=(HPEN)SelectObject(m_hdcStatic, (HPEN)GetStockObject(NULL_PEN));	
		HBRUSH hbrprevStatic=(HBRUSH)SelectObject(m_hdcStatic,cbrBack); 
		Rectangle(m_hdcStatic, 0, 0, m_lX, m_lY);

		
		if(m_eSize==NUMERICSIZE_1)
			m_pcNumPara1er		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_NUMPARA_1);
		else if(m_eSize==NUMERICSIZE_2)
			m_pcNumPara2er		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_NUMPARA_2);
		else 
			m_pcNumPara3er		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_NUMPARA_3);
		
		m_pcLimitHigh_Up	= new CBmp(theApp.m_hInstance,m_hDC,	IDB_LIMIT_HIGH);
		m_pcLimitLow_Up		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_LIMIT_LOW);

		m_pcTauSize1	= new CBmp(theApp.m_hInstance,m_hDC,	IDB_NUM_TAU1);
		//m_pcTauSize2	= new CBmp(theApp.m_hInstance,m_hDC,	IDB_NUM_TAU2);

		m_pcWarningYellow = new CBmp(theApp.m_hInstance,m_hDC,IDB_PARA_WARN_SUB);

		m_pcNumInfoTop		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_NUM_INFO_TOP);
		m_pcNumInfoBot		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_NUM_INFO_BOT);

		SelectObject(m_hdcStatic, hpenprevStatic);	
		SelectObject(m_hdcStatic,hbrprevStatic);
		SelectObject(m_hDC,hpenprev);
		SelectObject(m_hDC,hbrprev);
		//cbrBack.DeleteObject();//rkuNEWFIX

		return 1;
	}
	else
		return 0;
}

/**********************************************************************************************//**
 * Sets cursor field
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	state	True to state.
 **************************************************************************************************/

void CNumericField::setCursorField(bool state)
{
	m_bCursorField=state;
}

/**********************************************************************************************//**
 * Query if this instance is cursor field
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	True if cursor field, false if not.
 **************************************************************************************************/

bool CNumericField::isCursorField()
{
	return m_bCursorField;
}

/**********************************************************************************************//**
 * Translates all messages before they are processed by the main message loop
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param [in,out]	pMsg	If non-null, the message.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

BOOL CNumericField::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message)
	{
	case WM_KEYDOWN:
		if(pMsg->wParam==VK_SPACE)
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_SETFOCUS_PIMARYVIEW);
			return 1;
		}
		break;
	default:
		{

		}
		break;
	}
	return CWnd::PreTranslateMessage(pMsg);
}

/**********************************************************************************************//**
 * Executes the destroy action
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CNumericField::OnDestroy() 
{
	if(m_pcNumInfoTop)
		delete m_pcNumInfoTop;
	m_pcNumInfoTop=NULL;

	if(m_pcNumInfoBot)
		delete m_pcNumInfoBot;
	m_pcNumInfoBot=NULL;

	if(m_pcWarningYellow)
		delete m_pcWarningYellow;
	m_pcWarningYellow=NULL;

	if(m_pcNumPara1er)
		delete m_pcNumPara1er;
	m_pcNumPara1er=NULL;

	if(m_pcNumPara2er)
		delete m_pcNumPara2er;
	m_pcNumPara2er=NULL;

	if(m_pcNumPara3er)
		delete m_pcNumPara3er;
	m_pcNumPara3er=NULL;

	if(m_pcLimitHigh_Up)
		delete m_pcLimitHigh_Up;
	m_pcLimitHigh_Up=NULL;

	if(m_pcLimitLow_Up)
		delete m_pcLimitLow_Up;
	m_pcLimitLow_Up=NULL;

	if(m_pcTauSize1)
		delete m_pcTauSize1;
	m_pcTauSize1=NULL;

	/*if(m_pcTauSize2)
		delete m_pcTauSize2;
	m_pcTauSize2=NULL;*/

	CWnd::OnDestroy();

	if(m_hBmp)
	{
		SelectObject(m_hDC,m_hBmpPrev);
		DeleteObject(m_hBmp);
	}
	if(m_hDC)
		DeleteDC(m_hDC);

	if(m_hdcStatic)
		DeleteDC(m_hdcStatic);

	if(m_hbmpStatic)
		DeleteObject(m_hbmpStatic);

}

/**********************************************************************************************//**
 * Draw frames
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param [in,out]	pDC	If non-null, the device-context.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CNumericField::drawFrames(CDC* pDC)
{
	HDC hdc = *pDC;
	HBRUSH hPrevBrush = (HBRUSH)SelectObject(hdc, (HBRUSH)GetStockObject(WHITE_BRUSH));	
	HPEN hPrevPen = (HPEN)SelectObject(hdc, (HPEN)GetStockObject(WHITE_PEN));

	/*if(m_pcNumPara3er)
		m_pcNumPara3er->Draw(hdc,0,5);*/

	//RECT rc;

	switch(m_eSize)
	{
	case NUMERICSIZE_1:
		{
			if(m_pcNumPara1er)
				m_pcNumPara1er->Draw(hdc,0,5);
		}
		break;
	case NUMERICSIZE_2:
		{
			if(m_pcNumPara2er)
				m_pcNumPara2er->Draw(hdc,0,5);
		}
		break;
	case NUMERICSIZE_3:
		{
			if(m_pcNumPara3er)
				m_pcNumPara3er->Draw(hdc,0,5);
		}
		break;
	}
	

	SelectObject(hdc, hPrevBrush);	
	SelectObject(hdc, hPrevPen);

	return true;
}

/**********************************************************************************************//**
 * Draw high limit
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param [in,out]	pDC	If non-null, the device-context.
 **************************************************************************************************/

void CNumericField::drawHighLimit(CDC* pDC)
{
	HDC hdc = *pDC;
	int nBkMode=SetBkMode(hdc,TRANSPARENT);
	CPen penLine;
	penLine.CreatePen(PS_SOLID,2,NUMERICLIMIT_COL);
	HPEN hpenprev=(HPEN)SelectObject(hdc,(HPEN)penLine);

	if(m_eSize==NUMERICSIZE_1)
	{
		if(m_pcLimitHigh_Up)
			m_pcLimitHigh_Up->Draw(hdc,120,11);
		/*DrawWuLine(pDC,123, 23,127, 23, NUMERICLIMIT_COL);
		DrawWuLine(pDC,127, 23,133, 14, NUMERICLIMIT_COL);
		DrawWuLine(pDC,133, 14, 145, 14, NUMERICLIMIT_COL);

		DrawWuLine(pDC,139, 15,133, 23, NUMERICLIMIT_COL);
		DrawWuLine(pDC,133, 23, 145, 23, NUMERICLIMIT_COL);
		DrawWuLine(pDC,145, 23, 139, 15, NUMERICLIMIT_COL);*/
	}
	//else //if(m_eSize==NUMERICSIZE_2)
	//{
	//	/*if(m_pcLimitHigh_Up)
	//		m_pcLimitHigh_Up->Draw(hdc,120,60);*/

	//	/*if(m_pcLimitLow_Up)
	//		m_pcLimitLow_Up->Draw(hdc,120,95);*/

	//	
	//}

	SetBkMode(hdc,nBkMode);
	SelectObject(hdc,hpenprev);

	penLine.DeleteObject();
}

/**********************************************************************************************//**
 * Draw low limit
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param [in,out]	pDC	If non-null, the device-context.
 **************************************************************************************************/

void CNumericField::drawLowLimit(CDC* pDC)
{
	HDC hdc = *pDC;
	int nBkMode=SetBkMode(hdc,TRANSPARENT);
	CPen penLine;
	penLine.CreatePen(PS_SOLID,2,NUMERICLIMIT_COL);
	HPEN hpenprev=(HPEN)SelectObject(hdc,(HPEN)penLine);

	if(m_eSize==NUMERICSIZE_1)
	{
		if(m_pcLimitLow_Up)
			m_pcLimitLow_Up->Draw(hdc,120,44);
		/*DrawWuLine(pDC,123,56,134,56, NUMERICLIMIT_COL);
		DrawWuLine(pDC,134,56,140,47, NUMERICLIMIT_COL);
		DrawWuLine(pDC,140,47,145,47, NUMERICLIMIT_COL);

		DrawWuLine(pDC,129, 55,134,47, NUMERICLIMIT_COL);
		DrawWuLine(pDC,134, 47,123,47, NUMERICLIMIT_COL);
		DrawWuLine(pDC,123, 47,129,55, NUMERICLIMIT_COL);*/

	}
	//else //if(m_eSize==NUMERICSIZE_2)
	//{
	//}

	SetBkMode(hdc,nBkMode);
	SelectObject(hdc,hpenprev);

	penLine.DeleteObject();
}
