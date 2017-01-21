
#pragma once
#include "interface.h"
#include "globDefs.h"
//#include "parintfsdk.h"

class CInterfaceDIO :
	public CInterface
{
public:
	

	static CInterfaceDIO* GetInstance();
	static void DestroyInstance();

	BOOL Init();




	DWORD ReadPin (BYTE* pbyData,int nPort,BYTE chPin);
	BYTE ReadPin (int nPort,BYTE chPin);
	DWORD WritePin (BYTE chPin);
	DWORD ClearPin (BYTE chPin);

	DWORD Read (/*int nPort,*/BYTE* pbyData);
	DWORD Read3Bit (/*int nPort,*/BYTE* pbyData);
	DWORD Write (/*int nPort,*/BYTE byData);
	DWORD Write3Bit (bool bPin11,bool bPin13,bool bPin15);

	/*bool CreateISR(void);
	bool WaitISR(void);
	void ISRDone(void);
	bool EndISR(void);
	bool SetISR(void);*/

private:
	//singleton
	static CInterfaceDIO* theDIOInterface;

protected:
	CInterfaceDIO(void);
	virtual ~CInterfaceDIO(void);

	//void StartDIOThread(void);
	//void StopDIOThread( void );

	


protected:

	HANDLE m_hDIO;
	CRITICAL_SECTION csIFDIO;
	
};


