/**********************************************************************************************//**
 * @file	WndWrapper.cpp.
 *
 * Implements the window wrapper class.
 **************************************************************************************************/

#include "WndWrapper.h"

/**********************************************************************************************//**
 * Initializes a new instance of the CWndWrapper class.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

CWndWrapper::CWndWrapper(void)
{
	pView=NULL;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CWndWrapper class.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

CWndWrapper::~CWndWrapper(void)
{

}

/**********************************************************************************************//**
 * Posts a message.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 *
 * @param	Msg   	The message.
 * @param	wParam	The wParam field of the message.
 * @param	lParam	The lParam field of the message.
 **************************************************************************************************/

void CWndWrapper::PostMessage(UINT Msg,WPARAM wParam,LPARAM lParam)
{
	if(pView)
	{
		pView->PostMessage(Msg,wParam,lParam);
	}
}

/**********************************************************************************************//**
 * Gets the model.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 *
 * @return	Null if it fails, else the model.
 **************************************************************************************************/

CMVModel *CWndWrapper::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}