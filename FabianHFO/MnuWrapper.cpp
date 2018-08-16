/**********************************************************************************************//**
 * @file	MnuWrapper.cpp.
 *
 * Implements the mnu wrapper class.
 **************************************************************************************************/

#include "MnuWrapper.h"

/**********************************************************************************************//**
 * Initializes a new instance of the CMnuWrapper class.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

CMnuWrapper::CMnuWrapper(void)
{
	pMenu=NULL;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CMnuWrapper class.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

CMnuWrapper::~CMnuWrapper(void)
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

void CMnuWrapper::PostMessage(UINT Msg,WPARAM wParam,LPARAM lParam)
{
	if(pMenu)
	{
		pMenu->PostMessage(Msg,wParam,lParam);
	}
}
