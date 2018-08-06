//=============================================================================
/** 
* \class CInterfaceFSBus
* 		 
* \brief Interface for parallel bus (FS bus).
* 		 
* \author Rainer Kuehner
* \date 2017-10-23 Creation of file
*
**/
//=============================================================================


#pragma once
#include "interface.h"
#include "globDefs.h"
#include "parintfsdk.h"

class CInterfaceFSBus :
	public CInterface
{
public:
	
	static CInterfaceFSBus* GetInstance();
	static void DestroyInstance();

	BOOL Init();

	/*void StartPIFThread(void);
	void StopPIFThread( void );*/

	DWORD SendData(unsigned char* data);
	DWORD SendData(BYTE*  pData, DWORD dwBytesToWrite);
	CStringW GetDriverInfo();


private:
	//singleton
	static CInterfaceFSBus* theFSBusInterface;

protected:
	CInterfaceFSBus(void);
	virtual ~CInterfaceFSBus(void);

	


	


protected:

	HANDLE m_hPIF;

	bool m_bDoPIFThread;


	//int m_iCnt;

};


