#include "StdAfx.h"
#include "InterfaceSocket.h"
#include "MVModel.h"

CInterfaceSocket* CInterfaceSocket::theSocketInterface=0;

CInterfaceSocket::CInterfaceSocket(void)
{
}


CInterfaceSocket::~CInterfaceSocket(void)
{
}
// **************************************************************************
// 
// **************************************************************************
CInterfaceSocket* CInterfaceSocket::GetInstance()
{
	if(theSocketInterface == 0)
	{
		theSocketInterface = new CInterfaceSocket;
	}
	return theSocketInterface;
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceSocket::DestroyInstance()
{
	delete theSocketInterface;
	theSocketInterface = NULL;
}


// **************************************************************************
// 
// **************************************************************************
bool CInterfaceSocket::Init()
{
	
	return true;
}



// **************************************************************************
// 
// **************************************************************************
void CInterfaceSocket::Deinit()
{
	
}