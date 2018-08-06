/***************************************************************************************/
/***************************************************************************************/
/**                                                                                   **/
/**    F&S Elektronik Systeme GmbH                                                    **/
/**                                                                                   **/
/**    Project:      intern                                                           **/
/**    Filename:     DIO.h                                                            **/
/**    Author:       Christian Zutter                                                 **/
/**    Release:      0.0                                                              **/
/**                                                                                   **/
/**    19.12.05																		  **/
/**                                                                                   **/
/**                                                                                   **/
/***************************************************************************************/
/***************************************************************************************/

/*- Preprocessor directives and compiler switches -------------------------------------*/
#if !defined(__DIO_H__)
#define __DIO_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*- files at system folder ------------------------------------------------------------*/
#include <windows.h>

/////////////////////////////////////////////////////////////////////////////////////////
class CDIO
{
// Operations
public:
	virtual ~CDIO();
	CDIO();
	bool Init(void);
public:
	DWORD ReadPin (BYTE* pbyData,int nPort,BYTE chPin);
	BYTE ReadPin (int nPort,BYTE chPin);
	DWORD WritePin (BYTE chPin);
	DWORD ClearPin (BYTE chPin);

	DWORD Read (/*int nPort,*/BYTE* pbyData);
	DWORD Read3Bit (/*int nPort,*/BYTE* pbyData);
	DWORD Write (/*int nPort,*/BYTE byData);
	DWORD Write3Bit (bool bPin11,bool bPin13,bool bPin15);

	bool CreateISR(void);
	bool WaitISR(void);
	void ISRDone(void);
	bool EndISR(void);
	bool SetISR(void);

// Attributes
private:
	HANDLE m_hDIO;
	CRITICAL_SECTION csDIO;
	HANDLE m_hIsrEvent;
	DWORD m_dwSysIntr;
};

/////////////////////////////////////////////////////////////////////////////////////////

#endif // !defined(__DIO_H__)
